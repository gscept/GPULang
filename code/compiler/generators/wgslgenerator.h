#pragma once
//------------------------------------------------------------------------------
/**
    Compuler backend for WebGPU Shading Language

    (C) 2025 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "generator.h"
#include "ast/symbol.h"
#include "ast/types/type.h"
#include "ast/variable.h"
#include "ast/program.h"

namespace GPULang
{
class WGSLGenerator : public Generator
{
    /// Constructor
    WGSLGenerator();

    /// Setup intrinsics
    static void SetupIntrinsics();
    
    /// generate WGSL output
    bool Generate(const Compiler* compiler, const ProgramInstance* program, const PinnedArray<Symbol*>& symbols, std::function<void(const std::string&, const std::string&)> writerFunc) override;
};
}
