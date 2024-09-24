#pragma once
//------------------------------------------------------------------------------
/**
    The type symbol is the most primitive representation of a type

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "ast/symbol.h"
#include <vector>
#include <map>

#define __IMPLEMENT_GLOBAL_1(method, id, t, argtype)\
this->method.name = #id;\
this->method.returnType = {#t};\
this->globals.push_back(&this->method);\
activeFunction = &this->method;\
{\
    Variable* var = new Variable; \
    var->name = "_arg0"; \
    var->type = { #argtype }; \
    activeFunction->parameters.push_back(var); \
}

#define __IMPLEMENT_GLOBAL(method, id, type)\
this->method.name = #id;\
this->method.returnType = {#type};\
this->globals.push_back(&this->method);\
activeFunction = &this->method;\

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
    this->staticSymbols.push_back(swizzleMember);\
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


namespace GPULang
{

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
    , ReadWriteTexture1D
    , ReadWriteTexture2D
    , ReadWriteTexture2DMS
    , ReadWriteTexture3D
    , ReadWriteTextureCube
    , ReadWriteTexture1DArray
    , ReadWriteTexture2DArray
    , ReadWriteTexture2DMSArray
    , ReadWriteTextureCubeArray

    , UniformBuffer
    , ReadWriteBuffer

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
        ReadWriteTextureCategory,
        TextureCategory,
        SampledTextureCategory,
        PixelCacheCategory,
        ScalarCategory,
        UserTypeCategory,
        EnumCategory,
        UniformStructureCategory,
        MutableStructureCategory,
        VoidCategory,
        SamplerCategory
    };

    // Per element, the bit defines which other element it should be swizzled to (0, 1, 2, 3)
    struct SwizzleMask
    {
        enum
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
                uint8_t x : 3;
                uint8_t y : 3;
                uint8_t z : 3;
                uint8_t w : 3;
            } bits;
            uint32_t mask;
        };
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

    TypeCode baseType;
    uint32_t rowSize;
    uint32_t columnSize;
    uint32_t byteSize;
    Category category;

    struct FullType
    {
        std::string name;

        enum class Modifier : uint8_t
        {
            ArrayLevel,
            PointerLevel
        };

        void AddModifier(Modifier type, uint32_t value = 0x0)
        {
            this->modifiers.push_back(type);
            this->modifierValues.push_back(value);
        }

        void UpdateValue(uint32_t value)
        {
            this->modifierValues.back() = value;
        }

        void ClearModifiers()
        {
            this->modifiers.clear();
            this->modifierValues.clear();
        }
        
        std::vector<Modifier> modifiers;
        std::vector<uint32_t> modifierValues;

        static const uint32_t UNSIZED_ARRAY = 0;

        bool operator==(const FullType& rhs) const
        {
            if (this->modifiers != rhs.modifiers)
                return false;
            for (size_t i = 0; i < this->modifierValues.size(); i++)
                if (this->modifierValues[i] != 0                             // 0 means unsized, and thus it accepts anything
                    && this->modifierValues[i] != rhs.modifierValues[i])
                    return false;
            if (this->name != rhs.name)
                return false;
            return true;
        }

        bool operator!=(const FullType& rhs) const
        {
            return !(*this == rhs);
        }

        /// Convert to string
        std::string ToString();
        /// Returns true if type is pointer
        const bool IsPointer() const;
    };

    std::vector<Symbol*> globals;
    std::vector<Symbol*> staticSymbols;
    std::vector<Symbol*> symbols;
    std::vector<Symbol*> constructors;
    std::multimap<std::string, Symbol*> lookup;

    /// return member symbol
    Symbol* GetSymbol(const std::string str);
    /// return member symbols matching string
    std::vector<Symbol*> GetSymbols(const std::string str);

    /// setup all default shader types, like float, int, vec4, etc
    static std::vector<Symbol*> SetupDefaultTypes();

};

} // namespace GPULang
