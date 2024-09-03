//------------------------------------------------------------------------------
//  @file initializerexpression.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "initializerexpression.h"
#include "util.h"
#include "compiler.h"
#include "uintexpression.h"
namespace AnyFX 
{

//------------------------------------------------------------------------------
/**
*/
InitializerExpression::InitializerExpression(const std::vector<Expression*>& values)
    : values(values)
{
    this->symbolType = InitializerExpressionType;
    this->resolved = new InitializerExpression::__Resolved;
}

//------------------------------------------------------------------------------
/**
*/
InitializerExpression::~InitializerExpression()
{
    for (Expression* expr : this->values)
        delete expr;
}

//------------------------------------------------------------------------------
/**
*/
bool 
InitializerExpression::Resolve(Compiler* compiler)
{
    for (Expression* expr : this->values)
    {
        if (!expr->Resolve(compiler))
            return false;
    }

    auto thisResolved = Symbol::Resolved(this);

    // Then evaluate type
    Type::FullType inner;
    this->values[0]->EvalType(inner);

    // Append array level first
    thisResolved->fullType.name = inner.name;
    thisResolved->fullType.modifiers.push_back(Type::FullType::Modifier::ArrayLevel);
    thisResolved->fullType.modifierValues.push_back(this->values.size());
    thisResolved->fullType.modifiers.insert(thisResolved->fullType.modifiers.end(), inner.modifiers.begin(), inner.modifiers.end());
    thisResolved->fullType.modifierValues.insert(thisResolved->fullType.modifierValues.end(), inner.modifierValues.begin(), inner.modifierValues.end());

    thisResolved->type = compiler->GetSymbol<Type>(thisResolved->fullType.name);
    if (thisResolved->type == nullptr)
    {
        compiler->UnrecognizedTypeError(thisResolved->fullType.name, this);
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
InitializerExpression::EvalType(Type::FullType& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    out = thisResolved->fullType;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
std::string
InitializerExpression::EvalString() const
{
    std::string ret;
    for (Expression* expr : this->values)
    {
        std::string str;
        str = expr->EvalString();
        ret.append(str);
        if (expr != this->values.back())
            ret.append(", ");
    }
    return ret;
}

} // namespace AnyFX
