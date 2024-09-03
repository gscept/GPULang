//------------------------------------------------------------------------------
//  @file type.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "type.h"
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
#include "programtype.h"
#include "functiontype.h"
#include <set>
namespace AnyFX
{

Function* activeFunction = nullptr;

#define __BEGIN_TYPES__ std::vector<Symbol*> types; Type* newType = nullptr;

#define __MAKE_TYPE(typename, typecode)\
newType = new Type();\
newType->name = #typename;\
newType->baseType = typecode;\
types.push_back(newType);

#define __MAKE_RWTEX()\
newType->category = Type::ReadWriteTextureCategory;

#define __MAKE_TEX()\
newType->category = Type::TextureCategory;

#define __MAKE_SAMPTEX()\
newType->category = Type::SampledTextureCategory;

#define __MAKE_PIXELCACHE()\
newType->category = Type::PixelCacheCategory;

#define __MAKE_SAMPLER()\
newType->category = Type::SamplerCategory;

#define __MAKE_TYPE_CUSTOM(t1, t2)\
newType = new t2();\
newType->name = #t1;\
types.push_back(newType);

#define __ADD_LOOKUP(name) types[#name] = newType;
#define __END_TYPES__ return types;

//------------------------------------------------------------------------------
/**
*/
Type::Type()
{
    this->symbolType = Symbol::TypeType;
    this->baseType = TypeCode::InvalidType;
    this->rowSize = 1;
    this->columnSize = 1;
    this->byteSize = 4;
    this->category = Type::InvalidCategory;
}

//------------------------------------------------------------------------------
/**
*/
Type::~Type()
{
    for (Symbol* sym : this->symbols)
        delete sym;
}

std::map<TypeCode, std::string> codeToStringMapping =
{
    { TypeCode::Float, "float" }
    , { TypeCode::Float16, "float16" }
    , { TypeCode::Int, "int" }
    , { TypeCode::Int16, "int16" }
    , { TypeCode::UInt, "uint" }
    , { TypeCode::UInt16, "uint16" }
    , { TypeCode::Bool, "bool" }
};

//------------------------------------------------------------------------------
/**
*/
std::string 
Type::CodeToString(const TypeCode& code)
{
    auto it = codeToStringMapping.find(code);
    if (it == codeToStringMapping.end())
        return "";
    else
        return it->second;
}

//------------------------------------------------------------------------------
/**
*/
Symbol* 
Type::GetSymbol(const std::string str)
{
    auto it = this->lookup.find(str);
    if (it != this->lookup.end())
        return it->second;
    else
        return nullptr;
}

//------------------------------------------------------------------------------
/**
*/
std::vector<Symbol*> 
Type::GetSymbols(const std::string str)
{
    std::vector<Symbol*> ret;
    auto range = this->lookup.equal_range(str);
    for (auto it = range.first; it != range.second; it++)
        ret.push_back((*it).second);
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
std::vector<Symbol*>
Type::SetupDefaultTypes()
{
    __BEGIN_TYPES__

    __MAKE_TYPE_CUSTOM(int, AnyFX::Int);
    __MAKE_TYPE_CUSTOM(int2, AnyFX::Int2);
    __MAKE_TYPE_CUSTOM(int3, AnyFX::Int3);
    __MAKE_TYPE_CUSTOM(int4, AnyFX::Int4);
    __MAKE_TYPE_CUSTOM(uint, AnyFX::UInt);
    __MAKE_TYPE_CUSTOM(uint2, AnyFX::UInt2);
    __MAKE_TYPE_CUSTOM(uint3, AnyFX::UInt3);
    __MAKE_TYPE_CUSTOM(uint4, AnyFX::UInt4);
    __MAKE_TYPE_CUSTOM(bool, AnyFX::Bool);
    __MAKE_TYPE_CUSTOM(bool2, AnyFX::Bool2);
    __MAKE_TYPE_CUSTOM(bool3, AnyFX::Bool3);
    __MAKE_TYPE_CUSTOM(bool4, AnyFX::Bool4);

    __MAKE_TYPE_CUSTOM(float, AnyFX::Float);
    __MAKE_TYPE_CUSTOM(float2, AnyFX::Float2);
    __MAKE_TYPE_CUSTOM(float3, AnyFX::Float3);
    __MAKE_TYPE_CUSTOM(float4, AnyFX::Float4);
    __MAKE_TYPE_CUSTOM(float2x2, AnyFX::Mat2x2);
    __MAKE_TYPE_CUSTOM(float2x3, AnyFX::Mat2x3);
    __MAKE_TYPE_CUSTOM(float2x4, AnyFX::Mat2x4);
    __MAKE_TYPE_CUSTOM(float3x2, AnyFX::Mat3x2);
    __MAKE_TYPE_CUSTOM(float3x3, AnyFX::Mat3x3);
    __MAKE_TYPE_CUSTOM(float3x4, AnyFX::Mat3x4);
    __MAKE_TYPE_CUSTOM(float4x2, AnyFX::Mat4x2);
    __MAKE_TYPE_CUSTOM(float4x3, AnyFX::Mat4x3);
    __MAKE_TYPE_CUSTOM(float4x4, AnyFX::Mat4x4);

    __MAKE_TYPE(readWriteTexture1D, TypeCode::ReadWriteTexture1D);
    __MAKE_RWTEX();

    __MAKE_TYPE(readWriteTexture2D, TypeCode::ReadWriteTexture2D);
    __MAKE_RWTEX();

    __MAKE_TYPE(readWriteTexture2DMS, TypeCode::ReadWriteTexture2DMS);
    __MAKE_RWTEX();

    __MAKE_TYPE(readWriteTexture3D, TypeCode::ReadWriteTexture3D);
    __MAKE_RWTEX();

    __MAKE_TYPE(readWriteTextureCube, TypeCode::ReadWriteTextureCube);
    __MAKE_RWTEX();

    __MAKE_TYPE(readWriteTexture1DArray, TypeCode::ReadWriteTexture1DArray);
    __MAKE_RWTEX();

    __MAKE_TYPE(readWriteTexture2DArray, TypeCode::ReadWriteTexture2DArray);
    __MAKE_RWTEX();

    __MAKE_TYPE(readWriteTexture2DMSArray, TypeCode::ReadWriteTexture2DMSArray);
    __MAKE_RWTEX();

    __MAKE_TYPE(readWriteTextureCubeArray, TypeCode::ReadWriteTextureCubeArray);
    __MAKE_RWTEX();

    __MAKE_TYPE(texture1D, TypeCode::Texture1D);
    __MAKE_TEX();

    __MAKE_TYPE(texture2D, TypeCode::Texture2D);
    __MAKE_TEX();

    __MAKE_TYPE(texture2DMS, TypeCode::Texture2DMS);
    __MAKE_TEX();

    __MAKE_TYPE(texture3D, TypeCode::Texture3D);
    __MAKE_TEX();

    __MAKE_TYPE(textureCube, TypeCode::TextureCube);
    __MAKE_TEX();

    __MAKE_TYPE(texture1DArray, TypeCode::Texture1DArray);
    __MAKE_TEX();

    __MAKE_TYPE(texture2DArray, TypeCode::Texture2DArray);
    __MAKE_TEX();

    __MAKE_TYPE(texture2DMSArray, TypeCode::Texture2DMSArray);
    __MAKE_TEX();

    __MAKE_TYPE(textureCubeArray, TypeCode::TextureCubeArray);
    __MAKE_TEX();

    __MAKE_TYPE(pixelCache, TypeCode::PixelCache);
    __MAKE_PIXELCACHE();

    __MAKE_TYPE(pixelCacheMS, TypeCode::PixelCacheMS);
    __MAKE_PIXELCACHE();

    Enumeration* builtinEnum = new Enumeration();
    builtinEnum->name = "CompareMode";
    builtinEnum->baseType = AnyFX::TypeCode::UInt;
    builtinEnum->labels.push_back("InvalidCompareMode"); builtinEnum->values.push_back(nullptr);
    builtinEnum->labels.push_back("Never"); builtinEnum->values.push_back(nullptr);
    builtinEnum->labels.push_back("Less"); builtinEnum->values.push_back(nullptr);
    builtinEnum->labels.push_back("Equal"); builtinEnum->values.push_back(nullptr);
    builtinEnum->labels.push_back("LessEqual"); builtinEnum->values.push_back(nullptr);
    builtinEnum->labels.push_back("Greater"); builtinEnum->values.push_back(nullptr);
    builtinEnum->labels.push_back("NotEqual"); builtinEnum->values.push_back(nullptr);
    builtinEnum->labels.push_back("GreaterEqual"); builtinEnum->values.push_back(nullptr);
    builtinEnum->labels.push_back("Always"); builtinEnum->values.push_back(nullptr);
    types.push_back(builtinEnum);

    __MAKE_TYPE_CUSTOM(function, AnyFX::FunctionType);
    __MAKE_TYPE_CUSTOM(renderState, AnyFX::RenderStateType);
    __MAKE_TYPE_CUSTOM(samplerState, AnyFX::SamplerStateType);
    __MAKE_TYPE_CUSTOM(program, AnyFX::ProgramType);

    __MAKE_TYPE(sampler, TypeCode::Sampler);
    __MAKE_SAMPLER();

    __MAKE_TYPE(void, TypeCode::Void);

    __END_TYPES__
}

std::map<TypeCode, std::vector<std::string>> singleComponentToVectorMap =
{
    { TypeCode::Float, { "float", "float2", "float3", "float4" } }
    , { TypeCode::Int, { "int", "int2", "int3", "int4" } }
    , { TypeCode::UInt, { "uint", "uint2", "uint3", "uint4" } }
    , { TypeCode::Bool, { "bool", "bool2", "bool3", "bool4" } }

};

//------------------------------------------------------------------------------
/**
*/
const std::string 
Type::ToVector(const TypeCode baseType, unsigned members)
{
    if (members > 4)
        return "";
    else
        return singleComponentToVectorMap[baseType][members - 1];
}

//------------------------------------------------------------------------------
/**
*/
const bool
Type::IsVector() const
{
    return this->category == Type::ScalarCategory && this->columnSize > 1 && this->rowSize <= 1;
}

//------------------------------------------------------------------------------
/**
*/
const bool 
Type::IsMatrix() const
{
    return this->category == Type::ScalarCategory && this->columnSize > 1 && this->rowSize > 1;
}

//------------------------------------------------------------------------------
/**
*/
bool 
Type::StringToSwizzleMask(const std::string& str, SwizzleMask& out)
{
    // convert to swizzle mask
    out.x = SwizzleMask::Invalid;
    out.y = SwizzleMask::Invalid;
    out.z = SwizzleMask::Invalid;
    out.w = SwizzleMask::Invalid;
    for (size_t i = 0; i < str.length() && i <= 4; i++)
    {
        char c = str[i];
        switch (c)
        {
            case 'x':
            case 'r':
                switch (i)
                {
                    case 0:
                        out.x = SwizzleMask::X;
                        break;
                    case 1:
                        out.y = SwizzleMask::X;
                        break;
                    case 2:
                        out.z = SwizzleMask::X;
                        break;
                    case 3:
                        out.w = SwizzleMask::X;
                        break;
                }
                break;
            case 'y':
            case 'g':
                switch (i)
                {
                    case 0:
                        out.x = SwizzleMask::Y;
                        break;
                    case 1:
                        out.y = SwizzleMask::Y;
                        break;
                    case 2:
                        out.z = SwizzleMask::Y;
                        break;
                    case 3:
                        out.w = SwizzleMask::Y;
                        break;
                }
                break;
            case 'z':
            case 'b':
                switch (i)
                {
                    case 0:
                        out.x = SwizzleMask::Z;
                        break;
                    case 1:
                        out.y = SwizzleMask::Z;
                        break;
                    case 2:
                        out.z = SwizzleMask::Z;
                        break;
                    case 3:
                        out.w = SwizzleMask::Z;
                        break;
                }
                break;
            case 'w':
            case 'a':
                switch (i)
                {
                    case 0:
                        out.x = SwizzleMask::W;
                        break;
                    case 1:
                        out.y = SwizzleMask::W;
                        break;
                    case 2:
                        out.z = SwizzleMask::W;
                        break;
                    case 3:
                        out.w = SwizzleMask::W;
                        break;
                }
                break;
            default:
                return false;
        }
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
unsigned 
Type::SwizzleMaskComponents(SwizzleMask mask)
{
    int numComponents = 0;
    if (mask.x != SwizzleMask::Invalid)
        numComponents++;
    if (mask.y != SwizzleMask::Invalid)
        numComponents++;
    if (mask.z != SwizzleMask::Invalid)
        numComponents++;
    if (mask.w != SwizzleMask::Invalid)
        numComponents++;
    return numComponents;
}


//------------------------------------------------------------------------------
/**
*/
unsigned
Type::SwizzleMaskBiggestComponent(SwizzleMask mask)
{
    unsigned ret = 0;

    // Find the max value, but make sure to mask out Invalid
    ret = max(mask.x & ~SwizzleMask::Invalid, ret);
    ret = max(mask.y & ~SwizzleMask::Invalid, ret);
    ret = max(mask.z & ~SwizzleMask::Invalid, ret);
    ret = max(mask.w & ~SwizzleMask::Invalid, ret);
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
std::string 
Type::FullType::ToString()
{
    std::string modifiers = "";
    for (int i = this->modifiers.size()-1; i >= 0; i--)
    {
        uint32_t size = this->modifierValues[i];
        if (this->modifiers[i] == Type::FullType::Modifier::ArrayLevel)
            if (size == 0)
                modifiers.append("[]");
            else
            {
                modifiers.append(Format("[%d]", size));
            }
        else if (this->modifiers[i] == Type::FullType::Modifier::PointerLevel)
            modifiers.append("*");
    }

    return Format("%s%s", this->name.c_str(), modifiers.c_str());
}

//------------------------------------------------------------------------------
/**
*/
const bool 
Type::FullType::IsPointer() const
{
    if (!this->modifiers.empty())
        return this->modifiers.back() == Type::FullType::Modifier::PointerLevel;
    else
        return false;
}

} // namespace AnyFX
