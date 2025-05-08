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

    static std::regex simple("\\s*([A-z]+)");
    std::cmatch matches;
    std::regex_search("  Foobar Blorf    ", matches, simple);

    static std::regex macrocallRegex("\\b([A-Z|a-z][A-Z|a-z|0-9|_]*)\\s*(?:\\((\\s*[A-Z|a-z][A-Z|a-z|0-9|_]*(?:\\s*,\\s*[A-Z|a-z][A-Z|a-z|0-9|_]*)*)?\\s*\\))?");
    static std::regex replaceRegex("\\b([A-Z|a-z][A-Z|a-z|0-9|_]*)");
    static std::regex inclRegex("\\binclude\\s+(?:<|\\\")([A-Z|a-z|\\/|.]+)(?:>|\\\").*");
    static std::regex macroRegex("\\bdefine\\s+([A-Z|a-z][A-Z|a-z|0-9|_]*)\\s*(?:\\(([A-Z|a-z][A-Z|a-z|_]*(?:\\s*,\\s*[A-Z|a-z][A-Z|a-z|_]*)*)\\s*\\)\\s*)?(.*)");
    static std::regex undefineRegex("\\bundef\\s+([A-Z|a-z|_]+).*");
    static std::regex ifdefRegex("\\bifdef\\s+([A-Z|a-z|_]+).*");
    static std::regex ifndefRegex("\\bifndef\\s+([A-Z|a-z|_]+).*");
    static std::regex elifdefRegex("\\belifdef\\s+([A-Z|a-z|_]+).*");
    static std::regex elifndefRegex("\\belifndef\\s+([A-Z|a-z|_]+).*");
    static std::regex ifRegex("\\bif\\s+([A-Z|a-z|_]+)\\s*((==|\\!=|>=|<=|<|>)\\s*([0-9]*))?.*");
    static std::regex elifRegex("\\belif\\s+([A-Z|a-z|_]+)\\s*(==|\\!=|>=|<=|<|>)\\s*([0-9]*).*");
    static std::regex elseRegex("\\belse.*");
    static std::regex endifRegex("\\bendif.*");
    static std::regex directiveStartRegex("\\s*#");
    static std::regex identifierRegex("\\b[A-Z|a-z][A-Z|a-z|_|0-9]*");
    static std::regex singleLineCommentRegex("\\s*//");
    static std::regex startMultilineCommentRegex("\\s*/\\*");
    static std::regex endMultilineCommentRegex("\\s*\\*/");

    FileLevel* level = &fileStack.back();
    Macro* macro = nullptr;
    bool comment = false;
    output.clear();
    char buf[4096];

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
        level->lineCounter++;
        std::string lineStr(line);
        int lineEndingLength = 0;
        if (lineStr.ends_with("\r\n"))
            lineEndingLength = 2;
        else if (lineStr.ends_with("\r") || lineStr.ends_with("\n"))
            lineEndingLength = 1;

        // Check for valid end of line, meaning the file is corrupt after this point
        auto charIterator = lineStr.rbegin();
        if (lineStr.length() >= 3 && (charIterator[2] == '\r' || charIterator[2] == '\n'))
            lineEndingLength = 0;
        if (lineEndingLength == 0)
        {
            diagnostics.push_back(Diagnostic{ .error = Format("Line is either too long or has corrupt file endings\n%s", lineStr.c_str()), .file = level->path, .line = level->lineCounter });
            POP_FILE()
        }

    escape_newline:

        if (lineStr.ends_with("\\\r\n") || lineStr.ends_with("\\\n"))
        {
            // Remove escape and newline
            lineStr.resize(lineStr.length() - lineEndingLength - 1);
            char* line = fgets(buf, sizeof(buf), level->file);
            lineStr += std::string(line);
            level->lineCounter++;
            goto escape_newline;
        }

        auto lineIt = lineStr.cbegin();
        auto lineEnd = lineStr.cend();

#define IFDEFSTACK() \
        if (!ifStack.empty())\
        {\
            if (!ifStack.back())\
            {\
                output.push_back('\n');\
                goto next_line;\
            }\
        }

#define SETUP_PP(pp)\
        pp->location.file = level->path;\
        pp->location.line = level->lineCounter;\
        pp->location.start = lineIt - lineStr.cbegin();\
        pp->location.end = matches.suffix().first - lineStr.cbegin();\
        preprocessorSymbols.push_back(pp);

        std::smatch matches;

        if (std::regex_search(lineIt, lineEnd, matches, directiveStartRegex))
        {
            auto startOfDirective = matches.suffix().first;
            auto directiveIt = matches.suffix().first;
            if (std::regex_search(directiveIt, lineEnd, matches, inclRegex))
            {
                IFDEFSTACK()
                const std::string& path = matches[1];

                auto pp = Alloc<Preprocessor>();
                pp->type = Preprocessor::Include;
                SETUP_PP(pp)
                
                std::string foundPath;
                FILE* inc = LoadFile(path.c_str(), searchPaths, foundPath);
                output.append(Format("#line %d \"%s\"\n", level->lineCounter+1, level->path.c_str()));

                fileStack.push_back({ foundPath, inc });
                level = &fileStack.back();
                continue;
            }

            else if (std::regex_search(directiveIt, lineEnd, matches, macroRegex))
            {
                IFDEFSTACK()
                macro = &definitions[matches[1]];

                auto pp = Alloc<Preprocessor>();
                pp->type = Preprocessor::Macro;
                SETUP_PP(pp)
                pp->location.end = matches[3].first - lineIt;

                // Macro with argument path
                if (matches[2].matched)
                {
                    std::string argList = matches[2].str();
                    std::smatch argMatches;
                    while (std::regex_search(argList, argMatches, identifierRegex))
                    {
                        macro->args.push_back(argMatches[0]);
                        argList = argMatches.suffix();
                    }
                    macro->contents = matches[3];
                }
                else
                {
                    // Simple macro to value path
                    macro->contents = matches[3];
                }
                output.append(Format("#line %d \"%s\"", level->lineCounter+1, level->path.c_str()));
            }
            else if (std::regex_search(directiveIt, lineEnd, matches, undefineRegex))
            {
                IFDEFSTACK()
                auto pp = Alloc<Preprocessor>();
                pp->type = Preprocessor::Undefine;
                SETUP_PP(pp)

                if (definitions.contains(matches[1]))
                    definitions.erase(matches[1]);
            }
            else if (std::regex_search(directiveIt, lineEnd, matches, ifdefRegex))
            {
                IFDEFSTACK()
                auto pp = Alloc<Preprocessor>();
                pp->type = Preprocessor::If;
                SETUP_PP(pp)
                auto it = definitions.find(matches[1]);
                if (it != definitions.end())
                {
                    ifStack.push_back(true);
                }
                else
                {
                    ifStack.push_back(false);
                }
            }
            else if (std::regex_search(directiveIt, lineEnd, matches, elifdefRegex))
            {
                auto pp = Alloc<Preprocessor>();
                pp->type = Preprocessor::If;
                SETUP_PP(pp)
                auto it = definitions.find(matches[1]);
                if (it != definitions.end())
                {
                    ifStack.back() = true;
                }
                else
                {
                    ifStack.back() = false;
                }
            }
            else if (std::regex_search(directiveIt, lineEnd, matches, ifndefRegex))
            {
                IFDEFSTACK()
                auto pp = Alloc<Preprocessor>();
                pp->type = Preprocessor::If;
                SETUP_PP(pp)
                auto it = definitions.find(matches[1]);
                if (it == definitions.end())
                {
                    ifStack.push_back(true);
                }
                else
                {
                    ifStack.push_back(false);
                }
            }
            else if (std::regex_search(directiveIt, lineEnd, matches, elifndefRegex))
            {
                auto pp = Alloc<Preprocessor>();
                pp->type = Preprocessor::If;
                SETUP_PP(pp)
                auto it = definitions.find(matches[1]);
                if (it == definitions.end())
                {
                    ifStack.back() = true;
                }
                else
                {
                    ifStack.back() = false;
                }
            }
            else if (std::regex_search(directiveIt, lineEnd, matches, ifRegex))
            {
                IFDEFSTACK()

                auto pp = Alloc<Preprocessor>();
                pp->type = Preprocessor::If;
                SETUP_PP(pp)
                auto it = definitions.find(matches[1]);
                if (it != definitions.end())
                {
                    if (matches[2].matched)
                    {
                        if (matches[2] == "==")
                        {
                            ifStack.push_back(it->second.contents == matches[3]);
                        }
                        else if (matches[2] == "!=")
                        {
                            ifStack.push_back(it->second.contents != matches[3]);
                        }
                        else if (matches[2] == ">=")
                        {
                            ifStack.push_back(std::stoi(it->second.contents) >= std::stoi(matches[3]));
                        }
                        else if (matches[2] == "<=")
                        {
                            ifStack.push_back(std::stoi(it->second.contents) <= std::stoi(matches[3]));
                        }
                        else if (matches[2] == ">")
                        {
                            ifStack.push_back(std::stoi(it->second.contents) > std::stoi(matches[3]));
                        }
                        else if (matches[2] == "<")
                        {
                            ifStack.push_back(std::stoi(it->second.contents) < std::stoi(matches[3]));
                        }
                    }
                    else
                    {
                        ifStack.push_back(it->second.contents == "1");
                    }
                }
            }
            else if (std::regex_search(directiveIt, lineEnd, matches, elifRegex))
            {
                auto pp = Alloc<Preprocessor>();
                pp->type = Preprocessor::If;
                SETUP_PP(pp)
                auto it = definitions.find(matches[1]);
                if (matches[2] == "==")
                {
                    ifStack.back() = it->second.contents == matches[3];
                }
                else if (matches[2] == "!=")
                {
                    ifStack.back() = it->second.contents != matches[3];
                }
                else if (matches[2] == ">=")
                {
                    ifStack.back() = std::stoi(it->second.contents) >= std::stoi(matches[3]);
                }
                else if (matches[2] == "<=")
                {
                    ifStack.back() = std::stoi(it->second.contents) <= std::stoi(matches[3]);
                }
                else if (matches[2] == ">")
                {
                    ifStack.back() = std::stoi(it->second.contents) > std::stoi(matches[3]);
                }
                else if (matches[2] == "<")
                {
                    ifStack.back() = std::stoi(it->second.contents) < std::stoi(matches[3]);
                }
            }
            else if (std::regex_search(directiveIt, lineEnd, matches, elseRegex))
            {
                auto pp = Alloc<Preprocessor>();
                pp->type = Preprocessor::If;
                SETUP_PP(pp)
                ifStack.back() = !ifStack.back();
            }
            else if (std::regex_search(directiveIt, lineEnd, matches, endifRegex))
            {
                auto pp = Alloc<Preprocessor>();
                pp->type = Preprocessor::EndIf;
                SETUP_PP(pp)
                if (ifStack.empty())
                {
                    diagnostics.push_back(GPULang::Diagnostic{.error = "Invalid #endif, missing matching #if/#ifdef/#ifndef", .file = level->path, .line = level->lineCounter});
                    return false;
                }
                else
                    ifStack.pop_back();
            }
            else
            {
                diagnostics.push_back(GPULang::Diagnostic{.error = Format("Invalid preprocessor directive '%s'\n", lineStr.c_str()), .file = level->path, .line = level->lineCounter});
                return false;
            }
            output.append("\n");

            // Remove line
            goto next_line;
        }
        else // if not a directive
        {
            IFDEFSTACK()
match:
            if (!comment && std::regex_search(lineIt, lineEnd, matches, singleLineCommentRegex))
            {
                auto pp = Alloc<Preprocessor>();
                pp->type = Preprocessor::Comment;
                SETUP_PP(pp)
                pp->location.end = lineEnd - lineIt;

                output.append(matches.prefix());
                output.push_back('\n');
                goto next_line;
            }
            else if (!comment && std::regex_search(lineIt, lineEnd, matches, startMultilineCommentRegex))
            {
                comment = true;
                output.append(matches.prefix());
                // Match the rest of the line for a potential end multiline 
                std::string rest = matches.suffix().str();
                if (std::regex_search(rest, matches, endMultilineCommentRegex))
                {
                    comment = false;
                    // Anything that might come after should be output (including newline)
                    output.append(matches.suffix());
                    goto next_line;
                }
                else
                    output.push_back('\n');
                goto next_line;
            }
            else if (std::regex_search(lineIt, lineEnd, matches, endMultilineCommentRegex))
            {
                comment = false;
                // Anything that might come after should be output (including newline)
                output.append(matches.suffix());
                goto next_line;
            }
            else if (!comment && std::regex_search(lineIt, lineEnd, matches, macrocallRegex))
            {
                output.append(matches.prefix());
                auto it = definitions.find(matches[1]);
                if (it != definitions.end())
                {
                    auto pp = Alloc<Preprocessor>();
                    pp->type = Preprocessor::Call;
                    SETUP_PP(pp)

                    // Parse arguments
                    if (matches[2].matched)
                    {
                        std::string argList = matches[2].str();
                        std::vector<std::string> args;
                        std::smatch argMatches;
                        while (std::regex_search(argList, argMatches, identifierRegex))
                        {
                            args.push_back(argMatches[0]);
                            argList = argMatches.suffix();
                        }

                        std::string content = it->second.contents;

                        // Warn if mismatch
                        int maxIterations = min(args.size(), it->second.args.size());
                        int argCounter = 0;
                        for (auto& arg : it->second.args)
                        {
                            if (argCounter >= maxIterations)
                                break;
                            content = std::regex_replace(content, std::regex(arg), args[argCounter]);
                            argCounter++;
                        }
                        output.append(content);
                    }
                    else
                    {
                        auto pp = Alloc<Preprocessor>();
                        pp->type = Preprocessor::Call;
                        SETUP_PP(pp)
                        output.append(it->second.contents);
                    }
                }
                else
                {
                    output.append(matches[0]);
                }
                lineIt = matches.suffix().first;
                goto match;
            }
        }

        if (comment)
        {
            output.push_back('\n');
            goto next_line;
        }

        output.append(lineIt, lineEnd);
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
