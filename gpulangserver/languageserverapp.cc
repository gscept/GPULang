#include <lsp/messages.h>
#include "lsp/connection.h"
#include "lsp/messagehandler.h"
#include "lsp/io/standardio.h"
#include "gpulangcompiler.h"
#include "ast/function.h"
#include "ast/variable.h"
#include "ast/enumeration.h"
#include "ast/structure.h"
#include "ast/statements/scopestatement.h"
#include "ast/statements/expressionstatement.h"
#include "ast/expressions/callexpression.h"
#include "ast/expressions/symbolexpression.h"

#if _MSC_VER
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#endif

#include <map>
#include <filesystem>
#include <chrono>
#include <streambuf>
#include "memory.h"

class SocketBuffer : public std::basic_streambuf<char>
{
public:
    SocketBuffer(SOCKET sock) :
        sock(sock)
    {}

    std::streamsize xsputn(const char* buf, std::streamsize count)
    {
        int sent = send(this->sock, buf, count, 0);
        while (sent != count)
            sent = send(this->sock, buf + sent, count, 0);
        return count;
    }

    int underflow()
    {
        int size = recv(this->sock, this->buf, sizeof(buf), 0);
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
    SOCKET sock;
};

struct ParseContext
{
    std::vector<std::string> includePaths;
    struct ParsedFile
    {
        GPULangServerResult result;
        GPULang::Allocator alloc;


        struct TextRange
        {
            size_t startLine, stopLine;
            size_t startColumn, stopColumn;
        };

        std::vector<std::pair<TextRange, const GPULang::Symbol*>> symbolsByRange;
        std::map<size_t, std::vector<std::pair<TextRange, const GPULang::Symbol*>>> symbolsByLine;
    };
    std::map<std::string, ParsedFile> parsedFiles;
};

//------------------------------------------------------------------------------
/**
*/
ParseContext::ParsedFile* 
ParseFile(const std::string path, ParseContext* context, lsp::MessageHandler& messageHandler)
{
    auto it = context->parsedFiles.find(path);
    if (it == context->parsedFiles.end())
    {
        it = context->parsedFiles.insert({ path, ParseContext::ParsedFile() }).first;
        it->second.alloc = GPULang::CreateAllocator();
    }
    for (auto* symbol : it->second.result.symbols)
    {
        symbol->~Symbol();
    }
    GPULang::Compiler::Options options;
    GPULang::ResetAllocator(&it->second.alloc);
    GPULang::MakeAllocatorCurrent(&it->second.alloc);
    GPULangValidate(path, context->includePaths, options, it->second.result);
    if (!it->second.result.diagnostics.empty())
    {
        std::vector<lsp::Diagnostic> diagnostics;
        for (auto& diagnostic : it->second.result.diagnostics)
        {
            diagnostics.push_back(lsp::Diagnostic{
                .range = {
                    .start = { .line = (uint32_t)diagnostic.line - 1, .character = (uint32_t)diagnostic.column },
                    .end = { .line = (uint32_t)diagnostic.line - 1, .character = (uint32_t)diagnostic.column + diagnostic.length }
                },
                .message = diagnostic.error
            });
        }
        it->second.result.diagnostics.clear();
        messageHandler.messageDispatcher().sendNotification<lsp::notifications::TextDocument_PublishDiagnostics>(
            lsp::notifications::TextDocument_PublishDiagnostics::Params
            {
                .uri = path,
                .diagnostics = diagnostics
            }
        );
    }
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
            "defaultLibrary"
    }
};

enum class SemanticTypeMapping : uint32_t {
    Enum
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
    Declaration = 0x1
    , Definition = 0x2
    , ReadOnly = 0x4
    , Static = 0x8
    , Depreacted = 0x10
    , Documentation = 0x20
    , DefaultLibrary = 0x40
};

//------------------------------------------------------------------------------
/**
*/
void
InsertSemanticToken(GPULang::Symbol::Location& prev, const GPULang::Symbol::Location& loc, SemanticTypeMapping type, uint32_t modifiers, std::vector<uint32_t>& result)
{
    uint32_t ret[5];
    ret[0] = (loc.line - prev.line);
    ret[1] = loc.line == prev.line ? loc.column - prev.column : loc.column;
    ret[2] = loc.end - loc.start;
    ret[3] = (uint32_t)type;
    ret[4] = modifiers;
    prev = loc;
    result.insert(result.end(), ret, ret + 5);
}

//------------------------------------------------------------------------------
/**
*/
void
CreateSemanticToken(GPULang::Symbol::Location& prevLoc, const GPULang::Symbol* sym, ParseContext::ParsedFile* file, std::vector<uint32_t>& result)
{
    ParseContext::ParsedFile::TextRange range;
    range.startLine = sym->location.line;
    range.stopLine = sym->location.line;
    range.startColumn = sym->location.start;
    range.stopColumn = sym->location.end;
    file->symbolsByRange.push_back(std::make_pair(range, sym));
    file->symbolsByLine[range.startLine-1].push_back(std::make_pair(range, sym));

    switch (sym->symbolType)
    {
        case GPULang::Symbol::SymbolType::VariableType:
        {
            const GPULang::Variable* var = static_cast<const GPULang::Variable*>(sym);
            const GPULang::Variable::__Resolved* res = GPULang::Symbol::Resolved(var);
            for (auto attr : var->attributes)
            {
                InsertSemanticToken(prevLoc, attr->location, SemanticTypeMapping::Keyword, 0x0, result);
            }
            if (res->usageBits.flags.isParameter)
                InsertSemanticToken(prevLoc, sym->location, SemanticTypeMapping::Parameter, (uint32_t)SemanticModifierMapping::Definition, result);
            else
                InsertSemanticToken(prevLoc, sym->location, SemanticTypeMapping::Variable, (uint32_t)SemanticModifierMapping::Definition, result);

            if (var->valueExpression != nullptr)
                CreateSemanticToken(prevLoc, var->valueExpression, file, result);
            break;
        }
        case GPULang::Symbol::SymbolType::FunctionType:
        {
            const GPULang::Function* fun = static_cast<const GPULang::Function*>(sym);
            for (auto attr : fun->attributes)
            {
                InsertSemanticToken(prevLoc, attr->location, SemanticTypeMapping::Keyword, 0x0, result);
            }

            InsertSemanticToken(prevLoc, sym->location, SemanticTypeMapping::Function, (uint32_t)SemanticModifierMapping::Definition, result);

            for (auto var : fun->parameters)
            {
                CreateSemanticToken(prevLoc, var, file, result);
            }

            if (fun->ast != nullptr)
            {
                CreateSemanticToken(prevLoc, fun->ast, file, result);
            }
            break;
        }
        case GPULang::Symbol::SymbolType::EnumerationType:
        {
            const GPULang::Enumeration* enu= static_cast<const GPULang::Enumeration*>(sym);
            InsertSemanticToken(prevLoc, sym->location, SemanticTypeMapping::Enum, (uint32_t)SemanticModifierMapping::Definition, result);

            for (auto mem : enu->labels)
            {

            }
            break;
        }
        case GPULang::Symbol::SymbolType::ScopeStatementType:
        {
            const GPULang::ScopeStatement* scope = static_cast<const GPULang::ScopeStatement*>(sym);
            for (auto innerSym : scope->symbols)
                CreateSemanticToken(prevLoc, innerSym, file, result);
            break;
        }
        case GPULang::Symbol::SymbolType::ExpressionStatementType:
        {
            const GPULang::ExpressionStatement* stat = static_cast<const GPULang::ExpressionStatement*>(sym);
            CreateSemanticToken(prevLoc, stat->expr, file, result);
            break;
        }
        case GPULang::Symbol::SymbolType::CallExpressionType:
        {
            const GPULang::CallExpression* call = static_cast<const GPULang::CallExpression*>(sym);
            InsertSemanticToken(prevLoc, sym->location, SemanticTypeMapping::Function, 0x0, result);
            for (auto var : call->args)
                CreateSemanticToken(prevLoc, var, file, result);
            break;
        }
        case GPULang::Symbol::SymbolType::SymbolExpressionType:
        {
            break;
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
std::string
CreateMarkdown(const GPULang::Symbol* sym, bool header = false)
{
    std::string ret = "";
    if (header)
        ret = GPULang::Format("### %s\n", sym->name.c_str());
    switch (sym->symbolType)
    {
        case GPULang::Symbol::SymbolType::VariableType:
        {
            const GPULang::Variable* var = static_cast<const GPULang::Variable*>(sym);
            for (auto attr : var->attributes)
            {
                ret += GPULang::Format("*%s*", attr->name.c_str());
                if (attr->expression != nullptr)
                    ret += GPULang::Format("(%s) ", attr->expression->EvalString().c_str());
                else
                    ret += " ";
            }
            ret += "**variable**\n\n";
            ret += sym->documentation + "\n";
            break;
        }
        case GPULang::Symbol::SymbolType::FunctionType:
        {
            const GPULang::Function* fun = static_cast<const GPULang::Function*>(sym);
            for (auto attr : fun->attributes)
            {
                ret += GPULang::Format("*%s*", attr->name.c_str());
                if (attr->expression != nullptr)
                    ret += GPULang::Format("(%s) ", attr->expression->EvalString().c_str());
                else
                    ret += " ";
            }
            ret += "**function**\n\n";
            ret += sym->documentation + "\n";

            for (auto var : fun->parameters)
            {
                ret += CreateMarkdown(var);
            }
            break;
        }
        case GPULang::Symbol::SymbolType::EnumerationType:
        {
            const GPULang::Enumeration* enu = static_cast<const GPULang::Enumeration*>(sym);

            for (auto mem : enu->labels)
            {

            }
            break;
        }
        case GPULang::Symbol::SymbolType::StructureType:
        {
            const GPULang::Enumeration* enu = static_cast<const GPULang::Enumeration*>(sym);
            break;
        }
        case GPULang::Symbol::SymbolType::ExpressionStatementType:
        {
            const GPULang::ExpressionStatement* stat = static_cast<const GPULang::ExpressionStatement*>(sym);
            ret += CreateMarkdown(stat->expr);
            break;
        }
        case GPULang::Symbol::SymbolType::CallExpressionType:
        {
            const GPULang::CallExpression* lookup = static_cast<const GPULang::CallExpression*>(sym);
            const GPULang::CallExpression::__Resolved* res = GPULang::Symbol::Resolved(lookup);
            const GPULang::Function::__Resolved* funRes = GPULang::Symbol::Resolved(res->function);
            ret += funRes->name + "\n";
            ret += CreateMarkdown(res->function);

            for (auto arg : lookup->args)
                ret += CreateMarkdown(arg);
            break;
        }
        case GPULang::Symbol::SymbolType::SymbolExpressionType:
        {
            const GPULang::SymbolExpression* lookup = static_cast<const GPULang::SymbolExpression*>(sym);
            const GPULang::SymbolExpression::__Resolved* res = GPULang::Symbol::Resolved(lookup);
            ret += CreateMarkdown(res->symbol);
            break;
        }
    }
    return ret;
}


//------------------------------------------------------------------------------
/**
*/
int __cdecl
main(int argc, const char** argv)
{
#if _MSC_VER
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed.\n");
        exit(1);
    }

    if (LOBYTE(wsaData.wVersion) != 2 ||
        HIBYTE(wsaData.wVersion) != 2)
    {
        fprintf(stderr,"Version 2.2 of Winsock not available.\n");
        WSACleanup();
        exit(2);
    }
#endif
    
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(5007);
    bind(sock, reinterpret_cast<struct sockaddr*>(&server), sizeof(server));
    listen(sock, 5);


#if _MSC_VER
    //DWORD timeout = 1000;
    //setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof timeout);
#endif


    printf("GPULang Language Server Version 1.0\n");
    printf("Waiting for clients...\n");
    std::map<int, ParseContext> parseContexts;

    while (true)
    {
        SOCKET sockfd = accept(sock, nullptr, nullptr);
        std::vector<std::thread*> threads;
        auto clientThread = GPULang::Alloc<std::thread>([sockfd, &parseContexts]()
        {
            printf("Connection established\n");
            SocketBuffer inputBuffer(sockfd);
            SocketBuffer outputBuffer(sockfd);
            std::istream input(&inputBuffer);
            std::ostream output(&outputBuffer);
            lsp::Connection connection{ input, output };

            lsp::MessageHandler messageHandler{ connection };
            bool running = true;
            ParseContext* context = nullptr;

            messageHandler.requestHandler()
                .add<lsp::requests::Initialize>([&parseContexts, &context](const lsp::jsonrpc::MessageId& id, lsp::requests::Initialize::Params&& params)
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

                if (!params.workspaceFolders->isNull())
                {
                    for (const auto& workspaceFolder : params.workspaceFolders->value())
                    {
                        context->includePaths.push_back("-I" + workspaceFolder.uri.path() + "/");
                        for (const auto& it : std::filesystem::directory_iterator(workspaceFolder.uri.path()))
                        {
                            if (it.is_directory())
                            {
                                context->includePaths.push_back("-I" + it.path().string() + "/");
                            }
                        }
                    }
                }

                result.capabilities.workspace = lsp::ServerCapabilitiesWorkspace{ .workspaceFolders = lsp::WorkspaceFoldersServerCapabilities{ .supported = true } };
                result.capabilities.documentHighlightProvider = lsp::DocumentHighlightOptions{ .workDoneProgress = true };
                result.capabilities.referencesProvider = lsp::ReferenceOptions{ .workDoneProgress = true };
                result.capabilities.declarationProvider = lsp::DeclarationOptions{ .workDoneProgress = true };
                result.capabilities.definitionProvider = lsp::DefinitionOptions{ .workDoneProgress = true };
                result.capabilities.semanticTokensProvider = lsp::SemanticTokensOptions{ .legend = legend, .range = false, .full = true };
                result.capabilities.hoverProvider = lsp::HoverOptions{ .workDoneProgress = true };
                result.serverInfo = lsp::InitializeResultServerInfo{ .name = "GPULang Language Server", .version = "1.0" };
                return result;
            })
                .add<lsp::notifications::TextDocument_DidOpen>([&context](lsp::notifications::TextDocument_DidOpen::Params&& params)
            {
                if (params.textDocument.uri.path().ends_with("gpul"))
                {
                    GPULang::Compiler::Options options;
                    //GPULangValidate(params.textDocument.uri.path(), context->includePaths, options, context->result);
                }
            })
                .add<lsp::notifications::TextDocument_DidChange>([&context, &messageHandler](lsp::notifications::TextDocument_DidChange::Params&& params)
            {
                if (params.textDocument.uri.path().ends_with("gpul"))
                {
                    ParseFile(params.textDocument.uri.path(), context, messageHandler);
                }

            })
                .add<lsp::notifications::Workspace_DidChangeWatchedFiles>([&context, &messageHandler](lsp::notifications::Workspace_DidChangeWatchedFiles::Params&& params)
            {
                for (auto& event : params.changes)
                {
                    GPULang::Compiler::Options options;
                    switch (event.type)
                    {
                        case lsp::FileChangeType::Created:
                        case lsp::FileChangeType::Changed:
                            ParseFile(event.uri.path(), context, messageHandler);
                            break;
                        case lsp::FileChangeType::Deleted:
                            break;
                    }
                }
            })
                .add<lsp::requests::TextDocument_SemanticTokens_Full>([&context, &messageHandler](const lsp::jsonrpc::MessageId& id, lsp::requests::TextDocument_SemanticTokens_Full::Params&& params)
            {
                lsp::requests::TextDocument_SemanticTokens_Full::Result result;
                const std::string& path = params.textDocument.uri.path();
                ParseContext::ParsedFile* file = ParseFile(path, context, messageHandler);
                file->symbolsByRange.clear();
                file->symbolsByLine.clear();
                std::vector<uint32_t> tokens;
                GPULang::Symbol::Location prev;
                prev.line = 1;
                prev.column = 0;
                for (auto sym : file->result.symbols)
                {
                    if (sym->location.file == path)
                    {
                        CreateSemanticToken(prev, sym, file, tokens);
                    }
                }

                for (auto lineSymbols : file->symbolsByLine)
                {
                    std::sort(lineSymbols.second.begin(), lineSymbols.second.end(), [](const std::pair<ParseContext::ParsedFile::TextRange, const GPULang::Symbol*>& lhs, const std::pair<ParseContext::ParsedFile::TextRange, const GPULang::Symbol*>& rhs)
                    {
                        return lhs.first.startColumn < rhs.first.startColumn;
                    });
                }
                result = lsp::SemanticTokens{ .data = tokens };

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
                    ParseContext::ParsedFile::TextRange inputRange;
                    inputRange.startLine = params.position.line;
                    inputRange.startColumn = params.position.character;
                    auto it = file->symbolsByLine.find(params.position.line);
                    if (it != file->symbolsByLine.end())
                    {
                        for (auto& sym : it->second)
                        {
                            if (sym.second->location.start <= params.position.character && sym.second->location.end >= params.position.character)
                            {
                                result = lsp::requests::TextDocument_Hover::Result {
                                    lsp::Hover {
                                        .contents = lsp::MarkupContent{
                                            .kind = lsp::MarkupKind::Markdown,
                                            .value = CreateMarkdown(sym.second, true)
                                        },
                                        .range = lsp::Range{ 
                                            .start = { 
                                                .line = (uint32_t)sym.second->location.line - 1,
                                                .character = (uint32_t)sym.second->location.start
                                            }, 
                                            .end = {
                                                .line = (uint32_t)sym.second->location.line - 1,
                                                .character = (uint32_t)sym.second->location.end
                                            }
                                        }
                                    }
                                };
                                break;
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
                    ParseContext::ParsedFile::TextRange inputRange;
                    inputRange.startLine = params.position.line;
                    inputRange.startColumn = params.position.character;
                    auto it = std::upper_bound(file->symbolsByRange.begin(), file->symbolsByRange.end(), std::make_pair(inputRange, nullptr), [](const std::pair<ParseContext::ParsedFile::TextRange, const GPULang::Symbol*>& lhs, const std::pair<ParseContext::ParsedFile::TextRange, const GPULang::Symbol*>& rhs)
                    {
                        if (lhs.first.startLine == rhs.first.startLine)
                            return lhs.first.startColumn < rhs.first.startColumn;
                        else
                            return lhs.first.startLine < rhs.first.startLine;
                    });
                    if (it != file->symbolsByRange.end())
                    {
                        result = lsp::requests::TextDocument_DocumentHighlight::Result { 
                            { 
                                lsp::DocumentHighlight{ 
                                    .range = { 
                                        .start = { 
                                            .line = (uint32_t)it->second->location.line - 1, 
                                            .character = (uint32_t)it->second->location.start
                                        }, 
                                        .end = {
                                            .line = (uint32_t)it->second->location.line - 1, 
                                            .character = (uint32_t) + it->second->location.end
                                        }
                                    },
                                    .kind = lsp::DocumentHighlightKind::Write
                                }
                            }
                        };

                        int foo = 5;
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
                    shutdown(sockfd, 0);
                }
                using namespace std::chrono_literals;
                std::this_thread::sleep_for(1000ms);
            }
        });
        threads.push_back(clientThread);
    }

    return 0;
}
