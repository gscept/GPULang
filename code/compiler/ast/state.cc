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

const StaticMap<std::string, CompareMode> stringToCompareMode =
{
    { "Never", NeverCompare }
    , { "Less", LessCompare }
    , { "Equal", EqualCompare }
    , { "LessEqual", LessEqualCompare }
    , { "LEqual", LessEqualCompare }
    , { "Greater", GreaterCompare }
    , { "NotEqual", NotEqualCompare }
    , { "NEqual", NotEqualCompare }
    , { "GreaterEqual", GreaterEqualCompare }
    , { "GEqual", GreaterEqualCompare }
    , { "Always", AlwaysCompare }
};

//------------------------------------------------------------------------------
/**
*/
const CompareMode 
State::__Resolved::StringToCompareMode(const std::string& str)
{
    auto it = stringToCompareMode.Find(str);
    if (it != stringToCompareMode.end())
        return it->second;
    else
        return InvalidCompareMode;
}

} // namespace GPULang
