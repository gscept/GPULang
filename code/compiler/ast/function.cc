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

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Function::Function()
{
    this->symbolType = FunctionType;
    if (SYMBOL_STATIC_ALLOC)
        this->resolved = StaticAlloc<Function::__Resolved>();
    else
        this->resolved = Alloc<Function::__Resolved>();
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
    thisResolved->returnValueStorage = Storage::Default;
    thisResolved->scope.type = Scope::ScopeType::Local;
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
        if (fun->parameters.size() != args.size())
            continue;

        ret = sym;
        for (size_t i = 0; i < args.size() && ret != nullptr; i++)
        {
            Variable::__Resolved* paramResolved = Symbol::Resolved(fun->parameters[i]);
            if (args[i] != paramResolved->type)
            {
                if (allowImplicitConversion)
                {
                    // types don't match, check if there is a constructor for this argument which matches the input argument
                    Type* argType = compiler->GetType(fun->parameters[i]->type);
                    std::vector<Symbol*> conversionMethods = argType->GetSymbols(argType->name);

                    ret = nullptr;
                    for (size_t j = 0; j < conversionMethods.size(); j++)
                    {
                        Function* conv = static_cast<Function*>(conversionMethods[j]);
                        if (conv->symbolType != SymbolType::FunctionType)
                            continue;

                        if (conv->parameters.size() != 1)
                            continue;

                        if (conv->parameters[0]->type == args[i])
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

#define __BEGIN_INTRINSICS__ Function* newIntrinsic = nullptr; Variable* newVar = nullptr;

#define __DOCUMENT_INTRINSIC(str)\
newIntrinsic->documentation = str;

#define __MAKE_BUILTIN(nm, opcode)\
{\
newIntrinsic = StaticAlloc<Function>();\
newIntrinsic->name = #nm;\
Intrinsics::opcode = newIntrinsic;\
DefaultIntrinsics.push_back(newIntrinsic);\
}

#define __MAKE_INTRINSIC(nm, opcode, ty)\
{\
newIntrinsic = StaticAlloc<Function>();\
newIntrinsic->name = #nm;\
Intrinsics::opcode##_##ty = newIntrinsic;\
DefaultIntrinsics.push_back(newIntrinsic);\
}

#define __ADD_ARG(nm, tp)\
{\
newVar = StaticAlloc<Variable>();\
newVar->name = #nm;\
newVar->type = Type::FullType(tp);\
newIntrinsic->parameters.push_back(newVar);\
}

#define __ADD_ARG_LIT(nm, tp)\
{\
newVar = StaticAlloc<Variable>();\
newVar->name = #nm;\
newVar->type = Type::FullType(#tp);\
newIntrinsic->parameters.push_back(newVar);\
}

#define __ADD_MUTABLE_ARG_LIT(nm, tp)\
{\
newVar = StaticAlloc<Variable>();\
newVar->name = #nm;\
newVar->type = Type::FullType(#tp);\
newVar->type.mut = true;\
newIntrinsic->parameters.push_back(newVar);\
}

#define __ADD_VALUE_LIT(nm, tp)\
{\
newVar = StaticAlloc<Variable>();\
newVar->name = #nm;\
newVar->type = Type::FullType(#tp);\
newVar->type.literal = true;\
newIntrinsic->parameters.push_back(newVar);\
}

#define __ADD_ARG_ARR_LIT(nm, tp, size)\
{\
newVar = StaticAlloc<Variable>();\
newVar->name = #nm;\
newVar->type = Type::FullType(#tp);\
newVar->type.AddModifier(Type::FullType::Modifier::Array);\
newVar->type.UpdateValue(StaticAlloc<UIntExpression>(size));\
newIntrinsic->parameters.push_back(newVar);\
}

#define __ADD_HANDLE_ARG(nm, tp)\
{\
newVar = StaticAlloc<Variable>();\
newVar->name = #nm;\
newVar->type = Type::FullType(#tp);\
newVar->type.AddModifier(Type::FullType::Modifier::Pointer);\
newVar->attributes.push_back(StaticAlloc<Attribute>("uniform"));\
newIntrinsic->parameters.push_back(newVar);\
}

#define __ADD_HANDLE_ARG_LIT(nm, tp)\
{\
newVar = StaticAlloc<Variable>();\
newVar->name = #nm;\
newVar->type = Type::FullType(#tp);\
newVar->type.AddModifier(Type::FullType::Modifier::Pointer);\
newVar->attributes.push_back(StaticAlloc<Attribute>("uniform"));\
newIntrinsic->parameters.push_back(newVar);\
}

#define __ADD_SAMPLED_HANDLE_ARG_LIT(nm, tp)\
{\
newVar = StaticAlloc<Variable>();\
newVar->name = #nm;\
newVar->type = Type::FullType(#tp);\
newVar->type.AddModifier(Type::FullType::Modifier::Pointer);\
newVar->type.sampled = true;\
newVar->attributes.push_back(StaticAlloc<Attribute>("uniform"));\
newIntrinsic->parameters.push_back(newVar);\
}

#define __ADD_HANDLE_ARG_LIT_MUT(nm, tp)\
{\
newVar = StaticAlloc<Variable>();\
newVar->name = #nm;\
newVar->type = Type::FullType(#tp);\
newVar->type.AddModifier(Type::FullType::Modifier::Pointer);\
newVar->type.mut = true;\
newVar->attributes.push_back(StaticAlloc<Attribute>("uniform"));\
newIntrinsic->parameters.push_back(newVar);\
}

#define __SET_RET_LIT(name)\
{ newIntrinsic->returnType = Type::FullType(#name); }

#define __SET_RET(name)\
{ newIntrinsic->returnType = Type::FullType(name); }

//------------------------------------------------------------------------------
/**
*/
std::vector<Symbol*> DefaultIntrinsics;
void 
Function::SetupIntrinsics()
{
    SYMBOL_STATIC_ALLOC = true;
    __BEGIN_INTRINSICS__;

    std::string scalarArgs[] =
    {
        "f32"
        , "f32x2"
        , "f32x3"
        , "f32x4"
        , "i32"
        , "i32x2"
        , "i32x3"
        , "i32x4"
        , "u32"
        , "u32x2"
        , "u32x3"
        , "u32x4"
        , "b8"
        , "b8x2"
        , "b8x3"
        , "b8x4"
        , "f32x2x2"
        , "f32x2x3"
        , "f32x2x4"
        , "f32x3x2"
        , "f32x3x3"
        , "f32x3x4"
        , "f32x4x2"
        , "f32x4x3"
        , "f32x4x4"
    };
    constexpr uint32_t numScalarArgs = sizeof(scalarArgs) / sizeof(std::string);

#define X(ty, index)\
    __MAKE_INTRINSIC(pow, Pow, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __ADD_ARG(exp, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    FLOAT_LIST
#undef X
    
#define X(ty, index)\
    __MAKE_INTRINSIC(sqrt, Sqrt, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    FLOAT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(invSqrt, InvSqrt, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    FLOAT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(log, Log, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    FLOAT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(log2, Log2, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    FLOAT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(exp, Exp, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    FLOAT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(exp2, Exp2, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    FLOAT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(sin, Sin, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    FLOAT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(cos, Cos, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    FLOAT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(tan, Tan, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    FLOAT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(asin, ASin, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    FLOAT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(acos, ACos, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    FLOAT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(atan, ATan, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    FLOAT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(atan, ATan2, ty)\
    __ADD_ARG(y, scalarArgs[index]);\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    FLOAT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(sinh, SinH, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    FLOAT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(cosh, CosH, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    FLOAT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(tanh, TanH, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    FLOAT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(asinh, ASinH, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    FLOAT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(acosh, ACosH, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    FLOAT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(atanh, ATanH, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    FLOAT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(mad, Mad, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __ADD_ARG(product, scalarArgs[index]);\
    __ADD_ARG(addend, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    SCALAR_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(dot, Dot, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __ADD_ARG(y, scalarArgs[index]);\
    __SET_RET_LIT(f32);

    FLOAT_VEC_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(reflect, Reflect, ty)\
    __ADD_ARG(incident, scalarArgs[index]);\
    __ADD_ARG(normal, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    FLOAT_VEC_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(refract, Refract, ty)\
    __ADD_ARG(incident, scalarArgs[index]);\
    __ADD_ARG(normal, scalarArgs[index]);\
    __ADD_ARG_LIT(eta, f32);\
    __SET_RET(scalarArgs[index]);

    FLOAT_VEC_LIST
#undef X

__MAKE_INTRINSIC(cross, Cross, f32x3)
__ADD_ARG_LIT(v0, f32x3);
__ADD_ARG_LIT(v1, f32x3);
__SET_RET_LIT(f32x3);

#define X(ty, index)\
    __MAKE_INTRINSIC(normalize, Normalize, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    FLOAT_VEC_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(length, Length, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET_LIT(f32);

    FLOAT_VEC_LIST
#undef X

    #define X(ty, index)\
    __MAKE_INTRINSIC(distance, Distance, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __ADD_ARG(y, scalarArgs[index]);\
    __SET_RET_LIT(f32);

    FLOAT_VEC_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(min, Min, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __ADD_ARG(y, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

SCALAR_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(max, Max, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __ADD_ARG(y, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

SCALAR_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(clamp, Clamp, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __ADD_ARG(min, scalarArgs[index]);\
    __ADD_ARG(max, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

SCALAR_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(lerp, Lerp, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __ADD_ARG(y, scalarArgs[index]);\
    __ADD_ARG(control, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    FLOAT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(step, Step, ty)\
    __ADD_ARG(edge, scalarArgs[index]);\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);
    
    FLOAT_LIST  
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(smoothStep, SmoothStep, ty)\
    __ADD_ARG(edge0, scalarArgs[index]);\
    __ADD_ARG(edge1, scalarArgs[index]);\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);
    
    FLOAT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(ceil, Ceil, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

FLOAT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(floor, Floor, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

FLOAT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(fract, Fract, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

FLOAT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(saturate, Saturate, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

FLOAT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(trunc, Truncate, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

FLOAT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(abs, Abs, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    SIGN_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(sign, Sign, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    SIGN_LIST
#undef X
    
#define X(ty, index)\
    __MAKE_INTRINSIC(ddx, DDX, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    FLOAT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(ddy, DDY, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    FLOAT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(fwidth, FWidth, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    FLOAT_LIST
#undef X

    std::string uintTypes[] =
    {
        "u32"
        , "u32x2"
        , "u32x3"
        , "u32x4"
    };

#define X(ty, index)\
    __MAKE_INTRINSIC(castToU32, CastToU32, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(uintTypes[index]);

    FLOAT_LIST
#undef X

    std::string intTypes[] =
{
        "i32"
        , "i32x2"
        , "i32x3"
        , "i32x4"
    };

#define X(ty, index)\
    __MAKE_INTRINSIC(castToI32, CastToI32, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(intTypes[index]);

    FLOAT_LIST
#undef X

    std::string floatTypes[] =
    {
        "f32"
        , "f32x2"
        , "f32x3"
        , "f32x4"
        , "f32"
        , "f32x2"
        , "f32x3"
        , "f32x4"
    };

#define X(ty, index)\
    __MAKE_INTRINSIC(castToF32, CastToF32, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(floatTypes[index-4]);

    INT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(any, Any, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET("b8");

    BOOL_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(all, All, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET("b8");

    BOOL_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(transpose, Transpose, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    MATRIX_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(inverse, Inverse, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    MATRIX_LIST
#undef X

    //------------------------------------------------------------------------------
    /**
        Shader state intrinsics
    */
    //------------------------------------------------------------------------------
    __MAKE_BUILTIN(vertexSetOutputLayer, SetOutputLayer);
    __ADD_ARG_LIT(layer, u32);
    __SET_RET_LIT(void);

    __MAKE_BUILTIN(vertexGetOutputLayer, GetOutputLayer);
    __SET_RET_LIT(u32);

    __MAKE_BUILTIN(vertexSetOutputViewport, SetOutputViewport);
    __ADD_ARG_LIT(layer, u32);
    __SET_RET_LIT(void);

    __MAKE_BUILTIN(vertexGetOutputViewport, GetOutputViewport);
    __SET_RET_LIT(u32);

    __MAKE_BUILTIN(vertexExportCoordinates, ExportVertexCoordinates);
    __ADD_ARG_LIT(coordinates, f32x4);
    __SET_RET_LIT(void);
    __DOCUMENT_INTRINSIC("vertexExportCoordinates sends the provided f32x4 to the next step of the pipeline, similar to gl_Position or SV_POSITION");
    
    __MAKE_BUILTIN(geometryExportVertex, ExportVertex);
    __SET_RET_LIT(void);

    __MAKE_BUILTIN(geometryExportPrimitive, ExportPrimitive);
    __SET_RET_LIT(void);

    __MAKE_BUILTIN(vertexGetIndex, GetVertexIndex);
    __SET_RET_LIT(u32);

    __MAKE_BUILTIN(vertexGetInstanceIndex, GetInstanceIndex);
    __SET_RET_LIT(u32);

    __MAKE_BUILTIN(vertexGetBaseIndex, GetBaseVertexIndex);
    __SET_RET_LIT(u32);

    __MAKE_BUILTIN(vertexGetBaseInstanceIndex, GetBaseInstanceIndex);
    __SET_RET_LIT(u32);

    __MAKE_BUILTIN(vertexGetDrawIndex, GetDrawIndex);
    __SET_RET_LIT(u32);

    __MAKE_BUILTIN(pixelGetCoordinates, GetPixelCoordinates);
    __SET_RET_LIT(f32x4);
    __DOCUMENT_INTRINSIC("Returns post clip transformations coordinates as xyzw\n\n\
        xy is the screen space coordinates in range [0..render_target_width, [0..render_target_height]\n\n\
        z is the depth value in the range [0..1] as if 'pixelSetDepth' was never called\n\n\
        w is 1/w from the previous geometry shader.\n\
        ");

    __MAKE_BUILTIN(pixelSetDepth, SetPixelDepth);
    __ADD_ARG_LIT(depth, f32);
    __SET_RET_LIT(void);

    __MAKE_BUILTIN(pixelGetDepth, GetPixelDepth);
    __SET_RET_LIT(f32);

#define X(ty, idx)\
    __MAKE_INTRINSIC(pixelExportColor, ExportColor, ty)\
    __ADD_ARG_LIT(color, ty);\
    __ADD_VALUE_LIT(index, i32);\
    __SET_RET_LIT(void);\
    __DOCUMENT_INTRINSIC("Exports a value 'color' of " #ty " to the targeted export destination 'index'.\
    This is the same using a HLSL **SV_TARGET\\<index\\>** or GLSL fragment shader **out** variable with **binding(index)** and writing to it.");

    SCALAR_LIST
#undef X

    __MAKE_BUILTIN(computeGetLocalInvocationIndices, GetLocalInvocationIndices);
    __SET_RET_LIT(u32x3);
    __DOCUMENT_INTRINSIC("Returns the dispatch group's local indices as a u32x3");

    __MAKE_BUILTIN(computeGetGlobalInvocationIndices, GetGlobalInvocationIndices);
    __SET_RET_LIT(u32x3);
    __DOCUMENT_INTRINSIC("Returns the dispatch global indices as a u32x3");

    __MAKE_BUILTIN(computeGetWorkgroupIndices, GetWorkGroupIndices);
    __SET_RET_LIT(u32x3);
    __DOCUMENT_INTRINSIC("Returns the dispatch workgroup indices as a u32x3");

    __MAKE_BUILTIN(computeGetIndexInWorkgroup, GetIndexInWorkGroup);
    __SET_RET_LIT(u32);
    __DOCUMENT_INTRINSIC("Returns the dispatch groups flattened local index as u32");

    __MAKE_BUILTIN(computeGetWorkgroupDimensions, GetWorkGroupDimensions);
    __SET_RET_LIT(u32x3);
    __DOCUMENT_INTRINSIC("Returns the compute shaders workgroup dimensions as u32x3");

    //------------------------------------------------------------------------------
    /**
        Subgroup intrinsics
    */
    //------------------------------------------------------------------------------

    __MAKE_BUILTIN(subgroupGetId, GetSubgroupId);                       // The id of the subgroup
    __SET_RET_LIT(u32);

    __MAKE_BUILTIN(subgroupGetSize, GetSubgroupSize);                       // The size of the subgroup
    __SET_RET_LIT(u32);

    __MAKE_BUILTIN(subgroupGetNum, GetNumSubgroups);                       // The number of subgroups
    __SET_RET_LIT(u32);

    __MAKE_BUILTIN(subgroupGetLocalInvocationMask, GetSubgroupLocalInvocationMask);                       // The size of the subgroup
    __SET_RET_LIT(u32x4);

    __MAKE_BUILTIN(subgroupGetLocalInvocationAndLowerMask, GetSubgroupLocalInvocationAndLowerMask);                       // The size of the subgroup
    __SET_RET_LIT(u32x4);

    __MAKE_BUILTIN(subgroupGetLowerMask, GetSubgroupLowerMask);                       // The size of the subgroup
    __SET_RET_LIT(u32x4);

    __MAKE_BUILTIN(subgroupGetLocalInvocationAndGreaterMask, GetSubgroupLocalInvocationAndGreaterMask);                       // The size of the subgroup
    __SET_RET_LIT(u32x4);

    __MAKE_BUILTIN(subgroupGetGreaterMask, GetSubgroupGreaterMask);                       // The size of the subgroup
    __SET_RET_LIT(u32x4);

    __MAKE_BUILTIN(subgroupGetFirstInvocation, SubgroupFirstInvocation);      // Returns true for the first active invocation in the subgroup, and false for all else
    __SET_RET_LIT(u32);

    __MAKE_BUILTIN(subgroupRead, SubgroupRead);                 // Returns value from the invocation with the lowest invocation
    __ADD_ARG_LIT(x, u32);
    __SET_RET_LIT(u32);

    __MAKE_BUILTIN(subgroupBallot, SubgroupBallot);               // Creates lane mask with bit set to 1 if value in predicate is true
    __ADD_ARG_LIT(predicate, b8);
    __SET_RET_LIT(u32x4);

    __MAKE_BUILTIN(subgroupInverseBallot, SubgroupInverseBallot);    // Creates an inverted lane mask with bit set to 1 if predicate is false
    __ADD_ARG_LIT(predicate, b8);
    __SET_RET_LIT(u32x4);

    __MAKE_BUILTIN(subgroupBallotBitCount, SubgroupBallotBitCount);   // Counts bits set in ballot result up to thread index within subgroup
    __ADD_ARG_LIT(mask, u32x4);
    __SET_RET_LIT(u32);

    __MAKE_BUILTIN(subgroupBallotBit, SubgroupBallotBit);        // Check if bit in ballot result is for a given index
    __ADD_ARG_LIT(mask, u32x4);
    __ADD_ARG_LIT(index, u32);
    __SET_RET_LIT(b8);

    __MAKE_BUILTIN(subgroupBallotFirstOne, SubgroupBallotFirstOne);        // Get first bit set to 1
    __ADD_ARG_LIT(mask, u32x4);
    __SET_RET_LIT(u32);

    __MAKE_BUILTIN(subgroupBallotLastOne, SubgroupBallotLastOne);        // Get last bit set to 1
    __ADD_ARG_LIT(mask, u32x4);
    __SET_RET_LIT(u32);

#define X(ty, index)\
    __MAKE_INTRINSIC(subgroupSwapDiagonal, SubgroupSwapDiagonal, ty)\
    __ADD_ARG(value, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    SCALAR_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(subgroupSwapVertical, SubgroupSwapVertical, ty)\
    __ADD_ARG(value, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    SCALAR_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(subgroupSwapHorizontal, SubgroupSwapHorizontal, ty)\
    __ADD_ARG(value, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    SCALAR_LIST
#undef X

//------------------------------------------------------------------------------
/**
    Atomic intrinsics
*/
//------------------------------------------------------------------------------
#define X(ty, index)\
    __MAKE_INTRINSIC(atomicLoad, AtomicLoad, ty)\
    __ADD_ARG_LIT(value, ty);\
    __ADD_VALUE_LIT(semantics, MemorySemantics);\
    __SET_RET_LIT(ty);
    
    SINGLE_COMPONENT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(atomicStore, AtomicStore, ty)\
    __ADD_MUTABLE_ARG_LIT(orig, ty);\
    __ADD_ARG_LIT(value, ty);\
    __ADD_VALUE_LIT(semantics, MemorySemantics);\
    __SET_RET_LIT(ty);

    SINGLE_COMPONENT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(atomicExchange, AtomicExchange, ty)\
    __ADD_MUTABLE_ARG_LIT(value, ty);\
    __ADD_ARG_LIT(exchange, ty);\
    __ADD_VALUE_LIT(semantics, MemorySemantics);\
    __SET_RET_LIT(ty);

    SINGLE_COMPONENT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(atomicCompareExchange, AtomicCompareExchange, ty)\
    __ADD_MUTABLE_ARG_LIT(value, ty);\
    __ADD_ARG_LIT(compare, ty);\
    __ADD_ARG_LIT(exchange, ty);\
    __ADD_VALUE_LIT(semantics, MemorySemantics);\
    __SET_RET_LIT(ty);

    INT_SINGLE_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(atomicIncrement, AtomicIncrement, ty)\
    __ADD_MUTABLE_ARG_LIT(value, ty);\
    __ADD_VALUE_LIT(semantics, MemorySemantics);\
    __SET_RET_LIT(ty);

    INT_SINGLE_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(atomicDecrement, AtomicDecrement, ty)\
    __ADD_MUTABLE_ARG_LIT(value, ty);\
    __ADD_VALUE_LIT(semantics, MemorySemantics);\
    __SET_RET_LIT(ty);

    INT_SINGLE_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(atomicAdd, AtomicAdd, ty)\
    __ADD_MUTABLE_ARG_LIT(value, ty);\
    __ADD_ARG_LIT(addend, ty);\
    __ADD_VALUE_LIT(semantics, MemorySemantics);\
    __SET_RET_LIT(ty);

    INT_SINGLE_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(atomicSub, AtomicSub, ty)\
    __ADD_MUTABLE_ARG_LIT(value, ty);\
    __ADD_ARG_LIT(addend, ty);\
    __ADD_VALUE_LIT(semantics, MemorySemantics);\
    __SET_RET_LIT(ty);

    INT_SINGLE_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(atomicMin, AtomicMin, ty)\
    __ADD_MUTABLE_ARG_LIT(value1, ty);\
    __ADD_ARG_LIT(value2, ty);\
    __ADD_VALUE_LIT(semantics, MemorySemantics);\
    __SET_RET_LIT(ty);

    INT_SINGLE_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(atomicMax, AtomicMax, ty)\
    __ADD_MUTABLE_ARG_LIT(value1, ty);\
    __ADD_ARG_LIT(value2, ty);\
    __ADD_VALUE_LIT(semantics, MemorySemantics);\
    __SET_RET_LIT(ty);

    INT_SINGLE_LIST
#undef X
    
#define X(ty, index)\
    __MAKE_INTRINSIC(atomicAnd, AtomicAnd, ty)\
    __ADD_MUTABLE_ARG_LIT(value1, ty);\
    __ADD_ARG_LIT(value2, ty);\
    __ADD_VALUE_LIT(semantics, MemorySemantics);\
    __SET_RET_LIT(ty);

    INT_SINGLE_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(atomicOr, AtomicOr, ty)\
    __ADD_MUTABLE_ARG_LIT(value1, ty);\
    __ADD_ARG_LIT(value2, ty);\
    __ADD_VALUE_LIT(semantics, MemorySemantics);\
    __SET_RET_LIT(ty);

    INT_SINGLE_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(atomicXor, AtomicXor, ty)\
    __ADD_MUTABLE_ARG_LIT(value1, ty);\
    __ADD_ARG_LIT(value2, ty);\
    __ADD_VALUE_LIT(semantics, MemorySemantics);\
    __SET_RET_LIT(ty);

    INT_SINGLE_LIST
#undef X
    
//------------------------------------------------------------------------------
/**
    Bit intrinsics
*/
//------------------------------------------------------------------------------
    __MAKE_BUILTIN(bitInsert, BitInsert);
    __ADD_ARG_LIT(base, u32);
    __ADD_ARG_LIT(insert, u32);
    __ADD_ARG_LIT(offset, u32);
    __ADD_ARG_LIT(count, u32);
    __SET_RET_LIT(u32);

    __MAKE_BUILTIN(bitExtract, BitUExtract);
    __ADD_ARG_LIT(base, u32);
    __ADD_ARG_LIT(offset, u32);
    __ADD_ARG_LIT(count, u32);
    __SET_RET_LIT(u32);

    __MAKE_BUILTIN(bitExtract, BitSExtract);
    __ADD_ARG_LIT(base, i32);
    __ADD_ARG_LIT(offset, i32);
    __ADD_ARG_LIT(count, i32);
    __SET_RET_LIT(i32);

    __MAKE_BUILTIN(bitReverse, BitReverse);
    __ADD_ARG_LIT(base, u32);
    __SET_RET_LIT(u32);

    __MAKE_BUILTIN(bitCount, BitCount);
    __ADD_ARG_LIT(base, u32);
    __SET_RET_LIT(u32);

//------------------------------------------------------------------------------
/**
    Barrier intrinsics
*/
//------------------------------------------------------------------------------

    __MAKE_BUILTIN(executionBarrier, ExecutionBarrier);
    __SET_RET_LIT(void);

    __MAKE_BUILTIN(executionBarrierSubgroup, ExecutionBarrierSubgroup);
    __SET_RET_LIT(void);

    __MAKE_BUILTIN(executionBarrierWorkgroup, ExecutionBarrierWorkgroup);
    __SET_RET_LIT(void);

    __MAKE_BUILTIN(memoryBarrier, MemoryExecutionBarrier);
    __SET_RET_LIT(void);

    __MAKE_BUILTIN(memoryBarrierBuffer, MemoryExecutionBarrierBuffer);
    __SET_RET_LIT(void);

    __MAKE_BUILTIN(memoryBarrierTexture, MemoryExecutionBarrierTexture);
    __SET_RET_LIT(void);

    __MAKE_BUILTIN(memoryBarrierAtomic, MemoryExecutionBarrierAtomic);
    __SET_RET_LIT(void);

    __MAKE_BUILTIN(memoryBarrierSubgroup, MemoryExecutionBarrierSubgroup);
    __SET_RET_LIT(void);
    
    __MAKE_BUILTIN(memoryBarrierWorkgroup, MemoryExecutionBarrierWorkgroup);
    __SET_RET_LIT(void);


    //------------------------------------------------------------------------------
    /**
        Texture intrinsics
    */
    //------------------------------------------------------------------------------

    static const char* intCoordinates[] =
    {
        "i32"
        , "i32x2"
        , "i32x2"
        , "i32x3"
        , "i32x3"
        , "i32x2"
        , "i32x3"
        , "i32x3"
        , "i32x4"
    };

    static const char* dimensionality[] =
    {
        "u32",
        "u32x2",
        "u32x2",
        "u32x3",
        "u32x2",
        "u32x2",
        "u32x3",
        "u32x3",
        "u32x4"
    };


#define STRINGIFY(x) #x

#define __MAKE_TEXTURE_STORE_LOAD_INTRINSIC(ty, variant, overload, ret)\
newIntrinsic = StaticAlloc<Function>(); \
newIntrinsic->name = STRINGIFY(texture##ty##variant); \
new (&newIntrinsic->returnType) Type::FullType(#ret);\
Intrinsics::Texture##ty##variant##_##overload = newIntrinsic;\
DefaultIntrinsics.push_back(newIntrinsic);\
__ADD_HANDLE_ARG_LIT(texture, overload);\

#define __MAKE_TEXTURE_STORE_LOAD_INTRINSIC_BASE(ty, overload, ret)\
newIntrinsic = StaticAlloc<Function>(); \
newIntrinsic->name = STRINGIFY(texture##ty); \
new (&newIntrinsic->returnType) Type::FullType(#ret);\
Intrinsics::Texture##ty##Base_##overload = newIntrinsic;\
DefaultIntrinsics.push_back(newIntrinsic);\
__ADD_HANDLE_ARG_LIT(texture, overload);\

#define __MAKE_TEXTURE_STORE_LOAD_INTRINSIC_MUT(ty, variant, overload, ret)\
newIntrinsic = StaticAlloc<Function>(); \
newIntrinsic->name = STRINGIFY(texture##ty##variant); \
new (&newIntrinsic->returnType) Type::FullType(#ret);\
Intrinsics::Texture##ty##variant##_##overload = newIntrinsic;\
DefaultIntrinsics.push_back(newIntrinsic);\
__ADD_HANDLE_ARG_LIT_MUT(texture, overload);\

#define __MAKE_TEXTURE_STORE_LOAD_INTRINSIC_BASE_MUT(ty, overload, ret)\
newIntrinsic = StaticAlloc<Function>(); \
newIntrinsic->name = STRINGIFY(texture##ty); \
new (&newIntrinsic->returnType) Type::FullType(#ret);\
Intrinsics::Texture##ty##Base_##overload = newIntrinsic;\
DefaultIntrinsics.push_back(newIntrinsic);\
__ADD_HANDLE_ARG_LIT_MUT(texture, overload);\

#define __MAKE_TEXTURE_QUERY_INTRINSIC(ty, overload, index)\
newIntrinsic = StaticAlloc<Function>(); \
newIntrinsic->name = STRINGIFY(texture##ty); \
new (&newIntrinsic->returnType) Type::FullType(dimensionality[index]);\
Intrinsics::Texture##ty##_##overload = newIntrinsic;\
DefaultIntrinsics.push_back(newIntrinsic);\
__ADD_HANDLE_ARG_LIT(texture, overload);\

#define __MAKE_TEXTURE_QUERY_INTRINSIC_LIT(ty, overload, ret)\
newIntrinsic = StaticAlloc<Function>(); \
newIntrinsic->name = STRINGIFY(texture##ty); \
new (&newIntrinsic->returnType) Type::FullType(#ret);\
Intrinsics::Texture##ty##_##overload = newIntrinsic;\
DefaultIntrinsics.push_back(newIntrinsic);\
__ADD_HANDLE_ARG_LIT(texture, overload);\

#define __MAKE_SAMPLEDTEXTURE_QUERY_INTRINSIC_LIT(ty, overload, ret)\
newIntrinsic = StaticAlloc<Function>(); \
newIntrinsic->name = STRINGIFY(texture##ty); \
new (&newIntrinsic->returnType) Type::FullType(#ret);\
Intrinsics::Texture##ty##_##overload = newIntrinsic;\
DefaultIntrinsics.push_back(newIntrinsic);\
__ADD_SAMPLED_HANDLE_ARG_LIT(texture, overload);\

#define __MAKE_TEXTURE_INTRINSIC(ty, variant, overload, ret)\
newIntrinsic = StaticAlloc<Function>(); \
newIntrinsic->name = STRINGIFY(texture##ty##variant); \
new (&newIntrinsic->returnType) Type::FullType(#ret);\
Intrinsics::Texture##ty##variant##_##overload = newIntrinsic;\
DefaultIntrinsics.push_back(newIntrinsic);\
__ADD_HANDLE_ARG_LIT(texture, overload);\
__ADD_HANDLE_ARG_LIT(sampler, sampler);

#define __MAKE_SAMPLEDTEXTURE_INTRINSIC(ty, variant, overload, ret)\
newIntrinsic = StaticAlloc<Function>(); \
newIntrinsic->name = STRINGIFY(texture##ty##variant); \
new (&newIntrinsic->returnType) Type::FullType(#ret);\
Intrinsics::SampledTexture##ty##variant##_##overload = newIntrinsic;\
DefaultIntrinsics.push_back(newIntrinsic);\
__ADD_SAMPLED_HANDLE_ARG_LIT(texture, overload);\

#define __MAKE_TEXTURE_INTRINSIC_BASE(ty, overload, ret)\
newIntrinsic = StaticAlloc<Function>(); \
newIntrinsic->name = STRINGIFY(texture##ty); \
new (&newIntrinsic->returnType) Type::FullType(#ret);\
Intrinsics::Texture##ty##Base_##overload = newIntrinsic;\
DefaultIntrinsics.push_back(newIntrinsic);\
__ADD_HANDLE_ARG_LIT(texture, overload);\
__ADD_HANDLE_ARG_LIT(sampler, sampler);

#define __MAKE_SAMPLEDTEXTURE_INTRINSIC_BASE(ty, overload, ret)\
newIntrinsic = StaticAlloc<Function>(); \
newIntrinsic->name = STRINGIFY(texture##ty); \
new (&newIntrinsic->returnType) Type::FullType(#ret);\
Intrinsics::SampledTexture##ty##Base_##overload = newIntrinsic;\
DefaultIntrinsics.push_back(newIntrinsic);\
__ADD_SAMPLED_HANDLE_ARG_LIT(texture, overload);\

#define X(type, index)\
    __MAKE_TEXTURE_QUERY_INTRINSIC(GetSize, type, index)

    TEXTURE_INTRINSIC_NO_MS_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_QUERY_INTRINSIC(GetSizeMip, type, index)\
    __ADD_ARG_LIT(mip, f32);

    TEXTURE_INTRINSIC_NO_MS_LIST
#undef X

#define X(type, index)\
    __MAKE_SAMPLEDTEXTURE_QUERY_INTRINSIC_LIT(GetSampledMip, type, f32)\
    __ADD_HANDLE_ARG_LIT(sampler, sampler);\
    __ADD_ARG(coords, intCoordinates[index]);
    
    TEXTURE_INTRINSIC_NO_MS_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_QUERY_INTRINSIC_LIT(GetMips, type, u32)\
        
    TEXTURE_INTRINSIC_NO_MS_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_QUERY_INTRINSIC_LIT(GetSamples, type, u32)\
            
    TEXTURE_INTRINSIC_ONLY_MS_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_STORE_LOAD_INTRINSIC_BASE_MUT(Load, type, f32x4)\
    __ADD_ARG(coords, intCoordinates[index]);

    TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_STORE_LOAD_INTRINSIC_MUT(Load, Mip, type, f32x4)\
    __ADD_ARG(coords, intCoordinates[index]);\
    __ADD_ARG_LIT(lod, i32);

    TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_STORE_LOAD_INTRINSIC_BASE_MUT(Store, type, f32x4)\
    __ADD_ARG(coords, intCoordinates[index]);\
    __ADD_ARG_LIT(value, f32x4);

    TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_STORE_LOAD_INTRINSIC_MUT(Store, Mip, type, f32x4)\
    __ADD_ARG(coords, intCoordinates[index]);\
    __ADD_ARG_LIT(lod, i32);\
    __ADD_ARG_LIT(value, f32x4);

    TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_STORE_LOAD_INTRINSIC_BASE(Fetch, type, f32x4)\
    __ADD_ARG(coords, intCoordinates[index]);\
    __ADD_ARG_LIT(lod, i32);

    TEXTURE_INTRINSIC_NO_CUBE_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_STORE_LOAD_INTRINSIC(Fetch, Sample, type, f32x4)\
    __ADD_ARG(coords, intCoordinates[index]);\
    __ADD_ARG_LIT(lod, i32);
    __ADD_ARG_LIT(sample, i32);

    TEXTURE_INTRINSIC_ONLY_MS_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_STORE_LOAD_INTRINSIC_BASE(Gather, type, f32x4)\
    __ADD_ARG(coords, intCoordinates[index]);\
    __ADD_ARG_LIT(lod, i32);

    TEXTURE_INTRINSIC_GATHER_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_STORE_LOAD_INTRINSIC(Gather, Offsets, type, f32x4)\
    __ADD_ARG(coords, intCoordinates[index]);\
    __ADD_ARG_LIT(lod, i32);\
    __ADD_ARG_ARR_LIT(offsets, i32x2, 4);

    TEXTURE_INTRINSIC_2D_LIST
#undef X


    __MAKE_BUILTIN(pixelCacheLoad, PixelCacheLoad);
    __ADD_HANDLE_ARG_LIT(texture, pixelCache);
    __SET_RET_LIT(f32x4);

    __MAKE_BUILTIN(pixelCacheLoad, PixelCacheLoad);
    __ADD_HANDLE_ARG_LIT(texture, pixelCacheMS);
    __SET_RET_LIT(f32x4);

    std::string normCoordinates[] =
    {
        "f32"     // 1D
        , "f32x2"  // 2D
        , "f32x2"  // 2DMS
        , "f32x3"  // 3D
        , "f32x3"  // Cube
        , "f32x2"  // 1D + array slice
        , "f32x3"  // 2D + array slice
        , "f32x3"  // 2D + array slice
        , "f32x4"  // 3D + array slice
    };

    std::string projCoordinates[] =
    {
        "f32x2"    // 1D
        , "f32x3"  // 2D
        , "f32x3"  // 2DMS
        , "f32x4"  // 3D
        , "f32x4"  // Cube
        , "f32x3"  // 1D + array slice
        , "f32x4"  // 2D + array slice
        , "f32x4"  // 2D + array slice
        , "f32x5"  // 3D + array slice // invalid
    };

    std::string offsets[] =
    {
        "i32"     // 1D
        , "i32x2"  // 2D
        , "i32x2"  // 2DMS
        , "i32x3"  // 3D
        , "i32x3"  // Cube
        , "i32"   // 1DArray - array slice
        , "i32x2"  // 2DArray - array slice
        , "i32x2"  // 2DArray - array slice
        , "i32x3"  // 3DArray - array slice
    };

    std::string gradients[] =
    {
        "f32"     // 1D
        , "f32x2"  // 2D
        , "f32x2"  // 2DMS
        , "f32x3"  // 3D
        , "f32x3"  // Cube
        , "f32"   // 1DArray - array slice
        , "f32x2"  // 2DArray - array slice
        , "f32x2"  // 2DArray - array slice
        , "f32x3"  // 3DArray - array slice
    };

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC_BASE(Sample, type, f32x4)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC_BASE(Sample, type, f32x4)\
    __ADD_ARG(coords, normCoordinates[index]);

    TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, Offset, type, f32x4)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG(offsets, offsets[index]);\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, Offset, type, f32x4)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG(offsets, offsets[index]);

    TEXTURE_INTRINSIC_NO_CUBE_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, Bias, type, f32x4)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(bias, f32);\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, Bias, type, f32x4)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(bias, f32);

    TEXTURE_INTRINSIC_NO_MS_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, BiasCompare, type, f32)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(bias, f32);\
    __ADD_ARG_LIT(compare, f32);\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, BiasCompare, type, f32)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(bias, f32);\
    __ADD_ARG_LIT(compare, f32);

    TEXTURE_INTRINSIC_NO_MS_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, BiasOffset, type, f32x4)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(bias, f32);\
    __ADD_ARG(offsets, offsets[index]);\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, BiasOffset, type, f32x4)\
    __ADD_ARG(coords, normCoordinates[index]); \
    __ADD_ARG_LIT(bias, f32); \
    __ADD_ARG(offsets, offsets[index]);

    TEXTURE_INTRINSIC_NO_MS_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, BiasProj, type, f32x4)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(bias, f32);\
    __ADD_ARG_LIT(proj, f32);\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, BiasProj, type, f32x4)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(bias, f32);\
    __ADD_ARG_LIT(proj, f32);

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, BiasProjCompare, type, f32)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(bias, f32);\
    __ADD_ARG_LIT(proj, f32);\
    __ADD_ARG_LIT(compare, f32);\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, BiasProjCompare, type, f32)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(bias, f32);\
    __ADD_ARG_LIT(proj, f32);\
    __ADD_ARG_LIT(compare, f32);

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, BiasProjOffset, type, f32x4)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(bias, f32);\
    __ADD_ARG_LIT(proj, f32);\
    __ADD_ARG(offsets, offsets[index]);\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, BiasProjOffset, type, f32x4)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(bias, f32);\
    __ADD_ARG_LIT(proj, f32);\
    __ADD_ARG(offsets, offsets[index]);

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, Compare, type, f32)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(compare, f32);\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, Compare, type, f32)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(compare, f32);

    TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, CompareOffset, type, f32)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(compare, f32);\
    __ADD_ARG(offsets, offsets[index]);\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, CompareOffset, type, f32)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(compare, f32);\
    __ADD_ARG(offsets, offsets[index]);

    TEXTURE_INTRINSIC_NO_CUBE_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, Grad, type, f32x4)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG(gradX, gradients[index]);\
    __ADD_ARG(gradY, gradients[index]);\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, Grad, type, f32x4)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG(gradX, gradients[index]);\
    __ADD_ARG(gradY, gradients[index]);

    TEXTURE_INTRINSIC_NO_MS_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, GradCompare, type, f32)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG(gradX, gradients[index]);\
    __ADD_ARG(gradY, gradients[index]);\
    __ADD_ARG_LIT(compare, f32);\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, GradCompare, type, f32)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG(gradX, gradients[index]);\
    __ADD_ARG(gradY, gradients[index]);\
    __ADD_ARG_LIT(compare, f32);

    TEXTURE_INTRINSIC_NO_CUBE_MS_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, GradOffset, type, f32x4)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG(gradX, gradients[index]);\
    __ADD_ARG(gradY, gradients[index]);\
    __ADD_ARG(offsets, offsets[index]);\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, GradOffset, type, f32x4)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG(gradX, gradients[index]);\
    __ADD_ARG(gradY, gradients[index]);\
    __ADD_ARG(offsets, offsets[index]);

    TEXTURE_INTRINSIC_NO_CUBE_MS_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, GradProj, type, f32x4)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG(gradX, gradients[index]);\
    __ADD_ARG(gradY, gradients[index]);\
    __ADD_ARG_LIT(proj, f32);\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, GradProj, type, f32x4)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG(gradX, gradients[index]);\
    __ADD_ARG(gradY, gradients[index]);\
    __ADD_ARG_LIT(proj, f32);

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, GradProjCompare, type, f32)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG(gradX, gradients[index]);\
    __ADD_ARG(gradY, gradients[index]);\
    __ADD_ARG_LIT(proj, f32);\
    __ADD_ARG_LIT(compare, f32);\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, GradProjCompare, type, f32)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG(gradX, gradients[index]);\
    __ADD_ARG(gradY, gradients[index]);\
    __ADD_ARG_LIT(proj, f32);\
    __ADD_ARG_LIT(compare, f32);

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, GradProjCompareOffset, type, f32)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG(gradX, gradients[index]);\
    __ADD_ARG(gradY, gradients[index]);\
    __ADD_ARG_LIT(proj, f32);\
    __ADD_ARG_LIT(compare, f32);\
    __ADD_ARG(offsets, offsets[index]);\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, GradProjCompareOffset, type, f32)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG(gradX, gradients[index]);\
    __ADD_ARG(gradY, gradients[index]);\
    __ADD_ARG_LIT(proj, f32);\
    __ADD_ARG_LIT(compare, f32);\
    __ADD_ARG(offsets, offsets[index]);

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, GradProjOffset, type, f32x4)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG(gradX, gradients[index]);\
    __ADD_ARG(gradY, gradients[index]);\
    __ADD_ARG_LIT(proj, f32);\
    __ADD_ARG(offsets, offsets[index]);\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, GradProjOffset, type, f32x4)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG(gradX, gradients[index]);\
    __ADD_ARG(gradY, gradients[index]);\
    __ADD_ARG_LIT(proj, f32);\
    __ADD_ARG(offsets, offsets[index]);

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, Lod, type, f32x4)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(lod, f32);\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, Lod, type, f32x4)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(lod, f32);

    TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, LodCompare, type, f32)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(lod, f32);\
    __ADD_ARG_LIT(compare, f32);\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, LodCompare, type, f32)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(lod, f32);\
    __ADD_ARG_LIT(compare, f32);

    TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, LodCompareOffset, type, f32)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(lod, f32);\
    __ADD_ARG_LIT(compare, f32);\
    __ADD_ARG(offsets, offsets[index]);\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, LodCompareOffset, type, f32)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(lod, f32);\
    __ADD_ARG_LIT(compare, f32);\
    __ADD_ARG(offsets, offsets[index]);

    TEXTURE_INTRINSIC_NO_CUBE_LIST
#undef X

#define X(type, index)\
{\
    __MAKE_TEXTURE_INTRINSIC(Sample, LodOffset, type, f32x4)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(lod, f32);\
    __ADD_ARG(offsets, offsets[index]);\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, LodOffset, type, f32x4)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(lod, f32);\
    __ADD_ARG(offsets, offsets[index]);\
}

    TEXTURE_INTRINSIC_NO_CUBE_LIST
#undef X

#define X(type, index)\
{\
    __MAKE_TEXTURE_INTRINSIC(Sample, LodProj, type, f32x4)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(lod, f32);\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, LodProj, type, f32x4)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(lod, f32);\
}

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
{\
    __MAKE_TEXTURE_INTRINSIC(Sample, LodProjCompare, type, f32)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(lod, f32);\
    __ADD_ARG_LIT(compare, f32);\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, LodProjCompare, type, f32)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(lod, f32);\
    __ADD_ARG_LIT(compare, f32);\
}

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
{\
    __MAKE_TEXTURE_INTRINSIC(Sample, LodProjCompareOffset, type, f32)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(lod, f32);\
    __ADD_ARG_LIT(compare, f32);\
    __ADD_ARG(offsets, offsets[index]);\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, LodProjCompareOffset, type, f32)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(lod, f32);\
    __ADD_ARG_LIT(compare, f32);\
    __ADD_ARG(offsets, offsets[index]);\
}

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
{\
    __MAKE_TEXTURE_INTRINSIC(Sample, LodProjOffset, type, f32x4)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(lod, f32);\
    __ADD_ARG(offsets, offsets[index]);\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, LodProjOffset, type, f32x4)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(lod, f32);\
    __ADD_ARG(offsets, offsets[index]);\
}

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, Proj, type, f32x4)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, Proj, type, f32x4)\
    __ADD_ARG(coords, projCoordinates[index]);

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, ProjCompare, type, f32)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(compare, f32);\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, ProjCompare, type, f32)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(compare, f32);

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, ProjCompareOffset, type, f32)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(compare, f32);\
    __ADD_ARG(offsets, offsets[index]);\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, ProjCompareOffset, type, f32)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(compare, f32);\
    __ADD_ARG(offsets, offsets[index]);

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, ProjOffset, type, f32x4)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG(offsets, offsets[index]);\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, ProjOffset, type, f32x4)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG(offsets, offsets[index]);

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X
    
    //------------------------------------------------------------------------------
    /**
        Raytracing intrinsics
    */
    //------------------------------------------------------------------------------
    __MAKE_BUILTIN(rayTrace, TraceRay);
    __ADD_HANDLE_ARG_LIT(tlas, accelerationStructure);
    __ADD_ARG_LIT(origin, f32x3);
    __ADD_ARG_LIT(direction, f32x3);
    __ADD_ARG_LIT(min, f32);
    __ADD_ARG_LIT(max, f32);
    __ADD_ARG_LIT(shaderTableOffset, u32);
    __ADD_ARG_LIT(shaderTableStride, u32);
    __ADD_ARG_LIT(missShaderIndex, u32);
    __ADD_ARG_LIT(flags, u32);
    __ADD_ARG_LIT(mask, u32);
    __SET_RET_LIT(void)

    __MAKE_BUILTIN(rayExportIntersection, ExportRayIntersection);
    __SET_RET_LIT(void);

    __MAKE_BUILTIN(rayExecuteCallable, ExecuteCallable);
    __SET_RET_LIT(void);

    __MAKE_BUILTIN(rayGetLaunchIndex, RayLaunchIndex);
    __SET_RET_LIT(u32x3);

    __MAKE_BUILTIN(rayGetLaunchSize, RayLaunchSize);
    __SET_RET_LIT(u32x3);

    __MAKE_BUILTIN(BLASGetPrimitiveIndex, BLASPrimitiveIndex);
    __SET_RET_LIT(u32);

    __MAKE_BUILTIN(BLASGetGeometryIndex, BLASGeometryIndex);
    __SET_RET_LIT(u32);

    __MAKE_BUILTIN(TLASGetInstanceIndex, TLASInstanceIndex);
    __SET_RET_LIT(u32);

    __MAKE_BUILTIN(TLASGetInstanceCustomIndex, TLASInstanceCustomIndex);
    __SET_RET_LIT(u32);

    __MAKE_BUILTIN(rayGetWorldOrigin, RayWorldOrigin);
    __SET_RET_LIT(f32x3);
    
    __MAKE_BUILTIN(rayGetWorldDirection, RayWorldDirection);
    __SET_RET_LIT(f32x3);

    __MAKE_BUILTIN(rayGetObjectOrigin, RayObjectOrigin);
    __SET_RET_LIT(f32x3);

    __MAKE_BUILTIN(rayGetObjectDirection, RayObjectDirection);
    __SET_RET_LIT(f32x3);

    __MAKE_BUILTIN(rayGetMin, RayMin);
    __SET_RET_LIT(f32);

    __MAKE_BUILTIN(rayGetMax, RayMax);
    __SET_RET_LIT(f32);

    __MAKE_BUILTIN(rayGetFlags, RayFlags);
    __SET_RET_LIT(u32);

    __MAKE_BUILTIN(rayGetHitDistance, RayHitDistance);
    __SET_RET_LIT(f32);

    __MAKE_BUILTIN(rayGetHitKind, RayHitKind);
    __SET_RET_LIT(u32);

    __MAKE_BUILTIN(TLASGetObjectToWorld, TLASObjectToWorld);
    __SET_RET_LIT(f32x4x3);
    
    __MAKE_BUILTIN(TLASGetWorldToObject, TLASWorldToObject);
    __SET_RET_LIT(f32x4x3);

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
    if (otherFunction->parameters.size() != this->parameters.size())
        return false;

    // go through parameters
    for (size_t i = 0; i < this->parameters.size(); i++)
    {
        Variable* ourParameter = this->parameters[i];
        Variable* otherParameter = otherFunction->parameters[i];

        if (ourParameter->type != otherParameter->type)
            return false;
    }

    return true;
}

} // namespace GPULang
