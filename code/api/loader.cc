//------------------------------------------------------------------------------
//  @file loader.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "loader.h"
#include "serialize.h"

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
Loader::~Loader()
{
    auto it = this->nameToObject.begin();
    while (it != this->nameToObject.end())
    {
        delete it->second;
    }
}

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
LoadAnnotations(size_t annotationCount, AnyFX::Deserialize::Annotation* annotations, size_t offset, const char* buf)
{
    size_t annotationsOffset = offset;
    for (size_t i = 0; i < annotationCount; i++)
    {
        const AnyFX::Serialize::Annotation* annotation = ParseAndConsume<AnyFX::Serialize::Annotation>(buf, annotationsOffset);
        AnyFX::Deserialize::Annotation& annotDeserialized = annotations[i];

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
    frontIterator += sizeof(uint32_t);
    if (*magic != 'AFX3')
        return;

    // read the blob at the bottom of the file
    backIterator -= sizeof(uint32_t);
    const uint32_t* size = reinterpret_cast<const uint32_t*>(data + backIterator);
    backIterator -= *size;
    const char* buf = data + backIterator;

    while (frontIterator != backIterator)
    {
        const AnyFX::Serialize::Serializable* type = ParseAndConsume<AnyFX::Serialize::Serializable>(data, frontIterator);
        switch (type->type)
        {
        case AnyFX::Serialize::SamplerStateType:
        {
            const AnyFX::Serialize::SamplerState* samplerState = ParseAndConsume<AnyFX::Serialize::SamplerState>(data, frontIterator);
            AnyFX::Deserialize::SamplerState* deserialized = new AnyFX::Deserialize::SamplerState;
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
            deserialized->annotations = new AnyFX::Deserialize::Annotation[deserialized->annotationCount];
            LoadAnnotations(deserialized->annotationCount, deserialized->annotations, samplerState->annotationsOffset, buf);

            this->nameToObject[deserialized->name] = deserialized;
            break;
        }
        case AnyFX::Serialize::VariableType:
        {
            const AnyFX::Serialize::Variable* var = ParseAndConsume<AnyFX::Serialize::Variable>(data, frontIterator);
            AnyFX::Deserialize::Variable* deserialized = new AnyFX::Deserialize::Variable;
            deserialized->name = Parse<const char>(buf, var->nameOffset);
            deserialized->nameLength = var->nameLength;
            deserialized->binding = var->binding;
            deserialized->group = var->group;
            deserialized->arraySizeCount = var->arraySizesCount;
            deserialized->arraySizes = Parse<uint32_t>(buf, var->arraySizesOffset);

            deserialized->structureOffset = var->structureOffset;
            deserialized->byteSize = var->byteSize;
            

            deserialized->annotationCount = var->annotationsCount;
            deserialized->annotations = new AnyFX::Deserialize::Annotation[deserialized->annotationCount];
            LoadAnnotations(deserialized->annotationCount, deserialized->annotations, var->annotationsOffset, buf);

            this->nameToObject[deserialized->name] = deserialized;
            break;
        }
        case AnyFX::Serialize::StructureType:
        {
            const AnyFX::Serialize::Structure* struc = ParseAndConsume<AnyFX::Serialize::Structure>(data, frontIterator);
            AnyFX::Deserialize::Structure* deserialized = new AnyFX::Deserialize::Structure;

            deserialized->name = Parse<const char>(buf, struc->nameOffset);
            deserialized->nameLength = struc->nameLength;
            deserialized->variableCount = struc->variablesCount;
            deserialized->variables = new AnyFX::Deserialize::Variable[deserialized->variableCount];
            size_t variableIterator = struc->variablesOffset;

            for (int i = 0; i < struc->variablesCount; i++)
            {
                const AnyFX::Serialize::Variable* var = ParseAndConsume<AnyFX::Serialize::Variable>(buf, variableIterator);
                AnyFX::Deserialize::Variable& deserializedVar = deserialized->variables[i];

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
            deserialized->annotations = new AnyFX::Deserialize::Annotation[deserialized->annotationCount];
            LoadAnnotations(deserialized->annotationCount, deserialized->annotations, struc->annotationsOffset, buf);

            this->nameToObject[deserialized->name] = deserialized;
            break;
        }
        case AnyFX::Serialize::ProgramType:
        {
            const AnyFX::Serialize::Program* prog = ParseAndConsume<AnyFX::Serialize::Program>(data, frontIterator);
            AnyFX::Deserialize::Program* deserialized = new AnyFX::Deserialize::Program;

            deserialized->name = Parse<const char>(buf, prog->nameOffset);
            deserialized->nameLength = prog->nameLength;

            // load shaders
            deserialized->vs.binary = Parse<const char>(buf, prog->vs.binaryOffset);
            deserialized->vs.binaryLength = prog->vs.binaryLength;
            deserialized->hs.binary = Parse<const char>(buf, prog->hs.binaryOffset);
            deserialized->hs.binaryLength = prog->hs.binaryLength;
            deserialized->ds.binary = Parse<const char>(buf, prog->ds.binaryOffset);
            deserialized->ds.binaryLength = prog->ds.binaryLength;
            deserialized->gs.binary = Parse<const char>(buf, prog->gs.binaryOffset);
            deserialized->gs.binaryLength = prog->gs.binaryLength;
            deserialized->ps.binary = Parse<const char>(buf, prog->ps.binaryOffset);
            deserialized->ps.binaryLength = prog->ps.binaryLength;
            deserialized->cs.binary = Parse<const char>(buf, prog->cs.binaryOffset);
            deserialized->cs.binaryLength = prog->cs.binaryLength;
            deserialized->rgs.binary = Parse<const char>(buf, prog->rgs.binaryOffset);
            deserialized->rgs.binaryLength = prog->rgs.binaryLength;
            deserialized->rms.binary = Parse<const char>(buf, prog->rms.binaryOffset);
            deserialized->rms.binaryLength = prog->rhs.binaryLength;
            deserialized->rchs.binary = Parse<const char>(buf, prog->rchs.binaryOffset);
            deserialized->rchs.binaryLength = prog->rchs.binaryLength;
            deserialized->ris.binary = Parse<const char>(buf, prog->ris.binaryOffset);
            deserialized->ris.binaryLength = prog->ris.binaryLength;
            deserialized->rahs.binary = Parse<const char>(buf, prog->rahs.binaryOffset);
            deserialized->rahs.binaryLength = prog->rahs.binaryLength;

            deserialized->annotationCount = prog->annotationsCount;
            deserialized->annotations = new AnyFX::Deserialize::Annotation[deserialized->annotationCount];
            LoadAnnotations(deserialized->annotationCount, deserialized->annotations, prog->annotationsOffset, buf);

            this->nameToObject[deserialized->name] = deserialized;
            break;
        }
        case AnyFX::Serialize::RenderStateType:
        {
            const AnyFX::Serialize::RenderState* rend = ParseAndConsume<AnyFX::Serialize::RenderState>(data, frontIterator);
            AnyFX::Deserialize::RenderState* deserialized = new AnyFX::Deserialize::RenderState;

            this->nameToObject[deserialized->name] = deserialized;
            break;
        }
        }
    }
}

} // namespace AnyFX
