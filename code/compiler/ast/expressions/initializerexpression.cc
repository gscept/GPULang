//------------------------------------------------------------------------------
//  @file initializerexpression.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "initializerexpression.h"

#include "binaryexpression.h"
#include "util.h"
#include "compiler.h"
#include "uintexpression.h"
namespace GPULang 
{

//------------------------------------------------------------------------------
/**
*/
InitializerExpression::InitializerExpression(const FixedArray<Expression*>& values, const FixedString& type)
    : values(values)
    , explicitType(type)
{
    this->symbolType = InitializerExpressionType;
    auto resolved = Alloc<InitializerExpression::__Resolved>();
    this->resolved = resolved;
}

//------------------------------------------------------------------------------
/**
*/
InitializerExpression::~InitializerExpression()
{
    for (auto init : this->values)
        init->~Expression();
}

//------------------------------------------------------------------------------
/**
*/
bool 
InitializerExpression::Resolve(Compiler* compiler)
{
    auto thisResolved = Symbol::Resolved(this);
    Type::FullType inner;
    Type* ty = compiler->GetSymbol<Type>(this->explicitType);
    if (ty == nullptr)
    {
        compiler->UnrecognizedTypeError(TransientString(this->explicitType), this);
        return false;
    }
    thisResolved->type = ty;
    inner.name = ty->name;

    if (ty->symbols.size != this->values.size)
    {
        compiler->Error("Struct must be fully initialized", this);
        return false;
    }

    compiler->PushScope(&ty->scope);
    for (uint32_t i = 0; i < this->values.size; i++)
    {
        Expression* expr = this->values[i];
        if (expr->symbolType != Symbol::SymbolType::BinaryExpressionType)
        {
            compiler->Error("Initializer may only contain assignment expressions", expr);
            return false;
        }
        BinaryExpression* binExpr = static_cast<BinaryExpression*>(expr);
        if (!expr->Resolve(compiler))
            return false;
        
        FixedString name;
        binExpr->left->EvalSymbol(name);
        if (name != ty->symbols[i]->name)
        {
            compiler->Error(Format("Initializer may only appear in order, expecting '%s' but got '%s'", ty->symbols[i]->name.c_str(), name.c_str()), expr);
            return false;
        }
    }
    compiler->PopScope();
    
    // Append array level first
    thisResolved->fullType.name = inner.name;
    thisResolved->fullType.modifiers = TransientArray<Type::FullType::Modifier>::Concatenate(thisResolved->fullType.modifiers, inner.modifiers);;
    thisResolved->fullType.modifierValues = TransientArray<Expression*>::Concatenate(thisResolved->fullType.modifierValues, inner.modifierValues);

    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
InitializerExpression::EvalType(Type::FullType& out) const
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
InitializerExpression::EvalTypeSymbol(Type*& out) const
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
InitializerExpression::EvalString() const
{
    TransientString ret;
    for (Expression* expr : this->values)
    {
        TransientString str;
        str = expr->EvalString();
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
InitializerExpression::EvalAccessFlags(unsigned& out) const
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
InitializerExpression::EvalStorage(Storage& out) const
{
    out = Storage::Default;
    return true;
}

} // namespace GPULang
