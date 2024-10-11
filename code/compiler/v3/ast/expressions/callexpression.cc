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
    thisResolved->text = this->EvalString();
    if (this->isLhsValue)
        this->function->isLhsValue = true;

    this->function->EvalSymbol(thisResolved->functionSymbol);

    Function::__Resolved* owningResolvedFunction = Symbol::Resolved(compiler->currentFunction);
    if (thisResolved->functionSymbol.starts_with("gplExportVertexCoordinates"))
    {
        owningResolvedFunction->sideEffects.flags.exportsVertexPosition = true;
    }
    else if (thisResolved->functionSymbol.starts_with("gplExportColor"))
    {
        owningResolvedFunction->sideEffects.flags.exportsPixel = true;
    }
    else if (thisResolved->functionSymbol.starts_with("gplSetPixelDepth"))
    {
        owningResolvedFunction->sideEffects.flags.exportsExplicitDepth = true;
    }
    else if (thisResolved->functionSymbol.starts_with("gplSetOutputLayer"))
    {
        owningResolvedFunction->sideEffects.flags.setsOutputLayer = true;
    }
    else if (thisResolved->functionSymbol.starts_with("gplSetOutputViewport"))
    {
        owningResolvedFunction->sideEffects.flags.setsViewport = true;
    }

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
        std::vector<Symbol*> functionSymbols = compiler->GetSymbols(thisResolved->functionSymbol.c_str());
        bool conversionFound = false;
        for (auto functionSymbol : functionSymbols)
        {
            if (conversionFound)
                break;
            if (functionSymbol->symbolType == Type::SymbolType::TypeType)
            {
                Type* type = static_cast<Type*>(functionSymbol);
                for (Symbol* ctor : type->constructors)
                {
                    Function* ctorFun = static_cast<Function*>(ctor);
                    thisResolved->function = ctorFun;
                    thisResolved->returnType = thisResolved->function->returnType;
                    thisResolved->retType = compiler->GetSymbol<Type>(thisResolved->returnType.name);
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
            }
            else if (functionSymbol->symbolType == Type::SymbolType::FunctionType)
            {
                Function* fun = static_cast<Function*>(functionSymbol);
                thisResolved->function = fun;
                thisResolved->returnType = thisResolved->function->returnType;
                thisResolved->retType = compiler->GetSymbol<Type>(thisResolved->returnType.name);
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
                    if (numMatches == fun->parameters.size())
                    {
                        conversionFound = true;
                    }
                }
            }
        }
        if (!conversionFound)
        {
            compiler->Error(Format("No overload exists for %s", callSignature.c_str()), this);
            thisResolved->function = nullptr;
            return false;
        }
        else
        {
            if (compiler->options.disallowImplicitConversion)
            {
                compiler->Error(Format("Overload exists for %s but is disallowed. Either disable disallowImplicitConversion or explicitly convert arguments", callSignature.c_str()), this);
                return false;
            }
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

} // namespace GPULang
