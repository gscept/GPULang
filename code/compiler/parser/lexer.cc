//------------------------------------------------------------------------------
//  lexer.cc
//  (C) 2025 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "lexer.h"
#include "util.h"
#include <charconv>  // for std::from_chars

#include "ast/effect.h"
#include "ast/annotation.h"
#include "ast/attribute.h"
#include "ast/structure.h"
#include "ast/variable.h"
#include "ast/function.h"
#include "ast/expressions/stringexpression.h"
#include "ast/expressions/intexpression.h"
#include "ast/expressions/uintexpression.h"
#include "ast/expressions/floatexpression.h"
#include "ast/expressions/boolexpression.h"
#include "ast/expressions/unaryexpression.h"
#include "ast/expressions/ternaryexpression.h"
#include "ast/expressions/binaryexpression.h"
#include "ast/expressions/callexpression.h"
#include "ast/expressions/accessexpression.h"
#include "ast/expressions/arrayindexexpression.h"
#include "ast/expressions/symbolexpression.h"
#include "ast/expressions/declaredexpression.h"

namespace GPULang
{
StaticMap HardCodedTokens = std::array{
    std::pair{ "struct"_c, TokenType::Struct }
    , std::pair{ "enum"_c, TokenType::Enum }
    , std::pair{ "while"_c, TokenType::While }
    , std::pair{ "for"_c, TokenType::For }
    , std::pair{ "if"_c, TokenType::If }
    , std::pair{ "else"_c, TokenType::Else }
    , std::pair{ "generate"_c, TokenType::Generate }
    , std::pair{ "declared"_c, TokenType::Declared }
    , std::pair{ "true"_c, TokenType::Bool }
    , std::pair{ "false"_c, TokenType::Bool }
    , std::pair{ "const"_c, TokenType::Constant }
    , std::pair{ "var"_c, TokenType::Var }
    , std::pair{ "uniform"_c, TokenType::Uniform }
    , std::pair{ "workgroup"_c, TokenType::Workgroup }
    , std::pair{ "in"_c, TokenType::In }
    , std::pair{ "out"_c, TokenType::Out }
    , std::pair{ "#"_c, TokenType::Directive }
    , std::pair{ "@"_c, TokenType::Annot }
    , std::pair{ ","_c, TokenType::Comma }
    , std::pair{ ":"_c, TokenType::Colon }
    , std::pair{ ";"_c, TokenType::SemiColon }
    , std::pair{ "("_c, TokenType::LeftParant }
    , std::pair{ ")"_c, TokenType::RightParant }
    , std::pair{ "{"_c, TokenType::LeftScope }
    , std::pair{ "}"_c, TokenType::RightScope }
    , std::pair{ "["_c, TokenType::LeftBracket }
    , std::pair{ "]"_c, TokenType::RightBracket }
    , std::pair{ "<"_c, TokenType::LeftAngle }
    , std::pair{ ">"_c, TokenType::RightAngle }
    , std::pair{ "\""_c, TokenType::Quote }
    , std::pair{ "."_c, TokenType::Dot }
    , std::pair{ "+"_c, TokenType::Add }
    , std::pair{ "-"_c, TokenType::Sub }
    , std::pair{ "*"_c, TokenType::Mul }
    , std::pair{ "/"_c, TokenType::Div }
    , std::pair{ "%"_c, TokenType::Mod }
    , std::pair{ "|"_c, TokenType::Or }
    , std::pair{ "&"_c, TokenType::And }
    , std::pair{ "^"_c, TokenType::Xor }
    , std::pair{ "!"_c, TokenType::Not }
    , std::pair{ "~"_c, TokenType::Complement }
    , std::pair{ "?"_c, TokenType::Question }
    , std::pair{ "<<"_c, TokenType::LeftShift }
    , std::pair{ ">>"_c, TokenType::RightShift }
    , std::pair{ "+="_c, TokenType::AddAssign }
    , std::pair{ "-="_c, TokenType::SubAssign }
    , std::pair{ "*="_c, TokenType::MulAssign }
    , std::pair{ "/="_c, TokenType::DivAssign }
    , std::pair{ "%="_c, TokenType::ModAssign }
    , std::pair{ "<<="_c, TokenType::LeftShiftAssign }
    , std::pair{ ">>="_c, TokenType::RightShiftAssign }
    , std::pair{ "|="_c, TokenType::OrAssign }
    , std::pair{ "&="_c, TokenType::AndAssign }
    , std::pair{ "^="_c, TokenType::XorAssign }
    , std::pair{ "++"_c, TokenType::Increment }
    , std::pair{ "--"_c, TokenType::Decrement }
    , std::pair{ "<"_c, TokenType::LessThan }
    , std::pair{ "<="_c, TokenType::LessThanEqual }
    , std::pair{ ">"_c, TokenType::GreaterThan }
    , std::pair{ ">="_c, TokenType::GreaterThanEqual }
    , std::pair{ "=="_c, TokenType::Equal }
    , std::pair{ "!="_c, TokenType::NotEqual }
    , std::pair{ "||"_c, TokenType::LogicalOr }
    , std::pair{ "&&"_c, TokenType::LogicalAnd }
    , std::pair{ "="_c, TokenType::Assign }
    , std::pair{ "render_state"_c, TokenType::RenderState }
    , std::pair{ "sampler_state"_c, TokenType::SamplerState }
    , std::pair{ "program"_c, TokenType::Program }
    , std::pair{ "//"_c, TokenType::CommentRow}
    , std::pair{ "/*"_c, TokenType::CommentBlockStart}
    , std::pair{ "*/"_c, TokenType::CommentBlockEnd}
};


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
    uint64_t* wordPtr = (uint64_t*)begin;
    uint64_t word = 0x0;
    memcpy(&word, wordPtr, len);
    //uint64_t word = *(uint64_t*)begin;
    uint64_t strMask = (word & mask) ^ byte_mask;
    return (strMask - 0x0101010101010101ULL) & ~strMask & 0x8080808080808080ULL;
};

static auto whitespace = [](const char c) -> bool
{
    return c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == '\f' || c == '\v';
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

static auto findCharPos = [](const char* begin, const char* end, char c) -> uint8_t
{
    uint64_t mask = match_mask(begin, end, c);
    if (mask == 0x0) return 255;
    else return CountLeadingZeroes(mask) / 8;
};

#define WHITESPACE_CHARS {' ', '\t', '\f', '\v'}

static auto skipWS = [](const char* begin, const char* end) -> const char*
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

static auto lineEnd = [](const char* begin, const char* end) -> const char*
{
    const char* start = begin;
    while (start < end)
    {
        uint8_t crPos = findCharPos(start, end, '\r');
        uint8_t nlPos = findCharPos(start, end, '\n');
        uint8_t nPos = findCharPos(start, end, '\0');
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

static auto identifierStart = [](const char c) -> bool
{
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == '_'))
        return true;
    return false;
};

static auto identifierChar = [](const char c) -> bool
{
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || (c == '_'))
        return true;
    return false;
};

static auto numberStart = [](const char c) -> bool
{
    if (c >= '0' && c <= '9')
        return true;
    return false;
};

static auto numberChar = [](const char c) -> bool
{
    if (c >= '0' && c <= '9')
        return true;
    return false;
};

//------------------------------------------------------------------------------
/**
 */
TokenizationResult
Tokenize(const std::string& text)
{
    printf("%s", text.c_str());
    
    TokenizationResult ret;
    
    const char* it = text.data();
    const char* end = text.data() + text.length();
    const char* tokenStart = nullptr;
    const char* startOfLine = nullptr;
    TokenType tokenType = TokenType::InvalidToken;
    uint32_t line = 0;
    uint32_t charPos = 0;
    
    bool ignore = false;
    
    startOfLine = it;
    while (it != end)
    {
        const char* prev = it;
        
        it = skipWS(it, end);
        
        // Comments
        if (it[0] == '/' && it[1] == '*')
        {
            ignore = true;
            continue;
        }
        else if (it[0] == '*' && it[1] == '/' && ignore)
        {
            ignore = false;
            continue;
        }
        else if (it[0] == '/' && it[1] == '/') // If single line comment, move iterator to end
        {
            it = lineEnd(it, end);
            continue;
        }
        
        
        if (it[0] == '\r')
        {
            if (it[1] == '\n')
            {
                line += 2;
                it += 2;
                startOfLine = it;
                continue;
            }
            else
            {
                line += 1;
                it += 1;
                startOfLine = it;
                continue;
            }
        }
        else if (it[0] == '\n')
        {
            line += 1;
            it += 1;
            startOfLine = it;
            continue;
        }
        else if (it[0] == '\0')
            break; // EOF
        
        // Capture alpha numeric word, can be an actual identifier or any built in multi character word
        if (identifierStart(it[0]))
        {
            const char* begin = it;
            while (it != end)
            {
                if (!identifierChar(it[0]))
                    break;
                it++;
            }
            Token newToken;
            newToken.text = std::string_view(begin, it-begin);
            TokenType type = TokenType::Identifier;
            auto it1 = HardCodedTokens.Find(newToken.text);
            if (it1 != HardCodedTokens.end())
            {
                type = it1->second;
            }
            newToken.startLine = line;
            newToken.endLine = line;
            newToken.startChar = begin - startOfLine;
            newToken.endChar = it - startOfLine;
            ret.tokens.Append(newToken);
            ret.tokenTypes.Append(type);
            continue;
        }
        else if (numberStart(it[0])) // Capture numbers
        {
            Token newToken;
            TokenType type = TokenType::Integer;
            const char* begin = it;
            while (it != end)
            {
                if (!numberChar(it[0]))
                    break;
                it++;
            }
            
            // If decimal point
            if (it[0] == '.')
            {
                if (numberStart(it[1]))
                {
                    type = TokenType::Double;
                    it++;
                    while (it != end)
                    {
                        if (!numberChar(it[0]))
                            break;
                        it++;
                    }
                }
                else
                {
                    // Bad integer, throw error
                    LexerError error;
                    error.message = "Incorrectly formatted floating point number";
                    error.line = line;
                    error.pos = it - startOfLine;
                    ret.errors.Append(error);
                }
                // Floating point (not double)
                if (it[0] == 'f' || it[0] == 'F')
                {
                    type = TokenType::Float;
                    it++;
                }
            }
            else if (it[0] == 'u' || it[0] == 'U')
            {
                type = TokenType::UnsignedInteger;
                it++;
            }
        
            newToken.text = std::string_view(begin, it);
            newToken.startLine = line;
            newToken.endLine = line;
            newToken.startChar = begin - startOfLine;
            newToken.endChar = it - startOfLine;
            ret.tokens.Append(newToken);
            ret.tokenTypes.Append(type);
            continue;
        }
        else if (it[0] == '\"') // If string
        {
            const char* begin = it;
            it++;
            while (it != end)
            {
                uint8_t eosOffset = findCharPos(it, end, '"');
                if (eosOffset != 255)
                {
                    it += eosOffset + 1;
                    break;
                }
                else
                    it += 8;
            }
            if (it == end)
            {
                // End of string never found
                LexerError error;
                error.message = "String not closed";
                error.line = line;
                error.pos = begin - startOfLine;
                ret.errors.Append(error);
            }
            
            Token newToken;
            newToken.text = std::string_view(begin+1, it-1); // remove leading and trailing quotes
            newToken.startLine = line;
            newToken.endLine = line;
            newToken.startChar = begin - startOfLine;
            newToken.endChar = it - startOfLine;
            ret.tokens.Append(newToken);
            ret.tokenTypes.Append(TokenType::Quote);
            continue;
        }
        else // Non-character words
        {
            Token newToken;
            auto it1 = HardCodedTokens.Find(it[0]);
            if (it1 == HardCodedTokens.end())
            {
                LexerError error;
                error.message = TStr("Unknown character ", it[0]);
                error.line = line;
                error.pos = it - startOfLine;
                ret.errors.Append(error);
            }
            else
            {
                // Check for multi character operators
                std::string_view threeCharOp(it, it+3);
                auto it3 = HardCodedTokens.Find(threeCharOp);
                if (it3 == HardCodedTokens.end())
                {
                    std::string_view twoCharOp(it, it+2);
                    auto it2 = HardCodedTokens.Find(twoCharOp);
                    if (it2 != HardCodedTokens.end())
                        it1 = it2;
                }
                else
                    it1 = it3;
            }
            newToken.text = std::string_view(it, it+1);
            newToken.startLine = line;
            newToken.endLine = line;
            newToken.startChar = it - startOfLine;
            newToken.endChar = it + newToken.text.length() - startOfLine;
            it += newToken.text.length();
            ret.tokens.Append(newToken);
            ret.tokenTypes.Append(it1->second);
            continue;
        }
    }
    return ret;
}

//------------------------------------------------------------------------------
/**
 */
Symbol::Location
LocationFromToken(const Token& tok)
{
    Symbol::Location ret;
    ret.line = tok.startLine;
    ret.start = tok.startChar;
    ret.end = tok.endChar;
    return ret;
}

//------------------------------------------------------------------------------
/**
 */
ParseError
UnexpectedToken(const TokenStream& stream, const char* expected)
{
    ParseError error;
    error.message = TStr("Expected ", expected, " got ", stream.Data(0).text);
    error.line = stream.Data(0).startLine;
    error.pos = stream.Data(0).startChar;
    return error;
}

//------------------------------------------------------------------------------
/**
 */
ParseError
IncorrectToken(const TokenStream& stream)
{
    ParseError error;
    error.message = TStr("Invalid token ", stream.Data(0).text);
    error.line = stream.Data(0).startLine;
    error.pos = stream.Data(0).startChar;
    return error;
}

//------------------------------------------------------------------------------
/**
 */
FixedArray<Expression*>
ParseExpressionList(TokenStream& stream, ParseResult& ret)
{
    
}

//------------------------------------------------------------------------------
/**
 */
Expression*
ParseExpression(TokenStream& stream, ParseResult& ret, int precedence = 0)
{
    Expression* res = nullptr;

    if (precedence == 0)
    {
        res = ParseExpression(stream, ret, 1);
        if (stream.Match(TokenType::Increment) || stream.Match(TokenType::Decrement))
        {
            res = Alloc<UnaryExpression>(StringToFourCC(TransientString(stream.Data(0).text)), false, res);
            stream.Consume();
        }
    }
    else if (precedence == 1)
    {
        res = ParseExpression(stream, ret, 2);
        if (stream.Match(TokenType::LeftParant))
        {
            stream.Consume();
            FixedArray<Expression*> arguments;
            arguments = ParseExpressionList(stream, ret);
            if (stream.Match(TokenType::RightParant))
            {
                stream.Consume();
                res = Alloc<CallExpression>(res, std::move(arguments));
            }
            else
            {
                ret.errors.Append(UnexpectedToken(stream, ")"));
                return nullptr;
            }
        }
    }
    else if (precedence == 2)
    {
        res = ParseExpression(stream, ret, 3);
        if (stream.Match(TokenType::Dot))
        {
            stream.Consume();
            Expression* rhs = ParseExpression(stream, ret, 0);
            res = Alloc<AccessExpression>(res, rhs, false);
        }
    }
    else if (precedence == 3)
    {
        res = ParseExpression(stream, ret, 4);
        if (stream.Match(TokenType::LeftBracket))
        {
            stream.Consume();
            Expression* index = ParseExpression(stream, ret, 0);
            if (stream.Match(TokenType::RightBracket))
            {
                stream.Consume();
                res = Alloc<ArrayIndexExpression>(res, index);
            }
            else
            {
                ret.errors.Append(UnexpectedToken(stream, "]"));
                return nullptr;
            }
        }
    }
    else if (precedence == 4)
    {
        // Right-associative
        if (stream.Match(TokenType::Sub)
            || stream.Match(TokenType::Add)
            || stream.Match(TokenType::Not)
            || stream.Match(TokenType::Complement)
            || stream.Match(TokenType::Increment)
            || stream.Match(TokenType::Decrement)
            || stream.Match(TokenType::Mul)
            || stream.Match(TokenType::And)
            )
        {
            Token tok = stream.Data();
            stream.Consume();
            
            Expression* rhs = ParseExpression(stream, ret, 0);
            res = Alloc<UnaryExpression>(StringToFourCC(TransientString(tok.text)), true, rhs);
        }
        else
            res = ParseExpression(stream, ret, 5);
            
    }
    else if (precedence == 5)
    {
        res = ParseExpression(stream, ret, 6);
        if (stream.Match(TokenType::Mul) || stream.Match(TokenType::Div) || stream.Match(TokenType::Mod))
        {
            Token tok = stream.Data();
            stream.Consume();
            Expression* rhs = ParseExpression(stream, ret, 0);
            res = Alloc<BinaryExpression>(StringToFourCC(TransientString(tok.text)), res, rhs);
        }
    }
    else if (precedence == 6)
    {
        res = ParseExpression(stream, ret, 7);
        if (stream.Match(TokenType::Add) || stream.Match(TokenType::Sub))
        {
            Token tok = stream.Data();
            stream.Consume();
            Expression* rhs = ParseExpression(stream, ret, 0);
            res = Alloc<BinaryExpression>(StringToFourCC(TransientString(tok.text)), res, rhs);
        }
    }
    else if (precedence == 7)
    {
        res = ParseExpression(stream, ret, 8);
        if (stream.Match(TokenType::LeftShift) || stream.Match(TokenType::RightShift))
        {
            Token tok = stream.Data();
            stream.Consume();
            Expression* rhs = ParseExpression(stream, ret, 0);
            res = Alloc<BinaryExpression>(StringToFourCC(TransientString(tok.text)), res, rhs);
        }
    }
    else if (precedence == 8)
    {
        res = ParseExpression(stream, ret, 9);
        if (stream.Match(TokenType::LessThan) || stream.Match(TokenType::LessThanEqual) || stream.Match(TokenType::GreaterThan) || stream.Match(TokenType::GreaterThanEqual))
        {
            Token tok = stream.Data();
            stream.Consume();
            Expression* rhs = ParseExpression(stream, ret, 0);
            res = Alloc<BinaryExpression>(StringToFourCC(TransientString(tok.text)), res, rhs);
        }
    }
    else if (precedence == 9)
    {
        res = ParseExpression(stream, ret, 10);
        if (stream.Match(TokenType::Equal) || stream.Match(TokenType::NotEqual))
        {
            Token tok = stream.Data();
            stream.Consume();
            Expression* rhs = ParseExpression(stream, ret, 0);
            res = Alloc<BinaryExpression>(StringToFourCC(TransientString(tok.text)), res, rhs);
        }
    }
    else if (precedence == 10)
    {
        res = ParseExpression(stream, ret, 11);
        if (stream.Match(TokenType::And))
        {
            Token tok = stream.Data();
            stream.Consume();
            Expression* rhs = ParseExpression(stream, ret, 0);
            res = Alloc<BinaryExpression>(StringToFourCC(TransientString(tok.text)), res, rhs);
        }
    }
    else if (precedence == 11)
    {
        res = ParseExpression(stream, ret, 12);
        if (stream.Match(TokenType::Xor))
        {
            Token tok = stream.Data();
            stream.Consume();
            Expression* rhs = ParseExpression(stream, ret, 0);
            res = Alloc<BinaryExpression>(StringToFourCC(TransientString(tok.text)), res, rhs);
        }
    }
    else if (precedence == 12)
    {
        res = ParseExpression(stream, ret, 13);
        if (stream.Match(TokenType::Or))
        {
            Token tok = stream.Data();
            stream.Consume();
            Expression* rhs = ParseExpression(stream, ret, 0);
            res = Alloc<BinaryExpression>(StringToFourCC(TransientString(tok.text)), res, rhs);
        }
    }
    else if (precedence == 13)
    {
        res = ParseExpression(stream, ret, 14);
        if (stream.Match(TokenType::LogicalAnd))
        {
            Token tok = stream.Data();
            stream.Consume();
            Expression* rhs = ParseExpression(stream, ret, 0);
            res = Alloc<BinaryExpression>(StringToFourCC(TransientString(tok.text)), res, rhs);
        }
    }
    else if (precedence == 14)
    {
        res = ParseExpression(stream, ret, 15);
        if (stream.Match(TokenType::LogicalOr))
        {
            Token tok = stream.Data();
            stream.Consume();
            Expression* rhs = ParseExpression(stream, ret, 0);
            res = Alloc<BinaryExpression>(StringToFourCC(TransientString(tok.text)), res, rhs);
        }
    }
    else if (precedence == 15)
    {
        res = ParseExpression(stream, ret, 16);
        if (stream.Match(TokenType::Question))
        {
            stream.Consume();
            Expression* true_value = ParseExpression(stream, ret, 0);
            if (stream.Match(TokenType::Colon))
            {
                stream.Consume();
                Expression* false_value = ParseExpression(stream, ret, 0);
                res = Alloc<TernaryExpression>(res, true_value, false_value);
            }
            else
            {
                ret.errors.Append(UnexpectedToken(stream, ":"));
                return nullptr;
            }
        }
    }
    else if (precedence == 16)
    {
        res = ParseExpression(stream, ret, 17);
        if (stream.Match(TokenType::AddAssign)
            || stream.Match(TokenType::SubAssign)
            || stream.Match(TokenType::MulAssign)
            || stream.Match(TokenType::DivAssign)
            || stream.Match(TokenType::ModAssign)
            || stream.Match(TokenType::LeftShiftAssign)
            || stream.Match(TokenType::RightShiftAssign)
            || stream.Match(TokenType::AndAssign)
            || stream.Match(TokenType::OrAssign)
            || stream.Match(TokenType::XorAssign)
            || stream.Match(TokenType::Assign)
            )
        {
            Token tok = stream.Data();
            stream.Consume();
            Expression* rhs = ParseExpression(stream, ret, 0);
            res = Alloc<BinaryExpression>(StringToFourCC(TransientString(tok.text)), res, rhs);
        }
    }
    else // Atoms
    {
        if (stream.Match(TokenType::Quote))
        {
            const Token& tok = stream.Data(0);
            res = Alloc<StringExpression>(std::string(tok.text));
            stream.Consume();
        }
        else if (stream.Match(TokenType::Integer))
        {
            const Token& tok = stream.Data(0);
            int value;
            std::from_chars(tok.text.data(), tok.text.data() + tok.text.size(), value);
            res = Alloc<IntExpression>(value);
            stream.Consume();
        }
        else if (stream.Match(TokenType::UnsignedInteger))
        {
            const Token& tok = stream.Data(0);
            unsigned int value;
            std::from_chars(tok.text.data(), tok.text.data() + tok.text.size(), value);
            res = Alloc<UIntExpression>(value);
            stream.Consume();
        }
        else if (stream.Match(TokenType::Float) || stream.Match(TokenType::Double))
        {
            const Token& tok = stream.Data(0);
            res = Alloc<FloatExpression>(std::stof(std::string(tok.text)));
            stream.Consume();
        }
        else if (stream.Match(TokenType::Bool))
        {
            res = Alloc<BoolExpression>(stream.Data().text == "true" ? true : false);
            stream.Consume();
        }
        else if (stream.Match(TokenType::LeftParant))
        {
            stream.Consume();
            res = ParseExpression(stream, ret);
            if (!stream.Match(TokenType::RightParant))
            {
                ret.errors.Append(UnexpectedToken(stream, ")"));
                return nullptr;
            }
            stream.Consume();
        }
        else if (stream.Match(TokenType::Identifier))
        {
            const Token& tok = stream.Data();
            res = Alloc<SymbolExpression>(FixedString(tok.text));
            stream.Consume();
        }
        else if (stream.Match(TokenType::Declared))
        {
            stream.Consume();
            if (stream.Match(TokenType::LeftAngle))
            {
                stream.Consume();
                if (stream.Match(TokenType::Identifier))
                {
                    const Token& tok = stream.Data();
                    res = Alloc<DeclaredExpression>(FixedString(tok.text));
                    stream.Consume();
                    
                    if (!stream.Match(TokenType::RightAngle))
                    {
                        ret.errors.Append(UnexpectedToken(stream, ">"));
                        return nullptr;
                    }
                    stream.Consume();
                }
                else
                {
                    ret.errors.Append(UnexpectedToken(stream, "identifier"));
                    return nullptr;
                }
            }
            else
            {
                ret.errors.Append(UnexpectedToken(stream, "<"));
                return nullptr;
            }
        }
        else
        {
            ret.errors.Append(IncorrectToken(stream));
            return nullptr;
        }
    }
    
    return res;
}

//------------------------------------------------------------------------------
/**
 */
Annotation*
ParseAnnotation(TokenStream& stream, ParseResult& ret)
{
    Annotation* res = nullptr;
    if (stream.Match(TokenType::Annot))
    {
        stream.Consume(); // @
        res = Alloc<Annotation>();
        if (!stream.Match(TokenType::Identifier))
        {
            ret.errors.Append(UnexpectedToken(stream, "annotation identifier"));
            return nullptr;
        }
        res->name = stream.Data(0).text;
        res->location = LocationFromToken(stream.Data(0));
        
        stream.Consume(); // name
        if (!stream.Match(TokenType::LeftParant))
        {
            ret.errors.Append(UnexpectedToken(stream, "("));
            return nullptr;
        }
        
        stream.Consume();
        res->value = ParseExpression(stream, ret);
        if (!stream.Match(TokenType::RightParant))
        {
            ret.errors.Append(UnexpectedToken(stream, ")"));
            return nullptr;
        }
    }
    return res;
}

//------------------------------------------------------------------------------
/**
 */
Variable*
ParseVar(TokenStream& stream, ParseResult& ret)
{
    
}

//------------------------------------------------------------------------------
/**
 */
Structure*
ParseStruct(TokenStream& stream, ParseResult& ret)
{
    TransientArray<Annotation*> annotations(32);
    while (auto annot = ParseAnnotation(stream, ret))
    {
        annotations.Append(annot);
    }
    if (!stream.Match(TokenType::Identifier, 1))
    {
        ret.errors.Append(UnexpectedToken(stream, "struct identifier"));
        return nullptr;
    }
    else
    {
        // Consume the 'struct' and name identifier
        stream.Consume(2);
        Structure* str = Alloc<Structure>();
        const Token& tok = stream.Data(0);
        str->name = tok.text;
        str->location = LocationFromToken(tok);
        
        if (!stream.Match(TokenType::LeftScope))
        {
            ret.errors.Append(UnexpectedToken(stream, "'{'"));
            return str;
        }
        stream.Consume();
        
        TransientArray<Variable*> members(1024);
        if (!stream.Match(TokenType::Identifier) && !stream.Match(TokenType::Colon))
        {
            
        }
    }
}


//------------------------------------------------------------------------------
/**
 */
ParseResult
Parse(TokenStream& stream)
{
    ParseResult ret;
    ret.ast = Alloc<Effect>();
    TokenType type = stream.Type(0);
    size_t lookAhead = 0;
    while (type != TokenType::InvalidToken)
    {
        switch (type)
        {
            case TokenType::Struct:
                //stream.PushLookBehind(lookAhead);
                //stream.Consume(lookAhead);
                ret.ast->symbols.Append(ParseStruct(stream, ret));
                //stream.PopLookBehind();
                break;
            case TokenType::Constant:
            case TokenType::Var:
            case TokenType::Uniform:
            case TokenType::Workgroup:
                ret.ast->symbols.Append(ParseVar(stream, ret));
                break;
            case TokenType::Enum:
            case TokenType::RenderState:
            case TokenType::SamplerState:
            case TokenType::Program:
            case TokenType::Generate:
            case TokenType::Identifier: // variables and functions
            default:
                lookAhead++;
        }
        
        type = stream.Type(lookAhead);
    }
    
    return ret;
}

} // namespace GPULang
