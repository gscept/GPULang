

// include std container
#include <vector>
#include <typeinfo>
#include <iostream>
#include <string>
#include <stack>
#include <tuple>

#include "gpulangtoken.h"
#include "ast/alias.h"
#include "ast/annotation.h"
#include "ast/effect.h"
#include "ast/enumeration.h"
#include "ast/function.h"
#include "ast/program.h"
#include "ast/renderstate.h"
#include "ast/samplerstate.h"
#include "ast/state.h"
#include "ast/structure.h"
#include "ast/symbol.h"
#include "ast/variable.h"
#include "ast/statements/breakstatement.h"
#include "ast/statements/continuestatement.h"
#include "ast/statements/expressionstatement.h"
#include "ast/statements/forstatement.h"
#include "ast/statements/ifstatement.h"
#include "ast/statements/returnstatement.h"
#include "ast/statements/scopestatement.h"
#include "ast/statements/statement.h"
#include "ast/statements/switchstatement.h"
#include "ast/statements/whilestatement.h"
#include "ast/expressions/accessexpression.h"
#include "ast/expressions/arrayindexexpression.h"
#include "ast/expressions/binaryexpression.h"
#include "ast/expressions/boolexpression.h"
#include "ast/expressions/callexpression.h"
#include "ast/expressions/commaexpression.h"
#include "ast/expressions/expression.h"
#include "ast/expressions/floatexpression.h"
#include "ast/expressions/initializerexpression.h"
#include "ast/expressions/intexpression.h"
#include "ast/expressions/stringexpression.h"
#include "ast/expressions/symbolexpression.h"
#include "ast/expressions/ternaryexpression.h"
#include "ast/expressions/uintexpression.h"
#include "ast/expressions/unaryexpression.h"
#include "util.h"

using namespace GPULang;



// Generated from antlr4/grammar/GPULang.g4 by ANTLR 4.13.2


#include "GPULangListener.h"

#include "GPULangParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct GPULangParserStaticData final {
  GPULangParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  GPULangParserStaticData(const GPULangParserStaticData&) = delete;
  GPULangParserStaticData(GPULangParserStaticData&&) = delete;
  GPULangParserStaticData& operator=(const GPULangParserStaticData&) = delete;
  GPULangParserStaticData& operator=(GPULangParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag gpulangParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
std::unique_ptr<GPULangParserStaticData> gpulangParserStaticData = nullptr;

void gpulangParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (gpulangParserStaticData != nullptr) {
    return;
  }
#else
  assert(gpulangParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<GPULangParserStaticData>(
    std::vector<std::string>{
      "string", "boolean", "preprocess", "entry", "effect", "alias", "annotation", 
      "attribute", "typeDeclaration", "variables", "structureDeclaration", 
      "structure", "enumeration", "parameter", "functionDeclaration", "codeblock", 
      "function", "program", "state", "statement", "expressionStatement", 
      "ifStatement", "forStatement", "forRangeStatement", "whileStatement", 
      "scopeStatement", "returnStatement", "continueStatement", "switchStatement", 
      "breakStatement", "expression", "commaExpression", "assignmentExpression", 
      "logicalOrExpression", "logicalAndExpression", "orExpression", "xorExpression", 
      "andExpression", "equivalencyExpression", "relationalExpression", 
      "shiftExpression", "addSubtractExpression", "multiplyDivideExpression", 
      "prefixExpression", "suffixExpression", "namespaceExpression", "binaryexpatom", 
      "initializerExpression"
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
  	4,1,84,902,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,7,
  	35,2,36,7,36,2,37,7,37,2,38,7,38,2,39,7,39,2,40,7,40,2,41,7,41,2,42,7,
  	42,2,43,7,43,2,44,7,44,2,45,7,45,2,46,7,46,2,47,7,47,1,0,1,0,1,0,5,0,
  	100,8,0,10,0,12,0,103,9,0,1,0,1,0,1,0,1,0,5,0,109,8,0,10,0,12,0,112,9,
  	0,1,0,3,0,115,8,0,1,1,1,1,1,1,1,1,3,1,121,8,1,1,2,1,2,1,2,1,2,1,2,1,2,
  	1,2,5,2,130,8,2,10,2,12,2,133,9,2,1,2,1,2,1,3,1,3,1,3,1,3,1,4,1,4,1,4,
  	1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,
  	4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,5,4,172,8,4,10,4,12,4,175,9,
  	4,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,7,1,7,1,7,
  	1,7,1,7,1,7,1,7,1,7,3,7,199,8,7,1,8,1,8,1,8,1,8,1,8,1,8,3,8,207,8,8,1,
  	8,1,8,1,8,5,8,212,8,8,10,8,12,8,215,9,8,1,8,1,8,1,8,1,9,1,9,1,9,5,9,223,
  	8,9,10,9,12,9,226,9,9,1,9,1,9,1,9,4,9,231,8,9,11,9,12,9,232,1,9,1,9,1,
  	9,1,9,1,9,5,9,240,8,9,10,9,12,9,243,9,9,1,9,1,9,1,9,1,9,3,9,249,8,9,1,
  	9,1,9,1,9,1,9,1,9,1,9,1,9,5,9,258,8,9,10,9,12,9,261,9,9,3,9,263,8,9,1,
  	9,1,9,1,10,1,10,1,10,5,10,270,8,10,10,10,12,10,273,9,10,1,10,1,10,1,10,
  	1,10,5,10,279,8,10,10,10,12,10,282,9,10,1,10,1,10,1,10,1,11,1,11,1,11,
  	1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,3,11,299,8,11,1,11,5,11,
  	302,8,11,10,11,12,11,305,9,11,1,11,1,11,1,11,1,11,5,11,311,8,11,10,11,
  	12,11,314,9,11,1,11,1,11,1,11,1,12,1,12,1,12,1,12,1,12,1,12,1,12,3,12,
  	326,8,12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,3,12,335,8,12,1,12,1,12,1,
  	12,1,12,1,13,1,13,1,13,5,13,344,8,13,10,13,12,13,347,9,13,1,13,1,13,1,
  	13,1,13,1,13,1,13,1,13,1,13,1,13,3,13,358,8,13,1,13,1,13,1,14,1,14,1,
  	14,5,14,365,8,14,10,14,12,14,368,9,14,1,14,1,14,1,14,1,14,1,14,1,14,1,
  	14,1,14,1,14,5,14,379,8,14,10,14,12,14,382,9,14,3,14,384,8,14,1,14,1,
  	14,1,14,1,14,1,15,1,15,5,15,392,8,15,10,15,12,15,395,9,15,1,15,1,15,3,
  	15,399,8,15,1,16,1,16,1,16,1,16,1,16,1,16,1,17,1,17,1,17,5,17,410,8,17,
  	10,17,12,17,413,9,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,5,17,423,
  	8,17,10,17,12,17,426,9,17,1,17,1,17,1,17,1,18,1,18,1,18,1,18,3,18,435,
  	8,18,1,18,1,18,1,18,1,18,1,18,1,18,1,18,5,18,444,8,18,10,18,12,18,447,
  	9,18,1,18,1,18,1,18,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,
  	1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,
  	1,19,1,19,1,19,1,19,3,19,480,8,19,1,20,1,20,1,20,1,21,1,21,1,21,1,21,
  	1,21,1,21,1,21,1,21,1,21,1,21,1,21,1,21,3,21,497,8,21,1,21,1,21,1,22,
  	1,22,1,22,1,22,1,22,1,22,3,22,507,8,22,1,22,1,22,1,22,1,22,3,22,513,8,
  	22,1,22,1,22,1,22,1,22,3,22,519,8,22,1,22,1,22,1,22,1,22,1,22,1,23,1,
  	23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,24,1,24,1,
  	24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,
  	24,1,24,1,24,1,24,1,24,1,24,3,24,561,8,24,1,25,1,25,1,25,1,25,1,25,1,
  	25,1,25,1,25,1,25,5,25,572,8,25,10,25,12,25,575,9,25,1,25,1,25,1,25,1,
  	26,1,26,1,26,1,26,1,26,3,26,585,8,26,1,26,1,26,1,26,1,27,1,27,1,27,1,
  	27,1,27,1,28,1,28,1,28,1,28,1,28,1,28,1,28,1,28,1,28,1,28,1,28,1,28,1,
  	28,1,28,3,28,609,8,28,5,28,611,8,28,10,28,12,28,614,9,28,1,28,1,28,1,
  	28,1,28,1,28,3,28,621,8,28,3,28,623,8,28,1,28,1,28,1,28,1,29,1,29,1,29,
  	1,29,1,29,1,30,1,30,1,30,1,31,1,31,1,31,1,31,1,31,1,31,1,31,5,31,643,
  	8,31,10,31,12,31,646,9,31,1,32,1,32,1,32,1,32,1,32,1,32,1,32,1,32,1,32,
  	1,32,1,32,1,32,1,32,5,32,661,8,32,10,32,12,32,664,9,32,1,33,1,33,1,33,
  	1,33,1,33,1,33,1,33,5,33,673,8,33,10,33,12,33,676,9,33,1,34,1,34,1,34,
  	1,34,1,34,1,34,1,34,5,34,685,8,34,10,34,12,34,688,9,34,1,35,1,35,1,35,
  	1,35,1,35,1,35,1,35,5,35,697,8,35,10,35,12,35,700,9,35,1,36,1,36,1,36,
  	1,36,1,36,1,36,1,36,5,36,709,8,36,10,36,12,36,712,9,36,1,37,1,37,1,37,
  	1,37,1,37,1,37,1,37,5,37,721,8,37,10,37,12,37,724,9,37,1,38,1,38,1,38,
  	1,38,1,38,1,38,1,38,5,38,733,8,38,10,38,12,38,736,9,38,1,39,1,39,1,39,
  	1,39,1,39,1,39,1,39,5,39,745,8,39,10,39,12,39,748,9,39,1,40,1,40,1,40,
  	1,40,1,40,1,40,1,40,5,40,757,8,40,10,40,12,40,760,9,40,1,41,1,41,1,41,
  	1,41,1,41,1,41,1,41,5,41,769,8,41,10,41,12,41,772,9,41,1,42,1,42,1,42,
  	1,42,1,42,1,42,1,42,5,42,781,8,42,10,42,12,42,784,9,42,1,43,1,43,5,43,
  	788,8,43,10,43,12,43,791,9,43,1,43,1,43,1,43,1,44,1,44,1,44,5,44,799,
  	8,44,10,44,12,44,802,9,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,
  	1,44,1,44,1,44,5,44,816,8,44,10,44,12,44,819,9,44,3,44,821,8,44,1,44,
  	1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,
  	1,44,1,44,3,44,840,8,44,1,44,1,44,5,44,844,8,44,10,44,12,44,847,9,44,
  	3,44,849,8,44,1,45,1,45,1,45,1,45,1,45,1,46,1,46,1,46,1,46,1,46,1,46,
  	1,46,1,46,1,46,1,46,1,46,1,46,1,46,1,46,1,46,1,46,1,46,1,46,1,46,1,46,
  	1,46,1,46,1,46,1,46,1,46,1,46,3,46,882,8,46,1,47,1,47,1,47,1,47,1,47,
  	1,47,1,47,1,47,5,47,892,8,47,10,47,12,47,895,9,47,3,47,897,8,47,1,47,
  	1,47,1,47,1,47,2,131,173,0,48,0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,
  	30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,66,68,70,72,74,
  	76,78,80,82,84,86,88,90,92,94,0,11,1,0,47,47,1,0,56,56,1,0,40,41,5,0,
  	23,29,46,46,50,50,52,52,54,54,1,0,63,64,1,0,59,62,1,0,30,31,1,0,70,71,
  	2,0,67,67,72,73,5,0,32,33,45,45,55,55,70,71,73,73,1,0,32,33,951,0,114,
  	1,0,0,0,2,120,1,0,0,0,4,131,1,0,0,0,6,136,1,0,0,0,8,173,1,0,0,0,10,176,
  	1,0,0,0,12,183,1,0,0,0,14,198,1,0,0,0,16,213,1,0,0,0,18,224,1,0,0,0,20,
  	271,1,0,0,0,22,286,1,0,0,0,24,318,1,0,0,0,26,345,1,0,0,0,28,366,1,0,0,
  	0,30,398,1,0,0,0,32,400,1,0,0,0,34,411,1,0,0,0,36,434,1,0,0,0,38,479,
  	1,0,0,0,40,481,1,0,0,0,42,484,1,0,0,0,44,500,1,0,0,0,46,525,1,0,0,0,48,
  	560,1,0,0,0,50,562,1,0,0,0,52,579,1,0,0,0,54,589,1,0,0,0,56,594,1,0,0,
  	0,58,627,1,0,0,0,60,632,1,0,0,0,62,635,1,0,0,0,64,647,1,0,0,0,66,665,
  	1,0,0,0,68,677,1,0,0,0,70,689,1,0,0,0,72,701,1,0,0,0,74,713,1,0,0,0,76,
  	725,1,0,0,0,78,737,1,0,0,0,80,749,1,0,0,0,82,761,1,0,0,0,84,773,1,0,0,
  	0,86,789,1,0,0,0,88,848,1,0,0,0,90,850,1,0,0,0,92,881,1,0,0,0,94,883,
  	1,0,0,0,96,101,5,47,0,0,97,98,8,0,0,0,98,100,6,0,-1,0,99,97,1,0,0,0,100,
  	103,1,0,0,0,101,99,1,0,0,0,101,102,1,0,0,0,102,104,1,0,0,0,103,101,1,
  	0,0,0,104,115,5,47,0,0,105,110,5,56,0,0,106,107,8,1,0,0,107,109,6,0,-1,
  	0,108,106,1,0,0,0,109,112,1,0,0,0,110,108,1,0,0,0,110,111,1,0,0,0,111,
  	113,1,0,0,0,112,110,1,0,0,0,113,115,5,56,0,0,114,96,1,0,0,0,114,105,1,
  	0,0,0,115,1,1,0,0,0,116,117,5,1,0,0,117,121,6,1,-1,0,118,119,5,2,0,0,
  	119,121,6,1,-1,0,120,116,1,0,0,0,120,118,1,0,0,0,121,3,1,0,0,0,122,123,
  	6,2,-1,0,123,124,5,3,0,0,124,125,5,75,0,0,125,126,3,0,0,0,126,127,6,2,
  	-1,0,127,130,1,0,0,0,128,130,9,0,0,0,129,122,1,0,0,0,129,128,1,0,0,0,
  	130,133,1,0,0,0,131,132,1,0,0,0,131,129,1,0,0,0,132,134,1,0,0,0,133,131,
  	1,0,0,0,134,135,5,0,0,1,135,5,1,0,0,0,136,137,3,8,4,0,137,138,6,3,-1,
  	0,138,139,5,0,0,1,139,7,1,0,0,0,140,141,3,10,5,0,141,142,5,35,0,0,142,
  	143,6,4,-1,0,143,172,1,0,0,0,144,145,3,28,14,0,145,146,5,35,0,0,146,147,
  	6,4,-1,0,147,172,1,0,0,0,148,149,3,32,16,0,149,150,6,4,-1,0,150,172,1,
  	0,0,0,151,152,3,18,9,0,152,153,5,35,0,0,153,154,6,4,-1,0,154,172,1,0,
  	0,0,155,156,3,22,11,0,156,157,5,35,0,0,157,158,6,4,-1,0,158,172,1,0,0,
  	0,159,160,3,24,12,0,160,161,5,35,0,0,161,162,6,4,-1,0,162,172,1,0,0,0,
  	163,164,3,36,18,0,164,165,5,35,0,0,165,166,6,4,-1,0,166,172,1,0,0,0,167,
  	168,3,34,17,0,168,169,5,35,0,0,169,170,6,4,-1,0,170,172,1,0,0,0,171,140,
  	1,0,0,0,171,144,1,0,0,0,171,148,1,0,0,0,171,151,1,0,0,0,171,155,1,0,0,
  	0,171,159,1,0,0,0,171,163,1,0,0,0,171,167,1,0,0,0,172,175,1,0,0,0,173,
  	174,1,0,0,0,173,171,1,0,0,0,174,9,1,0,0,0,175,173,1,0,0,0,176,177,5,4,
  	0,0,177,178,5,83,0,0,178,179,5,5,0,0,179,180,5,83,0,0,180,181,6,5,-1,
  	0,181,182,6,5,-1,0,182,11,1,0,0,0,183,184,5,69,0,0,184,185,5,83,0,0,185,
  	186,5,38,0,0,186,187,3,60,30,0,187,188,5,39,0,0,188,189,6,6,-1,0,189,
  	13,1,0,0,0,190,191,5,83,0,0,191,192,5,38,0,0,192,193,3,60,30,0,193,194,
  	5,39,0,0,194,195,6,7,-1,0,195,199,1,0,0,0,196,197,5,83,0,0,197,199,6,
  	7,-1,0,198,190,1,0,0,0,198,196,1,0,0,0,199,15,1,0,0,0,200,201,5,73,0,
  	0,201,212,6,8,-1,0,202,203,5,42,0,0,203,206,6,8,-1,0,204,205,5,75,0,0,
  	205,207,6,8,-1,0,206,204,1,0,0,0,206,207,1,0,0,0,207,208,1,0,0,0,208,
  	212,5,43,0,0,209,210,5,83,0,0,210,212,6,8,-1,0,211,200,1,0,0,0,211,202,
  	1,0,0,0,211,209,1,0,0,0,212,215,1,0,0,0,213,211,1,0,0,0,213,214,1,0,0,
  	0,214,216,1,0,0,0,215,213,1,0,0,0,216,217,5,83,0,0,217,218,6,8,-1,0,218,
  	17,1,0,0,0,219,220,3,12,6,0,220,221,6,9,-1,0,221,223,1,0,0,0,222,219,
  	1,0,0,0,223,226,1,0,0,0,224,222,1,0,0,0,224,225,1,0,0,0,225,230,1,0,0,
  	0,226,224,1,0,0,0,227,228,3,14,7,0,228,229,6,9,-1,0,229,231,1,0,0,0,230,
  	227,1,0,0,0,231,232,1,0,0,0,232,230,1,0,0,0,232,233,1,0,0,0,233,234,1,
  	0,0,0,234,235,5,83,0,0,235,241,6,9,-1,0,236,237,5,36,0,0,237,238,5,83,
  	0,0,238,240,6,9,-1,0,239,236,1,0,0,0,240,243,1,0,0,0,241,239,1,0,0,0,
  	241,242,1,0,0,0,242,248,1,0,0,0,243,241,1,0,0,0,244,245,5,37,0,0,245,
  	246,3,16,8,0,246,247,6,9,-1,0,247,249,1,0,0,0,248,244,1,0,0,0,248,249,
  	1,0,0,0,249,262,1,0,0,0,250,251,5,46,0,0,251,252,3,64,32,0,252,259,6,
  	9,-1,0,253,254,5,36,0,0,254,255,3,64,32,0,255,256,6,9,-1,0,256,258,1,
  	0,0,0,257,253,1,0,0,0,258,261,1,0,0,0,259,257,1,0,0,0,259,260,1,0,0,0,
  	260,263,1,0,0,0,261,259,1,0,0,0,262,250,1,0,0,0,262,263,1,0,0,0,263,264,
  	1,0,0,0,264,265,6,9,-1,0,265,19,1,0,0,0,266,267,3,12,6,0,267,268,6,10,
  	-1,0,268,270,1,0,0,0,269,266,1,0,0,0,270,273,1,0,0,0,271,269,1,0,0,0,
  	271,272,1,0,0,0,272,274,1,0,0,0,273,271,1,0,0,0,274,280,5,6,0,0,275,276,
  	3,14,7,0,276,277,6,10,-1,0,277,279,1,0,0,0,278,275,1,0,0,0,279,282,1,
  	0,0,0,280,278,1,0,0,0,280,281,1,0,0,0,281,283,1,0,0,0,282,280,1,0,0,0,
  	283,284,5,83,0,0,284,285,6,10,-1,0,285,21,1,0,0,0,286,287,3,20,10,0,287,
  	288,6,11,-1,0,288,312,5,40,0,0,289,290,5,83,0,0,290,291,6,11,-1,0,291,
  	303,5,37,0,0,292,293,5,73,0,0,293,302,6,11,-1,0,294,295,5,42,0,0,295,
  	298,6,11,-1,0,296,297,5,75,0,0,297,299,6,11,-1,0,298,296,1,0,0,0,298,
  	299,1,0,0,0,299,300,1,0,0,0,300,302,5,43,0,0,301,292,1,0,0,0,301,294,
  	1,0,0,0,302,305,1,0,0,0,303,301,1,0,0,0,303,304,1,0,0,0,304,306,1,0,0,
  	0,305,303,1,0,0,0,306,307,5,83,0,0,307,308,6,11,-1,0,308,309,5,35,0,0,
  	309,311,6,11,-1,0,310,289,1,0,0,0,311,314,1,0,0,0,312,310,1,0,0,0,312,
  	313,1,0,0,0,313,315,1,0,0,0,314,312,1,0,0,0,315,316,5,41,0,0,316,317,
  	6,11,-1,0,317,23,1,0,0,0,318,319,5,7,0,0,319,320,5,83,0,0,320,325,6,12,
  	-1,0,321,322,5,37,0,0,322,323,3,16,8,0,323,324,6,12,-1,0,324,326,1,0,
  	0,0,325,321,1,0,0,0,325,326,1,0,0,0,326,327,1,0,0,0,327,328,5,40,0,0,
  	328,329,5,83,0,0,329,334,6,12,-1,0,330,331,5,46,0,0,331,332,3,60,30,0,
  	332,333,6,12,-1,0,333,335,1,0,0,0,334,330,1,0,0,0,334,335,1,0,0,0,335,
  	336,1,0,0,0,336,337,6,12,-1,0,337,338,5,41,0,0,338,339,6,12,-1,0,339,
  	25,1,0,0,0,340,341,3,14,7,0,341,342,6,13,-1,0,342,344,1,0,0,0,343,340,
  	1,0,0,0,344,347,1,0,0,0,345,343,1,0,0,0,345,346,1,0,0,0,346,348,1,0,0,
  	0,347,345,1,0,0,0,348,349,5,83,0,0,349,350,6,13,-1,0,350,351,5,37,0,0,
  	351,352,3,16,8,0,352,357,6,13,-1,0,353,354,5,46,0,0,354,355,3,64,32,0,
  	355,356,6,13,-1,0,356,358,1,0,0,0,357,353,1,0,0,0,357,358,1,0,0,0,358,
  	359,1,0,0,0,359,360,6,13,-1,0,360,27,1,0,0,0,361,362,3,14,7,0,362,363,
  	6,14,-1,0,363,365,1,0,0,0,364,361,1,0,0,0,365,368,1,0,0,0,366,364,1,0,
  	0,0,366,367,1,0,0,0,367,369,1,0,0,0,368,366,1,0,0,0,369,370,5,83,0,0,
  	370,371,6,14,-1,0,371,383,5,38,0,0,372,373,3,26,13,0,373,380,6,14,-1,
  	0,374,375,5,36,0,0,375,376,3,26,13,0,376,377,6,14,-1,0,377,379,1,0,0,
  	0,378,374,1,0,0,0,379,382,1,0,0,0,380,378,1,0,0,0,380,381,1,0,0,0,381,
  	384,1,0,0,0,382,380,1,0,0,0,383,372,1,0,0,0,383,384,1,0,0,0,384,385,1,
  	0,0,0,385,386,5,39,0,0,386,387,3,16,8,0,387,388,6,14,-1,0,388,29,1,0,
  	0,0,389,393,5,40,0,0,390,392,3,30,15,0,391,390,1,0,0,0,392,395,1,0,0,
  	0,393,391,1,0,0,0,393,394,1,0,0,0,394,396,1,0,0,0,395,393,1,0,0,0,396,
  	399,5,41,0,0,397,399,8,2,0,0,398,389,1,0,0,0,398,397,1,0,0,0,399,31,1,
  	0,0,0,400,401,3,28,14,0,401,402,6,16,-1,0,402,403,6,16,-1,0,403,404,3,
  	50,25,0,404,405,6,16,-1,0,405,33,1,0,0,0,406,407,3,12,6,0,407,408,6,17,
  	-1,0,408,410,1,0,0,0,409,406,1,0,0,0,410,413,1,0,0,0,411,409,1,0,0,0,
  	411,412,1,0,0,0,412,414,1,0,0,0,413,411,1,0,0,0,414,415,5,8,0,0,415,416,
  	5,83,0,0,416,417,6,17,-1,0,417,424,5,40,0,0,418,419,3,60,30,0,419,420,
  	6,17,-1,0,420,421,5,35,0,0,421,423,1,0,0,0,422,418,1,0,0,0,423,426,1,
  	0,0,0,424,422,1,0,0,0,424,425,1,0,0,0,425,427,1,0,0,0,426,424,1,0,0,0,
  	427,428,5,41,0,0,428,429,6,17,-1,0,429,35,1,0,0,0,430,431,5,9,0,0,431,
  	435,6,18,-1,0,432,433,5,10,0,0,433,435,6,18,-1,0,434,430,1,0,0,0,434,
  	432,1,0,0,0,435,436,1,0,0,0,436,437,5,83,0,0,437,438,6,18,-1,0,438,445,
  	5,40,0,0,439,440,3,60,30,0,440,441,6,18,-1,0,441,442,5,35,0,0,442,444,
  	1,0,0,0,443,439,1,0,0,0,444,447,1,0,0,0,445,443,1,0,0,0,445,446,1,0,0,
  	0,446,448,1,0,0,0,447,445,1,0,0,0,448,449,5,41,0,0,449,450,6,18,-1,0,
  	450,37,1,0,0,0,451,452,3,42,21,0,452,453,6,19,-1,0,453,480,1,0,0,0,454,
  	455,3,50,25,0,455,456,6,19,-1,0,456,480,1,0,0,0,457,458,3,44,22,0,458,
  	459,6,19,-1,0,459,480,1,0,0,0,460,461,3,48,24,0,461,462,6,19,-1,0,462,
  	480,1,0,0,0,463,464,3,56,28,0,464,465,6,19,-1,0,465,480,1,0,0,0,466,467,
  	3,52,26,0,467,468,6,19,-1,0,468,480,1,0,0,0,469,470,3,54,27,0,470,471,
  	6,19,-1,0,471,480,1,0,0,0,472,473,3,58,29,0,473,474,6,19,-1,0,474,480,
  	1,0,0,0,475,476,3,40,20,0,476,477,5,35,0,0,477,478,6,19,-1,0,478,480,
  	1,0,0,0,479,451,1,0,0,0,479,454,1,0,0,0,479,457,1,0,0,0,479,460,1,0,0,
  	0,479,463,1,0,0,0,479,466,1,0,0,0,479,469,1,0,0,0,479,472,1,0,0,0,479,
  	475,1,0,0,0,480,39,1,0,0,0,481,482,3,60,30,0,482,483,6,20,-1,0,483,41,
  	1,0,0,0,484,485,5,11,0,0,485,486,6,21,-1,0,486,487,5,38,0,0,487,488,3,
  	60,30,0,488,489,6,21,-1,0,489,490,5,39,0,0,490,491,3,38,19,0,491,496,
  	6,21,-1,0,492,493,5,12,0,0,493,494,3,38,19,0,494,495,6,21,-1,0,495,497,
  	1,0,0,0,496,492,1,0,0,0,496,497,1,0,0,0,497,498,1,0,0,0,498,499,6,21,
  	-1,0,499,43,1,0,0,0,500,501,5,13,0,0,501,502,6,22,-1,0,502,506,5,38,0,
  	0,503,504,3,18,9,0,504,505,6,22,-1,0,505,507,1,0,0,0,506,503,1,0,0,0,
  	506,507,1,0,0,0,507,508,1,0,0,0,508,512,5,35,0,0,509,510,3,60,30,0,510,
  	511,6,22,-1,0,511,513,1,0,0,0,512,509,1,0,0,0,512,513,1,0,0,0,513,514,
  	1,0,0,0,514,518,5,35,0,0,515,516,3,60,30,0,516,517,6,22,-1,0,517,519,
  	1,0,0,0,518,515,1,0,0,0,518,519,1,0,0,0,519,520,1,0,0,0,520,521,5,39,
  	0,0,521,522,3,38,19,0,522,523,6,22,-1,0,523,524,6,22,-1,0,524,45,1,0,
  	0,0,525,526,5,13,0,0,526,527,6,23,-1,0,527,528,5,38,0,0,528,529,5,83,
  	0,0,529,530,5,37,0,0,530,531,5,83,0,0,531,532,5,14,0,0,532,533,5,83,0,
  	0,533,534,5,39,0,0,534,535,3,38,19,0,535,536,6,23,-1,0,536,537,6,23,-1,
  	0,537,47,1,0,0,0,538,539,5,15,0,0,539,540,6,24,-1,0,540,541,5,38,0,0,
  	541,542,3,60,30,0,542,543,6,24,-1,0,543,544,5,39,0,0,544,545,3,38,19,
  	0,545,546,6,24,-1,0,546,547,6,24,-1,0,547,561,1,0,0,0,548,549,5,16,0,
  	0,549,550,6,24,-1,0,550,551,3,38,19,0,551,552,6,24,-1,0,552,553,5,15,
  	0,0,553,554,5,38,0,0,554,555,3,60,30,0,555,556,6,24,-1,0,556,557,5,39,
  	0,0,557,558,5,35,0,0,558,559,6,24,-1,0,559,561,1,0,0,0,560,538,1,0,0,
  	0,560,548,1,0,0,0,561,49,1,0,0,0,562,563,5,40,0,0,563,573,6,25,-1,0,564,
  	565,3,18,9,0,565,566,5,35,0,0,566,567,6,25,-1,0,567,572,1,0,0,0,568,569,
  	3,38,19,0,569,570,6,25,-1,0,570,572,1,0,0,0,571,564,1,0,0,0,571,568,1,
  	0,0,0,572,575,1,0,0,0,573,571,1,0,0,0,573,574,1,0,0,0,574,576,1,0,0,0,
  	575,573,1,0,0,0,576,577,5,41,0,0,577,578,6,25,-1,0,578,51,1,0,0,0,579,
  	580,5,17,0,0,580,584,6,26,-1,0,581,582,3,60,30,0,582,583,6,26,-1,0,583,
  	585,1,0,0,0,584,581,1,0,0,0,584,585,1,0,0,0,585,586,1,0,0,0,586,587,5,
  	35,0,0,587,588,6,26,-1,0,588,53,1,0,0,0,589,590,5,18,0,0,590,591,6,27,
  	-1,0,591,592,5,35,0,0,592,593,6,27,-1,0,593,55,1,0,0,0,594,595,5,19,0,
  	0,595,596,6,28,-1,0,596,597,5,38,0,0,597,598,3,60,30,0,598,599,5,39,0,
  	0,599,600,6,28,-1,0,600,612,5,40,0,0,601,602,5,20,0,0,602,603,5,75,0,
  	0,603,604,5,37,0,0,604,608,6,28,-1,0,605,606,3,38,19,0,606,607,6,28,-1,
  	0,607,609,1,0,0,0,608,605,1,0,0,0,608,609,1,0,0,0,609,611,1,0,0,0,610,
  	601,1,0,0,0,611,614,1,0,0,0,612,610,1,0,0,0,612,613,1,0,0,0,613,622,1,
  	0,0,0,614,612,1,0,0,0,615,616,5,21,0,0,616,620,5,37,0,0,617,618,3,38,
  	19,0,618,619,6,28,-1,0,619,621,1,0,0,0,620,617,1,0,0,0,620,621,1,0,0,
  	0,621,623,1,0,0,0,622,615,1,0,0,0,622,623,1,0,0,0,623,624,1,0,0,0,624,
  	625,5,41,0,0,625,626,6,28,-1,0,626,57,1,0,0,0,627,628,5,22,0,0,628,629,
  	6,29,-1,0,629,630,5,35,0,0,630,631,6,29,-1,0,631,59,1,0,0,0,632,633,3,
  	62,31,0,633,634,6,30,-1,0,634,61,1,0,0,0,635,636,3,64,32,0,636,644,6,
  	31,-1,0,637,638,5,36,0,0,638,639,6,31,-1,0,639,640,3,64,32,0,640,641,
  	6,31,-1,0,641,643,1,0,0,0,642,637,1,0,0,0,643,646,1,0,0,0,644,642,1,0,
  	0,0,644,645,1,0,0,0,645,63,1,0,0,0,646,644,1,0,0,0,647,648,3,66,33,0,
  	648,662,6,32,-1,0,649,650,7,3,0,0,650,651,3,66,33,0,651,652,6,32,-1,0,
  	652,661,1,0,0,0,653,654,5,48,0,0,654,655,6,32,-1,0,655,656,3,60,30,0,
  	656,657,5,37,0,0,657,658,3,60,30,0,658,659,6,32,-1,0,659,661,1,0,0,0,
  	660,649,1,0,0,0,660,653,1,0,0,0,661,664,1,0,0,0,662,660,1,0,0,0,662,663,
  	1,0,0,0,663,65,1,0,0,0,664,662,1,0,0,0,665,666,3,68,34,0,666,674,6,33,
  	-1,0,667,668,5,66,0,0,668,669,6,33,-1,0,669,670,3,68,34,0,670,671,6,33,
  	-1,0,671,673,1,0,0,0,672,667,1,0,0,0,673,676,1,0,0,0,674,672,1,0,0,0,
  	674,675,1,0,0,0,675,67,1,0,0,0,676,674,1,0,0,0,677,678,3,70,35,0,678,
  	686,6,34,-1,0,679,680,5,65,0,0,680,681,6,34,-1,0,681,682,3,70,35,0,682,
  	683,6,34,-1,0,683,685,1,0,0,0,684,679,1,0,0,0,685,688,1,0,0,0,686,684,
  	1,0,0,0,686,687,1,0,0,0,687,69,1,0,0,0,688,686,1,0,0,0,689,690,3,72,36,
  	0,690,698,6,35,-1,0,691,692,5,51,0,0,692,693,6,35,-1,0,693,694,3,72,36,
  	0,694,695,6,35,-1,0,695,697,1,0,0,0,696,691,1,0,0,0,697,700,1,0,0,0,698,
  	696,1,0,0,0,698,699,1,0,0,0,699,71,1,0,0,0,700,698,1,0,0,0,701,702,3,
  	74,37,0,702,710,6,36,-1,0,703,704,5,53,0,0,704,705,6,36,-1,0,705,706,
  	3,74,37,0,706,707,6,36,-1,0,707,709,1,0,0,0,708,703,1,0,0,0,709,712,1,
  	0,0,0,710,708,1,0,0,0,710,711,1,0,0,0,711,73,1,0,0,0,712,710,1,0,0,0,
  	713,714,3,76,38,0,714,722,6,37,-1,0,715,716,5,49,0,0,716,717,6,37,-1,
  	0,717,718,3,76,38,0,718,719,6,37,-1,0,719,721,1,0,0,0,720,715,1,0,0,0,
  	721,724,1,0,0,0,722,720,1,0,0,0,722,723,1,0,0,0,723,75,1,0,0,0,724,722,
  	1,0,0,0,725,726,3,78,39,0,726,734,6,38,-1,0,727,728,7,4,0,0,728,729,6,
  	38,-1,0,729,730,3,78,39,0,730,731,6,38,-1,0,731,733,1,0,0,0,732,727,1,
  	0,0,0,733,736,1,0,0,0,734,732,1,0,0,0,734,735,1,0,0,0,735,77,1,0,0,0,
  	736,734,1,0,0,0,737,738,3,80,40,0,738,746,6,39,-1,0,739,740,7,5,0,0,740,
  	741,6,39,-1,0,741,742,3,80,40,0,742,743,6,39,-1,0,743,745,1,0,0,0,744,
  	739,1,0,0,0,745,748,1,0,0,0,746,744,1,0,0,0,746,747,1,0,0,0,747,79,1,
  	0,0,0,748,746,1,0,0,0,749,750,3,82,41,0,750,758,6,40,-1,0,751,752,7,6,
  	0,0,752,753,6,40,-1,0,753,754,3,82,41,0,754,755,6,40,-1,0,755,757,1,0,
  	0,0,756,751,1,0,0,0,757,760,1,0,0,0,758,756,1,0,0,0,758,759,1,0,0,0,759,
  	81,1,0,0,0,760,758,1,0,0,0,761,762,3,84,42,0,762,770,6,41,-1,0,763,764,
  	7,7,0,0,764,765,6,41,-1,0,765,766,3,84,42,0,766,767,6,41,-1,0,767,769,
  	1,0,0,0,768,763,1,0,0,0,769,772,1,0,0,0,770,768,1,0,0,0,770,771,1,0,0,
  	0,771,83,1,0,0,0,772,770,1,0,0,0,773,774,3,86,43,0,774,782,6,42,-1,0,
  	775,776,7,8,0,0,776,777,6,42,-1,0,777,778,3,86,43,0,778,779,6,42,-1,0,
  	779,781,1,0,0,0,780,775,1,0,0,0,781,784,1,0,0,0,782,780,1,0,0,0,782,783,
  	1,0,0,0,783,85,1,0,0,0,784,782,1,0,0,0,785,786,7,9,0,0,786,788,6,43,-1,
  	0,787,785,1,0,0,0,788,791,1,0,0,0,789,787,1,0,0,0,789,790,1,0,0,0,790,
  	792,1,0,0,0,791,789,1,0,0,0,792,793,3,88,44,0,793,794,6,43,-1,0,794,87,
  	1,0,0,0,795,800,3,92,46,0,796,797,7,10,0,0,797,799,6,44,-1,0,798,796,
  	1,0,0,0,799,802,1,0,0,0,800,798,1,0,0,0,800,801,1,0,0,0,801,803,1,0,0,
  	0,802,800,1,0,0,0,803,804,6,44,-1,0,804,849,1,0,0,0,805,806,3,92,46,0,
  	806,845,6,44,-1,0,807,808,5,38,0,0,808,820,6,44,-1,0,809,810,3,66,33,
  	0,810,817,6,44,-1,0,811,812,5,36,0,0,812,813,3,66,33,0,813,814,6,44,-1,
  	0,814,816,1,0,0,0,815,811,1,0,0,0,816,819,1,0,0,0,817,815,1,0,0,0,817,
  	818,1,0,0,0,818,821,1,0,0,0,819,817,1,0,0,0,820,809,1,0,0,0,820,821,1,
  	0,0,0,821,822,1,0,0,0,822,823,5,39,0,0,823,844,6,44,-1,0,824,825,5,44,
  	0,0,825,826,6,44,-1,0,826,827,3,88,44,0,827,828,6,44,-1,0,828,844,1,0,
  	0,0,829,830,5,74,0,0,830,831,6,44,-1,0,831,832,3,88,44,0,832,833,6,44,
  	-1,0,833,844,1,0,0,0,834,835,5,42,0,0,835,839,6,44,-1,0,836,837,3,60,
  	30,0,837,838,6,44,-1,0,838,840,1,0,0,0,839,836,1,0,0,0,839,840,1,0,0,
  	0,840,841,1,0,0,0,841,842,5,43,0,0,842,844,6,44,-1,0,843,807,1,0,0,0,
  	843,824,1,0,0,0,843,829,1,0,0,0,843,834,1,0,0,0,844,847,1,0,0,0,845,843,
  	1,0,0,0,845,846,1,0,0,0,846,849,1,0,0,0,847,845,1,0,0,0,848,795,1,0,0,
  	0,848,805,1,0,0,0,849,89,1,0,0,0,850,851,3,92,46,0,851,852,5,34,0,0,852,
  	853,3,92,46,0,853,854,6,45,-1,0,854,91,1,0,0,0,855,856,5,75,0,0,856,882,
  	6,46,-1,0,857,858,5,76,0,0,858,882,6,46,-1,0,859,860,5,79,0,0,860,882,
  	6,46,-1,0,861,862,5,81,0,0,862,882,6,46,-1,0,863,864,5,82,0,0,864,882,
  	6,46,-1,0,865,866,3,0,0,0,866,867,6,46,-1,0,867,882,1,0,0,0,868,869,5,
  	83,0,0,869,882,6,46,-1,0,870,871,3,2,1,0,871,872,6,46,-1,0,872,882,1,
  	0,0,0,873,874,3,94,47,0,874,875,6,46,-1,0,875,882,1,0,0,0,876,877,5,38,
  	0,0,877,878,3,60,30,0,878,879,5,39,0,0,879,880,6,46,-1,0,880,882,1,0,
  	0,0,881,855,1,0,0,0,881,857,1,0,0,0,881,859,1,0,0,0,881,861,1,0,0,0,881,
  	863,1,0,0,0,881,865,1,0,0,0,881,868,1,0,0,0,881,870,1,0,0,0,881,873,1,
  	0,0,0,881,876,1,0,0,0,882,93,1,0,0,0,883,884,5,40,0,0,884,896,6,47,-1,
  	0,885,886,3,64,32,0,886,893,6,47,-1,0,887,888,5,36,0,0,888,889,3,64,32,
  	0,889,890,6,47,-1,0,890,892,1,0,0,0,891,887,1,0,0,0,892,895,1,0,0,0,893,
  	891,1,0,0,0,893,894,1,0,0,0,894,897,1,0,0,0,895,893,1,0,0,0,896,885,1,
  	0,0,0,896,897,1,0,0,0,897,898,1,0,0,0,898,899,5,41,0,0,899,900,6,47,-1,
  	0,900,95,1,0,0,0,74,101,110,114,120,129,131,171,173,198,206,211,213,224,
  	232,241,248,259,262,271,280,298,301,303,312,325,334,345,357,366,380,383,
  	393,398,411,424,434,445,479,496,506,512,518,560,571,573,584,608,612,620,
  	622,644,660,662,674,686,698,710,722,734,746,758,770,782,789,800,817,820,
  	839,843,845,848,881,893,896
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  gpulangParserStaticData = std::move(staticData);
}

}

GPULangParser::GPULangParser(TokenStream *input) : GPULangParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

GPULangParser::GPULangParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  GPULangParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *gpulangParserStaticData->atn, gpulangParserStaticData->decisionToDFA, gpulangParserStaticData->sharedContextCache, options);
}

GPULangParser::~GPULangParser() {
  delete _interpreter;
}

const atn::ATN& GPULangParser::getATN() const {
  return *gpulangParserStaticData->atn;
}

std::string GPULangParser::getGrammarFileName() const {
  return "GPULang.g4";
}

const std::vector<std::string>& GPULangParser::getRuleNames() const {
  return gpulangParserStaticData->ruleNames;
}

const dfa::Vocabulary& GPULangParser::getVocabulary() const {
  return gpulangParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView GPULangParser::getSerializedATN() const {
  return gpulangParserStaticData->serializedATN;
}


//----------------- StringContext ------------------------------------------------------------------

GPULangParser::StringContext::StringContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> GPULangParser::StringContext::QO() {
  return getTokens(GPULangParser::QO);
}

tree::TerminalNode* GPULangParser::StringContext::QO(size_t i) {
  return getToken(GPULangParser::QO, i);
}

std::vector<tree::TerminalNode *> GPULangParser::StringContext::Q() {
  return getTokens(GPULangParser::Q);
}

tree::TerminalNode* GPULangParser::StringContext::Q(size_t i) {
  return getToken(GPULangParser::Q, i);
}


size_t GPULangParser::StringContext::getRuleIndex() const {
  return GPULangParser::RuleString;
}

void GPULangParser::StringContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterString(this);
}

void GPULangParser::StringContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitString(this);
}

GPULangParser::StringContext* GPULangParser::string() {
  StringContext *_localctx = _tracker.createInstance<StringContext>(_ctx, getState());
  enterRule(_localctx, 0, GPULangParser::RuleString);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(114);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case GPULangParser::QO: {
        enterOuterAlt(_localctx, 1);
        setState(96);
        match(GPULangParser::QO);
        setState(101);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & -140737488355330) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 64)) & 2097151) != 0)) {
          setState(97);
          antlrcpp::downCast<StringContext *>(_localctx)->data = _input->LT(1);
          _la = _input->LA(1);
          if (_la == 0 || _la == Token::EOF || (_la == GPULangParser::QO)) {
            antlrcpp::downCast<StringContext *>(_localctx)->data = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
           _localctx->val.append((antlrcpp::downCast<StringContext *>(_localctx)->data != nullptr ? antlrcpp::downCast<StringContext *>(_localctx)->data->getText() : "")); 
          setState(103);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(104);
        match(GPULangParser::QO);
        break;
      }

      case GPULangParser::Q: {
        enterOuterAlt(_localctx, 2);
        setState(105);
        match(GPULangParser::Q);
        setState(110);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & -72057594037927938) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 64)) & 2097151) != 0)) {
          setState(106);
          antlrcpp::downCast<StringContext *>(_localctx)->data = _input->LT(1);
          _la = _input->LA(1);
          if (_la == 0 || _la == Token::EOF || (_la == GPULangParser::Q)) {
            antlrcpp::downCast<StringContext *>(_localctx)->data = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
           _localctx->val.append((antlrcpp::downCast<StringContext *>(_localctx)->data != nullptr ? antlrcpp::downCast<StringContext *>(_localctx)->data->getText() : "")); 
          setState(112);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(113);
        match(GPULangParser::Q);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BooleanContext ------------------------------------------------------------------

GPULangParser::BooleanContext::BooleanContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t GPULangParser::BooleanContext::getRuleIndex() const {
  return GPULangParser::RuleBoolean;
}

void GPULangParser::BooleanContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBoolean(this);
}

void GPULangParser::BooleanContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBoolean(this);
}

GPULangParser::BooleanContext* GPULangParser::boolean() {
  BooleanContext *_localctx = _tracker.createInstance<BooleanContext>(_ctx, getState());
  enterRule(_localctx, 2, GPULangParser::RuleBoolean);

          antlrcpp::downCast<BooleanContext *>(_localctx)->val =  false;
      

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(120);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case GPULangParser::T__0: {
        enterOuterAlt(_localctx, 1);
        setState(116);
        match(GPULangParser::T__0);
         antlrcpp::downCast<BooleanContext *>(_localctx)->val =  true; 
        break;
      }

      case GPULangParser::T__1: {
        enterOuterAlt(_localctx, 2);
        setState(118);
        match(GPULangParser::T__1);
         antlrcpp::downCast<BooleanContext *>(_localctx)->val =  false; 
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PreprocessContext ------------------------------------------------------------------

GPULangParser::PreprocessContext::PreprocessContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* GPULangParser::PreprocessContext::EOF() {
  return getToken(GPULangParser::EOF, 0);
}

std::vector<tree::TerminalNode *> GPULangParser::PreprocessContext::INTEGERLITERAL() {
  return getTokens(GPULangParser::INTEGERLITERAL);
}

tree::TerminalNode* GPULangParser::PreprocessContext::INTEGERLITERAL(size_t i) {
  return getToken(GPULangParser::INTEGERLITERAL, i);
}

std::vector<GPULangParser::StringContext *> GPULangParser::PreprocessContext::string() {
  return getRuleContexts<GPULangParser::StringContext>();
}

GPULangParser::StringContext* GPULangParser::PreprocessContext::string(size_t i) {
  return getRuleContext<GPULangParser::StringContext>(i);
}


size_t GPULangParser::PreprocessContext::getRuleIndex() const {
  return GPULangParser::RulePreprocess;
}

void GPULangParser::PreprocessContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPreprocess(this);
}

void GPULangParser::PreprocessContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPreprocess(this);
}

GPULangParser::PreprocessContext* GPULangParser::preprocess() {
  PreprocessContext *_localctx = _tracker.createInstance<PreprocessContext>(_ctx, getState());
  enterRule(_localctx, 4, GPULangParser::RulePreprocess);

          Token* start = nullptr;
      

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(131);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx);
    while (alt != 1 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1 + 1) {
        setState(129);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx)) {
        case 1: {
           start = _input->LT(1); 
          setState(123);
          match(GPULangParser::T__2);
          setState(124);
          antlrcpp::downCast<PreprocessContext *>(_localctx)->line = match(GPULangParser::INTEGERLITERAL);
          setState(125);
          antlrcpp::downCast<PreprocessContext *>(_localctx)->path = string();
           this->lines.push_back(std::make_tuple(atoi((antlrcpp::downCast<PreprocessContext *>(_localctx)->line != nullptr ? antlrcpp::downCast<PreprocessContext *>(_localctx)->line->getText() : "").c_str()), _input->LT(-1)->getLine(), start->getStartIndex(), _input->LT(1)->getStartIndex(), antlrcpp::downCast<PreprocessContext *>(_localctx)->path->val)); 
          break;
        }

        case 2: {
          setState(128);
          matchWildcard();
          break;
        }

        default:
          break;
        } 
      }
      setState(133);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx);
    }
    setState(134);
    match(GPULangParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EntryContext ------------------------------------------------------------------

GPULangParser::EntryContext::EntryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

GPULangParser::EffectContext* GPULangParser::EntryContext::effect() {
  return getRuleContext<GPULangParser::EffectContext>(0);
}

tree::TerminalNode* GPULangParser::EntryContext::EOF() {
  return getToken(GPULangParser::EOF, 0);
}


size_t GPULangParser::EntryContext::getRuleIndex() const {
  return GPULangParser::RuleEntry;
}

void GPULangParser::EntryContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterEntry(this);
}

void GPULangParser::EntryContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitEntry(this);
}

GPULangParser::EntryContext* GPULangParser::entry() {
  EntryContext *_localctx = _tracker.createInstance<EntryContext>(_ctx, getState());
  enterRule(_localctx, 6, GPULangParser::RuleEntry);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(136);
    antlrcpp::downCast<EntryContext *>(_localctx)->effectContext = effect();

            antlrcpp::downCast<EntryContext *>(_localctx)->returnEffect =  antlrcpp::downCast<EntryContext *>(_localctx)->effectContext->eff;
        
    setState(138);
    match(GPULangParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EffectContext ------------------------------------------------------------------

GPULangParser::EffectContext::EffectContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<GPULangParser::AliasContext *> GPULangParser::EffectContext::alias() {
  return getRuleContexts<GPULangParser::AliasContext>();
}

GPULangParser::AliasContext* GPULangParser::EffectContext::alias(size_t i) {
  return getRuleContext<GPULangParser::AliasContext>(i);
}

std::vector<tree::TerminalNode *> GPULangParser::EffectContext::SC() {
  return getTokens(GPULangParser::SC);
}

tree::TerminalNode* GPULangParser::EffectContext::SC(size_t i) {
  return getToken(GPULangParser::SC, i);
}

std::vector<GPULangParser::FunctionDeclarationContext *> GPULangParser::EffectContext::functionDeclaration() {
  return getRuleContexts<GPULangParser::FunctionDeclarationContext>();
}

GPULangParser::FunctionDeclarationContext* GPULangParser::EffectContext::functionDeclaration(size_t i) {
  return getRuleContext<GPULangParser::FunctionDeclarationContext>(i);
}

std::vector<GPULangParser::FunctionContext *> GPULangParser::EffectContext::function() {
  return getRuleContexts<GPULangParser::FunctionContext>();
}

GPULangParser::FunctionContext* GPULangParser::EffectContext::function(size_t i) {
  return getRuleContext<GPULangParser::FunctionContext>(i);
}

std::vector<GPULangParser::VariablesContext *> GPULangParser::EffectContext::variables() {
  return getRuleContexts<GPULangParser::VariablesContext>();
}

GPULangParser::VariablesContext* GPULangParser::EffectContext::variables(size_t i) {
  return getRuleContext<GPULangParser::VariablesContext>(i);
}

std::vector<GPULangParser::StructureContext *> GPULangParser::EffectContext::structure() {
  return getRuleContexts<GPULangParser::StructureContext>();
}

GPULangParser::StructureContext* GPULangParser::EffectContext::structure(size_t i) {
  return getRuleContext<GPULangParser::StructureContext>(i);
}

std::vector<GPULangParser::EnumerationContext *> GPULangParser::EffectContext::enumeration() {
  return getRuleContexts<GPULangParser::EnumerationContext>();
}

GPULangParser::EnumerationContext* GPULangParser::EffectContext::enumeration(size_t i) {
  return getRuleContext<GPULangParser::EnumerationContext>(i);
}

std::vector<GPULangParser::StateContext *> GPULangParser::EffectContext::state() {
  return getRuleContexts<GPULangParser::StateContext>();
}

GPULangParser::StateContext* GPULangParser::EffectContext::state(size_t i) {
  return getRuleContext<GPULangParser::StateContext>(i);
}

std::vector<GPULangParser::ProgramContext *> GPULangParser::EffectContext::program() {
  return getRuleContexts<GPULangParser::ProgramContext>();
}

GPULangParser::ProgramContext* GPULangParser::EffectContext::program(size_t i) {
  return getRuleContext<GPULangParser::ProgramContext>(i);
}


size_t GPULangParser::EffectContext::getRuleIndex() const {
  return GPULangParser::RuleEffect;
}

void GPULangParser::EffectContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterEffect(this);
}

void GPULangParser::EffectContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitEffect(this);
}

GPULangParser::EffectContext* GPULangParser::effect() {
  EffectContext *_localctx = _tracker.createInstance<EffectContext>(_ctx, getState());
  enterRule(_localctx, 8, GPULangParser::RuleEffect);

          antlrcpp::downCast<EffectContext *>(_localctx)->eff =  new Effect();
      

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(173);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx);
    while (alt != 1 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1 + 1) {
        setState(171);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 6, _ctx)) {
        case 1: {
          setState(140);
          antlrcpp::downCast<EffectContext *>(_localctx)->aliasContext = alias();
          setState(141);
          match(GPULangParser::SC);
           _localctx->eff->symbols.push_back(antlrcpp::downCast<EffectContext *>(_localctx)->aliasContext->sym); 
          break;
        }

        case 2: {
          setState(144);
          antlrcpp::downCast<EffectContext *>(_localctx)->functionDeclarationContext = functionDeclaration();
          setState(145);
          match(GPULangParser::SC);
           _localctx->eff->symbols.push_back(antlrcpp::downCast<EffectContext *>(_localctx)->functionDeclarationContext->sym); 
          break;
        }

        case 3: {
          setState(148);
          antlrcpp::downCast<EffectContext *>(_localctx)->functionContext = function();
           _localctx->eff->symbols.push_back(antlrcpp::downCast<EffectContext *>(_localctx)->functionContext->sym); 
          break;
        }

        case 4: {
          setState(151);
          antlrcpp::downCast<EffectContext *>(_localctx)->variablesContext = variables();
          setState(152);
          match(GPULangParser::SC);
           for (Variable* var : antlrcpp::downCast<EffectContext *>(_localctx)->variablesContext->list) { _localctx->eff->symbols.push_back(var); } 
          break;
        }

        case 5: {
          setState(155);
          antlrcpp::downCast<EffectContext *>(_localctx)->structureContext = structure();
          setState(156);
          match(GPULangParser::SC);
           _localctx->eff->symbols.push_back(antlrcpp::downCast<EffectContext *>(_localctx)->structureContext->sym); 
          break;
        }

        case 6: {
          setState(159);
          antlrcpp::downCast<EffectContext *>(_localctx)->enumerationContext = enumeration();
          setState(160);
          match(GPULangParser::SC);
           _localctx->eff->symbols.push_back(antlrcpp::downCast<EffectContext *>(_localctx)->enumerationContext->sym); 
          break;
        }

        case 7: {
          setState(163);
          antlrcpp::downCast<EffectContext *>(_localctx)->stateContext = state();
          setState(164);
          match(GPULangParser::SC);
           _localctx->eff->symbols.push_back(antlrcpp::downCast<EffectContext *>(_localctx)->stateContext->sym); 
          break;
        }

        case 8: {
          setState(167);
          antlrcpp::downCast<EffectContext *>(_localctx)->programContext = program();
          setState(168);
          match(GPULangParser::SC);
           _localctx->eff->symbols.push_back(antlrcpp::downCast<EffectContext *>(_localctx)->programContext->sym); 
          break;
        }

        default:
          break;
        } 
      }
      setState(175);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AliasContext ------------------------------------------------------------------

GPULangParser::AliasContext::AliasContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> GPULangParser::AliasContext::IDENTIFIER() {
  return getTokens(GPULangParser::IDENTIFIER);
}

tree::TerminalNode* GPULangParser::AliasContext::IDENTIFIER(size_t i) {
  return getToken(GPULangParser::IDENTIFIER, i);
}


size_t GPULangParser::AliasContext::getRuleIndex() const {
  return GPULangParser::RuleAlias;
}

void GPULangParser::AliasContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAlias(this);
}

void GPULangParser::AliasContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAlias(this);
}

GPULangParser::AliasContext* GPULangParser::alias() {
  AliasContext *_localctx = _tracker.createInstance<AliasContext>(_ctx, getState());
  enterRule(_localctx, 10, GPULangParser::RuleAlias);

          antlrcpp::downCast<AliasContext *>(_localctx)->sym =  nullptr;
          std::string name;
          std::string type;
      

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(176);
    match(GPULangParser::T__3);
    setState(177);
    antlrcpp::downCast<AliasContext *>(_localctx)->name = match(GPULangParser::IDENTIFIER);
    setState(178);
    match(GPULangParser::T__4);
    setState(179);
    antlrcpp::downCast<AliasContext *>(_localctx)->type = match(GPULangParser::IDENTIFIER);
     name = (antlrcpp::downCast<AliasContext *>(_localctx)->name != nullptr ? antlrcpp::downCast<AliasContext *>(_localctx)->name->getText() : ""); type = (antlrcpp::downCast<AliasContext *>(_localctx)->type != nullptr ? antlrcpp::downCast<AliasContext *>(_localctx)->type->getText() : ""); 

            antlrcpp::downCast<AliasContext *>(_localctx)->sym =  new Alias();
            _localctx->sym->name = name;
            _localctx->sym->type = type;
        
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AnnotationContext ------------------------------------------------------------------

GPULangParser::AnnotationContext::AnnotationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* GPULangParser::AnnotationContext::SOBAKA() {
  return getToken(GPULangParser::SOBAKA, 0);
}

tree::TerminalNode* GPULangParser::AnnotationContext::LP() {
  return getToken(GPULangParser::LP, 0);
}

tree::TerminalNode* GPULangParser::AnnotationContext::RP() {
  return getToken(GPULangParser::RP, 0);
}

tree::TerminalNode* GPULangParser::AnnotationContext::IDENTIFIER() {
  return getToken(GPULangParser::IDENTIFIER, 0);
}

GPULangParser::ExpressionContext* GPULangParser::AnnotationContext::expression() {
  return getRuleContext<GPULangParser::ExpressionContext>(0);
}


size_t GPULangParser::AnnotationContext::getRuleIndex() const {
  return GPULangParser::RuleAnnotation;
}

void GPULangParser::AnnotationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAnnotation(this);
}

void GPULangParser::AnnotationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAnnotation(this);
}

GPULangParser::AnnotationContext* GPULangParser::annotation() {
  AnnotationContext *_localctx = _tracker.createInstance<AnnotationContext>(_ctx, getState());
  enterRule(_localctx, 12, GPULangParser::RuleAnnotation);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(183);
    match(GPULangParser::SOBAKA);

    setState(184);
    antlrcpp::downCast<AnnotationContext *>(_localctx)->name = match(GPULangParser::IDENTIFIER);
    setState(185);
    match(GPULangParser::LP);
    setState(186);
    antlrcpp::downCast<AnnotationContext *>(_localctx)->value = expression();
    setState(187);
    match(GPULangParser::RP);
     _localctx->annot.name = (antlrcpp::downCast<AnnotationContext *>(_localctx)->name != nullptr ? antlrcpp::downCast<AnnotationContext *>(_localctx)->name->getText() : ""); _localctx->annot.value = antlrcpp::downCast<AnnotationContext *>(_localctx)->value->tree; 
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AttributeContext ------------------------------------------------------------------

GPULangParser::AttributeContext::AttributeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* GPULangParser::AttributeContext::LP() {
  return getToken(GPULangParser::LP, 0);
}

GPULangParser::ExpressionContext* GPULangParser::AttributeContext::expression() {
  return getRuleContext<GPULangParser::ExpressionContext>(0);
}

tree::TerminalNode* GPULangParser::AttributeContext::RP() {
  return getToken(GPULangParser::RP, 0);
}

tree::TerminalNode* GPULangParser::AttributeContext::IDENTIFIER() {
  return getToken(GPULangParser::IDENTIFIER, 0);
}


size_t GPULangParser::AttributeContext::getRuleIndex() const {
  return GPULangParser::RuleAttribute;
}

void GPULangParser::AttributeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAttribute(this);
}

void GPULangParser::AttributeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAttribute(this);
}

GPULangParser::AttributeContext* GPULangParser::attribute() {
  AttributeContext *_localctx = _tracker.createInstance<AttributeContext>(_ctx, getState());
  enterRule(_localctx, 14, GPULangParser::RuleAttribute);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(198);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 8, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(190);
      antlrcpp::downCast<AttributeContext *>(_localctx)->name = match(GPULangParser::IDENTIFIER);
      setState(191);
      match(GPULangParser::LP);
      setState(192);
      antlrcpp::downCast<AttributeContext *>(_localctx)->expressionContext = expression();
      setState(193);
      match(GPULangParser::RP);
       _localctx->attr.name = (antlrcpp::downCast<AttributeContext *>(_localctx)->name != nullptr ? antlrcpp::downCast<AttributeContext *>(_localctx)->name->getText() : ""); _localctx->attr.expression = antlrcpp::downCast<AttributeContext *>(_localctx)->expressionContext->tree; 
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(196);
      antlrcpp::downCast<AttributeContext *>(_localctx)->name = match(GPULangParser::IDENTIFIER);
       _localctx->attr.name = (antlrcpp::downCast<AttributeContext *>(_localctx)->name != nullptr ? antlrcpp::downCast<AttributeContext *>(_localctx)->name->getText() : ""); _localctx->attr.expression = nullptr; 
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeDeclarationContext ------------------------------------------------------------------

GPULangParser::TypeDeclarationContext::TypeDeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> GPULangParser::TypeDeclarationContext::IDENTIFIER() {
  return getTokens(GPULangParser::IDENTIFIER);
}

tree::TerminalNode* GPULangParser::TypeDeclarationContext::IDENTIFIER(size_t i) {
  return getToken(GPULangParser::IDENTIFIER, i);
}

std::vector<tree::TerminalNode *> GPULangParser::TypeDeclarationContext::MUL_OP() {
  return getTokens(GPULangParser::MUL_OP);
}

tree::TerminalNode* GPULangParser::TypeDeclarationContext::MUL_OP(size_t i) {
  return getToken(GPULangParser::MUL_OP, i);
}

std::vector<tree::TerminalNode *> GPULangParser::TypeDeclarationContext::LL() {
  return getTokens(GPULangParser::LL);
}

tree::TerminalNode* GPULangParser::TypeDeclarationContext::LL(size_t i) {
  return getToken(GPULangParser::LL, i);
}

std::vector<tree::TerminalNode *> GPULangParser::TypeDeclarationContext::RR() {
  return getTokens(GPULangParser::RR);
}

tree::TerminalNode* GPULangParser::TypeDeclarationContext::RR(size_t i) {
  return getToken(GPULangParser::RR, i);
}

std::vector<tree::TerminalNode *> GPULangParser::TypeDeclarationContext::INTEGERLITERAL() {
  return getTokens(GPULangParser::INTEGERLITERAL);
}

tree::TerminalNode* GPULangParser::TypeDeclarationContext::INTEGERLITERAL(size_t i) {
  return getToken(GPULangParser::INTEGERLITERAL, i);
}


size_t GPULangParser::TypeDeclarationContext::getRuleIndex() const {
  return GPULangParser::RuleTypeDeclaration;
}

void GPULangParser::TypeDeclarationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTypeDeclaration(this);
}

void GPULangParser::TypeDeclarationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTypeDeclaration(this);
}

GPULangParser::TypeDeclarationContext* GPULangParser::typeDeclaration() {
  TypeDeclarationContext *_localctx = _tracker.createInstance<TypeDeclarationContext>(_ctx, getState());
  enterRule(_localctx, 16, GPULangParser::RuleTypeDeclaration);

          _localctx->type.name = "";
      
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(213);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 11, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(211);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case GPULangParser::MUL_OP: {
            setState(200);
            match(GPULangParser::MUL_OP);
             _localctx->type.AddModifier(Type::FullType::Modifier::Pointer); 
            break;
          }

          case GPULangParser::LL: {
            setState(202);
            match(GPULangParser::LL);
             _localctx->type.AddModifier(Type::FullType::Modifier::Array); 
            setState(206);
            _errHandler->sync(this);

            _la = _input->LA(1);
            if (_la == GPULangParser::INTEGERLITERAL) {
              setState(204);
              antlrcpp::downCast<TypeDeclarationContext *>(_localctx)->arraySize0 = match(GPULangParser::INTEGERLITERAL);
               _localctx->type.UpdateValue(atoi((antlrcpp::downCast<TypeDeclarationContext *>(_localctx)->arraySize0 != nullptr ? antlrcpp::downCast<TypeDeclarationContext *>(_localctx)->arraySize0->getText() : "").c_str())); 
            }
            setState(208);
            match(GPULangParser::RR);
            break;
          }

          case GPULangParser::IDENTIFIER: {
            setState(209);
            antlrcpp::downCast<TypeDeclarationContext *>(_localctx)->identifierToken = match(GPULangParser::IDENTIFIER);
             _localctx->type.AddQualifier((antlrcpp::downCast<TypeDeclarationContext *>(_localctx)->identifierToken != nullptr ? antlrcpp::downCast<TypeDeclarationContext *>(_localctx)->identifierToken->getText() : "")); 
            break;
          }

        default:
          throw NoViableAltException(this);
        } 
      }
      setState(215);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 11, _ctx);
    }
    setState(216);
    antlrcpp::downCast<TypeDeclarationContext *>(_localctx)->typeName = match(GPULangParser::IDENTIFIER);
     _localctx->type.name = (antlrcpp::downCast<TypeDeclarationContext *>(_localctx)->typeName != nullptr ? antlrcpp::downCast<TypeDeclarationContext *>(_localctx)->typeName->getText() : ""); 
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VariablesContext ------------------------------------------------------------------

GPULangParser::VariablesContext::VariablesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> GPULangParser::VariablesContext::IDENTIFIER() {
  return getTokens(GPULangParser::IDENTIFIER);
}

tree::TerminalNode* GPULangParser::VariablesContext::IDENTIFIER(size_t i) {
  return getToken(GPULangParser::IDENTIFIER, i);
}

std::vector<GPULangParser::AnnotationContext *> GPULangParser::VariablesContext::annotation() {
  return getRuleContexts<GPULangParser::AnnotationContext>();
}

GPULangParser::AnnotationContext* GPULangParser::VariablesContext::annotation(size_t i) {
  return getRuleContext<GPULangParser::AnnotationContext>(i);
}

std::vector<GPULangParser::AttributeContext *> GPULangParser::VariablesContext::attribute() {
  return getRuleContexts<GPULangParser::AttributeContext>();
}

GPULangParser::AttributeContext* GPULangParser::VariablesContext::attribute(size_t i) {
  return getRuleContext<GPULangParser::AttributeContext>(i);
}

std::vector<tree::TerminalNode *> GPULangParser::VariablesContext::CO() {
  return getTokens(GPULangParser::CO);
}

tree::TerminalNode* GPULangParser::VariablesContext::CO(size_t i) {
  return getToken(GPULangParser::CO, i);
}

tree::TerminalNode* GPULangParser::VariablesContext::COL() {
  return getToken(GPULangParser::COL, 0);
}

GPULangParser::TypeDeclarationContext* GPULangParser::VariablesContext::typeDeclaration() {
  return getRuleContext<GPULangParser::TypeDeclarationContext>(0);
}

tree::TerminalNode* GPULangParser::VariablesContext::EQ() {
  return getToken(GPULangParser::EQ, 0);
}

std::vector<GPULangParser::AssignmentExpressionContext *> GPULangParser::VariablesContext::assignmentExpression() {
  return getRuleContexts<GPULangParser::AssignmentExpressionContext>();
}

GPULangParser::AssignmentExpressionContext* GPULangParser::VariablesContext::assignmentExpression(size_t i) {
  return getRuleContext<GPULangParser::AssignmentExpressionContext>(i);
}


size_t GPULangParser::VariablesContext::getRuleIndex() const {
  return GPULangParser::RuleVariables;
}

void GPULangParser::VariablesContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterVariables(this);
}

void GPULangParser::VariablesContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitVariables(this);
}

GPULangParser::VariablesContext* GPULangParser::variables() {
  VariablesContext *_localctx = _tracker.createInstance<VariablesContext>(_ctx, getState());
  enterRule(_localctx, 18, GPULangParser::RuleVariables);

          std::vector<Annotation> annotations;
          std::vector<Attribute> attributes;
          std::vector<std::string> names;
          std::vector<Expression*> valueExpressions;
          std::vector<Symbol::Location> locations;
          unsigned initCounter = 0;
          Type::FullType type = { "unknown" };
      
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(224);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == GPULangParser::SOBAKA) {
      setState(219);
      antlrcpp::downCast<VariablesContext *>(_localctx)->annotationContext = annotation();
       annotations.push_back(antlrcpp::downCast<VariablesContext *>(_localctx)->annotationContext->annot); 
      setState(226);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(230); 
    _errHandler->sync(this);
    alt = 1;
    do {
      switch (alt) {
        case 1: {
              setState(227);
              antlrcpp::downCast<VariablesContext *>(_localctx)->attributeContext = attribute();
               attributes.push_back(antlrcpp::downCast<VariablesContext *>(_localctx)->attributeContext->attr); 
              break;
            }

      default:
        throw NoViableAltException(this);
      }
      setState(232); 
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 13, _ctx);
    } while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER);
    setState(234);
    antlrcpp::downCast<VariablesContext *>(_localctx)->varName = match(GPULangParser::IDENTIFIER);
     names.push_back((antlrcpp::downCast<VariablesContext *>(_localctx)->varName != nullptr ? antlrcpp::downCast<VariablesContext *>(_localctx)->varName->getText() : "")); valueExpressions.push_back(nullptr); locations.push_back(SetupFile()); 
    setState(241);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == GPULangParser::CO) {
      setState(236);
      match(GPULangParser::CO);
      setState(237);
      antlrcpp::downCast<VariablesContext *>(_localctx)->varNameN = match(GPULangParser::IDENTIFIER);
       names.push_back((antlrcpp::downCast<VariablesContext *>(_localctx)->varNameN != nullptr ? antlrcpp::downCast<VariablesContext *>(_localctx)->varNameN->getText() : "")); valueExpressions.push_back(nullptr); locations.push_back(SetupFile()); 
      setState(243);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(248);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == GPULangParser::COL) {
      setState(244);
      match(GPULangParser::COL);
      setState(245);
      antlrcpp::downCast<VariablesContext *>(_localctx)->typeDeclarationContext = typeDeclaration();
       type = antlrcpp::downCast<VariablesContext *>(_localctx)->typeDeclarationContext->type; 
    }
    setState(262);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == GPULangParser::EQ) {
      setState(250);
      match(GPULangParser::EQ);
      setState(251);
      antlrcpp::downCast<VariablesContext *>(_localctx)->valueExpr = assignmentExpression();
       if (initCounter == names.size()) { valueExpressions.push_back(nullptr); } valueExpressions[initCounter++] = antlrcpp::downCast<VariablesContext *>(_localctx)->valueExpr->tree; 
      setState(259);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == GPULangParser::CO) {
        setState(253);
        match(GPULangParser::CO);
        setState(254);
        antlrcpp::downCast<VariablesContext *>(_localctx)->valueExprN = assignmentExpression();
         if (initCounter == names.size()) { valueExpressions.push_back(nullptr); } valueExpressions[initCounter++] = antlrcpp::downCast<VariablesContext *>(_localctx)->valueExprN->tree; 
        setState(261);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }

            for (size_t i = 0; i < names.size(); i++)
            {
                Variable* var = new Variable(); 
                var->type = type; 
                var->location = locations[i]; 
                var->annotations = annotations; 
                var->attributes = attributes;
                var->name = names[i];
                var->valueExpression = valueExpressions[i];
                _localctx->list.push_back(var);
            }
        
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StructureDeclarationContext ------------------------------------------------------------------

GPULangParser::StructureDeclarationContext::StructureDeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* GPULangParser::StructureDeclarationContext::IDENTIFIER() {
  return getToken(GPULangParser::IDENTIFIER, 0);
}

std::vector<GPULangParser::AnnotationContext *> GPULangParser::StructureDeclarationContext::annotation() {
  return getRuleContexts<GPULangParser::AnnotationContext>();
}

GPULangParser::AnnotationContext* GPULangParser::StructureDeclarationContext::annotation(size_t i) {
  return getRuleContext<GPULangParser::AnnotationContext>(i);
}

std::vector<GPULangParser::AttributeContext *> GPULangParser::StructureDeclarationContext::attribute() {
  return getRuleContexts<GPULangParser::AttributeContext>();
}

GPULangParser::AttributeContext* GPULangParser::StructureDeclarationContext::attribute(size_t i) {
  return getRuleContext<GPULangParser::AttributeContext>(i);
}


size_t GPULangParser::StructureDeclarationContext::getRuleIndex() const {
  return GPULangParser::RuleStructureDeclaration;
}

void GPULangParser::StructureDeclarationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStructureDeclaration(this);
}

void GPULangParser::StructureDeclarationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStructureDeclaration(this);
}

GPULangParser::StructureDeclarationContext* GPULangParser::structureDeclaration() {
  StructureDeclarationContext *_localctx = _tracker.createInstance<StructureDeclarationContext>(_ctx, getState());
  enterRule(_localctx, 20, GPULangParser::RuleStructureDeclaration);

          antlrcpp::downCast<StructureDeclarationContext *>(_localctx)->sym =  nullptr;
          std::vector<Annotation> annotations;
          std::vector<Attribute> attributes;
      
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(271);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == GPULangParser::SOBAKA) {
      setState(266);
      antlrcpp::downCast<StructureDeclarationContext *>(_localctx)->annotationContext = annotation();
       annotations.push_back(antlrcpp::downCast<StructureDeclarationContext *>(_localctx)->annotationContext->annot); 
      setState(273);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(274);
    match(GPULangParser::T__5);
    setState(280);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 19, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(275);
        antlrcpp::downCast<StructureDeclarationContext *>(_localctx)->attributeContext = attribute();
         attributes.push_back(antlrcpp::downCast<StructureDeclarationContext *>(_localctx)->attributeContext->attr);  
      }
      setState(282);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 19, _ctx);
    }
    setState(283);
    antlrcpp::downCast<StructureDeclarationContext *>(_localctx)->name = match(GPULangParser::IDENTIFIER);
     
            antlrcpp::downCast<StructureDeclarationContext *>(_localctx)->sym =  new Structure();
            _localctx->sym->name = (antlrcpp::downCast<StructureDeclarationContext *>(_localctx)->name != nullptr ? antlrcpp::downCast<StructureDeclarationContext *>(_localctx)->name->getText() : ""); 
            _localctx->sym->annotations = annotations;
            _localctx->sym->attributes = attributes;
            _localctx->sym->location = SetupFile();
        
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StructureContext ------------------------------------------------------------------

GPULangParser::StructureContext::StructureContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

GPULangParser::StructureDeclarationContext* GPULangParser::StructureContext::structureDeclaration() {
  return getRuleContext<GPULangParser::StructureDeclarationContext>(0);
}

tree::TerminalNode* GPULangParser::StructureContext::LB() {
  return getToken(GPULangParser::LB, 0);
}

tree::TerminalNode* GPULangParser::StructureContext::RB() {
  return getToken(GPULangParser::RB, 0);
}

std::vector<tree::TerminalNode *> GPULangParser::StructureContext::COL() {
  return getTokens(GPULangParser::COL);
}

tree::TerminalNode* GPULangParser::StructureContext::COL(size_t i) {
  return getToken(GPULangParser::COL, i);
}

std::vector<tree::TerminalNode *> GPULangParser::StructureContext::SC() {
  return getTokens(GPULangParser::SC);
}

tree::TerminalNode* GPULangParser::StructureContext::SC(size_t i) {
  return getToken(GPULangParser::SC, i);
}

std::vector<tree::TerminalNode *> GPULangParser::StructureContext::IDENTIFIER() {
  return getTokens(GPULangParser::IDENTIFIER);
}

tree::TerminalNode* GPULangParser::StructureContext::IDENTIFIER(size_t i) {
  return getToken(GPULangParser::IDENTIFIER, i);
}

std::vector<tree::TerminalNode *> GPULangParser::StructureContext::MUL_OP() {
  return getTokens(GPULangParser::MUL_OP);
}

tree::TerminalNode* GPULangParser::StructureContext::MUL_OP(size_t i) {
  return getToken(GPULangParser::MUL_OP, i);
}

std::vector<tree::TerminalNode *> GPULangParser::StructureContext::LL() {
  return getTokens(GPULangParser::LL);
}

tree::TerminalNode* GPULangParser::StructureContext::LL(size_t i) {
  return getToken(GPULangParser::LL, i);
}

std::vector<tree::TerminalNode *> GPULangParser::StructureContext::RR() {
  return getTokens(GPULangParser::RR);
}

tree::TerminalNode* GPULangParser::StructureContext::RR(size_t i) {
  return getToken(GPULangParser::RR, i);
}

std::vector<tree::TerminalNode *> GPULangParser::StructureContext::INTEGERLITERAL() {
  return getTokens(GPULangParser::INTEGERLITERAL);
}

tree::TerminalNode* GPULangParser::StructureContext::INTEGERLITERAL(size_t i) {
  return getToken(GPULangParser::INTEGERLITERAL, i);
}


size_t GPULangParser::StructureContext::getRuleIndex() const {
  return GPULangParser::RuleStructure;
}

void GPULangParser::StructureContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStructure(this);
}

void GPULangParser::StructureContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStructure(this);
}

GPULangParser::StructureContext* GPULangParser::structure() {
  StructureContext *_localctx = _tracker.createInstance<StructureContext>(_ctx, getState());
  enterRule(_localctx, 22, GPULangParser::RuleStructure);

          antlrcpp::downCast<StructureContext *>(_localctx)->sym =  nullptr;
          std::vector<Symbol*> members;
          bool isArray = false;
          Expression* arraySizeExpression = nullptr;
          std::string instanceName;
          Symbol::Location varLocation;
          Type::FullType varType;
          std::string varName;
      
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(286);
    antlrcpp::downCast<StructureContext *>(_localctx)->structureDeclarationContext = structureDeclaration();
     antlrcpp::downCast<StructureContext *>(_localctx)->sym =  antlrcpp::downCast<StructureContext *>(_localctx)->structureDeclarationContext->sym; 
    setState(288);
    match(GPULangParser::LB);
    setState(312);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == GPULangParser::IDENTIFIER) {
      setState(289);
      antlrcpp::downCast<StructureContext *>(_localctx)->varName = match(GPULangParser::IDENTIFIER);
       varName = (antlrcpp::downCast<StructureContext *>(_localctx)->varName != nullptr ? antlrcpp::downCast<StructureContext *>(_localctx)->varName->getText() : ""); varLocation = SetupFile(); 
      setState(291);
      match(GPULangParser::COL);
      setState(303);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == GPULangParser::LL

      || _la == GPULangParser::MUL_OP) {
        setState(301);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case GPULangParser::MUL_OP: {
            setState(292);
            match(GPULangParser::MUL_OP);
             varType.AddModifier(Type::FullType::Modifier::Pointer); 
            break;
          }

          case GPULangParser::LL: {
            setState(294);
            match(GPULangParser::LL);
             varType.AddModifier(Type::FullType::Modifier::Array); 
            setState(298);
            _errHandler->sync(this);

            _la = _input->LA(1);
            if (_la == GPULangParser::INTEGERLITERAL) {
              setState(296);
              antlrcpp::downCast<StructureContext *>(_localctx)->arraySize0 = match(GPULangParser::INTEGERLITERAL);
               varType.UpdateValue(atoi((antlrcpp::downCast<StructureContext *>(_localctx)->arraySize0 != nullptr ? antlrcpp::downCast<StructureContext *>(_localctx)->arraySize0->getText() : "").c_str())); 
            }
            setState(300);
            match(GPULangParser::RR);
            break;
          }

        default:
          throw NoViableAltException(this);
        }
        setState(305);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(306);
      antlrcpp::downCast<StructureContext *>(_localctx)->varTypeName = match(GPULangParser::IDENTIFIER);
       varType.name = (antlrcpp::downCast<StructureContext *>(_localctx)->varTypeName != nullptr ? antlrcpp::downCast<StructureContext *>(_localctx)->varTypeName->getText() : ""); 
      setState(308);
      match(GPULangParser::SC);

                      Variable* var = new Variable(); 
                      var->type = varType; 
                      var->location = varLocation; 
                      var->name = varName;
                      var->valueExpression = nullptr;
                      members.push_back(var);
                  
      setState(314);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(315);
    match(GPULangParser::RB);
     
            _localctx->sym->symbols = members; 
            //_localctx->sym->instanceName = instanceName;
            //_localctx->sym->isArray = isArray;
            //_localctx->sym->arraySizeExpression = arraySizeExpression;
        
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EnumerationContext ------------------------------------------------------------------

GPULangParser::EnumerationContext::EnumerationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* GPULangParser::EnumerationContext::LB() {
  return getToken(GPULangParser::LB, 0);
}

tree::TerminalNode* GPULangParser::EnumerationContext::RB() {
  return getToken(GPULangParser::RB, 0);
}

std::vector<tree::TerminalNode *> GPULangParser::EnumerationContext::IDENTIFIER() {
  return getTokens(GPULangParser::IDENTIFIER);
}

tree::TerminalNode* GPULangParser::EnumerationContext::IDENTIFIER(size_t i) {
  return getToken(GPULangParser::IDENTIFIER, i);
}

tree::TerminalNode* GPULangParser::EnumerationContext::COL() {
  return getToken(GPULangParser::COL, 0);
}

GPULangParser::TypeDeclarationContext* GPULangParser::EnumerationContext::typeDeclaration() {
  return getRuleContext<GPULangParser::TypeDeclarationContext>(0);
}

tree::TerminalNode* GPULangParser::EnumerationContext::EQ() {
  return getToken(GPULangParser::EQ, 0);
}

GPULangParser::ExpressionContext* GPULangParser::EnumerationContext::expression() {
  return getRuleContext<GPULangParser::ExpressionContext>(0);
}


size_t GPULangParser::EnumerationContext::getRuleIndex() const {
  return GPULangParser::RuleEnumeration;
}

void GPULangParser::EnumerationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterEnumeration(this);
}

void GPULangParser::EnumerationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitEnumeration(this);
}

GPULangParser::EnumerationContext* GPULangParser::enumeration() {
  EnumerationContext *_localctx = _tracker.createInstance<EnumerationContext>(_ctx, getState());
  enterRule(_localctx, 24, GPULangParser::RuleEnumeration);

          antlrcpp::downCast<EnumerationContext *>(_localctx)->sym =  nullptr;
          std::vector<std::string> enumLabels;
          std::vector<Expression*> enumValues;
          std::string name;
          Type::FullType type = { "u32" };
          Symbol::Location location;
      
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(318);
    match(GPULangParser::T__6);
    setState(319);
    antlrcpp::downCast<EnumerationContext *>(_localctx)->name = match(GPULangParser::IDENTIFIER);
     name = (antlrcpp::downCast<EnumerationContext *>(_localctx)->name != nullptr ? antlrcpp::downCast<EnumerationContext *>(_localctx)->name->getText() : ""); location = SetupFile(); 
    setState(325);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == GPULangParser::COL) {
      setState(321);
      match(GPULangParser::COL);
      setState(322);
      antlrcpp::downCast<EnumerationContext *>(_localctx)->typeDeclarationContext = typeDeclaration();
       type = antlrcpp::downCast<EnumerationContext *>(_localctx)->typeDeclarationContext->type; 
    }
    setState(327);
    match(GPULangParser::LB);
    setState(328);
    antlrcpp::downCast<EnumerationContext *>(_localctx)->label = match(GPULangParser::IDENTIFIER);
     Expression* expr = nullptr; 
    setState(334);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == GPULangParser::EQ) {
      setState(330);
      match(GPULangParser::EQ);
      setState(331);
      antlrcpp::downCast<EnumerationContext *>(_localctx)->value = expression();
       expr = antlrcpp::downCast<EnumerationContext *>(_localctx)->value->tree; 
    }

                enumLabels.push_back((antlrcpp::downCast<EnumerationContext *>(_localctx)->label != nullptr ? antlrcpp::downCast<EnumerationContext *>(_localctx)->label->getText() : ""));
                enumValues.push_back(expr);
            
    setState(337);
    match(GPULangParser::RB);

            antlrcpp::downCast<EnumerationContext *>(_localctx)->sym =  new Enumeration();
            _localctx->sym->name = name;
            type.literal = true;
            _localctx->sym->type = type;
            _localctx->sym->labels = enumLabels;
            _localctx->sym->values = enumValues;
            _localctx->sym->location = location;
        
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParameterContext ------------------------------------------------------------------

GPULangParser::ParameterContext::ParameterContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* GPULangParser::ParameterContext::COL() {
  return getToken(GPULangParser::COL, 0);
}

GPULangParser::TypeDeclarationContext* GPULangParser::ParameterContext::typeDeclaration() {
  return getRuleContext<GPULangParser::TypeDeclarationContext>(0);
}

tree::TerminalNode* GPULangParser::ParameterContext::IDENTIFIER() {
  return getToken(GPULangParser::IDENTIFIER, 0);
}

std::vector<GPULangParser::AttributeContext *> GPULangParser::ParameterContext::attribute() {
  return getRuleContexts<GPULangParser::AttributeContext>();
}

GPULangParser::AttributeContext* GPULangParser::ParameterContext::attribute(size_t i) {
  return getRuleContext<GPULangParser::AttributeContext>(i);
}

tree::TerminalNode* GPULangParser::ParameterContext::EQ() {
  return getToken(GPULangParser::EQ, 0);
}

GPULangParser::AssignmentExpressionContext* GPULangParser::ParameterContext::assignmentExpression() {
  return getRuleContext<GPULangParser::AssignmentExpressionContext>(0);
}


size_t GPULangParser::ParameterContext::getRuleIndex() const {
  return GPULangParser::RuleParameter;
}

void GPULangParser::ParameterContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterParameter(this);
}

void GPULangParser::ParameterContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitParameter(this);
}

GPULangParser::ParameterContext* GPULangParser::parameter() {
  ParameterContext *_localctx = _tracker.createInstance<ParameterContext>(_ctx, getState());
  enterRule(_localctx, 26, GPULangParser::RuleParameter);

          std::vector<Annotation> annotations;
          std::vector<Attribute> attributes;
          std::string name;
          Expression* valueExpression = nullptr;
          Symbol::Location location;
          Type::FullType type = { "unknown" };
      
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(345);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 26, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(340);
        antlrcpp::downCast<ParameterContext *>(_localctx)->attributeContext = attribute();
         attributes.push_back(antlrcpp::downCast<ParameterContext *>(_localctx)->attributeContext->attr);  
      }
      setState(347);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 26, _ctx);
    }
    setState(348);
    antlrcpp::downCast<ParameterContext *>(_localctx)->varName = match(GPULangParser::IDENTIFIER);
     name = (antlrcpp::downCast<ParameterContext *>(_localctx)->varName != nullptr ? antlrcpp::downCast<ParameterContext *>(_localctx)->varName->getText() : ""); location = SetupFile(); 
    setState(350);
    match(GPULangParser::COL);
    setState(351);
    antlrcpp::downCast<ParameterContext *>(_localctx)->typeDeclarationContext = typeDeclaration();
     type = antlrcpp::downCast<ParameterContext *>(_localctx)->typeDeclarationContext->type; 
    setState(357);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == GPULangParser::EQ) {
      setState(353);
      match(GPULangParser::EQ);
      setState(354);
      antlrcpp::downCast<ParameterContext *>(_localctx)->valueExpr = assignmentExpression();
       valueExpression = antlrcpp::downCast<ParameterContext *>(_localctx)->valueExpr->tree; 
    }

                antlrcpp::downCast<ParameterContext *>(_localctx)->sym =  new Variable(); 
                _localctx->sym->type = type; 
                _localctx->sym->location = location; 
                _localctx->sym->attributes = attributes;
                _localctx->sym->name = name;
                _localctx->sym->valueExpression = valueExpression;
        
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionDeclarationContext ------------------------------------------------------------------

GPULangParser::FunctionDeclarationContext::FunctionDeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* GPULangParser::FunctionDeclarationContext::LP() {
  return getToken(GPULangParser::LP, 0);
}

tree::TerminalNode* GPULangParser::FunctionDeclarationContext::RP() {
  return getToken(GPULangParser::RP, 0);
}

tree::TerminalNode* GPULangParser::FunctionDeclarationContext::IDENTIFIER() {
  return getToken(GPULangParser::IDENTIFIER, 0);
}

GPULangParser::TypeDeclarationContext* GPULangParser::FunctionDeclarationContext::typeDeclaration() {
  return getRuleContext<GPULangParser::TypeDeclarationContext>(0);
}

std::vector<GPULangParser::AttributeContext *> GPULangParser::FunctionDeclarationContext::attribute() {
  return getRuleContexts<GPULangParser::AttributeContext>();
}

GPULangParser::AttributeContext* GPULangParser::FunctionDeclarationContext::attribute(size_t i) {
  return getRuleContext<GPULangParser::AttributeContext>(i);
}

std::vector<GPULangParser::ParameterContext *> GPULangParser::FunctionDeclarationContext::parameter() {
  return getRuleContexts<GPULangParser::ParameterContext>();
}

GPULangParser::ParameterContext* GPULangParser::FunctionDeclarationContext::parameter(size_t i) {
  return getRuleContext<GPULangParser::ParameterContext>(i);
}

std::vector<tree::TerminalNode *> GPULangParser::FunctionDeclarationContext::CO() {
  return getTokens(GPULangParser::CO);
}

tree::TerminalNode* GPULangParser::FunctionDeclarationContext::CO(size_t i) {
  return getToken(GPULangParser::CO, i);
}


size_t GPULangParser::FunctionDeclarationContext::getRuleIndex() const {
  return GPULangParser::RuleFunctionDeclaration;
}

void GPULangParser::FunctionDeclarationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunctionDeclaration(this);
}

void GPULangParser::FunctionDeclarationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunctionDeclaration(this);
}

GPULangParser::FunctionDeclarationContext* GPULangParser::functionDeclaration() {
  FunctionDeclarationContext *_localctx = _tracker.createInstance<FunctionDeclarationContext>(_ctx, getState());
  enterRule(_localctx, 28, GPULangParser::RuleFunctionDeclaration);

          antlrcpp::downCast<FunctionDeclarationContext *>(_localctx)->sym =  nullptr;
          std::vector<Variable*> variables;
          std::vector<Attribute> attributes;
          Symbol::Location location;
      
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(366);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 28, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(361);
        antlrcpp::downCast<FunctionDeclarationContext *>(_localctx)->attributeContext = attribute();
         attributes.push_back(antlrcpp::downCast<FunctionDeclarationContext *>(_localctx)->attributeContext->attr);  
      }
      setState(368);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 28, _ctx);
    }
    setState(369);
    antlrcpp::downCast<FunctionDeclarationContext *>(_localctx)->name = match(GPULangParser::IDENTIFIER);
     location = SetupFile(); 
    setState(371);
    match(GPULangParser::LP);
    setState(383);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == GPULangParser::IDENTIFIER) {
      setState(372);
      antlrcpp::downCast<FunctionDeclarationContext *>(_localctx)->arg0 = parameter();
       variables.push_back(antlrcpp::downCast<FunctionDeclarationContext *>(_localctx)->arg0->sym); 
      setState(380);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == GPULangParser::CO) {
        setState(374);
        match(GPULangParser::CO);
        setState(375);
        antlrcpp::downCast<FunctionDeclarationContext *>(_localctx)->argn = parameter();
         variables.push_back(antlrcpp::downCast<FunctionDeclarationContext *>(_localctx)->argn->sym); 
        setState(382);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(385);
    match(GPULangParser::RP);
    setState(386);
    antlrcpp::downCast<FunctionDeclarationContext *>(_localctx)->returnType = typeDeclaration();

            antlrcpp::downCast<FunctionDeclarationContext *>(_localctx)->sym =  new Function(); 
            _localctx->sym->hasBody = false;
            _localctx->sym->location = location;
            _localctx->sym->returnType = antlrcpp::downCast<FunctionDeclarationContext *>(_localctx)->returnType->type; 
            _localctx->sym->name = (antlrcpp::downCast<FunctionDeclarationContext *>(_localctx)->name != nullptr ? antlrcpp::downCast<FunctionDeclarationContext *>(_localctx)->name->getText() : ""); 
            _localctx->sym->parameters = variables; 
            _localctx->sym->attributes = attributes; 
        
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CodeblockContext ------------------------------------------------------------------

GPULangParser::CodeblockContext::CodeblockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* GPULangParser::CodeblockContext::LB() {
  return getToken(GPULangParser::LB, 0);
}

tree::TerminalNode* GPULangParser::CodeblockContext::RB() {
  return getToken(GPULangParser::RB, 0);
}

std::vector<GPULangParser::CodeblockContext *> GPULangParser::CodeblockContext::codeblock() {
  return getRuleContexts<GPULangParser::CodeblockContext>();
}

GPULangParser::CodeblockContext* GPULangParser::CodeblockContext::codeblock(size_t i) {
  return getRuleContext<GPULangParser::CodeblockContext>(i);
}


size_t GPULangParser::CodeblockContext::getRuleIndex() const {
  return GPULangParser::RuleCodeblock;
}

void GPULangParser::CodeblockContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCodeblock(this);
}

void GPULangParser::CodeblockContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCodeblock(this);
}

GPULangParser::CodeblockContext* GPULangParser::codeblock() {
  CodeblockContext *_localctx = _tracker.createInstance<CodeblockContext>(_ctx, getState());
  enterRule(_localctx, 30, GPULangParser::RuleCodeblock);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(398);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case GPULangParser::LB: {
        enterOuterAlt(_localctx, 1);
        setState(389);
        match(GPULangParser::LB);
        setState(393);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & -2199023255554) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 64)) & 2097151) != 0)) {
          setState(390);
          codeblock();
          setState(395);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(396);
        match(GPULangParser::RB);
        break;
      }

      case GPULangParser::T__0:
      case GPULangParser::T__1:
      case GPULangParser::T__2:
      case GPULangParser::T__3:
      case GPULangParser::T__4:
      case GPULangParser::T__5:
      case GPULangParser::T__6:
      case GPULangParser::T__7:
      case GPULangParser::T__8:
      case GPULangParser::T__9:
      case GPULangParser::T__10:
      case GPULangParser::T__11:
      case GPULangParser::T__12:
      case GPULangParser::T__13:
      case GPULangParser::T__14:
      case GPULangParser::T__15:
      case GPULangParser::T__16:
      case GPULangParser::T__17:
      case GPULangParser::T__18:
      case GPULangParser::T__19:
      case GPULangParser::T__20:
      case GPULangParser::T__21:
      case GPULangParser::T__22:
      case GPULangParser::T__23:
      case GPULangParser::T__24:
      case GPULangParser::T__25:
      case GPULangParser::T__26:
      case GPULangParser::T__27:
      case GPULangParser::T__28:
      case GPULangParser::T__29:
      case GPULangParser::T__30:
      case GPULangParser::T__31:
      case GPULangParser::T__32:
      case GPULangParser::T__33:
      case GPULangParser::SC:
      case GPULangParser::CO:
      case GPULangParser::COL:
      case GPULangParser::LP:
      case GPULangParser::RP:
      case GPULangParser::LL:
      case GPULangParser::RR:
      case GPULangParser::DOT:
      case GPULangParser::NOT:
      case GPULangParser::EQ:
      case GPULangParser::QO:
      case GPULangParser::QU:
      case GPULangParser::AND:
      case GPULangParser::ANDSET:
      case GPULangParser::OR:
      case GPULangParser::ORSET:
      case GPULangParser::XOR:
      case GPULangParser::XORSET:
      case GPULangParser::CONNJUGATE:
      case GPULangParser::Q:
      case GPULangParser::NU:
      case GPULangParser::FORWARDSLASH:
      case GPULangParser::LESS:
      case GPULangParser::LESSEQ:
      case GPULangParser::GREATER:
      case GPULangParser::GREATEREQ:
      case GPULangParser::LOGICEQ:
      case GPULangParser::NOTEQ:
      case GPULangParser::LOGICAND:
      case GPULangParser::LOGICOR:
      case GPULangParser::MOD:
      case GPULangParser::UNDERSC:
      case GPULangParser::SOBAKA:
      case GPULangParser::ADD_OP:
      case GPULangParser::SUB_OP:
      case GPULangParser::DIV_OP:
      case GPULangParser::MUL_OP:
      case GPULangParser::ARROW:
      case GPULangParser::INTEGERLITERAL:
      case GPULangParser::UINTEGERLITERAL:
      case GPULangParser::COMMENT:
      case GPULangParser::ML_COMMENT:
      case GPULangParser::FLOATLITERAL:
      case GPULangParser::EXPONENT:
      case GPULangParser::DOUBLELITERAL:
      case GPULangParser::HEX:
      case GPULangParser::IDENTIFIER:
      case GPULangParser::WS: {
        enterOuterAlt(_localctx, 2);
        setState(397);
        _la = _input->LA(1);
        if (_la == 0 || _la == Token::EOF || (_la == GPULangParser::LB

        || _la == GPULangParser::RB)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionContext ------------------------------------------------------------------

GPULangParser::FunctionContext::FunctionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

GPULangParser::FunctionDeclarationContext* GPULangParser::FunctionContext::functionDeclaration() {
  return getRuleContext<GPULangParser::FunctionDeclarationContext>(0);
}

GPULangParser::ScopeStatementContext* GPULangParser::FunctionContext::scopeStatement() {
  return getRuleContext<GPULangParser::ScopeStatementContext>(0);
}


size_t GPULangParser::FunctionContext::getRuleIndex() const {
  return GPULangParser::RuleFunction;
}

void GPULangParser::FunctionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunction(this);
}

void GPULangParser::FunctionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunction(this);
}

GPULangParser::FunctionContext* GPULangParser::function() {
  FunctionContext *_localctx = _tracker.createInstance<FunctionContext>(_ctx, getState());
  enterRule(_localctx, 32, GPULangParser::RuleFunction);

          antlrcpp::downCast<FunctionContext *>(_localctx)->sym =  nullptr;
          Token* startToken = nullptr;
          Token* endToken = nullptr;
      

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(400);
    antlrcpp::downCast<FunctionContext *>(_localctx)->functionDeclarationContext = functionDeclaration();
     antlrcpp::downCast<FunctionContext *>(_localctx)->sym =  antlrcpp::downCast<FunctionContext *>(_localctx)->functionDeclarationContext->sym; 

            startToken = _input->LT(2);
        
    setState(403);
    antlrcpp::downCast<FunctionContext *>(_localctx)->scopeStatementContext = scopeStatement();

            endToken = _input->LT(-2);

            // extract code from between tokens
            antlr4::misc::Interval interval;
            interval.a = startToken->getTokenIndex();
            interval.b = endToken->getTokenIndex();
            _localctx->sym->body = _input->getText(interval);
            _localctx->sym->hasBody = true;
            _localctx->sym->ast = antlrcpp::downCast<FunctionContext *>(_localctx)->scopeStatementContext->tree;
        
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ProgramContext ------------------------------------------------------------------

GPULangParser::ProgramContext::ProgramContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* GPULangParser::ProgramContext::LB() {
  return getToken(GPULangParser::LB, 0);
}

tree::TerminalNode* GPULangParser::ProgramContext::RB() {
  return getToken(GPULangParser::RB, 0);
}

tree::TerminalNode* GPULangParser::ProgramContext::IDENTIFIER() {
  return getToken(GPULangParser::IDENTIFIER, 0);
}

std::vector<GPULangParser::AnnotationContext *> GPULangParser::ProgramContext::annotation() {
  return getRuleContexts<GPULangParser::AnnotationContext>();
}

GPULangParser::AnnotationContext* GPULangParser::ProgramContext::annotation(size_t i) {
  return getRuleContext<GPULangParser::AnnotationContext>(i);
}

std::vector<tree::TerminalNode *> GPULangParser::ProgramContext::SC() {
  return getTokens(GPULangParser::SC);
}

tree::TerminalNode* GPULangParser::ProgramContext::SC(size_t i) {
  return getToken(GPULangParser::SC, i);
}

std::vector<GPULangParser::ExpressionContext *> GPULangParser::ProgramContext::expression() {
  return getRuleContexts<GPULangParser::ExpressionContext>();
}

GPULangParser::ExpressionContext* GPULangParser::ProgramContext::expression(size_t i) {
  return getRuleContext<GPULangParser::ExpressionContext>(i);
}


size_t GPULangParser::ProgramContext::getRuleIndex() const {
  return GPULangParser::RuleProgram;
}

void GPULangParser::ProgramContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterProgram(this);
}

void GPULangParser::ProgramContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitProgram(this);
}

GPULangParser::ProgramContext* GPULangParser::program() {
  ProgramContext *_localctx = _tracker.createInstance<ProgramContext>(_ctx, getState());
  enterRule(_localctx, 34, GPULangParser::RuleProgram);

          antlrcpp::downCast<ProgramContext *>(_localctx)->sym =  nullptr;
          Symbol::Location location;
          std::vector<Program::SubroutineMapping> subroutines;
          std::vector<Expression*> entries;
          std::vector<Annotation> annotations;
      
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(411);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == GPULangParser::SOBAKA) {
      setState(406);
      antlrcpp::downCast<ProgramContext *>(_localctx)->annotationContext = annotation();
       annotations.push_back(antlrcpp::downCast<ProgramContext *>(_localctx)->annotationContext->annot); 
      setState(413);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(414);
    match(GPULangParser::T__7);
    setState(415);
    antlrcpp::downCast<ProgramContext *>(_localctx)->name = match(GPULangParser::IDENTIFIER);
     location = SetupFile(); 
    setState(417);
    match(GPULangParser::LB);
    setState(424);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 108263700191772678) != 0) || ((((_la - 70) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 70)) & 14955) != 0)) {
      setState(418);
      antlrcpp::downCast<ProgramContext *>(_localctx)->assignment = expression();
       entries.push_back(antlrcpp::downCast<ProgramContext *>(_localctx)->assignment->tree); 
      setState(420);
      match(GPULangParser::SC);
      setState(426);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(427);
    match(GPULangParser::RB);
     
            antlrcpp::downCast<ProgramContext *>(_localctx)->sym =  new Program();
            _localctx->sym->location = location;
            _localctx->sym->name = (antlrcpp::downCast<ProgramContext *>(_localctx)->name != nullptr ? antlrcpp::downCast<ProgramContext *>(_localctx)->name->getText() : "");
            _localctx->sym->annotations = annotations;
            _localctx->sym->entries = entries;
        
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StateContext ------------------------------------------------------------------

GPULangParser::StateContext::StateContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* GPULangParser::StateContext::LB() {
  return getToken(GPULangParser::LB, 0);
}

tree::TerminalNode* GPULangParser::StateContext::RB() {
  return getToken(GPULangParser::RB, 0);
}

tree::TerminalNode* GPULangParser::StateContext::IDENTIFIER() {
  return getToken(GPULangParser::IDENTIFIER, 0);
}

std::vector<tree::TerminalNode *> GPULangParser::StateContext::SC() {
  return getTokens(GPULangParser::SC);
}

tree::TerminalNode* GPULangParser::StateContext::SC(size_t i) {
  return getToken(GPULangParser::SC, i);
}

std::vector<GPULangParser::ExpressionContext *> GPULangParser::StateContext::expression() {
  return getRuleContexts<GPULangParser::ExpressionContext>();
}

GPULangParser::ExpressionContext* GPULangParser::StateContext::expression(size_t i) {
  return getRuleContext<GPULangParser::ExpressionContext>(i);
}


size_t GPULangParser::StateContext::getRuleIndex() const {
  return GPULangParser::RuleState;
}

void GPULangParser::StateContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterState(this);
}

void GPULangParser::StateContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitState(this);
}

GPULangParser::StateContext* GPULangParser::state() {
  StateContext *_localctx = _tracker.createInstance<StateContext>(_ctx, getState());
  enterRule(_localctx, 36, GPULangParser::RuleState);

          Expression* arrayExpression = nullptr;
          std::vector<Expression*> entries;
      
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(434);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case GPULangParser::T__8: {
        setState(430);
        match(GPULangParser::T__8);
         antlrcpp::downCast<StateContext *>(_localctx)->sym =  new SamplerState(); 
        break;
      }

      case GPULangParser::T__9: {
        setState(432);
        match(GPULangParser::T__9);
         antlrcpp::downCast<StateContext *>(_localctx)->sym =  new RenderState(); 
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(436);
    antlrcpp::downCast<StateContext *>(_localctx)->name = match(GPULangParser::IDENTIFIER);
     _localctx->sym->location = SetupFile(); 
    setState(438);
    match(GPULangParser::LB);
    setState(445);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 108263700191772678) != 0) || ((((_la - 70) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 70)) & 14955) != 0)) {
      setState(439);
      antlrcpp::downCast<StateContext *>(_localctx)->assign = expression();
       entries.push_back(antlrcpp::downCast<StateContext *>(_localctx)->assign->tree); 
      setState(441);
      match(GPULangParser::SC);
      setState(447);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(448);
    match(GPULangParser::RB);

            _localctx->sym->name = (antlrcpp::downCast<StateContext *>(_localctx)->name != nullptr ? antlrcpp::downCast<StateContext *>(_localctx)->name->getText() : "");
            _localctx->sym->entries = entries;
        
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatementContext ------------------------------------------------------------------

GPULangParser::StatementContext::StatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

GPULangParser::IfStatementContext* GPULangParser::StatementContext::ifStatement() {
  return getRuleContext<GPULangParser::IfStatementContext>(0);
}

GPULangParser::ScopeStatementContext* GPULangParser::StatementContext::scopeStatement() {
  return getRuleContext<GPULangParser::ScopeStatementContext>(0);
}

GPULangParser::ForStatementContext* GPULangParser::StatementContext::forStatement() {
  return getRuleContext<GPULangParser::ForStatementContext>(0);
}

GPULangParser::WhileStatementContext* GPULangParser::StatementContext::whileStatement() {
  return getRuleContext<GPULangParser::WhileStatementContext>(0);
}

GPULangParser::SwitchStatementContext* GPULangParser::StatementContext::switchStatement() {
  return getRuleContext<GPULangParser::SwitchStatementContext>(0);
}

GPULangParser::ReturnStatementContext* GPULangParser::StatementContext::returnStatement() {
  return getRuleContext<GPULangParser::ReturnStatementContext>(0);
}

GPULangParser::ContinueStatementContext* GPULangParser::StatementContext::continueStatement() {
  return getRuleContext<GPULangParser::ContinueStatementContext>(0);
}

GPULangParser::BreakStatementContext* GPULangParser::StatementContext::breakStatement() {
  return getRuleContext<GPULangParser::BreakStatementContext>(0);
}

GPULangParser::ExpressionStatementContext* GPULangParser::StatementContext::expressionStatement() {
  return getRuleContext<GPULangParser::ExpressionStatementContext>(0);
}

tree::TerminalNode* GPULangParser::StatementContext::SC() {
  return getToken(GPULangParser::SC, 0);
}


size_t GPULangParser::StatementContext::getRuleIndex() const {
  return GPULangParser::RuleStatement;
}

void GPULangParser::StatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStatement(this);
}

void GPULangParser::StatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStatement(this);
}

GPULangParser::StatementContext* GPULangParser::statement() {
  StatementContext *_localctx = _tracker.createInstance<StatementContext>(_ctx, getState());
  enterRule(_localctx, 38, GPULangParser::RuleStatement);

          antlrcpp::downCast<StatementContext *>(_localctx)->tree =  nullptr;
      

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(479);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 37, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(451);
      antlrcpp::downCast<StatementContext *>(_localctx)->ifStatementContext = ifStatement();
       antlrcpp::downCast<StatementContext *>(_localctx)->tree =  antlrcpp::downCast<StatementContext *>(_localctx)->ifStatementContext->tree; 
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(454);
      antlrcpp::downCast<StatementContext *>(_localctx)->scopeStatementContext = scopeStatement();
       antlrcpp::downCast<StatementContext *>(_localctx)->tree =  antlrcpp::downCast<StatementContext *>(_localctx)->scopeStatementContext->tree; 
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(457);
      antlrcpp::downCast<StatementContext *>(_localctx)->forStatementContext = forStatement();
       antlrcpp::downCast<StatementContext *>(_localctx)->tree =  antlrcpp::downCast<StatementContext *>(_localctx)->forStatementContext->tree; 
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(460);
      antlrcpp::downCast<StatementContext *>(_localctx)->whileStatementContext = whileStatement();
       antlrcpp::downCast<StatementContext *>(_localctx)->tree =  antlrcpp::downCast<StatementContext *>(_localctx)->whileStatementContext->tree; 
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(463);
      antlrcpp::downCast<StatementContext *>(_localctx)->switchStatementContext = switchStatement();
       antlrcpp::downCast<StatementContext *>(_localctx)->tree =  antlrcpp::downCast<StatementContext *>(_localctx)->switchStatementContext->tree; 
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(466);
      antlrcpp::downCast<StatementContext *>(_localctx)->returnStatementContext = returnStatement();
       antlrcpp::downCast<StatementContext *>(_localctx)->tree =  antlrcpp::downCast<StatementContext *>(_localctx)->returnStatementContext->tree; 
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(469);
      antlrcpp::downCast<StatementContext *>(_localctx)->continueStatementContext = continueStatement();
       antlrcpp::downCast<StatementContext *>(_localctx)->tree =  antlrcpp::downCast<StatementContext *>(_localctx)->continueStatementContext->tree; 
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(472);
      antlrcpp::downCast<StatementContext *>(_localctx)->breakStatementContext = breakStatement();
       antlrcpp::downCast<StatementContext *>(_localctx)->tree =  antlrcpp::downCast<StatementContext *>(_localctx)->breakStatementContext->tree; 
      break;
    }

    case 9: {
      enterOuterAlt(_localctx, 9);
      setState(475);
      antlrcpp::downCast<StatementContext *>(_localctx)->expressionStatementContext = expressionStatement();
      setState(476);
      match(GPULangParser::SC);
       antlrcpp::downCast<StatementContext *>(_localctx)->tree =  antlrcpp::downCast<StatementContext *>(_localctx)->expressionStatementContext->tree; 
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpressionStatementContext ------------------------------------------------------------------

GPULangParser::ExpressionStatementContext::ExpressionStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

GPULangParser::ExpressionContext* GPULangParser::ExpressionStatementContext::expression() {
  return getRuleContext<GPULangParser::ExpressionContext>(0);
}


size_t GPULangParser::ExpressionStatementContext::getRuleIndex() const {
  return GPULangParser::RuleExpressionStatement;
}

void GPULangParser::ExpressionStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExpressionStatement(this);
}

void GPULangParser::ExpressionStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExpressionStatement(this);
}

GPULangParser::ExpressionStatementContext* GPULangParser::expressionStatement() {
  ExpressionStatementContext *_localctx = _tracker.createInstance<ExpressionStatementContext>(_ctx, getState());
  enterRule(_localctx, 40, GPULangParser::RuleExpressionStatement);

          antlrcpp::downCast<ExpressionStatementContext *>(_localctx)->tree =  nullptr;
      

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(481);
    antlrcpp::downCast<ExpressionStatementContext *>(_localctx)->expressionContext = expression();

            antlrcpp::downCast<ExpressionStatementContext *>(_localctx)->tree =  new ExpressionStatement(antlrcpp::downCast<ExpressionStatementContext *>(_localctx)->expressionContext->tree);
            _localctx->tree->location = SetupFile();
        
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IfStatementContext ------------------------------------------------------------------

GPULangParser::IfStatementContext::IfStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* GPULangParser::IfStatementContext::LP() {
  return getToken(GPULangParser::LP, 0);
}

tree::TerminalNode* GPULangParser::IfStatementContext::RP() {
  return getToken(GPULangParser::RP, 0);
}

GPULangParser::ExpressionContext* GPULangParser::IfStatementContext::expression() {
  return getRuleContext<GPULangParser::ExpressionContext>(0);
}

std::vector<GPULangParser::StatementContext *> GPULangParser::IfStatementContext::statement() {
  return getRuleContexts<GPULangParser::StatementContext>();
}

GPULangParser::StatementContext* GPULangParser::IfStatementContext::statement(size_t i) {
  return getRuleContext<GPULangParser::StatementContext>(i);
}


size_t GPULangParser::IfStatementContext::getRuleIndex() const {
  return GPULangParser::RuleIfStatement;
}

void GPULangParser::IfStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIfStatement(this);
}

void GPULangParser::IfStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIfStatement(this);
}

GPULangParser::IfStatementContext* GPULangParser::ifStatement() {
  IfStatementContext *_localctx = _tracker.createInstance<IfStatementContext>(_ctx, getState());
  enterRule(_localctx, 42, GPULangParser::RuleIfStatement);

          antlrcpp::downCast<IfStatementContext *>(_localctx)->tree =  nullptr;
          Expression* condition = nullptr;
          Statement* ifBody = nullptr;
          Statement* elseBody = nullptr;
          Symbol::Location location;
      

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(484);
    match(GPULangParser::T__10);
     location = SetupFile(); 
    setState(486);
    match(GPULangParser::LP);
    setState(487);
    antlrcpp::downCast<IfStatementContext *>(_localctx)->condition = expression();
     condition = antlrcpp::downCast<IfStatementContext *>(_localctx)->condition->tree; 
    setState(489);
    match(GPULangParser::RP);
    setState(490);
    antlrcpp::downCast<IfStatementContext *>(_localctx)->ifBody = statement();
     ifBody = antlrcpp::downCast<IfStatementContext *>(_localctx)->ifBody->tree; 
    setState(496);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 38, _ctx)) {
    case 1: {
      setState(492);
      match(GPULangParser::T__11);
      setState(493);
      antlrcpp::downCast<IfStatementContext *>(_localctx)->elseBody = statement();
       elseBody = antlrcpp::downCast<IfStatementContext *>(_localctx)->elseBody->tree; 
      break;
    }

    default:
      break;
    }

            antlrcpp::downCast<IfStatementContext *>(_localctx)->tree =  new IfStatement(condition, ifBody, elseBody);
            _localctx->tree->location = location;
        
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ForStatementContext ------------------------------------------------------------------

GPULangParser::ForStatementContext::ForStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* GPULangParser::ForStatementContext::LP() {
  return getToken(GPULangParser::LP, 0);
}

std::vector<tree::TerminalNode *> GPULangParser::ForStatementContext::SC() {
  return getTokens(GPULangParser::SC);
}

tree::TerminalNode* GPULangParser::ForStatementContext::SC(size_t i) {
  return getToken(GPULangParser::SC, i);
}

tree::TerminalNode* GPULangParser::ForStatementContext::RP() {
  return getToken(GPULangParser::RP, 0);
}

GPULangParser::StatementContext* GPULangParser::ForStatementContext::statement() {
  return getRuleContext<GPULangParser::StatementContext>(0);
}

GPULangParser::VariablesContext* GPULangParser::ForStatementContext::variables() {
  return getRuleContext<GPULangParser::VariablesContext>(0);
}

std::vector<GPULangParser::ExpressionContext *> GPULangParser::ForStatementContext::expression() {
  return getRuleContexts<GPULangParser::ExpressionContext>();
}

GPULangParser::ExpressionContext* GPULangParser::ForStatementContext::expression(size_t i) {
  return getRuleContext<GPULangParser::ExpressionContext>(i);
}


size_t GPULangParser::ForStatementContext::getRuleIndex() const {
  return GPULangParser::RuleForStatement;
}

void GPULangParser::ForStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterForStatement(this);
}

void GPULangParser::ForStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitForStatement(this);
}

GPULangParser::ForStatementContext* GPULangParser::forStatement() {
  ForStatementContext *_localctx = _tracker.createInstance<ForStatementContext>(_ctx, getState());
  enterRule(_localctx, 44, GPULangParser::RuleForStatement);

          antlrcpp::downCast<ForStatementContext *>(_localctx)->tree =  nullptr;
          std::vector<Variable*> declarations;
          Expression* conditionExpression = nullptr;
          Expression* loopExpression = nullptr;
          Statement* contents = nullptr;
          Symbol::Location location;
      
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(500);
    match(GPULangParser::T__12);
     location = SetupFile(); 
    setState(502);
    match(GPULangParser::LP);
    setState(506);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == GPULangParser::SOBAKA

    || _la == GPULangParser::IDENTIFIER) {
      setState(503);
      antlrcpp::downCast<ForStatementContext *>(_localctx)->variablesContext = variables();
       declarations = antlrcpp::downCast<ForStatementContext *>(_localctx)->variablesContext->list; 
    }
    setState(508);
    match(GPULangParser::SC);
    setState(512);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 108263700191772678) != 0) || ((((_la - 70) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 70)) & 14955) != 0)) {
      setState(509);
      antlrcpp::downCast<ForStatementContext *>(_localctx)->condition = expression();
       conditionExpression = antlrcpp::downCast<ForStatementContext *>(_localctx)->condition->tree; 
    }
    setState(514);
    match(GPULangParser::SC);
    setState(518);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 108263700191772678) != 0) || ((((_la - 70) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 70)) & 14955) != 0)) {
      setState(515);
      antlrcpp::downCast<ForStatementContext *>(_localctx)->loop = expression();
       loopExpression = antlrcpp::downCast<ForStatementContext *>(_localctx)->loop->tree; 
    }
    setState(520);
    match(GPULangParser::RP);
    setState(521);
    antlrcpp::downCast<ForStatementContext *>(_localctx)->content = statement();
     contents = antlrcpp::downCast<ForStatementContext *>(_localctx)->content->tree; 

            antlrcpp::downCast<ForStatementContext *>(_localctx)->tree =  new ForStatement(declarations, conditionExpression, loopExpression, contents);
            _localctx->tree->location = location;
        
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ForRangeStatementContext ------------------------------------------------------------------

GPULangParser::ForRangeStatementContext::ForRangeStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* GPULangParser::ForRangeStatementContext::LP() {
  return getToken(GPULangParser::LP, 0);
}

tree::TerminalNode* GPULangParser::ForRangeStatementContext::COL() {
  return getToken(GPULangParser::COL, 0);
}

tree::TerminalNode* GPULangParser::ForRangeStatementContext::RP() {
  return getToken(GPULangParser::RP, 0);
}

std::vector<tree::TerminalNode *> GPULangParser::ForRangeStatementContext::IDENTIFIER() {
  return getTokens(GPULangParser::IDENTIFIER);
}

tree::TerminalNode* GPULangParser::ForRangeStatementContext::IDENTIFIER(size_t i) {
  return getToken(GPULangParser::IDENTIFIER, i);
}

GPULangParser::StatementContext* GPULangParser::ForRangeStatementContext::statement() {
  return getRuleContext<GPULangParser::StatementContext>(0);
}


size_t GPULangParser::ForRangeStatementContext::getRuleIndex() const {
  return GPULangParser::RuleForRangeStatement;
}

void GPULangParser::ForRangeStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterForRangeStatement(this);
}

void GPULangParser::ForRangeStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitForRangeStatement(this);
}

GPULangParser::ForRangeStatementContext* GPULangParser::forRangeStatement() {
  ForRangeStatementContext *_localctx = _tracker.createInstance<ForRangeStatementContext>(_ctx, getState());
  enterRule(_localctx, 46, GPULangParser::RuleForRangeStatement);

          antlrcpp::downCast<ForRangeStatementContext *>(_localctx)->tree =  nullptr;
          Statement* contents = nullptr;
          Symbol::Location location;
      

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(525);
    match(GPULangParser::T__12);
     location = SetupFile(); 
    setState(527);
    match(GPULangParser::LP);
    setState(528);
    antlrcpp::downCast<ForRangeStatementContext *>(_localctx)->iterator = match(GPULangParser::IDENTIFIER);
    setState(529);
    match(GPULangParser::COL);
    setState(530);
    antlrcpp::downCast<ForRangeStatementContext *>(_localctx)->start = match(GPULangParser::IDENTIFIER);
    setState(531);
    match(GPULangParser::T__13);
    setState(532);
    antlrcpp::downCast<ForRangeStatementContext *>(_localctx)->end = match(GPULangParser::IDENTIFIER);
    setState(533);
    match(GPULangParser::RP);
    setState(534);
    antlrcpp::downCast<ForRangeStatementContext *>(_localctx)->content = statement();
     contents = antlrcpp::downCast<ForRangeStatementContext *>(_localctx)->content->tree; 


        
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- WhileStatementContext ------------------------------------------------------------------

GPULangParser::WhileStatementContext::WhileStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* GPULangParser::WhileStatementContext::LP() {
  return getToken(GPULangParser::LP, 0);
}

tree::TerminalNode* GPULangParser::WhileStatementContext::RP() {
  return getToken(GPULangParser::RP, 0);
}

GPULangParser::ExpressionContext* GPULangParser::WhileStatementContext::expression() {
  return getRuleContext<GPULangParser::ExpressionContext>(0);
}

GPULangParser::StatementContext* GPULangParser::WhileStatementContext::statement() {
  return getRuleContext<GPULangParser::StatementContext>(0);
}

tree::TerminalNode* GPULangParser::WhileStatementContext::SC() {
  return getToken(GPULangParser::SC, 0);
}


size_t GPULangParser::WhileStatementContext::getRuleIndex() const {
  return GPULangParser::RuleWhileStatement;
}

void GPULangParser::WhileStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterWhileStatement(this);
}

void GPULangParser::WhileStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitWhileStatement(this);
}

GPULangParser::WhileStatementContext* GPULangParser::whileStatement() {
  WhileStatementContext *_localctx = _tracker.createInstance<WhileStatementContext>(_ctx, getState());
  enterRule(_localctx, 48, GPULangParser::RuleWhileStatement);

          antlrcpp::downCast<WhileStatementContext *>(_localctx)->tree =  nullptr;
          Expression* conditionExpression = nullptr;
          Statement* contents = nullptr;
          bool isDoWhile = false;
          Symbol::Location location;
      

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(560);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case GPULangParser::T__14: {
        enterOuterAlt(_localctx, 1);
        setState(538);
        match(GPULangParser::T__14);
         location = SetupFile(); 
        setState(540);
        match(GPULangParser::LP);
        setState(541);
        antlrcpp::downCast<WhileStatementContext *>(_localctx)->condition = expression();
         conditionExpression = antlrcpp::downCast<WhileStatementContext *>(_localctx)->condition->tree; 
        setState(543);
        match(GPULangParser::RP);
        setState(544);
        antlrcpp::downCast<WhileStatementContext *>(_localctx)->content = statement();
         contents = antlrcpp::downCast<WhileStatementContext *>(_localctx)->content->tree; 

                antlrcpp::downCast<WhileStatementContext *>(_localctx)->tree =  new WhileStatement(conditionExpression, contents, isDoWhile);
                _localctx->tree->location = location;
            
        break;
      }

      case GPULangParser::T__15: {
        enterOuterAlt(_localctx, 2);
        setState(548);
        match(GPULangParser::T__15);
         location = SetupFile(); 
        setState(550);
        antlrcpp::downCast<WhileStatementContext *>(_localctx)->content = statement();
         contents = antlrcpp::downCast<WhileStatementContext *>(_localctx)->content->tree; isDoWhile = true; 
        setState(552);
        match(GPULangParser::T__14);
        setState(553);
        match(GPULangParser::LP);
        setState(554);
        antlrcpp::downCast<WhileStatementContext *>(_localctx)->condition = expression();
         conditionExpression = antlrcpp::downCast<WhileStatementContext *>(_localctx)->condition->tree; 
        setState(556);
        match(GPULangParser::RP);
        setState(557);
        match(GPULangParser::SC);

                antlrcpp::downCast<WhileStatementContext *>(_localctx)->tree =  new WhileStatement(conditionExpression, contents, isDoWhile);
                _localctx->tree->location = location;
            
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ScopeStatementContext ------------------------------------------------------------------

GPULangParser::ScopeStatementContext::ScopeStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* GPULangParser::ScopeStatementContext::LB() {
  return getToken(GPULangParser::LB, 0);
}

tree::TerminalNode* GPULangParser::ScopeStatementContext::RB() {
  return getToken(GPULangParser::RB, 0);
}

std::vector<GPULangParser::VariablesContext *> GPULangParser::ScopeStatementContext::variables() {
  return getRuleContexts<GPULangParser::VariablesContext>();
}

GPULangParser::VariablesContext* GPULangParser::ScopeStatementContext::variables(size_t i) {
  return getRuleContext<GPULangParser::VariablesContext>(i);
}

std::vector<tree::TerminalNode *> GPULangParser::ScopeStatementContext::SC() {
  return getTokens(GPULangParser::SC);
}

tree::TerminalNode* GPULangParser::ScopeStatementContext::SC(size_t i) {
  return getToken(GPULangParser::SC, i);
}

std::vector<GPULangParser::StatementContext *> GPULangParser::ScopeStatementContext::statement() {
  return getRuleContexts<GPULangParser::StatementContext>();
}

GPULangParser::StatementContext* GPULangParser::ScopeStatementContext::statement(size_t i) {
  return getRuleContext<GPULangParser::StatementContext>(i);
}


size_t GPULangParser::ScopeStatementContext::getRuleIndex() const {
  return GPULangParser::RuleScopeStatement;
}

void GPULangParser::ScopeStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterScopeStatement(this);
}

void GPULangParser::ScopeStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitScopeStatement(this);
}

GPULangParser::ScopeStatementContext* GPULangParser::scopeStatement() {
  ScopeStatementContext *_localctx = _tracker.createInstance<ScopeStatementContext>(_ctx, getState());
  enterRule(_localctx, 50, GPULangParser::RuleScopeStatement);

          antlrcpp::downCast<ScopeStatementContext *>(_localctx)->tree =  nullptr;
          std::vector<Symbol*> contents;
          Symbol::Location location;
      
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(562);
    match(GPULangParser::LB);
     location = SetupFile(); 
    setState(573);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 108263700196993030) != 0) || ((((_la - 69) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 69)) & 29911) != 0)) {
      setState(571);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 43, _ctx)) {
      case 1: {
        setState(564);
        antlrcpp::downCast<ScopeStatementContext *>(_localctx)->variablesContext = variables();
        setState(565);
        match(GPULangParser::SC);
         for(Variable* var : antlrcpp::downCast<ScopeStatementContext *>(_localctx)->variablesContext->list) { contents.push_back(var); } 
        break;
      }

      case 2: {
        setState(568);
        antlrcpp::downCast<ScopeStatementContext *>(_localctx)->statementContext = statement();
         contents.push_back(antlrcpp::downCast<ScopeStatementContext *>(_localctx)->statementContext->tree); 
        break;
      }

      default:
        break;
      }
      setState(575);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(576);
    match(GPULangParser::RB);

            antlrcpp::downCast<ScopeStatementContext *>(_localctx)->tree =  new ScopeStatement(contents);
            _localctx->tree->location = location;
        
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ReturnStatementContext ------------------------------------------------------------------

GPULangParser::ReturnStatementContext::ReturnStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* GPULangParser::ReturnStatementContext::SC() {
  return getToken(GPULangParser::SC, 0);
}

GPULangParser::ExpressionContext* GPULangParser::ReturnStatementContext::expression() {
  return getRuleContext<GPULangParser::ExpressionContext>(0);
}


size_t GPULangParser::ReturnStatementContext::getRuleIndex() const {
  return GPULangParser::RuleReturnStatement;
}

void GPULangParser::ReturnStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterReturnStatement(this);
}

void GPULangParser::ReturnStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitReturnStatement(this);
}

GPULangParser::ReturnStatementContext* GPULangParser::returnStatement() {
  ReturnStatementContext *_localctx = _tracker.createInstance<ReturnStatementContext>(_ctx, getState());
  enterRule(_localctx, 52, GPULangParser::RuleReturnStatement);

          antlrcpp::downCast<ReturnStatementContext *>(_localctx)->tree =  nullptr;
          Expression* returnValue = nullptr;
          Symbol::Location location;
      
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(579);
    match(GPULangParser::T__16);
     location = SetupFile(); 
    setState(584);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 108263700191772678) != 0) || ((((_la - 70) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 70)) & 14955) != 0)) {
      setState(581);
      antlrcpp::downCast<ReturnStatementContext *>(_localctx)->value = expression();
       returnValue = antlrcpp::downCast<ReturnStatementContext *>(_localctx)->value->tree; 
    }
    setState(586);
    match(GPULangParser::SC);

            antlrcpp::downCast<ReturnStatementContext *>(_localctx)->tree =  new ReturnStatement(returnValue);
            _localctx->tree->location = location;
        
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ContinueStatementContext ------------------------------------------------------------------

GPULangParser::ContinueStatementContext::ContinueStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* GPULangParser::ContinueStatementContext::SC() {
  return getToken(GPULangParser::SC, 0);
}


size_t GPULangParser::ContinueStatementContext::getRuleIndex() const {
  return GPULangParser::RuleContinueStatement;
}

void GPULangParser::ContinueStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterContinueStatement(this);
}

void GPULangParser::ContinueStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitContinueStatement(this);
}

GPULangParser::ContinueStatementContext* GPULangParser::continueStatement() {
  ContinueStatementContext *_localctx = _tracker.createInstance<ContinueStatementContext>(_ctx, getState());
  enterRule(_localctx, 54, GPULangParser::RuleContinueStatement);

          antlrcpp::downCast<ContinueStatementContext *>(_localctx)->tree =  nullptr;
          Symbol::Location location;
      

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(589);
    match(GPULangParser::T__17);
     location = SetupFile(); 
    setState(591);
    match(GPULangParser::SC);

            antlrcpp::downCast<ContinueStatementContext *>(_localctx)->tree =  new ContinueStatement();
            _localctx->tree->location = location;
        
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SwitchStatementContext ------------------------------------------------------------------

GPULangParser::SwitchStatementContext::SwitchStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* GPULangParser::SwitchStatementContext::LP() {
  return getToken(GPULangParser::LP, 0);
}

GPULangParser::ExpressionContext* GPULangParser::SwitchStatementContext::expression() {
  return getRuleContext<GPULangParser::ExpressionContext>(0);
}

tree::TerminalNode* GPULangParser::SwitchStatementContext::RP() {
  return getToken(GPULangParser::RP, 0);
}

tree::TerminalNode* GPULangParser::SwitchStatementContext::LB() {
  return getToken(GPULangParser::LB, 0);
}

tree::TerminalNode* GPULangParser::SwitchStatementContext::RB() {
  return getToken(GPULangParser::RB, 0);
}

std::vector<tree::TerminalNode *> GPULangParser::SwitchStatementContext::INTEGERLITERAL() {
  return getTokens(GPULangParser::INTEGERLITERAL);
}

tree::TerminalNode* GPULangParser::SwitchStatementContext::INTEGERLITERAL(size_t i) {
  return getToken(GPULangParser::INTEGERLITERAL, i);
}

std::vector<tree::TerminalNode *> GPULangParser::SwitchStatementContext::COL() {
  return getTokens(GPULangParser::COL);
}

tree::TerminalNode* GPULangParser::SwitchStatementContext::COL(size_t i) {
  return getToken(GPULangParser::COL, i);
}

std::vector<GPULangParser::StatementContext *> GPULangParser::SwitchStatementContext::statement() {
  return getRuleContexts<GPULangParser::StatementContext>();
}

GPULangParser::StatementContext* GPULangParser::SwitchStatementContext::statement(size_t i) {
  return getRuleContext<GPULangParser::StatementContext>(i);
}


size_t GPULangParser::SwitchStatementContext::getRuleIndex() const {
  return GPULangParser::RuleSwitchStatement;
}

void GPULangParser::SwitchStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSwitchStatement(this);
}

void GPULangParser::SwitchStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSwitchStatement(this);
}

GPULangParser::SwitchStatementContext* GPULangParser::switchStatement() {
  SwitchStatementContext *_localctx = _tracker.createInstance<SwitchStatementContext>(_ctx, getState());
  enterRule(_localctx, 56, GPULangParser::RuleSwitchStatement);

          antlrcpp::downCast<SwitchStatementContext *>(_localctx)->tree =  nullptr;
          Expression* switchExpression;
          std::vector<Expression*> caseExpressions;
          std::vector<Statement*> caseStatements;
          Symbol::Location location;
          Statement* defaultStatement = nullptr;
      
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(594);
    match(GPULangParser::T__18);
     location = SetupFile(); 
    setState(596);
    match(GPULangParser::LP);
    setState(597);
    antlrcpp::downCast<SwitchStatementContext *>(_localctx)->expressionContext = expression();
    setState(598);
    match(GPULangParser::RP);
     switchExpression = antlrcpp::downCast<SwitchStatementContext *>(_localctx)->expressionContext->tree; 
    setState(600);
    match(GPULangParser::LB);
    setState(612);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == GPULangParser::T__19) {
      setState(601);
      match(GPULangParser::T__19);
      setState(602);
      antlrcpp::downCast<SwitchStatementContext *>(_localctx)->integerliteralToken = match(GPULangParser::INTEGERLITERAL);
      setState(603);
      match(GPULangParser::COL);
       caseExpressions.push_back(new IntExpression(atoi((antlrcpp::downCast<SwitchStatementContext *>(_localctx)->integerliteralToken != nullptr ? antlrcpp::downCast<SwitchStatementContext *>(_localctx)->integerliteralToken->getText() : "").c_str()))); caseStatements.push_back(nullptr); 
      setState(608);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 108263700196993030) != 0) || ((((_la - 70) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 70)) & 14955) != 0)) {
        setState(605);
        antlrcpp::downCast<SwitchStatementContext *>(_localctx)->statementContext = statement();
         
                        caseStatements.back() = antlrcpp::downCast<SwitchStatementContext *>(_localctx)->statementContext->tree;
                    
      }
      setState(614);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(622);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == GPULangParser::T__20) {
      setState(615);
      match(GPULangParser::T__20);
      setState(616);
      match(GPULangParser::COL);
      setState(620);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 108263700196993030) != 0) || ((((_la - 70) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 70)) & 14955) != 0)) {
        setState(617);
        antlrcpp::downCast<SwitchStatementContext *>(_localctx)->statementContext = statement();

                        defaultStatement = antlrcpp::downCast<SwitchStatementContext *>(_localctx)->statementContext->tree;
                    
      }
    }
    setState(624);
    match(GPULangParser::RB);

            antlrcpp::downCast<SwitchStatementContext *>(_localctx)->tree =  new SwitchStatement(switchExpression, caseExpressions, caseStatements, defaultStatement);
            _localctx->tree->location = location;
        
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BreakStatementContext ------------------------------------------------------------------

GPULangParser::BreakStatementContext::BreakStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* GPULangParser::BreakStatementContext::SC() {
  return getToken(GPULangParser::SC, 0);
}


size_t GPULangParser::BreakStatementContext::getRuleIndex() const {
  return GPULangParser::RuleBreakStatement;
}

void GPULangParser::BreakStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBreakStatement(this);
}

void GPULangParser::BreakStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBreakStatement(this);
}

GPULangParser::BreakStatementContext* GPULangParser::breakStatement() {
  BreakStatementContext *_localctx = _tracker.createInstance<BreakStatementContext>(_ctx, getState());
  enterRule(_localctx, 58, GPULangParser::RuleBreakStatement);

          antlrcpp::downCast<BreakStatementContext *>(_localctx)->tree =  nullptr;
          Symbol::Location location;
      

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(627);
    match(GPULangParser::T__21);
     location = SetupFile(); 
    setState(629);
    match(GPULangParser::SC);

            antlrcpp::downCast<BreakStatementContext *>(_localctx)->tree =  new BreakStatement();
            _localctx->tree->location = location;
        
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpressionContext ------------------------------------------------------------------

GPULangParser::ExpressionContext::ExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

GPULangParser::CommaExpressionContext* GPULangParser::ExpressionContext::commaExpression() {
  return getRuleContext<GPULangParser::CommaExpressionContext>(0);
}


size_t GPULangParser::ExpressionContext::getRuleIndex() const {
  return GPULangParser::RuleExpression;
}

void GPULangParser::ExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExpression(this);
}

void GPULangParser::ExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExpression(this);
}

GPULangParser::ExpressionContext* GPULangParser::expression() {
  ExpressionContext *_localctx = _tracker.createInstance<ExpressionContext>(_ctx, getState());
  enterRule(_localctx, 60, GPULangParser::RuleExpression);

          antlrcpp::downCast<ExpressionContext *>(_localctx)->tree =  nullptr;
      

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(632);
    antlrcpp::downCast<ExpressionContext *>(_localctx)->commaExpressionContext = commaExpression();
     antlrcpp::downCast<ExpressionContext *>(_localctx)->tree =  antlrcpp::downCast<ExpressionContext *>(_localctx)->commaExpressionContext->tree; _localctx->tree->location = SetupFile(); 
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CommaExpressionContext ------------------------------------------------------------------

GPULangParser::CommaExpressionContext::CommaExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<GPULangParser::AssignmentExpressionContext *> GPULangParser::CommaExpressionContext::assignmentExpression() {
  return getRuleContexts<GPULangParser::AssignmentExpressionContext>();
}

GPULangParser::AssignmentExpressionContext* GPULangParser::CommaExpressionContext::assignmentExpression(size_t i) {
  return getRuleContext<GPULangParser::AssignmentExpressionContext>(i);
}

std::vector<tree::TerminalNode *> GPULangParser::CommaExpressionContext::CO() {
  return getTokens(GPULangParser::CO);
}

tree::TerminalNode* GPULangParser::CommaExpressionContext::CO(size_t i) {
  return getToken(GPULangParser::CO, i);
}


size_t GPULangParser::CommaExpressionContext::getRuleIndex() const {
  return GPULangParser::RuleCommaExpression;
}

void GPULangParser::CommaExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCommaExpression(this);
}

void GPULangParser::CommaExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCommaExpression(this);
}

GPULangParser::CommaExpressionContext* GPULangParser::commaExpression() {
  CommaExpressionContext *_localctx = _tracker.createInstance<CommaExpressionContext>(_ctx, getState());
  enterRule(_localctx, 62, GPULangParser::RuleCommaExpression);

          antlrcpp::downCast<CommaExpressionContext *>(_localctx)->tree =  nullptr;
          Symbol::Location location;
      

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(635);
    antlrcpp::downCast<CommaExpressionContext *>(_localctx)->e1 = assignmentExpression();
     antlrcpp::downCast<CommaExpressionContext *>(_localctx)->tree =  antlrcpp::downCast<CommaExpressionContext *>(_localctx)->e1->tree; 
    setState(644);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 50, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(637);
        match(GPULangParser::CO);
         location = SetupFile(); 
        setState(639);
        antlrcpp::downCast<CommaExpressionContext *>(_localctx)->e2 = assignmentExpression();

                    CommaExpression* expr = new CommaExpression(_localctx->tree, antlrcpp::downCast<CommaExpressionContext *>(_localctx)->e2->tree);
                    expr->location = location;
                    antlrcpp::downCast<CommaExpressionContext *>(_localctx)->tree =  expr;
                 
      }
      setState(646);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 50, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AssignmentExpressionContext ------------------------------------------------------------------

GPULangParser::AssignmentExpressionContext::AssignmentExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<GPULangParser::LogicalOrExpressionContext *> GPULangParser::AssignmentExpressionContext::logicalOrExpression() {
  return getRuleContexts<GPULangParser::LogicalOrExpressionContext>();
}

GPULangParser::LogicalOrExpressionContext* GPULangParser::AssignmentExpressionContext::logicalOrExpression(size_t i) {
  return getRuleContext<GPULangParser::LogicalOrExpressionContext>(i);
}

std::vector<tree::TerminalNode *> GPULangParser::AssignmentExpressionContext::QU() {
  return getTokens(GPULangParser::QU);
}

tree::TerminalNode* GPULangParser::AssignmentExpressionContext::QU(size_t i) {
  return getToken(GPULangParser::QU, i);
}

std::vector<tree::TerminalNode *> GPULangParser::AssignmentExpressionContext::COL() {
  return getTokens(GPULangParser::COL);
}

tree::TerminalNode* GPULangParser::AssignmentExpressionContext::COL(size_t i) {
  return getToken(GPULangParser::COL, i);
}

std::vector<GPULangParser::ExpressionContext *> GPULangParser::AssignmentExpressionContext::expression() {
  return getRuleContexts<GPULangParser::ExpressionContext>();
}

GPULangParser::ExpressionContext* GPULangParser::AssignmentExpressionContext::expression(size_t i) {
  return getRuleContext<GPULangParser::ExpressionContext>(i);
}

std::vector<tree::TerminalNode *> GPULangParser::AssignmentExpressionContext::ANDSET() {
  return getTokens(GPULangParser::ANDSET);
}

tree::TerminalNode* GPULangParser::AssignmentExpressionContext::ANDSET(size_t i) {
  return getToken(GPULangParser::ANDSET, i);
}

std::vector<tree::TerminalNode *> GPULangParser::AssignmentExpressionContext::XORSET() {
  return getTokens(GPULangParser::XORSET);
}

tree::TerminalNode* GPULangParser::AssignmentExpressionContext::XORSET(size_t i) {
  return getToken(GPULangParser::XORSET, i);
}

std::vector<tree::TerminalNode *> GPULangParser::AssignmentExpressionContext::ORSET() {
  return getTokens(GPULangParser::ORSET);
}

tree::TerminalNode* GPULangParser::AssignmentExpressionContext::ORSET(size_t i) {
  return getToken(GPULangParser::ORSET, i);
}

std::vector<tree::TerminalNode *> GPULangParser::AssignmentExpressionContext::EQ() {
  return getTokens(GPULangParser::EQ);
}

tree::TerminalNode* GPULangParser::AssignmentExpressionContext::EQ(size_t i) {
  return getToken(GPULangParser::EQ, i);
}


size_t GPULangParser::AssignmentExpressionContext::getRuleIndex() const {
  return GPULangParser::RuleAssignmentExpression;
}

void GPULangParser::AssignmentExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAssignmentExpression(this);
}

void GPULangParser::AssignmentExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAssignmentExpression(this);
}

GPULangParser::AssignmentExpressionContext* GPULangParser::assignmentExpression() {
  AssignmentExpressionContext *_localctx = _tracker.createInstance<AssignmentExpressionContext>(_ctx, getState());
  enterRule(_localctx, 64, GPULangParser::RuleAssignmentExpression);

          antlrcpp::downCast<AssignmentExpressionContext *>(_localctx)->tree =  nullptr;
          Symbol::Location location;
      
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(647);
    antlrcpp::downCast<AssignmentExpressionContext *>(_localctx)->e1 = logicalOrExpression();
     antlrcpp::downCast<AssignmentExpressionContext *>(_localctx)->tree =  antlrcpp::downCast<AssignmentExpressionContext *>(_localctx)->e1->tree; 
    setState(662);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 52, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(660);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case GPULangParser::T__22:
          case GPULangParser::T__23:
          case GPULangParser::T__24:
          case GPULangParser::T__25:
          case GPULangParser::T__26:
          case GPULangParser::T__27:
          case GPULangParser::T__28:
          case GPULangParser::EQ:
          case GPULangParser::ANDSET:
          case GPULangParser::ORSET:
          case GPULangParser::XORSET: {
            setState(649);
            antlrcpp::downCast<AssignmentExpressionContext *>(_localctx)->op = _input->LT(1);
            _la = _input->LA(1);
            if (!((((_la & ~ 0x3fULL) == 0) &&
              ((1ULL << _la) & 23714267853225984) != 0))) {
              antlrcpp::downCast<AssignmentExpressionContext *>(_localctx)->op = _errHandler->recoverInline(this);
            }
            else {
              _errHandler->reportMatch(this);
              consume();
            }
            setState(650);
            antlrcpp::downCast<AssignmentExpressionContext *>(_localctx)->e2 = logicalOrExpression();
             
                        BinaryExpression* expr = new BinaryExpression(StringToFourCC((antlrcpp::downCast<AssignmentExpressionContext *>(_localctx)->op != nullptr ? antlrcpp::downCast<AssignmentExpressionContext *>(_localctx)->op->getText() : "")), _localctx->tree, antlrcpp::downCast<AssignmentExpressionContext *>(_localctx)->e2->tree);
                        expr->location = SetupFile();
                        antlrcpp::downCast<AssignmentExpressionContext *>(_localctx)->tree =  expr;
                    
            break;
          }

          case GPULangParser::QU: {
            setState(653);
            match(GPULangParser::QU);
             location = SetupFile(); 
            setState(655);
            antlrcpp::downCast<AssignmentExpressionContext *>(_localctx)->ifBody = expression();
            setState(656);
            match(GPULangParser::COL);
            setState(657);
            antlrcpp::downCast<AssignmentExpressionContext *>(_localctx)->elseBody = expression();
             
                        TernaryExpression* expr = new TernaryExpression(_localctx->tree, antlrcpp::downCast<AssignmentExpressionContext *>(_localctx)->ifBody->tree, antlrcpp::downCast<AssignmentExpressionContext *>(_localctx)->elseBody->tree);
                        expr->location = location;
                        antlrcpp::downCast<AssignmentExpressionContext *>(_localctx)->tree =  expr;
                    
            break;
          }

        default:
          throw NoViableAltException(this);
        } 
      }
      setState(664);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 52, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LogicalOrExpressionContext ------------------------------------------------------------------

GPULangParser::LogicalOrExpressionContext::LogicalOrExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<GPULangParser::LogicalAndExpressionContext *> GPULangParser::LogicalOrExpressionContext::logicalAndExpression() {
  return getRuleContexts<GPULangParser::LogicalAndExpressionContext>();
}

GPULangParser::LogicalAndExpressionContext* GPULangParser::LogicalOrExpressionContext::logicalAndExpression(size_t i) {
  return getRuleContext<GPULangParser::LogicalAndExpressionContext>(i);
}

std::vector<tree::TerminalNode *> GPULangParser::LogicalOrExpressionContext::LOGICOR() {
  return getTokens(GPULangParser::LOGICOR);
}

tree::TerminalNode* GPULangParser::LogicalOrExpressionContext::LOGICOR(size_t i) {
  return getToken(GPULangParser::LOGICOR, i);
}


size_t GPULangParser::LogicalOrExpressionContext::getRuleIndex() const {
  return GPULangParser::RuleLogicalOrExpression;
}

void GPULangParser::LogicalOrExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLogicalOrExpression(this);
}

void GPULangParser::LogicalOrExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLogicalOrExpression(this);
}

GPULangParser::LogicalOrExpressionContext* GPULangParser::logicalOrExpression() {
  LogicalOrExpressionContext *_localctx = _tracker.createInstance<LogicalOrExpressionContext>(_ctx, getState());
  enterRule(_localctx, 66, GPULangParser::RuleLogicalOrExpression);

          antlrcpp::downCast<LogicalOrExpressionContext *>(_localctx)->tree =  nullptr;
          Symbol::Location location;
      
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(665);
    antlrcpp::downCast<LogicalOrExpressionContext *>(_localctx)->e1 = logicalAndExpression();
     antlrcpp::downCast<LogicalOrExpressionContext *>(_localctx)->tree =  antlrcpp::downCast<LogicalOrExpressionContext *>(_localctx)->e1->tree; 
    setState(674);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == GPULangParser::LOGICOR) {
      setState(667);
      match(GPULangParser::LOGICOR);
       location = SetupFile(); 
      setState(669);
      antlrcpp::downCast<LogicalOrExpressionContext *>(_localctx)->e2 = logicalAndExpression();

                  BinaryExpression* expr = new BinaryExpression('||', _localctx->tree, antlrcpp::downCast<LogicalOrExpressionContext *>(_localctx)->e2->tree);
                  expr->location = location;
                  antlrcpp::downCast<LogicalOrExpressionContext *>(_localctx)->tree =  expr;
              
      setState(676);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LogicalAndExpressionContext ------------------------------------------------------------------

GPULangParser::LogicalAndExpressionContext::LogicalAndExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<GPULangParser::OrExpressionContext *> GPULangParser::LogicalAndExpressionContext::orExpression() {
  return getRuleContexts<GPULangParser::OrExpressionContext>();
}

GPULangParser::OrExpressionContext* GPULangParser::LogicalAndExpressionContext::orExpression(size_t i) {
  return getRuleContext<GPULangParser::OrExpressionContext>(i);
}

std::vector<tree::TerminalNode *> GPULangParser::LogicalAndExpressionContext::LOGICAND() {
  return getTokens(GPULangParser::LOGICAND);
}

tree::TerminalNode* GPULangParser::LogicalAndExpressionContext::LOGICAND(size_t i) {
  return getToken(GPULangParser::LOGICAND, i);
}


size_t GPULangParser::LogicalAndExpressionContext::getRuleIndex() const {
  return GPULangParser::RuleLogicalAndExpression;
}

void GPULangParser::LogicalAndExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLogicalAndExpression(this);
}

void GPULangParser::LogicalAndExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLogicalAndExpression(this);
}

GPULangParser::LogicalAndExpressionContext* GPULangParser::logicalAndExpression() {
  LogicalAndExpressionContext *_localctx = _tracker.createInstance<LogicalAndExpressionContext>(_ctx, getState());
  enterRule(_localctx, 68, GPULangParser::RuleLogicalAndExpression);

          antlrcpp::downCast<LogicalAndExpressionContext *>(_localctx)->tree =  nullptr;
          Symbol::Location location;

      
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(677);
    antlrcpp::downCast<LogicalAndExpressionContext *>(_localctx)->e1 = orExpression();
     antlrcpp::downCast<LogicalAndExpressionContext *>(_localctx)->tree =  antlrcpp::downCast<LogicalAndExpressionContext *>(_localctx)->e1->tree; 
    setState(686);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == GPULangParser::LOGICAND) {
      setState(679);
      match(GPULangParser::LOGICAND);
       location = SetupFile(); 
      setState(681);
      antlrcpp::downCast<LogicalAndExpressionContext *>(_localctx)->e2 = orExpression();

                  BinaryExpression* expr = new BinaryExpression('&&', _localctx->tree, antlrcpp::downCast<LogicalAndExpressionContext *>(_localctx)->e2->tree);
                  expr->location = location;
                  antlrcpp::downCast<LogicalAndExpressionContext *>(_localctx)->tree =  expr;
              
      setState(688);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OrExpressionContext ------------------------------------------------------------------

GPULangParser::OrExpressionContext::OrExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<GPULangParser::XorExpressionContext *> GPULangParser::OrExpressionContext::xorExpression() {
  return getRuleContexts<GPULangParser::XorExpressionContext>();
}

GPULangParser::XorExpressionContext* GPULangParser::OrExpressionContext::xorExpression(size_t i) {
  return getRuleContext<GPULangParser::XorExpressionContext>(i);
}

std::vector<tree::TerminalNode *> GPULangParser::OrExpressionContext::OR() {
  return getTokens(GPULangParser::OR);
}

tree::TerminalNode* GPULangParser::OrExpressionContext::OR(size_t i) {
  return getToken(GPULangParser::OR, i);
}


size_t GPULangParser::OrExpressionContext::getRuleIndex() const {
  return GPULangParser::RuleOrExpression;
}

void GPULangParser::OrExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterOrExpression(this);
}

void GPULangParser::OrExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitOrExpression(this);
}

GPULangParser::OrExpressionContext* GPULangParser::orExpression() {
  OrExpressionContext *_localctx = _tracker.createInstance<OrExpressionContext>(_ctx, getState());
  enterRule(_localctx, 70, GPULangParser::RuleOrExpression);

          antlrcpp::downCast<OrExpressionContext *>(_localctx)->tree =  nullptr;
          Symbol::Location location;

      
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(689);
    antlrcpp::downCast<OrExpressionContext *>(_localctx)->e1 = xorExpression();
     antlrcpp::downCast<OrExpressionContext *>(_localctx)->tree =  antlrcpp::downCast<OrExpressionContext *>(_localctx)->e1->tree; 
    setState(698);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == GPULangParser::OR) {
      setState(691);
      match(GPULangParser::OR);
       location = SetupFile(); 
      setState(693);
      antlrcpp::downCast<OrExpressionContext *>(_localctx)->e2 = xorExpression();

                  BinaryExpression* expr = new BinaryExpression('|', _localctx->tree, antlrcpp::downCast<OrExpressionContext *>(_localctx)->e2->tree);
                  expr->location = location;
                  antlrcpp::downCast<OrExpressionContext *>(_localctx)->tree =  expr;
              
      setState(700);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- XorExpressionContext ------------------------------------------------------------------

GPULangParser::XorExpressionContext::XorExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<GPULangParser::AndExpressionContext *> GPULangParser::XorExpressionContext::andExpression() {
  return getRuleContexts<GPULangParser::AndExpressionContext>();
}

GPULangParser::AndExpressionContext* GPULangParser::XorExpressionContext::andExpression(size_t i) {
  return getRuleContext<GPULangParser::AndExpressionContext>(i);
}

std::vector<tree::TerminalNode *> GPULangParser::XorExpressionContext::XOR() {
  return getTokens(GPULangParser::XOR);
}

tree::TerminalNode* GPULangParser::XorExpressionContext::XOR(size_t i) {
  return getToken(GPULangParser::XOR, i);
}


size_t GPULangParser::XorExpressionContext::getRuleIndex() const {
  return GPULangParser::RuleXorExpression;
}

void GPULangParser::XorExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterXorExpression(this);
}

void GPULangParser::XorExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitXorExpression(this);
}

GPULangParser::XorExpressionContext* GPULangParser::xorExpression() {
  XorExpressionContext *_localctx = _tracker.createInstance<XorExpressionContext>(_ctx, getState());
  enterRule(_localctx, 72, GPULangParser::RuleXorExpression);

          antlrcpp::downCast<XorExpressionContext *>(_localctx)->tree =  nullptr;
          Symbol::Location location;
      
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(701);
    antlrcpp::downCast<XorExpressionContext *>(_localctx)->e1 = andExpression();
     antlrcpp::downCast<XorExpressionContext *>(_localctx)->tree =  antlrcpp::downCast<XorExpressionContext *>(_localctx)->e1->tree; 
    setState(710);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == GPULangParser::XOR) {
      setState(703);
      match(GPULangParser::XOR);
       location = SetupFile(); 
      setState(705);
      antlrcpp::downCast<XorExpressionContext *>(_localctx)->e2 = andExpression();

                  BinaryExpression* expr = new BinaryExpression('^', _localctx->tree, antlrcpp::downCast<XorExpressionContext *>(_localctx)->e2->tree);
                  expr->location = location;
                  antlrcpp::downCast<XorExpressionContext *>(_localctx)->tree =  expr;
              
      setState(712);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AndExpressionContext ------------------------------------------------------------------

GPULangParser::AndExpressionContext::AndExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<GPULangParser::EquivalencyExpressionContext *> GPULangParser::AndExpressionContext::equivalencyExpression() {
  return getRuleContexts<GPULangParser::EquivalencyExpressionContext>();
}

GPULangParser::EquivalencyExpressionContext* GPULangParser::AndExpressionContext::equivalencyExpression(size_t i) {
  return getRuleContext<GPULangParser::EquivalencyExpressionContext>(i);
}

std::vector<tree::TerminalNode *> GPULangParser::AndExpressionContext::AND() {
  return getTokens(GPULangParser::AND);
}

tree::TerminalNode* GPULangParser::AndExpressionContext::AND(size_t i) {
  return getToken(GPULangParser::AND, i);
}


size_t GPULangParser::AndExpressionContext::getRuleIndex() const {
  return GPULangParser::RuleAndExpression;
}

void GPULangParser::AndExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAndExpression(this);
}

void GPULangParser::AndExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAndExpression(this);
}

GPULangParser::AndExpressionContext* GPULangParser::andExpression() {
  AndExpressionContext *_localctx = _tracker.createInstance<AndExpressionContext>(_ctx, getState());
  enterRule(_localctx, 74, GPULangParser::RuleAndExpression);

          antlrcpp::downCast<AndExpressionContext *>(_localctx)->tree =  nullptr;
          Symbol::Location location;
      
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(713);
    antlrcpp::downCast<AndExpressionContext *>(_localctx)->e1 = equivalencyExpression();
     antlrcpp::downCast<AndExpressionContext *>(_localctx)->tree =  antlrcpp::downCast<AndExpressionContext *>(_localctx)->e1->tree;	
    setState(722);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == GPULangParser::AND) {
      setState(715);
      match(GPULangParser::AND);
       location = SetupFile(); 
      setState(717);
      antlrcpp::downCast<AndExpressionContext *>(_localctx)->e2 = equivalencyExpression();

                  BinaryExpression* expr = new BinaryExpression('&', _localctx->tree, antlrcpp::downCast<AndExpressionContext *>(_localctx)->e2->tree);
                  expr->location = location;
                  antlrcpp::downCast<AndExpressionContext *>(_localctx)->tree =  expr;
              
      setState(724);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EquivalencyExpressionContext ------------------------------------------------------------------

GPULangParser::EquivalencyExpressionContext::EquivalencyExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<GPULangParser::RelationalExpressionContext *> GPULangParser::EquivalencyExpressionContext::relationalExpression() {
  return getRuleContexts<GPULangParser::RelationalExpressionContext>();
}

GPULangParser::RelationalExpressionContext* GPULangParser::EquivalencyExpressionContext::relationalExpression(size_t i) {
  return getRuleContext<GPULangParser::RelationalExpressionContext>(i);
}

std::vector<tree::TerminalNode *> GPULangParser::EquivalencyExpressionContext::LOGICEQ() {
  return getTokens(GPULangParser::LOGICEQ);
}

tree::TerminalNode* GPULangParser::EquivalencyExpressionContext::LOGICEQ(size_t i) {
  return getToken(GPULangParser::LOGICEQ, i);
}

std::vector<tree::TerminalNode *> GPULangParser::EquivalencyExpressionContext::NOTEQ() {
  return getTokens(GPULangParser::NOTEQ);
}

tree::TerminalNode* GPULangParser::EquivalencyExpressionContext::NOTEQ(size_t i) {
  return getToken(GPULangParser::NOTEQ, i);
}


size_t GPULangParser::EquivalencyExpressionContext::getRuleIndex() const {
  return GPULangParser::RuleEquivalencyExpression;
}

void GPULangParser::EquivalencyExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterEquivalencyExpression(this);
}

void GPULangParser::EquivalencyExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitEquivalencyExpression(this);
}

GPULangParser::EquivalencyExpressionContext* GPULangParser::equivalencyExpression() {
  EquivalencyExpressionContext *_localctx = _tracker.createInstance<EquivalencyExpressionContext>(_ctx, getState());
  enterRule(_localctx, 76, GPULangParser::RuleEquivalencyExpression);

          antlrcpp::downCast<EquivalencyExpressionContext *>(_localctx)->tree =  nullptr;
          Symbol::Location location;
      
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(725);
    antlrcpp::downCast<EquivalencyExpressionContext *>(_localctx)->e1 = relationalExpression();
     antlrcpp::downCast<EquivalencyExpressionContext *>(_localctx)->tree =  antlrcpp::downCast<EquivalencyExpressionContext *>(_localctx)->e1->tree; 
    setState(734);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == GPULangParser::LOGICEQ

    || _la == GPULangParser::NOTEQ) {
      setState(727);
      antlrcpp::downCast<EquivalencyExpressionContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!(_la == GPULangParser::LOGICEQ

      || _la == GPULangParser::NOTEQ)) {
        antlrcpp::downCast<EquivalencyExpressionContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
       location = SetupFile(); 
      setState(729);
      antlrcpp::downCast<EquivalencyExpressionContext *>(_localctx)->e2 = relationalExpression();

                  BinaryExpression* expr = new BinaryExpression(StringToFourCC((antlrcpp::downCast<EquivalencyExpressionContext *>(_localctx)->op != nullptr ? antlrcpp::downCast<EquivalencyExpressionContext *>(_localctx)->op->getText() : "")), _localctx->tree, antlrcpp::downCast<EquivalencyExpressionContext *>(_localctx)->e2->tree);
                  expr->location = location;
                  antlrcpp::downCast<EquivalencyExpressionContext *>(_localctx)->tree =  expr;
              
      setState(736);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RelationalExpressionContext ------------------------------------------------------------------

GPULangParser::RelationalExpressionContext::RelationalExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<GPULangParser::ShiftExpressionContext *> GPULangParser::RelationalExpressionContext::shiftExpression() {
  return getRuleContexts<GPULangParser::ShiftExpressionContext>();
}

GPULangParser::ShiftExpressionContext* GPULangParser::RelationalExpressionContext::shiftExpression(size_t i) {
  return getRuleContext<GPULangParser::ShiftExpressionContext>(i);
}

std::vector<tree::TerminalNode *> GPULangParser::RelationalExpressionContext::LESS() {
  return getTokens(GPULangParser::LESS);
}

tree::TerminalNode* GPULangParser::RelationalExpressionContext::LESS(size_t i) {
  return getToken(GPULangParser::LESS, i);
}

std::vector<tree::TerminalNode *> GPULangParser::RelationalExpressionContext::GREATER() {
  return getTokens(GPULangParser::GREATER);
}

tree::TerminalNode* GPULangParser::RelationalExpressionContext::GREATER(size_t i) {
  return getToken(GPULangParser::GREATER, i);
}

std::vector<tree::TerminalNode *> GPULangParser::RelationalExpressionContext::LESSEQ() {
  return getTokens(GPULangParser::LESSEQ);
}

tree::TerminalNode* GPULangParser::RelationalExpressionContext::LESSEQ(size_t i) {
  return getToken(GPULangParser::LESSEQ, i);
}

std::vector<tree::TerminalNode *> GPULangParser::RelationalExpressionContext::GREATEREQ() {
  return getTokens(GPULangParser::GREATEREQ);
}

tree::TerminalNode* GPULangParser::RelationalExpressionContext::GREATEREQ(size_t i) {
  return getToken(GPULangParser::GREATEREQ, i);
}


size_t GPULangParser::RelationalExpressionContext::getRuleIndex() const {
  return GPULangParser::RuleRelationalExpression;
}

void GPULangParser::RelationalExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterRelationalExpression(this);
}

void GPULangParser::RelationalExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitRelationalExpression(this);
}

GPULangParser::RelationalExpressionContext* GPULangParser::relationalExpression() {
  RelationalExpressionContext *_localctx = _tracker.createInstance<RelationalExpressionContext>(_ctx, getState());
  enterRule(_localctx, 78, GPULangParser::RuleRelationalExpression);

          antlrcpp::downCast<RelationalExpressionContext *>(_localctx)->tree =  nullptr;
          Symbol::Location location;
      
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(737);
    antlrcpp::downCast<RelationalExpressionContext *>(_localctx)->e1 = shiftExpression();
     antlrcpp::downCast<RelationalExpressionContext *>(_localctx)->tree =  antlrcpp::downCast<RelationalExpressionContext *>(_localctx)->e1->tree; 
    setState(746);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 8646911284551352320) != 0)) {
      setState(739);
      antlrcpp::downCast<RelationalExpressionContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 8646911284551352320) != 0))) {
        antlrcpp::downCast<RelationalExpressionContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
       location = SetupFile(); 
      setState(741);
      antlrcpp::downCast<RelationalExpressionContext *>(_localctx)->e2 = shiftExpression();

                  BinaryExpression* expr = new BinaryExpression(StringToFourCC((antlrcpp::downCast<RelationalExpressionContext *>(_localctx)->op != nullptr ? antlrcpp::downCast<RelationalExpressionContext *>(_localctx)->op->getText() : "")), _localctx->tree, antlrcpp::downCast<RelationalExpressionContext *>(_localctx)->e2->tree);
                  expr->location = location;
                  antlrcpp::downCast<RelationalExpressionContext *>(_localctx)->tree =  expr;
              
      setState(748);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ShiftExpressionContext ------------------------------------------------------------------

GPULangParser::ShiftExpressionContext::ShiftExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<GPULangParser::AddSubtractExpressionContext *> GPULangParser::ShiftExpressionContext::addSubtractExpression() {
  return getRuleContexts<GPULangParser::AddSubtractExpressionContext>();
}

GPULangParser::AddSubtractExpressionContext* GPULangParser::ShiftExpressionContext::addSubtractExpression(size_t i) {
  return getRuleContext<GPULangParser::AddSubtractExpressionContext>(i);
}


size_t GPULangParser::ShiftExpressionContext::getRuleIndex() const {
  return GPULangParser::RuleShiftExpression;
}

void GPULangParser::ShiftExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterShiftExpression(this);
}

void GPULangParser::ShiftExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitShiftExpression(this);
}

GPULangParser::ShiftExpressionContext* GPULangParser::shiftExpression() {
  ShiftExpressionContext *_localctx = _tracker.createInstance<ShiftExpressionContext>(_ctx, getState());
  enterRule(_localctx, 80, GPULangParser::RuleShiftExpression);

          antlrcpp::downCast<ShiftExpressionContext *>(_localctx)->tree =  nullptr;
          Symbol::Location location;
      
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(749);
    antlrcpp::downCast<ShiftExpressionContext *>(_localctx)->e1 = addSubtractExpression();
     antlrcpp::downCast<ShiftExpressionContext *>(_localctx)->tree =  antlrcpp::downCast<ShiftExpressionContext *>(_localctx)->e1->tree; 
    setState(758);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == GPULangParser::T__29

    || _la == GPULangParser::T__30) {
      setState(751);
      antlrcpp::downCast<ShiftExpressionContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!(_la == GPULangParser::T__29

      || _la == GPULangParser::T__30)) {
        antlrcpp::downCast<ShiftExpressionContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
       location = SetupFile(); 
      setState(753);
      antlrcpp::downCast<ShiftExpressionContext *>(_localctx)->e2 = addSubtractExpression();

                  BinaryExpression* expr = new BinaryExpression(StringToFourCC((antlrcpp::downCast<ShiftExpressionContext *>(_localctx)->op != nullptr ? antlrcpp::downCast<ShiftExpressionContext *>(_localctx)->op->getText() : "")), _localctx->tree, antlrcpp::downCast<ShiftExpressionContext *>(_localctx)->e2->tree);
                  expr->location = location;
                  antlrcpp::downCast<ShiftExpressionContext *>(_localctx)->tree =  expr;
              
      setState(760);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AddSubtractExpressionContext ------------------------------------------------------------------

GPULangParser::AddSubtractExpressionContext::AddSubtractExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<GPULangParser::MultiplyDivideExpressionContext *> GPULangParser::AddSubtractExpressionContext::multiplyDivideExpression() {
  return getRuleContexts<GPULangParser::MultiplyDivideExpressionContext>();
}

GPULangParser::MultiplyDivideExpressionContext* GPULangParser::AddSubtractExpressionContext::multiplyDivideExpression(size_t i) {
  return getRuleContext<GPULangParser::MultiplyDivideExpressionContext>(i);
}

std::vector<tree::TerminalNode *> GPULangParser::AddSubtractExpressionContext::ADD_OP() {
  return getTokens(GPULangParser::ADD_OP);
}

tree::TerminalNode* GPULangParser::AddSubtractExpressionContext::ADD_OP(size_t i) {
  return getToken(GPULangParser::ADD_OP, i);
}

std::vector<tree::TerminalNode *> GPULangParser::AddSubtractExpressionContext::SUB_OP() {
  return getTokens(GPULangParser::SUB_OP);
}

tree::TerminalNode* GPULangParser::AddSubtractExpressionContext::SUB_OP(size_t i) {
  return getToken(GPULangParser::SUB_OP, i);
}


size_t GPULangParser::AddSubtractExpressionContext::getRuleIndex() const {
  return GPULangParser::RuleAddSubtractExpression;
}

void GPULangParser::AddSubtractExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAddSubtractExpression(this);
}

void GPULangParser::AddSubtractExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAddSubtractExpression(this);
}

GPULangParser::AddSubtractExpressionContext* GPULangParser::addSubtractExpression() {
  AddSubtractExpressionContext *_localctx = _tracker.createInstance<AddSubtractExpressionContext>(_ctx, getState());
  enterRule(_localctx, 82, GPULangParser::RuleAddSubtractExpression);

          antlrcpp::downCast<AddSubtractExpressionContext *>(_localctx)->tree =  nullptr;
          Symbol::Location location;
      
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(761);
    antlrcpp::downCast<AddSubtractExpressionContext *>(_localctx)->e1 = multiplyDivideExpression();
     antlrcpp::downCast<AddSubtractExpressionContext *>(_localctx)->tree =  antlrcpp::downCast<AddSubtractExpressionContext *>(_localctx)->e1->tree; 
    setState(770);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == GPULangParser::ADD_OP

    || _la == GPULangParser::SUB_OP) {
      setState(763);
      antlrcpp::downCast<AddSubtractExpressionContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!(_la == GPULangParser::ADD_OP

      || _la == GPULangParser::SUB_OP)) {
        antlrcpp::downCast<AddSubtractExpressionContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
       location = SetupFile(); 
      setState(765);
      antlrcpp::downCast<AddSubtractExpressionContext *>(_localctx)->e2 = multiplyDivideExpression();

                  BinaryExpression* expr = new BinaryExpression(StringToFourCC((antlrcpp::downCast<AddSubtractExpressionContext *>(_localctx)->op != nullptr ? antlrcpp::downCast<AddSubtractExpressionContext *>(_localctx)->op->getText() : "")), _localctx->tree, antlrcpp::downCast<AddSubtractExpressionContext *>(_localctx)->e2->tree);
                  expr->location = location;
                  antlrcpp::downCast<AddSubtractExpressionContext *>(_localctx)->tree =  expr;
              
      setState(772);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MultiplyDivideExpressionContext ------------------------------------------------------------------

GPULangParser::MultiplyDivideExpressionContext::MultiplyDivideExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<GPULangParser::PrefixExpressionContext *> GPULangParser::MultiplyDivideExpressionContext::prefixExpression() {
  return getRuleContexts<GPULangParser::PrefixExpressionContext>();
}

GPULangParser::PrefixExpressionContext* GPULangParser::MultiplyDivideExpressionContext::prefixExpression(size_t i) {
  return getRuleContext<GPULangParser::PrefixExpressionContext>(i);
}

std::vector<tree::TerminalNode *> GPULangParser::MultiplyDivideExpressionContext::MUL_OP() {
  return getTokens(GPULangParser::MUL_OP);
}

tree::TerminalNode* GPULangParser::MultiplyDivideExpressionContext::MUL_OP(size_t i) {
  return getToken(GPULangParser::MUL_OP, i);
}

std::vector<tree::TerminalNode *> GPULangParser::MultiplyDivideExpressionContext::DIV_OP() {
  return getTokens(GPULangParser::DIV_OP);
}

tree::TerminalNode* GPULangParser::MultiplyDivideExpressionContext::DIV_OP(size_t i) {
  return getToken(GPULangParser::DIV_OP, i);
}

std::vector<tree::TerminalNode *> GPULangParser::MultiplyDivideExpressionContext::MOD() {
  return getTokens(GPULangParser::MOD);
}

tree::TerminalNode* GPULangParser::MultiplyDivideExpressionContext::MOD(size_t i) {
  return getToken(GPULangParser::MOD, i);
}


size_t GPULangParser::MultiplyDivideExpressionContext::getRuleIndex() const {
  return GPULangParser::RuleMultiplyDivideExpression;
}

void GPULangParser::MultiplyDivideExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMultiplyDivideExpression(this);
}

void GPULangParser::MultiplyDivideExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMultiplyDivideExpression(this);
}

GPULangParser::MultiplyDivideExpressionContext* GPULangParser::multiplyDivideExpression() {
  MultiplyDivideExpressionContext *_localctx = _tracker.createInstance<MultiplyDivideExpressionContext>(_ctx, getState());
  enterRule(_localctx, 84, GPULangParser::RuleMultiplyDivideExpression);

          antlrcpp::downCast<MultiplyDivideExpressionContext *>(_localctx)->tree =  nullptr;
          Symbol::Location location;
      
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(773);
    antlrcpp::downCast<MultiplyDivideExpressionContext *>(_localctx)->e1 = prefixExpression();
     antlrcpp::downCast<MultiplyDivideExpressionContext *>(_localctx)->tree =  antlrcpp::downCast<MultiplyDivideExpressionContext *>(_localctx)->e1->tree; 
    setState(782);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (((((_la - 67) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 67)) & 97) != 0)) {
      setState(775);
      antlrcpp::downCast<MultiplyDivideExpressionContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!(((((_la - 67) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 67)) & 97) != 0))) {
        antlrcpp::downCast<MultiplyDivideExpressionContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
       location = SetupFile(); 
      setState(777);
      antlrcpp::downCast<MultiplyDivideExpressionContext *>(_localctx)->e2 = prefixExpression();

                  BinaryExpression* expr = new BinaryExpression(StringToFourCC((antlrcpp::downCast<MultiplyDivideExpressionContext *>(_localctx)->op != nullptr ? antlrcpp::downCast<MultiplyDivideExpressionContext *>(_localctx)->op->getText() : "")), _localctx->tree, antlrcpp::downCast<MultiplyDivideExpressionContext *>(_localctx)->e2->tree);
                  expr->location = location;
                  antlrcpp::downCast<MultiplyDivideExpressionContext *>(_localctx)->tree =  expr;
              
      setState(784);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PrefixExpressionContext ------------------------------------------------------------------

GPULangParser::PrefixExpressionContext::PrefixExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

GPULangParser::SuffixExpressionContext* GPULangParser::PrefixExpressionContext::suffixExpression() {
  return getRuleContext<GPULangParser::SuffixExpressionContext>(0);
}

std::vector<tree::TerminalNode *> GPULangParser::PrefixExpressionContext::SUB_OP() {
  return getTokens(GPULangParser::SUB_OP);
}

tree::TerminalNode* GPULangParser::PrefixExpressionContext::SUB_OP(size_t i) {
  return getToken(GPULangParser::SUB_OP, i);
}

std::vector<tree::TerminalNode *> GPULangParser::PrefixExpressionContext::ADD_OP() {
  return getTokens(GPULangParser::ADD_OP);
}

tree::TerminalNode* GPULangParser::PrefixExpressionContext::ADD_OP(size_t i) {
  return getToken(GPULangParser::ADD_OP, i);
}

std::vector<tree::TerminalNode *> GPULangParser::PrefixExpressionContext::NOT() {
  return getTokens(GPULangParser::NOT);
}

tree::TerminalNode* GPULangParser::PrefixExpressionContext::NOT(size_t i) {
  return getToken(GPULangParser::NOT, i);
}

std::vector<tree::TerminalNode *> GPULangParser::PrefixExpressionContext::CONNJUGATE() {
  return getTokens(GPULangParser::CONNJUGATE);
}

tree::TerminalNode* GPULangParser::PrefixExpressionContext::CONNJUGATE(size_t i) {
  return getToken(GPULangParser::CONNJUGATE, i);
}

std::vector<tree::TerminalNode *> GPULangParser::PrefixExpressionContext::MUL_OP() {
  return getTokens(GPULangParser::MUL_OP);
}

tree::TerminalNode* GPULangParser::PrefixExpressionContext::MUL_OP(size_t i) {
  return getToken(GPULangParser::MUL_OP, i);
}


size_t GPULangParser::PrefixExpressionContext::getRuleIndex() const {
  return GPULangParser::RulePrefixExpression;
}

void GPULangParser::PrefixExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPrefixExpression(this);
}

void GPULangParser::PrefixExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPrefixExpression(this);
}

GPULangParser::PrefixExpressionContext* GPULangParser::prefixExpression() {
  PrefixExpressionContext *_localctx = _tracker.createInstance<PrefixExpressionContext>(_ctx, getState());
  enterRule(_localctx, 86, GPULangParser::RulePrefixExpression);

          antlrcpp::downCast<PrefixExpressionContext *>(_localctx)->tree =  nullptr;
          std::vector<uint32_t> ops;
          std::vector<Symbol::Location> locations;
      
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(789);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (((((_la - 32) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 32)) & 3023665373187) != 0)) {
      setState(785);
      antlrcpp::downCast<PrefixExpressionContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!(((((_la - 32) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 32)) & 3023665373187) != 0))) {
        antlrcpp::downCast<PrefixExpressionContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
       ops.push_back(StringToFourCC((antlrcpp::downCast<PrefixExpressionContext *>(_localctx)->op != nullptr ? antlrcpp::downCast<PrefixExpressionContext *>(_localctx)->op->getText() : ""))); locations.push_back(SetupFile()); 
      setState(791);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(792);
    antlrcpp::downCast<PrefixExpressionContext *>(_localctx)->e1 = suffixExpression();

            antlrcpp::downCast<PrefixExpressionContext *>(_localctx)->tree =  antlrcpp::downCast<PrefixExpressionContext *>(_localctx)->e1->tree;
            _localctx->tree->location = SetupFile();
            for (size_t i = 0; i < ops.size(); i++)
            {
                antlrcpp::downCast<PrefixExpressionContext *>(_localctx)->tree =  new UnaryExpression(ops[i], true, _localctx->tree);
                _localctx->tree->location = locations[i];
            }
        
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SuffixExpressionContext ------------------------------------------------------------------

GPULangParser::SuffixExpressionContext::SuffixExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

GPULangParser::BinaryexpatomContext* GPULangParser::SuffixExpressionContext::binaryexpatom() {
  return getRuleContext<GPULangParser::BinaryexpatomContext>(0);
}

std::vector<tree::TerminalNode *> GPULangParser::SuffixExpressionContext::LP() {
  return getTokens(GPULangParser::LP);
}

tree::TerminalNode* GPULangParser::SuffixExpressionContext::LP(size_t i) {
  return getToken(GPULangParser::LP, i);
}

std::vector<tree::TerminalNode *> GPULangParser::SuffixExpressionContext::RP() {
  return getTokens(GPULangParser::RP);
}

tree::TerminalNode* GPULangParser::SuffixExpressionContext::RP(size_t i) {
  return getToken(GPULangParser::RP, i);
}

std::vector<tree::TerminalNode *> GPULangParser::SuffixExpressionContext::DOT() {
  return getTokens(GPULangParser::DOT);
}

tree::TerminalNode* GPULangParser::SuffixExpressionContext::DOT(size_t i) {
  return getToken(GPULangParser::DOT, i);
}

std::vector<tree::TerminalNode *> GPULangParser::SuffixExpressionContext::ARROW() {
  return getTokens(GPULangParser::ARROW);
}

tree::TerminalNode* GPULangParser::SuffixExpressionContext::ARROW(size_t i) {
  return getToken(GPULangParser::ARROW, i);
}

std::vector<tree::TerminalNode *> GPULangParser::SuffixExpressionContext::LL() {
  return getTokens(GPULangParser::LL);
}

tree::TerminalNode* GPULangParser::SuffixExpressionContext::LL(size_t i) {
  return getToken(GPULangParser::LL, i);
}

std::vector<tree::TerminalNode *> GPULangParser::SuffixExpressionContext::RR() {
  return getTokens(GPULangParser::RR);
}

tree::TerminalNode* GPULangParser::SuffixExpressionContext::RR(size_t i) {
  return getToken(GPULangParser::RR, i);
}

std::vector<GPULangParser::SuffixExpressionContext *> GPULangParser::SuffixExpressionContext::suffixExpression() {
  return getRuleContexts<GPULangParser::SuffixExpressionContext>();
}

GPULangParser::SuffixExpressionContext* GPULangParser::SuffixExpressionContext::suffixExpression(size_t i) {
  return getRuleContext<GPULangParser::SuffixExpressionContext>(i);
}

std::vector<GPULangParser::LogicalOrExpressionContext *> GPULangParser::SuffixExpressionContext::logicalOrExpression() {
  return getRuleContexts<GPULangParser::LogicalOrExpressionContext>();
}

GPULangParser::LogicalOrExpressionContext* GPULangParser::SuffixExpressionContext::logicalOrExpression(size_t i) {
  return getRuleContext<GPULangParser::LogicalOrExpressionContext>(i);
}

std::vector<GPULangParser::ExpressionContext *> GPULangParser::SuffixExpressionContext::expression() {
  return getRuleContexts<GPULangParser::ExpressionContext>();
}

GPULangParser::ExpressionContext* GPULangParser::SuffixExpressionContext::expression(size_t i) {
  return getRuleContext<GPULangParser::ExpressionContext>(i);
}

std::vector<tree::TerminalNode *> GPULangParser::SuffixExpressionContext::CO() {
  return getTokens(GPULangParser::CO);
}

tree::TerminalNode* GPULangParser::SuffixExpressionContext::CO(size_t i) {
  return getToken(GPULangParser::CO, i);
}


size_t GPULangParser::SuffixExpressionContext::getRuleIndex() const {
  return GPULangParser::RuleSuffixExpression;
}

void GPULangParser::SuffixExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSuffixExpression(this);
}

void GPULangParser::SuffixExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSuffixExpression(this);
}

GPULangParser::SuffixExpressionContext* GPULangParser::suffixExpression() {
  SuffixExpressionContext *_localctx = _tracker.createInstance<SuffixExpressionContext>(_ctx, getState());
  enterRule(_localctx, 88, GPULangParser::RuleSuffixExpression);

          antlrcpp::downCast<SuffixExpressionContext *>(_localctx)->tree =  nullptr;

          Symbol::Location location;
          std::vector<Expression*> args;
          Expression* arrayIndexExpr = nullptr;

          std::vector<uint32_t> ops;
          std::vector<Symbol::Location> locations;
      
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    setState(848);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 70, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(795);
      antlrcpp::downCast<SuffixExpressionContext *>(_localctx)->e1 = binaryexpatom();
      setState(800);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == GPULangParser::T__31

      || _la == GPULangParser::T__32) {
        setState(796);
        antlrcpp::downCast<SuffixExpressionContext *>(_localctx)->op = _input->LT(1);
        _la = _input->LA(1);
        if (!(_la == GPULangParser::T__31

        || _la == GPULangParser::T__32)) {
          antlrcpp::downCast<SuffixExpressionContext *>(_localctx)->op = _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
         ops.push_back(StringToFourCC((antlrcpp::downCast<SuffixExpressionContext *>(_localctx)->op != nullptr ? antlrcpp::downCast<SuffixExpressionContext *>(_localctx)->op->getText() : ""))); locations.push_back(SetupFile()); 
        setState(802);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }

              antlrcpp::downCast<SuffixExpressionContext *>(_localctx)->tree =  antlrcpp::downCast<SuffixExpressionContext *>(_localctx)->e1->tree;
              _localctx->tree->location = SetupFile();
              for (size_t i = 0; i < ops.size(); i++)
              {
                  antlrcpp::downCast<SuffixExpressionContext *>(_localctx)->tree =  new UnaryExpression(ops[i], false, _localctx->tree);
                  _localctx->tree->location = locations[i];
              }
          
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(805);
      antlrcpp::downCast<SuffixExpressionContext *>(_localctx)->e1 = binaryexpatom();

              antlrcpp::downCast<SuffixExpressionContext *>(_localctx)->tree =  antlrcpp::downCast<SuffixExpressionContext *>(_localctx)->e1->tree;
          
      setState(845);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 69, _ctx);
      while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
        if (alt == 1) {
          setState(843);
          _errHandler->sync(this);
          switch (_input->LA(1)) {
            case GPULangParser::LP: {
              setState(807);
              match(GPULangParser::LP);
               location = SetupFile(); 
              setState(820);
              _errHandler->sync(this);

              _la = _input->LA(1);
              if ((((_la & ~ 0x3fULL) == 0) &&
                ((1ULL << _la) & 108263700191772678) != 0) || ((((_la - 70) & ~ 0x3fULL) == 0) &&
                ((1ULL << (_la - 70)) & 14955) != 0)) {
                setState(809);
                antlrcpp::downCast<SuffixExpressionContext *>(_localctx)->arg0 = logicalOrExpression();
                 args.push_back(antlrcpp::downCast<SuffixExpressionContext *>(_localctx)->arg0->tree); 
                setState(817);
                _errHandler->sync(this);
                _la = _input->LA(1);
                while (_la == GPULangParser::CO) {
                  setState(811);
                  match(GPULangParser::CO);
                  setState(812);
                  antlrcpp::downCast<SuffixExpressionContext *>(_localctx)->argn = logicalOrExpression();
                   args.push_back(antlrcpp::downCast<SuffixExpressionContext *>(_localctx)->argn->tree); 
                  setState(819);
                  _errHandler->sync(this);
                  _la = _input->LA(1);
                }
              }
              setState(822);
              match(GPULangParser::RP);

                          CallExpression* expr = new CallExpression(_localctx->tree, args);
                          expr->location = location;
                          antlrcpp::downCast<SuffixExpressionContext *>(_localctx)->tree =  expr;
                      
              break;
            }

            case GPULangParser::DOT: {
              setState(824);
              match(GPULangParser::DOT);
               location = SetupFile(); 
              setState(826);
              antlrcpp::downCast<SuffixExpressionContext *>(_localctx)->e2 = suffixExpression();

                          AccessExpression* expr = new AccessExpression(_localctx->tree, antlrcpp::downCast<SuffixExpressionContext *>(_localctx)->e2->tree, false);
                          expr->location = location;
                          antlrcpp::downCast<SuffixExpressionContext *>(_localctx)->tree =  expr;
                      
              break;
            }

            case GPULangParser::ARROW: {
              setState(829);
              match(GPULangParser::ARROW);
               location = SetupFile(); 
              setState(831);
              antlrcpp::downCast<SuffixExpressionContext *>(_localctx)->e2 = suffixExpression();

                          AccessExpression* expr = new AccessExpression(_localctx->tree, antlrcpp::downCast<SuffixExpressionContext *>(_localctx)->e2->tree, true);
                          expr->location = location;
                          antlrcpp::downCast<SuffixExpressionContext *>(_localctx)->tree =  expr;
                      
              break;
            }

            case GPULangParser::LL: {
              setState(834);
              match(GPULangParser::LL);
               location = SetupFile(); 
              setState(839);
              _errHandler->sync(this);

              _la = _input->LA(1);
              if ((((_la & ~ 0x3fULL) == 0) &&
                ((1ULL << _la) & 108263700191772678) != 0) || ((((_la - 70) & ~ 0x3fULL) == 0) &&
                ((1ULL << (_la - 70)) & 14955) != 0)) {
                setState(836);
                antlrcpp::downCast<SuffixExpressionContext *>(_localctx)->e3 = expression();
                 arrayIndexExpr = antlrcpp::downCast<SuffixExpressionContext *>(_localctx)->e3->tree; 
              }
              setState(841);
              match(GPULangParser::RR);

                          ArrayIndexExpression* expr = new ArrayIndexExpression(_localctx->tree, arrayIndexExpr);
                          expr->location = location;
                          antlrcpp::downCast<SuffixExpressionContext *>(_localctx)->tree =  expr;
                      
              break;
            }

          default:
            throw NoViableAltException(this);
          } 
        }
        setState(847);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 69, _ctx);
      }
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NamespaceExpressionContext ------------------------------------------------------------------

GPULangParser::NamespaceExpressionContext::NamespaceExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<GPULangParser::BinaryexpatomContext *> GPULangParser::NamespaceExpressionContext::binaryexpatom() {
  return getRuleContexts<GPULangParser::BinaryexpatomContext>();
}

GPULangParser::BinaryexpatomContext* GPULangParser::NamespaceExpressionContext::binaryexpatom(size_t i) {
  return getRuleContext<GPULangParser::BinaryexpatomContext>(i);
}


size_t GPULangParser::NamespaceExpressionContext::getRuleIndex() const {
  return GPULangParser::RuleNamespaceExpression;
}

void GPULangParser::NamespaceExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNamespaceExpression(this);
}

void GPULangParser::NamespaceExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNamespaceExpression(this);
}

GPULangParser::NamespaceExpressionContext* GPULangParser::namespaceExpression() {
  NamespaceExpressionContext *_localctx = _tracker.createInstance<NamespaceExpressionContext>(_ctx, getState());
  enterRule(_localctx, 90, GPULangParser::RuleNamespaceExpression);

          antlrcpp::downCast<NamespaceExpressionContext *>(_localctx)->tree =  nullptr;
      

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(850);
    antlrcpp::downCast<NamespaceExpressionContext *>(_localctx)->e1 = binaryexpatom();
    setState(851);
    match(GPULangParser::T__33);
    setState(852);
    antlrcpp::downCast<NamespaceExpressionContext *>(_localctx)->e2 = binaryexpatom();


        
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BinaryexpatomContext ------------------------------------------------------------------

GPULangParser::BinaryexpatomContext::BinaryexpatomContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* GPULangParser::BinaryexpatomContext::INTEGERLITERAL() {
  return getToken(GPULangParser::INTEGERLITERAL, 0);
}

tree::TerminalNode* GPULangParser::BinaryexpatomContext::UINTEGERLITERAL() {
  return getToken(GPULangParser::UINTEGERLITERAL, 0);
}

tree::TerminalNode* GPULangParser::BinaryexpatomContext::FLOATLITERAL() {
  return getToken(GPULangParser::FLOATLITERAL, 0);
}

tree::TerminalNode* GPULangParser::BinaryexpatomContext::DOUBLELITERAL() {
  return getToken(GPULangParser::DOUBLELITERAL, 0);
}

tree::TerminalNode* GPULangParser::BinaryexpatomContext::HEX() {
  return getToken(GPULangParser::HEX, 0);
}

GPULangParser::StringContext* GPULangParser::BinaryexpatomContext::string() {
  return getRuleContext<GPULangParser::StringContext>(0);
}

tree::TerminalNode* GPULangParser::BinaryexpatomContext::IDENTIFIER() {
  return getToken(GPULangParser::IDENTIFIER, 0);
}

GPULangParser::BooleanContext* GPULangParser::BinaryexpatomContext::boolean() {
  return getRuleContext<GPULangParser::BooleanContext>(0);
}

GPULangParser::InitializerExpressionContext* GPULangParser::BinaryexpatomContext::initializerExpression() {
  return getRuleContext<GPULangParser::InitializerExpressionContext>(0);
}

tree::TerminalNode* GPULangParser::BinaryexpatomContext::LP() {
  return getToken(GPULangParser::LP, 0);
}

GPULangParser::ExpressionContext* GPULangParser::BinaryexpatomContext::expression() {
  return getRuleContext<GPULangParser::ExpressionContext>(0);
}

tree::TerminalNode* GPULangParser::BinaryexpatomContext::RP() {
  return getToken(GPULangParser::RP, 0);
}


size_t GPULangParser::BinaryexpatomContext::getRuleIndex() const {
  return GPULangParser::RuleBinaryexpatom;
}

void GPULangParser::BinaryexpatomContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBinaryexpatom(this);
}

void GPULangParser::BinaryexpatomContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBinaryexpatom(this);
}

GPULangParser::BinaryexpatomContext* GPULangParser::binaryexpatom() {
  BinaryexpatomContext *_localctx = _tracker.createInstance<BinaryexpatomContext>(_ctx, getState());
  enterRule(_localctx, 92, GPULangParser::RuleBinaryexpatom);

          antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->tree =  nullptr;
      

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(881);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case GPULangParser::INTEGERLITERAL: {
        enterOuterAlt(_localctx, 1);
        setState(855);
        antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->integerliteralToken = match(GPULangParser::INTEGERLITERAL);
         antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->tree =  new IntExpression(atoi((antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->integerliteralToken != nullptr ? antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->integerliteralToken->getText() : "").c_str())); _localctx->tree->location = SetupFile(); 
        break;
      }

      case GPULangParser::UINTEGERLITERAL: {
        enterOuterAlt(_localctx, 2);
        setState(857);
        antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->uintegerliteralToken = match(GPULangParser::UINTEGERLITERAL);
         antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->tree =  new UIntExpression(strtoul((antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->uintegerliteralToken != nullptr ? antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->uintegerliteralToken->getText() : "").c_str(), nullptr, 10)); _localctx->tree->location = SetupFile(); 
        break;
      }

      case GPULangParser::FLOATLITERAL: {
        enterOuterAlt(_localctx, 3);
        setState(859);
        antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->floatliteralToken = match(GPULangParser::FLOATLITERAL);
         antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->tree =  new FloatExpression(atof((antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->floatliteralToken != nullptr ? antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->floatliteralToken->getText() : "").c_str())); _localctx->tree->location = SetupFile(); 
        break;
      }

      case GPULangParser::DOUBLELITERAL: {
        enterOuterAlt(_localctx, 4);
        setState(861);
        antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->doubleliteralToken = match(GPULangParser::DOUBLELITERAL);
         antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->tree =  new FloatExpression(atof((antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->doubleliteralToken != nullptr ? antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->doubleliteralToken->getText() : "").c_str())); _localctx->tree->location = SetupFile(); 
        break;
      }

      case GPULangParser::HEX: {
        enterOuterAlt(_localctx, 5);
        setState(863);
        antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->hexToken = match(GPULangParser::HEX);
         antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->tree =  new UIntExpression(strtoul((antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->hexToken != nullptr ? antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->hexToken->getText() : "").c_str(), nullptr, 16)); _localctx->tree->location = SetupFile(); 
        break;
      }

      case GPULangParser::QO:
      case GPULangParser::Q: {
        enterOuterAlt(_localctx, 6);
        setState(865);
        antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->stringContext = string();
         antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->tree =  new StringExpression(antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->stringContext->val); _localctx->tree->location = SetupFile(); 
        break;
      }

      case GPULangParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 7);
        setState(868);
        antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->identifierToken = match(GPULangParser::IDENTIFIER);
         antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->tree =  new SymbolExpression((antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->identifierToken != nullptr ? antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->identifierToken->getText() : "")); _localctx->tree->location = SetupFile(); 
        break;
      }

      case GPULangParser::T__0:
      case GPULangParser::T__1: {
        enterOuterAlt(_localctx, 8);
        setState(870);
        antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->booleanContext = boolean();
         antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->tree =  new BoolExpression(antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->booleanContext->val); _localctx->tree->location = SetupFile(); 
        break;
      }

      case GPULangParser::LB: {
        enterOuterAlt(_localctx, 9);
        setState(873);
        antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->initializerExpressionContext = initializerExpression();
         antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->tree =  antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->initializerExpressionContext->tree; 
        break;
      }

      case GPULangParser::LP: {
        enterOuterAlt(_localctx, 10);
        setState(876);
        match(GPULangParser::LP);
        setState(877);
        antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->expressionContext = expression();
        setState(878);
        match(GPULangParser::RP);
         antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->tree =  antlrcpp::downCast<BinaryexpatomContext *>(_localctx)->expressionContext->tree; 
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InitializerExpressionContext ------------------------------------------------------------------

GPULangParser::InitializerExpressionContext::InitializerExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* GPULangParser::InitializerExpressionContext::LB() {
  return getToken(GPULangParser::LB, 0);
}

tree::TerminalNode* GPULangParser::InitializerExpressionContext::RB() {
  return getToken(GPULangParser::RB, 0);
}

std::vector<GPULangParser::AssignmentExpressionContext *> GPULangParser::InitializerExpressionContext::assignmentExpression() {
  return getRuleContexts<GPULangParser::AssignmentExpressionContext>();
}

GPULangParser::AssignmentExpressionContext* GPULangParser::InitializerExpressionContext::assignmentExpression(size_t i) {
  return getRuleContext<GPULangParser::AssignmentExpressionContext>(i);
}

std::vector<tree::TerminalNode *> GPULangParser::InitializerExpressionContext::CO() {
  return getTokens(GPULangParser::CO);
}

tree::TerminalNode* GPULangParser::InitializerExpressionContext::CO(size_t i) {
  return getToken(GPULangParser::CO, i);
}


size_t GPULangParser::InitializerExpressionContext::getRuleIndex() const {
  return GPULangParser::RuleInitializerExpression;
}

void GPULangParser::InitializerExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterInitializerExpression(this);
}

void GPULangParser::InitializerExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitInitializerExpression(this);
}

GPULangParser::InitializerExpressionContext* GPULangParser::initializerExpression() {
  InitializerExpressionContext *_localctx = _tracker.createInstance<InitializerExpressionContext>(_ctx, getState());
  enterRule(_localctx, 94, GPULangParser::RuleInitializerExpression);

          antlrcpp::downCast<InitializerExpressionContext *>(_localctx)->tree =  nullptr;
          std::vector<Expression*> exprs;
          Symbol::Location location;
      
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(883);
    match(GPULangParser::LB);
     location = SetupFile(); 
    setState(896);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 108263700191772678) != 0) || ((((_la - 70) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 70)) & 14955) != 0)) {
      setState(885);
      antlrcpp::downCast<InitializerExpressionContext *>(_localctx)->arg0 = assignmentExpression();
       exprs.push_back(antlrcpp::downCast<InitializerExpressionContext *>(_localctx)->arg0->tree); 
      setState(893);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == GPULangParser::CO) {
        setState(887);
        match(GPULangParser::CO);
        setState(888);
        antlrcpp::downCast<InitializerExpressionContext *>(_localctx)->argN = assignmentExpression();
         exprs.push_back(antlrcpp::downCast<InitializerExpressionContext *>(_localctx)->argN->tree); 
        setState(895);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(898);
    match(GPULangParser::RB);

            antlrcpp::downCast<InitializerExpressionContext *>(_localctx)->tree =  new InitializerExpression(exprs);
            _localctx->tree->location = location;
        
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

void GPULangParser::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  gpulangParserInitialize();
#else
  ::antlr4::internal::call_once(gpulangParserOnceFlag, gpulangParserInitialize);
#endif
}
