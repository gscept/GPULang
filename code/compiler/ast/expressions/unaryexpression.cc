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
    Type* typeSymbol = compiler->GetType(type);

    static const std::set<TypeCode> allowedIncrementDecrementTypes =
    {
        TypeCode::Int,
        TypeCode::Int16,
        TypeCode::UInt,
        TypeCode::UInt16,
    };
    static const std::set<TypeCode> signedTypes =
    {
        TypeCode::Int,
        TypeCode::Int16,
        TypeCode::Float,
        TypeCode::Float16,
    };
    static const std::set<TypeCode> negatableTypes =
    {
        TypeCode::Int,
        TypeCode::Int16,
        TypeCode::UInt,
        TypeCode::UInt16,
        TypeCode::Bool,
    };

    
    switch (this->op)
    {
        case '++':
        {
            if (allowedIncrementDecrementTypes.find(typeSymbol->baseType) == allowedIncrementDecrementTypes.end())
            {
                compiler->Error(Format("Unary '++' only allowed on integer types"), this);
                return false;    
            }
            break;
        }
        case '--':
        {
            if (allowedIncrementDecrementTypes.find(typeSymbol->baseType) == allowedIncrementDecrementTypes.end())
            {
                compiler->Error(Format("Unary '++' only allowed on integer types"), this);
                return false;    
            }
            break;
        }
        case '*':
        {
            if (this->op == '*')
            {
                if (type.modifiers.empty() || type.modifiers.front() != Type::FullType::Modifier::Pointer)
                {
                    compiler->Error(Format("Dereferencing is only allowed on a pointer"), this);
                    return false;
                }
                type.modifiers.erase(type.modifiers.begin());
                type.modifierValues.erase(type.modifierValues.begin());
            }
            else if (this->isLhsValue)
            {
                compiler->Error(Format("Left hand unary operators must be pointer indirection '*'"), this);
                return false;
            }
            break;
        }
        case '-':
        {
            if (signedTypes.find(typeSymbol->baseType) == signedTypes.end())
            {
                compiler->Error(Format("Unary '-' only allowed on signed types"), this);
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
            if (negatableTypes.find(typeSymbol->baseType) == negatableTypes.end())
            {
                compiler->Error(Format("Unary '!' only allowed on integer and bool types"), this);
                return false;    
            }
            break;
        }    
        case '~':
        {
            if (negatableTypes.find(typeSymbol->baseType) == negatableTypes.end())
            {
                compiler->Error(Format("Unary '~' only allowed on integer and bool types"), this);
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
UnaryExpression::EvalValue(ValueUnion& out) const
{
#define INVERT_OPERATOR(mem)\
    out.mem[0] = !out.mem[0];\
    if (out.columnSize > 1)\
        out.mem[1] = !out.mem[1];\
    if (out.columnSize > 2)\
        out.mem[2] = !out.mem[2];\
    if (out.columnSize > 3)\
        out.mem[3] = !out.mem[3];\

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
                case TypeCode::Bool:
                    INVERT_OPERATOR(b)
                    break;
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
        return true;
    }

    return false;
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

//------------------------------------------------------------------------------
/**
*/
bool
UnaryExpression::EvalStorage(Storage& out) const
{
    return this->expr->EvalStorage(out);
}

} // namespace GPULang