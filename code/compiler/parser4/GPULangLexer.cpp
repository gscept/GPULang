
    #include <string>
    #include <vector>
    #include "gpulangtoken.h"


// Generated from antlr4/grammar/GPULang.g4 by ANTLR 4.13.2


#include "GPULangLexer.h"


using namespace antlr4;



using namespace antlr4;

namespace {

struct GPULangLexerStaticData final {
  GPULangLexerStaticData(std::vector<std::string> ruleNames,
                          std::vector<std::string> channelNames,
                          std::vector<std::string> modeNames,
                          std::vector<std::string> literalNames,
                          std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), channelNames(std::move(channelNames)),
        modeNames(std::move(modeNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  GPULangLexerStaticData(const GPULangLexerStaticData&) = delete;
  GPULangLexerStaticData(GPULangLexerStaticData&&) = delete;
  GPULangLexerStaticData& operator=(const GPULangLexerStaticData&) = delete;
  GPULangLexerStaticData& operator=(GPULangLexerStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> channelNames;
  const std::vector<std::string> modeNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag gpulanglexerLexerOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
std::unique_ptr<GPULangLexerStaticData> gpulanglexerLexerStaticData = nullptr;

void gpulanglexerLexerInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (gpulanglexerLexerStaticData != nullptr) {
    return;
  }
#else
  assert(gpulanglexerLexerStaticData == nullptr);
#endif
  auto staticData = std::make_unique<GPULangLexerStaticData>(
    std::vector<std::string>{
      "T__0", "T__1", "T__2", "T__3", "T__4", "T__5", "T__6", "T__7", "T__8", 
      "T__9", "T__10", "T__11", "T__12", "T__13", "T__14", "T__15", "T__16", 
      "T__17", "T__18", "T__19", "T__20", "T__21", "T__22", "T__23", "T__24", 
      "T__25", "T__26", "T__27", "T__28", "T__29", "T__30", "T__31", "T__32", 
      "T__33", "T__34", "SC", "CO", "COL", "LP", "RP", "LB", "RB", "LL", 
      "RR", "DOT", "NOT", "EQ", "QO", "QU", "AND", "ANDSET", "OR", "ORSET", 
      "XOR", "XORSET", "CONNJUGATE", "Q", "NU", "FORWARDSLASH", "LESS", 
      "LESSEQ", "GREATER", "GREATEREQ", "LOGICEQ", "NOTEQ", "LOGICAND", 
      "LOGICOR", "MOD", "UNDERSC", "SOBAKA", "ADD_OP", "SUB_OP", "DIV_OP", 
      "MUL_OP", "ARROW", "INTEGER", "INTEGERLITERAL", "UINTEGERLITERAL", 
      "COMMENT", "ML_COMMENT", "FLOATLITERAL", "EXPONENT", "DOUBLELITERAL", 
      "HEX", "ALPHABET", "IDENTIFIER", "WS"
    },
    std::vector<std::string>{
      "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
    },
    std::vector<std::string>{
      "DEFAULT_MODE"
    },
    std::vector<std::string>{
      "", "'true'", "'false'", "'#line'", "'alias'", "'as'", "'struct'", 
      "'enum'", "'program'", "'sampler'", "'render_state'", "'if'", "'else'", 
      "'for'", "'..'", "'while'", "'do'", "'return'", "'discard'", "'continue'", 
      "'switch'", "'case'", "'default'", "'break'", "'+='", "'-='", "'*='", 
      "'/='", "'%='", "'<<='", "'>>='", "'<<'", "'>>'", "'++'", "'--'", 
      "'::'", "';'", "','", "':'", "'('", "')'", "'{'", "'}'", "'['", "']'", 
      "'.'", "'!'", "'='", "'\"'", "'\\u003F'", "'&'", "'&='", "'|'", "'|='", 
      "'^'", "'^='", "'~'", "'''", "'#'", "'\\'", "'<'", "'<='", "'>'", 
      "'>='", "'=='", "'!='", "'&&'", "'||'", "'%'", "'_'", "'@'", "'+'", 
      "'-'", "'/'", "'*'", "'->'"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "SC", "CO", "COL", "LP", "RP", "LB", "RB", "LL", "RR", "DOT", 
      "NOT", "EQ", "QO", "QU", "AND", "ANDSET", "OR", "ORSET", "XOR", "XORSET", 
      "CONNJUGATE", "Q", "NU", "FORWARDSLASH", "LESS", "LESSEQ", "GREATER", 
      "GREATEREQ", "LOGICEQ", "NOTEQ", "LOGICAND", "LOGICOR", "MOD", "UNDERSC", 
      "SOBAKA", "ADD_OP", "SUB_OP", "DIV_OP", "MUL_OP", "ARROW", "INTEGERLITERAL", 
      "UINTEGERLITERAL", "COMMENT", "ML_COMMENT", "FLOATLITERAL", "EXPONENT", 
      "DOUBLELITERAL", "HEX", "IDENTIFIER", "WS"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,0,85,599,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,
  	6,2,7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,
  	7,14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,
  	7,21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,
  	7,28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,
  	7,35,2,36,7,36,2,37,7,37,2,38,7,38,2,39,7,39,2,40,7,40,2,41,7,41,2,42,
  	7,42,2,43,7,43,2,44,7,44,2,45,7,45,2,46,7,46,2,47,7,47,2,48,7,48,2,49,
  	7,49,2,50,7,50,2,51,7,51,2,52,7,52,2,53,7,53,2,54,7,54,2,55,7,55,2,56,
  	7,56,2,57,7,57,2,58,7,58,2,59,7,59,2,60,7,60,2,61,7,61,2,62,7,62,2,63,
  	7,63,2,64,7,64,2,65,7,65,2,66,7,66,2,67,7,67,2,68,7,68,2,69,7,69,2,70,
  	7,70,2,71,7,71,2,72,7,72,2,73,7,73,2,74,7,74,2,75,7,75,2,76,7,76,2,77,
  	7,77,2,78,7,78,2,79,7,79,2,80,7,80,2,81,7,81,2,82,7,82,2,83,7,83,2,84,
  	7,84,2,85,7,85,2,86,7,86,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,
  	1,2,1,2,1,2,1,2,1,2,1,2,1,3,1,3,1,3,1,3,1,3,1,3,1,4,1,4,1,4,1,5,1,5,1,
  	5,1,5,1,5,1,5,1,5,1,6,1,6,1,6,1,6,1,6,1,7,1,7,1,7,1,7,1,7,1,7,1,7,1,7,
  	1,8,1,8,1,8,1,8,1,8,1,8,1,8,1,8,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,
  	9,1,9,1,9,1,9,1,10,1,10,1,10,1,11,1,11,1,11,1,11,1,11,1,12,1,12,1,12,
  	1,12,1,13,1,13,1,13,1,14,1,14,1,14,1,14,1,14,1,14,1,15,1,15,1,15,1,16,
  	1,16,1,16,1,16,1,16,1,16,1,16,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,
  	1,18,1,18,1,18,1,18,1,18,1,18,1,18,1,18,1,18,1,19,1,19,1,19,1,19,1,19,
  	1,19,1,19,1,20,1,20,1,20,1,20,1,20,1,21,1,21,1,21,1,21,1,21,1,21,1,21,
  	1,21,1,22,1,22,1,22,1,22,1,22,1,22,1,23,1,23,1,23,1,24,1,24,1,24,1,25,
  	1,25,1,25,1,26,1,26,1,26,1,27,1,27,1,27,1,28,1,28,1,28,1,28,1,29,1,29,
  	1,29,1,29,1,30,1,30,1,30,1,31,1,31,1,31,1,32,1,32,1,32,1,33,1,33,1,33,
  	1,34,1,34,1,34,1,35,1,35,1,36,1,36,1,37,1,37,1,38,1,38,1,39,1,39,1,40,
  	1,40,1,41,1,41,1,42,1,42,1,43,1,43,1,44,1,44,1,45,1,45,1,46,1,46,1,47,
  	1,47,1,48,1,48,1,49,1,49,1,50,1,50,1,50,1,51,1,51,1,52,1,52,1,52,1,53,
  	1,53,1,54,1,54,1,54,1,55,1,55,1,56,1,56,1,57,1,57,1,58,1,58,1,59,1,59,
  	1,60,1,60,1,60,1,61,1,61,1,62,1,62,1,62,1,63,1,63,1,63,1,64,1,64,1,64,
  	1,65,1,65,1,65,1,66,1,66,1,66,1,67,1,67,1,68,1,68,1,69,1,69,1,70,1,70,
  	1,71,1,71,1,72,1,72,1,73,1,73,1,74,1,74,1,74,1,75,1,75,1,76,4,76,448,
  	8,76,11,76,12,76,449,1,77,4,77,453,8,77,11,77,12,77,454,1,77,1,77,1,78,
  	1,78,1,78,1,78,5,78,463,8,78,10,78,12,78,466,9,78,1,78,1,78,1,78,1,78,
  	1,79,1,79,1,79,1,79,5,79,476,8,79,10,79,12,79,479,9,79,1,79,1,79,1,79,
  	1,79,1,79,1,80,4,80,487,8,80,11,80,12,80,488,1,80,1,80,5,80,493,8,80,
  	10,80,12,80,496,9,80,1,80,3,80,499,8,80,1,80,1,80,1,80,1,80,5,80,505,
  	8,80,10,80,12,80,508,9,80,1,80,3,80,511,8,80,1,80,1,80,1,80,4,80,516,
  	8,80,11,80,12,80,517,1,80,3,80,521,8,80,1,80,1,80,3,80,525,8,80,1,81,
  	1,81,3,81,529,8,81,1,81,4,81,532,8,81,11,81,12,81,533,1,82,4,82,537,8,
  	82,11,82,12,82,538,1,82,1,82,5,82,543,8,82,10,82,12,82,546,9,82,1,82,
  	3,82,549,8,82,1,82,1,82,1,82,3,82,554,8,82,1,82,4,82,557,8,82,11,82,12,
  	82,558,1,82,1,82,3,82,563,8,82,1,83,1,83,1,83,5,83,568,8,83,10,83,12,
  	83,571,9,83,1,83,3,83,574,8,83,1,84,1,84,1,85,5,85,579,8,85,10,85,12,
  	85,582,9,85,1,85,1,85,1,85,1,85,5,85,588,8,85,10,85,12,85,591,9,85,1,
  	86,4,86,594,8,86,11,86,12,86,595,1,86,1,86,2,464,477,0,87,1,1,3,2,5,3,
  	7,4,9,5,11,6,13,7,15,8,17,9,19,10,21,11,23,12,25,13,27,14,29,15,31,16,
  	33,17,35,18,37,19,39,20,41,21,43,22,45,23,47,24,49,25,51,26,53,27,55,
  	28,57,29,59,30,61,31,63,32,65,33,67,34,69,35,71,36,73,37,75,38,77,39,
  	79,40,81,41,83,42,85,43,87,44,89,45,91,46,93,47,95,48,97,49,99,50,101,
  	51,103,52,105,53,107,54,109,55,111,56,113,57,115,58,117,59,119,60,121,
  	61,123,62,125,63,127,64,129,65,131,66,133,67,135,68,137,69,139,70,141,
  	71,143,72,145,73,147,74,149,75,151,0,153,76,155,77,157,78,159,79,161,
  	80,163,81,165,82,167,83,169,0,171,84,173,85,1,0,6,2,0,85,85,117,117,2,
  	0,69,69,101,101,2,0,43,43,45,45,3,0,48,57,65,70,97,102,2,0,65,90,97,122,
  	3,0,9,10,12,13,32,32,625,0,1,1,0,0,0,0,3,1,0,0,0,0,5,1,0,0,0,0,7,1,0,
  	0,0,0,9,1,0,0,0,0,11,1,0,0,0,0,13,1,0,0,0,0,15,1,0,0,0,0,17,1,0,0,0,0,
  	19,1,0,0,0,0,21,1,0,0,0,0,23,1,0,0,0,0,25,1,0,0,0,0,27,1,0,0,0,0,29,1,
  	0,0,0,0,31,1,0,0,0,0,33,1,0,0,0,0,35,1,0,0,0,0,37,1,0,0,0,0,39,1,0,0,
  	0,0,41,1,0,0,0,0,43,1,0,0,0,0,45,1,0,0,0,0,47,1,0,0,0,0,49,1,0,0,0,0,
  	51,1,0,0,0,0,53,1,0,0,0,0,55,1,0,0,0,0,57,1,0,0,0,0,59,1,0,0,0,0,61,1,
  	0,0,0,0,63,1,0,0,0,0,65,1,0,0,0,0,67,1,0,0,0,0,69,1,0,0,0,0,71,1,0,0,
  	0,0,73,1,0,0,0,0,75,1,0,0,0,0,77,1,0,0,0,0,79,1,0,0,0,0,81,1,0,0,0,0,
  	83,1,0,0,0,0,85,1,0,0,0,0,87,1,0,0,0,0,89,1,0,0,0,0,91,1,0,0,0,0,93,1,
  	0,0,0,0,95,1,0,0,0,0,97,1,0,0,0,0,99,1,0,0,0,0,101,1,0,0,0,0,103,1,0,
  	0,0,0,105,1,0,0,0,0,107,1,0,0,0,0,109,1,0,0,0,0,111,1,0,0,0,0,113,1,0,
  	0,0,0,115,1,0,0,0,0,117,1,0,0,0,0,119,1,0,0,0,0,121,1,0,0,0,0,123,1,0,
  	0,0,0,125,1,0,0,0,0,127,1,0,0,0,0,129,1,0,0,0,0,131,1,0,0,0,0,133,1,0,
  	0,0,0,135,1,0,0,0,0,137,1,0,0,0,0,139,1,0,0,0,0,141,1,0,0,0,0,143,1,0,
  	0,0,0,145,1,0,0,0,0,147,1,0,0,0,0,149,1,0,0,0,0,153,1,0,0,0,0,155,1,0,
  	0,0,0,157,1,0,0,0,0,159,1,0,0,0,0,161,1,0,0,0,0,163,1,0,0,0,0,165,1,0,
  	0,0,0,167,1,0,0,0,0,171,1,0,0,0,0,173,1,0,0,0,1,175,1,0,0,0,3,180,1,0,
  	0,0,5,186,1,0,0,0,7,192,1,0,0,0,9,198,1,0,0,0,11,201,1,0,0,0,13,208,1,
  	0,0,0,15,213,1,0,0,0,17,221,1,0,0,0,19,229,1,0,0,0,21,242,1,0,0,0,23,
  	245,1,0,0,0,25,250,1,0,0,0,27,254,1,0,0,0,29,257,1,0,0,0,31,263,1,0,0,
  	0,33,266,1,0,0,0,35,273,1,0,0,0,37,281,1,0,0,0,39,290,1,0,0,0,41,297,
  	1,0,0,0,43,302,1,0,0,0,45,310,1,0,0,0,47,316,1,0,0,0,49,319,1,0,0,0,51,
  	322,1,0,0,0,53,325,1,0,0,0,55,328,1,0,0,0,57,331,1,0,0,0,59,335,1,0,0,
  	0,61,339,1,0,0,0,63,342,1,0,0,0,65,345,1,0,0,0,67,348,1,0,0,0,69,351,
  	1,0,0,0,71,354,1,0,0,0,73,356,1,0,0,0,75,358,1,0,0,0,77,360,1,0,0,0,79,
  	362,1,0,0,0,81,364,1,0,0,0,83,366,1,0,0,0,85,368,1,0,0,0,87,370,1,0,0,
  	0,89,372,1,0,0,0,91,374,1,0,0,0,93,376,1,0,0,0,95,378,1,0,0,0,97,380,
  	1,0,0,0,99,382,1,0,0,0,101,384,1,0,0,0,103,387,1,0,0,0,105,389,1,0,0,
  	0,107,392,1,0,0,0,109,394,1,0,0,0,111,397,1,0,0,0,113,399,1,0,0,0,115,
  	401,1,0,0,0,117,403,1,0,0,0,119,405,1,0,0,0,121,407,1,0,0,0,123,410,1,
  	0,0,0,125,412,1,0,0,0,127,415,1,0,0,0,129,418,1,0,0,0,131,421,1,0,0,0,
  	133,424,1,0,0,0,135,427,1,0,0,0,137,429,1,0,0,0,139,431,1,0,0,0,141,433,
  	1,0,0,0,143,435,1,0,0,0,145,437,1,0,0,0,147,439,1,0,0,0,149,441,1,0,0,
  	0,151,444,1,0,0,0,153,447,1,0,0,0,155,452,1,0,0,0,157,458,1,0,0,0,159,
  	471,1,0,0,0,161,524,1,0,0,0,163,526,1,0,0,0,165,562,1,0,0,0,167,564,1,
  	0,0,0,169,575,1,0,0,0,171,580,1,0,0,0,173,593,1,0,0,0,175,176,5,116,0,
  	0,176,177,5,114,0,0,177,178,5,117,0,0,178,179,5,101,0,0,179,2,1,0,0,0,
  	180,181,5,102,0,0,181,182,5,97,0,0,182,183,5,108,0,0,183,184,5,115,0,
  	0,184,185,5,101,0,0,185,4,1,0,0,0,186,187,5,35,0,0,187,188,5,108,0,0,
  	188,189,5,105,0,0,189,190,5,110,0,0,190,191,5,101,0,0,191,6,1,0,0,0,192,
  	193,5,97,0,0,193,194,5,108,0,0,194,195,5,105,0,0,195,196,5,97,0,0,196,
  	197,5,115,0,0,197,8,1,0,0,0,198,199,5,97,0,0,199,200,5,115,0,0,200,10,
  	1,0,0,0,201,202,5,115,0,0,202,203,5,116,0,0,203,204,5,114,0,0,204,205,
  	5,117,0,0,205,206,5,99,0,0,206,207,5,116,0,0,207,12,1,0,0,0,208,209,5,
  	101,0,0,209,210,5,110,0,0,210,211,5,117,0,0,211,212,5,109,0,0,212,14,
  	1,0,0,0,213,214,5,112,0,0,214,215,5,114,0,0,215,216,5,111,0,0,216,217,
  	5,103,0,0,217,218,5,114,0,0,218,219,5,97,0,0,219,220,5,109,0,0,220,16,
  	1,0,0,0,221,222,5,115,0,0,222,223,5,97,0,0,223,224,5,109,0,0,224,225,
  	5,112,0,0,225,226,5,108,0,0,226,227,5,101,0,0,227,228,5,114,0,0,228,18,
  	1,0,0,0,229,230,5,114,0,0,230,231,5,101,0,0,231,232,5,110,0,0,232,233,
  	5,100,0,0,233,234,5,101,0,0,234,235,5,114,0,0,235,236,5,95,0,0,236,237,
  	5,115,0,0,237,238,5,116,0,0,238,239,5,97,0,0,239,240,5,116,0,0,240,241,
  	5,101,0,0,241,20,1,0,0,0,242,243,5,105,0,0,243,244,5,102,0,0,244,22,1,
  	0,0,0,245,246,5,101,0,0,246,247,5,108,0,0,247,248,5,115,0,0,248,249,5,
  	101,0,0,249,24,1,0,0,0,250,251,5,102,0,0,251,252,5,111,0,0,252,253,5,
  	114,0,0,253,26,1,0,0,0,254,255,5,46,0,0,255,256,5,46,0,0,256,28,1,0,0,
  	0,257,258,5,119,0,0,258,259,5,104,0,0,259,260,5,105,0,0,260,261,5,108,
  	0,0,261,262,5,101,0,0,262,30,1,0,0,0,263,264,5,100,0,0,264,265,5,111,
  	0,0,265,32,1,0,0,0,266,267,5,114,0,0,267,268,5,101,0,0,268,269,5,116,
  	0,0,269,270,5,117,0,0,270,271,5,114,0,0,271,272,5,110,0,0,272,34,1,0,
  	0,0,273,274,5,100,0,0,274,275,5,105,0,0,275,276,5,115,0,0,276,277,5,99,
  	0,0,277,278,5,97,0,0,278,279,5,114,0,0,279,280,5,100,0,0,280,36,1,0,0,
  	0,281,282,5,99,0,0,282,283,5,111,0,0,283,284,5,110,0,0,284,285,5,116,
  	0,0,285,286,5,105,0,0,286,287,5,110,0,0,287,288,5,117,0,0,288,289,5,101,
  	0,0,289,38,1,0,0,0,290,291,5,115,0,0,291,292,5,119,0,0,292,293,5,105,
  	0,0,293,294,5,116,0,0,294,295,5,99,0,0,295,296,5,104,0,0,296,40,1,0,0,
  	0,297,298,5,99,0,0,298,299,5,97,0,0,299,300,5,115,0,0,300,301,5,101,0,
  	0,301,42,1,0,0,0,302,303,5,100,0,0,303,304,5,101,0,0,304,305,5,102,0,
  	0,305,306,5,97,0,0,306,307,5,117,0,0,307,308,5,108,0,0,308,309,5,116,
  	0,0,309,44,1,0,0,0,310,311,5,98,0,0,311,312,5,114,0,0,312,313,5,101,0,
  	0,313,314,5,97,0,0,314,315,5,107,0,0,315,46,1,0,0,0,316,317,5,43,0,0,
  	317,318,5,61,0,0,318,48,1,0,0,0,319,320,5,45,0,0,320,321,5,61,0,0,321,
  	50,1,0,0,0,322,323,5,42,0,0,323,324,5,61,0,0,324,52,1,0,0,0,325,326,5,
  	47,0,0,326,327,5,61,0,0,327,54,1,0,0,0,328,329,5,37,0,0,329,330,5,61,
  	0,0,330,56,1,0,0,0,331,332,5,60,0,0,332,333,5,60,0,0,333,334,5,61,0,0,
  	334,58,1,0,0,0,335,336,5,62,0,0,336,337,5,62,0,0,337,338,5,61,0,0,338,
  	60,1,0,0,0,339,340,5,60,0,0,340,341,5,60,0,0,341,62,1,0,0,0,342,343,5,
  	62,0,0,343,344,5,62,0,0,344,64,1,0,0,0,345,346,5,43,0,0,346,347,5,43,
  	0,0,347,66,1,0,0,0,348,349,5,45,0,0,349,350,5,45,0,0,350,68,1,0,0,0,351,
  	352,5,58,0,0,352,353,5,58,0,0,353,70,1,0,0,0,354,355,5,59,0,0,355,72,
  	1,0,0,0,356,357,5,44,0,0,357,74,1,0,0,0,358,359,5,58,0,0,359,76,1,0,0,
  	0,360,361,5,40,0,0,361,78,1,0,0,0,362,363,5,41,0,0,363,80,1,0,0,0,364,
  	365,5,123,0,0,365,82,1,0,0,0,366,367,5,125,0,0,367,84,1,0,0,0,368,369,
  	5,91,0,0,369,86,1,0,0,0,370,371,5,93,0,0,371,88,1,0,0,0,372,373,5,46,
  	0,0,373,90,1,0,0,0,374,375,5,33,0,0,375,92,1,0,0,0,376,377,5,61,0,0,377,
  	94,1,0,0,0,378,379,5,34,0,0,379,96,1,0,0,0,380,381,5,63,0,0,381,98,1,
  	0,0,0,382,383,5,38,0,0,383,100,1,0,0,0,384,385,5,38,0,0,385,386,5,61,
  	0,0,386,102,1,0,0,0,387,388,5,124,0,0,388,104,1,0,0,0,389,390,5,124,0,
  	0,390,391,5,61,0,0,391,106,1,0,0,0,392,393,5,94,0,0,393,108,1,0,0,0,394,
  	395,5,94,0,0,395,396,5,61,0,0,396,110,1,0,0,0,397,398,5,126,0,0,398,112,
  	1,0,0,0,399,400,5,39,0,0,400,114,1,0,0,0,401,402,5,35,0,0,402,116,1,0,
  	0,0,403,404,5,92,0,0,404,118,1,0,0,0,405,406,5,60,0,0,406,120,1,0,0,0,
  	407,408,5,60,0,0,408,409,5,61,0,0,409,122,1,0,0,0,410,411,5,62,0,0,411,
  	124,1,0,0,0,412,413,5,62,0,0,413,414,5,61,0,0,414,126,1,0,0,0,415,416,
  	5,61,0,0,416,417,5,61,0,0,417,128,1,0,0,0,418,419,5,33,0,0,419,420,5,
  	61,0,0,420,130,1,0,0,0,421,422,5,38,0,0,422,423,5,38,0,0,423,132,1,0,
  	0,0,424,425,5,124,0,0,425,426,5,124,0,0,426,134,1,0,0,0,427,428,5,37,
  	0,0,428,136,1,0,0,0,429,430,5,95,0,0,430,138,1,0,0,0,431,432,5,64,0,0,
  	432,140,1,0,0,0,433,434,5,43,0,0,434,142,1,0,0,0,435,436,5,45,0,0,436,
  	144,1,0,0,0,437,438,5,47,0,0,438,146,1,0,0,0,439,440,5,42,0,0,440,148,
  	1,0,0,0,441,442,5,45,0,0,442,443,5,62,0,0,443,150,1,0,0,0,444,445,2,48,
  	57,0,445,152,1,0,0,0,446,448,3,151,75,0,447,446,1,0,0,0,448,449,1,0,0,
  	0,449,447,1,0,0,0,449,450,1,0,0,0,450,154,1,0,0,0,451,453,3,151,75,0,
  	452,451,1,0,0,0,453,454,1,0,0,0,454,452,1,0,0,0,454,455,1,0,0,0,455,456,
  	1,0,0,0,456,457,7,0,0,0,457,156,1,0,0,0,458,459,5,47,0,0,459,460,5,47,
  	0,0,460,464,1,0,0,0,461,463,9,0,0,0,462,461,1,0,0,0,463,466,1,0,0,0,464,
  	465,1,0,0,0,464,462,1,0,0,0,465,467,1,0,0,0,466,464,1,0,0,0,467,468,5,
  	10,0,0,468,469,1,0,0,0,469,470,6,78,0,0,470,158,1,0,0,0,471,472,5,47,
  	0,0,472,473,5,42,0,0,473,477,1,0,0,0,474,476,9,0,0,0,475,474,1,0,0,0,
  	476,479,1,0,0,0,477,478,1,0,0,0,477,475,1,0,0,0,478,480,1,0,0,0,479,477,
  	1,0,0,0,480,481,5,42,0,0,481,482,5,47,0,0,482,483,1,0,0,0,483,484,6,79,
  	0,0,484,160,1,0,0,0,485,487,3,151,75,0,486,485,1,0,0,0,487,488,1,0,0,
  	0,488,486,1,0,0,0,488,489,1,0,0,0,489,490,1,0,0,0,490,494,3,89,44,0,491,
  	493,3,151,75,0,492,491,1,0,0,0,493,496,1,0,0,0,494,492,1,0,0,0,494,495,
  	1,0,0,0,495,498,1,0,0,0,496,494,1,0,0,0,497,499,3,163,81,0,498,497,1,
  	0,0,0,498,499,1,0,0,0,499,500,1,0,0,0,500,501,5,102,0,0,501,525,1,0,0,
  	0,502,506,3,89,44,0,503,505,3,151,75,0,504,503,1,0,0,0,505,508,1,0,0,
  	0,506,504,1,0,0,0,506,507,1,0,0,0,507,510,1,0,0,0,508,506,1,0,0,0,509,
  	511,3,163,81,0,510,509,1,0,0,0,510,511,1,0,0,0,511,512,1,0,0,0,512,513,
  	5,102,0,0,513,525,1,0,0,0,514,516,3,151,75,0,515,514,1,0,0,0,516,517,
  	1,0,0,0,517,515,1,0,0,0,517,518,1,0,0,0,518,520,1,0,0,0,519,521,3,163,
  	81,0,520,519,1,0,0,0,520,521,1,0,0,0,521,522,1,0,0,0,522,523,5,102,0,
  	0,523,525,1,0,0,0,524,486,1,0,0,0,524,502,1,0,0,0,524,515,1,0,0,0,525,
  	162,1,0,0,0,526,528,7,1,0,0,527,529,7,2,0,0,528,527,1,0,0,0,528,529,1,
  	0,0,0,529,531,1,0,0,0,530,532,3,151,75,0,531,530,1,0,0,0,532,533,1,0,
  	0,0,533,531,1,0,0,0,533,534,1,0,0,0,534,164,1,0,0,0,535,537,3,151,75,
  	0,536,535,1,0,0,0,537,538,1,0,0,0,538,536,1,0,0,0,538,539,1,0,0,0,539,
  	540,1,0,0,0,540,544,3,89,44,0,541,543,3,151,75,0,542,541,1,0,0,0,543,
  	546,1,0,0,0,544,542,1,0,0,0,544,545,1,0,0,0,545,548,1,0,0,0,546,544,1,
  	0,0,0,547,549,3,163,81,0,548,547,1,0,0,0,548,549,1,0,0,0,549,563,1,0,
  	0,0,550,551,3,89,44,0,551,553,3,151,75,0,552,554,3,163,81,0,553,552,1,
  	0,0,0,553,554,1,0,0,0,554,563,1,0,0,0,555,557,3,151,75,0,556,555,1,0,
  	0,0,557,558,1,0,0,0,558,556,1,0,0,0,558,559,1,0,0,0,559,560,1,0,0,0,560,
  	561,3,163,81,0,561,563,1,0,0,0,562,536,1,0,0,0,562,550,1,0,0,0,562,556,
  	1,0,0,0,563,166,1,0,0,0,564,565,5,48,0,0,565,569,5,120,0,0,566,568,7,
  	3,0,0,567,566,1,0,0,0,568,571,1,0,0,0,569,567,1,0,0,0,569,570,1,0,0,0,
  	570,573,1,0,0,0,571,569,1,0,0,0,572,574,7,0,0,0,573,572,1,0,0,0,573,574,
  	1,0,0,0,574,168,1,0,0,0,575,576,7,4,0,0,576,170,1,0,0,0,577,579,5,95,
  	0,0,578,577,1,0,0,0,579,582,1,0,0,0,580,578,1,0,0,0,580,581,1,0,0,0,581,
  	583,1,0,0,0,582,580,1,0,0,0,583,589,3,169,84,0,584,588,3,169,84,0,585,
  	588,3,153,76,0,586,588,5,95,0,0,587,584,1,0,0,0,587,585,1,0,0,0,587,586,
  	1,0,0,0,588,591,1,0,0,0,589,587,1,0,0,0,589,590,1,0,0,0,590,172,1,0,0,
  	0,591,589,1,0,0,0,592,594,7,5,0,0,593,592,1,0,0,0,594,595,1,0,0,0,595,
  	593,1,0,0,0,595,596,1,0,0,0,596,597,1,0,0,0,597,598,6,86,0,0,598,174,
  	1,0,0,0,27,0,449,454,464,477,488,494,498,506,510,517,520,524,528,533,
  	538,544,548,553,558,562,569,573,580,587,589,595,1,0,1,0
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  gpulanglexerLexerStaticData = std::move(staticData);
}

}

GPULangLexer::GPULangLexer(CharStream *input) : Lexer(input) {
  GPULangLexer::initialize();
  _interpreter = new atn::LexerATNSimulator(this, *gpulanglexerLexerStaticData->atn, gpulanglexerLexerStaticData->decisionToDFA, gpulanglexerLexerStaticData->sharedContextCache);
}

GPULangLexer::~GPULangLexer() {
  delete _interpreter;
}

std::string GPULangLexer::getGrammarFileName() const {
  return "GPULang.g4";
}

const std::vector<std::string>& GPULangLexer::getRuleNames() const {
  return gpulanglexerLexerStaticData->ruleNames;
}

const std::vector<std::string>& GPULangLexer::getChannelNames() const {
  return gpulanglexerLexerStaticData->channelNames;
}

const std::vector<std::string>& GPULangLexer::getModeNames() const {
  return gpulanglexerLexerStaticData->modeNames;
}

const dfa::Vocabulary& GPULangLexer::getVocabulary() const {
  return gpulanglexerLexerStaticData->vocabulary;
}

antlr4::atn::SerializedATNView GPULangLexer::getSerializedATN() const {
  return gpulanglexerLexerStaticData->serializedATN;
}

const atn::ATN& GPULangLexer::getATN() const {
  return *gpulanglexerLexerStaticData->atn;
}




void GPULangLexer::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  gpulanglexerLexerInitialize();
#else
  ::antlr4::internal::call_once(gpulanglexerLexerOnceFlag, gpulanglexerLexerInitialize);
#endif
}
