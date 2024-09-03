//------------------------------------------------------------------------------
//  @file callexpression.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "callexpression.h"
#include "util.h"
#include "compiler.h"
#include "ast/function.h"
namespace AnyFX 
{

//------------------------------------------------------------------------------
/**
*/
CallExpression::CallExpression(Expression* function, const std::vector<Expression*>& args)
    : function(function)
    , args(args)
{
    this->resolved = new CallExpression::__Resolved;
    auto thisResolved = Symbol::Resolved(this);
    thisResolved->function = nullptr;
    thisResolved->retType = nullptr;
    this->symbolType = CallExpressionType;
}

//------------------------------------------------------------------------------
/**
*/
CallExpression::~CallExpression()
{
    for (Expression* expr : this->args)
        delete expr;
    delete this->function;
}

//------------------------------------------------------------------------------
/**
*/
bool 
CallExpression::Resolve(Compiler* compiler)
{
    auto thisResolved = Symbol::Resolved(this);
    if (this->isLhsValue)
        this->function->isLhsValue = true;

    if (!this->function->Resolve(compiler))
        return false;

    this->function->EvalSymbol(thisResolved->functionSymbol);
    Symbol* symbol = compiler->GetSymbol(thisResolved->functionSymbol);
    if (symbol == nullptr)
    {
        compiler->UnrecognizedSymbolError(thisResolved->functionSymbol, this);
        return false;
    }

    for (Expression* expr : this->args)
    {
        if (!expr->Resolve(compiler))
            return false;

        Type::FullType fullType;
        expr->EvalType(fullType);

        Type* type = compiler->GetSymbol<Type>(fullType.name);
        if (type == nullptr)
        {
            compiler->UnrecognizedTypeError(fullType.name, this);
            return false;
        }
        thisResolved->argumentTypes.push_back(fullType);
        thisResolved->argTypes.push_back(type);
    }

    // function case
    std::vector<Symbol*> matches;
    if (symbol->symbolType == Symbol::FunctionType)
    {
        matches = compiler->GetSymbols(thisResolved->functionSymbol);

        Symbol* match = Function::MatchOverload(compiler, matches, thisResolved->argumentTypes);
        if (match)
            thisResolved->function = static_cast<Function*>(match);
        else
        {
            std::string argList;
            for (size_t i = 0; i < thisResolved->argumentTypes.size(); i++)
            {
                argList.append(thisResolved->argumentTypes[i].ToString());
                if (i < thisResolved->argumentTypes.size() - 1)
                    argList.append(", ");
            }
            compiler->Error(Format("No overload of '%s' matches arguments '%s'", thisResolved->functionSymbol.c_str(), argList.c_str()), this);
            return false;
        }
    }
    else if (symbol->symbolType == Symbol::TypeType)
    {
        // constructor case
        Type* type = static_cast<Type*>(symbol);
        matches = type->GetSymbols(thisResolved->functionSymbol);

        // if type constructor, allow for implicit conversion of individual arguments
        Symbol* match = Function::MatchOverload(compiler, matches, thisResolved->argumentTypes, true);
        if (match)
            thisResolved->function = static_cast<Function*>(match);
        else
        {
            std::string argList;
            for (size_t i = 0; i < thisResolved->argumentTypes.size(); i++)
            {
                argList.append(thisResolved->argumentTypes[i].ToString());
                if (i < thisResolved->argumentTypes.size() - 1)
                    argList.append(", ");
            }
            compiler->Error(Format("No constructor for type '%s' matches arguments '%s'", type->name.c_str(), argList.c_str()), this);
            return false;
        }
    }

    if (thisResolved->function != nullptr)
    {
        thisResolved->returnType = thisResolved->function->returnType;
        thisResolved->retType = compiler->GetSymbol<Type>(thisResolved->returnType.name);
        if (thisResolved->retType == nullptr)
        {
            compiler->UnrecognizedTypeError(thisResolved->returnType.name, this);
            return false;
        }
        return true;
    }
    else
    {
        compiler->Error(Format("No overload of function '%s' found", thisResolved->functionSymbol.c_str()), this);
        return false;
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
CallExpression::EvalType(Type::FullType& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    out = thisResolved->returnType;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
CallExpression::EvalSymbol(std::string& out) const
{
    return this->function->EvalSymbol(out);
}

//------------------------------------------------------------------------------
/**
*/
std::string
CallExpression::EvalString() const
{
    std::string args;
    for (Expression* expr : this->args)
    {
        std::string arg = expr->EvalString();
        args.append(arg);
        if (expr != this->args.back())
            args.append(", ");
    }
    std::string fun = this->function->EvalString();
    return Format("%s(%s)", fun.c_str(), args.c_str());
}

} // namespace AnyFX
