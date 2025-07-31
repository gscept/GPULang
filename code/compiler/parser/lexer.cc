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
#include "ast/enumeration.h"
#include "ast/samplerstate.h"
#include "ast/renderstate.h"
#include "ast/program.h"
#include "ast/alias.h"
#include "ast/variable.h"
#include "ast/function.h"
#include "ast/generate.h"
#include "ast/expressions/stringexpression.h"
#include "ast/expressions/arrayinitializerexpression.h"
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
#include "ast/statements/breakstatement.h"
#include "ast/statements/continuestatement.h"
#include "ast/statements/terminatestatement.h"
#include "ast/statements/expressionstatement.h"
#include "ast/statements/forstatement.h"
#include "ast/statements/ifstatement.h"
#include "ast/statements/whilestatement.h"
#include "ast/statements/switchstatement.h"
#include "ast/statements/scopestatement.h"

namespace GPULang
{
StaticMap HardCodedTokens = std::array{
    std::pair{ "alias"_c, TokenType::TypeAlias }
    , std::pair{ "as"_c, TokenType::As }
    , std::pair{ "struct"_c, TokenType::Struct }
    , std::pair{ "enum"_c, TokenType::Enum }
    , std::pair{ "while"_c, TokenType::While }
    , std::pair{ "do"_c, TokenType::Do }
    , std::pair{ "break"_c, TokenType::Break }
    , std::pair{ "discard"_c, TokenType::Discard }
    , std::pair{ "ray_ignore"_c, TokenType::RayIgnore }
    , std::pair{ "ray_terminate"_c, TokenType::RayTerminate }
    , std::pair{ "return"_c, TokenType::Return }
    , std::pair{ "for"_c, TokenType::For }
    , std::pair{ "if"_c, TokenType::If }
    , std::pair{ "else"_c, TokenType::Else }
    , std::pair{ "generate"_c, TokenType::ConditionalCompile }
    , std::pair{ "declared"_c, TokenType::Declared }
    , std::pair{ "packed"_c, TokenType::Packed }
    , std::pair{ "true"_c, TokenType::Bool }
    , std::pair{ "false"_c, TokenType::Bool }
    , std::pair{ "const"_c, TokenType::Const_Storage }
    , std::pair{ "var"_c, TokenType::Var_Storage }
    , std::pair{ "uniform"_c, TokenType::Uniform_Storage }
    , std::pair{ "workgroup"_c, TokenType::Workgroup_Storage }
    , std::pair{ "inline"_c, TokenType::Inline_Storage }
    , std::pair{ "link_defind"_c, TokenType::LinkDefined_Storage }
    , std::pair{ "in"_c, TokenType::In_Storage }
    , std::pair{ "out"_c, TokenType::Out_Storage }
    , std::pair{ "ray_payload"_c, TokenType::RayPayload_Storage }
    , std::pair{ "ray_hit_attribute"_c, TokenType::RayHitAttribute_Storage }
    , std::pair{ "ray_callable_data"_c, TokenType::CallableData_Storage }
    , std::pair{ "volatile"_c, TokenType::Volatile_Decorator }
    , std::pair{ "atomic"_c, TokenType::Atomic_Decorator }
    , std::pair{ "no_read"_c, TokenType::NoRead_Decorator }
    , std::pair{ "non_temporal"_c, TokenType::NonTemporal_Decorator }
    , std::pair{ "binding"_c, TokenType::Binding_Decorator }
    , std::pair{ "group"_c, TokenType::Group_Decorator }
    , std::pair{ "no_interpolate"_c, TokenType::NoInterpolate_Modifier }
    , std::pair{ "no_perspective"_c, TokenType::NoPerspective_Modifier }
    , std::pair{ "patch"_c, TokenType::Patch_Domain }
    , std::pair{ "entry_point"_c, TokenType::EntryPoint_Attribute }
    , std::pair{ "threads"_c, TokenType::Threads_Attribute }
    , std::pair{ "threads_x"_c, TokenType::ThreadsX_Attribute }
    , std::pair{ "threads_y"_c, TokenType::ThreadsY_Attribute }
    , std::pair{ "threads_z"_c, TokenType::ThreadsZ_Attribute }
    , std::pair{ "local_size"_c, TokenType::Threads_Attribute }
    , std::pair{ "local_size_x"_c, TokenType::ThreadsX_Attribute }
    , std::pair{ "local_size_y"_c, TokenType::ThreadsY_Attribute }
    , std::pair{ "local_size_z"_c, TokenType::ThreadsZ_Attribute }
    , std::pair{ "early_depth"_c, TokenType::EarlyDepth_Attribute }
    , std::pair{ "depth_lesser"_c, TokenType::DepthLesser_Attribute }
    , std::pair{ "depth_greater"_c, TokenType::DepthGreater_Attribute }
    , std::pair{ "subgroup_size"_c, TokenType::SubgroupSize_Attribute }
    , std::pair{ "subgroups_per_workgroup"_c, TokenType::SubgroupsPerWorkgroup_Attribute }
    , std::pair{ "input_vertices"_c, TokenType::InputVertices_Attribute }
    , std::pair{ "max_output_vertices"_c, TokenType::MaxOutputVertices_Attribute }
    , std::pair{ "winding"_c, TokenType::Winding_Attribute }
    , std::pair{ "input_topology"_c, TokenType::InputTopology_Attribute }
    , std::pair{ "output_topology"_c, TokenType::OutputTopology_Attribute }
    , std::pair{ "patch_type"_c, TokenType::PatchType_Attribute }
    , std::pair{ "partition"_c, TokenType::Partition_Attribute }
    , std::pair{ "pixel_origin"_c, TokenType::PixelOrigin_Attribute }
    , std::pair{ "derivative_index_linear"_c, TokenType::DerivativeIndexLinear_Attribute }
    , std::pair{ "derivative_index_quad"_c, TokenType::DerivativeIndexQuad_Attribute }
    , std::pair{ "mutable"_c, TokenType::Mutable_TypeModifier }
    , std::pair{ "sampled"_c, TokenType::Sampled_TypeModifier }
    , std::pair{ "literal"_c, TokenType::Literal_TypeModifier }
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
    , std::pair{ "<="_c, TokenType::LessThanEqual }
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
    , std::pair{ "rgba16"_c, TokenType::Rgba16 }
    , std::pair{ "rgb10_a2"_c, TokenType::Rgb10_A2 }
    , std::pair{ "rgba8"_c, TokenType::Rgba8 }
    , std::pair{ "rg16"_c, TokenType::Rg16 }
    , std::pair{ "rg8"_c, TokenType::Rg8 }
    , std::pair{ "r16"_c, TokenType::R16 }
    , std::pair{ "r8"_c, TokenType::R8 }
    , std::pair{ "rgba16_snorm"_c, TokenType::Rgba16_Snorm }
    , std::pair{ "rgba8_snorm"_c, TokenType::Rgba8_Snorm }
    , std::pair{ "rg16_snorm"_c, TokenType::Rg16_Snorm }
    , std::pair{ "rg8_snorm"_c, TokenType::Rg8_Snorm }
    , std::pair{ "r16_snorm"_c, TokenType::R16_Snorm }
    , std::pair{ "r8_snorm"_c, TokenType::R8_Snorm }
    , std::pair{ "rgba32f"_c, TokenType::Rgba32F }
    , std::pair{ "rgba16f"_c, TokenType::Rgba16F }
    , std::pair{ "rg32f"_c, TokenType::Rg32F }
    , std::pair{ "rg16f"_c, TokenType::Rg16F }
    , std::pair{ "r11g11b10f"_c, TokenType::R11G11B10F }
    , std::pair{ "r32f"_c, TokenType::R32F }
    , std::pair{ "r16f"_c, TokenType::R16F }
    , std::pair{ "rga32i"_c, TokenType::Rgba32I }
    , std::pair{ "rgba16i"_c, TokenType::Rgba16I }
    , std::pair{ "rgba8i"_c, TokenType::Rgba8I }
    , std::pair{ "rg32i"_c, TokenType::Rg32I }
    , std::pair{ "rg16i"_c, TokenType::Rg16I }
    , std::pair{ "rg8i"_c, TokenType::Rg8I }
    , std::pair{ "r32i"_c, TokenType::R32I }
    , std::pair{ "r16i"_c, TokenType::R16I }
    , std::pair{ "r8i"_c, TokenType::R8I }
    , std::pair{ "rga32u"_c, TokenType::Rgba32U }
    , std::pair{ "rgba16u"_c, TokenType::Rgba16U }
    , std::pair{ "rgb10_a2u"_c, TokenType::Rgb10_A2U }
    , std::pair{ "rgba8u"_c, TokenType::Rgba8U }
    , std::pair{ "rg32u"_c, TokenType::Rg32U }
    , std::pair{ "rg16u"_c, TokenType::Rg16U }
    , std::pair{ "rg8u"_c, TokenType::Rg8U }
    , std::pair{ "r32u"_c, TokenType::R32U }
    , std::pair{ "r16u"_c, TokenType::R16U }
    , std::pair{ "r8u"_c, TokenType::R8U }
    , std::pair{ "unknown"_c, TokenType::UnknownFormat }
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

static auto hexChar = [](const char c) -> bool
{
    if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
        return true;
    return false;
};

//------------------------------------------------------------------------------
/**
 */
TokenizationResult
Tokenize(const std::string& text, const FixedString& path)
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
    
    FixedString currentPath = path;
    
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
            line += 1;
            continue;
        }
        
        // End early
        if (it[0] == '\0')
        {
            Token newToken;
            ret.tokens.Append(newToken);
            ret.tokenTypes.Append(TokenType::End);
            return ret;
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
            newToken.path = currentPath;
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
            newToken.path = currentPath;
            TokenType type = TokenType::Integer;
            const char* begin = it;
            if (it[1] == 'x')
            {
                type = TokenType::Hex;
                it += 2;
                begin += 2; // Skip the 0x bit
                while (it != end)
                {
                    if (!hexChar(it[0]))
                        break;
                    it++;
                }
                
                // Unsigned hex
                if (it[0] == 'u' || it[0] == 'U')
                {
                    type = TokenType::UnsignedHex;
                    it++;
                }
            }
            else
            {
                while (it != end)
                {
                    if (!numberChar(it[0]))
                        break;
                    it++;
                }
                
                // If decimal point
                if (it[0] == '.')
                {
                    if (type == TokenType::Hex)
                    {
                        LexerError error;
                        error.path = currentPath;
                        error.message = "Incorrectly formatted hex number";
                        error.line = line;
                        error.pos = it - startOfLine;
                        ret.errors.Append(error);
                    }
                    it++;
                    if (numberStart(it[0]))
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

                    
                    // Floating point exponent
                    if (it[0] == 'e' || it[0] == 'E')
                    {
                        it++;
                        if (it[0] == '-' || it[0] == '+')
                            it++;
                        if (!numberStart(it[0]))
                        {
                            LexerError error;
                            error.path = currentPath;
                            error.message = "Incorrectly formatted exponent";
                            error.line = line;
                            error.pos = it - startOfLine;
                            ret.errors.Append(error);
                        }
                        else
                        {
                            it++;
                            while (it != end)
                            {
                                if (!numberChar(it[0]))
                                    break;
                                it++;
                            }
                        }
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
                error.path = currentPath;
                error.message = "String not closed";
                error.line = line;
                error.pos = begin - startOfLine;
                ret.errors.Append(error);
            }
            
            Token newToken;
            newToken.path = currentPath;
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
            newToken.path = currentPath;
            auto it1 = HardCodedTokens.Find(it[0]);
            
            // Handle include/line directives
            if (it1->second == TokenType::Directive)
            {
                it = skipWS(it+1, end);
                if (identifierStart(it[0]))
                {
                    const char* begin = it;
                    while (it != end)
                    {
                        if (!identifierChar(it[0]))
                            break;
                        it++;
                    }
                    
                    std::string_view dir(begin, it);
                    if (dir == "line")
                    {
                        it = skipWS(it+1, end);
                        const char* begin = it;
                        if (numberStart(it[0]))
                        {
                            while (it != end)
                            {
                                if (!numberChar(it[0]))
                                    break;
                                it++;
                            }
                        }
                        std::string_view lineNo(begin, it);
                        int newLine;
                        std::from_chars(lineNo.begin(), lineNo.end(), newLine);
                        line = newLine;
                        
                        it = skipWS(it+1, end);
                        if (it[0] == '\"')
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
                            std::string_view path(begin + 1, it - 1);
                            currentPath = path;
                        }
                    }
                }
            }
            
            const char* begin = it;
            it++;
            if (it1 == HardCodedTokens.end())
            {
                LexerError error;
                error.path = currentPath;
                error.message = TStr("Unknown character ", it[0]);
                error.line = line;
                error.pos = it - startOfLine;
                ret.errors.Append(error);
            }
            else
            {
                // Check for multi character operators
                std::string_view threeCharOp(begin, it+2);
                auto it3 = HardCodedTokens.Find(threeCharOp);
                if (it3 == HardCodedTokens.end())
                {
                    std::string_view twoCharOp(begin, it+1);
                    auto it2 = HardCodedTokens.Find(twoCharOp);
                    if (it2 != HardCodedTokens.end())
                    {
                        it1 = it2;
                        it++;
                    }
                }
                else
                {
                    it1 = it3;
                    it += 2;
                }
            }
            if (it1 != HardCodedTokens.end())
            {
                newToken.text = std::string_view(begin, it);
                newToken.startLine = line;
                newToken.endLine = line;
                newToken.startChar = begin - startOfLine;
                newToken.endChar = it - startOfLine;
                ret.tokens.Append(newToken);
                ret.tokenTypes.Append(it1->second);
            }
            continue;
        }
    }
    
    // Add end of line
    Token newToken;
    ret.tokens.Append(newToken);
    ret.tokenTypes.Append(TokenType::End);
    return ret;
}

//------------------------------------------------------------------------------
/**
 */
Symbol::Location
LocationFromToken(const Token& tok)
{
    Symbol::Location ret;
    ret.file = tok.path;
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
    const Token& tok = stream.Data(0);
    ParseError error;
    error.message = TStr("Expected ", expected, ", got ", tok.text);
    error.path = tok.path;
    error.line = tok.startLine;
    error.pos = tok.startChar;
    return error;
}

//------------------------------------------------------------------------------
/**
 */
ParseError
IncorrectToken(const TokenStream& stream)
{
    const Token& tok = stream.Data(0);
    ParseError error;
    error.message = TStr("Invalid token ", tok.text);
    error.path = tok.path;
    error.line = tok.startLine;
    error.pos = tok.startChar;
    return error;
}

//------------------------------------------------------------------------------
/**
 */
ParseError
Unsupported(const TokenStream& stream, const char* unexpected, const char* symbol)
{
    const Token& tok = stream.Data(0);
    ParseError error;
    error.message = TStr("%s not supported for %s", unexpected, symbol);
    error.path = tok.path;
    error.line = tok.startLine;
    error.pos = tok.startChar;
    return error;
}

//------------------------------------------------------------------------------
/**
 */
ParseError
Limit(const TokenStream& stream, const char* name, size_t size)
{
    const Token& tok = stream.Data(0);

    ParseError error;
    error.message = TStr("Limit %s(%d) reached", name, size);
    error.path = tok.path;
    error.line = tok.startLine;
    error.pos = tok.startChar;
    return error;
}

Expression* ParseExpression(TokenStream&, ParseResult&, Expression*, int);
//------------------------------------------------------------------------------
/**
 */
FixedArray<Expression*>
ParseExpressionList(TokenStream& stream, ParseResult& ret)
{
    TransientArray<Expression*> expressions(256);
    while (Expression* expr = ParseExpression(stream, ret, nullptr, 0))
    {
        if (expressions.Full())
        {
            ret.errors.Append(Limit(stream, "expression list length", 256));
            break;
        }
        expressions.Append(expr);
        if (!stream.Match(TokenType::Comma))
            break;
    }
    return FixedArray<Expression*>(expressions);
}

//------------------------------------------------------------------------------
/**
 */
Expression*
ParseExpression(TokenStream& stream, ParseResult& ret, Expression* prev = nullptr, int precedence = 0)
{
    Expression* res = prev;

    if (precedence == 0)
    {
        res = ParseExpression(stream, ret, res, 1);
        
        // Assignment is always right-associative
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
            const Token& tok = stream.Data(-1);
            Expression* rhs = ParseExpression(stream, ret, res, 1);
            res = Alloc<BinaryExpression>(StringToFourCC(TransientString(tok.text)), res, rhs);
            res->location = LocationFromToken(tok);
        }
    }
    else if (precedence == 1)
    {
        res = ParseExpression(stream, ret, res, 2);
        
        // Right associative ternary
        if (stream.Match(TokenType::Question))
        {
            const Token& tok = stream.Data(-1);
            Expression* true_value = ParseExpression(stream, ret, res, 0);
            if (stream.Match(TokenType::Colon))
            {
                Expression* false_value = ParseExpression(stream, ret, res, 0);
                res = Alloc<TernaryExpression>(res, true_value, false_value);
                res->location = LocationFromToken(tok);
            }
            else
            {
                ret.errors.Append(UnexpectedToken(stream, ":"));
                return nullptr;
            }
        }
    }
    else if (precedence == 2)
    {
        res = ParseExpression(stream, ret, res, 3);
        while (stream.Match(TokenType::LogicalOr))
        {
            const Token& tok = stream.Data(-1);
            Expression* rhs = ParseExpression(stream, ret, res, 3);
            res = Alloc<BinaryExpression>(StringToFourCC(TransientString(tok.text)), res, rhs);
            res->location = LocationFromToken(tok);

        }
    }
    else if (precedence == 3)
    {
        res = ParseExpression(stream, ret, res, 4);
        while (stream.Match(TokenType::LogicalAnd))
        {
            const Token& tok = stream.Data(-1);
            Expression* rhs = ParseExpression(stream, ret, res, 4);
            res = Alloc<BinaryExpression>(StringToFourCC(TransientString(tok.text)), res, rhs);
            res->location = LocationFromToken(tok);

        }
    }
    else if (precedence == 4)
    {
        res = ParseExpression(stream, ret, res, 5);
        while (stream.Match(TokenType::Or))
        {
            const Token& tok = stream.Data(-1);
            Expression* rhs = ParseExpression(stream, ret, res, 5);
            res = Alloc<BinaryExpression>(StringToFourCC(TransientString(tok.text)), res, rhs);
            res->location = LocationFromToken(tok);
        }
    }
    else if (precedence == 5)
    {
        res = ParseExpression(stream, ret, res, 6);
        while (stream.Match(TokenType::Xor))
        {
            const Token& tok = stream.Data(-1);
            Expression* rhs = ParseExpression(stream, ret, res, 6);
            res = Alloc<BinaryExpression>(StringToFourCC(TransientString(tok.text)), res, rhs);
            res->location = LocationFromToken(tok);
        }
    }
    else if (precedence == 6)
    {
        res = ParseExpression(stream, ret, res, 7);
        while (stream.Match(TokenType::And))
        {
            const Token& tok = stream.Data(-1);
            Expression* rhs = ParseExpression(stream, ret, res, 7);
            res = Alloc<BinaryExpression>(StringToFourCC(TransientString(tok.text)), res, rhs);
            res->location = LocationFromToken(tok);
        }
    }
    else if (precedence == 7)
    {
        res = ParseExpression(stream, ret, res, 8);
        while (stream.Match(TokenType::Equal) || stream.Match(TokenType::NotEqual))
        {
            const Token& tok = stream.Data(-1);
            Expression* rhs = ParseExpression(stream, ret, res, 8);
            res = Alloc<BinaryExpression>(StringToFourCC(TransientString(tok.text)), res, rhs);
            res->location = LocationFromToken(tok);
        }
    }
    else if (precedence == 8)
    {
        res = ParseExpression(stream, ret, res, 9);
        while (stream.Match(TokenType::LeftAngle) || stream.Match(TokenType::LessThanEqual) || stream.Match(TokenType::RightAngle) || stream.Match(TokenType::GreaterThanEqual))
        {
            const Token& tok = stream.Data(-1);
            Expression* rhs = ParseExpression(stream, ret, res, 9);
            res = Alloc<BinaryExpression>(StringToFourCC(TransientString(tok.text)), res, rhs);
            res->location = LocationFromToken(tok);
        }
    }
    else if (precedence == 9)
    {
        res = ParseExpression(stream, ret, res, 10);
        while (stream.Match(TokenType::LeftShift) || stream.Match(TokenType::RightShift))
        {
            const Token& tok = stream.Data(-1);
            Expression* rhs = ParseExpression(stream, ret, res, 10);
            res = Alloc<BinaryExpression>(StringToFourCC(TransientString(tok.text)), res, rhs);
            res->location = LocationFromToken(tok);
        }
    }
    else if (precedence == 10)
    {
        res = ParseExpression(stream, ret, res, 11);
        while (stream.Match(TokenType::Add) || stream.Match(TokenType::Sub))
        {
            const Token& tok = stream.Data(-1);
            Expression* rhs = ParseExpression(stream, ret, res, 11);
            res = Alloc<BinaryExpression>(StringToFourCC(TransientString(tok.text)), res, rhs);
            res->location = LocationFromToken(tok);
        }
    }
    else if (precedence == 11)
    {
        res = ParseExpression(stream, ret, res, 12);
        while (stream.Match(TokenType::Mul) || stream.Match(TokenType::Div) || stream.Match(TokenType::Mod))
        {
            const Token& tok = stream.Data(-1);
            Expression* rhs = ParseExpression(stream, ret, res, 12);
            res = Alloc<BinaryExpression>(StringToFourCC(TransientString(tok.text)), res, rhs);
            res->location = LocationFromToken(tok);
        }
    }
    else if (precedence == 12)
    {
        // Right-associative unary operators
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
            const Token& tok = stream.Data(-1);
            
            Expression* rhs = ParseExpression(stream, ret, res, 12);
            res = Alloc<UnaryExpression>(StringToFourCC(TransientString(tok.text)), true, rhs);
            res->location = LocationFromToken(tok);
        }
        else
            res = ParseExpression(stream, ret, res, 13);

    }
    else if (precedence == 13)
    {
        res = ParseExpression(stream, ret, res, 14);
        while (stream.Match(TokenType::Dot))
        {
            const Token& tok = stream.Data(-1);
            Expression* rhs = ParseExpression(stream, ret, res, 14);
            res = Alloc<AccessExpression>(res, rhs, false);
            res->location = LocationFromToken(tok);
        }
    }
    else if (precedence == 14)
    {
        res = ParseExpression(stream, ret, res, 15);
        while (stream.Match(TokenType::LeftBracket))
        {
            const Token& tok = stream.Data(-1);
            Expression* index = ParseExpression(stream, ret, res, 0);
            if (stream.Match(TokenType::RightBracket))
            {
                res = Alloc<ArrayIndexExpression>(res, index);
                res->location = LocationFromToken(tok);
            }
            else
            {
                ret.errors.Append(UnexpectedToken(stream, "]"));
                return nullptr;
            }
        }
    }
    else if (precedence == 15)
    {
        res = ParseExpression(stream, ret, res, 16);
        if (stream.Match(TokenType::LeftParant))
        {
            const Token& tok = stream.Data(-1);
            FixedArray<Expression*> arguments;
            arguments = ParseExpressionList(stream, ret);
            if (stream.Match(TokenType::RightParant))
            {
                Symbol::Location prev = res->location;
                res = Alloc<CallExpression>(res, std::move(arguments));
                res->location = prev;
            }
            else
            {
                ret.errors.Append(UnexpectedToken(stream, ")"));
                return nullptr;
            }
        }
    }
    else if (precedence == 16)
    {
        res = ParseExpression(stream, ret, res, 17);
        if (stream.Match(TokenType::Increment) || stream.Match(TokenType::Decrement))
        {
            const Token& tok = stream.Data(-1);
            res = Alloc<UnaryExpression>(StringToFourCC(TransientString(stream.Data(-1).text)), false, res);
            res->location = LocationFromToken(tok);
        }
    }
    else if (precedence == 17)
    {
        if (stream.Match(TokenType::LeftBracket))
        {
            const Token& tok = stream.Data(-1);
            FixedArray<Expression*> expressions = ParseExpressionList(stream, ret);
            res = Alloc<ArrayInitializerExpression>(expressions);
            res->location = LocationFromToken(tok);
            if (!stream.Match(TokenType::RightBracket))
            {
                ret.errors.Append(UnexpectedToken(stream, "]"));
                return res;
            }
        }
        else if (stream.Match(TokenType::Quote))
        {
            const Token& tok = stream.Data(-1);
            res = Alloc<StringExpression>(std::string(tok.text));
            res->location = LocationFromToken(tok);
        }
        else if (stream.Match(TokenType::Integer))
        {
            const Token& tok = stream.Data(-1);
            int value;
            std::from_chars(tok.text.data(), tok.text.data() + tok.text.size(), value);
            res = Alloc<IntExpression>(value);
            res->location = LocationFromToken(tok);
        }
        else if (stream.Match(TokenType::UnsignedInteger))
        {
            const Token& tok = stream.Data(-1);
            unsigned int value;
            std::from_chars(tok.text.data(), tok.text.data() + tok.text.size(), value);
            res = Alloc<UIntExpression>(value);
            res->location = LocationFromToken(tok);
        }
        else if (stream.Match(TokenType::Hex))
        {
            const Token& tok = stream.Data(-1);
            int value;
            std::from_chars(tok.text.data(), tok.text.data() + tok.text.size(), value, 16);
            res = Alloc<IntExpression>(value);
            res->location = LocationFromToken(tok);
        }
        else if (stream.Match(TokenType::UnsignedHex))
        {
            const Token& tok = stream.Data(-1);
            unsigned int value;
            std::from_chars(tok.text.data(), tok.text.data() + tok.text.size(), value, 16);
            res = Alloc<UIntExpression>(value);
            res->location = LocationFromToken(tok);
        }
        else if (stream.Match(TokenType::Float) || stream.Match(TokenType::Double))
        {
            const Token& tok = stream.Data(-1);
            res = Alloc<FloatExpression>(std::stof(std::string(tok.text)));
            res->location = LocationFromToken(tok);
        }
        else if (stream.Match(TokenType::Bool))
        {
            const Token& tok = stream.Data(-1);
            res = Alloc<BoolExpression>(tok.text == "true" ? true : false);
            res->location = LocationFromToken(tok);
        }
        else if (stream.Match(TokenType::LeftParant))
        {
            res = ParseExpression(stream, ret);
            if (!stream.Match(TokenType::RightParant))
            {
                ret.errors.Append(UnexpectedToken(stream, ")"));
                return nullptr;
            }
        }
        else if (stream.Match(TokenType::Identifier))
        {
            const Token& tok = stream.Data(-1);
            res = Alloc<SymbolExpression>(FixedString(tok.text));
            res->location = LocationFromToken(tok);
        }
        else if (stream.Match(TokenType::Declared))
        {
            const Token& tok = stream.Data(-1);
            if (stream.Match(TokenType::LeftAngle))
            {
                if (stream.Match(TokenType::Identifier))
                {
                    const Token& tok = stream.Data(-1);
                    res = Alloc<DeclaredExpression>(FixedString(tok.text));
                    res->location = LocationFromToken(tok);
                    
                    if (!stream.Match(TokenType::RightAngle))
                    {
                        ret.errors.Append(UnexpectedToken(stream, ">"));
                        return nullptr;
                    }
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
        res = Alloc<Annotation>();
        if (!stream.Match(TokenType::Identifier))
        {
            ret.errors.Append(UnexpectedToken(stream, "annotation identifier"));
            return nullptr;
        }
        res->name = stream.Data(-1).text;
        res->location = LocationFromToken(stream.Data(-1));
        
        if (!stream.Match(TokenType::LeftParant))
        {
            ret.errors.Append(UnexpectedToken(stream, "("));
            return nullptr;
        }
        
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
Attribute*
ParseFunctionAttribute(TokenStream& stream, ParseResult& ret)
{
    Attribute* res = nullptr;
    if (
        stream.Match(TokenType::EntryPoint_Attribute)
        || stream.Match(TokenType::Threads_Attribute)
        || stream.Match(TokenType::ThreadsX_Attribute)
        || stream.Match(TokenType::ThreadsY_Attribute)
        || stream.Match(TokenType::ThreadsZ_Attribute)
        || stream.Match(TokenType::EarlyDepth_Attribute)
        || stream.Match(TokenType::DepthLesser_Attribute)
        || stream.Match(TokenType::DepthGreater_Attribute)
        || stream.Match(TokenType::SubgroupSize_Attribute)
        || stream.Match(TokenType::SubgroupsPerWorkgroup_Attribute)
        || stream.Match(TokenType::InputVertices_Attribute)
        || stream.Match(TokenType::MaxOutputVertices_Attribute)
        || stream.Match(TokenType::Winding_Attribute)
        || stream.Match(TokenType::InputTopology_Attribute)
        || stream.Match(TokenType::OutputTopology_Attribute)
        || stream.Match(TokenType::PatchType_Attribute)
        || stream.Match(TokenType::Partition_Attribute)
        || stream.Match(TokenType::PixelOrigin_Attribute)
        || stream.Match(TokenType::DerivativeIndexLinear_Attribute)
        || stream.Match(TokenType::DerivativeIndexQuad_Attribute)
        )
    {
        const Token& tok = stream.Data(-1);
        if (stream.Match(TokenType::LeftParant))
        {
            Expression* expr = ParseExpression(stream, ret);
            if (expr == nullptr)
            {
                ret.errors.Append(UnexpectedToken(stream, "expression"));
                return nullptr;
            }
            
            res = Alloc<Attribute>();
            res->name = tok.text;
            res->location = LocationFromToken(tok);
            res->expression = expr;
            if (!stream.Match(TokenType::RightParant))
            {
                ret.errors.Append(UnexpectedToken(stream, ")"));
                return nullptr;
            }
        }
        else
        {
            res = Alloc<Attribute>();
            res->name = tok.text;
            res->location = LocationFromToken(tok);
        }
    }
    return res;
}

//------------------------------------------------------------------------------
/**
 */
Attribute*
ParseVariableAttribute(TokenStream& stream, ParseResult& ret)
{
    Attribute* res = nullptr;
    if (
        stream.Match(TokenType::Binding_Decorator)
        || stream.Match(TokenType::Group_Decorator)
        || stream.Match(TokenType::Volatile_Decorator)
        || stream.Match(TokenType::Atomic_Decorator)
        || stream.Match(TokenType::NoRead_Decorator)
        || stream.Match(TokenType::NonTemporal_Decorator)
        )
    {
        const Token& tok = stream.Data(-1);
        if (stream.Match(TokenType::LeftParant))
        {
            Expression* expr = ParseExpression(stream, ret);
            if (expr == nullptr)
            {
                ret.errors.Append(UnexpectedToken(stream, "expression"));
                return nullptr;
            }
            
            res = Alloc<Attribute>();
            res->name = tok.text;
            res->location = LocationFromToken(tok);
            res->expression = expr;
            if (!stream.Match(TokenType::RightParant))
            {
                ret.errors.Append(UnexpectedToken(stream, ")"));
                return nullptr;
            }
        }
        else
        {
            res = Alloc<Attribute>();
            res->name = tok.text;
            res->location = LocationFromToken(tok);
        }
    }
    return res;
}

//------------------------------------------------------------------------------
/**
 */
Attribute*
ParseParameterAttribute(TokenStream& stream, ParseResult& ret)
{
    Attribute* res = nullptr;
    if (
        stream.Match(TokenType::In_Storage)
        || stream.Match(TokenType::Out_Storage)
        || stream.Match(TokenType::Uniform_Storage)
        || stream.Match(TokenType::Workgroup_Storage)
        || stream.Match(TokenType::RayPayload_Storage)
        || stream.Match(TokenType::CallableData_Storage)
        || stream.Match(TokenType::RayHitAttribute_Storage)
        || stream.Match(TokenType::NoInterpolate_Modifier)
        || stream.Match(TokenType::NoPerspective_Modifier)
        || stream.Match(TokenType::Patch_Domain)
        || stream.Match(TokenType::Binding_Decorator)
        )
    {
        const Token& tok = stream.Data(-1);
        if (stream.Match(TokenType::LeftParant))
        {
            Expression* expr = ParseExpression(stream, ret);
            if (expr == nullptr)
            {
                ret.errors.Append(UnexpectedToken(stream, "expression"));
                return nullptr;
            }
            
            res = Alloc<Attribute>();
            res->name = tok.text;
            res->location = LocationFromToken(tok);
            res->expression = expr;
            if (!stream.Match(TokenType::RightParant))
            {
                ret.errors.Append(UnexpectedToken(stream, ")"));
                return nullptr;
            }
        }
        else
        {
            res = Alloc<Attribute>();
            res->name = tok.text;
            res->location = LocationFromToken(tok);
        }
    }
    return res;
}

//------------------------------------------------------------------------------
/**
 */
bool
ParseType(TokenStream& stream, ParseResult& ret, Type::FullType& res)
{
    while (true)
    {
        if (stream.Match(TokenType::Mul))
        {
            res.AddModifier(Type::FullType::Modifier::Pointer);
        }
        else if (stream.Match(TokenType::LeftBracket))
        {
            Expression* sizeExpr = ParseExpression(stream, ret);
            if (sizeExpr != nullptr)
            {
                res.AddModifier(Type::FullType::Modifier::Array, sizeExpr);
            }
            else
            {
                res.AddModifier(Type::FullType::Modifier::Array);
            }
            
            if (!stream.Match(TokenType::RightBracket))
            {
                ret.errors.Append(UnexpectedToken(stream, "]"));
                return false;
            }
        }
        else
            break;
    }
    
    if (stream.Match(TokenType::Mutable_TypeModifier))
    {
        res.mut = true;
    }
    if (stream.Match(TokenType::Sampled_TypeModifier))
    {
        res.sampled = true;
    }
    if (stream.Match(TokenType::Literal_TypeModifier))
    {
        res.literal = true;
    }
    if (
        stream.Match(TokenType::Rgba16)
        || stream.Match(TokenType::Rgb10_A2)
        || stream.Match(TokenType::Rgba8)
        || stream.Match(TokenType::Rg16)
        || stream.Match(TokenType::Rg8)
        || stream.Match(TokenType::R16)
        || stream.Match(TokenType::R8)
        || stream.Match(TokenType::Rgba16_Snorm)
        || stream.Match(TokenType::Rgba8_Snorm)
        || stream.Match(TokenType::Rg16_Snorm)
        || stream.Match(TokenType::Rg8_Snorm)
        || stream.Match(TokenType::R16_Snorm)
        || stream.Match(TokenType::R8_Snorm)
        || stream.Match(TokenType::Rgba32F)
        || stream.Match(TokenType::Rgba16F)
        || stream.Match(TokenType::Rg32F)
        || stream.Match(TokenType::Rg16F)
        || stream.Match(TokenType::R11G11B10F)
        || stream.Match(TokenType::R32F)
        || stream.Match(TokenType::R16F)
        || stream.Match(TokenType::Rgba32I)
        || stream.Match(TokenType::Rgba16I)
        || stream.Match(TokenType::Rgba8I)
        || stream.Match(TokenType::Rg32I)
        || stream.Match(TokenType::Rg16I)
        || stream.Match(TokenType::Rg8I)
        || stream.Match(TokenType::R32I)
        || stream.Match(TokenType::R16I)
        || stream.Match(TokenType::R8I)
        || stream.Match(TokenType::Rgba32U)
        || stream.Match(TokenType::Rgba16U)
        || stream.Match(TokenType::Rgb10_A2U)
        || stream.Match(TokenType::Rgba8U)
        || stream.Match(TokenType::Rg32U)
        || stream.Match(TokenType::Rg16U)
        || stream.Match(TokenType::Rg8U)
        || stream.Match(TokenType::R32U)
        || stream.Match(TokenType::R16U)
        || stream.Match(TokenType::R8U)
        || stream.Match(TokenType::UnknownFormat)
        )
    {
        // TODO: Just set the image format here directly
        res.AddQualifier(FixedString(stream.Data(-1).text));
    }
    if (!stream.Match(TokenType::Identifier))
    {
        ret.errors.Append(UnexpectedToken(stream, "type"));
        return false;
    }
    res.name = stream.Data(-1).text;
    // TODO: Add location
    return true;
}

//------------------------------------------------------------------------------
/**
 */
struct Alias*
ParseAlias(TokenStream& stream, ParseResult& ret)
{
    Alias* res = nullptr;
    if (!stream.Match(TokenType::Identifier))
    {
        ret.errors.Append(UnexpectedToken(stream, "alias name"));
        return res;
    }
    res = Alloc<Alias>();
    res->name = stream.Data(-1).text;
    res->location = LocationFromToken(stream.Data(-1));
    res->nameLocation = res->location;
    
    if (!stream.Match(TokenType::As))
    {
        ret.errors.Append(UnexpectedToken(stream, "as"));
        return res;
    }
    
    if (!stream.Match(TokenType::Identifier))
    {
        ret.errors.Append(UnexpectedToken(stream, "alias type"));
        return res;
    }
    res->type = stream.Data(-1).text;
    res->typeLocation = LocationFromToken(stream.Data(-1));
    
    if (!stream.Match(TokenType::SemiColon))
    {
        ret.errors.Append(UnexpectedToken(stream, ";"));
        return res;
    }
    return res;
}

//------------------------------------------------------------------------------
/**
 */
FixedArray<Variable*>
ParseVariables(TokenStream& stream, ParseResult& ret)
{
    FixedArray<Variable*> res;
    
    TransientArray<Variable*> vars(128);
    while (stream.Match(TokenType::Identifier))
    {
        if (vars.Full())
        {
            ret.errors.Append(Limit(stream, "variable names", 128));
            break;
        }
        
        Variable* var = Alloc<Variable>();
        var->name = stream.Data(-1).text;
        var->location = LocationFromToken(stream.Data(-1));
        vars.Append(var);
        
        if (!stream.Match(TokenType::Comma))
            break;
    }
    res = vars;
    
    bool typeKnown = false;
    if (stream.Match(TokenType::Colon))
    {
        Type::FullType type;
        if (!ParseType(stream, ret, type))
        {
            return res;
        }
        typeKnown = true;
        
        for (auto& var : vars)
        {
            var->type = type;
        }
    }
    else
    {
        for (auto& var : vars)
        {
            var->type = Type::FullType{UNDEFINED_TYPE};
        }
    }
    
    TransientArray<Expression*> values(128);
    if (stream.Match(TokenType::Assign))
    {
        while (true)
        {
            if (values.Full())
            {
                ret.errors.Append(Limit(stream, "initializer values", 128));
                break;
            }
            values.Append(ParseExpression(stream, ret));
            
            // Match multiple arguments
            if (stream.Match(TokenType::Comma))
                continue;
            
            break;
        }
    }
    
    if (!typeKnown && values.size > 0)
    {
        if (values.size > vars.size)
        {
            const Token& tok = stream.Data(0);
            ParseError error;
            error.message = TStr("Expected a variable (%d) for for each value (%d)", vars.size, values.size);
            error.path = tok.path;
            error.line = tok.startLine;
            error.pos = tok.startChar;
            ret.errors.Append(error);
        }
        else if (values.size < vars.size)
        {
            const Token& tok = stream.Data(0);
            ParseError error;
            error.message = TStr("Expected a value (%d) for for each variable (%d)", values.size, vars.size);
            error.path = tok.path;
            error.line = tok.startLine;
            error.pos = tok.startChar;
            ret.errors.Append(error);
        }
    }
    
    for (size_t i = 0; i < vars.size; i++)
    {
        vars[i]->valueExpression = values[i];
    }

    return res;
}

Statement* ParseScopeStatement(TokenStream& stream, ParseResult& ret);
//------------------------------------------------------------------------------
/**
 */
Function*
ParseFunction(TokenStream& stream, ParseResult& ret)
{
    Function* res = nullptr;
    if (!stream.Match(TokenType::Identifier))
    {
        ret.errors.Append(UnexpectedToken(stream, "function name"));
        return nullptr;
    }
    
    res = Alloc<Function>();
    res->name = stream.Data(-1).text;
    res->location = LocationFromToken(stream.Data(-1));
    
    if (!stream.Match(TokenType::LeftParant))
    {
        ret.errors.Append(UnexpectedToken(stream, "("));
        return nullptr;
    }
    
    TransientArray<Variable*> parameters(128);
    while (true)
    {
        TransientArray<Attribute*> paramAttributes(32);
        while (auto attr = ParseParameterAttribute(stream, ret))
        {
            paramAttributes.Append(attr);
        }
         
        if (!stream.Match(TokenType::Identifier))
        {
            if (paramAttributes.size > 0)
            {
                ret.errors.Append(UnexpectedToken(stream, "parameter name"));
                return nullptr;
            }
            else
                break;
        }
        
        Variable* var = Alloc<Variable>();
        var->name = stream.Data(-1).text;
        var->location = LocationFromToken(stream.Data(-1));
        var->attributes = paramAttributes;
        
        if (!stream.Match(TokenType::Colon))
        {
            ret.errors.Append(UnexpectedToken(stream, "parameter type"));
            return nullptr;
        }
        
        if (!ParseType(stream, ret, var->type))
            return res;
        
        if (parameters.Full())
            ret.errors.Append(Limit(stream, "function parameters", 128));
        parameters.Append(var);
        
        // If there is a comma, parse the next variable
        if (stream.Match(TokenType::Comma))
            continue;

        break;
    }
    res->parameters = parameters;
    
    // If no comma and parameter is fully parsed, but there is no ), it means invalid argument list
    if (!stream.Match(TokenType::RightParant))
    {
        ret.errors.Append(UnexpectedToken(stream, ")"));
        return res;
    }
    
    if (!ParseType(stream, ret, res->returnType))
        return res;
    
    res->ast = ParseScopeStatement(stream, ret);
    
    return res;
}

//------------------------------------------------------------------------------
/**
 */
Enumeration*
ParseEnumeration(TokenStream& stream, ParseResult& ret)
{
    Enumeration* res = nullptr;

    if (!stream.Match(TokenType::Identifier))
    {
        ret.errors.Append(UnexpectedToken(stream, "enum identifier"));
        return res;
    }
    res = Alloc<Enumeration>();
    const Token& tok = stream.Data(-1);
    res->name = tok.text;
    res->location = LocationFromToken(tok);
    
    
    if (stream.Match(TokenType::Colon))
    {
        ParseType(stream, ret, res->type);
    }
    else
    {
        res->type = Type::FullType{ "u32"_c };
    }
    
    if (!stream.Match(TokenType::LeftScope))
    {
        ret.errors.Append(UnexpectedToken(stream, "{"));
        return res;
    }
    
    TransientArray<FixedString> labels(256);
    TransientArray<Symbol::Location> locations(256);
    TransientArray<Expression*> values(256);
    while (stream.Match(TokenType::Identifier))
    {
        if (labels.Full())
        {
            ret.errors.Append(Limit(stream, "enum labels", 256));
            break;
        }
        
        FixedString label = FixedString(stream.Data(-1).text);
        Symbol::Location labelLocation = LocationFromToken(stream.Data(-1));
        Expression* value = nullptr;
        if (stream.Match(TokenType::Assign))
        {
            value = ParseExpression(stream, ret);
        }
        
        labels.Append(label);
        locations.Append(labelLocation);
        values.Append(value);
        
        if (stream.Match(TokenType::Comma))
            continue;
        
        break;
    }
    
    res->labels = labels;
    res->labelLocations = locations;
    res->values = values;
    
    if (!stream.Match(TokenType::RightScope))
    {
        ret.errors.Append(UnexpectedToken(stream, "}"));
        return res;
    }
    
    if (!stream.Match(TokenType::SemiColon))
    {
        ret.errors.Append(UnexpectedToken(stream, ";"));
        return res;
    }
    return res;
}

//------------------------------------------------------------------------------
/**
 */
RenderStateInstance*
ParseRenderState(TokenStream& stream, ParseResult& ret)
{
    RenderStateInstance* res = nullptr;

    if (!stream.Match(TokenType::Identifier))
    {
        ret.errors.Append(UnexpectedToken(stream, "render_state name"));
        return res;
    }
    
    res = Alloc<RenderStateInstance>();
    const Token& tok = stream.Data(-1);
    res->name = tok.text;
    res->location = LocationFromToken(tok);
    
    if (!stream.Match(TokenType::LeftScope))
    {
        ret.errors.Append(UnexpectedToken(stream, "{"));
        return res;
    }
    
    TransientArray<Expression*> entries(32);
    while (Expression* expr = ParseExpression(stream, ret))
    {
        if (entries.Full())
        {
            ret.errors.Append(Limit(stream, "render_state entries", 32));
            break;
        }
        entries.Append(expr);
        if (!stream.Match(TokenType::SemiColon))
        {
            ret.errors.Append(UnexpectedToken(stream, ";"));
            return res;
        }
    }
    res->entries = entries;
    
    if (!stream.Match(TokenType::RightScope))
    {
        ret.errors.Append(UnexpectedToken(stream, "}"));
        return res;
    }
    
    if (!stream.Match(TokenType::SemiColon))
    {
        ret.errors.Append(UnexpectedToken(stream, ";"));
        return res;
    }
    
    return res;
}

//------------------------------------------------------------------------------
/**
 */
SamplerStateInstance*
ParseSamplerState(TokenStream& stream, ParseResult& ret)
{
    SamplerStateInstance* res = nullptr;

    if (!stream.Match(TokenType::Identifier))
    {
        ret.errors.Append(UnexpectedToken(stream, "sampler_state name"));
        return res;
    }
    
    res = Alloc<SamplerStateInstance>();
    const Token& tok = stream.Data(-1);
    res->name = tok.text;
    res->location = LocationFromToken(tok);
    
    if (!stream.Match(TokenType::LeftScope))
    {
        ret.errors.Append(UnexpectedToken(stream, "{"));
        return res;
    }
    
    TransientArray<Expression*> entries(32);
    while (Expression* expr = ParseExpression(stream, ret))
    {
        if (entries.Full())
        {
            ret.errors.Append(Limit(stream, "sampler_state entries", 32));
            break;
        }
        entries.Append(expr);
        if (!stream.Match(TokenType::SemiColon))
        {
            ret.errors.Append(UnexpectedToken(stream, ";"));
            return res;
        }
    }
    res->entries = entries;
    
    if (!stream.Match(TokenType::RightScope))
    {
        ret.errors.Append(UnexpectedToken(stream, "}"));
        return res;
    }
    
    if (!stream.Match(TokenType::SemiColon))
    {
        ret.errors.Append(UnexpectedToken(stream, ";"));
        return res;
    }
    
    return res;
}

//------------------------------------------------------------------------------
/**
 */
ProgramInstance*
ParseProgram(TokenStream& stream, ParseResult& ret)
{
    ProgramInstance* res = nullptr;
    if (!stream.Match(TokenType::Identifier))
    {
        ret.errors.Append(UnexpectedToken(stream, "program name"));
        return res;
    }
    
    res = Alloc<ProgramInstance>();
    const Token& tok = stream.Data(-1);
    res->name = tok.text;
    res->location = LocationFromToken(tok);
    
    if (!stream.Match(TokenType::LeftScope))
    {
        ret.errors.Append(UnexpectedToken(stream, "{"));
        return res;
    }
    
    TransientArray<Expression*> entries(32);
    while (Expression* expr = ParseExpression(stream, ret))
    {
        if (entries.Full())
        {
            ret.errors.Append(Limit(stream, "program entries", 32));
            break;
        }
        entries.Append(expr);
        if (!stream.Match(TokenType::SemiColon))
        {
            ret.errors.Append(UnexpectedToken(stream, ";"));
            return res;
        }
    }
    res->entries = entries;
    
    if (!stream.Match(TokenType::RightScope))
    {
        ret.errors.Append(UnexpectedToken(stream, "}"));
        return res;
    }
    
    if (!stream.Match(TokenType::SemiColon))
    {
        ret.errors.Append(UnexpectedToken(stream, ";"));
        return res;
    }
    
    return res;
}

Statement* ParseGenerateStatement(TokenStream& stream, ParseResult& ret);
//------------------------------------------------------------------------------
/**
 */
Statement*
ParseGenerateScopeStatement(TokenStream& stream, ParseResult& ret)
{
    Statement* res = nullptr;
    if (stream.Match(TokenType::LeftAngle))
    {
        const Token& tok = stream.Data(-1);

        TransientArray<Annotation*> annotations(128);
        TransientArray<Attribute*> attributes(128);
        PinnedArray<Symbol*> symbols = 0xFFFFFF;
        std::vector<Expression*> unfinished;
        while (true)
        {
            if (Annotation* annot = ParseAnnotation(stream, ret))
            {
                annotations.Append(annot);
                continue;
            }
            else if (Attribute* attr = ParseFunctionAttribute(stream, ret))
            {
                attributes.Append(attr);
            }
            else if (Attribute* attr = ParseVariableAttribute(stream, ret))
            {
                attributes.Append(attr);
            }
            else if (Statement* stat = ParseGenerateStatement(stream, ret))
            {
                symbols.Append(stat);
            }
            else if (
                     stream.Match(TokenType::Const_Storage)
                     || stream.Match(TokenType::Var_Storage)
                     || stream.Match(TokenType::Uniform_Storage)
                     || stream.Match(TokenType::Workgroup_Storage)
                     || stream.Match(TokenType::Inline_Storage)
                     || stream.Match(TokenType::LinkDefined_Storage)
                     )
            {
                const Token& tok = stream.Data(-1);
                FixedArray<Variable*> vars = ParseVariables(stream, ret);
                if (vars.size > 0)
                {
                    Attribute* storage = Alloc<Attribute>();
                    storage->name = tok.text;
                    storage->location = LocationFromToken(tok);
                    storage->expression = nullptr;
                    attributes.Append(storage);
                    
                    ret.ast->symbols.Grow(vars.size);
                    for (auto var : vars)
                    {
                        var->attributes = attributes;
                        var->annotations = annotations;
                        symbols.Append(var);
                    }
                    attributes.size = 0;
                    annotations.size = 0;
                    
                    if (!stream.Match(TokenType::SemiColon))
                    {
                        ret.errors.Append(UnexpectedToken(stream, ";"));
                        break;
                    }
                }
            }
            else if (stream.Match(TokenType::SamplerState))
            {
                SamplerStateInstance* sym = ParseSamplerState(stream, ret);
                sym->annotations = annotations;
                sym->attributes = attributes;
                annotations.size = 0;
                attributes.size = 0;
                symbols.Append(sym);
            }
            else if (stream.Type() == TokenType::Identifier && stream.Type(1) == TokenType::LeftParant)
            {
                Function* sym = ParseFunction(stream, ret);
                sym->annotations = annotations;
                sym->attributes = attributes;
                annotations.size = 0;
                attributes.size = 0;
                symbols.Append(sym);
            }
            else if (stream.Match(TokenType::TypeAlias))
            {
                Alias* sym = ParseAlias(stream, ret);
                if (annotations.size != 0)
                {
                    ret.errors.Append(Unsupported(stream, "annotations", "alias"));
                    break;
                }
                if (attributes.size != 0)
                {
                    ret.errors.Append(Unsupported(stream, "attributes", "alias"));
                    break;
                }
                symbols.Append(sym);
            }
            else if (stream.Match(TokenType::RightAngle))
            {
                if (annotations.size != 0)
                {
                    ret.errors.Append(UnexpectedToken(stream, "variable/function/sampler_state/program because of annotation"));
                    break;
                }
                if (attributes.size != 0)
                {
                    ret.errors.Append(UnexpectedToken(stream, "variable/function/sampler_state because of attribute"));
                    break;
                }
                break;
            }
            else
            {
                ret.errors.Append(UnexpectedToken(stream, "variables/sampler_state/function/if/alias"));
                return res;
            }
        }
        
        res = Alloc<ScopeStatement>(std::move(symbols), unfinished);
        res->location = LocationFromToken(tok);
    }
    return res;
}

//------------------------------------------------------------------------------
/**
 */
Statement*
ParseGenerateStatement(TokenStream& stream, ParseResult& ret)
{
    Statement* res = nullptr;

    if (stream.Match(TokenType::If))
    {
        const Token& tok = stream.Data(-1);
        if (!stream.Match(TokenType::LeftParant))
        {
            ret.errors.Append(UnexpectedToken(stream, "("));
            return res;
        }
        
        Expression* cond = ParseExpression(stream, ret);
        if (cond == nullptr)
            return res;
        
        if (!stream.Match(TokenType::RightParant))
        {
            ret.errors.Append(UnexpectedToken(stream, ")"));
            return res;
        }
        
        Statement* ifBody = ParseGenerateScopeStatement(stream, ret);
        if (ifBody == nullptr)
        {
            ret.errors.Append(UnexpectedToken(stream, "if body"));
            return res;
        }
        
        Statement* elseBody = nullptr;
        if (stream.Match(TokenType::Else))
        {
            elseBody = ParseGenerateStatement(stream, ret);
            if (elseBody == nullptr)
            {
                ret.errors.Append(UnexpectedToken(stream, "else body"));
                return res;
            }
        }
        
        res = Alloc<IfStatement>(cond, ifBody, elseBody);
        res->location = LocationFromToken(tok);
    }
    else
    {
        res = ParseGenerateScopeStatement(stream, ret);
    }
    
    return res;
}


//------------------------------------------------------------------------------
/**
 */
Generate*
ParseGenerate(TokenStream& stream, ParseResult& ret)
{
    Generate* res = nullptr;
    Symbol* body = ParseGenerateScopeStatement(stream, ret);
    if (body == nullptr)
    {
        ret.errors.Append(UnexpectedToken(stream, "generate body"));
        return res;
    }
    
    PinnedArray<Symbol*> symbols = 1;
    symbols.Append(body);
    res = Alloc<Generate>(symbols);
    
    if (!stream.Match(TokenType::SemiColon))
    {
        ret.errors.Append(UnexpectedToken(stream, ";"));
        return res;
    }
    
    return res;
}

//------------------------------------------------------------------------------
/**
 */
Structure*
ParseStruct(TokenStream& stream, ParseResult& ret)
{
    Structure* res = nullptr;

    if (!stream.Match(TokenType::Identifier))
    {
        ret.errors.Append(UnexpectedToken(stream, "struct identifier"));
        return res;
    }

    // Consume the 'struct' and name identifier
    res = Alloc<Structure>();
    const Token& tok = stream.Data(-1);
    res->name = tok.text;
    res->location = LocationFromToken(tok);

    if (!stream.Match(TokenType::LeftScope))
    {
        ret.errors.Append(UnexpectedToken(stream, "{"));
        return res;
    }
    
    TransientArray<Symbol*> members(1024);
    while (stream.Match(TokenType::Identifier))
    {
        Variable* var = Alloc<Variable>();
        var->name = stream.Data(-1).text;
        var->location = LocationFromToken(stream.Data(-1));
        
        if (!stream.Match(TokenType::Colon))
        {
            ret.errors.Append(UnexpectedToken(stream, ":"));
            break;
        }
        
        if (!ParseType(stream, ret, var->type))
        {
            ret.errors.Append(UnexpectedToken(stream, "type"));
            break;
        }
        
        if (!stream.Match(TokenType::SemiColon))
        {
            ret.errors.Append(UnexpectedToken(stream, ";"));
            break;
        }
        members.Append(var);
    }
    res->symbols = members;
    
    if (!stream.Match(TokenType::RightScope))
    {
        ret.errors.Append(UnexpectedToken(stream, "}"));
        return res;
    }
    
    if (!stream.Match(TokenType::SemiColon))
    {
        ret.errors.Append(UnexpectedToken(stream, ";"));
        return res;
    }

    return res;
}

Statement* ParseStatement(TokenStream& stream, ParseResult& ret);
//------------------------------------------------------------------------------
/**
 */
Statement*
ParseScopeStatement(TokenStream& stream, ParseResult& ret)
{
    Statement* res = nullptr;

    if (stream.Match(TokenType::LeftScope))
    {
        TransientArray<Attribute*> attributes(128);
        PinnedArray<Symbol*> symbols = 0xFFFFFFFF;
        std::vector<Expression*> unfinished;
        
        while (true)
        {
            if (Statement* stat = ParseStatement(stream, ret))
            {
                symbols.Append(stat);
            }
            else if (Attribute* attr = ParseVariableAttribute(stream, ret))
            {
                attributes.Append(attr);
            }
            else if (
                     stream.Match(TokenType::Const_Storage)
                     || stream.Match(TokenType::Var_Storage)
                     )
            {
                Attribute* storage = Alloc<Attribute>();
                storage->name = stream.Data(-1).text;
                storage->location = LocationFromToken(stream.Data(-1));
                storage->expression = nullptr;
                attributes.Append(storage);
                
                FixedArray<Variable*> vars = ParseVariables(stream, ret);
                if (vars.size > 0)
                {
                    symbols.Grow(vars.size);
                    for (auto var : vars)
                    {
                        symbols.Append(var);
                        var->attributes = attributes;
                    }
                    attributes.size = 0;
                    
                    if (!stream.Match(TokenType::SemiColon))
                    {
                        ret.errors.Append(UnexpectedToken(stream, ";"));
                        return res;
                    }
                }
                else
                    break;
            }
            else if (stream.Match(TokenType::TypeAlias))
            {
                Alias* alias = ParseAlias(stream, ret);
                if (attributes.size > 0)
                {
                    ret.errors.Append(Unsupported(stream, "attribute", "alias"));
                    return res;
                }
                if (alias != nullptr)
                    symbols.Append(alias);
                else
                    break;
            }
            else if (stream.Match(TokenType::RightScope))
            {
                if (attributes.size != 0)
                {
                    ret.errors.Append(UnexpectedToken(stream, "variable/function/sampler_state because of attribute"));
                    break;
                }
                break;
            }
            else
            {
                ret.errors.Append(UnexpectedToken(stream, "variables/expression/alias/statement"));
                break;
            }
        }
        res = Alloc<ScopeStatement>(std::move(symbols), unfinished);
    }
    return res;
}

//------------------------------------------------------------------------------
/**
 */
Statement*
ParseStatement(TokenStream& stream, ParseResult& ret)
{
    Statement* res = nullptr;

    if (stream.Match(TokenType::Break))
    {
        res = Alloc<BreakStatement>();
        if (!stream.Match(TokenType::SemiColon))
        {
            ret.errors.Append(UnexpectedToken(stream, "; after expression"));
            return res;
        }
    }
    else if (stream.Match(TokenType::Return))
    {
        res = Alloc<TerminateStatement>(ParseExpression(stream, ret), TerminateStatement::TerminationType::Return);
        if (!stream.Match(TokenType::SemiColon))
        {
            ret.errors.Append(UnexpectedToken(stream, "; after expression"));
            return res;
        }
    }
    else if (stream.Match(TokenType::Discard))
    {
        res = Alloc<TerminateStatement>(nullptr, TerminateStatement::TerminationType::Discard);
        if (!stream.Match(TokenType::SemiColon))
        {
            ret.errors.Append(UnexpectedToken(stream, "; after expression"));
            return res;
        }
    }
    else if (stream.Match(TokenType::RayIgnore))
    {
        res = Alloc<TerminateStatement>(nullptr, TerminateStatement::TerminationType::RayIgnoreIntersection);
        if (!stream.Match(TokenType::SemiColon))
        {
            ret.errors.Append(UnexpectedToken(stream, "; after expression"));
            return res;
        }
    }
    else if (stream.Match(TokenType::RayTerminate))
    {
        res = Alloc<TerminateStatement>(nullptr, TerminateStatement::TerminationType::RayTerminate);
        if (!stream.Match(TokenType::SemiColon))
        {
            ret.errors.Append(UnexpectedToken(stream, "; after expression"));
            return res;
        }
    }
    else if (stream.Match(TokenType::If))
    {
        if (!stream.Match(TokenType::LeftParant))
        {
            ret.errors.Append(UnexpectedToken(stream, "("));
            return res;
        }
        
        Expression* cond = ParseExpression(stream, ret);
        if (cond == nullptr)
            return res;
        
        if (!stream.Match(TokenType::RightParant))
        {
            ret.errors.Append(UnexpectedToken(stream, ")"));
            return res;
        }
        
        Statement* ifBody = ParseStatement(stream, ret);
        if (ifBody == nullptr)
        {
            ret.errors.Append(UnexpectedToken(stream, "statement"));
            return res;
        }
        
        Statement* elseBody = nullptr;
        if (stream.Match(TokenType::Else))
        {
            elseBody = ParseStatement(stream, ret);
            if (elseBody == nullptr)
            {
                ret.errors.Append(UnexpectedToken(stream, "statement"));
                return res;
            }
        }
        
        res = Alloc<IfStatement>(cond, ifBody, elseBody);
    }
    else if (res = ParseScopeStatement(stream, ret))
    {
    }
    else if (stream.Match(TokenType::For))
    {
        if (!stream.Match(TokenType::LeftParant))
        {
            ret.errors.Append(UnexpectedToken(stream, "("));
            return res;
        }
        
        if (!(stream.Match(TokenType::Const_Storage) || stream.Match(TokenType::Var_Storage)))
        {
            ret.errors.Append(UnexpectedToken(stream, "const/var"));
            return res;
        }
        
        Attribute* storage = Alloc<Attribute>();
        storage->name = stream.Data(-1).text;
        storage->location = LocationFromToken(stream.Data(-1));
        storage->expression = nullptr;
        
        FixedArray<Variable*> variables = ParseVariables(stream, ret);
        for (auto var : variables)
            var->attributes = { storage };
        
        if (!stream.Match(TokenType::SemiColon))
        {
            ret.errors.Append(UnexpectedToken(stream, ";"));
            return res;
        }   
        
        Expression* cond = ParseExpression(stream, ret);
        if (!stream.Match(TokenType::SemiColon))
        {
            ret.errors.Append(UnexpectedToken(stream, ";"));
            return res;
        }
        
        Expression* postLoop = ParseExpression(stream, ret);
        if (!stream.Match(TokenType::RightParant))
        {
            ret.errors.Append(UnexpectedToken(stream, ")"));
            return res;
        }
        Statement* body = ParseStatement(stream, ret);
        res = Alloc<ForStatement>(variables, cond, postLoop, body);
    }
    else if (stream.Match(TokenType::While))
    {
        if (!stream.Match(TokenType::LeftParant))
        {
            ret.errors.Append(UnexpectedToken(stream, "("));
            return res;
        }
        Expression* cond = ParseExpression(stream, ret);
        
        if (!stream.Match(TokenType::RightParant))
        {
            ret.errors.Append(UnexpectedToken(stream, ")"));
            return res;
        }
        Statement* body = ParseStatement(stream, ret);
        res = Alloc<WhileStatement>(cond, body, false);
    }
    else if (stream.Match(TokenType::Do))
    {
        Statement* body = ParseStatement(stream, ret);
        if (!stream.Match(TokenType::While))
        {
            ret.errors.Append(UnexpectedToken(stream, "while"));
            return res;
        }
        
        if (!stream.Match(TokenType::LeftParant))
        {
            ret.errors.Append(UnexpectedToken(stream, "("));
            return res;
        }
        Expression* cond = ParseExpression(stream, ret);
        
        if (!stream.Match(TokenType::RightParant))
        {
            ret.errors.Append(UnexpectedToken(stream, ")"));
            return res;
        }
        res = Alloc<WhileStatement>(cond, body, true);
        
        if (!stream.Match(TokenType::SemiColon))
        {
            ret.errors.Append(UnexpectedToken(stream, "; after expression"));
            return res;
        }
    }
    else
    {
        FixedArray<Expression*> exprs = ParseExpressionList(stream, ret);
        if (exprs.size > 0)
        {
            if (!stream.Match(TokenType::SemiColon))
            {
                ret.errors.Append(UnexpectedToken(stream, "; after expression"));
                return res;
            }
            
            res = Alloc<ExpressionStatement>(exprs);
        }
    }
    
    return res;
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
    
    TransientArray<Annotation*> annotations(128);
    TransientArray<Attribute*> attributes(128);
    while (true)
    {
        if (ret.errors.size > 0)
            return ret;
        
        if (stream.Match(TokenType::Directive))
        {
            // skip
        }
        else if (Annotation* annot = ParseAnnotation(stream, ret))
        {
            annotations.Append(annot);
        }
        else if (Attribute* attr = ParseFunctionAttribute(stream, ret))
        {
            attributes.Append(attr);
        }
        else if (Attribute* attr = ParseVariableAttribute(stream, ret))
        {
            attributes.Append(attr);
        }
        else if (stream.Match(TokenType::Packed))
        {
            Attribute* attr = Alloc<Attribute>();
            attr->name = stream.Data(0).text;
            attr->location = LocationFromToken(stream.Data(-1));
            attributes.Append(attr);
        }
        else if (stream.Match(TokenType::Struct))
        {
            Structure* sym = ParseStruct(stream, ret);
            sym->annotations = annotations;
            sym->attributes = attributes;
            annotations.size = 0;
            attributes.size = 0;
            ret.ast->symbols.Append(sym);
        }
        else if (
                 stream.Match(TokenType::Const_Storage)
                 || stream.Match(TokenType::Var_Storage)
                 || stream.Match(TokenType::Uniform_Storage)
                 || stream.Match(TokenType::Workgroup_Storage)
                 || stream.Match(TokenType::Inline_Storage)
                 || stream.Match(TokenType::LinkDefined_Storage)
                 )
        {
            Attribute* storage = Alloc<Attribute>();
            storage->name = stream.Data(-1).text;
            storage->location = LocationFromToken(stream.Data(-1));
            storage->expression = nullptr;
            attributes.Append(storage);
            
            FixedArray<Variable*> vars = ParseVariables(stream, ret);
            if (vars.size > 0)
            {
                ret.ast->symbols.Grow(vars.size);
                for (auto var : vars)
                {
                    var->attributes = attributes;
                    var->annotations = annotations;
                    ret.ast->symbols.Append(var);
                }
                attributes.size = 0;
                annotations.size = 0;
                
                if (!stream.Match(TokenType::SemiColon))
                {
                    ret.errors.Append(UnexpectedToken(stream, ";"));
                    break;
                }
            }
            
        }
        else if (stream.Match(TokenType::Enum))
        {
            Enumeration* sym = ParseEnumeration(stream, ret);
            if (annotations.size != 0)
            {
                ret.errors.Append(Unsupported(stream, "annotations", "generate"));
                break;
            }
            if (attributes.size != 0)
            {
                ret.errors.Append(Unsupported(stream, "attributes", "generate"));
                break;
            }
            ret.ast->symbols.Append(sym);
            
        }
        else if (stream.Match(TokenType::RenderState))
        {
            RenderStateInstance* sym = ParseRenderState(stream, ret);
            if (annotations.size != 0)
            {
                ret.errors.Append(Unsupported(stream, "annotations", "generate"));
                break;
            }
            if (attributes.size != 0)
            {
                ret.errors.Append(Unsupported(stream, "attributes", "generate"));
                break;
            }
            ret.ast->symbols.Append(sym);
            
        }
        else if (stream.Match(TokenType::SamplerState))
        {
            SamplerStateInstance* sym = ParseSamplerState(stream, ret);
            sym->annotations = annotations;
            sym->attributes = attributes;
            annotations.size = 0;
            attributes.size = 0;
            ret.ast->symbols.Append(sym);
            
        }
        else if (stream.Match(TokenType::Program))
        {
            ProgramInstance* sym = ParseProgram(stream, ret);
            sym->annotations = annotations;
            if (attributes.size != 0)
            {
                ret.errors.Append(Unsupported(stream, "attributes", "generate"));
                break;
            }
            annotations.size = 0;
            ret.ast->symbols.Append(sym);
        }
        else if (stream.Match(TokenType::ConditionalCompile))
        {
            Generate* sym = ParseGenerate(stream, ret);
            if (annotations.size != 0)
            {
                ret.errors.Append(Unsupported(stream, "annotations", "generate"));
                break;
            }
            if (attributes.size != 0)
            {
                ret.errors.Append(Unsupported(stream, "attributes", "generate"));
                break;
            }
            ret.ast->symbols.Append(sym);
        }
        else if (stream.Type() == TokenType::Identifier && stream.Type(1) == TokenType::LeftParant)
        {
            Function* sym = ParseFunction(stream, ret);
            if (sym == nullptr)
            {
                ret.errors.Append(UnexpectedToken(stream, "function"));
                break;
            }
            sym->annotations = annotations;
            sym->attributes = attributes;
            annotations.size = 0;
            attributes.size = 0;
            ret.ast->symbols.Append(sym);
        }
        else if (stream.Match(TokenType::End))
        {
            if (annotations.size != 0)
            {
                ret.errors.Append(UnexpectedToken(stream, "variable/function/sampler_state/program because of annotation"));
                break;
            }
            if (attributes.size != 0)
            {
                ret.errors.Append(UnexpectedToken(stream, "variable/function/sampler_state because of attribute"));
                break;
            }
            break;
        }
        else
        {
            ret.errors.Append(UnexpectedToken(stream, "annotation/variable/function/sampler_state/program/alias"));
            break;
        }
    }
    
    return ret;
}

} // namespace GPULang
