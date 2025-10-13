//------------------------------------------------------------------------------
//  @file state.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "state.h"
#include "ast/expressions/binaryexpression.h"
#include <map>
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
State::~State()
{
    for (auto entry : this->entries)
        entry->~Expression();
}

} // namespace GPULang
