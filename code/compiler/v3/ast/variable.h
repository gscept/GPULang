#pragma once
//------------------------------------------------------------------------------
/**
    AST variable (type name)

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "attribute.h"
#include "annotation.h"
#include "types/type.h"
#include "shaderusage.h"
#include <vector>
#include <unordered_map>
namespace GPULang
{
enum ImageFormat
{
    InvalidImageFormat,

    // byte
    RGBA16, RGB10_A2, RGBA8, RG16, RG8,
    R16, R8, RGBA16_SNORM, RGBA8_SNORM, RG16_SNORM, RG8_SNORM,
    R16_SNORM, R8_SNORM,

    // float
    RGBA32F, RGBA16F, RG32F, RG16F, R11G11B10F,
    R32F, R16F,

    // integer
    RGBA32I, RGBA16I, RGBA8I, RG32I, RG16I, RG8I,
    R32I, R16I, R8I,

    // unsigned integer
    RGBA32U, RGBA16U, RGB10_A2U, RGBA8U, RG32U, RG16U, RG8U,
    R32U, R16U, R8U,

    // Format is not known
    Unknown
};

extern std::unordered_map<std::string, ImageFormat> StringToFormats;
struct Variable : public Symbol
{
    /// constructor
    Variable();
    /// destructor
    ~Variable();

    std::vector<std::string> values;
    Type::FullType type;
    Expression* valueExpression;
    _IMPLEMENT_ANNOTATIONS()
    _IMPLEMENT_ATTRIBUTES()

    static ImageFormat StringToImageFormat(const std::string& format);
    static bool IsImageFormatInteger(ImageFormat format);
    static bool IsImageFormatUnsigned(ImageFormat format);

    struct __Resolved : Symbol::__Resolved
    {
        union AccessBits
        {
            AccessBits(uint32_t mask)
                : bits(mask)
            {}
            struct
            {
                uint32_t readAccess : 1;
                uint32_t writeAccess : 1;
                uint32_t atomicAccess : 1;
                uint32_t volatileAccess : 1;
            } flags;
            uint32_t bits;
        };
        AccessBits accessBits = 0x0;

        union ParameterBits
        {
            ParameterBits(uint32_t mask)
                : bits(mask)
            {}
            struct
            {
                uint32_t isPatch : 1;
                uint32_t isNoPerspective : 1;
                uint32_t isNoInterpolate : 1;
                uint32_t isCentroid : 1;
            } flags;
            uint32_t bits;
        };
        ParameterBits parameterBits = 0x0;

        union UsageBits
        {
            UsageBits(uint32_t mask)
                : bits(mask)
            {}
            struct
            {
                uint32_t isConst : 1;                   // variable may not change value after initialization
                uint32_t isVar : 1;
                uint32_t isParameter : 1;               // variable is a function parameter
                uint32_t isEntryPointParameter : 1;
                uint32_t isStructMember : 1;            // variable is a struct member
                uint32_t isSampled : 1;                 // for variables of image type, this means a sampler has to be associated from the client
            } flags;
            uint32_t bits;
        };
        UsageBits usageBits = 0x0;

        enum class Storage
        {
            Default,                                // default storage, on the stack
            Uniform,                                // variable is uniform (const) across all threads and provided by the CPU
            Workgroup,                              // variable is shared by workgroup and can be written/read
            InlineUniform,                          // variable is uniform but read from command buffer
            Input,                                  // variable is an input from a previous shader stage
            Output,                                 // variable is an output from the current shader stage
            Global                                  // variable is global in the shader

        } storage = Storage::Default;
        std::vector<Variable*> siblings;

        /// type here is the fully qualified (pointer and array) type
        Type* typeSymbol;
        Type::FullType type;
        std::string name;
        Expression* value;

        static const uint8_t NOT_BOUND = 0xF;
        uint32_t group;          // resource group
        uint32_t binding;        // resource binding

        uint8_t inBinding;      // parameter input binding
        uint8_t outBinding;     // parameter output binding

        uint32_t byteSize;          // size in bytes
        uint32_t structureOffset;   // offset within structure
        uint32_t elementPadding;    // padding between elements
        uint32_t startPadding;      // padding before variable

        ImageFormat imageFormat;    // for read write images, this is the format

        ShaderUsage visibilityBits;
    };
};


} // namespace GPULang
