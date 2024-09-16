//------------------------------------------------------------------------------
//  @file callexpression.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "callexpression.h"
#include "util.h"
#include "compiler.h"
#include "ast/function.h"
namespace GPULang 
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

    this->function->EvalSymbol(thisResolved->functionSymbol);

    std::string argList = "";
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
        argList.append(fullType.ToString());
        if (expr != this->args.back())
            argList.append(",");
    }
    std::string callSignature = Format("%s(%s)", thisResolved->functionSymbol.c_str(), argList.c_str());

    Symbol* symbol = compiler->GetSymbol(callSignature);
    if (symbol == nullptr)
    {
        // If the function isn't available, check for any type constructor that might implement it
        Symbol* typeSymbol = compiler->GetSymbol(thisResolved->functionSymbol.c_str());
        if (typeSymbol != nullptr)
        {
            Type* type = static_cast<Type*>(typeSymbol);
            bool conversionFound = false;
            for (Symbol* ctor : type->constructors)
            {
                Function* ctorFun = static_cast<Function*>(ctor);
                thisResolved->function = ctorFun;
                if (ctorFun->parameters.size() == thisResolved->argTypes.size())
                {
                    uint32_t numMatches = 0;
                    for (size_t i = 0; i < ctorFun->parameters.size(); i++)
                    {
                        if (ctorFun->parameters[i]->type.name != thisResolved->argTypes[i]->name)
                        {
                            std::string conversion = Format("%s(%s)", ctorFun->parameters[i]->type.name.c_str(), thisResolved->argTypes[i]->name.c_str());
                            Symbol* componentConversionSymbol = compiler->GetSymbol(conversion);

                            // No conversion available for this member, skip to next constructor
                            if (componentConversionSymbol == nullptr)
                            {
                                thisResolved->conversions.clear();
                                break;
                            }
                            thisResolved->conversions.push_back(static_cast<Function*>(componentConversionSymbol));
                        }
                        else
                        {
                            // No conversion needed
                            thisResolved->conversions.push_back(nullptr);
                        }
                        numMatches++;
                    }
                    if (numMatches == ctorFun->parameters.size())
                    {
                        conversionFound = true;
                        break;
                    }
                }
            }
            if (!conversionFound)
            {
                compiler->Error(Format("No constructor exists for %s", callSignature.c_str()), this);
                thisResolved->function = nullptr;
                return false;
            }
        }
        else
        {
            compiler->UnrecognizedSymbolError(callSignature, this);
            return false;
        }
    }
    else
    {
        if (symbol->symbolType != Symbol::FunctionType)
        {
            compiler->Error(Format("'%s' is not a function", thisResolved->functionSymbol.c_str()), this);
        }
        thisResolved->function = static_cast<Function*>(symbol);
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
        compiler->Error(Format("No overload of function '%s' found", callSignature.c_str()), this);
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
            args.append(",");
    }
    std::string fun = this->function->EvalString();
    return Format("%s(%s)", fun.c_str(), args.c_str());
}

} // namespace GPULang
