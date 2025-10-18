#pragma once
//------------------------------------------------------------------------------
/**
    AST for a struct
            
    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "attribute.h"
#include "variable.h"
#include "types/type.h"
#include "expressions/expression.h"
namespace GPULang
{

struct Structure : public Type
{
    /// constructor
    Structure();
    /// destructor
    virtual ~Structure();

    std::string instanceName;
    Expression* arraySizeExpression;
    bool isArray;

    _IMPLEMENT_ANNOTATIONS()
    _IMPLEMENT_ATTRIBUTES()

    struct __Resolved : public Type::__Resolved
    {
        virtual ~__Resolved();
        union UsageFlags
        {
            struct
            {
                bool isUniformBuffer : 1;
                bool isMutableBuffer : 1;
            } flags;
            int bits;
        };
        UsageFlags usageFlags;
        AccessBits accessBits = 0x0;

        static const uint8_t NOT_BOUND = 0xF;
        //uint32_t group;
        //uint32_t binding;

        // for alignment
        size_t byteSize = 0;
        size_t endPadding = 0;
        size_t baseAlignment = 1;

        bool packMembers = false;
        bool hasBoolMember = false;

        PinnedMap<FixedString, Function*> storageFunctions = 0xFF;
        PinnedMap<FixedString, Function*> storageIndexedFunctions = 0xFF;
        PinnedMap<FixedString, Function*> loadFunctions = 0xFF;
        PinnedMap<FixedString, Function*> loadIndexedFunctions = 0xFF;
        PinnedMap<FixedString, Function*> getReferenceFunctions = 0xFF;
    };
};

} // namespace GPULang
