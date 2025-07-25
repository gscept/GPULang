//------------------------------------------------------------------------------
//  @file glslgenerator.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "glslgenerator.h"
#include "ast/symbol.h"
#include "ast/function.h"
#include "ast/structure.h"
#include "ast/variable.h"
#include "ast/program.h"
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

#include "compiler.h"
#include "containers.h"
#include <algorithm>

#include "glslang/Include/ResourceLimits.h"
#include "glslang/Public/ShaderLang.h"
#define ENABLE_OPT 1
#include "ast/statements/terminatestatement.h"
#include "SPIRV/GlslangToSpv.h"
#include "SPIRV/SpvTools.h"
#include "spirv-tools/optimizer.hpp"

TBuiltInResource DefaultResources;

void SetupDefaultResources()
{
    DefaultResources.maxLights = 32;
    DefaultResources.maxClipPlanes = 6;
    DefaultResources.maxTextureUnits = 32;
    DefaultResources.maxTextureCoords = 32;
    DefaultResources.maxVertexAttribs = 64;
    DefaultResources.maxVertexUniformComponents = 4096;
    DefaultResources.maxVaryingFloats = 64;
    DefaultResources.maxVertexTextureImageUnits = 32;
    DefaultResources.maxCombinedTextureImageUnits = 80;
    DefaultResources.maxTextureImageUnits = 32;
    DefaultResources.maxFragmentUniformComponents = 4096;
    DefaultResources.maxDrawBuffers = 32;
    DefaultResources.maxVertexUniformVectors = 128;
    DefaultResources.maxVaryingVectors = 8;
    DefaultResources.maxFragmentUniformVectors = 16;
    DefaultResources.maxVertexOutputVectors = 16;
    DefaultResources.maxFragmentInputVectors = 15;
    DefaultResources.minProgramTexelOffset = -8;
    DefaultResources.maxProgramTexelOffset = 7;
    DefaultResources.maxClipDistances = 8;
    DefaultResources.maxComputeWorkGroupCountX = 65535;
    DefaultResources.maxComputeWorkGroupCountY = 65535;
    DefaultResources.maxComputeWorkGroupCountZ = 65535;
    DefaultResources.maxComputeWorkGroupSizeX = 1024;
    DefaultResources.maxComputeWorkGroupSizeY = 1024;
    DefaultResources.maxComputeWorkGroupSizeZ = 64;
    DefaultResources.maxComputeUniformComponents = 1024;
    DefaultResources.maxComputeTextureImageUnits = 16;
    DefaultResources.maxComputeImageUniforms = 8;
    DefaultResources.maxComputeAtomicCounters = 8;
    DefaultResources.maxComputeAtomicCounterBuffers = 1;
    DefaultResources.maxVaryingComponents = 60;
    DefaultResources.maxVertexOutputComponents = 64;
    DefaultResources.maxGeometryInputComponents = 64;
    DefaultResources.maxGeometryOutputComponents = 128;
    DefaultResources.maxFragmentInputComponents = 128;
    DefaultResources.maxImageUnits = 8;
    DefaultResources.maxCombinedImageUnitsAndFragmentOutputs = 8;
    DefaultResources.maxCombinedShaderOutputResources = 8;
    DefaultResources.maxImageSamples = 0;
    DefaultResources.maxVertexImageUniforms = 0;
    DefaultResources.maxTessControlImageUniforms = 0;
    DefaultResources.maxTessEvaluationImageUniforms = 0;
    DefaultResources.maxGeometryImageUniforms = 0;
    DefaultResources.maxFragmentImageUniforms = 8;
    DefaultResources.maxCombinedImageUniforms = 8;
    DefaultResources.maxGeometryTextureImageUnits = 16;
    DefaultResources.maxGeometryOutputVertices = 256;
    DefaultResources.maxGeometryTotalOutputComponents = 1024;
    DefaultResources.maxGeometryUniformComponents = 1024;
    DefaultResources.maxGeometryVaryingComponents = 64;
    DefaultResources.maxTessControlInputComponents = 128;
    DefaultResources.maxTessControlOutputComponents = 128;
    DefaultResources.maxTessControlTextureImageUnits = 16;
    DefaultResources.maxTessControlUniformComponents = 1024;
    DefaultResources.maxTessControlTotalOutputComponents = 4096;
    DefaultResources.maxTessEvaluationInputComponents = 128;
    DefaultResources.maxTessEvaluationOutputComponents = 128;
    DefaultResources.maxTessEvaluationTextureImageUnits = 16;
    DefaultResources.maxTessEvaluationUniformComponents = 1024;
    DefaultResources.maxTessPatchComponents = 120;
    DefaultResources.maxPatchVertices = 32;
    DefaultResources.maxTessGenLevel = 64;
    DefaultResources.maxViewports = 16;
    DefaultResources.maxVertexAtomicCounters = 0;
    DefaultResources.maxTessControlAtomicCounters = 0;
    DefaultResources.maxTessEvaluationAtomicCounters = 0;
    DefaultResources.maxGeometryAtomicCounters = 0;
    DefaultResources.maxFragmentAtomicCounters = 8;
    DefaultResources.maxCombinedAtomicCounters = 8;
    DefaultResources.maxAtomicCounterBindings = 1;
    DefaultResources.maxVertexAtomicCounterBuffers = 0;
    DefaultResources.maxTessControlAtomicCounterBuffers = 0;
    DefaultResources.maxTessEvaluationAtomicCounterBuffers = 0;
    DefaultResources.maxGeometryAtomicCounterBuffers = 0;
    DefaultResources.maxFragmentAtomicCounterBuffers = 1;
    DefaultResources.maxCombinedAtomicCounterBuffers = 1;
    DefaultResources.maxAtomicCounterBufferSize = 16384;
    DefaultResources.maxTransformFeedbackBuffers = 4;
    DefaultResources.maxTransformFeedbackInterleavedComponents = 64;
    DefaultResources.maxCullDistances = 8;
    DefaultResources.maxCombinedClipAndCullDistances = 8;
    DefaultResources.maxSamples = 4;

    DefaultResources.limits.nonInductiveForLoops = 1;
    DefaultResources.limits.whileLoops = 1;
    DefaultResources.limits.doWhileLoops = 1;
    DefaultResources.limits.generalUniformIndexing = 1;
    DefaultResources.limits.generalAttributeMatrixVectorIndexing = 1;
    DefaultResources.limits.generalVaryingIndexing = 1;
    DefaultResources.limits.generalSamplerIndexing = 1;
    DefaultResources.limits.generalVariableIndexing = 1;
    DefaultResources.limits.generalConstantMatrixVectorIndexing = 1;
}


namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
GLSLGenerator::GLSLGenerator(LanguageFeatureSet featureSet)
    : featureSet(featureSet)
{
    glslang::InitializeProcess();
}


//------------------------------------------------------------------------------
/**
*/
GLSLGenerator::~GLSLGenerator()
{
    glslang::FinalizeProcess();
}

//------------------------------------------------------------------------------
/**
*/
bool
GLSLGenerator::Generate(const Compiler* compiler, const ProgramInstance* program, const PinnedArray<Symbol*>& symbols, std::function<void(const std::string&, const std::string&)> writerFunc)
{
    SetupDefaultResources();

    static const std::unordered_map<ProgramInstance::__Resolved::EntryType, EShLanguage> entryToGlslangShaderMappings =
    {
        { ProgramInstance::__Resolved::VertexShader, EShLangVertex },
        { ProgramInstance::__Resolved::HullShader, EShLangTessControl },
        { ProgramInstance::__Resolved::DomainShader, EShLangTessEvaluation },
        { ProgramInstance::__Resolved::GeometryShader, EShLangGeometry },
        { ProgramInstance::__Resolved::PixelShader, EShLangFragment },
        { ProgramInstance::__Resolved::ComputeShader, EShLangCompute },
        { ProgramInstance::__Resolved::TaskShader, EShLangTaskNV },
        { ProgramInstance::__Resolved::MeshShader, EShLangMeshNV },
        { ProgramInstance::__Resolved::RayGenerationShader, EShLangRayGen },
        { ProgramInstance::__Resolved::RayMissShader, EShLangMiss },
        { ProgramInstance::__Resolved::RayClosestHitShader, EShLangClosestHit },
        { ProgramInstance::__Resolved::RayAnyHitShader, EShLangAnyHit },
        { ProgramInstance::__Resolved::RayIntersectionShader, EShLangIntersect },
        { ProgramInstance::__Resolved::RayCallableShader, EShLangCallable },
    };

    ProgramInstance::__Resolved* progResolved = static_cast<ProgramInstance::__Resolved*>(program->resolved);
    std::vector<glslang::TShader*> shaders;
    for (uint32_t mapping = 0; mapping < ProgramInstance::__Resolved::EntryType::NumProgramEntries; mapping++)
    {
        Symbol* object = progResolved->mappings[mapping];
        if (object == nullptr)
            continue;

        // for each shader, generate code and use it as a binary output
        if (mapping >= ProgramInstance::__Resolved::VertexShader && mapping <= ProgramInstance::__Resolved::RayIntersectionShader)
        {
            std::string code;
            this->mainFunction = static_cast<Function*>(object);
            for (Symbol* symbol : symbols)
            {
                switch (symbol->symbolType)
                {
                case Symbol::SymbolType::FunctionType:
                    this->GenerateFunctionSPIRV(compiler, program, symbol, code);
                    break;
                case Symbol::SymbolType::StructureType:
                    this->GenerateStructureSPIRV(compiler, program, symbol, code);
                    break;
                case Symbol::SymbolType::VariableType:
                    this->GenerateVariableSPIRV(compiler, program, symbol, code, false);
                    break;
                }
            }

            if (!map_contains(entryToGlslangShaderMappings, (ProgramInstance::__Resolved::EntryType)mapping))
            {
                this->Error(Format("Internal error, no known mapping of shader '%s'", ProgramInstance::__Resolved::EntryTypeToString((ProgramInstance::__Resolved::EntryType)mapping).c_str()));
                return false;
            }

            EShMessages messages = (EShMessages)(EShMsgDefault | EShMsgRelaxedErrors | EShMsgSpvRules | (this->featureSet == VulkanFeatureSet ? EShMsgVulkanRules : 0x0));

            // add platform specific message output
#if defined(_MSC_VER)
            messages = (EShMessages)(messages | EShMsgMSVCFormat);
#elif defined(__GNUC__) || defined(__clang__)
            messages = (EShMessages)(messages | EShMsgClangGCCFormat);
#endif

            std::string header;
            if (this->featureSet == OpenGLFeatureSet)
            {
                header = "\
#version 460 core\n\
#define gl_InstanceID gl_InstanceIndex\n\
#define gl_VertexID gl_VertexIndex\n\
#extension GL_GOOGLE_cpp_style_line_directive : enable\n\
#extension GL_ARB_separate_shader_objects : enable\n\
#extension GL_ARB_shading_language_420pack : enable\n\
#extension GL_KHR_shader_subgroup_quad : enable\n\
#define __OPENGL__\n";
            }
            else if (this->featureSet == VulkanFeatureSet)
            {
                header = "\
#version 460 core\n\
#define gl_InstanceID gl_InstanceIndex\n\
#define gl_VertexID gl_VertexIndex\n\
#extension GL_GOOGLE_cpp_style_line_directive : enable\n\
#extension GL_ARB_separate_shader_objects : enable\n\
#extension GL_ARB_shading_language_420pack : enable\n\
#extension GL_KHR_shader_subgroup_quad : enable\n\
#define __VULKAN__\n";
            }

            glslang::TShader* shaderObject = new glslang::TShader(entryToGlslangShaderMappings.at((ProgramInstance::__Resolved::EntryType)mapping));
            const char* sources[] = { header.c_str(), code.c_str() };
            int lengths[] = { (int)header.length(), (int)code.length() };
            shaderObject->setStringsWithLengths(sources, lengths, 2);

            // perform compilation
            if (this->featureSet == OpenGLFeatureSet)
            {
                shaderObject->setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_0);
                shaderObject->setEnvClient(glslang::EShClientOpenGL, glslang::EShTargetOpenGL_450);
            }
            else
            {
                shaderObject->setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_3);
                shaderObject->setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_2);
            }

            if (!shaderObject->parse(&DefaultResources, 460, EProfile::ENoProfile, false, true, messages))
            {
                std::string message = shaderObject->getInfoLog();
                this->Error(message);
                delete shaderObject;
                return false;
            }

            // run writer function
            if (writerFunc)
            {
                auto str = TransientString(program->name, "_", this->mainFunction->name);
                auto src = TransientString(sources[0], sources[1]);
                writerFunc(str.c_str(), src.c_str());
            }

            // add shaders to list
            shaders.push_back(shaderObject);
        }
    }

    // link program
    glslang::TProgram* programObject = new glslang::TProgram();
    for (glslang::TShader* shader : shaders)
    {
        programObject->addShader(shader);
    }

    EShMessages messages = EShMsgDefault;
    if (!programObject->link(messages))
    {
        std::string message = programObject->getInfoLog();
        this->Error(message);
        return false;
    }

    // build reflection to get uniform stuff
    bool refbuilt = programObject->buildReflection();
    assert(refbuilt);

    // run optimizations
    spvtools::Optimizer* optimizer = nullptr;
    if (this->featureSet == OpenGLFeatureSet)
        optimizer = new spvtools::Optimizer(SPV_ENV_OPENGL_4_5);
    else if (this->featureSet == VulkanFeatureSet)
        optimizer = new spvtools::Optimizer(SPV_ENV_VULKAN_1_2);
    
    optimizer->RegisterPerformancePasses();


    for (ProgramInstance::__Resolved::EntryType i = ProgramInstance::__Resolved::VertexShader; i < ProgramInstance::__Resolved::RayIntersectionShader; i = (ProgramInstance::__Resolved::EntryType)((int)i + 1))
    {
        glslang::TIntermediate* intermediate = programObject->getIntermediate(entryToGlslangShaderMappings.at(i));
        if (intermediate != NULL)
        {
            glslang::SpvOptions options;
            options.disableOptimizer = false;
            options.optimizeSize = true;

            std::vector<uint32_t> binary;
            glslang::GlslangToSpv(*intermediate, binary);

            std::vector<uint32_t> optimized;
            optimizer->Run(binary.data(), binary.size(), &optimized);

            // copy binary to program
            progResolved->binaries[i] = optimized;
        }
    }

    // cleanup
    delete optimizer;
    delete programObject;

    for (glslang::TShader* shader : shaders)
    {
        delete shader;
    }
    return true;
}

struct TypeDimensions
{
    unsigned int x, y;
};

StaticMap typeToDimensions =
std::array{
    std::pair{ ConstantString("f32"),      TypeDimensions{ 1, 1 } },
    std::pair{ ConstantString("f32x2"),     TypeDimensions{ 2, 1 } },
    std::pair{ ConstantString("f32x3"),     TypeDimensions{ 3, 1 } },
    std::pair{ ConstantString("f32x4"),     TypeDimensions{ 4, 1 } },
    std::pair{ ConstantString("i32"),        TypeDimensions{ 1, 1 } },
    std::pair{ ConstantString("i32x2"),      TypeDimensions{ 2, 1 } },
    std::pair{ ConstantString("i32x3"),      TypeDimensions{ 3, 1 } },
    std::pair{ ConstantString("i32x4"),      TypeDimensions{ 4, 1 } },
    std::pair{ ConstantString("u32"),       TypeDimensions{ 1, 1 } },
    std::pair{ ConstantString("u32x2"),      TypeDimensions{ 2, 1 } },
    std::pair{ ConstantString("u32x3"),      TypeDimensions{ 3, 1 } },
    std::pair{ ConstantString("u32x4"),      TypeDimensions{ 4, 1 } },
    std::pair{ ConstantString("b8"),       TypeDimensions{ 1, 1 } },
    std::pair{ ConstantString("b8x2"),      TypeDimensions{ 2, 1 } },
    std::pair{ ConstantString("b8x3"),      TypeDimensions{ 3, 1 } },
    std::pair{ ConstantString("b8x4"),      TypeDimensions{ 4, 1 } },
    std::pair{ ConstantString("f32x2x2"),   TypeDimensions{ 2, 2 } },
    std::pair{ ConstantString("f32x2x3"),   TypeDimensions{ 2, 3 } },
    std::pair{ ConstantString("f32x2x4"),   TypeDimensions{ 2, 4 } },
    std::pair{ ConstantString("f32x3x2"),   TypeDimensions{ 3, 2 } },
    std::pair{ ConstantString("f32x3x3"),   TypeDimensions{ 3, 3 } },
    std::pair{ ConstantString("f32x3x4"),   TypeDimensions{ 3, 4 } },
    std::pair{ ConstantString("f32x4x2"),   TypeDimensions{ 4, 2 } },
    std::pair{ ConstantString("f32x4x3"),   TypeDimensions{ 4, 3 } },
    std::pair{ ConstantString("f32x4x4"),   TypeDimensions{ 4, 4 } }
};

// type being the underlying base type (float2x2 -> float)
StaticMap typeToByteSize =
std::array{
    std::pair{ ConstantString("f32"),      4 },
    std::pair{ ConstantString("f32x2"),     4 },
    std::pair{ ConstantString("f32x3"),     4 },
    std::pair{ ConstantString("f32x4"),     4 },
    std::pair{ ConstantString("i32"),        4 },
    std::pair{ ConstantString("i32x2"),       4 },
    std::pair{ ConstantString("i32x3"),       4 },
    std::pair{ ConstantString("i32x4"),       4 },
    std::pair{ ConstantString("u32"),       4 },
    std::pair{ ConstantString("u32x2"),      4 },
    std::pair{ ConstantString("u32x3"),      4 },
    std::pair{ ConstantString("u32x4"),      4 },
    std::pair{ ConstantString("b8"),       1 },
    std::pair{ ConstantString("b8x2"),      1 },
    std::pair{ ConstantString("b8x3"),      1 },
    std::pair{ ConstantString("b8x4"),      1 },
    std::pair{ ConstantString("f32x2x2"),   4 },
    std::pair{ ConstantString("f32x2x3"),   4 },
    std::pair{ ConstantString("f32x2x4"),   4 },
    std::pair{ ConstantString("f32x3x2"),   4 },
    std::pair{ ConstantString("f32x3x3"),   4 },
    std::pair{ ConstantString("f32x3x4"),   4 },
    std::pair{ ConstantString("f32x4x2"),   4 },
    std::pair{ ConstantString("f32x4x3"),   4 },
    std::pair{ ConstantString("f32x4x4"),   4 },
};

//------------------------------------------------------------------------------
/**
*/
uint32_t
RoundToPow2(uint32_t value, uint32_t power)
{
    return value = (value + power - 1) & ~(power - 1);
}

//------------------------------------------------------------------------------
/**
    Calculate the alignment and byte size for a type
*/
void
CalculateLayout(const Compiler* compiler, Variable* var, uint32_t arraySize, const bool std140, uint32_t& size, uint32_t& alignment, uint32_t& arrayElementPadding)
{
    Variable::__Resolved* varResolved = static_cast<Variable::__Resolved*>(var->resolved);
    TypeDimensions dimensions = typeToDimensions.Find(var->type.name)->second;
    uint32_t byteSize = typeToByteSize.Find(var->type.name)->second;
    const uint32_t vec4alignment = 16;
    arrayElementPadding = 0;

    /*
    1.  If the member is a scalar consuming N basic machine units, the base alignment is N.
    2.  If the member is a two- or four-component vector with components consuming N basic machine units, the base alignment is 2N or 4N, respectively.
    3.  If the member is a three-component vector with components consuming N basic machine units, the base alignment is 4N.
    4.  If the member is an array of scalars or vectors, the base alignment and array stride are set to match the base alignment of a single array element,
        according to rules (1), (2), and (3), and rounded up to the base alignment of a vec4. The array may have padding at the end;
        the base offset of the member following the array is rounded up to the next multiple of the base alignment.
    5.  If the member is a column-major matrix with C columns and R rows, the matrix is stored identically to an array of C column vectors with R
        components each, according to rule (4).
    6.  If the member is an array of S column-major matrices with C columns and R rows, the matrix is stored identically to a row of
        S�C column vectors with R components each, according to rule (4).
    7.  If the member is a row-major matrix with C columns and R rows, the matrix is stored identically to an array of R row vectors with C components each,
        according to rule (4).
    8.  If the member is an array of S row-major matrices with C columns and R rows, the matrix is stored identically to a row of S�R row vectors with C
        components each, according to rule (4).
    9.  If the member is a structure, the base alignment of the structure is N, where N is the largest base alignment value of any of its members,
        and rounded up to the base alignment of a vec4. The individual members of this sub-structure are then assigned offsets by applying this set of
        rules recursively, where the base offset of the first member of the sub-structure is equal to the aligned offset of the structure.
        The structure may have padding at the end; the base offset of the member following the sub-structure is
        rounded up to the next multiple of the base alignment of the structure.
    10. If the member is an array of S structures, the S elements of the array are laid out in order, according to rule
    For std430, rounding up to vec4 in rule 4 and 9 is not relevant
    */

    switch (dimensions.x)
    {
    case 0:
        break;
    case 1:
        alignment = byteSize;
        size = byteSize;
        break;
    case 2:
        alignment = byteSize * 2;
        size = byteSize * 2;
        break;
    case 3:
        alignment = byteSize * 4;
        size = byteSize * 3;
        break;
    case 4:
        alignment = byteSize * 4;
        size = byteSize * 4;
        break;
    default:
        // should never happen
        alignment = byteSize;
        size = byteSize;
        break;
    }

    if (arraySize > 1)
    {
        // array case
        CalculateLayout(compiler, var, 1, std140, size, alignment, arrayElementPadding);
        uint32_t baseAlignment = alignment;
        if (std140)
            alignment = max(vec4alignment, alignment);

        // element padding is the difference between size and alignment
        if (std140)
            arrayElementPadding = size % alignment;

        // the final size is the size per element + element padding * number of elements
        size = (size + arrayElementPadding) * arraySize;
    }
    else if (varResolved->typeSymbol->symbolType == Symbol::StructureType)
    {
        // struct case
        Structure* struc = static_cast<Structure*>(varResolved->typeSymbol);
        Structure::__Resolved* strucResolved = static_cast<Structure::__Resolved*>(struc->resolved);
        alignment = strucResolved->baseAlignment;
        size = strucResolved->byteSize;
    }
    else if (dimensions.y > 1)
    {
        // matrix case
        if (std140)
            alignment = max(vec4alignment, alignment);
        size = RoundToPow2(size, alignment);
        size *= dimensions.y;
    }
}

StaticMap typeToGlslType =
std::array{
    std::pair{ ConstantString("readWriteTexture1D"), ConstantString("image1D") },
    std::pair{ ConstantString("readWriteTexture2D"), ConstantString("image2D") },
    std::pair{ ConstantString("readWriteTexture2DMS"), ConstantString("image2DMS") },
    std::pair{ ConstantString("readWriteTextureCube"), ConstantString("imageCube") },
    std::pair{ ConstantString("readWriteTexture3D"), ConstantString("image3D") },
    std::pair{ ConstantString("readWriteTexture1DArray"), ConstantString("image1DArray") },
    std::pair{ ConstantString("readWriteTexture2DArray"), ConstantString("image2DArray") },
    std::pair{ ConstantString("readWriteTexture2DMSArray"), ConstantString("image2DMSArray") },
    std::pair{ ConstantString("readWriteTextureCubeArray"), ConstantString("imageCubeArray") },
    std::pair{ ConstantString("texture1D"), ConstantString("texture1D") },
    std::pair{ ConstantString("texture2D"), ConstantString("texture2D") },
    std::pair{ ConstantString("texture2DMS"), ConstantString("texture2DMS") },
    std::pair{ ConstantString("textureCube"), ConstantString("textureCube") },
    std::pair{ ConstantString("texture3D"), ConstantString("texture3D") },
    std::pair{ ConstantString("texture1DArray"), ConstantString("texture1DArray") },
    std::pair{ ConstantString("texture2DArray"), ConstantString("texture2DArray") },
    std::pair{ ConstantString("texture2DMSArray"), ConstantString("texture2DMSArray") },
    std::pair{ ConstantString("textureCubeArray"), ConstantString("textureCubeArray") },
    std::pair{ ConstantString("sampledTexture1D"), ConstantString("sampler1D") },
    std::pair{ ConstantString("sampledTexture2D"), ConstantString("sampler2D") },
    std::pair{ ConstantString("sampledTexture2DMS"), ConstantString("sampler2DMS") },
    std::pair{ ConstantString("sampledTextureCube"), ConstantString("samplerCube") },
    std::pair{ ConstantString("sampledTexture3D"), ConstantString("sampler3D") },
    std::pair{ ConstantString("sampledTexture1DArray"), ConstantString("sampler1DArray") },
    std::pair{ ConstantString("sampledTexture2DArray"), ConstantString("sampler2DArray") },
    std::pair{ ConstantString("sampledTexture2DMSArray"), ConstantString("sampler2DMSArray") },
    std::pair{ ConstantString("sampledTextureCubeArray"), ConstantString("samplerCubeArray") },
    std::pair{ ConstantString("inputAttachment"), ConstantString("subpassInput") },
    std::pair{ ConstantString("inputAttachmentMS"), ConstantString("subpassInputMS") },
    std::pair{ ConstantString("f32"), ConstantString("float") },
    std::pair{ ConstantString("f32x2"), ConstantString("vec2") },
    std::pair{ ConstantString("f32x3"), ConstantString("vec3") },
    std::pair{ ConstantString("f32x4"), ConstantString("vec4") },
    std::pair{ ConstantString("i32"), ConstantString("int") },
    std::pair{ ConstantString("i32x2"), ConstantString("ivec2") },
    std::pair{ ConstantString("i32x3"), ConstantString("ivec3") },
    std::pair{ ConstantString("i32x4"), ConstantString("ivec4") },
    std::pair{ ConstantString("u32"), ConstantString("uint") },
    std::pair{ ConstantString("u32x2"), ConstantString("uvec2") },
    std::pair{ ConstantString("u32x3"), ConstantString("uvec3") },
    std::pair{ ConstantString("u32x4"), ConstantString("uvec4") },
    std::pair{ ConstantString("b8"), ConstantString("bool") },
    std::pair{ ConstantString("b8x2"), ConstantString("bvec2") },
    std::pair{ ConstantString("b8x3"), ConstantString("bvec3") },
    std::pair{ ConstantString("b8x4"), ConstantString("bvec4") },
    std::pair{ ConstantString("f32x2x2"), ConstantString("mat2") },
    std::pair{ ConstantString("f32x2x3"), ConstantString("mat2x3") },
    std::pair{ ConstantString("f32x2x4"), ConstantString("mat2x4") },
    std::pair{ ConstantString("f32x3x2"), ConstantString("mat3x2") },
    std::pair{ ConstantString("f32x3x3"), ConstantString("mat3") },
    std::pair{ ConstantString("f32x3x4"), ConstantString("mat3x4") },
    std::pair{ ConstantString("f32x4x2"), ConstantString("mat4x2") },
    std::pair{ ConstantString("f32x4x3"), ConstantString("mat4x3") },
    std::pair{ ConstantString("f32x4x4"), ConstantString("mat4") },
    std::pair{ ConstantString("sampler"), ConstantString("sampler") },
    std::pair{ ConstantString("void"), ConstantString("void") }
};

StaticMap imageFormatToGlsl =
std::array{
      std::pair{ ImageFormat::Rgba16, ConstantString("rgba16") }
    , std::pair{ ImageFormat::Rgb10_A2, ConstantString("rgb10_a2") }
    , std::pair{ ImageFormat::Rgba8, ConstantString("rgba8") }
    , std::pair{ ImageFormat::Rg16, ConstantString("rg16") }
    , std::pair{ ImageFormat::Rg8, ConstantString("rg8") }
    , std::pair{ ImageFormat::R16, ConstantString("r16") }
    , std::pair{ ImageFormat::R8, ConstantString("r8") }
    , std::pair{ ImageFormat::Rgba16_Snorm, ConstantString("rgba16_snorm") }
    , std::pair{ ImageFormat::Rgba8_Snorm, ConstantString("rgba8_snorm") }
    , std::pair{ ImageFormat::Rg16_Snorm, ConstantString("rg16_snorm") }
    , std::pair{ ImageFormat::Rg8_Snorm, ConstantString("rg8_snorm") }
    , std::pair{ ImageFormat::R16_Snorm, ConstantString("r16_snorm") }
    , std::pair{ ImageFormat::R8_Snorm, ConstantString("r8_snorm") }
    , std::pair{ ImageFormat::Rgba32F, ConstantString("rgba32f") }
    , std::pair{ ImageFormat::Rgba16F, ConstantString("rgba16f") }
    , std::pair{ ImageFormat::Rg32F, ConstantString("rg32f") }
    , std::pair{ ImageFormat::Rg16F, ConstantString("rg16f") }
    , std::pair{ ImageFormat::R11G11B10F, ConstantString("r11f_g11f_b10f") }
    , std::pair{ ImageFormat::R32F, ConstantString("r32f") }
    , std::pair{ ImageFormat::R16F, ConstantString("r16f") }
    , std::pair{ ImageFormat::Rgba32I, ConstantString("rga32i") }
    , std::pair{ ImageFormat::Rgba16I, ConstantString("rgba16i") }
    , std::pair{ ImageFormat::Rgba8I, ConstantString("rgba8i") }
    , std::pair{ ImageFormat::Rg32I, ConstantString("rg32i") }
    , std::pair{ ImageFormat::Rg16I, ConstantString("rg16i") }
    , std::pair{ ImageFormat::Rg8I, ConstantString("rg8i") }
    , std::pair{ ImageFormat::R32I, ConstantString("r32i") }
    , std::pair{ ImageFormat::R16I, ConstantString("r16i") }
    , std::pair{ ImageFormat::R8I, ConstantString("r8i") }
    , std::pair{ ImageFormat::Rgba32U, ConstantString("rga32ui") }
    , std::pair{ ImageFormat::Rgba16U, ConstantString("rgba16ui") }
    , std::pair{ ImageFormat::Rgb10_A2U, ConstantString("rgb10_a2ui") }
    , std::pair{ ImageFormat::Rgba8U, ConstantString("rgba8ui") }
    , std::pair{ ImageFormat::Rg32U, ConstantString("rg32ui") }
    , std::pair{ ImageFormat::Rg16U, ConstantString("rg16ui") }
    , std::pair{ ImageFormat::Rg8U, ConstantString("rg8ui") }
    , std::pair{ ImageFormat::R32U, ConstantString("r32ui") }
    , std::pair{ ImageFormat::R16U, ConstantString("r16ui") }
    , std::pair{ ImageFormat::R8U, ConstantString("r8ui") }
};

void GenerateExpressionGLSL(const Compiler* compiler, Expression* expr, std::string& outCode);

//------------------------------------------------------------------------------
/**
*/
void
GenerateCallExpressionGLSL(const Compiler* compiler, Expression* expr, std::string& outCode)
{
    CallExpression* callExpression = static_cast<CallExpression*>(expr);
    FixedString fun;
    callExpression->EvalSymbol(fun);
    auto it = typeToGlslType.Find(fun);
    if (it != typeToGlslType.end())
    {
        fun = it->second;
    }

    std::string args;
    for (Expression* arg : callExpression->args)
    {
        std::string str;
        GenerateExpressionGLSL(compiler, arg, str);
        args.append(str);
        if (arg != callExpression->args.back())
            args.append(", ");
    }
    outCode.append(Format("%s(%s)", fun.c_str(), args.c_str()));
}

//------------------------------------------------------------------------------
/**
*/
void
GenerateInitializerExpressionGLSL(const Compiler* compiler, Expression* expr, std::string& outCode)
{
    std::string inner;
    InitializerExpression* initExpression = static_cast<InitializerExpression*>(expr);
    for (Expression* expr : initExpression->values)
    {
        if (expr->symbolType == Symbol::InitializerExpressionType)
            GenerateInitializerExpressionGLSL(compiler, expr, inner);  
        else
        {
            Type::FullType type;
            expr->EvalType(type);

            GenerateExpressionGLSL(compiler, expr, inner);
        }

        if (expr != initExpression->values.back())
            inner.append(",");
    }
    outCode.append(Format("{ %s }", inner.c_str()));
}

//------------------------------------------------------------------------------
/**
*/
void 
GenerateAccessExpressionGLSL(const Compiler* compiler, Expression* expr, std::string& outCode)
{
    AccessExpression* accessExpression = static_cast<AccessExpression*>(expr);
    std::string lhs, rhs;
    GenerateExpressionGLSL(compiler, accessExpression->left, lhs);
    GenerateExpressionGLSL(compiler, accessExpression->right, rhs);

    // so the reason why we format this specially for GLSL is because it doesn't support pointers
    outCode = Format("%s.%s", lhs.c_str(), rhs.c_str());
}

//------------------------------------------------------------------------------
/**
*/
void
GenerateCommaExpressionGLSL(const Compiler* compiler, Expression* expr, std::string& outCode)
{
    CommaExpression* commaExpression = static_cast<CommaExpression*>(expr);
    std::string lhs, rhs;
    GenerateExpressionGLSL(compiler, commaExpression->left, lhs);
    GenerateExpressionGLSL(compiler, commaExpression->right, rhs);

    // so the reason why we format this specially for GLSL is because it doesn't support pointers
    outCode = Format("%s, %s", lhs.c_str(), rhs.c_str());
}


//------------------------------------------------------------------------------
/**
*/
void
GenerateExpressionGLSL(const Compiler* compiler, Expression* expr, std::string& outCode)
{
    switch (expr->symbolType)
    {
        // call expression might have to change constructor types
        case Symbol::CallExpressionType:
            GenerateCallExpressionGLSL(compiler, expr, outCode);
            break;
        case Symbol::InitializerExpressionType:
            GenerateInitializerExpressionGLSL(compiler, expr, outCode);
            break;
        case Symbol::AccessExpressionType:
            GenerateAccessExpressionGLSL(compiler, expr, outCode);
            break;
        case Symbol::CommaExpressionType:
            GenerateCommaExpressionGLSL(compiler, expr, outCode);
            break;
        default:
            outCode.append(expr->EvalString().c_str());
            break;
    }
}

void GenerateStatementGLSL(const Compiler* compiler, Statement* statement, std::string& outCode);
uint32_t IndentationLevel = 0;

//------------------------------------------------------------------------------
/**
*/
std::string
GenerateIndentation()
{
    std::string ret;
    ret.resize(IndentationLevel, '\t');
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
void
GenerateVariableGLSL(const Compiler* compiler, Variable* var, std::string& outCode, bool outputType = true)
{
    Variable::__Resolved* varResolved = static_cast<Variable::__Resolved*>(var->resolved);
    std::string indentation = GenerateIndentation();
    FixedString type = "";
    if (outputType)
    {
        if (varResolved->typeSymbol->symbolType == Symbol::StructureType)
            type = var->type.name;
        else
            type = typeToGlslType.Find(var->type.name)->second;
    }

    std::string arrays = "";
    for (int i = var->type.modifierValues.size() - 1; i >= 0; i--)
    {
        uint32_t size = 0;
        if (var->type.modifierValues[i] != nullptr)
        {
            ValueUnion val;
            var->type.modifierValues[i]->EvalValue(val);
            val.Store(size);
        }
        if (size > 0)
            arrays.append(Format("[%d]", size));
        else
            arrays.append("[]");
    }
    if (var->valueExpression != nullptr)
    {
        std::string value;
        GenerateExpressionGLSL(compiler, var->valueExpression, value);
        outCode.append(Format("%s%s %s%s = %s", indentation.c_str(), type.c_str(), var->name.c_str(), arrays.c_str(), value.c_str()));
    }
    else
    {
        outCode.append(Format("%s%s %s%s", indentation.c_str(), type.c_str(), var->name.c_str(), arrays.c_str()));
    }
}

//------------------------------------------------------------------------------
/**
*/
void
GenerateForStatementGLSL(const Compiler* compiler, Statement* statement, std::string& outCode)
{
    ForStatement* forStatement = static_cast<ForStatement*>(statement);
    std::string indentation = GenerateIndentation();
    uint32_t oldIndentation = IndentationLevel;
    IndentationLevel = 0;
    std::string declarations;
    for (Variable* declaration : forStatement->declarations)
    {
        Variable::__Resolved* varResolved = static_cast<Variable::__Resolved*>(declaration->resolved);

        GenerateVariableGLSL(compiler, declaration, declarations, declaration == forStatement->declarations.front());
        if (declaration != forStatement->declarations.back())
            declarations.append(",");
    }

    std::string conditions = "";
    if (forStatement->condition != nullptr)
        GenerateExpressionGLSL(compiler, forStatement->condition, conditions);

    std::string endOfLoopStatement = "";
    if (forStatement->loop != nullptr)
        GenerateExpressionGLSL(compiler, forStatement->loop, endOfLoopStatement);

    IndentationLevel = oldIndentation;
    std::string contents = "";
    GenerateStatementGLSL(compiler, forStatement->contents, contents);

    outCode.append(Format("%sfor (%s;%s;%s)\n%s\n", indentation.c_str(), declarations.c_str(), conditions.c_str(), endOfLoopStatement.c_str(), contents.c_str()));
}

//------------------------------------------------------------------------------
/**
*/
void
GenerateIfStatementGLSL(const Compiler* compiler, Statement* statement, std::string& outCode)
{
    IfStatement* ifStatement = static_cast<IfStatement*>(statement);
    std::string indentation = GenerateIndentation();
    std::string condition;
    GenerateExpressionGLSL(compiler, ifStatement->condition, condition);

    std::string ifBody;
    if (ifStatement->ifStatement->symbolType != Symbol::ScopeStatementType)
        IndentationLevel++;
    GenerateStatementGLSL(compiler, ifStatement->ifStatement, ifBody);
    if (ifStatement->ifStatement->symbolType != Symbol::ScopeStatementType)
        IndentationLevel--;

    std::string elseBody = "";
    if (ifStatement->elseStatement)
    {
        elseBody = Format("%selse", indentation.c_str());
        if (ifStatement->elseStatement->symbolType != Symbol::ScopeStatementType)
            IndentationLevel++;
        if (ifStatement->elseStatement->symbolType != Symbol::IfStatementType)
            elseBody.append("\n");
        GenerateStatementGLSL(compiler, ifStatement->elseStatement, elseBody);
        if (ifStatement->elseStatement->symbolType != Symbol::ScopeStatementType)
            IndentationLevel--;
    }

    outCode.append(Format("%sif (%s) \n%s\n%s\n", indentation.c_str(), condition.c_str(), ifBody.c_str(), elseBody.c_str()));
}

//------------------------------------------------------------------------------
/**
*/
void
GenerateTerminateStatementGLSL(const Compiler* compiler, Statement* statement, std::string& outCode)
{
    TerminateStatement* termStatement = static_cast<TerminateStatement*>(statement);
    std::string indentation = GenerateIndentation();
    if (termStatement->type == TerminateStatement::TerminationType::Return)
    {
        if (termStatement->returnValue != nullptr)
        {
            std::string val;
            GenerateExpressionGLSL(compiler, termStatement->returnValue, val);
            outCode.append(Format("%sreturn %s;", indentation.c_str(), val.c_str()));
        }
        else
        {
            outCode.append(Format("%sreturn;", indentation.c_str()));
        }    
    }
}

//------------------------------------------------------------------------------
/**
*/
void
GenerateScopeStatementGLSL(const Compiler* compiler, Statement* statement, std::string& outCode)
{
    ScopeStatement* scope = static_cast<ScopeStatement*>(statement);
    std::string indentation = GenerateIndentation();
    std::string contents;
    IndentationLevel++;
    for (Symbol* content : scope->symbols)
    {
        if (content->symbolType == Symbol::VariableType)
        {
            GenerateVariableGLSL(compiler, static_cast<Variable*>(content), contents);
            contents.append(";\n");
        }
        else
        {
            GenerateStatementGLSL(compiler, static_cast<Statement*>(content), contents);
            contents.append("\n");
        }
        
    }
    IndentationLevel--;
    outCode.append(Format("%s{\n%s%s}\n", indentation.c_str(), contents.c_str(), indentation.c_str()));
}

//------------------------------------------------------------------------------
/**
*/
void
GenerateSwitchStatementGLSL(const Compiler* compiler, Statement* statement, std::string& outCode)
{
    SwitchStatement* switchStatement = static_cast<SwitchStatement*>(statement);
    std::string indentation = GenerateIndentation();

    std::string expr;
    GenerateExpressionGLSL(compiler, switchStatement->switchExpression, expr);
    IndentationLevel++;

    std::string cases;
    for (size_t i = 0; i < switchStatement->caseExpressions.size; i++)
    {
        Expression* caseExpression = switchStatement->caseExpressions[i];
        std::string caseValue;
        GenerateExpressionGLSL(compiler, caseExpression, caseValue);
        std::string caseStatement;
        GenerateStatementGLSL(compiler, switchStatement->caseStatements[i], caseStatement);
        std::string indentation = GenerateIndentation();
        cases.append(Format("%scase %s:\n%s", indentation.c_str(), caseValue.c_str(), caseStatement.c_str()));
    }

    if (switchStatement->defaultStatement != nullptr)
    {
        std::string defaultStatement;
        GenerateStatementGLSL(compiler, switchStatement->defaultStatement, defaultStatement);
        std::string indentation = GenerateIndentation();
        cases.append(Format("%sdefault:\n%s", indentation.c_str(), defaultStatement.c_str()));
    }
    IndentationLevel--;
    outCode.append(Format("%sswitch (%s)\n{\n%s\n}", indentation.c_str(), expr.c_str(), cases.c_str()));
}

//------------------------------------------------------------------------------
/**
*/
void
GenerateWhileStatementGLSL(const Compiler* compiler, Statement* statement, std::string& outCode)
{
    WhileStatement* whileStatement = static_cast<WhileStatement*>(statement);
    std::string indentation = GenerateIndentation();

    std::string cond;
    GenerateExpressionGLSL(compiler, whileStatement->condition, cond);

    std::string body;
    GenerateStatementGLSL(compiler, whileStatement->statement, body);

    if (whileStatement->isDoWhile)
    {
        outCode.append(Format("%sdo \n%s %swhile (%s);\n", indentation.c_str(), body.c_str(), indentation.c_str(), cond.c_str()));
    }
    else
    {
        outCode.append(Format("%swhile (%s)\n%s", indentation.c_str(), cond.c_str(), body.c_str()));
    }
}

//------------------------------------------------------------------------------
/**
*/
void
GenerateStatementGLSL(const Compiler* compiler, Statement* statement, std::string& outCode)
{
    std::string indentation = GenerateIndentation();
    switch (statement->symbolType)
    {
        case Symbol::BreakStatementType:
            outCode.append(Format("%sbreak;", indentation.c_str()));
            break;
        case Symbol::ContinueStatementType:
            outCode.append(Format("%scontinue;", indentation.c_str()));
            break;
        case Symbol::ExpressionStatementType:
        {
            std::string expr;
            for (auto& expr : static_cast<ExpressionStatement*>(statement)->expressions)
                GenerateExpressionGLSL(compiler, expr, outCode);

            outCode.append(Format("%s%s;", indentation.c_str(), expr.c_str()));
            break;
        }
        case Symbol::ForStatementType:
            GenerateForStatementGLSL(compiler, statement, outCode);
            break;
        case Symbol::IfStatementType:
            GenerateIfStatementGLSL(compiler, statement, outCode);
            break;
        case Symbol::TerminateStatementType:
            GenerateTerminateStatementGLSL(compiler, statement, outCode);
            break;
        case Symbol::ScopeStatementType:
            GenerateScopeStatementGLSL(compiler, statement, outCode);
            break;
        case Symbol::SwitchStatementType:
            GenerateSwitchStatementGLSL(compiler, statement, outCode);
            break;
        case Symbol::WhileStatementType:
            GenerateWhileStatementGLSL(compiler, statement, outCode);
            break;
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
GLSLGenerator::GenerateFunctionSPIRV(const Compiler* compiler, const ProgramInstance* program, Symbol* symbol, std::string& outCode)
{
    Function* fun = static_cast<Function*>(symbol);
    Function::__Resolved* funResolved = Symbol::Resolved(fun);

    ProgramInstance::__Resolved* progResolved = Symbol::Resolved(program);


    bool isMain = fun == this->mainFunction;

    // skip shader functions which are not shaders
    if (funResolved->isEntryPoint)
        return;

    // convert typename to glsl
    Type::FullType returnType = fun->returnType;
    if (funResolved->returnTypeSymbol->symbolType != Symbol::StructureType)
        returnType.name = typeToGlslType.Find(returnType.name)->second;

    std::string body;
    if (funResolved->isPrototype)
    {
        auto overrider = progResolved->functionOverrides.Find(fun);
        if (overrider != progResolved->functionOverrides.end())
        {
            GenerateStatementGLSL(compiler, overrider->second->ast, body);
            //body = progResolved->functionOverrides[fun]->body;
        }
        else
        {
            body = "";
            this->Warning(Format("Prototype function '%s' is not bound for program '%s'", fun->name.c_str(), program->name.c_str()));
            return;
        }
    }
    else
    {
        //body = fun->body;
        if (fun->ast != nullptr)
            GenerateStatementGLSL(compiler, fun->ast, body);
    }

    std::string arguments;
    for (Variable* param : fun->parameters)
    {
        this->GenerateVariableSPIRV(compiler, program, param, arguments, isMain);

        if (!isMain)
        {
            if (param != fun->parameters.back())
                arguments.append(", ");
        }
        else
        {
            arguments.append(";\n");
        }
    }
    TransientString name = isMain ? "main" : fun->name;

    
    if (isMain)
    {
        outCode.append(Format("%s\n", arguments.c_str()));
        outCode.append(Format("#line %d %s\n", fun->location.line, fun->location.file.c_str()));
        outCode.append(Format("void %s()\
\n%s\n\n",
        name.c_str(), body.c_str()));
    }
    else
    {
        outCode.append(Format("#line %d %s\n", fun->location.line, fun->location.file.c_str()));
        if (fun->ast != nullptr)
        {
            outCode.append(Format("%s %s(%s)\
\n%s\n\n",
            returnType.ToString().c_str(), name.c_str(), arguments.c_str(), body.c_str()));
        }
        else
        {
            outCode.append(Format("%s %s(%s);\n", returnType.ToString().c_str(), name.c_str(), arguments.c_str()));
        }
    }
}

enum StructureAlignment
{
    STD140,
    STD430
};

//------------------------------------------------------------------------------
/**
*/
void
GenerateAlignedVariables(const Compiler* compiler, Structure* struc, StructureAlignment structureAlignment, uint32_t& outSize, uint32_t& outAlignment, std::string& outCode)
{
    std::string members;
    uint32_t maxAlignment = structureAlignment == StructureAlignment::STD140 ? 16 : 0;
    uint32_t offset = 0;

    for (Symbol* sym : struc->symbols)
    {
        if (sym->symbolType == Symbol::SymbolType::VariableType)
        {
            Variable* var = static_cast<Variable*>(sym);
            Variable::__Resolved* varResolved = static_cast<Variable::__Resolved*>(var->resolved);

            uint32_t totalArraySize = 1;
            for (Expression* sizeExpressions : var->type.modifierValues)
            {
                uint32_t size = 1;
                if (sizeExpressions != nullptr)
                {
                    ValueUnion val;
                    sizeExpressions->EvalValue(val);
                    val.Store(size);
                }
                totalArraySize *= size;
            }

            // calculate GLSL alignment
            uint32_t size;
            uint32_t alignment;
            uint32_t arrayElementPadding;
            CalculateLayout(compiler, var, totalArraySize, structureAlignment == StructureAlignment::STD140 ? true : false, size, alignment, arrayElementPadding);
            maxAlignment = max(alignment, maxAlignment);

            varResolved->byteSize = size;
            varResolved->elementPadding = arrayElementPadding;

            // calculate padding
            if (offset % alignment > 0)
            {
                int padding = alignment - (offset % alignment);
                offset += padding;
                varResolved->startPadding = padding;
            }

            // set offset within structure
            varResolved->structureOffset = offset;

            // progress to next variable
            offset += size;

            // generate code for variable
            members.append("\t");
            GenerateVariableGLSL(compiler, var, members);
            members.append(";\n");
        }
    }

    outSize = offset;
    outAlignment = maxAlignment;
    outCode = members;
}

//------------------------------------------------------------------------------
/**
*/
void 
GLSLGenerator::GenerateStructureSPIRV(const Compiler* compiler, const ProgramInstance* program, Symbol* symbol, std::string& outCode)
{
    Structure* struc = static_cast<Structure*>(symbol);
    Structure::__Resolved* strucResolved = static_cast<Structure::__Resolved*>(struc->resolved);

    // format variables
    uint32_t size, alignment;
    std::string members;
    GenerateAlignedVariables(compiler, struc, strucResolved->usageFlags.flags.isUniformBuffer ? StructureAlignment::STD140 : StructureAlignment::STD430, size, alignment, members);

    strucResolved->byteSize = size;
    strucResolved->baseAlignment = alignment;

    outCode.append(Format("#line %d %s\n", struc->location.line, struc->location.file.c_str()));
    /*
    if (strucResolved->usageFlags.flags.isUniformBuffer)
        outCode.append(Format("layout(set=%d, binding=%d) uniform __%s\n{\n%s} %s;\n\n", strucResolved->group, strucResolved->binding, struc->name.c_str(), members.c_str(), struc->name.c_str()));
    else if (strucResolved->usageFlags.flags.isMutableBuffer)
        outCode.append(Format("layout(set=%d, binding=%d) buffer __%s\n{\n%s} %s;\n\n", strucResolved->group, strucResolved->binding, struc->name.c_str(), members.c_str(), struc->name.c_str()));
    else
        */

        outCode.append(Format("struct %s \n{\n%s};\n\n", struc->name.c_str(), members.c_str()));
}

//------------------------------------------------------------------------------
/**
*/
void 
GLSLGenerator::GenerateVariableSPIRV(const Compiler* compiler, const ProgramInstance* program, Symbol* symbol, std::string& outCode, bool isShaderArgument)
{
    Variable* var = static_cast<Variable*>(symbol);
    Variable::__Resolved* varResolved = static_cast<Variable::__Resolved*>(var->resolved);

    // Abort if struct pointer (because it will be done with GenerateStruct)
    if (var->type.IsPointer() && varResolved->typeSymbol->category == Type::Category::UserTypeCategory)
        return;

    Type::FullType type = var->type;
    if (varResolved->typeSymbol->symbolType != Symbol::StructureType)
    {
        auto it = typeToGlslType.Find(type.name);
        if (it == typeToGlslType.end())
        {
            this->Error(Format("INTERNAL ERROR, built-in type '%s' has no GLSL mapping", type.name.c_str()));
        }
        type.name = it->second.c_str();
    }

    const FixedString& name = var->name;
    std::string arraySize = "";
    for (int i = var->type.modifierValues.size()-1; i >= 0; i--)
    {
        if (var->type.modifiers[i] == Type::FullType::Modifier::Pointer)
            continue;
        uint32_t size = 0;
        if (var->type.modifierValues[i] != nullptr)
        {
            ValueUnion val;
            var->type.modifierValues[i]->EvalValue(val);
            val.Store(size);
        }
        if (size > 0)
        {
            arraySize.append(Format("[%d]", size));
        }
        else
        {
            arraySize.append(Format("[]"));
        }
    }

    // format parameters
    if (varResolved->usageBits.flags.isParameter)
    {
        if (isShaderArgument)
        {
            outCode.append(Format("#line %d %s\n", var->location.line, var->location.file.c_str()));
            outCode.append(Format("layout(location=%d) ", varResolved->binding));
        }

        if (varResolved->storage == Storage::Input)
            outCode.append("in ");
        if (varResolved->storage == Storage::Output)
            outCode.append("out ");

        if (varResolved->parameterBits.flags.isNoInterpolate)
            outCode.append("flat ");
        if (varResolved->parameterBits.flags.isNoPerspective)
            outCode.append("noperspective ");

        if (varResolved->parameterBits.flags.isPatch)
            outCode.append("patch ");

        outCode.append(Format("%s %s", type.name.c_str(), name.c_str()));
    }
    else if (varResolved->usageBits.flags.isConst)
    {
        std::string initializerStr;
        GenerateExpressionGLSL(compiler, var->valueExpression, initializerStr);
        outCode.append(Format("const %s %s%s = %s;\n", type.name.c_str(), name.c_str(), arraySize.c_str(), initializerStr.c_str()));
    }
    else if (!varResolved->usageBits.flags.isStructMember)
    {
        
        outCode.append(Format("#line %d %s\n", var->location.line, var->location.file.c_str()));
        if (varResolved->typeSymbol->category == Type::TextureCategory && var->type.IsMutable())
        {
            auto glslFormat = imageFormatToGlsl.Find(var->type.imageFormat);
            if (glslFormat != imageFormatToGlsl.end())
            {
                outCode.append(Format("layout(set=%d, binding=%d, %s) ", varResolved->group, varResolved->binding, glslFormat->second.c_str()));
            }

            // if integer or unsigned, format variable
            if (IsImageFormatInteger(var->type.imageFormat))
                type.name = Format("i%s", type.name.c_str());
            else if (IsImageFormatUnsigned(var->type.imageFormat))
                type.name = Format("u%s", type.name.c_str());
        }
        else
            outCode.append(Format("layout(set=%d, binding=%d) ", varResolved->group, varResolved->binding));

        if (varResolved->accessBits.flags.atomicAccess)
            outCode.append("coherent ");
        if (varResolved->accessBits.flags.volatileAccess)
            outCode.append("volatile ");
        if (varResolved->accessBits.flags.readAccess && varResolved->accessBits.flags.writeAccess)
            outCode.append("restrict ");
        else
        {
            if (varResolved->accessBits.flags.readAccess)
                outCode.append("readonly ");
            if (varResolved->accessBits.flags.writeAccess)
                outCode.append("writeonly ");
        }

        if (varResolved->storage == Storage
::Workgroup)
            outCode.append("shared ");

        /*
        if (varResolved->typeSymbol->symbolType == Symbol::StructureType
            && (varResolved->usageBits.flags.isMutable
            || varResolved->usageBits.flags.isUniform))
        {
            // Type is a structure which is either a glsl 'buffer' or 'uniform'
            std::string variables;
            Structure* struc = static_cast<Structure*>(varResolved->typeSymbol);

            StructureAlignment alignment = StructureAlignment::STD430;
            std::string type = "buffer";
            if (varResolved->usageBits.flags.isUniform)
            {
                alignment = StructureAlignment::STD140;
                type = "uniform";
            }

            uint32_t dummy1, dummy2;
            GenerateAlignedVariables(compiler, struc, alignment, dummy1, dummy2, variables);
            outCode.append(Format("%s\n{\n%s} %s%s;\n", type.c_str(), variables.c_str(), name.c_str(), arraySize.c_str()));
        }
        else
        */
            outCode.append(Format("uniform %s %s%s;\n", type.name.c_str(), name.c_str(), arraySize.c_str()));
    }
    else
    {
        outCode.append(Format("    %s %s%s;", type.name.c_str(), name.c_str(), arraySize.c_str()));
    }

}

} // namespace GPULang
