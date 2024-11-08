//------------------------------------------------------------------------------
//  @file type.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "type.h"
#include "builtins.h"
#include "ast/expressions/expression.h"
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


#define __MAKE_TEX()\
newType->category = Type::TextureCategory;

#define __MAKE_PIXELCACHE()\
newType->category = Type::PixelCacheCategory;

#define __MAKE_SAMPLER()\
newType->category = Type::SamplerCategory;

#define __MAKE_TYPE_CUSTOM(t1, t2)\
newType = new t2();\
newType->name = #t1;\
DefaultTypes.push_back(newType);

#define __ADD_LOOKUP(name) DefaultTypes[#name] = newType;

static const std::unordered_map<std::string, ImageFormat> StringToFormats =
{
    { "rgba16", Rgba16 }
    , { "rgb10_a2", Rgb10_A2 }
    , { "rgba8", Rgba8 }
    , { "rg16", Rg16 }
    , { "rg8", Rg8 }
    , { "r16", R16 }
    , { "r8", R8 }
    , { "rgba16_snorm", Rgba16_Snorm }
    , { "rgba8_snorm", Rgba8_Snorm }
    , { "rg16_snorm", Rg16_Snorm }
    , { "rg8_snorm", Rg8_Snorm }
    , { "r16_snorm", R16_Snorm }
    , { "r8_snorm", R8_Snorm }
    , { "rgba32f", Rgba32F }
    , { "rgba16f", Rgba16F }
    , { "rg32f", Rg32F }
    , { "rg16f", Rg16F }
    , { "r11g11b10f", R11G11B10F }
    , { "r32f", R32F }
    , { "r16f", R16F }
    , { "rga32i", Rgba32I }
    , { "rgba16i", Rgba16I }
    , { "rgba8i", Rgba8I }
    , { "rg32i", Rg32I }
    , { "rg16i", Rg16I }
    , { "rg8i", Rg8I }
    , { "r32i", R32I }
    , { "r16i", R16I }
    , { "r8i", R8I }
    , { "rga32u", Rgba32U }
    , { "rgba16u", Rgba16U }
    , { "rgb10_a2u", Rgb10_A2U }
    , { "rgba8u", Rgba8U }
    , { "rg32u", Rg32U }
    , { "rg16u", Rg16U }
    , { "rg8u", Rg8U }
    , { "r32u", R32U }
    , { "r16u", R16U }
    , { "r8u", R8U }
    , { "unknown", Unknown }
};


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

    Enumeration* compareModeEnum = new Enumeration();
    compareModeEnum->name = "CompareMode";
    compareModeEnum->type = { "u32" };
    compareModeEnum->type.literal = true;
    compareModeEnum->baseType = GPULang::TypeCode::UInt;
    compareModeEnum->labels.push_back("InvalidCompareMode"); compareModeEnum->values.push_back(nullptr);
    compareModeEnum->labels.push_back("Never"); compareModeEnum->values.push_back(nullptr);
    compareModeEnum->labels.push_back("Less"); compareModeEnum->values.push_back(nullptr);
    compareModeEnum->labels.push_back("Equal"); compareModeEnum->values.push_back(nullptr);
    compareModeEnum->labels.push_back("LessEqual"); compareModeEnum->values.push_back(nullptr);
    compareModeEnum->labels.push_back("Greater"); compareModeEnum->values.push_back(nullptr);
    compareModeEnum->labels.push_back("NotEqual"); compareModeEnum->values.push_back(nullptr);
    compareModeEnum->labels.push_back("GreaterEqual"); compareModeEnum->values.push_back(nullptr);
    compareModeEnum->labels.push_back("Always"); compareModeEnum->values.push_back(nullptr);
    compareModeEnum->builtin = true;
    DefaultTypes.push_back(compareModeEnum);

    __MAKE_TYPE_CUSTOM(function, GPULang::FunctionType);
    __MAKE_TYPE_CUSTOM(renderState, GPULang::RenderStateType);
    __MAKE_TYPE_CUSTOM(samplerState, GPULang::SamplerStateType);
    __MAKE_TYPE_CUSTOM(program, GPULang::ProgramType);

    __MAKE_TYPE(sampler, TypeCode::Sampler);
    __MAKE_SAMPLER();

    Enumeration* executionScopeEnum = new Enumeration();
    executionScopeEnum->name = "ExecutionScope";
    executionScopeEnum->type = { "u32" };
    executionScopeEnum->type.literal = true;
    executionScopeEnum->baseType = GPULang::TypeCode::UInt;
    executionScopeEnum->labels.push_back("Global"); executionScopeEnum->values.push_back(nullptr);
    executionScopeEnum->labels.push_back("Device"); executionScopeEnum->values.push_back(nullptr);
    executionScopeEnum->labels.push_back("Workgroup"); executionScopeEnum->values.push_back(nullptr);
    executionScopeEnum->labels.push_back("Subgroup"); executionScopeEnum->values.push_back(nullptr);
    executionScopeEnum->labels.push_back("Invocation"); executionScopeEnum->values.push_back(nullptr);
    executionScopeEnum->labels.push_back("Queue"); executionScopeEnum->values.push_back(nullptr);
    executionScopeEnum->builtin = true;
    DefaultTypes.push_back(executionScopeEnum);

    Enumeration* memorySemanticsEnum = new Enumeration();
    memorySemanticsEnum->name = "MemorySemantics";
    memorySemanticsEnum->type = { "u32" };
    memorySemanticsEnum->type.literal = true;
    memorySemanticsEnum->baseType = GPULang::TypeCode::UInt;
    memorySemanticsEnum->labels.push_back("Relaxed"); memorySemanticsEnum->values.push_back(new UIntExpression(0x0));
    memorySemanticsEnum->labels.push_back("Acquire"); memorySemanticsEnum->values.push_back(new UIntExpression(0x1));
    memorySemanticsEnum->labels.push_back("Release"); memorySemanticsEnum->values.push_back(new UIntExpression(0x2));
    memorySemanticsEnum->labels.push_back("AcquireRelease"); memorySemanticsEnum->values.push_back(new UIntExpression(0x4));
    memorySemanticsEnum->builtin = true;
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
    return this->byteSize;
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
    uint32_t baseAlignment = (this->byteSize / this->rowSize) / this->columnSize;
    uint32_t roundedColumns = roundtopow2(this->columnSize);
    return baseAlignment * roundedColumns;
}

//------------------------------------------------------------------------------
/**
*/
uint32_t 
Type::CalculateStride() const
{
    uint32_t baseAlignment = (this->byteSize / this->rowSize) / this->columnSize;
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
    std::string base;
    if (this->literal)
        base.append("literal ");
    if (this->mut)
        base.append("mutable ");
    if (this->sampled)
        base.append("sampled ");
    return Format("%s%s%s", this->signature.c_str(), base.c_str(), this->name.c_str());
}

//------------------------------------------------------------------------------
/**
*/
bool
Type::FullType::Assignable(const Type::FullType& rhs) const
{
    if (this->literal)
        return false;
    if (this->sampled)
        return false;
    if (this->modifiers != rhs.modifiers)
        return false;
    for (size_t i = 0; i < this->modifierValues.size(); i++)
        if (this->modifierValues[i] != rhs.modifierValues[i])
        {
            uint32_t lhsSize, rhsSize;
            if (this->modifierValues[i] != nullptr)
                this->modifierValues[i]->EvalUInt(lhsSize);
            if (rhs.modifierValues[i] != nullptr)
                rhs.modifierValues[i]->EvalUInt(rhsSize);
            if (lhsSize != rhsSize)
                return false;
        }
        else
            return false;
    if (this->name != rhs.name)
        return false;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
Type::FullType::operator==(const FullType& rhs) const
{
    if (this->literal && !rhs.literal)
        return false;
    if (this->mut != rhs.mut)
        return false;
    if (this->sampled != rhs.sampled)
        return false;
    if (this->modifiers != rhs.modifiers)
        return false;
    for (size_t i = 0; i < this->modifierValues.size(); i++)
        if (this->modifierValues[i] != rhs.modifierValues[i])
        {
            uint32_t lhsSize, rhsSize;
            if (this->modifierValues[i] != nullptr)
                this->modifierValues[i]->EvalUInt(lhsSize);
            if (rhs.modifierValues[i] != nullptr)
                rhs.modifierValues[i]->EvalUInt(rhsSize);
            if (lhsSize != rhsSize)
                return false;
        }
    if (this->name != rhs.name)
        return false;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
const bool 
Type::FullType::IsPointer() const
{
    auto it = this->modifiers.crbegin();
    while (it != this->modifiers.crend())
    {
        if (*it == Type::FullType::Modifier::Pointer)
            return true;
        else if (*it == Type::FullType::Modifier::Array)
            return false;
        it++;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
const bool 
Type::FullType::IsMutable() const
{
    return this->mut;
}

//------------------------------------------------------------------------------
/**
*/
const Type::FullType::Modifier 
Type::FullType::LastIndirectionModifier() const
{
    auto it = this->modifiers.crbegin();
    while (it != this->modifiers.crend())
    {
        switch (*it)
        {
            case Type::FullType::Modifier::Array:
            case Type::FullType::Modifier::Pointer:
                return *it;
        }
        it++;
    }
    return Type::FullType::Modifier::Invalid;
}

} // namespace GPULang
