#pragma once
//------------------------------------------------------------------------------
/**
    Header for serializable types

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include <string>
#include <vector>
#include <cstring>
#include "shaderusage.h"

namespace GPULang
{

namespace Serialization
{


//------------------------------------------------------------------------------
/**
 Enums shared between compiler and API
 */
//------------------------------------------------------------------------------
enum class AddressMode
{
    InvalidAddressMode,
    RepeatAddressMode,
    MirrorAddressMode,
    ClampAddressMode,
    BorderAddressMode
};

enum class Filter
{
    InvalidFilter,
    PointFilter,
    LinearFilter
};

enum class BorderColor
{
    InvalidBorderColor,
    TransparentBorder,
    BlackBorder,
    WhiteBorder
};

enum class CompareMode
{
    InvalidCompareMode,
    NeverCompare,
    LessCompare,
    EqualCompare,
    LessEqualCompare,
    GreaterCompare,
    NotEqualCompare,
    GreaterEqualCompare,
    AlwaysCompare
};

enum class PolygonMode
{
    InvalidPolygonMode,
    FillMode,
    LineMode,
    PointMode
};

enum class CullMode
{
    InvalidCullMode,
    NoCullMode,
    FrontMode,
    BackMode,
    FrontAndBackMode
};

enum class WindingOrderMode
{
    InvalidWindingOrderMode,
    ClockwiseMode,
    CounterClockwiseMode
};

enum class StencilOp
{
    InvalidStencilOp,
    StencilKeepOp,
    StencilZeroOp,
    StencilReplaceOp,
    StencilIncrementClampOp,
    StencilDecrementClampOp,
    StencilInvertOp,
    StencilIncrementWrapOp,
    StencilDecrementWrapOp
};

enum class LogicOp
{
    InvalidLogicOp,
    LogicClearOp,
    LogicAndOp,
    LogicAndReverseOp,
    LogicCopyOp,
    LogicAndInvertedOp,
    LogicNoOp,
    LogicXorOp,
    LogicOrOp,
    LogicNorOp,
    LogicEquivalentOp,
    LogicInvertOp,
    LogicOrReverseOp,
    LogicCopyInvertedOp,
    LogicOrInvertedOp,
    LogicNandOp,
    LogicSetOp
};

enum class BlendFactor
{
    InvalidBlendFactor,
    ZeroFactor,
    OneFactor,
    SourceColorFactor,
    OneMinusSourceColorFactor,
    DestinationColorFactor,
    OneMinusDestinationColorFactor,
    SourceAlphaFactor,
    OneMinusSourceAlphaFactor,
    DestinationAlphaFactor,
    OneMinusDestinationAlphaFactor,
    ConstantColorFactor,
    OneMinusConstantColorFactor,
    ConstantAlphaFactor,
    OneMinusConstantAlphaFactor,
    SourceAlphaSaturateFactor,
    Source1ColorFactor,
    OneMinusSource1ColorFactor,
    Source1AlphaFactor,
    OneMinusSource1AlphaFactor
};

enum class BlendOp
{
    InvalidBlendOp,
    AddOp,
    SubtractOp,
    ReverseSubtractOp,
    MinOp,
    MaxOp
};

enum ColorComponents
{
    RedComponent = 0x1,
    GreenComponent = 0x2,
    BlueComponent = 0x4,
    AlphaComponent = 0x8
};

struct StencilState
{
    StencilOp fail;
    StencilOp pass;
    StencilOp depthFail;
    CompareMode compare;
    uint32_t compareMask;
    uint32_t writeMask;
    uint32_t referenceMask;
};

struct BlendState
{
    bool blendEnabled;
    BlendFactor sourceColorBlendFactor;
    BlendFactor destinationColorBlendFactor;
    BlendFactor sourceAlphaBlendFactor;
    BlendFactor destinationAlphaBlendFactor;
    BlendOp colorBlendOp;
    BlendOp alphaBlendOp;
    uint32_t colorComponentMask;
};

enum class BindingScope
{
    VertexInput,        // For vertex attributes
    Resource,           // For images, buffers, atomic counters, acceleration structures
    Constant,           // For constant shader values
    Member              // For struct members
};

enum class BindingType
{
    None,
    MutableBuffer,
    Buffer,
    MutableImage,
    Image,
    SampledImage,
    Sampler,
    PixelCache,
    Inline,
    LinkDefined,
    AccelerationStructure
};

} // namespace Serialization


namespace Serialize
{

enum Type
{
    SamplerStateType,
    VariableType,
    StructureType,
    ProgramType,
    RenderStateType
};

enum DataType
{
    NoData,
    FloatType,
    IntType,
    BoolType,
    StringType
};

struct Serializable
{
    Type type;
    size_t annotationsOffset;
    size_t annotationsCount;
    size_t nameOffset;
    size_t nameLength;
};

struct Annotation
{
    size_t nameOffset;
    size_t nameLength;
    DataType type;
    union
    {
        float f;
        int i;
        bool b;
        struct
        {
            size_t stringOffset;
            size_t stringLength;
        } s;
    } data;
};

struct RenderState : public Serializable
{
    /// constructor
    RenderState()
    {
        this->type = RenderStateType;
    }
    bool depthClampEnabled;
    bool noPixels;
    Serialization::PolygonMode polygonMode;
    Serialization::CullMode cullMode;
    Serialization::WindingOrderMode windingOrderMode;
    bool depthBiasEnabled;
    float depthBiasFactor;
    float depthBiasClamp;
    float depthBiasSlopeFactor;
    float lineWidth;
    bool depthTestEnabled;
    bool depthWriteEnabled;
    Serialization::CompareMode depthCompare;
    bool depthBoundsTestEnabled;
    float minDepthBounds;
    float maxDepthBounds;
    bool scissorEnabled;
    bool stencilEnabled;
    Serialization::StencilState frontStencilState;
    Serialization::StencilState backStencilState;
    bool logicOpEnabled;
    Serialization::LogicOp logicOp;
    size_t blendStatesOffset;
    size_t blendStatesCount;
    float blendConstants[4];
};

struct Program : public Serializable
{
    /// constructor
    Program()
    {
        this->type = ProgramType;
    }

    size_t renderStateNameOffset;
    size_t renderStateNameLength;

    struct Shader
    {
        /// constructor
        Shader()
            : binaryOffset(-1)
            , binaryLength(-1)
        {}

        size_t binaryOffset;
        size_t binaryLength;
    };

    Shader vs, gs, hs, ds, ps, cs, ts, ms, rgs, rms, rchs, rahs, rcs, ris;

    size_t vsInputsLength;
    size_t vsInputsOffset;
    uint16_t patchSize;                 // Patch size if program contains a hull shader
    uint16_t rayPayloadSize;            // Ray payload size in bytes if program contains a ray tracing shader
    uint16_t rayHitAttributeSize;       // Ray hit attribute size in bytes if program contains a ray tracing hit shader

};

struct Bindable : public Serializable
{
    unsigned int binding;
    unsigned int group;
};

struct SamplerState : public Bindable
{
    /// constructor
    SamplerState()
    {
        this->type = SamplerStateType;
    }
    Serialization::AddressMode addressU;
    Serialization::AddressMode addressV;
    Serialization::AddressMode addressW;

    Serialization::Filter minFilter;
    Serialization::Filter magFilter;
    Serialization::Filter mipFilter;

    float mipLodBias;
    bool anisotropicEnabled;
    float maxAnisotropy;

    bool compareSamplerEnabled;
    Serialization::CompareMode compareMode;

    float minLod;
    float maxLod;

    Serialization::BorderColor borderColor;

    bool unnormalizedSamplingEnabled;

    ShaderUsage visibility;
};

struct Variable : public Bindable
{
    /// constructor
    Variable()
    {
        this->type = VariableType;
    }
    size_t arraySizesOffset;
    size_t arraySizesCount;

    uint32_t byteSize;
    uint32_t structureOffset;

    Serialization::BindingScope bindingScope;
    Serialization::BindingType bindingType;
    ShaderUsage visibility;

    size_t structTypeNameOffset;
    size_t structTypeNameLength;
};

struct Structure : public Serializable
{
    /// constructor
    Structure()
    {
        this->type = StructureType;
    }
    bool isUniform;
    bool isMutable;
    unsigned int size;
    size_t variablesOffset;
    size_t variablesCount;
};

struct DynamicLengthBlob
{
    size_t capacity;
    size_t iterator;
    char* data;

    /// constructor
    DynamicLengthBlob()
        : capacity(256)
        , iterator(0)
    {
        this->data = new char[this->capacity];
    }

#ifndef min
#define min(x,y) (x < y ? x : y)
#endif

#ifndef max
#define max(x,y) (x > y ? x : y)
#endif

//------------------------------------------------------------------------------
/**
    Grow blob
*/
void Grow(size_t size)
{
    if (this->iterator + size >= this->capacity)
    {
        size_t growClamped = min(this->capacity << 1, 65535);
        size_t growBy = max(this->capacity + growClamped, size);

        size_t oldCapacity = this->capacity;
        char* oldData = this->data;

        this->capacity = growBy + this->capacity;
        this->data = new char[this->capacity];
        if (oldCapacity > 0)
            memcpy(this->data, oldData, oldCapacity);

        delete[] oldData;
    }
}
#undef min
#undef max

//------------------------------------------------------------------------------
/**
    Write block of memory
*/
size_t 
Write(const char* data, size_t size)
{
    this->Grow(size);
    memcpy(this->data + this->iterator, data, size);
    size_t ret = this->iterator;
    this->iterator += size;

    return ret;
};

//------------------------------------------------------------------------------
/**
    Write string
*/
size_t 
WriteString(const char* data, size_t size)
{
    this->Grow(size + 1);
    memcpy(this->data + this->iterator, data, size + 1);
    size_t ret = this->iterator;
    this->iterator += size + 1;

    return ret;
};

//------------------------------------------------------------------------------
/**
    Write type (short hand for writing a block)
*/
template <typename T>
size_t 
Write(const T& data)
{
    return this->Write((const char*)&data, sizeof(T));
}

//------------------------------------------------------------------------------
/**
    Write array of type (short hand for writing a block)
*/
template <typename T>
size_t 
Write(const T* data, const size_t count)
{
    return this->Write((const char*)data, sizeof(T) * count);
}

//------------------------------------------------------------------------------
/**
    Reserve memory without writing
*/
size_t 
Reserve(size_t size)
{
    this->Grow(size);
    size_t ret = this->iterator;
    this->iterator += size;
    return ret;
}

//------------------------------------------------------------------------------
/**
    Reserve type without writing
*/
template <typename T>
size_t 
Reserve(size_t count)
{
    return this->Reserve(sizeof(T) * count);
}

//------------------------------------------------------------------------------
/**
    Write block of memory to reserved memory
*/
void 
WriteReserved(const char* data, size_t offset, size_t size)
{
    memcpy(this->data + offset, data, size);
}

//------------------------------------------------------------------------------
/**
    Write type to reserved memory
*/
template <typename T>
void 
WriteReserved(const T& data, size_t offset)
{
    memcpy(this->data + offset, &data, sizeof(T));
}

};

} // namespace Serialize

namespace Deserialize
{

struct Annotation
{
    const char* name;
    size_t nameLength;

    Serialize::DataType type;
    union
    {
        float f;
        int i;
        bool b;
        struct
        {
            const char* string;
            size_t length;
        } s;
    } data;
};


struct Deserializable
{
    Serialize::Type type;
    Annotation* annotations;
    size_t annotationCount;
    const char* name;
    size_t nameLength;
};

struct RenderState : public Deserializable
{
    bool depthClampEnabled;
    bool noPixels;
    Serialization::PolygonMode polygonMode;
    Serialization::CullMode cullMode;
    Serialization::WindingOrderMode windingOrderMode;
    bool depthBiasEnabled;
    float depthBiasFactor;
    float depthBiasClamp;
    float depthBiasSlopeFactor;
    float lineWidth;
    bool depthTestEnabled;
    bool depthWriteEnabled;
    Serialization::CompareMode depthCompare;
    bool depthBoundsTestEnabled;
    float minDepthBounds;
    float maxDepthBounds;
    bool stencilEnabled;
    Serialization::StencilState frontStencilState;
    Serialization::StencilState backStencilState;
    bool logicOpEnabled;
    Serialization::LogicOp logicOp;
    Serialization::BlendState blendStates[8];
    float blendConstants[4];
};

struct Program : public Deserializable
{
    const RenderState* renderState;

    struct Shader
    {
        /// constructor
        Shader()
            : binary(nullptr)
            , binaryLength(-1)
        {}

        const uint32_t* binary;
        size_t binaryLength;
    };

    enum ShaderStages
    {
        VertexShader,
        HullShader,
        DomainShader,
        GeometryShader,
        PixelShader,
        ComputeShader,
        TaskShader,
        MeshShader,
        RayGenShader,
        RayAnyHitShader,
        RayClosestHitShader,
        RayMissShader,
        RayIntersectionShader,
        RayCallableShader,

        NumShaders
    };

    size_t vsInputLength;
    const uint8_t* vsInputs;

    uint16_t patchSize;                 // Patch size if program contains a hull shader
    uint16_t rayPayloadSize;            // Ray payload size in bytes if program contains a ray tracing shader
    uint16_t rayHitAttributeSize;       // Ray hit attribute size in bytes if program contains a ray tracing hit shader

    Shader shaders[ShaderStages::NumShaders];
};

struct Bindable : public Deserializable
{
    unsigned int binding;
    unsigned int group;
};

struct SamplerState : public Bindable
{
    Serialization::AddressMode addressU;
    Serialization::AddressMode addressV;
    Serialization::AddressMode addressW;

    Serialization::Filter minFilter;
    Serialization::Filter magFilter;
    Serialization::Filter mipFilter;

    float mipLodBias;
    bool anisotropicEnabled;
    float maxAnisotropy;

    bool compareSamplerEnabled;
    Serialization::CompareMode compareMode;

    float minLod;
    float maxLod;

    Serialization::BorderColor borderColor;

    bool unnormalizedSamplingEnabled;

    ShaderUsage visibility;
};

struct Structure;
struct Variable : public Bindable
{
    const uint32_t* arraySizes;
    uint32_t arraySizeCount;

    uint32_t byteSize;
    uint32_t structureOffset;

    Serialization::BindingScope bindingScope;
    Serialization::BindingType bindingType;
    ShaderUsage visibility;

    const Structure* structType;
};

struct Structure : public Deserializable
{
    /// constructor
    unsigned int size;
    Variable* variables;
    size_t variableCount;
};

} // namespace Deserialize

} // namespace GPULang
