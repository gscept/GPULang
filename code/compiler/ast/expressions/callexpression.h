#pragma once
//------------------------------------------------------------------------------
/**
    Function call expression

    (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "expression.h"
#include <vector>

namespace GPULang
{

struct CallExpression : public Expression
{
    /// constructor
    CallExpression(Expression* function, const std::vector<Expression*>& args);
    /// destructor
    virtual ~CallExpression();

    /// Resolve
    bool Resolve(Compiler* compiler) override;

    /// eval type
    bool EvalType(Type::FullType& out) const override;
    /// eval symbol
    bool EvalSymbol(std::string& out) const override;
    /// evaluates expression as a string
    std::string EvalString() const override;
    /// evaluates access flags
    bool EvalAccessFlags(unsigned& out) const override;
    /// evaluates storage
    bool EvalStorage(Storage& out) const override;

    Expression* function;
    std::vector<Expression*> args;

    struct __Resolved : public Expression::__Resolved
    {
        std::string functionSymbol;
        Type::FullType returnType;
        Type* retType;

        Function* function;

        std::vector<Type::FullType> argumentTypes;
        std::vector<Type*> argTypes;
        std::vector<Storage> argStorages;
        std::vector<Function*> conversions;
    };
    __Resolved* thisResolved;
};

} // namespace GPULang
