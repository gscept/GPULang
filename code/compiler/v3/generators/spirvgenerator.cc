//------------------------------------------------------------------------------
//  @file spirvgenerator.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "spirvgenerator.h"
#include "compiler.h"
#include "ast/symbol.h"
#include "ast/function.h"
#include "ast/structure.h"
#include "ast/variable.h"
#include "ast/program.h"
#include "ast/expressions/intexpression.h"
#include "ast/expressions/uintexpression.h"
#include "ast/expressions/floatexpression.h"
#include "ast/expressions/boolexpression.h"
#include "ast/expressions/initializerexpression.h"
#include "ast/expressions/callexpression.h"
#include "ast/expressions/accessexpression.h"
#include "ast/expressions/commaexpression.h"

#include "ast/statements/breakstatement.h"
#include "ast/statements/continuestatement.h"
#include "ast/statements/expressionstatement.h"
#include "ast/statements/forstatement.h"
#include "ast/statements/ifstatement.h"
#include "ast/statements/returnstatement.h"
#include "ast/statements/scopestatement.h"
#include "ast/statements/switchstatement.h"
#include "ast/statements/whilestatement.h"
namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
bool 
SPIRVGenerator::Generate(Compiler* compiler, Program* program, const std::vector<Symbol*>& symbols, std::function<void(const std::string&, const std::string&)> writerFunc)
{
    Program::__Resolved* progResolved = static_cast<Program::__Resolved*>(program->resolved);
    for (auto it : progResolved->programMappings)
    {
        // for each shader, generate code and use it as a binary output
        if (it.first >= Program::__Resolved::VertexShader && it.first <= Program::__Resolved::RayIntersectionShader)
        {
            std::string code;
            this->header.append("; Magic:     0x07230203 (SPIR-V)\n");
            this->header.append("; Version:   0x00010000 (Version: 1.0.0)\n");
            this->header.append("; Generator: 0x00080001 (AnyFX; 1)\n");
            this->header.append("OpCapability Shader\n");

            for (Symbol* sym : symbols)
            {
                switch (sym->symbolType)
                {
                    case Symbol::FunctionType:
                        this->GenerateFunction(compiler, program, sym, code);
                        break;
                    case Symbol::StructureType:
                        this->GenerateStructure(compiler, program, sym, code);
                        break;
                    case Symbol::VariableType:
                        this->GenerateVariable(compiler, program, sym, code, false, true);
                        break;
                }
            }

            std::string binary;
        }
    }
    return false;
}

std::map<std::string, std::string> typeToSpirvType =
{
    { "void", "OpTypeVoid" }
    , { "float", "OpTypeFloat 32" }
    , { "float16", "OpTypeFloat 16" }
    , { "bool", "OpTypeBool" }
    , { "int", "OpTypeInt 32 1" }
    , { "int16", "OpTypeInt 16 1" }
    , { "uint", "OpTypeInt 32 0" }
    , { "uint16", "OpTypeInt 16 0" }
    , { "readWriteTexture1D",           "OpTypeImage %d 1D 0 0 0 2 %s %s" }
    , { "readWriteTexture2D",           "OpTypeImage %d 2D 0 0 0 2 %s %s" }
    , { "readWriteTexture2DMS",         "OpTypeImage %d 2D 0 0 1 2 %s %s" }
    , { "readWriteTexture3D",           "OpTypeImage %d 3D 0 0 0 2 %s %s" }
    , { "readWriteTextureCube",         "OpTypeImage %d Cube 0 0 0 2 %s %s" }
    , { "readWriteTexture1DArray",      "OpTypeImage %d 1D 0 1 0 2 %s %s" }
    , { "readWriteTexture2DArray",      "OpTypeImage %d 2D 0 1 0 2 %s %s" }
    , { "readWriteTexture2DMSArray",    "OpTypeImage %d 2D 0 1 1 2 %s %s" }
    , { "readWriteTexture3DArray",      "OpTypeImage %d 3D 0 1 0 2 %s %s" }
    , { "readWriteTextureCubeArray",    "OpTypeImage %d Cube 0 1 0 2 %s %s" }
    , { "texture1D",                    "OpTypeImage %d 1D 0 0 0 0 %s" }
    , { "texture2D",                    "OpTypeImage %d 2D 0 0 0 0 %s" }
    , { "texture2DMS",                  "OpTypeImage %d 2D 0 0 1 0 %s" }
    , { "texture3D",                    "OpTypeImage %d 3D 0 0 0 0 %s" }
    , { "textureCube",                  "OpTypeImage %d Cube 0 0 0 0 %s" }
    , { "texture1DArray",               "OpTypeImage %d 1D 0 1 0 0 %s" }
    , { "texture2DArray",               "OpTypeImage %d 2D 0 1 0 0 %s" }
    , { "texture2DMSArray",             "OpTypeImage %d 2D 0 1 1 0 %s" }
    , { "texture3DArray",               "OpTypeImage %d 3D 0 1 0 0 %s" }
    , { "textureCubeArray",             "OpTypeImage %d Cube 0 1 0 0 %s" }
    , { "sampledTexture1D",             "OpTypeImage %d 1D 0 0 0 1 %s" }
    , { "sampledTexture2D",             "OpTypeImage %d 2D 0 0 0 1 %s" }
    , { "sampledTexture2DMS",           "OpTypeImage %d 2D 0 0 1 1 %s" }
    , { "sampledTexture3D",             "OpTypeImage %d 3D 0 0 0 1 %s" }
    , { "sampledTextureCube",           "OpTypeImage %d Cube 0 0 0 1 %s" }
    , { "sampledTexture1DArray",        "OpTypeImage %d 1D 0 1 0 1 %s" }
    , { "sampledTexture2DArray",        "OpTypeImage %d 2D 0 1 0 1 %s" }
    , { "sampledTexture2DMSArray",      "OpTypeImage %d 2D 0 1 1 1 %s" }
    , { "sampledTexture3DArray",        "OpTypeImage %d 3D 0 1 0 1 %s" }
    , { "sampledTextureCubeArray",      "OpTypeImage %d Cube 0 1 0 1 %s" }
    , { "inputAttachment",              "OpTypeImage %d SubpassData 0 0 0 2 %s" }
    , { "inputAttachmentMS",            "OpTypeImage %d SubpassData 0 0 1 2 %s" }
};

std::map<Variable::ImageFormat, std::string> imageFormatToSpirvType =
{

};

uint32_t GenerateExpressionSPIRV(Compiler* compiler, SPIRVGenerator* generator, Expression* expr, std::string& outCode);

//------------------------------------------------------------------------------
/**
*/
uint32_t
GenerateScalarTypeSPIRV(Compiler* compiler, SPIRVGenerator* generator, Type::FullType type, Type* typeSymbol)
{
    std::string baseType = Type::CodeToString(typeSymbol->baseType);
    std::string spirvType = typeToSpirvType[baseType];

    uint32_t name = generator->LookupIntermediate(baseType, spirvType);
    if (typeSymbol->IsVector())
    {
        name = generator->LookupIntermediate(type.name, Format("OpTypeVector %%%d %d", name, typeSymbol->columnSize));
    }
    else if (typeSymbol->IsMatrix())
    {
        baseType = Format("%s%d", baseType.c_str(), typeSymbol->columnSize);
        name = generator->LookupIntermediate(baseType, Format("OpTypeVector %%%d %d", name, typeSymbol->columnSize));
        baseType = Format("%s%dx%d", baseType.c_str(), typeSymbol->columnSize, typeSymbol->rowSize);
        name = generator->LookupIntermediate(baseType, Format("OpTypeMatrix %%%d %d", name, typeSymbol->rowSize));
    }
    return name;
}


//------------------------------------------------------------------------------
/**
*/
uint32_t
GenerateCallExpressionSPIRV(Compiler* compiler, SPIRVGenerator* generator, Expression* expr, std::string& outCode)
{
    CallExpression* callExpression = static_cast<CallExpression*>(expr);
    return 0xFFFFFFFF;
}

//------------------------------------------------------------------------------
/**
*/
uint32_t
GenerateInitializerExpressionSPIRV(Compiler* compiler, SPIRVGenerator* generator, Expression* expr, std::string& outCode)
{
    uint32_t name = 0xFFFFFFFF;
    InitializerExpression* initExpression = static_cast<InitializerExpression*>(expr);

    std::string values;
    
    for (Expression* expr : initExpression->values)
    {
        name = GenerateExpressionSPIRV(compiler, generator, expr, values);
    }
    Type::FullType type;
    initExpression->values[0]->EvalType(type);
    Symbol* typeSymbol = compiler->GetSymbol(type.name);
    uint32_t typeName = GenerateScalarTypeSPIRV(compiler, generator, type, static_cast<Type*>(typeSymbol));
    uint32_t uintName = generator->LookupIntermediate(Format("uint"), Format("OpTypeInt 32 0"));
    uint32_t sizeName = generator->LookupIntermediate(Format("%du", initExpression->values.size()), Format("OpConstant %%%d %d", uintName, initExpression->values.size()));
    typeName = generator->LookupIntermediate(Format("{%s}_array[%d]", type.name.c_str(), initExpression->values.size()), Format("OpTypeArray %%%d %%%d", typeName, sizeName));
    std::string initializer = Format("{%s}_init", values.c_str());
    name = generator->LookupIntermediate(initializer, Format("OpConstantComposite %%%d %s", typeName, values.c_str()));
    outCode.append(Format("%%%d ", name));
    return name;
}

//------------------------------------------------------------------------------
/**
*/
uint32_t
GenerateAccessExpressionSPIRV(Compiler* compiler, SPIRVGenerator* generator, Expression* expr, std::string& outCode)
{
    AccessExpression* accessExpression = static_cast<AccessExpression*>(expr);
    return 0xFFFFFFFF;
}

//------------------------------------------------------------------------------
/**
*/
uint32_t
GenerateCommaExpressionSPIRV(Compiler* compiler, SPIRVGenerator* generator, Expression* expr, std::string& outCode)
{
    CommaExpression* commaExpression = static_cast<CommaExpression*>(expr);
    return 0xFFFFFFFF;
}

//------------------------------------------------------------------------------
/**
*/
uint32_t
GenerateExpressionSPIRV(Compiler* compiler, SPIRVGenerator* generator, Expression* expr, std::string& outCode)
{
    switch (expr->symbolType)
    {
        // call expression might have to change constructor types
        case Symbol::CallExpressionType:
            return GenerateCallExpressionSPIRV(compiler, generator, expr, outCode);
            break;
        case Symbol::InitializerExpressionType:
            return GenerateInitializerExpressionSPIRV(compiler, generator, expr, outCode);
            break;
        case Symbol::AccessExpressionType:
            return GenerateAccessExpressionSPIRV(compiler, generator, expr, outCode);
            break;
        case Symbol::CommaExpressionType:
            return GenerateCommaExpressionSPIRV(compiler, generator, expr, outCode);
        case Symbol::IntExpressionType:
        {
            int value;
            IntExpression* intExpr = static_cast<IntExpression*>(expr);
            intExpr->EvalInt(value);
            uint32_t name = generator->LookupIntermediate("int", Format("OpTypeInt 32 1"));
            name = generator->LookupIntermediate(Format("%ds", value), Format("OpConstant %%%d %d", name, value));
            outCode.append(Format("%%%d ", name));
            return 0xFFFFFFFF;
        }
        case Symbol::UIntExpressionType:
        {
            uint32_t value;
            UIntExpression* uintExpr = static_cast<UIntExpression*>(expr);
            uintExpr->EvalUInt(value);
            uint32_t name = generator->LookupIntermediate("uint", Format("OpTypeInt 32 0"));
            name = generator->LookupIntermediate(Format("%du", value), Format("OpConstant %%%d %d", name, value));
            outCode.append(Format("%%%d ", name));
            return 0xFFFFFFFF;
        }
        case Symbol::FloatExpressionType:
        {
            float value;
            FloatExpression* floatExpr = static_cast<FloatExpression*>(expr);
            floatExpr->EvalFloat(value);
            uint32_t name = generator->LookupIntermediate("float", Format("OpTypeFloat 32"));
            name = generator->LookupIntermediate(Format("%ff", value), Format("OpConstant %%%d %f", name, value));
            outCode.append(Format("%%%d ", name));
            return 0xFFFFFFFF;
        }
        case Symbol::BoolExpressionType:
        {
            bool value;
            BoolExpression* boolExpr = static_cast<BoolExpression*>(expr);
            boolExpr->EvalBool(value);
            uint32_t name = generator->LookupIntermediate("bool", Format("OpTypeBool"));
            name = generator->LookupIntermediate(value ? "true" : "false", value ? "OpConstantTrue" : "OpConstantFalse");
            outCode.append(Format("%%%d ", name));
            return 0xFFFFFFFF;
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
SPIRVGenerator::GenerateFunction(Compiler* compiler, Program* program, Symbol* symbol, std::string& outCode)
{
}

//------------------------------------------------------------------------------
/**
*/
void 
SPIRVGenerator::GenerateStructure(Compiler* compiler, Program* program, Symbol* symbol, std::string& outCode)
{
    Structure* struc = static_cast<Structure*>(symbol);
    Structure::__Resolved* strucResolved = static_cast<Structure::__Resolved*>(struc->resolved);

    uint32_t numVariables = 0;
    for (Symbol* sym : struc->symbols)
    {
        if (sym->symbolType == Symbol::SymbolType::VariableType)
            numVariables++;
    }
    uint32_t name = 0xFFFFFFFF;
    uint32_t* varNames = (uint32_t*)alloca(sizeof(uint32_t) * numVariables);
    std::string str = "OpTypeStruct ";
    for (Symbol* sym : struc->symbols)
    {
        if (sym->symbolType == Symbol::SymbolType::VariableType)
        {
            Variable* var = static_cast<Variable*>(sym);
            Variable::__Resolved* varResolved = static_cast<Variable::__Resolved*>(var->resolved);
            this->GenerateVariable(compiler, program, var, outCode, false, false);
            uint32_t varName = this->LookupIntermediate(varResolved->name, "");
            str.append(Format("%%%d", varName));
        }
    }
    this->LookupIntermediate(struc->name, str);
}

//------------------------------------------------------------------------------
/**
*/
void 
SPIRVGenerator::GenerateVariable(Compiler* compiler, Program* program, Symbol* symbol, std::string& outCode, bool isShaderArgument, bool isGlobal)
{
    Variable* var = static_cast<Variable*>(symbol);
    Variable::__Resolved* varResolved = static_cast<Variable::__Resolved*>(var->resolved);
    uint32_t name = 0xFFFFFFFF;
    if (varResolved->typeSymbol->category == Type::ScalarCategory)
    {
        name = GenerateScalarTypeSPIRV(compiler, this, varResolved->type, varResolved->typeSymbol);
    }
    else if (varResolved->typeSymbol->category == Type::ReadWriteTextureCategory)
    {
        std::string spirvType = typeToSpirvType[varResolved->type.name];
        uint32_t floatName = this->LookupIntermediate("float", "float");
        std::string access;
        if (varResolved->accessBits.flags.readAccess && varResolved->accessBits.flags.writeAccess)
            access = "ReadWrite";
        else if (varResolved->accessBits.flags.readAccess)
            access = "ReadOnly";
        else if (varResolved->accessBits.flags.writeAccess)
            access = "WriteOnly";
        spirvType = Format(spirvType.c_str(), floatName, imageFormatToSpirvType[varResolved->imageFormat].c_str(), access.c_str());
        name = this->LookupIntermediate(spirvType, spirvType);
        this->symbolLookup[varResolved->name] = name;
    }
    else if (varResolved->typeSymbol->category == Type::TextureCategory)
    {
        std::string spirvType = typeToSpirvType[varResolved->type.name];
        uint32_t floatName = this->LookupIntermediate("float", "float");
        spirvType = Format(spirvType.c_str(), floatName, "Rgba32f");
        name = this->LookupIntermediate(spirvType, spirvType);
        name = this->LookupIntermediate(varResolved->name, Format("OpTypeSampledImage %%%d", name));
        this->symbolLookup[varResolved->type.name] = name;
    }
    else if (varResolved->typeSymbol->category == Type::SamplerCategory)
    {
        name = this->LookupIntermediate(varResolved->type.name, "OpTypeSampler");
    }
    else if (varResolved->typeSymbol->category == Type::PixelCacheCategory)
    {
        std::string spirvType = typeToSpirvType[varResolved->type.name];
        uint32_t floatName = this->LookupIntermediate("float", "float");
        spirvType = Format(spirvType.c_str(), floatName, "Rgba32f");
        name = this->LookupIntermediate(varResolved->name, spirvType);
    }
    else
    {
        // Just lookup struct as it has to have been declared prior
        name = this->LookupIntermediate(varResolved->type.name, "");
    }

    // Save type name
    uint32_t typeName = name;
    std::string type = varResolved->type.name;

    // Generate array or pointer
    for (int i = 0; i < varResolved->type.modifierValues.size(); i++)
    {
        if (varResolved->type.modifiers[i] == Type::FullType::Modifier::ArrayLevel)
        {
            // First add constant for array value
            uint32_t uintName = this->LookupIntermediate("uint", "OpTypeInt 32 0");
            uint32_t value = varResolved->type.modifierValues[i];
            name = this->LookupIntermediate(Format("%du", value), Format("OpConstant %%%d %d", uintName, value));
            type = Format("{%s}_array[%d]", type.c_str(), value);
            typeName = this->LookupIntermediate(type, Format("OpTypeArray %%%d %%%d", typeName, name));
        }
    }
    name = typeName;

    uint32_t initializer = 0xFFFFFFFF;
    if (varResolved->value != nullptr)
    {
        // Setup initializer
        initializer = GenerateExpressionSPIRV(compiler, this, varResolved->value, outCode);
    }

    if (isGlobal)
    {
        if (varResolved->usageBits.flags.isConst)
        {
            type = Format("{%s}_typeptr", type.c_str());
            name = this->LookupIntermediate(type, Format("OpTypePointer UniformConstant %%%d", name));
            name = this->LookupIntermediate(varResolved->name, Format("OpVariable %%%d UniformConstant %%%d", name, initializer));
        }
        else
        {
            type = Format("{%s}_typeptr", type.c_str());
            name = this->LookupIntermediate(type, Format("OpTypePointer Uniform %%%d", name));
            name = this->LookupIntermediate(varResolved->name, Format("OpVariable %%%d Uniform", name));
        }
    }
    else
    {
        type = Format("{%s}_typeptr", type.c_str());
        name = this->LookupIntermediate(type, Format("OpTypePointer Function %%%d", name));
        name = this->LookupIntermediate(varResolved->name, Format("OpVariable %%%d Function", name));
    }
}

//------------------------------------------------------------------------------
/**
*/
uint32_t 
SPIRVGenerator::LookupIntermediate(std::string value, std::string declare)
{
    uint32_t ret = 0xFFFFFFFF;
    auto it = this->symbolLookup.find(value);
    if (it != this->symbolLookup.end())
    {
        ret = it->second;
    }
    else
    {
        ret = this->symbolCounter;
        this->symbolLookup[value] = ret;
        this->declarations.append(Format("%%%d = %s\n", ret, declare.c_str()));
        this->symbolCounter++;
    }
    return ret;
}

} // namespace AnyFX
