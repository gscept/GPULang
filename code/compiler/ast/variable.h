#pragma once
//------------------------------------------------------------------------------
/**
    AST variable (type name)

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "attribute.h"
#include "annotation.h"
#include "types/type.h"
#include "shaderusage.h"
#include <vector>
namespace GPULang
{

struct Variable : public Symbol
{
    /// constructor
    Variable();
    /// destructor
    ~Variable();

    std::vector<std::string> values;
    Type::FullType type;
    Expression* valueExpression;
    _IMPLEMENT_ANNOTATIONS()
    _IMPLEMENT_ATTRIBUTES()
    
    struct __Resolved : Symbol::__Resolved
    {
        union AccessBits
        {
            AccessBits(uint32_t mask)
                : bits(mask)
            {}
            struct
            {
                uint32_t readAccess : 1;
                uint32_t writeAccess : 1;
                uint32_t atomicAccess : 1;
                uint32_t volatileAccess : 1;
            } flags;
            uint32_t bits;
        };
        AccessBits accessBits = 0x0;

        union ParameterBits
        {
            ParameterBits(uint32_t mask)
                : bits(mask)
            {}
            struct
            {
                uint32_t isPatch : 1;
                uint32_t isNoPerspective : 1;
                uint32_t isNoInterpolate : 1;
                uint32_t isCentroid : 1;
            } flags;
            uint32_t bits;
        };
        ParameterBits parameterBits = 0x0;

        union UsageBits
        {
            UsageBits(uint32_t mask)
                : bits(mask)
            {}
            struct
            {
                uint32_t isConst : 1;                   // variable may not change value after initialization
                uint32_t isVar : 1;
                uint32_t isParameter : 1;               // variable is a function parameter
                uint32_t isDynamicSizedArray : 1;       // variable type is a dynamically sized array
                uint32_t isEntryPointParameter : 1;
                uint32_t isStructMember : 1;            // variable is a struct member
                uint32_t isProgramMember : 1;           // variable is part of a program
                uint32_t isPadding : 1;
            } flags;
            uint32_t bits;
        };
        UsageBits usageBits = 0x0;

        Storage storage = Storage::Default;
        std::vector<Variable*> siblings;

        /// type here is the fully qualified (pointer and array) type
        Type* typeSymbol;
        Type::FullType type;
        std::string name;

        static const uint8_t NOT_BOUND = 0xF;
        uint32_t group;          // resource group
        uint32_t binding;        // resource binding

        uint8_t inBinding;      // parameter input binding
        uint8_t outBinding;     // parameter output binding

        uint32_t byteSize;          // size in bytes
        uint32_t structureOffset;   // offset within structure
        uint32_t elementPadding;    // padding between elements
        uint32_t startPadding;      // padding before variable

        Function* valueConversionFunction = nullptr;

        ShaderUsage visibilityBits;
        bool builtin = false;

    };
    __Resolved* thisResolved;
};


} // namespace GPULang
