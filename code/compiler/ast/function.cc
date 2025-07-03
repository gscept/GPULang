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
#include "ast/types/builtins.h"

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
        if (fun->parameters.size != args.size())
            continue;

        ret = sym;
        for (size_t i = 0; i < args.size() && ret != nullptr; i++)
        {
            Variable::__Resolved* paramResolved = Symbol::Resolved(fun->parameters.buf[i]);
            if (args[i] != paramResolved->type)
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

#define __BEGIN_INTRINSICS__ Function* newIntrinsic = nullptr; Variable* newVar = nullptr; TransientArray<Variable*> arguments(32); TransientArray<Symbol*> Intrinsics(1000);

#define __DOCUMENT_INTRINSIC(str)\
newIntrinsic->documentation = str;

#define __MAKE_BUILTIN(nm, opcode)\
{\
newIntrinsic = StaticAlloc<Function>();\
newIntrinsic->name = ConstantString(#nm);\
Intrinsics::opcode = newIntrinsic;\
Intrinsics.Append(newIntrinsic);\
arguments.Clear();\
}

#define __MAKE_INTRINSIC(nm, opcode, ty)\
{\
newIntrinsic = StaticAlloc<Function>();\
newIntrinsic->name = ConstantString(#nm);\
Intrinsics::opcode##_##ty = newIntrinsic;\
Intrinsics.Append(newIntrinsic);\
arguments.Clear();\
}

#define __ADD_ARG(nm, tp)\
{\
newVar = StaticAlloc<Variable>();\
newVar->name = ConstantString(#nm);\
newVar->type = Type::FullType(tp->name);\
Symbol::Resolved(newVar)->typeSymbol = tp;\
arguments.Append(newVar);\
}

#define __ADD_ARG_LIT(nm, tp)\
{\
newVar = StaticAlloc<Variable>();\
newVar->name = ConstantString(#nm);\
newVar->type = Type::FullType(tp##Type.name);\
Symbol::Resolved(newVar)->typeSymbol = &tp##Type;\
arguments.Append(newVar);\
}

#define __ADD_MUTABLE_ARG_LIT(nm, tp)\
{\
newVar = StaticAlloc<Variable>();\
newVar->name = ConstantString(#nm);\
newVar->type = Type::FullType(tp##Type.name);\
Symbol::Resolved(newVar)->typeSymbol = &tp##Type;\
newVar->type.mut = true;\
arguments.Append(newVar);\
}

#define __ADD_VALUE_LIT(nm, tp)\
{\
newVar = StaticAlloc<Variable>();\
newVar->name = ConstantString(#nm);\
newVar->type = Type::FullType(tp##Type.name);\
Symbol::Resolved(newVar)->typeSymbol = &tp##Type;\
newVar->type.literal = true;\
arguments.Append(newVar);\
}

#define __ADD_ARG_ARR_LIT(nm, tp, size)\
{\
newVar = StaticAlloc<Variable>();\
newVar->name = ConstantString(#nm);\
newVar->type = Type::FullType(tp##Type.name);\
Symbol::Resolved(newVar)->typeSymbol = &tp##Type;\
newVar->type.AddModifier(Type::FullType::Modifier::Array);\
newVar->type.UpdateValue(StaticAlloc<UIntExpression>(size));\
arguments.Append(newVar);\
}

#define __ADD_HANDLE_ARG(nm, tp)\
{\
newVar = StaticAlloc<Variable>();\
newVar->name = ConstantString(#nm);\
newVar->type = Type::FullType(tp##Type.name);\
Symbol::Resolved(newVar)->typeSymbol = &tp##Type;\
newVar->type.AddModifier(Type::FullType::Modifier::Pointer);\
newVar->attributes = StaticArray<Attribute*>(1);\
newVar->attributes.Append(StaticAlloc<Attribute>("uniform"));\
arguments.Append(newVar);\
}

#define __ADD_HANDLE_ARG_LIT(nm, tp)\
{\
newVar = StaticAlloc<Variable>();\
newVar->name = ConstantString(#nm);\
newVar->type = Type::FullType(tp##Type.name);\
Symbol::Resolved(newVar)->typeSymbol = &tp##Type;\
newVar->type.AddModifier(Type::FullType::Modifier::Pointer);\
newVar->attributes = StaticArray<Attribute*>(1);\
newVar->attributes.Append(StaticAlloc<Attribute>("uniform"));\
arguments.Append(newVar);\
}

#define __ADD_SAMPLED_HANDLE_ARG_LIT(nm, tp)\
{\
newVar = StaticAlloc<Variable>();\
newVar->name = ConstantString(#nm);\
newVar->type = Type::FullType(tp##Type.name);\
Symbol::Resolved(newVar)->typeSymbol = &tp##Type;\
newVar->type.AddModifier(Type::FullType::Modifier::Pointer);\
newVar->type.sampled = true;\
newVar->attributes = StaticArray<Attribute*>(1);\
newVar->attributes.Append(StaticAlloc<Attribute>("uniform"));\
arguments.Append(newVar);\
}

#define __ADD_HANDLE_ARG_LIT_MUT(nm, tp)\
{\
newVar = StaticAlloc<Variable>();\
newVar->name = ConstantString(#nm);\
newVar->type = Type::FullType(tp##Type.name);\
Symbol::Resolved(newVar)->typeSymbol = &tp##Type;\
newVar->type.AddModifier(Type::FullType::Modifier::Pointer);\
newVar->type.mut = true;\
newVar->attributes = StaticArray<Attribute*>(1);\
newVar->attributes.Append(StaticAlloc<Attribute>("uniform"));\
arguments.Append(newVar);\
}

#define __ADD_POINTER_ARG_LIT(nm, tp)\
{\
newVar = StaticAlloc<Variable>();\
newVar->name = ConstantString(#nm);\
newVar->type = Type::FullType(tp##Type.name);\
Symbol::Resolved(newVar)->typeSymbol = &tp##Type;\
newVar->type.AddModifier(Type::FullType::Modifier::Pointer);\
arguments.Append(newVar);\
}


#define __SET_RET_LIT(ty)\
{ newIntrinsic->returnType = Type::FullType(ty##Type.name); Symbol::Resolved(newIntrinsic)->returnTypeSymbol = &ty##Type;  newIntrinsic->parameters = StaticArray<Variable*>(arguments); }

#define __SET_RET(ty)\
{ newIntrinsic->returnType = Type::FullType(ty->name); Symbol::Resolved(newIntrinsic)->returnTypeSymbol = ty; newIntrinsic->parameters = StaticArray<Variable*>(arguments); }

//------------------------------------------------------------------------------
/**
*/
FixedArray<Symbol*> DefaultIntrinsics;
void
Function::SetupIntrinsics()
{
    SYMBOL_STATIC_ALLOC = true;
    __BEGIN_INTRINSICS__;

    static Type* scalarArgs[] =
    {
        &FloatType
        , &Float2Type
        , &Float3Type
        , &Float4Type
        , &IntType
        , &Int2Type
        , &Int3Type
        , &Int4Type
        , &UIntType
        , &UInt2Type
        , &UInt3Type
        , &UInt4Type
        , &BoolType
        , &Bool2Type
        , &Bool3Type
        , &Bool4Type
        , &Mat2x2Type
        , &Mat2x3Type
        , &Mat2x4Type
        , &Mat3x2Type
        , &Mat3x3Type
        , &Mat3x4Type
        , &Mat4x2Type
        , &Mat4x3Type
        , &Mat4x4Type
    };
    constexpr uint32_t numScalarArgs = sizeof(scalarArgs) / sizeof(Type*);

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
    __SET_RET_LIT(Float);

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
    __ADD_ARG_LIT(eta, Float);\
    __SET_RET(scalarArgs[index]);

    FLOAT_VEC_LIST
#undef X

__MAKE_INTRINSIC(cross, Cross, f32x3)
__ADD_ARG_LIT(v0, Float3);
__ADD_ARG_LIT(v1, Float3);
__SET_RET_LIT(Float3);

#define X(ty, index)\
    __MAKE_INTRINSIC(normalize, Normalize, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    FLOAT_VEC_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(length, Length, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET_LIT(Float);

    FLOAT_VEC_LIST
#undef X

    #define X(ty, index)\
    __MAKE_INTRINSIC(distance, Distance, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __ADD_ARG(y, scalarArgs[index]);\
    __SET_RET_LIT(Float);

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

    static Type* uintTypes[] =
    {
        &UIntType
        , &UInt2Type
        , &UInt3Type
        , &UInt4Type
    };

#define X(ty, index)\
    __MAKE_INTRINSIC(castToU32, CastToU32, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(uintTypes[index]);

    FLOAT_LIST
#undef X

    static Type* intTypes[] =
    {
        &IntType
        , &Int2Type
        , &Int3Type
        , &Int4Type
    };

#define X(ty, index)\
    __MAKE_INTRINSIC(castToI32, CastToI32, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET(intTypes[index]);

    FLOAT_LIST
#undef X

    static Type* floatTypes[] =
    {
        &FloatType
        , &Float2Type
        , &Float3Type
        , &Float4Type
        , &FloatType
        , &Float2Type
        , &Float3Type
        , &Float4Type
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
    __SET_RET_LIT(Bool);

    BOOL_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(all, All, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __SET_RET_LIT(Bool);

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
    __ADD_ARG_LIT(layer, UInt);
    __SET_RET_LIT(Void);

    __MAKE_BUILTIN(vertexGetOutputLayer, GetOutputLayer);
    __SET_RET_LIT(UInt);

    __MAKE_BUILTIN(vertexSetOutputViewport, SetOutputViewport);
    __ADD_ARG_LIT(layer, UInt);
    __SET_RET_LIT(Void);

    __MAKE_BUILTIN(vertexGetOutputViewport, GetOutputViewport);
    __SET_RET_LIT(UInt);

    __MAKE_BUILTIN(vertexExportCoordinates, ExportVertexCoordinates);
    __ADD_ARG_LIT(coordinates, Float4);
    __SET_RET_LIT(Void);
    __DOCUMENT_INTRINSIC("vertexExportCoordinates sends the provided f32x4 to the next step of the pipeline, similar to gl_Position or SV_POSITION");
    
    __MAKE_BUILTIN(geometryExportVertex, ExportVertex);
    __SET_RET_LIT(Void);

    __MAKE_BUILTIN(geometryExportPrimitive, ExportPrimitive);
    __SET_RET_LIT(Void);

    __MAKE_BUILTIN(vertexGetIndex, GetVertexIndex);
    __SET_RET_LIT(UInt);

    __MAKE_BUILTIN(vertexGetInstanceIndex, GetInstanceIndex);
    __SET_RET_LIT(UInt);

    __MAKE_BUILTIN(vertexGetBaseIndex, GetBaseVertexIndex);
    __SET_RET_LIT(UInt);

    __MAKE_BUILTIN(vertexGetBaseInstanceIndex, GetBaseInstanceIndex);
    __SET_RET_LIT(UInt);

    __MAKE_BUILTIN(vertexGetDrawIndex, GetDrawIndex);
    __SET_RET_LIT(UInt);

    __MAKE_BUILTIN(pixelGetCoordinates, GetPixelCoordinates);
    __SET_RET_LIT(Float4);
    __DOCUMENT_INTRINSIC("Returns post clip transformations coordinates as xyzw\n\n\
        xy is the screen space coordinates in range [0..render_target_width, [0..render_target_height]\n\n\
        z is the depth value in the range [0..1] as if 'pixelSetDepth' was never called\n\n\
        w is 1/w from the previous geometry shader.\n\
        ");

    __MAKE_BUILTIN(pixelSetDepth, SetPixelDepth);
    __ADD_ARG_LIT(depth, Float);
    __SET_RET_LIT(UInt);

    __MAKE_BUILTIN(pixelGetDepth, GetPixelDepth);
    __SET_RET_LIT(Float);

#define X(ty, idx)\
    __MAKE_INTRINSIC(pixelExportColor, ExportColor, ty)\
    __ADD_ARG_LIT(color, ty);\
    __ADD_VALUE_LIT(index, Int);\
    __SET_RET_LIT(Void);\
    __DOCUMENT_INTRINSIC("Exports a value 'color' of " #ty " to the targeted export destination 'index'.\
    This is the same using a HLSL **SV_TARGET\\<index\\>** or GLSL fragment shader **out** variable with **binding(index)** and writing to it.");

    SCALAR_LIST
#undef X

    __MAKE_BUILTIN(computeGetLocalInvocationIndices, GetLocalInvocationIndices);
    __SET_RET_LIT(UInt3);
    __DOCUMENT_INTRINSIC("Returns the u32x3 indices within the workgroup");

    __MAKE_BUILTIN(computeGetGlobalInvocationIndices, GetGlobalInvocationIndices);
    __SET_RET_LIT(UInt3);
    __DOCUMENT_INTRINSIC("Returns the u32x3 global indices for the dispatch call");

    __MAKE_BUILTIN(computeGetWorkgroupIndices, GetWorkGroupIndices);
    __SET_RET_LIT(UInt3);
    __DOCUMENT_INTRINSIC("Returns the u32x3 workgroup indices as invoked from the API");

    __MAKE_BUILTIN(computeGetIndexInWorkgroup, GetIndexInWorkGroup);
    __SET_RET_LIT(UInt);
    __DOCUMENT_INTRINSIC("Returns a flattened u32 index within the workgroup");

    __MAKE_BUILTIN(computeGetWorkgroupDimensions, GetWorkGroupDimensions);
    __SET_RET_LIT(UInt3);
    __DOCUMENT_INTRINSIC("Returns the compute shaders workgroup dimensions as u32x3");

    //------------------------------------------------------------------------------
    /**
        Subgroup intrinsics
    */
    //------------------------------------------------------------------------------

    __MAKE_BUILTIN(subgroupGetId, GetSubgroupId);                       // The id of the subgroup
    __SET_RET_LIT(UInt);
    __DOCUMENT_INTRINSIC("Returns the subgroup index within the workgroup");

    __MAKE_BUILTIN(subgroupGetSize, GetSubgroupSize);                       // The size of the subgroup
    __SET_RET_LIT(UInt);
    __DOCUMENT_INTRINSIC("Returns the size of the subgroup within the workgroup");

    __MAKE_BUILTIN(subgroupGetNum, GetNumSubgroups);                       // The number of subgroups
    __SET_RET_LIT(UInt);
    __DOCUMENT_INTRINSIC("Returns the number of subgroups within the work group");

    __MAKE_BUILTIN(subgroupGetLocalInvocationMask, GetSubgroupLocalInvocationMask);
    __SET_RET_LIT(UInt4);
    __DOCUMENT_INTRINSIC("Returns a mask where the index of the current thread has the bit set to 1");

    __MAKE_BUILTIN(subgroupGetLocalInvocationAndLowerMask, GetSubgroupLocalInvocationAndLowerMask);
    __SET_RET_LIT(UInt4);
    __DOCUMENT_INTRINSIC("Returns a mask where the index of the current thread, and all other threads lesser than it, with the bit set to 1");

    __MAKE_BUILTIN(subgroupGetLowerMask, GetSubgroupLowerMask);
    __SET_RET_LIT(UInt4);
    __DOCUMENT_INTRINSIC("Returns a mask where the index of all other threads lesser than it, with the bit set to 1");

    __MAKE_BUILTIN(subgroupGetLocalInvocationAndGreaterMask, GetSubgroupLocalInvocationAndGreaterMask);
    __SET_RET_LIT(UInt4);
    __DOCUMENT_INTRINSIC("Returns a mask where the index of the current thread, and all other threads greater than it, with the bit set to 1");

    __MAKE_BUILTIN(subgroupGetGreaterMask, GetSubgroupGreaterMask);                       // The size of the subgroup
    __SET_RET_LIT(UInt4);
    __DOCUMENT_INTRINSIC("Returns a mask where the index of all other threads greater than it, with the bit set to 1");

    __MAKE_BUILTIN(subgroupGetFirstInvocation, SubgroupFirstInvocation);      // Returns true for the first active invocation in the subgroup, and false for all else
    __SET_RET_LIT(UInt);
    __DOCUMENT_INTRINSIC("Returns the index of the first active thread within the workgroup. An active thread is one which executes the current conditional scope.");

    __MAKE_BUILTIN(subgroupRead, SubgroupRead);                 // Returns value from the invocation with the lowest invocation
    __ADD_ARG_LIT(x, UInt);
    __SET_RET_LIT(UInt);
    __DOCUMENT_INTRINSIC("Reads the value of the first active thread within the workgroup given a variable. An active thread is one which executes the current conditional scope.");

    __MAKE_BUILTIN(subgroupBallot, SubgroupBallot);               // Creates lane mask with bit set to 1 if value in predicate is true
    __ADD_ARG_LIT(predicate, Bool);
    __SET_RET_LIT(UInt4);
    __DOCUMENT_INTRINSIC("Constructs a mask of all threads within the workgroup where the predicate is true.");

    __MAKE_BUILTIN(subgroupInverseBallot, SubgroupInverseBallot);    // Creates an inverted lane mask with bit set to 1 if predicate is false
    __ADD_ARG_LIT(predicate, Bool);
    __SET_RET_LIT(UInt4);
    __DOCUMENT_INTRINSIC("Constructs a mask of all threads within the workgroup where the predicate is false.");

    __MAKE_BUILTIN(subgroupBallotBitCount, SubgroupBallotBitCount);   // Counts bits set in ballot result up to thread index within subgroup
    __ADD_ARG_LIT(mask, UInt4);
    __SET_RET_LIT(UInt);
    __DOCUMENT_INTRINSIC("Count the bits set to 1 in a subgroup thread mask.");

    __MAKE_BUILTIN(subgroupBallotBit, SubgroupBallotBit);        // Check if bit in ballot result is for a given index
    __ADD_ARG_LIT(mask, UInt4);
    __ADD_ARG_LIT(index, UInt);
    __SET_RET_LIT(Bool);
    __DOCUMENT_INTRINSIC("Returns true if the bit in mask at index is 1.");

    __MAKE_BUILTIN(subgroupBallotFirstOne, SubgroupBallotFirstOne);        // Get first bit set to 1
    __ADD_ARG_LIT(mask, UInt4);
    __SET_RET_LIT(UInt);
    __DOCUMENT_INTRINSIC("Get the first one (ctz) in a subgroup thread mask.");

    __MAKE_BUILTIN(subgroupBallotLastOne, SubgroupBallotLastOne);        // Get last bit set to 1
    __ADD_ARG_LIT(mask, UInt4);
    __SET_RET_LIT(UInt);
    __DOCUMENT_INTRINSIC("Get the first one (clz) in a subgroup thread mask.");

#define X(ty, index)\
    __MAKE_INTRINSIC(subgroupSwapDiagonal, SubgroupSwapDiagonal, ty)\
    __ADD_ARG(value, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);
    __DOCUMENT_INTRINSIC("Swap subgroup values diagonally");

    SCALAR_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(subgroupSwapVertical, SubgroupSwapVertical, ty)\
    __ADD_ARG(value, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);
    __DOCUMENT_INTRINSIC("Swap subgroup values vertically");

    SCALAR_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(subgroupSwapHorizontal, SubgroupSwapHorizontal, ty)\
    __ADD_ARG(value, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);
    __DOCUMENT_INTRINSIC("Swap subgroup values horizontally");

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
    __ADD_VALUE_LIT(semantics, MemorySemanticsType);\
    __SET_RET_LIT(ty);
    
    SINGLE_COMPONENT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(atomicStore, AtomicStore, ty)\
    __ADD_POINTER_ARG_LIT(orig, ty);\
    __ADD_ARG_LIT(value, ty);\
    __ADD_VALUE_LIT(semantics, MemorySemanticsType);\
    __SET_RET_LIT(ty);

    SINGLE_COMPONENT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(atomicExchange, AtomicExchange, ty)\
    __ADD_POINTER_ARG_LIT(value, ty);\
    __ADD_ARG_LIT(exchange, ty);\
    __ADD_VALUE_LIT(semantics, MemorySemanticsType);\
    __SET_RET_LIT(ty);

    SINGLE_COMPONENT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(atomicCompareExchange, AtomicCompareExchange, ty)\
    __ADD_POINTER_ARG_LIT(value, ty);\
    __ADD_ARG_LIT(compare, ty);\
    __ADD_ARG_LIT(exchange, ty);\
    __ADD_VALUE_LIT(semantics, MemorySemanticsType);\
    __SET_RET_LIT(ty);

    INT_SINGLE_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(atomicIncrement, AtomicIncrement, ty)\
    __ADD_POINTER_ARG_LIT(value, ty);\
    __ADD_VALUE_LIT(semantics, MemorySemanticsType);\
    __SET_RET_LIT(ty);

    INT_SINGLE_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(atomicDecrement, AtomicDecrement, ty)\
    __ADD_POINTER_ARG_LIT(value, ty);\
    __ADD_VALUE_LIT(semantics, MemorySemanticsType);\
    __SET_RET_LIT(ty);

    INT_SINGLE_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(atomicAdd, AtomicAdd, ty)\
    __ADD_POINTER_ARG_LIT(value, ty);\
    __ADD_ARG_LIT(addend, ty);\
    __ADD_VALUE_LIT(semantics, MemorySemanticsType);\
    __SET_RET_LIT(ty);

    INT_SINGLE_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(atomicSub, AtomicSub, ty)\
    __ADD_POINTER_ARG_LIT(value, ty);\
    __ADD_ARG_LIT(addend, ty);\
    __ADD_VALUE_LIT(semantics, MemorySemanticsType);\
    __SET_RET_LIT(ty);

    INT_SINGLE_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(atomicMin, AtomicMin, ty)\
    __ADD_POINTER_ARG_LIT(value1, ty);\
    __ADD_ARG_LIT(value2, ty);\
    __ADD_VALUE_LIT(semantics, MemorySemanticsType);\
    __SET_RET_LIT(ty);

    INT_SINGLE_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(atomicMax, AtomicMax, ty)\
    __ADD_POINTER_ARG_LIT(value1, ty);\
    __ADD_ARG_LIT(value2, ty);\
    __ADD_VALUE_LIT(semantics, MemorySemanticsType);\
    __SET_RET_LIT(ty);

    INT_SINGLE_LIST
#undef X
    
#define X(ty, index)\
    __MAKE_INTRINSIC(atomicAnd, AtomicAnd, ty)\
    __ADD_POINTER_ARG_LIT(value1, ty);\
    __ADD_ARG_LIT(value2, ty);\
    __ADD_VALUE_LIT(semantics, MemorySemanticsType);\
    __SET_RET_LIT(ty);

    INT_SINGLE_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(atomicOr, AtomicOr, ty)\
    __ADD_POINTER_ARG_LIT(value1, ty);\
    __ADD_ARG_LIT(value2, ty);\
    __ADD_VALUE_LIT(semantics, MemorySemanticsType);\
    __SET_RET_LIT(ty);

    INT_SINGLE_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(atomicXor, AtomicXor, ty)\
    __ADD_POINTER_ARG_LIT(value1, ty);\
    __ADD_ARG_LIT(value2, ty);\
    __ADD_VALUE_LIT(semantics, MemorySemanticsType);\
    __SET_RET_LIT(ty);

    INT_SINGLE_LIST
#undef X
    
//------------------------------------------------------------------------------
/**
    Bit intrinsics
*/
//------------------------------------------------------------------------------
    __MAKE_BUILTIN(bitInsert, BitInsert);
    __ADD_ARG_LIT(base, UInt);
    __ADD_ARG_LIT(insert, UInt);
    __ADD_ARG_LIT(offset, UInt);
    __ADD_ARG_LIT(count, UInt);
    __SET_RET_LIT(UInt);

    __MAKE_BUILTIN(bitExtract, BitUExtract);
    __ADD_ARG_LIT(base, UInt);
    __ADD_ARG_LIT(offset, UInt);
    __ADD_ARG_LIT(count, UInt);
    __SET_RET_LIT(UInt);

    __MAKE_BUILTIN(bitExtract, BitSExtract);
    __ADD_ARG_LIT(base, Int);
    __ADD_ARG_LIT(offset, Int);
    __ADD_ARG_LIT(count, Int);
    __SET_RET_LIT(Int);

    __MAKE_BUILTIN(bitReverse, BitReverse);
    __ADD_ARG_LIT(base, UInt);
    __SET_RET_LIT(UInt);

    __MAKE_BUILTIN(bitCount, BitCount);
    __ADD_ARG_LIT(base, UInt);
    __SET_RET_LIT(UInt);

//------------------------------------------------------------------------------
/**
    Barrier intrinsics
*/
//------------------------------------------------------------------------------

    __MAKE_BUILTIN(executionBarrier, ExecutionBarrier);
    __SET_RET_LIT(Void);
    __DOCUMENT_INTRINSIC("Synchronizes execution of all threads across all levels to this point, ensuring any previous instructions have finished.")

    __MAKE_BUILTIN(executionBarrierSubgroup, ExecutionBarrierSubgroup);
    __SET_RET_LIT(Void);
    __DOCUMENT_INTRINSIC("Synchronizes execution of all threads within a subgroup to this point, ensuring any previous instructions have finished.")

    __MAKE_BUILTIN(executionBarrierWorkgroup, ExecutionBarrierWorkgroup);
    __SET_RET_LIT(Void);
    __DOCUMENT_INTRINSIC("Synchronizes execution of all threads within a workgroup to this point, ensuring any previous instructions have finished.")

    __MAKE_BUILTIN(memoryBarrier, MemoryExecutionBarrier);
    __SET_RET_LIT(Void);
    __DOCUMENT_INTRINSIC("Ensures all memory writes across all levels finish before this barrier.")

    __MAKE_BUILTIN(memoryBarrierBuffer, MemoryExecutionBarrierBuffer);
    __SET_RET_LIT(Void);
    __DOCUMENT_INTRINSIC("Ensures all buffer writes across all levels finish before this barrier.")

    __MAKE_BUILTIN(memoryBarrierTexture, MemoryExecutionBarrierTexture);
    __SET_RET_LIT(Void);
    __DOCUMENT_INTRINSIC("Ensures all texture writes across all levels finish before this barrier.")

    __MAKE_BUILTIN(memoryBarrierAtomic, MemoryExecutionBarrierAtomic);
    __SET_RET_LIT(Void);
    __DOCUMENT_INTRINSIC("Ensures all atomic writes across all levels finish before this barrier.")

    __MAKE_BUILTIN(memoryBarrierSubgroup, MemoryExecutionBarrierSubgroup);
    __SET_RET_LIT(Void);
    __DOCUMENT_INTRINSIC("Ensures all memory writes within the subgroup finish before this barrier.")
    
    __MAKE_BUILTIN(memoryBarrierWorkgroup, MemoryExecutionBarrierWorkgroup);
    __SET_RET_LIT(Void);
    __DOCUMENT_INTRINSIC("Ensures all memory writes within the workgroup finish before this barrier.")


    //------------------------------------------------------------------------------
    /**
        Texture intrinsics
    */
    //------------------------------------------------------------------------------

    static Type* intCoordinates[] =
    {
        &IntType
        , &Int2Type
        , &Int2Type
        , &Int3Type
        , &Int3Type
        , &Int2Type
        , &Int3Type
        , &Int3Type
        , &Int4Type
    };

    static Type* dimensionality[] =
    {
        &UIntType,
        &UInt2Type,
        &UInt2Type,
        &UInt3Type,
        &UInt2Type,
        &UInt2Type,
        &UInt3Type,
        &UInt3Type,
        &UInt4Type
    };


#define STRINGIFY(x) #x

#define __MAKE_TEXTURE_STORE_LOAD_INTRINSIC(ty, variant, overload)\
newIntrinsic = StaticAlloc<Function>(); \
newIntrinsic->name = ConstantString(STRINGIFY(texture##ty##variant)); \
Intrinsics::Texture##ty##variant##_##overload = newIntrinsic;\
Intrinsics.Append(newIntrinsic);\
arguments.Clear();\
__ADD_HANDLE_ARG_LIT(texture, overload);\

#define __MAKE_TEXTURE_STORE_LOAD_INTRINSIC_BASE(ty, overload)\
newIntrinsic = StaticAlloc<Function>(); \
newIntrinsic->name = ConstantString(STRINGIFY(texture##ty)); \
Intrinsics::Texture##ty##Base_##overload = newIntrinsic;\
Intrinsics.Append(newIntrinsic);\
arguments.Clear();\
__ADD_HANDLE_ARG_LIT(texture, overload);\

#define __MAKE_TEXTURE_STORE_LOAD_INTRINSIC_MUT(ty, variant, overload)\
newIntrinsic = StaticAlloc<Function>(); \
newIntrinsic->name = ConstantString(STRINGIFY(texture##ty##variant)); \
Intrinsics::Texture##ty##variant##_##overload = newIntrinsic;\
Intrinsics.Append(newIntrinsic);\
arguments.Clear();\
__ADD_HANDLE_ARG_LIT_MUT(texture, overload);\

#define __MAKE_TEXTURE_STORE_LOAD_INTRINSIC_BASE_MUT(ty, overload)\
newIntrinsic = StaticAlloc<Function>(); \
newIntrinsic->name = ConstantString(STRINGIFY(texture##ty)); \
Intrinsics::Texture##ty##Base_##overload = newIntrinsic;\
Intrinsics.Append(newIntrinsic);\
arguments.Clear();\
__ADD_HANDLE_ARG_LIT_MUT(texture, overload);\

#define __MAKE_TEXTURE_QUERY_INTRINSIC(ty, overload, index)\
newIntrinsic = StaticAlloc<Function>(); \
newIntrinsic->name = ConstantString(STRINGIFY(texture##ty)); \
Intrinsics::Texture##ty##_##overload = newIntrinsic;\
Intrinsics.Append(newIntrinsic);\
arguments.Clear();\
__ADD_HANDLE_ARG_LIT(texture, overload);\

#define __MAKE_TEXTURE_QUERY_INTRINSIC_LIT(ty, overload)\
newIntrinsic = StaticAlloc<Function>(); \
newIntrinsic->name = ConstantString(STRINGIFY(texture##ty)); \
Intrinsics::Texture##ty##_##overload = newIntrinsic;\
Intrinsics.Append(newIntrinsic);\
arguments.Clear();\
__ADD_HANDLE_ARG_LIT(texture, overload);\

#define __MAKE_SAMPLEDTEXTURE_QUERY_INTRINSIC_LIT(ty, overload)\
newIntrinsic = StaticAlloc<Function>(); \
newIntrinsic->name = ConstantString(STRINGIFY(texture##ty)); \
Intrinsics::SampledTexture##ty##_##overload = newIntrinsic;\
Intrinsics.Append(newIntrinsic);\
arguments.Clear();\
__ADD_SAMPLED_HANDLE_ARG_LIT(texture, overload);\

#define __MAKE_TEXTURE_INTRINSIC(ty, variant, overload)\
newIntrinsic = StaticAlloc<Function>(); \
newIntrinsic->name = ConstantString(STRINGIFY(texture##ty##variant)); \
Intrinsics::Texture##ty##variant##_##overload = newIntrinsic;\
Intrinsics.Append(newIntrinsic);\
arguments.Clear();\
__ADD_HANDLE_ARG_LIT(texture, overload);\
__ADD_HANDLE_ARG_LIT(sampler, SamplerType);

#define __MAKE_SAMPLEDTEXTURE_INTRINSIC(ty, variant, overload)\
newIntrinsic = StaticAlloc<Function>(); \
newIntrinsic->name = ConstantString(STRINGIFY(texture##ty##variant)); \
Intrinsics::SampledTexture##ty##variant##_##overload = newIntrinsic;\
Intrinsics.Append(newIntrinsic);\
arguments.Clear();\
__ADD_SAMPLED_HANDLE_ARG_LIT(texture, overload);\

#define __MAKE_TEXTURE_INTRINSIC_BASE(ty, overload)\
newIntrinsic = StaticAlloc<Function>(); \
newIntrinsic->name = ConstantString(STRINGIFY(texture##ty)); \
Intrinsics::Texture##ty##Base_##overload = newIntrinsic;\
Intrinsics.Append(newIntrinsic);\
arguments.Clear();\
__ADD_HANDLE_ARG_LIT(texture, overload);\
__ADD_HANDLE_ARG_LIT(sampler, SamplerType);

#define __MAKE_SAMPLEDTEXTURE_INTRINSIC_BASE(ty, overload)\
newIntrinsic = StaticAlloc<Function>(); \
newIntrinsic->name = ConstantString(STRINGIFY(texture##ty)); \
Intrinsics::SampledTexture##ty##Base_##overload = newIntrinsic;\
Intrinsics.Append(newIntrinsic);\
arguments.Clear();\
__ADD_SAMPLED_HANDLE_ARG_LIT(texture, overload);\

#define X(type, index)\
    __MAKE_TEXTURE_QUERY_INTRINSIC(GetSize, type, index)\
    __SET_RET(dimensionality[index])\
    __DOCUMENT_INTRINSIC("Retrieve the size of this texture.")

    TEXTURE_INTRINSIC_NO_MS_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_QUERY_INTRINSIC(GetSizeMip, type, index)\
    __ADD_ARG_LIT(mip, Float);\
    __SET_RET(dimensionality[index])\
    __DOCUMENT_INTRINSIC("Retrieve the size of this texture for a specific mip.")

    TEXTURE_INTRINSIC_NO_MS_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_QUERY_INTRINSIC_LIT(GetSampledMip, type)\
    __ADD_HANDLE_ARG_LIT(sampler, SamplerType);\
    __ADD_ARG(coords, intCoordinates[index]);\
    __SET_RET_LIT(Float)\
    __MAKE_SAMPLEDTEXTURE_QUERY_INTRINSIC_LIT(GetSampledMip, type)\
    __ADD_ARG(coords, intCoordinates[index]);\
    __SET_RET_LIT(Float)
    
    TEXTURE_INTRINSIC_NO_MS_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_QUERY_INTRINSIC_LIT(GetMips, type)\
    __SET_RET_LIT(UInt)\
    __DOCUMENT_INTRINSIC("Retrieve the number of mips for this texture.")
        
    TEXTURE_INTRINSIC_NO_MS_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_QUERY_INTRINSIC_LIT(GetSamples, type)\
    __SET_RET_LIT(UInt)\
    __DOCUMENT_INTRINSIC("Retrieve the number of multisampling samples for this texture.")
            
    TEXTURE_INTRINSIC_ONLY_MS_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_STORE_LOAD_INTRINSIC_BASE_MUT(Load, type)\
    __ADD_ARG(coords, intCoordinates[index]);\
    __SET_RET_LIT(Float4)\
    __DOCUMENT_INTRINSIC("Load texel at pixel index without filtering.")

    TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_STORE_LOAD_INTRINSIC_MUT(Load, Mip, type)\
    __ADD_ARG(coords, intCoordinates[index]);\
    __ADD_ARG_LIT(lod, Int);\
    __SET_RET_LIT(Float4)\
    __DOCUMENT_INTRINSIC("Load texel at pixel index for specific mip without filtering.")

    TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_STORE_LOAD_INTRINSIC_BASE_MUT(Store, type)\
    __ADD_ARG(coords, intCoordinates[index]);\
    __ADD_ARG_LIT(value, Float4);\
    __SET_RET_LIT(Float4)\
    __DOCUMENT_INTRINSIC("Store texel at pixel index.")

    TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_STORE_LOAD_INTRINSIC_MUT(Store, Mip, type)\
    __ADD_ARG(coords, intCoordinates[index]);\
    __ADD_ARG_LIT(lod, Int);\
    __ADD_ARG_LIT(value, Float4);\
    __SET_RET_LIT(Float4)\
    __DOCUMENT_INTRINSIC("Store texel at pixel index for a specific mip.")

    TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_STORE_LOAD_INTRINSIC_BASE(Fetch, type)\
    __ADD_ARG(coords, intCoordinates[index]);\
    __ADD_ARG_LIT(lod, Int);\
    __SET_RET_LIT(Float4)\
    __DOCUMENT_INTRINSIC("Load texel at pixel index for specific mip without filtering.")

    TEXTURE_INTRINSIC_NO_CUBE_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_STORE_LOAD_INTRINSIC(Fetch, Sample, type)\
    __ADD_ARG(coords, intCoordinates[index]);\
    __ADD_ARG_LIT(lod, Int);\
    __ADD_ARG_LIT(sample, Int);\
    __SET_RET_LIT(Float4)\
    __DOCUMENT_INTRINSIC("Load texel at pixel index for specific mip and sample without filtering.")

    TEXTURE_INTRINSIC_ONLY_MS_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_STORE_LOAD_INTRINSIC_BASE(Gather, type)\
    __ADD_ARG(coords, intCoordinates[index]);\
    __ADD_ARG_LIT(channel, Int);\
    __SET_RET_LIT(Float4)\
    __DOCUMENT_INTRINSIC("Load the 4 values corresponding to a texel quad at a given position and channel.")

    TEXTURE_INTRINSIC_GATHER_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_STORE_LOAD_INTRINSIC(Gather, Offsets, type)\
    __ADD_ARG(coords, intCoordinates[index]);\
    __ADD_ARG_LIT(lod, Int);\
    __ADD_ARG_ARR_LIT(offsets, Int2, 4);\
    __SET_RET_LIT(Float4)

    TEXTURE_INTRINSIC_2D_LIST
#undef X


    __MAKE_BUILTIN(pixelCacheLoad, PixelCacheLoad);
    __ADD_HANDLE_ARG_LIT(texture, PixelCacheType);
    __SET_RET_LIT(Float4);

    __MAKE_BUILTIN(pixelCacheLoad, PixelCacheLoad);
    __ADD_HANDLE_ARG_LIT(texture, PixelCacheMS);
    __SET_RET_LIT(Float4);

    static Type* normCoordinates[] =
    {
        &FloatType     // 1D
        , &Float2Type  // 2D
        , &Float2Type  // 2DMS
        , &Float3Type  // 3D
        , &Float3Type  // Cube
        , &Float2Type  // 1D + array slice
        , &Float3Type  // 2D + array slice
        , &Float3Type  // 2D + array slice
        , &Float4Type  // 3D + array slice
    };

    static Type* projCoordinates[] =
    {
        &Float2Type    // 1D
        , &Float3Type  // 2D
        , &Float3Type  // 2DMS
        , &Float4Type  // 3D
        , &Float4Type  // Cube
        , &Float3Type  // 1D + array slice
        , &Float4Type  // 2D + array slice
        , &Float4Type  // 2D + array slice
        , nullptr  // 3D + array slice // invalid
    };

    static Type* offsets[] =
    {
        &IntType     // 1D
        , &Int2Type  // 2D
        , &Int2Type  // 2DMS
        , &Int3Type  // 3D
        , &Int3Type  // Cube
        , &IntType   // 1DArray - array slice
        , &Int2Type  // 2DArray - array slice
        , &Int2Type  // 2DArray - array slice
        , &Int3Type  // 3DArray - array slice
    };

    static Type* gradients[] =
    {
        &FloatType     // 1D
        , &Float2Type  // 2D
        , &Float2Type  // 2DMS
        , &Float3Type  // 3D
        , &Float3Type  // Cube
        , &FloatType   // 1DArray - array slice
        , &Float2Type  // 2DArray - array slice
        , &Float2Type  // 2DArray - array slice
        , &Float3Type  // 3DArray - array slice
    };

#define X(type, index)\
{\
    __MAKE_TEXTURE_INTRINSIC_BASE(Sample, type)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __SET_RET_LIT(Float4)\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC_BASE(Sample, type)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __SET_RET_LIT(Float4)\
}

    TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(type, index)\
{\
    __MAKE_TEXTURE_INTRINSIC(Sample, Offset, type)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG(offsets, offsets[index]);\
    __SET_RET_LIT(Float4)\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, Offset, type)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG(offsets, offsets[index]);\
    __SET_RET_LIT(Float4)\
}

    TEXTURE_INTRINSIC_NO_CUBE_LIST
#undef X

#define X(type, index)\
{\
    __MAKE_TEXTURE_INTRINSIC(Sample, Bias, type)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(bias, Float);\
    __SET_RET_LIT(Float4)\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, Bias, type)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(bias, Float);\
    __SET_RET_LIT(Float4)\
}

    TEXTURE_INTRINSIC_NO_MS_LIST
#undef X

#define X(type, index)\
{\
    __MAKE_TEXTURE_INTRINSIC(Sample, BiasCompare, type)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(bias, Float);\
    __ADD_ARG_LIT(compare, Float);\
    __SET_RET_LIT(Float)\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, BiasCompare, type)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(bias, Float);\
    __ADD_ARG_LIT(compare, Float);\
    __SET_RET_LIT(Float)\
}

    TEXTURE_INTRINSIC_NO_MS_LIST
#undef X

#define X(type, index)\
{\
    __MAKE_TEXTURE_INTRINSIC(Sample, BiasOffset, type)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(bias, Float);\
    __ADD_ARG(offsets, offsets[index]);\
    __SET_RET_LIT(Float4)\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, BiasOffset, type)\
    __ADD_ARG(coords, normCoordinates[index]); \
    __ADD_ARG_LIT(bias, Float); \
    __ADD_ARG(offsets, offsets[index]);\
    __SET_RET_LIT(Float4)\
}

    TEXTURE_INTRINSIC_NO_MS_LIST
#undef X

#define X(type, index)\
{\
    __MAKE_TEXTURE_INTRINSIC(Sample, BiasProj, type)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(bias, Float);\
    __ADD_ARG_LIT(proj, Float);\
    __SET_RET_LIT(Float4)\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, BiasProj, type)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(bias, Float);\
    __ADD_ARG_LIT(proj, Float);\
    __SET_RET_LIT(Float4)\
}

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
{\
    __MAKE_TEXTURE_INTRINSIC(Sample, BiasProjCompare, type)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(bias, Float);\
    __ADD_ARG_LIT(proj, Float);\
    __ADD_ARG_LIT(compare, Float);\
    __SET_RET_LIT(Float)\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, BiasProjCompare, type)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(bias, Float);\
    __ADD_ARG_LIT(proj, Float);\
    __ADD_ARG_LIT(compare, Float);\
    __SET_RET_LIT(Float)\
}

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
{\
    __MAKE_TEXTURE_INTRINSIC(Sample, BiasProjOffset, type)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(bias, Float);\
    __ADD_ARG_LIT(proj, Float);\
    __ADD_ARG(offsets, offsets[index]);\
    __SET_RET_LIT(Float4)\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, BiasProjOffset, type)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(bias, Float);\
    __ADD_ARG_LIT(proj, Float);\
    __ADD_ARG(offsets, offsets[index]);\
    __SET_RET_LIT(Float4)\
}

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
{\
    __MAKE_TEXTURE_INTRINSIC(Sample, Compare, type)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(compare, Float);\
    __SET_RET_LIT(Float)\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, Compare, type)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(compare, Float);\
    __SET_RET_LIT(Float4)\
}

    TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(type, index)\
{\
    __MAKE_TEXTURE_INTRINSIC(Sample, CompareOffset, type)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(compare, Float);\
    __ADD_ARG(offsets, offsets[index]);\
    __SET_RET_LIT(Float)\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, CompareOffset, type)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(compare, Float);\
    __ADD_ARG(offsets, offsets[index]);\
    __SET_RET_LIT(Float)\
}

    TEXTURE_INTRINSIC_NO_CUBE_LIST
#undef X

#define X(type, index)\
{\
    __MAKE_TEXTURE_INTRINSIC(Sample, Grad, type)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG(gradX, gradients[index]);\
    __ADD_ARG(gradY, gradients[index]);\
    __SET_RET_LIT(Float4)\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, Grad, type)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG(gradX, gradients[index]);\
    __ADD_ARG(gradY, gradients[index]);\
    __SET_RET_LIT(Float4)\
}

    TEXTURE_INTRINSIC_NO_MS_LIST
#undef X

#define X(type, index)\
{\
    __MAKE_TEXTURE_INTRINSIC(Sample, GradCompare, type)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG(gradX, gradients[index]);\
    __ADD_ARG(gradY, gradients[index]);\
    __ADD_ARG_LIT(compare, Float);\
    __SET_RET_LIT(Float)\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, GradCompare, type)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG(gradX, gradients[index]);\
    __ADD_ARG(gradY, gradients[index]);\
    __ADD_ARG_LIT(compare, Float);\
    __SET_RET_LIT(Float)\
}

    TEXTURE_INTRINSIC_NO_CUBE_MS_LIST
#undef X

#define X(type, index)\
{\
    __MAKE_TEXTURE_INTRINSIC(Sample, GradOffset, type)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG(gradX, gradients[index]);\
    __ADD_ARG(gradY, gradients[index]);\
    __ADD_ARG(offsets, offsets[index]);\
    __SET_RET_LIT(Float4)\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, GradOffset, type)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG(gradX, gradients[index]);\
    __ADD_ARG(gradY, gradients[index]);\
    __ADD_ARG(offsets, offsets[index]);\
    __SET_RET_LIT(Float4)\
}

    TEXTURE_INTRINSIC_NO_CUBE_MS_LIST
#undef X

#define X(type, index)\
{\
    __MAKE_TEXTURE_INTRINSIC(Sample, GradProj, type)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG(gradX, gradients[index]);\
    __ADD_ARG(gradY, gradients[index]);\
    __ADD_ARG_LIT(proj, Float);\
    __SET_RET_LIT(Float4)\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, GradProj, type)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG(gradX, gradients[index]);\
    __ADD_ARG(gradY, gradients[index]);\
    __ADD_ARG_LIT(proj, Float);\
    __SET_RET_LIT(Float4)\
}

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
{\
    __MAKE_TEXTURE_INTRINSIC(Sample, GradProjCompare, type)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG(gradX, gradients[index]);\
    __ADD_ARG(gradY, gradients[index]);\
    __ADD_ARG_LIT(proj, Float);\
    __ADD_ARG_LIT(compare, Float);\
    __SET_RET_LIT(Float)\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, GradProjCompare, type)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG(gradX, gradients[index]);\
    __ADD_ARG(gradY, gradients[index]);\
    __ADD_ARG_LIT(proj, Float);\
    __ADD_ARG_LIT(compare, Float);\
    __SET_RET_LIT(Float)\
}

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
{\
    __MAKE_TEXTURE_INTRINSIC(Sample, GradProjCompareOffset, type)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG(gradX, gradients[index]);\
    __ADD_ARG(gradY, gradients[index]);\
    __ADD_ARG_LIT(proj, Float);\
    __ADD_ARG_LIT(compare, Float);\
    __ADD_ARG(offsets, offsets[index]);\
    __SET_RET_LIT(Float)\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, GradProjCompareOffset, type)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG(gradX, gradients[index]);\
    __ADD_ARG(gradY, gradients[index]);\
    __ADD_ARG_LIT(proj, Float);\
    __ADD_ARG_LIT(compare, Float);\
    __ADD_ARG(offsets, offsets[index]);\
    __SET_RET_LIT(Float)\
}

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
{\
    __MAKE_TEXTURE_INTRINSIC(Sample, GradProjOffset, type)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG(gradX, gradients[index]);\
    __ADD_ARG(gradY, gradients[index]);\
    __ADD_ARG_LIT(proj, Float);\
    __ADD_ARG(offsets, offsets[index]);\
    __SET_RET_LIT(Float4)\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, GradProjOffset, type)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG(gradX, gradients[index]);\
    __ADD_ARG(gradY, gradients[index]);\
    __ADD_ARG_LIT(proj, Float);\
    __ADD_ARG(offsets, offsets[index]);\
    __SET_RET_LIT(Float4)\
}

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
{\
    __MAKE_TEXTURE_INTRINSIC(Sample, Lod, type)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(lod, Float);\
    __SET_RET_LIT(Float4)\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, Lod, type)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(lod, Float);\
    __SET_RET_LIT(Float4)\
}

    TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(type, index)\
{\
    __MAKE_TEXTURE_INTRINSIC(Sample, LodCompare, type)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(lod, Float);\
    __ADD_ARG_LIT(compare, Float);\
    __SET_RET_LIT(Float)\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, LodCompare, type)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(lod, Float);\
    __ADD_ARG_LIT(compare, Float);\
    __SET_RET_LIT(Float)\
}

    TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(type, index)\
{\
    __MAKE_TEXTURE_INTRINSIC(Sample, LodCompareOffset, type)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(lod, Float);\
    __ADD_ARG_LIT(compare, Float);\
    __ADD_ARG(offsets, offsets[index]);\
    __SET_RET_LIT(Float)\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, LodCompareOffset, type)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(lod, Float);\
    __ADD_ARG_LIT(compare, Float);\
    __ADD_ARG(offsets, offsets[index]);\
    __SET_RET_LIT(Float)\
}

    TEXTURE_INTRINSIC_NO_CUBE_LIST
#undef X

#define X(type, index)\
{\
    __MAKE_TEXTURE_INTRINSIC(Sample, LodOffset, type)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(lod, Float);\
    __ADD_ARG(offsets, offsets[index]);\
    __SET_RET_LIT(Float4)\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, LodOffset, type)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(lod, Float);\
    __ADD_ARG(offsets, offsets[index]);\
    __SET_RET_LIT(Float4)\
}

    TEXTURE_INTRINSIC_NO_CUBE_LIST
#undef X

#define X(type, index)\
{\
    __MAKE_TEXTURE_INTRINSIC(Sample, LodProj, type)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(lod, Float);\
    __SET_RET_LIT(Float4)\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, LodProj, type)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(lod, Float);\
    __SET_RET_LIT(Float4)\
}

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
{\
    __MAKE_TEXTURE_INTRINSIC(Sample, LodProjCompare, type)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(lod, Float);\
    __ADD_ARG_LIT(compare, Float);\
    __SET_RET_LIT(Float)\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, LodProjCompare, type)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(lod, Float);\
    __ADD_ARG_LIT(compare, Float);\
    __SET_RET_LIT(Float)\
}

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
{\
    __MAKE_TEXTURE_INTRINSIC(Sample, LodProjCompareOffset, type)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(lod, Float);\
    __ADD_ARG_LIT(compare, Float);\
    __ADD_ARG(offsets, offsets[index]);\
    __SET_RET_LIT(Float)\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, LodProjCompareOffset, type)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(lod, Float);\
    __ADD_ARG_LIT(compare, Float);\
    __ADD_ARG(offsets, offsets[index]);\
    __SET_RET_LIT(Float)\
}

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
{\
    __MAKE_TEXTURE_INTRINSIC(Sample, LodProjOffset, type)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(lod, Float);\
    __ADD_ARG(offsets, offsets[index]);\
    __SET_RET_LIT(Float4)\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, LodProjOffset, type)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(lod, Float);\
    __ADD_ARG(offsets, offsets[index]);\
    __SET_RET_LIT(Float4)\
}

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
{\
    __MAKE_TEXTURE_INTRINSIC(Sample, Proj, type)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __SET_RET_LIT(Float4)\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, Proj, type)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __SET_RET_LIT(Float4)\
}

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
{\
    __MAKE_TEXTURE_INTRINSIC(Sample, ProjCompare, type)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(compare, Float);\
    __SET_RET_LIT(Float)\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, ProjCompare, type)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(compare, Float);\
    __SET_RET_LIT(Float)\
}

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
{\
    __MAKE_TEXTURE_INTRINSIC(Sample, ProjCompareOffset, type)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(compare, Float);\
    __ADD_ARG(offsets, offsets[index]);\
    __SET_RET_LIT(Float)\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, ProjCompareOffset, type)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(compare, Float);\
    __ADD_ARG(offsets, offsets[index]);\
    __SET_RET_LIT(Float)\
}

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
{\
    __MAKE_TEXTURE_INTRINSIC(Sample, ProjOffset, type)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG(offsets, offsets[index]);\
    __SET_RET_LIT(Float4)\
    __MAKE_SAMPLEDTEXTURE_INTRINSIC(Sample, ProjOffset, type)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG(offsets, offsets[index]);\
    __SET_RET_LIT(Float4)\
}

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X
    
    //------------------------------------------------------------------------------
    /**
        Raytracing intrinsics
    */
    //------------------------------------------------------------------------------
    __MAKE_BUILTIN(rayTrace, TraceRay);
    __ADD_HANDLE_ARG_LIT(tlas, AccelerationStructureType);
    __ADD_ARG_LIT(origin, Float3);
    __ADD_ARG_LIT(direction, Float3);
    __ADD_ARG_LIT(min, Float);
    __ADD_ARG_LIT(max, Float);
    __ADD_ARG_LIT(shaderTableOffset, UInt);
    __ADD_ARG_LIT(shaderTableStride, UInt);
    __ADD_ARG_LIT(missShaderIndex, UInt);
    __ADD_ARG_LIT(flags, UInt);
    __ADD_ARG_LIT(mask, UInt);
    __SET_RET_LIT(Void)

    __MAKE_BUILTIN(rayExportIntersection, ExportRayIntersection);
    __SET_RET_LIT(Void);

    __MAKE_BUILTIN(rayExecuteCallable, ExecuteCallable);
    __SET_RET_LIT(Void);

    __MAKE_BUILTIN(rayGetLaunchIndex, RayLaunchIndex);
    __SET_RET_LIT(UInt3);

    __MAKE_BUILTIN(rayGetLaunchSize, RayLaunchSize);
    __SET_RET_LIT(UInt3);

    __MAKE_BUILTIN(BLASGetPrimitiveIndex, BLASPrimitiveIndex);
    __SET_RET_LIT(UInt);

    __MAKE_BUILTIN(BLASGetGeometryIndex, BLASGeometryIndex);
    __SET_RET_LIT(UInt);

    __MAKE_BUILTIN(TLASGetInstanceIndex, TLASInstanceIndex);
    __SET_RET_LIT(UInt);

    __MAKE_BUILTIN(TLASGetInstanceCustomIndex, TLASInstanceCustomIndex);
    __SET_RET_LIT(UInt);

    __MAKE_BUILTIN(rayGetWorldOrigin, RayWorldOrigin);
    __SET_RET_LIT(Float3);
    
    __MAKE_BUILTIN(rayGetWorldDirection, RayWorldDirection);
    __SET_RET_LIT(Float3);

    __MAKE_BUILTIN(rayGetObjectOrigin, RayObjectOrigin);
    __SET_RET_LIT(Float3);

    __MAKE_BUILTIN(rayGetObjectDirection, RayObjectDirection);
    __SET_RET_LIT(Float3);

    __MAKE_BUILTIN(rayGetMin, RayMin);
    __SET_RET_LIT(Float);

    __MAKE_BUILTIN(rayGetMax, RayMax);
    __SET_RET_LIT(Float);

    __MAKE_BUILTIN(rayGetFlags, RayFlags);
    __SET_RET_LIT(UInt);

    __MAKE_BUILTIN(rayGetHitDistance, RayHitDistance);
    __SET_RET_LIT(Float);

    __MAKE_BUILTIN(rayGetHitKind, RayHitKind);
    __SET_RET_LIT(UInt);

    __MAKE_BUILTIN(TLASGetObjectToWorld, TLASObjectToWorld);
    __SET_RET_LIT(Mat4x3);
    
    __MAKE_BUILTIN(TLASGetWorldToObject, TLASWorldToObject);
    __SET_RET_LIT(Mat4x3);
    
    DefaultIntrinsics = StaticArray(Intrinsics);

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

} // namespace GPULang
