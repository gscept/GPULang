//------------------------------------------------------------------------------
//  @file hgenerator.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "hgenerator.h"
#include "ast/symbol.h"
#include "ast/structure.h"
#include "ast/expressions/initializerexpression.h"
#include "compiler.h"
#include "util.h"
namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
bool 
HGenerator::Generate(Compiler* compiler, Program* program, const std::vector<Symbol*>& symbols, std::function<void(const std::string&, const std::string&)> writerFunc)
{
    std::string output;
    for (Symbol* sym : symbols)
    {
        switch (sym->symbolType)
        {
        case Symbol::StructureType:
            this->GenerateStructure(compiler, program, sym, output);
            break;
        case Symbol::VariableType:
            this->GenerateVariable(compiler, program, sym, output, false);
            break;
        }
    }

    // output header
    if (writerFunc)
        writerFunc("header", output);

    return true;
}

std::map<std::string, std::string> typeToHeaderType =
{
    { "float", "float" },
    { "float2", "float" },
    { "float3", "float" },
    { "float4", "float" },
    { "int", "int" },
    { "int2", "int" },
    { "int3", "int" },
    { "int4", "int" },
    { "uint", "unsigned int" },
    { "uint2", "unsigned int" },
    { "uint3", "unsigned int" },
    { "uint4", "unsigned int" },
    { "bool", "bool" },
    { "bool2", "bool" },
    { "bool3", "bool" },
    { "bool4", "bool" },
    { "float2x2", "float" },
    { "float2x3", "float" },
    { "float2x4", "float" },
    { "float3x2", "float" },
    { "float3x3", "float" },
    { "float3x4", "float" },
    { "float4x2", "float" },
    { "float4x3", "float" },
    { "float4x4", "float" },
    { "void", "void" }
};

std::map<std::string, std::string> typeToArraySize =
{
    { "float", "" },
    { "float2", "[2]" },
    { "float3", "[3]" },
    { "float4", "[4]" },
    { "int", "" },
    { "int2", "[2]" },
    { "int3", "[3]" },
    { "int4", "[4]" },
    { "uint", "" },
    { "uint2", "[2]" },
    { "uint3", "[3]" },
    { "uint4", "[4]" },
    { "bool", "" },
    { "bool2", "[2]" },
    { "bool3", "[3]" },
    { "bool4", "[4]" },
    { "float2x2", "[2][2]" },
    { "float2x3", "[2][3]" },
    { "float2x4", "[2][4]" },
    { "float3x2", "[3][2]" },
    { "float3x3", "[3][3]" },
    { "float3x4", "[3][4]" },
    { "float4x2", "[4][2]" },
    { "float4x3", "[4][3]" },
    { "float4x4", "[4][4]" },
    { "void", "void" }
};

//------------------------------------------------------------------------------
/**
*/
void 
HGenerator::GenerateStructure(Compiler* compiler, Program* program, Symbol* symbol, std::string& outCode)
{
    Structure* struc = static_cast<Structure*>(symbol);
    std::string variables;
    for (Symbol* sym : struc->symbols)
    {
        if (sym->symbolType == Symbol::SymbolType::VariableType)
        {
            Variable* var = static_cast<Variable*>(sym);
            this->GenerateVariable(compiler, program, var, variables, false);
            if (var != struc->symbols.back())
                variables.append("\n");
        }
    }

    outCode.append(Format("struct %s\n{\n%s\n};\n\n", struc->name.c_str(), variables.c_str()));
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
HGenerator::GenerateVariable(Compiler* compiler, Program* program, Symbol* symbol, std::string& outCode, bool isShaderArgument)
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
    else if (varResolved->usageBits.flags.isConst)
    {
        std::string typeStr = typeToHeaderType[var->type.name];
        std::string arrayTypeStr = typeToArraySize[var->type.name];
        std::string initializerStr;
        GenerateHInitializer(compiler, varResolved->value, initializerStr);
       
        std::string arraySize = "";
        for (int i = varResolved->type.modifierValues.size() - 1; i >= 0; i--)
        {
            size_t size = varResolved->type.modifierValues[i];
            if (size > 0)
                arraySize.append(Format("[%d]", size));
            else
                arraySize.append(Format("[]"));
        }

        outCode.append(Format("const %s %s%s%s = %s;\n", typeStr.c_str(), var->name.c_str(), arraySize.c_str(), arrayTypeStr.c_str(), initializerStr.c_str()));
    }
}

} // namespace AnyFX
