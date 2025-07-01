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
ArrayInitializerExpression::ArrayInitializerExpression(const FixedArray<Expression*>& values)
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
    for (auto init : this->values)
        init->~Expression();
}

//------------------------------------------------------------------------------
/**
*/
bool 
ArrayInitializerExpression::Resolve(Compiler* compiler)
{
    auto thisResolved = Symbol::Resolved(this);
    Type::FullType inner;

    if (this->values.size == 0)
    {
        compiler->Error("Array initializer can't be empty", this);
        return false;
    }

    bool isLiteral = true;
    for (Expression* expr : this->values)
    {
        if (!expr->Resolve(compiler))
            return false;
        
        Type::FullType valueType;
        expr->EvalType(valueType);
        isLiteral &= valueType.literal;

        if (inner.name == "<undefined>")
            inner = valueType;
        else if (valueType != inner)
        {
            compiler->Error(Format("Mismatching types, expected '%s' but got '%s'", inner.name.c_str(), valueType.name.c_str()), expr);
            return false;
        }
    }

    thisResolved->type = compiler->GetType(inner);
    if (thisResolved->type == nullptr)
    {
        compiler->UnrecognizedTypeError(TransientString(inner.name), this);
        return false;
    }
    
    // Append array level first
    thisResolved->fullType.name = inner.name;
    thisResolved->fullType.literal = isLiteral;
    thisResolved->fullType.modifiers.push_back(Type::FullType::Modifier::Array);
    thisResolved->fullType.modifierValues.push_back(Alloc<UIntExpression>((uint32_t)this->values.size));
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
    if (thisResolved->fullType.name == UNDEFINED_TYPE)
        return false;
    out = thisResolved->fullType;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
ArrayInitializerExpression::EvalTypeSymbol(Type*& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    if (thisResolved->type == nullptr)
        return false;
    out = thisResolved->type;
    assert(out->symbolType == Symbol::SymbolType::TypeType || out->symbolType == Symbol::SymbolType::EnumerationType || out->symbolType == Symbol::SymbolType::StructureType);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
TransientString
ArrayInitializerExpression::EvalString() const
{
    TransientString ret;
    for (Expression* expr : this->values)
    {
        TransientString str = expr->EvalString();
        ret.Append(str);
        if (expr != this->values.back())
            ret.Append(", ");
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

//------------------------------------------------------------------------------
/**
*/
bool
ArrayInitializerExpression::EvalStorage(Storage& out) const
{
    out = Storage::Default;
    return true;
}

} // namespace GPULang
