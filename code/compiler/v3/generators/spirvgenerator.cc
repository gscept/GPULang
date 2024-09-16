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
#include "ast/expressions/binaryexpression.h"
#include "ast/expressions/symbolexpression.h"
#include "ast/expressions/initializerexpression.h"
#include "ast/expressions/arrayindexexpression.h"
#include "ast/expressions/callexpression.h"
#include "ast/expressions/accessexpression.h"
#include "ast/expressions/commaexpression.h"
#include "ast/expressions/ternaryexpression.h"
#include "ast/expressions/unaryexpression.h"

#include "ast/statements/breakstatement.h"
#include "ast/statements/continuestatement.h"
#include "ast/statements/expressionstatement.h"
#include "ast/statements/forstatement.h"
#include "ast/statements/ifstatement.h"
#include "ast/statements/returnstatement.h"
#include "ast/statements/scopestatement.h"
#include "ast/statements/switchstatement.h"
#include "ast/statements/whilestatement.h"

#include "ast/types/builtins.h"
#include "ast/intrinsics.h"

#include "spirv-tools/libspirv.h"

#include <array>

namespace GPULang
{

uint32_t GenerateVariableSPIRV(Compiler* compiler, SPIRVGenerator* generator, Symbol* symbol, bool isShaderArgument, bool isGlobal);
uint32_t GenerateExpressionSPIRV(Compiler* compiler, SPIRVGenerator* generator, Expression* expr);
void GenerateStatementSPIRV(Compiler* compiler, SPIRVGenerator* generator, Statement* stat);

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
    , { "readWriteTexture1D", "OpTypeImage %%%d 1D 0 0 0 2 %s %s" }
    , { "readWriteTexture2D", "OpTypeImage %%%d 2D 0 0 0 2 %s %s" }
    , { "readWriteTexture2DMS", "OpTypeImage %%%d 2D 0 0 1 2 %s %s" }
    , { "readWriteTexture3D", "OpTypeImage %%%d 3D 0 0 0 2 %s %s" }
    , { "readWriteTextureCube", "OpTypeImage %%%d Cube 0 0 0 2 %s %s" }
    , { "readWriteTexture1DArray", "OpTypeImage %%%d 1D 0 1 0 2 %s %s" }
    , { "readWriteTexture2DArray", "OpTypeImage %%%d 2D 0 1 0 2 %s %s" }
    , { "readWriteTexture2DMSArray", "OpTypeImage %%%d 2D 0 1 1 2 %s %s" }
    , { "readWriteTexture3DArray", "OpTypeImage %%%d 3D 0 1 0 2 %s %s" }
    , { "readWriteTextureCubeArray", "OpTypeImage %%%d Cube 0 1 0 2 %s %s" }
    , { "texture1D", "OpTypeImage %%%d 1D 0 0 0 0 %s" }
    , { "texture2D", "OpTypeImage %%%d 2D 0 0 0 0 %s" }
    , { "texture2DMS", "OpTypeImage %%%d 2D 0 0 1 0 %s" }
    , { "texture3D", "OpTypeImage %%%d 3D 0 0 0 0 %s" }
    , { "textureCube", "OpTypeImage %%%d Cube 0 0 0 0 %s" }
    , { "texture1DArray", "OpTypeImage %%%d 1D 0 1 0 0 %s" }
    , { "texture2DArray", "OpTypeImage %%%d 2D 0 1 0 0 %s" }
    , { "texture2DMSArray", "OpTypeImage %%%d 2D 0 1 1 0 %s" }
    , { "texture3DArray", "OpTypeImage %%%d 3D 0 1 0 0 %s" }
    , { "textureCubeArray", "OpTypeImage %%%d Cube 0 1 0 0 %s" }
    , { "sampledTexture1D", "OpTypeImage %%%d 1D 0 0 0 1 %s" }
    , { "sampledTexture2D", "OpTypeImage %%%d 2D 0 0 0 1 %s" }
    , { "sampledTexture2DMS", "OpTypeImage %%%d 2D 0 0 1 1 %s" }
    , { "sampledTexture3D", "OpTypeImage %%%d 3D 0 0 0 1 %s" }
    , { "sampledTextureCube", "OpTypeImage %%%d Cube 0 0 0 1 %s" }
    , { "sampledTexture1DArray", "OpTypeImage %%%d 1D 0 1 0 1 %s" }
    , { "sampledTexture2DArray", "OpTypeImage %%%d 2D 0 1 0 1 %s" }
    , { "sampledTexture2DMSArray", "OpTypeImage %%%d 2D 0 1 1 1 %s" }
    , { "sampledTexture3DArray", "OpTypeImage %%%d 3D 0 1 0 1 %s" }
    , { "sampledTextureCubeArray", "OpTypeImage %%%d Cube 0 1 0 1 %s" }
    , { "inputAttachment", "OpTypeImage %%%d SubpassData 0 0 0 2 %s" }
    , { "inputAttachmentMS", "OpTypeImage %%%d SubpassData 0 0 1 2 %s" }
};

//------------------------------------------------------------------------------
/**
*/
uint32_t
GenerateScalarTypeSPIRV(Compiler* compiler, SPIRVGenerator* generator, Type::FullType type, Type* typeSymbol)
{
    std::string baseType = Type::CodeToString(typeSymbol->baseType);
    std::string spirvType = typeToSpirvType[baseType];

    uint32_t name = generator->AddSymbol(baseType, spirvType, true);
    if (typeSymbol->IsVector())
    {
        name = generator->AddSymbol(type.name, Format("OpTypeVector %%%d %d", name, typeSymbol->columnSize), true);
    }
    else if (typeSymbol->IsMatrix())
    {
        generator->AddCapability("matrix", "OpCapability Matrix");
        baseType = Format("%s%d", baseType.c_str(), typeSymbol->columnSize);
        name = generator->AddSymbol(baseType, Format("OpTypeVector %%%d %d", name, typeSymbol->columnSize), true);
        baseType = Format("%s%dx%d", baseType.c_str(), typeSymbol->columnSize, typeSymbol->rowSize);
        name = generator->AddSymbol(baseType, Format("OpTypeMatrix %%%d %d", name, typeSymbol->rowSize), true);
    }

    return name;
}

enum class ConversionTable
{
    IntToFloat,
    IntToUInt,
    UIntToFloat,
    UIntToInt,
    FloatToUInt,
    FloatToInt,
};

std::map<ConversionTable, std::function<uint32_t(Compiler*, SPIRVGenerator*, uint32_t, uint32_t)>> converters =
{
    { ConversionTable::IntToFloat, [](Compiler* c, SPIRVGenerator* g, uint32_t type, uint32_t name) -> uint32_t { return g->AddMappedOp(Format("OpConvertSToF %%%d %%%d", type, name)); } }
    , { ConversionTable::IntToUInt, [](Compiler* c, SPIRVGenerator* g, uint32_t type, uint32_t name) -> uint32_t { return g->AddMappedOp(Format("OpConvertSToU %%%d %%%d", type, name)); } }
    , { ConversionTable::UIntToInt, [](Compiler* c, SPIRVGenerator* g, uint32_t type, uint32_t name) -> uint32_t { return g->AddMappedOp(Format("OpConvertUToS %%%d %%%d", type, name)); } }
    , { ConversionTable::UIntToFloat, [](Compiler* c, SPIRVGenerator* g, uint32_t type, uint32_t name) -> uint32_t { return g->AddMappedOp(Format("OpConvertUToF %%%d %%%d", type, name)); } }
    , { ConversionTable::FloatToUInt, [](Compiler* c, SPIRVGenerator* g, uint32_t type, uint32_t name) -> uint32_t { return g->AddMappedOp(Format("OpConvertFToU %%%d %%%d", type, name)); } }
    , { ConversionTable::FloatToInt, [](Compiler* c, SPIRVGenerator* g, uint32_t type, uint32_t name) -> uint32_t { return g->AddMappedOp(Format("OpConvertFToS %%%d %%%d", type, name)); } }
};

//------------------------------------------------------------------------------
/**
*/
void
GenerateConversionsSPIRV(Compiler* compiler, SPIRVGenerator* generator, ConversionTable conversion, const std::vector<uint32_t>& inValues, const std::vector<uint32_t>& inTypes, std::vector<uint32_t>& outValues)
{
    for (size_t i = 0; i < inValues.size(); i++)
    {
        outValues.push_back(converters[conversion](compiler, generator, inValues[i], inTypes[i]));
    }
}

//------------------------------------------------------------------------------
/**
*/
uint32_t
GenerateConversionSPIRV(Compiler* compiler, SPIRVGenerator* generator, ConversionTable conversion, uint32_t invalue, uint32_t intype)
{
    return converters[conversion](compiler, generator, invalue, intype);
}

//------------------------------------------------------------------------------
/**
*/
uint32_t
GenerateCompositeSPIRV(Compiler* compiler, SPIRVGenerator* generator, uint32_t returnType, const std::vector<uint32_t>& args)
{
    std::string argList = "";
    for (uint32_t arg : args)
    {
        argList.append(Format("%%%d ", arg));
    }
    return generator->AddMappedOp(Format("OpCompositeConstruct %%%d %s", returnType, argList.c_str()));
}

//------------------------------------------------------------------------------
/**
*/
uint32_t
GenerateSplatCompositeSPIRV(Compiler* compiler, SPIRVGenerator* generator, uint32_t returnType, uint32_t num, uint32_t arg)
{
    std::vector<uint32_t> splat(4, arg);
    return GenerateCompositeSPIRV(compiler, generator, returnType, splat);
}

//------------------------------------------------------------------------------
/**
*/
uint32_t
GenerateConvertAndSplatCompositeSPIRV(Compiler* compiler, SPIRVGenerator* generator, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types, ConversionTable conversion)
{
    std::vector<uint32_t> converted;
    GenerateConversionsSPIRV(compiler, generator, conversion, args, types, converted);
    return GenerateSplatCompositeSPIRV(compiler, generator, returnType, 4, converted[0]);
}

//------------------------------------------------------------------------------
/**
*/
uint32_t
GenerateCompositeExtractSPIRV(Compiler* compiler, SPIRVGenerator* generator, uint32_t returnType, uint32_t arg, uint32_t index)
{
    return generator->AddMappedOp(Format("OpCompositeExtract %%%d %%%d %d", returnType, arg, index));
}

//------------------------------------------------------------------------------
/**
*/
uint32_t
GenerateCompositeInsertSPIRV(Compiler* compiler, SPIRVGenerator* generator, uint32_t returnType, uint32_t arg, uint32_t index, uint32_t val)
{
    return generator->AddMappedOp(Format("OpCompositeInsert %%%d %%%d %%%d %d", returnType, val, arg, index));
}

//------------------------------------------------------------------------------
/**
*/
uint32_t
GenerateConvertAndCompositeSPIRV(Compiler* compiler, SPIRVGenerator* generator, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types, ConversionTable conversion)
{
    std::vector<uint32_t> converted;
    GenerateConversionsSPIRV(compiler, generator, conversion, args, types, converted);
    return GenerateCompositeSPIRV(compiler, generator, returnType, converted);
}

//------------------------------------------------------------------------------
/**
*/
void
SPIRVGenerator::SetupIntrinsics()
{
    this->intrinsicMap[&Float4::ctor_XYZW] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
        assert(args.size() == 4);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Float4::ctorSingleValue] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
        assert(args.size() == 1);
        return GenerateSplatCompositeSPIRV(c, g, returnType, 4, args[0]);
    };
    this->intrinsicMap[&Float4::ctor_Float3_W] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
        assert(args.size() == 2);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Float4::ctor_Float2_ZW] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
        assert(args.size() == 3);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Float3::ctor_XYZ] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
        assert(args.size() == 3);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Float3::ctorSingleValue] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
        assert(args.size() == 1);
        return GenerateSplatCompositeSPIRV(c, g, returnType, 3, args[0]);
    };
    this->intrinsicMap[&Float3::ctor_Float2_Z] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
        assert(args.size() == 2);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Float2::ctor_XY] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
        assert(args.size() == 2);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Float2::ctorSingleValue] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
        assert(args.size() == 1);
        return GenerateSplatCompositeSPIRV(c, g, returnType, 2, args[0]);
    };
    this->intrinsicMap[&Float::ctor_UInt] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
        assert(args.size() == 1);
        return GenerateConversionSPIRV(c, g, ConversionTable::UIntToFloat, args[0], types[0]);
    };
    this->intrinsicMap[&Float::ctor_Int] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
        assert(args.size() == 1);
        return GenerateConversionSPIRV(c, g, ConversionTable::IntToFloat, args[0], types[0]);
    };

    // For matrix float constructors, we need to first construct the vectors and then compose the matrix from them
    this->intrinsicMap[&Mat2x2::floatConstructor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
        assert(args.size() == 4);
        Type* vecType = c->GetSymbol<Type>("float2");
        uint32_t vectorType = GenerateScalarTypeSPIRV(c, g, Type::FullType{ "float2" }, vecType);
        uint32_t col0 = GenerateCompositeSPIRV(c, g, vectorType, { args[0], args[2] });
        uint32_t col1 = GenerateCompositeSPIRV(c, g, vectorType, { args[1], args[3] });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1 });
    };
    this->intrinsicMap[&Mat2x2::vectorConstructor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
        assert(args.size() == 2);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Mat2x3::floatConstructor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
        assert(args.size() == 6);
        Type* vecType = c->GetSymbol<Type>("float2");
        uint32_t vectorType = GenerateScalarTypeSPIRV(c, g, Type::FullType{ "float2" }, vecType);
        uint32_t col0 = GenerateCompositeSPIRV(c, g, vectorType, { args[0], args[3] });
        uint32_t col1 = GenerateCompositeSPIRV(c, g, vectorType, { args[1], args[4] });
        uint32_t col3 = GenerateCompositeSPIRV(c, g, vectorType, { args[2], args[5] });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1, col3 });
    };
    this->intrinsicMap[&Mat2x3::vectorConstructor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
        assert(args.size() == 3);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Mat2x4::floatConstructor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
        assert(args.size() == 8);
        Type* vecType = c->GetSymbol<Type>("float2");
        uint32_t vectorType = GenerateScalarTypeSPIRV(c, g, Type::FullType{ "float2" }, vecType);
        uint32_t col0 = GenerateCompositeSPIRV(c, g, vectorType, { args[0], args[4] });
        uint32_t col1 = GenerateCompositeSPIRV(c, g, vectorType, { args[1], args[5] });
        uint32_t col3 = GenerateCompositeSPIRV(c, g, vectorType, { args[2], args[6] });
        uint32_t col4 = GenerateCompositeSPIRV(c, g, vectorType, { args[3], args[7] });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1, col3, col4 });
    };
    this->intrinsicMap[&Mat2x4::vectorConstructor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
        assert(args.size() == 3);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };

    this->intrinsicMap[&Mat3x2::floatConstructor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
        assert(args.size() == 6);
        Type* vecType = c->GetSymbol<Type>("float3");
        uint32_t vectorType = GenerateScalarTypeSPIRV(c, g, Type::FullType{ "float3" }, vecType);
        uint32_t col0 = GenerateCompositeSPIRV(c, g, vectorType, { args[0], args[2], args[4] });
        uint32_t col1 = GenerateCompositeSPIRV(c, g, vectorType, { args[1], args[3], args[5] });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1 });
    };
    this->intrinsicMap[&Mat3x2::vectorConstructor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
        assert(args.size() == 2);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Mat3x3::floatConstructor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
        assert(args.size() == 9);
        Type* vecType = c->GetSymbol<Type>("float3");
        uint32_t vectorType = GenerateScalarTypeSPIRV(c, g, Type::FullType{ "float3" }, vecType);
        uint32_t col0 = GenerateCompositeSPIRV(c, g, vectorType, { args[0], args[3], args[6] });
        uint32_t col1 = GenerateCompositeSPIRV(c, g, vectorType, { args[1], args[4], args[7] });
        uint32_t col2 = GenerateCompositeSPIRV(c, g, vectorType, { args[2], args[5], args[8] });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1, col2 });
    };
    this->intrinsicMap[&Mat3x3::vectorConstructor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
        assert(args.size() == 3);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Mat3x4::floatConstructor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
        assert(args.size() == 12);
        Type* vecType = c->GetSymbol<Type>("float3");
        uint32_t vectorType = GenerateScalarTypeSPIRV(c, g, Type::FullType{ "float3" }, vecType);
        uint32_t col0 = GenerateCompositeSPIRV(c, g, vectorType, { args[0], args[4], args[8] });
        uint32_t col1 = GenerateCompositeSPIRV(c, g, vectorType, { args[1], args[5], args[9] });
        uint32_t col2 = GenerateCompositeSPIRV(c, g, vectorType, { args[2], args[6], args[10] });
        uint32_t col3 = GenerateCompositeSPIRV(c, g, vectorType, { args[3], args[7], args[11] });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1, col2, col3 });
    };
    this->intrinsicMap[&Mat3x4::vectorConstructor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
        assert(args.size() == 4);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Mat4x2::floatConstructor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
        assert(args.size() == 8);
        Type* vecType = c->GetSymbol<Type>("float4");
        uint32_t vectorType = GenerateScalarTypeSPIRV(c, g, Type::FullType{ "float4" }, vecType);
        uint32_t col0 = GenerateCompositeSPIRV(c, g, vectorType, { args[0], args[2], args[4], args[6] });
        uint32_t col1 = GenerateCompositeSPIRV(c, g, vectorType, { args[1], args[3], args[5], args[7] });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1 });
    };
    this->intrinsicMap[&Mat4x2::vectorConstructor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
        assert(args.size() == 2);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Mat4x3::floatConstructor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
        assert(args.size() == 12);
        Type* vecType = c->GetSymbol<Type>("float4");
        uint32_t vectorType = GenerateScalarTypeSPIRV(c, g, Type::FullType{ "float4" }, vecType);
        uint32_t col0 = GenerateCompositeSPIRV(c, g, vectorType, { args[0], args[3], args[6], args[9] });
        uint32_t col1 = GenerateCompositeSPIRV(c, g, vectorType, { args[1], args[4], args[7], args[10] });
        uint32_t col2 = GenerateCompositeSPIRV(c, g, vectorType, { args[2], args[5], args[8], args[11] });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1, col2 });
    };
    this->intrinsicMap[&Mat4x3::vectorConstructor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
        assert(args.size() == 3);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Mat4x4::floatConstructor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
        assert(args.size() == 16);
        Type* vecType = c->GetSymbol<Type>("float4");
        uint32_t vectorType = GenerateScalarTypeSPIRV(c, g, Type::FullType{ "float4" }, vecType);
        uint32_t col0 = GenerateCompositeSPIRV(c, g, vectorType, { args[0], args[4], args[8], args[12] });
        uint32_t col1 = GenerateCompositeSPIRV(c, g, vectorType, { args[1], args[5], args[9], args[13] });
        uint32_t col2 = GenerateCompositeSPIRV(c, g, vectorType, { args[2], args[6], args[10], args[14] });
        uint32_t col3 = GenerateCompositeSPIRV(c, g, vectorType, { args[3], args[7], args[11], args[15] });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1, col2, col3 });
    };
    this->intrinsicMap[&Mat4x3::vectorConstructor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
        assert(args.size() == 4);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };

    std::map<Function*, std::pair<char, bool>> additionFunctions =
    {
        { &Float4::additionOperator, { 'F', false } }
        , { &Float3::additionOperator, { 'F', false } }
        , { &Float2::additionOperator, { 'F', false } }
        , { &Float::additionOperator, { 'F', false } }
        , { &Int4::additionOperator, { 'S', false } }
        , { &Int3::additionOperator, { 'S', false } }
        , { &Int2::additionOperator, { 'S', false } }
        , { &Int::additionOperator, { 'S', false } }
        , { &UInt4::additionOperator, { 'U', false } }
        , { &UInt3::additionOperator, { 'U', false } }
        , { &UInt2::additionOperator, { 'U', false } }
        , { &UInt::additionOperator, { 'U', false } }
        , { &Mat2x2::additionOperator, { 'F', false } }
        , { &Mat2x3::additionOperator, { 'F', false } }
        , { &Mat2x4::additionOperator, { 'F', false } }
        , { &Mat3x2::additionOperator, { 'F', false } }
        , { &Mat3x3::additionOperator, { 'F', false } }
        , { &Mat3x4::additionOperator, { 'F', false } }
        , { &Mat4x4::additionOperator, { 'F', false } }
        , { &Mat4x3::additionOperator, { 'F', false } }
        , { &Mat4x2::additionOperator, { 'F', false } }
        , { &Float4::additionAssignOperator, { 'F', true } }
        , { &Float3::additionAssignOperator, { 'F', true } }
        , { &Float2::additionAssignOperator, { 'F', true } }
        , { &Float::additionAssignOperator, { 'F', true } }
        , { &Int4::additionAssignOperator, { 'S', true } }
        , { &Int3::additionAssignOperator, { 'S', true } }
        , { &Int2::additionAssignOperator, { 'S', true } }
        , { &Int::additionAssignOperator, { 'S', true } }
        , { &UInt4::additionAssignOperator, { 'U', true } }
        , { &UInt3::additionAssignOperator, { 'U', true } }
        , { &UInt2::additionAssignOperator, { 'U', true } }
        , { &UInt::additionAssignOperator, { 'U', true } }
        , { &Mat2x2::additionAssignOperator, { 'F', true } }
        , { &Mat2x3::additionAssignOperator, { 'F', true } }
        , { &Mat2x4::additionAssignOperator, { 'F', true } }
        , { &Mat3x2::additionAssignOperator, { 'F', true } }
        , { &Mat3x3::additionAssignOperator, { 'F', true } }
        , { &Mat3x4::additionAssignOperator, { 'F', true } }
        , { &Mat4x4::additionAssignOperator, { 'F', true } }
        , { &Mat4x3::additionAssignOperator, { 'F', true } }
        , { &Mat4x2::additionAssignOperator, { 'F', true } }
    };
    for (auto fun : additionFunctions)
    {
        this->intrinsicMap[fun.first] = [op = fun.second.first, assign = fun.second.second](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
            assert(args.size() == 2);
            uint32_t ret = g->AddMappedOp(Format("Op%cAdd %%%d %%%d %%%d", op, returnType, args[0], args[1]));
            if (assign)
                g->ReplaceSymbolMapping(args[0], ret);
            return ret;
        };
    }

    std::map<Function*, std::pair<char, bool>> subtractionFunctions =
    {
        { &Float4::subtractionOperator, { 'F', false } }
        , { &Float3::subtractionOperator, { 'F', false } }
        , { &Float2::subtractionOperator, { 'F', false } }
        , { &Float::subtractionOperator, { 'F', false } }
        , { &Int4::subtractionOperator, { 'S', false } }
        , { &Int3::subtractionOperator, { 'S', false } }
        , { &Int2::subtractionOperator, { 'S', false } }
        , { &Int::subtractionOperator, { 'S', false } }
        , { &UInt4::subtractionOperator, { 'U', false } }
        , { &UInt3::subtractionOperator, { 'U', false } }
        , { &UInt2::subtractionOperator, { 'U', false } }
        , { &UInt::subtractionOperator, { 'U', false } }
        , { &Mat2x2::subtractionOperator, { 'F', false } }
        , { &Mat2x3::subtractionOperator, { 'F', false } }
        , { &Mat2x4::subtractionOperator, { 'F', false } }
        , { &Mat3x2::subtractionOperator, { 'F', false } }
        , { &Mat3x3::subtractionOperator, { 'F', false } }
        , { &Mat3x4::subtractionOperator, { 'F', false } }
        , { &Mat4x4::subtractionOperator, { 'F', false } }
        , { &Mat4x3::subtractionOperator, { 'F', false } }
        , { &Mat4x2::subtractionOperator, { 'F', false } }
        , { &Float4::subtractionAssignOperator, { 'F', true } }
        , { &Float3::subtractionAssignOperator, { 'F', true } }
        , { &Float2::subtractionAssignOperator, { 'F', true } }
        , { &Float::subtractionAssignOperator, { 'F', true } }
        , { &Int4::subtractionAssignOperator, { 'S', true } }
        , { &Int3::subtractionAssignOperator, { 'S', true } }
        , { &Int2::subtractionAssignOperator, { 'S', true } }
        , { &Int::subtractionAssignOperator, { 'S', true } }
        , { &UInt4::subtractionAssignOperator, { 'U', true } }
        , { &UInt3::subtractionAssignOperator, { 'U', true } }
        , { &UInt2::subtractionAssignOperator, { 'U', true } }
        , { &UInt::subtractionAssignOperator, { 'U', true } }
        , { &Mat2x2::subtractionAssignOperator, { 'F', true } }
        , { &Mat2x3::subtractionAssignOperator, { 'F', true } }
        , { &Mat2x4::subtractionAssignOperator, { 'F', true } }
        , { &Mat3x2::subtractionAssignOperator, { 'F', true } }
        , { &Mat3x3::subtractionAssignOperator, { 'F', true } }
        , { &Mat3x4::subtractionAssignOperator, { 'F', true } }
        , { &Mat4x4::subtractionAssignOperator, { 'F', true } }
        , { &Mat4x3::subtractionAssignOperator, { 'F', true } }
        , { &Mat4x2::subtractionAssignOperator, { 'F', true } }
    };
    for (auto fun : subtractionFunctions)
    {
        this->intrinsicMap[fun.first] = [op = fun.second.first, assign = fun.second.second](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
            assert(args.size() == 2);
            uint32_t ret = g->AddMappedOp(Format("Op%cSub %%%d %%%d %%%d", op, returnType, args[0], args[1]));
            if (assign)
                g->ReplaceSymbolMapping(args[0], ret);
            return ret;
        };
    }

    std::map<Function*, std::pair<char, bool>> multiplicationFunctions =
    {
        { &Float4::multiplicationOperator, { 'F', false } }
        , { &Float3::multiplicationOperator, { 'F', false } }
        , { &Float2::multiplicationOperator, { 'F', false } }
        , { &Float::multiplicationOperator, { 'F', false } }
        , { &Int4::multiplicationOperator, { 'S', false } }
        , { &Int3::multiplicationOperator, { 'S', false } }
        , { &Int2::multiplicationOperator, { 'S', false } }
        , { &Int::multiplicationOperator, { 'S', false } }
        , { &UInt4::multiplicationOperator, { 'U', false } }
        , { &UInt3::multiplicationOperator, { 'U', false } }
        , { &UInt2::multiplicationOperator, { 'U', false } }
        , { &UInt::multiplicationOperator, { 'U', false } }
        , { &Float4::multiplicationAssignOperator, { 'F', true } }
        , { &Float3::multiplicationAssignOperator, { 'F', true } }
        , { &Float2::multiplicationAssignOperator, { 'F', true } }
        , { &Float::multiplicationAssignOperator, { 'F', true } }
        , { &Int4::multiplicationAssignOperator, { 'S', true } }
        , { &Int3::multiplicationAssignOperator, { 'S', true } }
        , { &Int2::multiplicationAssignOperator, { 'S', true } }
        , { &Int::multiplicationAssignOperator, { 'S', true } }
        , { &UInt4::multiplicationAssignOperator, { 'U', true } }
        , { &UInt3::multiplicationAssignOperator, { 'U', true } }
        , { &UInt2::multiplicationAssignOperator, { 'U', true } }
        , { &UInt::multiplicationAssignOperator, { 'U', true } }
    };
    for (auto fun : multiplicationFunctions)
    {
        this->intrinsicMap[fun.first] = [op = fun.second.first, assign = fun.second.second](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
            assert(args.size() == 2);
            uint32_t ret = g->AddMappedOp(Format("Op%cMul %%%d %%%d %%%d", op, returnType, args[0], args[1]));
            if (assign)
                g->ReplaceSymbolMapping(args[0], ret);
            return ret;
        };
    }

    std::map<Function*, bool> matrixMultiplicationFunctions =
    {
        { &Mat2x2::multiplyOperator, false }
        , { &Mat2x3::multiplyOperator, false }
        , { &Mat2x4::multiplyOperator, false }
        , { &Mat3x2::multiplyOperator, false }
        , { &Mat3x3::multiplyOperator, false }
        , { &Mat3x4::multiplyOperator, false }
        , { &Mat4x4::multiplyOperator, false }
        , { &Mat4x3::multiplyOperator, false }
        , { &Mat4x2::multiplyOperator, false }
        , { &Mat2x2::multiplyAssignOperator, true }
        , { &Mat2x3::multiplyAssignOperator, true }
        , { &Mat2x4::multiplyAssignOperator, true }
        , { &Mat3x2::multiplyAssignOperator, true }
        , { &Mat3x3::multiplyAssignOperator, true }
        , { &Mat3x4::multiplyAssignOperator, true }
        , { &Mat4x4::multiplyAssignOperator, true }
        , { &Mat4x3::multiplyAssignOperator, true }
        , { &Mat4x2::multiplyAssignOperator, true }
    };
    for (auto fun : matrixMultiplicationFunctions)
    {
        this->intrinsicMap[fun.first] = [assign = fun.second](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
            assert(args.size() == 2);
            uint32_t ret = g->AddMappedOp(Format("OpMatrixTimesMatrix %%%d %%%d %%%d", returnType, args[0], args[1]));
            if (assign)
                g->ReplaceSymbolMapping(args[0], ret);
            return ret;
        };
    }

    std::vector<Function*> matrixVectorTransformFunctions =
    {
        &Mat2x2::vectorTransformOperator
        , &Mat2x3::vectorTransformOperator
        , &Mat2x4::vectorTransformOperator
        , &Mat3x2::vectorTransformOperator
        , &Mat3x3::vectorTransformOperator
        , &Mat3x4::vectorTransformOperator
        , &Mat4x2::vectorTransformOperator
        , &Mat4x3::vectorTransformOperator
        , &Mat4x4::vectorTransformOperator
    };
    for (auto fun : matrixVectorTransformFunctions)
    {
        this->intrinsicMap[fun] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
            assert(args.size() == 2);
            return g->AddMappedOp(Format("OpMatrixTimesVector %%%d %%%d %%%d", returnType, args[0], args[1]));
        };
    }

    std::map<Function*, std::pair<char, bool>> divisionFunctions =
    {
        { &Float4::divisionOperator, { 'F', false } }
        , { &Float3::divisionOperator, { 'F', false } }
        , { &Float2::divisionOperator, { 'F', false } }
        , { &Float::divisionOperator, { 'F', false } }
        , { &Int4::divisionOperator, { 'S', false } }
        , { &Int3::divisionOperator, { 'S', false } }
        , { &Int2::divisionOperator, { 'S', false } }
        , { &Int::divisionOperator, { 'S', false } }
        , { &UInt4::divisionOperator, { 'U', false } }
        , { &UInt3::divisionOperator, { 'U', false } }
        , { &UInt2::divisionOperator, { 'U', false } }
        , { &UInt::divisionOperator, { 'U', false } }
        , { &Float4::divisionAssignOperator, { 'F', true } }
        , { &Float3::divisionAssignOperator, { 'F', true } }
        , { &Float2::divisionAssignOperator, { 'F', true } }
        , { &Float::divisionAssignOperator, { 'F', true } }
        , { &Int4::divisionAssignOperator, { 'S', true } }
        , { &Int3::divisionAssignOperator, { 'S', true } }
        , { &Int2::divisionAssignOperator, { 'S', true } }
        , { &Int::divisionAssignOperator, { 'S', true } }
        , { &UInt4::divisionAssignOperator, { 'U', true } }
        , { &UInt3::divisionAssignOperator, { 'U', true } }
        , { &UInt2::divisionAssignOperator, { 'U', true } }
        , { &UInt::divisionAssignOperator, { 'U', true } }
    };
    for (auto fun : divisionFunctions)
    {
        this->intrinsicMap[fun.first] = [op = fun.second.first, assign = fun.second.second](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
            assert(args.size() == 2);
            uint32_t ret = g->AddMappedOp(Format("Op%cDiv %%%d %%%d %%%d", op, returnType, args[0], args[1]));
            if (assign)
                g->ReplaceSymbolMapping(args[0], ret);
            return ret;
        };
    }

    std::map<Function*, char> modFunctions =
    {
        { &Float4::modOperator, 'F' }
        , { &Float3::modOperator, 'F' }
        , { &Float2::modOperator, 'F' }
        , { &Float::modOperator, 'F' }
        , { &Int4::modOperator, 'S' }
        , { &Int3::modOperator, 'S' }
        , { &Int2::modOperator, 'S' }
        , { &Int::modOperator, 'S' }
        , { &UInt4::modOperator, 'U' }
        , { &UInt3::modOperator, 'U' }
        , { &UInt2::modOperator, 'U' }
        , { &UInt::modOperator, 'U' }
    };
    for (auto fun : modFunctions)
    {
        this->intrinsicMap[fun.first] = [op = fun.second](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
            assert(args.size() == 2);
            return g->AddMappedOp(Format("Op%cMod %%%d %%%d %%%d", op, returnType, args[0], args[1]));
        };
    }

    std::vector<Function*> vectorScaleFunctions =
    {
        &Float4::scaleOperator
        , &Float3::scaleOperator
        , &Float2::scaleOperator
    };
    for (auto fun : vectorScaleFunctions)
    {
        this->intrinsicMap[fun] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
            assert(args.size() == 2);
            return g->AddMappedOp(Format("OpVectorTimesScalar %%%d %%%d %%%d", returnType, args[0], args[1]));
        };
    }

    std::map<Function*, std::pair<char, uint32_t>> intVectorScaleFunctions =
    {
        { &Int4::scaleOperator, { 'S', 4 } }
        , { &Int3::scaleOperator, { 'S', 3 } }
        , { &Int2::scaleOperator, { 'S', 2 } }
        , { &UInt4::scaleOperator, { 'U', 4 } }
        , { &UInt3::scaleOperator, { 'U', 3 } }
        , { &UInt2::scaleOperator, { 'U', 2 } }
    };
    for (auto fun : intVectorScaleFunctions)
    {
        this->intrinsicMap[fun.first] = [op = fun.second.first, num = fun.second.second](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
            assert(args.size() == 2);
            uint32_t splat = GenerateSplatCompositeSPIRV(c, g, returnType, num, args[1]);
            return g->AddMappedOp(Format("Op%cMul %%%d %%%d %%%d", op, returnType, args[0], splat));
        };
    }

    std::vector<Function*> vectorMatrixMulFunctions =
    {
        &Float4::matrix44Mul
        , &Float4::matrix43Mul
        , &Float4::matrix42Mul
        , &Float3::matrix34Mul
        , &Float3::matrix33Mul
        , &Float3::matrix32Mul
        , &Float2::matrix24Mul
        , &Float2::matrix23Mul
        , &Float2::matrix22Mul
    };
    for (auto fun : vectorMatrixMulFunctions)
    {
        this->intrinsicMap[fun] = [fun](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
            assert(args.size() == 2);
            return g->AddMappedOp(Format("OpVectorTimesMatrix %%%d %%%d %%%d", types[0], args[0], args[1]), fun->name);
        };
    }

    std::vector<Function*> arrayAccessFunctions =
    {
        &Float4::elementAccessOperatorInt,
        &Float4::elementAccessOperatorUInt,
        &Float3::elementAccessOperatorInt,
        &Float3::elementAccessOperatorUInt,
        &Float2::elementAccessOperatorInt,
        &Float2::elementAccessOperatorUInt,
        &Int4::elementAccessOperatorInt,
        &Int4::elementAccessOperatorUInt,
        &Int3::elementAccessOperatorInt,
        &Int3::elementAccessOperatorUInt,
        &Int2::elementAccessOperatorInt,
        &Int2::elementAccessOperatorUInt,
        &UInt4::elementAccessOperatorInt,
        &UInt4::elementAccessOperatorUInt,
        &UInt3::elementAccessOperatorInt,
        &UInt3::elementAccessOperatorUInt,
        &UInt2::elementAccessOperatorInt,
        &UInt2::elementAccessOperatorUInt,
    };
    for (auto fun : arrayAccessFunctions)
    {
        this->intrinsicMap[fun] = [fun](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<uint32_t>& args, const std::vector<uint32_t>& types) -> uint32_t {
            assert(args.size() == 2);
            return g->AddMappedOp(Format("OpAccessChain %%%d %%%d %%%d", returnType, args[0], args[1]), fun->name);
        };
    }
}

//------------------------------------------------------------------------------
/**
*/
uint32_t
GenerateLocalVariableSPIRV(Compiler* compiler, SPIRVGenerator* generator, Type::FullType type, uint32_t init = 0xFFFFFFFF)
{
    uint32_t typeName = generator->GetSymbol(type.name);
    uint32_t ptrTypeName = generator->AddSymbol(Format("ptr(%s)", type.name.c_str()), Format("OpTypePointer Function %%%d", typeName), true);
    uint32_t name = generator->AddMappedOp(Format("OpVariable %%%d Function", ptrTypeName), Format("Intermediate '%s'", type.name.c_str()));
    if (init != 0xFFFFFFFF)
        generator->AddOp(Format("OpStore %%%d %%%d", name, init));
    return name;
}

//------------------------------------------------------------------------------
/**
*/
void
GenerateFunctionSPIRV(Compiler* compiler, SPIRVGenerator* generator, Symbol* symbol)
{
    Function* func = static_cast<Function*>(symbol);
    Function::__Resolved* funcResolved = static_cast<Function::__Resolved*>(func->resolved);

    if (funcResolved->isPrototype)
        return;
    uint32_t returnName = GenerateScalarTypeSPIRV(compiler, generator, func->returnType, static_cast<Type*>(funcResolved->returnTypeSymbol));
    std::string typeArgs = "";
    for (auto& param : func->parameters)
    {
        Variable::__Resolved* paramResolved = static_cast<Variable::__Resolved*>(param->resolved);
        uint32_t typeName = GenerateScalarTypeSPIRV(compiler, generator, paramResolved->type, paramResolved->typeSymbol);
        typeArgs.append(Format("%%%d", typeName));
    }
    uint32_t functionType = generator->AddSymbol(Format("type(%s)", funcResolved->name.c_str()), Format("OpTypeFunction %%%d %s", returnName, typeArgs.c_str()), true);

    // TODO: Add inline/const/functional
    generator->AddSymbol(funcResolved->name, Format("OpFunction %%%d None %%%d", returnName, functionType));
    for (auto& param : func->parameters)
    {
        Variable::__Resolved* paramResolved = static_cast<Variable::__Resolved*>(param->resolved);
        generator->AddSymbol(param->name, Format("OpFunctionParameter %%%d", GenerateScalarTypeSPIRV(compiler, generator, paramResolved->type, paramResolved->typeSymbol)));
    }

    GenerateStatementSPIRV(compiler, generator, func->ast);

    uint32_t returnValue = 0xFFFFFFFF;
    if (returnValue != 0xFFFFFFFF)
        generator->AddOp(Format("OpReturnValue %%%d", returnValue));
    else
        generator->AddOp("OpReturn");

    generator->AddOp("OpFunctionEnd");
}

//------------------------------------------------------------------------------
/**
*/
void
GenerateStructureSPIRV(Compiler* compiler, SPIRVGenerator* generator, Symbol* symbol)
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
            uint32_t varType = GenerateScalarTypeSPIRV(compiler, generator, varResolved->type, varResolved->typeSymbol);
            str.append(Format("%%%d ", varType));
        }
    }
    generator->AddSymbol(struc->name, str, true);
}

std::map<Variable::ImageFormat, std::string> imageFormatToSpirvType =
{

};

//------------------------------------------------------------------------------
/**
*/
uint32_t
GenerateVariableSPIRV(Compiler* compiler, SPIRVGenerator* generator, Symbol* symbol, bool isShaderArgument, bool isGlobal)
{
    Variable* var = static_cast<Variable*>(symbol);
    Variable::__Resolved* varResolved = static_cast<Variable::__Resolved*>(var->resolved);
    uint32_t name = 0xFFFFFFFF;
    if (varResolved->typeSymbol->category == Type::ScalarCategory)
    {
        name = GenerateScalarTypeSPIRV(compiler, generator, varResolved->type, varResolved->typeSymbol);
    }
    else if (varResolved->typeSymbol->category == Type::ReadWriteTextureCategory)
    {
        std::string spirvType = typeToSpirvType[varResolved->type.name];
        uint32_t floatName = generator->AddSymbol("float", "OpTypeFloat 32", true);
        std::string access;
        if (varResolved->accessBits.flags.readAccess && varResolved->accessBits.flags.writeAccess)
            access = "ReadWrite";
        else if (varResolved->accessBits.flags.readAccess)
            access = "ReadOnly";
        else if (varResolved->accessBits.flags.writeAccess)
            access = "WriteOnly";
        spirvType = Format(spirvType.c_str(), floatName, imageFormatToSpirvType[varResolved->imageFormat].c_str(), access.c_str());
        name = generator->AddSymbol(Format("%s, %d", varResolved->type.name.c_str(), varResolved->accessBits.bits & 0x3), spirvType, true);
    }
    else if (varResolved->typeSymbol->category == Type::TextureCategory)
    {
        std::string spirvType = typeToSpirvType[varResolved->type.name];
        uint32_t floatName = generator->AddSymbol("float", "OpTypeFloat 32", true);
        name = generator->AddSymbol(varResolved->type.name, Format("OpTypeSampledImage %%%d", name), true);

    }
    else if (varResolved->typeSymbol->category == Type::SamplerCategory)
    {
        name = generator->AddSymbol(varResolved->type.name, "OpTypeSampler", true);
    }
    else if (varResolved->typeSymbol->category == Type::PixelCacheCategory)
    {
        std::string spirvType = typeToSpirvType[varResolved->type.name];
        uint32_t floatName = generator->AddSymbol("float", "OpTypeFloat 32", true);
        spirvType = Format(spirvType.c_str(), floatName, "Rgba32f");
        name = generator->AddSymbol(varResolved->type.name, spirvType, true);
    }
    else
    {
        // Just lookup struct as it has to have been declared prior
        name = generator->GetSymbol(varResolved->type.name);
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
            uint32_t uintName = generator->AddSymbol("uint", "OpTypeInt 32 0", true);
            uint32_t value = varResolved->type.modifierValues[i];
            name = generator->AddSymbol(Format("%du", value), Format("OpConstant %%%d %d", uintName, value), true);
            type = Format("%s[%d]", type.c_str(), value);
            typeName = generator->AddSymbol(type, Format("OpTypeArray %%%d %%%d", typeName, name), true);
        }
    }

    uint32_t initializer = 0xFFFFFFFF;
    if (varResolved->value != nullptr)
    {
        // Setup initializer
        initializer = GenerateExpressionSPIRV(compiler, generator, varResolved->value);
    }

    if (isGlobal)
    {
        if (varResolved->usageBits.flags.isConst)
        {
            type = Format("ptr(%s)", type.c_str());
            uint32_t typePtrName = generator->AddSymbol(type, Format("OpTypePointer UniformConstant %%%d", typeName), true);
            name = generator->AddSymbol(varResolved->name, Format("OpVariable %%%d UniformConstant %%%d", typePtrName, initializer), isGlobal);
        }
        else
        {
            type = Format("ptr(%s)", type.c_str());
            uint32_t typePtrName = generator->AddSymbol(type, Format("OpTypePointer Uniform %%%d", typeName), true);
            name = generator->AddSymbol(varResolved->name, Format("OpVariable %%%d Uniform", typePtrName), isGlobal);
        }
    }
    else
    {
        type = Format("ptr(%s)", type.c_str());
        uint32_t typePtrName = generator->AddSymbol(type, Format("OpTypePointer Function %%%d", typeName), true);

        if (initializer != 0xFFFFFFFF)
        {
            uint32_t initializerValue = generator->AddMappedOp(Format("OpLoad %%%d %%%d", typeName, initializer));
            name = generator->AddSymbol(varResolved->name, Format("OpVariable %%%d Function", typePtrName), isGlobal);
            generator->AddOp(Format("OpStore %%%d %%%d", name, initializerValue));
        }
        else
            name = generator->AddSymbol(varResolved->name, Format("OpVariable %%%d Function", typePtrName), isGlobal);
    }
    return name;
}

//------------------------------------------------------------------------------
/**
*/
uint32_t
GenerateCallExpressionSPIRV(Compiler* compiler, SPIRVGenerator* generator, Expression* expr)
{
    CallExpression* callExpression = static_cast<CallExpression*>(expr);
    CallExpression::__Resolved* resolvedCall = Symbol::Resolved(callExpression);

    Function::__Resolved* resolvedFunction = Symbol::Resolved(resolvedCall->function);
    uint32_t returnTypeName = GenerateScalarTypeSPIRV(compiler, generator, resolvedCall->function->returnType, resolvedFunction->returnTypeSymbol);

    uint32_t funName;
    if (generator->HasSymbol(resolvedFunction->name))
    {
        funName = generator->GetSymbol(resolvedFunction->name);

        // Create arg list from argument expressions
        std::string argList = "";
        for (const auto& param : callExpression->args)
        {
            uint32_t argName = GenerateExpressionSPIRV(compiler, generator, param);
            Type::FullType type;
            param->EvalType(type);
            uint32_t typeName = generator->GetSymbol(type.ToString());
            argList.append(Format("%%%d ", argName));
        }

        // Then call the function
        return generator->AddMappedOp(Format("OpFunctionCall %%%d %%%d %s", returnTypeName, funName, argList.c_str()));
    }
    else
    {
        // If there is no symbol matching this function, then we assume it's an intrinsic function
        std::vector<uint32_t> argTypes;
        std::vector<uint32_t> argNames;

        // First step is to get the type of all parameters, and get their SSA mappings
        for (const auto& param : callExpression->args)
        {
            uint32_t argName = GenerateExpressionSPIRV(compiler, generator, param);
            Type::FullType type;
            param->EvalType(type);
            uint32_t typeName = generator->GetSymbol(type.ToString());
            argNames.push_back(argName);
            argTypes.push_back(typeName);
        }

        // Then we walk through all conversions which should be per parameter, or null if the type matches
        for (size_t i = 0; i < resolvedCall->conversions.size(); i++)
        {
            Function* converter = resolvedCall->conversions[i];
            if (converter != nullptr)
            {
                Function::__Resolved* converterResolved = Symbol::Resolved(converter);
                auto conv = generator->intrinsicMap.find(converter);
                assert(conv != generator->intrinsicMap.end());
                uint32_t returnTypeName = GenerateScalarTypeSPIRV(compiler, generator, converter->returnType, converterResolved->returnTypeSymbol);

                std::vector<uint32_t> types{ argTypes[i] };
                std::vector<uint32_t> names{ argNames[i] };

                // After conversion the type is changed and the SSA is the value of the conversion being done
                argNames[i] = conv->second(compiler, generator, returnTypeName, types, names);
                argTypes[i] = returnTypeName;
            }
        }

        // Function is not declared by user code, so must be intrinsic
        auto it = generator->intrinsicMap.find(resolvedCall->function);
        assert(it != generator->intrinsicMap.end());

        // Lastly, run the intrinsic generation with the converted parameters
        uint32_t val = it->second(compiler, generator, returnTypeName, argNames, argTypes);
        return GenerateLocalVariableSPIRV(compiler, generator, resolvedCall->returnType, val);
    }
}

//------------------------------------------------------------------------------
/**
*/
uint32_t
GenerateArrayIndexExpressionSPIRV(Compiler* compiler, SPIRVGenerator* generator, Expression* expr)
{
    ArrayIndexExpression* arrayIndexExpression = static_cast<ArrayIndexExpression*>(expr);
    ArrayIndexExpression::__Resolved* arrayIndexExpressionResolved = Symbol::Resolved(arrayIndexExpression);
    uint32_t leftSymbol = GenerateExpressionSPIRV(compiler, generator, arrayIndexExpression->left);

    Type::FullType leftType, rightType;
    arrayIndexExpression->left->EvalType(leftType);
    arrayIndexExpression->right->EvalType(rightType);

    uint32_t index;
    bool res = arrayIndexExpression->right->EvalUInt(index);
    assert(res);

    uint32_t returnType = GenerateScalarTypeSPIRV(compiler, generator, arrayIndexExpressionResolved->returnFullType, arrayIndexExpressionResolved->returnType);

    // Evaluate the index which has to be a literal or constant value that evaluates at compile time
    if (leftType.modifiers.empty())
    {
        Type* type = static_cast<Type*>(compiler->GetSymbol(leftType.name));

        if (type->IsVector())
        {
            auto it = type->lookup.find(Format("operator[](%s)", rightType.name.c_str()));
            Function* func = static_cast<Function*>((*it).second);

            /// Get intrinsic
            auto intrin = generator->intrinsicMap.find(func);
            assert(intrin != generator->intrinsicMap.end());

            uint32_t returnPtrType = generator->AddSymbol(Format("ptr(%s)", arrayIndexExpressionResolved->returnFullType.ToString().c_str()), Format("OpTypePointer Function %%%d", returnType), true);
            uint32_t indexType = generator->AddSymbol(Format("uint"), Format("OpTypeInt 32 0"), true);
            uint32_t indexConstant = generator->AddSymbol(Format("%du", index), Format("OpConstant %%%d %d", indexType, index), true);

            /// Generate array access
            return intrin->second(compiler, generator, returnPtrType, { leftSymbol, indexConstant }, {});

            /// %15 = OpAccessChain %12 %6 %14 ;
        }
    }
    else
    {

    }
    return 0xFFFFFFFF;
}

//------------------------------------------------------------------------------
/**
*/
uint32_t
GenerateInitializerExpressionSPIRV(Compiler* compiler, SPIRVGenerator* generator, Expression* expr)
{
    uint32_t name = 0xFFFFFFFF;
    InitializerExpression* initExpression = static_cast<InitializerExpression*>(expr);

    std::vector<uint32_t> composites;
    for (Expression* expr : initExpression->values)
    {
        name = GenerateExpressionSPIRV(compiler, generator, expr);
        composites.push_back(name);
    }
    Type::FullType type;
    initExpression->values[0]->EvalType(type);
    Symbol* typeSymbol = compiler->GetSymbol(type.name);
    uint32_t typeName = GenerateScalarTypeSPIRV(compiler, generator, type, static_cast<Type*>(typeSymbol));
    uint32_t uintName = generator->AddSymbol(Format("uint"), Format("OpTypeInt 32 0"), true);
    uint32_t sizeName = generator->AddSymbol(Format("%du", initExpression->values.size()), Format("OpConstant %%%d %d", uintName, initExpression->values.size()), true);
    typeName = generator->AddSymbol(Format("%s[%d]", type.name.c_str(), initExpression->values.size()), Format("OpTypeArray %%%d %%%d", typeName, sizeName), true);
    std::string initializer = "";
    for (uint32_t component : composites)
    {
        uint32_t loadedComponent = generator->AddMappedOp(Format("OpLoad %%%d %%%d", typeName, component));
        initializer.append(Format("%%%d ", loadedComponent));
    }
    name = generator->AddSymbol(Format("{%s}", initializer.c_str()), Format("OpCompositeConstruct %%%d %s", typeName, initializer.c_str()));
    //outCode.append(Format("%%%d \n", name));
    return name;
}

//------------------------------------------------------------------------------
/**
*/
uint32_t
GenerateBinaryExpressionSPIRV(Compiler* compiler, SPIRVGenerator* generator, Expression* expr)
{
    BinaryExpression* binaryExpression = static_cast<BinaryExpression*>(expr);
    BinaryExpression::__Resolved* binaryExpressionResolved = Symbol::Resolved(binaryExpression);
    Type::FullType leftType, rightType;
    binaryExpression->left->EvalType(leftType);
    binaryExpression->right->EvalType(rightType);

    uint32_t leftTypeName = GenerateScalarTypeSPIRV(compiler, generator, leftType, binaryExpressionResolved->lhsType);
    uint32_t rightTypeName = GenerateScalarTypeSPIRV(compiler, generator, rightType, binaryExpressionResolved->rhsType);
    uint32_t rightValue = GenerateExpressionSPIRV(compiler, generator, binaryExpression->right );

    // If there is a conversion function, generate it first
    if (binaryExpressionResolved->conversionFunction)
    {
        auto it = generator->intrinsicMap.find(binaryExpressionResolved->conversionFunction);
        assert(it != generator->intrinsicMap.end());
        rightValue = it->second(compiler, generator, leftTypeName, { rightValue }, { rightTypeName });
    }
    uint32_t leftValue = GenerateExpressionSPIRV(compiler, generator, binaryExpression->left);

    std::string functionName = Format("operator%s(%s)", FourCCToString(binaryExpression->op).c_str(), leftType.name.c_str());
    Function* fun = compiler->GetSymbol<Function>(functionName);

    // If fun is not a null pointer, there is some conversion needed
    if (fun != nullptr)
    {
        // Get operator 
        auto op = generator->intrinsicMap.find(fun);
        assert(op != generator->intrinsicMap.end());
        rightValue = op->second(compiler, generator, leftTypeName, { leftValue, rightValue }, { leftTypeName, leftTypeName });
    }

    // Assignment is a store operation
    if (binaryExpression->op == '=')
    {
        generator->AddOp(Format("OpStore %%%d %%%d", leftValue, rightValue));
    }
    
    return rightValue;
}

//------------------------------------------------------------------------------
/**
*/
uint32_t
GenerateAccessExpressionSPIRV(Compiler* compiler, SPIRVGenerator* generator, Expression* expr)
{
    AccessExpression* accessExpression = static_cast<AccessExpression*>(expr);
    AccessExpression::__Resolved* accessExpressionResolved = Symbol::Resolved(accessExpression);
    uint32_t lhs = GenerateExpressionSPIRV(compiler, generator, accessExpression->left);

    Type::SwizzleMask swizzle;
    std::string rhsSymbol;
    accessExpression->right->EvalSymbol(rhsSymbol);

    if (Type::StringToSwizzleMask(rhsSymbol, swizzle))
    {
        // If single value, then use an access chain
        if (Type::SwizzleMaskComponents(swizzle) == 1)
        {
            uint32_t uintName = generator->AddSymbol(Format("uint"), Format("OpTypeInt 32 0"), true);
            uint32_t indexName = generator->AddSymbol(Format("%du", swizzle.bits.x), Format("OpConstant %%%d %d", uintName, swizzle.bits.x), true);
            uint32_t retName = GenerateScalarTypeSPIRV(compiler, generator, accessExpressionResolved->rightType, accessExpressionResolved->rhsType);
            uint32_t ptr = generator->AddMappedOp(Format("OpAccessChain %%%d %%%d %%%d", retName, lhs, indexName));
            return ptr;
        }
        else
        {
            uint32_t retType = GenerateScalarTypeSPIRV(compiler, generator, accessExpressionResolved->rightType, accessExpressionResolved->rhsType);
            uint32_t origType = GenerateScalarTypeSPIRV(compiler, generator, accessExpressionResolved->leftType, accessExpressionResolved->lhsType);
            std::string swizzleIndices;
            if (swizzle.bits.x != Type::SwizzleMask::Invalid)
                swizzleIndices.append(Format("%d ", swizzle.bits.x));
            if (swizzle.bits.y != Type::SwizzleMask::Invalid)
                swizzleIndices.append(Format("%d ", swizzle.bits.y));
            if (swizzle.bits.z != Type::SwizzleMask::Invalid)
                swizzleIndices.append(Format("%d ", swizzle.bits.z));
            if (swizzle.bits.w != Type::SwizzleMask::Invalid)
                swizzleIndices.append(Format("%d ", swizzle.bits.w));
            uint32_t vec = generator->AddMappedOp(Format("OpLoad %%%d %%%d", origType, lhs), Format("Intermediate '%s'", accessExpressionResolved->leftType.name.c_str()));
            uint32_t shuffledVec = generator->AddMappedOp(Format("OpVectorShuffle %%%d %%%d %%%d %s", retType, vec, vec, swizzleIndices.c_str()));
            return GenerateLocalVariableSPIRV(compiler, generator, accessExpressionResolved->rightType, shuffledVec);
        }
        // Generate swizzle access
    }
    else
    {
        // Otherwise, find offset of member
        std::string lhsSymbol;
        accessExpression->left->EvalSymbol(lhsSymbol);
        Symbol* rhsSymbol = compiler->GetSymbol(lhsSymbol);

        if (rhsSymbol->symbolType == Symbol::StructureType)
        {

        }
    }
    return 0xFFFFFFFF;
}

//------------------------------------------------------------------------------
/**
*/
uint32_t
GenerateCommaExpressionSPIRV(Compiler* compiler, SPIRVGenerator* generator, Expression* expr)
{
    CommaExpression* commaExpression = static_cast<CommaExpression*>(expr);
    return 0xFFFFFFFF;
}

//------------------------------------------------------------------------------
/**
*/
uint32_t
GenerateTernaryExpressionSPIRV(Compiler* compiler, SPIRVGenerator* generator, Expression* expr)
{
    TernaryExpression* ternaryExpression = static_cast<TernaryExpression*>(expr);
    return 0xFFFFFFFF;
}

//------------------------------------------------------------------------------
/**
*/
uint32_t
GenerateUnaryExpressionSPIRV(Compiler* compiler, SPIRVGenerator* generator, Expression* expr)
{
    UnaryExpression* unaryExpression = static_cast<UnaryExpression*>(expr);
    return 0xFFFFFFFF;
}

//------------------------------------------------------------------------------
/**
*/
uint32_t
GenerateExpressionSPIRV(Compiler* compiler, SPIRVGenerator* generator, Expression* expr)
{
    switch (expr->symbolType)
    {
        case Symbol::FloatExpressionType:
        {
            float value;
            FloatExpression* floatExpr = static_cast<FloatExpression*>(expr);
            floatExpr->EvalFloat(value);
            uint32_t typeName = generator->AddSymbol("float", Format("OpTypeFloat 32"), true);
            uint32_t name = generator->AddSymbol(Format("%ff", value), Format("OpConstant %%%d %f", typeName, value), true);
            return GenerateLocalVariableSPIRV(compiler, generator, Type::FullType{ "float" }, name);
        }
        case Symbol::IntExpressionType:
        {
            int value;
            IntExpression* intExpr = static_cast<IntExpression*>(expr);
            intExpr->EvalInt(value);
            uint32_t typeName = generator->AddSymbol("int", Format("OpTypeInt 32 1"), true);
            uint32_t name = generator->AddSymbol(Format("%ds", value), Format("OpConstant %%%d %d", typeName, value), true);
            return GenerateLocalVariableSPIRV(compiler, generator, Type::FullType{ "int" }, name);
        }
        case Symbol::UIntExpressionType:
        {
            uint32_t value;
            UIntExpression* uintExpr = static_cast<UIntExpression*>(expr);
            uintExpr->EvalUInt(value);
            uint32_t typeName = generator->AddSymbol("uint", Format("OpTypeInt 32 0"), true);
            uint32_t name = generator->AddSymbol(Format("%du", value), Format("OpConstant %%%d %d", typeName, value), true);
            return GenerateLocalVariableSPIRV(compiler, generator, Type::FullType{ "uint" }, name);
        }
        case Symbol::BoolExpressionType:
        {
            bool value;
            BoolExpression* boolExpr = static_cast<BoolExpression*>(expr);
            boolExpr->EvalBool(value);
            uint32_t typeName = generator->AddSymbol("bool", Format("OpTypeBool"), true);
            uint32_t name = generator->AddSymbol(value ? "true" : "false", value ? "OpConstantTrue" : "OpConstantFalse", true);
            return GenerateLocalVariableSPIRV(compiler, generator, Type::FullType{ "bool" }, name);
        }
        case Symbol::BinaryExpressionType:
            return GenerateBinaryExpressionSPIRV(compiler, generator, expr);
            break;
        case Symbol::SymbolExpressionType:
        {
            SymbolExpression* symbolExpression = static_cast<SymbolExpression*>(expr);
            return generator->GetSymbol(symbolExpression->symbol);
        }
        case Symbol::AccessExpressionType:
            return GenerateAccessExpressionSPIRV(compiler, generator, expr);
        case Symbol::InitializerExpressionType:
            return GenerateInitializerExpressionSPIRV(compiler, generator, expr);
        case Symbol::ArrayIndexExpressionType:
            return GenerateArrayIndexExpressionSPIRV(compiler, generator, expr);
        case Symbol::CallExpressionType:
            return GenerateCallExpressionSPIRV(compiler, generator, expr);
        case Symbol::CommaExpressionType:
            return GenerateCommaExpressionSPIRV(compiler, generator, expr);
        case Symbol::TernaryExpressionType:
            return GenerateTernaryExpressionSPIRV(compiler, generator, expr);
        case Symbol::UnaryExpressionType:
            return GenerateUnaryExpressionSPIRV(compiler, generator, expr);
        default:
            printf("[SPIR-V Generator]: Unknown expression '%d'\n", expr->symbolType);
            abort();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
GenerateBreakStatementSPIRV(Compiler* compiler, SPIRVGenerator* generator, BreakStatement* stat)
{

}

//------------------------------------------------------------------------------
/**
*/
void
GenerateContinueStatementSPIRV(Compiler* compiler, SPIRVGenerator* generator, ContinueStatement* stat)
{

}

//------------------------------------------------------------------------------
/**
*/
void
GenerateForStatementSPIRV(Compiler* compiler, SPIRVGenerator* generator, ForStatement* stat)
{

}

//------------------------------------------------------------------------------
/**
*/
void
GenerateIfStatementSPIRV(Compiler* compiler, SPIRVGenerator* generator, IfStatement* stat)
{

}

//------------------------------------------------------------------------------
/**
*/
void
GenerateReturnStatementSPIRV(Compiler* compiler, SPIRVGenerator* generator, ReturnStatement* stat)
{

}

//------------------------------------------------------------------------------
/**
*/
void
GenerateSwitchStatementSPIRV(Compiler* compiler, SPIRVGenerator* generator, SwitchStatement* stat)
{

}

//------------------------------------------------------------------------------
/**
*/
void
GenerateWhileStatementSPIRV(Compiler* compiler, SPIRVGenerator* generator, WhileStatement* stat)
{

}

//------------------------------------------------------------------------------
/**
*/
void
GenerateStatementSPIRV(Compiler* compiler, SPIRVGenerator* generator, Statement* stat)
{
    switch (stat->symbolType)
    {
        case Symbol::BreakStatementType:
            GenerateBreakStatementSPIRV(compiler, generator, static_cast<BreakStatement*>(stat));
            break;
        case Symbol::ContinueStatementType:
            GenerateContinueStatementSPIRV(compiler, generator, static_cast<ContinueStatement*>(stat));
            break;
        case Symbol::ExpressionStatementType:
            GenerateExpressionSPIRV(compiler, generator, static_cast<ExpressionStatement*>(stat)->expr);
            break;
        case Symbol::ForStatementType:
            GenerateForStatementSPIRV(compiler, generator, static_cast<ForStatement*>(stat));
            break;
        case Symbol::IfStatementType:
            GenerateIfStatementSPIRV(compiler, generator, static_cast<IfStatement*>(stat));
            break;
        case Symbol::ReturnStatementType:
            GenerateReturnStatementSPIRV(compiler, generator, static_cast<ReturnStatement*>(stat));
            break;
        case Symbol::ScopeStatementType:
        {
            ScopeStatement* scope = static_cast<ScopeStatement*>(stat);
            generator->PushScope();
            const std::vector<Symbol*>& symbols = scope->symbols;
            for (const auto& symbol : symbols)
            {
                if (symbol->symbolType == GPULang::Symbol::SymbolType::VariableType)
                    GenerateVariableSPIRV(compiler, generator, symbol, false, false);
                else
                {
                    Statement* stat = static_cast<Statement*>(symbol);
                    GenerateStatementSPIRV(compiler, generator, stat);
                }
            }
            generator->PopScope();
            break;
        }
        case Symbol::SwitchStatementType:
            GenerateSwitchStatementSPIRV(compiler, generator, static_cast<SwitchStatement*>(stat));
            break;
        case Symbol::WhileStatementType:
            GenerateWhileStatementSPIRV(compiler, generator, static_cast<WhileStatement*>(stat));
            break;
        default:
            printf("[SPIR-V Generator]: Not handled symbol '%d' encountered\n", stat->symbolType);
            abort();
    }
}

//------------------------------------------------------------------------------
/**
*/
bool 
SPIRVGenerator::Generate(Compiler* compiler, Program* program, const std::vector<Symbol*>& symbols, std::function<void(const std::string&, const std::string&)> writerFunc)
{
    Program::__Resolved* progResolved = static_cast<Program::__Resolved*>(program->resolved);

    // Setup intrinsics
    intrinsicMap.clear();
    SetupIntrinsics();

    // Main scope
    this->PushScope();

    spv_context spvContext = spvContextCreate(SPV_ENV_VULKAN_1_2);

    for (auto it : progResolved->programMappings)
    {
        // for each shader, generate code and use it as a binary output
        if (it.first >= Program::__Resolved::VertexShader && it.first <= Program::__Resolved::RayIntersectionShader)
        {
            this->header.append("; Magic:     0x07230203 (SPIR-V)\n");
            this->header.append("; Version:   0x00010000 (Version: 1.0.0)\n");
            this->header.append("; Generator: 0x00080001 (GPULang; 1)\n");
            this->header.append("OpCapability Shader\n");

            for (Symbol* sym : symbols)
            {
                switch (sym->symbolType)
                {
                    case Symbol::FunctionType:
                        GenerateFunctionSPIRV(compiler, this, sym);
                        break;
                    case Symbol::StructureType:
                        GenerateStructureSPIRV(compiler, this, sym);
                        break;
                    case Symbol::VariableType:
                        GenerateVariableSPIRV(compiler, this, sym, false, true);
                        break;
                }
            }

            // Compose and convert to binary, then validate
            std::string binary = Format("%s /// Declarations\n %s /// Decorations\n %s /// Functions\n %s", this->header.c_str(), this->declarations.c_str(), this->decorations.c_str(), this->functional.c_str());
            spv_binary bin = nullptr;
            spv_diagnostic diag = nullptr;
            spv_result_t res = spvTextToBinary(spvContext, binary.c_str(), binary.size(), &bin, &diag);
            assert(res == SPV_SUCCESS);
            spv_const_binary constBin = (spv_const_binary)&bin;
            res = spvValidate(spvContext, constBin, &diag);
            assert(res == SPV_SUCCESS);

        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
uint32_t 
SPIRVGenerator::AddSymbol(std::string name, std::string declare, bool global)
{
    auto& scope = this->scopeStack.rbegin();
    while (scope != this->scopeStack.rend())
    {
        auto it = scope->symbols.find(name);
        if (it != scope->symbols.end())
        {
            return it->second;
        }
        scope++;
    }

    // If symbol isn't found in scope, create it
    uint32_t ret = this->symbolCounter;
    this->scopeStack.back().symbols[name] = ret;

    std::string decl = Format("%%%d\t=\t%s\t\t\t; %s\n", ret, declare.c_str(), name.c_str());
    if (global)
        this->declarations.append(decl);
    else
        this->functional.append(decl);
    this->symbolCounter++;
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
uint32_t 
SPIRVGenerator::GetSymbol(std::string name)
{
    uint32_t ret = 0xFFFFFFFF;
    auto& it = this->scopeStack.rbegin();
    while (it != this->scopeStack.rend())
    {
        auto& sym = it->symbols.find(name);
        if (sym != it->symbols.end())
        {
            ret = sym->second;
            break;
        }
        it++;
    }
    assert(ret != 0xFFFFFFFF);
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
bool
SPIRVGenerator::HasSymbol(std::string name)
{
    uint32_t ret = 0xFFFFFFFF;
    auto& it = this->scopeStack.rbegin();
    while (it != this->scopeStack.rend())
    {
        auto& sym = it->symbols.find(name);
        if (sym != it->symbols.end())
        {
            ret = sym->second;
            break;
        }
        it++;
    }
    return ret != 0xFFFFFFFF;
}

//------------------------------------------------------------------------------
/**
*/
void 
SPIRVGenerator::AddOp(std::string value, bool global, std::string comment)
{
    if (global)
        this->declarations.append(Format("\t\t%s\t\t\t; %s\n", value.c_str(), comment.c_str()));
    else
        this->functional.append(Format("\t\t%s\t\t\t; %s\n", value.c_str(), comment.c_str()));
}

//------------------------------------------------------------------------------
/**
*/
uint32_t 
SPIRVGenerator::AddMappedOp(std::string name, std::string comment)
{
    uint32_t ret = this->symbolCounter;
    this->functional.append(Format("%%%d\t=\t%s\t\t\t; %s\n", ret, name.c_str(), comment.c_str()));
    this->symbolCounter++;
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
void 
SPIRVGenerator::AddCapability(std::string name, std::string declare)
{
    if (this->capabilities.find(name) == this->capabilities.end())
    {
        this->header.append(Format("%s\n", declare.c_str()));
        this->capabilities.insert(name);
    }
}

//------------------------------------------------------------------------------
/**
*/
uint32_t 
SPIRVGenerator::FindSymbolMapping(std::string value)
{
    uint32_t ret = 0xFFFFFFFF;
    auto& it = this->scopeStack.rbegin();
    while (it != this->scopeStack.rend())
    {
        auto& sym = it->symbols.find(value);
        if (sym != it->symbols.end())
        {
            ret = sym->second;
            break;
        }
        it++;
    }
    assert(ret != 0xFFFFFFFF);
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
void 
SPIRVGenerator::ReplaceSymbolMapping(uint32_t oldMapping, uint32_t newMapping)
{
    auto& it = this->scopeStack.rbegin();
    while (it != this->scopeStack.rend())
    {
        auto& sym = it->symbols.begin();
        while (sym != it->symbols.end())
        {
            if (sym->second == oldMapping)
            {
                sym->second = newMapping;
                return;
            }
        }
        it++;
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
SPIRVGenerator::PushScope()
{
    this->scopeStack.push_back(Scope());
}

//------------------------------------------------------------------------------
/**
*/
void 
SPIRVGenerator::PopScope()
{
    this->scopeStack.pop_back();
}

} // namespace GPULang
