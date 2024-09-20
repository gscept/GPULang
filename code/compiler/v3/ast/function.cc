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
#include "v3/ast/expressions/symbolexpression.h"
#include "v3/ast/expressions/unaryexpression.h"

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Function::Function()
{
    this->symbolType = FunctionType;
    this->resolved = new Function::__Resolved();
    this->hasBody = false;
    this->ast = nullptr;

    Function::__Resolved* typeResolved = static_cast<Function::__Resolved*>(this->resolved);
    typeResolved->computeShaderWorkGroupSize[0] = 1;
    typeResolved->computeShaderWorkGroupSize[1] = 1;
    typeResolved->computeShaderWorkGroupSize[2] = 1;
    typeResolved->earlyDepth = false;
    typeResolved->isShader = false;
    typeResolved->shaderUsage.bits = 0x0;
    typeResolved->invocations = Function::__Resolved::INVALID_SIZE;
    typeResolved->maxOutputVertices = Function::__Resolved::INVALID_SIZE;
    typeResolved->patchSize = Function::__Resolved::INVALID_SIZE;
    typeResolved->windingOrder = Function::__Resolved::InvalidWindingOrder;
    typeResolved->inputPrimitiveTopology = Function::__Resolved::InvalidPrimitiveTopology;
    typeResolved->outputPrimitiveTopology = Function::__Resolved::InvalidPrimitiveTopology;
    typeResolved->patchType = Function::__Resolved::InvalidPatchType;
    typeResolved->partitionMethod = Function::__Resolved::InvalidPartitionMethod;
    typeResolved->pixelOrigin = Function::__Resolved::Lower;
    typeResolved->isPrototype = false;
}

//------------------------------------------------------------------------------
/**
*/
Function::~Function()
{
    for (Variable* var : this->parameters)
        delete var;
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
                    Type* argType = compiler->GetSymbol<Type>(fun->parameters[i]->type.name);
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

#define __BEGIN_INTRINSICS__ std::vector<Symbol*> intrinsics; Function* newIntrinsic = nullptr; Variable* newVar = nullptr;

#define __MAKE_INTRINSIC(nm, opcode, ty)\
newIntrinsic = new Function();\
newIntrinsic->name = #nm;\
Intrinsics::opcode##_##ty = newIntrinsic;\
intrinsics.push_back(newIntrinsic);

#define __MAKE_TEXTURE_LOAD_STORE_INTRINSIC(nm, ty, variant, ret)\
newIntrinsic = new Function(); \
newIntrinsic->name = #nm; \
newIntrinsic->returnType = { #ret }; \
Intrinsics::Texture##ty##variant##_texture1D = newIntrinsic; \
intrinsics.push_back(newIntrinsic);

#define __ADD_ARG(nm, tp)\
newVar = new Variable();\
newVar->type = { tp };\
newVar->name = #nm;\
newIntrinsic->parameters.push_back(newVar);

#define __ADD_ARG_LIT(nm, tp)\
newVar = new Variable();\
newVar->type = { #tp };\
newVar->name = #nm;\
newIntrinsic->parameters.push_back(newVar);

#define __ADD_ARG_ARR_LIT(nm, tp, size)\
newVar = new Variable();\
newVar->type = Type::FullType{ #tp };\
newVar->type.AddModifier(Type::FullType::Modifier::ArrayLevel);\
newVar->type.UpdateValue(size);\
newVar->name = #nm;\
newIntrinsic->parameters.push_back(newVar);

#define __ADD_HANDLE_ARG(nm, tp)\
newVar = new Variable();\
newVar->type = Type::FullType{ tp };\
newVar->type.AddModifier(Type::FullType::Modifier::PointerLevel);\
newVar->name = #nm;\
newIntrinsic->parameters.push_back(newVar);

#define __ADD_HANDLE_ARG_LIT(nm, tp)\
newVar = new Variable();\
newVar->type = { #tp };\
newVar->type.AddModifier(Type::FullType::Modifier::PointerLevel);\
newVar->name = #nm;\
newIntrinsic->parameters.push_back(newVar);

#define __SET_RET_LIT(name)\
newIntrinsic->returnType = { #name };

#define __SET_RET(name)\
newIntrinsic->returnType = { name };

#define __END_INTRINSICS__ return intrinsics;

//------------------------------------------------------------------------------
/**
*/
std::vector<Symbol*> 
Function::SetupIntrinsics()
{
    __BEGIN_INTRINSICS__;


    std::string scalarArgs[] =
    {
        "i32"
        , "i32x2"
        , "i32x3"
        , "i32x4"
        , "u32"
        , "u32x2"
        , "u32x3"
        , "u32x4"
        , "f32"
        , "f32x2"
        , "f32x3"
        , "f32x4"
        , "b8"
        , "b8x2"
        , "b8x3"
        , "b8x4"
    };
    constexpr uint32_t numScalarArgs = sizeof(scalarArgs) / sizeof(std::string);

#define X(ty, index)\
    __MAKE_INTRINSIC(mad, Mad, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __ADD_ARG(y, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    SCALAR_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(dot, Dot, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __ADD_ARG(y, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    FLOAT_LIST
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
    __MAKE_INTRINSIC(ceil, Ceil, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __ADD_ARG(y, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    FLOAT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(floor, Floor, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __ADD_ARG(y, scalarArgs[index]);\
    __SET_RET(scalarArgs[index]);

    FLOAT_LIST
#undef X

#define X(ty, index)\
    __MAKE_INTRINSIC(fract, Fract, ty)\
    __ADD_ARG(x, scalarArgs[index]);\
    __ADD_ARG(y, scalarArgs[index]);\
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

    //------------------------------------------------------------------------------
    /**
        Subgroup intrinsics
    */
    //------------------------------------------------------------------------------

    __MAKE_INTRINSIC(subgroupId, SubgroupId);                       // Simply returns the ID of the subgroup
    __SET_RET_LIT(u32);

    __MAKE_INTRINSIC(subgroupFirstActive, SubgroupFirstActive);      // Returns true for the first active invocation in the subgroup, and false for all else
    __SET_RET_LIT(b8);

    __MAKE_INTRINSIC(subgroupFirst, SubgroupFirst);                 // Returns value from the invocation with the lowest invocation
    __ADD_ARG_LIT(x, u32);
    __SET_RET_LIT(u32);

    __MAKE_INTRINSIC(subgroupBallot, SubgroupBallot);               // Creates lane mask with bit set to 1 if value in predicate is true
    __ADD_ARG_LIT(predicate, b8);
    __SET_RET_LIT(u32x4);

    __MAKE_INTRINSIC(subgroupInverseBallot, SubgroupInverseBallot);    // Creates an inverted lane mask with bit set to 1 if predicate is false
    __ADD_ARG_LIT(predicate, b8);
    __SET_RET_LIT(u32x4);

    __MAKE_INTRINSIC(subgroupBallotBitCount, SubgroupBallotBitCount);   // Counts bits set in ballot result up to thread index within subgroup
    __ADD_ARG_LIT(mask, u32x4);
    __SET_RET_LIT(u32);

    __MAKE_INTRINSIC(subgroupBallotBit, SubgroupBallotBit);        // Check if bit in ballot result is for a given index
    __ADD_ARG_LIT(mask, u32x4);
    __ADD_ARG_LIT(index, u32);
    __SET_RET_LIT(b8);

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

    std::string intCoordinates[] =
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

    //------------------------------------------------------------------------------
    /**
        Texture intrinsics
    */
    //------------------------------------------------------------------------------

#define STRINGIFY(x) #x

#define __MAKE_TEXTURE_STORE_LOAD_INTRINSIC(ty, variant, overload, ret)\
newIntrinsic = new Function(); \
newIntrinsic->name = STRINGIFY(texture##ty); \
newIntrinsic->returnType = { #ret }; \
Intrinsics::Texture##ty##variant##_##overload = newIntrinsic;\
intrinsics.push_back(newIntrinsic);\
__ADD_HANDLE_ARG_LIT(texture, overload);\

#define __MAKE_TEXTURE_INTRINSIC(ty, variant, overload, ret)\
newIntrinsic = new Function(); \
newIntrinsic->name = STRINGIFY(texture##ty); \
newIntrinsic->returnType = { #ret }; \
Intrinsics::Texture##ty##variant##_##overload = newIntrinsic;\
intrinsics.push_back(newIntrinsic);\
__ADD_HANDLE_ARG_LIT(texture, overload);\
__ADD_HANDLE_ARG_LIT(sampler, sampler);


#define X(type, index)\
    __MAKE_TEXTURE_STORE_LOAD_INTRINSIC(Load, Base, type, f32x4)\
    __ADD_ARG(coords, intCoordinates[index]);

    TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_STORE_LOAD_INTRINSIC(Load, Mip, type, f32x4)\
    __ADD_ARG(coords, intCoordinates[index]);\
    __ADD_ARG_LIT(lod, i32);

    TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_STORE_LOAD_INTRINSIC(Store, Base, type, f32x4)\
    __ADD_ARG(coords, intCoordinates[index]);\
    __ADD_ARG_LIT(value, f32x4);

    TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_STORE_LOAD_INTRINSIC(Store, Mip, type, f32x4)\
    __ADD_ARG(coords, intCoordinates[index]);\
    __ADD_ARG_LIT(lod, i32);\
    __ADD_ARG_LIT(value, f32x4);

    TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_STORE_LOAD_INTRINSIC(Fetch, Base, type, f32x4)\
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
    __MAKE_TEXTURE_STORE_LOAD_INTRINSIC(Gather, Base, type, f32x4)\
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


    __MAKE_INTRINSIC(pixelCacheLoad, PixelCacheLoad);
    __ADD_HANDLE_ARG_LIT(texture, pixelCache);
    __SET_RET_LIT(f32x4);

    __MAKE_INTRINSIC(pixelCacheLoad, PixelCacheLoad);
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
    __MAKE_TEXTURE_INTRINSIC(Sample, Base, type, f32x4)\
    __ADD_ARG(coords, normCoordinates[index]);\

    TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, Offset, type, f32x4)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG(offsets, offsets[index]);\

    TEXTURE_INTRINSIC_NO_CUBE_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, Bias, type, f32x4)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(bias, f32);\

    TEXTURE_INTRINSIC_NO_MS_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, BiasCompare, type, f32x4)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(bias, f32);\
    __ADD_ARG_LIT(compare, f32);\

    TEXTURE_INTRINSIC_NO_MS_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, BiasOffset, type, f32x4)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(bias, f32);\
    __ADD_ARG(offsets, offsets[index]);

    TEXTURE_INTRINSIC_NO_MS_LIST
#undef X


// These macros help to setup the different sampling functions based on requirements
#define OFFSET_REQUIREMENTS()\
if (textures[i].find_last_of("Cube") != std::string::npos)\
    continue;

#define BIAS_REQUIREMENTS()\
if (textures[i].find_last_of("MS") != std::string::npos)\
    continue;

#define GRAD_REQUIREMENTS() BIAS_REQUIREMENTS
#define LOD_REQUIREMENTS() BIAS_REQUIREMENTS

#define PROJ_REQUIREMENTS()\
if (textures[i].find_last_of("Array") != std::string::npos)\
    continue;\
BIAS_REQUIREMENTS\
OFFSET_REQUIREMENTS


#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, BiasProj, type, f32x4)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(bias, f32);\
    __ADD_ARG_LIT(proj, f32);

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, BiasProjCompare, type, f32x4)\
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
    __ADD_ARG(offsets, offsets[index]);

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, Compare, type, f32x4)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(compare, f32);

    TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, CompareOffset, type, f32x4)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(compare, f32);\
    __ADD_ARG(offsets, offsets[index]);

    TEXTURE_INTRINSIC_NO_CUBE_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, Grad, type, f32x4)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG(grad, gradients[index]);

    TEXTURE_INTRINSIC_NO_MS_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, GradCompare, type, f32x4)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG(grad, gradients[index]);\
    __ADD_ARG_LIT(compare, f32);

    TEXTURE_INTRINSIC_NO_CUBE_MS_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, GradOffset, type, f32x4)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG(grad, gradients[index]);\
    __ADD_ARG_LIT(compare, f32);\
    __ADD_ARG(offsets, offsets[index]);

    TEXTURE_INTRINSIC_NO_CUBE_MS_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, GradProj, type, f32x4)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG(grad, gradients[index]);\
    __ADD_ARG_LIT(proj, f32);

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, GradProjCompare, type, f32x4)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG(grad, gradients[index]);\
    __ADD_ARG_LIT(proj, f32);\
    __ADD_ARG_LIT(compare, f32);\

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, GradProjCompareOffset, type, f32x4)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG(grad, gradients[index]);\
    __ADD_ARG_LIT(proj, f32);\
    __ADD_ARG_LIT(compare, f32);\
    __ADD_ARG(offsets, offsets[index]);

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, GradProjOffset, type, f32x4)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG(grad, gradients[index]);\
    __ADD_ARG_LIT(proj, f32);\
    __ADD_ARG(offsets, offsets[index]);

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, Lod, type, f32x4)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(lod, f32);

    TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, LodCompare, type, f32x4)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(lod, f32);\
    __ADD_ARG_LIT(compare, f32);

    TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, LodCompareOffset, type, f32x4)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(lod, f32);\
    __ADD_ARG_LIT(compare, f32);\
    __ADD_ARG(offsets, offsets[index]);

    TEXTURE_INTRINSIC_NO_CUBE_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, LodOffset, type, f32x4)\
    __ADD_ARG(coords, normCoordinates[index]);\
    __ADD_ARG_LIT(lod, f32);\
    __ADD_ARG(offsets, offsets[index]);

    TEXTURE_INTRINSIC_NO_CUBE_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, LodProj, type, f32x4)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(lod, f32);\
    __ADD_ARG_LIT(proj, f32);

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, LodProjCompare, type, f32x4)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(lod, f32);\
    __ADD_ARG_LIT(proj, f32);\
    __ADD_ARG_LIT(compare, f32);

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, LodProjCompareOffset, type, f32x4)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(lod, f32);\
    __ADD_ARG_LIT(proj, f32);\
    __ADD_ARG_LIT(compare, f32);\
    __ADD_ARG(offsets, offsets[index]);

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, LodProjOffset, type, f32x4)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(lod, f32);\
    __ADD_ARG_LIT(proj, f32);\
    __ADD_ARG(offsets, offsets[index]);

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, Proj, type, f32x4)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(proj, f32);

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, ProjCompare, type, f32x4)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(proj, f32);\
    __ADD_ARG_LIT(compare, f32);

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, ProjCompareOffset, type, f32x4)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(proj, f32);\
    __ADD_ARG_LIT(compare, f32);\
    __ADD_ARG(offsets, offsets[index]);

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(type, index)\
    __MAKE_TEXTURE_INTRINSIC(Sample, ProjOffset, type, f32x4)\
    __ADD_ARG(coords, projCoordinates[index]);\
    __ADD_ARG_LIT(proj, f32);\
    __ADD_ARG(offsets, offsets[index]);

    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

    __END_INTRINSICS__
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
