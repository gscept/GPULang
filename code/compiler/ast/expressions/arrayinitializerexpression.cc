//------------------------------------------------------------------------------
//  @file initializerexpression.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "arrayinitializerexpression.h"
#include "util.h"
#include "compiler.h"
#include "uintexpression.h"
namespace GPULang 
{

//------------------------------------------------------------------------------
/**
*/
ArrayInitializerExpression::ArrayInitializerExpression(const std::vector<Expression*>& values)
    : values(values)
{
    this->symbolType = ArrayInitializerExpressionType;
    auto resolved = Alloc<ArrayInitializerExpression::__Resolved>();
    this->resolved = resolved;
}

//------------------------------------------------------------------------------
/**
*/
ArrayInitializerExpression::~ArrayInitializerExpression()
{
}

//------------------------------------------------------------------------------
/**
*/
bool 
ArrayInitializerExpression::Resolve(Compiler* compiler)
{
    auto thisResolved = Symbol::Resolved(this);
    Type::FullType inner;

    for (Expression* expr : this->values)
    {
        if (!expr->Resolve(compiler))
            return false;
        
        Type::FullType valueType;
        expr->EvalType(valueType);
        if (inner.name == "unknown")
            inner = valueType;
        else if (valueType != inner)
        {
            compiler->Error(Format("Mismatching types, expected '%s' but got '%s'", inner.name.c_str(), valueType.name.c_str()), expr);
            return false;
        }
    }

    thisResolved->type = compiler->GetSymbol<Type>(inner.name);
    if (thisResolved->type == nullptr)
    {
        compiler->UnrecognizedTypeError(inner.name, this);
        return false;
    }
    
    thisResolved->text = this->EvalString();

    // Append array level first
    thisResolved->fullType.name = inner.name;
    thisResolved->fullType.modifiers.push_back(Type::FullType::Modifier::Array);
    thisResolved->fullType.modifierValues.push_back(Alloc<UIntExpression>(this->values.size()));
    thisResolved->fullType.modifiers.insert(thisResolved->fullType.modifiers.end(), inner.modifiers.begin(), inner.modifiers.end());
    thisResolved->fullType.modifierValues.insert(thisResolved->fullType.modifierValues.end(), inner.modifierValues.begin(), inner.modifierValues.end());

    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
ArrayInitializerExpression::EvalType(Type::FullType& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    out = thisResolved->fullType;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
std::string
ArrayInitializerExpression::EvalString() const
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

//------------------------------------------------------------------------------
/**
*/
bool 
ArrayInitializerExpression::EvalAccessFlags(unsigned& out) const
{
    out = 0x0;
    for (Expression* expr : this->values)
    {
        unsigned access;
        expr->EvalAccessFlags(access);
        if (access & AccessFlags::Const)
            out &= AccessFlags::Const;
        else if (access & AccessFlags::LinkTime)
            out |= AccessFlags::LinkTime;
    }
    return true;
}

} // namespace GPULang
