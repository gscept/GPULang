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
    this->thisResolved = Symbol::Resolved(this);
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
            thisResolved->type = compiler->GetType(thisResolved->fullType);
            return true;
        }
        else if (thisResolved->symbol->symbolType == Symbol::StructureType)
        {
            Structure* struc = static_cast<Structure*>(thisResolved->symbol);
            thisResolved->fullType = Type::FullType{ struc->name };
            thisResolved->type = compiler->GetType(thisResolved->fullType);
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
            thisResolved->fullType = Type::FullType{ FUNCTION_TYPE };
            return true;
        }
        else if (thisResolved->symbol->symbolType == Symbol::EnumerationType)
        {
            Type* type = static_cast<Type*>(thisResolved->symbol);
            thisResolved->fullType = Type::FullType{ type->name };
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
            thisResolved->fullType = Type::FullType{ "renderState" };
            return true;
        }
        else if (thisResolved->symbol->symbolType == Symbol::SamplerStateType)
        {
            thisResolved->fullType = Type::FullType{ "sampler" };
            thisResolved->fullType.modifiers.push_back(Type::FullType::Modifier::Pointer);
            thisResolved->fullType.modifierValues.push_back(nullptr);
            thisResolved->type = compiler->GetType(thisResolved->fullType);
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
SymbolExpression::EvalValue(ValueUnion& out) const
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
                    return var->valueExpression->EvalValue(out);
                break;
            }
            case BoolExpressionType:
            case FloatExpressionType:
            case IntExpressionType:
            case UIntExpressionType:
            {
                Expression* expr = static_cast<Expression*>(thisResolved->symbol);
                return expr->EvalValue(out);
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
    if (this->thisResolved->symbol)
    {
        switch (this->thisResolved->symbol->symbolType)
        {
            case VariableType:
            {
                Variable* var = static_cast<Variable*>(this->thisResolved->symbol);
                Variable::__Resolved* varResolved = static_cast<Variable::__Resolved*>(var->resolved);
                if (varResolved->usageBits.flags.isConst)
                    out |= AccessFlags::Const;
                if (varResolved->storage == Storage::LinkDefined)
                    out |= AccessFlags::LinkTime;
                return true;
            }
            case StringExpressionType:
            case BoolExpressionType:
            case FloatExpressionType:
            case IntExpressionType:
            case UIntExpressionType:
            {
                Expression* expr = static_cast<Expression*>(this->thisResolved->symbol);
                return expr->EvalAccessFlags(out);
            }
            default:
                break;
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
SymbolExpression::EvalStorage(Storage& out) const
{
    out = Storage::Default;
    if (this->thisResolved->symbol)
    {
        switch (this->thisResolved->symbol->symbolType)
        {
            case VariableType:
            {
                Variable* var = static_cast<Variable*>(this->thisResolved->symbol);
                Variable::__Resolved* varResolved = static_cast<Variable::__Resolved*>(var->resolved);
                out = varResolved->storage;
                return true;
            }
            case SamplerStateType:
            {
                out = Storage::Uniform;
                return true;
            }
            case StringExpressionType:
            case BoolExpressionType:
            case FloatExpressionType:
            case IntExpressionType:
            case UIntExpressionType:
            {
                out = Storage::Default;
                return true;
            }
            default:
                break;
        }
    }
    return false;
}

} // namespace GPULang