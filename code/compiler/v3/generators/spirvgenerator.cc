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
#include "ast/expressions/enumexpression.h"

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
#include <format>

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
    , { "texture1D", "OpTypeImage %%%d 1D %d 0 0 %d %s" }
    , { "texture2D", "OpTypeImage %%%d 2D %d 0 0 %d %s" }
    , { "texture2DMS", "OpTypeImage %%%d 2D %d 0 1 %d %s" }
    , { "texture3D", "OpTypeImage %%%d 3D %d 0 0 %d %s" }
    , { "textureCube", "OpTypeImage %%%d Cube %d 0 0 %d %s" }
    , { "texture1DArray", "OpTypeImage %%%d 1D %d 1 0 %d %s" }
    , { "texture2DArray", "OpTypeImage %%%d 2D %d 1 0 %d %s" }
    , { "texture2DMSArray", "OpTypeImage %%%d 2D %d 1 1 %d %s" }
    , { "texture3DArray", "OpTypeImage %%%d 3D %d 1 0 %d %s" }
    , { "textureCubeArray", "OpTypeImage %%%d Cube %d 1 0 %d %s" }
    , { "pixelCache", "OpTypeImage %%%d SubpassData 0 0 0 2 Unknown ReadOnly" }
    , { "pixelCacheMS", "OpTypeImage %%%d SubpassData 0 0 1 2 Unknown ReadOnly" }
    , { "sampler", "OpTypeSampler" }
};

std::unordered_map<ImageFormat, std::string> imageFormatToSpirvType =
{
    { ImageFormat::Rgba16, "Rgba16" }
    , { ImageFormat::Rgb10_A2, "Rgb10A2" }
    , { ImageFormat::Rgba8, "Rgba8" }
    , { ImageFormat::Rg16, "Rg16" }
    , { ImageFormat::Rg8, "Rg8" }
    , { ImageFormat::R16, "R16" }
    , { ImageFormat::R8, "R8" }
    , { ImageFormat::Rgba16_Snorm, "Rgba16Snorm" }
    , { ImageFormat::Rgba8_Snorm, "Rgba8Snorm" }
    , { ImageFormat::Rg16_Snorm, "Rg16Snorm" }
    , { ImageFormat::Rg8_Snorm, "Rg8Snorm" }
    , { ImageFormat::R16_Snorm, "R16Snorm" }
    , { ImageFormat::R8_Snorm, "R8Snorm" }

    // float
    , { ImageFormat::Rgba32F, "Rgba32f" }
    , { ImageFormat::Rgba16F, "Rgba16f" }
    , { ImageFormat::Rg32F, "Rg32f" }
    , { ImageFormat::Rg16F, "Rg16f" }
    , { ImageFormat::R11G11B10F, "R11fG11fB10f" }
    , { ImageFormat::R32F, "R32f" }
    , { ImageFormat::R16F, "R16f" }

    // integer
    , { ImageFormat::Rgba32I, "Rgba32i" }
    , { ImageFormat::Rgba16I, "Rgba16i" }
    , { ImageFormat::Rgba8I, "Rgba8i" }
    , { ImageFormat::Rg32I, "Rg32i" }
    , { ImageFormat::Rg16I, "Rg16i" }
    , { ImageFormat::Rg8I, "Rg8i" }
    , { ImageFormat::R32I, "R32i" }
    , { ImageFormat::R16I, "R16i" }
    , { ImageFormat::R8I, "R8i" }

    // unsigned integer
    , { ImageFormat::Rgba32U, "Rgba32ui" }
    , { ImageFormat::Rgba16U, "Rgba16ui" }
    , { ImageFormat::Rgb10_A2U, "Rga10a2ui" }
    , { ImageFormat::Rgba8U, "Rgba8ui" }
    , { ImageFormat::Rg32U, "Rg32ui" }
    , { ImageFormat::Rg16U, "Rg16ui" }
    , { ImageFormat::Rg8U, "Rg8ui" }
    , { ImageFormat::R32U, "R32ui" }
    , { ImageFormat::R16U, "R16ui" }
    , { ImageFormat::R8U, "R8ui" }

    , { ImageFormat::Unknown, "Unknown" }
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
    , Variable::__Resolved::Storage storage = Variable::__Resolved::Storage::Default
)
{
    std::string baseType = Type::CodeToString(typeSymbol->baseType);
    std::string spirvType = typeToSpirvType[baseType];
    SPIRVResult::Storage scope = SPIRVResult::Storage::Function;
    std::vector<uint32_t> parentType;

    auto reducePointer = [](Compiler* compiler, Type::FullType& type)
    {
        if (!compiler->target.supportsPhysicalAddressing)
        {
            for (size_t i = 0; i < type.modifiers.size(); i++)
            {
                const Type::FullType::Modifier mod = type.modifiers[i];
                if (mod == Type::FullType::Modifier::Pointer)
                {
                    type.modifiers.erase(type.modifiers.begin() + i);
                    break;
                }
            }
        }
    };

    uint32_t name = 0xFFFFFFFF;
    if (typeSymbol->category == Type::ScalarCategory)
    {
        name = generator->AddSymbol(baseType, spirvType, true);
        if (typeSymbol->IsVector())
        {
            parentType.push_back(name);
            name = generator->AddSymbol(type.name, Format("OpTypeVector %%%d %d", name, typeSymbol->columnSize), true);
            
        }
        else if (typeSymbol->IsMatrix())
        {
            generator->AddCapability("Matrix");
            baseType = Format("%sx%d", baseType.c_str(), typeSymbol->rowSize);
            parentType.push_back(name);
            name = generator->AddSymbol(baseType, Format("OpTypeVector %%%d %d", name, typeSymbol->rowSize), true);
            parentType.push_back(name);
            name = generator->AddSymbol(type.name, Format("OpTypeMatrix %%%d %d", name, typeSymbol->columnSize), true);
        }

        if (storage == Variable::__Resolved::Storage::Input)
            scope = SPIRVResult::Storage::Input;
        else if (storage == Variable::__Resolved::Storage::Output)
            scope = SPIRVResult::Storage::Output;
        else if (storage == Variable::__Resolved::Storage::Workgroup)
            scope = SPIRVResult::Storage::WorkGroup;
        else if (storage == Variable::__Resolved::Storage::LinkDefined)
            scope = SPIRVResult::Storage::UniformConstant;
        else if (storage == Variable::__Resolved::Storage::Global)
            scope = SPIRVResult::Storage::Private;
    }
    else if (typeSymbol->category == Type::TextureCategory)
    {
        Type* vec = compiler->GetSymbol<Type>("f32");
        SPIRVResult vecType = GenerateTypeSPIRV(compiler, generator, Type::FullType{ "f32" }, vec);
        std::string accessPattern = "";
        uint32_t sampleBits, depthBits = 0;
        if (type.IsMutable())
            sampleBits = 2;
        else
            sampleBits = 1;

        std::string spirvFormat = imageFormatToSpirvType[imageFormat];
        spirvType = Format(spirvType.c_str(), vecType.typeName, depthBits, sampleBits, spirvFormat.c_str());
        baseType = Format("%s,%s,Sample=%d,Depth=%d", spirvFormat.c_str(), baseType.c_str(), sampleBits, depthBits);
        name = generator->AddSymbol(baseType, spirvType, true);
        if (type.sampled)
        {
            baseType = Format("sampledType(%d)", name);
            name = generator->AddSymbol(baseType, Format("OpTypeSampledImage %%%d", name), true);
        }
        if (type.mut)
            scope = SPIRVResult::Storage::MutableImage;
        else
            scope = SPIRVResult::Storage::Image;
    }
    else if (typeSymbol->category == Type::PixelCacheCategory)
    {
        Type* vec = compiler->GetSymbol<Type>("f32x4");
        SPIRVResult vecType = GenerateTypeSPIRV(compiler, generator, Type::FullType{ "f32x4" }, vec);
        spirvType = Format(spirvType.c_str(), vecType.typeName);
        name = generator->AddSymbol(baseType, spirvType, true);
        scope = SPIRVResult::Storage::Image;
    }
    else if (typeSymbol->category == Type::SamplerCategory)
    {
        name = generator->AddSymbol(baseType, spirvType, true);
        scope = SPIRVResult::Storage::Sampler;
    }
    else if (typeSymbol->category == Type::VoidCategory)
    {
        name = generator->AddSymbol("void", "OpTypeVoid", true);
    }
    else if (typeSymbol->category == Type::EnumCategory)
    {
        Type* u32 = compiler->GetSymbol<Type>("u32");
        name = GenerateTypeSPIRV(compiler, generator, Type::FullType{ "u32" }, u32).typeName;
        if (storage == Variable::__Resolved::Storage::Global)
            scope = SPIRVResult::Storage::Private;
    }
    else if (typeSymbol->category == Type::UserTypeCategory)
    {
        name = generator->GetSymbol(typeSymbol->name).value;
        baseType = typeSymbol->name;
        if (type.IsMutable())
            scope = SPIRVResult::Storage::StorageBuffer;
        else if (storage == Variable::__Resolved::Storage::Uniform)
            scope = SPIRVResult::Storage::Uniform;
        else if (storage == Variable::__Resolved::Storage::InlineUniform)
            scope = SPIRVResult::Storage::PushConstant;
        else if (storage == Variable::__Resolved::Storage::Global)
            scope = SPIRVResult::Storage::Private;            
    }

    std::string scopeString = SPIRVResult::ScopeToString(scope);
    for (size_t i = 0; i < type.modifiers.size(); i++)
    {
        const Type::FullType::Modifier& mod = type.modifiers[i];
        if (mod == Type::FullType::Modifier::Pointer)
        {
            std::string newBase = Format("ptr(%s)%s", baseType.c_str(), scopeString.c_str());
            parentType.push_back(name);
            name = generator->AddSymbol(newBase, Format("OpTypePointer %s %%%d", scopeString.c_str(), name), true);
            baseType = newBase;
        }
        else if (mod == Type::FullType::Modifier::Array)
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
                parentType.push_back(name);
                name = generator->AddSymbol(newBase, Format("OpTypeArray %%%d %%%d", name, arraySizeConstant), true);
                generator->AddDecoration(newBase, name, Format("ArrayStride %d", typeSymbol->CalculateAlignment()));
                baseType = newBase;
            }
        }
    }

    return SPIRVResult(0xFFFFFFFF, name, false, false, scope, parentType);
}

struct ConstantCreationInfo
{
    enum class Type
    {
        Float,
        Float16,
        Int,
        Int16,
        UInt,
        UInt16,
        Bool
    } type;
    union
    {
        float f;
        int32_t i;
        uint32_t ui;
        bool b;
    } data;
    bool linkDefined = false;

    static ConstantCreationInfo Float(float val)
    {
        ConstantCreationInfo ret;
        ret.type = Type::Float;
        ret.data.f = val;
        return ret;
    }

    static ConstantCreationInfo Float16(float val)
    {
        ConstantCreationInfo ret;
        ret.type = Type::Float16;
        ret.data.f = val;
        return ret;
    }

    static ConstantCreationInfo UInt(uint32_t val)
    {
        ConstantCreationInfo ret;
        ret.type = Type::UInt;
        ret.data.ui = val;
        return ret;
    }

    static ConstantCreationInfo UInt16(uint16_t val)
    {
        ConstantCreationInfo ret;
        ret.type = Type::UInt16;
        ret.data.ui = val;
        return ret;
    }

    static ConstantCreationInfo Int(int32_t val)
    {
        ConstantCreationInfo ret;
        ret.type = Type::Int;
        ret.data.i = val;
        return ret;
    }

    static ConstantCreationInfo Int16(int16_t val)
    {
        ConstantCreationInfo ret;
        ret.type = Type::Int16;
        ret.data.i = val;
        return ret;
    }

    static ConstantCreationInfo Bool(bool b)
    {
        ConstantCreationInfo ret;
        ret.type = Type::Bool;
        ret.data.b = b;
        return ret;
    }
};

//------------------------------------------------------------------------------
/**
*/
std::string
SwizzleMaskToIndices(const Type::SwizzleMask mask, bool offset = false)
{
    std::string swizzleIndices;
    if (mask.bits.x != Type::SwizzleMask::Component::Invalid)
        swizzleIndices.append(Format("%d ", mask.bits.x + (offset ? 4 : 0)));
    if (mask.bits.y != Type::SwizzleMask::Component::Invalid)
        swizzleIndices.append(Format("%d ", mask.bits.y + (offset ? 4 : 0)));
    if (mask.bits.z != Type::SwizzleMask::Component::Invalid)
        swizzleIndices.append(Format("%d ", mask.bits.z + (offset ? 4 : 0)));
    if (mask.bits.w != Type::SwizzleMask::Component::Invalid)
        swizzleIndices.append(Format("%d ", mask.bits.w + (offset ? 4 : 0)));
    return swizzleIndices;
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateConstantSPIRV(Compiler* compiler, SPIRVGenerator* generator, ConstantCreationInfo info)
{
    SPIRVResult res = SPIRVResult::Invalid();
    const char* baseFormat = generator->linkDefineEvaluation ? "OpSpecConstant" : "OpConstant";
    switch (info.type)
    {
        case ConstantCreationInfo::Type::UInt:
        {
            uint32_t baseType = generator->AddSymbol("u32", "OpTypeInt 32 0", true);
            res.typeName = baseType;
            if (generator->linkDefineEvaluation)
                res.name = generator->AddSymbol(Format("%du_link_defined%d", info.data.ui, generator->linkDefineSlot), Format("OpSpecConstant %%%d %d", baseType, info.data.ui), true);
            else
                res.name = generator->AddSymbol(Format("%du", info.data.ui), Format("OpConstant %%%d %d", baseType, info.data.ui), true);
            break;
        }
        case ConstantCreationInfo::Type::UInt16:
        {
            uint32_t baseType = generator->AddSymbol("u16", "OpTypeInt 16 0", true);
            res.typeName = baseType;
            if (generator->linkDefineEvaluation)
                res.name = generator->AddSymbol(Format("%du_link_defined%d", info.data.ui, generator->linkDefineSlot), Format("OpSpecConstant %%%d %d", baseType, info.data.ui), true);
            else
                res.name = generator->AddSymbol(Format("%du", info.data.ui), Format("OpConstant %%%d %d", baseType, info.data.ui), true);
            break;
        }
        case ConstantCreationInfo::Type::Int:
        {
            uint32_t baseType = generator->AddSymbol("i32", "OpTypeInt 32 1", true);
            res.typeName = baseType;
            if (generator->linkDefineEvaluation)
                res.name = generator->AddSymbol(Format("%di_link_defined%d", info.data.i, generator->linkDefineSlot), Format("OpSpecConstant %%%d %d", baseType, info.data.i), true);
            else
                res.name = generator->AddSymbol(Format("%di", info.data.i), Format("OpConstant %%%d %d", baseType, info.data.i), true);
            break;
        }
        case ConstantCreationInfo::Type::Int16:
        {
            uint32_t baseType = generator->AddSymbol("i16", "OpTypeInt 16 1", true);
            res.typeName = baseType;
            if (generator->linkDefineEvaluation)
                res.name = generator->AddSymbol(Format("%di_link_defined%d", info.data.i, generator->linkDefineSlot), Format("OpSpecConstant %%%d %d", baseType, info.data.i), true);
            else
                res.name = generator->AddSymbol(Format("%di", info.data.i), Format("OpConstant %%%d %d", baseType, info.data.i), true);
            break;
        }
        case ConstantCreationInfo::Type::Float:
        {
            uint32_t baseType = generator->AddSymbol("f32", "OpTypeFloat 32", true);
            res.typeName = baseType;
            if (generator->linkDefineEvaluation)
                res.name = generator->AddSymbol(Format("%ff_link_defined%d", info.data.f, generator->linkDefineSlot), Format("OpSpecConstant %%%d %f", baseType, info.data.f), true);
            else
                res.name = generator->AddSymbol(Format("%ff", info.data.f), Format("OpConstant %%%d %f", baseType, info.data.f), true);
            break;
        }
        case ConstantCreationInfo::Type::Float16:
        {
            uint32_t baseType = generator->AddSymbol("f16", "OpTypeFloat 16", true);
            res.typeName = baseType;
            if (generator->linkDefineEvaluation)
                res.name = generator->AddSymbol(Format("%ff_link_defined%d", info.data.f, generator->linkDefineSlot), Format("OpSpecConstant %%%d %f", baseType, info.data.f), true);
            else
                res.name = generator->AddSymbol(Format("%ff", info.data.f), Format("OpConstant %%%d %f", baseType, info.data.f), true);
            break;
        }
        case ConstantCreationInfo::Type::Bool:
        {
            uint32_t baseType = generator->AddSymbol("b8", "OpTypeBool", true);
            res.typeName = baseType;
            if (generator->linkDefineEvaluation)
                res.name = generator->AddSymbol(info.data.b ? Format("true_link_defined%d", generator->linkDefineSlot) : Format("false_link_defined%d", generator->linkDefineSlot), info.data.b ? Format("OpSpecConstantTrue %%%d", baseType) : Format("OpSpecConstantFalse %%%d", baseType), true);
            else
                res.name = generator->AddSymbol(info.data.b ? "true" : "false", info.data.b ? Format("OpConstantTrue %%%d", baseType) : Format("OpConstantFalse %%%d", baseType), true);
            break;
        }
    }
    res.isValue = true;
    res.isConst = true;
    res.isSpecialization = generator->linkDefineEvaluation;
    return res;
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
LoadValueSPIRV(Compiler* compiler, SPIRVGenerator* generator, SPIRVResult arg)
{
    if (arg.isLiteral)
    {
        switch (arg.literalValue.type)
        {
        case SPIRVResult::LiteralValue::FloatType:
            arg = GenerateConstantSPIRV(compiler, generator, GPULang::ConstantCreationInfo::Float(arg.literalValue.f));
            break;
        case SPIRVResult::LiteralValue::IntType:
            arg = GenerateConstantSPIRV(compiler, generator, GPULang::ConstantCreationInfo::Int(arg.literalValue.i));
            break;
        case SPIRVResult::LiteralValue::UIntType:
            arg = GenerateConstantSPIRV(compiler, generator, GPULang::ConstantCreationInfo::UInt(arg.literalValue.ui));
            break;
        case SPIRVResult::LiteralValue::BoolType:
            arg = GenerateConstantSPIRV(compiler, generator, GPULang::ConstantCreationInfo::Bool(arg.literalValue.b));
            break;
        }
        return arg;
    }

    uint32_t val = arg.name;

    if (!arg.isValue)
        val = generator->AddMappedOp(Format("OpLoad %%%d %%%d", arg.typeName, val));
    
    if (arg.swizzleMask != Type::SwizzleMask())
    {
        std::string swizzleIndices = SwizzleMaskToIndices(arg.swizzleMask);
        val = generator->AddMappedOp(Format("OpVectorShuffle %%%d %%%d %%%d %s", arg.swizzleType, val, val, swizzleIndices.c_str()));
    }
    auto res = SPIRVResult(val, arg.typeName);
    res.isValue = true;
    res.isConst = arg.isConst;
    res.isSpecialization = arg.isSpecialization;
    return res;
}


enum class ConversionTable
{
    None,
    IntToFloat,
    IntToUInt,
    UIntToFloat,
    UIntToInt,
    FloatToUInt,
    FloatToInt,
};

std::unordered_map<ConversionTable, std::function<SPIRVResult(Compiler*, SPIRVGenerator*, uint32_t vectorSize, SPIRVResult)>> converters =
{
    { ConversionTable::IntToFloat, [](Compiler* c, SPIRVGenerator* g, uint32_t vectorSize, SPIRVResult value) -> SPIRVResult {
        uint32_t type = g->AddSymbol("f32", "OpTypeFloat 32", true);        
        if (vectorSize > 1)
            type = g->AddSymbol(Format("f32x%d", vectorSize), Format("OpTypeVector %%%d %d", type, vectorSize), true);
        value = LoadValueSPIRV(c, g, value);
        return SPIRVResult(g->AddMappedOp(Format("OpConvertSToF %%%d %%%d", type, value.name)), type, true, value.isConst);
    } }
    , { ConversionTable::IntToUInt, [](Compiler* c, SPIRVGenerator* g, uint32_t vectorSize, SPIRVResult value) -> SPIRVResult {
        uint32_t type = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        if (vectorSize > 1)
            type = g->AddSymbol(Format("u32x%d", vectorSize), Format("OpTypeVector %%%d %d", type, vectorSize), true);
        value = LoadValueSPIRV(c, g, value);
        return SPIRVResult(g->AddMappedOp(Format("OpBitcast %%%d %%%d", type, value.name)), type, true, value.isConst);
    } }
    , { ConversionTable::UIntToInt, [](Compiler* c, SPIRVGenerator* g, uint32_t vectorSize, SPIRVResult value) -> SPIRVResult {
        uint32_t type = g->AddSymbol("i32", "OpTypeInt 32 1", true);
        if (vectorSize > 1)
            type = g->AddSymbol(Format("i32x%d", vectorSize), Format("OpTypeVector %%%d %d", type, vectorSize), true);
        value = LoadValueSPIRV(c, g, value);
        return SPIRVResult(g->AddMappedOp(Format("OpBitcast %%%d %%%d", type, value.name)), type, true, value.isConst);
    } }
    , { ConversionTable::UIntToFloat, [](Compiler* c, SPIRVGenerator* g, uint32_t vectorSize, SPIRVResult value) -> SPIRVResult {
        uint32_t type = g->AddSymbol("f32", "OpTypeFloat 32", true);
        if (vectorSize > 1)
            type = g->AddSymbol(Format("f32x%d", vectorSize), Format("OpTypeVector %%%d %d", type, vectorSize), true);
        value = LoadValueSPIRV(c, g, value);
        return SPIRVResult(g->AddMappedOp(Format("OpConvertUToF %%%d %%%d", type, value.name)), type, true, value.isConst);
    } }
    , { ConversionTable::FloatToUInt, [](Compiler* c, SPIRVGenerator* g, uint32_t vectorSize, SPIRVResult value) -> SPIRVResult {
        uint32_t type = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        if (vectorSize > 1)
            type = g->AddSymbol(Format("u32x%d", vectorSize), Format("OpTypeVector %%%d %d", type, vectorSize), true);
        value = LoadValueSPIRV(c, g, value);
        return SPIRVResult(g->AddMappedOp(Format("OpConvertFToU %%%d %%%d", type, value.name)), type, true, value.isConst);
    } }
    , { ConversionTable::FloatToInt, [](Compiler* c, SPIRVGenerator* g, uint32_t vectorSize, SPIRVResult value) -> SPIRVResult {
        uint32_t type = g->AddSymbol("i32", "OpTypeInt 32 1", true);
        if (vectorSize > 1)
            type = g->AddSymbol(Format("i32x%d", vectorSize), Format("OpTypeVector %%%d %d", type, vectorSize), true);
        value = LoadValueSPIRV(c, g, value);
        return SPIRVResult(g->AddMappedOp(Format("OpConvertFToS %%%d %%%d", type, value.name)), type, true, value.isConst);
    } }
};

//------------------------------------------------------------------------------
/**
*/
void
GenerateConversionsSPIRV(Compiler* compiler, SPIRVGenerator* generator, ConversionTable conversion, uint32_t vectorSize, const std::vector<SPIRVResult>& inArgs, std::vector<SPIRVResult>& outValues)
{
    for (size_t i = 0; i < inArgs.size(); i++)
    {
        outValues.push_back(converters[conversion](compiler, generator, vectorSize, inArgs[i]));
    }
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateConversionSPIRV(Compiler* compiler, SPIRVGenerator* generator, ConversionTable conversion, uint32_t vectorSize, SPIRVResult inArg)
{
    return converters[conversion](compiler, generator, vectorSize, inArg);
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateCompositeSPIRV(Compiler* compiler, SPIRVGenerator* generator, uint32_t returnType, const std::vector<SPIRVResult>& args)
{
    std::string argList = "";
    bool isConst = true;
    bool isSpecialization = false;
    for (const SPIRVResult& arg : args)
    {
        SPIRVResult loaded = LoadValueSPIRV(compiler, generator, arg);
        argList.append(Format("%%%d ", loaded.name));
        if (!loaded.isConst)
            isConst = false;
        isSpecialization |= loaded.isSpecialization;
    }
    if (isConst)
        if (isSpecialization)
            return SPIRVResult(generator->AddSymbol(Format("%%%d_composite(%s)", returnType, argList.c_str()), Format("OpSpecConstantComposite %%%d %s", returnType, argList.c_str()), true), returnType, true, true);
        else
            return SPIRVResult(generator->AddSymbol(Format("%%%d_composite(%s)", returnType, argList.c_str()), Format("OpConstantComposite %%%d %s", returnType, argList.c_str()), true), returnType, true, true);
    else
    {
        assert(!generator->linkDefineEvaluation);
        return SPIRVResult(generator->AddMappedOp(Format("OpCompositeConstruct %%%d %s", returnType, argList.c_str())), returnType, true);
    }
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateSplatCompositeSPIRV(Compiler* compiler, SPIRVGenerator* generator, uint32_t returnType, uint32_t num, SPIRVResult arg)
{
    std::vector<SPIRVResult> splat(num, arg);
    return GenerateCompositeSPIRV(compiler, generator, returnType, splat);
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateConvertAndSplatCompositeSPIRV(Compiler* compiler, SPIRVGenerator* generator, uint32_t returnType, const std::vector<SPIRVResult>& args, ConversionTable conversion)
{
    std::vector<SPIRVResult> converted;
    GenerateConversionsSPIRV(compiler, generator, conversion, 1, args, converted);
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
    GenerateConversionsSPIRV(compiler, generator, conversion, 1, args, converted);
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
    this->intrinsicMap[&Float4::ctor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 4);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Float3::ctor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 3);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Float2::ctor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 2);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Float::ctor_UInt] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateConversionSPIRV(c, g, ConversionTable::UIntToFloat, 1, args[0]);
    };
    this->intrinsicMap[&Float::ctor_Int] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateConversionSPIRV(c, g, ConversionTable::IntToFloat, 1, args[0]);
    };
    this->intrinsicMap[&Int4::ctor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 4);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Int3::ctor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 3);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Int2::ctor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 2);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&Int::ctor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateConversionSPIRV(c, g, ConversionTable::UIntToInt, 1, args[0]);
    };
    this->intrinsicMap[&UInt4::ctor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 4);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&UInt3::ctor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 3);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&UInt2::ctor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 2);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    this->intrinsicMap[&UInt::ctor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateConversionSPIRV(c, g, ConversionTable::IntToUInt, 1, args[0]);
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

#define X(type, ctor, val, argCount, splat, vectorSize, conversion)\
    this->intrinsicMap[&type::ctor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {\
        assert(args.size() == argCount);\
        if constexpr (splat > 1)\
            return GenerateSplatCompositeSPIRV(c, g, returnType, splat, args[0]);\
        if constexpr (ConversionTable::conversion != ConversionTable::None)\
            return GenerateConversionSPIRV(c, g, ConversionTable::conversion, vectorSize, args[0]);\
    };

    FLOAT2_CTOR_LIST
    FLOAT3_CTOR_LIST
    FLOAT4_CTOR_LIST
    INT2_CTOR_LIST
    INT3_CTOR_LIST
    INT4_CTOR_LIST
    UINT2_CTOR_LIST
    UINT3_CTOR_LIST
    UINT4_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    this->intrinsicMap[&type::ctor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {\
        return GenerateCompositeSPIRV(c, g, returnType, { args[0], args[1] });\
    };

    FLOAT3_CTOR2_LIST
    FLOAT4_CTOR2_LIST
    INT3_CTOR2_LIST
    INT4_CTOR2_LIST
    UINT3_CTOR2_LIST
    UINT4_CTOR2_LIST
#undef X

#define X(type, ctor, arg0, arg1, arg2)\
    this->intrinsicMap[&type::ctor] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {\
        return GenerateCompositeSPIRV(c, g, returnType, { args[0], args[1], args[2] });\
    };

    FLOAT4_CTOR3_LIST
    INT4_CTOR3_LIST
    UINT4_CTOR3_LIST
#undef X

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
            return GenerateConversionSPIRV(c, g, conv, 1, args[0]);
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
        , { &Mat4x2::multiplyAssignOperator, true }
        , { &Mat4x3::multiplyAssignOperator, true }
        , { &Mat4x4::multiplyAssignOperator, true }
    };
    for (auto fun : matrixMultiplicationFunctions)
    {
        this->intrinsicMap[fun.first] = [assign = fun.second](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            uint32_t ret = g->AddMappedOp(Format("OpMatrixTimesMatrix %%%d %%%d %%%d", returnType, args[0].name, args[1].name));
            if (assign)
            {
                g->AddOp(Format("OpStore %%%d %%%d", args[2].name, ret));
                return SPIRVResult(args[2].name, args[2].typeName);
            }
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
            {
                g->AddOp(Format("OpStore %%%d %%%d", args[2].name, ret));
                return SPIRVResult(args[2].name, args[2].typeName);
            }
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

#define MAKE_EXPORT_INTRINSICS(op)\
      { Intrinsics::##op##_f32, TypeCode::Float, 1 }\
    , { Intrinsics::##op##_f32x2, TypeCode::Float, 2 }\
    , { Intrinsics::##op##_f32x3, TypeCode::Float, 3 }\
    , { Intrinsics::##op##_f32x4, TypeCode::Float, 4 }\
    , { Intrinsics::##op##_i32, TypeCode::Int, 1 }\
    , { Intrinsics::##op##_i32x2, TypeCode::Int, 2 }\
    , { Intrinsics::##op##_i32x3, TypeCode::Int, 3 }\
    , { Intrinsics::##op##_i32x4, TypeCode::Int, 4 }\
    , { Intrinsics::##op##_u32, TypeCode::UInt, 1 }\
    , { Intrinsics::##op##_u32x2, TypeCode::UInt, 2 }\
    , { Intrinsics::##op##_u32x3, TypeCode::UInt, 3 }\
    , { Intrinsics::##op##_u32x4, TypeCode::UInt, 4 }

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
        this->intrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            assert(args.size() == 2);
            uint32_t ret = g->AddMappedOp(Format("OpFwidth %%%d %%%D", returnType, args[0].name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    this->intrinsicMap[Intrinsics::GetOutputLayer] = [](Compiler* c, SPIRVGenerator * g, uint32_t returnType, const std::vector<SPIRVResult>&args) -> SPIRVResult
    {
        g->AddCapability("ShaderLayer");
        uint32_t baseType = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        uint32_t typePtr = g->AddSymbol("ptr(u32)Input", Format("OpTypePointer Input %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplOutputLayer", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplOutputLayer", ret, "BuiltIn Layer");
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Storage::Input);
    };

    this->intrinsicMap[Intrinsics::SetOutputLayer] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->AddCapability("ShaderLayer");
        uint32_t baseType = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        uint32_t typePtr = g->AddSymbol("ptr(u32)Output", Format("OpTypePointer Output %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplOutputLayer", Format("OpVariable %%%d Output", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplOutputLayer", ret, "BuiltIn Layer");
        g->AddOp(Format("OpStore %%%d %%%d", ret, args[0].name));
        return SPIRVResult::Invalid();
    };

    this->intrinsicMap[Intrinsics::GetOutputViewport] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult 
    {
        g->AddCapability("ShaderViewportIndex");
        uint32_t baseType = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        uint32_t typePtr = g->AddSymbol("ptr(u32)Input", Format("OpTypePointer Input %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplOutputViewport", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplOutputViewport", ret, "BuiltIn ViewportIndex");
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Storage::Input);
    };

    this->intrinsicMap[Intrinsics::SetOutputViewport] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->AddCapability("ShaderViewportIndex");
        uint32_t baseType = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        uint32_t typePtr = g->AddSymbol("ptr(u32)Output", Format("OpTypePointer Output %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplOutputViewport", Format("OpVariable %%%d Output", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplOutputViewport", ret, "BuiltIn ViewportIndex");
        g->AddOp(Format("OpStore %%%d %%%d", ret, args[0].name));
        return SPIRVResult::Invalid();
    };

    this->intrinsicMap[Intrinsics::ExportVertexCoordinates] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->AddCapability("Shader");
        uint32_t baseType = g->AddSymbol("f32", "OpTypeFloat 32", true);
        uint32_t vecType = g->AddSymbol("f32x4", Format("OpTypeVector %%%d 4", baseType), true);
        uint32_t typePtr = g->AddSymbol("ptr(f32x4)Output", Format("OpTypePointer Output %%%d", vecType), true);
        uint32_t ret = g->AddSymbol("gplVertexCoordinates", Format("OpVariable %%%d Output", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplVertexCoordinates", ret, "BuiltIn Position");
        SPIRVResult loaded = LoadValueSPIRV(c, g, args[0]);
        g->AddOp(Format("OpStore %%%d %%%d", ret, loaded.name), false, "gplExportVertexCoordinates");
        return SPIRVResult::Invalid();
    };

    this->intrinsicMap[Intrinsics::ExportVertex] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->AddCapability("Geometry");
        g->AddOp("OpEmitVertex");
        return SPIRVResult::Invalid();
    };

    this->intrinsicMap[Intrinsics::ExportPrimitive] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->AddCapability("Geometry");
        g->AddOp("OpEndPrimitive");
        return SPIRVResult::Invalid();
    };

    this->intrinsicMap[Intrinsics::GetVertexIndex] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->AddCapability("Shader");
        uint32_t baseType = g->AddSymbol("u32", "OpTypeInt 32 0");
        uint32_t typePtr = g->AddSymbol("ptr(u32)Input", Format("OpTypePointer Input %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplVertexIndex", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplVertexIndex", ret, "BuiltIn VertexId");
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Storage::Input);
    };

    this->intrinsicMap[Intrinsics::GetInstanceIndex] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->AddCapability("Shader");
        uint32_t baseType = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        uint32_t typePtr = g->AddSymbol("ptr(u32)Input", Format("OpTypePointer Input %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplInstanceIndex", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplInstanceIndex", ret, "BuiltIn InstanceId");
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Storage::Input);
    };

    this->intrinsicMap[Intrinsics::GetPixelCoordinates] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->AddCapability("Shader");
        uint32_t baseType = g->AddSymbol("f32", "OpTypeFloat 32", true);
        uint32_t vecType = g->AddSymbol("f32x4", Format("OpTypeVector %%%d 4", baseType), true);
        uint32_t typePtr = g->AddSymbol("ptr(f32x4)Input", Format("OpTypePointer Input %%%d", vecType), true);
        uint32_t ret = g->AddSymbol("gplPixelCoordinates", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplPixelCoordinates", ret, "BuiltIn FragCoord");
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Storage::Input);
    };

    this->intrinsicMap[Intrinsics::SetPixelDepth] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->AddCapability("Shader");
        uint32_t baseType = g->AddSymbol("f32", "OpTypeFloat 32", true);
        uint32_t vecType = g->AddSymbol("f32x4", Format("OpTypeVector %%%d 4", baseType), true);
        uint32_t typePtr = g->AddSymbol("ptr(f32x4)Output", Format("OpTypePointer Output %%%d", vecType), true);
        uint32_t ret = g->AddSymbol("gplPixelDepth", Format("OpVariable %%%d Output", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplPixelDepth", ret, "BuiltIn FragDepth");
        SPIRVResult loaded = LoadValueSPIRV(c, g, args[0]);
        g->AddOp(Format("OpStore %%%d %%%d", ret, loaded.name), false, "gplSetPixelDepth");
        return SPIRVResult::Invalid();
    };

    std::vector<std::tuple<Function*, TypeCode, uint32_t>> colorExportIntrinsics =
    {
        MAKE_EXPORT_INTRINSICS(ExportColor)
    };
    for (auto fun : colorExportIntrinsics)
    {
        this->intrinsicMap[std::get<0>(fun)] = [baseTy = std::get<1>(fun), vecSize = std::get<2>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            g->AddCapability("Shader");
            uint32_t baseType;
            std::string name;
            switch (baseTy)
            {
                case TypeCode::Float:
                    baseType = g->AddSymbol("f32", "OpTypeFloat 32", true);
                    name = "f32";
                    break;
                case TypeCode::Int:
                    baseType = g->AddSymbol("i32", "OpTypeInt 32 1", true);
                    name = "i32";
                    break;
                case TypeCode::UInt:
                    baseType = g->AddSymbol("u32", "OpTypeInt 32 0", true);
                    name = "u32";
                    break;
                case TypeCode::Float16:
                    baseType = g->AddSymbol("f16", "OpTypeFloat 16", true);
                    name = "f16";
                    break;
                case TypeCode::Int16:
                    baseType = g->AddSymbol("i16", "OpTypeInt 16 1", true);
                    name = "i16";
                    break;
                case TypeCode::UInt16:
                    baseType = g->AddSymbol("u16", "OpTypeInt 16 0", true);
                    name = "u16";
                    break;
                default:
                    baseType = -1;
                    assert(false);
            }
            if (vecSize > 1)
            {
                name += Format("x%d", vecSize);
                baseType = g->AddSymbol(name, Format("OpTypeVector %%%d 4", baseType), true);
            }
            assert(args[1].isLiteral);
            uint32_t typePtr = g->AddSymbol(Format("ptr(%s)Output", name.c_str()), Format("OpTypePointer Output %%%d", baseType), true);
            uint32_t ret = g->AddSymbol(Format("gplExportColor%d", args[1].literalValue.i), Format("OpVariable %%%d Output", typePtr), true);
            g->interfaceVariables.insert(ret);
            g->AddDecoration(Format("gplExportColorIndex%d", args[1].literalValue.i), ret, Format("Index %d", args[1].literalValue.i));
            g->AddDecoration(Format("gplExportColorLocation%d", args[1].literalValue.i), ret, Format("Location %d", args[1].literalValue.i));
            SPIRVResult loaded = LoadValueSPIRV(c, g, args[0]);
            g->AddOp(Format("OpStore %%%d %%%d", ret, loaded.name), false, Format("gplExportColor%d", args[1].literalValue.i));
            return SPIRVResult::Invalid();
        };
    }

    this->intrinsicMap[Intrinsics::GetLocalInvocationIndex] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        uint32_t baseType = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        uint32_t vecType = g->AddSymbol("u32x3", Format("OpTypeVector %%%d 3", baseType), true);
        uint32_t typePtr = g->AddSymbol("ptr(u32x3)Input", Format("OpTypePointer Input %%%d", vecType), true);
        uint32_t ret = g->AddSymbol("gplLocalInvocationIndices", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplLocalInvocationIndices", ret, "BuiltIn LocalInvocationId");
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Storage::Input);
    };

    this->intrinsicMap[Intrinsics::GetGlobalInvocationIndex] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        uint32_t baseType = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        uint32_t vecType = g->AddSymbol("u32x3", Format("OpTypeVector %%%d 3", baseType), true);
        uint32_t typePtr = g->AddSymbol("ptr(u32x3)Input", Format("OpTypePointer Input %%%d", vecType), true);
        uint32_t ret = g->AddSymbol("gplGlobalInvocationIndices", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplGlobalInvocationIndices", ret, "BuiltIn GlobalInvocationId");
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Storage::Input);
    };

    this->intrinsicMap[Intrinsics::GetWorkGroupIndex] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        uint32_t baseType = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        uint32_t vecType = g->AddSymbol("u32x3", Format("OpTypeVector %%%d 3", baseType), true);
        uint32_t typePtr = g->AddSymbol("ptr(u32x3)Input", Format("OpTypePointer Input %%%d", vecType), true);
        uint32_t ret = g->AddSymbol("gplWorkGroupIndices", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplWorkGroupIndices", ret, "BuiltIn WorkgroupId");
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Storage::Input);
    };

    this->intrinsicMap[Intrinsics::GetWorkGroupDimensions] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        uint32_t baseType = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        uint32_t vecType = g->AddSymbol("u32x3", Format("OpTypeVector %%%d 3", baseType), true);
        uint32_t typePtr = g->AddSymbol("ptr(u32x3)Input", Format("OpTypePointer Input %%%d", vecType), true);
        uint32_t ret = g->AddSymbol("gplWorkGroupDimensions", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplWorkGroupDimensions", ret, "BuiltIn WorkgroupSize");
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Storage::Input);
    };

    this->intrinsicMap[Intrinsics::GetSubgroupId] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniform");
        uint32_t baseType = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        uint32_t vecType = g->AddSymbol("u32x3", Format("OpTypeVector %%%d 3", baseType), true);
        uint32_t typePtr = g->AddSymbol("ptr(u32x3)Input", Format("OpTypePointer Input %%%d", vecType), true);
        uint32_t ret = g->AddSymbol("gplSubgroupId", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplSubgroupId", ret, "BuiltIn SubgroupId");
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Storage::Input);
    };

    this->intrinsicMap[Intrinsics::GetSubgroupSize] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniform");
        uint32_t baseType = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        uint32_t vecType = g->AddSymbol("u32x3", Format("OpTypeVector %%%d 3", baseType), true);
        uint32_t typePtr = g->AddSymbol("ptr(u32x3)Input", Format("OpTypePointer Input %%%d", vecType), true);
        uint32_t ret = g->AddSymbol("gplSubgroupSize", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplSubgroupSize", ret, "BuiltIn SubgroupSize");
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Storage::Input);
    };

    this->intrinsicMap[Intrinsics::GetNumSubgroups] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniform");
        uint32_t baseType = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        uint32_t vecType = g->AddSymbol("u32x3", Format("OpTypeVector %%%d 3", baseType), true);
        uint32_t typePtr = g->AddSymbol("ptr(u32x3)Input", Format("OpTypePointer Input %%%d", vecType), true);
        uint32_t ret = g->AddSymbol("gplNumSubgroups", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplNumSubgroups", ret, "BuiltIn NumSubgroups");
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Storage::Input);
    };

    this->intrinsicMap[Intrinsics::GetSubgroupLocalInvocationMask] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniform");
        uint32_t baseType = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        uint32_t typePtr = g->AddSymbol("ptr(u32)Input", Format("OpTypePointer Input %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplSubgroupLocalInvocationMask", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplSubgroupLocalInvocationMask", ret, "BuiltIn SubgroupEqMask");
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Storage::Input);
    };

    this->intrinsicMap[Intrinsics::GetSubgroupLocalInvocationAndLowerMask] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniform");
        uint32_t baseType = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        uint32_t typePtr = g->AddSymbol("ptr(u32)Input", Format("OpTypePointer Input %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplSubgroupLocalInvocationAndLowerMask", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplSubgroupLocalInvocationAndLowerMask", ret, "BuiltIn SubgroupLeMask");
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Storage::Input);
    };

    this->intrinsicMap[Intrinsics::GetSubgroupLowerMask] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniform");
        uint32_t baseType = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        uint32_t typePtr = g->AddSymbol("ptr(u32)Input", Format("OpTypePointer Input %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplNumSubgroups", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplSubgroupLowerMask", ret, "BuiltIn SubgroupLtMask");
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Storage::Input);
    };

    this->intrinsicMap[Intrinsics::GetSubgroupLocalInvocationAndGreaterMask] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniform");
        uint32_t baseType = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        uint32_t typePtr = g->AddSymbol("ptr(u32)Input", Format("OpTypePointer Input %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplSubgroupLocalInvocationAndGreaterMask", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplSubgroupLocalInvocationAndGreaterMask", ret, "BuiltIn SubgroupGeMask");
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Storage::Input);
    };

    this->intrinsicMap[Intrinsics::GetSubgroupGreaterMask] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniform");
        uint32_t baseType = g->AddSymbol("u32", "OpTypeInt 32 0", true);
        uint32_t typePtr = g->AddSymbol("ptr(u32)Input", Format("OpTypePointer Input %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplSubgroupGreaterMask", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplSubgroupGreaterMask", ret, "BuiltIn SubgroupGtMask");
        return SPIRVResult(ret, returnType, false, false, SPIRVResult::Storage::Input);
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

    std::vector<std::tuple<Function*, const char>> subgroupSwapIntrinsics =
    {
        MAKE_SCALAR_INTRINSICS(SubgroupSwapDiagonal)
    };
    for (auto fun : subgroupSwapIntrinsics)
    {
        this->intrinsicMap[std::get<0>(fun)] =  [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            g->AddMappedOp(Format("OpGroupNonUniformQuadSwap %%%d"))
        };
    }

    this->intrinsicMap[Intrinsics::ExecutionBarrier] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        return SPIRVResult(ret, returnType);
    };

    this->intrinsicMap[Intrinsics::MemoryBarrier] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        return SPIRVResult(ret, returnType);
    }; 

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
            assert(args[0].parentTypes.size() > 0);
            uint32_t image = g->AddMappedOp(Format("OpLoad %%%d %%%d", args[0].parentTypes[0], args[0].name));
            uint32_t sampler = args[1].name;
            if (!args[1].isConst)
                sampler = g->AddMappedOp(Format("OpLoad %%%d %%%d", args[1].parentTypes[0], args[1].name));
            uint32_t sampledImageType = g->AddSymbol(Format("sampledType(%d)", args[0].parentTypes[0]), Format("OpTypeSampledImage %%%d", args[0].parentTypes[0]), true);
            uint32_t sampledImage = g->AddMappedOp(Format("OpSampledImage %%%d %%%d %%%d", sampledImageType, image, sampler));

            std::string format = "";
            if (operands & Lod || operands & Grad)
                if (operands & Comp)
                    if (operands & Grad)
                        if (operands & Proj)
                            if (operands & Bias)
                                format = "OpImageSampleProjDrefExplicitLod %%%d %%%d %%%d %%%d Grad %%%d Bias %%%d";
                            else
                                format = "OpImageSampleProjDrefExplicitLod %%%d %%%d %%%d %%%d Grad %%%d";
                        else
                            if (operands & Bias)
                                format = "OpImageSampleDrefExplicitLod %%%d %%%d %%%d %%%d Grad %%%d Bias %%%d";
                            else
                                format = "OpImageSampleDrefExplicitLod %%%d %%%d %%%d %%%d Grad %%%d";
                    else
                        if (operands & Proj)
                            if (operands & Bias)
                                format = "OpImageSampleProjDrefExplicitLod %%%d %%%d %%%d %%%d Lod %%%d Bias";
                            else
                                format = "OpImageSampleProjDrefExplicitLod %%%d %%%d %%%d %%%d Lod %%%d";
                        else
                            if (operands & Bias)
                                format = "OpImageSampleDrefExplicitLod %%%d %%%d %%%d %%%d Lod %%%d Bias %%%d";
                            else
                                format = "OpImageSampleDrefExplicitLod %%%d %%%d %%%d %%%d Lod %%%d";
                else
                    if (operands & Grad)
                        if (operands & Proj)
                            if (operands & Bias)
                                format = "OpImageSampleProjExplicitLod %%%d %%%d %%%d Grad %%%d Bias %%%d";
                            else
                                format = "OpImageSampleProjExplicitLod %%%d %%%d %%%d Grad %%%d";
                        else
                            if (operands & Bias)
                                format = "OpImageSampleExplicitLod %%%d %%%d %%%d Grad %%%d Bias %%%d";
                            else
                                format = "OpImageSampleExplicitLod %%%d %%%d %%%d Grad %%%d";
                    else
                        if (operands & Proj)
                            if (operands & Bias)
                                format = "OpImageSampleProjExplicitLod %%%d %%%d %%%d Lod %%%d Bias %%%d";
                            else
                                format = "OpImageSampleProjExplicitLod %%%d %%%d %%%d Lod %%%d";
                        else
                            if (operands & Bias)
                                format = "OpImageSampleExplicitLod %%%d %%%d %%%d Lod %%%d Bias %%%d";
                            else
                                format = "OpImageSampleExplicitLod %%%d %%%d %%%d Lod %%%d";
            else
                if (operands & Comp)
                    if (operands & Proj)
                        if (operands & Bias)
                            format = "OpImageSampleProjDrefImplicitLod %%%d %%%d %%%d %%%d Bias %%%d";
                        else
                            format = "OpImageSampleProjDrefImplicitLod %%%d %%%d %%%d %%%d None";
                    else
                        if (operands & Bias)
                            format = "OpImageSampleDrefImplicitLod %%%d %%%d %%%d %%%d Bias";
                        else
                            format = "OpImageSampleDrefImplicitLod %%%d %%%d %%%d %%%d None";
                else
                    if (operands & Proj)
                        if (operands & Bias)
                            format = "OpImageSampleProjImplicitLod %%%d %%%d %%%d Bias %%%d";
                        else
                            format = "OpImageSampleProjImplicitLod %%%d %%%d %%%d None";
                    else
                        if (operands & Bias)
                            format = "OpImageSampleImplicitLod %%%d %%%d %%%d Bias %%%d";
                        else
                            format = "OpImageSampleImplicitLod %%%d %%%d %%%d None";

            std::vector<uint32_t> newArgs = { returnType, sampledImage };
            
            for (auto arg : newArgs)
            {
                size_t offset = format.find("%%d");
                format.replace(offset, 3, Format("%d", arg));
            }
            for (size_t i = 2; i < args.size(); i++)
            {
                SPIRVResult loaded = LoadValueSPIRV(c, g, args[i]);
                newArgs.push_back(loaded.name);
                size_t offset = format.find("%%d");
                format.replace(offset, 3, Format("%d", loaded.name));
            }

            ret = g->AddMappedOp(format);
            return SPIRVResult(ret, returnType, true);
        };
    }

#define MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS(op, operands)\
      { Intrinsics::SampledTextureSample##op##_texture1D, SampleOperands::##operands }\
    , { Intrinsics::SampledTextureSample##op##_texture2D, SampleOperands::##operands }\
    , { Intrinsics::SampledTextureSample##op##_texture2DArray, SampleOperands::##operands }\
    , { Intrinsics::SampledTextureSample##op##_texture2DMS, SampleOperands::##operands }\
    , { Intrinsics::SampledTextureSample##op##_texture2DMSArray, SampleOperands::##operands }\
    , { Intrinsics::SampledTextureSample##op##_texture3D, SampleOperands::##operands }\
    , { Intrinsics::SampledTextureSample##op##_textureCube, SampleOperands::##operands }\
    , { Intrinsics::SampledTextureSample##op##_textureCubeArray, SampleOperands::##operands }

#define MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS_NO_CUBE(op, operands)\
      { Intrinsics::SampledTextureSample##op##_texture1D, SampleOperands::##operands }\
    , { Intrinsics::SampledTextureSample##op##_texture2D, SampleOperands::##operands }\
    , { Intrinsics::SampledTextureSample##op##_texture2DArray, SampleOperands::##operands }\
    , { Intrinsics::SampledTextureSample##op##_texture2DMS, SampleOperands::##operands }\
    , { Intrinsics::SampledTextureSample##op##_texture2DMSArray, SampleOperands::##operands }\
    , { Intrinsics::SampledTextureSample##op##_texture3D, SampleOperands::##operands }\

#define MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS_NO_MS(op, operands)\
      { Intrinsics::SampledTextureSample##op##_texture1D, SampleOperands::##operands }\
    , { Intrinsics::SampledTextureSample##op##_texture2D, SampleOperands::##operands }\
    , { Intrinsics::SampledTextureSample##op##_texture2DArray, SampleOperands::##operands }\
    , { Intrinsics::SampledTextureSample##op##_texture3D, SampleOperands::##operands }\
    , { Intrinsics::SampledTextureSample##op##_textureCube, SampleOperands::##operands }\
    , { Intrinsics::SampledTextureSample##op##_textureCubeArray, SampleOperands::##operands }

#define MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS_NO_ARRAY_NO_MS(op, operands)\
      { Intrinsics::SampledTextureSample##op##_texture1D, SampleOperands::##operands }\
    , { Intrinsics::SampledTextureSample##op##_texture2D, SampleOperands::##operands }\
    , { Intrinsics::SampledTextureSample##op##_texture3D, SampleOperands::##operands }\
    , { Intrinsics::SampledTextureSample##op##_textureCube, SampleOperands::##operands }\

#define MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS_NO_CUBE_NO_MS(op, operands)\
      { Intrinsics::SampledTextureSample##op##_texture1D, SampleOperands::##operands }\
    , { Intrinsics::SampledTextureSample##op##_texture2D, SampleOperands::##operands }\
    , { Intrinsics::SampledTextureSample##op##_texture2DArray, SampleOperands::##operands }\
    , { Intrinsics::SampledTextureSample##op##_texture3D, SampleOperands::##operands }\

    std::vector<std::tuple<Function*, uint32_t>> sampledTextureSampleInstructions =
    {
        MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS(Base, None),
        MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS(Lod, Lod),
        MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS(LodCompare, LodComp),
        MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS_NO_CUBE(LodOffset, LodOffset),
        MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS_NO_ARRAY_NO_MS(LodProj, LodProj),
        MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS_NO_ARRAY_NO_MS(LodProjCompare, LodProjComp),
        MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS_NO_ARRAY_NO_MS(LodProjCompareOffset, LodProjCompOffset),
        MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS_NO_ARRAY_NO_MS(LodProjOffset, LodProjOffset),
        MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS_NO_MS(Bias, Bias),
        MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS_NO_MS(BiasCompare, BiasComp),
        MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS_NO_MS(BiasOffset, BiasOffset),
        MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS_NO_ARRAY_NO_MS(BiasProj, BiasProj),
        MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS_NO_ARRAY_NO_MS(BiasProjCompare, BiasProjComp),
        MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS_NO_ARRAY_NO_MS(BiasProjOffset, BiasProjOffset),
        MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS(Compare, Comp),
        MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS_NO_CUBE(CompareOffset, CompOffset),
        MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS_NO_MS(Grad, Grad),
        MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS_NO_CUBE_NO_MS(GradCompare, GradComp),
        MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS_NO_CUBE_NO_MS(GradOffset, GradOffset),
        MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS_NO_ARRAY_NO_MS(GradProj, GradProj),
        MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS_NO_ARRAY_NO_MS(GradProjCompare, GradProjComp),
        MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS_NO_ARRAY_NO_MS(GradProjCompareOffset, GradProjCompOffset),
        MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS_NO_ARRAY_NO_MS(GradProjOffset, GradProjOffset),
        MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS_NO_ARRAY_NO_MS(Proj, Proj),
        MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS_NO_ARRAY_NO_MS(ProjCompare, ProjComp),
        MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS_NO_ARRAY_NO_MS(ProjCompareOffset, ProjCompOffset),
        MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS_NO_ARRAY_NO_MS(ProjOffset, ProjOffset)
    };

    for (auto fun : sampledTextureSampleInstructions)
    {
        this->intrinsicMap[std::get<0>(fun)] = [operands = std::get<1>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            uint32_t ret = 0xFFFFFFFF;
            assert(args[0].parentTypes.size() > 0);
            uint32_t sampledImage = g->AddMappedOp(Format("OpLoad %%%d %%%d", args[0].parentTypes[0], args[0].name));

            std::string format = "";
            if (operands & Lod || operands & Grad)
                if (operands & Comp)
                    if (operands & Grad)
                        if (operands & Proj)
                            if (operands & Bias)
                                format = "OpImageSampleProjDrefExplicitLod %%%d %%%d %%%d %%%d Grad %%%d Bias %%%d";
                            else
                                format = "OpImageSampleProjDrefExplicitLod %%%d %%%d %%%d %%%d Grad %%%d";
                        else
                            if (operands & Bias)
                                format = "OpImageSampleDrefExplicitLod %%%d %%%d %%%d %%%d Grad %%%d Bias %%%d";
                            else
                                format = "OpImageSampleDrefExplicitLod %%%d %%%d %%%d %%%d Grad %%%d";
                    else
                        if (operands & Proj)
                            if (operands & Bias)
                                format = "OpImageSampleProjDrefExplicitLod %%%d %%%d %%%d %%%d Lod %%%d Bias";
                            else
                                format = "OpImageSampleProjDrefExplicitLod %%%d %%%d %%%d %%%d Lod %%%d";
                        else
                            if (operands & Bias)
                                format = "OpImageSampleDrefExplicitLod %%%d %%%d %%%d %%%d Lod %%%d Bias %%%d";
                            else
                                format = "OpImageSampleDrefExplicitLod %%%d %%%d %%%d %%%d Lod %%%d";
                else
                    if (operands & Grad)
                        if (operands & Proj)
                            if (operands & Bias)
                                format = "OpImageSampleProjExplicitLod %%%d %%%d %%%d Grad %%%d Bias %%%d";
                            else
                                format = "OpImageSampleProjExplicitLod %%%d %%%d %%%d Grad %%%d";
                        else
                            if (operands & Bias)
                                format = "OpImageSampleExplicitLod %%%d %%%d %%%d Grad %%%d Bias %%%d";
                            else
                                format = "OpImageSampleExplicitLod %%%d %%%d %%%d Grad %%%d";
                    else
                        if (operands & Proj)
                            if (operands & Bias)
                                format = "OpImageSampleProjExplicitLod %%%d %%%d %%%d Lod %%%d Bias %%%d";
                            else
                                format = "OpImageSampleProjExplicitLod %%%d %%%d %%%d Lod %%%d";
                        else
                            if (operands & Bias)
                                format = "OpImageSampleExplicitLod %%%d %%%d %%%d Lod %%%d Bias %%%d";
                            else
                                format = "OpImageSampleExplicitLod %%%d %%%d %%%d Lod %%%d";
            else
                if (operands & Comp)
                    if (operands & Proj)
                        if (operands & Bias)
                            format = "OpImageSampleProjDrefImplicitLod %%%d %%%d %%%d %%%d Bias %%%d";
                        else
                            format = "OpImageSampleProjDrefImplicitLod %%%d %%%d %%%d %%%d None";
                    else
                        if (operands & Bias)
                            format = "OpImageSampleDrefImplicitLod %%%d %%%d %%%d %%%d Bias";
                        else
                            format = "OpImageSampleDrefImplicitLod %%%d %%%d %%%d %%%d None";
                else
                    if (operands & Proj)
                        if (operands & Bias)
                            format = "OpImageSampleProjImplicitLod %%%d %%%d %%%d Bias %%%d";
                        else
                            format = "OpImageSampleProjImplicitLod %%%d %%%d %%%d None";
                    else
                        if (operands & Bias)
                            format = "OpImageSampleImplicitLod %%%d %%%d %%%d Bias %%%d";
                        else
                            format = "OpImageSampleImplicitLod %%%d %%%d %%%d None";

            std::vector<uint32_t> newArgs = { returnType, sampledImage };

            for (auto arg : newArgs)
            {
                size_t offset = format.find("%%d");
                format.replace(offset, 3, Format("%d", arg));
            }
            for (size_t i = 1; i < args.size(); i++)
            {
                SPIRVResult loaded = LoadValueSPIRV(c, g, args[i]);
                newArgs.push_back(loaded.name);
                size_t offset = format.find("%%d");
                format.replace(offset, 3, Format("%d", loaded.name));
            }

            ret = g->AddMappedOp(format);
            return SPIRVResult(ret, returnType, true);
        };
    }

#define MAKE_TEXTURE_LOAD_STORE_INTRINSICS(op, operands, store)\
      { Intrinsics::Texture##op##_texture1D, store, SampleOperands::##operands }\
    , { Intrinsics::Texture##op##_texture2D, store, SampleOperands::##operands }\
    , { Intrinsics::Texture##op##_texture2DArray, store, SampleOperands::##operands }\
    , { Intrinsics::Texture##op##_texture2DMS, store, SampleOperands::##operands }\
    , { Intrinsics::Texture##op##_texture2DMSArray, store, SampleOperands::##operands }\
    , { Intrinsics::Texture##op##_texture3D, store, SampleOperands::##operands }\
    , { Intrinsics::Texture##op##_textureCube, store, SampleOperands::##operands }\
    , { Intrinsics::Texture##op##_textureCubeArray, store, SampleOperands::##operands }

    std::vector<std::tuple<Function*, bool, uint32_t>> textureStoreLoadInstructions =
    {
        MAKE_TEXTURE_LOAD_STORE_INTRINSICS(StoreBase, None, true),
        MAKE_TEXTURE_LOAD_STORE_INTRINSICS(StoreMip, Lod, true),
        MAKE_TEXTURE_LOAD_STORE_INTRINSICS(LoadBase, None, false),
        MAKE_TEXTURE_LOAD_STORE_INTRINSICS(LoadMip, Lod, false),
    };

    for (auto fun : textureStoreLoadInstructions)
    {
        this->intrinsicMap[std::get<0>(fun)] = [store = std::get<1>(fun), operands = std::get<2>(fun)](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            uint32_t ret = 0xFFFFFFFF;
            SPIRVResult res = SPIRVResult::Invalid();
            if (store)
            {
                if (operands & Lod)
                {
                    assert(args.size() == 4);
                    assert(args[0].parentTypes.size() > 0);
                    uint32_t dereffed;
                    if (c->target.supportsPhysicalAddressing)
                    {
                        dereffed = g->AddMappedOp(Format("OpLoad %%%d %%%d", args[0].typeName, args[0].name));
                        dereffed = g->AddMappedOp(Format("OpLoad %%%d %%%d", args[0].parentTypes[0], dereffed));
                    }
                    else
                    {
                        dereffed = g->AddMappedOp(Format("OpLoad %%%d %%%d", args[0].parentTypes[0], args[0].name));
                    }
                    SPIRVResult loaded = LoadValueSPIRV(c, g, args[2]);
                    g->AddOp(Format("OpImageWrite %%%d %%%d %%%d Lod %%%d", dereffed, args[1].name, loaded.name, args[3].name));
                }
                else
                {
                    assert(args.size() == 3);
                    assert(args[0].parentTypes.size() > 0);
                    uint32_t dereffed;
                    if (c->target.supportsPhysicalAddressing)
                    {
                        dereffed = g->AddMappedOp(Format("OpLoad %%%d %%%d", args[0].typeName, args[0].name));
                        dereffed = g->AddMappedOp(Format("OpLoad %%%d %%%d", args[0].parentTypes[0], dereffed));
                    }
                    else
                    {
                        dereffed = g->AddMappedOp(Format("OpLoad %%%d %%%d", args[0].parentTypes[0], args[0].name));
                    }
                    SPIRVResult loaded = LoadValueSPIRV(c, g, args[2]);
                    g->AddOp(Format("OpImageWrite %%%d %%%d %%%d", dereffed, args[1].name, loaded.name));
                }
            }
            else
            {
                if (operands & Lod)
                {
                    assert(args.size() == 3);
                    assert(args[0].parentTypes.size() > 0);
                    uint32_t dereffed;
                    if (c->target.supportsPhysicalAddressing)
                    {
                        dereffed = g->AddMappedOp(Format("OpLoad %%%d %%%d", args[0].typeName, args[0].name));
                        dereffed = g->AddMappedOp(Format("OpLoad %%%d %%%d", args[0].parentTypes[0], dereffed));
                    }
                    else
                    {
                        dereffed = g->AddMappedOp(Format("OpLoad %%%d %%%d", args[0].parentTypes[0], args[0].name));
                    }
                    ret = g->AddMappedOp(Format("OpImageRead %%%d %%%d %%%d Lod %%%d", returnType, dereffed, args[1].name, args[2].name));
                    res.isValue = true;
                }
                else
                {
                    assert(args.size() == 2);
                    assert(args[0].parentTypes.size() > 0);
                    uint32_t dereffed;
                    if (c->target.supportsPhysicalAddressing)
                    {
                        dereffed = g->AddMappedOp(Format("OpLoad %%%d %%%d", args[0].typeName, args[0].name));
                        dereffed = g->AddMappedOp(Format("OpLoad %%%d %%%d", args[0].parentTypes[0], dereffed));
                    }
                    else
                    {
                        dereffed = g->AddMappedOp(Format("OpLoad %%%d %%%d", args[0].parentTypes[0], args[0].name));
                    }
                    ret = g->AddMappedOp(Format("OpImageRead %%%d %%%d %%%d", returnType, dereffed, args[1].name));
                    res.isValue = true;
                }
            }
            res.name = ret;
            res.typeName = returnType;
            return res;
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
    if (funcResolved->isEntryPoint && generator->entryPoint != func)
        return;

    if (funcResolved->isPrototype)
        return;
    SPIRVResult returnName = GenerateTypeSPIRV(compiler, generator, func->returnType, static_cast<Type*>(funcResolved->returnTypeSymbol));

    std::string typeArgs = "";

    for (auto& param : func->parameters)
    {
        Variable::__Resolved* paramResolved = Symbol::Resolved(param);
        SPIRVResult typeName = GenerateTypeSPIRV(compiler, generator, paramResolved->type, paramResolved->typeSymbol);

        if (funcResolved->isEntryPoint)
        {
            GenerateVariableSPIRV(compiler, generator, param, true, true);
        }
        else
        {
            typeArgs.append(Format("%%%d ", typeName.typeName));
        }
    }
    
    uint32_t functionType = generator->AddSymbol(Format("function(%s) %s", typeArgs.c_str(), func->returnType.ToString().c_str()), Format("OpTypeFunction %%%d %s", returnName.typeName, typeArgs.c_str()), true);

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
    auto functionOverride = generator->evaluatingProgram->functionOverrides.find(func);
    if (functionOverride != generator->evaluatingProgram->functionOverrides.end())
        GenerateStatementSPIRV(compiler, generator, functionOverride->second->ast);
    else
        GenerateStatementSPIRV(compiler, generator, func->ast);
    generator->functions.append(generator->variableDeclarations);
    generator->variableDeclarations.clear();
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
uint32_t
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

            // If this struct is generated for binding 
            generator->AddMemberDecoration(structName, i, Format("Offset %d", varResolved->structureOffset));
            if (varResolved->typeSymbol->IsMatrix())
            {
                generator->AddMemberDecoration(structName, i, Format("MatrixStride %d", varResolved->typeSymbol->CalculateStride()));
                generator->AddMemberDecoration(structName, i, "ColMajor");
            }
            offset += varResolved->typeSymbol->CalculateSize();
        }
    }
    generator->AddReservedSymbol(struc->name, structName, Format("OpTypeStruct %s", memberTypes.c_str()), true);
    return structName;
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateEnumSPIRV(Compiler* compiler, SPIRVGenerator* generator, Symbol* symbol)
{
    Enumeration* enumeration = static_cast<Enumeration*>(symbol);
    Enumeration::__Resolved* enumResolved = Symbol::Resolved(enumeration);

    // Implement 'conversion' functions, which is to just return the argument
    for (size_t i = 0; i < enumeration->globals.size(); i++)
    {
        Symbol* sym = enumeration->globals[i];
        if (sym->symbolType == Symbol::FunctionType)
        {
            Function* fun = static_cast<Function*>(sym);
            
                generator->intrinsicMap[fun] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
                {
                    return args[0];
                };
            
        }
    }
    for (size_t i = 0; i < enumeration->staticSymbols.size(); i++)
    {
        Symbol* sym = enumeration->staticSymbols[i];
        if (sym->symbolType == Symbol::FunctionType)
        {
            Function* fun = static_cast<Function*>(sym);
            if (fun->name == "operator==")
            {
                generator->intrinsicMap[fun] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
                    {
                        return SPIRVResult(g->AddMappedOp(Format("OpIEqual %%%d %%%d %%%d", returnType, args[0].name, args[1].name)), returnType, true, true);
                    };
            }
            else if (fun->name == "operator!=")
            {
                generator->intrinsicMap[fun] = [](Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
                    {
                        return SPIRVResult(g->AddMappedOp(Format("OpINotEqual %%%d %%%d %%%d", returnType, args[0].name, args[1].name)), returnType, true, true);
                    };
            }
        }
    }
    SPIRVResult typeName = GenerateTypeSPIRV(compiler, generator, enumeration->type, enumResolved->typeSymbol);
    return typeName;
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateVariableSPIRV(Compiler* compiler, SPIRVGenerator* generator, Symbol* symbol, bool isShaderArgument, bool isGlobal)
{
    Variable* var = static_cast<Variable*>(symbol);
    Variable::__Resolved* varResolved = static_cast<Variable::__Resolved*>(var->resolved);
    
    SPIRVResult typeName = GenerateTypeSPIRV(compiler, generator, varResolved->type, varResolved->typeSymbol, varResolved->imageFormat, varResolved->accessBits, varResolved->usageBits, varResolved->storage);
    std::string type = varResolved->type.name;
    std::string scope = SPIRVResult::ScopeToString(typeName.scope);

    std::vector<uint32_t> parentTypes = { typeName.typeName };
    uint32_t name = 0xFFFFFFFF;

    SPIRVResult initializer = SPIRVResult::Invalid();

    Expression* initializerExpression = var->valueExpression;

    if (initializerExpression != nullptr)
    {
        // Setup initializer
        generator->linkDefineEvaluation = varResolved->storage == Variable::__Resolved::Storage::LinkDefined;
        generator->linkDefineSlot = varResolved->binding;
        initializer = GenerateExpressionSPIRV(compiler, generator, initializerExpression);


        // If initializer is a literal, make sure to load it
        if (initializer.isLiteral)
            initializer = LoadValueSPIRV(compiler, generator, initializer);
        generator->linkDefineEvaluation = false;
        generator->linkDefineSlot = UINT32_MAX;
    }

    if (varResolved->storage != Variable::__Resolved::Storage::LinkDefined)
    {
        uint32_t typePtrName = typeName.typeName;
        // If anything but void, then the type has to be a pointer
        if (!varResolved->type.IsPointer())
        {
            type = Format("ptr(%d)", typeName.typeName);
            typePtrName = generator->AddSymbol(Format("%s%s", type.c_str(), scope.c_str()), Format("OpTypePointer %s %%%d", scope.c_str(), typeName.typeName), true);
        }
        else
        {
            // We implicitly know a pointer was created
            typeName.typeName = typeName.parentTypes[0];
        }
        
        if (initializer != SPIRVResult::Invalid() && initializer.isConst)
            name = generator->AddVariableDeclaration(symbol, varResolved->name, typePtrName, initializer.name, typeName.scope, isGlobal);
        else
            name = generator->AddVariableDeclaration(symbol, varResolved->name, typePtrName, 0x0, typeName.scope, isGlobal);

        if (initializer != SPIRVResult::Invalid() && !initializer.isConst)
        {
            SPIRVResult loaded = LoadValueSPIRV(compiler, generator, initializer);
            generator->AddOp(Format("OpStore %%%d %%%d", name, loaded.name));
        }

        if (typeName.scope == SPIRVResult::Storage::StorageBuffer || typeName.scope == SPIRVResult::Storage::Uniform || typeName.scope == SPIRVResult::Storage::PushConstant || typeName.scope == SPIRVResult::Storage::Sampler)
        {
            uint32_t structSymbol = generator->GetSymbol(varResolved->typeSymbol->name).value;
            if (typeName.scope != SPIRVResult::Storage::Sampler)
                generator->AddDecoration(Format("Block(%s)", varResolved->typeSymbol->name.c_str()), structSymbol, "Block");
            generator->AddDecoration(Format("Set(%s)", varResolved->name.c_str()), name, Format("DescriptorSet %d", varResolved->group));
            generator->AddDecoration(Format("Binding(%s)", varResolved->name.c_str()), name, Format("Binding %d", varResolved->binding));
            generator->interfaceVariables.insert(name);
        }
        else if (typeName.scope == SPIRVResult::Storage::Image || typeName.scope == SPIRVResult::Storage::MutableImage)
        {
            generator->AddDecoration(Format("Set(%s)", varResolved->name.c_str()), name, Format("DescriptorSet %d", varResolved->group));
            generator->AddDecoration(Format("Binding(%s)", varResolved->name.c_str()), name, Format("Binding %d", varResolved->binding));
            generator->interfaceVariables.insert(name);
        }
        else if (typeName.scope == SPIRVResult::Storage::Input || typeName.scope == SPIRVResult::Storage::Output)
        {
            uint8_t binding = typeName.scope == SPIRVResult::Storage::Input ? varResolved->inBinding : varResolved->outBinding;
            generator->AddDecoration(Format("Location(%s)", varResolved->name.c_str()), name, Format("Location %d", binding));
            generator->AddDecoration(Format("Component(%s)", varResolved->name.c_str()), name, Format("Component 0"));
            if (varResolved->parameterBits.flags.isNoInterpolate)
                generator->AddDecoration(Format("NoInterpolate(%s)", varResolved->name.c_str()), name, "Flat");
            if (varResolved->parameterBits.flags.isNoPerspective)
                generator->AddDecoration(Format("NoPerspective(%s)", varResolved->name.c_str()), name, "NoPerspective");
            if (varResolved->parameterBits.flags.isPatch)
                generator->AddDecoration(Format("Patch(%s)", varResolved->name.c_str()), name, "Patch");
            if (varResolved->parameterBits.flags.isCentroid)
                generator->AddDecoration(Format("Centroid(%s)", varResolved->name.c_str()), name, "Centroid");
            generator->interfaceVariables.insert(name);
        }
        return SPIRVResult(name, typeName.typeName, false, false, typeName.scope, parentTypes);
    }
    else
    {
        // If it's a link defined variable, the constant generated by the initializer is sufficient
        generator->AddMapping(varResolved->name, initializer.name);
        generator->AddDecoration(Format("SpecId(%d)", varResolved->binding), initializer.name, Format("SpecId %d", varResolved->binding));
        auto res = SPIRVResult(initializer.name, typeName.typeName);
        res.isValue = true;
        res.isConst = true;
        res.isSpecialization = true;
        res.scope = typeName.scope;
        res.parentTypes = parentTypes;
        return res;
    }
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
        funName = generator->GetSymbol(resolvedFunction->name).value;

        // Create arg list from argument expressions
        std::string argList = "";
        for (size_t i = 0; i < callExpression->args.size(); i++)
        {
            if (resolvedCall->argumentTypes[i].literal)
                generator->literalExtract = true;

            SPIRVResult arg = GenerateExpressionSPIRV(compiler, generator, callExpression->args[i]);
            argList.append(Format("%%%d ", arg.name));

            generator->literalExtract = false;
        }

        // Then call the function
        return SPIRVResult(generator->AddMappedOp(Format("OpFunctionCall %%%d %%%d %s", returnTypeName.typeName, funName, argList.c_str())), returnTypeName.typeName);
    }
    else
    {
        // If there is no symbol matching this function, then we assume it's an intrinsic function
        std::vector<SPIRVResult> args;

        // Create arg list from argument expressions
        for (size_t i = 0; i < callExpression->args.size(); i++)
        {
            if (resolvedCall->argumentTypes[i].literal)
                generator->literalExtract = true;

            SPIRVResult arg = GenerateExpressionSPIRV(compiler, generator, callExpression->args[i]);
            args.push_back(arg);

            generator->literalExtract = false;
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
            SPIRVResult indexConstant = GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::UInt(index));

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
    auto initResolved = Symbol::Resolved(initExpression);
    uint32_t strucType = generator->GetSymbol(initResolved->type->name).value;

    std::vector<SPIRVResult> values;
    bool isConst = true;
    bool isLinkDefined = false;
    std::string initializer = "";
    for (Expression* expr : initExpression->values)
    {
        BinaryExpression* binExpr = static_cast<BinaryExpression*>(expr);
        SPIRVResult value = GenerateExpressionSPIRV(compiler, generator, binExpr->right);
        if (!value.isValue)
            value = LoadValueSPIRV(compiler, generator, value);
        initializer.append(Format("%%%d ", value.name));
        values.push_back(value);
        isConst &= value.isConst;
        isLinkDefined |= value.isSpecialization;
    }
    
    if (isConst)
        if (isLinkDefined)
            name = generator->AddSymbol(Format("{%s}", initializer.c_str()), Format("OpSpecConstantComposite %%%d %s", strucType, initializer.c_str()), true);
        else
            name = generator->AddSymbol(Format("{%s}", initializer.c_str()), Format("OpConstantComposite %%%d %s", strucType, initializer.c_str()), true);
    else
    {
        assert(!generator->linkDefineEvaluation);
        name = generator->AddSymbol(Format("{%s}", initializer.c_str()), Format("OpCompositeConstruct %%%d %s", strucType, initializer.c_str()));
    }
    return SPIRVResult(name, strucType, true, isConst);
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
    rightValue = LoadValueSPIRV(compiler, generator, rightValue);

    // If there is a conversion function, generate it first
    if (binaryExpressionResolved->conversionFunction)
    {
        auto it = generator->intrinsicMap.find(binaryExpressionResolved->conversionFunction);
        assert(it != generator->intrinsicMap.end());
        rightValue = it->second(compiler, generator, leftValue.typeName, { rightValue });
        rightType = binaryExpressionResolved->conversionFunction->returnType;
    }

    if (leftType == rightType && binaryExpression->op == '=')
    {
        assert(!leftValue.isValue);

        // If left value has a swizzle mask, then swizzle the RIGHT hand value before storing
        if (leftValue.swizzleMask != Type::SwizzleMask())
        {
            // First get the variable so we can get the size of the vector to store to
            std::string leftSymbol;
            binaryExpression->left->EvalSymbol(leftSymbol);
            const SymbolAssignment& sym = generator->GetSymbol(leftSymbol);
            Variable* var = static_cast<Variable*>(sym.sym);
            Variable::__Resolved* varRes = Symbol::Resolved(var);

            // Setup a binding table
            uint32_t slots[4] = { 0, 1, 2, 3 };
            uint32_t counter = 0;

            // Update the slot mask
            if (leftValue.swizzleMask.bits.x != Type::SwizzleMask::Invalid)
                slots[leftValue.swizzleMask.bits.x] = varRes->typeSymbol->columnSize + counter++;
            if (leftValue.swizzleMask.bits.y != Type::SwizzleMask::Invalid)
                slots[leftValue.swizzleMask.bits.y] = varRes->typeSymbol->columnSize + counter++;
            if (leftValue.swizzleMask.bits.z != Type::SwizzleMask::Invalid)
                slots[leftValue.swizzleMask.bits.z] = varRes->typeSymbol->columnSize + counter++;
            if (leftValue.swizzleMask.bits.w != Type::SwizzleMask::Invalid)
                slots[leftValue.swizzleMask.bits.w] = varRes->typeSymbol->columnSize + counter++;

            std::string swizzleMask = "";
            for (uint32_t i = 0; i < varRes->typeSymbol->columnSize; i++)
            {
                swizzleMask.append(Format("%d ", slots[i]));
            }

            // Make sure to reset the swizzle mask so we don't perform a swizzle load on the left
            leftValue.swizzleMask = Type::SwizzleMask();
            SPIRVResult leftLoaded = LoadValueSPIRV(compiler, generator, leftValue);
            rightValue.name = generator->AddMappedOp(Format("OpVectorShuffle %%%d %%%d %%%d %s", leftValue.typeName, leftLoaded.name, rightValue.name, swizzleMask.c_str()));
        }
        generator->AddOp(Format("OpStore %%%d %%%d", leftValue.name, rightValue.name));

        return leftValue;
    }
    else
    {

        std::string functionName = Format("operator%s(%s)", FourCCToString(binaryExpression->op).c_str(), rightType.name.c_str());
        Function* fun = binaryExpressionResolved->lhsType->GetSymbol<Function>(functionName);
        Function::__Resolved* funResolved = Symbol::Resolved(fun);
        assert(fun != nullptr);

        SPIRVResult retType = GenerateTypeSPIRV(compiler, generator, fun->returnType, funResolved->returnTypeSymbol);
        leftValue = LoadValueSPIRV(compiler, generator, leftValue);

        // Get operator 
        auto op = generator->intrinsicMap.find(fun);
        assert(op != generator->intrinsicMap.end());
        return op->second(compiler, generator, retType.typeName, { leftValue, rightValue });
    }
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
            SPIRVResult indexName = GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::UInt(swizzle.bits.x));
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

            SPIRVResult ret = SPIRVResult(lhs.name, origType.typeName);
            ret.swizzleMask = swizzle;
            ret.swizzleType = retType.typeName;
            return ret;
        }
        // Generate swizzle access
    }
    else
    {
        // Otherwise, find offset of member
        Type::FullType lhsType;
        accessExpression->left->EvalType(lhsType);
        Type* rhsSymbol = compiler->GetSymbol<Type>(lhsType.name);

        if (rhsSymbol->symbolType == Symbol::StructureType || rhsSymbol->symbolType == Symbol::EnumerationType)
        {
            for (size_t i = 0; i < rhsSymbol->symbols.size(); i++)
            {
                Symbol* sym = rhsSymbol->symbols[i];
                if (sym->name == accessExpressionResolved->rightSymbol)
                {
                    if (rhsSymbol->symbolType == Symbol::StructureType)
                    {
                        SPIRVResult indexName = GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::UInt(i));
                        SPIRVResult typeName = GenerateTypeSPIRV(compiler, generator, accessExpressionResolved->rightType, accessExpressionResolved->rhsType);
                        uint32_t ptrTypeName = generator->AddSymbol(Format("ptr(%s)%s", accessExpressionResolved->rightType.name.c_str(), scopeName.c_str()), Format("OpTypePointer %s %%%d", scopeName.c_str(), typeName.typeName), true);
                        return SPIRVResult(generator->AddMappedOp(Format("OpAccessChain %%%d %%%d %%%d", ptrTypeName, lhs.name, indexName.name), accessExpressionResolved->text), typeName.typeName);
                    }
                    else if (rhsSymbol->symbolType == Symbol::EnumerationType)
                    {
                        EnumExpression* enumExp = static_cast<EnumExpression*>(sym);
                        uint32_t val;
                        enumExp->EvalUInt(val);
                        if (generator->literalExtract)
                            return SPIRVResult(val);
                        else
                            return GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::UInt(val));
                    }
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
    
    bool b;
    if (ternaryExpression->lhs->EvalBool(b))
    {
        if (b)
            return GenerateExpressionSPIRV(compiler, generator, ternaryExpression->ifExpression);
        else
            return GenerateExpressionSPIRV(compiler, generator, ternaryExpression->elseExpression);
    }

    SPIRVResult lhsResult = GenerateExpressionSPIRV(compiler, generator, ternaryExpression->lhs);

    SPIRVResult ifResult = GenerateExpressionSPIRV(compiler, generator, ternaryExpression->ifExpression);
    SPIRVResult elseResult = GenerateExpressionSPIRV(compiler, generator, ternaryExpression->elseExpression);
    uint32_t ret = generator->AddMappedOp(Format("OpSelect %%%d %%%d %%%d %%%d", ifResult.typeName, lhsResult.name, ifResult.name, elseResult.name), ternaryExpressionResolved->text);

    return SPIRVResult(ret, ifResult.typeName, ifResult.isValue, ifResult.isConst);
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
                    constOne = GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::Int(1));
                }
                else
                {
                    constOne = GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::Float(1));
                }
            }
            else
            {
                constOne = GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::UInt(1));
            }

            SPIRVResult loaded = LoadValueSPIRV(compiler, generator, rhs);
            uint32_t res = generator->AddMappedOp(Format("Op%cAdd %%%d %%%d %%%d", op, rhs.typeName, loaded.name, constOne.name), unaryExpressionResolved->text);
            generator->AddOp(Format("OpStore %%%d %%%d", rhs.name, res));
            if (unaryExpression->isPrefix)
                return SPIRVResult(res, rhs.typeName, true);
            else
                return SPIRVResult(loaded.name, rhs.typeName, true);
        }
        case '--':
        {
            SPIRVResult constOne = SPIRVResult::Invalid();
            if (isSigned)
            {
                if (op == 'I')
                {
                    constOne = GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::Int(1));
                }
                else
                {
                    constOne = GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::Float(1));
                }
            }
            else
            {
                constOne = GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::UInt(1));
            }

            SPIRVResult loaded = LoadValueSPIRV(compiler, generator, rhs);
            uint32_t res = generator->AddMappedOp(Format("Op%cSub %%%d %%%d %%%d", op, rhs.typeName, loaded.name, constOne.name), unaryExpressionResolved->text);
            generator->AddOp(Format("OpStore %%%d %%%d", rhs.name, res));
            if (unaryExpression->isPrefix)
                return SPIRVResult(res, rhs.typeName, true);
            else
                return SPIRVResult(loaded.name, rhs.typeName, true);
            break;
        }
        case '*':
        {
            Type::FullType newType = unaryExpressionResolved->fullType;
            assert(newType.modifiers.size() > 0);
            assert(newType.modifiers.back() == Type::FullType::Modifier::Pointer);
            newType.modifiers.pop_back();
            Type* type = compiler->GetSymbol<Type>(newType.name);
            SPIRVResult typeName = GenerateTypeSPIRV(compiler, generator, newType, type);
            uint32_t deref = generator->AddMappedOp(Format("OpLoad %%%d %%%d", typeName.typeName, rhs.name), unaryExpressionResolved->text);
            return SPIRVResult(deref, typeName.typeName, newType.modifiers.empty());
            break;
        }
        case '-':
        {
            SPIRVResult loaded = LoadValueSPIRV(compiler, generator, rhs);
            uint32_t res = generator->AddMappedOp(Format("Op%cNegate %%%d", op, loaded.name), unaryExpressionResolved->text);
            generator->AddOp(Format("OpStore %%%d %%%d", rhs.name, res));
            break;
        }
        case '+':
        {
            SPIRVResult loaded = LoadValueSPIRV(compiler, generator, rhs);
            return SPIRVResult(loaded.name, rhs.typeName, true);
        }
        case '!':
        case '~':
        {
            SPIRVResult loaded = LoadValueSPIRV(compiler, generator, rhs);
            uint32_t res = generator->AddMappedOp(Format("OpNot %%%d", loaded.name));
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
            if (generator->literalExtract)
                return SPIRVResult(value);
            else
                return GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::Float(value));
        }
        case Symbol::IntExpressionType:
        {
            int value;
            IntExpression* intExpr = static_cast<IntExpression*>(expr);
            intExpr->EvalInt(value);
            if (generator->literalExtract)
                return SPIRVResult(value);
            else
                return GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::Int(value));
        }
        case Symbol::UIntExpressionType:
        {
            uint32_t value;
            UIntExpression* uintExpr = static_cast<UIntExpression*>(expr);
            uintExpr->EvalUInt(value);
            if (generator->literalExtract)
                return SPIRVResult(value);
            else
                return GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::UInt(value));
        }
        case Symbol::BoolExpressionType:
        {
            bool value;
            BoolExpression* boolExpr = static_cast<BoolExpression*>(expr);
            boolExpr->EvalBool(value);
            if (generator->literalExtract)
                return SPIRVResult(value);
            else
                return GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::Bool(value));
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
                SPIRVResult res = SPIRVResult::Invalid();
                if (!varResolved->builtin)
                {
                    bool isLinkDefined = varResolved->storage == Variable::__Resolved::Storage::LinkDefined;
                    type = GenerateTypeSPIRV(compiler, generator, symResolved->fullType, symResolved->type, varResolved->imageFormat, varResolved->accessBits, varResolved->usageBits, varResolved->storage);
                    res = SPIRVResult(generator->GetSymbol(symbolExpression->symbol).value, type.typeName);
                    res.isValue = isLinkDefined;
                    res.isConst = isLinkDefined;
                    res.isSpecialization = isLinkDefined;
                    res.scope = type.scope;
                    res.parentTypes = type.parentTypes;    
                }
                else
                {
                    res = GenerateVariableSPIRV(compiler, generator, var, false, true);
                }
                
                return res;
            }
            else if (symResolved->symbol->symbolType == Symbol::SymbolType::EnumerationType)
            {
                Enumeration* enu = static_cast<Enumeration*>(symResolved->symbol);
                Enumeration::__Resolved* enuResolved = Symbol::Resolved(enu);
                type = GenerateTypeSPIRV(compiler, generator, enu->type, enuResolved->typeSymbol);
                return SPIRVResult(0xFFFFFFFF, type.typeName, false, false, type.scope, type.parentTypes);
            }
            else
            {
                type = GenerateTypeSPIRV(compiler, generator, symResolved->fullType, symResolved->type);
                return SPIRVResult(generator->GetSymbol(symbolExpression->symbol).value, type.typeName, false, false, type.scope, type.parentTypes);
            }
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
    if (generator->skipBreakContinue)
        return;
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
    if (generator->skipBreakContinue)
        return;
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
    // Skip for loop generation if condition resolves to false
    bool b;
    if (stat->condition->EvalBool(b))
    {
        if (!b)
            return;
    }
    
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

    std::string unroll = "None";
    if (stat->unrollCount == UINT_MAX)
        unroll = "Unroll";
    else if (stat->unrollCount > 0)
        unroll = Format("PartialCount %d", stat->unrollCount);

    // All loops must begin with a loop merge
    generator->AddOp(Format("OpLoopMerge %%%d %%%d %s", endLabel, repeatLabel, unroll.c_str()));
    generator->AddOp(Format("OpBranch %%%d", conditionLabel));

    // This block is for the condition testing
    generator->AddReserved("OpLabel", conditionLabel, "for condition");
    generator->blockOpen = true;
    SPIRVResult cond = GenerateExpressionSPIRV(compiler, generator, stat->condition);
    cond = LoadValueSPIRV(compiler, generator, cond);

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
    // If B evaluates to a constant value, pick a branch and return early
    bool b;
    if (stat->condition->EvalBool(b))
    {
        if (b)
            GenerateStatementSPIRV(compiler, generator, stat->ifStatement);
        else
            GenerateStatementSPIRV(compiler, generator, stat->elseStatement);
        return;
    }
    
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
        res = LoadValueSPIRV(compiler, generator, res);
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
    uint32_t index;
    if (stat->switchExpression->EvalUInt(index))
    {
        generator->skipBreakContinue = true;
        if (index < stat->caseExpressions.size())
            GenerateStatementSPIRV(compiler, generator, stat->caseStatements[index]);
        else if (stat->defaultStatement != nullptr)
            GenerateStatementSPIRV(compiler, generator, stat->defaultStatement);
        generator->skipBreakContinue = false;
        return;
    }
    
    SPIRVResult switchRes = GenerateExpressionSPIRV(compiler, generator, stat->switchExpression);
    switchRes = LoadValueSPIRV(compiler, generator, switchRes);

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
    // Skip generation if statement resolves to fales
    bool b;
    if (stat->condition->EvalBool(b))
    {
        if (!b)
            return;
    }
    
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
        cond = LoadValueSPIRV(compiler, generator, cond);

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
        cond = LoadValueSPIRV(compiler, generator, cond);

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
        case Symbol::DiscardStatementType:
            generator->AddOp("OpKill");
            generator->blockOpen = false;
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
                    ret = GenerateStatementSPIRV(compiler, generator, stat);
                    if (ret)
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
            gen->functions.clear();
            gen->capabilities.clear();
            gen->extensions.clear();
            gen->decorationMap.clear();
            gen->interfaceVariables.clear();
            gen->mergeBlockCounter = 0;
            gen->symbolCounter = 0;
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

    this->evaluatingProgram = progResolved;
    for (uint32_t mapping = Program::__Resolved::ProgramEntryType::FirstShader; mapping < Program::__Resolved::ProgramEntryType::LastShader; mapping++)
    {
        Cleanup cleanup(this);

        Symbol* object = progResolved->mappings[mapping];
        if (object == nullptr)
            continue;

        compiler->shaderValueExpressions[mapping].value = true;

        // Main scope
        this->PushScope();

        // Temporarily store original variable values
        std::unordered_map<Variable*, Expression*> originalVariableValues;
        auto it = progResolved->constVarInitializationOverrides.begin();
        for (; it != progResolved->constVarInitializationOverrides.end(); it++)
        {
            originalVariableValues[it->first] = it->first->valueExpression;
            it->first->valueExpression = it->second;
        }
        
        this->header.append("; Magic:     0x00010500 (SPIRV Universal 1.5)\n");
        this->header.append("; Version:   0x00010000 (Version: 1.0.0)\n");
        this->header.append("; Generator: 0x00080001 (GPULang; 1)\n");
        this->AddCapability(extensionMap[(Program::__Resolved::ProgramEntryType)mapping]);

        this->entryPoint = static_cast<Function*>(object);
        
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
                case Symbol::EnumerationType:
                    GenerateEnumSPIRV(compiler, this, sym);
                    break;
                case Symbol::VariableType:
                    GenerateVariableSPIRV(compiler, this, sym, false, true);
                    break;
            }
        }

        Function::__Resolved* funResolved = Symbol::Resolved(static_cast<Function*>(object));
        uint32_t entryFunction = this->GetSymbol(funResolved->name).value;

        if (funResolved->executionModifiers.groupSize != 64 || funResolved->executionModifiers.groupsPerWorkgroup != 1)
            this->AddCapability("SubgroupDispatch");

        if (compiler->target.supportsPhysicalAddressing)
        {
            this->AddCapability("Addresses");
            this->header.append("\tOpMemoryModel Physical64 GLSL450\n");
        }
        else
        {
            this->AddCapability("PhysicalStorageBufferAddresses");
            this->header.append("\tOpMemoryModel PhysicalStorageBuffer64 GLSL450\n");
        }

        std::string interfaces = "";
        for (const uint32_t inter : this->interfaceVariables)
        {
            interfaces.append(Format("%%%d ", inter));
        }
        this->header.append(Format("\tOpEntryPoint %s %%%d \"main\" %s\n", executionModelMap[(Program::__Resolved::ProgramEntryType)mapping].c_str(), entryFunction, interfaces.c_str()));
        
        switch (mapping)
        {
            case Program::__Resolved::GeometryShader:
                this->header.append(Format("\tOpExecutionMode %%%d Invocations %d\n", entryFunction, funResolved->executionModifiers.invocations));
                this->header.append(Format("\tOpExecutionMode %%%d %s\n", entryFunction, inputPrimitiveTopologyMap[funResolved->executionModifiers.inputPrimitiveTopology].c_str()));
                this->header.append(Format("\tOpExecutionMode %%%d %s\n", entryFunction, outputPrimitiveTopologyMap[funResolved->executionModifiers.outputPrimitiveTopology].c_str()));
                this->header.append(Format("\tOpExecutionMode %%%d OutputVertices %d\n", entryFunction, funResolved->executionModifiers.maxOutputVertices));
                break;
            case Program::__Resolved::HullShader:
                this->header.append(Format("\tOpExecutionMode %%%d %s\n", entryFunction, partitionMap[funResolved->executionModifiers.partitionMethod].c_str()));
                this->header.append(Format("\tOpExecutionMode %%%d %s\n", entryFunction, windingOrderMap[funResolved->executionModifiers.windingOrder].c_str()));
                this->header.append(Format("\tOpExecutionMode %%%d %s\n", entryFunction, inputPrimitiveTopologyMap[funResolved->executionModifiers.inputPrimitiveTopology].c_str()));
                this->header.append(Format("\tOpExecutionMode %%%d OutputVertices %d\n", entryFunction, funResolved->executionModifiers.maxOutputVertices));
                break;
            case Program::__Resolved::DomainShader:
                break;
            case Program::__Resolved::PixelShader:
                this->header.append(Format("\tOpExecutionMode %%%d %s\n", entryFunction, pixelOriginMap[funResolved->executionModifiers.pixelOrigin].c_str()));
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
        compiler->shaderValueExpressions[mapping].value = false;
        
        // Reset variable values
        auto it2 = originalVariableValues.begin();
        for (; it2 != originalVariableValues.end(); it2++)
        {
            originalVariableValues[it2->first] = it2->second;
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
        progResolved->binaries[mapping] = binaryData;

        delete bin;
        delete diag;
    }
    this->evaluatingProgram = nullptr;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
uint32_t 
SPIRVGenerator::AddSymbol(const std::string& name, const std::string& declare, bool global)
{
    auto scope = this->scopeStack.rbegin();
    while (scope != this->scopeStack.rend())
    {
        auto it = scope->symbols.find(name);
        if (it != scope->symbols.end())
        {
            return it->second.value;
        }
        scope++;
    }

    // If symbol isn't found in scope, create it
    uint32_t ret = this->symbolCounter;
    if (global)
        this->scopeStack.front().symbols[name] = { .sym = nullptr, .value = ret };
    else
        this->scopeStack.back().symbols[name] = { .sym = nullptr, .value = ret };

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
SPIRVGenerator::AddReservedSymbol(const std::string& name, uint32_t object, const std::string& declare, bool global)
{
    auto scope = this->scopeStack.rbegin();
    while (scope != this->scopeStack.rend())
    {
        auto it = scope->symbols.find(name);
        if (it != scope->symbols.end())
        {
            assert(false);
            //break;
        }
        scope++;
    }

    // If symbol isn't found in scope, create it
    if (global)
        this->scopeStack.front().symbols[name] = { .sym = nullptr, .value = object };
    else
        this->scopeStack.back().symbols[name] = { .sym = nullptr, .value = object };

    std::string decl = Format("%%%d\t=\t%s\t\t\t; %s\n", object, declare.c_str(), name.c_str());
    if (global)
        this->declarations.append(decl);
    else
        this->functional.append(decl);
}

//------------------------------------------------------------------------------
/**
*/
const SymbolAssignment
SPIRVGenerator::GetSymbol(const std::string& name)
{
    SymbolAssignment ret = { .sym = nullptr, .value = 0xFFFFFFFF };
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
    assert(ret.value != 0xFFFFFFFF);
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
bool
SPIRVGenerator::HasSymbol(const std::string& name)
{
    SymbolAssignment ret = { .sym = nullptr, .value = 0xFFFFFFFF };
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
    return ret.value != 0xFFFFFFFF;
}

//------------------------------------------------------------------------------
/**
*/
void 
SPIRVGenerator::AddOp(const std::string& value, bool global, std::string comment)
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
SPIRVGenerator::AddMappedOp(const std::string& name, std::string comment)
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
SPIRVGenerator::AddCapability(const std::string& declare)
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
SPIRVGenerator::AddExtension(const std::string& name)
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
SPIRVGenerator::AddDecoration(const std::string& name, uint32_t object, const std::string& decorate)
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
SPIRVGenerator::AddMemberDecoration(uint32_t struc, uint32_t index, const std::string& decorate)
{
    this->decorations.append(Format("\tOpMemberDecorate %%%d %d %s\n", struc, index, decorate.c_str()));
}

//------------------------------------------------------------------------------
/**
*/
void
SPIRVGenerator::AddMapping(const std::string& name, uint32_t object)
{
    auto scope = this->scopeStack.rbegin();
    while (scope != this->scopeStack.rend())
    {
        auto it = scope->symbols.find(name);
        if (it != scope->symbols.end())
        {
            assert(false);
        }
        scope++;
    }

    // If symbol isn't found in scope, create it
    this->scopeStack.back().symbols[name] = { .sym = nullptr, .value = object };
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
SPIRVGenerator::AddReserved(const std::string& op, uint32_t name, std::string comment)
{
    this->functional.append(Format("%%%d = \t%s\t\t\t; %s\n", name, op.c_str(), comment.c_str()));
}

//------------------------------------------------------------------------------
/**
*/
uint32_t 
SPIRVGenerator::AddVariableDeclaration(Symbol* sym, const std::string& name, uint32_t type, uint32_t init, SPIRVResult::Storage storage, bool global)
{
    auto scope = this->scopeStack.rbegin();
    while (scope != this->scopeStack.rend())
    {
        auto it = scope->symbols.find(name);
        if (it != scope->symbols.end())
        {
            return it->second.value;
        }
        scope++;
    }

    // If symbol isn't found in scope, create it
    uint32_t ret = this->symbolCounter;
    this->scopeStack.back().symbols[name] = SymbolAssignment{ .sym = sym, .value = ret };

    std::string scopeStr = SPIRVResult::ScopeToString(storage);

    if (!global)
    {
        if (init != 0)
            this->variableDeclarations.append(Format("%%%d = \tOpVariable %%%d %s %%%d\t\t\t; %s\n", this->symbolCounter, type, scopeStr.c_str(), init, name.c_str()));
        else
            this->variableDeclarations.append(Format("%%%d = \tOpVariable %%%d %s\t\t\t; %s\n", this->symbolCounter, type, scopeStr.c_str(), name.c_str()));
    }
    else
    {
        if (storage == SPIRVResult::Storage::StorageBuffer 
            || storage == SPIRVResult::Storage::Image 
            || storage == SPIRVResult::Storage::MutableImage 
            || storage == SPIRVResult::Storage::Uniform 
            || storage == SPIRVResult::Storage::UniformConstant
            || storage == SPIRVResult::Storage::Private
            )
            this->interfaceVariables.insert(this->symbolCounter);

        if (init != 0)
            this->declarations.append(Format("%%%d = \tOpVariable %%%d %s %%%d\t\t\t; %s\n", this->symbolCounter, type, scopeStr.c_str(), init, name.c_str()));
        else
            this->declarations.append(Format("%%%d = \tOpVariable %%%d %s\t\t\t; %s\n", this->symbolCounter, type, scopeStr.c_str(), name.c_str()));
    }
    return this->symbolCounter++;

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
