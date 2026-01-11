//------------------------------------------------------------------------------
//  @file type.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "type.h"
#include "generated/types.h"
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

Type::FullType UndefinedType = Type::FullType{ UNDEFINED_TYPE };
Function* activeFunction = nullptr;

#define __BEGIN_TYPES__ Type* newType = nullptr; TransientArray<Symbol*> types(100);

#define __MAKE_TYPE_CUSTOM(t1, t2)\
newType = &t2##Type;\
newType->name = ConstantString(#t1);\
types.Append(newType);

#define __ADD_LOOKUP(name) DefaultTypes[#name] = newType;


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
    this->builtin = false;
    this->category = Type::InvalidCategory;
    this->resolved = Alloc<Type::__Resolved>();
    this->scope.type = Scope::ScopeType::Type;
    this->scope.owningSymbol = this;
    
}

//------------------------------------------------------------------------------
/**
*/
Type::~Type()
{
}

constexpr StaticMap codeToStringMapping =
std::array{
    std::pair{ TypeCode::Void, ConstantString("void") }
    , std::pair{ TypeCode::Float, ConstantString("f32") }
    , std::pair{ TypeCode::Float16, ConstantString("f16") }
    , std::pair{ TypeCode::Int, ConstantString("i32") }
    , std::pair{ TypeCode::Int16, ConstantString("i16") }
    , std::pair{ TypeCode::UInt, ConstantString("u32") }
    , std::pair{ TypeCode::UInt16, ConstantString("u16") }
    , std::pair{ TypeCode::Bool, ConstantString("b8") }
    , std::pair{ TypeCode::Texture1D, ConstantString("texture1D") }
    , std::pair{ TypeCode::Texture1DArray, ConstantString("texture1DArray") }
    , std::pair{ TypeCode::Texture2D, ConstantString("texture2D") }
    , std::pair{ TypeCode::Texture2DArray, ConstantString("texture2DArray") }
    , std::pair{ TypeCode::Texture2DMS, ConstantString("texture2DMS") }
    , std::pair{ TypeCode::Texture2DMSArray, ConstantString("texture2DMSArray") }
    , std::pair{ TypeCode::Texture3D, ConstantString("texture3D") }
    , std::pair{ TypeCode::Texture3DArray, ConstantString("texture3DArray") }
    , std::pair{ TypeCode::TextureCube, ConstantString("textureCube") }
    , std::pair{ TypeCode::TextureCubeArray, ConstantString("textureCubeArray") }
    , std::pair{ TypeCode::PixelCache, ConstantString("pixelCache") }
    , std::pair{ TypeCode::PixelCacheMS, ConstantString("pixelCacheMS") }
    , std::pair{ TypeCode::Sampler, ConstantString("sampler") }
};

const ConstantString NoCode = "";

//------------------------------------------------------------------------------
/**
*/
const ConstantString&
Type::CodeToString(const TypeCode& code)
{
    auto it = codeToStringMapping.Find(code);
    if (it == codeToStringMapping.end())
        return NoCode;
    else
        return it->second;
}

constexpr StaticMap categoryToStringMapping =
std::array{
    std::pair{ Type::Category::TextureCategory, ConstantString("Texture") }
    , std::pair{ Type::Category::PixelCacheCategory, ConstantString("PixelCache") }
    , std::pair{ Type::Category::ScalarCategory, ConstantString("Scalar") }
    , std::pair{ Type::Category::StructureCategory, ConstantString("Buffer") }
    , std::pair{ Type::Category::EnumCategory, ConstantString("Enum") }
    , std::pair{ Type::Category::VoidCategory, ConstantString("Void") }
    , std::pair{ Type::Category::SamplerStateCategory, ConstantString("Sampler") }
    , std::pair{ Type::Category::AccelerationStructureCategory, ConstantString("AccelerationStructure") }
};

//------------------------------------------------------------------------------
/**
*/
ConstantString
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
Type::GetSymbol(const FixedString& str)
{
    auto it = this->scope.symbolLookup.Find(HashString(str));
    if (it != this->scope.symbolLookup.end())
        return it->second;
    else
        return nullptr;
}

//------------------------------------------------------------------------------
/**
*/
std::vector<Symbol*> 
Type::GetSymbols(const FixedString& str)
{
    std::vector<Symbol*> ret;
    auto range = this->scope.symbolLookup.FindRange(HashString(str));
    for (auto it = range.first; it != range.second; it++)
        ret.push_back((*it).second);
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
Symbol*
Type::GetSymbol(const TransientString& str)
{
    auto it = this->scope.symbolLookup.Find(HashString(str));
    if (it != this->scope.symbolLookup.end())
        return it->second;
    else
        return nullptr;
}

//------------------------------------------------------------------------------
/**
*/
std::vector<Symbol*>
Type::GetSymbols(const TransientString& str)
{
    std::vector<Symbol*> ret;
    auto range = this->scope.symbolLookup.FindRange(HashString(str));
    for (auto it = range.first; it != range.second; it++)
        ret.push_back((*it).second);
    return ret;
}

constexpr StaticMap singleComponentToVectorMap =
std::array{
    std::pair{ TypeCode::Float,      std::array{ ConstantString("f32"),    ConstantString("f32x2"),    ConstantString("f32x3"),    ConstantString("f32x4") } }
    , std::pair{ TypeCode::Float16,  std::array{ ConstantString("f16"),    ConstantString("f16x2"),    ConstantString("f16x3"),    ConstantString("f16x4") } }
    , std::pair{ TypeCode::Int,      std::array{ ConstantString("i32"),    ConstantString("i32x2"),    ConstantString("i32x3"),    ConstantString("i32x4") } }
    , std::pair{ TypeCode::Int16,    std::array{ ConstantString("i16"),    ConstantString("i16x2"),    ConstantString("i16x3"),    ConstantString("i16x4") } }
    , std::pair{ TypeCode::UInt,     std::array{ ConstantString("u32"),    ConstantString("u32x2"),    ConstantString("u32x3"),    ConstantString("u32x4") } }
    , std::pair{ TypeCode::UInt16,   std::array{ ConstantString("u16"),    ConstantString("u16x2"),    ConstantString("u16x3"),    ConstantString("u16x4") } }
    , std::pair{ TypeCode::Bool,     std::array{ ConstantString("b8"),     ConstantString("b8x2"),     ConstantString("b8x3"),     ConstantString("b8x4")  } }

};

//------------------------------------------------------------------------------
/**
*/
const ConstantString
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
Type::StringToSwizzleMask(const FixedString& str, SwizzleMask& out)
{
    // convert to swizzle mask
    out.bits.x = SwizzleMask::Invalid;
    out.bits.y = SwizzleMask::Invalid;
    out.bits.z = SwizzleMask::Invalid;
    out.bits.w = SwizzleMask::Invalid;
    for (size_t i = 0; i < str.len && i <= 4; i++)
    {
        char c = str.buf[i];
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
TransientString
Type::FullType::ToString(bool includeLiteral) const
{
    TransientString base;
    for (size_t i = 0; i < this->modifiers.size; i++)
    {
        if (this->modifiers[i] == Modifier::Pointer)
        {
            base.Append("*");
        }
        else if (this->modifiers[i] == Modifier::Array)
        {
            if (this->modifierValues[i] == nullptr)
                base.Append("[]_");
            else
            {
                uint32_t size;
                ValueUnion value;
                this->modifierValues[i]->EvalValue(value);
                value.Store(size);
                base.Concatenate<false>("[", size, "]_");
            }
        }
    }
    if (this->literal && includeLiteral)
        base.Append("literal ");
    if (this->mut)
        base.Append("mutable ");
    if (this->address)
        base.Append("address ");

    if (this->swizzleName.len > 0)
        return TransientString(base, this->swizzleName);
    else
        return TransientString(base, this->name);
}

//------------------------------------------------------------------------------
/**
*/
bool
Type::FullType::Assignable(const Type::FullType& rhs) const
{
    if (this->literal)
        return false;
    if (this->modifiers != rhs.modifiers)
        return false;
    if (this->address != rhs.address)
        return false;
    for (size_t i = 0; i < this->modifierValues.size; i++)
        if (this->modifierValues[i] != rhs.modifierValues[i])
        {
            uint32_t lhsSize = UINT32_MAX, rhsSize = UINT32_MAX;
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
    FixedString lhsName = this->swizzleName.len == 0 ? this->name : this->swizzleName;
    FixedString rhsName = rhs.swizzleName.len == 0 ? rhs.name : rhs.swizzleName;
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
    if (this->address != rhs.address)
        return false;
    if (this->modifiers != rhs.modifiers)
        return false;
    for (size_t i = 0; i < this->modifierValues.size; i++)
        if (this->modifierValues[i] != rhs.modifierValues[i])
        {
            uint32_t lhsSize = UINT32_MAX, rhsSize = UINT32_MAX;
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
    FixedString lhsName = this->swizzleName.len == 0 ? this->name : this->swizzleName;
    FixedString rhsName = rhs.swizzleName.len == 0 ? rhs.name : rhs.swizzleName;
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
    if (this->strict || rhs.strict)
    {
        if (this->mut != rhs.mut)
            return false;
    }
    else
    {
        if (this->mut && !rhs.mut)
            return false;
    }
    if (this->address != rhs.address)
        return false; 
    if (this->modifiers != rhs.modifiers)
        return false;
    if (this->modifierValues.size != rhs.modifierValues.size)
        return false;
    for (size_t i = 0; i < this->modifierValues.size; i++)
        if (this->modifierValues[i] != rhs.modifierValues[i])
        {
            uint32_t lhsSize = UINT32_MAX, rhsSize = UINT32_MAX;
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
    FixedString lhsName = this->swizzleName.len == 0 ? this->name : this->swizzleName;
    FixedString rhsName = rhs.swizzleName.len == 0 ? rhs.name : rhs.swizzleName;
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
    auto it = this->modifiers.rbegin();
    while (it != this->modifiers.rend())
    {
        if (*it.it == Type::FullType::Modifier::Pointer)
            return true;
        else if (*it.it == Type::FullType::Modifier::Array)
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
    auto it = this->modifiers.rbegin();
    while (it != this->modifiers.rend())
    {
        switch (*it.it)
        {
            case Type::FullType::Modifier::Array:
            case Type::FullType::Modifier::Pointer:
                return *it.it;
        }
        it++;
    }
    return Type::FullType::Modifier::Invalid;
}

//------------------------------------------------------------------------------
/**
*/
const Type::FullType::Modifier 
Type::FullType::FirstIndirectionModifier() const
{
    auto it = this->modifiers.begin();
    while (it != this->modifiers.end())
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
