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
#include "ast/generate.h"
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

#include "generated/types.h"
#include "generated/intrinsics.h"
#include "ast/intrinsics.h"

#include "spirv-tools/libspirv.h"
#include "spirv-tools/optimizer.hpp"

#include <array>
#include <complex>
#include <format>
#include <algorithm>

#include "ast/samplerstate.h"
#include "ast/expressions/arrayinitializerexpression.h"
#include "ast/expressions/floatvecexpression.h"

namespace GPULang
{
struct SPVHeader
{
    uint32_t magic;
    union
    {
        struct
        {
            uint32_t leading : 8;
            uint32_t minor : 8;
            uint32_t major : 8;
            uint32_t trailing : 8;
        } flags;
        uint32_t bits;
    } version;
    uint32_t generatorVersion;
    uint32_t bound; // range of IDs generated, the tighter the better
    uint32_t schema;
};

union SPVInstruction
{
    struct
    {
        uint32_t code : 16;
        uint32_t wordCount : 16;
    } flags;
    uint32_t bits;
};

struct SPVOp
{
    const char* str = nullptr;
    const uint16_t c = 0xFFFF;
    const uint16_t wordCount = 1;
    const bool dynamicWords = false;
};

struct SPVEnum
{
    const char* str = nullptr;
    uint16_t c = 0xFFFF;

    SPVEnum operator|(const SPVEnum& rhs)
    {
        char buf[256];
        snprintf(buf, 256, "%s|%s", this->str, rhs.str);
        uint16_t code = this->c | rhs.c;
        return SPVEnum{.str = strdup(buf), .c = code};
    }
    void operator=(const SPVEnum& rhs)
    {
        this->str = rhs.str;
        this->c = rhs.c;
    }
    
    bool operator==(const SPVEnum& rhs) const
    {
        return this->c == rhs.c;
    }
    
    bool operator!=(const SPVEnum& rhs) const
    {
        return this->c != rhs.c;
    }
};

bool operator<(const SPVEnum& lhs, const SPVEnum& rhs)
{
    return lhs.c < rhs.c;
}

struct SPVComment
{
    const char* str = nullptr;
};

struct SPVArg
{
    uint32_t arg;
    SPVArg()
        : arg(UINT32_MAX)
    {
    }
    SPVArg(const uint32_t arg)
        : arg(arg)
    {
    }
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

    explicit SPVResultList(const TransientArray<SPIRVResult>& stackArr)
    {
        this->vals = stackArr.ptr;
        this->num = (uint8_t)stackArr.size;
    }
};

struct SPVArgList
{
    SPVArg* vals = nullptr;
    uint8_t num = 0;

    explicit SPVArgList(const TransientArray<SPVArg>& stackArr)
    {
        this->vals = stackArr.ptr;
        this->num = (uint8_t)stackArr.size;
    }
};

struct SPVCaseList
{
    uint32_t* labels = nullptr;
    SPVArg* branches = nullptr;
    uint8_t num = 0;

    explicit SPVCaseList(const TransientArray<uint32_t>& labels, const TransientArray<SPVArg>& branches)
    {
        this->labels = labels.ptr;
        this->branches = branches.ptr;
        this->num = (uint8_t)labels.size;
    }
};


struct ConstantCreationInfo
{
    enum class Type
    {
        Float,
        Float32 = Float,
        Float16,
        Int,
        Int32 = Int,
        Int16,
        UInt,
        UInt32 = UInt,
        UInt16,
        Bool,
        Bool8 = Bool
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
    
    static ConstantCreationInfo Float32(float val)
    {
        ConstantCreationInfo ret;
        ret.type = Type::Float32;
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
    
    static ConstantCreationInfo UInt32(uint32_t val)
    {
        ConstantCreationInfo ret;
        ret.type = Type::UInt32;
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
    
    static ConstantCreationInfo Int32(int32_t val)
    {
        ConstantCreationInfo ret;
        ret.type = Type::Int32;
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
    
    static ConstantCreationInfo Bool8(bool b)
    {
        ConstantCreationInfo ret;
        ret.type = Type::Bool;
        ret.data.b = b;
        return ret;
    }
};

#define SPV_INSTRUCTION(name, code, words, dyn) constexpr SPVOp name { .str = #name, .c = code, .wordCount = words, .dynamicWords = dyn };
SPV_INSTRUCTION(OpNop, 0, 1, false)
SPV_INSTRUCTION(OpSourceContinued, 2, 2, true)
SPV_INSTRUCTION(OpSource, 3, 3, true)
SPV_INSTRUCTION(OpName, 5, 3, true)
SPV_INSTRUCTION(OpMemberName, 6, 4, true)
SPV_INSTRUCTION(OpString, 7, 3, true)
SPV_INSTRUCTION(OpLine, 8, 4, false)
SPV_INSTRUCTION(OpNoLine, 317, 1, false)
SPV_INSTRUCTION(OpExtension, 10, 2, true)
SPV_INSTRUCTION(OpExtInstImport, 11, 3, true)
SPV_INSTRUCTION(OpExtInst, 12, 5, true)
SPV_INSTRUCTION(OpMemoryModel, 14, 3, false)
SPV_INSTRUCTION(OpEntryPoint, 15, 4, true)
SPV_INSTRUCTION(OpExecutionMode, 16, 3, true)
SPV_INSTRUCTION(OpCapability, 17, 2, false)
SPV_INSTRUCTION(OpTypeVoid, 19, 2, false)
SPV_INSTRUCTION(OpTypeBool, 20, 2, false)
SPV_INSTRUCTION(OpTypeInt, 21, 4, false)
SPV_INSTRUCTION(OpTypeFloat, 22, 3, true)
SPV_INSTRUCTION(OpTypeVector, 23, 4, false)
SPV_INSTRUCTION(OpTypeMatrix, 24, 4, false)
SPV_INSTRUCTION(OpTypeImage, 25, 9, true)
SPV_INSTRUCTION(OpTypeSampler, 26, 2, false)
SPV_INSTRUCTION(OpTypeSampledImage, 27, 3, false)
SPV_INSTRUCTION(OpTypeArray, 28, 4, false)
SPV_INSTRUCTION(OpTypeRuntimeArray, 29, 3, false)
SPV_INSTRUCTION(OpTypeStruct, 30, 2, true)
SPV_INSTRUCTION(OpTypePointer, 32, 4, false)
SPV_INSTRUCTION(OpTypeFunction, 33, 3, true)
SPV_INSTRUCTION(OpConstantTrue, 41, 3, false)
SPV_INSTRUCTION(OpConstantFalse, 42, 3, false)
SPV_INSTRUCTION(OpConstant, 43, 4, true)
SPV_INSTRUCTION(OpConstantComposite, 44, 3, true)
SPV_INSTRUCTION(OpConstantSampler, 45, 6, false)
SPV_INSTRUCTION(OpSpecConstantTrue, 48, 3, false)
SPV_INSTRUCTION(OpSpecConstantFalse, 49, 3, false)
SPV_INSTRUCTION(OpSpecConstant, 50, 4, true)
SPV_INSTRUCTION(OpSpecConstantComposite, 51, 3, true)
SPV_INSTRUCTION(OpFunction, 54, 5, false)
SPV_INSTRUCTION(OpFunctionParameter, 55, 3, false)
SPV_INSTRUCTION(OpFunctionEnd, 56, 1, false)
SPV_INSTRUCTION(OpFunctionCall, 57, 4, true)
SPV_INSTRUCTION(OpVariable, 59, 4, true)
SPV_INSTRUCTION(OpAccessChain, 65, 4, true)
SPV_INSTRUCTION(OpLoad, 61, 4, true)
SPV_INSTRUCTION(OpStore, 62, 3, true)
SPV_INSTRUCTION(OpCopyMemory, 63, 3, true)
SPV_INSTRUCTION(OpDecorate, 71, 3, true)
SPV_INSTRUCTION(OpMemberDecorate, 72, 4, true)
SPV_INSTRUCTION(OpVectorShuffle, 79, 5, true)
SPV_INSTRUCTION(OpCompositeConstruct, 80, 3, true)
SPV_INSTRUCTION(OpCompositeExtract, 81, 4, true)
SPV_INSTRUCTION(OpCompositeInsert, 82, 5, true)
SPV_INSTRUCTION(OpTranspose, 84, 4, false)
SPV_INSTRUCTION(OpSampledImage, 86, 5, false)
SPV_INSTRUCTION(OpImageSampleImplicitLod, 87, 5, true)
SPV_INSTRUCTION(OpImageSampleExplicitLod, 88, 7, true)
SPV_INSTRUCTION(OpImageSampleDrefImplicitLod, 89, 6, true)
SPV_INSTRUCTION(OpImageSampleDrefExplicitLod, 90, 8, true)
SPV_INSTRUCTION(OpImageSampleProjImplicitLod, 91, 5, true)
SPV_INSTRUCTION(OpImageSampleProjExplicitLod, 92, 7, true)
SPV_INSTRUCTION(OpImageSampleProjDrefImplicitLod, 93, 6, true)
SPV_INSTRUCTION(OpImageSampleProjDrefExplicitLod, 94, 8, true)
SPV_INSTRUCTION(OpImageFetch, 95, 5, true)
SPV_INSTRUCTION(OpImageGather, 96, 6, true)
SPV_INSTRUCTION(OpImageDrefGather, 97, 6, true)
SPV_INSTRUCTION(OpImageRead, 98, 5, true)
SPV_INSTRUCTION(OpImageWrite, 99, 4, true)
SPV_INSTRUCTION(OpImageQuerySizeLod, 103, 5, false)
SPV_INSTRUCTION(OpImageQuerySize, 104, 4, false)
SPV_INSTRUCTION(OpImageQueryLod, 105, 5, false)
SPV_INSTRUCTION(OpImageQueryLevels, 106, 4, false)
SPV_INSTRUCTION(OpImageQuerySamples, 107, 4, false)
SPV_INSTRUCTION(OpConvertFToU, 109, 4, false)
SPV_INSTRUCTION(OpConvertFToS, 110, 4, false)
SPV_INSTRUCTION(OpConvertSToF, 111, 4, false)
SPV_INSTRUCTION(OpConvertUToF, 112, 4, false)
SPV_INSTRUCTION(OpUConvert, 113, 4, false)
SPV_INSTRUCTION(OpSConvert, 114, 4, false)
SPV_INSTRUCTION(OpFConvert, 115, 4, false)
SPV_INSTRUCTION(OpBitcast, 124, 4, false)
SPV_INSTRUCTION(OpSNegate, 126, 4, false)
SPV_INSTRUCTION(OpFNegate, 127, 4, false)
SPV_INSTRUCTION(OpIAdd, 128, 5, false)
SPV_INSTRUCTION(OpFAdd, 129, 5, false)
SPV_INSTRUCTION(OpISub, 130, 5, false)
SPV_INSTRUCTION(OpFSub, 131, 5, false)
SPV_INSTRUCTION(OpIMul, 132, 5, false)
SPV_INSTRUCTION(OpFMul, 133, 5, false)
SPV_INSTRUCTION(OpUDiv, 134, 5, false)
SPV_INSTRUCTION(OpSDiv, 135, 5, false)
SPV_INSTRUCTION(OpFDiv, 136, 5, false)
SPV_INSTRUCTION(OpUMod, 137, 5, false)
SPV_INSTRUCTION(OpSMod, 139, 5, false)
SPV_INSTRUCTION(OpFMod, 141, 5, false)
SPV_INSTRUCTION(OpVectorTimesScalar, 142, 5, false)
SPV_INSTRUCTION(OpMatrixTimesScalar, 143, 5, false)
SPV_INSTRUCTION(OpVectorTimesMatrix, 144, 5, false)
SPV_INSTRUCTION(OpMatrixTimesVector, 145, 5, false)
SPV_INSTRUCTION(OpMatrixTimesMatrix, 146, 5, false)
SPV_INSTRUCTION(OpDot, 148, 5, false)
SPV_INSTRUCTION(OpAny, 154, 4, false)
SPV_INSTRUCTION(OpAll, 155, 4, false)
SPV_INSTRUCTION(OpLogicalOr, 166, 5, false)
SPV_INSTRUCTION(OpLogicalAnd, 167, 5, false)
SPV_INSTRUCTION(OpLogicalNot, 168, 5, false)
SPV_INSTRUCTION(OpSelect, 169, 6, false)
SPV_INSTRUCTION(OpIEqual, 170, 5, false)
SPV_INSTRUCTION(OpINotEqual, 171, 5, false)
SPV_INSTRUCTION(OpUGreaterThan, 172, 5, false)
SPV_INSTRUCTION(OpSGreaterThan, 173, 5, false)
SPV_INSTRUCTION(OpUGreaterThanEqual, 174, 5, false)
SPV_INSTRUCTION(OpSGreaterThanEqual, 175, 5, false)
SPV_INSTRUCTION(OpULessThan, 176, 5, false)
SPV_INSTRUCTION(OpSLessThan, 177, 5, false)
SPV_INSTRUCTION(OpULessThanEqual, 178, 5, false)
SPV_INSTRUCTION(OpSLessThanEqual, 179, 5, false)
SPV_INSTRUCTION(OpFOrdEqual, 181, 5, false)
SPV_INSTRUCTION(OpFOrdNotEqual, 183, 5, false)
SPV_INSTRUCTION(OpFOrdLessThan, 185, 5, false)
SPV_INSTRUCTION(OpFOrdGreaterThan, 187, 5, false)
SPV_INSTRUCTION(OpFOrdLessThanEqual, 189, 5, false)
SPV_INSTRUCTION(OpFOrdGreaterThanEqual, 191, 5, false)
SPV_INSTRUCTION(OpShiftRightLogical, 194, 5, false)
SPV_INSTRUCTION(OpShiftRightArithmetic, 195, 5, false)
SPV_INSTRUCTION(OpShiftLeftLogical, 196, 5, false)
SPV_INSTRUCTION(OpBitwiseOr, 197, 5, false)
SPV_INSTRUCTION(OpBitwiseXor, 198, 5, false)
SPV_INSTRUCTION(OpBitwiseAnd, 199, 5, false)
SPV_INSTRUCTION(OpNot, 200, 4, false)
SPV_INSTRUCTION(OpBitFieldInsert, 201, 7, false)
SPV_INSTRUCTION(OpBitFieldSExtract, 202, 6, false)
SPV_INSTRUCTION(OpBitFieldUExtract, 203, 6, false)
SPV_INSTRUCTION(OpBitReverse, 204, 4, false)
SPV_INSTRUCTION(OpBitCount, 205, 4, false)
SPV_INSTRUCTION(OpDPdx, 207, 4, false)
SPV_INSTRUCTION(OpDPdy, 208, 4, false)
SPV_INSTRUCTION(OpFwidth, 209, 4, false)
SPV_INSTRUCTION(OpEmitVertex, 218, 1, false)
SPV_INSTRUCTION(OpEndPrimitive, 219, 1, false)
SPV_INSTRUCTION(OpControlBarrier, 224, 4, false)
SPV_INSTRUCTION(OpMemoryBarrier, 225, 3, false)
SPV_INSTRUCTION(OpAtomicLoad, 227, 6, false)
SPV_INSTRUCTION(OpAtomicStore, 228, 5, false)
SPV_INSTRUCTION(OpAtomicExchange, 229, 7, false)
SPV_INSTRUCTION(OpAtomicCompareExchange, 230, 9, false)
SPV_INSTRUCTION(OpAtomicIIncrement, 232, 6, false)
SPV_INSTRUCTION(OpAtomicIDecrement, 233, 6, false)
SPV_INSTRUCTION(OpAtomicIAdd, 234, 7, false)
SPV_INSTRUCTION(OpAtomicISub, 235, 7, false)
SPV_INSTRUCTION(OpAtomicSMin, 236, 7, false)
SPV_INSTRUCTION(OpAtomicUMin, 237, 7, false)
SPV_INSTRUCTION(OpAtomicSMax, 238, 7, false)
SPV_INSTRUCTION(OpAtomicUMax, 239, 7, false)
SPV_INSTRUCTION(OpAtomicAnd, 240, 7, false)
SPV_INSTRUCTION(OpAtomicOr, 241, 7, false)
SPV_INSTRUCTION(OpAtomicXor, 242, 7, false)
SPV_INSTRUCTION(OpLoopMerge, 246, 4, true)
SPV_INSTRUCTION(OpSelectionMerge, 247, 3, true)
SPV_INSTRUCTION(OpLabel, 248, 2, false)
SPV_INSTRUCTION(OpBranch, 249, 2, false)
SPV_INSTRUCTION(OpBranchConditional, 250, 4, true)
SPV_INSTRUCTION(OpSwitch, 251, 3, true)
SPV_INSTRUCTION(OpKill, 252, 1, true)
SPV_INSTRUCTION(OpReturn, 253, 1, false)
SPV_INSTRUCTION(OpReturnValue, 254, 2, false)
SPV_INSTRUCTION(OpUnreachable, 255, 1, false)
SPV_INSTRUCTION(OpGroupNonUniformElect, 333, 4, false)
SPV_INSTRUCTION(OpGroupNonUniformAll, 334, 5, false)
SPV_INSTRUCTION(OpGroupNonUniformAny, 335, 5, false)
SPV_INSTRUCTION(OpGroupNonUniformAllEqual, 336, 5, false)
SPV_INSTRUCTION(OpGroupNonUniformBroadcast, 337, 6, false)
SPV_INSTRUCTION(OpGroupNonUniformBroadcastFirst, 338, 5, false)
SPV_INSTRUCTION(OpGroupNonUniformBallot, 339, 5, false)
SPV_INSTRUCTION(OpGroupNonUniformInverseBallot, 340, 5, false)
SPV_INSTRUCTION(OpGroupNonUniformBallotBitExtract, 341, 6, false)
SPV_INSTRUCTION(OpGroupNonUniformBallotBitCount, 342, 6, false)
SPV_INSTRUCTION(OpGroupNonUniformBallotFindLSB, 343, 5, false)
SPV_INSTRUCTION(OpGroupNonUniformBallotFindMSB, 344, 5, false)
SPV_INSTRUCTION(OpGroupNonUniformQuadSwap, 366, 6, false)
SPV_INSTRUCTION(OpTerminateInvocation, 4416, 1, false)
SPV_INSTRUCTION(OpIgnoreIntersectionKHR, 4448, 1, false)
SPV_INSTRUCTION(OpTerminateRayKHR, 4449, 1, false)
SPV_INSTRUCTION(OpReportIntersectionKHR, 5334, 5, false)


#define SPV_ENUM(name, code) SPVEnum name { .str = #name, .c = code };
#define SPV_ENUM_DIGITED(name, code) SPVEnum _##name { .str = #name, .c = code };
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
SPV_ENUM(InputAttachment, 40)
SPV_ENUM(Sampled1D, 43)
SPV_ENUM(StorageImageExtendedFormats, 49)
SPV_ENUM(ImageQuery, 50)
SPV_ENUM(SubgroupDispatch, 58)
SPV_ENUM(GroupNonUniform, 61)
SPV_ENUM(GroupNonUniformBallot, 64)
SPV_ENUM(GroupNonUniformQuad, 68)
SPV_ENUM(ShaderLayer, 69)
SPV_ENUM(ShaderViewportIndex, 70)
SPV_ENUM(MeshShadingEXT, 5283)
SPV_ENUM(RayTracingKHR, 4479)
SPV_ENUM(Int64ImageEXT, 5016)
SPV_ENUM(ComputeDerivativeGroupLinearKHR, 5350)
SPV_ENUM(ComputeDerivativeGroupQuadsKHR, 5288)
SPV_ENUM(PhysicalStorageBufferAddresses, 5347)
SPV_ENUM(BitInstructions, 6025)
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
SPV_ENUM(DerivativeGroupLinearKHR, 5290)
}

namespace ExecutionScopes
{
SPV_ENUM(CrossDevice, 0)
SPV_ENUM(Device, 1)
SPV_ENUM(Workgroup, 2)
SPV_ENUM(Subgroup, 3)
SPV_ENUM(Invocation, 4)
}

namespace Subgroup
{
SPV_ENUM(Reduce, 0)
SPV_ENUM(InclusiveScan, 1)
SPV_ENUM(ExclusiveScan, 2)
}

namespace FunctionControl
{
SPV_ENUM(None, 0)
SPV_ENUM(Inline, 0x1)
SPV_ENUM(DontInline, 0x2)
SPV_ENUM(Pure, 0x4)
SPV_ENUM(Const, 0x8)
}

namespace SelectionControl
{
SPV_ENUM(None, 0)
SPV_ENUM(Flatten, 1)
SPV_ENUM(DontFlatten, 1)
}

namespace LoopControl
{
SPV_ENUM(None, 0)
SPV_ENUM(Unroll, 1)
SPV_ENUM(DontUnroll, 1)
}

namespace ImageOperands
{
SPV_ENUM(None, 0)
SPV_ENUM(Bias, 0x1)
SPV_ENUM(Lod, 0x2)
SPV_ENUM(Grad, 0x4)
SPV_ENUM(ConstOffset, 0x8)
SPV_ENUM(Offset, 0x10)
SPV_ENUM(ConstOffsets, 0x20)
SPV_ENUM(Sample, 0x40)
SPV_ENUM(MinLod, 0x80)
}

namespace ImageDimensions
{
SPV_ENUM_DIGITED(1D, 0)
SPV_ENUM_DIGITED(2D, 1)
SPV_ENUM_DIGITED(3D, 2)
SPV_ENUM(Cube, 3)
SPV_ENUM(Rect, 4)
SPV_ENUM(Buffer, 5)
SPV_ENUM(SubpassData, 6)
}

namespace ImageFormats
{
SPV_ENUM(Unknown, 0)
SPV_ENUM(Rgba32f, 1)
SPV_ENUM(Rgba16f, 2)
SPV_ENUM(R32f, 3)
SPV_ENUM(Rgba8, 4)
SPV_ENUM(Rgba8Snorm, 5)
SPV_ENUM(Rg32f, 6)
SPV_ENUM(Rg16f, 7)
SPV_ENUM(R11fG11fB10f, 8)
SPV_ENUM(R16f, 9)
SPV_ENUM(Rgba16, 10)
SPV_ENUM(Rgb10A2, 11)
SPV_ENUM(Rg16, 12)
SPV_ENUM(Rg8, 13)
SPV_ENUM(R16, 14)
SPV_ENUM(R8, 15)
SPV_ENUM(Rgba16Snorm, 16)
SPV_ENUM(Rg16Snorm, 17)
SPV_ENUM(Rg8Snorm, 18)
SPV_ENUM(R16Snorm, 19)
SPV_ENUM(R8Snorm, 20)
SPV_ENUM(Rgba32i, 21)
SPV_ENUM(Rgba16i, 22)
SPV_ENUM(Rgba8i, 23)
SPV_ENUM(R32i, 24)
SPV_ENUM(Rg32i, 25)
SPV_ENUM(Rg16i, 26)
SPV_ENUM(Rg8i, 27)
SPV_ENUM(R16i, 28)
SPV_ENUM(R8i, 29)
SPV_ENUM(Rgba32ui, 30)
SPV_ENUM(Rgba16ui, 31)
SPV_ENUM(Rgba8ui, 32)
SPV_ENUM(R32ui, 33)
SPV_ENUM(Rgb10a2ui, 34)
SPV_ENUM(Rg32ui, 35)
SPV_ENUM(Rg16ui, 36)
SPV_ENUM(Rg8ui, 37)
SPV_ENUM(R16ui, 38)
SPV_ENUM(R8ui, 39)
SPV_ENUM(R64ui, 40)
SPV_ENUM(R64i, 41)
}

namespace SamplerAddressingModes
{
SPV_ENUM(None, 0)
SPV_ENUM(ClampToEdge, 1)
SPV_ENUM(Clamp, 2)
SPV_ENUM(Repeat, 3)
SPV_ENUM(RepeatMirrored, 4)
}

namespace SamplerFilterModes
{
SPV_ENUM(Nearest, 0)
SPV_ENUM(Linear, 1)
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

namespace Builtins
{
SPV_ENUM(Position, 0)
SPV_ENUM(PointSize, 1)
SPV_ENUM(ClipDistance, 3)
SPV_ENUM(CullDistance, 4)
SPV_ENUM(VertexId, 5)
SPV_ENUM(InstanceId, 6)
SPV_ENUM(PrimitiveId, 7)
SPV_ENUM(InvocationId, 8)
SPV_ENUM(Layer, 9)
SPV_ENUM(ViewportIndex, 10)
SPV_ENUM(TessLevelOuter, 11)
SPV_ENUM(TessLevelInner, 12)
SPV_ENUM(TessCoord, 13)
SPV_ENUM(PatchVertices, 14)
SPV_ENUM(FragCoord, 15)
SPV_ENUM(PointCoord, 16)
SPV_ENUM(FrontFacing, 17)
SPV_ENUM(SampleId, 18)
SPV_ENUM(SamplePosition, 19)
SPV_ENUM(SampleMask, 20)
SPV_ENUM(FragDepth, 22)
SPV_ENUM(HelperInvocation, 23)
SPV_ENUM(NumWorkgroups, 24)
SPV_ENUM(WorkgroupSize, 25)
SPV_ENUM(WorkgroupId, 26)
SPV_ENUM(LocalInvocationId, 27)
SPV_ENUM(GlobalInvocationId, 28)
SPV_ENUM(LocalInvocationIndex, 29)
SPV_ENUM(SubgroupSize, 36)
SPV_ENUM(SubgroupMaxSize, 37)
SPV_ENUM(NumSubgroups, 38)
SPV_ENUM(SubgroupId, 40)
SPV_ENUM(SubgroupLocalInvocationId, 41)
SPV_ENUM(VertexIndex, 42)
SPV_ENUM(InstanceIndex, 43)
SPV_ENUM(SubgroupEqMask, 4416)
SPV_ENUM(SubgroupGeMask, 4417)
SPV_ENUM(SubgroupGtMask, 4418)
SPV_ENUM(SubgroupLeMask, 4419)
SPV_ENUM(SubgroupLtMask, 4420)
SPV_ENUM(BaseVertex, 4424)
SPV_ENUM(BaseInstance, 4425)
SPV_ENUM(DrawIndex, 4426)
SPV_ENUM(PrimitiveShadingRateKHR, 4432)
SPV_ENUM(LaunchIdKHR, 5319)
SPV_ENUM(LaunchSizeKHR, 5320)
SPV_ENUM(WorldRayOriginKHR, 5321)
SPV_ENUM(WorldRayDirectionKHR, 5322)
SPV_ENUM(ObjectRayOriginKHR, 5323)
SPV_ENUM(ObjectRayDirectionKHR, 5324)
SPV_ENUM(RayTminKHR, 5325)
SPV_ENUM(RayTmaxKHR, 5326)
SPV_ENUM(InstanceCustomIndexKHR, 5327)
SPV_ENUM(ObjectToWorldKHR, 5330)
SPV_ENUM(WorldToObjectKHR, 5331)
SPV_ENUM(HitKindKHR, 5333)
SPV_ENUM(IncomingRayFlagsKHR, 5351)
SPV_ENUM(RayGeometryIndexKHR, 5352)
}

namespace SourceLanguage
{
SPV_ENUM(Unknown, 0)
SPV_ENUM(GPULang, 14)
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
SPV_ENUM(SPV_KHR_bit_instructions, 0xFFFF)

struct SPVWriter
{
    bool outputText = false;

    enum class Section : uint32_t
    {
        Top,
        Capabilities,
        Extensions,
        ExtImports,
        Header,
        DebugStrings,
        DebugNames,
        Decorations,
        Declarations,
        Functions,
        LocalFunction,
        VariableDeclarations,
        ParameterInitializations,
        FunctionInit,
        FunctionStart,

        NumSections
    } section = Section::Header;
    uint32_t counter = 0;

    struct Scope
    {
        std::unordered_map<std::string, SymbolAssignment> symbols;
    };
    std::vector<Scope> scopeStack;

    void PushScope() { this->scopeStack.push_back(Scope()); }
    void PopScope() { this->scopeStack.pop_back(); }

    void Header(uint32_t spvMajor, uint32_t spvMinor, uint32_t compilerVersion, uint32_t generatorVersion, uint32_t bound)
    {
        SPVHeader header;
        header.magic = 0x07230203;
        header.version.flags.leading = 0;
        header.version.flags.major = spvMajor;
        header.version.flags.minor = spvMinor;
        header.version.flags.trailing = 0;
        header.generatorVersion = generatorVersion;
        header.bound = bound;
        header.schema = 0;
            
        this->binaries[(uint32_t)Section::Top].push_back(header.magic);
        this->binaries[(uint32_t)Section::Top].push_back(header.version.bits);
        this->binaries[(uint32_t)Section::Top].push_back(header.generatorVersion);
        this->binaries[(uint32_t)Section::Top].push_back(header.bound);
        this->binaries[(uint32_t)Section::Top].push_back(header.schema);
        if (this->outputText)
        {
            char buf[64];
            uint32_t written = snprintf(buf, 64, "; Magic: 0x%.8lx (SPIRV Universal %d.%d)\n", header.version.bits, spvMajor, spvMinor);
            this->texts[(uint32_t)Section::Top].Append(buf, written);
            written = snprintf(buf, 64, "; Generator: 0x%.8lx (GPULang: %d)\n", 0x00080001, header.generatorVersion);
            this->texts[(uint32_t)Section::Top].Append(buf, written);
        }
    }

    uint32_t Import(const char* str)
    {
        auto it = this->imports.Find(str);
        if (it != this->imports.end())
            return it->second;

        uint32_t c = this->counter++;
        size_t len = strlen(str) + 1;
        size_t lenInWords = std::ceil(len / 4.0f);
        SPVInstruction instr;
        instr.flags.wordCount = OpExtInstImport.wordCount + lenInWords - 1;
        instr.flags.code = OpExtInstImport.c;

        this->binaries[(uint32_t)Section::ExtImports].push_back(instr.bits);
        this->binaries[(uint32_t)Section::ExtImports].push_back(c);
        uint32_t* strAsWords = (uint32_t*)str;
        for (size_t i = 0; i < lenInWords; i++)
        {
            this->binaries[(uint32_t)Section::ExtImports].push_back(strAsWords[i]);
        }
        char* lastInt = (char*)&this->binaries[(uint32_t)Section::ExtImports].back();
        lastInt[3] = '\0';
        if (this->outputText)
        {
            TStr extension = TStr::Compact("\"", str, "\"");
            this->texts[(uint32_t)Section::ExtImports].Append(TStr::Separated(SPVArg(c), "=", "OpExtInstImport", extension));
            this->texts[(uint32_t)Section::ExtImports].Append("\n");
        }
        this->imports.Insert(str, c);
        return c;
    }

    void Capability(SPVEnum cap)
    {
        if (set_contains(this->capabilities, cap))
            return;
        this->Instruction(OpCapability, Section::Capabilities, cap);
        this->capabilities.Insert(cap);
    }

    void Extension(SPVEnum extension)
    {
        if (set_contains(this->extensions, extension))
            return;
        this->Instruction(OpExtension, Section::Extensions, extension.str);
        this->extensions.Insert(extension);
    }

    template<typename ...ARGS>
    void Decorate(SPVArg symbol, SPVEnum decoration, const ARGS&... args)
    {
        std::string key = TStr(symbol.arg, "_", decoration.str, std::forward<const ARGS&>(args)...).ToString();
        if (this->decorations.Find(key) == this->decorations.end())
        {
            this->Instruction(OpDecorate, Section::Decorations, symbol, decoration, std::forward<const ARGS&>(args)...);
            this->decorations.Insert(key);
        }
    }

    uint32_t String(const char* str)
    {
        auto it = this->strings.Find(str);
        if (it == this->strings.end())
        {
            uint32_t ret = this->MappedInstruction(OpString, SPVWriter::Section::DebugStrings, str); 
            this->strings.Insert(str, ret);
            return ret;
        }
        else
            return it->second;
    }

    template<typename ...ARGS>
    void MemberDecorate(SPVArg type, uint32_t member, SPVEnum decoration, const ARGS&... args)
    {
        this->Instruction(OpMemberDecorate, Section::Decorations, type, member, decoration, std::forward<const ARGS&>(args)...);
    }

    template<typename ...ARGS>
    uint32_t MappedInstruction(const SPVOp& op, SPVWriter::Section section, uint32_t type, const ARGS&... args)
    {
        uint32_t c = this->counter++;
        this->section = section;

        SPVInstruction instr;
        instr.flags.wordCount = op.wordCount;
        instr.flags.code = op.c;

        if (op.dynamicWords)
        {
            uint32_t dynamicWordCount = 0;
            ([&]
            {
                dynamicWordCount += ArgCount(args);
            } (), ...);

            // Subtract all necessary words
            dynamicWordCount = dynamicWordCount - (op.wordCount - 3);
            instr.flags.wordCount += dynamicWordCount;
        }
        else
        {
            uint32_t totalArgs = 3;
            ([&] {totalArgs += ArgCount(args);} (), ...);
            assert(op.wordCount == totalArgs);        
        }
        assert(instr.flags.wordCount >= op.wordCount);

        this->binaries[(uint32_t)section].push_back(instr.bits);
        this->binaries[(uint32_t)section].push_back(type);
        this->binaries[(uint32_t)section].push_back(c);

        if (this->outputText)
            this->texts[(uint32_t)section].Append(TStr::Separated(SPVArg(c), "=", op.str, SPVArg(type)).ToString());

        (Append(args), ...);

        if (this->outputText)
            this->texts[(uint32_t)section].Append("\n");

        return c;
    }

    template<typename ...ARGS>
    uint32_t MappedInstruction(const SPVOp& op, SPVWriter::Section section, const ARGS&... args)
    {
        uint32_t c = this->counter++;
        this->section = section;

        SPVInstruction instr;
        instr.flags.wordCount = op.wordCount;
        instr.flags.code = op.c;

        if (op.dynamicWords)
        {
            uint32_t dynamicWordCount = 0;
            ([&]
            {
                dynamicWordCount += ArgCount(args);
            } (), ...);

            // Subtract all necessary words
            dynamicWordCount -= (op.wordCount - 2);
            instr.flags.wordCount += dynamicWordCount;
        }
        else
        {
            uint32_t totalArgs = 2;
            ([&] {totalArgs += ArgCount(args);} (), ...);
            assert(op.wordCount == totalArgs);                
        }

        assert(instr.flags.wordCount >= op.wordCount);

        this->binaries[(uint32_t)section].push_back(instr.bits);
        this->binaries[(uint32_t)section].push_back(c);

        if (this->outputText)
            this->texts[(uint32_t)section].Append(TStr::Separated(SPVArg(c), "=", op.str).ToString());

        (Append(args), ...);

        if (this->outputText)
            this->texts[(uint32_t)section].Append("\n");
        return c;
    }

    void Instruction(const SPVOp& op, SPVWriter::Section section, uint32_t type, std::vector<uint32_t> args)
    {
        this->section = section;
        SPVInstruction instr;
        instr.flags.wordCount = op.wordCount;
        instr.flags.code = op.c;

        if (op.dynamicWords)
        {
            uint32_t dynamicWordCount = args.size() - (op.wordCount - 2);
            instr.flags.wordCount += dynamicWordCount;
        }
        else
        {
            uint32_t totalArgs = 2 + args.size();
            assert(op.wordCount == totalArgs);               
        }

        assert(instr.flags.wordCount >= op.wordCount);
        this->binaries[(uint32_t)section].push_back(instr.bits);
        this->binaries[(uint32_t)section].push_back(type);
        this->binaries[(uint32_t)section].insert(binaries[(uint32_t)this->section].end(), args.begin(), args.end());

        if (this->outputText)
        {
            TStr argsStr;
            for (uint32_t arg : args)
                argsStr.Concatenate<false>(SPVArg(arg), " ");
            texts[(uint32_t)section].Append(TStr::Separated(op.str, SPVArg(type), argsStr));
            texts[(uint32_t)section].Append("\n");
        }
    }

    template<typename ...ARGS>
    void Instruction(const SPVOp& op, SPVWriter::Section section, const ARGS&... args)
    {
        this->section = section;
        SPVInstruction instr;
        instr.flags.wordCount = op.wordCount;
        instr.flags.code = op.c;

        if (op.dynamicWords)
        {
            uint32_t dynamicWordCount = 0;
            ([&]
            {
                dynamicWordCount += ArgCount(args);
            } (), ...);

            // Subtract all necessary words
            dynamicWordCount -= (op.wordCount - 1);
            instr.flags.wordCount += dynamicWordCount;
        }
        else
        {
            uint32_t totalArgs = 1;
            ([&] {totalArgs += ArgCount(args);} (), ...);
            assert(op.wordCount == totalArgs);            
        }
        assert(instr.flags.wordCount >= op.wordCount);
        this->binaries[(uint32_t)section].push_back(instr.bits);
        
        if (this->outputText)
            this->texts[(uint32_t)section].Append(op.str);

        (Append(args), ...);

        if (this->outputText)
            this->texts[(uint32_t)section].Append("\n");
    }

    uint32_t Reserve() { return this->counter++; }
    
    template<typename ...ARGS>
    void Reserved(const SPVOp& op, SPVWriter::Section section, uint32_t id, const ARGS&... args)
    {
        this->section = section;

        SPVInstruction instr;
        instr.flags.wordCount = op.wordCount;
        instr.flags.code = op.c;

        if (op.dynamicWords)
        {
            uint32_t dynamicWordCount = 0;
            ([&]
            {
                dynamicWordCount += ArgCount(args);
            } (), ...);

            // Subtract all necessary words
            dynamicWordCount -= (op.wordCount - 2);
            instr.flags.wordCount += dynamicWordCount;
        }

        this->binaries[(uint32_t)section].push_back(instr.bits);
        this->binaries[(uint32_t)section].push_back(id);

        if (this->outputText)
            this->texts[(uint32_t)section].Append(TStr::Separated(SPVArg{id}, "=", op.str).ToString());

        (Append(args), ...);

        if (this->outputText)
            this->texts[(uint32_t)section].Append("\n");
    }

    template<typename ...ARGS>
    void ReservedType(const SPVOp& op, const TransientString& name, SPVWriter::Section section, uint32_t id, const ARGS&... args)
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
        this->scopeStack.front().symbols[name.ToString()] = { .sym = nullptr, .value = id };

        this->section = section;

        SPVInstruction instr;
        instr.flags.wordCount = op.wordCount;
        instr.flags.code = op.c;

        if (op.dynamicWords)
        {
            uint32_t dynamicWordCount = 0;
            ([&]
            {
                dynamicWordCount += ArgCount(args);
            } (), ...);

            // Subtract all necessary words
            dynamicWordCount -= (op.wordCount - 2);
            instr.flags.wordCount += dynamicWordCount;
        }

        this->binaries[(uint32_t)section].push_back(instr.bits);
        this->binaries[(uint32_t)section].push_back(id);

        if (this->outputText)
            this->texts[(uint32_t)section].Append(TStr::Separated(SPVArg{ id }, "=", op.str).ToString());

        (Append(args), ...);

        if (this->outputText)
            this->texts[(uint32_t)section].Append("\n");
    }

    template<typename ...ARGS>
    void LinkDefinedVariable(const TransientString& name, uint32_t id)
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
        this->scopeStack.front().symbols[name.ToString()] = { .sym = nullptr, .value = id };
    }

    void BeginFunction()
    {
        this->binaries[(uint32_t)SPVWriter::Section::Functions].insert(
            this->binaries[(uint32_t)SPVWriter::Section::Functions].end()
            , this->binaries[(uint32_t)SPVWriter::Section::VariableDeclarations].begin()
            , this->binaries[(uint32_t)SPVWriter::Section::VariableDeclarations].end()
        );
        this->binaries[(uint32_t)SPVWriter::Section::Functions].insert(
            this->binaries[(uint32_t)SPVWriter::Section::Functions].end()
            , this->binaries[(uint32_t)SPVWriter::Section::ParameterInitializations].begin()
            , this->binaries[(uint32_t)SPVWriter::Section::ParameterInitializations].end()
        );
        this->binaries[(uint32_t)SPVWriter::Section::VariableDeclarations].clear();
        this->binaries[(uint32_t)SPVWriter::Section::ParameterInitializations].clear();

        if (this->outputText)
        {
            this->texts[(uint32_t)SPVWriter::Section::Functions].Append(this->texts[(uint32_t)SPVWriter::Section::VariableDeclarations]);
            this->texts[(uint32_t)SPVWriter::Section::Functions].Append(this->texts[(uint32_t)SPVWriter::Section::ParameterInitializations]);
            this->texts[(uint32_t)SPVWriter::Section::VariableDeclarations].Clear();
            this->texts[(uint32_t)SPVWriter::Section::ParameterInitializations].Clear();
        }
    }

    void FinishFunction()
    {
        this->binaries[(uint32_t)SPVWriter::Section::Functions].insert(
            this->binaries[(uint32_t)SPVWriter::Section::Functions].end()
            , this->binaries[(uint32_t)SPVWriter::Section::LocalFunction].begin()
            , this->binaries[(uint32_t)SPVWriter::Section::LocalFunction].end()
        );
        this->binaries[(uint32_t)SPVWriter::Section::LocalFunction].clear();

        if (this->outputText)
        {
            this->texts[(uint32_t)SPVWriter::Section::Functions].Append(this->texts[(uint32_t)SPVWriter::Section::LocalFunction]);
            this->texts[(uint32_t)SPVWriter::Section::LocalFunction].Clear();
        }
    }

    template<typename T>
    void Append(const T& arg)
    {
        assert(false && "Should never enter");
    }

    template<>
    void Append(const SPVArg& arg)
    {
        this->binaries[(uint32_t)this->section].push_back(arg.arg);
        if (this->outputText)
        {
            char buf[64];
            int numWritten = snprintf(buf, 64, " %%%d", arg.arg);
            this->texts[(uint32_t)this->section].Append(buf, numWritten);
        }
    }
    
    template<>
    void Append(const char* const& str)
    {        
        size_t len = strlen(str) + 1;
        size_t lenInWords = std::ceil(len / 4.0f);
        uint32_t* strAsWords = (uint32_t*)str;

        for (size_t i = 0; i < lenInWords; i++)
        {
            this->binaries[(uint32_t)this->section].push_back(strAsWords[i]);
        }
        char* lastInt = (char*)&this->binaries[(uint32_t)this->section].back();
        lastInt[3] = '\0';
        
        if (this->outputText)
        {
            this->texts[(uint32_t)this->section].Append(TStr::Compact(" \"", str, "\"").ToString());
        }
    }

    template<int SIZE>
    void Append(const char (&str)[SIZE])
    {
        size_t len = SIZE;
        size_t lenInWords = std::ceil(len / 4.0f);
        uint32_t* strAsWords = (uint32_t*)str;
        for (size_t i = 0; i < lenInWords; i++)
        {
            this->binaries[(uint32_t)this->section].push_back(strAsWords[i]);
        }
        
        if (this->outputText)
        {
            this->texts[(uint32_t)this->section].Append(TStr::Compact(" \"", str, "\"").ToString());
        }
    }

    template<>
    void Append(const int32_t& arg)
    {    
        this->binaries[(uint32_t)this->section].push_back(*reinterpret_cast<const uint32_t*>(&arg));
        if (this->outputText)
        {
            char buf[64];
            int numWritten = snprintf(buf, 64, " %d", arg);
            this->texts[(uint32_t)this->section].Append(buf, numWritten);
        }
    }

    template<>
    void Append(const float& arg)
    {
        this->binaries[(uint32_t)this->section].push_back(*reinterpret_cast<const uint32_t*>(&arg));
        
        if (this->outputText)
        {
            char buf[64];
            int numWritten = snprintf(buf, 64, " %f", arg);
            this->texts[(uint32_t)this->section].Append(buf, numWritten);
        }
    }

    template<>
    void Append(const uint32_t& arg)
    {    
        this->binaries[(uint32_t)this->section].push_back(arg);
        
        if (this->outputText)
        {
            char buf[64];
            int numWritten = snprintf(buf, 64, " %u", arg);
            this->texts[(uint32_t)this->section].Append(buf, numWritten);
        }
    }

    template<>
    void Append(const int16_t& arg)
    {
        this->binaries[(uint32_t)this->section].push_back(arg);

        if (this->outputText)
        {
            char buf[16];
            int numWritten = snprintf(buf, 16, " %d", arg);
            this->texts[(uint32_t)this->section].Append(buf, numWritten);
        }
    }

    template<>
    void Append(const uint16_t& arg)
    {
        this->binaries[(uint32_t)this->section].push_back(arg);

        if (this->outputText)
        {
            char buf[16];
            int numWritten = snprintf(buf, 16, " %u", arg);
            this->texts[(uint32_t)this->section].Append(buf, numWritten);
        }
    }

    template<>
    void Append(const size_t& arg)
    {
        this->binaries[(uint32_t)this->section].push_back(arg);
        
        if (this->outputText)
        {
            char buf[64];
            int numWritten = snprintf(buf, 64, " %zu", arg);
            this->texts[(uint32_t)this->section].Append(buf, numWritten);
        }
    }

    template<>
    void Append(const char& arg)
    {
        this->binaries[(uint32_t)this->section].push_back(*reinterpret_cast<const uint32_t*>(&arg));
        if (this->outputText)
        {
            this->texts[(uint32_t)this->section].Append(arg);
        }
    }

    template<>
    void Append(const unsigned char& arg)
    {    
        this->binaries[(uint32_t)this->section].push_back((uint32_t)arg);
        if (this->outputText)
        {
            char buf[3];
            int numWritten = snprintf(buf, 3, " %hhu", arg);
            this->texts[(uint32_t)this->section].Append(buf, numWritten);
        }
    }

    template<>
    void Append(const SPVEnum& arg)
    {
        if (arg.c == 0xFFFF)
            return;
        this->binaries[(uint32_t)this->section].push_back(arg.c);
        if (this->outputText)
        {
            this->texts[(uint32_t)this->section].Append(" ");
            this->texts[(uint32_t)this->section].Append(arg.str);
        }
    }


    template<>
    void Append(const SPVLiteralList& arg)
    {
        for (uint8_t it = 0; it < arg.num; it++)
            this->binaries[(uint32_t)this->section].push_back(arg.vals[it]);
        if (this->outputText)
        {
            for (uint8_t it = 0; it < arg.num; it++)
            {
                this->texts[(uint32_t)this->section].Append(" ");
                this->texts[(uint32_t)this->section].Append(arg.vals[it]);
            }
        }
    }

    template<>
    void Append(const SPVResultList& arg)
    {
        for (uint8_t it = 0; it < arg.num; it++)
            this->binaries[(uint32_t)this->section].push_back(arg.vals[it].name);
        if (this->outputText)
        {
            for (uint8_t it = 0; it < arg.num; it++)
            {
                char buf[64];
                uint32_t numWritten = snprintf(buf, 64, " %%%d", arg.vals[it].name);
                this->texts[(uint32_t)this->section].Append(" ");
                this->texts[(uint32_t)this->section].Append(buf, numWritten);
            }
        }
    }

    template<>
    void Append(const SPVArgList& arg)
    {
        for (uint8_t it = 0; it < arg.num; it++)
            this->binaries[(uint32_t)this->section].push_back(arg.vals[it].arg);
        if (this->outputText)
        {
            for (uint8_t it = 0; it < arg.num; it++)
            {
                char buf[64];
                uint32_t numWritten = snprintf(buf, 64, " %%%d", arg.vals[it].arg);
                this->texts[(uint32_t)this->section].Append(" ");
                this->texts[(uint32_t)this->section].Append(buf, numWritten);
            }
        }
    }

    template<>
    void Append(const SPVCaseList& arg)
    {
        for (uint8_t it = 0; it < arg.num; it++)
        {
            this->binaries[(uint32_t)this->section].push_back(arg.labels[it]);
            this->binaries[(uint32_t)this->section].push_back(arg.branches[it].arg);
        }
        if (this->outputText)
        {
            for (uint8_t it = 0; it < arg.num; it++)
            {
                this->texts[(uint32_t)this->section].Append(" ");
                this->texts[(uint32_t)this->section].Append(arg.labels[it]);
                char buf[64];
                uint32_t numWritten = snprintf(buf, 64, " %%%d", arg.branches[it].arg);

                this->texts[(uint32_t)this->section].Append(" ");
                this->texts[(uint32_t)this->section].Append(buf, numWritten);
            }
        }
    }

    template<>
    void Append(const SPIRVResult& arg)
    {
        this->Append(SPVArg(arg.name));
    }

    template<>
    void Append(const SPVComment& arg)
    {
        if (this->outputText)
        {
            this->texts[(uint32_t)this->section].Append("\t\t\t; ");
            this->texts[(uint32_t)this->section].Append(arg.str);
        }
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

    template<typename T>
    uint32_t ArgCount(const T& arg)
    {
        return 1;
    };

    template<>
    uint32_t ArgCount(const SPVArgList& args)
    {
        return args.num;
    }

    template<>
    uint32_t ArgCount(const SPVResultList& args)
    {
        return args.num;
    }

    template<>
    uint32_t ArgCount(const SPVCaseList& args)
    {
        return args.num * 2;
    }

    template<>
    uint32_t ArgCount(const SPVLiteralList& args)
    {
        return args.num;
    }

    template<>
    uint32_t ArgCount(const std::vector<uint32_t>& args)
    {
        return args.size();
    }

    template<>
    uint32_t ArgCount(const std::vector<SPIRVResult>& args)
    {
        return args.size();
    }

    template<>
    uint32_t ArgCount(const SPVComment& args)
    {
        return 0;
    }

    template<>
    uint32_t ArgCount(const char* const& args)
    {
        return std::ceil((strlen(args) + 1) / (float)sizeof(uint32_t));
    }

    template<int SIZE>
    uint32_t ArgCount(const char (&args)[SIZE])
    {
        return std::ceil(SIZE / (float)sizeof(uint32_t));
    }

    GrowingString texts[(uint32_t)Section::NumSections];
    std::vector<uint32_t> binaries[(uint32_t)Section::NumSections];

    PinnedMap<const char*, uint32_t> imports = 0xFFF;
    PinnedSet<SPVEnum> capabilities = 0xFFF;
    PinnedSet<SPVEnum> extensions = 0xFFF;
    PinnedSet<std::string> decorations = 0xFFF;
    PinnedMap<std::string, uint32_t> strings = 0xFFFF;
};

template<typename ...ARGS>
uint32_t
AddType(SPIRVGenerator* gen, const TransientString& name, const SPVOp& op, const ARGS&... args)
{
    auto scope = gen->writer->scopeStack.rbegin();
    while (scope != gen->writer->scopeStack.rend())
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
    gen->writer->scopeStack.front().symbols[name.ToString()] = { .sym = nullptr, .value = ret, .type = SPIRVResult::Invalid() };
    gen->writer->section = SPVWriter::Section::Declarations;
    gen->writer->MappedInstruction(op, SPVWriter::Section::Declarations, std::forward<const ARGS&>(args)..., SPVComment{ .str = name.buf });
    return ret;
}

template<typename ...ARGS>
uint32_t
AddType(SPIRVGenerator* gen, const TransientString& name, const SPIRVResult& baseType, const SPVOp& op, const ARGS&... args)
{
    auto scope = gen->writer->scopeStack.rbegin();
    while (scope != gen->writer->scopeStack.rend())
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
    gen->writer->scopeStack.front().symbols[name.ToString()] = { .sym = nullptr, .value = ret, .type = baseType };
    gen->writer->section = SPVWriter::Section::Declarations;
    gen->writer->MappedInstruction(op, SPVWriter::Section::Declarations, std::forward<const ARGS&>(args)..., SPVComment{ .str = name.buf });
    return ret;
}

template<typename ...ARGS>
uint32_t 
AddSymbol(SPIRVGenerator* gen, const TransientString& name, SPVWriter::Section section, const SPVOp& op, uint32_t type, const ARGS&... args)
{
    assert(section == SPVWriter::Section::Declarations || section == SPVWriter::Section::LocalFunction || section == SPVWriter::Section::Functions || section == SPVWriter::Section::VariableDeclarations);
    uint32_t ret = gen->writer->counter;

    if (section == SPVWriter::Section::Declarations)
    {
        auto scope = gen->writer->scopeStack.rbegin();
        while (scope != gen->writer->scopeStack.rend())
        {
            auto it = scope->symbols.find(name.ToString());
            if (it != scope->symbols.end())
            {
                return it->second.value;
            }
            scope++;
        }
        gen->writer->scopeStack.front().symbols[name.ToString()] = { .sym = nullptr, .value = ret, .type = SPIRVResult::Invalid() };
    }
    else
    {
        auto& scope = gen->writer->scopeStack.back();
        auto it = scope.symbols.find(name.ToString());
        if (it != scope.symbols.end())
        {
            return it->second.value;
        }
        gen->writer->scopeStack.back().symbols[name.ToString()] = { .sym = nullptr, .value = ret, .type = SPIRVResult::Invalid() };
    }
    gen->writer->section = section;
    gen->writer->MappedInstruction(op, section, type, std::forward<const ARGS&>(args)..., SPVComment{ .str = name.buf });
    return ret;
}

template<typename ...ARGS>
uint32_t
AddSymbol(SPIRVGenerator* gen, const TransientString& name, SPVWriter::Section section, const SPVOp& op, const SPIRVResult& type, const ARGS&... args)
{
    assert(section == SPVWriter::Section::Declarations || section == SPVWriter::Section::LocalFunction || section == SPVWriter::Section::Functions || section == SPVWriter::Section::VariableDeclarations);
    uint32_t ret = gen->writer->counter;

    if (section == SPVWriter::Section::Declarations)
    {
        auto scope = gen->writer->scopeStack.rbegin();
        while (scope != gen->writer->scopeStack.rend())
        {
            auto it = scope->symbols.find(name.ToString());
            if (it != scope->symbols.end())
            {
                return it->second.value;
            }
            scope++;
        }
        gen->writer->scopeStack.front().symbols[name.ToString()] = { .sym = nullptr, .value = ret, .type = type };
    }
    else
    {
        auto& scope = gen->writer->scopeStack.back();
        auto it = scope.symbols.find(name.ToString());
        if (it != scope.symbols.end())
        {
            return it->second.value;
        }
        gen->writer->scopeStack.back().symbols[name.ToString()] = { .sym = nullptr, .value = ret, .type = type };
    }
    gen->writer->section = section;
    gen->writer->MappedInstruction(op, section, type.typeName, std::forward<const ARGS&>(args)..., SPVComment{ .str = name.buf });
    return ret;
}

SymbolAssignment
GetSymbol(SPIRVGenerator* gen, const TransientString& name)
{
    SymbolAssignment ret = { .sym = nullptr, .value = 0xFFFFFFFF };
    auto it = gen->writer->scopeStack.rbegin();
    while (it != gen->writer->scopeStack.rend())
    {
        auto sym = it->symbols.find(name.ToString());
        if (sym != it->symbols.end())
        {
            ret = sym->second;
            break;
        }
        it++;
    }
    return ret;
}


template<typename ...ARGS>
uint32_t 
MappedInstruction(SPIRVGenerator* gen, SPVWriter::Section section, const SPVOp& op, uint32_t type, const ARGS&... args)
{
    uint32_t ret = gen->writer->counter;
    gen->writer->section = section;
    gen->writer->MappedInstruction(op, section, type, std::forward<const ARGS&>(args)...);
    return ret;
}

template<typename ...ARGS>
uint32_t 
MappedInstruction(SPIRVGenerator* gen, SPVWriter::Section section, const SPVOp& op, const ARGS&... args)
{
    uint32_t ret = gen->writer->counter;
    gen->writer->section = section;
    gen->writer->MappedInstruction(op, section, std::forward<const ARGS&>(args)...);
    return ret;
}

template<typename ...ARGS>
void
Instruction(SPIRVGenerator* gen, SPVWriter::Section section, const SPVOp& op, uint32_t type, const ARGS&... args)
{
    uint32_t ret = gen->writer->counter;
    gen->writer->section = section;
    gen->writer->Instruction(op, section, type, std::forward<const ARGS&>(args)...);
}

template<typename ...ARGS>
void 
Instruction(SPIRVGenerator* gen, SPVWriter::Section section, const SPVOp& op, const ARGS&... args)
{
    uint32_t ret = gen->writer->counter;
    gen->writer->section = section;
    gen->writer->Instruction(op, section, std::forward<const ARGS&>(args)...);
}

template<>
void FragmentString<GPULang::SPVArg>(GPULang::SPVArg arg, char* buf, size_t size)
{
    buf[0] = '%';
    snprintf(buf + 1, size, "%d", arg.arg);
}

template<>
void FragmentString<GPULang::SPVEnum>(GPULang::SPVEnum arg, char* buf, size_t size)
{
    buf[0] = '%';
    memcpy(buf + 1, arg.str, const_len(arg.str));
}

template<>
void TransientString::Append<GPULang::SPVArg>(GPULang::SPVArg arg)
{
    this->buf[this->size] = '%';
    snprintf(this->buf + this->size + 1, this->capacity - this->size, "%d", arg.arg);
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

template<>
inline size_t FragmentSize<GPULang::SPVEnum>(GPULang::SPVEnum arg)
{
    return const_len(arg.str);
}

GPULang::SPVArg operator""_spv(unsigned long long arg)
{
    return GPULang::SPVArg{ (uint32_t)arg };
};


SPIRVResult GenerateVariableSPIRV(const Compiler* compiler, SPIRVGenerator* generator, Symbol* symbol, bool isShaderArgument, bool isGlobal);
SPIRVResult GenerateExpressionSPIRV(const Compiler* compiler, SPIRVGenerator* generator, Expression* expr);
bool GenerateStatementSPIRV(const Compiler* compiler, SPIRVGenerator* generator, Statement* stat);
SPIRVResult GenerateConstantSPIRV(const Compiler* compiler, SPIRVGenerator* generator, ConstantCreationInfo info, uint32_t vectorSize = 1);

using ImageFormatter = std::function<TransientString(uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format)>;
using ImageTypeGenerator = std::function<uint32_t(SPIRVGenerator* gen, const TStr& name, uint32_t type, uint32_t depthBits, uint32_t sampledBits, SPVEnum format)>;

static const std::unordered_map<TypeCode, std::tuple<ConstantString, ImageFormatter>> handleTable =
{
    { TypeCode::Texture1D, { "texture1D", [](uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format) { return TStr::Separated("OpTypeImage", SPVArg(type), "1D", depthBits, 0, 0, sampledBits, format); } } }
    , { TypeCode::Texture2D, { "texture2D", [](uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format) { return TStr::Separated("OpTypeImage", SPVArg(type), "2D", depthBits, 0, 0, sampledBits, format); } } }
    , { TypeCode::Texture2DMS, { "texture2DMS", [](uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format) { return TStr::Separated("OpTypeImage", SPVArg(type), "2D", depthBits, 0, 1, sampledBits, format); } } }
    , { TypeCode::Texture3D, { "texture3D", [](uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format) { return TStr::Separated("OpTypeImage", SPVArg(type), "3D", depthBits, 0, 0, sampledBits, format); } } }
    , { TypeCode::TextureCube, { "textureCube", [](uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format) { return TStr::Separated("OpTypeImage", SPVArg(type), "Cube", depthBits, 0, 0, sampledBits, format); } } }
    , { TypeCode::Texture1DArray, { "texture1DArray", [](uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format) { return TStr::Separated("OpTypeImage", SPVArg(type), "1D", depthBits, 1, 0, sampledBits, format); } } }
    , { TypeCode::Texture2DArray, { "texture2DArray", [](uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format) { return TStr::Separated("OpTypeImage", SPVArg(type), "2D", depthBits, 1, 0, sampledBits, format); } } }
    , { TypeCode::Texture2DMSArray, { "texture2DMSArray", [](uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format) { return TStr::Separated("OpTypeImage", SPVArg(type), "2D", depthBits, 1, 1, sampledBits, format); } } }
    , { TypeCode::Texture3DArray, { "texture3DArray", [](uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format) { return TStr::Separated("OpTypeImage", SPVArg(type), "3D", depthBits, 1, 0, sampledBits, format); } } }
    , { TypeCode::TextureCubeArray, { "textureCubeArray", [](uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format) { return TStr::Separated("OpTypeImage", SPVArg(type), "Cube", depthBits, 1, 0, sampledBits, format); } } }
    , { TypeCode::SampledTexture1D, { "textureSampled1D", [](uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format) { return TStr::Separated("OpTypeImage", SPVArg(type), "1D", depthBits, 0, 0, sampledBits, format); } } }
    , { TypeCode::SampledTexture2D, { "textureSampled2D", [](uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format) { return TStr::Separated("OpTypeImage", SPVArg(type), "2D", depthBits, 0, 0, sampledBits, format); } } }
    , { TypeCode::SampledTexture2DMS, { "textureSampled2DMS", [](uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format) { return TStr::Separated("OpTypeImage", SPVArg(type), "2D", depthBits, 0, 1, sampledBits, format); } } }
    , { TypeCode::SampledTexture3D, { "textureSampled3D", [](uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format) { return TStr::Separated("OpTypeImage", SPVArg(type), "3D", depthBits, 0, 0, sampledBits, format); } } }
    , { TypeCode::SampledTextureCube, { "textureSampledCube", [](uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format) { return TStr::Separated("OpTypeImage", SPVArg(type), "Cube", depthBits, 0, 0, sampledBits, format); } } }
    , { TypeCode::SampledTexture1DArray, { "textureSampled1DArray", [](uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format) { return TStr::Separated("OpTypeImage", SPVArg(type), "1D", depthBits, 1, 0, sampledBits, format); } } }
    , { TypeCode::SampledTexture2DArray, { "textureSampled2DArray", [](uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format) { return TStr::Separated("OpTypeImage", SPVArg(type), "2D", depthBits, 1, 0, sampledBits, format); } } }
    , { TypeCode::SampledTexture2DMSArray, { "textureSampled2DMSArray", [](uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format) { return TStr::Separated("OpTypeImage", SPVArg(type), "2D", depthBits, 1, 1, sampledBits, format); } } }
    , { TypeCode::SampledTexture3DArray, { "textureSampled3DArray", [](uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format) { return TStr::Separated("OpTypeImage", SPVArg(type), "3D", depthBits, 1, 0, sampledBits, format); } } }
    , { TypeCode::SampledTextureCubeArray, { "textureSampledCubeArray", [](uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format) { return TStr::Separated("OpTypeImage", SPVArg(type), "Cube", depthBits, 1, 0, sampledBits, format); } } }
    , { TypeCode::PixelCache, { "pixelCache", [](uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format) { return TStr::Separated("OpTypeImage", SPVArg(type), "SubpassData", 0, 0, 0, 2, "Unknown"); } } }
    , { TypeCode::PixelCacheMS, { "pixelCacheMS", [](uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format) { return TStr::Separated("OpTypeImage", SPVArg(type), "SubpassData", 0, 0, 1, 2, "Unknown"); } } }
    , { TypeCode::Sampler, { "sampler", [](uint32_t type, uint32_t depthBits, uint32_t sampledBits, const char* format) { return TStr("OpTypeSampler"); } } }
};

static const std::unordered_map<TypeCode, ImageTypeGenerator> generators =
{
    { TypeCode::Texture1D,          [](SPIRVGenerator* gen, const TStr& name, uint32_t type, uint32_t depthBits, uint32_t sampledBits, SPVEnum format) { return AddType(gen, name, OpTypeImage, SPVArg{ type }, ImageDimensions::_1D, depthBits, 0, 0, sampledBits, format); } }
    , { TypeCode::Texture2D,        [](SPIRVGenerator* gen, const TStr& name, uint32_t type, uint32_t depthBits, uint32_t sampledBits, SPVEnum format) { return AddType(gen, name, OpTypeImage, SPVArg{ type }, ImageDimensions::_2D, depthBits, 0, 0, sampledBits, format); } }
    , { TypeCode::Texture2DMS,      [](SPIRVGenerator* gen, const TStr& name, uint32_t type, uint32_t depthBits, uint32_t sampledBits, SPVEnum format) { return AddType(gen, name, OpTypeImage, SPVArg{ type }, ImageDimensions::_2D, depthBits, 0, 1, sampledBits, format); } }
    , { TypeCode::Texture3D,        [](SPIRVGenerator* gen, const TStr& name, uint32_t type, uint32_t depthBits, uint32_t sampledBits, SPVEnum format) { return AddType(gen, name, OpTypeImage, SPVArg{ type }, ImageDimensions::_3D, depthBits, 0, 0, sampledBits, format); } }
    , { TypeCode::TextureCube,      [](SPIRVGenerator* gen, const TStr& name, uint32_t type, uint32_t depthBits, uint32_t sampledBits, SPVEnum format) { return AddType(gen, name, OpTypeImage, SPVArg{ type }, ImageDimensions::Cube, depthBits, 0, 0, sampledBits, format); } }
    , { TypeCode::Texture1DArray,   [](SPIRVGenerator* gen, const TStr& name, uint32_t type, uint32_t depthBits, uint32_t sampledBits, SPVEnum format) { return AddType(gen, name, OpTypeImage, SPVArg{ type }, ImageDimensions::_1D, depthBits, 1, 0, sampledBits, format); } }
    , { TypeCode::Texture2DArray,   [](SPIRVGenerator* gen, const TStr& name, uint32_t type, uint32_t depthBits, uint32_t sampledBits, SPVEnum format) { return AddType(gen, name, OpTypeImage, SPVArg{ type }, ImageDimensions::_2D, depthBits, 1, 0, sampledBits, format); } }
    , { TypeCode::Texture2DMSArray, [](SPIRVGenerator* gen, const TStr& name, uint32_t type, uint32_t depthBits, uint32_t sampledBits, SPVEnum format) { return AddType(gen, name, OpTypeImage, SPVArg{ type }, ImageDimensions::_2D, depthBits, 1, 1, sampledBits, format); } }
    , { TypeCode::Texture3DArray,   [](SPIRVGenerator* gen, const TStr& name, uint32_t type, uint32_t depthBits, uint32_t sampledBits, SPVEnum format) { return AddType(gen, name, OpTypeImage, SPVArg{ type }, ImageDimensions::_3D, depthBits, 1, 0, sampledBits, format); } }
    , { TypeCode::TextureCubeArray, [](SPIRVGenerator* gen, const TStr& name, uint32_t type, uint32_t depthBits, uint32_t sampledBits, SPVEnum format) { return AddType(gen, name, OpTypeImage, SPVArg{ type }, ImageDimensions::Cube, depthBits, 1, 0, sampledBits, format); } }
    , { TypeCode::PixelCache,       [](SPIRVGenerator* gen, const TStr& name, uint32_t type, uint32_t depthBits, uint32_t sampledBits, SPVEnum format) { return AddType(gen, name, OpTypeImage, SPVArg{ type }, ImageDimensions::SubpassData, 0, 0, 0, 2, ImageFormats::Unknown); } }
    , { TypeCode::PixelCacheMS,     [](SPIRVGenerator* gen, const TStr& name, uint32_t type, uint32_t depthBits, uint32_t sampledBits, SPVEnum format) { return AddType(gen, name, OpTypeImage, SPVArg{ type }, ImageDimensions::SubpassData, 0, 0, 1, 2, ImageFormats::Unknown); } }
    , { TypeCode::Sampler,          [](SPIRVGenerator* gen, const TStr& name, uint32_t type, uint32_t depthBits, uint32_t sampledBits, SPVEnum format) { return AddType(gen, name, OpTypeSampler); } }
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

std::unordered_map<ImageFormat, std::tuple<ConstantString, ConstantString, SPVEnum, SPVEnum>> imageFormatToSpirvType =
{
    { ImageFormat::Rgba16,          { "Rgba16", "", ImageFormats::Rgba16, SPVEnum() }}
    , { ImageFormat::Rgb10_A2,      { "Rgb10A2", "StorageImageExtendedFormats", ImageFormats::Rgb10A2, Capabilities::StorageImageExtendedFormats } }
    , { ImageFormat::Rgba8,         { "Rgba8", "", ImageFormats::Rgba8, SPVEnum() } }
    , { ImageFormat::Rg16,          { "Rg16", "StorageImageExtendedFormats", ImageFormats::Rg16, Capabilities::StorageImageExtendedFormats } }
    , { ImageFormat::Rg8,           { "Rg8", "StorageImageExtendedFormats", ImageFormats::Rg8, Capabilities::StorageImageExtendedFormats } }
    , { ImageFormat::R16,           { "R16", "StorageImageExtendedFormats", ImageFormats::R16, Capabilities::StorageImageExtendedFormats } }
    , { ImageFormat::R8,            { "R8", "StorageImageExtendedFormats", ImageFormats::R8, Capabilities::StorageImageExtendedFormats } }
    , { ImageFormat::Rgba16_Snorm,  { "Rgba16Snorm", "StorageImageExtendedFormats", ImageFormats::Rgba16Snorm, Capabilities::StorageImageExtendedFormats } }
    , { ImageFormat::Rgba8_Snorm,   { "Rgba8Snorm", "", ImageFormats::Rgba8Snorm, SPVEnum() } }
    , { ImageFormat::Rg16_Snorm,    { "Rg16Snorm", "StorageImageExtendedFormats", ImageFormats::Rg16Snorm, Capabilities::StorageImageExtendedFormats } }
    , { ImageFormat::Rg8_Snorm,     { "Rg8Snorm", "StorageImageExtendedFormats", ImageFormats::Rg8Snorm, Capabilities::StorageImageExtendedFormats } }
    , { ImageFormat::R16_Snorm,     { "R16Snorm", "StorageImageExtendedFormats", ImageFormats::R16Snorm, Capabilities::StorageImageExtendedFormats } }
    , { ImageFormat::R8_Snorm,      { "R8Snorm", "StorageImageExtendedFormats", ImageFormats::R8Snorm, Capabilities::StorageImageExtendedFormats } }

    // float
    , { ImageFormat::Rgba32F,       { "Rgba32f", "", ImageFormats::Rgba32f, SPVEnum() } }
    , { ImageFormat::Rgba16F,       { "Rgba16f", "", ImageFormats::Rgba16f, SPVEnum() } }
    , { ImageFormat::Rg32F,         { "Rg32f", "StorageImageExtendedFormats", ImageFormats::Rg32f, Capabilities::StorageImageExtendedFormats } }
    , { ImageFormat::Rg16F,         { "Rg16f", "StorageImageExtendedFormats", ImageFormats::Rg16f, Capabilities::StorageImageExtendedFormats } }
    , { ImageFormat::R11G11B10F,    { "R11fG11fB10f", "StorageImageExtendedFormats", ImageFormats::R11fG11fB10f, Capabilities::StorageImageExtendedFormats } }
    , { ImageFormat::R32F,          { "R32f", "", ImageFormats::R32f, SPVEnum() } }
    , { ImageFormat::R16F,          { "R16f", "StorageImageExtendedFormats", ImageFormats::R16f, Capabilities::StorageImageExtendedFormats } }

    // integer
    , { ImageFormat::Rgba32I,       { "Rgba32i", "", ImageFormats::Rgba32i, SPVEnum() } }
    , { ImageFormat::Rgba16I,       { "Rgba16i", "", ImageFormats::Rgba16i, SPVEnum() } }
    , { ImageFormat::Rgba8I,        { "Rgba8i", "", ImageFormats::Rgba8i, SPVEnum() } }
    , { ImageFormat::Rg32I,         { "Rg32i", "StorageImageExtendedFormats", ImageFormats::Rg32i, Capabilities::StorageImageExtendedFormats } }
    , { ImageFormat::Rg16I,         { "Rg16i", "StorageImageExtendedFormats", ImageFormats::Rg16i, Capabilities::StorageImageExtendedFormats } }
    , { ImageFormat::Rg8I,          { "Rg8i", "StorageImageExtendedFormats", ImageFormats::Rg8i, Capabilities::StorageImageExtendedFormats } }
    , { ImageFormat::R32I,          { "R32i", "", ImageFormats::R32i, SPVEnum() } }
    , { ImageFormat::R16I,          { "R16i", "StorageImageExtendedFormats", ImageFormats::R16i, Capabilities::StorageImageExtendedFormats } }
    , { ImageFormat::R8I,           { "R8i", "StorageImageExtendedFormats", ImageFormats::R8i, Capabilities::StorageImageExtendedFormats } }

    // unsigned integer
    , { ImageFormat::Rgba32U,       { "Rgba32ui", "", ImageFormats::Rgba32ui, SPVEnum() } }
    , { ImageFormat::Rgba16U,       { "Rgba16ui", "", ImageFormats::Rgba16ui, SPVEnum() } }
    , { ImageFormat::Rgb10_A2U,     { "Rgb10a2ui", "StorageImageExtendedFormats", ImageFormats::Rgb10a2ui, Capabilities::StorageImageExtendedFormats } }
    , { ImageFormat::Rgba8U,        { "Rgba8ui", "", ImageFormats::Rgba8ui, SPVEnum() } }
    , { ImageFormat::Rg32U,         { "Rg32ui", "StorageImageExtendedFormats", ImageFormats::Rg32ui, Capabilities::StorageImageExtendedFormats } }
    , { ImageFormat::Rg16U,         { "Rg16ui", "StorageImageExtendedFormats", ImageFormats::Rg16ui, Capabilities::StorageImageExtendedFormats } }
    , { ImageFormat::Rg8U,          { "Rg8ui", "StorageImageExtendedFormats", ImageFormats::Rg8ui, Capabilities::StorageImageExtendedFormats } }
    , { ImageFormat::R32U,          { "R32ui", "", ImageFormats::R32ui, SPVEnum() } }
    , { ImageFormat::R16U,          { "R16ui", "StorageImageExtendedFormats", ImageFormats::R16ui, Capabilities::StorageImageExtendedFormats } }
    , { ImageFormat::R8U,           { "R8ui", "StorageImageExtendedFormats", ImageFormats::R8ui, Capabilities::StorageImageExtendedFormats } }
    , { ImageFormat::R64U,          { "R64ui", "Int64ImageEXT", ImageFormats::R64ui, Capabilities::Int64ImageEXT } }
    , { ImageFormat::R64I,          { "R64i", "Int64ImageEXT", ImageFormats::R64i, Capabilities::Int64ImageEXT } }

    , { ImageFormat::Unknown,       { "Unknown", "", ImageFormats::Unknown, SPVEnum() } }
};


//------------------------------------------------------------------------------
/**
*/
uint32_t
GeneratePODTypeSPIRV(const Compiler* compiler, SPIRVGenerator* generator, TypeCode code, uint32_t vectorSize = 1, uint32_t rowSize = 1)
{
    auto it = scalarTable.find(code);
    assert(it != scalarTable.end());
    auto [gpuLangType, spirvType, op, args] = it->second;
    uint32_t baseType = AddType(generator, gpuLangType, op, args);
    
    // Matrix
    if (rowSize > 1)
    {
        assert(vectorSize > 1);
        uint32_t vecTypeArg = AddType(generator, TStr(gpuLangType, "x", rowSize), OpTypeVector, SPVArg{ baseType }, rowSize);
        baseType = AddType(generator, TStr(gpuLangType, "x", rowSize, "x", vectorSize), OpTypeMatrix, SPVArg{ vecTypeArg }, vectorSize);
    }
    else
    {
        // Vector
        if (vectorSize > 1)
        {
            baseType = AddType(generator, TStr(gpuLangType, "x", vectorSize), OpTypeVector, SPVArg{ baseType }, vectorSize);
        }
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
    uint32_t baseType = AddType(generator, str, op, args);
    TStr type = std::get<0>(it->second);
    
    // Matrix
    if (rowSize > 1)
    {
        assert(vectorSize > 1);
        TStr vecType = TStr(type, "x", rowSize);
        TStr matType = TStr(vecType, "x", vectorSize);
        uint32_t vecTypeArg = AddType(generator, vecType, OpTypeVector, SPVArg{baseType}, rowSize);
        baseType = AddType(generator, matType, OpTypeMatrix, SPVArg{vecTypeArg}, vectorSize);
        type = matType;
    }
    else if (vectorSize > 1)
    {
        // Vector
        TStr vecType = TStr(type, "x", vectorSize);
        baseType = AddType(generator, vecType, OpTypeVector, SPVArg{baseType}, vectorSize);
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

        if (typeSymbol->baseType == TypeCode::Texture1D || typeSymbol->baseType == TypeCode::Texture1DArray
            || typeSymbol->baseType == TypeCode::SampledTexture1D || typeSymbol->baseType == TypeCode::SampledTexture1DArray)
        {
            generator->writer->Capability(Capabilities::Sampled1D);
        }

        auto [spirvFormat, extension, format, cap] = imageFormatToSpirvType[type.imageFormat];
        TStr gpuLangStr = TStr::Compact(spirvFormat, "_", gpulangType, "Sample_", sampleBits, "Depth_", depthBits);
        if (extension.size > 0)
        {   
            generator->writer->Capability(cap);
        }
        auto handleGenerator = generators.find(typeSymbol->baseType);
        uint32_t name = handleGenerator->second(generator, gpuLangStr, floatType, depthBits, sampleBits, format);

        TStr ty = spirvFormatter(floatType, depthBits, sampleBits, spirvFormat.buf);
        baseType = std::tie(name, gpuLangStr);
    }
    else if (typeSymbol->category == Type::PixelCacheCategory)
    {
        generator->writer->Capability(Capabilities::InputAttachment);
        auto [floatType, floatBaseType] = GenerateBaseTypeSPIRV(compiler, generator, TypeCode::Float, 1);

        auto handleGenerator = generators.find(typeSymbol->baseType);
        uint32_t name = handleGenerator->second(generator, typeSymbol->name, floatType, 0, 0, ImageFormats::Unknown);

        auto handleTypeIt = handleTable.find(typeSymbol->baseType);
        auto [gpulangType, spirvFormatter] = handleTypeIt->second;
        
        TStr ty = spirvFormatter(floatType, 0, 0, nullptr);
        baseType = std::tie(name, gpulangType);
    }
    else if (typeSymbol->category == Type::SamplerCategory)
    {
        auto handleGenerator = generators.find(typeSymbol->baseType);
        uint32_t name = handleGenerator->second(generator, typeSymbol->name, 0, 0, 0, ImageFormats::Unknown);

        auto handleTypeIt = handleTable.find(typeSymbol->baseType);
        auto [gpulangType, spirvFormatter] = handleTypeIt->second;
        baseType = std::tie(name, gpulangType);
    }
    else if (typeSymbol->category == Type::EnumCategory)
    {
        baseType = GenerateBaseTypeSPIRV(compiler, generator, TypeCode::UInt, 1);
    }
    else if (typeSymbol->category == Type::UserTypeCategory)
    {
        uint32_t name = GetSymbol(generator, typeSymbol->name).value;
        baseType = std::tie(name, typeSymbol->name);
    }

    bool isStructPadded = false;

    ConstantString scopeString = SPIRVResult::ScopeToString(storage);
    SPVEnum scopeEnum = ScopeToEnum(storage);
    for (size_t i = 0; i < type.modifiers.size; i++)
    {
        auto [typeName, gpulangType] = baseType; 
        const Type::FullType::Modifier& mod = type.modifiers[i];
        if (mod == Type::FullType::Modifier::Pointer)
        {
            TStr newBase = TStr("ptr_", gpulangType, "_", scopeString);

            parentType.push_back(typeName);
            typeName = AddType(generator, newBase, OpTypePointer, scopeEnum, SPVArg{ typeName });
            baseType = std::tie(typeName, newBase);
        }
        else if (mod == Type::FullType::Modifier::Array)
        {
            if (type.modifierValues[i] == nullptr)
            {
                TStr newBase = TStr("[]_", gpulangType, scopeString);
                parentType.push_back(typeName);
                typeName = AddType(generator, newBase, OpTypeRuntimeArray, SPVArg{ typeName });
                if (typeSymbol->category == Type::UserTypeCategory)
                {
                    Structure::__Resolved* strucRes = Symbol::Resolved(static_cast<Structure*>(typeSymbol));
                    generator->writer->Decorate(SPVArg(typeName), Decorations::ArrayStride, strucRes->byteSize);
                }
                else
                {
                    generator->writer->Decorate(SPVArg(typeName), Decorations::ArrayStride, typeSymbol->CalculateStride());
                }
                newBase = TStr::Compact("struct_", newBase);
                typeName = AddType(generator, newBase, OpTypeStruct, SPVArg{ typeName });
                baseType = std::tie(typeName, newBase);

                generator->writer->MemberDecorate(SPVArg(typeName), 0, Decorations::Offset, 0);
                storage = SPIRVResult::Storage::StorageBuffer;
                scopeString = SPIRVResult::ScopeToString(storage);
                scopeEnum = ScopeToEnum(storage);
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

                SPIRVResult arraySizeConstant = GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::Int(size));
                typeName = AddType(generator, newBase, OpTypeArray, SPVArg{ typeName }, arraySizeConstant);
                baseType = std::tie(typeName, newBase);
                if (typeSymbol->category == Type::UserTypeCategory)
                {
                    Structure::__Resolved* strucRes = Symbol::Resolved(static_cast<Structure*>(typeSymbol));
                    generator->writer->Decorate(SPVArg(typeName), Decorations::ArrayStride, strucRes->byteSize);
                }
                else
                {
                    generator->writer->Decorate(SPVArg(typeName), Decorations::ArrayStride, typeSymbol->CalculateStride());
                }

                // if this is an interface, wrap it in a struct to allow interface binding
                if (isInterface)
                {
                    newBase = TStr("struct_", newBase);
                    typeName = AddType(generator, newBase, OpTypeStruct, SPVArg{ typeName });
                    baseType = std::tie(typeName, newBase);

                    generator->writer->MemberDecorate(SPVArg(typeName), 0, Decorations::Offset, 0);
                    isStructPadded = true;
                }
            }
        }
    }

    auto ret = SPIRVResult(0xFFFFFFFF, std::get<0>(baseType), false, false, storage, parentType);
    ret.isStructPadded = isStructPadded;
    return ret;
}

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
GenerateConstantSPIRV(const Compiler* compiler, SPIRVGenerator* generator, ConstantCreationInfo info, uint32_t vectorSize)
{
    SPIRVResult res = SPIRVResult::Invalid();
    uint32_t baseType;
    uint32_t vecType = 0xFFFFFFFF;

#define ADD_CONSTANT(short, ty) \
if (generator->linkDefineEvaluation)\
{\
    TStr symbolName = TStr(info.data.ty, "_", #short"_link-defined", generator->linkDefineSlot);\
    res.name = AddSymbol(generator, symbolName, SPVWriter::Section::Declarations, OpSpecConstant, baseType, info.data.ty);\
}\
else\
{\
    TStr symbolName = TStr(info.data.ty, "_", #short);\
    res.name = AddSymbol(generator, symbolName, SPVWriter::Section::Declarations, OpConstant, baseType, info.data.ty);\
}
    

    switch (info.type)
    {
        case ConstantCreationInfo::Type::UInt32:
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
        case ConstantCreationInfo::Type::Int32:
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
        case ConstantCreationInfo::Type::Float32:
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
        case ConstantCreationInfo::Type::Bool8:
        {
            baseType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::Bool, 1);
            if (vectorSize > 1)
                vecType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::Bool, vectorSize);
            res.typeName = baseType;
            if (generator->linkDefineEvaluation)
            {
                TStr symbolName = TStr::Compact(info.data.b ? "true" : "false", "_", "link_defined", generator->linkDefineSlot);
                res.name = AddSymbol(generator, symbolName, SPVWriter::Section::Declarations, info.data.b ? OpSpecConstantTrue : OpSpecConstantFalse, baseType);
            }
            else
            {
                TStr symbolName = TStr::Compact(info.data.b ? "true" : "false");
                res.name = AddSymbol(generator, symbolName, SPVWriter::Section::Declarations, info.data.b ? OpConstantTrue : OpConstantFalse, baseType);
            }
            break;
        }
    }
    if (vectorSize > 1)
    {
        TransientArray<SPVArg> vectorArgs(vectorSize);
        TStr name;
        for (int i = 0; i < vectorSize; i++)
        {
            vectorArgs.Append(SPVArg{ res.name });
            name.Append(SPVArg(res.name));
            if (i < vectorSize - 1)
                name.Append(" ");
        }
        if (generator->linkDefineEvaluation)
        {
            res.name = AddSymbol(generator, TStr("{", name, "}_link_defined"), SPVWriter::Section::Declarations, OpSpecConstantComposite, vecType, SPVArgList{ vectorArgs });
        }
        else
        {
            res.name = AddSymbol(generator, TStr("{", name, "}"), SPVWriter::Section::Declarations, OpConstantComposite, vecType, SPVArgList{ vectorArgs });
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
    {
        returnPtrType.modifiers = TransientArray<Type::FullType::Modifier>::Concatenate(returnPtrType.modifiers, Type::FullType::Modifier::Pointer);
        returnPtrType.modifierValues = TransientArray<Expression*>::Concatenate(returnPtrType.modifierValues, (Expression*)nullptr);
    }
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
        val = generator->writer->MappedInstruction(OpAccessChain, SPVWriter::Section::LocalFunction, type, SPVArg(val), chain);
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
        val = generator->writer->MappedInstruction(OpLoad, SPVWriter::Section::LocalFunction, type, SPVArg(val));
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
        val = generator->writer->MappedInstruction(OpLoad, SPVWriter::Section::LocalFunction, type, SPVArg(val));
    }
    
    assert(val != -1);
    if (arg.swizzleMask != Type::SwizzleMask())
    {
        // Can't swizzle a pointer
        uint32_t swizzleType = arg.swizzleType;
        if (Type::SwizzleMaskComponents(arg.swizzleMask) == 1)
        {
            val = generator->writer->MappedInstruction(OpCompositeExtract, SPVWriter::Section::LocalFunction, swizzleType, SPVArg(val), arg.swizzleMask.bits.x);
            type = swizzleType;
        }
        else
        {
            uint32_t indices[4];
            uint8_t written = 0;
            SwizzleMaskIndices(arg.swizzleMask, indices, written);
            val = generator->writer->MappedInstruction(OpVectorShuffle, SPVWriter::Section::LocalFunction, swizzleType, SPVArg(val), SPVArg(val), SPVLiteralList{ .vals = indices, .num = written });

            TStr swizzleIndices = SwizzleMaskToIndices(arg.swizzleMask);
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
        val = generator->writer->MappedInstruction(OpLoad, SPVWriter::Section::LocalFunction, type, SPVArg(val));
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
    }
    else
    {
        generator->writer->Instruction(OpCopyMemory, SPVWriter::Section::LocalFunction, SPVArg(val), SPVArg(source.name));
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
            uint32_t res = g->writer->MappedInstruction(OpConvertSToF, SPVWriter::Section::LocalFunction, type, value);
            return SPIRVResult(res, type, true);
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
            uint32_t res = g->writer->MappedInstruction(OpBitcast, SPVWriter::Section::LocalFunction, type, value);
            return SPIRVResult(res, type, true);
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
            uint32_t res = g->writer->MappedInstruction(OpINotEqual, SPVWriter::Section::LocalFunction, type, value, falseValue);
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
            uint32_t res = g->writer->MappedInstruction(OpBitcast, SPVWriter::Section::LocalFunction, type, value);
            return SPIRVResult(res, type, true);
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
            uint32_t res = g->writer->MappedInstruction(OpConvertUToF, SPVWriter::Section::LocalFunction, type, value);
            return SPIRVResult(res, type, true);
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
            uint32_t res = g->writer->MappedInstruction(OpINotEqual, SPVWriter::Section::LocalFunction, type, value, falseValue);
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
            uint32_t res = g->writer->MappedInstruction(OpConvertFToU, SPVWriter::Section::LocalFunction, type, value);
            return SPIRVResult(res, type, true);
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
            uint32_t res = g->writer->MappedInstruction(OpConvertFToS, SPVWriter::Section::LocalFunction, type, value);
            return SPIRVResult(res, type, true);
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
            uint32_t res = g->writer->MappedInstruction(OpSelect, SPVWriter::Section::LocalFunction, type, value, trueValue, falseValue);
            return SPIRVResult(res, type, true);
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
            uint32_t res = g->writer->MappedInstruction(OpSelect, SPVWriter::Section::LocalFunction, type, value, trueValue, falseValue);
            return SPIRVResult(res, type, true);
        }
    } }
};

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
    TransientArray<SPIRVResult> loadedArgs(args.size());
    for (const SPIRVResult& arg : args)
    {
        SPIRVResult loaded = LoadValueSPIRV(compiler, generator, arg);
        loadedArgs.Append(loaded);
        argList.Concatenate<false>(SPVArg(loaded.name), " ");
        if (!loaded.isConst)
            isConst = false;
        isSpecialization |= loaded.isSpecialization;
    }
    if (isConst)
    {
        if (isSpecialization)
        {
            uint32_t ret = AddSymbol(generator, TStr::Compact(SPVArg(returnType), "_", "composite_link_defined", "_", argList), SPVWriter::Section::Declarations, OpSpecConstantComposite, returnType, SPVResultList(loadedArgs));
            return SPIRVResult(ret, returnType, true, true);
        }
        else
        {
            uint32_t ret = AddSymbol(generator, TStr::Compact(SPVArg(returnType), "_", "composite", "_", argList), SPVWriter::Section::Declarations, OpConstantComposite, returnType, SPVResultList(loadedArgs));
            return SPIRVResult(ret, returnType, true, true);
        }
    }
    else
    {
        assert(!generator->linkDefineEvaluation);
        uint32_t res = generator->writer->MappedInstruction(OpCompositeConstruct, SPVWriter::Section::LocalFunction, returnType, SPVResultList(loadedArgs));
        return SPIRVResult(res, returnType, true);
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
GenerateCompositeExtractSPIRV(const Compiler* compiler, SPIRVGenerator* generator, uint32_t returnType, uint32_t arg, uint32_t index)
{
    uint32_t res = generator->writer->MappedInstruction(OpCompositeExtract, SPVWriter::Section::LocalFunction, returnType, SPVArg(arg), SPVArg(index));
    return SPIRVResult(res, returnType, true);
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateCompositeInsertSPIRV(const Compiler* compiler, SPIRVGenerator* generator, uint32_t returnType, uint32_t arg, uint32_t index, uint32_t val)
{
    uint32_t res = generator->writer->MappedInstruction(OpCompositeInsert, SPVWriter::Section::LocalFunction, returnType, SPVArg(arg), SPVArg(index));
    return SPIRVResult(res, returnType, true );
}

#include "generated/spirv.h"

//------------------------------------------------------------------------------
/**
*/
SPIRVGenerator::SPIRVGenerator()
{
    // Setup intrinsics
    this->mergeBlockCounter = 0;
    if (SPIRVGenerator::IntrinsicMap.empty())
        SetupIntrinsics();
    this->writer = Alloc<SPVWriter>();
}

std::unordered_map<Function*, SPIRVGenerator::FunctionToSPIRVMapping> SPIRVGenerator::IntrinsicMap;

//------------------------------------------------------------------------------
/**
*/
void
SPIRVGenerator::SetupIntrinsics()
{
   

    /*
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
        g->writer->Capability(Capabilities::RayTracingKHR);
        g->writer->Instruction(OpReportIntersectionKHR, SPVWriter::Section::LocalFunction);
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
        g->interfaceVariables.Insert(ret);
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
        g->interfaceVariables.Insert(ret);
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
        g->interfaceVariables.Insert(ret);
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
        g->interfaceVariables.Insert(ret);
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
        g->interfaceVariables.Insert(ret);
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
        g->interfaceVariables.Insert(ret);
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
        g->interfaceVariables.Insert(ret);
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
        g->interfaceVariables.Insert(ret);
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
        g->interfaceVariables.Insert(ret);
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
        g->interfaceVariables.Insert(ret);
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
        g->interfaceVariables.Insert(ret);
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
        g->interfaceVariables.Insert(ret);
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
        g->interfaceVariables.Insert(ret);
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
        g->interfaceVariables.Insert(ret);
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
        g->interfaceVariables.Insert(ret);
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
        g->interfaceVariables.Insert(ret);
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
        g->interfaceVariables.Insert(ret);
        g->AddDecoration("gplTLASWorldToObject", ret, "BuiltIn WorldToObjectKHR");
        SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);
        res.parentTypes.push_back(matType);
        return res;
    };
    */
}

//------------------------------------------------------------------------------
/**
*/
void
GenerateFunctionSPIRV(const Compiler* compiler, SPIRVGenerator* generator, Symbol* symbol)
{
    Function* func = static_cast<Function*>(symbol);
    Function::__Resolved* funcResolved = Symbol::Resolved(func);
    if (func != generator->entryPoint)
    {
        Function::__Resolved* entryRes = Symbol::Resolved(generator->entryPoint);
        if (entryRes->visibleSymbols.Find(func) == entryRes->visibleSymbols.end())
            return;
    }

    if (funcResolved->isEntryPoint && generator->entryPoint != func)
        return;

    if (funcResolved->isPrototype)
        return;
    SPIRVResult returnName = GenerateTypeSPIRV(compiler, generator, func->returnType, static_cast<Type*>(funcResolved->returnTypeSymbol));

    TStr typeArgs;
    TStr spvTypes;

    TransientArray<SPVArg> spvTypeArgs(func->parameters.size);
    for (auto param : func->parameters)
    {
        Variable::__Resolved* paramResolved = Symbol::Resolved(param);

        if (funcResolved->isEntryPoint)
        {
            GenerateVariableSPIRV(compiler, generator, param, true, true);
        }
        else
        {
            SPIRVResult::Storage storage = ResolveSPIRVVariableStorage(param->type, paramResolved->typeSymbol, paramResolved->storage, paramResolved->usageBits);
            SPIRVResult typeName = GenerateTypeSPIRV(compiler, generator, param->type, paramResolved->typeSymbol, storage);
            typeArgs.Append(param->type.ToString());
            if (param != func->parameters.back())
                typeArgs.Append(",");

            spvTypes.Append(SPVArg(typeName.typeName));
            spvTypes.Append(" ");
            spvTypeArgs.Append(SPVArg(typeName.typeName));
        }
    }
    
    TStr functionSymbolName = TStr::Compact("function_", typeArgs, "_", func->returnType.ToString());
    uint32_t functionType = AddType(generator, functionSymbolName, OpTypeFunction, SPVArg{ returnName.typeName }, SPVArgList(spvTypeArgs));
    
    // TODO: Add inline/const/functional
    AddSymbol(generator, funcResolved->name, SPVWriter::Section::Functions, OpFunction, returnName, FunctionControl::None, SPVArg{functionType});

    generator->writer->PushScope();
    if (!funcResolved->isEntryPoint)
    {
        for (auto& param : func->parameters)
        {
            Variable::__Resolved* paramResolved = Symbol::Resolved(param);
            SPIRVResult::Storage storage = ResolveSPIRVVariableStorage(param->type, paramResolved->typeSymbol, paramResolved->storage, paramResolved->usageBits);
            SPIRVResult varType = GenerateTypeSPIRV(compiler, generator, param->type, paramResolved->typeSymbol, storage);

            // If value is not a pointer, generate a copy of the value inside the function
            if (!param->type.IsPointer())
            {
                uint32_t paramName = MappedInstruction(generator, SPVWriter::Section::Functions, OpFunctionParameter, varType.typeName, SPVComment{param->name.c_str()});
                TransientString type = param->type.ToString();
                ConstantString scope = SPIRVResult::ScopeToString(varType.scope);

                TStr argPtrType = TStr::Compact("ptr_", type, "_", scope);
                uint32_t typePtrName = GPULang::AddType(generator, argPtrType, OpTypePointer, ScopeToEnum(varType.scope), SPVArg{varType.typeName});

                // Function parameters are values if they are not pointers
                varType.parentTypes.push_back(varType.typeName);
                varType.typeName = typePtrName;
                varType.isValue = false;
                uint32_t paramSymbol = AddSymbol(generator, param->name, SPVWriter::Section::VariableDeclarations, OpVariable, varType, VariableStorage::Function);
                generator->writer->Instruction(OpStore, SPVWriter::Section::ParameterInitializations, SPVArg{ paramSymbol }, SPVArg{ paramName });
            }
            else
            {
                // If value is already a pointer, then any stores to it in the function should be visible externally
                AddSymbol(generator, param->name, SPVWriter::Section::Functions, OpFunctionParameter, varType);
            }
        }
    }

    uint32_t label = generator->writer->MappedInstruction(OpLabel, SPVWriter::Section::Functions);
    generator->blockOpen = true;
    auto functionOverride = generator->evaluatingProgram->functionOverrides.Find(func);
    if (functionOverride != generator->evaluatingProgram->functionOverrides.end())
        GenerateStatementSPIRV(compiler, generator, functionOverride->second->ast);
    else
        GenerateStatementSPIRV(compiler, generator, func->ast);

    generator->writer->BeginFunction();
    if (generator->blockOpen)
    {
        if (!funcResolved->hasExplicitReturn)
        {
            generator->writer->Instruction(OpReturn, SPVWriter::Section::LocalFunction);
        }
        else
        {
            generator->writer->Instruction(OpUnreachable, SPVWriter::Section::LocalFunction);
        }
    }
    generator->blockOpen = false;
    generator->writer->Instruction(OpFunctionEnd, SPVWriter::Section::LocalFunction);

    generator->writer->FinishFunction();
    generator->writer->PopScope();
}

//------------------------------------------------------------------------------
/**
*/
uint32_t
GenerateStructureSPIRV(const Compiler* compiler, SPIRVGenerator* generator, Symbol* symbol)
{
    Structure* struc = static_cast<Structure*>(symbol);
    Structure::__Resolved* strucResolved = Symbol::Resolved(struc);

    Function::__Resolved* entryRes = Symbol::Resolved(generator->entryPoint);
    if (entryRes->visibleSymbols.Find(struc) == entryRes->visibleSymbols.end())
        return 0xFFFFFFFF;
    
    uint32_t numVariables = 0;
    for (Symbol* sym : struc->symbols)
    {
        if (sym->symbolType == Symbol::SymbolType::VariableType)
            numVariables++;
    }
    uint32_t name = 0xFFFFFFFF;
    uint32_t structName = generator->writer->Reserve();

    if (compiler->options.debugSymbols)
        generator->writer->Instruction(OpName, SPVWriter::Section::DebugNames, SPVArg{ structName }, struc->name.c_str());

    TransientArray<SPVArg> memberTypeArray(struc->symbols.size);
    uint32_t offset = 0;
    std::string memberTypes = "";
    for (size_t i = 0; i < struc->symbols.size; i++)
    {
        Symbol* sym = struc->symbols[i];
        if (sym->symbolType == Symbol::SymbolType::VariableType)
        {
            Variable* var = static_cast<Variable*>(sym);
            Variable::__Resolved* varResolved = Symbol::Resolved(var);

            if (compiler->options.debugSymbols)
                generator->writer->Instruction(OpMemberName, SPVWriter::Section::DebugNames, SPVArg{ structName }, i, var->name.c_str());
            
            SPIRVResult varType;
            if (varResolved->usageBits.flags.isPhysicalAddress)
            {
                // If physical address, generate a uint type
                varType.typeName = GeneratePODTypeSPIRV(compiler, generator, TypeCode::UInt);
            }
            else
            {
                varType = GenerateTypeSPIRV(compiler, generator, var->type, varResolved->typeSymbol);
            }
            memberTypes.append(Format("%%%d ", varType.typeName));
            memberTypeArray.Append(SPVArg{ varType.typeName });

            // If this struct is generated for binding 
            generator->writer->MemberDecorate(SPVArg{ structName }, i, Decorations::Offset, varResolved->structureOffset);
            if (varResolved->typeSymbol->IsMatrix())
            {
                generator->writer->MemberDecorate(SPVArg{ structName }, i, Decorations::MatrixStride, varResolved->typeSymbol->CalculateStride() / varResolved->typeSymbol->rowSize);
                generator->writer->MemberDecorate(SPVArg{ structName }, i, Decorations::ColMajor);
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
    
    generator->writer->ReservedType(OpTypeStruct, struc->name, SPVWriter::Section::Declarations, structName, SPVArgList(memberTypeArray), SPVComment{ .str = struc->name.c_str() });
    return structName;
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateSamplerSPIRV(const Compiler* compiler, SPIRVGenerator* generator, Symbol* symbol)
{
    SamplerStateInstance* sampler = static_cast<SamplerStateInstance*>(symbol);
    SamplerStateInstance::__Resolved* samplerResolved = Symbol::Resolved(sampler);
    
    Function::__Resolved* entryRes = Symbol::Resolved(generator->entryPoint);
    if (entryRes->visibleSymbols.Find(sampler) == entryRes->visibleSymbols.end())
        return SPIRVResult::Invalid();
    
    Type* samplerTypeSymbol = compiler->GetSymbol<Type>(ConstantString("sampler"));
    Type::FullType fullType = Type::FullType{ ConstantString("sampler") };
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

        static SPVEnum addressingTable[] =
        {
            SamplerAddressingModes::None,
            SamplerAddressingModes::Repeat,
            SamplerAddressingModes::RepeatMirrored,
            SamplerAddressingModes::ClampToEdge,
            SamplerAddressingModes::Clamp,
        };

        static SPVEnum filterTable[] =
        {
            SPVEnum(),
            SamplerFilterModes::Nearest,
            SamplerFilterModes::Linear
        };
        uint32_t name = AddSymbol(generator, sampler->name, SPVWriter::Section::Declarations, OpConstantSampler, samplerType.typeName, addressingTable[(uint32_t)samplerResolved->addressU], samplerResolved->unnormalizedSamplingEnabled ? 0 : 1, filterTable[(uint32_t)samplerResolved->magFilter]);

        if (compiler->options.debugSymbols)
            generator->writer->Instruction(OpName, SPVWriter::Section::DebugNames, SPVArg{ name }, sampler->name.c_str());
        
        return SPIRVResult(name, samplerType.typeName, true, true);
    }
    else
    {
        // Generate immutable sampler
        uint32_t name = AddSymbol(generator, symbol->name, SPVWriter::Section::Declarations, OpVariable, samplerType, ScopeToEnum(samplerType.scope));
        generator->interfaceVariables.Insert(name);

        if (compiler->options.debugSymbols)
            generator->writer->Instruction(OpName, SPVWriter::Section::DebugNames, SPVArg{ name }, sampler->name.c_str());

        generator->writer->Decorate(SPVArg(name), Decorations::DescriptorSet, samplerResolved->group);
        generator->writer->Decorate(SPVArg(name), Decorations::Binding, samplerResolved->binding);
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
    for (size_t i = 0; i < enumeration->globals.size; i++)
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
    for (size_t i = 0; i < enumeration->staticSymbols.size; i++)
    {
        Symbol* sym = enumeration->staticSymbols[i];
        if (sym->symbolType == Symbol::FunctionType)
        {
            Function* fun = static_cast<Function*>(sym);
            if (fun->name == "operator==")
            {
                generator->generatorIntrinsics[fun] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
                {
                    uint32_t ret = g->writer->MappedInstruction(OpIEqual, SPVWriter::Section::LocalFunction, returnType, args[0], args[1]);
                    return SPIRVResult(ret, returnType, true, true);
                };
            }
            else if (fun->name == "operator!=")
            {
                generator->generatorIntrinsics[fun] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
                {
                    uint32_t ret = g->writer->MappedInstruction(OpINotEqual, SPVWriter::Section::LocalFunction, returnType, args[0], args[1]);
                    return SPIRVResult(ret, returnType, true, true);
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

    Function::__Resolved* entryRes = Symbol::Resolved(generator->entryPoint);
    if (entryRes->visibleSymbols.Find(var) == entryRes->visibleSymbols.end())
        return SPIRVResult::Invalid();

    SPIRVResult::Storage storage = ResolveSPIRVVariableStorage(var->type, varResolved->typeSymbol, varResolved->storage, varResolved->usageBits);
    
    SPIRVResult typeName = GenerateTypeSPIRV(compiler, generator, var->type, varResolved->typeSymbol, storage, storage == SPIRVResult::Storage::Uniform || storage == SPIRVResult::Storage::StorageBuffer);
    //std::string type = varResolved->type.name;
    ConstantString scope = SPIRVResult::ScopeToString(typeName.scope);

    std::vector<uint32_t> parentTypes = { typeName.typeName };
    uint32_t name = 0xFFFFFFFF;

    SPIRVResult initializer = SPIRVResult::Invalid();

    Expression* initializerExpression = var->valueExpression;

    // override initializer if the variable is a gplIs variable
    if (varResolved->builtin && var->name.StartsWith("gplIs"))
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
                assert(varResolved->valueConversionFunction->backendIndex != UINT64_MAX && varResolved->valueConversionFunction->backendIndex < SPIRVDefaultIntrinsics.size());
                auto it = SPIRVDefaultIntrinsics[varResolved->valueConversionFunction->backendIndex];
                it(compiler, generator, typeName.typeName, { initializer });
            }
            else
                initializer = it->second(compiler, generator, typeName.typeName, { initializer });
        }

        generator->linkDefineEvaluation = false;
        generator->linkDefineSlot = UINT32_MAX;
    }

    uint32_t debugName;

    if (varResolved->storage != Storage::LinkDefined)
    {
        uint32_t typePtrName = typeName.typeName;
        
        // If anything but void, then the type has to be a pointer
        if (!var->type.IsPointer())
        {
            TStr ptrType = TStr("ptr_", var->type.ToString());
            typePtrName = AddType(generator, TStr::Compact(ptrType, "_", scope), typeName, OpTypePointer, ScopeToEnum(typeName.scope), SPVArg{ typeName.typeName });
            typeName.parentTypes.push_back(typeName.typeName);
            typeName.typeName = typePtrName;
        }
        
        if (initializer != SPIRVResult::Invalid())
        {
            // Only create the variable with the initializer if it's const and the variable is global
            if (initializer.isConst && isGlobal)
            {
                name = AddSymbol(generator, var->name, isGlobal ? SPVWriter::Section::Declarations : SPVWriter::Section::VariableDeclarations, OpVariable, typeName, ScopeToEnum(typeName.scope), SPVArg{ initializer.name });
                if (compiler->options.debugSymbols)
                    generator->writer->Instruction(OpName, SPVWriter::Section::DebugNames, SPVArg{ name }, var->name.c_str());
            }
            else
            {
                name = AddSymbol(generator, var->name, isGlobal ? SPVWriter::Section::Declarations : SPVWriter::Section::VariableDeclarations, OpVariable, typeName, ScopeToEnum(typeName.scope));
                if (compiler->options.debugSymbols)
                    generator->writer->Instruction(OpName, SPVWriter::Section::DebugNames, SPVArg{ name }, var->name.c_str());
                SPIRVResult loaded = LoadValueSPIRV(compiler, generator, initializer);
                generator->writer->Instruction(OpStore, SPVWriter::Section::LocalFunction, SPVArg(name), loaded);
            }
        }
        else
        {
            name = AddSymbol(generator, var->name, isGlobal ? SPVWriter::Section::Declarations : SPVWriter::Section::VariableDeclarations, OpVariable, typeName, ScopeToEnum(typeName.scope));
            if (compiler->options.debugSymbols)
                generator->writer->Instruction(OpName, SPVWriter::Section::DebugNames, SPVArg{ name }, var->name.c_str());
        }

        if (storage == SPIRVResult::Storage::StorageBuffer || storage == SPIRVResult::Storage::Uniform || storage == SPIRVResult::Storage::PushConstant || storage == SPIRVResult::Storage::UniformConstant || storage == SPIRVResult::Storage::Sampler)
        {
            uint32_t structSymbol = GetSymbol(generator, varResolved->typeSymbol->name).value;
            if (typeName.scope != SPIRVResult::Storage::Sampler && var->type.IsPointer())
            {
                generator->writer->Decorate(SPVArg(typeName.parentTypes.back()), Decorations::Block);
            }

            if (storage != SPIRVResult::Storage::PushConstant)
            {
                generator->writer->Decorate(SPVArg(name), Decorations::DescriptorSet, varResolved->group);
                generator->writer->Decorate(SPVArg(name), Decorations::Binding, varResolved->binding);
            }
        }
        else if (storage == SPIRVResult::Storage::Image || storage == SPIRVResult::Storage::MutableImage)
        {
            generator->writer->Decorate(SPVArg(name), Decorations::DescriptorSet, varResolved->group);
            generator->writer->Decorate(SPVArg(name), Decorations::Binding, varResolved->binding);
        }
        else if (storage == SPIRVResult::Storage::Input || storage == SPIRVResult::Storage::Output)
        {
            uint8_t binding = typeName.scope == SPIRVResult::Storage::Input ? varResolved->inBinding : varResolved->outBinding;
            generator->writer->Decorate(SPVArg(name), Decorations::Location, binding);
            generator->writer->Decorate(SPVArg(name), Decorations::Component, 0);
            if (varResolved->parameterBits.flags.isNoInterpolate)
            {
                generator->writer->Decorate(SPVArg(name), Decorations::Flat);
            }
            if (varResolved->parameterBits.flags.isNoPerspective)
            {
                generator->writer->Decorate(SPVArg(name), Decorations::NoPerspective);
            }
            if (varResolved->parameterBits.flags.isPatch)
            {
                generator->writer->Decorate(SPVArg(name), Decorations::Patch);
            }
            if (varResolved->parameterBits.flags.isCentroid)
            {
                generator->writer->Decorate(SPVArg(name), Decorations::Centroid);
            }
        }

        if (storage == SPIRVResult::Storage::StorageBuffer
            || storage == SPIRVResult::Storage::Image
            || storage == SPIRVResult::Storage::MutableImage
            || storage == SPIRVResult::Storage::Uniform
            || storage == SPIRVResult::Storage::UniformConstant
            || storage == SPIRVResult::Storage::PushConstant
            || storage == SPIRVResult::Storage::Private
            || storage == SPIRVResult::Storage::Sampler
            || storage == SPIRVResult::Storage::WorkGroup
            || storage == SPIRVResult::Storage::Input
            || storage == SPIRVResult::Storage::Output
            )
            generator->interfaceVariables.Insert(name);
        
        auto ret = SPIRVResult(name, typeName.typeName, false, false, typeName.scope, parentTypes);\
        ret.isStructPadded = typeName.isStructPadded;
        return ret;
    }
    else
    {
        // If it's a link defined variable, the constant generated by the initializer is sufficient
        generator->writer->LinkDefinedVariable(var->name, initializer.name);
        generator->writer->Decorate(SPVArg(initializer.name), Decorations::SpecId, varResolved->binding);

        if (compiler->options.debugSymbols)
            generator->writer->Instruction(OpName, SPVWriter::Section::DebugNames, initializer, var->name.c_str());

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
    const SymbolAssignment& sym = GetSymbol(generator, resolvedFunction->name);
    if (sym.value != 0xFFFFFFFF)
    {
        funName = sym.value;


        // If the conversions list is non empty, it means we have to convert every argument
        bool argumentsNeedsConversion = resolvedCall->conversions.size != 0;
        
        // Create arg list from argument expressions
        std::string argList = "";

        TransientArray<SPVArg> argListArray(callExpression->args.size);
        for (size_t i = 0; i < callExpression->args.size; i++)
        {
            // If the function calls for a literal argument, the generator needs to extract the value as a literal result
            if (resolvedCall->function->parameters.buf[i]->type.literal)
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
                        assert(conversion->backendIndex != UINT64_MAX && conversion->backendIndex < SPIRVDefaultIntrinsics.size());
                        auto it = SPIRVDefaultIntrinsics[conversion->backendIndex];
                        arg = it(compiler, generator, convertedReturn, { arg });
                    }
                    else
                        arg = it->second(compiler, generator, convertedReturn, { arg });
                }
            }

            argListArray.Append(SPVArg(arg.name));
            argList.append(Format("%%%d ", arg.name));
        }

        if (compiler->options.debugSymbols)
        {
            uint32_t name = generator->writer->String(expr->location.file.c_str());
            generator->writer->Instruction(OpLine, SPVWriter::Section::LocalFunction, SPVArg{name}, expr->location.line, expr->location.start);
        }

        // Then call the function
        uint32_t ret = generator->writer->MappedInstruction(OpFunctionCall, SPVWriter::Section::LocalFunction, returnTypeName.typeName, SPVArg(funName), SPVArgList(argListArray));
        return SPIRVResult(ret, returnTypeName.typeName, true);
    }
    else
    {
        // If there is no symbol matching this function, then we assume it's an intrinsic function
        std::vector<SPIRVResult> args;

        // Create arg list from argument expressions
        for (size_t i = 0; i < callExpression->args.size; i++)
        {
            Variable* var = resolvedCall->function->parameters.buf[i];
            Variable::__Resolved* varResolved = Symbol::Resolved(var);

            // If an argument is a literal, evalute it directly
            if (!var->type.literal)
            {
                if (resolvedCall->argumentTypes[i].literal)
                {
                    ValueUnion val;
                    callExpression->args[i]->EvalValue(val);
                    val.Convert(varResolved->typeSymbol->baseType);
                    val.Expand(varResolved->typeSymbol->columnSize, varResolved->typeSymbol->rowSize);
                    SPIRVResult literalResults[16] = { };
                    uint32_t resultsIterator = 0;
                    SPIRVResult typeName = GenerateTypeSPIRV(compiler, generator, var->type, varResolved->typeSymbol);

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
                        SPIRVResult returnTypeName = GenerateTypeSPIRV(compiler, generator, converter->returnType, converterResolved->returnTypeSymbol);
                        std::vector<SPIRVResult> tempArgs{ args[i] };
                        
                        auto it = generator->generatorIntrinsics.find(converter);
                        if (it == generator->generatorIntrinsics.end())
                        {
                            assert(converter->backendIndex != UINT64_MAX && converter->backendIndex < SPIRVDefaultIntrinsics.size());
                            auto it = SPIRVDefaultIntrinsics[converter->backendIndex];
                            args[i] = it(compiler, generator, returnTypeName.typeName, tempArgs);
                        }
                        else

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

        if (compiler->options.debugSymbols)
        {
            uint32_t name = generator->writer->String(expr->location.file.c_str());
            generator->writer->Instruction(OpLine, SPVWriter::Section::LocalFunction, SPVArg{name}, expr->location.line, expr->location.start);
        }
        // Function is not declared by user code, so must be intrinsic
        auto it = generator->generatorIntrinsics.find(resolvedCall->function);
        if (it == generator->generatorIntrinsics.end())
        {
            assert(resolvedCall->function->backendIndex != UINT64_MAX && resolvedCall->function->backendIndex < SPIRVDefaultIntrinsics.size());
            auto it = SPIRVDefaultIntrinsics[resolvedCall->function->backendIndex];
            return it(compiler, generator, returnTypeName.typeName, args);
        }
        else
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

    if (compiler->options.debugSymbols)
    {
        uint32_t name = generator->writer->String(expr->location.file.c_str());
        generator->writer->Instruction(OpLine, SPVWriter::Section::LocalFunction, SPVArg{name}, expr->location.line, expr->location.start);
    }
    if (leftType.modifiers.size == 0)
    {
        Type* type = static_cast<Type*>(compiler->GetSymbol(leftType.name));

        if (type->IsVector() || type->IsMatrix())
        {
            auto op = TransientString("operator[](", rightType.name, ")");
            auto it = type->scope.symbolLookup.Find(op);
            Function* func = static_cast<Function*>((*it).second);

            SPIRVResult res;
            /// Get intrinsic
            auto intrinsicIt = generator->generatorIntrinsics.find(func);
            if (intrinsicIt == generator->generatorIntrinsics.end())
            {
                assert(func->backendIndex != UINT64_MAX && func->backendIndex < SPIRVDefaultIntrinsics.size());
                auto it = SPIRVDefaultIntrinsics[func->backendIndex];
                res = it(compiler, generator, returnType.typeName, { leftSymbol, indexConstant });
            }
            else
                res = intrinsicIt->second(compiler, generator, returnType.typeName, { leftSymbol, indexConstant });

            /// Generate array access
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
    uint32_t strucType = GetSymbol(generator, initResolved->type->name).value;

    std::vector<SPIRVResult> values;
    bool isConst = true;
    bool isLinkDefined = false;

    TransientArray<SPVArg> argList(initExpression->values.size);

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
        argList.Append(SPVArg(value.name));
    }
    
    if (compiler->options.debugSymbols)
    {
        uint32_t name = generator->writer->String(expr->location.file.c_str());
        generator->writer->Instruction(OpLine, SPVWriter::Section::LocalFunction, SPVArg{name}, expr->location.line, expr->location.start);
    }
    if (isConst)
    {
        if (isLinkDefined)
        {
            name = AddSymbol(generator, TStr::Compact("{", initializer, "}"), SPVWriter::Section::Declarations, OpSpecConstantComposite, strucType, SPVArgList(argList));
        }
        else
        {
            name = AddSymbol(generator, TStr::Compact("{", initializer, "}"), SPVWriter::Section::Declarations, OpConstantComposite, strucType, SPVArgList(argList));
        }
    }
    else
    {
        assert(!generator->linkDefineEvaluation);
        name = AddSymbol(generator, TStr::Compact("{", initializer, "}"), SPVWriter::Section::LocalFunction, OpCompositeConstruct, strucType, SPVArgList(argList));
    }
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

    TransientArray<SPVArg> argList(sym->values.size);
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
        argList.Append(SPVArg(res.name));
    }

    if (compiler->options.debugSymbols)
    {
        uint32_t name = generator->writer->String(expr->location.file.c_str());
        generator->writer->Instruction(OpLine, SPVWriter::Section::LocalFunction, SPVArg{name}, expr->location.line, expr->location.start);
    }
    if (isConst)
    {
        if (isLinkDefined)
        {
            name = AddSymbol(generator, TStr::Compact("{", initializer, "}"), SPVWriter::Section::Declarations, OpSpecConstantComposite, type.typeName, SPVArgList(argList));
        }
        else
        {
            name = AddSymbol(generator, TStr::Compact("{", initializer, "}"), SPVWriter::Section::Declarations, OpConstantComposite, type.typeName, SPVArgList(argList));
        }
    }
    else
    {
        assert(!generator->linkDefineEvaluation);
        name = AddSymbol(generator, TStr::Compact("{", initializer, "}"), SPVWriter::Section::LocalFunction, OpCompositeConstruct, type.typeName, SPVArgList(argList));
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

    if (compiler->options.debugSymbols)
    {
        uint32_t name = generator->writer->String(expr->location.file.c_str());
        generator->writer->Instruction(OpLine, SPVWriter::Section::LocalFunction, SPVArg{name}, expr->location.line, expr->location.start);
    }

    // If there is a conversion function, generate it first
    if (binaryExpressionResolved->rightConversion)
    {
        Function::__Resolved* rightConvResolved = Symbol::Resolved(binaryExpressionResolved->rightConversion);
        SPIRVResult rightConvType = GenerateTypeSPIRV(compiler, generator, binaryExpressionResolved->rightConversion->returnType, rightConvResolved->returnTypeSymbol);
        auto it = generator->generatorIntrinsics.find(binaryExpressionResolved->rightConversion);
        if (it == generator->generatorIntrinsics.end())
        {
            assert(binaryExpressionResolved->rightConversion->backendIndex != UINT64_MAX && binaryExpressionResolved->rightConversion->backendIndex < SPIRVDefaultIntrinsics.size());
            auto it = SPIRVDefaultIntrinsics[binaryExpressionResolved->rightConversion->backendIndex];
            rightValue = it(compiler, generator, rightConvType.typeName, { rightValue });
        }
        else
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
            assert(binaryExpressionResolved->leftConversion->backendIndex != UINT64_MAX && binaryExpressionResolved->leftConversion->backendIndex < SPIRVDefaultIntrinsics.size());
            auto it = SPIRVDefaultIntrinsics[binaryExpressionResolved->leftConversion->backendIndex];
            leftValue = it(compiler, generator, leftConvType.typeName, { leftValue });
        }
        else
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
            assert(fun->backendIndex != UINT64_MAX && fun->backendIndex < SPIRVDefaultIntrinsics.size());
            auto it = SPIRVDefaultIntrinsics[fun->backendIndex];
            binaryOpResult = it(compiler, generator, retType.typeName, { leftValue, rightValue });
        }
        else
            binaryOpResult = it->second(compiler, generator, retType.typeName, { leftValue, rightValue });
    }

    if (binaryExpressionResolved->isAssignment)
    {
        assert(!leftValue.isValue);

        // If left value has a swizzle mask, then swizzle the RIGHT hand value before storing
        if (leftValue.swizzleMask != Type::SwizzleMask())
        {
            Type* vectorType;
            binaryExpression->left->EvalUnswizzledTypeSymbol(vectorType);
            
            // Get the unswizzled type and generate a SPIRV type for it.
            // Storing to a swizzle value requires an OpStore of the same size, so we are going to shuffle
            // the vector of the right hand side such that it has the same amount of components and the left hand side
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
                rightValue.name = generator->writer->MappedInstruction(OpVectorShuffle, SPVWriter::Section::LocalFunction, vectorTypeName.typeName, leftLoaded, rightValue, SPVLiteralList{ .vals = slots, .num = (uint8_t)vectorType->columnSize });
                rightValue.typeName = vectorTypeName.typeName;
            }
            binaryOpResult = rightValue;

        }
        StoreValueSPIRV(compiler, generator, leftValue, binaryOpResult);
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

    if (compiler->options.debugSymbols)
    {
        uint32_t name = generator->writer->String(expr->location.file.c_str());
        generator->writer->Instruction(OpLine, SPVWriter::Section::LocalFunction, SPVArg{name}, expr->location.line, expr->location.start);
    }
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

        if (lhsType.modifiers.size > 0 && lhsType.modifiers.front() == Type::FullType::Modifier::Array)
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
            for (size_t i = 0; i < lhsSymbol->symbols.size; i++)
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
    
    if (compiler->options.debugSymbols)
    {
        uint32_t name = generator->writer->String(expr->location.file.c_str());
        generator->writer->Instruction(OpLine, SPVWriter::Section::LocalFunction, SPVArg{name}, expr->location.line, expr->location.start);
    }
    SPIRVResult loadedCondition = LoadValueSPIRV(compiler, generator, lhsResult);
    SPIRVResult loadedLeft = LoadValueSPIRV(compiler, generator, ifResult);
    SPIRVResult loadedRight = LoadValueSPIRV(compiler, generator, elseResult);
    uint32_t ret = generator->writer->MappedInstruction(OpSelect, SPVWriter::Section::LocalFunction, loadedLeft.typeName, loadedCondition, loadedLeft, loadedRight);

    SPIRVResult res = ifResult;
    res.typeName = ret;
    return res;
}

struct UnaryOperator
{
    char bit;
    bool sign;
    char elements;
    SPVOp addOp, subOp, negateOp;
    
    constexpr UnaryOperator()
        : bit('\0')
        , sign(false)
        , elements(0)
        , addOp(OpNop)
        , subOp(OpNop)
        , negateOp(OpNop)
    {}
    
    constexpr UnaryOperator(char bit, bool sign, int elements, SPVOp addOp, SPVOp subOp, SPVOp negateOp)
        : bit(bit)
        , sign(sign)
        , elements(elements)
        , addOp(addOp)
        , subOp(subOp)
        , negateOp(negateOp)
    {}
    
    UnaryOperator(const UnaryOperator& rhs)
    {
        memcpy(this, &rhs, sizeof(UnaryOperator));
    }
    
    void operator=(const UnaryOperator& rhs)
    {
        memcpy(this, &rhs, sizeof(UnaryOperator));
    }
};

// TODO: Add support for looking up SPVOps as well
static const StaticMap UnaryOperatorTable =
std::array{
    std::pair{ "f32"_c, UnaryOperator{ 'F', true, 1, OpFAdd, OpFSub, OpFNegate } }
    , std::pair{ "f32x2"_c, UnaryOperator{ 'F', true, 2, OpFAdd, OpFSub, OpFNegate } }
    , std::pair{ "f32x3"_c, UnaryOperator{ 'F', true, 3, OpFAdd, OpFSub, OpFNegate } }
    , std::pair{ "f32x4"_c, UnaryOperator{ 'F', true, 4, OpFAdd, OpFSub, OpFNegate } }
    , std::pair{ "i32"_c, UnaryOperator{ 'S', true, 1, OpIAdd, OpISub, OpSNegate } }
    , std::pair{ "i32x2"_c, UnaryOperator{ 'S', true, 2, OpIAdd, OpISub, OpSNegate } }
    , std::pair{ "i32x3"_c, UnaryOperator{ 'S', true, 3, OpIAdd, OpISub, OpSNegate } }
    , std::pair{ "i32x4"_c, UnaryOperator{ 'S', true, 4, OpIAdd, OpISub, OpSNegate } }
    , std::pair{ "u32"_c, UnaryOperator{ 'U', false, 1, OpIAdd, OpISub, OpNop } }
    , std::pair{ "u32x2"_c, UnaryOperator{ 'U', false, 2, OpIAdd, OpISub, OpNop } }
    , std::pair{ "u32x3"_c, UnaryOperator{ 'U', false, 3, OpIAdd, OpISub, OpNop } }
    , std::pair{ "u32x4"_c, UnaryOperator{ 'U', false, 4, OpIAdd, OpISub, OpNop } }
    , std::pair{ "b8"_c, UnaryOperator{ 'B', false, 1, OpNop, OpNop, OpNop } }
    , std::pair{ "b8x2"_c, UnaryOperator{ 'B', false, 2, OpNop, OpNop, OpNop } }
    , std::pair{ "b8x3"_c, UnaryOperator{ 'B', false, 3, OpNop, OpNop, OpNop } }
    , std::pair{ "b8x4"_c, UnaryOperator{ 'B', false, 4, OpNop, OpNop, OpNop } }
};

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateUnaryExpressionSPIRV(const Compiler* compiler, SPIRVGenerator* generator, Expression* expr)
{
    UnaryExpression* unaryExpression = static_cast<UnaryExpression*>(expr);
    UnaryExpression::__Resolved* unaryExpressionResolved = Symbol::Resolved(unaryExpression);

    SPIRVResult rhs = GenerateExpressionSPIRV(compiler, generator, unaryExpression->expr);

    if (compiler->options.debugSymbols)
    {
        uint32_t name = generator->writer->String(expr->location.file.c_str());
        generator->writer->Instruction(OpLine, SPVWriter::Section::LocalFunction, SPVArg{name}, expr->location.line, expr->location.start);
    }
    auto value = UnaryOperatorTable.Find(unaryExpressionResolved->fullType.name);
    auto [op, sign, vectorSize, addOp, subOp, negOp] = value->second;
    switch (unaryExpression->op)
    {
        case '++':
        {
            assert(value != UnaryOperatorTable.end());
            uint32_t vectorSize = value->second.elements;
            SPIRVResult constOne = SPIRVResult::Invalid();
            if (sign)
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
            uint32_t res = generator->writer->MappedInstruction(addOp, SPVWriter::Section::LocalFunction, loaded.typeName, loaded, constOne);
            generator->writer->Instruction(OpStore, SPVWriter::Section::LocalFunction, rhs, SPVArg{ res });
            if (unaryExpression->isPrefix)
            {
                return SPIRVResult(res, rhs.typeName, true);
            }
            else
            {
                return SPIRVResult(loaded.name, rhs.typeName, true);
            }
        }
        case '--':
        {
            assert(value != UnaryOperatorTable.end());
            uint32_t vectorSize = value->second.elements;
            SPIRVResult constOne = SPIRVResult::Invalid();
            if (sign)
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
            uint32_t res = generator->writer->MappedInstruction(subOp, SPVWriter::Section::LocalFunction, loaded.typeName, loaded);
            generator->writer->Instruction(OpStore, SPVWriter::Section::LocalFunction, rhs, SPVArg(res));
            if (unaryExpression->isPrefix)
            {
                return SPIRVResult(res, rhs.typeName, true);
            }
            else
            {
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
        case '&':
        {
            uint32_t name = AccessChainSPIRV(compiler, generator, rhs.name, rhs.typeName, rhs.accessChain);
            return SPIRVResult(name, rhs.typeName, false);
        }
        case '-':
        {
            assert(value != UnaryOperatorTable.end());
            const char op = value->second.bit;
            bool isSigned = value->second.sign;
            uint32_t vectorSize = value->second.elements;
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
                uint32_t res = generator->writer->MappedInstruction(negOp, SPVWriter::Section::LocalFunction, loaded.typeName, loaded);
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
            assert(value != UnaryOperatorTable.end());
            const char op = value->second.bit;
            bool isSigned = value->second.sign;
            uint32_t vectorSize = value->second.elements;
            
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
                uint32_t res = generator->writer->MappedInstruction(OpNot, SPVWriter::Section::LocalFunction, loaded.typeName, loaded);

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
    if (compiler->options.debugSymbols)
    {
        uint32_t name = generator->writer->String(expr->location.file.c_str());
        generator->writer->Instruction(OpLine, SPVWriter::Section::LocalFunction, SPVArg{name}, expr->location.line, expr->location.start);
    }
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
            for (uint32_t i = 0; i < floatVecExpr->values.size; i++)
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
            for (uint32_t i = 0; i < intVecExpr->values.size; i++)
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
            for (uint32_t i = 0; i < uintVecExpr->values.size; i++)
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
            for (uint32_t i = 0; i < boolVecExpr->values.size; i++)
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
                    const SymbolAssignment& sym = GetSymbol(generator, symbolExpression->symbol);
                    type = sym.type;
                    if (generator->accessChain.empty())
                    {
                        if (generator->literalExtract)
                        {
                            if (sym.sym->symbolType == Symbol::SymbolType::VariableType)
                            {
                                Variable* var = static_cast<Variable*>(sym.sym);
                                Variable::__Resolved* varRes = Symbol::Resolved(var);
                                if (var->type.literal && varRes->typeSymbol->columnSize == 1 && varRes->typeSymbol->rowSize == 1)
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
                        for (size_t i = 0; i < ty->symbols.size; i++)
                        {
                            if (ty->symbols[i]->name == symbolExpression->symbol)
                            {
                                Variable* var = static_cast<Variable*>(ty->symbols[i]);
                                Variable::__Resolved* varRes = Symbol::Resolved(var);
                                SPIRVResult index = GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::UInt(i));

                                // If part of an access chain, generate a pointer version of the type
                                res = GeneratePointerTypeSPIRV(compiler, generator, var->type, varRes->typeSymbol, accessStorage);
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
                return SPIRVResult(GetSymbol(generator, symbolExpression->symbol).value, type.typeName, false, false, type.scope, type.parentTypes);
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
    
    uint32_t startLabel = generator->writer->Reserve();
    uint32_t conditionLabel = generator->writer->Reserve();
    uint32_t repeatLabel = generator->writer->Reserve();
    uint32_t bodyLabel = generator->writer->Reserve();
    uint32_t endLabel = generator->writer->Reserve();

    generator->mergeBlocks[generator->mergeBlockCounter++] =
    SPIRVGenerator::MergeBlock
    {
        repeatLabel, endLabel
    };

    // Initial label to start the loop
    generator->writer->Instruction(OpBranch, SPVWriter::Section::LocalFunction, SPVArg{startLabel});
    generator->writer->Reserved(OpLabel, SPVWriter::Section::LocalFunction, startLabel);

    std::string unroll = "None";
    if (stat->unrollCount == UINT_MAX)
        unroll = "Unroll";
    else if (stat->unrollCount > 0)
        unroll = Format("PartialCount %d", stat->unrollCount);

    if (compiler->options.debugSymbols)
    {
        uint32_t name = generator->writer->String(stat->location.file.c_str());
        generator->writer->Instruction(OpLine, SPVWriter::Section::LocalFunction, SPVArg{name}, stat->location.line, stat->location.start);
    }

    // TODO: support unrolling
    // All loops must begin with a loop merge
    generator->writer->Instruction(OpLoopMerge, SPVWriter::Section::LocalFunction, SPVArg{endLabel}, SPVArg{repeatLabel}, LoopControl::None);
    generator->writer->Instruction(OpBranch, SPVWriter::Section::LocalFunction, SPVArg{conditionLabel});
    generator->writer->Reserved(OpLabel, SPVWriter::Section::LocalFunction, conditionLabel);
    
    // This block is for the condition testing
    generator->blockOpen = true;
    SPIRVResult cond = GenerateExpressionSPIRV(compiler, generator, stat->condition);
    cond = LoadValueSPIRV(compiler, generator, cond);

    generator->writer->Instruction(OpBranchConditional, SPVWriter::Section::LocalFunction, cond, SPVArg{bodyLabel}, SPVArg{endLabel});
    generator->writer->Reserved(OpLabel, SPVWriter::Section::LocalFunction, bodyLabel);
    
    // Decide whether or not to end the loop
    generator->blockOpen = true;
    if (!GenerateStatementSPIRV(compiler, generator, stat->contents))
    {
        generator->writer->Instruction(OpBranch, SPVWriter::Section::LocalFunction, SPVArg{repeatLabel});
        generator->blockOpen = false;
    }

    // This is the repeat condition
    generator->writer->Reserved(OpLabel, SPVWriter::Section::LocalFunction, repeatLabel);
    
    generator->blockOpen = true;
    GenerateExpressionSPIRV(compiler, generator, stat->loop);

    generator->writer->Instruction(OpBranch, SPVWriter::Section::LocalFunction, SPVArg{startLabel});
    generator->writer->Reserved(OpLabel, SPVWriter::Section::LocalFunction, endLabel);
    
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
    
    uint32_t ifLabel = generator->writer->Reserve();
    uint32_t endLabel = generator->writer->Reserve();

    if (compiler->options.debugSymbols)
    {
        uint32_t name = generator->writer->String(stat->location.file.c_str());
        generator->writer->Instruction(OpLine, SPVWriter::Section::LocalFunction, SPVArg{name}, stat->location.line, stat->location.start);
    }
    generator->writer->Instruction(OpSelectionMerge, SPVWriter::Section::LocalFunction, SPVArg{endLabel}, SelectionControl::None);
    if (stat->elseStatement)
    {
        uint32_t elseLabel = generator->writer->Reserve();

        generator->writer->Instruction(OpBranchConditional, SPVWriter::Section::LocalFunction, lhsResult, SPVArg{ifLabel}, SPVArg{elseLabel});
        generator->blockOpen = false;

        generator->writer->Reserved(OpLabel, SPVWriter::Section::LocalFunction, ifLabel);
        generator->blockOpen = true;
        if (!GenerateStatementSPIRV(compiler, generator, stat->ifStatement))
        {
            generator->writer->Instruction(OpBranch, SPVWriter::Section::LocalFunction, SPVArg{endLabel});
            generator->blockOpen = false;
        }

        generator->writer->Reserved(OpLabel, SPVWriter::Section::LocalFunction, elseLabel);
        generator->blockOpen = true;
        if (!GenerateStatementSPIRV(compiler, generator, stat->elseStatement))
        {
            generator->writer->Instruction(OpBranch, SPVWriter::Section::LocalFunction, SPVArg{endLabel});
            generator->blockOpen = false;
        }
    }
    else
    {
        generator->writer->Instruction(OpBranchConditional, SPVWriter::Section::LocalFunction, lhsResult, SPVArg{ifLabel}, SPVArg{endLabel});
        generator->blockOpen = false;

        generator->writer->Reserved(OpLabel, SPVWriter::Section::LocalFunction, ifLabel);
        generator->blockOpen = true;
        if (!GenerateStatementSPIRV(compiler, generator, stat->ifStatement))
        {
            generator->writer->Instruction(OpBranch, SPVWriter::Section::LocalFunction, SPVArg{endLabel});
            generator->blockOpen = false;
        }
    }
    
    generator->writer->Reserved(OpLabel, SPVWriter::Section::LocalFunction, endLabel);
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

            generator->writer->Instruction(OpReturnValue, SPVWriter::Section::LocalFunction, res);
            generator->blockOpen = false;
        }
        else
        {
            generator->writer->Instruction(OpReturn, SPVWriter::Section::LocalFunction);
            generator->blockOpen = false;
        }
    }
    else if (stat->type == TerminateStatement::TerminationType::Discard)
    {
        generator->writer->Instruction(OpKill, SPVWriter::Section::LocalFunction);
        generator->blockOpen = false;
    }
    else if (stat->type == TerminateStatement::TerminationType::RayTerminate)
    {
        generator->writer->Instruction(OpTerminateRayKHR, SPVWriter::Section::LocalFunction);
        generator->blockOpen = false;
    }
    else if (stat->type == TerminateStatement::TerminationType::RayIgnoreIntersection)
    {
        generator->writer->Instruction(OpIgnoreIntersectionKHR, SPVWriter::Section::LocalFunction);
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
        if (val.ui[0] < stat->caseExpressions.size)
            GenerateStatementSPIRV(compiler, generator, stat->caseStatements[val.ui[0]]);
        else if (stat->defaultStatement != nullptr)
            GenerateStatementSPIRV(compiler, generator, stat->defaultStatement);
        generator->skipBreakContinue = false;
        return;
    }
    
    SPIRVResult switchRes = GenerateExpressionSPIRV(compiler, generator, stat->switchExpression);
    switchRes = LoadValueSPIRV(compiler, generator, switchRes);

    uint32_t defaultCase = generator->writer->Reserve();
    uint32_t mergeLabel = generator->writer->Reserve();

    generator->mergeBlocks[generator->mergeBlockCounter++] =
    SPIRVGenerator::MergeBlock
    {
        0xFFFFFFFF, mergeLabel
    };

    // First forward declare our labels and setup the switch
    std::string caseList = "";
    std::vector<uint32_t> reservedCaseLabels;


    TransientArray<uint32_t> caseArgs(stat->caseExpressions.size);
    TransientArray<SPVArg> branchArgs(stat->caseExpressions.size);
    for (size_t i = 0; i < stat->caseExpressions.size; i++)
    {
        uint32_t caseLabel = generator->writer->Reserve();
        ValueUnion val;
        bool res = stat->caseExpressions[i]->EvalValue(val);
        assert(res);
        caseList += Format("%d %%%d ", val.i[0], caseLabel);
        reservedCaseLabels.push_back(caseLabel);

        branchArgs.Append(SPVArg{caseLabel});
        caseArgs.Append(val.ui[0]);
    }

    if (compiler->options.debugSymbols)
    {
        uint32_t name = generator->writer->String(stat->location.file.c_str());
        generator->writer->Instruction(OpLine, SPVWriter::Section::LocalFunction, SPVArg{name}, stat->location.line, stat->location.start);
    }
    generator->writer->Instruction(OpSelectionMerge, SPVWriter::Section::LocalFunction, SPVArg{mergeLabel}, SelectionControl::None);
    generator->writer->Instruction(OpSwitch, SPVWriter::Section::LocalFunction, switchRes, SPVArg{defaultCase}, SPVCaseList(caseArgs, branchArgs));

    for (size_t i = 0; i < stat->caseStatements.size; i++)
    {
        generator->writer->Reserved(OpLabel, SPVWriter::Section::LocalFunction, reservedCaseLabels[i]);
        generator->blockOpen = true;
        if (stat->caseStatements[i] != nullptr)
        {
            if (!GenerateStatementSPIRV(compiler, generator, stat->caseStatements[i]))
                if (i + 1 < stat->caseStatements.size)
                {
                    generator->writer->Instruction(OpBranch, SPVWriter::Section::LocalFunction, SPVArg{reservedCaseLabels[i+1]});
                }
                else
                {
                    generator->writer->Instruction(OpBranch, SPVWriter::Section::LocalFunction, SPVArg{stat->defaultStatement == nullptr ? mergeLabel : defaultCase});
                }
            generator->blockOpen = false;
        }
        else
        {
            if (i + 1 < stat->caseStatements.size)
            {
                generator->writer->Instruction(OpBranch, SPVWriter::Section::LocalFunction, SPVArg{reservedCaseLabels[i+1]});
            }
            else
            {
                generator->writer->Instruction(OpBranch, SPVWriter::Section::LocalFunction, SPVArg{stat->defaultStatement == nullptr ? mergeLabel : defaultCase});
            }
            generator->blockOpen = false;
        }
    }

    generator->writer->Reserved(OpLabel, SPVWriter::Section::LocalFunction, defaultCase);
    generator->blockOpen = true;
    if (stat->defaultStatement)
    {
        if (!GenerateStatementSPIRV(compiler, generator, stat->defaultStatement))
        {
            generator->writer->Instruction(OpBranch, SPVWriter::Section::LocalFunction, SPVArg{mergeLabel});
            generator->blockOpen = false;
        }
    }
    else
    {
        generator->writer->Instruction(OpBranch, SPVWriter::Section::LocalFunction, SPVArg{mergeLabel});
        generator->blockOpen = false;
    }

    generator->writer->Reserved(OpLabel, SPVWriter::Section::LocalFunction, mergeLabel);
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

    uint32_t startLabel = generator->writer->Reserve();
    uint32_t conditionLabel = generator->writer->Reserve();
    uint32_t continueLabel = generator->writer->Reserve();
    uint32_t bodyLabel = generator->writer->Reserve();
    uint32_t endLabel = generator->writer->Reserve();
    
    generator->mergeBlocks[generator->mergeBlockCounter++] =
    SPIRVGenerator::MergeBlock
    {
        continueLabel, endLabel
    };

    generator->writer->Instruction(OpBranch, SPVWriter::Section::LocalFunction, SPVArg{startLabel});
    generator->writer->Reserved(OpLabel, SPVWriter::Section::LocalFunction, startLabel);
    
    // Initial label to start the loop
    generator->blockOpen = true;

    if (stat->isDoWhile)
    {
        // All loops must begin with a loop merge
        generator->writer->Instruction(OpLoopMerge, SPVWriter::Section::LocalFunction, SPVArg{endLabel}, SPVArg{continueLabel});
        generator->writer->Instruction(OpBranch, SPVWriter::Section::LocalFunction, SPVArg{bodyLabel});
        
        generator->blockOpen = false;

        // Decide whether or not to end the loop
        generator->writer->Reserved(OpLabel, SPVWriter::Section::LocalFunction, bodyLabel);
        generator->blockOpen = true;

        if (!GenerateStatementSPIRV(compiler, generator, stat->statement))
        {
            generator->writer->Instruction(OpBranch, SPVWriter::Section::LocalFunction, SPVArg{conditionLabel});
            generator->blockOpen = false;
        }

        // This block is for the condition testing
        generator->writer->Reserved(OpLabel, SPVWriter::Section::LocalFunction, conditionLabel);
        generator->blockOpen = true;

        SPIRVResult cond = GenerateExpressionSPIRV(compiler, generator, stat->condition);
        cond = LoadValueSPIRV(compiler, generator, cond);

        generator->writer->Instruction(OpBranchConditional, SPVWriter::Section::LocalFunction, cond, SPVArg{continueLabel}, SPVArg{endLabel});
        generator->blockOpen = false;

        generator->writer->Reserved(OpLabel, SPVWriter::Section::LocalFunction, continueLabel);
        generator->writer->Instruction(OpBranch, SPVWriter::Section::LocalFunction, SPVArg{startLabel});
        
        // This is the repeat condition
        generator->writer->Reserved(OpLabel, SPVWriter::Section::LocalFunction, endLabel);
        generator->blockOpen = true;
    }
    else
    {
        // All loops must begin with a loop merge
        generator->writer->Instruction(OpLoopMerge, SPVWriter::Section::LocalFunction, SPVArg{endLabel}, SPVArg{continueLabel});
        generator->writer->Instruction(OpBranch, SPVWriter::Section::LocalFunction, SPVArg{conditionLabel});
        generator->blockOpen = false;

        // Load from the variable to do the branch condition
        generator->writer->Reserved(OpLabel, SPVWriter::Section::LocalFunction, conditionLabel);
        generator->blockOpen = true;

        SPIRVResult cond = GenerateExpressionSPIRV(compiler, generator, stat->condition);
        cond = LoadValueSPIRV(compiler, generator, cond);

        // Decide whether or not to end the loop
        generator->writer->Instruction(OpBranchConditional, SPVWriter::Section::LocalFunction, cond, SPVArg{bodyLabel}, SPVArg{endLabel});
        generator->blockOpen = false;
        if (stat->statement)
        {
            generator->writer->Reserved(OpLabel, SPVWriter::Section::LocalFunction, bodyLabel);
            generator->blockOpen = true;
            if (!GenerateStatementSPIRV(compiler, generator, stat->statement))
            {
                generator->writer->Instruction(OpBranch, SPVWriter::Section::LocalFunction, SPVArg{continueLabel});
                generator->blockOpen = false;
            }
        }

        generator->writer->Reserved(OpLabel, SPVWriter::Section::LocalFunction, continueLabel);
        generator->writer->Instruction(OpBranch, SPVWriter::Section::LocalFunction, SPVArg{startLabel});

        // This is the repeat condition
        generator->writer->Reserved(OpLabel, SPVWriter::Section::LocalFunction, endLabel);
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
    if (compiler->options.debugSymbols)
    {
        uint32_t name = generator->writer->String(stat->location.file.c_str());
        generator->writer->Instruction(OpLine, SPVWriter::Section::LocalFunction, SPVArg{name}, stat->location.line, stat->location.start);
    }
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
            for (auto& expr : static_cast<ExpressionStatement*>(stat)->expressions)
                GenerateExpressionSPIRV(compiler, generator, expr);
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
            generator->writer->PushScope();
            const PinnedArray<Symbol*>& symbols = scope->symbols;
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
            generator->writer->PopScope();
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
SPIRVGenerator::Generate(const Compiler* compiler, const ProgramInstance* program, const PinnedArray<Symbol*>& symbols, std::function<void(const std::string&, const std::string&)> writerFunc)
{
    ProgramInstance::__Resolved* progResolved = static_cast<ProgramInstance::__Resolved*>(program->resolved);

    struct Cleanup
    {
        Cleanup(SPIRVGenerator* gen) :
            gen(gen) {}

        ~Cleanup()
        {
            gen->interfaceVariables.Clear();
            gen->accessChain.clear();
            gen->mergeBlockCounter = 0;
            gen->linkDefineSlot = 0;

            gen->writer->counter = 0;
            for (auto& binary : gen->writer->binaries)
                binary.clear();
            for (auto& text : gen->writer->texts)
                text.Clear();
            gen->writer->scopeStack.clear();
            gen->writer->imports.Clear();
            gen->writer->extensions.Clear();
            gen->writer->decorations.Clear();
            gen->writer->capabilities.Clear();
            gen->writer->strings.Clear();
            
        }

        SPIRVGenerator* gen;
    };

    spv_context spvContext = spvContextCreate(SPV_ENV_VULKAN_1_2);

    static std::unordered_map<ProgramInstance::__Resolved::EntryType, std::string> executionModelMap =
    {
        { ProgramInstance::__Resolved::EntryType::VertexShader, "Vertex" }
        , { ProgramInstance::__Resolved::EntryType::HullShader, "TessellationControl" }
        , { ProgramInstance::__Resolved::EntryType::DomainShader, "TessellationEvaluation" }
        , { ProgramInstance::__Resolved::EntryType::GeometryShader, "Geometry" }
        , { ProgramInstance::__Resolved::EntryType::PixelShader, "Fragment" }
        , { ProgramInstance::__Resolved::EntryType::ComputeShader, "GLCompute" }
        , { ProgramInstance::__Resolved::EntryType::TaskShader, "TaskEXT" }
        , { ProgramInstance::__Resolved::EntryType::MeshShader, "MeshEXT" }
        , { ProgramInstance::__Resolved::EntryType::RayGenerationShader, "RayGenerationKHR" }
        , { ProgramInstance::__Resolved::EntryType::RayMissShader, "MissKHR" }
        , { ProgramInstance::__Resolved::EntryType::RayClosestHitShader, "ClosestHitKHR" }
        , { ProgramInstance::__Resolved::EntryType::RayAnyHitShader, "AnyHitKHR" }
        , { ProgramInstance::__Resolved::EntryType::RayIntersectionShader, "IntersectionKHR" }
        , { ProgramInstance::__Resolved::EntryType::RayCallableShader, "CallableKHR" }
    };

    static std::unordered_map<ProgramInstance::__Resolved::EntryType, SPVEnum> executionModelEnumMap =
    {
        { ProgramInstance::__Resolved::EntryType::VertexShader, ExecutionModels::Vertex }
        , { ProgramInstance::__Resolved::EntryType::HullShader, ExecutionModels::TessellationControl }
        , { ProgramInstance::__Resolved::EntryType::DomainShader, ExecutionModels::TessellationEvaluation }
        , { ProgramInstance::__Resolved::EntryType::GeometryShader, ExecutionModels::Geometry }
        , { ProgramInstance::__Resolved::EntryType::PixelShader, ExecutionModels::Fragment }
        , { ProgramInstance::__Resolved::EntryType::ComputeShader, ExecutionModels::GLCompute }
        , { ProgramInstance::__Resolved::EntryType::TaskShader, ExecutionModels::TaskEXT }
        , { ProgramInstance::__Resolved::EntryType::MeshShader, ExecutionModels::MeshEXT }
        , { ProgramInstance::__Resolved::EntryType::RayGenerationShader, ExecutionModels::RayGenerationKHR }
        , { ProgramInstance::__Resolved::EntryType::RayMissShader, ExecutionModels::MissKHR }
        , { ProgramInstance::__Resolved::EntryType::RayClosestHitShader, ExecutionModels::ClosestHitKHR }
        , { ProgramInstance::__Resolved::EntryType::RayAnyHitShader, ExecutionModels::AnyHitKHR }
        , { ProgramInstance::__Resolved::EntryType::RayIntersectionShader, ExecutionModels::IntersectionKHR }
        , { ProgramInstance::__Resolved::EntryType::RayCallableShader, ExecutionModels::CallableKHR }
    };

    static std::unordered_map<ProgramInstance::__Resolved::EntryType, std::string> extensionMap =
    {
        { ProgramInstance::__Resolved::EntryType::VertexShader, "Shader" }
        , { ProgramInstance::__Resolved::EntryType::HullShader, "Tessellation" }
        , { ProgramInstance::__Resolved::EntryType::DomainShader, "Tessellation" }
        , { ProgramInstance::__Resolved::EntryType::GeometryShader, "Geometry" }
        , { ProgramInstance::__Resolved::EntryType::PixelShader, "Shader" }
        , { ProgramInstance::__Resolved::EntryType::ComputeShader, "Shader" }
        , { ProgramInstance::__Resolved::EntryType::TaskShader, "MeshShadingEXT" }
        , { ProgramInstance::__Resolved::EntryType::MeshShader, "MeshShadingEXT" }
        , { ProgramInstance::__Resolved::EntryType::RayGenerationShader, "RayTracingKHR" }
        , { ProgramInstance::__Resolved::EntryType::RayMissShader, "RayTracingKHR" }
        , { ProgramInstance::__Resolved::EntryType::RayClosestHitShader, "RayTracingKHR" }
        , { ProgramInstance::__Resolved::EntryType::RayAnyHitShader, "RayTracingKHR" }
        , { ProgramInstance::__Resolved::EntryType::RayIntersectionShader, "RayTracingKHR" }
        , { ProgramInstance::__Resolved::EntryType::RayCallableShader, "RayTracingKHR" }
    };

    static std::unordered_map<ProgramInstance::__Resolved::EntryType, SPVEnum> extensionEnumMap =
    {
        { ProgramInstance::__Resolved::EntryType::VertexShader, Capabilities::Shader }
        , { ProgramInstance::__Resolved::EntryType::HullShader, Capabilities::Tessellation }
        , { ProgramInstance::__Resolved::EntryType::DomainShader, Capabilities::Tessellation }
        , { ProgramInstance::__Resolved::EntryType::GeometryShader, Capabilities::Geometry }
        , { ProgramInstance::__Resolved::EntryType::PixelShader, Capabilities::Shader }
        , { ProgramInstance::__Resolved::EntryType::ComputeShader, Capabilities::Shader }
        , { ProgramInstance::__Resolved::EntryType::TaskShader, Capabilities::MeshShadingEXT }
        , { ProgramInstance::__Resolved::EntryType::MeshShader, Capabilities::MeshShadingEXT }
        , { ProgramInstance::__Resolved::EntryType::RayGenerationShader, Capabilities::RayTracingKHR }
        , { ProgramInstance::__Resolved::EntryType::RayMissShader, Capabilities::RayTracingKHR }
        , { ProgramInstance::__Resolved::EntryType::RayClosestHitShader, Capabilities::RayTracingKHR }
        , { ProgramInstance::__Resolved::EntryType::RayAnyHitShader, Capabilities::RayTracingKHR }
        , { ProgramInstance::__Resolved::EntryType::RayIntersectionShader, Capabilities::RayTracingKHR }
        , { ProgramInstance::__Resolved::EntryType::RayCallableShader, Capabilities::RayTracingKHR }
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
    for (uint32_t mapping = ProgramInstance::__Resolved::EntryType::FirstShader; mapping < ProgramInstance::__Resolved::EntryType::LastShader; mapping++)
    {
        Cleanup cleanup(this);

        Symbol* object = progResolved->mappings[mapping];
        if (object == nullptr)
            continue;

        this->shaderStage = mapping;
        this->shaderValueExpressions[mapping].value = true;

        this->writer->counter++;
        this->writer->outputText = compiler->options.validate;

        // Main scope
        this->writer->PushScope();

        // Temporarily store original variable values
        std::unordered_map<Variable*, Expression*> originalVariableValues;
        auto it = progResolved->constVarInitializationOverrides.begin();
        for (; it != progResolved->constVarInitializationOverrides.end(); it++)
        {
            originalVariableValues[it->first] = it->first->valueExpression;
            it->first->valueExpression = it->second;
        }

        this->writer->Capability(extensionEnumMap[(ProgramInstance::__Resolved::EntryType)mapping]);
        if (compiler->target.supportsPhysicalAddressing)
        {
            this->writer->Capability(Capabilities::Addresses);
            this->writer->Instruction(OpMemoryModel, SPVWriter::Section::Header, AddressingModels::Physical64, MemoryModels::GLSL450);
        }
        else
        {
            this->writer->Capability(Capabilities::PhysicalStorageBufferAddresses);
            this->writer->Instruction(OpMemoryModel, SPVWriter::Section::Header, AddressingModels::PhysicalStorageBuffer64, MemoryModels::GLSL450);
        }

        if (compiler->options.debugSymbols)
        {
            TransientString path = compiler->path;
            std::replace(path.buf, path.buf + path.size, '\\', '/');
            uint32_t file = this->writer->String(path.c_str());
            this->writer->Instruction(OpSource, SPVWriter::Section::DebugNames, SourceLanguage::Unknown, 1, SPVArg{file});
        }

        this->entryPoint = static_cast<Function*>(object);

        for (Symbol* sym : symbols)
        {
            switch (sym->symbolType)
            {
            case Symbol::GenerateType:
            {
                auto gen = static_cast<struct Generate*>(sym);
                auto genRes = Symbol::Resolved(gen);
                for (Symbol* genSym : genRes->generatedSymbols)
                {
                    switch (genSym->symbolType)
                    {
                        case Symbol::FunctionType:
                            GenerateFunctionSPIRV(compiler, this, genSym);
                            break;
                        case Symbol::StructureType:
                            GenerateStructureSPIRV(compiler, this, genSym);
                            break;
                        case Symbol::EnumerationType:
                            GenerateEnumSPIRV(compiler, this, genSym);
                            break;
                        case Symbol::SamplerStateInstanceType:
                            GenerateSamplerSPIRV(compiler, this, genSym);
                            break;
                        case Symbol::VariableType:
                            GenerateVariableSPIRV(compiler, this, genSym, false, true);
                            break;
                    }
                }
                break;
            }
            case Symbol::FunctionType:
                GenerateFunctionSPIRV(compiler, this, sym);
                break;
            case Symbol::StructureType:
                GenerateStructureSPIRV(compiler, this, sym);
                break;
            case Symbol::EnumerationType:
                GenerateEnumSPIRV(compiler, this, sym);
                break;
            case Symbol::SamplerStateInstanceType:
                GenerateSamplerSPIRV(compiler, this, sym);
                break;
            case Symbol::VariableType:
                GenerateVariableSPIRV(compiler, this, sym, false, true);
                break;
            }
        }

        Function::__Resolved* funResolved = Symbol::Resolved(static_cast<Function*>(object));
        uint32_t entryFunction = GPULang::GetSymbol(this, funResolved->name).value;

        if (funResolved->executionModifiers.groupSize != 64 || funResolved->executionModifiers.groupsPerWorkgroup != 1)
        {
            this->writer->Capability(Capabilities::SubgroupDispatch);
        }
        std::string interfaces = "";

        TransientArray<SPVArg> interfaceVars(this->interfaceVariables.size());
        for (const uint32_t inter : this->interfaceVariables)
        {
            interfaces.append(Format("%%%d ", inter));
            interfaceVars.Append(SPVArg(inter));
        }

        this->writer->Instruction(OpEntryPoint, SPVWriter::Section::Header, executionModelEnumMap[(ProgramInstance::__Resolved::EntryType)mapping], SPVArg(entryFunction), "main", SPVArgList(interfaceVars));

        switch (mapping)
        {
        case ProgramInstance::__Resolved::GeometryShader:
            this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{ entryFunction }, ExecutionModes::Invocations, funResolved->executionModifiers.invocations);
            this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{ entryFunction }, ExecutionModes::OutputVertices, funResolved->executionModifiers.maxOutputVertices);
            this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{ entryFunction }, inputPrimitiveTopologyEnumMap[funResolved->executionModifiers.inputPrimitiveTopology]);
            this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{ entryFunction }, outputPrimitiveTopologyEnumMap[funResolved->executionModifiers.inputPrimitiveTopology]);
            break;
        case ProgramInstance::__Resolved::HullShader:
            this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{ entryFunction }, partitionMap[funResolved->executionModifiers.partitionMethod]);
            this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{ entryFunction }, windingOrderMap[funResolved->executionModifiers.windingOrder]);
            this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{ entryFunction }, inputPrimitiveTopologyEnumMap[funResolved->executionModifiers.inputPrimitiveTopology]);
            this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{ entryFunction }, ExecutionModes::OutputVertices, funResolved->executionModifiers.maxOutputVertices);
            break;
        case ProgramInstance::__Resolved::DomainShader:
            break;
        case ProgramInstance::__Resolved::PixelShader:
        {
            this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{ entryFunction }, pixelOriginEnumMap[funResolved->executionModifiers.pixelOrigin]);
            if (funResolved->executionModifiers.earlyDepth)
            {
                this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{ entryFunction }, ExecutionModes::EarlyFragmentTests);
            }
            if (funResolved->executionModifiers.depthAlwaysGreater)
            {
                this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{ entryFunction }, ExecutionModes::DepthGreater);
            }
            if (funResolved->executionModifiers.depthAlwaysLesser)
            {
                this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{ entryFunction }, ExecutionModes::DepthLess);
            }
            RenderStateInstance* state = static_cast<RenderStateInstance*>(progResolved->mappings[ProgramInstance::__Resolved::RenderState]);
            RenderStateInstance::__Resolved* stateRes = Symbol::Resolved(state);
            if (!stateRes->depthWriteEnabled)
            {
                // Set depth to be unchanged if depth is never written
                this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{entryFunction}, ExecutionModes::DepthUnchanged);
            }
            else
            {
                if (progResolved->effects.flags.explicitDepth)
                {
                    this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{entryFunction}, ExecutionModes::DepthReplacing);
                }
            }
            break;
        }
        case ProgramInstance::__Resolved::ComputeShader:
            this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{ entryFunction }, ExecutionModes::LocalSize, funResolved->executionModifiers.computeShaderWorkGroupSize[0], funResolved->executionModifiers.computeShaderWorkGroupSize[1], funResolved->executionModifiers.computeShaderWorkGroupSize[2]);
            if (funResolved->executionModifiers.groupSize != 64)
            {
                this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{ entryFunction }, ExecutionModes::SubgroupSize, funResolved->executionModifiers.groupSize);
            }
            if (funResolved->executionModifiers.groupsPerWorkgroup != 1)
            {
                this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{ entryFunction }, ExecutionModes::SubgroupsPerWorkgroup, funResolved->executionModifiers.groupsPerWorkgroup);
            }
            if (funResolved->executionModifiers.computeDerivativeIndexing == Function::__Resolved::DerivativeIndexLinear)
            {
                this->writer->Extension(SPV_KHR_compute_shader_derivatives);
                this->writer->Capability(Capabilities::ComputeDerivativeGroupLinearKHR);
                this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg(entryFunction), ExecutionModes::DerivativeGroupLinearKHR);
            }
            else if (funResolved->executionModifiers.computeDerivativeIndexing == Function::__Resolved::DerivativeIndexQuad)
            {
                this->writer->Extension(SPV_KHR_compute_shader_derivatives);
                this->writer->Capability(Capabilities::ComputeDerivativeGroupQuadsKHR);
                this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg(entryFunction), ExecutionModes::DerivativeGroupQuadsKHR);
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

        this->writer->Header(1, 5, 1, 1, this->writer->counter);

        std::vector<uint32_t> spvBinary;
        spvBinary.insert(spvBinary.end(), this->writer->binaries[(uint32_t)SPVWriter::Section::Top].begin(), this->writer->binaries[(uint32_t)SPVWriter::Section::Top].end());
        spvBinary.insert(spvBinary.end(), this->writer->binaries[(uint32_t)SPVWriter::Section::Capabilities].begin(), this->writer->binaries[(uint32_t)SPVWriter::Section::Capabilities].end());
        spvBinary.insert(spvBinary.end(), this->writer->binaries[(uint32_t)SPVWriter::Section::Extensions].begin(), this->writer->binaries[(uint32_t)SPVWriter::Section::Extensions].end());
        spvBinary.insert(spvBinary.end(), this->writer->binaries[(uint32_t)SPVWriter::Section::ExtImports].begin(), this->writer->binaries[(uint32_t)SPVWriter::Section::ExtImports].end());
        spvBinary.insert(spvBinary.end(), this->writer->binaries[(uint32_t)SPVWriter::Section::Header].begin(), this->writer->binaries[(uint32_t)SPVWriter::Section::Header].end());
        if (compiler->options.debugSymbols)
        {
            spvBinary.insert(spvBinary.end(), this->writer->binaries[(uint32_t)SPVWriter::Section::DebugStrings].begin(), this->writer->binaries[(uint32_t)SPVWriter::Section::DebugStrings].end());
            spvBinary.insert(spvBinary.end(), this->writer->binaries[(uint32_t)SPVWriter::Section::DebugNames].begin(), this->writer->binaries[(uint32_t)SPVWriter::Section::DebugNames].end());
        }   
        spvBinary.insert(spvBinary.end(), this->writer->binaries[(uint32_t)SPVWriter::Section::Decorations].begin(), this->writer->binaries[(uint32_t)SPVWriter::Section::Decorations].end());
        spvBinary.insert(spvBinary.end(), this->writer->binaries[(uint32_t)SPVWriter::Section::Declarations].begin(), this->writer->binaries[(uint32_t)SPVWriter::Section::Declarations].end());
        spvBinary.insert(spvBinary.end(), this->writer->binaries[(uint32_t)SPVWriter::Section::Functions].begin(), this->writer->binaries[(uint32_t)SPVWriter::Section::Functions].end());
        
        spv_diagnostic diag = nullptr;
        spv_result_t res;
        
        if (compiler->options.validate)
        {
            // Run spv validation for internal consistency and fault testing
            spv_const_binary_t constBin = { spvBinary.data(), spvBinary.size() };
            res = spvValidate(spvContext, &constBin, &diag);
            if (res != SPV_SUCCESS)
            {
                this->Error(Format("Internal SPIRV generation error: %s", diag->error));
                printf("%s\n", diag->error);
                
                GrowingString binary;
                binary.Line("; Entry point", funResolved->name);
                binary.Append(this->writer->texts[(uint32_t)SPVWriter::Section::Top]);
                binary.Append(this->writer->texts[(uint32_t)SPVWriter::Section::Capabilities]);
                binary.Append(this->writer->texts[(uint32_t)SPVWriter::Section::Extensions]);
                binary.Append(this->writer->texts[(uint32_t)SPVWriter::Section::ExtImports]);
                binary.Append(this->writer->texts[(uint32_t)SPVWriter::Section::Header]);
                if (compiler->options.debugSymbols)
                {
                    binary.Append("\n; Debug\n");
                    binary.Append(this->writer->texts[(uint32_t)SPVWriter::Section::DebugStrings]);
                    binary.Append(this->writer->texts[(uint32_t)SPVWriter::Section::DebugNames]);
                }
                binary.Append("\n; Decorations\n");
                binary.Append(this->writer->texts[(uint32_t)SPVWriter::Section::Decorations]);
                binary.Append("\n; Declarations\n");
                binary.Append(this->writer->texts[(uint32_t)SPVWriter::Section::Declarations]);
                binary.Append("\n; Functions\n");
                binary.Append(this->writer->texts[(uint32_t)SPVWriter::Section::Functions]);
                
                printf("%s\n", binary.data);
                this->Error(std::string(binary.data, binary.size));
                return false;
            }
        }

        if (compiler->options.optimize)
        {
            spvtools::Optimizer optimizer(SPV_ENV_VULKAN_1_2);
            optimizer.RegisterPerformancePasses();

            std::vector<uint32_t> optimized;
            if (optimizer.Run(spvBinary.data(), spvBinary.size(), &optimized))
            {
                spvBinary = std::move(optimized);
            }
        }

        // conversion and optional validation is successful, dump binary in program
        progResolved->binaries[mapping] = spvBinary;

        if (diag != nullptr)
            delete diag;
    }

    this->evaluatingProgram = nullptr;
    return true;
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

} // namespace GPULang
