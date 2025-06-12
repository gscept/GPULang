
    #include <string>
    #include <vector>
    #include "gpulangtoken.h"


// Generated from antlr4/grammar/GPULang.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"




class  GPULangLexer : public antlr4::Lexer {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, T__10 = 11, T__11 = 12, T__12 = 13, T__13 = 14, 
    T__14 = 15, T__15 = 16, T__16 = 17, T__17 = 18, T__18 = 19, T__19 = 20, 
    T__20 = 21, T__21 = 22, T__22 = 23, T__23 = 24, T__24 = 25, T__25 = 26, 
    T__26 = 27, T__27 = 28, T__28 = 29, T__29 = 30, T__30 = 31, T__31 = 32, 
    T__32 = 33, T__33 = 34, T__34 = 35, T__35 = 36, T__36 = 37, T__37 = 38, 
    SC = 39, CO = 40, COL = 41, LP = 42, RP = 43, LB = 44, RB = 45, LL = 46, 
    RR = 47, DOT = 48, NOT = 49, EQ = 50, QO = 51, QU = 52, AND = 53, ANDSET = 54, 
    OR = 55, ORSET = 56, XOR = 57, XORSET = 58, CONJUGATE = 59, Q = 60, 
    NU = 61, FORWARDSLASH = 62, LESS = 63, LESSEQ = 64, GREATER = 65, GREATEREQ = 66, 
    LOGICEQ = 67, NOTEQ = 68, LOGICAND = 69, LOGICOR = 70, MOD = 71, UNDERSC = 72, 
    SOBAKA = 73, ADD_OP = 74, SUB_OP = 75, DIV_OP = 76, MUL_OP = 77, ARROW = 78, 
    INTEGERLITERAL = 79, UINTEGERLITERAL = 80, COMMENT = 81, ML_COMMENT = 82, 
    FLOATLITERAL = 83, DOUBLELITERAL = 84, HEX = 85, IDENTIFIER = 86, WS = 87
  };

  explicit GPULangLexer(antlr4::CharStream *input);

  ~GPULangLexer() override;


  #include <iostream>


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

};

