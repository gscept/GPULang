//------------------------------------------------------------------------------
//  declaredexpression.cc
//  (C) 2025 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "declaredexpression.h"
#include "generated/types.h"
#include "compiler.h"

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
DeclaredExpression::DeclaredExpression(const FixedString& symbol)
{
    this->resolved = Alloc<DeclaredExpression::__Resolved>();
    this->symbol = symbol;
    auto res = Symbol::Resolved(this);
    res->declared = false;
    this->symbolType = DeclaredExpressionType;
}

//------------------------------------------------------------------------------
/**
*/
DeclaredExpression::~DeclaredExpression()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool 
DeclaredExpression::Resolve(Compiler* compiler)
{
    if (!compiler->generationState.active)
    {
        compiler->Error("'declared' expressions are only allowed inside 'generate' blocks", this);
        return false;
    }
    auto res = Symbol::Resolved(this);
    Symbol* sym = compiler->GetSymbol(this->symbol);
    res->declared = sym != nullptr;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
DeclaredExpression::EvalType(Type::FullType& out) const
{
    out.name = ConstantString("b8");
    out.literal = true;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
DeclaredExpression::EvalTypeSymbol(Type*& out) const
{
    out = &Bool8Type;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
DeclaredExpression::EvalSymbol(FixedString& out) const
{
    out = this->symbol;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
DeclaredExpression::EvalValue(ValueUnion& out) const
{
    auto res = Symbol::Resolved(this);
    out.code = TypeCode::Bool8;
    out.b[0] = res->declared;
    out.columnSize = 1;
    out.rowSize = 1;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
DeclaredExpression::EvalAccessFlags(unsigned& out) const
{
    out = 0x0;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
DeclaredExpression::EvalStorage(Storage& out) const
{
    out = Storage::Default;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
TransientString
DeclaredExpression::EvalString() const
{
    return this->symbol;
}

} // namespace GPULang
