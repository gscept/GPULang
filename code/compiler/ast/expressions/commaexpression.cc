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
    this->left->~Expression();
    this->right->~Expression();
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
    this->left->EvalType(thisResolved->leftType);
    this->right->EvalType(thisResolved->rightType);

    thisResolved->lhsType = compiler->GetType(thisResolved->leftType);
    if (thisResolved->lhsType == nullptr)
    {
        compiler->UnrecognizedTypeError(TransientString(thisResolved->leftType.name), this);
        return false;
    }
    thisResolved->rhsType = compiler->GetType(thisResolved->rightType);
    if (thisResolved->rhsType == nullptr)
    {
        compiler->UnrecognizedTypeError(TransientString(thisResolved->rightType.name), this);
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
CommaExpression::EvalSymbol(FixedString& out) const
{
    return this->right->EvalSymbol(out);
}

//------------------------------------------------------------------------------
/**
*/
TransientString
CommaExpression::EvalString() const
{
    TransientString left, right;
    left = this->left->EvalString();
    right = this->right->EvalString();
    return TransientString(left, ",", right);
}

//------------------------------------------------------------------------------
/**
*/
bool 
CommaExpression::EvalAccessFlags(unsigned& out) const
{
    return this->right->EvalAccessFlags(out);
}

//------------------------------------------------------------------------------
/**
*/
bool
CommaExpression::EvalStorage(Storage& out) const
{
    return this->right->EvalStorage(out);
}

} // namespace GPULang
