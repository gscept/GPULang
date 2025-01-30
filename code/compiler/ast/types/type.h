#pragma once
//------------------------------------------------------------------------------
/**
    The type symbol is the most primitive representation of a type

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "ast/symbol.h"
#include "util.h"
#include <vector>
#include <unordered_map>
#include <map>

#define STRINGIFY(x) #x

#define __IMPLEMENT_CTOR_1(method, id, t, argtype)\
this->method.name = #id;\
this->method.returnType = {#t};\
this->globals.push_back(&this->method);\
activeFunction = &this->method;\
{\
    Variable* var = new Variable; \
    var->name = "_arg0"; \
    var->type = { #argtype }; \
    activeFunction->parameters.push_back(var); \
}\
this->constructors.push_back(activeFunction);

#define __IMPLEMENT_CTOR(method, id, type)\
this->method.name = #id;\
this->method.returnType = {#type};\
this->globals.push_back(&this->method);\
activeFunction = &this->method;\
this->constructors.push_back(activeFunction);

#define __IMPLEMENT_FUNCTION_1(method, id, t, argtype)\
this->method.name = #id;\
this->method.returnType = {#t};\
this->staticSymbols.push_back(&this->method);\
activeFunction = &this->method;\
{\
    Variable* var = new Variable; \
    var->name = "_arg0"; \
    var->type = { #argtype }; \
    activeFunction->parameters.push_back(var); \
}

#define __IMPLEMENT_FUNCTION(method, id, type)\
this->method.name = #id;\
this->method.returnType = {#type};\
this->staticSymbols.push_back(&this->method);\
activeFunction = &this->method;\


#define __ADD_FUNCTION_LOOKUP(id)\
this->lookup.insert({ #id, activeFunction });

#define __ADD_SWIZZLE(retType, format, ...)\
{\
    Variable* swizzleMember = new Variable;\
    swizzleMember->name = Format(format, __VA_ARGS__);\
    swizzleMember->type = {#retType};\
    Variable::__Resolved* resolved = Symbol::Resolved(swizzleMember);\
    resolved->usageBits.flags.isVar = true;\
    this->swizzleSymbols.push_back(swizzleMember);\
}

#define __ADD_FUNCTION_PARAM(id, t)\
{\
    Variable* var = new Variable;\
    var->name = #id;\
    var->type = {#t};\
    activeFunction->parameters.push_back(var);\
}

#define __ADD_VARIBLE_LOOKUP(variable, id, t)\
this->variable.name = id;\
this->variable.type = {#t};\
this->lookup.insert({id, &this->variable});

#define __ADD_CONSTRUCTOR()\
this->constructors.push_back(activeFunction);

#define __IMPLEMENT_SWIZZLE(type, size, mask)\
    for (char x = 0; x < size; x++)\
    {\
        __ADD_SWIZZLE(type, "%c", mask[x]);\
        for (char y = 0; y < size; y++)\
        {\
            __ADD_SWIZZLE(type##x2, "%c%c", mask[x], mask[y]);\
            for (char z = 0; z < size; z++)\
            {\
                __ADD_SWIZZLE(type##x3, "%c%c%c", mask[x], mask[y], mask[z]);\
                for (char w = 0; w < size; w++)\
                {\
                    __ADD_SWIZZLE(type##x4, "%c%c%c%c", mask[x], mask[y], mask[z], mask[w]);\
                }\
            }\
        }\
    }

namespace GPULang
{

enum ImageFormat
{
    InvalidImageFormat,

    // byte
    Rgba16, Rgb10_A2, Rgba8, Rg16, Rg8,
    R16, R8, Rgba16_Snorm, Rgba8_Snorm, Rg16_Snorm, Rg8_Snorm,
    R16_Snorm, R8_Snorm,

    // float
    Rgba32F, Rgba16F, Rg32F, Rg16F, R11G11B10F,
    R32F, R16F,

    // integer
    Rgba32I, Rgba16I, Rgba8I, Rg32I, Rg16I, Rg8I,
    R32I, R16I, R8I,

    // unsigned integer
    Rgba32U, Rgba16U, Rgb10_A2U, Rgba8U, Rg32U, Rg16U, Rg8U,
    R32U, R16U, R8U,

    // Format is not known
    Unknown
};

extern const std::unordered_map<std::string, ImageFormat> StringToFormats;

extern std::vector<Symbol*> DefaultTypes;
enum class TypeCode
{
    InvalidType

    /// Scalar types, matrix, array and vector types are not a part of the type code
    , Float
    , Float16
    , Int
    , Int16
    , UInt
    , UInt16
    , Bool

    /// Texture types
    , Texture1D
    , Texture2D
    , Texture2DMS
    , Texture3D
    , TextureCube
    , Texture1DArray
    , Texture2DArray
    , Texture2DMSArray
    , TextureCubeArray
    , PixelCache
    , PixelCacheMS

    , Buffer

    /// Sampler
    , Sampler


    /// Other types
    , Void
};

struct Variable;
struct Function;
struct Expression;
struct Compiler;
extern Function* activeFunction;

struct Type : public Symbol
{
    /// constructor
    Type();
    /// destructor
    virtual ~Type();

    /// convert type to string
    static std::string CodeToString(const TypeCode& code);

    enum Category
    {
        InvalidCategory,
        TextureCategory,
        PixelCacheCategory,
        ScalarCategory,
        UserTypeCategory,
        EnumCategory,
        VoidCategory,
        SamplerCategory,
        AccelerationStructureCategory
    };

    static std::string CategoryToString(const Category& cat);

    static TypeCode PromoteTypes(const TypeCode lhs, const TypeCode rhs)
    {
        switch (lhs)
        {
            case TypeCode::UInt:
                switch (rhs)
                {
                    case TypeCode::UInt:
                        return TypeCode::UInt;
                    case TypeCode::Int:
                        return TypeCode::Int;
                    case TypeCode::Float:
                        return TypeCode::Float;
                    case TypeCode::Bool:
                        return TypeCode::UInt;
                }
                break;
            case TypeCode::Int:
                switch (rhs)
                {
                    case TypeCode::UInt:
                        return TypeCode::Int;
                    case TypeCode::Int:
                        return TypeCode::Int;
                    case TypeCode::Float:
                        return TypeCode::Float;
                    case TypeCode::Bool:
                        return TypeCode::Int;
                }
                break;
            case TypeCode::Float:
                switch (rhs)
                {
                    case TypeCode::UInt:
                        return TypeCode::Float;
                    case TypeCode::Int:
                        return TypeCode::Float;
                    case TypeCode::Float:
                        return TypeCode::Float;
                    case TypeCode::Bool:
                        return TypeCode::InvalidType;
                }
                break;
            case TypeCode::Bool:
                switch (rhs)
                {
                    case TypeCode::UInt:
                        return TypeCode::UInt;
                    case TypeCode::Int:
                        return TypeCode::Int;
                    case TypeCode::Float:
                        return TypeCode::InvalidType;
                    case TypeCode::Bool:
                        return TypeCode::Bool;
                }
            break;
        }
        return TypeCode::InvalidType;
    }

    // Per element, the bit defines which other element it should be swizzled to (0, 1, 2, 3)
    struct SwizzleMask
    {
        enum Component
        {
            X = 0,
            Y = 1,
            Z = 2,
            W = 3,
            Invalid = 4
        };
        union
        {
            struct {
                uint32_t x : 8;
                uint32_t y : 8;
                uint32_t z : 8;
                uint32_t w : 8;
            } bits;
            uint32_t mask;
        };

        SwizzleMask() 
        {
            this->bits.x = SwizzleMask::Invalid;
            this->bits.y = SwizzleMask::Invalid;
            this->bits.z = SwizzleMask::Invalid;
            this->bits.w = SwizzleMask::Invalid;
        }

        bool operator==(const SwizzleMask& rhs)
        {
            return this->mask == rhs.mask;
        }

        bool operator!=(const SwizzleMask& rhs)
        {
            return this->mask != rhs.mask;
        }
    };

    /// convert string to swizzle mask, returns true if possible swizzle mask
    static bool StringToSwizzleMask(const std::string& str, SwizzleMask& out);
    /// count set bits in swizzle mask
    static unsigned SwizzleMaskComponents(SwizzleMask mask);
    /// count highest value in swizzle mask
    static unsigned SwizzleMaskBiggestComponent(SwizzleMask mask);

    /// convert from type to vector with count
    static const std::string ToVector(const TypeCode baseType, unsigned dimensions);
    /// returns true if type is vector
    const bool IsVector() const;
    /// returns true if type is matrix
    const bool IsMatrix() const;
    /// Calculate byte size
    uint32_t CalculateSize() const;
    /// Calculate alignment
    uint32_t CalculateAlignment() const;
    /// Calculate stride between elements
    uint32_t CalculateStride() const;

    static uint32_t Align(uint32_t alignant, uint32_t alignment);

    TypeCode baseType;
    uint32_t rowSize;
    uint32_t columnSize;
    uint32_t byteSize;
    Category category;

    struct FullType
    {
        enum class Modifier : uint8_t
        {
            Invalid,
            Array,
            Pointer
        };

        FullType()
        {
            this->name = "unknown";
            this->literal = false;
        }
        FullType(std::string type)
        {
            this->name = type;
            this->literal = false;
        }
        FullType(std::string type, const std::vector<Modifier>& modifiers, const std::vector<Expression*>& modifierValues)
        {
            this->name = type;
            this->modifiers = modifiers;
            this->modifierValues = modifierValues;
            this->literal = false;
        }
        std::string name;
        ImageFormat imageFormat = ImageFormat::Unknown;

        void AddModifier(Modifier type, Expression* value = nullptr)
        {
            this->modifiers.push_back(type);
            this->modifierValues.push_back(value);

            switch (type)
            {
                case Type::FullType::Modifier::Array:
                {
                    if (value == 0)
                        this->signature.append("[]");
                    else
                    {
                        this->signature.append(Format("[%d]", value));
                    }
                    break;
                }
                case Type::FullType::Modifier::Pointer:
                    this->signature.append("*");
                    break;
            }
        }
        
        void AddQualifier(std::string identifier)
        {
            if (identifier == "mutable")
                this->mut = true;
            else if (identifier == "literal")
                this->literal = true;
            else if (identifier == "sampled")
                this->sampled = true;
            else
            {
                auto it = StringToFormats.find(identifier);
                if (it != StringToFormats.end())
                    this->imageFormat = it->second;
                else
                    this->AddModifier(Modifier::Invalid);
            }
        }

        void UpdateValue(Expression* value)
        {
            this->modifierValues.back() = value;
        }

        void ClearModifiers()
        {
            this->modifiers.clear();
            this->modifierValues.clear();
        }
        
        struct
        {
            uint8_t literal: 1 = false;
            uint8_t mut: 1 = false;
            uint8_t sampled: 1 = false;
        };

        std::vector<Modifier> modifiers;
        std::vector<Expression*> modifierValues;
        std::string signature;

        static const uint32_t UNSIZED_ARRAY = 0;

        /// Type assignment follows special rules to equality
        bool Assignable(const FullType& rhs) const;
        bool operator==(const FullType& rhs) const;
        bool operator!=(const FullType& rhs) const
        {
            return !(*this == rhs);
        }

        /// Convert to string
        std::string ToString();
        /// Returns true if top most indirection qualifier is a pointer
        const bool IsPointer() const;
        /// Returns true if top most access qualifier is mutable
        const bool IsMutable() const;
        /// Returns true if top most access qualifier is uniform

        /// Get the last modifier that is an indirectiom modifier
        const Modifier LastIndirectionModifier() const;
    };

    static FullType TypeFromCode(const TypeCode code, uint8_t columnSize = 0, uint8_t rowSize = 0)
    {
        FullType result;
        switch (code)
        {
            case TypeCode::UInt:
                result.name = "u32";
                break;
            case TypeCode::Int:
                result.name = "i32";
                break;
            case TypeCode::Float:
                result.name = "f32";
                break;
            case TypeCode::Bool:
                result.name = "b8";
                break;
        }
        if (columnSize > 1)
        {
            result.name.append(Format("x%d", columnSize));
            if (rowSize > 1)
                result.name.append(Format("x%d", rowSize));
        }
        
        return result;
    }

    std::vector<Symbol*> globals;
    std::vector<Symbol*> staticSymbols;
    std::vector<Symbol*> swizzleSymbols;
    std::vector<Symbol*> symbols;
    std::vector<Symbol*> constructors;
    std::multimap<std::string, Symbol*> lookup;

    /// return member symbol
    Symbol* GetSymbol(const std::string str);
    /// return member symbol
    template<typename T>
    T* GetSymbol(const std::string str);
    /// return member symbols matching string
    std::vector<Symbol*> GetSymbols(const std::string str);

    /// setup all default shader types, like float, int, vec4, etc
    static void SetupDefaultTypes();

};

//------------------------------------------------------------------------------
/**
*/
template<typename T>
inline T*
Type::GetSymbol(const std::string str)
{
    auto it = this->lookup.find(str);
    if (it != this->lookup.end())
        return static_cast<T*>(it->second);
    else
        return nullptr;
}


} // namespace GPULang
