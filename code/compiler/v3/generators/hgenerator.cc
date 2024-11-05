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

namespace GPULang
{


//------------------------------------------------------------------------------
/**
*/
bool 
HGenerator::Generate(Compiler* compiler, Program* program, const std::vector<Symbol*>& symbols, std::function<void(const std::string&, const std::string&)> writerFunc)
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



std::map<std::string, std::string> typeToHeaderType =
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

std::map<std::string, std::string> typeToArraySize =
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
HGenerator::GenerateStructureH(Compiler* compiler, Program* program, Symbol* symbol, HeaderWriter& writer)
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

    writer.Unindent();
    writer.WriteLine("};\n");
}

//------------------------------------------------------------------------------
/**
*/
void
GenerateHInitializer(Compiler* compiler, Expression* expr, HeaderWriter& writer)
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
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
HGenerator::GenerateVariableH(Compiler* compiler, Program* program, Symbol* symbol, HeaderWriter& writer, bool isShaderArgument, bool evaluateLinkDefinedVariables)
{
    Variable* var = static_cast<Variable*>(symbol);
    Variable::__Resolved* varResolved = static_cast<Variable::__Resolved*>(var->resolved);

    if (evaluateLinkDefinedVariables)
    {
        if (varResolved->storage == Variable::__Resolved::Storage::LinkDefined)
        {
            std::string typeStr;
            auto headerType = typeToHeaderType.find(var->type.name);
            if (headerType != typeToHeaderType.end())
                typeStr = headerType->second;
            else
                typeStr = var->type.name;
            std::string arrayTypeStr = typeToArraySize[var->type.name];

            Expression* init = var->valueExpression;

            Program::__Resolved* progResolved = Symbol::Resolved(program);
            HeaderWriter tempWriter;

            auto constOverride = progResolved->constVarInitializationOverrides.find(var);
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
                    size_t size = varResolved->type.modifierValues[i];
                    if (size > 0)
                        arraySize.append(Format("[%d]", size));
                    else
                        arraySize.append(Format("[]"));
                }
            }

            if (init != nullptr)
                writer.WriteLine(Format("static inline const %s %s%s%s = %s;", typeStr.c_str(), var->name.c_str(), arraySize.c_str(), arrayTypeStr.c_str(), tempWriter.output.c_str()));
            else
                writer.WriteLine(Format("static inline const %s %s%s%s;", typeStr.c_str(), var->name.c_str(), arraySize.c_str(), arrayTypeStr.c_str()));
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

            std::string type = typeToHeaderType[var->type.name];
            std::string arrayType = typeToArraySize[var->type.name];

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
                    writer.Write(Format("%s %s_%d%s;", type.c_str(), var->name.c_str(), i, arrayType.c_str()));
                    if (i < varResolved->type.modifierValues[i] - 1)
                        writer.Write("\n");
                }
            }
            else
            {
                writer.Write(Format("%s %s%s;", type.c_str(), var->name.c_str(), arrayType.c_str()));
            }
        }
        else if (varResolved->storage == Variable::__Resolved::Storage::Uniform)
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
        else if (varResolved->storage == Variable::__Resolved::Storage::LinkDefined)
        {
            writer.WriteLine(Format("struct %s", varResolved->name.c_str()));
            writer.WriteLine("{");
            writer.Indent();
            writer.WriteLine(Format("static const unsigned int LINK_BINDING = %d;", varResolved->binding));
            writer.WriteLine(Format("static const unsigned int SIZE = %d;", varResolved->byteSize));
            writer.Unindent();
            writer.WriteLine("};\n");
        }
        else if (varResolved->usageBits.flags.isConst)
        {
            std::string arraySize = "";
            for (int i = 0; i < varResolved->type.modifierValues.size(); i++)
            {
                if (varResolved->type.modifiers[i] == Type::FullType::Modifier::Array)
                {
                    size_t size = varResolved->type.modifierValues[i];
                    if (size > 0)
                        arraySize.append(Format("[%d]", size));
                    else
                        arraySize.append(Format("[]"));
                }
            }

            uint32_t accessFlags;
            var->valueExpression->EvalAccessFlags(accessFlags);
            if (accessFlags & AccessFlags::LinkTime)
                return;
            
            HeaderWriter initWriter;
            if (var->valueExpression != nullptr)
            {
                GenerateHInitializer(compiler, var->valueExpression, initWriter);
                writer.WriteLine(Format("static const %s %s%s = %s;", typeToHeaderType[varResolved->type.name].c_str(), varResolved->name.c_str(), arraySize.c_str(), initWriter.output.c_str()));
            }
            else
            {
                writer.WriteLine(Format("static const %s %s%s;", typeToHeaderType[varResolved->type.name].c_str(), varResolved->name.c_str(), arraySize.c_str()));
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
HGenerator::GenerateEnumH(Compiler* compiler, Program* program, Symbol* symbol, HeaderWriter& writer)
{
    Enumeration* enu = static_cast<Enumeration*>(symbol);
    Enumeration::__Resolved* enuResolved = Symbol::Resolved(enu);

    writer.WriteLine(Format("enum %s", enu->name.c_str()));
    writer.WriteLine("{");
    writer.Indent();

    for (size_t i = 0; i < enu->labels.size(); i++)
    {
        HeaderWriter tempWriter;
        tempWriter.Write(Format("%s", enu->labels[i].c_str()));
        if (enu->values[i] != nullptr)
        {
            uint32_t val;
            enu->values[i]->EvalUInt(val);
            tempWriter.Write(Format(" = %d", val));
        }
        if (i != enu->labels.size() - 1)
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
HGenerator::GenerateProgramH(Compiler* compiler, Program* program, const std::vector<Symbol*>& symbols, HeaderWriter& writer)
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
HGenerator::GenerateFunctionH(Compiler* compiler, Program* program, Symbol* symbol, Program::__Resolved::ProgramEntryType shaderType, HeaderWriter& writer)
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
                if (argRes->usageBits.flags.isEntryPointParameter && argRes->storage == Variable::__Resolved::Storage::Input)
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
