#pragma once
//------------------------------------------------------------------------------
/**
    Compiler backend for SPIRV code generation. This is not to be confused
    with compiling for Vulkan, this generator is meant for generating 
    pure SPIRV output from the AST.

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "generator.h"
#include "ast/symbol.h"
#include "ast/types/type.h"
#include "ast/variable.h"
#include "ast/program.h"
#include <unordered_map>
#include <set>
#include <assert.h>



namespace GPULang
{
struct SPVWriter;
struct Statement;

struct SpvId
{
    uint32_t name = 0xFFFFFFFF;
    SpvId* type = nullptr;
    bool isStructPadded = false;    // If true, the type is padded in a struct and requires an extra access indirection
    
};


struct SPIRVResult
{
    enum class Storage
    {
        Invalid,
        Private,
        Device,
        WorkGroup,
        Uniform,
        UniformConstant,
        StorageBuffer,
        PhysicalBuffer,
        Sampler,
        PushConstant,
        Function,
        Image,
        MutableImage,
        TexelPointer,                           // variable is a pointer to a texel
        Input,
        Output,
        RayPayload,                             // variable is a ray payload
        RayPayloadInput,                        // variable is a ray payload input
        RayHitAttribute,                        // variable ray tracing hit attribute (barycentrics)
        CallableData,                           // variable is ray tracing callable data
        CallableDataInput,                      // variable is ray tracing callable data
    } scope = Storage::Function;

    uint32_t name = 0xFFFFFFFF;
    uint32_t typeName = 0xFFFFFFFF;
    bool isValue = false;           // If not, then the object needs a load to be read. If it is, doesn't support store
    bool isConst = false;           // If true, value is a constant
    bool isLiteral = false;         // If true, then the value is a literal value and can be constant constructed
    bool isSpecialization = false;  // If true, then the value is the product of specialization
    bool isStructPadded = false;    // If true, the type is padded in a struct and requires an extra access indirection
    uint32_t derefs = 0;
    uint32_t addrefs = 0;
    std::vector<uint32_t> parentTypes;
    std::vector<SPIRVResult::Storage> parentScopes;

    struct MemoryModifiers
    {
        uint32_t volatileAccess : 1;
        uint32_t nonTemporalAccess : 1;
        uint32_t alignedAccess : 1;

        MemoryModifiers()
            : volatileAccess(false)
            , nonTemporalAccess(false)
            , alignedAccess(false)
        {
            
        }
    } memoryModifiers;



    Type::SwizzleMask swizzleMask;
    uint32_t swizzleType = 0xFFFFFFFF;

    struct Indirection
    {
        enum class Type
        {
            Access,          // Access chain like foo.bar.test or foo[index]
            Pointer,         // Pointer dereference
            Address,         // Address of physical buffer, requires casting from u64 to ptr
            Array,           // Access is through an array
        } type;

        union
        {
            struct AccessInfo
            {
                uint32_t ptrType, dataType, offset;
            } accessInfo;
            struct PointerInfo
            {
                uint32_t ptrType, dataType, storage;
            } pointerInfo;
            struct AddressInfo
            {
                uint32_t ptrType, dataType, indexType, offset, alignment;
            } addressInfo;
            struct ArrayInfo
            {
                uint32_t ptrType, dataType, indexType, size;
            } arrayInfo;
        };
    };
    std::vector<Indirection> indirections;
    void AddIndirection(const std::vector<Indirection>& offsets)
    {
        indirections.insert(indirections.end(), offsets.begin(), offsets.end());
    }

    void PopIndirection()
    {
        if (!indirections.empty())
            indirections.pop_back();
    }

    static Indirection Access(uint32_t link, uint32_t ptrType, uint32_t type)
    {
        return Indirection{ .type = Indirection::Type::Access, .accessInfo = { .ptrType = ptrType, .dataType = type, .offset = link } };
    }

    static Indirection Access(SPIRVResult res, uint32_t ptrType)
    {
        return Indirection{ .type = Indirection::Type::Access, .accessInfo = { .ptrType = ptrType, .dataType = res.typeName, .offset = res.name } };
    }

    static Indirection Address(uint32_t ptrType, uint32_t dataType, uint32_t indexType, uint32_t offset, uint32_t alignment)
    {
        return Indirection{ .type = Indirection::Type::Address, .addressInfo = { .ptrType = ptrType, .dataType = dataType, .indexType = indexType, .offset = offset, .alignment = alignment } };
    }

    static Indirection Address(uint32_t ptrType, uint32_t dataType, uint32_t alignment)
    {
        return Indirection{ .type = Indirection::Type::Address, .addressInfo = { .ptrType = ptrType, .dataType = dataType, .indexType = 0xFFFFFFFF, .offset = 0xFFFFFFFF, .alignment = alignment } };
    }

    static Indirection Pointer(uint32_t ptrType, uint32_t dataType, SPIRVResult::Storage storage)
    {
        return Indirection{ .type = Indirection::Type::Pointer, .pointerInfo = { .ptrType = ptrType, .dataType = dataType, .storage = (uint32_t)storage } };
    }

    static Indirection Array(uint32_t ptrType, uint32_t dataType, uint32_t indexType, uint32_t size)
    {
        return Indirection{ .type = Indirection::Type::Array, .arrayInfo = { .ptrType = ptrType, .dataType = dataType, .indexType = indexType, .size = size } };
    }

    struct LiteralValue
    {
        union
        {
            float f;
            float f32;
            short f16;
            int i;
            int i32;
            int16_t i16;
            uint32_t ui;
            uint32_t u32;
            uint16_t u16;
            uint64_t u64;
            bool b;
            bool b8;
        };

        TypeCode type;
    } literalValue;

    SPIRVResult ConvertTo(TypeCode toType) const
    {
        SPIRVResult result = *this;
        if (this->isLiteral)
        {
            switch (this->literalValue.type)
            {
                case TypeCode::Float32:
                    switch (toType)
                    {
                        case TypeCode::Float16:
                            result.literalValue.f16 = this->literalValue.f32;
                            break;
                        case TypeCode::Int32:
                            result.literalValue.i32 = this->literalValue.f32;
                            break;
                        case TypeCode::Int16:
                            result.literalValue.i16 = this->literalValue.f32;
                            break;
                        case TypeCode::UInt32:
                            result.literalValue.u32 = this->literalValue.f32;
                            break;
                        case TypeCode::UInt16:
                            result.literalValue.u16 = this->literalValue.f32;
                            break;
                        case TypeCode::Bool8:
                            result.literalValue.b = this->literalValue.f32 != 0.0f;
                            break;
                    }
                    break;
                case TypeCode::Float16:
                    switch (toType)
                    {
                        case TypeCode::Float32:
                            result.literalValue.f32 = this->literalValue.f16;
                            break;
                        case TypeCode::Int32:
                            result.literalValue.i32 = this->literalValue.f16;
                            break;
                        case TypeCode::Int16:
                            result.literalValue.i16 = this->literalValue.f16;
                            break;
                        case TypeCode::UInt32:
                            result.literalValue.u32 = this->literalValue.f16;
                            break;
                        case TypeCode::UInt16:
                            result.literalValue.u16 = this->literalValue.f16;
                            break;
                        case TypeCode::Bool8:
                            result.literalValue.b = this->literalValue.f16 != 0.0f;
                            break;
                    }
                    break;
                case TypeCode::Int32:
                    switch (toType)
                    {
                        case TypeCode::Float32:
                            result.literalValue.f32 = this->literalValue.i32;
                            break;
                        case TypeCode::Float16:
                            result.literalValue.f16 = this->literalValue.i32;
                            break;
                        case TypeCode::Int16:
                            result.literalValue.i16 = this->literalValue.i32;
                            break;
                        case TypeCode::UInt32:
                            result.literalValue.u32 = this->literalValue.i32;
                            break;
                        case TypeCode::UInt16:
                            result.literalValue.u16 = this->literalValue.i32;
                            break;
                        case TypeCode::Bool8:
                            result.literalValue.b = this->literalValue.i32 != 0;
                            break;
                    }
                    break;
                case TypeCode::Int16:
                    switch (toType)
                    {
                        case TypeCode::Float32:
                            result.literalValue.f32 = this->literalValue.i16;
                            break;
                        case TypeCode::Float16:
                            result.literalValue.f16 = this->literalValue.i16;
                            break;
                        case TypeCode::Int32:
                            result.literalValue.i16 = this->literalValue.i16;
                            break;
                        case TypeCode::UInt32:
                            result.literalValue.u32 = this->literalValue.i16;
                            break;
                        case TypeCode::UInt16:
                            result.literalValue.u16 = this->literalValue.i16;
                            break;
                        case TypeCode::Bool8:
                            result.literalValue.b = this->literalValue.i16 != 0;
                            break;
                    }
                    break;
                case TypeCode::UInt32:
                    switch (toType)
                    {
                        case TypeCode::Float32:
                            result.literalValue.f32 = this->literalValue.u32;
                            break;
                        case TypeCode::Float16:
                            result.literalValue.f16 = this->literalValue.u32;
                            break;
                        case TypeCode::Int32:
                            result.literalValue.i32 = this->literalValue.u32;
                            break;
                        case TypeCode::Int16:
                            result.literalValue.i16 = this->literalValue.u32;
                            break;
                        case TypeCode::UInt16:
                            result.literalValue.u16 = this->literalValue.u32;
                            break;
                        case TypeCode::Bool8:
                            result.literalValue.b = this->literalValue.u32 != 0;
                            break;
                    }
                    break;
                case TypeCode::UInt16:
                    switch (toType)
                    {
                        case TypeCode::Float32:
                            result.literalValue.f32 = this->literalValue.u16;
                            break;
                        case TypeCode::Float16:
                            result.literalValue.f16 = this->literalValue.u16;
                            break;
                        case TypeCode::Int32:
                            result.literalValue.i32 = this->literalValue.u16;
                            break;
                        case TypeCode::Int16:
                            result.literalValue.i16 = this->literalValue.u16;
                            break;
                        case TypeCode::UInt32:
                            result.literalValue.u16 = this->literalValue.u16;
                            break;
                        case TypeCode::Bool8:
                            result.literalValue.b = this->literalValue.u16 != 0;
                            break;
                    }
                    break;
                case TypeCode::Bool8:
                    switch (toType)
                    {
                        case TypeCode::Float32:
                            result.literalValue.f32 = this->literalValue.b;
                            break;
                        case TypeCode::Float16:
                            result.literalValue.f16 = this->literalValue.b;
                            break;
                        case TypeCode::Int32:
                            result.literalValue.i32 = this->literalValue.b;
                            break;
                        case TypeCode::Int16:
                            result.literalValue.i16 = this->literalValue.b;
                            break;
                        case TypeCode::UInt32:
                            result.literalValue.u32 = this->literalValue.b;
                            break;
                        case TypeCode::UInt16:
                            result.literalValue.u16 = this->literalValue.b;
                            break;
                    }
                    break;
            }
            result.literalValue.type = toType;
        }
        return result;
    }



    static ConstantString ScopeToString(Storage s)
    {
        switch (s)
        {
            case Storage::Device:
                return "CrossWorkgroup";
            case Storage::Private:
                return "Private";
            case Storage::WorkGroup:
                return "Workgroup";
            case Storage::Uniform:
                return "Uniform";
            case Storage::Sampler:
            case Storage::Image:
            case Storage::MutableImage:
            case Storage::UniformConstant:
                return "UniformConstant";
            case Storage::TexelPointer:
                return "Image";
            case Storage::StorageBuffer:
                return "StorageBuffer";
            case Storage::PhysicalBuffer:
                return "PhysicalStorageBuffer";
            case Storage::PushConstant:
                return "PushConstant";
            case Storage::Function:
                return "Function";
            case Storage::Input:
                return "Input";
            case Storage::Output:
                return "Output";
            case Storage::RayPayload:
                return "RayPayloadKHR";
            case Storage::RayPayloadInput:
                return "IncomingRayPayloadKHR";
            case Storage::RayHitAttribute:
                return "HitAttributeKHR";
            case Storage::CallableData:
                return "CallableDataKHR";
            case Storage::CallableDataInput:
                return "IncomingCallableDataKHR";
            default:
                assert(false);
                return "";
        }
    }



    SPIRVResult()
    {
        *this = SPIRVResult::Invalid();
    }

    SPIRVResult(uint32_t name, uint32_t type, bool isValue = false, bool isConstant = false, Storage scope = Storage::Function)
        : name(name)
        , typeName(type)
        , isValue(isValue)
        , isConst(isConstant)
        , scope(scope)
        , parentTypes({})
        , literalValue({})
    {
        this->swizzleMask.bits.x = Type::SwizzleMask::Invalid;
        this->swizzleMask.bits.y = Type::SwizzleMask::Invalid;
        this->swizzleMask.bits.z = Type::SwizzleMask::Invalid;
        this->swizzleMask.bits.w = Type::SwizzleMask::Invalid;
    };

    /*
    SPIRVResult(uint32_t name, uint32_t type, bool isValue, bool isConstant, Storage scope, const std::vector<uint32_t>& parentTypes, const std::vector<SPIRVResult::Storage>& parentScopes)
        : name(name)
        , typeName(type)
        , isValue(isValue)
        , isConst(isConstant)
        , scope(scope)
        , parentTypes(parentTypes)
        , parentScopes(parentScopes)
        , literalValue({})
    {
        assert(parentTypes.size() == parentScopes.size());
        this->swizzleMask.bits.x = Type::SwizzleMask::Invalid;
        this->swizzleMask.bits.y = Type::SwizzleMask::Invalid;
        this->swizzleMask.bits.z = Type::SwizzleMask::Invalid;
        this->swizzleMask.bits.w = Type::SwizzleMask::Invalid;
    };
    */

    explicit SPIRVResult(const ValueUnion& value, const uint8_t offset = 0)
    {
        this->literalValue = {};
        switch (value.code)
        {
            case TypeCode::Bool8:
                this->literalValue.type = TypeCode::Bool8;
                this->literalValue.b8 = value.b[offset];
                this->isLiteral = true;
                this->isValue = false;
                break;
            case TypeCode::UInt16:
                this->literalValue.type = TypeCode::UInt16;
                this->literalValue.u16 = value.ui[offset];
                this->isLiteral = true;
                this->isValue = false;
                break;
            case TypeCode::UInt32:
                this->literalValue.type = TypeCode::UInt32;
                this->literalValue.u32 = value.ui[offset];
                this->isLiteral = true;
                this->isValue = false;
                break;
            case TypeCode::Int16:
                this->literalValue.type = TypeCode::Int16;
                this->literalValue.i16 = value.i[offset];
                this->isLiteral = true;
                this->isValue = false;
                break;
            case TypeCode::Int32:
                this->literalValue.type = TypeCode::Int32;
                this->literalValue.i32 = value.i[offset];
                this->isLiteral = true;
                this->isValue = false;
                break;
            case TypeCode::Float16:
                this->literalValue.type = TypeCode::Float16;
                this->literalValue.f16 = value.f[offset];
                this->isLiteral = true;
                this->isValue = false;
                break;
            case TypeCode::Float32:
                this->literalValue.type = TypeCode::Float32;
                this->literalValue.f32 = value.f[offset];
                this->isLiteral = true;
                this->isValue = false;
                break;
        }
    }

    explicit SPIRVResult(float literal)
    {
        this->literalValue.type = TypeCode::Float32;
        this->literalValue.f = literal;
        this->isLiteral = true;
        this->isValue = false;
    }

    explicit SPIRVResult(int32_t literal)
    {
        this->literalValue.type = TypeCode::Int32;
        this->literalValue.i32 = literal;
        this->isLiteral = true;
        this->isValue = false;
    }
    
    explicit SPIRVResult(int16_t literal)
    {
        this->literalValue.type = TypeCode::Int16;
        this->literalValue.i16 = literal;
        this->isLiteral = true;
        this->isValue = false;
    }

    explicit SPIRVResult(bool literal)
    {
        this->literalValue.type = TypeCode::Bool8;
        this->literalValue.b8 = literal;
        this->isLiteral = true;
        this->isValue = false;
    }

    explicit SPIRVResult(uint32_t literal)
    {
        this->literalValue.type = TypeCode::UInt32;
        this->literalValue.u32 = literal;
        this->isLiteral = true;
        this->isValue = false;
    }
    
    explicit SPIRVResult(uint16_t literal)
    {
        this->literalValue.type = TypeCode::UInt16;
        this->literalValue.u16 = literal;
        this->isLiteral = true;
        this->isValue = false;
    }

    static SPIRVResult Invalid()
    {
        return SPIRVResult(0xFFFFFFFF, 0xFFFFFFFF);
    }

    bool operator!=(const SPIRVResult& rhs) { return this->name != rhs.name || this->typeName != rhs.typeName; }
    bool operator==(const SPIRVResult& rhs) { return this->name == rhs.name && this->typeName == rhs.typeName; }
};

struct SymbolAssignment
{
    Symbol* sym;
    SPIRVResult value;
};

class SPIRVGenerator : public Generator
{
public:

    /// Constructor
    SPIRVGenerator();

    /// Setup intrinsics
    static void SetupIntrinsics();

    /// generate SPIRV output
    bool Generate(const Compiler* compiler, const ProgramInstance* program, const PinnedArray<Symbol*>& symbols, std::function<void(const std::string&, const std::string&)> writerFunc) override;

    /// Push a type to the stack
    void PushAccessChain(SPIRVResult res);
    /// Pop a type from the stack
    void PopAccessChain();

    /// Construct a new Id
    SpvId* NewId();
    
    std::vector<SPIRVResult> accessChain;
    PinnedSet<uint32_t> interfaceVariables = 0xFFFF;
    PinnedArray<SpvId> ids = 0xFFFFFF;
    uint64_t idCounter = 0;

    bool blockOpen = false;
    bool literalExtract = false;
    bool linkDefineEvaluation = false;
    uint32_t linkDefineSlot = UINT32_MAX;

    bool skipBreakContinue = false;

    Function* entryPoint = nullptr;
    ProgramInstance::__Resolved* evaluatingProgram = nullptr;

    SPVWriter* writer;

    struct MergeBlock
    {
        uint32_t continueLabel, breakLabel;
    } mergeBlocks[256];
    uint32_t mergeBlockCounter;

    using FunctionToSPIRVMapping = std::function<SPIRVResult(const Compiler*, SPIRVGenerator*, uint32_t, const std::vector<SPIRVResult>&)>;
    static std::unordered_map<Function*, FunctionToSPIRVMapping> IntrinsicMap;
    std::unordered_map<Function*, FunctionToSPIRVMapping> generatorIntrinsics;

    struct TypeState
    {
        enum TypeLayout
        {
            Relaxed,    // Typical layout, no specific offsets or strides
            Explicit,   // Layout is explicitly defined via decorations
            Interface,  // Layout follows interface block rules (std140/std430), infers explicit
        } layout = Relaxed;
        SPIRVResult::Storage storage = SPIRVResult::Storage::Function;
    } typeState;

    uint32_t shaderStage = 0xFFFFFFFF;

    // For use with ray tracing, any function input or output value must be assigned to this variable as it's passed to the OpTraceRayKHR intrinsic
    SPIRVResult rayPayload;
    SPIRVResult callableData;
};

} // namespace GPULang
