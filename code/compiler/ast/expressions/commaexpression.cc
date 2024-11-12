//------------------------------------------------------------------------------
//  @file commaexpression.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "commaexpression.h"
#include "util.h"
#include "compiler.h"
namespace GPULang 
{

//------------------------------------------------------------------------------
/**
*/
CommaExpression::CommaExpression(Expression* left, Expression* right)
    : left(left)
    , right(right)
{
    this->resolved = Alloc<CommaExpression::__Resolved>();
    this->symbolType = CommaExpressionType;
}

//------------------------------------------------------------------------------
/**
*/
CommaExpression::~CommaExpression()
{
}

//------------------------------------------------------------------------------
/**
*/
bool 
CommaExpression::Resolve(Compiler* compiler)
{
    if (this->isLhsValue)
        this->left->isLhsValue = true;

    if (!this->left->Resolve(compiler))
        return false;
    if (!this->right->Resolve(compiler))
        return false;

    auto thisResolved = Symbol::Resolved(this);
    thisResolved->text = this->EvalString();
    this->left->EvalType(thisResolved->leftType);
    this->right->EvalType(thisResolved->rightType);

    thisResolved->lhsType = compiler->GetSymbol<Type>(thisResolved->leftType.name);
    if (thisResolved->lhsType == nullptr)
    {
        compiler->UnrecognizedTypeError(thisResolved->leftType.name, this);
        return false;
    }
    thisResolved->rhsType = compiler->GetSymbol<Type>(thisResolved->rightType.name);
    if (thisResolved->rhsType == nullptr)
    {
        compiler->UnrecognizedTypeError(thisResolved->rightType.name, this);
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
CommaExpression::EvalType(Type::FullType& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    out = thisResolved->rightType;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
CommaExpression::EvalSymbol(std::string& out) const
{
    return this->right->EvalSymbol(out);
}

//------------------------------------------------------------------------------
/**
*/
std::string
CommaExpression::EvalString() const
{
    std::string left, right;
    left = this->left->EvalString();
    right = this->right->EvalString();
    return Format("%s,%s", left.c_str(), right.c_str());
}

//------------------------------------------------------------------------------
/**
*/
bool 
CommaExpression::EvalAccessFlags(unsigned& out) const
{
    this->right->EvalAccessFlags(out);
    return true;
}

} // namespace GPULang
