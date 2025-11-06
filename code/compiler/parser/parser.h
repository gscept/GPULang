#pragma once
//------------------------------------------------------------------------------
/**
    Lexer and Parser

    (C) 2025 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "string_utils.h"
#include "containers.h"

struct GPULangDiagnostic;
struct GPULangFile;

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
    , Switch
    , Case
    , Default
    , Break
    , Continue
    , Discard
    , RayTerminate
    , RayIgnore
    , Return
    , For
    , ForUniform
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
    , NoReflect_Decorator
    , NoInterpolate_Modifier
    , NoPerspective_Modifier
    , Centroid_Modifier
    , Sample_Modifier
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
    , ComputeDerivatives_Attribute
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
    , Arrow
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
    , Comment
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
    
    // All following tokens are special tokens inserted by the parser
    , Call
    , Subscript
    , ArrayInitializer
    , PrefixSub
    , PrefixAdd
    , PrefixIncrement
    , PrefixDecrement
    , PrefixMul       // Dereference
    , PrefixAnd       // Indirection
    
    , NumTokenTypes
};

struct Token
{
    std::string_view text;
    FixedString path;
    uint32_t startLine, endLine;
    uint16_t startChar, endChar;
};

struct TokenizationResult
{
    PinnedArray<TokenType> tokenTypes = 0xFFFFFF;
    PinnedArray<Token> tokens = 0xFFFFFF;
    PinnedArray<GPULangDiagnostic> diagnostics = 0xFFFF;
    PinnedSet<TransientString> resolvedPaths = 0xFFF;
    TransientArray<GPULangFile*> files = 128;
    size_t lineCount = 0;
};

// Tokenize string
void Tokenize(const GPULangFile* file, const TransientArray<std::string_view>& searchPaths, TokenizationResult& ret, bool captureComments = false);

struct ParseResult
{
    Effect* ast;
    PinnedArray<GPULangDiagnostic> diagnostics = 0xFFFF;
};
extern uint32_t TokenClassTable[(uint32_t)TokenType::NumTokenTypes];
struct TokenStream
{
    TokenStream(const TokenizationResult& result)
    {
        this->tokens = std::move(result.tokens);
        this->tokenTypes = std::move(result.tokenTypes);
        this->dataIt = this->tokens.begin();
        this->dataEnd = this->tokens.end();
        this->typeIt = this->tokenTypes.begin();
        this->typeEnd = this->tokenTypes.end();
        this->lastComment = nullptr;
    }
    
    // Consume the token
    void Consume(size_t count = 1)
    {
        this->dataIt += count;
        this->typeIt += count;
    }

    const TokenType Type(int64_t lookAhead = 0) const
    {
        if (this->typeIt + lookAhead > this->typeEnd)
            return TokenType::InvalidToken;
        else
            return *(this->typeIt + lookAhead);
    }
    
    const Token& Data(int64_t lookAhead = 0) const
    {
        static Token InvalidToken = Token();
        if (this->dataIt + lookAhead >= this->dataEnd)
            return InvalidToken;
        else
            return *(this->dataIt + lookAhead);
    }
    
    inline bool Match(TokenType type)
    {
        // If not matching comments, skip all comment tokens
        while (*this->typeIt == TokenType::Comment)
        {
            this->lastComment = this->dataIt;
            this->typeIt++;
            this->dataIt++;
        }
        if (*this->typeIt == type)
        {
            this->typeIt++;
            this->dataIt++;
            return true;
        }
        return false;
    }
    
    void Unmatch()
    {
        this->typeIt--;
        this->dataIt--;
    }
    
    inline bool MatchClass(uint32_t bits)
    {
        // Always skip comments when matching a class
        while (*this->typeIt == TokenType::Comment)
        {
            this->lastComment = this->dataIt;
            this->typeIt++;
            this->dataIt++;
        }
        if ((TokenClassTable[(uint32_t)*this->typeIt] & bits) == bits)
        {
            this->typeIt++;
            this->dataIt++;
            return true;
        }
        return false;
    }
    
    inline std::string_view ConsumeComment()
    {
        if (this->lastComment != nullptr)
        {
            std::string_view ret = this->lastComment->text;
            this->lastComment = nullptr;
            return ret;
        }
        return std::string_view();
    }
    
    Token* lastComment;
    TokenType* typeIt;
    TokenType* typeEnd;
    Token* dataIt;
    Token* dataEnd;
    PinnedArray<TokenType> tokenTypes = 0xFFFFFF;
    PinnedArray<Token> tokens = 0xFFFFFF;
};

// Parse tokenized result and return AST
ParseResult Parse(TokenStream& stream);

}
