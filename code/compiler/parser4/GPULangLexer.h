
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
    T__32 = 33, T__33 = 34, T__34 = 35, SC = 36, CO = 37, COL = 38, LP = 39, 
    RP = 40, LB = 41, RB = 42, LL = 43, RR = 44, DOT = 45, NOT = 46, EQ = 47, 
    QO = 48, QU = 49, AND = 50, ANDSET = 51, OR = 52, ORSET = 53, XOR = 54, 
    XORSET = 55, CONNJUGATE = 56, Q = 57, NU = 58, FORWARDSLASH = 59, LESS = 60, 
    LESSEQ = 61, GREATER = 62, GREATEREQ = 63, LOGICEQ = 64, NOTEQ = 65, 
    LOGICAND = 66, LOGICOR = 67, MOD = 68, UNDERSC = 69, SOBAKA = 70, ADD_OP = 71, 
    SUB_OP = 72, DIV_OP = 73, MUL_OP = 74, ARROW = 75, INTEGERLITERAL = 76, 
    UINTEGERLITERAL = 77, COMMENT = 78, ML_COMMENT = 79, FLOATLITERAL = 80, 
    EXPONENT = 81, DOUBLELITERAL = 82, HEX = 83, IDENTIFIER = 84, WS = 85
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

