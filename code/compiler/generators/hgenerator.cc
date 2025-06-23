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
HGenerator::Generate(const Compiler* compiler, const Program* program, const PinnedArray<Symbol*>& symbols, std::function<void(const std::string&, const std::string&)> writerFunc)
{
    HeaderWriter writer;

    writer.WriteLine(Format("namespace %s", compiler->filename.c_str()));
    writer.WriteLine("{");
    for (Symbol* sym : symbols)
    {
        switch (sym->symbolType)
        {
        case Symbol::StructureType:
            this->GenerateStructureH(compiler, nullptr, sym, writer);
            break;
        case Symbol::VariableType:
            this->GenerateVariableH(compiler, nullptr, sym, writer, false, false);
            break;
        case Symbol::EnumerationType:
            this->GenerateEnumH(compiler, nullptr, sym, writer);
            break;
        case Symbol::ProgramType:
            this->GenerateProgramH(compiler, static_cast<Program*>(sym), symbols, writer);
            break;

        }
    }

    writer.WriteLine(Format("} // namespace %s", compiler->filename.c_str()));

    // output header
    if (writerFunc)
        writerFunc("header", writer.output);

    return true;
}



StaticMap<ConstantString, ConstantString> typeToHeaderType =
{
    { "f32", "float" },
    { "f32x2", "float" },
    { "f32x3", "float" },
    { "f32x4", "float" },
    { "i32", "int" },
    { "i32x2", "int" },
    { "i32x3", "int" },
    { "i32x4", "int" },
    { "u32", "unsigned int" },
    { "u32x2", "unsigned int" },
    { "u32x3", "unsigned int" },
    { "u32x4", "unsigned int" },
    { "b8", "bool" },
    { "b8x2", "bool" },
    { "b8x3", "bool" },
    { "b8x4", "bool" },
    { "f32x2x2", "float" },
    { "f32x2x3", "float" },
    { "f32x2x4", "float" },
    { "f32x3x2", "float" },
    { "f32x3x3", "float" },
    { "f32x3x4", "float" },
    { "f32x4x2", "float" },
    { "f32x4x3", "float" },
    { "f32x4x4", "float" },
    { "void", "void" }
};

StaticMap<ConstantString, ConstantString> typeToArraySize =
{
    { "f32", "" },
    { "f32x2", "[2]" },
    { "f32x3", "[3]" },
    { "f32x4", "[4]" },
    { "i32", "" },
    { "i32x2", "[2]" },
    { "i32x3", "[3]" },
    { "i32x4", "[4]" },
    { "u32", "" },
    { "u32x2", "[2]" },
    { "u32x3", "[3]" },
    { "u32x4", "[4]" },
    { "b8", "" },
    { "b8x2", "[2]" },
    { "b8x3", "[3]" },
    { "b8x4", "[4]" },
    { "f32x2x2", "[2][2]" },
    { "f32x2x3", "[2][3]" },
    { "f32x2x4", "[2][4]" },
    { "f32x3x2", "[3][2]" },
    { "f32x3x3", "[3][3]" },
    { "f32x3x4", "[3][4]" },
    { "f32x4x2", "[4][2]" },
    { "f32x4x3", "[4][3]" },
    { "f32x4x4", "[4][4]" },
    { "void", "void" }
};

//------------------------------------------------------------------------------
/**
*/
void 
HGenerator::GenerateStructureH(const Compiler* compiler, const Program* program, Symbol* symbol, HeaderWriter& writer)
{
    Structure* struc = static_cast<Structure*>(symbol);
    Structure::__Resolved* strucResolved = Symbol::Resolved(struc);
    writer.WriteLine(Format("struct %s", struc->name.c_str()));
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
                writer.Write(expr->EvalString());
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
HGenerator::GenerateVariableH(const Compiler* compiler, const Program* program, Symbol* symbol, HeaderWriter& writer, bool isShaderArgument, bool evaluateLinkDefinedVariables)
{
    Variable* var = static_cast<Variable*>(symbol);
    Variable::__Resolved* varResolved = static_cast<Variable::__Resolved*>(var->resolved);

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
            const ConstantString& arrayTypeStr = typeToArraySize.Find(var->type.name)->second;

            Expression* init = var->valueExpression;

            Program::__Resolved* progResolved = Symbol::Resolved(program);
            HeaderWriter tempWriter;

            auto constOverride = progResolved->constVarInitializationOverrides.Find(var);
            if (constOverride != progResolved->constVarInitializationOverrides.end())
                init = constOverride->second;
            
            if (init != nullptr)
            {
                GenerateHInitializer(compiler, init, tempWriter);
            }

            std::string arraySize = "";
            for (int i = 0; i < varResolved->type.modifierValues.size(); i++)
            {
                if (varResolved->type.modifiers[i] == Type::FullType::Modifier::Array)
                {
                    uint32_t size = 0;
                    if (varResolved->type.modifierValues[i] != nullptr)
                    {
                        ValueUnion val;
                        varResolved->type.modifierValues[i]->EvalValue(val);
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

            TStr type = var->type.name;
            auto it = typeToHeaderType.Find(type.ToView());
            if (it != typeToHeaderType.end())
                type = it->second.c_str();
            auto item = typeToArraySize.Find(var->type.name);
            TStr arrayType;// = typeToArraySize.Find(var->type.name)->second.c_str();
            if (item != typeToArraySize.end())
                arrayType = item->second;
            auto modIt = var->type.modifiers.rbegin();
            while (modIt != var->type.modifiers.rend())
            {
                if (*modIt == Type::FullType::Modifier::Pointer)
                    type.Append("*");
                else if (*modIt == Type::FullType::Modifier::Array)
                {
                    ptrdiff_t diff = std::distance(modIt, var->type.modifiers.rend()) - 1;
                    ValueUnion val;
                    if (varResolved->type.modifierValues[diff]->EvalValue(val))
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
                for (int i = 0; i < varResolved->type.modifierValues.size(); i++)
                {
                    // don't pad the first element
                    if (i > 0)
                    {
                        uint32_t numElements = varResolved->elementPadding / 4;
                        for (uint32_t i = 0; i < numElements; i++)
                            writer.WriteLine("unsigned int : 32;");
                    }
                    uint32_t size = 0;
                    if (varResolved->type.modifierValues[i] != nullptr)
                    {
                        ValueUnion val;
                        varResolved->type.modifierValues[i]->EvalValue(val);
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
            if (varResolved->typeSymbol->category == Type::Category::UserTypeCategory
                || varResolved->typeSymbol->category == Type::Category::TextureCategory
                || varResolved->typeSymbol->category == Type::Category::SamplerCategory
                || varResolved->typeSymbol->category == Type::Category::PixelCacheCategory
                )
            {
                writer.WriteLine(Format("struct %s", varResolved->name.c_str()));
                writer.WriteLine("{");
                writer.Indent();
                writer.WriteLine(Format("static const unsigned int BINDING = %d;", varResolved->binding));
                writer.WriteLine(Format("static const unsigned int GROUP = %d;", varResolved->group));
                if (varResolved->typeSymbol->category == Type::Category::UserTypeCategory)
                {
                    writer.WriteLine(Format("using STRUCT = %s;", varResolved->typeSymbol->name.c_str()));
                }
                writer.Unindent();
                writer.WriteLine("};\n");
                
            }
        }
        else if (varResolved->storage == Storage::LinkDefined)
        {
            writer.WriteLine(Format("struct %s", varResolved->name.c_str()));
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
            TStr arrayType = typeToArraySize.Find(var->type.name)->second.c_str();
            auto modIt = var->type.modifiers.rbegin();
            while (modIt != var->type.modifiers.rend())
            {
                if (*modIt == Type::FullType::Modifier::Pointer)
                    type.Append("*");
                else if (*modIt == Type::FullType::Modifier::Array)
                {
                    ptrdiff_t diff = std::distance(modIt, var->type.modifiers.rend()) - 1;
                    ValueUnion val;
                    if (varResolved->type.modifierValues[diff]->EvalValue(val))
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
                writer.WriteLine(Format("static const %s %s%s = %s;", type.buf, varResolved->name.c_str(), arrayType.buf, initWriter.output.c_str()));
            }
            else
            {
                writer.WriteLine(Format("static const %s %s%s;", type.buf, varResolved->name.c_str(), arrayType.buf));
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
HGenerator::GenerateEnumH(const Compiler* compiler, const Program* program, Symbol* symbol, HeaderWriter& writer)
{
    Enumeration* enu = static_cast<Enumeration*>(symbol);
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
HGenerator::GenerateProgramH(const Compiler* compiler, const Program* program, const PinnedArray<Symbol*>& symbols, HeaderWriter& writer)
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
    Program::__Resolved* progRes = Symbol::Resolved(program); 
    for (uint32_t i = Program::__Resolved::ProgramEntryType::FirstShader; i <= Program::__Resolved::ProgramEntryType::LastShader; i++)
    {
        if (progRes->mappings[i] != nullptr)
        {
            this->GenerateFunctionH(compiler, program, progRes->mappings[i], Program::__Resolved::ProgramEntryType(i), writer);
        }
    }
    writer.Unindent();
    writer.WriteLine("};\n");
}

//------------------------------------------------------------------------------
/**
*/
void 
HGenerator::GenerateFunctionH(const Compiler* compiler, const Program* program, Symbol* symbol, Program::__Resolved::ProgramEntryType shaderType, HeaderWriter& writer)
{
    Function* fun = static_cast<Function*>(symbol);
    Function::__Resolved* funResolved = Symbol::Resolved(fun);
    if (funResolved->isEntryPoint)
    {
        writer.WriteLine(Format("struct %s", fun->name.c_str()));
        writer.WriteLine("{");
        writer.Indent();
        if (shaderType == Program::__Resolved::VertexShader)
        {
            uint32_t offset = 0;
            for (Variable* arg : fun->parameters)
            {
                Variable::__Resolved* argRes = Symbol::Resolved(arg);
                if (argRes->usageBits.flags.isEntryPointParameter && argRes->storage == Storage::Input)
                {
                    writer.WriteLine(Format("static const unsigned int %s_BINDING = %d;", argRes->name.c_str(), argRes->inBinding));
                    writer.WriteLine(Format("static const unsigned int %s_OFFSET = %d;", argRes->name.c_str(), offset));
                    writer.WriteLine(Format("static const unsigned int %s_SIZE = %d;", argRes->name.c_str(), argRes->byteSize));
                    offset += argRes->byteSize;
                }
            }
            writer.WriteLine(Format("static const uint32_t VERTEX_STRIDE = %d;", offset));
        }
        if (shaderType == Program::__Resolved::ComputeShader)
        {
            const Function::__Resolved::ExecutionModifiers& mods = funResolved->executionModifiers;
            writer.WriteLine(Format("static inline const unsigned int WORKGROUP_SIZE[] = { %d, %d, %d };", mods.computeShaderWorkGroupSize[0], mods.computeShaderWorkGroupSize[1], mods.computeShaderWorkGroupSize[2]));
        }
        writer.Unindent();
        writer.WriteLine("};\n");
    }
}

} // namespace GPULang
