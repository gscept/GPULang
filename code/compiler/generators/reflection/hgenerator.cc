//------------------------------------------------------------------------------
//  @file hgenerator.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "hgenerator.h"
#include "ast/symbol.h"
#include "ast/structure.h"
#include "ast/function.h"
#include "ast/enumeration.h"
#include "ast/expressions/initializerexpression.h"
#include "ast/expressions/accessexpression.h"
#include "ast/expressions/floatexpression.h"
#include "ast/expressions/intexpression.h"
#include "ast/expressions/uintexpression.h"
#include "ast/expressions/boolexpression.h"
#include "ast/expressions/enumexpression.h"
#include "ast/generate.h"
#include "ast/statements/scopestatement.h"
#include "ast/statements/ifstatement.h"
#include "compiler.h"
#include "util.h"
#include "ast/expressions/arrayinitializerexpression.h"
#include "ast/expressions/callexpression.h"
#include <regex>

namespace GPULang
{


//------------------------------------------------------------------------------
/**
*/
bool 
HGenerator::Generate(const Compiler* compiler, const ProgramInstance* program, const PinnedArray<Symbol*>& symbols, std::function<void(const std::string&, const std::string&)> writerFunc)
{
    HeaderWriter writer;
    writer.WriteLine("#pragma once");
    writer.WriteLine(Format("namespace %s", compiler->filename.c_str()));
    writer.WriteLine("{");

    this->GenerateSymbols(compiler, program, symbols, writer);
   
    writer.WriteLine(Format("} // namespace %s", compiler->filename.c_str()));

    // output header
    if (writerFunc)
        writerFunc("header", writer.output);

    return true;
}

//------------------------------------------------------------------------------
/**
*/
void 
HGenerator::GenerateSymbols(const Compiler* compiler, const ProgramInstance* program, const PinnedArray<Symbol*>& symbols, HeaderWriter& writer)
{
    for (Symbol* sym : symbols)
    {
        this->GenerateSymbol(compiler, program, sym, writer);
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
HGenerator::GenerateSymbol(const Compiler* compiler, const ProgramInstance* program, const Symbol* sym, HeaderWriter& writer)
{
    switch (sym->symbolType)
    {
        case Symbol::GenerateType:
        {
            const GPULang::Generate* generate = static_cast<const GPULang::Generate*>(sym);
            this->GenerateSymbols(compiler, program, generate->symbols, writer);
            break;
        }
        case Symbol::IfStatementType:
        {
            const IfStatement* cond = static_cast<const IfStatement*>(sym);
            ValueUnion val;
            if (cond->condition->EvalValue(val))
            {
                if (val.b[0])
                {
                    ScopeStatement* scope = static_cast<ScopeStatement*>(cond->ifStatement);
                    this->GenerateSymbols(compiler, program, scope->symbols, writer);
                }
                else if (cond->elseStatement != nullptr)
                {
                    this->GenerateSymbol(compiler, program, cond->elseStatement, writer);
                }

            }
            break;
        }
        case Symbol::ScopeStatementType:
        {
            const ScopeStatement* scope = static_cast<const ScopeStatement*>(sym);
            this->GenerateSymbols(compiler, program, scope->symbols, writer);
            break;
        }
        case Symbol::StructureType:
            this->GenerateStructureH(compiler, nullptr, sym, writer);
            break;
        case Symbol::VariableType:
            this->GenerateVariableH(compiler, nullptr, sym, writer, false, false);
            break;
        case Symbol::EnumerationType:
            this->GenerateEnumH(compiler, nullptr, sym, writer);
            break;
        //case Symbol::ProgramInstanceType:
        //    this->GenerateProgramH(compiler, static_cast<const ProgramInstance*>(sym), symbols, writer);
        //    break;

    }
}



StaticMap typeToHeaderType =
std::array{
    std::pair{ ConstantString("f32"), ConstantString("float") },
    std::pair{ ConstantString("f32x2"), ConstantString("float") },
    std::pair{ ConstantString("f32x3"), ConstantString("float") },
    std::pair{ ConstantString("f32x4"), ConstantString("float") },
    std::pair{ ConstantString("i32"), ConstantString("int") },
    std::pair{ ConstantString("i32x2"), ConstantString("int") },
    std::pair{ ConstantString("i32x3"), ConstantString("int") },
    std::pair{ ConstantString("i32x4"), ConstantString("int") },
    std::pair{ ConstantString("u32"), ConstantString("unsigned int") },
    std::pair{ ConstantString("u32x2"), ConstantString("unsigned int") },
    std::pair{ ConstantString("u32x3"), ConstantString("unsigned int") },
    std::pair{ ConstantString("u32x4"), ConstantString("unsigned int") },
    std::pair{ ConstantString("b8"), ConstantString("bool") },
    std::pair{ ConstantString("b8x2"), ConstantString("bool") },
    std::pair{ ConstantString("b8x3"), ConstantString("bool") },
    std::pair{ ConstantString("b8x4"), ConstantString("bool") },
    std::pair{ ConstantString("f32x2x2"), ConstantString("float") },
    std::pair{ ConstantString("f32x2x3"), ConstantString("float") },
    std::pair{ ConstantString("f32x2x4"), ConstantString("float") },
    std::pair{ ConstantString("f32x3x2"), ConstantString("float") },
    std::pair{ ConstantString("f32x3x3"), ConstantString("float") },
    std::pair{ ConstantString("f32x3x4"), ConstantString("float") },
    std::pair{ ConstantString("f32x4x2"), ConstantString("float") },
    std::pair{ ConstantString("f32x4x3"), ConstantString("float") },
    std::pair{ ConstantString("f32x4x4"), ConstantString("float") },
    std::pair{ ConstantString("void"), ConstantString("void") }
};

StaticMap typeToArraySize =
std::array{
    std::pair{ ConstantString("f32"), ConstantString("") },
    std::pair{ ConstantString("f32x2"), ConstantString("[2]") },
    std::pair{ ConstantString("f32x3"), ConstantString("[3]") },
    std::pair{ ConstantString("f32x4"), ConstantString("[4]") },
    std::pair{ ConstantString("i32"), ConstantString("") },
    std::pair{ ConstantString("i32x2"), ConstantString("[2]") },
    std::pair{ ConstantString("i32x3"), ConstantString("[3]") },
    std::pair{ ConstantString("i32x4"), ConstantString("[4]") },
    std::pair{ ConstantString("u32"), ConstantString("") },
    std::pair{ ConstantString("u32x2"), ConstantString("[2]") },
    std::pair{ ConstantString("u32x3"), ConstantString("[3]") },
    std::pair{ ConstantString("u32x4"), ConstantString("[4]") },
    std::pair{ ConstantString("b8"), ConstantString("") },
    std::pair{ ConstantString("b8x2"), ConstantString("[2]") },
    std::pair{ ConstantString("b8x3"), ConstantString("[3]") },
    std::pair{ ConstantString("b8x4"), ConstantString("[4]") },
    std::pair{ ConstantString("f32x2x2"), ConstantString("[2][2]") },
    std::pair{ ConstantString("f32x2x3"), ConstantString("[2][3]") },
    std::pair{ ConstantString("f32x2x4"), ConstantString("[2][4]") },
    std::pair{ ConstantString("f32x3x2"), ConstantString("[3][2]") },
    std::pair{ ConstantString("f32x3x3"), ConstantString("[3][3]") },
    std::pair{ ConstantString("f32x3x4"), ConstantString("[3][4]") },
    std::pair{ ConstantString("f32x4x2"), ConstantString("[4][2]") },
    std::pair{ ConstantString("f32x4x3"), ConstantString("[4][3]") },
    std::pair{ ConstantString("f32x4x4"), ConstantString("[4][4]") },
    std::pair{ ConstantString("void"), ConstantString("void") }
};

//------------------------------------------------------------------------------
/**
*/
void 
HGenerator::GenerateStructureH(const Compiler* compiler, const ProgramInstance* program, const Symbol* symbol, HeaderWriter& writer)
{
    const Structure* struc = static_cast<const Structure*>(symbol);
    Structure::__Resolved* strucResolved = Symbol::Resolved(struc);
    writer.WriteLine(Format("struct alignas(16) %s", struc->name.c_str()));
    writer.WriteLine("{");
    writer.Indent();
    for (Symbol* sym : struc->symbols)
    {
        if (sym->symbolType == Symbol::SymbolType::VariableType)
        {
            Variable* var = static_cast<Variable*>(sym);
            Variable::__Resolved* varResolved = Symbol::Resolved(var);
            this->GenerateVariableH(compiler, program, var, writer, false, false);
            writer.WriteLine("");
        }
    }
    uint32_t numPads = strucResolved->endPadding / 4;
    for (uint32_t i = 0; i < numPads; i++)
        writer.WriteLine("unsigned int : 32;");

    writer.Unindent();
    writer.WriteLine("};\n");
}

//------------------------------------------------------------------------------
/**
*/
void
GenerateHInitializer(const Compiler* compiler, Expression* expr, HeaderWriter& writer)
{
    switch (expr->symbolType)
    {
        case Symbol::AccessExpressionType:
        {
            AccessExpression* accExpr = static_cast<AccessExpression*>(expr);
            writer.Write(Format("%s::%s", accExpr->left->EvalString().c_str(), accExpr->right->EvalString().c_str()));
            break;
        }
        case Symbol::InitializerExpressionType:
        {
            InitializerExpression* initExpr = static_cast<InitializerExpression*>(expr);
            writer.Write("{");
            for (Expression* expr : initExpr->values)
            {
                writer.Write(expr->EvalString());
                if (expr != initExpr->values.back())
                    writer.Write(", ");
            }
            writer.Write("};");
            break;
        }
        case Symbol::BinaryExpressionType:
        {
            ValueUnion val;
            if (expr->EvalValue(val))
            {
                TransientString initializer;
                for (int size = 0; size < val.columnSize; size++)
                {
                    switch (val.code)
                    {
                        case TypeCode::Bool:
                            initializer.Append(val.b[size]);
                            break;
                        case TypeCode::Int:
                        case TypeCode::Int16:
                            initializer.Append(val.i[size]);
                            break;
                        case TypeCode::UInt:
                        case TypeCode::UInt16:
                            initializer.Append(val.ui[size]);
                            break;
                        case TypeCode::Float:
                        case TypeCode::Float16:
                            initializer.Append(val.f[size]);
                            initializer.Append('f');
                            break;
                    }
                    if (size < val.columnSize - 1)
                        initializer.Append(",");
                }
                writer.Write(initializer);
            }
            break;
        }
        case Symbol::FloatExpressionType:
        case Symbol::IntExpressionType:
        case Symbol::UIntExpressionType:
        case Symbol::BoolExpressionType:
            writer.Write(expr->EvalString());
            break;
        case Symbol::ArrayInitializerExpressionType:
        {
            auto* initExpr = static_cast<ArrayInitializerExpression*>(expr);
            writer.Write("{");
            for (Expression* expr : initExpr->values)
            {
                GenerateHInitializer(compiler, expr, writer);
                //writer.Write(expr->EvalString());
                if (expr != initExpr->values.back())
                    writer.Write(", ");
            }
            writer.Write("}");
            break;
        }
        case Symbol::CallExpressionType:
        {
            auto callExpr = static_cast<CallExpression*>(expr);
            auto res = Symbol::Resolved(callExpr);
            static std::regex re("(f32|i32|u32|b8)(x[0-9])?(x[0-9])?");
            std::cmatch m;
            if (std::regex_match(res->function->name.c_str(), m, re))
            {
                if (m.length() > 1)
                {
                    writer.Write("{");
                }
                for (auto& arg : callExpr->args)
                {
                    writer.Write(arg->EvalString());
                    if (arg != callExpr->args.back())
                        writer.Write(", ");
                }
                if (m.length() > 1)
                {
                    writer.Write("}");
                }
            }
            res->function->name;
            break;
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
HGenerator::GenerateVariableH(const Compiler* compiler, const ProgramInstance* program, const Symbol* symbol, HeaderWriter& writer, bool isShaderArgument, bool evaluateLinkDefinedVariables)
{
    const Variable* var = static_cast<const Variable*>(symbol);
    Variable::__Resolved* varResolved = static_cast<Variable::__Resolved*>(var->resolved);
    if (varResolved->usageBits.flags.isNoReflect) // skip
        return;        

    if (evaluateLinkDefinedVariables)
    {
        if (varResolved->storage == Storage::LinkDefined)
        {
            TStr typeStr;
            auto headerType = typeToHeaderType.Find(var->type.name);
            if (headerType != typeToHeaderType.end())
                typeStr = headerType->second.c_str();
            else
                typeStr = var->type.name;
            FixedString arrayTypeStr(ConstantString(""));
            auto arrayTypeIt = typeToArraySize.Find(var->type.name);
            if (arrayTypeIt != typeToArraySize.end())
                arrayTypeStr = arrayTypeIt->second;

            Expression* init = var->valueExpression;

            ProgramInstance::__Resolved* progResolved = Symbol::Resolved(program);
            HeaderWriter tempWriter;

            auto constOverride = progResolved->constVarInitializationOverrides.Find(var);
            if (constOverride != progResolved->constVarInitializationOverrides.end())
                init = constOverride->second;
            
            if (init != nullptr)
            {
                GenerateHInitializer(compiler, init, tempWriter);
            }

            std::string arraySize = "";
            for (int i = 0; i < var->type.modifierValues.size; i++)
            {
                if (var->type.modifiers[i] == Type::FullType::Modifier::Array)
                {
                    uint32_t size = 0;
                    if (var->type.modifierValues[i] != nullptr)
                    {
                        ValueUnion val;
                        var->type.modifierValues[i]->EvalValue(val);
                        val.Store(size);
                    }
                    if (size > 0)
                        arraySize.append(Format("[%d]", size));
                    else
                        arraySize.append(Format("[]"));
                }
            }

            if (init != nullptr)
                writer.WriteLine(Format("static inline const %s %s%s%s = %s;", typeStr.buf, var->name.c_str(), arraySize.c_str(), arrayTypeStr.c_str(), tempWriter.output.c_str()));
            else
                writer.WriteLine(Format("static inline const %s %s%s%s;", typeStr.buf, var->name.c_str(), arraySize.c_str(), arrayTypeStr.c_str()));
        }
    }
    else
    {
        if (varResolved->usageBits.flags.isStructMember)
        {
            // add start padding if any
            if (varResolved->startPadding > 0)
            {
                uint32_t numElements = varResolved->startPadding / 4;
                for (uint32_t i = 0; i < numElements; i++)
                    writer.WriteLine("unsigned int : 32;");
            }
            writer.Write(TransientString("/* Offset: ", varResolved->structureOffset, "*/"));

            TStr type = var->type.name;
            auto it = typeToHeaderType.Find(type.ToView());
            if (it != typeToHeaderType.end())
                type = it->second.c_str();
            auto item = typeToArraySize.Find(var->type.name);
            TStr arrayType = "";// = typeToArraySize.Find(var->type.name)->second.c_str();
            if (item != typeToArraySize.end())
                arrayType = item->second;
            auto modIt = var->type.modifiers.rbegin();
            while (modIt != var->type.modifiers.rend())
            {
                if (*modIt.it == Type::FullType::Modifier::Pointer)
                    type.Append("*");
                else if (*modIt.it == Type::FullType::Modifier::Array)
                {
                    ptrdiff_t diff = std::distance(modIt, var->type.modifiers.rend()) - 1;
                    ValueUnion val;
                    if (var->type.modifierValues[diff] && var->type.modifierValues[diff]->EvalValue(val))
                    {
                        arrayType = Format("[%d]%s", val.ui[0], arrayType.buf);
                    }
                    else
                    {
                        type.Append("*");
                    }
                }
                modIt++;
            }

            // if element padding, we need to split the array into elements where each element is padded
            if (varResolved->elementPadding > 0)
            {
                for (int i = 0; i < var->type.modifierValues.size; i++)
                {
                    // don't pad the first element
                    if (i > 0)
                    {
                        uint32_t numElements = varResolved->elementPadding / 4;
                        for (uint32_t i = 0; i < numElements; i++)
                            writer.WriteLine("unsigned int : 32;");
                    }
                    uint32_t size = 0;
                    if (var->type.modifierValues[i] != nullptr)
                    {
                        ValueUnion val;
                        var->type.modifierValues[i]->EvalValue(val);
                        val.Store(size);
                    }
                    writer.Write(Format("%s %s_%d%s;", type.buf, var->name.c_str(), i, arrayType.buf));
                    if (i < size - 1)
                        writer.Write("\n");
                }
            }
            else
            {
                writer.Write(Format("%s %s%s;", type.buf, var->name.c_str(), arrayType.buf));
            }
        }
        else if (varResolved->storage == Storage::Uniform)
        {
            if (varResolved->typeSymbol->category == Type::Category::StructureCategory
                || varResolved->typeSymbol->category == Type::Category::TextureCategory
                || varResolved->typeSymbol->category == Type::Category::SamplerCategory
                || varResolved->typeSymbol->category == Type::Category::PixelCacheCategory
                )
            {
                writer.WriteLine(Format("struct %s", var->name.c_str()));
                writer.WriteLine("{");
                writer.Indent();
                writer.WriteLine(Format("static const unsigned int BINDING = %d;", varResolved->binding));
                writer.WriteLine(Format("static const unsigned int GROUP = %d;", varResolved->group));
                if (varResolved->typeSymbol->category == Type::Category::StructureCategory)
                {
                    writer.WriteLine(Format("using STRUCT = %s;", varResolved->typeSymbol->name.c_str()));
                }
                writer.Unindent();
                writer.WriteLine("};\n");
                
            }
        }
        else if (varResolved->storage == Storage::InlineUniform)
        {
            writer.WriteLine(Format("struct %s", var->name.c_str()));
            writer.WriteLine("{");
            writer.Indent();
            writer.WriteLine(Format("using STRUCT = %s;", varResolved->typeSymbol->name.c_str()));
            writer.Unindent();
            writer.WriteLine("};\n");
        }
        else if (varResolved->storage == Storage::LinkDefined)
        {
            writer.WriteLine(Format("struct %s", var->name.c_str()));
            writer.WriteLine("{");
            writer.Indent();
            writer.WriteLine(Format("static const unsigned int LINK_BINDING = %d;", varResolved->binding));
            writer.WriteLine(Format("static const unsigned int SIZE = %d;", varResolved->byteSize));
            writer.Unindent();
            writer.WriteLine("};\n");
        }
        else if (varResolved->usageBits.flags.isConst && varResolved->storage == Storage::Global)
        {
            TStr type = var->type.name;
            auto it = typeToHeaderType.Find(type.ToView());
            if (it != typeToHeaderType.end())
                type = it->second.c_str();
            auto arrayTypeIt = typeToArraySize.Find(var->type.name);
            TStr arrayType = "";
            if (arrayTypeIt != typeToArraySize.end())
                arrayType = arrayTypeIt->second.c_str();
            auto modIt = var->type.modifiers.rbegin();
            while (modIt != var->type.modifiers.rend())
            {
                if (*modIt.it == Type::FullType::Modifier::Pointer)
                    type.Append("*");
                else if (*modIt.it == Type::FullType::Modifier::Array)
                {
                    ptrdiff_t diff = std::distance(modIt, var->type.modifiers.rend()) - 1;
                    ValueUnion val;
                    if (var->type.modifierValues[diff]->EvalValue(val))
                    {
                        arrayType = Format("[%d]%s", val.ui[0], arrayType.buf);
                    }
                    else
                    {
                        type.Append("*");
                    }
                }
                modIt++;
            }

            uint32_t accessFlags;
            var->valueExpression->EvalAccessFlags(accessFlags);
            if (accessFlags & AccessFlags::LinkTime)
                return;
            
            HeaderWriter initWriter;
            if (var->valueExpression != nullptr)
            {
                GenerateHInitializer(compiler, var->valueExpression, initWriter);
                writer.WriteLine(Format("static const %s %s%s = %s;", type.buf, var->name.c_str(), arrayType.buf, initWriter.output.c_str()));
            }
            else
            {
                writer.WriteLine(Format("static const %s %s%s;", type.buf, var->name.c_str(), arrayType.buf));
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
HGenerator::GenerateEnumH(const Compiler* compiler, const ProgramInstance* program, const  Symbol* symbol, HeaderWriter& writer)
{
    const Enumeration* enu = static_cast<const Enumeration*>(symbol);
    Enumeration::__Resolved* enuResolved = Symbol::Resolved(enu);

    writer.WriteLine(Format("enum %s", enu->name.c_str()));
    writer.WriteLine("{");
    writer.Indent();

    for (size_t i = 0; i < enu->labels.size; i++)
    {
        HeaderWriter tempWriter;
        tempWriter.Write(Format("%s", enu->labels.buf[i].c_str()));
        if (enu->values.buf[i] != nullptr)
        {
            uint32_t val;
            ValueUnion value;
            enu->values.buf[i]->EvalValue(value);
            value.Store(val);
            tempWriter.Write(Format(" = %d", val));
        }
        if (i != enu->labels.size - 1)
            tempWriter.Write(",");
        writer.WriteLine(tempWriter.output);
    }
    writer.Unindent();
    writer.Write("};\n\n");
}

//------------------------------------------------------------------------------
/**
*/
void 
HGenerator::GenerateProgramH(const Compiler* compiler, const ProgramInstance* program, const PinnedArray<Symbol*>& symbols, HeaderWriter& writer)
{
    writer.WriteLine(Format("struct %s", program->name.c_str()));
    writer.WriteLine("{");
    writer.Indent();
    for (Symbol* sym : symbols)
    {
        switch (sym->symbolType)
        {
        case Symbol::VariableType:
            this->GenerateVariableH(compiler, program, sym, writer, false, true);
            break;
        }
    }

    // Generate program mappings
    ProgramInstance::__Resolved* progRes = Symbol::Resolved(program);
    for (uint32_t i = ProgramInstance::__Resolved::EntryType::FirstShader; i <= ProgramInstance::__Resolved::EntryType::LastShader; i++)
    {
        if (progRes->mappings[i] != nullptr)
        {
            this->GenerateFunctionH(compiler, program, progRes->mappings[i], ProgramInstance::__Resolved::EntryType(i), writer);
        }
    }
    writer.Unindent();
    writer.WriteLine("};\n");
}

//------------------------------------------------------------------------------
/**
*/
void 
HGenerator::GenerateFunctionH(const Compiler* compiler, const ProgramInstance* program, const Symbol* symbol, ProgramInstance::__Resolved::EntryType shaderType, HeaderWriter& writer)
{
    const Function* fun = static_cast<const Function*>(symbol);
    Function::__Resolved* funResolved = Symbol::Resolved(fun);
    if (funResolved->isEntryPoint)
    {
        writer.WriteLine(Format("struct %s", fun->name.c_str()));
        writer.WriteLine("{");
        writer.Indent();
        if (shaderType == ProgramInstance::__Resolved::VertexShader)
        {
            uint32_t offset = 0;
            for (Variable* arg : fun->parameters)
            {
                Variable::__Resolved* argRes = Symbol::Resolved(arg);
                if (argRes->usageBits.flags.isEntryPointParameter && argRes->storage == Storage::Input)
                {
                    writer.WriteLine(Format("static const unsigned int %s_BINDING = %d;", arg->name.c_str(), argRes->inBinding));
                    writer.WriteLine(Format("static const unsigned int %s_OFFSET = %d;", arg->name.c_str(), offset));
                    writer.WriteLine(Format("static const unsigned int %s_SIZE = %d;", arg->name.c_str(), argRes->byteSize));
                    offset += argRes->byteSize;
                }
            }
            writer.WriteLine(Format("static const uint32_t VERTEX_STRIDE = %d;", offset));
        }
        if (shaderType == ProgramInstance::__Resolved::ComputeShader)
        {
            const Function::__Resolved::ExecutionModifiers& mods = funResolved->executionModifiers;
            writer.WriteLine(Format("static inline const unsigned int WORKGROUP_SIZE[] = { %d, %d, %d };", mods.computeShaderWorkGroupSize[0], mods.computeShaderWorkGroupSize[1], mods.computeShaderWorkGroupSize[2]));
        }
        writer.Unindent();
        writer.WriteLine("};\n");
    }
}

} // namespace GPULang
