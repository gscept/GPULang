//------------------------------------------------------------------------------
//  @file arrayindexexpression.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "arrayindexexpression.h"
#include "util.h"
#include "compiler.h"
#include "v3/ast/types/type.h"
namespace AnyFX 
{

//------------------------------------------------------------------------------
/**
*/
ArrayIndexExpression::ArrayIndexExpression(Expression* left, Expression* right)
    : left(left)
    , right(right)
{
    this->resolved = new ArrayIndexExpression::__Resolved;
    auto thisResolved = Symbol::Resolved(this);
    thisResolved->lhsType = nullptr;
    thisResolved->rhsType = nullptr;
    this->symbolType = ArrayIndexExpressionType;
}

//------------------------------------------------------------------------------
/**
*/
ArrayIndexExpression::~ArrayIndexExpression()
{
    delete this->left;
    if (this->right != nullptr)
        delete this->right;
}

//------------------------------------------------------------------------------
/**
*/
bool 
ArrayIndexExpression::Resolve(Compiler* compiler)
{
    auto thisResolved = Symbol::Resolved(this);
    if (this->isLhsValue)
        this->left->isLhsValue = true;
    if (this->isDeclaration)
        this->left->isDeclaration = true;

    // If rhs value, array indexing needs to provide an argument 
    if (this->right == nullptr && !this->isDeclaration)
    {
        compiler->Error(Format("Array access needs index but got '[]'"), this);
        return false;
    }

    if (!this->left->Resolve(compiler))
        return false;
    if (this->right != nullptr && !this->right->Resolve(compiler))
        return false;
    this->left->EvalType(thisResolved->leftFullType);
    if (this->right != nullptr)
        this->right->EvalType(thisResolved->rightFullType);

    thisResolved->returnType = thisResolved->leftFullType;
    if (!this->isDeclaration)
    {
        if (thisResolved->returnType.modifiers.empty())
        {
            Type* type = static_cast<Type*>(compiler->GetSymbol(thisResolved->returnType.name));
            auto it = type->lookup.find("operator[]");
            if (it == type->lookup.end())
            {
                compiler->Error(Format("'%s' does not implement the [] operator", thisResolved->leftFullType.ToString().c_str()), this);
                return false;
            }
            return true;
        }
        else
        {
            if (thisResolved->returnType.modifiers.back() != Type::FullType::Modifier::ArrayLevel)
            {
                compiler->Error(Format("operator [] not valid on non-array type '%s'", thisResolved->leftFullType.ToString().c_str()), this);
                return false;
            }
            thisResolved->returnType.modifiers.pop_back();
            thisResolved->returnType.modifierValues.pop_back();
        }
    }
    else
    {
        unsigned int value = 0;
        if (this->right != nullptr)
            this->right->EvalUInt(value);
        else if (!compiler->IsScopeGlobal())
        {
            compiler->Error(Format("Only globally declared arrays can be unsized"), this);
            return false;
        }
        thisResolved->returnType.modifiers.push_back(Type::FullType::Modifier::ArrayLevel);
        thisResolved->returnType.modifierValues.push_back(value);
    }

    // Return type is the left type with one less modifier
    thisResolved->lhsType = compiler->GetSymbol<Type>(thisResolved->leftFullType.name);
    if (thisResolved->lhsType == nullptr)
    {
        compiler->UnrecognizedTypeError(thisResolved->leftFullType.name, this);
        return false;
    }

    // If we have a right expression (array index), get the type and validate
    if (this->right != nullptr)
    {
        thisResolved->rhsType = compiler->GetSymbol<Type>(thisResolved->rightFullType.name);
        if (thisResolved->rhsType == nullptr)
        {
            compiler->UnrecognizedTypeError(thisResolved->rightFullType.name, this);
            return false;
        }

        if (thisResolved->rhsType->name != "uint" && thisResolved->rhsType->name != "int")
        {
            compiler->Error(Format("Expected array index to be uint or int but got '%s'", thisResolved->rightFullType.ToString().c_str()), this->right);
            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
ArrayIndexExpression::EvalType(Type::FullType& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    out = thisResolved->returnType;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
ArrayIndexExpression::EvalSymbol(std::string& out) const
{
    return this->left->EvalSymbol(out);
}

//------------------------------------------------------------------------------
/**
*/
bool  
ArrayIndexExpression::EvalInt(int& out) const
{
    return this->left->EvalInt(out);
}

//------------------------------------------------------------------------------
/**
*/
bool 
ArrayIndexExpression::EvalUInt(unsigned& out) const
{
    return this->left->EvalUInt(out);
}

//------------------------------------------------------------------------------
/**
*/
bool 
ArrayIndexExpression::EvalFloat(float& out) const
{
    return this->left->EvalFloat(out);
}

//------------------------------------------------------------------------------
/**
*/
bool 
ArrayIndexExpression::EvalBool(bool& out) const
{
    return this->left->EvalBool(out);
}

//------------------------------------------------------------------------------
/**
*/
std::string
ArrayIndexExpression::EvalString() const
{
    std::string left, right;
    left = this->left->EvalString();
    right = this->right->EvalString();

    if (this->right != nullptr)
        return Format("%s[%s]", left.c_str(), right.c_str());
    else
        return Format("%s[]", left.c_str());
}

} // namespace AnyFX
