
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
    T__7 = 8, T__8 = 9, SC = 10, CO = 11, COL = 12, LP = 13, RP = 14, LB = 15, 
    RB = 16, LL = 17, RR = 18, DOT = 19, NOT = 20, EQ = 21, QO = 22, QU = 23, 
    AND = 24, ANDSET = 25, OR = 26, ORSET = 27, XOR = 28, XORSET = 29, CONNJUGATE = 30, 
    Q = 31, NU = 32, FORWARDSLASH = 33, LESS = 34, LESSEQ = 35, GREATER = 36, 
    GREATEREQ = 37, LOGICEQ = 38, NOTEQ = 39, LOGICAND = 40, LOGICOR = 41, 
    MOD = 42, UNDERSC = 43, SOBAKA = 44, ADD_OP = 45, SUB_OP = 46, DIV_OP = 47, 
    MUL_OP = 48, INTEGERLITERAL = 49, COMMENT = 50, ML_COMMENT = 51, FLOATLITERAL = 52, 
    EXPONENT = 53, DOUBLELITERAL = 54, HEX = 55, IDENTIFIER = 56, WS = 57
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

