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

const StaticMap stringToCompareMode =
std::array{
    std::pair{ ConstantString("Never"), NeverCompare }
    , std::pair{ ConstantString("Less"), LessCompare }
    , std::pair{ ConstantString("Equal"), EqualCompare }
    , std::pair{ ConstantString("LessEqual"), LessEqualCompare }
    , std::pair{ ConstantString("LEqual"), LessEqualCompare }
    , std::pair{ ConstantString("Greater"), GreaterCompare }
    , std::pair{ ConstantString("NotEqual"), NotEqualCompare }
    , std::pair{ ConstantString("NEqual"), NotEqualCompare }
    , std::pair{ ConstantString("GreaterEqual"), GreaterEqualCompare }
    , std::pair{ ConstantString("GEqual"), GreaterEqualCompare }
    , std::pair{ ConstantString("Always"), AlwaysCompare }
};

//------------------------------------------------------------------------------
/**
*/
const CompareMode 
State::__Resolved::StringToCompareMode(const TransientString& str)
{
    auto it = stringToCompareMode.Find(str);
    if (it != stringToCompareMode.end())
        return it->second;
    else
        return InvalidCompareMode;
}

} // namespace GPULang
