#pragma once
//------------------------------------------------------------------------------
/**
    The validator performs type checking and other validation.
    It provides a set of function pointers for validation, which the implementations bind

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "ast/symbol.h"
#include "ast/types/type.h"
#include <set>
#include <map>
#include <vector>
namespace GPULang
{

struct Compiler;
struct Expression;
struct Statement;
struct Validator
{
    /// constructor
    Validator();

    /// Resolve
    bool Resolve(Compiler* compiler, Symbol* symbol);

    /// Resolve alias
    bool ResolveAlias(Compiler* compiler, Symbol* symbol);
    /// Resolve type
    bool ResolveType(Compiler* compiler, Symbol* symbol);
    /// Resolve type operators
    bool ResolveTypeMethods(Compiler* compiler, Symbol* symbol);
    /// Resolve swizzles 
    bool ResolveTypeSwizzles(Compiler* compiler, Symbol* symbol);
    /// Resolve blend state
    bool ResolveSamplerState(Compiler* compiler, Symbol* symbol);
    /// Resolve function
    bool ResolveFunction(Compiler* compiler, Symbol* symbol);
    /// Resolve program
    bool ResolveProgram(Compiler* compiler, Symbol* symbol);
    /// Resolve render state
    bool ResolveRenderState(Compiler* compiler, Symbol* symbol);
    /// Resolve structure 
    bool ResolveStructure(Compiler* compiler, Symbol* symbol);
    /// Resolve enumeration
    bool ResolveEnumeration(Compiler* compiler, Symbol* symbol);
    /// Resolve variable
    bool ResolveVariable(Compiler* compiler, Symbol* symbol);
    /// Resolve statement
    bool ResolveStatement(Compiler* compiler, Symbol* symbol);
    /// Resolve generate
    bool ResolveGenerate(Compiler* compiler, Symbol* symbol);

    /// Validate function
    bool ValidateFunction(Compiler* compiler, Symbol* symbol);
    /// Validate program
    bool ValidateProgram(Compiler* compiler, Symbol* symbol);
    /// Validate type
    bool ValidateType(Compiler* compiler, const Type::FullType& type, Type* typeSymbol, Symbol* sym);

    /// Resolve variable visibility
    bool ResolveVisibility(Compiler* compiler, Symbol* symbol);

    PinnedSet<FixedString> allowedTextureAttributes = 0xFF;
    PinnedSet<FixedString> allowedSamplerAttributes = 0xFF;
    PinnedSet<FixedString> allowedScalarAttributes = 0xFF;
    PinnedSet<FixedString> allowedPointerAttributes = 0xFF;
    PinnedSet<FixedString> allowedArrayAttributes = 0xFF;

    PinnedSet<FixedString> allowedSamplerStateAttributes = 0xFF;

    PinnedSet<FixedString> allowedFunctionAttributes = 0xFF;

    PinnedSet<FixedString> allowedParameterAttributes = 0xFF;

    PinnedSet<FixedString> allowedStructureAttributes = 0xFF;

    enum ResourceIndexingMode
    {
        ResourceIndexingByType,    // resource indexing is by type, so each constant buffer, storage buffer, texture and sampler have their own set of indexes
        ResourceIndexingByGroup    // resource indexing is by group, so each resource in a group get an incremental index
    };
    ResourceIndexingMode resourceIndexingMode;
    PinnedMap<uint32_t, uint32_t> resourceIndexCounter = 0xFFFF;
    PinnedMap<uint32_t, PinnedMap<uint32_t, Type::Category>> resourceTypePerGroupAndBinding = 0xFFFF;
    uint8_t defaultGroup;
    
    uint32_t inParameterIndexCounter;
    uint32_t outParameterIndexCounter;
};

} // namespace GPULang
