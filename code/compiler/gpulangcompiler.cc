//------------------------------------------------------------------------------
/**
    AnyFX compiler functions
    
    (C) 2013 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "cmdlineargs.h"
#include "gpulangcompiler.h"
#include "compiler.h"
#include "ast/preprocessor.h"
#include "ast/effect.h"
#include "memory.h"

#include "antlr4-runtime.h"
#include "antlr4-common.h"
#include "parser4/GPULangLexer.h"
#include "parser4/GPULangParser.h"
#include "parser4/gpulangerrorhandlers.h"

#include <regex>

#ifdef __WIN32__
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

using namespace antlr4;
using namespace GPULang;

//------------------------------------------------------------------------------
/**
*/
std::string
FixBackslashes(const std::string& path)
{
    std::string escaped = path;
    auto it = escaped.find("\\");
    while (it != std::string::npos)
    {
        escaped.replace(it, 1, "/");
        it = escaped.find("\\");
    }
    return escaped;
}

struct FileLevel
{
    int lineCounter;
    GPULangFile* file;
    const char* it;
    
    FileLevel()
    {
        this->file = nullptr;
        this->lineCounter = 0;
    }

    FileLevel(GPULangFile* file)
    {
        this->file = file;
        this->it = this->file->contents;
        this->lineCounter = 0;
    }

    FileLevel(FileLevel&& rhs) noexcept
    {
        this->lineCounter = rhs.lineCounter;
        this->file = rhs.file;
        this->it = rhs.it;
        rhs.file = nullptr;
    }

    FileLevel(const FileLevel& rhs)
    {
        this->lineCounter = rhs.lineCounter;
        this->file = rhs.file;
        this->it = rhs.it;
    }

};


//------------------------------------------------------------------------------
/**
*/
GPULangFile*
GPULangLoadFile(const char* path, const std::vector<std::string_view>& searchPaths)
{
    GPULangFile* file = nullptr;
    FILE* f = fopen(path, "rb");
    
    if (f == nullptr)
    {
        for (auto& searchPath : searchPaths)
        {
            TStr fullPath = TStr::Compact(searchPath.data(), path);
            f = fopen(fullPath.Data(), "rb");
            if (f != nullptr)
            {
                file = new GPULangFile();
                file->path = fullPath.buf;
                break;
            }
        }
    }
    else
    {
        file = new GPULangFile();
        file->path = path;
    }

    if (file != nullptr)
    {
        fseek(f, 0, SEEK_END);
        int size = ftell(f);

        file->contents = (char*)malloc(size);
        rewind(f);
        fread(file->contents, 1, size, f);
        fclose(f);
        file->contentSize = size;
    }

    return file;
}

//------------------------------------------------------------------------------
/**
*/
bool
GPULangPreprocess(
    GPULangFile* file
    , const std::vector<std::string>& defines
    , std::string& output
    , std::vector<GPULang::Symbol*>& preprocessorSymbols
    , std::vector<GPULang::Diagnostic>& diagnostics
)
{

#define DIAGNOSTIC(message)\
        GPULang::Diagnostic{.error = message, .file = level->file->path, .line = level->lineCounter }

    bool ret = true;
    struct Macro
    {
        std::vector<std::string> args;
        std::string contents;
    };
    std::map<std::string_view, Macro> definitions;

    std::vector<bool> ifStack;
    std::vector<FileLevel> fileStack{ { file } };
    std::vector<std::string_view> searchPaths;
    for (auto& arg : defines)
    {
        std::string_view argView = arg;
        if (arg[0] == '-')
        {
            if (arg[1] == 'I')
                searchPaths.push_back(argView.substr(2));
            else if (arg[1] == 'D')
                definitions[argView.substr(2)] = Macro{ .contents = "" };
        }
    }

    // Insert into file map
    std::map<std::string, GPULangFile*> fileMap;

    FileLevel* level = &fileStack.back();
    if (file->contents == nullptr)
    {
        diagnostics.push_back(DIAGNOSTIC(Format("Can't open '%s' for reading", file->path.c_str())));
        return false;
    }
    output.clear();
    output.reserve(file->contentSize * 1.5f);
    output.append(Format("#line 0 \"%s\"\n", file->path.c_str()));
    Macro* macro = nullptr;
    bool comment = false;
    level->lineCounter = 0;

    static auto has_char = [](const char* begin, const char* end, char c) -> uint8_t
    {
        int len = min(end - begin, 8);
        static const uint64_t masks[] = {
            0x0000000000000000ULL,
            0x00000000000000FFULL,
            0x000000000000FFFFULL,
            0x0000000000FFFFFFULL,
            0x00000000FFFFFFFFULL,
            0x000000FFFFFFFFFFULL,
            0x0000FFFFFFFFFFFFULL,
            0x00FFFFFFFFFFFFFFULL,
            0xFFFFFFFFFFFFFFFFULL
        };
        uint64_t mask = masks[len];
        uint64_t byte_mask = 0x0101010101010101ULL * c;
        uint64_t x = c ^ byte_mask;

        return 0;
    };

    static auto is_digit = [](const char c) -> bool
    {
        return (c >= '0' && c <= '9');
    };

    static auto validateIdentifier = [](const char* begin, const char* end) -> bool
    {
        if (!GPULangValidIdentifierStart(begin[0]))
            return false;
        const char* start = begin+1;
        while (start != end)
        {
            if (!GPULangValidIdentifierChar(start[0]))
                return false;
            start++;
        }
        return true;
    };

    static auto whitespace = [](const char c) -> bool
    {
        return c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == '\f' || c == '\v';
    };

    static auto wordStart = [](const char* begin, const char* end) -> const char*
    {
        const char* start = begin;
        while (start != end)
        {
            if (start[0] == '*')
                if (start + 1 != end && start[1] == '/')
                    return start;
            if (start[0] == '/')
                if (start + 1 != end && start[1] == '*')
                    return start;
                else if (start + 1 != end && start[1] == '/')
                    return start;
            if (!whitespace(start[0]))
                return start;
            start++;
        }
        return end;
    };

    static auto wordEnd = [](const char* begin, const char* end) -> const char*
    {
        const char* start = begin;
        while (start != end)
        {
            if (start[0] == '*')
                if (start + 1 != end && start[1] == '/')
                    return start;
            if (start[0] == '/')
                if (start + 1 != end && start[1] == '*')
                    return start;
                else if (start + 1 != end && start[1] == '/')
                    return start;
            if (whitespace(start[0]))
                return start;
            start++;
        }
        return end;
    };

    
    static auto wordEndOrParanthesis = [](const char* begin, const char* end) -> const char*
    {
        const char* start = begin;
        while (start != end)
        {
            if (start[0] == '(')
                return start;
            if (whitespace(start[0]))
                return start;
            start++;
        }
        return end;
    };

    static auto commentEnd = [](const char* begin, const char* end) -> const char*
    {
        const char* start = begin;
        while (start != end)
        {
            if (start[0] == '*')
                if (start + 1 != end && start[1] == '/')
                    return start;
            start++;
        }
        return end;
    };

    static auto nextArg = [](const char* begin, const char* end) -> const char*
    {
        const char* start = begin;
        while (start != end)
        {
            if (start[0] == ',')
                return wordStart(start+1, end);
            if (start[0] == ')')
                return start;
            start++;
        }
        return end;
    };

    static auto nextComment = [](const char* begin, const char* end) -> const char*
    {
        const char* start = begin;
        while (start != end)
        {
            if (start[0] == '*' && start[1] == '/')
                return start;
            start++;
        }
        return end;
    };

    static auto lineEnd = [](const char* begin, const char* end) -> const char*
    {
        const char* start = begin;
        while (start != end)
        {
            if (start[0] == '\r')
            {
                // Check if \r\n
                if ((start != (end - 1)) && start[1] == '\n')
                    return start + 2;
                return start + 1;
            }
            else if (start[0] == '\n')
                return start + 1;
            else if (start[0] == '\0')
                return start;
            start++;
        }
        return end;
    };

    
    static auto identifierBegin = [](const char* begin, const char* end) -> const char*
    {
        const char* start = begin;
        while (start != end)
        {
            if (GPULangValidIdentifierStart(start[0]))
                return start;
            start++;
        }
        return end;
    };

    static auto identifierEnd = [](const char* begin, const char* end) -> const char*
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

    static std::function<int(const char*, const char*, FileLevel*, std::vector<GPULang::Diagnostic>&, std::map<std::string_view, Macro>&)> eval = [](const char* begin, const char* end, FileLevel* level, std::vector<GPULang::Diagnostic>& diagnostics, std::map<std::string_view, Macro>& definitions) -> int
    {
        struct Token
        {
            int value;
            bool op;
        };
        StackArray<Token> result(256);
        StackArray<Token> opstack(256);
        
        auto digit = [&diagnostics, &level](const char* begin, const char* end, int& size) -> int
        {
            int val = 0;
            size = 0;
            int dec = 1;
            while (begin != end)
            {
                if (begin[0] >= '0' && begin[0] <= '9')
                {
                    val += begin[0] - '0' * dec;
                    size++;
                    dec *= 10;
                }
                else if (dec == 1)
                    return -1;
                else
                    break;
                    
                begin++;
            }
            return val;
        };
        
        auto op = [](const char* begin, const char* end, int& size) -> int
        {
            int val = 0;
            size = 0;
            while (begin != end)
            {
                if (begin[0] == '+' || begin[0] == '-' || begin[0] == '*' || begin[0] == '/' || begin[0] == '=' || begin[0] == '!' || begin[0] == '>' || begin[0] == '<' || begin[0] == '&' || begin[0] == '^' || begin[0] == '|')
                {
                    val |= begin[0] << size*8;
                    size++;
                }
                else if (size == 0)
                    return -1;
                else
                    break;
                
                // If the counter is 3 then it's definitely something wrong with the operator
                if (size == 3)
                    return -1;
                begin++;
            }
            return val;
        };
        
        auto presedence = [&diagnostics, &level](const int op) -> int
        {
            switch(op)
            {
                case '||':
                    return 1;
                case '&&':
                    return 2;
                case '|':
                    return 3;
                case '^':
                    return 4;
                case '&':
                    return 5;
                case '==':
                case '!=':
                    return 6;
                case '>':
                case '<':
                case '>=':
                case '<=':
                    return 7;
                case '<<':
                case '>>':
                    return 8;
                case '+':
                case '-':
                    return 9;
                case '*':
                case '/':
                case '%':
                    return 10;
                case '!':
                    return 11;
                case '(':
                case ')':
                    return 12;
                default:
                    diagnostics.push_back(DIAGNOSTIC(Format("Unknown operator '%c%c%c%c'", op & 0xFF, (op >> 8) & 0xFF, (op >> 16) & 0xFF, (op >> 24) & 0xFF)));
                    return -1;
            }
        };

        auto eval_op = [](int op, int left, int right) -> int
        {
            switch (op)
            {
                case '+':
                    return left + right;
                case '-':
                    return left - right;
                case '*':
                    return left * right;
                case '/':
                    return left / right;
                case '%':
                    return left % right;
                case '<<':
                    return left << right;
                case '>>':
                    return left >> right;
                case '<':
                    return left < right;
                case '>':
                    return left > right;
                case '<=':
                    return left <= right;
                case '>=':
                    return left >= right;
                case '==':
                    return left == right;
                case '!=':
                    return left != right;
                case '&':
                    return left & right;
                case '^':
                    return left ^ right;
                case '|':
                    return left | right;
                case '&&':
                    return (left != 0) && (right != 0);
                case '||':
                    return (left != 0) || (right != 0);
                default:
                    return INT_MAX;
            }
        };
        
        const char* word = begin;
        int unconsumedIntegers = 0;
        while (word != end)
        {
            word = wordStart(word, end);
            Token t;
            int stride = 0;
            if (word == end)
                break;
            int val = digit(word, end, stride);
            if (val == -1)
            {
                val = op(word, end, stride);
                if (val == -1)
                {
                    const char* identStart = identifierBegin(word, end);
                    const char* identEnd = identifierEnd(identStart, end);
                    std::string_view ident(identStart, identEnd);
                    if (ident.length() > 0)
                    {
                        auto defIt = definitions.find(ident);
                        if (defIt == definitions.end())
                        {
                            diagnostics.push_back(DIAGNOSTIC(Format("Unknown identifier *.%s", ident.length(), ident.data())));
                            return -1;
                        }
                        else
                        {
                            // If identifier of a macro, evaluate the contents of said macro
                            t.value = eval(defIt->second.contents.data(), defIt->second.contents.data() + defIt->second.contents.length(), level, diagnostics, definitions);
                            t.op = false;
                            unconsumedIntegers++;
                            result.Append(t);
                        }
                    }
                    else
                    {
                        diagnostics.push_back(DIAGNOSTIC(Format("Invalid token '%c'", word[0])));
                        return -1;
                    }
                    word = identEnd;
                }
                else
                {
                    t.value = val;
                    t.op = true;
                    if (t.value != '!') // unary operator
                    { 
                        if (unconsumedIntegers < 1)
                        {
                            diagnostics.push_back(DIAGNOSTIC(Format("Operators must be surrounded by expressions '*.%s'", stride, word)));
                            return -1;
                        }

                        // When two values are consumed by an operator, one gets produced as an outcome, so only reduce this value by 1
                        unconsumedIntegers -= 1;
                    }                    
                
                    // Operator, check presedence
                    if (opstack.size > 0)
                    {
                        int pres = presedence(t.value);
                        int prevPres = presedence(opstack.ptr[opstack.size-1].value);
                        if (pres < prevPres)
                        {
                            // Flush operators to results
                            for (int i = opstack.size-1; i >= 0; i--)
                            {
                                result.Append(opstack.ptr[i]);
                            }
                            opstack.size = 0;
                        }
                    }
                    opstack.Append(t);
                }
            }
            else
            {
                // Integer, push to results
                t.value = val;
                t.op = false;
                unconsumedIntegers++;
                result.Append(t);
            }
            word += stride;
        }
        
        // If we have dangling ops, push to result
        if (opstack.size > 0)
        {
            // Flush operators to results
            for (int i = opstack.size-1; i >= 0; i--)
            {
                result.Append(opstack.ptr[i]);
            }
            opstack.size = 0;
        }

        // Go through results and evaluate
        for (int i = 0; i < result.size; i++)
        {
            Token& t = result.ptr[i];
            
            if (t.op)
            {
                if (t.value == '!') // unary operator
                {
                    const Token& rhs = result.ptr[i - 1];
                    int res = !rhs.value;
                    t.op = false;
                    t.value = res;
                    memmove(&result.ptr[i - 1], &result.ptr[i], (result.size - i) * sizeof(Token));
                    result.size -= 1;
                    i -= 1;
                }
                else
                {
                    const Token& rhs = result.ptr[i - 1];
                    const Token& lhs = result.ptr[i - 2];
                    int res = eval_op(t.value, lhs.value, rhs.value);
                    t.op = false;
                    t.value = res;
                    memmove(&result.ptr[i - 2], &result.ptr[i], (result.size - i) * sizeof(Token));
                    result.size -= 2;
                    i -= 2;
                }
            }
        }
        
        // The last value is the result
        return result.ptr[0].value;
    };

#define POP_FILE() \
            fileStack.pop_back();\
            if (!fileStack.empty())\
            {\
                level = &fileStack.back();\
                output.append(Format("#line %d \"%s\"\n", level->lineCounter, level->file->path.c_str()));\
                level->lineCounter++;\
            }\
            continue;
    while (!fileStack.empty())
    {
        // Find next unescaped \n
        if (level->it == nullptr || level->it == level->file->contents + level->file->contentSize)
        {
            POP_FILE()
        }
        const char* endOfLine = lineEnd(level->it, level->file->contents + level->file->contentSize);

        std::string_view lineSpan(level->it, endOfLine);
        level->it = endOfLine;

        int lineEndingLength = 0;
        if (lineSpan.ends_with("\r\n"))
            lineEndingLength = 2;
        else if (lineSpan.ends_with("\r") || lineSpan.ends_with("\n"))
            lineEndingLength = 1;

        // Check for valid end of line, meaning the file is corrupt after this point
        auto charIterator = lineSpan.rbegin();
        if (lineSpan.length() >= 3 && (charIterator[2] == '\r' || charIterator[2] == '\n'))
            lineEndingLength = -1;
        if (lineEndingLength == -1)
        {
            //diagnostics.push_back(Diagnostic{ .error = Format("Line is either too long or has corrupt file endings\n%s", lineStr.c_str()), .file = level->path, .line = level->lineCounter });
            POP_FILE()
        }        

escape_newline:

        if (lineSpan.ends_with("\\\n"))
        {
            char* endOfLineMutable = const_cast<char*>(level->it);
            endOfLineMutable[-1] = ' ';
            endOfLineMutable[-2] = ' ';
            const char* startOfNextLine = lineEnd(level->it, level->file->contents + level->file->contentSize);
            lineSpan = std::string_view(lineSpan.data(), startOfNextLine);
            level->it = startOfNextLine;
            level->lineCounter++;
            goto escape_newline;
        }
        else if (lineSpan.ends_with("\\\r\n"))
        {
            char* endOfLineMutable = const_cast<char*>(level->it);
            endOfLineMutable[-1] = ' ';
            endOfLineMutable[-2] = ' ';
            endOfLineMutable[-3] = ' ';
            const char* startOfNextLine = lineEnd(level->it, level->file->contents + level->file->contentSize);
            lineSpan = std::string_view(lineSpan.data(), startOfNextLine);
            level->it = startOfNextLine;
            level->lineCounter++;
            goto escape_newline;
        }

        auto lineIt = lineSpan.cbegin();
        auto lineEnd = lineSpan.cend();

        // Macro to eliminate directives and output if an if-scope is inactive
#define IFDEFSTACK() \
        if (!ifStack.empty())\
        {\
            if (!ifStack.back())\
            {\
                auto pp = Alloc<Preprocessor>();\
                pp->type = Preprocessor::Comment;\
                pp->location.file = level->file->path;\
                pp->location.line = level->lineCounter;\
                pp->location.start = 0;\
                pp->location.end = lineSpan.length();\
                preprocessorSymbols.push_back(pp);\
                output.push_back('\n');\
                goto next_line;\
            }\
        }

#define IFDEFSTACKPUSH() \
        if (!ifStack.empty())\
        {\
            if (!ifStack.back())\
            {\
                auto pp = Alloc<Preprocessor>();\
                pp->type = Preprocessor::Comment;\
                pp->location.file = level->file->path;\
                pp->location.line = level->lineCounter;\
                pp->location.start = 0;\
                pp->location.end = lineSpan.length();\
                preprocessorSymbols.push_back(pp);\
                output.push_back('\n');\
                ifStack.push_back(false);\
                goto next_line;\
            }\
        }

#define ELIFDEFSTACKPUSH() \
        if (ifStack.size() >= 2)\
        {\
            if (!*(ifStack.rbegin()+1))\
            {\
                auto pp = Alloc<Preprocessor>();\
                pp->type = Preprocessor::Comment;\
                pp->location.file = level->file->path;\
                pp->location.line = level->lineCounter;\
                pp->location.start = 0;\
                pp->location.end = lineSpan.length();\
                preprocessorSymbols.push_back(pp);\
                output.push_back('\n');\
                goto next_line;\
            }\
        }

#define SETUP_PP2(pp, begin, stop)\
        pp->location.file = level->file->path;\
        pp->location.line = level->lineCounter;\
        pp->location.start = begin - lineBegin;\
        pp->location.end = stop - lineBegin;\
        preprocessorSymbols.push_back(pp);

#define SETUP_ARG2(pp, arg, begin, stop)\
        pp->args.push_back(arg);\
        Symbol::Location argLoc;\
        argLoc.file = level->file->path;\
        argLoc.line = level->lineCounter;\
        argLoc.start = begin - lineBegin;\
        argLoc.end = stop - lineBegin;\
        pp->argLocations.push_back(argLoc);

        const char* lineBegin = &(*lineIt);
        const char* eol = &(*(lineEnd - 1)) + 1;
        const char* columnIt = lineBegin;
        const char* prevColumnIt = columnIt;
        do 
        {
            // Find beginning of first non-WS character
            const char* firstWord = wordStart(columnIt, eol);

            // If empty line, just add it and continue
            if (firstWord == eol)
            {
                output.append(columnIt, eol);
                goto next_line;
            }
            if (firstWord[0] == '#') // Directives
            {
                firstWord++;
                const char* startOfDirective = wordStart(firstWord, eol);
                const char* endOfDirective = wordEnd(firstWord, eol);
                if (strncmp(startOfDirective, "include", 7) == 0) // Include
                {
                    IFDEFSTACK()

                    const char* startOfPath = wordStart(endOfDirective, eol);
                    const char* endOfPath = wordEnd(startOfPath, eol);
                    auto pp = Alloc<Preprocessor>();
                    pp->type = Preprocessor::Include;
                    SETUP_PP2(pp, firstWord-1, endOfDirective);

                    if ((startOfPath[0] == '"' && endOfPath[-1] == '"')
                        || (startOfPath[0] == '<' && endOfPath[-1] == '>'))
                    {
                        const std::string path = std::string(startOfPath + 1, endOfPath - 1);
                        SETUP_ARG2(pp, path, startOfPath, endOfPath);

                        GPULangFile* inc = nullptr;
                        auto fileIt = fileMap.find(path);
                        if (fileIt == fileMap.end())
                        {
                            inc = GPULangLoadFile(path.c_str(), searchPaths);
                            fileMap[path] = inc;
                        }
                        else
                        {
                            inc = fileIt->second;
                        }
                        pp->contents = inc->path;
                        if (inc == nullptr)
                        {
                            diagnostics.push_back(DIAGNOSTIC(Format("File not found '%s'", inc->path.c_str())));
                            ret = false;
                            goto end;
                        }
                        output.append(Format("#line %d \"%s\"\n", level->lineCounter, level->file->path.c_str()));

                        fileStack.push_back({ inc });
                        level = &fileStack.back();
                    }
                    else
                    {
                        diagnostics.push_back(DIAGNOSTIC("include directory must be provided a path"));
                        ret = false;
                        goto end;
                    }
                }
                else if (strncmp(startOfDirective, "define", 6) == 0)
                {
                    IFDEFSTACK()
                    const char* startOfDefinition = wordStart(endOfDirective, eol);
                    if (startOfDefinition == eol)
                    {
                        diagnostics.push_back(DIAGNOSTIC("define missing identifier"));
                        ret = false;
                        goto end;
                    }
                    const char* endOfDefinition = wordEndOrParanthesis(startOfDefinition, eol);
                    if (!validateIdentifier(startOfDefinition, endOfDefinition))
                    {
                        diagnostics.push_back(DIAGNOSTIC("define identifier invalid"));
                        ret = false;
                        goto end;
                    }

                    std::string_view def = std::string_view(startOfDefinition, endOfDefinition);
                    auto prev = definitions.find(def);
                    if (prev != definitions.end())
                    {
                        diagnostics.push_back(DIAGNOSTIC(Format("macro %s redefinition", std::string(def).c_str())));
                        ret = false;
                        goto end;
                    }

                    macro = &definitions[def];

                    auto pp = Alloc<Preprocessor>();
                    pp->type = Preprocessor::Macro;
                    SETUP_PP2(pp, firstWord-1, endOfDirective)
                    SETUP_ARG2(pp, std::string(def), startOfDefinition, endOfDefinition);

                    const char* startOfContents = wordStart(endOfDefinition, eol);
                    
                    if (endOfDefinition[0] == '(')
                    {
                        // Argument list, start parsing arguments
                        const char* argumentIt = wordStart(endOfDefinition+1, endOfDefinition);
                        while (argumentIt != endOfDefinition)
                        {
                            if (argumentIt[0] == ')') // end of list
                            {
                                // Move new start to after argument list
                                pp->argLocations[0].end = argumentIt + 1 - lineBegin;
                                startOfContents = wordStart(argumentIt+1, eol);
                                break;
                            }
                            const char* argumentEnd = identifierEnd(argumentIt, endOfDefinition);

                            if (!validateIdentifier(argumentIt, argumentEnd))
                            {
                                diagnostics.push_back(DIAGNOSTIC("Invalid argument identifier"));
                                ret = false;
                                goto end;
                            }

                            // Identifier validation done, save argument
                            macro->args.push_back(std::string(argumentIt, argumentEnd));
                            argumentIt = nextArg(argumentIt, endOfDefinition);
                        }

                        // append the rest of the contents
                        macro->contents.append(startOfContents, eol - lineEndingLength);
                    }
                    else if (startOfContents != eol)
                    {
                        macro->contents = std::string(startOfContents, eol - lineEndingLength);
                    }               
                    else
                    {

                    }
                    output.append(Format("#line %d \"%s\"", level->lineCounter, level->file->path.c_str()));
                }
                else if (strncmp(startOfDirective, "undef", 5) == 0)
                {
                    IFDEFSTACK()

                    const char* startOfDefinition = wordStart(endOfDirective, eol);
                    if (startOfDefinition == nullptr)
                    {
                        diagnostics.push_back(DIAGNOSTIC("undef missing identifier"));
                        ret = false;
                        goto end;
                    }
                    const char* endOfDefinition = wordEnd(startOfDefinition, eol);

                    if (!validateIdentifier(startOfDefinition, endOfDefinition))
                    {
                        diagnostics.push_back(DIAGNOSTIC("Invalid undef identifier"));
                        ret = false;
                        goto end;
                    }

                    std::string_view definition = std::string_view(startOfDefinition, endOfDefinition);

                    auto pp = Alloc<Preprocessor>();
                    pp->type = Preprocessor::Undefine;
                    SETUP_PP2(pp, firstWord - 1, endOfDirective);
                    SETUP_ARG2(pp, std::string(definition), startOfDefinition, endOfDefinition);

                    auto it = definitions.find(definition);
                    if (it != definitions.end())
                    {
                        definitions.erase(definition);
                    }
                }
                else if (strncmp(startOfDirective, "ifdef", 5) == 0)
                {
                    IFDEFSTACKPUSH()

                    const char* startOfDefinition = wordStart(endOfDirective, eol);
                    if (startOfDefinition == nullptr)
                    {
                        diagnostics.push_back(DIAGNOSTIC("ifdef missing identifier"));
                        ret = false;
                        goto end;
                    }
                    const char* endOfDefinition = wordEnd(startOfDefinition, eol);

                    if (!validateIdentifier(startOfDefinition, endOfDefinition))
                    {
                        diagnostics.push_back(DIAGNOSTIC("Invalid ifdef identifier"));
                        ret = false;
                        goto end;
                    }

                    std::string_view definition = std::string_view(startOfDefinition, endOfDefinition);

                    auto pp = Alloc<Preprocessor>();
                    pp->type = Preprocessor::If;
                    SETUP_PP2(pp, firstWord - 1, endOfDirective);
                    SETUP_ARG2(pp, std::string(definition), startOfDefinition, endOfDefinition);

                    auto it = definitions.find(definition);
                    if (it != definitions.end())
                    {
                        ifStack.push_back(true);
                    }
                    else
                    {
                        ifStack.push_back(false);
                    }
                }
                else if (strncmp(startOfDirective, "ifndef", 6) == 0)
                {
                    IFDEFSTACKPUSH()

                    const char* startOfDefinition = wordStart(endOfDirective, eol);
                    if (startOfDefinition == nullptr)
                    {
                        diagnostics.push_back(DIAGNOSTIC("ifdef missing identifier"));
                        ret = false;
                        goto end;
                    }
                    const char* endOfDefinition = wordEnd(startOfDefinition, eol);

                    if (!validateIdentifier(startOfDefinition, endOfDefinition))
                    {
                        diagnostics.push_back(DIAGNOSTIC("Invalid ifdef identifier"));
                        ret = false;
                        goto end;
                    }

                    std::string_view definition = std::string_view(startOfDefinition, endOfDefinition);

                    auto pp = Alloc<Preprocessor>();
                    pp->type = Preprocessor::If;
                    SETUP_PP2(pp, firstWord - 1, endOfDirective);
                    SETUP_ARG2(pp, std::string(definition), startOfDefinition, endOfDefinition);

                    auto it = definitions.find(definition);
                    if (it == definitions.end())
                    {
                        ifStack.push_back(true);
                    }
                    else
                    {
                        ifStack.push_back(false);
                    }
                }
                else if (strncmp(startOfDirective, "elif", 4) == 0)
                {
                    ELIFDEFSTACKPUSH()
                    
                    auto pp = Alloc<Preprocessor>();
                    pp->type = Preprocessor::If;
                    SETUP_PP2(pp, firstWord - 1, endOfDirective);

                    int val = eval(endOfDirective, eol, level, diagnostics, definitions);
                    SETUP_ARG2(pp, std::string(val == 0 ? "false" : "true"), endOfDirective, eol);

                    if (ifStack.empty())
                    {
                        diagnostics.push_back(DIAGNOSTIC("elif missing if/ifdef/ifndef"));
                        ret = false;
                        goto end;
                    }

                    ifStack.pop_back();
                    if (val != 0)
                    {
                        ifStack.push_back(true);
                    }
                    else
                    {
                        ifStack.push_back(false);
                    }
                }
                else if (strncmp(startOfDirective, "if", 2) == 0)
                {
                    IFDEFSTACKPUSH()

                    auto pp = Alloc<Preprocessor>();
                    pp->type = Preprocessor::If;
                    SETUP_PP2(pp, firstWord - 1, endOfDirective);

                    int val = eval(endOfDirective, eol, level, diagnostics, definitions);
                    SETUP_ARG2(pp, std::string(val == 0 ? "false" : "true"), endOfDirective, eol);

                    if (val != 0)
                    {
                        ifStack.push_back(true);
                    }
                    else
                    {
                        ifStack.push_back(false);
                    }
                }
                else if (strncmp(startOfDirective, "else", 4) == 0)
                {
                    ELIFDEFSTACKPUSH()
                    auto pp = Alloc<Preprocessor>();
                    pp->type = Preprocessor::Else;
                    SETUP_PP2(pp, firstWord-1, endOfDirective)
                    ifStack.back() = !ifStack.back();
                }
                else if (strncmp(startOfDirective, "endif", 5) == 0)
                {
                    auto pp = Alloc<Preprocessor>();
                    pp->type = Preprocessor::EndIf;
                    SETUP_PP2(pp, firstWord-1, endOfDirective)
                    if (ifStack.empty())
                    {
                        diagnostics.push_back(GPULang::Diagnostic{ .error = "Invalid #endif, missing matching #if/#ifdef/#ifndef", .file = level->file->path, .line = level->lineCounter });
                        ret = false;
                        goto end;
                    }
                    else
                        ifStack.pop_back();
                }
                output.append("\n");
                goto next_line;
            }
            else if (firstWord[0] == '/' && firstWord[1] == '/')
            {
                auto pp = Alloc<Preprocessor>();
                pp->type = Preprocessor::Comment;
                SETUP_PP2(pp, firstWord, eol)
                pp->location.end = lineEnd - lineIt;

                output.append(columnIt, firstWord);
                output.push_back('\n');
                goto next_line;
            }
            else if (!comment && firstWord[0] == '/' && firstWord[1] == '*')
            {
                comment = true;
                output.append(columnIt, firstWord);
                auto pp = Alloc<Preprocessor>();
                pp->type = Preprocessor::Comment;
                SETUP_PP2(pp, firstWord, eol)

                const char* endOfComment = commentEnd(columnIt, eol);
                if (endOfComment != eol)
                {
                    pp->location.end = endOfComment + 2 - lineBegin;
                    columnIt = endOfComment + 2;
                    comment = false;
                }
                else
                    columnIt = firstWord + 2;
                continue;
            }
            else if (comment && firstWord[0] == '*' && firstWord[1] == '/')
            {
                comment = false;
                auto pp = Alloc<Preprocessor>();
                pp->type = Preprocessor::Comment;
                SETUP_PP2(pp, columnIt, firstWord+2)
                columnIt = firstWord + 2;
                continue;
            }
            else if (!comment)
            {
                IFDEFSTACK()
                const char* startOfWord = identifierBegin(columnIt, eol);
                const char* endOfWord = identifierEnd(startOfWord, eol);

                // Add whatever comes before the first identifier
                output.append(columnIt, startOfWord);
                if (startOfWord != eol)
                {
                    std::string_view word = std::string_view(startOfWord, endOfWord);
                    auto it = definitions.find(word);
                    if (it != definitions.end())
                    {
                        auto pp = Alloc<Preprocessor>();
                        pp->type = Preprocessor::Call;
                        SETUP_PP2(pp, startOfWord, endOfWord)

                        if (endOfWord[0] == '(')
                        {
                            const char* argumentIt = wordStart(endOfWord + 1, eol);
                            std::vector<std::string> args;
                            bool valid = false;
                            while (argumentIt != eol)
                            {
                                if (argumentIt[0] == ')') // end of list
                                {
                                    // Move new start to after argument list
                                    valid = true;
                                    break;
                                }
                                const char* argumentEnd = identifierEnd(argumentIt, eol);

                                // Check argument for validity
                                if (!GPULangValidIdentifierStart(argumentIt[0])) // First argument may not be a number
                                {
                                    diagnostics.push_back(DIAGNOSTIC(Format("Invalid argument character '%c'", argumentIt[0])));
                                    ret = false;
                                    goto end;
                                }
                                else
                                {
                                    // If first char is valid, check the rest
                                    const char* argumentCharIt = argumentIt + 1;
                                    while (argumentCharIt != argumentEnd)
                                    {
                                        if (!GPULangValidIdentifierChar(argumentCharIt[0]))
                                        {
                                            diagnostics.push_back(DIAGNOSTIC(Format("Invalid argument character '%c'", argumentCharIt[0])));
                                            ret = false;
                                            goto end;
                                        }
                                        argumentCharIt++;
                                    }
                                }
                                args.push_back(std::string(argumentIt, argumentEnd));
                                argumentIt = nextArg(argumentIt, eol);
                            }
                            if (!valid)
                            {
                                diagnostics.push_back(DIAGNOSTIC("Macro call missing ')'"));
                                ret = false;
                                goto end;
                            }
                            endOfWord = argumentIt + 1;
                            std::map<std::string, std::string> argumentMap;

                            // Warn if mismatch
                            int maxIterations = min(args.size(), it->second.args.size());
                            int argCounter = 0;
                            for (auto& arg : it->second.args)
                            {
                                argumentMap[arg] = args[argCounter++];
                            }

                            const char* macroContentsIt = &(*it->second.contents.begin());
                            const char* macroContentsEnd = &(*(it->second.contents.end() - 1)) + 1;

                            // Replace macro contents with definitions and arguments
                            while (macroContentsIt != macroContentsEnd)
                            {
                                const char* startOfMacroWord = identifierBegin(macroContentsIt, macroContentsEnd);
                                const char* endOfMacroWord = identifierEnd(startOfMacroWord, macroContentsEnd);

                                // Add whatever comes before the identifier to the output
                                pp->contents.append(macroContentsIt, startOfMacroWord);
                                if (macroContentsEnd == startOfMacroWord)
                                    break;

                                if (startOfMacroWord != macroContentsEnd)
                                {
                                    // Add everything leading up to the identifier to the output
                                    std::string word(startOfMacroWord, endOfMacroWord);
                                    macroContentsIt = endOfMacroWord;

                                    auto argIt = argumentMap.find(word);
                                    if (argIt == argumentMap.end())
                                    {
                                        auto defIt = definitions.find(word);
                                        if (defIt == definitions.end())
                                        {
                                            pp->contents.append(word);
                                        }
                                        else
                                        {
                                            pp->contents.append(defIt->second.contents);
                                        }
                                    }
                                    else
                                    {
                                        pp->contents.append(argIt->second);
                                    }
                                }
                                else
                                {
                                    pp->contents.append(startOfMacroWord, endOfMacroWord);
                                    macroContentsIt = endOfMacroWord;
                                }

                            }
                        }
                        else
                        {
                            const char* macroContentsIt = &(*it->second.contents.begin());
                            const char* macroContentsEnd = &(*(it->second.contents.end() - 1)) + 1;

                            // Replace macro contents with definitions and arguments
                            while (macroContentsIt != macroContentsEnd)
                            {
                                const char* startOfMacroIdentifier = identifierBegin(macroContentsIt, macroContentsEnd);
                                const char* endOfMacroIdentifier = identifierEnd(startOfMacroIdentifier, macroContentsEnd);

                                // Add whatever comes before the identifier to the output
                                pp->contents.append(macroContentsIt, startOfMacroIdentifier);
                                if (macroContentsEnd == startOfMacroIdentifier)
                                    break;

                                std::string word(startOfMacroIdentifier, endOfMacroIdentifier);

                                auto defIt = definitions.find(word);
                                if (defIt == definitions.end())
                                {
                                    pp->contents.append(word);
                                }
                                else
                                {
                                    pp->contents.append(defIt->second.contents);
                                }

                                macroContentsIt = endOfMacroIdentifier;
                            }
                        }

                        // Add summed contents to output
                        output.append(pp->contents);
                    }
                    else
                    {
                        output.append(startOfWord, endOfWord);
                    }
                }
                else
                {
                    endOfWord = startOfWord;
                    output.append(startOfWord, endOfWord);
                }
                columnIt = endOfWord;
            }
            if (comment)
            {
                auto pp = Alloc<Preprocessor>();
                pp->type = Preprocessor::Comment;
                SETUP_PP2(pp, lineBegin, eol)
                output.append("\n");
                goto next_line;
            }

            // Go to next non white space word
            prevColumnIt = columnIt;
        } while (columnIt != eol);

next_line:
        level->lineCounter++;
    }
end:
    for (auto& file : fileMap)
    {
        delete file.second;
    }
    return ret;
}

//------------------------------------------------------------------------------
/**
    Preprocess a file
*/
bool
GPULangPreprocessFile(
    const std::string& path
    , const std::vector<std::string>& defines
    , std::string& output
    , std::vector<GPULang::Symbol*>& preprocessorSymbols
    , std::vector<GPULang::Diagnostic>& diagnostics
)
{
    std::string escaped = FixBackslashes(path);
    std::string fileName = path.substr(escaped.rfind("/") + 1, escaped.length() - 1);
    std::string folder = escaped.substr(0, escaped.rfind("/") + 1);
    std::string dummy;
    GPULangFile* file = GPULangLoadFile(path.c_str(), {});
    bool ret = GPULangPreprocess(file, defines, output, preprocessorSymbols, diagnostics);
    delete file;
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
std::vector<std::string>
GPULangGenerateDependencies(const std::string& file, const std::vector<std::string>& defines)
{
    std::vector<std::string> res;

    const char* constArgs[] =
    {
        "",			// first argument is supposed to be application system path, but is omitted since we run mcpp directly
        "-M"
    };
    const unsigned numConstArgs = sizeof(constArgs) / sizeof(char*);
    const unsigned numTotalArgs = numConstArgs + defines.size() + 1;
    const char** args = new const char*[numConstArgs + defines.size() + 1];
    memcpy(args, constArgs, sizeof(constArgs));

    unsigned i;
    for (i = 0; i < defines.size(); i++)
    {
        args[numConstArgs + i] = defines[i].c_str();
    }
    args[numTotalArgs - 1] = file.c_str();

    /*
    // run preprocessing
    mcpp_use_mem_buffers(1);
    int result = mcpp_lib_main(numTotalArgs, (char**)args);
    if (result == 0)
    {
        std::string output = mcpp_get_mem_buffer((OUTDEST)0);

        // grah, remove the padding and the Makefile stuff, using std::string...
        size_t colon = output.find_first_of(':')+1;
        output = output.substr(colon);
        size_t newline = output.find_first_of('\n');
        while (newline != output.npos)
        {
            std::string line = output.substr(0, newline);
            if (!line.empty())
            {
                while (!line.empty() && (line.front() == ' '))								line = line.substr(1);
                while (!line.empty() && (line.back() == ' ' || line.back() == '\\'))		line = line.substr(0, line.size() - 1);
                res.push_back(line);
                output = output.substr(newline + 1);
                newline = output.find_first_of('\n');
            }
            else
                break;
        }
    }
    delete[] args;
     */
    return res;
}

//------------------------------------------------------------------------------
/**
*/
GPULangErrorBlob*
Error(const std::string message)
{
    GPULangErrorBlob* ret = new GPULangErrorBlob();
    if (message.size() > 0)
    {
        ret->buffer = new char[message.size() + 1];
        ret->size = message.size();
        message.copy(ret->buffer, ret->size);
        ret->buffer[ret->size] = '\0';
    }
    else
    {
        ret->buffer = nullptr;
        ret->size = 0;
    }
    return ret;
}

//------------------------------------------------------------------------------
/**
    Compiles GPULang file.

    @param file			Input file to compile
    @param output		Output destination file
    @param target		Target language
    @param defines		List of preprocessor definitions
    @param flags		List of compiler flags
    @param errorBuffer	Buffer containing errors, created in function but must be deleted manually
*/
bool
GPULangCompile(const std::string& file, GPULang::Compiler::Language target, const std::string& output, const std::string& header_output, const std::vector<std::string>& defines, GPULang::Compiler::Options options, GPULangErrorBlob*& errorBuffer)
{
    bool ret = true;
    Allocator alloc = GPULang::CreateAllocator();
    GPULang::InitAllocator(&alloc);
    GPULang::MakeAllocatorCurrent(&alloc);

    std::string preprocessed;
    errorBuffer = nullptr;

    GPULang::Compiler::Timer timer;

    timer.Start();
    std::vector<GPULang::Symbol*> preprocessorSymbols;
    std::vector<GPULang::Diagnostic> diagnostics;
    if (GPULangPreprocessFile(file, defines, preprocessed, preprocessorSymbols, diagnostics))
    {
        // get the name of the shader
        std::locale loc;
        size_t extension = file.rfind('.');
        size_t lastFolder = file.rfind('/') + 1;
        std::string effectName = file.substr(lastFolder, (file.length() - lastFolder) - (file.length() - extension));
        effectName[0] = std::toupper(effectName[0]);
        size_t undersc = effectName.find('_');
        while (undersc != std::string::npos)
        {
            effectName[undersc + 1] = std::toupper(effectName[undersc + 1]);
            effectName = effectName.erase(undersc, 1);
            undersc = effectName.find('_');
        }

        // setup preprocessor
        //parser.preprocess();

        timer.Stop();
        if (options.emitTimings)
            timer.Print("Preprocessing");

        GPULangLexerErrorHandler lexerErrorHandler;
        GPULangParserErrorHandler parserErrorHandler;
        timer.Start();

        ANTLRInputStream input;
        GPULangLexer lexer(&input);
        lexer.setTokenFactory(GPULangTokenFactory::DEFAULT);
        CommonTokenStream tokens(&lexer);
        GPULangParser parser(&tokens);
        parser.getInterpreter<antlr4::atn::ParserATNSimulator>()->setPredictionMode(antlr4::atn::PredictionMode::SLL);

        // reload the preprocessed data
        input.reset();
        input.load(preprocessed);
        lexer.setInputStream(&input);
        lexer.setTokenFactory(GPULangTokenFactory::DEFAULT);
        lexer.removeErrorListeners();
        lexer.addErrorListener(&lexerErrorHandler);
        tokens.setTokenSource(&lexer);
        parser.setTokenStream(&tokens);
        parser.removeErrorListeners();
        parser.addErrorListener(&parserErrorHandler);

        Effect* effect = parser.entry()->returnEffect;
        GPULangParser::LineStack.clear();
        timer.Stop();
        if (options.emitTimings)
            timer.Print("Parsing");

        // if we have any lexer or parser error, return early
        if (lexerErrorHandler.hasError || parserErrorHandler.hasError)
        {
            std::string errorMessage;
            errorMessage.append(lexerErrorHandler.errorBuffer);
            errorMessage.append(parserErrorHandler.errorBuffer);
            errorBuffer = Error(errorMessage);
            return false;
        }

        // setup and run compiler
        BinWriter binaryWriter;
        binaryWriter.SetPath(output);
        TextWriter headerWriter;
        headerWriter.SetPath(header_output);

        Compiler compiler;
        compiler.path = file;
        compiler.filename = effectName;
        compiler.debugPath = output;
        compiler.debugOutput = true;
        compiler.Setup(target, defines, options);

        bool res = compiler.Compile(effect, binaryWriter, headerWriter);
        effect->Destroy();
        effect->~Effect();
        compiler.intrinsicScope->~Scope();
        compiler.mainScope->~Scope();
        GPULang::ResetAllocator(&alloc);
        
        // convert error list to string
        if (!compiler.messages.empty() && !compiler.options.quiet)
        {
            std::string err;
            for (size_t i = 0; i < compiler.messages.size(); i++)
            {
                if (i > 0)
                    err.append("\n");
                err.append(compiler.messages[i]);
            }
            if (err.empty() && compiler.hasErrors)
                err = "Unhandled internal compiler error";
            errorBuffer = Error(err);
        }

        return res;
    }
    std::string err;
    for (auto& diagnostic : diagnostics)
    {
        err += Format("%s(%d:%d): %s\n", diagnostic.file.c_str(), diagnostic.line, diagnostic.column, diagnostic.error.c_str());
    }
    errorBuffer = Error(err);
    return false;
}

//------------------------------------------------------------------------------
/**
    Runs compilation without output
*/
bool
GPULangValidateFile(const std::string& file, const std::vector<std::string>& defines, GPULang::Compiler::Options options, GPULangServerResult& result)
{
    bool ret = true;

    std::string preprocessed;

    GPULang::Compiler::Timer timer;
    timer.Start();

    std::vector<GPULang::Symbol*> preprocessorSymbols;
    std::vector<GPULang::Diagnostic> diagnostics;
    if (GPULangPreprocessFile(file, defines, preprocessed, preprocessorSymbols, diagnostics))
    {
        // get the name of the shader
        std::locale loc;
        size_t extension = file.rfind('.');
        size_t lastFolder = file.rfind('/') + 1;
        std::string effectName = file.substr(lastFolder, (file.length() - lastFolder) - (file.length() - extension));
        effectName[0] = std::toupper(effectName[0]);
        size_t undersc = effectName.find('_');
        while (undersc != std::string::npos)
        {
            effectName[undersc + 1] = std::toupper(effectName[undersc + 1]);
            effectName = effectName.erase(undersc, 1);
            undersc = effectName.find('_');
        }

        // setup preprocessor
        //parser.preprocess();

        timer.Stop();
        if (options.emitTimings)
            timer.Print("Preprocessing");


        GPULangLexerErrorHandler lexerErrorHandler;
        GPULangParserErrorHandler parserErrorHandler;
        timer.Start();

        ANTLRInputStream input;
        GPULangLexer lexer(&input);
        lexer.setTokenFactory(GPULangTokenFactory::DEFAULT);
        CommonTokenStream tokens(&lexer);
        GPULangParser parser(&tokens);
        parser.getInterpreter<antlr4::atn::ParserATNSimulator>()->setPredictionMode(antlr4::atn::PredictionMode::SLL);
        GPULangParser::LineStack.clear();

        input.load(preprocessed);
        lexer.setInputStream(&input);
        lexer.setTokenFactory(GPULangTokenFactory::DEFAULT);
        lexer.removeErrorListeners();
        lexer.addErrorListener(&lexerErrorHandler);
        tokens.setTokenSource(&lexer);
        parser.setTokenStream(&tokens);
        parser.removeErrorListeners();
        parser.addErrorListener(&parserErrorHandler);

        Effect* effect = parser.entry()->returnEffect;

        timer.Stop();
        if (options.emitTimings)
            timer.Print("Parsing");

        result.diagnostics.clear();

        // if we have any lexer or parser error, return early
        if (lexerErrorHandler.hasError || parserErrorHandler.hasError)
        {
            std::string errorMessage;
            errorMessage.append(lexerErrorHandler.errorBuffer);
            errorMessage.append(parserErrorHandler.errorBuffer);
            result.diagnostics.insert(result.diagnostics.end(), lexerErrorHandler.diagnostics.begin(), lexerErrorHandler.diagnostics.end());
            result.diagnostics.insert(result.diagnostics.end(), parserErrorHandler.diagnostics.begin(), parserErrorHandler.diagnostics.end());
        }

        Compiler compiler;
        compiler.path = file;
        compiler.filename = effectName;
        compiler.Setup(options);

        bool res = compiler.Validate(effect);
        effect->~Effect();

        result.root = effect;
        result.symbols = compiler.symbols;
        result.symbols.insert(result.symbols.begin(), preprocessorSymbols.begin(), preprocessorSymbols.end());
        result.intrinsicScope = compiler.intrinsicScope;
        result.mainScope = compiler.mainScope;
        if (!compiler.diagnostics.empty())
            result.diagnostics = compiler.diagnostics;
        
        // convert error list to string
        if (!compiler.messages.empty() && !compiler.options.quiet)
        {
            std::string err;
            for (size_t i = 0; i < compiler.messages.size(); i++)
            {
                if (i > 0)
                    err.append("\n");
                err.append(compiler.messages[i]);
            }
            if (err.empty() && compiler.hasErrors)
                err = "Unhandled internal compiler error";
            result.messages.push_back(err);
        }

        return res;
    }
    result.diagnostics.insert(result.diagnostics.end(), diagnostics.begin(), diagnostics.end());
    return false;
}

//------------------------------------------------------------------------------
/**
    Runs compilation without output
*/
bool
GPULangValidate(GPULangFile* file, const std::vector<std::string>& defines, GPULang::Compiler::Options options, GPULangServerResult& result)
{
    bool ret = true;

    std::string preprocessed;

    GPULang::Compiler::Timer timer;
    timer.Start();

    std::vector<GPULang::Symbol*> preprocessorSymbols;
    std::vector<GPULang::Diagnostic> diagnostics;
    if (GPULangPreprocess(file, defines, preprocessed, preprocessorSymbols, diagnostics))
    {
        // get the name of the shader
        std::locale loc;
        size_t extension = file->path.rfind('.');
        size_t lastFolder = file->path.rfind('/') + 1;
        std::string effectName = file->path.substr(lastFolder, (file->path.length() - lastFolder) - (file->path.length() - extension));
        effectName[0] = std::toupper(effectName[0]);
        size_t undersc = effectName.find('_');
        while (undersc != std::string::npos)
        {
            effectName[undersc + 1] = std::toupper(effectName[undersc + 1]);
            effectName = effectName.erase(undersc, 1);
            undersc = effectName.find('_');
        }

        // setup preprocessor
        //parser.preprocess();

        timer.Stop();
        if (options.emitTimings)
            timer.Print("Preprocessing");


        GPULangLexerErrorHandler lexerErrorHandler;
        GPULangParserErrorHandler parserErrorHandler;
        timer.Start();

        ANTLRInputStream input;
        GPULangLexer lexer(&input);
        lexer.setTokenFactory(GPULangTokenFactory::DEFAULT);
        CommonTokenStream tokens(&lexer);
        GPULangParser parser(&tokens);
        parser.getInterpreter<antlr4::atn::ParserATNSimulator>()->setPredictionMode(antlr4::atn::PredictionMode::SLL);
        GPULangParser::LineStack.clear();

        input.load(preprocessed);
        lexer.setInputStream(&input);
        lexer.setTokenFactory(GPULangTokenFactory::DEFAULT);
        lexer.removeErrorListeners();
        lexer.addErrorListener(&lexerErrorHandler);
        tokens.setTokenSource(&lexer);
        parser.setTokenStream(&tokens);
        parser.removeErrorListeners();
        parser.addErrorListener(&parserErrorHandler);

        Effect* effect = parser.entry()->returnEffect;

        timer.Stop();
        if (options.emitTimings)
            timer.Print("Parsing");

        result.diagnostics.clear();

        // if we have any lexer or parser error, return early
        if (lexerErrorHandler.hasError || parserErrorHandler.hasError)
        {
            std::string errorMessage;
            errorMessage.append(lexerErrorHandler.errorBuffer);
            errorMessage.append(parserErrorHandler.errorBuffer);
            result.diagnostics.insert(result.diagnostics.end(), lexerErrorHandler.diagnostics.begin(), lexerErrorHandler.diagnostics.end());
            result.diagnostics.insert(result.diagnostics.end(), parserErrorHandler.diagnostics.begin(), parserErrorHandler.diagnostics.end());
        }

        Compiler compiler;
        compiler.path = file->path;
        compiler.filename = effectName;
        compiler.Setup(options);

        bool res = compiler.Validate(effect);
        effect->~Effect();

        result.root = effect;
        result.symbols = compiler.symbols;
        result.symbols.insert(result.symbols.begin(), preprocessorSymbols.begin(), preprocessorSymbols.end());
        result.intrinsicScope = compiler.intrinsicScope;
        result.mainScope = compiler.mainScope;
        if (!compiler.diagnostics.empty())
            result.diagnostics = compiler.diagnostics;
        
        // convert error list to string
        if (!compiler.messages.empty() && !compiler.options.quiet)
        {
            std::string err;
            for (size_t i = 0; i < compiler.messages.size(); i++)
            {
                if (i > 0)
                    err.append("\n");
                err.append(compiler.messages[i]);
            }
            if (err.empty() && compiler.hasErrors)
                err = "Unhandled internal compiler error";
            result.messages.push_back(err);
        }

        return res;
    }
    result.diagnostics.insert(result.diagnostics.end(), diagnostics.begin(), diagnostics.end());
    return false;
}
