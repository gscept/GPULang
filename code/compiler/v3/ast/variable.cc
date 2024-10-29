//------------------------------------------------------------------------------
//  @file variable.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "variable.h"
#include "util.h"

#include <unordered_map>
namespace GPULang
{


//------------------------------------------------------------------------------
/**
*/
Variable::Variable()
{
    this->symbolType = VariableType;
    this->resolved = new Variable::__Resolved;
    this->type = { "" };
    this->valueExpression = nullptr;

    Variable::__Resolved* typeResolved = static_cast<Variable::__Resolved*>(this->resolved);
    typeResolved->accessBits.bits = 0x0;
    typeResolved->parameterBits.bits = 0x0;
    typeResolved->usageBits.bits = 0x0;
    typeResolved->group = __Resolved::NOT_BOUND;
    typeResolved->binding = __Resolved::NOT_BOUND;
    typeResolved->imageFormat = ImageFormat::Unknown;
    typeResolved->inBinding = 0xF;
    typeResolved->outBinding = 0xF;
    typeResolved->byteSize = 0;
    typeResolved->structureOffset = 0;
    typeResolved->elementPadding = 0;
    typeResolved->startPadding = 0;
    typeResolved->value = nullptr;
    typeResolved->visibilityBits.bits = 0x0;
}

//------------------------------------------------------------------------------
/**
*/
Variable::~Variable()
{
    Variable::__Resolved* typeResolved = static_cast<Variable::__Resolved*>(this->resolved);
    for (Variable* sibling : typeResolved->siblings)
        delete sibling;
    delete typeResolved;
}

std::unordered_map<std::string, ImageFormat> StringToFormats =
{
    { "rgba16", Rgba16 }
    , { "rgb10_a2", Rgb10_A2 }
    , { "rgba8", Rgba8 }
    , { "rg16", Rg16 }
    , { "rg8", Rg8 }
    , { "r16", R16 }
    , { "r8", R8 }
    , { "rgba16_snorm", Rgba16_Snorm }
    , { "rgba8_snorm", Rgba8_Snorm }
    , { "rg16_snorm", Rg16_Snorm }
    , { "rg8_snorm", Rg8_Snorm }
    , { "r16_snorm", R16_Snorm }
    , { "r8_snorm", R8_Snorm }
    , { "rgba32f", Rgba32F }
    , { "rgba16f", Rgba16F }
    , { "rg32f", Rg32F }
    , { "rg16f", Rg16F }
    , { "r11g11b10f", R11G11B10F }
    , { "r32f", R32F }
    , { "r16f", R16F }
    , { "rga32i", Rgba32I }
    , { "rgba16i", Rgba16I }
    , { "rgba8i", Rgba8I }
    , { "rg32i", Rg32I }
    , { "rg16i", Rg16I }
    , { "rg8i", Rg8I }
    , { "r32i", R32I }
    , { "r16i", R16I }
    , { "r8i", R8I }
    , { "rga32u", Rgba32U }
    , { "rgba16u", Rgba16U }
    , { "rgb10_a2u", Rgb10_A2U }
    , { "rgba8u", Rgba8U }
    , { "rg32u", Rg32U }
    , { "rg16u", Rg16U }
    , { "rg8u", Rg8U }
    , { "r32u", R32U }
    , { "r16u", R16U }
    , { "r8u", R8U }
    , { "unknown", Unknown }
};

//------------------------------------------------------------------------------
/**
*/
ImageFormat 
Variable::StringToImageFormat(const std::string& format)
{
    auto it = StringToFormats.find(format);
    if (it == StringToFormats.end())
        return InvalidImageFormat;
    else
        return it->second;
}

//------------------------------------------------------------------------------
/**
*/
bool 
Variable::IsImageFormatInteger(ImageFormat format)
{
    return format >= Rgba32I && format <= R8I;
}

//------------------------------------------------------------------------------
/**
*/
bool 
Variable::IsImageFormatUnsigned(ImageFormat format)
{
    return format >= Rgba32U && format <= R8U;
}

} // namespace GPULang
