//------------------------------------------------------------------------------
//  @file hgenerator.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "hgenerator.h"
#include "ast/symbol.h"
#include "ast/structure.h"
#include "ast/function.h"
#include "ast/expressions/initializerexpression.h"
#include "compiler.h"
#include "util.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
bool 
HGenerator::Generate(Compiler* compiler, Program* program, const std::vector<Symbol*>& symbols, std::function<void(const std::string&, const std::string&)> writerFunc)
{
    std::string output;
    output.append(Format("namespace %s\n", compiler->filename.c_str()));
    output.append("{\n");
    for (Symbol* sym : symbols)
    {
        switch (sym->symbolType)
        {
        case Symbol::StructureType:
            this->GenerateStructureH(compiler, program, sym, output);
            break;
        case Symbol::VariableType:
            this->GenerateVariableH(compiler, program, sym, output, false);
            break;
        case Symbol::FunctionType:
            this->GenerateFunctionH(compiler, program, sym, output);
            break;
        }
    }

    output.append(Format("} // namespace %s\n", compiler->filename.c_str()));

    // output header
    if (writerFunc)
        writerFunc("header", output);

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
HGenerator::GenerateStructureH(Compiler* compiler, Program* program, Symbol* symbol, std::string& outCode)
{
    Structure* struc = static_cast<Structure*>(symbol);
    Structure::__Resolved* strucResolved = Symbol::Resolved(struc);
    std::string variables;
    std::string offsets;
    for (Symbol* sym : struc->symbols)
    {
        if (sym->symbolType == Symbol::SymbolType::VariableType)
        {
            Variable* var = static_cast<Variable*>(sym);
            Variable::__Resolved* varResolved = Symbol::Resolved(var);
            this->GenerateVariableH(compiler, program, var, variables, false);
            variables.append("\n");

            offsets.append(Format("    static const uint32_t %s_OFFSET = %d;\n", var->name.c_str(), varResolved->structureOffset));
        }
    }

    outCode.append(Format("struct %s\n", struc->name.c_str()));
    outCode.append("{\n");
    outCode.append(Format("    static const uint32_t SIZE = %d;\n", strucResolved->byteSize));
    outCode.append(offsets);
    /*
    if (strucResolved->binding != Structure::__Resolved::NOT_BOUND)
        outCode.append(Format("    static const uint32_t BINDING = %d;\n", strucResolved->binding));
    if (strucResolved->group != Structure::__Resolved::NOT_BOUND)
        outCode.append(Format("    static const uint32_t GROUP = %d;\n", strucResolved->group));
        */
    outCode.append("\n");
    outCode.append(variables);
    outCode.append("};\n\n");
}

//------------------------------------------------------------------------------
/**
*/
void
GenerateHInitializer(Compiler* compiler, Expression* expr, std::string& outCode)
{
    std::string inner;
    InitializerExpression* initExpression = static_cast<InitializerExpression*>(expr);
    for (Expression* expr : initExpression->values)
    {
        if (expr->symbolType == Symbol::InitializerExpressionType)
            GenerateHInitializer(compiler, expr, inner);
        else
        {
            Type::FullType type;
            if (!expr->EvalType(type))
            {
                compiler->Error(Format("INTERNAL ERROR IN '%s' LINE '%s'", __FILE__, __LINE__), expr);
            }

            inner = expr->EvalString();
        }
    }

    outCode = Format("{ %s }", inner.c_str());
}

//------------------------------------------------------------------------------
/**
*/
void 
HGenerator::GenerateVariableH(Compiler* compiler, Program* program, Symbol* symbol, std::string& outCode, bool isShaderArgument)
{
    Variable* var = static_cast<Variable*>(symbol);
    Variable::__Resolved* varResolved = static_cast<Variable::__Resolved*>(var->resolved);
    if (varResolved->usageBits.flags.isStructMember)
    {        
        // add start padding if any
        if (varResolved->startPadding > 0)
        {
            outCode.append(Format("    unsigned int : %d;\n", varResolved->startPadding));
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
                    outCode.append(Format("    unsigned int : %d;\n", varResolved->elementPadding));
                outCode.append(Format("    %s %s_%d%s;", type.c_str(), var->name.c_str(), i, arrayType.c_str()));
                if (i < varResolved->type.modifierValues[i] - 1)
                    outCode.append("\n");
            }
        }
        else
        {
            outCode.append(Format("    %s %s%s;", type.c_str(), var->name.c_str(), arrayType.c_str()));
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
            outCode.append(Format("struct %s\n", varResolved->name.c_str()));
            outCode.append("{\n");
            outCode.append(Format("    static const uint32_t BINDING = %d;\n", varResolved->binding));
            outCode.append(Format("    static const uint32_t GROUP = %d;\n", varResolved->group));
            if (varResolved->typeSymbol->category == Type::Category::UserTypeCategory)
            {
                outCode.append(Format("    using STRUCT = %s;\n", varResolved->typeSymbol->name.c_str()));
            }
            outCode.append("};\n\n");
        }
    }
    else if (varResolved->usageBits.flags.isConst)
    {
        std::string typeStr = typeToHeaderType[var->type.name];
        std::string arrayTypeStr = typeToArraySize[var->type.name];
        std::string initializerStr;
        if (varResolved->value != nullptr)
        {
            GenerateHInitializer(compiler, varResolved->value, initializerStr);
        }
       
        std::string arraySize = "";
        for (int i = varResolved->type.modifierValues.size() - 1; i >= 0; i--)
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

        if (varResolved->value != nullptr)
            outCode.append(Format("    static const %s %s%s%s = %s;\n", typeStr.c_str(), var->name.c_str(), arraySize.c_str(), arrayTypeStr.c_str(), initializerStr.c_str()));
        else
            outCode.append(Format("    static const %s %s%s%s;\n", typeStr.c_str(), var->name.c_str(), arraySize.c_str(), arrayTypeStr.c_str()));
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
HGenerator::GenerateFunctionH(Compiler* compiler, Program* program, Symbol* symbol, std::string& outCode)
{
    Function* fun = static_cast<Function*>(symbol);
    Function::__Resolved* funResolved = Symbol::Resolved(fun);
    if (funResolved->isEntryPoint)
    {
        outCode.append(Format("struct %s\n", fun->name.c_str()));
        outCode.append("{\n");
        if (funResolved->shaderUsage.flags.vertexShader)
        {
            uint32_t offset = 0;
            for (Variable* arg : fun->parameters)
            {
                Variable::__Resolved* argRes = Symbol::Resolved(arg);
                if (argRes->usageBits.flags.isEntryPointParameter && argRes->storage == Variable::__Resolved::Storage::Input)
                {
                    outCode.append(Format("    static const uint32_t %s_BINDING = %d;\n", argRes->name.c_str(), argRes->inBinding));
                    outCode.append(Format("    static const uint32_t %s_OFFSET = %d;\n", argRes->name.c_str(), offset));
                    outCode.append(Format("    static const uint32_t %s_SIZE = %d;\n", argRes->name.c_str(), argRes->byteSize));
                    offset += argRes->byteSize;
                }
            }
            outCode.append(Format("    static const uint32_t VERTEX_STRIDE = %d;\n", offset));
        }
        if (funResolved->shaderUsage.flags.computeShader)
        {
            const Function::__Resolved::ExecutionModifiers& mods = funResolved->executionModifiers;
            outCode.append(Format("    static const uint32_t WORKGROUP_SIZE[] = { %d, %d, %d };\n", mods.computeShaderWorkGroupSize[0], mods.computeShaderWorkGroupSize[1], mods.computeShaderWorkGroupSize[2]));
        }
        outCode.append("};\n");
    }
}

} // namespace GPULang
