//------------------------------------------------------------------------------
/**
    AnyFX compiler functions
    
    (C) 2013 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "cmdlineargs.h"
#include "gpulangcompiler.h"
#include "compiler.h"
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
std::string
LoadFile(const char* path, const std::vector<std::string>& searchPaths, std::string& foundPath)
{
    FILE* f = fopen(path, "r");
    
    if (f == nullptr)
    {
        for (auto& searchPath : searchPaths)
        {
            foundPath = searchPath + std::string(path);
            f = fopen(foundPath.c_str(), "r");
            if (f != nullptr)
                break;
        }
    }

    fseek(f, 0, SEEK_END);
    int size = ftell(f);
    rewind(f);
    GPULang::StackArray<char> arr(size);
    fread(arr.ptr, size, 1, f);
    fclose(f);
    return std::string(arr.ptr, arr.ptr + size);
}


//------------------------------------------------------------------------------
/**
*/
bool
GPULangPreprocess(const std::string& file, const std::vector<std::string>& defines, std::string& output, std::string& err)
{
    std::string escaped = FixBackslashes(file);
    std::string fileName = file.substr(escaped.rfind("/")+1, escaped.length()-1);
    std::string folder = escaped.substr(0, escaped.rfind("/")+1);
    std::string dummy;
    std::string f = LoadFile(file.c_str(), {}, dummy);

    struct FileLevel
    {
        int lineCounter;
        std::string path;
        std::string contents;
        char* line;

        FileLevel(const std::string& path, std::string&& contents)
        {
            this->path = path;
            this->contents = contents;
            this->line = this->contents.data();
            this->lineCounter = 0;
        }

        FileLevel(FileLevel&& rhs) noexcept
        {
            this->lineCounter = rhs.lineCounter;
            this->line = rhs.line;
            this->path = std::move(rhs.path);
            this->contents = std::move(rhs.contents);
            rhs.line = nullptr;
        }

        FileLevel(const FileLevel& rhs)
        {
            this->lineCounter = rhs.lineCounter;
            this->path = rhs.path;
            this->contents = rhs.contents;
            this->line = this->contents.data();

        }
    };

    struct Macro
    {
        std::vector<std::string> args;
        std::string contents;
    };
    std::map<std::string, Macro> definitions;

    std::vector<bool> ifStack;
    std::vector<FileLevel> fileStack{ { file, std::move(f) } };
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

    static std::regex macrocallRegex("\\b([A-z][A-z|0-9|_]*)\\s*(?:\\((\\s*[A-z][A-z|0-9|_]*(?:\\s*,\\s*[A-z][A-z|0-9|_]*)*)?\\s*\\))?");
    static std::regex inclRegex("\\binclude\\s+<([A-z|\\/|.]+)>");
    static std::regex macroRegex("\\bdefine\\s+([A-z][A-z|0-9|_]*)\\s*(?:\\(([A-z][A-z|_]*(?:\\s*,\\s*[A-z][A-z|_]*)*)\\s*\\)\\s*)?([\\s\\S]*)");
    static std::regex undefineRegex("\\bundef\\s+([A-z|_]+).*");
    static std::regex ifdefRegex("\\bifdef\\s+([A-z|_]+).*");
    static std::regex ifndefRegex("\\bifndef\\s+([A-z|_]+).*");
    static std::regex elifdefRegex("\\belifdef\\s+([A-z|_]+).*");
    static std::regex elifndefRegex("\\belifndef\\s+([A-z|_]+).*");
    static std::regex ifRegex("\\bif\\s+([A-z|_]+)\\s*(==|\\!=|>=|<=|<|>)\\s*([0-9]*).*");
    static std::regex elifRegex("\\belif\\s+([A-z|_]+)\\s*(==|\\!=|>=|<=|<|>)\\s*([0-9]*).*");
    static std::regex elseRegex("\\belse.*");
    static std::regex endifRegex("\\bendif.*");
    static std::regex directiveStartRegex("\\s*#");


    FileLevel* level = &fileStack.back();
    Macro* macro = nullptr;
    bool comment = false;
    output.clear();
    while (!fileStack.empty())
    {
        // Find next unescaped \n
        char* eol = strchr(level->line, '\n');
        if (eol != nullptr)
        {
            if (eol[-1] == '\\')
            {
                eol = strchr(eol + 1, '\n');
            }
        }

        char* nextLine = eol;

        if (nextLine == nullptr)
            nextLine = strchr(level->line, '\0');
        else
            nextLine += 1;

        std::string lineStr(level->line, nextLine - level->line);
        std::smatch matches;

        if (std::regex_search(lineStr, matches, macrocallRegex))
        {
            auto it = definitions.find(matches[1]);
            if (it != definitions.end())
            {
                // Parse arguments
                std::string argList = matches[2].str();
                std::vector<std::string> args;
                char* arg = argList.data();
                while (arg != nullptr)
                {
                    char* nextArg = strchr(arg, ',');
                    if (nextArg != nullptr)
                    {
                        args.push_back(std::string(arg, nextArg - arg));
                        arg = nextArg + 1;
                    }
                    else
                    {
                        args.push_back(arg);
                        arg = nullptr;
                    }
                }

                std::string content = it->second.contents;
                int argCounter = 0;
                for (auto& arg : it->second.args)
                {
                    if (argCounter > args.size())
                        break;
                    content = std::regex_replace(content, std::regex(arg), args[argCounter]);
                    argCounter++;
                }
                output.append(content);
            }
        }


        if (std::regex_search(lineStr, matches, directiveStartRegex))
        {
            std::string directiveString = matches.suffix().str();
            if (std::regex_match(directiveString, matches, inclRegex))
            {
                const std::string& path = matches[1];
                
                std::string foundPath;
                std::string incl = LoadFile(path.c_str(), searchPaths, foundPath);
                output.append(Format("#line %s %d\n", level->path.c_str(), level->lineCounter));
                level->line = strchr(level->line, '\n') + 1;

                fileStack.push_back({ foundPath, std::move(incl) });
                level = &fileStack.back();
                continue;
            }

            else if (std::regex_match(directiveString, matches, macroRegex))
            {
                macro = &definitions[matches[1]];

                // Macro with argument path
                if (matches[2].matched)
                {
                    std::string argList = matches[2].str();
                    char* arg = argList.data();
                    while (arg != nullptr)
                    {
                        char* nextArg = strchr(arg, ',');
                        if (nextArg != nullptr)
                        {
                            macro->args.push_back(std::string(arg, nextArg - arg));
                            arg = nextArg + 1;
                        }
                        else
                        {
                            macro->args.push_back(arg);
                            arg = nullptr;
                        }
                    }
                    macro->contents = matches[3];
                }
                else
                {
                    // Simple macro to value path
                    macro->contents = matches[3];
                }
            }
            else if (std::regex_match(directiveString, matches, undefineRegex))
            {
                if (definitions.contains(matches[1]))
                    definitions.erase(matches[1]);
            }
            else if (std::regex_match(directiveString, matches, ifdefRegex))
            {
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
            else if (std::regex_match(directiveString, matches, elifdefRegex))
            {
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
            else if (std::regex_match(directiveString, matches, ifndefRegex))
            {
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
            else if (std::regex_match(directiveString, matches, elifndefRegex))
            {
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
            else if (std::regex_match(directiveString, matches, ifRegex))
            {
                auto it = definitions.find(matches[1]);
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
            else if (std::regex_match(directiveString, matches, elifRegex))
            {
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
            else if (std::regex_match(directiveString, matches, elseRegex))
            {
                ifStack.back() = !ifStack.back();
            }
            else if (std::regex_match(directiveString, matches, endifRegex))
            {
                ifStack.pop_back();
            }
            else
            {
                err.append(Format("Invalid preprocessor directive '%s'\n", directiveString.c_str()));
            }

            // Remove line
            goto next_line;
        }
        else if (level->line[0] == '/') // Match comments
        {
            if (level->line[1] == '/') // Single line comment
            {
                output.push_back('\n');
                goto next_line;
            }
            else if (level->line[1] == '*') // Multi line comment
                comment = true;
        }
        else if (level->line[0] == '*' && level->line[1] == '/') // End of multi line comment
        {
            comment = false;
            output.push_back('\n');
            goto next_line;
        }
        if (comment)
        {
            output.push_back('\n');
            goto next_line;
        }

        if (!ifStack.empty())
        {
            if (!ifStack.back())
            {
                output.push_back('\n');
                goto next_line;
            }
        }
        output.append(lineStr);

next_line:

        level->line = nextLine;
        if (level->line == nullptr)
        {
            fileStack.pop_back();
            if (!fileStack.empty())
            {
                level = &fileStack.back();
            }
        }
        level->lineCounter++;

    }
    
    std::vector<std::string> args =
    {

        "",
        escaped
        , "-W 0"
        , "-a"
    };
    for (const auto def : defines)
        args.push_back(FixBackslashes(def));

    const char** argArr = new const char*[args.size()];
    uint32_t counter = 0;
    for (const auto& arg : args)
    {
        argArr[counter++] = arg.c_str();
    }

    // run preprocessing
    mcpp_use_mem_buffers(1);
    int result = mcpp_lib_main(args.size(), (char**)argArr);
    if (result != 0)
    {
        char* error = mcpp_get_mem_buffer(ERR);
        err = error;
        delete[] argArr;
        return false;
    }
    else
    {
        char* preprocessed = mcpp_get_mem_buffer(OUT);
        output.append(preprocessed);

        // cleanup mcpp
        mcpp_use_mem_buffers(1);
        delete[] argArr;
        return true;
    }
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
    std::string pperr;
    if (GPULangPreprocess(file, defines, preprocessed, pperr))
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
    errorBuffer = Error(pperr);
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
    std::string pperr;
    if (GPULangPreprocess(file, defines, preprocessed, pperr))
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

        std::ifstream stream;
        stream.open(file);
        ANTLRInputStream input(stream);
        GPULangLexer lexer(&input);
        lexer.setTokenFactory(GPULangTokenFactory::DEFAULT);
        CommonTokenStream tokens(&lexer);
        GPULangParser parser(&tokens);
        parser.getInterpreter<antlr4::atn::ParserATNSimulator>()->setPredictionMode(antlr4::atn::PredictionMode::SLL);
        GPULangParser::LineStack.clear();

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
    std::stringstream ss(pperr);
    std::string line;
    while (std::getline(ss, line, '\n'))
    {
        std::string remainder = line;
        size_t messageDelimiter = line.find_last_of(":");
        if (messageDelimiter == std::string::npos)
            continue;
        std::string message = remainder.substr(messageDelimiter+1);
        remainder = remainder.substr(0, messageDelimiter);

        size_t errorDelimited = remainder.find_last_of(":");
        remainder = remainder.substr(0, errorDelimited);

        size_t lineDelimiter = remainder.find_last_of(":");
        int lineNo = atoi(remainder.substr(lineDelimiter + 1).c_str());

        remainder = remainder.substr(0, lineDelimiter);
        result.diagnostics.push_back(GPULang::Diagnostic{ .error = message, .file = remainder, .line = lineNo, .column = 0 });
    }
    result.messages.push_back(pperr);
    return false;
}
