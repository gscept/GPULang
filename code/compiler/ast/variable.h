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
#include <set>
#include <vector>
namespace GPULang
{

struct Variable : public Symbol
{
    /// constructor
    Variable();
    /// destructor
    virtual ~Variable();
    
    Type::FullType type;
    Expression* valueExpression;
    _IMPLEMENT_ANNOTATIONS()
    _IMPLEMENT_ATTRIBUTES()
    
    struct __Resolved : Symbol::__Resolved
    {
        virtual ~__Resolved() {};
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
                uint32_t isSample : 1;
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
                uint32_t isNoReflect : 1;               // variable should not be emitted in reflection
            } flags;
            uint32_t bits;
        };
        UsageBits usageBits = 0x0;

        Storage storage = Storage::Default;

        /// type here is the fully qualified (pointer and array) type
        Type* typeSymbol = nullptr;

        static const uint8_t NOT_BOUND = 0xFF;
        uint8_t group = Variable::__Resolved::NOT_BOUND;          // resource group
        uint8_t binding = Variable::__Resolved::NOT_BOUND;        // resource binding

        uint8_t inBinding = Variable::__Resolved::NOT_BOUND;      // parameter input binding
        uint8_t outBinding = Variable::__Resolved::NOT_BOUND;     // parameter output binding

        uint32_t byteSize = 0;          // size in bytes
        uint32_t structureOffset = 0;   // offset within structure
        uint32_t elementPadding = 0;    // padding between elements
        uint32_t startPadding = 0;      // padding before variable

        Function* valueConversionFunction = nullptr;
        Function* traceRayFunction = nullptr;

        ShaderUsage visibilityBits;
        Domain domain;
        bool builtin = false;
    } variableResolved;
    __Resolved* thisResolved;
};


} // namespace GPULang
