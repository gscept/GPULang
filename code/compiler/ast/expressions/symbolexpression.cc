//------------------------------------------------------------------------------
//  symbolexpression.cc
//  (C) 2020 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "symbolexpression.h"
#include "ast/symbol.h"
#include "ast/variable.h"
#include "ast/function.h"
#include "compiler.h"
#include "util.h"
#include "ast/types/type.h"
#include "ast/structure.h"
#include "ast/expressions/enumexpression.h"
#include <string>

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
SymbolExpression::SymbolExpression(const std::string symbol)
    : symbol(symbol)
{
    this->resolved = Alloc<SymbolExpression::__Resolved>();
    this->symbolType = SymbolExpressionType;
}

//------------------------------------------------------------------------------
/**
*/
SymbolExpression::~SymbolExpression()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool 
SymbolExpression::Resolve(Compiler* compiler)
{
    auto thisResolved = Symbol::Resolved(this);
    thisResolved->symbol = compiler->GetSymbol(this->symbol);
    thisResolved->text = this->EvalString();
    if (thisResolved->symbol == nullptr)
    {
        compiler->UnrecognizedSymbolError(this->symbol, this);
        return false;
    }
    else
    {
        if (thisResolved->symbol->symbolType == Symbol::VariableType)
        {
            Variable* var = static_cast<Variable*>(thisResolved->symbol);
            auto varResolved = Symbol::Resolved(var);
            thisResolved->fullType = varResolved->type;
            thisResolved->type = compiler->GetSymbol<Type>(thisResolved->fullType.name);
            return true;
        }
        else if (thisResolved->symbol->symbolType == Symbol::StructureType)
        {
            Structure* struc = static_cast<Structure*>(thisResolved->symbol);
            thisResolved->fullType = Type::FullType{ struc->name };
            thisResolved->type = compiler->GetSymbol<Type>(thisResolved->fullType.name);
            return true;
        }
        else if (thisResolved->symbol->symbolType == Symbol::TypeType)
        {
            Type* type = static_cast<Type*>(thisResolved->symbol);
            thisResolved->fullType = Type::FullType{ type->name };
            thisResolved->type = type;
            return true;
        }
        else if (thisResolved->symbol->symbolType == Symbol::FunctionType)
        {
            // If lhs, it means it's a function assignment, therefore function pointer
            thisResolved->fullType = { "function" };
            return true;
        }
        else if (thisResolved->symbol->symbolType == Symbol::EnumerationType)
        {
            Type* type = static_cast<Type*>(thisResolved->symbol);
            thisResolved->fullType = type->name;
            return true;
        }
        else if (thisResolved->symbol->symbolType == Symbol::EnumExpressionType)
        {
            EnumExpression* expr = static_cast<EnumExpression*>(thisResolved->symbol);
            thisResolved->fullType = expr->type;
            return true;
        }
        else if (thisResolved->symbol->symbolType == Symbol::RenderStateType)
        {
            thisResolved->fullType = { "renderState" };
            return true;
        }
        else if (thisResolved->symbol->symbolType == Symbol::SamplerStateType)
        {
            thisResolved->fullType = { "sampler" };
            thisResolved->fullType.modifiers.push_back(Type::FullType::Modifier::Pointer);
            thisResolved->fullType.modifierValues.push_back(nullptr);
            return true;
        }
        else
        {
            compiler->Error(Format("Symbol is not function, type, variable, enum or structure"), this);
            return false;
        }
    }

    compiler->Error(Format("Could not resolve type for '%s'", this->symbol.c_str()), this);
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool 
SymbolExpression::EvalType(Type::FullType& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    out = thisResolved->fullType;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
SymbolExpression::EvalSymbol(std::string& out) const
{
    out = this->symbol;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
SymbolExpression::EvalInt(int& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    if (thisResolved->symbol)
    {
        switch (thisResolved->symbol->symbolType)
        {
            case VariableType:
            {
                Variable* var = static_cast<Variable*>(thisResolved->symbol);
                Variable::__Resolved* varResolved = static_cast<Variable::__Resolved*>(var->resolved);
                if (var->valueExpression != nullptr && varResolved->usageBits.flags.isConst)
                    return var->valueExpression->EvalInt(out);
                break;
            }
            case BoolExpressionType:
            case FloatExpressionType:
            case IntExpressionType:
            case UIntExpressionType:
            {
                Expression* expr = static_cast<Expression*>(thisResolved->symbol);
                return expr->EvalInt(out);
            }
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
SymbolExpression::EvalUInt(unsigned& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    if (thisResolved->symbol)
    {
        switch (thisResolved->symbol->symbolType)
        {
            case VariableType:
            {
                Variable* var = static_cast<Variable*>(thisResolved->symbol);
                Variable::__Resolved* varResolved = static_cast<Variable::__Resolved*>(var->resolved);
                if (var->valueExpression != nullptr && varResolved->usageBits.flags.isConst)
                    return var->valueExpression->EvalUInt(out);
                break;
            }
            case BoolExpressionType:
            case FloatExpressionType:
            case IntExpressionType:
            case UIntExpressionType:
            {
                Expression* expr = static_cast<Expression*>(thisResolved->symbol);
                return expr->EvalUInt(out);
            }
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
SymbolExpression::EvalFloat(float& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    if (thisResolved->symbol != nullptr)
    {
        switch (thisResolved->symbol->symbolType)
        {
            case VariableType:
            {
                Variable* var = static_cast<Variable*>(thisResolved->symbol);
                Variable::__Resolved* varResolved = static_cast<Variable::__Resolved*>(var->resolved);
                if (var->valueExpression != nullptr && varResolved->usageBits.flags.isConst)
                    return var->valueExpression->EvalFloat(out);
                break;
            }
            case BoolExpressionType:
            case FloatExpressionType:
            case IntExpressionType:
            case UIntExpressionType:
            {
                Expression* expr = static_cast<Expression*>(thisResolved->symbol);
                return expr->EvalFloat(out);
            }
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
SymbolExpression::EvalBool(bool& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    if (thisResolved->symbol)
    {
        switch (thisResolved->symbol->symbolType)
        {
            case VariableType:
            {
                Variable* var = static_cast<Variable*>(thisResolved->symbol);
                Variable::__Resolved* varResolved = static_cast<Variable::__Resolved*>(var->resolved);
                if (var->valueExpression != nullptr && varResolved->usageBits.flags.isConst)
                    return var->valueExpression->EvalBool(out);
                break;
            }
            case BoolExpressionType:
            case FloatExpressionType:
            case IntExpressionType:
            case UIntExpressionType:
            {
                Expression* expr = static_cast<Expression*>(thisResolved->symbol);
                return expr->EvalBool(out);
            }
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
std::string
SymbolExpression::EvalString() const
{
    return this->symbol;
}

//------------------------------------------------------------------------------
/**
*/
bool 
SymbolExpression::EvalAccessFlags(unsigned& out) const
{
    out = 0x0;
    auto thisResolved = Symbol::Resolved(this);
    if (thisResolved->symbol)
    {
        switch (thisResolved->symbol->symbolType)
        {
            case VariableType:
            {
                Variable* var = static_cast<Variable*>(thisResolved->symbol);
                Variable::__Resolved* varResolved = static_cast<Variable::__Resolved*>(var->resolved);
                if (varResolved->usageBits.flags.isConst)
                    out |= AccessFlags::Const;
                if (varResolved->storage == Variable::__Resolved::Storage::LinkDefined)
                    out |= AccessFlags::LinkTime;
                return true;
            }
            case StringExpressionType:
            case BoolExpressionType:
            case FloatExpressionType:
            case IntExpressionType:
            case UIntExpressionType:
            {
                Expression* expr = static_cast<Expression*>(thisResolved->symbol);
                return expr->EvalAccessFlags(out);
            }
        }
    }
    return false;
}

} // namespace GPULang