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
    this->resolved = Alloc<AccessExpression::__Resolved>();
    this->symbolType = AccessExpressionType;
}

//------------------------------------------------------------------------------
/**
*/
AccessExpression::~AccessExpression()
{
    if (this->left != nullptr)
        this->left->~Expression();
    if (this->right != nullptr)
        this->right->~Expression();
}

//------------------------------------------------------------------------------
/**
*/
bool 
AccessExpression::Resolve(Compiler* compiler)
{
    auto thisResolved = Symbol::Resolved(this);
    if (this->isLhsValue)
        this->left->isLhsValue = true;

    if (this->left == nullptr || this->right == nullptr)
        return false;
    
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

    this->left->EvalTypeSymbol(thisResolved->lhsType);
    if (thisResolved->lhsType == nullptr)
    {
        compiler->UnrecognizedTypeError(TransientString(thisResolved->leftType.name), this);
        return false;
    }

    // Setup type scope and resolve right hand side
    Compiler::LocalScope scope = Compiler::LocalScope::MakeLocalScope(compiler, &thisResolved->lhsType->scope);

    if (!this->right->Resolve(compiler))
        return false;

    this->right->EvalType(thisResolved->rightType);
    this->right->EvalTypeSymbol(thisResolved->rhsType);
    if (thisResolved->rhsType == nullptr)
    {
        compiler->UnrecognizedTypeError(TransientString(thisResolved->rightType.name), this);
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
        thisResolved->returnType = thisResolved->leftType;
        thisResolved->retType = thisResolved->lhsType;
        thisResolved->returnType.swizzleName = vectorType;
        thisResolved->returnType.swizzleMask = swizzle;
        thisResolved->swizzleType = compiler->GetType(thisResolved->returnType);
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
        EnumExpression::__Resolved* exprRes = Symbol::Resolved(expr);
        thisResolved->returnType = expr->type;
        thisResolved->retType = exprRes->type;
    }
    else
    {
        thisResolved->returnType = thisResolved->rightType;
        thisResolved->retType = thisResolved->rhsType;
        thisResolved->returnType.mut = thisResolved->leftType.mut;
        /*
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
        */
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
    if (thisResolved->returnType.name == UNDEFINED_TYPE)
        return false;
    out = thisResolved->returnType;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
AccessExpression::EvalTypeSymbol(Type*& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    if (thisResolved->swizzleType != nullptr)
        out = thisResolved->swizzleType;
    else if (thisResolved->retType != nullptr)
        out = thisResolved->retType;
    else
        return false;
    assert(out->symbolType == Symbol::SymbolType::TypeType || out->symbolType == Symbol::SymbolType::EnumerationType || out->symbolType == Symbol::SymbolType::StructureType);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
AccessExpression::EvalSymbol(FixedString& out) const
{
    return this->left->EvalSymbol(out);
}

//------------------------------------------------------------------------------
/**
*/
bool
AccessExpression::EvalValue(ValueUnion& out) const
{
    AccessExpression::__Resolved* thisRes = Symbol::Resolved(this);
    if (thisRes->lhsType == nullptr || thisRes->rhsType == nullptr)
        return false;
    if (thisRes->lhsType->symbolType == Type::SymbolType::EnumerationType)
    {
        EnumExpression* expr = static_cast<EnumExpression*>(thisRes->lhsType->GetSymbol(thisRes->rightSymbol));
        return expr->EvalValue(out);
    }
    out.SetType(thisRes->retType);
    return false;
}

//------------------------------------------------------------------------------
/**
*/
TransientString
AccessExpression::EvalString() const
{
    TransientString left, right;
    left = this->left->EvalString();
    right = this->right->EvalString();
    return TransientString(left, ".", right);
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

//------------------------------------------------------------------------------
/**
*/
bool
AccessExpression::EvalStorage(Storage& out) const
{
    return this->right->EvalStorage(out);
}
} // namespace GPULang
