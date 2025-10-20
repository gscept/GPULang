//------------------------------------------------------------------------------
//  symbolexpression.cc
//  (C) 2020 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "symbolexpression.h"
#include "ast/symbol.h"
#include "ast/variable.h"
#include "ast/function.h"
#include "generated/types.h"
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
SymbolExpression::SymbolExpression(const FixedString& symbol)
    : symbol(symbol)
{
    this->thisResolved = Alloc<SymbolExpression::__Resolved>();
    this->thisResolved->type = nullptr;
    this->symbolType = SymbolExpressionType;
    this->resolved = this->thisResolved;
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
    if (thisResolved->symbol == nullptr)
    {
        compiler->UnrecognizedSymbolError(TransientString(this->symbol), this);
        return false;
    }
    else
    {
        if (thisResolved->symbol->symbolType == Symbol::VariableType)
        {
            Variable* var = static_cast<Variable*>(thisResolved->symbol);
            assert(var->symbolType == Symbol::SymbolType::VariableType);
            auto varResolved = Symbol::Resolved(var);
            thisResolved->fullType = var->type;
            thisResolved->type = varResolved->typeSymbol;
            return true;
        }
        else if (thisResolved->symbol->symbolType == Symbol::StructureType)
        {
            Structure* struc = static_cast<Structure*>(thisResolved->symbol);
            assert(struc->symbolType == Symbol::SymbolType::StructureType);
            thisResolved->fullType = Type::FullType{ struc->name };
            thisResolved->type = struc;
            return true;
        }
        else if (thisResolved->symbol->symbolType == Symbol::TypeType)
        {
            Type* type = static_cast<Type*>(thisResolved->symbol);
            assert(type->symbolType == Symbol::SymbolType::TypeType);
            thisResolved->fullType = Type::FullType{ type->name };
            thisResolved->type = type;
            return true;
        }
        else if (thisResolved->symbol->symbolType == Symbol::FunctionType)
        {
            // If lhs, it means it's a function assignment, therefore function pointer
            thisResolved->fullType = Type::FullType{ FUNCTION_TYPE };
            thisResolved->type = &GPULang::FunctionPtrType;
            return true;
        }
        else if (thisResolved->symbol->symbolType == Symbol::EnumerationType)
        {
            Type* type = static_cast<Type*>(thisResolved->symbol);
            assert(type->symbolType == Symbol::SymbolType::EnumerationType);
            thisResolved->fullType = Type::FullType{ type->name };
            thisResolved->type = type;
            return true;
        }
        else if (thisResolved->symbol->symbolType == Symbol::EnumExpressionType)
        {
            EnumExpression* expr = static_cast<EnumExpression*>(thisResolved->symbol);
            assert(expr->symbolType == Symbol::SymbolType::EnumExpressionType);
            thisResolved->fullType = expr->type;
            if (!expr->EvalTypeSymbol(thisResolved->type))
            {
                compiler->Error(Format("Symbol is not a valid enum value"), this);
                return false;
            }
            return true;
        }
        else if (thisResolved->symbol->symbolType == Symbol::RenderStateInstanceType)
        {
            thisResolved->fullType = Type::FullType{ "RenderState"_c };
            thisResolved->type = &RenderStateType;
            return true;
        }
        else if (thisResolved->symbol->symbolType == Symbol::SamplerStateInstanceType)
        {
            thisResolved->fullType = Type::FullType{ "sampler"_c };
            thisResolved->fullType.modifiers = { Type::FullType::Modifier::Pointer };
            thisResolved->fullType.modifierValues = { nullptr };
            thisResolved->type = &SamplerType;
            return true;
        }
        else if (thisResolved->symbol->symbolType == Symbol::BoolExpressionType)
        {
            thisResolved->fullType = Type::FullType{ "b8"_c };
            thisResolved->type = &Bool8Type;
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
    if (thisResolved->fullType.name == UNDEFINED_TYPE)
        return false;
    out = thisResolved->fullType;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
SymbolExpression::EvalTypeSymbol(Type*& out) const
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
bool
SymbolExpression::EvalSymbol(FixedString& out) const
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
TransientString
SymbolExpression::EvalString() const
{
    return TransientString(this->symbol);
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
            case SamplerStateInstanceType:
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
    return true;
}

} // namespace GPULang
