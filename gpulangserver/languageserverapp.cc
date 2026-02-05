#include <lsp/messages.h>
#include "lsp/connection.h"
#include "lsp/messagehandler.h"
#include "lsp/io/standardio.h"
#include "gpulangcompiler.h"
#include "ast/alias.h"
#include "ast/preprocessor.h"
#include "ast/function.h"
#include "ast/generate.h"
#include "ast/variable.h"
#include "ast/enumeration.h"
#include "ast/structure.h"
#include "ast/renderstate.h"
#include "ast/samplerstate.h"
#include "ast/types/type.h"
#include "generated/types.h"
#include "ast/statements/scopestatement.h"
#include "ast/statements/ifstatement.h"
#include "ast/statements/forstatement.h"
#include "ast/statements/whilestatement.h"
#include "ast/statements/terminatestatement.h"
#include "ast/statements/expressionstatement.h"
#include "ast/expressions/callexpression.h"
#include "ast/expressions/symbolexpression.h"
#include "ast/expressions/binaryexpression.h"
#include "ast/expressions/unaryexpression.h"
#include "ast/expressions/accessexpression.h"
#include "ast/expressions/arrayindexexpression.h"
#include "thread.h"

#if __WIN32__
#include <winsock2.h>
#include <ws2tcpip.h>
#define SOCKET HANDLE
#else
#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#define SOCKET int
#endif

#include <map>
#include <filesystem>
#include <chrono>
#include <streambuf>
#include "memory.h"

GPULang::Allocator GlobalStringAllocator;

struct Socket
{
    SOCKET sock;
};

Socket CreateServerSocket()
{
    Socket ret;
    
    
#if __WIN32__
    
    ret.sock = CreateNamedPipeA(
        TEXT("\\\\.\\pipe\\gpulang_socket"),
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
        1,              // max instances
        65535,    // out buffer size
        65535,    // in buffer size
        0,              // default timeout
        NULL);
    
    assert(ret.sock != INVALID_HANDLE_VALUE);
    
#else
    // macOS uses TMPDIR
    const char* tmp = std::getenv("TMPDIR");
    
    // If that fails, fall back to using /tmp
    if (!tmp) tmp = "/tmp/";
    GPULang::TransientString socket_path = GPULang::TransientString(tmp, "gpulang_socket");
    
    unlink(socket_path.c_str());
    ret.sock = socket(AF_UNIX, SOCK_STREAM, 0);
    
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    std::strncpy(addr.sun_path, socket_path.c_str(), socket_path.size);

    int res = bind(ret.sock, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr));
    assert(res == 0);
    listen(ret.sock, 5);
#endif
    return ret;
}

Socket SocketAccept(const Socket& socket)
{
    Socket ret;
#if __WIN32__
    BOOL connected = ConnectNamedPipe(socket.sock, NULL);
    ret = socket; // On Windows, it's a shared pipe
#else
    ret.sock = accept(socket.sock, nullptr, nullptr);
#endif
    return ret;
}

void SocketClose(const Socket& socket)
{
#if __WIN32__
    CloseHandle(socket.sock);
#else
    shutdown(socket.sock, 0);
#endif
}

class SocketBuffer : public std::basic_streambuf<char>
{
public:
    SocketBuffer(Socket sock) :
        sock(sock)
    {}

    std::streamsize xsputn(const char* buf, std::streamsize count)
    {
#if __WIN32__
        DWORD sent;
        WriteFile(this->sock.sock, buf, count, &sent, nullptr);
        while (sent != count)
            WriteFile(this->sock.sock, buf + sent, count, &sent, nullptr);
#else
        int sent = write(this->sock.sock, buf, count);
        while (sent != count)
            sent = write(this->sock.sock, buf + sent, count);
#endif
        return count;

    }

    int underflow()
    {
#if __WIN32__
        DWORD size;
        ReadFile(this->sock.sock, this->buf, sizeof(buf), &size, NULL);
#else
        int size = read(this->sock.sock, this->buf, sizeof(buf));
#endif
        this->setg(this->buf, this->buf, this->buf + size);
        this->bytesAvail = size;
        return size;
    }

    template<typename T>
    void Write(const T* data, int count = 1)
    {
        size_t length = sizeof(T) * count;
        memcpy(this->buf, data, length);
        this->setp(this->buf, this->buf + length);
        this->bytesAvail = length;
    }

    char buf[65535];
    int bytesAvail;
    Socket sock;
};

union PresentationBits
{
    struct Flags
    {
        uint32_t symbolLookup : 1 = 0; // Presentation is according to a symbol being looked up
        uint32_t typeLookup : 1 = 0;   // Presentation is akin to a type being looked up
        uint32_t formatted : 1 = 0;    // Text is already formatted
    } flags;
    uint32_t bits;

    PresentationBits(uint32_t bits) : bits(bits) {};
    PresentationBits(Flags flags) : flags(flags) {};
};

struct SemanticToken
{
    uint32_t deltaLine, deltaColumn, length, type, modifiers;
};

struct TextRange
{
    size_t startLine, stopLine;
    size_t startColumn, stopColumn;

    bool operator<(const TextRange& rhs) const
    {
        if (this->startLine == rhs.startLine)
            return this->startColumn < rhs.startColumn;
        else
            return this->startLine < rhs.startLine;
    }
};

struct ParseContext
{
    GPULang::Compiler::Language lang = GPULang::Compiler::Language::VULKAN_SPIRV;
    GPULang::Compiler::Options options;
    std::vector<std::string> includePaths;
    struct ParsedFile
    {
        GPULangServerResult result;
        GPULang::Allocator alloc, stringAlloc;

        std::string path;
        GPULangFile* f;

        std::map<size_t, std::vector<std::tuple<TextRange, PresentationBits, const GPULang::Symbol*>>> symbolsByLine;
        std::map<TextRange, std::vector<const GPULang::Scope*>> scopesByRange;
        std::map<size_t, std::vector<const GPULang::Scope*>> scopesByLine;
        std::map<int, std::string_view> textByLine;

        static const GPULang::Symbol* FindSymbol(const GPULang::FixedString& name, const std::vector<const GPULang::Scope*> scopes)
        {
            auto scopeIt = scopes.rbegin();
            while (scopeIt != scopes.rend())
            {
                auto symIt = (*scopeIt)->symbolLookup.Find(GPULang::HashString(name));
                if (symIt != (*scopeIt)->symbolLookup.end())
                {
                    return symIt->second;
                }
                scopeIt++;
            }
            return nullptr;
        }

        static const std::set<GPULang::Symbol*> FindSymbols(const std::string_view& name, const std::vector<const GPULang::Scope*> scopes)
        {
            std::set<GPULang::Symbol*> ret;

            struct Comp
            {
                bool operator()(const std::string_view& lhs, const std::pair<std::string, const GPULang::Symbol*>& rhs) const { return rhs.first.starts_with(lhs); }
                bool operator()(const std::pair<std::string, const GPULang::Symbol*>& rhs, const std::string_view& lhs) const { return rhs.first.starts_with(lhs); }
            };
            auto scopeIt = scopes.rbegin();
            while (scopeIt != scopes.rend())
            {
                auto symIt = (*scopeIt)->symbolLookup.begin();
                bool startMatch = false;
                while (symIt != (*scopeIt)->symbolLookup.end())
                {
                    if (symIt->second->name.StartsWith(name))
                    {
                        ret.insert(symIt->second);
                        startMatch = true;
                    }
                    else if (startMatch)
                        break;
                    if (ret.size() == 32)
                        goto escape;
                    symIt++;
                }
                scopeIt++;
            }
            escape:
            return ret;
        }

        std::vector<uint32_t> semanticTokens;
        std::vector<lsp::DocumentSymbol> documentSymbols;
    };
    std::map<std::string, ParsedFile> parsedFiles;
};

//------------------------------------------------------------------------------
/**
*/
void
Clear(ParseContext::ParsedFile* file)
{
    if (file->result.intrinsicScope != nullptr)
        file->result.intrinsicScope->~Scope();
    file->result.intrinsicScope = nullptr;
    if (file->result.mainScope != nullptr)
        file->result.mainScope->~Scope();
    file->result.mainScope = nullptr;
    file->result.symbols.Invalidate();
    GPULang::ResetAllocator(&file->alloc);
    GPULang::ResetAllocator(&file->stringAlloc);
}

//------------------------------------------------------------------------------
/**
*/
void
ValidateFile(ParseContext::ParsedFile* file, ParseContext* context, const std::string& contents, lsp::MessageHandler& messageHandler)
{
    if (!contents.empty())
    {
        if (file->f->contents)
            free(file->f->contents);
        file->f->contents = (char*)malloc(contents.length());
        file->f->contentSize = contents.length();
        memcpy(file->f->contents, contents.data(), contents.length());
    }
    
    static auto find_eol = [](const char* begin, const char* end) -> const char*
    {
        const char* start = begin;
        while (start != end)
        {
            if (start[0] == '\r')
            {
                if ((start+1) != end && start[1] == '\n')
                    return start + 2;
                else
                    return start + 1;
            }
            else if (start[0] == '\n')
                return start+1;
            start++;
        }
        return end;
    };

    file->textByLine.clear();
    const char* start = file->f->contents;
    const char* end = file->f->contents + file->f->contentSize;
    int lineCounter = 0;
    while (start != end)
    {
        const char* eol = find_eol(start, end);
        file->textByLine[lineCounter++] = std::string_view(start, eol);
        start = eol;
    }

    // Pass it to the compiler
    Clear(file);
    GPULang::CurrentAllocator = &file->alloc;
    GPULang::StringAllocator = &file->stringAlloc;
    GPULang::Compiler::Options options = context->options;
    options.emitTimings = true;

    GPULangValidate(file->f, context->lang, context->includePaths, options, file->result);

    // sort symbols on line and starting point
    std::sort(file->result.symbols.begin(), file->result.symbols.end(), [](const GPULang::Symbol* lhs, const GPULang::Symbol* rhs)
    {
        if (lhs == nullptr)
        if (lhs->location.line == rhs->location.line)
            return lhs->location.start < rhs->location.start;
        return lhs->location.line < rhs->location.line;
    });
    
    GPULang::Preprocessor* pp = nullptr;
    GPULang::PinnedMap<GPULang::FixedString, GPULang::Preprocessor*> ppPerFileMap(0xFFF);
    
    for (auto sym : file->result.symbols)
    {
        if (sym->symbolType == GPULang::Symbol::SymbolType::PreprocessorType)
        {
            GPULang::Preprocessor* symPp = static_cast<GPULang::Preprocessor*>(sym);
            if (symPp->type == GPULang::Preprocessor::Type::Comment)
            {
                auto it = ppPerFileMap.Find(symPp->location.file);
                if (it == ppPerFileMap.end())
                {
                    ppPerFileMap.Insert(symPp->location.file, symPp);
                }
                else
                    it->second = symPp;
            }
        }
        else
        {
            // Assign documentation to symbol from previous preprocessor
            auto it = ppPerFileMap.Find(sym->location.file);
            if (it != ppPerFileMap.end() && it->second != nullptr)
            {
                sym->documentation = it->second->contents;
                it->second = nullptr;
            }
        }
    }

    std::map<GPULang::FixedString, lsp::notifications::TextDocument_PublishDiagnostics::Params> diagnosticsResults;
    if (file->result.diagnostics.size > 0)
    {
        for (auto& diagnostic : file->result.diagnostics)
        {
            if (diagnostic.file.buf != nullptr)
            {
                auto& it = diagnosticsResults[diagnostic.file];
                it.uri = diagnostic.file.c_str();
                it.diagnostics.push_back(lsp::Diagnostic{
                    .range = {
                            .start = { .line = (uint32_t)diagnostic.line - 1, .character = (uint32_t)diagnostic.column },
                            .end = { .line = (uint32_t)diagnostic.line - 1, .character = (uint32_t)diagnostic.column + diagnostic.length }
                    },
                    .message = diagnostic.error.c_str(),
                    .severity = lsp::DiagnosticSeverity::Error // FIXME, should come from the diagnostic
                });
            }
        }
        file->result.diagnostics.Clear();
    }
    if (!diagnosticsResults.empty())
    {
        for (auto res : diagnosticsResults)
        {
            messageHandler.messageDispatcher().sendNotification<lsp::notifications::TextDocument_PublishDiagnostics>(std::move(res.second));
        }
    }
    else
    {
        messageHandler.messageDispatcher().sendNotification<lsp::notifications::TextDocument_PublishDiagnostics>(lsp::PublishDiagnosticsParams{ .uri = file->f->path.c_str() });
    }
    
}

//------------------------------------------------------------------------------
/**
*/
ParseContext::ParsedFile*
ParseFile(const std::string path, ParseContext* context, lsp::MessageHandler& messageHandler, bool reload = false)
{
    auto it = context->parsedFiles.find(path);
    if (it == context->parsedFiles.end())
    {
        it = context->parsedFiles.insert({ path, ParseContext::ParsedFile() }).first;
        it->second.alloc = GPULang::CreateAllocator();
        it->second.stringAlloc = GPULang::CreateAllocator();
        GPULang::StringAllocator = &GlobalStringAllocator;
        GPULang::MakeAllocatorCurrent(&GPULang::DefaultAllocator);
        it->second.f = GPULangLoadFile(path.c_str());
        it->second.path = path;
    }
    else if (reload)
    {
        GPULangFile* file = it->second.f;
        assert(file != nullptr && file->contents != nullptr);
        
        FILE* f = fopen(file->path.c_str(), "rb");
        fseek(f, 0, SEEK_END);
        int size = ftell(f);

        free(file->contents);
        file->contents = (char*)malloc(size);
        rewind(f);
        fread(file->contents, 1, size, f);
        fclose(f);
        file->contentSize = size;
    }

    // Create temporary file for the compiler
    ValidateFile(&it->second, context, "", messageHandler);

    return &it->second;
};


//------------------------------------------------------------------------------
/**
*/
ParseContext::ParsedFile*
GetFile(const std::string path, ParseContext* context, lsp::MessageHandler& messageHandler)
{
    auto it = context->parsedFiles.find(path);
    if (it == context->parsedFiles.end())
    {
        messageHandler.messageDispatcher().sendNotification<lsp::notifications::CancelRequest>(lsp::notifications::CancelRequest::Params{ "File not parsed on server" });
        return nullptr;
    }
    return &it->second;
}


lsp::SemanticTokensLegend legend{
        .tokenTypes = {
            "type",
            "enum",
            "struct",
            "parameter",
            "variable",
            "property",
            "enumMember",
            "function",
            "macro",
            "keyword",
            "modifier",
            "comment",
            "string",
            "number",
            "operator",
            "decorator"
    },
        .tokenModifiers = {
            "declaration",
            "definition",
            "readonly",
            "static",
            "deprecated",
            "documentation",
            "defaultLibrary",
            "unused"
    }
};

enum class SemanticTypeMapping : uint32_t {
    Type
    , Enum
    , Struct
    , Parameter
    , Variable
    , Property
    , EnumMember
    , Function
    , Macro
    , Keyword
    , Modifier
    , Comment
    , String
    , Number
    , Operator
    , Decorator
};

enum class SemanticModifierMapping : uint32_t {
    None = 0x0
    , Declaration = 0x1
    , Definition = 0x2
    , ReadOnly = 0x4
    , Static = 0x8
    , Depreacted = 0x10
    , Documentation = 0x20
    , DefaultLibrary = 0x40
    , Dead = 0x80
};

SemanticModifierMapping operator|(const SemanticModifierMapping lhs, const SemanticModifierMapping rhs)
{
    return SemanticModifierMapping((uint32_t)lhs | (uint32_t)rhs);
}


struct Context
{
    GPULang::Symbol::Location loc;
    int carry = 0;
    
    int activeBranchIt = 0;
    bool activeBranchStack[32] = {true};
};

//------------------------------------------------------------------------------
/**
*/
void
InsertSemanticToken(Context& ctx, const GPULang::Symbol::Location& loc, SemanticTypeMapping type, uint32_t modifiers, std::vector<uint32_t>& result)
{
    SemanticToken token;
    if (loc.line == -1)
    {
        return;
    }
    
    if (loc.line < ctx.loc.line)
        return;
    // If new line, reset carry
    if (ctx.loc.line != loc.line)
        ctx.carry = 0;
    token.deltaLine = loc.line - ctx.loc.line;
    token.deltaColumn = loc.line == ctx.loc.line ? loc.start - ctx.loc.start + ctx.carry : loc.start;
    token.length = loc.end - loc.start;
    token.type = (uint32_t)type;
    token.modifiers = modifiers;
    ctx.loc = loc;
    result.insert(result.end(), reinterpret_cast<uint32_t*>(&token), reinterpret_cast<uint32_t*>(&token) + 5);
}

//------------------------------------------------------------------------------
/**
*/
void
CreateSemanticToken(Context& ctx, const GPULang::Symbol* sym, ParseContext::ParsedFile* file, std::vector<uint32_t>& result, std::vector<const GPULang::Scope*>& scopes)
{
    TextRange range;
    range.startLine = sym->location.line;
    range.stopLine = sym->location.line;
    range.startColumn = sym->location.start;
    range.stopColumn = sym->location.end;
    file->symbolsByLine[range.startLine].push_back(std::make_tuple(range, PresentationBits(0x0), sym));
    
    bool deadBranch = !ctx.activeBranchStack[ctx.activeBranchIt];
    SemanticModifierMapping dead = deadBranch ? SemanticModifierMapping::Dead : SemanticModifierMapping::None;
    
    auto annotationSemanticToken = [](const GPULang::Annotation* annot, Context& prevLoc, ParseContext::ParsedFile* file, std::vector<uint32_t>& result, std::vector<const GPULang::Scope*>& scopes)
    {
        InsertSemanticToken(prevLoc, annot->location, SemanticTypeMapping::Decorator, 0x0, result);
        if (annot->value != nullptr)
            CreateSemanticToken(prevLoc, annot->value, file, result, scopes);
    };

    switch (sym->symbolType)
    {
        case GPULang::Symbol::SymbolType::GenerateType:
        {
            const auto generate = static_cast<const GPULang::Generate*>(sym);
            InsertSemanticToken(ctx, generate->location, SemanticTypeMapping::Keyword, 0x0, result);
            for (const auto sym : generate->symbols)
            {
                CreateSemanticToken(ctx, sym, file, result, scopes);
            }
            break;
        }
        case GPULang::Symbol::SymbolType::PreprocessorType:
        {
            const GPULang::Preprocessor* pp = static_cast<const GPULang::Preprocessor*>(sym);
            switch (pp->type)
            {
            case GPULang::Preprocessor::Type::Comment:
                InsertSemanticToken(ctx, pp->location, SemanticTypeMapping::Comment, 0x0, result);
                break;
            case GPULang::Preprocessor::Type::Pragma:
                InsertSemanticToken(ctx, pp->location, SemanticTypeMapping::Keyword, 0x0, result);
                break;
            case GPULang::Preprocessor::Type::If:
            case GPULang::Preprocessor::Type::Undefine:
                InsertSemanticToken(ctx, pp->location, SemanticTypeMapping::Keyword, 0x0, result);
                InsertSemanticToken(ctx, pp->argLocations[0], SemanticTypeMapping::Macro, 0x0, result);
                break;
            case GPULang::Preprocessor::Type::Macro:
                InsertSemanticToken(ctx, pp->location, SemanticTypeMapping::Keyword, 0x0, result);
                InsertSemanticToken(ctx, pp->argLocations[0], SemanticTypeMapping::Macro, 0x0, result);
                break;
            case GPULang::Preprocessor::Type::Include:
                InsertSemanticToken(ctx, pp->location, SemanticTypeMapping::Keyword, 0x0, result);
                InsertSemanticToken(ctx, pp->argLocations[0], SemanticTypeMapping::String, 0x0, result);
                break;
            case GPULang::Preprocessor::Type::Else:
            case GPULang::Preprocessor::Type::EndIf:
                InsertSemanticToken(ctx, pp->location, SemanticTypeMapping::Keyword, 0x0, result);
                break;
            case GPULang::Preprocessor::Type::Call:
            {
                //InsertSemanticToken(ctx, pp->location, SemanticTypeMapping::Macro, 0x0, result);
                //uint32_t callLength = uint32_t(pp->location.end - pp->location.start);
                //if (callLength < pp->contents.len)
                    //ctx.carry += int(pp->contents.len - callLength); // Add the difference in length as a carry
                break;
            }
            }
            break;
        }
        case GPULang::Symbol::SymbolType::AliasType:
        {
            const GPULang::Alias* alias = static_cast<const GPULang::Alias*>(sym);
            InsertSemanticToken(ctx, alias->nameLocation, deadBranch ? SemanticTypeMapping::Comment : SemanticTypeMapping::Type, (uint32_t)dead, result);
            InsertSemanticToken(ctx, alias->typeLocation, deadBranch ? SemanticTypeMapping::Comment : SemanticTypeMapping::Type, (uint32_t)dead, result);
            break;
        }
        case GPULang::Symbol::SymbolType::VariableType:
        {
            const GPULang::Variable* var = static_cast<const GPULang::Variable*>(sym);
            const GPULang::Variable::__Resolved* res = GPULang::Symbol::Resolved(var);
            for (auto annot : var->annotations)
            {
                annotationSemanticToken(annot, ctx, file, result, scopes);
            }
            for (auto attr : var->attributes)
            {
                InsertSemanticToken(ctx, attr->location, deadBranch ? SemanticTypeMapping::Comment : SemanticTypeMapping::Modifier, (uint32_t)dead, result);
                if (attr->expression != nullptr)
                    CreateSemanticToken(ctx, attr->expression, file, result, scopes);
            }
            if (res->usageBits.flags.isParameter)
                InsertSemanticToken(ctx, sym->location, deadBranch ? SemanticTypeMapping::Comment : SemanticTypeMapping::Parameter, (uint32_t)(SemanticModifierMapping::Definition | dead), result);
            else
                InsertSemanticToken(ctx, sym->location, deadBranch ? SemanticTypeMapping::Comment : SemanticTypeMapping::Variable, (uint32_t)(SemanticModifierMapping::Definition | dead), result);

            for (auto& modifier : var->type.modifierValues)
            {
                if (modifier != nullptr)
                    CreateSemanticToken(ctx, modifier, file, result, scopes);
            }
            if (var->type.modifierLocation.Valid())
                InsertSemanticToken(ctx, var->type.modifierLocation, deadBranch ? SemanticTypeMapping::Comment : SemanticTypeMapping::Keyword, (uint32_t)dead, result);
            if (var->type.formatLocation.Valid())
                InsertSemanticToken(ctx, var->type.formatLocation, deadBranch ? SemanticTypeMapping::Comment : SemanticTypeMapping::Keyword, (uint32_t)dead, result);
            if (var->type.nameLocation.Valid())
            {
                InsertSemanticToken(ctx, var->type.nameLocation, deadBranch ? SemanticTypeMapping::Comment : SemanticTypeMapping::Type, (uint32_t)dead, result);
                TextRange range;
                range.startLine = var->type.nameLocation.line;
                range.stopLine = var->type.nameLocation.line;
                range.startColumn = var->type.nameLocation.start;
                range.stopColumn = var->type.nameLocation.end;
                file->symbolsByLine[range.startLine].push_back(std::make_tuple(range, PresentationBits{ {.typeLookup = 1} }, res->typeSymbol));
            }

            if (var->valueExpression != nullptr)
                CreateSemanticToken(ctx, var->valueExpression, file, result, scopes);
            break;
        }
        case GPULang::Symbol::SymbolType::FunctionType:
        {
            const GPULang::Function* fun = static_cast<const GPULang::Function*>(sym);
            const GPULang::Function::__Resolved* res = GPULang::Symbol::Resolved(fun);
            scopes.push_back(&res->scope);
            file->scopesByLine[range.startLine] = scopes;
            file->scopesByRange[range] = scopes;
            for (auto annot : fun->annotations)
                annotationSemanticToken(annot, ctx, file, result, scopes);
            for (auto attr : fun->attributes)
            {
                InsertSemanticToken(ctx, attr->location, deadBranch ? SemanticTypeMapping::Comment : SemanticTypeMapping::Keyword, (uint32_t)dead, result);
                if (attr->expression != nullptr)
                    CreateSemanticToken(ctx, attr->expression, file, result, scopes);
            }

            InsertSemanticToken(ctx, sym->location, deadBranch ? SemanticTypeMapping::Comment : SemanticTypeMapping::Function, (uint32_t)(SemanticModifierMapping::Definition | dead), result);

            for (auto var : fun->parameters)
            {
                CreateSemanticToken(ctx, var, file, result, scopes);
            }

            InsertSemanticToken(ctx, fun->returnType.nameLocation, deadBranch ? SemanticTypeMapping::Comment : SemanticTypeMapping::Type, (uint32_t)dead, result);
            TextRange range;
            range.startLine = fun->returnType.nameLocation.line;
            range.stopLine = fun->returnType.nameLocation.line;
            range.startColumn = fun->returnType.nameLocation.start;
            range.stopColumn = fun->returnType.nameLocation.end;
            file->symbolsByLine[range.startLine].push_back(std::make_tuple(range, PresentationBits{ {.typeLookup = 1} }, res->returnTypeSymbol));

            if (fun->ast != nullptr)
            {
                CreateSemanticToken(ctx, fun->ast, file, result, scopes);
            }
            scopes.pop_back();

            break;
        }
        case GPULang::Symbol::SymbolType::EnumerationType:
        {
            const GPULang::Enumeration* enu = static_cast<const GPULang::Enumeration*>(sym);
            const GPULang::Enumeration::__Resolved* res = GPULang::Symbol::Resolved(enu);
            InsertSemanticToken(ctx, sym->location, deadBranch ? SemanticTypeMapping::Comment : SemanticTypeMapping::Enum, (uint32_t)SemanticModifierMapping::Definition, result);
            if (res->typeSymbol == nullptr)
                return;
            scopes.push_back(&res->typeSymbol->scope);
            file->scopesByLine[range.startLine] = scopes;

            for (auto mem : enu->labelLocations)
            {
                InsertSemanticToken(ctx, mem, deadBranch ? SemanticTypeMapping::Comment : SemanticTypeMapping::EnumMember, (uint32_t)SemanticModifierMapping::Definition, result);
            }
            scopes.pop_back();
            break;
        }
        case GPULang::Symbol::SymbolType::RenderStateInstanceType:
        {
            const GPULang::RenderStateInstance* struc = static_cast<const GPULang::RenderStateInstance*>(sym);
            const GPULang::RenderStateInstance::__Resolved* res = GPULang::Symbol::Resolved(struc);
            InsertSemanticToken(ctx, sym->location, deadBranch ? SemanticTypeMapping::Comment : SemanticTypeMapping::Struct, (uint32_t)SemanticModifierMapping::Definition, result);
            auto& [_0, bits, _1] = file->symbolsByLine[range.startLine].back();
            bits = PresentationBits{ { .typeLookup=1 } };
            if (res->typeSymbol == nullptr)
                return;
            scopes.push_back(&res->typeSymbol->scope);
            file->scopesByLine[range.startLine] = scopes;
            for (auto entry : struc->entries)
            {
                const GPULang::BinaryExpression* assignEntry = static_cast<const GPULang::BinaryExpression*>(entry);
                CreateSemanticToken(ctx, assignEntry->left, file, result, scopes);
                CreateSemanticToken(ctx, assignEntry->right, file, result, scopes);
            }
            scopes.pop_back();
            break;
        }
        case GPULang::Symbol::SymbolType::SamplerStateInstanceType:
        {
            const GPULang::SamplerStateInstance* struc = static_cast<const GPULang::SamplerStateInstance*>(sym);
            const GPULang::SamplerStateInstance::__Resolved* res = GPULang::Symbol::Resolved(struc);
            for (auto annot : struc->annotations)
            {
                annotationSemanticToken(annot, ctx, file, result, scopes);
            }
            for (auto attr : struc->attributes)
            {
                InsertSemanticToken(ctx, attr->location, deadBranch ? SemanticTypeMapping::Comment : SemanticTypeMapping::Modifier, (uint32_t)dead, result);
                if (attr->expression != nullptr)
                    CreateSemanticToken(ctx, attr->expression, file, result, scopes);
            }
            InsertSemanticToken(ctx, sym->location, deadBranch ? SemanticTypeMapping::Comment : SemanticTypeMapping::Struct, (uint32_t)SemanticModifierMapping::Definition, result);
            auto& [_0, bits, _1] = file->symbolsByLine[range.startLine].back();
            bits = PresentationBits{ { .typeLookup=1 } };
            if (res->typeSymbol == nullptr)
                return;
            scopes.push_back(&res->typeSymbol->scope);
            file->scopesByLine[range.startLine] = scopes;
            for (auto entry : struc->entries)
            {
                const GPULang::BinaryExpression* assignEntry = static_cast<const GPULang::BinaryExpression*>(entry);
                CreateSemanticToken(ctx, assignEntry->left, file, result, scopes);
                CreateSemanticToken(ctx, assignEntry->right, file, result, scopes);
            }
            scopes.pop_back();
            break;
        }
        case GPULang::Symbol::SymbolType::StructureType:
        {
            const GPULang::Structure* struc = static_cast<const GPULang::Structure*>(sym);
            InsertSemanticToken(ctx, sym->location, deadBranch ? SemanticTypeMapping::Comment : SemanticTypeMapping::Struct, (uint32_t)SemanticModifierMapping::Definition, result);
            scopes.push_back(&struc->scope);
            file->scopesByLine[range.startLine] = scopes;
            for (auto var : struc->symbols)
            {
                CreateSemanticToken(ctx, var, file, result, scopes);
            }
            scopes.pop_back();
            break;
        }
        case GPULang::Symbol::SymbolType::ProgramInstanceType:
        {
            const GPULang::ProgramInstance* prog = static_cast<const GPULang::ProgramInstance*>(sym);
            const auto* res = GPULang::Symbol::Resolved(prog);
            auto& [_0, bits, _1] = file->symbolsByLine[range.startLine].back();
            bits = PresentationBits{ { .typeLookup=1 } };
            for (auto annot : prog->annotations)
                annotationSemanticToken(annot, ctx, file, result, scopes);

            InsertSemanticToken(ctx, sym->location, deadBranch ? SemanticTypeMapping::Comment : SemanticTypeMapping::Struct, (uint32_t)SemanticModifierMapping::Definition, result);
            if (res->typeSymbol == nullptr)
                return;
            scopes.push_back(&res->typeSymbol->scope);
            file->scopesByLine[range.startLine] = scopes;
            for (auto entry : prog->entries)
            {
                CreateSemanticToken(ctx, entry, file, result, scopes);
            }
            scopes.pop_back();
            
            break;
        }
        case GPULang::Symbol::SymbolType::ScopeStatementType:
        {
            const GPULang::ScopeStatement* scope = static_cast<const GPULang::ScopeStatement*>(sym);
            const GPULang::ScopeStatement::__Resolved* res = GPULang::Symbol::Resolved(scope);
            scopes.push_back(&res->scope);
            file->scopesByLine[range.startLine] = scopes;

            TextRange scopeRange = range;
            scopeRange.stopLine = scope->ends.line;
            scopeRange.stopColumn = scope->ends.start;
            file->scopesByRange[scopeRange] = scopes;

            for (auto innerSym : scope->symbols)
            {
                CreateSemanticToken(ctx, innerSym, file, result, scopes);
            }
            scopes.pop_back();
            break;
        }
        case GPULang::Symbol::SymbolType::ExpressionStatementType:
        {
            const GPULang::ExpressionStatement* stat = static_cast<const GPULang::ExpressionStatement*>(sym);
            for (auto& expr : stat->expressions)
                CreateSemanticToken(ctx, expr, file, result, scopes);
            break;
        }
        case GPULang::Symbol::SymbolType::ForStatementType:
        {
            const GPULang::ForStatement* stat = static_cast<const GPULang::ForStatement*>(sym);
            InsertSemanticToken(ctx, sym->location, deadBranch ? SemanticTypeMapping::Comment : SemanticTypeMapping::Keyword, 0x0, result);
            for (auto var : stat->declarations)
                CreateSemanticToken(ctx, var, file, result, scopes);

            CreateSemanticToken(ctx, stat->condition, file, result, scopes);
            CreateSemanticToken(ctx, stat->loop, file, result, scopes);

            CreateSemanticToken(ctx, stat->contents, file, result, scopes);
            break;
        }
        case GPULang::Symbol::SymbolType::WhileStatementType:
        {
            const GPULang::WhileStatement* stat = static_cast<const GPULang::WhileStatement*>(sym);
            InsertSemanticToken(ctx, sym->location, deadBranch ? SemanticTypeMapping::Comment : SemanticTypeMapping::Keyword, 0x0, result);
            CreateSemanticToken(ctx, stat->condition, file, result, scopes);
            CreateSemanticToken(ctx, stat->statement, file, result, scopes);
            break;
        }
        case GPULang::Symbol::SymbolType::IfStatementType:
        {
            const GPULang::IfStatement* stat = static_cast<const GPULang::IfStatement*>(sym);
            InsertSemanticToken(ctx, sym->location, SemanticTypeMapping::Keyword, 0x0, result);
            
            CreateSemanticToken(ctx, stat->condition, file, result, scopes);
            
            GPULang::ValueUnion staticBranch;
            if (stat->condition->EvalValue(staticBranch))
            {
                int condition;
                staticBranch.Store(condition);
                ctx.activeBranchStack[++ctx.activeBranchIt] = (condition != 0) && !deadBranch;
                
                CreateSemanticToken(ctx, stat->ifStatement, file, result, scopes);
                ctx.activeBranchStack[ctx.activeBranchIt] = !ctx.activeBranchStack[ctx.activeBranchIt];
                if (stat->elseStatement != nullptr)
                    CreateSemanticToken(ctx, stat->elseStatement, file, result, scopes);
                
                ctx.activeBranchStack[ctx.activeBranchIt--] = true;
            }
            else
            {
                CreateSemanticToken(ctx, stat->ifStatement, file, result, scopes);
                if (stat->elseStatement != nullptr)
                    CreateSemanticToken(ctx, stat->elseStatement, file, result, scopes);
            }
            
            break;
        }
        case GPULang::Symbol::SymbolType::TerminateStatementType:
        {
            const GPULang::TerminateStatement* stat = static_cast<const GPULang::TerminateStatement*>(sym);
            InsertSemanticToken(ctx, sym->location, deadBranch ? SemanticTypeMapping::Comment : SemanticTypeMapping::Keyword, 0x0, result);
            if (stat->returnValue != nullptr)
                CreateSemanticToken(ctx, stat->returnValue, file, result, scopes);
            break;
        }
        case GPULang::Symbol::SymbolType::BinaryExpressionType:
        {
            const GPULang::BinaryExpression* expr = static_cast<const GPULang::BinaryExpression*>(sym);
            CreateSemanticToken(ctx, expr->left, file, result, scopes);
            CreateSemanticToken(ctx, expr->right, file, result, scopes);
            break;
        }
        case GPULang::Symbol::SymbolType::UnaryExpressionType:
        {
            auto unary = static_cast<const GPULang::UnaryExpression*>(sym);
            CreateSemanticToken(ctx, unary->expr, file, result, scopes);
            break;
        }
        case GPULang::Symbol::SymbolType::AccessExpressionType:
        {
            const GPULang::AccessExpression* expr = static_cast<const GPULang::AccessExpression*>(sym);
            const auto res = GPULang::Symbol::Resolved(expr);
            CreateSemanticToken(ctx, expr->left, file, result, scopes);

            // Push type on the left
            if (res->lhsType == nullptr)
                return;
            scopes.push_back(&res->lhsType->scope);
            file->scopesByLine[range.startLine] = scopes;
            CreateSemanticToken(ctx, expr->right, file, result, scopes);
            scopes.pop_back();
            break;
        }
        case GPULang::Symbol::SymbolType::ArrayIndexExpressionType:
        {
            const GPULang::ArrayIndexExpression* expr = static_cast<const GPULang::ArrayIndexExpression*>(sym);
            CreateSemanticToken(ctx, expr->left, file, result, scopes);
            CreateSemanticToken(ctx, expr->right, file, result, scopes);
            break;
        }
        case GPULang::Symbol::SymbolType::CallExpressionType:
        {
            const GPULang::CallExpression* call = static_cast<const GPULang::CallExpression*>(sym);
            InsertSemanticToken(ctx, sym->location, deadBranch ? SemanticTypeMapping::Comment : SemanticTypeMapping::Function, 0x0, result);
            for (auto var : call->args)
                CreateSemanticToken(ctx, var, file, result, scopes);
            break;
        }
        case GPULang::Symbol::SymbolType::SymbolExpressionType:
        {
            const GPULang::SymbolExpression* symExpr = static_cast<const GPULang::SymbolExpression*>(sym);
            auto foundSym = ParseContext::ParsedFile::FindSymbol(symExpr->symbol, scopes);
            if (foundSym != nullptr)
            {
                switch (foundSym->symbolType)
                {
                    case GPULang::Symbol::SymbolType::VariableType:
                        InsertSemanticToken(ctx, sym->location, deadBranch ? SemanticTypeMapping::Comment : SemanticTypeMapping::Variable, 0x0, result);
                        break;
                    case GPULang::Symbol::SymbolType::EnumerationType:
                    {
                        InsertSemanticToken(ctx, sym->location, deadBranch ? SemanticTypeMapping::Comment : SemanticTypeMapping::Enum, 0x0, result);
                        break;
                    }
                    case GPULang::Symbol::SymbolType::EnumExpressionType:
                        InsertSemanticToken(ctx, sym->location, deadBranch ? SemanticTypeMapping::Comment : SemanticTypeMapping::EnumMember, 0x0, result);
                        break;
                    default:
                        InsertSemanticToken(ctx, sym->location, deadBranch ? SemanticTypeMapping::Comment : SemanticTypeMapping::Variable, 0x0, result);
                }
                TextRange range;
                range.startLine = sym->location.line;
                range.stopLine = sym->location.line;
                range.startColumn = sym->location.start;
                range.stopColumn = sym->location.end;
                file->symbolsByLine[range.startLine].push_back(std::make_tuple(range, PresentationBits{{.symbolLookup=1}}, foundSym));
            }
            else
                InsertSemanticToken(ctx, sym->location, deadBranch ? SemanticTypeMapping::Comment : SemanticTypeMapping::Variable, 0x0, result);
            break;
        }
        case GPULang::Symbol::SymbolType::IntExpressionType:
        case GPULang::Symbol::SymbolType::FloatExpressionType:
        case GPULang::Symbol::SymbolType::BoolExpressionType:
        case GPULang::Symbol::SymbolType::UIntExpressionType:
        {
            InsertSemanticToken(ctx, sym->location, deadBranch ? SemanticTypeMapping::Comment : SemanticTypeMapping::Number, 0x0, result);
            break;
        }
        case GPULang::Symbol::SymbolType::StringExpressionType:
        {
            InsertSemanticToken(ctx, sym->location, deadBranch ? SemanticTypeMapping::Comment : SemanticTypeMapping::String, 0x0, result);
            break;
        }
    }
}


//------------------------------------------------------------------------------
/**
*/
std::string
CreateMarkdown(const GPULang::Symbol* sym, PresentationBits lookup = 0x0)
{
    std::string ret = "";
    
    if (!lookup.flags.formatted)
        ret += "```gpulang\n";

    switch (sym->symbolType)
    {
        case GPULang::Symbol::SymbolType::PreprocessorType:
        {
            auto pp = static_cast<const GPULang::Preprocessor*>(sym);
            if (pp->type == GPULang::Preprocessor::Type::Call)
            {
                ret = pp->contents.c_str();
            }
            else if (pp->type == GPULang::Preprocessor::Type::Include)
            {
                ret = pp->contents.c_str();
            }
            else
            {
                ret = "";
            }
            break;
        }
        case GPULang::Symbol::SymbolType::VariableType:
        {
            const GPULang::Variable* var = static_cast<const GPULang::Variable*>(sym);
            const GPULang::Variable::__Resolved* res = GPULang::Symbol::Resolved(var);

            if (lookup.flags.symbolLookup)
            {
                if (var->attributes.size > 0)
                {
                    for (auto attr : var->attributes)
                    {
                        ret += GPULang::Format("%s", attr->name.c_str());
                        if (attr->expression != nullptr)
                            ret += GPULang::Format("(%s) ", attr->expression->EvalString().c_str());
                        else
                            ret += " ";
                    }
                    ret += ": ";
                }
                ret += var->type.ToString().c_str();
                if (var->valueExpression != nullptr)
                {
                    GPULang::ValueUnion val;
                    if (var->valueExpression->EvalValue(val))
                    {
                        ret += "\n";
                        ret += GPULang::Format("Value: %s\n", var->valueExpression->EvalString().c_str());
                    }
                }
            }
            else if (lookup.flags.typeLookup)
            {
                for (auto attr : var->attributes)
                {
                    ret += GPULang::Format("*%s*", attr->name.c_str());
                    if (attr->expression != nullptr)
                        ret += GPULang::Format("(%s) ", attr->expression->EvalString().c_str());
                    else
                        ret += " ";
                }
                ret += GPULang::Format("%s : ", var->name.c_str());
                ret += var->type.ToString().c_str();
            }
            else
            {
                ret += var->type.ToString().c_str();
            }
            break;
        }
        case GPULang::Symbol::SymbolType::FunctionType:
        {
            const GPULang::Function* fun = static_cast<const GPULang::Function*>(sym);

            if (lookup.flags.symbolLookup)
            {
                ret += "function\n";
                for (auto attr : fun->attributes)
                {
                    ret += GPULang::Format("%s", attr->name.c_str());
                    if (attr->expression != nullptr)
                        ret += GPULang::Format("(%s) ", attr->expression->EvalString().c_str());
                    else
                        ret += " ";
                }
            }
            else
            {
                const GPULang::Function::__Resolved* res = GPULang::Symbol::Resolved(fun);
                if (res->isEntryPoint)
                    ret += "Function can be bound to a program\n\n";
            }
            break;
        }
        case GPULang::Symbol::SymbolType::EnumerationType:
        {
            const GPULang::Enumeration* enu = static_cast<const GPULang::Enumeration*>(sym);

            if (
                (lookup.bits == 0x0 && enu->builtin)
                || (lookup.flags.symbolLookup)
                )
            {
                ret += "enum\n";
                for (auto mem : enu->labels)
                {
                    ret += std::string(mem.c_str()) + "\n";
                }
            }
            break;
        }
        case GPULang::Symbol::SymbolType::StructureType:
        {
            const GPULang::Structure* struc = static_cast<const GPULang::Structure*>(sym);
            if (lookup.flags.typeLookup)
            {
                PresentationBits memberBits(lookup);
                memberBits.flags.formatted = true;
                ret += "struct\n";
                for (auto mem : struc->symbols)
                {
                    ret += CreateMarkdown(mem, memberBits);
                    ret += "\n";
                }
            }
            break;
        }
        case GPULang::Symbol::SymbolType::ExpressionStatementType:
        {
            const GPULang::ExpressionStatement* stat = static_cast<const GPULang::ExpressionStatement*>(sym);
            for (auto& expr : stat->expressions)
                ret += CreateMarkdown(expr, lookup);
            break;
        }
        case GPULang::Symbol::SymbolType::ArrayIndexExpressionType:
        {
            const auto expr = static_cast<const GPULang::ArrayIndexExpression*>(sym);
            ret += CreateMarkdown(expr->left, lookup);
            ret += "[";
            ret += CreateMarkdown(expr->right, lookup);
            ret += "]";
            break;
        }
        case GPULang::Symbol::SymbolType::AccessExpressionType:
        {
            const auto expr = static_cast<const GPULang::AccessExpression*>(sym);
            ret += CreateMarkdown(expr->left, lookup);
            ret += ".";
            ret += CreateMarkdown(expr->right, lookup);
            break;
        }
        case GPULang::Symbol::SymbolType::CallExpressionType:
        {
            const GPULang::CallExpression* call = static_cast<const GPULang::CallExpression*>(sym);
            const GPULang::CallExpression::__Resolved* res = GPULang::Symbol::Resolved(call);
            if (res->function != nullptr)
            {
                const GPULang::Function::__Resolved* funRes = GPULang::Symbol::Resolved(res->function);
                ret += GPULang::Format("%s(", res->function->name.c_str());
                for (auto param : res->function->parameters)
                {
                    const GPULang::Variable::__Resolved* paramRes = GPULang::Symbol::Resolved(param);
                    ret += GPULang::Format("%s : %s", param->name.c_str(), param->type.ToString(true).c_str());
                    if (param != res->function->parameters.back())
                        ret += ", ";
                }
                ret += GPULang::Format(") %s\n", res->function->returnType.ToString().c_str());
                lookup.flags.symbolLookup = true;
                sym = res->function;
            }
            break;
        }
        case GPULang::Symbol::SymbolType::SymbolExpressionType:
        {
            const GPULang::SymbolExpression* symbol = static_cast<const GPULang::SymbolExpression*>(sym);
            const GPULang::SymbolExpression::__Resolved* res = GPULang::Symbol::Resolved(symbol);
            if (res->symbol != nullptr)
            {
                if (lookup.flags.symbolLookup)
                    ret += CreateMarkdown(res->symbol, PresentationBits{ {.symbolLookup = 1, .formatted = 1} });
                else
                    ret += symbol->symbol.c_str();
            }
            
            break;
        }
        case GPULang::Symbol::SymbolType::RenderStateInstanceType:
        {
            const auto state = static_cast<const GPULang::RenderStateInstance*>(sym);
            const auto res = GPULang::Symbol::Resolved(state);
            ret += "render_state\n";
            if (res->typeSymbol != nullptr)
            {
                if (lookup.flags.symbolLookup)
                {
                    for (auto entry : state->entries)
                    {
                        const auto expr = static_cast<GPULang::BinaryExpression*>(entry);
                        ret += CreateMarkdown(expr->left, PresentationBits{ {.typeLookup = 1, .formatted = 1}});
                        ret += " = ";
                        ret += CreateMarkdown(expr->right, PresentationBits{ { .formatted = 1}});
                        ret += "\n";
                    }
                }
                else
                {
                    for (auto mem : res->typeSymbol->scope.symbolLookup)
                    {
                        ret += CreateMarkdown(mem.second, PresentationBits{ {.typeLookup = 1, .formatted = 1} });
                        ret += "\n";
                    }
                }
            }
            break;
        }
        case GPULang::Symbol::SymbolType::SamplerStateInstanceType:
        {
            const auto state = static_cast<const GPULang::SamplerStateInstance*>(sym);
            const auto res = GPULang::Symbol::Resolved(state);
            ret += "sampler_state\n";
            if (lookup.flags.symbolLookup)
            {
                for (auto attr : state->attributes)
                {
                    ret += GPULang::Format("%s", attr->name.c_str());
                    if (attr->expression != nullptr)
                        ret += GPULang::Format("(%s) ", attr->expression->EvalString().c_str());
                    else
                        ret += " ";
                }
            }
            else if (lookup.flags.typeLookup)
            {
                for (auto attr : state->attributes)
                {
                    ret += GPULang::Format("*%s*", attr->name.c_str());
                    if (attr->expression != nullptr)
                        ret += GPULang::Format("(%s) ", attr->expression->EvalString().c_str());
                    else
                        ret += " ";
                }
                ret += GPULang::Format("%s : ", state->name.c_str());
            }
            if (res->typeSymbol != nullptr)
            {
                if (lookup.flags.symbolLookup)
                {
                    for (auto entry : state->entries)
                    {
                        const auto expr = static_cast<GPULang::BinaryExpression*>(entry);
                        ret += CreateMarkdown(expr->left, PresentationBits{ {.typeLookup = 1, .formatted = 1}});
                        ret += " = ";
                        ret += CreateMarkdown(expr->right, PresentationBits{ { .formatted = 1}});
                        ret += "\n";
                    }
                }
                else
                {
                    for (auto mem : res->typeSymbol->scope.symbolLookup)
                    {
                        ret += CreateMarkdown(mem.second, PresentationBits{ {.typeLookup = 1, .formatted = 1} });
                        ret += "\n";
                    }
                }
            }
            break;
        }
        case GPULang::Symbol::SymbolType::ProgramInstanceType:
        {
            const auto state = static_cast<const GPULang::ProgramInstance*>(sym);
            const auto res = GPULang::Symbol::Resolved(state);
            ret += "program\n";
            if (res->typeSymbol != nullptr)
            {
                for (auto mem : res->typeSymbol->scope.symbolLookup)
                {
                    ret += CreateMarkdown(mem.second, PresentationBits{ {.typeLookup = 1, .formatted = 1} });
                    ret += "\n";
                }
            }
            break;
        }
        case GPULang::Symbol::SymbolType::IntExpressionType:
        case GPULang::Symbol::SymbolType::UIntExpressionType:
        case GPULang::Symbol::SymbolType::FloatExpressionType:
        case GPULang::Symbol::SymbolType::BoolExpressionType:
        {
            const GPULang::Expression* expr = static_cast<const GPULang::Expression*>(sym);
            ret += expr->EvalString().c_str();
            break;
        }
        default:
            return "";
    }
    
    if (!lookup.flags.formatted)
        ret += "\n```\n";
    
    if (lookup.flags.symbolLookup)
    {
        if (sym->documentation.buf != nullptr)
        {
            ret += sym->documentation.c_str();
            ret += "\n";
        }
        
        if (sym->location.line != -1)
            ret += GPULang::Format("[Declared Here](%s#L%d)\n", sym->location.file.c_str(), sym->location.line+1);
    }
    
    return ret;
}

GPULang::SystemSetup dummy;

//------------------------------------------------------------------------------
/**
*/
int __cdecl
main(int argc, const char** argv)
{
    Socket socket = CreateServerSocket();

    fprintf(stdout, "*** GPULang Language Server ***\n");
    fprintf(stdout, "Waiting for client...\n");
    fflush(stdout);

    std::map<int, ParseContext> parseContexts;

    Socket client = SocketAccept(socket);

    printf("Connection established\n");
    SocketBuffer inputBuffer(client);
    SocketBuffer outputBuffer(client);
    std::istream input(&inputBuffer);
    std::ostream output(&outputBuffer);
    lsp::Connection connection{ input, output };
    
    GPULang::Compiler::Options options;
    GPULang::Compiler::Language language;
    
    GlobalStringAllocator = GPULang::CreateAllocator();
    GPULang::InitAllocator(&GlobalStringAllocator);
    GPULang::StringAllocator = &GlobalStringAllocator;

    lsp::MessageHandler messageHandler{ connection };
    bool running = true;
    ParseContext* context = nullptr;

    messageHandler.requestHandler()
    .add<lsp::requests::Initialize>([&parseContexts, &context, &options, &language](const lsp::jsonrpc::MessageId& id, lsp::requests::Initialize::Params&& params)
    {
        lsp::requests::Initialize::Result result;
        if (params.processId.isNull())
        {
            parseContexts[params.processId.value()] = ParseContext{};
            context = &parseContexts[params.processId.value()];
        }
        else
        {
            parseContexts[0] = ParseContext{};
            context = &parseContexts[0];
        }
            
        if (params.workspaceFolders)
        {
            for (const auto& workspaceFolder : params.workspaceFolders->value())
            {
                context->includePaths.push_back("-I" + workspaceFolder.uri.path() + "/");
            }
        }
         
        // Use config if provided
        if (params.initializationOptions.has_value())
        {
            const auto options = params.initializationOptions->object();
            const auto config = options.get("config").object();
            const auto proj_dir = config.get("project_root").string();
            const auto dirs = config.get("include_directories").array();
            const auto flags = config.get("flags").array();
                                
            for (const auto& dir : dirs)
                context->includePaths.push_back("-I" + proj_dir + "/" + dir.string() + "/");
            
            for (const auto& flag : flags)
            {
                const std::string flagStr = flag.string();
                if (flagStr == "SPIRV")
                    context->lang = GPULang::Compiler::Language::SPIRV;
                else if (flagStr == "VULKAN_SPIRV")
                    context->lang = GPULang::Compiler::Language::VULKAN_SPIRV;
                else if (flagStr == "disallowImplicitConversion")
                    context->options.disallowImplicitConversion = true;
                else if (flagStr == "disallowImplicitPromotion")
                    context->options.disallowImplicitPromotion = true;
                else if (flagStr == "profile")
                    context->options.emitTimings = true;
                else if (flagStr == "generateBackend")
                {
                    context->options.languageServerGeneration = true;
                    context->options.validate = true;
                }
                    
            }
        }
        else
        {

        }

        for (auto& path : context->includePaths)
        {
            std::replace(path.begin(), path.end(), '\\', '/');
        }
        printf("Initialized\n");

        result.capabilities.textDocumentSync = lsp::TextDocumentSyncOptions{ .openClose = true, .change = lsp::TextDocumentSyncKind::Full, .save = true };
        result.capabilities.workspace = lsp::ServerCapabilitiesWorkspace{ .workspaceFolders = lsp::WorkspaceFoldersServerCapabilities{ .supported = true } };
        result.capabilities.documentHighlightProvider = lsp::DocumentHighlightOptions{ .workDoneProgress = true };
        result.capabilities.referencesProvider = lsp::ReferenceOptions{ .workDoneProgress = true };
        //result.capabilities.declarationProvider = lsp::DeclarationOptions{ .workDoneProgress = true };
        result.capabilities.definitionProvider = lsp::DefinitionOptions{ .workDoneProgress = true };
        result.capabilities.semanticTokensProvider = lsp::SemanticTokensOptions{ .legend = legend, .range = false, .full = true };
        result.capabilities.hoverProvider = lsp::HoverOptions{ .workDoneProgress = true };
        result.capabilities.completionProvider = lsp::CompletionOptions{ .workDoneProgress = true, .triggerCharacters = { { "." } }, .allCommitCharacters = { { ";" } }, .resolveProvider = false, .completionItem = lsp::CompletionOptionsCompletionItem{ true } };

        result.serverInfo = lsp::InitializeResultServerInfo{ .name = "GPULang Language Server", .version = "1.0" };
        return result;
    })
    .add<lsp::notifications::TextDocument_DidOpen>([&context, &messageHandler](lsp::notifications::TextDocument_DidOpen::Params&& params)
    {
        if (params.textDocument.uri.path().ends_with("gpul") || params.textDocument.uri.path().ends_with("gpuh"))
        {
            ParseContext::ParsedFile* file = ParseFile(params.textDocument.uri.path(), context, messageHandler);
            file->symbolsByLine.clear();
            file->semanticTokens.clear();
            Context prev;
            prev.loc.line = 0;
            prev.loc.start = 0;
            prev.carry = 0;
            for (auto sym : file->result.symbols)
            {
                if (sym->location.file == file->f->path)
                {
                    std::vector<const GPULang::Scope*> scopes{ file->result.intrinsicScope, file->result.mainScope };
                    CreateSemanticToken(prev, sym, file, file->semanticTokens, scopes);
                }
            }

            for (auto& lineSymbols : file->symbolsByLine)
            {
                std::sort(lineSymbols.second.begin(), lineSymbols.second.end(), [](const std::tuple<TextRange, PresentationBits, const GPULang::Symbol*>& lhs, const std::tuple<TextRange, PresentationBits, const GPULang::Symbol*>& rhs)
                {
                    auto& [lrange, _0, _1] = lhs;
                    auto& [rrange, _2, _3] = rhs;
                    return lrange.startColumn < rrange.startColumn;
                });
            }
        }
    })
    .add<lsp::notifications::TextDocument_DidClose>([&context, &messageHandler](lsp::notifications::TextDocument_DidClose::Params&& params)
    {
        ParseContext::ParsedFile* file = GetFile(params.textDocument.uri.path(), context, messageHandler);
        if (file != nullptr)
            Clear(file);
    })
    .add<lsp::notifications::TextDocument_DidSave>([&context, &messageHandler](lsp::notifications::TextDocument_DidSave::Params&& params)
    {
        if (params.textDocument.uri.path().ends_with("gpul") || params.textDocument.uri.path().ends_with("gpuh"))
        {
            ParseContext::ParsedFile* file = ParseFile(params.textDocument.uri.path(), context, messageHandler, true);
            file->symbolsByLine.clear();
            file->semanticTokens.clear();
            file->scopesByLine.clear();
            file->scopesByRange.clear();
            Context ctx;
            ctx.loc.line = 0;
            ctx.loc.start = 0;
            ctx.carry = 0;
            for (auto sym : file->result.symbols)
            {
                if (sym->location.file == file->f->path)
                {
                    std::vector<const GPULang::Scope*> scopes{ file->result.intrinsicScope, file->result.mainScope };
                    CreateSemanticToken(ctx, sym, file, file->semanticTokens, scopes);
                }
            }

            for (auto& lineSymbols : file->symbolsByLine)
            {
                std::sort(lineSymbols.second.begin(), lineSymbols.second.end(), [](const std::tuple<TextRange, PresentationBits, const GPULang::Symbol*>& lhs, const std::tuple<TextRange, PresentationBits, const GPULang::Symbol*>& rhs)
                {
                    auto& [lrange, _0, _1] = lhs;
                    auto& [rrange, _2, _3] = rhs;
                    return lrange.startColumn < rrange.startColumn;
                });
            }
        }
    })
    .add<lsp::notifications::TextDocument_DidChange>([&context, &messageHandler](lsp::notifications::TextDocument_DidChange::Params&& params)
    {
        if (params.textDocument.uri.path().ends_with("gpul") || params.textDocument.uri.path().ends_with("gpuh"))
        {
            ParseContext::ParsedFile* file = GetFile(params.textDocument.uri.path(), context, messageHandler);
            if (file != nullptr)
            {
                ValidateFile(file, context, std::get<lsp::TextDocumentContentChangeEvent_Text>(params.contentChanges[0]).text, messageHandler);
                file->symbolsByLine.clear();
                file->scopesByRange.clear();
                file->scopesByLine.clear();
                file->semanticTokens.clear();
                
                Context ctx;
                ctx.loc.line = 0;
                ctx.loc.start = 0;
                ctx.carry = 0;
                std::vector<const GPULang::Scope*> scopes{ file->result.intrinsicScope, file->result.mainScope };
                GPULang::Preprocessor* pp = nullptr;
                for (auto sym : file->result.symbols)
                {
                    if (sym->location.file == file->f->path)
                    {
                        CreateSemanticToken(ctx, sym, file, file->semanticTokens, scopes);
                    }
                }

                for (auto& lineSymbols : file->symbolsByLine)
                {
                    std::sort(lineSymbols.second.begin(), lineSymbols.second.end(), [](const std::tuple<TextRange, PresentationBits, const GPULang::Symbol*>& lhs, const std::tuple<TextRange, PresentationBits, const GPULang::Symbol*>& rhs)
                    {
                        auto& [lrange, _0, _1] = lhs;
                        auto& [rrange, _2, _3] = rhs;
                        return lrange.startColumn < rrange.startColumn;
                    });
                }
            }
        }
    })
    .add<lsp::requests::TextDocument_SemanticTokens_Full>([&context, &messageHandler](const lsp::jsonrpc::MessageId& id, lsp::requests::TextDocument_SemanticTokens_Full::Params&& params)
    {
        lsp::requests::TextDocument_SemanticTokens_Full::Result result;
        const std::string& path = params.textDocument.uri.path();
        ParseContext::ParsedFile* file = GetFile(path, context, messageHandler);
        if (file != nullptr)
            result = lsp::SemanticTokens{ .data = file->semanticTokens };
        else
            result = lsp::SemanticTokens{ };

        return result;
    })
    .add<lsp::requests::TextDocument_SemanticTokens_Range>([&context](const lsp::jsonrpc::MessageId& id, lsp::requests::TextDocument_SemanticTokens_Range::Params&& params)
    {
        lsp::requests::TextDocument_SemanticTokens_Range::Result result;

        return result;
    })
    .add<lsp::requests::TextDocument_Hover>([&context, &messageHandler](const lsp::jsonrpc::MessageId& id, lsp::requests::TextDocument_Hover::Params&& params)
    {
        lsp::requests::TextDocument_Hover::Result result;
        ParseContext::ParsedFile* file = GetFile(params.textDocument.uri.path(), context, messageHandler);
        if (file != nullptr)
        {

            auto it = file->symbolsByLine.find(params.position.line);
            if (it != file->symbolsByLine.end())
            {
                const GPULang::Symbol* closestSymbol = nullptr;
                PresentationBits presentationBits(0x0);
                for (auto& sym : it->second)
                {
                    auto& [range, bits, symbol] = sym;
                    if (range.startColumn <= params.position.character)
                    {
                        if (params.position.character > range.stopColumn)
                            continue;
                        closestSymbol = symbol;
                        presentationBits = bits;
                    }
                }
                presentationBits.flags.symbolLookup = true;

                if (closestSymbol != nullptr)
                {
                     result = lsp::requests::TextDocument_Hover::Result {
                        lsp::Hover {
                            .contents = lsp::MarkupContent{
                                .kind = lsp::MarkupKind::Markdown,
                                .value = GPULang::Format("\n%s\n", CreateMarkdown(closestSymbol, presentationBits).c_str())
                            },
                            .range = lsp::Range{
                                .start = {
                                    .line = (uint32_t)closestSymbol->location.line,
                                    .character = (uint32_t)closestSymbol->location.start
                                },
                                .end = {
                                    .line = (uint32_t)closestSymbol->location.line,
                                    .character = (uint32_t)closestSymbol->location.end
                                }
                            }
                        }
                    };
                }
            }
        }

        return result;
    })
    .add<lsp::requests::TextDocument_References>([&context, &messageHandler](const lsp::jsonrpc::MessageId& id, lsp::requests::TextDocument_References::Params&& params)
    {
        lsp::requests::TextDocument_References::Result result;
        ParseContext::ParsedFile* file = GetFile(params.textDocument.uri.path(), context, messageHandler);
        if (file != nullptr)
        {
            TextRange inputRange;
            inputRange.startLine = params.position.line;
            inputRange.startColumn = params.position.character;
            const auto symbolsOnLine = file->symbolsByLine[params.position.line];
            const GPULang::Symbol* sym = nullptr;
            for (const auto& [range, bits, lineSym] : symbolsOnLine)
            {
                if (lineSym->location.start < params.position.character && lineSym->location.end > params.position.character)
                    sym = lineSym;
                else if (lineSym->location.start > params.position.character)
                    break;
            }
            if (sym != nullptr)
            {
                if (sym->symbolType == GPULang::Symbol::SymbolType::CallExpressionType)
                {
                    auto* expr = static_cast<const GPULang::CallExpression*>(sym);
                    auto* res = GPULang::Symbol::Resolved(expr);
                    if (res->function->location.line != -1)
                    {
                        lsp::Location link;
                        link.uri = res->function->location.file.c_str();
                        link.range.start.line = res->function->location.line;
                        link.range.start.character = res->function->location.start;
                        link.range.end.line = res->function->location.line;
                        link.range.end.character = res->function->location.end;
                        result = lsp::requests::TextDocument_References::Result {
                            { link }
                        };
                    }
                }
                else if (sym->symbolType == GPULang::Symbol::SymbolType::SymbolExpressionType)
                {
                    auto* expr = static_cast<const GPULang::SymbolExpression*>(sym);
                    auto* res = GPULang::Symbol::Resolved(expr);
                    if (res->symbol->location.line != -1)
                    {
                        lsp::Location link;
                        link.uri = res->symbol->location.file.c_str();
                        link.range.start.line = res->symbol->location.line;
                        link.range.start.character = res->symbol->location.start;
                        link.range.end.line = res->symbol->location.line;
                        link.range.end.character = res->symbol->location.end;
                        result = lsp::requests::TextDocument_References::Result {
                            { link }
                        };
                    }
                }
            }
        }
            
        return result;
    })
    .add<lsp::requests::TextDocument_DocumentHighlight>([&context, &messageHandler](const lsp::jsonrpc::MessageId& id, lsp::requests::TextDocument_DocumentHighlight::Params&& params)
    {
        lsp::requests::TextDocument_DocumentHighlight::Result result;
        ParseContext::ParsedFile* file = GetFile(params.textDocument.uri.path(), context, messageHandler);
        if (file != nullptr)
        {
            TextRange inputRange;
            inputRange.startLine = params.position.line;
            inputRange.startColumn = params.position.character;
            const auto symbolsOnLine = file->symbolsByLine[params.position.line];
            const GPULang::Symbol* sym = nullptr;
            for (const auto& [range, bits, lineSym] : symbolsOnLine)
            {
                if (lineSym != nullptr)
                {
                    if (lineSym->location.start < params.position.character && lineSym->location.end > params.position.character)
                        sym = lineSym;
                    else if (lineSym->location.start > params.position.character)
                        break;
                }
            }
            if (sym != nullptr)
            {
                result = lsp::requests::TextDocument_DocumentHighlight::Result {
                    {
                        lsp::DocumentHighlight{
                            .range = {
                                .start = {
                                    .line = (uint32_t)sym->location.line,
                                    .character = (uint32_t)sym->location.start
                                },
                                .end = {
                                    .line = (uint32_t)sym->location.line,
                                    .character = (uint32_t) sym->location.end
                                }
                            },
                            .kind = lsp::DocumentHighlightKind::Write
                        }
                    }
                };
            }
        }
        return result;
    })
    .add<lsp::requests::TextDocument_Definition>([&context, &messageHandler](const lsp::jsonrpc::MessageId& id, lsp::requests::TextDocument_Definition::Params&& params)
    {
            lsp::requests::TextDocument_Definition::Result result;
            const std::string& path = params.textDocument.uri.path();
            ParseContext::ParsedFile* file = GetFile(path, context, messageHandler);
            
            TextRange inputRange;
            inputRange.startLine = params.position.line;
            inputRange.startColumn = params.position.character;
            const auto symbolsOnLine = file->symbolsByLine[params.position.line];
            const GPULang::Symbol* sym = nullptr;
            for (const auto& [range, bits, lineSym] : symbolsOnLine)
            {
                if (lineSym != nullptr)
                {
                    if (lineSym->location.start < params.position.character && lineSym->location.end > params.position.character)
                        sym = lineSym;
                    else if (lineSym->location.start > params.position.character)
                        break;
                }
            }
            if (sym != nullptr)
            {
                if (sym->symbolType == GPULang::Symbol::SymbolType::SymbolExpressionType)
                {
                    const auto expr = static_cast<const GPULang::SymbolExpression*>(sym);
                    GPULang::Symbol* symbol = GPULang::Symbol::Resolved(expr)->symbol;
                    if (symbol != nullptr)
                    {
                        if (symbol->location.line != -1)
                        {
                            lsp::DefinitionLink link;
                            link.targetUri = symbol->location.file.c_str();
                            lsp::Range range;
                            range.start.line = symbol->location.line;
                            range.start.character = symbol->location.start;
                            range.end.line = symbol->location.line;
                            range.end.character = symbol->location.end;
                            link.targetRange = range;
                            link.targetSelectionRange = range;
                            result = std::vector{ link };
                        }
                    }
                }
                else
                {
                    lsp::Range sourceRange;
                    sourceRange.start.line = sym->location.line;
                    sourceRange.start.character = sym->location.start;
                    sourceRange.end.line = sym->location.line;
                    sourceRange.end.character = sym->location.end;
                    lsp::Definition def = lsp::Location{ .uri = sym->location.file.c_str(), .range = sourceRange };
                    result = def;
                }
            }
            return result;
    })
    .add<lsp::requests::TextDocument_Completion>([&context, &messageHandler](const lsp::jsonrpc::MessageId& id, lsp::requests::TextDocument_Completion::Params&& params)
    {
        lsp::requests::TextDocument_Completion::Result result;
        ParseContext::ParsedFile* file = GetFile(params.textDocument.uri.path(), context, messageHandler);

        static auto reverse_identifier_search = [](const char* begin, const char* end) -> const char*
        {
            const char* start = end;
            while (start != begin)
            {
                if (!GPULangValidIdentifierChar(start[0]))
                    return start+1;
                start--;
            }
            return end;
        };
        
        static auto forward_identifier_search = [](const char* begin, const char* end) -> const char*
        {
            const char* start = begin;
            while (start != end)
            {
                if (!GPULangValidIdentifierChar(start[0]))
                    return start;
                start++;
            }
            return end;
        };
            
        if (file != nullptr)
        {
            TextRange inputRange;
            
            enum LookupType
            {
                Symbol,
                Member
            } completionType = LookupType::Symbol;
            if (params.context)
            {
                if (params.context->triggerKind == lsp::CompletionTriggerKind::TriggerCharacter)
                {
                    if (params.context->triggerCharacter == ".")
                    {
                        params.position.character -= 1;
                        completionType = LookupType::Member;
                    }
                }
            }
            
            inputRange.startLine = params.position.line;
            inputRange.startColumn = params.position.character;
            auto it = file->textByLine.find(params.position.line);
            if (it != file->textByLine.end())
            {
                auto scopeIt = file->scopesByRange.begin();
                std::vector<const GPULang::Scope*>* scopes = nullptr;
                int distanceFromSymbol = INT_MAX;
                while (scopeIt != file->scopesByRange.end())
                {
                    int distance = int(params.position.line - scopeIt->first.startLine);
                    if (distance < 0) // If distance is negative, we passed by the scope
                        break;
                    if (scopeIt->first.startLine <= params.position.line && scopeIt->first.stopLine >= params.position.line)
                    {
                        if (distance < distanceFromSymbol)
                        {
                            scopes = &scopeIt->second;
                            distanceFromSymbol = distance;
                        }
                    }
                    
                    scopeIt++;
                }
                if (scopes != nullptr)
                {
                    const char* begin = it->second.data() + params.position.character;
                    const char* eol = it->second.data() + it->second.length();
                    const char* identifierBegin = reverse_identifier_search(it->second.data(), begin-1);
                    std::string_view ident = std::string_view (identifierBegin, begin);
                 
                    const std::set<GPULang::Symbol*> symbols = ParseContext::ParsedFile::FindSymbols(ident, *scopes);
                    std::vector<lsp::CompletionItem> items;
                    for (auto& sym : symbols)
                    {
                        switch (sym->symbolType)
                        {
                        case GPULang::Symbol::SymbolType::FunctionType:
                        {
                            auto fun = static_cast<GPULang::Function*>(sym);
                            auto res = GPULang::Symbol::Resolved(fun);
                            if (res->name.buf != nullptr)
                            {
                                items.push_back(lsp::CompletionItem{ .label = res->name.c_str(), .kind = lsp::CompletionItemKind::Function, .insertText = sym->name.c_str(), .commitCharacters = {{"("}} });
                            }
                            break;
                        }
                        case GPULang::Symbol::SymbolType::EnumerationType:
                        {
                            auto enu = static_cast<GPULang::Enumeration*>(sym);
                            const char* memberEnd = forward_identifier_search(begin+1, eol);
                            std::string_view member = std::string_view(begin+1, memberEnd);
                            
                            for (const auto& label : enu->labels)
                            {
                                if (label.StartsWith(member))
                                {
                                    items.push_back(lsp::CompletionItem{ .label = label.c_str(), .labelDetails = lsp::CompletionItemLabelDetails{.description = enu->name.c_str()}, .kind = lsp::CompletionItemKind::EnumMember, .commitCharacters = {{"."}} });
                                }
                            }
                            break;
                        }
                        case GPULang::Symbol::SymbolType::VariableType:
                        {
                            auto var = static_cast<GPULang::Variable*>(sym);
                            auto res = GPULang::Symbol::Resolved(var);
                            if (completionType == LookupType::Member)
                            {
                                const char* memberEnd = forward_identifier_search(begin+1, eol);
                                std::string_view member = std::string_view(begin+1, memberEnd);
                                GPULang::Type* type = res->typeSymbol;
                                for (const auto tySym : type->symbols)
                                {
                                    if (tySym->name.StartsWith(member))
                                    {
                                        items.push_back(lsp::CompletionItem{ .label = tySym->name.c_str(), .labelDetails = lsp::CompletionItemLabelDetails{.description = type->name.c_str()}, .kind = lsp::CompletionItemKind::Field, .commitCharacters = {{"."}} });
                                    }
                                }
                            }
                            else
                            {
                                items.push_back(lsp::CompletionItem{ .label = sym->name.c_str(), .labelDetails = lsp::CompletionItemLabelDetails{.description = var->type.name.c_str()}, .kind = lsp::CompletionItemKind::Variable, .commitCharacters = {{"."}} });
                            }
                            break;
                        }
                        default:
                            items.push_back(lsp::CompletionItem{.label = sym->name.c_str(), .kind = lsp::CompletionItemKind::Keyword});
                        }
                        
                    }
                
                    //std::string_view text(it->second.data() + params.position.character, it->second.data() + params.position.character + params.)
                    result = lsp::requests::TextDocument_Completion::Result {
                            items
                    };
                }
            }
        }
        return result;
    })
    .add<lsp::notifications::Exit>([&running]()
    {
        running = false;
    });

    while (running)
    {
        try
        {
            messageHandler.processIncomingMessages();
        }
        catch (std::exception e)
        {
            printf("%s\n", e.what());
            running = false;
            SocketClose(client);
        }
    }


    
    return 0;
}
