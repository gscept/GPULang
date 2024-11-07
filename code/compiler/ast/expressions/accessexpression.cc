//------------------------------------------------------------------------------
//  @file accessexpression.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "accessexpression.h"
#include "ast/expressions/enumexpression.h"
#include "util.h"
#include "compiler.h"
#include "ast/variable.h"
namespace GPULang 
{

//------------------------------------------------------------------------------
/**
*/
AccessExpression::AccessExpression(Expression* left, Expression* right, bool deref)
    : left(left)
    , right(right)
    , deref(deref)
{
    this->resolved = new AccessExpression::__Resolved;
    this->symbolType = AccessExpressionType;
}

//------------------------------------------------------------------------------
/**
*/
AccessExpression::~AccessExpression()
{
    delete this->resolved;
    delete this->left;
    delete this->right;
}

//------------------------------------------------------------------------------
/**
*/
bool 
AccessExpression::Resolve(Compiler* compiler)
{
    auto thisResolved = Symbol::Resolved(this);
    thisResolved->text = this->EvalString();
    if (this->isLhsValue)
        this->left->isLhsValue = true;

    if (!this->left->Resolve(compiler))
        return false;

    this->left->EvalType(thisResolved->leftType);

    if (this->deref)
    {
        if (thisResolved->leftType.modifiers.empty()
            || thisResolved->leftType.modifiers.back() != Type::FullType::Modifier::Pointer)
        {
            compiler->Error(Format("Cannot dereference type '%s', did you mean to use '.' instead?", thisResolved->leftType.ToString().c_str()), this);
            return false;
        }
        thisResolved->leftType.modifierValues.pop_back();
        thisResolved->leftType.modifiers.pop_back();
    }

    thisResolved->lhsType = compiler->GetSymbol<Type>(thisResolved->leftType.name);
    if (thisResolved->lhsType == nullptr)
    {
        compiler->UnrecognizedTypeError(thisResolved->leftType.name, this);
        return false;
    }

    // Setup type scope and resolve right hand side
    Compiler::LocalScope scope = Compiler::LocalScope::MakeTypeScope(compiler, thisResolved->lhsType);

    if (!this->right->Resolve(compiler))
        return false;

    this->right->EvalType(thisResolved->rightType);
    thisResolved->rhsType = compiler->GetSymbol<Type>(thisResolved->rightType.name);
    if (thisResolved->rhsType == nullptr)
    {
        compiler->UnrecognizedTypeError(thisResolved->rightType.name, this);
        return false;
    }

    this->right->EvalSymbol(thisResolved->rightSymbol);
    thisResolved->swizzleMask.mask = 0x0;

    if (thisResolved->lhsType->IsVector())
    {
        // eval swizzled type, since a vector can expand for example vec2 -> vec4 by using .xxyy;
        Type::SwizzleMask swizzle;
        if (!Type::StringToSwizzleMask(thisResolved->rightSymbol, swizzle))
        {
            compiler->Error(Format("Invalid swizzle mask '%s'", thisResolved->rightSymbol.c_str()), this);
            return false;
        }
        unsigned biggestComponent = Type::SwizzleMaskBiggestComponent(swizzle);
        if (biggestComponent > thisResolved->lhsType->columnSize)
        {
            compiler->Error(Format("Swizzle '%s' indexes beyond vector size '%s'", thisResolved->rightSymbol.c_str(), thisResolved->lhsType->name.c_str()), this);
            return false;
        }
        thisResolved->swizzleMask = swizzle;
        unsigned numComponents = Type::SwizzleMaskComponents(swizzle);
        std::string vectorType = Type::ToVector(thisResolved->lhsType->baseType, numComponents);
        thisResolved->returnType = Type::FullType{ vectorType };
    }
    else if (!thisResolved->leftType.modifiers.empty() && thisResolved->leftType.modifiers.front() == Type::FullType::Modifier::Array)
    {
        if (thisResolved->rightSymbol != "length")
        {
            compiler->Error(Format("Invalid member for array '%s'", thisResolved->rightSymbol.c_str()), this);
            return false;
        }
    }
    else if (thisResolved->lhsType->symbolType == Type::SymbolType::EnumerationType)
    {
        EnumExpression* expr = static_cast<EnumExpression*>(thisResolved->lhsType->GetSymbol(thisResolved->rightSymbol));
        thisResolved->returnType = expr->type;
    }
    else
    {
        Variable* memberVar = static_cast<Variable*>(thisResolved->lhsType->GetSymbol(thisResolved->rightSymbol));
        if (memberVar == nullptr)
        {
            compiler->Error(Format("Type does not have member or method '%s'", thisResolved->rightSymbol.c_str()), this);
            return false;
        }
        else
        {
            Variable::__Resolved* varResolved = Symbol::Resolved(memberVar);
            thisResolved->returnType = varResolved->type;
            thisResolved->returnType.mut = thisResolved->leftType.mut;
        }
    }

    return thisResolved->lhsType != nullptr && thisResolved->rhsType != nullptr;
}

//------------------------------------------------------------------------------
/**
*/
bool
AccessExpression::EvalType(Type::FullType& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    out = thisResolved->returnType;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
AccessExpression::EvalSymbol(std::string& out) const
{
    return this->left->EvalSymbol(out);
}

//------------------------------------------------------------------------------
/**
*/
bool
AccessExpression::EvalUInt(unsigned& out) const
{
    AccessExpression::__Resolved* thisRes = Symbol::Resolved(this);
    if (thisRes->lhsType->symbolType == Type::SymbolType::EnumerationType)
    {
        EnumExpression* expr = static_cast<EnumExpression*>(thisRes->lhsType->GetSymbol(thisRes->rightSymbol));
        return expr->EvalUInt(out);
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
AccessExpression::EvalInt(int& out) const
{
    AccessExpression::__Resolved* thisRes = Symbol::Resolved(this);
    if (thisRes->lhsType->symbolType == Type::SymbolType::EnumerationType)
    {
        EnumExpression* expr = static_cast<EnumExpression*>(thisRes->lhsType->GetSymbol(thisRes->rightSymbol));
        return expr->EvalInt(out);
    }
    return Expression::EvalInt(out);
}

//------------------------------------------------------------------------------
/**
*/
bool
AccessExpression::EvalFloat(float& out) const
{
    AccessExpression::__Resolved* thisRes = Symbol::Resolved(this);
    if (thisRes->lhsType->symbolType == Type::SymbolType::EnumerationType)
    {
        EnumExpression* expr = static_cast<EnumExpression*>(thisRes->lhsType->GetSymbol(thisRes->rightSymbol));
        return expr->EvalFloat(out);
    }
    return Expression::EvalFloat(out);
}

//------------------------------------------------------------------------------
/**
*/
bool
AccessExpression::EvalBool(bool& out) const
{
    AccessExpression::__Resolved* thisRes = Symbol::Resolved(this);
    if (thisRes->lhsType->symbolType == Type::SymbolType::EnumerationType)
    {
        EnumExpression* expr = static_cast<EnumExpression*>(thisRes->lhsType->GetSymbol(thisRes->rightSymbol));
        return expr->EvalBool(out);
    }
    return Expression::EvalBool(out);
}

//------------------------------------------------------------------------------
/**
*/
std::string
AccessExpression::EvalString() const
{
    std::string left, right;
    left = this->left->EvalString();
    right = this->right->EvalString();
    return Format("%s.%s", left.c_str(), right.c_str());
}

//------------------------------------------------------------------------------
/**
*/
bool 
AccessExpression::EvalAccessFlags(unsigned& out) const
{
    unsigned left, right;
    this->left->EvalAccessFlags(left);
    this->right->EvalAccessFlags(right);
    out = left | right;
    return true;
}

} // namespace GPULang