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

namespace GPULang
{

enum class Storage
{
    Default,                                // default storage, on the stack
    Uniform,                                // variable is uniform (const) across all threads and provided by the CPU
    Workgroup,                              // variable is shared by workgroup and can be written/read
    Device,                                 // variable is visible across all workgroups
    InlineUniform,                          // variable is uniform but read from command buffer
    Input,                                  // variable is an input from a previous shader stage
    Output,                                 // variable is an output from the current shader stage
    Global,                                 // variable is global in the shader
    LinkDefined,                            // variable value is defined at link time
    RayPayload,                             // variable is a ray payload
    RayPayloadInput,                        // variable is a ray payload input
    RayHitAttribute,                        // variable ray tracing hit attribute (barycentrics)
    CallableData,                           // variable is ray tracing callable data
    CallableDataInput,                      // variable is ray tracing callable data
};
constexpr StaticMap StorageToStrings =
std::array{
    std::pair{ Storage::Default, ""_c }
    , std::pair{ Storage::Uniform, "uniform"_c }
    , std::pair{ Storage::Workgroup, "workgroup"_c }
    , std::pair{ Storage::Device, "device"_c }
    , std::pair{ Storage::InlineUniform, "inline_uniform"_c }
    , std::pair{ Storage::Input, "in"_c }
    , std::pair{ Storage::Output, "out"_c }
    , std::pair{ Storage::Global, "global"_c }
    , std::pair{ Storage::LinkDefined, "link_defined"_c }
    , std::pair{ Storage::RayPayload, "ray_payload"_c }
    , std::pair{ Storage::RayPayloadInput, "in ray_payload"_c }
    , std::pair{ Storage::RayHitAttribute, "ray_hit_attributes"_c }
    , std::pair{ Storage::CallableData, "ray_callable_data"_c }
    , std::pair{ Storage::CallableDataInput, "in ray_callable_data"_c }
};

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

    // 64 bit formats
    R64U, R64I,

    // Format is not known
    Unknown
};

extern bool IsImageFormatInteger(ImageFormat format);
extern bool IsImageFormatUnsigned(ImageFormat format);
constexpr StaticMap StringToFormats =
std::array{
    std::pair{ "rgba16"_c, Rgba16 }
    , std::pair{ "rgb10_a2"_c, Rgb10_A2 }
    , std::pair{ "rgba8"_c, Rgba8 }
    , std::pair{ "rg16"_c, Rg16 }
    , std::pair{ "rg8"_c, Rg8 }
    , std::pair{ "r16"_c, R16 }
    , std::pair{ "r8"_c, R8 }
    , std::pair{ "rgba16_snorm"_c, Rgba16_Snorm }
    , std::pair{ "rgba8_snorm"_c, Rgba8_Snorm }
    , std::pair{ "rg16_snorm"_c, Rg16_Snorm }
    , std::pair{ "rg8_snorm"_c, Rg8_Snorm }
    , std::pair{ "r16_snorm"_c, R16_Snorm }
    , std::pair{ "r8_snorm"_c, R8_Snorm }
    , std::pair{ "rgba32f"_c, Rgba32F }
    , std::pair{ "rgba16f"_c, Rgba16F }
    , std::pair{ "rg32f"_c, Rg32F }
    , std::pair{ "rg16f"_c, Rg16F }
    , std::pair{ "r11g11b10f"_c, R11G11B10F }
    , std::pair{ "r32f"_c, R32F }
    , std::pair{ "r16f"_c, R16F }
    , std::pair{ "rga32i"_c, Rgba32I }
    , std::pair{ "rgba16i"_c, Rgba16I }
    , std::pair{ "rgba8i"_c, Rgba8I }
    , std::pair{ "rg32i"_c, Rg32I }
    , std::pair{ "rg16i"_c, Rg16I }
    , std::pair{ "rg8i"_c, Rg8I }
    , std::pair{ "r32i"_c, R32I }
    , std::pair{ "r16i"_c, R16I }
    , std::pair{ "r8i"_c, R8I }
    , std::pair{ "rga32u"_c, Rgba32U }
    , std::pair{ "rgba16u"_c, Rgba16U }
    , std::pair{ "rgb10_a2u"_c, Rgb10_A2U }
    , std::pair{ "rgba8u"_c, Rgba8U }
    , std::pair{ "rg32u"_c, Rg32U }
    , std::pair{ "rg16u"_c, Rg16U }
    , std::pair{ "rg8u"_c, Rg8U }
    , std::pair{ "r32u"_c, R32U }
    , std::pair{ "r16u"_c, R16U }
    , std::pair{ "r8u"_c, R8U }
    , std::pair{ "unknown"_c, Unknown }
};


enum class TypeCode
{
    InvalidType

    /// Scalar types, matrix, array and vector types are not a part of the type code
    , Float
    , Float32 = Float
    , Float16
    , Int
    , Int32 = Int
    , Int16
    , UInt
    , UInt32 = UInt
    , UInt64
    , UInt16
    , Bool
    , Bool8 = Bool

    /// Texture types
    , Texture1D
    , Texture2D
    , Texture2DMS
    , Texture3D
    , TextureCube
    , Texture1DArray
    , Texture2DArray
    , Texture2DMSArray
    , Texture3DArray
    , TextureCubeArray
    , SampledTexture1D
    , SampledTexture2D
    , SampledTexture2DMS
    , SampledTexture3D
    , SampledTextureCube
    , SampledTexture1DArray
    , SampledTexture2DArray
    , SampledTexture2DMSArray
    , SampledTexture3DArray
    , SampledTextureCubeArray
    , PixelCache
    , PixelCacheMS
    , AccelerationStructure
    
    , TexelPointer

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
static constexpr auto UNDEFINED_TYPE = "<undefined>"_c;
static constexpr auto FUNCTION_TYPE = "Function"_c;


struct Type : public Symbol
{
    /// constructor
    Type();
    /// destructor
    virtual ~Type();

    /// convert type to string
    static const ConstantString& CodeToString(const TypeCode& code);

    struct __Resolved : public Symbol::__Resolved
    {
        size_t baseAlignment = 4; // Smallest GPU unit is going to be 4 bytes
    };

    enum Category
    {
        InvalidCategory,
        TextureCategory,
        SampledTextureCategory,
        PixelCacheCategory,
        ScalarCategory,
        StructureCategory,
        EnumCategory,
        VoidCategory,
        SamplerStateCategory,
        AccelerationStructureCategory,
        TexelPointerCategory,

        // Special categories
        RenderStateCategory,
        StencilStateCategory,
        ProgramCategory
    };

    static ConstantString CategoryToString(const Category& cat);

    static TypeCode PromoteTypes(const TypeCode lhs, const TypeCode rhs)
    {
        switch (lhs)
        {
            case TypeCode::UInt:
                switch (rhs)
                {
                    case TypeCode::UInt:
                        return TypeCode::UInt;
                    case TypeCode::UInt16:
                        return TypeCode::UInt;
                    case TypeCode::Int:
                        return TypeCode::UInt;
                    case TypeCode::Int16:
                        return TypeCode::UInt;
                    case TypeCode::Float:
                        return TypeCode::Float;
                    case TypeCode::Float16:
                        return TypeCode::Float;
                    case TypeCode::Bool:
                        return TypeCode::UInt;
                }
                break;
            case TypeCode::UInt16:
                switch (rhs)
                {
                    case TypeCode::UInt:
                        return TypeCode::UInt;
                    case TypeCode::UInt16:
                        return TypeCode::UInt16;
                    case TypeCode::Int:
                        return TypeCode::UInt;
                    case TypeCode::Int16:
                        return TypeCode::UInt16;
                    case TypeCode::Float:
                        return TypeCode::Float;
                    case TypeCode::Float16:
                        return TypeCode::Float;
                    case TypeCode::Bool:
                        return TypeCode::UInt;
                }
                break;
            case TypeCode::Int:
                switch (rhs)
                {
                    case TypeCode::UInt:
                        return TypeCode::UInt;
                    case TypeCode::UInt16:
                        return TypeCode::UInt;
                    case TypeCode::Int:
                        return TypeCode::Int;
                    case TypeCode::Int16:
                        return TypeCode::Int;
                    case TypeCode::Float:
                        return TypeCode::Float;
                    case TypeCode::Float16:
                        return TypeCode::Float;
                    case TypeCode::Bool:
                        return TypeCode::Int;
                }
                break;
            case TypeCode::Int16:
                switch (rhs)
                {
                    case TypeCode::UInt:
                        return TypeCode::UInt;
                    case TypeCode::UInt16:
                        return TypeCode::UInt16;
                    case TypeCode::Int:
                        return TypeCode::Int;
                    case TypeCode::Int16:
                        return TypeCode::Int16;
                    case TypeCode::Float:
                        return TypeCode::Float;
                    case TypeCode::Float16:
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
                    case TypeCode::UInt16:
                        return TypeCode::Float;
                    case TypeCode::Int:
                        return TypeCode::Float;
                    case TypeCode::Int16:
                        return TypeCode::Float;
                    case TypeCode::Float:
                        return TypeCode::Float;
                    case TypeCode::Float16:
                        return TypeCode::Float;
                    case TypeCode::Bool:
                        return TypeCode::InvalidType;
                }
                break;
            case TypeCode::Float16:
                switch (rhs)
                {
                    case TypeCode::UInt:
                        return TypeCode::Float;
                    case TypeCode::UInt16:
                        return TypeCode::Float;
                    case TypeCode::Int:
                        return TypeCode::Float;
                    case TypeCode::Int16:
                        return TypeCode::Float;
                    case TypeCode::Float:
                        return TypeCode::Float;
                    case TypeCode::Float16:
                        return TypeCode::Float16;
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
    static bool StringToSwizzleMask(const FixedString& str, SwizzleMask& out);
    /// count set bits in swizzle mask
    static unsigned SwizzleMaskComponents(SwizzleMask mask);
    /// count highest value in swizzle mask
    static unsigned SwizzleMaskBiggestComponent(SwizzleMask mask);

    /// convert from type to vector with count
    static const ConstantString ToVector(const TypeCode baseType, unsigned dimensions);
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
    bool builtin;

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
            this->name = UNDEFINED_TYPE;
            this->literal = false;
        }
        
        explicit FullType(const char* type)
            : name(type)
            , literal(false)
        {
        }
        
        explicit FullType(const std::string& type)
            : name(type)
            , literal(false)
        {
        }
        
        explicit FullType(const FixedString& type)
            : name(type)
            , literal(false)
        {
        }
        
        explicit FullType(const FixedString& type, bool literal)
            : name(type)
            , literal(literal)
        {
        }
        
        explicit FullType(const TransientString& type)
            : name(type)
            , literal(false)
        {
        }
        
        explicit FullType(const ConstantString& type)
            : name(type)
            , literal(false)
        {
        }
        
        explicit FullType(const ConstantString& type, const TransientArray<Modifier>& modifiers, const TransientArray<Expression*>& modifierValues)
            : name(type)
            , literal(false)
            , modifiers(modifiers)
            , modifierValues(modifierValues)
        {
        }

        explicit FullType(const FixedString& type, const TransientArray<Modifier>& modifiers, const TransientArray<Expression*>& modifierValues)
            : name(type)
            , literal(false)
            , modifiers(modifiers)
            , modifierValues(modifierValues)
        {
        }
        
        explicit FullType(const char* type, const TransientArray<Modifier>& modifiers, const TransientArray<Expression*>& modifierValues)
            : name(type)
            , literal(false)
            , modifiers(modifiers)
            , modifierValues(modifierValues)
        {
        }
        
        explicit FullType(const std::string& type, const TransientArray<Modifier>& modifiers, const TransientArray<Expression*>& modifierValues)
            : name(type)
            , literal(false)
            , modifiers(modifiers)
            , modifierValues(modifierValues)
        {
        }
        
        FixedString name;
        FixedString swizzleName;
        Type::SwizzleMask swizzleMask;
        ImageFormat imageFormat = ImageFormat::Unknown;
        
        Symbol::Location modifierLocation, formatLocation, nameLocation;

        const FixedString& Name()
        {
            return this->swizzleName.len == 0 ? this->name : this->swizzleName;
        }

        struct
        {
            uint8_t literal: 1 = false;
            uint8_t mut: 1 = false;
            uint8_t strict: 1 = false;
            uint8_t address : 1 = false;
        };


        FixedArray<Modifier> modifiers;
        FixedArray<Expression*> modifierValues;

        static const uint32_t UNSIZED_ARRAY = 0;

        /// Type assignment follows special rules to equality
        bool Assignable(const FullType& rhs) const;
        /// Type construction follows sliglthy different rules
        bool Constructible(const FullType& rhs) const;
        bool operator==(const FullType& rhs) const;
        bool operator!=(const FullType& rhs) const
        {
            return !(*this == rhs);
        }

        /// Convert to string
        TransientString ToString(bool includeLiteral = false) const;
        /// Returns true if top most indirection qualifier is a pointer
        const bool IsPointer() const;
        /// Returns true if top most access qualifier is mutable
        const bool IsMutable() const;
        /// Returns true if top most access qualifier is uniform

        /// Get the last modifier that is an indirectiom modifier
        const Modifier LastIndirectionModifier() const;
        /// Get the first modifier
        const Modifier FirstIndirectionModifier() const;
    };

    static FullType TypeFromCode(const TypeCode code, uint8_t columnSize = 0, uint8_t rowSize = 0)
    {
        FullType result;
        TStr name;
        switch (code)
        {
            case TypeCode::UInt:
                name.Append("u32");
                break;
            case TypeCode::UInt16:
                name.Append("u16");
                break;
            case TypeCode::Int:
                name.Append("i32");
                break;
            case TypeCode::Int16:
                name.Append("i16");
                break;
            case TypeCode::Float:
                name.Append("f32");
                break;
            case TypeCode::Float16:
                name.Append("f16");
                break;
            case TypeCode::Bool:
                name.Append("b8");
                break;
            default:
                break;
        }
        if (columnSize > 1)
        {
            name.Concatenate<false>("x", columnSize);
            if (rowSize > 1)
                name.Concatenate<false>("x", rowSize);
        }
        result.name = name;
        return result;
    }

    FixedArray<Symbol*> globals;
    FixedArray<Symbol*> staticSymbols;
    FixedArray<Symbol*> swizzleSymbols;
    PinnedArray<Symbol*> symbols = 0xFFF;
    FixedArray<Symbol*> constructors;

    Scope scope;

    /// return member symbol
    Symbol* GetSymbol(const FixedString& str);
    /// return member symbol
    template<typename T> T* GetSymbol(const FixedString& str);
    /// return member symbols matching string
    std::vector<Symbol*> GetSymbols(const FixedString& str);
    /// return member symbol
    Symbol* GetSymbol(const TransientString& str);
    /// return member symbol
    template<typename T> T* GetSymbol(const TransientString& str);
    /// return member symbols matching string
    std::vector<Symbol*> GetSymbols(const TransientString& str);

};

extern Type::FullType UndefinedType;

//------------------------------------------------------------------------------
/**
*/
template<typename T>
inline T*
Type::GetSymbol(const FixedString& str)
{
    auto it = this->scope.symbolLookup.Find(HashString(str));
    if (it != this->scope.symbolLookup.end())
        return static_cast<T*>(it->second);
    else
        return nullptr;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
inline T*
Type::GetSymbol(const TransientString& str)
{
    auto it = this->scope.symbolLookup.Find(HashString(str));
    if (it != this->scope.symbolLookup.end())
        return static_cast<T*>(it->second);
    else
        return nullptr;
}

struct TypeDeclaration
{
    Type::FullType type;
    Symbol::Location location;
};


} // namespace GPULang
