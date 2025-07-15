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

#include "generated/types_spirv.h"

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
SPV_INSTRUCTION(OpConvertFToU, 109, 4, false)
SPV_INSTRUCTION(OpConvertFToS, 110, 4, false)
SPV_INSTRUCTION(OpConvertSToF, 111, 4, false)
SPV_INSTRUCTION(OpConvertUToF, 112, 4, false)
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
        if (type.sampled)
        {
            name = AddType(generator, TStr("sampled_", gpuLangStr), OpTypeSampledImage, SPVArg{ name });
        }

        TStr ty = spirvFormatter(floatType, depthBits, sampleBits, spirvFormat.buf);
        baseType = std::tie(name, gpuLangStr);
        if (type.sampled)
        {
            TStr sampledImageStr = TStr("sampled_", gpuLangStr);
        }
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
    for (size_t i = 0; i < type.modifiers.size(); i++)
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
    SPIRVGenerator::IntrinsicMap[&Float4_ctor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 4);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    SPIRVGenerator::IntrinsicMap[&Float3_ctor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 3);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    SPIRVGenerator::IntrinsicMap[&Float2_ctor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 2);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    SPIRVGenerator::IntrinsicMap[&Float_ctor_UInt] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateConversionSPIRV(c, g, ConversionTable::UIntToFloat, 1, args[0]);
    };
    SPIRVGenerator::IntrinsicMap[&Float_ctor_Int] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateConversionSPIRV(c, g, ConversionTable::IntToFloat, 1, args[0]);
    };
    SPIRVGenerator::IntrinsicMap[&Int4_ctor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 4);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    SPIRVGenerator::IntrinsicMap[&Int3_ctor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 3);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    SPIRVGenerator::IntrinsicMap[&Int2_ctor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 2);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    SPIRVGenerator::IntrinsicMap[&Int_ctor_UInt] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateConversionSPIRV(c, g, ConversionTable::UIntToInt, 1, args[0]);
    };
    SPIRVGenerator::IntrinsicMap[&Int_ctor_Bool] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateConversionSPIRV(c, g, ConversionTable::BoolToInt, 1, args[0]);
    };
    SPIRVGenerator::IntrinsicMap[&Int_ctor_Float] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateConversionSPIRV(c, g, ConversionTable::FloatToInt, 1, args[0]);
    };
    SPIRVGenerator::IntrinsicMap[&UInt4_ctor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 4);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    SPIRVGenerator::IntrinsicMap[&UInt3_ctor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 3);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    SPIRVGenerator::IntrinsicMap[&UInt2_ctor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 2);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    SPIRVGenerator::IntrinsicMap[&UInt_ctor_Int] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateConversionSPIRV(c, g, ConversionTable::IntToUInt, 1, args[0]);
    };
    SPIRVGenerator::IntrinsicMap[&UInt_ctor_Bool] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateConversionSPIRV(c, g, ConversionTable::BoolToUInt, 1, args[0]);
    };
    SPIRVGenerator::IntrinsicMap[&UInt_ctor_Float] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateConversionSPIRV(c, g, ConversionTable::FloatToUInt, 1, args[0]);
    };
    SPIRVGenerator::IntrinsicMap[&Bool4_ctor_XYZW] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 4);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    SPIRVGenerator::IntrinsicMap[&Bool4_ctorSingleValue] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateSplatCompositeSPIRV(c, g, returnType, 4, args[0]);
    };
    SPIRVGenerator::IntrinsicMap[&Bool3_ctor_XYZ] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 3);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    SPIRVGenerator::IntrinsicMap[&Bool3_ctorSingleValue] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateSplatCompositeSPIRV(c, g, returnType, 3, args[0]);
    };
    SPIRVGenerator::IntrinsicMap[&Bool2_ctor_XY] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 2);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    SPIRVGenerator::IntrinsicMap[&Bool2_ctorSingleValue] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        return GenerateSplatCompositeSPIRV(c, g, returnType, 2, args[0]);
    };
    SPIRVGenerator::IntrinsicMap[&Bool_ctor_UInt] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        SPIRVResult val = LoadValueSPIRV(c, g, args[0]);
        uint32_t type = GeneratePODTypeSPIRV(c, g, TypeCode::Bool);
        
        // First, test if integer is either 0 or 1 by comparing it to 
        SPIRVResult falseValue = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(0), 1);
        uint32_t res = g->writer->MappedInstruction(OpINotEqual, SPVWriter::Section::LocalFunction, type, val, falseValue);
        return SPIRVResult(res, type, true);
    };
    SPIRVGenerator::IntrinsicMap[&Bool_ctor_Int] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 1);
        SPIRVResult val = LoadValueSPIRV(c, g, args[0]);
        uint32_t type = GeneratePODTypeSPIRV(c, g, TypeCode::Bool);
        
        // First, test if integer is either 0 or 1 by comparing it to 
        SPIRVResult falseValue = GenerateConstantSPIRV(c, g, ConstantCreationInfo::Int(0), 1);
        uint32_t res = g->writer->MappedInstruction(OpINotEqual, SPVWriter::Section::LocalFunction, type, val, falseValue);
        return SPIRVResult(res, type, true);
    };

#define X(type, ctor, val, argCount, splat, vectorSize, conversion)\
    SPIRVGenerator::IntrinsicMap[&type##_##ctor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {\
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
    SPIRVGenerator::IntrinsicMap[&type##_##ctor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {\
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
    SPIRVGenerator::IntrinsicMap[&type##_##ctor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {\
        return GenerateCompositeSPIRV(c, g, returnType, { args[0], args[1], args[2] });\
    };

    FLOAT4_CTOR3_LIST
    INT4_CTOR3_LIST
    UINT4_CTOR3_LIST
#undef X

    // For matrix float constructors, we need to first construct the vectors and then compose the matrix from them
    SPIRVGenerator::IntrinsicMap[&Mat2x2_identityConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        assert(args.size() == 0);
        SPIRVResult zero = GenerateConstantSPIRV(c, g, ConstantCreationInfo::Float(0));
        SPIRVResult one = GenerateConstantSPIRV(c, g, ConstantCreationInfo::Float(1));
        uint32_t vectorType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, 2);
        SPIRVResult col0 = GenerateCompositeSPIRV(c, g, vectorType, { one, zero });
        SPIRVResult col1 = GenerateCompositeSPIRV(c, g, vectorType, { zero, one });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1 });
    };
    SPIRVGenerator::IntrinsicMap[&Mat2x2_floatConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 4);
        uint32_t vectorType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, 2);
        SPIRVResult col0 = GenerateCompositeSPIRV(c, g, vectorType, { args[0], args[2] });
        SPIRVResult col1 = GenerateCompositeSPIRV(c, g, vectorType, { args[1], args[3] });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1 });
    };
    SPIRVGenerator::IntrinsicMap[&Mat2x2_vectorConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 2);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    
    SPIRVGenerator::IntrinsicMap[&Mat2x3_floatConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 6);
        uint32_t vectorType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, 2);
        SPIRVResult col0 = GenerateCompositeSPIRV(c, g, vectorType, { args[0], args[3] });
        SPIRVResult col1 = GenerateCompositeSPIRV(c, g, vectorType, { args[1], args[4] });
        SPIRVResult col3 = GenerateCompositeSPIRV(c, g, vectorType, { args[2], args[5] });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1, col3 });
    };
    SPIRVGenerator::IntrinsicMap[&Mat2x3_vectorConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 3);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    SPIRVGenerator::IntrinsicMap[&Mat2x4_floatConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 8);
        uint32_t vectorType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, 2);
        SPIRVResult col0 = GenerateCompositeSPIRV(c, g, vectorType, { args[0], args[4] });
        SPIRVResult col1 = GenerateCompositeSPIRV(c, g, vectorType, { args[1], args[5] });
        SPIRVResult col3 = GenerateCompositeSPIRV(c, g, vectorType, { args[2], args[6] });
        SPIRVResult col4 = GenerateCompositeSPIRV(c, g, vectorType, { args[3], args[7] });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1, col3, col4 });
    };
    SPIRVGenerator::IntrinsicMap[&Mat2x4_vectorConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 3);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };

    SPIRVGenerator::IntrinsicMap[&Mat3x2_floatConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 6);
        uint32_t vectorType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, 3);
        SPIRVResult col0 = GenerateCompositeSPIRV(c, g, vectorType, { args[0], args[2], args[4] });
        SPIRVResult col1 = GenerateCompositeSPIRV(c, g, vectorType, { args[1], args[3], args[5] });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1 });
    };
    SPIRVGenerator::IntrinsicMap[&Mat3x2_vectorConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 2);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    SPIRVGenerator::IntrinsicMap[&Mat3x3_identityConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
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
    SPIRVGenerator::IntrinsicMap[&Mat3x3_floatConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 9);
        uint32_t vectorType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, 3);
        SPIRVResult col0 = GenerateCompositeSPIRV(c, g, vectorType, { args[0], args[3], args[6] });
        SPIRVResult col1 = GenerateCompositeSPIRV(c, g, vectorType, { args[1], args[4], args[7] });
        SPIRVResult col2 = GenerateCompositeSPIRV(c, g, vectorType, { args[2], args[5], args[8] });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1, col2 });
    };
    SPIRVGenerator::IntrinsicMap[&Mat3x3_vectorConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 3);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    SPIRVGenerator::IntrinsicMap[&Mat3x4_floatConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 12);
        uint32_t vectorType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, 3);
        SPIRVResult col0 = GenerateCompositeSPIRV(c, g, vectorType, { args[0], args[4], args[8] });
        SPIRVResult col1 = GenerateCompositeSPIRV(c, g, vectorType, { args[1], args[5], args[9] });
        SPIRVResult col2 = GenerateCompositeSPIRV(c, g, vectorType, { args[2], args[6], args[10] });
        SPIRVResult col3 = GenerateCompositeSPIRV(c, g, vectorType, { args[3], args[7], args[11] });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1, col2, col3 });
    };
    SPIRVGenerator::IntrinsicMap[&Mat3x4_vectorConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 4);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    SPIRVGenerator::IntrinsicMap[&Mat4x2_floatConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 8);
        uint32_t vectorType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, 4);
        SPIRVResult col0 = GenerateCompositeSPIRV(c, g, vectorType, { args[0], args[2], args[4], args[6] });
        SPIRVResult col1 = GenerateCompositeSPIRV(c, g, vectorType, { args[1], args[3], args[5], args[7] });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1 });
    };
    SPIRVGenerator::IntrinsicMap[&Mat4x2_vectorConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 2);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    SPIRVGenerator::IntrinsicMap[&Mat4x3_floatConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 12);
        uint32_t vectorType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, 4);
        SPIRVResult col0 = GenerateCompositeSPIRV(c, g, vectorType, { args[0], args[3], args[6], args[9] });
        SPIRVResult col1 = GenerateCompositeSPIRV(c, g, vectorType, { args[1], args[4], args[7], args[10] });
        SPIRVResult col2 = GenerateCompositeSPIRV(c, g, vectorType, { args[2], args[5], args[8], args[11] });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1, col2 });
    };
    SPIRVGenerator::IntrinsicMap[&Mat4x3_vectorConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 3);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };
    SPIRVGenerator::IntrinsicMap[&Mat4x4_identityConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
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
    SPIRVGenerator::IntrinsicMap[&Mat4x4_floatConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 16);
        uint32_t vectorType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, 4);
        SPIRVResult col0 = GenerateCompositeSPIRV(c, g, vectorType, { args[0], args[4], args[8], args[12] });
        SPIRVResult col1 = GenerateCompositeSPIRV(c, g, vectorType, { args[1], args[5], args[9], args[13] });
        SPIRVResult col2 = GenerateCompositeSPIRV(c, g, vectorType, { args[2], args[6], args[10], args[14] });
        SPIRVResult col3 = GenerateCompositeSPIRV(c, g, vectorType, { args[3], args[7], args[11], args[15] });
        return GenerateCompositeSPIRV(c, g, returnType, { col0, col1, col2, col3 });
    };
    SPIRVGenerator::IntrinsicMap[&Mat4x3_vectorConstructor] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        assert(args.size() == 4);
        return GenerateCompositeSPIRV(c, g, returnType, args);
    };

#define OPERATOR_INTRINSIC(ty, fn, op, inst)\
    { &ty##_##fn##Operator, #op, #inst, false, Op##op##inst }\
    , { &ty##_##fn##AssignOperator, #op, #inst, true, Op##op##inst }


#define OPERATOR_INTRINSIC_NO_ASSIGN(ty, fn, op, inst)\
    { &ty##_##fn##Operator, #op, #inst, false, Op##op##inst }

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
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float4, lt, FOrd, LessThan)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float3, lt, FOrd, LessThan)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float2, lt, FOrd, LessThan)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float, lt, FOrd, LessThan)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float4, lte, FOrd, LessThanEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float3, lte, FOrd, LessThanEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float2, lte, FOrd, LessThanEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float, lte, FOrd, LessThanEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float4, gt, FOrd, GreaterThan)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float3, gt, FOrd, GreaterThan)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float2, gt, FOrd, GreaterThan)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float, gt, FOrd, GreaterThan)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float4, gte, FOrd, GreaterThanEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float3, gte, FOrd, GreaterThanEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float2, gte, FOrd, GreaterThanEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float, gte, FOrd, GreaterThanEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float4, e, FOrd, Equal)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float3, e, FOrd, Equal)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float2, e, FOrd, Equal)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float, e, FOrd, Equal)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float4, ne, FOrd, NotEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float3, ne, FOrd, NotEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float2, ne, FOrd, NotEqual)
        , OPERATOR_INTRINSIC_NO_ASSIGN(Float, ne, FOrd, NotEqual)
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
            uint32_t ret = g->writer->MappedInstruction(inst, SPVWriter::Section::LocalFunction, returnType, lhs, rhs);
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
    { &ty##_##fn##Operator, #op, size, #inst, false, Op##op##inst }\
    , { &ty##_##fn##AssignOperator, #op, size, #inst, true, Op##op##inst }

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

            TransientArray<SPVArg> intermediateArgs(size);
            for (uint32_t i = 0; i < size; i++)
            {
                // Proceed to extract from the composites
                uint32_t lhsVec = g->writer->MappedInstruction(OpCompositeExtract, SPVWriter::Section::LocalFunction, vectorType, lhs, i);
                uint32_t rhsVec = g->writer->MappedInstruction(OpCompositeExtract, SPVWriter::Section::LocalFunction, vectorType, rhs, i);
                uint32_t res = g->writer->MappedInstruction(inst, SPVWriter::Section::LocalFunction, vectorType, SPVArg(lhsVec), SPVArg(rhsVec));
                intermediateArgs.Append(SPVArg(res));
            }

            // Finally compose back to a matrix
            uint32_t ret = g->writer->MappedInstruction(OpCompositeConstruct, SPVWriter::Section::LocalFunction, returnType, SPVArgList(intermediateArgs));
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
    { &ty##_##fn##Operator, #inst, size, false, OpI##inst }
    
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

            uint32_t ret = g->writer->MappedInstruction(inst, SPVWriter::Section::LocalFunction, returnType, lhsConverted, rhsConverted);
            TStr opStr = TStr::Compact("OpI", ty);
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
        { &Int_uintAssignOperator, ConversionTable::UIntToInt }
        , { &Int_floatAssignOperator, ConversionTable::FloatToInt }
        , { &Int_boolAssignOperator, ConversionTable::BoolToInt }
        , { &UInt_intAssignOperator, ConversionTable::IntToUInt }
        , { &UInt_floatAssignOperator, ConversionTable::FloatToUInt }
        , { &UInt_boolAssignOperator, ConversionTable::BoolToUInt }
        , { &Float_intAssignOperator, ConversionTable::IntToFloat }
        , { &Float_uintAssignOperator, ConversionTable::UIntToFloat }
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
        { &Mat2x2_multiplyOperator, false }
        , { &Mat2x3_multiplyOperator, false }
        , { &Mat2x4_multiplyOperator, false }
        , { &Mat3x2_multiplyOperator, false }
        , { &Mat3x3_multiplyOperator, false }
        , { &Mat3x4_multiplyOperator, false }
        , { &Mat4x4_multiplyOperator, false }
        , { &Mat4x3_multiplyOperator, false }
        , { &Mat4x2_multiplyOperator, false }
        , { &Mat2x2_multiplyAssignOperator, true }
        , { &Mat2x3_multiplyAssignOperator, true }
        , { &Mat2x4_multiplyAssignOperator, true }
        , { &Mat3x2_multiplyAssignOperator, true }
        , { &Mat3x3_multiplyAssignOperator, true }
        , { &Mat3x4_multiplyAssignOperator, true }
        , { &Mat4x2_multiplyAssignOperator, true }
        , { &Mat4x3_multiplyAssignOperator, true }
        , { &Mat4x4_multiplyAssignOperator, true }
    };
    for (auto fun : matrixMultiplicationFunctions)
    {
        SPIRVGenerator::IntrinsicMap[fun.first] = [assign = fun.second](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            SPIRVResult lhs = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult rhs = LoadValueSPIRV(c, g, args[1]);
            uint32_t ret = g->writer->MappedInstruction(OpMatrixTimesMatrix, SPVWriter::Section::LocalFunction, returnType, lhs, rhs);
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
        &Mat2x2_scaleOperator
        , &Mat2x3_scaleOperator
        , &Mat2x4_scaleOperator
        , &Mat3x2_scaleOperator
        , &Mat3x3_scaleOperator
        , &Mat3x4_scaleOperator
        , &Mat4x2_scaleOperator
        , &Mat4x3_scaleOperator
        , &Mat4x4_scaleOperator
    };
    for (auto fun : matrixScaleFunctions)
    {
        SPIRVGenerator::IntrinsicMap[fun] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            SPIRVResult lhs = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult rhs = LoadValueSPIRV(c, g, args[1]);
            uint32_t res = g->writer->MappedInstruction(OpMatrixTimesScalar, SPVWriter::Section::LocalFunction, returnType, lhs, rhs);
            return SPIRVResult(res, returnType, true);
        };
    }

    std::vector<Function*> matrixVectorTransformFunctions =
    {
        &Mat2x2_vectorTransformOperator
        , &Mat2x3_vectorTransformOperator
        , &Mat2x4_vectorTransformOperator
        , &Mat3x2_vectorTransformOperator
        , &Mat3x3_vectorTransformOperator
        , &Mat3x4_vectorTransformOperator
        , &Mat4x2_vectorTransformOperator
        , &Mat4x3_vectorTransformOperator
        , &Mat4x4_vectorTransformOperator
    };
    for (auto fun : matrixVectorTransformFunctions)
    {
        SPIRVGenerator::IntrinsicMap[fun] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            SPIRVResult lhs = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult rhs = LoadValueSPIRV(c, g, args[1]);
            uint32_t res = g->writer->MappedInstruction(OpMatrixTimesVector, SPVWriter::Section::LocalFunction, returnType, lhs, rhs);
            return SPIRVResult(res, returnType, true);
        };
    }

    std::vector<Function*> vectorScaleFunctions =
    {
        &Float4_scaleOperator
        , &Float3_scaleOperator
        , &Float2_scaleOperator
    };
    for (auto fun : vectorScaleFunctions)
    {
        SPIRVGenerator::IntrinsicMap[fun] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            SPIRVResult lhs = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult rhs = LoadValueSPIRV(c, g, args[1]);
            uint32_t res = g->writer->MappedInstruction(OpVectorTimesScalar, SPVWriter::Section::LocalFunction, returnType, lhs, rhs);
            return SPIRVResult(res, returnType, true);
        };
    }

    std::unordered_map<Function*, std::tuple<char, uint32_t, SPVOp>> intVectorScaleFunctions =
    {
        { &Int4_scaleOperator, { 'S', 4, OpIMul } }
        , { &Int3_scaleOperator, { 'S', 3, OpIMul } }
        , { &Int2_scaleOperator, { 'S', 2, OpIMul } }
        , { &UInt4_scaleOperator, { 'U', 4, OpIMul } }
        , { &UInt3_scaleOperator, { 'U', 3, OpIMul } }
        , { &UInt2_scaleOperator, { 'U', 2, OpIMul } }
    };
    for (auto fun : intVectorScaleFunctions)
    {
        SPIRVGenerator::IntrinsicMap[fun.first] = [op = std::get<0>(fun.second), num = std::get<1>(fun.second), inst = std::get<2>(fun.second)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            SPIRVResult splat = GenerateSplatCompositeSPIRV(c, g, returnType, num, args[1]);
            SPIRVResult lhs = LoadValueSPIRV(c, g, args[0]);
            TStr opStr = TStr::Compact("Op", op, "Mul");
            uint32_t res = g->writer->MappedInstruction(inst, SPVWriter::Section::LocalFunction, returnType, lhs, splat);
            return SPIRVResult(res, returnType, true);
        };
    }

    std::vector<Function*> vectorMatrixMulFunctions =
    {
        &Float4_matrix44Mul
        , &Float4_matrix43Mul
        , &Float4_matrix42Mul
        , &Float3_matrix34Mul
        , &Float3_matrix33Mul
        , &Float3_matrix32Mul
        , &Float2_matrix24Mul
        , &Float2_matrix23Mul
        , &Float2_matrix22Mul
    };
    for (auto fun : vectorMatrixMulFunctions)
    {
        SPIRVGenerator::IntrinsicMap[fun] = [fun](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);

            SPIRVResult lhs = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult rhs = LoadValueSPIRV(c, g, args[1]);
            uint32_t res = g->writer->MappedInstruction(OpVectorTimesMatrix, SPVWriter::Section::LocalFunction, returnType, lhs, rhs);
            return SPIRVResult(res, returnType, true);
        };
    }

    std::vector<Function*> arrayAccessFunctions =
    {
        &Float4_elementAccessOperatorInt,
        &Float4_elementAccessOperatorUInt,
        &Float3_elementAccessOperatorInt,
        &Float3_elementAccessOperatorUInt,
        &Float2_elementAccessOperatorInt,
        &Float2_elementAccessOperatorUInt,
        &Int4_elementAccessOperatorInt,
        &Int4_elementAccessOperatorUInt,
        &Int3_elementAccessOperatorInt,
        &Int3_elementAccessOperatorUInt,
        &Int2_elementAccessOperatorInt,
        &Int2_elementAccessOperatorUInt,
        &UInt4_elementAccessOperatorInt,
        &UInt4_elementAccessOperatorUInt,
        &UInt3_elementAccessOperatorInt,
        &UInt3_elementAccessOperatorUInt,
        &UInt2_elementAccessOperatorInt,
        &UInt2_elementAccessOperatorUInt,
        &Bool2_elementAccessOperatorInt,
        &Bool2_elementAccessOperatorUInt,
        &Bool3_elementAccessOperatorInt,
        &Bool3_elementAccessOperatorUInt,
        &Bool4_elementAccessOperatorInt,
        &Bool4_elementAccessOperatorUInt,
        &Mat2x2_elementAccessOperatorInt,
        &Mat2x2_elementAccessOperatorUInt,
        &Mat2x3_elementAccessOperatorInt,
        &Mat2x3_elementAccessOperatorUInt,
        &Mat2x4_elementAccessOperatorInt,
        &Mat2x4_elementAccessOperatorUInt,
        &Mat3x2_elementAccessOperatorInt,
        &Mat3x2_elementAccessOperatorUInt,
        &Mat3x3_elementAccessOperatorInt,
        &Mat3x3_elementAccessOperatorUInt,
        &Mat3x4_elementAccessOperatorInt,
        &Mat3x4_elementAccessOperatorUInt,
        &Mat4x2_elementAccessOperatorInt,
        &Mat4x2_elementAccessOperatorUInt,
        &Mat4x3_elementAccessOperatorInt,
        &Mat4x3_elementAccessOperatorUInt,
        &Mat4x4_elementAccessOperatorInt,
        &Mat4x4_elementAccessOperatorUInt,
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
    { &ty##_##fn##Operator, #inst, false, Op##inst }\
    , { &ty##_##fn##AssignOperator, #inst, true, Op##inst }

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
            uint32_t ret = g->writer->MappedInstruction(inst, SPVWriter::Section::LocalFunction, returnType, arg0, arg1);
            TStr opStr = TStr::Compact("Op", op);
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
    { &ty##_##fn##Operator, #inst, false, Op##inst }\
    
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
            uint32_t ret = g->writer->MappedInstruction(inst, SPVWriter::Section::LocalFunction, returnType, arg0, arg1);

            TStr opStr = TStr::Compact("Op", op);

            return SPIRVResult(ret, returnType, true);
        };
    }
   
#define MAKE_SCALAR_INTRINSICS(op)\
      { Intrinsics::op##_Float, 'F' }\
    , { Intrinsics::op##_Float2, 'F' }\
    , { Intrinsics::op##_Float3, 'F' }\
    , { Intrinsics::op##_Float4, 'F' }\
    , { Intrinsics::op##_Int, 'S' }\
    , { Intrinsics::op##_Int2, 'S' }\
    , { Intrinsics::op##_Int3, 'S' }\
    , { Intrinsics::op##_Int4, 'S' }\
    , { Intrinsics::op##_UInt, 'U' }\
    , { Intrinsics::op##_UInt2, 'U' }\
    , { Intrinsics::op##_UInt3, 'U' }\
    , { Intrinsics::op##_UInt4, 'U' }

#define MAKE_SCALAR_TYPED_INTRINSICS(op)\
      { Intrinsics::op##_Float, 'F', F##op }\
    , { Intrinsics::op##_Float2, 'F', F##op }\
    , { Intrinsics::op##_Float3, 'F', F##op }\
    , { Intrinsics::op##_Float4, 'F', F##op }\
    , { Intrinsics::op##_Int, 'S', S##op }\
    , { Intrinsics::op##_Int2, 'S', S##op }\
    , { Intrinsics::op##_Int3, 'S', S##op }\
    , { Intrinsics::op##_Int4, 'S', S##op }\
    , { Intrinsics::op##_UInt, 'U', U##op }\
    , { Intrinsics::op##_UInt2, 'U', U##op }\
    , { Intrinsics::op##_UInt3, 'U', U##op }\
    , { Intrinsics::op##_UInt4, 'U', U##op }

#define MAKE_EXPORT_INTRINSICS(op)\
      { Intrinsics::op##_Float, TypeCode::Float, 1 }\
    , { Intrinsics::op##_Float2, TypeCode::Float, 2 }\
    , { Intrinsics::op##_Float3, TypeCode::Float, 3 }\
    , { Intrinsics::op##_Float4, TypeCode::Float, 4 }\
    , { Intrinsics::op##_Int, TypeCode::Int, 1 }\
    , { Intrinsics::op##_Int2, TypeCode::Int, 2 }\
    , { Intrinsics::op##_Int3, TypeCode::Int, 3 }\
    , { Intrinsics::op##_Int4, TypeCode::Int, 4 }\
    , { Intrinsics::op##_UInt, TypeCode::UInt, 1 }\
    , { Intrinsics::op##_UInt2, TypeCode::UInt, 2 }\
    , { Intrinsics::op##_UInt3, TypeCode::UInt, 3 }\
    , { Intrinsics::op##_UInt4, TypeCode::UInt, 4 }

#define MAKE_FLOAT_INTRINSICS(op)\
      { Intrinsics::op##_Float, 'F' }\
    , { Intrinsics::op##_Float2, 'F' }\
    , { Intrinsics::op##_Float3, 'F' }\
    , { Intrinsics::op##_Float4, 'F' }

#define MAKE_BOOL_INTRINSICS(op)\
    { Intrinsics::op##_Bool, 'B' }\
    , { Intrinsics::op##_Bool2, 'B' }\
    , { Intrinsics::op##_Bool3, 'B' }\
    , { Intrinsics::op##_Bool4, 'B' }

#define MAKE_FLOAT_VEC_INTRINSICS(op)\
    { Intrinsics::op##_Float2, 'F' }\
    , { Intrinsics::op##_Float3, 'F' }\
    , { Intrinsics::op##_Float4, 'F' }

#define MAKE_MAT_INTRINSICS(op)\
    { Intrinsics::op##_Mat2x2, 'F' }\
    , { Intrinsics::op##_Mat2x3, 'F' }\
    , { Intrinsics::op##_Mat2x4, 'F' }\
    , { Intrinsics::op##_Mat3x2, 'F' }\
    , { Intrinsics::op##_Mat3x3, 'F' }\
    , { Intrinsics::op##_Mat3x4, 'F' }\
    , { Intrinsics::op##_Mat4x2, 'F' }\
    , { Intrinsics::op##_Mat4x3, 'F' }\
    , { Intrinsics::op##_Mat4x4, 'F' }

#define MAKE_SIGN_INTRINSICS(op)\
      { Intrinsics::op##_Float, 'F', F##op }\
    , { Intrinsics::op##_Float2, 'F', F##op }\
    , { Intrinsics::op##_Float3, 'F', F##op }\
    , { Intrinsics::op##_Float4, 'F', F##op }\
    , { Intrinsics::op##_Int, 'S', S##op }\
    , { Intrinsics::op##_Int2, 'S', S##op }\
    , { Intrinsics::op##_Int3, 'S', S##op }\
    , { Intrinsics::op##_Int4, 'S', S##op }

#define MAKE_FLOAT_INTRINSICS_VEC(op)\
      { Intrinsics::op##_Float, 'F', 1 }\
    , { Intrinsics::op##_Float2, 'F', 2 }\
    , { Intrinsics::op##_Float3, 'F', 3 }\
    , { Intrinsics::op##_Float4, 'F', 4 }


#define MAKE_INT_INTRINSICS(op)\
      { Intrinsics::op##_Int, 'S' }\
    , { Intrinsics::op##_Int2, 'S' }\
    , { Intrinsics::op##_Int3, 'S' }\
    , { Intrinsics::op##_Int4, 'S' }\
    , { Intrinsics::op##_UInt, 'U' }\
    , { Intrinsics::op##_UInt2, 'U' }\
    , { Intrinsics::op##_UInt3, 'U' }\
    , { Intrinsics::op##_UInt4, 'U' }
    
#define MAKE_SINGLE_COMPONENT_INTRINSICS(op)\
    { Intrinsics::op##_Float, 'F' }\
    , { Intrinsics::op##_Int, 'S' }\
    , { Intrinsics::op##_UInt, 'U' }\

#define MAKE_EXT_FLOAT_INTRINSICS(op, fun)\
    { Intrinsics::op##_Float, #fun, fun }\
    , { Intrinsics::op##_Float2, #fun, fun }\
    , { Intrinsics::op##_Float3, #fun, fun }\
    , { Intrinsics::op##_Float4, #fun, fun }

#define MAKE_EXT_INT_INTRINSICS(op, fun)\
    { Intrinsics::op##_Int, #fun, fun }\
    , { Intrinsics::op##_Int2, #fun, fun }\
    , { Intrinsics::op##_Int3, #fun, fun }\
    , { Intrinsics::op##_Int4, #fun, fun }

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
            uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), en, base, exp);
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
            uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), en, arg);
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
            uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), en, arg1, arg2);
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
            uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), Fma, arg0, arg1, arg2);
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
            uint32_t ret = g->writer->MappedInstruction(OpDot, SPVWriter::Section::LocalFunction, returnType, arg0, arg1);
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
            SPIRVResult arg0 = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult arg1 = LoadValueSPIRV(c, g, args[1]);
            uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), Reflect, arg0, arg1);
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
            SPIRVResult arg0 = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult arg1 = LoadValueSPIRV(c, g, args[1]);
            SPIRVResult arg2 = LoadValueSPIRV(c, g, args[2]);
            uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), Refract, arg0, arg1, arg2);
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
            SPIRVResult arg0 = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult arg1 = LoadValueSPIRV(c, g, args[1]);
            uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), Cross, arg0, arg1);
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
            uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), Normalize, arg);
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
            uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), Length, arg);
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
            uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), Distance, arg0, arg1);
            return SPIRVResult(ret, returnType, true);
        };
    }
    
    std::vector<std::tuple<Function*, const char, SPVEnum>> minIntrinsics =
    {
        MAKE_SCALAR_TYPED_INTRINSICS(Min)
    };
    for (auto fun : minIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [fun](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            auto [fn, op, inst] = fun;
            SPIRVResult arg0 = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult arg1 = LoadValueSPIRV(c, g, args[1]);
            uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), inst, arg0, arg1);
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char, SPVEnum>> maxIntrinsics =
    {
        MAKE_SCALAR_TYPED_INTRINSICS(Max)
    };
    for (auto fun : maxIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [fun](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 2);
            auto [fn, op, inst] = fun;
            SPIRVResult arg0 = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult arg1 = LoadValueSPIRV(c, g, args[1]);
            uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), inst, arg0, arg1);
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char, SPVEnum>> clampIntrinsics =
    {
        MAKE_SCALAR_TYPED_INTRINSICS(Clamp)
    };
    for (auto fun : clampIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [fun](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 3);
            auto [fn, op, inst] = fun;
            SPIRVResult arg0 = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult arg1 = LoadValueSPIRV(c, g, args[1]);
            SPIRVResult arg2 = LoadValueSPIRV(c, g, args[2]);
            uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), inst, arg0, arg1, arg2);

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
            uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), FMix, arg0, arg1, arg2);
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
            uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), Step, arg0, arg1);
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
            uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), SmoothStep, arg0, arg1, arg2);
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char, uint32_t>> saturateIntrinsics =
    {
        MAKE_FLOAT_INTRINSICS_VEC(Saturate)
    };
    for (auto fun : saturateIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [fun](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 1);

            auto [fn, op, size] = fun;
            uint32_t ftype = GeneratePODTypeSPIRV(c, g, TypeCode::Float);
            SPIRVResult min = GenerateConstantSPIRV(c, g, ConstantCreationInfo::Float(0));
            SPIRVResult max = GenerateConstantSPIRV(c, g, ConstantCreationInfo::Float(1));
            
            // If vector, make clamp constant composite
            if (size > 1)
            {
                min = GenerateSplatCompositeSPIRV(c, g, returnType, size, min);
                max = GenerateSplatCompositeSPIRV(c, g, returnType, size, max);
            }
            
            SPIRVResult arg = LoadValueSPIRV(c, g, args[0]);
            uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), FClamp, arg, min, max);
            
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char, SPVEnum>> absIntrinsics =
    {
        MAKE_SIGN_INTRINSICS(Abs)
    };
    for (auto fun : absIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [fun](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 1);
            SPIRVResult arg = LoadValueSPIRV(c, g, args[0]);

            auto [fn, op, inst] = fun;
            uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), inst, arg);

            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char, SPVEnum>> signIntrinsics =
    {
        MAKE_SIGN_INTRINSICS(Sign)
    };
    for (auto fun : signIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [fun](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            assert(args.size() == 1);
            SPIRVResult arg = LoadValueSPIRV(c, g, args[0]);
            
            auto [fn, op, inst] = fun;
            uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), inst, arg);
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
            uint32_t ret = g->writer->MappedInstruction(OpDPdx, SPVWriter::Section::LocalFunction, returnType, arg);
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
            uint32_t ret = g->writer->MappedInstruction(OpDPdy, SPVWriter::Section::LocalFunction, returnType, arg);
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
            uint32_t ret = g->writer->MappedInstruction(OpFwidth, SPVWriter::Section::LocalFunction, returnType, arg);
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
            uint32_t ret = g->writer->MappedInstruction(OpBitcast, SPVWriter::Section::LocalFunction, returnType, arg);
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
            uint32_t ret = g->writer->MappedInstruction(OpAll, SPVWriter::Section::LocalFunction, returnType, arg);
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
            uint32_t ret = g->writer->MappedInstruction(OpAny, SPVWriter::Section::LocalFunction, returnType, arg);
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
            uint32_t ret = g->writer->MappedInstruction(OpTranspose, SPVWriter::Section::LocalFunction, returnType, arg);
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
            uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), MatrixInverse, arg);
            return SPIRVResult(ret, returnType, true);
        };
    }
    
    SPIRVGenerator::IntrinsicMap[Intrinsics::GetOutputLayer] = [](const Compiler* c, SPIRVGenerator * g, uint32_t returnType, const std::vector<SPIRVResult>&args) -> SPIRVResult
    {
        g->writer->Capability(Capabilities::ShaderLayer);
        uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt, 1);
        uint32_t typePtr = GPULang::AddType(g, TStr("ptr_u32_Input"), OpTypePointer, VariableStorage::Input, SPVArg(baseType));
        uint32_t ret = GPULang::AddSymbol(g, TStr("gplOutputLayer"), SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Input);
        g->writer->Decorate(SPVArg{ret}, Decorations::BuiltIn, Builtins::Layer);
        
        g->interfaceVariables.Insert(ret);
        SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);
        res.parentTypes.push_back(baseType);
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::SetOutputLayer] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->writer->Capability(Capabilities::ShaderLayer);
        uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt, 1);
        uint32_t typePtr = GPULang::AddType(g, TStr("ptr_u32_Output"), OpTypePointer, VariableStorage::Output, SPVArg(baseType));
        uint32_t ret = GPULang::AddSymbol(g, TStr("gplOutputLayer"), SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Output);
        g->writer->Decorate(SPVArg{ret}, Decorations::BuiltIn, Builtins::Layer);
        
        g->interfaceVariables.Insert(ret);
        g->writer->Instruction(OpStore, SPVWriter::Section::LocalFunction, SPVArg{ ret }, args[0]);
        return SPIRVResult::Invalid();
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::GetOutputViewport] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult 
    {
        g->writer->Capability(Capabilities::ShaderViewportIndex);
        uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt, 1);

        uint32_t typePtr = GPULang::AddType(g, TStr("ptr_u32_Input"), OpTypePointer, VariableStorage::Input, SPVArg(baseType));
        uint32_t ret = GPULang::AddSymbol(g, TStr("gplInputViewport"), SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Input);
        g->writer->Decorate(SPVArg{ret}, Decorations::BuiltIn, Builtins::ViewportIndex);
        
        g->interfaceVariables.Insert(ret);
        SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);
        res.parentTypes.push_back(baseType);
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::SetOutputViewport] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->writer->Capability(Capabilities::ShaderViewportIndex);
        uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt, 1);

        uint32_t typePtr = GPULang::AddType(g, TStr("ptr_u32_Output"), OpTypePointer, VariableStorage::Output, SPVArg(baseType));
        uint32_t ret = GPULang::AddSymbol(g, TStr("gplOutputViewport"), SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Output);
        g->writer->Decorate(SPVArg{ret}, Decorations::BuiltIn, Builtins::ViewportIndex);
        
        g->interfaceVariables.Insert(ret);
        g->writer->Instruction(OpStore, SPVWriter::Section::LocalFunction, SPVArg{ ret }, args[0]);
        return SPIRVResult::Invalid();
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::ExportVertexCoordinates] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->writer->Capability(Capabilities::Shader);
        uint32_t vecType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, 4);

        uint32_t typePtr = GPULang::AddType(g, TStr("ptr_f32x4_Output"), OpTypePointer, VariableStorage::Output, SPVArg(vecType));
        uint32_t ret = GPULang::AddSymbol(g, TStr("gplVertexCoordinates"), SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Output);
        g->writer->Decorate(SPVArg{ret}, Decorations::BuiltIn, Builtins::Position);
        
        g->interfaceVariables.Insert(ret);
        SPIRVResult loaded = LoadValueSPIRV(c, g, args[0]);
        g->writer->Instruction(OpStore, SPVWriter::Section::LocalFunction, SPVArg{ ret }, loaded);
        return SPIRVResult::Invalid();
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::ExportVertex] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->writer->Capability(Capabilities::Geometry);
        g->writer->Instruction(OpEmitVertex, SPVWriter::Section::LocalFunction);
        return SPIRVResult::Invalid();
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::ExportPrimitive] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->writer->Capability(Capabilities::Geometry);
        g->writer->Instruction(OpEndPrimitive, SPVWriter::Section::LocalFunction);
        return SPIRVResult::Invalid();
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::GetVertexIndex] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->writer->Capability(Capabilities::Shader);
        uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt, 1);

        uint32_t typePtr = GPULang::AddType(g, TStr("ptr_u32_Input"), OpTypePointer, VariableStorage::Input, SPVArg(baseType));
        uint32_t ret = GPULang::AddSymbol(g, TStr("gplVertexIndex"), SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Input);
        g->writer->Decorate(SPVArg{ret}, Decorations::BuiltIn, Builtins::VertexId);
        
        g->interfaceVariables.Insert(ret);
        SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);
        res.parentTypes.push_back(baseType);
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::GetInstanceIndex] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->writer->Capability(Capabilities::Shader);
        uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt, 1);

        uint32_t typePtr = GPULang::AddType(g, TStr("ptr_u32_Input"), OpTypePointer, VariableStorage::Input, SPVArg(baseType));
        uint32_t ret = GPULang::AddSymbol(g, TStr("gplInstanceIndex"), SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Input);
        g->writer->Decorate(SPVArg{ret}, Decorations::BuiltIn, Builtins::InstanceId);
        
        g->interfaceVariables.Insert(ret);
        SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);
        res.parentTypes.push_back(baseType);
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::GetPixelCoordinates] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->writer->Capability(Capabilities::Shader);
        uint32_t vecType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, 4);

        uint32_t typePtr = GPULang::AddType(g, TStr("ptr_f32x4_Input"), OpTypePointer, VariableStorage::Input, SPVArg(vecType));
        uint32_t ret = GPULang::AddSymbol(g, TStr("gplPixelCoordinates"), SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Input);
        g->writer->Decorate(SPVArg{ret}, Decorations::BuiltIn, Builtins::FragCoord);
        
        g->interfaceVariables.Insert(ret);
        SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);
        res.parentTypes.push_back(vecType);
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::SetPixelDepth] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->writer->Capability(Capabilities::Shader);
        uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, 1);

        uint32_t typePtr = GPULang::AddType(g, TStr("ptr_f32_Output"), OpTypePointer, VariableStorage::Output, SPVArg(baseType));
        uint32_t ret = GPULang::AddSymbol(g, TStr("gplSetPixelDepth"), SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Output);
        g->writer->Decorate(SPVArg{ret}, Decorations::BuiltIn, Builtins::FragDepth);
        
        g->interfaceVariables.Insert(ret);
        SPIRVResult loaded = LoadValueSPIRV(c, g, args[0]);
        
        g->writer->Instruction(OpStore, SPVWriter::Section::LocalFunction, SPVArg{ ret }, loaded);
        return SPIRVResult::Invalid();
    };

    std::vector<std::tuple<Function*, TypeCode, uint32_t>> colorExportIntrinsics =
    {
        MAKE_EXPORT_INTRINSICS(ExportColor)
    };
    for (auto fun : colorExportIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [baseTy = std::get<1>(fun), vecSize = std::get<2>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            uint32_t baseType = GeneratePODTypeSPIRV(c, g, baseTy, vecSize);
            std::string name = Type::CodeToString(baseTy).c_str();

            TStr typeSymbolName = vecSize > 1 ? TStr::Compact("ptr_", name, "x", vecSize, "_Output") : TStr::Compact("ptr_", name);

            uint32_t typePtr = GPULang::AddType(g, typeSymbolName, OpTypePointer, VariableStorage::Output, SPVArg(baseType));
            uint32_t ret = GPULang::AddSymbol(g, TStr("gplExportColor", args[1].literalValue.i), SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Output);
            g->writer->Decorate(SPVArg{ret}, Decorations::Index, args[1].literalValue.i);
            g->writer->Decorate(SPVArg{ret}, Decorations::Location, args[1].literalValue.i);
           
            assert(args[1].isLiteral);
            g->interfaceVariables.Insert(ret);
            
            SPIRVResult loaded = LoadValueSPIRV(c, g, args[0]);
            g->writer->Instruction(OpStore, SPVWriter::Section::LocalFunction, SPVArg{ ret }, loaded);
            return SPIRVResult::Invalid();
        };
    }

    SPIRVGenerator::IntrinsicMap[Intrinsics::GetLocalInvocationIndices] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
        uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt, 3);
        uint32_t typePtr = GPULang::AddType(g, "ptr_u32x3_Input", OpTypePointer, VariableStorage::Input, SPVArg(baseType));
        uint32_t ret = GPULang::AddSymbol(g, "gplLocalInvocationIndices", SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Input);
        g->writer->Decorate(SPVArg{ret}, Decorations::BuiltIn, Builtins::LocalInvocationId);

        Type::FullType fullType = Type::FullType{ ConstantString("u32x3") };
        fullType.AddModifier(Type::FullType::Modifier::Pointer);
        Type* typeSymbol = c->GetType(fullType);
        SPIRVResult type = GenerateTypeSPIRV(c, g, fullType, typeSymbol, SPIRVResult::Storage::Input);
        g->interfaceVariables.Insert(ret);
        SPIRVResult res = type;
        res.name = ret;
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::GetIndexInWorkGroup] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
    
        uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt, 3);
        uint32_t typePtr = GPULang::AddType(g, "ptr_u32_Input", OpTypePointer, VariableStorage::Input, SPVArg(baseType));
        uint32_t ret = GPULang::AddSymbol(g, "gplGetIndexInWorkGroup", SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Input);
        g->writer->Decorate(SPVArg{ret}, Decorations::BuiltIn, Builtins::LocalInvocationIndex);
        
        Type::FullType fullType = Type::FullType{ ConstantString("u32") };
        fullType.AddModifier(Type::FullType::Modifier::Pointer);
        Type* typeSymbol = c->GetType(fullType);
        SPIRVResult type = GenerateTypeSPIRV(c, g, fullType, typeSymbol, SPIRVResult::Storage::Input);
        g->interfaceVariables.Insert(ret);
        SPIRVResult res = type;
        res.name = ret;
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::GetGlobalInvocationIndices] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {

        uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt, 3);
        uint32_t typePtr = GPULang::AddType(g, "ptr_u32x3_Input", OpTypePointer, VariableStorage::Input, SPVArg(baseType));
        uint32_t ret = GPULang::AddSymbol(g, "gplGlobalInvocationIndices", SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Input);
        g->writer->Decorate(SPVArg{ret}, Decorations::BuiltIn, Builtins::GlobalInvocationId);
        
        Type::FullType fullType = Type::FullType{ ConstantString("u32x3") };
        fullType.AddModifier(Type::FullType::Modifier::Pointer);
        Type* typeSymbol = c->GetType(fullType);
        SPIRVResult type = GenerateTypeSPIRV(c, g, fullType, typeSymbol, SPIRVResult::Storage::Input);
        g->interfaceVariables.Insert(ret);
        SPIRVResult res = type;
        res.name = ret;
        return res;
    };


    SPIRVGenerator::IntrinsicMap[Intrinsics::GetWorkGroupIndices] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
    
        uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt, 3);
        uint32_t typePtr = GPULang::AddType(g, "ptr_u32x3_Input", OpTypePointer, VariableStorage::Input, SPVArg(baseType));
        uint32_t ret = GPULang::AddSymbol(g, "gplWorkGroupIndices", SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Input);
        g->writer->Decorate(SPVArg{ret}, Decorations::BuiltIn, Builtins::WorkgroupId);
        
        Type::FullType fullType = Type::FullType{ ConstantString("u32x3") };
        fullType.AddModifier(Type::FullType::Modifier::Pointer);
        Type* typeSymbol = c->GetType(fullType);
        SPIRVResult type = GenerateTypeSPIRV(c, g, fullType, typeSymbol, SPIRVResult::Storage::Input);
        g->interfaceVariables.Insert(ret);
        SPIRVResult res = type;
        res.name = ret;
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::GetWorkGroupDimensions] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {

        uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt, 3);
        uint32_t typePtr = GPULang::AddType(g, "ptr_u32x3_Input", OpTypePointer, VariableStorage::Input, SPVArg(baseType));
        uint32_t ret = GPULang::AddSymbol(g, "gplWorkGroupDimensions", SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Input);
        g->writer->Decorate(SPVArg{ret}, Decorations::BuiltIn, Builtins::WorkgroupSize);
        
        Type::FullType fullType = Type::FullType{ ConstantString("u32x3") };
        fullType.AddModifier(Type::FullType::Modifier::Pointer);
        Type* typeSymbol = c->GetType(fullType);
        SPIRVResult type = GenerateTypeSPIRV(c, g, fullType, typeSymbol, SPIRVResult::Storage::Input);
        g->interfaceVariables.Insert(ret);
        SPIRVResult res = type;
        type.name = ret;
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::GetSubgroupId] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->writer->Capability(Capabilities::GroupNonUniform);
        uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt, 3);
        uint32_t typePtr = GPULang::AddType(g, "ptr_u32x3_Input", OpTypePointer, VariableStorage::Input, SPVArg(baseType));
        uint32_t ret = GPULang::AddSymbol(g, "gplSubgroupId", SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Input);
        g->writer->Decorate(SPVArg{ret}, Decorations::BuiltIn, Builtins::SubgroupId);
        
        Type::FullType fullType = Type::FullType{ ConstantString("u32x3") };
        fullType.AddModifier(Type::FullType::Modifier::Pointer);
        Type* typeSymbol = c->GetType(fullType);
        SPIRVResult type = GenerateTypeSPIRV(c, g, fullType, typeSymbol, SPIRVResult::Storage::Input);
        g->interfaceVariables.Insert(ret);
        SPIRVResult res = type;
        res.name = ret;
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::GetSubgroupSize] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->writer->Capability(Capabilities::GroupNonUniform);
        uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt, 3);
        uint32_t typePtr = GPULang::AddType(g, "ptr_u32x3_Input", OpTypePointer, VariableStorage::Input, SPVArg(baseType));
        uint32_t ret = GPULang::AddSymbol(g, "gplSubgroupSize", SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Input);
        g->writer->Decorate(SPVArg{ret}, Decorations::BuiltIn, Builtins::SubgroupSize);
        
        Type::FullType fullType = Type::FullType{ ConstantString("u32x3") };
        fullType.AddModifier(Type::FullType::Modifier::Pointer);
        Type* typeSymbol = c->GetType(fullType);
        SPIRVResult type = GenerateTypeSPIRV(c, g, fullType, typeSymbol, SPIRVResult::Storage::Input);
        g->interfaceVariables.Insert(ret);
        SPIRVResult res = type;
        res.name = ret;
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::GetNumSubgroups] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->writer->Capability(Capabilities::GroupNonUniform);
        uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt, 3);
        uint32_t typePtr = GPULang::AddType(g, "ptr_u32x3_Input", OpTypePointer, VariableStorage::Input, SPVArg(baseType));
        uint32_t ret = GPULang::AddSymbol(g, "gplNumSubgroups", SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Input);
        g->writer->Decorate(SPVArg{ret}, Decorations::BuiltIn, Builtins::NumSubgroups);
        
        Type::FullType fullType = Type::FullType{ ConstantString("u32x3") };
        fullType.AddModifier(Type::FullType::Modifier::Pointer);
        Type* typeSymbol = c->GetType(fullType);
        SPIRVResult type = GenerateTypeSPIRV(c, g, fullType, typeSymbol, SPIRVResult::Storage::Input);
        g->interfaceVariables.Insert(ret);
        SPIRVResult res = type;
        res.name = ret;
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::GetSubgroupLocalInvocationMask] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->writer->Capability(Capabilities::GroupNonUniform);
        uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt, 4);
        uint32_t typePtr = GPULang::AddType(g, "ptr_u32x4_Input", OpTypePointer, VariableStorage::Input, SPVArg(baseType));
        uint32_t ret = GPULang::AddSymbol(g, "gplGlobalInvocationIndices", SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Input);
        g->writer->Decorate(SPVArg{ret}, Decorations::BuiltIn, Builtins::SubgroupEqMask);
        
        Type::FullType fullType = Type::FullType{ ConstantString("u32x4") };
        fullType.AddModifier(Type::FullType::Modifier::Pointer);
        Type* typeSymbol = c->GetType(fullType);
        SPIRVResult type = GenerateTypeSPIRV(c, g, fullType, typeSymbol, SPIRVResult::Storage::Input);
        g->interfaceVariables.Insert(ret);
        SPIRVResult res = type;
        res.name = ret;
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::GetSubgroupLocalInvocationAndLowerMask] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->writer->Capability(Capabilities::GroupNonUniform);
        uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt, 4);
        uint32_t typePtr = GPULang::AddType(g, "ptr_u32x4_Input", OpTypePointer, VariableStorage::Input, SPVArg(baseType));
        uint32_t ret = GPULang::AddSymbol(g, "gplGlobalInvocationIndices", SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Input);
        g->writer->Decorate(SPVArg{ret}, Decorations::BuiltIn, Builtins::SubgroupLeMask);
        
        Type::FullType fullType = Type::FullType{ ConstantString("u32x4") };
        fullType.AddModifier(Type::FullType::Modifier::Pointer);
        Type* typeSymbol = c->GetType(fullType);
        SPIRVResult type = GenerateTypeSPIRV(c, g, fullType, typeSymbol, SPIRVResult::Storage::Input);
        g->interfaceVariables.Insert(ret);
        SPIRVResult res = type;
        res.name = ret;
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::GetSubgroupLowerMask] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->writer->Capability(Capabilities::GroupNonUniform);
        uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt, 4);
        uint32_t typePtr = GPULang::AddType(g, "ptr_u32x4_Input", OpTypePointer, VariableStorage::Input, SPVArg(baseType));
        uint32_t ret = GPULang::AddSymbol(g, "gplGlobalInvocationIndices", SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Input);
        g->writer->Decorate(SPVArg{ret}, Decorations::BuiltIn, Builtins::SubgroupLtMask);
        
        Type::FullType fullType = Type::FullType{ ConstantString("u32x4") };
        fullType.AddModifier(Type::FullType::Modifier::Pointer);
        Type* typeSymbol = c->GetType(fullType);
        SPIRVResult type = GenerateTypeSPIRV(c, g, fullType, typeSymbol, SPIRVResult::Storage::Input);
        g->interfaceVariables.Insert(ret);
        SPIRVResult res = type;
        res.name = ret;
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::GetSubgroupLocalInvocationAndGreaterMask] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->writer->Capability(Capabilities::GroupNonUniform);
        uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt, 4);
        uint32_t typePtr = GPULang::AddType(g, "ptr_u32x4_Input", OpTypePointer, VariableStorage::Input, SPVArg(baseType));
        uint32_t ret = GPULang::AddSymbol(g, "gplGlobalInvocationIndices", SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Input);
        g->writer->Decorate(SPVArg{ret}, Decorations::BuiltIn, Builtins::SubgroupGeMask);
        
        Type::FullType fullType = Type::FullType{ ConstantString("u32x4") };
        fullType.AddModifier(Type::FullType::Modifier::Pointer);
        Type* typeSymbol = c->GetType(fullType);
        SPIRVResult type = GenerateTypeSPIRV(c, g, fullType, typeSymbol, SPIRVResult::Storage::Input);
        g->interfaceVariables.Insert(ret);
        SPIRVResult res = type;
        res.name = ret;
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::GetSubgroupGreaterMask] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->writer->Capability(Capabilities::GroupNonUniform);
        uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt, 4);
        uint32_t typePtr = GPULang::AddType(g, "ptr_u32x4_Input", OpTypePointer, VariableStorage::Input, SPVArg(baseType));
        uint32_t ret = GPULang::AddSymbol(g, "gplGlobalInvocationIndices", SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Input);
        g->writer->Decorate(SPVArg{ret}, Decorations::BuiltIn, Builtins::SubgroupGtMask);
        
        Type::FullType fullType = Type::FullType{ ConstantString("u32x4") };
        fullType.AddModifier(Type::FullType::Modifier::Pointer);
        Type* typeSymbol = c->GetType(fullType);
        SPIRVResult type = GenerateTypeSPIRV(c, g, fullType, typeSymbol, SPIRVResult::Storage::Input);
        g->interfaceVariables.Insert(ret);
        SPIRVResult res = type;
        res.name = ret;
        return res;
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::SubgroupFirstInvocation] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult 
    {
        g->writer->Capability(Capabilities::GroupNonUniform);
        uint32_t ret = g->writer->MappedInstruction(OpGroupNonUniformElect, SPVWriter::Section::LocalFunction, returnType, ExecutionScopes::Subgroup);
        return SPIRVResult(ret, returnType);
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::SubgroupRead] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult 
    {
        g->writer->Capability(Capabilities::GroupNonUniformBallot);
        uint32_t ret = g->writer->MappedInstruction(OpGroupNonUniformBroadcastFirst, SPVWriter::Section::LocalFunction, returnType, ExecutionScopes::Subgroup, args[0]);
        return SPIRVResult(ret, returnType);
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::SubgroupBallot] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult 
    {
        g->writer->Capability(Capabilities::GroupNonUniformBallot);
        uint32_t ret = g->writer->MappedInstruction(OpGroupNonUniformBallot, SPVWriter::Section::LocalFunction, returnType, ExecutionScopes::Subgroup, args[0]);
        return SPIRVResult(ret, returnType);
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::SubgroupInverseBallot] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult 
    {
        g->writer->Capability(Capabilities::GroupNonUniformBallot);
        uint32_t ret = g->writer->MappedInstruction(OpGroupNonUniformInverseBallot, SPVWriter::Section::LocalFunction, returnType, ExecutionScopes::Subgroup, args[0]);
        return SPIRVResult(ret, returnType);
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::SubgroupBallotBitCount] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult 
    {
        g->writer->Capability(Capabilities::GroupNonUniformBallot);
        uint32_t ret = g->writer->MappedInstruction(OpGroupNonUniformBallotBitCount, SPVWriter::Section::LocalFunction, returnType, ExecutionScopes::Subgroup, Subgroup::Reduce, args[0]);
        return SPIRVResult(ret, returnType);
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::SubgroupBallotBit] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult 
    {
        g->writer->Capability(Capabilities::GroupNonUniformBallot);
        uint32_t ret = g->writer->MappedInstruction(OpGroupNonUniformBallotBitExtract, SPVWriter::Section::LocalFunction, returnType, ExecutionScopes::Subgroup, args[0], args[1]);
        return SPIRVResult(ret, returnType);
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::SubgroupBallotFirstOne] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult 
    {
        g->writer->Capability(Capabilities::GroupNonUniformBallot);
        uint32_t ret = g->writer->MappedInstruction(OpGroupNonUniformBallotFindLSB, SPVWriter::Section::LocalFunction, returnType, ExecutionScopes::Subgroup, args[0]);
        return SPIRVResult(ret, returnType);
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::SubgroupBallotLastOne] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult 
    {
        g->writer->Capability(Capabilities::GroupNonUniformBallot);
        uint32_t ret = g->writer->MappedInstruction(OpGroupNonUniformBallotFindMSB, SPVWriter::Section::LocalFunction, returnType, ExecutionScopes::Subgroup, args[0]);
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
            g->writer->Capability(Capabilities::GroupNonUniformQuad);
            SPIRVResult direction = GenerateConstantSPIRV(c, g, ConstantCreationInfo::Int(2));
            uint32_t ret = g->writer->MappedInstruction(OpGroupNonUniformQuadSwap, SPVWriter::Section::LocalFunction, returnType, ExecutionScopes::Subgroup, args[0], direction);
            
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
            g->writer->Capability(Capabilities::GroupNonUniformQuad);
            SPIRVResult direction = GenerateConstantSPIRV(c, g, ConstantCreationInfo::Int(1));
            uint32_t ret = g->writer->MappedInstruction(OpGroupNonUniformQuadSwap, SPVWriter::Section::LocalFunction, returnType, ExecutionScopes::Subgroup, args[0], direction);
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
            g->writer->Capability(Capabilities::GroupNonUniformQuad);
            SPIRVResult direction = GenerateConstantSPIRV(c, g, ConstantCreationInfo::Int(0));
            uint32_t ret = g->writer->MappedInstruction(OpGroupNonUniformQuadSwap, SPVWriter::Section::LocalFunction, returnType, ExecutionScopes::Subgroup, args[0], direction);
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
    { Intrinsics::Atomic##op##_Float, #op, OpAtomic##op }\
    , { Intrinsics::Atomic##op##_Int, #op, OpAtomic##op }\
    , { Intrinsics::Atomic##op##_UInt, #op, OpAtomic##op }\

#define MAKE_ATOMIC_INT_INTRINSICS(op)\
    { Intrinsics::Atomic##op##_Int, #op, OpAtomic##op }\
    , { Intrinsics::Atomic##op##_UInt, #op, OpAtomic##op }\

#define MAKE_ATOMIC_INT_TYPED_INTRINSICS(op)\
    { Intrinsics::Atomic##op##_Int, #op, OpAtomicI##op }\
    , { Intrinsics::Atomic##op##_UInt, #op, OpAtomicI##op }\

#define MAKE_ATOMIC_SIGNED_INT_INTRINSICS(op)\
    { Intrinsics::Atomic##op##_Int, 'S', #op, OpAtomicS##op }\
    , { Intrinsics::Atomic##op##_UInt, 'U', #op, OpAtomicU##op }\
    
    std::vector<std::tuple<Function*, const char*, SPVOp>> atomicLoadIntrinsics =
    {
        MAKE_ATOMIC_SCALAR_INTRINSICS(Load)
    };
    for (auto fun : atomicLoadIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] =  [fun](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            assert(args.size() == 2);
            assert(args[1].isLiteral);

            auto [fn, op, inst] = fun;
            
            uint32_t scope = ScopeToAtomicScope(args[0].scope);
            uint32_t semantics = SemanticsTable[args[1].literalValue.ui];
            semantics |= ScopeToMemorySemantics(args[0].scope);

            TStr opStr = TStr::Compact("OpAtomic", op);
            
            SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(scope));
            SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(semantics));
            uint32_t ret = g->writer->MappedInstruction(inst, SPVWriter::Section::LocalFunction, returnType, args[0], scopeId, semanticsId);
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char*, SPVOp>> atomicStoreIntrinsics =
    {
        MAKE_ATOMIC_SCALAR_INTRINSICS(Store)
    };
    for (auto fun : atomicStoreIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] =  [fun](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            assert(args.size() == 3);
            assert(args[2].isLiteral);
            auto [fn, op, inst] = fun;

            uint32_t scope = ScopeToAtomicScope(args[0].scope);
            uint32_t semantics = SemanticsTable[args[2].literalValue.ui];
            semantics |= ScopeToMemorySemantics(args[0].scope);

            SPIRVResult valueLoaded = LoadValueSPIRV(c, g, args[1]);
            SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(scope));
            SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(semantics));
            uint32_t ret = g->writer->MappedInstruction(inst, SPVWriter::Section::LocalFunction, returnType, args[0], scopeId, semanticsId);
            return SPIRVResult(ret, returnType);
        };
    }

    std::vector<std::tuple<Function*, const char*, SPVOp>> atomicExchangeIntrinsics =
    {
        MAKE_ATOMIC_SCALAR_INTRINSICS(Exchange)
    };
    for (auto fun : atomicExchangeIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] =  [fun](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            assert(args.size() == 3);
            assert(args[1].isLiteral);
            auto [fn, op, inst] = fun;

            uint32_t scope = ScopeToAtomicScope(args[0].scope);
            uint32_t semantics = SemanticsTable[args[2].literalValue.ui];
            semantics |= ScopeToMemorySemantics(args[0].scope);

            SPIRVResult valueLoaded = LoadValueSPIRV(c, g, args[1]);
            SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(scope));
            SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(semantics));
            uint32_t ret = g->writer->MappedInstruction(inst, SPVWriter::Section::LocalFunction, returnType, args[0], scopeId, semanticsId, valueLoaded);

            return SPIRVResult(ret, returnType, true);
        };
    }
    
    std::vector<std::tuple<Function*, const char*, SPVOp>> atomicCompareExchangeIntrinsics =
    {
        MAKE_ATOMIC_INT_INTRINSICS(CompareExchange)
    };
    for (auto fun : atomicCompareExchangeIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] =  [fun](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            assert(args.size() == 4);
            assert(args[1].isLiteral);
            auto [fn, op, inst] = fun;

            uint32_t scope = ScopeToAtomicScope(args[0].scope);
            uint32_t semantics = SemanticsTable[args[3].literalValue.ui];
            semantics |= ScopeToMemorySemantics(args[0].scope);

            SPIRVResult value1Loaded = LoadValueSPIRV(c, g, args[1]);
            SPIRVResult value2Loaded = LoadValueSPIRV(c, g, args[2]);
            SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(scope));
            SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(semantics));
            uint32_t ret =  g->writer->MappedInstruction(inst, SPVWriter::Section::LocalFunction, returnType, args[0], scopeId, semanticsId, semanticsId, value1Loaded, value2Loaded);
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char*, SPVOp>> atomicIncrementDecrementIntrinsics =
    {
        MAKE_ATOMIC_INT_TYPED_INTRINSICS(Increment)
        , MAKE_ATOMIC_INT_TYPED_INTRINSICS(Decrement)
    };
    for (auto fun : atomicIncrementDecrementIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] =  [fun](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            assert(args.size() == 2);
            assert(args[1].isLiteral);
            auto [fn, op, inst] = fun;

            uint32_t scope = ScopeToAtomicScope(args[0].scope);
            uint32_t semantics = SemanticsTable[args[1].literalValue.ui];
            semantics |= ScopeToMemorySemantics(args[0].scope);
            
            SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(scope));
            SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(semantics));
            uint32_t ret = g->writer->MappedInstruction(inst, SPVWriter::Section::LocalFunction, returnType, args[0], scopeId, semanticsId, semanticsId);
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char*, SPVOp>> atomicAddSubIntrinsics =
    {
        MAKE_ATOMIC_INT_TYPED_INTRINSICS(Add)
        , MAKE_ATOMIC_INT_TYPED_INTRINSICS(Sub)
    };
    for (auto fun : atomicAddSubIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] =  [fun](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            assert(args.size() == 3);
            assert(args[2].isLiteral);
            auto [fn, op, inst] = fun;

            uint32_t scope = ScopeToAtomicScope(args[0].scope);
            uint32_t semantics = SemanticsTable[args[2].literalValue.ui];
            semantics |= ScopeToMemorySemantics(args[0].scope);

            SPIRVResult valueLoaded = LoadValueSPIRV(c, g, args[1]);
            SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(scope));
            SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(semantics));
            uint32_t ret = g->writer->MappedInstruction(inst, SPVWriter::Section::LocalFunction, returnType, args[0], scopeId, semanticsId, semanticsId, valueLoaded);

            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char, const char*, SPVOp>> atomicMinMaxIntrinsics =
    {
        MAKE_ATOMIC_SIGNED_INT_INTRINSICS(Min)
        , MAKE_ATOMIC_SIGNED_INT_INTRINSICS(Max)
    };
    for (auto fun : atomicMinMaxIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] =  [fun](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            assert(args.size() == 2);
            assert(args[1].isLiteral);
            auto [fn, sign, op, inst] = fun;

            uint32_t scope = ScopeToAtomicScope(args[0].scope);
            uint32_t semantics = SemanticsTable[args[2].literalValue.ui];
            semantics |= ScopeToMemorySemantics(args[0].scope);

            SPIRVResult valueLoaded = LoadValueSPIRV(c, g, args[1]);
            SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(scope));
            SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(semantics));
            uint32_t ret = g->writer->MappedInstruction(inst, SPVWriter::Section::LocalFunction, returnType, args[0], scopeId, semanticsId, semanticsId, valueLoaded);

            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, const char*, SPVOp>> atomicAndOrXorIntrinsics =
{
        MAKE_ATOMIC_INT_INTRINSICS(And)
        , MAKE_ATOMIC_INT_INTRINSICS(Or)
        , MAKE_ATOMIC_INT_INTRINSICS(Xor)
    };
    for (auto fun : atomicAndOrXorIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] =  [fun](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            assert(args.size() == 3);
            assert(args[2].isLiteral);
            auto [fn, op, inst] = fun;

            uint32_t scope = ScopeToAtomicScope(args[0].scope);
            uint32_t semantics = SemanticsTable[args[2].literalValue.ui];
            semantics |= ScopeToMemorySemantics(args[0].scope);

            SPIRVResult valueLoaded = LoadValueSPIRV(c, g, args[1]);
            SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(scope));
            SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(semantics));
            uint32_t ret = g->writer->MappedInstruction(inst, SPVWriter::Section::LocalFunction, returnType, args[0], scopeId, semanticsId, valueLoaded);

            return SPIRVResult(ret, returnType, true);
        };
    }

    SPIRVGenerator::IntrinsicMap[Intrinsics::BitInsert] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->writer->Extension(SPV_KHR_bit_instructions);
        //g->AddCapability("BitInstructions");
        SPIRVResult base = LoadValueSPIRV(c, g, args[0]);
        SPIRVResult insert = LoadValueSPIRV(c, g, args[1]);
        SPIRVResult offset = LoadValueSPIRV(c, g, args[2]);
        SPIRVResult count = LoadValueSPIRV(c, g, args[3]);
        uint32_t res = g->writer->MappedInstruction(OpBitFieldInsert, SPVWriter::Section::LocalFunction, returnType, base, insert, offset, count);

        return SPIRVResult(res, returnType, true);
    };

    std::vector<std::tuple<Function*, char, SPVOp>> bitExtractFunctions =
    {
        { Intrinsics::BitSExtract, 'S', OpBitFieldSExtract }
        , { Intrinsics::BitUExtract, 'U', OpBitFieldUExtract }
    };
    for (auto fun : bitExtractFunctions)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [fun](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            g->writer->Extension(SPV_KHR_bit_instructions);
            auto [fn, sign, inst] = fun;
            //g->AddCapability("BitInstructions");
            SPIRVResult base = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult offset = LoadValueSPIRV(c, g, args[1]);
            SPIRVResult count = LoadValueSPIRV(c, g, args[2]);
            uint32_t res = g->writer->MappedInstruction(inst, SPVWriter::Section::LocalFunction, returnType, base, offset, count);

            return SPIRVResult(res, returnType, true);
        };
    }

    SPIRVGenerator::IntrinsicMap[Intrinsics::BitReverse] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->writer->Extension(SPV_KHR_bit_instructions);
        
        SPIRVResult base = LoadValueSPIRV(c, g, args[0]);
        uint32_t res = g->writer->MappedInstruction(OpBitReverse, SPVWriter::Section::LocalFunction, returnType, base);
        return SPIRVResult(res, returnType, true);
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::BitCount] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        g->writer->Extension(SPV_KHR_bit_instructions);
        
        SPIRVResult base = LoadValueSPIRV(c, g, args[0]);
        uint32_t res = g->writer->MappedInstruction(OpBitCount, SPVWriter::Section::LocalFunction, returnType, base);
        return SPIRVResult(res, returnType, true);
    };
    
    SPIRVGenerator::IntrinsicMap[Intrinsics::ExecutionBarrier] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(2));
        SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(0x2 | 0x40 | 0x80 | 0x100 | 0x200 | 0x400 | 0x800));
        g->writer->Instruction(OpControlBarrier, SPVWriter::Section::LocalFunction, scopeId, scopeId, semanticsId);
        return SPIRVResult(0xFFFFFFFF, returnType);
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::ExecutionBarrierSubgroup] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult 
    {
        SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(3));
        SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(0x2 | 0x80));
        g->writer->Instruction(OpControlBarrier, SPVWriter::Section::LocalFunction, scopeId, scopeId, semanticsId);
        return SPIRVResult(0xFFFFFFFF, returnType);
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::ExecutionBarrierWorkgroup] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(2));
        SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(0x2 | 0x100));
        g->writer->Instruction(OpControlBarrier, SPVWriter::Section::LocalFunction, scopeId, scopeId, semanticsId);
        return SPIRVResult(0xFFFFFFFF, returnType);
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::MemoryExecutionBarrier] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(2));
        SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(0x2 | 0x40 | 0x80 | 0x100 | 0x200 | 0x400 | 0x800));
        g->writer->Instruction(OpMemoryBarrier, SPVWriter::Section::LocalFunction, scopeId, semanticsId);
        return SPIRVResult(0xFFFFFFFF, returnType);
    };
    
    SPIRVGenerator::IntrinsicMap[Intrinsics::MemoryExecutionBarrierBuffer] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(2));
        SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(0x2 | 0x40));
        g->writer->Instruction(OpMemoryBarrier, SPVWriter::Section::LocalFunction, scopeId, semanticsId);
        return SPIRVResult(0xFFFFFFFF, returnType);
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::MemoryExecutionBarrierTexture] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(2));
        SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(0x2 | 0x800));
        g->writer->Instruction(OpMemoryBarrier, SPVWriter::Section::LocalFunction, scopeId, semanticsId);
        return SPIRVResult(0xFFFFFFFF, returnType);
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::MemoryExecutionBarrierAtomic] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(2));
        SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(0x2 | 0x400));
        g->writer->Instruction(OpMemoryBarrier, SPVWriter::Section::LocalFunction, scopeId, semanticsId);
        return SPIRVResult(0xFFFFFFFF, returnType);
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::MemoryExecutionBarrierSubgroup] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(3));
        SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(0x2 | 0x80));
        g->writer->Instruction(OpMemoryBarrier, SPVWriter::Section::LocalFunction, scopeId, semanticsId);
        return SPIRVResult(0xFFFFFFFF, returnType);
    };

    SPIRVGenerator::IntrinsicMap[Intrinsics::MemoryExecutionBarrierWorkgroup] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
    {
        SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(2));
        SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(0x2 | 0x100));
        g->writer->Instruction(OpMemoryBarrier, SPVWriter::Section::LocalFunction, scopeId, semanticsId);
        return SPIRVResult(0xFFFFFFFF, returnType);
    };

    static auto createSampledImage = [](const Compiler* c, SPIRVGenerator* g, SPIRVResult arg0, SPIRVResult arg1) -> SPIRVResult
    {
        assert(arg0.parentTypes.size() > 0);
        SPIRVResult image = LoadValueSPIRV(c, g, arg0, true);
        SPIRVResult sampler = LoadValueSPIRV(c, g, arg1, true);
        uint32_t typeSymbol = AddType(g, TStr::Compact("sampledImage_", arg0.parentTypes[0]), OpTypeSampledImage, SPVArg{arg0.parentTypes[0]});
        uint32_t sampledImage = g->writer->MappedInstruction(OpSampledImage, SPVWriter::Section::LocalFunction, typeSymbol, image, sampler);
        return SPIRVResult(sampledImage, typeSymbol, true);
    };

    std::vector<std::tuple<Function*, bool>> textureGetSizeIntrinsics =
    {
        { Intrinsics::TextureGetSize_Texture1D, false }
        , { Intrinsics::TextureGetSize_Texture1DArray, false }
        , { Intrinsics::TextureGetSize_Texture2D, false }
        , { Intrinsics::TextureGetSize_Texture2DArray, false }
        , { Intrinsics::TextureGetSize_TextureCube, false }
        , { Intrinsics::TextureGetSize_TextureCubeArray, false }
        , { Intrinsics::TextureGetSize_Texture3D, false }
        , { Intrinsics::TextureGetSizeMip_Texture1D, true }
        , { Intrinsics::TextureGetSizeMip_Texture1DArray, true }
        , { Intrinsics::TextureGetSizeMip_Texture2D, true }
        , { Intrinsics::TextureGetSizeMip_Texture2DArray, true }
        , { Intrinsics::TextureGetSizeMip_TextureCube, true }
        , { Intrinsics::TextureGetSizeMip_TextureCubeArray, true }
        , { Intrinsics::TextureGetSizeMip_Texture3D, true }
    };
    for (auto fun : textureGetSizeIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [mip = std::get<1>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            g->writer->Capability(Capabilities::ImageQuery);
            SPIRVResult loadedArg = LoadValueSPIRV(c,g, args[0]);
            uint32_t ret;
            if (mip)
            {
                ret = g->writer->MappedInstruction(OpImageQuerySizeLod, SPVWriter::Section::LocalFunction, returnType, loadedArg, args[1]);
            }
            else
            {
                SPIRVResult zero = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(0));
                ret = g->writer->MappedInstruction(OpImageQuerySizeLod, SPVWriter::Section::LocalFunction, returnType, loadedArg, zero);
            }
                //ret = g->AddMappedOp(Format("OpImageQuerySize %%%d %%%d", returnType, image));
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*>> textureGetSampledMipIntrinsics =
    {
        { Intrinsics::TextureGetSampledMip_Texture1D }
        , { Intrinsics::TextureGetSampledMip_Texture1DArray }
        , { Intrinsics::TextureGetSampledMip_Texture2D }
        , { Intrinsics::TextureGetSampledMip_Texture2DArray }
        , { Intrinsics::TextureGetSampledMip_TextureCube }
        , { Intrinsics::TextureGetSampledMip_TextureCubeArray }
        , { Intrinsics::TextureGetSampledMip_Texture3D }
    };
    for (auto fun : textureGetSampledMipIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            g->writer->Capability(Capabilities::ImageQuery);
            uint32_t ret;
            SPIRVResult sampledImage = createSampledImage(c, g, args[0], args[1]);
            ret = g->writer->MappedInstruction(OpImageQueryLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, args[2]);
            return SPIRVResult(ret, returnType, true);
        };
    }
    
    std::vector<std::tuple<Function*>> sampledTextureGetSampledMipIntrinsics =
    {
        { Intrinsics::SampledTextureGetSampledMip_Texture1D }
        , { Intrinsics::SampledTextureGetSampledMip_Texture1DArray }
        , { Intrinsics::SampledTextureGetSampledMip_Texture2D }
        , { Intrinsics::SampledTextureGetSampledMip_Texture2DArray }
        , { Intrinsics::SampledTextureGetSampledMip_TextureCube }
        , { Intrinsics::SampledTextureGetSampledMip_TextureCubeArray }
        , { Intrinsics::SampledTextureGetSampledMip_Texture3D }
    };
    for (auto fun : sampledTextureGetSampledMipIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            g->writer->Capability(Capabilities::ImageQuery);
            uint32_t ret;
            ret = g->writer->MappedInstruction(OpImageQueryLod, SPVWriter::Section::LocalFunction, returnType, args[0], args[1]);
            return SPIRVResult(ret, returnType, true);
        };
    }
    
    std::vector<std::tuple<Function*>> textureGetMips =
    {
        { Intrinsics::TextureGetMips_Texture1D }
        , { Intrinsics::TextureGetMips_Texture1DArray }
        , { Intrinsics::TextureGetMips_Texture2D }
        , { Intrinsics::TextureGetMips_Texture2DArray }
        , { Intrinsics::TextureGetMips_TextureCube }
        , { Intrinsics::TextureGetMips_TextureCubeArray }
        , { Intrinsics::TextureGetMips_Texture3D }
        , { Intrinsics::TextureGetMips_Texture3D }
    };
    for (auto fun : textureGetSampledMipIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            g->writer->Capability(Capabilities::ImageQuery);
            SPIRVResult loadedArg = LoadValueSPIRV(c, g, args[0]);
            uint32_t ret;
            ret = g->writer->MappedInstruction(OpImageQueryLevels, SPVWriter::Section::LocalFunction, returnType, loadedArg);
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*>> textureGetSamples =
    {
        { Intrinsics::TextureGetSamples_Texture2DMS }
        , { Intrinsics::TextureGetSamples_Texture2DMSArray }
    };
    for (auto fun : textureGetSampledMipIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            g->writer->Capability(Capabilities::ImageQuery);
            SPIRVResult loadedArg = LoadValueSPIRV(c, g, args[0]);
            uint32_t ret;
            ret = g->writer->MappedInstruction(OpImageQueryLevels, SPVWriter::Section::LocalFunction, returnType, loadedArg);
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, bool>> textureFetchIntrinsics =
    {
        { Intrinsics::TextureFetchBase_Texture1D, false }
        , { Intrinsics::TextureFetchBase_Texture2D, false }
        , { Intrinsics::TextureFetchBase_Texture2DMS, false }
        , { Intrinsics::TextureFetchBase_Texture3D, false }
        , { Intrinsics::TextureFetchBase_Texture1DArray, false }
        , { Intrinsics::TextureFetchBase_Texture2DArray, false }
        , { Intrinsics::TextureFetchBase_Texture2DMSArray, false }
        , { Intrinsics::TextureFetchSample_Texture2DMS, true }
        , { Intrinsics::TextureFetchSample_Texture2DMSArray, true }
    };
    for (auto fun : textureFetchIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [sample = std::get<1>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            uint32_t ret;

            SPIRVResult image = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult coord = LoadValueSPIRV(c, g, args[1]);
            if (sample)
            {
                ret = g->writer->MappedInstruction(OpImageFetch, SPVWriter::Section::LocalFunction, returnType, image, coord, ImageOperands::Sample, args[2]);
            }
            else
            {
                ret = g->writer->MappedInstruction(OpImageFetch, SPVWriter::Section::LocalFunction, returnType, image, coord, ImageOperands::None);
            }
            return SPIRVResult(ret, returnType, true);
        };
    }

    std::vector<std::tuple<Function*, bool>> textureGatherIntrinsics =
    {
        { Intrinsics::TextureGatherBase_Texture2D, false }
        , { Intrinsics::TextureGatherBase_Texture2DArray, false }
        , { Intrinsics::TextureGatherBase_TextureCube, false }
        , { Intrinsics::TextureGatherBase_TextureCubeArray, false }
        , { Intrinsics::TextureGatherOffsets_Texture2D, true }
        , { Intrinsics::TextureGatherOffsets_Texture2DArray, true }
    };
    for (auto fun : textureGatherIntrinsics)
    {
        SPIRVGenerator::IntrinsicMap[std::get<0>(fun)] = [offsets = std::get<1>(fun)](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult {
            uint32_t ret;
            SPIRVResult loadedArg = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult coord = LoadValueSPIRV(c, g, args[1]);
            if (offsets)
            {
                ret = g->writer->MappedInstruction(OpImageGather, SPVWriter::Section::LocalFunction, returnType, loadedArg, args[2], ImageOperands::ConstOffsets, args[3]);
            }
            else
            {
                ret = g->writer->MappedInstruction(OpImageGather, SPVWriter::Section::LocalFunction, returnType, loadedArg, args[2], ImageOperands::None);
            }
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
      { Intrinsics::TextureSample##op##_Texture1D, SampleOperands::operands }\
    , { Intrinsics::TextureSample##op##_Texture2D, SampleOperands::operands }\
    , { Intrinsics::TextureSample##op##_Texture2DArray, SampleOperands::operands }\
    , { Intrinsics::TextureSample##op##_Texture2DMS, SampleOperands::operands }\
    , { Intrinsics::TextureSample##op##_Texture2DMSArray, SampleOperands::operands }\
    , { Intrinsics::TextureSample##op##_Texture3D, SampleOperands::operands }\
    , { Intrinsics::TextureSample##op##_TextureCube, SampleOperands::operands }\
    , { Intrinsics::TextureSample##op##_TextureCubeArray, SampleOperands::operands }

#define MAKE_TEXTURE_SAMPLE_INTRINSICS_NO_CUBE(op, operands)\
      { Intrinsics::TextureSample##op##_Texture1D, SampleOperands::operands }\
    , { Intrinsics::TextureSample##op##_Texture2D, SampleOperands::operands }\
    , { Intrinsics::TextureSample##op##_Texture2DArray, SampleOperands::operands }\
    , { Intrinsics::TextureSample##op##_Texture2DMS, SampleOperands::operands }\
    , { Intrinsics::TextureSample##op##_Texture2DMSArray, SampleOperands::operands }\
    , { Intrinsics::TextureSample##op##_Texture3D, SampleOperands::operands }\

#define MAKE_TEXTURE_SAMPLE_INTRINSICS_NO_MS(op, operands)\
      { Intrinsics::TextureSample##op##_Texture1D, SampleOperands::operands }\
    , { Intrinsics::TextureSample##op##_Texture2D, SampleOperands::operands }\
    , { Intrinsics::TextureSample##op##_Texture2DArray, SampleOperands::operands }\
    , { Intrinsics::TextureSample##op##_Texture3D, SampleOperands::operands }\
    , { Intrinsics::TextureSample##op##_TextureCube, SampleOperands::operands }\
    , { Intrinsics::TextureSample##op##_TextureCubeArray, SampleOperands::operands }

#define MAKE_TEXTURE_SAMPLE_INTRINSICS_NO_ARRAY_NO_MS(op, operands)\
      { Intrinsics::TextureSample##op##_Texture1D, SampleOperands::operands }\
    , { Intrinsics::TextureSample##op##_Texture2D, SampleOperands::operands }\
    , { Intrinsics::TextureSample##op##_Texture3D, SampleOperands::operands }\
    , { Intrinsics::TextureSample##op##_TextureCube, SampleOperands::operands }\

#define MAKE_TEXTURE_SAMPLE_INTRINSICS_NO_CUBE_NO_MS(op, operands)\
      { Intrinsics::TextureSample##op##_Texture1D, SampleOperands::operands }\
    , { Intrinsics::TextureSample##op##_Texture2D, SampleOperands::operands }\
    , { Intrinsics::TextureSample##op##_Texture2DArray, SampleOperands::operands }\
    , { Intrinsics::TextureSample##op##_Texture3D, SampleOperands::operands }\

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
            SPIRVResult loadedCoord = LoadValueSPIRV(c, g, args[2]);

            std::vector<uint32_t> newArgs = { returnType, sampledImage.name, loadedCoord.name };

            std::vector<SPIRVResult> extraArgs;
            for (size_t i = 3; i < args.size(); i++)
            {
                SPIRVResult loaded = LoadValueSPIRV(c, g, args[i]);
                extraArgs.push_back(loaded);
            }

            if (operands == None)
            {
                ret = g->writer->MappedInstruction(OpImageSampleImplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, ImageOperands::None);
            }
            else if (operands == Lod)
            {
                SPIRVResult loadedLod = LoadValueSPIRV(c, g, extraArgs[0]);
                ret = g->writer->MappedInstruction(OpImageSampleExplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, ImageOperands::Lod, loadedLod);
            }
            else if (operands == LodComp)
            {
                SPIRVResult loadedLod = LoadValueSPIRV(c, g, extraArgs[0]);
                SPIRVResult loadedComp = LoadValueSPIRV(c, g, extraArgs[1]);
                ret = g->writer->MappedInstruction(OpImageSampleDrefExplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, loadedComp, ImageOperands::Lod, loadedLod);
            }
            else if (operands == LodCompOffset)
            {
                SPIRVResult loadedLod = LoadValueSPIRV(c, g, extraArgs[0]);
                SPIRVResult loadedComp = LoadValueSPIRV(c, g, extraArgs[1]);
                SPIRVResult loadedOffset = LoadValueSPIRV(c, g, extraArgs[2]);
                ret = g->writer->MappedInstruction(OpImageSampleDrefExplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, loadedComp, ImageOperands::Lod, loadedLod, ImageOperands::Offset, loadedOffset);
            }
            else if (operands == LodOffset)
            {
                SPIRVResult loadedLod = LoadValueSPIRV(c, g, extraArgs[0]);
                SPIRVResult loadedOffset = LoadValueSPIRV(c, g, extraArgs[1]);
                ret = g->writer->MappedInstruction(OpImageSampleExplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, ImageOperands::Lod, loadedLod, ImageOperands::Offset, loadedOffset);
            }
            else if (operands == LodProj)
            {
                SPIRVResult loadedLod = LoadValueSPIRV(c, g, extraArgs[0]);
                ret = g->writer->MappedInstruction(OpImageSampleProjExplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, ImageOperands::Lod, loadedLod);
            }
            else if (operands == LodProjComp)
            {
                SPIRVResult loadedLod = LoadValueSPIRV(c, g, extraArgs[0]);
                SPIRVResult loadedComp = LoadValueSPIRV(c, g, extraArgs[1]);
                ret = g->writer->MappedInstruction(OpImageSampleProjDrefExplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, loadedComp, ImageOperands::Lod, loadedLod);
            }
            else if (operands == LodProjCompOffset)
            {
                SPIRVResult loadedLod = LoadValueSPIRV(c, g, extraArgs[0]);
                SPIRVResult loadedComp = LoadValueSPIRV(c, g, extraArgs[1]);
                SPIRVResult loadedOffset = LoadValueSPIRV(c, g, extraArgs[2]);
                ret = g->writer->MappedInstruction(OpImageSampleProjDrefExplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, loadedComp, ImageOperands::Lod, loadedLod, ImageOperands::Offset, loadedOffset);
            }
            else if (operands == LodProjOffset)
            {
                SPIRVResult loadedLod = LoadValueSPIRV(c, g, extraArgs[0]);
                SPIRVResult loadedOffset = LoadValueSPIRV(c, g, extraArgs[1]);
                ret = g->writer->MappedInstruction(OpImageSampleProjExplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, ImageOperands::Lod, loadedLod, ImageOperands::Offset, loadedOffset);
            }
            else if (operands == Bias)
            {
                SPIRVResult loadedBias = LoadValueSPIRV(c, g, extraArgs[0]);
                ret = g->writer->MappedInstruction(OpImageSampleImplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, ImageOperands::Bias, loadedBias);
            }
            else if (operands == BiasComp)
            {
                SPIRVResult loadedBias = LoadValueSPIRV(c, g, extraArgs[0]);
                SPIRVResult loadedComp = LoadValueSPIRV(c, g, extraArgs[1]);
                ret = g->writer->MappedInstruction(OpImageSampleDrefImplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, loadedComp, ImageOperands::Bias, loadedBias);
            }
            else if (operands == BiasOffset)
            {
                SPIRVResult loadedBias = LoadValueSPIRV(c, g, extraArgs[0]);
                SPIRVResult loadedOffset = LoadValueSPIRV(c, g, extraArgs[1]);
                ret = g->writer->MappedInstruction(OpImageSampleImplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, ImageOperands::Bias, loadedBias, ImageOperands::Offset, loadedOffset);
            }
            else if (operands == BiasProj)
            {
                SPIRVResult loadedBias = LoadValueSPIRV(c, g, extraArgs[0]);
                ret = g->writer->MappedInstruction(OpImageSampleProjImplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, ImageOperands::Bias, loadedBias);
            }
            else if (operands == BiasProjComp)
            {
                SPIRVResult loadedBias = LoadValueSPIRV(c, g, extraArgs[0]);
                SPIRVResult loadedComp = LoadValueSPIRV(c, g, extraArgs[1]);
                ret = g->writer->MappedInstruction(OpImageSampleProjDrefImplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, loadedComp, ImageOperands::Bias, loadedBias);
            }
            else if (operands == BiasProjOffset)
            {
                SPIRVResult loadedBias = LoadValueSPIRV(c, g, extraArgs[0]);
                SPIRVResult loadedOffset = LoadValueSPIRV(c, g, extraArgs[1]);
                ret = g->writer->MappedInstruction(OpImageSampleProjImplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, ImageOperands::Bias, loadedBias, ImageOperands::Offset, loadedOffset);
            }
            else if (operands == Comp)
            {
                SPIRVResult loadedComp = LoadValueSPIRV(c, g, extraArgs[0]);
                ret = g->writer->MappedInstruction(OpImageSampleDrefImplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, loadedComp, ImageOperands::None);
            }
            else if (operands == CompOffset)
            {
                SPIRVResult loadedComp = LoadValueSPIRV(c, g, extraArgs[0]);
                SPIRVResult loadedOffset = LoadValueSPIRV(c, g, extraArgs[1]);
                ret = g->writer->MappedInstruction(OpImageSampleDrefImplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, loadedComp, ImageOperands::Offset, loadedOffset);
            }
            else if (operands == Grad)
            {
                SPIRVResult loadedGradX = LoadValueSPIRV(c, g, extraArgs[0]);
                SPIRVResult loadedGradY = LoadValueSPIRV(c, g, extraArgs[1]);
                ret = g->writer->MappedInstruction(OpImageSampleExplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, ImageOperands::Grad, loadedGradX, loadedGradY);
            }
            else if (operands == GradComp)
            {
                SPIRVResult loadedGradX = LoadValueSPIRV(c, g, extraArgs[0]);
                SPIRVResult loadedGradY = LoadValueSPIRV(c, g, extraArgs[1]);
                SPIRVResult loadedComp = LoadValueSPIRV(c, g, extraArgs[2]);
                ret = g->writer->MappedInstruction(OpImageSampleDrefExplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, loadedComp, ImageOperands::Grad, loadedGradX, loadedGradY);
            }
            else if (operands == GradOffset)
            {
                SPIRVResult loadedGradX = LoadValueSPIRV(c, g, extraArgs[0]);
                SPIRVResult loadedGradY = LoadValueSPIRV(c, g, extraArgs[1]);
                SPIRVResult loadedOffset = LoadValueSPIRV(c, g, extraArgs[2]);
                ret = g->writer->MappedInstruction(OpImageSampleExplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, ImageOperands::Grad, loadedGradX, loadedGradY, ImageOperands::Offset, loadedOffset);
            }
            else if (operands == GradProj)
            {
                SPIRVResult loadedGradX = LoadValueSPIRV(c, g, extraArgs[1]);
                SPIRVResult loadedGradY = LoadValueSPIRV(c, g, extraArgs[2]);
                ret = g->writer->MappedInstruction(OpImageSampleProjExplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, ImageOperands::Grad, loadedGradX, loadedGradY);
            }
            else if (operands == GradProjComp)
            {
                SPIRVResult loadedGradX = LoadValueSPIRV(c, g, extraArgs[0]);
                SPIRVResult loadedGradY = LoadValueSPIRV(c, g, extraArgs[1]);
                SPIRVResult loadedComp = LoadValueSPIRV(c, g, extraArgs[2]);
                ret = g->writer->MappedInstruction(OpImageSampleProjDrefExplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, loadedComp, ImageOperands::Grad, loadedGradX, loadedGradY);
            }
            else if (operands == GradProjCompOffset)
            {
                SPIRVResult loadedGradX = LoadValueSPIRV(c, g, extraArgs[0]);
                SPIRVResult loadedGradY = LoadValueSPIRV(c, g, extraArgs[1]);
                SPIRVResult loadedComp = LoadValueSPIRV(c, g, extraArgs[2]);
                SPIRVResult loadedOffset = LoadValueSPIRV(c, g, extraArgs[3]);
                ret = g->writer->MappedInstruction(OpImageSampleProjDrefExplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, loadedComp, ImageOperands::Grad, loadedGradX, loadedGradY, ImageOperands::Offset, loadedOffset);
            }
            else if (operands == GradProjOffset)
            {
                SPIRVResult loadedGradX = LoadValueSPIRV(c, g, extraArgs[0]);
                SPIRVResult loadedGradY = LoadValueSPIRV(c, g, extraArgs[1]);
                SPIRVResult loadedOffset = LoadValueSPIRV(c, g, extraArgs[2]);
                ret = g->writer->MappedInstruction(OpImageSampleProjExplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, ImageOperands::Grad, loadedGradX, loadedGradY, ImageOperands::Offset, loadedOffset);
            }
            else if (operands == Proj)
            {
                ret = g->writer->MappedInstruction(OpImageSampleProjImplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, ImageOperands::None);
            }
            else if (operands == ProjComp)
            {
                SPIRVResult loadedComp = LoadValueSPIRV(c, g, extraArgs[0]);
                ret = g->writer->MappedInstruction(OpImageSampleProjDrefImplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedComp, ImageOperands::None);
            }
            else if (operands == ProjCompOffset)
            {
                SPIRVResult loadedComp = LoadValueSPIRV(c, g, extraArgs[0]);
                SPIRVResult loadedOffset = LoadValueSPIRV(c, g, extraArgs[1]);
                ret = g->writer->MappedInstruction(OpImageSampleProjDrefImplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedComp, ImageOperands::Offset, loadedOffset);
            }
            else if (operands == ProjOffset)
            {
                SPIRVResult loadedOffset = LoadValueSPIRV(c, g, extraArgs[0]);
                ret = g->writer->MappedInstruction(OpImageSampleProjImplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, ImageOperands::Offset, loadedOffset);
            }
            return SPIRVResult(ret, returnType, true);
        };
    }

#define MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS(op, operands)\
      { Intrinsics::SampledTextureSample##op##_Texture1D, SampleOperands::operands }\
    , { Intrinsics::SampledTextureSample##op##_Texture2D, SampleOperands::operands }\
    , { Intrinsics::SampledTextureSample##op##_Texture2DArray, SampleOperands::operands }\
    , { Intrinsics::SampledTextureSample##op##_Texture2DMS, SampleOperands::operands }\
    , { Intrinsics::SampledTextureSample##op##_Texture2DMSArray, SampleOperands::operands }\
    , { Intrinsics::SampledTextureSample##op##_Texture3D, SampleOperands::operands }\
    , { Intrinsics::SampledTextureSample##op##_TextureCube, SampleOperands::operands }\
    , { Intrinsics::SampledTextureSample##op##_TextureCubeArray, SampleOperands::operands }

#define MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS_NO_CUBE(op, operands)\
      { Intrinsics::SampledTextureSample##op##_Texture1D, SampleOperands::operands }\
    , { Intrinsics::SampledTextureSample##op##_Texture2D, SampleOperands::operands }\
    , { Intrinsics::SampledTextureSample##op##_Texture2DArray, SampleOperands::operands }\
    , { Intrinsics::SampledTextureSample##op##_Texture2DMS, SampleOperands::operands }\
    , { Intrinsics::SampledTextureSample##op##_Texture2DMSArray, SampleOperands::operands }\
    , { Intrinsics::SampledTextureSample##op##_Texture3D, SampleOperands::operands }\

#define MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS_NO_MS(op, operands)\
      { Intrinsics::SampledTextureSample##op##_Texture1D, SampleOperands::operands }\
    , { Intrinsics::SampledTextureSample##op##_Texture2D, SampleOperands::operands }\
    , { Intrinsics::SampledTextureSample##op##_Texture2DArray, SampleOperands::operands }\
    , { Intrinsics::SampledTextureSample##op##_Texture3D, SampleOperands::operands }\
    , { Intrinsics::SampledTextureSample##op##_TextureCube, SampleOperands::operands }\
    , { Intrinsics::SampledTextureSample##op##_TextureCubeArray, SampleOperands::operands }

#define MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS_NO_ARRAY_NO_MS(op, operands)\
      { Intrinsics::SampledTextureSample##op##_Texture1D, SampleOperands::operands }\
    , { Intrinsics::SampledTextureSample##op##_Texture2D, SampleOperands::operands }\
    , { Intrinsics::SampledTextureSample##op##_Texture3D, SampleOperands::operands }\
    , { Intrinsics::SampledTextureSample##op##_TextureCube, SampleOperands::operands }\

#define MAKE_SAMPLEDTEXTURE_SAMPLE_INTRINSICS_NO_CUBE_NO_MS(op, operands)\
      { Intrinsics::SampledTextureSample##op##_Texture1D, SampleOperands::operands }\
    , { Intrinsics::SampledTextureSample##op##_Texture2D, SampleOperands::operands }\
    , { Intrinsics::SampledTextureSample##op##_Texture2DArray, SampleOperands::operands }\
    , { Intrinsics::SampledTextureSample##op##_Texture3D, SampleOperands::operands }\

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
            SPIRVResult sampledImage = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult loadedCoord = LoadValueSPIRV(c, g, args[1]);

            std::vector<uint32_t> newArgs = { returnType, sampledImage.name, loadedCoord.name };

            std::vector<SPIRVResult> extraArgs;
            for (size_t i = 2; i < args.size(); i++)
            {
                SPIRVResult loaded = LoadValueSPIRV(c, g, args[i]);
                extraArgs.push_back(loaded);
            }

            if (operands == None)
            {
                ret = g->writer->MappedInstruction(OpImageSampleImplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, ImageOperands::None);
            }
            else if (operands == Lod)
            {
                SPIRVResult loadedLod = LoadValueSPIRV(c, g, extraArgs[0]);
                ret = g->writer->MappedInstruction(OpImageSampleExplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, ImageOperands::Lod, loadedLod);
            }
            else if (operands == LodComp)
            {
                SPIRVResult loadedLod = LoadValueSPIRV(c, g, extraArgs[0]);
                SPIRVResult loadedComp = LoadValueSPIRV(c, g, extraArgs[1]);
                ret = g->writer->MappedInstruction(OpImageSampleDrefExplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, loadedComp, ImageOperands::Lod, loadedLod);
            }
            else if (operands == LodCompOffset)
            {
                SPIRVResult loadedLod = LoadValueSPIRV(c, g, extraArgs[0]);
                SPIRVResult loadedComp = LoadValueSPIRV(c, g, extraArgs[1]);
                SPIRVResult loadedOffset = LoadValueSPIRV(c, g, extraArgs[2]);
                ret = g->writer->MappedInstruction(OpImageSampleDrefExplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, loadedComp, ImageOperands::Lod, loadedLod, ImageOperands::Offset, loadedOffset);
            }
            else if (operands == LodOffset)
            {
                SPIRVResult loadedLod = LoadValueSPIRV(c, g, extraArgs[0]);
                SPIRVResult loadedOffset = LoadValueSPIRV(c, g, extraArgs[1]);
                ret = g->writer->MappedInstruction(OpImageSampleExplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, ImageOperands::Lod, loadedLod, ImageOperands::Offset, loadedOffset);
            }
            else if (operands == LodProj)
            {
                SPIRVResult loadedLod = LoadValueSPIRV(c, g, extraArgs[0]);
                ret = g->writer->MappedInstruction(OpImageSampleProjExplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, ImageOperands::Lod, loadedLod);
            }
            else if (operands == LodProjComp)
            {
                SPIRVResult loadedLod = LoadValueSPIRV(c, g, extraArgs[0]);
                SPIRVResult loadedComp = LoadValueSPIRV(c, g, extraArgs[1]);
                ret = g->writer->MappedInstruction(OpImageSampleProjDrefExplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, loadedComp, ImageOperands::Lod, loadedLod);
            }
            else if (operands == LodProjCompOffset)
            {
                SPIRVResult loadedLod = LoadValueSPIRV(c, g, extraArgs[0]);
                SPIRVResult loadedComp = LoadValueSPIRV(c, g, extraArgs[1]);
                SPIRVResult loadedOffset = LoadValueSPIRV(c, g, extraArgs[2]);
                ret = g->writer->MappedInstruction(OpImageSampleProjDrefExplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, loadedComp, ImageOperands::Lod, loadedLod, ImageOperands::Offset, loadedOffset);
            }
            else if (operands == LodProjOffset)
            {
                SPIRVResult loadedLod = LoadValueSPIRV(c, g, extraArgs[0]);
                SPIRVResult loadedOffset = LoadValueSPIRV(c, g, extraArgs[1]);
                ret = g->writer->MappedInstruction(OpImageSampleProjExplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, ImageOperands::Lod, loadedLod, ImageOperands::Offset, loadedOffset);
            }
            else if (operands == Bias)
            {
                SPIRVResult loadedBias = LoadValueSPIRV(c, g, extraArgs[0]);
                ret = g->writer->MappedInstruction(OpImageSampleImplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, ImageOperands::Bias, loadedBias);
            }
            else if (operands == BiasComp)
            {
                SPIRVResult loadedBias = LoadValueSPIRV(c, g, extraArgs[0]);
                SPIRVResult loadedComp = LoadValueSPIRV(c, g, extraArgs[1]);
                ret = g->writer->MappedInstruction(OpImageSampleDrefImplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, loadedComp, ImageOperands::Bias, loadedBias);
            }
            else if (operands == BiasOffset)
            {
                SPIRVResult loadedBias = LoadValueSPIRV(c, g, extraArgs[0]);
                SPIRVResult loadedOffset = LoadValueSPIRV(c, g, extraArgs[1]);
                ret = g->writer->MappedInstruction(OpImageSampleImplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, ImageOperands::Bias, loadedBias, ImageOperands::Offset, loadedOffset);
            }
            else if (operands == BiasProj)
            {
                SPIRVResult loadedBias = LoadValueSPIRV(c, g, extraArgs[0]);
                ret = g->writer->MappedInstruction(OpImageSampleProjImplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, ImageOperands::Bias, loadedBias);
            }
            else if (operands == BiasProjComp)
            {
                SPIRVResult loadedBias = LoadValueSPIRV(c, g, extraArgs[0]);
                SPIRVResult loadedComp = LoadValueSPIRV(c, g, extraArgs[1]);
                ret = g->writer->MappedInstruction(OpImageSampleProjDrefImplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, loadedComp, ImageOperands::Bias, loadedBias);
            }
            else if (operands == BiasProjOffset)
            {
                SPIRVResult loadedBias = LoadValueSPIRV(c, g, extraArgs[0]);
                SPIRVResult loadedOffset = LoadValueSPIRV(c, g, extraArgs[1]);
                ret = g->writer->MappedInstruction(OpImageSampleProjImplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, ImageOperands::Bias, loadedBias, ImageOperands::Offset, loadedOffset);
            }
            else if (operands == Comp)
            {
                SPIRVResult loadedComp = LoadValueSPIRV(c, g, extraArgs[0]);
                ret = g->writer->MappedInstruction(OpImageSampleDrefImplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, loadedComp, ImageOperands::None);
            }
            else if (operands == CompOffset)
            {
                SPIRVResult loadedComp = LoadValueSPIRV(c, g, extraArgs[0]);
                SPIRVResult loadedOffset = LoadValueSPIRV(c, g, extraArgs[1]);
                ret = g->writer->MappedInstruction(OpImageSampleDrefImplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, loadedComp, ImageOperands::Offset, loadedOffset);
            }
            else if (operands == Grad)
            {
                SPIRVResult loadedGradX = LoadValueSPIRV(c, g, extraArgs[0]);
                SPIRVResult loadedGradY = LoadValueSPIRV(c, g, extraArgs[1]);
                ret = g->writer->MappedInstruction(OpImageSampleExplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, ImageOperands::Grad, loadedGradX, loadedGradY);
            }
            else if (operands == GradComp)
            {
                SPIRVResult loadedGradX = LoadValueSPIRV(c, g, extraArgs[0]);
                SPIRVResult loadedGradY = LoadValueSPIRV(c, g, extraArgs[1]);
                SPIRVResult loadedComp = LoadValueSPIRV(c, g, extraArgs[2]);
                ret = g->writer->MappedInstruction(OpImageSampleDrefExplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, loadedComp, ImageOperands::Grad, loadedGradX, loadedGradY);
            }
            else if (operands == GradOffset)
            {
                SPIRVResult loadedGradX = LoadValueSPIRV(c, g, extraArgs[0]);
                SPIRVResult loadedGradY = LoadValueSPIRV(c, g, extraArgs[1]);
                SPIRVResult loadedOffset = LoadValueSPIRV(c, g, extraArgs[2]);
                ret = g->writer->MappedInstruction(OpImageSampleExplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, ImageOperands::Grad, loadedGradX, loadedGradY, ImageOperands::Offset, loadedOffset);
            }
            else if (operands == GradProj)
            {
                SPIRVResult loadedGradX = LoadValueSPIRV(c, g, extraArgs[1]);
                SPIRVResult loadedGradY = LoadValueSPIRV(c, g, extraArgs[2]);
                ret = g->writer->MappedInstruction(OpImageSampleProjExplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, ImageOperands::Grad, loadedGradX, loadedGradY);
            }
            else if (operands == GradProjComp)
            {
                SPIRVResult loadedGradX = LoadValueSPIRV(c, g, extraArgs[0]);
                SPIRVResult loadedGradY = LoadValueSPIRV(c, g, extraArgs[1]);
                SPIRVResult loadedComp = LoadValueSPIRV(c, g, extraArgs[2]);
                ret = g->writer->MappedInstruction(OpImageSampleProjDrefExplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, loadedComp, ImageOperands::Grad, loadedGradX, loadedGradY);
            }
            else if (operands == GradProjCompOffset)
            {
                SPIRVResult loadedGradX = LoadValueSPIRV(c, g, extraArgs[0]);
                SPIRVResult loadedGradY = LoadValueSPIRV(c, g, extraArgs[1]);
                SPIRVResult loadedComp = LoadValueSPIRV(c, g, extraArgs[2]);
                SPIRVResult loadedOffset = LoadValueSPIRV(c, g, extraArgs[3]);
                ret = g->writer->MappedInstruction(OpImageSampleProjDrefExplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, loadedComp, ImageOperands::Grad, loadedGradX, loadedGradY, ImageOperands::Offset, loadedOffset);
            }
            else if (operands == GradProjOffset)
            {
                SPIRVResult loadedGradX = LoadValueSPIRV(c, g, extraArgs[0]);
                SPIRVResult loadedGradY = LoadValueSPIRV(c, g, extraArgs[1]);
                SPIRVResult loadedOffset = LoadValueSPIRV(c, g, extraArgs[2]);
                ret = g->writer->MappedInstruction(OpImageSampleProjExplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, ImageOperands::Grad, loadedGradX, loadedGradY, ImageOperands::Offset, loadedOffset);
            }
            else if (operands == Proj)
            {
                ret = g->writer->MappedInstruction(OpImageSampleProjImplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, ImageOperands::None);
            }
            else if (operands == ProjComp)
            {
                SPIRVResult loadedComp = LoadValueSPIRV(c, g, extraArgs[0]);
                ret = g->writer->MappedInstruction(OpImageSampleProjDrefImplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedComp, ImageOperands::None);
            }
            else if (operands == ProjCompOffset)
            {
                SPIRVResult loadedComp = LoadValueSPIRV(c, g, extraArgs[0]);
                SPIRVResult loadedOffset = LoadValueSPIRV(c, g, extraArgs[1]);
                ret = g->writer->MappedInstruction(OpImageSampleProjDrefImplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedComp, ImageOperands::Offset, loadedOffset);
            }
            else if (operands == ProjOffset)
            {
                SPIRVResult loadedOffset = LoadValueSPIRV(c, g, extraArgs[0]);
                ret = g->writer->MappedInstruction(OpImageSampleProjImplicitLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, loadedCoord, ImageOperands::Offset, loadedOffset);
            }

            return SPIRVResult(ret, returnType, true);
        };
    }

#define MAKE_TEXTURE_LOAD_STORE_INTRINSICS(op, operands, store)\
      { Intrinsics::Texture##op##_Texture1D, store, SampleOperands::operands }\
    , { Intrinsics::Texture##op##_Texture2D, store, SampleOperands::operands }\
    , { Intrinsics::Texture##op##_Texture2DArray, store, SampleOperands::operands }\
    , { Intrinsics::Texture##op##_Texture2DMS, store, SampleOperands::operands }\
    , { Intrinsics::Texture##op##_Texture2DMSArray, store, SampleOperands::operands }\
    , { Intrinsics::Texture##op##_Texture3D, store, SampleOperands::operands }\
    , { Intrinsics::Texture##op##_TextureCube, store, SampleOperands::operands }\
    , { Intrinsics::Texture##op##_TextureCubeArray, store, SampleOperands::operands }

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
                    g->writer->Instruction(OpImageWrite, SPVWriter::Section::LocalFunction, SPVArg{ dereffed }, loadedCoord, loadedValue, ImageOperands::Lod, loadedMip);
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
                    g->writer->Instruction(OpImageWrite, SPVWriter::Section::LocalFunction, SPVArg{ dereffed }, loadedCoord, loadedValue);
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
                    ret = g->writer->MappedInstruction(OpImageRead, SPVWriter::Section::LocalFunction, returnType, SPVArg{ dereffed }, loadedCoord, ImageOperands::Lod, loadedMip);
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
                    ret = g->writer->MappedInstruction(OpImageRead, SPVWriter::Section::LocalFunction, returnType, SPVArg{ dereffed }, loadedCoord);
                    res.isValue = true;
                }
            }
            res.name = ret;
            res.typeName = returnType;
            return res;
        };
    }

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
            SPIRVResult::Storage storage = ResolveSPIRVVariableStorage(paramResolved->type, paramResolved->typeSymbol, paramResolved->storage, paramResolved->usageBits);
            SPIRVResult typeName = GenerateTypeSPIRV(compiler, generator, paramResolved->type, paramResolved->typeSymbol, storage);
            typeArgs.Append(paramResolved->type.ToString());
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
            SPIRVResult::Storage storage = ResolveSPIRVVariableStorage(paramResolved->type, paramResolved->typeSymbol, paramResolved->storage, paramResolved->usageBits);
            SPIRVResult varType = GenerateTypeSPIRV(compiler, generator, paramResolved->type, paramResolved->typeSymbol, storage);

            // If value is not a pointer, generate a copy of the value inside the function
            if (!paramResolved->type.IsPointer())
            {
                uint32_t paramName = MappedInstruction(generator, SPVWriter::Section::Functions, OpFunctionParameter, varType.typeName, SPVComment{param->name.c_str()});
                TransientString type = paramResolved->type.ToString();
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

    if (compiler->options.symbols)
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

            if (compiler->options.symbols)
                generator->writer->Instruction(OpMemberName, SPVWriter::Section::DebugNames, SPVArg{ structName }, i, var->name.c_str());
            
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
    SamplerState* sampler = static_cast<SamplerState*>(symbol);
    SamplerState::__Resolved* samplerResolved = Symbol::Resolved(sampler);
    
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
        uint32_t name = AddSymbol(generator, sampler->name, SPVWriter::Section::Declarations, OpConstantSampler, samplerType.typeName, addressingTable[samplerResolved->addressU], samplerResolved->unnormalizedSamplingEnabled ? 0 : 1, filterTable[samplerResolved->magFilter]);

        if (compiler->options.symbols)
            generator->writer->Instruction(OpName, SPVWriter::Section::DebugNames, SPVArg{ name }, sampler->name.c_str());
        
        return SPIRVResult(name, samplerType.typeName, true, true);
    }
    else
    {
        // Generate immutable sampler
        uint32_t name = AddSymbol(generator, symbol->name, SPVWriter::Section::Declarations, OpVariable, samplerType, ScopeToEnum(samplerType.scope));
        generator->interfaceVariables.Insert(name);

        if (compiler->options.symbols)
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

    SPIRVResult::Storage storage = ResolveSPIRVVariableStorage(varResolved->type, varResolved->typeSymbol, varResolved->storage, varResolved->usageBits);
    
    SPIRVResult typeName = GenerateTypeSPIRV(compiler, generator, varResolved->type, varResolved->typeSymbol, storage, storage == SPIRVResult::Storage::Uniform || storage == SPIRVResult::Storage::StorageBuffer);
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
                it = SPIRVGenerator::IntrinsicMap.find(varResolved->valueConversionFunction);
                assert(it != SPIRVGenerator::IntrinsicMap.end());
            }
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
        if (!varResolved->type.IsPointer())
        {
            TStr ptrType = TStr("ptr_", varResolved->type.ToString());
            typePtrName = AddType(generator, TStr::Compact(ptrType, "_", scope), typeName, OpTypePointer, ScopeToEnum(typeName.scope), SPVArg{ typeName.typeName });
            typeName.parentTypes.push_back(typeName.typeName);
            typeName.typeName = typePtrName;
        }
        
        if (initializer != SPIRVResult::Invalid())
        {
            // Only create the variable with the initializer if it's const and the variable is global
            if (initializer.isConst && isGlobal)
            {
                name = AddSymbol(generator, varResolved->name, isGlobal ? SPVWriter::Section::Declarations : SPVWriter::Section::VariableDeclarations, OpVariable, typeName, ScopeToEnum(typeName.scope), SPVArg{ initializer.name });
                if (compiler->options.symbols)
                    generator->writer->Instruction(OpName, SPVWriter::Section::DebugNames, SPVArg{ name }, var->name.c_str());
            }
            else
            {
                name = AddSymbol(generator, varResolved->name, isGlobal ? SPVWriter::Section::Declarations : SPVWriter::Section::VariableDeclarations, OpVariable, typeName, ScopeToEnum(typeName.scope));
                if (compiler->options.symbols)
                    generator->writer->Instruction(OpName, SPVWriter::Section::DebugNames, SPVArg{ name }, var->name.c_str());
                SPIRVResult loaded = LoadValueSPIRV(compiler, generator, initializer);
                generator->writer->Instruction(OpStore, SPVWriter::Section::LocalFunction, SPVArg(name), loaded);
            }
        }
        else
        {
            name = AddSymbol(generator, varResolved->name, isGlobal ? SPVWriter::Section::Declarations : SPVWriter::Section::VariableDeclarations, OpVariable, typeName, ScopeToEnum(typeName.scope));
            if (compiler->options.symbols)
                generator->writer->Instruction(OpName, SPVWriter::Section::DebugNames, SPVArg{ name }, var->name.c_str());
        }

        if (storage == SPIRVResult::Storage::StorageBuffer || storage == SPIRVResult::Storage::Uniform || storage == SPIRVResult::Storage::PushConstant || storage == SPIRVResult::Storage::UniformConstant || storage == SPIRVResult::Storage::Sampler)
        {
            uint32_t structSymbol = GetSymbol(generator, varResolved->typeSymbol->name).value;
            if (typeName.scope != SPIRVResult::Storage::Sampler && varResolved->type.IsPointer())
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
        generator->writer->LinkDefinedVariable(varResolved->name, initializer.name);
        generator->writer->Decorate(SPVArg(initializer.name), Decorations::SpecId, varResolved->binding);

        if (compiler->options.symbols)
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
        bool argumentsNeedsConversion = !resolvedCall->conversions.empty();
        
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
                        it = SPIRVGenerator::IntrinsicMap.find(conversion);
                        assert(it != SPIRVGenerator::IntrinsicMap.end());
                    }

                    arg = it->second(compiler, generator, convertedReturn, { arg });
                }
            }

            argListArray.Append(SPVArg(arg.name));
            argList.append(Format("%%%d ", arg.name));
        }

        if (compiler->options.symbols)
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

        if (compiler->options.symbols)
        {
            uint32_t name = generator->writer->String(expr->location.file.c_str());
            generator->writer->Instruction(OpLine, SPVWriter::Section::LocalFunction, SPVArg{name}, expr->location.line, expr->location.start);
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

    if (compiler->options.symbols)
    {
        uint32_t name = generator->writer->String(expr->location.file.c_str());
        generator->writer->Instruction(OpLine, SPVWriter::Section::LocalFunction, SPVArg{name}, expr->location.line, expr->location.start);
    }
    if (leftType.modifiers.empty())
    {
        Type* type = static_cast<Type*>(compiler->GetSymbol(leftType.name));

        if (type->IsVector() || type->IsMatrix())
        {
            auto op = TransientString("operator[](", rightType.name, ")");
            auto it = type->scope.symbolLookup.Find(op);
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
    
    if (compiler->options.symbols)
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

    if (compiler->options.symbols)
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

    if (compiler->options.symbols)
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

    if (compiler->options.symbols)
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
    
    if (compiler->options.symbols)
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

    if (compiler->options.symbols)
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
    if (compiler->options.symbols)
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
                        for (size_t i = 0; i < ty->symbols.size; i++)
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

    if (compiler->options.symbols)
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

    if (compiler->options.symbols)
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

    if (compiler->options.symbols)
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
    if (compiler->options.symbols)
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
SPIRVGenerator::Generate(const Compiler* compiler, const Program* program, const PinnedArray<Symbol*>& symbols, std::function<void(const std::string&, const std::string&)> writerFunc)
{
    Program::__Resolved* progResolved = static_cast<Program::__Resolved*>(program->resolved);

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

        this->writer->Capability(extensionEnumMap[(Program::__Resolved::ProgramEntryType)mapping]);
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

        if (compiler->options.symbols)
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

        this->writer->Instruction(OpEntryPoint, SPVWriter::Section::Header, executionModelEnumMap[(Program::__Resolved::ProgramEntryType)mapping], SPVArg(entryFunction), "main", SPVArgList(interfaceVars));

        switch (mapping)
        {
        case Program::__Resolved::GeometryShader:
            this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{ entryFunction }, ExecutionModes::Invocations, funResolved->executionModifiers.invocations);
            this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{ entryFunction }, ExecutionModes::OutputVertices, funResolved->executionModifiers.maxOutputVertices);
            this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{ entryFunction }, inputPrimitiveTopologyEnumMap[funResolved->executionModifiers.inputPrimitiveTopology]);
            this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{ entryFunction }, outputPrimitiveTopologyEnumMap[funResolved->executionModifiers.inputPrimitiveTopology]);
            break;
        case Program::__Resolved::HullShader:
            this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{ entryFunction }, partitionMap[funResolved->executionModifiers.partitionMethod]);
            this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{ entryFunction }, windingOrderMap[funResolved->executionModifiers.windingOrder]);
            this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{ entryFunction }, inputPrimitiveTopologyEnumMap[funResolved->executionModifiers.inputPrimitiveTopology]);
            this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, SPVArg{ entryFunction }, ExecutionModes::OutputVertices, funResolved->executionModifiers.maxOutputVertices);
            break;
        case Program::__Resolved::DomainShader:
            break;
        case Program::__Resolved::PixelShader:
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
            RenderState* state = static_cast<RenderState*>(progResolved->mappings[Program::__Resolved::RenderState]);
            RenderState::__Resolved* stateRes = Symbol::Resolved(state);
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
        case Program::__Resolved::ComputeShader:
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

        GrowingString binary;
        binary.Line("; Entry point", funResolved->name);
        binary.Append(this->writer->texts[(uint32_t)SPVWriter::Section::Top]);
        binary.Append(this->writer->texts[(uint32_t)SPVWriter::Section::Capabilities]);
        binary.Append(this->writer->texts[(uint32_t)SPVWriter::Section::Extensions]);
        binary.Append(this->writer->texts[(uint32_t)SPVWriter::Section::ExtImports]);
        binary.Append(this->writer->texts[(uint32_t)SPVWriter::Section::Header]);
        if (compiler->options.symbols)
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

        std::vector<uint32_t> spvBinary;
        spvBinary.insert(spvBinary.end(), this->writer->binaries[(uint32_t)SPVWriter::Section::Top].begin(), this->writer->binaries[(uint32_t)SPVWriter::Section::Top].end());
        spvBinary.insert(spvBinary.end(), this->writer->binaries[(uint32_t)SPVWriter::Section::Capabilities].begin(), this->writer->binaries[(uint32_t)SPVWriter::Section::Capabilities].end());
        spvBinary.insert(spvBinary.end(), this->writer->binaries[(uint32_t)SPVWriter::Section::Extensions].begin(), this->writer->binaries[(uint32_t)SPVWriter::Section::Extensions].end());
        spvBinary.insert(spvBinary.end(), this->writer->binaries[(uint32_t)SPVWriter::Section::ExtImports].begin(), this->writer->binaries[(uint32_t)SPVWriter::Section::ExtImports].end());
        spvBinary.insert(spvBinary.end(), this->writer->binaries[(uint32_t)SPVWriter::Section::Header].begin(), this->writer->binaries[(uint32_t)SPVWriter::Section::Header].end());
        if (compiler->options.symbols)
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
                spv_diagnostic diag2;
                this->Error(Format("Internal SPIRV generation error: %s", diag->error));
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
