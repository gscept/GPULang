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

#ifdef __WIN32__
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <unistd.h>
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
GPULangLoadFile(const std::string_view& path, const std::vector<std::string_view>& searchPaths)
{
    GPULangFile* file = nullptr;
    FILE* f = fopen(TStr(path).Data(), "rb");
    
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
    , PinnedArray<GPULang::Symbol*>& preprocessorSymbols
    , PinnedArray<GPULang::Diagnostic>& diagnostics
)
{

#define DIAGNOSTIC(message)\
        GPULang::Diagnostic{.error = message, .file = level->file->path, .line = level->lineCounter }

    bool ret = true;
    struct Macro
    {
        FixedArray<std::string_view> args;
        std::string_view contents;
    };
    PinnedMap<std::string_view, Macro> definitions(0xFFFF);

    struct IfLevel
    {
        bool consumed : 1 = false;
        bool active : 1 = false;
    };

    std::vector<IfLevel> ifStack;
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
                definitions.Insert(argView.substr(2), Macro{ .contents = "" });
        }
    }

    // Insert into file map
    PinnedMap<std::string_view, GPULangFile*> fileMap = 0xFFFF;

    FileLevel* level = &fileStack.back();
    if (file->contents == nullptr)
    {
        diagnostics.Append(DIAGNOSTIC(Format("Can't open '%s' for reading", file->path.c_str())));
        return false;
    }
    output.clear();
    output.reserve(file->contentSize * 1.5f);
    output.append(Format("#line 0 \"%s\"\n", file->path.c_str()));
    Macro* macro = nullptr;
    bool comment = false;
    level->lineCounter = 0;

    static auto match_mask = [](const char* begin, const char* end, char c) -> uint64_t
    {
        int len = min(end - begin, 8);
        if (len < 0)
            return 255;
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
        uint64_t word = *(uint64_t*)begin;
        uint64_t strMask = (word & mask) ^ byte_mask;
        return (strMask - 0x0101010101010101ULL) & ~strMask & 0x8080808080808080ULL;
    };

    static auto charPos = [](const char* begin, const char* end, char c) -> uint8_t
    {
        uint64_t mask = match_mask(begin, end, c);
        if (mask == 0x0) return 255;
        else return CountLeadingZeroes(mask) / 8;
    };
    
    static auto notCharPos = [](const char* begin, const char* end, char c) -> uint8_t
    {
        uint64_t mask = match_mask(begin, end, c);
        mask = ~mask & 0x8080808080808080ULL;
        if (mask == 0x0) return 255;
        else return CountLeadingZeroes(mask) / 8;
    };
    
    // Find any char in the given string
    static auto anyCharPos = [](const char* begin, const char* end, const std::initializer_list<char>& list) -> uint8_t
    {
        uint64_t resultMask = 0x0;
        for (auto c : list)
        {
            resultMask |= match_mask(begin, end, c);
        }
        if (resultMask == 0x0) return 255;
        else return CountLeadingZeroes(resultMask) / 8;
    };
    
    // Find position where none of the characters in list are present
    static auto noCharPos = [](const char* begin, const char* end, const std::initializer_list<char>& list) -> uint8_t
    {
        uint64_t resultMask = 0x0;
        for (auto c : list)
        {
            resultMask |= match_mask(begin, end, c);;
        }
        resultMask = ~resultMask & 0x8080808080808080ULL;
        if (resultMask == 0x0) return 255;
        else return CountLeadingZeroes(resultMask) / 8;
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
    
#define WHITESPACE_CHARS {' ', '\n', '\r', '\t', '\f', '\v'}

    static auto wordStart = [](const char* begin, const char* end) -> const char*
    {
        const char* start = begin;
        while (start < end)
        {
            uint8_t nonWsOffset = noCharPos(start, end, WHITESPACE_CHARS);
            if (nonWsOffset != 255)
                return start + nonWsOffset;
            else
                start += 8;
        }
        return end;
    };

    static auto wordEnd = [](const char* begin, const char* end) -> const char*
    {
        const char* start = begin;
        while (start != end)
        {
            uint8_t asterixOffset = charPos(start, end, '*');
            uint8_t slashOffset = charPos(start, end, '/');
            uint8_t wsOffset = anyCharPos(start, end, WHITESPACE_CHARS);
            if (asterixOffset != 255)
            {
                if (asterixOffset < slashOffset && asterixOffset < wsOffset)
                {
                    if (start + asterixOffset + 1 < end && start[asterixOffset + 1] == '/')
                        return start + asterixOffset;
                }
                asterixOffset = 255;
            }
            
            if (slashOffset != 255)
            {
                if (slashOffset < wsOffset)
                {
                    if (start + slashOffset + 1 < end && start[slashOffset + 1] == '*')
                        return start + slashOffset;
                    else if (start + slashOffset + 1 < end && start[slashOffset + 1] == '/')
                        return start + slashOffset;
                }
            }
            if (wsOffset != 255)
                return start + wsOffset;
            
            start += 8;
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

    static auto lineEnd = [](const char* begin, const char* end) -> const char*
    {
        const char* start = begin;
        while (start < end)
        {
            uint8_t crPos = charPos(start, end, '\r');
            uint8_t nlPos = charPos(start, end, '\n');
            uint8_t nPos = charPos(start, end, '\0');
            if (crPos != 255)
            {
                const char* offset = start + crPos;
                if ((offset != (end - 1)) && offset[1] == '\n')
                    return offset + 2;
                return offset + 1;
            }
            else if (nlPos != 255)
            {
                return start + nlPos + 1;
            }
            else if (nPos != 255)
            {
                return start + nPos;
            }
            start += 8;
        }
        return end;
    };

    
    static auto identifierBegin = [](const char* begin, const char* end) -> const char*
    {
        const char* start = begin;
        while (start != end)
        {
            if (start[0] == '/')
            {
                if (start + 1 < end && start[1] == '*')
                    return start;
                if (start + 1 < end && start[1] == '/')
                    return start;
            }
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

    static auto argListStart = [](const char* begin, const char* end) -> const char*
    {
        const char* start = begin;
        while (start < end)
        {
            uint8_t pos = charPos(start, end, '(');
            if (pos == 255)
                start += 8;
            else
                return start + pos + 1;
        }
        return end;
    };

    static auto argListEnd = [](const char* begin, const char* end) -> const char*
    {
        const char* start = begin;
        while (start < end)
        {
            uint8_t pos = charPos(start, end, ')');
            if (pos == 255)
                start += 8;
            else
                return start + pos;
        }
        return end;
    };

    static std::function<int(const char*, const char*, FileLevel*, PinnedArray<GPULang::Diagnostic>&, const PinnedMap<std::string_view, Macro>&, bool& res)> eval = [](const char* begin, const char* end, FileLevel* level, PinnedArray<GPULang::Diagnostic>& diagnostics, const PinnedMap<std::string_view, Macro>& definitions, bool& res) -> int
    {
        struct Token
        {
            int value;
            bool op;
        };
        StackArray<Token> result(256);
        StackArray<Token> opstack(256);
        
        auto digit = [&diagnostics, &level](const char* begin, const char* end, int& size, bool& ret) -> int
        {
            int val = 0;
            size = 0;
            int dec = 1;
            
            // Find first non-digit character
            if (begin[0] < '0' || begin[0] > '9')
                return -1;
            
            const char* rbegin = begin;
            while (rbegin < end)
            {
                // Find the end of the list
                uint8_t endOfDigit = noCharPos(begin, end, {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'});
                if (endOfDigit == 255)
                    rbegin += 8;
                else
                {
                    rbegin += endOfDigit-1;
                    break;
                }
            }

            // Parse the digit
            while (rbegin >= begin)
            {
                if (rbegin[0] >= '0' && rbegin[0] <= '9')
                {
                    val += (rbegin[0] - '0') * dec;
                    size++;
                    dec *= 10;
                }
                else if (dec == 1)
                    return -1;
                else
                    break;
                    
                rbegin--;
            }
            return val;
        };
        
        auto op = [](const char* begin, const char* end, int& size, bool& res) -> int
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
        
        auto presedence = [&diagnostics, &level](const int op, bool& res) -> int
        {
            switch(op)
            {
                case '(':
                    return 0;
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
                default:
                    diagnostics.Append(DIAGNOSTIC(Format("Unknown operator '%c%c%c%c'", op & 0xFF, (op >> 8) & 0xFF, (op >> 16) & 0xFF, (op >> 24) & 0xFF)));
                    res = false;
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
        int parantCounter = 0;
        while (word != end)
        {
            word = wordStart(word, end);
            Token t;
            int stride = 0;
            if (word == end)
                break;
            if (word[0] == '(')
            {
                t.op = true;
                t.value = '(';
                opstack.Append(t);
                parantCounter++;
                word++;
                continue;
            }
            else if (word[0] == ')')
            {
                if (parantCounter == 0)
                {
                    diagnostics.Append(DIAGNOSTIC("Invalid closing paranthesis ')' in expression, missing opening '('"));
                    res = false;
                    return -1;
                }
                
                // Flush operators to results
                for (int i = opstack.size-1; i >= 0; i--)
                {
                    if (opstack[i].value == '(')
                    {
                        opstack.size--; // make sure to consume the opening paranthesis in this case
                        break;
                    }
                    result.Append(opstack.ptr[i]);
                    opstack.size--;
                }
                parantCounter--;
                word++;
                continue;
            }
            int val = digit(word, end, stride, res);
            if (val == -1)
            {
                val = op(word, end, stride, res);
                if (val == -1)
                {
                    const char* identStart = identifierBegin(word, end);
                    const char* identEnd = identifierEnd(identStart, end);
                    std::string_view ident(identStart, identEnd);
                    if (ident.length() > 0)
                    {
                        auto defIt = definitions.Find(ident);
                        if (defIt == definitions.end())
                        {
                            // This is a built-in macro to check if a value is defined or not
                            if (ident == "defined")
                            {
                                const char* argStart = argListStart(identEnd, end);
                                if (argStart != end)
                                {
                                    const char* argEnd = argListEnd(argStart, end);
                                    identEnd = argEnd+1;
                                    auto definedIt = definitions.Find(std::string_view(argStart, argEnd));
                                    if (definedIt != definitions.end())
                                        t.value = 1;
                                    else
                                        t.value = 0;
                                    t.op = false;
                                    unconsumedIntegers++;
                                    result.Append(t);
                                }
                                else
                                {
                                    diagnostics.Append(DIAGNOSTIC(Format("Malformed 'defined'")));
                                    res = false;
                                    return -1;
                                }
                            }
                            else
                            {
                                // Undefined arguments are simply resolved to 0
                                t.value = 0;
                                t.op = false;
                                unconsumedIntegers++;
                                result.Append(t);
                            }
                        }
                        else
                        {
                            // TODO: Must actually invoke the macro here, with arguments which must be parsed
                            const char* argStart = argListStart(identEnd, end);
                            if (argStart != end)
                            {
                                const char* argEnd = argListEnd(argStart, end);
                                identEnd = argEnd + 1;
                                stride += argEnd - argStart;
                            }

                            // If identifier of a macro, evaluate the contents of said macro
                            t.value = eval(defIt->second.contents.data(), defIt->second.contents.data() + defIt->second.contents.size(), level, diagnostics, definitions, res);
                            t.op = false;
                            unconsumedIntegers++;
                            result.Append(t);
                        }
                    }
                    else
                    {
                        diagnostics.Append(DIAGNOSTIC(Format("Invalid token '%c'", word[0])));
                        res = false;
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
                            diagnostics.Append(DIAGNOSTIC(Format("Operators must be surrounded by expressions '*.%s'", stride, word)));
                            res = false;
                            return -1;
                        }

                        // When two values are consumed by an operator, one gets produced as an outcome, so only reduce this value by 1
                        unconsumedIntegers -= 1;
                    }                    
                
                    // Operator, check presedence
                    if (opstack.size > 0)
                    {
                        int pres = presedence(t.value, res);
                        int prevPres = presedence(opstack.ptr[opstack.size-1].value, res);
                        if (pres < prevPres)
                        {
                            // If we are within paranthesis, only pop operators until the previous paranthesis
                            if (parantCounter > 0)
                            {
                                // Flush operators to results
                                for (int i = opstack.size-1; i >= 0; i--)
                                {
                                    if (opstack[i].value == '(')
                                    {
                                        break;
                                    }
                                    result.Append(opstack.ptr[i]);
                                    opstack.size--;
                                }
                            }
                            else
                            {
                                // Flush operators to results
                                for (int i = opstack.size-1; i >= 0; i--)
                                {
                                    result.Append(opstack.ptr[i]);
                                }
                                opstack.size = 0;
                            }
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
        
        if (parantCounter > 0)
        {
            diagnostics.Append(DIAGNOSTIC("Missing closing paranthesis ')'"));
            res = false;
            return -1;
        }
        else if (parantCounter < 0)
        {
            diagnostics.Append(DIAGNOSTIC("Too closing paranthesis ')' mismatching opening paranthesis '('"));
            res = false;
            return -1;
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
        
#define CHECK_IF()\
        if (!ifStack.empty())\
        {\
            const IfLevel& ilevel = ifStack.back();\
            if (!ilevel.active)\
            {\
                auto pp = Alloc<Preprocessor>();\
                pp->type = Preprocessor::Comment;\
                pp->location.file = level->file->path;\
                pp->location.line = level->lineCounter;\
                pp->location.start = 0;\
                pp->location.end = lineSpan.length();\
                preprocessorSymbols.Append(pp);\
                output.push_back('\n');\
                goto next_line;\
            }\
        }

#define SETUP_PP2(pp, begin, stop)\
        pp->location.file = level->file->path;\
        pp->location.line = level->lineCounter;\
        pp->location.start = begin - lineBegin;\
        pp->location.end = stop - lineBegin;\
        preprocessorSymbols.Append(pp);

#define SETUP_ARG2(pp, arg, begin, stop)\
        args.Append(arg);\
        Symbol::Location argLoc;\
        argLoc.file = level->file->path;\
        argLoc.line = level->lineCounter;\
        argLoc.start = begin - lineBegin;\
        argLoc.end = stop - lineBegin;\
        argLocs.Append(argLoc);

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
                    CHECK_IF()

                    const char* startOfPath = wordStart(endOfDirective, eol);
                    const char* endOfPath = wordEnd(startOfPath, eol);
                    auto pp = Alloc<Preprocessor>();
                    pp->type = Preprocessor::Include;
                    SETUP_PP2(pp, firstWord-1, endOfDirective);

                    StackArray<Symbol::Location> argLocs(1);
                    StackArray<std::string_view> args(1);

                    if ((startOfPath[0] == '"' && endOfPath[-1] == '"')
                        || (startOfPath[0] == '<' && endOfPath[-1] == '>'))
                    {
                        const std::string_view path = std::string_view(startOfPath + 1, endOfPath - 1);
                        SETUP_ARG2(pp, path, startOfPath, endOfPath);

                        GPULangFile* inc = nullptr;
                        auto fileIt = fileMap.Find(path);
                        if (fileIt == fileMap.end())
                        {
                            inc = GPULangLoadFile(path, searchPaths);
                            fileMap.Insert(path, inc);
                        }
                        else
                        {
                            inc = fileIt->second;
                        }
                        pp->contents = inc->path;
                        if (inc == nullptr)
                        {
                            diagnostics.Append(DIAGNOSTIC(Format("File not found '%s'", inc->path.c_str())));
                            ret = false;
                            goto end;
                        }
                        output.append(Format("#line %d \"%s\"\n", level->lineCounter, level->file->path.c_str()));

                        // Include guarded
                        if (!inc->consumed)
                        {
                            fileStack.push_back({ inc });
                            level = &fileStack.back();
                        }
                    }
                    else
                    {
                        diagnostics.Append(DIAGNOSTIC("include directory must be provided a path"));
                        ret = false;
                        goto end;
                    }
                    pp->args = args;
                    pp->argLocations = argLocs;
                }
                else if (strncmp(startOfDirective, "define", 6) == 0)
                {
                    CHECK_IF()
                    const char* startOfDefinition = wordStart(endOfDirective, eol);
                    if (startOfDefinition == eol)
                    {
                        diagnostics.Append(DIAGNOSTIC("define missing identifier"));
                        ret = false;
                        goto end;
                    }
                    const char* endOfDefinition = wordEndOrParanthesis(startOfDefinition, eol);
                    if (!validateIdentifier(startOfDefinition, endOfDefinition))
                    {
                        diagnostics.Append(DIAGNOSTIC("define identifier invalid"));
                        ret = false;
                        goto end;
                    }

                    std::string_view def = std::string_view(startOfDefinition, endOfDefinition);
                    auto prev = definitions.Find(def);
                    if (prev != definitions.end())
                    {
                        diagnostics.Append(DIAGNOSTIC(Format("macro %s redefinition", std::string(def).c_str())));
                        ret = false;
                        goto end;
                    }

                    macro = &definitions.Emplace(def)->second;

                    StackArray<Symbol::Location> argLocs(1);
                    StackArray<std::string_view> args(1);

                    auto pp = Alloc<Preprocessor>();
                    pp->type = Preprocessor::Macro;
                    SETUP_PP2(pp, firstWord-1, endOfDirective)
                    SETUP_ARG2(pp, def, startOfDefinition, endOfDefinition);

                    const char* startOfContents = wordStart(endOfDefinition, eol);
                    
                    StackArray<std::string_view> macroArgs(32);
                    if (endOfDefinition[0] == '(')
                    {
                        // Argument list, start parsing arguments
                        const char* argumentIt = wordStart(endOfDefinition+1, eol);
                        while (argumentIt != eol)
                        {
                            if (argumentIt[0] == ')') // end of list
                            {
                                // Move new start to after argument list
                                argLocs[0].end = argumentIt + 1 - lineBegin;
                                startOfContents = wordStart(argumentIt+1, eol);
                                break;
                            }
                            const char* argumentEnd = identifierEnd(argumentIt, eol);

                            if (!validateIdentifier(argumentIt, argumentEnd))
                            {
                                diagnostics.Append(DIAGNOSTIC("Invalid argument identifier"));
                                ret = false;
                                goto end;
                            }

                            // Identifier validation done, save argument
                            macroArgs.Append(std::string_view(argumentIt, argumentEnd));
                            argumentIt = nextArg(argumentIt, eol);
                        }

                        // append the rest of the contents
                        macro->args = macroArgs;
                        macro->contents = std::string_view(startOfContents, eol - lineEndingLength);
                    }
                    else if (startOfContents != eol)
                    {
                        macro->contents = std::string_view(startOfContents, eol - lineEndingLength);
                    }               
                    else
                    {

                    }
                    pp->args = args;
                    pp->argLocations = argLocs;
                    output.append(Format("#line %d \"%s\"", level->lineCounter, level->file->path.c_str()));
                }
                else if (strncmp(startOfDirective, "undef", 5) == 0)
                {
                    CHECK_IF()

                    const char* startOfDefinition = wordStart(endOfDirective, eol);
                    if (startOfDefinition == nullptr)
                    {
                        diagnostics.Append(DIAGNOSTIC("undef missing identifier"));
                        ret = false;
                        goto end;
                    }
                    const char* endOfDefinition = wordEnd(startOfDefinition, eol);

                    if (!validateIdentifier(startOfDefinition, endOfDefinition))
                    {
                        diagnostics.Append(DIAGNOSTIC("Invalid undef identifier"));
                        ret = false;
                        goto end;
                    }

                    std::string_view definition = std::string_view(startOfDefinition, endOfDefinition);

                    StackArray<Symbol::Location> argLocs(1);
                    StackArray<std::string_view> args(1);

                    auto pp = Alloc<Preprocessor>();
                    pp->type = Preprocessor::Undefine;
                    SETUP_PP2(pp, firstWord - 1, endOfDirective);
                    SETUP_ARG2(pp, definition, startOfDefinition, endOfDefinition);

                    auto it = definitions.Find(definition);
                    if (it != definitions.end())
                    {
                        definitions.Erase(definition);
                    }

                    pp->args = args;
                    pp->argLocations = argLocs;
                }
                else if (strncmp(startOfDirective, "ifdef", 5) == 0)
                {
                    const char* startOfDefinition = wordStart(endOfDirective, eol);
                    if (startOfDefinition == nullptr)
                    {
                        diagnostics.Append(DIAGNOSTIC("ifdef missing identifier"));
                        ret = false;
                        goto end;
                    }
                    const char* endOfDefinition = wordEnd(startOfDefinition, eol);

                    if (!validateIdentifier(startOfDefinition, endOfDefinition))
                    {
                        diagnostics.Append(DIAGNOSTIC("Invalid ifdef identifier"));
                        ret = false;
                        goto end;
                    }

                    std::string_view definition = std::string_view(startOfDefinition, endOfDefinition);

                    StackArray<Symbol::Location> argLocs(1);
                    StackArray<std::string_view> args(1);

                    auto pp = Alloc<Preprocessor>();
                    pp->type = Preprocessor::If;
                    SETUP_PP2(pp, firstWord - 1, endOfDirective);
                    SETUP_ARG2(pp, definition, startOfDefinition, endOfDefinition);
                    
                    bool active = true;
                    if (!ifStack.empty())
                        active = ifStack.back().active;
                    if (active)
                    {
                        auto it = definitions.Find(definition);
                        IfLevel ilevel{ .consumed = it != definitions.end(), .active = it != definitions.end() };
                        ifStack.push_back(ilevel);
                    }
                    else
                    {
                        IfLevel ilevel{.consumed = true, .active = false};
                        ifStack.push_back(ilevel);
                    }

                    pp->args = args;
                    pp->argLocations = argLocs;
                }
                else if (strncmp(startOfDirective, "ifndef", 6) == 0)
                {
                    const char* startOfDefinition = wordStart(endOfDirective, eol);
                    if (startOfDefinition == nullptr)
                    {
                        diagnostics.Append(DIAGNOSTIC("ifdef missing identifier"));
                        ret = false;
                        goto end;
                    }
                    const char* endOfDefinition = wordEnd(startOfDefinition, eol);

                    if (!validateIdentifier(startOfDefinition, endOfDefinition))
                    {
                        diagnostics.Append(DIAGNOSTIC("Invalid ifndef identifier"));
                        ret = false;
                        goto end;
                    }

                    std::string_view definition = std::string_view(startOfDefinition, endOfDefinition);

                    StackArray<Symbol::Location> argLocs(1);
                    StackArray<std::string_view> args(1);

                    auto pp = Alloc<Preprocessor>();
                    pp->type = Preprocessor::If;
                    SETUP_PP2(pp, firstWord - 1, endOfDirective);
                    SETUP_ARG2(pp, definition, startOfDefinition, endOfDefinition);

                    bool active = true;
                    if (!ifStack.empty())
                        active = ifStack.back().active;
                    
                    if (active)
                    {
                        auto it = definitions.Find(definition);
                        IfLevel ilevel{ .consumed = it == definitions.end(), .active = it == definitions.end() };
                        ifStack.push_back(ilevel);
                    }
                    else
                    {
                        IfLevel ilevel{.consumed = true, .active = false};
                        ifStack.push_back(ilevel);
                    }
                    
                    pp->args = args;
                    pp->argLocations = argLocs;
                }
                else if (strncmp(startOfDirective, "elif", 4) == 0)
                {
                    if (ifStack.empty())
                    {
                        diagnostics.Append(DIAGNOSTIC("elif missing if/ifdef/ifndef"));
                        ret = false;
                        goto end;
                    }
                    
                    StackArray<Symbol::Location> argLocs(1);
                    StackArray<std::string> args(1);

                    auto pp = Alloc<Preprocessor>();
                    pp->type = Preprocessor::Else;
                    SETUP_PP2(pp, firstWord - 1, endOfDirective);

                    IfLevel& prevLevel = ifStack.back();
                    if (!prevLevel.consumed)
                    {
                        int val = eval(endOfDirective, eol, level, diagnostics, definitions, ret);
                        SETUP_ARG2(pp, std::string(val == 0 ? "false" : "true"), endOfDirective, eol);

                        pp->args = args;
                        pp->argLocations = argLocs;
                        prevLevel.active = val != 0;
                        prevLevel.consumed = prevLevel.active;
                    }
                    else
                    {
                        SETUP_ARG2(pp, "false", endOfDirective, eol);
                        prevLevel.active = false;

                        pp->args = args;
                        pp->argLocations = argLocs;
                    }
                }
                else if (strncmp(startOfDirective, "if", 2) == 0)
                {
                    
                    StackArray<Symbol::Location> argLocs(1);
                    StackArray<std::string> args(1);

                    auto pp = Alloc<Preprocessor>();
                    pp->type = Preprocessor::If;
                    SETUP_PP2(pp, firstWord - 1, endOfDirective);

                    bool active = true;
                    if (!ifStack.empty())
                        active = ifStack.back().active;
                    if (active)
                    {
                        int val = eval(endOfDirective, eol, level, diagnostics, definitions, ret);
                        SETUP_ARG2(pp, std::string(val == 0 ? "false" : "true"), endOfDirective, eol);
                        IfLevel ilevel{ .consumed = val != 0, .active = val != 0};
                        ifStack.push_back(ilevel);
                    }
                    else
                    {
                        IfLevel ilevel{.consumed = true, .active = false};
                        ifStack.push_back(ilevel);
                    }

                    pp->args = args;
                    pp->argLocations = argLocs;
                }
                else if (strncmp(startOfDirective, "else", 4) == 0)
                {
                    if (ifStack.empty())
                    {
                        diagnostics.Append(DIAGNOSTIC("elif missing if/ifdef/ifndef"));
                        ret = false;
                        goto end;
                    }
                    
                    auto pp = Alloc<Preprocessor>();
                    pp->type = Preprocessor::Else;
                    SETUP_PP2(pp, firstWord-1, endOfDirective)
                    
                    IfLevel& prevLevel = ifStack.back();
                    if (!prevLevel.consumed)
                    {
                        prevLevel.consumed = true;
                        prevLevel.active = true;
                    }
                    else
                    {
                        prevLevel.active = false;
                    }
                }
                else if (strncmp(startOfDirective, "endif", 5) == 0)
                {
                    auto pp = Alloc<Preprocessor>();
                    pp->type = Preprocessor::EndIf;
                    SETUP_PP2(pp, firstWord-1, endOfDirective)
                    if (ifStack.empty())
                    {
                        diagnostics.Append(GPULang::Diagnostic{ .error = "Invalid #endif, missing matching #if/#ifdef/#ifndef", .file = level->file->path, .line = level->lineCounter });
                        ret = false;
                        goto end;
                    }
                    else
                        ifStack.pop_back();
                }
                else if (strncmp(startOfDirective, "pragma", 6) == 0)
                {
                    auto pp = Alloc<Preprocessor>();
                    pp->type = Preprocessor::Pragma;
                    SETUP_PP2(pp, firstWord-1, endOfDirective);
                    
                    const char* startOfCommand = wordStart(endOfDirective, eol);
                    const char* endOfCommand = wordEnd(startOfCommand, eol);
                    std::string_view command(startOfCommand, endOfCommand);
                    pp->contents = command;
                    
                    if (command == "once")
                    {
                        level->file->consumed = true;
                    }
                    else
                    {
                        diagnostics.Append(GPULang::Diagnostic{ .error = Format("Unknown #pragma '%s'", command.data()), .file = level->file->path, .line = level->lineCounter });
                        ret = false;
                        goto end;
                    }
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
                CHECK_IF()
                const char* startOfWord = identifierBegin(columnIt, eol);
                const char* endOfWord = identifierEnd(startOfWord, eol);

                // Add whatever comes before the first identifier
                output.append(columnIt, startOfWord);
                if (startOfWord != eol)
                {
                    std::string_view word = std::string_view(startOfWord, endOfWord);
                    auto it = definitions.Find(word);
                    if (it != definitions.end())
                    {
                        auto pp = Alloc<Preprocessor>();
                        pp->type = Preprocessor::Call;
                        SETUP_PP2(pp, startOfWord, endOfWord)
                        
                        static std::function<const char*(const char*, const char*, const Macro*, GrowingString&, const PinnedMap<std::string_view, Macro>&, const FileLevel*, PinnedArray<GPULang::Diagnostic>&)> expandMacro = [](const char* beginOfCall, const char* eol, const Macro* macro, GrowingString& expanded, const PinnedMap<std::string_view, Macro>& definitions, const FileLevel* level, PinnedArray<GPULang::Diagnostic>& diagnostics) -> const char*
                        {
                            StackMap<std::string_view, std::string_view> argumentMap(32);
                            StackArray<std::string_view> args(32);
                            if (beginOfCall[0] == '(')
                            {
                                int argStack = 1;
                                const char* argListIt = beginOfCall+1;
                                const char* argBegin = argListIt;
                                
                                // parse arguments
                                while (argListIt != eol)
                                {
                                    if (argListIt[0] == '(')
                                        argStack++;
                                    else if (argListIt[0] == ')')
                                    {
                                        argStack--;
                                        if (argStack == 0)
                                        {
                                            if (args.Full())
                                            {
                                                diagnostics.Append(DIAGNOSTIC("Argument limit of 32 for macro arguments reached"));
                                                return nullptr;
                                            }
                                            args.Append(std::string_view(argBegin, argListIt));
                                            
                                            argListIt += 1; // parsing done
                                            break;
                                        }
                                    }
                                    else if (argListIt[0] == ',')
                                    {
                                        if (argStack == 1)
                                        {
                                            args.Append(std::string_view(argBegin, argListIt));
                                            argBegin = argListIt+1;
                                        }
                                    }
                                    argListIt++;
                                }
                                
                                if (argStack > 0)
                                {
                                    diagnostics.Append(DIAGNOSTIC("Macro call missing ')'"));
                                    return nullptr;
                                }
                                
                                beginOfCall = argListIt;
                            }
                                
                            // Warn if mismatch
                            size_t maxIterations = min(args.size, macro->args.size);
                            int argCounter = 0;
                            for (auto& arg : macro->args)
                            {
                                argumentMap.Insert(arg, args[argCounter++]);
                            }
                            
                            const char* macroContentsIt = macro->contents.data();
                            const char* macroContentsEnd = macro->contents.data() + macro->contents.size();
                            
                            // Replace macro contents with definitions and arguments
                            while (macroContentsIt != macroContentsEnd)
                            {
                                // Look for identifiers which may be the start of another macro execution
                                const char* startOfMacroWord = identifierBegin(macroContentsIt, macroContentsEnd);
                                const char* endOfMacroWord = identifierEnd(startOfMacroWord, macroContentsEnd);

                                // Add whatever comes before the identifier to the output
                                expanded.Append(macroContentsIt, startOfMacroWord);
                                if (macroContentsEnd == startOfMacroWord)
                                    break;

                                if (startOfMacroWord != macroContentsEnd)
                                {
                                    // Add everything leading up to the identifier to the output
                                    std::string_view word(startOfMacroWord, endOfMacroWord);
                                    macroContentsIt = endOfMacroWord;

                                    // Small optimization, don't bother with arguments if it's empty
                                    if (argumentMap.Empty())
                                    {
                                        auto defIt = definitions.Find(word);
                                        if (defIt == definitions.end())
                                        {
                                            expanded.Append(word);
                                        }
                                        else
                                        {
                                            macroContentsIt = expandMacro(macroContentsIt, macroContentsEnd, &defIt->second, expanded, definitions, level, diagnostics);
                                            if (macroContentsIt == nullptr)
                                                return nullptr;
                                        }
                                    }
                                    else
                                    {
                                        auto argIt = argumentMap.Find(word);
                                        if (argIt == argumentMap.end())
                                        {
                                            auto defIt = definitions.Find(word);
                                            if (defIt == definitions.end())
                                            {
                                                expanded.Append(word);
                                            }
                                            else
                                            {
                                                macroContentsIt = expandMacro(macroContentsIt, macroContentsEnd, &defIt->second, expanded, definitions, level, diagnostics);
                                                if (macroContentsIt == nullptr)
                                                    return nullptr;
                                            }
                                        }
                                        else
                                        {
                                            expanded.Append(argIt->second);
                                        }
                                    }
                                }
                                else
                                {
                                    expanded.Append(startOfMacroWord, endOfMacroWord);
                                    macroContentsIt = endOfMacroWord;
                                }
                            }
                            return beginOfCall;
                        };

                        GrowingString contents;
                        const char* ret = expandMacro(endOfWord, eol, &it->second, contents, definitions, level, diagnostics);
                        if (ret == nullptr)
                            return false;
                        endOfWord = ret;

                        pp->location.end = max(contents.size, pp->location.end);
                        pp->contents = contents;

                        // Add summed contents to output
                        output.append(pp->contents.c_str());
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
    , PinnedArray<GPULang::Symbol*>& preprocessorSymbols
    , PinnedArray<GPULang::Diagnostic>& diagnostics
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
    SetupSystem();
    bool ret = true;
    Allocator alloc = GPULang::CreateAllocator();
    GPULang::InitAllocator(&alloc);
    GPULang::MakeAllocatorCurrent(&alloc);

    std::string preprocessed;
    errorBuffer = nullptr;

    GPULang::Compiler::Timer timer;

    timer.Start();
    PinnedArray<GPULang::Symbol*> preprocessorSymbols(0xFFFFFF);
    PinnedArray<GPULang::Diagnostic> diagnostics(0xFFFFFF);
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
    SetupSystem();
    bool ret = true;
    std::string preprocessed;

    GPULang::Compiler::Timer timer;
    timer.Start();

    PinnedArray<GPULang::Symbol*> preprocessorSymbols(0xFFFFFF);
    PinnedArray<GPULang::Diagnostic> diagnostics(0xFFFFFF);
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

        result.diagnostics.Clear();

        // if we have any lexer or parser error, return early
        if (lexerErrorHandler.hasError || parserErrorHandler.hasError)
        {
            std::string errorMessage;
            errorMessage.append(lexerErrorHandler.errorBuffer);
            errorMessage.append(parserErrorHandler.errorBuffer);
            result.diagnostics.Append(lexerErrorHandler.diagnostics);
            result.diagnostics.Append(parserErrorHandler.diagnostics);
        }

        Compiler compiler;
        compiler.path = file;
        compiler.filename = effectName;
        compiler.Setup(options);

        bool res = compiler.Validate(effect);
        effect->~Effect();

        result.root = effect;
        result.symbols = compiler.symbols;
        result.symbols.Prepend(preprocessorSymbols);
        result.intrinsicScope = compiler.intrinsicScope;
        result.mainScope = compiler.mainScope;
        if (compiler.diagnostics.size > 0)
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
    result.diagnostics.Append(diagnostics);
    return false;
}

//------------------------------------------------------------------------------
/**
    Runs compilation without output
*/
bool
GPULangValidate(GPULangFile* file, const std::vector<std::string>& defines, GPULang::Compiler::Options options, GPULangServerResult& result)
{
    SetupSystem();
    bool ret = true;

    std::string preprocessed;

    GPULang::Compiler::Timer timer;
    timer.Start();

    PinnedArray<GPULang::Symbol*> preprocessorSymbols(0xFFFFFF);
    PinnedArray<GPULang::Diagnostic> diagnostics(0xFFFFFF);
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

        result.diagnostics.Clear();

        // if we have any lexer or parser error, return early
        if (lexerErrorHandler.hasError || parserErrorHandler.hasError)
        {
            std::string errorMessage;
            errorMessage.append(lexerErrorHandler.errorBuffer);
            errorMessage.append(parserErrorHandler.errorBuffer);
            result.diagnostics.Append(lexerErrorHandler.diagnostics);
            result.diagnostics.Append(parserErrorHandler.diagnostics);
        }

        Compiler compiler;
        compiler.path = file->path;
        compiler.filename = effectName;
        compiler.Setup(options);

        bool res = compiler.Validate(effect);
        effect->~Effect();

        result.root = effect;
        result.symbols = compiler.symbols;
        result.symbols.Prepend(preprocessorSymbols);
        result.intrinsicScope = compiler.intrinsicScope;
        result.mainScope = compiler.mainScope;
        if (compiler.diagnostics.size > 0)
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
    result.diagnostics.Append(diagnostics);
    return false;
}
