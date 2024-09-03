

// include std container
#include <vector>
#include <typeinfo>
#include <iostream>
#include <string>
#include <stack>
#include <tuple>

#include "anyfxtoken.h"
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

using namespace AnyFX;



// Generated from h:\anyfxv3\fips-anyfx\code\compiler\ext\antlr4\grammar\AnyFX.g4 by ANTLR 4.8



#include "AnyFXParser.h"


using namespace antlrcpp;
using namespace antlr4;

AnyFXParser::AnyFXParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

AnyFXParser::~AnyFXParser() {
  delete _interpreter;
}

std::string AnyFXParser::getGrammarFileName() const {
  return "AnyFX.g4";
}

const std::vector<std::string>& AnyFXParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& AnyFXParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- StringContext ------------------------------------------------------------------

AnyFXParser::StringContext::StringContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> AnyFXParser::StringContext::QO() {
  return getTokens(AnyFXParser::QO);
}

tree::TerminalNode* AnyFXParser::StringContext::QO(size_t i) {
  return getToken(AnyFXParser::QO, i);
}

std::vector<tree::TerminalNode *> AnyFXParser::StringContext::Q() {
  return getTokens(AnyFXParser::Q);
}

tree::TerminalNode* AnyFXParser::StringContext::Q(size_t i) {
  return getToken(AnyFXParser::Q, i);
}


size_t AnyFXParser::StringContext::getRuleIndex() const {
  return AnyFXParser::RuleString;
}


AnyFXParser::StringContext* AnyFXParser::string() {
  StringContext *_localctx = _tracker.createInstance<StringContext>(_ctx, getState());
  enterRule(_localctx, 0, AnyFXParser::RuleString);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(114);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case AnyFXParser::QO: {
        enterOuterAlt(_localctx, 1);
        setState(96);
        match(AnyFXParser::QO);
        setState(101);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << AnyFXParser::T__0)
          | (1ULL << AnyFXParser::T__1)
          | (1ULL << AnyFXParser::T__2)
          | (1ULL << AnyFXParser::T__3)
          | (1ULL << AnyFXParser::T__4)
          | (1ULL << AnyFXParser::T__5)
          | (1ULL << AnyFXParser::T__6)
          | (1ULL << AnyFXParser::T__7)
          | (1ULL << AnyFXParser::T__8)
          | (1ULL << AnyFXParser::T__9)
          | (1ULL << AnyFXParser::T__10)
          | (1ULL << AnyFXParser::T__11)
          | (1ULL << AnyFXParser::T__12)
          | (1ULL << AnyFXParser::T__13)
          | (1ULL << AnyFXParser::T__14)
          | (1ULL << AnyFXParser::T__15)
          | (1ULL << AnyFXParser::T__16)
          | (1ULL << AnyFXParser::T__17)
          | (1ULL << AnyFXParser::T__18)
          | (1ULL << AnyFXParser::T__19)
          | (1ULL << AnyFXParser::T__20)
          | (1ULL << AnyFXParser::T__21)
          | (1ULL << AnyFXParser::T__22)
          | (1ULL << AnyFXParser::T__23)
          | (1ULL << AnyFXParser::T__24)
          | (1ULL << AnyFXParser::T__25)
          | (1ULL << AnyFXParser::T__26)
          | (1ULL << AnyFXParser::T__27)
          | (1ULL << AnyFXParser::T__28)
          | (1ULL << AnyFXParser::T__29)
          | (1ULL << AnyFXParser::T__30)
          | (1ULL << AnyFXParser::T__31)
          | (1ULL << AnyFXParser::T__32)
          | (1ULL << AnyFXParser::T__33)
          | (1ULL << AnyFXParser::SC)
          | (1ULL << AnyFXParser::CO)
          | (1ULL << AnyFXParser::COL)
          | (1ULL << AnyFXParser::LP)
          | (1ULL << AnyFXParser::RP)
          | (1ULL << AnyFXParser::LB)
          | (1ULL << AnyFXParser::RB)
          | (1ULL << AnyFXParser::LL)
          | (1ULL << AnyFXParser::RR)
          | (1ULL << AnyFXParser::DOT)
          | (1ULL << AnyFXParser::NOT)
          | (1ULL << AnyFXParser::EQ)
          | (1ULL << AnyFXParser::QU)
          | (1ULL << AnyFXParser::AND)
          | (1ULL << AnyFXParser::ANDSET)
          | (1ULL << AnyFXParser::OR)
          | (1ULL << AnyFXParser::ORSET)
          | (1ULL << AnyFXParser::XOR)
          | (1ULL << AnyFXParser::XORSET)
          | (1ULL << AnyFXParser::CONNJUGATE)
          | (1ULL << AnyFXParser::Q)
          | (1ULL << AnyFXParser::NU)
          | (1ULL << AnyFXParser::FORWARDSLASH)
          | (1ULL << AnyFXParser::LESS)
          | (1ULL << AnyFXParser::LESSEQ)
          | (1ULL << AnyFXParser::GREATER)
          | (1ULL << AnyFXParser::GREATEREQ)
          | (1ULL << AnyFXParser::LOGICEQ))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 64)) & ((1ULL << (AnyFXParser::NOTEQ - 64))
          | (1ULL << (AnyFXParser::LOGICAND - 64))
          | (1ULL << (AnyFXParser::LOGICOR - 64))
          | (1ULL << (AnyFXParser::MOD - 64))
          | (1ULL << (AnyFXParser::UNDERSC - 64))
          | (1ULL << (AnyFXParser::SOBAKA - 64))
          | (1ULL << (AnyFXParser::ADD_OP - 64))
          | (1ULL << (AnyFXParser::SUB_OP - 64))
          | (1ULL << (AnyFXParser::DIV_OP - 64))
          | (1ULL << (AnyFXParser::MUL_OP - 64))
          | (1ULL << (AnyFXParser::ARROW - 64))
          | (1ULL << (AnyFXParser::INTEGERLITERAL - 64))
          | (1ULL << (AnyFXParser::UINTEGERLITERAL - 64))
          | (1ULL << (AnyFXParser::COMMENT - 64))
          | (1ULL << (AnyFXParser::ML_COMMENT - 64))
          | (1ULL << (AnyFXParser::FLOATLITERAL - 64))
          | (1ULL << (AnyFXParser::EXPONENT - 64))
          | (1ULL << (AnyFXParser::DOUBLELITERAL - 64))
          | (1ULL << (AnyFXParser::HEX - 64))
          | (1ULL << (AnyFXParser::IDENTIFIER - 64))
          | (1ULL << (AnyFXParser::WS - 64)))) != 0)) {
          setState(97);
          dynamic_cast<StringContext *>(_localctx)->data = _input->LT(1);
          _la = _input->LA(1);
          if (_la == 0 || _la == Token::EOF || (_la == AnyFXParser::QO)) {
            dynamic_cast<StringContext *>(_localctx)->data = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
           _localctx->val.append((dynamic_cast<StringContext *>(_localctx)->data != nullptr ? dynamic_cast<StringContext *>(_localctx)->data->getText() : "")); 
          setState(103);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(104);
        match(AnyFXParser::QO);
        break;
      }

      case AnyFXParser::Q: {
        enterOuterAlt(_localctx, 2);
        setState(105);
        match(AnyFXParser::Q);
        setState(110);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << AnyFXParser::T__0)
          | (1ULL << AnyFXParser::T__1)
          | (1ULL << AnyFXParser::T__2)
          | (1ULL << AnyFXParser::T__3)
          | (1ULL << AnyFXParser::T__4)
          | (1ULL << AnyFXParser::T__5)
          | (1ULL << AnyFXParser::T__6)
          | (1ULL << AnyFXParser::T__7)
          | (1ULL << AnyFXParser::T__8)
          | (1ULL << AnyFXParser::T__9)
          | (1ULL << AnyFXParser::T__10)
          | (1ULL << AnyFXParser::T__11)
          | (1ULL << AnyFXParser::T__12)
          | (1ULL << AnyFXParser::T__13)
          | (1ULL << AnyFXParser::T__14)
          | (1ULL << AnyFXParser::T__15)
          | (1ULL << AnyFXParser::T__16)
          | (1ULL << AnyFXParser::T__17)
          | (1ULL << AnyFXParser::T__18)
          | (1ULL << AnyFXParser::T__19)
          | (1ULL << AnyFXParser::T__20)
          | (1ULL << AnyFXParser::T__21)
          | (1ULL << AnyFXParser::T__22)
          | (1ULL << AnyFXParser::T__23)
          | (1ULL << AnyFXParser::T__24)
          | (1ULL << AnyFXParser::T__25)
          | (1ULL << AnyFXParser::T__26)
          | (1ULL << AnyFXParser::T__27)
          | (1ULL << AnyFXParser::T__28)
          | (1ULL << AnyFXParser::T__29)
          | (1ULL << AnyFXParser::T__30)
          | (1ULL << AnyFXParser::T__31)
          | (1ULL << AnyFXParser::T__32)
          | (1ULL << AnyFXParser::T__33)
          | (1ULL << AnyFXParser::SC)
          | (1ULL << AnyFXParser::CO)
          | (1ULL << AnyFXParser::COL)
          | (1ULL << AnyFXParser::LP)
          | (1ULL << AnyFXParser::RP)
          | (1ULL << AnyFXParser::LB)
          | (1ULL << AnyFXParser::RB)
          | (1ULL << AnyFXParser::LL)
          | (1ULL << AnyFXParser::RR)
          | (1ULL << AnyFXParser::DOT)
          | (1ULL << AnyFXParser::NOT)
          | (1ULL << AnyFXParser::EQ)
          | (1ULL << AnyFXParser::QO)
          | (1ULL << AnyFXParser::QU)
          | (1ULL << AnyFXParser::AND)
          | (1ULL << AnyFXParser::ANDSET)
          | (1ULL << AnyFXParser::OR)
          | (1ULL << AnyFXParser::ORSET)
          | (1ULL << AnyFXParser::XOR)
          | (1ULL << AnyFXParser::XORSET)
          | (1ULL << AnyFXParser::CONNJUGATE)
          | (1ULL << AnyFXParser::NU)
          | (1ULL << AnyFXParser::FORWARDSLASH)
          | (1ULL << AnyFXParser::LESS)
          | (1ULL << AnyFXParser::LESSEQ)
          | (1ULL << AnyFXParser::GREATER)
          | (1ULL << AnyFXParser::GREATEREQ)
          | (1ULL << AnyFXParser::LOGICEQ))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 64)) & ((1ULL << (AnyFXParser::NOTEQ - 64))
          | (1ULL << (AnyFXParser::LOGICAND - 64))
          | (1ULL << (AnyFXParser::LOGICOR - 64))
          | (1ULL << (AnyFXParser::MOD - 64))
          | (1ULL << (AnyFXParser::UNDERSC - 64))
          | (1ULL << (AnyFXParser::SOBAKA - 64))
          | (1ULL << (AnyFXParser::ADD_OP - 64))
          | (1ULL << (AnyFXParser::SUB_OP - 64))
          | (1ULL << (AnyFXParser::DIV_OP - 64))
          | (1ULL << (AnyFXParser::MUL_OP - 64))
          | (1ULL << (AnyFXParser::ARROW - 64))
          | (1ULL << (AnyFXParser::INTEGERLITERAL - 64))
          | (1ULL << (AnyFXParser::UINTEGERLITERAL - 64))
          | (1ULL << (AnyFXParser::COMMENT - 64))
          | (1ULL << (AnyFXParser::ML_COMMENT - 64))
          | (1ULL << (AnyFXParser::FLOATLITERAL - 64))
          | (1ULL << (AnyFXParser::EXPONENT - 64))
          | (1ULL << (AnyFXParser::DOUBLELITERAL - 64))
          | (1ULL << (AnyFXParser::HEX - 64))
          | (1ULL << (AnyFXParser::IDENTIFIER - 64))
          | (1ULL << (AnyFXParser::WS - 64)))) != 0)) {
          setState(106);
          dynamic_cast<StringContext *>(_localctx)->data = _input->LT(1);
          _la = _input->LA(1);
          if (_la == 0 || _la == Token::EOF || (_la == AnyFXParser::Q)) {
            dynamic_cast<StringContext *>(_localctx)->data = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
           _localctx->val.append((dynamic_cast<StringContext *>(_localctx)->data != nullptr ? dynamic_cast<StringContext *>(_localctx)->data->getText() : "")); 
          setState(112);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(113);
        match(AnyFXParser::Q);
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

AnyFXParser::BooleanContext::BooleanContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t AnyFXParser::BooleanContext::getRuleIndex() const {
  return AnyFXParser::RuleBoolean;
}


AnyFXParser::BooleanContext* AnyFXParser::boolean() {
  BooleanContext *_localctx = _tracker.createInstance<BooleanContext>(_ctx, getState());
  enterRule(_localctx, 2, AnyFXParser::RuleBoolean);

          dynamic_cast<BooleanContext *>(_localctx)->val =  false;
      

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(120);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case AnyFXParser::T__0: {
        enterOuterAlt(_localctx, 1);
        setState(116);
        match(AnyFXParser::T__0);
         dynamic_cast<BooleanContext *>(_localctx)->val =  true; 
        break;
      }

      case AnyFXParser::T__1: {
        enterOuterAlt(_localctx, 2);
        setState(118);
        match(AnyFXParser::T__1);
         dynamic_cast<BooleanContext *>(_localctx)->val =  false; 
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

AnyFXParser::PreprocessContext::PreprocessContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* AnyFXParser::PreprocessContext::EOF() {
  return getToken(AnyFXParser::EOF, 0);
}

std::vector<tree::TerminalNode *> AnyFXParser::PreprocessContext::INTEGERLITERAL() {
  return getTokens(AnyFXParser::INTEGERLITERAL);
}

tree::TerminalNode* AnyFXParser::PreprocessContext::INTEGERLITERAL(size_t i) {
  return getToken(AnyFXParser::INTEGERLITERAL, i);
}

std::vector<AnyFXParser::StringContext *> AnyFXParser::PreprocessContext::string() {
  return getRuleContexts<AnyFXParser::StringContext>();
}

AnyFXParser::StringContext* AnyFXParser::PreprocessContext::string(size_t i) {
  return getRuleContext<AnyFXParser::StringContext>(i);
}


size_t AnyFXParser::PreprocessContext::getRuleIndex() const {
  return AnyFXParser::RulePreprocess;
}


AnyFXParser::PreprocessContext* AnyFXParser::preprocess() {
  PreprocessContext *_localctx = _tracker.createInstance<PreprocessContext>(_ctx, getState());
  enterRule(_localctx, 4, AnyFXParser::RulePreprocess);

          Token* start = nullptr;
      

  auto onExit = finally([=] {
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
          match(AnyFXParser::T__2);
          setState(124);
          dynamic_cast<PreprocessContext *>(_localctx)->line = match(AnyFXParser::INTEGERLITERAL);
          setState(125);
          dynamic_cast<PreprocessContext *>(_localctx)->path = string();
           lines.push_back(std::make_tuple(atoi((dynamic_cast<PreprocessContext *>(_localctx)->line != nullptr ? dynamic_cast<PreprocessContext *>(_localctx)->line->getText() : "").c_str()), _input->LT(-1)->getLine(), start->getStartIndex(), _input->LT(1)->getStartIndex(), (dynamic_cast<PreprocessContext *>(_localctx)->path != nullptr ? _input->getText(dynamic_cast<PreprocessContext *>(_localctx)->path->start, dynamic_cast<PreprocessContext *>(_localctx)->path->stop) : nullptr))); 
          break;
        }

        case 2: {
          setState(128);
          matchWildcard();
          break;
        }

        } 
      }
      setState(133);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx);
    }
    setState(134);
    match(AnyFXParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EntryContext ------------------------------------------------------------------

AnyFXParser::EntryContext::EntryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

AnyFXParser::EffectContext* AnyFXParser::EntryContext::effect() {
  return getRuleContext<AnyFXParser::EffectContext>(0);
}

tree::TerminalNode* AnyFXParser::EntryContext::EOF() {
  return getToken(AnyFXParser::EOF, 0);
}


size_t AnyFXParser::EntryContext::getRuleIndex() const {
  return AnyFXParser::RuleEntry;
}


AnyFXParser::EntryContext* AnyFXParser::entry() {
  EntryContext *_localctx = _tracker.createInstance<EntryContext>(_ctx, getState());
  enterRule(_localctx, 6, AnyFXParser::RuleEntry);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(136);
    dynamic_cast<EntryContext *>(_localctx)->effectContext = effect();

            dynamic_cast<EntryContext *>(_localctx)->returnEffect =  dynamic_cast<EntryContext *>(_localctx)->effectContext->eff;
        
    setState(138);
    match(AnyFXParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EffectContext ------------------------------------------------------------------

AnyFXParser::EffectContext::EffectContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<AnyFXParser::AliasContext *> AnyFXParser::EffectContext::alias() {
  return getRuleContexts<AnyFXParser::AliasContext>();
}

AnyFXParser::AliasContext* AnyFXParser::EffectContext::alias(size_t i) {
  return getRuleContext<AnyFXParser::AliasContext>(i);
}

std::vector<tree::TerminalNode *> AnyFXParser::EffectContext::SC() {
  return getTokens(AnyFXParser::SC);
}

tree::TerminalNode* AnyFXParser::EffectContext::SC(size_t i) {
  return getToken(AnyFXParser::SC, i);
}

std::vector<AnyFXParser::FunctionDeclarationContext *> AnyFXParser::EffectContext::functionDeclaration() {
  return getRuleContexts<AnyFXParser::FunctionDeclarationContext>();
}

AnyFXParser::FunctionDeclarationContext* AnyFXParser::EffectContext::functionDeclaration(size_t i) {
  return getRuleContext<AnyFXParser::FunctionDeclarationContext>(i);
}

std::vector<AnyFXParser::FunctionContext *> AnyFXParser::EffectContext::function() {
  return getRuleContexts<AnyFXParser::FunctionContext>();
}

AnyFXParser::FunctionContext* AnyFXParser::EffectContext::function(size_t i) {
  return getRuleContext<AnyFXParser::FunctionContext>(i);
}

std::vector<AnyFXParser::VariablesContext *> AnyFXParser::EffectContext::variables() {
  return getRuleContexts<AnyFXParser::VariablesContext>();
}

AnyFXParser::VariablesContext* AnyFXParser::EffectContext::variables(size_t i) {
  return getRuleContext<AnyFXParser::VariablesContext>(i);
}

std::vector<AnyFXParser::StructureContext *> AnyFXParser::EffectContext::structure() {
  return getRuleContexts<AnyFXParser::StructureContext>();
}

AnyFXParser::StructureContext* AnyFXParser::EffectContext::structure(size_t i) {
  return getRuleContext<AnyFXParser::StructureContext>(i);
}

std::vector<AnyFXParser::EnumerationContext *> AnyFXParser::EffectContext::enumeration() {
  return getRuleContexts<AnyFXParser::EnumerationContext>();
}

AnyFXParser::EnumerationContext* AnyFXParser::EffectContext::enumeration(size_t i) {
  return getRuleContext<AnyFXParser::EnumerationContext>(i);
}

std::vector<AnyFXParser::StateContext *> AnyFXParser::EffectContext::state() {
  return getRuleContexts<AnyFXParser::StateContext>();
}

AnyFXParser::StateContext* AnyFXParser::EffectContext::state(size_t i) {
  return getRuleContext<AnyFXParser::StateContext>(i);
}

std::vector<AnyFXParser::ProgramContext *> AnyFXParser::EffectContext::program() {
  return getRuleContexts<AnyFXParser::ProgramContext>();
}

AnyFXParser::ProgramContext* AnyFXParser::EffectContext::program(size_t i) {
  return getRuleContext<AnyFXParser::ProgramContext>(i);
}


size_t AnyFXParser::EffectContext::getRuleIndex() const {
  return AnyFXParser::RuleEffect;
}


AnyFXParser::EffectContext* AnyFXParser::effect() {
  EffectContext *_localctx = _tracker.createInstance<EffectContext>(_ctx, getState());
  enterRule(_localctx, 8, AnyFXParser::RuleEffect);

          dynamic_cast<EffectContext *>(_localctx)->eff =  new Effect();
      

  auto onExit = finally([=] {
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
          dynamic_cast<EffectContext *>(_localctx)->aliasContext = alias();
          setState(141);
          match(AnyFXParser::SC);
           _localctx->eff->symbols.push_back(dynamic_cast<EffectContext *>(_localctx)->aliasContext->sym); 
          break;
        }

        case 2: {
          setState(144);
          dynamic_cast<EffectContext *>(_localctx)->functionDeclarationContext = functionDeclaration();
          setState(145);
          match(AnyFXParser::SC);
           _localctx->eff->symbols.push_back(dynamic_cast<EffectContext *>(_localctx)->functionDeclarationContext->sym); 
          break;
        }

        case 3: {
          setState(148);
          dynamic_cast<EffectContext *>(_localctx)->functionContext = function();
           _localctx->eff->symbols.push_back(dynamic_cast<EffectContext *>(_localctx)->functionContext->sym); 
          break;
        }

        case 4: {
          setState(151);
          dynamic_cast<EffectContext *>(_localctx)->variablesContext = variables();
          setState(152);
          match(AnyFXParser::SC);
           for (Variable* var : dynamic_cast<EffectContext *>(_localctx)->variablesContext->list) { _localctx->eff->symbols.push_back(var); } 
          break;
        }

        case 5: {
          setState(155);
          dynamic_cast<EffectContext *>(_localctx)->structureContext = structure();
          setState(156);
          match(AnyFXParser::SC);
           _localctx->eff->symbols.push_back(dynamic_cast<EffectContext *>(_localctx)->structureContext->sym); 
          break;
        }

        case 6: {
          setState(159);
          dynamic_cast<EffectContext *>(_localctx)->enumerationContext = enumeration();
          setState(160);
          match(AnyFXParser::SC);
           _localctx->eff->symbols.push_back(dynamic_cast<EffectContext *>(_localctx)->enumerationContext->sym); 
          break;
        }

        case 7: {
          setState(163);
          dynamic_cast<EffectContext *>(_localctx)->stateContext = state();
          setState(164);
          match(AnyFXParser::SC);
           _localctx->eff->symbols.push_back(dynamic_cast<EffectContext *>(_localctx)->stateContext->sym); 
          break;
        }

        case 8: {
          setState(167);
          dynamic_cast<EffectContext *>(_localctx)->programContext = program();
          setState(168);
          match(AnyFXParser::SC);
           _localctx->eff->symbols.push_back(dynamic_cast<EffectContext *>(_localctx)->programContext->sym); 
          break;
        }

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

AnyFXParser::AliasContext::AliasContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> AnyFXParser::AliasContext::IDENTIFIER() {
  return getTokens(AnyFXParser::IDENTIFIER);
}

tree::TerminalNode* AnyFXParser::AliasContext::IDENTIFIER(size_t i) {
  return getToken(AnyFXParser::IDENTIFIER, i);
}


size_t AnyFXParser::AliasContext::getRuleIndex() const {
  return AnyFXParser::RuleAlias;
}


AnyFXParser::AliasContext* AnyFXParser::alias() {
  AliasContext *_localctx = _tracker.createInstance<AliasContext>(_ctx, getState());
  enterRule(_localctx, 10, AnyFXParser::RuleAlias);

          dynamic_cast<AliasContext *>(_localctx)->sym =  nullptr;
          std::string name;
          std::string type;
      

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(176);
    match(AnyFXParser::T__3);
    setState(177);
    dynamic_cast<AliasContext *>(_localctx)->name = match(AnyFXParser::IDENTIFIER);
    setState(178);
    match(AnyFXParser::T__4);
    setState(179);
    dynamic_cast<AliasContext *>(_localctx)->type = match(AnyFXParser::IDENTIFIER);
     name = (dynamic_cast<AliasContext *>(_localctx)->name != nullptr ? dynamic_cast<AliasContext *>(_localctx)->name->getText() : ""); type = (dynamic_cast<AliasContext *>(_localctx)->type != nullptr ? dynamic_cast<AliasContext *>(_localctx)->type->getText() : ""); 

            dynamic_cast<AliasContext *>(_localctx)->sym =  new Alias();
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

AnyFXParser::AnnotationContext::AnnotationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* AnyFXParser::AnnotationContext::SOBAKA() {
  return getToken(AnyFXParser::SOBAKA, 0);
}

tree::TerminalNode* AnyFXParser::AnnotationContext::LP() {
  return getToken(AnyFXParser::LP, 0);
}

tree::TerminalNode* AnyFXParser::AnnotationContext::RP() {
  return getToken(AnyFXParser::RP, 0);
}

tree::TerminalNode* AnyFXParser::AnnotationContext::IDENTIFIER() {
  return getToken(AnyFXParser::IDENTIFIER, 0);
}

AnyFXParser::ExpressionContext* AnyFXParser::AnnotationContext::expression() {
  return getRuleContext<AnyFXParser::ExpressionContext>(0);
}


size_t AnyFXParser::AnnotationContext::getRuleIndex() const {
  return AnyFXParser::RuleAnnotation;
}


AnyFXParser::AnnotationContext* AnyFXParser::annotation() {
  AnnotationContext *_localctx = _tracker.createInstance<AnnotationContext>(_ctx, getState());
  enterRule(_localctx, 12, AnyFXParser::RuleAnnotation);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(183);
    match(AnyFXParser::SOBAKA);

    setState(184);
    dynamic_cast<AnnotationContext *>(_localctx)->name = match(AnyFXParser::IDENTIFIER);
    setState(185);
    match(AnyFXParser::LP);
    setState(186);
    dynamic_cast<AnnotationContext *>(_localctx)->value = expression();
    setState(187);
    match(AnyFXParser::RP);
     _localctx->annot.name = (dynamic_cast<AnnotationContext *>(_localctx)->name != nullptr ? dynamic_cast<AnnotationContext *>(_localctx)->name->getText() : ""); _localctx->annot.value = dynamic_cast<AnnotationContext *>(_localctx)->value->tree; 
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AttributeContext ------------------------------------------------------------------

AnyFXParser::AttributeContext::AttributeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* AnyFXParser::AttributeContext::LP() {
  return getToken(AnyFXParser::LP, 0);
}

AnyFXParser::ExpressionContext* AnyFXParser::AttributeContext::expression() {
  return getRuleContext<AnyFXParser::ExpressionContext>(0);
}

tree::TerminalNode* AnyFXParser::AttributeContext::RP() {
  return getToken(AnyFXParser::RP, 0);
}

tree::TerminalNode* AnyFXParser::AttributeContext::IDENTIFIER() {
  return getToken(AnyFXParser::IDENTIFIER, 0);
}


size_t AnyFXParser::AttributeContext::getRuleIndex() const {
  return AnyFXParser::RuleAttribute;
}


AnyFXParser::AttributeContext* AnyFXParser::attribute() {
  AttributeContext *_localctx = _tracker.createInstance<AttributeContext>(_ctx, getState());
  enterRule(_localctx, 14, AnyFXParser::RuleAttribute);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(198);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 8, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(190);
      dynamic_cast<AttributeContext *>(_localctx)->name = match(AnyFXParser::IDENTIFIER);
      setState(191);
      match(AnyFXParser::LP);
      setState(192);
      dynamic_cast<AttributeContext *>(_localctx)->expressionContext = expression();
      setState(193);
      match(AnyFXParser::RP);
       _localctx->attr.name = (dynamic_cast<AttributeContext *>(_localctx)->name != nullptr ? dynamic_cast<AttributeContext *>(_localctx)->name->getText() : ""); _localctx->attr.expression = dynamic_cast<AttributeContext *>(_localctx)->expressionContext->tree; 
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(196);
      dynamic_cast<AttributeContext *>(_localctx)->name = match(AnyFXParser::IDENTIFIER);
       _localctx->attr.name = (dynamic_cast<AttributeContext *>(_localctx)->name != nullptr ? dynamic_cast<AttributeContext *>(_localctx)->name->getText() : ""); _localctx->attr.expression = nullptr; 
      break;
    }

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

AnyFXParser::TypeDeclarationContext::TypeDeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* AnyFXParser::TypeDeclarationContext::IDENTIFIER() {
  return getToken(AnyFXParser::IDENTIFIER, 0);
}


size_t AnyFXParser::TypeDeclarationContext::getRuleIndex() const {
  return AnyFXParser::RuleTypeDeclaration;
}


AnyFXParser::TypeDeclarationContext* AnyFXParser::typeDeclaration() {
  TypeDeclarationContext *_localctx = _tracker.createInstance<TypeDeclarationContext>(_ctx, getState());
  enterRule(_localctx, 16, AnyFXParser::RuleTypeDeclaration);

          _localctx->type.name = "";
      

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(200);
    dynamic_cast<TypeDeclarationContext *>(_localctx)->identifierToken = match(AnyFXParser::IDENTIFIER);
     _localctx->type.name = (dynamic_cast<TypeDeclarationContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<TypeDeclarationContext *>(_localctx)->identifierToken->getText() : ""); 
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VariablesContext ------------------------------------------------------------------

AnyFXParser::VariablesContext::VariablesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

AnyFXParser::TypeDeclarationContext* AnyFXParser::VariablesContext::typeDeclaration() {
  return getRuleContext<AnyFXParser::TypeDeclarationContext>(0);
}

std::vector<AnyFXParser::AnnotationContext *> AnyFXParser::VariablesContext::annotation() {
  return getRuleContexts<AnyFXParser::AnnotationContext>();
}

AnyFXParser::AnnotationContext* AnyFXParser::VariablesContext::annotation(size_t i) {
  return getRuleContext<AnyFXParser::AnnotationContext>(i);
}

std::vector<AnyFXParser::AttributeContext *> AnyFXParser::VariablesContext::attribute() {
  return getRuleContexts<AnyFXParser::AttributeContext>();
}

AnyFXParser::AttributeContext* AnyFXParser::VariablesContext::attribute(size_t i) {
  return getRuleContext<AnyFXParser::AttributeContext>(i);
}

std::vector<AnyFXParser::AssignmentExpressionContext *> AnyFXParser::VariablesContext::assignmentExpression() {
  return getRuleContexts<AnyFXParser::AssignmentExpressionContext>();
}

AnyFXParser::AssignmentExpressionContext* AnyFXParser::VariablesContext::assignmentExpression(size_t i) {
  return getRuleContext<AnyFXParser::AssignmentExpressionContext>(i);
}

std::vector<tree::TerminalNode *> AnyFXParser::VariablesContext::CO() {
  return getTokens(AnyFXParser::CO);
}

tree::TerminalNode* AnyFXParser::VariablesContext::CO(size_t i) {
  return getToken(AnyFXParser::CO, i);
}


size_t AnyFXParser::VariablesContext::getRuleIndex() const {
  return AnyFXParser::RuleVariables;
}


AnyFXParser::VariablesContext* AnyFXParser::variables() {
  VariablesContext *_localctx = _tracker.createInstance<VariablesContext>(_ctx, getState());
  enterRule(_localctx, 18, AnyFXParser::RuleVariables);

          std::vector<Annotation> annotations;
          std::vector<Attribute> attributes;
          std::vector<Expression*> nameExpressions;
          Symbol::Location location;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(208);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == AnyFXParser::SOBAKA) {
      setState(203);
      dynamic_cast<VariablesContext *>(_localctx)->annotationContext = annotation();
       annotations.push_back(dynamic_cast<VariablesContext *>(_localctx)->annotationContext->annot); 
      setState(210);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(216);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 10, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(211);
        dynamic_cast<VariablesContext *>(_localctx)->attributeContext = attribute();
         attributes.push_back(dynamic_cast<VariablesContext *>(_localctx)->attributeContext->attr);  
      }
      setState(218);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 10, _ctx);
    }
    setState(219);
    dynamic_cast<VariablesContext *>(_localctx)->typeDeclarationContext = typeDeclaration();
     location = SetupFile(); 

    setState(221);
    dynamic_cast<VariablesContext *>(_localctx)->var0 = assignmentExpression();
     nameExpressions.push_back(dynamic_cast<VariablesContext *>(_localctx)->var0->tree); 
    setState(229);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == AnyFXParser::CO) {
      setState(223);
      match(AnyFXParser::CO);
      setState(224);
      dynamic_cast<VariablesContext *>(_localctx)->varN = assignmentExpression();
       nameExpressions.push_back(dynamic_cast<VariablesContext *>(_localctx)->varN->tree); 
      setState(231);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }

            for (Expression* expr : nameExpressions)
            {
                Variable* var = new Variable(); 
                var->type = dynamic_cast<VariablesContext *>(_localctx)->typeDeclarationContext->type; 
                var->location = location; 
                var->annotations = annotations; 
                var->attributes = attributes;
                var->nameExpression = expr;
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

//----------------- VariableContext ------------------------------------------------------------------

AnyFXParser::VariableContext::VariableContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

AnyFXParser::TypeDeclarationContext* AnyFXParser::VariableContext::typeDeclaration() {
  return getRuleContext<AnyFXParser::TypeDeclarationContext>(0);
}

AnyFXParser::ExpressionContext* AnyFXParser::VariableContext::expression() {
  return getRuleContext<AnyFXParser::ExpressionContext>(0);
}

std::vector<AnyFXParser::AttributeContext *> AnyFXParser::VariableContext::attribute() {
  return getRuleContexts<AnyFXParser::AttributeContext>();
}

AnyFXParser::AttributeContext* AnyFXParser::VariableContext::attribute(size_t i) {
  return getRuleContext<AnyFXParser::AttributeContext>(i);
}


size_t AnyFXParser::VariableContext::getRuleIndex() const {
  return AnyFXParser::RuleVariable;
}


AnyFXParser::VariableContext* AnyFXParser::variable() {
  VariableContext *_localctx = _tracker.createInstance<VariableContext>(_ctx, getState());
  enterRule(_localctx, 20, AnyFXParser::RuleVariable);

          dynamic_cast<VariableContext *>(_localctx)->sym =  nullptr;
          std::vector<Attribute> attributes;
          std::vector<Expression*> nameExpressions;
          Expression* nameExpression = nullptr;
          Symbol::Location location;
      

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(239);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 12, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(234);
        dynamic_cast<VariableContext *>(_localctx)->attributeContext = attribute();
         attributes.push_back(dynamic_cast<VariableContext *>(_localctx)->attributeContext->attr);  
      }
      setState(241);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 12, _ctx);
    }
    setState(242);
    dynamic_cast<VariableContext *>(_localctx)->typeDeclarationContext = typeDeclaration();
     location = SetupFile(); 
    setState(244);
    dynamic_cast<VariableContext *>(_localctx)->var0 = expression();
     nameExpression = dynamic_cast<VariableContext *>(_localctx)->var0->tree; 

            dynamic_cast<VariableContext *>(_localctx)->sym =  new Variable(); 
            _localctx->sym->type = dynamic_cast<VariableContext *>(_localctx)->typeDeclarationContext->type; 
            _localctx->sym->location = location; 
            _localctx->sym->attributes = attributes;
            _localctx->sym->nameExpression = nameExpression;
        
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StructureDeclarationContext ------------------------------------------------------------------

AnyFXParser::StructureDeclarationContext::StructureDeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* AnyFXParser::StructureDeclarationContext::IDENTIFIER() {
  return getToken(AnyFXParser::IDENTIFIER, 0);
}

std::vector<AnyFXParser::AnnotationContext *> AnyFXParser::StructureDeclarationContext::annotation() {
  return getRuleContexts<AnyFXParser::AnnotationContext>();
}

AnyFXParser::AnnotationContext* AnyFXParser::StructureDeclarationContext::annotation(size_t i) {
  return getRuleContext<AnyFXParser::AnnotationContext>(i);
}

std::vector<AnyFXParser::AttributeContext *> AnyFXParser::StructureDeclarationContext::attribute() {
  return getRuleContexts<AnyFXParser::AttributeContext>();
}

AnyFXParser::AttributeContext* AnyFXParser::StructureDeclarationContext::attribute(size_t i) {
  return getRuleContext<AnyFXParser::AttributeContext>(i);
}


size_t AnyFXParser::StructureDeclarationContext::getRuleIndex() const {
  return AnyFXParser::RuleStructureDeclaration;
}


AnyFXParser::StructureDeclarationContext* AnyFXParser::structureDeclaration() {
  StructureDeclarationContext *_localctx = _tracker.createInstance<StructureDeclarationContext>(_ctx, getState());
  enterRule(_localctx, 22, AnyFXParser::RuleStructureDeclaration);

          dynamic_cast<StructureDeclarationContext *>(_localctx)->sym =  nullptr;
          std::vector<Annotation> annotations;
          std::vector<Attribute> attributes;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(253);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == AnyFXParser::SOBAKA) {
      setState(248);
      dynamic_cast<StructureDeclarationContext *>(_localctx)->annotationContext = annotation();
       annotations.push_back(dynamic_cast<StructureDeclarationContext *>(_localctx)->annotationContext->annot); 
      setState(255);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(256);
    match(AnyFXParser::T__5);
    setState(262);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 14, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(257);
        dynamic_cast<StructureDeclarationContext *>(_localctx)->attributeContext = attribute();
         attributes.push_back(dynamic_cast<StructureDeclarationContext *>(_localctx)->attributeContext->attr);  
      }
      setState(264);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 14, _ctx);
    }
    setState(265);
    dynamic_cast<StructureDeclarationContext *>(_localctx)->name = match(AnyFXParser::IDENTIFIER);
     
            dynamic_cast<StructureDeclarationContext *>(_localctx)->sym =  new Structure();
            _localctx->sym->name = (dynamic_cast<StructureDeclarationContext *>(_localctx)->name != nullptr ? dynamic_cast<StructureDeclarationContext *>(_localctx)->name->getText() : ""); 
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

AnyFXParser::StructureContext::StructureContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

AnyFXParser::StructureDeclarationContext* AnyFXParser::StructureContext::structureDeclaration() {
  return getRuleContext<AnyFXParser::StructureDeclarationContext>(0);
}

tree::TerminalNode* AnyFXParser::StructureContext::LB() {
  return getToken(AnyFXParser::LB, 0);
}

tree::TerminalNode* AnyFXParser::StructureContext::RB() {
  return getToken(AnyFXParser::RB, 0);
}

std::vector<AnyFXParser::VariablesContext *> AnyFXParser::StructureContext::variables() {
  return getRuleContexts<AnyFXParser::VariablesContext>();
}

AnyFXParser::VariablesContext* AnyFXParser::StructureContext::variables(size_t i) {
  return getRuleContext<AnyFXParser::VariablesContext>(i);
}

std::vector<tree::TerminalNode *> AnyFXParser::StructureContext::SC() {
  return getTokens(AnyFXParser::SC);
}

tree::TerminalNode* AnyFXParser::StructureContext::SC(size_t i) {
  return getToken(AnyFXParser::SC, i);
}


size_t AnyFXParser::StructureContext::getRuleIndex() const {
  return AnyFXParser::RuleStructure;
}


AnyFXParser::StructureContext* AnyFXParser::structure() {
  StructureContext *_localctx = _tracker.createInstance<StructureContext>(_ctx, getState());
  enterRule(_localctx, 24, AnyFXParser::RuleStructure);

          dynamic_cast<StructureContext *>(_localctx)->sym =  nullptr;
          std::vector<Symbol*> members;
          bool isArray = false;
          Expression* arraySizeExpression = nullptr;
          std::string instanceName;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(268);
    dynamic_cast<StructureContext *>(_localctx)->structureDeclarationContext = structureDeclaration();
     dynamic_cast<StructureContext *>(_localctx)->sym =  dynamic_cast<StructureContext *>(_localctx)->structureDeclarationContext->sym; 
    setState(270);
    match(AnyFXParser::LB);
    setState(277);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == AnyFXParser::SOBAKA

    || _la == AnyFXParser::IDENTIFIER) {
      setState(271);
      dynamic_cast<StructureContext *>(_localctx)->variablesContext = variables();
       for(Variable* var : dynamic_cast<StructureContext *>(_localctx)->variablesContext->list) { members.push_back(var); }
      setState(273);
      match(AnyFXParser::SC);
      setState(279);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(280);
    match(AnyFXParser::RB);
     
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

AnyFXParser::EnumerationContext::EnumerationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* AnyFXParser::EnumerationContext::LB() {
  return getToken(AnyFXParser::LB, 0);
}

tree::TerminalNode* AnyFXParser::EnumerationContext::RB() {
  return getToken(AnyFXParser::RB, 0);
}

std::vector<tree::TerminalNode *> AnyFXParser::EnumerationContext::IDENTIFIER() {
  return getTokens(AnyFXParser::IDENTIFIER);
}

tree::TerminalNode* AnyFXParser::EnumerationContext::IDENTIFIER(size_t i) {
  return getToken(AnyFXParser::IDENTIFIER, i);
}

tree::TerminalNode* AnyFXParser::EnumerationContext::EQ() {
  return getToken(AnyFXParser::EQ, 0);
}

AnyFXParser::ExpressionContext* AnyFXParser::EnumerationContext::expression() {
  return getRuleContext<AnyFXParser::ExpressionContext>(0);
}


size_t AnyFXParser::EnumerationContext::getRuleIndex() const {
  return AnyFXParser::RuleEnumeration;
}


AnyFXParser::EnumerationContext* AnyFXParser::enumeration() {
  EnumerationContext *_localctx = _tracker.createInstance<EnumerationContext>(_ctx, getState());
  enterRule(_localctx, 26, AnyFXParser::RuleEnumeration);

          dynamic_cast<EnumerationContext *>(_localctx)->sym =  nullptr;
          std::vector<std::string> enumLabels;
          std::vector<Expression*> enumValues;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(283);
    match(AnyFXParser::T__6);
    setState(284);
    dynamic_cast<EnumerationContext *>(_localctx)->name = match(AnyFXParser::IDENTIFIER);
    setState(285);
    match(AnyFXParser::LB);
    setState(286);
    dynamic_cast<EnumerationContext *>(_localctx)->label = match(AnyFXParser::IDENTIFIER);
     Expression* expr = nullptr; 
    setState(292);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == AnyFXParser::EQ) {
      setState(288);
      match(AnyFXParser::EQ);
      setState(289);
      dynamic_cast<EnumerationContext *>(_localctx)->value = expression();
       expr = dynamic_cast<EnumerationContext *>(_localctx)->value->tree; 
    }

                enumLabels.push_back((dynamic_cast<EnumerationContext *>(_localctx)->label != nullptr ? dynamic_cast<EnumerationContext *>(_localctx)->label->getText() : ""));
                enumValues.push_back(expr);
            
    setState(295);
    match(AnyFXParser::RB);

            dynamic_cast<EnumerationContext *>(_localctx)->sym =  new Enumeration();
            _localctx->sym->labels = enumLabels;
            _localctx->sym->values = enumValues;
            _localctx->sym->location = SetupFile();
        
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionDeclarationContext ------------------------------------------------------------------

AnyFXParser::FunctionDeclarationContext::FunctionDeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* AnyFXParser::FunctionDeclarationContext::LP() {
  return getToken(AnyFXParser::LP, 0);
}

tree::TerminalNode* AnyFXParser::FunctionDeclarationContext::RP() {
  return getToken(AnyFXParser::RP, 0);
}

AnyFXParser::TypeDeclarationContext* AnyFXParser::FunctionDeclarationContext::typeDeclaration() {
  return getRuleContext<AnyFXParser::TypeDeclarationContext>(0);
}

tree::TerminalNode* AnyFXParser::FunctionDeclarationContext::IDENTIFIER() {
  return getToken(AnyFXParser::IDENTIFIER, 0);
}

std::vector<AnyFXParser::AttributeContext *> AnyFXParser::FunctionDeclarationContext::attribute() {
  return getRuleContexts<AnyFXParser::AttributeContext>();
}

AnyFXParser::AttributeContext* AnyFXParser::FunctionDeclarationContext::attribute(size_t i) {
  return getRuleContext<AnyFXParser::AttributeContext>(i);
}

std::vector<AnyFXParser::VariableContext *> AnyFXParser::FunctionDeclarationContext::variable() {
  return getRuleContexts<AnyFXParser::VariableContext>();
}

AnyFXParser::VariableContext* AnyFXParser::FunctionDeclarationContext::variable(size_t i) {
  return getRuleContext<AnyFXParser::VariableContext>(i);
}

std::vector<tree::TerminalNode *> AnyFXParser::FunctionDeclarationContext::CO() {
  return getTokens(AnyFXParser::CO);
}

tree::TerminalNode* AnyFXParser::FunctionDeclarationContext::CO(size_t i) {
  return getToken(AnyFXParser::CO, i);
}


size_t AnyFXParser::FunctionDeclarationContext::getRuleIndex() const {
  return AnyFXParser::RuleFunctionDeclaration;
}


AnyFXParser::FunctionDeclarationContext* AnyFXParser::functionDeclaration() {
  FunctionDeclarationContext *_localctx = _tracker.createInstance<FunctionDeclarationContext>(_ctx, getState());
  enterRule(_localctx, 28, AnyFXParser::RuleFunctionDeclaration);

          dynamic_cast<FunctionDeclarationContext *>(_localctx)->sym =  nullptr;
          std::vector<Variable*> variables;
          std::vector<Attribute> attributes;
          Symbol::Location location;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(303);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 17, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(298);
        dynamic_cast<FunctionDeclarationContext *>(_localctx)->attributeContext = attribute();
         attributes.push_back(dynamic_cast<FunctionDeclarationContext *>(_localctx)->attributeContext->attr);  
      }
      setState(305);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 17, _ctx);
    }
    setState(306);
    dynamic_cast<FunctionDeclarationContext *>(_localctx)->returnType = typeDeclaration();
    setState(307);
    dynamic_cast<FunctionDeclarationContext *>(_localctx)->name = match(AnyFXParser::IDENTIFIER);
     location = SetupFile(); 
    setState(309);
    match(AnyFXParser::LP);
    setState(321);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == AnyFXParser::IDENTIFIER) {
      setState(310);
      dynamic_cast<FunctionDeclarationContext *>(_localctx)->arg0 = variable();
       variables.push_back(dynamic_cast<FunctionDeclarationContext *>(_localctx)->arg0->sym); 
      setState(318);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == AnyFXParser::CO) {
        setState(312);
        match(AnyFXParser::CO);
        setState(313);
        dynamic_cast<FunctionDeclarationContext *>(_localctx)->argn = variable();
         variables.push_back(dynamic_cast<FunctionDeclarationContext *>(_localctx)->argn->sym); 
        setState(320);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(323);
    match(AnyFXParser::RP);

            dynamic_cast<FunctionDeclarationContext *>(_localctx)->sym =  new Function(); 
            _localctx->sym->hasBody = false;
            _localctx->sym->location = location;
            _localctx->sym->returnType = dynamic_cast<FunctionDeclarationContext *>(_localctx)->returnType->type; 
            _localctx->sym->name = (dynamic_cast<FunctionDeclarationContext *>(_localctx)->name != nullptr ? dynamic_cast<FunctionDeclarationContext *>(_localctx)->name->getText() : ""); 
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

AnyFXParser::CodeblockContext::CodeblockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* AnyFXParser::CodeblockContext::LB() {
  return getToken(AnyFXParser::LB, 0);
}

tree::TerminalNode* AnyFXParser::CodeblockContext::RB() {
  return getToken(AnyFXParser::RB, 0);
}

std::vector<AnyFXParser::CodeblockContext *> AnyFXParser::CodeblockContext::codeblock() {
  return getRuleContexts<AnyFXParser::CodeblockContext>();
}

AnyFXParser::CodeblockContext* AnyFXParser::CodeblockContext::codeblock(size_t i) {
  return getRuleContext<AnyFXParser::CodeblockContext>(i);
}


size_t AnyFXParser::CodeblockContext::getRuleIndex() const {
  return AnyFXParser::RuleCodeblock;
}


AnyFXParser::CodeblockContext* AnyFXParser::codeblock() {
  CodeblockContext *_localctx = _tracker.createInstance<CodeblockContext>(_ctx, getState());
  enterRule(_localctx, 30, AnyFXParser::RuleCodeblock);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(335);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case AnyFXParser::LB: {
        enterOuterAlt(_localctx, 1);
        setState(326);
        match(AnyFXParser::LB);
        setState(330);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << AnyFXParser::T__0)
          | (1ULL << AnyFXParser::T__1)
          | (1ULL << AnyFXParser::T__2)
          | (1ULL << AnyFXParser::T__3)
          | (1ULL << AnyFXParser::T__4)
          | (1ULL << AnyFXParser::T__5)
          | (1ULL << AnyFXParser::T__6)
          | (1ULL << AnyFXParser::T__7)
          | (1ULL << AnyFXParser::T__8)
          | (1ULL << AnyFXParser::T__9)
          | (1ULL << AnyFXParser::T__10)
          | (1ULL << AnyFXParser::T__11)
          | (1ULL << AnyFXParser::T__12)
          | (1ULL << AnyFXParser::T__13)
          | (1ULL << AnyFXParser::T__14)
          | (1ULL << AnyFXParser::T__15)
          | (1ULL << AnyFXParser::T__16)
          | (1ULL << AnyFXParser::T__17)
          | (1ULL << AnyFXParser::T__18)
          | (1ULL << AnyFXParser::T__19)
          | (1ULL << AnyFXParser::T__20)
          | (1ULL << AnyFXParser::T__21)
          | (1ULL << AnyFXParser::T__22)
          | (1ULL << AnyFXParser::T__23)
          | (1ULL << AnyFXParser::T__24)
          | (1ULL << AnyFXParser::T__25)
          | (1ULL << AnyFXParser::T__26)
          | (1ULL << AnyFXParser::T__27)
          | (1ULL << AnyFXParser::T__28)
          | (1ULL << AnyFXParser::T__29)
          | (1ULL << AnyFXParser::T__30)
          | (1ULL << AnyFXParser::T__31)
          | (1ULL << AnyFXParser::T__32)
          | (1ULL << AnyFXParser::T__33)
          | (1ULL << AnyFXParser::SC)
          | (1ULL << AnyFXParser::CO)
          | (1ULL << AnyFXParser::COL)
          | (1ULL << AnyFXParser::LP)
          | (1ULL << AnyFXParser::RP)
          | (1ULL << AnyFXParser::LB)
          | (1ULL << AnyFXParser::LL)
          | (1ULL << AnyFXParser::RR)
          | (1ULL << AnyFXParser::DOT)
          | (1ULL << AnyFXParser::NOT)
          | (1ULL << AnyFXParser::EQ)
          | (1ULL << AnyFXParser::QO)
          | (1ULL << AnyFXParser::QU)
          | (1ULL << AnyFXParser::AND)
          | (1ULL << AnyFXParser::ANDSET)
          | (1ULL << AnyFXParser::OR)
          | (1ULL << AnyFXParser::ORSET)
          | (1ULL << AnyFXParser::XOR)
          | (1ULL << AnyFXParser::XORSET)
          | (1ULL << AnyFXParser::CONNJUGATE)
          | (1ULL << AnyFXParser::Q)
          | (1ULL << AnyFXParser::NU)
          | (1ULL << AnyFXParser::FORWARDSLASH)
          | (1ULL << AnyFXParser::LESS)
          | (1ULL << AnyFXParser::LESSEQ)
          | (1ULL << AnyFXParser::GREATER)
          | (1ULL << AnyFXParser::GREATEREQ)
          | (1ULL << AnyFXParser::LOGICEQ))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 64)) & ((1ULL << (AnyFXParser::NOTEQ - 64))
          | (1ULL << (AnyFXParser::LOGICAND - 64))
          | (1ULL << (AnyFXParser::LOGICOR - 64))
          | (1ULL << (AnyFXParser::MOD - 64))
          | (1ULL << (AnyFXParser::UNDERSC - 64))
          | (1ULL << (AnyFXParser::SOBAKA - 64))
          | (1ULL << (AnyFXParser::ADD_OP - 64))
          | (1ULL << (AnyFXParser::SUB_OP - 64))
          | (1ULL << (AnyFXParser::DIV_OP - 64))
          | (1ULL << (AnyFXParser::MUL_OP - 64))
          | (1ULL << (AnyFXParser::ARROW - 64))
          | (1ULL << (AnyFXParser::INTEGERLITERAL - 64))
          | (1ULL << (AnyFXParser::UINTEGERLITERAL - 64))
          | (1ULL << (AnyFXParser::COMMENT - 64))
          | (1ULL << (AnyFXParser::ML_COMMENT - 64))
          | (1ULL << (AnyFXParser::FLOATLITERAL - 64))
          | (1ULL << (AnyFXParser::EXPONENT - 64))
          | (1ULL << (AnyFXParser::DOUBLELITERAL - 64))
          | (1ULL << (AnyFXParser::HEX - 64))
          | (1ULL << (AnyFXParser::IDENTIFIER - 64))
          | (1ULL << (AnyFXParser::WS - 64)))) != 0)) {
          setState(327);
          codeblock();
          setState(332);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(333);
        match(AnyFXParser::RB);
        break;
      }

      case AnyFXParser::T__0:
      case AnyFXParser::T__1:
      case AnyFXParser::T__2:
      case AnyFXParser::T__3:
      case AnyFXParser::T__4:
      case AnyFXParser::T__5:
      case AnyFXParser::T__6:
      case AnyFXParser::T__7:
      case AnyFXParser::T__8:
      case AnyFXParser::T__9:
      case AnyFXParser::T__10:
      case AnyFXParser::T__11:
      case AnyFXParser::T__12:
      case AnyFXParser::T__13:
      case AnyFXParser::T__14:
      case AnyFXParser::T__15:
      case AnyFXParser::T__16:
      case AnyFXParser::T__17:
      case AnyFXParser::T__18:
      case AnyFXParser::T__19:
      case AnyFXParser::T__20:
      case AnyFXParser::T__21:
      case AnyFXParser::T__22:
      case AnyFXParser::T__23:
      case AnyFXParser::T__24:
      case AnyFXParser::T__25:
      case AnyFXParser::T__26:
      case AnyFXParser::T__27:
      case AnyFXParser::T__28:
      case AnyFXParser::T__29:
      case AnyFXParser::T__30:
      case AnyFXParser::T__31:
      case AnyFXParser::T__32:
      case AnyFXParser::T__33:
      case AnyFXParser::SC:
      case AnyFXParser::CO:
      case AnyFXParser::COL:
      case AnyFXParser::LP:
      case AnyFXParser::RP:
      case AnyFXParser::LL:
      case AnyFXParser::RR:
      case AnyFXParser::DOT:
      case AnyFXParser::NOT:
      case AnyFXParser::EQ:
      case AnyFXParser::QO:
      case AnyFXParser::QU:
      case AnyFXParser::AND:
      case AnyFXParser::ANDSET:
      case AnyFXParser::OR:
      case AnyFXParser::ORSET:
      case AnyFXParser::XOR:
      case AnyFXParser::XORSET:
      case AnyFXParser::CONNJUGATE:
      case AnyFXParser::Q:
      case AnyFXParser::NU:
      case AnyFXParser::FORWARDSLASH:
      case AnyFXParser::LESS:
      case AnyFXParser::LESSEQ:
      case AnyFXParser::GREATER:
      case AnyFXParser::GREATEREQ:
      case AnyFXParser::LOGICEQ:
      case AnyFXParser::NOTEQ:
      case AnyFXParser::LOGICAND:
      case AnyFXParser::LOGICOR:
      case AnyFXParser::MOD:
      case AnyFXParser::UNDERSC:
      case AnyFXParser::SOBAKA:
      case AnyFXParser::ADD_OP:
      case AnyFXParser::SUB_OP:
      case AnyFXParser::DIV_OP:
      case AnyFXParser::MUL_OP:
      case AnyFXParser::ARROW:
      case AnyFXParser::INTEGERLITERAL:
      case AnyFXParser::UINTEGERLITERAL:
      case AnyFXParser::COMMENT:
      case AnyFXParser::ML_COMMENT:
      case AnyFXParser::FLOATLITERAL:
      case AnyFXParser::EXPONENT:
      case AnyFXParser::DOUBLELITERAL:
      case AnyFXParser::HEX:
      case AnyFXParser::IDENTIFIER:
      case AnyFXParser::WS: {
        enterOuterAlt(_localctx, 2);
        setState(334);
        _la = _input->LA(1);
        if (_la == 0 || _la == Token::EOF || (_la == AnyFXParser::LB

        || _la == AnyFXParser::RB)) {
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

AnyFXParser::FunctionContext::FunctionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

AnyFXParser::FunctionDeclarationContext* AnyFXParser::FunctionContext::functionDeclaration() {
  return getRuleContext<AnyFXParser::FunctionDeclarationContext>(0);
}

AnyFXParser::ScopeStatementContext* AnyFXParser::FunctionContext::scopeStatement() {
  return getRuleContext<AnyFXParser::ScopeStatementContext>(0);
}


size_t AnyFXParser::FunctionContext::getRuleIndex() const {
  return AnyFXParser::RuleFunction;
}


AnyFXParser::FunctionContext* AnyFXParser::function() {
  FunctionContext *_localctx = _tracker.createInstance<FunctionContext>(_ctx, getState());
  enterRule(_localctx, 32, AnyFXParser::RuleFunction);

          dynamic_cast<FunctionContext *>(_localctx)->sym =  nullptr;
          Token* startToken = nullptr;
          Token* endToken = nullptr;
      

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(337);
    dynamic_cast<FunctionContext *>(_localctx)->functionDeclarationContext = functionDeclaration();
     dynamic_cast<FunctionContext *>(_localctx)->sym =  dynamic_cast<FunctionContext *>(_localctx)->functionDeclarationContext->sym; 

            startToken = _input->LT(2);
        
    setState(340);
    dynamic_cast<FunctionContext *>(_localctx)->scopeStatementContext = scopeStatement();

            endToken = _input->LT(-2);

            // extract code from between tokens
            antlr4::misc::Interval interval;
            interval.a = startToken->getTokenIndex();
            interval.b = endToken->getTokenIndex();
            _localctx->sym->body = _input->getText(interval);
            _localctx->sym->hasBody = true;
            _localctx->sym->ast = dynamic_cast<FunctionContext *>(_localctx)->scopeStatementContext->tree;
        
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ProgramContext ------------------------------------------------------------------

AnyFXParser::ProgramContext::ProgramContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* AnyFXParser::ProgramContext::LB() {
  return getToken(AnyFXParser::LB, 0);
}

tree::TerminalNode* AnyFXParser::ProgramContext::RB() {
  return getToken(AnyFXParser::RB, 0);
}

tree::TerminalNode* AnyFXParser::ProgramContext::IDENTIFIER() {
  return getToken(AnyFXParser::IDENTIFIER, 0);
}

std::vector<AnyFXParser::AnnotationContext *> AnyFXParser::ProgramContext::annotation() {
  return getRuleContexts<AnyFXParser::AnnotationContext>();
}

AnyFXParser::AnnotationContext* AnyFXParser::ProgramContext::annotation(size_t i) {
  return getRuleContext<AnyFXParser::AnnotationContext>(i);
}

std::vector<tree::TerminalNode *> AnyFXParser::ProgramContext::SC() {
  return getTokens(AnyFXParser::SC);
}

tree::TerminalNode* AnyFXParser::ProgramContext::SC(size_t i) {
  return getToken(AnyFXParser::SC, i);
}

std::vector<AnyFXParser::ExpressionContext *> AnyFXParser::ProgramContext::expression() {
  return getRuleContexts<AnyFXParser::ExpressionContext>();
}

AnyFXParser::ExpressionContext* AnyFXParser::ProgramContext::expression(size_t i) {
  return getRuleContext<AnyFXParser::ExpressionContext>(i);
}


size_t AnyFXParser::ProgramContext::getRuleIndex() const {
  return AnyFXParser::RuleProgram;
}


AnyFXParser::ProgramContext* AnyFXParser::program() {
  ProgramContext *_localctx = _tracker.createInstance<ProgramContext>(_ctx, getState());
  enterRule(_localctx, 34, AnyFXParser::RuleProgram);

          dynamic_cast<ProgramContext *>(_localctx)->sym =  nullptr;
          Symbol::Location location;
          std::vector<Program::SubroutineMapping> subroutines;
          std::vector<Expression*> entries;
          std::vector<Annotation> annotations;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(348);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == AnyFXParser::SOBAKA) {
      setState(343);
      dynamic_cast<ProgramContext *>(_localctx)->annotationContext = annotation();
       annotations.push_back(dynamic_cast<ProgramContext *>(_localctx)->annotationContext->annot); 
      setState(350);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(351);
    match(AnyFXParser::T__7);
    setState(352);
    dynamic_cast<ProgramContext *>(_localctx)->name = match(AnyFXParser::IDENTIFIER);
     location = SetupFile(); 
    setState(354);
    match(AnyFXParser::LB);
    setState(361);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << AnyFXParser::T__0)
      | (1ULL << AnyFXParser::T__1)
      | (1ULL << AnyFXParser::T__31)
      | (1ULL << AnyFXParser::T__32)
      | (1ULL << AnyFXParser::LP)
      | (1ULL << AnyFXParser::LB)
      | (1ULL << AnyFXParser::NOT)
      | (1ULL << AnyFXParser::QO)
      | (1ULL << AnyFXParser::CONNJUGATE)
      | (1ULL << AnyFXParser::Q))) != 0) || ((((_la - 70) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 70)) & ((1ULL << (AnyFXParser::ADD_OP - 70))
      | (1ULL << (AnyFXParser::SUB_OP - 70))
      | (1ULL << (AnyFXParser::MUL_OP - 70))
      | (1ULL << (AnyFXParser::INTEGERLITERAL - 70))
      | (1ULL << (AnyFXParser::UINTEGERLITERAL - 70))
      | (1ULL << (AnyFXParser::FLOATLITERAL - 70))
      | (1ULL << (AnyFXParser::DOUBLELITERAL - 70))
      | (1ULL << (AnyFXParser::HEX - 70))
      | (1ULL << (AnyFXParser::IDENTIFIER - 70)))) != 0)) {
      setState(355);
      dynamic_cast<ProgramContext *>(_localctx)->assignment = expression();
       entries.push_back(dynamic_cast<ProgramContext *>(_localctx)->assignment->tree); 
      setState(357);
      match(AnyFXParser::SC);
      setState(363);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(364);
    match(AnyFXParser::RB);
     
            dynamic_cast<ProgramContext *>(_localctx)->sym =  new Program();
            _localctx->sym->location = location;
            _localctx->sym->name = (dynamic_cast<ProgramContext *>(_localctx)->name != nullptr ? dynamic_cast<ProgramContext *>(_localctx)->name->getText() : "");
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

AnyFXParser::StateContext::StateContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* AnyFXParser::StateContext::LB() {
  return getToken(AnyFXParser::LB, 0);
}

tree::TerminalNode* AnyFXParser::StateContext::RB() {
  return getToken(AnyFXParser::RB, 0);
}

tree::TerminalNode* AnyFXParser::StateContext::IDENTIFIER() {
  return getToken(AnyFXParser::IDENTIFIER, 0);
}

std::vector<tree::TerminalNode *> AnyFXParser::StateContext::SC() {
  return getTokens(AnyFXParser::SC);
}

tree::TerminalNode* AnyFXParser::StateContext::SC(size_t i) {
  return getToken(AnyFXParser::SC, i);
}

std::vector<AnyFXParser::ExpressionContext *> AnyFXParser::StateContext::expression() {
  return getRuleContexts<AnyFXParser::ExpressionContext>();
}

AnyFXParser::ExpressionContext* AnyFXParser::StateContext::expression(size_t i) {
  return getRuleContext<AnyFXParser::ExpressionContext>(i);
}


size_t AnyFXParser::StateContext::getRuleIndex() const {
  return AnyFXParser::RuleState;
}


AnyFXParser::StateContext* AnyFXParser::state() {
  StateContext *_localctx = _tracker.createInstance<StateContext>(_ctx, getState());
  enterRule(_localctx, 36, AnyFXParser::RuleState);

          Expression* arrayExpression = nullptr;
          Symbol::Location location;
          std::vector<Expression*> entries;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(371);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case AnyFXParser::T__8: {
        setState(367);
        match(AnyFXParser::T__8);
         dynamic_cast<StateContext *>(_localctx)->sym =  new SamplerState(); 
        break;
      }

      case AnyFXParser::T__9: {
        setState(369);
        match(AnyFXParser::T__9);
         dynamic_cast<StateContext *>(_localctx)->sym =  new RenderState(); 
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(373);
    dynamic_cast<StateContext *>(_localctx)->name = match(AnyFXParser::IDENTIFIER);
     location = SetupFile(); 
    setState(375);
    match(AnyFXParser::LB);
    setState(382);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << AnyFXParser::T__0)
      | (1ULL << AnyFXParser::T__1)
      | (1ULL << AnyFXParser::T__31)
      | (1ULL << AnyFXParser::T__32)
      | (1ULL << AnyFXParser::LP)
      | (1ULL << AnyFXParser::LB)
      | (1ULL << AnyFXParser::NOT)
      | (1ULL << AnyFXParser::QO)
      | (1ULL << AnyFXParser::CONNJUGATE)
      | (1ULL << AnyFXParser::Q))) != 0) || ((((_la - 70) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 70)) & ((1ULL << (AnyFXParser::ADD_OP - 70))
      | (1ULL << (AnyFXParser::SUB_OP - 70))
      | (1ULL << (AnyFXParser::MUL_OP - 70))
      | (1ULL << (AnyFXParser::INTEGERLITERAL - 70))
      | (1ULL << (AnyFXParser::UINTEGERLITERAL - 70))
      | (1ULL << (AnyFXParser::FLOATLITERAL - 70))
      | (1ULL << (AnyFXParser::DOUBLELITERAL - 70))
      | (1ULL << (AnyFXParser::HEX - 70))
      | (1ULL << (AnyFXParser::IDENTIFIER - 70)))) != 0)) {
      setState(376);
      dynamic_cast<StateContext *>(_localctx)->assign = expression();
       entries.push_back(dynamic_cast<StateContext *>(_localctx)->assign->tree); 
      setState(378);
      match(AnyFXParser::SC);
      setState(384);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(385);
    match(AnyFXParser::RB);

            _localctx->sym->name = (dynamic_cast<StateContext *>(_localctx)->name != nullptr ? dynamic_cast<StateContext *>(_localctx)->name->getText() : "");
            _localctx->sym->location = location;
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

AnyFXParser::StatementContext::StatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

AnyFXParser::IfStatementContext* AnyFXParser::StatementContext::ifStatement() {
  return getRuleContext<AnyFXParser::IfStatementContext>(0);
}

AnyFXParser::ScopeStatementContext* AnyFXParser::StatementContext::scopeStatement() {
  return getRuleContext<AnyFXParser::ScopeStatementContext>(0);
}

AnyFXParser::ForStatementContext* AnyFXParser::StatementContext::forStatement() {
  return getRuleContext<AnyFXParser::ForStatementContext>(0);
}

AnyFXParser::WhileStatementContext* AnyFXParser::StatementContext::whileStatement() {
  return getRuleContext<AnyFXParser::WhileStatementContext>(0);
}

AnyFXParser::ReturnStatementContext* AnyFXParser::StatementContext::returnStatement() {
  return getRuleContext<AnyFXParser::ReturnStatementContext>(0);
}

AnyFXParser::ContinueStatementContext* AnyFXParser::StatementContext::continueStatement() {
  return getRuleContext<AnyFXParser::ContinueStatementContext>(0);
}

AnyFXParser::BreakStatementContext* AnyFXParser::StatementContext::breakStatement() {
  return getRuleContext<AnyFXParser::BreakStatementContext>(0);
}

AnyFXParser::ExpressionStatementContext* AnyFXParser::StatementContext::expressionStatement() {
  return getRuleContext<AnyFXParser::ExpressionStatementContext>(0);
}

tree::TerminalNode* AnyFXParser::StatementContext::SC() {
  return getToken(AnyFXParser::SC, 0);
}


size_t AnyFXParser::StatementContext::getRuleIndex() const {
  return AnyFXParser::RuleStatement;
}


AnyFXParser::StatementContext* AnyFXParser::statement() {
  StatementContext *_localctx = _tracker.createInstance<StatementContext>(_ctx, getState());
  enterRule(_localctx, 38, AnyFXParser::RuleStatement);

          dynamic_cast<StatementContext *>(_localctx)->tree =  nullptr;
      

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(413);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 26, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(388);
      dynamic_cast<StatementContext *>(_localctx)->ifStatementContext = ifStatement();
       dynamic_cast<StatementContext *>(_localctx)->tree =  dynamic_cast<StatementContext *>(_localctx)->ifStatementContext->tree; 
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(391);
      dynamic_cast<StatementContext *>(_localctx)->scopeStatementContext = scopeStatement();
       dynamic_cast<StatementContext *>(_localctx)->tree =  dynamic_cast<StatementContext *>(_localctx)->scopeStatementContext->tree; 
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(394);
      dynamic_cast<StatementContext *>(_localctx)->forStatementContext = forStatement();
       dynamic_cast<StatementContext *>(_localctx)->tree =  dynamic_cast<StatementContext *>(_localctx)->forStatementContext->tree; 
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(397);
      dynamic_cast<StatementContext *>(_localctx)->whileStatementContext = whileStatement();
       dynamic_cast<StatementContext *>(_localctx)->tree =  dynamic_cast<StatementContext *>(_localctx)->whileStatementContext->tree; 
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(400);
      dynamic_cast<StatementContext *>(_localctx)->returnStatementContext = returnStatement();
       dynamic_cast<StatementContext *>(_localctx)->tree =  dynamic_cast<StatementContext *>(_localctx)->returnStatementContext->tree; 
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(403);
      dynamic_cast<StatementContext *>(_localctx)->continueStatementContext = continueStatement();
       dynamic_cast<StatementContext *>(_localctx)->tree =  dynamic_cast<StatementContext *>(_localctx)->continueStatementContext->tree; 
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(406);
      dynamic_cast<StatementContext *>(_localctx)->breakStatementContext = breakStatement();
       dynamic_cast<StatementContext *>(_localctx)->tree =  dynamic_cast<StatementContext *>(_localctx)->breakStatementContext->tree; 
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(409);
      dynamic_cast<StatementContext *>(_localctx)->expressionStatementContext = expressionStatement();
      setState(410);
      match(AnyFXParser::SC);
       dynamic_cast<StatementContext *>(_localctx)->tree =  dynamic_cast<StatementContext *>(_localctx)->expressionStatementContext->tree; 
      break;
    }

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

AnyFXParser::ExpressionStatementContext::ExpressionStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

AnyFXParser::ExpressionContext* AnyFXParser::ExpressionStatementContext::expression() {
  return getRuleContext<AnyFXParser::ExpressionContext>(0);
}


size_t AnyFXParser::ExpressionStatementContext::getRuleIndex() const {
  return AnyFXParser::RuleExpressionStatement;
}


AnyFXParser::ExpressionStatementContext* AnyFXParser::expressionStatement() {
  ExpressionStatementContext *_localctx = _tracker.createInstance<ExpressionStatementContext>(_ctx, getState());
  enterRule(_localctx, 40, AnyFXParser::RuleExpressionStatement);

          dynamic_cast<ExpressionStatementContext *>(_localctx)->tree =  nullptr;
      

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(415);
    dynamic_cast<ExpressionStatementContext *>(_localctx)->expressionContext = expression();

            dynamic_cast<ExpressionStatementContext *>(_localctx)->tree =  new ExpressionStatement(dynamic_cast<ExpressionStatementContext *>(_localctx)->expressionContext->tree);
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

AnyFXParser::IfStatementContext::IfStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* AnyFXParser::IfStatementContext::LP() {
  return getToken(AnyFXParser::LP, 0);
}

tree::TerminalNode* AnyFXParser::IfStatementContext::RP() {
  return getToken(AnyFXParser::RP, 0);
}

AnyFXParser::ExpressionContext* AnyFXParser::IfStatementContext::expression() {
  return getRuleContext<AnyFXParser::ExpressionContext>(0);
}

std::vector<AnyFXParser::StatementContext *> AnyFXParser::IfStatementContext::statement() {
  return getRuleContexts<AnyFXParser::StatementContext>();
}

AnyFXParser::StatementContext* AnyFXParser::IfStatementContext::statement(size_t i) {
  return getRuleContext<AnyFXParser::StatementContext>(i);
}


size_t AnyFXParser::IfStatementContext::getRuleIndex() const {
  return AnyFXParser::RuleIfStatement;
}


AnyFXParser::IfStatementContext* AnyFXParser::ifStatement() {
  IfStatementContext *_localctx = _tracker.createInstance<IfStatementContext>(_ctx, getState());
  enterRule(_localctx, 42, AnyFXParser::RuleIfStatement);

          dynamic_cast<IfStatementContext *>(_localctx)->tree =  nullptr;
          Expression* condition = nullptr;
          Statement* ifBody = nullptr;
          Statement* elseBody = nullptr;
          Symbol::Location location;
      

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(418);
    match(AnyFXParser::T__10);
     location = SetupFile(); 
    setState(420);
    match(AnyFXParser::LP);
    setState(421);
    dynamic_cast<IfStatementContext *>(_localctx)->condition = expression();
     condition = dynamic_cast<IfStatementContext *>(_localctx)->condition->tree; 
    setState(423);
    match(AnyFXParser::RP);
    setState(424);
    dynamic_cast<IfStatementContext *>(_localctx)->ifBody = statement();
     ifBody = dynamic_cast<IfStatementContext *>(_localctx)->ifBody->tree; 
    setState(430);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 27, _ctx)) {
    case 1: {
      setState(426);
      match(AnyFXParser::T__11);
      setState(427);
      dynamic_cast<IfStatementContext *>(_localctx)->elseBody = statement();
       elseBody = dynamic_cast<IfStatementContext *>(_localctx)->elseBody->tree; 
      break;
    }

    }

            dynamic_cast<IfStatementContext *>(_localctx)->tree =  new IfStatement(condition, ifBody, elseBody);
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

AnyFXParser::ForStatementContext::ForStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* AnyFXParser::ForStatementContext::LP() {
  return getToken(AnyFXParser::LP, 0);
}

std::vector<tree::TerminalNode *> AnyFXParser::ForStatementContext::SC() {
  return getTokens(AnyFXParser::SC);
}

tree::TerminalNode* AnyFXParser::ForStatementContext::SC(size_t i) {
  return getToken(AnyFXParser::SC, i);
}

tree::TerminalNode* AnyFXParser::ForStatementContext::RP() {
  return getToken(AnyFXParser::RP, 0);
}

AnyFXParser::StatementContext* AnyFXParser::ForStatementContext::statement() {
  return getRuleContext<AnyFXParser::StatementContext>(0);
}

AnyFXParser::VariablesContext* AnyFXParser::ForStatementContext::variables() {
  return getRuleContext<AnyFXParser::VariablesContext>(0);
}

std::vector<AnyFXParser::ExpressionContext *> AnyFXParser::ForStatementContext::expression() {
  return getRuleContexts<AnyFXParser::ExpressionContext>();
}

AnyFXParser::ExpressionContext* AnyFXParser::ForStatementContext::expression(size_t i) {
  return getRuleContext<AnyFXParser::ExpressionContext>(i);
}


size_t AnyFXParser::ForStatementContext::getRuleIndex() const {
  return AnyFXParser::RuleForStatement;
}


AnyFXParser::ForStatementContext* AnyFXParser::forStatement() {
  ForStatementContext *_localctx = _tracker.createInstance<ForStatementContext>(_ctx, getState());
  enterRule(_localctx, 44, AnyFXParser::RuleForStatement);

          dynamic_cast<ForStatementContext *>(_localctx)->tree =  nullptr;
          std::vector<Variable*> declarations;
          Expression* conditionExpression = nullptr;
          Expression* loopExpression = nullptr;
          Statement* contents = nullptr;
          Symbol::Location location;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(434);
    match(AnyFXParser::T__12);
     location = SetupFile(); 
    setState(436);
    match(AnyFXParser::LP);
    setState(440);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == AnyFXParser::SOBAKA

    || _la == AnyFXParser::IDENTIFIER) {
      setState(437);
      dynamic_cast<ForStatementContext *>(_localctx)->variablesContext = variables();
       declarations = dynamic_cast<ForStatementContext *>(_localctx)->variablesContext->list; 
    }
    setState(442);
    match(AnyFXParser::SC);
    setState(446);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << AnyFXParser::T__0)
      | (1ULL << AnyFXParser::T__1)
      | (1ULL << AnyFXParser::T__31)
      | (1ULL << AnyFXParser::T__32)
      | (1ULL << AnyFXParser::LP)
      | (1ULL << AnyFXParser::LB)
      | (1ULL << AnyFXParser::NOT)
      | (1ULL << AnyFXParser::QO)
      | (1ULL << AnyFXParser::CONNJUGATE)
      | (1ULL << AnyFXParser::Q))) != 0) || ((((_la - 70) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 70)) & ((1ULL << (AnyFXParser::ADD_OP - 70))
      | (1ULL << (AnyFXParser::SUB_OP - 70))
      | (1ULL << (AnyFXParser::MUL_OP - 70))
      | (1ULL << (AnyFXParser::INTEGERLITERAL - 70))
      | (1ULL << (AnyFXParser::UINTEGERLITERAL - 70))
      | (1ULL << (AnyFXParser::FLOATLITERAL - 70))
      | (1ULL << (AnyFXParser::DOUBLELITERAL - 70))
      | (1ULL << (AnyFXParser::HEX - 70))
      | (1ULL << (AnyFXParser::IDENTIFIER - 70)))) != 0)) {
      setState(443);
      dynamic_cast<ForStatementContext *>(_localctx)->condition = expression();
       conditionExpression = dynamic_cast<ForStatementContext *>(_localctx)->condition->tree; 
    }
    setState(448);
    match(AnyFXParser::SC);
    setState(452);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << AnyFXParser::T__0)
      | (1ULL << AnyFXParser::T__1)
      | (1ULL << AnyFXParser::T__31)
      | (1ULL << AnyFXParser::T__32)
      | (1ULL << AnyFXParser::LP)
      | (1ULL << AnyFXParser::LB)
      | (1ULL << AnyFXParser::NOT)
      | (1ULL << AnyFXParser::QO)
      | (1ULL << AnyFXParser::CONNJUGATE)
      | (1ULL << AnyFXParser::Q))) != 0) || ((((_la - 70) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 70)) & ((1ULL << (AnyFXParser::ADD_OP - 70))
      | (1ULL << (AnyFXParser::SUB_OP - 70))
      | (1ULL << (AnyFXParser::MUL_OP - 70))
      | (1ULL << (AnyFXParser::INTEGERLITERAL - 70))
      | (1ULL << (AnyFXParser::UINTEGERLITERAL - 70))
      | (1ULL << (AnyFXParser::FLOATLITERAL - 70))
      | (1ULL << (AnyFXParser::DOUBLELITERAL - 70))
      | (1ULL << (AnyFXParser::HEX - 70))
      | (1ULL << (AnyFXParser::IDENTIFIER - 70)))) != 0)) {
      setState(449);
      dynamic_cast<ForStatementContext *>(_localctx)->loop = expression();
       loopExpression = dynamic_cast<ForStatementContext *>(_localctx)->loop->tree; 
    }
    setState(454);
    match(AnyFXParser::RP);
    setState(455);
    dynamic_cast<ForStatementContext *>(_localctx)->content = statement();
     contents = dynamic_cast<ForStatementContext *>(_localctx)->content->tree; 

            dynamic_cast<ForStatementContext *>(_localctx)->tree =  new ForStatement(declarations, conditionExpression, loopExpression, contents);
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

AnyFXParser::ForRangeStatementContext::ForRangeStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* AnyFXParser::ForRangeStatementContext::LP() {
  return getToken(AnyFXParser::LP, 0);
}

tree::TerminalNode* AnyFXParser::ForRangeStatementContext::COL() {
  return getToken(AnyFXParser::COL, 0);
}

tree::TerminalNode* AnyFXParser::ForRangeStatementContext::RP() {
  return getToken(AnyFXParser::RP, 0);
}

std::vector<tree::TerminalNode *> AnyFXParser::ForRangeStatementContext::IDENTIFIER() {
  return getTokens(AnyFXParser::IDENTIFIER);
}

tree::TerminalNode* AnyFXParser::ForRangeStatementContext::IDENTIFIER(size_t i) {
  return getToken(AnyFXParser::IDENTIFIER, i);
}

AnyFXParser::StatementContext* AnyFXParser::ForRangeStatementContext::statement() {
  return getRuleContext<AnyFXParser::StatementContext>(0);
}


size_t AnyFXParser::ForRangeStatementContext::getRuleIndex() const {
  return AnyFXParser::RuleForRangeStatement;
}


AnyFXParser::ForRangeStatementContext* AnyFXParser::forRangeStatement() {
  ForRangeStatementContext *_localctx = _tracker.createInstance<ForRangeStatementContext>(_ctx, getState());
  enterRule(_localctx, 46, AnyFXParser::RuleForRangeStatement);

          dynamic_cast<ForRangeStatementContext *>(_localctx)->tree =  nullptr;
          Statement* contents = nullptr;
          Symbol::Location location;
      

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(459);
    match(AnyFXParser::T__12);
     location = SetupFile(); 
    setState(461);
    match(AnyFXParser::LP);
    setState(462);
    dynamic_cast<ForRangeStatementContext *>(_localctx)->iterator = match(AnyFXParser::IDENTIFIER);
    setState(463);
    match(AnyFXParser::COL);
    setState(464);
    dynamic_cast<ForRangeStatementContext *>(_localctx)->start = match(AnyFXParser::IDENTIFIER);
    setState(465);
    match(AnyFXParser::T__13);
    setState(466);
    dynamic_cast<ForRangeStatementContext *>(_localctx)->end = match(AnyFXParser::IDENTIFIER);
    setState(467);
    match(AnyFXParser::RP);
    setState(468);
    dynamic_cast<ForRangeStatementContext *>(_localctx)->content = statement();
     contents = dynamic_cast<ForRangeStatementContext *>(_localctx)->content->tree; 


        
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- WhileStatementContext ------------------------------------------------------------------

AnyFXParser::WhileStatementContext::WhileStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* AnyFXParser::WhileStatementContext::LP() {
  return getToken(AnyFXParser::LP, 0);
}

tree::TerminalNode* AnyFXParser::WhileStatementContext::RP() {
  return getToken(AnyFXParser::RP, 0);
}

AnyFXParser::ExpressionContext* AnyFXParser::WhileStatementContext::expression() {
  return getRuleContext<AnyFXParser::ExpressionContext>(0);
}

AnyFXParser::StatementContext* AnyFXParser::WhileStatementContext::statement() {
  return getRuleContext<AnyFXParser::StatementContext>(0);
}

tree::TerminalNode* AnyFXParser::WhileStatementContext::SC() {
  return getToken(AnyFXParser::SC, 0);
}


size_t AnyFXParser::WhileStatementContext::getRuleIndex() const {
  return AnyFXParser::RuleWhileStatement;
}


AnyFXParser::WhileStatementContext* AnyFXParser::whileStatement() {
  WhileStatementContext *_localctx = _tracker.createInstance<WhileStatementContext>(_ctx, getState());
  enterRule(_localctx, 48, AnyFXParser::RuleWhileStatement);

          dynamic_cast<WhileStatementContext *>(_localctx)->tree =  nullptr;
          Expression* conditionExpression = nullptr;
          Statement* contents = nullptr;
          bool isDoWhile = false;
          Symbol::Location location;
      

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(494);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case AnyFXParser::T__14: {
        enterOuterAlt(_localctx, 1);
        setState(472);
        match(AnyFXParser::T__14);
         location = SetupFile(); 
        setState(474);
        match(AnyFXParser::LP);
        setState(475);
        dynamic_cast<WhileStatementContext *>(_localctx)->condition = expression();
         conditionExpression = dynamic_cast<WhileStatementContext *>(_localctx)->condition->tree; 
        setState(477);
        match(AnyFXParser::RP);
        setState(478);
        dynamic_cast<WhileStatementContext *>(_localctx)->content = statement();
         contents = dynamic_cast<WhileStatementContext *>(_localctx)->content->tree; 

                dynamic_cast<WhileStatementContext *>(_localctx)->tree =  new WhileStatement(conditionExpression, contents, isDoWhile);
                _localctx->tree->location = location;
            
        break;
      }

      case AnyFXParser::T__15: {
        enterOuterAlt(_localctx, 2);
        setState(482);
        match(AnyFXParser::T__15);
         location = SetupFile(); 
        setState(484);
        dynamic_cast<WhileStatementContext *>(_localctx)->content = statement();
         contents = dynamic_cast<WhileStatementContext *>(_localctx)->content->tree; isDoWhile = true; 
        setState(486);
        match(AnyFXParser::T__14);
        setState(487);
        match(AnyFXParser::LP);
        setState(488);
        dynamic_cast<WhileStatementContext *>(_localctx)->condition = expression();
         conditionExpression = dynamic_cast<WhileStatementContext *>(_localctx)->condition->tree; 
        setState(490);
        match(AnyFXParser::RP);
        setState(491);
        match(AnyFXParser::SC);

                dynamic_cast<WhileStatementContext *>(_localctx)->tree =  new WhileStatement(conditionExpression, contents, isDoWhile);
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

AnyFXParser::ScopeStatementContext::ScopeStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* AnyFXParser::ScopeStatementContext::LB() {
  return getToken(AnyFXParser::LB, 0);
}

tree::TerminalNode* AnyFXParser::ScopeStatementContext::RB() {
  return getToken(AnyFXParser::RB, 0);
}

std::vector<AnyFXParser::StatementContext *> AnyFXParser::ScopeStatementContext::statement() {
  return getRuleContexts<AnyFXParser::StatementContext>();
}

AnyFXParser::StatementContext* AnyFXParser::ScopeStatementContext::statement(size_t i) {
  return getRuleContext<AnyFXParser::StatementContext>(i);
}

std::vector<AnyFXParser::VariablesContext *> AnyFXParser::ScopeStatementContext::variables() {
  return getRuleContexts<AnyFXParser::VariablesContext>();
}

AnyFXParser::VariablesContext* AnyFXParser::ScopeStatementContext::variables(size_t i) {
  return getRuleContext<AnyFXParser::VariablesContext>(i);
}

std::vector<tree::TerminalNode *> AnyFXParser::ScopeStatementContext::SC() {
  return getTokens(AnyFXParser::SC);
}

tree::TerminalNode* AnyFXParser::ScopeStatementContext::SC(size_t i) {
  return getToken(AnyFXParser::SC, i);
}


size_t AnyFXParser::ScopeStatementContext::getRuleIndex() const {
  return AnyFXParser::RuleScopeStatement;
}


AnyFXParser::ScopeStatementContext* AnyFXParser::scopeStatement() {
  ScopeStatementContext *_localctx = _tracker.createInstance<ScopeStatementContext>(_ctx, getState());
  enterRule(_localctx, 50, AnyFXParser::RuleScopeStatement);

          dynamic_cast<ScopeStatementContext *>(_localctx)->tree =  nullptr;
          std::vector<Symbol*> contents;
          Symbol::Location location;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(496);
    match(AnyFXParser::LB);
     location = SetupFile(); 
    setState(507);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << AnyFXParser::T__0)
      | (1ULL << AnyFXParser::T__1)
      | (1ULL << AnyFXParser::T__10)
      | (1ULL << AnyFXParser::T__12)
      | (1ULL << AnyFXParser::T__14)
      | (1ULL << AnyFXParser::T__15)
      | (1ULL << AnyFXParser::T__16)
      | (1ULL << AnyFXParser::T__17)
      | (1ULL << AnyFXParser::T__21)
      | (1ULL << AnyFXParser::T__31)
      | (1ULL << AnyFXParser::T__32)
      | (1ULL << AnyFXParser::LP)
      | (1ULL << AnyFXParser::LB)
      | (1ULL << AnyFXParser::NOT)
      | (1ULL << AnyFXParser::QO)
      | (1ULL << AnyFXParser::CONNJUGATE)
      | (1ULL << AnyFXParser::Q))) != 0) || ((((_la - 69) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 69)) & ((1ULL << (AnyFXParser::SOBAKA - 69))
      | (1ULL << (AnyFXParser::ADD_OP - 69))
      | (1ULL << (AnyFXParser::SUB_OP - 69))
      | (1ULL << (AnyFXParser::MUL_OP - 69))
      | (1ULL << (AnyFXParser::INTEGERLITERAL - 69))
      | (1ULL << (AnyFXParser::UINTEGERLITERAL - 69))
      | (1ULL << (AnyFXParser::FLOATLITERAL - 69))
      | (1ULL << (AnyFXParser::DOUBLELITERAL - 69))
      | (1ULL << (AnyFXParser::HEX - 69))
      | (1ULL << (AnyFXParser::IDENTIFIER - 69)))) != 0)) {
      setState(505);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 32, _ctx)) {
      case 1: {
        setState(498);
        dynamic_cast<ScopeStatementContext *>(_localctx)->statementContext = statement();
         contents.push_back(dynamic_cast<ScopeStatementContext *>(_localctx)->statementContext->tree); 
        break;
      }

      case 2: {
        setState(501);
        dynamic_cast<ScopeStatementContext *>(_localctx)->variablesContext = variables();
        setState(502);
        match(AnyFXParser::SC);
         for(Variable* var : dynamic_cast<ScopeStatementContext *>(_localctx)->variablesContext->list) { contents.push_back(var); } 
        break;
      }

      }
      setState(509);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(510);
    match(AnyFXParser::RB);

            dynamic_cast<ScopeStatementContext *>(_localctx)->tree =  new ScopeStatement(contents);
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

AnyFXParser::ReturnStatementContext::ReturnStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* AnyFXParser::ReturnStatementContext::SC() {
  return getToken(AnyFXParser::SC, 0);
}

AnyFXParser::ExpressionContext* AnyFXParser::ReturnStatementContext::expression() {
  return getRuleContext<AnyFXParser::ExpressionContext>(0);
}


size_t AnyFXParser::ReturnStatementContext::getRuleIndex() const {
  return AnyFXParser::RuleReturnStatement;
}


AnyFXParser::ReturnStatementContext* AnyFXParser::returnStatement() {
  ReturnStatementContext *_localctx = _tracker.createInstance<ReturnStatementContext>(_ctx, getState());
  enterRule(_localctx, 52, AnyFXParser::RuleReturnStatement);

          dynamic_cast<ReturnStatementContext *>(_localctx)->tree =  nullptr;
          Expression* returnValue = nullptr;
          Symbol::Location location;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(513);
    match(AnyFXParser::T__16);
     location = SetupFile(); 
    setState(518);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << AnyFXParser::T__0)
      | (1ULL << AnyFXParser::T__1)
      | (1ULL << AnyFXParser::T__31)
      | (1ULL << AnyFXParser::T__32)
      | (1ULL << AnyFXParser::LP)
      | (1ULL << AnyFXParser::LB)
      | (1ULL << AnyFXParser::NOT)
      | (1ULL << AnyFXParser::QO)
      | (1ULL << AnyFXParser::CONNJUGATE)
      | (1ULL << AnyFXParser::Q))) != 0) || ((((_la - 70) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 70)) & ((1ULL << (AnyFXParser::ADD_OP - 70))
      | (1ULL << (AnyFXParser::SUB_OP - 70))
      | (1ULL << (AnyFXParser::MUL_OP - 70))
      | (1ULL << (AnyFXParser::INTEGERLITERAL - 70))
      | (1ULL << (AnyFXParser::UINTEGERLITERAL - 70))
      | (1ULL << (AnyFXParser::FLOATLITERAL - 70))
      | (1ULL << (AnyFXParser::DOUBLELITERAL - 70))
      | (1ULL << (AnyFXParser::HEX - 70))
      | (1ULL << (AnyFXParser::IDENTIFIER - 70)))) != 0)) {
      setState(515);
      dynamic_cast<ReturnStatementContext *>(_localctx)->value = expression();
       returnValue = dynamic_cast<ReturnStatementContext *>(_localctx)->value->tree; 
    }
    setState(520);
    match(AnyFXParser::SC);

            dynamic_cast<ReturnStatementContext *>(_localctx)->tree =  new ReturnStatement(returnValue);
        
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ContinueStatementContext ------------------------------------------------------------------

AnyFXParser::ContinueStatementContext::ContinueStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* AnyFXParser::ContinueStatementContext::SC() {
  return getToken(AnyFXParser::SC, 0);
}


size_t AnyFXParser::ContinueStatementContext::getRuleIndex() const {
  return AnyFXParser::RuleContinueStatement;
}


AnyFXParser::ContinueStatementContext* AnyFXParser::continueStatement() {
  ContinueStatementContext *_localctx = _tracker.createInstance<ContinueStatementContext>(_ctx, getState());
  enterRule(_localctx, 54, AnyFXParser::RuleContinueStatement);

          dynamic_cast<ContinueStatementContext *>(_localctx)->tree =  nullptr;
          Symbol::Location location;
      

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(523);
    match(AnyFXParser::T__17);
     location = SetupFile(); 
    setState(525);
    match(AnyFXParser::SC);

            dynamic_cast<ContinueStatementContext *>(_localctx)->tree =  new ContinueStatement();
        
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SwitchStatementContext ------------------------------------------------------------------

AnyFXParser::SwitchStatementContext::SwitchStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* AnyFXParser::SwitchStatementContext::LP() {
  return getToken(AnyFXParser::LP, 0);
}

AnyFXParser::ExpressionContext* AnyFXParser::SwitchStatementContext::expression() {
  return getRuleContext<AnyFXParser::ExpressionContext>(0);
}

tree::TerminalNode* AnyFXParser::SwitchStatementContext::RP() {
  return getToken(AnyFXParser::RP, 0);
}

tree::TerminalNode* AnyFXParser::SwitchStatementContext::LB() {
  return getToken(AnyFXParser::LB, 0);
}

tree::TerminalNode* AnyFXParser::SwitchStatementContext::RB() {
  return getToken(AnyFXParser::RB, 0);
}

std::vector<tree::TerminalNode *> AnyFXParser::SwitchStatementContext::IDENTIFIER() {
  return getTokens(AnyFXParser::IDENTIFIER);
}

tree::TerminalNode* AnyFXParser::SwitchStatementContext::IDENTIFIER(size_t i) {
  return getToken(AnyFXParser::IDENTIFIER, i);
}

std::vector<tree::TerminalNode *> AnyFXParser::SwitchStatementContext::COL() {
  return getTokens(AnyFXParser::COL);
}

tree::TerminalNode* AnyFXParser::SwitchStatementContext::COL(size_t i) {
  return getToken(AnyFXParser::COL, i);
}

std::vector<AnyFXParser::StatementContext *> AnyFXParser::SwitchStatementContext::statement() {
  return getRuleContexts<AnyFXParser::StatementContext>();
}

AnyFXParser::StatementContext* AnyFXParser::SwitchStatementContext::statement(size_t i) {
  return getRuleContext<AnyFXParser::StatementContext>(i);
}


size_t AnyFXParser::SwitchStatementContext::getRuleIndex() const {
  return AnyFXParser::RuleSwitchStatement;
}


AnyFXParser::SwitchStatementContext* AnyFXParser::switchStatement() {
  SwitchStatementContext *_localctx = _tracker.createInstance<SwitchStatementContext>(_ctx, getState());
  enterRule(_localctx, 56, AnyFXParser::RuleSwitchStatement);

          dynamic_cast<SwitchStatementContext *>(_localctx)->tree =  nullptr;
          Expression* switchExpression;
          std::vector<std::string> caseValues;
          std::vector<Statement*> caseStatements;
          Symbol::Location location;
          Statement* defaultStatement = nullptr;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(528);
    match(AnyFXParser::T__18);
     location = SetupFile(); 
    setState(530);
    match(AnyFXParser::LP);
    setState(531);
    dynamic_cast<SwitchStatementContext *>(_localctx)->expressionContext = expression();
    setState(532);
    match(AnyFXParser::RP);
     switchExpression = dynamic_cast<SwitchStatementContext *>(_localctx)->expressionContext->tree; 
    setState(534);
    match(AnyFXParser::LB);
    setState(543);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == AnyFXParser::T__19) {
      setState(535);
      match(AnyFXParser::T__19);
      setState(536);
      dynamic_cast<SwitchStatementContext *>(_localctx)->identifierToken = match(AnyFXParser::IDENTIFIER);
      setState(537);
      match(AnyFXParser::COL);
      setState(538);
      dynamic_cast<SwitchStatementContext *>(_localctx)->statementContext = statement();
       
                      caseValues.push_back((dynamic_cast<SwitchStatementContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<SwitchStatementContext *>(_localctx)->identifierToken->getText() : "")); 
                      caseStatements.push_back(dynamic_cast<SwitchStatementContext *>(_localctx)->statementContext->tree);
                  
      setState(545);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(551);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == AnyFXParser::T__20) {
      setState(546);
      match(AnyFXParser::T__20);
      setState(547);
      match(AnyFXParser::COL);
      setState(548);
      dynamic_cast<SwitchStatementContext *>(_localctx)->statementContext = statement();

                      defaultStatement = dynamic_cast<SwitchStatementContext *>(_localctx)->statementContext->tree;
                  
    }
    setState(553);
    match(AnyFXParser::RB);

            dynamic_cast<SwitchStatementContext *>(_localctx)->tree =  new SwitchStatement(switchExpression, caseValues, caseStatements, defaultStatement);
        
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BreakStatementContext ------------------------------------------------------------------

AnyFXParser::BreakStatementContext::BreakStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* AnyFXParser::BreakStatementContext::SC() {
  return getToken(AnyFXParser::SC, 0);
}


size_t AnyFXParser::BreakStatementContext::getRuleIndex() const {
  return AnyFXParser::RuleBreakStatement;
}


AnyFXParser::BreakStatementContext* AnyFXParser::breakStatement() {
  BreakStatementContext *_localctx = _tracker.createInstance<BreakStatementContext>(_ctx, getState());
  enterRule(_localctx, 58, AnyFXParser::RuleBreakStatement);

          dynamic_cast<BreakStatementContext *>(_localctx)->tree =  nullptr;
          Symbol::Location location;
      

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(556);
    match(AnyFXParser::T__21);
    setState(557);
    match(AnyFXParser::SC);

            dynamic_cast<BreakStatementContext *>(_localctx)->tree =  new BreakStatement();
        
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpressionContext ------------------------------------------------------------------

AnyFXParser::ExpressionContext::ExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

AnyFXParser::CommaExpressionContext* AnyFXParser::ExpressionContext::commaExpression() {
  return getRuleContext<AnyFXParser::CommaExpressionContext>(0);
}


size_t AnyFXParser::ExpressionContext::getRuleIndex() const {
  return AnyFXParser::RuleExpression;
}


AnyFXParser::ExpressionContext* AnyFXParser::expression() {
  ExpressionContext *_localctx = _tracker.createInstance<ExpressionContext>(_ctx, getState());
  enterRule(_localctx, 60, AnyFXParser::RuleExpression);

          dynamic_cast<ExpressionContext *>(_localctx)->tree =  nullptr;
      

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(560);
    dynamic_cast<ExpressionContext *>(_localctx)->commaExpressionContext = commaExpression();
     dynamic_cast<ExpressionContext *>(_localctx)->tree =  dynamic_cast<ExpressionContext *>(_localctx)->commaExpressionContext->tree; 
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CommaExpressionContext ------------------------------------------------------------------

AnyFXParser::CommaExpressionContext::CommaExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<AnyFXParser::AssignmentExpressionContext *> AnyFXParser::CommaExpressionContext::assignmentExpression() {
  return getRuleContexts<AnyFXParser::AssignmentExpressionContext>();
}

AnyFXParser::AssignmentExpressionContext* AnyFXParser::CommaExpressionContext::assignmentExpression(size_t i) {
  return getRuleContext<AnyFXParser::AssignmentExpressionContext>(i);
}

std::vector<tree::TerminalNode *> AnyFXParser::CommaExpressionContext::CO() {
  return getTokens(AnyFXParser::CO);
}

tree::TerminalNode* AnyFXParser::CommaExpressionContext::CO(size_t i) {
  return getToken(AnyFXParser::CO, i);
}


size_t AnyFXParser::CommaExpressionContext::getRuleIndex() const {
  return AnyFXParser::RuleCommaExpression;
}


AnyFXParser::CommaExpressionContext* AnyFXParser::commaExpression() {
  CommaExpressionContext *_localctx = _tracker.createInstance<CommaExpressionContext>(_ctx, getState());
  enterRule(_localctx, 62, AnyFXParser::RuleCommaExpression);

          dynamic_cast<CommaExpressionContext *>(_localctx)->tree =  nullptr;
      

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(563);
    dynamic_cast<CommaExpressionContext *>(_localctx)->e1 = assignmentExpression();
     dynamic_cast<CommaExpressionContext *>(_localctx)->tree =  dynamic_cast<CommaExpressionContext *>(_localctx)->e1->tree; 
    setState(571);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 37, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(565);
        match(AnyFXParser::CO);
        setState(566);
        dynamic_cast<CommaExpressionContext *>(_localctx)->e2 = assignmentExpression();

                    CommaExpression* expr = new CommaExpression(_localctx->tree, dynamic_cast<CommaExpressionContext *>(_localctx)->e2->tree);
                    expr->location = SetupFile();
                    dynamic_cast<CommaExpressionContext *>(_localctx)->tree =  expr;
                 
      }
      setState(573);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 37, _ctx);
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

AnyFXParser::AssignmentExpressionContext::AssignmentExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<AnyFXParser::LogicalOrExpressionContext *> AnyFXParser::AssignmentExpressionContext::logicalOrExpression() {
  return getRuleContexts<AnyFXParser::LogicalOrExpressionContext>();
}

AnyFXParser::LogicalOrExpressionContext* AnyFXParser::AssignmentExpressionContext::logicalOrExpression(size_t i) {
  return getRuleContext<AnyFXParser::LogicalOrExpressionContext>(i);
}

std::vector<tree::TerminalNode *> AnyFXParser::AssignmentExpressionContext::QU() {
  return getTokens(AnyFXParser::QU);
}

tree::TerminalNode* AnyFXParser::AssignmentExpressionContext::QU(size_t i) {
  return getToken(AnyFXParser::QU, i);
}

std::vector<tree::TerminalNode *> AnyFXParser::AssignmentExpressionContext::COL() {
  return getTokens(AnyFXParser::COL);
}

tree::TerminalNode* AnyFXParser::AssignmentExpressionContext::COL(size_t i) {
  return getToken(AnyFXParser::COL, i);
}

std::vector<AnyFXParser::ExpressionContext *> AnyFXParser::AssignmentExpressionContext::expression() {
  return getRuleContexts<AnyFXParser::ExpressionContext>();
}

AnyFXParser::ExpressionContext* AnyFXParser::AssignmentExpressionContext::expression(size_t i) {
  return getRuleContext<AnyFXParser::ExpressionContext>(i);
}

std::vector<tree::TerminalNode *> AnyFXParser::AssignmentExpressionContext::ANDSET() {
  return getTokens(AnyFXParser::ANDSET);
}

tree::TerminalNode* AnyFXParser::AssignmentExpressionContext::ANDSET(size_t i) {
  return getToken(AnyFXParser::ANDSET, i);
}

std::vector<tree::TerminalNode *> AnyFXParser::AssignmentExpressionContext::XORSET() {
  return getTokens(AnyFXParser::XORSET);
}

tree::TerminalNode* AnyFXParser::AssignmentExpressionContext::XORSET(size_t i) {
  return getToken(AnyFXParser::XORSET, i);
}

std::vector<tree::TerminalNode *> AnyFXParser::AssignmentExpressionContext::ORSET() {
  return getTokens(AnyFXParser::ORSET);
}

tree::TerminalNode* AnyFXParser::AssignmentExpressionContext::ORSET(size_t i) {
  return getToken(AnyFXParser::ORSET, i);
}

std::vector<tree::TerminalNode *> AnyFXParser::AssignmentExpressionContext::EQ() {
  return getTokens(AnyFXParser::EQ);
}

tree::TerminalNode* AnyFXParser::AssignmentExpressionContext::EQ(size_t i) {
  return getToken(AnyFXParser::EQ, i);
}


size_t AnyFXParser::AssignmentExpressionContext::getRuleIndex() const {
  return AnyFXParser::RuleAssignmentExpression;
}


AnyFXParser::AssignmentExpressionContext* AnyFXParser::assignmentExpression() {
  AssignmentExpressionContext *_localctx = _tracker.createInstance<AssignmentExpressionContext>(_ctx, getState());
  enterRule(_localctx, 64, AnyFXParser::RuleAssignmentExpression);

          dynamic_cast<AssignmentExpressionContext *>(_localctx)->tree =  nullptr;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(574);
    dynamic_cast<AssignmentExpressionContext *>(_localctx)->e1 = logicalOrExpression();
     dynamic_cast<AssignmentExpressionContext *>(_localctx)->tree =  dynamic_cast<AssignmentExpressionContext *>(_localctx)->e1->tree; 
    setState(588);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 39, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(586);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case AnyFXParser::T__22:
          case AnyFXParser::T__23:
          case AnyFXParser::T__24:
          case AnyFXParser::T__25:
          case AnyFXParser::T__26:
          case AnyFXParser::T__27:
          case AnyFXParser::T__28:
          case AnyFXParser::EQ:
          case AnyFXParser::ANDSET:
          case AnyFXParser::ORSET:
          case AnyFXParser::XORSET: {
            setState(576);
            dynamic_cast<AssignmentExpressionContext *>(_localctx)->op = _input->LT(1);
            _la = _input->LA(1);
            if (!((((_la & ~ 0x3fULL) == 0) &&
              ((1ULL << _la) & ((1ULL << AnyFXParser::T__22)
              | (1ULL << AnyFXParser::T__23)
              | (1ULL << AnyFXParser::T__24)
              | (1ULL << AnyFXParser::T__25)
              | (1ULL << AnyFXParser::T__26)
              | (1ULL << AnyFXParser::T__27)
              | (1ULL << AnyFXParser::T__28)
              | (1ULL << AnyFXParser::EQ)
              | (1ULL << AnyFXParser::ANDSET)
              | (1ULL << AnyFXParser::ORSET)
              | (1ULL << AnyFXParser::XORSET))) != 0))) {
              dynamic_cast<AssignmentExpressionContext *>(_localctx)->op = _errHandler->recoverInline(this);
            }
            else {
              _errHandler->reportMatch(this);
              consume();
            }
            setState(577);
            dynamic_cast<AssignmentExpressionContext *>(_localctx)->e2 = logicalOrExpression();
             
                        BinaryExpression* expr = new BinaryExpression(StringToFourCC((dynamic_cast<AssignmentExpressionContext *>(_localctx)->op != nullptr ? dynamic_cast<AssignmentExpressionContext *>(_localctx)->op->getText() : "")), _localctx->tree, dynamic_cast<AssignmentExpressionContext *>(_localctx)->e2->tree);
                        expr->location = SetupFile();
                        dynamic_cast<AssignmentExpressionContext *>(_localctx)->tree =  expr;
                    
            break;
          }

          case AnyFXParser::QU: {
            setState(580);
            match(AnyFXParser::QU);
            setState(581);
            dynamic_cast<AssignmentExpressionContext *>(_localctx)->ifBody = expression();
            setState(582);
            match(AnyFXParser::COL);
            setState(583);
            dynamic_cast<AssignmentExpressionContext *>(_localctx)->elseBody = expression();
             
                        TernaryExpression* expr = new TernaryExpression(_localctx->tree, dynamic_cast<AssignmentExpressionContext *>(_localctx)->ifBody->tree, dynamic_cast<AssignmentExpressionContext *>(_localctx)->elseBody->tree);
                        expr->location = SetupFile();
                        dynamic_cast<AssignmentExpressionContext *>(_localctx)->tree =  expr;
                    
            break;
          }

        default:
          throw NoViableAltException(this);
        } 
      }
      setState(590);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 39, _ctx);
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

AnyFXParser::LogicalOrExpressionContext::LogicalOrExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<AnyFXParser::LogicalAndExpressionContext *> AnyFXParser::LogicalOrExpressionContext::logicalAndExpression() {
  return getRuleContexts<AnyFXParser::LogicalAndExpressionContext>();
}

AnyFXParser::LogicalAndExpressionContext* AnyFXParser::LogicalOrExpressionContext::logicalAndExpression(size_t i) {
  return getRuleContext<AnyFXParser::LogicalAndExpressionContext>(i);
}

std::vector<tree::TerminalNode *> AnyFXParser::LogicalOrExpressionContext::LOGICOR() {
  return getTokens(AnyFXParser::LOGICOR);
}

tree::TerminalNode* AnyFXParser::LogicalOrExpressionContext::LOGICOR(size_t i) {
  return getToken(AnyFXParser::LOGICOR, i);
}


size_t AnyFXParser::LogicalOrExpressionContext::getRuleIndex() const {
  return AnyFXParser::RuleLogicalOrExpression;
}


AnyFXParser::LogicalOrExpressionContext* AnyFXParser::logicalOrExpression() {
  LogicalOrExpressionContext *_localctx = _tracker.createInstance<LogicalOrExpressionContext>(_ctx, getState());
  enterRule(_localctx, 66, AnyFXParser::RuleLogicalOrExpression);

          dynamic_cast<LogicalOrExpressionContext *>(_localctx)->tree =  nullptr;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(591);
    dynamic_cast<LogicalOrExpressionContext *>(_localctx)->e1 = logicalAndExpression();
     dynamic_cast<LogicalOrExpressionContext *>(_localctx)->tree =  dynamic_cast<LogicalOrExpressionContext *>(_localctx)->e1->tree; 
    setState(599);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == AnyFXParser::LOGICOR) {
      setState(593);
      match(AnyFXParser::LOGICOR);
      setState(594);
      dynamic_cast<LogicalOrExpressionContext *>(_localctx)->e2 = logicalAndExpression();

                  BinaryExpression* expr = new BinaryExpression('||', _localctx->tree, dynamic_cast<LogicalOrExpressionContext *>(_localctx)->e2->tree);
                  expr->location = SetupFile();
                  dynamic_cast<LogicalOrExpressionContext *>(_localctx)->tree =  expr;
              
      setState(601);
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

AnyFXParser::LogicalAndExpressionContext::LogicalAndExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<AnyFXParser::OrExpressionContext *> AnyFXParser::LogicalAndExpressionContext::orExpression() {
  return getRuleContexts<AnyFXParser::OrExpressionContext>();
}

AnyFXParser::OrExpressionContext* AnyFXParser::LogicalAndExpressionContext::orExpression(size_t i) {
  return getRuleContext<AnyFXParser::OrExpressionContext>(i);
}

std::vector<tree::TerminalNode *> AnyFXParser::LogicalAndExpressionContext::LOGICAND() {
  return getTokens(AnyFXParser::LOGICAND);
}

tree::TerminalNode* AnyFXParser::LogicalAndExpressionContext::LOGICAND(size_t i) {
  return getToken(AnyFXParser::LOGICAND, i);
}


size_t AnyFXParser::LogicalAndExpressionContext::getRuleIndex() const {
  return AnyFXParser::RuleLogicalAndExpression;
}


AnyFXParser::LogicalAndExpressionContext* AnyFXParser::logicalAndExpression() {
  LogicalAndExpressionContext *_localctx = _tracker.createInstance<LogicalAndExpressionContext>(_ctx, getState());
  enterRule(_localctx, 68, AnyFXParser::RuleLogicalAndExpression);

          dynamic_cast<LogicalAndExpressionContext *>(_localctx)->tree =  nullptr;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(602);
    dynamic_cast<LogicalAndExpressionContext *>(_localctx)->e1 = orExpression();
     dynamic_cast<LogicalAndExpressionContext *>(_localctx)->tree =  dynamic_cast<LogicalAndExpressionContext *>(_localctx)->e1->tree; 
    setState(610);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == AnyFXParser::LOGICAND) {
      setState(604);
      match(AnyFXParser::LOGICAND);
      setState(605);
      dynamic_cast<LogicalAndExpressionContext *>(_localctx)->e2 = orExpression();

                  BinaryExpression* expr = new BinaryExpression('&&', _localctx->tree, dynamic_cast<LogicalAndExpressionContext *>(_localctx)->e2->tree);
                  expr->location = SetupFile();
                  dynamic_cast<LogicalAndExpressionContext *>(_localctx)->tree =  expr;
              
      setState(612);
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

AnyFXParser::OrExpressionContext::OrExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<AnyFXParser::XorExpressionContext *> AnyFXParser::OrExpressionContext::xorExpression() {
  return getRuleContexts<AnyFXParser::XorExpressionContext>();
}

AnyFXParser::XorExpressionContext* AnyFXParser::OrExpressionContext::xorExpression(size_t i) {
  return getRuleContext<AnyFXParser::XorExpressionContext>(i);
}

std::vector<tree::TerminalNode *> AnyFXParser::OrExpressionContext::OR() {
  return getTokens(AnyFXParser::OR);
}

tree::TerminalNode* AnyFXParser::OrExpressionContext::OR(size_t i) {
  return getToken(AnyFXParser::OR, i);
}


size_t AnyFXParser::OrExpressionContext::getRuleIndex() const {
  return AnyFXParser::RuleOrExpression;
}


AnyFXParser::OrExpressionContext* AnyFXParser::orExpression() {
  OrExpressionContext *_localctx = _tracker.createInstance<OrExpressionContext>(_ctx, getState());
  enterRule(_localctx, 70, AnyFXParser::RuleOrExpression);

          dynamic_cast<OrExpressionContext *>(_localctx)->tree =  nullptr;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(613);
    dynamic_cast<OrExpressionContext *>(_localctx)->e1 = xorExpression();
     dynamic_cast<OrExpressionContext *>(_localctx)->tree =  dynamic_cast<OrExpressionContext *>(_localctx)->e1->tree; 
    setState(621);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == AnyFXParser::OR) {
      setState(615);
      match(AnyFXParser::OR);
      setState(616);
      dynamic_cast<OrExpressionContext *>(_localctx)->e2 = xorExpression();

                  BinaryExpression* expr = new BinaryExpression('|', _localctx->tree, dynamic_cast<OrExpressionContext *>(_localctx)->e2->tree);
                  expr->location = SetupFile();
                  dynamic_cast<OrExpressionContext *>(_localctx)->tree =  expr;
              
      setState(623);
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

AnyFXParser::XorExpressionContext::XorExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<AnyFXParser::AndExpressionContext *> AnyFXParser::XorExpressionContext::andExpression() {
  return getRuleContexts<AnyFXParser::AndExpressionContext>();
}

AnyFXParser::AndExpressionContext* AnyFXParser::XorExpressionContext::andExpression(size_t i) {
  return getRuleContext<AnyFXParser::AndExpressionContext>(i);
}

std::vector<tree::TerminalNode *> AnyFXParser::XorExpressionContext::XOR() {
  return getTokens(AnyFXParser::XOR);
}

tree::TerminalNode* AnyFXParser::XorExpressionContext::XOR(size_t i) {
  return getToken(AnyFXParser::XOR, i);
}


size_t AnyFXParser::XorExpressionContext::getRuleIndex() const {
  return AnyFXParser::RuleXorExpression;
}


AnyFXParser::XorExpressionContext* AnyFXParser::xorExpression() {
  XorExpressionContext *_localctx = _tracker.createInstance<XorExpressionContext>(_ctx, getState());
  enterRule(_localctx, 72, AnyFXParser::RuleXorExpression);

          dynamic_cast<XorExpressionContext *>(_localctx)->tree =  nullptr;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(624);
    dynamic_cast<XorExpressionContext *>(_localctx)->e1 = andExpression();
     dynamic_cast<XorExpressionContext *>(_localctx)->tree =  dynamic_cast<XorExpressionContext *>(_localctx)->e1->tree; 
    setState(632);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == AnyFXParser::XOR) {
      setState(626);
      match(AnyFXParser::XOR);
      setState(627);
      dynamic_cast<XorExpressionContext *>(_localctx)->e2 = andExpression();

                  BinaryExpression* expr = new BinaryExpression('^', _localctx->tree, dynamic_cast<XorExpressionContext *>(_localctx)->e2->tree);
                  expr->location = SetupFile();
                  dynamic_cast<XorExpressionContext *>(_localctx)->tree =  expr;
              
      setState(634);
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

AnyFXParser::AndExpressionContext::AndExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<AnyFXParser::EquivalencyExpressionContext *> AnyFXParser::AndExpressionContext::equivalencyExpression() {
  return getRuleContexts<AnyFXParser::EquivalencyExpressionContext>();
}

AnyFXParser::EquivalencyExpressionContext* AnyFXParser::AndExpressionContext::equivalencyExpression(size_t i) {
  return getRuleContext<AnyFXParser::EquivalencyExpressionContext>(i);
}

std::vector<tree::TerminalNode *> AnyFXParser::AndExpressionContext::AND() {
  return getTokens(AnyFXParser::AND);
}

tree::TerminalNode* AnyFXParser::AndExpressionContext::AND(size_t i) {
  return getToken(AnyFXParser::AND, i);
}


size_t AnyFXParser::AndExpressionContext::getRuleIndex() const {
  return AnyFXParser::RuleAndExpression;
}


AnyFXParser::AndExpressionContext* AnyFXParser::andExpression() {
  AndExpressionContext *_localctx = _tracker.createInstance<AndExpressionContext>(_ctx, getState());
  enterRule(_localctx, 74, AnyFXParser::RuleAndExpression);

          dynamic_cast<AndExpressionContext *>(_localctx)->tree =  nullptr;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(635);
    dynamic_cast<AndExpressionContext *>(_localctx)->e1 = equivalencyExpression();
     dynamic_cast<AndExpressionContext *>(_localctx)->tree =  dynamic_cast<AndExpressionContext *>(_localctx)->e1->tree;	
    setState(643);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == AnyFXParser::AND) {
      setState(637);
      match(AnyFXParser::AND);
      setState(638);
      dynamic_cast<AndExpressionContext *>(_localctx)->e2 = equivalencyExpression();

                  BinaryExpression* expr = new BinaryExpression('&', _localctx->tree, dynamic_cast<AndExpressionContext *>(_localctx)->e2->tree);
                  expr->location = SetupFile();
                  dynamic_cast<AndExpressionContext *>(_localctx)->tree =  expr;
              
      setState(645);
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

AnyFXParser::EquivalencyExpressionContext::EquivalencyExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<AnyFXParser::RelationalExpressionContext *> AnyFXParser::EquivalencyExpressionContext::relationalExpression() {
  return getRuleContexts<AnyFXParser::RelationalExpressionContext>();
}

AnyFXParser::RelationalExpressionContext* AnyFXParser::EquivalencyExpressionContext::relationalExpression(size_t i) {
  return getRuleContext<AnyFXParser::RelationalExpressionContext>(i);
}

std::vector<tree::TerminalNode *> AnyFXParser::EquivalencyExpressionContext::LOGICEQ() {
  return getTokens(AnyFXParser::LOGICEQ);
}

tree::TerminalNode* AnyFXParser::EquivalencyExpressionContext::LOGICEQ(size_t i) {
  return getToken(AnyFXParser::LOGICEQ, i);
}

std::vector<tree::TerminalNode *> AnyFXParser::EquivalencyExpressionContext::NOTEQ() {
  return getTokens(AnyFXParser::NOTEQ);
}

tree::TerminalNode* AnyFXParser::EquivalencyExpressionContext::NOTEQ(size_t i) {
  return getToken(AnyFXParser::NOTEQ, i);
}


size_t AnyFXParser::EquivalencyExpressionContext::getRuleIndex() const {
  return AnyFXParser::RuleEquivalencyExpression;
}


AnyFXParser::EquivalencyExpressionContext* AnyFXParser::equivalencyExpression() {
  EquivalencyExpressionContext *_localctx = _tracker.createInstance<EquivalencyExpressionContext>(_ctx, getState());
  enterRule(_localctx, 76, AnyFXParser::RuleEquivalencyExpression);

          dynamic_cast<EquivalencyExpressionContext *>(_localctx)->tree =  nullptr;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(646);
    dynamic_cast<EquivalencyExpressionContext *>(_localctx)->e1 = relationalExpression();
     dynamic_cast<EquivalencyExpressionContext *>(_localctx)->tree =  dynamic_cast<EquivalencyExpressionContext *>(_localctx)->e1->tree; 
    setState(654);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == AnyFXParser::LOGICEQ

    || _la == AnyFXParser::NOTEQ) {
      setState(648);
      dynamic_cast<EquivalencyExpressionContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!(_la == AnyFXParser::LOGICEQ

      || _la == AnyFXParser::NOTEQ)) {
        dynamic_cast<EquivalencyExpressionContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(649);
      dynamic_cast<EquivalencyExpressionContext *>(_localctx)->e2 = relationalExpression();

                  BinaryExpression* expr = new BinaryExpression(StringToFourCC((dynamic_cast<EquivalencyExpressionContext *>(_localctx)->op != nullptr ? dynamic_cast<EquivalencyExpressionContext *>(_localctx)->op->getText() : "")), _localctx->tree, dynamic_cast<EquivalencyExpressionContext *>(_localctx)->e2->tree);
                  expr->location = SetupFile();
                  dynamic_cast<EquivalencyExpressionContext *>(_localctx)->tree =  expr;
              
      setState(656);
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

AnyFXParser::RelationalExpressionContext::RelationalExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<AnyFXParser::ShiftExpressionContext *> AnyFXParser::RelationalExpressionContext::shiftExpression() {
  return getRuleContexts<AnyFXParser::ShiftExpressionContext>();
}

AnyFXParser::ShiftExpressionContext* AnyFXParser::RelationalExpressionContext::shiftExpression(size_t i) {
  return getRuleContext<AnyFXParser::ShiftExpressionContext>(i);
}

std::vector<tree::TerminalNode *> AnyFXParser::RelationalExpressionContext::LESS() {
  return getTokens(AnyFXParser::LESS);
}

tree::TerminalNode* AnyFXParser::RelationalExpressionContext::LESS(size_t i) {
  return getToken(AnyFXParser::LESS, i);
}

std::vector<tree::TerminalNode *> AnyFXParser::RelationalExpressionContext::GREATER() {
  return getTokens(AnyFXParser::GREATER);
}

tree::TerminalNode* AnyFXParser::RelationalExpressionContext::GREATER(size_t i) {
  return getToken(AnyFXParser::GREATER, i);
}

std::vector<tree::TerminalNode *> AnyFXParser::RelationalExpressionContext::LESSEQ() {
  return getTokens(AnyFXParser::LESSEQ);
}

tree::TerminalNode* AnyFXParser::RelationalExpressionContext::LESSEQ(size_t i) {
  return getToken(AnyFXParser::LESSEQ, i);
}

std::vector<tree::TerminalNode *> AnyFXParser::RelationalExpressionContext::GREATEREQ() {
  return getTokens(AnyFXParser::GREATEREQ);
}

tree::TerminalNode* AnyFXParser::RelationalExpressionContext::GREATEREQ(size_t i) {
  return getToken(AnyFXParser::GREATEREQ, i);
}


size_t AnyFXParser::RelationalExpressionContext::getRuleIndex() const {
  return AnyFXParser::RuleRelationalExpression;
}


AnyFXParser::RelationalExpressionContext* AnyFXParser::relationalExpression() {
  RelationalExpressionContext *_localctx = _tracker.createInstance<RelationalExpressionContext>(_ctx, getState());
  enterRule(_localctx, 78, AnyFXParser::RuleRelationalExpression);

          dynamic_cast<RelationalExpressionContext *>(_localctx)->tree =  nullptr;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(657);
    dynamic_cast<RelationalExpressionContext *>(_localctx)->e1 = shiftExpression();
     dynamic_cast<RelationalExpressionContext *>(_localctx)->tree =  dynamic_cast<RelationalExpressionContext *>(_localctx)->e1->tree; 
    setState(665);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << AnyFXParser::LESS)
      | (1ULL << AnyFXParser::LESSEQ)
      | (1ULL << AnyFXParser::GREATER)
      | (1ULL << AnyFXParser::GREATEREQ))) != 0)) {
      setState(659);
      dynamic_cast<RelationalExpressionContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << AnyFXParser::LESS)
        | (1ULL << AnyFXParser::LESSEQ)
        | (1ULL << AnyFXParser::GREATER)
        | (1ULL << AnyFXParser::GREATEREQ))) != 0))) {
        dynamic_cast<RelationalExpressionContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(660);
      dynamic_cast<RelationalExpressionContext *>(_localctx)->e2 = shiftExpression();

                  BinaryExpression* expr = new BinaryExpression(StringToFourCC((dynamic_cast<RelationalExpressionContext *>(_localctx)->op != nullptr ? dynamic_cast<RelationalExpressionContext *>(_localctx)->op->getText() : "")), _localctx->tree, dynamic_cast<RelationalExpressionContext *>(_localctx)->e2->tree);
                  expr->location = SetupFile();
                  dynamic_cast<RelationalExpressionContext *>(_localctx)->tree =  expr;
              
      setState(667);
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

AnyFXParser::ShiftExpressionContext::ShiftExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<AnyFXParser::AddSubtractExpressionContext *> AnyFXParser::ShiftExpressionContext::addSubtractExpression() {
  return getRuleContexts<AnyFXParser::AddSubtractExpressionContext>();
}

AnyFXParser::AddSubtractExpressionContext* AnyFXParser::ShiftExpressionContext::addSubtractExpression(size_t i) {
  return getRuleContext<AnyFXParser::AddSubtractExpressionContext>(i);
}


size_t AnyFXParser::ShiftExpressionContext::getRuleIndex() const {
  return AnyFXParser::RuleShiftExpression;
}


AnyFXParser::ShiftExpressionContext* AnyFXParser::shiftExpression() {
  ShiftExpressionContext *_localctx = _tracker.createInstance<ShiftExpressionContext>(_ctx, getState());
  enterRule(_localctx, 80, AnyFXParser::RuleShiftExpression);

          dynamic_cast<ShiftExpressionContext *>(_localctx)->tree =  nullptr;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(668);
    dynamic_cast<ShiftExpressionContext *>(_localctx)->e1 = addSubtractExpression();
     dynamic_cast<ShiftExpressionContext *>(_localctx)->tree =  dynamic_cast<ShiftExpressionContext *>(_localctx)->e1->tree; 
    setState(676);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == AnyFXParser::T__29

    || _la == AnyFXParser::T__30) {
      setState(670);
      dynamic_cast<ShiftExpressionContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!(_la == AnyFXParser::T__29

      || _la == AnyFXParser::T__30)) {
        dynamic_cast<ShiftExpressionContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(671);
      dynamic_cast<ShiftExpressionContext *>(_localctx)->e2 = addSubtractExpression();

                  BinaryExpression* expr = new BinaryExpression(StringToFourCC((dynamic_cast<ShiftExpressionContext *>(_localctx)->op != nullptr ? dynamic_cast<ShiftExpressionContext *>(_localctx)->op->getText() : "")), _localctx->tree, dynamic_cast<ShiftExpressionContext *>(_localctx)->e2->tree);
                  expr->location = SetupFile();
                  dynamic_cast<ShiftExpressionContext *>(_localctx)->tree =  expr;
              
      setState(678);
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

AnyFXParser::AddSubtractExpressionContext::AddSubtractExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<AnyFXParser::MultiplyDivideExpressionContext *> AnyFXParser::AddSubtractExpressionContext::multiplyDivideExpression() {
  return getRuleContexts<AnyFXParser::MultiplyDivideExpressionContext>();
}

AnyFXParser::MultiplyDivideExpressionContext* AnyFXParser::AddSubtractExpressionContext::multiplyDivideExpression(size_t i) {
  return getRuleContext<AnyFXParser::MultiplyDivideExpressionContext>(i);
}

std::vector<tree::TerminalNode *> AnyFXParser::AddSubtractExpressionContext::ADD_OP() {
  return getTokens(AnyFXParser::ADD_OP);
}

tree::TerminalNode* AnyFXParser::AddSubtractExpressionContext::ADD_OP(size_t i) {
  return getToken(AnyFXParser::ADD_OP, i);
}

std::vector<tree::TerminalNode *> AnyFXParser::AddSubtractExpressionContext::SUB_OP() {
  return getTokens(AnyFXParser::SUB_OP);
}

tree::TerminalNode* AnyFXParser::AddSubtractExpressionContext::SUB_OP(size_t i) {
  return getToken(AnyFXParser::SUB_OP, i);
}


size_t AnyFXParser::AddSubtractExpressionContext::getRuleIndex() const {
  return AnyFXParser::RuleAddSubtractExpression;
}


AnyFXParser::AddSubtractExpressionContext* AnyFXParser::addSubtractExpression() {
  AddSubtractExpressionContext *_localctx = _tracker.createInstance<AddSubtractExpressionContext>(_ctx, getState());
  enterRule(_localctx, 82, AnyFXParser::RuleAddSubtractExpression);

          dynamic_cast<AddSubtractExpressionContext *>(_localctx)->tree =  nullptr;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(679);
    dynamic_cast<AddSubtractExpressionContext *>(_localctx)->e1 = multiplyDivideExpression();
     dynamic_cast<AddSubtractExpressionContext *>(_localctx)->tree =  dynamic_cast<AddSubtractExpressionContext *>(_localctx)->e1->tree; 
    setState(687);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == AnyFXParser::ADD_OP

    || _la == AnyFXParser::SUB_OP) {
      setState(681);
      dynamic_cast<AddSubtractExpressionContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!(_la == AnyFXParser::ADD_OP

      || _la == AnyFXParser::SUB_OP)) {
        dynamic_cast<AddSubtractExpressionContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(682);
      dynamic_cast<AddSubtractExpressionContext *>(_localctx)->e2 = multiplyDivideExpression();

                  BinaryExpression* expr = new BinaryExpression(StringToFourCC((dynamic_cast<AddSubtractExpressionContext *>(_localctx)->op != nullptr ? dynamic_cast<AddSubtractExpressionContext *>(_localctx)->op->getText() : "")), _localctx->tree, dynamic_cast<AddSubtractExpressionContext *>(_localctx)->e2->tree);
                  expr->location = SetupFile();
                  dynamic_cast<AddSubtractExpressionContext *>(_localctx)->tree =  expr;
              
      setState(689);
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

AnyFXParser::MultiplyDivideExpressionContext::MultiplyDivideExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<AnyFXParser::PrefixExpressionContext *> AnyFXParser::MultiplyDivideExpressionContext::prefixExpression() {
  return getRuleContexts<AnyFXParser::PrefixExpressionContext>();
}

AnyFXParser::PrefixExpressionContext* AnyFXParser::MultiplyDivideExpressionContext::prefixExpression(size_t i) {
  return getRuleContext<AnyFXParser::PrefixExpressionContext>(i);
}

std::vector<tree::TerminalNode *> AnyFXParser::MultiplyDivideExpressionContext::MUL_OP() {
  return getTokens(AnyFXParser::MUL_OP);
}

tree::TerminalNode* AnyFXParser::MultiplyDivideExpressionContext::MUL_OP(size_t i) {
  return getToken(AnyFXParser::MUL_OP, i);
}

std::vector<tree::TerminalNode *> AnyFXParser::MultiplyDivideExpressionContext::DIV_OP() {
  return getTokens(AnyFXParser::DIV_OP);
}

tree::TerminalNode* AnyFXParser::MultiplyDivideExpressionContext::DIV_OP(size_t i) {
  return getToken(AnyFXParser::DIV_OP, i);
}

std::vector<tree::TerminalNode *> AnyFXParser::MultiplyDivideExpressionContext::MOD() {
  return getTokens(AnyFXParser::MOD);
}

tree::TerminalNode* AnyFXParser::MultiplyDivideExpressionContext::MOD(size_t i) {
  return getToken(AnyFXParser::MOD, i);
}


size_t AnyFXParser::MultiplyDivideExpressionContext::getRuleIndex() const {
  return AnyFXParser::RuleMultiplyDivideExpression;
}


AnyFXParser::MultiplyDivideExpressionContext* AnyFXParser::multiplyDivideExpression() {
  MultiplyDivideExpressionContext *_localctx = _tracker.createInstance<MultiplyDivideExpressionContext>(_ctx, getState());
  enterRule(_localctx, 84, AnyFXParser::RuleMultiplyDivideExpression);

          dynamic_cast<MultiplyDivideExpressionContext *>(_localctx)->tree =  nullptr;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(690);
    dynamic_cast<MultiplyDivideExpressionContext *>(_localctx)->e1 = prefixExpression();
     dynamic_cast<MultiplyDivideExpressionContext *>(_localctx)->tree =  dynamic_cast<MultiplyDivideExpressionContext *>(_localctx)->e1->tree; 
    setState(698);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (((((_la - 67) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 67)) & ((1ULL << (AnyFXParser::MOD - 67))
      | (1ULL << (AnyFXParser::DIV_OP - 67))
      | (1ULL << (AnyFXParser::MUL_OP - 67)))) != 0)) {
      setState(692);
      dynamic_cast<MultiplyDivideExpressionContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!(((((_la - 67) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 67)) & ((1ULL << (AnyFXParser::MOD - 67))
        | (1ULL << (AnyFXParser::DIV_OP - 67))
        | (1ULL << (AnyFXParser::MUL_OP - 67)))) != 0))) {
        dynamic_cast<MultiplyDivideExpressionContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(693);
      dynamic_cast<MultiplyDivideExpressionContext *>(_localctx)->e2 = prefixExpression();

                  BinaryExpression* expr = new BinaryExpression(StringToFourCC((dynamic_cast<MultiplyDivideExpressionContext *>(_localctx)->op != nullptr ? dynamic_cast<MultiplyDivideExpressionContext *>(_localctx)->op->getText() : "")), _localctx->tree, dynamic_cast<MultiplyDivideExpressionContext *>(_localctx)->e2->tree);
                  expr->location = SetupFile();
                  dynamic_cast<MultiplyDivideExpressionContext *>(_localctx)->tree =  expr;
              
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

//----------------- PrefixExpressionContext ------------------------------------------------------------------

AnyFXParser::PrefixExpressionContext::PrefixExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

AnyFXParser::SuffixExpressionContext* AnyFXParser::PrefixExpressionContext::suffixExpression() {
  return getRuleContext<AnyFXParser::SuffixExpressionContext>(0);
}

std::vector<tree::TerminalNode *> AnyFXParser::PrefixExpressionContext::SUB_OP() {
  return getTokens(AnyFXParser::SUB_OP);
}

tree::TerminalNode* AnyFXParser::PrefixExpressionContext::SUB_OP(size_t i) {
  return getToken(AnyFXParser::SUB_OP, i);
}

std::vector<tree::TerminalNode *> AnyFXParser::PrefixExpressionContext::ADD_OP() {
  return getTokens(AnyFXParser::ADD_OP);
}

tree::TerminalNode* AnyFXParser::PrefixExpressionContext::ADD_OP(size_t i) {
  return getToken(AnyFXParser::ADD_OP, i);
}

std::vector<tree::TerminalNode *> AnyFXParser::PrefixExpressionContext::NOT() {
  return getTokens(AnyFXParser::NOT);
}

tree::TerminalNode* AnyFXParser::PrefixExpressionContext::NOT(size_t i) {
  return getToken(AnyFXParser::NOT, i);
}

std::vector<tree::TerminalNode *> AnyFXParser::PrefixExpressionContext::CONNJUGATE() {
  return getTokens(AnyFXParser::CONNJUGATE);
}

tree::TerminalNode* AnyFXParser::PrefixExpressionContext::CONNJUGATE(size_t i) {
  return getToken(AnyFXParser::CONNJUGATE, i);
}

std::vector<tree::TerminalNode *> AnyFXParser::PrefixExpressionContext::MUL_OP() {
  return getTokens(AnyFXParser::MUL_OP);
}

tree::TerminalNode* AnyFXParser::PrefixExpressionContext::MUL_OP(size_t i) {
  return getToken(AnyFXParser::MUL_OP, i);
}


size_t AnyFXParser::PrefixExpressionContext::getRuleIndex() const {
  return AnyFXParser::RulePrefixExpression;
}


AnyFXParser::PrefixExpressionContext* AnyFXParser::prefixExpression() {
  PrefixExpressionContext *_localctx = _tracker.createInstance<PrefixExpressionContext>(_ctx, getState());
  enterRule(_localctx, 86, AnyFXParser::RulePrefixExpression);

          dynamic_cast<PrefixExpressionContext *>(_localctx)->tree =  nullptr;
          std::vector<uint32_t> ops;
          std::vector<Symbol::Location> locations;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(705);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (((((_la - 32) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 32)) & ((1ULL << (AnyFXParser::T__31 - 32))
      | (1ULL << (AnyFXParser::T__32 - 32))
      | (1ULL << (AnyFXParser::NOT - 32))
      | (1ULL << (AnyFXParser::CONNJUGATE - 32))
      | (1ULL << (AnyFXParser::ADD_OP - 32))
      | (1ULL << (AnyFXParser::SUB_OP - 32))
      | (1ULL << (AnyFXParser::MUL_OP - 32)))) != 0)) {
      setState(701);
      dynamic_cast<PrefixExpressionContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!(((((_la - 32) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 32)) & ((1ULL << (AnyFXParser::T__31 - 32))
        | (1ULL << (AnyFXParser::T__32 - 32))
        | (1ULL << (AnyFXParser::NOT - 32))
        | (1ULL << (AnyFXParser::CONNJUGATE - 32))
        | (1ULL << (AnyFXParser::ADD_OP - 32))
        | (1ULL << (AnyFXParser::SUB_OP - 32))
        | (1ULL << (AnyFXParser::MUL_OP - 32)))) != 0))) {
        dynamic_cast<PrefixExpressionContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
       ops.push_back(StringToFourCC((dynamic_cast<PrefixExpressionContext *>(_localctx)->op != nullptr ? dynamic_cast<PrefixExpressionContext *>(_localctx)->op->getText() : ""))); locations.push_back(SetupFile()); 
      setState(707);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(708);
    dynamic_cast<PrefixExpressionContext *>(_localctx)->e1 = suffixExpression();

            dynamic_cast<PrefixExpressionContext *>(_localctx)->tree =  dynamic_cast<PrefixExpressionContext *>(_localctx)->e1->tree;
            _localctx->tree->location = SetupFile();
            for (size_t i = 0; i < ops.size(); i++)
            {
                dynamic_cast<PrefixExpressionContext *>(_localctx)->tree =  new UnaryExpression(ops[i], 0x0, _localctx->tree);
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

AnyFXParser::SuffixExpressionContext::SuffixExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

AnyFXParser::BinaryexpatomContext* AnyFXParser::SuffixExpressionContext::binaryexpatom() {
  return getRuleContext<AnyFXParser::BinaryexpatomContext>(0);
}

std::vector<tree::TerminalNode *> AnyFXParser::SuffixExpressionContext::LP() {
  return getTokens(AnyFXParser::LP);
}

tree::TerminalNode* AnyFXParser::SuffixExpressionContext::LP(size_t i) {
  return getToken(AnyFXParser::LP, i);
}

std::vector<tree::TerminalNode *> AnyFXParser::SuffixExpressionContext::RP() {
  return getTokens(AnyFXParser::RP);
}

tree::TerminalNode* AnyFXParser::SuffixExpressionContext::RP(size_t i) {
  return getToken(AnyFXParser::RP, i);
}

std::vector<tree::TerminalNode *> AnyFXParser::SuffixExpressionContext::DOT() {
  return getTokens(AnyFXParser::DOT);
}

tree::TerminalNode* AnyFXParser::SuffixExpressionContext::DOT(size_t i) {
  return getToken(AnyFXParser::DOT, i);
}

std::vector<tree::TerminalNode *> AnyFXParser::SuffixExpressionContext::ARROW() {
  return getTokens(AnyFXParser::ARROW);
}

tree::TerminalNode* AnyFXParser::SuffixExpressionContext::ARROW(size_t i) {
  return getToken(AnyFXParser::ARROW, i);
}

std::vector<tree::TerminalNode *> AnyFXParser::SuffixExpressionContext::LL() {
  return getTokens(AnyFXParser::LL);
}

tree::TerminalNode* AnyFXParser::SuffixExpressionContext::LL(size_t i) {
  return getToken(AnyFXParser::LL, i);
}

std::vector<tree::TerminalNode *> AnyFXParser::SuffixExpressionContext::RR() {
  return getTokens(AnyFXParser::RR);
}

tree::TerminalNode* AnyFXParser::SuffixExpressionContext::RR(size_t i) {
  return getToken(AnyFXParser::RR, i);
}

std::vector<AnyFXParser::SuffixExpressionContext *> AnyFXParser::SuffixExpressionContext::suffixExpression() {
  return getRuleContexts<AnyFXParser::SuffixExpressionContext>();
}

AnyFXParser::SuffixExpressionContext* AnyFXParser::SuffixExpressionContext::suffixExpression(size_t i) {
  return getRuleContext<AnyFXParser::SuffixExpressionContext>(i);
}

std::vector<AnyFXParser::LogicalOrExpressionContext *> AnyFXParser::SuffixExpressionContext::logicalOrExpression() {
  return getRuleContexts<AnyFXParser::LogicalOrExpressionContext>();
}

AnyFXParser::LogicalOrExpressionContext* AnyFXParser::SuffixExpressionContext::logicalOrExpression(size_t i) {
  return getRuleContext<AnyFXParser::LogicalOrExpressionContext>(i);
}

std::vector<AnyFXParser::ExpressionContext *> AnyFXParser::SuffixExpressionContext::expression() {
  return getRuleContexts<AnyFXParser::ExpressionContext>();
}

AnyFXParser::ExpressionContext* AnyFXParser::SuffixExpressionContext::expression(size_t i) {
  return getRuleContext<AnyFXParser::ExpressionContext>(i);
}

std::vector<tree::TerminalNode *> AnyFXParser::SuffixExpressionContext::CO() {
  return getTokens(AnyFXParser::CO);
}

tree::TerminalNode* AnyFXParser::SuffixExpressionContext::CO(size_t i) {
  return getToken(AnyFXParser::CO, i);
}


size_t AnyFXParser::SuffixExpressionContext::getRuleIndex() const {
  return AnyFXParser::RuleSuffixExpression;
}


AnyFXParser::SuffixExpressionContext* AnyFXParser::suffixExpression() {
  SuffixExpressionContext *_localctx = _tracker.createInstance<SuffixExpressionContext>(_ctx, getState());
  enterRule(_localctx, 88, AnyFXParser::RuleSuffixExpression);

          dynamic_cast<SuffixExpressionContext *>(_localctx)->tree =  nullptr;

          Symbol::Location location;
          std::vector<Expression*> args;
          Expression* arrayIndexExpr = nullptr;

          std::vector<uint32_t> ops;
          std::vector<Symbol::Location> locations;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    setState(764);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 57, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(711);
      dynamic_cast<SuffixExpressionContext *>(_localctx)->e1 = binaryexpatom();
      setState(716);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == AnyFXParser::T__31

      || _la == AnyFXParser::T__32) {
        setState(712);
        dynamic_cast<SuffixExpressionContext *>(_localctx)->op = _input->LT(1);
        _la = _input->LA(1);
        if (!(_la == AnyFXParser::T__31

        || _la == AnyFXParser::T__32)) {
          dynamic_cast<SuffixExpressionContext *>(_localctx)->op = _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
         ops.push_back(StringToFourCC((dynamic_cast<SuffixExpressionContext *>(_localctx)->op != nullptr ? dynamic_cast<SuffixExpressionContext *>(_localctx)->op->getText() : ""))); locations.push_back(SetupFile()); 
        setState(718);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }

              dynamic_cast<SuffixExpressionContext *>(_localctx)->tree =  dynamic_cast<SuffixExpressionContext *>(_localctx)->e1->tree;
              _localctx->tree->location = SetupFile();
              for (size_t i = 0; i < ops.size(); i++)
              {
                  dynamic_cast<SuffixExpressionContext *>(_localctx)->tree =  new UnaryExpression(0x0, ops[i], _localctx->tree);
                  _localctx->tree->location = locations[i];
              }
          
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(721);
      dynamic_cast<SuffixExpressionContext *>(_localctx)->e1 = binaryexpatom();

              dynamic_cast<SuffixExpressionContext *>(_localctx)->tree =  dynamic_cast<SuffixExpressionContext *>(_localctx)->e1->tree;
          
      setState(761);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 56, _ctx);
      while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
        if (alt == 1) {
          setState(759);
          _errHandler->sync(this);
          switch (_input->LA(1)) {
            case AnyFXParser::LP: {
              setState(723);
              match(AnyFXParser::LP);
               location = SetupFile(); 
              setState(736);
              _errHandler->sync(this);

              _la = _input->LA(1);
              if ((((_la & ~ 0x3fULL) == 0) &&
                ((1ULL << _la) & ((1ULL << AnyFXParser::T__0)
                | (1ULL << AnyFXParser::T__1)
                | (1ULL << AnyFXParser::T__31)
                | (1ULL << AnyFXParser::T__32)
                | (1ULL << AnyFXParser::LP)
                | (1ULL << AnyFXParser::LB)
                | (1ULL << AnyFXParser::NOT)
                | (1ULL << AnyFXParser::QO)
                | (1ULL << AnyFXParser::CONNJUGATE)
                | (1ULL << AnyFXParser::Q))) != 0) || ((((_la - 70) & ~ 0x3fULL) == 0) &&
                ((1ULL << (_la - 70)) & ((1ULL << (AnyFXParser::ADD_OP - 70))
                | (1ULL << (AnyFXParser::SUB_OP - 70))
                | (1ULL << (AnyFXParser::MUL_OP - 70))
                | (1ULL << (AnyFXParser::INTEGERLITERAL - 70))
                | (1ULL << (AnyFXParser::UINTEGERLITERAL - 70))
                | (1ULL << (AnyFXParser::FLOATLITERAL - 70))
                | (1ULL << (AnyFXParser::DOUBLELITERAL - 70))
                | (1ULL << (AnyFXParser::HEX - 70))
                | (1ULL << (AnyFXParser::IDENTIFIER - 70)))) != 0)) {
                setState(725);
                dynamic_cast<SuffixExpressionContext *>(_localctx)->arg0 = logicalOrExpression();
                 args.push_back(dynamic_cast<SuffixExpressionContext *>(_localctx)->arg0->tree); 
                setState(733);
                _errHandler->sync(this);
                _la = _input->LA(1);
                while (_la == AnyFXParser::CO) {
                  setState(727);
                  match(AnyFXParser::CO);
                  setState(728);
                  dynamic_cast<SuffixExpressionContext *>(_localctx)->argn = logicalOrExpression();
                   args.push_back(dynamic_cast<SuffixExpressionContext *>(_localctx)->argn->tree); 
                  setState(735);
                  _errHandler->sync(this);
                  _la = _input->LA(1);
                }
              }
              setState(738);
              match(AnyFXParser::RP);

                          CallExpression* expr = new CallExpression(_localctx->tree, args);
                          expr->location = location;
                          dynamic_cast<SuffixExpressionContext *>(_localctx)->tree =  expr;
                      
              break;
            }

            case AnyFXParser::DOT: {
              setState(740);
              match(AnyFXParser::DOT);
               location = SetupFile(); 
              setState(742);
              dynamic_cast<SuffixExpressionContext *>(_localctx)->e2 = suffixExpression();

                          AccessExpression* expr = new AccessExpression(_localctx->tree, dynamic_cast<SuffixExpressionContext *>(_localctx)->e2->tree, false);
                          expr->location = location;
                          dynamic_cast<SuffixExpressionContext *>(_localctx)->tree =  expr;
                      
              break;
            }

            case AnyFXParser::ARROW: {
              setState(745);
              match(AnyFXParser::ARROW);
               location = SetupFile(); 
              setState(747);
              dynamic_cast<SuffixExpressionContext *>(_localctx)->e2 = suffixExpression();

                          AccessExpression* expr = new AccessExpression(_localctx->tree, dynamic_cast<SuffixExpressionContext *>(_localctx)->e2->tree, true);
                          expr->location = location;
                          dynamic_cast<SuffixExpressionContext *>(_localctx)->tree =  expr;
                      
              break;
            }

            case AnyFXParser::LL: {
              setState(750);
              match(AnyFXParser::LL);
               location = SetupFile(); 
              setState(755);
              _errHandler->sync(this);

              _la = _input->LA(1);
              if ((((_la & ~ 0x3fULL) == 0) &&
                ((1ULL << _la) & ((1ULL << AnyFXParser::T__0)
                | (1ULL << AnyFXParser::T__1)
                | (1ULL << AnyFXParser::T__31)
                | (1ULL << AnyFXParser::T__32)
                | (1ULL << AnyFXParser::LP)
                | (1ULL << AnyFXParser::LB)
                | (1ULL << AnyFXParser::NOT)
                | (1ULL << AnyFXParser::QO)
                | (1ULL << AnyFXParser::CONNJUGATE)
                | (1ULL << AnyFXParser::Q))) != 0) || ((((_la - 70) & ~ 0x3fULL) == 0) &&
                ((1ULL << (_la - 70)) & ((1ULL << (AnyFXParser::ADD_OP - 70))
                | (1ULL << (AnyFXParser::SUB_OP - 70))
                | (1ULL << (AnyFXParser::MUL_OP - 70))
                | (1ULL << (AnyFXParser::INTEGERLITERAL - 70))
                | (1ULL << (AnyFXParser::UINTEGERLITERAL - 70))
                | (1ULL << (AnyFXParser::FLOATLITERAL - 70))
                | (1ULL << (AnyFXParser::DOUBLELITERAL - 70))
                | (1ULL << (AnyFXParser::HEX - 70))
                | (1ULL << (AnyFXParser::IDENTIFIER - 70)))) != 0)) {
                setState(752);
                dynamic_cast<SuffixExpressionContext *>(_localctx)->e3 = expression();
                 arrayIndexExpr = dynamic_cast<SuffixExpressionContext *>(_localctx)->e3->tree; 
              }
              setState(757);
              match(AnyFXParser::RR);

                          ArrayIndexExpression* expr = new ArrayIndexExpression(_localctx->tree, arrayIndexExpr);
                          expr->location = location;
                          dynamic_cast<SuffixExpressionContext *>(_localctx)->tree =  expr;
                      
              break;
            }

          default:
            throw NoViableAltException(this);
          } 
        }
        setState(763);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 56, _ctx);
      }
      break;
    }

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

AnyFXParser::NamespaceExpressionContext::NamespaceExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<AnyFXParser::BinaryexpatomContext *> AnyFXParser::NamespaceExpressionContext::binaryexpatom() {
  return getRuleContexts<AnyFXParser::BinaryexpatomContext>();
}

AnyFXParser::BinaryexpatomContext* AnyFXParser::NamespaceExpressionContext::binaryexpatom(size_t i) {
  return getRuleContext<AnyFXParser::BinaryexpatomContext>(i);
}


size_t AnyFXParser::NamespaceExpressionContext::getRuleIndex() const {
  return AnyFXParser::RuleNamespaceExpression;
}


AnyFXParser::NamespaceExpressionContext* AnyFXParser::namespaceExpression() {
  NamespaceExpressionContext *_localctx = _tracker.createInstance<NamespaceExpressionContext>(_ctx, getState());
  enterRule(_localctx, 90, AnyFXParser::RuleNamespaceExpression);

          dynamic_cast<NamespaceExpressionContext *>(_localctx)->tree =  nullptr;
      

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(766);
    dynamic_cast<NamespaceExpressionContext *>(_localctx)->e1 = binaryexpatom();
    setState(767);
    match(AnyFXParser::T__33);
    setState(768);
    dynamic_cast<NamespaceExpressionContext *>(_localctx)->e2 = binaryexpatom();


        
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BinaryexpatomContext ------------------------------------------------------------------

AnyFXParser::BinaryexpatomContext::BinaryexpatomContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* AnyFXParser::BinaryexpatomContext::INTEGERLITERAL() {
  return getToken(AnyFXParser::INTEGERLITERAL, 0);
}

tree::TerminalNode* AnyFXParser::BinaryexpatomContext::UINTEGERLITERAL() {
  return getToken(AnyFXParser::UINTEGERLITERAL, 0);
}

tree::TerminalNode* AnyFXParser::BinaryexpatomContext::FLOATLITERAL() {
  return getToken(AnyFXParser::FLOATLITERAL, 0);
}

tree::TerminalNode* AnyFXParser::BinaryexpatomContext::DOUBLELITERAL() {
  return getToken(AnyFXParser::DOUBLELITERAL, 0);
}

tree::TerminalNode* AnyFXParser::BinaryexpatomContext::HEX() {
  return getToken(AnyFXParser::HEX, 0);
}

AnyFXParser::StringContext* AnyFXParser::BinaryexpatomContext::string() {
  return getRuleContext<AnyFXParser::StringContext>(0);
}

tree::TerminalNode* AnyFXParser::BinaryexpatomContext::IDENTIFIER() {
  return getToken(AnyFXParser::IDENTIFIER, 0);
}

AnyFXParser::BooleanContext* AnyFXParser::BinaryexpatomContext::boolean() {
  return getRuleContext<AnyFXParser::BooleanContext>(0);
}

AnyFXParser::InitializerExpressionContext* AnyFXParser::BinaryexpatomContext::initializerExpression() {
  return getRuleContext<AnyFXParser::InitializerExpressionContext>(0);
}

tree::TerminalNode* AnyFXParser::BinaryexpatomContext::LP() {
  return getToken(AnyFXParser::LP, 0);
}

AnyFXParser::ExpressionContext* AnyFXParser::BinaryexpatomContext::expression() {
  return getRuleContext<AnyFXParser::ExpressionContext>(0);
}

tree::TerminalNode* AnyFXParser::BinaryexpatomContext::RP() {
  return getToken(AnyFXParser::RP, 0);
}


size_t AnyFXParser::BinaryexpatomContext::getRuleIndex() const {
  return AnyFXParser::RuleBinaryexpatom;
}


AnyFXParser::BinaryexpatomContext* AnyFXParser::binaryexpatom() {
  BinaryexpatomContext *_localctx = _tracker.createInstance<BinaryexpatomContext>(_ctx, getState());
  enterRule(_localctx, 92, AnyFXParser::RuleBinaryexpatom);

          dynamic_cast<BinaryexpatomContext *>(_localctx)->tree =  nullptr;
      

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(797);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case AnyFXParser::INTEGERLITERAL: {
        enterOuterAlt(_localctx, 1);
        setState(771);
        dynamic_cast<BinaryexpatomContext *>(_localctx)->integerliteralToken = match(AnyFXParser::INTEGERLITERAL);
         dynamic_cast<BinaryexpatomContext *>(_localctx)->tree =  new IntExpression(atoi((dynamic_cast<BinaryexpatomContext *>(_localctx)->integerliteralToken != nullptr ? dynamic_cast<BinaryexpatomContext *>(_localctx)->integerliteralToken->getText() : "").c_str())); _localctx->tree->location = SetupFile(); 
        break;
      }

      case AnyFXParser::UINTEGERLITERAL: {
        enterOuterAlt(_localctx, 2);
        setState(773);
        dynamic_cast<BinaryexpatomContext *>(_localctx)->uintegerliteralToken = match(AnyFXParser::UINTEGERLITERAL);
         dynamic_cast<BinaryexpatomContext *>(_localctx)->tree =  new UIntExpression(strtoul((dynamic_cast<BinaryexpatomContext *>(_localctx)->uintegerliteralToken != nullptr ? dynamic_cast<BinaryexpatomContext *>(_localctx)->uintegerliteralToken->getText() : "").c_str(), nullptr, 10)); _localctx->tree->location = SetupFile(); 
        break;
      }

      case AnyFXParser::FLOATLITERAL: {
        enterOuterAlt(_localctx, 3);
        setState(775);
        dynamic_cast<BinaryexpatomContext *>(_localctx)->floatliteralToken = match(AnyFXParser::FLOATLITERAL);
         dynamic_cast<BinaryexpatomContext *>(_localctx)->tree =  new FloatExpression(atof((dynamic_cast<BinaryexpatomContext *>(_localctx)->floatliteralToken != nullptr ? dynamic_cast<BinaryexpatomContext *>(_localctx)->floatliteralToken->getText() : "").c_str())); _localctx->tree->location = SetupFile(); 
        break;
      }

      case AnyFXParser::DOUBLELITERAL: {
        enterOuterAlt(_localctx, 4);
        setState(777);
        dynamic_cast<BinaryexpatomContext *>(_localctx)->doubleliteralToken = match(AnyFXParser::DOUBLELITERAL);
         dynamic_cast<BinaryexpatomContext *>(_localctx)->tree =  new FloatExpression(atof((dynamic_cast<BinaryexpatomContext *>(_localctx)->doubleliteralToken != nullptr ? dynamic_cast<BinaryexpatomContext *>(_localctx)->doubleliteralToken->getText() : "").c_str())); _localctx->tree->location = SetupFile(); 
        break;
      }

      case AnyFXParser::HEX: {
        enterOuterAlt(_localctx, 5);
        setState(779);
        dynamic_cast<BinaryexpatomContext *>(_localctx)->hexToken = match(AnyFXParser::HEX);
         dynamic_cast<BinaryexpatomContext *>(_localctx)->tree =  new UIntExpression(strtoul((dynamic_cast<BinaryexpatomContext *>(_localctx)->hexToken != nullptr ? dynamic_cast<BinaryexpatomContext *>(_localctx)->hexToken->getText() : "").c_str(), nullptr, 16)); _localctx->tree->location = SetupFile(); 
        break;
      }

      case AnyFXParser::QO:
      case AnyFXParser::Q: {
        enterOuterAlt(_localctx, 6);
        setState(781);
        dynamic_cast<BinaryexpatomContext *>(_localctx)->stringContext = string();
         dynamic_cast<BinaryexpatomContext *>(_localctx)->tree =  new StringExpression(dynamic_cast<BinaryexpatomContext *>(_localctx)->stringContext->val); _localctx->tree->location = SetupFile(); 
        break;
      }

      case AnyFXParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 7);
        setState(784);
        dynamic_cast<BinaryexpatomContext *>(_localctx)->identifierToken = match(AnyFXParser::IDENTIFIER);
         dynamic_cast<BinaryexpatomContext *>(_localctx)->tree =  new SymbolExpression((dynamic_cast<BinaryexpatomContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<BinaryexpatomContext *>(_localctx)->identifierToken->getText() : "")); _localctx->tree->location = SetupFile(); 
        break;
      }

      case AnyFXParser::T__0:
      case AnyFXParser::T__1: {
        enterOuterAlt(_localctx, 8);
        setState(786);
        dynamic_cast<BinaryexpatomContext *>(_localctx)->booleanContext = boolean();
         dynamic_cast<BinaryexpatomContext *>(_localctx)->tree =  new BoolExpression(dynamic_cast<BinaryexpatomContext *>(_localctx)->booleanContext->val); _localctx->tree->location = SetupFile(); 
        break;
      }

      case AnyFXParser::LB: {
        enterOuterAlt(_localctx, 9);
        setState(789);
        dynamic_cast<BinaryexpatomContext *>(_localctx)->initializerExpressionContext = initializerExpression();
         dynamic_cast<BinaryexpatomContext *>(_localctx)->tree =  dynamic_cast<BinaryexpatomContext *>(_localctx)->initializerExpressionContext->tree; 
        break;
      }

      case AnyFXParser::LP: {
        enterOuterAlt(_localctx, 10);
        setState(792);
        match(AnyFXParser::LP);
        setState(793);
        dynamic_cast<BinaryexpatomContext *>(_localctx)->expressionContext = expression();
        setState(794);
        match(AnyFXParser::RP);
         dynamic_cast<BinaryexpatomContext *>(_localctx)->tree =  dynamic_cast<BinaryexpatomContext *>(_localctx)->expressionContext->tree; 
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

AnyFXParser::InitializerExpressionContext::InitializerExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* AnyFXParser::InitializerExpressionContext::LB() {
  return getToken(AnyFXParser::LB, 0);
}

tree::TerminalNode* AnyFXParser::InitializerExpressionContext::RB() {
  return getToken(AnyFXParser::RB, 0);
}

std::vector<AnyFXParser::AssignmentExpressionContext *> AnyFXParser::InitializerExpressionContext::assignmentExpression() {
  return getRuleContexts<AnyFXParser::AssignmentExpressionContext>();
}

AnyFXParser::AssignmentExpressionContext* AnyFXParser::InitializerExpressionContext::assignmentExpression(size_t i) {
  return getRuleContext<AnyFXParser::AssignmentExpressionContext>(i);
}

std::vector<tree::TerminalNode *> AnyFXParser::InitializerExpressionContext::CO() {
  return getTokens(AnyFXParser::CO);
}

tree::TerminalNode* AnyFXParser::InitializerExpressionContext::CO(size_t i) {
  return getToken(AnyFXParser::CO, i);
}


size_t AnyFXParser::InitializerExpressionContext::getRuleIndex() const {
  return AnyFXParser::RuleInitializerExpression;
}


AnyFXParser::InitializerExpressionContext* AnyFXParser::initializerExpression() {
  InitializerExpressionContext *_localctx = _tracker.createInstance<InitializerExpressionContext>(_ctx, getState());
  enterRule(_localctx, 94, AnyFXParser::RuleInitializerExpression);

          dynamic_cast<InitializerExpressionContext *>(_localctx)->tree =  nullptr;
          std::vector<Expression*> exprs;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(799);
    match(AnyFXParser::LB);
    setState(811);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << AnyFXParser::T__0)
      | (1ULL << AnyFXParser::T__1)
      | (1ULL << AnyFXParser::T__31)
      | (1ULL << AnyFXParser::T__32)
      | (1ULL << AnyFXParser::LP)
      | (1ULL << AnyFXParser::LB)
      | (1ULL << AnyFXParser::NOT)
      | (1ULL << AnyFXParser::QO)
      | (1ULL << AnyFXParser::CONNJUGATE)
      | (1ULL << AnyFXParser::Q))) != 0) || ((((_la - 70) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 70)) & ((1ULL << (AnyFXParser::ADD_OP - 70))
      | (1ULL << (AnyFXParser::SUB_OP - 70))
      | (1ULL << (AnyFXParser::MUL_OP - 70))
      | (1ULL << (AnyFXParser::INTEGERLITERAL - 70))
      | (1ULL << (AnyFXParser::UINTEGERLITERAL - 70))
      | (1ULL << (AnyFXParser::FLOATLITERAL - 70))
      | (1ULL << (AnyFXParser::DOUBLELITERAL - 70))
      | (1ULL << (AnyFXParser::HEX - 70))
      | (1ULL << (AnyFXParser::IDENTIFIER - 70)))) != 0)) {
      setState(800);
      dynamic_cast<InitializerExpressionContext *>(_localctx)->arg0 = assignmentExpression();
       exprs.push_back(dynamic_cast<InitializerExpressionContext *>(_localctx)->arg0->tree); 
      setState(808);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == AnyFXParser::CO) {
        setState(802);
        match(AnyFXParser::CO);
        setState(803);
        dynamic_cast<InitializerExpressionContext *>(_localctx)->argN = assignmentExpression();
         exprs.push_back(dynamic_cast<InitializerExpressionContext *>(_localctx)->argN->tree); 
        setState(810);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(813);
    match(AnyFXParser::RB);

            dynamic_cast<InitializerExpressionContext *>(_localctx)->tree =  new InitializerExpression(exprs);
            _localctx->tree->location = SetupFile();
        
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

// Static vars and initialization.
std::vector<dfa::DFA> AnyFXParser::_decisionToDFA;
atn::PredictionContextCache AnyFXParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN AnyFXParser::_atn;
std::vector<uint16_t> AnyFXParser::_serializedATN;

std::vector<std::string> AnyFXParser::_ruleNames = {
  "string", "boolean", "preprocess", "entry", "effect", "alias", "annotation", 
  "attribute", "typeDeclaration", "variables", "variable", "structureDeclaration", 
  "structure", "enumeration", "functionDeclaration", "codeblock", "function", 
  "program", "state", "statement", "expressionStatement", "ifStatement", 
  "forStatement", "forRangeStatement", "whileStatement", "scopeStatement", 
  "returnStatement", "continueStatement", "switchStatement", "breakStatement", 
  "expression", "commaExpression", "assignmentExpression", "logicalOrExpression", 
  "logicalAndExpression", "orExpression", "xorExpression", "andExpression", 
  "equivalencyExpression", "relationalExpression", "shiftExpression", "addSubtractExpression", 
  "multiplyDivideExpression", "prefixExpression", "suffixExpression", "namespaceExpression", 
  "binaryexpatom", "initializerExpression"
};

std::vector<std::string> AnyFXParser::_literalNames = {
  "", "'true'", "'false'", "'#line'", "'alias'", "'as'", "'struct'", "'enum'", 
  "'program'", "'sampler_state'", "'render_state'", "'if'", "'else'", "'for'", 
  "'..'", "'while'", "'do'", "'return'", "'continue'", "'switch'", "'case'", 
  "'default'", "'break'", "'+='", "'-='", "'*='", "'/='", "'%='", "'<<='", 
  "'>>='", "'<<'", "'>>'", "'++'", "'--'", "'::'", "';'", "','", "':'", 
  "'('", "')'", "'{'", "'}'", "'['", "']'", "'.'", "'!'", "'='", "'\"'", 
  "'?'", "'&'", "'&='", "'|'", "'|='", "'^'", "'^='", "'~'", "'''", "'#'", 
  "'\\'", "'<'", "'<='", "'>'", "'>='", "'=='", "'!='", "'&&'", "'||'", 
  "'%'", "'_'", "'@'", "'+'", "'-'", "'/'", "'*'", "'->'"
};

std::vector<std::string> AnyFXParser::_symbolicNames = {
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "SC", 
  "CO", "COL", "LP", "RP", "LB", "RB", "LL", "RR", "DOT", "NOT", "EQ", "QO", 
  "QU", "AND", "ANDSET", "OR", "ORSET", "XOR", "XORSET", "CONNJUGATE", "Q", 
  "NU", "FORWARDSLASH", "LESS", "LESSEQ", "GREATER", "GREATEREQ", "LOGICEQ", 
  "NOTEQ", "LOGICAND", "LOGICOR", "MOD", "UNDERSC", "SOBAKA", "ADD_OP", 
  "SUB_OP", "DIV_OP", "MUL_OP", "ARROW", "INTEGERLITERAL", "UINTEGERLITERAL", 
  "COMMENT", "ML_COMMENT", "FLOATLITERAL", "EXPONENT", "DOUBLELITERAL", 
  "HEX", "IDENTIFIER", "WS"
};

dfa::Vocabulary AnyFXParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> AnyFXParser::_tokenNames;

AnyFXParser::Initializer::Initializer() {
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  _serializedATN = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
    0x3, 0x56, 0x333, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
    0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 0x7, 
    0x4, 0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 0x4, 0xb, 
    0x9, 0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 0xe, 0x9, 0xe, 
    0x4, 0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x4, 0x11, 0x9, 0x11, 0x4, 
    0x12, 0x9, 0x12, 0x4, 0x13, 0x9, 0x13, 0x4, 0x14, 0x9, 0x14, 0x4, 0x15, 
    0x9, 0x15, 0x4, 0x16, 0x9, 0x16, 0x4, 0x17, 0x9, 0x17, 0x4, 0x18, 0x9, 
    0x18, 0x4, 0x19, 0x9, 0x19, 0x4, 0x1a, 0x9, 0x1a, 0x4, 0x1b, 0x9, 0x1b, 
    0x4, 0x1c, 0x9, 0x1c, 0x4, 0x1d, 0x9, 0x1d, 0x4, 0x1e, 0x9, 0x1e, 0x4, 
    0x1f, 0x9, 0x1f, 0x4, 0x20, 0x9, 0x20, 0x4, 0x21, 0x9, 0x21, 0x4, 0x22, 
    0x9, 0x22, 0x4, 0x23, 0x9, 0x23, 0x4, 0x24, 0x9, 0x24, 0x4, 0x25, 0x9, 
    0x25, 0x4, 0x26, 0x9, 0x26, 0x4, 0x27, 0x9, 0x27, 0x4, 0x28, 0x9, 0x28, 
    0x4, 0x29, 0x9, 0x29, 0x4, 0x2a, 0x9, 0x2a, 0x4, 0x2b, 0x9, 0x2b, 0x4, 
    0x2c, 0x9, 0x2c, 0x4, 0x2d, 0x9, 0x2d, 0x4, 0x2e, 0x9, 0x2e, 0x4, 0x2f, 
    0x9, 0x2f, 0x4, 0x30, 0x9, 0x30, 0x4, 0x31, 0x9, 0x31, 0x3, 0x2, 0x3, 
    0x2, 0x3, 0x2, 0x7, 0x2, 0x66, 0xa, 0x2, 0xc, 0x2, 0xe, 0x2, 0x69, 0xb, 
    0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x7, 0x2, 0x6f, 0xa, 0x2, 
    0xc, 0x2, 0xe, 0x2, 0x72, 0xb, 0x2, 0x3, 0x2, 0x5, 0x2, 0x75, 0xa, 0x2, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x5, 0x3, 0x7b, 0xa, 0x3, 0x3, 
    0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x7, 
    0x4, 0x84, 0xa, 0x4, 0xc, 0x4, 0xe, 0x4, 0x87, 0xb, 0x4, 0x3, 0x4, 0x3, 
    0x4, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 
    0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 
    0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 
    0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 
    0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 
    0x6, 0x7, 0x6, 0xae, 0xa, 0x6, 0xc, 0x6, 0xe, 0x6, 0xb1, 0xb, 0x6, 0x3, 
    0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 
    0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 
    0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 
    0x9, 0x5, 0x9, 0xc9, 0xa, 0x9, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xb, 
    0x3, 0xb, 0x3, 0xb, 0x7, 0xb, 0xd1, 0xa, 0xb, 0xc, 0xb, 0xe, 0xb, 0xd4, 
    0xb, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x7, 0xb, 0xd9, 0xa, 0xb, 0xc, 
    0xb, 0xe, 0xb, 0xdc, 0xb, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 
    0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x7, 0xb, 0xe6, 0xa, 0xb, 0xc, 
    0xb, 0xe, 0xb, 0xe9, 0xb, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xc, 0x3, 0xc, 
    0x3, 0xc, 0x7, 0xc, 0xf0, 0xa, 0xc, 0xc, 0xc, 0xe, 0xc, 0xf3, 0xb, 0xc, 
    0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xd, 
    0x3, 0xd, 0x3, 0xd, 0x7, 0xd, 0xfe, 0xa, 0xd, 0xc, 0xd, 0xe, 0xd, 0x101, 
    0xb, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x7, 0xd, 0x107, 0xa, 
    0xd, 0xc, 0xd, 0xe, 0xd, 0x10a, 0xb, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 
    0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 
    0x7, 0xe, 0x116, 0xa, 0xe, 0xc, 0xe, 0xe, 0xe, 0x119, 0xb, 0xe, 0x3, 
    0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 
    0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x5, 0xf, 0x127, 0xa, 0xf, 
    0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 
    0x7, 0x10, 0x130, 0xa, 0x10, 0xc, 0x10, 0xe, 0x10, 0x133, 0xb, 0x10, 
    0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 
    0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x7, 0x10, 0x13f, 0xa, 0x10, 
    0xc, 0x10, 0xe, 0x10, 0x142, 0xb, 0x10, 0x5, 0x10, 0x144, 0xa, 0x10, 
    0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x11, 0x3, 0x11, 0x7, 0x11, 0x14b, 
    0xa, 0x11, 0xc, 0x11, 0xe, 0x11, 0x14e, 0xb, 0x11, 0x3, 0x11, 0x3, 0x11, 
    0x5, 0x11, 0x152, 0xa, 0x11, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 
    0x3, 0x12, 0x3, 0x12, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x7, 0x13, 0x15d, 
    0xa, 0x13, 0xc, 0x13, 0xe, 0x13, 0x160, 0xb, 0x13, 0x3, 0x13, 0x3, 0x13, 
    0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x7, 
    0x13, 0x16a, 0xa, 0x13, 0xc, 0x13, 0xe, 0x13, 0x16d, 0xb, 0x13, 0x3, 
    0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 
    0x5, 0x14, 0x176, 0xa, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 
    0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x7, 0x14, 0x17f, 0xa, 0x14, 0xc, 0x14, 
    0xe, 0x14, 0x182, 0xb, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x15, 
    0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 
    0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 
    0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 
    0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x5, 0x15, 0x1a0, 
    0xa, 0x15, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x3, 0x17, 0x3, 0x17, 0x3, 
    0x17, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 
    0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x5, 0x17, 0x1b1, 0xa, 0x17, 0x3, 0x17, 
    0x3, 0x17, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 
    0x18, 0x5, 0x18, 0x1bb, 0xa, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 
    0x3, 0x18, 0x5, 0x18, 0x1c1, 0xa, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 
    0x3, 0x18, 0x5, 0x18, 0x1c7, 0xa, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 
    0x3, 0x18, 0x3, 0x18, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 
    0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 
    0x3, 0x19, 0x3, 0x19, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 
    0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 
    0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 
    0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x5, 0x1a, 0x1f1, 
    0xa, 0x1a, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x3, 
    0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x7, 0x1b, 0x1fc, 0xa, 0x1b, 
    0xc, 0x1b, 0xe, 0x1b, 0x1ff, 0xb, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 
    0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x5, 0x1c, 0x209, 
    0xa, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1d, 0x3, 0x1d, 0x3, 
    0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 
    0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 
    0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x7, 0x1e, 0x220, 0xa, 0x1e, 0xc, 0x1e, 
    0xe, 0x1e, 0x223, 0xb, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 
    0x3, 0x1e, 0x5, 0x1e, 0x22a, 0xa, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 
    0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x20, 0x3, 0x20, 0x3, 
    0x20, 0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 
    0x7, 0x21, 0x23c, 0xa, 0x21, 0xc, 0x21, 0xe, 0x21, 0x23f, 0xb, 0x21, 
    0x3, 0x22, 0x3, 0x22, 0x3, 0x22, 0x3, 0x22, 0x3, 0x22, 0x3, 0x22, 0x3, 
    0x22, 0x3, 0x22, 0x3, 0x22, 0x3, 0x22, 0x3, 0x22, 0x3, 0x22, 0x7, 0x22, 
    0x24d, 0xa, 0x22, 0xc, 0x22, 0xe, 0x22, 0x250, 0xb, 0x22, 0x3, 0x23, 
    0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x7, 0x23, 0x258, 
    0xa, 0x23, 0xc, 0x23, 0xe, 0x23, 0x25b, 0xb, 0x23, 0x3, 0x24, 0x3, 0x24, 
    0x3, 0x24, 0x3, 0x24, 0x3, 0x24, 0x3, 0x24, 0x7, 0x24, 0x263, 0xa, 0x24, 
    0xc, 0x24, 0xe, 0x24, 0x266, 0xb, 0x24, 0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 
    0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 0x7, 0x25, 0x26e, 0xa, 0x25, 0xc, 0x25, 
    0xe, 0x25, 0x271, 0xb, 0x25, 0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 
    0x3, 0x26, 0x3, 0x26, 0x7, 0x26, 0x279, 0xa, 0x26, 0xc, 0x26, 0xe, 0x26, 
    0x27c, 0xb, 0x26, 0x3, 0x27, 0x3, 0x27, 0x3, 0x27, 0x3, 0x27, 0x3, 0x27, 
    0x3, 0x27, 0x7, 0x27, 0x284, 0xa, 0x27, 0xc, 0x27, 0xe, 0x27, 0x287, 
    0xb, 0x27, 0x3, 0x28, 0x3, 0x28, 0x3, 0x28, 0x3, 0x28, 0x3, 0x28, 0x3, 
    0x28, 0x7, 0x28, 0x28f, 0xa, 0x28, 0xc, 0x28, 0xe, 0x28, 0x292, 0xb, 
    0x28, 0x3, 0x29, 0x3, 0x29, 0x3, 0x29, 0x3, 0x29, 0x3, 0x29, 0x3, 0x29, 
    0x7, 0x29, 0x29a, 0xa, 0x29, 0xc, 0x29, 0xe, 0x29, 0x29d, 0xb, 0x29, 
    0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x7, 
    0x2a, 0x2a5, 0xa, 0x2a, 0xc, 0x2a, 0xe, 0x2a, 0x2a8, 0xb, 0x2a, 0x3, 
    0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x7, 0x2b, 
    0x2b0, 0xa, 0x2b, 0xc, 0x2b, 0xe, 0x2b, 0x2b3, 0xb, 0x2b, 0x3, 0x2c, 
    0x3, 0x2c, 0x3, 0x2c, 0x3, 0x2c, 0x3, 0x2c, 0x3, 0x2c, 0x7, 0x2c, 0x2bb, 
    0xa, 0x2c, 0xc, 0x2c, 0xe, 0x2c, 0x2be, 0xb, 0x2c, 0x3, 0x2d, 0x3, 0x2d, 
    0x7, 0x2d, 0x2c2, 0xa, 0x2d, 0xc, 0x2d, 0xe, 0x2d, 0x2c5, 0xb, 0x2d, 
    0x3, 0x2d, 0x3, 0x2d, 0x3, 0x2d, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x7, 
    0x2e, 0x2cd, 0xa, 0x2e, 0xc, 0x2e, 0xe, 0x2e, 0x2d0, 0xb, 0x2e, 0x3, 
    0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 
    0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x7, 0x2e, 0x2de, 
    0xa, 0x2e, 0xc, 0x2e, 0xe, 0x2e, 0x2e1, 0xb, 0x2e, 0x5, 0x2e, 0x2e3, 
    0xa, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 
    0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 
    0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x5, 0x2e, 0x2f6, 
    0xa, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x7, 0x2e, 0x2fa, 0xa, 0x2e, 0xc, 0x2e, 
    0xe, 0x2e, 0x2fd, 0xb, 0x2e, 0x5, 0x2e, 0x2ff, 0xa, 0x2e, 0x3, 0x2f, 
    0x3, 0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x3, 0x30, 0x3, 0x30, 0x3, 
    0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 
    0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 
    0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 
    0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x5, 0x30, 0x320, 0xa, 0x30, 
    0x3, 0x31, 0x3, 0x31, 0x3, 0x31, 0x3, 0x31, 0x3, 0x31, 0x3, 0x31, 0x3, 
    0x31, 0x7, 0x31, 0x329, 0xa, 0x31, 0xc, 0x31, 0xe, 0x31, 0x32c, 0xb, 
    0x31, 0x5, 0x31, 0x32e, 0xa, 0x31, 0x3, 0x31, 0x3, 0x31, 0x3, 0x31, 
    0x3, 0x31, 0x4, 0x85, 0xaf, 0x2, 0x32, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 
    0xe, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1a, 0x1c, 0x1e, 0x20, 0x22, 0x24, 
    0x26, 0x28, 0x2a, 0x2c, 0x2e, 0x30, 0x32, 0x34, 0x36, 0x38, 0x3a, 0x3c, 
    0x3e, 0x40, 0x42, 0x44, 0x46, 0x48, 0x4a, 0x4c, 0x4e, 0x50, 0x52, 0x54, 
    0x56, 0x58, 0x5a, 0x5c, 0x5e, 0x60, 0x2, 0xd, 0x3, 0x2, 0x31, 0x31, 
    0x3, 0x2, 0x3a, 0x3a, 0x3, 0x2, 0x2a, 0x2b, 0x7, 0x2, 0x19, 0x1f, 0x30, 
    0x30, 0x34, 0x34, 0x36, 0x36, 0x38, 0x38, 0x3, 0x2, 0x41, 0x42, 0x3, 
    0x2, 0x3d, 0x40, 0x3, 0x2, 0x20, 0x21, 0x3, 0x2, 0x48, 0x49, 0x4, 0x2, 
    0x45, 0x45, 0x4a, 0x4b, 0x7, 0x2, 0x22, 0x23, 0x2f, 0x2f, 0x39, 0x39, 
    0x48, 0x49, 0x4b, 0x4b, 0x3, 0x2, 0x22, 0x23, 0x2, 0x355, 0x2, 0x74, 
    0x3, 0x2, 0x2, 0x2, 0x4, 0x7a, 0x3, 0x2, 0x2, 0x2, 0x6, 0x85, 0x3, 0x2, 
    0x2, 0x2, 0x8, 0x8a, 0x3, 0x2, 0x2, 0x2, 0xa, 0xaf, 0x3, 0x2, 0x2, 0x2, 
    0xc, 0xb2, 0x3, 0x2, 0x2, 0x2, 0xe, 0xb9, 0x3, 0x2, 0x2, 0x2, 0x10, 
    0xc8, 0x3, 0x2, 0x2, 0x2, 0x12, 0xca, 0x3, 0x2, 0x2, 0x2, 0x14, 0xd2, 
    0x3, 0x2, 0x2, 0x2, 0x16, 0xf1, 0x3, 0x2, 0x2, 0x2, 0x18, 0xff, 0x3, 
    0x2, 0x2, 0x2, 0x1a, 0x10e, 0x3, 0x2, 0x2, 0x2, 0x1c, 0x11d, 0x3, 0x2, 
    0x2, 0x2, 0x1e, 0x131, 0x3, 0x2, 0x2, 0x2, 0x20, 0x151, 0x3, 0x2, 0x2, 
    0x2, 0x22, 0x153, 0x3, 0x2, 0x2, 0x2, 0x24, 0x15e, 0x3, 0x2, 0x2, 0x2, 
    0x26, 0x175, 0x3, 0x2, 0x2, 0x2, 0x28, 0x19f, 0x3, 0x2, 0x2, 0x2, 0x2a, 
    0x1a1, 0x3, 0x2, 0x2, 0x2, 0x2c, 0x1a4, 0x3, 0x2, 0x2, 0x2, 0x2e, 0x1b4, 
    0x3, 0x2, 0x2, 0x2, 0x30, 0x1cd, 0x3, 0x2, 0x2, 0x2, 0x32, 0x1f0, 0x3, 
    0x2, 0x2, 0x2, 0x34, 0x1f2, 0x3, 0x2, 0x2, 0x2, 0x36, 0x203, 0x3, 0x2, 
    0x2, 0x2, 0x38, 0x20d, 0x3, 0x2, 0x2, 0x2, 0x3a, 0x212, 0x3, 0x2, 0x2, 
    0x2, 0x3c, 0x22e, 0x3, 0x2, 0x2, 0x2, 0x3e, 0x232, 0x3, 0x2, 0x2, 0x2, 
    0x40, 0x235, 0x3, 0x2, 0x2, 0x2, 0x42, 0x240, 0x3, 0x2, 0x2, 0x2, 0x44, 
    0x251, 0x3, 0x2, 0x2, 0x2, 0x46, 0x25c, 0x3, 0x2, 0x2, 0x2, 0x48, 0x267, 
    0x3, 0x2, 0x2, 0x2, 0x4a, 0x272, 0x3, 0x2, 0x2, 0x2, 0x4c, 0x27d, 0x3, 
    0x2, 0x2, 0x2, 0x4e, 0x288, 0x3, 0x2, 0x2, 0x2, 0x50, 0x293, 0x3, 0x2, 
    0x2, 0x2, 0x52, 0x29e, 0x3, 0x2, 0x2, 0x2, 0x54, 0x2a9, 0x3, 0x2, 0x2, 
    0x2, 0x56, 0x2b4, 0x3, 0x2, 0x2, 0x2, 0x58, 0x2c3, 0x3, 0x2, 0x2, 0x2, 
    0x5a, 0x2fe, 0x3, 0x2, 0x2, 0x2, 0x5c, 0x300, 0x3, 0x2, 0x2, 0x2, 0x5e, 
    0x31f, 0x3, 0x2, 0x2, 0x2, 0x60, 0x321, 0x3, 0x2, 0x2, 0x2, 0x62, 0x67, 
    0x7, 0x31, 0x2, 0x2, 0x63, 0x64, 0xa, 0x2, 0x2, 0x2, 0x64, 0x66, 0x8, 
    0x2, 0x1, 0x2, 0x65, 0x63, 0x3, 0x2, 0x2, 0x2, 0x66, 0x69, 0x3, 0x2, 
    0x2, 0x2, 0x67, 0x65, 0x3, 0x2, 0x2, 0x2, 0x67, 0x68, 0x3, 0x2, 0x2, 
    0x2, 0x68, 0x6a, 0x3, 0x2, 0x2, 0x2, 0x69, 0x67, 0x3, 0x2, 0x2, 0x2, 
    0x6a, 0x75, 0x7, 0x31, 0x2, 0x2, 0x6b, 0x70, 0x7, 0x3a, 0x2, 0x2, 0x6c, 
    0x6d, 0xa, 0x3, 0x2, 0x2, 0x6d, 0x6f, 0x8, 0x2, 0x1, 0x2, 0x6e, 0x6c, 
    0x3, 0x2, 0x2, 0x2, 0x6f, 0x72, 0x3, 0x2, 0x2, 0x2, 0x70, 0x6e, 0x3, 
    0x2, 0x2, 0x2, 0x70, 0x71, 0x3, 0x2, 0x2, 0x2, 0x71, 0x73, 0x3, 0x2, 
    0x2, 0x2, 0x72, 0x70, 0x3, 0x2, 0x2, 0x2, 0x73, 0x75, 0x7, 0x3a, 0x2, 
    0x2, 0x74, 0x62, 0x3, 0x2, 0x2, 0x2, 0x74, 0x6b, 0x3, 0x2, 0x2, 0x2, 
    0x75, 0x3, 0x3, 0x2, 0x2, 0x2, 0x76, 0x77, 0x7, 0x3, 0x2, 0x2, 0x77, 
    0x7b, 0x8, 0x3, 0x1, 0x2, 0x78, 0x79, 0x7, 0x4, 0x2, 0x2, 0x79, 0x7b, 
    0x8, 0x3, 0x1, 0x2, 0x7a, 0x76, 0x3, 0x2, 0x2, 0x2, 0x7a, 0x78, 0x3, 
    0x2, 0x2, 0x2, 0x7b, 0x5, 0x3, 0x2, 0x2, 0x2, 0x7c, 0x7d, 0x8, 0x4, 
    0x1, 0x2, 0x7d, 0x7e, 0x7, 0x5, 0x2, 0x2, 0x7e, 0x7f, 0x7, 0x4d, 0x2, 
    0x2, 0x7f, 0x80, 0x5, 0x2, 0x2, 0x2, 0x80, 0x81, 0x8, 0x4, 0x1, 0x2, 
    0x81, 0x84, 0x3, 0x2, 0x2, 0x2, 0x82, 0x84, 0xb, 0x2, 0x2, 0x2, 0x83, 
    0x7c, 0x3, 0x2, 0x2, 0x2, 0x83, 0x82, 0x3, 0x2, 0x2, 0x2, 0x84, 0x87, 
    0x3, 0x2, 0x2, 0x2, 0x85, 0x86, 0x3, 0x2, 0x2, 0x2, 0x85, 0x83, 0x3, 
    0x2, 0x2, 0x2, 0x86, 0x88, 0x3, 0x2, 0x2, 0x2, 0x87, 0x85, 0x3, 0x2, 
    0x2, 0x2, 0x88, 0x89, 0x7, 0x2, 0x2, 0x3, 0x89, 0x7, 0x3, 0x2, 0x2, 
    0x2, 0x8a, 0x8b, 0x5, 0xa, 0x6, 0x2, 0x8b, 0x8c, 0x8, 0x5, 0x1, 0x2, 
    0x8c, 0x8d, 0x7, 0x2, 0x2, 0x3, 0x8d, 0x9, 0x3, 0x2, 0x2, 0x2, 0x8e, 
    0x8f, 0x5, 0xc, 0x7, 0x2, 0x8f, 0x90, 0x7, 0x25, 0x2, 0x2, 0x90, 0x91, 
    0x8, 0x6, 0x1, 0x2, 0x91, 0xae, 0x3, 0x2, 0x2, 0x2, 0x92, 0x93, 0x5, 
    0x1e, 0x10, 0x2, 0x93, 0x94, 0x7, 0x25, 0x2, 0x2, 0x94, 0x95, 0x8, 0x6, 
    0x1, 0x2, 0x95, 0xae, 0x3, 0x2, 0x2, 0x2, 0x96, 0x97, 0x5, 0x22, 0x12, 
    0x2, 0x97, 0x98, 0x8, 0x6, 0x1, 0x2, 0x98, 0xae, 0x3, 0x2, 0x2, 0x2, 
    0x99, 0x9a, 0x5, 0x14, 0xb, 0x2, 0x9a, 0x9b, 0x7, 0x25, 0x2, 0x2, 0x9b, 
    0x9c, 0x8, 0x6, 0x1, 0x2, 0x9c, 0xae, 0x3, 0x2, 0x2, 0x2, 0x9d, 0x9e, 
    0x5, 0x1a, 0xe, 0x2, 0x9e, 0x9f, 0x7, 0x25, 0x2, 0x2, 0x9f, 0xa0, 0x8, 
    0x6, 0x1, 0x2, 0xa0, 0xae, 0x3, 0x2, 0x2, 0x2, 0xa1, 0xa2, 0x5, 0x1c, 
    0xf, 0x2, 0xa2, 0xa3, 0x7, 0x25, 0x2, 0x2, 0xa3, 0xa4, 0x8, 0x6, 0x1, 
    0x2, 0xa4, 0xae, 0x3, 0x2, 0x2, 0x2, 0xa5, 0xa6, 0x5, 0x26, 0x14, 0x2, 
    0xa6, 0xa7, 0x7, 0x25, 0x2, 0x2, 0xa7, 0xa8, 0x8, 0x6, 0x1, 0x2, 0xa8, 
    0xae, 0x3, 0x2, 0x2, 0x2, 0xa9, 0xaa, 0x5, 0x24, 0x13, 0x2, 0xaa, 0xab, 
    0x7, 0x25, 0x2, 0x2, 0xab, 0xac, 0x8, 0x6, 0x1, 0x2, 0xac, 0xae, 0x3, 
    0x2, 0x2, 0x2, 0xad, 0x8e, 0x3, 0x2, 0x2, 0x2, 0xad, 0x92, 0x3, 0x2, 
    0x2, 0x2, 0xad, 0x96, 0x3, 0x2, 0x2, 0x2, 0xad, 0x99, 0x3, 0x2, 0x2, 
    0x2, 0xad, 0x9d, 0x3, 0x2, 0x2, 0x2, 0xad, 0xa1, 0x3, 0x2, 0x2, 0x2, 
    0xad, 0xa5, 0x3, 0x2, 0x2, 0x2, 0xad, 0xa9, 0x3, 0x2, 0x2, 0x2, 0xae, 
    0xb1, 0x3, 0x2, 0x2, 0x2, 0xaf, 0xb0, 0x3, 0x2, 0x2, 0x2, 0xaf, 0xad, 
    0x3, 0x2, 0x2, 0x2, 0xb0, 0xb, 0x3, 0x2, 0x2, 0x2, 0xb1, 0xaf, 0x3, 
    0x2, 0x2, 0x2, 0xb2, 0xb3, 0x7, 0x6, 0x2, 0x2, 0xb3, 0xb4, 0x7, 0x55, 
    0x2, 0x2, 0xb4, 0xb5, 0x7, 0x7, 0x2, 0x2, 0xb5, 0xb6, 0x7, 0x55, 0x2, 
    0x2, 0xb6, 0xb7, 0x8, 0x7, 0x1, 0x2, 0xb7, 0xb8, 0x8, 0x7, 0x1, 0x2, 
    0xb8, 0xd, 0x3, 0x2, 0x2, 0x2, 0xb9, 0xba, 0x7, 0x47, 0x2, 0x2, 0xba, 
    0xbb, 0x7, 0x55, 0x2, 0x2, 0xbb, 0xbc, 0x7, 0x28, 0x2, 0x2, 0xbc, 0xbd, 
    0x5, 0x3e, 0x20, 0x2, 0xbd, 0xbe, 0x7, 0x29, 0x2, 0x2, 0xbe, 0xbf, 0x8, 
    0x8, 0x1, 0x2, 0xbf, 0xf, 0x3, 0x2, 0x2, 0x2, 0xc0, 0xc1, 0x7, 0x55, 
    0x2, 0x2, 0xc1, 0xc2, 0x7, 0x28, 0x2, 0x2, 0xc2, 0xc3, 0x5, 0x3e, 0x20, 
    0x2, 0xc3, 0xc4, 0x7, 0x29, 0x2, 0x2, 0xc4, 0xc5, 0x8, 0x9, 0x1, 0x2, 
    0xc5, 0xc9, 0x3, 0x2, 0x2, 0x2, 0xc6, 0xc7, 0x7, 0x55, 0x2, 0x2, 0xc7, 
    0xc9, 0x8, 0x9, 0x1, 0x2, 0xc8, 0xc0, 0x3, 0x2, 0x2, 0x2, 0xc8, 0xc6, 
    0x3, 0x2, 0x2, 0x2, 0xc9, 0x11, 0x3, 0x2, 0x2, 0x2, 0xca, 0xcb, 0x7, 
    0x55, 0x2, 0x2, 0xcb, 0xcc, 0x8, 0xa, 0x1, 0x2, 0xcc, 0x13, 0x3, 0x2, 
    0x2, 0x2, 0xcd, 0xce, 0x5, 0xe, 0x8, 0x2, 0xce, 0xcf, 0x8, 0xb, 0x1, 
    0x2, 0xcf, 0xd1, 0x3, 0x2, 0x2, 0x2, 0xd0, 0xcd, 0x3, 0x2, 0x2, 0x2, 
    0xd1, 0xd4, 0x3, 0x2, 0x2, 0x2, 0xd2, 0xd0, 0x3, 0x2, 0x2, 0x2, 0xd2, 
    0xd3, 0x3, 0x2, 0x2, 0x2, 0xd3, 0xda, 0x3, 0x2, 0x2, 0x2, 0xd4, 0xd2, 
    0x3, 0x2, 0x2, 0x2, 0xd5, 0xd6, 0x5, 0x10, 0x9, 0x2, 0xd6, 0xd7, 0x8, 
    0xb, 0x1, 0x2, 0xd7, 0xd9, 0x3, 0x2, 0x2, 0x2, 0xd8, 0xd5, 0x3, 0x2, 
    0x2, 0x2, 0xd9, 0xdc, 0x3, 0x2, 0x2, 0x2, 0xda, 0xd8, 0x3, 0x2, 0x2, 
    0x2, 0xda, 0xdb, 0x3, 0x2, 0x2, 0x2, 0xdb, 0xdd, 0x3, 0x2, 0x2, 0x2, 
    0xdc, 0xda, 0x3, 0x2, 0x2, 0x2, 0xdd, 0xde, 0x5, 0x12, 0xa, 0x2, 0xde, 
    0xdf, 0x8, 0xb, 0x1, 0x2, 0xdf, 0xe0, 0x5, 0x42, 0x22, 0x2, 0xe0, 0xe7, 
    0x8, 0xb, 0x1, 0x2, 0xe1, 0xe2, 0x7, 0x26, 0x2, 0x2, 0xe2, 0xe3, 0x5, 
    0x42, 0x22, 0x2, 0xe3, 0xe4, 0x8, 0xb, 0x1, 0x2, 0xe4, 0xe6, 0x3, 0x2, 
    0x2, 0x2, 0xe5, 0xe1, 0x3, 0x2, 0x2, 0x2, 0xe6, 0xe9, 0x3, 0x2, 0x2, 
    0x2, 0xe7, 0xe5, 0x3, 0x2, 0x2, 0x2, 0xe7, 0xe8, 0x3, 0x2, 0x2, 0x2, 
    0xe8, 0xea, 0x3, 0x2, 0x2, 0x2, 0xe9, 0xe7, 0x3, 0x2, 0x2, 0x2, 0xea, 
    0xeb, 0x8, 0xb, 0x1, 0x2, 0xeb, 0x15, 0x3, 0x2, 0x2, 0x2, 0xec, 0xed, 
    0x5, 0x10, 0x9, 0x2, 0xed, 0xee, 0x8, 0xc, 0x1, 0x2, 0xee, 0xf0, 0x3, 
    0x2, 0x2, 0x2, 0xef, 0xec, 0x3, 0x2, 0x2, 0x2, 0xf0, 0xf3, 0x3, 0x2, 
    0x2, 0x2, 0xf1, 0xef, 0x3, 0x2, 0x2, 0x2, 0xf1, 0xf2, 0x3, 0x2, 0x2, 
    0x2, 0xf2, 0xf4, 0x3, 0x2, 0x2, 0x2, 0xf3, 0xf1, 0x3, 0x2, 0x2, 0x2, 
    0xf4, 0xf5, 0x5, 0x12, 0xa, 0x2, 0xf5, 0xf6, 0x8, 0xc, 0x1, 0x2, 0xf6, 
    0xf7, 0x5, 0x3e, 0x20, 0x2, 0xf7, 0xf8, 0x8, 0xc, 0x1, 0x2, 0xf8, 0xf9, 
    0x8, 0xc, 0x1, 0x2, 0xf9, 0x17, 0x3, 0x2, 0x2, 0x2, 0xfa, 0xfb, 0x5, 
    0xe, 0x8, 0x2, 0xfb, 0xfc, 0x8, 0xd, 0x1, 0x2, 0xfc, 0xfe, 0x3, 0x2, 
    0x2, 0x2, 0xfd, 0xfa, 0x3, 0x2, 0x2, 0x2, 0xfe, 0x101, 0x3, 0x2, 0x2, 
    0x2, 0xff, 0xfd, 0x3, 0x2, 0x2, 0x2, 0xff, 0x100, 0x3, 0x2, 0x2, 0x2, 
    0x100, 0x102, 0x3, 0x2, 0x2, 0x2, 0x101, 0xff, 0x3, 0x2, 0x2, 0x2, 0x102, 
    0x108, 0x7, 0x8, 0x2, 0x2, 0x103, 0x104, 0x5, 0x10, 0x9, 0x2, 0x104, 
    0x105, 0x8, 0xd, 0x1, 0x2, 0x105, 0x107, 0x3, 0x2, 0x2, 0x2, 0x106, 
    0x103, 0x3, 0x2, 0x2, 0x2, 0x107, 0x10a, 0x3, 0x2, 0x2, 0x2, 0x108, 
    0x106, 0x3, 0x2, 0x2, 0x2, 0x108, 0x109, 0x3, 0x2, 0x2, 0x2, 0x109, 
    0x10b, 0x3, 0x2, 0x2, 0x2, 0x10a, 0x108, 0x3, 0x2, 0x2, 0x2, 0x10b, 
    0x10c, 0x7, 0x55, 0x2, 0x2, 0x10c, 0x10d, 0x8, 0xd, 0x1, 0x2, 0x10d, 
    0x19, 0x3, 0x2, 0x2, 0x2, 0x10e, 0x10f, 0x5, 0x18, 0xd, 0x2, 0x10f, 
    0x110, 0x8, 0xe, 0x1, 0x2, 0x110, 0x117, 0x7, 0x2a, 0x2, 0x2, 0x111, 
    0x112, 0x5, 0x14, 0xb, 0x2, 0x112, 0x113, 0x8, 0xe, 0x1, 0x2, 0x113, 
    0x114, 0x7, 0x25, 0x2, 0x2, 0x114, 0x116, 0x3, 0x2, 0x2, 0x2, 0x115, 
    0x111, 0x3, 0x2, 0x2, 0x2, 0x116, 0x119, 0x3, 0x2, 0x2, 0x2, 0x117, 
    0x115, 0x3, 0x2, 0x2, 0x2, 0x117, 0x118, 0x3, 0x2, 0x2, 0x2, 0x118, 
    0x11a, 0x3, 0x2, 0x2, 0x2, 0x119, 0x117, 0x3, 0x2, 0x2, 0x2, 0x11a, 
    0x11b, 0x7, 0x2b, 0x2, 0x2, 0x11b, 0x11c, 0x8, 0xe, 0x1, 0x2, 0x11c, 
    0x1b, 0x3, 0x2, 0x2, 0x2, 0x11d, 0x11e, 0x7, 0x9, 0x2, 0x2, 0x11e, 0x11f, 
    0x7, 0x55, 0x2, 0x2, 0x11f, 0x120, 0x7, 0x2a, 0x2, 0x2, 0x120, 0x121, 
    0x7, 0x55, 0x2, 0x2, 0x121, 0x126, 0x8, 0xf, 0x1, 0x2, 0x122, 0x123, 
    0x7, 0x30, 0x2, 0x2, 0x123, 0x124, 0x5, 0x3e, 0x20, 0x2, 0x124, 0x125, 
    0x8, 0xf, 0x1, 0x2, 0x125, 0x127, 0x3, 0x2, 0x2, 0x2, 0x126, 0x122, 
    0x3, 0x2, 0x2, 0x2, 0x126, 0x127, 0x3, 0x2, 0x2, 0x2, 0x127, 0x128, 
    0x3, 0x2, 0x2, 0x2, 0x128, 0x129, 0x8, 0xf, 0x1, 0x2, 0x129, 0x12a, 
    0x7, 0x2b, 0x2, 0x2, 0x12a, 0x12b, 0x8, 0xf, 0x1, 0x2, 0x12b, 0x1d, 
    0x3, 0x2, 0x2, 0x2, 0x12c, 0x12d, 0x5, 0x10, 0x9, 0x2, 0x12d, 0x12e, 
    0x8, 0x10, 0x1, 0x2, 0x12e, 0x130, 0x3, 0x2, 0x2, 0x2, 0x12f, 0x12c, 
    0x3, 0x2, 0x2, 0x2, 0x130, 0x133, 0x3, 0x2, 0x2, 0x2, 0x131, 0x12f, 
    0x3, 0x2, 0x2, 0x2, 0x131, 0x132, 0x3, 0x2, 0x2, 0x2, 0x132, 0x134, 
    0x3, 0x2, 0x2, 0x2, 0x133, 0x131, 0x3, 0x2, 0x2, 0x2, 0x134, 0x135, 
    0x5, 0x12, 0xa, 0x2, 0x135, 0x136, 0x7, 0x55, 0x2, 0x2, 0x136, 0x137, 
    0x8, 0x10, 0x1, 0x2, 0x137, 0x143, 0x7, 0x28, 0x2, 0x2, 0x138, 0x139, 
    0x5, 0x16, 0xc, 0x2, 0x139, 0x140, 0x8, 0x10, 0x1, 0x2, 0x13a, 0x13b, 
    0x7, 0x26, 0x2, 0x2, 0x13b, 0x13c, 0x5, 0x16, 0xc, 0x2, 0x13c, 0x13d, 
    0x8, 0x10, 0x1, 0x2, 0x13d, 0x13f, 0x3, 0x2, 0x2, 0x2, 0x13e, 0x13a, 
    0x3, 0x2, 0x2, 0x2, 0x13f, 0x142, 0x3, 0x2, 0x2, 0x2, 0x140, 0x13e, 
    0x3, 0x2, 0x2, 0x2, 0x140, 0x141, 0x3, 0x2, 0x2, 0x2, 0x141, 0x144, 
    0x3, 0x2, 0x2, 0x2, 0x142, 0x140, 0x3, 0x2, 0x2, 0x2, 0x143, 0x138, 
    0x3, 0x2, 0x2, 0x2, 0x143, 0x144, 0x3, 0x2, 0x2, 0x2, 0x144, 0x145, 
    0x3, 0x2, 0x2, 0x2, 0x145, 0x146, 0x7, 0x29, 0x2, 0x2, 0x146, 0x147, 
    0x8, 0x10, 0x1, 0x2, 0x147, 0x1f, 0x3, 0x2, 0x2, 0x2, 0x148, 0x14c, 
    0x7, 0x2a, 0x2, 0x2, 0x149, 0x14b, 0x5, 0x20, 0x11, 0x2, 0x14a, 0x149, 
    0x3, 0x2, 0x2, 0x2, 0x14b, 0x14e, 0x3, 0x2, 0x2, 0x2, 0x14c, 0x14a, 
    0x3, 0x2, 0x2, 0x2, 0x14c, 0x14d, 0x3, 0x2, 0x2, 0x2, 0x14d, 0x14f, 
    0x3, 0x2, 0x2, 0x2, 0x14e, 0x14c, 0x3, 0x2, 0x2, 0x2, 0x14f, 0x152, 
    0x7, 0x2b, 0x2, 0x2, 0x150, 0x152, 0xa, 0x4, 0x2, 0x2, 0x151, 0x148, 
    0x3, 0x2, 0x2, 0x2, 0x151, 0x150, 0x3, 0x2, 0x2, 0x2, 0x152, 0x21, 0x3, 
    0x2, 0x2, 0x2, 0x153, 0x154, 0x5, 0x1e, 0x10, 0x2, 0x154, 0x155, 0x8, 
    0x12, 0x1, 0x2, 0x155, 0x156, 0x8, 0x12, 0x1, 0x2, 0x156, 0x157, 0x5, 
    0x34, 0x1b, 0x2, 0x157, 0x158, 0x8, 0x12, 0x1, 0x2, 0x158, 0x23, 0x3, 
    0x2, 0x2, 0x2, 0x159, 0x15a, 0x5, 0xe, 0x8, 0x2, 0x15a, 0x15b, 0x8, 
    0x13, 0x1, 0x2, 0x15b, 0x15d, 0x3, 0x2, 0x2, 0x2, 0x15c, 0x159, 0x3, 
    0x2, 0x2, 0x2, 0x15d, 0x160, 0x3, 0x2, 0x2, 0x2, 0x15e, 0x15c, 0x3, 
    0x2, 0x2, 0x2, 0x15e, 0x15f, 0x3, 0x2, 0x2, 0x2, 0x15f, 0x161, 0x3, 
    0x2, 0x2, 0x2, 0x160, 0x15e, 0x3, 0x2, 0x2, 0x2, 0x161, 0x162, 0x7, 
    0xa, 0x2, 0x2, 0x162, 0x163, 0x7, 0x55, 0x2, 0x2, 0x163, 0x164, 0x8, 
    0x13, 0x1, 0x2, 0x164, 0x16b, 0x7, 0x2a, 0x2, 0x2, 0x165, 0x166, 0x5, 
    0x3e, 0x20, 0x2, 0x166, 0x167, 0x8, 0x13, 0x1, 0x2, 0x167, 0x168, 0x7, 
    0x25, 0x2, 0x2, 0x168, 0x16a, 0x3, 0x2, 0x2, 0x2, 0x169, 0x165, 0x3, 
    0x2, 0x2, 0x2, 0x16a, 0x16d, 0x3, 0x2, 0x2, 0x2, 0x16b, 0x169, 0x3, 
    0x2, 0x2, 0x2, 0x16b, 0x16c, 0x3, 0x2, 0x2, 0x2, 0x16c, 0x16e, 0x3, 
    0x2, 0x2, 0x2, 0x16d, 0x16b, 0x3, 0x2, 0x2, 0x2, 0x16e, 0x16f, 0x7, 
    0x2b, 0x2, 0x2, 0x16f, 0x170, 0x8, 0x13, 0x1, 0x2, 0x170, 0x25, 0x3, 
    0x2, 0x2, 0x2, 0x171, 0x172, 0x7, 0xb, 0x2, 0x2, 0x172, 0x176, 0x8, 
    0x14, 0x1, 0x2, 0x173, 0x174, 0x7, 0xc, 0x2, 0x2, 0x174, 0x176, 0x8, 
    0x14, 0x1, 0x2, 0x175, 0x171, 0x3, 0x2, 0x2, 0x2, 0x175, 0x173, 0x3, 
    0x2, 0x2, 0x2, 0x176, 0x177, 0x3, 0x2, 0x2, 0x2, 0x177, 0x178, 0x7, 
    0x55, 0x2, 0x2, 0x178, 0x179, 0x8, 0x14, 0x1, 0x2, 0x179, 0x180, 0x7, 
    0x2a, 0x2, 0x2, 0x17a, 0x17b, 0x5, 0x3e, 0x20, 0x2, 0x17b, 0x17c, 0x8, 
    0x14, 0x1, 0x2, 0x17c, 0x17d, 0x7, 0x25, 0x2, 0x2, 0x17d, 0x17f, 0x3, 
    0x2, 0x2, 0x2, 0x17e, 0x17a, 0x3, 0x2, 0x2, 0x2, 0x17f, 0x182, 0x3, 
    0x2, 0x2, 0x2, 0x180, 0x17e, 0x3, 0x2, 0x2, 0x2, 0x180, 0x181, 0x3, 
    0x2, 0x2, 0x2, 0x181, 0x183, 0x3, 0x2, 0x2, 0x2, 0x182, 0x180, 0x3, 
    0x2, 0x2, 0x2, 0x183, 0x184, 0x7, 0x2b, 0x2, 0x2, 0x184, 0x185, 0x8, 
    0x14, 0x1, 0x2, 0x185, 0x27, 0x3, 0x2, 0x2, 0x2, 0x186, 0x187, 0x5, 
    0x2c, 0x17, 0x2, 0x187, 0x188, 0x8, 0x15, 0x1, 0x2, 0x188, 0x1a0, 0x3, 
    0x2, 0x2, 0x2, 0x189, 0x18a, 0x5, 0x34, 0x1b, 0x2, 0x18a, 0x18b, 0x8, 
    0x15, 0x1, 0x2, 0x18b, 0x1a0, 0x3, 0x2, 0x2, 0x2, 0x18c, 0x18d, 0x5, 
    0x2e, 0x18, 0x2, 0x18d, 0x18e, 0x8, 0x15, 0x1, 0x2, 0x18e, 0x1a0, 0x3, 
    0x2, 0x2, 0x2, 0x18f, 0x190, 0x5, 0x32, 0x1a, 0x2, 0x190, 0x191, 0x8, 
    0x15, 0x1, 0x2, 0x191, 0x1a0, 0x3, 0x2, 0x2, 0x2, 0x192, 0x193, 0x5, 
    0x36, 0x1c, 0x2, 0x193, 0x194, 0x8, 0x15, 0x1, 0x2, 0x194, 0x1a0, 0x3, 
    0x2, 0x2, 0x2, 0x195, 0x196, 0x5, 0x38, 0x1d, 0x2, 0x196, 0x197, 0x8, 
    0x15, 0x1, 0x2, 0x197, 0x1a0, 0x3, 0x2, 0x2, 0x2, 0x198, 0x199, 0x5, 
    0x3c, 0x1f, 0x2, 0x199, 0x19a, 0x8, 0x15, 0x1, 0x2, 0x19a, 0x1a0, 0x3, 
    0x2, 0x2, 0x2, 0x19b, 0x19c, 0x5, 0x2a, 0x16, 0x2, 0x19c, 0x19d, 0x7, 
    0x25, 0x2, 0x2, 0x19d, 0x19e, 0x8, 0x15, 0x1, 0x2, 0x19e, 0x1a0, 0x3, 
    0x2, 0x2, 0x2, 0x19f, 0x186, 0x3, 0x2, 0x2, 0x2, 0x19f, 0x189, 0x3, 
    0x2, 0x2, 0x2, 0x19f, 0x18c, 0x3, 0x2, 0x2, 0x2, 0x19f, 0x18f, 0x3, 
    0x2, 0x2, 0x2, 0x19f, 0x192, 0x3, 0x2, 0x2, 0x2, 0x19f, 0x195, 0x3, 
    0x2, 0x2, 0x2, 0x19f, 0x198, 0x3, 0x2, 0x2, 0x2, 0x19f, 0x19b, 0x3, 
    0x2, 0x2, 0x2, 0x1a0, 0x29, 0x3, 0x2, 0x2, 0x2, 0x1a1, 0x1a2, 0x5, 0x3e, 
    0x20, 0x2, 0x1a2, 0x1a3, 0x8, 0x16, 0x1, 0x2, 0x1a3, 0x2b, 0x3, 0x2, 
    0x2, 0x2, 0x1a4, 0x1a5, 0x7, 0xd, 0x2, 0x2, 0x1a5, 0x1a6, 0x8, 0x17, 
    0x1, 0x2, 0x1a6, 0x1a7, 0x7, 0x28, 0x2, 0x2, 0x1a7, 0x1a8, 0x5, 0x3e, 
    0x20, 0x2, 0x1a8, 0x1a9, 0x8, 0x17, 0x1, 0x2, 0x1a9, 0x1aa, 0x7, 0x29, 
    0x2, 0x2, 0x1aa, 0x1ab, 0x5, 0x28, 0x15, 0x2, 0x1ab, 0x1b0, 0x8, 0x17, 
    0x1, 0x2, 0x1ac, 0x1ad, 0x7, 0xe, 0x2, 0x2, 0x1ad, 0x1ae, 0x5, 0x28, 
    0x15, 0x2, 0x1ae, 0x1af, 0x8, 0x17, 0x1, 0x2, 0x1af, 0x1b1, 0x3, 0x2, 
    0x2, 0x2, 0x1b0, 0x1ac, 0x3, 0x2, 0x2, 0x2, 0x1b0, 0x1b1, 0x3, 0x2, 
    0x2, 0x2, 0x1b1, 0x1b2, 0x3, 0x2, 0x2, 0x2, 0x1b2, 0x1b3, 0x8, 0x17, 
    0x1, 0x2, 0x1b3, 0x2d, 0x3, 0x2, 0x2, 0x2, 0x1b4, 0x1b5, 0x7, 0xf, 0x2, 
    0x2, 0x1b5, 0x1b6, 0x8, 0x18, 0x1, 0x2, 0x1b6, 0x1ba, 0x7, 0x28, 0x2, 
    0x2, 0x1b7, 0x1b8, 0x5, 0x14, 0xb, 0x2, 0x1b8, 0x1b9, 0x8, 0x18, 0x1, 
    0x2, 0x1b9, 0x1bb, 0x3, 0x2, 0x2, 0x2, 0x1ba, 0x1b7, 0x3, 0x2, 0x2, 
    0x2, 0x1ba, 0x1bb, 0x3, 0x2, 0x2, 0x2, 0x1bb, 0x1bc, 0x3, 0x2, 0x2, 
    0x2, 0x1bc, 0x1c0, 0x7, 0x25, 0x2, 0x2, 0x1bd, 0x1be, 0x5, 0x3e, 0x20, 
    0x2, 0x1be, 0x1bf, 0x8, 0x18, 0x1, 0x2, 0x1bf, 0x1c1, 0x3, 0x2, 0x2, 
    0x2, 0x1c0, 0x1bd, 0x3, 0x2, 0x2, 0x2, 0x1c0, 0x1c1, 0x3, 0x2, 0x2, 
    0x2, 0x1c1, 0x1c2, 0x3, 0x2, 0x2, 0x2, 0x1c2, 0x1c6, 0x7, 0x25, 0x2, 
    0x2, 0x1c3, 0x1c4, 0x5, 0x3e, 0x20, 0x2, 0x1c4, 0x1c5, 0x8, 0x18, 0x1, 
    0x2, 0x1c5, 0x1c7, 0x3, 0x2, 0x2, 0x2, 0x1c6, 0x1c3, 0x3, 0x2, 0x2, 
    0x2, 0x1c6, 0x1c7, 0x3, 0x2, 0x2, 0x2, 0x1c7, 0x1c8, 0x3, 0x2, 0x2, 
    0x2, 0x1c8, 0x1c9, 0x7, 0x29, 0x2, 0x2, 0x1c9, 0x1ca, 0x5, 0x28, 0x15, 
    0x2, 0x1ca, 0x1cb, 0x8, 0x18, 0x1, 0x2, 0x1cb, 0x1cc, 0x8, 0x18, 0x1, 
    0x2, 0x1cc, 0x2f, 0x3, 0x2, 0x2, 0x2, 0x1cd, 0x1ce, 0x7, 0xf, 0x2, 0x2, 
    0x1ce, 0x1cf, 0x8, 0x19, 0x1, 0x2, 0x1cf, 0x1d0, 0x7, 0x28, 0x2, 0x2, 
    0x1d0, 0x1d1, 0x7, 0x55, 0x2, 0x2, 0x1d1, 0x1d2, 0x7, 0x27, 0x2, 0x2, 
    0x1d2, 0x1d3, 0x7, 0x55, 0x2, 0x2, 0x1d3, 0x1d4, 0x7, 0x10, 0x2, 0x2, 
    0x1d4, 0x1d5, 0x7, 0x55, 0x2, 0x2, 0x1d5, 0x1d6, 0x7, 0x29, 0x2, 0x2, 
    0x1d6, 0x1d7, 0x5, 0x28, 0x15, 0x2, 0x1d7, 0x1d8, 0x8, 0x19, 0x1, 0x2, 
    0x1d8, 0x1d9, 0x8, 0x19, 0x1, 0x2, 0x1d9, 0x31, 0x3, 0x2, 0x2, 0x2, 
    0x1da, 0x1db, 0x7, 0x11, 0x2, 0x2, 0x1db, 0x1dc, 0x8, 0x1a, 0x1, 0x2, 
    0x1dc, 0x1dd, 0x7, 0x28, 0x2, 0x2, 0x1dd, 0x1de, 0x5, 0x3e, 0x20, 0x2, 
    0x1de, 0x1df, 0x8, 0x1a, 0x1, 0x2, 0x1df, 0x1e0, 0x7, 0x29, 0x2, 0x2, 
    0x1e0, 0x1e1, 0x5, 0x28, 0x15, 0x2, 0x1e1, 0x1e2, 0x8, 0x1a, 0x1, 0x2, 
    0x1e2, 0x1e3, 0x8, 0x1a, 0x1, 0x2, 0x1e3, 0x1f1, 0x3, 0x2, 0x2, 0x2, 
    0x1e4, 0x1e5, 0x7, 0x12, 0x2, 0x2, 0x1e5, 0x1e6, 0x8, 0x1a, 0x1, 0x2, 
    0x1e6, 0x1e7, 0x5, 0x28, 0x15, 0x2, 0x1e7, 0x1e8, 0x8, 0x1a, 0x1, 0x2, 
    0x1e8, 0x1e9, 0x7, 0x11, 0x2, 0x2, 0x1e9, 0x1ea, 0x7, 0x28, 0x2, 0x2, 
    0x1ea, 0x1eb, 0x5, 0x3e, 0x20, 0x2, 0x1eb, 0x1ec, 0x8, 0x1a, 0x1, 0x2, 
    0x1ec, 0x1ed, 0x7, 0x29, 0x2, 0x2, 0x1ed, 0x1ee, 0x7, 0x25, 0x2, 0x2, 
    0x1ee, 0x1ef, 0x8, 0x1a, 0x1, 0x2, 0x1ef, 0x1f1, 0x3, 0x2, 0x2, 0x2, 
    0x1f0, 0x1da, 0x3, 0x2, 0x2, 0x2, 0x1f0, 0x1e4, 0x3, 0x2, 0x2, 0x2, 
    0x1f1, 0x33, 0x3, 0x2, 0x2, 0x2, 0x1f2, 0x1f3, 0x7, 0x2a, 0x2, 0x2, 
    0x1f3, 0x1fd, 0x8, 0x1b, 0x1, 0x2, 0x1f4, 0x1f5, 0x5, 0x28, 0x15, 0x2, 
    0x1f5, 0x1f6, 0x8, 0x1b, 0x1, 0x2, 0x1f6, 0x1fc, 0x3, 0x2, 0x2, 0x2, 
    0x1f7, 0x1f8, 0x5, 0x14, 0xb, 0x2, 0x1f8, 0x1f9, 0x7, 0x25, 0x2, 0x2, 
    0x1f9, 0x1fa, 0x8, 0x1b, 0x1, 0x2, 0x1fa, 0x1fc, 0x3, 0x2, 0x2, 0x2, 
    0x1fb, 0x1f4, 0x3, 0x2, 0x2, 0x2, 0x1fb, 0x1f7, 0x3, 0x2, 0x2, 0x2, 
    0x1fc, 0x1ff, 0x3, 0x2, 0x2, 0x2, 0x1fd, 0x1fb, 0x3, 0x2, 0x2, 0x2, 
    0x1fd, 0x1fe, 0x3, 0x2, 0x2, 0x2, 0x1fe, 0x200, 0x3, 0x2, 0x2, 0x2, 
    0x1ff, 0x1fd, 0x3, 0x2, 0x2, 0x2, 0x200, 0x201, 0x7, 0x2b, 0x2, 0x2, 
    0x201, 0x202, 0x8, 0x1b, 0x1, 0x2, 0x202, 0x35, 0x3, 0x2, 0x2, 0x2, 
    0x203, 0x204, 0x7, 0x13, 0x2, 0x2, 0x204, 0x208, 0x8, 0x1c, 0x1, 0x2, 
    0x205, 0x206, 0x5, 0x3e, 0x20, 0x2, 0x206, 0x207, 0x8, 0x1c, 0x1, 0x2, 
    0x207, 0x209, 0x3, 0x2, 0x2, 0x2, 0x208, 0x205, 0x3, 0x2, 0x2, 0x2, 
    0x208, 0x209, 0x3, 0x2, 0x2, 0x2, 0x209, 0x20a, 0x3, 0x2, 0x2, 0x2, 
    0x20a, 0x20b, 0x7, 0x25, 0x2, 0x2, 0x20b, 0x20c, 0x8, 0x1c, 0x1, 0x2, 
    0x20c, 0x37, 0x3, 0x2, 0x2, 0x2, 0x20d, 0x20e, 0x7, 0x14, 0x2, 0x2, 
    0x20e, 0x20f, 0x8, 0x1d, 0x1, 0x2, 0x20f, 0x210, 0x7, 0x25, 0x2, 0x2, 
    0x210, 0x211, 0x8, 0x1d, 0x1, 0x2, 0x211, 0x39, 0x3, 0x2, 0x2, 0x2, 
    0x212, 0x213, 0x7, 0x15, 0x2, 0x2, 0x213, 0x214, 0x8, 0x1e, 0x1, 0x2, 
    0x214, 0x215, 0x7, 0x28, 0x2, 0x2, 0x215, 0x216, 0x5, 0x3e, 0x20, 0x2, 
    0x216, 0x217, 0x7, 0x29, 0x2, 0x2, 0x217, 0x218, 0x8, 0x1e, 0x1, 0x2, 
    0x218, 0x221, 0x7, 0x2a, 0x2, 0x2, 0x219, 0x21a, 0x7, 0x16, 0x2, 0x2, 
    0x21a, 0x21b, 0x7, 0x55, 0x2, 0x2, 0x21b, 0x21c, 0x7, 0x27, 0x2, 0x2, 
    0x21c, 0x21d, 0x5, 0x28, 0x15, 0x2, 0x21d, 0x21e, 0x8, 0x1e, 0x1, 0x2, 
    0x21e, 0x220, 0x3, 0x2, 0x2, 0x2, 0x21f, 0x219, 0x3, 0x2, 0x2, 0x2, 
    0x220, 0x223, 0x3, 0x2, 0x2, 0x2, 0x221, 0x21f, 0x3, 0x2, 0x2, 0x2, 
    0x221, 0x222, 0x3, 0x2, 0x2, 0x2, 0x222, 0x229, 0x3, 0x2, 0x2, 0x2, 
    0x223, 0x221, 0x3, 0x2, 0x2, 0x2, 0x224, 0x225, 0x7, 0x17, 0x2, 0x2, 
    0x225, 0x226, 0x7, 0x27, 0x2, 0x2, 0x226, 0x227, 0x5, 0x28, 0x15, 0x2, 
    0x227, 0x228, 0x8, 0x1e, 0x1, 0x2, 0x228, 0x22a, 0x3, 0x2, 0x2, 0x2, 
    0x229, 0x224, 0x3, 0x2, 0x2, 0x2, 0x229, 0x22a, 0x3, 0x2, 0x2, 0x2, 
    0x22a, 0x22b, 0x3, 0x2, 0x2, 0x2, 0x22b, 0x22c, 0x7, 0x2b, 0x2, 0x2, 
    0x22c, 0x22d, 0x8, 0x1e, 0x1, 0x2, 0x22d, 0x3b, 0x3, 0x2, 0x2, 0x2, 
    0x22e, 0x22f, 0x7, 0x18, 0x2, 0x2, 0x22f, 0x230, 0x7, 0x25, 0x2, 0x2, 
    0x230, 0x231, 0x8, 0x1f, 0x1, 0x2, 0x231, 0x3d, 0x3, 0x2, 0x2, 0x2, 
    0x232, 0x233, 0x5, 0x40, 0x21, 0x2, 0x233, 0x234, 0x8, 0x20, 0x1, 0x2, 
    0x234, 0x3f, 0x3, 0x2, 0x2, 0x2, 0x235, 0x236, 0x5, 0x42, 0x22, 0x2, 
    0x236, 0x23d, 0x8, 0x21, 0x1, 0x2, 0x237, 0x238, 0x7, 0x26, 0x2, 0x2, 
    0x238, 0x239, 0x5, 0x42, 0x22, 0x2, 0x239, 0x23a, 0x8, 0x21, 0x1, 0x2, 
    0x23a, 0x23c, 0x3, 0x2, 0x2, 0x2, 0x23b, 0x237, 0x3, 0x2, 0x2, 0x2, 
    0x23c, 0x23f, 0x3, 0x2, 0x2, 0x2, 0x23d, 0x23b, 0x3, 0x2, 0x2, 0x2, 
    0x23d, 0x23e, 0x3, 0x2, 0x2, 0x2, 0x23e, 0x41, 0x3, 0x2, 0x2, 0x2, 0x23f, 
    0x23d, 0x3, 0x2, 0x2, 0x2, 0x240, 0x241, 0x5, 0x44, 0x23, 0x2, 0x241, 
    0x24e, 0x8, 0x22, 0x1, 0x2, 0x242, 0x243, 0x9, 0x5, 0x2, 0x2, 0x243, 
    0x244, 0x5, 0x44, 0x23, 0x2, 0x244, 0x245, 0x8, 0x22, 0x1, 0x2, 0x245, 
    0x24d, 0x3, 0x2, 0x2, 0x2, 0x246, 0x247, 0x7, 0x32, 0x2, 0x2, 0x247, 
    0x248, 0x5, 0x3e, 0x20, 0x2, 0x248, 0x249, 0x7, 0x27, 0x2, 0x2, 0x249, 
    0x24a, 0x5, 0x3e, 0x20, 0x2, 0x24a, 0x24b, 0x8, 0x22, 0x1, 0x2, 0x24b, 
    0x24d, 0x3, 0x2, 0x2, 0x2, 0x24c, 0x242, 0x3, 0x2, 0x2, 0x2, 0x24c, 
    0x246, 0x3, 0x2, 0x2, 0x2, 0x24d, 0x250, 0x3, 0x2, 0x2, 0x2, 0x24e, 
    0x24c, 0x3, 0x2, 0x2, 0x2, 0x24e, 0x24f, 0x3, 0x2, 0x2, 0x2, 0x24f, 
    0x43, 0x3, 0x2, 0x2, 0x2, 0x250, 0x24e, 0x3, 0x2, 0x2, 0x2, 0x251, 0x252, 
    0x5, 0x46, 0x24, 0x2, 0x252, 0x259, 0x8, 0x23, 0x1, 0x2, 0x253, 0x254, 
    0x7, 0x44, 0x2, 0x2, 0x254, 0x255, 0x5, 0x46, 0x24, 0x2, 0x255, 0x256, 
    0x8, 0x23, 0x1, 0x2, 0x256, 0x258, 0x3, 0x2, 0x2, 0x2, 0x257, 0x253, 
    0x3, 0x2, 0x2, 0x2, 0x258, 0x25b, 0x3, 0x2, 0x2, 0x2, 0x259, 0x257, 
    0x3, 0x2, 0x2, 0x2, 0x259, 0x25a, 0x3, 0x2, 0x2, 0x2, 0x25a, 0x45, 0x3, 
    0x2, 0x2, 0x2, 0x25b, 0x259, 0x3, 0x2, 0x2, 0x2, 0x25c, 0x25d, 0x5, 
    0x48, 0x25, 0x2, 0x25d, 0x264, 0x8, 0x24, 0x1, 0x2, 0x25e, 0x25f, 0x7, 
    0x43, 0x2, 0x2, 0x25f, 0x260, 0x5, 0x48, 0x25, 0x2, 0x260, 0x261, 0x8, 
    0x24, 0x1, 0x2, 0x261, 0x263, 0x3, 0x2, 0x2, 0x2, 0x262, 0x25e, 0x3, 
    0x2, 0x2, 0x2, 0x263, 0x266, 0x3, 0x2, 0x2, 0x2, 0x264, 0x262, 0x3, 
    0x2, 0x2, 0x2, 0x264, 0x265, 0x3, 0x2, 0x2, 0x2, 0x265, 0x47, 0x3, 0x2, 
    0x2, 0x2, 0x266, 0x264, 0x3, 0x2, 0x2, 0x2, 0x267, 0x268, 0x5, 0x4a, 
    0x26, 0x2, 0x268, 0x26f, 0x8, 0x25, 0x1, 0x2, 0x269, 0x26a, 0x7, 0x35, 
    0x2, 0x2, 0x26a, 0x26b, 0x5, 0x4a, 0x26, 0x2, 0x26b, 0x26c, 0x8, 0x25, 
    0x1, 0x2, 0x26c, 0x26e, 0x3, 0x2, 0x2, 0x2, 0x26d, 0x269, 0x3, 0x2, 
    0x2, 0x2, 0x26e, 0x271, 0x3, 0x2, 0x2, 0x2, 0x26f, 0x26d, 0x3, 0x2, 
    0x2, 0x2, 0x26f, 0x270, 0x3, 0x2, 0x2, 0x2, 0x270, 0x49, 0x3, 0x2, 0x2, 
    0x2, 0x271, 0x26f, 0x3, 0x2, 0x2, 0x2, 0x272, 0x273, 0x5, 0x4c, 0x27, 
    0x2, 0x273, 0x27a, 0x8, 0x26, 0x1, 0x2, 0x274, 0x275, 0x7, 0x37, 0x2, 
    0x2, 0x275, 0x276, 0x5, 0x4c, 0x27, 0x2, 0x276, 0x277, 0x8, 0x26, 0x1, 
    0x2, 0x277, 0x279, 0x3, 0x2, 0x2, 0x2, 0x278, 0x274, 0x3, 0x2, 0x2, 
    0x2, 0x279, 0x27c, 0x3, 0x2, 0x2, 0x2, 0x27a, 0x278, 0x3, 0x2, 0x2, 
    0x2, 0x27a, 0x27b, 0x3, 0x2, 0x2, 0x2, 0x27b, 0x4b, 0x3, 0x2, 0x2, 0x2, 
    0x27c, 0x27a, 0x3, 0x2, 0x2, 0x2, 0x27d, 0x27e, 0x5, 0x4e, 0x28, 0x2, 
    0x27e, 0x285, 0x8, 0x27, 0x1, 0x2, 0x27f, 0x280, 0x7, 0x33, 0x2, 0x2, 
    0x280, 0x281, 0x5, 0x4e, 0x28, 0x2, 0x281, 0x282, 0x8, 0x27, 0x1, 0x2, 
    0x282, 0x284, 0x3, 0x2, 0x2, 0x2, 0x283, 0x27f, 0x3, 0x2, 0x2, 0x2, 
    0x284, 0x287, 0x3, 0x2, 0x2, 0x2, 0x285, 0x283, 0x3, 0x2, 0x2, 0x2, 
    0x285, 0x286, 0x3, 0x2, 0x2, 0x2, 0x286, 0x4d, 0x3, 0x2, 0x2, 0x2, 0x287, 
    0x285, 0x3, 0x2, 0x2, 0x2, 0x288, 0x289, 0x5, 0x50, 0x29, 0x2, 0x289, 
    0x290, 0x8, 0x28, 0x1, 0x2, 0x28a, 0x28b, 0x9, 0x6, 0x2, 0x2, 0x28b, 
    0x28c, 0x5, 0x50, 0x29, 0x2, 0x28c, 0x28d, 0x8, 0x28, 0x1, 0x2, 0x28d, 
    0x28f, 0x3, 0x2, 0x2, 0x2, 0x28e, 0x28a, 0x3, 0x2, 0x2, 0x2, 0x28f, 
    0x292, 0x3, 0x2, 0x2, 0x2, 0x290, 0x28e, 0x3, 0x2, 0x2, 0x2, 0x290, 
    0x291, 0x3, 0x2, 0x2, 0x2, 0x291, 0x4f, 0x3, 0x2, 0x2, 0x2, 0x292, 0x290, 
    0x3, 0x2, 0x2, 0x2, 0x293, 0x294, 0x5, 0x52, 0x2a, 0x2, 0x294, 0x29b, 
    0x8, 0x29, 0x1, 0x2, 0x295, 0x296, 0x9, 0x7, 0x2, 0x2, 0x296, 0x297, 
    0x5, 0x52, 0x2a, 0x2, 0x297, 0x298, 0x8, 0x29, 0x1, 0x2, 0x298, 0x29a, 
    0x3, 0x2, 0x2, 0x2, 0x299, 0x295, 0x3, 0x2, 0x2, 0x2, 0x29a, 0x29d, 
    0x3, 0x2, 0x2, 0x2, 0x29b, 0x299, 0x3, 0x2, 0x2, 0x2, 0x29b, 0x29c, 
    0x3, 0x2, 0x2, 0x2, 0x29c, 0x51, 0x3, 0x2, 0x2, 0x2, 0x29d, 0x29b, 0x3, 
    0x2, 0x2, 0x2, 0x29e, 0x29f, 0x5, 0x54, 0x2b, 0x2, 0x29f, 0x2a6, 0x8, 
    0x2a, 0x1, 0x2, 0x2a0, 0x2a1, 0x9, 0x8, 0x2, 0x2, 0x2a1, 0x2a2, 0x5, 
    0x54, 0x2b, 0x2, 0x2a2, 0x2a3, 0x8, 0x2a, 0x1, 0x2, 0x2a3, 0x2a5, 0x3, 
    0x2, 0x2, 0x2, 0x2a4, 0x2a0, 0x3, 0x2, 0x2, 0x2, 0x2a5, 0x2a8, 0x3, 
    0x2, 0x2, 0x2, 0x2a6, 0x2a4, 0x3, 0x2, 0x2, 0x2, 0x2a6, 0x2a7, 0x3, 
    0x2, 0x2, 0x2, 0x2a7, 0x53, 0x3, 0x2, 0x2, 0x2, 0x2a8, 0x2a6, 0x3, 0x2, 
    0x2, 0x2, 0x2a9, 0x2aa, 0x5, 0x56, 0x2c, 0x2, 0x2aa, 0x2b1, 0x8, 0x2b, 
    0x1, 0x2, 0x2ab, 0x2ac, 0x9, 0x9, 0x2, 0x2, 0x2ac, 0x2ad, 0x5, 0x56, 
    0x2c, 0x2, 0x2ad, 0x2ae, 0x8, 0x2b, 0x1, 0x2, 0x2ae, 0x2b0, 0x3, 0x2, 
    0x2, 0x2, 0x2af, 0x2ab, 0x3, 0x2, 0x2, 0x2, 0x2b0, 0x2b3, 0x3, 0x2, 
    0x2, 0x2, 0x2b1, 0x2af, 0x3, 0x2, 0x2, 0x2, 0x2b1, 0x2b2, 0x3, 0x2, 
    0x2, 0x2, 0x2b2, 0x55, 0x3, 0x2, 0x2, 0x2, 0x2b3, 0x2b1, 0x3, 0x2, 0x2, 
    0x2, 0x2b4, 0x2b5, 0x5, 0x58, 0x2d, 0x2, 0x2b5, 0x2bc, 0x8, 0x2c, 0x1, 
    0x2, 0x2b6, 0x2b7, 0x9, 0xa, 0x2, 0x2, 0x2b7, 0x2b8, 0x5, 0x58, 0x2d, 
    0x2, 0x2b8, 0x2b9, 0x8, 0x2c, 0x1, 0x2, 0x2b9, 0x2bb, 0x3, 0x2, 0x2, 
    0x2, 0x2ba, 0x2b6, 0x3, 0x2, 0x2, 0x2, 0x2bb, 0x2be, 0x3, 0x2, 0x2, 
    0x2, 0x2bc, 0x2ba, 0x3, 0x2, 0x2, 0x2, 0x2bc, 0x2bd, 0x3, 0x2, 0x2, 
    0x2, 0x2bd, 0x57, 0x3, 0x2, 0x2, 0x2, 0x2be, 0x2bc, 0x3, 0x2, 0x2, 0x2, 
    0x2bf, 0x2c0, 0x9, 0xb, 0x2, 0x2, 0x2c0, 0x2c2, 0x8, 0x2d, 0x1, 0x2, 
    0x2c1, 0x2bf, 0x3, 0x2, 0x2, 0x2, 0x2c2, 0x2c5, 0x3, 0x2, 0x2, 0x2, 
    0x2c3, 0x2c1, 0x3, 0x2, 0x2, 0x2, 0x2c3, 0x2c4, 0x3, 0x2, 0x2, 0x2, 
    0x2c4, 0x2c6, 0x3, 0x2, 0x2, 0x2, 0x2c5, 0x2c3, 0x3, 0x2, 0x2, 0x2, 
    0x2c6, 0x2c7, 0x5, 0x5a, 0x2e, 0x2, 0x2c7, 0x2c8, 0x8, 0x2d, 0x1, 0x2, 
    0x2c8, 0x59, 0x3, 0x2, 0x2, 0x2, 0x2c9, 0x2ce, 0x5, 0x5e, 0x30, 0x2, 
    0x2ca, 0x2cb, 0x9, 0xc, 0x2, 0x2, 0x2cb, 0x2cd, 0x8, 0x2e, 0x1, 0x2, 
    0x2cc, 0x2ca, 0x3, 0x2, 0x2, 0x2, 0x2cd, 0x2d0, 0x3, 0x2, 0x2, 0x2, 
    0x2ce, 0x2cc, 0x3, 0x2, 0x2, 0x2, 0x2ce, 0x2cf, 0x3, 0x2, 0x2, 0x2, 
    0x2cf, 0x2d1, 0x3, 0x2, 0x2, 0x2, 0x2d0, 0x2ce, 0x3, 0x2, 0x2, 0x2, 
    0x2d1, 0x2d2, 0x8, 0x2e, 0x1, 0x2, 0x2d2, 0x2ff, 0x3, 0x2, 0x2, 0x2, 
    0x2d3, 0x2d4, 0x5, 0x5e, 0x30, 0x2, 0x2d4, 0x2fb, 0x8, 0x2e, 0x1, 0x2, 
    0x2d5, 0x2d6, 0x7, 0x28, 0x2, 0x2, 0x2d6, 0x2e2, 0x8, 0x2e, 0x1, 0x2, 
    0x2d7, 0x2d8, 0x5, 0x44, 0x23, 0x2, 0x2d8, 0x2df, 0x8, 0x2e, 0x1, 0x2, 
    0x2d9, 0x2da, 0x7, 0x26, 0x2, 0x2, 0x2da, 0x2db, 0x5, 0x44, 0x23, 0x2, 
    0x2db, 0x2dc, 0x8, 0x2e, 0x1, 0x2, 0x2dc, 0x2de, 0x3, 0x2, 0x2, 0x2, 
    0x2dd, 0x2d9, 0x3, 0x2, 0x2, 0x2, 0x2de, 0x2e1, 0x3, 0x2, 0x2, 0x2, 
    0x2df, 0x2dd, 0x3, 0x2, 0x2, 0x2, 0x2df, 0x2e0, 0x3, 0x2, 0x2, 0x2, 
    0x2e0, 0x2e3, 0x3, 0x2, 0x2, 0x2, 0x2e1, 0x2df, 0x3, 0x2, 0x2, 0x2, 
    0x2e2, 0x2d7, 0x3, 0x2, 0x2, 0x2, 0x2e2, 0x2e3, 0x3, 0x2, 0x2, 0x2, 
    0x2e3, 0x2e4, 0x3, 0x2, 0x2, 0x2, 0x2e4, 0x2e5, 0x7, 0x29, 0x2, 0x2, 
    0x2e5, 0x2fa, 0x8, 0x2e, 0x1, 0x2, 0x2e6, 0x2e7, 0x7, 0x2e, 0x2, 0x2, 
    0x2e7, 0x2e8, 0x8, 0x2e, 0x1, 0x2, 0x2e8, 0x2e9, 0x5, 0x5a, 0x2e, 0x2, 
    0x2e9, 0x2ea, 0x8, 0x2e, 0x1, 0x2, 0x2ea, 0x2fa, 0x3, 0x2, 0x2, 0x2, 
    0x2eb, 0x2ec, 0x7, 0x4c, 0x2, 0x2, 0x2ec, 0x2ed, 0x8, 0x2e, 0x1, 0x2, 
    0x2ed, 0x2ee, 0x5, 0x5a, 0x2e, 0x2, 0x2ee, 0x2ef, 0x8, 0x2e, 0x1, 0x2, 
    0x2ef, 0x2fa, 0x3, 0x2, 0x2, 0x2, 0x2f0, 0x2f1, 0x7, 0x2c, 0x2, 0x2, 
    0x2f1, 0x2f5, 0x8, 0x2e, 0x1, 0x2, 0x2f2, 0x2f3, 0x5, 0x3e, 0x20, 0x2, 
    0x2f3, 0x2f4, 0x8, 0x2e, 0x1, 0x2, 0x2f4, 0x2f6, 0x3, 0x2, 0x2, 0x2, 
    0x2f5, 0x2f2, 0x3, 0x2, 0x2, 0x2, 0x2f5, 0x2f6, 0x3, 0x2, 0x2, 0x2, 
    0x2f6, 0x2f7, 0x3, 0x2, 0x2, 0x2, 0x2f7, 0x2f8, 0x7, 0x2d, 0x2, 0x2, 
    0x2f8, 0x2fa, 0x8, 0x2e, 0x1, 0x2, 0x2f9, 0x2d5, 0x3, 0x2, 0x2, 0x2, 
    0x2f9, 0x2e6, 0x3, 0x2, 0x2, 0x2, 0x2f9, 0x2eb, 0x3, 0x2, 0x2, 0x2, 
    0x2f9, 0x2f0, 0x3, 0x2, 0x2, 0x2, 0x2fa, 0x2fd, 0x3, 0x2, 0x2, 0x2, 
    0x2fb, 0x2f9, 0x3, 0x2, 0x2, 0x2, 0x2fb, 0x2fc, 0x3, 0x2, 0x2, 0x2, 
    0x2fc, 0x2ff, 0x3, 0x2, 0x2, 0x2, 0x2fd, 0x2fb, 0x3, 0x2, 0x2, 0x2, 
    0x2fe, 0x2c9, 0x3, 0x2, 0x2, 0x2, 0x2fe, 0x2d3, 0x3, 0x2, 0x2, 0x2, 
    0x2ff, 0x5b, 0x3, 0x2, 0x2, 0x2, 0x300, 0x301, 0x5, 0x5e, 0x30, 0x2, 
    0x301, 0x302, 0x7, 0x24, 0x2, 0x2, 0x302, 0x303, 0x5, 0x5e, 0x30, 0x2, 
    0x303, 0x304, 0x8, 0x2f, 0x1, 0x2, 0x304, 0x5d, 0x3, 0x2, 0x2, 0x2, 
    0x305, 0x306, 0x7, 0x4d, 0x2, 0x2, 0x306, 0x320, 0x8, 0x30, 0x1, 0x2, 
    0x307, 0x308, 0x7, 0x4e, 0x2, 0x2, 0x308, 0x320, 0x8, 0x30, 0x1, 0x2, 
    0x309, 0x30a, 0x7, 0x51, 0x2, 0x2, 0x30a, 0x320, 0x8, 0x30, 0x1, 0x2, 
    0x30b, 0x30c, 0x7, 0x53, 0x2, 0x2, 0x30c, 0x320, 0x8, 0x30, 0x1, 0x2, 
    0x30d, 0x30e, 0x7, 0x54, 0x2, 0x2, 0x30e, 0x320, 0x8, 0x30, 0x1, 0x2, 
    0x30f, 0x310, 0x5, 0x2, 0x2, 0x2, 0x310, 0x311, 0x8, 0x30, 0x1, 0x2, 
    0x311, 0x320, 0x3, 0x2, 0x2, 0x2, 0x312, 0x313, 0x7, 0x55, 0x2, 0x2, 
    0x313, 0x320, 0x8, 0x30, 0x1, 0x2, 0x314, 0x315, 0x5, 0x4, 0x3, 0x2, 
    0x315, 0x316, 0x8, 0x30, 0x1, 0x2, 0x316, 0x320, 0x3, 0x2, 0x2, 0x2, 
    0x317, 0x318, 0x5, 0x60, 0x31, 0x2, 0x318, 0x319, 0x8, 0x30, 0x1, 0x2, 
    0x319, 0x320, 0x3, 0x2, 0x2, 0x2, 0x31a, 0x31b, 0x7, 0x28, 0x2, 0x2, 
    0x31b, 0x31c, 0x5, 0x3e, 0x20, 0x2, 0x31c, 0x31d, 0x7, 0x29, 0x2, 0x2, 
    0x31d, 0x31e, 0x8, 0x30, 0x1, 0x2, 0x31e, 0x320, 0x3, 0x2, 0x2, 0x2, 
    0x31f, 0x305, 0x3, 0x2, 0x2, 0x2, 0x31f, 0x307, 0x3, 0x2, 0x2, 0x2, 
    0x31f, 0x309, 0x3, 0x2, 0x2, 0x2, 0x31f, 0x30b, 0x3, 0x2, 0x2, 0x2, 
    0x31f, 0x30d, 0x3, 0x2, 0x2, 0x2, 0x31f, 0x30f, 0x3, 0x2, 0x2, 0x2, 
    0x31f, 0x312, 0x3, 0x2, 0x2, 0x2, 0x31f, 0x314, 0x3, 0x2, 0x2, 0x2, 
    0x31f, 0x317, 0x3, 0x2, 0x2, 0x2, 0x31f, 0x31a, 0x3, 0x2, 0x2, 0x2, 
    0x320, 0x5f, 0x3, 0x2, 0x2, 0x2, 0x321, 0x32d, 0x7, 0x2a, 0x2, 0x2, 
    0x322, 0x323, 0x5, 0x42, 0x22, 0x2, 0x323, 0x32a, 0x8, 0x31, 0x1, 0x2, 
    0x324, 0x325, 0x7, 0x26, 0x2, 0x2, 0x325, 0x326, 0x5, 0x42, 0x22, 0x2, 
    0x326, 0x327, 0x8, 0x31, 0x1, 0x2, 0x327, 0x329, 0x3, 0x2, 0x2, 0x2, 
    0x328, 0x324, 0x3, 0x2, 0x2, 0x2, 0x329, 0x32c, 0x3, 0x2, 0x2, 0x2, 
    0x32a, 0x328, 0x3, 0x2, 0x2, 0x2, 0x32a, 0x32b, 0x3, 0x2, 0x2, 0x2, 
    0x32b, 0x32e, 0x3, 0x2, 0x2, 0x2, 0x32c, 0x32a, 0x3, 0x2, 0x2, 0x2, 
    0x32d, 0x322, 0x3, 0x2, 0x2, 0x2, 0x32d, 0x32e, 0x3, 0x2, 0x2, 0x2, 
    0x32e, 0x32f, 0x3, 0x2, 0x2, 0x2, 0x32f, 0x330, 0x7, 0x2b, 0x2, 0x2, 
    0x330, 0x331, 0x8, 0x31, 0x1, 0x2, 0x331, 0x61, 0x3, 0x2, 0x2, 0x2, 
    0x3f, 0x67, 0x70, 0x74, 0x7a, 0x83, 0x85, 0xad, 0xaf, 0xc8, 0xd2, 0xda, 
    0xe7, 0xf1, 0xff, 0x108, 0x117, 0x126, 0x131, 0x140, 0x143, 0x14c, 0x151, 
    0x15e, 0x16b, 0x175, 0x180, 0x19f, 0x1b0, 0x1ba, 0x1c0, 0x1c6, 0x1f0, 
    0x1fb, 0x1fd, 0x208, 0x221, 0x229, 0x23d, 0x24c, 0x24e, 0x259, 0x264, 
    0x26f, 0x27a, 0x285, 0x290, 0x29b, 0x2a6, 0x2b1, 0x2bc, 0x2c3, 0x2ce, 
    0x2df, 0x2e2, 0x2f5, 0x2f9, 0x2fb, 0x2fe, 0x31f, 0x32a, 0x32d, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

AnyFXParser::Initializer AnyFXParser::_init;
