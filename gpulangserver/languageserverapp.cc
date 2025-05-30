#include <lsp/messages.h>
#include "lsp/connection.h"
#include "lsp/messagehandler.h"
#include "lsp/io/standardio.h"
#include "gpulangcompiler.h"
#include "ast/function.h"
#include "ast/variable.h"
#include "ast/enumeration.h"
#include "ast/structure.h"
#include "ast/renderstate.h"
#include "ast/samplerstate.h"
#include "ast/types/type.h"
#include "ast/types/renderstatetype.h"
#include "ast/types/samplerstatetype.h"
#include "ast/statements/scopestatement.h"
#include "ast/statements/ifstatement.h"
#include "ast/statements/forstatement.h"
#include "ast/statements/whilestatement.h"
#include "ast/statements/expressionstatement.h"
#include "ast/expressions/callexpression.h"
#include "ast/expressions/symbolexpression.h"
#include "ast/expressions/binaryexpression.h"
#include "ast/expressions/accessexpression.h"
#include "ast/expressions/arrayindexexpression.h"

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

union PresentationBits
{
    struct Flags
    {
        uint32_t symbolLookup : 1; // Presentation is according to a symbol being looked up
        uint32_t typeLookup : 1;   // Presentation is akin to a type being looked up
    } flags;
    uint32_t bits;

    PresentationBits(uint32_t bits) : bits(bits) {};
    PresentationBits(Flags flags) : flags(flags) {};
};

struct ParseContext
{
    std::vector<std::string> includePaths;
    struct ParsedFile
    {
        GPULangServerResult result;
        GPULang::Allocator alloc;

        std::string path;
        std::string file;
        std::FILE* tmpFile;
        std::string tmpPath;
        struct TextRange
        {
            size_t startLine, stopLine;
            size_t startColumn, stopColumn;
        };

        std::vector<std::pair<TextRange, const GPULang::Symbol*>> symbolsByRange;
        std::map<size_t, std::vector<std::tuple<TextRange, PresentationBits, const GPULang::Symbol*>>> symbolsByLine;
        std::map<std::string, const GPULang::Symbol*> symbolsByName;

        static const GPULang::Symbol* FindSymbol(const std::string& name, const std::vector<const GPULang::Scope*> scopes)
        {
            auto scopeIt = scopes.rbegin();
            while (scopeIt != scopes.rend())
            {
                auto symIt = (*scopeIt)->symbolLookup.find(name);
                if (symIt != (*scopeIt)->symbolLookup.end())
                {
                    return symIt->second;
                }
                scopeIt++;
            }
            return nullptr;
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
Clear(GPULangServerResult& result, GPULang::Allocator& allocator)
{
    if (result.intrinsicScope != nullptr)
        result.intrinsicScope->~Scope();
    result.intrinsicScope = nullptr;
    if (result.mainScope != nullptr)
        result.mainScope->~Scope();
    result.mainScope = nullptr;
    for (auto* symbol : result.symbols)
    {
        if (symbol->symbolType != GPULang::Symbol::SymbolType::InvalidType)
            symbol->~Symbol();
    }
    GPULang::ResetAllocator(&allocator);
}

//------------------------------------------------------------------------------
/**
*/
void
WriteFile(ParseContext::ParsedFile* file, ParseContext* context, std::string contents, lsp::MessageHandler& messageHandler)
{
    std::fputs(contents.c_str(), file->tmpFile);
    std::fflush(file->tmpFile);

    // Pass it to the compiler
    Clear(file->result, file->alloc);
    GPULang::MakeAllocatorCurrent(&file->alloc);
    GPULang::Compiler::Options options;
    GPULangValidate(file->tmpPath, context->includePaths, options, file->result);
    std::vector<lsp::Diagnostic> diagnostics;

    if (!file->result.diagnostics.empty())
    {
        for (auto& diagnostic : file->result.diagnostics)
        {
            diagnostics.push_back(lsp::Diagnostic{
                .range = {
                    .start = { .line = (uint32_t)diagnostic.line - 1, .character = (uint32_t)diagnostic.column },
                    .end = { .line = (uint32_t)diagnostic.line - 1, .character = (uint32_t)diagnostic.column + diagnostic.length }
                },
                .message = diagnostic.error
            });
        }
        file->result.diagnostics.clear();
    }
    messageHandler.messageDispatcher().sendNotification<lsp::notifications::TextDocument_PublishDiagnostics>(
        lsp::notifications::TextDocument_PublishDiagnostics::Params
        {
            .uri = file->path,
            .diagnostics = diagnostics
        }
    );
}

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
    it->second.path = path;

    // Open file and store in context
    FILE* f = fopen(path.c_str(), "r");
    assert(f != nullptr);
    fseek(f, 0, SEEK_END);
    int size = ftell(f);
    char* data = GPULang::AllocStack<char>(size);
    rewind(f);
    fread(data, size, 1, f);
    it->second.file = std::string(data, data + size);
    fclose(f);
    GPULang::DeallocStack(size, data);

    // Create temporary file for the compiler
    it->second.tmpPath = (std::filesystem::temp_directory_path() / std::filesystem::path(std::tmpnam(nullptr))).string();
    std::replace(it->second.tmpPath.begin(), it->second.tmpPath.end(), '\\', '/');
    it->second.tmpFile = std::fopen(it->second.tmpPath.c_str(), "w");
    WriteFile(&it->second, context, it->second.file, messageHandler);

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
            "defaultLibrary"
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
CreateSemanticToken(GPULang::Symbol::Location& prevLoc, const GPULang::Symbol* sym, ParseContext::ParsedFile* file, std::vector<uint32_t>& result, std::vector<const GPULang::Scope*>& scopes)
{
    ParseContext::ParsedFile::TextRange range;
    range.startLine = sym->location.line;
    range.stopLine = sym->location.line;
    range.startColumn = sym->location.start;
    range.stopColumn = sym->location.end;
    file->symbolsByRange.push_back(std::make_pair(range, sym));
    file->symbolsByLine[range.startLine-1].push_back(std::make_tuple(range, PresentationBits(0x0), sym));

    switch (sym->symbolType)
    {
        case GPULang::Symbol::SymbolType::VariableType:
        {
            const GPULang::Variable* var = static_cast<const GPULang::Variable*>(sym);
            const GPULang::Variable::__Resolved* res = GPULang::Symbol::Resolved(var);
            for (auto annot : var->annotations)
            {
                //InsertSemanticToken(prevLoc, annot->location, SemanticTypeMapping::Decorator, 0x0, result);
                if (annot->value != nullptr)
                    CreateSemanticToken(prevLoc, annot->value, file, result, scopes);
            }
            for (auto attr : var->attributes)
            {
                InsertSemanticToken(prevLoc, attr->location, SemanticTypeMapping::Keyword, 0x0, result);
                if (attr->expression != nullptr)
                    CreateSemanticToken(prevLoc, attr->expression, file, result, scopes);
            }
            if (res->usageBits.flags.isParameter)
                InsertSemanticToken(prevLoc, sym->location, SemanticTypeMapping::Parameter, (uint32_t)SemanticModifierMapping::Definition, result);
            else
                InsertSemanticToken(prevLoc, sym->location, SemanticTypeMapping::Variable, (uint32_t)SemanticModifierMapping::Definition, result);

            InsertSemanticToken(prevLoc, var->typeLocation, SemanticTypeMapping::Type, 0x0, result);
            ParseContext::ParsedFile::TextRange range;
            range.startLine = var->typeLocation.line;
            range.stopLine = var->typeLocation.line;
            range.startColumn = var->typeLocation.start;
            range.stopColumn = var->typeLocation.end;
            file->symbolsByRange.push_back(std::make_pair(range, res->typeSymbol));
            file->symbolsByLine[range.startLine - 1].push_back(std::make_tuple(range, PresentationBits{ {.typeLookup = 1} }, res->typeSymbol));

            if (var->valueExpression != nullptr)
                CreateSemanticToken(prevLoc, var->valueExpression, file, result, scopes);
            break;
        }
        case GPULang::Symbol::SymbolType::FunctionType:
        {
            const GPULang::Function* fun = static_cast<const GPULang::Function*>(sym);
            const GPULang::Function::__Resolved* res = GPULang::Symbol::Resolved(fun);
            scopes.push_back(&res->scope);
            for (auto annot : fun->annotations)
            {
                //InsertSemanticToken(prevLoc, annot->location, SemanticTypeMapping::Decorator, 0x0, result);
                if (annot->value != nullptr)
                    CreateSemanticToken(prevLoc, annot->value, file, result, scopes);
            }
            for (auto attr : fun->attributes)
            {
                InsertSemanticToken(prevLoc, attr->location, SemanticTypeMapping::Keyword, 0x0, result);
                if (attr->expression != nullptr)
                    CreateSemanticToken(prevLoc, attr->expression, file, result, scopes);
            }

            InsertSemanticToken(prevLoc, sym->location, SemanticTypeMapping::Function, (uint32_t)SemanticModifierMapping::Definition, result);

            for (auto var : fun->parameters)
            {
                CreateSemanticToken(prevLoc, var, file, result, scopes);
            }

            InsertSemanticToken(prevLoc, fun->returnTypeLocation, SemanticTypeMapping::Type, 0x0, result);
            ParseContext::ParsedFile::TextRange range;
            range.startLine = fun->returnTypeLocation.line;
            range.stopLine = fun->returnTypeLocation.line;
            range.startColumn = fun->returnTypeLocation.start;
            range.stopColumn = fun->returnTypeLocation.end;
            file->symbolsByRange.push_back(std::make_pair(range, res->returnTypeSymbol));
            file->symbolsByLine[range.startLine - 1].push_back(std::make_tuple(range, PresentationBits{ {.typeLookup = 1} }, res->returnTypeSymbol));

            if (fun->ast != nullptr)
            {
                CreateSemanticToken(prevLoc, fun->ast, file, result, scopes);
            }
            scopes.pop_back();

            break;
        }
        case GPULang::Symbol::SymbolType::EnumerationType:
        {
            const GPULang::Enumeration* enu = static_cast<const GPULang::Enumeration*>(sym);
            const GPULang::Enumeration::__Resolved* res = GPULang::Symbol::Resolved(enu);
            InsertSemanticToken(prevLoc, sym->location, SemanticTypeMapping::Enum, (uint32_t)SemanticModifierMapping::Definition, result);
            scopes.push_back(&res->typeSymbol->scope);

            for (auto mem : enu->labelLocations)
            {
                InsertSemanticToken(prevLoc, mem, SemanticTypeMapping::EnumMember, (uint32_t)SemanticModifierMapping::Definition, result);
            }
            scopes.pop_back();
            break;
        }
        case GPULang::Symbol::SymbolType::RenderStateType:
        {
            const GPULang::RenderState* struc = static_cast<const GPULang::RenderState*>(sym);
            const GPULang::RenderState::__Resolved* res = GPULang::Symbol::Resolved(struc);
            InsertSemanticToken(prevLoc, sym->location, SemanticTypeMapping::Struct, (uint32_t)SemanticModifierMapping::Definition, result);
            auto& [_0, bits, _1] = file->symbolsByLine[range.startLine - 1].back();
            bits = PresentationBits{ { .typeLookup=1 } };
            scopes.push_back(&res->typeSymbol->scope);
            for (auto entry : struc->entries)
            {
                const GPULang::BinaryExpression* assignEntry = static_cast<const GPULang::BinaryExpression*>(entry);
                CreateSemanticToken(prevLoc, assignEntry->left, file, result, scopes);
                CreateSemanticToken(prevLoc, assignEntry->right, file, result, scopes);
            }
            scopes.pop_back();
            break;
        }
        case GPULang::Symbol::SymbolType::SamplerStateType:
        {
            const GPULang::SamplerState* struc = static_cast<const GPULang::SamplerState*>(sym);
            const GPULang::SamplerState::__Resolved* res = GPULang::Symbol::Resolved(struc);
            InsertSemanticToken(prevLoc, sym->location, SemanticTypeMapping::Struct, (uint32_t)SemanticModifierMapping::Definition, result);
            auto& [_0, bits, _1] = file->symbolsByLine[range.startLine - 1].back();
            bits = PresentationBits{ { .typeLookup=1 } };
            scopes.push_back(&res->typeSymbol->scope);
            for (auto entry : struc->entries)
            {
                const GPULang::BinaryExpression* assignEntry = static_cast<const GPULang::BinaryExpression*>(entry);
                CreateSemanticToken(prevLoc, assignEntry->left, file, result, scopes);
                CreateSemanticToken(prevLoc, assignEntry->right, file, result, scopes);
            }
            scopes.pop_back();
            break;
        }
        case GPULang::Symbol::SymbolType::StructureType:
        {
            const GPULang::Structure* struc = static_cast<const GPULang::Structure*>(sym);
            InsertSemanticToken(prevLoc, sym->location, SemanticTypeMapping::Struct, (uint32_t)SemanticModifierMapping::Definition, result);
            scopes.push_back(&struc->scope);
            for (auto var : struc->symbols)
            {
                CreateSemanticToken(prevLoc, var, file, result, scopes);
            }
            scopes.pop_back();
            break;
        }
        case GPULang::Symbol::SymbolType::ProgramType:
        {
            const GPULang::Program* prog = static_cast<const GPULang::Program*>(sym);
            const auto* res = GPULang::Symbol::Resolved(prog);
            auto& [_0, bits, _1] = file->symbolsByLine[range.startLine - 1].back();
            bits = PresentationBits{ { .typeLookup=1 } };
            for (auto annot : prog->annotations)
            {
                if (annot->value != nullptr)
                    CreateSemanticToken(prevLoc, annot->value, file, result, scopes);
            }

            scopes.push_back(&res->typeSymbol->scope);
            for (auto entry : prog->entries)
            {
                CreateSemanticToken(prevLoc, entry, file, result, scopes);
            }
            scopes.pop_back();
            InsertSemanticToken(prevLoc, sym->location, SemanticTypeMapping::Struct, (uint32_t)SemanticModifierMapping::Definition, result);
            break;
        }
        case GPULang::Symbol::SymbolType::ScopeStatementType:
        {
            const GPULang::ScopeStatement* scope = static_cast<const GPULang::ScopeStatement*>(sym);
            const GPULang::ScopeStatement::__Resolved* res = GPULang::Symbol::Resolved(scope);
            scopes.push_back(&res->scope);
            for (auto innerSym : scope->symbols)
            {
                CreateSemanticToken(prevLoc, innerSym, file, result, scopes);
            }
            scopes.push_back(&res->scope);
            break;
        }
        case GPULang::Symbol::SymbolType::ExpressionStatementType:
        {
            const GPULang::ExpressionStatement* stat = static_cast<const GPULang::ExpressionStatement*>(sym);
            CreateSemanticToken(prevLoc, stat->expr, file, result, scopes);
            break;
        }
        case GPULang::Symbol::SymbolType::ForStatementType:
        {
            const GPULang::ForStatement* stat = static_cast<const GPULang::ForStatement*>(sym);
            InsertSemanticToken(prevLoc, sym->location, SemanticTypeMapping::Keyword, 0x0, result);
            for (auto var : stat->declarations)
                CreateSemanticToken(prevLoc, var, file, result, scopes);

            CreateSemanticToken(prevLoc, stat->condition, file, result, scopes);
            CreateSemanticToken(prevLoc, stat->loop, file, result, scopes);

            CreateSemanticToken(prevLoc, stat->contents, file, result, scopes);
            break;
        }
        case GPULang::Symbol::SymbolType::WhileStatementType:
        {
            const GPULang::WhileStatement* stat = static_cast<const GPULang::WhileStatement*>(sym);
            InsertSemanticToken(prevLoc, sym->location, SemanticTypeMapping::Keyword, 0x0, result);
            CreateSemanticToken(prevLoc, stat->condition, file, result, scopes);
            CreateSemanticToken(prevLoc, stat->statement, file, result, scopes);
            break;
        }
        case GPULang::Symbol::SymbolType::IfStatementType:
        {
            const GPULang::IfStatement* stat = static_cast<const GPULang::IfStatement*>(sym);
            InsertSemanticToken(prevLoc, sym->location, SemanticTypeMapping::Keyword, 0x0, result);
            CreateSemanticToken(prevLoc, stat->condition, file, result, scopes);
            CreateSemanticToken(prevLoc, stat->ifStatement, file, result, scopes);
            if (stat->elseStatement != nullptr)
                CreateSemanticToken(prevLoc, stat->elseStatement, file, result, scopes);
            break;
        }
        case GPULang::Symbol::SymbolType::BinaryExpressionType:
        {
            const GPULang::BinaryExpression* expr = static_cast<const GPULang::BinaryExpression*>(sym);
            CreateSemanticToken(prevLoc, expr->left, file, result, scopes);
            CreateSemanticToken(prevLoc, expr->right, file, result, scopes);
            break;
        }
        case GPULang::Symbol::SymbolType::AccessExpressionType:
        {
            const GPULang::AccessExpression* expr = static_cast<const GPULang::AccessExpression*>(sym);
            const auto res = GPULang::Symbol::Resolved(expr);
            CreateSemanticToken(prevLoc, expr->left, file, result, scopes);

            // Push type on the left
            scopes.push_back(&res->lhsType->scope);
            CreateSemanticToken(prevLoc, expr->right, file, result, scopes);
            scopes.pop_back();
            break;
        }
        case GPULang::Symbol::SymbolType::ArrayIndexExpressionType:
        {
            const GPULang::ArrayIndexExpression* expr = static_cast<const GPULang::ArrayIndexExpression*>(sym);
            CreateSemanticToken(prevLoc, expr->left, file, result, scopes);
            CreateSemanticToken(prevLoc, expr->right, file, result, scopes);
            break;
        }
        case GPULang::Symbol::SymbolType::CallExpressionType:
        {
            const GPULang::CallExpression* call = static_cast<const GPULang::CallExpression*>(sym);
            InsertSemanticToken(prevLoc, sym->location, SemanticTypeMapping::Function, 0x0, result);
            for (auto var : call->args)
                CreateSemanticToken(prevLoc, var, file, result, scopes);
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
                        InsertSemanticToken(prevLoc, sym->location, SemanticTypeMapping::Variable, 0x0, result);
                        break;
                    case GPULang::Symbol::SymbolType::EnumerationType:
                    {
                        InsertSemanticToken(prevLoc, sym->location, SemanticTypeMapping::Enum, 0x0, result);
                        break;
                    }
                    case GPULang::Symbol::SymbolType::EnumExpressionType:
                        InsertSemanticToken(prevLoc, sym->location, SemanticTypeMapping::EnumMember, 0x0, result);
                        break;
                    default:
                        InsertSemanticToken(prevLoc, sym->location, SemanticTypeMapping::Variable, 0x0, result);
                }
                ParseContext::ParsedFile::TextRange range;
                range.startLine = sym->location.line;
                range.stopLine = sym->location.line;
                range.startColumn = sym->location.start;
                range.stopColumn = sym->location.end;
                file->symbolsByRange.push_back(std::make_pair(range, foundSym));
                file->symbolsByLine[range.startLine-1].push_back(std::make_tuple(range, PresentationBits{{.symbolLookup=1}}, foundSym));
            }
            else
                InsertSemanticToken(prevLoc, sym->location, SemanticTypeMapping::Variable, 0x0, result);
            break;
        }
        case GPULang::Symbol::SymbolType::IntExpressionType:
        case GPULang::Symbol::SymbolType::FloatExpressionType:
        case GPULang::Symbol::SymbolType::BoolExpressionType:
        case GPULang::Symbol::SymbolType::UIntExpressionType:
        {
            InsertSemanticToken(prevLoc, sym->location, SemanticTypeMapping::Number, 0x0, result);
            break;
        }
        case GPULang::Symbol::SymbolType::StringExpressionType:
        {
            InsertSemanticToken(prevLoc, sym->location, SemanticTypeMapping::String, 0x0, result);
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
    if (lookup.bits == 0x0)
        ret = GPULang::Format("### %s\n", sym->name.c_str());
    switch (sym->symbolType)
    {
        case GPULang::Symbol::SymbolType::VariableType:
        {
            const GPULang::Variable* var = static_cast<const GPULang::Variable*>(sym);
            const GPULang::Variable::__Resolved* res = GPULang::Symbol::Resolved(var);
            if (lookup.flags.symbolLookup)
            {
                for (auto attr : var->attributes)
                {
                    ret += GPULang::Format("*%s*", attr->name.c_str());
                    if (attr->expression != nullptr)
                        ret += GPULang::Format("(%s) ", attr->expression->EvalString().c_str());
                    else
                        ret += " ";
                }
                ret += res->type.ToString();
            
                if (var->valueExpression != nullptr)
                {
                    std::string value = CreateMarkdown(var->valueExpression);
                    if (!value.empty())
                        ret += " = " + value;
                }
                ret += "\n\n";
                if (sym->location.line != -1)
                    ret += GPULang::Format("Declared in %s line %d\n", sym->location.file.c_str(), sym->location.line);
                ret += sym->documentation + "\n";
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
                ret += var->name + " : ";
                ret += res->type.ToString();
                ret += sym->documentation + "\n";
            }
            else
            {
                ret += res->type.ToString();
            }
            break;
        }
        case GPULang::Symbol::SymbolType::FunctionType:
        {
            const GPULang::Function* fun = static_cast<const GPULang::Function*>(sym);
            if (lookup.flags.symbolLookup)
            {
                for (auto attr : fun->attributes)
                {
                    ret += GPULang::Format("*%s*", attr->name.c_str());
                    if (attr->expression != nullptr)
                        ret += GPULang::Format("(%s) ", attr->expression->EvalString().c_str());
                    else
                        ret += " ";
                }

                ret += "\n\n";
                if (sym->location.line != -1)
                    ret += GPULang::Format("Declared in %s line %d\n", sym->location.file.c_str(), sym->location.line);
                ret += sym->documentation + "\n";
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
                ret += "##### Enum\n";
                ret += "```gpulang\n";
                for (auto mem : enu->labels)
                {
                    ret += mem + "\n";
                }
                ret += "```";
            }
            break;
        }
        case GPULang::Symbol::SymbolType::StructureType:
        {
            const GPULang::Structure* struc = static_cast<const GPULang::Structure*>(sym);
            if (lookup.flags.typeLookup)
            {
                ret += "##### Struct\n";
                ret += "```\n";
                for (auto mem : struc->symbols)
                {
                    ret += CreateMarkdown(mem, lookup);
                }
                ret += "```";
            }
            break;
        }
        case GPULang::Symbol::SymbolType::ExpressionStatementType:
        {
            const GPULang::ExpressionStatement* stat = static_cast<const GPULang::ExpressionStatement*>(sym);
            ret += CreateMarkdown(stat->expr, lookup);
            break;
        }
        case GPULang::Symbol::SymbolType::CallExpressionType:
        {
            const GPULang::CallExpression* lookup = static_cast<const GPULang::CallExpression*>(sym);
            const GPULang::CallExpression::__Resolved* res = GPULang::Symbol::Resolved(lookup);
            if (res->function != nullptr)
            {
                const GPULang::Function::__Resolved* funRes = GPULang::Symbol::Resolved(res->function);
                ret += res->function->name + "(";
                for (auto param : res->function->parameters)
                {
                    const GPULang::Variable::__Resolved* paramRes = GPULang::Symbol::Resolved(param);
                    ret += param->name + " : " + paramRes->type.ToString(true);
                    if (param != res->function->parameters.back())
                        ret += ", ";
                }
                ret += ") " + res->function->returnType.ToString() + "\n\n";
                ret += res->function->documentation + "\n";
            }
            break;
        }
        case GPULang::Symbol::SymbolType::SymbolExpressionType:
        {
            const GPULang::SymbolExpression* lookup = static_cast<const GPULang::SymbolExpression*>(sym);
            const GPULang::SymbolExpression::__Resolved* res = GPULang::Symbol::Resolved(lookup);
            if (res->symbol != nullptr)
                ret += CreateMarkdown(res->symbol, true);
            break;
        }
        case GPULang::Symbol::SymbolType::RenderStateType:
        {
            const auto state = static_cast<const GPULang::RenderState*>(sym);
            const auto res = GPULang::Symbol::Resolved(state);
            ret += "##### Render State\n";
            ret += "```\n";
            for (auto mem : res->typeSymbol->scope.symbolLookup)
            {
                ret += CreateMarkdown(mem.second, PresentationBits{ {.typeLookup = 1} });
            }
            ret += "```";
            break;
        }
        case GPULang::Symbol::SymbolType::SamplerStateType:
        {
            const auto state = static_cast<const GPULang::SamplerState*>(sym);
            const auto res = GPULang::Symbol::Resolved(state);
            ret += "##### Sampler State\n";
            ret += "```\n";
            for (auto mem : res->typeSymbol->scope.symbolLookup)
            {
                ret += CreateMarkdown(mem.second, PresentationBits{ {.typeLookup = 1} });
            }
            ret += "```";
            break;
        }
        case GPULang::Symbol::SymbolType::ProgramType:
        {
            const auto state = static_cast<const GPULang::Program*>(sym);
            const auto res = GPULang::Symbol::Resolved(state);
            ret += "##### Program\n";
            ret += "```\n";
            for (auto mem : res->typeSymbol->scope.symbolLookup)
            {
                ret += CreateMarkdown(mem.second, PresentationBits{ {.typeLookup = 1} });
            }
            ret += "```";
            break;
        }
        case GPULang::Symbol::SymbolType::IntExpressionType:
        case GPULang::Symbol::SymbolType::UIntExpressionType:
        case GPULang::Symbol::SymbolType::FloatExpressionType:
        case GPULang::Symbol::SymbolType::BoolExpressionType:
        {
            const GPULang::Expression* expr = static_cast<const GPULang::Expression*>(sym);
            ret += expr->EvalString();
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

                result.capabilities.textDocumentSync = lsp::TextDocumentSyncOptions{ .openClose = true, .change = lsp::TextDocumentSyncKind::Full };
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
                .add<lsp::notifications::TextDocument_DidOpen>([&context, &messageHandler](lsp::notifications::TextDocument_DidOpen::Params&& params)
            {
                if (params.textDocument.uri.path().ends_with("gpul") || params.textDocument.uri.path().ends_with("gpuh"))
                {
                    ParseContext::ParsedFile* file = ParseFile(params.textDocument.uri.path(), context, messageHandler);
                    file->symbolsByRange.clear();
                    file->symbolsByLine.clear();
                    file->semanticTokens.clear();
                    GPULang::Symbol::Location prev;
                    prev.line = 1;
                    prev.column = 0;
                    for (auto sym : file->result.symbols)
                    {
                        if (sym->location.file == file->tmpPath)
                        {
                            std::vector<const GPULang::Scope*> scopes{ file->result.intrinsicScope, file->result.mainScope };
                            CreateSemanticToken(prev, sym, file, file->semanticTokens, scopes);
                        }
                    }

                    for (auto& lineSymbols : file->symbolsByLine)
                    {
                        std::sort(lineSymbols.second.begin(), lineSymbols.second.end(), [](const std::tuple<ParseContext::ParsedFile::TextRange, PresentationBits, const GPULang::Symbol*>& lhs, const std::tuple<ParseContext::ParsedFile::TextRange, PresentationBits, const GPULang::Symbol*>& rhs)
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
                    Clear(file->result, file->alloc);
            })
                .add<lsp::notifications::TextDocument_DidChange>([&context, &messageHandler](lsp::notifications::TextDocument_DidChange::Params&& params)
            {
                if (params.textDocument.uri.path().ends_with("gpul") || params.textDocument.uri.path().ends_with("gpuh"))
                {
                    ParseContext::ParsedFile* file = GetFile(params.textDocument.uri.path(), context, messageHandler);
                    if (file != nullptr)
                    {
                        WriteFile(file, context, std::get<lsp::TextDocumentContentChangeEvent_Text>(params.contentChanges[0]).text, messageHandler);
                        file->symbolsByRange.clear();
                        file->symbolsByLine.clear();
                        file->semanticTokens.clear();
                        GPULang::Symbol::Location prev;
                        prev.line = 1;
                        prev.column = 0;
                        for (auto sym : file->result.symbols)
                        {
                            if (sym->location.file == file->tmpPath)
                            {
                                std::vector<const GPULang::Scope*> scopes{ file->result.intrinsicScope, file->result.mainScope };
                                CreateSemanticToken(prev, sym, file, file->semanticTokens, scopes);
                            }
                        }

                        for (auto& lineSymbols : file->symbolsByLine)
                        {
                            std::sort(lineSymbols.second.begin(), lineSymbols.second.end(), [](const std::tuple<ParseContext::ParsedFile::TextRange, PresentationBits, const GPULang::Symbol*>& lhs, const std::tuple<ParseContext::ParsedFile::TextRange, PresentationBits, const GPULang::Symbol*>& rhs)
                            {
                                auto& [lrange, _0, _1] = lhs;
                                auto& [rrange, _2, _3] = rhs;
                                return lrange.startColumn < rrange.startColumn;
                            });
                        }
                    }
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
                    ParseContext::ParsedFile::TextRange inputRange;
                    inputRange.startLine = params.position.line;
                    inputRange.startColumn = params.position.character;
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

                        if (closestSymbol != nullptr)
                        {
                             result = lsp::requests::TextDocument_Hover::Result {
                                lsp::Hover {
                                    .contents = lsp::MarkupContent{
                                        .kind = lsp::MarkupKind::Markdown,
                                        .value = CreateMarkdown(closestSymbol, presentationBits)
                                    },
                                    .range = lsp::Range{ 
                                        .start = { 
                                            .line = (uint32_t)closestSymbol->location.line - 1,
                                            .character = (uint32_t)closestSymbol->location.start
                                        }, 
                                        .end = {
                                            .line = (uint32_t)closestSymbol->location.line - 1,
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
