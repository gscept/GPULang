#pragma once
//------------------------------------------------------------------------------
/**
    Lexer

    (C) 2025 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "string_utils.h"
#include "containers.h"

namespace GPULang
{
struct Effect;
enum TokenType
{
    InvalidToken
    , Identifier
    , Float
    , Double
    , Integer
    , UnsignedInteger
    , Bool
    , Struct
    , Enum
    , While
    , For
    , If
    , Else
    , Generate
    , Declared
    , Constant
    , Var
    , Uniform
    , Workgroup
    , In
    , Out
    , Directive
    , Annot
    , Comma
    , Colon
    , SemiColon
    , LeftParant    // (
    , RightParant   // )
    , LeftScope     // {
    , RightScope    // }
    , LeftBracket   // [
    , RightBracket  // ]
    , LeftAngle     // <
    , RightAngle    // >
    , Quote
    , Dot
    , Add
    , Sub
    , Mul
    , Div
    , Mod
    , Or
    , And
    , Xor
    , Not
    , Complement
    , Question
    , LeftShift
    , RightShift
    , AddAssign
    , SubAssign
    , MulAssign
    , DivAssign
    , ModAssign
    , LeftShiftAssign
    , RightShiftAssign
    , OrAssign
    , AndAssign
    , XorAssign
    , Increment
    , Decrement
    , LessThan
    , LessThanEqual
    , GreaterThan
    , GreaterThanEqual
    , Equal
    , NotEqual
    , LogicalOr
    , LogicalAnd

    , Assign
    , RenderState
    , SamplerState
    , Program
    , CommentRow
    , CommentBlockStart
    , CommentBlockEnd
};

struct Token
{
    std::string_view text;
    uint32_t startLine, endLine;
    uint32_t startChar, endChar;
};

struct LexerError
{
    GPULang::FixedString message;
    uint32_t line;
    uint32_t pos;
};


struct TokenizationResult
{
    PinnedArray<TokenType> tokenTypes = 0xFFFFFF;
    PinnedArray<Token> tokens = 0xFFFFFF;
    PinnedArray<LexerError> errors = 0xFFFF;
};

// Tokenize string
TokenizationResult Tokenize(const std::string& text);

struct ParseError
{
    GPULang::FixedString message;
    uint32_t line;
    uint32_t pos;
};

struct ParseResult
{
    Effect* ast;
    PinnedArray<ParseError> errors = 0xFFFF;
};

struct TokenStream
{
    TokenStream(const TokenizationResult& result)
    {
        this->tokens = std::move(result.tokens);
        this->tokenTypes = std::move(result.tokenTypes);
    }
    
    // Consume the token
    void Consume(size_t count = 1)
    {
        this->tokenIndex += count;
    }

    TokenType Type(size_t lookAhead = 0)
    {
        if (this->tokenIndex + lookAhead > this->tokenTypes.size)
            return TokenType::InvalidToken;
        else
            return this->tokenTypes[this->tokenIndex + lookAhead];
    }
    
    const Token& Data(size_t lookAhead = 0) const
    {
        static Token InvalidToken = Token();
        if (this->tokenIndex + lookAhead > this->tokens.size)
            return InvalidToken;
        else
            return this->tokens[this->tokenIndex + lookAhead];
    }
    
    bool Match(TokenType type, size_t lookAhead = 0)
    {
        return this->Type(lookAhead) == type;
    }
    
    size_t tokenIndex = 0;
    PinnedArray<TokenType> tokenTypes = 0xFFFFFF;
    PinnedArray<Token> tokens = 0xFFFFFF;
};

// Parse tokenized result and return AST
ParseResult Parse(TokenStream& stream);

}
