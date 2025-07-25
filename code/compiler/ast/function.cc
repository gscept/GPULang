//------------------------------------------------------------------------------
//  @file function.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "function.h"
#include "util.h"
#include "variable.h"
#include "types/type.h"
#include "compiler.h"
#include "intrinsics.h"
#include "ast/expressions/symbolexpression.h"
#include "ast/expressions/unaryexpression.h"
#include "ast/expressions/uintexpression.h"
#include "generated/types.h"

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Function::Function()
{
    this->symbolType = FunctionType;
    this->resolved = &this->functionResolved;
    this->hasBody = false;
    this->compileTime = false;
    this->ast = nullptr;

    Function::__Resolved* thisResolved = static_cast<Function::__Resolved*>(this->resolved);
    thisResolved->isEntryPoint = false;
    thisResolved->executionModifiers.invocations = Function::__Resolved::INVALID_SIZE;
    thisResolved->executionModifiers.maxOutputVertices = Function::__Resolved::INVALID_SIZE;
    thisResolved->executionModifiers.windingOrder = Function::__Resolved::InvalidWindingOrder;
    thisResolved->executionModifiers.inputPrimitiveTopology = Function::__Resolved::InvalidPrimitiveTopology;
    thisResolved->executionModifiers.outputPrimitiveTopology = Function::__Resolved::InvalidPrimitiveTopology;
    thisResolved->executionModifiers.patchType = Function::__Resolved::InvalidPatchType;
    thisResolved->executionModifiers.partitionMethod = Function::__Resolved::InvalidPartitionMethod;
    thisResolved->executionModifiers.pixelOrigin = Function::__Resolved::Upper;
    thisResolved->executionModifiers.computeDerivativeIndexing = Function::__Resolved::NoDerivatives;
    thisResolved->executionModifiers.computeShaderWorkGroupSize[0] = 1;
    thisResolved->executionModifiers.computeShaderWorkGroupSize[1] = 1;
    thisResolved->executionModifiers.computeShaderWorkGroupSize[2] = 1;
    thisResolved->executionModifiers.groupSize = 64;
    thisResolved->executionModifiers.groupsPerWorkgroup = 1;
    thisResolved->executionModifiers.earlyDepth = false;
    thisResolved->executionModifiers.depthAlwaysGreater = false;
    thisResolved->executionModifiers.depthAlwaysLesser = false;
    thisResolved->isPrototype = false;
    thisResolved->scope.type = Scope::ScopeType::Local;
    thisResolved->scope.owningSymbol = this;
}

//------------------------------------------------------------------------------
/**
*/
Function::~Function()
{
    this->CleanupAnnotations();
    this->CleanupAttributes();
    for (auto var : this->parameters)
        var->~Variable();
    if (this->ast != nullptr)
        this->ast->~Statement();

    for (auto modifier : this->returnType.modifierValues)
        if (modifier != nullptr)
            modifier->~Expression();
}

//------------------------------------------------------------------------------
/**
*/
Symbol*
Function::MatchOverload(Compiler* compiler, const std::vector<Symbol*>& functions, const std::vector<Type::FullType>& args, bool allowImplicitConversion)
{
    Symbol* ret = nullptr;
    for (Symbol* sym : functions)
    {
        // presume nothing
        if (sym->symbolType != SymbolType::FunctionType)
            continue;

        Function* fun = static_cast<Function*>(sym);
        if (fun->parameters.size != args.size())
            continue;

        ret = sym;
        for (size_t i = 0; i < args.size() && ret != nullptr; i++)
        {
            Variable* param = fun->parameters.buf[i];
            Variable::__Resolved* paramResolved = Symbol::Resolved(param);
            if (args[i] != param->type)
            {
                if (allowImplicitConversion)
                {
                    // types don't match, check if there is a constructor for this argument which matches the input argument
                    Type* argType = compiler->GetType(fun->parameters.buf[i]->type);
                    std::vector<Symbol*> conversionMethods = argType->GetSymbols(argType->name);

                    ret = nullptr;
                    for (size_t j = 0; j < conversionMethods.size(); j++)
                    {
                        Function* conv = static_cast<Function*>(conversionMethods[j]);
                        if (conv->symbolType != SymbolType::FunctionType)
                            continue;

                        if (conv->parameters.size != 1)
                            continue;

                        if (conv->parameters.buf[0]->type == args[i])
                        {
                            // okay, conversion found so move to next argument in outer loop
                            ret = sym;
                            break;
                        }
                    }
                }
                else
                {
                    ret = nullptr;
                }
            }
        }

        // if ret kept itself from being unset, return it as it's a match
        if (ret != nullptr)
            return ret;
    }
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
void
Function::SetupIntrinsics()
{
    SYMBOL_STATIC_ALLOC = true;
    
    //------------------------------------------------------------------------------
    /**
        Raytracing intrinsics
    */
    //------------------------------------------------------------------------------
    //__MAKE_BUILTIN(rayTrace, TraceRay);
    //__ADD_HANDLE_ARG_LIT(tlas, AccelerationStructureType);
    //__ADD_ARG_LIT(origin, Float32x3);
    //__ADD_ARG_LIT(direction, Float32x3);
    //__ADD_ARG_LIT(min, Float32);
    //__ADD_ARG_LIT(max, Float32);
    //__ADD_ARG_LIT(shaderTableOffset, UInt32);
    //__ADD_ARG_LIT(shaderTableStride, UInt32);
    //__ADD_ARG_LIT(missShaderIndex, UInt32);
    //__ADD_ARG_LIT(flags, UInt32);
    //__ADD_ARG_LIT(mask, UInt32);
    //__SET_RET_LIT(Void)
//
    //__MAKE_BUILTIN(rayExportIntersection, ExportRayIntersection);
    //__SET_RET_LIT(Void);
//
    //__MAKE_BUILTIN(rayExecuteCallable, ExecuteCallable);
    //__SET_RET_LIT(Void);
//
    //__MAKE_BUILTIN(rayGetLaunchIndex, RayLaunchIndex);
    //__SET_RET_LIT(UInt32x3);
//
    //__MAKE_BUILTIN(rayGetLaunchSize, RayLaunchSize);
    //__SET_RET_LIT(UInt32x3);
//
    //__MAKE_BUILTIN(BLASGetPrimitiveIndex, BLASPrimitiveIndex);
    //__SET_RET_LIT(UInt32);
//
    //__MAKE_BUILTIN(BLASGetGeometryIndex, BLASGeometryIndex);
    //__SET_RET_LIT(UInt32);
//
    //__MAKE_BUILTIN(TLASGetInstanceIndex, TLASInstanceIndex);
    //__SET_RET_LIT(UInt32);
//
    //__MAKE_BUILTIN(TLASGetInstanceCustomIndex, TLASInstanceCustomIndex);
    //__SET_RET_LIT(UInt32);
//
    //__MAKE_BUILTIN(rayGetWorldOrigin, RayWorldOrigin);
    //__SET_RET_LIT(Float32x3);
    //
    //__MAKE_BUILTIN(rayGetWorldDirection, RayWorldDirection);
    //__SET_RET_LIT(Float32x3);
//
    //__MAKE_BUILTIN(rayGetObjectOrigin, RayObjectOrigin);
    //__SET_RET_LIT(Float32x3);
//
    //__MAKE_BUILTIN(rayGetObjectDirection, RayObjectDirection);
    //__SET_RET_LIT(Float32x3);
//
    //__MAKE_BUILTIN(rayGetMin, RayMin);
    //__SET_RET_LIT(Float32);
//
    //__MAKE_BUILTIN(rayGetMax, RayMax);
    //__SET_RET_LIT(Float32);
//
    //__MAKE_BUILTIN(rayGetFlags, RayFlags);
    //__SET_RET_LIT(UInt32);
//
    //__MAKE_BUILTIN(rayGetHitDistance, RayHitDistance);
    //__SET_RET_LIT(Float32);
//
    //__MAKE_BUILTIN(rayGetHitKind, RayHitKind);
    //__SET_RET_LIT(UInt32);
//
    //__MAKE_BUILTIN(TLASGetObjectToWorld, TLASObjectToWorld);
    //__SET_RET_LIT(Float32x4x3);
    //
    //__MAKE_BUILTIN(TLASGetWorldToObject, TLASWorldToObject);
    //__SET_RET_LIT(Float32x4x3);
    
    //DefaultIntrinsics = StaticArray(Intrinsics);

    SYMBOL_STATIC_ALLOC = false;
}

//------------------------------------------------------------------------------
/**
*/
bool 
Function::IsCompatible(Function* otherFunction, bool checkReturnType)
{
    // no match if return types differ
    if (checkReturnType)
    {
        if (otherFunction->returnType != this->returnType)
            return false;
    }

    // no match if amount of parameters differ
    if (otherFunction->parameters.size != this->parameters.size)
        return false;

    // go through parameters
    for (size_t i = 0; i < this->parameters.size; i++)
    {
        Variable* ourParameter = this->parameters[i];
        Variable* otherParameter = otherFunction->parameters[i];

        if (ourParameter->type != otherParameter->type)
            return false;
    }

    return true;
}


//------------------------------------------------------------------------------
/**
*/
void
Function::SetupAsBuiltin(const FixedArray<Variable*>& parameters)
{
    Function::__Resolved* thisResolved = static_cast<Function::__Resolved*>(this->resolved);
    this->parameters = parameters;
}

} // namespace GPULang
