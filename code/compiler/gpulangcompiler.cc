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

using namespace antlr4;
using namespace GPULang;

#include "mcpp_lib.h"
#include "mcpp_out.h"

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

//------------------------------------------------------------------------------
/**
*/
FILE*
LoadFile(const char* path, const std::vector<std::string>& searchPaths, std::string& foundPath)
{
    FILE* f = fopen(path, "rb");
    
    if (f == nullptr)
    {
        for (auto& searchPath : searchPaths)
        {
            foundPath = searchPath + std::string(path);
            f = fopen(foundPath.c_str(), "rb");
            if (f != nullptr)
                break;
        }
    }

    return f;
}


//------------------------------------------------------------------------------
/**
*/
bool
GPULangPreprocess(
    const std::string& file
    , const std::vector<std::string>& defines
    , std::string& output
    , std::vector<GPULang::Symbol*>& preprocessorSymbols
    , std::vector<GPULang::Diagnostic>& diagnostics
)
{
    std::string escaped = FixBackslashes(file);
    std::string fileName = file.substr(escaped.rfind("/")+1, escaped.length()-1);
    std::string folder = escaped.substr(0, escaped.rfind("/")+1);
    std::string dummy;
    FILE* f = LoadFile(file.c_str(), {}, dummy);

    struct FileLevel
    {
        int lineCounter;
        std::string path;
        FILE* file;

        FileLevel(const std::string& path, FILE* file)
        {
            this->path = path;
            this->file = file;
            this->lineCounter = 0;
        }

        FileLevel(FileLevel&& rhs) noexcept
        {
            this->lineCounter = rhs.lineCounter;
            this->file = rhs.file;
            this->path = std::move(rhs.path);
        }

        FileLevel(const FileLevel& rhs)
        {
            this->lineCounter = rhs.lineCounter;
            this->path = rhs.path;
            this->file = rhs.file;
        }
    };

    struct Macro
    {
        std::vector<std::string> args;
        std::string contents;
    };
    std::map<std::string, Macro> definitions;

    std::vector<bool> ifStack;
    std::vector<FileLevel> fileStack{ { file, f } };
    std::vector<std::string> searchPaths;
    for (auto arg : defines)
    {
        if (arg[0] == '-')
        {
            if (arg[1] == 'I')
                searchPaths.push_back(arg.substr(2));
            else if (arg[1] == 'D')
                definitions[arg.substr(2)] = Macro{ .contents = "" };
        }

    }

    static auto validIdentifierStart = [](const char c) -> bool
    {
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == '_'))
            return true;
        return false;
    };

    static auto validIdentifierChar = [](const char c) -> bool
    {
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || (c == '_'))
            return true;
        return false;
    };

    static auto validateIdentifier = [](const char* begin, const char* end) -> bool
    {
        if (!validIdentifierStart(begin[0]))
            return false;
        const char* start = begin+1;
        while (start != end)
        {
            if (!validIdentifierChar(start[0]))
                return false;
            start++;
        }
        return true;
    };

    static auto wordStart = [](const char* begin, const char* end) -> const char*
    {
        const char* start = begin;
        while (start != end)
        {
            if (start[0] != ' ' && start[0] != '\t' && start[0] != '\n' && start[0] != '\r' && start[0] != '\f' && start[0] != '\v')
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
            if (start[0] == ' ' || start[0] == '\t' || start[0] == '\n' || start[0] == '\r' || start[0] == '\f' || start[0] == '\v')
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
            if (validIdentifierStart(start[0]))
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
            if (!validIdentifierChar(start[0]))
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

    FileLevel* level = &fileStack.back();
    Macro* macro = nullptr;
    bool comment = false;
    output.clear();
    char buf[4096];
    int lineOffset = 1;
    GPULang::Preprocessor* prevMultiline = nullptr;

#define POP_FILE() \
            fileStack.pop_back();\
            if (!fileStack.empty())\
            {\
                level = &fileStack.back();\
                output.append(Format("#line %d \"%s\"\n", level->lineCounter+1, level->path.c_str()));\
            }\
            continue;
    while (!fileStack.empty())
    {
        // Find next unescaped \n
    next_line:
        char* line = fgets(buf, sizeof(buf), level->file);
        if (line == nullptr)
        {
            POP_FILE()
        }
        level->lineCounter += lineOffset;
        lineOffset = 1;
        std::string lineStr(line);
        int lineEndingLength = 0;
        if (lineStr.ends_with("\r\n"))
            lineEndingLength = 2;
        else if (lineStr.ends_with("\r") || lineStr.ends_with("\n"))
            lineEndingLength = 1;

        // Check for valid end of line, meaning the file is corrupt after this point
        auto charIterator = lineStr.rbegin();
        if (lineStr.length() >= 3 && (charIterator[2] == '\r' || charIterator[2] == '\n'))
            lineEndingLength = -1;
        if (lineEndingLength == -1)
        {
            //diagnostics.push_back(Diagnostic{ .error = Format("Line is either too long or has corrupt file endings\n%s", lineStr.c_str()), .file = level->path, .line = level->lineCounter });
            POP_FILE()
        }

    escape_newline:

        if (lineStr.ends_with("\\\r\n") || lineStr.ends_with("\\\n"))
        {
            // Remove escape and newline
            lineStr.resize(lineStr.length() - lineEndingLength - 1);
            char* line = fgets(buf, sizeof(buf), level->file);
            lineStr += std::string(line);
            lineOffset++;
            goto escape_newline;
        }

        auto lineIt = lineStr.cbegin();
        auto lineEnd = lineStr.cend();

        // Macro to eliminate directives and output if an if-scope is inactive
#define IFDEFSTACK() \
        if (!ifStack.empty())\
        {\
            if (!ifStack.back())\
            {\
                auto pp = Alloc<Preprocessor>();\
                pp->type = Preprocessor::Comment;\
                pp->location.file = level->path;\
                pp->location.line = level->lineCounter;\
                pp->location.column = 0;\
                pp->location.start = 0;\
                pp->location.end = lineStr.length();\
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
                pp->location.file = level->path;\
                pp->location.line = level->lineCounter;\
                pp->location.column = 0;\
                pp->location.start = 0;\
                pp->location.end = lineStr.length();\
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
                pp->location.file = level->path;\
                pp->location.line = level->lineCounter;\
                pp->location.column = 0;\
                pp->location.start = 0;\
                pp->location.end = lineStr.length();\
                preprocessorSymbols.push_back(pp);\
                output.push_back('\n');\
                goto next_line;\
            }\
        }

#define SETUP_PP2(pp, begin, stop)\
        pp->location.file = level->path;\
        pp->location.line = level->lineCounter;\
        pp->location.start = begin - lineBegin;\
        pp->location.column = pp->location.start;\
        pp->location.end = stop - lineBegin;\
        preprocessorSymbols.push_back(pp);

#define SETUP_ARG2(pp, arg, begin, stop)\
        pp->args.push_back(arg);\
        Symbol::Location argLoc;\
        argLoc.file = level->path;\
        argLoc.line = level->lineCounter;\
        argLoc.start = begin - lineBegin;\
        argLoc.column = pp->location.start;\
        argLoc.end = stop - lineBegin;\
        pp->argLocations.push_back(argLoc);

#define DIAGNOSTIC(message)\
        GPULang::Diagnostic{.error = message, .file = level->path, .line = level->lineCounter }

        std::smatch matches;

        const char* lineBegin = &(*lineIt);
        const char* eol = &(*(lineEnd - 1)) + 1;
        const char* columnIt = lineBegin;
        const char* prevColumnIt = columnIt;
        do 
        {
            // Find beginning of first non-WS character
            const char* firstWord = wordStart(columnIt, eol);
            if (firstWord[0] == '#') // Directives
            {
                firstWord++;
                const char* startOfDirective = wordStart(firstWord, eol);
                const char* endOfDirective = wordEnd(firstWord, eol);
                if (strncmp(startOfDirective, "include", endOfDirective - startOfDirective) == 0) // Include
                {
                    IFDEFSTACK()

                    const char* startOfPath = wordStart(endOfDirective, eol);
                    const char* endOfPath = wordEnd(startOfPath, eol);
                    auto pp = Alloc<Preprocessor>();
                    pp->type = Preprocessor::Include;
                    SETUP_PP2(pp, startOfDirective, endOfDirective);

                    if ((startOfPath[0] == '"' && endOfPath[-1] == '"')
                        || (startOfPath[0] == '<' && endOfPath[-1] == '>'))
                    {
                        const std::string path = std::string(startOfPath + 1, endOfPath - 1);
                        SETUP_ARG2(pp, path, startOfPath, endOfPath);

                        std::string foundPath;
                        FILE* inc = LoadFile(path.c_str(), searchPaths, foundPath);
                        if (inc == nullptr)
                        {
                            diagnostics.push_back(DIAGNOSTIC(Format("File not found '%s'", foundPath.c_str())));
                            return false;
                        }
                        output.append(Format("#line %d \"%s\"\n", level->lineCounter + 1, level->path.c_str()));

                        fileStack.push_back({ foundPath, inc });
                        level = &fileStack.back();
                    }
                    else
                    {
                        diagnostics.push_back(DIAGNOSTIC("include directory must be provided a path"));
                        return false;
                    }
                    goto next_line;
                }
                else if (strncmp(startOfDirective, "define", endOfDirective - startOfDirective) == 0)
                {
                    IFDEFSTACK()
                    const char* startOfDefinition = wordStart(endOfDirective, eol);
                    if (startOfDefinition == nullptr)
                    {
                        diagnostics.push_back(DIAGNOSTIC("define missing identifier"));
                        return false;
                    }
                    const char* endOfDefinition = wordEnd(startOfDefinition, eol);
                    const char* endOfIdentifier = identifierEnd(startOfDefinition, endOfDefinition);

                    if (!validateIdentifier(startOfDefinition, endOfIdentifier))
                    {
                        diagnostics.push_back(DIAGNOSTIC("Invalid macro identifier"));
                        return false;
                    }

                    std::string def = std::string(startOfDefinition, endOfIdentifier);
                    macro = &definitions[def];

                    auto pp = Alloc<Preprocessor>();
                    pp->type = Preprocessor::Macro;
                    SETUP_PP2(pp, startOfDefinition, endOfDefinition)

                    const char* startOfContents = wordStart(endOfDefinition, eol);
                    
                    if (endOfIdentifier[0] == '(')
                    {
                        // Argument list, start parsing arguments
                        const char* argumentIt = wordStart(endOfIdentifier+1, endOfDefinition);
                        while (argumentIt != endOfDefinition)
                        {
                            if (argumentIt[0] == ')') // end of list
                            {
                                // Move new start to after argument list
                                startOfContents = wordStart(argumentIt+1, eol);
                                break;
                            }
                            const char* argumentEnd = identifierEnd(argumentIt, endOfDefinition);

                            if (!validateIdentifier(argumentIt, argumentEnd))
                            {
                                diagnostics.push_back(DIAGNOSTIC("Invalid argument identifier"));
                                return false;
                            }

                            // Identifier validation done, save argument
                            macro->args.push_back(std::string(argumentIt, argumentEnd));
                            argumentIt = nextArg(argumentIt, endOfDefinition);
                        }

                        // append the rest of the contents
                        macro->contents.append(startOfContents, eol-lineEndingLength);
                    }
                    else if (startOfContents != eol)
                    {
                        macro->contents = std::string(startOfContents, eol - lineEndingLength);
                    }               
                    output.append(Format("#line %d \"%s\"", level->lineCounter + 1, level->path.c_str()));
                }
                else if (strncmp(startOfDirective, "undef", endOfDirective - startOfDirective) == 0)
                {
                    IFDEFSTACK()

                    const char* startOfDefinition = wordStart(endOfDirective, eol);
                    if (startOfDefinition == nullptr)
                    {
                        diagnostics.push_back(DIAGNOSTIC("undef missing identifier"));
                        return false;
                    }
                    const char* endOfDefinition = wordEnd(startOfDefinition, eol);

                    if (!validateIdentifier(startOfDefinition, endOfDefinition))
                    {
                        diagnostics.push_back(DIAGNOSTIC("Invalid undef identifier"));
                        return false;
                    }

                    auto pp = Alloc<Preprocessor>();
                    pp->type = Preprocessor::Undefine;
                    SETUP_PP2(pp, startOfDefinition, endOfDefinition);

                    std::string definition = std::string(startOfDefinition, endOfDefinition);
                    auto it = definitions.find(definition);
                    if (it != definitions.end())
                    {
                        definitions.erase(definition);
                    }
                }
                else if (strncmp(startOfDirective, "ifdef", endOfDirective - startOfDirective) == 0)
                {
                    IFDEFSTACKPUSH()

                    const char* startOfDefinition = wordStart(endOfDirective, eol);
                    if (startOfDefinition == nullptr)
                    {
                        diagnostics.push_back(DIAGNOSTIC("ifdef missing identifier"));
                        return false;
                    }
                    const char* endOfDefinition = wordEnd(startOfDefinition, eol);

                    if (!validateIdentifier(startOfDefinition, endOfDefinition))
                    {
                        diagnostics.push_back(DIAGNOSTIC("Invalid ifdef identifier"));
                        return false;
                    }

                    auto pp = Alloc<Preprocessor>();
                    pp->type = Preprocessor::If;
                    SETUP_PP2(pp, startOfDefinition, endOfDefinition);

                    std::string definition = std::string(startOfDefinition, endOfDefinition);
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
                else if (strncmp(startOfDirective, "ifndef", endOfDirective - startOfDirective) == 0)
                {
                    IFDEFSTACKPUSH()

                    const char* startOfDefinition = wordStart(endOfDirective, eol);
                    if (startOfDefinition == nullptr)
                    {
                        diagnostics.push_back(DIAGNOSTIC("ifdef missing identifier"));
                        return false;
                    }
                    const char* endOfDefinition = wordEnd(startOfDefinition, eol);

                    if (!validateIdentifier(startOfDefinition, endOfDefinition))
                    {
                        diagnostics.push_back(DIAGNOSTIC("Invalid ifdef identifier"));
                        return false;
                    }

                    auto pp = Alloc<Preprocessor>();
                    pp->type = Preprocessor::If;
                    SETUP_PP2(pp, startOfDefinition, endOfDefinition);

                    std::string definition = std::string(startOfDefinition, endOfDefinition);
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
                else if (strncmp(startOfDirective, "elif", endOfDirective - startOfDirective) == 0)
                {

                }
                else if (strncmp(startOfDirective, "if", endOfDirective - startOfDirective) == 0)
                {

                }
                else if (strncmp(startOfDirective, "else", endOfDirective - startOfDirective) == 0)
                {
                    ELIFDEFSTACKPUSH()
                    auto pp = Alloc<Preprocessor>();
                    pp->type = Preprocessor::If;
                    SETUP_PP2(pp, startOfDirective, endOfDirective)
                    ifStack.back() = !ifStack.back();
                }
                else if (strncmp(startOfDirective, "endif", endOfDirective - startOfDirective) == 0)
                {
                    auto pp = Alloc<Preprocessor>();
                    pp->type = Preprocessor::EndIf;
                    SETUP_PP2(pp, startOfDirective, endOfDirective)
                    if (ifStack.empty())
                    {
                        diagnostics.push_back(GPULang::Diagnostic{ .error = "Invalid #endif, missing matching #if/#ifdef/#ifndef", .file = level->path, .line = level->lineCounter });
                        return false;
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
                columnIt = firstWord + 2;
                prevMultiline = pp;
            }
            else if (comment && firstWord[0] == '*' && firstWord[1] == '/')
            {
                comment = false;
                assert(prevMultiline != nullptr);
                auto back = static_cast<GPULang::Preprocessor*>(prevMultiline);
                back->location.end = firstWord + 2 - lineBegin;
                prevMultiline = nullptr;
                columnIt = firstWord + 2;
            }
            else if (!comment)
            {
                IFDEFSTACK()
                const char* startOfWord = wordStart(columnIt, eol);
                const char* endOfWord = wordEnd(startOfWord, eol);

                const char* startOfIdentifier = identifierBegin(startOfWord, eol);
                const char* endOfIdentifier = identifierEnd(startOfIdentifier, eol);
                std::string word = std::string(startOfIdentifier, endOfIdentifier);

                // Add whatever comes before the first identifier
                output.append(columnIt, startOfWord);
                auto it = definitions.find(word);
                if (it != definitions.end())
                {
                    auto pp = Alloc<Preprocessor>();
                    pp->type = Preprocessor::Call;
                    SETUP_PP2(pp, startOfIdentifier, endOfIdentifier)

                    if (endOfIdentifier[0] == '(')
                    {
                        const char* argumentIt = wordStart(endOfIdentifier + 1, eol);
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
                            if (!validIdentifierStart(argumentIt[0])) // First argument may not be a number
                            {
                                diagnostics.push_back(DIAGNOSTIC(Format("Invalid argument character '%c'", argumentIt[0])));
                                return false;
                            }
                            else
                            {
                                // If first char is valid, check the rest
                                const char* argumentCharIt = argumentIt + 1;
                                while (argumentCharIt != argumentEnd)
                                {
                                    if (!validIdentifierChar(argumentCharIt[0]))
                                    {
                                        diagnostics.push_back(DIAGNOSTIC(Format("Invalid argument character '%c'", argumentCharIt[0])));
                                        return false;
                                    }
                                    argumentCharIt++;
                                }
                            }
                            args.push_back(std::string(argumentIt, argumentEnd));
                            argumentIt = nextArg(argumentIt, eol);
                        }
                        if (!valid)
                        {
                            {
                                diagnostics.push_back(DIAGNOSTIC("Macro call missing ')'"));
                                return false;
                            }
                        }
                        endOfWord = argumentIt+1;
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
                            const char* startOfIdentifier = identifierBegin(macroContentsIt, macroContentsEnd);
                            const char* endOfIdentifier = identifierEnd(startOfIdentifier, macroContentsEnd);

                            // Add whatever comes before the identifier to the output
                            output.append(macroContentsIt, startOfIdentifier);
                            if (macroContentsEnd == startOfIdentifier)
                                break;

                            std::string word(startOfIdentifier, endOfIdentifier);

                            auto argIt = argumentMap.find(word);
                            if (argIt == argumentMap.end())
                            {
                                auto defIt = definitions.find(word);
                                if (defIt == definitions.end())
                                {
                                    output.append(word);
                                }
                                else
                                {
                                    output.append(defIt->second.contents);
                                }
                            }
                            else
                            {
                                output.append(argIt->second);
                            }

                            macroContentsIt = endOfIdentifier;
                        }
                    }
                    else
                    {
                        const char* macroContentsIt = &(*it->second.contents.begin());
                        const char* macroContentsEnd = &(*(it->second.contents.end() - 1)) + 1;

                        // Replace macro contents with definitions and arguments
                        while (macroContentsIt != macroContentsEnd)
                        {
                            const char* startOfIdentifier = identifierBegin(macroContentsIt, macroContentsEnd);
                            const char* endOfIdentifier = identifierEnd(startOfIdentifier, macroContentsEnd);

                            // Add whatever comes before the identifier to the output
                            output.append(macroContentsIt, startOfIdentifier);
                            if (macroContentsEnd == startOfIdentifier)
                                break;

                            std::string word(startOfIdentifier, endOfIdentifier);

                            auto defIt = definitions.find(word);
                            if (defIt == definitions.end())
                            {
                                output.append(word);
                            }
                            else
                            {
                                output.append(defIt->second.contents);
                            }

                            macroContentsIt = endOfIdentifier;
                        }
                    }
                }
                else
                {
                    output.append(startOfWord, endOfWord);
                }
                columnIt = endOfWord;
            }
            if (comment)
            {
                columnIt = nextComment(columnIt, eol);
            }

            // Go to next non white space word
            prevColumnIt = columnIt;
        } while (columnIt != eol);
    }
    return true;
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
    if (GPULangPreprocess(file, defines, preprocessed, preprocessorSymbols, diagnostics))
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
    Compiles GPULang file.

    @param file			Input file to compile
    @param flags		List of compiler flags
*/
bool
GPULangValidate(const std::string& file, const std::vector<std::string>& defines, GPULang::Compiler::Options options, GPULangServerResult& result)
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
