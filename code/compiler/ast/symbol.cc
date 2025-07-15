//------------------------------------------------------------------------------
//  symbol.cc
//  (C) 2013 gscept
//------------------------------------------------------------------------------
#include "symbol.h"
#include "util.h"
#include "compiler.h"

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Symbol::Symbol() :
    symbolType(InvalidType)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Symbol::~Symbol()
{
    if (this->resolved != nullptr)
        this->resolved->~__Resolved();
}

//------------------------------------------------------------------------------
/**
*/
std::string
Symbol::GetLocation() const
{
    std::string result = Format("%d:%d in file %s", this->location.line, this->location.start, this->location.file.c_str());
    return result;
}

//------------------------------------------------------------------------------
/**
*/
const TransientString
Symbol::TypeToString() const
{
    switch (this->symbolType)
    {
    case ProgramType:
        return "program";
    case RenderStateInstanceType:
        return "render_state";
    case SamplerStateInstanceType:
        return "sampler_state";
    case VariableType:
        return "variable";
    case SubroutineType:
        return "subroutine";
    case FunctionType:
        return "function";
    case StructureType:
        return "struct";
    case FloatExpressionType:
        return "f32";
    case IntExpressionType:
        return "i32";
    case BoolExpressionType:
        return "b8";
    }
    return "";
}

} // namespace GPULang
