//------------------------------------------------------------------------------
//  compiler.cc
//  (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "compiler.h"
#include "ast/program.h"
#include "validators/validator.h"
#include "generators/hlslgenerator.h"
#include "generators/glslgenerator.h"
#include "generators/spirvgenerator.h"
#include "generators/hgenerator.h"
#include "util.h"
#include "v3/ast/types/type.h"
#include "v3/ast/function.h"
#include "v3/ast/variable.h"
#include "v3/ast/renderstate.h"
#include "v3/ast/samplerstate.h"
#include "v3/ast/program.h"
#include "v3/ast/structure.h"
#include "v3/ast/variable.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Compiler::Compiler() 
    : debugOutput(false)
    , declareTy(nullptr)
    , declareType(Type::FullType{})
    , ignoreReservedWords(false)
    , hasErrors(false)
{
    this->validator = new Validator;
    this->options.validate = true;
    this->options.optimize = false;
    this->options.warningsAsErrors = false;
    this->options.emitTimings = false;
    this->branchReturns = false;

    // push global scope for all the builtins
    this->PushScope(Scope::ScopeType::Global);

    // setup default types and their lookups
    if (DefaultTypes.empty())
        Type::SetupDefaultTypes();
    auto typeIt = DefaultTypes.begin();
    while (typeIt != DefaultTypes.end())
    {
        this->validator->ResolveType(this, *typeIt);
        typeIt++;
    }

    // Run again but resolve methods this time (needed as forward declaration)
    typeIt = DefaultTypes.begin();
    while (typeIt != DefaultTypes.end())
    {
        this->validator->ResolveTypeMethods(this, *typeIt);
        typeIt++;
    }

    // setup intrinsics
    if (DefaultIntrinsics.empty())
        Function::SetupIntrinsics();
    this->ignoreReservedWords = true;
    auto intrinIt = DefaultIntrinsics.begin();
    while (intrinIt != DefaultIntrinsics.end())
    {
        this->validator->ResolveFunction(this, *intrinIt);
        intrinIt++;
    }
    this->ignoreReservedWords = false;

    // push a new scope for all the parsed symbols
    this->PushScope(Scope::ScopeType::Global);
}

//------------------------------------------------------------------------------
/**
*/
Compiler::~Compiler()
{
    for (Symbol* sym : this->dynamicSymbols)
        delete sym;
    delete this->validator;
}

//------------------------------------------------------------------------------
/**
*/
void 
Compiler::Setup(const Compiler::Language& lang, const std::vector<std::string>& defines, Options options, unsigned int version)
{
    this->lang = lang;
    switch (lang)
    {
    case Language::GLSL_SPIRV:
        this->target.name = "GLSL-SPIRV";
        this->target.generator = new GLSLGenerator(GLSLGenerator::VulkanFeatureSet);
        break;
    case Language::GLSL:
        this->target.name = "GLSL";
        this->target.generator = new GLSLGenerator(GLSLGenerator::OpenGLFeatureSet);
        break;
    case Language::HLSL_SPIRV:
    case Language::HLSL:
        this->target.name = "HLSL";
        this->target.generator = new HLSLGenerator;
        break;
    case Language::SPIRV:
        this->target.name = "SPIRV";
        this->target.generator = new SPIRVGenerator();
        this->target.supportsPhysicalAddressing = true;
        break;
    case Language::VULKAN_SPIRV:
        this->target.name = "VULKAN-SPIRV";
        this->target.generator = new SPIRVGenerator();
        this->target.supportsPhysicalAddressing = false;
        break;
    }

    this->options = options;

    // if we want other header generators in the future, add here
    this->headerGenerator = new HGenerator();
}

//------------------------------------------------------------------------------
/**
*/
bool 
Compiler::AddSymbol(const std::string& name, Symbol* symbol, bool allowDuplicate, bool bypass)
{
    Scope* scope = this->scopes.back();
    if (bypass)
        scope = *(this->scopes.end() - 1);
    std::multimap<std::string, Symbol*>* lookup;
    std::vector<Symbol*>* symbols;
    if (scope->type == Scope::ScopeType::Type)
    {
        Type* type = static_cast<Type*>(scope->owningSymbol);
        lookup = &type->lookup;
        symbols = &type->symbols;
    }
    else
    {
        lookup = &scope->symbolLookup;
        symbols = &scope->symbols;
    }

    auto it = lookup->find(name);
    if (it != lookup->end() && !allowDuplicate)
    {
        Symbol* prevSymbol = it->second;
        this->Error(Format("Symbol %s redefinition, previous definition at %s(%d)", name.c_str(), prevSymbol->location.file.c_str(), prevSymbol->location.line), symbol);
        return false;
    }
    lookup->insert({ name, symbol });
    // Only add to symbols if scope type isn't a type, because they already have the symbols setup
    if (scope->type != Scope::ScopeType::Type)
        symbols->push_back(symbol);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
Symbol* 
Compiler::GetSymbol(const std::string& name) const
{
    auto scopeIter = this->scopes.rbegin();
    do
    {
        auto scope = *scopeIter;
        std::multimap<std::string, Symbol*>* map;
        if (scope->type == Scope::ScopeType::Type)
        {
            Type* type = static_cast<Type*>(scope->owningSymbol);
            map = &type->lookup;
        }
        else
        {
            map = &scope->symbolLookup;
        }
        auto it = map->find(name);
        if (it != map->end())
            return it->second;
        scopeIter++;
    } 
    while (scopeIter != this->scopes.rend());
    return nullptr;
}

//------------------------------------------------------------------------------
/**
*/
std::vector<Symbol*>
Compiler::GetSymbols(const std::string& name) const
{
    std::vector<Symbol*> ret;
    auto scopeIter = this->scopes.rbegin();
    do
    {
        auto scope = *scopeIter;
        std::multimap<std::string, Symbol*>* map;
        if (scope->type == Scope::ScopeType::Type)
        {
            Type* type = static_cast<Type*>(scope->owningSymbol);
            map = &type->lookup;
        }
        else
        {
            map = &scope->symbolLookup;
        }

        auto range = map->equal_range(name);
        for (auto it = range.first; it != range.second; it++)
            ret.push_back((*it).second);
        scopeIter++;
    } while (scopeIter != this->scopes.rend());
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
void 
Compiler::PushScope(Scope::ScopeType type, Symbol* owner)
{
    this->scopes.push_back(new Scope());
    this->scopes.back()->type = type;
    this->scopes.back()->owningSymbol = owner;
    this->scopes.back()->unreachable = false;
}

//------------------------------------------------------------------------------
/**
*/
void 
Compiler::PushScope(Type* type)
{
    this->scopes.push_back(new Scope());
    this->scopes.back()->type = Scope::ScopeType::Type;
    this->scopes.back()->owningSymbol = type;
}

//------------------------------------------------------------------------------
/**
*/
void 
Compiler::PopScope()
{
    Scope* currentScope = this->scopes.back();
    delete currentScope;
    this->scopes.pop_back();
}

//------------------------------------------------------------------------------
/**
*/
bool 
Compiler::IsScopeGlobal()
{
    return this->scopes.back()->type == Scope::ScopeType::Global;
}

//------------------------------------------------------------------------------
/**
*/
bool 
Compiler::IsScopeType()
{
    return this->scopes.back()->type == Scope::ScopeType::Type;
}

//------------------------------------------------------------------------------
/**
*/
Symbol* 
Compiler::GetScopeOwner()
{
    Symbol* lastSymbol = nullptr;
    auto it = this->scopes.rbegin();
    while (it != this->scopes.rend())
    {
        lastSymbol = (*it)->owningSymbol;
        if (lastSymbol != nullptr)
            break;
        it++;
    }
    return lastSymbol;
}

//------------------------------------------------------------------------------
/**
*/
Symbol* 
Compiler::GetParentScopeOwner(Symbol::SymbolType type)
{
    auto it = this->scopes.rbegin();
    while (it != this->scopes.rend())
    {
        if ((*it)->owningSymbol != nullptr && (*it)->owningSymbol->symbolType == type)
            return (*it)->owningSymbol;
        it++;
    }
    return nullptr;
}

//------------------------------------------------------------------------------
/**
*/
void 
Compiler::MarkScopeUnreachable()
{
    this->scopes.back()->unreachable = true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
Compiler::IsUnreachable()
{
    return this->scopes.back()->unreachable;
}

//------------------------------------------------------------------------------
/**
*/
void 
Compiler::MarkScopeReachable()
{
    this->scopes.back()->unreachable = false;
}

//------------------------------------------------------------------------------
/**
*/
bool 
Compiler::Compile(Effect* root, BinWriter& binaryWriter, TextWriter& headerWriter)
{
    bool ret = true;

    this->symbols = root->symbols;

        this->performanceTimer.Start();

    // resolves parser state and runs validation
    for (this->symbolIterator = 0; this->symbolIterator < this->symbols.size(); this->symbolIterator++)
    {
        ret &= this->validator->Resolve(this, this->symbols[this->symbolIterator]);
    }

    this->performanceTimer.Stop();

    // if failed, don't proceed to next step
    if (!ret || this->hasErrors)
        return false;

    if (this->options.emitTimings)
        this->performanceTimer.Print("Type checking");

    // setup potential debug output stream
    std::function<void(const std::string&, const std::string&)> writeFunction = nullptr;
    if (this->debugOutput)
    {
        writeFunction = [this](const std::string& name, const std::string& code)
        {
            TextWriter outputStream;
            outputStream.SetPath(this->debugPath + "/" + name);
            if (outputStream.Open())
            {
                outputStream.WriteString(code);
                outputStream.Close();
            }
        };
    }


    this->performanceTimer.Start();

    // collect programs
    for (Symbol* symbol : this->symbols)
    {
        if (symbol->symbolType == Symbol::ProgramType)
        {
            ret &= this->target.generator->Generate(this, static_cast<Program*>(symbol), this->symbols, writeFunction);
        }
    }

    this->performanceTimer.Stop();

    if (this->options.emitTimings)
        this->performanceTimer.Print("Code generation");

    // if failed, don't proceed to next step
    if (!ret || this->hasErrors)
        return false;

    // output binary if possible
    if (binaryWriter.Open())
    {
        this->performanceTimer.Start();

        // write magic
        binaryWriter.WriteInt('AFX3');

        // run binary output step
        Serialize::DynamicLengthBlob blob;
        for (Symbol* symbol : this->symbols)
        {
            this->OutputBinary(symbol, binaryWriter, blob);
        }

        // output dynamic blob
        binaryWriter.WriteBytes(&blob.data[0], blob.iterator);

        // write size of dynamic blob at the end
        binaryWriter.WriteUInt(blob.iterator);
        binaryWriter.Close();

        this->performanceTimer.Stop();

        if (this->options.emitTimings)
            this->performanceTimer.Print("Reflection serialization");
    }
    else
    {
        return false;
    }

    // header writing is optional
    if (headerWriter.Open())
    {
        this->performanceTimer.Start();

        ret &= this->headerGenerator->Generate(this, nullptr, this->symbols, [&headerWriter](const std::string& name, const std::string& code)
        {
            headerWriter.WriteString(code);
        });

        headerWriter.Close();

        this->performanceTimer.Stop();

        if (this->options.emitTimings)
            this->performanceTimer.Print("Header generation");
    }

    return ret;
}

//------------------------------------------------------------------------------
/**
*/
void 
Compiler::Error(const std::string& msg, const std::string& file, int line, int column)
{
    if (line == -1) 
    {
        std::string err = Format("%s(%s) : internal error error: %s", "?", "?", msg.c_str());
        this->messages.push_back(err);
    }
    else
    {
        std::string err = Format("%s(%d) : error: %s", file.c_str(), line, msg.c_str());
        this->messages.push_back(err);
    }
    this->hasErrors = true;
}

//------------------------------------------------------------------------------
/**
*/
void 
Compiler::Error(const std::string& msg, const Symbol* sym)
{
    this->Error(msg, sym->location.file, sym->location.line, sym->location.column);
}

//------------------------------------------------------------------------------
/**
*/
void 
Compiler::Warning(const std::string& msg, const std::string& file, int line, int column)
{
    std::string err = Format("%s(%d) : warning: %s", file.c_str(), line, msg.c_str());
    this->messages.push_back(err);
    if (this->options.warningsAsErrors)
        this->hasErrors = true;
}

//------------------------------------------------------------------------------
/**
*/
void 
Compiler::Warning(const std::string& msg, const Symbol* sym)
{
    this->Warning(msg, sym->location.file, sym->location.line, sym->location.column);
}

//------------------------------------------------------------------------------
/**
*/
void 
Compiler::GeneratorError(const std::string& msg)
{
    this->messages.push_back(msg);
}

//------------------------------------------------------------------------------
/**
*/
void 
Compiler::UnrecognizedTypeError(const std::string& type, Symbol* sym)
{
    this->Error(Format("Unrecognized type '%s'", type.c_str()), sym);
}

//------------------------------------------------------------------------------
/**
*/
void 
Compiler::UnrecognizedSymbolError(const std::string& symbol, Symbol* sym)
{
    this->Error(Format("Unrecognized symbol '%s'", symbol.c_str()), sym);
}

//------------------------------------------------------------------------------
/**
*/
void 
Compiler::ReservedPrefixError(const std::string& name, const std::string& word, Symbol* sym)
{
    this->Error(Format("Reserved word '%s'. No variables or functions are allowed to be prefixed with '%s'", name.c_str(), word.c_str()), sym);
}

//------------------------------------------------------------------------------
/**
*/
void
WriteAnnotation(Compiler* compiler, const Annotation& annot, size_t offset, Serialize::DynamicLengthBlob& dynamicDataBlob)
{
    Serialize::Annotation output;
    output.nameLength = annot.name.length();
    output.nameOffset = dynamicDataBlob.Write(annot.name.c_str(), annot.name.length());
    if (annot.value != nullptr)
    {
        switch (annot.value->symbolType)
        {
        case Symbol::StringExpressionType:
        {
            std::string str = annot.value->EvalString();
            output.data.s.stringOffset = dynamicDataBlob.Write(str.c_str(), str.length());
            output.data.s.stringLength = str.length();
            output.type = Serialize::StringType;
            break;
        }
        case Symbol::IntExpressionType:
        {
            unsigned int i;
            bool res = annot.value->EvalUInt(i);
            assert(res == true);
            output.data.i = i;
            output.type = Serialize::IntType;
            break;
        }
        case Symbol::FloatExpressionType:
        {
            float f;
            bool res = annot.value->EvalFloat(f);
            assert(res == true);
            output.data.f = f;
            output.type = Serialize::FloatType;
            break;
        }
        case Symbol::BoolExpressionType:
        {
            bool b;
            bool res = annot.value->EvalBool(b);
            assert(res == true);
            output.data.b = b;
            output.type = Serialize::BoolType;
            break;
        }
        }
    }
    else
    {
        output.type = Serialize::NoData;
    }
    dynamicDataBlob.WriteReserved(output, offset);
}

//------------------------------------------------------------------------------
/**
*/
void 
Compiler::OutputBinary(Symbol* symbol, BinWriter& writer, Serialize::DynamicLengthBlob& dynamicDataBlob)
{
    if (symbol->symbolType == Symbol::ProgramType)
    {
        Program* program = static_cast<Program*>(symbol);
        Program::__Resolved* resolved = static_cast<Program::__Resolved*>(symbol->resolved);
        Serialize::Program output;
        output.nameLength = symbol->name.length();
        output.nameOffset = dynamicDataBlob.Write(symbol->name.c_str(), symbol->name.length());

        if (resolved->usage.flags.hasVertexShader)
        {
            const std::vector<uint32_t>& binary = resolved->binaries[Program::__Resolved::VertexShader];
            output.vs.binaryOffset = dynamicDataBlob.Write((const char*)&binary.front(), binary.size() * sizeof(uint32_t));
            output.vs.binaryLength = binary.size() * sizeof(uint32_t);
        }
        else
        {
            output.vs.binaryOffset = -1;
            output.vs.binaryLength = -1;
        }

        if (resolved->usage.flags.hasHullShader)
        {
            const std::vector<uint32_t>& binary = resolved->binaries[Program::__Resolved::HullShader];
            output.hs.binaryOffset = dynamicDataBlob.Write((const char*)&binary.front(), binary.size() * sizeof(uint32_t));
            output.hs.binaryLength = binary.size() * sizeof(uint32_t);
        }
        else
        {
            output.hs.binaryOffset = -1;
            output.hs.binaryLength = -1;
        }

        if (resolved->usage.flags.hasDomainShader)
        {
            const std::vector<uint32_t>& binary = resolved->binaries[Program::__Resolved::DomainShader];
            output.ds.binaryOffset = dynamicDataBlob.Write((const char*)&binary.front(), binary.size() * sizeof(uint32_t));
            output.ds.binaryLength = binary.size() * sizeof(uint32_t);
        }
        else
        {
            output.ds.binaryOffset = -1;
            output.ds.binaryLength = -1;
        }

        if (resolved->usage.flags.hasGeometryShader)
        {
            const std::vector<uint32_t>& binary = resolved->binaries[Program::__Resolved::GeometryShader];
            output.gs.binaryOffset = dynamicDataBlob.Write((const char*)&binary.front(), binary.size() * sizeof(uint32_t));
            output.gs.binaryLength = binary.size() * sizeof(uint32_t);
        }
        else
        {
            output.gs.binaryOffset = -1;
            output.gs.binaryLength = -1;
        }

        if (resolved->usage.flags.hasPixelShader)
        {
            const std::vector<uint32_t>& binary = resolved->binaries[Program::__Resolved::PixelShader];
            output.ps.binaryOffset = dynamicDataBlob.Write((const char*)&binary.front(), binary.size() * sizeof(uint32_t));
            output.ps.binaryLength = binary.size() * sizeof(uint32_t);
        }
        else
        {
            output.ps.binaryOffset = -1;
            output.ps.binaryLength = -1;
        }

        if (resolved->usage.flags.hasComputeShader)
        {
            const std::vector<uint32_t>& binary = resolved->binaries[Program::__Resolved::ComputeShader];
            output.cs.binaryOffset = dynamicDataBlob.Write((const char*)&binary.front(), binary.size() * sizeof(uint32_t));
            output.cs.binaryLength = binary.size() * sizeof(uint32_t);
        }
        else
        {
            output.cs.binaryOffset = -1;
            output.cs.binaryLength = -1;
        }

        if (resolved->usage.flags.hasTaskShader)
        {
            const std::vector<uint32_t>& binary = resolved->binaries[Program::__Resolved::TaskShader];
            output.cs.binaryOffset = dynamicDataBlob.Write((const char*)&binary.front(), binary.size() * sizeof(uint32_t));
            output.cs.binaryLength = binary.size() * sizeof(uint32_t);
        }
        else
        {
            output.cs.binaryOffset = -1;
            output.cs.binaryLength = -1;
        }

        if (resolved->usage.flags.hasMeshShader)
        {
            const std::vector<uint32_t>& binary = resolved->binaries[Program::__Resolved::MeshShader];
            output.cs.binaryOffset = dynamicDataBlob.Write((const char*)&binary.front(), binary.size() * sizeof(uint32_t));
            output.cs.binaryLength = binary.size() * sizeof(uint32_t);
        }
        else
        {
            output.cs.binaryOffset = -1;
            output.cs.binaryLength = -1;
        }

        if (resolved->usage.flags.hasRayGenerationShader)
        {
            const std::vector<uint32_t>& binary = resolved->binaries[Program::__Resolved::RayGenerationShader];
            output.rgs.binaryOffset = dynamicDataBlob.Write((const char*)&binary.front(), binary.size() * sizeof(uint32_t));
            output.rgs.binaryLength = binary.size() * sizeof(uint32_t);
        }
        else
        {
            output.rgs.binaryOffset = -1;
            output.rgs.binaryLength = -1;
        }

        if (resolved->usage.flags.hasRayMissShader)
        {
            const std::vector<uint32_t>& binary = resolved->binaries[Program::__Resolved::RayMissShader];
            output.rms.binaryOffset = dynamicDataBlob.Write((const char*)&binary.front(), binary.size() * sizeof(uint32_t));
            output.rms.binaryLength = binary.size() * sizeof(uint32_t);
        }
        else
        {
            output.rms.binaryOffset = -1;
            output.rms.binaryLength = -1;
        }

        if (resolved->usage.flags.hasRayClosestHitShader)
        {
            const std::vector<uint32_t>& binary = resolved->binaries[Program::__Resolved::RayClosestHitShader];
            output.rchs.binaryOffset = dynamicDataBlob.Write((const char*)&binary.front(), binary.size() * sizeof(uint32_t));
            output.rchs.binaryLength = binary.size() * sizeof(uint32_t);
        }
        else
        {
            output.rchs.binaryOffset = -1;
            output.rchs.binaryLength = -1;
        }

        if (resolved->usage.flags.hasRayAnyHitShader)
        {
            const std::vector<uint32_t>& binary = resolved->binaries[Program::__Resolved::RayAnyHitShader];
            output.rahs.binaryOffset = dynamicDataBlob.Write((const char*)&binary.front(), binary.size() * sizeof(uint32_t));
            output.rahs.binaryLength = binary.size() * sizeof(uint32_t);
        }
        else
        {
            output.rahs.binaryOffset = -1;
            output.rahs.binaryLength = -1;
        }

        if (resolved->usage.flags.hasRayIntersectionShader)
        {
            const std::vector<uint32_t>& binary = resolved->binaries[Program::__Resolved::RayIntersectionShader];
            output.ris.binaryOffset = dynamicDataBlob.Write((const char*)&binary.front(), binary.size() * sizeof(uint32_t));
            output.ris.binaryLength = binary.size() * sizeof(uint32_t);
        }
        else
        {
            output.ris.binaryOffset = -1;
            output.ris.binaryLength = -1;
        }

        if (resolved->usage.flags.hasRayCallableShader)
        {
            const std::vector<uint32_t>& binary = resolved->binaries[Program::__Resolved::RayCallableShader];
            output.rhs.binaryOffset = dynamicDataBlob.Write((const char*)&binary.front(), binary.size() * sizeof(uint32_t));
            output.rhs.binaryLength = binary.size() * sizeof(uint32_t);
        }
        else
        {
            output.rhs.binaryOffset = -1;
            output.rhs.binaryLength = -1;
        }
        if (resolved->usage.flags.hasRenderState)
        {
            Symbol* renderState = resolved->programMappings[Program::__Resolved::RenderState];
            output.renderStateLength = renderState->name.length();
            output.renderStateOffset = dynamicDataBlob.Write(renderState->name.c_str(), renderState->name.length());
        }

        output.annotationsOffset = dynamicDataBlob.Reserve<Serialize::Annotation>(program->annotations.size());
        output.annotationsCount = program->annotations.size();

        size_t offset = output.annotationsOffset;
        for (const Annotation& annot : program->annotations)
        {
            WriteAnnotation(this, annot, offset, dynamicDataBlob);
            offset += sizeof(Serialize::Annotation);
        }

        writer.WriteType(output);
    }
    else if (symbol->symbolType == Symbol::RenderStateType)
    {
        RenderState::__Resolved* resolved = static_cast<RenderState::__Resolved*>(symbol->resolved);
        Serialize::RenderState output;
        output.nameLength = symbol->name.length();
        output.nameOffset = dynamicDataBlob.Write(symbol->name.c_str(), symbol->name.length());
        output.depthClampEnabled = resolved->depthClampEnabled;
        output.noPixels = resolved->noPixels;
        output.polygonMode = resolved->polygonMode;
        output.cullMode = resolved->cullMode;
        output.windingOrderMode = resolved->windingOrderMode;
        output.depthBiasEnabled = resolved->depthBiasEnabled;
        output.depthBiasFactor = resolved->depthBiasFactor;
        output.depthBiasClamp = resolved->depthBiasClamp;
        output.depthBiasSlopeFactor = resolved->depthBiasSlopeFactor;
        output.lineWidth = resolved->lineWidth;
        output.depthTestEnabled = resolved->depthTestEnabled;
        output.depthWriteEnabled = resolved->depthWriteEnabled;
        output.depthCompare = resolved->depthCompare;
        output.depthBoundsTestEnabled = resolved->depthBoundsTestEnabled;
        output.minDepthBounds = resolved->minDepthBounds;
        output.maxDepthBounds = resolved->maxDepthBounds;
        output.stencilEnabled = resolved->stencilEnabled;
        output.frontStencilState = resolved->frontStencilState;
        output.backStencilState = resolved->backStencilState;
        output.logicOpEnabled = resolved->logicOpEnabled;
        output.logicOp = resolved->logicOp;
        output.depthClampEnabled = resolved->depthClampEnabled;
        output.backStencilState = resolved->backStencilState;

        output.blendStatesOffset = dynamicDataBlob.iterator;
        output.blendStatesCount = 8 * sizeof(BlendState);
        for (int i = 0; i < 8; i++)
        {
            const BlendState& state = resolved->blendStates[i];
            dynamicDataBlob.Write(state);
        }

        writer.WriteType(output);
    }
    else if (symbol->symbolType == Symbol::SamplerStateType)
    {
        SamplerState::__Resolved* resolved = static_cast<SamplerState::__Resolved*>(symbol->resolved);
        Serialize::SamplerState output;

        output.nameLength = symbol->name.length();
        output.nameOffset = dynamicDataBlob.Write(symbol->name.c_str(), symbol->name.length());
        output.addressU = resolved->addressU;
        output.addressV = resolved->addressV;
        output.addressW = resolved->addressW;
        output.minFilter = resolved->minFilter;
        output.magFilter = resolved->magFilter;
        output.mipFilter = resolved->mipFilter;
        output.mipLodBias = resolved->mipLodBias;
        output.anisotropicEnabled = resolved->anisotropicEnabled;
        output.maxAnisotropy = resolved->maxAnisotropy;
        output.compareSamplerEnabled = resolved->compareSamplerEnabled;
        output.compareMode = resolved->compareMode;
        output.minLod = resolved->minLod;
        output.maxLod = resolved->maxLod;
        output.borderColor = resolved->borderColor;
        output.unnormalizedSamplingEnabled = resolved->unnormalizedSamplingEnabled;

        writer.WriteType(output);
    }
    else if (symbol->symbolType == Symbol::StructureType)
    {
        Structure* structure = static_cast<Structure*>(symbol);
        Structure::__Resolved* resolved = static_cast<Structure::__Resolved*>(symbol->resolved);
        Serialize::Structure output;
        output.isUniform = false;
        output.isMutable = false;
        output.binding = resolved->binding;
        output.group = resolved->group;
        output.nameLength = symbol->name.length();
        output.nameOffset = dynamicDataBlob.Write(symbol->name.c_str(), symbol->name.length());
        output.size = resolved->byteSize;
        if (resolved->usageFlags.flags.isUniformBuffer)
        {
            output.isUniform = true;
        }
        else if (resolved->usageFlags.flags.isMutableBuffer)
        {
            output.isMutable = true;
        }

        output.variablesCount = 0;
        for (Symbol* sym : structure->symbols)
        {
            if (sym->symbolType == Symbol::VariableType)
                output.variablesCount++;
        }

        // start serializing variables
        output.variablesOffset = dynamicDataBlob.Reserve<Serialize::Variable>(output.variablesCount);

        size_t offset = output.variablesOffset;
        for (Symbol* sym : structure->symbols)
        {
            if (sym->symbolType == Symbol::VariableType)
            {
                Variable* var = static_cast<Variable*>(sym);
                Variable::__Resolved* resolved = static_cast<Variable::__Resolved*>(var->resolved);
                Serialize::Variable varOutput;
                varOutput.binding = -1;
                varOutput.group = -1;
                varOutput.nameLength = var->name.length();
                varOutput.nameOffset = dynamicDataBlob.Write(var->name.c_str(), var->name.length());
                varOutput.byteSize = resolved->byteSize;
                varOutput.structureOffset = resolved->structureOffset;
                varOutput.arraySizesCount = resolved->type.modifierValues.size();
                varOutput.arraySizesOffset = dynamicDataBlob.Write(resolved->type.modifierValues.begin(), resolved->type.modifierValues.size());

                // write variable
                dynamicDataBlob.WriteReserved(varOutput, offset);
                offset += sizeof(Serialize::Variable);
            }
        }

        // end serializing variables

        output.annotationsOffset = dynamicDataBlob.Reserve<Serialize::Annotation>(structure->annotations.size());
        output.annotationsCount = structure->annotations.size();

        size_t annotOffset = output.annotationsOffset;
        for (const Annotation& annot : structure->annotations)
        {
            WriteAnnotation(this, annot, annotOffset, dynamicDataBlob);
            annotOffset += sizeof(Serialize::Annotation);
        }

        writer.WriteType(output);
    }
    else if (symbol->symbolType == Symbol::VariableType)
    {
        Variable* var = static_cast<Variable*>(symbol);
        Variable::__Resolved* resolved = static_cast<Variable::__Resolved*>(symbol->resolved);
        Serialize::Variable output;
        output.binding = resolved->binding;
        output.group = resolved->group;
        output.nameLength = symbol->name.length();
        output.nameOffset = dynamicDataBlob.Write(symbol->name.c_str(), symbol->name.length());
        output.byteSize = resolved->byteSize;
        output.structureOffset = resolved->structureOffset;
        output.arraySizesCount = resolved->type.modifierValues.size();
        output.arraySizesOffset = dynamicDataBlob.Write(resolved->type.modifierValues.begin(), resolved->type.modifierValues.size());
        output.annotationsOffset = dynamicDataBlob.Reserve<Serialize::Annotation>(var->annotations.size());
        output.annotationsCount = var->annotations.size();

        size_t offset = output.annotationsOffset;
        for (const Annotation& annot : var->annotations)
        {
            WriteAnnotation(this, annot, offset, dynamicDataBlob);
            offset += sizeof(Serialize::Annotation);
        }

        writer.WriteType(output);
    }
}

} // namespace GPULang
