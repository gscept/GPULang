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
    this->expr->~Expression();
}

//------------------------------------------------------------------------------
/**
*/
bool 
UnaryExpression::Resolve(Compiler* compiler)
{
    if (this->isLhsValue)
        this->expr->isLhsValue = true;
    
    if (this->expr == nullptr)
        return false;

    this->expr->Resolve(compiler);
    auto thisResolved = Symbol::Resolved(this);

    Type::FullType type;
    this->expr->EvalType(type);
    Type* typeSymbol;
    this->expr->EvalTypeSymbol(typeSymbol);

    static const StaticSet<TypeCode> allowedIncrementDecrementTypes =
    {
        TypeCode::Int,
        TypeCode::Int16,
        TypeCode::UInt,
        TypeCode::UInt16,
        TypeCode::Float,
        TypeCode::Float16
    };
    static const StaticSet<TypeCode> signedTypes =
    {
        TypeCode::Int,
        TypeCode::Int16,
        TypeCode::Float,
        TypeCode::Float16,
    };
    static const StaticSet<TypeCode> negatableTypes =
    {
        TypeCode::Int,
        TypeCode::Int16,
        TypeCode::UInt,
        TypeCode::UInt16,
        TypeCode::Bool,
    };
    static const StaticSet<TypeCode> conjuctableTypes =
    {
        TypeCode::Int,
        TypeCode::Int16,
        TypeCode::UInt,
        TypeCode::UInt16
    };

    
    switch (this->op)
    {
        case '++':
        {
            if (allowedIncrementDecrementTypes.Find(typeSymbol->baseType) == allowedIncrementDecrementTypes.end())
            {
                compiler->Error("Unary '++' only allowed on numeric (int/float) types", this);
                return false;    
            }
            break;
        }
        case '--':
        {
            if (allowedIncrementDecrementTypes.Find(typeSymbol->baseType) == allowedIncrementDecrementTypes.end())
            {
                compiler->Error("Unary '++' only allowed on numeric (int/float) types", this);
                return false;    
            }
            break;
        }
        case '*':
        {
            if (type.modifiers.size == 0 || type.modifiers.front() != Type::FullType::Modifier::Pointer)
            {
                compiler->Error("Dereferencing is only allowed on a pointer", this);
                return false;
            }
            
            type.modifiers = TransientArray(type.modifiers, 1);
            type.modifierValues = TransientArray(type.modifierValues, 1);
            break;
        }
        case '&':
        {
            // Add modifier to type
            type.modifiers = TransientArray<Type::FullType::Modifier>::Concatenate(type.modifiers, Type::FullType::Modifier::Pointer);
            type.modifierValues = TransientArray<Expression*>::Concatenate(type.modifierValues, (Expression*)nullptr);
            break;
        }
        case '-':
        {
            if (signedTypes.Find(typeSymbol->baseType) == signedTypes.end())
            {
                compiler->Error("Unary '-' only allowed on signed types", this);
                return false;    
            }
            if (type.modifiers.size != 0)
            {
                compiler->Error("Unary '-' only allowed on scalar values", this);
                return false;
            }
            break;
        }
        case '+':
        {
            // always allowed
            break;
        }
        case '!':
        {
            if (negatableTypes.Find(typeSymbol->baseType) == negatableTypes.end())
            {
                compiler->Error(Format("Unary '!' only allowed on integer and bool types"), this);
                return false;    
            }
            break;
        }    
        case '~':
        {
            if (conjuctableTypes.Find(typeSymbol->baseType) == conjuctableTypes.end())
            {
                compiler->Error(Format("Unary '~' only allowed on integer types"), this);
                return false;    
            }
            break;
        }
    }

    thisResolved->type = compiler->GetType(type);
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
    if (thisResolved->fullType.name == UNDEFINED_TYPE)
        return false;
    out = thisResolved->fullType;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
UnaryExpression::EvalTypeSymbol(Type*& out) const
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
UnaryExpression::EvalSymbol(FixedString& out) const
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
UnaryExpression::EvalValue(ValueUnion& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    if (thisResolved->type == nullptr)
        return false;
#define INVERT_OPERATOR(mem)\
    out.mem[0] = !out.mem[0];\
    if (out.columnSize > 1)\
        out.mem[1] = !out.mem[1];\
    if (out.columnSize > 2)\
        out.mem[2] = !out.mem[2];\
    if (out.columnSize > 3)\
        out.mem[3] = !out.mem[3];\

#define CONJUGATE_OPERATOR(mem)\
    out.mem[0] = ~out.mem[0];\
    if (out.columnSize > 1)\
        out.mem[1] = ~out.mem[1];\
    if (out.columnSize > 2)\
        out.mem[2] = ~out.mem[2];\
    if (out.columnSize > 3)\
        out.mem[3] = ~out.mem[3];\

#define NEG_OPERATOR(mem)\
    out.mem[0] *= -1;\
    if (out.columnSize > 1)\
        out.mem[1] *= -1;\
    if (out.columnSize > 2)\
        out.mem[2] *= -1;\
    if (out.columnSize > 3)\
        out.mem[3] *= -1;\
    if (out.rowSize > 1)\
    {\
        out.mem[4] *= -1;\
        if (out.columnSize > 1)\
            out.mem[5] *= -1;\
        if (out.columnSize > 2)\
            out.mem[6] *= -1;\
        if (out.columnSize > 3)\
            out.mem[7] *= -1;\
    }\
    if (out.rowSize > 2)\
    {\
        out.mem[8] *= -1;\
        if (out.columnSize > 1)\
            out.mem[9] *= -1;\
        if (out.columnSize > 2)\
            out.mem[10] *= -1;\
        if (out.columnSize > 3)\
            out.mem[11] *= -1;\
    }\
    if (out.rowSize > 3)\
    {\
        out.mem[12] *= -1;\
        if (out.columnSize > 1)\
            out.mem[13] *= -1;\
        if (out.columnSize > 2)\
            out.mem[14] *= -1;\
        if (out.columnSize > 3)\
            out.mem[15] *= -1;\
    }
    
    if (this->expr->EvalValue(out))
    {
        if (this->op == '-')
        {
            switch (out.code)
            {
                case TypeCode::Int:
                case TypeCode::Int16:
                    NEG_OPERATOR(i)
                    break;
                case TypeCode::UInt:
                case TypeCode::UInt16:
                    NEG_OPERATOR(ui)
                    break;
                case TypeCode::Float:
                case TypeCode::Float16:
                    NEG_OPERATOR(f)
                    break;
                default:
                    assert(false);
                break;
            }    
        }
        else if (this->op == '!')
        {
            switch (out.code)
            {
                case TypeCode::Bool:
                    INVERT_OPERATOR(b)
                    break;
                case TypeCode::Int:
                case TypeCode::Int16:
                    INVERT_OPERATOR(i)
                    break;
                case TypeCode::UInt:
                case TypeCode::UInt16:
                    INVERT_OPERATOR(ui)
                    break;
                default:
                    assert(false);
                break;
            }
        }
        else if (this->op == '~')
        {
            switch (out.code)
            {
                case TypeCode::Int:
                case TypeCode::Int16:
                    CONJUGATE_OPERATOR(i)
                    break;
                case TypeCode::UInt:
                case TypeCode::UInt16:
                    CONJUGATE_OPERATOR(ui)
                    break;
                default:
                    assert(false);
                    break;
            }
        }
        return true;
    }

    return false;
}

//------------------------------------------------------------------------------
/**
*/
TransientString
UnaryExpression::EvalString() const
{
    TransientString expString;
    expString = this->expr->EvalString();
    if (this->op != 0x0)
        if (this->isPrefix)
            return TransientString(FourCCToString(this->op), expString);
        else
            return TransientString(expString, FourCCToString(this->op));
    else
        return expString;
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

//------------------------------------------------------------------------------
/**
*/
bool
UnaryExpression::EvalStorage(Storage& out) const
{
    return this->expr->EvalStorage(out);
}

} // namespace GPULang
