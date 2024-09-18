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

#define __MAKE_INTRINSIC(nm, opcode)\
newIntrinsic = new Function();\
newIntrinsic->name = #nm;\
Intrinsics::opcode = newIntrinsic;\
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

    //------------------------------------------------------------------------------
    /**
        Scalar intrinsics (min, max, etc)
    */
    //------------------------------------------------------------------------------

    for (int i = 0; i < numScalarArgs; i++)
    {
        __MAKE_INTRINSIC(mad, Mad);
        __ADD_ARG(x, scalarArgs[i]);
        __ADD_ARG(y, scalarArgs[i]);
        __SET_RET(scalarArgs[i]);
    }

    for (int i = 0; i < numScalarArgs; i++)
    {
        __MAKE_INTRINSIC(dot, Dot);
        __ADD_ARG(x, scalarArgs[i]);
        __ADD_ARG(y, scalarArgs[i]);
        __SET_RET(scalarArgs[i]);
    }

    for (int i = 0; i < numScalarArgs; i++)
    {
        __MAKE_INTRINSIC(min, Min);
        __ADD_ARG(x, scalarArgs[i]);
        __ADD_ARG(y, scalarArgs[i]);
        __SET_RET(scalarArgs[i]);
    }

    for (int i = 0; i < numScalarArgs; i++)
    {
        __MAKE_INTRINSIC(max, Max);
        __ADD_ARG(x, scalarArgs[i]);
        __ADD_ARG(y, scalarArgs[i]);
        __SET_RET(scalarArgs[i]);
    }

    for (int i = 0; i < numScalarArgs; i++)
    {
        __MAKE_INTRINSIC(clamp, Clamp);
        __ADD_ARG(x, scalarArgs[i]);
        __ADD_ARG(min, scalarArgs[i]);
        __ADD_ARG(max, scalarArgs[i]);
        __SET_RET(scalarArgs[i]);
    }

    std::string floatArgs[] =
    {
        "f32"
        , "f32x2"
        , "f32x3"
        , "f32x4"
    };
    constexpr uint32_t numFloatArgs = sizeof(floatArgs) / sizeof(std::string);

    for (int i = 0; i < numFloatArgs; i++)
    {
        __MAKE_INTRINSIC(ceil, Ceil);
        __ADD_ARG(x, floatArgs[i]);
        __ADD_ARG(y, floatArgs[i]);
        __SET_RET(floatArgs[i]);
    }

    for (int i = 0; i < numFloatArgs; i++)
    {
        __MAKE_INTRINSIC(floor, Floor);
        __ADD_ARG(x, floatArgs[i]);
        __ADD_ARG(y, floatArgs[i]);
        __SET_RET(floatArgs[i]);
    }

    for (int i = 0; i < numFloatArgs; i++)
    {
        __MAKE_INTRINSIC(frac, Frac);
        __ADD_ARG(x, floatArgs[i]);
        __ADD_ARG(y, floatArgs[i]);
        __SET_RET(floatArgs[i]);
    }

    for (int i = 0; i < numFloatArgs; i++)
    {
        __MAKE_INTRINSIC(saturate, Saturate);
        __ADD_ARG(x, floatArgs[i]);
        __SET_RET(floatArgs[i]);
    }

    for (int i = 0; i < numFloatArgs; i++)
    {
        __MAKE_INTRINSIC(trunc, Truncate);
        __ADD_ARG(x, floatArgs[i]);
        __SET_RET(floatArgs[i]);
    }

    for (int i = 0; i < numFloatArgs; i++)
    {
        __MAKE_INTRINSIC(ddx, DDX);
        __ADD_ARG(x, floatArgs[i]);
        __SET_RET(floatArgs[i]);
    }

    for (int i = 0; i < numFloatArgs; i++)
    {
        __MAKE_INTRINSIC(ddy, DDY);
        __ADD_ARG(x, floatArgs[i]);
        __SET_RET(floatArgs[i]);
    }

    for (int i = 0; i < numFloatArgs; i++)
    {
        __MAKE_INTRINSIC(fwidth, FWidth);
        __ADD_ARG(x, floatArgs[i]);
        __SET_RET(floatArgs[i]);
    }

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

    for (int i = 0; i < numScalarArgs; i++)
    {
        __MAKE_INTRINSIC(subgroupSwapDiagonal, SubgroupSwapDiagonal);
        __ADD_ARG(value, scalarArgs[i]);
        __SET_RET(scalarArgs[i]);
    }

    for (int i = 0; i < numScalarArgs; i++)
    {
        __MAKE_INTRINSIC(subgroupSwapVertical, SubgroupSwapVertical);
        __ADD_ARG(value, scalarArgs[i]);
        __SET_RET(scalarArgs[i]);
    }

    for (int i = 0; i < numScalarArgs; i++)
    {
        __MAKE_INTRINSIC(subgroupSwapHorizontal, SubgroupSwapHorizontal);
        __ADD_ARG(value, scalarArgs[i]);
        __SET_RET(scalarArgs[i]);
    }

    std::string rwTextures[] =
    {
        "readWriteTexture1D"
        , "readWriteTexture2D"
        , "readWriteTexture2DMS"
        , "readWriteTexture3D"
        , "readWriteTextureCube"
        , "readWriteTexture1DArray"
        , "readWriteTexture2DArray"
        , "readWriteTexture2DMSArray"
        , "readWriteTextureCubeArray"
    };

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
    constexpr uint32_t numTextureTypes = sizeof(rwTextures) / sizeof(std::string);

    //------------------------------------------------------------------------------
    /**
        Texture intrinsics
    */
    //------------------------------------------------------------------------------

    // Load
    for (int i = 0; i < numTextureTypes; i++)
    {
        __MAKE_INTRINSIC(textureLoad, TextureLoad);
        __ADD_HANDLE_ARG(texture, rwTextures[i]);
        __ADD_ARG(coords, intCoordinates[i]);
        __SET_RET_LIT(f32x4);
    }

    // Load + lod
    for (int i = 0; i < numTextureTypes; i++)
    {
        __MAKE_INTRINSIC(textureLoadMip, TextureLoadMip);
        __ADD_HANDLE_ARG(texture, rwTextures[i]);
        __ADD_ARG(coords, intCoordinates[i]);
        __ADD_ARG_LIT(lod, i32);
        __SET_RET_LIT(f32x4);
    }

    // Store
    for (int i = 0; i < numTextureTypes; i++)
    {
        __MAKE_INTRINSIC(textureStore, TextureStore);
        __ADD_HANDLE_ARG(texture, rwTextures[i]);
        __ADD_ARG(coords, intCoordinates[i]);
        __ADD_ARG_LIT(value, f32x4);
        __SET_RET_LIT(f32x4);
    }

    // Store + lod
    for (int i = 0; i < numTextureTypes; i++)
    {
        __MAKE_INTRINSIC(textureStoreMip, TextureStoreMip);
        __ADD_HANDLE_ARG(texture, rwTextures[i]);
        __ADD_ARG(coords, intCoordinates[i]);
        __ADD_ARG_LIT(lod, i32);
        __ADD_ARG_LIT(value, f32x4);
        __SET_RET_LIT(f32x4);
    }

    __MAKE_INTRINSIC(pixelCacheLoad, PixelCacheLoad);
    __ADD_HANDLE_ARG_LIT(texture, pixelCache);
    __SET_RET_LIT(f32x4);

    __MAKE_INTRINSIC(pixelCacheLoad, PixelCacheLoad);
    __ADD_HANDLE_ARG_LIT(texture, pixelCacheMS);
    __SET_RET_LIT(f32x4);

    std::string textures[] =
    {
        "texture1D"
        , "texture2D"
        , "texture2DMS"
        , "texture3D"
        , "textureCube"
        , "texture1DArray"
        , "texture2DArray"
        , "texture2DMSArray"
        , "textureCubeArray"
    };

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

    // Normal sample
    for (int i = 0; i < numTextureTypes; i++)
    {
        __MAKE_INTRINSIC(textureSample, TextureSample);
        __ADD_HANDLE_ARG(texture, textures[i]);
        __ADD_ARG(coords, normCoordinates[i]);
        __ADD_HANDLE_ARG_LIT(sampler, sampler);
        __SET_RET_LIT(f32x4);
    }

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

// These macros help to setup the different sampling functions based on requirements
#define OFFSET_REQUIREMENTS()\
if (textures[i].find_last_of("Cube") != std::string::npos)\
    continue;

#define BIAS_REQUIREMENTS()\
if (textures[i].find_last_of("MS") != std::string::npos)\
    continue;

#define GRAD_REQUIREMENTS()\
if (textures[i].find_last_of("MS") != std::string::npos)\
    continue;

#define PROJ_REQUIREMENTS()\
if (textures[i].find_last_of("Array") != std::string::npos)\
    continue;\
if (textures[i].find_last_of("MS") != std::string::npos)\
    continue;\
if (textures[i].find_last_of("Cube") != std::string::npos)\
    continue;

#define LOD_REQUIREMENTS()\
if (textures[i].find_last_of("MS") != std::string::npos)\
    continue;\

    // Offset
    for (int i = 0; i < numTextureTypes; i++)
    {
        // Skip cube textures for offset
        OFFSET_REQUIREMENTS();
        __MAKE_INTRINSIC(textureSampleOffset, TextureSampleOffset);
        __ADD_HANDLE_ARG(texture, textures[i]);
        __ADD_ARG(coords, normCoordinates[i]);
        __ADD_ARG(offsets, offsets[i]);
        __ADD_HANDLE_ARG_LIT(sampler, sampler);
        __SET_RET_LIT(f32x4);
    }

    // Bias
    for (int i = 0; i < numTextureTypes; i++)
    {
        BIAS_REQUIREMENTS();
        __MAKE_INTRINSIC(textureSampleBias, TextureSampleBias);
        __ADD_HANDLE_ARG(texture, textures[i]);
        __ADD_ARG(coords, normCoordinates[i]);
        __ADD_ARG_LIT(bias, f32);
        __ADD_HANDLE_ARG_LIT(sampler, sampler);
        __SET_RET_LIT(f32x4);
    }

    // Bias + compare
    for (int i = 0; i < numTextureTypes; i++)
    {
        BIAS_REQUIREMENTS();
        __MAKE_INTRINSIC(textureSampleBiasCompare, TextureSampleBiasCompare);
        __ADD_HANDLE_ARG(texture, textures[i]);
        __ADD_ARG(coords, normCoordinates[i]);
        __ADD_ARG_LIT(bias, f32);
        __ADD_ARG_LIT(compare, f32);
        __ADD_HANDLE_ARG_LIT(sampler, sampler);
        __SET_RET_LIT(f32x4);
    }

    // Offset + bias
    for (int i = 0; i < numTextureTypes; i++)
    {
        OFFSET_REQUIREMENTS();
        BIAS_REQUIREMENTS();
        __MAKE_INTRINSIC(textureSampleBiasOffset, TextureSampleBiasOffset);
        __ADD_HANDLE_ARG(texture, textures[i]);
        __ADD_ARG(coords, normCoordinates[i]);
        __ADD_ARG_LIT(bias, f32);
        __ADD_ARG(offsets, offsets[i]);
        __ADD_HANDLE_ARG_LIT(sampler, sampler);
        __SET_RET_LIT(f32x4);
    }

    // Proj + bias
    for (int i = 0; i < numTextureTypes; i++)
    {
        BIAS_REQUIREMENTS();
        PROJ_REQUIREMENTS();
        __MAKE_INTRINSIC(textureSampleBiasProj, TextureSampleBiasProj);
        __ADD_HANDLE_ARG(texture, textures[i]);
        __ADD_ARG(coord, normCoordinates[i]);
        __ADD_ARG_LIT(bias, f32);
        __ADD_ARG_LIT(proj, f32);
        __ADD_HANDLE_ARG_LIT(sampler, sampler);
        __SET_RET_LIT(f32x4);
    }

    // Proj + bias + compare
    for (int i = 0; i < numTextureTypes; i++)
    {
        BIAS_REQUIREMENTS();
        PROJ_REQUIREMENTS();
        __MAKE_INTRINSIC(textureSampleBiasProjCompare, TextureSample);
        __ADD_HANDLE_ARG(texture, textures[i]);
        __ADD_ARG(coord, normCoordinates[i]);
        __ADD_ARG_LIT(bias, f32);
        __ADD_ARG_LIT(proj, f32);
        __ADD_ARG_LIT(compare, f32);
        __ADD_HANDLE_ARG_LIT(sampler, sampler);
        __SET_RET_LIT(f32x4);
    }

    // Proj + offset + bias
    for (int i = 0; i < numTextureTypes; i++)
    {
        OFFSET_REQUIREMENTS();
        BIAS_REQUIREMENTS();
        PROJ_REQUIREMENTS();
        __MAKE_INTRINSIC(textureSampleBiasProjOffset, TextureSampleBiasProjOffset);
        __ADD_HANDLE_ARG(texture, textures[i]);
        __ADD_ARG(coord, normCoordinates[i]);
        __ADD_ARG_LIT(bias, f32);
        __ADD_ARG_LIT(proj, f32);
        __ADD_ARG(offsets, offsets[i]);
        __ADD_HANDLE_ARG_LIT(sampler, sampler);
        __SET_RET_LIT(f32x4);
    }


    // Compare
    for (int i = 0; i < numTextureTypes; i++)
    {
        __MAKE_INTRINSIC(textureSampleCompare, TextureSample);
        __ADD_HANDLE_ARG(texture, textures[i]);
        __ADD_ARG(coords, normCoordinates[i]);
        __ADD_ARG_LIT(compare, f32);
        __ADD_HANDLE_ARG_LIT(sampler, sampler);
        __SET_RET_LIT(f32x4);
    }

    // Compare + offset
    for (int i = 0; i < numTextureTypes; i++)
    {
        OFFSET_REQUIREMENTS();
        __MAKE_INTRINSIC(textureSampleCompareOffset, TextureSampleCompareOffset);
        __ADD_HANDLE_ARG(texture, textures[i]);
        __ADD_ARG(coords, normCoordinates[i]);
        __ADD_ARG_LIT(compare, f32);
        __ADD_ARG(offsets, offsets[i]);
        __ADD_HANDLE_ARG_LIT(sampler, sampler);
        __SET_RET_LIT(f32x4);
    }

    // Grad
    for (int i = 0; i < numTextureTypes; i++)
    {
        GRAD_REQUIREMENTS();
        __MAKE_INTRINSIC(textureSampleGrad, TextureSampleGrad);
        __ADD_HANDLE_ARG(texture, textures[i]);
        __ADD_ARG(coords, normCoordinates[i]);
        __ADD_ARG(grad, gradients[i]);
        __ADD_HANDLE_ARG_LIT(sampler, sampler);
        __SET_RET_LIT(f32x4);
    }

    // Grad + compare
    for (int i = 0; i < numTextureTypes; i++)
    {
        GRAD_REQUIREMENTS();
        __MAKE_INTRINSIC(textureSampleGradCompare, TextureSampleGradCompare);
        __ADD_HANDLE_ARG(texture, textures[i]);
        __ADD_ARG(coords, normCoordinates[i]);
        __ADD_ARG(grad, gradients[i]);
        __ADD_ARG_LIT(compare, f32);
        __ADD_HANDLE_ARG_LIT(sampler, sampler);
        __SET_RET_LIT(f32x4);
    }

    // Grad + compare + offset
    for (int i = 0; i < numTextureTypes; i++)
    {
        OFFSET_REQUIREMENTS();
        GRAD_REQUIREMENTS();
        __MAKE_INTRINSIC(textureSampleGradCompareOffset, TextureSampleGradCompareOffset);
        __ADD_HANDLE_ARG(texture, textures[i]);
        __ADD_ARG(coords, normCoordinates[i]);
        __ADD_ARG(grad, gradients[i]);
        __ADD_ARG_LIT(compare, f32);
        __ADD_ARG(offsets, offsets[i]);
        __ADD_HANDLE_ARG_LIT(sampler, sampler);
        __SET_RET_LIT(f32x4);
    }

    // Grad + offset
    for (int i = 0; i < numTextureTypes; i++)
    {
        OFFSET_REQUIREMENTS();
        GRAD_REQUIREMENTS();
        __MAKE_INTRINSIC(textureSampleGradOffset, TextureSampleGradOffset);
        __ADD_HANDLE_ARG(texture, textures[i]);
        __ADD_ARG(coords, normCoordinates[i]);
        __ADD_ARG(grad, gradients[i]);
        __ADD_ARG(offsets, offsets[i]);
        __ADD_HANDLE_ARG_LIT(sampler, sampler);
        __SET_RET_LIT(f32x4);
    }

    // Grad + proj
    for (int i = 0; i < numTextureTypes; i++)
    {
        PROJ_REQUIREMENTS();
        GRAD_REQUIREMENTS();
        __MAKE_INTRINSIC(textureSampleGradProj, TextureSampleGradProj);
        __ADD_HANDLE_ARG(texture, textures[i]);
        __ADD_ARG(coords, normCoordinates[i]);
        __ADD_ARG(grad, gradients[i]);
        __ADD_ARG_LIT(proj, f32);
        __ADD_HANDLE_ARG_LIT(sampler, sampler);
        __SET_RET_LIT(f32x4);
    }

    // Grad + proj + compare
    for (int i = 0; i < numTextureTypes; i++)
    {
        PROJ_REQUIREMENTS();
        GRAD_REQUIREMENTS();
        __MAKE_INTRINSIC(textureSampleGradProjCompare, TextureSampleGradProjCompare);
        __ADD_HANDLE_ARG(texture, textures[i]);
        __ADD_ARG(coords, normCoordinates[i]);
        __ADD_ARG(grad, gradients[i]);
        __ADD_ARG_LIT(proj, f32);
        __ADD_ARG_LIT(compare, f32);
        __ADD_HANDLE_ARG_LIT(sampler, sampler);
        __SET_RET_LIT(f32x4);
    }

    // Grad + proj + compare + offset
    for (int i = 0; i < numTextureTypes; i++)
    {
        PROJ_REQUIREMENTS();
        GRAD_REQUIREMENTS();
        OFFSET_REQUIREMENTS();
        __MAKE_INTRINSIC(textureSampleGradProjCompareOffset, TextureSampleGradProjCompareOffset);
        __ADD_HANDLE_ARG(texture, textures[i]);
        __ADD_ARG(coords, normCoordinates[i]);
        __ADD_ARG(grad, gradients[i]);
        __ADD_ARG_LIT(proj, f32);
        __ADD_ARG_LIT(compare, f32);
        __ADD_ARG(offsets, offsets[i]);
        __ADD_HANDLE_ARG_LIT(sampler, sampler);
        __SET_RET_LIT(f32x4);
    }

    // Grad + proj + offset
    for (int i = 0; i < numTextureTypes; i++)
    {
        PROJ_REQUIREMENTS();
        GRAD_REQUIREMENTS();
        OFFSET_REQUIREMENTS();
        __MAKE_INTRINSIC(textureSampleGradProjOffset, TextureSampleGradProjOffset);
        __ADD_HANDLE_ARG(texture, textures[i]);
        __ADD_ARG(coords, normCoordinates[i]);
        __ADD_ARG(grad, gradients[i]);
        __ADD_ARG_LIT(proj, f32);
        __ADD_ARG(offsets, offsets[i]);
        __ADD_HANDLE_ARG_LIT(sampler, sampler);
        __SET_RET_LIT(f32x4);
    }

    // Lod
    for (int i = 0; i < numTextureTypes; i++)
    {
        LOD_REQUIREMENTS();
        __MAKE_INTRINSIC(textureSampleLod, TextureSampleLod);
        __ADD_HANDLE_ARG(texture, textures[i]);
        __ADD_ARG(coords, normCoordinates[i]);
        __ADD_ARG_LIT(lod, f32);
        __ADD_HANDLE_ARG_LIT(sampler, sampler);
        __SET_RET_LIT(f32x4);
    }

    // Lod + compare
    for (int i = 0; i < numTextureTypes; i++)
    {
        LOD_REQUIREMENTS();
        __MAKE_INTRINSIC(textureSampleLodCompare, TextureSampleLodCompare);
        __ADD_HANDLE_ARG(texture, textures[i]);
        __ADD_ARG(coords, normCoordinates[i]);
        __ADD_ARG_LIT(lod, f32);
        __ADD_ARG_LIT(compare, f32);
        __ADD_HANDLE_ARG_LIT(sampler, sampler);
        __SET_RET_LIT(f32x4);
    }

    // Lod + compare + offset
    for (int i = 0; i < numTextureTypes; i++)
    {
        LOD_REQUIREMENTS();
        OFFSET_REQUIREMENTS();
        __MAKE_INTRINSIC(textureSampleLodCompareOffset, TextureSampleLodCompareOffset);
        __ADD_HANDLE_ARG(texture, textures[i]);
        __ADD_ARG(coords, normCoordinates[i]);
        __ADD_ARG_LIT(lod, f32);
        __ADD_ARG_LIT(compare, f32);
        __ADD_ARG(offsets, offsets[i]);
        __ADD_HANDLE_ARG_LIT(sampler, sampler);
        __SET_RET_LIT(f32x4);
    }

    // Lod + offset
    for (int i = 0; i < numTextureTypes; i++)
    {
        OFFSET_REQUIREMENTS();
        LOD_REQUIREMENTS();
        __MAKE_INTRINSIC(textureSampleLodOffset, TextureSampleLodOffset);
        __ADD_HANDLE_ARG(texture, textures[i]);
        __ADD_ARG(coords, normCoordinates[i]);
        __ADD_ARG_LIT(lod, f32);
        __ADD_ARG(offsets, offsets[i]);
        __ADD_HANDLE_ARG_LIT(sampler, sampler);
        __SET_RET_LIT(f32x4);
    }

    // Lod + proj
    for (int i = 0; i < numTextureTypes; i++)
    {
        PROJ_REQUIREMENTS();
        LOD_REQUIREMENTS();
        __MAKE_INTRINSIC(textureSampleLodProj, TextureSampleLodProj);
        __ADD_HANDLE_ARG(texture, textures[i]);
        __ADD_ARG(coords, normCoordinates[i]);
        __ADD_ARG_LIT(lod, f32);
        __ADD_ARG_LIT(proj, f32);
        __ADD_HANDLE_ARG_LIT(sampler, sampler);
        __SET_RET_LIT(f32x4);
    }

    // Lod + proj + compare
    for (int i = 0; i < numTextureTypes; i++)
    {
        PROJ_REQUIREMENTS();
        LOD_REQUIREMENTS();
        __MAKE_INTRINSIC(textureSampleLodProjCompare, TextureSampleLodProjCompare);
        __ADD_HANDLE_ARG(texture, textures[i]);
        __ADD_ARG(coords, normCoordinates[i]);
        __ADD_ARG_LIT(lod, f32);
        __ADD_ARG_LIT(proj, f32);
        __ADD_ARG_LIT(compare, f32);
        __ADD_HANDLE_ARG_LIT(sampler, sampler);
        __SET_RET_LIT(f32x4);
    }

    // Lod + proj + compare + offset
    for (int i = 0; i < numTextureTypes; i++)
    {
        OFFSET_REQUIREMENTS();
        LOD_REQUIREMENTS();
        PROJ_REQUIREMENTS();
        __MAKE_INTRINSIC(textureSampleLodProjCompareOffset, TextureSampleLodProjCompareOffset);
        __ADD_HANDLE_ARG(texture, textures[i]);
        __ADD_ARG(coords, normCoordinates[i]);
        __ADD_ARG_LIT(lod, f32);
        __ADD_ARG_LIT(proj, f32);
        __ADD_ARG_LIT(compare, f32);
        __ADD_ARG(offsets, offsets[i]);
        __ADD_HANDLE_ARG_LIT(sampler, sampler);
        __SET_RET_LIT(f32x4);
    }

    // Proj + lod + offset
    for (int i = 0; i < numTextureTypes; i++)
    {
        OFFSET_REQUIREMENTS();
        PROJ_REQUIREMENTS();
        LOD_REQUIREMENTS();
        __MAKE_INTRINSIC(textureSampleLodProjOffset, TextureSampleLodProjOffset);
        __ADD_HANDLE_ARG(texture, textures[i]);
        __ADD_ARG(coord, normCoordinates[i]);
        __ADD_ARG_LIT(lod, f32);
        __ADD_ARG_LIT(proj, f32);
        __ADD_ARG(offsets, offsets[i]);
        __ADD_HANDLE_ARG_LIT(sampler, sampler);
        __SET_RET_LIT(f32x4);
    }

    // Proj
    for (int i = 0; i < numTextureTypes; i++)
    {
        PROJ_REQUIREMENTS();
        __MAKE_INTRINSIC(textureSampleProj, TextureSampleProj);
        __ADD_HANDLE_ARG(texture, textures[i]);
        __ADD_ARG(coord, normCoordinates[i]);
        __ADD_ARG_LIT(proj, f32);
        __ADD_HANDLE_ARG_LIT(sampler, sampler);
        __SET_RET_LIT(f32x4);
    }

    // Proj + compare
    for (int i = 0; i < numTextureTypes; i++)
    {
        PROJ_REQUIREMENTS();
        __MAKE_INTRINSIC(textureSampleProjCompare, TextureSampleProjCompare);
        __ADD_HANDLE_ARG(texture, textures[i]);
        __ADD_ARG(coord, normCoordinates[i]);
        __ADD_ARG_LIT(proj, f32);
        __ADD_ARG_LIT(compare, f32);
        __ADD_HANDLE_ARG_LIT(sampler, sampler);
        __SET_RET_LIT(f32x4);
    }

    // Proj + compare + offset
    for (int i = 0; i < numTextureTypes; i++)
    {
        PROJ_REQUIREMENTS();
        OFFSET_REQUIREMENTS();
        __MAKE_INTRINSIC(textureSampleProjCompareOffset, TextureSampleProjCompareOffset);
        __ADD_HANDLE_ARG(texture, textures[i]);
        __ADD_ARG(coord, normCoordinates[i]);
        __ADD_ARG_LIT(proj, f32);
        __ADD_ARG_LIT(compare, f32);
        __ADD_ARG(offsets, offsets[i]);
        __ADD_HANDLE_ARG_LIT(sampler, sampler);
        __SET_RET_LIT(f32x4);
    }

    // Proj + offset
    for (int i = 0; i < numTextureTypes; i++)
    {
        PROJ_REQUIREMENTS();
        OFFSET_REQUIREMENTS();
        __MAKE_INTRINSIC(textureSampleProjOffset, TextureSampleProjOffset);
        __ADD_HANDLE_ARG(texture, textures[i]);
        __ADD_ARG(coord, normCoordinates[i]);
        __ADD_ARG_LIT(proj, f32);
        __ADD_ARG(offsets, offsets[i]);
        __ADD_HANDLE_ARG_LIT(sampler, sampler);
        __SET_RET_LIT(f32x4);
    }

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
