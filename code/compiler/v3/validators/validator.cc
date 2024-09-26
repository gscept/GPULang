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
std::set<std::string> readWriteAccessFlags =
{
    "mutable", "no_read", "atomic", "volatile"
};

std::set<std::string> readWriteTextureQualifiers =
{
    "group", "binding"
};

std::set<std::string> textureQualifiers =
{
    "group", "binding"
};

std::set<std::string> scalarQualifiers =
{
    "const", "workgroup",
};

std::set<std::string> samplerQualifiers =
{
    "group", "binding"
};

std::set<std::string> constantBufferQualifiers =
{
    "group", "binding"
};

std::set<std::string> storageBufferAccessFlags =
{
    "mutable", "no_read", "atomic", "volatile"
};

std::set<std::string> storageBufferQualifiers =
{
    "group", "binding"
};

std::set<std::string> functionAttributes =
{
    "shader", "local_size_x", "local_size_y", "local_size_z", "early_depth"
    , "input_vertices", "max_output_vertices", "winding"
    , "topology", "patch_type", "patch_size", "partition",
    "prototype"
};

std::set<std::string> parameterAccessFlags =
{
    "in", "out", "in_out"
};

std::set<std::string> parameterQualifiers =
{
    "patch", "no_interpolate", "no_perspective", "binding", "pixel_origin"
};

std::set<std::string> structureQualifiers =
{
    "group", "binding", "push"
};

std::set<std::string> pixelShaderInputQualifiers =
{
    "binding", "no_interpolate", "no_perspective", "pixel_origin"
};

std::set<std::string> hullOutputQualifiers =
{
    "patch", "domain"
};

std::set<std::string> domainInputQualifiers =
{
    "binding", "patch"
};

std::set<std::string> attributesRequiringEvaluation =
{
    "binding", "group", "local_size_x", "local_size_y", "local_size_z"
    , "input_vertices", "max_output_vertices", "winding"
    , "topology", "patch_type", "patch_size", "partition"
};

std::set<std::string> pointerQualifiers =
{
    "uniform", "mutable", "read", "write", "inline", "read_write", "atomic", "volatile"
};

//------------------------------------------------------------------------------
/**
*/
Validator::Validator() 
    : resourceIndexingMode(ResourceIndexingByGroup)
    , defaultGroup(0)
{
    this->allowedReadWriteTextureAttributes.insert(readWriteAccessFlags.begin(), readWriteAccessFlags.end());
    this->allowedReadWriteTextureAttributes.insert(readWriteTextureQualifiers.begin(), readWriteTextureQualifiers.end());

    // add formats
    for (auto it : Variable::stringToFormats)
    {
        this->allowedReadWriteTextureAttributes.insert(it.first);
    }

    this->allowedTextureAttributes.insert(textureQualifiers.begin(), textureQualifiers.end());
    this->allowedScalarAttributes.insert(scalarQualifiers.begin(), scalarQualifiers.end());
    this->allowedSamplerAttributes.insert(samplerQualifiers.begin(), samplerQualifiers.end());
    this->allowedConstantBufferAttributes.insert(constantBufferQualifiers.begin(), constantBufferQualifiers.end());
    this->allowedStorageBufferAttributes.insert(storageBufferAccessFlags.begin(), storageBufferAccessFlags.end());
    this->allowedStorageBufferAttributes.insert(storageBufferQualifiers.begin(), storageBufferQualifiers.end());
    this->allowedPointerAttributes.insert(pointerQualifiers.begin(), pointerQualifiers.end());
    this->allowedPointerAttributes.insert(readWriteAccessFlags.begin(), readWriteAccessFlags.end());

    this->allowedFunctionAttributes.insert(functionAttributes.begin(), functionAttributes.end());

    this->allowedParameterAttributes.insert(parameterQualifiers.begin(), parameterQualifiers.end());
    this->allowedParameterAttributes.insert(parameterAccessFlags.begin(), parameterAccessFlags.end());
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
        return this->ResolveEnumeration(compiler, symbol);
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

        if (attr.name == "shader")
        {
            funResolved->isShader = true;
        }
        else if (attr.name == "local_size_x")
            if (!attr.expression->EvalUInt(funResolved->computeShaderWorkGroupSize[0]))
            {
                compiler->Error(Format("Value '%s' has to be a compile time uint constant", attr.name.c_str()), symbol);
                return false;
            }
        else if (attr.name == "local_size_y")
            if (!attr.expression->EvalUInt(funResolved->computeShaderWorkGroupSize[1]))
            {
                compiler->Error(Format("Value '%s' has to be a compile time uint constant", attr.name.c_str()), symbol);
                return false;
            }
        else if (attr.name == "local_size_z")
            if (!attr.expression->EvalUInt(funResolved->computeShaderWorkGroupSize[2]))
            {
                compiler->Error(Format("Value '%s' has to be a compile time uint constant", attr.name.c_str()), symbol);
                return false;
            }
        else if (attr.name == "early_depth")
            funResolved->earlyDepth = true;
        else if (attr.name == "invocations")
            if (!attr.expression->EvalUInt(funResolved->invocations))
            {
                compiler->Error(Format("Value '%s' has to be a compile time uint constant", attr.name.c_str()), symbol);
                return false;
            }
        else if (attr.name == "max_output_vertices")
            if (!attr.expression->EvalUInt(funResolved->maxOutputVertices))
            {
                compiler->Error(Format("Value '%s' has to be a compile time uint constant", attr.name.c_str()), symbol);
                return false;
            }
        else if (attr.name == "patch_size")
            if (!attr.expression->EvalUInt(funResolved->patchSize))
            {
                compiler->Error(Format("Value '%s' has to be a compile time uint constant", attr.name.c_str()), symbol);
                return false;
            }
        else if (attr.name == "winding")
        {
            std::string str = attr.expression->EvalString();
            funResolved->windingOrder = Function::__Resolved::WindingOrderFromString(str);
            if (funResolved->windingOrder == Function::__Resolved::InvalidWindingOrder)
            {
                compiler->Error(Format("Attribute 'winding' supports values: cw/clockwise, ccw/counter_clockwise, but got '%s'", str.c_str()), symbol);
                return false;
            }
        }
        else if (attr.name == "input_topology")
        {
            std::string str = attr.expression->EvalString();
            funResolved->inputPrimitiveTopology = Function::__Resolved::PrimitiveTopologyFromString(str);
            if (funResolved->inputPrimitiveTopology == Function::__Resolved::InvalidPrimitiveTopology)
            {
                compiler->Error(Format("Attribute 'input_topology' supports values: points, lines, lines_adjacency, triangles, triangles_adjacency, but got '%s'", str.c_str()), symbol);
                return false;
            }
        }
        else if (attr.name == "output_topology")
        {
            std::string str = attr.expression->EvalString();
            funResolved->outputPrimitiveTopology = Function::__Resolved::PrimitiveTopologyFromString(str);
            if (funResolved->outputPrimitiveTopology == Function::__Resolved::InvalidPrimitiveTopology 
                || funResolved->outputPrimitiveTopology == Function::__Resolved::LinesAdjacency
                || funResolved->outputPrimitiveTopology == Function::__Resolved::TrianglesAdjacency)
            {
                compiler->Error(Format("Attribute 'output_topology' supports values: points, lines, triangles, but got '%s'", str.c_str()), symbol);
                return false;
            }
        }
        else if (attr.name == "patch_type")
        {
            std::string str = attr.expression->EvalString();
            funResolved->patchType = Function::__Resolved::PatchTypeFromString(str);
            if (funResolved->patchType == Function::__Resolved::InvalidPatchType)
            {
                compiler->Error(Format("Attribute 'patch_type' supports values: isolines, triangles, quads, but got '%s'", str.c_str()), symbol);
                return false;
            }
        }
        else if (attr.name == "partition")
        {
            std::string str = attr.expression->EvalString();
            funResolved->partitionMethod = Function::__Resolved::PartitionMethodFromString(str);
            if (funResolved->partitionMethod == Function::__Resolved::InvalidPartitionMethod)
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
            funResolved->pixelOrigin = Function::__Resolved::PixelOriginFromString(str);
            if (funResolved->pixelOrigin == Function::__Resolved::InvalidPixelOrigin)
            {
                compiler->Error(Format("Attribute 'pixel_origin' supports values: lower/lower_left, upper/upper_left, center, but got '%s'", str.c_str()), symbol);
                return false;
            }
        }
    }

    // validate attributes
    if (!funResolved->isShader)
    {
        if (funResolved->earlyDepth)
        {
            compiler->Error("'early_depth' is only allowed on functions with the 'shader' qualifier", symbol);
            return false;
        }

        if (
            funResolved->computeShaderWorkGroupSize[0] > 1
            || funResolved->computeShaderWorkGroupSize[1] > 1
            || funResolved->computeShaderWorkGroupSize[2] > 1
            )
        {
            compiler->Error("'local_size_(x/y/z)' is only allowed on functions with the 'shader' qualifier", symbol);
            return false;
        }
    }

    // Push temporary scope to evaluate variables
    compiler->PushScope(Compiler::Scope::ScopeType::Local, fun);

    // run validation on parameters
    for (Variable* var : fun->parameters)
    {
        Variable::__Resolved* varResolved = Symbol::Resolved(var);
        varResolved->usageBits.flags.isParameter = !funResolved->isShader;
        varResolved->usageBits.flags.isShaderParameter = funResolved->isShader;
        this->ResolveVariable(compiler, var);
    }

    compiler->PopScope();

    // setup our variables and attributes as sets
    std::string paramList;
    for (Variable* var : fun->parameters)
    {
        // add comma if not first argument
        Variable::__Resolved* varResolved = Symbol::Resolved(var);
        if (var != fun->parameters.front())
            paramList.append(",");

        // finally add type
        paramList.append(varResolved->type.ToString());
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
    if (!compiler->AddSymbol(funResolved->name, fun, true))
        return false;

    // also add the signature for type lookup
    if (!compiler->AddSymbol(funResolved->signature, fun, false))
        return false;

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

    // validate function body
    if (
        fun->ast != nullptr
        && !this->ResolveStatement(compiler, fun->ast)
        )
    {
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
        if (!entry->Resolve(compiler))
            return false;

        const BinaryExpression* assignEntry = static_cast<const BinaryExpression*>(entry);
        if (entry->symbolType != Symbol::BinaryExpressionType || assignEntry->left->symbolType == Symbol::ArrayIndexExpressionType)
        {
            compiler->Error(Format("Program entry '%s' must be an assignment expression", assignEntry->EvalString().c_str()), symbol);
            return false;
        }

        // only allow symbol assignments
        if (assignEntry->right->symbolType != Symbol::UnaryExpressionType)
        {
            compiler->Error(Format("Program binds '%s' but it is not an identifier", assignEntry->EvalString().c_str()), symbol);
            return false;
        }

        std::string entry = assignEntry->left->EvalString();
        Program::__Resolved::ProgramEntryType entryType = Program::__Resolved::StringToEntryType(entry);
        if (entryType == Program::__Resolved::InvalidProgramEntryType)
        {
            // get all functions responding to this function
            Function* functionStub = static_cast<Function*>(compiler->GetSymbol(entry));

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
                    progResolved->functionOverrides[functionStub] = func;
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

            progResolved->programMappings[entryType] = value;

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
                    funResolved->shaderUsage.flags.isVertexShader = true;
                    break;
                case Program::__Resolved::ProgramEntryType::HullShader:
                    funResolved->shaderUsage.flags.isHullShader = true;
                    break;
                case Program::__Resolved::ProgramEntryType::DomainShader:
                    funResolved->shaderUsage.flags.isDomainShader = true;
                    break;
                case Program::__Resolved::ProgramEntryType::GeometryShader:
                    funResolved->shaderUsage.flags.isGeometryShader = true;
                    break;
                case Program::__Resolved::ProgramEntryType::PixelShader:
                    funResolved->shaderUsage.flags.isPixelShader = true;
                    break;
                case Program::__Resolved::ProgramEntryType::ComputeShader:
                    funResolved->shaderUsage.flags.isComputeShader = true;
                    break;
                case Program::__Resolved::ProgramEntryType::TaskShader:
                    funResolved->shaderUsage.flags.isTaskShader = true;
                    break;
                case Program::__Resolved::ProgramEntryType::MeshShader:
                    funResolved->shaderUsage.flags.isMeshShader = true;
                    break;
                case Program::__Resolved::ProgramEntryType::RayGenerationShader:
                    funResolved->shaderUsage.flags.isRayGenerationShader = true;
                    break;
                case Program::__Resolved::ProgramEntryType::RayMissShader:
                    funResolved->shaderUsage.flags.isRayMissShader = true;
                    break;
                case Program::__Resolved::ProgramEntryType::RayClosestHitShader:
                    funResolved->shaderUsage.flags.isRayClosestHitShader = true;
                    break;
                case Program::__Resolved::ProgramEntryType::RayAnyHitShader:
                    funResolved->shaderUsage.flags.isRayAnyHitShader = true;
                    break;
                case Program::__Resolved::ProgramEntryType::RayIntersectionShader:
                    funResolved->shaderUsage.flags.isRayIntersectionShader = true;
                    break;
                case Program::__Resolved::ProgramEntryType::RayCallableShader:
                    funResolved->shaderUsage.flags.isRayCallableShader = true;
                    break;
                }

                // when we've set these flags, run function validation to make sure it's properly formatted
                if (!this->ValidateFunction(compiler, fun))
                    return false;
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
            if (attr.name == "group")
                if (!attr.expression->EvalUInt(strucResolved->group))
                {
                    compiler->Error(Format("group qualifier has to evaluate to a compile time uint", attr.name.c_str()), symbol);
                    return false;
                }
            else if (attr.name == "binding")
                if (!attr.expression->EvalUInt(strucResolved->binding))
                {
                    compiler->Error(Format("binding qualifier has to evaluate to a compile time uint", attr.name.c_str()), symbol);
                    return false;
                }
            else if (attr.name == "push")
            {
                strucResolved->usageFlags.flags.isPush = true;
            }
        }
    }

    // push scope for struct but not for storage and constant buffers
    Compiler::LocalScope scope = Compiler::LocalScope::MakeTypeScope(compiler, struc);

    // validate members
    for (Symbol* sym : struc->symbols)
    {
        if (sym->symbolType == Symbol::VariableType)
        {
            Variable* var = static_cast<Variable*>(sym);
            Variable::__Resolved* varResolved = Symbol::Resolved(var);
            varResolved->usageBits.flags.isStructMember = true;
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
Validator::ResolveEnumeration(Compiler* compiler, Symbol* symbol)
{
    Enumeration* enumeration = static_cast<Enumeration*>(symbol);
    Enumeration::__Resolved* enumResolved = Symbol::Resolved(enumeration);

    if (!compiler->AddSymbol(enumeration->name, enumeration))
        return false;

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

        // Resolve value
        if (expr != nullptr)
        {
            if (!expr->Resolve(compiler))
                return false;

            Type::FullType type;
            expr->EvalType(type);
            if (type.name == "u32" || type.name == "i32" || type.name == "u16" || type.name == "i16")
            {
                uint32_t value;
                expr->EvalUInt(value);
                sym = new EnumExpression(value, { enumeration->name });
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
            sym = new EnumExpression(nextValue++, { enumeration->name });
        }

        // Add to type
        sym->name = str;
        enumeration->symbols.push_back(sym);
        enumeration->lookup.insert({ sym->name, sym });

        // Also add to the scope where the enum is declared, as by the enum C rules
        if (!compiler->AddSymbol(sym->name, sym))
            return false;
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
    }

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

    // struct members may only be scalars
    if (varResolved->usageBits.flags.isStructMember && 
        (type->category != Type::ScalarCategory && type->category != Type::EnumCategory))
    {
        compiler->Error(Format("Member '%s' may only be scalar type if member of a struct", varResolved->name.c_str()), symbol);
        return false;
    }

    // Add symbol
    if (!compiler->AddSymbol(var->name, var))
        return false;

    // resolve array type and names
    if (var->valueExpression != nullptr)
    {
        Expression* expr = var->valueExpression;
        expr->isDeclaration = true;
        expr->isLhsValue = false;
        expr->Resolve(compiler);
        varResolved->value = var->valueExpression;
    }

    // If struct member, only allow sized arrays and no initializers
    if (varResolved->usageBits.flags.isStructMember)
    {
        if (varResolved->value != nullptr)
        {
            compiler->Error(Format("'struct' members may not have initializers"), symbol);
            return false;
        }

        for (uint32_t size : varResolved->type.modifierValues)
            if (size == 0)
            {
                compiler->Error(Format("'struct' array member can't be of dynamic size"), symbol);
                return false;
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
        if (type->category == Type::ReadWriteTextureCategory)
            allowedAttributesSet = &this->allowedReadWriteTextureAttributes;
        else if (type->category == Type::TextureCategory)
            allowedAttributesSet = &this->allowedTextureAttributes;
        else if (type->category == Type::ScalarCategory)
            allowedAttributesSet = &this->allowedScalarAttributes;
        else if (type->category == Type::SamplerCategory)
            allowedAttributesSet = &this->allowedSamplerAttributes;
        else if (type->category == Type::UserTypeCategory
                && varResolved->type.IsPointer())
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
        }
        else if (attr.name == "const")
            varResolved->usageBits.flags.isConst = true;
        else if (attr.name == "uniform")
        {
            varResolved->usageBits.flags.isUniform = true;
            if (varResolved->usageBits.flags.isMutable)
            {
                compiler->Error(Format("Variable declared as 'uniform' can't also be 'mutable'"), symbol);
                return false;
            }
            if (varResolved->usageBits.flags.isInline)
            {
                compiler->Error(Format("Variable declared as 'uniform' can't also be 'inline'"), symbol);
                return false;
            }
            if (varResolved->usageBits.flags.isGroupShared)
            {
                compiler->Error(Format("Variable declared as 'uniform' can't also be 'workgroup'"), symbol);
                return false;
            }
        }
        else if (attr.name == "inline")
        {
            varResolved->usageBits.flags.isInline = true;
            if (varResolved->usageBits.flags.isMutable)
            {
                compiler->Error(Format("Variable declared as 'inline' can't also be 'mutable'"), symbol);
                return false;
            }
            if (varResolved->usageBits.flags.isUniform)
            {
                compiler->Error(Format("Variable declared as 'inline' can't also be 'uniform'"), symbol);
                return false;
            }
            if (varResolved->usageBits.flags.isGroupShared)
            {
                compiler->Error(Format("Variable declared as 'inline' can't also be 'workgroup'"), symbol);
                return false;
            }
        }
        else if (attr.name == "mutable")
        {
            varResolved->usageBits.flags.isMutable = true;
            varResolved->accessBits.flags.writeAccess = true;
            if (varResolved->usageBits.flags.isUniform)
            {
                compiler->Error(Format("Variable declared as 'mutable' can't also be 'uniform'"), symbol);
                return false;
            }
            if (varResolved->usageBits.flags.isInline)
            {
                compiler->Error(Format("Variable declared as 'mutable' can't also be 'inline'"), symbol);
                return false;
            }
            if (varResolved->usageBits.flags.isGroupShared)
            {
                compiler->Error(Format("Variable declared as 'mutable' can't also be 'workgroup'"), symbol);
                return false;
            }

        }
        else if (attr.name == "workgroup")
        {
            varResolved->usageBits.flags.isGroupShared = true;
            if (varResolved->usageBits.flags.isMutable)
            {
                compiler->Error(Format("Variable declared as 'workgroup' can't also be 'mutable'"), symbol);
                return false;
            }
            if (varResolved->usageBits.flags.isInline)
            {
                compiler->Error(Format("Variable declared as 'workgroup' can't also be 'inline'"), symbol);
                return false;
            }
            if (varResolved->usageBits.flags.isUniform)
            {
                compiler->Error(Format("Variable declared as 'workgroup' can't also be 'uniform'"), symbol);
                return false;
            }
        }
        else
        {
            // more complicated lookups
            if (set_contains(readWriteAccessFlags, attr.name))
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
                varResolved->parameterBits.flags.isIn = true;
            else if (attr.name == "out")
                varResolved->parameterBits.flags.isOut = true;
            else if (attr.name == "in_out")
            {
                if (varResolved->usageBits.flags.isShaderParameter)
                {
                    compiler->Error(Format("in_out is not supported on shader arguments"), symbol);
                    return false;
                }
                varResolved->parameterBits.flags.isIn = varResolved->parameterBits.flags.isOut = true;
            }
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
        if (map_contains(Variable::stringToFormats, attr.name))
        {
            varResolved->imageFormat = Variable::stringToFormats[attr.name];
        }
    }

    if (varResolved->usageBits.flags.isMutable)
    {
        if (varResolved->usageBits.flags.isUniform)
        {
            compiler->Error(Format("Variable declared as 'uniform' can't also be 'mutable'"), symbol);
            return false;
        }
    }

    if (compiler->IsScopeGlobal())
    {
        if (type->category != Type::ScalarCategory)
        {
            uint16_t numArrays = 0;
            uint16_t numPointers = 0;
            for (Type::FullType::Modifier mod : varResolved->type.modifiers)
            {
                if (mod == Type::FullType::Modifier::ArrayLevel)
                    numArrays++;
                else if (mod == Type::FullType::Modifier::PointerLevel)
                    numPointers++;
            }
            if (numArrays > 1)
            {
                compiler->Error(Format("Global variables may only be single array"), symbol);
                return false;
            }
            if (numPointers > 1)
            {
                compiler->Error(Format("Global variables may only be single pointer"), symbol);
                return false;
            }

            if (type->category == Type::ReadWriteTextureCategory
                && !varResolved->type.IsPointer())
            {
                compiler->Error(Format("Instance of 'readWriteTexture' must be pointer if in global scope"), symbol);
                return false;
            }            
            else if (type->category == Type::TextureCategory
                     && !varResolved->type.IsPointer())
            {
                compiler->Error(Format("Instance of 'texture' must be pointer if in global scope"), symbol);
                return false;
            }
            else if (type->category == Type::SamplerCategory
                     && !varResolved->type.IsPointer())
            {
                compiler->Error(Format("Instance of 'sampler' must be pointer if in global scope"), symbol);
                return false;
            }
            else if (type->category == Type::UserTypeCategory
                     && !varResolved->type.IsPointer())
            {
                compiler->Error(Format("Instance of '%s' must be pointer if in global scope", type->name.c_str()), symbol);
                return false;
            }
        }
        else // scalar
        {
            if (!varResolved->usageBits.flags.isConst)
            {
                compiler->Error(Format("Non-const scalars are not allowed in global scope"), symbol);
                return false;
            }
            else if (varResolved->usageBits.flags.isConst && varResolved->value == nullptr)
            {
                // check for variable initialization criteria
                compiler->Error(Format("Variable declared as const but is never initialized"), symbol);
                return false;
            }
        }
    }
    else
    {
        if (varResolved->type.IsPointer() && !varResolved->usageBits.flags.isParameter)
        {
            compiler->Error(Format("Pointers are only allowed as symbols in the global scope", type->name.c_str()), symbol);
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

        Type* lhsType = compiler->GetSymbol<Type>(lhs.name);
        std::vector<Symbol*> assignmentOperators = lhsType->GetSymbols(Format("operator=(%s)", rhs.name.c_str()));
        Symbol* fun = Function::MatchOverload(compiler, assignmentOperators, { rhs });
        if (lhs != rhs
            && fun == nullptr)
        {
            compiler->Error(Format("Type '%s' can't be converted to '%s'", lhs.ToString().c_str(), rhs.ToString().c_str()), symbol);
            return false;
        }

        // Okay, so now when we're done, we'll copy over the modifier values from rhs to lhs
        varResolved->type.modifierValues = rhs.modifierValues;
    }

    // check if image formats have been resolved
    if (type->category == Type::ReadWriteTextureCategory 
        && varResolved->imageFormat == Variable::InvalidImageFormat
        && !varResolved->usageBits.flags.isParameter)
    {
        compiler->Error(Format("readWriteTexture variable '%s' must provide a format qualifier", varResolved->name.c_str()), var);
        return false;
    }

    if (varResolved->usageBits.flags.isParameter)
    {
        // if parameter, resolve in and out bindings
        if (varResolved->binding == Variable::__Resolved::NOT_BOUND)
        {
            if (varResolved->parameterBits.flags.isIn)
                varResolved->inBinding = this->inParameterIndexCounter++;
            if (varResolved->parameterBits.flags.isOut)
                varResolved->outBinding = this->outParameterIndexCounter++;
        }
        else
        {
            varResolved->inBinding = varResolved->binding;
            varResolved->outBinding = varResolved->binding;
            if (varResolved->parameterBits.flags.isIn)
                this->inParameterIndexCounter = varResolved->binding + 1;
            if (varResolved->parameterBits.flags.isOut)
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
                if (varResolved->usageBits.flags.isMutable)
                    cat = Type::Category::MutableStructureCategory;
                else if (varResolved->usageBits.flags.isUniform)
                    cat = Type::Category::UniformStructureCategory;

                if (!varResolved->type.IsPointer())
                {
                    compiler->Error(Format("Variable of '%s' with qualifier 'uniform' or 'mutable' must be pointer", type->name.c_str()), var);
                    return false;
                }
                else if (cat == Type::Category::UserTypeCategory)
                {
                    compiler->Error(Format("Variable of '%s' must be either 'uniform' or 'mutable'", type->name.c_str()), var);
                    return false;
                }
            }
            else
            {
                if (varResolved->usageBits.flags.isMutable || varResolved->usageBits.flags.isUniform)
                {
                    compiler->Error(Format("Variable of '%s' with qualifier 'uniform' or 'mutable' are only allowed in the global scope", type->name.c_str()), var);
                    return false;
                }
            }
        }

        // if not a parameter, assume resource (these types can't be declared inside functions)
        if (cat == Type::Category::ReadWriteTextureCategory
            || cat == Type::Category::TextureCategory
            || cat == Type::Category::SamplerCategory
            || cat == Type::Category::PixelCacheCategory
            || cat == Type::Category::UniformStructureCategory
            || cat == Type::Category::MutableStructureCategory)
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

        if (cat == Type::Category::MutableStructureCategory
            || cat == Type::Category::UniformStructureCategory)
        {
            // Generate mutable/uniform variant of struct
            Structure* generatedStruct = new Structure;
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
                    generatedStruct->symbols.push_back(generatedVar);
                    generatedStruct->lookup.insert({ varResolved->name, generatedVar });
                }
            }
            Structure::__Resolved* generatedStructResolved = Symbol::Resolved(generatedStruct);
            generatedStruct->name = varResolved->name;
            generatedStruct->annotations = var->annotations;
            generatedStruct->location = var->location;
            generatedStructResolved->group = varResolved->group;
            generatedStructResolved->binding = varResolved->binding;
            if (cat == Type::Category::MutableStructureCategory)
            {
                generatedStructResolved->usageFlags.flags.isMutableBuffer = true;
                generatedStruct->baseType = TypeCode::ReadWriteBuffer;
            }
            else if (cat == Type::Category::UniformStructureCategory)
            {
                generatedStructResolved->usageFlags.flags.isUniformBuffer = true;
                generatedStruct->baseType = TypeCode::UniformBuffer;
            }

            // Insert symbol before this one, avoiding resolving (we assume the struct and members are already valid)
            compiler->symbols.insert(compiler->symbols.begin() + compiler->symbolIterator, generatedStruct);
            compiler->symbolIterator++;
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
    switch (symbol->symbolType)
    {
        case Symbol::BreakStatementType:
        {
            auto statement = reinterpret_cast<BreakStatement*>(symbol);
            Symbol* scopeOwner = compiler->GetScopeOwner();
            if (scopeOwner == nullptr
                || !(
                scopeOwner->symbolType == Symbol::ForStatementType
                || scopeOwner->symbolType == Symbol::WhileStatementType
                || scopeOwner->symbolType == Symbol::SwitchStatementType)
                )
            {
                compiler->Error(Format("'break' is only valid inside a for, while or switch statement body"), statement);
                return false;
            }
            return true;
        }
        case Symbol::ContinueStatementType:
        {
            auto statement = reinterpret_cast<ContinueStatement*>(symbol);
            Symbol* scopeOwner = compiler->GetScopeOwner();
            if (scopeOwner == nullptr
                || !(
                scopeOwner->symbolType == Symbol::ForStatementType
                || scopeOwner->symbolType == Symbol::WhileStatementType)
                )
            {
                compiler->Error(Format("'continue' is only valid inside a for or while statement body"), statement);
                return false;
            }
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
                    compiler->Error(Format("For condition must evaluate to a boolean value"), statement);
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
            return statement->condition->Resolve(compiler) 
                && this->ResolveStatement(compiler, statement->ifStatement) 
                && statement->elseStatement == nullptr ? true : this->ResolveStatement(compiler, statement->elseStatement);
        }
        case Symbol::ReturnStatementType:
        {
            auto statement = reinterpret_cast<ReturnStatement*>(symbol);
            Symbol* scopeOwner = compiler->GetScopeOwner();
            if (scopeOwner == nullptr
                || scopeOwner->symbolType != Symbol::FunctionType)
            {
                compiler->Error(Format("'return' is only valid inside function body"), statement);
                return false;
            }

            if (statement->returnValue != nullptr)
                return statement->returnValue->Resolve(compiler);
            else 
                return true;
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

            for (Symbol* sym : statement->caseStatements)
            {
                if (!this->ResolveStatement(compiler, sym))
                {
                    return false;
                }
            }
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
            return this->ResolveStatement(compiler, statement->defaultStatement);
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
            if (varResolved->parameterBits.flags.isIn
                && varResolved->parameterBits.flags.isOut)
            {
                compiler->Error(Format("Parameter '%s' can not be both 'in' and 'out' or 'in_out' if function '%s' is used as a shader", var->name.c_str(), fun->name.c_str()), var);
                return false;
            }

            if (varResolved->parameterBits.flags.isPatch
                && !(funResolved->shaderUsage.flags.isHullShader || funResolved->shaderUsage.flags.isDomainShader))
            {
                compiler->Error(Format("Parameter '%s' can not use 'patch' if function is not being used as a HullShader/TessellationControlShader or DomainShader/TessellationEvaluationShader", var->name.c_str(), fun->name.c_str()), var);
                return false;
            }

            if (varResolved->parameterBits.flags.isNoInterpolate
                && !funResolved->shaderUsage.flags.isPixelShader)
            {
                compiler->Error(Format("Parameter '%s' can not use 'no_interpolate' if function is not being used as a PixelShader", var->name.c_str(), fun->name.c_str()), var);
                return false;
            }

            if (varResolved->parameterBits.flags.isNoPerspective
                && !funResolved->shaderUsage.flags.isPixelShader)
            {
                compiler->Error(Format("Parameter '%s' can not use 'no_perspective' if function is not being used as a PixelShader", var->name.c_str(), fun->name.c_str()), var);
                return false;
            }
        }
    }

    // validate function attribute validity
    if (funResolved->shaderUsage.flags.isPixelShader)
    {
        uint32_t numOutputs = 0;
        for (Variable* var : fun->parameters)
        {
            Variable::__Resolved* varResolved = Symbol::Resolved(var);
            if (varResolved->parameterBits.flags.isOut)
                numOutputs++;
        }
        if (numOutputs == 0)
            compiler->Warning(Format("Function '%s' is used as pixel shader but produces no color output", fun->name.c_str()), symbol);
    }
    else
    {
        if (funResolved->earlyDepth)
            compiler->Warning(Format("Function '%s' has attribute 'early_depth' but is not used as a pixel shader", fun->name.c_str()), symbol);
    }

    if (funResolved->shaderUsage.flags.isHullShader)
    {
        if (funResolved->patchSize == Function::__Resolved::INVALID_SIZE)
        {
            compiler->Error(Format("Hull shader '%s' is hull/tessellation control shader but does not define 'patch_size'", fun->name.c_str()), symbol);
            return false;
        }
    }
    else
    {
        if (funResolved->patchSize != Function::__Resolved::INVALID_SIZE)
            compiler->Warning(Format("Function '%s' has attribute 'patch_size' but is not used as a HullShader/TessellationControlShader", fun->name.c_str()), symbol);
    }

    if (funResolved->shaderUsage.flags.isDomainShader)
    {
        // validate required qualifiers
        if (funResolved->patchType != Function::__Resolved::InvalidPatchType)
        {
            compiler->Warning(Format("Domain shader '%s' does not define 'patch_type' for DomainShader/TessellationEvaluationShader, defaulting to 'triangles'", fun->name.c_str()), symbol);
            funResolved->patchType = Function::__Resolved::PatchType::TrianglePatch;
        }
        if (funResolved->partitionMethod != Function::__Resolved::InvalidPartitionMethod)
        {
            compiler->Warning(Format("Domain shader '%s' does not define 'partition', defaulting to 'steps'", fun->name.c_str()), symbol);
            funResolved->partitionMethod = Function::__Resolved::PartitionMethod::IntegerSteps;
        }
    }
    else
    {
        if (funResolved->patchType != Function::__Resolved::InvalidPatchType)
            compiler->Warning(Format("Function '%s' has attribute 'patch_type' but is not used as a DomainShader/TessellationEvaluationShader", fun->name.c_str()), symbol);
        if (funResolved->partitionMethod != Function::__Resolved::InvalidPartitionMethod)
            compiler->Warning(Format("Function '%s' has attribute 'partition' but is not used as a DomainShader/TessellationEvaluationShader", fun->name.c_str()), symbol);
    }

    if (funResolved->shaderUsage.flags.isGeometryShader)
    {
        if (funResolved->maxOutputVertices == Function::__Resolved::INVALID_SIZE)
        {
            compiler->Error(Format("Geometry shader '%s' does not define 'max_output_vertices' for GeometryShader", fun->name.c_str()), symbol);
            return false;
        }
        if (funResolved->inputPrimitiveTopology == Function::__Resolved::InvalidPrimitiveTopology)
        {
            compiler->Warning(Format("Geometry shader '%s' does not define 'input_topology' for GeometryShader, defaulting to 'triangles'", fun->name.c_str()), symbol);
            funResolved->inputPrimitiveTopology = Function::__Resolved::Triangles;
        }
        if (funResolved->outputPrimitiveTopology == Function::__Resolved::InvalidPrimitiveTopology)
        {
            compiler->Warning(Format("Geometry shader '%s' does not define 'output_topology' for GeometryShader, defaulting to 'triangles'", fun->name.c_str()), symbol);
            funResolved->outputPrimitiveTopology = Function::__Resolved::Triangles;
        }
    }
    else
    {
        if (funResolved->invocations != Function::__Resolved::INVALID_SIZE)
            compiler->Warning(Format("Function '%s' has attribute 'invocations' but is not used as a GeometryShader", fun->name.c_str()), symbol);
        if (funResolved->maxOutputVertices != Function::__Resolved::INVALID_SIZE)
            compiler->Warning(Format("Function '%s' has attribute 'max_output_vertices' but is not used as a GeometryShader", fun->name.c_str()), symbol);
        if (funResolved->inputPrimitiveTopology != Function::__Resolved::InvalidPrimitiveTopology)
            compiler->Warning(Format("Function '%s' has attribute 'input_topology' but is not used as a GeometryShader", fun->name.c_str()), symbol);
        if (funResolved->outputPrimitiveTopology != Function::__Resolved::InvalidPrimitiveTopology)
            compiler->Warning(Format("Function '%s' has attribute 'output_topology' but is not used as a GeometryShader", fun->name.c_str()), symbol);
    }

    if (funResolved->shaderUsage.flags.isComputeShader)
    {
        if (funResolved->computeShaderWorkGroupSize[0] <= 0)
        {
            compiler->Error(Format("Compute shader must declare 'local_size_x' bigger than or equal to 1", fun->name.c_str()), symbol);
            return false;
        }
        if (funResolved->computeShaderWorkGroupSize[1] <= 0)
        {
            compiler->Error(Format("Compute shader must declare 'local_size_y' bigger than or equal to 1", fun->name.c_str()), symbol);
            return false;
        }
        if (funResolved->computeShaderWorkGroupSize[2] <= 0)
        {
            compiler->Error(Format("Compute shader must declare 'local_size_z' bigger than or equal to 1", fun->name.c_str()), symbol);
            return false;
        }
    }
    else
    {
        if (funResolved->computeShaderWorkGroupSize[0] > 1)
            compiler->Warning(Format("Function '%s' has attribute 'local_size_x' but is not used as a compute shader", fun->name.c_str()), symbol);
        if (funResolved->computeShaderWorkGroupSize[1] > 1)
            compiler->Warning(Format("Function '%s' has attribute 'local_size_y' but is not used as a compute shader", fun->name.c_str()), symbol);
        if (funResolved->computeShaderWorkGroupSize[2] > 1)
            compiler->Warning(Format("Function '%s' has attribute 'local_size_z' but is not used as a compute shader", fun->name.c_str()), symbol);
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
        if (varResolved->parameterBits.flags.isIn && in)
            ret.push_back(var);
        else if (varResolved->parameterBits.flags.isOut && !in)
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

    enum GraphicsProgramTristate
    {
        NotSet,
        IsGraphics,
        IsCompute
    };
    GraphicsProgramTristate programType = GraphicsProgramTristate::NotSet;

    // validate program setup as compute or graphics program, do it on a first-come-first-serve basis
    for (auto& it : progResolved->programMappings)
    {
        if (it.first == Program::__Resolved::ComputeShader)
        {
            if (programType == GraphicsProgramTristate::IsGraphics)
            {
                compiler->Error(Format("Invalid program setup, program already used as a graphics program, ComputeShader is not allowed"), symbol);
                return false;
            }
            programType = GraphicsProgramTristate::IsCompute;
        }
        else if (it.first == Program::__Resolved::VertexShader
            || it.first == Program::__Resolved::HullShader
            || it.first == Program::__Resolved::DomainShader
            || it.first == Program::__Resolved::GeometryShader
            || it.first == Program::__Resolved::PixelShader
            || it.first == Program::__Resolved::TaskShader
            || it.first == Program::__Resolved::MeshShader
            || it.first == Program::__Resolved::RayAnyHitShader
            || it.first == Program::__Resolved::RayCallableShader
            || it.first == Program::__Resolved::RayIntersectionShader
            || it.first == Program::__Resolved::RayMissShader
            || it.first == Program::__Resolved::RenderState
            )
        {
            if (programType == GraphicsProgramTristate::IsCompute)
            {
                compiler->Error(Format("Invalid program setup, program already used with ComputeShader but '%s' was provided", Program::__Resolved::EntryTypeToString(it.first)), symbol);
                return false;
            }
            programType = GraphicsProgramTristate::IsGraphics;
        }
    }

    // if a part of the graphics pipeline, go through the different stages and validate them
    if (!progResolved->usage.flags.hasComputeShader)
    {
        Function* lastPrimitiveShader = nullptr;
        if (progResolved->usage.flags.hasVertexShader)
        {
            Function* vs = static_cast<Function*>(progResolved->programMappings[Program::__Resolved::VertexShader]);
            lastPrimitiveShader = vs;
        }

        if (progResolved->usage.flags.hasHullShader)
        {
            if (lastPrimitiveShader == nullptr)
            {
                compiler->Error(Format("Invalid program setup, HullShader/TessellationControlShader needs a VertexShader"), symbol);
                return false;
            }
            Function* hs = static_cast<Function*>(progResolved->programMappings[Program::__Resolved::HullShader]);
            if (!ValidateParameterSets(compiler, lastPrimitiveShader, hs))
                return false;

            lastPrimitiveShader = hs;
        }

        if (progResolved->usage.flags.hasDomainShader)
        {
            if (lastPrimitiveShader == nullptr 
                && map_contains(progResolved->programMappings, Program::__Resolved::HullShader))
            {
                compiler->Error(Format("Invalid program setup, DomainShader needs a HullShader/TessellationControlShader"), symbol);
                return false;
            }
            Function* ds = static_cast<Function*>(progResolved->programMappings[Program::__Resolved::DomainShader]);
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
            Function* gs = static_cast<Function*>(progResolved->programMappings[Program::__Resolved::GeometryShader]);
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
            Function* ps = static_cast<Function*>(progResolved->programMappings[Program::__Resolved::PixelShader]);
            if (!ValidateParameterSets(compiler, lastPrimitiveShader, ps))
                return false;
        }
    }
    return true;
}

} // namespace GPULang
