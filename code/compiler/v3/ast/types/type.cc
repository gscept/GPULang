//------------------------------------------------------------------------------
//  @file type.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "type.h"
#include "builtins.h"
#include "ast/expressions/uintexpression.h"
#include <set>
namespace GPULang
{

Function* activeFunction = nullptr;

#define __BEGIN_TYPES__ Type* newType = nullptr;

#define __MAKE_TYPE(typename, typecode)\
newType = new Type();\
newType->name = #typename;\
newType->baseType = typecode;\
newType->category = Type::VoidCategory;\
DefaultTypes.push_back(newType);

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
DefaultTypes.push_back(newType);

#define __ADD_LOOKUP(name) DefaultTypes[#name] = newType;

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

const std::map<TypeCode, std::string> codeToStringMapping =
{
    { TypeCode::Void, "void" }
    , { TypeCode::Float, "f32" }
    , { TypeCode::Float16, "f16" }
    , { TypeCode::Int, "i32" }
    , { TypeCode::Int16, "i16" }
    , { TypeCode::UInt, "u32" }
    , { TypeCode::UInt16, "u16" }
    , { TypeCode::Bool, "b8" }
    , { TypeCode::Texture1D, "texture1D" }
    , { TypeCode::Texture2D, "texture2D" }
    , { TypeCode::Texture2DMS, "texture2DMS" }
    , { TypeCode::Texture3D, "texture3D" }
    , { TypeCode::TextureCube, "textureCube" }
    , { TypeCode::Texture1D, "texture1D" }
    , { TypeCode::Texture1D, "texture1D" }
    , { TypeCode::PixelCache, "pixelCache" }
    , { TypeCode::PixelCacheMS, "pixelCacheMS" }
    , { TypeCode::ReadWriteTexture1D, "readWriteTexture1D" }
    , { TypeCode::ReadWriteTexture2D, "readWriteTexture2D" }
    , { TypeCode::ReadWriteTexture2DMS, "readWriteTexture2DMS" }
    , { TypeCode::ReadWriteTexture3D, "readWriteTexture3D" }
    , { TypeCode::ReadWriteTextureCube, "readWriteTextureCube" }
    , { TypeCode::ReadWriteTexture1DArray, "readWriteTexture1DArray" }
    , { TypeCode::ReadWriteTexture2DArray, "readWriteTexture2DArray" }
    , { TypeCode::ReadWriteTexture2DMSArray, "readWriteTexture2DMSArray" }
    , { TypeCode::ReadWriteTextureCubeArray, "readWriteTextureCubeArray" }
    , { TypeCode::Sampler, "sampler" }
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
std::vector<Symbol*> DefaultTypes;
void
Type::SetupDefaultTypes()
{
    __BEGIN_TYPES__

    __MAKE_TYPE_CUSTOM(i32, GPULang::Int);
    __MAKE_TYPE_CUSTOM(i32x2, GPULang::Int2);
    __MAKE_TYPE_CUSTOM(i32x3, GPULang::Int3);
    __MAKE_TYPE_CUSTOM(i32x4, GPULang::Int4);
    __MAKE_TYPE_CUSTOM(u32, GPULang::UInt);
    __MAKE_TYPE_CUSTOM(u32x2, GPULang::UInt2);
    __MAKE_TYPE_CUSTOM(u32x3, GPULang::UInt3);
    __MAKE_TYPE_CUSTOM(u32x4, GPULang::UInt4);
    __MAKE_TYPE_CUSTOM(b8, GPULang::Bool);
    __MAKE_TYPE_CUSTOM(b8x2, GPULang::Bool2);
    __MAKE_TYPE_CUSTOM(b8x3, GPULang::Bool3);
    __MAKE_TYPE_CUSTOM(b8x4, GPULang::Bool4);

    __MAKE_TYPE_CUSTOM(f32, GPULang::Float);
    __MAKE_TYPE_CUSTOM(f32x2, GPULang::Float2);
    __MAKE_TYPE_CUSTOM(f32x3, GPULang::Float3);
    __MAKE_TYPE_CUSTOM(f32x4, GPULang::Float4);
    __MAKE_TYPE_CUSTOM(f32x2x2, GPULang::Mat2x2);
    __MAKE_TYPE_CUSTOM(f32x2x3, GPULang::Mat2x3);
    __MAKE_TYPE_CUSTOM(f32x2x4, GPULang::Mat2x4);
    __MAKE_TYPE_CUSTOM(f32x3x2, GPULang::Mat3x2);
    __MAKE_TYPE_CUSTOM(f32x3x3, GPULang::Mat3x3);
    __MAKE_TYPE_CUSTOM(f32x3x4, GPULang::Mat3x4);
    __MAKE_TYPE_CUSTOM(f32x4x2, GPULang::Mat4x2);
    __MAKE_TYPE_CUSTOM(f32x4x3, GPULang::Mat4x3);
    __MAKE_TYPE_CUSTOM(f32x4x4, GPULang::Mat4x4);

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
    builtinEnum->baseType = GPULang::TypeCode::UInt;
    builtinEnum->labels.push_back("InvalidCompareMode"); builtinEnum->values.push_back(nullptr);
    builtinEnum->labels.push_back("Never"); builtinEnum->values.push_back(nullptr);
    builtinEnum->labels.push_back("Less"); builtinEnum->values.push_back(nullptr);
    builtinEnum->labels.push_back("Equal"); builtinEnum->values.push_back(nullptr);
    builtinEnum->labels.push_back("LessEqual"); builtinEnum->values.push_back(nullptr);
    builtinEnum->labels.push_back("Greater"); builtinEnum->values.push_back(nullptr);
    builtinEnum->labels.push_back("NotEqual"); builtinEnum->values.push_back(nullptr);
    builtinEnum->labels.push_back("GreaterEqual"); builtinEnum->values.push_back(nullptr);
    builtinEnum->labels.push_back("Always"); builtinEnum->values.push_back(nullptr);
    DefaultTypes.push_back(builtinEnum);

    __MAKE_TYPE_CUSTOM(function, GPULang::FunctionType);
    __MAKE_TYPE_CUSTOM(renderState, GPULang::RenderStateType);
    __MAKE_TYPE_CUSTOM(samplerState, GPULang::SamplerStateType);
    __MAKE_TYPE_CUSTOM(program, GPULang::ProgramType);

    __MAKE_TYPE(sampler, TypeCode::Sampler);
    __MAKE_SAMPLER();

    Enumeration* executionScopeEnum = new Enumeration();
    executionScopeEnum->name = "ExecutionScope";
    executionScopeEnum->baseType = GPULang::TypeCode::UInt;
    executionScopeEnum->labels.push_back("CrossDevice"); executionScopeEnum->values.push_back(nullptr);
    executionScopeEnum->labels.push_back("DeviceLocal"); executionScopeEnum->values.push_back(nullptr);
    executionScopeEnum->labels.push_back("Workgroup"); executionScopeEnum->values.push_back(nullptr);
    executionScopeEnum->labels.push_back("Subgroup"); executionScopeEnum->values.push_back(nullptr);
    executionScopeEnum->labels.push_back("Invocation"); executionScopeEnum->values.push_back(nullptr);
    DefaultTypes.push_back(executionScopeEnum);

    Enumeration* memorySemanticsEnum = new Enumeration();
    memorySemanticsEnum->name = "MemorySemantics";
    memorySemanticsEnum->baseType = GPULang::TypeCode::UInt;
    memorySemanticsEnum->labels.push_back("Relaxed"); memorySemanticsEnum->values.push_back(new UIntExpression(0x0));
    memorySemanticsEnum->labels.push_back("Acquire"); memorySemanticsEnum->values.push_back(new UIntExpression(0x1));
    memorySemanticsEnum->labels.push_back("Release"); memorySemanticsEnum->values.push_back(new UIntExpression(0x2));
    memorySemanticsEnum->labels.push_back("AcquireRelease"); memorySemanticsEnum->values.push_back(new UIntExpression(0x4));
    memorySemanticsEnum->labels.push_back("UniformMemory"); memorySemanticsEnum->values.push_back(new UIntExpression(0x8));
    memorySemanticsEnum->labels.push_back("SubgroupMemory"); memorySemanticsEnum->values.push_back(new UIntExpression(0x10));
    memorySemanticsEnum->labels.push_back("WorkgroupMemory"); memorySemanticsEnum->values.push_back(new UIntExpression(0x20));
    memorySemanticsEnum->labels.push_back("DeviceMemory"); memorySemanticsEnum->values.push_back(new UIntExpression(0x40));
    memorySemanticsEnum->labels.push_back("AtomicCounterMemory"); memorySemanticsEnum->values.push_back(new UIntExpression(0x80));
    memorySemanticsEnum->labels.push_back("ImageMemory"); memorySemanticsEnum->values.push_back(new UIntExpression(0x100));
    DefaultTypes.push_back(memorySemanticsEnum);

    __MAKE_TYPE(void, TypeCode::Void);
}

std::map<TypeCode, std::vector<std::string>> singleComponentToVectorMap =
{
    { TypeCode::Float,      { "f32",    "f32x2",    "f32x3",    "f32x4" } }
    , { TypeCode::Float16,  { "f16",    "f16x2",    "f16x3",    "f16x4" } }
    , { TypeCode::Int,      { "i32",    "i32x2",    "i32x3",    "i32x4" } }
    , { TypeCode::Int16,    { "i16",    "i16x2",    "i16x3",    "i16x4" } }
    , { TypeCode::UInt,     { "u32",    "u32x2",    "u32x3",    "u32x4" } }
    , { TypeCode::UInt16,   { "u16",    "u16x2",    "u16x3",    "u16x4" } }
    , { TypeCode::Bool,     { "b8",     "b8x2",     "b8x3",     "b8x4"  } }

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
uint32_t 
Type::CalculateSize() const
{
    return this->byteSize * this->columnSize * this->rowSize;
}

//------------------------------------------------------------------------------
/**
    Rounds up to next power of 2
*/
__forceinline unsigned int
roundtopow2(unsigned int val)
{
    val--;
    val |= val >> 1;
    val |= val >> 2;
    val |= val >> 4;
    val |= val >> 8;
    val |= val >> 16;
    val++;
    return val;
}

//------------------------------------------------------------------------------
/**
*/
uint32_t 
Type::CalculateAlignment() const
{
    uint32_t baseAlignment = this->byteSize;
    uint32_t roundedColumns = roundtopow2(this->columnSize);
    return baseAlignment * roundedColumns;
}

//------------------------------------------------------------------------------
/**
*/
uint32_t 
Type::Align(uint32_t alignant, uint32_t alignment)
{
    return (alignant + alignment - 1) & ~(alignment - 1);
}

//------------------------------------------------------------------------------
/**
*/
bool 
Type::StringToSwizzleMask(const std::string& str, SwizzleMask& out)
{
    // convert to swizzle mask
    out.bits.x = SwizzleMask::Invalid;
    out.bits.y = SwizzleMask::Invalid;
    out.bits.z = SwizzleMask::Invalid;
    out.bits.w = SwizzleMask::Invalid;
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
                        out.bits.x = SwizzleMask::X;
                        break;
                    case 1:
                        out.bits.y = SwizzleMask::X;
                        break;
                    case 2:
                        out.bits.z = SwizzleMask::X;
                        break;
                    case 3:
                        out.bits.w = SwizzleMask::X;
                        break;
                }
                break;
            case 'y':
            case 'g':
                switch (i)
                {
                    case 0:
                        out.bits.x = SwizzleMask::Y;
                        break;
                    case 1:
                        out.bits.y = SwizzleMask::Y;
                        break;
                    case 2:
                        out.bits.z = SwizzleMask::Y;
                        break;
                    case 3:
                        out.bits.w = SwizzleMask::Y;
                        break;
                }
                break;
            case 'z':
            case 'b':
                switch (i)
                {
                    case 0:
                        out.bits.x = SwizzleMask::Z;
                        break;
                    case 1:
                        out.bits.y = SwizzleMask::Z;
                        break;
                    case 2:
                        out.bits.z = SwizzleMask::Z;
                        break;
                    case 3:
                        out.bits.w = SwizzleMask::Z;
                        break;
                }
                break;
            case 'w':
            case 'a':
                switch (i)
                {
                    case 0:
                        out.bits.x = SwizzleMask::W;
                        break;
                    case 1:
                        out.bits.y = SwizzleMask::W;
                        break;
                    case 2:
                        out.bits.z = SwizzleMask::W;
                        break;
                    case 3:
                        out.bits.w = SwizzleMask::W;
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
    if (mask.bits.x != SwizzleMask::Invalid)
        numComponents++;
    if (mask.bits.y != SwizzleMask::Invalid)
        numComponents++;
    if (mask.bits.z != SwizzleMask::Invalid)
        numComponents++;
    if (mask.bits.w != SwizzleMask::Invalid)
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
    ret = max(mask.bits.x & ~SwizzleMask::Invalid, ret);
    ret = max(mask.bits.y & ~SwizzleMask::Invalid, ret);
    ret = max(mask.bits.z & ~SwizzleMask::Invalid, ret);
    ret = max(mask.bits.w & ~SwizzleMask::Invalid, ret);
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

} // namespace GPULang
