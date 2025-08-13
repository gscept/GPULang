//------------------------------------------------------------------------------
//  @file validator.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "validator.h"
#include "ast/alias.h"
#include "ast/samplerstate.h"
#include "ast/function.h"
#include "ast/program.h"
#include "ast/renderstate.h"
#include "ast/structure.h"
#include "ast/enumeration.h"
#include "ast/variable.h"
#include "ast/generate.h"

#include "ast/expressions/binaryexpression.h"
#include "ast/expressions/commaexpression.h"
#include "ast/expressions/arrayindexexpression.h"
#include "ast/expressions/symbolexpression.h"
#include "ast/expressions/callexpression.h"
#include "ast/expressions/ternaryexpression.h"
#include "ast/expressions/initializerexpression.h"
#include "ast/expressions/accessexpression.h"
#include "ast/expressions/accessexpression.h"
#include "ast/expressions/unaryexpression.h"
#include "ast/expressions/intexpression.h"
#include "ast/expressions/intvecexpression.h"
#include "ast/expressions/floatexpression.h"
#include "ast/expressions/floatvecexpression.h"
#include "ast/expressions/uintexpression.h"
#include "ast/expressions/uintvecexpression.h"
#include "ast/expressions/enumexpression.h"

#include "ast/statements/breakstatement.h"
#include "ast/statements/continuestatement.h"
#include "ast/statements/expressionstatement.h"
#include "ast/statements/forstatement.h"
#include "ast/statements/ifstatement.h"
#include "ast/statements/returnstatement.h"
#include "ast/statements/terminatestatement.h"
#include "ast/statements/scopestatement.h"
#include "ast/statements/switchstatement.h"
#include "ast/statements/whilestatement.h"

#include "compiler.h"
#include "util.h"
#include <algorithm>

#include "ast/expressions/arrayinitializerexpression.h"
#include "ast/expressions/boolvecexpression.h"
#include "ast/expressions/stringexpression.h"
#include "ast/statements/discardstatement.h"

#include "generated/types.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
static StaticSet scalarQualifiers =
std::array{
    "const"_c, "var"_c
};

static StaticSet bindingQualifiers =
std::array{
    "group"_c, "binding"_c
};

static StaticSet functionAttributes =
std::array{
    "entry_point"_c, "local_size_x"_c, "local_size_y"_c, "local_size_z"_c, "local_size"_c, "threads_x"_c, "threads_y"_c, "threads_z"_c, "threads"_c
    , "early_depth"_c, "depth_greater"_c, "depth_lesser"_c
    , "group_size"_c, "groups_per_workgroup"_c
    , "input_vertices"_c, "max_output_vertices"_c, "winding"_c
    , "input_topology"_c, "output_topology"_c, "patch_type"_c, "partition"_c
    , "pixel_origin"_c
    , "prototype"_c
    , "derivative_index_linear"_c, "derivative_index_quad"_c
};

static StaticSet parameterAccessFlags =
std::array{
    "in"_c, "out"_c, "ray_payload"_c, "ray_hit_attribute"_c, "ray_callable_data"_c
};

static StaticSet parameterQualifiers =
std::array{
    "patch"_c, "no_interpolate"_c, "no_perspective"_c, "binding"_c
};

static StaticSet structureQualifiers =
std::array{
    "packed"_c
};

static StaticSet pixelShaderInputQualifiers =
std::array{
    "binding"_c, "no_interpolate"_c, "no_perspective"_c
};

static StaticSet attributesRequiringEvaluation =
std::array{
    "binding"_c, "group"_c, "local_size_x"_c, "local_size_y"_c, "local_size_z"_c, "local_size"_c, "threads_x"_c, "threads_y"_c, "threads_z"_c, "threads"_c
    , "group_size"_c, "groups_per_workgroup"_c
    , "input_vertices"_c, "max_output_vertices"_c, "winding"_c
    , "input_topology"_c, "output_topology"_c, "patch_type"_c, "patch_size"_c, "partition"_c
};

static StaticSet pointerQualifiers =
std::array{
    "no_read"_c, "atomic"_c, "volatile"_c
};

static StaticSet storageQualifiers =
std::array{
    "uniform"_c, "inline"_c, "workgroup"_c, "device"_c, "link_defined"_c
};

static StaticSet textureQualifiers =
std::array{
    "sampled"_c
};

//------------------------------------------------------------------------------
/**
*/
Validator::Validator() 
    : resourceIndexingMode(ResourceIndexingByGroup)
    , defaultGroup(0)
{
    // add formats
    for (auto it : StringToFormats)
    {
        this->allowedTextureAttributes.Insert(FixedString(it.first));
    }

    this->allowedTextureAttributes.Insert(bindingQualifiers);
    this->allowedTextureAttributes.Insert(pointerQualifiers);
    this->allowedTextureAttributes.Insert(storageQualifiers);
    this->allowedTextureAttributes.Insert(textureQualifiers);

    this->allowedScalarAttributes.Insert(scalarQualifiers);
    this->allowedScalarAttributes.Insert(storageQualifiers);

    this->allowedSamplerAttributes.Insert(bindingQualifiers);
    this->allowedSamplerAttributes.Insert(storageQualifiers);

    this->allowedPointerAttributes.Insert(pointerQualifiers);
    this->allowedPointerAttributes.Insert(bindingQualifiers);
    this->allowedPointerAttributes.Insert(storageQualifiers);

    this->allowedFunctionAttributes.Insert(functionAttributes);

    this->allowedParameterAttributes.Insert(parameterQualifiers);
    this->allowedParameterAttributes.Insert(parameterAccessFlags);
    this->allowedParameterAttributes.Insert(pointerQualifiers);
    this->allowedParameterAttributes.Insert(storageQualifiers);

    this->allowedStructureAttributes.Insert(structureQualifiers);
}

//------------------------------------------------------------------------------
/**
*/
bool 
Validator::Resolve(Compiler* compiler, Symbol* symbol)
{
    switch (symbol->symbolType)
    {
    case Symbol::SymbolType::FunctionType:
        return this->ResolveFunction(compiler, symbol);
        break;
    case Symbol::SymbolType::AliasType:
        return this->ResolveAlias(compiler, symbol);
        break;
    case Symbol::SymbolType::ProgramInstanceType:
        return this->ResolveProgram(compiler, symbol);
        break;
    case Symbol::SymbolType::RenderStateInstanceType:
        return this->ResolveRenderState(compiler, symbol);
        break;
    case Symbol::SymbolType::StructureType:
        return this->ResolveStructure(compiler, symbol);
        break;
    case Symbol::SymbolType::EnumerationType:
        return this->ResolveEnumeration(compiler, symbol) && this->ResolveTypeMethods(compiler, symbol);
        break;
    case Symbol::SymbolType::VariableType:
        return this->ResolveVariable(compiler, symbol);
        break;
    case Symbol::SymbolType::SamplerStateInstanceType:
        return this->ResolveSamplerState(compiler, symbol);
        break;
    case Symbol::SymbolType::GenerateType:
        return this->ResolveGenerate(compiler, symbol);
        break;
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
Validator::ResolveAlias(Compiler* compiler, Symbol* symbol)
{
    Alias* alias = static_cast<Alias*>(symbol);
    Symbol* sym = compiler->GetType(Type::FullType{alias->type});
    if (sym == nullptr)
    {
        compiler->UnrecognizedTypeError(alias->type, alias);
        return false;
    }
    return compiler->AddSymbol(alias->name, sym);
}

//------------------------------------------------------------------------------
/**
*/
bool 
Validator::ResolveType(Compiler* compiler, Symbol* symbol)
{
    if (symbol->symbolType == Symbol::SymbolType::EnumerationType)
    {
        if (!this->ResolveEnumeration(compiler, symbol))
            return false;
    }
    else if (symbol->symbolType == Symbol::SymbolType::StructureType)
    {
        if (!this->ResolveStructure(compiler, symbol))
            return false;
    }
    else if (symbol->symbolType == Symbol::SymbolType::TypeType)
    {
        if (!compiler->AddSymbol(symbol->name, symbol))
            return false;
    }
    else if (symbol->symbolType != Symbol::SymbolType::FunctionType)
    {
        if (!compiler->AddSymbol(symbol->name, symbol))
            return false;
    }

    Type* type = static_cast<Type*>(symbol);
    Compiler::LocalScope scope = Compiler::LocalScope::MakeLocalScope(compiler, &type->scope);

    for (Symbol* sym : type->symbols)
    {
        if (sym->symbolType == Symbol::SymbolType::VariableType)
        {
            Variable* var = static_cast<Variable*>(sym);
            if (!this->ResolveVariable(compiler, var))
                return false;
        }
        else if (sym->symbolType == Symbol::SymbolType::EnumerationType)
        {
            if (!this->ResolveEnumeration(compiler, sym))
                return false;
        }
        else if (sym->symbolType == Symbol::SymbolType::StructureType)
        {
            if (!this->ResolveStructure(compiler, sym))
                return false;
        }
        else if (sym->symbolType == Symbol::SymbolType::TypeType)
        {
            if (!this->ResolveType(compiler, symbol))
                return false;
        }
    }

    for (Symbol* sym : type->staticSymbols)
    {
        if (sym->symbolType == Symbol::SymbolType::VariableType)
        {
            Variable* var = static_cast<Variable*>(sym);
            if (!this->ResolveVariable(compiler, var))
                return false;
        }
        else if (sym->symbolType == Symbol::SymbolType::EnumerationType)
        {
            Enumeration* en = static_cast<Enumeration*>(sym);
            en->scope.symbolLookup.Invalidate();
            en->symbols.Invalidate();
            if (!this->ResolveEnumeration(compiler, sym))
                return false;
        }
        else if (sym->symbolType == Symbol::SymbolType::StructureType)
        {
            Structure* struc = static_cast<Structure*>(sym);
            struc->scope.symbolLookup.Invalidate();
            struc->symbols.Invalidate();
            if (!this->ResolveStructure(compiler, sym))
                return false;
        }
        else if (sym->symbolType == Symbol::SymbolType::TypeType)
        {
            if (!this->ResolveType(compiler, symbol))
                return false;
        }
    }


    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
Validator::ResolveTypeMethods(Compiler* compiler, Symbol* symbol)
{
    Type* type = static_cast<Type*>(symbol);

    for (Symbol* sym : type->globals)
    {
        if (sym->symbolType == Symbol::SymbolType::FunctionType)
        {
            Function* fun = static_cast<Function*>(sym);
            Function::__Resolved* funRes = Symbol::Resolved(fun);
            funRes->scope.symbols.Invalidate();
            funRes->scope.symbolLookup.Invalidate();
            if (!this->ResolveFunction(compiler, fun))
                return false;
        }
    }

    Compiler::LocalScope scope = Compiler::LocalScope::MakeLocalScope(compiler, &type->scope);

    for (Symbol* sym : type->symbols)
    {
        if (sym->symbolType == Symbol::SymbolType::FunctionType)
        {
            Function* fun = static_cast<Function*>(sym);
            Function::__Resolved* funRes = Symbol::Resolved(fun);
            funRes->scope.symbols.Invalidate();
            funRes->scope.symbolLookup.Invalidate();
            if (!this->ResolveFunction(compiler, fun))
                return false;
        }
    }    

    for (Symbol* sym : type->staticSymbols)
    {
        if (sym->symbolType == Symbol::SymbolType::FunctionType)
        {
            Function* fun = static_cast<Function*>(sym);
            Function::__Resolved* funRes = Symbol::Resolved(fun);
            funRes->scope.symbols.Invalidate();
            funRes->scope.symbolLookup.Invalidate();
            if (!this->ResolveFunction(compiler, fun))
                return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
Validator::ResolveTypeSwizzles(Compiler* compiler, Symbol* symbol)
{
    Type* type = static_cast<Type*>(symbol);
    Compiler::LocalScope scope = Compiler::LocalScope::MakeLocalScope(compiler, &type->scope);

    for (Symbol* sym : type->swizzleSymbols)
    {
        if (sym->symbolType == Symbol::SymbolType::VariableType)
        {
            Variable* var = static_cast<Variable*>(sym);
            if (!this->ResolveVariable(compiler, var))
                return false;
        }
    }  
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
Validator::ResolveSamplerState(Compiler* compiler, Symbol* symbol)
{
    SamplerStateInstance* state = static_cast<SamplerStateInstance*>(symbol);
    SamplerStateInstance::__Resolved* stateResolved = Symbol::Resolved(state);

    if (!compiler->AddSymbol(symbol->name, symbol))
        return false;

    stateResolved->isInline = state->isInline;
    stateResolved->isImmutable = state->isImmutable;

    Type* samplerStateType = &SamplerStateType;
    stateResolved->typeSymbol = samplerStateType;
    Compiler::LocalScope scope = Compiler::LocalScope::MakeLocalScope(compiler, &samplerStateType->scope);

    stateResolved->group = this->defaultGroup;

    // run attribute validation
    for (const Attribute* attr : state->attributes)
    {
        if (attr->expression != nullptr)
            attr->expression->Resolve(compiler);

        ValueUnion val;
        // resolve attributes
        if (attr->name == "group")
        {
            if (!attr->expression->EvalValue(val))
            {
                compiler->Error(Format("Expected compile time constant for 'group' qualifier"), symbol);
                return false;
            }
            val.Store(stateResolved->group);
        }
        else if (attr->name == "binding")
        {
            if (!attr->expression->EvalValue(val))
            {
                compiler->Error(Format("Expected compile time constant for 'binding' qualifier"), symbol);
                return false;
            }
            val.Store(stateResolved->binding);
        }
        else
        {
            compiler->Error(Format("Invalid sampler_state attribute '%s'", attr->name.c_str()), symbol);
            return false;
        }
    }
    
    Type::Category cat = samplerStateType->category;
    if (this->resourceIndexingMode == ResourceIndexingByType)
    {
        auto it = this->resourceIndexCounter.Find(cat);
        if (it == this->resourceIndexCounter.end())
        {
            this->resourceIndexCounter.Insert(cat, 0);
            it = this->resourceIndexCounter.Find(stateResolved->group);
        }

        if (stateResolved->binding == Variable::__Resolved::NOT_BOUND)
        {
            stateResolved->binding = it->second++;
        }
        else
        {
            it->second = max(it->second, stateResolved->binding + 1);
        }
    }
    else if (this->resourceIndexingMode == ResourceIndexingByGroup)
    {
        auto it = this->resourceIndexCounter.Find(stateResolved->group);
        if (it == this->resourceIndexCounter.end())
        {
            this->resourceIndexCounter.Insert(stateResolved->group, 0);
            it = this->resourceIndexCounter.Find(stateResolved->group);
        }

        if (stateResolved->binding == Variable::__Resolved::NOT_BOUND)
        {
            stateResolved->binding = it->second++;
        }
        else
        {
            it->second = max(it->second, stateResolved->binding + 1);
        }
        auto it2 = this->resourceTypePerGroupAndBinding.Find(stateResolved->group);
        if (it2 == this->resourceTypePerGroupAndBinding.end())
        {
            PinnedMap<uint32_t, Type::Category> table = 0xFFF;
            table.Insert(stateResolved->binding, samplerStateType->category);
            this->resourceTypePerGroupAndBinding.Insert(stateResolved->group, table);
        }
        else
        {
            PinnedMap<uint32_t, Type::Category>& table = it2->second;
            auto it3 = table.Find(stateResolved->binding);
            if (it3 == table.end())
            {
                table.Insert(stateResolved->binding, samplerStateType->category);
            }
            else
            {
                if (it3->second != samplerStateType->category)
                {
                    compiler->Error(Format("Aliasing is only allowed on resource pointers of same type. First declared as '%s' can't be aliased as '%s'", Type::CategoryToString(samplerStateType->category).c_str(), Type::CategoryToString(it3->second).c_str()), state);
                    return false;
                }
            }
        }
    }

    for (Expression* entry : state->entries)
    {
        if (!entry->Resolve(compiler))
            return false;
        const BinaryExpression* assignEntry = static_cast<const BinaryExpression*>(entry);

        if (assignEntry->symbolType != Symbol::BinaryExpressionType || assignEntry->op != '=')
        {
            compiler->Error(Format("Sampler state entry '%s' must be an assignment expression", assignEntry->EvalString().c_str()), assignEntry);
            return false;
        }

        // check if lhs is binary expression, then check if array access
        if (assignEntry->left->symbolType != Symbol::SymbolExpressionType)
        {
            compiler->Error(Format("Sampler state entry '%s' must be a valid identifier", assignEntry->left->EvalString().c_str()), assignEntry->left);
            return false;
        }

        TransientString entryString = assignEntry->left->EvalString();
        SamplerStateInstance::__Resolved::SamplerStateEntryType entryType = SamplerStateInstance::__Resolved::StringToEntryType(entryString.c_str());
        if (entryType == SamplerStateInstance::__Resolved::InvalidSamplerStateEntryType)
        {
            compiler->Error(Format("Invalid sampler state entry '%s'", entryString.c_str()), assignEntry);
            return false;
        }

        ValueUnion value;
        assignEntry->right->EvalValue(value);
        switch (entryType)
        {
        case SamplerStateInstance::__Resolved::AllAddressType:
            stateResolved->addressU = stateResolved->addressV = stateResolved->addressW = (GPULang::Serialization::AddressMode)value.i[0];
            break;
        case SamplerStateInstance::__Resolved::AddressUType:
            if (stateResolved->isInline)
            {
                compiler->Error(Format("inline_sampler doesn't support per-coordinate addressing mode"), assignEntry);
                return false;
            }
            stateResolved->addressU = (GPULang::Serialization::AddressMode)value.i[0];
            break;
        case SamplerStateInstance::__Resolved::AddressVType:
            if (stateResolved->isInline)
            {
                compiler->Error(Format("inline_sampler doesn't support per-coordinate addressing mode"), assignEntry);
                return false;
            }
            stateResolved->addressV = (GPULang::Serialization::AddressMode)value.i[0];
            break;
        case SamplerStateInstance::__Resolved::AddressWType:
            if (stateResolved->isInline)
            {
                compiler->Error(Format("inline_sampler doesn't support per-coordinate addressing mode"), assignEntry);
                return false;
            }
            stateResolved->addressW = (GPULang::Serialization::AddressMode)value.i[0];
            break;
        case SamplerStateInstance::__Resolved::AllFilterType:
            if (stateResolved->isInline && (value.i[0] != 0x7 && value.i[0] != 0x0))
            {
                compiler->Error(Format("inline_sampler requires filter mode to either be FilterMode.Linear or FilterMode.Point"), assignEntry);
                return false;
            }
            stateResolved->minFilter = (GPULang::Serialization::Filter)((value.i[0] & 0x1) + 1);
            stateResolved->magFilter = (GPULang::Serialization::Filter)(((value.i[0] >> 1) & 0x1) + 1);
            stateResolved->mipFilter = (GPULang::Serialization::Filter)(((value.i[0] >> 2) & 0x1) + 1);
            break;
        case SamplerStateInstance::__Resolved::MinFilterType:
            if (stateResolved->isInline)
            {
                compiler->Error(Format("inline_sampler doesn't support setting filter modes individually"), assignEntry);
                return false;
            }
            stateResolved->minFilter = (GPULang::Serialization::Filter)value.i[0];
            break;
        case SamplerStateInstance::__Resolved::MagFilterType:
            if (stateResolved->isInline)
            {
                compiler->Error(Format("inline_sampler doesn't support setting filter modes individually"), assignEntry);
                return false;
            }
            stateResolved->magFilter = (GPULang::Serialization::Filter)value.i[0];
            break;
        case SamplerStateInstance::__Resolved::MipFilterType:
            if (stateResolved->isInline)
            {
                compiler->Error(Format("inline_sampler doesn't support setting filter modes individually"), assignEntry);
                return false;
            }
            stateResolved->mipFilter = (GPULang::Serialization::Filter)value.i[0];
            break;
        case SamplerStateInstance::__Resolved::MipLodBiasType:
            if (stateResolved->isInline)
            {
                compiler->Error(Format("inline_sampler doesn't support mip lod bias"), assignEntry);
                return false;
            }
            value.Store(stateResolved->mipLodBias);
            break;
        case SamplerStateInstance::__Resolved::AnisotropicFlagType:
            if (stateResolved->isInline)
            {
                compiler->Error(Format("inline_sampler doesn't support anisotropy"), assignEntry);
                return false;
            }
            value.Store(stateResolved->anisotropicEnabled);
            break;
        case SamplerStateInstance::__Resolved::MaxAnisotropyType:
            if (stateResolved->isInline)
            {
                compiler->Error(Format("inline_sampler doesn't support anisotropy"), assignEntry);
                return false;
            }
            value.Store(stateResolved->maxAnisotropy);
            break;
        case SamplerStateInstance::__Resolved::CompareFlagType:
            if (stateResolved->isInline)
            {
                compiler->Error(Format("inline_sampler doesn't support comparison samplers"), assignEntry);
                return false;
            }
            value.Store(stateResolved->compareSamplerEnabled);
            break;
        case SamplerStateInstance::__Resolved::CompareModeType:
            if (stateResolved->isInline)
            {
                compiler->Error(Format("inline_sampler doesn't support comparison samplers"), assignEntry);
                return false;
            }
            stateResolved->compareMode = (GPULang::Serialization::CompareMode)value.i[0];
            break;
        case SamplerStateInstance::__Resolved::MinLodType:
            if (stateResolved->isInline)
            {
                compiler->Error(Format("inline_sampler doesn't support lod controls"), assignEntry);
                return false;
            }
            value.Store(stateResolved->minLod);
            break;
        case SamplerStateInstance::__Resolved::MaxLodType:
            if (stateResolved->isInline)
            {
                compiler->Error(Format("inline_sampler doesn't support lod controls"), assignEntry);
                return false;
            }
            value.Store(stateResolved->maxLod);
            break;
        case SamplerStateInstance::__Resolved::BorderColorType:
            if (stateResolved->isInline)
            {
                compiler->Error(Format("inline_sampler doesn't support setting border color"), assignEntry);
                return false;
            }
            stateResolved->borderColor = (GPULang::Serialization::BorderColor)value.i[0];
            break;
        case SamplerStateInstance::__Resolved::UnnormalizedSamplingType:
            value.Store(stateResolved->unnormalizedSamplingEnabled);
            break;
        }
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
Validator::ResolveFunction(Compiler* compiler, Symbol* symbol)
{
    Function* fun = static_cast<Function*>(symbol);
    Function::__Resolved* funResolved = Symbol::Resolved(fun);
    funResolved->scope.symbolLookup.Clear();

     // run attribute validation
    for (const Attribute* attr : fun->attributes)
    {
        if (attr->expression != nullptr)
        {
            if (!attr->expression->Resolve(compiler))
                return false;
        }
        if (!set_contains(this->allowedFunctionAttributes, attr->name))
        {
            compiler->Error(Format("Invalid attribute for function: '%s'", attr->name.c_str()), symbol);
            return false;
        }

        // make sure attribute has expression
        if (attr->expression == nullptr && set_contains(attributesRequiringEvaluation, attr->name))
        {
            compiler->Error(Format("Attribute '%s' requires value but none provided", attr->name.c_str()), symbol);
            return false;
        }

        ValueUnion val;
        if (attr->name == "entry_point")
        {
            funResolved->isEntryPoint = true;
        }
        else if (attr->name == "local_size_x" || attr->name == "threads_x")
        {
            if (!attr->expression->EvalValue(val))
            {
                compiler->Error(Format("Value '%s' has to be a compile time constant", attr->name.c_str()), symbol);
                return false;
            }
            val.Store(funResolved->executionModifiers.computeShaderWorkGroupSize[0]);
        }
        else if (attr->name == "local_size_y" || attr->name == "threads_y")
        {
            if (!attr->expression->EvalValue(val))
            {
                compiler->Error(Format("Value '%s' has to be a compile time constant", attr->name.c_str()), symbol);
                return false;
            }
            val.Store(funResolved->executionModifiers.computeShaderWorkGroupSize[1]);
        }
        else if (attr->name == "local_size_z" || attr->name == "threads_z")
        {
            if (!attr->expression->EvalValue(val))
            {
                compiler->Error(Format("Value '%s' has to be a compile time constant", attr->name.c_str()), symbol);
                return false;
            }
            val.Store(funResolved->executionModifiers.computeShaderWorkGroupSize[2]);
        }
        else if (attr->name == "local_size" || attr->name == "threads")
        {
            if (!attr->expression->EvalValue(val))
            {
                compiler->Error(Format("Value '%s' has to be a compile time vector", attr->name.c_str()), symbol);
                return false;
            }
            if (val.columnSize != 3 || val.rowSize != 1)
            {
                compiler->Error(Format("Value '%s' has to be a compile time vector of 3", attr->name.c_str()), symbol);
                return false;
            }
            val.Convert(TypeCode::UInt);
            funResolved->executionModifiers.computeShaderWorkGroupSize[0] = val.ui[0];
            funResolved->executionModifiers.computeShaderWorkGroupSize[1] = val.ui[1];
            funResolved->executionModifiers.computeShaderWorkGroupSize[2] = val.ui[2];
        }
        else if (attr->name == "group_size")
        {
            if (!attr->expression->EvalValue(val))
            {
                compiler->Error(Format("Value '%s' has to be a compile time constant", attr->name.c_str()), symbol);
                return false;
            }
            val.Store(funResolved->executionModifiers.groupSize);
        }
        else if (attr->name == "groups_per_workgroup")
        {
            if (!attr->expression->EvalValue(val))
            {
                compiler->Error(Format("Value '%s' has to be a compile time constant", attr->name.c_str()), symbol);
                return false;
            }
            val.Store(funResolved->executionModifiers.groupsPerWorkgroup);
        }
        else if (attr->name == "early_depth")
            funResolved->executionModifiers.earlyDepth = 1;
        else if (attr->name == "depth_greater")
            funResolved->executionModifiers.depthAlwaysGreater = 1;
        else if (attr->name == "depth_less")
            funResolved->executionModifiers.depthAlwaysLesser = 1;
        else if (attr->name == "invocations")
        {
            if (!attr->expression->EvalValue(val))
            {
                compiler->Error(Format("Value '%s' has to be a compile time constant", attr->name.c_str()), symbol);
                return false;
            }
            val.Store(funResolved->executionModifiers.invocations);
        }
        else if (attr->name == "max_output_vertices")
        {
            if (!attr->expression->EvalValue(val))
            {
                compiler->Error(Format("Value '%s' has to be a compile time constant", attr->name.c_str()), symbol);
                return false;
            }
            val.Store(funResolved->executionModifiers.maxOutputVertices);
        }
        else if (attr->name == "winding")
        {
            TransientString str = attr->expression->EvalString();
            funResolved->executionModifiers.windingOrder = Function::__Resolved::WindingOrderFromString(str);
            if (funResolved->executionModifiers.windingOrder == Function::__Resolved::InvalidWindingOrder)
            {
                compiler->Error(Format("Attribute 'winding' supports values: cw/clockwise, ccw/counter_clockwise, but got '%s'", str.c_str()), symbol);
                return false;
            }
        }
        else if (attr->name == "input_topology")
        {
            TransientString str = attr->expression->EvalString();
            funResolved->executionModifiers.inputPrimitiveTopology = Function::__Resolved::PrimitiveTopologyFromString(str);
            if (funResolved->executionModifiers.inputPrimitiveTopology == Function::__Resolved::InvalidPrimitiveTopology)
            {
                compiler->Error(Format("Attribute 'input_topology' supports values: points, lines, lines_adjacency, triangles, triangles_adjacency, but got '%s'", str.c_str()), symbol);
                return false;
            }
        }
        else if (attr->name == "output_topology")
        {
            TransientString str = attr->expression->EvalString();
            funResolved->executionModifiers.outputPrimitiveTopology = Function::__Resolved::PrimitiveTopologyFromString(str);
            if (funResolved->executionModifiers.outputPrimitiveTopology == Function::__Resolved::InvalidPrimitiveTopology
                || funResolved->executionModifiers.outputPrimitiveTopology == Function::__Resolved::LinesAdjacency
                || funResolved->executionModifiers.outputPrimitiveTopology == Function::__Resolved::TrianglesAdjacency)
            {
                compiler->Error(Format("Attribute 'output_topology' supports values: points, lines, triangles, but got '%s'", str.c_str()), symbol);
                return false;
            }
        }
        else if (attr->name == "patch_type")
        {
            TransientString str = attr->expression->EvalString();
            funResolved->executionModifiers.patchType = Function::__Resolved::PatchTypeFromString(str);
            if (funResolved->executionModifiers.patchType == Function::__Resolved::InvalidPatchType)
            {
                compiler->Error(Format("Attribute 'patch_type' supports values: isolines, triangles, quads, but got '%s'", str.c_str()), symbol);
                return false;
            }
        }
        else if (attr->name == "partition")
        {
            TransientString str = attr->expression->EvalString();
            funResolved->executionModifiers.partitionMethod = Function::__Resolved::PartitionMethodFromString(str);
            if (funResolved->executionModifiers.partitionMethod == Function::__Resolved::InvalidPartitionMethod)
            {
                compiler->Error(Format("Attribute 'partition' supports values: steps/integer, even/fract_even, odd/fract_odd, but got '%s'", str.c_str()), symbol);
                return false;
            }
        }
        else if (attr->name == "prototype")
        {
            if (fun->hasBody)
            {
                compiler->Error(Format("Prototype function '%s' may not have a function body", fun->name.c_str()), symbol);
                return false;
            }
            funResolved->isPrototype = true;
        }
        else if (attr->name == "pixel_origin")
        {
            TransientString str = attr->expression->EvalString();
            funResolved->executionModifiers.pixelOrigin = Function::__Resolved::PixelOriginFromString(str);
            if (funResolved->executionModifiers.pixelOrigin == Function::__Resolved::InvalidPixelOrigin)
            {
                compiler->Error(Format("Attribute 'pixel_origin' supports values: lower/lower_left, upper/upper_left, center, but got '%s'", str.c_str()), symbol);
                return false;
            }
        }
        else if (attr->name == "derivative_index_linear")
        {
            funResolved->executionModifiers.computeDerivativeIndexing = Function::__Resolved::DerivativeIndexLinear;
        }
        else if (attr->name == "derivative_index_quad")
        {
            funResolved->executionModifiers.computeDerivativeIndexing = Function::__Resolved::DerivativeIndexQuad;
        }
    }

    // validate attributes
    if (!funResolved->isEntryPoint)
    {
        if (funResolved->executionModifiers.earlyDepth)
        {
            compiler->Error("'early_depth' is only allowed on functions with the 'entry_point' qualifier", symbol);
            return false;
        }
        if (funResolved->executionModifiers.depthAlwaysGreater)
        {
            compiler->Error("'depth_greater' is only allowed on functions with the 'entry_point' qualifier", symbol);
            return false;
        }
        if (funResolved->executionModifiers.depthAlwaysLesser)
        {
            compiler->Error("'depth_lesser' is only allowed on functions with the 'entry_point' qualifier", symbol);
            return false;
        }

        if (
            funResolved->executionModifiers.computeShaderWorkGroupSize[0] > 1
            || funResolved->executionModifiers.computeShaderWorkGroupSize[1] > 1
            || funResolved->executionModifiers.computeShaderWorkGroupSize[2] > 1
            )
        {
            compiler->Error("'local_size_(x/y/z)' is only allowed on functions with the 'entry_point' qualifier", symbol);
            return false;
        }

        if (funResolved->executionModifiers.computeDerivativeIndexing != Function::__Resolved::NoDerivatives)
        {
            compiler->Error("Setting derivative indexing is only allowed on functions with the 'entry_point' qualifier", symbol);
            return false;
        }
    }
    else
    {
        if (fun->returnType != Type::FullType{ ConstantString("void") })
        {
            compiler->Error("Entry point may only return 'void'", symbol);
            return false;
        }
        if (funResolved->executionModifiers.computeDerivativeIndexing == Function::__Resolved::DerivativeIndexLinear)
        {
            uint32_t numThreads = funResolved->executionModifiers.computeShaderWorkGroupSize[0] * funResolved->executionModifiers.computeShaderWorkGroupSize[1] * funResolved->executionModifiers.computeShaderWorkGroupSize[2];
            if (numThreads % 4 != 0)
            {
                compiler->Error("Work group linear derivatives require the work group size to be a multiple of 4", symbol);
                return false;
            }
        }
        else if (funResolved->executionModifiers.computeDerivativeIndexing == Function::__Resolved::DerivativeIndexQuad)
        {
            uint32_t numThreadsX = funResolved->executionModifiers.computeShaderWorkGroupSize[0] % 2;
            uint32_t numThreadsY = funResolved->executionModifiers.computeShaderWorkGroupSize[1] % 2;
            if (numThreadsX != 0 && numThreadsY != 0)
            {
                compiler->Error("Work group quad derivatives require the work group size to be evenly divisible by 2 in both X and Y", symbol);
                return false;
            }
        }
    }

    // Push temporary scope to evaluate parameters
    compiler->PushScope(&funResolved->scope);

    // run validation on parameters
    bool rayHitAttributeConsumed = false;
    bool rayPayloadConsumed = false;
    for (Variable* var : fun->parameters)
    {
        Variable::__Resolved* varResolved = Symbol::Resolved(var);
        varResolved->usageBits.flags.isParameter = true;
        varResolved->usageBits.flags.isEntryPointParameter = funResolved->isEntryPoint;
        this->ResolveVariable(compiler, var);

        if (varResolved->storage == Storage::RayHitAttribute)
        {
            if (!rayHitAttributeConsumed)
                rayHitAttributeConsumed = true;
            else
                compiler->Error("Only one parameter is allowed to be of storage class 'ray_hit_attribute'", symbol);

            if (!rayPayloadConsumed)
                rayPayloadConsumed = true;
            else
                compiler->Error("Only one parameter is allowed to be of storage class 'ray_payload'", symbol);
        }
    }

    compiler->PopScope();

    // setup our variables and attributes as sets
    TransientString paramList;
    TransientString paramListNamed;
    for (Variable* var : fun->parameters)
    {
        // add comma if not first argument
        Variable::__Resolved* varResolved = Symbol::Resolved(var);
        if (StorageRequiresSignature(varResolved->storage))
        {
            paramListNamed.Concatenate<true>(StorageToString(varResolved->storage), var->name, ":", var->type.ToString());
            paramList.Concatenate<true>(StorageToString(varResolved->storage), var->type.ToString());
        }
        else
        {
            paramListNamed.Concatenate<true>(var->name, ":", var->type.ToString());
            paramList.Append(var->type.ToString());
        }

        if (var != fun->parameters.back())
        {
            paramList.Append(",");
            paramListNamed.Append(", ");
        }
    }

    TransientString attributeList;

    // make a set of all attributes
    for (const Attribute* attr : fun->attributes)
    {
        TransientString attrAsString;
        if (!attr->ToString(attrAsString))
        {
            compiler->Error(Format("Attribute '%s' can not be evaluated to a compile time value", attr->name.c_str()), symbol);
            return false;
        }
        attributeList.Append(attrAsString);
        attributeList.Append(" ");
        if (attr->name == "prototype")
            funResolved->isPrototype = true;
    }

    // format function with all attributes and parameters
    auto resolvedName = TransientString(fun->name, "(", paramList, ")");
    auto resolvedNameWithParamNames = TransientString(fun->name, "(", paramListNamed, ")");
    auto functionFormatted = TransientString::Compact(attributeList, resolvedName, " ", fun->returnType.name);
    
    funResolved->name = resolvedName;
    funResolved->nameWithVarNames = resolvedNameWithParamNames;
    funResolved->signature = functionFormatted;

    // if prototype, add as an ordinary symbol
    if (funResolved->isPrototype)
    {
        if (!compiler->AddSymbol(fun->name, fun, false))
            return false;
    }
    else
    {
        if (!compiler->staticSymbolSetup)
        {
            // find functions with similar name
            TransientArray<Symbol*> matchingFunctions = compiler->GetSymbols(fun->name);
            for (Symbol* matchingFunction : matchingFunctions)
            {
                if (matchingFunction->symbolType == Symbol::FunctionType)
                {
                    Function* otherFunction = static_cast<Function*>(matchingFunction);

                    if (!fun->IsCompatible(otherFunction, false))
                        continue;

                    // if all checks prove these functions are identical, throw error
                    if (fun->returnType != otherFunction->returnType)
                        compiler->Error(Format("Function '%s' can not be overloaded because it only differs by return type when trying to overload previous definition at %s(%d)", functionFormatted.c_str(), otherFunction->location.file.c_str(), otherFunction->location.line), fun);
                    else
                        compiler->Error(Format("Function '%s' redefinition, previous definition at %s(%d)", functionFormatted.c_str(), otherFunction->location.file.c_str(), otherFunction->location.line), fun);

                    return false;
                }
            }
        }
    }

    // if we didn't fault, add the symbol
    if (!compiler->AddSymbol(TransientString(funResolved->name), fun, false))
        return false;

    // also add the signature for type lookup
    if (!compiler->AddSymbol(TransientString(funResolved->signature), fun, false))
        return false;

    if (fun->constructorType == nullptr)
    {
        if (!compiler->AddSymbol(fun->name, fun, true))
            return false;
    }

    /*
    // Check if the function has a constructor type associated with it
    if (fun->constructorType == nullptr && !compiler->staticSymbolSetup)
    {
        // If not look it up
        Symbol* constructorType = compiler->GetSymbol<Symbol>(fun->name);
        if (constructorType == nullptr || constructorType->symbolType == Type::SymbolType::FunctionType)
        {
            // also add the signature for type lookup
            if (!compiler->AddSymbol(fun->name, fun, true))
                return false;
        }
    }
    else if (fun->constructorType->symbolType == Type::SymbolType::FunctionType)
    {
        // Add constructor
        if (!compiler->AddSymbol(fun->name, fun, true))
            return false;
    }
    */

    if (funResolved->returnTypeSymbol == nullptr)
    {
        Type* type = (Type*)compiler->GetType(fun->returnType);
        if (type == nullptr)
        {
            compiler->UnrecognizedTypeError(TransientString(fun->returnType.name), fun);
            return false;
        }
        funResolved->returnTypeSymbol = type;
    }

    
    Compiler::LocalScope scope = Compiler::LocalScope::MakeLocalScope(compiler, &funResolved->scope);
    
    /*
    // Variables already resolved, just need to add them back to the scope
    for (Variable* var : fun->parameters)
    {
        if (!compiler->AddSymbol(var->name, var))
            return false;
    }
    */

    // before resolving variables (as parameters), reset in and out bindings
    this->inParameterIndexCounter = 0;
    this->outParameterIndexCounter = 0;
    
    // validate function body
    if (
        fun->ast != nullptr
        && !this->ResolveStatement(compiler, fun->ast)
        )
    {
        return false;
    }

    funResolved->hasExplicitReturn = compiler->branchReturns;

    // Reset if the function has a branch return
    compiler->branchReturns = false;

    if (fun->returnType != Type::FullType{ ConstantString("void") } && fun->ast != nullptr && !funResolved->hasExplicitReturn)
    {
        compiler->Error(Format("All paths don't return a value, expected to return value of '%s'", fun->returnType.ToString().c_str()), fun);
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
Validator::ResolveProgram(Compiler* compiler, Symbol* symbol)
{
    ProgramInstance* prog = static_cast<ProgramInstance*>(symbol);
    ProgramInstance::__Resolved* progResolved = Symbol::Resolved(prog);

    Type* progType = &ProgramType;
    Compiler::LocalScope scope = Compiler::LocalScope::MakeLocalScope(compiler, &progType->scope);
    progResolved->typeSymbol = progType;
    for (Expression* entry : prog->entries)
    {
        compiler->currentState.allowConstOverride = true;
        if (!entry->Resolve(compiler))
            return false;

        compiler->currentState.allowConstOverride = false;

        const BinaryExpression* assignEntry = static_cast<const BinaryExpression*>(entry);
        BinaryExpression::__Resolved* binExp = Symbol::Resolved(assignEntry);

        if (entry->symbolType != Symbol::BinaryExpressionType)
        {
            compiler->Error(Format("Program entry '%s' must be an assignment expression", assignEntry->EvalString().c_str()), symbol);
            return false;
        }

        TransientString entryStr = assignEntry->left->EvalString();
        ProgramInstance::__Resolved::EntryType entryType = ProgramInstance::__Resolved::StringToEntryType(entryStr);
        if (entryType == ProgramInstance::__Resolved::InvalidProgramEntryType)
        {
            Symbol* overrideSymbol = compiler->GetSymbol(entryStr);
            if (overrideSymbol->symbolType == Symbol::FunctionType)
            {
                // get all functions responding to this function
                Function* functionStub = static_cast<Function*>(overrideSymbol);

                // check that we actually got a symbol
                if (functionStub == nullptr)
                {
                    compiler->UnrecognizedTypeError(entryStr, symbol);
                    return false;
                }

                // check that it's actually a function
                if (functionStub->symbolType != Symbol::FunctionType)
                {
                    compiler->Error(Format("Symbol '%s' is not a recognized function", entryStr.c_str()), symbol);
                    return false;
                }

                // next up, function to assign
                FixedString functionName;
                if (!assignEntry->right->EvalSymbol(functionName))
                {
                    compiler->Error(Format("Expected symbol, but got '%s'", entryStr.c_str()), symbol);
                    return false;
                }
                TransientArray<Symbol*> functions = compiler->GetSymbols(functionName);

                // again, check if not null
                if (functions.size == 0)
                {
                    compiler->UnrecognizedTypeError(TransientString(functionName), symbol);
                    return false;
                }

                bool matched = false;
                for (Symbol* sym : functions)
                {
                    Function* func = static_cast<Function*>(sym);

                    // and check that it's actually a function
                    if (func->symbolType != Symbol::FunctionType)
                    {
                        compiler->Error(Format("Symbol '%s' is not a recognized function", functionName.c_str()), symbol);
                        return false;
                    }

                    if (functionStub->IsCompatible(func, true))
                    {
                        // if compatible, this is our match
                        progResolved->functionOverrides.Insert(functionStub, func);
                        matched = true;
                    }
                }

                if (!matched)
                {
                    TransientString candidates;
                    for (Symbol* sym : functions)
                    {
                        Function* func = static_cast<Function*>(sym);
                        Function::__Resolved* res = Symbol::Resolved(func);

                        candidates.Append(res->signature);
                        if (sym != functions.back())
                            candidates.Append(",\n");
                    }
                    compiler->Error(Format("Function prototype '%s' can not bind function '%s', possible candidates: \n%s", functionStub->name.c_str(), assignEntry->right->EvalString().c_str(), candidates.c_str()), symbol);
                    return false;
                }
            }
            else if (overrideSymbol->symbolType == Symbol::VariableType)
            {
                Variable* var = static_cast<Variable*>(overrideSymbol);
                Variable::__Resolved* varResolved = Symbol::Resolved(var);
                if (varResolved->storage != Storage::LinkDefined)
                {
                    compiler->Error("Only variables declared as 'link_defined' can be overriden in program assembly", var);
                    return false;
                }
                
                if (!binExp->rightType.literal)
                {
                    compiler->Error("Constant overrides must be literal values", var);
                    return false;
                }
                if (var->type != binExp->rightType)
                {
                    compiler->Error(Format("Trying to assign a value of type '%s' to a link_defined variable of '%s'", binExp->rightType.ToString().c_str(), var->type.ToString().c_str()), var);
                    return false;
                }
                progResolved->constVarInitializationOverrides.Insert(var, assignEntry->right);
            }
        }
        else if (entryType == ProgramInstance::__Resolved::RenderState)
        {
            FixedString sym;
            if (!assignEntry->right->EvalSymbol(sym))
            {
                compiler->Error(Format("Entry '%s' has to be a symbol", entryStr.c_str()), symbol);
                return false;
            }
            Symbol* value = compiler->GetSymbol(sym);
            if (value->symbolType != Symbol::SymbolType::RenderStateInstanceType)
            {
                compiler->Error(Format("Program binds symbol '%s' to '%s' but it is not a recognized render_state", sym.c_str(), assignEntry->name.c_str()), assignEntry);
                return false;
            }
            RenderStateInstance* state = static_cast<RenderStateInstance*>(value);
            progResolved->mappings[entryType] = state;
        }
        else
        {
            // get the symbol for this entry
            FixedString sym;
            if (!assignEntry->right->EvalSymbol(sym))
            {
                compiler->Error(Format("Entry '%s' has to be a symbol", entryStr.c_str()), symbol);
                return false;
            }
            Symbol* value = compiler->GetSymbol(sym);
            if (value->symbolType != Symbol::SymbolType::FunctionType)
            {
                compiler->Error(Format("Program binds symbol '%s' to '%s' but it is not a recognized function", sym.c_str(), assignEntry->name.c_str()), assignEntry);
                return false;
            }
            Function* fun = static_cast<Function*>(value);
            Function::__Resolved* funResolved = Symbol::Resolved(fun);
            if (!funResolved->isEntryPoint)
            {
                compiler->Error(Format("Program binds symbol '%s' to '%s' but it is not qualified as 'entry_point'", sym.c_str(), assignEntry->name.c_str()), assignEntry);
                return false;
            }

            progResolved->mappings[entryType] = value;

            switch (entryType)
            {
            case ProgramInstance::__Resolved::VertexShader:
                progResolved->usage.flags.hasVertexShader = true;
                break;
            case ProgramInstance::__Resolved::HullShader:
                progResolved->usage.flags.hasHullShader = true;
                break;
            case ProgramInstance::__Resolved::DomainShader:
                progResolved->usage.flags.hasDomainShader = true;
                break;
            case ProgramInstance::__Resolved::GeometryShader:
                progResolved->usage.flags.hasGeometryShader = true;
                break;
            case ProgramInstance::__Resolved::PixelShader:
                progResolved->usage.flags.hasPixelShader = true;
                break;
            case ProgramInstance::__Resolved::ComputeShader:
                progResolved->usage.flags.hasComputeShader = true;
                break;
            case ProgramInstance::__Resolved::TaskShader:
                progResolved->usage.flags.hasTaskShader = true;
                break;
            case ProgramInstance::__Resolved::MeshShader:
                progResolved->usage.flags.hasMeshShader = true;
                break;
            case ProgramInstance::__Resolved::RayGenerationShader:
                progResolved->usage.flags.hasRayGenerationShader = true;
                break;
            case ProgramInstance::__Resolved::RayMissShader:
                progResolved->usage.flags.hasRayMissShader = true;
                break;
            case ProgramInstance::__Resolved::RayClosestHitShader:
                progResolved->usage.flags.hasRayClosestHitShader = true;
                break;
            case ProgramInstance::__Resolved::RayAnyHitShader:
                progResolved->usage.flags.hasRayAnyHitShader = true;
                break;
            case ProgramInstance::__Resolved::RayIntersectionShader:
                progResolved->usage.flags.hasRayIntersectionShader = true;
                break;
            case ProgramInstance::__Resolved::RayCallableShader:
                progResolved->usage.flags.hasRayCallableShader = true;
                break;
            case ProgramInstance::__Resolved::RenderState:
                progResolved->usage.flags.hasRenderState = true;
                break;
            }

            // if shader, value must be a function
            if (entryType >= ProgramInstance::__Resolved::EntryType::VertexShader
                && entryType <= ProgramInstance::__Resolved::EntryType::RayIntersectionShader)
            {
                compiler->currentState.shaderType = entryType;
                // when we've set these flags, run function validation to make sure it's properly formatted
                if (!this->ValidateFunction(compiler, fun))
                    return false;

                compiler->currentState.sideEffects.bits = 0x0;
                compiler->shaderValueExpressions[entryType].value = true;
                compiler->currentState.function = fun;
                
                Function::__Resolved* funRes = Symbol::Resolved(fun);
                if (funRes->visibleSymbols.data.capacity == 0)
                    funRes->visibleSymbols = 0xFFFFFF; // initialize lookup

                // Temporarily store original variable values
                StackMap<Variable*, Expression*> originalVariableValues(progResolved->constVarInitializationOverrides.data.size);
                originalVariableValues.BeginBulkAdd();
                auto it = progResolved->constVarInitializationOverrides.begin();
                for (; it != progResolved->constVarInitializationOverrides.end(); it++)
                {
                    originalVariableValues.Insert(it->first, it->first->valueExpression);
                    it->first->valueExpression = it->second;
                }
                originalVariableValues.EndBulkAdd();

                // Resolve visibility
                bool ret = true;
                for (auto& param : fun->parameters)
                    ret |= this->ResolveVisibility(compiler, param);
                ret |= this->ResolveVisibility(compiler, fun->ast);

                if (!ret)
                    return false;

                // Reset variable values
                auto it2 = originalVariableValues.begin();
                for (; it2 != originalVariableValues.end(); it2++)
                {
                    originalVariableValues.Find(it2->first)->second = it2->second;
                }
                
                compiler->shaderValueExpressions[entryType].value = false;
                compiler->currentState.function = nullptr;

                if (entryType == ProgramInstance::__Resolved::VertexShader)
                {
                    if (!compiler->currentState.sideEffects.flags.exportsVertexPosition)
                    {
                        compiler->Error(Format("Vertex shader must call vertexExportCoordinates"), assignEntry);
                        return false;
                    }
                }
                else if (entryType == ProgramInstance::__Resolved::PixelShader)
                {
                    if (!compiler->currentState.sideEffects.flags.exportsPixel && compiler->options.warnOnMissingColorExport)
                    {
                        compiler->Warning(Format("Pixel shader doesn't call pixelExportColor"), assignEntry);
                    }
                    if (compiler->currentState.sideEffects.flags.exportsExplicitDepth)
                    {
                        progResolved->effects.flags.explicitDepth = 1;
                        if (funResolved->executionModifiers.earlyDepth)
                        {
                            compiler->Warning(Format("Pixel shader using 'early_depth' and explicitly setting depth results in undefined behavior"), assignEntry);
                        }
                    }
                }
            }
            else
            {
                if (value->symbolType != Symbol::SymbolType::RenderStateInstanceType)
                {
                    compiler->Error(Format("Program binds symbol '%s' as RenderState but it is not a recognized render_state symbol", assignEntry->name.c_str(), assignEntry->name.c_str()), assignEntry);
                    return false;
                }
            }
        }
    }

    if (!this->ValidateProgram(compiler, prog))
        return false;

    if (progResolved->usage.flags.hasPixelShader && !progResolved->usage.flags.hasRenderState)
    {
        if (compiler->options.warnOnMissingRenderState)
            compiler->Warning(Format("Program is general graphics but does not specify a render state, falling back on the default"), symbol);
        progResolved->mappings[ProgramInstance::__Resolved::EntryType::RenderState] = &compiler->defaultRenderState;
        progResolved->usage.flags.hasRenderState = true;
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
Validator::ResolveRenderState(Compiler* compiler, Symbol* symbol)
{
    RenderStateInstance* state = static_cast<RenderStateInstance*>(symbol);
    RenderStateInstance::__Resolved* stateResolved = Symbol::Resolved(state);

    Type* renderStateType = &RenderStateType;
    stateResolved->typeSymbol = renderStateType;
    if (!compiler->AddSymbol(symbol->name, symbol))
        return false;

    Compiler::LocalScope scope = Compiler::LocalScope::MakeLocalScope(compiler, &renderStateType->scope);
    for (Expression* entry : state->entries)
    {
        if (!entry->Resolve(compiler))
            return false;
        const BinaryExpression* assignEntry = static_cast<const BinaryExpression*>(entry);

        if (assignEntry->symbolType == Symbol::BinaryExpressionType && assignEntry->op != '=')
        {
            compiler->Error(Format("Render state entry '%s' must be assignment", assignEntry->EvalString().c_str()), entry);
            return false;
        }
        
        TransientString entryStr;

        // if left is binary, then validate it is an array expression
        if (assignEntry->left->symbolType == Symbol::ArrayIndexExpressionType)
        {
            const ArrayIndexExpression* lhs = static_cast<const ArrayIndexExpression*>(assignEntry->left);
            if (lhs->right == nullptr)
            {
                compiler->Error(Format("Render state entry '%s' must provide array index", assignEntry->EvalString().c_str()), assignEntry);
                return false;
            }
            else if (lhs->right->symbolType != Symbol::IntExpressionType
                && lhs->right->symbolType != Symbol::UIntExpressionType)
            {
                compiler->Error(Format("Render state entry '%s' may be either identifier or array expression", assignEntry->EvalString().c_str()), assignEntry);
                return false;
            }
            if (lhs->left->symbolType != Symbol::SymbolExpressionType)
            {
                compiler->Error(Format("Render state array entry '%s' must be a valid identifier", lhs->left->EvalString().c_str()), assignEntry);
                return false;
            }
            entryStr = lhs->left->EvalString();
        }
        else if (assignEntry->left->symbolType == Symbol::AccessExpressionType)
        {
            entryStr = assignEntry->left->EvalString();
        }
        else
        {
            if (assignEntry->left->symbolType != Symbol::SymbolExpressionType)
            {
                compiler->Error(Format("Render state entry '%s' must be a valid identifier", assignEntry->left->EvalString().c_str()), assignEntry);
                return false;
            }
            entryStr = assignEntry->left->EvalString();
        }

        RenderStateInstance::__Resolved::RenderStateEntryType entryType = RenderStateInstance::__Resolved::StringToEntryType(entryStr);
        if (entryType == RenderStateInstance::__Resolved::InvalidRenderStateEntryType)
        {
            compiler->Error(Format("Invalid render state entry '%s'", entryStr.c_str()), assignEntry);
            return false;
        }

        // Look at the array states (blend per MRT)
        if (entryType >= RenderStateInstance::__Resolved::BlendEnabledType && entryType <= RenderStateInstance::__Resolved::ColorComponentMaskType)
        {
            uint32_t index = -1;

            // If array expression (blend state) get the index
            if (assignEntry->left->symbolType == Symbol::ArrayIndexExpressionType)
            {
                const ArrayIndexExpression* lhs = static_cast<const ArrayIndexExpression*>(assignEntry->left);
                ValueUnion val;
                if (!lhs->right->EvalValue(val))
                {
                    compiler->Error(Format("Blend state entry '%s' must evaluate at compile time", assignEntry->EvalString().c_str()), assignEntry);
                    return false;    
                }
                val.Store(index);
            }
            else
            {
                compiler->Error(Format("Blend state entry '%s' must be an array accessor", assignEntry->EvalString().c_str()), assignEntry);
                return false;
            }

            if (index >= RenderStateInstance::__Resolved::NUM_BLEND_STATES)
            {
                compiler->Error(Format("Only %d blend states are allowed", RenderStateInstance::__Resolved::NUM_BLEND_STATES), assignEntry);
                return false;
            }

            ValueUnion val;
            bool valid = assignEntry->right->EvalValue(val);
            uint32_t enumValue = 0;
            switch (entryType)
            {
            case RenderStateInstance::__Resolved::BlendEnabledType:
                if (!valid)
                {
                    compiler->Error(Format("Blend state entry '%s' must evaluate to a compile time bool", entryStr.c_str()), assignEntry);
                    return false;
                }
                val.Store(stateResolved->blendStates[index].blendEnabled);
                break;
            case RenderStateInstance::__Resolved::SourceBlendColorFactorType:
                if (!valid)
                {
                    compiler->Error(Format("Source blend factor entry '%s' must evaluate to a compile time enum of BlendFactor", entryStr.c_str()), assignEntry);
                    return false;
                }
                val.Store(enumValue);
                stateResolved->blendStates[index].sourceColorBlendFactor = (GPULang::Serialization::BlendFactor)enumValue;
                break;
            case RenderStateInstance::__Resolved::DestinationBlendColorFactorType:
                if (!valid)
                {
                    compiler->Error(Format("Destination blend factor entry '%s' must evaluate to a compile time enum of BlendFactor", entryStr.c_str()), assignEntry);
                    return false;
                }
                val.Store(enumValue);
                stateResolved->blendStates[index].destinationColorBlendFactor = (GPULang::Serialization::BlendFactor)enumValue;
                break;
            case RenderStateInstance::__Resolved::SourceBlendAlphaFactorType:
                if (!valid)
                {
                    compiler->Error(Format("Source blend alpha factor entry '%s' must evaluate to a compile time enum of BlendFactor", entryStr.c_str()), assignEntry);
                    return false;
                }
                val.Store(enumValue);
                stateResolved->blendStates[index].sourceAlphaBlendFactor = (GPULang::Serialization::BlendFactor)enumValue;
                break;
            case RenderStateInstance::__Resolved::DestinationBlendAlphaFactorType:
                if (!valid)
                {
                    compiler->Error(Format("Destination blend alpha factor entry '%s' must evaluate to a compile time enum of BlendFactor", entryStr.c_str()), assignEntry);
                    return false;
                }
                val.Store(enumValue);
                stateResolved->blendStates[index].destinationAlphaBlendFactor = (GPULang::Serialization::BlendFactor)enumValue;
                break;
            case RenderStateInstance::__Resolved::ColorBlendOpType:
                if (!valid)
                {
                    compiler->Error(Format("Color blend op entry '%s' must evaluate to a compile time enum of BlendOp", entryStr.c_str()), assignEntry);
                    return false;
                }
                val.Store(enumValue);
                stateResolved->blendStates[index].colorBlendOp = (GPULang::Serialization::BlendOp)enumValue;
                break;
            case RenderStateInstance::__Resolved::AlphaBlendOpType:
                if (!valid)
                {
                    compiler->Error(Format("Alpha blend op entry '%s' must evaluate to a compile time enum of BlendOp", entryStr.c_str()), assignEntry);
                    return false;
                }
                val.Store(enumValue);
                stateResolved->blendStates[index].alphaBlendOp = (GPULang::Serialization::BlendOp)enumValue;
                break;
                
            }
        }
        else if (entryType >= RenderStateInstance::__Resolved::StencilFailOpType && entryType <= RenderStateInstance::__Resolved::StencilReferenceMaskType)
        {
            AccessExpression* access = static_cast<AccessExpression*>(assignEntry->left);
            TransientString face = access->left->EvalString();
            Serialization::StencilState* state;
            if (face == "StencilBack")
                state = &stateResolved->backStencilState;
            else if (face == "StencilFront")
                state = &stateResolved->frontStencilState;

            uint32_t enumValue = 0;
            ValueUnion val;
            bool valid = assignEntry->right->EvalValue(val);
            switch (entryType)
            {
                case RenderStateInstance::__Resolved::StencilFailOpType:
                    if (!valid)
                    {
                        compiler->Error(Format("Stencil fail op entry '%s' must evaluate to a compile time enum of StencilOp", entryStr.c_str()), assignEntry);
                        return false;
                    }
                    val.Store(enumValue);
                    state->fail = (GPULang::Serialization::StencilOp)enumValue;
                    break;
                case RenderStateInstance::__Resolved::StencilPassOpType:
                    if (!valid)
                    {
                        compiler->Error(Format("Stencil pass op entry '%s' must evaluate to a compile time enum of StencilOp", entryStr.c_str()), assignEntry);
                        return false;
                    }
                    val.Store(enumValue);
                    state->pass = (GPULang::Serialization::StencilOp)enumValue;
                    break;
                case RenderStateInstance::__Resolved::StencilDepthFailOpType:
                    if (!valid)
                    {
                        compiler->Error(Format("Stencil depth fail op entry '%s' must evaluate to a compile time enum of StencilOp", entryStr.c_str()), assignEntry);
                        return false;
                    }
                    val.Store(enumValue);
                    state->depthFail = (GPULang::Serialization::StencilOp)enumValue;
                    break;
                case RenderStateInstance::__Resolved::StencilCompareModeType:
                    if (!valid)
                    {
                        compiler->Error(Format("Stencil compare mode '%s' must evaluate to a compile time enum of CompareMode", entryStr.c_str()), assignEntry);
                        return false;
                    }
                    val.Store(enumValue);
                    state->compare = (GPULang::Serialization::CompareMode)enumValue;
                    break;
                case RenderStateInstance::__Resolved::StencilCompareMaskType:
                    if (!valid)
                    {
                        compiler->Error(Format("Stencil compare mask '%s' must evaluate to a compile time unsigned integer", entryStr.c_str()), assignEntry);
                        return false;
                    }
                    val.Store(state->compareMask);
                    break;
                case RenderStateInstance::__Resolved::StencilWriteMaskType:
                    if (!valid)
                    {
                        compiler->Error(Format("Stencil write mask '%s' must evaluate to a compile time unsigned integer", entryStr.c_str()), assignEntry);
                        return false;
                    }
                    val.Store(state->writeMask);
                    break;
                case RenderStateInstance::__Resolved::StencilReferenceMaskType:
                    if (!valid)
                    {
                        compiler->Error(Format("Stencil reference mask '%s' must evaluate to a compile time unsigned integer", entryStr.c_str()), assignEntry);
                        return false;
                    }
                    val.Store(state->referenceMask);
                    break;
            }
        }
        else if (entryType == RenderStateInstance::__Resolved::BlendConstantsType)
        {
            InitializerExpression* init = static_cast<InitializerExpression*>(assignEntry->right);
            if (init->values.size != 4)
            {
                compiler->Error(Format("Blend constants must be an initializer of 4 values"), symbol);
                return false;
            }

            ValueUnion values[4];
            bool result = true;

            result |= init->values[0]->EvalValue(values[0]);
            result |= init->values[0]->EvalValue(values[1]);
            result |= init->values[0]->EvalValue(values[2]);
            result |= init->values[0]->EvalValue(values[3]);

            if (!result)
            {
                compiler->Error(Format("Each value in the initializer must resolve to a compile time literal"), symbol);
                return false;
            }

            values[0].Store(stateResolved->blendConstants[0]);
            values[1].Store(stateResolved->blendConstants[1]);
            values[2].Store(stateResolved->blendConstants[2]);
            values[3].Store(stateResolved->blendConstants[3]);
        }
        else
        {
            ValueUnion value;
            assignEntry->right->EvalValue(value);
            uint32_t enumValue = 0;
            switch (entryType)
            {
                case RenderStateInstance::__Resolved::DepthClampEnabledType:
                    
                    value.Store(stateResolved->depthClampEnabled);
                    break;
                case RenderStateInstance::__Resolved::NoPixelsType:
                    value.Store(stateResolved->noPixels);
                    break;
                case RenderStateInstance::__Resolved::PolygonModeType:
                    stateResolved->polygonMode = (GPULang::Serialization::PolygonMode)value.i[0];
                    break;
                case RenderStateInstance::__Resolved::CullModeType:
                    stateResolved->cullMode = (GPULang::Serialization::CullMode)value.i[0];
                    break;
                case RenderStateInstance::__Resolved::WindingOrderType:
                    stateResolved->windingOrderMode = (GPULang::Serialization::WindingOrderMode)value.i[0];
                    break;
                case RenderStateInstance::__Resolved::DepthBiasEnabledType:
                    value.Store(stateResolved->depthBiasEnabled);
                    break;
                case RenderStateInstance::__Resolved::DepthBiasFactorType:
                    value.Store(stateResolved->depthBiasFactor);
                    break;
                case RenderStateInstance::__Resolved::DepthBiasClampType:
                    value.Store(stateResolved->depthBiasClamp);
                    break;
                case RenderStateInstance::__Resolved::DepthBiasSlopeFactorType:
                    value.Store(stateResolved->depthBiasSlopeFactor);
                    break;
                case RenderStateInstance::__Resolved::LineWidthType:
                    value.Store(stateResolved->lineWidth);
                    break;
                case RenderStateInstance::__Resolved::DepthTestEnabledType:
                    value.Store(stateResolved->depthTestEnabled);
                    break;
                case RenderStateInstance::__Resolved::DepthWriteEnabledType:
                    value.Store(stateResolved->depthWriteEnabled);
                    break;
                case RenderStateInstance::__Resolved::DepthTestFunction:
                    stateResolved->depthCompare = (GPULang::Serialization::CompareMode)value.i[0];
                    break;
                case RenderStateInstance::__Resolved::DepthBoundsTestEnabledType:
                    value.Store(stateResolved->depthBoundsTestEnabled);
                    break;
                case RenderStateInstance::__Resolved::MinDepthBoundsType:
                    value.Store(stateResolved->minDepthBounds);
                    break;
                case RenderStateInstance::__Resolved::MaxDepthBoundsType:
                    value.Store(stateResolved->maxDepthBounds);
                    break;
                case RenderStateInstance::__Resolved::LogicOpEnabledType:
                    value.Store(stateResolved->logicOpEnabled);
                    break;
                case RenderStateInstance::__Resolved::ScissorEnabledType:
                    value.Store(stateResolved->scissorEnabled);
                    break;
                case RenderStateInstance::__Resolved::StencilEnabledType:
                    value.Store(stateResolved->stencilEnabled);
                    break;
                case RenderStateInstance::__Resolved::LogicOpType:
                    stateResolved->logicOp = (GPULang::Serialization::LogicOp)value.i[0];
                    break;
                default:
                    compiler->Error(Format("Unknown render state entry '%s'", entryStr.c_str()), symbol);
                    return false;
            }
        }
    }

    if (stateResolved->logicOpEnabled)
    {
        for (uint8_t i = 0; i < RenderStateInstance::__Resolved::NUM_BLEND_STATES; i++)
        {
            if (stateResolved->blendStates[i].blendEnabled)
            {
                compiler->Error(Format("Blend can't be enabled with a logic op"), symbol);
                return false;
            }
        }
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
Validator::ResolveStructure(Compiler* compiler, Symbol* symbol)
{
    Structure* struc = static_cast<Structure*>(symbol);
    Structure::__Resolved* strucResolved = Symbol::Resolved(struc);
    
    if (struc->name.StartsWith("gpl") && !compiler->ignoreReservedWords)
    {
        compiler->ReservedPrefixError(struc->name, "gpl", symbol);
        return false;
    }
    
    if (!compiler->AddSymbol(symbol->name, symbol))
        return false;

    // run attribute validation
    for (const Attribute* attr : struc->attributes)
    {
        if (!set_contains(this->allowedStructureAttributes, attr->name))
        {
            compiler->Error(Format("Invalid attribute for structure '%s': '%s'", struc->name.c_str(), attr->name.c_str()), symbol);
            return false;
        }

        if (set_contains(structureQualifiers, attr->name))
        {
            if (attr->name == "packed")
                strucResolved->packMembers = true;
        }
    }

    // push scope for struct but not for storage and constant buffers
    Compiler::LocalScope scope = Compiler::LocalScope::MakeLocalScope(compiler, &struc->scope);

    // validate members
    uint32_t offset = 0;
    for (Symbol* sym : struc->symbols)
    {
        if (sym->symbolType == Symbol::VariableType)
        {
            Variable* var = static_cast<Variable*>(sym);
            Variable::__Resolved* varResolved = Symbol::Resolved(var);
            varResolved->usageBits.flags.isStructMember = true;
            if (!this->ResolveVariable(compiler, var))
                return false;

            if (varResolved->typeSymbol->baseType == TypeCode::Bool)
                strucResolved->hasBoolMember = true;

            uint32_t arraySize = 0;
            for (Expression* expr : var->type.modifierValues)
            {
                if (expr != nullptr)
                {
                    uint32_t size;
                    ValueUnion val;
                    expr->EvalValue(val);
                    val.Store(size);
                    arraySize += size;
                }
            }
            uint32_t packedOffset = offset;
            uint32_t varSize = varResolved->byteSize;
            if (!strucResolved->packMembers)
            {
                if (varResolved->typeSymbol->category == Type::Category::UserTypeCategory)
                {
                    Structure::__Resolved* strucRes = Symbol::Resolved(static_cast<Structure*>(varResolved->typeSymbol));
                    offset = Type::Align(offset, strucRes->byteSize);
                    varSize = strucRes->byteSize;
                }
                else
                {
                    offset = Type::Align(offset, varResolved->typeSymbol->CalculateAlignment());
                }
            }
                

            uint32_t diff = offset - packedOffset;
            varResolved->startPadding = diff;
            varResolved->structureOffset = offset;
            offset += varSize * (arraySize > 0 ? arraySize : 1);

        }
    }
    strucResolved->byteSize = offset;
    uint32_t alignedSize = Type::Align(strucResolved->byteSize, 16);
    strucResolved->endPadding = alignedSize - strucResolved->byteSize;
    strucResolved->byteSize = alignedSize;

    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
Validator::ResolveEnumeration(Compiler* compiler, Symbol* symbol)
{
    Enumeration* enumeration = static_cast<Enumeration*>(symbol);
    Enumeration::__Resolved* enumResolved = Symbol::Resolved(enumeration);
    
    if (enumeration->builtin)
        enumeration->scope.symbolLookup.BeginBulkAdd();

    if (!compiler->AddSymbol(enumeration->name, enumeration))
        return false;

    if (enumeration->type.name != "u32" && enumeration->type.name != "i32" && enumeration->type.name != "u16" && enumeration->type.name != "i16")
    {
        compiler->Error(Format("Enumeration may only be an integer type of either 32 or 16 bit"), symbol);
        return false;
    }

    if (enumResolved->typeSymbol == nullptr)
        enumResolved->typeSymbol = compiler->GetType(enumeration->type);
    enumeration->baseType = enumResolved->typeSymbol->baseType;
    enumeration->symbols.Clear();
    
    static ConstantString arg0 = "_arg0";
    static ConstantString rhs = "rhs";
    static ConstantString eqOp = "operator==";
    static ConstantString neqOp = "operator!=";
    
    if (enumeration->globals.size == 0)
    {
        TransientArray<Variable*> parameters(1);
        TransientArray<Symbol*> generatedFunctions(2);

        // Create constructor from type, and to type
        Function* fromUnderlyingType = &enumeration->fromUnderlyingType;
        fromUnderlyingType->name = enumeration->name;
        fromUnderlyingType->returnType = Type::FullType{ enumeration->name };
        fromUnderlyingType->compileTime = true;
        fromUnderlyingType->constructorType = enumeration;
        Variable* arg = &enumeration->fromUnderlyingTypeArg;
        arg->name = arg0;
        arg->type = enumeration->type;
        parameters.Clear();
        parameters.Append(arg);
        fromUnderlyingType->parameters = parameters;
        generatedFunctions.Append(fromUnderlyingType);

        Function* toUnderlyingType = &enumeration->toUnderlyingType;
        toUnderlyingType->name = enumeration->type.name;
        toUnderlyingType->returnType = enumeration->type;
        toUnderlyingType->compileTime = true;
        toUnderlyingType->constructorType = enumeration;
        arg = &enumeration->toUnderlyingTypeArg;
        arg->name = arg0;
        arg->type = Type::FullType{ enumeration->name };
        parameters.Clear();
        parameters.Append(arg);
        toUnderlyingType->parameters = parameters;
        generatedFunctions.Append(toUnderlyingType);
        
        enumeration->globals = generatedFunctions;
        generatedFunctions.Clear();

        Function* comparison = &enumeration->eqOp;
        comparison->name = eqOp;
        comparison->returnType = Type::FullType{ ConstantString("b8") };
        arg = &enumeration->eqOpArg;
        arg->name = rhs;
        arg->type = Type::FullType{ enumeration->name };
        parameters.Clear();
        parameters.Append(arg);
        comparison->parameters = parameters;
        generatedFunctions.Append(comparison);
        

        comparison = &enumeration->neqOp;
        comparison->name = neqOp;
        comparison->returnType = Type::FullType{ ConstantString("b8") };
        arg = &enumeration->neqOpArg;
        arg->name = rhs;
        arg->type = Type::FullType{ enumeration->name };
        parameters.Clear();
        parameters.Append(arg);
        comparison->parameters = parameters;
        generatedFunctions.Append(comparison);
        enumeration->staticSymbols = generatedFunctions;
    }

    uint32_t nextValue = 0;
    for (size_t i = 0; i < enumeration->labels.size; i++)
    {
        const FixedString& str = enumeration->labels.buf[i];
        Expression* expr = enumeration->values.buf[i];

        // Check of label redefinition
        if (!compiler->staticSymbolSetup)
        {
            if (enumeration->scope.symbolLookup.Find(HashString(str)) != enumeration->scope.symbolLookup.end())
            {
                compiler->Error(Format("Enumeration redefinition '%s' in '%s'", str.c_str(), enumeration->name.c_str()), symbol);
                return false;
            }
        }

        // Setup variable
        EnumExpression* sym = nullptr;
        Type::FullType expressionType{ enumeration->name };
        expressionType.literal = true;

        // Resolve value
        if (expr != nullptr)
        {
            if (!expr->Resolve(compiler))
                return false;

            Type::FullType type;
            expr->EvalType(type);
            if (type != enumeration->type)
            {
                compiler->Error(Format("Enumeration is of type '%s' but label '%s' is of type '%s'", enumeration->type.ToString().c_str(), str.c_str(), type.ToString().c_str()), symbol);
                return false;
            }

            if (type.name == "u32" || type.name == "i32" || type.name == "u16" || type.name == "i16")
            {
                ValueUnion val;
                expr->EvalValue(val);
                uint32_t value;
                val.Store(value);
                sym = Alloc<EnumExpression>(value, expressionType, enumeration->type);
                Symbol::Resolved(sym)->type = enumeration;
                nextValue = value + 1;
            }
            else
            {
                compiler->Error(Format("Enum value must evaluate to uint or int"), symbol);
                return false;
            }
        }
        else
        {
            sym = Alloc<EnumExpression>(nextValue++, expressionType, enumeration->type);
            Symbol::Resolved(sym)->type = enumeration;
        }

        // Add to type
        sym->name = str.c_str();
        enumeration->symbols.Append(sym);
        enumeration->scope.symbolLookup.Insert(HashString(sym->name), sym);
    }
    
    if (enumeration->builtin)
        enumeration->scope.symbolLookup.EndBulkAdd();

    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
Validator::ResolveVariable(Compiler* compiler, Symbol* symbol)
{
    Variable* var = static_cast<Variable*>(symbol);
    auto varResolved = Symbol::Resolved(var);
    if (var->name.StartsWith("gpl") && !compiler->ignoreReservedWords)
    {
        compiler->ReservedPrefixError(var->name, "gpl", symbol);
        return false;
    }

    // resolve array type and names
    if (var->valueExpression != nullptr)
    {
        Expression* expr = var->valueExpression;
        expr->isLhsValue = false;
        expr->Resolve(compiler);
    }
    
    // Only one exception where the symbol (not resolved) gets changed during validation
    if (var->type.name == UNDEFINED_TYPE)
    {
        if (var->valueExpression != nullptr)
        {
            var->valueExpression->EvalType(var->type);
            var->type.nameLocation = Symbol::Location();
            var->type.formatLocation = Symbol::Location();
            var->type.modifierLocation = Symbol::Location();
        }
        else
        {
            compiler->Error(Format("'%s' can't infer it's type, either initialize the value or declare its type explicitly", var->name.c_str()), symbol);
            return false;
        }
    }

    Type* type = varResolved->typeSymbol;
    if (type == nullptr)
    {
        type = compiler->GetType(var->type);
        if (type == nullptr)
        {
            compiler->UnrecognizedTypeError(var->type.name.c_str(), symbol);
            return false;
        }
        varResolved->typeSymbol = type;
        var->type.name = type->name;        // because we can do an alias lookup, this value might change
    }
    Type::FullType::Modifier lastIndirectionModifier = var->type.LastIndirectionModifier();

    varResolved->accessBits.flags.readAccess = true; // Implicitly set read access to true
    varResolved->byteSize = type->byteSize;
    varResolved->storage = Storage::Default;
    varResolved->parameterBits.bits = 0x0;

    for (Expression* expr : var->type.modifierValues)
    {
        if (expr != nullptr)
            expr->Resolve(compiler);
    }

    if (!this->ValidateType(compiler, var->type, varResolved->typeSymbol, var))
        return false;

    // struct members may only be scalars, or stencil states but they can't be created by the grammar rules
    if (varResolved->usageBits.flags.isStructMember && 
        (type->category != Type::ScalarCategory && type->category != Type::EnumCategory && type->category != Type::UserTypeCategory && type->category != Type::StencilStateCategory))
    {
        compiler->Error(Format("'%s' may only be scalar or struct type if member of a struct", var->name.c_str()), symbol);
        return false;
    }    
    
    // Add symbol
    if (!compiler->AddSymbol(var->name, var))
        return false;

    // If struct member, only allow sized arrays and no initializers
    if (varResolved->usageBits.flags.isStructMember)
    {
        if (var->valueExpression != nullptr)
        {
            compiler->Error(Format("'struct' members may not have initializers"), symbol);
            return false;
        }

        for (Expression* expr : var->type.modifierValues)
        {
            uint32_t size = 0;
            if (expr != nullptr)
            {
                ValueUnion val;
                expr->EvalValue(val);
                val.Store(size);
            }
            if (size == 0)
            {
                if (compiler->target.supportsPhysicalBufferAddresses)
                    varResolved->usageBits.flags.isPhysicalAddress = true;
                else
                {
                    compiler->Error(Format("'struct' array member can't be of dynamic size"), symbol);
                    return false;    
                }                
            }
        }
    }

    // figure out set of allowed attributes
    PinnedSet<FixedString>* allowedAttributesSet = nullptr;
    if (varResolved->usageBits.flags.isStructMember)
        allowedAttributesSet = nullptr;
    else if (varResolved->usageBits.flags.isParameter)
        allowedAttributesSet = &this->allowedParameterAttributes;
    else
    {
        if (type->category == Type::TextureCategory)
            allowedAttributesSet = &this->allowedTextureAttributes;
        else if (type->category == Type::PixelCacheCategory)
            allowedAttributesSet = &this->allowedPointerAttributes;
        else if (type->category == Type::ScalarCategory || type->category == Type::EnumCategory)
            allowedAttributesSet = &this->allowedScalarAttributes;
        else if (type->category == Type::SamplerCategory)
            allowedAttributesSet = &this->allowedSamplerAttributes;
        else if (type->category == Type::UserTypeCategory)
        {
            if (lastIndirectionModifier == Type::FullType::Modifier::Pointer)
                allowedAttributesSet = &this->allowedPointerAttributes;
            else
                allowedAttributesSet = &this->allowedScalarAttributes;
        }
    }

    // run attribute validation
    for (const Attribute* attr : var->attributes)
    {
        if (attr->expression != nullptr)
            attr->expression->Resolve(compiler);
        if (allowedAttributesSet == nullptr || (!set_contains(*allowedAttributesSet, attr->name)))
        {
            compiler->Error(Format("Invalid attribute for variable of type '%s': '%s'", var->type.ToString().c_str(), attr->name.c_str()), symbol);
            return false;
        }

        ValueUnion val;
        // resolve attributes
        if (attr->name == "group")
        {
            if (!attr->expression->EvalValue(val))
            {
                compiler->Error(Format("Expected compile time constant for 'group' qualifier"), symbol);
                return false;
            }
            val.Store(varResolved->group);
        }
        else if (attr->name == "binding")
        {
            if (!attr->expression->EvalValue(val))
            {
                compiler->Error(Format("Expected compile time constant for 'binding' qualifier"), symbol);
                return false;
            }
            val.Store(varResolved->binding);
            if (varResolved->usageBits.flags.isStructMember)
            {
                compiler->Error(Format("Qualifier 'binding' illegal for struct members"), symbol);
                return false;
            }
            if (varResolved->usageBits.flags.isParameter && !varResolved->usageBits.flags.isEntryPointParameter)
            {
                compiler->Error(Format("Qualifier 'binding' illegal for parameters of non entry_point functions"), symbol);
                return false;
            }
        }
        else if (attr->name == "const")
            varResolved->usageBits.flags.isConst = true;
        else if (attr->name == "var")
        {
            varResolved->usageBits.flags.isVar = true;
            var->type.literal = false;
        }
        else if (attr->name == "link_defined")
        {
            if (varResolved->storage != Storage::Default)
            {
                compiler->Error(Format("Multiple storage qualifiers are not allowed"), symbol);
                return false;
            }
            varResolved->storage = Storage::LinkDefined;

            // Uncheck the literal bit if the variable is link-defined
            var->type.literal = false;
            varResolved->binding = compiler->linkDefineCounter++;
        }
        else if (attr->name == "uniform")
        {
            if (varResolved->storage != Storage::Default)
            {
                compiler->Error(Format("Multiple storage qualifiers are not allowed"), symbol);
                return false;
            }
            varResolved->storage = Storage::Uniform;
            varResolved->usageBits.flags.isConst = true & !var->type.IsMutable();
        }
        else if (attr->name == "inline")
        {
            if (varResolved->storage != Storage::Default)
            {
                compiler->Error(Format("Multiple storage qualifiers are not allowed"), symbol);
                return false;
            }
            varResolved->storage = Storage::InlineUniform;
            varResolved->usageBits.flags.isConst = true;
        }
        else if (attr->name == "workgroup")
        {
            if (varResolved->storage != Storage::Default)
            {
                compiler->Error(Format("Multiple storage qualifiers are not allowed"), symbol);
                return false;
            }
            varResolved->storage = Storage::Workgroup;
        }
        else if (attr->name == "device")
        {
            if (varResolved->storage != Storage::Default)
            {
                compiler->Error(Format("Multiple storage qualifiers are not allowed"), symbol);
                return false;
            }
            if (!compiler->target.supportsGlobalDeviceStorage)
            {
                compiler->Error(Format("'device' storage not supported by target '%s'", compiler->target.name.c_str()), symbol);
                return false;
            }
            varResolved->storage = Storage::Device;
        }
        else
        {
            // more complicated lookups
            if (set_contains(pointerQualifiers, attr->name))
            {
                if (attr->name == "no_read")
                    varResolved->accessBits.flags.readAccess = false;
                else if (attr->name == "atomic")
                    varResolved->accessBits.flags.atomicAccess = true;
                else if (attr->name == "volatile")
                    varResolved->accessBits.flags.volatileAccess = true;
            }
        }

        if (set_contains(parameterAccessFlags, attr->name))
        {
            if (attr->name == "in")
                varResolved->storage = Storage::Input;
            else if (attr->name == "out")
                varResolved->storage = Storage::Output;
            else if (attr->name == "ray_payload")
            {
                if (varResolved->storage == Storage::Input)
                    varResolved->storage = Storage::RayPayloadInput;
                else
                {
                    if (varResolved->storage != Storage::Default)
                    {
                        compiler->Error(Format("Multiple storage qualifiers are not allowed"), symbol);
                        return false;
                    }
                    varResolved->storage = Storage::RayPayload;
                }
            }
            else if (attr->name == "ray_callable_data")
            {
                if (varResolved->storage == Storage::Input)
                    varResolved->storage = Storage::CallableDataInput;
                else
                {
                    if (varResolved->storage != Storage::Default)
                    {
                        compiler->Error(Format("Multiple storage qualifiers are not allowed"), symbol);
                        return false;
                    }
                    varResolved->storage = Storage::CallableData;
                }
            }
            else if (attr->name == "ray_hit_attribute")
            {
                if (varResolved->storage != Storage::Default)
                {
                    compiler->Error(Format("Multiple storage qualifiers are not allowed"), symbol);
                    return false;
                }
                varResolved->storage = Storage::RayHitAttribute;
            }
        }
        else if (set_contains(parameterQualifiers, attr->name))
        {
            if (attr->name == "patch")
                varResolved->parameterBits.flags.isPatch = true;
            else if (attr->name == "no_perspective")
                varResolved->parameterBits.flags.isNoPerspective = true;
            else if (attr->name == "no_interpolate")
                varResolved->parameterBits.flags.isNoInterpolate = true;
            else if (attr->name == "centroid")
                varResolved->parameterBits.flags.isCentroid = true;
        }
    }

    if (!varResolved->usageBits.flags.isParameter && !varResolved->usageBits.flags.isStructMember && (varResolved->storage == Storage::Default || varResolved->storage == Storage::Global))
    {
        if (varResolved->usageBits.flags.isVar && varResolved->usageBits.flags.isConst)
        {
            compiler->Error(Format("Variable may be either 'var' or 'const' but not both"), var);
            return false;
        }
        else if (!varResolved->usageBits.flags.isVar && !varResolved->usageBits.flags.isConst)
        {
            if (varResolved->storage == Storage::Global)
            {
                compiler->Error(Format("Variable must be either 'uniform' or 'const'"), var);
                return false;
            }
            else
            {
                compiler->Error(Format("Variable must be either 'var' or 'const'"), var);
                return false;    
            }            
        }

        if (varResolved->usageBits.flags.isConst && var->valueExpression == nullptr)
        {
            if (compiler->options.disallowUninitializedConst)
            {
                compiler->Error(Format("Uninitialized 'const' variables are not allowed. Either turn off 'disallowUninitializedConst' or initialize the variable"), var);
                return false;
            }
            else
                compiler->Warning(Format("Variable is 'const' but uninitialized"), var);
        }
    }

    if (!compiler->target.supportsPhysicalAddressing)
    {
        if (varResolved->usageBits.flags.isStructMember && type->category == Type::UserTypeCategory && var->type.IsPointer())
        {
            if (!compiler->target.supportsPhysicalBufferAddresses)
            {
                compiler->Error(Format("Struct members may not be pointers if ('%s') does not support physical buffer addresses", compiler->target.name.c_str()), var);
                return false;
            }
        }
        else
        {
            if (type->category == Type::UserTypeCategory && varResolved->storage != Storage::InlineUniform && varResolved->storage != Storage::Uniform && var->type.IsPointer())
            {
                compiler->Error(Format("Type may not be pointer if target language ('%s') does not support physical addressing", compiler->target.name.c_str()), var);
                return false;
            }    
        }
    }

    if (compiler->IsScopeGlobal())
    {
        uint16_t numArrays = 0;
        uint16_t numPointers = 0;
        for (Type::FullType::Modifier mod : var->type.modifiers)
        {
            if (mod == Type::FullType::Modifier::Array)
                numArrays++;
            else if (mod == Type::FullType::Modifier::Pointer)
                numPointers++;
        }
        if (varResolved->storage == Storage::Uniform)
        {
            if (numArrays > 1)
            {
                compiler->Error(Format("Variables of non scalar type in the global scope may only be a one dimensional array"), symbol);
                return false;
            }
            if (numPointers > 1)
            {
                compiler->Error(Format("Variables of non scalar type in the global scope may only be single pointer"), symbol);
                return false;
            }
            

            /*
            if (varResolved->type.modifiers.front() != Type::FullType::Modifier::Pointer && (varResolved->typeSymbol->category == Type::UserTypeCategory || varResolved->typeSymbol->category == Type::ScalarCategory))
            {
                compiler->Error(Format("Variables in the global scope with storage 'uniform' must be pointers"), symbol);
                return false;        
            }
            */

            if (type->category != Type::SamplerCategory && type->category != Type::TextureCategory && type->category != Type::PixelCacheCategory && type->category != Type::UserTypeCategory)
            {
                compiler->Error(Format("Variables of storage 'uniform' may only be pointers to 'sampler'/'texture'/'pixel_cache'/'struct' types"), symbol);
                return false;
            }
        }
        else if (varResolved->storage == Storage::InlineUniform)
        {
            if (type->category != Type::UserTypeCategory)
            {
                compiler->Error(Format("Variables of storage 'inline_uniform' storage may only be pointers to 'struct' types"), symbol);
                return false;
            }
        }
        else // Types that are not resources
        {
            if (type->category == Type::SamplerCategory || type->category == Type::TextureCategory || type->category == Type::PixelCacheCategory)
            {
                compiler->Error(Format("Variables of sampler/texture/pixel_cache types must be 'uniform'"), symbol);
                return false;
            }
            
            if (varResolved->usageBits.flags.isConst && var->valueExpression == nullptr)
            {
                // check for variable initialization criteria
                compiler->Error(Format("Variable declared as const but is never initialized"), symbol);
                return false;
            }
            if (varResolved->storage == Storage::Default)
                varResolved->storage = Storage::Global;
        }
    }
    else if (varResolved->usageBits.flags.isStructMember)
    {
        if (varResolved->storage != Storage::Default)
        {
            compiler->Error(Format("Storage type not allowed on struct member %s", type->name.c_str()), symbol);
            return false;
        }

        /*
        if (varResolved->typeSymbol->baseType == TypeCode::Bool)
        {
            compiler->Warning(Format("Struct member %s of type 'b8' will be automatically promoted to u32", varResolved->name.c_str()), symbol);
            varResolved->type.name = "u32";
            varResolved->typeSymbol = compiler->GetType(varResolved->type);
        }
        */
    }
    else if (varResolved->usageBits.flags.isParameter)
    {
        if (varResolved->storage == Storage::Device)
        {
            if (!compiler->target.supportsGlobalDeviceStorage)
            {
                compiler->Error(Format("Target language %s does not support 'device' storage", compiler->target.name.c_str()), symbol);
                return false;    
            }
        }
        
        if (type->category == Type::SamplerCategory || type->category == Type::TextureCategory || type->category == Type::PixelCacheCategory)
        {
            if (varResolved->storage != Storage::Uniform)
            {
                compiler->Error(Format("Variables of sampler/texture/pixel_cache types must be 'uniform'"), symbol);
                return false;
            }
        }
    }
    else // Local variable
    {
        // Shouldn't be possible
        if (lastIndirectionModifier == Type::FullType::Modifier::Pointer && !varResolved->usageBits.flags.isParameter)
        {
            compiler->Error(Format("Pointers are only allowed on variables in the global scope", type->name.c_str()), symbol);
            return false;
        }

        if (varResolved->storage != Storage::Default)
        {
            compiler->Error(Format("Storage type not allowed on local variables", type->name.c_str()), symbol);
            return false;
        }
    }

    if (type->category == Type::TextureCategory
        && lastIndirectionModifier != Type::FullType::Modifier::Pointer)
    {
        compiler->Error(Format("Variable of texture type must be pointer"), symbol);
        return false;
    }
    else if (type->category == Type::SamplerCategory
        && lastIndirectionModifier != Type::FullType::Modifier::Pointer)
    {
        compiler->Error(Format("Variable of sampler type must be pointer"), symbol);
        return false;
    }

    if (type->category == Type::UserTypeCategory)
    {
        if (lastIndirectionModifier != Type::FullType::Modifier::Pointer)
        {
            if (varResolved->storage == Storage::Uniform)
            {
                compiler->Error(Format("Variable of uniform '%s' type must be pointer", type->name.c_str()), symbol);
                return false;
            }
            else if (varResolved->storage == Storage::InlineUniform)
            {
                compiler->Error(Format("Variable of inline '%s' type must be pointer", type->name.c_str()), symbol);
                return false;
            }
        }
    }

    if (varResolved->storage == Storage::LinkDefined)
    {
        if (varResolved->typeSymbol->columnSize > 1 ||
            (varResolved->typeSymbol->category != Type::Category::ScalarCategory && varResolved->typeSymbol->category != Type::Category::EnumCategory)
        )
        {
            compiler->Error(Format("Only scalar types can be 'link_defined'"), symbol);
            return false;
        }
    }

    // Check that the type can be mutable
    if (var->type.IsMutable())
    {
        varResolved->accessBits.flags.writeAccess = true;
        if (type->category == Type::SamplerCategory)
        {
            compiler->Error(Format("Sampler can not be mutable", type->name.c_str()), symbol);
            return false;
        }
        else if (type->category == Type::SampledTextureCategory)
        {
            compiler->Error(Format("Sampled textures can not be mutable", type->name.c_str()), symbol);
            return false;
        }
        else if (type->category == Type::PixelCacheCategory)
        {
            compiler->Error(Format("PixelCache can not be mutable", type->name.c_str()), symbol);
            return false;
        }
    }

    // validate types on both sides of the assignment
    if (var->valueExpression != nullptr)
    {
        Type::FullType lhs = var->type;
        lhs.literal = false; // Disable literal on variables even if the rhs type is literal
        Type::FullType rhs;
        if (!var->valueExpression->EvalType(rhs))
        {
            compiler->UnrecognizedTypeError(rhs.name, symbol);
            return false;
        }
        Type* lhsType = varResolved->typeSymbol;
        Type* rhsType;
        var->valueExpression->EvalTypeSymbol(rhsType);

        if (varResolved->usageBits.flags.isConst && varResolved->storage == Storage::Global)
        {
            if (!rhs.literal)
            {
                compiler->Error(Format("Global constants must be initialized by a compile time value"), symbol);
                return false;
            }
        }
        
        if (!lhs.Constructible(rhs))
        {
            // If right hand side value is a literal, attempt to reduce the right hand side to a single expression
            ValueUnion val;
            if (rhs.literal && var->valueExpression->EvalValue(val))
            {
                auto loc = var->valueExpression->location;
                #define X(Type, type, ty)\
                    if (val.columnSize > 1)\
                    {\
                        auto arr = FixedArray<ty>(val.type, val.type + val.columnSize);\
                        var->valueExpression = Alloc<Type##VecExpression>(arr);\
                        var->valueExpression->location = loc;\
                    }\
                    else\
                        var->valueExpression = Alloc<Type##Expression>(val.type[0]);\
                        var->valueExpression->location = loc;\
                        var->valueExpression->Resolve(compiler);
                
                switch (lhsType->baseType)
                {
                    VALUE_UNION_SWITCH()
                }
            }
            else
            {
                auto conversionName = TransientString(lhsType->name, "(", rhs.ToString(), ")");
                Function* conv = compiler->GetSymbol<Function>(conversionName);
                if (conv == nullptr)
                {
                    compiler->Error(Format("'%s' can't be converted to '%s'", rhs.ToString().c_str(), lhs.ToString().c_str()), symbol);
                    return false;
                }
                else
                {
                    if (compiler->options.disallowImplicitConversion)
                    {
                        compiler->Error(Format("Initialization not possible because implicit conversions ('%s' to '%s') are not allowed. Either disable implicit conversions or explicitly convert the value.", lhs.ToString().c_str(), rhs.ToString().c_str()), symbol);
                        return false;
                    }
                }
                varResolved->valueConversionFunction = conv;
            }
        }


        // Okay, so now when we're done, we'll copy over the modifier values from rhs to lhs
        var->type.modifierValues = rhs.modifierValues;
    }

    // check if image formats have been resolved
    if (type->category == Type::TextureCategory
        && var->type.mut
        && (var->type.imageFormat == InvalidImageFormat || var->type.imageFormat == Unknown)
        && !varResolved->usageBits.flags.isParameter)
    {
        compiler->Error(Format("Texture type must provide a format qualifier because it's marked as 'mutable'"), var);
        return false;
    }

    if (varResolved->usageBits.flags.isParameter)
    {
        // if parameter, resolve in and out bindings
        if (varResolved->binding == Variable::__Resolved::NOT_BOUND)
        {
            if (varResolved->storage == Storage::Input)
                varResolved->inBinding = this->inParameterIndexCounter++;
            if (varResolved->storage == Storage::Output)
                varResolved->outBinding = this->outParameterIndexCounter++;
        }
        else
        {
            varResolved->inBinding = varResolved->binding;
            varResolved->outBinding = varResolved->binding;
            if (varResolved->storage == Storage::Input)
                this->inParameterIndexCounter = varResolved->binding + 1;
            if (varResolved->storage == Storage::Output)
                this->outParameterIndexCounter = varResolved->binding + 1;
        }
    }
    else if (!compiler->IsScopeType())
    {
        Type::Category cat = type->category;

        // if user type and mutable or uniform, convert to either mutable or constant type
        if (cat == Type::Category::UserTypeCategory)
        {
            if (compiler->IsScopeGlobal())
            {
                if (varResolved->storage == Storage::Uniform && lastIndirectionModifier != Type::FullType::Modifier::Pointer)
                {
                    compiler->Error(Format("Global variable '%s' with storage class 'uniform' must be a pointer", type->name.c_str()), var);
                    return false;
                }
            }
        }

        // if not a parameter, assume resource (these types can't be declared inside functions)
        if (cat == Type::Category::TextureCategory
            || cat == Type::Category::SamplerCategory
            || cat == Type::Category::PixelCacheCategory
            || (cat == Type::Category::UserTypeCategory && varResolved->storage == Storage::Uniform))
        {
            if (varResolved->group == Variable::__Resolved::NOT_BOUND)
            {
                varResolved->group = this->defaultGroup;
            }

            if (this->resourceIndexingMode == ResourceIndexingByType)
            {
                auto it = this->resourceIndexCounter.Find(cat);
                if (it == this->resourceIndexCounter.end())
                {
                    this->resourceIndexCounter.Insert(cat, 0);
                    it = this->resourceIndexCounter.Find(varResolved->group);
                }

                if (varResolved->binding == Variable::__Resolved::NOT_BOUND)
                {
                    varResolved->binding = it->second++;
                }
                else
                {
                    it->second = max(it->second, varResolved->binding + 1);
                }
            }
            else if (this->resourceIndexingMode == ResourceIndexingByGroup)
            {
                auto it = this->resourceIndexCounter.Find(varResolved->group);
                if (it == this->resourceIndexCounter.end())
                {
                    this->resourceIndexCounter.Insert(varResolved->group, 0);
                    it = this->resourceIndexCounter.Find(varResolved->group);
                }

                if (varResolved->binding == Variable::__Resolved::NOT_BOUND)
                {
                    varResolved->binding = it->second++;
                }
                else
                {
                    it->second = max(it->second, varResolved->binding + 1);
                }
                auto it2 = this->resourceTypePerGroupAndBinding.Find(varResolved->group);
                if (it2 == this->resourceTypePerGroupAndBinding.end())
                {
                    PinnedMap<uint32_t, Type::Category> table = 0xFFF;
                    table.Insert(varResolved->binding, varResolved->typeSymbol->category);
                    this->resourceTypePerGroupAndBinding.Insert(varResolved->group, table);
                }
                else
                {
                    PinnedMap<uint32_t, Type::Category>& table = it2->second;
                    auto it3 = table.Find(varResolved->binding);
                    if (it3 == table.end())
                    {
                        table.Insert(varResolved->binding, varResolved->typeSymbol->category);
                    }
                    else
                    {
                        if (it3->second != varResolved->typeSymbol->category)
                        {
                            compiler->Error(Format("Aliasing is only allowed on resource pointers of same type. First declared as '%s' can't be aliased as '%s'", Type::CategoryToString(varResolved->typeSymbol->category).c_str(), Type::CategoryToString(it3->second).c_str()), var);
                            return false;
                        }
                    }
                }
            }
        }

        if (cat == Type::Category::UserTypeCategory && varResolved->storage == Storage::Uniform)
        {
            Structure* currentStructure = static_cast<Structure*>(varResolved->typeSymbol);
            Structure::__Resolved* currentStrucResolved = Symbol::Resolved(currentStructure);

            // If the structure is packed, we need to inflate it to adhere to alignment rules
            if (currentStrucResolved->packMembers || currentStrucResolved->hasBoolMember)
            {
                const char* bufferType = var->type.IsMutable() ? "MutableBuffer" : "Buffer";
                std::string structName = Format("gpl%s_%s", bufferType, var->name.c_str());
                if (currentStrucResolved->packMembers && compiler->options.warnOnImplicitBufferPadding)
                    compiler->Warning(Format("'%s' of packed type '%s' with 'uniform' storage uses a generated struct '%s' with fixed alignment of each member", var->name.c_str(), var->type.ToString().c_str(), structName.c_str(), var->type.name.c_str()), var);
                if (currentStrucResolved->hasBoolMember && compiler->options.warnOnImplicitBoolPromotion)
                    compiler->Warning(Format("'%s' of type '%s' with 'uniform' storage uses a generated struct '%s' with a promotion of u8 members to u32", var->name.c_str(), var->type.ToString().c_str(), structName.c_str(), var->type.name.c_str()), var);
                
                 // Generate mutable/uniform variant of struct
                Structure* generatedStruct = Alloc<Structure>();
                uint32_t structSize = 0;
                uint32_t padCounter = 0;
                uint32_t offset = 0;
                for (Symbol* sym : type->symbols)
                {
                    if (sym->symbolType == Symbol::VariableType)
                    {
                        Variable* var = static_cast<Variable*>(sym);
                        Variable::__Resolved* varResolved = Symbol::Resolved(var);
                        Variable* generatedVar = Alloc<Variable>();
                        Variable::__Resolved* generatedVarResolved = Symbol::Resolved(generatedVar);
                        generatedVar->name = var->name;
                        generatedVar->type = var->type;
                        generatedVarResolved->usageBits = varResolved->usageBits;
                        generatedVarResolved->typeSymbol = varResolved->typeSymbol;
                        if (generatedVarResolved->typeSymbol->baseType == TypeCode::Bool)
                        {
                            generatedVar->type.name = "u32"_c;
                            generatedVarResolved->typeSymbol = &UInt32Type;
                        }
                        
                        uint32_t size = varResolved->typeSymbol->CalculateSize();
                        uint32_t alignment = varResolved->typeSymbol->CalculateAlignment();
                        uint32_t alignedOffset = Type::Align(offset, alignment);
                        generatedVarResolved->startPadding = alignedOffset - offset;
                        generatedVarResolved->byteSize = size;
                        generatedVarResolved->structureOffset = alignedOffset;
                        offset = alignedOffset + size;
                        structSize += generatedVarResolved->byteSize + generatedVarResolved->startPadding;
                        generatedStruct->symbols.Append(generatedVar);
                        generatedStruct->scope.symbolLookup.Insert(HashString(var->name), generatedVar);
                    }
                }
                Structure::__Resolved* generatedStructResolved = Symbol::Resolved(generatedStruct);
                currentStrucResolved = generatedStructResolved;
                generatedStruct->name = structName;
                //generatedStruct->annotations = var->annotations;
            
                generatedStructResolved->byteSize = structSize;
                generatedStructResolved->packMembers = false;
                //generatedStructResolved->byteSize = varResolved
                if (var->type.IsMutable())
                {
                    generatedStructResolved->usageFlags.flags.isMutableBuffer = true;
                    generatedStruct->baseType = TypeCode::Buffer;
                }
                else
                {
                    generatedStructResolved->usageFlags.flags.isUniformBuffer = true;
                    generatedStruct->baseType = TypeCode::Buffer;
                }
                
                Type::FullType newType{ generatedStruct->name };
                newType.modifiers = var->type.modifiers;
                newType.modifierValues = var->type.modifierValues;
                newType.mut = var->type.mut;
                newType.literal = var->type.literal;
                var->type = newType;
                varResolved->typeSymbol = generatedStruct;


                // Insert symbol before this one, avoiding resolving (we assume the struct and members are already valid)
                compiler->symbols.Insert(generatedStruct, compiler->symbolIterator);
                compiler->scopes.back()->symbolLookup.Insert(HashString(generatedStruct->name), generatedStruct);
                compiler->symbolIterator++;
            }

            if (var->type.IsMutable())
            {
                if (currentStrucResolved->storageFunction == nullptr)
                {
                    currentStrucResolved->storageFunction = Alloc<Function>();
                    currentStrucResolved->storageFunction->name = "bufferStore";
                    currentStrucResolved->storageFunction->returnType = Type::FullType{ ConstantString("void") };

                    Variable* arg = Alloc<Variable>();
                    arg->name = "buffer";
                    Attribute* attr = Alloc<Attribute>();
                    attr->name = "uniform";
                    attr->expression = nullptr;
                    arg->attributes = { attr };
                    arg->type = var->type;
                    arg->type.modifiers = TransientArray<Type::FullType::Modifier>::Concatenate(Type::FullType::Modifier::Pointer);
                    arg->type.modifierValues = TransientArray<Expression*>::Concatenate((Expression*)nullptr);
                
                    Variable* arg2 = Alloc<Variable>();
                    arg2->name = "value";
                    arg2->type = var->type;
                    arg2->type.mut = false;
                    arg2->type.modifiers = TransientArray<Type::FullType::Modifier>();
                    arg2->type.modifierValues = TransientArray<Expression*>();
                    currentStrucResolved->storageFunction->parameters = { arg, arg2 };
                    this->ResolveFunction(compiler, currentStrucResolved->storageFunction);    
                }
                if (currentStrucResolved->loadFunction == nullptr)
                {
                    currentStrucResolved->loadFunction = Alloc<Function>();
                    currentStrucResolved->loadFunction->name = "bufferLoad";
                    currentStrucResolved->loadFunction->returnType = Type::FullType{currentStructure->name};

                    Variable* arg = Alloc<Variable>();
                    arg->name = "buffer";
                    Attribute* attr = Alloc<Attribute>();
                    attr->name = "uniform";
                    attr->expression = nullptr;
                    arg->attributes = { attr };
                    arg->type = var->type;
                    arg->type.modifiers = TransientArray<Type::FullType::Modifier>::Concatenate(Type::FullType::Modifier::Pointer);
                    arg->type.modifierValues = TransientArray<Expression*>::Concatenate((Expression*)nullptr);
                    
                    currentStrucResolved->loadFunction->parameters = { arg };
                    this->ResolveFunction(compiler, currentStrucResolved->loadFunction);   
                }
            }
        }
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
Validator::ResolveStatement(Compiler* compiler, Symbol* symbol)
{
    if (compiler->IsUnreachable())
    {
        compiler->Warning(Format("Unreachable code"), symbol);
        Symbol::__Resolved* resolvedSymbol = Symbol::Resolved(symbol);
        resolvedSymbol->unreachable = true;
        return true;
    }

    switch (symbol->symbolType)
    {
        case Symbol::AliasType:
        {
            Alias* alias = static_cast<Alias*>(symbol);
            Symbol* sym = compiler->GetType(Type::FullType{alias->type});
            if (sym == nullptr)
            {
                compiler->UnrecognizedTypeError(alias->type, alias);
                return false;
            }
            return compiler->AddSymbol(alias->name, sym);
        }
        case Symbol::BreakStatementType:
        {
            if (compiler->generationState.active)
            {
                compiler->Error(Format("Code generation blocks can't break control flow"), symbol);
                return false;
            }
            auto statement = reinterpret_cast<BreakStatement*>(symbol);
            Symbol* forOwner = compiler->GetParentScopeOwner(Symbol::ForStatementType);
            Symbol* whileOwner = compiler->GetParentScopeOwner(Symbol::WhileStatementType);
            Symbol* switchOwner = compiler->GetParentScopeOwner(Symbol::SwitchStatementType);
            if (forOwner == nullptr && whileOwner == nullptr && switchOwner == nullptr)
            {
                compiler->Error(Format("'break' is only valid inside a for, while or switch statement body"), statement);
                return false;
            }
            compiler->MarkScopeUnreachable();
            return true;
        }
        case Symbol::ContinueStatementType:
        {
            if (compiler->generationState.active)
            {
                compiler->Error(Format("Code generation blocks can't break control flow"), symbol);
                return false;
            }
            auto statement = reinterpret_cast<ContinueStatement*>(symbol);
            Symbol* forOwner = compiler->GetParentScopeOwner(Symbol::ForStatementType);
            Symbol* whileOwner = compiler->GetParentScopeOwner(Symbol::WhileStatementType);
            if (forOwner == nullptr && whileOwner == nullptr)
            {
                compiler->Error(Format("'continue' is only valid inside a for or while statement body"), statement);
                return false;
            }
            compiler->MarkScopeUnreachable();
            return true;
        }
        case Symbol::ExpressionStatementType:
        {
            auto statement = reinterpret_cast<ExpressionStatement*>(symbol);
            for (auto& expr : statement->expressions)
                expr->Resolve(compiler);
            break;
        }
        case Symbol::ForStatementType:
        {
            if (compiler->generationState.active)
            {
                compiler->Error(Format("Code generation don't support loops"), symbol);
                return false;
            }
            auto statement = reinterpret_cast<ForStatement*>(symbol);
            auto statementRes = Symbol::Resolved(statement);
            for (const Attribute* attr : statement->attributes)
            {
                if (attr->name == "unroll")
                {
                    if (attr->expression == nullptr)
                        statement->unrollCount = UINT_MAX;
                    else
                    {
                        ValueUnion val;
                        if (!attr->expression->EvalValue(val))
                        {
                            compiler->Error(Format("Unroll count must evaluate to a literal integer value", attr->name.c_str()), statement);
                            return false;
                        }
                        val.Store(statement->unrollCount);
                    }
                }
                else
                {
                    compiler->Error(Format("Invalid loop modifier '%s'", attr->name.c_str()), statement);
                    return false;
                }
            }
            statementRes->scope.owningSymbol = statement;
            Compiler::LocalScope scope = Compiler::LocalScope::MakeLocalScope(compiler, &statementRes->scope);
            for (Variable* var : statement->declarations)
            {
                if (!this->ResolveVariable(compiler, var))
                    return false;
            }
            if (statement->condition->Resolve(compiler))
            {
                // Convert condition if not bool
                Type::FullType conditionType;
                statement->condition->EvalType(conditionType);
                Type* typeSymbol = compiler->GetType(conditionType);
                if (typeSymbol->baseType != TypeCode::Bool)
                {
                    Symbol* conversionFunction = compiler->GetSymbol(Format("b8(%s)", conditionType.name.c_str()));
                    if (conversionFunction == nullptr)
                    {
                        compiler->Error(Format("Condition of type '%s' must evaluate to a boolean expression", conditionType.ToString().c_str()), statement);
                        return false;
                    }

                    // Replace condition with a call expression to the conversion function
                    FixedArray<Expression*> arguments = { statement->condition };
                    Symbol::Location loc = statement->condition->location;
                    statement->condition = Alloc<CallExpression>(Alloc<SymbolExpression>(conversionFunction->name), arguments);
                    statement->condition->location = loc;

                    // Resolve again
                    if (!statement->condition->Resolve(compiler))
                        return false;
                }
            }
            else
            {
                return false;
            }
            return statement->loop->Resolve(compiler) && this->ResolveStatement(compiler, statement->contents);
        }
        case Symbol::IfStatementType:
        {
            auto statement = reinterpret_cast<IfStatement*>(symbol);
            if (!statement->condition->Resolve(compiler))
                return false;

            // Convert condition if not bool
            Type::FullType conditionType;
            statement->condition->EvalType(conditionType);
            Type* typeSymbol;
            statement->condition->EvalTypeSymbol(typeSymbol);
            
            if (typeSymbol->baseType != TypeCode::Bool)
            {
                Symbol* conversionFunction = compiler->GetSymbol(Format("b8(%s)", conditionType.name.c_str()));
                if (conversionFunction == nullptr)
                {
                    compiler->Error(Format("Condition of type '%s' must evaluate to a boolean expression", conditionType.ToString().c_str()), statement);
                    return false;
                }

                // Replace condition with a call expression to the conversion function
                FixedArray<Expression*> arguments = { statement->condition };
                Symbol::Location loc = statement->condition->location;
                statement->condition = Alloc<CallExpression>(Alloc<SymbolExpression>(conversionFunction->name), arguments);
                statement->condition->location = loc;

                // Resolve again
                if (!statement->condition->Resolve(compiler))
                    return false;
            }
            
            if (compiler->generationState.active)
            {
                ValueUnion val;
                if (!statement->condition->EvalValue(val))
                {
                    compiler->Error(Format("Code generation blocks conditionals must resolve to a compile time value"), symbol);
                    return false;
                }
                
                int condition;
                val.Store(condition);
                compiler->generationState.branchActive = condition != 0;
                
                if (compiler->generationState.branchActive)
                {
                    if (!this->ResolveStatement(compiler, statement->ifStatement))
                        return false;
                }
                else if (statement->elseStatement)
                {
                    compiler->generationState.branchActive = true;
                    if (!this->ResolveStatement(compiler, statement->elseStatement))
                        return false;
                    compiler->generationState.branchActive = false;
                }
            }
            else
            {
                if (!this->ResolveStatement(compiler, statement->ifStatement))
                    return false;
                
                // If the if statement isn't a scope, it might mark the scope as unreachable, which has to reset when we leave the branch
                if (statement->ifStatement->symbolType != Symbol::ScopeStatementType)
                    compiler->MarkScopeReachable();
                
                bool ifReturns = compiler->branchReturns;
                compiler->branchReturns = false;
                if (statement->elseStatement)
                {
                    compiler->MarkScopeReachable();
                    if (!this->ResolveStatement(compiler, statement->elseStatement))
                        return false;
                    ifReturns &= compiler->branchReturns;
                }
                else
                {
                    // If there is no else statement, then the branch won't necessarily return
                    ifReturns = false;
                }
                compiler->branchReturns = ifReturns;
            }

            return true;
        }
        case Symbol::TerminateStatementType:
        {
            if (compiler->generationState.active)
            {
                compiler->Error(Format("Code generation blocks don't support control flow"), symbol);
                return false;
            }
            auto statement = reinterpret_cast<TerminateStatement*>(symbol);
            Symbol* scopeOwner = compiler->GetParentScopeOwner(Symbol::FunctionType);
            const ConstantString& terminationString = TerminateStatement::TerminationTypeToString(statement->type);
            if (scopeOwner == nullptr)
            {
                compiler->Error(Format("'%s' is only valid inside function body", terminationString.c_str()), statement);
                return false;
            }
            compiler->branchReturns = true;

            if (statement->type == TerminateStatement::TerminationType::Return)
            {
                Function* functionOwner = static_cast<Function*>(scopeOwner);
                Function::__Resolved* functionOwnerResolved = Symbol::Resolved(functionOwner);
                bool ret = true;
                if (statement->returnValue != nullptr)
                {
                    ret = statement->returnValue->Resolve(compiler);
                
                    Type::FullType type;
                    statement->returnValue->EvalType(type);
                    if (functionOwner->returnType != type)
                    {
                        compiler->Error(Format("Function expects return of type '%s', got '%s'", functionOwner->returnType.ToString().c_str(), type.ToString().c_str()), statement);
                        return false;
                    }
                }
                else
                {
                    if (functionOwner->returnType != Type::FullType{ ConstantString("void") })
                    {
                        compiler->Error(Format("Function expects return of type '%s', got 'void'", functionOwner->returnType.ToString().c_str()), statement);
                        return false;
                    }
                }
                compiler->MarkScopeUnreachable();
                return ret;
            }
            compiler->MarkScopeUnreachable();
            return true;
        }
        case Symbol::ScopeStatementType:
        {
            auto statement = reinterpret_cast<ScopeStatement*>(symbol);
            if (compiler->generationState.active)
            {
                // If generating, resolve the symbols as if there was no scope
                for (Symbol* sym : statement->symbols)
                {
                    if (sym->symbolType == Symbol::VariableType)
                    {
                        if (!this->ResolveVariable(compiler, sym))
                            return false;

                        Symbol::Resolved(compiler->generationState.owner)->generatedSymbols.Append(sym);
                    }
                    else if (sym->symbolType == Symbol::FunctionType)
                    {
                        if (!compiler->generationState.active)
                        {
                            compiler->Error(Format("Functions can only be declared in statement blocks if the block is generating code"), symbol);
                            return false;
                        }

                        // Temporarily disable since functions shouldn't be generating code
                        compiler->generationState.active = false;
                        if (!this->ResolveFunction(compiler, sym))
                            return false;
                        compiler->generationState.active = true;

                        Symbol::Resolved(compiler->generationState.owner)->generatedSymbols.Append(sym);
                    }
                    else if (sym->symbolType == Symbol::AliasType)
                    {
                        if (!this->ResolveAlias(compiler, sym))
                            return false;
                        Symbol::Resolved(compiler->generationState.owner)->generatedSymbols.Append(sym);
                    }
                    else
                    {
                        if (!this->ResolveStatement(compiler, sym))
                            return false;
                    }
                }
            }
            else
            {
                auto statementRes = Symbol::Resolved(statement);
                statementRes->scope.owningSymbol = compiler->GetScopeOwner();
                Compiler::LocalScope scope = Compiler::LocalScope::MakeLocalScope(compiler, &statementRes->scope);
                
                for (Symbol* sym : statement->symbols)
                {
                    if (sym->symbolType == Symbol::VariableType)
                    {
                        if (!this->ResolveVariable(compiler, sym))
                            return false;
                    }
                    else
                    {
                        if (!this->ResolveStatement(compiler, sym))
                            return false;
                    }
                }
                for (Expression* expr : statement->unfinished)
                {
                    if (!expr->Resolve(compiler))
                        return false;
                }
            }
            return true;
        }
        case Symbol::SwitchStatementType:
        {
            if (compiler->generationState.active)
            {
                compiler->Error(Format("Code generation blocks don't support switches"), symbol);
                return false;
            }
            auto statement = reinterpret_cast<SwitchStatement*>(symbol);
            auto statementRes = Symbol::Resolved(statement);
            statementRes->scope.owningSymbol = statement;
            Compiler::LocalScope scope = Compiler::LocalScope::MakeLocalScope(compiler, &statementRes->scope);

            if (statement->switchExpression->Resolve(compiler))
            {
                Type::FullType type;
                statement->switchExpression->EvalType(type);
                Type* typeSymbol = compiler->GetType(type);
                if (typeSymbol->baseType != TypeCode::Int && typeSymbol->baseType != TypeCode::UInt && typeSymbol->category != Type::Category::EnumCategory)
                {
                    compiler->Error(Format("Switch condition must evaluate to an integer value"), statement);
                    return false;
                }
            }
            else
            {
                return false;
            }

            bool switchReturns = true;
            for (size_t i = 0; i < statement->caseExpressions.size; i++)
            {
                if (!statement->caseExpressions[i]->Resolve(compiler))
                {
                    return false;
                }

                compiler->branchReturns = false;
                if (statement->caseStatements[i] != nullptr)
                {
                    if (!this->ResolveStatement(compiler, statement->caseStatements[i]))
                    {
                        return false;
                    }
                    switchReturns &= compiler->branchReturns;
                }
                compiler->branchReturns = false;
                compiler->MarkScopeReachable();
            }
            if (statement->defaultStatement)
            {
                if (!this->ResolveStatement(compiler, statement->defaultStatement))
                    return false;
                switchReturns &= compiler->branchReturns;
                compiler->branchReturns = false;
            }
            compiler->branchReturns = switchReturns;
            return true;
        }
        case Symbol::WhileStatementType:
        {
            if (compiler->generationState.active)
            {
                compiler->Error(Format("Code generation blocks don't loops"), symbol);
                return false;
            }
            auto statement = reinterpret_cast<WhileStatement*>(symbol);
            auto statementRes = Symbol::Resolved(statement);

            statementRes->scope.owningSymbol = statement;
            Compiler::LocalScope scope = Compiler::LocalScope::MakeLocalScope(compiler, &statementRes->scope);
            if (statement->condition->Resolve(compiler))
            {
                // Convert condition if not bool
                Type::FullType conditionType;
                statement->condition->EvalType(conditionType);
                Type* typeSymbol;
                statement->condition->EvalTypeSymbol(typeSymbol);
                if (typeSymbol->baseType != TypeCode::Bool)
                {
                    Symbol* conversionFunction = compiler->GetSymbol(Format("b8(%s)", conditionType.name.c_str()));
                    if (conversionFunction == nullptr)
                    {
                        compiler->Error(Format("Condition of type '%s' must evaluate to a boolean expression", conditionType.ToString().c_str()), statement);
                        return false;
                    }

                    // Replace condition with a call expression to the conversion function
                    FixedArray<Expression*> arguments = { statement->condition };
                    Symbol::Location loc = statement->condition->location;
                    statement->condition = Alloc<CallExpression>(Alloc<SymbolExpression>(conversionFunction->name), arguments);
                    statement->condition->location = loc;

                    // Resolve again
                    if (!statement->condition->Resolve(compiler))
                        return false;
                }
            }
            else
            {
                return false;
            }

            return this->ResolveStatement(compiler, statement->statement);
        }
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
Validator::ResolveGenerate(Compiler* compiler, Symbol* symbol)
{
    Generate* gen = static_cast<Generate*>(symbol);
    Generate::__Resolved* genResolved = gen->thisResolved;
    
    if (compiler->generationState.active)
    {
        compiler->Error("Generate blocks can not be recursive", symbol);
        return false;
    }
    compiler->generationState.active = true;
    compiler->generationState.branchActive = true;
    compiler->generationState.owner = gen;
    for (Symbol* sym : gen->symbols)
    {
        if (sym->symbolType == Symbol::VariableType)
        {
            if (!this->ResolveVariable(compiler, sym))
                return false;

            genResolved->generatedSymbols.Append(sym);
        }
        else if (sym->symbolType == Symbol::AliasType)
        {
            if (!this->ResolveAlias(compiler, sym))
                return false;

            genResolved->generatedSymbols.Append(sym);
        }
        else if (sym->symbolType == Symbol::FunctionType)
        {
            if (!this->ResolveFunction(compiler, sym))
                return false;

            genResolved->generatedSymbols.Append(sym);
        }
        else
        {
            if (!this->ResolveStatement(compiler, sym))
                return false;
        }
    }
    compiler->generationState.active = false;
    compiler->generationState.branchActive = false;
    compiler->generationState.owner = nullptr;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
Validator::ValidateFunction(Compiler* compiler, Symbol* symbol)
{
    Function* fun = static_cast<Function*>(symbol);
    Function::__Resolved* funResolved = Symbol::Resolved(fun);

    for (Variable* var : fun->parameters)
    {
        // if function is used for shader, validate parameters with special rules
        if (compiler->currentState.shaderType != ProgramInstance::__Resolved::InvalidProgramEntryType)
        {
            Variable::__Resolved* varResolved = Symbol::Resolved(var);

            if (varResolved->parameterBits.flags.isPatch
                && !(compiler->currentState.shaderType == ProgramInstance::__Resolved::HullShader || compiler->currentState.shaderType == ProgramInstance::__Resolved::DomainShader))
            {
                compiler->Error(Format("Parameter '%s' can not use 'patch' if function is not being used as a HullShader/TessellationControlShader or DomainShader/TessellationEvaluationShader", var->name.c_str(), fun->name.c_str()), var);
                return false;
            }

            if (varResolved->parameterBits.flags.isNoInterpolate
                && compiler->currentState.shaderType != ProgramInstance::__Resolved::PixelShader)
            {
                compiler->Error(Format("Parameter '%s' can not use 'no_interpolate' if function is not being used as a PixelShader", var->name.c_str(), fun->name.c_str()), var);
                return false;
            }

            if (varResolved->parameterBits.flags.isNoPerspective
                && compiler->currentState.shaderType != ProgramInstance::__Resolved::PixelShader)
            {
                compiler->Error(Format("Parameter '%s' can not use 'no_perspective' if function is not being used as a PixelShader", var->name.c_str(), fun->name.c_str()), var);
                return false;
            }
        }
    }

    if (compiler->currentState.shaderType == ProgramInstance::__Resolved::HullShader)
    {
        if (funResolved->executionModifiers.maxOutputVertices == Function::__Resolved::INVALID_SIZE)
        {
            compiler->Error(Format("Hull shader '%s' does not define 'patch_size'", fun->name.c_str()), symbol);
            return false;
        }

        if (funResolved->executionModifiers.inputPrimitiveTopology == Function::__Resolved::PrimitiveTopology::Points)
        {
            compiler->Error(Format("Hull shader '%s' doesn't support input topology 'points", fun->name.c_str()), symbol);
            return false;
        }

        if (funResolved->executionModifiers.inputPrimitiveTopology == Function::__Resolved::PrimitiveTopology::Lines)
        {
            compiler->Error(Format("Hull shader '%s' doesn't support input topology 'lines", fun->name.c_str()), symbol);
            return false;
        }

        if (funResolved->executionModifiers.inputPrimitiveTopology == Function::__Resolved::PrimitiveTopology::LinesAdjacency)
        {
            compiler->Error(Format("Hull shader '%s' doesn't support input topology 'lines_adjacency", fun->name.c_str()), symbol);
            return false;
        }

        if (funResolved->executionModifiers.inputPrimitiveTopology == Function::__Resolved::PrimitiveTopology::TrianglesAdjacency)
        {
            compiler->Error(Format("Hull shader '%s' doesn't support input topology 'triangles_adjacency", fun->name.c_str()), symbol);
            return false;
        }
        if (funResolved->executionModifiers.partitionMethod != Function::__Resolved::InvalidPartitionMethod)
        {
            compiler->Warning(Format("Domain shader '%s' does not define 'partition', defaulting to 'integer'", fun->name.c_str()), symbol);
            funResolved->executionModifiers.partitionMethod = Function::__Resolved::PartitionMethod::IntegerSteps;
        }
    }

    if (compiler->currentState.shaderType == ProgramInstance::__Resolved::DomainShader)
    {
        // validate required qualifiers
        if (funResolved->executionModifiers.patchType != Function::__Resolved::InvalidPatchType)
        {
            compiler->Warning(Format("Domain shader '%s' does not define 'patch_type' for DomainShader/TessellationEvaluationShader, defaulting to 'triangles'", fun->name.c_str()), symbol);
            funResolved->executionModifiers.patchType = Function::__Resolved::PatchType::TrianglePatch;
        }

    }

    if (compiler->currentState.shaderType == ProgramInstance::__Resolved::GeometryShader)
    {
        if (funResolved->executionModifiers.maxOutputVertices == Function::__Resolved::INVALID_SIZE)
        {
            compiler->Error(Format("Geometry shader '%s' does not define 'max_output_vertices' for GeometryShader", fun->name.c_str()), symbol);
            return false;
        }
        if (funResolved->executionModifiers.inputPrimitiveTopology == Function::__Resolved::InvalidPrimitiveTopology)
        {
            compiler->Warning(Format("Geometry shader '%s' does not define 'input_topology' for GeometryShader, defaulting to 'triangles'", fun->name.c_str()), symbol);
            funResolved->executionModifiers.inputPrimitiveTopology = Function::__Resolved::Triangles;
        }
        if (funResolved->executionModifiers.outputPrimitiveTopology == Function::__Resolved::InvalidPrimitiveTopology)
        {
            compiler->Warning(Format("Geometry shader '%s' does not define 'output_topology' for GeometryShader, defaulting to 'triangles'", fun->name.c_str()), symbol);
            funResolved->executionModifiers.outputPrimitiveTopology = Function::__Resolved::Triangles;
        }

        if (funResolved->executionModifiers.inputPrimitiveTopology == Function::__Resolved::PrimitiveTopology::Quads)
        {
            compiler->Error(Format("Geometry shader '%s' doesn't support input topology 'quads", fun->name.c_str()), symbol);
            return false;
        }

        if (funResolved->executionModifiers.inputPrimitiveTopology == Function::__Resolved::PrimitiveTopology::Isolines)
        {
            compiler->Error(Format("Geometry shader '%s' doesn't support input topology 'isolines", fun->name.c_str()), symbol);
            return false;
        }
    }

    if (compiler->currentState.shaderType == ProgramInstance::__Resolved::ComputeShader)
    {
        if (funResolved->executionModifiers.computeShaderWorkGroupSize[0] <= 0)
        {
            compiler->Error(Format("Compute shader must declare 'local_size_x' bigger than or equal to 1", fun->name.c_str()), symbol);
            return false;
        }
        if (funResolved->executionModifiers.computeShaderWorkGroupSize[1] <= 0)
        {
            compiler->Error(Format("Compute shader must declare 'local_size_y' bigger than or equal to 1", fun->name.c_str()), symbol);
            return false;
        }
        if (funResolved->executionModifiers.computeShaderWorkGroupSize[2] <= 0)
        {
            compiler->Error(Format("Compute shader must declare 'local_size_z' bigger than or equal to 1", fun->name.c_str()), symbol);
            return false;
        }
        if (funResolved->executionModifiers.groupSize <= 0)
        {
            compiler->Error(Format("Compute shader must declare 'group_size' bigger than or equal to 1", fun->name.c_str()), symbol);
            return false;
        }
        if (funResolved->executionModifiers.groupsPerWorkgroup <= 0)
        {
            compiler->Error(Format("Compute shader must declare 'groups_per_workgroup' bigger than or equal to 1", fun->name.c_str()), symbol);
            return false;
        }
    }

    if (compiler->currentState.shaderType == ProgramInstance::__Resolved::TaskShader)
    {
        if (funResolved->executionModifiers.computeShaderWorkGroupSize[1] > 1)
        {
            compiler->Error(Format("Task shader must declare 'local_size_y' as 1", fun->name.c_str()), symbol);
            return false;
        }
        if (funResolved->executionModifiers.computeShaderWorkGroupSize[2] > 1)
        {
            compiler->Error(Format("Task shader must declare 'local_size_z' as 1", fun->name.c_str()), symbol);
            return false;
        }
    }

    if (compiler->currentState.shaderType == ProgramInstance::__Resolved::MeshShader)
    {
        if (funResolved->executionModifiers.computeShaderWorkGroupSize[0] <= 0)
        {
            compiler->Error(Format("Mesh shader must declare 'local_size_x' bigger than or equal to 1", fun->name.c_str()), symbol);
            return false;
        }
        if (funResolved->executionModifiers.computeShaderWorkGroupSize[1] <= 0)
        {
            compiler->Error(Format("Mesh shader must declare 'local_size_y' bigger than or equal to 1", fun->name.c_str()), symbol);
            return false;
        }
        if (funResolved->executionModifiers.computeShaderWorkGroupSize[2] <= 0)
        {
            compiler->Error(Format("Mesh shader must declare 'local_size_z' bigger than or equal to 1", fun->name.c_str()), symbol);
            return false;
        }
        if (funResolved->executionModifiers.groupSize <= 0)
        {
            compiler->Error(Format("Mesh shader must declare 'group_size' bigger than or equal to 1", fun->name.c_str()), symbol);
            return false;
        }
        if (funResolved->executionModifiers.groupsPerWorkgroup <= 0)
        {
            compiler->Error(Format("Mesh shader must declare 'groups_per_workgroup' bigger than or equal to 1", fun->name.c_str()), symbol);
            return false;
        }
    }

    if (compiler->currentState.shaderType != ProgramInstance::__Resolved::ComputeShader && compiler->currentState.shaderType != ProgramInstance::__Resolved::TaskShader && compiler->currentState.shaderType != ProgramInstance::__Resolved::MeshShader)
    {
        if (funResolved->executionModifiers.computeDerivativeIndexing != Function::__Resolved::NoDerivatives)
        {
            compiler->Error("Setting the derivative indexing method is only allowed on shaders with explicit work group sizes", symbol);
            return false;
        }
    }
    return true;
}

//------------------------------------------------------------------------------
/**
    First filters parameters based on in/out qualifiers, then sorts within that set
    based on either inBinding or outBinding respectively
*/
TransientArray<Variable*>
SortAndFilterParameters(const FixedArray<Variable*>& vars, bool in)
{
    TransientArray<Variable*> ret(vars.size);
    for (Variable* var : vars)
    {
        Variable::__Resolved* varResolved = Symbol::Resolved(var);
        if (varResolved->storage == Storage::Input && in)
            ret.Append(var);
        else if (varResolved->storage == Storage::Output && !in)
            ret.Append(var);
    }

    // sort based on in or out binding respectively
    std::sort(ret.begin(), ret.end(), [in](Variable* a, Variable* b)
    {
        Variable::__Resolved* aResolved = Symbol::Resolved(a);
        Variable::__Resolved* bResolved = Symbol::Resolved(b);
        return in ? aResolved->inBinding < bResolved->inBinding : aResolved->outBinding < bResolved->outBinding;
    });
    return ret;
}

//------------------------------------------------------------------------------
/**
    TODO: Add program to better describe when unused bindings happen. Also add a compiler flag to turning the warnings off
*/
bool 
ValidateParameterSets(Compiler* compiler, Function* outFunc, Function* inFunc)
{
    TransientArray<Variable*> outParams = SortAndFilterParameters(outFunc->parameters, false);
    TransientArray<Variable*> inParams = SortAndFilterParameters(inFunc->parameters, true);
    size_t inIterator = 0;
    for (Variable* var : outParams)
    {
        Variable::__Resolved* outResolved = Symbol::Resolved(var);
        Variable::__Resolved* inResolved = Symbol::Resolved(inParams.ptr[inIterator]);

        // if bindings don't match, it means the output will be unused since the parameter sets should be sorted
        if ((outResolved->outBinding != inResolved->inBinding))
        {
            if (compiler->options.warnOnUnusedParameter)
                compiler->Warning(Format("Unused parameter '%s' (binding %d) from shader '%s' to '%s'", var->name.c_str(), outResolved->outBinding, outFunc->name.c_str(), inFunc->name.c_str()), outFunc);
        }
        else
        {
            if (var->type != inParams.ptr[inIterator]->type)
            {
                compiler->Error(Format("Can't match types '%s' and '%s' between shader '%s' and '%s'", var->type.name.c_str(), inParams.ptr[inIterator]->type.name.c_str(), outFunc->name.c_str(), inFunc->name.c_str()), outFunc);
                return false;
            }
        }
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
Validator::ValidateProgram(Compiler* compiler, Symbol* symbol)
{
    ProgramInstance* prog = static_cast<ProgramInstance*>(symbol);
    ProgramInstance::__Resolved* progResolved = Symbol::Resolved(prog);

    if (progResolved->usage.flags.hasHullShader
        && !progResolved->usage.flags.hasDomainShader)
    {
        compiler->Error(Format("HullShader/TessellationControlShader is not allowed without a DomainShader/TessellationEvaluationShader"), symbol);
        return false;
    }

    if (progResolved->usage.flags.hasHullShader
        && !progResolved->usage.flags.hasVertexShader)
    {
        compiler->Error(Format("HullShader/TessellationControlShader is not allowed without a VertexShader"), symbol);
        return false;
    }

    if (progResolved->usage.flags.hasGeometryShader
        && !progResolved->usage.flags.hasVertexShader)
    {
        compiler->Error(Format("GeometryShader is not allowed without a VertexShader"), symbol);
        return false;
    }

    enum ProgramType
    {
        NotSet,
        IsGraphics,
        IsCompute,
        IsRaytracing,
    };
    ProgramType programType = ProgramType::NotSet;

    // validate program setup as compute or graphics program, do it on a first-come-first-serve basis
    for (uint32_t mapping = 0; mapping < ProgramInstance::__Resolved::EntryType::NumProgramEntries; mapping++)
    {
        Symbol* object = progResolved->mappings[mapping];
        if (object == nullptr)
            continue;

        if (mapping == ProgramInstance::__Resolved::ComputeShader)
        {
            if (programType == ProgramType::IsGraphics)
            {
                compiler->Error(Format("Program may not be both compute and general graphics"), symbol);
                return false;
            }
            if (programType == ProgramType::IsRaytracing)
            {
                compiler->Error(Format("Program may not be both compute and raytracing"), symbol);
                return false;
            }
            programType = ProgramType::IsCompute;
        }
        else if (mapping == ProgramInstance::__Resolved::VertexShader
            || mapping == ProgramInstance::__Resolved::HullShader
            || mapping == ProgramInstance::__Resolved::DomainShader
            || mapping == ProgramInstance::__Resolved::GeometryShader
            || mapping == ProgramInstance::__Resolved::PixelShader
            || mapping == ProgramInstance::__Resolved::TaskShader
            || mapping == ProgramInstance::__Resolved::MeshShader
            )
        {
            if (programType == ProgramType::IsCompute)
            {
                compiler->Error("Program may not be both general graphics and compute", symbol);
                return false;
            }
            if (programType == ProgramType::IsRaytracing)
            {
                compiler->Error("Program may not be both general graphics and ray tracing", symbol);
                return false;
            }
            programType = ProgramType::IsGraphics;
        }
        else if (mapping == ProgramInstance::__Resolved::RayAnyHitShader
            || mapping == ProgramInstance::__Resolved::RayCallableShader
            || mapping == ProgramInstance::__Resolved::RayIntersectionShader
            || mapping == ProgramInstance::__Resolved::RayMissShader)
        {
            if (programType == ProgramType::IsCompute)
            {
                compiler->Error("Program may not be both raytracing and compute", symbol);
                return false;
            }
            if (programType == ProgramType::IsGraphics)
            {
                compiler->Error("Program may not be both raytracing and general graphics", symbol);
                return false;
            }
            programType = ProgramType::IsRaytracing;
        }
    }

    // if a part of the graphics pipeline, go through the different stages and validate them
    if (!progResolved->usage.flags.hasComputeShader)
    {
        Function* lastPrimitiveShader = nullptr;
        if (progResolved->usage.flags.hasVertexShader)
        {
            Function* vs = static_cast<Function*>(progResolved->mappings[ProgramInstance::__Resolved::VertexShader]);
            lastPrimitiveShader = vs;
        }

        if (progResolved->usage.flags.hasHullShader)
        {
            if (lastPrimitiveShader == nullptr)
            {
                compiler->Error(Format("Invalid program setup, HullShader/TessellationControlShader needs a VertexShader"), symbol);
                return false;
            }
            Function* hs = static_cast<Function*>(progResolved->mappings[ProgramInstance::__Resolved::HullShader]);
            if (!ValidateParameterSets(compiler, lastPrimitiveShader, hs))
                return false;

            lastPrimitiveShader = hs;
        }

        if (progResolved->usage.flags.hasDomainShader)
        {
            if (lastPrimitiveShader == nullptr 
                && progResolved->mappings[ProgramInstance::__Resolved::HullShader] != nullptr)
            {
                compiler->Error(Format("Invalid program setup, DomainShader needs a HullShader/TessellationControlShader"), symbol);
                return false;
            }
            Function* ds = static_cast<Function*>(progResolved->mappings[ProgramInstance::__Resolved::DomainShader]);
            if (!ValidateParameterSets(compiler, lastPrimitiveShader, ds))
                return false;

            lastPrimitiveShader = ds;
        }

        if (progResolved->usage.flags.hasGeometryShader)
        {
            if (lastPrimitiveShader == nullptr)
            {
                compiler->Error(Format("Invalid program setup, GeometryShader needs either a VertexShader or a VertexShader-HullShader/TessellationControlShader-DomainShader/TessellationEvaluationShader setup"), symbol);
                return false;
            }
            Function* gs = static_cast<Function*>(progResolved->mappings[ProgramInstance::__Resolved::GeometryShader]);
            if (!ValidateParameterSets(compiler, lastPrimitiveShader, gs))
                return false;

            lastPrimitiveShader = gs;
        }

        if (progResolved->usage.flags.hasPixelShader)
        {
            if (lastPrimitiveShader == nullptr)
            {
                compiler->Error(Format("Invalid program setup, PixelShader needs either a VertexShader, a VertexShader-GeometryShader, a VertexShader-HullShader/TessellationControlShader-DomainShader/TessellationEvaluationShader or a VertexShader-HullShader/TessellationControlShader-DomainShader/TessellationEvaluationShader-GeometryShader setup"), symbol);
                return false;
            }
            Function* ps = static_cast<Function*>(progResolved->mappings[ProgramInstance::__Resolved::PixelShader]);
            if (!ValidateParameterSets(compiler, lastPrimitiveShader, ps))
                return false;
        }
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
Validator::ValidateType(Compiler* compiler, const Type::FullType& type, Type* typeSymbol, Symbol* sym)
{
    uint32_t numPointers = 0;
    for (auto mod : type.modifiers)
    {
        if (mod == Type::FullType::Modifier::Invalid)
        {
            compiler->Error(Format("Invalid type modifier"), sym);
            return false;
        }
        if (mod == Type::FullType::Modifier::Pointer)
            numPointers++;
    }

    for (auto expr : type.modifierValues)
    {
        if (expr != nullptr)
        {
            ValueUnion dummy;
            if (!expr->EvalValue(dummy))
            {
                compiler->Error(Format("Array modifier must be a literal or compile time value"), sym);
                return false;
            }
        }
    }

    if (!compiler->target.supportsPhysicalAddressing)
    {
        if (numPointers > 1)
        {
            compiler->Error(Format("Target language %s does not support dereferencing.", compiler->target.name.c_str()), sym);
            return false;
        }
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
Validator::ResolveVisibility(Compiler* compiler, Symbol* symbol)
{
    bool res = true;
    switch (symbol->symbolType)
    {
        case Symbol::ScopeStatementType:
        {
            ScopeStatement* scope = static_cast<ScopeStatement*>(symbol);
            for (auto* statement : scope->symbols)
            {
                res |= this->ResolveVisibility(compiler, statement);
            }
            break;
        }
        case Symbol::ForStatementType:
        {
            ForStatement* forStat = static_cast<ForStatement*>(symbol);

            // If condition can be evaluated to false, don't run loop
            ValueUnion val;
            if (forStat->condition->EvalValue(val) && !val.b[0])
                break;
            
            for (auto* var : forStat->declarations)
                res |= this->ResolveVisibility(compiler, var);
            res |= this->ResolveVisibility(compiler, forStat->condition);
            res |= this->ResolveVisibility(compiler, forStat->loop);
            res |= this->ResolveVisibility(compiler, forStat->contents);
            break;
        }
        case Symbol::WhileStatementType:
        {
            WhileStatement* whileStat = static_cast<WhileStatement*>(symbol);

            // If static evaluation turns out the while is meaningless, end early
            ValueUnion val;
            if (whileStat->condition->EvalValue(val) && !val.b[0])
                break;
                
            res |= this->ResolveVisibility(compiler, whileStat->condition);
            res |= this->ResolveVisibility(compiler, whileStat->statement);    
            break;
        }
        case Symbol::TerminateStatementType:
        {
            TerminateStatement* termStat = static_cast<TerminateStatement*>(symbol);
            if (termStat->type == TerminateStatement::TerminationType::Return)
            {
                if (termStat->returnValue != nullptr)
                    res |= this->ResolveVisibility(compiler, termStat->returnValue);
            }
            else if (termStat->type == TerminateStatement::TerminationType::Discard)
            {
                compiler->currentState.sideEffects.flags.killsPixel = true;
                if (compiler->currentState.shaderType != ProgramInstance::__Resolved::EntryType::PixelShader)
                {
                    compiler->Error(Format("'discard' can only be used from a pixel shader"), termStat);
                    return false;
                }    
            }
            else if (termStat->type == TerminateStatement::TerminationType::RayTerminate)
            {
                compiler->currentState.sideEffects.flags.stopsRay = true;
                if (compiler->currentState.shaderType != ProgramInstance::__Resolved::EntryType::RayAnyHitShader)
                {
                    compiler->Error(Format("'ray_terminate' can only be used in a ray anyhit shader"), termStat);
                    return false;
                }    
            }
            else if (termStat->type == TerminateStatement::TerminationType::RayIgnoreIntersection)
            {
                compiler->currentState.sideEffects.flags.ignoresRay = true;
                if (compiler->currentState.shaderType != ProgramInstance::__Resolved::EntryType::RayAnyHitShader)
                {
                    compiler->Error(Format("'ray_ignore' can only be used in a ray anyhit"), termStat);
                    return false;
                }    
            }
            compiler->MarkScopeUnreachable();
            break;
        }

        case Symbol::ExpressionStatementType:
        {
            ExpressionStatement* exprStat = static_cast<ExpressionStatement*>(symbol);
            for (auto& expr : exprStat->expressions)
                res |= this->ResolveVisibility(compiler, expr);
            break;
        }
        case Symbol::IfStatementType:
        {
            IfStatement* ifStat = static_cast<IfStatement*>(symbol);
            ValueUnion val;
            if (ifStat->condition->EvalValue(val))
            {
                if (val.b[0])
                    res |= this->ResolveVisibility(compiler, ifStat->ifStatement);
                else if (ifStat->elseStatement != nullptr)
                    res |= this->ResolveVisibility(compiler, ifStat->elseStatement);
            }
            else
            {
                res |= this->ResolveVisibility(compiler, ifStat->condition);

                res |= this->ResolveVisibility(compiler, ifStat->ifStatement);
                if (ifStat->elseStatement != nullptr)
                    res |= this->ResolveVisibility(compiler, ifStat->elseStatement);
            }
            break;
        }
        case Symbol::TernaryExpressionType:
        {
            TernaryExpression* ternExp = static_cast<TernaryExpression*>(symbol);
            ValueUnion val;
            if (ternExp->lhs->EvalValue(val))
            {
                if (val.b[0])
                    res |= this->ResolveVisibility(compiler, ternExp->ifExpression);
                else
                    res |= this->ResolveVisibility(compiler, ternExp->elseExpression);
            }
            else
            {
                res |= this->ResolveVisibility(compiler, ternExp->lhs);
                res |= this->ResolveVisibility(compiler, ternExp->ifExpression);
                res |= this->ResolveVisibility(compiler, ternExp->elseExpression);
            }
            break;
        }
        case Symbol::SwitchStatementType:
        {
            SwitchStatement* switchStat = static_cast<SwitchStatement*>(symbol);
            ValueUnion val;
            if (switchStat->switchExpression->EvalValue(val))
            {
                if (val.i[0] < switchStat->caseExpressions.size)
                    res |= this->ResolveVisibility(compiler, switchStat->caseStatements[val.i[0]]);
                else if (switchStat->defaultStatement != nullptr)
                    res |= this->ResolveVisibility(compiler, switchStat->defaultStatement);
            }
            else
            {
                for (uint32_t i = 0; i < switchStat->caseStatements.size; i++)
                {
                    res |= this->ResolveVisibility(compiler, switchStat->caseExpressions[i]);
                    res |= this->ResolveVisibility(compiler, switchStat->caseStatements[i]);
                    if (switchStat->defaultStatement != nullptr)
                        res |= this->ResolveVisibility(compiler, switchStat->defaultStatement);         
                }
            }
            break;
        }
        case Symbol::BinaryExpressionType:
        {
            BinaryExpression* binExp = static_cast<BinaryExpression*>(symbol);
            auto binExpRes = Symbol::Resolved(binExp);
            res |= this->ResolveVisibility(compiler, binExp->left);
            res |= this->ResolveVisibility(compiler, binExp->right);
            if (binExpRes->constValueExpression != nullptr)
                res |= this->ResolveVisibility(compiler, binExpRes->constValueExpression);
            break;
        }
        case Symbol::AccessExpressionType:
        {
            auto access = static_cast<AccessExpression*>(symbol);
            res |= this->ResolveVisibility(compiler, access->left);
            res |= this->ResolveVisibility(compiler, access->right);
            break;
        }
        case Symbol::ArrayIndexExpressionType:
        {
            auto arrayExpr = static_cast<ArrayIndexExpression*>(symbol);
            res |= this->ResolveVisibility(compiler, arrayExpr->left);
            res |= this->ResolveVisibility(compiler, arrayExpr->right);
            break;
        }
        case Symbol::CallExpressionType:
        {
            auto callExpr = static_cast<CallExpression*>(symbol);
            auto callResolved = Symbol::Resolved(callExpr);
            
            struct IntrinsicsShaderMask
            {
                StaticArray<ProgramInstance::__Resolved::EntryType, ProgramInstance::__Resolved::EntryType::NumProgramEntries> entries;
                Compiler::State::SideEffects::Masks sideEffect;
                
                IntrinsicsShaderMask()
                    : sideEffect(Compiler::State::SideEffects::Masks{0x0})
                {}
                
                IntrinsicsShaderMask(const std::initializer_list<ProgramInstance::__Resolved::EntryType>& entries, Compiler::State::SideEffects::Masks sideEffects)
                    : entries(entries)
                    , sideEffect(sideEffects)
                {}
            };

            static const StaticMap allowedBuiltins =
            std::array{
                std::pair{ ConstantString("vertexExportCoordinates"), IntrinsicsShaderMask{ { ProgramInstance::__Resolved::EntryType::VertexShader }, Compiler::State::SideEffects::Masks::EXPORT_VERTEX_POSITION_BIT }}
                , std::pair{ ConstantString("geometryExportVertex"), IntrinsicsShaderMask{ { ProgramInstance::__Resolved::EntryType::GeometryShader }, Compiler::State::SideEffects::Masks::EXPORT_VERTEX_BIT }}
                , std::pair{ ConstantString("geometryExportPrimitive"), IntrinsicsShaderMask{ { ProgramInstance::__Resolved::EntryType::GeometryShader }, Compiler::State::SideEffects::Masks::EXPORT_PRIMITIVE_BIT }}
                , std::pair{ ConstantString("computeGetLocalInvocationIndex"), IntrinsicsShaderMask{ { ProgramInstance::__Resolved::EntryType::ComputeShader }, Compiler::State::SideEffects::Masks() }}
                , std::pair{ ConstantString("computeGetGlobalInvocationIndex"), IntrinsicsShaderMask{ { ProgramInstance::__Resolved::EntryType::ComputeShader }, Compiler::State::SideEffects::Masks() }}
                , std::pair{ ConstantString("computeGetWorkGroupIndex"), IntrinsicsShaderMask{ { ProgramInstance::__Resolved::EntryType::ComputeShader }, Compiler::State::SideEffects::Masks() }}
                , std::pair{ ConstantString("computeGetWorkGroupDimensions"), IntrinsicsShaderMask{ { ProgramInstance::__Resolved::EntryType::ComputeShader }, Compiler::State::SideEffects::Masks() }}
                , std::pair{ ConstantString("vertexSetOutputLayer"), IntrinsicsShaderMask{ { ProgramInstance::__Resolved::EntryType::VertexShader }, Compiler::State::SideEffects::Masks::SET_OUTPUT_LAYER_BIT }}
                , std::pair{ ConstantString("vertexSetOutputViewport"), IntrinsicsShaderMask{ { ProgramInstance::__Resolved::EntryType::VertexShader }, Compiler::State::SideEffects::Masks::SET_VIEWPORT_BIT }}
                , std::pair{ ConstantString("pixelExportColor"), IntrinsicsShaderMask{ { ProgramInstance::__Resolved::EntryType::PixelShader }, Compiler::State::SideEffects::Masks::EXPORT_PIXEL_BIT }}
                , std::pair{ ConstantString("pixelGetDepth"), IntrinsicsShaderMask{ { ProgramInstance::__Resolved::EntryType::PixelShader }, Compiler::State::SideEffects::Masks() }}
                , std::pair{ ConstantString("pixelSetDepth"), IntrinsicsShaderMask{ { ProgramInstance::__Resolved::EntryType::PixelShader }, Compiler::State::SideEffects::Masks::EXPORT_DEPTH_BIT }}
                , std::pair{ ConstantString("rayTrace"), IntrinsicsShaderMask{ { ProgramInstance::__Resolved::EntryType::RayGenerationShader }, Compiler::State::SideEffects::Masks() }}
                , std::pair{ ConstantString("rayExportIntersection"), IntrinsicsShaderMask{ { ProgramInstance::__Resolved::EntryType::RayIntersectionShader }, Compiler::State::SideEffects::Masks() }}
                , std::pair{ ConstantString("rayExecuteCallable"), IntrinsicsShaderMask{ { ProgramInstance::__Resolved::EntryType::RayGenerationShader, ProgramInstance::__Resolved::EntryType::RayClosestHitShader, ProgramInstance::__Resolved::EntryType::RayMissShader, ProgramInstance::__Resolved::EntryType::RayCallableShader }, Compiler::State::SideEffects::Masks() }}
                , std::pair{ ConstantString("rayGetLaunchIndex"), IntrinsicsShaderMask{ { ProgramInstance::__Resolved::EntryType::RayGenerationShader, ProgramInstance::__Resolved::EntryType::RayClosestHitShader, ProgramInstance::__Resolved::EntryType::RayMissShader, ProgramInstance::__Resolved::EntryType::RayAnyHitShader, ProgramInstance::__Resolved::EntryType::RayIntersectionShader  }, Compiler::State::SideEffects::Masks() }}
                , std::pair{ ConstantString("rayGetLaunchSize"), IntrinsicsShaderMask{ { ProgramInstance::__Resolved::EntryType::RayGenerationShader, ProgramInstance::__Resolved::EntryType::RayClosestHitShader, ProgramInstance::__Resolved::EntryType::RayMissShader, ProgramInstance::__Resolved::EntryType::RayAnyHitShader, ProgramInstance::__Resolved::EntryType::RayIntersectionShader  }, Compiler::State::SideEffects::Masks() }}
                , std::pair{ ConstantString("blasGetPrimitiveIndex"), IntrinsicsShaderMask{ { ProgramInstance::__Resolved::EntryType::RayClosestHitShader, ProgramInstance::__Resolved::EntryType::RayAnyHitShader, ProgramInstance::__Resolved::EntryType::RayIntersectionShader  }, Compiler::State::SideEffects::Masks() }}
                , std::pair{ ConstantString("blasGetGeometryIndex"), IntrinsicsShaderMask{ { ProgramInstance::__Resolved::EntryType::RayClosestHitShader, ProgramInstance::__Resolved::EntryType::RayAnyHitShader, ProgramInstance::__Resolved::EntryType::RayIntersectionShader  }, Compiler::State::SideEffects::Masks() }}
                , std::pair{ ConstantString("tlasGetInstanceIndex"), IntrinsicsShaderMask{ { ProgramInstance::__Resolved::EntryType::RayClosestHitShader, ProgramInstance::__Resolved::EntryType::RayAnyHitShader, ProgramInstance::__Resolved::EntryType::RayIntersectionShader  }, Compiler::State::SideEffects::Masks() }}
                , std::pair{ ConstantString("tlasGetInstanceCustomIndex"), IntrinsicsShaderMask{ { ProgramInstance::__Resolved::EntryType::RayClosestHitShader, ProgramInstance::__Resolved::EntryType::RayAnyHitShader, ProgramInstance::__Resolved::EntryType::RayIntersectionShader  }, Compiler::State::SideEffects::Masks() }}
                , std::pair{ ConstantString("rayGetWorldOrigin"), IntrinsicsShaderMask{ { ProgramInstance::__Resolved::EntryType::RayClosestHitShader, ProgramInstance::__Resolved::EntryType::RayAnyHitShader, ProgramInstance::__Resolved::EntryType::RayMissShader, ProgramInstance::__Resolved::EntryType::RayIntersectionShader  }, Compiler::State::SideEffects::Masks() }}
                , std::pair{ ConstantString("rayGetWorldDirection"), IntrinsicsShaderMask{ { ProgramInstance::__Resolved::EntryType::RayClosestHitShader, ProgramInstance::__Resolved::EntryType::RayAnyHitShader, ProgramInstance::__Resolved::EntryType::RayMissShader, ProgramInstance::__Resolved::EntryType::RayIntersectionShader  }, Compiler::State::SideEffects::Masks() }}
                , std::pair{ ConstantString("rayGetObjectOrigin"), IntrinsicsShaderMask{ { ProgramInstance::__Resolved::EntryType::RayClosestHitShader, ProgramInstance::__Resolved::EntryType::RayAnyHitShader, ProgramInstance::__Resolved::EntryType::RayIntersectionShader  }, Compiler::State::SideEffects::Masks() }}
                , std::pair{ ConstantString("rayGetObjectDirection"), IntrinsicsShaderMask{ { ProgramInstance::__Resolved::EntryType::RayClosestHitShader, ProgramInstance::__Resolved::EntryType::RayAnyHitShader, ProgramInstance::__Resolved::EntryType::RayIntersectionShader  }, Compiler::State::SideEffects::Masks() }}
                , std::pair{ ConstantString("rayGetMin"), IntrinsicsShaderMask{ { ProgramInstance::__Resolved::EntryType::RayClosestHitShader, ProgramInstance::__Resolved::EntryType::RayAnyHitShader, ProgramInstance::__Resolved::EntryType::RayMissShader, ProgramInstance::__Resolved::EntryType::RayIntersectionShader  }, Compiler::State::SideEffects::Masks() }}
                , std::pair{ ConstantString("rayGetMax"), IntrinsicsShaderMask{ { ProgramInstance::__Resolved::EntryType::RayClosestHitShader, ProgramInstance::__Resolved::EntryType::RayAnyHitShader, ProgramInstance::__Resolved::EntryType::RayMissShader, ProgramInstance::__Resolved::EntryType::RayIntersectionShader  }, Compiler::State::SideEffects::Masks() }}
                , std::pair{ ConstantString("rayGetFlags"), IntrinsicsShaderMask{ { ProgramInstance::__Resolved::EntryType::RayClosestHitShader, ProgramInstance::__Resolved::EntryType::RayAnyHitShader, ProgramInstance::__Resolved::EntryType::RayMissShader, ProgramInstance::__Resolved::EntryType::RayIntersectionShader  }, Compiler::State::SideEffects::Masks() }}
                , std::pair{ ConstantString("rayGetHitDistance"), IntrinsicsShaderMask{ { ProgramInstance::__Resolved::EntryType::RayClosestHitShader, ProgramInstance::__Resolved::EntryType::RayAnyHitShader }, Compiler::State::SideEffects::Masks() }}
                , std::pair{ ConstantString("rayGetHitKind"), IntrinsicsShaderMask{ { ProgramInstance::__Resolved::EntryType::RayClosestHitShader, ProgramInstance::__Resolved::EntryType::RayAnyHitShader }, Compiler::State::SideEffects::Masks() }}
                , std::pair{ ConstantString("tlasGetObjectToWorld"), IntrinsicsShaderMask{ { ProgramInstance::__Resolved::EntryType::RayClosestHitShader, ProgramInstance::__Resolved::EntryType::RayAnyHitShader, ProgramInstance::__Resolved::EntryType::RayIntersectionShader  }, Compiler::State::SideEffects::Masks() }}
                , std::pair{ ConstantString("tlasGetWorldToObject"), IntrinsicsShaderMask{ { ProgramInstance::__Resolved::EntryType::RayClosestHitShader, ProgramInstance::__Resolved::EntryType::RayAnyHitShader, ProgramInstance::__Resolved::EntryType::RayIntersectionShader  }, Compiler::State::SideEffects::Masks() }}
            };

            const auto it = allowedBuiltins.Find(callResolved->functionSymbol);
            if (it != allowedBuiltins.end())
            {
                bool allowedInShader = false;
                for (auto shaderType : it->second.entries)
                {
                    if (shaderType == compiler->currentState.shaderType)
                    {
                        allowedInShader = true;
                        break;
                    }
                }
                if (!allowedInShader)
                {
                    const ConstantString& shaderString = ProgramInstance::__Resolved::EntryTypeToString(compiler->currentState.shaderType);
                    compiler->Error(Format("%s can not be called from a %s", it->first.c_str(), shaderString.c_str()), callExpr);
                    return false;
                }
                compiler->currentState.sideEffects.bits |= (uint32_t)it->second.sideEffect;
            }

            static const auto derivativeConditionFunction = [](Compiler* compiler, Expression* expr, const ConstantString& fun)
            {
                
                static constexpr StaticArray derivativeProducingShaders =
                std::array {
                    ProgramInstance::__Resolved::EntryType::VertexShader
                    , ProgramInstance::__Resolved::EntryType::GeometryShader
                    , ProgramInstance::__Resolved::EntryType::HullShader
                    , ProgramInstance::__Resolved::EntryType::DomainShader
                    , ProgramInstance::__Resolved::EntryType::PixelShader
                };
                for (const auto shader : derivativeProducingShaders)
                    if (shader == compiler->currentState.shaderType)
                        return true;

                if (compiler->currentState.shaderType == ProgramInstance::__Resolved::ComputeShader || compiler->currentState.shaderType == ProgramInstance::__Resolved::TaskShader || compiler->currentState.shaderType == ProgramInstance::__Resolved::EntryType::MeshShader)
                {
                    Function::__Resolved* funResolved = Symbol::Resolved(compiler->currentState.function);
                    if (funResolved->executionModifiers.computeDerivativeIndexing != Function::__Resolved::NoDerivatives)
                        return true;
                    else
                    {
                        const ConstantString& shaderString = ProgramInstance::__Resolved::EntryTypeToString(compiler->currentState.shaderType);
                        compiler->Error(Format("%s must either specify 'derivative_index_linear' or 'derivative_index_quads' when using derivatives", shaderString.c_str()), expr);
                        return false;    
                    }
                }
                else
                {
                    const ConstantString& shaderString = ProgramInstance::__Resolved::EntryTypeToString(compiler->currentState.shaderType);
                    compiler->Error(Format("%s can not be called from a %s", fun.c_str(), shaderString.c_str()), expr);
                    return false;
                }
                
                
                return false;
            };
            
            static const StaticMap conditionalBuiltins =
            std::array{
                std::pair{ ConstantString("textureSample"), derivativeConditionFunction }
                , std::pair{ ConstantString("textureSampleBias"), derivativeConditionFunction }
                , std::pair{ ConstantString("textureSampleBiasCompare"), derivativeConditionFunction }
                , std::pair{ ConstantString("textureSampleBiasOffset"), derivativeConditionFunction }
                , std::pair{ ConstantString("textureSampleBiasProj"), derivativeConditionFunction }
                , std::pair{ ConstantString("textureSampleBiasProjCompare"), derivativeConditionFunction }
                , std::pair{ ConstantString("textureSampleBiasProjOffset"), derivativeConditionFunction }
                , std::pair{ ConstantString("textureSampleCompare"), derivativeConditionFunction }
                , std::pair{ ConstantString("textureSampleCompareOffset"), derivativeConditionFunction }
                , std::pair{ ConstantString("textureSampleProj"), derivativeConditionFunction }
                , std::pair{ ConstantString("textureSampleProjCompare"), derivativeConditionFunction }
                , std::pair{ ConstantString("textureSampleProjCompareOffset"), derivativeConditionFunction }
                , std::pair{ ConstantString("textureSampleProjOffset"), derivativeConditionFunction }
                , std::pair{ ConstantString("ddx"), derivativeConditionFunction }
                , std::pair{ ConstantString("ddy"), derivativeConditionFunction }
                , std::pair{ ConstantString("fwidth"), derivativeConditionFunction }
            };
            const auto it2 = conditionalBuiltins.Find(callResolved->functionSymbol);
            if (it2 != conditionalBuiltins.end())
            {
                bool allowedInShader = it2->second(compiler, callExpr, it2->first);
                if (!allowedInShader)
                {
                    return false;
                }
            }
            
            for (auto& arg : callExpr->args)
                res |= this->ResolveVisibility(compiler, arg);
            res |= this->ResolveVisibility(compiler, callResolved->function);
            break;
        }
        case Symbol::CommaExpressionType:
        {
            auto commaExpr = static_cast<CommaExpression*>(symbol);
            res |= this->ResolveVisibility(compiler, commaExpr->left);
            res |= this->ResolveVisibility(compiler, commaExpr->right);
            break;
        }
        case Symbol::InitializerExpressionType:
        {
            auto init = static_cast<InitializerExpression*>(symbol);
            for (auto& initializer : init->values)
                res |= this->ResolveVisibility(compiler, initializer);
            break;
        }
        case Symbol::ArrayInitializerExpressionType:
        {
            auto init = static_cast<ArrayInitializerExpression*>(symbol);
            for (auto& initializer : init->values)
                res |= this->ResolveVisibility(compiler, initializer);
            break;
        }
        case Symbol::UnaryExpressionType:
        {
            auto unary = static_cast<UnaryExpression*>(symbol);
            res |= this->ResolveVisibility(compiler, unary->expr);
            break;
        }
        case Symbol::FunctionType:
        {
            auto fun = static_cast<Function*>(symbol);
            auto funResolved = Symbol::Resolved(fun);

            // Add this function to the visibility map
            Function::__Resolved* entryRes = Symbol::Resolved(compiler->currentState.function);
            entryRes->visibleSymbols.Insert(fun);

            for (auto& param : fun->parameters)
                res |= this->ResolveVisibility(compiler, param);

            if (fun->ast != nullptr)
                res |= this->ResolveVisibility(compiler, fun->ast);
            break;
        }
        case Symbol::SymbolExpressionType:
        {
            auto symExpr = static_cast<SymbolExpression*>(symbol);
            FixedString symbol;
            symExpr->EvalSymbol(symbol);
            Symbol* newSymbol = compiler->GetSymbol(symbol);
            if (newSymbol != nullptr)
                res |= this->ResolveVisibility(compiler, newSymbol);
            break;
        }
        case Symbol::VariableType:
        {
            auto var = static_cast<Variable*>(symbol);
            auto varResolved = Symbol::Resolved(var);
            
            Function::__Resolved* entryRes = Symbol::Resolved(compiler->currentState.function);
            entryRes->visibleSymbols.Insert(var);
            
            switch (compiler->currentState.shaderType)
            {
                case ProgramInstance::__Resolved::EntryType::VertexShader:
                    varResolved->visibilityBits.flags.vertexShader = true;
                    break;
                case ProgramInstance::__Resolved::EntryType::HullShader:
                    varResolved->visibilityBits.flags.hullShader = true;
                    break;
                case ProgramInstance::__Resolved::EntryType::DomainShader:
                    varResolved->visibilityBits.flags.domainShader = true;
                    break;
                case ProgramInstance::__Resolved::EntryType::GeometryShader:
                    varResolved->visibilityBits.flags.geometryShader = true;
                    break;
                case ProgramInstance::__Resolved::EntryType::PixelShader:
                    varResolved->visibilityBits.flags.pixelShader = true;
                    break;
                case ProgramInstance::__Resolved::EntryType::ComputeShader:
                    varResolved->visibilityBits.flags.computeShader = true;
                    break;
                case ProgramInstance::__Resolved::EntryType::TaskShader:
                    varResolved->visibilityBits.flags.taskShader = true;
                    break;
                case ProgramInstance::__Resolved::EntryType::MeshShader:
                    varResolved->visibilityBits.flags.meshShader = true;
                    break;
                case ProgramInstance::__Resolved::EntryType::RayGenerationShader:
                    varResolved->visibilityBits.flags.rayGenerationShader = true;
                    break;
                case ProgramInstance::__Resolved::EntryType::RayMissShader:
                    varResolved->visibilityBits.flags.rayMissShader = true;
                    break;
                case ProgramInstance::__Resolved::EntryType::RayClosestHitShader:
                    varResolved->visibilityBits.flags.rayClosestHitShader = true;
                    break;
                case ProgramInstance::__Resolved::EntryType::RayAnyHitShader:
                    varResolved->visibilityBits.flags.rayAnyHitShader = true;
                    break;
                case ProgramInstance::__Resolved::EntryType::RayIntersectionShader:
                    varResolved->visibilityBits.flags.rayIntersectionShader = true;
                    break;
                case ProgramInstance::__Resolved::EntryType::RayCallableShader:
                    varResolved->visibilityBits.flags.rayCallableShader = true;
                    break;
                default:
                    assert(false);
            }
                
            if (var->valueExpression != nullptr)
                res |= this->ResolveVisibility(compiler, var->valueExpression);

            // If variable points to a structure, add it to the visibility
            if (varResolved->typeSymbol->symbolType == Symbol::StructureType)
            {
                res |= this->ResolveVisibility(compiler, varResolved->typeSymbol);
            }
            break;
        }
        case Symbol::SamplerStateInstanceType:
        {
            auto sampler = static_cast<SamplerStateInstance*>(symbol);
            auto sampResolved = Symbol::Resolved(sampler);
            
            Function::__Resolved* entryRes = Symbol::Resolved(compiler->currentState.function);
            entryRes->visibleSymbols.Insert(sampler);
            
            switch (compiler->currentState.shaderType)
            {
                case ProgramInstance::__Resolved::EntryType::VertexShader:
                    sampResolved->visibilityBits.flags.vertexShader = true;
                    break;
                case ProgramInstance::__Resolved::EntryType::HullShader:
                    sampResolved->visibilityBits.flags.hullShader = true;
                    break;
                case ProgramInstance::__Resolved::EntryType::DomainShader:
                    sampResolved->visibilityBits.flags.domainShader = true;
                    break;
                case ProgramInstance::__Resolved::EntryType::GeometryShader:
                    sampResolved->visibilityBits.flags.geometryShader = true;
                    break;
                case ProgramInstance::__Resolved::EntryType::PixelShader:
                    sampResolved->visibilityBits.flags.pixelShader = true;
                    break;
                case ProgramInstance::__Resolved::EntryType::ComputeShader:
                    sampResolved->visibilityBits.flags.computeShader = true;
                    break;
                case ProgramInstance::__Resolved::EntryType::TaskShader:
                    sampResolved->visibilityBits.flags.taskShader = true;
                    break;
                case ProgramInstance::__Resolved::EntryType::MeshShader:
                    sampResolved->visibilityBits.flags.meshShader = true;
                    break;
                case ProgramInstance::__Resolved::EntryType::RayGenerationShader:
                    sampResolved->visibilityBits.flags.rayGenerationShader = true;
                    break;
                case ProgramInstance::__Resolved::EntryType::RayMissShader:
                    sampResolved->visibilityBits.flags.rayMissShader = true;
                    break;
                case ProgramInstance::__Resolved::EntryType::RayClosestHitShader:
                    sampResolved->visibilityBits.flags.rayClosestHitShader = true;
                    break;
                case ProgramInstance::__Resolved::EntryType::RayAnyHitShader:
                    sampResolved->visibilityBits.flags.rayAnyHitShader = true;
                    break;
                case ProgramInstance::__Resolved::EntryType::RayIntersectionShader:
                    sampResolved->visibilityBits.flags.rayIntersectionShader = true;
                    break;
                case ProgramInstance::__Resolved::EntryType::RayCallableShader:
                    sampResolved->visibilityBits.flags.rayCallableShader = true;
                    break;
                default:
                    assert(false);
            }
            break;
        }
        case Symbol::StructureType:
        {
            auto struc = static_cast<Structure*>(symbol);
            auto strucResolved = Symbol::Resolved(struc);
            
            Function::__Resolved* entryRes = Symbol::Resolved(compiler->currentState.function);
            entryRes->visibleSymbols.Insert(struc);

            for (auto mem : struc->symbols)
                res |= this->ResolveVisibility(compiler, mem);
            break;
        }
    }
    return res;
}

} // namespace GPULang
