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

constexpr StaticMap stringToCompareMode =
std::array{
    std::pair{ ConstantString("Never"), Serialization::CompareMode::NeverCompare }
    , std::pair{ ConstantString("Less"), Serialization::CompareMode::LessCompare }
    , std::pair{ ConstantString("Equal"), Serialization::CompareMode::EqualCompare }
    , std::pair{ ConstantString("LessEqual"), Serialization::CompareMode::LessEqualCompare }
    , std::pair{ ConstantString("LEqual"), Serialization::CompareMode::LessEqualCompare }
    , std::pair{ ConstantString("Greater"), Serialization::CompareMode::GreaterCompare }
    , std::pair{ ConstantString("NotEqual"), Serialization::CompareMode::NotEqualCompare }
    , std::pair{ ConstantString("NEqual"), Serialization::CompareMode::NotEqualCompare }
    , std::pair{ ConstantString("GreaterEqual"), Serialization::CompareMode::GreaterEqualCompare }
    , std::pair{ ConstantString("GEqual"), Serialization::CompareMode::GreaterEqualCompare }
    , std::pair{ ConstantString("Always"), Serialization::CompareMode::AlwaysCompare }
};

//------------------------------------------------------------------------------
/**
*/
const Serialization::CompareMode
State::__Resolved::StringToCompareMode(const TransientString& str)
{
    auto it = stringToCompareMode.Find(str);
    if (it != stringToCompareMode.end())
        return it->second;
    else
        return Serialization::CompareMode::InvalidCompareMode;
}

} // namespace GPULang
