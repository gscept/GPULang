//------------------------------------------------------------------------------
//  expression.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "expression.h"
#include "util.h"
#include "compiler.h"
#include "ast/types/type.h"

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
ConstantString
StorageToString(Storage storage)
{
    switch (storage)
    {
        case Storage::Default:
        case Storage::Global:
            return ""; // Global and default storage means there is no special linkage
        case Storage::Uniform:
            return "uniform";
        case Storage::Workgroup:
            return "workgroup";
        case Storage::Device:
            return "device";
        case Storage::InlineUniform:
            return "inline_uniform";
        case Storage::Input:
            return "in";
        case Storage::Output:
            return "out";
        case Storage::LinkDefined:
            return "link_defined";
        case Storage::RayPayload:
            return "ray_payload";
        case Storage::RayPayloadInput:
            return "in ray_payload";
        case Storage::RayHitAttribute:
            return "ray_hit_attributes";
        case Storage::CallableData:
            return "ray_callable_data";
        case Storage::CallableDataInput:
            return "in ray_callable_data";
    }
    return "";
}

//------------------------------------------------------------------------------
/**
*/
bool
IsStorageCompatible(Storage lhs, Storage rhs)
{
    // All storages which store direct values can write to globals, locals, input output and workgroup
    if (rhs == Storage::Default || rhs == Storage::Global || rhs == Storage::Input || rhs == Storage::Output || rhs == Storage::InlineUniform || rhs == Storage::Uniform || rhs == Storage::Workgroup || rhs == Storage::RayPayloadInput || rhs == Storage::RayPayload || rhs == Storage::RayHitAttribute || rhs == Storage::CallableData || rhs == Storage::CallableDataInput)
    {
        if (lhs == Storage::Default || lhs == Storage::Global || lhs == Storage::Input || lhs == Storage::Output || lhs == Storage::Uniform || lhs == Storage::Workgroup || lhs == Storage::RayPayload || lhs == Storage::CallableData)
            return true;
        return false;
    }
    else
    {
        // Otherwise, storage must match, such as uniform -> uniform or
        return lhs == rhs;
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
StorageRequiresSignature(Storage storage)
{
    switch (storage)
    {
        case Storage::Default:
        case Storage::InlineUniform:
        case Storage::Input:
        case Storage::Output:
        case Storage::LinkDefined:
        case Storage::Global:
        case Storage::Workgroup:
            return false;
        case Storage::Uniform:
        case Storage::Device:
        case Storage::RayPayload:
        case Storage::RayPayloadInput:
        case Storage::RayHitAttribute:
        case Storage::CallableData:
        case Storage::CallableDataInput:
            return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
Expression::Expression() 
    : isLhsValue(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Expression::~Expression()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool 
Expression::Resolve(Compiler* compiler)
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
Expression::EvalType(Type::FullType& out) const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
Expression::EvalTypeSymbol(Type*& out) const
{
    out = nullptr;
    return false;
}


//------------------------------------------------------------------------------
/**
*/
bool
Expression::EvalUnswizzledTypeSymbol(Type*& out) const
{
    return EvalTypeSymbol(out);
}

//------------------------------------------------------------------------------
/**
*/
bool
Expression::EvalSymbol(FixedString& out) const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
Expression::EvalValue(ValueUnion& out) const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool 
Expression::EvalAccessFlags(unsigned& out) const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
Expression::EvalStorage(Storage& out) const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
TransientString
Expression::EvalString() const
{
    return "";
}

} // namespace GPULang
