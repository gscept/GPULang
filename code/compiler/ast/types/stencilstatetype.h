#pragma once
//------------------------------------------------------------------------------
/**
    Stencil state type type

    @copyright (C) 2025 Gustav Sterbrant
*/  
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
#include "ast/variable.h"
#include "ast/enumeration.h"
#include "ast/structure.h"
namespace GPULang
{

struct StencilStateType : public Type
{
    /// Constructor
    StencilStateType();

    Variable fail;
    Variable pass;
    Variable depthFail;
    Variable compare;
    Variable compareMask;
    Variable writeMask;
    Variable referenceMask;

};

} // namespace GPULang
