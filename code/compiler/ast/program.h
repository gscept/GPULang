#pragma once
//------------------------------------------------------------------------------
/**
    AST for Program

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
struct Program : public Symbol
{
    /// constructor
    Program();
    /// destructor
    virtual ~Program();

    FixedArray<Expression*> entries;
    _IMPLEMENT_ANNOTATIONS()

    struct __Resolved : public Symbol::__Resolved
    {
        virtual ~__Resolved() {};
        Type* typeSymbol;
        enum ProgramEntryType
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
        static const ProgramEntryType StringToEntryType(const TransientString& str);
        /// convert from mapping to string
        static const ConstantString& EntryTypeToString(const ProgramEntryType type);

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
        Symbol* mappings[ProgramEntryType::NumProgramEntries];
        std::vector<uint32_t> binaries[ProgramEntryType::NumProgramEntries];
        PinnedMap<Function*, Function*> functionOverrides = 0xFFF;
        PinnedMap<Variable*, Expression*> constVarInitializationOverrides = 0xFFF;
    };

};

extern const StaticMap<ConstantString, Program::__Resolved::ProgramEntryType> programEntryTypeLookup;

} // namespace GPULang
