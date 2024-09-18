//------------------------------------------------------------------------------
//  @file variable.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "variable.h"
#include "util.h"

#include <map>
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
    typeResolved->imageFormat = InvalidImageFormat;
    typeResolved->inBinding = 0xF;
    typeResolved->outBinding = 0xF;
    typeResolved->byteSize = 0;
    typeResolved->structureOffset = 0;
    typeResolved->elementPadding = 0;
    typeResolved->startPadding = 0;
    typeResolved->value = nullptr;
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

std::map<std::string, Variable::ImageFormat> Variable::stringToFormats;
//------------------------------------------------------------------------------
/**
*/
void Variable::SetupImageFormats()
{
    stringToFormats =
    {
        { "rgba16", RGBA16 },
        { "rgb10_a2", RGB10_A2 },
        { "rgba8", RGBA8 },
        { "rg16", RG16 },
        { "rg8", RG8 },
        { "r16", R16 },
        { "r8", R8 },
        { "rgba16_snorm", RGBA16_SNORM },
        { "rgba8_snorm", RGBA8_SNORM },
        { "rg16_snorm", RG16_SNORM },
        { "rg8_snorm", RG8_SNORM },
        { "r16_snorm", R16_SNORM },
        { "r8_snorm", R8_SNORM },
        { "rgba32f", RGBA32F },
        { "rgba16f", RGBA16F },
        { "rg32f", RG32F },
        { "rg16f", RG16F },
        { "r11g11b10f", R11G11B10F },
        { "r32f", R32F },
        { "r16f", R16F },
        { "rga32i", RGBA32I },
        { "rgba16i", RGBA16I },
        { "rgba8i", RGBA8I },
        { "rg32i", RG32I },
        { "rg16i", RG16I },
        { "rg8i", RG8I },
        { "r32i", R32I },
        { "r16i", R16I },
        { "r8i", R8I },
        { "rga32u", RGBA32U },
        { "rgba16u", RGBA16U },
        { "rgb10_a2u", RGB10_A2U },
        { "rgba8u", RGBA8U },
        { "rg32u", RG32U },
        { "rg16u", RG16U },
        { "rg8u", RG8U },
        { "r32u", R32U },
        { "r16u", R16U },
        { "r8u", R8U }
    };
}

//------------------------------------------------------------------------------
/**
*/
Variable::ImageFormat 
Variable::StringToImageFormat(const std::string& format)
{
    auto it = stringToFormats.find(format);
    if (it == stringToFormats.end())
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
    return format >= RGBA32I && format <= R8I;
}

//------------------------------------------------------------------------------
/**
*/
bool 
Variable::IsImageFormatUnsigned(ImageFormat format)
{
    return format >= RGBA32U && format <= R8U;
}

} // namespace GPULang
