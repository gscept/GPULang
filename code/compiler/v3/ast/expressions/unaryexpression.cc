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
UnaryExpression::UnaryExpression(uint32_t op, uint32_t postOp, Expression* expr)
    : op(op)
    , postOp(postOp)
    , expr(expr)
{
    this->resolved = new UnaryExpression::__Resolved;
    this->symbolType = UnaryExpressionType;
}

//------------------------------------------------------------------------------
/**
*/
UnaryExpression::~UnaryExpression()
{
    delete this->expr;
}

//------------------------------------------------------------------------------
/**
*/
bool 
UnaryExpression::Resolve(Compiler* compiler)
{
    if (this->isLhsValue)
        this->expr->isLhsValue = true;
    if (this->isDeclaration)
        this->expr->isDeclaration = true;

    this->expr->Resolve(compiler);
    auto thisResolved = Symbol::Resolved(this);

    Type::FullType type;
    this->expr->EvalType(type);
    if (this->op == '*')
    {
        if (!this->isLhsValue)
        {
            if (type.modifiers.empty())
            {
                Symbol* sym = compiler->GetSymbol<Symbol>(this->expr->EvalString());
                switch (sym->symbolType)
                {
                    case Symbol::SymbolType::FunctionType:
                    {
                        Function* fun = static_cast<Function*>(sym);
                        thisResolved->dereffedSymbol = sym;
                        thisResolved->fullType = { "function" };
                        return true;
                    }
                }
            }
            else
            {
                type.modifiers.pop_back();
                type.modifierValues.pop_back();
            }
        }
        else
        {
            type.modifiers.push_back(Type::FullType::Modifier::PointerLevel);
            type.modifierValues.push_back(0);
        }
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
        return Format("%s%s", FourCCToString(this->op).c_str(), expString.c_str());
    else if (this->postOp != 0x0)
        return Format("%s%s", expString.c_str(), FourCCToString(this->postOp).c_str());
    else
        return Format("%s", expString.c_str());
}
} // namespace GPULang