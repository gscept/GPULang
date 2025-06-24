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
};
extern StencilStateType StencilStateTypeType;

extern Variable StencilState_fail;
extern Variable StencilState_pass;
extern Variable StencilState_depthFail;
extern Variable StencilState_compare;
extern Variable StencilState_compareMask;
extern Variable StencilState_writeMask;
extern Variable StencilState_referenceMask;

} // namespace GPULang
