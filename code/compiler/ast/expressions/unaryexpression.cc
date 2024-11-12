//------------------------------------------------------------------------------
//  unaryexpression.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "unaryexpression.h"
#include "util.h"
#include "compiler.h"
#include "../function.h"

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
UnaryExpression::UnaryExpression(uint32_t op, bool isPrefix, Expression* expr)
    : op(op)
    , isPrefix(isPrefix)
    , expr(expr)
{
    this->resolved = Alloc<UnaryExpression::__Resolved>();
    this->symbolType = UnaryExpressionType;
}

//------------------------------------------------------------------------------
/**
*/
UnaryExpression::~UnaryExpression()
{
}

//------------------------------------------------------------------------------
/**
*/
bool 
UnaryExpression::Resolve(Compiler* compiler)
{
    if (this->isLhsValue)
        this->expr->isLhsValue = true;

    this->expr->Resolve(compiler);
    auto thisResolved = Symbol::Resolved(this);
    thisResolved->text = this->EvalString();

    Type::FullType type;
    this->expr->EvalType(type);
    if (this->op == '*')
    {
        if (type.modifiers.empty() || type.modifiers.back() != Type::FullType::Modifier::Pointer)
        {
            compiler->Error(Format("Dereferencing is only allowed on a pointer"), this);
            return false;
        }
        type.modifiers.pop_back();
        type.modifierValues.pop_back();
    }
    else if (this->isLhsValue)
    {
        compiler->Error(Format("Left hand unary operators must be pointer indirection '*'"), this);
        return false;
    }

    thisResolved->type = compiler->GetSymbol<Type>(type.name);
    thisResolved->fullType = type;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
UnaryExpression::EvalType(Type::FullType& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    out = thisResolved->fullType;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
UnaryExpression::EvalSymbol(std::string& out) const
{
    if (this->op == '*')
    {
        auto thisResolved = Symbol::Resolved(this);
        if (thisResolved->dereffedSymbol != nullptr)
        {
            if (thisResolved->dereffedSymbol->symbolType == Symbol::SymbolType::FunctionType)
            {
                Function* fun = static_cast<Function*>(thisResolved->dereffedSymbol);
                Function::__Resolved* funResolved = Symbol::Resolved(fun);

                out = funResolved->name;
                return true;
            }
        }
    }
    return this->expr->EvalSymbol(out);
}

//------------------------------------------------------------------------------
/**
*/
bool
UnaryExpression::EvalInt(int& out) const
{
    if (this->op == '-')
    {
        return this->expr->EvalInt(out) * -1;
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
UnaryExpression::EvalUInt(unsigned& out) const
{
    if (this->op == '-')
    {
        return this->expr->EvalUInt(out) * -1;
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
UnaryExpression::EvalFloat(float& out) const
{
    if (this->op == '-')
    {
        return this->expr->EvalFloat(out) * -1;
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
UnaryExpression::EvalBool(bool& out) const
{
    if (this->op == '!')
    {
        return !this->expr->EvalBool(out);
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**
*/
std::string
UnaryExpression::EvalString() const
{
    std::string expString;
    expString = this->expr->EvalString();
    if (this->op != 0x0)
        if (this->isPrefix)
            return Format("%s%s", FourCCToString(this->op).c_str(), expString.c_str());
        else
            return Format("%s%s", expString.c_str(), FourCCToString(this->op).c_str());
    else
        return Format("%s", expString.c_str());
}

//------------------------------------------------------------------------------
/**
*/
bool 
UnaryExpression::EvalAccessFlags(unsigned& out) const
{
    this->expr->EvalAccessFlags(out);
    return true;
}

} // namespace GPULang