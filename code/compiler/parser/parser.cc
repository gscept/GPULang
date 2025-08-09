//------------------------------------------------------------------------------
//  parser.cc
//  (C) 2025 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "parser.h"
#include "util.h"
#include "gpulangcompiler.h"
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
#include "ast/preprocessor.h"
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

#ifdef __WIN32__
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <io.h>
#define stat _stat
#else
#include <unistd.h>
#include <sys/stat.h>
#endif

namespace GPULang
{
static const uint32_t NUMERIC_CHARACTER = 0x1; // [0..9]
static const uint32_t HEX_NUMERIC_CHARACTER = 0x2; // [0..9|a..f|A..F]
static const uint32_t IDENTIFIER_START_CHARACTER = 0x4; // [a..z|A...Z|_]
static const uint32_t IDENTIFIER_CONTD_CHARACTER = 0x8; // [a..z|A...Z|_|0..9]
static const uint32_t SPECIAL_CHARACTER = 0x10; // [.,;:(){}[]<>+-*/%&|^=!~?@#\\]
static const uint32_t OPERATOR_CHARACTER = 0x20; // [.,(){}[]<>+-*/%&|^=!~]
static const uint32_t WHITESPACE_CHARACTER = 0x40; // [ \t\r\f\v]
static const uint32_t EOL_CHARACTER = 0x80;
static const uint32_t MULTILINE_COMMENT_END_BIT1 = 0x100; // end of multiline comment, first bit
static const uint32_t MULTILINE_COMMENT_END_BIT2 = 0x200; // end of multiline comment, second bit
static const uint32_t MULTILINE_COMMENT = MULTILINE_COMMENT_END_BIT1 | MULTILINE_COMMENT_END_BIT2;
static const uint32_t PATH_CHARACTER_BIT = 0x400; // path character, first bit

static uint32_t CharacterClassTable[256] = {};
struct CharacterClassInitializer
{
    CharacterClassInitializer()
    {
        for (char c = 'a'; c <= 'z'; ++c)
            CharacterClassTable[c] = IDENTIFIER_START_CHARACTER | IDENTIFIER_CONTD_CHARACTER | PATH_CHARACTER_BIT;
        for (char c = 'A'; c <= 'Z'; ++c)
            CharacterClassTable[c] = IDENTIFIER_START_CHARACTER | IDENTIFIER_CONTD_CHARACTER | PATH_CHARACTER_BIT;
        CharacterClassTable['_'] = IDENTIFIER_START_CHARACTER | IDENTIFIER_CONTD_CHARACTER | PATH_CHARACTER_BIT;
        for (char c = 'a'; c <= 'f'; ++c)
            CharacterClassTable[c] |= HEX_NUMERIC_CHARACTER;
        for (char c = 'A'; c <= 'F'; ++c)
            CharacterClassTable[c] |= HEX_NUMERIC_CHARACTER;
        
        for (char c = '0'; c <= '9'; ++c)
            CharacterClassTable[c] = HEX_NUMERIC_CHARACTER | NUMERIC_CHARACTER | IDENTIFIER_CONTD_CHARACTER | PATH_CHARACTER_BIT;
        
        CharacterClassTable['#'] = SPECIAL_CHARACTER;
        CharacterClassTable['@'] = SPECIAL_CHARACTER;
        CharacterClassTable[','] = SPECIAL_CHARACTER;
        CharacterClassTable[':'] = SPECIAL_CHARACTER;
        CharacterClassTable[';'] = SPECIAL_CHARACTER;
        CharacterClassTable['('] = SPECIAL_CHARACTER;
        CharacterClassTable[')'] = SPECIAL_CHARACTER;
        CharacterClassTable['{'] = SPECIAL_CHARACTER;
        CharacterClassTable['}'] = SPECIAL_CHARACTER;
        CharacterClassTable['['] = SPECIAL_CHARACTER;
        CharacterClassTable[']'] = SPECIAL_CHARACTER;
        CharacterClassTable['<'] = SPECIAL_CHARACTER | OPERATOR_CHARACTER;
        CharacterClassTable['>'] = SPECIAL_CHARACTER | OPERATOR_CHARACTER;
        CharacterClassTable['"'] = SPECIAL_CHARACTER;
        CharacterClassTable['.'] = SPECIAL_CHARACTER | PATH_CHARACTER_BIT;
        CharacterClassTable['+'] = SPECIAL_CHARACTER | OPERATOR_CHARACTER;
        CharacterClassTable['-'] = SPECIAL_CHARACTER | OPERATOR_CHARACTER;
        CharacterClassTable['*'] = SPECIAL_CHARACTER | OPERATOR_CHARACTER | MULTILINE_COMMENT_END_BIT1;
        CharacterClassTable['/'] = SPECIAL_CHARACTER | OPERATOR_CHARACTER | MULTILINE_COMMENT_END_BIT2  | PATH_CHARACTER_BIT;
        CharacterClassTable['%'] = SPECIAL_CHARACTER | OPERATOR_CHARACTER;
        CharacterClassTable['|'] = SPECIAL_CHARACTER | OPERATOR_CHARACTER;
        CharacterClassTable['&'] = SPECIAL_CHARACTER | OPERATOR_CHARACTER;
        CharacterClassTable['^'] = SPECIAL_CHARACTER | OPERATOR_CHARACTER;
        CharacterClassTable['!'] = SPECIAL_CHARACTER | OPERATOR_CHARACTER;
        CharacterClassTable['~'] = SPECIAL_CHARACTER;
        CharacterClassTable['?'] = SPECIAL_CHARACTER;
        CharacterClassTable['='] = SPECIAL_CHARACTER | OPERATOR_CHARACTER;
        
        CharacterClassTable[' '] = WHITESPACE_CHARACTER;
        CharacterClassTable['\t'] = WHITESPACE_CHARACTER;
        CharacterClassTable['\f'] = WHITESPACE_CHARACTER;
        CharacterClassTable['\v'] = WHITESPACE_CHARACTER;
        
        CharacterClassTable['\r'] = WHITESPACE_CHARACTER;
        CharacterClassTable['\n'] = EOL_CHARACTER;
        CharacterClassTable['\0'] = EOL_CHARACTER;
    }
} CharacterTableInitializer;

constexpr StaticMap HardCodedTokens = std::array{
    std::pair{ "alias"_h, TokenType::TypeAlias }
    , std::pair{ "as"_h, TokenType::As }
    , std::pair{ "struct"_h, TokenType::Struct }
    , std::pair{ "enum"_h, TokenType::Enum }
    , std::pair{ "while"_h, TokenType::While }
    , std::pair{ "do"_h, TokenType::Do }
    , std::pair{ "break"_h, TokenType::Break }
    , std::pair{ "continue"_h, TokenType::Continue }
    , std::pair{ "discard"_h, TokenType::Discard }
    , std::pair{ "ray_ignore"_h, TokenType::RayIgnore }
    , std::pair{ "ray_terminate"_h, TokenType::RayTerminate }
    , std::pair{ "return"_h, TokenType::Return }
    , std::pair{ "for"_h, TokenType::For }
    , std::pair{ "if"_h, TokenType::If }
    , std::pair{ "else"_h, TokenType::Else }
    , std::pair{ "generate"_h, TokenType::ConditionalCompile }
    , std::pair{ "declared"_h, TokenType::Declared }
    , std::pair{ "packed"_h, TokenType::Packed }
    , std::pair{ "true"_h, TokenType::Bool }
    , std::pair{ "false"_h, TokenType::Bool }
    , std::pair{ "const"_h, TokenType::Const_Storage }
    , std::pair{ "var"_h, TokenType::Var_Storage }
    , std::pair{ "uniform"_h, TokenType::Uniform_Storage }
    , std::pair{ "workgroup"_h, TokenType::Workgroup_Storage }
    , std::pair{ "inline"_h, TokenType::Inline_Storage }
    , std::pair{ "link_defined"_h, TokenType::LinkDefined_Storage }
    , std::pair{ "in"_h, TokenType::In_Storage }
    , std::pair{ "out"_h, TokenType::Out_Storage }
    , std::pair{ "ray_payload"_h, TokenType::RayPayload_Storage }
    , std::pair{ "ray_hit_attribute"_h, TokenType::RayHitAttribute_Storage }
    , std::pair{ "ray_callable_data"_h, TokenType::CallableData_Storage }
    , std::pair{ "volatile"_h, TokenType::Volatile_Decorator }
    , std::pair{ "atomic"_h, TokenType::Atomic_Decorator }
    , std::pair{ "no_read"_h, TokenType::NoRead_Decorator }
    , std::pair{ "non_temporal"_h, TokenType::NonTemporal_Decorator }
    , std::pair{ "binding"_h, TokenType::Binding_Decorator }
    , std::pair{ "group"_h, TokenType::Group_Decorator }
    , std::pair{ "no_interpolate"_h, TokenType::NoInterpolate_Modifier }
    , std::pair{ "no_perspective"_h, TokenType::NoPerspective_Modifier }
    , std::pair{ "patch"_h, TokenType::Patch_Domain }
    , std::pair{ "entry_point"_h, TokenType::EntryPoint_Attribute }
    , std::pair{ "threads"_h, TokenType::Threads_Attribute }
    , std::pair{ "threads_x"_h, TokenType::ThreadsX_Attribute }
    , std::pair{ "threads_y"_h, TokenType::ThreadsY_Attribute }
    , std::pair{ "threads_z"_h, TokenType::ThreadsZ_Attribute }
    , std::pair{ "local_size"_h, TokenType::Threads_Attribute }
    , std::pair{ "local_size_x"_h, TokenType::ThreadsX_Attribute }
    , std::pair{ "local_size_y"_h, TokenType::ThreadsY_Attribute }
    , std::pair{ "local_size_z"_h, TokenType::ThreadsZ_Attribute }
    , std::pair{ "early_depth"_h, TokenType::EarlyDepth_Attribute }
    , std::pair{ "depth_lesser"_h, TokenType::DepthLesser_Attribute }
    , std::pair{ "depth_greater"_h, TokenType::DepthGreater_Attribute }
    , std::pair{ "subgroup_size"_h, TokenType::SubgroupSize_Attribute }
    , std::pair{ "subgroups_per_workgroup"_h, TokenType::SubgroupsPerWorkgroup_Attribute }
    , std::pair{ "input_vertices"_h, TokenType::InputVertices_Attribute }
    , std::pair{ "max_output_vertices"_h, TokenType::MaxOutputVertices_Attribute }
    , std::pair{ "winding"_h, TokenType::Winding_Attribute }
    , std::pair{ "input_topology"_h, TokenType::InputTopology_Attribute }
    , std::pair{ "output_topology"_h, TokenType::OutputTopology_Attribute }
    , std::pair{ "patch_type"_h, TokenType::PatchType_Attribute }
    , std::pair{ "partition"_h, TokenType::Partition_Attribute }
    , std::pair{ "pixel_origin"_h, TokenType::PixelOrigin_Attribute }
    , std::pair{ "derivative_index_linear"_h, TokenType::DerivativeIndexLinear_Attribute }
    , std::pair{ "derivative_index_quad"_h, TokenType::DerivativeIndexQuad_Attribute }
    , std::pair{ "mutable"_h, TokenType::Mutable_TypeModifier }
    , std::pair{ "literal"_h, TokenType::Literal_TypeModifier }
    , std::pair{ "#"_h, TokenType::Directive }
    , std::pair{ "@"_h, TokenType::Annot }
    , std::pair{ ","_h, TokenType::Comma }
    , std::pair{ ":"_h, TokenType::Colon }
    , std::pair{ ";"_h, TokenType::SemiColon }
    , std::pair{ "("_h, TokenType::LeftParant }
    , std::pair{ ")"_h, TokenType::RightParant }
    , std::pair{ "{"_h, TokenType::LeftScope }
    , std::pair{ "}"_h, TokenType::RightScope }
    , std::pair{ "["_h, TokenType::LeftBracket }
    , std::pair{ "]"_h, TokenType::RightBracket }
    , std::pair{ "<"_h, TokenType::LeftAngle }
    , std::pair{ ">"_h, TokenType::RightAngle }
    , std::pair{ "\""_h, TokenType::Quote }
    , std::pair{ "."_h, TokenType::Dot }
    , std::pair{ "+"_h, TokenType::Add }
    , std::pair{ "-"_h, TokenType::Sub }
    , std::pair{ "*"_h, TokenType::Mul }
    , std::pair{ "/"_h, TokenType::Div }
    , std::pair{ "%"_h, TokenType::Mod }
    , std::pair{ "|"_h, TokenType::Or }
    , std::pair{ "&"_h, TokenType::And }
    , std::pair{ "^"_h, TokenType::Xor }
    , std::pair{ "!"_h, TokenType::Not }
    , std::pair{ "~"_h, TokenType::Complement }
    , std::pair{ "?"_h, TokenType::Question }
    , std::pair{ "="_h, TokenType::Assign }
    , std::pair{ "<<"_h, TokenType::LeftShift }
    , std::pair{ ">>"_h, TokenType::RightShift }
    , std::pair{ "+="_h, TokenType::AddAssign }
    , std::pair{ "-="_h, TokenType::SubAssign }
    , std::pair{ "*="_h, TokenType::MulAssign }
    , std::pair{ "/="_h, TokenType::DivAssign }
    , std::pair{ "%="_h, TokenType::ModAssign }
    , std::pair{ "<<="_h, TokenType::LeftShiftAssign }
    , std::pair{ ">>="_h, TokenType::RightShiftAssign }
    , std::pair{ "|="_h, TokenType::OrAssign }
    , std::pair{ "&="_h, TokenType::AndAssign }
    , std::pair{ "^="_h, TokenType::XorAssign }
    , std::pair{ "++"_h, TokenType::Increment }
    , std::pair{ "--"_h, TokenType::Decrement }
    , std::pair{ "<="_h, TokenType::LessThanEqual }
    , std::pair{ ">="_h, TokenType::GreaterThanEqual }
    , std::pair{ "=="_h, TokenType::Equal }
    , std::pair{ "!="_h, TokenType::NotEqual }
    , std::pair{ "||"_h, TokenType::LogicalOr }
    , std::pair{ "&&"_h, TokenType::LogicalAnd }
    , std::pair{ "render_state"_h, TokenType::RenderState }
    , std::pair{ "sampler_state"_h, TokenType::SamplerState }
    , std::pair{ "program"_h, TokenType::Program }
    , std::pair{ "//"_h, TokenType::CommentRow}
    , std::pair{ "/*"_h, TokenType::CommentBlockStart}
    , std::pair{ "*/"_h, TokenType::CommentBlockEnd}
    , std::pair{ "rgba16"_h, TokenType::Rgba16 }
    , std::pair{ "rgb10_a2"_h, TokenType::Rgb10_A2 }
    , std::pair{ "rgba8"_h, TokenType::Rgba8 }
    , std::pair{ "rg16"_h, TokenType::Rg16 }
    , std::pair{ "rg8"_h, TokenType::Rg8 }
    , std::pair{ "r16"_h, TokenType::R16 }
    , std::pair{ "r8"_h, TokenType::R8 }
    , std::pair{ "rgba16_snorm"_h, TokenType::Rgba16_Snorm }
    , std::pair{ "rgba8_snorm"_h, TokenType::Rgba8_Snorm }
    , std::pair{ "rg16_snorm"_h, TokenType::Rg16_Snorm }
    , std::pair{ "rg8_snorm"_h, TokenType::Rg8_Snorm }
    , std::pair{ "r16_snorm"_h, TokenType::R16_Snorm }
    , std::pair{ "r8_snorm"_h, TokenType::R8_Snorm }
    , std::pair{ "rgba32f"_h, TokenType::Rgba32F }
    , std::pair{ "rgba16f"_h, TokenType::Rgba16F }
    , std::pair{ "rg32f"_h, TokenType::Rg32F }
    , std::pair{ "rg16f"_h, TokenType::Rg16F }
    , std::pair{ "r11g11b10f"_h, TokenType::R11G11B10F }
    , std::pair{ "r32f"_h, TokenType::R32F }
    , std::pair{ "r16f"_h, TokenType::R16F }
    , std::pair{ "rga32i"_h, TokenType::Rgba32I }
    , std::pair{ "rgba16i"_h, TokenType::Rgba16I }
    , std::pair{ "rgba8i"_h, TokenType::Rgba8I }
    , std::pair{ "rg32i"_h, TokenType::Rg32I }
    , std::pair{ "rg16i"_h, TokenType::Rg16I }
    , std::pair{ "rg8i"_h, TokenType::Rg8I }
    , std::pair{ "r32i"_h, TokenType::R32I }
    , std::pair{ "r16i"_h, TokenType::R16I }
    , std::pair{ "r8i"_h, TokenType::R8I }
    , std::pair{ "rga32u"_h, TokenType::Rgba32U }
    , std::pair{ "rgba16u"_h, TokenType::Rgba16U }
    , std::pair{ "rgb10_a2u"_h, TokenType::Rgb10_A2U }
    , std::pair{ "rgba8u"_h, TokenType::Rgba8U }
    , std::pair{ "rg32u"_h, TokenType::Rg32U }
    , std::pair{ "rg16u"_h, TokenType::Rg16U }
    , std::pair{ "rg8u"_h, TokenType::Rg8U }
    , std::pair{ "r32u"_h, TokenType::R32U }
    , std::pair{ "r16u"_h, TokenType::R16U }
    , std::pair{ "r8u"_h, TokenType::R8U }
    , std::pair{ "unknown"_h, TokenType::UnknownFormat }
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
        if ((CharacterClassTable[start[0]] & WHITESPACE_CHARACTER) == WHITESPACE_CHARACTER)
            start++;
        else
            return start;
    }
    return end;
};

static auto lineEnd = [](const char* begin, const char* end) -> const char*
{
    const char* start = begin;
    while (start < end)
    {
        if ((CharacterClassTable[start[0]] & EOL_CHARACTER) != EOL_CHARACTER)
            start++;
        else
            return start+1;
    }
    return end;
};

static auto identifierStart = [](const char c) -> bool
{
    return (CharacterClassTable[c] & IDENTIFIER_START_CHARACTER) == IDENTIFIER_START_CHARACTER;
};

static auto pathStart = [](const char c) -> bool
{
    return (CharacterClassTable[c] & IDENTIFIER_START_CHARACTER) == IDENTIFIER_START_CHARACTER;
};

static auto pathEnd = [](const char* it, const char* end) -> const char*
{
    uint32_t c;
    do
    {
        c = CharacterClassTable[it[0]];
        if ((c & PATH_CHARACTER_BIT) != PATH_CHARACTER_BIT)
            return it;
        
        c = CharacterClassTable[it[1]];
        if ((c & PATH_CHARACTER_BIT) != PATH_CHARACTER_BIT)
            return it + 1;
            
        c = CharacterClassTable[it[2]];
        if ((c & PATH_CHARACTER_BIT) != PATH_CHARACTER_BIT)
            return it + 2;
            
        c = CharacterClassTable[it[3]];
        if ((c & PATH_CHARACTER_BIT) != PATH_CHARACTER_BIT)
            return it + 3;
            
        it += 4;
    } while(it < end);
    
    return end;
};

static auto identifierEnd = [](const char* it, const char* end) -> const char*
{
    uint32_t c;
    do
    {
        c = CharacterClassTable[it[0]];
        if ((c & IDENTIFIER_CONTD_CHARACTER) != IDENTIFIER_CONTD_CHARACTER)
            return it;
        
        c = CharacterClassTable[it[1]];
        if ((c & IDENTIFIER_CONTD_CHARACTER) != IDENTIFIER_CONTD_CHARACTER)
            return it + 1;
            
        c = CharacterClassTable[it[2]];
        if ((c & IDENTIFIER_CONTD_CHARACTER) != IDENTIFIER_CONTD_CHARACTER)
            return it + 2;
            
        c = CharacterClassTable[it[3]];
        if ((c & IDENTIFIER_CONTD_CHARACTER) != IDENTIFIER_CONTD_CHARACTER)
            return it + 3;
            
        it += 4;
    } while(it < end);
    
    return end;
};

static auto identifierChar = [](const char c) -> bool
{
    return (CharacterClassTable[c] & IDENTIFIER_CONTD_CHARACTER) == IDENTIFIER_CONTD_CHARACTER;
};

static auto numberStart = [](const char c) -> bool
{
    return (CharacterClassTable[c] & NUMERIC_CHARACTER) == NUMERIC_CHARACTER;
};

static auto numberEnd = [](const char* it, const char* end) -> const char*
{
    uint32_t c;
    do
    {
        c = CharacterClassTable[it[0]];
        if ((c & NUMERIC_CHARACTER) != NUMERIC_CHARACTER)
            return it;
        
        c = CharacterClassTable[it[1]];
        if ((c & NUMERIC_CHARACTER) != NUMERIC_CHARACTER)
            return it + 1;
            
        c = CharacterClassTable[it[2]];
        if ((c & NUMERIC_CHARACTER) != NUMERIC_CHARACTER)
            return it + 2;
            
        c = CharacterClassTable[it[3]];
        if ((c & NUMERIC_CHARACTER) != NUMERIC_CHARACTER)
            return it + 3;
            
        it += 4;
    } while(it < end);
    
    return end;
};

static auto hexEnd = [](const char* it, const char* end) -> const char*
{
    uint32_t c;
    do
    {
        c = CharacterClassTable[it[0]];
        if ((c & HEX_NUMERIC_CHARACTER) != HEX_NUMERIC_CHARACTER)
            return it;
        
        c = CharacterClassTable[it[1]];
        if ((c & HEX_NUMERIC_CHARACTER) != HEX_NUMERIC_CHARACTER)
            return it + 1;
            
        c = CharacterClassTable[it[2]];
        if ((c & HEX_NUMERIC_CHARACTER) != HEX_NUMERIC_CHARACTER)
            return it + 2;
            
        c = CharacterClassTable[it[3]];
        if ((c & HEX_NUMERIC_CHARACTER) != HEX_NUMERIC_CHARACTER)
            return it + 3;
            
        it += 4;
    } while(it < end);
    
    return end;
};

static auto numberChar = [](const char c) -> bool
{
    return (CharacterClassTable[c] & NUMERIC_CHARACTER) == NUMERIC_CHARACTER;
};

static auto hexChar = [](const char c) -> bool
{
    return (CharacterClassTable[c] & HEX_NUMERIC_CHARACTER) == HEX_NUMERIC_CHARACTER;
};

static auto operatorStart = [](const char c) -> bool
{
    return (CharacterClassTable[c] & OPERATOR_CHARACTER) == OPERATOR_CHARACTER;
};

static auto operatorEnd = [](const char* it, const char* end) -> const char*
{
    uint32_t c;
    do
    {
        c = CharacterClassTable[it[0]];
        if ((c & OPERATOR_CHARACTER) != OPERATOR_CHARACTER)
            return it;
        
        c = CharacterClassTable[it[1]];
        if ((c & OPERATOR_CHARACTER) != OPERATOR_CHARACTER)
            return it + 1;
            
        c = CharacterClassTable[it[2]];
        if ((c & OPERATOR_CHARACTER) != OPERATOR_CHARACTER)
            return it + 2;
            
        c = CharacterClassTable[it[3]];
        if ((c & OPERATOR_CHARACTER) != OPERATOR_CHARACTER)
            return it + 3;
            
        it += 4;
    } while(it < end);
    
    return end;
};

static auto commentEnd = [](const char* it, const char* end) -> const char*
{
    return it;
};

static auto resolvePath = [](const std::string_view& path, const GPULang::FixedString& currentFilePath, const FixedArray<std::string_view>& searchPaths, const PinnedSet<TransientString>& resolvedPaths) -> TransientString
{
    struct stat sb;
    if (stat(path.data(), &sb) == 0)
    {
        return TransientString(path);
    }
    else
    {
        // First check the folder the file is currently in
        const char* lastSlash = strrchr(currentFilePath.c_str(), '/');
        if (lastSlash != nullptr)
        {
            std::string_view fileFolder(currentFilePath.c_str(), lastSlash + 1);
            TStr fullPath = TStr::Compact(fileFolder, path);            
            struct stat sb;
            if (stat(fullPath.Data(), &sb) == 0)
            {
                return fullPath;
            }
        }
        
        // If the file still isn't found, try the search paths
        for (auto& searchPath : searchPaths)
        {
            TStr fullPath = TStr::Compact(searchPath, path);
            auto it = resolvedPaths.Find(fullPath);
            struct stat sb;
            if (stat(fullPath.Data(), &sb) == 0)
            {
                return fullPath;
            }
        }
    }

    return TransientString();
};

//------------------------------------------------------------------------------
/**
 */
void
Tokenize(const GPULangFile* file, const TransientArray<std::string_view>& searchPaths, TokenizationResult& ret, bool captureComments)
{
    const char* it = file->contents;
    const char* end = file->contents + file->contentSize;
    const char* tokenStart = nullptr;
    const char* startOfLine = nullptr;
    TokenType tokenType = TokenType::InvalidToken;
    uint32_t line = 0;
    uint32_t charPos = 0;
    
    TransientString currentPath = file->path;
    
    bool ignore = false;
    const char* commentBegin = nullptr;
    
    startOfLine = it;
    while (it != end)
    {
        const char* prev = it;
        
        it = skipWS(it, end);
        
        // Comments
        if (it[0] == '/' && it[1] == '*')
        {
            ignore = true;
            it += 2;
            commentBegin = it;
            continue;
        }
        else if (ignore)
        {
            const char* begin = it;
            const char* eol = lineEnd(it, end);
            while (it != end)
            {
                if (it == eol)
                {
                    ret.lineCount++;
                    line++;
                    break;
                }
                if (it[0] == '*' && it[1] == '/')
                {
                    ignore = false;
                    if (captureComments)
                    {
                        std::string_view commentText(commentBegin, it);
                        ret.tokenTypes.Append(TokenType::Comment);
                        Token newToken;
                        newToken.path = currentPath;
                        newToken.text = commentText;
                        ret.tokens.Append(newToken);
                    }
                    it += 2;
                    break;
                }
                it++;
            }
            continue;
        }
        else if (it[0] == '/' && it[1] == '/') // If single line comment, move iterator to end
        {
            const char* eol = lineEnd(it+2, end);
            std::string_view commentText(it+2, eol);
            ret.tokenTypes.Append(TokenType::Comment);
            Token newToken;
            newToken.path = currentPath;
            newToken.text = commentText;
            newToken.startLine = line;
            newToken.endLine = line;
            newToken.startChar = it - startOfLine + 2;
            newToken.endChar = eol - startOfLine;
            ret.tokens.Append(newToken);
            
            it = eol;
            startOfLine = it;
            line++;
            ret.lineCount++;
            continue;
        }
        
        
        // End early
        if (it[0] == '\0')
        {
            Token newToken;
            ret.tokens.Append(newToken);
            ret.tokenTypes.Append(TokenType::End);
            return;
        }
        
        if (it[0] == '\r')
        {
            if (it[1] == '\n')
            {
                line += 1;
                it += 2;
                startOfLine = it;
                ret.lineCount++;
                continue;
            }
            else
            {
                line += 1;
                it += 1;
                startOfLine = it;
                ret.lineCount++;
                continue;
            }
        }
        else if (it[0] == '\n')
        {
            line += 1;
            it += 1;
            startOfLine = it;
            ret.lineCount++;
            continue;
        }
        else if (it[0] == '\0')
            break; // EOF
        
        // Capture alpha numeric word, can be an actual identifier or any built in multi character word
        if (identifierStart(it[0]))
        {
            const char* begin = it;
            it = identifierEnd(it+1, end);
            Token newToken;
            newToken.path = currentPath;
            newToken.text = std::string_view(begin, it-begin);
            TokenType type = TokenType::Identifier;
            auto it1 = HardCodedTokens.Find(HashString(newToken.text));
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
                begin += 2; // Skip the 0x bit
                it = hexEnd(it+2, end);
                
                // Unsigned hex
                if (it[0] == 'u' || it[0] == 'U')
                {
                    type = TokenType::UnsignedHex;
                    it++;
                }
            }
            else
            {
                it = numberEnd(it+1, end);
                
                // Decimal point
                if (it[0] == '.')
                {
                    if (type == TokenType::Hex)
                    {
                        GPULangDiagnostic diagnostic;
                        diagnostic.file = currentPath;
                        diagnostic.error = "Incorrectly formatted hex number";
                        diagnostic.line = line;
                        diagnostic.column = it - startOfLine;
                        diagnostic.length = 1;
                        ret.errors.Append(diagnostic);
                    }
                    it++;
                    if (numberStart(it[0]))
                    {
                        type = TokenType::Double;
                        it = numberEnd(it+1, end);
                    }
                }

                    
                // Exponent
                if (it[0] == 'e' || it[0] == 'E')
                {
                    type = TokenType::Double;
                    it++;
                    if (it[0] == '-' || it[0] == '+')
                        it++;
                    if (!numberStart(it[0]))
                    {
                        GPULangDiagnostic diagnostic;
                        diagnostic.file = currentPath;
                        diagnostic.error = "Incorrectly formatted exponent";
                        diagnostic.line = line;
                        diagnostic.column = it - startOfLine;
                        diagnostic.length = 1;
                        ret.errors.Append(diagnostic);
                    }
                    else
                    {
                        it = numberEnd(it+1, end);
                    }
                }
                
                // Floating point (not double)
                if (it[0] == 'f' || it[0] == 'F')
                {
                    type = TokenType::Float;
                    it++;
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
                GPULangDiagnostic diagnostic;
                diagnostic.file = currentPath;
                diagnostic.error = "String not closed";
                diagnostic.line = line;
                diagnostic.column = it - startOfLine;
                diagnostic.length = 1;
                ret.errors.Append(diagnostic);
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
            // Handle include/line directives
            if (it[0] == '#')
            {
                it = skipWS(it+1, end);
                if (identifierStart(it[0]))
                {
                    const char* begin = it;
                    it = identifierEnd(it+1, end);
                    
                    std::string_view dir(begin, it);
                    if (dir == "include")
                    {
                        it = skipWS(it, end);
                        if (it[0] == '\"' || it[0] == '<')
                        {
                            it++;
                            if (pathStart(it[0]))
                            {
                                const char* begin = it;
                                it = pathEnd(it, end);
                                
                                std::string_view path(begin, it);
                                TransientString resolvedPath = resolvePath(path, file->path, searchPaths, ret.resolvedPaths);
                                
                                // Bad include
                                if (resolvedPath.size == 0)
                                {
                                    GPULangDiagnostic diagnostic;
                                    diagnostic.file = currentPath;
                                    diagnostic.error = TStr("File cannot be found: ", path);
                                    diagnostic.line = line;
                                    diagnostic.column = it - startOfLine;
                                    diagnostic.length = 1;
                                    ret.errors.Append(diagnostic);
                                    return;
                                }
                                
                                // If resolved paths is not in set, it means file was never opened
                                if (ret.resolvedPaths.Find(resolvedPath) == ret.resolvedPaths.end())
                                {
                                    // Add file to list of files that have been included
                                    ret.resolvedPaths.Insert(resolvedPath);
                                    
                                    GPULangFile* file = GPULangLoadFile(resolvedPath.ToView());
                                    ret.files.Append(file);
                                    
                                    // Tokenize included file
                                    Tokenize(file, searchPaths, ret, captureComments);
                                    
                                    // Remove previous files END token
                                    ret.tokens.size--;
                                    ret.tokenTypes.size--;
                                }

                                
                                // Move past either " or >
                                it++;
                            }
                            else
                            {
                                GPULangDiagnostic diagnostic;
                                diagnostic.file = currentPath;
                                diagnostic.error = TStr("Include directive file path missing");
                                diagnostic.line = line;
                                diagnostic.column = it - startOfLine;
                                diagnostic.length = 1;
                                ret.errors.Append(diagnostic);
                            }
                        }
                        continue;
                    }
                    else
                    {
                        GPULangDiagnostic diagnostic;
                        diagnostic.file = currentPath;
                        diagnostic.error = TStr("Invalid directive ", dir);
                        diagnostic.line = line;
                        diagnostic.column = it - startOfLine;
                        diagnostic.length = 1;
                        ret.errors.Append(diagnostic);
                    }
                }
                else
                {
                    GPULangDiagnostic diagnostic;
                    diagnostic.file = currentPath;
                    diagnostic.error = TStr("Invalid directive ", it[0]);
                    diagnostic.line = line;
                    diagnostic.column = it - startOfLine;
                    diagnostic.length = 1;
                    ret.errors.Append(diagnostic);
                }
                line--;
                continue; // Ignore the directive
            }
            
            const char* begin = it;
            if (operatorStart(it[0]))
            {
                it = operatorEnd(it+1, end);
            }
            else
            {
                it++;
            }
            std::string_view tokenText(begin, it);
            auto it1 = HardCodedTokens.Find(HashString(tokenText));
            if (it1 == HardCodedTokens.end())
            {
                GPULangDiagnostic diagnostic;
                diagnostic.file = currentPath;
                diagnostic.error = TStr("Invalid character sequence ", tokenText);
                diagnostic.line = line;
                diagnostic.column = it - startOfLine;
                diagnostic.length = it - begin;
                ret.errors.Append(diagnostic);
            }
            Token newToken;
            newToken.path = currentPath;
            newToken.text = tokenText;
            newToken.startLine = line;
            newToken.endLine = line;
            newToken.startChar = begin - startOfLine;
            newToken.endChar = it - startOfLine;
            ret.tokens.Append(newToken);
            ret.tokenTypes.Append(it1->second);
            
            continue;
        }
    }
    
    // Add end of line
    Token newToken;
    ret.tokens.Append(newToken);
    ret.tokenTypes.Append(TokenType::End);
    return;
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
GPULangDiagnostic
UnexpectedToken(const TokenStream& stream, const char* expected)
{
    const Token& tok = stream.Data(0);
    
    GPULangDiagnostic diagnostic;
    diagnostic.severity = GPULangDiagnostic::Severity::Error;
    diagnostic.error = TStr("Expected ", expected);
    diagnostic.file = tok.path;
    diagnostic.line = tok.startLine;
    diagnostic.column = tok.startChar;
    diagnostic.length = tok.endChar - tok.startChar;
    return diagnostic;
}

//------------------------------------------------------------------------------
/**
 */
GPULangDiagnostic
UnexpectedEndToken(const TokenStream& stream, const char* expected)
{
    const Token& tok = stream.Data(0);
    
    GPULangDiagnostic diagnostic;
    diagnostic.severity = GPULangDiagnostic::Severity::Error;
    diagnostic.error = TStr("Expected ", expected, " got ", tok.text);
    diagnostic.file = tok.path;
    diagnostic.line = tok.startLine;
    diagnostic.column = tok.startChar;
    diagnostic.length = tok.endChar - tok.startChar;
    return diagnostic;
}

//------------------------------------------------------------------------------
/**
 */
GPULangDiagnostic
Unsupported(const TokenStream& stream, const char* unexpected, const char* symbol)
{
    const Token& tok = stream.Data(0);
    
    GPULangDiagnostic diagnostic;
    diagnostic.severity = GPULangDiagnostic::Severity::Error;
    diagnostic.error = TStr("%s not supported for %s", unexpected, symbol);
    diagnostic.file = tok.path;
    diagnostic.line = tok.startLine;
    diagnostic.column = tok.startChar;
    diagnostic.length = tok.endChar - tok.startChar;
    return diagnostic;
}

//------------------------------------------------------------------------------
/**
 */
GPULangDiagnostic
Limit(const TokenStream& stream, const char* name, size_t size)
{
    const Token& tok = stream.Data(0);

    GPULangDiagnostic diagnostic;
    diagnostic.severity = GPULangDiagnostic::Severity::Error;
    diagnostic.error = TStr("Limit %s(%d) reached", name, size);
    diagnostic.file = tok.path;
    diagnostic.line = tok.startLine;
    diagnostic.column = tok.startChar;
    diagnostic.length = tok.endChar - tok.startChar;
    return diagnostic;
}

Expression* ParseExpression2(TokenStream&, ParseResult&, bool);
//------------------------------------------------------------------------------
/**
 */
FixedArray<Expression*>
ParseExpressionList2(TokenStream& stream, ParseResult& ret)
{
    TransientArray<Expression*> expressions(256);
    while (Expression* expr = ParseExpression2(stream, ret, true))
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

static const uint32_t TOKEN_ASSIGNMENT_OPERATOR_BIT = 0x1;
static const uint32_t TOKEN_EQUALITY_OPERATOR_BIT = 0x2;
static const uint32_t TOKEN_COMPARISON_OPERATOR_BIT = 0x4;
static const uint32_t TOKEN_SHIFT_OPERATOR_BIT = 0x8;
static const uint32_t TOKEN_ADD_SUB_OPERATOR_BIT = 0x10;
static const uint32_t TOKEN_MUL_DIV_MOD_OPERATOR_BIT = 0x20;
static const uint32_t TOKEN_PREFIX_OPERATOR_BIT = 0x40;
static const uint32_t TOKEN_INCREMENT_DECREMENT_OPERATOR_BIT = 0x80;
static const uint32_t TOKEN_FUNCTION_ATTRIBUTE_BIT = 0x100;
static const uint32_t TOKEN_PARAMETER_ATTRIBUTE_BIT = 0x200;
static const uint32_t TOKEN_VARIABLE_ATTRIBUTE_BIT = 0x400;
static const uint32_t TOKEN_VARIABLE_STORAGE_BIT = 0x800;
static const uint32_t TOKEN_PARAMETER_STORAGE_BIT = 0x1000;
static const uint32_t TOKEN_OPERATOR_BIT = 0x2000;
uint32_t TokenClassTable[(uint32_t)TokenType::NumTokenTypes];

uint32_t PrefixAssociativityTable[(uint32_t)TokenType::NumTokenTypes];
uint32_t PostfixAssociativityTable[(uint32_t)TokenType::NumTokenTypes];
uint32_t PrefixPrecedenceTable[(uint32_t)TokenType::NumTokenTypes];
uint32_t PostfixPrecedenceTable[(uint32_t)TokenType::NumTokenTypes];
static const uint32_t ASSOC_LEFT = 0;
static const uint32_t ASSOC_RIGHT = 1;

static void SetupTokenClassTable()
{
    memset(TokenClassTable, 0x0, sizeof(TokenClassTable));
    TokenClassTable[(uint32_t)TokenType::AddAssign] |= TOKEN_ASSIGNMENT_OPERATOR_BIT | TOKEN_OPERATOR_BIT;
    TokenClassTable[(uint32_t)TokenType::SubAssign] |= TOKEN_ASSIGNMENT_OPERATOR_BIT| TOKEN_OPERATOR_BIT;
    TokenClassTable[(uint32_t)TokenType::MulAssign] |= TOKEN_ASSIGNMENT_OPERATOR_BIT| TOKEN_OPERATOR_BIT;
    TokenClassTable[(uint32_t)TokenType::DivAssign] |= TOKEN_ASSIGNMENT_OPERATOR_BIT| TOKEN_OPERATOR_BIT;
    TokenClassTable[(uint32_t)TokenType::ModAssign] |= TOKEN_ASSIGNMENT_OPERATOR_BIT| TOKEN_OPERATOR_BIT;
    TokenClassTable[(uint32_t)TokenType::LeftShiftAssign] |= TOKEN_ASSIGNMENT_OPERATOR_BIT| TOKEN_OPERATOR_BIT;
    TokenClassTable[(uint32_t)TokenType::RightShiftAssign] |= TOKEN_ASSIGNMENT_OPERATOR_BIT| TOKEN_OPERATOR_BIT;
    TokenClassTable[(uint32_t)TokenType::AndAssign] |= TOKEN_ASSIGNMENT_OPERATOR_BIT| TOKEN_OPERATOR_BIT;
    TokenClassTable[(uint32_t)TokenType::OrAssign] |= TOKEN_ASSIGNMENT_OPERATOR_BIT| TOKEN_OPERATOR_BIT;
    TokenClassTable[(uint32_t)TokenType::XorAssign] |= TOKEN_ASSIGNMENT_OPERATOR_BIT| TOKEN_OPERATOR_BIT;
    TokenClassTable[(uint32_t)TokenType::Assign] |= TOKEN_ASSIGNMENT_OPERATOR_BIT| TOKEN_OPERATOR_BIT;
    
    TokenClassTable[(uint32_t)TokenType::Equal] |= TOKEN_EQUALITY_OPERATOR_BIT| TOKEN_OPERATOR_BIT;
    TokenClassTable[(uint32_t)TokenType::NotEqual] |= TOKEN_EQUALITY_OPERATOR_BIT| TOKEN_OPERATOR_BIT;
    
    TokenClassTable[(uint32_t)TokenType::LeftAngle] |= TOKEN_COMPARISON_OPERATOR_BIT| TOKEN_OPERATOR_BIT;
    TokenClassTable[(uint32_t)TokenType::LessThanEqual] |= TOKEN_COMPARISON_OPERATOR_BIT| TOKEN_OPERATOR_BIT;
    TokenClassTable[(uint32_t)TokenType::RightAngle] |= TOKEN_COMPARISON_OPERATOR_BIT| TOKEN_OPERATOR_BIT;
    TokenClassTable[(uint32_t)TokenType::GreaterThanEqual] |= TOKEN_COMPARISON_OPERATOR_BIT| TOKEN_OPERATOR_BIT;
    
    TokenClassTable[(uint32_t)TokenType::LeftShift] |= TOKEN_SHIFT_OPERATOR_BIT| TOKEN_OPERATOR_BIT;
    TokenClassTable[(uint32_t)TokenType::RightShift] |= TOKEN_SHIFT_OPERATOR_BIT| TOKEN_OPERATOR_BIT;
    
    TokenClassTable[(uint32_t)TokenType::Add] |= TOKEN_ADD_SUB_OPERATOR_BIT| TOKEN_OPERATOR_BIT;
    TokenClassTable[(uint32_t)TokenType::Sub] |= TOKEN_ADD_SUB_OPERATOR_BIT| TOKEN_OPERATOR_BIT;
    
    TokenClassTable[(uint32_t)TokenType::Mul] |= TOKEN_MUL_DIV_MOD_OPERATOR_BIT| TOKEN_OPERATOR_BIT;
    TokenClassTable[(uint32_t)TokenType::Div] |= TOKEN_MUL_DIV_MOD_OPERATOR_BIT| TOKEN_OPERATOR_BIT;
    TokenClassTable[(uint32_t)TokenType::Mod] |= TOKEN_MUL_DIV_MOD_OPERATOR_BIT| TOKEN_OPERATOR_BIT;
    
    TokenClassTable[(uint32_t)TokenType::Sub] |= TOKEN_PREFIX_OPERATOR_BIT| TOKEN_OPERATOR_BIT;
    TokenClassTable[(uint32_t)TokenType::Add] |= TOKEN_PREFIX_OPERATOR_BIT| TOKEN_OPERATOR_BIT;
    TokenClassTable[(uint32_t)TokenType::Not] |= TOKEN_PREFIX_OPERATOR_BIT| TOKEN_OPERATOR_BIT;
    TokenClassTable[(uint32_t)TokenType::Complement] |= TOKEN_PREFIX_OPERATOR_BIT| TOKEN_OPERATOR_BIT;
    TokenClassTable[(uint32_t)TokenType::Increment] |= TOKEN_PREFIX_OPERATOR_BIT | TOKEN_INCREMENT_DECREMENT_OPERATOR_BIT| TOKEN_OPERATOR_BIT;
    TokenClassTable[(uint32_t)TokenType::Decrement] |= TOKEN_PREFIX_OPERATOR_BIT | TOKEN_INCREMENT_DECREMENT_OPERATOR_BIT| TOKEN_OPERATOR_BIT;
    TokenClassTable[(uint32_t)TokenType::Mul] |= TOKEN_PREFIX_OPERATOR_BIT| TOKEN_OPERATOR_BIT;
    TokenClassTable[(uint32_t)TokenType::And] |= TOKEN_PREFIX_OPERATOR_BIT| TOKEN_OPERATOR_BIT;
    
    TokenClassTable[(uint32_t)TokenType::LogicalOr] |= TOKEN_OPERATOR_BIT;
    TokenClassTable[(uint32_t)TokenType::LogicalAnd] |= TOKEN_OPERATOR_BIT;
    TokenClassTable[(uint32_t)TokenType::Or] |= TOKEN_OPERATOR_BIT;
    TokenClassTable[(uint32_t)TokenType::Xor] |= TOKEN_OPERATOR_BIT;
    TokenClassTable[(uint32_t)TokenType::And] |= TOKEN_OPERATOR_BIT;
    TokenClassTable[(uint32_t)TokenType::Dot] |= TOKEN_OPERATOR_BIT;
    //TokenClassTable[(uint32_t)TokenType::Comma] |= TOKEN_OPERATOR_BIT;
    
    TokenClassTable[(uint32_t)TokenType::EntryPoint_Attribute] |= TOKEN_FUNCTION_ATTRIBUTE_BIT;
    TokenClassTable[(uint32_t)TokenType::Threads_Attribute] |= TOKEN_FUNCTION_ATTRIBUTE_BIT;
    TokenClassTable[(uint32_t)TokenType::ThreadsX_Attribute] |= TOKEN_FUNCTION_ATTRIBUTE_BIT;
    TokenClassTable[(uint32_t)TokenType::ThreadsY_Attribute] |= TOKEN_FUNCTION_ATTRIBUTE_BIT;
    TokenClassTable[(uint32_t)TokenType::ThreadsZ_Attribute] |= TOKEN_FUNCTION_ATTRIBUTE_BIT;
    TokenClassTable[(uint32_t)TokenType::EarlyDepth_Attribute] |= TOKEN_FUNCTION_ATTRIBUTE_BIT;
    TokenClassTable[(uint32_t)TokenType::DepthLesser_Attribute] |= TOKEN_FUNCTION_ATTRIBUTE_BIT;
    TokenClassTable[(uint32_t)TokenType::DepthGreater_Attribute] |= TOKEN_FUNCTION_ATTRIBUTE_BIT;
    TokenClassTable[(uint32_t)TokenType::SubgroupSize_Attribute] |= TOKEN_FUNCTION_ATTRIBUTE_BIT;
    TokenClassTable[(uint32_t)TokenType::SubgroupsPerWorkgroup_Attribute] |= TOKEN_FUNCTION_ATTRIBUTE_BIT;
    TokenClassTable[(uint32_t)TokenType::InputVertices_Attribute] |= TOKEN_FUNCTION_ATTRIBUTE_BIT;
    TokenClassTable[(uint32_t)TokenType::MaxOutputVertices_Attribute] |= TOKEN_FUNCTION_ATTRIBUTE_BIT;
    TokenClassTable[(uint32_t)TokenType::Winding_Attribute] |= TOKEN_FUNCTION_ATTRIBUTE_BIT;
    TokenClassTable[(uint32_t)TokenType::InputTopology_Attribute] |= TOKEN_FUNCTION_ATTRIBUTE_BIT;
    TokenClassTable[(uint32_t)TokenType::OutputTopology_Attribute] |= TOKEN_FUNCTION_ATTRIBUTE_BIT;
    TokenClassTable[(uint32_t)TokenType::PatchType_Attribute] |= TOKEN_FUNCTION_ATTRIBUTE_BIT;
    TokenClassTable[(uint32_t)TokenType::Partition_Attribute] |= TOKEN_FUNCTION_ATTRIBUTE_BIT;
    TokenClassTable[(uint32_t)TokenType::PixelOrigin_Attribute] |= TOKEN_FUNCTION_ATTRIBUTE_BIT;
    TokenClassTable[(uint32_t)TokenType::DerivativeIndexLinear_Attribute] |= TOKEN_FUNCTION_ATTRIBUTE_BIT;
    TokenClassTable[(uint32_t)TokenType::DerivativeIndexQuad_Attribute] |= TOKEN_FUNCTION_ATTRIBUTE_BIT;
    
    TokenClassTable[(uint32_t)TokenType::Binding_Decorator] |= TOKEN_VARIABLE_ATTRIBUTE_BIT;
    TokenClassTable[(uint32_t)TokenType::Group_Decorator] |= TOKEN_VARIABLE_ATTRIBUTE_BIT;
    TokenClassTable[(uint32_t)TokenType::Volatile_Decorator] |= TOKEN_VARIABLE_ATTRIBUTE_BIT;
    TokenClassTable[(uint32_t)TokenType::Atomic_Decorator] |= TOKEN_VARIABLE_ATTRIBUTE_BIT;
    TokenClassTable[(uint32_t)TokenType::NoRead_Decorator] |= TOKEN_VARIABLE_ATTRIBUTE_BIT;
    TokenClassTable[(uint32_t)TokenType::NonTemporal_Decorator] |= TOKEN_VARIABLE_ATTRIBUTE_BIT;
    
    TokenClassTable[(uint32_t)TokenType::In_Storage] |= TOKEN_PARAMETER_ATTRIBUTE_BIT;
    TokenClassTable[(uint32_t)TokenType::Out_Storage] |= TOKEN_PARAMETER_ATTRIBUTE_BIT;
    TokenClassTable[(uint32_t)TokenType::Uniform_Storage] |= TOKEN_PARAMETER_ATTRIBUTE_BIT;
    TokenClassTable[(uint32_t)TokenType::Workgroup_Storage] |= TOKEN_PARAMETER_ATTRIBUTE_BIT;
    TokenClassTable[(uint32_t)TokenType::RayPayload_Storage] |= TOKEN_PARAMETER_ATTRIBUTE_BIT;
    TokenClassTable[(uint32_t)TokenType::CallableData_Storage] |= TOKEN_PARAMETER_ATTRIBUTE_BIT;
    TokenClassTable[(uint32_t)TokenType::RayHitAttribute_Storage] |= TOKEN_PARAMETER_ATTRIBUTE_BIT;
    TokenClassTable[(uint32_t)TokenType::NoInterpolate_Modifier] |= TOKEN_PARAMETER_ATTRIBUTE_BIT;
    TokenClassTable[(uint32_t)TokenType::NoPerspective_Modifier] |= TOKEN_PARAMETER_ATTRIBUTE_BIT;
    TokenClassTable[(uint32_t)TokenType::Patch_Domain] |= TOKEN_PARAMETER_ATTRIBUTE_BIT;
    TokenClassTable[(uint32_t)TokenType::Binding_Decorator] |= TOKEN_PARAMETER_ATTRIBUTE_BIT;
    
    TokenClassTable[(uint32_t)TokenType::Const_Storage] |= TOKEN_VARIABLE_STORAGE_BIT | TOKEN_PARAMETER_STORAGE_BIT;
    TokenClassTable[(uint32_t)TokenType::Var_Storage] |= TOKEN_VARIABLE_STORAGE_BIT | TOKEN_PARAMETER_STORAGE_BIT;
    TokenClassTable[(uint32_t)TokenType::Uniform_Storage] |= TOKEN_VARIABLE_STORAGE_BIT;
    TokenClassTable[(uint32_t)TokenType::Workgroup_Storage] |= TOKEN_VARIABLE_STORAGE_BIT;
    TokenClassTable[(uint32_t)TokenType::Inline_Storage] |= TOKEN_VARIABLE_STORAGE_BIT;
    TokenClassTable[(uint32_t)TokenType::LinkDefined_Storage] |= TOKEN_VARIABLE_STORAGE_BIT;
    
    memset(PrefixAssociativityTable, 0x0, sizeof(PrefixAssociativityTable));
    memset(PostfixAssociativityTable, 0x0, sizeof(PostfixAssociativityTable));
    PrefixAssociativityTable[(uint32_t)TokenType::Add] = ASSOC_RIGHT;
    PrefixAssociativityTable[(uint32_t)TokenType::Mul] = ASSOC_RIGHT;
    PrefixAssociativityTable[(uint32_t)TokenType::Sub] = ASSOC_RIGHT;
    PrefixAssociativityTable[(uint32_t)TokenType::Not] = ASSOC_RIGHT;
    PrefixAssociativityTable[(uint32_t)TokenType::Complement] = ASSOC_RIGHT;
    PrefixAssociativityTable[(uint32_t)TokenType::Increment] = ASSOC_RIGHT;
    PrefixAssociativityTable[(uint32_t)TokenType::Decrement] = ASSOC_RIGHT;
    PrefixAssociativityTable[(uint32_t)TokenType::And] = ASSOC_RIGHT;
    
    PostfixAssociativityTable[(uint32_t)TokenType::Increment] = ASSOC_LEFT;
    PostfixAssociativityTable[(uint32_t)TokenType::Decrement] = ASSOC_LEFT;
    PostfixAssociativityTable[(uint32_t)TokenType::Add] = ASSOC_LEFT;
    PostfixAssociativityTable[(uint32_t)TokenType::Sub] = ASSOC_LEFT;
    PostfixAssociativityTable[(uint32_t)TokenType::Mul] = ASSOC_LEFT;
    PostfixAssociativityTable[(uint32_t)TokenType::Div] = ASSOC_LEFT;
    PostfixAssociativityTable[(uint32_t)TokenType::Mod] = ASSOC_LEFT;
    PostfixAssociativityTable[(uint32_t)TokenType::Increment] = ASSOC_LEFT;
    PostfixAssociativityTable[(uint32_t)TokenType::Decrement] = ASSOC_LEFT;
    PostfixAssociativityTable[(uint32_t)TokenType::And] = ASSOC_LEFT;
    PostfixAssociativityTable[(uint32_t)TokenType::Or] = ASSOC_LEFT;
    PostfixAssociativityTable[(uint32_t)TokenType::Xor] = ASSOC_LEFT;
    PostfixAssociativityTable[(uint32_t)TokenType::AddAssign] = ASSOC_RIGHT;
    PostfixAssociativityTable[(uint32_t)TokenType::SubAssign] = ASSOC_RIGHT;
    PostfixAssociativityTable[(uint32_t)TokenType::MulAssign] = ASSOC_RIGHT;
    PostfixAssociativityTable[(uint32_t)TokenType::DivAssign] = ASSOC_RIGHT;
    PostfixAssociativityTable[(uint32_t)TokenType::ModAssign] = ASSOC_RIGHT;
    PostfixAssociativityTable[(uint32_t)TokenType::AndAssign] = ASSOC_RIGHT;
    PostfixAssociativityTable[(uint32_t)TokenType::OrAssign] = ASSOC_RIGHT;
    PostfixAssociativityTable[(uint32_t)TokenType::XorAssign] = ASSOC_RIGHT;
    PostfixAssociativityTable[(uint32_t)TokenType::Question] = ASSOC_RIGHT;
    PostfixAssociativityTable[(uint32_t)TokenType::Comma] = ASSOC_LEFT;
    
    memset(PostfixPrecedenceTable, 0x0, sizeof(PostfixPrecedenceTable));
    PostfixPrecedenceTable[(uint32_t)TokenType::Increment] = 2;
    PostfixPrecedenceTable[(uint32_t)TokenType::Decrement] = 2;
    PostfixPrecedenceTable[(uint32_t)TokenType::LeftBracket] = 0xFFFFFFFF;
    PostfixPrecedenceTable[(uint32_t)TokenType::RightBracket] = 0xFFFFFFFF;
    PostfixPrecedenceTable[(uint32_t)TokenType::Subscript] = 0xFFFFFFFF;
    PostfixPrecedenceTable[(uint32_t)TokenType::LeftParant] = 0xFFFFFFFF;
    PostfixPrecedenceTable[(uint32_t)TokenType::RightParant] = 0xFFFFFFFF;
    PostfixPrecedenceTable[(uint32_t)TokenType::Dot] = 2;
    PostfixPrecedenceTable[(uint32_t)TokenType::PrefixAdd] = 3;
    PostfixPrecedenceTable[(uint32_t)TokenType::PrefixSub] = 3;
    PostfixPrecedenceTable[(uint32_t)TokenType::PrefixMul] = 3;
    PostfixPrecedenceTable[(uint32_t)TokenType::PrefixAnd] = 3;
    PostfixPrecedenceTable[(uint32_t)TokenType::PrefixIncrement] = 3;
    PostfixPrecedenceTable[(uint32_t)TokenType::PrefixDecrement] = 3;
    PostfixPrecedenceTable[(uint32_t)TokenType::Not] = 3;
    PostfixPrecedenceTable[(uint32_t)TokenType::Mul] = 5;
    PostfixPrecedenceTable[(uint32_t)TokenType::Div] = 5;
    PostfixPrecedenceTable[(uint32_t)TokenType::Mod] = 5;
    PostfixPrecedenceTable[(uint32_t)TokenType::Add] = 6;
    PostfixPrecedenceTable[(uint32_t)TokenType::Sub] = 6;
    PostfixPrecedenceTable[(uint32_t)TokenType::LeftShift] = 7;
    PostfixPrecedenceTable[(uint32_t)TokenType::RightShift] = 7;
    PostfixPrecedenceTable[(uint32_t)TokenType::LeftAngle] = 9;
    PostfixPrecedenceTable[(uint32_t)TokenType::LessThanEqual] = 9;
    PostfixPrecedenceTable[(uint32_t)TokenType::RightAngle] = 9;
    PostfixPrecedenceTable[(uint32_t)TokenType::GreaterThanEqual] = 9;
    PostfixPrecedenceTable[(uint32_t)TokenType::Equal] = 10;
    PostfixPrecedenceTable[(uint32_t)TokenType::NotEqual] = 10;
    PostfixPrecedenceTable[(uint32_t)TokenType::And] = 11;
    PostfixPrecedenceTable[(uint32_t)TokenType::Xor] = 12;
    PostfixPrecedenceTable[(uint32_t)TokenType::Or] = 13;
    PostfixPrecedenceTable[(uint32_t)TokenType::LogicalAnd] = 14;
    PostfixPrecedenceTable[(uint32_t)TokenType::LogicalOr] = 15;
    PostfixPrecedenceTable[(uint32_t)TokenType::Assign] = 16;
    PostfixPrecedenceTable[(uint32_t)TokenType::AddAssign] = 16;
    PostfixPrecedenceTable[(uint32_t)TokenType::SubAssign] = 16;
    PostfixPrecedenceTable[(uint32_t)TokenType::MulAssign] = 16;
    PostfixPrecedenceTable[(uint32_t)TokenType::DivAssign] = 16;
    PostfixPrecedenceTable[(uint32_t)TokenType::ModAssign] = 16;
    PostfixPrecedenceTable[(uint32_t)TokenType::LeftShiftAssign] = 16;
    PostfixPrecedenceTable[(uint32_t)TokenType::RightShiftAssign] = 16;
    PostfixPrecedenceTable[(uint32_t)TokenType::AndAssign] = 16;
    PostfixPrecedenceTable[(uint32_t)TokenType::OrAssign] = 16;
    PostfixPrecedenceTable[(uint32_t)TokenType::XorAssign] = 16;
    PostfixPrecedenceTable[(uint32_t)TokenType::Comma] = 0xFFFFFFFF;
    PostfixPrecedenceTable[(uint32_t)TokenType::ArrayInitializer] = 0xFFFFFFFF;
    PostfixPrecedenceTable[(uint32_t)TokenType::Call] = 0xFFFFFFFF;

    
    memset(PrefixPrecedenceTable, 0x0, sizeof(PrefixPrecedenceTable));
    PrefixPrecedenceTable[(uint32_t)TokenType::Add] = 3;
    PrefixPrecedenceTable[(uint32_t)TokenType::Sub] = 3;
    PrefixPrecedenceTable[(uint32_t)TokenType::Mul] = 3;
    PrefixPrecedenceTable[(uint32_t)TokenType::Increment] = 3;
    PrefixPrecedenceTable[(uint32_t)TokenType::Decrement] = 3;
    PrefixPrecedenceTable[(uint32_t)TokenType::Complement] = 3;
    PrefixPrecedenceTable[(uint32_t)TokenType::Not] = 3;
}


//------------------------------------------------------------------------------
/**
 */
Expression*
ParseExpression2(TokenStream& stream, ParseResult& ret, bool stopAtComma = false)
{
    static const uint8_t BINARY_ARITY = 0;
    static const uint8_t UNARY_ARITY = 1;
    static const uint8_t TERNARY_ARITY = 2;
    struct Operator
    {
        TokenType type;
        uint32_t fourcc = 0xFFFFFFFF;
        uint8_t prefix = 0;
        uint8_t arity = BINARY_ARITY; // 0 = binary, 1 = unary, 2 = ternary
        uint16_t operandDepth = 0xFFFFFFFF;
        const Token* token = nullptr;
    };
    
    static const auto reduceTop = [](TransientArray<Operator>& operatorStack, TransientArray<Expression*>& operandStack) -> void
    {
        const Operator& tok = operatorStack.back(); operatorStack.size--;
        if (tok.arity == UNARY_ARITY)
        {
            Expression* rhs = operandStack.back(); operandStack.size--;
            Expression* expr = Alloc<UnaryExpression>(tok.fourcc, tok.prefix, rhs);
            expr->location = LocationFromToken(*tok.token);
            operandStack.Append(expr);
        }
        else if (tok.arity == BINARY_ARITY)
        {
            if (tok.type == TokenType::Dot)
            {
                Expression* rhs = operandStack.back(); operandStack.size--;
                Expression* lhs = operandStack.back(); operandStack.size--;
                Expression* expr = Alloc<AccessExpression>(lhs, rhs, false);
                expr->location = LocationFromToken(*tok.token);
                operandStack.Append(expr);
            }
            else
            {
                Expression* rhs = operandStack.back(); operandStack.size--;
                Expression* lhs = operandStack.back(); operandStack.size--;
                Expression* expr = Alloc<BinaryExpression>(tok.fourcc, lhs, rhs);
                expr->location = LocationFromToken(*tok.token);
                operandStack.Append(expr);
            }
        }
        else if (tok.arity == TERNARY_ARITY)
        {
            Expression* rhs = operandStack.back(); operandStack.size--;
            Expression* lhs = operandStack.back(); operandStack.size--;
            Expression* cond = operandStack.back(); operandStack.size--;
            Expression* expr = Alloc<TernaryExpression>(cond, lhs, rhs);
            expr->location = LocationFromToken(*tok.token);
            operandStack.Append(expr);
        }
    };
    
    TransientArray<Operator> operatorStack(256);
    TokenType type = stream.Type();
    TransientArray<Expression*> operandStack(256);
    TransientArray<TransientArray<Expression*>> expressionListStack(32);
    size_t paranthesisDepth = 0;
    size_t bracketDepth = 0;
    
    uint32_t* precedenceTable = PrefixPrecedenceTable;
    uint32_t* associativityTable = PrefixAssociativityTable;
    
    while (type != TokenType::End)
    {
        if (stream.Match(TokenType::Quote))
        {
            const Token& tok = stream.Data(-1);
            Expression* res = Alloc<StringExpression>(std::string(tok.text));
            res->location = LocationFromToken(tok);
            operandStack.Append(res);
            precedenceTable = PostfixPrecedenceTable;
            associativityTable = PostfixAssociativityTable;
        }
        else if (stream.Match(TokenType::Integer))
        {
            const Token& tok = stream.Data(-1);
            int value;
            std::from_chars(tok.text.data(), tok.text.data() + tok.text.size(), value);
            Expression* res = Alloc<IntExpression>(value);
            res->location = LocationFromToken(tok);
            operandStack.Append(res);
            precedenceTable = PostfixPrecedenceTable;
            associativityTable = PostfixAssociativityTable;
        }
        else if (stream.Match(TokenType::UnsignedInteger))
        {
            const Token& tok = stream.Data(-1);
            unsigned int value;
            std::from_chars(tok.text.data(), tok.text.data() + tok.text.size(), value);
            Expression* res = Alloc<UIntExpression>(value);
            res->location = LocationFromToken(tok);
            operandStack.Append(res);
            precedenceTable = PostfixPrecedenceTable;
            associativityTable = PostfixAssociativityTable;
        }
        else if (stream.Match(TokenType::Hex))
        {
            const Token& tok = stream.Data(-1);
            int value;
            std::from_chars(tok.text.data(), tok.text.data() + tok.text.size(), value, 16);
            Expression* res = Alloc<IntExpression>(value);
            res->location = LocationFromToken(tok);
            operandStack.Append(res);
            precedenceTable = PostfixPrecedenceTable;
            associativityTable = PostfixAssociativityTable;
        }
        else if (stream.Match(TokenType::UnsignedHex))
        {
            const Token& tok = stream.Data(-1);
            unsigned int value;
            std::from_chars(tok.text.data(), tok.text.data() + tok.text.size(), value, 16);
            Expression* res = Alloc<UIntExpression>(value);
            res->location = LocationFromToken(tok);
            operandStack.Append(res);
            precedenceTable = PostfixPrecedenceTable;
            associativityTable = PostfixAssociativityTable;
        }
        else if (stream.Match(TokenType::Float) || stream.Match(TokenType::Double))
        {
            const Token& tok = stream.Data(-1);
            Expression* res = Alloc<FloatExpression>(std::stof(std::string(tok.text)));
            res->location = LocationFromToken(tok);
            operandStack.Append(res);
            precedenceTable = PostfixPrecedenceTable;
            associativityTable = PostfixAssociativityTable;
        }
        else if (stream.Match(TokenType::Bool))
        {
            const Token& tok = stream.Data(-1);
            Expression* res = Alloc<BoolExpression>(tok.text == "true" ? true : false);
            res->location = LocationFromToken(tok);
            operandStack.Append(res);
            precedenceTable = PostfixPrecedenceTable;
            associativityTable = PostfixAssociativityTable;
        }
        else if (stream.Match(TokenType::Identifier))
        {
            const Token& tok = stream.Data(-1);
            Expression* res = Alloc<SymbolExpression>(FixedString(tok.text));
            res->location = LocationFromToken(tok);
            operandStack.Append(res);
            precedenceTable = PostfixPrecedenceTable;
            associativityTable = PostfixAssociativityTable;
        }
        else if (stream.Match(TokenType::Declared))
        {
            const Token& tok = stream.Data(-1);
            if (stream.Match(TokenType::LeftAngle))
            {
                if (stream.Match(TokenType::Identifier))
                {
                    const Token& tok = stream.Data(-1);
                    Expression* res = Alloc<DeclaredExpression>(FixedString(tok.text));
                    res->location = LocationFromToken(tok);
                    
                    if (!stream.Match(TokenType::RightAngle))
                    {
                        ret.errors.Append(UnexpectedToken(stream, ">"));
                        break;
                    }
                    operandStack.Append(res);
                    precedenceTable = PostfixPrecedenceTable;
                    associativityTable = PostfixAssociativityTable;
                }
                else
                {
                    ret.errors.Append(UnexpectedToken(stream, "identifier"));
                    break;
                }
            }
            else
            {
                ret.errors.Append(UnexpectedToken(stream, "<"));
                break;
            }
        }
        else if (stream.MatchClass(TOKEN_OPERATOR_BIT))
        {
            const Token* matchedTok = &stream.Data(-1);
            TokenType matchedType = stream.Type(-1);
            Operator parseTok;
            parseTok.type = matchedType;
            parseTok.fourcc = StringToFourCC(matchedTok->text);
            parseTok.token = matchedTok;
            
            // If using prefix precedence, this is a prefix unary operator
            if (precedenceTable == PrefixPrecedenceTable)
            {
                parseTok.arity = UNARY_ARITY;
                parseTok.prefix = 1;
                switch (matchedType)
                {
                    case TokenType::Add:
                        parseTok.type = TokenType::PrefixAdd;
                        break;
                    case TokenType::Sub:
                        parseTok.type = TokenType::PrefixSub;
                        break;
                    case TokenType::Increment:
                        parseTok.type = TokenType::PrefixIncrement;
                        break;
                    case TokenType::Decrement:
                        parseTok.type = TokenType::PrefixDecrement;
                        break;
                    case TokenType::Mul:
                        parseTok.type = TokenType::PrefixMul;
                        break;
                    case TokenType::And:
                        parseTok.type = TokenType::PrefixAnd;
                        break;
                    case TokenType::Not:
                    case TokenType::Complement:
                        break; // These are only allowed as unary
                    default:
                        ret.errors.Append(UnexpectedToken(stream, "valid prefix operator (+ - * ! ~ ++ --)"));
                }
            }
            else if ((TokenClassTable[(uint32_t)matchedType] & TOKEN_INCREMENT_DECREMENT_OPERATOR_BIT) == TOKEN_INCREMENT_DECREMENT_OPERATOR_BIT)
            {
                parseTok.arity = UNARY_ARITY;
                parseTok.prefix = 0;
            }
            else
            {
                // Otherwise it's a binary
                while (operatorStack.size > 0)
                {
                    const Operator& tok = operatorStack.back();
                    uint32_t p1 = precedenceTable[(uint32_t)matchedType];
                    uint32_t p2 = precedenceTable[(uint32_t)operatorStack.back().type];
                    uint32_t assoc = associativityTable[(uint32_t)matchedType];
                    
                    if ((assoc == ASSOC_LEFT && p2 <= p1) ||
                        (assoc == ASSOC_RIGHT && p2 < p1))
                    {
                        reduceTop(operatorStack, operandStack);
                    }
                    else
                        break;
                }
                precedenceTable = PrefixPrecedenceTable;
                associativityTable = PrefixAssociativityTable;
            }
            
            operatorStack.Append(parseTok);
        }
        else if (stream.Match(TokenType::LeftParant))
        {
            paranthesisDepth++;
            // No previous operand, assume '(' expr ')'
            if (precedenceTable == PrefixPrecedenceTable)
            {
                Operator parseTok;
                parseTok.type = TokenType::LeftParant;
                operatorStack.Append(parseTok);
            }
            else // Assume call
            {
                Operator parseTok;
                parseTok.type = TokenType::Call;
                parseTok.operandDepth = operandStack.size;
                operatorStack.Append(parseTok);
                expressionListStack.Append(TransientArray<Expression*>(32));
            }
            precedenceTable = PrefixPrecedenceTable;
            associativityTable = PrefixAssociativityTable;
        }
        else if (stream.Match(TokenType::RightParant))
        {
            if (paranthesisDepth == 0)
            {
                stream.Unmatch();
                break; // No matching left parant
            }
            paranthesisDepth--;
            uint8_t callOrParen = 0;

            // Reduce to '('
            while (operatorStack.size > 0)
            {
                if (operatorStack.back().type == TokenType::LeftParant)
                {
                    callOrParen = 1;
                    break;
                }
                else if (operatorStack.back().type == TokenType::Call)
                {
                    callOrParen = 0;
                    break;
                }
                reduceTop(operatorStack, operandStack);
            }
            
            const Operator& lastOp = operatorStack.back();
            if (callOrParen == 0)
            {
                if (operatorStack.size == 0 || lastOp.type != TokenType::Call)
                {
                    ret.errors.Append(UnexpectedToken(stream, ")"));
                    break;
                }
                
                uint8_t hasArguments = (operandStack.size - lastOp.operandDepth) > 0;
                Expression* name = nullptr;
                TransientArray<Expression*>& expressionList = expressionListStack.back();
                if (hasArguments)
                {
                    if (expressionList.Full())
                    {
                        ret.errors.Append(Limit(stream, "expression list", 256));
                        break;
                    }
                    Expression* arguments = operandStack.back(); operandStack.size--;
                    name = operandStack.back(); operandStack.size--;
                    expressionList.Append(arguments);
                }
                else
                {
                    name = operandStack.back(); operandStack.size--;
                }
                
                Expression* expr = Alloc<CallExpression>(name, expressionList);
                expressionListStack.Pop();
                expr->location = name->location;
                operandStack.Append(expr);
            }
            else
            {
                if (operatorStack.size == 0 || lastOp.type != TokenType::LeftParant)
                {
                    ret.errors.Append(UnexpectedToken(stream, ")"));
                    break;
                }
            }
            operatorStack.size--; // Remove the left parant
            precedenceTable = PostfixPrecedenceTable;
            associativityTable = PostfixAssociativityTable;
        }
        else if (stream.Match(TokenType::LeftBracket))
        {
            bracketDepth++;
            if (precedenceTable == PostfixPrecedenceTable)
            {
                Operator parseTok;
                parseTok.type = TokenType::Subscript;
                parseTok.operandDepth = operandStack.size;
                parseTok.token = &stream.Data(-1);
                operatorStack.Append(parseTok);
            }
            else
            {
                Operator parseTok;
                parseTok.type = TokenType::ArrayInitializer;
                parseTok.operandDepth = operandStack.size;
                parseTok.token = &stream.Data(-1);
                operatorStack.Append(parseTok);
                expressionListStack.Append(TransientArray<Expression*>(256));
            }
            precedenceTable = PrefixPrecedenceTable;
            associativityTable = PrefixAssociativityTable;
        }
        else if (stream.Match(TokenType::RightBracket))
        {
            if (bracketDepth == 0)
            {
                stream.Unmatch();
                break; // No matching left parant
            }
            
            bracketDepth--;
            
            uint8_t subscriptOrArray = 0;
            while (operatorStack.size > 0)
            {
                if (operatorStack.back().type == TokenType::Subscript)
                {
                    subscriptOrArray = 0;
                    break;
                }
                else if (operatorStack.back().type == TokenType::ArrayInitializer)
                {
                    subscriptOrArray = 1;
                    break;
                }
                
                reduceTop(operatorStack, operandStack);
            }
            
            const Operator& lastOp = operatorStack.back();
            if (subscriptOrArray == 0)
            {
                uint16_t numIndexExpressions = operandStack.size - lastOp.operandDepth;
                if (numIndexExpressions == 0)
                {
                    ret.errors.Append(UnexpectedToken(stream, "index expression"));
                    break;
                }
                Expression* index = operandStack.back(); operandStack.size--;
                
                if (operatorStack.size == 0 || lastOp.type != TokenType::Subscript)
                {
                    ret.errors.Append(UnexpectedToken(stream, "]"));
                    break;
                }
                
                Expression* target = operandStack.back(); operandStack.size--;
                Expression* expr = Alloc<ArrayIndexExpression>(target, index);
                expr->location = target->location;
                operandStack.Append(expr);
            }
            else if (subscriptOrArray == 1)
            {
                if (operatorStack.size == 0 || lastOp.type != TokenType::ArrayInitializer)
                {
                    ret.errors.Append(UnexpectedToken(stream, "]"));
                    break;
                }
                
                // Flatten comma separated binary expression to a list
                Expression* target = operandStack.back(); operandStack.size--;
                TransientArray<Expression*>& expressionList = expressionListStack.back();
                if (expressionList.Full())
                {
                    ret.errors.Append(Limit(stream, "expression list", 256));
                    break;
                }
                expressionList.Append(target);
                Expression* expr = Alloc<ArrayInitializerExpression>(expressionList);
                expressionListStack.Pop();
                operandStack.Append(expr);
            }
            operatorStack.size--; // Remove the left bracket
            precedenceTable = PostfixPrecedenceTable;
            associativityTable = PostfixAssociativityTable;
        }
        else if (stream.Match(TokenType::Question))
        {
            Operator parseTok;
            parseTok.type = TokenType::Question;
            parseTok.arity = TERNARY_ARITY;
            parseTok.token = &stream.Data(-1);
            precedenceTable = PrefixPrecedenceTable;
            associativityTable = PrefixAssociativityTable;
            operatorStack.Append(parseTok);
        }
        else if (stream.Match(TokenType::Colon))
        {
            while (operatorStack.size > 0 && operatorStack.back().type != TokenType::Question)
            {
                reduceTop(operatorStack, operandStack);
            }
            precedenceTable = PrefixPrecedenceTable;
            associativityTable = PrefixAssociativityTable;
        }
        else if (stream.Match(TokenType::Comma))
        {
            if (stopAtComma && paranthesisDepth == 0 && bracketDepth == 0)
            {
                stream.Unmatch();
                break;
            }
            // Reduce to left of comma
            // Otherwise it's a binary
            while (operatorStack.size > 0)
            {
                const Operator& tok = operatorStack.back();
                uint32_t p1 = precedenceTable[(uint32_t)operatorStack.back().type];
                
                if (p1 != 0xFFFFFFFF)
                {
                    reduceTop(operatorStack, operandStack);
                }
                else
                    break;
            }
            
            TransientArray<Expression*>& expressionList = expressionListStack.back();
            if (expressionList.Full())
            {
                ret.errors.Append(Limit(stream, "expression list", 256));
                break;
            }
            expressionList.Append(operandStack.back());
            
            operandStack.size--;
            precedenceTable = PrefixPrecedenceTable;
            associativityTable = PrefixAssociativityTable;
        }
        else
            break;
        type = stream.Type();
    }
    
    while (operatorStack.size > 0)
    {
        reduceTop(operatorStack, operandStack);
    }
    
    if (paranthesisDepth > 0)
    {
        ret.errors.Append(UnexpectedEndToken(stream, ")"));
    }
    
    if (bracketDepth > 0)
    {
        ret.errors.Append(UnexpectedEndToken(stream, "]"));
    }
    
    if (operandStack.size > 1)
    {
        ret.errors.Append(UnexpectedToken(stream, "operator"));
    }
    
    
    // Didn't match an expression
    if (operandStack.size == 0)
        return nullptr;
    return operandStack.back();
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
        if (stream.MatchClass(TOKEN_ASSIGNMENT_OPERATOR_BIT))
        {
            const Token& tok = stream.Data(-1);
            Expression* rhs = ParseExpression(stream, ret, res, 0);
            res = Alloc<BinaryExpression>(StringToFourCC(TransientString(tok.text)), res, rhs);
            res->location = LocationFromToken(tok);
        }
        else if (stream.Match(TokenType::Question))
        {
            // Right associative ternary
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
    else if (precedence == 1)
    {
        res = ParseExpression(stream, ret, res, 2);
        while (stream.Match(TokenType::LogicalOr))
        {
            const Token& tok = stream.Data(-1);
            Expression* rhs = ParseExpression(stream, ret, res, 2);
            res = Alloc<BinaryExpression>(StringToFourCC(TransientString(tok.text)), res, rhs);
            res->location = LocationFromToken(tok);

        }
    }
    else if (precedence == 2)
    {
        res = ParseExpression(stream, ret, res, 3);
        while (stream.Match(TokenType::LogicalAnd))
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
        while (stream.Match(TokenType::Or))
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
        while (stream.Match(TokenType::Xor))
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
        while (stream.Match(TokenType::And))
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
        while (stream.MatchClass(TOKEN_EQUALITY_OPERATOR_BIT))
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
        while (stream.MatchClass(TOKEN_COMPARISON_OPERATOR_BIT))
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
        while (stream.MatchClass(TOKEN_SHIFT_OPERATOR_BIT))
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
        while (stream.MatchClass(TOKEN_ADD_SUB_OPERATOR_BIT))
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
        while (stream.MatchClass(TOKEN_MUL_DIV_MOD_OPERATOR_BIT))
        {
            const Token& tok = stream.Data(-1);
            Expression* rhs = ParseExpression(stream, ret, res, 11);
            res = Alloc<BinaryExpression>(StringToFourCC(TransientString(tok.text)), res, rhs);
            res->location = LocationFromToken(tok);
        }
    }
    else if (precedence == 11)
    {
        // Right-associative unary operators
        if (stream.MatchClass(TOKEN_PREFIX_OPERATOR_BIT))
        {
            const Token& tok = stream.Data(-1);
            
            Expression* rhs = ParseExpression(stream, ret, res, 12);
            res = Alloc<UnaryExpression>(StringToFourCC(TransientString(tok.text)), true, rhs);
            res->location = LocationFromToken(tok);
        }
        else
            res = ParseExpression(stream, ret, res, 12);

    }
    else if (precedence == 12)
    {
        res = ParseExpression(stream, ret, res, 13);
        while (true)
        {
            if (stream.Match(TokenType::Dot))
            {
                const Token& tok = stream.Data(-1);
                Expression* rhs = ParseExpression(stream, ret, res, 13);
                res = Alloc<AccessExpression>(res, rhs, false);
                res->location = LocationFromToken(tok);
            }
            else if (stream.Match(TokenType::LeftBracket))
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
            else if (stream.Match(TokenType::LeftParant))
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
            else if (stream.MatchClass(TOKEN_INCREMENT_DECREMENT_OPERATOR_BIT))
            {
                const Token& tok = stream.Data(-1);
                res = Alloc<UnaryExpression>(StringToFourCC(TransientString(stream.Data(-1).text)), false, res);
                res->location = LocationFromToken(tok);
            }
            else
                break;
        }
    }
    else if (precedence == 13)
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
        
        res->value = ParseExpression2(stream, ret);
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
    if (stream.MatchClass(TOKEN_FUNCTION_ATTRIBUTE_BIT))
    {
        const Token& tok = stream.Data(-1);
        if (stream.Match(TokenType::LeftParant))
        {
            Expression* expr = ParseExpression2(stream, ret);
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
    if (stream.MatchClass(TOKEN_VARIABLE_ATTRIBUTE_BIT))
    {
        const Token& tok = stream.Data(-1);
        if (stream.Match(TokenType::LeftParant))
        {
            Expression* expr = ParseExpression2(stream, ret);
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
    if (stream.MatchClass(TOKEN_PARAMETER_ATTRIBUTE_BIT))
    {
        const Token& tok = stream.Data(-1);
        if (stream.Match(TokenType::LeftParant))
        {
            Expression* expr = ParseExpression2(stream, ret);
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
    TransientArray<Type::FullType::Modifier> modifiers(32);
    TransientArray<Expression*> modifierValues(32);
    while (true)
    {
        if (stream.Match(TokenType::Mul))
        {
            modifiers.Append(Type::FullType::Modifier::Pointer);
            modifierValues.Append(nullptr);
        }
        else if (stream.Match(TokenType::LeftBracket))
        {
            Expression* sizeExpr = ParseExpression2(stream, ret);
            modifiers.Append(Type::FullType::Modifier::Array);
            modifierValues.Append(sizeExpr);
            
            if (!stream.Match(TokenType::RightBracket))
            {
                ret.errors.Append(UnexpectedToken(stream, "]"));
                return false;
            }
        }
        else
            break;
    }
    res.modifiers = modifiers;
    res.modifierValues = modifierValues;
    
    if (stream.Match(TokenType::Mutable_TypeModifier))
    {
        res.modifierLocation = LocationFromToken(stream.Data(-1));
        res.mut = true;
    }
    else if (stream.Match(TokenType::Literal_TypeModifier))
    {
        res.modifierLocation = LocationFromToken(stream.Data(-1));
        res.literal = true;
    }
    
    switch (stream.Type())
    {
        case TokenType::Rgba16:
            res.imageFormat = ImageFormat::Rgba16;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::Rgb10_A2:
            res.imageFormat = ImageFormat::Rgb10_A2;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::Rgba8:
            res.imageFormat = ImageFormat::Rgba8;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::Rg16:
            res.imageFormat = ImageFormat::Rg16;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::Rg8:
            res.imageFormat = ImageFormat::Rg8;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::R16:
            res.imageFormat = ImageFormat::R16;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::R8:
            res.imageFormat = ImageFormat::R8;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::Rgba16_Snorm:
            res.imageFormat = ImageFormat::Rgba16_Snorm;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::Rgba8_Snorm:
            res.imageFormat = ImageFormat::Rgba8_Snorm;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::Rg16_Snorm:
            res.imageFormat = ImageFormat::Rg16_Snorm;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::Rg8_Snorm:
            res.imageFormat = ImageFormat::Rg8_Snorm;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::R16_Snorm:
            res.imageFormat = ImageFormat::R16_Snorm;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::R8_Snorm:
            res.imageFormat = ImageFormat::R8_Snorm;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::Rgba32F:
            res.imageFormat = ImageFormat::Rgba32F;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::Rgba16F:
            res.imageFormat = ImageFormat::Rgba16F;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::Rg32F:
            res.imageFormat = ImageFormat::Rg32F;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::Rg16F:
            res.imageFormat = ImageFormat::Rg16F;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::R11G11B10F:
            res.imageFormat = ImageFormat::R11G11B10F;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::R32F:
            res.imageFormat = ImageFormat::R32F;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::R16F:
            res.imageFormat = ImageFormat::R16F;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::Rgba32I:
            res.imageFormat = ImageFormat::Rgba32I;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::Rgba16I:
            res.imageFormat = ImageFormat::Rgba16I;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::Rgba8I:
            res.imageFormat = ImageFormat::Rgba8I;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::Rg32I:
            res.imageFormat = ImageFormat::Rg32I;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::Rg16I:
            res.imageFormat = ImageFormat::Rg16I;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::Rg8I:
            res.imageFormat = ImageFormat::Rg8I;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::R32I:
            res.imageFormat = ImageFormat::R32I;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::R16I:
            res.imageFormat = ImageFormat::R16I;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::R8I:
            res.imageFormat = ImageFormat::R8I;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::Rgba32U:
            res.imageFormat = ImageFormat::Rgba32U;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::Rgba16U:
            res.imageFormat = ImageFormat::Rgba16U;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::Rgb10_A2U:
            res.imageFormat = ImageFormat::Rgb10_A2U;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::Rgba8U:
            res.imageFormat = ImageFormat::Rgba8U;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::Rg32U:
            res.imageFormat = ImageFormat::Rg32U;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::Rg16U:
            res.imageFormat = ImageFormat::Rg16U;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::Rg8U:
            res.imageFormat = ImageFormat::Rg8U;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::R32U:
            res.imageFormat = ImageFormat::R32U;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::R16U:
            res.imageFormat = ImageFormat::R16U;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::R8U:
            res.imageFormat = ImageFormat::R8U;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
        case TokenType::UnknownFormat:
            res.imageFormat = ImageFormat::Unknown;
            res.formatLocation = LocationFromToken(stream.Data());
            stream.Consume();
            break;
    }
    if (!stream.Match(TokenType::Identifier))
    {
        ret.errors.Append(UnexpectedToken(stream, "type"));
        return false;
    }
    res.nameLocation = LocationFromToken(stream.Data(-1));
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
        ret.errors.Append(UnexpectedEndToken(stream, ";"));
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
            var->type = UndefinedType;
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
            values.Append(ParseExpression2(stream, ret));

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

            GPULangDiagnostic diagnostic;
            diagnostic.file = tok.path;
            diagnostic.error = TStr("Expected a variable (%d) for for each value (%d)", vars.size, values.size);
            diagnostic.line = tok.startLine;
            diagnostic.column = tok.startChar;
            diagnostic.length = tok.endChar - tok.startChar;
            ret.errors.Append(diagnostic);
        }
        else if (values.size < vars.size)
        {
            const Token& tok = stream.Data(0);

            GPULangDiagnostic diagnostic;
            diagnostic.file = tok.path;
            diagnostic.error = TStr("Expected a value (%d) for for each variable (%d)", values.size, vars.size);
            diagnostic.line = tok.startLine;
            diagnostic.column = tok.startChar;
            diagnostic.length = tok.endChar - tok.startChar;
            ret.errors.Append(diagnostic);
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
    res->documentation = stream.ConsumeComment();
    
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
        var->documentation = stream.ConsumeComment();
        
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
    res->documentation = stream.ConsumeComment();
    
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
            value = ParseExpression2(stream, ret);
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
        ret.errors.Append(UnexpectedEndToken(stream, "}"));
        return res;
    }
    
    if (!stream.Match(TokenType::SemiColon))
    {
        ret.errors.Append(UnexpectedEndToken(stream, ";"));
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
    res->documentation = stream.ConsumeComment();
    
    if (!stream.Match(TokenType::LeftScope))
    {
        ret.errors.Append(UnexpectedToken(stream, "{"));
        return res;
    }
    
    TransientArray<Expression*> entries(32);
    while (Expression* expr = ParseExpression2(stream, ret))
    {
        if (entries.Full())
        {
            ret.errors.Append(Limit(stream, "render_state entries", 32));
            break;
        }
        entries.Append(expr);
        if (!stream.Match(TokenType::SemiColon))
        {
            ret.errors.Append(UnexpectedEndToken(stream, ";"));
            return res;
        }
    }
    res->entries = entries;
    
    if (!stream.Match(TokenType::RightScope))
    {
        ret.errors.Append(UnexpectedEndToken(stream, "}"));
        return res;
    }
    
    if (!stream.Match(TokenType::SemiColon))
    {
        ret.errors.Append(UnexpectedEndToken(stream, ";"));
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
    res->documentation = stream.ConsumeComment();

    if (!stream.Match(TokenType::LeftScope))
    {
        ret.errors.Append(UnexpectedToken(stream, "{"));
        return res;
    }
    
    TransientArray<Expression*> entries(32);
    while (Expression* expr = ParseExpression2(stream, ret))
    {
        if (entries.Full())
        {
            ret.errors.Append(Limit(stream, "sampler_state entries", 32));
            break;
        }
        entries.Append(expr);
        if (!stream.Match(TokenType::SemiColon))
        {
            ret.errors.Append(UnexpectedEndToken(stream, ";"));
            return res;
        }
    }
    res->entries = entries;
    
    if (!stream.Match(TokenType::RightScope))
    {
        ret.errors.Append(UnexpectedEndToken(stream, "}"));
        return res;
    }
    
    if (!stream.Match(TokenType::SemiColon))
    {
        ret.errors.Append(UnexpectedEndToken(stream, ";"));
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
    res->documentation = stream.ConsumeComment();
    
    if (!stream.Match(TokenType::LeftScope))
    {
        ret.errors.Append(UnexpectedToken(stream, "{"));
        return res;
    }
    
    TransientArray<Expression*> entries(32);
    while (Expression* expr = ParseExpression2(stream, ret))
    {
        if (entries.Full())
        {
            ret.errors.Append(Limit(stream, "program entries", 32));
            break;
        }
        entries.Append(expr);
        if (!stream.Match(TokenType::SemiColon))
        {
            ret.errors.Append(UnexpectedEndToken(stream, ";"));
            return res;
        }
    }
    res->entries = entries;
    
    if (!stream.Match(TokenType::RightScope))
    {
        ret.errors.Append(UnexpectedEndToken(stream, "}"));
        return res;
    }
    
    if (!stream.Match(TokenType::SemiColon))
    {
        ret.errors.Append(UnexpectedEndToken(stream, ";"));
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
            else if (stream.MatchClass(TOKEN_VARIABLE_STORAGE_BIT))
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
                        ret.errors.Append(UnexpectedEndToken(stream, ";"));
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
        
        Expression* cond = ParseExpression2(stream, ret);
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
        ret.errors.Append(UnexpectedEndToken(stream, ";"));
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
    res->documentation = stream.ConsumeComment();

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
        var->documentation = stream.ConsumeComment();
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
            ret.errors.Append(UnexpectedEndToken(stream, ";"));
            break;
        }
        members.Append(var);
    }
    res->symbols = members;
    
    if (!stream.Match(TokenType::RightScope))
    {
        ret.errors.Append(UnexpectedEndToken(stream, "}"));
        return res;
    }
    
    if (!stream.Match(TokenType::SemiColon))
    {
        ret.errors.Append(UnexpectedEndToken(stream, ";"));
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
            else if (stream.MatchClass(TOKEN_PARAMETER_STORAGE_BIT))
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
                    std::string_view doc = stream.ConsumeComment();
                    for (auto var : vars)
                    {
                        symbols.Append(var);
                        var->attributes = attributes;
                        var->documentation = doc;
                    }
                    attributes.size = 0;
                    
                    if (!stream.Match(TokenType::SemiColon))
                    {
                        ret.errors.Append(UnexpectedEndToken(stream, ";"));
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
        const Token& tok = stream.Data(-1);
        res = Alloc<BreakStatement>();
        res->location = LocationFromToken(tok);
        if (!stream.Match(TokenType::SemiColon))
        {
            ret.errors.Append(UnexpectedToken(stream, "; after statement"));
            return res;
        }
    }
    else if (stream.Match(TokenType::Continue))
    {
        const Token& tok = stream.Data(-1);
        res = Alloc<ContinueStatement>();
        res->location = LocationFromToken(tok);
        if (!stream.Match(TokenType::SemiColon))
        {
            ret.errors.Append(UnexpectedToken(stream, "; after statement"));
            return res;
        }
    }
    else if (stream.Match(TokenType::Return))
    {
        const Token& tok = stream.Data(-1);
        res = Alloc<TerminateStatement>(ParseExpression2(stream, ret), TerminateStatement::TerminationType::Return);
        res->location = LocationFromToken(tok);
        if (!stream.Match(TokenType::SemiColon))
        {
            ret.errors.Append(UnexpectedToken(stream, "; after statement"));
            return res;
        }
    }
    else if (stream.Match(TokenType::Discard))
    {
        const Token& tok = stream.Data(-1);
        res = Alloc<TerminateStatement>(nullptr, TerminateStatement::TerminationType::Discard);
        res->location = LocationFromToken(tok);
        if (!stream.Match(TokenType::SemiColon))
        {
            ret.errors.Append(UnexpectedToken(stream, "; after statement"));
            return res;
        }
    }
    else if (stream.Match(TokenType::RayIgnore))
    {
        const Token& tok = stream.Data(-1);
        res = Alloc<TerminateStatement>(nullptr, TerminateStatement::TerminationType::RayIgnoreIntersection);
        res->location = LocationFromToken(tok);
        if (!stream.Match(TokenType::SemiColon))
        {
            ret.errors.Append(UnexpectedToken(stream, "; after statement"));
            return res;
        }
    }
    else if (stream.Match(TokenType::RayTerminate))
    {
        const Token& tok = stream.Data(-1);
        res = Alloc<TerminateStatement>(nullptr, TerminateStatement::TerminationType::RayTerminate);
        res->location = LocationFromToken(tok);
        if (!stream.Match(TokenType::SemiColon))
        {
            ret.errors.Append(UnexpectedToken(stream, "; after statement"));
            return res;
        }
    }
    else if (stream.Match(TokenType::If))
    {
        const Token& tok = stream.Data(-1);
        if (!stream.Match(TokenType::LeftParant))
        {
            ret.errors.Append(UnexpectedToken(stream, "("));
            return res;
        }
        
        Expression* cond = ParseExpression2(stream, ret);
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
        res->location = LocationFromToken(tok);
    }
    else if ((res = ParseScopeStatement(stream, ret)))
    {
    }
    else if (stream.Match(TokenType::For))
    {
        const Token& tok = stream.Data(-1);
        if (!stream.Match(TokenType::LeftParant))
        {
            ret.errors.Append(UnexpectedToken(stream, "("));
            return res;
        }
        
        // If variables, parse
        FixedArray<Variable*> variables;
        if (stream.MatchClass(TOKEN_PARAMETER_STORAGE_BIT))
        {
            Attribute* storage = Alloc<Attribute>();
            storage->name = stream.Data(-1).text;
            storage->location = LocationFromToken(stream.Data(-1));
            storage->expression = nullptr;

            variables = ParseVariables(stream, ret);
            for (auto var : variables)
                var->attributes = { storage };

        }
        
        if (!stream.Match(TokenType::SemiColon))
        {
            ret.errors.Append(UnexpectedToken(stream, ";"));
            return res;
        }   
        
        Expression* cond = ParseExpression2(stream, ret);
        if (!stream.Match(TokenType::SemiColon))
        {
            ret.errors.Append(UnexpectedToken(stream, ";"));
            return res;
        }
        
        Expression* postLoop = ParseExpression2(stream, ret);
        if (!stream.Match(TokenType::RightParant))
        {
            ret.errors.Append(UnexpectedToken(stream, ")"));
            return res;
        }
        Statement* body = ParseStatement(stream, ret);
        res = Alloc<ForStatement>(variables, cond, postLoop, body);
        res->location = LocationFromToken(tok);
    }
    else if (stream.Match(TokenType::While))
    {
        const Token& tok = stream.Data(-1);
        if (!stream.Match(TokenType::LeftParant))
        {
            ret.errors.Append(UnexpectedToken(stream, "("));
            return res;
        }
        Expression* cond = ParseExpression2(stream, ret);
        
        if (!stream.Match(TokenType::RightParant))
        {
            ret.errors.Append(UnexpectedToken(stream, ")"));
            return res;
        }
        Statement* body = ParseStatement(stream, ret);
        res = Alloc<WhileStatement>(cond, body, false);
        res->location = LocationFromToken(tok);
    }
    else if (stream.Match(TokenType::Do))
    {
        const Token& tok = stream.Data(-1);
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
        Expression* cond = ParseExpression2(stream, ret);
        
        if (!stream.Match(TokenType::RightParant))
        {
            ret.errors.Append(UnexpectedToken(stream, ")"));
            return res;
        }
        res = Alloc<WhileStatement>(cond, body, true);
        res->location = LocationFromToken(tok);
        
        if (!stream.Match(TokenType::SemiColon))
        {
            ret.errors.Append(UnexpectedToken(stream, "; after expression"));
            return res;
        }
    }
    else
    {
        FixedArray<Expression*> exprs = ParseExpressionList2(stream, ret);
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
    SetupTokenClassTable();
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
        
        if (Annotation* annot = ParseAnnotation(stream, ret))
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
        else if (stream.MatchClass(TOKEN_VARIABLE_STORAGE_BIT))
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
                const std::string_view comment = stream.ConsumeComment();
                for (auto var : vars)
                {
                    var->attributes = attributes;
                    var->annotations = annotations;
                    var->documentation = comment;
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
