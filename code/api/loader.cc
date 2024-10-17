//------------------------------------------------------------------------------
//  @file loader.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "loader.h"

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
template<typename T>
inline const T* 
ParseAndConsume(const char* data, size_t& offset)
{
    const T* ret = reinterpret_cast<const T*>(data + offset);
    offset += sizeof(T);
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
inline const T* 
Parse(const char* data, const size_t offset)
{
    return reinterpret_cast<const T*>(data + offset);
}

//------------------------------------------------------------------------------
/**
*/
void
LoadAnnotations(size_t annotationCount, GPULang::Deserialize::Annotation* annotations, size_t offset, const char* buf)
{
    size_t annotationsOffset = offset;
    for (size_t i = 0; i < annotationCount; i++)
    {
        const GPULang::Serialize::Annotation* annotation = ParseAndConsume<GPULang::Serialize::Annotation>(buf, annotationsOffset);
        GPULang::Deserialize::Annotation& annotDeserialized = annotations[i];

        annotDeserialized.name = Parse<const char>(buf, annotation->nameOffset);
        annotDeserialized.nameLength = annotation->nameLength;
        annotDeserialized.type = annotation->type;
        switch (annotDeserialized.type)
        {
        case Serialize::FloatType:
            annotDeserialized.data.f = annotation->data.f;
            break;
        case Serialize::IntType:
            annotDeserialized.data.i = annotation->data.i;
            break;
        case Serialize::BoolType:
            annotDeserialized.data.b = annotation->data.b;
            break;
        case Serialize::StringType:
            annotDeserialized.data.s.string = Parse<const char>(buf, annotation->data.s.stringOffset);
            annotDeserialized.data.s.length = annotation->data.s.stringLength;
            break;
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
Loader::Load(const char* data, const size_t length)
{
    size_t frontIterator = 0;
    size_t backIterator = length;

    const uint32_t* magic = reinterpret_cast<const uint32_t*>(data + frontIterator);
    frontIterator += sizeof(int32_t);
    if (*magic != 'AFX3')
        return;

    // read the blob at the bottom of the file
    backIterator -= sizeof(uint32_t);
    const uint32_t* size = reinterpret_cast<const uint32_t*>(data + backIterator);
    backIterator -= *size;
    const char* buf = data + backIterator;

    while (frontIterator != backIterator)
    {
        const GPULang::Serialize::Serializable* type = Parse<GPULang::Serialize::Serializable>(data, frontIterator);
        switch (type->type)
        {
        case GPULang::Serialize::SamplerStateType:
        {
            const GPULang::Serialize::SamplerState* samplerState = ParseAndConsume<GPULang::Serialize::SamplerState>(data, frontIterator);
            GPULang::Deserialize::SamplerState* deserialized = new GPULang::Deserialize::SamplerState;
            deserialized->type = type->type;
            deserialized->name = Parse<const char>(buf, samplerState->nameOffset);
            deserialized->nameLength = samplerState->nameLength;
            deserialized->binding = samplerState->binding;
            deserialized->group = samplerState->group;

            deserialized->addressU = samplerState->addressU;
            deserialized->addressV = samplerState->addressV;
            deserialized->addressW = samplerState->addressW;
            deserialized->minFilter = samplerState->minFilter;
            deserialized->magFilter = samplerState->magFilter;
            deserialized->mipFilter = samplerState->mipFilter;
            deserialized->mipLodBias = samplerState->mipLodBias;
            deserialized->anisotropicEnabled = samplerState->anisotropicEnabled;
            deserialized->maxAnisotropy = samplerState->maxAnisotropy;
            deserialized->compareSamplerEnabled = samplerState->compareSamplerEnabled;
            deserialized->compareMode = samplerState->compareMode;
            deserialized->minLod = samplerState->minLod;
            deserialized->maxLod = samplerState->maxLod;
            deserialized->borderColor = samplerState->borderColor;
            deserialized->unnormalizedSamplingEnabled = samplerState->unnormalizedSamplingEnabled;

            deserialized->annotationCount = samplerState->annotationsCount;
            deserialized->annotations = nullptr;
            if (deserialized->annotationCount > 0)
            {
                deserialized->annotations = new GPULang::Deserialize::Annotation[deserialized->annotationCount];
                LoadAnnotations(deserialized->annotationCount, deserialized->annotations, samplerState->annotationsOffset, buf);
            }

            this->nameToObject[deserialized->name] = deserialized;
            break;
        }
        case GPULang::Serialize::VariableType:
        {
            const GPULang::Serialize::Variable* var = ParseAndConsume<GPULang::Serialize::Variable>(data, frontIterator);
            GPULang::Deserialize::Variable* deserialized = new GPULang::Deserialize::Variable;
            deserialized->type = type->type;
            deserialized->name = Parse<const char>(buf, var->nameOffset);
            deserialized->nameLength = var->nameLength;
            deserialized->binding = var->binding;
            deserialized->group = var->group;
            deserialized->arraySizeCount = var->arraySizesCount;
            deserialized->arraySizes = Parse<uint32_t>(buf, var->arraySizesOffset);

            deserialized->structureOffset = var->structureOffset;
            deserialized->byteSize = var->byteSize;
            

            deserialized->annotationCount = var->annotationsCount;
            deserialized->annotations = nullptr;
            if (deserialized->annotationCount > 0)
            {
                deserialized->annotations = new GPULang::Deserialize::Annotation[deserialized->annotationCount];
                LoadAnnotations(deserialized->annotationCount, deserialized->annotations, var->annotationsOffset, buf);
            }

            this->nameToObject[deserialized->name] = deserialized;
            break;
        }
        case GPULang::Serialize::StructureType:
        {
            const GPULang::Serialize::Structure* struc = ParseAndConsume<GPULang::Serialize::Structure>(data, frontIterator);
            GPULang::Deserialize::Structure* deserialized = new GPULang::Deserialize::Structure;
            deserialized->type = type->type;
            deserialized->name = Parse<const char>(buf, struc->nameOffset);
            deserialized->nameLength = struc->nameLength;
            deserialized->variableCount = struc->variablesCount;
            deserialized->variables = new GPULang::Deserialize::Variable[deserialized->variableCount];
            size_t variableIterator = struc->variablesOffset;

            for (int i = 0; i < struc->variablesCount; i++)
            {
                const GPULang::Serialize::Variable* var = ParseAndConsume<GPULang::Serialize::Variable>(buf, variableIterator);
                GPULang::Deserialize::Variable& deserializedVar = deserialized->variables[i];

                deserializedVar.binding = var->binding;
                deserializedVar.group = var->group;
                deserializedVar.arraySizeCount = var->arraySizesCount;
                deserializedVar.arraySizes = Parse<uint32_t>(buf, var->arraySizesOffset);
                deserializedVar.structureOffset = var->structureOffset;
                deserializedVar.byteSize = var->byteSize;
                deserializedVar.name = Parse<const char>(buf, var->nameOffset);
                deserializedVar.nameLength = var->nameLength;
            }

            deserialized->annotationCount = struc->annotationsCount;
            deserialized->annotations = nullptr;
            if (deserialized->annotationCount > 0)
            {
                deserialized->annotations = new GPULang::Deserialize::Annotation[deserialized->annotationCount];
                LoadAnnotations(deserialized->annotationCount, deserialized->annotations, struc->annotationsOffset, buf);
            }

            this->nameToObject[deserialized->name] = deserialized;
            break;
        }
        case GPULang::Serialize::ProgramType:
        {
            const GPULang::Serialize::Program* prog = ParseAndConsume<GPULang::Serialize::Program>(data, frontIterator);
            GPULang::Deserialize::Program* deserialized = new GPULang::Deserialize::Program;
            deserialized->type = type->type;
            deserialized->name = Parse<const char>(buf, prog->nameOffset);
            deserialized->nameLength = prog->nameLength;

#define LOAD_SHADER(x) \
if (prog->##x.binaryOffset != 0)\
{\
    deserialized->##x.binary = Parse<uint32_t>(buf, prog->##x.binaryOffset);\
    deserialized->##x.binaryLength = prog->##x.binaryLength;\
}\
else\
{\
    deserialized->##x.binary = nullptr;\
    deserialized->##x.binaryLength = prog->##x.binaryLength;\
}

            LOAD_SHADER(vs)
            LOAD_SHADER(hs)
            LOAD_SHADER(ds)
            LOAD_SHADER(gs)
            LOAD_SHADER(ps)
            LOAD_SHADER(cs)
            LOAD_SHADER(ts)
            LOAD_SHADER(ms)
            LOAD_SHADER(rgs)
            LOAD_SHADER(rms)
            LOAD_SHADER(rchs)
            LOAD_SHADER(ris)
            LOAD_SHADER(rahs)
            LOAD_SHADER(rcs)

            // load shaders
            if (prog->renderStateNameOffset != 0)
            {
                std::string renderStateName = Parse<const char>(buf, prog->renderStateNameOffset);
                deserialized->renderState = (GPULang::Deserialize::RenderState*)this->nameToObject[renderStateName];
            }
            else
            {
                deserialized->renderState = nullptr;
            }

            deserialized->annotationCount = prog->annotationsCount;
            deserialized->annotations = nullptr;
            if (deserialized->annotationCount > 0)
            {
                deserialized->annotations = new GPULang::Deserialize::Annotation[deserialized->annotationCount];
                LoadAnnotations(deserialized->annotationCount, deserialized->annotations, prog->annotationsOffset, buf);
            }

            this->nameToObject[deserialized->name] = deserialized;
            break;
        }
        case GPULang::Serialize::RenderStateType:
        {
            const GPULang::Serialize::RenderState* rend = ParseAndConsume<GPULang::Serialize::RenderState>(data, frontIterator);
            GPULang::Deserialize::RenderState* deserialized = new GPULang::Deserialize::RenderState;
            deserialized->type = type->type;
            deserialized->name = Parse<const char>(buf, rend->nameOffset);
            deserialized->nameLength = rend->nameLength;
            deserialized->depthClampEnabled = rend->depthClampEnabled;
            deserialized->noPixels = rend->noPixels;
            deserialized->polygonMode = rend->polygonMode;
            deserialized->cullMode = rend->cullMode;
            deserialized->windingOrderMode = rend->windingOrderMode;
            deserialized->depthBiasEnabled = rend->depthBiasEnabled;
            deserialized->depthBiasFactor = rend->depthBiasFactor;
            deserialized->depthBiasClamp = rend->depthBiasClamp;
            deserialized->depthBiasSlopeFactor = rend->depthBiasSlopeFactor;
            deserialized->lineWidth = rend->lineWidth;
            deserialized->depthTestEnabled = rend->depthTestEnabled;
            deserialized->depthWriteEnabled = rend->depthWriteEnabled;
            deserialized->depthCompare = rend->depthCompare;
            deserialized->depthBoundsTestEnabled = rend->depthBoundsTestEnabled;
            deserialized->minDepthBounds = rend->minDepthBounds;
            deserialized->maxDepthBounds = rend->maxDepthBounds;
            deserialized->stencilEnabled = rend->stencilEnabled;
            deserialized->frontStencilState = rend->frontStencilState;
            deserialized->backStencilState = rend->backStencilState;
            deserialized->logicOpEnabled = rend->logicOp;
            deserialized->logicOp = rend->logicOp;
            for (size_t i = 0; i < rend->blendStatesCount; i++)
            {
                deserialized->blendStates[i] = *Parse<GPULang::BlendState>(buf, rend->blendStatesOffset + i * sizeof(BlendState));
            }

            deserialized->annotationCount = rend->annotationsCount;
            deserialized->annotations = nullptr;
            if (deserialized->annotationCount > 0)
            {
                deserialized->annotations = new GPULang::Deserialize::Annotation[deserialized->annotationCount];
                LoadAnnotations(deserialized->annotationCount, deserialized->annotations, rend->annotationsOffset, buf);
            }

            this->nameToObject[deserialized->name] = deserialized;
            break;
        }
        }
    }
}

} // namespace GPULang
