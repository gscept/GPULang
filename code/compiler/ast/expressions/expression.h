#pragma once
//------------------------------------------------------------------------------
/**
    @class GPULang::Expression
    
    Base class for expressions. 
    In AnyFX, expressions are purely aritmetic, meaning they can only return a value.
    
    (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "ast/symbol.h"
#include "ast/types/type.h"

namespace GPULang
{

enum AccessFlags
{
    Const = 0x1,
    LinkTime = 0x2
};

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

/// Convert storage to string
std::string StorageToString(Storage storage);
/// Check if storage is compatible
bool IsStorageCompatible(Storage lhs, Storage rhs);

struct Compiler;
struct Expression : public Symbol
{
    /// constructor
    Expression();
    /// destructor
    virtual ~Expression();

    /// Evaluate expression and store data in resolved
    virtual bool Resolve(Compiler* compiler);

    /// eval type
    virtual bool EvalType(Type::FullType& out) const;
    /// eval symbol
    virtual bool EvalSymbol(std::string& out) const;
    /// evaluates expression as an integer
    virtual bool EvalInt(int& out) const;
    /// evaluates expression as an integer
    virtual bool EvalUInt(unsigned& out) const;
    /// evaulates expression as a float
    virtual bool EvalFloat(float& out) const;
    /// evaluates expression as a boolean
    virtual bool EvalBool(bool& out) const;
    /// evaluates access flags
    virtual bool EvalAccessFlags(unsigned& out) const;
    /// evaluates storage
    virtual bool EvalStorage(Storage& out) const;
    
    /// evaluates expression as a string
    virtual std::string EvalString() const;

    bool isLhsValue;

    struct __Resolved : public Symbol::__Resolved
    {
        union
        {
            int32_t i;
            uint32_t ui;
            float f;
            bool b;
        } value;

        std::string text;
    };
}; 
} // namespace GPULang
//------------------------------------------------------------------------------