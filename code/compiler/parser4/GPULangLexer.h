
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
    T__38 = 39, SC = 40, CO = 41, COL = 42, LP = 43, RP = 44, LB = 45, RB = 46, 
    LL = 47, RR = 48, DOT = 49, NOT = 50, EQ = 51, QO = 52, QU = 53, AND = 54, 
    ANDSET = 55, OR = 56, ORSET = 57, XOR = 58, XORSET = 59, CONJUGATE = 60, 
    Q = 61, NU = 62, FORWARDSLASH = 63, LESS = 64, LESSEQ = 65, GREATER = 66, 
    GREATEREQ = 67, LOGICEQ = 68, NOTEQ = 69, LOGICAND = 70, LOGICOR = 71, 
    MOD = 72, UNDERSC = 73, SOBAKA = 74, ADD_OP = 75, SUB_OP = 76, DIV_OP = 77, 
    MUL_OP = 78, ARROW = 79, INTEGERLITERAL = 80, UINTEGERLITERAL = 81, 
    COMMENT = 82, ML_COMMENT = 83, FLOATLITERAL = 84, DOUBLELITERAL = 85, 
    HEX = 86, IDENTIFIER = 87, WS = 88
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

