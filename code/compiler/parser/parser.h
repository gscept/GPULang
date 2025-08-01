#pragma once
//------------------------------------------------------------------------------
/**
    Lexer and Parser

    (C) 2025 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "string_utils.h"
#include "containers.h"

namespace GPULang
{
struct Effect;
enum class TokenType
{
    InvalidToken
    , End
    , Identifier
    , Float
    , Double
    , Integer
    , UnsignedInteger
    , Hex
    , UnsignedHex
    , Bool
    , TypeAlias
    , As
    , Struct
    , Enum
    , While
    , Do
    , Break
    , Discard
    , RayTerminate
    , RayIgnore
    , Return
    , For
    , If
    , Else
    , ConditionalCompile
    , Declared
    , Packed
    , Const_Storage
    , Var_Storage
    , Uniform_Storage
    , Workgroup_Storage
    , Inline_Storage
    , LinkDefined_Storage
    , In_Storage
    , Out_Storage
    , RayPayload_Storage
    , RayHitAttribute_Storage
    , CallableData_Storage
    , Volatile_Decorator
    , Atomic_Decorator
    , NoRead_Decorator
    , NonTemporal_Decorator
    , Binding_Decorator
    , Group_Decorator
    , NoInterpolate_Modifier
    , NoPerspective_Modifier
    , Patch_Domain
    , EntryPoint_Attribute
    , Threads_Attribute
    , ThreadsX_Attribute
    , ThreadsY_Attribute
    , ThreadsZ_Attribute
    , EarlyDepth_Attribute
    , DepthLesser_Attribute
    , DepthGreater_Attribute
    , SubgroupSize_Attribute
    , SubgroupsPerWorkgroup_Attribute
    , InputVertices_Attribute
    , MaxOutputVertices_Attribute
    , Winding_Attribute
    , InputTopology_Attribute
    , OutputTopology_Attribute
    , PatchType_Attribute
    , Partition_Attribute
    , PixelOrigin_Attribute
    , DerivativeIndexLinear_Attribute
    , DerivativeIndexQuad_Attribute
    , Mutable_TypeModifier
    , Literal_TypeModifier
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
    , Rgba16
    , Rgb10_A2
    , Rgba8
    , Rg16
    , Rg8
    , R16
    , R8
    , Rgba16_Snorm
    , Rgba8_Snorm
    , Rg16_Snorm
    , Rg8_Snorm
    , R16_Snorm
    , R8_Snorm
    , Rgba32F
    , Rgba16F
    , Rg32F
    , Rg16F
    , R11G11B10F
    , R32F
    , R16F
    , Rgba32I
    , Rgba16I
    , Rgba8I
    , Rg32I
    , Rg16I
    , Rg8I
    , R32I
    , R16I
    , R8I
    , Rgba32U
    , Rgba16U
    , Rgb10_A2U
    , Rgba8U
    , Rg32U
    , Rg16U
    , Rg8U
    , R32U
    , R16U
    , R8U
    , UnknownFormat
};

struct Token
{
    std::string_view text;
    FixedString path;
    uint32_t startLine, endLine;
    uint32_t startChar, endChar;
};

struct LexerError
{
    GPULang::FixedString message;
    GPULang::FixedString path;
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
TokenizationResult Tokenize(const std::string& text, const TransientString& path);

struct ParseError
{
    GPULang::FixedString message;
    GPULang::FixedString path;
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

    TokenType Type(int64_t lookAhead = 0)
    {
        if (this->tokenIndex + lookAhead > this->tokenTypes.size)
            return TokenType::InvalidToken;
        else
            return this->tokenTypes[this->tokenIndex + lookAhead];
    }
    
    const Token& Data(int64_t lookAhead = 0) const
    {
        static Token InvalidToken = Token();
        if (this->tokenIndex + lookAhead > this->tokens.size)
            return InvalidToken;
        else
            return this->tokens[this->tokenIndex + lookAhead];
    }
    
    bool Match(TokenType type)
    {
        if (this->Type(0) == type)
        {
            this->lastMatched = type;
            this->tokenIndex++;
            this->current = this->Type(0);
            return true;
        }
        return false;
    }
    
    void Unmatch(size_t count)
    {
        this->tokenIndex -= count;
    }
    
    TokenType lastMatched;
    TokenType current;
    size_t tokenIndex = 0;
    PinnedArray<TokenType> tokenTypes = 0xFFFFFF;
    PinnedArray<Token> tokens = 0xFFFFFF;
};

// Parse tokenized result and return AST
ParseResult Parse(TokenStream& stream);

}
