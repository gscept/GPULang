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

    struct SubroutineMapping
    {
        std::string name;
        std::string value;
    };
    
    std::vector<Expression*> entries;
    _IMPLEMENT_ANNOTATIONS()

    struct __Resolved : public Symbol::__Resolved
    {
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
        static const ProgramEntryType StringToEntryType(const std::string& str);
        /// convert from mapping to string
        static const std::string& EntryTypeToString(const ProgramEntryType type);

        union ProgramUsage
        {
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
        Symbol* mappings[ProgramEntryType::NumProgramEntries];
        std::vector<uint32_t> binaries[ProgramEntryType::NumProgramEntries];
        std::unordered_map<Function*, Function*> functionOverrides;
        std::unordered_map<Variable*, Expression*> constVarInitializationOverrides;
    };

};

extern const std::unordered_map<std::string, Program::__Resolved::ProgramEntryType> programEntryTypeLookup;

} // namespace GPULang
