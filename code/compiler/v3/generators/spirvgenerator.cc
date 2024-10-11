//------------------------------------------------------------------------------
//  @file spirvgenerator.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "spirvgenerator.h"
#include "compiler.h"

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

#include "ast/types/BuiltIns.h"
#include "ast/intrinsics.h"

#include "spirv-tools/libspirv.h"
#include "spirv-tools/optimizer.hpp"

#include <array>

namespace GPULang
{


SPIRVResult GenerateVariableSPIRV(Compiler* compiler, SPIRVGenerator* generator, Symbol* symbol, bool isShaderArgument, bool isGlobal);
SPIRVResult GenerateExpressionSPIRV(Compiler* compiler, SPIRVGenerator* generator, Expression* expr);
bool GenerateStatementSPIRV(Compiler* compiler, SPIRVGenerator* generator, Statement* stat);

std::unordered_map<std::string, std::string> typeToSpirvType =
{
    { "void", "OpTypeVoid" }
    , { "f32", "OpTypeFloat 32" }
    , { "f16", "OpTypeFloat 16" }
    , { "b8", "OpTypeBool" }
    , { "i32", "OpTypeInt 32 1" }
    , { "i16", "OpTypeInt 16 1" }
    , { "u32", "OpTypeInt 32 0" }
    , { "u16", "OpTypeInt 16 0" }
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
    , { "pixelCache", "OpTypeImage %%%d SubpassData 0 0 0 2 Unknown ReadOnly" }
    , { "pixelCacheMS", "OpTypeImage %%%d SubpassData 0 0 1 2 Unknown ReadOnly" }
    , { "sampler", "OpTypeSampler" }
};

std::unordered_map<ImageFormat, std::string> imageFormatToSpirvType =
{
    { ImageFormat::RGBA16, "Rgba16" }
    , { ImageFormat::RGB10_A2, "Rgb10A2" }
    , { ImageFormat::RGBA8, "Rgba8" }
    , { ImageFormat::RG16, "Rg16" }
    , { ImageFormat::RG8, "Rg8" }
    , { ImageFormat::R16, "R16" }
    , { ImageFormat::R8, "R8" }
    , { ImageFormat::RGBA16_SNORM, "Rgba16Snorm" }
    , { ImageFormat::RGBA8_SNORM, "Rgba8Snorm" }
    , { ImageFormat::RG16_SNORM, "Rg16Snorm" }
    , { ImageFormat::RG8_SNORM, "Rg8Snorm" }
    , { ImageFormat::R16_SNORM, "R16Snorm" }
    , { ImageFormat::R8_SNORM, "R8Snorm" }

    // float
    , { ImageFormat::RGBA32F, "Rgba32f" }
    , { ImageFormat::RGBA16F, "Rgba16f" }
    , { ImageFormat::RG32F, "Rg32f" }
    , { ImageFormat::RG16F, "Rg16f" }
    , { ImageFormat::R11G11B10F, "R11fG11fB10f" }
    , { ImageFormat::R32F, "R32f" }
    , { ImageFormat::R16F, "R16f" }

    // integer
    , { ImageFormat::RGBA32I, "Rgba32i" }
    , { ImageFormat::RGBA16I, "Rgba16i" }
    , { ImageFormat::RGBA8I, "Rgba8i" }
    , { ImageFormat::RG32I, "Rg32i" }
    , { ImageFormat::RG16I, "Rg16i" }
    , { ImageFormat::RG8I, "Rg8i" }
    , { ImageFormat::R32I, "R32i" }
    , { ImageFormat::R16I, "R16i" }
    , { ImageFormat::R8I, "R8i" }

    // unsigned integer
    , { ImageFormat::RGBA32U, "Rgba32ui" }
    , { ImageFormat::RGBA16U, "Rgba16ui" }
    , { ImageFormat::RGB10_A2U, "Rga10a2ui" }
    , { ImageFormat::RGBA8U, "Rgba8ui" }
    , { ImageFormat::RG32U, "Rg32ui" }
    , { ImageFormat::RG16U, "Rg16ui" }
    , { ImageFormat::RG8U, "Rg8ui" }
    , { ImageFormat::R32U, "R32ui" }
    , { ImageFormat::R16U, "R16ui" }
    , { ImageFormat::R8U, "R8ui" }
};

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateTypeSPIRV(
    Compiler* compiler
    , SPIRVGenerator* generator
    , Type::FullType type
    , Type* typeSymbol
    , ImageFormat imageFormat = ImageFormat::InvalidImageFormat
    , Variable::__Resolved::AccessBits accessBits = Variable::__Resolved::AccessBits(0x0)
    , Variable::__Resolved::UsageBits usageBits = Variable::__Resolved::UsageBits(0x0)
)
{
    std::string baseType = Type::CodeToString(typeSymbol->baseType);
    std::string spirvType = typeToSpirvType[baseType];
    SPIRVResult::Scope scope = SPIRVResult::Scope::Function;

    uint32_t name = 0xFFFFFFFF;
    if (typeSymbol->category == Type::ScalarCategory)
    {
        name = generator->AddSymbol(baseType, spirvType, true);
        if (typeSymbol->IsVector())
        {
            name = generator->AddSymbol(type.name, Format("OpTypeVector %%%d %d", name, typeSymbol->columnSize), true);
        }
        else if (typeSymbol->IsMatrix())
        {
            generator->AddCapability("Matrix");
            baseType = Format("%s(%d", baseType.c_str(), typeSymbol->columnSize);
            name = generator->AddSymbol(baseType, Format("OpTypeVector %%%d %d", name, typeSymbol->columnSize), true);
            name = generator->AddSymbol(type.name, Format("OpTypeMatrix %%%d %d", name, typeSymbol->rowSize), true);
        }

        if (usageBits.flags.isShaderInput)
            scope = SPIRVResult::Scope::Input;
        else if (usageBits.flags.isShaderOutput)
            scope = SPIRVResult::Scope::Output;
        else if (usageBits.flags.isGroupShared)
            scope = SPIRVResult::Scope::WorkGroup;
    }
    else if (typeSymbol->category == Type::ReadWriteTextureCategory || typeSymbol->category == Type::TextureCategory || typeSymbol->category == Type::SampledTextureCategory)
    {
        Type* vec = compiler->GetSymbol<Type>("f32x4");
        SPIRVResult vecType = GenerateTypeSPIRV(compiler, generator, Type::FullType{ "f32x4" }, vec);
        std::string accessPattern = "";
        if (typeSymbol->category == Type::ReadWriteTextureCategory)
        {
            if (accessBits.flags.readAccess && accessBits.flags.writeAccess)
                accessPattern = "ReadWrite";
            else if (accessBits.flags.readAccess)
                accessPattern = "ReadOnly";
            else if (accessBits.flags.writeAccess)
                accessPattern = "WriteOnly";
            std::string spirvFormat = imageFormatToSpirvType[imageFormat];
            spirvType = Format(spirvType.c_str(), vecType.typeName, spirvFormat.c_str(), accessPattern.c_str());
        }
        else
        {
            spirvType = Format(spirvType.c_str(), vecType, "ReadOnly");
        }
        name = generator->AddSymbol(baseType, spirvType, true);
        scope = SPIRVResult::Scope::Image;
    }
    else if (typeSymbol->category == Type::PixelCacheCategory)
    {
        Type* vec = compiler->GetSymbol<Type>("f32x4");
        SPIRVResult vecType = GenerateTypeSPIRV(compiler, generator, Type::FullType{ "f32x4" }, vec);
        spirvType = Format(spirvType.c_str(), vecType.typeName);
        name = generator->AddSymbol(baseType, spirvType, true);
        scope = SPIRVResult::Scope::Image;
    }
    else if (typeSymbol->category == Type::VoidCategory)
    {
        name = generator->AddSymbol("void", "OpTypeVoid", true);
    }
    else if (typeSymbol->category == Type::UserTypeCategory)
    {
        name = generator->GetSymbol(typeSymbol->name);
        baseType = typeSymbol->name;
        if (usageBits.flags.isMutable)
            scope = SPIRVResult::Scope::StorageBuffer;
        else if (usageBits.flags.isUniform)
            scope = SPIRVResult::Scope::Uniform;
        else if (usageBits.flags.isInline)
            scope = SPIRVResult::Scope::PushConstant;
    }

    std::string scopeString = SPIRVResult::ScopeToString(scope);
    for (size_t i = 0; i < type.modifiers.size(); i++)
    {
        const Type::FullType::Modifier& mod = type.modifiers[i];
        if (mod == Type::FullType::Modifier::PointerLevel)
        {
            std::string newBase = Format("ptr(%s)%s", baseType.c_str(), scopeString.c_str());
            name = generator->AddSymbol(newBase, Format("OpTypePointer %s %%%d", scopeString.c_str(), name), true);
            baseType = newBase;
        }
        else if (mod == Type::FullType::Modifier::ArrayLevel)
        {
            if (type.modifierValues[i] == 0x0)
            {
                std::string newBase = Format("arr[](%s)", baseType.c_str());
                name = generator->AddSymbol(newBase, Format("OpTypeRuntimeArray %%%d", name));
                baseType = newBase;
            }
            else
            {
                std::string newBase = Format("arr[%d](%s)", type.modifierValues[i], baseType.c_str());
                Type* i32 = compiler->GetSymbol<Type>("i32");
                SPIRVResult intType = GenerateTypeSPIRV(compiler, generator, Type::FullType{ "i32" }, i32);
                uint32_t arraySizeConstant = generator->AddSymbol(Format("%ds", type.modifierValues[i]), Format("OpConstant %%%d %d", intType.typeName, type.modifierValues[i]), true);
                name = generator->AddSymbol(newBase, Format("OpTypeArray %%%d %%%d", name, arraySizeConstant), true);
                generator->AddDecoration(newBase, name, Format("ArrayStride %d", typeSymbol->CalculateAlignment()));
                baseType = newBase;
            }
        }
    }

    return SPIRVResult(0xFFFFFFFF, name, false, false, scope);
}



struct ConstantCreationInfo
{
    enum class Type
    {
        Float32,
        Int32,
        UInt32
    } type;
    union
    {
        float f;
        int32_t i;
        uint32_t ui;
    } data;

    static ConstantCreationInfo Float32(float val)
    {
        ConstantCreationInfo ret;
        ret.type = Type::Float32;
        ret.data.f = val;
        return ret;
    }

    static ConstantCreationInfo UInt32(uint32_t val)
    {
        ConstantCreationInfo ret;
        ret.type = Type::UInt32;
        ret.data.ui = val;
        return ret;
    }

    static ConstantCreationInfo Int32(int32_t val)
    {
        ConstantCreationInfo ret;
        ret.type = Type::Int32;
        ret.data.i = val;
        return ret;
    }
};

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateConstantSPIRV(Compiler* compiler, SPIRVGenerator* generator, ConstantCreationInfo info)
{
    SPIRVResult res = SPIRVResult::Invalid();
    switch (info.type)
    {
        case ConstantCreationInfo::Type::UInt32:
        {
            uint32_t baseType = generator->AddSymbol("u32", "OpTypeInt 32 0", true);
            res.typeName = baseType;
            res.name = generator->AddSymbol(Format("%du", info.data.ui), Format("OpConstant %%%d %d", baseType, info.data.ui), true);
            break;
        }
        case ConstantCreationInfo::Type::Int32:
        {
            uint32_t baseType = generator->AddSymbol("i32", "OpTypeInt 32 1", true);
            res.typeName = baseType;
            res.name = generator->AddSymbol(Format("%di", info.data.i), Format("OpConstant %%%d %d", baseType, info.data.i), true);
            break;
        }
        case ConstantCreationInfo::Type::Float32:
        {
            uint32_t baseType = generator->AddSymbol("f32", "OpTypeFloat 32", true);
            res.typeName = baseType;
            res.name = generator->AddSymbol(Format("%ff", info.data.f), Format("OpConstant %%%d %d", baseType, info.data.f), true);
            break;
        }
    }
    return res;
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

std::unordered_map<ConversionTable, std::function<SPIRVResult(Compiler*, SPIRVGenerator*, SPIRVResult)>> converters =
{
    { ConversionTable::IntToFloat, [](Compiler* c, SPIRVGenerator* g, SPIRVResult value) -> SPIRVResult {
        uint32_t type = g->AddSymbol("f32", "OpTypeFloat 32", true);        
        return SPIRVResult(g->AddMappedOp(Format("OpConvertSToF %%%d %%%d", type, value.name)), type, true, value.isLiteral);
    } }
    , { ConversionTable::IntToUInt, [](Compiler* c, SPIRVGenerator* g, SPIRVResult value) -> SPIRVResult {
        uint32_t type = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        if (!value.isValue)
            value.name = g->AddMappedOp(Format("OpLoad %%%d %%%d", value.typeName, value.name));
        return SPIRVResult(g->AddMappedOp(Format("OpBitcast %%%d %%%d", type, value.name)), type, true, value.isLiteral);
    } }
    , { ConversionTable::UIntToInt, [](Compiler* c, SPIRVGenerator* g, SPIRVResult value) -> SPIRVResult {
        uint32_t type = g->AddSymbol("i32", "OpTypeInt 32 1", true);
        if (!value.isValue)
            value.name = g->AddMappedOp(Format("OpLoad %%%d %%%d", value.typeName, value.name));
        return SPIRVResult(g->AddMappedOp(Format("OpBitcast %%%d %%%d", type, value.name)), type, true, value.isLiteral);
    } }
    , { ConversionTable::UIntToFloat, [](Compiler* c, SPIRVGenerator* g, SPIRVResult value) -> SPIRVResult {
        uint32_t type = g->AddSymbol("f32", "OpTypeFloat 32", true);
        return SPIRVResult(g->AddMappedOp(Format("OpConvertUToF %%%d %%%d", type, value.name)), type, true, value.isLiteral);
    } }
    , { ConversionTable::FloatToUInt, [](Compiler* c, SPIRVGenerator* g, SPIRVResult value) -> SPIRVResult {
        uint32_t type = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        return SPIRVResult(g->AddMappedOp(Format("OpConvertFToU %%%d %%%d", type, value.name)), type, true, value.isLiteral);
    } }
    , { ConversionTable::FloatToInt, [](Compiler* c, SPIRVGenerator* g, SPIRVResult value) -> SPIRVResult {
        uint32_t type = g->AddSymbol("i32", "OpTypeInt 32 1", true);
        return SPIRVResult(g->AddMappedOp(Format("OpConvertFToS %%%d %%%d", type, value.name)), type, true, value.isLiteral);
    } }
};

//------------------------------------------------------------------------------
/**
*/
void
GenerateConversionsSPIRV(Compiler* compiler, SPIRVGenerator* generator, ConversionTable conversion, const std::vector<SPIRVResult>& inArgs, std::vector<SPIRVResult>& outValues)
{
    for (size_t i = 0; i < inArgs.size(); i++)
    {
        outValues.push_back(converters[conversion](compiler, generator, inArgs[i]));
    }
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateConversionSPIRV(Compiler* compiler, SPIRVGenerator* generator, ConversionTable conversion, SPIRVResult inArg)
{
    return converters[conversion](compiler, generator, inArg);
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateCompositeSPIRV(Compiler* compiler, SPIRVGenerator* generator, uint32_t returnType, const std::vector<SPIRVResult>& args)
{
    std::string argList = "";
    bool isLiteral = true;
    for (const SPIRVResult& arg : args)
    {
        argList.append(Format("%%%d ", arg.name));
        if (!arg.isLiteral)
            isLiteral = false;
    }
    if (isLiteral)
        return SPIRVResult(generator->AddMappedOp(Format("OpConstantConstruct %%%d %s", returnType, argList.c_str())), returnType, true, true);
    else
        return SPIRVResult(generator->AddMappedOp(Format("OpCompositeConstruct %%%d %s", returnType, argList.c_str())), returnType, true);
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateSplatCompositeSPIRV(Compiler* compiler, SPIRVGenerator* generator, uint32_t returnType, uint32_t num, SPIRVResult arg)
{
    std::vector<SPIRVResult> splat(4, arg);
    return GenerateCompositeSPIRV(compiler, generator, returnType, splat);
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateConvertAndSplatCompositeSPIRV(Compiler* compiler, SPIRVGenerator* generator, uint32_t returnType, const std::vector<SPIRVResult>& args, ConversionTable conversion)
{
    std::vector<SPIRVResult> converted;
    GenerateConversionsSPIRV(compiler, generator, conversion, args, converted);
    return GenerateSplatCompositeSPIRV(compiler, generator, returnType, 4, converted[0]);
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateCompositeExtractSPIRV(Compiler* compiler, SPIRVGenerator* generator, uint32_t returnType, uint32_t arg, uint32_t index)
{
    return SPIRVResult(generator->AddMappedOp(Format("OpCompositeExtract %%%d %%%d %d", returnType, arg, index)), returnType, true);
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateCompositeInsertSPIRV(Compiler* compiler, SPIRVGenerator* generator, uint32_t returnType, uint32_t arg, uint32_t index, uint32_t val)
{
    return SPIRVResult(generator->AddMappedOp(Format("OpCompositeInsert %%%d %%%d %%%d %d", returnType, val, arg, index)), returnType, true );
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateConvertAndCompositeSPIRV(Compiler* compiler, SPIRVGenerator* generator, uint32_t returnType, const std::vector<SPIRVResult>& args, ConversionTable conversion)
{
    std::vector<SPIRVResult> converted;
    GenerateConversionsSPIRV(compiler, generator, conversion, args, converted);
    return GenerateCompositeSPIRV(compiler, generator, returnType, converted);
}

//------------------------------------------------------------------------------
/**
*/
SPIRVGenerator::SPIRVGenerator()
    : symbolCounter(0)
{
    // Setup intrinsics
    this->mergeBlockCounter = 0;
    intrinsicMap.clear();
    SetupIntrinsics();
}

//------------------------------------------------------------------------------
/**
*/
void
SPIRVGenerator::SetupIntrinsics()
{
    this->intrinsicMap[&Float4::ctor_XYZW] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 4);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Float4::ctorSingleValue] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateSplatCompositeSPIRV(c, g, returnType, 4, args[0]);
    };
    this->intrinsicMap[&Float4::ctor_3_W] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 2);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Float4::ctor_2_ZW] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 3);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Float3::ctor_XYZ] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 3);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Float3::ctorSingleValue] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateSplatCompositeSPIRV(c, g, returnType, 3, args[0]);
    };
    this->intrinsicMap[&Float3::ctor_f32x2_Z] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 2);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Float2::ctor_XY] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 2);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Float2::ctorSingleValue] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateSplatCompositeSPIRV(c, g, returnType, 2, args[0]);
    };
    this->intrinsicMap[&Float::ctor_UInt] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateConversionSPIRV(c, g, ConversionTable::UIntToFloat, args[0]);
    };
    this->intrinsicMap[&Float::ctor_Int] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateConversionSPIRV(c, g, ConversionTable::IntToFloat, args[0]);
    };
    this->intrinsicMap[&Int4::ctor_XYZW] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 4);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Int4::ctorSingleValue] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateSplatCompositeSPIRV(c, g, returnType, 4, args[0]);
    };
    this->intrinsicMap[&Int4::ctor_3_W] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 2);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Int4::ctor_2_ZW] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 3);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Int3::ctor_XYZ] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 3);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Int3::ctorSingleValue] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateSplatCompositeSPIRV(c, g, returnType, 3, args[0]);
    };
    this->intrinsicMap[&Int2::ctor_XY] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 2);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Int2::ctorSingleValue] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateSplatCompositeSPIRV(c, g, returnType, 2, args[0]);
    };
    this->intrinsicMap[&Int::ctor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateConversionSPIRV(c, g, ConversionTable::UIntToInt, args[0]);
    };
    this->intrinsicMap[&UInt4::ctor_XYZW] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 4);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&UInt4::ctorSingleValue] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateSplatCompositeSPIRV(c, g, returnType, 4, args[0]);
    };
    this->intrinsicMap[&UInt4::ctor_3_W] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 2);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&UInt4::ctor_2_ZW] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 3);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&UInt3::ctor_XYZ] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 3);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&UInt3::ctorSingleValue] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateSplatCompositeSPIRV(c, g, returnType, 3, args[0]);
    };
    this->intrinsicMap[&UInt2::ctor_XY] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 2);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&UInt2::ctorSingleValue] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateSplatCompositeSPIRV(c, g, returnType, 2, args[0]);
    };
    this->intrinsicMap[&UInt::ctor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateConversionSPIRV(c, g, ConversionTable::IntToUInt, args[0]);
    };
    this->intrinsicMap[&Bool4::ctor_XYZW] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 4);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Bool4::ctorSingleValue] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateSplatCompositeSPIRV(c, g, returnType, 4, args[0]);
    };
    this->intrinsicMap[&Bool3::ctor_XYZ] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 3);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Bool3::ctorSingleValue] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateSplatCompositeSPIRV(c, g, returnType, 3, args[0]);
    };
    this->intrinsicMap[&Bool2::ctor_XY] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 2);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Bool2::ctorSingleValue] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateSplatCompositeSPIRV(c, g, returnType, 2, args[0]);
    };
    this->intrinsicMap[&Bool::ctor_UInt] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return args[0];
    };
    this->intrinsicMap[&Bool::ctor_Int] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return args[0];
    };

    // For matrix float constructors, we need to first construct the vectors and then compose the matrix from them
    this->intrinsicMap[&Mat2x2::floatConstructor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 4);
        Type* vecType = c->GetSymbol<Type>("f32x2");
        SPIRVResult vectorType = GenerateTypeSPIRV(c, g, Type::FullType{ "f32x2" }, vecType);
        SPIRVResult col0 = GenerateCompositeSPIRV(c, g, vectorType.typeName, { args[0], args[2] });
        SPIRVResult col1 = GenerateCompositeSPIRV(c, g, vectorType.typeName, { args[1], args[3] });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1 });
    };
    this->intrinsicMap[&Mat2x2::vectorConstructor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 2);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Mat2x3::floatConstructor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 6);
        Type* vecType = c->GetSymbol<Type>("f32x2");
        SPIRVResult vectorType = GenerateTypeSPIRV(c, g, Type::FullType{ "f32x2" }, vecType);
        SPIRVResult col0 = GenerateCompositeSPIRV(c, g, vectorType.typeName, { args[0], args[3] });
        SPIRVResult col1 = GenerateCompositeSPIRV(c, g, vectorType.typeName, { args[1], args[4] });
        SPIRVResult col3 = GenerateCompositeSPIRV(c, g, vectorType.typeName, { args[2], args[5] });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1, col3 });
    };
    this->intrinsicMap[&Mat2x3::vectorConstructor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 3);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Mat2x4::floatConstructor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 8);
        Type* vecType = c->GetSymbol<Type>("f32x2");
        SPIRVResult vectorType = GenerateTypeSPIRV(c, g, Type::FullType{ "f32x2" }, vecType);
        SPIRVResult col0 = GenerateCompositeSPIRV(c, g, vectorType.typeName, { args[0], args[4] });
        SPIRVResult col1 = GenerateCompositeSPIRV(c, g, vectorType.typeName, { args[1], args[5] });
        SPIRVResult col3 = GenerateCompositeSPIRV(c, g, vectorType.typeName, { args[2], args[6] });
        SPIRVResult col4 = GenerateCompositeSPIRV(c, g, vectorType.typeName, { args[3], args[7] });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1, col3, col4 });
    };
    this->intrinsicMap[&Mat2x4::vectorConstructor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 3);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };

    this->intrinsicMap[&Mat3x2::floatConstructor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 6);
        Type* vecType = c->GetSymbol<Type>("f32x3");
        SPIRVResult vectorType = GenerateTypeSPIRV(c, g, Type::FullType{ "f32x3" }, vecType);
        SPIRVResult col0 = GenerateCompositeSPIRV(c, g, vectorType.typeName, { args[0], args[2], args[4] });
        SPIRVResult col1 = GenerateCompositeSPIRV(c, g, vectorType.typeName, { args[1], args[3], args[5] });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1 });
    };
    this->intrinsicMap[&Mat3x2::vectorConstructor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 2);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Mat3x3::floatConstructor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 9);
        Type* vecType = c->GetSymbol<Type>("f32x3");
        SPIRVResult vectorType = GenerateTypeSPIRV(c, g, Type::FullType{ "f32x3" }, vecType);
        SPIRVResult col0 = GenerateCompositeSPIRV(c, g, vectorType.typeName, { args[0], args[3], args[6] });
        SPIRVResult col1 = GenerateCompositeSPIRV(c, g, vectorType.typeName, { args[1], args[4], args[7] });
        SPIRVResult col2 = GenerateCompositeSPIRV(c, g, vectorType.typeName, { args[2], args[5], args[8] });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1, col2 });
    };
    this->intrinsicMap[&Mat3x3::vectorConstructor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 3);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Mat3x4::floatConstructor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 12);
        Type* vecType = c->GetSymbol<Type>("f32x3");
        SPIRVResult vectorType = GenerateTypeSPIRV(c, g, Type::FullType{ "f32x3" }, vecType);
        SPIRVResult col0 = GenerateCompositeSPIRV(c, g, vectorType.typeName, { args[0], args[4], args[8] });
        SPIRVResult col1 = GenerateCompositeSPIRV(c, g, vectorType.typeName, { args[1], args[5], args[9] });
        SPIRVResult col2 = GenerateCompositeSPIRV(c, g, vectorType.typeName, { args[2], args[6], args[10] });
        SPIRVResult col3 = GenerateCompositeSPIRV(c, g, vectorType.typeName, { args[3], args[7], args[11] });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1, col2, col3 });
    };
    this->intrinsicMap[&Mat3x4::vectorConstructor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 4);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Mat4x2::floatConstructor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 8);
        Type* vecType = c->GetSymbol<Type>("float4");
        SPIRVResult vectorType = GenerateTypeSPIRV(c, g, Type::FullType{ "float4" }, vecType);
        SPIRVResult col0 = GenerateCompositeSPIRV(c, g, vectorType.typeName, { args[0], args[2], args[4], args[6] });
        SPIRVResult col1 = GenerateCompositeSPIRV(c, g, vectorType.typeName, { args[1], args[3], args[5], args[7] });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1 });
    };
    this->intrinsicMap[&Mat4x2::vectorConstructor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 2);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Mat4x3::floatConstructor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 12);
        Type* vecType = c->GetSymbol<Type>("f32x4");
        SPIRVResult vectorType = GenerateTypeSPIRV(c, g, Type::FullType{ "f32x4" }, vecType);
        SPIRVResult col0 = GenerateCompositeSPIRV(c, g, vectorType.typeName, { args[0], args[3], args[6], args[9] });
        SPIRVResult col1 = GenerateCompositeSPIRV(c, g, vectorType.typeName, { args[1], args[4], args[7], args[10] });
        SPIRVResult col2 = GenerateCompositeSPIRV(c, g, vectorType.typeName, { args[2], args[5], args[8], args[11] });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1, col2 });
    };
    this->intrinsicMap[&Mat4x3::vectorConstructor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 3);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Mat4x4::floatConstructor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 16);
        Type* vecType = c->GetSymbol<Type>("f32x4");
        SPIRVResult vectorType = GenerateTypeSPIRV(c, g, Type::FullType{ "f32x4" }, vecType);
        SPIRVResult col0 = GenerateCompositeSPIRV(c, g, vectorType.typeName, { args[0], args[4], args[8], args[12] });
        SPIRVResult col1 = GenerateCompositeSPIRV(c, g, vectorType.typeName, { args[1], args[5], args[9], args[13] });
        SPIRVResult col2 = GenerateCompositeSPIRV(c, g, vectorType.typeName, { args[2], args[6], args[10], args[14] });
        SPIRVResult col3 = GenerateCompositeSPIRV(c, g, vectorType.typeName, { args[3], args[7], args[11], args[15] });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1, col2, col3 });
    };
    this->intrinsicMap[&Mat4x3::vectorConstructor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 4);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };

#define OPERATOR_INTRINSIC(ty, fn, op, inst)\
    { &ty::##fn##Operator, #op[0], #inst, false }\
    , { &ty::##fn##AssignOperator, #op[0], #inst, true }

#define OPERATOR_INTRINSIC_NO_ASSIGN(ty, fn, op, inst)\
    { &ty::##fn##Operator, #op[0], #inst, false }\

    std::vector<std::tuple<Function*, char, const char*, bool>> operatorFunctions =
    {
        OPERATOR_INTRINSIC(Float4, addition, F, Add)
        , OPERATOR_INTRINSIC(Float3, addition, F, Add)
        , OPERATOR_INTRINSIC(Float2, addition, F, Add)
        , OPERATOR_INTRINSIC(Float, addition, F, Add)
        , OPERATOR_INTRINSIC(Float4, subtraction, F, Sub)
        , OPERATOR_INTRINSIC(Float3, subtraction, F, Sub)
        , OPERATOR_INTRINSIC(Float2, subtraction, F, Sub)
        , OPERATOR_INTRINSIC(Float, subtraction, F, Sub)
        , OPERATOR_INTRINSIC(Float4, multiplication, F, Mul)
        , OPERATOR_INTRINSIC(Float3, multiplication, F, Mul)
        , OPERATOR_INTRINSIC(Float2, multiplication, F, Mul)
        , OPERATOR_INTRINSIC(Float, multiplication, F, Mul)
        , OPERATOR_INTRINSIC(Float4, division, F, Div)
        , OPERATOR_INTRINSIC(Float3, division, F, Div)
        , OPERATOR_INTRINSIC(Float2, division, F, Div)
        , OPERATOR_INTRINSIC(Float, division, F, Div)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float4, mod, F, Mod)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float3, mod, F, Mod)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float2, mod, F, Mod)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float, mod, F, Mod)
        , OPERATOR_INTRINSIC(Int4, addition, I, Add)
        , OPERATOR_INTRINSIC(Int3, addition, I, Add)
        , OPERATOR_INTRINSIC(Int2, addition, I, Add)
        , OPERATOR_INTRINSIC(Int, addition, I, Add)
        , OPERATOR_INTRINSIC(Int4, subtraction, I, Sub)
        , OPERATOR_INTRINSIC(Int3, subtraction, I, Sub)
        , OPERATOR_INTRINSIC(Int2, subtraction, I, Sub)
        , OPERATOR_INTRINSIC(Int, subtraction, I, Sub)
        , OPERATOR_INTRINSIC(Int4, multiplication, I, Mul)
        , OPERATOR_INTRINSIC(Int3, multiplication, I, Mul)
        , OPERATOR_INTRINSIC(Int2, multiplication, I, Mul)
        , OPERATOR_INTRINSIC(Int, multiplication, I, Mul)
        , OPERATOR_INTRINSIC(Int4, division, S, Div)
        , OPERATOR_INTRINSIC(Int3, division, S, Div)
        , OPERATOR_INTRINSIC(Int2, division, S, Div)
        , OPERATOR_INTRINSIC(Int, division, S, Div)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Int4, mod, S, Mod)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Int3, mod, S, Mod)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Int2, mod, S, Mod)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Int, mod, S, Mod)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Int4, lt, S, LessThan)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Int3, lt, S, LessThan)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Int2, lt, S, LessThan)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Int, lt, S, LessThan)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Int4, lte, S, LessThanEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Int3, lte, S, LessThanEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Int2, lte, S, LessThanEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Int, lte, S, LessThanEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Int4, gt, S, GreaterThan)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Int3, gt, S, GreaterThan)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Int2, gt, S, GreaterThan)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Int, gt, S, GreaterThan)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Int4, gte, S, GreaterThanEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Int3, gte, S, GreaterThanEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Int2, gte, S, GreaterThanEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Int, gte, S, GreaterThanEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Int4, e, I, Equal)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Int3, e, I, Equal)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Int2, e, I, Equal)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Int, e, I, Equal)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Int4, ne, I, Equal)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Int3, ne, I, Equal)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Int2, ne, I, Equal)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Int, ne, I, Equal)
        , OPERATOR_INTRINSIC(UInt4, addition, I, Add)
        , OPERATOR_INTRINSIC(UInt3, addition, I, Add)
        , OPERATOR_INTRINSIC(UInt2, addition, I, Add)
        , OPERATOR_INTRINSIC(UInt, addition, I, Add)
        , OPERATOR_INTRINSIC(UInt4, subtraction, I, Sub)
        , OPERATOR_INTRINSIC(UInt3, subtraction, I, Sub)
        , OPERATOR_INTRINSIC(UInt2, subtraction, I, Sub)
        , OPERATOR_INTRINSIC(UInt, subtraction, I, Sub)
        , OPERATOR_INTRINSIC(UInt4, multiplication, I, Mul)
        , OPERATOR_INTRINSIC(UInt3, multiplication, I, Mul)
        , OPERATOR_INTRINSIC(UInt2, multiplication, I, Mul)
        , OPERATOR_INTRINSIC(UInt, multiplication, I, Mul)
        , OPERATOR_INTRINSIC(UInt4, division, U, Div)
        , OPERATOR_INTRINSIC(UInt3, division, U, Div)
        , OPERATOR_INTRINSIC(UInt2, division, U, Div)
        , OPERATOR_INTRINSIC(UInt, division, U, Div)
        , OPERATOR_INTRINSIC_NO_ASSIGN(UInt4, mod, U, Mod)
        , OPERATOR_INTRINSIC_NO_ASSIGN(UInt3, mod, U, Mod)
        , OPERATOR_INTRINSIC_NO_ASSIGN(UInt2, mod, U, Mod)
        , OPERATOR_INTRINSIC_NO_ASSIGN(UInt, mod, U, Mod)
        , OPERATOR_INTRINSIC_NO_ASSIGN(UInt4, lt, U, LessThan)
        , OPERATOR_INTRINSIC_NO_ASSIGN(UInt3, lt, U, LessThan)
        , OPERATOR_INTRINSIC_NO_ASSIGN(UInt2, lt, U, LessThan)
        , OPERATOR_INTRINSIC_NO_ASSIGN(UInt, lt, U, LessThan)
        , OPERATOR_INTRINSIC_NO_ASSIGN(UInt4, lte, U, LessThanEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(UInt3, lte, U, LessThanEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(UInt2, lte, U, LessThanEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(UInt, lte, U, LessThanEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(UInt4, gt, U, GreaterThan)
        , OPERATOR_INTRINSIC_NO_ASSIGN(UInt3, gt, U, GreaterThan)
        , OPERATOR_INTRINSIC_NO_ASSIGN(UInt2, gt, U, GreaterThan)
        , OPERATOR_INTRINSIC_NO_ASSIGN(UInt, gt, U, GreaterThan)
        , OPERATOR_INTRINSIC_NO_ASSIGN(UInt4, gte, U, GreaterThanEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(UInt3, gte, U, GreaterThanEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(UInt2, gte, U, GreaterThanEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(UInt, gte, U, GreaterThanEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(UInt4, e, I, Equal)
        , OPERATOR_INTRINSIC_NO_ASSIGN(UInt3, e, I, Equal)
        , OPERATOR_INTRINSIC_NO_ASSIGN(UInt2, e, I, Equal)
        , OPERATOR_INTRINSIC_NO_ASSIGN(UInt, e, I, Equal)
        , OPERATOR_INTRINSIC_NO_ASSIGN(UInt4, ne, I, Equal)
        , OPERATOR_INTRINSIC_NO_ASSIGN(UInt3, ne, I, Equal)
        , OPERATOR_INTRINSIC_NO_ASSIGN(UInt2, ne, I, Equal)
        , OPERATOR_INTRINSIC_NO_ASSIGN(UInt, ne, I, Equal)
        , OPERATOR_INTRINSIC(Mat2x2, addition, F, Add)
        , OPERATOR_INTRINSIC(Mat2x3, addition, F, Add)
        , OPERATOR_INTRINSIC(Mat2x4, addition, F, Add)
        , OPERATOR_INTRINSIC(Mat3x2, addition, F, Add)
        , OPERATOR_INTRINSIC(Mat3x3, addition, F, Add)
        , OPERATOR_INTRINSIC(Mat3x4, addition, F, Add)
        , OPERATOR_INTRINSIC(Mat4x2, addition, F, Add)
        , OPERATOR_INTRINSIC(Mat4x3, addition, F, Add)
        , OPERATOR_INTRINSIC(Mat4x4, addition, F, Add)
    };

    for (auto fun : operatorFunctions)
    {
        this->intrinsicMap[std::get<0>(fun)] = [ty = std::get<1>(fun), op = std::get<2>(fun), assign = std::get<3>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() >= 2);
            uint32_t ret = g->AddMappedOp(Format("Op%c%s %%%d %%%d %%%d", ty, op, returnType, args[0].name, args[1].name));
            if (assign)
            {
                assert(args.size() == 3);
                assert(!args[2].isValue);
                g->AddOp(Format("OpStore %%%d %%%d", args[2].name, ret));
                return SPIRVResult(args[2].name, args[2].typeName);
            }
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, ConversionTable>> assignOperators =
    {
        { &Int::uintAssignOperator, ConversionTable::UIntToInt }
        , { &UInt::intAssignOperator, ConversionTable::IntToUInt }
        , { &Float::intAssignOperator, ConversionTable::IntToFloat }
        , { &Float::uintAssignOperator, ConversionTable::UIntToFloat }
    };
    for (auto fun : assignOperators)
    {
        this->intrinsicMap[std::get<0>(fun)] = [conv = std::get<1>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            return GenerateConversionSPIRV(c, g, conv, args[0]);
        };
    }

    std::vector<std::pair<Function*, bool>> matrixMultiplicationFunctions =
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
        this->intrinsicMap[fun.first] = [assign = fun.second](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            uint32_t ret = g->AddMappedOp(Format("OpMatrixTimesMatrix %%%d %%%d %%%d", returnType, args[0].name, args[1].name));
            if (assign)
                g->ReplaceSymbolMapping(args[0].name, ret);
            return SPIRVResult(ret, returnType, true);
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
        this->intrinsicMap[fun] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            return SPIRVResult(g->AddMappedOp(Format("OpMatrixTimesVector %%%d %%%d %%%d", returnType, args[0].name, args[1].name)), returnType, true);
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
        this->intrinsicMap[fun] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            return SPIRVResult(g->AddMappedOp(Format("OpVectorTimesScalar %%%d %%%d %%%d", returnType, args[0].name, args[1].name)), returnType, true);
        };
    }

    std::unordered_map<Function*, std::pair<char, uint32_t>> intVectorScaleFunctions =
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
        this->intrinsicMap[fun.first] = [op = fun.second.first, num = fun.second.second](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            SPIRVResult splat = GenerateSplatCompositeSPIRV(c, g, returnType, num, args[1]);
            return SPIRVResult(g->AddMappedOp(Format("Op%cMul %%%d %%%d %%%d", op, returnType, args[0].name, splat.name)), returnType, true);
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
        this->intrinsicMap[fun] = [fun](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            return SPIRVResult(g->AddMappedOp(Format("OpVectorTimesMatrix %%%d %%%d %%%d", args[0].typeName, args[0].name, args[1].name), fun->name), returnType, true);
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
        this->intrinsicMap[fun] = [fun](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            return SPIRVResult(g->AddMappedOp(Format("OpAccessChain %%%d %%%d %%%d", returnType, args[0].name, args[1].name), fun->name), returnType);
        };
    }

#define BIT_INTRINSIC(ty, fn, inst)\
    { &ty::##fn##Operator, #inst, false }\
    , { &ty::##fn##AssignOperator, #inst, true }

    std::vector<std::tuple<Function*, const char*, bool>> bitwiseOps =
    {
        BIT_INTRINSIC(Int4, or, BitwiseOr)
        , BIT_INTRINSIC(Int3, or, BitwiseOr)
        , BIT_INTRINSIC(Int2, or, BitwiseOr)
        , BIT_INTRINSIC(Int, or, BitwiseOr)
        , BIT_INTRINSIC(Int4, xor, BitwiseXor)
        , BIT_INTRINSIC(Int3, xor, BitwiseXor)
        , BIT_INTRINSIC(Int2, xor, BitwiseXor)
        , BIT_INTRINSIC(Int, xor, BitwiseXor)
        , BIT_INTRINSIC(Int4, and, BitwiseAnd)
        , BIT_INTRINSIC(Int3, and, BitwiseAnd)
        , BIT_INTRINSIC(Int2, and, BitwiseAnd)
        , BIT_INTRINSIC(Int, and, BitwiseAnd)
        , BIT_INTRINSIC(UInt4, or, BitwiseOr)
        , BIT_INTRINSIC(UInt3, or, BitwiseOr)
        , BIT_INTRINSIC(UInt2, or, BitwiseOr)
        , BIT_INTRINSIC(UInt, or, BitwiseOr)
        , BIT_INTRINSIC(UInt4, xor, BitwiseXor)
        , BIT_INTRINSIC(UInt3, xor, BitwiseXor)
        , BIT_INTRINSIC(UInt2, xor, BitwiseXor)
        , BIT_INTRINSIC(UInt, xor, BitwiseXor)
        , BIT_INTRINSIC(UInt4, and, BitwiseAnd)
        , BIT_INTRINSIC(UInt3, and, BitwiseAnd)
        , BIT_INTRINSIC(UInt2, and, BitwiseAnd)
        , BIT_INTRINSIC(UInt, and, BitwiseAnd)
        , BIT_INTRINSIC(Int4, leftShift, ShiftLeftLogical)
        , BIT_INTRINSIC(Int3, leftShift, ShiftLeftLogical)
        , BIT_INTRINSIC(Int2, leftShift, ShiftLeftLogical)
        , BIT_INTRINSIC(Int, leftShift, ShiftLeftLogical)
        , BIT_INTRINSIC(Int4, rightShift, ShiftRightLogical)
        , BIT_INTRINSIC(Int3, rightShift, ShiftRightLogical)
        , BIT_INTRINSIC(Int2, rightShift, ShiftRightLogical)
        , BIT_INTRINSIC(Int, rightShift, ShiftRightLogical)
        , BIT_INTRINSIC(UInt4, leftShift, ShiftLeftLogical)
        , BIT_INTRINSIC(UInt3, leftShift, ShiftLeftLogical)
        , BIT_INTRINSIC(UInt2, leftShift, ShiftLeftLogical)
        , BIT_INTRINSIC(UInt, leftShift, ShiftLeftLogical)
        , BIT_INTRINSIC(UInt4, rightShift, ShiftRightLogical)
        , BIT_INTRINSIC(UInt3, rightShift, ShiftRightLogical)
        , BIT_INTRINSIC(UInt2, rightShift, ShiftRightLogical)
        , BIT_INTRINSIC(UInt, rightShift, ShiftRightLogical)
    };
    for (auto fun : bitwiseOps)
    {
        this->intrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun), assign = std::get<2>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            uint32_t ret = g->AddMappedOp(Format("Op%s %%%d %%%d %%%d", op, returnType, args[0].name, args[1].name));
            if (assign)
                g->ReplaceSymbolMapping(args[0].name, ret);
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, bool>> textureFetchIntrinsics =
    {
        { Intrinsics::TextureFetchBase_texture1D, false }
        , { Intrinsics::TextureFetchBase_texture2D, false }
        , { Intrinsics::TextureFetchBase_texture2DMS, false }
        , { Intrinsics::TextureFetchBase_texture3D, false }
        , { Intrinsics::TextureFetchBase_texture1DArray, false }
        , { Intrinsics::TextureFetchBase_texture2DArray, false }
        , { Intrinsics::TextureFetchBase_texture2DMSArray, false }
        , { Intrinsics::TextureFetchSample_texture2DMS, true }
        , { Intrinsics::TextureFetchSample_texture2DMSArray, true }
    };
    for (auto fun : textureFetchIntrinsics)
    {
        this->intrinsicMap[std::get<0>(fun)] = [sample = std::get<1>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            uint32_t ret;
            if (sample)
                ret = g->AddMappedOp(Format("OpImageFetch %%%d %%%d %%%d Sample %d", returnType, args[0].name, args[1].name, args[2].name));
            else
                ret = g->AddMappedOp(Format("OpImageFetch %%%d %%%d %%%d None", returnType, args[0].name, args[1].name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, bool>> textureGatherIntrinsics =
    {
        { Intrinsics::TextureGatherBase_texture2D, false }
        , { Intrinsics::TextureGatherBase_texture2DArray, false }
        , { Intrinsics::TextureGatherBase_textureCube, false }
        , { Intrinsics::TextureGatherBase_textureCubeArray, false }
        , { Intrinsics::TextureGatherOffsets_texture2D, true }
        , { Intrinsics::TextureGatherOffsets_texture2DArray, true }
    };
    for (auto fun : textureGatherIntrinsics)
    {
        this->intrinsicMap[std::get<0>(fun)] = [offsets = std::get<1>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            uint32_t ret;
            if (offsets)
                ret = g->AddMappedOp(Format("OpImageGather %%%d %%%d %%%d %%%d ConstOffsets %%%d", returnType, args[0].name, args[1].name, args[2].name));
            else
                ret = g->AddMappedOp(Format("OpImageGather %%%d %%%d %%%d %%%d None", returnType, args[0].name, args[1].name));
            return SPIRVResult(ret, returnType, true);
        };
    }

#define MAKE_SCALAR_INTRINSICS(op)\
      { Intrinsics::##op##_f32, 'F' }\
    , { Intrinsics::##op##_f32x2, 'F' }\
    , { Intrinsics::##op##_f32x3, 'F' }\
    , { Intrinsics::##op##_f32x4, 'F' }\
    , { Intrinsics::##op##_i32, 'S' }\
    , { Intrinsics::##op##_i32x2, 'S' }\
    , { Intrinsics::##op##_i32x3, 'S' }\
    , { Intrinsics::##op##_i32x4, 'S' }\
    , { Intrinsics::##op##_u32, 'U' }\
    , { Intrinsics::##op##_u32x2, 'U' }\
    , { Intrinsics::##op##_u32x3, 'U' }\
    , { Intrinsics::##op##_u32x4, 'U' }

#define MAKE_FLOAT_INTRINSICS(op)\
      { Intrinsics::##op##_f32, 'F' }\
    , { Intrinsics::##op##_f32x2, 'F' }\
    , { Intrinsics::##op##_f32x3, 'F' }\
    , { Intrinsics::##op##_f32x4, 'F' }

#define MAKE_FLOAT_VEC_INTRINSICS(op)\
    { Intrinsics::##op##_f32x2, 'F' }\
    , { Intrinsics::##op##_f32x3, 'F' }\
    , { Intrinsics::##op##_f32x4, 'F' }

#define MAKE_SIGN_INTRINSICS(op)\
      { Intrinsics::##op##_f32, 'F' }\
    , { Intrinsics::##op##_f32x2, 'F' }\
    , { Intrinsics::##op##_f32x3, 'F' }\
    , { Intrinsics::##op##_f32x4, 'F' }\
    , { Intrinsics::##op##_i32, 'S' }\
    , { Intrinsics::##op##_i32x2, 'S' }\
    , { Intrinsics::##op##_i32x3, 'S' }\
    , { Intrinsics::##op##_i32x4, 'S' }

#define MAKE_FLOAT_INTRINSICS_VEC(op)\
      { Intrinsics::##op##_f32, 'F', 1 }\
    , { Intrinsics::##op##_f32x2, 'F', 2 }\
    , { Intrinsics::##op##_f32x3, 'F', 3 }\
    , { Intrinsics::##op##_f32x4, 'F', 4 }


#define MAKE_INT_INTRINSICS(op)\
      { Intrinsics::##op##_i32, 'S' }\
    , { Intrinsics::##op##_i32x2, 'S' }\
    , { Intrinsics::##op##_i32x3, 'S' }\
    , { Intrinsics::##op##_i32x4, 'S' }\
    , { Intrinsics::##op##_u32, 'U' }\
    , { Intrinsics::##op##_u32x2, 'U' }\
    , { Intrinsics::##op##_u32x3, 'U' }\
    , { Intrinsics::##op##_u32x4, 'U' }

    std::vector<std::tuple<Function*, const char>> sqrtIntrinsics =
    {
        MAKE_FLOAT_INTRINSICS(Sqrt)
    };
    for (auto fun : sqrtIntrinsics)
    {
        this->intrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 3);
            uint32_t ext = g->AddExtension("GLSL.std.450");
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d Sqrt %%%d", returnType, ext, args[0].name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> invSqrtIntrinsics =
    {
        MAKE_FLOAT_INTRINSICS(InvSqrt)
    };
    for (auto fun : invSqrtIntrinsics)
    {
        this->intrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 3);
            uint32_t ext = g->AddExtension("GLSL.std.450");
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d InverseSqrt %%%d", returnType, ext, args[0].name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> logIntrinsics =
    {
        MAKE_FLOAT_INTRINSICS(Log)
    };
    for (auto fun : logIntrinsics)
    {
        this->intrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 3);
            uint32_t ext = g->AddExtension("GLSL.std.450");
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d Log %%%d", returnType, ext, args[0].name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> log2Intrinsics =
    {
        MAKE_FLOAT_INTRINSICS(Log2)
    };
    for (auto fun : log2Intrinsics)
    {
        this->intrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 3);
            uint32_t ext = g->AddExtension("GLSL.std.450");
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d Log2 %%%d", returnType, ext, args[0].name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> expIntrinsics =
    {
        MAKE_FLOAT_INTRINSICS(Exp)
    };
    for (auto fun : expIntrinsics)
    {
        this->intrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 3);
            uint32_t ext = g->AddExtension("GLSL.std.450");
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d Exp %%%d", returnType, ext, args[0].name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> exp2Intrinsics =
    {
        MAKE_FLOAT_INTRINSICS(Exp2)
    };
    for (auto fun : exp2Intrinsics)
    {
        this->intrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 3);
            uint32_t ext = g->AddExtension("GLSL.std.450");
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d Exp2 %%%d", returnType, ext, args[0].name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> sinIntrinsics =
    {
        MAKE_FLOAT_INTRINSICS(Sin)
    };
    for (auto fun : sinIntrinsics)
    {
        this->intrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 3);
            uint32_t ext = g->AddExtension("GLSL.std.450");
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d Sin %%%d", returnType, ext, args[0].name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> cosIntrinsics =
    {
        MAKE_FLOAT_INTRINSICS(Cos)
    };
    for (auto fun : cosIntrinsics)
    {
        this->intrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 3);
            uint32_t ext = g->AddExtension("GLSL.std.450");
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d Cos %%%d", returnType, ext, args[0].name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> tanIntrinsics =
    {
        MAKE_FLOAT_INTRINSICS(Tan)
    };
    for (auto fun : tanIntrinsics)
    {
        this->intrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 3);
            uint32_t ext = g->AddExtension("GLSL.std.450");
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d Tan %%%d", returnType, ext, args[0].name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> asinIntrinsics =
    {
        MAKE_FLOAT_INTRINSICS(ASin)
    };
    for (auto fun : asinIntrinsics)
    {
        this->intrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 3);
            uint32_t ext = g->AddExtension("GLSL.std.450");
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d ASin %%%d", returnType, ext, args[0].name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> acosIntrinsics =
    {
        MAKE_FLOAT_INTRINSICS(ACos)
    };
    for (auto fun : acosIntrinsics)
    {
        this->intrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 3);
            uint32_t ext = g->AddExtension("GLSL.std.450");
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d ACos %%%d", returnType, ext, args[0].name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> atanIntrinsics =
    {
        MAKE_FLOAT_INTRINSICS(ATan)
    };
    for (auto fun : atanIntrinsics)
    {
        this->intrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 3);
            uint32_t ext = g->AddExtension("GLSL.std.450");
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d ATan %%%d", returnType, ext, args[0].name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> madIntrinsics =
    {
        MAKE_SCALAR_INTRINSICS(Mad)
    };
    for (auto fun : madIntrinsics)
    {
        this->intrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 3);
            uint32_t ext = g->AddExtension("GLSL.std.450");
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d Fma %%%d %%%d %%%d", returnType, ext, args[0].name, args[1].name, args[2].name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> dotIntrinsics =
    {
        MAKE_FLOAT_VEC_INTRINSICS(Dot)
    };
    for (auto fun : madIntrinsics)
    {
        this->intrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            uint32_t ret = g->AddMappedOp(Format("OpDot %%%d %%%d %%%d", returnType, args[0].name, args[1].name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> crossIntrinsics =
    {
        { Intrinsics::Cross_f32x3, 'F' }
    };
    for (auto fun : madIntrinsics)
    {
        this->intrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            uint32_t ext = g->AddExtension("GLSL.std.450");
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d Cross %%%d %%%d", returnType, args[0].name, args[1].name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> normalizeIntrinsics =
    {
        MAKE_FLOAT_VEC_INTRINSICS(Normalize)
    };
    for (auto fun : normalizeIntrinsics)
    {
        this->intrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            uint32_t ext = g->AddExtension("GLSL.std.450");
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d Normalize %%%d", returnType, args[0].name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> lengthIntrinsics =
    {
        MAKE_FLOAT_VEC_INTRINSICS(Length)
    };
    for (auto fun : lengthIntrinsics)
    {
        this->intrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            uint32_t ext = g->AddExtension("GLSL.std.450");
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d Length %%%d", returnType, args[0].name));
            return SPIRVResult(ret, returnType, true);
        };
    }
    
    std::vector<std::tuple<Function*, const char>> minIntrinsics =
    {
        MAKE_SCALAR_INTRINSICS(Min)
    };
    for (auto fun : minIntrinsics)
    {
        this->intrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            uint32_t ext = g->AddExtension("GLSL.std.450");
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d %cMin %%%d %%%d", returnType, ext, op, args[0].name, args[1].name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> maxIntrinsics =
    {
        MAKE_SCALAR_INTRINSICS(Max)
    };
    for (auto fun : maxIntrinsics)
    {
        this->intrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            uint32_t ext = g->AddExtension("GLSL.std.450");
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d %cMax %%%d %%%d", returnType, ext, op, args[0].name, args[1].name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> clampIntrinsics =
    {
        MAKE_SCALAR_INTRINSICS(Clamp)
    };
    for (auto fun : clampIntrinsics)
    {
        this->intrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            uint32_t ext = g->AddExtension("GLSL.std.450");
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d %cClamp %%%d %%%d %%%d", returnType, ext, op, args[0].name, args[1].name, args[2].name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> ceilIntrinsics =
    {
        MAKE_FLOAT_INTRINSICS(Ceil)
    };
    for (auto fun : ceilIntrinsics)
    {
        this->intrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            uint32_t ext = g->AddExtension("GLSL.std.450");
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d Ceil %%%d", returnType, ext, op, args[0].name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> floorIntrinsics =
    {
        MAKE_FLOAT_INTRINSICS(Floor)
    };
    for (auto fun : floorIntrinsics)
    {
        this->intrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            uint32_t ext = g->AddExtension("GLSL.std.450");
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d Floor %%%d", returnType, ext, op, args[0].name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> fractIntrinsics =
    {
        MAKE_FLOAT_INTRINSICS(Fract)
    };
    for (auto fun : fractIntrinsics)
    {
        this->intrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            uint32_t ext = g->AddExtension("GLSL.std.450");
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d Fract %%%d", returnType, ext, op, args[0].name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char, uint32_t>> saturateIntrinsics =
    {
        MAKE_FLOAT_INTRINSICS_VEC(Saturate)
    };
    for (auto fun : saturateIntrinsics)
    {
        this->intrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun), size = std::get<2>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            uint32_t ext = g->AddExtension("GLSL.std.450");
            uint32_t fType = g->AddSymbol("float", "OpTypeFloat 32");
            uint32_t min = g->AddSymbol("0.0f", Format("OpConstant %%%d 0", fType));
            uint32_t max = g->AddSymbol("1.0f", Format("OpConstant %%%d 1", fType));

            // If vector, make clamp constant composite
            if (size > 1)
            {
                std::string minComposite = Format("OpConstantComposite %%%d ", returnType);
                std::string maxComposite = Format("OpConstantComposite %%%d ", returnType);
                for (uint32_t i = 0; i < size; i++)
                {
                    minComposite.append(Format("%%%d ", min));
                    maxComposite.append(Format("%%%d ", max));
                }
                min = g->AddSymbol(Format("f32x%d(0.0f)", size), minComposite);
                max = g->AddSymbol(Format("f32x%d(1.0f)", size), maxComposite);
            }
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d %cClamp %%%d %%%d %%%d", returnType, ext, op, args[0].name, min, max));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> truncateIntrinsics =
    {
        MAKE_FLOAT_INTRINSICS(Truncate)
    };
    for (auto fun : truncateIntrinsics)
    {
        this->intrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            uint32_t ext = g->AddExtension("GLSL.std.450");
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d Trunc %%%d", returnType, ext, op, args[0].name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> absIntrinsics =
    {
        MAKE_SIGN_INTRINSICS(Abs)
    };
    for (auto fun : absIntrinsics)
    {
        this->intrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            uint32_t ext = g->AddExtension("GLSL.std.450");
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d Abs %%%d", returnType, ext, op, args[0].name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> signIntrinsics =
    {
        MAKE_SIGN_INTRINSICS(Sign)
    };
    for (auto fun : signIntrinsics)
    {
        this->intrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            uint32_t ext = g->AddExtension("GLSL.std.450");
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d Sign %%%d", returnType, ext, op, args[0].name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> ddxIntrinsics =
    {
        MAKE_FLOAT_INTRINSICS(DDX)
    };
    for (auto fun : ddxIntrinsics)
    {
        this->intrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            uint32_t ret = g->AddMappedOp(Format("OpDPdx %%%d %%%D", returnType, args[0].name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> ddyIntrinsics =
    {
        MAKE_FLOAT_INTRINSICS(DDY)
    };
    for (auto fun : ddyIntrinsics)
    {
        this->intrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            uint32_t ret = g->AddMappedOp(Format("OpDPdy %%%d %%%D", returnType, args[0].name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> fwidthIntrinsics =
    {
        MAKE_FLOAT_INTRINSICS(FWidth)
    };
    for (auto fun : fwidthIntrinsics)
    {
        this->intrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            uint32_t ret = g->AddMappedOp(Format("OpFwidth %%%d %%%D", returnType, args[0].name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    this->intrinsicMap[Intrinsics::GetOutputLayer] = [](Compiler* c, SPIRVGenerator * g, uint32_t returnType, const std::vector<SPIRVResult>&args) -> SPIRVResult{
        g->AddCapability("ShaderLayer");
        uint32_t baseType = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        uint32_t typePtr = g->AddSymbol("ptr(u32)Input", Format("OpTypePointer Input %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplOutputLayer", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.push_back(ret);
        g->AddDecoration("gplOutputLayer", ret, "BuiltIn Layer");
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Scope::Input);
    };

    this->intrinsicMap[Intrinsics::SetOutputLayer] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("ShaderLayer");
        uint32_t baseType = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        uint32_t typePtr = g->AddSymbol("ptr(u32)Output", Format("OpTypePointer Output %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplOutputLayer", Format("OpVariable %%%d Output", typePtr), true);
        g->interfaceVariables.push_back(ret);
        g->AddDecoration("gplOutputLayer", ret, "BuiltIn Layer");
        g->AddOp(Format("OpStore %%%d %%%d", ret, args[0].name));
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Scope::Output);
    };

    this->intrinsicMap[Intrinsics::GetOutputViewport] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("ShaderViewportIndex");
        uint32_t baseType = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        uint32_t typePtr = g->AddSymbol("ptr(u32)Input", Format("OpTypePointer Input %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplOutputViewport", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.push_back(ret);
        g->AddDecoration("gplOutputViewport", ret, "BuiltIn ViewportIndex");
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Scope::Input);
    };

    this->intrinsicMap[Intrinsics::SetOutputViewport] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("ShaderViewportIndex");
        uint32_t baseType = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        uint32_t typePtr = g->AddSymbol("ptr(u32)Input", Format("OpTypePointer Output %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplOutputViewport", Format("OpVariable %%%d Output", typePtr), true);
        g->interfaceVariables.push_back(ret);
        g->AddDecoration("gplOutputViewport", ret, "BuiltIn ViewportIndex");
        g->AddOp(Format("OpStore %%%d %%%d", ret, args[0].name));
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Scope::Output);
    };

    this->intrinsicMap[Intrinsics::ExportVertexCoordinates] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("Shader");
        uint32_t baseType = g->AddSymbol("f32", "OpTypeFloat 32", true);
        uint32_t vecType = g->AddSymbol("f32x4", Format("OpTypeVector %%%d 4", baseType), true);
        uint32_t typePtr = g->AddSymbol("ptr(f32x4)Input", Format("OpTypePointer Input %%%d", vecType), true);
        uint32_t ret = g->AddSymbol("gplVertexCoordinates", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.push_back(ret);
        g->AddDecoration("gplVertexCoordinates", ret, "BuiltIn Position");
        g->AddOp(Format("OpStore %%%d %%%d", ret, args[0].name));
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Scope::Input);
    };

    this->intrinsicMap[Intrinsics::GetVertexIndex] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("Shader");
        uint32_t baseType = g->AddSymbol("u32", "OpTypeInt 32 0");
        uint32_t typePtr = g->AddSymbol("ptr(u32)Input", Format("OpTypePointer Input %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplVertexIndex", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.push_back(ret);
        g->AddDecoration("gplVertexIndex", ret, "BuiltIn VertexId");
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Scope::Input);
    };

    this->intrinsicMap[Intrinsics::GetInstanceIndex] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("Shader");
        uint32_t baseType = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        uint32_t typePtr = g->AddSymbol("ptr(u32)Input", Format("OpTypePointer Input %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplInstanceIndex", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.push_back(ret);
        g->AddDecoration("gplInstanceIndex", ret, "BuiltIn InstanceId");
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Scope::Input);
    };

    this->intrinsicMap[Intrinsics::GetPixelCoordinates] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("Shader");
        uint32_t baseType = g->AddSymbol("f32", "OpTypeFloat 32", true);
        uint32_t vecType = g->AddSymbol("f32x4", Format("OpTypeVector %%%d 4", baseType), true);
        uint32_t typePtr = g->AddSymbol("ptr(f32x4)Input", Format("OpTypePointer Input %%%d", vecType), true);
        uint32_t ret = g->AddSymbol("gplPixelCoordinates", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.push_back(ret);
        g->AddDecoration("gplPixelCoordinates", ret, "BuiltIn FragCoord");
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Scope::Input);
    };

    this->intrinsicMap[Intrinsics::SetPixelDepth] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("Shader");
        uint32_t baseType = g->AddSymbol("f32", "OpTypeFloat 32", true);
        uint32_t vecType = g->AddSymbol("f32x4", Format("OpTypeVector %%%d 4", baseType), true);
        uint32_t typePtr = g->AddSymbol("ptr(f32x4)Output", Format("OpTypePointer Output %%%d", vecType), true);
        uint32_t ret = g->AddSymbol("gplPixelDepth", Format("OpVariable %%%d Output", typePtr), true);
        g->interfaceVariables.push_back(ret);
        g->AddDecoration("gplPixelDepth", ret, "BuiltIn FragDepth");
        g->AddOp(Format("OpStore %%%d %%%d", ret, args[0].name));
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Scope::Output);
    };

    this->intrinsicMap[Intrinsics::GetLocalInvocationIndices] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        uint32_t baseType = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        uint32_t vecType = g->AddSymbol("u32x3", Format("OpTypeVector %%%d 3", baseType), true);
        uint32_t typePtr = g->AddSymbol("ptr(u32x3)Input", Format("OpTypePointer Input %%%d", vecType), true);
        uint32_t ret = g->AddSymbol("gplLocalInvocationIndices", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.push_back(ret);
        g->AddDecoration("gplLocalInvocationIndices", ret, "BuiltIn LocalInvocationId");
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Scope::Input);
    };

    this->intrinsicMap[Intrinsics::GetGlobalInvocationIndices] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        uint32_t baseType = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        uint32_t vecType = g->AddSymbol("u32x3", Format("OpTypeVector %%%d 3", baseType), true);
        uint32_t typePtr = g->AddSymbol("ptr(u32x3)Input", Format("OpTypePointer Input %%%d", vecType), true);
        uint32_t ret = g->AddSymbol("gplGlobalInvocationIndices", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.push_back(ret);
        g->AddDecoration("gplGlobalInvocationIndices", ret, "BuiltIn GlobalInvocationId");
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Scope::Input);
    };

    this->intrinsicMap[Intrinsics::GetWorkGroupIndices] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        uint32_t baseType = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        uint32_t vecType = g->AddSymbol("u32x3", Format("OpTypeVector %%%d 3", baseType), true);
        uint32_t typePtr = g->AddSymbol("ptr(u32x3)Input", Format("OpTypePointer Input %%%d", vecType), true);
        uint32_t ret = g->AddSymbol("gplWorkGroupIndices", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.push_back(ret);
        g->AddDecoration("gplWorkGroupIndices", ret, "BuiltIn WorkgroupId");
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Scope::Input);
    };

    this->intrinsicMap[Intrinsics::GetWorkGroupDimensions] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        uint32_t baseType = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        uint32_t vecType = g->AddSymbol("u32x3", Format("OpTypeVector %%%d 3", baseType), true);
        uint32_t typePtr = g->AddSymbol("ptr(u32x3)Input", Format("OpTypePointer Input %%%d", vecType), true);
        uint32_t ret = g->AddSymbol("gplWorkGroupDimensions", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.push_back(ret);
        g->AddDecoration("gplWorkGroupDimensions", ret, "BuiltIn WorkgroupSize");
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Scope::Input);
    };

    this->intrinsicMap[Intrinsics::GetSubgroupId] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniform");
        uint32_t baseType = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        uint32_t vecType = g->AddSymbol("u32x3", Format("OpTypeVector %%%d 3", baseType), true);
        uint32_t typePtr = g->AddSymbol("ptr(u32x3)Input", Format("OpTypePointer Input %%%d", vecType), true);
        uint32_t ret = g->AddSymbol("gplSubgroupId", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.push_back(ret);
        g->AddDecoration("gplSubgroupId", ret, "BuiltIn SubgroupId");
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Scope::Input);
    };

    this->intrinsicMap[Intrinsics::GetSubgroupSize] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniform");
        uint32_t baseType = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        uint32_t vecType = g->AddSymbol("u32x3", Format("OpTypeVector %%%d 3", baseType), true);
        uint32_t typePtr = g->AddSymbol("ptr(u32x3)Input", Format("OpTypePointer Input %%%d", vecType), true);
        uint32_t ret = g->AddSymbol("gplSubgroupSize", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.push_back(ret);
        g->AddDecoration("gplSubgroupSize", ret, "BuiltIn SubgroupSize");
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Scope::Input);
    };

    this->intrinsicMap[Intrinsics::GetNumSubgroups] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniform");
        uint32_t baseType = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        uint32_t vecType = g->AddSymbol("u32x3", Format("OpTypeVector %%%d 3", baseType), true);
        uint32_t typePtr = g->AddSymbol("ptr(u32x3)Input", Format("OpTypePointer Input %%%d", vecType), true);
        uint32_t ret = g->AddSymbol("gplNumSubgroups", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.push_back(ret);
        g->AddDecoration("gplNumSubgroups", ret, "BuiltIn NumSubgroups");
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Scope::Input);
    };

    this->intrinsicMap[Intrinsics::GetSubgroupLocalInvocationMask] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniform");
        uint32_t baseType = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        uint32_t typePtr = g->AddSymbol("ptr(u32)Input", Format("OpTypePointer Input %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplSubgroupLocalInvocationMask", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.push_back(ret);
        g->AddDecoration("gplSubgroupLocalInvocationMask", ret, "BuiltIn SubgroupEqMask");
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Scope::Input);
    };

    this->intrinsicMap[Intrinsics::GetSubgroupLocalInvocationAndLowerMask] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniform");
        uint32_t baseType = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        uint32_t typePtr = g->AddSymbol("ptr(u32)Input", Format("OpTypePointer Input %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplSubgroupLocalInvocationAndLowerMask", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.push_back(ret);
        g->AddDecoration("gplSubgroupLocalInvocationAndLowerMask", ret, "BuiltIn SubgroupLeMask");
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Scope::Input);
    };

    this->intrinsicMap[Intrinsics::GetSubgroupLowerMask] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniform");
        uint32_t baseType = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        uint32_t typePtr = g->AddSymbol("ptr(u32)Input", Format("OpTypePointer Input %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplNumSubgroups", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.push_back(ret);
        g->AddDecoration("gplSubgroupLowerMask", ret, "BuiltIn SubgroupLtMask");
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Scope::Input);
    };

    this->intrinsicMap[Intrinsics::GetSubgroupLocalInvocationAndGreaterMask] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniform");
        uint32_t baseType = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        uint32_t typePtr = g->AddSymbol("ptr(u32)Input", Format("OpTypePointer Input %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplSubgroupLocalInvocationAndGreaterMask", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.push_back(ret);
        g->AddDecoration("gplSubgroupLocalInvocationAndGreaterMask", ret, "BuiltIn SubgroupGeMask");
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Scope::Input);
    };

    this->intrinsicMap[Intrinsics::GetSubgroupGreaterMask] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniform");
        uint32_t baseType = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        uint32_t typePtr = g->AddSymbol("ptr(u32)Input", Format("OpTypePointer Input %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplSubgroupGreaterMask", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.push_back(ret);
        g->AddDecoration("gplSubgroupGreaterMask", ret, "BuiltIn SubgroupGtMask");
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Scope::Input);
    };

    this->intrinsicMap[Intrinsics::SubgroupFirstInvocation] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniform");
        uint32_t ret = g->AddMappedOp(Format("OpGroupNonUniformElect %%%d Subgroup", returnType));
        return SPIRVResult(ret, returnType);
    };

    this->intrinsicMap[Intrinsics::SubgroupRead] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniformBallot");
        uint32_t ret = g->AddMappedOp(Format("OpGroupNonUniformBroadcastFirst %%%d Subgroup %%%d", returnType, args[0].name));
        return SPIRVResult(ret, returnType);
    };

    this->intrinsicMap[Intrinsics::SubgroupBallot] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniformBallot");
        uint32_t ret = g->AddMappedOp(Format("OpGroupNonUniformBallot %%%d Subgroup %%%d", returnType, args[0].name));
        return SPIRVResult(ret, returnType);
    };

    this->intrinsicMap[Intrinsics::SubgroupInverseBallot] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniformBallot");
        uint32_t ret = g->AddMappedOp(Format("OpGroupNonUniformInverseBallot %%%d Subgroup %%%d", returnType, args[0].name));
        return SPIRVResult(ret, returnType);
    };

    this->intrinsicMap[Intrinsics::SubgroupBallotBitCount] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniformBallot");
        uint32_t ret = g->AddMappedOp(Format("OpGroupNonUniformBallotBitCount %%%d Subgroup Reduce %%%d", returnType, args[0].name));
        return SPIRVResult(ret, returnType);
    };

    this->intrinsicMap[Intrinsics::SubgroupBallotBit] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniformBallot");
        uint32_t ret = g->AddMappedOp(Format("OpGroupNonUniformBallotBitExtract %%%d Subgroup %%%d %%%d", returnType, args[0].name, args[1].name));
        return SPIRVResult(ret, returnType);
    };

    this->intrinsicMap[Intrinsics::SubgroupBallotFirstOne] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniformBallot");
        uint32_t ret = g->AddMappedOp(Format("OpGroupNonUniformBallotFindLSB %%%d Subgroup %%%d", returnType, args[0].name));
        return SPIRVResult(ret, returnType);
    };

    this->intrinsicMap[Intrinsics::SubgroupBallotLastOne] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniformBallot");
        uint32_t ret = g->AddMappedOp(Format("OpGroupNonUniformBallotFindMSB %%%d Subgroup %%%d", returnType, args[0].name));
        return SPIRVResult(ret, returnType);
    };

    enum SampleOperands : uint8_t
    {
        None = 0x0,
        Lod = 0x1,
        Bias = 0x2,
        Grad = 0x4,
        Offset = 0x8,
        Proj = 0x10,
        Comp = 0x20,

        LodComp = Lod | Comp,
        LodCompOffset = Lod | Comp | Offset,
        LodOffset = Lod | Offset,
        LodProj = Lod | Proj,
        LodProjComp = Lod | Proj | Comp,
        LodProjCompOffset = Lod | Proj | Comp | Offset,
        LodProjOffset = Lod | Proj | Offset,

        BiasComp = Bias | Comp,
        BiasOffset = Bias | Offset,
        BiasProj = Bias | Proj,
        BiasProjComp = Bias | Proj | Comp,
        BiasProjOffset = Bias | Proj | Offset,

        CompOffset = Comp | Offset,

        GradComp = Grad | Comp,
        GradOffset = Grad | Offset,
        GradProj = Grad | Proj,
        GradProjComp = Grad | Proj | Comp,
        GradProjCompOffset = Grad | Proj | Comp | Offset,
        GradProjOffset = Grad | Proj | Offset,

        ProjComp = Proj | Comp,
        ProjCompOffset = Proj | Comp | Offset,
        ProjOffset = Proj | Offset,
    };

#define MAKE_TEXTURE_SAMPLE_INTRINSICS(op, operands)\
      { Intrinsics::TextureSample##op##_texture1D, SampleOperands::##operands }\
    , { Intrinsics::TextureSample##op##_texture2D, SampleOperands::##operands }\
    , { Intrinsics::TextureSample##op##_texture2DArray, SampleOperands::##operands }\
    , { Intrinsics::TextureSample##op##_texture2DMS, SampleOperands::##operands }\
    , { Intrinsics::TextureSample##op##_texture2DMSArray, SampleOperands::##operands }\
    , { Intrinsics::TextureSample##op##_texture3D, SampleOperands::##operands }\
    , { Intrinsics::TextureSample##op##_textureCube, SampleOperands::##operands }\
    , { Intrinsics::TextureSample##op##_textureCubeArray, SampleOperands::##operands }

#define MAKE_TEXTURE_SAMPLE_INTRINSICS_NO_CUBE(op, operands)\
      { Intrinsics::TextureSample##op##_texture1D, SampleOperands::##operands }\
    , { Intrinsics::TextureSample##op##_texture2D, SampleOperands::##operands }\
    , { Intrinsics::TextureSample##op##_texture2DArray, SampleOperands::##operands }\
    , { Intrinsics::TextureSample##op##_texture2DMS, SampleOperands::##operands }\
    , { Intrinsics::TextureSample##op##_texture2DMSArray, SampleOperands::##operands }\
    , { Intrinsics::TextureSample##op##_texture3D, SampleOperands::##operands }\

#define MAKE_TEXTURE_SAMPLE_INTRINSICS_NO_MS(op, operands)\
      { Intrinsics::TextureSample##op##_texture1D, SampleOperands::##operands }\
    , { Intrinsics::TextureSample##op##_texture2D, SampleOperands::##operands }\
    , { Intrinsics::TextureSample##op##_texture2DArray, SampleOperands::##operands }\
    , { Intrinsics::TextureSample##op##_texture3D, SampleOperands::##operands }\
    , { Intrinsics::TextureSample##op##_textureCube, SampleOperands::##operands }\
    , { Intrinsics::TextureSample##op##_textureCubeArray, SampleOperands::##operands }

#define MAKE_TEXTURE_SAMPLE_INTRINSICS_NO_ARRAY_NO_MS(op, operands)\
      { Intrinsics::TextureSample##op##_texture1D, SampleOperands::##operands }\
    , { Intrinsics::TextureSample##op##_texture2D, SampleOperands::##operands }\
    , { Intrinsics::TextureSample##op##_texture3D, SampleOperands::##operands }\
    , { Intrinsics::TextureSample##op##_textureCube, SampleOperands::##operands }\

#define MAKE_TEXTURE_SAMPLE_INTRINSICS_NO_CUBE_NO_MS(op, operands)\
      { Intrinsics::TextureSample##op##_texture1D, SampleOperands::##operands }\
    , { Intrinsics::TextureSample##op##_texture2D, SampleOperands::##operands }\
    , { Intrinsics::TextureSample##op##_texture2DArray, SampleOperands::##operands }\
    , { Intrinsics::TextureSample##op##_texture3D, SampleOperands::##operands }\

    std::vector<std::tuple<Function*, uint32_t>> textureSampleInstructions =
    {
        MAKE_TEXTURE_SAMPLE_INTRINSICS(Base, None),
        MAKE_TEXTURE_SAMPLE_INTRINSICS(Lod, Lod),
        MAKE_TEXTURE_SAMPLE_INTRINSICS(LodCompare, LodComp),
        MAKE_TEXTURE_SAMPLE_INTRINSICS_NO_CUBE(LodOffset, LodOffset),
        MAKE_TEXTURE_SAMPLE_INTRINSICS_NO_ARRAY_NO_MS(LodProj, LodProj),
        MAKE_TEXTURE_SAMPLE_INTRINSICS_NO_ARRAY_NO_MS(LodProjCompare, LodProjComp),
        MAKE_TEXTURE_SAMPLE_INTRINSICS_NO_ARRAY_NO_MS(LodProjCompareOffset, LodProjCompOffset),
        MAKE_TEXTURE_SAMPLE_INTRINSICS_NO_ARRAY_NO_MS(LodProjOffset, LodProjOffset),
        MAKE_TEXTURE_SAMPLE_INTRINSICS_NO_MS(Bias, Bias),
        MAKE_TEXTURE_SAMPLE_INTRINSICS_NO_MS(BiasCompare, BiasComp),
        MAKE_TEXTURE_SAMPLE_INTRINSICS_NO_MS(BiasOffset, BiasOffset),
        MAKE_TEXTURE_SAMPLE_INTRINSICS_NO_ARRAY_NO_MS(BiasProj, BiasProj),
        MAKE_TEXTURE_SAMPLE_INTRINSICS_NO_ARRAY_NO_MS(BiasProjCompare, BiasProjComp),
        MAKE_TEXTURE_SAMPLE_INTRINSICS_NO_ARRAY_NO_MS(BiasProjOffset, BiasProjOffset),
        MAKE_TEXTURE_SAMPLE_INTRINSICS(Compare, Comp),
        MAKE_TEXTURE_SAMPLE_INTRINSICS_NO_CUBE(CompareOffset, CompOffset),
        MAKE_TEXTURE_SAMPLE_INTRINSICS_NO_MS(Grad, Grad),
        MAKE_TEXTURE_SAMPLE_INTRINSICS_NO_CUBE_NO_MS(GradCompare, GradComp),
        MAKE_TEXTURE_SAMPLE_INTRINSICS_NO_CUBE_NO_MS(GradOffset, GradOffset),
        MAKE_TEXTURE_SAMPLE_INTRINSICS_NO_ARRAY_NO_MS(GradProj, GradProj),
        MAKE_TEXTURE_SAMPLE_INTRINSICS_NO_ARRAY_NO_MS(GradProjCompare, GradProjComp),
        MAKE_TEXTURE_SAMPLE_INTRINSICS_NO_ARRAY_NO_MS(GradProjCompareOffset, GradProjCompOffset),
        MAKE_TEXTURE_SAMPLE_INTRINSICS_NO_ARRAY_NO_MS(GradProjOffset, GradProjOffset),
        MAKE_TEXTURE_SAMPLE_INTRINSICS_NO_ARRAY_NO_MS(Proj, Proj),
        MAKE_TEXTURE_SAMPLE_INTRINSICS_NO_ARRAY_NO_MS(ProjCompare, ProjComp),
        MAKE_TEXTURE_SAMPLE_INTRINSICS_NO_ARRAY_NO_MS(ProjCompareOffset, ProjCompOffset),
        MAKE_TEXTURE_SAMPLE_INTRINSICS_NO_ARRAY_NO_MS(ProjOffset, ProjOffset)
    };
    for (auto fun : textureSampleInstructions)
    {
        this->intrinsicMap[std::get<0>(fun)] = [operands = std::get<1>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            uint32_t ret = 0xFFFFFFFF;
            if (operands & Lod || operands & Grad)
                if (operands & Comp)
                    if (operands & Grad)
                        if (operands & Proj)
                            if (operands & Bias)
                                ret = g->AddMappedOp(Format("OpImageSampleProjDrefExplicitLod %%%d %%%d %%%d %%%d Grad %%%d Bias %%%d", returnType, args[0].name, args[1].name, args[2].name, args[3].name, args[4].name));
                            else
                                ret = g->AddMappedOp(Format("OpImageSampleProjDrefExplicitLod %%%d %%%d %%%d %%%d Grad %%%d", returnType, args[0].name, args[1].name, args[2].name, args[3].name));
                        else
                            if (operands & Bias)
                                ret = g->AddMappedOp(Format("OpImageSampleDrefExplicitLod %%%d %%%d %%%d %%%d Grad %%%d Bias", returnType, args[0].name, args[1].name, args[2].name, args[3].name, args[4].name));
                            else
                                ret = g->AddMappedOp(Format("OpImageSampleDrefExplicitLod %%%d %%%d %%%d %%%d Grad %%%d", returnType, args[0].name, args[1].name, args[2].name, args[3].name));
                    else
                        if (operands & Proj)
                            if (operands & Bias)
                                ret = g->AddMappedOp(Format("OpImageSampleProjDrefExplicitLod %%%d %%%d %%%d %%%d Lod %%%d Bias", returnType, args[0].name, args[1].name, args[2].name, args[3].name, args[4].name));
                            else
                                ret = g->AddMappedOp(Format("OpImageSampleProjDrefExplicitLod %%%d %%%d %%%d %%%d Lod %%%d", returnType, args[0].name, args[1].name, args[2].name, args[3].name));
                        else
                            if (operands & Bias)
                                ret = g->AddMappedOp(Format("OpImageSampleDrefExplicitLod %%%d %%%d %%%d %%%d Lod %%%d Bias %%%d", returnType, args[0].name, args[1].name, args[2].name, args[3].name, args[4].name));
                            else
                                ret = g->AddMappedOp(Format("OpImageSampleDrefExplicitLod %%%d %%%d %%%d %%%d Lod %%%d", returnType, args[0].name, args[1].name, args[2].name, args[3].name));
                else
                    if (operands & Grad)
                        if (operands & Proj)
                            if (operands & Bias)
                                ret = g->AddMappedOp(Format("OpImageSampleProjExplicitLod %%%d %%%d %%%d Grad %%%d Bias %%%d", returnType, args[0].name, args[1].name, args[2].name, args[3].name));
                            else
                                ret = g->AddMappedOp(Format("OpImageSampleProjExplicitLod %%%d %%%d %%%d Grad %%%d", returnType, args[0].name, args[1].name, args[2].name));
                        else
                            if (operands & Bias)
                                ret = g->AddMappedOp(Format("OpImageSampleExplicitLod %%%d %%%d %%%d Grad %%%d Bias %%%d", returnType, args[0].name, args[1].name, args[2].name, args[3].name));
                            else
                                ret = g->AddMappedOp(Format("OpImageSampleExplicitLod %%%d %%%d %%%d Grad %%%d", returnType, args[0].name, args[1].name, args[2].name));
                    else
                        if (operands & Proj)
                            if (operands & Bias)
                                ret = g->AddMappedOp(Format("OpImageSampleProjExplicitLod %%%d %%%d %%%d Lod %%%d Bias %%%d", returnType, args[0].name, args[1].name, args[2].name, args[3].name));
                            else
                                ret = g->AddMappedOp(Format("OpImageSampleProjExplicitLod %%%d %%%d %%%d Lod %%%d", returnType, args[0].name, args[1].name, args[2].name));
                        else
                            if (operands & Bias)
                                ret = g->AddMappedOp(Format("OpImageSampleExplicitLod %%%d %%%d %%%d Lod %%%d Bias %%%d", returnType, args[0].name, args[1].name, args[2].name, args[3].name));
                            else
                                ret = g->AddMappedOp(Format("OpImageSampleExplicitLod %%%d %%%d %%%d Lod %%%d", returnType, args[0].name, args[1].name, args[2].name));
            else
                if (operands & Comp)
                    if (operands & Proj)
                        if (operands & Bias)
                            ret = g->AddMappedOp(Format("OpImageSampleProjDrefImplicitLod %%%d %%%d %%%d %%%d Bias %%%d", returnType, args[0].name, args[1].name, args[2].name, args[3].name));
                        else
                            ret = g->AddMappedOp(Format("OpImageSampleProjDrefImplicitLod %%%d %%%d %%%d %%%d None", returnType, args[0].name, args[1].name, args[2].name));
                    else
                        if (operands & Bias)
                            ret = g->AddMappedOp(Format("OpImageSampleDrefImplicitLod %%%d %%%d %%%d %%%d Bias", returnType, args[0].name, args[1].name, args[2].name, args[3].name));
                        else
                            ret = g->AddMappedOp(Format("OpImageSampleDrefImplicitLod %%%d %%%d %%%d %%%d None", returnType, args[0].name, args[1].name, args[2].name));
                else
                    if (operands & Proj)
                        if (operands & Bias)
                            ret = g->AddMappedOp(Format("OpImageSampleProjImplicitLod %%%d %%%d %%%d Bias %%%d", returnType, args[0].name, args[1].name, args[2].name));
                        else
                            ret = g->AddMappedOp(Format("OpImageSampleProjImplicitLod %%%d %%%d %%%d None", returnType, args[0].name, args[1].name));
                    else
                        if (operands & Bias)
                            ret = g->AddMappedOp(Format("OpImageSampleImplicitLod %%%d %%%d %%%d Bias %%%d", returnType, args[0].name, args[1].name, args[2].name));
                        else
                            ret = g->AddMappedOp(Format("OpImageSampleImplicitLod %%%d %%%d %%%d None", returnType, args[0].name, args[1].name));

            return SPIRVResult(ret, returnType);
        };
    }
}

//------------------------------------------------------------------------------
/**
*/
void
GenerateFunctionSPIRV(Compiler* compiler, SPIRVGenerator* generator, Symbol* symbol)
{
    Function* func = static_cast<Function*>(symbol);
    Function::__Resolved* funcResolved = Symbol::Resolved(func);

    if (funcResolved->isPrototype)
        return;
    SPIRVResult returnName = GenerateTypeSPIRV(compiler, generator, func->returnType, static_cast<Type*>(funcResolved->returnTypeSymbol));
    std::string typeArgs = "";
    for (auto& param : func->parameters)
    {
        Variable::__Resolved* paramResolved = Symbol::Resolved(param);
        SPIRVResult typeName = GenerateTypeSPIRV(compiler, generator, paramResolved->type, paramResolved->typeSymbol);
        typeArgs.append(Format("%%%d", typeName.typeName));

        if (funcResolved->isEntryPoint)
        {
            GenerateVariableSPIRV(compiler, generator, param, true, false);
        }
    }
    uint32_t functionType = generator->AddSymbol(Format("type(%s)", funcResolved->name.c_str()), Format("OpTypeFunction %%%d %s", returnName.typeName, typeArgs.c_str()), true);

    // TODO: Add inline/const/functional
    generator->AddSymbol(funcResolved->name, Format("OpFunction %%%d None %%%d", returnName.typeName, functionType));
    for (auto& param : func->parameters)
    {
        Variable::__Resolved* paramResolved = Symbol::Resolved(param);
        SPIRVResult varType = GenerateTypeSPIRV(compiler, generator, paramResolved->type, paramResolved->typeSymbol);
        generator->AddSymbol(param->name, Format("OpFunctionParameter %%%d", varType.typeName));
    }
    uint32_t label = generator->AddMappedOp("OpLabel");
    generator->blockOpen = true;
    generator->functions.append(generator->functional);


    generator->functional.clear();
    GenerateStatementSPIRV(compiler, generator, func->ast);
    generator->functions.append(generator->variableDeclarations);
    if (generator->blockOpen)
    {
        if (!funcResolved->hasExplicitReturn)
            generator->AddOp("OpReturn");
        else
            generator->AddOp("OpUnreachable");
    }
    generator->blockOpen = false;
    generator->AddOp("OpFunctionEnd", false, Format("End of %s", func->name.c_str()));

    generator->functions.append(generator->functional);
    generator->functional.clear();
}

//------------------------------------------------------------------------------
/**
*/
void
GenerateStructureSPIRV(Compiler* compiler, SPIRVGenerator* generator, Symbol* symbol)
{
    Structure* struc = static_cast<Structure*>(symbol);
    Structure::__Resolved* strucResolved = Symbol::Resolved(struc);

    uint32_t numVariables = 0;
    for (Symbol* sym : struc->symbols)
    {
        if (sym->symbolType == Symbol::SymbolType::VariableType)
            numVariables++;
    }
    uint32_t name = 0xFFFFFFFF;
    uint32_t* varNames = (uint32_t*)alloca(sizeof(uint32_t) * numVariables);
    uint32_t structName = generator->ReserveName();

    uint32_t offset = 0;
    std::string memberTypes = "";
    for (size_t i = 0; i < struc->symbols.size(); i++)
    {
        Symbol* sym = struc->symbols[i];
        if (sym->symbolType == Symbol::SymbolType::VariableType)
        {
            Variable* var = static_cast<Variable*>(sym);
            Variable::__Resolved* varResolved = Symbol::Resolved(var);
            SPIRVResult varType = GenerateTypeSPIRV(compiler, generator, varResolved->type, varResolved->typeSymbol);
            memberTypes.append(Format("%%%d ", varType.typeName));
            offset = Type::Align(offset, varResolved->typeSymbol->CalculateAlignment());
            generator->AddMemberDecoration(structName, i, Format("Offset %d", offset));
            offset += varResolved->typeSymbol->CalculateSize();
        }
    }
    generator->AddReservedSymbol(struc->name, structName, Format("OpTypeStruct %s", memberTypes.c_str()), true);
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateVariableSPIRV(Compiler* compiler, SPIRVGenerator* generator, Symbol* symbol, bool isShaderArgument, bool isGlobal)
{
    Variable* var = static_cast<Variable*>(symbol);
    Variable::__Resolved* varResolved = static_cast<Variable::__Resolved*>(var->resolved);

    SPIRVResult initializer = SPIRVResult::Invalid();
    if (varResolved->value != nullptr)
    {
        // Setup initializer
        initializer = GenerateExpressionSPIRV(compiler, generator, varResolved->value);
    }

    SPIRVResult typeName = GenerateTypeSPIRV(compiler, generator, varResolved->type, varResolved->typeSymbol, varResolved->imageFormat, varResolved->accessBits, varResolved->usageBits);
    std::string type = varResolved->type.name;
    std::string scope = SPIRVResult::ScopeToString(typeName.scope);

    type = Format("ptr(%s)", type.c_str());
    uint32_t typePtrName = generator->AddSymbol(Format("%s%s", type.c_str(), scope.c_str()), Format("OpTypePointer %s %%%d", scope.c_str(), typeName.typeName), true);

    uint32_t name = 0xFFFFFFFF;
    if (initializer != SPIRVResult::Invalid())
        name = generator->AddSymbol(varResolved->name, Format("OpVariable %%%d %s %%%d", typePtrName, scope.c_str(), initializer.name), isGlobal);
    else
        name = generator->AddSymbol(varResolved->name, Format("OpVariable %%%d %s", typePtrName, scope.c_str()), isGlobal);

    if (typeName.scope == SPIRVResult::Scope::StorageBuffer || typeName.scope == SPIRVResult::Scope::Uniform || typeName.scope == SPIRVResult::Scope::PushConstant)
    {
        uint32_t structSymbol = generator->GetSymbol(varResolved->typeSymbol->name);
        generator->AddDecoration(Format("Block(%s)", varResolved->typeSymbol->name.c_str()), structSymbol, "Block");
        generator->AddDecoration(Format("Set(%s)", varResolved->name.c_str()), name, Format("DescriptorSet %d", varResolved->group));
        generator->AddDecoration(Format("Binding(%s)", varResolved->name.c_str()), name, Format("Binding %d", varResolved->group));
        generator->interfaceVariables.push_back(name);
    }
    else if (typeName.scope == SPIRVResult::Scope::Input || typeName.scope == SPIRVResult::Scope::Output)
    {
        generator->AddDecoration(Format("Location(%s)", varResolved->name.c_str()), name, Format("Location %d", varResolved->inBinding));
        if (varResolved->parameterBits.flags.isNoInterpolate)
            generator->AddDecoration(Format("NoInterpolate(%s)", varResolved->name.c_str()), name, "Flat");
        if (varResolved->parameterBits.flags.isNoPerspective)
            generator->AddDecoration(Format("NoPerspective(%s)", varResolved->name.c_str()), name, "NoPerspective");
        if (varResolved->parameterBits.flags.isPatch)
            generator->AddDecoration(Format("Patch(%s)", varResolved->name.c_str()), name, "Patch");
        if (varResolved->parameterBits.flags.isCentroid)
            generator->AddDecoration(Format("Centroid(%s)", varResolved->name.c_str()), name, "Centroid");
        generator->interfaceVariables.push_back(name);
    }
    return SPIRVResult(name, typeName.typeName, false, false, typeName.scope);
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateCallExpressionSPIRV(Compiler* compiler, SPIRVGenerator* generator, Expression* expr)
{
    CallExpression* callExpression = static_cast<CallExpression*>(expr);
    CallExpression::__Resolved* resolvedCall = Symbol::Resolved(callExpression);

    Function::__Resolved* resolvedFunction = Symbol::Resolved(resolvedCall->function);
    SPIRVResult returnTypeName = GenerateTypeSPIRV(compiler, generator, resolvedCall->function->returnType, resolvedFunction->returnTypeSymbol);

    uint32_t funName;
    if (generator->HasSymbol(resolvedFunction->name))
    {
        funName = generator->GetSymbol(resolvedFunction->name);

        // Create arg list from argument expressions
        std::string argList = "";
        for (const auto& param : callExpression->args)
        {
            SPIRVResult arg = GenerateExpressionSPIRV(compiler, generator, param);
            argList.append(Format("%%%d ", arg.name));
        }

        // Then call the function
        return SPIRVResult(generator->AddMappedOp(Format("OpFunctionCall %%%d %%%d %s", returnTypeName.typeName, funName, argList.c_str())), returnTypeName.typeName);
    }
    else
    {
        // If there is no symbol matching this function, then we assume it's an intrinsic function
        std::vector<SPIRVResult> args;

        // First step is to get the type of all parameters, and get their SSA mappings
        for (const auto& param : callExpression->args)
        {
            SPIRVResult arg = GenerateExpressionSPIRV(compiler, generator, param);
            args.push_back(arg);
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
                SPIRVResult returnTypeName = GenerateTypeSPIRV(compiler, generator, converter->returnType, converterResolved->returnTypeSymbol);

                std::vector<SPIRVResult> tempArgs{ args[i] };

                // After conversion the type is changed and the SSA is the value of the conversion being done
                args[i] = conv->second(compiler, generator, returnTypeName.typeName, tempArgs);
            }
        }

        // Function is not declared by user code, so must be intrinsic
        auto it = generator->intrinsicMap.find(resolvedCall->function);
        assert(it != generator->intrinsicMap.end());

        // Lastly, run the intrinsic generation with the converted parameters
        return it->second(compiler, generator, returnTypeName.typeName, args);
    }
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateArrayIndexExpressionSPIRV(Compiler* compiler, SPIRVGenerator* generator, Expression* expr)
{
    ArrayIndexExpression* arrayIndexExpression = static_cast<ArrayIndexExpression*>(expr);
    ArrayIndexExpression::__Resolved* arrayIndexExpressionResolved = Symbol::Resolved(arrayIndexExpression);
    SPIRVResult leftSymbol = GenerateExpressionSPIRV(compiler, generator, arrayIndexExpression->left);

    Type::FullType leftType, rightType;
    arrayIndexExpression->left->EvalType(leftType);
    arrayIndexExpression->right->EvalType(rightType);

    uint32_t index;
    bool res = arrayIndexExpression->right->EvalUInt(index);
    assert(res);

    SPIRVResult returnType = GenerateTypeSPIRV(compiler, generator, arrayIndexExpressionResolved->returnFullType, arrayIndexExpressionResolved->returnType);

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

            uint32_t returnPtrType = generator->AddSymbol(Format("ptr(%s)Function", arrayIndexExpressionResolved->returnFullType.ToString().c_str()), Format("OpTypePointer Function %%%d", returnType.typeName), true);
            SPIRVResult indexConstant = GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::UInt32(index));

            /// Generate array access
            return intrin->second(compiler, generator, returnPtrType, { leftSymbol, indexConstant });
        }
    }
    else
    {

    }
    return SPIRVResult::Invalid();
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateInitializerExpressionSPIRV(Compiler* compiler, SPIRVGenerator* generator, Expression* expr)
{
    uint32_t name = 0xFFFFFFFF;
    InitializerExpression* initExpression = static_cast<InitializerExpression*>(expr);

    std::vector<SPIRVResult> composites;
    for (Expression* expr : initExpression->values)
    {
        SPIRVResult value = GenerateExpressionSPIRV(compiler, generator, expr);
        composites.push_back(value);
    }
    Type::FullType type;
    initExpression->values[0]->EvalType(type);
    Symbol* typeSymbol = compiler->GetSymbol(type.name);
    SPIRVResult typeName = GenerateTypeSPIRV(compiler, generator, type, static_cast<Type*>(typeSymbol));
    SPIRVResult sizeName = GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::UInt32(initExpression->values.size()));
    typeName.typeName = generator->AddSymbol(Format("%s[%d]", type.name.c_str(), initExpression->values.size()), Format("OpTypeArray %%%d %%%d", typeName.typeName, sizeName.name), true);
    std::string initializer = "";
    bool isLiteral = true;
    for (SPIRVResult component : composites)
    {
        if (!component.isValue)
        {
            uint32_t loadedComponent = generator->AddMappedOp(Format("OpLoad %%%d %%%d", component.typeName, component.name));
            initializer.append(Format("%%%d ", loadedComponent));
        }
        else
        {
            initializer.append(Format("%%%d ", component.name));
        }
        isLiteral &= component.isLiteral;
    }

    if (isLiteral)
        name = generator->AddSymbol(Format("{%s}", initializer.c_str()), Format("OpConstantComposite %%%d %s", typeName.typeName, initializer.c_str()), true);
    else
        name = generator->AddSymbol(Format("{%s}", initializer.c_str()), Format("OpCompositeConstruct %%%d %s", typeName.typeName, initializer.c_str()));
    return SPIRVResult(name, typeName.typeName, true, isLiteral);
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateBinaryExpressionSPIRV(Compiler* compiler, SPIRVGenerator* generator, Expression* expr)
{
    BinaryExpression* binaryExpression = static_cast<BinaryExpression*>(expr);
    BinaryExpression::__Resolved* binaryExpressionResolved = Symbol::Resolved(binaryExpression);
    Type::FullType leftType, rightType;
    binaryExpression->left->EvalType(leftType);
    binaryExpression->right->EvalType(rightType);

    SPIRVResult rightValue = GenerateExpressionSPIRV(compiler, generator, binaryExpression->right);
    SPIRVResult leftValue = GenerateExpressionSPIRV(compiler, generator, binaryExpression->left);

    if (!rightValue.isValue)
    {
        rightValue.name = generator->AddMappedOp(Format("OpLoad %%%d %%%d", rightValue.typeName, rightValue.name));
        rightValue.isValue = true;
    }

    // If there is a conversion function, generate it first
    if (binaryExpressionResolved->conversionFunction)
    {
        auto it = generator->intrinsicMap.find(binaryExpressionResolved->conversionFunction);
        assert(it != generator->intrinsicMap.end());
        rightValue = it->second(compiler, generator, leftValue.typeName, { rightValue });
        rightType = binaryExpressionResolved->conversionFunction->returnType;
    }

    SPIRVResult opResult = SPIRVResult::Invalid();
    if (leftType == rightType && binaryExpression->op == '=')
    {
        assert(!leftValue.isValue);
        generator->AddOp(Format("OpStore %%%d %%%d", leftValue.name, rightValue.name));
        opResult = leftValue;
    }
    else
    {

        std::string functionName = Format("operator%s(%s)", FourCCToString(binaryExpression->op).c_str(), leftType.name.c_str());
        Function* fun = binaryExpressionResolved->lhsType->GetSymbol<Function>(functionName);
        Function::__Resolved* funResolved = Symbol::Resolved(fun);
        assert(fun != nullptr);

        SPIRVResult retType = GenerateTypeSPIRV(compiler, generator, fun->returnType, funResolved->returnTypeSymbol);
        if (!leftValue.isValue)
        {
            leftValue.name = generator->AddMappedOp(Format("OpLoad %%%d %%%d", leftValue.typeName, leftValue.name));
            leftValue.isValue = true;
        }

        // Get operator 
        auto op = generator->intrinsicMap.find(fun);
        assert(op != generator->intrinsicMap.end());
        opResult = op->second(compiler, generator, retType.typeName, { leftValue, rightValue, leftValue });
    }

    return opResult;
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateAccessExpressionSPIRV(Compiler* compiler, SPIRVGenerator* generator, Expression* expr)
{
    AccessExpression* accessExpression = static_cast<AccessExpression*>(expr);
    AccessExpression::__Resolved* accessExpressionResolved = Symbol::Resolved(accessExpression);
    SPIRVResult lhs = GenerateExpressionSPIRV(compiler, generator, accessExpression->left);
    std::string scopeName = SPIRVResult::ScopeToString(lhs.scope);
    Type::SwizzleMask swizzle = accessExpressionResolved->swizzleMask;
    if (swizzle.mask != 0x0)
    {
        // If single value, then use an access chain
        if (Type::SwizzleMaskComponents(swizzle) == 1)
        {
            SPIRVResult indexName = GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::UInt32(swizzle.bits.x));
            SPIRVResult typeName = GenerateTypeSPIRV(compiler, generator, accessExpressionResolved->rightType, accessExpressionResolved->rhsType);
            uint32_t ptrTypeName = generator->AddSymbol(Format("ptr(%s)%s", accessExpressionResolved->rightType.name.c_str(), scopeName.c_str()), Format("OpTypePointer %s %%%d", scopeName.c_str(), typeName.typeName), true);
            uint32_t ptr = generator->AddMappedOp(Format("OpAccessChain %%%d %%%d %%%d", ptrTypeName, lhs.name, indexName.name), accessExpressionResolved->text);
            return SPIRVResult(ptr, typeName.typeName);
        }
        else
        {
            assert(accessExpressionResolved->lhsType->IsVector());
            SPIRVResult retType = GenerateTypeSPIRV(compiler, generator, accessExpressionResolved->rightType, accessExpressionResolved->rhsType);
            SPIRVResult origType = GenerateTypeSPIRV(compiler, generator, accessExpressionResolved->leftType, accessExpressionResolved->lhsType);
            std::string swizzleIndices;
            if (swizzle.bits.x != Type::SwizzleMask::Invalid)
                swizzleIndices.append(Format("%d ", swizzle.bits.x));
            if (swizzle.bits.y != Type::SwizzleMask::Invalid)
                swizzleIndices.append(Format("%d ", swizzle.bits.y));
            if (swizzle.bits.z != Type::SwizzleMask::Invalid)
                swizzleIndices.append(Format("%d ", swizzle.bits.z));
            if (swizzle.bits.w != Type::SwizzleMask::Invalid)
                swizzleIndices.append(Format("%d ", swizzle.bits.w));
            uint32_t vec = generator->AddMappedOp(Format("OpLoad %%%d %%%d", origType.typeName, lhs.name), Format("Intermediate '%s'", accessExpressionResolved->leftType.name.c_str()));
            uint32_t shuffledVec = generator->AddMappedOp(Format("OpVectorShuffle %%%d %%%d %%%d %s", retType.typeName, vec, vec, swizzleIndices.c_str()), accessExpressionResolved->text);
            return SPIRVResult(shuffledVec, retType.typeName, true);
        }
        // Generate swizzle access
    }
    else
    {
        // Otherwise, find offset of member
        Type::FullType lhsType;
        accessExpression->left->EvalType(lhsType);
        Type* rhsSymbol = compiler->GetSymbol<Type>(lhsType.name);

        if (rhsSymbol->symbolType == Symbol::StructureType)
        {
            for (size_t i = 0; i < rhsSymbol->symbols.size(); i++)
            {
                Symbol* sym = rhsSymbol->symbols[i];
                if (sym->name == accessExpressionResolved->rightSymbol)
                {
                    SPIRVResult indexName = GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::UInt32(i));
                    SPIRVResult typeName = GenerateTypeSPIRV(compiler, generator, accessExpressionResolved->rightType, accessExpressionResolved->rhsType);
                    uint32_t ptrTypeName = generator->AddSymbol(Format("ptr(%s)%s", accessExpressionResolved->rightType.name.c_str(), scopeName.c_str()), Format("OpTypePointer %s %%%d", scopeName.c_str(), typeName.typeName), true);
                    return SPIRVResult(generator->AddMappedOp(Format("OpAccessChain %%%d %%%d %%%d", ptrTypeName, lhs.name, indexName.name), accessExpressionResolved->text), typeName.typeName);
                }
            }
        }
    }
    return SPIRVResult::Invalid();
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateCommaExpressionSPIRV(Compiler* compiler, SPIRVGenerator* generator, Expression* expr)
{
    CommaExpression* commaExpression = static_cast<CommaExpression*>(expr);

    GenerateExpressionSPIRV(compiler, generator, commaExpression->left);
    return GenerateExpressionSPIRV(compiler, generator, commaExpression->right);
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateTernaryExpressionSPIRV(Compiler* compiler, SPIRVGenerator* generator, Expression* expr)
{
    TernaryExpression* ternaryExpression = static_cast<TernaryExpression*>(expr);
    TernaryExpression::__Resolved* ternaryExpressionResolved = Symbol::Resolved(ternaryExpression);

    SPIRVResult lhsResult = GenerateExpressionSPIRV(compiler, generator, ternaryExpression->lhs);

    SPIRVResult ifResult = GenerateExpressionSPIRV(compiler, generator, ternaryExpression->ifExpression);
    SPIRVResult elseResult = GenerateExpressionSPIRV(compiler, generator, ternaryExpression->ifExpression);
    uint32_t ret = generator->AddMappedOp(Format("OpSelect %%%d %%%d %%%d %%%d", ifResult.typeName, lhsResult.name, ifResult.name, elseResult.name), ternaryExpressionResolved->text);

    return SPIRVResult(ret, ifResult.typeName, ifResult.isValue, ifResult.isLiteral);
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateUnaryExpressionSPIRV(Compiler* compiler, SPIRVGenerator* generator, Expression* expr)
{
    UnaryExpression* unaryExpression = static_cast<UnaryExpression*>(expr);
    UnaryExpression::__Resolved* unaryExpressionResolved = Symbol::Resolved(unaryExpression);

    SPIRVResult rhs = GenerateExpressionSPIRV(compiler, generator, unaryExpression->expr);

    static std::unordered_map<std::string, std::tuple<const char, bool>> lookupTable =
    {
        { "f32", { 'F', true } }
        , { "i32", { 'I', true } }
        , { "u32", { 'I', false } }
    };

    auto value = lookupTable.find(unaryExpressionResolved->fullType.name);
    assert(value != lookupTable.end());

    const char op = std::get<0>(value->second);
    bool isSigned = std::get<1>(value->second);
    switch (unaryExpression->op)
    {
        case '++':
        {
            SPIRVResult constOne = SPIRVResult::Invalid();
            if (isSigned)
            {
                if (op == 'I')
                {
                    constOne = GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::Int32(1));
                }
                else
                {
                    constOne = GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::Float32(1));
                }
            }
            else
            {
                constOne = GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::UInt32(1));
            }

            uint32_t value = rhs.name;
            if (!rhs.isValue)
                value = generator->AddMappedOp(Format("OpLoad %%%d %%%d", rhs.typeName, rhs.name));
            uint32_t res = generator->AddMappedOp(Format("Op%cAdd %%%d %%%d %%%d", op, rhs.typeName, value, constOne.name), unaryExpressionResolved->text);
            generator->AddOp(Format("OpStore %%%d %%%d", rhs.name, res));
            if (unaryExpression->isPrefix)
                return SPIRVResult(res, rhs.typeName, true);
            else
                return SPIRVResult(value, rhs.typeName, true);
        }
        case '--':
        {
            SPIRVResult constOne = SPIRVResult::Invalid();
            if (isSigned)
            {
                if (op == 'I')
                {
                    constOne = GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::Int32(1));
                }
                else
                {
                    constOne = GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::Float32(1));
                }
            }
            else
            {
                constOne = GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::UInt32(1));
            }

            uint32_t value = rhs.name;
            if (!rhs.isValue)
                value = generator->AddMappedOp(Format("OpLoad %%%d %%%d", rhs.typeName, rhs.name));
            uint32_t res = generator->AddMappedOp(Format("Op%cSub %%%d %%%d %%%d", op, rhs.typeName, value, constOne.name), unaryExpressionResolved->text);
            generator->AddOp(Format("OpStore %%%d %%%d", rhs.name, res));
            if (unaryExpression->isPrefix)
                return SPIRVResult(res, rhs.typeName, true);
            else
                return SPIRVResult(value, rhs.typeName, true);
            break;
        }
        case '*':
        {
            Type::FullType newType = unaryExpressionResolved->fullType;
            assert(newType.modifiers.size() > 0);
            assert(newType.modifiers.back() == Type::FullType::Modifier::PointerLevel);
            newType.modifiers.pop_back();
            Type* type = compiler->GetSymbol<Type>(newType.name);
            SPIRVResult typeName = GenerateTypeSPIRV(compiler, generator, newType, type);
            uint32_t deref = generator->AddMappedOp(Format("OpLoad %%%d %%%d", typeName.typeName, rhs.name), unaryExpressionResolved->text);
            return SPIRVResult(deref, typeName.typeName, newType.modifiers.empty());
            break;
        }
        case '-':
        {
            uint32_t value = rhs.name;
            if (!rhs.isValue)
                value = generator->AddMappedOp(Format("OpLoad %%%d %%%d", rhs.typeName, rhs.name));
            uint32_t res = generator->AddMappedOp(Format("Op%cNegate %%%d", op, value), unaryExpressionResolved->text);
            generator->AddOp(Format("OpStore %%%d %%%d", rhs.name, res));
            break;
        }
        case '+':
        {
            uint32_t value = rhs.name;
            if (!rhs.isValue)
                value = generator->AddMappedOp(Format("OpLoad %%%d %%%d", rhs.typeName, rhs.name), unaryExpressionResolved->text);
            return SPIRVResult(value, rhs.typeName, true);
        }
        case '!':
        case '~':
        {
            uint32_t value = rhs.name;
            if (!rhs.isValue)
                value = generator->AddMappedOp(Format("OpLoad %%%d %%%d", rhs.typeName, rhs.name), unaryExpressionResolved->text);
            uint32_t res = generator->AddMappedOp(Format("OpNot %%%d", value));
            generator->AddOp(Format("OpStore %%%d %%%d", rhs.name, res));
            break;
        }
    }
    return SPIRVResult::Invalid();
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateExpressionSPIRV(Compiler* compiler, SPIRVGenerator* generator, Expression* expr)
{
    switch (expr->symbolType)
    {
        case Symbol::FloatExpressionType:
        {
            float value;
            FloatExpression* floatExpr = static_cast<FloatExpression*>(expr);
            floatExpr->EvalFloat(value);
            uint32_t typeName = generator->AddSymbol("f32", Format("OpTypeFloat 32"), true);
            uint32_t name = generator->AddSymbol(Format("%ff", value), Format("OpConstant %%%d %f", typeName, value), true);
            return SPIRVResult(name, typeName, true, true);
        }
        case Symbol::IntExpressionType:
        {
            int value;
            IntExpression* intExpr = static_cast<IntExpression*>(expr);
            intExpr->EvalInt(value);
            uint32_t typeName = generator->AddSymbol("i32", Format("OpTypeInt 32 1"), true);
            uint32_t name = generator->AddSymbol(Format("%ds", value), Format("OpConstant %%%d %d", typeName, value), true);
            return SPIRVResult(name, typeName, true, true);
        }
        case Symbol::UIntExpressionType:
        {
            uint32_t value;
            UIntExpression* uintExpr = static_cast<UIntExpression*>(expr);
            uintExpr->EvalUInt(value);
            uint32_t typeName = generator->AddSymbol("u32", Format("OpTypeInt 32 0"), true);
            uint32_t name = generator->AddSymbol(Format("%du", value), Format("OpConstant %%%d %d", typeName, value), true);
            return SPIRVResult(name, typeName, true, true);
        }
        case Symbol::BoolExpressionType:
        {
            bool value;
            BoolExpression* boolExpr = static_cast<BoolExpression*>(expr);
            boolExpr->EvalBool(value);
            uint32_t typeName = generator->AddSymbol("b8", Format("OpTypeBool"), true);
            uint32_t name = generator->AddSymbol(value ? "true" : "false", value ? Format("OpConstantTrue %%%d", typeName) : Format("OpConstantFalse %%%d", typeName), true);
            return SPIRVResult(name, typeName, true, true);
        }
        case Symbol::BinaryExpressionType:
            return GenerateBinaryExpressionSPIRV(compiler, generator, expr);
            break;
        case Symbol::SymbolExpressionType:
        {
            SymbolExpression* symbolExpression = static_cast<SymbolExpression*>(expr);
            SymbolExpression::__Resolved* symResolved = Symbol::Resolved(symbolExpression);
            SPIRVResult type = SPIRVResult::Invalid();
            if (symResolved->symbol->symbolType == Symbol::SymbolType::VariableType)
            {
                Variable* var = static_cast<Variable*>(symResolved->symbol);
                Variable::__Resolved* varResolved = Symbol::Resolved(var);
                type = GenerateTypeSPIRV(compiler, generator, symResolved->fullType, symResolved->type, varResolved->imageFormat, varResolved->accessBits, varResolved->usageBits);
            }
            else
            {
                type = GenerateTypeSPIRV(compiler, generator, symResolved->fullType, symResolved->type);
            }
            return SPIRVResult(generator->GetSymbol(symbolExpression->symbol), type.typeName, false, false, type.scope);
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
    assert(generator->mergeBlocks[generator->mergeBlockCounter-1].breakLabel != 0xFFFFFFFF);
    generator->AddOp(Format("OpBranch %%%d", generator->mergeBlocks[generator->mergeBlockCounter - 1].breakLabel), false, "break");
    generator->blockOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
void
GenerateContinueStatementSPIRV(Compiler* compiler, SPIRVGenerator* generator, ContinueStatement* stat)
{
    assert(generator->mergeBlocks[generator->mergeBlockCounter - 1].continueLabel != 0xFFFFFFFF);
    generator->AddOp(Format("OpBranch %%%d", generator->mergeBlocks[generator->mergeBlockCounter - 1].continueLabel), false, "continue");
    generator->blockOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
void
GenerateForStatementSPIRV(Compiler* compiler, SPIRVGenerator* generator, ForStatement* stat)
{
    for (auto decl : stat->declarations)
        GenerateVariableSPIRV(compiler, generator, decl, false, false);

    uint32_t startLabel = generator->ReserveName();
    uint32_t conditionLabel = generator->ReserveName();
    uint32_t repeatLabel = generator->ReserveName();
    uint32_t bodyLabel = generator->ReserveName();
    uint32_t endLabel = generator->ReserveName();

    generator->mergeBlocks[generator->mergeBlockCounter++] =
    SPIRVGenerator::MergeBlock
    {
        startLabel, endLabel
    };

    // Initial label to start the loop
    generator->AddOp(Format("OpBranch %%%d", startLabel));
    generator->AddReserved("OpLabel", startLabel);

    // All loops must begin with a loop merge
    generator->AddOp(Format("OpLoopMerge %%%d %%%d None", endLabel, repeatLabel));
    generator->AddOp(Format("OpBranch %%%d", conditionLabel));

    // This block is for the condition testing
    generator->AddReserved("OpLabel", conditionLabel, "for condition");
    generator->blockOpen = true;
    SPIRVResult cond = GenerateExpressionSPIRV(compiler, generator, stat->condition);
    if (!cond.isValue)
        cond.name = generator->AddMappedOp(Format("OpLoad %%%d %%%d", cond.typeName, cond.name));

    // Decide whether or not to end the loop
    generator->AddOp(Format("OpBranchConditional %%%d %%%d %%%d", cond.name, bodyLabel, endLabel));
    generator->AddReserved("OpLabel", bodyLabel, "for body");
    generator->blockOpen = true;
    if (!GenerateStatementSPIRV(compiler, generator, stat->contents))
    {
        generator->AddOp(Format("OpBranch %%%d", repeatLabel));
        generator->blockOpen = false;
    }

    // This is the repeat condition
    generator->AddReserved("OpLabel", repeatLabel, "for repeat");
    generator->blockOpen = true;
    GenerateExpressionSPIRV(compiler, generator, stat->loop);
    generator->AddOp(Format("OpBranch %%%d", startLabel));
    generator->AddReserved("OpLabel", endLabel, "end of for");
    generator->blockOpen = true;

    generator->mergeBlocks[--generator->mergeBlockCounter] =
    SPIRVGenerator::MergeBlock
    {
        0xFFFFFFFF, 0xFFFFFFFF
    };
}

//------------------------------------------------------------------------------
/**
*/
void
GenerateIfStatementSPIRV(Compiler* compiler, SPIRVGenerator* generator, IfStatement* stat)
{
    SPIRVResult lhsResult = GenerateExpressionSPIRV(compiler, generator, stat->condition);

    uint32_t ifLabel = generator->ReserveName();
    uint32_t endLabel = generator->ReserveName();

    generator->AddOp(Format("OpSelectionMerge %%%d None", endLabel));
    if (stat->elseStatement)
    {
        uint32_t elseLabel = generator->ReserveName();

        generator->AddOp(Format("OpBranchConditional %%%d %%%d %%%d", lhsResult.name, ifLabel, elseLabel));
        generator->blockOpen = false;

        generator->AddReserved("OpLabel", ifLabel, stat->condition->EvalString());
        generator->blockOpen = true;
        if (!GenerateStatementSPIRV(compiler, generator, stat->ifStatement))
        {
            generator->AddOp(Format("OpBranch %%%d", endLabel));
            generator->blockOpen = false;
        }

        generator->AddReserved("OpLabel", elseLabel, "else");
        generator->blockOpen = true;
        if (!GenerateStatementSPIRV(compiler, generator, stat->elseStatement))
        {
            generator->AddOp(Format("OpBranch %%%d", endLabel));
            generator->blockOpen = false;
        }
    }
    else
    {
        generator->AddOp(Format("OpBranchConditional %%%d %%%d %%%d", lhsResult.name, ifLabel, endLabel));
        generator->blockOpen = false;

        generator->AddReserved("OpLabel", ifLabel, stat->condition->EvalString());
        generator->blockOpen = true;
        if (!GenerateStatementSPIRV(compiler, generator, stat->ifStatement))
        {
            generator->AddOp(Format("OpBranch %%%d", endLabel));
            generator->blockOpen = false;
        }
        
    }
    generator->AddReserved("OpLabel", endLabel, "end of condition");
    generator->blockOpen = true;
}

//------------------------------------------------------------------------------
/**
*/
void
GenerateReturnStatementSPIRV(Compiler* compiler, SPIRVGenerator* generator, ReturnStatement* stat)
{
    if (stat->returnValue != nullptr)
    {
        SPIRVResult res = GenerateExpressionSPIRV(compiler, generator, stat->returnValue);
        if (!res.isValue)
            res.name = generator->AddMappedOp(Format("OpLoad %%%d %%%d", res.typeName, res.name));
        generator->AddOp(Format("OpReturnValue %%%d", res.name));
        generator->blockOpen = false;
    }
    else
    {
        generator->AddOp("OpReturn");
        generator->blockOpen = false;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
GenerateSwitchStatementSPIRV(Compiler* compiler, SPIRVGenerator* generator, SwitchStatement* stat)
{
    SPIRVResult switchRes = GenerateExpressionSPIRV(compiler, generator, stat->switchExpression);
    if (!switchRes.isValue)
        switchRes.name = generator->AddMappedOp(Format("OpLoad %%%d %%%d", switchRes.typeName, switchRes.name));

    uint32_t defaultCase = generator->ReserveName();
    uint32_t mergeLabel = generator->ReserveName();

    generator->mergeBlocks[generator->mergeBlockCounter++] =
    SPIRVGenerator::MergeBlock
    {
        0xFFFFFFFF, mergeLabel
    };

    // First forward declare our labels and setup the switch
    std::string caseList = "";
    std::vector<uint32_t> reservedCaseLabels;
    for (size_t i = 0; i < stat->caseExpressions.size(); i++)
    {
        uint32_t caseLabel = generator->ReserveName();
        int lit;
        bool res = stat->caseExpressions[i]->EvalInt(lit);
        assert(res);
        caseList += Format("%d %%%d ", lit, caseLabel);
        reservedCaseLabels.push_back(caseLabel);
    }

    generator->AddOp(Format("OpSelectionMerge %%%d None", mergeLabel));
    generator->AddOp(Format("OpSwitch %%%d %%%d %s", switchRes.name, defaultCase, caseList.c_str()));

    for (size_t i = 0; i < stat->caseStatements.size(); i++)
    {
        generator->AddReserved("OpLabel", reservedCaseLabels[i], Format("case %s", stat->caseExpressions[i]->EvalString().c_str()));
        generator->blockOpen = true;
        if (stat->caseStatements[i] != nullptr)
        {
            if (!GenerateStatementSPIRV(compiler, generator, stat->caseStatements[i]))
                if (i + 1 < stat->caseStatements.size())
                    generator->AddOp(Format("OpBranch %%%d", reservedCaseLabels[i + 1]));
                else
                    generator->AddOp(Format("OpBranch %%%d", stat->defaultStatement == nullptr ? mergeLabel : defaultCase));
            generator->blockOpen = false;
        }
        else
        {
            if (i + 1 < stat->caseStatements.size())
                generator->AddOp(Format("OpBranch %%%d", reservedCaseLabels[i + 1]));
            else
                generator->AddOp(Format("OpBranch %%%d", stat->defaultStatement == nullptr ? mergeLabel : defaultCase));
            generator->blockOpen = false;
        }
    }
    generator->AddReserved("OpLabel", defaultCase, "case default");
    generator->blockOpen = true;
    if (stat->defaultStatement)
    {
        if (!GenerateStatementSPIRV(compiler, generator, stat->defaultStatement))
        {
            generator->AddOp(Format("OpBranch %%%d", mergeLabel));
            generator->blockOpen = false;
        }
    }
    else
    {

        generator->AddOp(Format("OpBranch %%%d", mergeLabel));
        generator->blockOpen = false;
    }
    
    generator->AddReserved("OpLabel", mergeLabel, "end of switch");
    generator->blockOpen = true;

    generator->mergeBlocks[--generator->mergeBlockCounter] =
    SPIRVGenerator::MergeBlock
    {
        0xFFFFFFFF, 0xFFFFFFFF
    };
}

//------------------------------------------------------------------------------
/**
*/
void
GenerateWhileStatementSPIRV(Compiler* compiler, SPIRVGenerator* generator, WhileStatement* stat)
{
    uint32_t startLabel = generator->ReserveName();
    uint32_t conditionLabel = generator->ReserveName();
    uint32_t continueLabel = generator->ReserveName();
    uint32_t bodyLabel = generator->ReserveName();
    uint32_t endLabel = generator->ReserveName();

    generator->mergeBlocks[generator->mergeBlockCounter++] =
    SPIRVGenerator::MergeBlock
    {
        continueLabel, endLabel
    };

    // Initial label to start the loop
    generator->AddOp(Format("OpBranch %%%d", startLabel));
    generator->AddReserved("OpLabel", startLabel);
    generator->blockOpen = true;

    if (stat->isDoWhile)
    {
        // All loops must begin with a loop merge
        generator->AddOp(Format("OpLoopMerge %%%d %%%d None", endLabel, continueLabel));
        generator->AddOp(Format("OpBranch %%%d", bodyLabel));
        generator->blockOpen = false;

        // Decide whether or not to end the loop
        generator->AddReserved("OpLabel", bodyLabel, "while body");
        generator->blockOpen = true;

        if (!GenerateStatementSPIRV(compiler, generator, stat->statement))
        {
            generator->AddOp(Format("OpBranch %%%d", conditionLabel));
            generator->blockOpen = false;
        }

        // This block is for the condition testing
        generator->AddReserved("OpLabel", conditionLabel, "while condition");
        generator->blockOpen = true;

        SPIRVResult cond = GenerateExpressionSPIRV(compiler, generator, stat->condition);
        if (!cond.isValue)
            cond.name = generator->AddMappedOp(Format("OpLoad %%%d %%%d", cond.typeName, cond.name));

        generator->AddOp(Format("OpBranchConditional %%%d %%%d %%%d", cond.name, continueLabel, endLabel));
        generator->blockOpen = false;

        generator->AddReserved("OpLabel", continueLabel, "continue");
        generator->AddOp(Format("OpBranch %%%d", startLabel));

        // This is the repeat condition
        generator->AddReserved("OpLabel", endLabel, "end of while");
        generator->blockOpen = true;
    }
    else
    {
        // All loops must begin with a loop merge
        generator->AddOp(Format("OpLoopMerge %%%d %%%d None", endLabel, continueLabel));
        generator->AddOp(Format("OpBranch %%%d", conditionLabel));
        generator->blockOpen = false;

        // Load from the variable to do the branch condition
        generator->AddReserved("OpLabel", conditionLabel, "for condition test");
        generator->blockOpen = true;

        SPIRVResult cond = GenerateExpressionSPIRV(compiler, generator, stat->condition);
        if (!cond.isValue)
            cond.name = generator->AddMappedOp(Format("OpLoad %%%d %%%d", cond.typeName, cond.name));

        // Decide whether or not to end the loop
        generator->AddOp(Format("OpBranchConditional %%%d %%%d %%%d", cond.name, bodyLabel, endLabel));
        generator->blockOpen = false;
        if (stat->statement)
        {
            generator->AddReserved("OpLabel", bodyLabel, "while body");
            generator->blockOpen = true;
            if (!GenerateStatementSPIRV(compiler, generator, stat->statement))
            {
                generator->AddOp(Format("OpBranch %%%d", continueLabel));
                generator->blockOpen = false;
            }
        }

        generator->AddReserved("OpLabel", continueLabel, "continue");
        generator->AddOp(Format("OpBranch %%%d", startLabel));

        // This is the repeat condition
        generator->AddReserved("OpLabel", endLabel, "end of while");
        generator->blockOpen = true;
    }

    generator->mergeBlocks[--generator->mergeBlockCounter] =
    SPIRVGenerator::MergeBlock
    {
        0xFFFFFFFF, 0xFFFFFFFF
    };
}

//------------------------------------------------------------------------------
/**
    Generate statement, returns true if statement closes the current block
*/
bool
GenerateStatementSPIRV(Compiler* compiler, SPIRVGenerator* generator, Statement* stat)
{
    bool ret = false;
    Symbol::__Resolved* symResolved = Symbol::Resolved(stat);
    if (symResolved->unreachable)
        return true;
    switch (stat->symbolType)
    {
        case Symbol::BreakStatementType:
            GenerateBreakStatementSPIRV(compiler, generator, static_cast<BreakStatement*>(stat));
            ret = true;
            break;
        case Symbol::ContinueStatementType:
            GenerateContinueStatementSPIRV(compiler, generator, static_cast<ContinueStatement*>(stat));
            ret = true;
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
            ret = true;
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
                    bool shouldEnd = GenerateStatementSPIRV(compiler, generator, stat);
                    if (shouldEnd)
                        break;
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
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
bool 
SPIRVGenerator::Generate(Compiler* compiler, Program* program, const std::vector<Symbol*>& symbols, std::function<void(const std::string&, const std::string&)> writerFunc)
{
    Program::__Resolved* progResolved = static_cast<Program::__Resolved*>(program->resolved);

    // Main scope
    this->PushScope();

    struct Cleanup
    {
        Cleanup(SPIRVGenerator* gen) :
            gen(gen) {}

        ~Cleanup()
        {
            gen->scopeStack.clear();
            gen->header.clear();
            gen->declarations.clear();
            gen->decorations.clear();
            gen->functional.clear();
            gen->capabilities.clear();
            gen->extensions.clear();
            gen->decorationMap.clear();
            gen->mergeBlockCounter = 0;
        }

        SPIRVGenerator* gen;
    };

    spv_context spvContext = spvContextCreate(SPV_ENV_VULKAN_1_2);

    static std::unordered_map<Program::__Resolved::ProgramEntryType, std::string> executionModelMap =
    {
        { Program::__Resolved::ProgramEntryType::VertexShader, "Vertex" }
        , { Program::__Resolved::ProgramEntryType::HullShader, "TessellationControl" }
        , { Program::__Resolved::ProgramEntryType::DomainShader, "TessellationEvaluation" }
        , { Program::__Resolved::ProgramEntryType::GeometryShader, "Geometry" }
        , { Program::__Resolved::ProgramEntryType::PixelShader, "Fragment" }
        , { Program::__Resolved::ProgramEntryType::ComputeShader, "GLCompute" }
        , { Program::__Resolved::ProgramEntryType::TaskShader, "TaskEXT" }
        , { Program::__Resolved::ProgramEntryType::MeshShader, "MeshEXT" }
        , { Program::__Resolved::ProgramEntryType::RayGenerationShader, "RayGenerationKHR" }
        , { Program::__Resolved::ProgramEntryType::RayMissShader, "MissKHR" }
        , { Program::__Resolved::ProgramEntryType::RayClosestHitShader, "ClosestHitKHR" }
        , { Program::__Resolved::ProgramEntryType::RayAnyHitShader, "AnyHitKHR" }
        , { Program::__Resolved::ProgramEntryType::RayIntersectionShader, "IntersectionKHR" }
        , { Program::__Resolved::ProgramEntryType::RayCallableShader, "CallableKHR" }
    };

    static std::unordered_map<Program::__Resolved::ProgramEntryType, std::string> extensionMap =
    {
        { Program::__Resolved::ProgramEntryType::VertexShader, "Shader" }
        , { Program::__Resolved::ProgramEntryType::HullShader, "Tessellation" }
        , { Program::__Resolved::ProgramEntryType::DomainShader, "Tessellation" }
        , { Program::__Resolved::ProgramEntryType::GeometryShader, "Geometry" }
        , { Program::__Resolved::ProgramEntryType::PixelShader, "Shader" }
        , { Program::__Resolved::ProgramEntryType::ComputeShader, "Shader" }
        , { Program::__Resolved::ProgramEntryType::TaskShader, "MeshShadingEXT" }
        , { Program::__Resolved::ProgramEntryType::MeshShader, "MeshShadingEXT" }
        , { Program::__Resolved::ProgramEntryType::RayGenerationShader, "RayTracingKHR" }
        , { Program::__Resolved::ProgramEntryType::RayMissShader, "RayTracingKHR" }
        , { Program::__Resolved::ProgramEntryType::RayClosestHitShader, "RayTracingKHR" }
        , { Program::__Resolved::ProgramEntryType::RayAnyHitShader, "RayTracingKHR" }
        , { Program::__Resolved::ProgramEntryType::RayIntersectionShader, "RayTracingKHR" }
        , { Program::__Resolved::ProgramEntryType::RayCallableShader, "RayTracingKHR" }
    };

    static std::unordered_map<Function::__Resolved::PartitionMethod, std::string> partitionMap =
    {
        { Function::__Resolved::PartitionMethod::IntegerSteps, "SpacingEqual" }
        , { Function::__Resolved::PartitionMethod::FloatEven, "SpacingFractionalEven" }
        , { Function::__Resolved::PartitionMethod::FloatOdd, "SpacingFractionalOdd" }
    };

    static std::unordered_map<Function::__Resolved::WindingOrder, std::string> windingOrderMap =
    {
        { Function::__Resolved::WindingOrder::Clockwise, "VertexOrderCw" }
        , { Function::__Resolved::WindingOrder::CounterClockwise, "VertexOrderCcw" }
    };

    static std::unordered_map<Function::__Resolved::PixelOrigin, std::string> pixelOriginMap =
    {
        { Function::__Resolved::PixelOrigin::Center, "VertexOrderCw" }
        , { Function::__Resolved::PixelOrigin::Upper, "OriginUpperLeft" }
        , { Function::__Resolved::PixelOrigin::Lower, "OriginLowerLeft" }
    };

    static std::unordered_map<Function::__Resolved::PrimitiveTopology, std::string> inputPrimitiveTopologyMap =
    {
        { Function::__Resolved::PrimitiveTopology::Points, "InputPoints" }
        , { Function::__Resolved::PrimitiveTopology::Lines, "InputLines" }
        , { Function::__Resolved::PrimitiveTopology::LinesAdjacency, "InputLinesAdjacency" }
        , { Function::__Resolved::PrimitiveTopology::Triangles, "Triangles" }
        , { Function::__Resolved::PrimitiveTopology::TrianglesAdjacency, "InputTrianglesAdjacency" }
        , { Function::__Resolved::PrimitiveTopology::Quads, "Quads" }
        , { Function::__Resolved::PrimitiveTopology::Isolines, "Isolines" }
    };

    static std::unordered_map<Function::__Resolved::PrimitiveTopology, std::string> outputPrimitiveTopologyMap =
    {
        { Function::__Resolved::PrimitiveTopology::Points, "OutputPoints" }
        , { Function::__Resolved::PrimitiveTopology::Lines, "OutputLineStrip" }
        , { Function::__Resolved::PrimitiveTopology::Triangles, "OutputTriangleStrip" }
    };

    for (auto it : progResolved->programMappings)
    {
        // for each shader, generate code and use it as a binary output
        if (it.first >= Program::__Resolved::VertexShader && it.first <= Program::__Resolved::RayIntersectionShader)
        {
            this->header.append("; Magic:     0x00010500 (Vulkan 1.2)\n");
            this->header.append("; Version:   0x00010000 (Version: 1.0.0)\n");
            this->header.append("; Generator: 0x00080001 (GPULang; 1)\n");
            this->header.append(Format("\tOpCapability %s\n", extensionMap[it.first].c_str()));
            
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

            Function::__Resolved* funResolved = Symbol::Resolved(static_cast<Function*>(it.second));
            uint32_t entryFunction = this->GetSymbol(funResolved->name);

            if (funResolved->executionModifiers.groupSize != 64 || funResolved->executionModifiers.groupsPerWorkgroup != 1)
                this->header.append("\tOpCapability SubgroupDispatch\n");
            this->header.append("\tOpMemoryModel Logical GLSL450\n");

            std::string interfaces = "";
            for (const uint32_t interface : this->interfaceVariables)
            {
                interfaces.append(Format("%%%d ", interface));
            }
            this->header.append(Format("\tOpEntryPoint %s %%%d \"main\" %s\n", executionModelMap[it.first].c_str(), entryFunction, interfaces.c_str()));
            
            switch (it.first)
            {
                case Program::__Resolved::GeometryShader:
                    this->header.append(Format("\tOpExecutionMode %%%d Invocations %d\n", entryFunction, funResolved->executionModifiers.invocations));
                    this->header.append(Format("\tOpExecutionMode %%%d %s\n", entryFunction, inputPrimitiveTopologyMap[funResolved->executionModifiers.inputPrimitiveTopology]));
                    this->header.append(Format("\tOpExecutionMode %%%d %s\n", entryFunction, outputPrimitiveTopologyMap[funResolved->executionModifiers.outputPrimitiveTopology]));
                    this->header.append(Format("\tOpExecutionMode %%%d OutputVertices %d\n", entryFunction, funResolved->executionModifiers.maxOutputVertices));
                    break;
                case Program::__Resolved::HullShader:
                    this->header.append(Format("\tOpExecutionMode %%%d %s\n", entryFunction, partitionMap[funResolved->executionModifiers.partitionMethod]));
                    this->header.append(Format("\tOpExecutionMode %%%d %s\n", entryFunction, windingOrderMap[funResolved->executionModifiers.windingOrder]));
                    this->header.append(Format("\tOpExecutionMode %%%d %s\n", entryFunction, inputPrimitiveTopologyMap[funResolved->executionModifiers.inputPrimitiveTopology]));
                    this->header.append(Format("\tOpExecutionMode %%%d OutputVertices %d\n", entryFunction, funResolved->executionModifiers.maxOutputVertices));
                    break;
                case Program::__Resolved::PixelShader:
                    this->header.append(Format("\tOpExecutionMode %%%d %s\n", entryFunction, pixelOriginMap[funResolved->executionModifiers.pixelOrigin]));
                    if (funResolved->executionModifiers.earlyDepth)
                        this->header.append(Format("\tOpExecutionMode %%%d EarlyFragmentTests\n", entryFunction));
                    break;
                case Program::__Resolved::ComputeShader:
                    this->header.append(Format("\tOpExecutionMode %%%d LocalSize %d %d %d\n", entryFunction, funResolved->executionModifiers.computeShaderWorkGroupSize[0], funResolved->executionModifiers.computeShaderWorkGroupSize[1], funResolved->executionModifiers.computeShaderWorkGroupSize[2]));
                    if (funResolved->executionModifiers.groupSize != 64)
                        this->header.append(Format("\tOpExecutionMode %%%d SubgroupSize %d\n", entryFunction, funResolved->executionModifiers.groupSize));
                    if (funResolved->executionModifiers.groupsPerWorkgroup != 1)
                        this->header.append(Format("\tOpExecutionMode %%%d SubgroupsPerWorkgroup %d\n", entryFunction, funResolved->executionModifiers.groupsPerWorkgroup));
                    break;
            }


            // Compose and convert to binary, then validate
            std::string binary = Format("; Header\n%s\n; Decorations\n%s\n; Declarations\n%s\n; Functions\n%s\n", this->header.c_str(), this->decorations.c_str(), this->declarations.c_str(), this->functions.c_str());
            spv_binary bin = nullptr;
            spv_diagnostic diag = nullptr;
            spv_result_t res = spvTextToBinaryWithOptions(spvContext, binary.c_str(), binary.size(), SPV_TEXT_TO_BINARY_OPTION_NONE, &bin, &diag);
            if (res != SPV_SUCCESS)
            {
                compiler->Error(Format("Internal SPIRV generation error: %s", diag->error), "", -1, -1);
                return false;
            }

            if (compiler->options.validate)
            {
                // Run spv validation for internal consistency and fault testing
                spv_const_binary_t constBin = { bin->code, bin->wordCount };
                res = spvValidate(spvContext, &constBin, &diag);
                if (res != SPV_SUCCESS)
                {
                    compiler->Error(Format("Internal SPIRV generation error: %s", diag->error), "", -1, -1);
                    return false;
                }
            }

            std::vector<uint32_t> binaryData(bin->code, bin->code + bin->wordCount);
            if (compiler->options.optimize)
            {
                spvtools::Optimizer optimizer(SPV_ENV_VULKAN_1_2);
                optimizer.RegisterPerformancePasses();

                std::vector<uint32_t> optimized;
                if (optimizer.Run(bin->code, bin->wordCount, &optimized))
                {
                    binaryData = std::move(optimized);
                }
            }

            // conversion and optional validation is successful, dump binary in program
            progResolved->binaries[it.first] = binaryData;

            delete bin;
            delete diag;
        }
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
uint32_t 
SPIRVGenerator::AddSymbol(std::string name, std::string declare, bool global)
{
    auto scope = this->scopeStack.rbegin();
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
void
SPIRVGenerator::AddReservedSymbol(std::string name, uint32_t object, std::string declare, bool global)
{
    auto scope = this->scopeStack.rbegin();
    while (scope != this->scopeStack.rend())
    {
        auto it = scope->symbols.find(name);
        if (it != scope->symbols.end())
        {
            break;
        }
        scope++;
    }

    // If symbol isn't found in scope, create it
    this->scopeStack.back().symbols[name] = object;

    std::string decl = Format("%%%d\t=\t%s\t\t\t; %s\n", object, declare.c_str(), name.c_str());
    if (global)
        this->declarations.append(decl);
    else
        this->functional.append(decl);
}

//------------------------------------------------------------------------------
/**
*/
uint32_t 
SPIRVGenerator::GetSymbol(std::string name)
{
    uint32_t ret = 0xFFFFFFFF;
    auto it = this->scopeStack.rbegin();
    while (it != this->scopeStack.rend())
    {
        auto sym = it->symbols.find(name);
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
    auto it = this->scopeStack.rbegin();
    while (it != this->scopeStack.rend())
    {
        auto sym = it->symbols.find(name);
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
SPIRVGenerator::AddCapability(std::string declare)
{
    if (this->capabilities.find(declare) == this->capabilities.end())
    {
        this->header.append(Format("\tOpCapability %s\n", declare.c_str()));
        this->capabilities.insert(declare);
    }
}

//------------------------------------------------------------------------------
/**
*/
uint32_t 
SPIRVGenerator::AddExtension(std::string name)
{
    uint32_t ret = 0xFFFFFFFF;
    auto it = this->extensions.find(name);
    if (it == this->extensions.end())
    {
        this->header.append(Format("%%%d = OpExtInstImport \"%s\"\n", this->symbolCounter, name.c_str()));
        this->extensions[name] = this->symbolCounter;
        ret = this->symbolCounter;
    }
    else
    {
        ret = it->second;
    }
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
void 
SPIRVGenerator::AddDecoration(std::string name, uint32_t object, std::string decorate)
{
    auto it = this->decorationMap.find(name);
    if (it == this->decorationMap.end())
    {
        this->decorationMap[name].insert(decorate);
        this->decorations.append(Format("\tOpDecorate %%%d %s\n", object, decorate.c_str()));
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
SPIRVGenerator::AddMemberDecoration(uint32_t struc, uint32_t index, std::string decorate)
{
    this->decorations.append(Format("\tOpMemberDecorate %%%d %d %s\n", struc, index, decorate.c_str()));
}

//------------------------------------------------------------------------------
/**
*/
uint32_t 
SPIRVGenerator::ReserveName()
{
    return this->symbolCounter++;
}

//------------------------------------------------------------------------------
/**
*/
void 
SPIRVGenerator::AddReserved(std::string op, uint32_t name, std::string comment)
{
    this->functional.append(Format("%%%d = \t%s\t\t\t; %s\n", name, op.c_str(), comment.c_str()));
}

//------------------------------------------------------------------------------
/**
*/
uint32_t 
SPIRVGenerator::AddVariableDeclaration(uint32_t type, uint32_t init, std::string comment)
{
    if (init != 0)
        this->variableDeclarations.append(Format("%%%d = \tOpVariable %%%d Function %%%d\t\t\t; %s\n", this->symbolCounter, type, init, comment.c_str()));
    else
        this->variableDeclarations.append(Format("%%%d = \tOpVariable %%%d Function\t\t\t; %s\n", this->symbolCounter, type, comment.c_str()));
    return this->symbolCounter++;
}

//------------------------------------------------------------------------------
/**
*/
uint32_t 
SPIRVGenerator::FindSymbolMapping(std::string value)
{
    uint32_t ret = 0xFFFFFFFF;
    auto it = this->scopeStack.rbegin();
    while (it != this->scopeStack.rend())
    {
        auto sym = it->symbols.find(value);
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
    auto it = this->scopeStack.rbegin();
    while (it != this->scopeStack.rend())
    {
        auto sym = it->symbols.begin();
        while (sym != it->symbols.end())
        {
            if (sym->second == oldMapping)
            {
                sym->second = newMapping;
                return;
            }
            sym++;
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
