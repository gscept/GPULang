
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
    T__32 = 33, T__33 = 34, SC = 35, CO = 36, COL = 37, LP = 38, RP = 39, 
    LB = 40, RB = 41, LL = 42, RR = 43, DOT = 44, NOT = 45, EQ = 46, QO = 47, 
    QU = 48, AND = 49, ANDSET = 50, OR = 51, ORSET = 52, XOR = 53, XORSET = 54, 
    CONNJUGATE = 55, Q = 56, NU = 57, FORWARDSLASH = 58, LESS = 59, LESSEQ = 60, 
    GREATER = 61, GREATEREQ = 62, LOGICEQ = 63, NOTEQ = 64, LOGICAND = 65, 
    LOGICOR = 66, MOD = 67, UNDERSC = 68, SOBAKA = 69, ADD_OP = 70, SUB_OP = 71, 
    DIV_OP = 72, MUL_OP = 73, ARROW = 74, INTEGERLITERAL = 75, UINTEGERLITERAL = 76, 
    COMMENT = 77, ML_COMMENT = 78, FLOATLITERAL = 79, EXPONENT = 80, DOUBLELITERAL = 81, 
    HEX = 82, IDENTIFIER = 83, WS = 84
  };

  explicit GPULangLexer(antlr4::CharStream *input);

  ~GPULangLexer() override;


  #include <iostream>

  misc::Interval interval;
  std::string currentFile;
  int currentLine = 0;


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

