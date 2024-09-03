
    #include <string>
    #include <vector>
    #include "anyfxtoken.h"


// Generated from antlr4/grammar/AnyFX.g4 by ANTLR 4.7.1

#pragma once


#include "antlr4-runtime.h"




class  AnyFXLexer : public antlr4::Lexer {
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

  AnyFXLexer(antlr4::CharStream *input);
  ~AnyFXLexer();


  #include <iostream>

  misc::Interval interval;
  std::string currentFile;
  int currentLine = 0;

  virtual std::string getGrammarFileName() const override;
  virtual const std::vector<std::string>& getRuleNames() const override;

  virtual const std::vector<std::string>& getChannelNames() const override;
  virtual const std::vector<std::string>& getModeNames() const override;
  virtual const std::vector<std::string>& getTokenNames() const override; // deprecated, use vocabulary instead
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;

  virtual const std::vector<uint16_t> getSerializedATN() const override;
  virtual const antlr4::atn::ATN& getATN() const override;

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;
  static std::vector<std::string> _channelNames;
  static std::vector<std::string> _modeNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

