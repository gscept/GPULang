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
#include "ast/expressions/boolexpression.h"
#include "ast/types/type.h"
#include "ast/function.h"
#include "ast/variable.h"
#include "ast/renderstate.h"
#include "ast/samplerstate.h"
#include "ast/program.h"
#include "ast/structure.h"
#include "ast/variable.h"
#include <thread>

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
    this->validator = Alloc<Validator>();

    this->options.errorFormat = ErrorFormat::MSVC;
    this->options.warningsAsErrors = false;
    this->options.emitTimings = false;
    this->options.disallowImplicitConversion = false;
    this->options.disallowUninitializedConst = true;
    this->options.warnOnMissingRenderState = false;

    this->options.validate = true;
    this->options.optimize = false;

    this->branchReturns = false;
    this->defaultRenderState.name = "__DefaultRenderState";

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

    // Run again to resolve swizzle variables
    typeIt = DefaultTypes.begin();
    while (typeIt != DefaultTypes.end())
    {
        this->validator->ResolveTypeSwizzles(this, *typeIt);
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
        
    this->shaderSwitches[Program::__Resolved::ProgramEntryType::VertexShader].name = "gplIsVertexShader";
    this->shaderSwitches[Program::__Resolved::ProgramEntryType::HullShader].name = "gplIsHullShader";
    this->shaderSwitches[Program::__Resolved::ProgramEntryType::DomainShader].name = "gplIsDomainShader";
    this->shaderSwitches[Program::__Resolved::ProgramEntryType::GeometryShader].name = "gplIsGeometryShader";
    this->shaderSwitches[Program::__Resolved::ProgramEntryType::PixelShader].name = "gplIsPixelShader";
    this->shaderSwitches[Program::__Resolved::ProgramEntryType::TaskShader].name = "gplIsTaskShader";
    this->shaderSwitches[Program::__Resolved::ProgramEntryType::MeshShader].name = "gplIsMeshShader";
    this->shaderSwitches[Program::__Resolved::ProgramEntryType::RayGenerationShader].name = "gplIsRayGenerationShader";
    this->shaderSwitches[Program::__Resolved::ProgramEntryType::RayClosestHitShader].name = "gplIsRayClosestHitShader";
    this->shaderSwitches[Program::__Resolved::ProgramEntryType::RayAnyHitShader].name = "gplIsRayAnyHitShader";
    this->shaderSwitches[Program::__Resolved::ProgramEntryType::RayMissShader].name = "gplIsRayMissShader";
    this->shaderSwitches[Program::__Resolved::ProgramEntryType::RayIntersectionShader].name = "gplIsRayIntersectionShader";
    this->shaderSwitches[Program::__Resolved::ProgramEntryType::RayCallableShader].name = "gplIsRayCallableShader";

    for (uint32_t i = Program::__Resolved::ProgramEntryType::FirstShader; i <= Program::__Resolved::ProgramEntryType::LastShader; i++)
    {
        this->shaderSwitches[i].type = Type::FullType{ "b8" };
        Variable::__Resolved* res = Symbol::Resolved(&this->shaderSwitches[i]);
        res->usageBits.flags.isConst = true;
        res->builtin = true;
        this->shaderValueExpressions[i].value = false;
        this->shaderSwitches[i].valueExpression = &this->shaderValueExpressions[i];
        this->validator->ResolveVariable(this, &this->shaderSwitches[i]);
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
    
}

//------------------------------------------------------------------------------
/**
*/
void 
Compiler::Setup(const Compiler::Language& lang, const std::vector<std::string>& defines, Options options)
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
        SPIRVGenerator::SetupIntrinsics();
        this->target.supportsInlineSamplers = true;
        this->target.supportsPhysicalBufferAddresses = true;
        this->target.supportsPhysicalAddressing = true;
        this->target.supportsGlobalDeviceStorage = true;
        break;
    case Language::VULKAN_SPIRV:
        this->target.name = "VULKAN-SPIRV";
        this->target.generator = new SPIRVGenerator();
        this->target.supportsInlineSamplers = true;
        this->target.supportsPhysicalBufferAddresses = true;
        this->target.supportsPhysicalAddressing = false;
        this->target.supportsGlobalDeviceStorage = false;
        break;
    }

    this->options = options;

    // if we want other header generators in the future, add here
    this->headerGenerator = new HGenerator();
}

//------------------------------------------------------------------------------
/**
*/
Generator* 
Compiler::CreateGenerator(const Compiler::Language& lang, Options options)
{
    switch (lang)
    {
        case Language::GLSL_SPIRV:
            return Alloc<GLSLGenerator>(GLSLGenerator::VulkanFeatureSet);
            break;
        case Language::GLSL:
            return Alloc<GLSLGenerator>(GLSLGenerator::OpenGLFeatureSet);
            break;
        case Language::HLSL_SPIRV:
        case Language::HLSL:
            return Alloc<HLSLGenerator>();
            break;
        case Language::SPIRV:
        case Language::VULKAN_SPIRV:
            return Alloc<SPIRVGenerator>();
            break;
    }
    return nullptr;
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
    this->linkDefineCounter = 0;

    this->symbols = root->symbols;

    this->performanceTimer.Start();

    std::vector<Program*> programs;

    // resolves parser state and runs validation
    for (this->symbolIterator = 0; this->symbolIterator < this->symbols.size(); this->symbolIterator++)
    {
        ret &= this->validator->Resolve(this, this->symbols[this->symbolIterator]);
        if (this->hasErrors)
            break;

        if (this->symbols[this->symbolIterator]->symbolType == Symbol::SymbolType::ProgramType)
            programs.push_back((Program*)this->symbols[this->symbolIterator]);
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

    uint32_t numAvailableThreads = std::thread::hardware_concurrency();
    std::thread* threads = AllocStack<std::thread>(programs.size());
    bool* returnValues = AllocStack<bool>(programs.size());
    std::vector<Generator*> generators;

    // Run the code generation per thread
    for (size_t programIndex = 0; programIndex < programs.size(); programIndex++)
    {
        Generator* gen = CreateGenerator(this->lang, this->options);
        generators.push_back(gen);
        new (&threads[programIndex]) std::thread([this, returnValues, program = programs[programIndex], programIndex, gen, &symbols = this->symbols, writeFunction]()
        {
            returnValues[programIndex] = gen->Generate(this, program, symbols, writeFunction);
        });
    }

    for (size_t programIndex = 0; programIndex < programs.size(); programIndex++)
    {
        threads[programIndex].join();
        if (!returnValues[programIndex])
        {
            this->messages.insert(this->messages.begin(), generators[programIndex]->messages.begin(), generators[programIndex]->messages.end());
        }
        ret &= returnValues[programIndex];
    }
    DeallocStack(programs.size(), returnValues);
    DeallocStack(programs.size(), threads);

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
        std::vector<Symbol*> symbolsWithBuiltins = { &this->defaultRenderState };
        symbolsWithBuiltins.insert(symbolsWithBuiltins.end(), this->symbols.begin(), this->symbols.end());
        this->OutputBinary(symbolsWithBuiltins, binaryWriter, blob);

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
        this->messages.push_back(Format("Failed to open file '%s'", binaryWriter.GetPath().c_str()));
        this->hasErrors = true;
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
    else
    {
        this->messages.push_back(Format("Failed to open file '%s'", headerWriter.GetPath().c_str()));
        this->hasErrors = true;
    }

    return ret;
}

//------------------------------------------------------------------------------
/**
*/
void 
Compiler::Error(const std::string& msg, const std::string& file, int line, int column)
{
    static const char* ErrorStrings[] =
    {
        "%s(%d,%d): error: %s", // MSVC
        "%s:%d:%d: error: %s", // GCC
        "%s:%d:%d: error: %s", // Clang
    };
    static const char* InternalErrorStrings[] =
    {
        "internal error: %s", // MSVC
        "internal error: %s", // GCC
        "internal error: %s", // Clang
    };
    if (line == -1) 
    {
        std::string err = Format(InternalErrorStrings[(uint8_t)this->options.errorFormat], msg.c_str());
        this->messages.push_back(err);
    }
    else
    {
        std::string err = Format(ErrorStrings[(uint8_t)this->options.errorFormat], file.c_str(), line, column, msg.c_str());
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
    this->Error(Format("Unrecognized type '%s' for symbol '%s'", type.c_str(), sym->name.c_str()), sym);
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
            ValueUnion val;
            bool res = annot.value->EvalValue(val);
            assert(res == true);
            val.Store(i);
            output.data.i = i;
            output.type = Serialize::IntType;
            break;
        }
        case Symbol::FloatExpressionType:
        {
            float f;
            ValueUnion val;
            bool res = annot.value->EvalValue(val);
            assert(res == true);
            val.Store(f);
            output.data.f = f;
            output.type = Serialize::FloatType;
            break;
        }
        case Symbol::BoolExpressionType:
        {
            bool b;
            ValueUnion val;
            bool res = annot.value->EvalValue(val);
            assert(res == true);
            val.Store(b);
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
Compiler::OutputBinary(const std::vector<Symbol*>& symbols, BinWriter& writer, Serialize::DynamicLengthBlob& dynamicDataBlob)
{
    std::unordered_map<Symbol*, uint32_t> offsetMapping;
    for (uint32_t i = 0; i < symbols.size(); i++)
    {
        Symbol* symbol = symbols[i];
        if (symbol->symbolType == Symbol::ProgramType)
        {
            Program* program = static_cast<Program*>(symbol);
            Program::__Resolved* resolved = static_cast<Program::__Resolved*>(symbol->resolved);
            Serialize::Program output;
            output.nameLength = symbol->name.length();
            output.nameOffset = dynamicDataBlob.WriteString(symbol->name.c_str(), symbol->name.length());

            output.rayHitAttributeSize = 0;
            output.rayPayloadSize = 0;
            output.vsInputsLength = 0;
            output.patchSize = 0;

#define WRITE_BINARY(x, y)\
    if (resolved->usage.flags.has##x)\
    {\
        const std::vector<uint32_t>& binary = resolved->binaries[Program::__Resolved::##x];\
        output.y.binaryOffset = dynamicDataBlob.Write((const char*)binary.data(), binary.size() * sizeof(uint32_t));\
        output.y.binaryLength = binary.size() * sizeof(uint32_t);\
    }\
    else\
    {\
        output.y.binaryOffset = 0;\
        output.y.binaryLength = 0;\
    }

            WRITE_BINARY(VertexShader, vs)
            WRITE_BINARY(HullShader, hs)
            WRITE_BINARY(DomainShader, ds)
            WRITE_BINARY(GeometryShader, gs)
            WRITE_BINARY(PixelShader, ps)
            WRITE_BINARY(ComputeShader, cs)
            WRITE_BINARY(VertexShader, vs)
            WRITE_BINARY(TaskShader, ts)
            WRITE_BINARY(MeshShader, ms)
            WRITE_BINARY(RayGenerationShader, rgs)
            WRITE_BINARY(RayAnyHitShader, rahs)
            WRITE_BINARY(RayClosestHitShader, rchs)
            WRITE_BINARY(RayMissShader, rms)
            WRITE_BINARY(RayCallableShader, rcs)
            WRITE_BINARY(RayIntersectionShader, ris)

            if (resolved->usage.flags.hasVertexShader)
            {
                Function* vs = (Function*)resolved->mappings[Program::__Resolved::VertexShader];
                Function::__Resolved* vsRes = Symbol::Resolved(vs);
                output.vsInputsLength = 0;
                output.vsInputsOffset = dynamicDataBlob.iterator;
                for (const Variable* var : vs->parameters)
                {
                    const Variable::__Resolved* varRes = Symbol::Resolved(var);
                    if (varRes->storage == Storage::Input)
                    {
                        output.vsInputsLength++;
                        dynamicDataBlob.Write(varRes->inBinding);
                    }
                }
            }
            if (resolved->usage.flags.hasHullShader)
            {
                Function* hs = (Function*)resolved->mappings[Program::__Resolved::HullShader];
                Function::__Resolved* hsRes = Symbol::Resolved(hs);
                output.patchSize = hsRes->executionModifiers.maxOutputVertices;
            }
            if (resolved->usage.flags.hasRayGenerationShader)
            {
                Function* sh = (Function*)resolved->mappings[Program::__Resolved::RayGenerationShader];
                Function::__Resolved* shRes = Symbol::Resolved(sh);
                for (const Variable* var : sh->parameters)
                {
                    const Variable::__Resolved* varRes = Symbol::Resolved(var);
                    if (varRes->storage == Storage::RayPayload)
                        output.rayPayloadSize = max(output.rayPayloadSize, (uint16_t)varRes->byteSize);
                }
            }
            if (resolved->usage.flags.hasRayAnyHitShader)
            {
                Function* sh = (Function*)resolved->mappings[Program::__Resolved::RayAnyHitShader];
                Function::__Resolved* shRes = Symbol::Resolved(sh);
                for (const Variable* var : sh->parameters)
                {
                    const Variable::__Resolved* varRes = Symbol::Resolved(var);
                    if (varRes->storage == Storage::RayPayloadInput)
                        output.rayPayloadSize = max(output.rayPayloadSize, (uint16_t)varRes->byteSize);
                    else if (varRes->storage == Storage::RayHitAttribute)
                        output.rayHitAttributeSize = max(output.rayHitAttributeSize, (uint16_t)varRes->byteSize);
                }
            }
            if (resolved->usage.flags.hasRayClosestHitShader)
            {
                Function* sh = (Function*)resolved->mappings[Program::__Resolved::RayClosestHitShader];
                Function::__Resolved* shRes = Symbol::Resolved(sh);
                for (const Variable* var : sh->parameters)
                {
                    const Variable::__Resolved* varRes = Symbol::Resolved(var);
                    if (varRes->storage == Storage::RayPayload || varRes->storage == Storage::RayPayloadInput)
                        output.rayPayloadSize = max(output.rayPayloadSize, (uint16_t)varRes->byteSize);
                    else if (varRes->storage == Storage::RayHitAttribute)
                        output.rayHitAttributeSize = max(output.rayHitAttributeSize, (uint16_t)varRes->byteSize);
                }
            }
            if (resolved->usage.flags.hasRayMissShader)
            {
                Function* sh = (Function*)resolved->mappings[Program::__Resolved::RayMissShader];
                Function::__Resolved* shRes = Symbol::Resolved(sh);
                for (const Variable* var : sh->parameters)
                {
                    const Variable::__Resolved* varRes = Symbol::Resolved(var);
                    if (varRes->storage == Storage::RayPayload || varRes->storage == Storage::RayPayloadInput)
                        output.rayPayloadSize = max(output.rayPayloadSize, (uint16_t)varRes->byteSize);
                }
            }
            if (resolved->usage.flags.hasRayIntersectionShader)
            {
                Function* sh = (Function*)resolved->mappings[Program::__Resolved::RayIntersectionShader];
                Function::__Resolved* shRes = Symbol::Resolved(sh);
                for (const Variable* var : sh->parameters)
                {
                    const Variable::__Resolved* varRes = Symbol::Resolved(var);
                    if (varRes->storage == Storage::RayHitAttribute)
                        output.rayHitAttributeSize = max(output.rayHitAttributeSize, (uint16_t)varRes->byteSize);
                }
            }

            if (resolved->usage.flags.hasRenderState)
            {
                Symbol* renderState = resolved->mappings[Program::__Resolved::RenderState];
                output.renderStateNameLength = renderState->name.length();
                output.renderStateNameOffset = dynamicDataBlob.WriteString(renderState->name.c_str(), renderState->name.length());
            }
            else
            {
                output.renderStateNameLength = 0;
                output.renderStateNameOffset = 0;
            }

            output.annotationsOffset = dynamicDataBlob.Reserve<Serialize::Annotation>(program->annotations.size());
            output.annotationsCount = program->annotations.size();

            size_t offset = output.annotationsOffset;
            for (const Annotation& annot : program->annotations)
            {
                WriteAnnotation(this, annot, offset, dynamicDataBlob);
                offset += sizeof(Serialize::Annotation);
            }

            offset = writer.WriteType(output);
            offsetMapping[symbol] = offset;
        }
        else if (symbol->symbolType == Symbol::RenderStateType)
        {
            RenderState* rend = static_cast<RenderState*>(symbol);

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
            output.scissorEnabled = resolved->scissorEnabled;
            output.stencilEnabled = resolved->stencilEnabled;
            output.frontStencilState = resolved->frontStencilState;
            output.backStencilState = resolved->backStencilState;
            output.logicOpEnabled = resolved->logicOpEnabled;
            output.logicOp = resolved->logicOp;
            output.depthClampEnabled = resolved->depthClampEnabled;
            output.backStencilState = resolved->backStencilState;
            output.blendConstants[0] = resolved->blendConstants[0];
            output.blendConstants[1] = resolved->blendConstants[1];
            output.blendConstants[2] = resolved->blendConstants[2];
            output.blendConstants[3] = resolved->blendConstants[3];

            output.blendStatesOffset = dynamicDataBlob.iterator;
            output.blendStatesCount = 8;
            for (int i = 0; i < 8; i++)
            {
                const BlendState& state = resolved->blendStates[i];
                dynamicDataBlob.Write(state);
            }

            output.annotationsOffset = 0;
            output.annotationsCount = 0;

            size_t offset = writer.WriteType(output);
            offsetMapping[symbol] = offset;
        }
        else if (symbol->symbolType == Symbol::SamplerStateType)
        {
            SamplerState::__Resolved* resolved = static_cast<SamplerState::__Resolved*>(symbol->resolved);
            Serialize::SamplerState output;

            output.binding = resolved->binding;
            output.group = resolved->group;

            output.nameLength = symbol->name.length();
            output.nameOffset = dynamicDataBlob.WriteString(symbol->name.c_str(), symbol->name.length());
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

            size_t offset = writer.WriteType(output);
            offsetMapping[symbol] = offset;
        }
        else if (symbol->symbolType == Symbol::StructureType)
        {
            Structure* structure = static_cast<Structure*>(symbol);
            Structure::__Resolved* resolved = static_cast<Structure::__Resolved*>(symbol->resolved);
            Serialize::Structure output;
            output.isUniform = false;
            output.isMutable = false;
            output.nameLength = symbol->name.length();
            output.nameOffset = dynamicDataBlob.WriteString(symbol->name.c_str(), symbol->name.length());
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
                    varOutput.nameOffset = dynamicDataBlob.WriteString(var->name.c_str(), var->name.length());
                    varOutput.byteSize = resolved->byteSize;
                    varOutput.structureOffset = resolved->structureOffset;
                    varOutput.arraySizesCount = resolved->type.modifierValues.size();
                    varOutput.arraySizesOffset = dynamicDataBlob.Write(resolved->type.modifierValues.data(), resolved->type.modifierValues.size());

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

            offset = writer.WriteType(output);
            offsetMapping[symbol] = offset;
        }
        else if (symbol->symbolType == Symbol::VariableType)
        {
            Variable* var = static_cast<Variable*>(symbol);
            Variable::__Resolved* resolved = static_cast<Variable::__Resolved*>(symbol->resolved);
            Serialize::Variable output;
            output.binding = resolved->binding;
            output.group = resolved->group;
            output.visibility.bits = resolved->visibilityBits.bits;
            output.nameLength = symbol->name.length();
            output.nameOffset = dynamicDataBlob.WriteString(symbol->name.c_str(), symbol->name.length());
            output.byteSize = resolved->byteSize;
            output.structureOffset = resolved->structureOffset;
            output.arraySizesCount = 0;
            output.arraySizesOffset = dynamicDataBlob.iterator;
            for (size_t i = 0; i < resolved->type.modifiers.size(); i++)
            {
                if (resolved->type.modifiers[i] == Type::FullType::Modifier::Array && resolved->type.modifierValues[i] != nullptr)
                {
                    output.arraySizesCount++;
                    ValueUnion size;
                    resolved->type.modifierValues[i]->EvalValue(size);
                    dynamicDataBlob.Write(size.ui);
                }
            }
            output.annotationsCount = var->annotations.size();
            output.annotationsOffset = dynamicDataBlob.Reserve<Serialize::Annotation>(var->annotations.size());
            if (resolved->usageBits.flags.isEntryPointParameter)
                output.bindingScope = GPULang::BindingScope::VertexInput;
            else if (resolved->storage == Storage::Global)
                output.bindingScope = GPULang::BindingScope::Constant;
            else
                output.bindingScope = GPULang::BindingScope::Resource;

            if (resolved->type.IsMutable())
            {
                if (resolved->typeSymbol->category == Type::Category::UserTypeCategory)
                    output.bindingType = GPULang::BindingType::MutableBuffer;
                else if (resolved->typeSymbol->category == Type::Category::TextureCategory)
                    output.bindingType = GPULang::BindingType::MutableImage;
            }
            else
            {
                if (resolved->typeSymbol->category == Type::Category::UserTypeCategory)
                    output.bindingType = GPULang::BindingType::Buffer;
                else if (resolved->typeSymbol->category == Type::Category::TextureCategory)
                    if (resolved->type.sampled)
                        output.bindingType = GPULang::BindingType::SampledImage;
                    else
                        output.bindingType = GPULang::BindingType::Image;
                else if (resolved->typeSymbol->category == Type::Category::SamplerCategory)
                    output.bindingType = GPULang::BindingType::Sampler;
                else if (resolved->typeSymbol->category == Type::Category::ScalarCategory)
                    output.bindingType = GPULang::BindingType::LinkDefined;
                else if (resolved->typeSymbol->category == Type::Category::PixelCacheCategory)
                    output.bindingType = GPULang::BindingType::PixelCache;
                else if (resolved->typeSymbol->category == Type::Category::AccelerationStructureCategory)
                    output.bindingType = GPULang::BindingType::AccelerationStructure;
            }

            output.structTypeNameLength = 0;
            output.structTypeNameOffset = 0;
            if (output.binding == GPULang::BindingType::Buffer || output.bindingType == GPULang::BindingType::MutableBuffer)
            {
                output.structTypeNameLength = resolved->type.name.length();
                output.structTypeNameOffset = dynamicDataBlob.WriteString(resolved->type.name.c_str(), resolved->type.name.length());
            }
            size_t offset = output.annotationsOffset;
            for (const Annotation& annot : var->annotations)
            {
                WriteAnnotation(this, annot, offset, dynamicDataBlob);
                offset += sizeof(Serialize::Annotation);
            }

            offset = writer.WriteType(output);
        }
    }
}

} // namespace GPULang
