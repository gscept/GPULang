
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
      "T__33", "SC", "CO", "COL", "LP", "RP", "LB", "RB", "LL", "RR", "DOT", 
      "NOT", "EQ", "QO", "QU", "AND", "ANDSET", "OR", "ORSET", "XOR", "XORSET", 
      "CONNJUGATE", "Q", "NU", "FORWARDSLASH", "LESS", "LESSEQ", "GREATER", 
      "GREATEREQ", "LOGICEQ", "NOTEQ", "LOGICAND", "LOGICOR", "MOD", "UNDERSC", 
      "SOBAKA", "ADD_OP", "SUB_OP", "DIV_OP", "MUL_OP", "ARROW", "INTEGER", 
      "INTEGERLITERAL", "UINTEGERLITERAL", "COMMENT", "ML_COMMENT", "FLOATLITERAL", 
      "EXPONENT", "DOUBLELITERAL", "HEX", "ALPHABET", "IDENTIFIER", "WS"
    },
    std::vector<std::string>{
      "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
    },
    std::vector<std::string>{
      "DEFAULT_MODE"
    },
    std::vector<std::string>{
      "", "'true'", "'false'", "'#line'", "'alias'", "'as'", "'struct'", 
      "'enum'", "'program'", "'sampler_state'", "'render_state'", "'if'", 
      "'else'", "'for'", "'..'", "'while'", "'do'", "'return'", "'continue'", 
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
      "", "SC", "CO", "COL", "LP", "RP", "LB", "RB", "LL", "RR", "DOT", 
      "NOT", "EQ", "QO", "QU", "AND", "ANDSET", "OR", "ORSET", "XOR", "XORSET", 
      "CONNJUGATE", "Q", "NU", "FORWARDSLASH", "LESS", "LESSEQ", "GREATER", 
      "GREATEREQ", "LOGICEQ", "NOTEQ", "LOGICAND", "LOGICOR", "MOD", "UNDERSC", 
      "SOBAKA", "ADD_OP", "SUB_OP", "DIV_OP", "MUL_OP", "ARROW", "INTEGERLITERAL", 
      "UINTEGERLITERAL", "COMMENT", "ML_COMMENT", "FLOATLITERAL", "EXPONENT", 
      "DOUBLELITERAL", "HEX", "IDENTIFIER", "WS"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,0,84,595,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,
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
  	7,84,2,85,7,85,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,2,1,
  	2,1,2,1,2,1,2,1,3,1,3,1,3,1,3,1,3,1,3,1,4,1,4,1,4,1,5,1,5,1,5,1,5,1,5,
  	1,5,1,5,1,6,1,6,1,6,1,6,1,6,1,7,1,7,1,7,1,7,1,7,1,7,1,7,1,7,1,8,1,8,1,
  	8,1,8,1,8,1,8,1,8,1,8,1,8,1,8,1,8,1,8,1,8,1,8,1,9,1,9,1,9,1,9,1,9,1,9,
  	1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,10,1,10,1,10,1,11,1,11,1,11,1,11,1,11,1,
  	12,1,12,1,12,1,12,1,13,1,13,1,13,1,14,1,14,1,14,1,14,1,14,1,14,1,15,1,
  	15,1,15,1,16,1,16,1,16,1,16,1,16,1,16,1,16,1,17,1,17,1,17,1,17,1,17,1,
  	17,1,17,1,17,1,17,1,18,1,18,1,18,1,18,1,18,1,18,1,18,1,19,1,19,1,19,1,
  	19,1,19,1,20,1,20,1,20,1,20,1,20,1,20,1,20,1,20,1,21,1,21,1,21,1,21,1,
  	21,1,21,1,22,1,22,1,22,1,23,1,23,1,23,1,24,1,24,1,24,1,25,1,25,1,25,1,
  	26,1,26,1,26,1,27,1,27,1,27,1,27,1,28,1,28,1,28,1,28,1,29,1,29,1,29,1,
  	30,1,30,1,30,1,31,1,31,1,31,1,32,1,32,1,32,1,33,1,33,1,33,1,34,1,34,1,
  	35,1,35,1,36,1,36,1,37,1,37,1,38,1,38,1,39,1,39,1,40,1,40,1,41,1,41,1,
  	42,1,42,1,43,1,43,1,44,1,44,1,45,1,45,1,46,1,46,1,47,1,47,1,48,1,48,1,
  	49,1,49,1,49,1,50,1,50,1,51,1,51,1,51,1,52,1,52,1,53,1,53,1,53,1,54,1,
  	54,1,55,1,55,1,56,1,56,1,57,1,57,1,58,1,58,1,59,1,59,1,59,1,60,1,60,1,
  	61,1,61,1,61,1,62,1,62,1,62,1,63,1,63,1,63,1,64,1,64,1,64,1,65,1,65,1,
  	65,1,66,1,66,1,67,1,67,1,68,1,68,1,69,1,69,1,70,1,70,1,71,1,71,1,72,1,
  	72,1,73,1,73,1,73,1,74,1,74,1,75,4,75,444,8,75,11,75,12,75,445,1,76,4,
  	76,449,8,76,11,76,12,76,450,1,76,1,76,1,77,1,77,1,77,1,77,5,77,459,8,
  	77,10,77,12,77,462,9,77,1,77,1,77,1,77,1,77,1,78,1,78,1,78,1,78,5,78,
  	472,8,78,10,78,12,78,475,9,78,1,78,1,78,1,78,1,78,1,78,1,79,4,79,483,
  	8,79,11,79,12,79,484,1,79,1,79,5,79,489,8,79,10,79,12,79,492,9,79,1,79,
  	3,79,495,8,79,1,79,1,79,1,79,1,79,5,79,501,8,79,10,79,12,79,504,9,79,
  	1,79,3,79,507,8,79,1,79,1,79,1,79,4,79,512,8,79,11,79,12,79,513,1,79,
  	3,79,517,8,79,1,79,1,79,3,79,521,8,79,1,80,1,80,3,80,525,8,80,1,80,4,
  	80,528,8,80,11,80,12,80,529,1,81,4,81,533,8,81,11,81,12,81,534,1,81,1,
  	81,5,81,539,8,81,10,81,12,81,542,9,81,1,81,3,81,545,8,81,1,81,1,81,1,
  	81,3,81,550,8,81,1,81,4,81,553,8,81,11,81,12,81,554,1,81,1,81,3,81,559,
  	8,81,1,82,1,82,1,82,5,82,564,8,82,10,82,12,82,567,9,82,1,82,3,82,570,
  	8,82,1,83,1,83,1,84,5,84,575,8,84,10,84,12,84,578,9,84,1,84,1,84,1,84,
  	1,84,5,84,584,8,84,10,84,12,84,587,9,84,1,85,4,85,590,8,85,11,85,12,85,
  	591,1,85,1,85,2,460,473,0,86,1,1,3,2,5,3,7,4,9,5,11,6,13,7,15,8,17,9,
  	19,10,21,11,23,12,25,13,27,14,29,15,31,16,33,17,35,18,37,19,39,20,41,
  	21,43,22,45,23,47,24,49,25,51,26,53,27,55,28,57,29,59,30,61,31,63,32,
  	65,33,67,34,69,35,71,36,73,37,75,38,77,39,79,40,81,41,83,42,85,43,87,
  	44,89,45,91,46,93,47,95,48,97,49,99,50,101,51,103,52,105,53,107,54,109,
  	55,111,56,113,57,115,58,117,59,119,60,121,61,123,62,125,63,127,64,129,
  	65,131,66,133,67,135,68,137,69,139,70,141,71,143,72,145,73,147,74,149,
  	0,151,75,153,76,155,77,157,78,159,79,161,80,163,81,165,82,167,0,169,83,
  	171,84,1,0,6,2,0,85,85,117,117,2,0,69,69,101,101,2,0,43,43,45,45,3,0,
  	48,57,65,70,97,102,2,0,65,90,97,122,3,0,9,10,12,13,32,32,621,0,1,1,0,
  	0,0,0,3,1,0,0,0,0,5,1,0,0,0,0,7,1,0,0,0,0,9,1,0,0,0,0,11,1,0,0,0,0,13,
  	1,0,0,0,0,15,1,0,0,0,0,17,1,0,0,0,0,19,1,0,0,0,0,21,1,0,0,0,0,23,1,0,
  	0,0,0,25,1,0,0,0,0,27,1,0,0,0,0,29,1,0,0,0,0,31,1,0,0,0,0,33,1,0,0,0,
  	0,35,1,0,0,0,0,37,1,0,0,0,0,39,1,0,0,0,0,41,1,0,0,0,0,43,1,0,0,0,0,45,
  	1,0,0,0,0,47,1,0,0,0,0,49,1,0,0,0,0,51,1,0,0,0,0,53,1,0,0,0,0,55,1,0,
  	0,0,0,57,1,0,0,0,0,59,1,0,0,0,0,61,1,0,0,0,0,63,1,0,0,0,0,65,1,0,0,0,
  	0,67,1,0,0,0,0,69,1,0,0,0,0,71,1,0,0,0,0,73,1,0,0,0,0,75,1,0,0,0,0,77,
  	1,0,0,0,0,79,1,0,0,0,0,81,1,0,0,0,0,83,1,0,0,0,0,85,1,0,0,0,0,87,1,0,
  	0,0,0,89,1,0,0,0,0,91,1,0,0,0,0,93,1,0,0,0,0,95,1,0,0,0,0,97,1,0,0,0,
  	0,99,1,0,0,0,0,101,1,0,0,0,0,103,1,0,0,0,0,105,1,0,0,0,0,107,1,0,0,0,
  	0,109,1,0,0,0,0,111,1,0,0,0,0,113,1,0,0,0,0,115,1,0,0,0,0,117,1,0,0,0,
  	0,119,1,0,0,0,0,121,1,0,0,0,0,123,1,0,0,0,0,125,1,0,0,0,0,127,1,0,0,0,
  	0,129,1,0,0,0,0,131,1,0,0,0,0,133,1,0,0,0,0,135,1,0,0,0,0,137,1,0,0,0,
  	0,139,1,0,0,0,0,141,1,0,0,0,0,143,1,0,0,0,0,145,1,0,0,0,0,147,1,0,0,0,
  	0,151,1,0,0,0,0,153,1,0,0,0,0,155,1,0,0,0,0,157,1,0,0,0,0,159,1,0,0,0,
  	0,161,1,0,0,0,0,163,1,0,0,0,0,165,1,0,0,0,0,169,1,0,0,0,0,171,1,0,0,0,
  	1,173,1,0,0,0,3,178,1,0,0,0,5,184,1,0,0,0,7,190,1,0,0,0,9,196,1,0,0,0,
  	11,199,1,0,0,0,13,206,1,0,0,0,15,211,1,0,0,0,17,219,1,0,0,0,19,233,1,
  	0,0,0,21,246,1,0,0,0,23,249,1,0,0,0,25,254,1,0,0,0,27,258,1,0,0,0,29,
  	261,1,0,0,0,31,267,1,0,0,0,33,270,1,0,0,0,35,277,1,0,0,0,37,286,1,0,0,
  	0,39,293,1,0,0,0,41,298,1,0,0,0,43,306,1,0,0,0,45,312,1,0,0,0,47,315,
  	1,0,0,0,49,318,1,0,0,0,51,321,1,0,0,0,53,324,1,0,0,0,55,327,1,0,0,0,57,
  	331,1,0,0,0,59,335,1,0,0,0,61,338,1,0,0,0,63,341,1,0,0,0,65,344,1,0,0,
  	0,67,347,1,0,0,0,69,350,1,0,0,0,71,352,1,0,0,0,73,354,1,0,0,0,75,356,
  	1,0,0,0,77,358,1,0,0,0,79,360,1,0,0,0,81,362,1,0,0,0,83,364,1,0,0,0,85,
  	366,1,0,0,0,87,368,1,0,0,0,89,370,1,0,0,0,91,372,1,0,0,0,93,374,1,0,0,
  	0,95,376,1,0,0,0,97,378,1,0,0,0,99,380,1,0,0,0,101,383,1,0,0,0,103,385,
  	1,0,0,0,105,388,1,0,0,0,107,390,1,0,0,0,109,393,1,0,0,0,111,395,1,0,0,
  	0,113,397,1,0,0,0,115,399,1,0,0,0,117,401,1,0,0,0,119,403,1,0,0,0,121,
  	406,1,0,0,0,123,408,1,0,0,0,125,411,1,0,0,0,127,414,1,0,0,0,129,417,1,
  	0,0,0,131,420,1,0,0,0,133,423,1,0,0,0,135,425,1,0,0,0,137,427,1,0,0,0,
  	139,429,1,0,0,0,141,431,1,0,0,0,143,433,1,0,0,0,145,435,1,0,0,0,147,437,
  	1,0,0,0,149,440,1,0,0,0,151,443,1,0,0,0,153,448,1,0,0,0,155,454,1,0,0,
  	0,157,467,1,0,0,0,159,520,1,0,0,0,161,522,1,0,0,0,163,558,1,0,0,0,165,
  	560,1,0,0,0,167,571,1,0,0,0,169,576,1,0,0,0,171,589,1,0,0,0,173,174,5,
  	116,0,0,174,175,5,114,0,0,175,176,5,117,0,0,176,177,5,101,0,0,177,2,1,
  	0,0,0,178,179,5,102,0,0,179,180,5,97,0,0,180,181,5,108,0,0,181,182,5,
  	115,0,0,182,183,5,101,0,0,183,4,1,0,0,0,184,185,5,35,0,0,185,186,5,108,
  	0,0,186,187,5,105,0,0,187,188,5,110,0,0,188,189,5,101,0,0,189,6,1,0,0,
  	0,190,191,5,97,0,0,191,192,5,108,0,0,192,193,5,105,0,0,193,194,5,97,0,
  	0,194,195,5,115,0,0,195,8,1,0,0,0,196,197,5,97,0,0,197,198,5,115,0,0,
  	198,10,1,0,0,0,199,200,5,115,0,0,200,201,5,116,0,0,201,202,5,114,0,0,
  	202,203,5,117,0,0,203,204,5,99,0,0,204,205,5,116,0,0,205,12,1,0,0,0,206,
  	207,5,101,0,0,207,208,5,110,0,0,208,209,5,117,0,0,209,210,5,109,0,0,210,
  	14,1,0,0,0,211,212,5,112,0,0,212,213,5,114,0,0,213,214,5,111,0,0,214,
  	215,5,103,0,0,215,216,5,114,0,0,216,217,5,97,0,0,217,218,5,109,0,0,218,
  	16,1,0,0,0,219,220,5,115,0,0,220,221,5,97,0,0,221,222,5,109,0,0,222,223,
  	5,112,0,0,223,224,5,108,0,0,224,225,5,101,0,0,225,226,5,114,0,0,226,227,
  	5,95,0,0,227,228,5,115,0,0,228,229,5,116,0,0,229,230,5,97,0,0,230,231,
  	5,116,0,0,231,232,5,101,0,0,232,18,1,0,0,0,233,234,5,114,0,0,234,235,
  	5,101,0,0,235,236,5,110,0,0,236,237,5,100,0,0,237,238,5,101,0,0,238,239,
  	5,114,0,0,239,240,5,95,0,0,240,241,5,115,0,0,241,242,5,116,0,0,242,243,
  	5,97,0,0,243,244,5,116,0,0,244,245,5,101,0,0,245,20,1,0,0,0,246,247,5,
  	105,0,0,247,248,5,102,0,0,248,22,1,0,0,0,249,250,5,101,0,0,250,251,5,
  	108,0,0,251,252,5,115,0,0,252,253,5,101,0,0,253,24,1,0,0,0,254,255,5,
  	102,0,0,255,256,5,111,0,0,256,257,5,114,0,0,257,26,1,0,0,0,258,259,5,
  	46,0,0,259,260,5,46,0,0,260,28,1,0,0,0,261,262,5,119,0,0,262,263,5,104,
  	0,0,263,264,5,105,0,0,264,265,5,108,0,0,265,266,5,101,0,0,266,30,1,0,
  	0,0,267,268,5,100,0,0,268,269,5,111,0,0,269,32,1,0,0,0,270,271,5,114,
  	0,0,271,272,5,101,0,0,272,273,5,116,0,0,273,274,5,117,0,0,274,275,5,114,
  	0,0,275,276,5,110,0,0,276,34,1,0,0,0,277,278,5,99,0,0,278,279,5,111,0,
  	0,279,280,5,110,0,0,280,281,5,116,0,0,281,282,5,105,0,0,282,283,5,110,
  	0,0,283,284,5,117,0,0,284,285,5,101,0,0,285,36,1,0,0,0,286,287,5,115,
  	0,0,287,288,5,119,0,0,288,289,5,105,0,0,289,290,5,116,0,0,290,291,5,99,
  	0,0,291,292,5,104,0,0,292,38,1,0,0,0,293,294,5,99,0,0,294,295,5,97,0,
  	0,295,296,5,115,0,0,296,297,5,101,0,0,297,40,1,0,0,0,298,299,5,100,0,
  	0,299,300,5,101,0,0,300,301,5,102,0,0,301,302,5,97,0,0,302,303,5,117,
  	0,0,303,304,5,108,0,0,304,305,5,116,0,0,305,42,1,0,0,0,306,307,5,98,0,
  	0,307,308,5,114,0,0,308,309,5,101,0,0,309,310,5,97,0,0,310,311,5,107,
  	0,0,311,44,1,0,0,0,312,313,5,43,0,0,313,314,5,61,0,0,314,46,1,0,0,0,315,
  	316,5,45,0,0,316,317,5,61,0,0,317,48,1,0,0,0,318,319,5,42,0,0,319,320,
  	5,61,0,0,320,50,1,0,0,0,321,322,5,47,0,0,322,323,5,61,0,0,323,52,1,0,
  	0,0,324,325,5,37,0,0,325,326,5,61,0,0,326,54,1,0,0,0,327,328,5,60,0,0,
  	328,329,5,60,0,0,329,330,5,61,0,0,330,56,1,0,0,0,331,332,5,62,0,0,332,
  	333,5,62,0,0,333,334,5,61,0,0,334,58,1,0,0,0,335,336,5,60,0,0,336,337,
  	5,60,0,0,337,60,1,0,0,0,338,339,5,62,0,0,339,340,5,62,0,0,340,62,1,0,
  	0,0,341,342,5,43,0,0,342,343,5,43,0,0,343,64,1,0,0,0,344,345,5,45,0,0,
  	345,346,5,45,0,0,346,66,1,0,0,0,347,348,5,58,0,0,348,349,5,58,0,0,349,
  	68,1,0,0,0,350,351,5,59,0,0,351,70,1,0,0,0,352,353,5,44,0,0,353,72,1,
  	0,0,0,354,355,5,58,0,0,355,74,1,0,0,0,356,357,5,40,0,0,357,76,1,0,0,0,
  	358,359,5,41,0,0,359,78,1,0,0,0,360,361,5,123,0,0,361,80,1,0,0,0,362,
  	363,5,125,0,0,363,82,1,0,0,0,364,365,5,91,0,0,365,84,1,0,0,0,366,367,
  	5,93,0,0,367,86,1,0,0,0,368,369,5,46,0,0,369,88,1,0,0,0,370,371,5,33,
  	0,0,371,90,1,0,0,0,372,373,5,61,0,0,373,92,1,0,0,0,374,375,5,34,0,0,375,
  	94,1,0,0,0,376,377,5,63,0,0,377,96,1,0,0,0,378,379,5,38,0,0,379,98,1,
  	0,0,0,380,381,5,38,0,0,381,382,5,61,0,0,382,100,1,0,0,0,383,384,5,124,
  	0,0,384,102,1,0,0,0,385,386,5,124,0,0,386,387,5,61,0,0,387,104,1,0,0,
  	0,388,389,5,94,0,0,389,106,1,0,0,0,390,391,5,94,0,0,391,392,5,61,0,0,
  	392,108,1,0,0,0,393,394,5,126,0,0,394,110,1,0,0,0,395,396,5,39,0,0,396,
  	112,1,0,0,0,397,398,5,35,0,0,398,114,1,0,0,0,399,400,5,92,0,0,400,116,
  	1,0,0,0,401,402,5,60,0,0,402,118,1,0,0,0,403,404,5,60,0,0,404,405,5,61,
  	0,0,405,120,1,0,0,0,406,407,5,62,0,0,407,122,1,0,0,0,408,409,5,62,0,0,
  	409,410,5,61,0,0,410,124,1,0,0,0,411,412,5,61,0,0,412,413,5,61,0,0,413,
  	126,1,0,0,0,414,415,5,33,0,0,415,416,5,61,0,0,416,128,1,0,0,0,417,418,
  	5,38,0,0,418,419,5,38,0,0,419,130,1,0,0,0,420,421,5,124,0,0,421,422,5,
  	124,0,0,422,132,1,0,0,0,423,424,5,37,0,0,424,134,1,0,0,0,425,426,5,95,
  	0,0,426,136,1,0,0,0,427,428,5,64,0,0,428,138,1,0,0,0,429,430,5,43,0,0,
  	430,140,1,0,0,0,431,432,5,45,0,0,432,142,1,0,0,0,433,434,5,47,0,0,434,
  	144,1,0,0,0,435,436,5,42,0,0,436,146,1,0,0,0,437,438,5,45,0,0,438,439,
  	5,62,0,0,439,148,1,0,0,0,440,441,2,48,57,0,441,150,1,0,0,0,442,444,3,
  	149,74,0,443,442,1,0,0,0,444,445,1,0,0,0,445,443,1,0,0,0,445,446,1,0,
  	0,0,446,152,1,0,0,0,447,449,3,149,74,0,448,447,1,0,0,0,449,450,1,0,0,
  	0,450,448,1,0,0,0,450,451,1,0,0,0,451,452,1,0,0,0,452,453,7,0,0,0,453,
  	154,1,0,0,0,454,455,5,47,0,0,455,456,5,47,0,0,456,460,1,0,0,0,457,459,
  	9,0,0,0,458,457,1,0,0,0,459,462,1,0,0,0,460,461,1,0,0,0,460,458,1,0,0,
  	0,461,463,1,0,0,0,462,460,1,0,0,0,463,464,5,10,0,0,464,465,1,0,0,0,465,
  	466,6,77,0,0,466,156,1,0,0,0,467,468,5,47,0,0,468,469,5,42,0,0,469,473,
  	1,0,0,0,470,472,9,0,0,0,471,470,1,0,0,0,472,475,1,0,0,0,473,474,1,0,0,
  	0,473,471,1,0,0,0,474,476,1,0,0,0,475,473,1,0,0,0,476,477,5,42,0,0,477,
  	478,5,47,0,0,478,479,1,0,0,0,479,480,6,78,0,0,480,158,1,0,0,0,481,483,
  	3,149,74,0,482,481,1,0,0,0,483,484,1,0,0,0,484,482,1,0,0,0,484,485,1,
  	0,0,0,485,486,1,0,0,0,486,490,3,87,43,0,487,489,3,149,74,0,488,487,1,
  	0,0,0,489,492,1,0,0,0,490,488,1,0,0,0,490,491,1,0,0,0,491,494,1,0,0,0,
  	492,490,1,0,0,0,493,495,3,161,80,0,494,493,1,0,0,0,494,495,1,0,0,0,495,
  	496,1,0,0,0,496,497,5,102,0,0,497,521,1,0,0,0,498,502,3,87,43,0,499,501,
  	3,149,74,0,500,499,1,0,0,0,501,504,1,0,0,0,502,500,1,0,0,0,502,503,1,
  	0,0,0,503,506,1,0,0,0,504,502,1,0,0,0,505,507,3,161,80,0,506,505,1,0,
  	0,0,506,507,1,0,0,0,507,508,1,0,0,0,508,509,5,102,0,0,509,521,1,0,0,0,
  	510,512,3,149,74,0,511,510,1,0,0,0,512,513,1,0,0,0,513,511,1,0,0,0,513,
  	514,1,0,0,0,514,516,1,0,0,0,515,517,3,161,80,0,516,515,1,0,0,0,516,517,
  	1,0,0,0,517,518,1,0,0,0,518,519,5,102,0,0,519,521,1,0,0,0,520,482,1,0,
  	0,0,520,498,1,0,0,0,520,511,1,0,0,0,521,160,1,0,0,0,522,524,7,1,0,0,523,
  	525,7,2,0,0,524,523,1,0,0,0,524,525,1,0,0,0,525,527,1,0,0,0,526,528,3,
  	149,74,0,527,526,1,0,0,0,528,529,1,0,0,0,529,527,1,0,0,0,529,530,1,0,
  	0,0,530,162,1,0,0,0,531,533,3,149,74,0,532,531,1,0,0,0,533,534,1,0,0,
  	0,534,532,1,0,0,0,534,535,1,0,0,0,535,536,1,0,0,0,536,540,3,87,43,0,537,
  	539,3,149,74,0,538,537,1,0,0,0,539,542,1,0,0,0,540,538,1,0,0,0,540,541,
  	1,0,0,0,541,544,1,0,0,0,542,540,1,0,0,0,543,545,3,161,80,0,544,543,1,
  	0,0,0,544,545,1,0,0,0,545,559,1,0,0,0,546,547,3,87,43,0,547,549,3,149,
  	74,0,548,550,3,161,80,0,549,548,1,0,0,0,549,550,1,0,0,0,550,559,1,0,0,
  	0,551,553,3,149,74,0,552,551,1,0,0,0,553,554,1,0,0,0,554,552,1,0,0,0,
  	554,555,1,0,0,0,555,556,1,0,0,0,556,557,3,161,80,0,557,559,1,0,0,0,558,
  	532,1,0,0,0,558,546,1,0,0,0,558,552,1,0,0,0,559,164,1,0,0,0,560,561,5,
  	48,0,0,561,565,5,120,0,0,562,564,7,3,0,0,563,562,1,0,0,0,564,567,1,0,
  	0,0,565,563,1,0,0,0,565,566,1,0,0,0,566,569,1,0,0,0,567,565,1,0,0,0,568,
  	570,7,0,0,0,569,568,1,0,0,0,569,570,1,0,0,0,570,166,1,0,0,0,571,572,7,
  	4,0,0,572,168,1,0,0,0,573,575,5,95,0,0,574,573,1,0,0,0,575,578,1,0,0,
  	0,576,574,1,0,0,0,576,577,1,0,0,0,577,579,1,0,0,0,578,576,1,0,0,0,579,
  	585,3,167,83,0,580,584,3,167,83,0,581,584,3,151,75,0,582,584,5,95,0,0,
  	583,580,1,0,0,0,583,581,1,0,0,0,583,582,1,0,0,0,584,587,1,0,0,0,585,583,
  	1,0,0,0,585,586,1,0,0,0,586,170,1,0,0,0,587,585,1,0,0,0,588,590,7,5,0,
  	0,589,588,1,0,0,0,590,591,1,0,0,0,591,589,1,0,0,0,591,592,1,0,0,0,592,
  	593,1,0,0,0,593,594,6,85,0,0,594,172,1,0,0,0,27,0,445,450,460,473,484,
  	490,494,502,506,513,516,520,524,529,534,540,544,549,554,558,565,569,576,
  	583,585,591,1,0,1,0
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
