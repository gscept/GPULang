#include <lsp/messages.h>
#include "lsp/connection.h"
#include "lsp/messagehandler.h"
#include "lsp/io/standardio.h"
#include "gpulangcompiler.h"

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
        GPULang::InitAllocator(&it->second.alloc);
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

                lsp::SemanticTokensLegend legend{
                    .tokenTypes = {
                        "type",
                        "class",
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
                result.capabilities.workspace = lsp::ServerCapabilitiesWorkspace{ .workspaceFolders = lsp::WorkspaceFoldersServerCapabilities{ .supported = true } };
                result.capabilities.documentHighlightProvider = lsp::DocumentHighlightOptions{ .workDoneProgress = true };
                result.capabilities.referencesProvider = lsp::ReferenceOptions{ .workDoneProgress = true };
                result.capabilities.declarationProvider = lsp::DeclarationOptions{ .workDoneProgress = true };
                result.capabilities.definitionProvider = lsp::DefinitionOptions{ .workDoneProgress = true };
                result.capabilities.semanticTokensProvider = lsp::SemanticTokensOptions{ .legend = legend, .range = false, .full = true };
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
                ParseFile(path, context, messageHandler);
                result = lsp::SemanticTokens{ .data = {0, 0, 0} };

                return result;
            })
                .add<lsp::requests::TextDocument_SemanticTokens_Range>([&context](const lsp::jsonrpc::MessageId& id, lsp::requests::TextDocument_SemanticTokens_Range::Params&& params)
            {
                lsp::requests::TextDocument_SemanticTokens_Range::Result result;

                return result;
            })
                .add<lsp::requests::TextDocument_Hover>([&context](const lsp::jsonrpc::MessageId& id, lsp::requests::TextDocument_Hover::Params&& params)
            {
                lsp::requests::TextDocument_Hover::Result result;
                if (params.textDocument.uri.path().ends_with("gpul"))
                {
                    GPULang::Compiler::Options options;
                    //GPULangValidate(params.textDocument.uri.path(), context->includePaths, options, context->result);
                }

                return result;
            })
                .add<lsp::requests::TextDocument_DocumentHighlight>([&context](const lsp::jsonrpc::MessageId& id, lsp::requests::TextDocument_DocumentHighlight::Params&& params)
            {
                lsp::requests::TextDocument_DocumentHighlight::Result result;
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
