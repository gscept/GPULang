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
    this->resolved = Alloc<CallExpression::__Resolved>();
    this->thisResolved = Symbol::Resolved(this);
    this->thisResolved->function = nullptr;
    this->thisResolved->retType = nullptr;
    this->symbolType = CallExpressionType;
}

//------------------------------------------------------------------------------
/**
*/
CallExpression::~CallExpression()
{
}

//------------------------------------------------------------------------------
/**
*/
bool 
CallExpression::Resolve(Compiler* compiler)
{
    this->thisResolved->text = this->EvalString();
    if (this->isLhsValue)
        this->function->isLhsValue = true;

    this->function->EvalSymbol(this->thisResolved->functionSymbol);

    std::string argList = "";
    for (Expression* expr : this->args)
    {
        if (!expr->Resolve(compiler))
            return false;

        Type::FullType fullType;
        expr->EvalType(fullType);

        Storage storage;
        expr->EvalStorage(storage);

        Type* type = compiler->GetType(fullType);
        if (type == nullptr)
        {
            compiler->UnrecognizedTypeError(fullType.ToString(), this);
            return false;
        }
        this->thisResolved->argStorages.push_back(storage);
        this->thisResolved->argumentTypes.push_back(fullType);
        this->thisResolved->argTypes.push_back(type);
        if (storage != Storage::Default && storage != Storage::Global)
            argList.append(Format("%s %s", StorageToString(storage).c_str(), fullType.ToString().c_str()));
        else
            argList.append(fullType.ToString());
        
        if (expr != this->args.back())
            argList.append(",");
    }
    std::string callSignature = Format("%s(%s)", this->thisResolved->functionSymbol.c_str(), argList.c_str());
    Symbol* symbol = compiler->GetSymbol(callSignature);

    struct Candidate
    {
        Function* function;
        bool needsConversion;
        bool simpleConversion;
        std::vector<Function*> argumentConversionFunctions;
    };
    if (symbol == nullptr)
    {
        // If the function isn't available, check for any type constructor that might implement it
        std::vector<Symbol*> functionSymbols = compiler->GetSymbols(this->thisResolved->functionSymbol.c_str());
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
                    candidate.simpleConversion = false;
                    candidate.function = ctorFun;

                    if (ctorFun->parameters.size() == this->thisResolved->argTypes.size())
                    {
                        uint32_t numMatches = 0;
                        for (size_t i = 0; i < ctorFun->parameters.size(); i++)
                        {
                            Variable* param = ctorFun->parameters[i];
                            Variable::__Resolved* paramResolved = Symbol::Resolved(param);

                            // There is no help if storage doesn't align
                            if (!IsStorageCompatible(paramResolved->storage, this->thisResolved->argStorages[i]))
                                continue;
                            
                            if (param->type != this->thisResolved->argumentTypes[i])
                            {
                                std::string conversion = Format("%s(%s)", ctorFun->parameters[i]->type.name.c_str(), this->thisResolved->argTypes[i]->name.c_str());
                                Symbol* componentConversionSymbol = compiler->GetSymbol(conversion);

                                // No conversion available for this member, skip to next constructor
                                if (componentConversionSymbol == nullptr)
                                {
                                    break;
                                }
                                candidate.needsConversion = true;
                                if (paramResolved->typeSymbol->columnSize == this->thisResolved->argTypes[i]->columnSize)
                                    candidate.simpleConversion = true;
                                candidate.argumentConversionFunctions.push_back(static_cast<Function*>(componentConversionSymbol));
                            }
                            else
                            {
                                // No conversion needed
                                candidate.argumentConversionFunctions.push_back(nullptr);
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
                candidate.simpleConversion = false;

                if (fun->parameters.size() == this->thisResolved->argTypes.size())
                {
                    uint32_t numMatches = 0;
                    for (size_t i = 0; i < fun->parameters.size(); i++)
                    {
                        Variable* param = fun->parameters[i];
                        Variable::__Resolved* paramResolved = Symbol::Resolved(param);

                        // There is no help if storage doesn't align
                        if (!IsStorageCompatible(paramResolved->storage, this->thisResolved->argStorages[i]))
                            continue;
                        
                        if (fun->parameters[i]->type != this->thisResolved->argumentTypes[i])
                        {
                            std::string conversion = Format("%s(%s)", fun->parameters[i]->type.name.c_str(), this->thisResolved->argTypes[i]->name.c_str());
                            Symbol* componentConversionSymbol = compiler->GetSymbol(conversion);

                            // No conversion available for this member, skip to next constructor
                            if (componentConversionSymbol == nullptr)
                            {
                                break;
                            }
                            candidate.needsConversion = true;
                            if (paramResolved->typeSymbol->columnSize == this->thisResolved->argTypes[i]->columnSize)
                                candidate.simpleConversion = true;
                            candidate.argumentConversionFunctions.push_back(static_cast<Function*>(componentConversionSymbol));
                        }
                        else
                        {
                            // No conversion needed
                            candidate.argumentConversionFunctions.push_back(nullptr);
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
            this->thisResolved->function = nullptr;
            return false;
        }
        else
        {
            std::vector<Candidate> ambiguousCalls;
            for (auto& candidate : candidates)
            {
                if (!candidate.needsConversion)
                {
                    this->thisResolved->function = candidate.function;
                    this->thisResolved->returnType = this->thisResolved->function->returnType;
                    this->thisResolved->retType = compiler->GetType(this->thisResolved->returnType);
                    this->thisResolved->conversions.clear();
                    ambiguousCalls.clear();
                    break;
                }
                else if (candidate.simpleConversion)
                {
                    this->thisResolved->function = candidate.function;
                    this->thisResolved->returnType = this->thisResolved->function->returnType;
                    this->thisResolved->retType = compiler->GetType(this->thisResolved->returnType);
                    this->thisResolved->conversions = candidate.argumentConversionFunctions;
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
                        if (candidate.needsConversion)
                            fmt.append("\n      using conversions:");
                        int counter = 0;
                        for (auto& conv : candidate.argumentConversionFunctions)
                        {
                            if (conv != nullptr)
                                fmt.append(Format("\n          argument %d -> %s", counter, Symbol::Resolved(conv)->name.c_str()));
                            counter++;
                        }
                    }
                    compiler->Error(fmt, this);
                    this->thisResolved->function = nullptr;
                    return false;
                }
            }
            else if (ambiguousCalls.size() == 1)
            {
                this->thisResolved->function = ambiguousCalls[0].function;
                this->thisResolved->conversions = ambiguousCalls[0].argumentConversionFunctions;
                this->thisResolved->returnType = this->thisResolved->function->returnType;
                this->thisResolved->retType = compiler->GetType(this->thisResolved->returnType);
            }
        }
    }
    else
    {
        if (symbol->symbolType != Symbol::FunctionType)
        {
            compiler->Error(Format("Unrecognized function '%s' called", this->thisResolved->functionSymbol.c_str()), this);
        }
        this->thisResolved->function = static_cast<Function*>(symbol);
    }

    if (this->thisResolved->function != nullptr)
    {
        this->thisResolved->returnType = this->thisResolved->function->returnType;
        this->thisResolved->retType = compiler->GetType(this->thisResolved->returnType);
       
        if (thisResolved->retType == nullptr)
        {
            compiler->UnrecognizedTypeError(this->thisResolved->returnType.ToString(), this);
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
    out = this->thisResolved->returnType;
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

//------------------------------------------------------------------------------
/**
*/
bool
CallExpression::EvalStorage(Storage& out) const
{
    // Function calls return values on the stack
    Function::__Resolved* funResolved = Symbol::Resolved(this->thisResolved->function);
    out = funResolved->returnValueStorage;
    return true;
}

} // namespace GPULang
