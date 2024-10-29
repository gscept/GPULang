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

#include "ast/expressions/binaryexpression.h"
#include "ast/expressions/commaexpression.h"
#include "ast/expressions/arrayindexexpression.h"
#include "ast/expressions/symbolexpression.h"
#include "ast/expressions/callexpression.h"
#include "ast/expressions/ternaryexpression.h"
#include "ast/expressions/initializerexpression.h"
#include "ast/expressions/accessexpression.h"
#include "ast/expressions/unaryexpression.h"
#include "ast/expressions/uintexpression.h"
#include "ast/expressions/enumexpression.h"

#include "ast/statements/breakstatement.h"
#include "ast/statements/continuestatement.h"
#include "ast/statements/expressionstatement.h"
#include "ast/statements/forstatement.h"
#include "ast/statements/ifstatement.h"
#include "ast/statements/returnstatement.h"
#include "ast/statements/scopestatement.h"
#include "ast/statements/switchstatement.h"
#include "ast/statements/whilestatement.h"

#include "compiler.h"
#include "util.h"
#include <algorithm>

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
static std::set<std::string> scalarQualifiers =
{
    "const", "var"
};

static std::set<std::string> bindingQualifiers =
{
    "group", "binding"
};

static std::set<std::string> functionAttributes =
{
    "entry_point", "local_size_x", "local_size_y", "local_size_z", "early_depth"
    , "group_size", "groups_per_workgroup"
    , "input_vertices", "max_output_vertices", "winding"
    , "input_topology", "output_topology", "patch_type", "partition"
    , "pixel_origin"
    , "prototype"
};

static std::set<std::string> parameterAccessFlags =
{
    "in", "out"
};

static std::set<std::string> parameterQualifiers =
{
    "patch", "no_interpolate", "no_perspective", "binding"
};

static std::set<std::string> structureQualifiers =
{
    "packed"
};

static std::set<std::string> pixelShaderInputQualifiers =
{
    "binding", "no_interpolate", "no_perspective"
};

static std::set<std::string> attributesRequiringEvaluation =
{
    "binding", "group", "local_size_x", "local_size_y", "local_size_z"
    , "group_size", "groups_per_workgroup"
    , "input_vertices", "max_output_vertices", "winding"
    , "input_topology", "output_topology", "patch_type", "patch_size", "partition"
};

static std::set<std::string> pointerQualifiers =
{
    "no_read", "atomic", "volatile"
};

static std::set<std::string> storageQualifiers =
{
    "uniform", "inline", "workgroup", "link_defined"
};

static std::set<std::string> textureQualifiers =
{
    "sampled"
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
        this->allowedTextureAttributes.insert(it.first);
    }

    this->allowedTextureAttributes.insert(bindingQualifiers.begin(), bindingQualifiers.end());
    this->allowedTextureAttributes.insert(pointerQualifiers.begin(), pointerQualifiers.end());
    this->allowedTextureAttributes.insert(storageQualifiers.begin(), storageQualifiers.end());
    this->allowedTextureAttributes.insert(textureQualifiers.begin(), textureQualifiers.end());

    this->allowedScalarAttributes.insert(scalarQualifiers.begin(), scalarQualifiers.end());
    this->allowedScalarAttributes.insert(storageQualifiers.begin(), storageQualifiers.end());

    this->allowedSamplerAttributes.insert(bindingQualifiers.begin(), bindingQualifiers.end());
    this->allowedSamplerAttributes.insert(bindingQualifiers.begin(), bindingQualifiers.end());
    this->allowedSamplerAttributes.insert(storageQualifiers.begin(), storageQualifiers.end());

    this->allowedPointerAttributes.insert(pointerQualifiers.begin(), pointerQualifiers.end());
    this->allowedPointerAttributes.insert(storageQualifiers.begin(), storageQualifiers.end());

    this->allowedFunctionAttributes.insert(functionAttributes.begin(), functionAttributes.end());

    this->allowedParameterAttributes.insert(parameterQualifiers.begin(), parameterQualifiers.end());
    this->allowedParameterAttributes.insert(parameterAccessFlags.begin(), parameterAccessFlags.end());
    this->allowedParameterAttributes.insert(pointerQualifiers.begin(), pointerQualifiers.end());

    this->allowedStructureAttributes.insert(structureQualifiers.begin(), structureQualifiers.end());
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
    case Symbol::SymbolType::ProgramType:
        return this->ResolveProgram(compiler, symbol);
        break;
    case Symbol::SymbolType::RenderStateType:
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
    case Symbol::SymbolType::SamplerStateType:
        return this->ResolveSamplerState(compiler, symbol);
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
    Symbol* sym = compiler->GetSymbol(alias->type);
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
    Type* type = static_cast<Type*>(symbol);
    type->symbols.clear();
    type->lookup.clear();

    if (type->symbolType == Symbol::SymbolType::EnumerationType)
    {
        if (!this->ResolveEnumeration(compiler, type))
            return false;
    }
    else if (type->symbolType == Symbol::SymbolType::StructureType)
    {
        if (!this->ResolveStructure(compiler, type))
            return false;
    }
    else if (type->symbolType == Symbol::SymbolType::TypeType)
    {
        if (!compiler->AddSymbol(symbol->name, symbol))
            return false;
    }
    else if (type->symbolType != Symbol::SymbolType::FunctionType)
    {
        if (!compiler->AddSymbol(symbol->name, symbol))
            return false;
    }

    Compiler::LocalScope scope = Compiler::LocalScope::MakeTypeScope(compiler, type);

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
            en->lookup.clear();
            en->symbols.clear();
            if (!this->ResolveEnumeration(compiler, sym))
                return false;
        }
        else if (sym->symbolType == Symbol::SymbolType::StructureType)
        {
            Structure* struc = static_cast<Structure*>(sym);
            struc->lookup.clear();
            struc->symbols.clear();
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
            if (!this->ResolveFunction(compiler, fun))
                return false;
        }
    }

    Compiler::LocalScope scope = Compiler::LocalScope::MakeTypeScope(compiler, type);

    for (Symbol* sym : type->symbols)
    {
        if (sym->symbolType == Symbol::SymbolType::FunctionType)
        {
            Function* fun = static_cast<Function*>(sym);
            if (!this->ResolveFunction(compiler, fun))
                return false;
        }
    }    

    for (Symbol* sym : type->staticSymbols)
    {
        if (sym->symbolType == Symbol::SymbolType::FunctionType)
        {
            Function* fun = static_cast<Function*>(sym);
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
    Compiler::LocalScope scope = Compiler::LocalScope::MakeTypeScope(compiler, type);

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
    SamplerState* state = static_cast<SamplerState*>(symbol);
    SamplerState::__Resolved* stateResolved = Symbol::Resolved(state);

    if (!compiler->AddSymbol(symbol->name, symbol))
        return false;

    Type* samplerStateType = compiler->GetSymbol<Type>("samplerState");
    Compiler::LocalScope scope = Compiler::LocalScope::MakeTypeScope(compiler, samplerStateType);

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

        std::string entryString = assignEntry->left->EvalString();
        SamplerState::__Resolved::SamplerStateEntryType entryType = SamplerState::__Resolved::StringToEntryType(entryString.c_str());
        if (entryType == SamplerState::__Resolved::InvalidSamplerStateEntryType)
        {
            compiler->Error(Format("Invalid sampler state entry '%s'", entryString.c_str()), assignEntry);
            return false;
        }

        uint32_t value;
        switch (entryType)
        {
        case SamplerState::__Resolved::AllAddressType:
            assignEntry->right->EvalUInt(value);
            stateResolved->addressU = stateResolved->addressV = stateResolved->addressW = (GPULang::AddressMode)value;
            break;
        case SamplerState::__Resolved::AddressUType:
            assignEntry->right->EvalUInt(value);
            stateResolved->addressU = (GPULang::AddressMode)value;
            break;
        case SamplerState::__Resolved::AddressVType:
            assignEntry->right->EvalUInt(value);
            stateResolved->addressV = (GPULang::AddressMode)value;
            break;
        case SamplerState::__Resolved::AddressWType:
            assignEntry->right->EvalUInt(value);
            stateResolved->addressW = (GPULang::AddressMode)value;
            break;
        case SamplerState::__Resolved::AllFilterType:
            assignEntry->right->EvalUInt(value);
            stateResolved->minFilter = stateResolved->magFilter = stateResolved->mipFilter = (GPULang::Filter)value;
            break;
        case SamplerState::__Resolved::MinFilterType:
            assignEntry->right->EvalUInt(value);
            stateResolved->minFilter = (GPULang::Filter)value;
            break;
        case SamplerState::__Resolved::MagFilterType:
            assignEntry->right->EvalUInt(value);
            stateResolved->magFilter = (GPULang::Filter)value;
            break;
        case SamplerState::__Resolved::MipFilterType:
            assignEntry->right->EvalUInt(value);
            stateResolved->mipFilter = (GPULang::Filter)value;
            break;
        case SamplerState::__Resolved::MipLodBiasType:
            assignEntry->right->EvalFloat(stateResolved->mipLodBias);
            break;
        case SamplerState::__Resolved::AnisotropicFlagType:
            assignEntry->right->EvalBool(stateResolved->anisotropicEnabled);
            break;
        case SamplerState::__Resolved::MaxAnisotropyType:
            assignEntry->right->EvalFloat(stateResolved->maxAnisotropy);
            break;
        case SamplerState::__Resolved::CompareFlagType:
            assignEntry->right->EvalBool(stateResolved->compareSamplerEnabled);
            break;
        case SamplerState::__Resolved::CompareModeType:
            assignEntry->right->EvalUInt(value);
            stateResolved->compareMode = (GPULang::CompareMode)value;
            break;
        case SamplerState::__Resolved::MinLodType:
            assignEntry->right->EvalFloat(stateResolved->minLod);
            break;
        case SamplerState::__Resolved::MaxLodType:
            assignEntry->right->EvalFloat(stateResolved->maxLod);
            break;
        case SamplerState::__Resolved::BorderColorType:
            assignEntry->right->EvalUInt(value);
            stateResolved->borderColor = (GPULang::BorderColor)value;
            break;
        case SamplerState::__Resolved::UnnormalizedSamplingType:
            assignEntry->right->EvalBool(stateResolved->unnormalizedSamplingEnabled);
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

    if (fun->name.substr(0, 3) == "gpl" && !compiler->ignoreReservedWords)
    {
        compiler->ReservedPrefixError(fun->name, "gpl", symbol);
    }

     // run attribute validation
    for (const Attribute& attr : fun->attributes)
    {
        if (attr.expression != nullptr)
        {
            if (!attr.expression->Resolve(compiler))
                return false;
        }
        if (!set_contains(this->allowedFunctionAttributes, attr.name))
        {
            compiler->Error(Format("Invalid attribute for function: '%s'", attr.name.c_str()), symbol);
            return false;
        }

        // make sure attribute has expression
        if (attr.expression == nullptr && set_contains(attributesRequiringEvaluation, attr.name))
        {
            compiler->Error(Format("Attribute '%s' requires value but none provided", attr.name.c_str()), symbol);
            return false;
        }

        if (attr.name == "entry_point")
        {
            funResolved->isEntryPoint = true;
        }
        else if (attr.name == "local_size_x")
            if (!attr.expression->EvalUInt(funResolved->executionModifiers.computeShaderWorkGroupSize[0]))
            {
                compiler->Error(Format("Value '%s' has to be a compile time uint constant", attr.name.c_str()), symbol);
                return false;
            }
        else if (attr.name == "local_size_y")
            if (!attr.expression->EvalUInt(funResolved->executionModifiers.computeShaderWorkGroupSize[1]))
            {
                compiler->Error(Format("Value '%s' has to be a compile time uint constant", attr.name.c_str()), symbol);
                return false;
            }
        else if (attr.name == "local_size_z")
            if (!attr.expression->EvalUInt(funResolved->executionModifiers.computeShaderWorkGroupSize[2]))
            {
                compiler->Error(Format("Value '%s' has to be a compile time uint constant", attr.name.c_str()), symbol);
                return false;
            }
        else if (attr.name == "group_size")
            if (!attr.expression->EvalUInt(funResolved->executionModifiers.groupSize))
            {
                compiler->Error(Format("Value '%s' has to be a compile time uint constant", attr.name.c_str()), symbol);
                return false;
            }
        else if (attr.name == "groups_per_workgroup")
            if (!attr.expression->EvalUInt(funResolved->executionModifiers.groupsPerWorkgroup))
            {
                compiler->Error(Format("Value '%s' has to be a compile time uint constant", attr.name.c_str()), symbol);
                return false;
            }
        else if (attr.name == "early_depth")
            funResolved->executionModifiers.earlyDepth = true;
        else if (attr.name == "invocations")
            if (!attr.expression->EvalUInt(funResolved->executionModifiers.invocations))
            {
                compiler->Error(Format("Value '%s' has to be a compile time uint constant", attr.name.c_str()), symbol);
                return false;
            }
        else if (attr.name == "max_output_vertices")
            if (!attr.expression->EvalUInt(funResolved->executionModifiers.maxOutputVertices))
            {
                compiler->Error(Format("Value '%s' has to be a compile time uint constant", attr.name.c_str()), symbol);
                return false;
            }
        else if (attr.name == "winding")
        {
            std::string str = attr.expression->EvalString();
            funResolved->executionModifiers.windingOrder = Function::__Resolved::WindingOrderFromString(str);
            if (funResolved->executionModifiers.windingOrder == Function::__Resolved::InvalidWindingOrder)
            {
                compiler->Error(Format("Attribute 'winding' supports values: cw/clockwise, ccw/counter_clockwise, but got '%s'", str.c_str()), symbol);
                return false;
            }
        }
        else if (attr.name == "input_topology")
        {
            std::string str = attr.expression->EvalString();
            funResolved->executionModifiers.inputPrimitiveTopology = Function::__Resolved::PrimitiveTopologyFromString(str);
            if (funResolved->executionModifiers.inputPrimitiveTopology == Function::__Resolved::InvalidPrimitiveTopology)
            {
                compiler->Error(Format("Attribute 'input_topology' supports values: points, lines, lines_adjacency, triangles, triangles_adjacency, but got '%s'", str.c_str()), symbol);
                return false;
            }
        }
        else if (attr.name == "output_topology")
        {
            std::string str = attr.expression->EvalString();
            funResolved->executionModifiers.outputPrimitiveTopology = Function::__Resolved::PrimitiveTopologyFromString(str);
            if (funResolved->executionModifiers.outputPrimitiveTopology == Function::__Resolved::InvalidPrimitiveTopology
                || funResolved->executionModifiers.outputPrimitiveTopology == Function::__Resolved::LinesAdjacency
                || funResolved->executionModifiers.outputPrimitiveTopology == Function::__Resolved::TrianglesAdjacency)
            {
                compiler->Error(Format("Attribute 'output_topology' supports values: points, lines, triangles, but got '%s'", str.c_str()), symbol);
                return false;
            }
        }
        else if (attr.name == "patch_type")
        {
            std::string str = attr.expression->EvalString();
            funResolved->executionModifiers.patchType = Function::__Resolved::PatchTypeFromString(str);
            if (funResolved->executionModifiers.patchType == Function::__Resolved::InvalidPatchType)
            {
                compiler->Error(Format("Attribute 'patch_type' supports values: isolines, triangles, quads, but got '%s'", str.c_str()), symbol);
                return false;
            }
        }
        else if (attr.name == "partition")
        {
            std::string str = attr.expression->EvalString();
            funResolved->executionModifiers.partitionMethod = Function::__Resolved::PartitionMethodFromString(str);
            if (funResolved->executionModifiers.partitionMethod == Function::__Resolved::InvalidPartitionMethod)
            {
                compiler->Error(Format("Attribute 'partition' supports values: steps/integer, even/fract_even, odd/fract_odd, but got '%s'", str.c_str()), symbol);
                return false;
            }
        }
        else if (attr.name == "prototype")
        {
            if (fun->hasBody)
            {
                compiler->Error(Format("Prototype function '%s' may not have a function body", fun->name.c_str()), symbol);
                return false;
            }
            funResolved->isPrototype = true;
        }
        else if (attr.name == "pixel_origin")
        {
            std::string str = attr.expression->EvalString();
            funResolved->executionModifiers.pixelOrigin = Function::__Resolved::PixelOriginFromString(str);
            if (funResolved->executionModifiers.pixelOrigin == Function::__Resolved::InvalidPixelOrigin)
            {
                compiler->Error(Format("Attribute 'pixel_origin' supports values: lower/lower_left, upper/upper_left, center, but got '%s'", str.c_str()), symbol);
                return false;
            }
        }
    }

    // validate attributes
    if (!funResolved->isEntryPoint)
    {
        if (funResolved->executionModifiers.earlyDepth)
        {
            compiler->Error("'early_depth' is only allowed on functions with the 'shader' qualifier", symbol);
            return false;
        }

        if (
            funResolved->executionModifiers.computeShaderWorkGroupSize[0] > 1
            || funResolved->executionModifiers.computeShaderWorkGroupSize[1] > 1
            || funResolved->executionModifiers.computeShaderWorkGroupSize[2] > 1
            )
        {
            compiler->Error("'local_size_(x/y/z)' is only allowed on functions with the 'shader' qualifier", symbol);
            return false;
        }
    }
    else
    {
        if (fun->returnType != Type::FullType{ "void" })
        {
            compiler->Error("Entry point may only return 'void'", symbol);
            return false;
        }
    }

    // Push temporary scope to evaluate variables
    compiler->PushScope(Compiler::Scope::ScopeType::Local, fun);

    // run validation on parameters
    for (Variable* var : fun->parameters)
    {
        Variable::__Resolved* varResolved = Symbol::Resolved(var);
        varResolved->usageBits.flags.isParameter = true;
        varResolved->usageBits.flags.isEntryPointParameter = funResolved->isEntryPoint;
        this->ResolveVariable(compiler, var);
    }

    compiler->PopScope();

    // setup our variables and attributes as sets
    std::string paramList;
    for (Variable* var : fun->parameters)
    {
        // add comma if not first argument
        Variable::__Resolved* varResolved = Symbol::Resolved(var);
        paramList.append(varResolved->type.ToString());

        if (varResolved->type.sampled)
        {
            int i = 5;
        }

        if (var != fun->parameters.back())
            paramList.append(",");
    }

    std::string attributeList;

    // make a set of all attributes
    for (const Attribute& attr : fun->attributes)
    {
        std::string attrAsString;
        if (!attr.ToString(attrAsString))
        {
            compiler->Error(Format("Attribute '%s' can not be evaluated to a compile time value", attr.name.c_str()), symbol);
            return false;
        }
        attributeList.append(Format("%s ", attrAsString.c_str()));
        if (attr.name == "prototype")
            funResolved->isPrototype = true;
    }

    // format function with all attributes and parameters
    std::string resolvedName = Format("%s(%s)", fun->name.c_str(), paramList.c_str());
    std::string functionFormatted = Format("%s%s %s", attributeList.c_str(), fun->returnType.name.c_str(), resolvedName.c_str());
    funResolved->name = resolvedName;
    funResolved->signature = functionFormatted;

    // if prototype, add as an ordinary symbol
    if (funResolved->isPrototype)
    {
        if (!compiler->AddSymbol(fun->name, fun, false))
            return false;
    }
    else
    {
        // find functions with similar name
        std::vector<Symbol*> matchingFunctions = compiler->GetSymbols(fun->name);
        for (Symbol* matchingFunction : matchingFunctions)
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

    // if we didn't fault, add the symbol
    if (!compiler->AddSymbol(funResolved->name, fun, false))
        return false;

    // also add the signature for type lookup
    if (!compiler->AddSymbol(funResolved->signature, fun, false))
        return false;

    // Add just the name if the function isn't a constructor
    Symbol* constructorType = compiler->GetSymbol<Symbol>(fun->name);
    if (constructorType == nullptr || constructorType->symbolType == Type::SymbolType::FunctionType)
    {
        // also add the signature for type lookup
        if (!compiler->AddSymbol(fun->name, fun, true))
            return false;
    }

    Type* type = (Type*)compiler->GetSymbol(fun->returnType.name);
    if (type == nullptr)
    {
        compiler->UnrecognizedTypeError(fun->returnType.name, fun);
        return false;
    }
    funResolved->returnTypeSymbol = type;

    Compiler::LocalScope scope = Compiler::LocalScope::MakeLocalScope(compiler, fun);

    // Variables already resolved, just need to add them back to the scope
    for (Variable* var : fun->parameters)
    {
        if (!compiler->AddSymbol(var->name, var))
            return false;
    }

    // before resolving variables (as parameters), reset in and out bindings
    this->inParameterIndexCounter = 0;
    this->outParameterIndexCounter = 0;

    compiler->currentFunction = fun;

    // validate function body
    if (
        fun->ast != nullptr
        && !this->ResolveStatement(compiler, fun->ast)
        )
    {
        return false;
    }

    compiler->currentFunction = nullptr;

    funResolved->hasExplicitReturn = compiler->branchReturns;

    if (fun->returnType != Type::FullType{ "void" } && fun->ast != nullptr && !funResolved->hasExplicitReturn)
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
    Program* prog = static_cast<Program*>(symbol);
    Program::__Resolved* progResolved = Symbol::Resolved(prog);

    Type* progType = compiler->GetSymbol<Type>("program");
    Compiler::LocalScope scope = Compiler::LocalScope::MakeTypeScope(compiler, progType);
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

        std::string entry = assignEntry->left->EvalString();
        Program::__Resolved::ProgramEntryType entryType = Program::__Resolved::StringToEntryType(entry);
        if (entryType == Program::__Resolved::InvalidProgramEntryType)
        {
            Symbol* overrideSymbol = compiler->GetSymbol(entry);
            if (overrideSymbol->symbolType == Symbol::FunctionType)
            {
                // get all functions responding to this function
                Function* functionStub = static_cast<Function*>(overrideSymbol);

                // check that we actually got a symbol
                if (functionStub == nullptr)
                {
                    compiler->UnrecognizedTypeError(entry, symbol);
                    return false;
                }

                // check that it's actually a function
                if (functionStub->symbolType != Symbol::FunctionType)
                {
                    compiler->Error(Format("Symbol '%s' is not a recognized function", entry.c_str()), symbol);
                    return false;
                }

                // next up, function to assign
                std::string functionName;
                if (!assignEntry->right->EvalSymbol(functionName))
                {
                    compiler->Error(Format("Expected symbol, but got '%s'", entry.c_str()), symbol);
                    return false;
                }
                std::vector<Symbol*> functions = compiler->GetSymbols(functionName);

                // again, check if not null
                if (functions.empty())
                {
                    compiler->UnrecognizedTypeError(functionName, symbol);
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
                        progResolved->functionOverrides.insert({ functionStub, func });
                        matched = true;
                    }
                }

                if (!matched)
                {
                    std::string candidates;
                    for (Symbol* sym : functions)
                    {
                        Function* func = static_cast<Function*>(sym);
                        Function::__Resolved* res = Symbol::Resolved(func);

                        candidates.append(res->signature);
                        if (sym != functions.back())
                            candidates.append(",\n");
                    }
                    compiler->Error(Format("Function prototype '%s' can not bind function '%s', possible candidates: \n%s", functionStub->name.c_str(), assignEntry->right->EvalString().c_str(), candidates.c_str()), symbol);
                    return false;
                }
            }
            else if (overrideSymbol->symbolType == Symbol::VariableType)
            {
                Variable* var = static_cast<Variable*>(overrideSymbol);
                Variable::__Resolved* varResolved = Symbol::Resolved(var);
                if (!varResolved->usageBits.flags.isConst)
                {
                    compiler->Error("Only variables declared as 'const' can be overriden in program assembly", var);
                    return false;
                }
                
                if (!binExp->rightType.literal)
                {
                    compiler->Error("Constant overrides must be literal values", var);
                    return false;
                }
                if (varResolved->type != binExp->rightType)
                {
                    compiler->Error(Format("Trying to assign a value of type '%s' to a constant of '%s'", binExp->rightType.ToString().c_str(), varResolved->type.ToString().c_str()), var);
                    return false;
                }
                progResolved->constVarInitializationOverrides.insert({ var, assignEntry->right });
            }
        }
        else
        {
            // get the symbol for this entry
            std::string sym;
            if (!assignEntry->right->EvalSymbol(sym))
            {
                compiler->Error(Format("Entry '%s' has to be a symbol", entry.c_str()), symbol);
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
            case Program::__Resolved::VertexShader:
                progResolved->usage.flags.hasVertexShader = true;
                break;
            case Program::__Resolved::HullShader:
                progResolved->usage.flags.hasHullShader = true;
                break;
            case Program::__Resolved::DomainShader:
                progResolved->usage.flags.hasDomainShader = true;
                break;
            case Program::__Resolved::GeometryShader:
                progResolved->usage.flags.hasGeometryShader = true;
                break;
            case Program::__Resolved::PixelShader:
                progResolved->usage.flags.hasPixelShader = true;
                break;
            case Program::__Resolved::ComputeShader:
                progResolved->usage.flags.hasComputeShader = true;
                break;
            case Program::__Resolved::TaskShader:
                progResolved->usage.flags.hasTaskShader = true;
                break;
            case Program::__Resolved::MeshShader:
                progResolved->usage.flags.hasMeshShader = true;
                break;
            case Program::__Resolved::RayGenerationShader:
                progResolved->usage.flags.hasRayGenerationShader = true;
                break;
            case Program::__Resolved::RayMissShader:
                progResolved->usage.flags.hasRayMissShader = true;
                break;
            case Program::__Resolved::RayClosestHitShader:
                progResolved->usage.flags.hasRayClosestHitShader = true;
                break;
            case Program::__Resolved::RayAnyHitShader:
                progResolved->usage.flags.hasRayAnyHitShader = true;
                break;
            case Program::__Resolved::RayIntersectionShader:
                progResolved->usage.flags.hasRayIntersectionShader = true;
                break;
            case Program::__Resolved::RayCallableShader:
                progResolved->usage.flags.hasRayCallableShader = true;
                break;
            case Program::__Resolved::RenderState:
                progResolved->usage.flags.hasRenderState = true;
                break;
            }

            // if shader, value must be a function
            if (entryType >= Program::__Resolved::ProgramEntryType::VertexShader
                && entryType <= Program::__Resolved::ProgramEntryType::RayIntersectionShader)
            {
                Function* fun = static_cast<Function*>(value);
                Function::__Resolved* funResolved = Symbol::Resolved(fun);

                switch (entryType)
                {
                case Program::__Resolved::ProgramEntryType::VertexShader:
                    funResolved->shaderUsage.flags.vertexShader = true;
                    break;
                case Program::__Resolved::ProgramEntryType::HullShader:
                    funResolved->shaderUsage.flags.hullShader = true;
                    break;
                case Program::__Resolved::ProgramEntryType::DomainShader:
                    funResolved->shaderUsage.flags.domainShader = true;
                    break;
                case Program::__Resolved::ProgramEntryType::GeometryShader:
                    funResolved->shaderUsage.flags.geometryShader = true;
                    break;
                case Program::__Resolved::ProgramEntryType::PixelShader:
                    funResolved->shaderUsage.flags.pixelShader = true;
                    break;
                case Program::__Resolved::ProgramEntryType::ComputeShader:
                    funResolved->shaderUsage.flags.computeShader = true;
                    break;
                case Program::__Resolved::ProgramEntryType::TaskShader:
                    funResolved->shaderUsage.flags.taskShader = true;
                    break;
                case Program::__Resolved::ProgramEntryType::MeshShader:
                    funResolved->shaderUsage.flags.meshShader = true;
                    break;
                case Program::__Resolved::ProgramEntryType::RayGenerationShader:
                    funResolved->shaderUsage.flags.rayGenerationShader = true;
                    break;
                case Program::__Resolved::ProgramEntryType::RayMissShader:
                    funResolved->shaderUsage.flags.rayMissShader = true;
                    break;
                case Program::__Resolved::ProgramEntryType::RayClosestHitShader:
                    funResolved->shaderUsage.flags.rayClosestHitShader = true;
                    break;
                case Program::__Resolved::ProgramEntryType::RayAnyHitShader:
                    funResolved->shaderUsage.flags.rayAnyHitShader = true;
                    break;
                case Program::__Resolved::ProgramEntryType::RayIntersectionShader:
                    funResolved->shaderUsage.flags.rayIntersectionShader = true;
                    break;
                case Program::__Resolved::ProgramEntryType::RayCallableShader:
                    funResolved->shaderUsage.flags.rayCallableShader = true;
                    break;
                }

                // when we've set these flags, run function validation to make sure it's properly formatted
                if (!this->ValidateFunction(compiler, fun))
                    return false;

                // Resolve variable visibility
                compiler->currentFunction = fun;
                if (!this->ResolveVisibility(compiler, fun->ast))
                {
                    compiler->currentFunction = nullptr;
                    return false;
                }
                compiler->currentFunction = nullptr;
            }
            else
            {
                if (value->symbolType != Symbol::SymbolType::RenderStateType)
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
        progResolved->mappings[Program::__Resolved::ProgramEntryType::RenderState] = &compiler->defaultRenderState;
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
    RenderState* state = static_cast<RenderState*>(symbol);
    RenderState::__Resolved* stateResolved = Symbol::Resolved(state);

    Type* renderStateType = compiler->GetSymbol<Type>("renderState");

    Compiler::LocalScope scope = Compiler::LocalScope::MakeTypeScope(compiler, renderStateType);
    if (!compiler->AddSymbol(symbol->name, symbol))
        return false;

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

        std::string entry;

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
            entry = lhs->left->EvalString();
        }
        else if (assignEntry->left->symbolType == Symbol::AccessExpressionType)
        {
            entry = assignEntry->left->EvalString();
        }
        else
        {
            if (assignEntry->left->symbolType != Symbol::SymbolExpressionType)
            {
                compiler->Error(Format("Render state entry '%s' must be a valid identifier", assignEntry->left->EvalString().c_str()), assignEntry);
                return false;
            }
            entry = assignEntry->left->EvalString();
        }

        RenderState::__Resolved::RenderStateEntryType entryType = RenderState::__Resolved::StringToEntryType(entry);
        if (entryType == RenderState::__Resolved::InvalidRenderStateEntryType)
        {
            compiler->Error(Format("Invalid render state entry '%s'", entry.c_str()), assignEntry);
            return false;
        }

        // Look at the array states (blend per MRT)
        if (entryType >= RenderState::__Resolved::BlendEnabledType && entryType <= RenderState::__Resolved::ColorComponentMaskType)
        {
            uint32_t index = -1;

            // If array expression (blend state) get the index
            if (assignEntry->left->symbolType == Symbol::ArrayIndexExpressionType)
            {
                const ArrayIndexExpression* lhs = static_cast<const ArrayIndexExpression*>(assignEntry->left);
                lhs->right->EvalUInt(index);
            }
            else
            {
                compiler->Error(Format("Blend state entry '%s' must be an array accessor", assignEntry->EvalString().c_str()), assignEntry);
                return false;
            }

            if (index >= 8)
            {
                compiler->Error(Format("Only 8 blend states are allowed"), assignEntry);
                return false;
            }

            uint32_t enumValue = 0;
            switch (entryType)
            {
            case RenderState::__Resolved::BlendEnabledType:
                if (!assignEntry->right->EvalBool(stateResolved->blendStates[index].blendEnabled))
                {
                    compiler->Error(Format("Blend state entry '%s' must evaluate to a compile time bool", entry.c_str()), assignEntry);
                    return false;
                }
                break;
            case RenderState::__Resolved::SourceBlendColorFactorType:
                assignEntry->right->EvalUInt(enumValue);
                stateResolved->blendStates[index].sourceColorBlendFactor = (GPULang::BlendFactor)enumValue;
                break;
            case RenderState::__Resolved::DestinationBlendColorFactorType:
                assignEntry->right->EvalUInt(enumValue);
                stateResolved->blendStates[index].destinationColorBlendFactor = (GPULang::BlendFactor)enumValue;
                break;
            case RenderState::__Resolved::SourceBlendAlphaFactorType:
                assignEntry->right->EvalUInt(enumValue);
                stateResolved->blendStates[index].sourceAlphaBlendFactor = (GPULang::BlendFactor)enumValue;
                break;
            case RenderState::__Resolved::DestinationBlendAlphaFactorType:
                assignEntry->right->EvalUInt(enumValue);
                stateResolved->blendStates[index].destinationAlphaBlendFactor = (GPULang::BlendFactor)enumValue;
                break;
            case RenderState::__Resolved::ColorBlendOpType:
                assignEntry->right->EvalUInt(enumValue);
                stateResolved->blendStates[index].colorBlendOp = (GPULang::BlendOp)enumValue;
                break;
            case RenderState::__Resolved::AlphaBlendOpType:
                assignEntry->right->EvalUInt(enumValue);
                stateResolved->blendStates[index].alphaBlendOp = (GPULang::BlendOp)enumValue;
                break;
                
            }
        }
        else if (entryType >= RenderState::__Resolved::StencilFailOpType && entryType <= RenderState::__Resolved::StencilReferenceMaskType)
        {
            AccessExpression* access = static_cast<AccessExpression*>(assignEntry->left);
            std::string face = access->left->EvalString();
            StencilState* state;
            if (face == "BackStencil")
                state = &stateResolved->backStencilState;
            else if (face == "FrontStencil")
                state = &stateResolved->frontStencilState;

            uint32_t enumValue = 0;
            switch (entryType)
            {
                case RenderState::__Resolved::StencilFailOpType:
                    assignEntry->right->EvalUInt(enumValue);
                    state->fail = (GPULang::StencilOp)enumValue;
                    break;
                case RenderState::__Resolved::StencilPassOpType:
                    assignEntry->right->EvalUInt(enumValue);
                    state->pass = (GPULang::StencilOp)enumValue;
                    break;
                case RenderState::__Resolved::StencilDepthFailOpType:
                    assignEntry->right->EvalUInt(enumValue);
                    state->depthFail = (GPULang::StencilOp)enumValue;
                    break;
                case RenderState::__Resolved::StencilCompareModeType:
                    assignEntry->right->EvalUInt(enumValue);
                    state->compare = (GPULang::CompareMode)enumValue;
                    break;
                case RenderState::__Resolved::StencilCompareMaskType:
                    assignEntry->right->EvalUInt(state->compareMask);
                    break;
                case RenderState::__Resolved::StencilWriteMaskType:
                    assignEntry->right->EvalUInt(state->writeMask);
                    break;
                case RenderState::__Resolved::StencilReferenceMaskType:
                    assignEntry->right->EvalUInt(state->referenceMask);
                    break;
            }
        }
        else if (entryType == RenderState::__Resolved::BlendConstantsType)
        {
            InitializerExpression* init = static_cast<InitializerExpression*>(assignEntry->right);
            if (init->values.size() != 4)
            {
                compiler->Error(Format("Blend constants must be an initializer of 4 values"), symbol);
                return false;
            }

            bool result = true;
            result |= init->values[0]->EvalFloat(stateResolved->blendConstants[0]);
            result |= init->values[1]->EvalFloat(stateResolved->blendConstants[1]);
            result |= init->values[2]->EvalFloat(stateResolved->blendConstants[2]);
            result |= init->values[3]->EvalFloat(stateResolved->blendConstants[3]);
            if (!result)
            {
                compiler->Error(Format("Each value in the initializer must resolve to a compile time literal"), symbol);
                return false;
            }
        }
        else
        {
            uint32_t enumValue = 0;
            switch (entryType)
            {
                case RenderState::__Resolved::DepthClampEnabledType:
                    assignEntry->right->EvalBool(stateResolved->depthClampEnabled);
                    break;
                case RenderState::__Resolved::NoPixelsType:
                    assignEntry->right->EvalBool(stateResolved->noPixels);
                    break;
                case RenderState::__Resolved::PolygonModeType:
                    assignEntry->right->EvalUInt(enumValue);
                    stateResolved->polygonMode = (GPULang::PolygonMode)enumValue;
                    break;
                case RenderState::__Resolved::CullModeType:
                    assignEntry->right->EvalUInt(enumValue);
                    stateResolved->cullMode = (GPULang::CullMode)enumValue;
                    break;
                case RenderState::__Resolved::WindingOrderType:
                    assignEntry->right->EvalUInt(enumValue);
                    stateResolved->windingOrderMode = (GPULang::WindingOrderMode)enumValue;
                    break;
                case RenderState::__Resolved::DepthBiasEnabledType:
                    assignEntry->right->EvalBool(stateResolved->depthBiasEnabled);
                    break;
                case RenderState::__Resolved::DepthBiasFactorType:
                    assignEntry->right->EvalFloat(stateResolved->depthBiasFactor);
                    break;
                case RenderState::__Resolved::DepthBiasClampType:
                    assignEntry->right->EvalFloat(stateResolved->depthBiasClamp);
                    break;
                case RenderState::__Resolved::DepthBiasSlopeFactorType:
                    assignEntry->right->EvalFloat(stateResolved->depthBiasSlopeFactor);
                    break;
                case RenderState::__Resolved::LineWidthType:
                    assignEntry->right->EvalFloat(stateResolved->lineWidth);
                    break;
                case RenderState::__Resolved::DepthTestEnabledType:
                    assignEntry->right->EvalBool(stateResolved->depthTestEnabled);
                    break;
                case RenderState::__Resolved::DepthWriteEnabledType:
                    assignEntry->right->EvalBool(stateResolved->depthWriteEnabled);
                    break;
                case RenderState::__Resolved::DepthTestFunction:
                    assignEntry->right->EvalUInt(enumValue);
                    stateResolved->depthCompare = (GPULang::CompareMode)enumValue;
                    break;
                case RenderState::__Resolved::DepthBoundsTestEnabledType:
                    assignEntry->right->EvalBool(stateResolved->depthBoundsTestEnabled);
                    break;
                case RenderState::__Resolved::MinDepthBoundsType:
                    assignEntry->right->EvalFloat(stateResolved->minDepthBounds);
                    break;
                case RenderState::__Resolved::MaxDepthBoundsType:
                    assignEntry->right->EvalFloat(stateResolved->maxDepthBounds);
                    break;
                case RenderState::__Resolved::LogicOpEnabledType:
                    assignEntry->right->EvalBool(stateResolved->logicOpEnabled);
                    break;
                case RenderState::__Resolved::StencilEnabledType:
                    assignEntry->right->EvalBool(stateResolved->stencilEnabled);
                    break;
                case RenderState::__Resolved::LogicOpType:
                    assignEntry->right->EvalUInt(enumValue);
                    stateResolved->logicOp = (GPULang::LogicOp)enumValue;
                    break;
                default:
                    compiler->Error(Format("Unknown render state entry '%s'", entry.c_str()), symbol);
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

    if (!compiler->AddSymbol(symbol->name, symbol))
        return false;

    // run attribute validation
    for (const Attribute& attr : struc->attributes)
    {
        if (!set_contains(this->allowedStructureAttributes, attr.name))
        {
            compiler->Error(Format("Invalid attribute for structure '%s': '%s'", struc->name.c_str(), attr.name.c_str()), symbol);
            return false;
        }

        if (set_contains(structureQualifiers, attr.name))
        {
            if (attr.name == "packed")
                strucResolved->packMembers = true;
        }
    }

    // push scope for struct but not for storage and constant buffers
    Compiler::LocalScope scope = Compiler::LocalScope::MakeTypeScope(compiler, struc);

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

            uint32_t packedOffset = offset;
            if (!strucResolved->packMembers)
                offset = Type::Align(offset, varResolved->typeSymbol->CalculateAlignment());

            uint32_t diff = offset - packedOffset;
            varResolved->startPadding = diff;
            varResolved->structureOffset = offset;
            offset += varResolved->byteSize;

            strucResolved->byteSize += varResolved->byteSize;
        }
    }

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

    if (!compiler->AddSymbol(enumeration->name, enumeration))
        return false;

    if (enumeration->type.name != "u32" && enumeration->type.name != "i32" && enumeration->type.name != "u16" && enumeration->type.name != "i16")
    {
        compiler->Error(Format("Enumeration can only be either 'u32' 'i32' 'u16' or 'i16'"), symbol);
        return false;
    }

    enumResolved->typeSymbol = compiler->GetSymbol<Type>(enumeration->type.name);

    for (auto symbol : enumeration->globals)
        if (symbol->symbolType == Symbol::FunctionType)
        {
            Function* fun = static_cast<Function*>(symbol);
            delete fun;
        }

    for (auto symbol : enumeration->staticSymbols)
        if (symbol->symbolType == Symbol::FunctionType)
        {
            Function* fun = static_cast<Function*>(symbol);
            delete fun;
        }
    enumeration->globals.clear();
    enumeration->staticSymbols.clear();

    // Create constructor from type, and to type
    Function* fromUnderlyingType = new Function;
    fromUnderlyingType->name = enumeration->name;
    fromUnderlyingType->returnType = Type::FullType{ enumeration->name };
    Variable* arg = new Variable;
    arg->name = "_arg0";
    arg->type = enumeration->type;
    arg->type.literal = false;
    fromUnderlyingType->parameters.push_back(arg);
    enumeration->globals.push_back(fromUnderlyingType);

    fromUnderlyingType = new Function;
    fromUnderlyingType->name = enumeration->name;
    fromUnderlyingType->returnType = Type::FullType{ enumeration->name };
    fromUnderlyingType->returnType.literal = true;
    arg = new Variable;
    arg->name = "_arg0";
    arg->type = enumeration->type;
    arg->type.literal = true;
    fromUnderlyingType->parameters.push_back(arg);
    enumeration->globals.push_back(fromUnderlyingType);

    Function* toUnderlyingType = new Function;
    toUnderlyingType->name = enumeration->type.name;
    toUnderlyingType->returnType = enumeration->type;
    arg = new Variable;
    arg->name = "_arg0";
    arg->type = Type::FullType{ enumeration->name };
    arg->type.literal = false;
    toUnderlyingType->parameters.push_back(arg);
    enumeration->globals.push_back(toUnderlyingType);

    toUnderlyingType = new Function;
    toUnderlyingType->name = enumeration->type.name;
    toUnderlyingType->returnType = enumeration->type;
    toUnderlyingType->returnType.literal = true;
    arg = new Variable;
    arg->name = "_arg0";
    arg->type = Type::FullType{ enumeration->name };
    arg->type.literal = true;
    toUnderlyingType->parameters.push_back(arg);
    enumeration->globals.push_back(toUnderlyingType);

    Function* comparison = new Function;
    comparison->name = "operator==";
    comparison->returnType = { "b8" };
    arg = new Variable;
    arg->name = "rhs";
    arg->type = Type::FullType{ enumeration->name };
    comparison->parameters.push_back(arg);
    enumeration->staticSymbols.push_back(comparison);

    comparison = new Function;
    comparison->name = "operator!=";
    comparison->returnType = { "b8" };
    arg = new Variable;
    arg->name = "rhs";
    arg->type = Type::FullType{ enumeration->name };
    comparison->parameters.push_back(arg);
    enumeration->staticSymbols.push_back(comparison);

    uint32_t nextValue = 0;
    for (size_t i = 0; i < enumeration->labels.size(); i++)
    {
        const std::string& str = enumeration->labels[i];
        Expression* expr = enumeration->values[i];

        // Check of label redefinition
        if (enumeration->lookup.find(str) != enumeration->lookup.end())
        {
            compiler->Error(Format("Enumeration redefinition '%s' in '%s'", str.c_str(), enumeration->name.c_str()), symbol);
            return false;
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
                uint32_t value;
                expr->EvalUInt(value);
                sym = new EnumExpression(value, expressionType, enumeration->type);
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
            sym = new EnumExpression(nextValue++, expressionType, enumeration->type);
        }

        // Add to type
        sym->name = str;
        enumeration->symbols.push_back(sym);
        enumeration->lookup.insert({ sym->name, sym });
    }

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
    if (var->name.substr(0, 3) == "gpl" && !compiler->ignoreReservedWords)
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
        varResolved->value = var->valueExpression;
    }

    if (var->type.name == "unknown")
    {
        if (var->valueExpression != nullptr)
            var->valueExpression->EvalType(var->type);
        else
        {
            compiler->Error(Format("'%s' can't infer it's type, either initialize the value or declare its type explicitly", var->name.c_str()), symbol);
            return false;
        }
    }

    Type::FullType::Modifier lastIndirectionModifier = var->type.LastIndirectionModifier();

    Type* type = (Type*)compiler->GetSymbol(var->type.name);
    if (type == nullptr)
    {
        compiler->UnrecognizedTypeError(var->type.name, symbol);
        return false;
    }
    varResolved->typeSymbol = type;
    var->type.name = type->name;        // because we can do an alias lookup, this value might change
    varResolved->type = var->type;
    varResolved->name = var->name;
    varResolved->accessBits.flags.readAccess = true; // Implicitly set read access to true
    varResolved->byteSize = type->byteSize;

    if (!this->ValidateType(compiler, var->type, varResolved->typeSymbol, var))
        return false;

    // struct members may only be scalars
    if (varResolved->usageBits.flags.isStructMember && 
        (type->category != Type::ScalarCategory && type->category != Type::EnumCategory))
    {
        compiler->Error(Format("'%s' may only be scalar type if member of a struct", varResolved->name.c_str()), symbol);
        return false;
    }

    // Add symbol
    if (!compiler->AddSymbol(var->name, var))
        return false;

    // If struct member, only allow sized arrays and no initializers
    if (varResolved->usageBits.flags.isStructMember)
    {
        if (varResolved->value != nullptr)
        {
            compiler->Error(Format("'struct' members may not have initializers"), symbol);
            return false;
        }

        for (uint32_t size : varResolved->type.modifierValues)
        {
            if (size == 0)
            {
                compiler->Error(Format("'struct' array member can't be of dynamic size"), symbol);
                return false;
            }
        }
    }

    // figure out set of allowed attributes
    std::set<std::string>* allowedAttributesSet = nullptr;
    if (varResolved->usageBits.flags.isStructMember)
        allowedAttributesSet = nullptr;
    else if (varResolved->usageBits.flags.isParameter)
        allowedAttributesSet = &this->allowedParameterAttributes;
    else
    {
        if (type->category == Type::TextureCategory)
            allowedAttributesSet = &this->allowedTextureAttributes;
        else if (type->category == Type::ScalarCategory || type->category == Type::EnumCategory)
            allowedAttributesSet = &this->allowedScalarAttributes;
        else if (type->category == Type::SamplerCategory)
            allowedAttributesSet = &this->allowedSamplerAttributes;
        else if (type->category == Type::UserTypeCategory
                && lastIndirectionModifier == Type::FullType::Modifier::Pointer)
        {
            allowedAttributesSet = &this->allowedPointerAttributes;
        }
    }

    // run attribute validation
    for (const Attribute& attr : var->attributes)
    {
        if (attr.expression != nullptr)
            attr.expression->Resolve(compiler);
        if (allowedAttributesSet == nullptr || (!set_contains(*allowedAttributesSet, attr.name)))
        {
            compiler->Error(Format("Invalid attribute for type '%s': '%s'", varResolved->name.c_str(), attr.name.c_str()), symbol);
            return false;
        }

        // resolve attributes
        if (attr.name == "group")
        {
            if (!attr.expression->EvalUInt(varResolved->group))
            {
                compiler->Error(Format("Expected compile time constant for 'group' qualifier"), symbol);
                return false;
            }
        }
        else if (attr.name == "binding")
        {
            if (!attr.expression->EvalUInt(varResolved->binding))
            {
                compiler->Error(Format("Expected compile time constant for 'binding' qualifier"), symbol);
                return false;
            }
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
        else if (attr.name == "const")
            varResolved->usageBits.flags.isConst = true;
        else if (attr.name == "var")
            varResolved->usageBits.flags.isVar = true;
        else if (attr.name == "link_defined")
        {
            if (varResolved->storage != Variable::__Resolved::Storage::Default)
            {
                compiler->Error(Format("Multiple storage qualifiers are not allowed"), symbol);
                return false;
            }
            varResolved->storage = Variable::__Resolved::Storage::LinkDefined;
            varResolved->binding = compiler->linkDefineCounter++;
        }
        else if (attr.name == "uniform")
        {
            if (varResolved->storage != Variable::__Resolved::Storage::Default)
            {
                compiler->Error(Format("Multiple storage qualifiers are not allowed"), symbol);
                return false;
            }
            varResolved->storage = Variable::__Resolved::Storage::Uniform;
            varResolved->usageBits.flags.isConst = true & !varResolved->type.IsMutable();
        }
        else if (attr.name == "inline")
        {
            if (varResolved->storage != Variable::__Resolved::Storage::Default)
            {
                compiler->Error(Format("Multiple storage qualifiers are not allowed"), symbol);
                return false;
            }
            varResolved->storage = Variable::__Resolved::Storage::InlineUniform;
            varResolved->usageBits.flags.isConst = true;
        }
        else if (attr.name == "workgroup")
        {
            if (varResolved->storage != Variable::__Resolved::Storage::Default)
            {
                compiler->Error(Format("Multiple storage qualifiers are not allowed"), symbol);
                return false;
            }
            varResolved->storage = Variable::__Resolved::Storage::Workgroup;
        }
        else
        {
            // more complicated lookups
            if (set_contains(pointerQualifiers, attr.name))
            {
                if (attr.name == "no_read")
                    varResolved->accessBits.flags.readAccess = false;
                else if (attr.name == "atomic")
                    varResolved->accessBits.flags.atomicAccess = true;
                else if (attr.name == "volatile")
                    varResolved->accessBits.flags.volatileAccess = true;
            }
        }

        if (set_contains(parameterAccessFlags, attr.name))
        {
            if (attr.name == "in")
                varResolved->storage = Variable::__Resolved::Storage::Input;
            else if (attr.name == "out")
                varResolved->storage = Variable::__Resolved::Storage::Output;
        }
        else if (set_contains(parameterQualifiers, attr.name))
        {
            if (attr.name == "patch")
                varResolved->parameterBits.flags.isPatch = true;
            else if (attr.name == "no_perspective")
                varResolved->parameterBits.flags.isNoPerspective = true;
            else if (attr.name == "no_interpolate")
                varResolved->parameterBits.flags.isNoInterpolate = true;
            else if (attr.name == "centroid")
                varResolved->parameterBits.flags.isCentroid = true;
        }

        // check image formats
        if (map_contains(StringToFormats, attr.name))
        {
            varResolved->imageFormat = StringToFormats[attr.name];
        }
    }

    if (!varResolved->usageBits.flags.isParameter && !varResolved->usageBits.flags.isStructMember && (varResolved->storage == Variable::__Resolved::Storage::Default || varResolved->storage == Variable::__Resolved::Storage::Global))
    {
        if (varResolved->usageBits.flags.isVar && varResolved->usageBits.flags.isConst)
        {
            compiler->Error(Format("Variable may be either 'var' or 'const' but not both"), var);
            return false;
        }
        else if (!varResolved->usageBits.flags.isVar && !varResolved->usageBits.flags.isConst)
        {
            compiler->Error(Format("Variable must be either 'var' or 'const'"), var);
            return false;
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
        if (type->category == Type::UserTypeCategory && varResolved->storage != Variable::__Resolved::Storage::InlineUniform && varResolved->storage != Variable::__Resolved::Storage::Uniform && varResolved->type.IsPointer())
        {
            compiler->Error(Format("Type may not be pointer if target language ('%s') does not support physical addressing", compiler->target.name.c_str()), var);
            return false;
        }
    }

    if (compiler->IsScopeGlobal())
    {
        if (type->category != Type::ScalarCategory && type->category != Type::EnumCategory)
        {
            uint16_t numArrays = 0;
            uint16_t numPointers = 0;
            for (Type::FullType::Modifier mod : varResolved->type.modifiers)
            {
                if (mod == Type::FullType::Modifier::Array)
                    numArrays++;
                else if (mod == Type::FullType::Modifier::Pointer)
                    numPointers++;
            }
            if (numArrays > 1)
            {
                compiler->Error(Format("Variables of non scalar type in the global scope may only be one dimensional array"), symbol);
                return false;
            }
            if (numPointers > 1)
            {
                compiler->Error(Format("Variables of non scalar type in the global scope may only be single pointer"), symbol);
                return false;
            }

            if (varResolved->storage != Variable::__Resolved::Storage::Uniform && varResolved->storage != Variable::__Resolved::Storage::InlineUniform)
            {
                compiler->Error(Format("Variable of non scalar type in the global scope must either use 'uniform' or 'inline' storage", type->name.c_str()), symbol);
                return false;
            }
        }
        else // scalar
        {
            if (varResolved->usageBits.flags.isConst && varResolved->value == nullptr)
            {
                // check for variable initialization criteria
                compiler->Error(Format("Variable declared as const but is never initialized"), symbol);
                return false;
            }
            if (varResolved->storage == Variable::__Resolved::Storage::Default)
                varResolved->storage = Variable::__Resolved::Storage::Global;
        }
    }
    else
    {
        if (lastIndirectionModifier == Type::FullType::Modifier::Pointer && !varResolved->usageBits.flags.isParameter)
        {
            compiler->Error(Format("Pointers are only allowed as symbols in the global scope", type->name.c_str()), symbol);
            return false;
        }

        if (varResolved->storage == Variable::__Resolved::Storage::Workgroup)
        {
            compiler->Error(Format("Variables with 'workgroup' storage may only be global", type->name.c_str()), symbol);
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
            if (varResolved->storage == Variable::__Resolved::Storage::Uniform)
            {
                compiler->Error(Format("Variable of uniform '%s' type must be pointer", type->name.c_str()), symbol);
                return false;
            }
            else if (varResolved->storage == Variable::__Resolved::Storage::InlineUniform)
            {
                compiler->Error(Format("Variable of inline '%s' type must be pointer", type->name.c_str()), symbol);
                return false;
            }
        }
    }

    if (varResolved->storage == Variable::__Resolved::Storage::LinkDefined)
    {
        if (varResolved->typeSymbol->columnSize > 1 || varResolved->typeSymbol->category != Type::Category::ScalarCategory)
        {
            compiler->Error(Format("Only scalar types can be 'link_defined'"), symbol);
            return false;
        }
    }

    // Check that the type can be mutable
    if (varResolved->type.IsMutable())
    {
        varResolved->accessBits.flags.writeAccess = true;
        if (type->category == Type::SamplerCategory)
        {
            compiler->Error(Format("Sampler can not be mutable", type->name.c_str()), symbol);
            return false;
        }
        else if (type->category == Type::PixelCacheCategory)
        {

            compiler->Error(Format("PixelCache can not be mutable", type->name.c_str()), symbol);
            return false;
        }
        else if (type->category == Type::ScalarCategory)
        {
            compiler->Error(Format("Scalar can not be mutable", type->name.c_str()), symbol);
            return false;
        }
        else if (type->category == Type::EnumCategory)
        {
            compiler->Error(Format("Enum can not be mutable", type->name.c_str()), symbol);
            return false;
        }
    }

    // validate types on both sides of the assignment
    if (varResolved->value != nullptr)
    {
        Type::FullType lhs = varResolved->type;
        Type::FullType rhs;
        if (!varResolved->value->EvalType(rhs))
        {
            compiler->UnrecognizedTypeError(rhs.name, symbol);
            return false;
        }

        if (lhs != rhs)
        {
            Type* lhsType = compiler->GetSymbol<Type>(lhs.name);
            std::vector<Symbol*> assignmentOperators = lhsType->GetSymbols(Format("operator=(%s)", rhs.name.c_str()));
            Symbol* fun = Function::MatchOverload(compiler, assignmentOperators, { rhs });
            if (fun == nullptr)
            {
                compiler->Error(Format("Type '%s' can't be converted to '%s'", lhs.ToString().c_str(), rhs.ToString().c_str()), symbol);
                return false;
            }

            if (fun != nullptr
                && compiler->options.disallowImplicitConversion)
            {
                compiler->Error(Format("Initialization not possible because implicit conversions ('%s' to '%s') are not allowed. Either disable implicit conversions or explicitly convert the value.", lhs.ToString().c_str(), rhs.ToString().c_str()), symbol);
                return false;
            }
        }

        // Okay, so now when we're done, we'll copy over the modifier values from rhs to lhs
        varResolved->type.modifierValues = rhs.modifierValues;
    }

    // check if image formats have been resolved
    if (type->category == Type::TextureCategory
        && varResolved->type.mut
        && (varResolved->imageFormat == InvalidImageFormat || varResolved->imageFormat == Unknown)
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
            if (varResolved->storage == Variable::__Resolved::Storage::Input)
                varResolved->inBinding = this->inParameterIndexCounter++;
            if (varResolved->storage == Variable::__Resolved::Storage::Output)
                varResolved->outBinding = this->outParameterIndexCounter++;
        }
        else
        {
            varResolved->inBinding = varResolved->binding;
            varResolved->outBinding = varResolved->binding;
            if (varResolved->storage == Variable::__Resolved::Storage::Input)
                this->inParameterIndexCounter = varResolved->binding + 1;
            if (varResolved->storage == Variable::__Resolved::Storage::Output)
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
                if (varResolved->storage == Variable::__Resolved::Storage::Uniform && lastIndirectionModifier != Type::FullType::Modifier::Pointer)
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
            || (cat == Type::Category::UserTypeCategory && varResolved->storage == Variable::__Resolved::Storage::Uniform))
        {
            if (varResolved->group == Variable::__Resolved::NOT_BOUND)
            {
                varResolved->group = this->defaultGroup;
            }

            if (this->resourceIndexingMode == ResourceIndexingByType)
            {
                auto it = this->resourceIndexCounter.find(cat);
                if (it == this->resourceIndexCounter.end())
                {
                    this->resourceIndexCounter[cat] = 0;
                    it = this->resourceIndexCounter.find(varResolved->group);
                }

                if (varResolved->binding == Variable::__Resolved::NOT_BOUND)
                {
                    varResolved->binding = it->second++;
                }
                else
                {
                    this->resourceIndexCounter[cat] = max(it->second, varResolved->binding + 1);
                }
            }
            else if (this->resourceIndexingMode == ResourceIndexingByGroup)
            {
                auto it = this->resourceIndexCounter.find(varResolved->group);
                if (it == this->resourceIndexCounter.end())
                {
                    this->resourceIndexCounter[varResolved->group] = 0;
                    it = this->resourceIndexCounter.find(varResolved->group);
                }

                if (varResolved->binding == Variable::__Resolved::NOT_BOUND)
                {
                    varResolved->binding = it->second++;
                }
                else
                {
                    this->resourceIndexCounter[varResolved->group] = max(it->second, varResolved->binding + 1);
                }
            }
        }

        if (cat == Type::Category::UserTypeCategory && varResolved->storage == Variable::__Resolved::Storage::Uniform)
        {
            Structure* currentStructure = static_cast<Structure*>(varResolved->typeSymbol);
            Structure::__Resolved* currentStrucResolved = Symbol::Resolved(currentStructure);

            // If the structure is packed, we need to inflate it to adhere to alignment rules
            if (currentStrucResolved->packMembers)
            {
                 // Generate mutable/uniform variant of struct
                Structure* generatedStruct = new Structure;
                uint32_t structSize = 0;
                uint32_t padCounter = 0;
                uint32_t offset = 0;
                for (Symbol* sym : type->symbols)
                {
                    if (sym->symbolType == Symbol::VariableType)
                    {
                        Variable* var = static_cast<Variable*>(sym);
                        Variable::__Resolved* varResolved = Symbol::Resolved(var);
                        Variable* generatedVar = new Variable;
                        Variable::__Resolved* generatedVarResolved = Symbol::Resolved(generatedVar);
                        generatedVar->name = var->name;
                        generatedVar->type = var->type;
                        generatedVarResolved->usageBits = varResolved->usageBits;
                        generatedVarResolved->type = varResolved->type;
                        generatedVarResolved->typeSymbol = varResolved->typeSymbol;
                        generatedVarResolved->name = varResolved->name;
                        uint32_t size = varResolved->typeSymbol->CalculateSize();
                        uint32_t alignment = varResolved->typeSymbol->CalculateAlignment();
                        uint32_t alignedOffset = Type::Align(offset, alignment);
                        generatedVarResolved->startPadding = alignedOffset - offset;
                        generatedVarResolved->byteSize = size;
                        generatedVarResolved->structureOffset = alignedOffset;
                        offset = alignedOffset + size;
                        structSize += generatedVarResolved->byteSize + generatedVarResolved->startPadding;
                        generatedStruct->symbols.push_back(generatedVar);
                        generatedStruct->lookup.insert({ varResolved->name, generatedVar });
                    }
                }
                Structure::__Resolved* generatedStructResolved = Symbol::Resolved(generatedStruct);
                const char* bufferType = varResolved->type.IsMutable() ? "MutableBuffer" : "Buffer";
                generatedStruct->name = Format("gpl%s_%s", bufferType, varResolved->name.c_str()); ;
                //generatedStruct->annotations = var->annotations;
            
                generatedStructResolved->byteSize = structSize;
                generatedStructResolved->packMembers = false;
                //generatedStructResolved->byteSize = varResolved
                if (varResolved->type.IsMutable())
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
                newType.sampled = var->type.sampled;
                var->type = newType;
                varResolved->typeSymbol = generatedStruct;
                varResolved->type = newType;

                // Insert symbol before this one, avoiding resolving (we assume the struct and members are already valid)
                compiler->symbols.insert(compiler->symbols.begin() + compiler->symbolIterator, generatedStruct);
                compiler->scopes.back()->symbolLookup.insert({ generatedStruct->name, generatedStruct });
                compiler->symbolIterator++;
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
        case Symbol::BreakStatementType:
        {
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
            return statement->expr->Resolve(compiler);
        }
        case Symbol::ForStatementType:
        {
            auto statement = reinterpret_cast<ForStatement*>(symbol);
            for (const auto& attr : statement->attributes)
            {
                if (attr.name == "unroll")
                {
                    if (attr.expression == nullptr)
                        statement->unrollCount = UINT_MAX;
                    else
                    {
                        if (!attr.expression->EvalUInt(statement->unrollCount))
                        {
                            compiler->Error(Format("Unroll count must evaluate to a literal integer value", attr.name.c_str()), statement);
                            return false;
                        }
                    }
                }
                else
                {
                    compiler->Error(Format("Invalid loop modifier '%s'", attr.name.c_str()), statement);
                    return false;
                }
            }
            Compiler::LocalScope scope = Compiler::LocalScope::MakeLocalScope(compiler, statement);
            for (Variable* var : statement->declarations)
            {
                if (!this->ResolveVariable(compiler, var))
                    return false;
            }
            Type::FullType type;
            if (statement->condition->Resolve(compiler))
            {
                statement->condition->EvalType(type);
                Type* typeSymbol = compiler->GetSymbol<Type>(type.name);
                if (typeSymbol->baseType != TypeCode::Bool)
                {
                    compiler->Error(Format("Loop condition must evaluate to a boolean value"), statement);
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
            if (!this->ResolveStatement(compiler, statement->ifStatement))
                return false;
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
            return true;
        }
        case Symbol::ReturnStatementType:
        {
            auto statement = reinterpret_cast<ReturnStatement*>(symbol);
            Symbol* scopeOwner = compiler->GetParentScopeOwner(Symbol::FunctionType);
            if (scopeOwner == nullptr)
            {
                compiler->Error(Format("'return' is only valid inside function body"), statement);
                return false;
            }
            Function* functionOwner = static_cast<Function*>(scopeOwner);
            Function::__Resolved* functionOwnerResolved = Symbol::Resolved(functionOwner);
            compiler->branchReturns = true;

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
                if (functionOwner->returnType != Type::FullType{ "void" })
                {
                    compiler->Error(Format("Function expects return of type '%s', got 'void'", functionOwner->returnType.ToString().c_str()), statement);
                    return false;
                }
            }
            compiler->MarkScopeUnreachable();
            return ret;
        }
        case Symbol::ScopeStatementType:
        {
            auto statement = reinterpret_cast<ScopeStatement*>(symbol);
            Compiler::LocalScope scope = Compiler::LocalScope::MakeLocalScope(compiler, compiler->GetScopeOwner());

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
            return true;
        }
        case Symbol::SwitchStatementType:
        {
            auto statement = reinterpret_cast<SwitchStatement*>(symbol);
            Compiler::LocalScope scope = Compiler::LocalScope::MakeLocalScope(compiler, statement);

            if (statement->switchExpression->Resolve(compiler))
            {
                Type::FullType type;
                statement->switchExpression->EvalType(type);
                Type* typeSymbol = compiler->GetSymbol<Type>(type.name);
                if (typeSymbol->baseType != TypeCode::Int && typeSymbol->baseType != TypeCode::UInt)
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
            for (size_t i = 0; i < statement->caseExpressions.size(); i++)
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
            auto statement = reinterpret_cast<WhileStatement*>(symbol);
            Compiler::LocalScope scope = Compiler::LocalScope::MakeLocalScope(compiler, statement);
            Type::FullType type;
            if (statement->condition->Resolve(compiler))
            {
                statement->condition->EvalType(type);
                Type* typeSymbol = compiler->GetSymbol<Type>(type.name);
                if (typeSymbol->baseType != TypeCode::Bool)
                {
                    compiler->Error(Format("While condition has to evaluate to a boolean value"), statement);
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
Validator::ValidateFunction(Compiler* compiler, Symbol* symbol)
{
    Function* fun = static_cast<Function*>(symbol);
    Function::__Resolved* funResolved = Symbol::Resolved(fun);

    for (Variable* var : fun->parameters)
    {
        // if function is used for shader, validate parameters with special rules
        if (funResolved->shaderUsage.bits != 0x0)
        {
            Variable::__Resolved* varResolved = Symbol::Resolved(var);

            if (varResolved->parameterBits.flags.isPatch
                && !(funResolved->shaderUsage.flags.hullShader || funResolved->shaderUsage.flags.domainShader))
            {
                compiler->Error(Format("Parameter '%s' can not use 'patch' if function is not being used as a HullShader/TessellationControlShader or DomainShader/TessellationEvaluationShader", var->name.c_str(), fun->name.c_str()), var);
                return false;
            }

            if (varResolved->parameterBits.flags.isNoInterpolate
                && !funResolved->shaderUsage.flags.pixelShader)
            {
                compiler->Error(Format("Parameter '%s' can not use 'no_interpolate' if function is not being used as a PixelShader", var->name.c_str(), fun->name.c_str()), var);
                return false;
            }

            if (varResolved->parameterBits.flags.isNoPerspective
                && !funResolved->shaderUsage.flags.pixelShader)
            {
                compiler->Error(Format("Parameter '%s' can not use 'no_perspective' if function is not being used as a PixelShader", var->name.c_str(), fun->name.c_str()), var);
                return false;
            }
        }
    }

    if (funResolved->shaderUsage.flags.vertexShader)
    {
        if (!funResolved->sideEffects.flags.exportsVertexPosition)
        {
            compiler->Warning(Format("Function '%s' is used as vertex shader but produces no vertex position", fun->name.c_str()), symbol);
        }
    }

    // validate function attribute validity
    if (funResolved->shaderUsage.flags.pixelShader)
    {
        if (!funResolved->sideEffects.flags.exportsPixel)
            compiler->Warning(Format("Function '%s' is used as pixel shader but produces no color output", fun->name.c_str()), symbol);
        if (funResolved->executionModifiers.earlyDepth && funResolved->sideEffects.flags.exportsExplicitDepth)
            compiler->Warning(Format("Function '%s' uses early depth testing but writes an explicit depth value", fun->name.c_str()), symbol);
    }
    else
    {
        if (funResolved->executionModifiers.earlyDepth)
            compiler->Warning(Format("Function '%s' has attribute 'early_depth' but is not used as a pixel shader", fun->name.c_str()), symbol);
    }

    if (funResolved->shaderUsage.flags.hullShader)
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

    if (funResolved->shaderUsage.flags.domainShader)
    {
        // validate required qualifiers
        if (funResolved->executionModifiers.patchType != Function::__Resolved::InvalidPatchType)
        {
            compiler->Warning(Format("Domain shader '%s' does not define 'patch_type' for DomainShader/TessellationEvaluationShader, defaulting to 'triangles'", fun->name.c_str()), symbol);
            funResolved->executionModifiers.patchType = Function::__Resolved::PatchType::TrianglePatch;
        }

    }
    else
    {
        if (funResolved->executionModifiers.patchType != Function::__Resolved::InvalidPatchType)
            compiler->Warning(Format("Function '%s' has attribute 'patch_type' but is not used as a DomainShader/TessellationEvaluationShader", fun->name.c_str()), symbol);
        if (funResolved->executionModifiers.partitionMethod != Function::__Resolved::InvalidPartitionMethod)
            compiler->Warning(Format("Function '%s' has attribute 'partition' but is not used as a DomainShader/TessellationEvaluationShader", fun->name.c_str()), symbol);
    }

    if (funResolved->shaderUsage.flags.geometryShader)
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
    else
    {
        if (funResolved->executionModifiers.invocations != Function::__Resolved::INVALID_SIZE)
            compiler->Warning(Format("Function '%s' has attribute 'invocations' but is not used as a GeometryShader", fun->name.c_str()), symbol);
        if (funResolved->executionModifiers.maxOutputVertices != Function::__Resolved::INVALID_SIZE)
            compiler->Warning(Format("Function '%s' has attribute 'max_output_vertices' but is not used as a GeometryShader", fun->name.c_str()), symbol);
        if (funResolved->executionModifiers.inputPrimitiveTopology != Function::__Resolved::InvalidPrimitiveTopology)
            compiler->Warning(Format("Function '%s' has attribute 'input_topology' but is not used as a GeometryShader", fun->name.c_str()), symbol);
        if (funResolved->executionModifiers.outputPrimitiveTopology != Function::__Resolved::InvalidPrimitiveTopology)
            compiler->Warning(Format("Function '%s' has attribute 'output_topology' but is not used as a GeometryShader", fun->name.c_str()), symbol);
    }

    if (funResolved->shaderUsage.flags.computeShader)
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
    else
    {
        if (funResolved->executionModifiers.computeShaderWorkGroupSize[0] > 1)
            compiler->Warning(Format("Function '%s' sets attribute 'local_size_x' but is not used as a compute shader", fun->name.c_str()), symbol);
        if (funResolved->executionModifiers.computeShaderWorkGroupSize[1] > 1)
            compiler->Warning(Format("Function '%s' sets attribute 'local_size_y' but is not used as a compute shader", fun->name.c_str()), symbol);
        if (funResolved->executionModifiers.computeShaderWorkGroupSize[2] > 1)
            compiler->Warning(Format("Function '%s' sets attribute 'local_size_z' but is not used as a compute shader", fun->name.c_str()), symbol);
        if (funResolved->executionModifiers.groupSize != 64)
            compiler->Warning(Format("Function '%s' sets attribute 'group_size' but is not used as a compute shader", fun->name.c_str()), symbol);
        if (funResolved->executionModifiers.groupsPerWorkgroup != 1)
            compiler->Warning(Format("Function '%s' sets attribute 'groups_per_workgroup' but is not used as a compute shader", fun->name.c_str()), symbol);
    }

    return true;
}

//------------------------------------------------------------------------------
/**
    First filters parameters based on in/out qualifiers, then sorts within that set
    based on either inBinding or outBinding respectively
*/
std::vector<Variable*>
SortAndFilterParameters(const std::vector<Variable*>& vars, bool in)
{
    std::vector<Variable*> ret;
    for (Variable* var : vars)
    {
        Variable::__Resolved* varResolved = Symbol::Resolved(var);
        if (varResolved->storage == Variable::__Resolved::Storage::Input && in)
            ret.push_back(var);
        else if (varResolved->storage == Variable::__Resolved::Storage::Output && !in)
            ret.push_back(var);
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
*/
bool 
ValidateParameterSets(Compiler* compiler, Function* outFunc, Function* inFunc)
{
    std::vector<Variable*> outParams = SortAndFilterParameters(outFunc->parameters, false);
    std::vector<Variable*> inParams = SortAndFilterParameters(inFunc->parameters, true);
    size_t inIterator = 0;
    for (Variable* var : outParams)
    {
        Variable::__Resolved* outResolved = Symbol::Resolved(var);
        Variable::__Resolved* inResolved = Symbol::Resolved(inParams[inIterator]);

        // if bindings don't match, it means the output will be unused since the parameter sets should be sorted
        if (outResolved->outBinding != inResolved->inBinding)
        {
            compiler->Warning(Format("Unused parameter '%s' (binding %d) from shader '%s' to '%s'", var->name.c_str(), outResolved->outBinding, outFunc->name.c_str(), inFunc->name.c_str()), outFunc);
        }
        else
        {
            if (var->type != inParams[inIterator]->type)
            {
                compiler->Error(Format("Can't match types '%s' and '%s' between shader '%s' and '%s'", var->type.name.c_str(), inParams[inIterator]->type.name.c_str(), outFunc->name.c_str(), inFunc->name.c_str()), outFunc);
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
    Program* prog = static_cast<Program*>(symbol);
    Program::__Resolved* progResolved = Symbol::Resolved(prog);

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
    for (uint32_t mapping = 0; mapping < Program::__Resolved::ProgramEntryType::NumProgramEntries; mapping++)
    {
        Symbol* object = progResolved->mappings[mapping];
        if (object == nullptr)
            continue;

        if (mapping == Program::__Resolved::ComputeShader)
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
        else if (mapping == Program::__Resolved::VertexShader
            || mapping == Program::__Resolved::HullShader
            || mapping == Program::__Resolved::DomainShader
            || mapping == Program::__Resolved::GeometryShader
            || mapping == Program::__Resolved::PixelShader
            || mapping == Program::__Resolved::TaskShader
            || mapping == Program::__Resolved::MeshShader
            )
        {
            if (programType == ProgramType::IsCompute)
            {
                compiler->Error(Format("Program may not be both general graphics and compute", Program::__Resolved::EntryTypeToString((Program::__Resolved::ProgramEntryType)mapping)), symbol);
                return false;
            }
            if (programType == ProgramType::IsRaytracing)
            {
                compiler->Error(Format("Program may not be both general graphics and ray tracing", Program::__Resolved::EntryTypeToString((Program::__Resolved::ProgramEntryType)mapping)), symbol);
                return false;
            }
            programType = ProgramType::IsGraphics;
        }
        else if (mapping == Program::__Resolved::RayAnyHitShader
            || mapping == Program::__Resolved::RayCallableShader
            || mapping == Program::__Resolved::RayIntersectionShader
            || mapping == Program::__Resolved::RayMissShader)
        {
            if (programType == ProgramType::IsCompute)
            {
                compiler->Error(Format("Program may not be both raytracing and compute", Program::__Resolved::EntryTypeToString((Program::__Resolved::ProgramEntryType)mapping)), symbol);
                return false;
            }
            if (programType == ProgramType::IsGraphics)
            {
                compiler->Error(Format("Program may not be both raytracing and general graphics", Program::__Resolved::EntryTypeToString((Program::__Resolved::ProgramEntryType)mapping)), symbol);
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
            Function* vs = static_cast<Function*>(progResolved->mappings[Program::__Resolved::VertexShader]);
            lastPrimitiveShader = vs;
        }

        if (progResolved->usage.flags.hasHullShader)
        {
            if (lastPrimitiveShader == nullptr)
            {
                compiler->Error(Format("Invalid program setup, HullShader/TessellationControlShader needs a VertexShader"), symbol);
                return false;
            }
            Function* hs = static_cast<Function*>(progResolved->mappings[Program::__Resolved::HullShader]);
            if (!ValidateParameterSets(compiler, lastPrimitiveShader, hs))
                return false;

            lastPrimitiveShader = hs;
        }

        if (progResolved->usage.flags.hasDomainShader)
        {
            if (lastPrimitiveShader == nullptr 
                && progResolved->mappings[Program::__Resolved::HullShader] != nullptr)
            {
                compiler->Error(Format("Invalid program setup, DomainShader needs a HullShader/TessellationControlShader"), symbol);
                return false;
            }
            Function* ds = static_cast<Function*>(progResolved->mappings[Program::__Resolved::DomainShader]);
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
            Function* gs = static_cast<Function*>(progResolved->mappings[Program::__Resolved::GeometryShader]);
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
            Function* ps = static_cast<Function*>(progResolved->mappings[Program::__Resolved::PixelShader]);
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

    if (!compiler->target.supportsPhysicalAddressing)
    {
        if (
            (typeSymbol->category != Type::TextureCategory && typeSymbol->category != Type::PixelCacheCategory && typeSymbol->category != Type::UserTypeCategory && typeSymbol->category != Type::SamplerCategory && numPointers > 0)
            || (numPointers > 1)
            )
        {
            compiler->Error(Format("Target language %s does not support dereferencing. Only one indirection is allowed and only on texture, pixel cache, sampler and struct types", compiler->target.name.c_str()), sym);
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
            for (auto* var : forStat->declarations)
                this->ResolveVisibility(compiler, var);
            res |= this->ResolveVisibility(compiler, forStat->condition);
            res |= this->ResolveVisibility(compiler, forStat->loop);
            res |= this->ResolveVisibility(compiler, forStat->contents);
            break;
        }
        case Symbol::WhileStatementType:
        {
            WhileStatement* whileStat = static_cast<WhileStatement*>(symbol);
            res |= this->ResolveVisibility(compiler, whileStat->condition);
            res |= this->ResolveVisibility(compiler, whileStat->statement);
            break;
        }
        case Symbol::ReturnStatementType:
        {
            ReturnStatement* returnStat = static_cast<ReturnStatement*>(symbol);
            if (returnStat->returnValue != nullptr)
                res |= this->ResolveVisibility(compiler, returnStat->returnValue);
            break;
        }
        case Symbol::ExpressionStatementType:
        {
            ExpressionStatement* exprStat = static_cast<ExpressionStatement*>(symbol);
            res |= this->ResolveVisibility(compiler, exprStat->expr);
            break;
        }
        case Symbol::IfStatementType:
        {
            IfStatement* ifStat = static_cast<IfStatement*>(symbol);
            res |= this->ResolveVisibility(compiler, ifStat->ifStatement);
            if (ifStat->elseStatement != nullptr)
                res |= this->ResolveVisibility(compiler, ifStat->elseStatement);
            break;
        }
        case Symbol::TernaryExpressionType:
        {
            TernaryExpression* ternExp = static_cast<TernaryExpression*>(symbol);
            res |= this->ResolveVisibility(compiler, ternExp->lhs);
            res |= this->ResolveVisibility(compiler, ternExp->ifExpression);
            res |= this->ResolveVisibility(compiler, ternExp->elseExpression);
            break;
        }
        case Symbol::BinaryExpressionType:
        {
            BinaryExpression* binExp = static_cast<BinaryExpression*>(symbol);
            res |= this->ResolveVisibility(compiler, binExp->left);
            res |= this->ResolveVisibility(compiler, binExp->right);
            break;
        }
        case Symbol::AccessExpressionType:
        {
            AccessExpression* access = static_cast<AccessExpression*>(symbol);
            res |= this->ResolveVisibility(compiler, access->left);
            res |= this->ResolveVisibility(compiler, access->right);
            break;
        }
        case Symbol::ArrayIndexExpressionType:
        {
            ArrayIndexExpression* arrayExpr = static_cast<ArrayIndexExpression*>(symbol);
            res |= this->ResolveVisibility(compiler, arrayExpr->left);
            res |= this->ResolveVisibility(compiler, arrayExpr->right);
            break;
        }
        case Symbol::CallExpressionType:
        {
            CallExpression* callExpr = static_cast<CallExpression*>(symbol);
            CallExpression::__Resolved* callResolved = Symbol::Resolved(callExpr);
            for (auto& arg : callExpr->args)
                res |= this->ResolveVisibility(compiler, arg);
            res |= this->ResolveVisibility(compiler, callResolved->function);
            break;
        }
        case Symbol::CommaExpressionType:
        {
            CommaExpression* commaExpr = static_cast<CommaExpression*>(symbol);
            res |= this->ResolveVisibility(compiler, commaExpr->left);
            res |= this->ResolveVisibility(compiler, commaExpr->right);
        }
        case Symbol::InitializerExpressionType:
        {
            InitializerExpression* init = static_cast<InitializerExpression*>(symbol);
            for (auto& initializer : init->values)
                res |= this->ResolveVisibility(compiler, initializer);
            break;
        }
        case Symbol::UnaryExpressionType:
        {
            UnaryExpression* unary = static_cast<UnaryExpression*>(symbol);
            res |= this->ResolveVisibility(compiler, unary->expr);
        }
        case Symbol::FunctionType:
        {
            Function* fun = static_cast<Function*>(symbol);
            if (fun->ast != nullptr)
                res |= this->ResolveVisibility(compiler, fun->ast);
            break;
        }
        case Symbol::SymbolExpressionType:
        {
            SymbolExpression* symExpr = static_cast<SymbolExpression*>(symbol);
            std::string symbol;
            symExpr->EvalSymbol(symbol);
            Symbol* newSymbol = compiler->GetSymbol(symbol);
            if (newSymbol != nullptr)
                res |= this->ResolveVisibility(compiler, newSymbol);
            break;
        }
        case Symbol::VariableType:
        {
            Variable* var = static_cast<Variable*>(symbol);
            Variable::__Resolved* varResolved = Symbol::Resolved(var);
            Function::__Resolved* currentFunResolved = Symbol::Resolved(compiler->currentFunction);
            varResolved->visibilityBits.bits |= currentFunResolved->shaderUsage.bits;
            if (varResolved->value != nullptr)
                res |= this->ResolveVisibility(compiler, varResolved->value);
            break;
        }
    }
    return res;
}

} // namespace GPULang
