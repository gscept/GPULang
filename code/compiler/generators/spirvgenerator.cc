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
#include "ast/expressions/intvecexpression.h"
#include "ast/expressions/uintexpression.h"
#include "ast/expressions/uintvecexpression.h"
#include "ast/expressions/floatexpression.h"
#include "ast/expressions/floatvecexpression.h"
#include "ast/expressions/boolexpression.h"
#include "ast/expressions/boolvecexpression.h"
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
#include "ast/statements/terminatestatement.h"
#include "ast/statements/returnstatement.h"
#include "ast/statements/scopestatement.h"
#include "ast/statements/switchstatement.h"
#include "ast/statements/whilestatement.h"

#include "ast/types/builtins.h"
#include "ast/intrinsics.h"

#include "spirv-tools/libspirv.h"
#include "spirv-tools/optimizer.hpp"

#include <array>
#include <complex>
#include <format>
#include <vcruntime_startup.h>

#include "ast/samplerstate.h"
#include "ast/expressions/arrayinitializerexpression.h"
#include "ast/expressions/floatvecexpression.h"

namespace GPULang
{
struct SPVHeader
{
    uint32_t magic;
    struct
    {
        uint32_t leading : 8;
        uint32_t major : 8;
        uint32_t minor : 8;
        uint32_t trailing : 8;
    } version;
    uint32_t bound; // range of IDs generated, the tighter the better
    uint32_t schema;
};

union SPVInstruction
{
    struct
    {
        uint32_t wordCount : 16;
        uint32_t code : 16;
    } flags;
    uint32_t bits;
};

struct SPVOp
{
    const char* str = nullptr;
    const uint16_t c = 0xFFFF;
};

struct SPVEnum
{
    const char* str = nullptr;
    const uint16_t c = 0xFFFF;
};

bool operator<(const SPVEnum& lhs, const SPVEnum& rhs)
{
    return lhs.c < rhs.c;
}

struct SPVArg
{
    uint32_t arg;
};

struct SPVLiteralList
{
    uint32_t* vals = nullptr;
    uint8_t num = 0;
};

struct SPVResultList
{
    SPIRVResult* vals = nullptr;
    uint8_t num = 0;
};

struct SPVArgList
{
    SPVArg* vals = nullptr;
    uint8_t num = 0;
};

#define SPV_INSTRUCTION(name, code) SPVOp name { .str = #name, .c = code };
SPV_INSTRUCTION(OpExtension, 10)
SPV_INSTRUCTION(OpExtInstImport, 11)
SPV_INSTRUCTION(OpExtInst, 12)
SPV_INSTRUCTION(OpMemoryModel, 14)
SPV_INSTRUCTION(OpEntryPoint, 15)
SPV_INSTRUCTION(OpExecutionMode, 15)
SPV_INSTRUCTION(OpCapability, 17)
SPV_INSTRUCTION(OpTypeVoid, 19)
SPV_INSTRUCTION(OpTypeBool, 20)
SPV_INSTRUCTION(OpTypeInt, 21)
SPV_INSTRUCTION(OpTypeFloat, 22)
SPV_INSTRUCTION(OpTypeVector, 23)
SPV_INSTRUCTION(OpTypeMatrix, 24)
SPV_INSTRUCTION(OpTypeImage, 25)
SPV_INSTRUCTION(OpTypeSampler, 26)
SPV_INSTRUCTION(OpTypeSampledImage, 27)
SPV_INSTRUCTION(OpTypeArray, 28)
SPV_INSTRUCTION(OpTypeRuntimeArray, 29)
SPV_INSTRUCTION(OpTypeStruct, 30)
SPV_INSTRUCTION(OpTypePointer, 32)
SPV_INSTRUCTION(OpTypeFunction, 33)
SPV_INSTRUCTION(OpConstantTrue, 41)
SPV_INSTRUCTION(OpConstantFalse, 42)
SPV_INSTRUCTION(OpConstant, 43)
SPV_INSTRUCTION(OpConstantComposite, 44)
SPV_INSTRUCTION(OpConstantSampler, 45)
SPV_INSTRUCTION(OpSpecConstantTrue, 48)
SPV_INSTRUCTION(OpSpecConstantFalse, 49)
SPV_INSTRUCTION(OpSpecConstant, 50)
SPV_INSTRUCTION(OpSpecConstantComposite, 51)
SPV_INSTRUCTION(OpFunction, 54)
SPV_INSTRUCTION(OpFunctionParameter, 55)
SPV_INSTRUCTION(OpFunctionEnd, 56)
SPV_INSTRUCTION(OpFunctionCall, 57)
SPV_INSTRUCTION(OpVariable, 59)
SPV_INSTRUCTION(OpAccessChain, 65)
SPV_INSTRUCTION(OpLoad, 61)
SPV_INSTRUCTION(OpStore, 62)
SPV_INSTRUCTION(OpCopyMemory, 63)
SPV_INSTRUCTION(OpDecorate, 71)
SPV_INSTRUCTION(OpMemberDecorate, 72)
SPV_INSTRUCTION(OpVectorShuffle, 79)
SPV_INSTRUCTION(OpCompositeExtract, 81)
SPV_INSTRUCTION(OpCompositeInsert, 82)
SPV_INSTRUCTION(OpCompositeConstruct, 80)
SPV_INSTRUCTION(OpTranspose, 84)
SPV_INSTRUCTION(OpConvertFToU, 109)
SPV_INSTRUCTION(OpConvertFToS, 110)
SPV_INSTRUCTION(OpConvertSToF, 111)
SPV_INSTRUCTION(OpConvertUToF, 112)
SPV_INSTRUCTION(OpBitcast, 124)
SPV_INSTRUCTION(OpSNegate, 126)
SPV_INSTRUCTION(OpFNegate, 127)
SPV_INSTRUCTION(OpIAdd, 128)
SPV_INSTRUCTION(OpFAdd, 129)
SPV_INSTRUCTION(OpISub, 130)
SPV_INSTRUCTION(OpFSub, 131)
SPV_INSTRUCTION(OpIMul, 132)
SPV_INSTRUCTION(OpFMul, 133)
SPV_INSTRUCTION(OpUDiv, 134)
SPV_INSTRUCTION(OpSDiv, 135)
SPV_INSTRUCTION(OpFDiv, 136)
SPV_INSTRUCTION(OpUMod, 137)
SPV_INSTRUCTION(OpSMod, 139)
SPV_INSTRUCTION(OpFMod, 141)
SPV_INSTRUCTION(OpVectorTimesScalar, 142)
SPV_INSTRUCTION(OpMatrixTimesScalar, 143)
SPV_INSTRUCTION(OpVectorTimesMatrix, 144)
SPV_INSTRUCTION(OpMatrixTimesVector, 145)
SPV_INSTRUCTION(OpMatrixTimesMatrix, 146)
SPV_INSTRUCTION(OpDot, 148)
SPV_INSTRUCTION(OpAny, 154)
SPV_INSTRUCTION(OpAll, 155)
SPV_INSTRUCTION(OpLogicalOr, 166)
SPV_INSTRUCTION(OpLogicalAnd, 167)
SPV_INSTRUCTION(OpLogicalNot, 168)
SPV_INSTRUCTION(OpSelect, 169)
SPV_INSTRUCTION(OpIEqual, 170)
SPV_INSTRUCTION(OpINotEqual, 171)
SPV_INSTRUCTION(OpUGreaterThan, 172)
SPV_INSTRUCTION(OpSGreaterThan, 173)
SPV_INSTRUCTION(OpUGreaterThanEqual, 174)
SPV_INSTRUCTION(OpSGreaterThanEqual, 175)
SPV_INSTRUCTION(OpULessThan, 176)
SPV_INSTRUCTION(OpSLessThan, 177)
SPV_INSTRUCTION(OpULessThanEqual, 178)  
SPV_INSTRUCTION(OpSLessThanEqual, 179)
SPV_INSTRUCTION(OpFUnordEqual, 181)
SPV_INSTRUCTION(OpFUnordNotEqual, 183)
SPV_INSTRUCTION(OpFUnordLessThan, 185)
SPV_INSTRUCTION(OpFUnordGreaterThan, 187)
SPV_INSTRUCTION(OpFUnordLessThanEqual, 189)
SPV_INSTRUCTION(OpFUnordGreaterThanEqual, 191)
SPV_INSTRUCTION(OpShiftRightLogical, 194)
SPV_INSTRUCTION(OpShiftRightArithmetic, 195)
SPV_INSTRUCTION(OpShiftLeftLogical, 196)
SPV_INSTRUCTION(OpShiftLeftArithmetic, 197)
SPV_INSTRUCTION(OpBitwiseOr, 197)
SPV_INSTRUCTION(OpBitwiseXor, 198)
SPV_INSTRUCTION(OpBitwiseAnd, 199)
SPV_INSTRUCTION(OpNot, 200)
SPV_INSTRUCTION(OpBitfieldInsert, 201)
SPV_INSTRUCTION(OpBitfieldSExtract, 202)
SPV_INSTRUCTION(OpBitfieldUExtract, 203)
SPV_INSTRUCTION(OpBitReverse, 204)
SPV_INSTRUCTION(OpBitCount, 205)
SPV_INSTRUCTION(OpDPdx, 207)
SPV_INSTRUCTION(OpDPdy, 208)
SPV_INSTRUCTION(OpFwidth, 209)
SPV_INSTRUCTION(OpLoopMerge, 246)
SPV_INSTRUCTION(OpSelectionMerge, 247)
SPV_INSTRUCTION(OpLabel, 248)
SPV_INSTRUCTION(OpBranch, 249)
SPV_INSTRUCTION(OpBranchConditional, 250)
SPV_INSTRUCTION(OpSwitch, 251)
SPV_INSTRUCTION(OpReturn, 253)
SPV_INSTRUCTION(OpReturnValue, 254)
SPV_INSTRUCTION(OpUnreachable, 255)
SPV_INSTRUCTION(OpTerminateInvocation, 4416)


#define SPV_ENUM(name, code) SPVEnum name { .str = #name, .c = code };
/// Capabilties
namespace Capabilities
{
SPV_ENUM(Matrix, 0)
SPV_ENUM(Shader, 1)
SPV_ENUM(Geometry, 2)
SPV_ENUM(Tessellation, 3)
SPV_ENUM(Addresses, 4)
SPV_ENUM(ImageBasic, 13)
SPV_ENUM(ImageReadWrite, 14)
SPV_ENUM(SubgroupDispatch, 58)
SPV_ENUM(MeshShadingEXT, 5283)
SPV_ENUM(RayTracingKHR, 4479)
SPV_ENUM(ComputeDerivativeGroupLinearKHR, 5288)
SPV_ENUM(ComputeDerivativeGroupQuadsKHR, 5230)
}

namespace AddressingModels
{
/// Addressing models
SPV_ENUM(Logical, 0)
SPV_ENUM(Physical32, 1)
SPV_ENUM(Physical64, 2)
SPV_ENUM(PhysicalStorageBuffer64, 5348)
}

namespace MemoryModels
{
/// Memory models
SPV_ENUM(Simple, 0)
SPV_ENUM(GLSL450, 1)
SPV_ENUM(OpenCL, 2)
SPV_ENUM(Vulkan, 3)
}

namespace ExecutionModels
{
/// Execution models
SPV_ENUM(Vertex, 0)
SPV_ENUM(TessellationControl, 1)
SPV_ENUM(TessellationEvaluation, 2)
SPV_ENUM(Geometry, 3)
SPV_ENUM(Fragment, 4)
SPV_ENUM(GLCompute, 5)
SPV_ENUM(RayGenerationKHR, 5313)
SPV_ENUM(IntersectionKHR, 5314)
SPV_ENUM(AnyHitKHR, 5315)
SPV_ENUM(ClosestHitKHR, 5316)
SPV_ENUM(MissKHR, 5317)
SPV_ENUM(CallableKHR, 5318)
SPV_ENUM(TaskEXT, 5364)
SPV_ENUM(MeshEXT, 5365)
}

namespace ExecutionModes
{
SPV_ENUM(Invocations, 0)
SPV_ENUM(SpacingEqual, 1)
SPV_ENUM(SpacingFractionalEven, 2)
SPV_ENUM(SpacingFractionalOdd, 3)
SPV_ENUM(VertexOrderCw, 4)
SPV_ENUM(VertexOrderCcw, 5)
SPV_ENUM(PixelCenterInteger, 6)
SPV_ENUM(OriginUpperLeft, 7)
SPV_ENUM(OriginLowerLeft, 8)
SPV_ENUM(EarlyFragmentTests, 9)
SPV_ENUM(PointMode, 10)
SPV_ENUM(DepthReplacing, 12)
SPV_ENUM(DepthGreater, 14)
SPV_ENUM(DepthLess, 15)
SPV_ENUM(DepthUnchanged, 16)
SPV_ENUM(LocalSize, 17)
SPV_ENUM(LocalSizeHint, 18)
SPV_ENUM(InputPoints, 19)
SPV_ENUM(InputLines, 20)
SPV_ENUM(InputLinesAdjacency, 21)
SPV_ENUM(Triangles, 22)
SPV_ENUM(InputTrianglesAdjacency, 23)
SPV_ENUM(Quads, 24)
SPV_ENUM(Isolines, 25)
SPV_ENUM(OutputVertices, 26)
SPV_ENUM(OutputPoints, 27)
SPV_ENUM(OutputLineStrip, 28)
SPV_ENUM(OutputTriangleStrip, 29)
SPV_ENUM(SubgroupSize, 35)
SPV_ENUM(SubgroupsPerWorkgroup, 36)
SPV_ENUM(DerivativeGroupQuadsKHR, 5289)
SPV_ENUM(DerivativeGroupLinearKHR, 5289)
}

namespace FunctionControl
{
SPV_ENUM(None, 0)
SPV_ENUM(Inline, 0x1)
SPV_ENUM(DontInline, 0x2)
SPV_ENUM(Pure, 0x4)
SPV_ENUM(Const, 0x8)
}

namespace VariableStorage
{
SPV_ENUM(UniformConstant, 0)
SPV_ENUM(Input, 1)
SPV_ENUM(Uniform, 2)
SPV_ENUM(Output, 3)
SPV_ENUM(Workgroup, 4)
SPV_ENUM(CrossWorkgroup, 5)
SPV_ENUM(Private, 6)
SPV_ENUM(Function, 7)
SPV_ENUM(Generic, 8)
SPV_ENUM(PushConstant, 9)
SPV_ENUM(AtomicCounter, 10)
SPV_ENUM(Image, 11)
SPV_ENUM(StorageBuffer, 12)
SPV_ENUM(CallableDataKHR, 5328)
SPV_ENUM(IncomingCallableDataKHR, 5329)
SPV_ENUM(RayPayloadKHR, 5338)
SPV_ENUM(HitAttributeKHR, 5339)
SPV_ENUM(IncomingRayPayloadKHR, 5342)
SPV_ENUM(PhysicalStorageBuffer, 5349)
SPV_ENUM(TaskPayloadWorkgroupEXT, 5402)
}

namespace Decorations
{
SPV_ENUM(SpecId, 1)
SPV_ENUM(Block, 2)
SPV_ENUM(RowMajor, 4)
SPV_ENUM(ColMajor, 5)
SPV_ENUM(ArrayStride, 6)
SPV_ENUM(MatrixStride, 7)
SPV_ENUM(BuiltIn, 11)
SPV_ENUM(NoPerspective, 13)
SPV_ENUM(Flat, 14)
SPV_ENUM(Patch, 15)
SPV_ENUM(Centroid, 16)
SPV_ENUM(Restrict, 19)
SPV_ENUM(Volatile, 21)
SPV_ENUM(Coherent, 23)
SPV_ENUM(Location, 30)
SPV_ENUM(Component, 31)
SPV_ENUM(Index, 32)
SPV_ENUM(Binding, 33)
SPV_ENUM(DescriptorSet, 34)
SPV_ENUM(Offset, 35)
SPV_ENUM(InputAttachmentIndex, 43)
SPV_ENUM(Alignment, 44)
}

SPVEnum ScopeToEnum(SPIRVResult::Storage s)
{
    switch (s)
    {
        case SPIRVResult::Storage::Device:
            return VariableStorage::CrossWorkgroup;
        case SPIRVResult::Storage::Private:
            return VariableStorage::Private;
        case SPIRVResult::Storage::WorkGroup:
            return VariableStorage::Workgroup;
        case SPIRVResult::Storage::Uniform:
            return VariableStorage::Uniform;
        case SPIRVResult::Storage::Sampler:
        case SPIRVResult::Storage::Image:
        case SPIRVResult::Storage::MutableImage:
        case SPIRVResult::Storage::UniformConstant:
            return VariableStorage::UniformConstant;
        case SPIRVResult::Storage::StorageBuffer:
            return VariableStorage::StorageBuffer;
        case SPIRVResult::Storage::PushConstant:
            return VariableStorage::PushConstant;
        case SPIRVResult::Storage::Function:
            return VariableStorage::Function;
        case SPIRVResult::Storage::Input:
            return VariableStorage::Input;
        case SPIRVResult::Storage::Output:
            return VariableStorage::Output;
        case SPIRVResult::Storage::RayPayload:
            return VariableStorage::RayPayloadKHR;
        case SPIRVResult::Storage::RayPayloadInput:
            return VariableStorage::IncomingRayPayloadKHR;
        case SPIRVResult::Storage::RayHitAttribute:
            return VariableStorage::HitAttributeKHR;
        case SPIRVResult::Storage::CallableData:
            return VariableStorage::CallableDataKHR;
        case SPIRVResult::Storage::CallableDataInput:
            return VariableStorage::IncomingCallableDataKHR;
        default:
            assert(false);
        return SPVEnum();
    }
}

/// GLSL 450 ops
SPV_ENUM(Round, 1)
SPV_ENUM(RoundEven, 2)
SPV_ENUM(Trunc, 3)
SPV_ENUM(FAbs, 4)
SPV_ENUM(SAbs, 5)
SPV_ENUM(FSign, 6)
SPV_ENUM(SSign, 7)
SPV_ENUM(Floor, 8)
SPV_ENUM(Ceil, 9)
SPV_ENUM(Fract, 10)
SPV_ENUM(Sin, 13)
SPV_ENUM(Cos, 14)
SPV_ENUM(Tan, 15)
SPV_ENUM(Asin, 16)
SPV_ENUM(Acos, 17)
SPV_ENUM(Atan, 18)
SPV_ENUM(Sinh, 19)
SPV_ENUM(Cosh, 20)
SPV_ENUM(Tanh, 21)
SPV_ENUM(Asinh, 22)
SPV_ENUM(Acosh, 23)
SPV_ENUM(Atanh, 24)
SPV_ENUM(Atan2, 25)
SPV_ENUM(Pow, 26)
SPV_ENUM(Exp, 27)
SPV_ENUM(Log, 28)
SPV_ENUM(Exp2, 29)
SPV_ENUM(Log2, 30)
SPV_ENUM(Sqrt, 31)
SPV_ENUM(InverseSqrt, 32)
SPV_ENUM(Determinant, 33)
SPV_ENUM(MatrixInverse, 34)
SPV_ENUM(FMin, 37)
SPV_ENUM(UMin, 38)
SPV_ENUM(SMin, 39)
SPV_ENUM(FMax, 40)
SPV_ENUM(UMax, 41)
SPV_ENUM(SMax, 42)
SPV_ENUM(FClamp, 43)
SPV_ENUM(UClamp, 44)
SPV_ENUM(SClamp, 45)
SPV_ENUM(FMix, 46)
SPV_ENUM(Step, 48)
SPV_ENUM(SmoothStep, 49)
SPV_ENUM(Fma, 50)
SPV_ENUM(Length, 66)
SPV_ENUM(Distance, 67)
SPV_ENUM(Cross, 68)
SPV_ENUM(Normalize, 69)
SPV_ENUM(Reflect, 71)
SPV_ENUM(Refract, 72)

#define SPV_STRING(str, value) const char* str = #value;
SPV_STRING(GLSL, GLSL.std.450)
SPV_ENUM(SPV_KHR_compute_shader_derivatives, 0xFFFF)

struct SPVWriter
{
    enum Mode
    {
        Text,
        Binary
    } mode;

    enum class Section : uint32_t
    {
        Header,
        Capabilities,
        Extensions,
        ExtImports,
        Decorations,
        Declarations,
        Functions,
        LocalFunction,
        FunctionInit,
        FunctionStart,

        NumSections
    } section = Section::Header;
    uint32_t counter = 0;

    void Header(const SPVHeader& header)
    {
    }

    uint32_t Import(const char* str)
    {
        uint32_t c = this->counter;
        auto it = this->imports.find(str);
        if (it != this->imports.end())
            return it->second;

        if (this->mode == Mode::Binary)
        {
            size_t len = strlen(str);
            size_t lenInWords = std::ceil(len / 4.0f);
            SPVInstruction instr;
            instr.flags.wordCount = 3 + lenInWords;
            instr.flags.code = OpExtInstImport.c;

            this->binaries[(uint32_t)Section::ExtImports].push_back(instr.bits);
            this->binaries[(uint32_t)Section::ExtImports].push_back(this->counter++);
            uint32_t* strAsWords = (uint32_t*)str;
            for (size_t i = 0; i < lenInWords; i++)
            {
                this->binaries[(uint32_t)Section::ExtImports].push_back(strAsWords[i]);
            }
        }
        else
        {
            TStr extension = TStr::Compact("\"", str, "\"");
            this->texts[(uint32_t)Section::ExtImports].append(TStr::Separated(SPVArg(this->counter++), "=", "OpExtInstImport", extension).ToString());
            this->texts[(uint32_t)Section::ExtImports].append("\n");
        }
        this->imports[str] = c;
        return c;
    }

    void Capability(SPVEnum cap)
    {
        if (this->capabilities.contains(cap))
            return;
        this->Instruction(OpCapability, Section::Capabilities, cap);
        this->capabilities.insert(cap);
    }

    void Extension(SPVEnum extension)
    {
        if (this->extensions.contains(extension))
            return;
        this->Instruction(OpExtension, Section::Extensions, extension.str);
        this->extensions.insert(extension);
    }

    template<typename ...ARGS>
    void Decorate(SPVArg type, SPVEnum decoration, const ARGS&... args)
    {
        this->Instruction(OpDecorate, Section::Decorations, type, std::forward<const ARGS&...>(args));
    }

    template<typename ...ARGS>
    void MemberDecorate(SPVArg type, uint32_t member, SPVEnum decoration, const ARGS&... args)
    {
        this->Instruction(OpMemberDecorate, Section::Decorations, type, member, std::forward<const ARGS&...>(args));
    }

    template<typename ...ARGS>
    uint32_t MappedInstruction(const SPVOp& op, SPVWriter::Section section, uint32_t type, const ARGS&... args)
    {
        uint32_t c = this->counter;
        this->section = section;
        if (this->mode == Mode::Binary)
        {
            SPVInstruction instr;
            instr.flags.wordCount = sizeof...(args) + 3; // +3 for opcode, type and id
            instr.flags.code = op.c;

            this->binaries[(uint32_t)section].push_back(instr.bits);
            this->binaries[(uint32_t)section].push_back(type);
            this->binaries[(uint32_t)section].push_back(this->counter++);
            (Append(args), ...);
        }
        else
        {
            this->texts[(uint32_t)section].append(TStr::Separated(SPVArg(this->counter++), "=", op.str, SPVArg(type)).ToString());
            (Append(args), ...);
            this->texts[(uint32_t)section].append("\n");
        }
        return c;
    }

    template<typename ...ARGS>
    uint32_t MappedInstruction(const SPVOp& op, SPVWriter::Section section, const ARGS&... args)
    {
        uint32_t c = this->counter;
        this->section = section;
        if (this->mode == Mode::Binary)
        {
            SPVInstruction instr;
            instr.flags.wordCount = sizeof...(args) + 3; // +3 for opcode, type and id
            instr.flags.code = op.c;

            this->binaries[(uint32_t)section].push_back(instr.bits);
            this->binaries[(uint32_t)section].push_back(this->counter++);
            (Append(args), ...);
        }
        else
        {
            this->texts[(uint32_t)section].append(TStr::Separated(SPVArg(this->counter++), "=", op.str).ToString());
            (Append(args), ...);
            this->texts[(uint32_t)section].append("\n");
        }
        return c;
    }

    void Instruction(const SPVOp& op, SPVWriter::Section section, uint32_t type, std::vector<uint32_t> args)
    {
        this->section = section;
        if (this->mode == Mode::Binary)
        {
            SPVInstruction instr;
            instr.flags.wordCount = args.size();
            instr.flags.code = op.c;

            this->binaries[(uint32_t)section].push_back(instr.bits);
            this->binaries[(uint32_t)section].insert(binaries[(uint32_t)this->section].end(), args.begin(), args.end());
        }
        else
        {
            TStr argsStr;
            for (uint32_t arg : args)
                argsStr.Concatenate<false>(SPVArg(arg), " ");
            texts[(uint32_t)section].append(TStr::Separated(op.str, SPVArg(type), argsStr).ToString());
            texts[(uint32_t)section].append("\n");
        }
    }

    template<typename ...ARGS>
    void Instruction(const SPVOp& op, SPVWriter::Section section, ARGS... args)
    {
        this->section = section;
        if (this->mode == Mode::Binary)
        {
            SPVInstruction instr;
            instr.flags.wordCount = sizeof...(args);
            instr.flags.code = op.c;

            this->binaries[(uint32_t)section].push_back(instr.bits);
            (Append(args), ...);
        }
        else
        {
            this->texts[(uint32_t)section].append(op.str);
            (Append(args), ...);
            this->texts[(uint32_t)section].append("\n");
        }
    }

    void FinishFunction()
    {
        if (this->mode == Mode::Binary)
        {
            this->binaries[(uint32_t)SPVWriter::Section::Functions].insert(
                this->binaries[(uint32_t)SPVWriter::Section::Functions].begin()
                , this->binaries[(uint32_t)SPVWriter::Section::LocalFunction].begin()
                , this->binaries[(uint32_t)SPVWriter::Section::LocalFunction].end()
            );
            this->binaries[(uint32_t)SPVWriter::Section::LocalFunction].clear();
        }
        else
        {
            this->texts[(uint32_t)SPVWriter::Section::Functions].insert(
                this->texts[(uint32_t)SPVWriter::Section::Functions].begin()
                , this->texts[(uint32_t)SPVWriter::Section::LocalFunction].begin()
                , this->texts[(uint32_t)SPVWriter::Section::LocalFunction].end()
            );
            this->texts[(uint32_t)SPVWriter::Section::LocalFunction].clear();
        }
    }

    template<typename T>
    void Append(const T& arg)
    {
        assert(false && "Should never enter");
        //static_assert(false, "This should never happen");
    }

    template<>
    void Append(const SPVArg& arg)
    {
        if (this->mode == Mode::Binary)
        {
            this->binaries[(uint32_t)this->section].push_back(arg.arg);
        }
        else
        {
            char buf[64];
            int numWritten = snprintf(buf, 64, " %%%d", arg.arg);
            this->texts[(uint32_t)this->section].append(buf, numWritten);
        }
    }
    
    template<>
    void Append(const char* const& str)
    {
        if (this->mode == Mode::Binary)
        {
            size_t len = strlen(str);
            size_t lenInWords = std::ceil(len / 4.0f);
            uint32_t* strAsWords = (uint32_t*)str;
            for (size_t i = 0; i < lenInWords; i++)
            {
                this->binaries[(uint32_t)this->section].push_back(strAsWords[i]);
            }
        }
        else
        {
            this->texts[(uint32_t)this->section].append(TStr::Compact(" \"", str, "\"").ToString());
            this->texts[(uint32_t)this->section].append("\n");
        }
    }

    template<>
    void Append(const int& arg)
    {
        if (this->mode == Mode::Binary)
        {
            this->binaries[(uint32_t)this->section].push_back(arg);
        }
        else
        {
            char buf[64];
            int numWritten = snprintf(buf, 64, " %d", arg);
            this->texts[(uint32_t)this->section].append(buf, numWritten);
        }
    }

    template<>
    void Append(const float& arg)
    {
        if (this->mode == Mode::Binary)
        {
            this->binaries[(uint32_t)this->section].push_back(arg);
        }
        else
        {
            char buf[64];
            int numWritten = snprintf(buf, 64, " %f", arg);
            this->texts[(uint32_t)this->section].append(buf, numWritten);
        }
    }

    template<>
    void Append(const unsigned int& arg)
    {
        if (this->mode == Mode::Binary)
        {
            this->binaries[(uint32_t)this->section].push_back(arg);
        }
        else
        {
            char buf[64];
            int numWritten = snprintf(buf, 64, " %u", arg);
            this->texts[(uint32_t)this->section].append(buf, numWritten);
        }
    }

    template<>
    void Append(const SPVEnum& arg)
    {
        if (this->mode == Mode::Binary)
        {
            this->binaries[(uint32_t)this->section].push_back(arg.c);
        }
        else
        {
            this->texts[(uint32_t)this->section].append(" ");
            this->texts[(uint32_t)this->section].append(arg.str);
        }
    }

    template<>
    void Append(const SPVLiteralList& arg)
    {
        if (this->mode == Mode::Binary)
        {
            for (uint8_t it = 0; it < arg.num; it++)
                this->binaries[(uint32_t)this->section].push_back(arg.vals[it]);
        }
        else
        {
            for (uint8_t it = 0; it < arg.num; it++)
                this->Append(arg.vals[it]);
        }
    }

    template<>
    void Append(const SPVResultList& arg)
    {
        if (this->mode == Mode::Binary)
        {
            for (uint8_t it = 0; it < arg.num; it++)
                this->binaries[(uint32_t)this->section].push_back(arg.vals[it].name);
        }
        else
        {
            for (uint8_t it = 0; it < arg.num; it++)
                this->Append(arg.vals[it]);
        }
    }

    template<>
    void Append(const SPVArgList& arg)
    {
        if (this->mode == Mode::Binary)
        {
            for (uint8_t it = 0; it < arg.num; it++)
                this->binaries[(uint32_t)this->section].push_back(arg.vals[it].arg);
        }
        else
        {
            for (uint8_t it = 0; it < arg.num; it++)
                this->Append(arg.vals[it]);
        }
    }

    template<>
    void Append(const SPIRVResult& arg)
    {
        this->Append(SPVArg(arg.name));
    }

    template<>
    void Append(const std::vector<SPIRVResult>& args)
    {
        for (const auto& arg : args)
            this->Append(SPVArg(arg.name));
    }

    template<>
    void Append(const std::vector<uint32_t>& args)
    {
        for (const auto& arg : args)
            this->Append(arg);
    }

    std::string texts[(uint32_t)Section::NumSections];
    std::vector<uint32_t> binaries[(uint32_t)Section::NumSections];

    std::map<const char*, uint32_t> imports;
    std::set<SPVEnum> capabilities;
    std::set<SPVEnum> extensions;
};

template<typename ...ARGS>
uint32_t
AddBaseType(SPIRVGenerator* gen, const TransientString& name, const SPVOp& op, const ARGS&... args)
{
    auto scope = gen->scopeStack.rbegin();
    while (scope != gen->scopeStack.rend())
    {
        auto it = scope->symbols.find(name.ToString());
        if (it != scope->symbols.end())
        {
            return it->second.value;
        }
        scope++;
    }

    // If symbol isn't found in scope, create it
    uint32_t ret = gen->writer->counter;
    gen->scopeStack.front().symbols[name.ToString()] = { .sym = nullptr, .value = ret, .type = SPIRVResult::Invalid() };
    gen->writer->section = SPVWriter::Section::Declarations;
    gen->writer->MappedInstruction(op, SPVWriter::Section::Declarations, std::forward<const ARGS&>(args)...);

    gen->writer->counter++;
    return ret;
}

template<typename ...ARGS>
uint32_t 
AddSymbol(SPIRVGenerator* gen, const TransientString& name, SPVWriter::Section section, const SPVOp& op, uint32_t type, const ARGS&... args)
{
    assert(section == SPVWriter::Section::Declarations || section == SPVWriter::Section::LocalFunction || section == SPVWriter::Section::FunctionInit);
    uint32_t ret = gen->writer->counter;

    if (section == SPVWriter::Section::Declarations)
    {
        auto scope = gen->scopeStack.rbegin();
        while (scope != gen->scopeStack.rend())
        {
            auto it = scope->symbols.find(name.ToString());
            if (it != scope->symbols.end())
            {
                return it->second.value;
            }
            scope++;
        }
        gen->scopeStack.front().symbols[name.ToString()] = { .sym = nullptr, .value = ret, .type = SPIRVResult::Invalid() };
    }
    else
    {
        auto& scope = gen->scopeStack.back();
        auto it = scope.symbols.find(name.ToString());
        if (it != scope.symbols.end())
        {
            return it->second.value;
        }
        gen->scopeStack.back().symbols[name.ToString()] = { .sym = nullptr, .value = ret, .type = SPIRVResult::Invalid() };
    }
    gen->writer->section = section;
    gen->writer->MappedInstruction(op, section, type, std::forward<const ARGS&>(args)...);

    gen->writer->counter++;
    return ret;
}

template<typename ...ARGS>
uint32_t 
MappedInstruction(SPIRVGenerator* gen, SPVWriter::Section section, const SPVOp& op, uint32_t type, const ARGS&... args)
{
    uint32_t ret = gen->writer->counter;
    gen->writer->section = section;
    gen->writer->MappedInstruction(op, section, type, std::forward<const ARGS&>(args)...);

    gen->writer->counter++;
    return ret;
}

template<typename ...ARGS>
uint32_t 
MappedInstruction(SPIRVGenerator* gen, SPVWriter::Section section, const SPVOp& op, const ARGS&... args)
{
    uint32_t ret = gen->writer->counter;
    gen->writer->section = section;
    gen->writer->MappedInstruction(op, section, std::forward<const ARGS&>(args)...);

    gen->writer->counter++;
    return ret;
}

template<typename ...ARGS>
uint32_t 
Instruction(SPIRVGenerator* gen, SPVWriter::Section section, const SPVOp& op, uint32_t type, const ARGS&... args)
{
    uint32_t ret = gen->writer->counter;
    gen->writer->section = section;
    gen->writer->Instruction(op, section, type, std::forward<const ARGS&>(args)...);

    gen->writer->counter++;
    return ret;
}

template<typename ...ARGS>
uint32_t 
Instruction(SPIRVGenerator* gen, SPVWriter::Section section, const SPVOp& op, const ARGS&... args)
{
    uint32_t ret = gen->writer->counter;
    gen->writer->section = section;
    gen->writer->Instruction(op, section, std::forward<const ARGS&>(args)...);

    gen->writer->counter++;
    return ret;
}

} // namespace GPULang

template<>
void GrowingString::FragmentString<GPULang::SPVArg>(GPULang::SPVArg arg, char* buf, size_t size)
{
    buf[0] = '%';
    _ultoa_s(arg.arg, buf + 1, size, 10);
}

template<>
void TransientString::FragmentString<GPULang::SPVArg>(GPULang::SPVArg arg)
{
    this->buf[this->size] = '%';
    _ultoa(arg.arg, this->buf + this->size + 1, 10);
}

template<>
void TransientString::Append<GPULang::SPVArg>(GPULang::SPVArg arg)
{
    this->buf[this->size] = '%';
    _ultoa(arg.arg, this->buf + this->size + 1, 10);
    this->size += NumChars(arg.arg) + 1;
}

template<>
inline size_t FragmentSize<GPULang::SPVArg>(GPULang::SPVArg arg)
{
    return (arg.arg < 10 ? 1 :
            (arg.arg < 100 ? 2 :
            (arg.arg < 1000 ? 3 :
            (arg.arg < 10000 ? 4 :
            (arg.arg < 100000 ? 5 :
            (arg.arg < 1000000 ? 6 :
            (arg.arg < 10000000 ? 7 :
            (arg.arg < 100000000 ? 8 :
            (arg.arg < 1000000000 ? 9 :
            10))))))))) + 1;
}

GPULang::SPVArg operator""_spv(unsigned long long arg)
{
    return GPULang::SPVArg{ (uint32_t)arg };
};

namespace GPULang
{


SPIRVResult GenerateVariableSPIRV(const Compiler* compiler, SPIRVGenerator* generator, Symbol* symbol, bool isShaderArgument, bool isGlobal);
SPIRVResult GenerateExpressionSPIRV(const Compiler* compiler, SPIRVGenerator* generator, Expression* expr);
bool GenerateStatementSPIRV(const Compiler* compiler, SPIRVGenerator* generator, Statement* stat);

using ImageFormatter = std::function<TransientString(uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format)>;

static const std::unordered_map<TypeCode, std::tuple<ConstantString, ImageFormatter>> handleTable =
{
    { TypeCode::Texture1D, { "texture1D", [](uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format) { return TStr::Separated("OpTypeImage", SPVArg(type), "1D", depthBits, 0, 0, sampledBits, format); } } }
    , { TypeCode::Texture2D, { "texture2D", [](uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format) { return TStr::Separated("OpTypeImage", SPVArg(type), "2D", depthBits, 0, 0, sampledBits, format); } } }
    , { TypeCode::Texture2DMS, { "texture2DMS", [](uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format) { return TStr::Separated("OpTypeImage", SPVArg(type), "2D", depthBits, 0, 1, sampledBits, format); } } }
    , { TypeCode::Texture3D, { "texture3D", [](uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format) { return TStr::Separated("OpTypeImage", SPVArg(type), "3D", depthBits, 0, 0, sampledBits, format); } } }
    , { TypeCode::TextureCube, { "textureCube", [](uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format) { return TStr::Separated("OpTypeImage", SPVArg(type), "Cube", depthBits, 0, 0, sampledBits, format); } } }
    , { TypeCode::Texture1DArray, { "texture1DArray", [](uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format) { return TStr::Separated("OpTypeImage", SPVArg(type), "1D", depthBits, 1, 0, sampledBits, format); } } }
    , { TypeCode::Texture2DArray, { "texture2DArray", [](uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format) { return TStr::Separated("OpTypeImage", SPVArg(type), "2D", depthBits, 1, 0, sampledBits, format); } } }
    , { TypeCode::Texture2DMSArray, { "texture2DMSArray", [](uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format) { return TStr::Separated("OpTypeImage", SPVArg(type), "1D", depthBits, 1, 1, sampledBits, format); } } }
    , { TypeCode::Texture3DArray, { "texture3DArray", [](uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format) { return TStr::Separated("OpTypeImage", SPVArg(type), "3D", depthBits, 1, 0, sampledBits, format); } } }
    , { TypeCode::TextureCubeArray, { "textureCubeArray", [](uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format) { return TStr::Separated("OpTypeImage", SPVArg(type), "Cube", depthBits, 1, 0, sampledBits, format); } } }
    , { TypeCode::PixelCache, { "pixelCache", [](uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format) { return TStr::Separated("OpTypeImage", SPVArg(type), SPACE, "SubpassData", 0, 0, 0, 2, "Unknown"); } } }
    , { TypeCode::PixelCacheMS, { "pixelCacheMS", [](uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format) { return TStr::Separated("OpTypeImage", SPVArg(type), SPACE, "SubpassData", 0, 0, 1, 2, "Unknown"); } } }
    , { TypeCode::Sampler, { "sampler", [](uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format) { return TStr("OpTypeSampler"); } } }
};

static const std::unordered_map<TypeCode, std::tuple<ConstantString, ConstantString, SPVOp, std::vector<uint32_t>>> scalarTable =
{
    { TypeCode::Int, { "i32", "OpTypeInt 32 1", OpTypeInt, { 32u, 1u } } }
    , { TypeCode::Int16, { "i16", "OpTypeInt 16 1", OpTypeInt, { 16u, 1u } } }
    , { TypeCode::UInt, { "u32", "OpTypeInt 32 0", OpTypeInt, { 32u, 0u } } }
    , { TypeCode::UInt16, { "u16", "OpTypeInt 16 0", OpTypeInt, { 16u, 0u } } }
    , { TypeCode::Float, { "f32", "OpTypeFloat 32", OpTypeFloat, { 32u } } }
    , { TypeCode::Float16, { "f16", "OpTypeFloat 16", OpTypeFloat, { 16u } } }
    , { TypeCode::Bool, { "b8", "OpTypeBool", OpTypeBool, { } } }
    , { TypeCode::Void, { "void", "OpTypeVoid", OpTypeVoid, { } } }
};

std::unordered_map<ImageFormat, std::pair<ConstantString, ConstantString>> imageFormatToSpirvType =
{
    { ImageFormat::Rgba16, { "Rgba16", "" }}
    , { ImageFormat::Rgb10_A2, { "Rgb10A2", "StorageImageExtendedFormats" } }
    , { ImageFormat::Rgba8, { "Rgba8", "" } }
    , { ImageFormat::Rg16, { "Rg16", "StorageImageExtendedFormats" } }
    , { ImageFormat::Rg8, { "Rg8", "StorageImageExtendedFormats" } }
    , { ImageFormat::R16, { "R16", "StorageImageExtendedFormats" } }
    , { ImageFormat::R8, { "R8", "StorageImageExtendedFormats" } }
    , { ImageFormat::Rgba16_Snorm, { "Rgba16Snorm", "StorageImageExtendedFormats" } }
    , { ImageFormat::Rgba8_Snorm, { "Rgba8Snorm", "" } }
    , { ImageFormat::Rg16_Snorm, { "Rg16Snorm", "StorageImageExtendedFormats" } }
    , { ImageFormat::Rg8_Snorm, { "Rg8Snorm", "StorageImageExtendedFormats" } }
    , { ImageFormat::R16_Snorm, { "R16Snorm", "StorageImageExtendedFormats" } }
    , { ImageFormat::R8_Snorm, { "R8Snorm", "StorageImageExtendedFormats" } }

    // float
    , { ImageFormat::Rgba32F, { "Rgba32f", "" } }
    , { ImageFormat::Rgba16F, { "Rgba16f", "" } }
    , { ImageFormat::Rg32F, { "Rg32f", "StorageImageExtendedFormats" } }
    , { ImageFormat::Rg16F, { "Rg16f", "StorageImageExtendedFormats" } }
    , { ImageFormat::R11G11B10F, { "R11fG11fB10f", "StorageImageExtendedFormats" } }
    , { ImageFormat::R32F, { "R32f", "" } }
    , { ImageFormat::R16F, { "R16f", "StorageImageExtendedFormats" } }

    // integer
    , { ImageFormat::Rgba32I, { "Rgba32i", "" } }
    , { ImageFormat::Rgba16I, { "Rgba16i", "" } }
    , { ImageFormat::Rgba8I, { "Rgba8i", ""  } }
    , { ImageFormat::Rg32I, { "Rg32i", "StorageImageExtendedFormats" } }
    , { ImageFormat::Rg16I, { "Rg16i", "StorageImageExtendedFormats" } }
    , { ImageFormat::Rg8I, { "Rg8i", "StorageImageExtendedFormats" } }
    , { ImageFormat::R32I, { "R32i", "" } }
    , { ImageFormat::R16I, { "R16i", "StorageImageExtendedFormats" } }
    , { ImageFormat::R8I, { "R8i", "StorageImageExtendedFormats" } }

    // unsigned integer
    , { ImageFormat::Rgba32U, { "Rgba32ui", "" } } 
    , { ImageFormat::Rgba16U, { "Rgba16ui", "" } } 
    , { ImageFormat::Rgb10_A2U, { "Rga10a2ui", "StorageImageExtendedFormats" } } 
    , { ImageFormat::Rgba8U, { "Rgba8ui", "" } } 
    , { ImageFormat::Rg32U, { "Rg32ui", "StorageImageExtendedFormats" } } 
    , { ImageFormat::Rg16U, { "Rg16ui", "StorageImageExtendedFormats" } } 
    , { ImageFormat::Rg8U, { "Rg8ui", "StorageImageExtendedFormats" } } 
    , { ImageFormat::R32U, { "R32ui", "" } } 
    , { ImageFormat::R16U, { "R16ui", "StorageImageExtendedFormats" } } 
    , { ImageFormat::R8U, { "R8ui", "StorageImageExtendedFormats" } } 
    , { ImageFormat::R64U, { "R64ui", "Int64ImageEXT" } } 
    , { ImageFormat::R64I, { "R64i", "Int64ImageEXT" } } 

    , { ImageFormat::Unknown, { "Unknown", "" } }
};

//------------------------------------------------------------------------------
/**
*/
uint32_t
GeneratePODTypeSPIRV(const Compiler* compiler, SPIRVGenerator* generator, TypeCode code, uint32_t vectorSize = 1, uint32_t rowSize = 1)
{
    auto it = scalarTable.find(code);
    assert(it != scalarTable.end());
    uint32_t baseType = generator->AddSymbol(std::get<0>(it->second), std::get<1>(it->second), true);
    
    // Matrix
    if (rowSize > 1)
    {
        assert(vectorSize > 1);
        baseType = generator->AddSymbol(TStr(std::get<0>(it->second), "x", rowSize), TStr::Separated("OpTypeVector", SPVArg(baseType), rowSize), true);
        baseType = generator->AddSymbol(TStr(std::get<0>(it->second), "x", rowSize, "x", vectorSize), TStr::Separated("OpTypeMatrix", SPVArg(baseType), vectorSize), true);
    }
    else
    {
        // Vector
        if (vectorSize > 1)
            baseType = generator->AddSymbol(TStr(std::get<0>(it->second), "x", vectorSize), TStr::Separated("OpTypeVector", SPVArg(baseType), vectorSize), true);
    }
    return baseType;   
}

//------------------------------------------------------------------------------
/**
*/
std::tuple<uint32_t, TStr>
GenerateBaseTypeSPIRV(const Compiler* compiler, SPIRVGenerator* generator, TypeCode code, uint32_t vectorSize = 1, uint32_t rowSize = 1)
{
    auto it = scalarTable.find(code);
    assert(it != scalarTable.end());
    auto [str, spirvstr, op, args] = it->second;
    uint32_t newBaseType = AddBaseType(generator, str, op, args);
    uint32_t baseType = generator->AddSymbol(std::get<0>(it->second), std::get<1>(it->second), true);
    TStr type = std::get<0>(it->second);
    
    // Matrix
    if (rowSize > 1)
    {
        assert(vectorSize > 1);
        TStr vecType = TStr(type, "x", rowSize);
        TStr matType = TStr(vecType, "x", vectorSize);
        AddSymbol(generator, vecType, SPVWriter::Section::Declarations, OpTypeVector, baseType, rowSize);
        AddSymbol(generator, vecType, SPVWriter::Section::Declarations, OpTypeMatrix, baseType, vectorSize);
        baseType = generator->AddSymbol(vecType, TStr::Separated("OpTypeVector", SPVArg(baseType), rowSize), true);
        baseType = generator->AddSymbol(matType, TStr::Separated("OpTypeMatrix", SPVArg(baseType), vectorSize), true);
        type = matType;
    }
    else if (vectorSize > 1)
    {
        // Vector
        TStr vecType = TStr(type, "x", vectorSize);
        AddSymbol(generator, vecType, SPVWriter::Section::Declarations, OpTypeVector, baseType, vectorSize);
        baseType = generator->AddSymbol(vecType, TStr::Separated("OpTypeVector", SPVArg(baseType), vectorSize), true);
        type = vecType;
    }
    return std::make_tuple(baseType, std::move(type));   
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult::Storage
ResolveSPIRVVariableStorage(
    Type::FullType type
    , Type* typeSymbol
    , Storage storage = Storage::Default
    , Variable::__Resolved::UsageBits usageBits = Variable::__Resolved::UsageBits(0x0)
    )
{
    SPIRVResult::Storage scope = SPIRVResult::Storage::Function;
    if (typeSymbol->category == Type::ScalarCategory || typeSymbol->category == Type::VoidCategory)
    {
        if (storage == Storage::Input)
            scope = SPIRVResult::Storage::Input;
        else if (storage == Storage::Output)
            scope = SPIRVResult::Storage::Output;
        else if (storage == Storage::Workgroup)
            scope = SPIRVResult::Storage::WorkGroup;
        else if (storage == Storage::LinkDefined)
            scope = SPIRVResult::Storage::UniformConstant;
        else if (storage == Storage::Global)
            scope = SPIRVResult::Storage::Private;
        else if (storage == Storage::Device)
            scope = SPIRVResult::Storage::Device;
    }
    else if (typeSymbol->category == Type::TextureCategory)
    {
        if (storage == Storage::Uniform)
        {
            if (type.mut)
                scope = SPIRVResult::Storage::MutableImage;
            else
                scope = SPIRVResult::Storage::Image;
        }
    }
    else if (typeSymbol->category == Type::PixelCacheCategory)
    {
        if (storage == Storage::Uniform)
            scope = SPIRVResult::Storage::Image;
    }
    else if (typeSymbol->category == Type::SamplerCategory)
    {
        if (storage == Storage::Uniform)
            scope = SPIRVResult::Storage::Sampler;
    }
    else if (typeSymbol->category == Type::EnumCategory)
    {
        if (storage == Storage::Global)
            scope = SPIRVResult::Storage::Private;
        else if (storage == Storage::Workgroup)
            scope = SPIRVResult::Storage::WorkGroup;
        else if (storage == Storage::Device)
            scope = SPIRVResult::Storage::Device;
    }
    else if (typeSymbol->category == Type::UserTypeCategory)
    {
        if (storage == Storage::Uniform && (type.IsMutable() || usageBits.flags.isDynamicSizedArray))
            scope = SPIRVResult::Storage::StorageBuffer;
        else if (storage == Storage::Uniform)
            scope = SPIRVResult::Storage::Uniform;
        else if (storage == Storage::InlineUniform)
            scope = SPIRVResult::Storage::PushConstant;
        else if (storage == Storage::Global)
            scope = SPIRVResult::Storage::Private;
        else if (storage == Storage::Workgroup)
            scope = SPIRVResult::Storage::WorkGroup;
        else if (storage == Storage::Device)
            scope = SPIRVResult::Storage::Device;
        else if (storage == Storage::RayPayload)
            scope = SPIRVResult::Storage::RayPayload;
        else if (storage == Storage::RayPayloadInput)
            scope = SPIRVResult::Storage::RayPayloadInput;
        else if (storage == Storage::RayHitAttribute)
            scope = SPIRVResult::Storage::RayHitAttribute;
        else if (storage == Storage::CallableData)
            scope = SPIRVResult::Storage::CallableData;
        else if (storage == Storage::CallableDataInput)
            scope = SPIRVResult::Storage::CallableDataInput;
    }
    return scope;
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateTypeSPIRV(
    const Compiler* compiler
    , SPIRVGenerator* generator
    , Type::FullType type
    , Type* typeSymbol
    , SPIRVResult::Storage storage = SPIRVResult::Storage::Function
    , bool isInterface = false
)
{
    std::tuple<uint32_t, TStr> baseType;
    std::vector<uint32_t> parentType;

    auto reducePointer = [](const Compiler* compiler, Type::FullType& type)
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

    if (typeSymbol->category == Type::ScalarCategory || typeSymbol->category == Type::VoidCategory)
    {
        baseType = GenerateBaseTypeSPIRV(compiler, generator, typeSymbol->baseType, typeSymbol->columnSize, typeSymbol->rowSize);
    }
    else if (typeSymbol->category == Type::TextureCategory)
    {
        uint32_t floatType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::Float, 1);
        auto handleTypeIt = handleTable.find(typeSymbol->baseType);
        auto [gpulangType, spirvFormatter] = handleTypeIt->second;

        std::string accessPattern = "";
        uint32_t sampleBits, depthBits = 0;
        if (type.IsMutable())
            sampleBits = 2;
        else
            sampleBits = 1;

        if (typeSymbol->baseType == TypeCode::Texture1D || typeSymbol->baseType == TypeCode::Texture1DArray)
            generator->AddCapability("Sampled1D");

        auto [spirvFormat, extension] = imageFormatToSpirvType[type.imageFormat];
        if (extension.size > 0)
            generator->AddCapability(extension);
        TStr ty = spirvFormatter(floatType, depthBits, sampleBits, spirvFormat.buf);
        TStr gpuLangStr = TStr::Compact(spirvFormat, "_", gpulangType, "Sample_", sampleBits, "Depth_", depthBits);
        uint32_t name = generator->AddSymbol(gpuLangStr, ty, true);
        baseType = std::tie(name, gpulangType);
        if (type.sampled)
        {
            TStr sampledImageStr = TStr("sampled_", gpuLangStr);
            name = generator->AddSymbol(sampledImageStr, TStr::Separated("OpTypeSampledImage", SPVArg(name)), true);
        }
        baseType = std::tie(name, gpulangType);
    }
    else if (typeSymbol->category == Type::PixelCacheCategory)
    {
        generator->AddCapability("InputAttachment");
        auto [floatType, floatBaseType] = GenerateBaseTypeSPIRV(compiler, generator, TypeCode::Float, 1);

        auto handleTypeIt = handleTable.find(typeSymbol->baseType);
        auto [gpulangType, spirvFormatter] = handleTypeIt->second;
        TStr ty = spirvFormatter(floatType, 0, 0, nullptr);
        uint32_t name = generator->AddSymbol(gpulangType, ty, true);
        baseType = std::tie(name, gpulangType);
    }
    else if (typeSymbol->category == Type::SamplerCategory)
    {
        auto handleTypeIt = handleTable.find(typeSymbol->baseType);
        auto [gpulangType, spirvFormatter] = handleTypeIt->second;
        uint32_t name = generator->AddSymbol(gpulangType, spirvFormatter(0, 0, 0, nullptr), true);
        baseType = std::tie(name, gpulangType);
    }
    else if (typeSymbol->category == Type::EnumCategory)
    {
        baseType = GenerateBaseTypeSPIRV(compiler, generator, TypeCode::UInt, 1);
    }
    else if (typeSymbol->category == Type::UserTypeCategory)
    {
        uint32_t name = generator->GetSymbol(typeSymbol->name).value;
        baseType = std::tie(name, typeSymbol->name);
    }

    bool isStructPadded = false;

    ConstantString scopeString = SPIRVResult::ScopeToString(storage);
    for (size_t i = 0; i < type.modifiers.size(); i++)
    {
        auto [typeName, gpulangType] = baseType; 
        const Type::FullType::Modifier& mod = type.modifiers[i];
        if (mod == Type::FullType::Modifier::Pointer)
        {
            TStr newBase = TStr("ptr_", gpulangType, "_", scopeString);
            parentType.push_back(typeName);
            typeName = generator->AddSymbol(newBase, TStr::Separated("OpTypePointer", scopeString, SPVArg(typeName)), true);
            baseType = std::tie(typeName, newBase);
        }
        else if (mod == Type::FullType::Modifier::Array)
        {
            if (type.modifierValues[i] == nullptr)
            {
                TStr newBase = TStr("[]_", gpulangType, scopeString);
                parentType.push_back(typeName);
                typeName = generator->AddSymbol(newBase, TStr::Separated("OpTypeRuntimeArray", SPVArg(typeName)), true);
                if (typeSymbol->category == Type::UserTypeCategory)
                {
                    Structure::__Resolved* strucRes = Symbol::Resolved(static_cast<Structure*>(typeSymbol));
                    generator->AddDecoration(newBase, typeName, TStr::Separated("ArrayStride", strucRes->byteSize));
                }
                else
                {
                    generator->AddDecoration(newBase, typeName, TStr::Separated("ArrayStride", typeSymbol->CalculateStride()));
                }
                newBase = TStr::Compact("struct_", newBase);
                typeName = generator->AddSymbol(newBase, TStr::Separated("OpTypeStruct", SPVArg(typeName)), true);
                baseType = std::tie(typeName, newBase);
                generator->writer->MemberDecorate(SPVArg(typeName), 0, Decorations::Offset, 0);
                generator->AddMemberDecoration(typeName, 0, "Offset 0");
                storage = SPIRVResult::Storage::StorageBuffer;
                scopeString = SPIRVResult::ScopeToString(storage);
                isStructPadded = true;
            }
            else
            {
                uint32_t size;
                ValueUnion val;
                type.modifierValues[i]->EvalValue(val);
                val.Store(size);
                TStr newBase = TStr("[", size, "]_", gpulangType);
                parentType.push_back(typeName);
                uint32_t intType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::Int, 1);
                uint32_t arraySizeConstant = generator->AddSymbol(TStr::Compact(size, "i"), TStr::Separated("OpConstant", SPVArg(intType), size), true);
                typeName = generator->AddSymbol(newBase, TStr::Separated("OpTypeArray", SPVArg(typeName), SPVArg(arraySizeConstant)), true);
                baseType = std::tie(typeName, newBase);
                if (typeSymbol->category == Type::UserTypeCategory)
                {
                    Structure::__Resolved* strucRes = Symbol::Resolved(static_cast<Structure*>(typeSymbol));
                    generator->AddDecoration(newBase, typeName, TStr::Separated("ArrayStride", strucRes->byteSize));
                }
                else
                {
                    generator->AddDecoration(newBase, typeName, TStr::Separated("ArrayStride", typeSymbol->CalculateStride()));
                }

                // if this is an interface, wrap it in a struct to allow interface binding
                if (isInterface)
                {
                    newBase = TStr("struct_", newBase);
                    typeName = generator->AddSymbol(newBase, TStr::Separated("OpTypeStruct", SPVArg(typeName)), true);
                    baseType = std::tie(typeName, newBase);
                    generator->writer->MemberDecorate(SPVArg(typeName), 0, Decorations::Offset, 0);
                    generator->AddMemberDecoration(typeName, 0, "Offset 0");
                    isStructPadded = true;
                }
            }
        }
    }

    auto ret = SPIRVResult(0xFFFFFFFF, std::get<0>(baseType), false, false, storage, parentType);
    ret.isStructPadded = isStructPadded;
    return ret;
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
TStr
SwizzleMaskToIndices(const Type::SwizzleMask mask, bool offset = false)
{
    TStr swizzleIndices;
    if (mask.bits.x != Type::SwizzleMask::Component::Invalid)
        swizzleIndices.Concatenate<false>(mask.bits.x + (offset ? 4 : 0), " ");
    if (mask.bits.y != Type::SwizzleMask::Component::Invalid)
        swizzleIndices.Concatenate<false>(mask.bits.y + (offset ? 4 : 0), " ");
    if (mask.bits.z != Type::SwizzleMask::Component::Invalid)
        swizzleIndices.Concatenate<false>(mask.bits.z + (offset ? 4 : 0), " ");
    if (mask.bits.w != Type::SwizzleMask::Component::Invalid)
        swizzleIndices.Concatenate<false>(mask.bits.w + (offset ? 4 : 0), " ");
    return swizzleIndices;
}


//------------------------------------------------------------------------------
/**
*/
void
SwizzleMaskIndices(const Type::SwizzleMask mask, uint32_t* indices, uint8_t& numWritten, bool offset = false)
{
    if (mask.bits.x != Type::SwizzleMask::Component::Invalid)
        indices[numWritten++] = mask.bits.x + (offset ? 4 : 0);
    if (mask.bits.y != Type::SwizzleMask::Component::Invalid)
        indices[numWritten++] = mask.bits.y + (offset ? 4 : 0);
    if (mask.bits.z != Type::SwizzleMask::Component::Invalid)
        indices[numWritten++] = mask.bits.z + (offset ? 4 : 0);
    if (mask.bits.w != Type::SwizzleMask::Component::Invalid)
        indices[numWritten++] = mask.bits.w + (offset ? 4 : 0);
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateConstantSPIRV(const Compiler* compiler, SPIRVGenerator* generator, ConstantCreationInfo info, uint32_t vectorSize = 1)
{
    SPIRVResult res = SPIRVResult::Invalid();
    uint32_t baseType;
    uint32_t vecType = 0xFFFFFFFF;

#define ADD_CONSTANT(short, ty) \
if (generator->linkDefineEvaluation)\
{\
    TStr symbolName = TStr(info.data.ty, "_", #short"_link-defined", generator->linkDefineSlot);\
    AddSymbol(generator, symbolName, SPVWriter::Section::Declarations, OpSpecConstant, baseType, info.data.ty);\
    res.name = generator->AddSymbol(symbolName, TStr::Separated("OpSpecConstant", SPVArg(baseType), info.data.ty), true);\
}\
else\
{\
    TStr symbolName = TStr(info.data.ty, "_", #short);\
    AddSymbol(generator, symbolName, SPVWriter::Section::Declarations, OpConstant, baseType, info.data.ty);\
    res.name = generator->AddSymbol(symbolName, TStr::Separated("OpConstant", SPVArg(baseType), info.data.ty), true);\
}
    

    switch (info.type)
    {
        case ConstantCreationInfo::Type::UInt:
        {
            baseType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::UInt, 1);
            if (vectorSize > 1)
                vecType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::UInt, vectorSize);
            res.typeName = baseType;
            ADD_CONSTANT(u32, ui)
            break;
        }
        case ConstantCreationInfo::Type::UInt16:
        {
            baseType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::UInt16, 1);
            if (vectorSize > 1)
                vecType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::UInt16, vectorSize);
            res.typeName = baseType;
            ADD_CONSTANT(u16, ui)
            break;
        }
        case ConstantCreationInfo::Type::Int:
        {
            baseType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::Int, 1);
            if (vectorSize > 1)
                vecType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::Int, vectorSize);
            res.typeName = baseType;
            ADD_CONSTANT(i32, i)
            break;
        }
        case ConstantCreationInfo::Type::Int16:
        {
            baseType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::Int16, 1);
            if (vectorSize > 1)
                vecType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::Int16, vectorSize);
            res.typeName = baseType;
            ADD_CONSTANT(i16, i)
            break;
        }
        case ConstantCreationInfo::Type::Float:
        {
            baseType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::Float, 1);
            if (vectorSize > 1)
                vecType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::Float, vectorSize);
            res.typeName = baseType;
            ADD_CONSTANT(f32, f)
            break;
        }
        case ConstantCreationInfo::Type::Float16:
        {
            baseType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::Float16, 1);
            if (vectorSize > 1)
                vecType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::Float16, vectorSize);
            res.typeName = baseType;
            ADD_CONSTANT(f16, f)
            break;
        }
        case ConstantCreationInfo::Type::Bool:
        {
            baseType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::Bool, 1);
            if (vectorSize > 1)
                vecType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::Bool, vectorSize);
            res.typeName = baseType;
            if (generator->linkDefineEvaluation)
            {
                TStr symbolName = TStr::Compact(info.data.b ? "true" : "false", "_", "link_defined", generator->linkDefineSlot);
                AddSymbol(generator, symbolName, SPVWriter::Section::Declarations, info.data.b ? OpSpecConstantTrue : OpSpecConstantFalse, baseType);
                res.name = generator->AddSymbol(TStr::Compact(info.data.b ? "true" : "false", "_", "link_defined", generator->linkDefineSlot), TStr::Separated(info.data.b ? "OpSpecConstantTrue" : "OpSpecConstantFalse", SPVArg(baseType)), true);
            }
            else
            {
                TStr symbolName = TStr::Compact(info.data.b ? "true" : "false");
                AddSymbol(generator, symbolName, SPVWriter::Section::Declarations, info.data.b ? OpConstantTrue : OpConstantFalse, baseType);
                res.name = generator->AddSymbol(TStr::Compact(info.data.b ? "true" : "false"), TStr::Separated(info.data.b ? "OpConstantTrue" : "OpConstantFalse", SPVArg(baseType)), true);
            }
            break;
        }
    }
    if (vectorSize > 1)
    {
        TStr name;
        for (int i = 0; i < vectorSize; i++)
        {
            name.Append(SPVArg(res.name));
            if (i < vectorSize - 1)
                name.Append(" ");
        }
        if (generator->linkDefineEvaluation)
            res.name = generator->AddSymbol(TStr::Compact("{", name, "}"), TStr::Separated("OpSpecConstantComposite", SPVArg(vecType), name), true);
        else
            res.name = generator->AddSymbol(TStr::Compact("{", name, "}"), TStr::Separated("OpConstantComposite", SPVArg(vecType), name), true);
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
GeneratePointerTypeSPIRV(
    const Compiler* compiler
    , SPIRVGenerator* generator
    , const Type::FullType& type
    , Type* typeSymbol
    , SPIRVResult::Storage storage = SPIRVResult::Storage::Function
    , bool isInterface = false
    )
{
    Type::FullType returnPtrType = type;
    if (!type.IsPointer())
        returnPtrType.AddModifier(Type::FullType::Modifier::Pointer);
    SPIRVResult returnType = GenerateTypeSPIRV(compiler, generator, returnPtrType, typeSymbol, storage, isInterface);

    return returnType;
}

//------------------------------------------------------------------------------
/**
*/
uint32_t
AccessChainSPIRV(const Compiler* compiler, SPIRVGenerator* generator, uint32_t name, uint32_t type, const std::vector<SPIRVResult>& chain)
{
    uint32_t val = name;
    
    /// Resolve access chain
    if (!chain.empty())
    {
        generator->writer->MappedInstruction(OpAccessChain, SPVWriter::Section::LocalFunction, type, SPVArg(val), chain);
        TStr accessChain = TStr::Separated("OpAccessChain", SPVArg(type), SPVArg(val));
        for (const auto& index : chain)
        {
            accessChain = TStr::Separated(accessChain, SPVArg(index.name));
        }
        val = generator->AddMappedOp(accessChain);
    }
    return val;
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
LoadValueSPIRV(const Compiler* compiler, SPIRVGenerator* generator, SPIRVResult arg, bool loadParentType = false)
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
    uint32_t type = arg.typeName;
    bool isValue = arg.isValue;
    assert(arg.derefs > 0 ? !arg.isValue : true);

    for (uint32_t i = 0; i < arg.derefs; i++)
    {
        type = arg.parentTypes.back();
        arg.parentTypes.pop_back();
        generator->writer->MappedInstruction(OpLoad, SPVWriter::Section::LocalFunction, type, SPVArg(val));
        val = generator->AddMappedOp(TStr::Separated("OpLoad", SPVArg(type), SPVArg(val)));
        arg.isValue = arg.parentTypes.empty();
    }

    val = AccessChainSPIRV(compiler, generator, val, type, arg.accessChain);
    
    assert(val != -1);
    if (!arg.isValue)
    {
        // The value loaded should be the parent type, as the current type has to be a pointer
        assert(!arg.parentTypes.empty());
        type = arg.parentTypes.back();
        arg.parentTypes.pop_back();
        generator->writer->MappedInstruction(OpLoad, SPVWriter::Section::LocalFunction, type, SPVArg(val));
        val = generator->AddMappedOp(TStr::Separated("OpLoad", SPVArg(type), SPVArg(val)));
    }
    
    assert(val != -1);
    if (arg.swizzleMask != Type::SwizzleMask())
    {
        // Can't swizzle a pointer
        uint32_t swizzleType = arg.swizzleType;
        if (Type::SwizzleMaskComponents(arg.swizzleMask) == 1)
        {
            generator->writer->MappedInstruction(OpCompositeExtract, SPVWriter::Section::LocalFunction, swizzleType, SPVArg(val), arg.swizzleMask.bits.x);
            val = generator->AddMappedOp(TStr::Separated("OpCompositeExtract", SPVArg(swizzleType), SPVArg(val), arg.swizzleMask.bits.x));
            type = swizzleType;
        }
        else
        {
            uint32_t indices[4];
            uint8_t written = 0;
            SwizzleMaskIndices(arg.swizzleMask, indices, written);
            generator->writer->MappedInstruction(OpVectorShuffle, SPVWriter::Section::LocalFunction, swizzleType, SPVArg(val), SPVArg(val), SPVLiteralList{ .vals = indices, .num = written });

            TStr swizzleIndices = SwizzleMaskToIndices(arg.swizzleMask);
            val = generator->AddMappedOp(TStr::Separated("OpVectorShuffle", SPVArg(swizzleType), SPVArg(val), SPVArg(val), swizzleIndices));
            type = swizzleType;
        }
    }
    auto res = SPIRVResult(val, type);
    res.isValue = arg.parentTypes.empty(); // The result is only a value if there are no more parent types (indirections)
    res.isConst = arg.isConst;
    res.isSpecialization = arg.isSpecialization;
    res.parentTypes = arg.parentTypes;
    res.derefs = 0;
    return res;
}

//------------------------------------------------------------------------------
/**
*/
void
StoreValueSPIRV(const Compiler* compiler, SPIRVGenerator* generator, SPIRVResult target, SPIRVResult source)
{
    uint32_t val = target.name;
    uint32_t type = target.typeName;

    for (uint32_t i = 1; i < target.derefs; i++)
    {
        type = target.parentTypes.back();
        target.parentTypes.pop_back();
        generator->writer->MappedInstruction(OpLoad, SPVWriter::Section::LocalFunction, type, SPVArg(val));
        val = generator->AddMappedOp(TStr::Separated("OpLoad", SPVArg(type), SPVArg(val)));
    }

    
    val = AccessChainSPIRV(compiler, generator, val, type, target.accessChain);
    
    /// Resolve access chain
    /*
    if (!target.accessChain.empty())
    {
        std::string accessChain = Format("OpAccessChain %%%d %%%d", type, val);
        for (const auto& index : target.accessChain)
        {
            accessChain = Format("%s %%%d", accessChain.c_str(), index.name);
        }
        val = generator->AddMappedOp(accessChain);
    }
    */

    // Perform OpStore if source is a value, otherwise copy memory
    if (source.isValue)
    {
        generator->writer->Instruction(OpStore, SPVWriter::Section::LocalFunction, SPVArg(val), SPVArg(source.name));
        generator->AddOp(TStr::Separated("OpStore", SPVArg(val), SPVArg(source.name)));
    }
    else
    {
        generator->writer->Instruction(OpCopyMemory, SPVWriter::Section::LocalFunction, SPVArg(val), SPVArg(source.name));
        generator->AddOp(TStr::Separated("OpCopyMemory", SPVArg(val), SPVArg(source.name)));
    }

}

enum class ConversionTable
{
    None,
    IntToFloat,
    IntToUInt,
    IntToBool,
    UIntToFloat,
    UIntToInt,
    UIntToBool,
    FloatToUInt,
    FloatToInt,
    BoolToInt,
    BoolToUInt
};

std::unordered_map<ConversionTable, std::function<SPIRVResult(const Compiler*, SPIRVGenerator*, uint32_t, SPIRVResult)>> converters =
{
    { ConversionTable::IntToFloat, [](const Compiler* c, SPIRVGenerator* g, uint32_t vectorSize, SPIRVResult value) -> SPIRVResult {
        uint32_t type = GeneratePODTypeSPIRV(c, g, TypeCode::Float);
        if (value.isLiteral)
        {
            assert(vectorSize == 1);
            return GenerateConstantSPIRV(c, g, ConstantCreationInfo::Float(value.literalValue.i));
        }
        else
        {
            if (vectorSize > 1)
                type = GeneratePODTypeSPIRV(c, g, TypeCode::Float, vectorSize);
            value = LoadValueSPIRV(c, g, value);
            g->writer->MappedInstruction(OpConvertSToF, SPVWriter::Section::LocalFunction, type, value);
            return SPIRVResult(g->AddMappedOp(Format("OpConvertSToF %%%d %%%d", type, value.name)), type, true);
        }
    } }
    , { ConversionTable::IntToUInt, [](const Compiler* c, SPIRVGenerator* g, uint32_t vectorSize, SPIRVResult value) -> SPIRVResult {
        uint32_t type = GeneratePODTypeSPIRV(c, g, TypeCode::UInt);
        if (value.isLiteral)
        {
            assert(vectorSize == 1);
            return GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(value.literalValue.i));
        }
        else
        {
            if (vectorSize > 1)
                type = GeneratePODTypeSPIRV(c, g, TypeCode::UInt, vectorSize);
            value = LoadValueSPIRV(c, g, value);
            g->writer->MappedInstruction(OpBitcast, SPVWriter::Section::LocalFunction, type, value);
            return SPIRVResult(g->AddMappedOp(Format("OpBitcast %%%d %%%d", type, value.name)), type, true);
        }
    } }
    , { ConversionTable::IntToBool, [](const Compiler* c, SPIRVGenerator* g, uint32_t vectorSize, SPIRVResult value) -> SPIRVResult {
        uint32_t type = GeneratePODTypeSPIRV(c, g, TypeCode::Bool);
        if (value.isLiteral)
        {
            assert(vectorSize == 1);
            return GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(value.literalValue.i));
        }
        else
        {
            if (vectorSize > 1)
                type = GeneratePODTypeSPIRV(c, g, TypeCode::Bool, vectorSize);
            value = LoadValueSPIRV(c, g, value);

            // First, test if integer is either 0 or 1 by comparing it to 
            SPIRVResult falseValue = GenerateConstantSPIRV(c, g, ConstantCreationInfo::Int(0), vectorSize);
            g->writer->MappedInstruction(OpINotEqual, SPVWriter::Section::LocalFunction, type, value, falseValue);
            uint32_t res = g->AddMappedOp(Format("OpINotEqual %%%d %%%d %%%d", type, value.name, falseValue.name));
            return SPIRVResult(res, type, true);
        }
    } }
    , { ConversionTable::UIntToInt, [](const Compiler* c, SPIRVGenerator* g, uint32_t vectorSize, SPIRVResult value) -> SPIRVResult {
        uint32_t type = GeneratePODTypeSPIRV(c, g, TypeCode::Int);
        if (value.isLiteral)
        {
            assert(vectorSize == 1);
            return GenerateConstantSPIRV(c, g, ConstantCreationInfo::Int(value.literalValue.ui));
        }
        else
        {
            if (vectorSize > 1)
                type = GeneratePODTypeSPIRV(c, g, TypeCode::Int, vectorSize);
            value = LoadValueSPIRV(c, g, value);
            g->writer->MappedInstruction(OpBitcast, SPVWriter::Section::LocalFunction, type, value);
            return SPIRVResult(g->AddMappedOp(Format("OpBitcast %%%d %%%d", type, value.name)), type, true);
        }
    } }
    , { ConversionTable::UIntToFloat, [](const Compiler* c, SPIRVGenerator* g, uint32_t vectorSize, SPIRVResult value) -> SPIRVResult {
        uint32_t type = GeneratePODTypeSPIRV(c, g, TypeCode::Float);
        if (value.isLiteral)
        {
            assert(vectorSize == 1);
            return GenerateConstantSPIRV(c, g, ConstantCreationInfo::Float(value.literalValue.ui));
        }
        else
        {
            if (vectorSize > 1)
                type = GeneratePODTypeSPIRV(c, g, TypeCode::Float, vectorSize);
            value = LoadValueSPIRV(c, g, value);
            g->writer->MappedInstruction(OpConvertUToF, SPVWriter::Section::LocalFunction, type, value);
            return SPIRVResult(g->AddMappedOp(Format("OpConvertUToF %%%d %%%d", type, value.name)), type, true);
        }
    } }
    , { ConversionTable::UIntToBool, [](const Compiler* c, SPIRVGenerator* g, uint32_t vectorSize, SPIRVResult value) -> SPIRVResult {
        uint32_t type = GeneratePODTypeSPIRV(c, g, TypeCode::Bool);
        if (value.isLiteral)
        {
            assert(vectorSize == 1);
            return GenerateConstantSPIRV(c, g, ConstantCreationInfo::Bool(value.literalValue.ui));
        }
        else
        {
            if (vectorSize > 1)
                type = GeneratePODTypeSPIRV(c, g, TypeCode::Bool, vectorSize);
            value = LoadValueSPIRV(c, g, value);

            // First, test if integer is either 0 or 1 by comparing it to 
            SPIRVResult falseValue = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(0), vectorSize);
            uint32_t res = g->AddMappedOp(Format("OpINotEqual %%%d %%%d %%%d", type, value.name, falseValue.name));
            return SPIRVResult(res, type, true);
        }
    } }
    , { ConversionTable::FloatToUInt, [](const Compiler* c, SPIRVGenerator* g, uint32_t vectorSize, SPIRVResult value) -> SPIRVResult {
        uint32_t type = GeneratePODTypeSPIRV(c, g, TypeCode::UInt);
        if (value.isLiteral)
        {
            assert(vectorSize == 1);
            return GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(value.literalValue.f));
        }
        else
        {
            if (vectorSize > 1)
                type = GeneratePODTypeSPIRV(c, g, TypeCode::UInt, vectorSize);
            value = LoadValueSPIRV(c, g, value);
            g->writer->MappedInstruction(OpConvertFToU, SPVWriter::Section::LocalFunction, type, value);
            return SPIRVResult(g->AddMappedOp(Format("OpConvertFToU %%%d %%%d", type, value.name)), type, true);
        }
    } }
    , { ConversionTable::FloatToInt, [](const Compiler* c, SPIRVGenerator* g, uint32_t vectorSize, SPIRVResult value) -> SPIRVResult {
        uint32_t type = GeneratePODTypeSPIRV(c, g, TypeCode::Int);
        if (value.isLiteral)
        {
            assert(vectorSize == 1);
            return GenerateConstantSPIRV(c, g, ConstantCreationInfo::Int(value.literalValue.f));
        }
        else
        {
            if (vectorSize > 1)
                type = GeneratePODTypeSPIRV(c, g, TypeCode::Int, vectorSize);
            value = LoadValueSPIRV(c, g, value);
            g->writer->MappedInstruction(OpConvertFToS, SPVWriter::Section::LocalFunction, type, value);
            return SPIRVResult(g->AddMappedOp(Format("OpConvertFToS %%%d %%%d", type, value.name)), type, true);
        }
    } }
    , { ConversionTable::BoolToInt, [](const Compiler* c, SPIRVGenerator* g, uint32_t vectorSize, SPIRVResult value) -> SPIRVResult {
        uint32_t type = GeneratePODTypeSPIRV(c, g, TypeCode::Int);
        if (value.isLiteral)
        {
            assert(vectorSize == 1);
            return GenerateConstantSPIRV(c, g, ConstantCreationInfo::Int(value.literalValue.b));
        }
        else
        {
            if (vectorSize > 1)
                type = GeneratePODTypeSPIRV(c, g, TypeCode::Int, vectorSize);
            value = LoadValueSPIRV(c, g, value);
            SPIRVResult trueValue = GenerateConstantSPIRV(c, g, ConstantCreationInfo::Int(1), vectorSize);
            SPIRVResult falseValue = GenerateConstantSPIRV(c, g, ConstantCreationInfo::Int(0), vectorSize);
            g->writer->MappedInstruction(OpSelect, SPVWriter::Section::LocalFunction, type, value, trueValue, falseValue);
            return SPIRVResult(g->AddMappedOp(Format("OpSelect %%%d %%%d %%%d %%%d", type, value.name, trueValue.name, falseValue.name)), type, true);
        }
    } }
    , { ConversionTable::BoolToUInt, [](const Compiler* c, SPIRVGenerator* g, uint32_t vectorSize, SPIRVResult value) -> SPIRVResult {
        uint32_t type = GeneratePODTypeSPIRV(c, g, TypeCode::UInt);
        if (value.isLiteral)
        {
            assert(vectorSize == 1);
            return GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(value.literalValue.b));
        }
        else
        {
            if (vectorSize > 1)
                type = GeneratePODTypeSPIRV(c, g, TypeCode::UInt, vectorSize);
            value = LoadValueSPIRV(c, g, value);
            SPIRVResult trueValue = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(1), vectorSize);
            SPIRVResult falseValue = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(0), vectorSize);
            g->writer->MappedInstruction(OpSelect, SPVWriter::Section::LocalFunction, type, value, trueValue, falseValue);
            return SPIRVResult(g->AddMappedOp(Format("OpSelect %%%d %%%d %%%d %%%d", type, value.name, trueValue.name, falseValue.name)), type, true);
        }
    } }
};

//------------------------------------------------------------------------------
/**
*/
void
GenerateConversionsSPIRV(const Compiler* compiler, SPIRVGenerator* generator, ConversionTable conversion, uint32_t vectorSize, const std::vector<SPIRVResult>& inArgs, std::vector<SPIRVResult>& outValues)
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
GenerateConversionSPIRV(const Compiler* compiler, SPIRVGenerator* generator, ConversionTable conversion, uint32_t vectorSize, SPIRVResult inArg)
{
    return converters[conversion](compiler, generator, vectorSize, inArg);
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateCompositeSPIRV(const Compiler* compiler, SPIRVGenerator* generator, uint32_t returnType, const std::vector<SPIRVResult>& args)
{
    TStr argList;
    bool isConst = true;
    bool isSpecialization = false;
    SPIRVResult* loadedArgs = AllocStack<SPIRVResult>(args.size());
    uint8_t argCounter = 0;
    for (const SPIRVResult& arg : args)
    {
        SPIRVResult loaded = LoadValueSPIRV(compiler, generator, arg);
        loadedArgs[argCounter++] = loaded;
        argList.Concatenate<false>(SPVArg(loaded.name), " ");
        if (!loaded.isConst)
            isConst = false;
        isSpecialization |= loaded.isSpecialization;
    }
    if (isConst)
    {
        DeallocStack(args.size(), loadedArgs);
        if (isSpecialization)
            return SPIRVResult(generator->AddSymbol(TStr::Compact(SPVArg(returnType), "_", "composite", "_", argList), TStr::Separated("OpSpecConstantComposite", SPVArg(returnType), argList), true), returnType, true, true);
        else
            return SPIRVResult(generator->AddSymbol(TStr::Compact(SPVArg(returnType), "_", "composite", "_", argList), TStr::Separated("OpConstantComposite", SPVArg(returnType), argList), true), returnType, true, true);
    }
    else
    {
        assert(!generator->linkDefineEvaluation);
        generator->writer->MappedInstruction(OpCompositeConstruct, SPVWriter::Section::LocalFunction, returnType, SPVResultList{ .vals = loadedArgs, .num = argCounter });
        DeallocStack(args.size(), loadedArgs);
        return SPIRVResult(generator->AddMappedOp(TStr::Separated("OpCompositeConstruct", SPVArg(returnType), argList)), returnType, true);
    }
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateSplatCompositeSPIRV(const Compiler* compiler, SPIRVGenerator* generator, uint32_t returnType, uint32_t num, SPIRVResult arg)
{
    std::vector<SPIRVResult> splat(num, arg);
    assert(num > 1);
    return GenerateCompositeSPIRV(compiler, generator, returnType, splat);
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateConvertAndSplatCompositeSPIRV(const Compiler* compiler, SPIRVGenerator* generator, uint32_t returnType, const std::vector<SPIRVResult>& args, ConversionTable conversion)
{
    std::vector<SPIRVResult> converted;
    GenerateConversionsSPIRV(compiler, generator, conversion, 1, args, converted);
    return GenerateSplatCompositeSPIRV(compiler, generator, returnType, 4, converted[0]);
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateCompositeExtractSPIRV(const Compiler* compiler, SPIRVGenerator* generator, uint32_t returnType, uint32_t arg, uint32_t index)
{
    generator->writer->MappedInstruction(OpCompositeExtract, SPVWriter::Section::LocalFunction, returnType, SPVArg(arg), SPVArg(index));
    return SPIRVResult(generator->AddMappedOp(TStr::Separated("OpCompositeExtract", SPVArg(returnType), SPVArg(arg), SPVArg(index))), returnType, true);
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateCompositeInsertSPIRV(const Compiler* compiler, SPIRVGenerator* generator, uint32_t returnType, uint32_t arg, uint32_t index, uint32_t val)
{
    generator->writer->MappedInstruction(OpCompositeInsert, SPVWriter::Section::LocalFunction, returnType, SPVArg(arg), SPVArg(index));
    return SPIRVResult(generator->AddMappedOp(TStr::Separated("OpCompositeInsert", SPVArg(returnType), SPVArg(arg), SPVArg(index))), returnType, true );
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateConvertAndCompositeSPIRV(const Compiler* compiler, SPIRVGenerator* generator, uint32_t returnType, const std::vector<SPIRVResult>& args, ConversionTable conversion)
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
    if (SPIRVGenerator::IntrinsicMap.empty())
        SetupIntrinsics();
    this->writer = Alloc<SPVWriter>();
    this->writer->mode = SPVWriter::Mode::Text;
}

std::unordered_map<Function*, SPIRVGenerator::FunctionToSPIRVMapping> SPIRVGenerator::IntrinsicMap;

//------------------------------------------------------------------------------
/**
*/
void
SPIRVGenerator::SetupIntrinsics()
{
    SPIRVGenerator::IntrinsicMap[&Float4::ctor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 4);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    SPIRVGenerator::IntrinsicMap[&Float3::ctor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 3);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    SPIRVGenerator::IntrinsicMap[&Float2::ctor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 2);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    SPIRVGenerator::IntrinsicMap[&Float::ctor_UInt] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateConversionSPIRV(c, g, ConversionTable::UIntToFloat, 1, args[0]);
    };
    SPIRVGenerator::IntrinsicMap[&Float::ctor_Int] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateConversionSPIRV(c, g, ConversionTable::IntToFloat, 1, args[0]);
    };
    SPIRVGenerator::IntrinsicMap[&Int4::ctor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 4);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    SPIRVGenerator::IntrinsicMap[&Int3::ctor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 3);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    SPIRVGenerator::IntrinsicMap[&Int2::ctor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 2);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    SPIRVGenerator::IntrinsicMap[&Int::ctor_UInt] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateConversionSPIRV(c, g, ConversionTable::UIntToInt, 1, args[0]);
    };
    SPIRVGenerator::IntrinsicMap[&Int::ctor_Bool] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateConversionSPIRV(c, g, ConversionTable::BoolToInt, 1, args[0]);
    };
    SPIRVGenerator::IntrinsicMap[&Int::ctor_Float] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateConversionSPIRV(c, g, ConversionTable::FloatToInt, 1, args[0]);
    };
    SPIRVGenerator::IntrinsicMap[&UInt4::ctor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 4);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    SPIRVGenerator::IntrinsicMap[&UInt3::ctor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 3);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    SPIRVGenerator::IntrinsicMap[&UInt2::ctor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 2);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    SPIRVGenerator::IntrinsicMap[&UInt::ctor_Int] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateConversionSPIRV(c, g, ConversionTable::IntToUInt, 1, args[0]);
    };
    SPIRVGenerator::IntrinsicMap[&UInt::ctor_Bool] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateConversionSPIRV(c, g, ConversionTable::BoolToUInt, 1, args[0]);
    };
    SPIRVGenerator::IntrinsicMap[&UInt::ctor_Float] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateConversionSPIRV(c, g, ConversionTable::FloatToUInt, 1, args[0]);
    };
    SPIRVGenerator::IntrinsicMap[&Bool4::ctor_XYZW] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 4);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    SPIRVGenerator::IntrinsicMap[&Bool4::ctorSingleValue] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateSplatCompositeSPIRV(c, g, returnType, 4, args[0]);
    };
    SPIRVGenerator::IntrinsicMap[&Bool3::ctor_XYZ] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 3);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    SPIRVGenerator::IntrinsicMap[&Bool3::ctorSingleValue] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateSplatCompositeSPIRV(c, g, returnType, 3, args[0]);
    };
    SPIRVGenerator::IntrinsicMap[&Bool2::ctor_XY] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 2);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    SPIRVGenerator::IntrinsicMap[&Bool2::ctorSingleValue] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateSplatCompositeSPIRV(c, g, returnType, 2, args[0]);
    };
    SPIRVGenerator::IntrinsicMap[&Bool::ctor_UInt] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        SPIRVResult val = LoadValueSPIRV(c, g, args[0]);
        uint32_t type = GeneratePODTypeSPIRV(c, g, TypeCode::Bool);
        
        // First, test if integer is either 0 or 1 by comparing it to 
        SPIRVResult falseValue = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(0), 1);
        g->writer->MappedInstruction(OpINotEqual, SPVWriter::Section::LocalFunction, type, val, falseValue);
        uint32_t res = g->AddMappedOp(TStr::Separated("OpINotEqual", SPVArg(type), SPVArg(val.name), SPVArg(falseValue.name)));
        return SPIRVResult(res, type, true);
    };
    SPIRVGenerator::IntrinsicMap[&Bool::ctor_Int] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        SPIRVResult val = LoadValueSPIRV(c, g, args[0]);
        uint32_t type = GeneratePODTypeSPIRV(c, g, TypeCode::Bool);
        
        // First, test if integer is either 0 or 1 by comparing it to 
        SPIRVResult falseValue = GenerateConstantSPIRV(c, g, ConstantCreationInfo::Int(0), 1);
        g->writer->MappedInstruction(OpINotEqual, SPVWriter::Section::LocalFunction, type, val, falseValue);
        uint32_t res = g->AddMappedOp(TStr::Separated("OpINotEqual", SPVArg(type), SPVArg(val.name), SPVArg(falseValue.name)));
        return SPIRVResult(res, type, true);
    };

#define X(type, ctor, val, argCount, splat, vectorSize, conversion)\
    SPIRVGenerator::IntrinsicMap[&type::ctor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {\
        assert(args.size() == argCount);\
        SPIRVResult loadedArg = LoadValueSPIRV(c, g, args[0]);\
        if (returnType == loadedArg.typeName)\
            return loadedArg;\
        if constexpr (ConversionTable::conversion != ConversionTable::None)\
            loadedArg = GenerateConversionSPIRV(c, g, ConversionTable::conversion, vectorSize/splat, loadedArg);\
        if constexpr (splat > 1)\
            loadedArg = GenerateSplatCompositeSPIRV(c, g, returnType, splat, loadedArg);\
        return loadedArg;\
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
    SPIRVGenerator::IntrinsicMap[&type::ctor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {\
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
    SPIRVGenerator::IntrinsicMap[&type::ctor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {\
        return GenerateCompositeSPIRV(c, g, returnType, { args[0], args[1], args[2] });\
    };

    FLOAT4_CTOR3_LIST
    INT4_CTOR3_LIST
    UINT4_CTOR3_LIST
#undef X

    // For matrix float constructors, we need to first construct the vectors and then compose the matrix from them
    SPIRVGenerator::IntrinsicMap[&Mat2x2::identityConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        assert(args.size() == 0);
        SPIRVResult zero = GenerateConstantSPIRV(c, g, ConstantCreationInfo::Float(0));
        SPIRVResult one = GenerateConstantSPIRV(c, g, ConstantCreationInfo::Float(1));
        uint32_t vectorType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, 2);
        SPIRVResult col0 = GenerateCompositeSPIRV(c, g, vectorType, { one, zero });
        SPIRVResult col1 = GenerateCompositeSPIRV(c, g, vectorType, { zero, one });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1 });
    };
    SPIRVGenerator::IntrinsicMap[&Mat2x2::floatConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 4);
        uint32_t vectorType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, 2);
        SPIRVResult col0 = GenerateCompositeSPIRV(c, g, vectorType, { args[0], args[2] });
        SPIRVResult col1 = GenerateCompositeSPIRV(c, g, vectorType, { args[1], args[3] });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1 });
    };
    SPIRVGenerator::IntrinsicMap[&Mat2x2::vectorConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 2);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    
    SPIRVGenerator::IntrinsicMap[&Mat2x3::floatConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 6);
        uint32_t vectorType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, 2);
        SPIRVResult col0 = GenerateCompositeSPIRV(c, g, vectorType, { args[0], args[3] });
        SPIRVResult col1 = GenerateCompositeSPIRV(c, g, vectorType, { args[1], args[4] });
        SPIRVResult col3 = GenerateCompositeSPIRV(c, g, vectorType, { args[2], args[5] });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1, col3 });
    };
    SPIRVGenerator::IntrinsicMap[&Mat2x3::vectorConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 3);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    SPIRVGenerator::IntrinsicMap[&Mat2x4::floatConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 8);
        uint32_t vectorType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, 2);
        SPIRVResult col0 = GenerateCompositeSPIRV(c, g, vectorType, { args[0], args[4] });
        SPIRVResult col1 = GenerateCompositeSPIRV(c, g, vectorType, { args[1], args[5] });
        SPIRVResult col3 = GenerateCompositeSPIRV(c, g, vectorType, { args[2], args[6] });
        SPIRVResult col4 = GenerateCompositeSPIRV(c, g, vectorType, { args[3], args[7] });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1, col3, col4 });
    };
    SPIRVGenerator::IntrinsicMap[&Mat2x4::vectorConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 3);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };

    SPIRVGenerator::IntrinsicMap[&Mat3x2::floatConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 6);
        uint32_t vectorType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, 3);
        SPIRVResult col0 = GenerateCompositeSPIRV(c, g, vectorType, { args[0], args[2], args[4] });
        SPIRVResult col1 = GenerateCompositeSPIRV(c, g, vectorType, { args[1], args[3], args[5] });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1 });
    };
    SPIRVGenerator::IntrinsicMap[&Mat3x2::vectorConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 2);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    SPIRVGenerator::IntrinsicMap[&Mat3x3::identityConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        assert(args.size() == 0);
        SPIRVResult zero = GenerateConstantSPIRV(c, g, ConstantCreationInfo::Float(0));
        SPIRVResult one = GenerateConstantSPIRV(c, g, ConstantCreationInfo::Float(1));
        uint32_t vectorType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, 3);
        SPIRVResult col0 = GenerateCompositeSPIRV(c, g, vectorType, { one, zero, zero });
        SPIRVResult col1 = GenerateCompositeSPIRV(c, g, vectorType, { zero, one, zero });
        SPIRVResult col2 = GenerateCompositeSPIRV(c, g, vectorType, { zero, zero, one });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1, col2 });
    };
    SPIRVGenerator::IntrinsicMap[&Mat3x3::floatConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 9);
        uint32_t vectorType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, 3);
        SPIRVResult col0 = GenerateCompositeSPIRV(c, g, vectorType, { args[0], args[3], args[6] });
        SPIRVResult col1 = GenerateCompositeSPIRV(c, g, vectorType, { args[1], args[4], args[7] });
        SPIRVResult col2 = GenerateCompositeSPIRV(c, g, vectorType, { args[2], args[5], args[8] });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1, col2 });
    };
    SPIRVGenerator::IntrinsicMap[&Mat3x3::vectorConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 3);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    SPIRVGenerator::IntrinsicMap[&Mat3x4::floatConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 12);
        uint32_t vectorType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, 3);
        SPIRVResult col0 = GenerateCompositeSPIRV(c, g, vectorType, { args[0], args[4], args[8] });
        SPIRVResult col1 = GenerateCompositeSPIRV(c, g, vectorType, { args[1], args[5], args[9] });
        SPIRVResult col2 = GenerateCompositeSPIRV(c, g, vectorType, { args[2], args[6], args[10] });
        SPIRVResult col3 = GenerateCompositeSPIRV(c, g, vectorType, { args[3], args[7], args[11] });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1, col2, col3 });
    };
    SPIRVGenerator::IntrinsicMap[&Mat3x4::vectorConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 4);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    SPIRVGenerator::IntrinsicMap[&Mat4x2::floatConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 8);
        uint32_t vectorType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, 4);
        SPIRVResult col0 = GenerateCompositeSPIRV(c, g, vectorType, { args[0], args[2], args[4], args[6] });
        SPIRVResult col1 = GenerateCompositeSPIRV(c, g, vectorType, { args[1], args[3], args[5], args[7] });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1 });
    };
    SPIRVGenerator::IntrinsicMap[&Mat4x2::vectorConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 2);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    SPIRVGenerator::IntrinsicMap[&Mat4x3::floatConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 12);
        uint32_t vectorType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, 4);
        SPIRVResult col0 = GenerateCompositeSPIRV(c, g, vectorType, { args[0], args[3], args[6], args[9] });
        SPIRVResult col1 = GenerateCompositeSPIRV(c, g, vectorType, { args[1], args[4], args[7], args[10] });
        SPIRVResult col2 = GenerateCompositeSPIRV(c, g, vectorType, { args[2], args[5], args[8], args[11] });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1, col2 });
    };
    SPIRVGenerator::IntrinsicMap[&Mat4x3::vectorConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 3);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    SPIRVGenerator::IntrinsicMap[&Mat4x4::identityConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        assert(args.size() == 0);
        SPIRVResult zero = GenerateConstantSPIRV(c, g, ConstantCreationInfo::Float(0));
        SPIRVResult one = GenerateConstantSPIRV(c, g, ConstantCreationInfo::Float(1));
        uint32_t vectorType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, 4);
        SPIRVResult col0 = GenerateCompositeSPIRV(c, g, vectorType, { one, zero, zero, zero });
        SPIRVResult col1 = GenerateCompositeSPIRV(c, g, vectorType, { zero, one, zero, zero });
        SPIRVResult col2 = GenerateCompositeSPIRV(c, g, vectorType, { zero, zero, one, zero });
        SPIRVResult col3 = GenerateCompositeSPIRV(c, g, vectorType, { zero, zero, zero, one });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1, col2, col3 });
    };
    SPIRVGenerator::IntrinsicMap[&Mat4x4::floatConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 16);
        uint32_t vectorType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, 4);
        SPIRVResult col0 = GenerateCompositeSPIRV(c, g, vectorType, { args[0], args[4], args[8], args[12] });
        SPIRVResult col1 = GenerateCompositeSPIRV(c, g, vectorType, { args[1], args[5], args[9], args[13] });
        SPIRVResult col2 = GenerateCompositeSPIRV(c, g, vectorType, { args[2], args[6], args[10], args[14] });
        SPIRVResult col3 = GenerateCompositeSPIRV(c, g, vectorType, { args[3], args[7], args[11], args[15] });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1, col2, col3 });
    };
    SPIRVGenerator::IntrinsicMap[&Mat4x3::vectorConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 4);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };

#define OPERATOR_INTRINSIC(ty, fn, op, inst)\
    { &ty::##fn##Operator, #op, #inst, false, Op##op##inst }\
    , { &ty::##fn##AssignOperator, #op, #inst, true, Op##op##inst }


#define OPERATOR_INTRINSIC_NO_ASSIGN(ty, fn, op, inst)\
    { &ty::##fn##Operator, #op, #inst, false, Op##op##inst## }

    std::vector<std::tuple<Function*, const char*, const char*, bool, SPVOp>> operatorFunctions =
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
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float4, lt, FUnord, LessThan)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float3, lt, FUnord, LessThan)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float2, lt, FUnord, LessThan)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float, lt, FUnord, LessThan)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float4, lte, FUnord, LessThanEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float3, lte, FUnord, LessThanEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float2, lte, FUnord, LessThanEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float, lte, FUnord, LessThanEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float4, gt, FUnord, GreaterThan)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float3, gt, FUnord, GreaterThan)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float2, gt, FUnord, GreaterThan)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float, gt, FUnord, GreaterThan)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float4, gte, FUnord, GreaterThanEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float3, gte, FUnord, GreaterThanEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float2, gte, FUnord, GreaterThanEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float, gte, FUnord, GreaterThanEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float4, e, FUnord, Equal)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float3, e, FUnord, Equal)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float2, e, FUnord, Equal)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float, e, FUnord, Equal)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float4, ne, FUnord, NotEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float3, ne, FUnord, NotEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float2, ne, FUnord, NotEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float, ne, FUnord, NotEqual)
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
        , OPERATOR_INTRINSIC_NO_ASSIGN(Int4, ne, I, NotEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Int3, ne, I, NotEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Int2, ne, I, NotEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Int, ne, I, NotEqual)
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
        , OPERATOR_INTRINSIC_NO_ASSIGN(UInt4, ne, I, NotEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(UInt3, ne, I, NotEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(UInt2, ne, I, NotEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(UInt, ne, I, NotEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Bool, oror, , LogicalOr)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Bool, andand, , LogicalAnd)
    };

    for (auto fun : operatorFunctions)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [ty = std::get<1>(fun), op = std::get<2>(fun), assign = std::get<3>(fun), inst = std::get<4>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            SPIRVResult lhs = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult rhs = LoadValueSPIRV(c, g, args[1]);
            TStr opStr = TStr::Compact("Op", ty, op);
            g->writer->MappedInstruction(inst, SPVWriter::Section::LocalFunction, returnType, lhs, rhs);
            uint32_t ret = g->AddMappedOp(TStr::Separated(opStr, SPVArg(returnType), SPVArg(lhs.name), SPVArg(rhs.name)));
            /*
            if (assign)
            {
                assert(!args[0].isLiteral);
                assert(!args[0].isValue);
                SPIRVResult val(ret, returnType, true);
                StoreValueSPIRV(c, g, args[0], val);
                return SPIRVResult(args[0].name, args[0].typeName);
            }
            */
            return SPIRVResult(ret, returnType, true);
        };
    }

#define MATRIX_OPERATOR_INTRINSIC(ty, fn, size, op, inst)\
    { &ty::##fn##Operator, #op, size, #inst, false, Op##op##inst## }\
    , { &ty::##fn##AssignOperator, #op, size, #inst, true, Op##op##inst## }

    std::vector<std::tuple<Function*, const char*, uint32_t, const char*, bool, SPVOp>> matrixOperators =
    {
        MATRIX_OPERATOR_INTRINSIC(Mat2x2, addition, 2, F, Add)
        , MATRIX_OPERATOR_INTRINSIC(Mat2x3, addition, 3, F, Add)
        , MATRIX_OPERATOR_INTRINSIC(Mat2x4, addition, 4, F, Add)
        , MATRIX_OPERATOR_INTRINSIC(Mat3x2, addition, 2, F, Add)
        , MATRIX_OPERATOR_INTRINSIC(Mat3x3, addition, 3, F, Add)
        , MATRIX_OPERATOR_INTRINSIC(Mat3x4, addition, 4, F, Add)
        , MATRIX_OPERATOR_INTRINSIC(Mat4x2, addition, 2, F, Add)
        , MATRIX_OPERATOR_INTRINSIC(Mat4x3, addition, 3, F, Add)
        , MATRIX_OPERATOR_INTRINSIC(Mat4x4, addition, 4, F, Add)
    };

    for (auto fun : matrixOperators)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [ty = std::get<1>(fun), size = std::get<2>(fun), op = std::get<3>(fun), assign = std::get<4>(fun), inst = std::get<5>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);

            // First load both sides
            SPIRVResult lhs = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult rhs = LoadValueSPIRV(c, g, args[1]);

            // We will have to extract each vector from the matrix to do the operation, so we need the vector type
            uint32_t vectorType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, size);
            TStr opStr = TStr::Compact("Op", ty, op);
            TStr intermediateResults;
            SPVArg* intermediateArgs = AllocStack<SPVArg>(size);
            for (uint32_t i = 0; i < size; i++)
            {
                // Proceed to extract from the composites
                uint32_t newLhsVec = g->writer->MappedInstruction(OpCompositeExtract, SPVWriter::Section::LocalFunction, vectorType, lhs, i);
                uint32_t newRhsVec = g->writer->MappedInstruction(OpCompositeExtract, SPVWriter::Section::LocalFunction, vectorType, rhs, i);
                uint32_t newRes = g->writer->MappedInstruction(inst, SPVWriter::Section::LocalFunction, vectorType, SPVArg(newLhsVec), SPVArg(newRhsVec));
                intermediateArgs[i] = SPVArg(newRes);

                uint32_t lhsVec = g->AddMappedOp(TStr::Separated("OpCompositeExtract", SPVArg(vectorType), SPVArg(lhs.name), i));
                uint32_t rhsVec = g->AddMappedOp(TStr::Separated("OpCompositeExtract", SPVArg(vectorType), SPVArg(rhs.name), i));

                // Run the operator on the vectors
                uint32_t res = g->AddMappedOp(TStr::Separated(opStr, SPVArg(vectorType), SPVArg(lhsVec), SPVArg(rhsVec)));
                intermediateResults.Concatenate<false>(SPVArg(res), " ");
            }

            // Finally compose back to a matrix
            g->writer->MappedInstruction(OpCompositeConstruct, SPVWriter::Section::LocalFunction, returnType, SPVArgList{ .vals = intermediateArgs, .num = (uint8_t)size });
            uint32_t ret = g->AddMappedOp(TStr::Separated("OpCompositeConstruct", SPVArg(returnType), intermediateResults));

            DeallocStack(size, intermediateArgs);
            /*
            if (assign)
            {
                assert(!args[0].isLiteral);
                assert(!args[0].isValue);
                SPIRVResult val(ret, returnType, true);
                StoreValueSPIRV(c, g, args[0], val);
                return SPIRVResult(args[0].name, args[0].typeName);
            }
            */
            return SPIRVResult(ret, returnType, true);
        };
    }

#define BOOL_OPERATOR_INTRINSIC_NO_ASSIGN(ty, fn, size, inst)\
    { &ty::##fn##Operator, #inst, size, false, OpI##inst }
    
    std::vector<std::tuple<Function*, const char*, uint32_t, bool, SPVOp>> boolOperatorFunctions =
    {
        BOOL_OPERATOR_INTRINSIC_NO_ASSIGN(Bool4, e, 4, Equal)
        , BOOL_OPERATOR_INTRINSIC_NO_ASSIGN(Bool3, e, 3, Equal)
        , BOOL_OPERATOR_INTRINSIC_NO_ASSIGN(Bool2, e, 2, Equal)
        , BOOL_OPERATOR_INTRINSIC_NO_ASSIGN(Bool, e, 1, Equal)
        , BOOL_OPERATOR_INTRINSIC_NO_ASSIGN(Bool4, ne, 4, NotEqual)
        , BOOL_OPERATOR_INTRINSIC_NO_ASSIGN(Bool3, ne, 3, NotEqual)
        , BOOL_OPERATOR_INTRINSIC_NO_ASSIGN(Bool2, ne, 2, NotEqual)
        , BOOL_OPERATOR_INTRINSIC_NO_ASSIGN(Bool, ne, 1, NotEqual)
    };
    for (auto fun : boolOperatorFunctions)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [ty = std::get<1>(fun), size = std::get<2>(fun), assign = std::get<3>(fun), inst = std::get<4>(fun) ](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            SPIRVResult lhs = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult rhs = LoadValueSPIRV(c, g, args[1]);
            
            // First convert from bool to int
            SPIRVResult lhsConverted = GenerateConversionSPIRV(c, g, ConversionTable::BoolToUInt, size, lhs);
            SPIRVResult rhsConverted = GenerateConversionSPIRV(c, g, ConversionTable::BoolToUInt, size, rhs);

            g->writer->MappedInstruction(inst, SPVWriter::Section::LocalFunction, returnType, lhsConverted, rhsConverted);
            TStr opStr = TStr::Compact("OpI", ty);
            uint32_t ret = g->AddMappedOp(TStr::Separated(opStr, SPVArg(returnType), SPVArg(lhsConverted.name), SPVArg(rhsConverted.name)));
            /*
            if (assign)
            {
                assert(!args[0].isLiteral);
                assert(!args[0].isValue);
                SPIRVResult val(ret, returnType, true);
                StoreValueSPIRV(c, g, args[0], val);
                return SPIRVResult(args[0].name, args[0].typeName);
            }
            */
            return SPIRVResult(ret, returnType, true);
        };
    }
    
    std::vector<std::tuple<Function*, ConversionTable>> assignOperators =
    {
        { &Int::uintAssignOperator, ConversionTable::UIntToInt }
        , { &Int::floatAssignOperator, ConversionTable::FloatToInt }
        , { &Int::boolAssignOperator, ConversionTable::BoolToInt }
        , { &UInt::intAssignOperator, ConversionTable::IntToUInt }
        , { &UInt::floatAssignOperator, ConversionTable::FloatToUInt }
        , { &UInt::boolAssignOperator, ConversionTable::BoolToUInt }
        , { &Float::intAssignOperator, ConversionTable::IntToFloat }
        , { &Float::uintAssignOperator, ConversionTable::UIntToFloat }
    };
    for (auto fun : assignOperators)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [conv = std::get<1>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
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
        SPIRVGenerator::IntrinsicMap[fun.first] = [assign = fun.second](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            SPIRVResult lhs = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult rhs = LoadValueSPIRV(c, g, args[1]);
            g->writer->MappedInstruction(OpMatrixTimesMatrix, SPVWriter::Section::LocalFunction, returnType, lhs, rhs);
            uint32_t ret = g->AddMappedOp(TStr::Separated("OpMatrixTimesMatrix", SPVArg(returnType), SPVArg(lhs.name), SPVArg(rhs.name)));
            /*
            if (assign)
            {
                assert(!args[0].isLiteral);
                assert(!args[0].isValue);
                SPIRVResult val(ret, returnType, true);
                StoreValueSPIRV(c, g, args[0], val);
                return SPIRVResult(args[0].name, args[0].typeName);
            }
            */
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<Function*> matrixScaleFunctions =
{
        &Mat2x2::scaleOperator
        , &Mat2x3::scaleOperator
        , &Mat2x4::scaleOperator
        , &Mat3x2::scaleOperator
        , &Mat3x3::scaleOperator
        , &Mat3x4::scaleOperator
        , &Mat4x2::scaleOperator
        , &Mat4x3::scaleOperator
        , &Mat4x4::scaleOperator
    };
    for (auto fun : matrixScaleFunctions)
    {
        SPIRVGenerator::IntrinsicMap[fun] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            SPIRVResult lhs = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult rhs = LoadValueSPIRV(c, g, args[1]);
            g->writer->MappedInstruction(OpMatrixTimesScalar, SPVWriter::Section::LocalFunction, returnType, lhs, rhs);
            return SPIRVResult(g->AddMappedOp(TStr::Separated("OpMatrixTimesScalar", SPVArg(returnType), SPVArg(lhs.name), SPVArg(rhs.name))), returnType, true);
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
        SPIRVGenerator::IntrinsicMap[fun] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            SPIRVResult lhs = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult rhs = LoadValueSPIRV(c, g, args[1]);
            g->writer->MappedInstruction(OpMatrixTimesVector, SPVWriter::Section::LocalFunction, returnType, lhs, rhs);
            return SPIRVResult(g->AddMappedOp(TStr::Separated("OpMatrixTimesVector", SPVArg(returnType), SPVArg(lhs.name), SPVArg(rhs.name))), returnType, true);
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
        SPIRVGenerator::IntrinsicMap[fun] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            SPIRVResult lhs = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult rhs = LoadValueSPIRV(c, g, args[1]);
            g->writer->MappedInstruction(OpVectorTimesScalar, SPVWriter::Section::LocalFunction, SPVWriter::Section::LocalFunction, returnType, lhs, rhs);
            return SPIRVResult(g->AddMappedOp(TStr::Separated("OpVectorTimesScalar", SPVArg(returnType), SPVArg(lhs.name), SPVArg(rhs.name))), returnType, true);
        };
    }

    std::unordered_map<Function*, std::tuple<char, uint32_t, SPVOp>> intVectorScaleFunctions =
    {
        { &Int4::scaleOperator, { 'S', 4, OpIMul } }
        , { &Int3::scaleOperator, { 'S', 3, OpIMul } }
        , { &Int2::scaleOperator, { 'S', 2, OpIMul } }
        , { &UInt4::scaleOperator, { 'U', 4, OpIMul } }
        , { &UInt3::scaleOperator, { 'U', 3, OpIMul } }
        , { &UInt2::scaleOperator, { 'U', 2, OpIMul } }
    };
    for (auto fun : intVectorScaleFunctions)
    {
        SPIRVGenerator::IntrinsicMap[fun.first] = [op = std::get<0>(fun.second), num = std::get<1>(fun.second), inst = std::get<2>(fun.second)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            SPIRVResult splat = GenerateSplatCompositeSPIRV(c, g, returnType, num, args[1]);
            SPIRVResult lhs = LoadValueSPIRV(c, g, args[0]);
            TStr opStr = TStr::Compact("Op", op, "Mul");
            g->writer->MappedInstruction(inst, SPVWriter::Section::LocalFunction, returnType, lhs, splat);
            return SPIRVResult(g->AddMappedOp(TStr::Separated(opStr, SPVArg(returnType), SPVArg(lhs.name), SPVArg(splat.name))), returnType, true);
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
        SPIRVGenerator::IntrinsicMap[fun] = [fun](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);

            SPIRVResult lhs = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult rhs = LoadValueSPIRV(c, g, args[1]);
            g->writer->MappedInstruction(OpVectorTimesMatrix, SPVWriter::Section::LocalFunction, returnType, lhs, rhs);
            return SPIRVResult(g->AddMappedOp(TStr::Separated("OpVectorTimesMatrix", SPVArg(returnType), SPVArg(lhs.name), SPVArg(rhs.name))), returnType, true);
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
        &Bool2::elementAccessOperatorInt,
        &Bool2::elementAccessOperatorUInt,
        &Bool3::elementAccessOperatorInt,
        &Bool3::elementAccessOperatorUInt,
        &Bool4::elementAccessOperatorInt,
        &Bool4::elementAccessOperatorUInt,
        &Mat2x2::elementAccessOperatorInt,
        &Mat2x2::elementAccessOperatorUInt,
        &Mat2x3::elementAccessOperatorInt,
        &Mat2x3::elementAccessOperatorUInt,
        &Mat2x4::elementAccessOperatorInt,
        &Mat2x4::elementAccessOperatorUInt,
        &Mat3x2::elementAccessOperatorInt,
        &Mat3x2::elementAccessOperatorUInt,
        &Mat3x3::elementAccessOperatorInt,
        &Mat3x3::elementAccessOperatorUInt,
        &Mat3x4::elementAccessOperatorInt,
        &Mat3x4::elementAccessOperatorUInt,
        &Mat4x2::elementAccessOperatorInt,
        &Mat4x2::elementAccessOperatorUInt,
        &Mat4x3::elementAccessOperatorInt,
        &Mat4x3::elementAccessOperatorUInt,
        &Mat4x4::elementAccessOperatorInt,
        &Mat4x4::elementAccessOperatorUInt,
    };
    for (auto fun : arrayAccessFunctions)
    {
        SPIRVGenerator::IntrinsicMap[fun] = [fun](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            assert(!args[0].isValue);
            Function::__Resolved* funRes = Symbol::Resolved(fun);

            SPIRVResult returnTypePtr = GeneratePointerTypeSPIRV(c, g, fun->returnType, funRes->returnTypeSymbol, args[0].scope);

            // Load index
            SPIRVResult loadedIndex = LoadValueSPIRV(c, g, args[1]);
            SPIRVResult ret = args[0];
            ret.AddAccessChainLink({loadedIndex});
            ret.typeName = returnTypePtr.typeName;
            ret.parentTypes = returnTypePtr.parentTypes;
            ret.scope = args[0].scope;
            ret.isValue = false;
            return ret;
        };
    }

#define BIT_INTRINSIC(ty, fn, inst)\
    { &ty::##fn##Operator, #inst, false, Op##inst }\
    , { &ty::##fn##AssignOperator, #inst, true, Op##inst }

    std::vector<std::tuple<Function*, const char*, bool, SPVOp>> bitwiseOps =
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
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [fun](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);

            auto [fn, op, assign, inst] = fun;
            SPIRVResult arg0 = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult arg1 = LoadValueSPIRV(c, g, args[1]);
            g->writer->MappedInstruction(inst, SPVWriter::Section::LocalFunction, returnType, arg0, arg1);
            TStr opStr = TStr::Compact("Op", op);
            uint32_t ret = g->AddMappedOp(TStr::Separated(opStr, SPVArg(returnType), SPVArg(arg0.name), SPVArg(arg1.name)));
            /*
            if (assign)
            {
                assert(!args[0].isLiteral);
                assert(!args[0].isValue);
                SPIRVResult val(ret, returnType, true);
                StoreValueSPIRV(c, g, args[0], val);
                return SPIRVResult(args[0].name, args[0].typeName);
            }
            */
            return SPIRVResult(ret, returnType, true);
        };
    }

#define NO_ASSIGN_BIT_INTRINSIC(ty, fn, inst)\
    { &ty::##fn##Operator, #inst, false, Op##inst }\
    
    std::vector<std::tuple<Function*, const char*, bool, SPVOp>> noAssignBitwiseOps =
    {
        NO_ASSIGN_BIT_INTRINSIC(Bool4, oror, LogicalOr)
        , NO_ASSIGN_BIT_INTRINSIC(Bool3, oror, LogicalOr)
        , NO_ASSIGN_BIT_INTRINSIC(Bool2, oror, LogicalOr)
        , NO_ASSIGN_BIT_INTRINSIC(Bool, oror, LogicalOr)
        , NO_ASSIGN_BIT_INTRINSIC(Bool4, andand, LogicalAnd)
        , NO_ASSIGN_BIT_INTRINSIC(Bool3, andand, LogicalAnd)
        , NO_ASSIGN_BIT_INTRINSIC(Bool2, andand, LogicalAnd)
        , NO_ASSIGN_BIT_INTRINSIC(Bool, andand, LogicalAnd)
    };
    for (auto fun : noAssignBitwiseOps)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [fun](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);

            auto [fn, op, assign, inst] = fun;
            SPIRVResult arg0 = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult arg1 = LoadValueSPIRV(c, g, args[1]);
            g->writer->MappedInstruction(inst, SPVWriter::Section::LocalFunction, returnType, arg0, arg1);

            TStr opStr = TStr::Compact("Op", op);
            uint32_t ret = g->AddMappedOp(TStr::Separated(opStr, SPVArg(returnType), SPVArg(arg0.name), SPVArg(arg1.name)));

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

#define MAKE_BOOL_INTRINSICS(op)\
    { Intrinsics::##op##_b8, 'B' }\
    , { Intrinsics::##op##_b8x2, 'B' }\
    , { Intrinsics::##op##_b8x3, 'B' }\
    , { Intrinsics::##op##_b8x4, 'B' }

#define MAKE_FLOAT_VEC_INTRINSICS(op)\
    { Intrinsics::##op##_f32x2, 'F' }\
    , { Intrinsics::##op##_f32x3, 'F' }\
    , { Intrinsics::##op##_f32x4, 'F' }

#define MAKE_MAT_INTRINSICS(op)\
    { Intrinsics::##op##_f32x2x2, 'F' }\
    , { Intrinsics::##op##_f32x2x3, 'F' }\
    , { Intrinsics::##op##_f32x2x4, 'F' }\
    , { Intrinsics::##op##_f32x3x2, 'F' }\
    , { Intrinsics::##op##_f32x3x3, 'F' }\
    , { Intrinsics::##op##_f32x3x4, 'F' }\
    , { Intrinsics::##op##_f32x4x2, 'F' }\
    , { Intrinsics::##op##_f32x4x3, 'F' }\
    , { Intrinsics::##op##_f32x4x4, 'F' }

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
    
#define MAKE_SINGLE_COMPONENT_INTRINSICS(op)\
    { Intrinsics::##op##_f32, 'F' }\
    , { Intrinsics::##op##_i32, 'S' }\
    , { Intrinsics::##op##_u32, 'U' }\

#define MAKE_EXT_FLOAT_INTRINSICS(op, fun)\
    { Intrinsics::##op##_f32, #fun, fun }\
    , { Intrinsics::##op##_f32x2, #fun, fun }\
    , { Intrinsics::##op##_f32x3, #fun, fun }\
    , { Intrinsics::##op##_f32x4, #fun, fun }

#define MAKE_EXT_INT_INTRINSICS(op, fun)\
    { Intrinsics::##op##_i32, #fun, fun }\
    , { Intrinsics::##op##_i32x2, #fun, fun }\
    , { Intrinsics::##op##_i32x3, #fun, fun }\
    , { Intrinsics::##op##_i32x4, #fun, fun }

    std::vector<std::tuple<Function*, const char*, SPVEnum>> extThreeArgumentIntrinsics =
    {
        MAKE_EXT_FLOAT_INTRINSICS(Pow, Pow)
    };
    for (auto fun : extThreeArgumentIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [fun](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);

            auto [fn, op, en] = fun;
            SPIRVResult arg = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult base = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult exp = LoadValueSPIRV(c, g, args[1]);
            uint32_t ext = g->ImportExtension("GLSL.std.450");
            g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), en, base, exp);
            uint32_t ret = g->AddMappedOp(TStr::Separated("OpExtInst", SPVArg(returnType), SPVArg(ext), op, SPVArg(base.name), SPVArg(exp.name)));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char*, SPVEnum>> extSingleArgumentFloatIntrinsics =
    {
        MAKE_EXT_FLOAT_INTRINSICS(Sqrt, Sqrt)
        , MAKE_EXT_FLOAT_INTRINSICS(InvSqrt, InverseSqrt)
        , MAKE_EXT_FLOAT_INTRINSICS(Log, Log)
        , MAKE_EXT_FLOAT_INTRINSICS(Log2, Log2)
        , MAKE_EXT_FLOAT_INTRINSICS(Exp, Exp)
        , MAKE_EXT_FLOAT_INTRINSICS(Exp2, Exp2)
        , MAKE_EXT_FLOAT_INTRINSICS(Sin, Sin)
        , MAKE_EXT_FLOAT_INTRINSICS(Cos, Cos)
        , MAKE_EXT_FLOAT_INTRINSICS(Tan, Tan)
        , MAKE_EXT_FLOAT_INTRINSICS(ASin, Asin)
        , MAKE_EXT_FLOAT_INTRINSICS(ACos, Acos)
        , MAKE_EXT_FLOAT_INTRINSICS(ATan, Atan)
        , MAKE_EXT_FLOAT_INTRINSICS(SinH, Sinh)
        , MAKE_EXT_FLOAT_INTRINSICS(CosH, Cosh)
        , MAKE_EXT_FLOAT_INTRINSICS(TanH, Tanh)
        , MAKE_EXT_FLOAT_INTRINSICS(ASinH, Asinh)
        , MAKE_EXT_FLOAT_INTRINSICS(ACosH, Acosh)
        , MAKE_EXT_FLOAT_INTRINSICS(ATanH, Atanh)
        , MAKE_EXT_FLOAT_INTRINSICS(Ceil, Ceil)
        , MAKE_EXT_FLOAT_INTRINSICS(Floor, Floor)
        , MAKE_EXT_FLOAT_INTRINSICS(Fract, Fract)
        , MAKE_EXT_FLOAT_INTRINSICS(Truncate, Trunc)

    };
    for (auto fun : extSingleArgumentFloatIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [fun](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 1);

            auto [fn, op, en] = fun;
            SPIRVResult arg = LoadValueSPIRV(c, g, args[0]);
            uint32_t ext = g->ImportExtension("GLSL.std.450");
            g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), en, arg);

            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d %s %%%d", returnType, ext, op, arg.name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char*, SPVEnum>> extTwoArgumentTranscendentalsFloatIntrinsics =
    {
        MAKE_EXT_FLOAT_INTRINSICS(ATan2, Atan2)
    };
    for (auto fun : extTwoArgumentTranscendentalsFloatIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [fun](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);

            auto [fn, op, en] = fun;
            SPIRVResult arg1 = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult arg2 = LoadValueSPIRV(c, g, args[1]);
            uint32_t ext = g->ImportExtension("GLSL.std.450");
            g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), en, arg1, arg2);
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d %s %%%d %%%d", returnType, ext, op, arg1.name, arg2.name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> madIntrinsics =
    {
        MAKE_SCALAR_INTRINSICS(Mad)
    };
    for (auto fun : madIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 3);
            SPIRVResult arg0 = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult arg1 = LoadValueSPIRV(c, g, args[1]);
            SPIRVResult arg2 = LoadValueSPIRV(c, g, args[2]);
            uint32_t ext = g->ImportExtension("GLSL.std.450");
            g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), Fma, arg0, arg1, arg2);
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d Fma %%%d %%%d %%%d", returnType, ext, arg0.name, arg1.name, arg2.name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> dotIntrinsics =
    {
        MAKE_FLOAT_VEC_INTRINSICS(Dot)
    };
    for (auto fun : dotIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            SPIRVResult arg0 = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult arg1 = LoadValueSPIRV(c, g, args[1]);
            g->writer->MappedInstruction(OpDot, SPVWriter::Section::LocalFunction, returnType, arg0, arg1);
            uint32_t ret = g->AddMappedOp(Format("OpDot %%%d %%%d %%%d", returnType, arg0.name, arg1.name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> reflectIntrinsics =
    {
        MAKE_FLOAT_VEC_INTRINSICS(Reflect)
    };
    for (auto fun : reflectIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            uint32_t ext = g->ImportExtension("GLSL.std.450");
            SPIRVResult arg0 = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult arg1 = LoadValueSPIRV(c, g, args[1]);
            g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), Reflect, arg0, arg1);
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d Reflect %%%d %%%d", returnType, ext, arg0.name, arg1.name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> refractIntrinsics =
    {
        MAKE_FLOAT_VEC_INTRINSICS(Refract)
    };
    for (auto fun : refractIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            uint32_t ext = g->ImportExtension("GLSL.std.450");
            SPIRVResult arg0 = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult arg1 = LoadValueSPIRV(c, g, args[1]);
            SPIRVResult arg2 = LoadValueSPIRV(c, g, args[2]);
            g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), Refract, arg0, arg1, arg2);
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d Refract %%%d %%%d %%%d", returnType, ext, arg0.name, arg1.name, arg2.name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> crossIntrinsics =
    {
        { Intrinsics::Cross_f32x3, 'F' }
    };
    for (auto fun : crossIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            uint32_t ext = g->ImportExtension("GLSL.std.450");
            SPIRVResult arg0 = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult arg1 = LoadValueSPIRV(c, g, args[1]);
            g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), Cross, arg0, arg1);
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d Cross %%%d %%%d", returnType, ext, arg0.name, arg1.name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> normalizeIntrinsics =
    {
        MAKE_FLOAT_VEC_INTRINSICS(Normalize)
    };
    for (auto fun : normalizeIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 1);
            SPIRVResult arg = LoadValueSPIRV(c, g, args[0]);
            uint32_t ext = g->ImportExtension("GLSL.std.450");
            g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), Normalize, arg);
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d Normalize %%%d", returnType, ext, arg.name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> lengthIntrinsics =
    {
        MAKE_FLOAT_VEC_INTRINSICS(Length)
    };
    for (auto fun : lengthIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 1);
            SPIRVResult arg = LoadValueSPIRV(c, g, args[0]);
            uint32_t ext = g->ImportExtension("GLSL.std.450");
            g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), Length, arg);
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d Length %%%d", returnType, ext, arg.name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> distanceIntrinsics =
    {
        MAKE_FLOAT_VEC_INTRINSICS(Distance)
    };
    for (auto fun : distanceIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 1);
            SPIRVResult arg0 = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult arg1 = LoadValueSPIRV(c, g, args[1]);
            uint32_t ext = g->ImportExtension("GLSL.std.450");
            g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), Distance, arg0, arg1);
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d Distance %%%d %%%d", returnType, ext, arg0.name, arg1.name));
            return SPIRVResult(ret, returnType, true);
        };
    }
    
    std::vector<std::tuple<Function*, const char>> minIntrinsics =
    {
        MAKE_SCALAR_INTRINSICS(Min)
    };
    for (auto fun : minIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            SPIRVResult arg0 = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult arg1 = LoadValueSPIRV(c, g, args[1]);
            uint32_t ext = g->ImportExtension("GLSL.std.450");
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d %cMin %%%d %%%d", returnType, ext, op, arg0.name, arg1.name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> maxIntrinsics =
    {
        MAKE_SCALAR_INTRINSICS(Max)
    };
    for (auto fun : maxIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            SPIRVResult arg0 = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult arg1 = LoadValueSPIRV(c, g, args[1]);
            uint32_t ext = g->ImportExtension("GLSL.std.450");
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d %cMax %%%d %%%d", returnType, ext, op, arg0.name, arg1.name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> clampIntrinsics =
    {
        MAKE_SCALAR_INTRINSICS(Clamp)
    };
    for (auto fun : clampIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 3);
            SPIRVResult arg0 = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult arg1 = LoadValueSPIRV(c, g, args[1]);
            SPIRVResult arg2 = LoadValueSPIRV(c, g, args[2]);
            uint32_t ext = g->ImportExtension("GLSL.std.450");
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d %cClamp %%%d %%%d %%%d", returnType, ext, op, arg0.name, arg1.name, arg2.name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> lerpIntrinsics =
    {
        MAKE_FLOAT_INTRINSICS(Lerp)    
    };
    for (auto fun : lerpIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 3);
            SPIRVResult arg0 = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult arg1 = LoadValueSPIRV(c, g, args[1]);
            SPIRVResult arg2 = LoadValueSPIRV(c, g, args[2]);
            uint32_t ext = g->ImportExtension("GLSL.std.450");
            g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), FMix, arg0, arg1, arg2);
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d FMix %%%d %%%d %%%d", returnType, ext, arg0.name, arg1.name, arg2.name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> stepIntrinsics =
    {
        MAKE_FLOAT_INTRINSICS(Step)    
    };
    for (auto fun : stepIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            SPIRVResult arg0 = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult arg1 = LoadValueSPIRV(c, g, args[1]);
            uint32_t ext = g->ImportExtension("GLSL.std.450");
            g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), Step, arg0, arg1);
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d Step %%%d %%%d", returnType, ext, arg0.name, arg1.name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> smoothStepIntrinsics =
    {
        MAKE_FLOAT_INTRINSICS(SmoothStep)    
    };
    for (auto fun : smoothStepIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 3);
            SPIRVResult arg0 = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult arg1 = LoadValueSPIRV(c, g, args[1]);
            SPIRVResult arg2 = LoadValueSPIRV(c, g, args[2]);
            uint32_t ext = g->ImportExtension("GLSL.std.450");
            g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), SmoothStep, arg0, arg1, arg2);
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d SmoothStep %%%d %%%d %%%d", returnType, ext, arg0.name, arg1.name, arg2.name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char, uint32_t>> saturateIntrinsics =
    {
        MAKE_FLOAT_INTRINSICS_VEC(Saturate)
    };
    for (auto fun : saturateIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun), size = std::get<2>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 1);
            uint32_t ext = g->ImportExtension("GLSL.std.450");
            uint32_t fType = g->AddSymbol("f32", "OpTypeFloat 32", true);
            uint32_t min = g->AddSymbol("0.0f", Format("OpConstant %%%d 0", fType), true);
            uint32_t max = g->AddSymbol("1.0f", Format("OpConstant %%%d 1", fType), true);

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
                min = g->AddSymbol(Format("f32x%d(0.0f)", size), minComposite, true);
                max = g->AddSymbol(Format("f32x%d(1.0f)", size), maxComposite, true);
            }
            SPIRVResult arg = LoadValueSPIRV(c, g, args[0]);
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d %cClamp %%%d %%%d %%%d", returnType, ext, op, arg.name, min, max));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> absIntrinsics =
    {
        MAKE_SIGN_INTRINSICS(Abs)
    };
    for (auto fun : absIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 1);
            SPIRVResult arg = LoadValueSPIRV(c, g, args[0]);
            uint32_t ext = g->ImportExtension("GLSL.std.450");
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d %cAbs %%%d", returnType, ext, op, arg.name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> signIntrinsics =
    {
        MAKE_SIGN_INTRINSICS(Sign)
    };
    for (auto fun : signIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 1);
            SPIRVResult arg = LoadValueSPIRV(c, g, args[0]);
            uint32_t ext = g->ImportExtension("GLSL.std.450");
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d Sign %%%d", returnType, ext, arg.name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> ddxIntrinsics =
    {
        MAKE_FLOAT_INTRINSICS(DDX)
    };
    for (auto fun : ddxIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 1);
            SPIRVResult arg = LoadValueSPIRV(c, g, args[0]);
            g->writer->MappedInstruction(OpDPdx, SPVWriter::Section::LocalFunction, returnType, arg);
            uint32_t ret = g->AddMappedOp(Format("OpDPdx %%%d %%%d", returnType, arg.name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> ddyIntrinsics =
    {
        MAKE_FLOAT_INTRINSICS(DDY)
    };
    for (auto fun : ddyIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 1);
            SPIRVResult arg = LoadValueSPIRV(c, g, args[0]);
            g->writer->MappedInstruction(OpDPdy, SPVWriter::Section::LocalFunction, returnType, arg);
            uint32_t ret = g->AddMappedOp(Format("OpDPdy %%%d %%%d", returnType, arg.name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> fwidthIntrinsics =
    {
        MAKE_FLOAT_INTRINSICS(FWidth)
    };
    for (auto fun : fwidthIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            assert(args.size() == 1);
            SPIRVResult arg = LoadValueSPIRV(c, g, args[0]);
            g->writer->MappedInstruction(OpFwidth, SPVWriter::Section::LocalFunction, returnType, arg);
            uint32_t ret = g->AddMappedOp(Format("OpFwidth %%%d %%%d", returnType, arg.name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> castIntrinsics =
    {
        MAKE_FLOAT_INTRINSICS(CastToU32),
        MAKE_FLOAT_INTRINSICS(CastToI32),
        MAKE_INT_INTRINSICS(CastToF32)
    };
    for (auto fun : castIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [op = std::get<1>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            assert(args.size() == 1);
            SPIRVResult arg = LoadValueSPIRV(c, g, args[0]);
            g->writer->MappedInstruction(OpBitcast, SPVWriter::Section::LocalFunction, returnType, arg);
            uint32_t ret = g->AddMappedOp(Format("OpBitcast %%%d %%%d", returnType, arg.name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> allIntrinsics =
    {
        MAKE_BOOL_INTRINSICS(All)
    };
    for (auto fun : allIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>&args) -> SPIRVResult
        {
            assert(args.size() == 1);
            SPIRVResult arg = LoadValueSPIRV(c, g, args[0]);
            g->writer->MappedInstruction(OpAll, SPVWriter::Section::LocalFunction, returnType, arg);
            uint32_t ret = g->AddMappedOp(Format("OpAll %%%d %%%d", returnType, arg.name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> anyIntrinsics =
    {
        MAKE_BOOL_INTRINSICS(Any)
    };
    for (auto fun : anyIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>&args) -> SPIRVResult
        {
            assert(args.size() == 1);
            SPIRVResult arg = LoadValueSPIRV(c, g, args[0]);
            g->writer->MappedInstruction(OpAny, SPVWriter::Section::LocalFunction, returnType, arg);
            uint32_t ret = g->AddMappedOp(Format("OpAny %%%d %%%d", returnType, arg.name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> transposeIntrinsics =
    {
        MAKE_MAT_INTRINSICS(Transpose)
    };
    for (auto fun : transposeIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>&args) -> SPIRVResult
        {
            assert(args.size() == 1);
            SPIRVResult arg = LoadValueSPIRV(c, g, args[0]);
            g->writer->MappedInstruction(OpTranspose, SPVWriter::Section::LocalFunction, returnType, arg);
            uint32_t ret = g->AddMappedOp(Format("OpTranspose %%%d %%%d", returnType, arg.name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char>> inverseIntrinsics =
    {
        MAKE_MAT_INTRINSICS(Inverse)
    };
    for (auto fun : inverseIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>&args) -> SPIRVResult
        {
            assert(args.size() == 1);
            SPIRVResult arg = LoadValueSPIRV(c, g, args[0]);
            uint32_t ext = g->ImportExtension("GLSL.std.450");
            g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), MatrixInverse, arg);
            uint32_t ret = g->AddMappedOp(Format("OpExtInst %%%d %%%d MatrixInverse %%%d", returnType, ext, arg.name));
            return SPIRVResult(ret, returnType, true);
        };
    }
    
    SPIRVGenerator::IntrinsicMap[Intrinsics::GetOutputLayer] = [](const Compiler* c, SPIRVGenerator * g, uint32_t returnType, const std::vector<SPIRVResult>&args) -> SPIRVResult
    {
        g->AddCapability("ShaderLayer");
        uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt, 1);
        uint32_t typePtr = g->AddSymbol("ptr_u32_Input", Format("OpTypePointer Input %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplOutputLayer", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplOutputLayer", ret, "BuiltIn Layer");
        SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);
        res.parentTypes.push_back(baseType);
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::SetOutputLayer] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->AddCapability("ShaderLayer");
        uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt, 1);
        uint32_t typePtr = g->AddSymbol("ptr_u32)Output", Format("OpTypePointer Output %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplOutputLayer", Format("OpVariable %%%d Output", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplOutputLayer", ret, "BuiltIn Layer");
        g->AddOp(Format("OpStore %%%d %%%d", ret, args[0].name));
        return SPIRVResult::Invalid();
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::GetOutputViewport] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult 
    {
        g->AddCapability("ShaderViewportIndex");
        uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt, 1);
        uint32_t typePtr = g->AddSymbol("ptr_u32_Input", Format("OpTypePointer Input %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplOutputViewport", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplOutputViewport", ret, "BuiltIn ViewportIndex");
        SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);
        res.parentTypes.push_back(baseType);
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::SetOutputViewport] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->AddCapability("ShaderViewportIndex");
        uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt, 1);
        uint32_t typePtr = g->AddSymbol("ptr_u32)Output", Format("OpTypePointer Output %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplOutputViewport", Format("OpVariable %%%d Output", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplOutputViewport", ret, "BuiltIn ViewportIndex");
        g->AddOp(Format("OpStore %%%d %%%d", ret, args[0].name));
        return SPIRVResult::Invalid();
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::ExportVertexCoordinates] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->AddCapability("Shader");
        uint32_t vecType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, 4);
        uint32_t typePtr = g->AddSymbol("ptr_f32x4)Output", Format("OpTypePointer Output %%%d", vecType), true);
        uint32_t ret = g->AddSymbol("gplVertexCoordinates", Format("OpVariable %%%d Output", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplVertexCoordinates", ret, "BuiltIn Position");
        SPIRVResult loaded = LoadValueSPIRV(c, g, args[0]);
        g->AddOp(Format("OpStore %%%d %%%d", ret, loaded.name), false, "gplExportVertexCoordinates");
        return SPIRVResult::Invalid();
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::ExportVertex] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->AddCapability("Geometry");
        g->AddOp("OpEmitVertex");
        return SPIRVResult::Invalid();
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::ExportPrimitive] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->AddCapability("Geometry");
        g->AddOp("OpEndPrimitive");
        return SPIRVResult::Invalid();
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::GetVertexIndex] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->AddCapability("Shader");
        uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt, 1);
        uint32_t typePtr = g->AddSymbol("ptr_u32_Input", Format("OpTypePointer Input %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplVertexIndex", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplVertexIndex", ret, "BuiltIn VertexId");
        SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);
        res.parentTypes.push_back(baseType);
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::GetInstanceIndex] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->AddCapability("Shader");
        uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt, 1);
        uint32_t typePtr = g->AddSymbol("ptr_u32_Input", Format("OpTypePointer Input %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplInstanceIndex", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplInstanceIndex", ret, "BuiltIn InstanceId");
        SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);
        res.parentTypes.push_back(baseType);
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::GetPixelCoordinates] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->AddCapability("Shader");
        uint32_t vecType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, 4);
        uint32_t typePtr = g->AddSymbol("ptr_f32x4_Input", Format("OpTypePointer Input %%%d", vecType), true);
        uint32_t ret = g->AddSymbol("gplPixelCoordinates", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplPixelCoordinates", ret, "BuiltIn FragCoord");
        SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);
        res.parentTypes.push_back(vecType);
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::SetPixelDepth] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->AddCapability("Shader");
        uint32_t baseType = g->AddSymbol("f32", "OpTypeFloat 32", true);
        uint32_t typePtr = g->AddSymbol("ptr_f32x4_Output", Format("OpTypePointer Output %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplSetPixelDepth", Format("OpVariable %%%d Output", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplSetPixelDepth", ret, "BuiltIn FragDepth");
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
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [baseTy = std::get<1>(fun), vecSize = std::get<2>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            g->AddCapability("Shader");
            uint32_t baseType = GeneratePODTypeSPIRV(c, g, baseTy, vecSize);
            std::string name = Type::CodeToString(baseTy);
            std::string typeSymbolName = vecSize > 1 ? Format("ptr_%sx%d_Output", name.c_str(), vecSize) : Format("ptr_%s", name.c_str());
           
            assert(args[1].isLiteral);
            uint32_t typePtr = g->AddSymbol(typeSymbolName, Format("OpTypePointer Output %%%d", baseType), true);
            uint32_t ret = g->AddSymbol(Format("gplExportColor%d", args[1].literalValue.i), Format("OpVariable %%%d Output", typePtr), true);
            g->interfaceVariables.insert(ret);
            g->AddDecoration(Format("gplExportColorIndex%d", args[1].literalValue.i), ret, Format("Index %d", args[1].literalValue.i));
            g->AddDecoration(Format("gplExportColorLocation%d", args[1].literalValue.i), ret, Format("Location %d", args[1].literalValue.i));
            SPIRVResult loaded = LoadValueSPIRV(c, g, args[0]);
            g->AddOp(Format("OpStore %%%d %%%d", ret, loaded.name), false, Format("gplExportColor%d", args[1].literalValue.i));
            return SPIRVResult::Invalid();
        };
    }

    SPIRVGenerator::IntrinsicMap[Intrinsics::GetLocalInvocationIndex] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        Type::FullType fullType = Type::FullType{ "u32x3" };
        fullType.AddModifier(Type::FullType::Modifier::Pointer);
        Type* typeSymbol = c->GetType(fullType);
        SPIRVResult type = GenerateTypeSPIRV(c, g, fullType, typeSymbol, SPIRVResult::Storage::Input);
        uint32_t ret = g->AddSymbol("gplLocalInvocationIndices", Format("OpVariable %%%d Input", type.typeName), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplLocalInvocationIndices", ret, "BuiltIn LocalInvocationId");
        SPIRVResult res = type;
        type.name = ret;
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::GetGlobalInvocationIndex] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        Type::FullType fullType = Type::FullType{ "u32x3" };
        fullType.AddModifier(Type::FullType::Modifier::Pointer);
        Type* typeSymbol = c->GetType(fullType);
        SPIRVResult type = GenerateTypeSPIRV(c, g, fullType, typeSymbol, SPIRVResult::Storage::Input);
        uint32_t ret = g->AddSymbol("gplGlobalInvocationIndices", Format("OpVariable %%%d Input", type.typeName), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplGlobalInvocationIndices", ret, "BuiltIn GlobalInvocationId");
        SPIRVResult res = type;
        type.name = ret;
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::GetWorkGroupIndex] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        Type::FullType fullType = Type::FullType{ "u32x3" };
        fullType.AddModifier(Type::FullType::Modifier::Pointer);
        Type* typeSymbol = c->GetType(fullType);
        SPIRVResult type = GenerateTypeSPIRV(c, g, fullType, typeSymbol, SPIRVResult::Storage::Input);
        uint32_t ret = g->AddSymbol("gplWorkGroupIndices", Format("OpVariable %%%d Input", type.typeName), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplWorkGroupIndices", ret, "BuiltIn WorkgroupId");
        SPIRVResult res = type;
        type.name = ret;
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::GetWorkGroupDimensions] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        Type::FullType fullType = Type::FullType{ "u32x3" };
        fullType.AddModifier(Type::FullType::Modifier::Pointer);
        Type* typeSymbol = c->GetType(fullType);
        SPIRVResult type = GenerateTypeSPIRV(c, g, fullType, typeSymbol, SPIRVResult::Storage::Input);
        uint32_t ret = g->AddSymbol("gplWorkGroupDimensions", Format("OpVariable %%%d Input", type.typeName), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplWorkGroupDimensions", ret, "BuiltIn WorkgroupSize");
        SPIRVResult res = type;
        type.name = ret;
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::GetSubgroupId] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniform");
        Type::FullType fullType = Type::FullType{ "u32x3" };
        fullType.AddModifier(Type::FullType::Modifier::Pointer);
        Type* typeSymbol = c->GetType(fullType);
        SPIRVResult type = GenerateTypeSPIRV(c, g, fullType, typeSymbol, SPIRVResult::Storage::Input);
        uint32_t typePtr = g->AddSymbol("ptr_u32x3_Input", Format("OpTypePointer Input %%%d", type.typeName), true);
        uint32_t ret = g->AddSymbol("gplSubgroupId", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplSubgroupId", ret, "BuiltIn SubgroupId");
        SPIRVResult res = type;
        type.name = ret;
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::GetSubgroupSize] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniform");
        Type::FullType fullType = Type::FullType{ "u32x3" };
        fullType.AddModifier(Type::FullType::Modifier::Pointer);
        Type* typeSymbol = c->GetType(fullType);
        SPIRVResult type = GenerateTypeSPIRV(c, g, fullType, typeSymbol, SPIRVResult::Storage::Input);
        uint32_t ret = g->AddSymbol("gplSubgroupSize", Format("OpVariable %%%d Input", type.typeName), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplSubgroupSize", ret, "BuiltIn SubgroupSize");
        SPIRVResult res = type;
        type.name = ret;
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::GetNumSubgroups] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniform");
        Type::FullType fullType = Type::FullType{ "u32x3" };
        fullType.AddModifier(Type::FullType::Modifier::Pointer);
        Type* typeSymbol = c->GetType(fullType);
        SPIRVResult type = GenerateTypeSPIRV(c, g, fullType, typeSymbol, SPIRVResult::Storage::Input);
        uint32_t ret = g->AddSymbol("gplNumSubgroups", Format("OpVariable %%%d Input", type.typeName), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplNumSubgroups", ret, "BuiltIn NumSubgroups");
        SPIRVResult res = type;
        type.name = ret;
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::GetSubgroupLocalInvocationMask] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniform");
        Type::FullType fullType = Type::FullType{ "u32x4" };
        fullType.AddModifier(Type::FullType::Modifier::Pointer);
        Type* typeSymbol = c->GetType(fullType);
        SPIRVResult type = GenerateTypeSPIRV(c, g, fullType, typeSymbol, SPIRVResult::Storage::Input);
        uint32_t ret = g->AddSymbol("gplSubgroupLocalInvocationMask", Format("OpVariable %%%d Input", type.typeName), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplSubgroupLocalInvocationMask", ret, "BuiltIn SubgroupEqMask");
        SPIRVResult res = type;
        type.name = ret;
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::GetSubgroupLocalInvocationAndLowerMask] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniform");
        Type::FullType fullType = Type::FullType{ "u32x4" };
        fullType.AddModifier(Type::FullType::Modifier::Pointer);
        Type* typeSymbol = c->GetType(fullType);
        SPIRVResult type = GenerateTypeSPIRV(c, g, fullType, typeSymbol, SPIRVResult::Storage::Input);
        uint32_t ret = g->AddSymbol("gplSubgroupLocalInvocationAndLowerMask", Format("OpVariable %%%d Input", type.typeName), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplSubgroupLocalInvocationAndLowerMask", ret, "BuiltIn SubgroupLeMask");
        SPIRVResult res = type;
        type.name = ret;
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::GetSubgroupLowerMask] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniform");
        Type::FullType fullType = Type::FullType{ "u32x4" };
        fullType.AddModifier(Type::FullType::Modifier::Pointer);
        Type* typeSymbol = c->GetType(fullType);
        SPIRVResult type = GenerateTypeSPIRV(c, g, fullType, typeSymbol, SPIRVResult::Storage::Input);
        uint32_t ret = g->AddSymbol("gplGetSubgroupLowerMask", Format("OpVariable %%%d Input", type.typeName), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplSubgroupLowerMask", ret, "BuiltIn SubgroupLtMask");
        SPIRVResult res = type;
        type.name = ret;
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::GetSubgroupLocalInvocationAndGreaterMask] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniform");
        Type::FullType fullType = Type::FullType{ "u32x4" };
        fullType.AddModifier(Type::FullType::Modifier::Pointer);
        Type* typeSymbol = c->GetType(fullType);
        SPIRVResult type = GenerateTypeSPIRV(c, g, fullType, typeSymbol, SPIRVResult::Storage::Input);
        uint32_t ret = g->AddSymbol("gplSubgroupLocalInvocationAndGreaterMask", Format("OpVariable %%%d Input", type.typeName), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplSubgroupLocalInvocationAndGreaterMask", ret, "BuiltIn SubgroupGeMask");
        SPIRVResult res = type;
        type.name = ret;
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::GetSubgroupGreaterMask] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniform");
        Type::FullType fullType = Type::FullType{ "u32x4" };
        fullType.AddModifier(Type::FullType::Modifier::Pointer);
        Type* typeSymbol = c->GetType(fullType);
        SPIRVResult type = GenerateTypeSPIRV(c, g, fullType, typeSymbol, SPIRVResult::Storage::Input);
        uint32_t ret = g->AddSymbol("gplSubgroupGreaterMask", Format("OpVariable %%%d Input", type.typeName), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplSubgroupGreaterMask", ret, "BuiltIn SubgroupGtMask");
        SPIRVResult res = type;
        type.name = ret;
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::SubgroupFirstInvocation] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniform");
        uint32_t ret = g->AddMappedOp(Format("OpGroupNonUniformElect %%%d Subgroup", returnType));
        return SPIRVResult(ret, returnType);
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::SubgroupRead] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniformBallot");
        uint32_t ret = g->AddMappedOp(Format("OpGroupNonUniformBroadcastFirst %%%d Subgroup %%%d", returnType, args[0].name));
        return SPIRVResult(ret, returnType);
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::SubgroupBallot] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniformBallot");
        uint32_t ret = g->AddMappedOp(Format("OpGroupNonUniformBallot %%%d Subgroup %%%d", returnType, args[0].name));
        return SPIRVResult(ret, returnType);
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::SubgroupInverseBallot] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniformBallot");
        uint32_t ret = g->AddMappedOp(Format("OpGroupNonUniformInverseBallot %%%d Subgroup %%%d", returnType, args[0].name));
        return SPIRVResult(ret, returnType);
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::SubgroupBallotBitCount] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniformBallot");
        uint32_t ret = g->AddMappedOp(Format("OpGroupNonUniformBallotBitCount %%%d Subgroup Reduce %%%d", returnType, args[0].name));
        return SPIRVResult(ret, returnType);
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::SubgroupBallotBit] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniformBallot");
        uint32_t ret = g->AddMappedOp(Format("OpGroupNonUniformBallotBitExtract %%%d Subgroup %%%d %%%d", returnType, args[0].name, args[1].name));
        return SPIRVResult(ret, returnType);
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::SubgroupBallotFirstOne] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniformBallot");
        uint32_t ret = g->AddMappedOp(Format("OpGroupNonUniformBallotFindLSB %%%d Subgroup %%%d", returnType, args[0].name));
        return SPIRVResult(ret, returnType);
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::SubgroupBallotLastOne] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("GroupNonUniformBallot");
        uint32_t ret = g->AddMappedOp(Format("OpGroupNonUniformBallotFindMSB %%%d Subgroup %%%d", returnType, args[0].name));
        return SPIRVResult(ret, returnType);
    };

    std::vector<std::tuple<Function*, const char>> subgroupSwapDiagonalIntrinsics =
    {
        MAKE_SCALAR_INTRINSICS(SubgroupSwapDiagonal)
    };
    for (auto fun : subgroupSwapDiagonalIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] =  [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            assert(args.size() == 1);
            uint32_t ret = g->AddMappedOp(Format("OpGroupNonUniformQuadSwap %%%d Subgroup %%%d 2", returnType, args[0].name));
            return SPIRVResult(ret, returnType);
        };
    }

    std::vector<std::tuple<Function*, const char>> subgroupSwapVerticalIntrinsics =
    {
        MAKE_SCALAR_INTRINSICS(SubgroupSwapVertical)
    };
    for (auto fun : subgroupSwapVerticalIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] =  [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            assert(args.size() == 1);
            uint32_t ret = g->AddMappedOp(Format("OpGroupNonUniformQuadSwap %%%d Subgroup %%%d 1", returnType, args[0].name));
            return SPIRVResult(ret, returnType);
        };
    }

    std::vector<std::tuple<Function*, const char>> subgroupSwapHorizontalIntrinsics =
    {
        MAKE_SCALAR_INTRINSICS(SubgroupSwapHorizontal)
    };
    for (auto fun : subgroupSwapHorizontalIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] =  [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            assert(args.size() == 1);
            uint32_t ret = g->AddMappedOp(Format("OpGroupNonUniformQuadSwap %%%d Subgroup %%%d 0", returnType, args[0].name));
            return SPIRVResult(ret, returnType);
        };
    }

    static const uint32_t SemanticsTable[] =
    {
        0x0,
        0x1,
        0x2,
        0x4,
        0x8,
        0x10,
        0x20,
        0x40,
        0x80,
        0x100
    };

    static auto ScopeToAtomicScope = [](SPIRVResult::Storage scope) -> uint32_t
    {
        switch (scope)
        {
            case SPIRVResult::Storage::Function:
            case SPIRVResult::Storage::Input:
            case SPIRVResult::Storage::Output:
            case SPIRVResult::Storage::PushConstant:
            case SPIRVResult::Storage::Private:
                return 4;
            case SPIRVResult::Storage::WorkGroup:
                return 2;
            case SPIRVResult::Storage::Device:
            case SPIRVResult::Storage::Uniform:
            case SPIRVResult::Storage::UniformConstant:
            case SPIRVResult::Storage::StorageBuffer:
            case SPIRVResult::Storage::MutableImage:
                return 1;
            case SPIRVResult::Storage::Sampler:
            case SPIRVResult::Storage::Image:
                assert(false);
        }
        return 0;
    };

    static auto ScopeToMemorySemantics = [](SPIRVResult::Storage scope) -> uint32_t
    {
        switch (scope)
        {
            case SPIRVResult::Storage::WorkGroup:
                return 0x100; // WorkgroupMemory
            case SPIRVResult::Storage::Uniform:
            case SPIRVResult::Storage::UniformConstant:
            case SPIRVResult::Storage::StorageBuffer:
            case SPIRVResult::Storage::Sampler:
                return 0x40; // UniformMemory
            case SPIRVResult::Storage::Image:
            case SPIRVResult::Storage::MutableImage:
                return 0x800; // ImageMemory
        }
        return 0x0;
    };

#define MAKE_ATOMIC_SCALAR_INTRINSICS(op)\
    { Intrinsics::Atomic##op##_f32, #op }\
    , { Intrinsics::Atomic##op##_i32, #op }\
    , { Intrinsics::Atomic##op##_u32, #op }\

#define MAKE_ATOMIC_INT_INTRINSICS(op)\
    { Intrinsics::Atomic##op##_i32, #op }\
    , { Intrinsics::Atomic##op##_u32, #op }\

#define MAKE_ATOMIC_SIGNED_INT_INTRINSICS(op)\
    { Intrinsics::Atomic##op##_i32, 'S', #op }\
    , { Intrinsics::Atomic##op##_u32, 'U', #op }\
    
    std::vector<std::tuple<Function*, const char*>> atomicLoadIntrinsics =
    {
        MAKE_ATOMIC_SCALAR_INTRINSICS(Load)
    };
    for (auto fun : atomicLoadIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] =  [op = std::get<1>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            assert(args.size() == 2);
            assert(args[1].isLiteral);
            
            uint32_t scope = ScopeToAtomicScope(args[0].scope);
            uint32_t semantics = SemanticsTable[args[1].literalValue.ui];
            semantics |= ScopeToMemorySemantics(args[0].scope);

            TStr opStr = TStr::Compact("OpAtomic", op);
            
            SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(scope));
            SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(semantics));
            uint32_t ret = g->AddMappedOp(Format("OpAtomic%s %%%d %%%d %%%d %%%d", op, returnType, args[0].name, scopeId.name, semanticsId.name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char*>> atomicStoreIntrinsics =
    {
        MAKE_ATOMIC_SCALAR_INTRINSICS(Store)
    };
    for (auto fun : atomicStoreIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] =  [op = std::get<1>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            assert(args.size() == 3);
            assert(args[1].isLiteral);
            
            uint32_t scope = ScopeToAtomicScope(args[0].scope);
            uint32_t semantics = SemanticsTable[args[2].literalValue.ui];
            semantics |= ScopeToMemorySemantics(args[0].scope);

            uint32_t ptr = AccessChainSPIRV(c, g, args[0].name, args[0].typeName, args[0].accessChain);
            SPIRVResult valueLoaded = LoadValueSPIRV(c, g, args[1]);

            SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(scope));
            SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(semantics));
            uint32_t ret = g->AddMappedOp(Format("OpAtomic%s %%%d %%%d %%%d %%%d", op, ptr, scopeId.name, semanticsId.name, valueLoaded.name));
            return SPIRVResult(ret, returnType);
        };
    }

    std::vector<std::tuple<Function*, const char*>> atomicExchangeIntrinsics =
    {
        MAKE_ATOMIC_SCALAR_INTRINSICS(Exchange)
    };
    for (auto fun : atomicExchangeIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] =  [op = std::get<1>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            assert(args.size() == 3);
            assert(args[1].isLiteral);
            
            uint32_t scope = ScopeToAtomicScope(args[0].scope);
            uint32_t semantics = SemanticsTable[args[2].literalValue.ui];
            semantics |= ScopeToMemorySemantics(args[0].scope);

            uint32_t ptr = AccessChainSPIRV(c, g, args[0].name, args[0].typeName, args[0].accessChain);
            SPIRVResult valueLoaded = LoadValueSPIRV(c, g, args[1]);

            SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(scope));
            SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(semantics));
            uint32_t ret = g->AddMappedOp(Format("OpAtomic%s %%%d %%%d %%%d %%%d %%%d", op, returnType, ptr, scopeId.name, semanticsId.name, valueLoaded.name));
            return SPIRVResult(ret, returnType, true);
        };
    }
    
    std::vector<std::tuple<Function*, const char*>> atomicCompareExchangeIntrinsics =
    {
        MAKE_ATOMIC_INT_INTRINSICS(CompareExchange)
    };
    for (auto fun : atomicCompareExchangeIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] =  [op = std::get<1>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            assert(args.size() == 3);
            assert(args[1].isLiteral);
            
            uint32_t scope = ScopeToAtomicScope(args[0].scope);
            uint32_t semantics = SemanticsTable[args[3].literalValue.ui];
            semantics |= ScopeToMemorySemantics(args[0].scope);

            uint32_t ptr = AccessChainSPIRV(c, g, args[0].name, args[0].typeName, args[0].accessChain);
            SPIRVResult value1Loaded = LoadValueSPIRV(c, g, args[1]);
            SPIRVResult value2Loaded = LoadValueSPIRV(c, g, args[2]);
            
            SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(scope));
            SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(semantics));
            uint32_t ret = g->AddMappedOp(Format("OpAtomic%s %%%d %%%d %%%d %%%d %%%d %%%d %%%d", op, returnType, ptr, scopeId.name, semanticsId.name, semanticsId.name, value1Loaded.name, value2Loaded.name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char*>> atomicIncrementDecrementIntrinsics =
    {
        MAKE_ATOMIC_INT_INTRINSICS(Increment)
        , MAKE_ATOMIC_INT_INTRINSICS(Decrement)
    };
    for (auto fun : atomicIncrementDecrementIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] =  [op = std::get<1>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            assert(args.size() == 2);
            assert(args[1].isLiteral);
            
            uint32_t scope = ScopeToAtomicScope(args[0].scope);
            uint32_t semantics = SemanticsTable[args[1].literalValue.ui];
            semantics |= ScopeToMemorySemantics(args[0].scope);

            uint32_t ptr = AccessChainSPIRV(c, g, args[0].name, args[0].typeName, args[0].accessChain);
            
            SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(scope));
            SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(semantics));
            uint32_t ret = g->AddMappedOp(Format("OpAtomicI%s %%%d %%%d %%%d %%%d %%%d", op, returnType, ptr, scopeId.name, semanticsId.name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char*>> atomicAddSubIntrinsics =
    {
        MAKE_ATOMIC_INT_INTRINSICS(Add)
        , MAKE_ATOMIC_INT_INTRINSICS(Sub)
    };
    for (auto fun : atomicAddSubIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] =  [op = std::get<1>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            assert(args.size() == 3);
            assert(args[2].isLiteral);

            uint32_t scope = ScopeToAtomicScope(args[0].scope);
            uint32_t semantics = SemanticsTable[args[2].literalValue.ui];
            semantics |= ScopeToMemorySemantics(args[0].scope);

            uint32_t ptr = AccessChainSPIRV(c, g, args[0].name, args[0].typeName, args[0].accessChain);
            SPIRVResult valueLoaded = LoadValueSPIRV(c, g, args[1]);
            
            SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(scope));
            SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(semantics));
            uint32_t ret = g->AddMappedOp(Format("OpAtomicI%s %%%d %%%d %%%d %%%d %%%d", op, returnType, ptr, scopeId.name, semanticsId.name, valueLoaded.name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char, const char*>> atomicMinMaxIntrinsics =
    {
        MAKE_ATOMIC_SIGNED_INT_INTRINSICS(Min)
        , MAKE_ATOMIC_SIGNED_INT_INTRINSICS(Max)
    };
    for (auto fun : atomicMinMaxIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] =  [sign = std::get<1>(fun), op = std::get<2>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            assert(args.size() == 2);
            assert(args[1].isLiteral);
            
            uint32_t scope = ScopeToAtomicScope(args[0].scope);
            uint32_t semantics = SemanticsTable[args[2].literalValue.ui];
            semantics |= ScopeToMemorySemantics(args[0].scope);

            uint32_t ptr = AccessChainSPIRV(c, g, args[0].name, args[0].typeName, args[0].accessChain);
            SPIRVResult valueLoaded = LoadValueSPIRV(c, g, args[1]);
            
            SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(scope));
            SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(semantics));
            uint32_t ret = g->AddMappedOp(Format("OpAtomic%c%s %%%d %%%d %%%d %%%d %%%d", sign, op, returnType, ptr, scopeId.name, semanticsId.name, valueLoaded.name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char*>> atomicAndOrXorIntrinsics =
{
        MAKE_ATOMIC_INT_INTRINSICS(And)
        , MAKE_ATOMIC_INT_INTRINSICS(Or)
        , MAKE_ATOMIC_INT_INTRINSICS(Xor)
    };
    for (auto fun : atomicAndOrXorIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] =  [op = std::get<1>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            assert(args.size() == 3);
            assert(args[2].isLiteral);
            
            uint32_t scope = ScopeToAtomicScope(args[0].scope);
            uint32_t semantics = SemanticsTable[args[2].literalValue.ui];
            semantics |= ScopeToMemorySemantics(args[0].scope);

            uint32_t ptr = AccessChainSPIRV(c, g, args[0].name, args[0].typeName, args[0].accessChain);
            SPIRVResult valueLoaded = LoadValueSPIRV(c, g, args[1]);
            
            SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(scope));
            SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(semantics));
            uint32_t ret = g->AddMappedOp(Format("OpAtomic%s %%%d %%%d %%%d %%%d %%%d", op, returnType, ptr, scopeId.name, semanticsId.name, valueLoaded.name));
            return SPIRVResult(ret, returnType, true);
        };
    }

    SPIRVGenerator::IntrinsicMap[Intrinsics::BitInsert] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->AddExtension("SPV_KHR_bit_instructions");
        //g->AddCapability("BitInstructions");
        SPIRVResult base = LoadValueSPIRV(c, g, args[0]);
        SPIRVResult insert = LoadValueSPIRV(c, g, args[1]);
        SPIRVResult offset = LoadValueSPIRV(c, g, args[2]);
        SPIRVResult count = LoadValueSPIRV(c, g, args[3]);
        uint32_t res = g->AddMappedOp(Format("OpBitFieldInsert %%%d %%%d %%%d %%%d %%%d", returnType, base.name, insert.name, offset.name, count.name));
        return SPIRVResult(res, returnType, true);
    };

    std::vector<std::tuple<Function*, char>> bitExtractFunctions =
    {
        { Intrinsics::BitSExtract, 'S' }
        , { Intrinsics::BitUExtract, 'U' }
    };
    for (auto fun : bitExtractFunctions)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [sign = std::get<1>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            g->AddExtension("SPV_KHR_bit_instructions");
            //g->AddCapability("BitInstructions");
            SPIRVResult base = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult offset = LoadValueSPIRV(c, g, args[1]);
            SPIRVResult count = LoadValueSPIRV(c, g, args[2]);
            uint32_t res = g->AddMappedOp(Format("OpBitField%cExtract %%%d %%%d %%%d %%%d", sign, returnType, base.name, offset.name, count.name));
            return SPIRVResult(res, returnType, true);
        };
    }

    SPIRVGenerator::IntrinsicMap[Intrinsics::BitReverse] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->AddExtension("SPV_KHR_bit_instructions");
        //g->AddCapability("BitInstructions");
        SPIRVResult base = LoadValueSPIRV(c, g, args[0]);
        uint32_t res = g->AddMappedOp(Format("OpBitReverse %%%d %%%d", returnType, base.name));
        return SPIRVResult(res, returnType, true);
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::BitCount] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->AddExtension("SPV_KHR_bit_instructions");
        //g->AddCapability("BitInstructions");
        SPIRVResult base = LoadValueSPIRV(c, g, args[0]);
        uint32_t res = g->AddMappedOp(Format("OpBitCount %%%d %%%d", returnType, base.name));
        return SPIRVResult(res, returnType, true);
    };
    
    SPIRVGenerator::IntrinsicMap[Intrinsics::ExecutionBarrier] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(2));
        SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(0x2 | 0x40 | 0x80 | 0x100 | 0x200 | 0x400 | 0x800));
        g->AddOp(Format("OpControlBarrier %%%d %%%d %%%d", scopeId.name, scopeId.name, semanticsId.name));
        return SPIRVResult(0xFFFFFFFF, returnType);
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::ExecutionBarrierSubgroup] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult 
    {
        SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(3));
        SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(0x2 | 0x80));
        g->AddOp(Format("OpControlBarrier %%%d %%%d %%%d", scopeId.name, scopeId.name, semanticsId.name));
        return SPIRVResult(0xFFFFFFFF, returnType);
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::ExecutionBarrierWorkgroup] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(2));
        SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(0x2 | 0x100));
        g->AddOp(Format("OpControlBarrier %%%d %%%d %%%d", scopeId.name, scopeId.name, semanticsId.name));
        return SPIRVResult(0xFFFFFFFF, returnType);
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::MemoryExecutionBarrier] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(2));
        SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(0x2 | 0x40 | 0x80 | 0x100 | 0x200 | 0x400 | 0x800));
        g->AddOp(Format("OpMemoryBarrier %%%d %%%d", scopeId.name, semanticsId.name));
        return SPIRVResult(0xFFFFFFFF, returnType);
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::MemoryExecutionBarrierSubgroup] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(3));
        SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(0x2 | 0x80));
        g->AddOp(Format("OpMemoryBarrier %%%d %%%d", scopeId.name, semanticsId.name));
        return SPIRVResult(0xFFFFFFFF, returnType);
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::MemoryExecutionBarrierWorkgroup] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(2));
        SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(0x2 | 0x100));
        g->AddOp(Format("OpMemoryBarrier %%%d %%%d", scopeId.name, semanticsId.name));
        return SPIRVResult(0xFFFFFFFF, returnType);
    };

    static auto createSampledImage = [](const Compiler* c, SPIRVGenerator* g, SPIRVResult arg0, SPIRVResult arg1) -> SPIRVResult
    {
        assert(arg0.parentTypes.size() > 0);
        SPIRVResult image = LoadValueSPIRV(c, g, arg0, true);
        SPIRVResult sampler = LoadValueSPIRV(c, g, arg1, true);
        uint32_t sampledImageType = g->AddSymbol(Format("sampledType(%d)", arg0.parentTypes[0]), Format("OpTypeSampledImage %%%d", arg0.parentTypes[0]), true);
        uint32_t sampledImage = g->AddMappedOp(Format("OpSampledImage %%%d %%%d %%%d", sampledImageType, image.name, sampler.name));
        return SPIRVResult(sampledImage, sampledImage, true);
    };

    std::vector<std::tuple<Function*, bool>> textureGetSizeIntrinsics =
    {
        { Intrinsics::TextureGetSize_texture1D, false }
        , { Intrinsics::TextureGetSize_texture1DArray, false }
        , { Intrinsics::TextureGetSize_texture2D, false }
        , { Intrinsics::TextureGetSize_texture2DArray, false }
        , { Intrinsics::TextureGetSize_textureCube, false }
        , { Intrinsics::TextureGetSize_textureCubeArray, false }
        , { Intrinsics::TextureGetSize_texture3D, false }
        , { Intrinsics::TextureGetSizeMip_texture1D, true }
        , { Intrinsics::TextureGetSizeMip_texture1DArray, true }
        , { Intrinsics::TextureGetSizeMip_texture2D, true }
        , { Intrinsics::TextureGetSizeMip_texture2DArray, true }
        , { Intrinsics::TextureGetSizeMip_textureCube, true }
        , { Intrinsics::TextureGetSizeMip_textureCubeArray, true }
        , { Intrinsics::TextureGetSizeMip_texture3D, true }
    };
    for (auto fun : textureGetSizeIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [mip = std::get<1>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            g->AddCapability("ImageQuery");
            uint32_t image = g->AddMappedOp(Format("OpLoad %%%d %%%d", args[0].parentTypes[0], args[0].name));
            uint32_t ret;
            if (mip)
                ret = g->AddMappedOp(Format("OpImageQuerySizeLod %%%d %%%d %%%d", returnType, image, args[1].name));
            else
            {
                SPIRVResult zero = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(0));
                ret = g->AddMappedOp(Format("OpImageQuerySizeLod %%%d %%%d %%%d", returnType, image, zero.name));
            }
                //ret = g->AddMappedOp(Format("OpImageQuerySize %%%d %%%d", returnType, image));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*>> textureGetSampledMipIntrinsics =
    {
        { Intrinsics::TextureGetSampledMip_texture1D }
        , { Intrinsics::TextureGetSampledMip_texture1DArray }
        , { Intrinsics::TextureGetSampledMip_texture2D }
        , { Intrinsics::TextureGetSampledMip_texture2DArray }
        , { Intrinsics::TextureGetSampledMip_textureCube }
        , { Intrinsics::TextureGetSampledMip_textureCubeArray }
        , { Intrinsics::TextureGetSampledMip_texture3D }
        , { Intrinsics::TextureGetSampledMip_texture3D }
    };
    for (auto fun : textureGetSampledMipIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            g->AddCapability("ImageQuery");
            uint32_t ret;
            SPIRVResult sampledImage = createSampledImage(c, g, args[0], args[1]);
            ret = g->AddMappedOp(Format("OpImageQueryLod %%%d %%%d %%%d", returnType, sampledImage.name, args[2].name));
            return SPIRVResult(ret, returnType, true);
        };
    }
    
    std::vector<std::tuple<Function*>> textureGetMips =
    {
        { Intrinsics::TextureGetMips_texture1D }
        , { Intrinsics::TextureGetMips_texture1DArray }
        , { Intrinsics::TextureGetMips_texture2D }
        , { Intrinsics::TextureGetMips_texture2DArray }
        , { Intrinsics::TextureGetMips_textureCube }
        , { Intrinsics::TextureGetMips_textureCubeArray }
        , { Intrinsics::TextureGetMips_texture3D }
        , { Intrinsics::TextureGetMips_texture3D }
    };
    for (auto fun : textureGetSampledMipIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            g->AddCapability("ImageQuery");
            uint32_t image = g->AddMappedOp(Format("OpLoad %%%d %%%d", args[0].parentTypes[0], args[0].name));
            uint32_t ret;
            ret = g->AddMappedOp(Format("OpImageQueryLevels %%%d %%%d", returnType, image));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*>> textureGetSamples =
    {
        { Intrinsics::TextureGetSamples_texture2DMS }
        , { Intrinsics::TextureGetSamples_texture2DMSArray }
    };
    for (auto fun : textureGetSampledMipIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            g->AddCapability("ImageQuery");
            uint32_t image = g->AddMappedOp(Format("OpLoad %%%d %%%d", args[0].parentTypes[0], args[0].name));
            uint32_t ret;
            ret = g->AddMappedOp(Format("OpImageQuerySamples %%%d %%%d", returnType, image));
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
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [sample = std::get<1>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            uint32_t ret;

            SPIRVResult image = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult coord = LoadValueSPIRV(c, g, args[1]);
            if (sample)
                ret = g->AddMappedOp(Format("OpImageFetch %%%d %%%d %%%d Sample %d", returnType, image.name, coord.name, args[2].name));
            else
                ret = g->AddMappedOp(Format("OpImageFetch %%%d %%%d %%%d None", returnType, image.name, coord.name));
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
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [offsets = std::get<1>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            uint32_t ret;
            uint32_t image = g->AddMappedOp(Format("OpLoad %%%d %%%d", args[0].parentTypes[0], args[0].name));
            SPIRVResult coord = LoadValueSPIRV(c, g, args[1]);
            if (offsets)
                ret = g->AddMappedOp(Format("OpImageGather %%%d %%%d %%%d %%%d ConstOffsets %%%d", returnType, image, args[1].name, args[2].name));
            else
                ret = g->AddMappedOp(Format("OpImageGather %%%d %%%d %%%d %%%d None", returnType, image, args[1].name));
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
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [operands = std::get<1>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            uint32_t ret = 0xFFFFFFFF;
            SPIRVResult sampledImage = createSampledImage(c, g, args[0], args[1]);

            std::string format = "";
            if (operands & Lod || operands & Grad)
                if (operands & Comp)
                    if (operands & Grad)
                        if (operands & Proj)
                            if (operands & Bias)
                                format = "OpImageSampleProjDrefExplicitLod %%%d %%%d %%%d %%%d Grad %%%d %%%d Bias %%%d";
                            else
                                format = "OpImageSampleProjDrefExplicitLod %%%d %%%d %%%d %%%d Grad %%%d %%%d";
                        else
                            if (operands & Bias)
                                format = "OpImageSampleDrefExplicitLod %%%d %%%d %%%d %%%d Grad %%%d %%%d Bias %%%d";
                            else
                                format = "OpImageSampleDrefExplicitLod %%%d %%%d %%%d %%%d Grad %%%d %%%d";
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
                                format = "OpImageSampleProjExplicitLod %%%d %%%d %%%d Grad %%%d %%%d Bias %%%d";
                            else
                                format = "OpImageSampleProjExplicitLod %%%d %%%d %%%d Grad %%%d %%%d";
                        else
                            if (operands & Bias)
                                format = "OpImageSampleExplicitLod %%%d %%%d %%%d Grad %%%d %%%d Bias %%%d";
                            else
                                format = "OpImageSampleExplicitLod %%%d %%%d %%%d Grad %%%d %%%d";
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

            std::vector<uint32_t> newArgs = { returnType, sampledImage.name };
            
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
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [operands = std::get<1>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            uint32_t ret = 0xFFFFFFFF;
            assert(args[0].parentTypes.size() > 0);
            uint32_t sampledImage = LoadValueSPIRV(c, g, args[0]).name;
            //uint32_t sampledImage = g->AddMappedOp(Format("OpLoad %%%d %%%d", args[0].parentTypes[0], args[0].name));

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
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [store = std::get<1>(fun), operands = std::get<2>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
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
                        SPIRVResult loadedPointer = LoadValueSPIRV(c, g, args[0]);
                        dereffed = LoadValueSPIRV(c, g, loadedPointer, true).name;
                    }
                    else
                    {
                        dereffed = LoadValueSPIRV(c, g, args[0], true).name;
                    }
                    SPIRVResult loadedCoord = LoadValueSPIRV(c, g, args[1]);
                    SPIRVResult loadedValue = LoadValueSPIRV(c, g, args[2]);
                    SPIRVResult loadedMip = LoadValueSPIRV(c, g, args[3]);
                    g->AddOp(Format("OpImageWrite %%%d %%%d %%%d Lod %%%d", dereffed, loadedCoord.name, loadedValue.name, loadedMip.name));
                }
                else
                {
                    assert(args.size() == 3);
                    assert(args[0].parentTypes.size() > 0);
                    uint32_t dereffed;
                    if (c->target.supportsPhysicalAddressing)
                    {
                        SPIRVResult loadedPointer = LoadValueSPIRV(c, g, args[0]);
                        dereffed = LoadValueSPIRV(c, g, loadedPointer, true).name;
                    }
                    else
                    {
                        dereffed = LoadValueSPIRV(c, g, args[0], true).name;
                    }
                    SPIRVResult loadedCoord = LoadValueSPIRV(c, g, args[1]);
                    SPIRVResult loadedValue = LoadValueSPIRV(c, g, args[2]);
                    g->AddOp(Format("OpImageWrite %%%d %%%d %%%d", dereffed, loadedCoord.name, loadedValue.name));
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
                        SPIRVResult loadedPointer = LoadValueSPIRV(c, g, args[0]);
                        dereffed = LoadValueSPIRV(c, g, loadedPointer, true).name;
                    }
                    else
                    {
                        dereffed = LoadValueSPIRV(c, g, args[0], true).name;
                    }
                    SPIRVResult loadedCoord = LoadValueSPIRV(c, g, args[1]);
                    SPIRVResult loadedMip = LoadValueSPIRV(c, g, args[2]);
                    ret = g->AddMappedOp(Format("OpImageRead %%%d %%%d %%%d Lod %%%d", returnType, dereffed, loadedCoord.name, loadedMip.name));
                    res.isValue = true;
                }
                else
                {
                    assert(args.size() == 2);
                    assert(args[0].parentTypes.size() > 0);
                    uint32_t dereffed;
                    if (c->target.supportsPhysicalAddressing)
                    {
                        SPIRVResult loadedPointer = LoadValueSPIRV(c, g, args[0]);
                        dereffed = LoadValueSPIRV(c, g, loadedPointer, true).name;
                    }
                    else
                    {
                        dereffed = LoadValueSPIRV(c, g, args[0], true).name;
                    }
                    SPIRVResult loadedCoord = LoadValueSPIRV(c, g, args[1]);
                    ret = g->AddMappedOp(Format("OpImageRead %%%d %%%d %%%d", returnType, dereffed, loadedCoord.name));
                    res.isValue = true;
                }
            }
            res.name = ret;
            res.typeName = returnType;
            return res;
        };
    }
    SPIRVGenerator::IntrinsicMap[Intrinsics::TraceRay] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        assert(args.size() == 11);
        g->AddCapability("RayTracingKHR");
        // Load acceleration structure with a special parent load type, as it's
        SPIRVResult acc = SPIRVResult::Invalid();
        if (c->target.supportsPhysicalAddressing)
        {
            // First load pointer
            acc = LoadValueSPIRV(c, g, args[0]);

            // Then load structure
            acc = LoadValueSPIRV(c,g, acc, true);
        }
        else
        {
            acc = LoadValueSPIRV(c, g, args[0], true);
        }
        std::string fmt;
        for (size_t i = 1; i < args.size() - 1; i++)
        {
            SPIRVResult loaded = LoadValueSPIRV(c, g, args[i]);
            fmt.append(Format("%%%d", loaded.name));
        }
        g->AddOp(Format("OpTraceRayKHR %%%d %s", fmt.c_str()));
        return SPIRVResult(0xFFFFFFFF, returnType);
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::ExportRayIntersection] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->AddCapability("RayTracingKHR");
        g->AddOp("OpReportIntersectionKHR");
        return SPIRVResult(0xFFFFFFFF, returnType);
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::ExecuteCallable] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        assert(args.size() == 2);
        g->AddCapability("RayTracingKHR");
        SPIRVResult shaderBindingIndex = LoadValueSPIRV(c, g, args[0]);
        SPIRVResult callableData = LoadValueSPIRV(c, g, args[1]);
        g->AddOp(Format("OpExecuteCallableKHR %%%d %%%d", shaderBindingIndex.name, callableData.name));
        return SPIRVResult(0xFFFFFFFF, returnType);
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::RayLaunchIndex] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("RayTracingKHR");
        uint32_t vecType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt, 3);
        uint32_t typePtr = g->AddSymbol("ptr_u32x3_Input", Format("OpTypePointer Input %%%d", vecType), true);
        uint32_t ret = g->AddSymbol("gplRayLaunchIndex", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplRayLaunchIndex", ret, "BuiltIn LaunchIdKHR");
        SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);
        res.parentTypes.push_back(vecType);
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::RayLaunchSize] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("RayTracingKHR");
        uint32_t vecType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt, 3);
        uint32_t typePtr = g->AddSymbol("ptr_u32x3_Input", Format("OpTypePointer Input %%%d", vecType), true);
        uint32_t ret = g->AddSymbol("gplRayLaunchSize", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplRayLaunchSize", ret, "BuiltIn LaunchSizeKHR");
        SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);
        res.parentTypes.push_back(vecType);
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::BLASPrimitiveIndex] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("RayTracingKHR");
        uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt, 1);
        uint32_t typePtr = g->AddSymbol("ptr_u32_Input", Format("OpTypePointer Input %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplBLASPrimitiveIndex", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplBLASPrimitiveIndex", ret, "BuiltIn PrimitiveId");
        SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);
        res.parentTypes.push_back(baseType);
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::BLASGeometryIndex] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("RayTracingKHR");
        uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt, 1);
        uint32_t typePtr = g->AddSymbol("ptr_u32_Input", Format("OpTypePointer Input %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplBLASGeometryIndex", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplBLASGeometryIndex", ret, "BuiltIn RayGeometryIndexKHR");
        SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);
        res.parentTypes.push_back(baseType);
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::TLASInstanceIndex] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("RayTracingKHR");
        uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt, 1);
        uint32_t typePtr = g->AddSymbol("ptr_u32_Input", Format("OpTypePointer Input %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplTLASInstanceIndex", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplTLASInstanceIndex", ret, "BuiltIn InstanceId");
        SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);
        res.parentTypes.push_back(baseType);
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::TLASInstanceCustomIndex] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("RayTracingKHR");
        uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt, 1);
        uint32_t typePtr = g->AddSymbol("ptr_u32_Input", Format("OpTypePointer Input %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplTLASInstanceCustomIndex", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplTLASInstanceCustomIndex", ret, "BuiltIn InstanceCustomIndexKHR");
        SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);
        res.parentTypes.push_back(baseType);
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::RayWorldOrigin] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("RayTracingKHR");
        uint32_t vecType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, 3);
        uint32_t typePtr = g->AddSymbol("ptr_f32x3_Input", Format("OpTypePointer Input %%%d", vecType), true);
        uint32_t ret = g->AddSymbol("gplRayWorldOrigin", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplRayWorldOrigin", ret, "BuiltIn WorldRayOriginKHR");
        SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);
        res.parentTypes.push_back(vecType);
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::RayWorldDirection] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("RayTracingKHR");
        uint32_t vecType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, 3);
        uint32_t typePtr = g->AddSymbol("ptr_f32x3_Input", Format("OpTypePointer Input %%%d", vecType), true);
        uint32_t ret = g->AddSymbol("gplRayWorldDirection", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplRayWorldDirection", ret, "BuiltIn WorldRayDirectionKHR");
        SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);
        res.parentTypes.push_back(vecType);
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::RayObjectOrigin] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("RayTracingKHR");
        uint32_t vecType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, 3);
        uint32_t typePtr = g->AddSymbol("ptr_f32x3_Input", Format("OpTypePointer Input %%%d", vecType), true);
        uint32_t ret = g->AddSymbol("gplRayObjectOrigin", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplRayObjectOrigin", ret, "BuiltIn ObjectRayOriginKHR");
        SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);
        res.parentTypes.push_back(vecType);
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::RayObjectDirection] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("RayTracingKHR");
        uint32_t vecType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, 3);
        uint32_t typePtr = g->AddSymbol("ptr_f32x3_Input", Format("OpTypePointer Input %%%d", vecType), true);
        uint32_t ret = g->AddSymbol("gplRayObjectDirection", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplRayObjectDirection", ret, "BuiltIn ObjectRayDirectionKHR");
        SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);
        res.parentTypes.push_back(vecType);
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::RayMin] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("RayTracingKHR");
        uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, 1);
        uint32_t typePtr = g->AddSymbol("ptr_f32_Input", Format("OpTypePointer Input %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplRayMin", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplRayMin", ret, "BuiltIn RayTminKHR");
        SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);
        res.parentTypes.push_back(baseType);
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::RayMax] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("RayTracingKHR");
        uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, 1);
        uint32_t typePtr = g->AddSymbol("ptr_f32_Input", Format("OpTypePointer Input %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplRayMax", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplRayMax", ret, "BuiltIn RayTmaxKHR");
        SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);
        res.parentTypes.push_back(baseType);
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::RayFlags] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("RayTracingKHR");
        uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt, 1);
        uint32_t typePtr = g->AddSymbol("ptr_u32_Input", Format("OpTypePointer Input %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplRayFlags", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplRayFlags", ret, "BuiltIn IncomingRayFlagsKHR");
        SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);
        res.parentTypes.push_back(baseType);
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::RayHitDistance] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("RayTracingKHR");
        uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, 1);
        uint32_t typePtr = g->AddSymbol("ptr_f32_Input", Format("OpTypePointer Input %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplRayHitDistance", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplRayHitDistance", ret, "BuiltIn RayTmaxKHR");
        SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);
        res.parentTypes.push_back(baseType);
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::RayHitKind] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("RayTracingKHR");
        uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt, 1);
        uint32_t typePtr = g->AddSymbol("ptr_u32_Input", Format("OpTypePointer Input %%%d", baseType), true);
        uint32_t ret = g->AddSymbol("gplRayHitKind", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplRayHitKind", ret, "BuiltIn HitKindKHR");
        SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);
        res.parentTypes.push_back(baseType);
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::TLASObjectToWorld] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("RayTracingKHR");
        uint32_t matType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, 4, 3);
        uint32_t typePtr = g->AddSymbol("ptr_f32x4x4)Input", Format("OpTypePointer Input %%%d", matType), true);
        uint32_t ret = g->AddSymbol("gplTLASObjectToWorld", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplTLASObjectToWorld", ret, "BuiltIn ObjectToWorldKHR");
        SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);
        res.parentTypes.push_back(matType);
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::TLASWorldToObject] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        g->AddCapability("RayTracingKHR");
        uint32_t matType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, 4, 3);
        uint32_t typePtr = g->AddSymbol("ptr_f32x4x4)Input", Format("OpTypePointer Input %%%d", matType), true);
        uint32_t ret = g->AddSymbol("gplTLASWorldToObject", Format("OpVariable %%%d Input", typePtr), true);
        g->interfaceVariables.insert(ret);
        g->AddDecoration("gplTLASWorldToObject", ret, "BuiltIn WorldToObjectKHR");
        SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);
        res.parentTypes.push_back(matType);
        return res;
    };
}

//------------------------------------------------------------------------------
/**
*/
void
GenerateFunctionSPIRV(const Compiler* compiler, SPIRVGenerator* generator, Symbol* symbol)
{
    Function* func = static_cast<Function*>(symbol);
    Function::__Resolved* funcResolved = Symbol::Resolved(func);
    if (funcResolved->isEntryPoint && generator->entryPoint != func)
        return;

    if (funcResolved->isPrototype)
        return;
    SPIRVResult returnName = GenerateTypeSPIRV(compiler, generator, func->returnType, static_cast<Type*>(funcResolved->returnTypeSymbol));

    TStr typeArgs;
    TStr spvTypes;

    uint8_t spvTypeArgIt = 0;
    SPVArg* spvTypeArgs = AllocStack<SPVArg>(func->parameters.size());

    for (auto param : func->parameters)
    {
        Variable::__Resolved* paramResolved = Symbol::Resolved(param);

        if (funcResolved->isEntryPoint)
        {
            GenerateVariableSPIRV(compiler, generator, param, true, true);
        }
        else
        {
            SPIRVResult::Storage storage = ResolveSPIRVVariableStorage(paramResolved->type, paramResolved->typeSymbol, paramResolved->storage, paramResolved->usageBits);
            SPIRVResult typeName = GenerateTypeSPIRV(compiler, generator, paramResolved->type, paramResolved->typeSymbol, storage);
            typeArgs.Append(paramResolved->type.ToString());
            if (param != func->parameters.back())
                typeArgs.Append(",");

            spvTypes.Append(SPVArg(typeName.typeName));
            spvTypes.Append(" ");
            spvTypeArgs[spvTypeArgIt++] = SPVArg(typeName.typeName);
        }
    }
    
    TStr functionSymbolName = TStr::Compact("function_", typeArgs, "_", func->returnType.ToString());
    AddSymbol(generator, functionSymbolName, SPVWriter::Section::Declarations, OpTypeFunction, returnName.typeName, SPVArgList{.vals = spvTypeArgs, .num = spvTypeArgIt});
    DeallocStack(spvTypeArgIt, spvTypeArgs);
    
    uint32_t functionType = generator->AddSymbol(functionSymbolName, TStr::Separated("OpTypeFunction", SPVArg{returnName.typeName}, spvTypes), returnName, true);

    // TODO: Add inline/const/functional
    AddSymbol(generator, funcResolved->name, SPVWriter::Section::LocalFunction, OpFunction, returnName.typeName, FunctionControl::None, SPVArg{functionType});
    generator->AddSymbol(funcResolved->name, Format("OpFunction %%%d None %%%d", returnName.typeName, functionType));

    generator->PushScope();
    if (!funcResolved->isEntryPoint)
    {
        for (auto& param : func->parameters)
        {
            Variable::__Resolved* paramResolved = Symbol::Resolved(param);
            SPIRVResult::Storage storage = ResolveSPIRVVariableStorage(paramResolved->type, paramResolved->typeSymbol, paramResolved->storage, paramResolved->usageBits);
            SPIRVResult varType = GenerateTypeSPIRV(compiler, generator, paramResolved->type, paramResolved->typeSymbol, storage);

            // If value is not a pointer, generate a copy of the value inside the function
            if (!paramResolved->type.IsPointer())
            {
                MappedInstruction(generator, SPVWriter::Section::FunctionInit, OpFunctionParameter, varType.typeName);
                uint32_t paramName = generator->AddMappedOp(TStr::Separated("OpFunctionParameter", SPVArg(varType.typeName)), param->name);
                std::string type = paramResolved->type.ToString();
                ConstantString scope = SPIRVResult::ScopeToString(varType.scope);

                TStr argPtrType = TStr::Compact("ptr_", type, "_", scope);
                MappedInstruction(generator, SPVWriter::Section::Declarations, OpTypePointer, ScopeToEnum(varType.scope), SPVArg(varType.typeName));
                uint32_t typePtrName = generator->AddSymbol(argPtrType, TStr::Separated("OpTypePointer", scope, SPVArg(varType.typeName)), true);

                // Function parameters are values if they are not pointers
                varType.parentTypes.push_back(varType.typeName);
                varType.typeName = typePtrName;
                varType.isValue = false;
                AddSymbol(generator, param->name, SPVWriter::Section::FunctionInit, OpVariable, typePtrName, SPVArg{paramName}, VariableStorage::Function);
                generator->AddVariableDeclaration(param, param->name, typePtrName, 0xFFFFFFFF, paramName, SPIRVResult::Storage::Function, varType, false);
            }
            else
            {
                // If value is already a pointer, then any stores to it in the function should be visible externally
                AddSymbol(generator, param->name, SPVWriter::Section::FunctionInit, OpFunctionParameter, varType.typeName);
                generator->AddSymbol(param->name, TStr::Separated("OpFunctionParameter", SPVArg(varType.typeName)), varType);
            }
        }
    }

    generator->writer->MappedInstruction(OpLabel, SPVWriter::Section::LocalFunction);
    uint32_t label = generator->AddMappedOp("OpLabel");
    generator->blockOpen = true;
    generator->functions.Append(generator->functional);

    generator->functional.Clear();
    auto functionOverride = generator->evaluatingProgram->functionOverrides.find(func);
    if (functionOverride != generator->evaluatingProgram->functionOverrides.end())
        GenerateStatementSPIRV(compiler, generator, functionOverride->second->ast);
    else
        GenerateStatementSPIRV(compiler, generator, func->ast);
    generator->functions.Append(generator->variableDeclarations);
    generator->variableDeclarations.Clear();
    generator->functions.Append(generator->parameterInitializations);
    generator->parameterInitializations.Clear();
    if (generator->blockOpen)
    {
        if (!funcResolved->hasExplicitReturn)
        {
            generator->AddOp("OpReturn");
            generator->writer->Instruction(OpReturn, SPVWriter::Section::LocalFunction);
        }
        else
        {
            generator->AddOp("OpUnreachable");
            generator->writer->Instruction(OpUnreachable, SPVWriter::Section::LocalFunction);
        }
    }
    generator->blockOpen = false;
    generator->writer->Instruction(OpFunctionEnd, SPVWriter::Section::LocalFunction);
    generator->AddOp("OpFunctionEnd", false, Format("End of %s", func->name.c_str()));

    generator->writer->FinishFunction();
    generator->functions.Append(generator->functional);
    generator->functional.Clear();

    generator->PopScope();
}

//------------------------------------------------------------------------------
/**
*/
uint32_t
GenerateStructureSPIRV(const Compiler* compiler, SPIRVGenerator* generator, Symbol* symbol)
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
            SPIRVResult varType;
            if (varResolved->usageBits.flags.isPhysicalAddress)
            {
                // If physical address, generate a uint type
                varType.typeName = GeneratePODTypeSPIRV(compiler, generator, TypeCode::UInt);
            }
            else
            {
                varType = GenerateTypeSPIRV(compiler, generator, varResolved->type, varResolved->typeSymbol);
            }
            memberTypes.append(Format("%%%d ", varType.typeName));


            // If this struct is generated for binding 
            generator->writer->MemberDecorate(SPVArg{ structName }, i, Decorations::Offset, varResolved->structureOffset);
            generator->AddMemberDecoration(structName, i, Format("Offset %d", varResolved->structureOffset));
            if (varResolved->typeSymbol->IsMatrix())
            {
                generator->writer->MemberDecorate(SPVArg{ structName }, i, Decorations::MatrixStride, varResolved->typeSymbol->CalculateStride() / varResolved->typeSymbol->rowSize);
                generator->writer->MemberDecorate(SPVArg{ structName }, i, Decorations::ColMajor);

                generator->AddMemberDecoration(structName, i, Format("MatrixStride %d", varResolved->typeSymbol->CalculateStride() / varResolved->typeSymbol->rowSize));
                generator->AddMemberDecoration(structName, i, "ColMajor");
            }
            offset += varResolved->typeSymbol->CalculateSize();
        }
    }

    if (strucResolved->storageFunction != nullptr)
    {
        generator->generatorIntrinsics[strucResolved->storageFunction] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            assert(args.size() == 2);
            assert(!args[0].isValue);
            StoreValueSPIRV(c, g, args[0], args[1]);
            return SPIRVResult::Invalid();
        };
    }
    if (strucResolved->loadFunction != nullptr)
    {
        generator->generatorIntrinsics[strucResolved->loadFunction] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            assert(args.size() == 1);
            assert(!args[0].isValue);
            return LoadValueSPIRV(c, g, args[0]);
        };
    }
    generator->AddReservedSymbol(struc->name, structName, Format("OpTypeStruct %s", memberTypes.c_str()), true);
    return structName;
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateSamplerSPIRV(const Compiler* compiler, SPIRVGenerator* generator, Symbol* symbol)
{
    SamplerState* sampler = static_cast<SamplerState*>(symbol);
    SamplerState::__Resolved* samplerResolved = Symbol::Resolved(sampler);

    Type* samplerTypeSymbol = compiler->GetSymbol<Type>("sampler");
    Type::FullType fullType = Type::FullType{ "sampler" };
    SPIRVResult::Storage scope = ResolveSPIRVVariableStorage(fullType, samplerTypeSymbol, Storage::Uniform);
    SPIRVResult samplerType = GeneratePointerTypeSPIRV(compiler, generator, fullType, samplerTypeSymbol, scope);

    // Generate inline sampler
    if (samplerResolved->isInline)
    {
        static const char* addressingLookup[] =
        {
            "None", // AddressMode::InvalidAddressMode,
            "Repeat", // AddressMode::RepeatAddressMode,
            "RepeatMirrored", // AddressMode::MirrorAddressMode,
            "ClampToEdge", // AddressMode::ClampAddressMode,
            "Clamp", // AddressMode::BorderAddressMode
        };
        static const char* filterLookup[] =
        {
            "", // Filter::InvalidFilter,
            "Nearest", // Filter::PointFilter,
            "Linear"// Filter::LinearFilter
        };
        generator->AddCapability("LiteralSampler");
        uint32_t name = generator->AddSymbol(sampler->name, Format("OpConstantSampler %%%d %s %d %s", samplerType.typeName, addressingLookup[samplerResolved->addressU], samplerResolved->unnormalizedSamplingEnabled ? 0 : 1, filterLookup[samplerResolved->magFilter]), true);
        return SPIRVResult(name, samplerType.typeName, true, true);
    }
    else
    {
        // Generate immutable sampler
        uint32_t name = generator->AddVariableDeclaration(symbol, symbol->name, samplerType.typeName, 0xFFFFFFFF, 0xFFFFFFFF, samplerType.scope, samplerType, true);
        generator->writer->Decorate(SPVArg(name), Decorations::DescriptorSet, samplerResolved->group);
        generator->writer->Decorate(SPVArg(name), Decorations::Binding, samplerResolved->binding);
        generator->AddDecoration(Format("Set(%s)", symbol->name.c_str()), name, Format("DescriptorSet %d", samplerResolved->group));
        generator->AddDecoration(Format("Binding(%s)", symbol->name.c_str()), name, Format("Binding %d", samplerResolved->binding));
        return SPIRVResult(name, samplerType.typeName);
    }
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateEnumSPIRV(const Compiler* compiler, SPIRVGenerator* generator, Symbol* symbol)
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
            
            generator->generatorIntrinsics[fun] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
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
                generator->generatorIntrinsics[fun] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
                {
                    return SPIRVResult(g->AddMappedOp(Format("OpIEqual %%%d %%%d %%%d", returnType, args[0].name, args[1].name)), returnType, true, true);
                };
            }
            else if (fun->name == "operator!=")
            {
                generator->generatorIntrinsics[fun] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
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
GenerateVariableSPIRV(const Compiler* compiler, SPIRVGenerator* generator, Symbol* symbol, bool isShaderArgument, bool isGlobal)
{
    Variable* var = static_cast<Variable*>(symbol);
    Variable::__Resolved* varResolved = static_cast<Variable::__Resolved*>(var->resolved);
    SPIRVResult::Storage storage = ResolveSPIRVVariableStorage(varResolved->type, varResolved->typeSymbol, varResolved->storage, varResolved->usageBits);
    
    SPIRVResult typeName = GenerateTypeSPIRV(compiler, generator, varResolved->type, varResolved->typeSymbol, storage, storage == SPIRVResult::Storage::Uniform || storage == SPIRVResult::Storage::StorageBuffer);
    //std::string type = varResolved->type.name;
    ConstantString scope = SPIRVResult::ScopeToString(typeName.scope);

    std::vector<uint32_t> parentTypes = { typeName.typeName };
    uint32_t name = 0xFFFFFFFF;

    SPIRVResult initializer = SPIRVResult::Invalid();

    Expression* initializerExpression = var->valueExpression;

    // override initializer if the variable is a gplIs variable
    if (varResolved->builtin && var->name.starts_with("gplIs"))
    {
        initializerExpression = &generator->shaderValueExpressions[generator->shaderStage];
    }
    else if (initializerExpression != nullptr)
    {
        // Setup initializer
        generator->linkDefineEvaluation = varResolved->storage == Storage::LinkDefined;
        generator->linkDefineSlot = varResolved->binding;

        initializer = GenerateExpressionSPIRV(compiler, generator, initializerExpression);

        // If initializer is a literal, make sure to load it
        if (initializer.isLiteral)
            initializer = LoadValueSPIRV(compiler, generator, initializer);

        if (varResolved->valueConversionFunction != nullptr)
        {
            auto it = generator->generatorIntrinsics.find(varResolved->valueConversionFunction);
            if (it == generator->generatorIntrinsics.end())
            {
                it = SPIRVGenerator::IntrinsicMap.find(varResolved->valueConversionFunction);
                assert(it != SPIRVGenerator::IntrinsicMap.end());
            }
            initializer = it->second(compiler, generator, typeName.typeName, { initializer });
        }

        generator->linkDefineEvaluation = false;
        generator->linkDefineSlot = UINT32_MAX;
    }

    if (varResolved->storage != Storage::LinkDefined)
    {
        uint32_t typePtrName = typeName.typeName;
        
        // If anything but void, then the type has to be a pointer
        if (!varResolved->type.IsPointer())
        {
            TStr ptrType = TStr("ptr_", varResolved->type.ToString());
            typePtrName = generator->AddSymbol(std::move(TStr::Compact(ptrType, "_", scope)), std::move(TStr::Separated("OpTypePointer", scope, SPVArg(typeName.typeName))), typeName, true);
            typeName.parentTypes.push_back(typeName.typeName);
            typeName.typeName = typePtrName;
        }
        
        if (initializer != SPIRVResult::Invalid() && initializer.isConst)
            name = generator->AddVariableDeclaration(symbol, varResolved->name, typeName.typeName, initializer.name, 0xFFFFFFFF, typeName.scope, typeName, isGlobal);
        else
            name = generator->AddVariableDeclaration(symbol, varResolved->name, typeName.typeName, 0xFFFFFFFF, 0xFFFFFFFF, typeName.scope, typeName, isGlobal);

        if (initializer != SPIRVResult::Invalid() && !initializer.isConst)
        {
            SPIRVResult loaded = LoadValueSPIRV(compiler, generator, initializer);
            generator->writer->Instruction(OpStore, SPVWriter::Section::LocalFunction, SPVArg(name), loaded);
            generator->AddOp(Format("OpStore %%%d %%%d", name, loaded.name));
        }

        if (storage == SPIRVResult::Storage::StorageBuffer || storage == SPIRVResult::Storage::Uniform || storage == SPIRVResult::Storage::PushConstant || storage == SPIRVResult::Storage::Sampler)
        {
            uint32_t structSymbol = generator->GetSymbol(varResolved->typeSymbol->name).value;
            if (typeName.scope != SPIRVResult::Storage::Sampler && varResolved->type.IsPointer())
            {
                generator->writer->Decorate(SPVArg(typeName.parentTypes.back()), Decorations::Block);
                generator->AddDecoration(Format("Block(%s)", varResolved->typeSymbol->name.c_str()), typeName.parentTypes.back(), "Block");
            }
            generator->writer->Decorate(SPVArg(name), Decorations::DescriptorSet, varResolved->group);
            generator->writer->Decorate(SPVArg(name), Decorations::Binding, varResolved->binding);
            generator->AddDecoration(Format("Set(%s)", varResolved->name.c_str()), name, Format("DescriptorSet %d", varResolved->group));
            generator->AddDecoration(Format("Binding(%s)", varResolved->name.c_str()), name, Format("Binding %d", varResolved->binding));
            generator->interfaceVariables.insert(name);
        }
        else if (storage == SPIRVResult::Storage::Image || storage == SPIRVResult::Storage::MutableImage)
        {
            generator->writer->Decorate(SPVArg(name), Decorations::DescriptorSet, varResolved->group);
            generator->writer->Decorate(SPVArg(name), Decorations::Binding, varResolved->binding);
            generator->AddDecoration(Format("Set(%s)", varResolved->name.c_str()), name, Format("DescriptorSet %d", varResolved->group));
            generator->AddDecoration(Format("Binding(%s)", varResolved->name.c_str()), name, Format("Binding %d", varResolved->binding));
            generator->interfaceVariables.insert(name);
        }
        else if (storage == SPIRVResult::Storage::Input || storage == SPIRVResult::Storage::Output)
        {
            uint8_t binding = typeName.scope == SPIRVResult::Storage::Input ? varResolved->inBinding : varResolved->outBinding;
            generator->writer->Decorate(SPVArg(name), Decorations::Location, binding);
            generator->writer->Decorate(SPVArg(name), Decorations::Component, 0);
            generator->AddDecoration(Format("Location(%s)", varResolved->name.c_str()), name, Format("Location %d", binding));
            generator->AddDecoration(Format("Component(%s)", varResolved->name.c_str()), name, Format("Component 0"));
            if (varResolved->parameterBits.flags.isNoInterpolate)
            {
                generator->writer->Decorate(SPVArg(name), Decorations::Flat);
                generator->AddDecoration(Format("NoInterpolate(%s)", varResolved->name.c_str()), name, "Flat");
            }
            if (varResolved->parameterBits.flags.isNoPerspective)
            {
                generator->writer->Decorate(SPVArg(name), Decorations::NoPerspective);
                generator->AddDecoration(Format("NoPerspective(%s)", varResolved->name.c_str()), name, "NoPerspective");
            }
            if (varResolved->parameterBits.flags.isPatch)
            {
                generator->writer->Decorate(SPVArg(name), Decorations::Patch);
                generator->AddDecoration(Format("Patch(%s)", varResolved->name.c_str()), name, "Patch");
            }
            if (varResolved->parameterBits.flags.isCentroid)
            {
                generator->writer->Decorate(SPVArg(name), Decorations::Centroid);
                generator->AddDecoration(Format("Centroid(%s)", varResolved->name.c_str()), name, "Centroid");
            }
            generator->interfaceVariables.insert(name);
        }    
        
        auto ret = SPIRVResult(name, typeName.typeName, false, false, typeName.scope, parentTypes);\
        ret.isStructPadded = typeName.isStructPadded;
        return ret;
    }
    else
    {
        // If it's a link defined variable, the constant generated by the initializer is sufficient
        generator->AddMapping(varResolved->name, initializer.name);
        generator->writer->Decorate(SPVArg(initializer.name), Decorations::SpecId, varResolved->binding);

        generator->AddDecoration(Format("SpecId(%d)", varResolved->binding), initializer.name, Format("SpecId %d", varResolved->binding));
        auto res = SPIRVResult(initializer.name, typeName.typeName);
        res.isValue = true;
        res.isConst = true;
        res.isSpecialization = true;
        res.scope = typeName.scope;
        res.isStructPadded = typeName.isStructPadded;
        res.parentTypes = parentTypes;
        return res;
    }
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateCallExpressionSPIRV(const Compiler* compiler, SPIRVGenerator* generator, Expression* expr)
{
    CallExpression* callExpression = static_cast<CallExpression*>(expr);
    CallExpression::__Resolved* resolvedCall = Symbol::Resolved(callExpression);

    Function::__Resolved* resolvedFunction = Symbol::Resolved(resolvedCall->function);
    SPIRVResult returnTypeName = GenerateTypeSPIRV(compiler, generator, resolvedCall->function->returnType, resolvedFunction->returnTypeSymbol);

    uint32_t funName;
    const SymbolAssignment& sym = generator->GetSymbol(resolvedFunction->name);
    if (sym.value != 0xFFFFFFFF)
    {
        funName = sym.value;


        // If the conversions list is non empty, it means we have to convert every argument
        bool argumentsNeedsConversion = !resolvedCall->conversions.empty();
        
        // Create arg list from argument expressions
        std::string argList = "";

        uint8_t argListIt = 0;
        SPVArg* argListArray = AllocStack<SPVArg>(callExpression->args.size());
        for (size_t i = 0; i < callExpression->args.size(); i++)
        {
            // If the function calls for a literal argument, the generator needs to extract the value as a literal result
            if (resolvedCall->function->parameters[i]->type.literal)
                generator->literalExtract = true;

            SPIRVResult arg = GenerateExpressionSPIRV(compiler, generator, callExpression->args[i]);
            if (!resolvedCall->argumentTypes[i].IsPointer())
                arg = LoadValueSPIRV(compiler,generator, arg);
            

            generator->literalExtract = false;

            if (argumentsNeedsConversion)
            {
                Function* conversion = resolvedCall->conversions[i];
                if (conversion != nullptr)
                {
                    Function::__Resolved* resConversion = Symbol::Resolved(conversion);
                    uint32_t convertedReturn = GeneratePODTypeSPIRV(compiler, generator, resConversion->returnTypeSymbol->baseType, resConversion->returnTypeSymbol->columnSize, resConversion->returnTypeSymbol->rowSize);
                    auto it = generator->generatorIntrinsics.find(conversion);
                    if (it == generator->generatorIntrinsics.end())
                    {
                        it = SPIRVGenerator::IntrinsicMap.find(conversion);
                        assert(it != SPIRVGenerator::IntrinsicMap.end());
                    }

                    arg = it->second(compiler, generator, convertedReturn, { arg });
                }
            }

            argListArray[argListIt++] = SPVArg(arg.name);
            argList.append(Format("%%%d ", arg.name));
        }

        // Then call the function
        generator->writer->MappedInstruction(OpFunctionCall, SPVWriter::Section::LocalFunction, returnTypeName.typeName, SPVArg(funName), SPVArgList{ .vals = argListArray, .num = argListIt });
        return SPIRVResult(generator->AddMappedOp(Format("OpFunctionCall %%%d %%%d %s", returnTypeName.typeName, funName, argList.c_str()), resolvedFunction->name), returnTypeName.typeName, true);
    }
    else
    {
        // If there is no symbol matching this function, then we assume it's an intrinsic function
        std::vector<SPIRVResult> args;

        // Create arg list from argument expressions
        for (size_t i = 0; i < callExpression->args.size(); i++)
        {
            Variable* var = resolvedCall->function->parameters[i];
            Variable::__Resolved* varResolved = Symbol::Resolved(var);

            // If an argument is a literal, evalute it directly
            if (!varResolved->type.literal)
            {
                if (resolvedCall->argumentTypes[i].literal)
                {
                    ValueUnion val;
                    callExpression->args[i]->EvalValue(val);
                    val.Convert(varResolved->typeSymbol->baseType);
                    val.Expand(varResolved->typeSymbol->columnSize, varResolved->typeSymbol->rowSize);
                    SPIRVResult literalResults[16] = { };
                    uint32_t resultsIterator = 0;
                    SPIRVResult typeName = GenerateTypeSPIRV(compiler, generator, varResolved->type, varResolved->typeSymbol);

    #define X(Type, type, ty)\
        for (size_t i = 0; i < val.columnSize; i++)\
        {\
            for (size_t j = 0; j < val.rowSize; j++)\
            {\
                literalResults[resultsIterator] = GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::Type(val.type[resultsIterator]));\
                resultsIterator++;\
            }\
        }\
        if (resultsIterator > 1)\
            args.push_back(GenerateCompositeSPIRV(compiler, generator, typeName.typeName, std::vector<SPIRVResult>(literalResults, literalResults + resultsIterator)));\
        else\
            args.push_back(literalResults[0]);
                                    
                    switch(varResolved->typeSymbol->baseType)
                    {
                        VALUE_UNION_SWITCH()
                    }
#undef  X
                }
                else
                {
                    // If the value can't be evaluated as a compile time expression, generate an expression 
                    SPIRVResult arg = GenerateExpressionSPIRV(compiler, generator, callExpression->args[i]);
                    args.push_back(arg);
                    
                    Function* converter = resolvedCall->conversions[i];
                    if (converter != nullptr)
                    {
                        Function::__Resolved* converterResolved = Symbol::Resolved(converter);
                        auto it = generator->generatorIntrinsics.find(converter);
                        if (it == generator->generatorIntrinsics.end())
                        {
                            it = SPIRVGenerator::IntrinsicMap.find(converter);
                            assert(it != SPIRVGenerator::IntrinsicMap.end());
                        }
                        SPIRVResult returnTypeName = GenerateTypeSPIRV(compiler, generator, converter->returnType, converterResolved->returnTypeSymbol);

                        std::vector<SPIRVResult> tempArgs{ args[i] };

                        // After conversion the type is changed and the SSA is the value of the conversion being done
                        args[i] = it->second(compiler, generator, returnTypeName.typeName, tempArgs);
                    }
                }
            }
            else
            {
                ValueUnion val;
                callExpression->args[i]->EvalValue(val);
                val.Convert(varResolved->typeSymbol->baseType);
#define X(Type, type, ty)\
        args.push_back(SPIRVResult(val.type[0]));

                switch(varResolved->typeSymbol->baseType)
                {
                    VALUE_UNION_SWITCH()
                }
#undef X
            }
        }

        // Function is not declared by user code, so must be intrinsic
        auto it = generator->generatorIntrinsics.find(resolvedCall->function);
        if (it == generator->generatorIntrinsics.end())
        {
            it = SPIRVGenerator::IntrinsicMap.find(resolvedCall->function);
            assert(it != SPIRVGenerator::IntrinsicMap.end());
        }

        // Lastly, run the intrinsic generation with the converted parameters
        return it->second(compiler, generator, returnTypeName.typeName, args);
    }
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateArrayIndexExpressionSPIRV(const Compiler* compiler, SPIRVGenerator* generator, Expression* expr)
{
    ArrayIndexExpression* arrayIndexExpression = static_cast<ArrayIndexExpression*>(expr);
    ArrayIndexExpression::__Resolved* arrayIndexExpressionResolved = Symbol::Resolved(arrayIndexExpression);
    SPIRVResult leftSymbol = GenerateExpressionSPIRV(compiler, generator, arrayIndexExpression->left);

    Type::FullType leftType, rightType;
    arrayIndexExpression->left->EvalType(leftType);
    arrayIndexExpression->right->EvalType(rightType);

    SPIRVResult returnType = GeneratePointerTypeSPIRV(compiler, generator, arrayIndexExpressionResolved->returnFullType, arrayIndexExpressionResolved->returnType, leftSymbol.scope);

    // Temporarily remove the access chain since the index expression doesn't need it
    auto accessChain = generator->accessChain;
    generator->accessChain.clear();
    SPIRVResult index = GenerateExpressionSPIRV(compiler, generator, arrayIndexExpression->right);
    generator->accessChain = accessChain;

    SPIRVResult indexConstant = index;
    if (!indexConstant.isValue)
        indexConstant = LoadValueSPIRV(compiler, generator, indexConstant);
    // Evaluate the index which has to be a literal or constant value that evaluates at compile time
    if (leftType.modifiers.empty())
    {
        Type* type = static_cast<Type*>(compiler->GetSymbol(leftType.name));

        if (type->IsVector() || type->IsMatrix())
        {
            auto it = type->lookup.find(Format("operator[](%s)", rightType.name.c_str()));
            Function* func = static_cast<Function*>((*it).second);

            /// Get intrinsic
            auto intrinsicIt = generator->generatorIntrinsics.find(func);
            if (intrinsicIt == generator->generatorIntrinsics.end())
            {
                intrinsicIt = SPIRVGenerator::IntrinsicMap.find(func);
                assert(intrinsicIt != SPIRVGenerator::IntrinsicMap.end());
            }

            /// Generate array access
            SPIRVResult res = intrinsicIt->second(compiler, generator, returnType.typeName, { leftSymbol, indexConstant });
            assert(res.typeName == returnType.typeName);
            assert(res.parentTypes == returnType.parentTypes);
            assert(res.scope == returnType.scope);
            return res;
        }
    }
    else
    {
        SPIRVResult ret = leftSymbol;
        ret.typeName = returnType.typeName;
        ret.parentTypes = returnType.parentTypes;
        
        // If the left side is an unbound array, it lives in a struct, so access chain must first get the first (0) element
        if (ret.isStructPadded)
        {
            SPIRVResult zero = GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::UInt(0));
            ret.AddAccessChainLink({zero, indexConstant});
        }
        else
        {
            ret.AddAccessChainLink({indexConstant});
        }
        return ret;
    }
    return SPIRVResult::Invalid();
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateInitializerExpressionSPIRV(const Compiler* compiler, SPIRVGenerator* generator, Expression* expr)
{
    uint32_t name = 0xFFFFFFFF;
    InitializerExpression* initExpression = static_cast<InitializerExpression*>(expr);
    auto initResolved = Symbol::Resolved(initExpression);
    uint32_t strucType = generator->GetSymbol(initResolved->type->name).value;

    std::vector<SPIRVResult> values;
    bool isConst = true;
    bool isLinkDefined = false;

    uint8_t argListIt = 0;
    SPVArg* argList = AllocStack<SPVArg>(initExpression->values.size());
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
        argList[argListIt++] = SPVArg(value.name);
    }
    
    if (isConst)
    {
        if (isLinkDefined)
        {
            AddSymbol(generator, TStr::Compact("{", initializer, "}"), SPVWriter::Section::Declarations, OpSpecConstantComposite, strucType, SPVArgList{.vals = argList, .num = argListIt});
            name = generator->AddSymbol(Format("{%s}", initializer.c_str()), Format("OpSpecConstantComposite %%%d %s", strucType, initializer.c_str()), true);
        }
        else
        {
            AddSymbol(generator, TStr::Compact("{", initializer, "}"), SPVWriter::Section::Declarations, OpConstantComposite, strucType, SPVArgList{.vals = argList, .num = argListIt});
            name = generator->AddSymbol(Format("{%s}", initializer.c_str()), Format("OpConstantComposite %%%d %s", strucType, initializer.c_str()), true);
        }
    }
    else
    {
        assert(!generator->linkDefineEvaluation);
        AddSymbol(generator, TStr::Compact("{", initializer, "}"), SPVWriter::Section::LocalFunction, OpCompositeConstruct, strucType, SPVArgList{.vals = argList, .num = argListIt});
        name = generator->AddSymbol(Format("{%s}", initializer.c_str()), Format("OpCompositeConstruct %%%d %s", strucType, initializer.c_str()));
    }
    DeallocStack(argListIt, argList);
    return SPIRVResult(name, strucType, true, isConst);
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateArrayInitializerExpressionSPIRV(const Compiler* compiler, SPIRVGenerator* generator, Expression* expr)
{
    auto sym = static_cast<ArrayInitializerExpression*>(expr);
    ArrayInitializerExpression::__Resolved* arrayInitializerExpressionResolved = Symbol::Resolved(sym);

    SPIRVResult type = GenerateTypeSPIRV(compiler, generator, arrayInitializerExpressionResolved->fullType, arrayInitializerExpressionResolved->type);

    uint32_t name = 0xFFFFFFFF;
    std::vector<SPIRVResult> initResults;
    bool isConst = true;
    bool isLinkDefined = false;
    uint8_t argListIt = 0;
    SPVArg* argList = AllocStack<SPVArg>(sym->values.size());
    std::string initializer = "";
    for (auto value : sym->values)
    {
        SPIRVResult res = GenerateExpressionSPIRV(compiler, generator, value);
        initResults.push_back(res);
        if (!res.isValue)
            res = LoadValueSPIRV(compiler, generator, res);
        initializer.append(Format("%%%d ", res.name));
        
        isConst &= res.isConst;
        isLinkDefined |= res.isSpecialization;
        argList[argListIt++] = SPVArg(res.name);
    }

    if (isConst)
    {
        if (isLinkDefined)
        {
            AddSymbol(generator, TStr::Compact("{", initializer, "}"), SPVWriter::Section::Declarations, OpSpecConstantComposite, type.typeName, SPVArgList{.vals = argList, .num = argListIt});
            name = generator->AddSymbol(Format("{%s}", initializer.c_str()), Format("OpSpecConstantComposite %%%d %s", type.typeName, initializer.c_str()), true);
        }
        else
        {
            AddSymbol(generator, TStr::Compact("{", initializer, "}"), SPVWriter::Section::Declarations, OpConstantComposite, type.typeName, SPVArgList{.vals = argList, .num = argListIt});
            name = generator->AddSymbol(Format("{%s}", initializer.c_str()), Format("OpConstantComposite %%%d %s", type.typeName, initializer.c_str()), true);
        }
    }
    else
    {
        assert(!generator->linkDefineEvaluation);
                AddSymbol(generator, TStr::Compact("{", initializer, "}"), SPVWriter::Section::LocalFunction, OpCompositeConstruct, type.typeName, SPVArgList{.vals = argList, .num = argListIt});
        name = generator->AddSymbol(Format("{%s}", initializer.c_str()), Format("OpCompositeConstruct %%%d %s", type.typeName, initializer.c_str()));
    }
    return SPIRVResult(name, type.typeName, true, isConst);
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateBinaryExpressionSPIRV(const Compiler* compiler, SPIRVGenerator* generator, Expression* expr)
{
    BinaryExpression* binaryExpression = static_cast<BinaryExpression*>(expr);
    BinaryExpression::__Resolved* binaryExpressionResolved = Symbol::Resolved(binaryExpression);
    Type::FullType leftType, rightType;
    binaryExpression->left->EvalType(leftType);
    binaryExpression->right->EvalType(rightType);

    Type* lhsType = binaryExpressionResolved->lhsType;
    Type* rhsType = binaryExpressionResolved->rhsType;

    SPIRVResult rightValue = GenerateExpressionSPIRV(compiler, generator, binaryExpression->right);
    SPIRVResult leftValue = GenerateExpressionSPIRV(compiler, generator, binaryExpression->left);
    SPIRVResult returnType = GenerateTypeSPIRV(compiler, generator, binaryExpressionResolved->returnType, binaryExpressionResolved->retType);
    if (binaryExpressionResolved->constValueExpression != nullptr)
        return GenerateExpressionSPIRV(compiler, generator, binaryExpressionResolved->constValueExpression);
    rightValue = LoadValueSPIRV(compiler, generator, rightValue);

    // If there is a conversion function, generate it first
    if (binaryExpressionResolved->rightConversion)
    {
        Function::__Resolved* rightConvResolved = Symbol::Resolved(binaryExpressionResolved->rightConversion);
        SPIRVResult rightConvType = GenerateTypeSPIRV(compiler, generator, binaryExpressionResolved->rightConversion->returnType, rightConvResolved->returnTypeSymbol);
        auto it = generator->generatorIntrinsics.find(binaryExpressionResolved->rightConversion);
        if (it == generator->generatorIntrinsics.end())
        {
            it = SPIRVGenerator::IntrinsicMap.find(binaryExpressionResolved->rightConversion);
            assert(it != SPIRVGenerator::IntrinsicMap.end());
        }
        rightValue = it->second(compiler, generator, rightConvType.typeName, { rightValue });
        rightType = binaryExpressionResolved->rightConversion->returnType;
        rhsType = rightConvResolved->returnTypeSymbol;
    }
    if (binaryExpressionResolved->leftConversion)
    {
        Function::__Resolved* leftConvResolved = Symbol::Resolved(binaryExpressionResolved->leftConversion);
        SPIRVResult leftConvType = GenerateTypeSPIRV(compiler, generator, binaryExpressionResolved->leftConversion->returnType, leftConvResolved->returnTypeSymbol);
        auto it = generator->generatorIntrinsics.find(binaryExpressionResolved->leftConversion);
        if (it == generator->generatorIntrinsics.end())
        {
            it = SPIRVGenerator::IntrinsicMap.find(binaryExpressionResolved->leftConversion);
            assert(it != SPIRVGenerator::IntrinsicMap.end());
        }
        leftValue = it->second(compiler, generator, leftConvType.typeName, { leftValue });
        leftType = binaryExpressionResolved->leftConversion->returnType;
        lhsType = leftConvResolved->returnTypeSymbol;
    }

    SPIRVResult binaryOpResult = rightValue;
    if (binaryExpression->op != '=')
    {
        std::string functionName = Format("operator%s(%s)", FourCCToString(binaryExpression->op).c_str(), rightType.Name().c_str());
        Function* fun = lhsType->GetSymbol<Function>(functionName);
        Function::__Resolved* funResolved = Symbol::Resolved(fun);
        assert(fun != nullptr);

        SPIRVResult retType = GenerateTypeSPIRV(compiler, generator, fun->returnType, funResolved->returnTypeSymbol);

        // Get operator 
        auto it = generator->generatorIntrinsics.find(fun);
        if (it == generator->generatorIntrinsics.end())
        {
            it = SPIRVGenerator::IntrinsicMap.find(fun);
            assert(it != SPIRVGenerator::IntrinsicMap.end());
        }
        binaryOpResult = it->second(compiler, generator, retType.typeName, { leftValue, rightValue });
    }

    if (binaryExpressionResolved->isAssignment)
    {
        assert(!leftValue.isValue);

        // If left value has a swizzle mask, then swizzle the RIGHT hand value before storing
        if (leftValue.swizzleMask != Type::SwizzleMask())
        {
            // Get the unswizzled type and generate a SPIRV type for it.
            // Storing to a swizzle value requires an OpStore of the same size, so we are going to shuffle
            // the vector of the right hand side such that it has the same amount of components and the left hand side
            Type* vectorType = compiler->GetSymbol<Type>(leftType.name);
            SPIRVResult vectorTypeName = GenerateTypeSPIRV(compiler, generator, leftType, vectorType);

            // Setup a binding table
            uint32_t slots[4] = { 0, 1, 2, 3 };
            uint8_t counter = 0;

            // Update the slot mask
            if (leftValue.swizzleMask.bits.x != Type::SwizzleMask::Invalid)
                slots[leftValue.swizzleMask.bits.x] = vectorType->columnSize + counter++;
            if (leftValue.swizzleMask.bits.y != Type::SwizzleMask::Invalid)
                slots[leftValue.swizzleMask.bits.y] = vectorType->columnSize + counter++;
            if (leftValue.swizzleMask.bits.z != Type::SwizzleMask::Invalid)
                slots[leftValue.swizzleMask.bits.z] = vectorType->columnSize + counter++;
            if (leftValue.swizzleMask.bits.w != Type::SwizzleMask::Invalid)
                slots[leftValue.swizzleMask.bits.w] = vectorType->columnSize + counter++;

            std::string swizzleMask = "";
            for (uint32_t i = 0; i < vectorType->columnSize; i++)
            {
                swizzleMask.append(Format("%d ", slots[i]));
            }
            
            leftValue.swizzleMask = Type::SwizzleMask();
            SPIRVResult leftLoaded = LoadValueSPIRV(compiler, generator, leftValue, true);

            // If right hand side is not a vector, make it into a splatted composite
            if (!rhsType->IsVector())
            {
                // If right hand side isn't even a vector, make it one
                rightValue = GenerateSplatCompositeSPIRV(compiler, generator, leftLoaded.typeName, vectorType->columnSize, rightValue);
            }
            if (rhsType->columnSize != vectorType->columnSize)
            {
                // Shuffle the values into a single vector
                generator->writer->MappedInstruction(OpVectorShuffle, SPVWriter::Section::LocalFunction, vectorTypeName.typeName, leftLoaded, rightValue, SPVLiteralList{ .vals = slots, .num = counter });
                rightValue.name = generator->AddMappedOp(Format("OpVectorShuffle %%%d %%%d %%%d %s", vectorTypeName.typeName, leftLoaded.name, rightValue.name, swizzleMask.c_str()));
                rightValue.typeName = vectorTypeName.typeName;
            }
        }
        StoreValueSPIRV(compiler, generator, leftValue, rightValue);
        return leftValue;
    }
    else
    {
        return binaryOpResult;
    }
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateAccessExpressionSPIRV(const Compiler* compiler, SPIRVGenerator* generator, Expression* expr)
{
    AccessExpression* accessExpression = static_cast<AccessExpression*>(expr);
    AccessExpression::__Resolved* accessExpressionResolved = Symbol::Resolved(accessExpression);
    SPIRVResult lhs = GenerateExpressionSPIRV(compiler, generator, accessExpression->left);
    Type::SwizzleMask swizzle = accessExpressionResolved->swizzleMask;
    if (swizzle.mask != 0x0)
    {
        assert(accessExpressionResolved->lhsType->IsVector());

        // Get unswizzled type
        Type::FullType unswizzledReturnType = accessExpressionResolved->returnType;
        unswizzledReturnType.swizzleName = "";
        Type* unswizzledType = compiler->GetType(unswizzledReturnType);
        Type* swizzledType = compiler->GetType(accessExpressionResolved->returnType);

        // And a type for the value returned, which is a pointer to the full vector type, but with a swizzle type appended
        SPIRVResult swizzleType = GenerateTypeSPIRV(compiler, generator, accessExpressionResolved->returnType, swizzledType); 
        SPIRVResult ret = lhs;
        ret.swizzleMask = swizzle;
        ret.swizzleType = swizzleType.typeName;
        return ret;
    }
    else
    {
        // Otherwise, find offset of member
        Type::FullType lhsType;
        accessExpression->left->EvalType(lhsType);
        Type* lhsSymbol = compiler->GetType(lhsType);

        if (lhsType.modifiers.size() > 0 && lhsType.modifiers.front() == Type::FullType::Modifier::Array)
        {
            assert(accessExpressionResolved->rightSymbol == "length");
            uint32_t size = 0;
            if (lhsType.modifierValues.front() != nullptr)
            {
                ValueUnion val;
                lhsType.modifierValues.front()->EvalValue(val);
                val.Store(size);
            }
            assert(size > 0);

            return GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::UInt(size));
        }
        else if (lhsSymbol->symbolType == Symbol::StructureType || lhsSymbol->symbolType == Symbol::EnumerationType)
        {
            for (size_t i = 0; i < lhsSymbol->symbols.size(); i++)
            {
                Symbol* sym = lhsSymbol->symbols[i];
                if (sym->name == accessExpressionResolved->rightSymbol)
                {
                    if (lhsSymbol->symbolType == Symbol::StructureType)
                    {
                        // Get variable in struct this member corresponds to
                        Variable* mem = lhsSymbol->GetSymbol<Variable>(accessExpressionResolved->rightSymbol);
                        Variable::__Resolved* memResolved = Symbol::Resolved(mem);

                        // Create an index for the offset in the struct, and a type for the type of that member
                        SPIRVResult indexName = GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::UInt(i));
                        SPIRVResult typeName = GeneratePointerTypeSPIRV(compiler, generator, mem->type, memResolved->typeSymbol, lhs.scope);
                        
                        // Since this is na access chain, we need to be returning a pointer to the type
                        generator->PushAccessChain(lhsSymbol, lhs.scope);
                        SPIRVResult rhs = GenerateExpressionSPIRV(compiler, generator, accessExpression->right);
                        generator->PopAccessChain();
                        rhs.scope = lhs.scope;
                        rhs.name = lhs.name;
                        rhs.accessChain.insert(rhs.accessChain.begin(), lhs.accessChain.begin(), lhs.accessChain.end());
                        return rhs;
                    }
                    else if (lhsSymbol->symbolType == Symbol::EnumerationType)
                    {
                        EnumExpression* enumExp = static_cast<EnumExpression*>(sym);
                        uint32_t val;
                        ValueUnion value;
                        enumExp->EvalValue(value);
                        value.Store(val);
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
GenerateCommaExpressionSPIRV(const Compiler* compiler, SPIRVGenerator* generator, Expression* expr)
{
    CommaExpression* commaExpression = static_cast<CommaExpression*>(expr);

    GenerateExpressionSPIRV(compiler, generator, commaExpression->left);
    return GenerateExpressionSPIRV(compiler, generator, commaExpression->right);
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateTernaryExpressionSPIRV(const Compiler* compiler, SPIRVGenerator* generator, Expression* expr)
{
    TernaryExpression* ternaryExpression = static_cast<TernaryExpression*>(expr);
    TernaryExpression::__Resolved* ternaryExpressionResolved = Symbol::Resolved(ternaryExpression);
    
    ValueUnion val;
    if (ternaryExpression->lhs->EvalValue(val))
    {
        if (val.b[0])
            return GenerateExpressionSPIRV(compiler, generator, ternaryExpression->ifExpression);
        else
            return GenerateExpressionSPIRV(compiler, generator, ternaryExpression->elseExpression);
    }

    SPIRVResult lhsResult = GenerateExpressionSPIRV(compiler, generator, ternaryExpression->lhs);
    SPIRVResult ifResult = GenerateExpressionSPIRV(compiler, generator, ternaryExpression->ifExpression);
    SPIRVResult elseResult = GenerateExpressionSPIRV(compiler, generator, ternaryExpression->elseExpression);
    
    SPIRVResult loadedCondition = LoadValueSPIRV(compiler, generator, lhsResult);
    SPIRVResult loadedLeft = LoadValueSPIRV(compiler, generator, ifResult);
    SPIRVResult loadedRight = LoadValueSPIRV(compiler, generator, elseResult);
    generator->writer->MappedInstruction(OpSelect, SPVWriter::Section::LocalFunction, loadedLeft.typeName, loadedCondition, loadedLeft, loadedRight);
    uint32_t ret = generator->AddMappedOp(Format("OpSelect %%%d %%%d %%%d %%%d", loadedLeft.typeName, loadedCondition.name, loadedLeft.name, loadedRight.name), ternaryExpressionResolved->text);

    SPIRVResult res = ifResult;
    res.typeName = ret;
    return res;
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateUnaryExpressionSPIRV(const Compiler* compiler, SPIRVGenerator* generator, Expression* expr)
{
    UnaryExpression* unaryExpression = static_cast<UnaryExpression*>(expr);
    UnaryExpression::__Resolved* unaryExpressionResolved = Symbol::Resolved(unaryExpression);

    SPIRVResult rhs = GenerateExpressionSPIRV(compiler, generator, unaryExpression->expr);

    // TODO: Add support for looking up SPVOps as well
    static std::unordered_map<std::string, std::tuple<const char, bool, uint32_t>> scalarTable =
    {
        { "f32", { 'F', true, 1 } }
        , { "f32x2", { 'F', true, 2 } }
        , { "f32x3", { 'F', true, 3} }
        , { "f32x4", { 'F', true, 4 } }
        , { "i32", { 'S', true, 1 } }
        , { "i32x2", { 'S', true, 2 } }
        , { "i32x3", { 'S', true, 3 } }
        , { "i32x4", { 'S', true, 4 } }
        , { "u32", { 'U', false, 1 } }
        , { "u32x2", { 'U', false, 2 } }
        , { "u32x3", { 'U', false, 3 } }
        , { "u32x4", { 'U', false, 4 } }
        , { "b8", { 'B', false, 1 } }
        , { "b8x2", { 'B', false, 2 } }
        , { "b8x3", { 'B', false, 3 } }
        , { "b8x4", { 'B', false, 4 } }
    };

    auto value = scalarTable.find(unaryExpressionResolved->fullType.name);
    switch (unaryExpression->op)
    {
        case '++':
        {
            assert(value != scalarTable.end());
            char op = std::get<0>(value->second);
            bool isSigned = std::get<1>(value->second);
            uint32_t vectorSize = std::get<2>(value->second);
            SPIRVResult constOne = SPIRVResult::Invalid();
            if (isSigned)
            {
                if (op == 'U' || op == 'S')
                {
                    constOne = GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::Int(1));
                    op = 'I';
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
            uint32_t res = generator->AddMappedOp(Format("Op%cAdd %%%d %%%d %%%d", op, loaded.typeName, loaded.name, constOne.name), unaryExpressionResolved->text);
            if (unaryExpression->isPrefix)
                return SPIRVResult(res, rhs.typeName, true);
            else
            {
                generator->AddOp(Format("OpStore %%%d %%%d", rhs.name, res));   
                return SPIRVResult(loaded.name, rhs.typeName, true);
            }
        }
        case '--':
        {
            assert(value != scalarTable.end());
            char op = std::get<0>(value->second);
            bool isSigned = std::get<1>(value->second);
            uint32_t vectorSize = std::get<2>(value->second);
            SPIRVResult constOne = SPIRVResult::Invalid();
            if (isSigned)
            {
                if (op == 'U' || op == 'S')
                {
                    constOne = GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::Int(1));
                    op = 'I';
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
            uint32_t res = generator->AddMappedOp(Format("Op%cSub %%%d %%%d %%%d", op, loaded.typeName, loaded.name, constOne.name), unaryExpressionResolved->text);
            if (unaryExpression->isPrefix)
                return SPIRVResult(res, rhs.typeName, true);
            else
            {
                generator->writer->Instruction(OpStore, SPVWriter::Section::LocalFunction, rhs, SPVArg(res));
                generator->AddOp(Format("OpStore %%%d %%%d", rhs.name, res));
                return SPIRVResult(loaded.name, rhs.typeName, true);
            }
        }
        case '*':
        {
            rhs.derefs++;
            return rhs;
            //SPIRVResult typeName = GenerateTypeSPIRV(compiler, generator, unaryExpressionResolved->fullType, unaryExpressionResolved->type);
            //uint32_t deref = generator->AddMappedOp(Format("OpLoad %%%d %%%d", typeName.typeName, rhs.name), unaryExpressionResolved->text);
            //return SPIRVResult(deref, typeName.typeName, true);
            //break;
        }
        case '-':
        {
            assert(value != scalarTable.end());
            char op = std::get<0>(value->second);
            bool isSigned = std::get<1>(value->second);
            uint32_t vectorSize = std::get<2>(value->second);
            if (rhs.isLiteral)
            {
                switch (op)
                {
                    case 'F':
                        return GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::Float(-rhs.literalValue.f));            
                    case 'U':
                    case 'S':
                        return GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::Int(-rhs.literalValue.i));
                    default:
                        assert(false);
                }
            }
            else
            {
                SPIRVResult loaded = LoadValueSPIRV(compiler, generator, rhs);
                uint32_t res = generator->AddMappedOp(Format("Op%cNegate %%%d %%%d", op, loaded.typeName, loaded.name), unaryExpressionResolved->text);
                return SPIRVResult(res, rhs.typeName, true);
            }
        }
        case '+':
        {
            SPIRVResult loaded = LoadValueSPIRV(compiler, generator, rhs);
            return SPIRVResult(loaded.name, rhs.typeName, true);
        }
        case '!':
        case '~':
        {
            assert(value != scalarTable.end());
            const char op = std::get<0>(value->second);
            bool isSigned = std::get<1>(value->second);
            uint32_t vectorSize = std::get<2>(value->second);
            
            if (rhs.isLiteral)
            {
                return GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::UInt(!rhs.literalValue.ui));                
            }
            else
            {
                SPIRVResult loaded = LoadValueSPIRV(compiler, generator, rhs);
                // If bool, convert to u32 first and then perform ! or ~
                if (op == 'B')
                    loaded = GenerateConversionSPIRV(compiler, generator, ConversionTable::BoolToUInt, vectorSize, loaded);
                generator->writer->MappedInstruction(OpNot, SPVWriter::Section::LocalFunction, loaded.typeName, loaded);
                uint32_t res = generator->AddMappedOp(Format("OpNot %%%d %%%d", loaded.typeName, loaded.name));

                // Again, if bool, convert back from uint to Bool
                if (op == 'B')
                    loaded = GenerateConversionSPIRV(compiler, generator, ConversionTable::UIntToBool, vectorSize, SPIRVResult(res, loaded.typeName, true));
                return loaded;    
            }
        }
        default:
            assert(false);
    }
    return SPIRVResult::Invalid();
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateExpressionSPIRV(const Compiler* compiler, SPIRVGenerator* generator, Expression* expr)
{
    ValueUnion val;
    expr->EvalValue(val);
    switch (expr->symbolType)
    {
        case Symbol::FloatExpressionType:
        {
            FloatExpression* floatExpr = static_cast<FloatExpression*>(expr);
            if (generator->literalExtract)
                return SPIRVResult(val.f[0]);
            else
                return GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::Float(val.f[0]));
        }
        case Symbol::FloatVecExpressionType:
        {
            FloatVecExpression* floatVecExpr = static_cast<FloatVecExpression*>(expr);
            FloatVecExpression::__Resolved* floatVecExprRes = Symbol::Resolved(floatVecExpr);
            assert(!generator->literalExtract);
            std::vector<SPIRVResult> results;
            for (uint32_t i = 0; i < floatVecExpr->values.size(); i++)
            {
                results.push_back(GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::Float(floatVecExpr->values[i])));
            }
            SPIRVResult ty = GenerateTypeSPIRV(compiler, generator, floatVecExprRes->fullType, floatVecExprRes->type);
            return GenerateCompositeSPIRV(compiler, generator, ty.typeName, results);
        }
        case Symbol::IntExpressionType:
        {
            IntExpression* intExpr = static_cast<IntExpression*>(expr);
            if (generator->literalExtract)
                return SPIRVResult(val.i[0]);
            else
                return GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::Int(val.i[0]));
        }
        case Symbol::IntVecExpressionType:
        {
            IntVecExpression* intVecExpr = static_cast<IntVecExpression*>(expr);
            IntVecExpression::__Resolved* intVecExprRes = Symbol::Resolved(intVecExpr);
            assert(!generator->literalExtract);
            std::vector<SPIRVResult> results;
            for (uint32_t i = 0; i < intVecExpr->values.size(); i++)
            {
                results.push_back(GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::Int(intVecExpr->values[i])));
            }
            SPIRVResult ty = GenerateTypeSPIRV(compiler, generator, intVecExprRes->fullType, intVecExprRes->type);
            return GenerateCompositeSPIRV(compiler, generator, ty.typeName, results);
        }
        case Symbol::UIntExpressionType:
        {
            UIntExpression* uintExpr = static_cast<UIntExpression*>(expr);
            if (generator->literalExtract)
                return SPIRVResult(val.ui[0]);
            else
                return GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::UInt(val.ui[0]));
        }
        case Symbol::UIntVecExpressionType:
        {
            UIntVecExpression* uintVecExpr = static_cast<UIntVecExpression*>(expr);
            UIntVecExpression::__Resolved* uintVecExprRes = Symbol::Resolved(uintVecExpr);
            assert(!generator->literalExtract);
            std::vector<SPIRVResult> results;
            for (uint32_t i = 0; i < uintVecExpr->values.size(); i++)
            {
                results.push_back(GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::UInt(uintVecExpr->values[i])));
            }
            SPIRVResult ty = GenerateTypeSPIRV(compiler, generator, uintVecExprRes->fullType, uintVecExprRes->type);
            return GenerateCompositeSPIRV(compiler, generator, ty.typeName, results);
        }
        case Symbol::BoolExpressionType:
        {
            BoolExpression* boolExpr = static_cast<BoolExpression*>(expr);
            if (generator->literalExtract)
                return SPIRVResult(val.b[0]);
            else
                return GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::Bool(val.b[0]));
        }
        case Symbol::BoolVecExpressionType:
        {
            BoolVecExpression* boolVecExpr = static_cast<BoolVecExpression*>(expr);
            BoolVecExpression::__Resolved* boolVecExprRes = Symbol::Resolved(boolVecExpr);
            assert(!generator->literalExtract);
            std::vector<SPIRVResult> results;
            for (uint32_t i = 0; i < boolVecExpr->values.size(); i++)
            {
                results.push_back(GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::Bool(boolVecExpr->values[i])));
            }
            SPIRVResult ty = GenerateTypeSPIRV(compiler, generator, boolVecExprRes->fullType, boolVecExprRes->type);
            return GenerateCompositeSPIRV(compiler, generator, ty.typeName, results);
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
                    SPIRVResult::Storage storage = ResolveSPIRVVariableStorage(var->type, varResolved->typeSymbol, varResolved->storage, varResolved->usageBits);
                    const SymbolAssignment& sym = generator->GetSymbol(symbolExpression->symbol);
                    type = sym.type;
                    if (generator->accessChain.empty())
                    {
                        if (generator->literalExtract)
                        {
                            if (sym.sym->symbolType == Symbol::SymbolType::VariableType)
                            {
                                Variable* var = static_cast<Variable*>(sym.sym);
                                Variable::__Resolved* varRes = Symbol::Resolved(var);
                                if (varRes->type.literal && varRes->typeSymbol->columnSize == 1 && varRes->typeSymbol->rowSize == 1)
                                {
                                    res = SPIRVResult(0xFFFFFFFF, type.typeName);
                                    res.isValue = true;
                                    res.isConst = true;
                                    res.literalValue = SPIRVResult::LiteralValue();
                                }
                            }
                            else
                            {
                                assert(false);
                            }
                        }
                        else
                        {
                            res = type;
                            res.name = sym.value;
                        }
                    }
                    else
                    {
                        auto [ty, accessStorage] = generator->accessChain.back();
                        for (size_t i = 0; i < ty->symbols.size(); i++)
                        {
                            if (ty->symbols[i]->name == symbolExpression->symbol)
                            {
                                Variable* var = static_cast<Variable*>(ty->symbols[i]);
                                Variable::__Resolved* varRes = Symbol::Resolved(var);
                                SPIRVResult index = GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::UInt(i));

                                // If part of an access chain, generate a pointer version of the type
                                res = GeneratePointerTypeSPIRV(compiler, generator, varRes->type, varRes->typeSymbol, accessStorage);
                                res.scope = accessStorage;
                                res.AddAccessChainLink({index});
                                break;
                            }
                        }
                    }
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
        case Symbol::ArrayInitializerExpressionType:
            return GenerateArrayInitializerExpressionSPIRV(compiler, generator, expr);
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
GenerateBreakStatementSPIRV(const Compiler* compiler, SPIRVGenerator* generator, BreakStatement* stat)
{
    if (generator->skipBreakContinue)
        return;
    assert(generator->mergeBlocks[generator->mergeBlockCounter-1].breakLabel != 0xFFFFFFFF);
    generator->writer->Instruction(OpBranch, SPVWriter::Section::LocalFunction, SPVArg(generator->mergeBlocks[generator->mergeBlockCounter - 1].breakLabel));
    generator->AddOp(Format("OpBranch %%%d", generator->mergeBlocks[generator->mergeBlockCounter - 1].breakLabel), false, "break");
    generator->blockOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
void
GenerateContinueStatementSPIRV(const Compiler* compiler, SPIRVGenerator* generator, ContinueStatement* stat)
{
    if (generator->skipBreakContinue)
        return;
    assert(generator->mergeBlocks[generator->mergeBlockCounter - 1].continueLabel != 0xFFFFFFFF);
    generator->writer->Instruction(OpBranch, SPVWriter::Section::LocalFunction, SPVArg(generator->mergeBlocks[generator->mergeBlockCounter - 1].continueLabel));
    generator->AddOp(Format("OpBranch %%%d", generator->mergeBlocks[generator->mergeBlockCounter - 1].continueLabel), false, "continue");
    generator->blockOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
void
GenerateForStatementSPIRV(const Compiler* compiler, SPIRVGenerator* generator, ForStatement* stat)
{
    // Skip for loop generation if condition resolves to false
    ValueUnion val;
    if (stat->condition->EvalValue(val))
    {
        val.Convert(TypeCode::Bool);
        if (!val.b[0])
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
        repeatLabel, endLabel
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
GenerateIfStatementSPIRV(const Compiler* compiler, SPIRVGenerator* generator, IfStatement* stat)
{
    // If B evaluates to a constant value, pick a branch and return early
    ValueUnion val;
    if (stat->condition->EvalValue(val))
    {
        if (val.b[0])
            GenerateStatementSPIRV(compiler, generator, stat->ifStatement);
        else if (stat->elseStatement != nullptr)
            GenerateStatementSPIRV(compiler, generator, stat->elseStatement);
        return;
    }
    
    SPIRVResult lhsResult = GenerateExpressionSPIRV(compiler, generator, stat->condition);
    lhsResult = LoadValueSPIRV(compiler, generator, lhsResult);
    
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
GenerateTerminateStatementSPIRV(const Compiler* compiler, SPIRVGenerator* generator, TerminateStatement* stat)
{
    if (stat->type == TerminateStatement::TerminationType::Return)
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
    else if (stat->type == TerminateStatement::TerminationType::Discard)
    {
        generator->AddOp("OpKill");
        generator->blockOpen = false;
    }
    else if (stat->type == TerminateStatement::TerminationType::RayTerminate)
    {
        generator->AddOp("OpTerminateRayKHR");
        generator->blockOpen = false;
    }
    else if (stat->type == TerminateStatement::TerminationType::RayIgnoreIntersection)
    {
        generator->AddOp("OpIgnoreIntersectionKHR");
        generator->blockOpen = false;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
GenerateReturnStatementSPIRV(const Compiler* compiler, SPIRVGenerator* generator, ReturnStatement* stat)
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
GenerateSwitchStatementSPIRV(const Compiler* compiler, SPIRVGenerator* generator, SwitchStatement* stat)
{
    ValueUnion val;
    if (stat->switchExpression->EvalValue(val))
    {
        generator->skipBreakContinue = true;
        if (val.ui[0] < stat->caseExpressions.size())
            GenerateStatementSPIRV(compiler, generator, stat->caseStatements[val.ui[0]]);
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
        ValueUnion val;
        bool res = stat->caseExpressions[i]->EvalValue(val);
        assert(res);
        caseList += Format("%d %%%d ", val.i[0], caseLabel);
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
GenerateWhileStatementSPIRV(const Compiler* compiler, SPIRVGenerator* generator, WhileStatement* stat)
{
    // Skip generation if statement resolves to false
    ValueUnion val;
    if (stat->condition->EvalValue(val))
    {
        if (!val.b[0])
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
GenerateStatementSPIRV(const Compiler* compiler, SPIRVGenerator* generator, Statement* stat)
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
        case Symbol::TerminateStatementType:
            GenerateTerminateStatementSPIRV(compiler, generator, static_cast<TerminateStatement*>(stat));
            ret = true;
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
SPIRVGenerator::Generate(const Compiler* compiler, const Program* program, const std::vector<Symbol*>& symbols, std::function<void(const std::string&, const std::string&)> writerFunc)
{
    Program::__Resolved* progResolved = static_cast<Program::__Resolved*>(program->resolved);

    struct Cleanup
    {
        Cleanup(SPIRVGenerator* gen) :
            gen(gen) {}

        ~Cleanup()
        {
            gen->header.Clear();
            gen->capability.Clear();
            gen->extension.Clear();
            gen->extImport.Clear();
            gen->decorations.Clear();
            gen->declarations.Clear();
            gen->functional.Clear();
            gen->interfaceVariables.clear();
            gen->extensions.clear();
            gen->decorationMap.clear();
            gen->variableDeclarations.Clear();
            gen->parameterInitializations.Clear();
            
            gen->scopeStack.clear();
            gen->functions.Clear();
            gen->capabilities.clear();
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

    static std::unordered_map<Program::__Resolved::ProgramEntryType, SPVEnum> executionModelEnumMap =
    {
        { Program::__Resolved::ProgramEntryType::VertexShader, ExecutionModels::Vertex }
        , { Program::__Resolved::ProgramEntryType::HullShader, ExecutionModels::TessellationControl }
        , { Program::__Resolved::ProgramEntryType::DomainShader, ExecutionModels::TessellationEvaluation }
        , { Program::__Resolved::ProgramEntryType::GeometryShader, ExecutionModels::Geometry }
        , { Program::__Resolved::ProgramEntryType::PixelShader, ExecutionModels::Fragment }
        , { Program::__Resolved::ProgramEntryType::ComputeShader, ExecutionModels::GLCompute }
        , { Program::__Resolved::ProgramEntryType::TaskShader, ExecutionModels::TaskEXT }
        , { Program::__Resolved::ProgramEntryType::MeshShader, ExecutionModels::MeshEXT }
        , { Program::__Resolved::ProgramEntryType::RayGenerationShader, ExecutionModels::RayGenerationKHR }
        , { Program::__Resolved::ProgramEntryType::RayMissShader, ExecutionModels::MissKHR }
        , { Program::__Resolved::ProgramEntryType::RayClosestHitShader, ExecutionModels::ClosestHitKHR }
        , { Program::__Resolved::ProgramEntryType::RayAnyHitShader, ExecutionModels::AnyHitKHR }
        , { Program::__Resolved::ProgramEntryType::RayIntersectionShader, ExecutionModels::IntersectionKHR }
        , { Program::__Resolved::ProgramEntryType::RayCallableShader, ExecutionModels::CallableKHR }
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

    static std::unordered_map<Program::__Resolved::ProgramEntryType, SPVEnum> extensionEnumMap =
    {
        { Program::__Resolved::ProgramEntryType::VertexShader, Capabilities::Shader }
        , { Program::__Resolved::ProgramEntryType::HullShader, Capabilities::Tessellation }
        , { Program::__Resolved::ProgramEntryType::DomainShader, Capabilities::Tessellation }
        , { Program::__Resolved::ProgramEntryType::GeometryShader, Capabilities::Geometry }
        , { Program::__Resolved::ProgramEntryType::PixelShader, Capabilities::Shader }
        , { Program::__Resolved::ProgramEntryType::ComputeShader, Capabilities::Shader }
        , { Program::__Resolved::ProgramEntryType::TaskShader, Capabilities::MeshShadingEXT }
        , { Program::__Resolved::ProgramEntryType::MeshShader, Capabilities::MeshShadingEXT }
        , { Program::__Resolved::ProgramEntryType::RayGenerationShader, Capabilities::RayTracingKHR }
        , { Program::__Resolved::ProgramEntryType::RayMissShader, Capabilities::RayTracingKHR }
        , { Program::__Resolved::ProgramEntryType::RayClosestHitShader, Capabilities::RayTracingKHR }
        , { Program::__Resolved::ProgramEntryType::RayAnyHitShader, Capabilities::RayTracingKHR }
        , { Program::__Resolved::ProgramEntryType::RayIntersectionShader, Capabilities::RayTracingKHR }
        , { Program::__Resolved::ProgramEntryType::RayCallableShader, Capabilities::RayTracingKHR }
    };

    static std::unordered_map<Function::__Resolved::PartitionMethod, std::string> partitionMap =
    {
        { Function::__Resolved::PartitionMethod::IntegerSteps, "SpacingEqual" }
        , { Function::__Resolved::PartitionMethod::FloatEven, "SpacingFractionalEven" }
        , { Function::__Resolved::PartitionMethod::FloatOdd, "SpacingFractionalOdd" }
    };
    
    static std::unordered_map<Function::__Resolved::PartitionMethod, SPVEnum> partitionEnumMap =
    {
        { Function::__Resolved::PartitionMethod::IntegerSteps, ExecutionModes::SpacingEqual }
        , { Function::__Resolved::PartitionMethod::FloatEven, ExecutionModes::SpacingFractionalEven }
        , { Function::__Resolved::PartitionMethod::FloatOdd, ExecutionModes::SpacingFractionalOdd }
    };

    static std::unordered_map<Function::__Resolved::WindingOrder, std::string> windingOrderMap =
    {
        { Function::__Resolved::WindingOrder::Clockwise, "VertexOrderCw" }
        , { Function::__Resolved::WindingOrder::CounterClockwise, "VertexOrderCcw" }
    };
    
    static std::unordered_map<Function::__Resolved::WindingOrder, SPVEnum> windingOrderEnumMap =
    {
        { Function::__Resolved::WindingOrder::Clockwise, ExecutionModes::VertexOrderCw }
        , { Function::__Resolved::WindingOrder::CounterClockwise, ExecutionModes::VertexOrderCcw }
    };

    static std::unordered_map<Function::__Resolved::PixelOrigin, std::string> pixelOriginMap =
    {
        { Function::__Resolved::PixelOrigin::Center, "VertexOrderCw" }
        , { Function::__Resolved::PixelOrigin::Upper, "OriginUpperLeft" }
        , { Function::__Resolved::PixelOrigin::Lower, "OriginLowerLeft" }
    };
    
    static std::unordered_map<Function::__Resolved::PixelOrigin, SPVEnum> pixelOriginEnumMap =
    {
        { Function::__Resolved::PixelOrigin::Center, ExecutionModes::VertexOrderCw }
        , { Function::__Resolved::PixelOrigin::Upper, ExecutionModes::OriginUpperLeft }
        , { Function::__Resolved::PixelOrigin::Lower, ExecutionModes::OriginLowerLeft }
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

    static std::unordered_map<Function::__Resolved::PrimitiveTopology, SPVEnum> inputPrimitiveTopologyEnumMap =
    {
        { Function::__Resolved::PrimitiveTopology::Points, ExecutionModes::InputPoints }
        , { Function::__Resolved::PrimitiveTopology::Lines, ExecutionModes::InputLines }
        , { Function::__Resolved::PrimitiveTopology::LinesAdjacency, ExecutionModes::InputLinesAdjacency }
        , { Function::__Resolved::PrimitiveTopology::Triangles, ExecutionModes::Triangles }
        , { Function::__Resolved::PrimitiveTopology::TrianglesAdjacency, ExecutionModes::InputTrianglesAdjacency }
        , { Function::__Resolved::PrimitiveTopology::Quads, ExecutionModes::Quads }
        , { Function::__Resolved::PrimitiveTopology::Isolines, ExecutionModes::Isolines }
};

    static std::unordered_map<Function::__Resolved::PrimitiveTopology, std::string> outputPrimitiveTopologyMap =
    {
        { Function::__Resolved::PrimitiveTopology::Points, "OutputPoints" }
        , { Function::__Resolved::PrimitiveTopology::Lines, "OutputLineStrip" }
        , { Function::__Resolved::PrimitiveTopology::Triangles, "OutputTriangleStrip" }
    };

    static std::unordered_map<Function::__Resolved::PrimitiveTopology, SPVEnum> outputPrimitiveTopologyEnumMap =
    {
        { Function::__Resolved::PrimitiveTopology::Points, ExecutionModes::OutputPoints }
        , { Function::__Resolved::PrimitiveTopology::Lines, ExecutionModes::OutputLineStrip }
        , { Function::__Resolved::PrimitiveTopology::Triangles, ExecutionModes::OutputTriangleStrip }
    };

    this->evaluatingProgram = progResolved;
    for (uint32_t mapping = Program::__Resolved::ProgramEntryType::FirstShader; mapping < Program::__Resolved::ProgramEntryType::LastShader; mapping++)
    {
        Cleanup cleanup(this);

        Symbol* object = progResolved->mappings[mapping];
        if (object == nullptr)
            continue;

        this->shaderStage = mapping;
        this->shaderValueExpressions[mapping].value = true;

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

        SPVHeader header;
        header.magic = 0x00010500;
        header.version.leading = 0;
        header.version.major = 1;
        header.version.minor = 0;
        header.version.trailing = 0;
        header.schema = 0;
        header.bound = 1000;

        this->writer->Capability(extensionEnumMap[(Program::__Resolved::ProgramEntryType)mapping]);
        if (compiler->target.supportsPhysicalAddressing)
        {
            this->writer->Capability(Capabilities::Addresses);
            this->writer->Instruction(OpMemoryModel, SPVWriter::Section::Header, AddressingModels::Physical64, MemoryModels::GLSL450);
        }
        else
        {
            this->writer->Capability(Capabilities::Addresses);
            this->writer->Instruction(OpMemoryModel, SPVWriter::Section::Header, AddressingModels::PhysicalStorageBuffer64, MemoryModels::GLSL450);
        }
        
        this->header.Line("; Magic:     0x00010500 (SPIRV Universal 1.5)");
        this->header.Line("; Version:   0x00010000 (Version: 1.0.0)");
        this->header.Line("; Generator: 0x00080001 (GPULang; 1)");
        this->AddCapability(extensionMap[(Program::__Resolved::ProgramEntryType)mapping]);

        if (compiler->target.supportsPhysicalAddressing)
        {
            this->AddCapability("Addresses");
            this->header.Line(" OpMemoryModel Physical64 GLSL450");
        }
        else
        {
            this->AddCapability("PhysicalStorageBufferAddresses");
            this->header.Line(" OpMemoryModel PhysicalStorageBuffer64 GLSL450");
        }


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
                case Symbol::SamplerStateType:
                    GenerateSamplerSPIRV(compiler, this, sym);
                    break;
                case Symbol::VariableType:
                    GenerateVariableSPIRV(compiler, this, sym, false, true);
                    break;
            }
        }

        Function::__Resolved* funResolved = Symbol::Resolved(static_cast<Function*>(object));
        uint32_t entryFunction = this->GetSymbol(funResolved->name).value;

        if (funResolved->executionModifiers.groupSize != 64 || funResolved->executionModifiers.groupsPerWorkgroup != 1)
        {
            this->writer->Capability(Capabilities::SubgroupDispatch);
            this->AddCapability("SubgroupDispatch");
        }
        std::string interfaces = "";
        SPVArg* interfaceVars = (SPVArg*)AllocStack<SPVArg>(this->interfaceVariables.size());
        uint32_t interfaceVarCounter = 0;
        for (const uint32_t inter : this->interfaceVariables)
        {
            interfaces.append(Format("%%%d ", inter));
            interfaceVars[interfaceVarCounter++] = SPVArg(inter);
        }
        this->header.Line(Format(" OpEntryPoint %s %%%d \"main\" %s\n", executionModelMap[(Program::__Resolved::ProgramEntryType)mapping].c_str(), entryFunction, interfaces.c_str()));

        this->writer->Instruction(OpEntryPoint, SPVWriter::Section::Header, executionModelEnumMap[(Program::__Resolved::ProgramEntryType)mapping], SPVArg(entryFunction), "main", SPVArgList{ .vals = interfaceVars, .num = (uint8_t)interfaceVarCounter });
        DeallocStack(this->interfaceVariables.size(), interfaceVars);
        
        switch (mapping)
        {
            case Program::__Resolved::GeometryShader:
                this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{entryFunction}, ExecutionModes::Invocations, funResolved->executionModifiers.invocations);
                this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{entryFunction}, ExecutionModes::OutputVertices, funResolved->executionModifiers.maxOutputVertices);
                this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{entryFunction}, inputPrimitiveTopologyEnumMap[funResolved->executionModifiers.inputPrimitiveTopology]);
                this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{entryFunction}, outputPrimitiveTopologyEnumMap[funResolved->executionModifiers.inputPrimitiveTopology]);
            
                this->header.Line(" OpExecutionMode", SPVArg(entryFunction), "Invocations", funResolved->executionModifiers.invocations);
                this->header.Line(" OpExecutionMode", SPVArg(entryFunction), inputPrimitiveTopologyMap[funResolved->executionModifiers.inputPrimitiveTopology]);
                this->header.Line(" OpExecutionMode", SPVArg(entryFunction), outputPrimitiveTopologyMap[funResolved->executionModifiers.outputPrimitiveTopology]);
                this->header.Line(" OpExecutionMode", SPVArg(entryFunction), "OutputVertices", funResolved->executionModifiers.maxOutputVertices);
                break;
            case Program::__Resolved::HullShader:
                this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{entryFunction}, partitionMap[funResolved->executionModifiers.partitionMethod]);
                this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{entryFunction}, windingOrderMap[funResolved->executionModifiers.windingOrder]);
                this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{entryFunction}, inputPrimitiveTopologyEnumMap[funResolved->executionModifiers.inputPrimitiveTopology]);
                this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{entryFunction}, ExecutionModes::OutputVertices, funResolved->executionModifiers.maxOutputVertices);
            
                this->header.Line(" OpExecutionMode", SPVArg(entryFunction), partitionMap[funResolved->executionModifiers.partitionMethod]);
                this->header.Line(" OpExecutionMode", SPVArg(entryFunction), windingOrderMap[funResolved->executionModifiers.windingOrder]);
                this->header.Line(" OpExecutionMode", SPVArg(entryFunction), inputPrimitiveTopologyMap[funResolved->executionModifiers.inputPrimitiveTopology]);
                this->header.Line(" OpExecutionMode", SPVArg(entryFunction), "OutputVertices", funResolved->executionModifiers.maxOutputVertices);
                break;
            case Program::__Resolved::DomainShader:
                break;
            case Program::__Resolved::PixelShader:
                this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{entryFunction}, pixelOriginEnumMap[funResolved->executionModifiers.pixelOrigin]);
                this->header.Line(" OpExecutionMode", SPVArg(entryFunction), pixelOriginMap[funResolved->executionModifiers.pixelOrigin]);
                if (funResolved->executionModifiers.earlyDepth)
                    this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{entryFunction}, ExecutionModes::EarlyFragmentTests);
                    this->header.Line(" OpExecutionMode", SPVArg(entryFunction), "EarlyFragmentTests");
                break;
            case Program::__Resolved::ComputeShader:
                this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{entryFunction}, ExecutionModes::LocalSize, funResolved->executionModifiers.computeShaderWorkGroupSize[0], funResolved->executionModifiers.computeShaderWorkGroupSize[1], funResolved->executionModifiers.computeShaderWorkGroupSize[2]);
                this->header.Line(" OpExecutionMode", SPVArg(entryFunction), "LocalSize", funResolved->executionModifiers.computeShaderWorkGroupSize[0], funResolved->executionModifiers.computeShaderWorkGroupSize[1], funResolved->executionModifiers.computeShaderWorkGroupSize[2]);
                if (funResolved->executionModifiers.groupSize != 64)
                    this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{entryFunction}, ExecutionModes::SubgroupSize, funResolved->executionModifiers.groupSize);
                    this->header.Line(" OpExecutionMode", SPVArg(entryFunction), "SubgroupSize", funResolved->executionModifiers.groupSize);
                if (funResolved->executionModifiers.groupsPerWorkgroup != 1)
                    this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{entryFunction}, ExecutionModes::SubgroupsPerWorkgroup, funResolved->executionModifiers.groupsPerWorkgroup);
                    this->header.Line(" OpExecutionMode", SPVArg(entryFunction), "SubgroupsPerWorkgroup", funResolved->executionModifiers.groupsPerWorkgroup);
                if (funResolved->executionModifiers.computeDerivativeIndexing == Function::__Resolved::DerivativeIndexLinear)
                {
                    this->writer->Extension(SPV_KHR_compute_shader_derivatives);
                    this->writer->Capability(Capabilities::ComputeDerivativeGroupLinearKHR);
                    this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg(entryFunction), ExecutionModes::DerivativeGroupLinearKHR);
                    this->AddExtension("SPV_KHR_compute_shader_derivatives");
                    this->AddCapability("ComputeDerivativeGroupLinearNV");
                    this->header.Line(" OpExecutionMode", SPVArg(entryFunction), "DerivativeGroupLinearNV");
                }
                else if (funResolved->executionModifiers.computeDerivativeIndexing == Function::__Resolved::DerivativeIndexQuad)
                {
                    this->writer->Extension(SPV_KHR_compute_shader_derivatives);
                    this->writer->Capability(Capabilities::ComputeDerivativeGroupQuadsKHR);
                    this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg(entryFunction), ExecutionModes::DerivativeGroupQuadsKHR);
                    this->AddExtension("SPV_NV_compute_shader_derivatives");
                    this->AddCapability("ComputeDerivativeGroupQuadsNV");
                    this->header.Line(" OpExecutionMode", SPVArg(entryFunction), "DerivativeGroupQuadsNV");
                }
                break;
        }
        this->shaderValueExpressions[mapping].value = false;
        
        // Reset variable values
        auto it2 = originalVariableValues.begin();
        for (; it2 != originalVariableValues.end(); it2++)
        {
            originalVariableValues[it2->first] = it2->second;
        }

        // Compose and convert to binary, then validate
        GrowingString binary;
        binary.Line("; Header\n");
        binary.Line("; Entry point", funResolved->name);
        binary.Append(this->capability);
        binary.Append(this->extension);
        binary.Append(this->extImport);
        binary.Append(this->header);
        binary.Append("\n; Decorations\n");
        binary.Append(this->decorations);
        binary.Append("\n; Declarations\n");
        binary.Append(this->declarations);
        binary.Append("\n; Functions\n");
        binary.Append(this->functions);
        
        GrowingString writerBinary;
        writerBinary.Line("; Header\n");
        writerBinary.Line("; Entry point", funResolved->name);
        writerBinary.Append(this->writer->texts[(uint32_t)SPVWriter::Section::Capabilities]);
        writerBinary.Append(this->writer->texts[(uint32_t)SPVWriter::Section::Extensions]);
        writerBinary.Append(this->writer->texts[(uint32_t)SPVWriter::Section::ExtImports]);
        writerBinary.Append(this->writer->texts[(uint32_t)SPVWriter::Section::Header]);
        writerBinary.Append("\n; Decorations\n");
        writerBinary.Append(this->writer->texts[(uint32_t)SPVWriter::Section::Decorations]);
        writerBinary.Append("\n; Declarations\n");
        writerBinary.Append(this->writer->texts[(uint32_t)SPVWriter::Section::Declarations]);
        writerBinary.Append("\n; Functions\n");
        writerBinary.Append(this->writer->texts[(uint32_t)SPVWriter::Section::Functions]);
        
        spv_binary bin = nullptr;

        spv_diagnostic diag = nullptr;
        spv_result_t res = spvTextToBinaryWithOptions(spvContext, binary.data, binary.size, SPV_BINARY_TO_TEXT_OPTION_NONE, &bin, &diag);

        if (res != SPV_SUCCESS)
        {
            this->Error(Format("Internal SPIRV generation error: %s", diag->error));
            return false;
        }
        
        if (compiler->options.validate)
        {
            // Run spv validation for internal consistency and fault testing
            spv_const_binary_t constBin = { bin->code, bin->wordCount };
            res = spvValidate(spvContext, &constBin, &diag);
            if (res != SPV_SUCCESS)
            {
                spv_diagnostic diag2;
                spv_text text;
                spvBinaryToText(spvContext, bin->code, bin->wordCount, SPV_BINARY_TO_TEXT_OPTION_FRIENDLY_NAMES, &text, &diag2);
                this->Error(Format("Internal SPIRV generation error: %s", diag->error));
                this->Error(std::string(text->str, text->length));
                //return false;
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
SPIRVGenerator::AddSymbol(const TransientString& name, const TransientString& declare, bool global)
{
    if (global)
    {
        auto scope = this->scopeStack.rbegin();
        while (scope != this->scopeStack.rend())
        {
            auto it = scope->symbols.find(name.ToString());
            if (it != scope->symbols.end())
            {
                return it->second.value;
            }
            scope++;
        }
    }
    else
    {
        auto& scope = this->scopeStack.back();
        auto it = scope.symbols.find(name.ToString());
        if (it != scope.symbols.end())
        {
            return it->second.value;
        }
    }

    // If symbol isn't found in scope, create it
    uint32_t ret = this->symbolCounter;
    if (global)
        this->scopeStack.front().symbols[name.ToString()] = { .sym = nullptr, .value = ret, .type = SPIRVResult::Invalid() };
    else
        this->scopeStack.back().symbols[name.ToString()] = { .sym = nullptr, .value = ret, .type = SPIRVResult::Invalid() };

    if (global)
        this->declarations.Line(SPVArg(ret), "=", declare, ";", name);
    else
        this->functional.Line(SPVArg(ret), "=", declare, ";", name);
    this->symbolCounter++;
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
uint32_t
SPIRVGenerator::AddSymbol(const TransientString& name, const TransientString& declare, SPIRVResult type, bool global)
{
    if (global)
    {
        auto scope = this->scopeStack.rbegin();
        while (scope != this->scopeStack.rend())
        {
            auto it = scope->symbols.find(name.ToString());
            if (it != scope->symbols.end())
            {
                return it->second.value;
            }
            scope++;
        }
    }
    else
    {
        auto& scope = this->scopeStack.back();
        auto it = scope.symbols.find(name.ToString());
        if (it != scope.symbols.end())
        {
            return it->second.value;
        }
    }

    // If symbol isn't found in scope, create it
    uint32_t ret = this->symbolCounter;
    if (global)
        this->scopeStack.front().symbols[name.ToString()] = { .sym = nullptr, .value = ret, .type = type };
    else
        this->scopeStack.back().symbols[name.ToString()] = { .sym = nullptr, .value = ret, .type = type };

    if (global)
        this->declarations.Line(SPVArg(ret), "=", declare, ";", name);
    else
        this->functional.Line(SPVArg(ret), "=", declare, ";", name);
    this->symbolCounter++;
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
void
SPIRVGenerator::AddReservedSymbol(const TransientString& name, uint32_t object, const TransientString& declare, bool global)
{
    auto scope = this->scopeStack.rbegin();
    while (scope != this->scopeStack.rend())
    {
        auto it = scope->symbols.find(name.ToString());
        if (it != scope->symbols.end())
        {
            assert(false);
            //break;
        }
        scope++;
    }

    // If symbol isn't found in scope, create it
    if (global)
        this->scopeStack.front().symbols[name.ToString()] = { .sym = nullptr, .value = object };
    else
        this->scopeStack.back().symbols[name.ToString()] = { .sym = nullptr, .value = object };

    if (global)
        this->declarations.Line(SPVArg(object), "=", declare, ";", name);
    else
        this->functional.Line(SPVArg(object), "=", declare, ";", name);
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
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
void 
SPIRVGenerator::AddOp(const TransientString& value, bool global, TransientString comment)
{
    if (global)
        this->declarations.Line(value, ";", comment);
    else
        this->functional.Line(value, ";", comment);
}

//------------------------------------------------------------------------------
/**
*/
uint32_t 
SPIRVGenerator::AddMappedOp(const TransientString& name, TransientString comment)
{
    uint32_t ret = this->symbolCounter;
    this->functional.Line(SPVArg(ret), "=", name, ";", comment);
    this->symbolCounter++;
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
void 
SPIRVGenerator::AddCapability(const TransientString& declare)
{
    bool found = false;
    for (auto& cap : this->capabilities)
        if (cap == declare.ToString())
            found = true;
    if (!found)
    {
        this->capability.Line(" OpCapability", declare);
        this->capabilities.push_back(declare.ToString());
    }
}

//------------------------------------------------------------------------------
/**
*/
uint32_t 
SPIRVGenerator::ImportExtension(const TransientString& name)
{
    uint32_t ret = 0xFFFFFFFF;
    auto it = this->extensions.find(name.ToString());
    if (it == this->extensions.end())
    {
        this->extImport.CompactLine(SPVArg(this->symbolCounter), " = OpExtInstImport ", "\"", name, "\"");
        this->extensions[name.ToString()] = this->symbolCounter;
        ret = this->symbolCounter++;
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
SPIRVGenerator::AddExtension(const TransientString& name)
{
    auto it = this->extensions.find(name.ToString());
    if (it == this->extensions.end())
    {
        this->extension.CompactLine(" OpExtension ", "\"", name, "\"");
        this->extensions[name.ToString()] = -1;
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
SPIRVGenerator::AddDecoration(const TransientString& name, uint32_t object, const TransientString& decorate)
{
    auto it = this->decorationMap.find(name.ToString());
    if (it == this->decorationMap.end())
    {
        this->decorationMap[name.ToString()].insert(decorate.ToString());
        this->decorations.Line(" OpDecorate", SPVArg(object), decorate, ";", name);
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
SPIRVGenerator::AddMemberDecoration(uint32_t struc, uint32_t index, const TransientString& decorate)
{
    TransientString decorationName;
    decorationName.Concatenate<true>(" OpMemberDecorate", SPVArg(struc), index, decorate);
    
    // = SmallFormat(" OpMemberDecorate %%%d %d %s\n", struc, index, decorate.c_str());
    auto it = this->decorationMap.find(decorationName.ToString());
    if (it == this->decorationMap.end())
    {
        this->decorationMap[decorationName.ToString()].insert(decorationName.ToString());
        this->decorations.Line(decorationName);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
SPIRVGenerator::AddMapping(const TransientString& name, uint32_t object)
{
    auto scope = this->scopeStack.rbegin();
    while (scope != this->scopeStack.rend())
    {
        auto it = scope->symbols.find(name.ToString());
        if (it != scope->symbols.end())
        {
            assert(false);
        }
        scope++;
    }

    // If symbol isn't found in scope, create it
    this->scopeStack.back().symbols[name.ToString()] = { .sym = nullptr, .value = object };
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
SPIRVGenerator::AddReserved(const TransientString& op, uint32_t name, TransientString comment)
{
    this->functional.Line(SPVArg(name), "=", op, ";", comment);
}

//------------------------------------------------------------------------------
/**
*/
uint32_t 
SPIRVGenerator::AddVariableDeclaration(Symbol* sym, const TransientString& name, uint32_t typeName, uint32_t init, uint32_t copy, SPIRVResult::Storage storage, SPIRVResult type, bool global)
{
    if (global)
    {
        auto scope = this->scopeStack.rbegin();
        while (scope != this->scopeStack.rend())
        {
            auto it = scope->symbols.find(name.ToString());
            if (it != scope->symbols.end())
            {
                return it->second.value;
            }
            scope++;
        }
    }
    else
    {
        auto& scope = this->scopeStack.back();
        auto it = scope.symbols.find(name.ToString());
        if (it != scope.symbols.end())
        {
            return it->second.value;
        }
    }

    // If symbol isn't found in scope, create it
    uint32_t ret = this->symbolCounter;
    this->scopeStack.back().symbols[name.ToString()] = SymbolAssignment{ .sym = sym, .value = ret, .type = type };

    ConstantString scopeStr = SPIRVResult::ScopeToString(storage);

    if (!global)
    {
        if (init != 0xFFFFFFFF)
            this->variableDeclarations.Line(SPVArg(this->symbolCounter), "=", "OpVariable", SPVArg(typeName), scopeStr, SPVArg(init), ";", name);
        else
            this->variableDeclarations.Line(SPVArg(this->symbolCounter), "=", "OpVariable", SPVArg(typeName), scopeStr, ";", name);

        if (copy != 0xFFFFFFFF)
            this->parameterInitializations.Line(" OpStore", SPVArg(this->symbolCounter), SPVArg(copy));
    }
    else
    {
        if (storage == SPIRVResult::Storage::StorageBuffer 
            || storage == SPIRVResult::Storage::Image 
            || storage == SPIRVResult::Storage::MutableImage 
            || storage == SPIRVResult::Storage::Uniform 
            || storage == SPIRVResult::Storage::UniformConstant
            || storage == SPIRVResult::Storage::Private
            || storage == SPIRVResult::Storage::Sampler
            || storage == SPIRVResult::Storage::WorkGroup
            )
            this->interfaceVariables.insert(this->symbolCounter);

        if (init != 0xFFFFFFFF)
            this->declarations.Line(SPVArg(this->symbolCounter), "=", "OpVariable", SPVArg(typeName), scopeStr, SPVArg(init), ";", name);
        else
            this->declarations.Line(SPVArg(this->symbolCounter), "=", "OpVariable", SPVArg(typeName), scopeStr, ";", name);
    }
    return this->symbolCounter++;

}

//------------------------------------------------------------------------------
/**
*/
void
SPIRVGenerator::PushAccessChain(Type* chain, SPIRVResult::Storage scope)
{
    this->accessChain.push_back(std::make_tuple(chain, scope));
}

//------------------------------------------------------------------------------
/**
*/
void
SPIRVGenerator::PopAccessChain()
{
    this->accessChain.pop_back();
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
