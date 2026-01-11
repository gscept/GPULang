//------------------------------------------------------------------------------
//  compiler.cc
//  (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "compiler.h"
#include "ast/program.h"
#include "validators/validator.h"
#include "generators/hlslgenerator.h"
#include "generators/spirvgenerator.h"
#include "generators/reflection/hgenerator.h"
#include "util.h"
#include "ast/expressions/boolexpression.h"
#include "ast/statements/ifstatement.h"
#include "ast/statements/scopestatement.h"
#include "ast/types/type.h"
#include "ast/function.h"
#include "ast/variable.h"
#include "ast/generate.h"
#include "ast/renderstate.h"
#include "ast/samplerstate.h"
#include "ast/program.h"
#include "ast/structure.h"
#include "ast/variable.h"
#include "ast/preprocessor.h"
#include "generated/types.h"
#include "generated/intrinsics.h"
#include "gpulangcompiler.h"
#include "thread.h"
#include <thread>



namespace GPULang
{

thread_local BoolExpression ShaderValueExpressions[ProgramInstance::__Resolved::EntryType::NumProgramEntries];

//------------------------------------------------------------------------------
/**
*/
Compiler::Compiler() 
    : hasErrors(false)
    , ignoreReservedWords(false)
{
    this->validator = Alloc<Validator>();

    this->generationState.active = false;
    this->generationState.branchActive = false;
    this->generationState.owner = nullptr;
    
    this->options.errorFormat = ErrorFormat::MSVC;
    this->options.warningsAsErrors = false;
    this->options.emitTimings = false;
    this->options.disallowImplicitConversion = false;
    this->options.disallowUninitializedConst = true;
    this->options.warnOnMissingRenderState = false;

    this->options.validate = true;
    this->options.optimize = false;
    this->options.emitTimings = true;
    this->currentState.prog = nullptr;
    this->currentState.function = nullptr;

    this->branchReturns = false;
    this->defaultRenderState.name = "__DefaultRenderState";
}

//------------------------------------------------------------------------------
/**
*/
Compiler::~Compiler()
{
    //this->validator->~Validator();
}


//------------------------------------------------------------------------------
/**
*/
void
SetupTargetLanguage(const Compiler::Language& lang, Compiler::Target& target)
{
    switch (lang)
    {
    case Compiler::Language::GLSL:
        target.name = "GLSL"_c;
        break;
    case Compiler::Language::HLSL:
        target.name = "HLSL"_c;
        break;
    case Compiler::Language::DXIL:
        target.name = "DXIL"_c;
        break;
    case Compiler::Language::SPIRV:
        target.name = "SPIRV"_c;
        target.supportsInlineSamplers = true;
        target.supportsPhysicalBufferAddresses = true;
        target.supportsPhysicalAddressing = true;
        target.supportsGlobalDeviceStorage = true;
        break;
    case Compiler::Language::VULKAN_SPIRV:
        target.name = "Vulkan"_c;
        target.supportsInlineSamplers = true;
        target.supportsPhysicalBufferAddresses = true;
        target.supportsPhysicalAddressing = false;
        target.supportsGlobalDeviceStorage = false;
        break;
    case Compiler::Language::WEBGPU:
        target.name = "WebGPU"_c;
        break;
    case Compiler::Language::METAL_IR:
    case Compiler::Language::METAL:
        target.name = "Metal"_c;
        break;
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
Compiler::Setup(const Compiler::Language& lang, Options options)
{
    this->lang = lang;
    this->options = options;
    SetupTargetLanguage(lang, this->target);

    // if we want other header generators in the future, add here
    this->headerGenerator = Alloc<HGenerator>();

    this->performanceTimer.Start();

    // Allocate main scopes
    this->intrinsicScope = Alloc<Scope>();
    this->intrinsicScope->type = Scope::ScopeType::Global;
    this->mainScope = Alloc<Scope>();
    this->mainScope->type = Scope::ScopeType::Global;

    this->BeginStaticSymbolSetup();

    // push global scope for all the builtins
    this->PushScope(this->intrinsicScope);
    
    this->performanceTimer.Start();
    SetupIntrinsics();
    this->performanceTimer.Stop();
    if (this->options.emitTimings)
        this->performanceTimer.Print("Static Intrinsic Setup");

    this->intrinsicScope->symbolLookup = DefaultIntrinsics;

    this->ignoreReservedWords = true;
    for (uint32_t i = ProgramInstance::__Resolved::EntryType::FirstShader; i <= ProgramInstance::__Resolved::EntryType::LastShader; i++)
    {
        ShaderValueExpressions[i].value = false;
        TransientString str = TransientString("gplIs", ProgramInstance::__Resolved::EntryTypeToString(ProgramInstance::__Resolved::EntryType(i)));
        this->intrinsicScope->symbolLookup.Insert(HashString(str), &ShaderValueExpressions[i]);
    }

    this->ignoreReservedWords = false;
    this->EndStaticSymbolSetup();

    this->performanceTimer.Start();

    switch (this->lang)
    {
    case Language::GLSL:
        break;
    case Language::HLSL:
        break;
    case Language::SPIRV:
    case Language::VULKAN_SPIRV:
        SPIRVGenerator::SetupIntrinsics();
        break;
    }

    this->performanceTimer.Stop();
    if (this->options.emitTimings)
        this->performanceTimer.Print("Static setup target intrinsics");
}

//------------------------------------------------------------------------------
/**
*/
void
Compiler::SetupServer(const Compiler::Language& lang,Options options)
{
    this->options = options;
    this->lang = lang;
    SetupTargetLanguage(lang, this->target);
    
    // Allocate main scopes
    this->intrinsicScope = Alloc<Scope>();
    this->intrinsicScope->type = Scope::ScopeType::Global;
    this->mainScope = Alloc<Scope>();
    this->mainScope->type = Scope::ScopeType::Global;
    
    this->BeginStaticSymbolSetup();
    
    // push global scope for all the builtins
    this->PushScope(this->intrinsicScope);
    
    this->performanceTimer.Start();
    SetupIntrinsics();
    this->performanceTimer.Stop();
    if (this->options.emitTimings)
        this->performanceTimer.Print("Static Intrinsic Setup");
    
    this->intrinsicScope->symbolLookup = DefaultIntrinsics;
    
    this->ignoreReservedWords = true;
    
    for (uint32_t i = ProgramInstance::__Resolved::EntryType::FirstShader; i <= ProgramInstance::__Resolved::EntryType::LastShader; i++)
    {
        ShaderValueExpressions[i].value = false;
        TransientString str = TransientString("gplIs", ProgramInstance::__Resolved::EntryTypeToString(ProgramInstance::__Resolved::EntryType(i)));
        this->intrinsicScope->symbolLookup.Insert(HashString(str), &ShaderValueExpressions[i]);

    }
    
    this->ignoreReservedWords = false;
    this->EndStaticSymbolSetup();
}

//------------------------------------------------------------------------------
/**
*/
Generator* 
Compiler::CreateGenerator(const Compiler::Language& lang, Options options)
{
    switch (lang)
    {
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
void 
Compiler::BeginStaticSymbolSetup()
{
    this->staticSymbolSetup = true;
}

//------------------------------------------------------------------------------
/**
*/
void 
Compiler::EndStaticSymbolSetup()
{
    this->staticSymbolSetup = false;
}

//------------------------------------------------------------------------------
/**
*/
bool 
Compiler::AddSymbol(const FixedString& name, Symbol* symbol, bool allowDuplicate, bool bypass)
{
    Scope* scope = this->scopes.back();
    if (bypass)
        scope = *(this->scopes.end() - 1);
    PinnedMap<HashString, Symbol*>* lookup;
    PinnedArray<Symbol*>* symbols;
    if (scope->type == Scope::ScopeType::Type)
    {
        Type* type = static_cast<Type*>(scope->owningSymbol);
        lookup = &type->scope.symbolLookup;
        symbols = &type->symbols;
    }
    else
    {
        lookup = &scope->symbolLookup;
        symbols = &scope->symbols;
    }

    if (!allowDuplicate && !this->staticSymbolSetup)
    {
        auto it = lookup->Find(HashString(name));
        if (it != lookup->end())
        {
            Symbol* prevSymbol = it->second;
            this->Error(Format("Symbol %s redefinition, previous definition at %s(%d)", name.c_str(), prevSymbol->location.file.c_str(), prevSymbol->location.line), symbol);
            return false;
        }
    }
    lookup->Insert(name, symbol);
    // Only add to symbols if scope type isn't a type, because they already have the symbols setup
    if (scope->type != Scope::ScopeType::Type)
        symbols->Append(symbol);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
Symbol* 
Compiler::GetSymbol(const FixedString& name) const
{
    auto scopeIter = this->scopes.rbegin();
    do
    {
        auto scope = scopeIter.get();
        PinnedMap<HashString, Symbol*>* map;
        if (scope->type == Scope::ScopeType::Type)
        {
            Type* type = static_cast<Type*>(scope->owningSymbol);
            map = &type->scope.symbolLookup;
        }
        else
        {
            map = &scope->symbolLookup;
        }
        auto it = map->Find(HashString(name));
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
TransientArray<Symbol*>
Compiler::GetSymbols(const FixedString& name) const
{
    TransientArray<Symbol*> ret(256);
    auto scopeIter = this->scopes.rbegin();
    do
    {
        auto scope = scopeIter.get();
        PinnedMap<HashString, Symbol*>* map;
        if (scope->type == Scope::ScopeType::Type)
        {
            Type* type = static_cast<Type*>(scope->owningSymbol);
            map = &type->scope.symbolLookup;
        }
        else
        {
            map = &scope->symbolLookup;
        }

        auto range = map->FindRange(HashString(name));
        for (auto it = range.first; it != range.second; it++)
            ret.Append((*it).second);
        scopeIter++;
    } while (scopeIter != this->scopes.rend());
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
bool
Compiler::AddSymbol(const TransientString& name, Symbol* symbol, bool allowDuplicate, bool bypass)
{
    Scope* scope = this->scopes.back();
    if (bypass)
        scope = *(this->scopes.end() - 1);
    PinnedMap<HashString, Symbol*>* lookup;
    PinnedArray<Symbol*>* symbols;
    if (scope->type == Scope::ScopeType::Type)
    {
        Type* type = static_cast<Type*>(scope->owningSymbol);
        lookup = &type->scope.symbolLookup;
        symbols = &type->symbols;
    }
    else
    {
        lookup = &scope->symbolLookup;
        symbols = &scope->symbols;
    }

    if (!allowDuplicate && !this->staticSymbolSetup)
    {
        auto it = lookup->Find(HashString(name));
        if (it != lookup->end())
        {
            Symbol* prevSymbol = it->second;
            this->Error(Format("Symbol %s redefinition, previous definition at %s(%d)", name.c_str(), prevSymbol->location.file.c_str(), prevSymbol->location.line), symbol);
            return false;
        }
    }
    lookup->Insert(HashString(name), symbol);
    // Only add to symbols if scope type isn't a type, because they already have the symbols setup
    if (scope->type != Scope::ScopeType::Type)
        symbols->Append(symbol);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
Compiler::AddGlobalSymbol(const FixedString& name, Symbol* symbol, bool allowDuplicate)
{
    Scope* scope = this->scopes.front();
    PinnedMap<HashString, Symbol*>* lookup = &scope->symbolLookup;
    PinnedArray<Symbol*>* symbols = &scope->symbols;
    assert(scope->type == Scope::ScopeType::Global);

    if (!allowDuplicate && !this->staticSymbolSetup)
    {
        auto it = lookup->Find(HashString(name));
        if (it != lookup->end())
        {
            Symbol* prevSymbol = it->second;
            this->Error(Format("Symbol %s redefinition, previous definition at %s(%d)", name.c_str(), prevSymbol->location.file.c_str(), prevSymbol->location.line), symbol);
            return false;
        }
    }
    lookup->Insert(HashString(name), symbol);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
Compiler::AddGlobalSymbol(const TransientString& name, Symbol* symbol, bool allowDuplicate)
{
    Scope* scope = this->scopes.front();
    PinnedMap<HashString, Symbol*>* lookup = &scope->symbolLookup;
    PinnedArray<Symbol*>* symbols = &scope->symbols;
    assert(scope->type == Scope::ScopeType::Global);

    if (!allowDuplicate && !this->staticSymbolSetup)
    {
        auto it = lookup->Find(HashString(name));
        if (it != lookup->end())
        {
            Symbol* prevSymbol = it->second;
            this->Error(Format("Symbol %s redefinition, previous definition at %s(%d)", name.c_str(), prevSymbol->location.file.c_str(), prevSymbol->location.line), symbol);
            return false;
        }
    }
    lookup->Insert(HashString(name), symbol);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
Symbol*
Compiler::GetSymbol(const TransientString& name) const
{
    auto scopeIter = this->scopes.rbegin();
    do
    {
        auto scope = scopeIter.get();
        PinnedMap<HashString, Symbol*>* map;
        if (scope->type == Scope::ScopeType::Type)
        {
            Type* type = static_cast<Type*>(scope->owningSymbol);
            map = &type->scope.symbolLookup;
        }
        else
        {
            map = &scope->symbolLookup;
        }
        auto it = map->Find(HashString(name));
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
TransientArray<Symbol*>
Compiler::GetSymbols(const TransientString& name) const
{
    TransientArray<Symbol*> ret(256);
    auto scopeIter = this->scopes.rbegin();
    do
    {
        auto scope = scopeIter.get();
        PinnedMap<HashString, Symbol*>* map;
        if (scope->type == Scope::ScopeType::Type)
        {
            Type* type = static_cast<Type*>(scope->owningSymbol);
            map = &type->scope.symbolLookup;
        }
        else
        {
            map = &scope->symbolLookup;
        }

        auto range = map->FindRange(HashString(name));
        for (auto it = range.first; it != range.second; it++)
            ret.Append((*it).second);
        scopeIter++;
    } while (scopeIter != this->scopes.rend());
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
void Compiler::PushScope(Scope* scope)
{
    this->scopes.Append(scope);
}

//------------------------------------------------------------------------------
/**
*/
void 
Compiler::PopScope()
{
    Scope* currentScope = this->scopes.back();
    this->scopes.size--;
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
        lastSymbol = it.get()->owningSymbol;
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
        if (it.get()->owningSymbol != nullptr && it.get()->owningSymbol->symbolType == type)
            return it.get()->owningSymbol;
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
    // push a new scope for all the parsed symbols
    this->PushScope(this->mainScope);

    bool ret = true;
    this->linkDefineCounter = 0;
    this->validator->defaultGroup = this->options.defaultGroupBinding;

    this->symbols = root->symbols;
    this->symbols.Prepend(&this->defaultRenderState);

    this->performanceTimer.Start();

    std::vector<ProgramInstance*> programs;

    // resolves parser state and runs validation
    for (this->symbolIterator = 0; this->symbolIterator < this->symbols.size; this->symbolIterator++)
    {
        ret &= this->validator->Resolve(this, this->symbols.data[this->symbolIterator]);
        if (this->hasErrors)
            break;

        if (this->symbols.data[this->symbolIterator]->symbolType == Symbol::SymbolType::ProgramInstanceType)
            programs.push_back((ProgramInstance*)this->symbols.data[this->symbolIterator]);
    }

    this->performanceTimer.Stop();

    // if failed, don't proceed to next step
    if (!ret || this->hasErrors)
        return false;

    if (this->options.emitTimings)
        this->performanceTimer.Print("Type checking");

    this->performanceTimer.Start();

    uint32_t numAvailableThreads = std::thread::hardware_concurrency();
    size_t threadByteSize, returnValueByteSize;
    TransientArray<std::thread> threads(programs.size());
    TransientArray<bool> returnValues(programs.size());
    TransientArray<Generator*> generators(programs.size());
    TransientArray<Allocator> allocators(programs.size());

    TransientString debugOutputPath = StripExtension(binaryWriter.GetPath().c_str());

    // Run the code generation per thread
    for (size_t programIndex = 0; programIndex < programs.size(); programIndex++)
    {
        auto program = programs[programIndex];
        Generator* gen = CreateGenerator(this->lang, this->options);
        allocators[programIndex] = CreateAllocator();
        InitAllocator(&allocators[programIndex]);
        generators.Append(gen);

        std::function<void(const std::string&, const std::string&)> writeFunction = nullptr;
        if (this->options.debugInfo)
        {
            writeFunction = [this, &debugOutputPath](const std::string& name, const std::string& code)
            {
                TextWriter outputStream;
                outputStream.SetPath(TStr(debugOutputPath, "_debug", "/", name.c_str(), ".txt").c_str());
                if (outputStream.Open())
                {
                    outputStream.WriteString(code);
                    outputStream.Close();
                }
            };
        }
        new (&threads[programIndex]) std::thread([this, values = returnValues.begin(), program = programs[programIndex], allocator = &allocators[programIndex], programIndex, gen, &symbols = this->symbols, writeFunction]()
        {
            GPULang::CurrentAllocator = allocator;
            GPULang::StringAllocator = allocator;
            values[programIndex] = gen->Generate(this, program, symbols, writeFunction);
        });
    }

    for (size_t programIndex = 0; programIndex < programs.size(); programIndex++)
    {
        threads[programIndex].join();
        if (!returnValues[programIndex])
        {
            // Copy messages from each generator, since they use their own allocator
            for (const auto& msg : generators[programIndex]->messages)
            {
                this->messages.Append(FixedString(TStr(msg)));
            }
        }
        DestroyAllocator(&allocators[programIndex]);
        ret &= returnValues[programIndex];
    }

    this->performanceTimer.Stop();

    if (this->options.emitTimings)
        this->performanceTimer.Print("Code generation");

    // if failed, don't proceed to next step
    if (!ret || this->hasErrors)
        return false;

    // Output binary if file can be opened and there are programs to dump
    if (programs.size() > 0)
    {
        if (binaryWriter.Open())
        {
            this->performanceTimer.Start();

            // write magic
            binaryWriter.WriteInt('AFX3'); // Homage to AnyFX

            // run binary output step
            Serialize::DynamicLengthBlob blob;
            this->OutputBinary(this->symbols, binaryWriter, blob);

            // output dynamic blob
            binaryWriter.WriteBytes(&blob.data[0], blob.iterator);

            // write size of dynamic blob at the end
            binaryWriter.WriteUInt(blob.iterator);
            binaryWriter.Close();

            this->performanceTimer.Stop();

            if (this->options.emitTimings)
                this->performanceTimer.Print("Reflection serialization");
        }
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
bool
Compiler::Validate(Effect* root)
{
    bool ret = true;
    this->linkDefineCounter = 0;
    this->validator->defaultGroup = this->options.defaultGroupBinding;

    this->symbols = root->symbols;
    this->symbols.Prepend(&this->defaultRenderState);

    this->performanceTimer.Start();

    std::vector<ProgramInstance*> programs;
    
    // push a new scope for all the parsed symbols
    this->PushScope(this->mainScope);

    // resolves parser state and runs validation
    Symbol* prevPP = nullptr;
    for (this->symbolIterator = 0; this->symbolIterator < this->symbols.size; this->symbolIterator++)
    {
        Symbol* sym = this->symbols.data[this->symbolIterator];
        if (sym == nullptr)
        {
            this->symbols.size = this->symbolIterator-2;
            return false;
        }
        ret &= this->validator->Resolve(this, sym);
        if (this->hasErrors)
            break;

        if (sym->symbolType == Symbol::SymbolType::ProgramInstanceType)
            programs.push_back((ProgramInstance*)sym);
    }

    this->performanceTimer.Stop();

    // if failed, don't proceed to next step
    if (!ret || this->hasErrors)
        return false;

    if (this->options.emitTimings)
        this->performanceTimer.Print("Type checking");
    
    if (this->options.languageServerGeneration)
    {
        // setup potential debug output stream
        std::function<void(const std::string&, const std::string&)> writeFunction = nullptr;
        this->performanceTimer.Start();
        
        uint32_t numAvailableThreads = std::thread::hardware_concurrency();
        size_t threadByteSize, returnValueByteSize;
        TransientArray<std::thread> threads(programs.size());
        TransientArray<bool> returnValues(programs.size());
        TransientArray<Generator*> generators(programs.size());
        
        // Run the code generation per thread
        for (size_t programIndex = 0; programIndex < programs.size(); programIndex++)
        {
            auto program = programs[programIndex];
            Generator* gen = CreateGenerator(this->lang, this->options);
            generators.Append(gen);
            new (&threads[programIndex]) std::thread([this, values = returnValues.begin(), program, programIndex, gen, &symbols = this->symbols, writeFunction]()
            {
                values[programIndex] = gen->Generate(this, program, symbols, writeFunction);
            });
        }
        
        for (size_t programIndex = 0; programIndex < programs.size(); programIndex++)
        {
            threads[programIndex].join();
            if (!returnValues[programIndex])
            {
                this->messages.Append(generators[programIndex]->messages);
            }
            ret &= returnValues[programIndex];
        }
        
        this->performanceTimer.Stop();
        
        if (this->options.emitTimings)
            this->performanceTimer.Print("Code generation");
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
Compiler::Error(const TransientString& msg, const FixedString& file, int line, int column, int length)
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
        TransientString err;
        err.Format(InternalErrorStrings[(uint8_t)this->options.errorFormat], msg.c_str());
        this->messages.Append(FixedString(err));

        this->diagnostics.Append(GPULangDiagnostic{ .error = FixedString(msg), .file = file, .line = line, .column = column, .length = length });
    }
    else
    {
        TransientString err;
        err.Format(ErrorStrings[(uint8_t)this->options.errorFormat], file.c_str(), line+1, column, msg.c_str());
        this->messages.Append(err.c_str());
        this->diagnostics.Append(GPULangDiagnostic{ .error = FixedString(msg), .file = file, .line = line+1, .column = column, .length = length });
    }
    this->hasErrors = true;
}

//------------------------------------------------------------------------------
/**
*/
void
Compiler::Error(TransientString msg, const Symbol* sym)
{
    if (this->currentState.prog != nullptr)
    {
        msg.Append(TStr(" in program '", this->currentState.prog->name, "'"));
    }
    this->Error(msg, sym->location.file, sym->location.line, sym->location.start, sym->location.end - sym->location.start);
}

//------------------------------------------------------------------------------
/**
*/
void
Compiler::Warning(const TransientString& msg, const FixedString& file, int line, int column)
{
    this->messages.Append(TransientString(file, "(", line, ")", " : warning: ", msg).c_str());
    if (this->options.warningsAsErrors)
        this->hasErrors = true;
}

//------------------------------------------------------------------------------
/**
*/
void
Compiler::Warning(const TransientString& msg, const Symbol* sym)
{
    this->Warning(msg, sym->location.file, sym->location.line, sym->location.start);
}

//------------------------------------------------------------------------------
/**
*/
void
Compiler::GeneratorError(const TransientString& msg)
{
    this->messages.Append(FixedString(msg));
}

//------------------------------------------------------------------------------
/**
*/
void 
Compiler::UnrecognizedTypeError(const TransientString& type, Symbol* sym)
{
    if (type == UNDEFINED_TYPE)
        this->Error(Format("Type not defined or can't inferred for '%s'", sym->name.c_str()), sym);
    else
        this->Error(Format("Unrecognized type '%s' for '%s'", type.c_str(), sym->name.c_str()), sym);
}

//------------------------------------------------------------------------------
/**
*/
void 
Compiler::UnrecognizedSymbolError(const TransientString& symbol, Symbol* sym)
{
    this->Error(Format("Unrecognized symbol '%s'", symbol.c_str()), sym);
}

//------------------------------------------------------------------------------
/**
*/
void 
Compiler::ReservedPrefixError(const FixedString& name, const std::string& word, Symbol* sym)
{
    this->Error(Format("Reserved word '%s'. No variables or functions are allowed to be prefixed with '%s'", name.c_str(), word.c_str()), sym);
}

//------------------------------------------------------------------------------
/**
*/
void
WriteAnnotation(Compiler* compiler, const Annotation* annot, size_t offset, Serialize::DynamicLengthBlob& dynamicDataBlob)
{
    Serialize::Annotation output;
    output.nameLength = annot->name.len;
    output.nameOffset = dynamicDataBlob.WriteString(annot->name.c_str(), annot->name.len);
    if (annot->value != nullptr)
    {
        switch (annot->value->symbolType)
        {
        case Symbol::StringExpressionType:
        {
            TransientString str = annot->value->EvalString();
            output.data.s.stringOffset = dynamicDataBlob.WriteString(str.c_str(), str.size);
            output.data.s.stringLength = str.size;
            output.type = Serialize::StringType;
            break;
        }
        case Symbol::IntExpressionType:
        {
            unsigned int i;
            ValueUnion val;
            bool res = annot->value->EvalValue(val);
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
            bool res = annot->value->EvalValue(val);
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
            bool res = annot->value->EvalValue(val);
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
Compiler::OutputBinary(const PinnedArray<Symbol*>& symbols, BinWriter& writer, Serialize::DynamicLengthBlob& dynamicDataBlob)
{
    for (uint32_t i = 0; i < symbols.size; i++)
    {
        Symbol* symbol = symbols[i];
        this->OutputSymbolToBinary(symbol, writer, dynamicDataBlob);
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
Compiler::OutputSymbolToBinary(Symbol* symbol, BinWriter& writer, Serialize::DynamicLengthBlob& dynamicDataBlob)
{
    if (symbol->symbolType == Symbol::GenerateType)
    {
        Generate* generate = static_cast<Generate*>(symbol);
        this->OutputBinary(generate->symbols, writer, dynamicDataBlob);
    }
    else if (symbol->symbolType == Symbol::IfStatementType)
    {
        IfStatement* cond = static_cast<IfStatement*>(symbol);
        ValueUnion val;
        if (cond->condition->EvalValue(val))
        {
            if (val.b[0])
            {
                ScopeStatement* scope = static_cast<ScopeStatement*>(cond->ifStatement);
                this->OutputBinary(scope->symbols, writer, dynamicDataBlob);
            }
            else if (cond->elseStatement != nullptr)
            {
                this->OutputSymbolToBinary(cond->elseStatement, writer, dynamicDataBlob);
            }
        }
    }
    else if (symbol->symbolType == Symbol::ScopeStatementType)
    {
        ScopeStatement* scope = static_cast<ScopeStatement*>(symbol);
        this->OutputBinary(scope->symbols, writer, dynamicDataBlob);
    }
    else if (symbol->symbolType == Symbol::ProgramInstanceType)
    {
        ProgramInstance* program = static_cast<ProgramInstance*>(symbol);
        ProgramInstance::__Resolved* resolved = static_cast<ProgramInstance::__Resolved*>(symbol->resolved);
        Serialize::Program output;
        output.nameLength = symbol->name.len;
        output.nameOffset = dynamicDataBlob.WriteString(symbol->name.c_str(), symbol->name.len);

        output.rayHitAttributeSize = 0;
        output.rayPayloadSize = 0;
        output.vsInputsLength = 0;
        output.patchSize = 0;

#define WRITE_BINARY(x, y)\
    if (resolved->usage.flags.has##x)\
    {\
        const std::vector<uint32_t>& binary = resolved->binaries[ProgramInstance::__Resolved::x];\
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
            Function* vs = (Function*)resolved->mappings[ProgramInstance::__Resolved::VertexShader];
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
            Function* hs = (Function*)resolved->mappings[ProgramInstance::__Resolved::HullShader];
            Function::__Resolved* hsRes = Symbol::Resolved(hs);
            output.patchSize = hsRes->executionModifiers.patchSize;
        }
        if (resolved->usage.flags.hasRayGenerationShader)
        {
            Function* sh = (Function*)resolved->mappings[ProgramInstance::__Resolved::RayGenerationShader];
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
            Function* sh = (Function*)resolved->mappings[ProgramInstance::__Resolved::RayAnyHitShader];
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
            Function* sh = (Function*)resolved->mappings[ProgramInstance::__Resolved::RayClosestHitShader];
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
            Function* sh = (Function*)resolved->mappings[ProgramInstance::__Resolved::RayMissShader];
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
            Function* sh = (Function*)resolved->mappings[ProgramInstance::__Resolved::RayIntersectionShader];
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
            Symbol* renderState = resolved->mappings[ProgramInstance::__Resolved::RenderState];
            output.renderStateNameLength = renderState->name.len;
            output.renderStateNameOffset = dynamicDataBlob.WriteString(renderState->name.c_str(), renderState->name.len);
        }
        else
        {
            output.renderStateNameLength = 0;
            output.renderStateNameOffset = 0;
        }

        output.annotationsOffset = dynamicDataBlob.Reserve<Serialize::Annotation>(program->annotations.size);
        output.annotationsCount = program->annotations.size;

        size_t offset = output.annotationsOffset;
        for (const Annotation* annot : program->annotations)
        {
            WriteAnnotation(this, annot, offset, dynamicDataBlob);
            offset += sizeof(Serialize::Annotation);
        }

        writer.WriteType(output);
        writer.IncrementDecodeSize(sizeof(Deserialize::Annotation) * program->annotations.size);
        writer.IncrementDecodeSize(sizeof(Deserialize::Program));
    }
    else if (symbol->symbolType == Symbol::RenderStateInstanceType)
    {
        RenderStateInstance* rend = static_cast<RenderStateInstance*>(symbol);

        auto resolved = Symbol::Resolved(rend);
        Serialize::RenderState output;
        output.nameLength = symbol->name.len;
        output.nameOffset = dynamicDataBlob.WriteString(symbol->name.c_str(), symbol->name.len);
        output.depthClampEnabled = resolved->depthClampEnabled;
        output.noPixels = resolved->noPixels;
        output.rasterizationMode = resolved->rasterizationMode;
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

        output.samples = resolved->samples;
        output.sampleShadingEnabled = resolved->sampleShadingEnabled;
        output.minSampleShading = resolved->minSampleShading;
        output.alphaToCoverageEnabled = resolved->alphaToCoverageEnabled;
        output.alphaToOneEnabled = resolved->alphaToOneEnabled;

        output.blendStatesOffset = dynamicDataBlob.iterator;
        output.blendStatesCount = 8;
        for (int i = 0; i < 8; i++)
        {
            const Serialization::BlendState& state = resolved->blendStates[i];
            dynamicDataBlob.Write(state);
        }

        output.annotationsOffset = 0;
        output.annotationsCount = 0;

        writer.WriteType(output);
        writer.IncrementDecodeSize(sizeof(Deserialize::RenderState));
    } 
    else if (symbol->symbolType == Symbol::SamplerStateInstanceType)
    {
        SamplerStateInstance* sampler = static_cast<SamplerStateInstance*>(symbol);
        SamplerStateInstance::__Resolved* resolved = Symbol::Resolved(sampler);
        Serialize::SamplerState output;

        output.binding = resolved->binding;
        output.group = resolved->group;
        output.visibility.bits = resolved->visibilityBits.bits;

        output.nameLength = symbol->name.len;
        output.nameOffset = dynamicDataBlob.WriteString(symbol->name.c_str(), symbol->name.len);
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

        output.annotationsOffset = dynamicDataBlob.Reserve<Serialize::Annotation>(sampler->annotations.size);
        output.annotationsCount = sampler->annotations.size;

        size_t annotOffset = output.annotationsOffset;
        for (const Annotation* annot : sampler->annotations)
        {
            WriteAnnotation(this, annot, annotOffset, dynamicDataBlob);
            annotOffset += sizeof(Serialize::Annotation);
        }

        writer.WriteType(output);
        writer.IncrementDecodeSize(sizeof(Deserialize::Annotation) * sampler->annotations.size);
        writer.IncrementDecodeSize(sizeof(Deserialize::SamplerState));
    }
    else if (symbol->symbolType == Symbol::StructureType)
    {
        Structure* structure = static_cast<Structure*>(symbol);
        Structure::__Resolved* resolved = static_cast<Structure::__Resolved*>(symbol->resolved);
        Serialize::Structure output;
        output.isUniform = false;
        output.isMutable = false;
        output.nameLength = symbol->name.len;
        output.nameOffset = dynamicDataBlob.WriteString(symbol->name.c_str(), symbol->name.len);
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
                varOutput.nameLength = var->name.len;
                varOutput.nameOffset = dynamicDataBlob.WriteString(var->name.c_str(), var->name.len);
                varOutput.byteSize = resolved->byteSize;
                varOutput.structureOffset = resolved->structureOffset;
                varOutput.arraySizesCount = 0;
                varOutput.arraySizesOffset = dynamicDataBlob.iterator;
                for (size_t i = 0; i < var->type.modifiers.size; i++)
                {
                    if (var->type.modifiers[i] == Type::FullType::Modifier::Array && var->type.modifierValues[i] != nullptr)
                    {
                        varOutput.arraySizesCount++;
                        ValueUnion size;
                        var->type.modifierValues[i]->EvalValue(size);
                        dynamicDataBlob.Write(size.ui);
                    }
                }

                // write variable
                dynamicDataBlob.WriteReserved(varOutput, offset);
                offset += sizeof(Serialize::Variable);
                writer.IncrementDecodeSize(sizeof(Deserialize::Variable));
            }
        }

        // end serializing variables

        output.annotationsOffset = dynamicDataBlob.Reserve<Serialize::Annotation>(structure->annotations.size);
        output.annotationsCount = structure->annotations.size;

        size_t annotOffset = output.annotationsOffset;
        for (const Annotation* annot : structure->annotations)
        {
            WriteAnnotation(this, annot, annotOffset, dynamicDataBlob);
            annotOffset += sizeof(Serialize::Annotation);
        }

        writer.WriteType(output);
        writer.IncrementDecodeSize(sizeof(Deserialize::Annotation) * structure->annotations.size);
        writer.IncrementDecodeSize(sizeof(Deserialize::Structure));
    }
    else if (symbol->symbolType == Symbol::VariableType)
    {
        Variable* var = static_cast<Variable*>(symbol);
        Variable::__Resolved* resolved = static_cast<Variable::__Resolved*>(symbol->resolved);
        if (resolved->usageBits.flags.isNoReflect)
            return;
        if (resolved->usageBits.flags.isConst && (
            resolved->storage != Storage::Uniform
            && resolved->storage != Storage::InlineUniform
            )) // No need to dump consts to the reflection
            return;
        Serialize::Variable output;
        output.binding = resolved->binding;
        output.group = resolved->group;
        output.visibility.bits = resolved->visibilityBits.bits;
        output.nameLength = symbol->name.len;
        output.nameOffset = dynamicDataBlob.WriteString(symbol->name.c_str(), symbol->name.len);
        output.byteSize = resolved->typeSymbol->byteSize;
        output.structureOffset = resolved->structureOffset;
        output.arraySizesCount = 0;
        output.arraySizesOffset = dynamicDataBlob.iterator;
        for (size_t i = 0; i < var->type.modifiers.size; i++)
        {
            if (var->type.modifiers[i] == Type::FullType::Modifier::Array && var->type.modifierValues[i] != nullptr)
            {
                output.arraySizesCount++;
                ValueUnion size;
                var->type.modifierValues[i]->EvalValue(size);
                dynamicDataBlob.Write(size.ui);
            }
        }
        output.annotationsCount = var->annotations.size;
        output.annotationsOffset = dynamicDataBlob.Reserve<Serialize::Annotation>(var->annotations.size);
        output.bindingScope = Serialization::BindingScope::Constant;
        if (resolved->usageBits.flags.isEntryPointParameter)
            output.bindingScope = Serialization::BindingScope::VertexInput;
        else if (resolved->storage == Storage::Global)
            output.bindingScope = Serialization::BindingScope::Constant;
        else
            output.bindingScope = Serialization::BindingScope::Resource;

        output.bindingType = Serialization::BindingType::None;
        bool mut = var->type.IsMutable();
        if (resolved->typeSymbol->category == Type::Category::StructureCategory)
        {
            if (var->type.modifiers.size > 1 && var->type.modifiers[1] == Type::FullType::Modifier::Array)
            {
                mut = true;
            }
            if (mut)
                output.bindingType = Serialization::BindingType::MutableBuffer;
            else if (resolved->storage == Storage::InlineUniform)
                output.bindingType = Serialization::BindingType::Inline;
            else
                output.bindingType = Serialization::BindingType::Buffer;
        }
        else if (resolved->typeSymbol->category == Type::Category::TextureCategory)
        {
            if (mut)
                output.bindingType = Serialization::BindingType::MutableImage;
            else
                output.bindingType = Serialization::BindingType::Image;
        }
        else if (resolved->typeSymbol->category == Type::Category::SampledTextureCategory)
            output.bindingType = Serialization::BindingType::SampledImage;
        else if (resolved->typeSymbol->category == Type::Category::SamplerStateCategory)
            output.bindingType = Serialization::BindingType::Sampler;
        else if (resolved->typeSymbol->category == Type::Category::ScalarCategory)
            output.bindingType = Serialization::BindingType::LinkDefined;
        else if (resolved->typeSymbol->category == Type::Category::PixelCacheCategory)
            output.bindingType = Serialization::BindingType::PixelCache;
        else if (resolved->typeSymbol->category == Type::Category::AccelerationStructureCategory)
            output.bindingType = Serialization::BindingType::AccelerationStructure;

        output.structTypeNameLength = 0;
        output.structTypeNameOffset = 0;
        if (output.bindingType == Serialization::BindingType::Buffer || output.bindingType == Serialization::BindingType::MutableBuffer)
        {
            output.structTypeNameLength = var->type.name.len;
            output.structTypeNameOffset = dynamicDataBlob.WriteString(var->type.name.c_str(), var->type.name.len);
        }
        size_t offset = output.annotationsOffset;
        for (const Annotation* annot : var->annotations)
        {
            WriteAnnotation(this, annot, offset, dynamicDataBlob);
            offset += sizeof(Serialize::Annotation);
        }

        writer.WriteType(output);
        writer.IncrementDecodeSize(sizeof(Deserialize::Annotation) * var->annotations.size);
        writer.IncrementDecodeSize(sizeof(Deserialize::Variable));
    }
}

} // namespace GPULang
