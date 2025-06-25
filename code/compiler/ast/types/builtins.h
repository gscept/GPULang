#pragma once
//------------------------------------------------------------------------------
/**
    Builtin types

    @copyright
    (C) 2024 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "util.h"
#include "bool.h"
#include "bool2.h"
#include "bool3.h"
#include "bool4.h"
#include "int.h"
#include "int2.h"
#include "int3.h"
#include "int4.h"
#include "uint.h"
#include "uint2.h"
#include "uint3.h"
#include "uint4.h"
#include "float.h"
#include "float2.h"
#include "float3.h"
#include "float4.h"
#include "mat2x2.h"
#include "mat2x3.h"
#include "mat2x4.h"
#include "mat3x2.h"
#include "mat3x3.h"
#include "mat3x4.h"
#include "mat4x2.h"
#include "mat4x3.h"
#include "mat4x4.h"
#include "renderstatetype.h"
#include "samplerstatetype.h"
#include "stencilstatetype.h"
#include "programtype.h"
#include "functiontype.h"
#include "ast/enumeration.h"

namespace GPULang
{
extern FunctionType FunctionTypeType;

struct CompareModeType : public Enumeration
{
    CompareModeType();
};
extern CompareModeType CompareModeTypeType;

struct StencilOpType : public Enumeration
{
    StencilOpType();
};
extern StencilOpType StencilOpTypeType;

struct ExecutionScopeType : public Enumeration
{
    ExecutionScopeType();
};
extern ExecutionScopeType ExecutionScopeTypeType;

struct MemorySemanticsType : public Enumeration
{
    MemorySemanticsType();
};
extern MemorySemanticsType MemorySemanticsTypeType;

struct Void : public Type
{
    Void()
    {
        this->name = ConstantString("void");
        this->category = VoidCategory;
        this->baseType = TypeCode::Void;
    }
};
extern Void VoidType;

#define MAKE_TEXTURE_TYPE(type) struct Texture##type : public Type { Texture##type() { this->name = ConstantString("texture" #type); this->baseType = TypeCode::Texture##type; this->category = TextureCategory;}}; extern Texture##type Texture##type##Type;

MAKE_TEXTURE_TYPE(1D)
MAKE_TEXTURE_TYPE(1DArray)
MAKE_TEXTURE_TYPE(2D)
MAKE_TEXTURE_TYPE(2DArray)
MAKE_TEXTURE_TYPE(2DMS)
MAKE_TEXTURE_TYPE(2DMSArray)
MAKE_TEXTURE_TYPE(3D)
MAKE_TEXTURE_TYPE(3DArray)
MAKE_TEXTURE_TYPE(Cube)
MAKE_TEXTURE_TYPE(CubeArray)

struct SamplerType : public Type
{
    SamplerType()
    {
        this->name = ConstantString("sampler");
        this->category = SamplerCategory;
        this->baseType = TypeCode::Sampler;
    }
};
extern SamplerType SamplerTypeType;

struct PixelCacheType : public Type
{
    PixelCacheType()
    {
        this->name = ConstantString("pixelCache");
        this->category = PixelCacheCategory;
        this->baseType = TypeCode::PixelCache;
    }
};
extern PixelCacheType PixelCacheTypeType;

struct PixelCacheMS : public Type
{
    PixelCacheMS()
    {
        this->name = ConstantString("pixelCacheMS");
        this->category = PixelCacheCategory;
        this->baseType = TypeCode::PixelCacheMS;
    }
};
extern PixelCacheMS PixelCacheMSType;

struct AccelerationStructureType : public Type
{
    AccelerationStructureType()
    {
        this->name = ConstantString("accelerationStructure");
        this->category = AccelerationStructureCategory;
        this->baseType = TypeCode::AccelerationStructure;
    }
};
extern AccelerationStructureType AccelerationStructureTypeType;


}