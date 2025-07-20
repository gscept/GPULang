#pragma once
//------------------------------------------------------------------------------
/**
    AST for a program instance
 
    A program instance is a linker assembly object which combines a set of shaders
    and an optional render state which can trivially be lowered to a pipeline object at runtime.

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include <vector>
#include <unordered_map>
#include <map>
#include <string>
#include "annotation.h"
#include "symbol.h"
#include "ast/statements/statement.h"
namespace GPULang
{

struct Function;
struct ProgramInstance : public Symbol
{
    /// constructor
    ProgramInstance();
    /// destructor
    virtual ~ProgramInstance();

    FixedArray<Expression*> entries;
    _IMPLEMENT_ANNOTATIONS()

    struct __Resolved : public Symbol::__Resolved
    {
        virtual ~__Resolved() {};
        Type* typeSymbol;
        enum EntryType
        {
            InvalidProgramEntryType,
            VertexShader,
            FirstShader = VertexShader,
            HullShader,
            DomainShader,
            GeometryShader,
            PixelShader,
            ComputeShader,
            TaskShader,
            MeshShader,
            RayGenerationShader,
            RayMissShader,
            RayClosestHitShader,
            RayAnyHitShader,
            RayIntersectionShader,
            RayCallableShader,
            LastShader = RayCallableShader,
            RenderState,

            NumProgramEntries
        };

        /// convert from string to program entry type
        static const EntryType StringToEntryType(const TransientString& str);
        /// convert from mapping to string
        static const ConstantString& EntryTypeToString(const EntryType type);

        union ProgramUsage
        {
            enum ShaderPipelineBits
            {
                GraphicsBits = 0xDF,
                ComputeBits = 0x20,
                RayTracingBits = 0x3F00,
            };
            struct
            {
                uint32_t hasVertexShader : 1;
                uint32_t hasHullShader : 1;
                uint32_t hasDomainShader : 1;
                uint32_t hasGeometryShader : 1;
                uint32_t hasPixelShader : 1;
                uint32_t hasComputeShader : 1;
                uint32_t hasTaskShader : 1;
                uint32_t hasMeshShader : 1;
                uint32_t hasRayGenerationShader : 1;
                uint32_t hasRayMissShader : 1;
                uint32_t hasRayClosestHitShader : 1;
                uint32_t hasRayAnyHitShader : 1;
                uint32_t hasRayIntersectionShader : 1;
                uint32_t hasRayCallableShader : 1;
                uint32_t hasRenderState : 1;
            } flags;
            uint32_t bits;
        } usage;

        union ProgramEffects
        {
            struct
            {
                uint32_t explicitDepth : 1;
            } flags;
            uint32_t bits;
        } effects;
        Symbol* mappings[EntryType::NumProgramEntries];
        std::vector<uint32_t> binaries[EntryType::NumProgramEntries];
        PinnedMap<Function*, Function*> functionOverrides = 0xFFF;
        PinnedMap<Variable*, Expression*> constVarInitializationOverrides = 0xFFF;
    };

};

constexpr StaticMap programInstanceEntryTypeLookup =
std::array{
    std::pair{ "VertexShader"_c, ProgramInstance::__Resolved::VertexShader }
    , std::pair{ "HullShader"_c, ProgramInstance::__Resolved::HullShader }
    , std::pair{ "TessellationControlShader"_c, ProgramInstance::__Resolved::HullShader }
    , std::pair{ "DomainShader"_c, ProgramInstance::__Resolved::DomainShader }
    , std::pair{ "TessellationEvaluationShader"_c, ProgramInstance::__Resolved::DomainShader }
    , std::pair{ "GeometryShader"_c, ProgramInstance::__Resolved::GeometryShader }
    , std::pair{ "PixelShader"_c, ProgramInstance::__Resolved::PixelShader }
    , std::pair{ "ComputeShader"_c, ProgramInstance::__Resolved::ComputeShader }
    , std::pair{ "TaskShader"_c, ProgramInstance::__Resolved::TaskShader }
    , std::pair{ "MeshShader"_c, ProgramInstance::__Resolved::MeshShader }
    , std::pair{ "RayGenerationShader"_c, ProgramInstance::__Resolved::RayGenerationShader }
    , std::pair{ "RayMissShader"_c, ProgramInstance::__Resolved::RayMissShader }
    , std::pair{ "RayClosestHitShader"_c, ProgramInstance::__Resolved::RayClosestHitShader }
    , std::pair{ "RayAnyHitShader"_c, ProgramInstance::__Resolved::RayAnyHitShader }
    , std::pair{ "RayIntersectionShader"_c, ProgramInstance::__Resolved::RayIntersectionShader }
    , std::pair{ "RayCallableShader"_c, ProgramInstance::__Resolved::RayCallableShader }
    , std::pair{ "RenderState"_c, ProgramInstance::__Resolved::RenderState }
};


} // namespace GPULang
