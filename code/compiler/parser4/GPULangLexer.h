
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
    T__32 = 33, T__33 = 34, T__34 = 35, T__35 = 36, T__36 = 37, SC = 38, 
    CO = 39, COL = 40, LP = 41, RP = 42, LB = 43, RB = 44, LL = 45, RR = 46, 
    DOT = 47, NOT = 48, EQ = 49, QO = 50, QU = 51, AND = 52, ANDSET = 53, 
    OR = 54, ORSET = 55, XOR = 56, XORSET = 57, CONJUGATE = 58, Q = 59, 
    NU = 60, FORWARDSLASH = 61, LESS = 62, LESSEQ = 63, GREATER = 64, GREATEREQ = 65, 
    LOGICEQ = 66, NOTEQ = 67, LOGICAND = 68, LOGICOR = 69, MOD = 70, UNDERSC = 71, 
    SOBAKA = 72, ADD_OP = 73, SUB_OP = 74, DIV_OP = 75, MUL_OP = 76, ARROW = 77, 
    INTEGERLITERAL = 78, UINTEGERLITERAL = 79, COMMENT = 80, ML_COMMENT = 81, 
    FLOATLITERAL = 82, DOUBLELITERAL = 83, HEX = 84, IDENTIFIER = 85, WS = 86
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

