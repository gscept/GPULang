//------------------------------------------------------------------------------
//  @file type.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "type.h"
#include "builtins.h"
#include "ast/expressions/expression.h"
#include "ast/expressions/uintexpression.h"
#include <set>

#if __WIN32__
#define forceinline __forceinline
#else
#define forceinline __attribute__((always_inline))
#endif
namespace GPULang
{

Function* activeFunction = nullptr;

#define __BEGIN_TYPES__ Type* newType = nullptr;

#define __MAKE_TYPE(typename, typecode)\
newType = StaticAlloc<Type>();\
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
newType = StaticAlloc<t2>();\
newType->name = #t1;\
DefaultTypes.push_back(newType);

#define __ADD_LOOKUP(name) DefaultTypes[#name] = newType;

const std::unordered_map<StaticString, ImageFormat> StringToFormats =
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
bool 
IsImageFormatInteger(ImageFormat format)
{
    return format >= Rgba32I && format <= R8I;
}

//------------------------------------------------------------------------------
/**
*/
bool 
IsImageFormatUnsigned(ImageFormat format)
{
    return format >= Rgba32U && format <= R8U;
}

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
    this->resolved = nullptr;
    this->scope.type = Scope::ScopeType::Type;
    this->scope.owningSymbol = this;
}

//------------------------------------------------------------------------------
/**
*/
Type::~Type()
{
}

const StaticMap<TypeCode, StaticString> codeToStringMapping =
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
    , { TypeCode::Texture1DArray, "texture1DArray" }
    , { TypeCode::Texture2D, "texture2D" }
    , { TypeCode::Texture2DArray, "texture2DArray" }
    , { TypeCode::Texture2DMS, "texture2DMS" }
    , { TypeCode::Texture2DMSArray, "texture2DMSArray" }
    , { TypeCode::Texture3D, "texture3D" }
    , { TypeCode::Texture3DArray, "texture3DArray" }
    , { TypeCode::TextureCube, "textureCube" }
    , { TypeCode::TextureCubeArray, "textureCubeArray" }
    , { TypeCode::PixelCache, "pixelCache" }
    , { TypeCode::PixelCacheMS, "pixelCacheMS" }
    , { TypeCode::Sampler, "sampler" }
};

const StaticString NoCode = "";

//------------------------------------------------------------------------------
/**
*/
const StaticString&
Type::CodeToString(const TypeCode& code)
{
    auto it = codeToStringMapping.Find(code);
    if (it == codeToStringMapping.end())
        return NoCode;
    else
        return it->second;
}

const StaticMap<Type::Category, std::string> categoryToStringMapping =
{
    { Type::Category::TextureCategory, "Texture" }
    , { Type::Category::PixelCacheCategory, "PixelCache" }
    , { Type::Category::ScalarCategory, "Scalar" }
    , { Type::Category::UserTypeCategory, "Buffer" }
    , { Type::Category::EnumCategory, "Enum" }
    , { Type::Category::VoidCategory, "Void" }
    , { Type::Category::SamplerCategory, "Sampler" }
    , { Type::Category::AccelerationStructureCategory, "AccelerationStructure" }
};

//------------------------------------------------------------------------------
/**
*/
std::string
Type::CategoryToString(const Category& cat)
{
    auto it = categoryToStringMapping.Find(cat);
    if (it == categoryToStringMapping.end())
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
    auto it = this->scope.symbolLookup.Find(str);
    if (it != this->scope.symbolLookup.end())
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
    auto range = this->scope.symbolLookup.FindRange(str);
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
    SYMBOL_STATIC_ALLOC = true;
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

    __MAKE_TYPE(texture3DArray, TypeCode::Texture3DArray);
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

    __MAKE_TYPE(accelerationStructure, TypeCode::AccelerationStructure);
    newType->category = AccelerationStructureCategory;

#define __ADD_ENUM(val) labels.Append(#val); values.Append(nullptr);
#define __FINISH_ENUM(enum) enum->labels = StaticArray<FixedString>(labels); enum->values = StaticArray<Expression*>(values); labels.size = 0; values.size = 0;

    Enumeration* compareModeEnum = StaticAlloc<Enumeration>();
    compareModeEnum->name = "CompareMode";
    compareModeEnum->type = Type::FullType{ "u32" };
    compareModeEnum->type.literal = true;
    compareModeEnum->baseType = GPULang::TypeCode::UInt;
    StackArray<StaticString> labels(32);
    StackArray<Expression*> values(32);
    __ADD_ENUM(InvalidCompareMode);
    __ADD_ENUM(Never);
    __ADD_ENUM(Less);
    __ADD_ENUM(Equal);
    __ADD_ENUM(LessEqual);
    __ADD_ENUM(Greater);
    __ADD_ENUM(NotEqual);
    __ADD_ENUM(GreaterEqual);
    __ADD_ENUM(Always);
    __FINISH_ENUM(compareModeEnum);
    compareModeEnum->builtin = true;
    DefaultTypes.push_back(compareModeEnum);

    Enumeration* stencilOpEnum = StaticAlloc<Enumeration>();
    stencilOpEnum->name = "StencilOp";
    stencilOpEnum->type = Type::FullType{ "u32" };
    stencilOpEnum->type.literal = true;
    stencilOpEnum->baseType = GPULang::TypeCode::UInt;
    __ADD_ENUM(Invalid);
    __ADD_ENUM(Keep);
    __ADD_ENUM(Zero);
    __ADD_ENUM(Replace);
    __ADD_ENUM(IncrementClamp);
    __ADD_ENUM(DecrementClamp);
    __ADD_ENUM(Invert);
    __ADD_ENUM(IncrementWrap);
    __ADD_ENUM(DecrementWrap);
    __FINISH_ENUM(stencilOpEnum);
    stencilOpEnum->builtin = true;
    DefaultTypes.push_back(stencilOpEnum);

    __MAKE_TYPE_CUSTOM(function, GPULang::FunctionType);
    __MAKE_TYPE_CUSTOM(stencilState, GPULang::StencilStateType);
    __MAKE_TYPE_CUSTOM(renderState, GPULang::RenderStateType);
    __MAKE_TYPE_CUSTOM(samplerState, GPULang::SamplerStateType);
    __MAKE_TYPE_CUSTOM(program, GPULang::ProgramType);

    __MAKE_TYPE(sampler, TypeCode::Sampler);
    __MAKE_SAMPLER();

    Enumeration* executionScopeEnum = StaticAlloc<Enumeration>();
    executionScopeEnum->name = "ExecutionScope";
    executionScopeEnum->type = Type::FullType{ "u32" };
    executionScopeEnum->type.literal = true;
    executionScopeEnum->baseType = GPULang::TypeCode::UInt;
    __ADD_ENUM(Global)
    __ADD_ENUM(Device)
    __ADD_ENUM(Workgroup)
    __ADD_ENUM(Subgroup)
    __ADD_ENUM(Invocation)
    __ADD_ENUM(Queue)
    __FINISH_ENUM(executionScopeEnum);
    executionScopeEnum->builtin = true;
    DefaultTypes.push_back(executionScopeEnum);

    Enumeration* memorySemanticsEnum = StaticAlloc<Enumeration>();
    memorySemanticsEnum->name = "MemorySemantics";
    memorySemanticsEnum->type = Type::FullType{ "u32" };
    memorySemanticsEnum->type.literal = true;
    memorySemanticsEnum->baseType = GPULang::TypeCode::UInt;
    labels.Append("Relaxed"); values.Append(StaticAlloc<UIntExpression>(0x0));
    labels.Append("Acquire"); values.Append(StaticAlloc<UIntExpression>(0x1));
    labels.Append("Release"); values.Append(StaticAlloc<UIntExpression>(0x2));
    labels.Append("AcquireRelease"); values.Append(StaticAlloc<UIntExpression>(0x4));
    __FINISH_ENUM(memorySemanticsEnum);
    memorySemanticsEnum->builtin = true;
    DefaultTypes.push_back(memorySemanticsEnum);

    __MAKE_TYPE(void, TypeCode::Void);
    SYMBOL_STATIC_ALLOC = false;
}

StaticMap<TypeCode, std::vector<std::string>> singleComponentToVectorMap =
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
        return singleComponentToVectorMap.Find(baseType)->second[members - 1];
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
forceinline unsigned int
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
    return baseAlignment * roundedColumns;// * this->rowSize;
}

//------------------------------------------------------------------------------
/**
*/
uint32_t 
Type::CalculateStride() const
{
    uint32_t baseAlignment = (this->byteSize / this->rowSize) / this->columnSize;
    uint32_t roundedColumns = roundtopow2(this->columnSize);
    return baseAlignment * roundedColumns * this->rowSize;
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
Type::FullType::ToString(bool includeLiteral) const
{
    std::string base;
    for (size_t i = 0; i < this->modifiers.size(); i++)
    {
        if (this->modifiers[i] == Modifier::Pointer)
        {
            base.append("*");
        }
        else if (this->modifiers[i] == Modifier::Array)
        {
            if (this->modifierValues[i] == nullptr)
                base.append("[]");
            else
            {
                uint32_t size;
                ValueUnion value;
                this->modifierValues[i]->EvalValue(value);
                value.Store(size);
                base.append(Format("[%d]", size));
            }
        }
    }
    if (this->literal && includeLiteral)
        base.append("literal ");
    if (this->mut)
        base.append("mutable ");
    if (this->sampled)
        base.append("sampled ");
    
    if (!this->swizzleName.empty())
        return Format("%s%s", base.c_str(), this->swizzleName.c_str());
    else
        return Format("%s%s", base.c_str(), this->name.c_str());
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
            uint32_t lhsSize = UINT32_MAX, rhsSize;
            if (this->modifierValues[i] != nullptr)
            {
                ValueUnion value;
                this->modifierValues[i]->EvalValue(value);
                value.Store(lhsSize);
            }
            if (rhs.modifierValues[i] != nullptr)
            {
                ValueUnion value;
                rhs.modifierValues[i]->EvalValue(value);
                value.Store(rhsSize);
            }

            if (lhsSize != rhsSize)
                return false;
        }
        else
            return false;
    std::string lhsName = this->swizzleName.empty() ? this->name : this->swizzleName;
    std::string rhsName = rhs.swizzleName.empty() ? rhs.name : rhs.swizzleName;
    if (lhsName != rhsName)
        return false;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
Type::FullType::Constructible(const FullType& rhs) const
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
            uint32_t lhsSize = UINT32_MAX, rhsSize;
            // If we are constructing an unbound array to a bound sized array, that's fine
            if (this->modifierValues[i] == nullptr && rhs.modifierValues[i] != nullptr)
                continue;
            else
            {
                if (this->modifierValues[i] != nullptr)
                {
                    ValueUnion value;
                    this->modifierValues[i]->EvalValue(value);
                    value.Store(lhsSize);
                }
                if (rhs.modifierValues[i] != nullptr)
                {
                    ValueUnion value;
                    rhs.modifierValues[i]->EvalValue(value);
                    value.Store(rhsSize);
                }

                if (lhsSize != rhsSize)
                    return false;    
            }
        }
        else
            return false;
    std::string lhsName = this->swizzleName.empty() ? this->name : this->swizzleName;
    std::string rhsName = rhs.swizzleName.empty() ? rhs.name : rhs.swizzleName;
    if (lhsName != rhsName)
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
    if (this->mut && !rhs.mut)
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
            {
                ValueUnion value;
                this->modifierValues[i]->EvalValue(value);
                value.Store(lhsSize);
            }
            if (rhs.modifierValues[i] != nullptr)
            {
                ValueUnion value;
                rhs.modifierValues[i]->EvalValue(value);
                value.Store(rhsSize);
            }
            if (lhsSize != rhsSize)
                return false;
        }
    std::string lhsName = this->swizzleName.empty() ? this->name : this->swizzleName;
    std::string rhsName = rhs.swizzleName.empty() ? rhs.name : rhs.swizzleName;
    if (lhsName != rhsName)
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
