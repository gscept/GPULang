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
std::string
StorageToString(Storage storage)
{
    switch (storage)
    {
        case Storage::Default:
        case Storage::Global:
            return ""; // Global and default storage means there is no special linkage
            break;
        case Storage::Uniform:
            return "uniform";
            break;
        case Storage::Workgroup:
            return "workgroup";
            break;
        case Storage::Device:
            return "device";
            break;
        case Storage::InlineUniform:
            return "inline_uniform";
            break;
        case Storage::Input:
            return "in";
            break;
        case Storage::Output:
            return "out";
            break;
        case Storage::LinkDefined:
            return "link_defined";
            break;
        case Storage::RayPayload:
            return "ray_payload";
            break;
        case Storage::RayPayloadInput:
            return "in ray_payload";
            break;
        case Storage::RayHitAttribute:
            return "ray_hit_attributes";
            break;
        case Storage::CallableData:
            return "ray_callable_data";
            break;
        case Storage::CallableDataInput:
            return "in ray_callable_data";
            break;
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
IsStorageCompatible(Storage lhs, Storage rhs)
{
    if (lhs == Storage::Default || lhs == Storage::Global)
    {
        if (rhs == Storage::Default || rhs == Storage::Global)
        {
            return true;
        }
        return false;
    }
    else
    {
        return lhs == rhs;
    }
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
Expression::EvalSymbol(std::string& out) const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
Expression::EvalInt(int& out) const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
Expression::EvalUInt(unsigned& out) const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
Expression::EvalFloat(float& out) const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
Expression::EvalBool(bool& out) const
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
std::string
Expression::EvalString() const
{
    return "";
}

} // namespace GPULang