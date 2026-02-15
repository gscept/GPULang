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

#include <array>
#include <complex>
#include <algorithm>

#include "ast/samplerstate.h"
#include "ast/expressions/arrayinitializerexpression.h"
#include "ast/expressions/floatvecexpression.h"

namespace GPULang
{
struct SPVHeader
{
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

struct SPVEnumList
{
    SPVEnum* enums = nullptr;
    uint8_t num = 0;

    explicit SPVEnumList(const TransientArray<SPVEnum>& enums)
    {
        this->enums = enums.ptr;
        this->num = (uint8_t)enums.size;
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
        UInt64,
        Bool,
        Bool8 = Bool
    } type = Type::Float;
    union
    {
        float f[4];
        int32_t i[4];
        uint32_t ui[4];
        uint64_t ul[4];
        bool b[4];
    } data = {};
    bool linkDefined = false;
    uint8_t size = 1;

    static ConstantCreationInfo Float(float val)
    {
        ConstantCreationInfo ret;
        ret.type = Type::Float;
        ret.data.f[0] = val;
        return ret;
    }
    
    static ConstantCreationInfo Float32(float val)
    {
        ConstantCreationInfo ret;
        ret.type = Type::Float32;
        ret.data.f[0] = val;
        return ret;
    }

    static ConstantCreationInfo Float32(const SPIRVResult& val)
    {
        ConstantCreationInfo ret;
        ret.type = Type::Float32;
        ret.data.f[0] = val.literalValue.f32;
        return ret;
    }

    static ConstantCreationInfo Float16(float val)
    {
        ConstantCreationInfo ret;
        ret.type = Type::Float16;
        ret.data.f[0] = val;
        return ret;
    }

    static ConstantCreationInfo Float16(const SPIRVResult& val)
    {
        ConstantCreationInfo ret;
        ret.type = Type::Float16;
        ret.data.f[0] = val.literalValue.f16;
        return ret;
    }

    static ConstantCreationInfo UInt(uint32_t val)
    {
        ConstantCreationInfo ret;
        ret.type = Type::UInt;
        ret.data.ui[0] = val;
        return ret;
    }
    
    static ConstantCreationInfo UInt32(uint32_t val)
    {
        ConstantCreationInfo ret;
        ret.type = Type::UInt32;
        ret.data.ui[0] = val;
        return ret;
    }

    static ConstantCreationInfo UInt32(const SPIRVResult& val)
    {
        ConstantCreationInfo ret;
        ret.type = Type::UInt32;
        ret.data.ui[0] = val.literalValue.u32;
        return ret;
    }

    static ConstantCreationInfo UInt64(uint64_t val)
    {
        ConstantCreationInfo ret;
        ret.type = Type::UInt64;
        ret.data.ul[0] = val;
        return ret;
    }

    static ConstantCreationInfo UInt64(const SPIRVResult& val)
    {
        ConstantCreationInfo ret;
        ret.type = Type::UInt64;
        ret.data.ul[0] = val.literalValue.u64;
        return ret;
    }

    static ConstantCreationInfo UInt16(uint16_t val)
    {
        ConstantCreationInfo ret;
        ret.type = Type::UInt16;
        ret.data.ui[0] = val;
        return ret;
    }

    static ConstantCreationInfo UInt16(const SPIRVResult& val)
    {
        ConstantCreationInfo ret;
        ret.type = Type::UInt16;
        ret.data.ui[0] = val.literalValue.u16;
        return ret;
    }

    static ConstantCreationInfo Int(int32_t val)
    {
        ConstantCreationInfo ret;
        ret.type = Type::Int;
        ret.data.i[0] = val;
        return ret;
    }
    
    static ConstantCreationInfo Int32(int32_t val)
    {
        ConstantCreationInfo ret;
        ret.type = Type::Int32;
        ret.data.i[0] = val;
        return ret;
    }

    static ConstantCreationInfo Int32(const SPIRVResult& val)
    {
        ConstantCreationInfo ret;
        ret.type = Type::Int32;
        ret.data.i[0] = val.literalValue.i32;
        return ret;
    }

    static ConstantCreationInfo Int16(int16_t val)
    {
        ConstantCreationInfo ret;
        ret.type = Type::Int16;
        ret.data.i[0] = val;
        return ret;
    }

    static ConstantCreationInfo Int16(const SPIRVResult& val)
    {
        ConstantCreationInfo ret;
        ret.type = Type::Int16;
        ret.data.i[0] = val.literalValue.i16;
        return ret;
    }

    static ConstantCreationInfo Bool(bool b)
    {
        ConstantCreationInfo ret;
        ret.type = Type::Bool;
        ret.data.b[0] = b;
        return ret;
    }
    
    static ConstantCreationInfo Bool8(bool b)
    {
        ConstantCreationInfo ret;
        ret.type = Type::Bool;
        ret.data.b[0] = b;
        return ret;
    }

    static ConstantCreationInfo Bool8(const SPIRVResult& val)
    {
        ConstantCreationInfo ret;
        ret.type = Type::Bool8;
        ret.data.b[0] = val.literalValue.b8;
        return ret;
    }

    static ConstantCreationInfo ValueUnion(const ValueUnion& val)
    {
        ConstantCreationInfo ret;
        memcpy(ret.data.ul, val.ui, val.columnSize);
        ret.size = val.columnSize;
        switch (val.code)
        {
            case TypeCode::Float32:
                ret.type = Type::Float32;
                break;
            case TypeCode::Float16:
                ret.type = Type::Float16;
                break;
            case TypeCode::Int32:
                ret.type = Type::Int32;
                break;
            case TypeCode::Int16:
                ret.type = Type::Int16;
                break;
            case TypeCode::UInt32:
                ret.type = Type::UInt32;
                break;
            case TypeCode::UInt16:
                ret.type = Type::UInt16;
                break;
            case TypeCode::Bool8:
                ret.type = Type::Bool8;
                break;
            default:
                assert(false);
                return ConstantCreationInfo();
        }
        return ret;
    }
};

#define SPV_INSTRUCTION(name, code, words, dyn) constexpr SPVOp name { .str = #name, .c = code, .wordCount = words, .dynamicWords = dyn };
SPV_INSTRUCTION(OpNop, 1, 0, false)
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
SPV_INSTRUCTION(OpTypeAccelerationStructureKHR, 5341, 2, false)
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
SPV_INSTRUCTION(OpImageTexelPointer, 60, 6, false)
SPV_INSTRUCTION(OpAccessChain, 65, 4, true)
SPV_INSTRUCTION(OpLoad, 61, 4, true)
SPV_INSTRUCTION(OpStore, 62, 3, true)
SPV_INSTRUCTION(OpCopyMemory, 63, 3, true)
SPV_INSTRUCTION(OpCopyMemorySized, 64, 4, true)
SPV_INSTRUCTION(OpDecorate, 71, 3, true)
SPV_INSTRUCTION(OpMemberDecorate, 72, 4, true)
SPV_INSTRUCTION(OpVectorShuffle, 79, 5, true)
SPV_INSTRUCTION(OpCompositeConstruct, 80, 3, true)
SPV_INSTRUCTION(OpCompositeExtract, 81, 4, true)
SPV_INSTRUCTION(OpCompositeInsert, 82, 5, true)
SPV_INSTRUCTION(OpCopyObject, 83, 4, false)
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
SPV_INSTRUCTION(OpConvertPtrToU, 117, 4, false)
SPV_INSTRUCTION(OpConvertUToPtr, 120, 4, false)
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
SPV_INSTRUCTION(OpLogicalEqual, 164, 5, false)
SPV_INSTRUCTION(OpLogicalNotEqual, 165, 5, false)
SPV_INSTRUCTION(OpLogicalOr, 166, 5, false)
SPV_INSTRUCTION(OpLogicalAnd, 167, 5, false)
SPV_INSTRUCTION(OpLogicalNot, 168, 4, false)
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
SPV_INSTRUCTION(OpCopyLogical, 400, 4, false);
SPV_INSTRUCTION(OpGroupNonUniformQuadSwap, 366, 6, false)
SPV_INSTRUCTION(OpTerminateInvocation, 4416, 1, false)
SPV_INSTRUCTION(OpTraceRayKHR, 4445, 12, false)
SPV_INSTRUCTION(OpExecuteCallableKHR, 4446, 3, false)
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
SPV_ENUM(Float16, 9)
SPV_ENUM(Int64, 11)
SPV_ENUM(ImageBasic, 13)
SPV_ENUM(ImageReadWrite, 14)
SPV_ENUM(Int16, 22)
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
SPV_ENUM(VariablePointersStorageBuffer, 4441)
SPV_ENUM(VariablePointers, 4442)
SPV_ENUM(MeshShadingEXT, 5283)
SPV_ENUM(RayTracingKHR, 4479)
SPV_ENUM(Int64ImageEXT, 5016)
SPV_ENUM(ShaderViewportIndexLayerEXT, 5254)
SPV_ENUM(ComputeDerivativeGroupQuadsKHR, 5288)
SPV_ENUM(PhysicalStorageBufferAddresses, 5347)
SPV_ENUM(ComputeDerivativeGroupLinearKHR, 5350)
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
SPV_ENUM(Sample, 17)
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

namespace MemoryOperands
{
SPV_ENUM(Volatile, 0x1)
SPV_ENUM(Aligned, 0x2)
SPV_ENUM(Nontemporal, 0x4)
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
        case SPIRVResult::Storage::TexelPointer:
            return VariableStorage::Image;
        case SPIRVResult::Storage::StorageBuffer:
            return VariableStorage::StorageBuffer;
        case SPIRVResult::Storage::PhysicalBuffer:
            return VariableStorage::PhysicalStorageBuffer;
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
SPV_ENUM(PackSnorm4x8, 54)
SPV_ENUM(PackUnorm4x8, 55)
SPV_ENUM(PackSnorm2x16, 56)
SPV_ENUM(PackUnorm2x16, 57)
SPV_ENUM(UnpackSnorm2x16, 60)
SPV_ENUM(UnpackUnorm2x16, 61)
SPV_ENUM(UnpackSnorm4x8, 63)
SPV_ENUM(UnpackUnorm4x8, 64)
SPV_ENUM(Length, 66)
SPV_ENUM(Distance, 67)
SPV_ENUM(Cross, 68)
SPV_ENUM(Normalize, 69)
SPV_ENUM(Reflect, 71)
SPV_ENUM(Refract, 72)

#define SPV_STRING(str, value) const char* str = #value;
SPV_STRING(GLSL, GLSL.std.450)
SPV_ENUM(SPV_KHR_compute_shader_derivatives, 0)
SPV_ENUM(SPV_KHR_bit_instructions, 1)
SPV_ENUM(SPV_EXT_shader_viewport_index_layer, 2)
SPV_ENUM(SPV_KHR_physical_storage_buffer, 3)
SPV_ENUM(SPV_KHR_storage_buffer_storage_class, 4)
SPV_ENUM(SPV_KHR_ray_tracing, 5)
SPV_ENUM(SPV_KHR_variable_pointers, 6)

static const unsigned INVALID_ARG = 0xFFFFFFFF;



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
        uint32_t magic = 0x07230203;
        header.version.flags.leading = 0;
        header.version.flags.major = spvMajor;
        header.version.flags.minor = spvMinor;
        header.version.flags.trailing = 0;
        header.generatorVersion = generatorVersion;
        header.bound = bound;
        header.schema = 0;
            
        this->binaries[(uint32_t)Section::Top].push_back(magic);
        this->binaries[(uint32_t)Section::Top].push_back(header.version.bits);
        this->binaries[(uint32_t)Section::Top].push_back(header.generatorVersion);
        this->binaries[(uint32_t)Section::Top].push_back(header.bound);
        this->binaries[(uint32_t)Section::Top].push_back(header.schema);
        if (this->outputText)
        {
            char buf[64];
            uint32_t written = snprintf(buf, 64, "; Magic: 0x%.8lx (SPIRV Vulkan %d.%d)\n", header.version.bits, spvMajor, spvMinor);
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
        auto key = TStr(symbol.arg, "_", decoration.str, std::forward<const ARGS&>(args)...);
        if (this->decorations.Find(key) == this->decorations.end())
        {
            this->Instruction(OpDecorate, Section::Decorations, symbol, decoration, std::forward<const ARGS&>(args)...);
            this->decorations.Insert(FixedString(key));
        }
    }

    template<typename ...ARGS>
    bool HasDecoration(SPVArg symbol, SPVEnum decoration, const ARGS&... args)
    {
        auto key = TStr(symbol.arg, "_", decoration.str, std::forward<const ARGS&>(args)...);
        return this->decorations.Find(key) != this->decorations.end();
    }

    uint32_t String(const char* str)
    {
        TStr string(str);
        auto it = this->strings.Find(string);
        if (it == this->strings.end())
        {
            FixedString finalStr(string);
            uint32_t ret = this->MappedInstruction(OpString, SPVWriter::Section::DebugStrings, finalStr);
            this->strings.Insert(finalStr, ret);
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
            this->texts[(uint32_t)section].Append(TStr::Separated(SPVArg(c), "=", op.str, SPVArg(type)));

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
            this->texts[(uint32_t)section].Append(TStr::Separated(SPVArg(c), "=", op.str));

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
            this->texts[(uint32_t)section].Append(TStr::Separated(SPVArg{id}, "=", op.str));

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
        this->scopeStack.front().symbols[name.ToString()] = { .sym = nullptr, .value = SPIRVResult(0xFFFFFFFF, id) };

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
            this->texts[(uint32_t)section].Append(TStr::Separated(SPVArg{ id }, "=", op.str));

        (Append(args), ...);

        if (this->outputText)
            this->texts[(uint32_t)section].Append("\n");
    }

    template<typename ...ARGS>
    void LinkDefinedVariable(const TransientString& name, SPIRVResult id)
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
            this->texts[(uint32_t)this->section].Append(TStr::Compact(" \"", str, "\""));
        }
    }

    template<>
    void Append(const FixedString& str)
    {
        size_t len = str.len + 1;
        size_t lenInWords = std::ceil(len / 4.0f);
        uint32_t* strAsWords = (uint32_t*)str.buf;

        for (size_t i = 0; i < lenInWords; i++)
        {
            this->binaries[(uint32_t)this->section].push_back(strAsWords[i]);
        }
        char* lastInt = (char*)&this->binaries[(uint32_t)this->section].back();
        lastInt[3] = '\0';

        if (this->outputText)
        {
            this->texts[(uint32_t)this->section].Append(TStr::Compact(" \"", str, "\""));
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
            this->texts[(uint32_t)this->section].Append(TStr::Compact(" \"", str, "\""));
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
            int numWritten = snprintf(buf, 64, " %.17g", arg);
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
        this->binaries[(uint32_t)this->section].push_back(arg & 0xFFFFFFFFu);
        this->binaries[(uint32_t)this->section].push_back((arg >> 32) & 0xFFFFFFFFu);
        
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
                this->texts[(uint32_t)this->section].Append(buf, numWritten);
            }
        }
    }

    template<>
    void Append(const SPVEnumList& arg)
    {
        for (uint8_t it = 0; it < arg.num; it++)
            this->binaries[(uint32_t)this->section].push_back((uint32_t)arg.enums[it].c);
        if (this->outputText)
        {
            for (uint8_t it = 0; it < arg.num; it++)
            {
                this->texts[(uint32_t)this->section].Append(arg.enums[it].str);
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
    uint32_t ArgCount(const SPVEnumList& args)
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

    template<>
    uint32_t ArgCount(const uint64_t& arg)
    {
        return 2;
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
    PinnedSet<FixedString> decorations = 0xFFF;
    PinnedMap<FixedString, uint32_t> strings = 0xFFFF;
    PinnedMap<uint32_t, uint32_t> sizes = 0xFFFF;
};

//------------------------------------------------------------------------------
/**
*/
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
            return it->second.value.typeName;
        }
        scope++;
    }

    // If symbol isn't found in scope, create it
    uint32_t ret = gen->writer->counter;
    gen->writer->scopeStack.front().symbols[name.ToString()] = { .sym = nullptr, .value = SPIRVResult(0xFFFFFFFF, ret) };
    gen->writer->section = SPVWriter::Section::Declarations;
    gen->writer->MappedInstruction(op, SPVWriter::Section::Declarations, std::forward<const ARGS&>(args)..., SPVComment{ .str = name.buf });
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
bool
HasType(SPIRVGenerator* gen, const TransientString& name)
{
    auto scope = gen->writer->scopeStack.rbegin();
    while (scope != gen->writer->scopeStack.rend())
    {
        auto it = scope->symbols.find(name.ToString());
        if (it != scope->symbols.end())
        {
            return true;
        }
        scope++;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
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
                return it->second.value.name;
            }
            scope++;
        }
        gen->writer->scopeStack.front().symbols[name.ToString()] = { .sym = nullptr, .value = SPIRVResult(ret, type) };
    }
    else
    {
        auto& scope = gen->writer->scopeStack.back();
        auto it = scope.symbols.find(name.ToString());
        if (it != scope.symbols.end())
        {
            return it->second.value.name;
        }
        gen->writer->scopeStack.back().symbols[name.ToString()] = { .sym = nullptr, .value = SPIRVResult(ret, type) };
    }
    gen->writer->section = section;
    gen->writer->MappedInstruction(op, section, type, std::forward<const ARGS&>(args)..., SPVComment{ .str = name.buf });
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
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
                return it->second.value.name;
            }
            scope++;
        }
        gen->writer->scopeStack.front().symbols[name.ToString()] = { .sym = nullptr, .value = SPIRVResult(ret, type.typeName) };
    }
    else
    {
        auto& scope = gen->writer->scopeStack.back();
        auto it = scope.symbols.find(name.ToString());
        if (it != scope.symbols.end())
        {
            return it->second.value.name;
        }
        gen->writer->scopeStack.back().symbols[name.ToString()] = { .sym = nullptr, .value = SPIRVResult(ret, type.typeName) };
    }
    gen->writer->section = section;
    gen->writer->MappedInstruction(op, section, type.typeName, std::forward<const ARGS&>(args)..., SPVComment{ .str = name.buf });
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
void
BindSymbol(SPIRVGenerator* gen, SPVWriter::Section section, const TransientString& name, SPIRVResult bind)
{
    if (section == SPVWriter::Section::Declarations)
    {
        auto scope = gen->writer->scopeStack.rbegin();
        while (scope != gen->writer->scopeStack.rend())
        {
            auto it = scope->symbols.find(name.ToString());
            assert(it == scope->symbols.end() && "Symbol double bind is a generator programmer error");
            scope++;
        }
        gen->writer->scopeStack.front().symbols[name.ToString()] = { .sym = nullptr, .value = bind };
    }
    else
    {
        auto& scope = gen->writer->scopeStack.back();
        auto it = scope.symbols.find(name.ToString());
        assert(it == scope.symbols.end() && "Symbol double bind is a generator programmer error");
        gen->writer->scopeStack.back().symbols[name.ToString()] = { .sym = nullptr, .value = bind };
    }
}

//------------------------------------------------------------------------------
/**
*/
SymbolAssignment
GetSymbol(SPIRVGenerator* gen, const TransientString& name)
{
    SymbolAssignment ret = { .sym = nullptr, .value = SPIRVResult::Invalid() };
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

//------------------------------------------------------------------------------
/**
*/
template<typename ...ARGS>
uint32_t 
MappedInstruction(SPIRVGenerator* gen, SPVWriter::Section section, const SPVOp& op, uint32_t type, const ARGS&... args)
{
    uint32_t ret = gen->writer->counter;
    gen->writer->section = section;
    gen->writer->MappedInstruction(op, section, type, std::forward<const ARGS&>(args)...);
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
template<typename ...ARGS>
uint32_t 
MappedInstruction(SPIRVGenerator* gen, SPVWriter::Section section, const SPVOp& op, const ARGS&... args)
{
    uint32_t ret = gen->writer->counter;
    gen->writer->section = section;
    gen->writer->MappedInstruction(op, section, std::forward<const ARGS&>(args)...);
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
template<typename ...ARGS>
void
Instruction(SPIRVGenerator* gen, SPVWriter::Section section, const SPVOp& op, uint32_t type, const ARGS&... args)
{
    uint32_t ret = gen->writer->counter;
    gen->writer->section = section;
    gen->writer->Instruction(op, section, type, std::forward<const ARGS&>(args)...);
}

//------------------------------------------------------------------------------
/**
*/
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
    return const_len(arg.str) + 1;
}

GPULang::SPVArg operator""_spv(unsigned long long arg)
{
    return GPULang::SPVArg{ (uint32_t)arg };
};


SPIRVResult GenerateVariableSPIRV(const Compiler* compiler, SPIRVGenerator* generator, Symbol* symbol, bool isGlobal);
SPIRVResult GenerateExpressionSPIRV(const Compiler* compiler, SPIRVGenerator* generator, Expression* expr);
bool GenerateStatementSPIRV(const Compiler* compiler, SPIRVGenerator* generator, Statement* stat);
SPIRVResult GenerateConstantSPIRV(const Compiler* compiler, SPIRVGenerator* generator, ConstantCreationInfo info, uint32_t vectorSize = 1);

TransientString 
FormatExplicitLayout(const TransientString& name, const SPIRVGenerator::TypeState::TypeLayout layout)
{
    TransientString nameStr = TransientString(name);
    switch (layout)
    {
        case SPIRVGenerator::TypeState::TypeLayout::Explicit:
            nameStr = TransientString(nameStr, "_Explicit");
            break;
        case SPIRVGenerator::TypeState::TypeLayout::Interface:
            nameStr = TransientString(nameStr, "_Interface");
            break;
        default:
            break;
    }
    return nameStr;

}
uint32_t GenerateStructureSPIRV(const Compiler* compiler, SPIRVGenerator* generator, Symbol* symbol);

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
    , { TypeCode::UInt64, { "u64", "OpTypeInt 64 0", OpTypeInt, { 64u, 0u } } }
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

    if (code == TypeCode::UInt64)
        generator->writer->Capability(Capabilities::Int64);
    if (code == TypeCode::Float16)
        generator->writer->Capability(Capabilities::Float16);
    if (code == TypeCode::UInt16 || code == TypeCode::Int16)
        generator->writer->Capability(Capabilities::Int16);
    
    // Matrix
    if (rowSize > 1)
    {
        assert(vectorSize > 1);
        TStr vecType = TStr(type, "x", vectorSize);
        TStr matType = TStr(vecType, "x", rowSize);
        uint32_t vecTypeArg = AddType(generator, vecType, OpTypeVector, SPVArg{baseType}, vectorSize);
        baseType = AddType(generator, matType, OpTypeMatrix, SPVArg{vecTypeArg}, rowSize);
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
        // Check for potential struct member pointers
        if (type.address)
            scope = SPIRVResult::Storage::Function;

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
        else if (storage == Storage::RayHitAttribute)
            scope = SPIRVResult::Storage::RayHitAttribute;
        else if (storage == Storage::RayPayload)
            scope = SPIRVResult::Storage::RayPayload;
        else if (storage == Storage::RayPayloadInput)
            scope = SPIRVResult::Storage::RayPayloadInput;
        else if (storage == Storage::CallableData)
            scope = SPIRVResult::Storage::CallableData;
        else if (storage == Storage::CallableDataInput)
            scope = SPIRVResult::Storage::CallableDataInput;
        else if (storage == Storage::Uniform)
            if (type.mut)
                scope = SPIRVResult::Storage::StorageBuffer;
            else
                scope = SPIRVResult::Storage::Uniform;
        
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
    else if (typeSymbol->category == Type::SamplerStateCategory)
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
    else if (typeSymbol->category == Type::StructureCategory)
    {
        // Check for potential struct member pointers
        if (type.address)
            scope = SPIRVResult::Storage::Function;

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
    else if (typeSymbol->category == Type::AccelerationStructureCategory)
    {
        scope = SPIRVResult::Storage::UniformConstant;
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
    , const Type::FullType& type
    , Type* typeSymbol
    , bool returnAsPointer = false
)
{
    assert(generator->typeState.storage != SPIRVResult::Storage::Invalid);
    std::tuple<uint32_t, TStr> baseType;
    TransientString typeNameStr;
    std::vector<SPIRVResult::Indirection> indirections;
    ConstantString scopeString = SPIRVResult::ScopeToString(generator->typeState.storage);
    if (typeSymbol->category == Type::ScalarCategory || typeSymbol->category == Type::VoidCategory)
    {
        typeNameStr = typeSymbol->name;
        baseType = GenerateBaseTypeSPIRV(compiler, generator, typeSymbol->baseType, typeSymbol->columnSize, typeSymbol->rowSize);

        if (type.address)
        {
            // If we are generating a pointer type, then we need to generate the base type first
            //indirections.push_back(SPIRVResult::Pointer(0xFFFFFFFF, std::get<0>(baseType), generator->typeState.storage));
            baseType = GenerateBaseTypeSPIRV(compiler, generator, TypeCode::UInt64, 1);
            typeNameStr = "u64";

            generator->writer->Capability(Capabilities::Int64);
        }
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

        baseType = std::tie(name, gpuLangStr);
        typeNameStr = gpuLangStr;
    }
    else if (typeSymbol->category == Type::SampledTextureCategory)
    {
        uint32_t floatType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::Float, 1);
        auto handleTypeIt = handleTable.find(typeSymbol->baseType);
        auto [gpulangType, spirvFormatter] = handleTypeIt->second;

        std::string accessPattern = "";
        uint32_t sampleBits = 1, depthBits = 0;

        if (typeSymbol->baseType == TypeCode::SampledTexture1D || typeSymbol->baseType == TypeCode::SampledTexture1DArray)
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

        baseType = std::tie(name, gpuLangStr);
        typeNameStr = gpuLangStr;
    }
    else if (typeSymbol->category == Type::PixelCacheCategory)
    {
        generator->writer->Capability(Capabilities::InputAttachment);
        auto [floatType, floatBaseType] = GenerateBaseTypeSPIRV(compiler, generator, TypeCode::Float, 1);

        auto handleGenerator = generators.find(typeSymbol->baseType);
        uint32_t name = handleGenerator->second(generator, typeSymbol->name, floatType, 0, 0, ImageFormats::Unknown);

        auto handleTypeIt = handleTable.find(typeSymbol->baseType);
        auto [gpulangType, spirvFormatter] = handleTypeIt->second;
        
        baseType = std::tie(name, gpulangType);
        typeNameStr = gpulangType;
    }
    else if (typeSymbol->category == Type::SamplerStateCategory)
    {
        auto handleGenerator = generators.find(typeSymbol->baseType);
        uint32_t name = handleGenerator->second(generator, typeSymbol->name, 0, 0, 0, ImageFormats::Unknown);

        auto handleTypeIt = handleTable.find(typeSymbol->baseType);
        auto [gpulangType, spirvFormatter] = handleTypeIt->second;
        baseType = std::tie(name, gpulangType);
        typeNameStr = gpulangType;
    }
    else if (typeSymbol->category == Type::EnumCategory)
    {
        baseType = GenerateBaseTypeSPIRV(compiler, generator, TypeCode::UInt, 1);
    }
    else if (typeSymbol->category == Type::AccelerationStructureCategory)
    {
        typeNameStr = "accelerationStructure"_c;
        uint32_t name = AddType(generator, typeNameStr, OpTypeAccelerationStructureKHR);
        baseType = std::tie(name, typeNameStr);
    }
    else if (typeSymbol->category == Type::StructureCategory)
    {
        SPIRVGenerator::TypeState::TypeLayout oldLayout = generator->typeState.layout;

        // If we are generating an interface, but there are array types sitting on top, then the struct is a subtype that needs explicit layout
        if (generator->typeState.layout == SPIRVGenerator::TypeState::TypeLayout::Interface)
        {
            if (type.modifiers.size > 0)
            {
                for (int i = type.modifiers.size - 1; i >= 0; i--)
                {
                    if (type.modifiers[i] == Type::FullType::Modifier::Array)
                    {
                        generator->typeState.layout = SPIRVGenerator::TypeState::TypeLayout::Explicit;
                        break;
                    }
                }
            }
        }
        typeNameStr = FormatExplicitLayout(typeSymbol->name, generator->typeState.layout);

        auto t = GetSymbol(generator, typeNameStr);
        if (t.value == SPIRVResult::Invalid())
        {
            // type is not generated by a previous structure declaration
            
            t.value.typeName = GenerateStructureSPIRV(compiler, generator, static_cast<Structure*>(typeSymbol));
        }
        generator->typeState.layout = oldLayout;

        // The scope string is already consumed by the struct
        baseType = std::tie(t.value.typeName, typeSymbol->name);

        if (type.address)
        {
            // When generating an addr type, make a higher type as u64 and parent the actual type
            //indirections.push_back(SPIRVResult::Pointer(0xFFFFFFFF, std::get<0>(baseType), generator->typeState.storage));
            baseType = GenerateBaseTypeSPIRV(compiler, generator, TypeCode::UInt64, 1);
            typeNameStr = "u64";

        }
    }

    bool isStructPadded = false;
    bool logicallyAddressed = generator->typeState.storage == SPIRVResult::Storage::UniformConstant
        || generator->typeState.storage == SPIRVResult::Storage::Image
        || generator->typeState.storage == SPIRVResult::Storage::Sampler
        || generator->typeState.storage == SPIRVResult::Storage::MutableImage;

    SPVEnum scopeEnum = ScopeToEnum(generator->typeState.storage);

    std::vector<SPIRVResult::Indirection> paddingIndirections;
    
    uint32_t lastStructType = std::get<0>(baseType);
    if (type.modifiers.size > 0)
    {
        for (int i = type.modifiers.size - 1; i >= 0; i--)
        {
            auto [typeName, gpulangType] = baseType;
            const Type::FullType::Modifier& mod = type.modifiers[i];
            if (mod == Type::FullType::Modifier::Pointer && !logicallyAddressed)
            {
                gpulangType = TStr("ptr_", gpulangType);
                typeNameStr = TStr("ptr_", typeNameStr, "_", scopeString);
                if (scopeString != nullptr)
                {
                    gpulangType.Append("_");
                    gpulangType.Append(scopeString);
                }

                uint32_t prevType = typeName;
                typeName = AddType(generator, typeNameStr, OpTypePointer, scopeEnum, SPVArg{ typeName });
                indirections.push_back(SPIRVResult::Pointer(typeName, prevType, generator->typeState.storage));
                baseType = std::tie(typeName, gpulangType);
            }
            else if (mod == Type::FullType::Modifier::Array)
            {
                if (type.modifierValues[i] == nullptr)
                {
                    SPIRVGenerator::TypeState::TypeLayout oldLayout = generator->typeState.layout;
                    if (generator->typeState.layout == SPIRVGenerator::TypeState::TypeLayout::Interface)
                        generator->typeState.layout = SPIRVGenerator::TypeState::TypeLayout::Explicit;
                    gpulangType = TStr("[]_", gpulangType);
                    typeNameStr = FormatExplicitLayout(gpulangType, generator->typeState.layout);
                    uint32_t structType = typeName;

                    bool newType = !HasType(generator, typeNameStr);
                    typeName = AddType(generator, typeNameStr, OpTypeRuntimeArray, SPVArg{ typeName });
                    uint32_t prevType = typeName;
                    baseType = std::tie(typeName, gpulangType);

                    if (newType)
                    {
                        if (generator->typeState.layout == SPIRVGenerator::TypeState::TypeLayout::Explicit || generator->typeState.layout == SPIRVGenerator::TypeState::TypeLayout::Interface)
                        {
                            if (typeSymbol->category == Type::StructureCategory)
                            {
                                Structure::__Resolved* strucRes = Symbol::Resolved(static_cast<Structure*>(typeSymbol));
                                generator->writer->Decorate(SPVArg(typeName), Decorations::ArrayStride, strucRes->byteSize);
                            }
                            else
                            {
                                if (!logicallyAddressed)
                                    generator->writer->Decorate(SPVArg(typeName), Decorations::ArrayStride, typeSymbol->CalculateStride());
                            }
                        }
                    }

                    generator->typeState.layout = oldLayout;
                    
                    if (generator->typeState.layout == SPIRVGenerator::TypeState::TypeLayout::Interface && typeSymbol->category == Type::StructureCategory)
                    {
                        gpulangType = TStr("struct_", gpulangType);
                        typeNameStr = FormatExplicitLayout(gpulangType, generator->typeState.layout);
                        typeName = AddType(generator, typeNameStr, OpTypeStruct, SPVArg{ typeName });

                        lastStructType = typeName;
                        baseType = std::tie(typeName, gpulangType);

                        // If this type is bound (interface), set storage to StorageBuffer
                        if (newType)
                        {
                            generator->writer->MemberDecorate(SPVArg(typeName), 0, Decorations::Offset, 0);
                            generator->writer->Decorate(SPVArg(typeName), Decorations::Block);
                        }
                        generator->typeState.storage = SPIRVResult::Storage::StorageBuffer;
                        scopeString = SPIRVResult::ScopeToString(SPIRVResult::Storage::StorageBuffer);
                        scopeEnum = ScopeToEnum(generator->typeState.storage);
                        generator->typeState.layout = SPIRVGenerator::TypeState::TypeLayout::Explicit;

                        SPIRVResult zero = GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::UInt(0));
                        paddingIndirections.push_back(SPIRVResult::Access(zero.name, prevType, typeName));

                        isStructPadded = true;
                    }
                }
                else
                {
                    uint32_t size;
                    ValueUnion val;
                    type.modifierValues[i]->EvalValue(val);
                    val.Store(size);

                    SPIRVGenerator::TypeState::TypeLayout oldLayout = generator->typeState.layout;
                    if (generator->typeState.layout == SPIRVGenerator::TypeState::TypeLayout::Interface)
                        generator->typeState.layout = SPIRVGenerator::TypeState::TypeLayout::Explicit;

                    gpulangType = TStr("[", size, "]_", gpulangType);
                    typeNameStr = FormatExplicitLayout(gpulangType, generator->typeState.layout);

                    uint32_t intType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::Int, 1);

                    SPIRVResult arraySizeConstant = GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::Int(size));
                    bool newType = !HasType(generator, typeNameStr);
                    typeName = AddType(generator, typeNameStr, OpTypeArray, SPVArg{ typeName }, arraySizeConstant);
                    //indirections.push_back(SPIRVResult::Array(typeName, prevType, arraySizeConstant.typeName, arraySizeConstant.name));
                    baseType = std::tie(typeName, gpulangType);
                    uint32_t prevType = typeName;

                    if (newType)
                    {
                        if (generator->typeState.layout == SPIRVGenerator::TypeState::TypeLayout::Explicit || generator->typeState.layout == SPIRVGenerator::TypeState::TypeLayout::Interface)
                        {
                            if (typeSymbol->category == Type::StructureCategory)
                            {
                                Structure::__Resolved* strucRes = Symbol::Resolved(static_cast<Structure*>(typeSymbol));
                                generator->writer->Decorate(SPVArg(typeName), Decorations::ArrayStride, strucRes->byteSize);
                            }
                            else
                            {
                                if (!logicallyAddressed)
                                    generator->writer->Decorate(SPVArg(typeName), Decorations::ArrayStride, typeSymbol->CalculateStride());
                            }
                        }
                    }

                    generator->typeState.layout = oldLayout;

                    // if this is an interface, wrap it in a struct to allow interface binding
                    if (generator->typeState.layout == SPIRVGenerator::TypeState::TypeLayout::Interface && typeSymbol->category == Type::StructureCategory)
                    {

                        gpulangType = TStr("struct_", gpulangType);
                        typeNameStr = FormatExplicitLayout(gpulangType, generator->typeState.layout);
                        typeName = AddType(generator, typeNameStr, OpTypeStruct, SPVArg{ typeName });
                        lastStructType = typeName;
                        baseType = std::tie(typeName, gpulangType);
                        if (newType)
                        {
                            generator->writer->MemberDecorate(SPVArg(typeName), 0, Decorations::Offset, 0);
                            generator->writer->Decorate(SPVArg(typeName), Decorations::Block);
                        }
                        generator->typeState.storage = SPIRVResult::Storage::StorageBuffer;
                        scopeString = SPIRVResult::ScopeToString(SPIRVResult::Storage::StorageBuffer);
                        scopeEnum = ScopeToEnum(generator->typeState.storage);
                        generator->typeState.layout = SPIRVGenerator::TypeState::TypeLayout::Explicit;

                        SPIRVResult zero = GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::UInt(0));
                        paddingIndirections.push_back(SPIRVResult::Access(zero.name, prevType, typeName));

                        isStructPadded = true;
                    }
                }
            }
        }
    }

    if (logicallyAddressed || returnAsPointer)
    {
        auto [typeName, gpulangType] = baseType;
        gpulangType = TStr("ptr_", gpulangType);
        typeNameStr = TStr("ptr_", typeNameStr, "_", scopeString);

        uint32_t prevType = typeName;
        typeName = AddType(generator, typeNameStr, OpTypePointer, scopeEnum, SPVArg{ typeName });
        indirections.push_back(SPIRVResult::Pointer(typeName, prevType, generator->typeState.storage));
        baseType = std::tie(typeName, gpulangType);

        // Tack on scope
    }

    // If struct padded, add an indirection after the pointer
    if (isStructPadded)
    {
        indirections.insert(indirections.end(), paddingIndirections.begin(), paddingIndirections.end());
    }

    auto ret = SPIRVResult(INVALID_ARG, std::get<0>(baseType), false, false, generator->typeState.storage);
    ret.indirections = indirections;
    ret.isStructPadded = isStructPadded;
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
TransientString
ToSPIRVTypeString(
    const Compiler* compiler,
    SPIRVGenerator* generator,
    const Type::FullType& type,
    Type* typeSymbol
)
{
    TransientString ret;
    if (type.address)
    {
        return "u64";
    }
    if (typeSymbol->category == Type::ScalarCategory || typeSymbol->category == Type::VoidCategory)
    {
        auto it = scalarTable.find(typeSymbol->baseType);
        assert(it != scalarTable.end());
        auto [str, spirvstr, op, args] = it->second;
        ret = std::get<0>(it->second);

        // Matrix
        if (typeSymbol->rowSize > 1)
        {
            assert(typeSymbol->rowSize > 1);
            ret = TStr(ret, "x", typeSymbol->rowSize);
            ret = TStr(ret, "x", typeSymbol->columnSize);
        }
        else if (typeSymbol->columnSize > 1)
        {
            // Vector
            ret = TStr(ret, "x", typeSymbol->columnSize);
        }

    }
    else if (typeSymbol->category == Type::TextureCategory)
    {
        auto handleTypeIt = handleTable.find(typeSymbol->baseType);
        auto [gpulangType, spirvFormatter] = handleTypeIt->second;

        std::string accessPattern = "";
        uint32_t sampleBits, depthBits = 0;
        if (type.IsMutable())
            sampleBits = 2;
        else
            sampleBits = 1;

        uint32_t floatType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::Float, 1);
        auto [spirvFormat, extension, format, cap] = imageFormatToSpirvType[type.imageFormat];
        ret = spirvFormatter(floatType, depthBits, sampleBits, spirvFormat.buf);
    }
    else if (typeSymbol->category == Type::SampledTextureCategory)
    {
        uint32_t floatType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::Float, 1);
        auto handleTypeIt = handleTable.find(typeSymbol->baseType);
        auto [gpulangType, spirvFormatter] = handleTypeIt->second;

        std::string accessPattern = "";
        uint32_t sampleBits = 1, depthBits = 0;

        auto [spirvFormat, extension, format, cap] = imageFormatToSpirvType[type.imageFormat];
        TStr gpuLangStr = TStr::Compact(spirvFormat, "_", gpulangType, "Sample_", sampleBits, "Depth_", depthBits);

        ret = spirvFormatter(floatType, depthBits, sampleBits, spirvFormat.buf);
    }
    else if (typeSymbol->category == Type::SamplerStateCategory)
    {
        auto handleTypeIt = handleTable.find(typeSymbol->baseType);
        auto [gpulangType, spirvFormatter] = handleTypeIt->second;
        ret = gpulangType;
    }
    else if (typeSymbol->category == Type::EnumCategory)
    {
        ret = "u32";
    }
    else if (typeSymbol->category == Type::StructureCategory)
    {
        ret = FormatExplicitLayout(typeSymbol->name, generator->typeState.layout);
    }

    bool isStructPadded = false;

    for (size_t i = 0; i < type.modifiers.size; i++)
    {
        const Type::FullType::Modifier& mod = type.modifiers[i];
        if (mod == Type::FullType::Modifier::Pointer)
        {
            ret = TStr("ptr_", ret);
        }
        else if (mod == Type::FullType::Modifier::Array)
        {
            if (type.modifierValues[i] == nullptr)
            {
                ret = TStr("[]_struct_", ret);
                isStructPadded = true;
            }
            else
            {
                uint32_t size;
                ValueUnion val;
                type.modifierValues[i]->EvalValue(val);
                val.Store(size);
                ret = TStr("[", size, "]_", ret);
            }
        }
    }
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
    SPIRVResult res[4] = { SPIRVResult::Invalid() };
    uint32_t baseType;
    uint32_t vecType = INVALID_ARG;

#define ADD_CONSTANT(short, ty, idx) \
if (generator->linkDefineEvaluation)\
{\
    TStr symbolName = TStr(info.data.ty[idx], "_", #short"_link-defined", generator->linkDefineSlot);\
    res[idx].name = AddSymbol(generator, symbolName, SPVWriter::Section::Declarations, OpSpecConstant, baseType, info.data.ty[idx]);\
}\
else\
{\
    TStr symbolName = TStr(info.data.ty[idx], "_", #short);\
    res[idx].name = AddSymbol(generator, symbolName, SPVWriter::Section::Declarations, OpConstant, baseType, info.data.ty[idx]);\
}
    

    switch (info.type)
    {
        case ConstantCreationInfo::Type::UInt32:
        {
            baseType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::UInt, 1);
            if (vectorSize > 1)
                vecType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::UInt, vectorSize);
            for (uint8_t idx = 0; idx < info.size; idx++)
            {
                res[idx].typeName = baseType;
                ADD_CONSTANT(u32, ui, idx)
            }
            break;
        }
        case ConstantCreationInfo::Type::UInt16:
        {
            baseType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::UInt16, 1);
            if (vectorSize > 1)
                vecType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::UInt16, vectorSize);
            for (uint8_t idx = 0; idx < info.size; idx++)
            {
                res[idx].typeName = baseType;
                ADD_CONSTANT(u16, ui, idx)
            }
            
            break;
        }
        case ConstantCreationInfo::Type::UInt64:
        {
            baseType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::UInt64, 1);
            if (vectorSize > 1)
                vecType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::UInt64, vectorSize);
            for (uint8_t idx = 0; idx < info.size; idx++)
            {
                res[idx].typeName = baseType;
                ADD_CONSTANT(u64, ul, idx) // not a bug, SPIRV expects 32 bit words for constants, so this must be truncated
            }
            break;
        }
        case ConstantCreationInfo::Type::Int32:
        {
            baseType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::Int, 1);
            if (vectorSize > 1)
                vecType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::Int, vectorSize);
            for (uint8_t idx = 0; idx < info.size; idx++)
            {
                res[idx].typeName = baseType;
                ADD_CONSTANT(i32, i, idx)
            }
            break;
        }
        case ConstantCreationInfo::Type::Int16:
        {
            baseType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::Int16, 1);
            if (vectorSize > 1)
                vecType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::Int16, vectorSize);
            for (uint8_t idx = 0; idx < info.size; idx++)
            {
                res[idx].typeName = baseType;
                ADD_CONSTANT(i16, i, idx)
            }
            break;
        }
        case ConstantCreationInfo::Type::Float32:
        {
            baseType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::Float, 1);
            if (vectorSize > 1)
                vecType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::Float, vectorSize);
            for (uint8_t idx = 0; idx < info.size; idx++)
            {
                res[idx].typeName = baseType;
                ADD_CONSTANT(f32, f, idx)
            }
            break;
        }
        case ConstantCreationInfo::Type::Float16:
        {
            baseType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::Float16, 1);
            if (vectorSize > 1)
                vecType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::Float16, vectorSize);
            for (uint8_t idx = 0; idx < info.size; idx++)
            {
                res[idx].typeName = baseType;
                ADD_CONSTANT(f16, f, idx)
            }
            break;
        }
        case ConstantCreationInfo::Type::Bool8:
        {
            baseType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::Bool, 1);
            if (vectorSize > 1)
                vecType = GeneratePODTypeSPIRV(compiler, generator, TypeCode::Bool, vectorSize);
            for (uint8_t idx = 0; idx < info.size; idx++)
            {
                res[idx].typeName = baseType;
                if (generator->linkDefineEvaluation)
                {
                    TStr symbolName = TStr::Compact(info.data.b[idx] ? "true" : "false", "_", "link_defined", generator->linkDefineSlot);
                    res[idx].name = AddSymbol(generator, symbolName, SPVWriter::Section::Declarations, info.data.b[idx] ? OpSpecConstantTrue : OpSpecConstantFalse, baseType);
                }
                else
                {
                    TStr symbolName = TStr::Compact(info.data.b[idx] ? "true" : "false");
                    res[idx].name = AddSymbol(generator, symbolName, SPVWriter::Section::Declarations, info.data.b[idx] ? OpConstantTrue : OpConstantFalse, baseType);
                }
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
            uint8_t splatIndex = min(i, info.size - 1);
            vectorArgs.Append(SPVArg{ res[splatIndex].name });
            name.Append(SPVArg(res[splatIndex].name));
            if (i < vectorSize - 1)
                name.Append(" ");
        }
        
        // For vectors, reuse index 0 as the composite constant
        if (generator->linkDefineEvaluation)
        {
            res[0].name = AddSymbol(generator, TStr("{", name, "}_link_defined"), SPVWriter::Section::Declarations, OpSpecConstantComposite, vecType, SPVArgList{ vectorArgs });
        }
        else
        {
            res[0].name = AddSymbol(generator, TStr("{", name, "}"), SPVWriter::Section::Declarations, OpConstantComposite, vecType, SPVArgList{ vectorArgs });
        }
        res[0].typeName = vecType;
    }
    // Use index 0 as our final result
    res[0].isValue = true;
    res[0].isConst = true;
    res[0].isSpecialization = generator->linkDefineEvaluation;
    return res[0];
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
    )
{
    SPIRVResult returnType = GenerateTypeSPIRV(compiler, generator, type, typeSymbol, true);

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


struct IndirectionState
{
    uint32_t needsLoad : 1;
    uint32_t needsAlignment : 1;
    uint32_t volatileData : 1;
    uint32_t nonTemporal : 1;
    uint32_t alignment : 28;

    IndirectionState()
        : needsLoad(false)
        , needsAlignment(false)
        , volatileData(false)
        , nonTemporal(false)
        , alignment(0)
    {
    }
};

//------------------------------------------------------------------------------
/**
    Unrolls the indirections on a SPIRVResult.
    The indirection list is segmented into two parts, first is the type indirections which come at the beginning of the list. The last item in this list indicates the final pointer type.
    The second part are the array, struct and address accesses, all of which results in the final pointer type.

    Any subsequent indirection unroll will peel of one more pointer type from the stack. If the resulting indirection list is empty, then the final type is the data type.
*/
uint32_t
IndirectionUnrollSPIRV(const Compiler* compiler, SPIRVGenerator* generator, uint32_t name, uint32_t& type, SPIRVResult::Storage& storage, std::vector<SPIRVResult::Indirection>& chain, IndirectionState& state)
{
    uint32_t val = name;

    state.needsLoad = false;
    state.needsAlignment = false;
    state.alignment = 0;

    TransientArray<SPVArg> accessChain(chain.size());
    SPIRVResult::Indirection::Type currentType = SPIRVResult::Indirection::Type::Access;
    uint32_t ptrType = type;
    auto it = chain.begin();
    auto startType = chain.begin();
    while (it != chain.end())
    {
        const SPIRVResult::Indirection& indirection = *it;

        TransientArray<SPVEnum> memOps(2);
        if (state.volatileData)
            memOps.Append(MemoryOperands::Volatile);
        if (state.nonTemporal)
            memOps.Append(MemoryOperands::Nontemporal);

        SPVEnumList memoryOperands(memOps);

        if (indirection.type == SPIRVResult::Indirection::Type::Pointer)
        {
            // The last pointer in the list will be the type loaded by the rest of the expression
            startType = it;
            type = indirection.pointerInfo.dataType;
            ptrType = indirection.pointerInfo.ptrType;
            storage = (SPIRVResult::Storage)indirection.pointerInfo.storage;
            state.needsLoad = true;
            //val = generator->writer->MappedInstruction(OpLoad, SPVWriter::Section::LocalFunction, indirection.pointerInfo.dataType, SPVArg(val));
        }
        else if (indirection.type == SPIRVResult::Indirection::Type::Access)
        {
            // Access chain expects a pointer, so no need to load the pointer here
            accessChain.Append(SPVArg(indirection.accessInfo.offset));
            type = indirection.accessInfo.dataType;
            ptrType = indirection.accessInfo.ptrType;

            // If last part of chain or next is different type, flush access chain
            if (it+1 == chain.end())
            {
                val = generator->writer->MappedInstruction(OpAccessChain, SPVWriter::Section::LocalFunction, ptrType, SPVArg(val), SPVArgList(accessChain));
                accessChain.Clear();
            }
            else if ((it+1)->type != SPIRVResult::Indirection::Type::Access)
            {
                val = generator->writer->MappedInstruction(OpAccessChain, SPVWriter::Section::LocalFunction, ptrType, SPVArg(val), SPVArgList(accessChain));
                accessChain.Clear();
            }
            //state.needsLoad = false;
        }
        else if (indirection.type == SPIRVResult::Indirection::Type::Address)
        {
            // If the previous value is a pointer, we first need to load the u64 address
            if (state.needsLoad)
            {
                if (!state.needsAlignment)
                    val = generator->writer->MappedInstruction(OpLoad, SPVWriter::Section::LocalFunction, type, SPVArg(val), memoryOperands);
                else
                {
                    val = generator->writer->MappedInstruction(OpLoad, SPVWriter::Section::LocalFunction, type, SPVArg(val), MemoryOperands::Aligned, state.alignment, memoryOperands);
                    state.alignment = 0;
                    state.needsAlignment = false;
                }
            }

            ptrType = indirection.addressInfo.ptrType;
            state.alignment = indirection.addressInfo.alignment;
            uint32_t offset = indirection.addressInfo.offset;

            if (offset != 0xFFFFFFFF)
                val = generator->writer->MappedInstruction(OpIAdd, SPVWriter::Section::LocalFunction, type, SPVArg(val), SPVArg(offset));
            val = generator->writer->MappedInstruction(OpConvertUToPtr, SPVWriter::Section::LocalFunction, ptrType, SPVArg(val));

            type = indirection.addressInfo.dataType;
            state.needsAlignment = true;
            state.needsLoad = true;
        }
        it++;

        currentType = indirection.type;
    }

    // Erase the processed indirections, leaving the initial pointer indirection (if any)
    chain.erase(startType, chain.end());
    
    return val;
}


//------------------------------------------------------------------------------
/**
*/
SPIRVResult
LoadValueSPIRV(const Compiler* compiler, SPIRVGenerator* generator, SPIRVResult arg)
{
    if (arg.isLiteral)
    {
        switch (arg.literalValue.type)
        {
        case TypeCode::Float32:
            arg = GenerateConstantSPIRV(compiler, generator, GPULang::ConstantCreationInfo::Float32(arg.literalValue.f32));
            break;
        case TypeCode::Float16:
            arg = GenerateConstantSPIRV(compiler, generator, GPULang::ConstantCreationInfo::Float16(arg.literalValue.f16));
            break;
        case TypeCode::Int32:
            arg = GenerateConstantSPIRV(compiler, generator, GPULang::ConstantCreationInfo::Int32(arg.literalValue.i32));
            break;
        case TypeCode::Int16:
            arg = GenerateConstantSPIRV(compiler, generator, GPULang::ConstantCreationInfo::Int16(arg.literalValue.i16));
            break;
        case TypeCode::UInt64:
            arg = GenerateConstantSPIRV(compiler, generator, GPULang::ConstantCreationInfo::UInt64(arg.literalValue.u64));
            break;
        case TypeCode::UInt32:
            arg = GenerateConstantSPIRV(compiler, generator, GPULang::ConstantCreationInfo::UInt32(arg.literalValue.u32));
            break;
        case TypeCode::UInt16:
            arg = GenerateConstantSPIRV(compiler, generator, GPULang::ConstantCreationInfo::UInt16(arg.literalValue.u16));
            break;
        case TypeCode::Bool8:
            arg = GenerateConstantSPIRV(compiler, generator, GPULang::ConstantCreationInfo::Bool8(arg.literalValue.b8));
            break;
        }
        return arg;
    }

    uint32_t val = arg.name;
    uint32_t type = arg.typeName;
    SPIRVResult::Storage storage = arg.scope;
    bool isValue = arg.isValue;

    IndirectionState state;
    val = IndirectionUnrollSPIRV(compiler, generator, val, type, storage, arg.indirections, state);

    TransientArray<SPVEnum> memOps(2);
    if (state.volatileData)
        memOps.Append(MemoryOperands::Volatile);
    if (state.nonTemporal)
        memOps.Append(MemoryOperands::Nontemporal);

    SPVEnumList memoryOperands(memOps);

    // If the output value from the indirection is a pointer, load the value
    if (state.needsLoad)
    {
        if (!state.needsAlignment)
            val = generator->writer->MappedInstruction(OpLoad, SPVWriter::Section::LocalFunction, type, SPVArg(val), memoryOperands);
        else
            val = generator->writer->MappedInstruction(OpLoad, SPVWriter::Section::LocalFunction, type, SPVArg(val), MemoryOperands::Aligned, state.alignment, memoryOperands);
    }
    
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
    res.isValue = true; // The result is only a value if there are no more parent types (indirections)
    res.isConst = arg.isConst;
    res.isSpecialization = arg.isSpecialization;
    res.indirections = arg.indirections;
    res.derefs = 0;
    res.scope = storage;
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
    SPIRVResult::Storage storage = target.scope;
    
    IndirectionState state;
    assert(target.indirections.size() > 0);
    auto indirections = target.indirections;
    val = IndirectionUnrollSPIRV(compiler, generator, val, type, storage, indirections, state);
    assert(state.needsLoad); // For Store operations, we assume the value is a pointer (needs loading)

    TransientArray<SPVEnum> memOps(2);
    if (state.volatileData)
        memOps.Append(MemoryOperands::Volatile);
    if (state.nonTemporal)
        memOps.Append(MemoryOperands::Nontemporal);

    SPVEnumList memoryOperands(memOps);


    // Perform OpStore if source is a value, otherwise copy memory
    if (source.isValue)
    {
        if (target.indirections.back().pointerInfo.dataType != source.typeName)
        {
            source.name = generator->writer->MappedInstruction(OpCopyLogical, SPVWriter::Section::LocalFunction, target.indirections.back().pointerInfo.dataType, source);
        }
        
        if (!state.needsAlignment)
            generator->writer->Instruction(OpStore, SPVWriter::Section::LocalFunction, SPVArg(val), SPVArg(source.name), memoryOperands);
        else
            generator->writer->Instruction(OpStore, SPVWriter::Section::LocalFunction, SPVArg(val), SPVArg(source.name), MemoryOperands::Aligned, state.alignment, memoryOperands);
    }
    else
    {
        // If we copy between pointers, there is a chance the storage classes don't match, so we need to handle that
        if (target.indirections.back().pointerInfo.dataType != source.typeName)
        {
            // Load the source and do a logical copy conversion to the target storage class
            source = LoadValueSPIRV(compiler, generator, source);
            source.name = generator->writer->MappedInstruction(OpCopyLogical, SPVWriter::Section::LocalFunction, target.indirections.back().pointerInfo.dataType, source);

            if (!state.needsAlignment)
                generator->writer->Instruction(OpStore, SPVWriter::Section::LocalFunction, SPVArg(val), SPVArg(source.name), memoryOperands);
            else
                generator->writer->Instruction(OpStore, SPVWriter::Section::LocalFunction, SPVArg(val), SPVArg(source.name), MemoryOperands::Aligned, state.alignment, memoryOperands);
        }
        else
        {
            // If storages do match, do a simple memory copy
            if (!state.needsAlignment)
                generator->writer->Instruction(OpCopyMemory, SPVWriter::Section::LocalFunction, SPVArg(val), SPVArg(source.name), memoryOperands);
            else
                generator->writer->Instruction(OpCopyMemory, SPVWriter::Section::LocalFunction, SPVArg(val), SPVArg(source.name), MemoryOperands::Aligned, state.alignment, memoryOperands);
        }
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
GenerateCompositeSPIRV(const Compiler* compiler, SPIRVGenerator* generator, uint32_t returnType, const TransientArray<SPIRVResult>& args)
{
    TStr argList;
    bool isConst = true;
    bool isSpecialization = false;
    TransientArray<SPIRVResult> loadedArgs(args.size);
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
    TransientArray<SPIRVResult> splat(num);
    for (uint32_t i = 0; i < num; i++)
    {
        splat.Append(arg);
    }
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

    if (funcResolved->isReentrant)
        return;

    if (funcResolved->isEntryPoint && generator->entryPoint != func)
        return;

    if (funcResolved->isPrototype)
        return;

    generator->typeState.storage = SPIRVResult::Storage::Function;
    SPIRVResult returnName = GenerateTypeSPIRV(compiler, generator, func->returnType, static_cast<Type*>(funcResolved->returnTypeSymbol));
    generator->typeState.storage = SPIRVResult::Storage::Function;

    TStr typeArgs;
    TStr spvTypes;

    TransientArray<SPIRVResult::Storage> paramStorages(func->parameters.size);
    TransientArray<bool> paramLogicalAddressing(func->parameters.size);
    TransientArray<SPVArg> spvTypeArgs(func->parameters.size);
    TransientArray<SPIRVResult> paramTypes(func->parameters.size);
    TransientArray<TransientString> paramTypeNameStrings(func->parameters.size);
    for (auto param : func->parameters)
    {
        Variable::__Resolved* paramResolved = Symbol::Resolved(param);

        if (funcResolved->isEntryPoint)
        {
            GenerateVariableSPIRV(compiler, generator, param, true);
        }
        else
        {
            SPIRVResult::Storage storage = ResolveSPIRVVariableStorage(param->type, paramResolved->typeSymbol, paramResolved->storage, paramResolved->usageBits);
            paramStorages.Append(storage);

            bool logicallyAddressed =
                storage == SPIRVResult::Storage::UniformConstant
                || storage == SPIRVResult::Storage::Image
                || storage == SPIRVResult::Storage::Sampler
                || storage == SPIRVResult::Storage::MutableImage
                || storage == SPIRVResult::Storage::StorageBuffer && paramResolved->typeSymbol->category == Type::Category::StructureCategory
                || storage == SPIRVResult::Storage::Uniform && paramResolved->typeSymbol->category == Type::Category::StructureCategory
                || storage == SPIRVResult::Storage::PushConstant
                || storage == SPIRVResult::Storage::Output && !paramResolved->usageBits.flags.isEntryPointParameter
                ;
            paramLogicalAddressing.Append(logicallyAddressed);

            assert(generator->typeState.storage == SPIRVResult::Storage::Function);
            generator->typeState.storage = storage;

            SPIRVResult typeName;
            if (storage == SPIRVResult::Storage::Output && logicallyAddressed)
            {
                generator->typeState.storage = SPIRVResult::Storage::Function;
                paramStorages.back() = SPIRVResult::Storage::Function;
                typeName = GeneratePointerTypeSPIRV(compiler, generator, param->type, paramResolved->typeSymbol);
            }
            else
            {
                typeName = GenerateTypeSPIRV(compiler, generator, param->type, paramResolved->typeSymbol);
            }
            paramTypes.Append(typeName);
            auto name = ToSPIRVTypeString(compiler, generator, param->type, paramResolved->typeSymbol);
            paramTypeNameStrings.Append(name);
            generator->typeState.storage = SPIRVResult::Storage::Function;

            spvTypes.Append(SPVArg(typeName.typeName));
            spvTypes.Append(" ");
            spvTypeArgs.Append(SPVArg(typeName.typeName));
            typeArgs.Append(name);
            if (param != func->parameters.back())
                typeArgs.Append(",");
        }
    }
    
    TStr functionSymbolName = TStr::Compact("function_", typeArgs, "_", ToSPIRVTypeString(compiler, generator, func->returnType, funcResolved->returnTypeSymbol));
    uint32_t functionType = AddType(generator, functionSymbolName, OpTypeFunction, SPVArg{ returnName.typeName }, SPVArgList(spvTypeArgs));
    
    // TODO: Add inline/const/functional
    AddSymbol(generator, funcResolved->name, SPVWriter::Section::Functions, OpFunction, returnName, FunctionControl::None, SPVArg{functionType});

    generator->writer->PushScope();
    if (!funcResolved->isEntryPoint)
    {
        for (size_t i = 0; i < func->parameters.size; i++)
        {
            const auto& param = func->parameters[i];
            Variable::__Resolved* paramResolved = Symbol::Resolved(param);
            SPIRVResult::Storage storage = paramStorages[i];
            SPIRVResult varType = paramTypes[i];
            TransientString paramTypeName = paramTypeNameStrings[i];

            bool logicallyAddressed = paramLogicalAddressing[i];

            // If value is not logically addressed (such arguments are uniform, which is treated as constants for storage), generate a variable that points to the function parameter
            if (!logicallyAddressed)
            {
                uint32_t paramName = MappedInstruction(generator, SPVWriter::Section::Functions, OpFunctionParameter, varType.typeName, SPVComment{ param->name.c_str() });

                // Function parameters are values if they are not pointers
                ConstantString scope = SPIRVResult::ScopeToString(varType.scope);
                TStr argPtrType = TStr::Compact("ptr_", paramTypeName, "_", scope);
                uint32_t typePtrName = GPULang::AddType(generator, argPtrType, OpTypePointer, ScopeToEnum(varType.scope), SPVArg{ varType.typeName });
                uint32_t paramSymbol = generator->writer->MappedInstruction(OpVariable, SPVWriter::Section::VariableDeclarations, typePtrName, VariableStorage::Function, SPVComment(param->name.c_str()));

                SPIRVResult res;
                res.name = paramSymbol;
                res.typeName = typePtrName;
                res.indirections = varType.indirections;
                res.AddIndirection({ SPIRVResult::Pointer(typePtrName, varType.typeName, varType.scope) });
                res.isValue = false;

                BindSymbol(generator, SPVWriter::Section::VariableDeclarations, param->name, res);
                generator->writer->Instruction(OpStore, SPVWriter::Section::ParameterInitializations, SPVArg{ paramSymbol }, SPVArg{ paramName });
            }
            else
            {
                // If parameter is a logically addressed resource, 
                uint32_t paramName = MappedInstruction(generator, SPVWriter::Section::Functions, OpFunctionParameter, varType.typeName, SPVComment{ param->name.c_str() });
                SPIRVResult res;
                res.name = paramName;
                res.typeName = varType.typeName;
                res.indirections = varType.indirections;
                res.isValue = false;
                BindSymbol(generator, SPVWriter::Section::VariableDeclarations, param->name, res);
            }
        }
    }

    uint32_t label = generator->writer->MappedInstruction(OpLabel, SPVWriter::Section::Functions);

    generator->blockOpen = true;
    auto functionOverride = generator->evaluatingProgram->functionOverrides.Find(func);
    if (functionOverride != generator->evaluatingProgram->functionOverrides.end())
        GenerateStatementSPIRV(compiler, generator, functionOverride->second->ast);
    else if (func->ast != nullptr)
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
        return INVALID_ARG;
    
    uint32_t numVariables = 0;
    for (Symbol* sym : struc->symbols)
    {
        if (sym->symbolType == Symbol::SymbolType::VariableType)
            numVariables++;
    }
    uint32_t name = INVALID_ARG;
    uint32_t structName = generator->writer->Reserve();

    // If the generator is set to generate an interface, do so and set the state to emit explicitly laid out sub-types
    TransientString nameStr = FormatExplicitLayout(struc->name, generator->typeState.layout);
    bool boundAsInterface = generator->typeState.layout == SPIRVGenerator::TypeState::TypeLayout::Interface;
    if (boundAsInterface)
    {
        generator->writer->Decorate(SPVArg(structName), Decorations::Block);
        generator->typeState.layout = SPIRVGenerator::TypeState::TypeLayout::Explicit;
    }

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
            
            SPIRVResult varType = GenerateTypeSPIRV(compiler, generator, var->type, varResolved->typeSymbol);
            memberTypes.append(Format("%%%d ", varType.typeName));
            memberTypeArray.Append(SPVArg{ varType.typeName });

            // If this struct is generated for binding, decorate with offsets
            if (generator->typeState.layout == SPIRVGenerator::TypeState::Interface || generator->typeState.layout == SPIRVGenerator::TypeState::Explicit)
            {
                generator->writer->MemberDecorate(SPVArg{ structName }, i, Decorations::Offset, varResolved->structureOffset);
                if (varResolved->typeSymbol->IsMatrix())
                {
                    generator->writer->MemberDecorate(SPVArg{ structName }, i, Decorations::MatrixStride, varResolved->typeSymbol->CalculateStride() / varResolved->typeSymbol->rowSize);
                    generator->writer->MemberDecorate(SPVArg{ structName }, i, Decorations::ColMajor);
                }
            }
            offset += varResolved->typeSymbol->CalculateSize();
        }
    }

    auto loadIt = strucResolved->loadFunctions.begin();
    while (loadIt != strucResolved->loadFunctions.end())
    {
        generator->generatorIntrinsics[loadIt->second] = [fun = loadIt->second](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            assert(args.size() == 1);
            assert(!args[0].isValue);

            Function::__Resolved* funRes = Symbol::Resolved(fun);
            g->typeState.storage = SPIRVResult::Storage::StorageBuffer;
            g->typeState.layout = SPIRVGenerator::TypeState::TypeLayout::Explicit;
            SPIRVResult ptrReturnType = GeneratePointerTypeSPIRV(c, g, fun->returnType, funRes->returnTypeSymbol);
            g->typeState.layout = SPIRVGenerator::TypeState::TypeLayout::Relaxed;
            g->typeState.storage = SPIRVResult::Storage::Function;
            SPIRVResult accessedArg = args[0];
            accessedArg.typeName = ptrReturnType.typeName;
            return accessedArg;
        };
        loadIt++;
    }

    auto loadIndexedIt = strucResolved->loadIndexedFunctions.begin();
    while (loadIndexedIt != strucResolved->loadIndexedFunctions.end())
    {
        generator->generatorIntrinsics[loadIndexedIt->second] = [fun = loadIndexedIt->second](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            assert(args.size() == 2);
            assert(!args[0].isValue);
            std::vector<SPIRVResult::Indirection> index;
            SPIRVResult accessedArg = args[0];

            Function::__Resolved* funRes = Symbol::Resolved(fun);
            g->typeState.storage = SPIRVResult::Storage::StorageBuffer;
            g->typeState.layout = SPIRVGenerator::TypeState::TypeLayout::Explicit;
            SPIRVResult ptrReturnType = GeneratePointerTypeSPIRV(c, g, fun->returnType, funRes->returnTypeSymbol);
            g->typeState.layout = SPIRVGenerator::TypeState::TypeLayout::Relaxed;
            g->typeState.storage = SPIRVResult::Storage::Function;

            index = { SPIRVResult::Access(LoadValueSPIRV(c, g, args[1]).name, ptrReturnType.indirections[0].pointerInfo.ptrType, ptrReturnType.indirections[0].pointerInfo.dataType) };

            accessedArg.AddIndirection(index);
            accessedArg.typeName = ptrReturnType.typeName;
            return accessedArg;
        };
        loadIndexedIt++;
    }

    auto storeIt = strucResolved->storageFunctions.begin();
    while (storeIt != strucResolved->storageFunctions.end())
    {
        generator->generatorIntrinsics[storeIt->second] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            assert(args.size() == 2);
            assert(!args[0].isValue);
            StoreValueSPIRV(c, g, args[0], args[1]);
            return SPIRVResult::Invalid();
        };
        storeIt++;
    }

    auto storeIndexedIt = strucResolved->storageIndexedFunctions.begin();
    while (storeIndexedIt != strucResolved->storageIndexedFunctions.end())
    {
        generator->generatorIntrinsics[storeIndexedIt->second] = [fun = storeIndexedIt->second](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            assert(args.size() == 3);
            assert(!args[0].isValue);
            std::vector<SPIRVResult::Indirection> index;
            SPIRVResult accessedArg = args[0];

            Variable::__Resolved* firstArgRes = Symbol::Resolved(fun->parameters[0]);
            Type::FullType temp = fun->parameters[0]->type;
            temp.modifiers.size = 0;
            temp.modifierValues.size = 0;
            g->typeState.storage = SPIRVResult::Storage::StorageBuffer;
            g->typeState.layout = SPIRVGenerator::TypeState::TypeLayout::Explicit;
            SPIRVResult ptrToDataType = GeneratePointerTypeSPIRV(c, g, temp, firstArgRes->typeSymbol);
            g->typeState.layout = SPIRVGenerator::TypeState::TypeLayout::Relaxed;
            g->typeState.storage = SPIRVResult::Storage::Function;

            index = { SPIRVResult::Access(LoadValueSPIRV(c, g, args[1]).name, ptrToDataType.indirections[0].pointerInfo.ptrType, ptrToDataType.indirections[0].pointerInfo.dataType) };

            accessedArg.AddIndirection(index);
            accessedArg.typeName = ptrToDataType.typeName;
            StoreValueSPIRV(c, g, accessedArg, args[2]);
            return SPIRVResult::Invalid();
        };
        storeIndexedIt++;
    }

    auto refIndexedIt = strucResolved->getReferenceFunctions.begin();
    while (refIndexedIt != strucResolved->getReferenceFunctions.end())
    {
        generator->generatorIntrinsics[refIndexedIt->second] = [fun = refIndexedIt->second](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            assert(args.size() == 2);
            assert(!args[0].isValue);
            std::vector<SPIRVResult::Indirection> index;
            SPIRVResult accessedArg = args[0];

            Function::__Resolved* funRes = Symbol::Resolved(fun);
            g->typeState.storage = SPIRVResult::Storage::StorageBuffer;
            g->typeState.layout = SPIRVGenerator::TypeState::TypeLayout::Explicit;
            SPIRVResult ptrReturnType = GeneratePointerTypeSPIRV(c, g, fun->returnType, funRes->returnTypeSymbol);
            g->typeState.layout = SPIRVGenerator::TypeState::TypeLayout::Relaxed;
            g->typeState.storage = SPIRVResult::Storage::Function;

            index = { SPIRVResult::Access(LoadValueSPIRV(c, g, args[1]).name, ptrReturnType.indirections[0].pointerInfo.ptrType, ptrReturnType.indirections[0].pointerInfo.dataType) };
            
            accessedArg.AddIndirection(index);
            accessedArg.typeName = ptrReturnType.typeName;
            return accessedArg;
        };
        refIndexedIt++;
    }

    generator->writer->ReservedType(OpTypeStruct, nameStr, SPVWriter::Section::Declarations, structName, SPVArgList(memberTypeArray), SPVComment{ .str = nameStr.c_str() });
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
    SPIRVResult::Storage storage = ResolveSPIRVVariableStorage(fullType, samplerTypeSymbol, Storage::Uniform);
    generator->typeState.storage = storage;
    SPIRVResult samplerType = GeneratePointerTypeSPIRV(compiler, generator, fullType, samplerTypeSymbol);
    generator->typeState.storage = SPIRVResult::Storage::Function;

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

        generator->writer->Decorate(SPVArg(name), Decorations::DescriptorSet, (uint32_t)samplerResolved->group);
        generator->writer->Decorate(SPVArg(name), Decorations::Binding, (uint32_t)samplerResolved->binding);
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
                    SPIRVResult loadedArg0 = LoadValueSPIRV(c, g, args[0]);
                    SPIRVResult loadedArg1 = LoadValueSPIRV(c, g, args[1]);
                    uint32_t ret = g->writer->MappedInstruction(OpIEqual, SPVWriter::Section::LocalFunction, returnType, loadedArg0, loadedArg1);
                    return SPIRVResult(ret, returnType, true, true);
                };
            }
            else if (fun->name == "operator!=")
            {
                generator->generatorIntrinsics[fun] = [](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
                {
                    SPIRVResult loadedArg0 = LoadValueSPIRV(c, g, args[0]);
                    SPIRVResult loadedArg1 = LoadValueSPIRV(c, g, args[1]);
                    uint32_t ret = g->writer->MappedInstruction(OpINotEqual, SPVWriter::Section::LocalFunction, returnType, loadedArg0, loadedArg1);
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
GenerateVariableSPIRV(const Compiler* compiler, SPIRVGenerator* generator, Symbol* symbol, bool isGlobal)
{
    Variable* var = static_cast<Variable*>(symbol);
    Variable::__Resolved* varResolved = static_cast<Variable::__Resolved*>(var->resolved);

    Function::__Resolved* entryRes = Symbol::Resolved(generator->entryPoint);
    if (entryRes->visibleSymbols.Find(var) == entryRes->visibleSymbols.end())
        return SPIRVResult::Invalid();

    SPIRVResult::Storage storage = ResolveSPIRVVariableStorage(var->type, varResolved->typeSymbol, varResolved->storage, varResolved->usageBits);
    bool logicallyAddressed = storage == SPIRVResult::Storage::UniformConstant
        || storage == SPIRVResult::Storage::Image
        || storage == SPIRVResult::Storage::Sampler
        || storage == SPIRVResult::Storage::MutableImage
        || storage == SPIRVResult::Storage::StorageBuffer && varResolved->typeSymbol->category == Type::Category::StructureCategory
        || storage == SPIRVResult::Storage::Uniform && (varResolved->typeSymbol->category == Type::Category::StructureCategory || varResolved->typeSymbol->category == Type::Category::AccelerationStructureCategory)
        || storage == SPIRVResult::Storage::PushConstant;

    bool physicalStorage = storage == SPIRVResult::Storage::PhysicalBuffer;
    
    generator->typeState.storage = storage;
    if (logicallyAddressed)
    {
        generator->typeState.layout = SPIRVGenerator::TypeState::TypeLayout::Interface;
    }

    if (physicalStorage)
    {
        generator->typeState.layout = SPIRVGenerator::TypeState::TypeLayout::Explicit;
    }

    SPIRVResult typeName = GenerateTypeSPIRV(compiler, generator, var->type, varResolved->typeSymbol);

    generator->typeState.layout = SPIRVGenerator::TypeState::TypeLayout::Relaxed;
    generator->typeState.storage = SPIRVResult::Storage::Function;
    //std::string type = varResolved->type.name;
    ConstantString scope = SPIRVResult::ScopeToString(typeName.scope);

    std::vector<SPIRVResult::Indirection> parentIndirections = typeName.indirections;
    uint32_t name = INVALID_ARG;

    SPIRVResult initializer = SPIRVResult::Invalid();
    Expression* initializerExpression = var->valueExpression;

    if (varResolved->traceRayFunction != nullptr)
    {
        generator->generatorIntrinsics[varResolved->traceRayFunction] = [fun = varResolved->traceRayFunction](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
        {
            assert(args.size() == 11);
            assert(!args[0].isValue);
            g->writer->Capability(Capabilities::RayTracingKHR);
            SPIRVResult bvh = LoadValueSPIRV(c, g, args[0]);
            SPIRVResult flags = LoadValueSPIRV(c, g, args[1]);
            SPIRVResult mask = LoadValueSPIRV(c, g, args[2]);
            SPIRVResult shaderTableOffset = LoadValueSPIRV(c, g, args[3]);
            SPIRVResult shaderTableStride = LoadValueSPIRV(c, g, args[4]);
            SPIRVResult missShaderIndex = LoadValueSPIRV(c, g, args[5]);
            SPIRVResult rayOrigin = LoadValueSPIRV(c, g, args[6]);
            SPIRVResult rayDirection = LoadValueSPIRV(c, g, args[7]);
            SPIRVResult rayTMin = LoadValueSPIRV(c, g, args[8]);
            SPIRVResult rayTMax = LoadValueSPIRV(c, g, args[9]);
            g->writer->Instruction(OpTraceRayKHR, SPVWriter::Section::LocalFunction, bvh, flags, mask, shaderTableOffset, shaderTableStride, missShaderIndex, rayOrigin, rayTMin, rayDirection, rayTMax, args[10]);
            return SPIRVResult::Invalid();
        };
    }

    // override initializer if the variable is a gplIs variable
    if (varResolved->builtin && var->name.StartsWith("gplIs"))
    {
        initializerExpression = &generator->shaderValueExpressions[generator->shaderStage];
    }
    else if (initializerExpression != nullptr)
    {
        // Check if the program overrides this variable
        if (varResolved->usageBits.flags.isConst)
        {
            const auto elem = generator->evaluatingProgram->constVarInitializationOverrides.Find(var);
            if (elem != generator->evaluatingProgram->constVarInitializationOverrides.end())
            {
                initializerExpression = elem->second;
            }
        }

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
                initializer = it(compiler, generator, typeName.typeName, { initializer });
            }
            else
                initializer = it->second(compiler, generator, typeName.typeName, { initializer });
        }

        // TODO: If physical storage, convert the uint to a pointer type
        if (physicalStorage)
        {

        }

        generator->linkDefineEvaluation = false;
        generator->linkDefineSlot = UINT32_MAX;
    }

    uint32_t debugName;
    if (varResolved->storage != Storage::LinkDefined)
    {
        uint32_t typePtrName = typeName.typeName;
        
        // If anything but void, then the type has to be a pointer
        if (!logicallyAddressed)
        {
            TStr ptrType = TStr("ptr_", ToSPIRVTypeString(compiler, generator, var->type, varResolved->typeSymbol));
            auto prevScope = typeName.scope;

            if (typeName.scope == SPIRVResult::Storage::StorageBuffer
                || typeName.scope == SPIRVResult::Storage::Uniform)
            {
                typeName.scope = SPIRVResult::Storage::Function;
                storage = SPIRVResult::Storage::Function;
            }
            if (physicalStorage && varResolved->typeSymbol->category == Type::Category::StructureCategory)
            {
                ptrType = TStr(ptrType, "_Explicit");
            }
            typePtrName = AddType(generator, TStr::Compact(ptrType, "_", scope), OpTypePointer, ScopeToEnum(typeName.scope), SPVArg{ typeName.typeName });
            typeName.AddIndirection({ SPIRVResult::Pointer(typePtrName, typeName.typeName, prevScope) });
            
            typeName.typeName = typePtrName;
        }

        SPVWriter::Section declarationScope = isGlobal ? SPVWriter::Section::Declarations : SPVWriter::Section::VariableDeclarations;
        
        if (initializer != SPIRVResult::Invalid())
        {
            declarationScope = initializer.isConst && isGlobal ? SPVWriter::Section::Declarations : declarationScope;
            // Only create the variable with the initializer if it's const and the variable is global
            if (initializer.isConst && isGlobal)
            {
                name = generator->writer->MappedInstruction(OpVariable, declarationScope, typeName.typeName, ScopeToEnum(typeName.scope), SPVArg{ initializer.name }, SPVComment(var->name.c_str()));
                if (compiler->options.debugSymbols)
                    generator->writer->Instruction(OpName, SPVWriter::Section::DebugNames, SPVArg{ name }, var->name.c_str());
            }
            else
            {
                name = generator->writer->MappedInstruction(OpVariable, declarationScope, typeName.typeName, ScopeToEnum(typeName.scope), SPVComment(var->name.c_str()));
                if (compiler->options.debugSymbols)
                    generator->writer->Instruction(OpName, SPVWriter::Section::DebugNames, SPVArg{ name }, var->name.c_str());

                SPIRVResult loaded = LoadValueSPIRV(compiler, generator, initializer);
                if (loaded.typeName != typeName.indirections.front().pointerInfo.dataType)
                {
                    loaded.name = generator->writer->MappedInstruction(OpCopyLogical, SPVWriter::Section::LocalFunction, typeName.indirections.front().pointerInfo.dataType, SPVArg(loaded.name));
                }
                generator->writer->Instruction(OpStore, SPVWriter::Section::LocalFunction, SPVArg(name), loaded);
            }
        }
        else
        {
            name = generator->writer->MappedInstruction(OpVariable, declarationScope, typeName.typeName, ScopeToEnum(typeName.scope), SPVComment(var->name.c_str()));
            if (compiler->options.debugSymbols)
                generator->writer->Instruction(OpName, SPVWriter::Section::DebugNames, SPVArg{ name }, var->name.c_str());
        }
        auto ret = SPIRVResult(name, typeName.typeName, false, false, typeName.scope);

        if (storage == SPIRVResult::Storage::StorageBuffer 
            || storage == SPIRVResult::Storage::Uniform 
            || storage == SPIRVResult::Storage::UniformConstant 
            || storage == SPIRVResult::Storage::Sampler)
        {
            generator->writer->Decorate(SPVArg(name), Decorations::DescriptorSet, (uint32_t)varResolved->group);
            generator->writer->Decorate(SPVArg(name), Decorations::Binding, (uint32_t)varResolved->binding);
        }
        else if (storage == SPIRVResult::Storage::Image || storage == SPIRVResult::Storage::MutableImage)
        {
            generator->writer->Decorate(SPVArg(name), Decorations::DescriptorSet, (uint32_t)varResolved->group);
            generator->writer->Decorate(SPVArg(name), Decorations::Binding, (uint32_t)varResolved->binding);
        }
        else if (storage == SPIRVResult::Storage::Input || storage == SPIRVResult::Storage::Output)
        {
            uint8_t binding = typeName.scope == SPIRVResult::Storage::Input ? varResolved->inBinding : varResolved->outBinding;
            generator->writer->Decorate(SPVArg(name), Decorations::Location, binding);
            generator->writer->Decorate(SPVArg(name), Decorations::Component, 0);
            Variable::__Resolved::ParameterBits paramBits = varResolved->parameterBits.bits;
            auto transientBits = generator->evaluatingProgram->variablesWithTransientModifiers.Find(var);
            if (transientBits != generator->evaluatingProgram->variablesWithTransientModifiers.end())
                paramBits.bits |= transientBits->second;

            if (paramBits.flags.isNoInterpolate)
            {
                generator->writer->Decorate(SPVArg(name), Decorations::Flat);
            }
            if (paramBits.flags.isNoPerspective)
            {
                generator->writer->Decorate(SPVArg(name), Decorations::NoPerspective);
            }
            if (paramBits.flags.isPatch)
            {
                generator->writer->Decorate(SPVArg(name), Decorations::Patch);
            }
            if (paramBits.flags.isCentroid)
            {
                generator->writer->Decorate(SPVArg(name), Decorations::Centroid);
            }
            if (paramBits.flags.isSample)
            {
                generator->writer->Decorate(SPVArg(name), Decorations::Sample);
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
            || storage == SPIRVResult::Storage::RayPayload
            || storage == SPIRVResult::Storage::RayPayloadInput
            || storage == SPIRVResult::Storage::RayHitAttribute
            || storage == SPIRVResult::Storage::CallableData
            || storage == SPIRVResult::Storage::CallableDataInput
            )
            generator->interfaceVariables.Insert(name);
        
        ret.isStructPadded = typeName.isStructPadded;
        ret.derefs = typeName.derefs;
        ret.indirections = typeName.indirections;
        ret.memoryModifiers.volatileAccess = varResolved->accessBits.flags.volatileAccess;
        ret.memoryModifiers.nonTemporalAccess = varResolved->accessBits.flags.nonTemporalAccess;
        BindSymbol(generator, declarationScope, var->name.c_str(), ret);
        
        return ret;
    }
    else
    {
        // If it's a link defined variable, the constant generated by the initializer is sufficient
        generator->writer->LinkDefinedVariable(var->name, initializer);
        generator->writer->Decorate(SPVArg(initializer.name), Decorations::SpecId, varResolved->binding);

        if (compiler->options.debugSymbols)
            generator->writer->Instruction(OpName, SPVWriter::Section::DebugNames, initializer, var->name.c_str());

        auto res = SPIRVResult(initializer.name, typeName.typeName);
        res.isValue = true;
        res.isConst = true;
        res.isSpecialization = true;
        res.scope = typeName.scope;
        res.isStructPadded = typeName.isStructPadded;
        res.indirections = parentIndirections;
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
    if (sym.value.name != 0xFFFFFFFF)
    {
        funName = sym.value.name;

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
            Variable* param = resolvedCall->function->parameters[i];
            Variable::__Resolved* paramResolved = Symbol::Resolved(param);
            SPIRVResult::Storage storage = ResolveSPIRVVariableStorage(param->type, paramResolved->typeSymbol, paramResolved->storage, paramResolved->usageBits);
            generator->typeState.storage = storage;

            SPIRVResult typeName;
            if (paramResolved->storage == Storage::Output)
            {
                arg.isValue = true;
                storage = SPIRVResult::Storage::Function;
                generator->typeState.storage = storage;
                typeName = GeneratePointerTypeSPIRV(compiler, generator, param->type, paramResolved->typeSymbol);
            }
            else
            {
                typeName = GenerateTypeSPIRV(compiler, generator, param->type, paramResolved->typeSymbol);
            }

            generator->typeState.storage = SPIRVResult::Storage::Function;

            if (!resolvedCall->argumentTypes[i].IsPointer() && paramResolved->storage != Storage::Output)
                arg = LoadValueSPIRV(compiler, generator, arg);

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
            
            // If there is a storage mismatch between call and function, make a logical copy
            if (arg.isValue)
            {
                if (typeName.typeName != arg.typeName)
                {
                    arg.name = generator->writer->MappedInstruction(OpCopyLogical, SPVWriter::Section::LocalFunction, typeName.typeName, SPVArg(arg.name));
                }
            }
            else
            {
                if (typeName.indirections.back().pointerInfo.dataType != arg.indirections.back().pointerInfo.dataType)
                {
                    arg.name = generator->writer->MappedInstruction(OpCopyLogical, SPVWriter::Section::LocalFunction, typeName.typeName, SPVArg(arg.name));
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
        uint32_t ret = generator->writer->MappedInstruction(OpFunctionCall, SPVWriter::Section::LocalFunction, returnTypeName.typeName, SPVArg(funName), SPVArgList(argListArray), SPVComment(resolvedCall->functionSymbol.c_str()));
        return SPIRVResult(ret, returnTypeName.typeName, true);
    }
    else
    {
        // If there is no symbol matching this function, then we assume it's an intrinsic function
        std::vector<SPIRVResult> args;

        // Create arg list from argument expressions
        for (size_t i = 0; i < callExpression->args.size; i++)
        {
            Variable* param = resolvedCall->function->parameters.buf[i];
            Variable::__Resolved* paramResolved = Symbol::Resolved(param);

            // If an argument is a literal, evalute it directly
            if (!param->type.literal)
            {
                if (resolvedCall->argumentTypes[i].literal)
                {
                    ValueUnion val;
                    bool res = callExpression->args[i]->EvalValue(val);
                    assert(res);
                    SPIRVResult arg = SPIRVResult(val);
                    SPIRVResult ty = GenerateTypeSPIRV(compiler, generator, param->type, paramResolved->typeSymbol);

                    if (val.columnSize > 1)
                    {
                        TransientArray<SPIRVResult> literals(val.columnSize);
                        for (uint8_t i = 0; i < val.columnSize; i++)
                        {
                            literals.Append(SPIRVResult(val, i).ConvertTo(paramResolved->typeSymbol->baseType));
                        }
                        arg = GenerateCompositeSPIRV(compiler, generator, ty.typeName, literals);
                    }
                    else if (paramResolved->typeSymbol->columnSize > 1)
                    {
                        arg = arg.ConvertTo(paramResolved->typeSymbol->baseType);
                        arg = GenerateSplatCompositeSPIRV(compiler, generator, ty.typeName, paramResolved->typeSymbol->columnSize, arg);
                    }
                    else
                    {
                        arg = arg.ConvertTo(paramResolved->typeSymbol->baseType);
                    }
                    args.push_back(arg);
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
            else // Param must be literal
            {
                ValueUnion val;
                bool res = callExpression->args[i]->EvalValue(val);
                assert(res);
                SPIRVResult arg = SPIRVResult(val);
                arg = arg.ConvertTo(paramResolved->typeSymbol->baseType);
                if (paramResolved->typeSymbol->columnSize > 1)
                {
                    SPIRVResult ty = GenerateTypeSPIRV(compiler, generator, param->type, paramResolved->typeSymbol);
                    arg = GenerateSplatCompositeSPIRV(compiler, generator, ty.typeName, paramResolved->typeSymbol->columnSize, arg);
                }
                args.push_back(arg);
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
            if (resolvedCall->function->backendIndex != UINT64_MAX)
            {
                assert(resolvedCall->function->backendIndex < SPIRVDefaultIntrinsics.size());
                auto it = SPIRVDefaultIntrinsics[resolvedCall->function->backendIndex];
                return it(compiler, generator, returnTypeName.typeName, args);
            }
            else
            {
                // Generate functions here, only function available now is pointer cast
                generator->generatorIntrinsics[resolvedCall->function] = [&resolvedCall](const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args) -> SPIRVResult
                {
                    return args[0];
                    /*
                    Function::__Resolved* resolvedCallFunction = Symbol::Resolved(resolvedCall->function);
                    auto currentStorage = g->typeState.storage;
                    SPIRVGenerator::TypeState::TypeLayout oldLayout = g->typeState.layout;
                    g->typeState.storage = SPIRVResult::Storage::PhysicalBuffer;
                    g->typeState.layout = SPIRVGenerator::TypeState::TypeLayout::Explicit;
                    auto type = resolvedCall->function->returnType;
                    type.address = false;
                    SPIRVResult ptrReturnType = GenerateTypeSPIRV(c, g, type, resolvedCallFunction->returnTypeSymbol, true);
                    SPIRVResult loadedValue = LoadValueSPIRV(c, g, args[0]);
                    uint32_t ret = g->writer->MappedInstruction(OpConvertUToPtr, SPVWriter::Section::LocalFunction, SPVArg(ptrReturnType.typeName), loadedValue);

                    SPIRVResult res = ptrReturnType;
                    res.name = ret;

                    g->typeState.storage = currentStorage; 
                    g->typeState.layout = oldLayout;
                    return res;
                    */
                };

                // This means the function must be generated here
                auto it = generator->generatorIntrinsics.find(resolvedCall->function);
                assert(it != generator->generatorIntrinsics.end());
                return it->second(compiler, generator, returnTypeName.typeName, args);
            }
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

    generator->typeState.storage = leftSymbol.scope;
    if (leftSymbol.scope == SPIRVResult::Storage::StorageBuffer
        || leftSymbol.scope == SPIRVResult::Storage::UniformConstant)
        generator->typeState.layout = SPIRVGenerator::TypeState::TypeLayout::Explicit;
    SPIRVResult returnType = GeneratePointerTypeSPIRV(compiler, generator, arrayIndexExpressionResolved->returnFullType, arrayIndexExpressionResolved->returnType);
    generator->typeState.layout = SPIRVGenerator::TypeState::TypeLayout::Relaxed;
    generator->typeState.storage = SPIRVResult::Storage::Function;

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
    if (leftType.modifiers.size == 0 && !leftType.address)
    {
        Type* type = static_cast<Type*>(compiler->GetSymbol(leftType.name));

        if (type->IsVector() || type->IsMatrix())
        {
            auto op = TransientString("operator[](", rightType.name, ")");
            auto it = type->scope.symbolLookup.Find(HashString(op));
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
            assert(res.scope == returnType.scope);
            return res;
        }
    }
    else
    {
        if (arrayIndexExpressionResolved->isAddressIndex)
        {
            SPIRVResult ret = leftSymbol;
            ret.typeName = GeneratePODTypeSPIRV(compiler, generator, TypeCode::UInt64);

            // If the left side is an unbound array, it lives in a struct, so access chain must first get the first (0) element
            if (ret.isStructPadded)
            {
                SPIRVResult zero = GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::UInt(0));
                
                ret.AddIndirection({ SPIRVResult::Access(zero.name, ret.indirections.back().pointerInfo.ptrType, ret.indirections.back().pointerInfo.dataType) });
            }

            if (indexConstant.typeName != ret.typeName)
            {
                indexConstant.name = generator->writer->MappedInstruction(OpUConvert, SPVWriter::Section::LocalFunction, ret.typeName, indexConstant);
                indexConstant.typeName = ret.typeName;
            }
            uint32_t indexOffset = indexConstant.name;

            // If the accessed type is not void, multiply by stride
            if (arrayIndexExpressionResolved->lhsType->category != Type::Category::VoidCategory)
            {
                SPIRVResult stride = GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::UInt64(arrayIndexExpressionResolved->lhsType->byteSize));
                indexOffset = generator->writer->MappedInstruction(OpIMul, SPVWriter::Section::LocalFunction, indexConstant.typeName, stride, indexConstant);
            }

            // Change layout to be explicit first, then generate data type
            auto oldLayout = generator->typeState.layout;
            generator->typeState.layout = SPIRVGenerator::TypeState::TypeLayout::Explicit;
            SPIRVResult dataType = GenerateTypeSPIRV(compiler, generator, arrayIndexExpressionResolved->returnFullType, arrayIndexExpressionResolved->returnType);

            // Now generate pointer type, and change storage
            auto oldStorage = generator->typeState.storage;
            generator->typeState.storage = SPIRVResult::Storage::PhysicalBuffer;
            returnType = GeneratePointerTypeSPIRV(compiler, generator, arrayIndexExpressionResolved->returnFullType, arrayIndexExpressionResolved->returnType);
            
            // Finally convert back to original state
            generator->typeState.layout = oldLayout;
            generator->typeState.storage = oldStorage;

            // Add an access chain link to index at element zero
            Type::__Resolved* returnTypeResolved = Symbol::Resolved(arrayIndexExpressionResolved->returnType);
            ret.AddIndirection({ SPIRVResult::Address(returnType.typeName, dataType.typeName, indexConstant.typeName, indexOffset, returnTypeResolved->baseAlignment) });
            ret.addrefs++; // Add a fake ref to avoid crawling up the types
            ret.memoryModifiers.alignedAccess = true;
            ret.scope = SPIRVResult::Storage::PhysicalBuffer;

            return ret;
        }
        else
        {
            SPIRVResult ret = leftSymbol;
            ret.typeName = returnType.typeName;

            ret.AddIndirection({ SPIRVResult::Access(indexConstant.name, returnType.indirections.back().pointerInfo.ptrType, returnType.indirections.back().pointerInfo.dataType) });
            return ret;
        }
    }
    return SPIRVResult::Invalid();
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateInitializerExpressionSPIRV(const Compiler* compiler, SPIRVGenerator* generator, Expression* expr)
{
    uint32_t name = INVALID_ARG;
    InitializerExpression* initExpression = static_cast<InitializerExpression*>(expr);
    auto initResolved = Symbol::Resolved(initExpression);
    SPIRVResult strucType = GetSymbol(generator, initResolved->type->name).value;

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
            name = AddSymbol(generator, TStr::Compact("{", initializer, "}"), SPVWriter::Section::Declarations, OpSpecConstantComposite, strucType.typeName, SPVArgList(argList));
        }
        else
        {
            name = AddSymbol(generator, TStr::Compact("{", initializer, "}"), SPVWriter::Section::Declarations, OpConstantComposite, strucType.typeName, SPVArgList(argList));
        }
    }
    else
    {
        assert(!generator->linkDefineEvaluation);
        name = AddSymbol(generator, TStr::Compact("{", initializer, "}"), SPVWriter::Section::LocalFunction, OpCompositeConstruct, strucType.typeName, SPVArgList(argList));
    }
    return SPIRVResult(name, strucType.typeName, true, isConst);
}

//------------------------------------------------------------------------------
/**
*/
SPIRVResult
GenerateArrayInitializerExpressionSPIRV(const Compiler* compiler, SPIRVGenerator* generator, Expression* expr)
{
    auto sym = static_cast<ArrayInitializerExpression*>(expr);
    ArrayInitializerExpression::__Resolved* arrayInitializerExpressionResolved = Symbol::Resolved(sym);

    SPIRVResult::Storage storage = ResolveSPIRVVariableStorage(arrayInitializerExpressionResolved->fullType, arrayInitializerExpressionResolved->type, arrayInitializerExpressionResolved->storage);

    generator->typeState.storage = storage;
    SPIRVResult type = GenerateTypeSPIRV(compiler, generator, arrayInitializerExpressionResolved->fullType, arrayInitializerExpressionResolved->type);
    generator->typeState.storage = SPIRVResult::Storage::Function;

    uint32_t name = INVALID_ARG;
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

    if (binaryExpression->op != '=')
    {
        // If the left operator is void, assume address operation and use UInt32 as operator type
        Type* operatorType = lhsType;
        if (operatorType->category == Type::Category::VoidCategory)
        {
            assert(binaryExpressionResolved->isAddressOperation);
            operatorType = &UInt32Type;
        }
        std::string functionName = Format("operator%s(%s)", FourCCToString(binaryExpression->op).c_str(), rightType.Name().c_str());
        Function* fun = operatorType->GetSymbol<Function>(functionName);
        Function::__Resolved* funResolved = Symbol::Resolved(fun);
        assert(fun != nullptr);

        SPIRVResult retType = GenerateTypeSPIRV(compiler, generator, fun->returnType, funResolved->returnTypeSymbol);

        // Get operator 
        if (!binaryExpressionResolved->isAddressOperation)
        {
            auto it = generator->generatorIntrinsics.find(fun);
            if (it == generator->generatorIntrinsics.end())
            {
                assert(fun->backendIndex != UINT64_MAX && fun->backendIndex < SPIRVDefaultIntrinsics.size());
                auto it = SPIRVDefaultIntrinsics[fun->backendIndex];
                rightValue = it(compiler, generator, retType.typeName, { leftValue, rightValue });
            }
            else
                rightValue = it->second(compiler, generator, retType.typeName, { leftValue, rightValue });
        }
        else
        {
            // Scale with size of type first
            uint32_t scaledOffset = rightValue.name;
            uint32_t stride = 0;

            // If either side is void, then the stride is 1 byte
            if (lhsType->category == Type::Category::VoidCategory || rhsType->category == Type::Category::VoidCategory)
            {
                stride = 1;
            }
            else
            {
                // Otherwise, the stride is the side of the address type
                if (leftType.address)
                    stride = binaryExpressionResolved->lhsType->byteSize;
                else
                    stride = binaryExpressionResolved->rhsType->byteSize;
            }

            SPIRVResult offsetValue = GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::UInt64(stride));

            if (rightValue.typeName != offsetValue.typeName)
            {
                rightValue.name = generator->writer->MappedInstruction(OpUConvert, SPVWriter::Section::LocalFunction, offsetValue.typeName, rightValue);
                rightValue.typeName = offsetValue.typeName;
            }

            // If rhs is void, then just add the integers together
            if (rhsType->category != Type::Category::VoidCategory)
                scaledOffset = generator->writer->MappedInstruction(OpIMul, SPVWriter::Section::LocalFunction, rightValue.typeName, rightValue, offsetValue);
            uint32_t offsetPtr = 0x0;
            SPIRVResult leftLoaded = LoadValueSPIRV(compiler, generator, leftValue);

            if (binaryExpression->op == '+')
                offsetPtr = generator->writer->MappedInstruction(OpIAdd, SPVWriter::Section::LocalFunction, rightValue.typeName, leftLoaded, SPVArg(scaledOffset));
            else if (binaryExpression->op == '-')
                offsetPtr = generator->writer->MappedInstruction(OpISub, SPVWriter::Section::LocalFunction, rightValue.typeName, leftLoaded, SPVArg(scaledOffset));
            else
                assert(false); // Unsupported pointer operation
            rightValue.name = offsetPtr;
        }
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
            SPIRVResult leftLoaded = LoadValueSPIRV(compiler, generator, leftValue);

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
            rightValue = rightValue;
        }
        StoreValueSPIRV(compiler, generator, leftValue, rightValue);
        return leftValue;
    }
    else
    {
        return rightValue;
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
                        auto oldStorage = generator->typeState.storage;
                        SPIRVResult::Storage storage = lhs.scope;
                        if (accessExpressionResolved->isAddressDeref)
                            storage = SPIRVResult::Storage::PhysicalBuffer;
                        generator->typeState.storage = storage;
                        SPIRVResult typeName = GeneratePointerTypeSPIRV(compiler, generator, mem->type, memResolved->typeSymbol);

                        if (accessExpressionResolved->isAddressDeref)
                        {
                            Type::FullType temp = accessExpressionResolved->leftType;
                            temp.address = false;
                            auto oldLayout = generator->typeState.layout;
                            generator->typeState.layout = SPIRVGenerator::TypeState::TypeLayout::Explicit;
                            SPIRVResult leftType = GeneratePointerTypeSPIRV(compiler, generator, temp, accessExpressionResolved->lhsType);
                            lhs.AddIndirection({ SPIRVResult::Address(leftType.indirections.back().pointerInfo.ptrType, leftType.indirections.back().pointerInfo.dataType, Symbol::Resolved(memResolved->typeSymbol)->baseAlignment) });
                            generator->typeState.layout = oldLayout;
                        }
                        
                        // Add access
                        lhs.AddIndirection({ SPIRVResult::Access(indexName.name, typeName.indirections[0].pointerInfo.ptrType, typeName.indirections[0].pointerInfo.dataType) });

                        generator->typeState.storage = oldStorage;
                        
                        // Push a temporary value without any access chains
                        SPIRVResult temp = lhs;
                        //temp.indirections = {};
                        generator->PushAccessChain(temp);
                        SPIRVResult rhs = GenerateExpressionSPIRV(compiler, generator, accessExpression->right);
                        generator->PopAccessChain();

                        // Merge the result of the rhs with the lhs
                        lhs.typeName = rhs.typeName;
                        lhs.indirections = rhs.indirections;

                        if (accessExpression->tailDeref)
                        {
                            SPIRVResult temp = lhs;
                            temp.indirections = { lhs.indirections.back() };
                            temp = LoadValueSPIRV(compiler, generator, temp);
                            temp.indirections = rhs.indirections;
                            temp.indirections.pop_back();
                            temp.isValue = temp.indirections.size() == 0;
                            lhs = temp;
                        }
                        if (accessExpression->tailRef)
                        {
                            lhs.indirections.erase(lhs.indirections.begin());
                            lhs.isValue = rhs.indirections.size() == 0;
                        }
                        return lhs;
                    }
                    else if (lhsSymbol->symbolType == Symbol::EnumerationType)
                    {
                        EnumExpression* enumExp = static_cast<EnumExpression*>(sym);
                        uint32_t val;
                        ValueUnion value;
                        bool res = enumExp->EvalValue(value);
                        assert(res);
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
    Function::__Resolved* entryRes = Symbol::Resolved(generator->entryPoint);

    SPIRVResult lhsResult = GenerateExpressionSPIRV(compiler, generator, ternaryExpression->lhs);

    // First check for literal code gen
    if (lhsResult.isLiteral)
    {
        if (lhsResult.literalValue.b)
            return GenerateExpressionSPIRV(compiler, generator, ternaryExpression->ifExpression);
        else
            return GenerateExpressionSPIRV(compiler, generator, ternaryExpression->elseExpression);
    }

    SPIRVResult type = GenerateTypeSPIRV(compiler, generator, ternaryExpressionResolved->fullType, ternaryExpressionResolved->type);

    SPIRVResult ifResult = GenerateExpressionSPIRV(compiler, generator, ternaryExpression->ifExpression);
    SPIRVResult elseResult = GenerateExpressionSPIRV(compiler, generator, ternaryExpression->elseExpression);

    if (ternaryExpressionResolved->ifValueConversion)
    {
        auto it = generator->generatorIntrinsics.find(ternaryExpressionResolved->ifValueConversion);
        if (it == generator->generatorIntrinsics.end())
        {
            assert(ternaryExpressionResolved->ifValueConversion->backendIndex != UINT64_MAX && ternaryExpressionResolved->ifValueConversion->backendIndex < SPIRVDefaultIntrinsics.size());
            auto it = SPIRVDefaultIntrinsics[ternaryExpressionResolved->ifValueConversion->backendIndex];
            ifResult = it(compiler, generator, type.typeName, { ifResult });
        }
        else
            ifResult = it->second(compiler, generator, type.typeName, { ifResult });
    }
    if (ternaryExpressionResolved->elseValueConversion)
    {
        auto it = generator->generatorIntrinsics.find(ternaryExpressionResolved->elseValueConversion);
        if (it == generator->generatorIntrinsics.end())
        {
            assert(ternaryExpressionResolved->elseValueConversion->backendIndex != UINT64_MAX && ternaryExpressionResolved->elseValueConversion->backendIndex < SPIRVDefaultIntrinsics.size());
            auto it = SPIRVDefaultIntrinsics[ternaryExpressionResolved->elseValueConversion->backendIndex];
            elseResult = it(compiler, generator, type.typeName, { elseResult });
        }
        else
            elseResult = it->second(compiler, generator, type.typeName, { elseResult });
    }
    
    if (compiler->options.debugSymbols)
    {
        uint32_t name = generator->writer->String(expr->location.file.c_str());
        generator->writer->Instruction(OpLine, SPVWriter::Section::LocalFunction, SPVArg{name}, expr->location.line, expr->location.start);
    }
    SPIRVResult loadedCondition = LoadValueSPIRV(compiler, generator, lhsResult);
    SPIRVResult loadedLeft = LoadValueSPIRV(compiler, generator, ifResult);
    SPIRVResult loadedRight = LoadValueSPIRV(compiler, generator, elseResult);
    uint32_t ret = generator->writer->MappedInstruction(OpSelect, SPVWriter::Section::LocalFunction, type.typeName, loadedCondition, loadedLeft, loadedRight);

    // The type of left and right are identical
    SPIRVResult res = loadedLeft;
    res.name = ret;
    res.isLiteral = false;
    res.isConst = false;
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
            if (rhs.isLiteral)
            {
                switch (op)
                {
                    case 'F':
                        rhs.literalValue.f += 1.0f;
                        break;
                    case 'U':
                        rhs.literalValue.ui += 1;
                        break;
                    case 'S':
                        rhs.literalValue.i += 1;
                        break;
                    default:
                        assert(false);
                        return SPIRVResult::Invalid();
                }
                return rhs;
            }

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
            if (rhs.isLiteral)
            {
                switch (op)
                {
                    case 'F':
                        rhs.literalValue.f -= 1.0f;
                        break;
                    case 'U':
                        rhs.literalValue.ui -= 1;
                        break;
                    case 'S':
                        rhs.literalValue.i -= 1;
                        break;
                    default:
                        assert(false);
                        return SPIRVResult::Invalid();
                }
                return rhs;
            }
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
            uint32_t res = generator->writer->MappedInstruction(subOp, SPVWriter::Section::LocalFunction, loaded.typeName, loaded, constOne);
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
            // Split indirections by shaving off the topmost one
            SPIRVResult temp = rhs;
            temp.indirections = { rhs.indirections.back() };
            temp = LoadValueSPIRV(compiler, generator, temp);
            temp.indirections = rhs.indirections;
            temp.indirections.pop_back();
            temp.isValue = temp.indirections.empty();
            rhs = temp;
            rhs.derefs++;
            return rhs;
            //SPIRVResult typeName = GenerateTypeSPIRV(compiler, generator, unaryExpressionResolved->fullType, unaryExpressionResolved->type);
            //uint32_t deref = generator->AddMappedOp(Format("OpLoad %%%d %%%d", typeName.typeName, rhs.name), unaryExpressionResolved->text);
            //return SPIRVResult(deref, typeName.typeName, true);
            //break;
        }
        case '&':
        {
            // Find the last pointer indirection and erase it
            auto it = rhs.indirections.begin();
            while (it != rhs.indirections.end())
            {
                if (it->type == SPIRVResult::Indirection::Type::Pointer)
                {
                    rhs.indirections.erase(it);
                    break;
                }
                it++;
            }

            rhs.isValue = rhs.indirections.empty();
            rhs.addrefs++;
            return rhs;
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
                        rhs.literalValue.f = -rhs.literalValue.f;
                        break;
                    case 'U':
                        rhs.literalValue.ui = -rhs.literalValue.ui;
                        break;
                    case 'S':
                        rhs.literalValue.i = -rhs.literalValue.i;
                        break;
                    default:
                        assert(false);
                        return SPIRVResult::Invalid();
                }
                return rhs;
            }
            else
            {
                SPIRVResult loaded = LoadValueSPIRV(compiler, generator, rhs);
                uint32_t res = generator->writer->MappedInstruction(negOp, SPVWriter::Section::LocalFunction, loaded.typeName, loaded);
                return SPIRVResult(res, loaded.typeName, true);
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
                if (op == 'B')
                    rhs.literalValue.b8 = !rhs.literalValue.b8;
                else
                    rhs.literalValue.ui = ~rhs.literalValue.ui;
                return rhs;
            }
            else
            {
                SPIRVResult loaded = LoadValueSPIRV(compiler, generator, rhs);
                if (op == 'B')
                {
                    loaded.name = generator->writer->MappedInstruction(OpLogicalNot, SPVWriter::Section::LocalFunction, loaded.typeName, loaded);
                }
                else
                {
                    loaded.name = generator->writer->MappedInstruction(OpNot, SPVWriter::Section::LocalFunction, loaded.typeName, loaded);
                }
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

    // If we can evaluate the expression immediately, don't bother traversing the expression itself
    ValueUnion val;
    bool valueEvaled = expr->EvalValue(val);

    switch (expr->symbolType)
    {
        case Symbol::FloatExpressionType:
        {
            FloatExpression* floatExpr = static_cast<FloatExpression*>(expr);
            assert(valueEvaled);
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
            TransientArray<SPIRVResult> results(floatVecExpr->values.size);
            for (uint32_t i = 0; i < floatVecExpr->values.size; i++)
            {
                results.Append(GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::Float(floatVecExpr->values[i])));
            }
            SPIRVResult ty = GenerateTypeSPIRV(compiler, generator, floatVecExprRes->fullType, floatVecExprRes->type);
            return GenerateCompositeSPIRV(compiler, generator, ty.typeName, results);
        }
        case Symbol::IntExpressionType:
        {
            IntExpression* intExpr = static_cast<IntExpression*>(expr);
            assert(valueEvaled);
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
            TransientArray<SPIRVResult> results(intVecExpr->values.size);
            for (uint32_t i = 0; i < intVecExpr->values.size; i++)
            {
                results.Append(GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::Int(intVecExpr->values[i])));
            }
            SPIRVResult ty = GenerateTypeSPIRV(compiler, generator, intVecExprRes->fullType, intVecExprRes->type);
            return GenerateCompositeSPIRV(compiler, generator, ty.typeName, results);
        }
        case Symbol::UIntExpressionType:
        {
            UIntExpression* uintExpr = static_cast<UIntExpression*>(expr);
            assert(valueEvaled);
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
            TransientArray<SPIRVResult> results(uintVecExpr->values.size);
            for (uint32_t i = 0; i < uintVecExpr->values.size; i++)
            {
                results.Append(GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::UInt(uintVecExpr->values[i])));
            }
            SPIRVResult ty = GenerateTypeSPIRV(compiler, generator, uintVecExprRes->fullType, uintVecExprRes->type);
            return GenerateCompositeSPIRV(compiler, generator, ty.typeName, results);
        }
        case Symbol::BoolExpressionType:
        {
            BoolExpression* boolExpr = static_cast<BoolExpression*>(expr);
            assert(valueEvaled);
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
            TransientArray<SPIRVResult> results(boolVecExpr->values.size);
            for (uint32_t i = 0; i < boolVecExpr->values.size; i++)
            {
                results.Append(GenerateConstantSPIRV(compiler, generator, ConstantCreationInfo::Bool(boolVecExpr->values[i])));
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

            if (symbolExpression->symbol.StartsWith("gpl"))
            {
                // Special case for variables that start with GPL and might be per program
                const SymbolAssignment& assign = GetSymbol(generator, symbolExpression->symbol);
                return SPIRVResult(assign.value);
            }
            else
            {
                if (symResolved->symbol->symbolType == Symbol::SymbolType::VariableType)
                {
                    Variable* var = static_cast<Variable*>(symResolved->symbol);
                    Variable::__Resolved* varResolved = Symbol::Resolved(var);
                    SPIRVResult res = SPIRVResult::Invalid();
                    if (!varResolved->builtin)
                    {
                        SPIRVResult::Storage storage = ResolveSPIRVVariableStorage(var->type, varResolved->typeSymbol, varResolved->storage, varResolved->usageBits);
                        const SymbolAssignment& sym = GetSymbol(generator, symbolExpression->symbol);
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
                                        res = SPIRVResult(INVALID_ARG, type.typeName);
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
                                res = sym.value;
                            }
                        }
                        else
                        {
                            return generator->accessChain.back();
                        }
                    }
                    else
                    {
                        res = GenerateVariableSPIRV(compiler, generator, var, true);
                    }

                    return res;
                }
                else if (symResolved->symbol->symbolType == Symbol::SymbolType::EnumerationType)
                {
                    Enumeration* enu = static_cast<Enumeration*>(symResolved->symbol);
                    Enumeration::__Resolved* enuResolved = Symbol::Resolved(enu);
                    type = GenerateTypeSPIRV(compiler, generator, enu->type, enuResolved->typeSymbol);
                    auto res = SPIRVResult(INVALID_ARG, type.typeName, false, false, type.scope);
                    res.indirections = type.indirections;
                    return res;
                }
                else
                {
                    SPIRVResult::Storage storage = ResolveSPIRVVariableStorage(symResolved->fullType, symResolved->type, symResolved->storage);
                    generator->typeState.storage = storage;
                    type = GenerateTypeSPIRV(compiler, generator, symResolved->fullType, symResolved->type);
                    generator->typeState.storage = SPIRVResult::Storage::Function;
                    SPIRVResult ret = GetSymbol(generator, symbolExpression->symbol).value;
                    ret.typeName = type.typeName;
                    ret.scope = type.scope;
                    ret.indirections = type.indirections;
                    return ret;
                }
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
    assert(generator->mergeBlocks[generator->mergeBlockCounter-1].breakLabel != INVALID_ARG);
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
    assert(generator->mergeBlocks[generator->mergeBlockCounter - 1].continueLabel != INVALID_ARG);
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
    generator->writer->PushScope();
    for (auto decl : stat->declarations)
        GenerateVariableSPIRV(compiler, generator, decl, false);
    
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
		INVALID_ARG, INVALID_ARG
    };

    generator->writer->PopScope();
}

//------------------------------------------------------------------------------
/**
*/
void
GenerateIfStatementSPIRV(const Compiler* compiler, SPIRVGenerator* generator, IfStatement* stat)
{
    Function::__Resolved* entryRes = Symbol::Resolved(generator->entryPoint);

    SPIRVResult lhsResult = GenerateExpressionSPIRV(compiler, generator, stat->condition);

    // First check if branching is static, but dio
    if (lhsResult.isLiteral)
    {
        // We don't care about the value of val, all we care about is that only one branch will be visible
        if (lhsResult.literalValue.b)
            GenerateStatementSPIRV(compiler, generator, stat->ifStatement);
        else if (stat->elseStatement)
             GenerateStatementSPIRV(compiler, generator, stat->elseStatement);
        return;
    }

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
    SPIRVResult lhsResult = GenerateExpressionSPIRV(compiler, generator, stat->switchExpression);

    // Check if switch is static
    if (lhsResult.isLiteral)
    {
        Function::__Resolved* entryRes = Symbol::Resolved(generator->entryPoint);

        generator->skipBreakContinue = true;

        // We don't care about the value of val, all we care about is that only one branch will be visible
        if (lhsResult.literalValue.u32 < stat->caseExpressions.size)
            GenerateStatementSPIRV(compiler, generator, stat->caseStatements[lhsResult.literalValue.u32]);
        else if (stat->defaultStatement != nullptr)
            GenerateStatementSPIRV(compiler, generator, stat->defaultStatement);

        generator->skipBreakContinue = false;
        return;
    }
    
    lhsResult = LoadValueSPIRV(compiler, generator, lhsResult);

    uint32_t defaultCase = generator->writer->Reserve();
    uint32_t mergeLabel = generator->writer->Reserve();

    generator->mergeBlocks[generator->mergeBlockCounter++] =
    SPIRVGenerator::MergeBlock
    {
		INVALID_ARG, mergeLabel
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
    generator->writer->Instruction(OpSwitch, SPVWriter::Section::LocalFunction, lhsResult, SPVArg{defaultCase}, SPVCaseList(caseArgs, branchArgs));

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
		INVALID_ARG, INVALID_ARG
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
		INVALID_ARG, INVALID_ARG
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
                    GenerateVariableSPIRV(compiler, generator, symbol, false);
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

    static std::unordered_map<uint32_t, std::string> partitionMap =
    {
        { PartitionIntegerSteps_value, "SpacingEqual" }
        , { PartitionFractionalEven_value, "SpacingFractionalEven" }
        , { PartitionFractionalOdd_value, "SpacingFractionalOdd" }
    };
    
    static std::unordered_map<uint32_t, SPVEnum> partitionEnumMap =
    {
        { PartitionIntegerSteps_value, ExecutionModes::SpacingEqual }
        , { PartitionFractionalEven_value, ExecutionModes::SpacingFractionalEven }
        , { PartitionFractionalOdd_value, ExecutionModes::SpacingFractionalOdd }
    };

    static std::unordered_map<uint32_t, std::string> windingOrderMap =
    {
        { WindingClockwise_value, "VertexOrderCw" }
        , { WindingCounterClockwise_value, "VertexOrderCcw" }
    };
    
    static std::unordered_map<uint32_t, SPVEnum> windingOrderEnumMap =
    {
        { WindingClockwise_value, ExecutionModes::VertexOrderCw }
        , { WindingCounterClockwise_value, ExecutionModes::VertexOrderCcw }
    };

    static std::unordered_map<uint32_t, std::string> pixelOriginMap =
    {
        { 0, "OriginUpperLeft" }
        , { PixelOriginCenter_value, "PixelCenterInteger" }
        , { PixelOriginUpperLeft_value, "OriginUpperLeft" }
        , { PixelOriginLowerLeft_value, "OriginLowerLeft" }
    };
    
    static std::unordered_map<uint32_t, SPVEnum> pixelOriginEnumMap =
    {
        { 0, ExecutionModes::OriginUpperLeft }
        , { PixelOriginCenter_value, ExecutionModes::PixelCenterInteger }
        , { PixelOriginUpperLeft_value, ExecutionModes::OriginUpperLeft }
        , { PixelOriginLowerLeft_value, ExecutionModes::OriginLowerLeft }
    };

    static std::unordered_map<uint32_t, std::string> inputPrimitiveTopologyMap =
    {
        { InputTopologyPoints_value, "InputPoints" }
        , { InputTopologyLines_value, "InputLines" }
        , { InputTopologyLinesAdjacency_value, "InputLinesAdjacency" }
        , { InputTopologyTriangles_value, "Triangles" }
        , { InputTopologyTrianglesAdjacency_value, "InputTrianglesAdjacency" }
    };

    static std::unordered_map<uint32_t, SPVEnum> inputPrimitiveTopologyEnumMap =
    {
        { InputTopologyPoints_value, ExecutionModes::InputPoints }
        , { InputTopologyLines_value, ExecutionModes::InputLines }
        , { InputTopologyLinesAdjacency_value, ExecutionModes::InputLinesAdjacency }
        , { InputTopologyTriangles_value, ExecutionModes::Triangles }
        , { InputTopologyTrianglesAdjacency_value, ExecutionModes::InputTrianglesAdjacency }
};

    static std::unordered_map<uint32_t, std::string> outputPrimitiveTopologyMap =
    {
        { OutputTopologyPoints_value, "OutputPoints" }
        , { OutputTopologyLines_value, "OutputLineStrip" }
        , { OutputTopologyTriangles_value, "OutputTriangleStrip" }
    };

    static std::unordered_map<uint32_t, SPVEnum> outputPrimitiveTopologyEnumMap =
    {
        { OutputTopologyPoints_value, ExecutionModes::OutputPoints }
        , { OutputTopologyLines_value, ExecutionModes::OutputLineStrip }
        , { OutputTopologyTriangles_value, ExecutionModes::OutputTriangleStrip }
    };

    static std::unordered_map<uint32_t, std::string> tessellationDomainMap =
    {
        { PatchIsolines_value, "Isolines" }
        , { PatchTriangles_value, "Triangles" }
        , { PatchQuads_value, "Quads" }
    };

    static std::unordered_map<uint32_t, SPVEnum> tessellationDomainEnumMap =
    {
        { PatchIsolines_value, ExecutionModes::Isolines }
        , { PatchTriangles_value, ExecutionModes::Triangles }
        , { PatchQuads_value, ExecutionModes::Quads }
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

        uint32_t boolType = GeneratePODTypeSPIRV(compiler, this, GPULang::TypeCode::Bool8);
        for (uint32_t switchMapping = ProgramInstance::__Resolved::EntryType::FirstShader; switchMapping < ProgramInstance::__Resolved::EntryType::LastShader; switchMapping++)
        {
            TransientString str = TransientString("gplIs", ProgramInstance::__Resolved::EntryTypeToString(ProgramInstance::__Resolved::EntryType(switchMapping)));
            this->writer->scopeStack.back().symbols[str.c_str()] = SymbolAssignment{ .sym = nullptr, .value = SPIRVResult(this->shaderValueExpressions[switchMapping].value) };
        }

        this->writer->Capability(extensionEnumMap[(ProgramInstance::__Resolved::EntryType)mapping]);
        this->writer->Extension(SPV_KHR_storage_buffer_storage_class);
        this->writer->Extension(SPV_KHR_variable_pointers);
        this->writer->Capability(Capabilities::VariablePointers);

        if (compiler->target.supportsPhysicalAddressing)
        {
            this->writer->Capability(Capabilities::Addresses);
            this->writer->Instruction(OpMemoryModel, SPVWriter::Section::Header, AddressingModels::Physical64, MemoryModels::GLSL450);
        }
        else
        {
            this->writer->Extension(SPV_KHR_physical_storage_buffer);
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
                            GenerateVariableSPIRV(compiler, this, genSym, true);
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
                GenerateVariableSPIRV(compiler, this, sym, true);
                break;
            }
        }

        Function::__Resolved* funResolved = Symbol::Resolved(static_cast<Function*>(object));
        SPIRVResult entryFunction = GPULang::GetSymbol(this, funResolved->name).value;

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

        this->writer->Instruction(OpEntryPoint, SPVWriter::Section::Header, executionModelEnumMap[(ProgramInstance::__Resolved::EntryType)mapping], entryFunction, "main", SPVArgList(interfaceVars));

        switch (mapping)
        {
        case ProgramInstance::__Resolved::VertexShader:
            if (funResolved->executionModifiers.layerOrViewportOutput)
            {
                this->writer->Extension(SPV_EXT_shader_viewport_index_layer);
            }
            break;
        case ProgramInstance::__Resolved::GeometryShader:
            this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, entryFunction, ExecutionModes::Invocations, funResolved->executionModifiers.invocations);
            this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, entryFunction, ExecutionModes::OutputVertices, funResolved->executionModifiers.maxOutputVertices);
            this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, entryFunction, inputPrimitiveTopologyEnumMap[funResolved->executionModifiers.inputPrimitiveTopology]);
            this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, entryFunction, outputPrimitiveTopologyEnumMap[funResolved->executionModifiers.outputPrimitiveTopology]);
            if (funResolved->executionModifiers.layerOrViewportOutput)
            {
                this->writer->Extension(SPV_EXT_shader_viewport_index_layer);
            }
            break;
        case ProgramInstance::__Resolved::HullShader:

            this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, entryFunction, ExecutionModes::OutputVertices, funResolved->executionModifiers.patchSize);
            break;
        case ProgramInstance::__Resolved::DomainShader:
            if (funResolved->executionModifiers.layerOrViewportOutput)
            {
                this->writer->Extension(SPV_EXT_shader_viewport_index_layer);
            }
            this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, entryFunction, partitionEnumMap[funResolved->executionModifiers.partitionMethod]);
            this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, entryFunction, windingOrderEnumMap[funResolved->executionModifiers.windingOrder]);
            this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, entryFunction, tessellationDomainEnumMap[funResolved->executionModifiers.patchType]);
            break;
        case ProgramInstance::__Resolved::PixelShader:
        {
            this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, entryFunction, pixelOriginEnumMap[funResolved->executionModifiers.pixelOrigin]);
            if (funResolved->executionModifiers.earlyDepth)
            {
                this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, entryFunction, ExecutionModes::EarlyFragmentTests);
            }
            if (funResolved->executionModifiers.depthAlwaysGreater)
            {
                this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, entryFunction, ExecutionModes::DepthGreater);
            }
            if (funResolved->executionModifiers.depthAlwaysLesser)
            {
                this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, entryFunction, ExecutionModes::DepthLess);
            }
            RenderStateInstance* state = static_cast<RenderStateInstance*>(progResolved->mappings[ProgramInstance::__Resolved::RenderState]);
            RenderStateInstance::__Resolved* stateRes = Symbol::Resolved(state);
            if (!stateRes->depthWriteEnabled)
            {
                // Set depth to be unchanged if depth is never written
                this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, entryFunction, ExecutionModes::DepthUnchanged);
            }
            if (progResolved->effects.flags.explicitDepth)
            {
                this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, entryFunction, ExecutionModes::DepthReplacing);
            }
            break;
        }
        case ProgramInstance::__Resolved::ComputeShader:
            this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, entryFunction, ExecutionModes::LocalSize, funResolved->executionModifiers.computeShaderWorkGroupSize[0], funResolved->executionModifiers.computeShaderWorkGroupSize[1], funResolved->executionModifiers.computeShaderWorkGroupSize[2]);
            if (funResolved->executionModifiers.groupSize != 64)
            {
                this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, entryFunction, ExecutionModes::SubgroupSize, funResolved->executionModifiers.groupSize);
            }
            if (funResolved->executionModifiers.groupsPerWorkgroup != 1)
            {
                this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, entryFunction, ExecutionModes::SubgroupsPerWorkgroup, funResolved->executionModifiers.groupsPerWorkgroup);
            }
            if (funResolved->executionModifiers.computeDerivativeIndexing == ComputeDerivativesIndexLinear_value)
            {
                this->writer->Extension(SPV_KHR_compute_shader_derivatives);
                this->writer->Capability(Capabilities::ComputeDerivativeGroupLinearKHR);
                this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, entryFunction, ExecutionModes::DerivativeGroupLinearKHR);
            }
            else if (funResolved->executionModifiers.computeDerivativeIndexing == ComputeDerivativesIndexQuad_value)
            {
                this->writer->Extension(SPV_KHR_compute_shader_derivatives);
                this->writer->Capability(Capabilities::ComputeDerivativeGroupQuadsKHR);
                this->writer->Instruction(OpExecutionMode, SPVWriter::Section::Header, entryFunction, ExecutionModes::DerivativeGroupQuadsKHR);
            }
            break;
        case ProgramInstance::__Resolved::RayGenerationShader:
        case ProgramInstance::__Resolved::RayAnyHitShader:
        case ProgramInstance::__Resolved::RayClosestHitShader:
        case ProgramInstance::__Resolved::RayIntersectionShader:
        case ProgramInstance::__Resolved::RayMissShader:
        case ProgramInstance::__Resolved::RayCallableShader:
            this->writer->Extension(SPV_KHR_ray_tracing);
            this->writer->Capability(Capabilities::RayTracingKHR);
            break;
        }
        this->shaderValueExpressions[mapping].value = false;

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

            // Use this to support out variables being pointers to function OpVariable
            spv_validator_options options = spvValidatorOptionsCreate();
            //spvValidatorOptionsSetRelaxLogicalPointer(options, true);

            res = spvValidateWithOptions(spvContext, options, &constBin, &diag);
            if (res != SPV_SUCCESS)
            {
                if (diag != nullptr)
                {
                    this->Error(Format("Internal SPIRV generation error: %s", diag->error));
                    printf("%s\n", diag->error);
                }
                
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

            if (compiler->options.debugInfo)
            {
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
                writerFunc(TStr(object->name, "_SPIRV").c_str(), std::string(binary.data));
            }
        }

        if (compiler->options.optimize)
        {
            spv_optimizer_t* optimizer = spvOptimizerCreate(SPV_ENV_VULKAN_1_2);
            spvOptimizerRegisterPerformancePasses(optimizer);

            spv_binary optimizedBinary = nullptr;
            spv_optimizer_options options = spvOptimizerOptionsCreate();
            res = spvOptimizerRun(optimizer, spvBinary.data(), spvBinary.size(), &optimizedBinary, options);
            if (res == SPV_SUCCESS)
            {
                spvBinary = std::vector<uint32_t>(optimizedBinary->code, optimizedBinary->code + optimizedBinary->wordCount);
            }
            else
            {
                assert(false && "Fuuuuck");
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
SPIRVGenerator::PushAccessChain(SPIRVResult res)
{
    this->accessChain.push_back(res);
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
SpvId* 
SPIRVGenerator::NewId()
{
    this->ids.Append(SpvId());
    SpvId* ret = &this->ids.back();
    ret->name = this->idCounter++;
    return ret;
}

} // namespace GPULang
