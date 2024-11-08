//------------------------------------------------------------------------------
//  @file callexpression.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "callexpression.h"
#include "util.h"
#include "compiler.h"
#include "ast/function.h"
#include "ast/enumeration.h"
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
    thisResolved->text = this->EvalString();
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

    struct Candidate
    {
        Function* function;
        bool needsConversion;
        std::vector<Function*> argumentConversionFunction;
    };
    if (symbol == nullptr)
    {
        // If the function isn't available, check for any type constructor that might implement it
        std::vector<Symbol*> functionSymbols = compiler->GetSymbols(thisResolved->functionSymbol.c_str());
        std::vector<Candidate> candidates;
        for (auto functionSymbol : functionSymbols)
        {
            if (functionSymbol->symbolType == Type::SymbolType::TypeType)
            {
                Type* type = static_cast<Type*>(functionSymbol);
                for (Symbol* ctor : type->constructors)
                {
                    Function* ctorFun = static_cast<Function*>(ctor);
                    Candidate candidate;
                    candidate.needsConversion = false;
                    candidate.function = ctorFun;

                    if (ctorFun->parameters.size() == thisResolved->argTypes.size())
                    {
                        uint32_t numMatches = 0;
                        for (size_t i = 0; i < ctorFun->parameters.size(); i++)
                        {
                            if (ctorFun->parameters[i]->type != thisResolved->argumentTypes[i])
                            {
                                std::string conversion = Format("%s(%s)", ctorFun->parameters[i]->type.name.c_str(), thisResolved->argTypes[i]->name.c_str());
                                Symbol* componentConversionSymbol = compiler->GetSymbol(conversion);

                                // No conversion available for this member, skip to next constructor
                                if (componentConversionSymbol == nullptr)
                                {
                                    break;
                                }
                                candidate.needsConversion = true;
                                candidate.argumentConversionFunction.push_back(static_cast<Function*>(componentConversionSymbol));
                            }
                            else
                            {
                                // No conversion needed
                                candidate.argumentConversionFunction.push_back(nullptr);
                            }
                            numMatches++;
                        }
                        if (numMatches == ctorFun->parameters.size())
                        {
                            candidates.push_back(candidate);
                        }
                    }
                }
            }
            else if (functionSymbol->symbolType == Type::SymbolType::FunctionType)
            {
                Function* fun = static_cast<Function*>(functionSymbol);
                Candidate candidate;
                candidate.function = fun;
                candidate.needsConversion = false;
                if (fun->parameters.size() == thisResolved->argTypes.size())
                {
                    uint32_t numMatches = 0;
                    for (size_t i = 0; i < fun->parameters.size(); i++)
                    {
                        if (fun->parameters[i]->type != thisResolved->argumentTypes[i])
                        {
                            std::string conversion = Format("%s(%s)", fun->parameters[i]->type.name.c_str(), thisResolved->argTypes[i]->name.c_str());
                            Symbol* componentConversionSymbol = compiler->GetSymbol(conversion);

                            // No conversion available for this member, skip to next constructor
                            if (componentConversionSymbol == nullptr)
                            {
                                break;
                            }
                            candidate.needsConversion = true;
                            candidate.argumentConversionFunction.push_back(static_cast<Function*>(componentConversionSymbol));
                        }
                        else
                        {
                            // No conversion needed
                            candidate.argumentConversionFunction.push_back(nullptr);
                        }
                        numMatches++;
                    }
                    if (numMatches == fun->parameters.size())
                    {
                        candidates.push_back(candidate);
                    }
                }
            }
        }

        if (candidates.empty())
        {
            std::string potentialHints;
            for (auto& hint : functionSymbols)
            {
                if (hint->symbolType == Type::SymbolType::FunctionType)
                {
                    Function* fun = static_cast<Function*>(hint);
                    Function::__Resolved* res = Symbol::Resolved(fun);
                    potentialHints.append(Format("\n    %s", res->name.c_str()));
                }
            }
            if (!potentialHints.empty())
                compiler->Error(Format("No overload exists for %s, maybe you meant:%s", callSignature.c_str(), potentialHints.c_str()), this);
            else
                compiler->Error(Format("No overload exists for %s", callSignature.c_str()), this);
            thisResolved->function = nullptr;
            return false;
        }
        else
        {
            std::vector<Candidate> ambiguousCalls;
            for (auto& candidate : candidates)
            {
                if (!candidate.needsConversion)
                {
                    thisResolved->function = candidate.function;
                    thisResolved->returnType = thisResolved->function->returnType;
                    thisResolved->retType = compiler->GetSymbol<Type>(thisResolved->returnType.name);
                    thisResolved->conversions.clear();
                    ambiguousCalls.clear();
                    break;
                }
                else
                {
                    ambiguousCalls.push_back(candidate);
                }
            }
            if (ambiguousCalls.size() > 1)
            {
                if (compiler->options.disallowImplicitConversion)
                {
                    compiler->Error(Format("Overload exists for %s but is disallowed. Either disable disallowImplicitConversion or explicitly convert arguments", callSignature.c_str()), this);
                    return false;
                }
                else
                {
                    std::string fmt = Format("Ambiguous call %s, could be:", callSignature.c_str());
                    for (auto& candidate : ambiguousCalls)
                    {
                        fmt.append(Format("\n    %s", Symbol::Resolved(candidate.function)->signature.c_str()));
                    }
                    compiler->Error(fmt, this);
                    thisResolved->function = nullptr;
                    return false;
                }
            }
            else if (ambiguousCalls.size() == 1)
            {
                thisResolved->function = ambiguousCalls[0].function;
                thisResolved->conversions = ambiguousCalls[0].argumentConversionFunction;
                thisResolved->returnType = thisResolved->function->returnType;
                thisResolved->retType = compiler->GetSymbol<Type>(thisResolved->returnType.name);
            }
        }
    }
    else
    {
        if (symbol->symbolType != Symbol::FunctionType)
        {
            compiler->Error(Format("Unrecognized function '%s' called", thisResolved->functionSymbol.c_str()), this);
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
        compiler->Error(Format("No overload of function '%s' was defined", callSignature.c_str()), this);
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

//------------------------------------------------------------------------------
/**
*/
bool 
CallExpression::EvalAccessFlags(unsigned& out) const
{
    out = 0x0;
    return true;
}

} // namespace GPULang
