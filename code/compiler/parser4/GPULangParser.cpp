

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



// Generated from antlr4/grammar/GPULang.g4 by ANTLR 4.7.1


#include "GPULangListener.h"

#include "GPULangParser.h"


using namespace antlrcpp;
using namespace antlr4;

GPULangParser::GPULangParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

GPULangParser::~GPULangParser() {
  delete _interpreter;
}

std::string GPULangParser::getGrammarFileName() const {
  return "GPULang.g4";
}

const std::vector<std::string>& GPULangParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& GPULangParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- StringContext ------------------------------------------------------------------

GPULangParser::StringContext::StringContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
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

  auto onExit = finally([=] {
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
          ((1ULL << _la) & ((1ULL << GPULangParser::T__0)
          | (1ULL << GPULangParser::T__1)
          | (1ULL << GPULangParser::T__2)
          | (1ULL << GPULangParser::T__3)
          | (1ULL << GPULangParser::T__4)
          | (1ULL << GPULangParser::T__5)
          | (1ULL << GPULangParser::T__6)
          | (1ULL << GPULangParser::T__7)
          | (1ULL << GPULangParser::T__8)
          | (1ULL << GPULangParser::T__9)
          | (1ULL << GPULangParser::T__10)
          | (1ULL << GPULangParser::T__11)
          | (1ULL << GPULangParser::T__12)
          | (1ULL << GPULangParser::T__13)
          | (1ULL << GPULangParser::T__14)
          | (1ULL << GPULangParser::T__15)
          | (1ULL << GPULangParser::T__16)
          | (1ULL << GPULangParser::T__17)
          | (1ULL << GPULangParser::T__18)
          | (1ULL << GPULangParser::T__19)
          | (1ULL << GPULangParser::T__20)
          | (1ULL << GPULangParser::T__21)
          | (1ULL << GPULangParser::T__22)
          | (1ULL << GPULangParser::T__23)
          | (1ULL << GPULangParser::T__24)
          | (1ULL << GPULangParser::T__25)
          | (1ULL << GPULangParser::T__26)
          | (1ULL << GPULangParser::T__27)
          | (1ULL << GPULangParser::T__28)
          | (1ULL << GPULangParser::T__29)
          | (1ULL << GPULangParser::T__30)
          | (1ULL << GPULangParser::T__31)
          | (1ULL << GPULangParser::T__32)
          | (1ULL << GPULangParser::T__33)
          | (1ULL << GPULangParser::SC)
          | (1ULL << GPULangParser::CO)
          | (1ULL << GPULangParser::COL)
          | (1ULL << GPULangParser::LP)
          | (1ULL << GPULangParser::RP)
          | (1ULL << GPULangParser::LB)
          | (1ULL << GPULangParser::RB)
          | (1ULL << GPULangParser::LL)
          | (1ULL << GPULangParser::RR)
          | (1ULL << GPULangParser::DOT)
          | (1ULL << GPULangParser::NOT)
          | (1ULL << GPULangParser::EQ)
          | (1ULL << GPULangParser::QU)
          | (1ULL << GPULangParser::AND)
          | (1ULL << GPULangParser::ANDSET)
          | (1ULL << GPULangParser::OR)
          | (1ULL << GPULangParser::ORSET)
          | (1ULL << GPULangParser::XOR)
          | (1ULL << GPULangParser::XORSET)
          | (1ULL << GPULangParser::CONNJUGATE)
          | (1ULL << GPULangParser::Q)
          | (1ULL << GPULangParser::NU)
          | (1ULL << GPULangParser::FORWARDSLASH)
          | (1ULL << GPULangParser::LESS)
          | (1ULL << GPULangParser::LESSEQ)
          | (1ULL << GPULangParser::GREATER)
          | (1ULL << GPULangParser::GREATEREQ)
          | (1ULL << GPULangParser::LOGICEQ))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 64)) & ((1ULL << (GPULangParser::NOTEQ - 64))
          | (1ULL << (GPULangParser::LOGICAND - 64))
          | (1ULL << (GPULangParser::LOGICOR - 64))
          | (1ULL << (GPULangParser::MOD - 64))
          | (1ULL << (GPULangParser::UNDERSC - 64))
          | (1ULL << (GPULangParser::SOBAKA - 64))
          | (1ULL << (GPULangParser::ADD_OP - 64))
          | (1ULL << (GPULangParser::SUB_OP - 64))
          | (1ULL << (GPULangParser::DIV_OP - 64))
          | (1ULL << (GPULangParser::MUL_OP - 64))
          | (1ULL << (GPULangParser::ARROW - 64))
          | (1ULL << (GPULangParser::INTEGERLITERAL - 64))
          | (1ULL << (GPULangParser::UINTEGERLITERAL - 64))
          | (1ULL << (GPULangParser::COMMENT - 64))
          | (1ULL << (GPULangParser::ML_COMMENT - 64))
          | (1ULL << (GPULangParser::FLOATLITERAL - 64))
          | (1ULL << (GPULangParser::EXPONENT - 64))
          | (1ULL << (GPULangParser::DOUBLELITERAL - 64))
          | (1ULL << (GPULangParser::HEX - 64))
          | (1ULL << (GPULangParser::IDENTIFIER - 64))
          | (1ULL << (GPULangParser::WS - 64)))) != 0)) {
          setState(97);
          dynamic_cast<StringContext *>(_localctx)->data = _input->LT(1);
          _la = _input->LA(1);
          if (_la == 0 || _la == Token::EOF || (_la == GPULangParser::QO)) {
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
          ((1ULL << _la) & ((1ULL << GPULangParser::T__0)
          | (1ULL << GPULangParser::T__1)
          | (1ULL << GPULangParser::T__2)
          | (1ULL << GPULangParser::T__3)
          | (1ULL << GPULangParser::T__4)
          | (1ULL << GPULangParser::T__5)
          | (1ULL << GPULangParser::T__6)
          | (1ULL << GPULangParser::T__7)
          | (1ULL << GPULangParser::T__8)
          | (1ULL << GPULangParser::T__9)
          | (1ULL << GPULangParser::T__10)
          | (1ULL << GPULangParser::T__11)
          | (1ULL << GPULangParser::T__12)
          | (1ULL << GPULangParser::T__13)
          | (1ULL << GPULangParser::T__14)
          | (1ULL << GPULangParser::T__15)
          | (1ULL << GPULangParser::T__16)
          | (1ULL << GPULangParser::T__17)
          | (1ULL << GPULangParser::T__18)
          | (1ULL << GPULangParser::T__19)
          | (1ULL << GPULangParser::T__20)
          | (1ULL << GPULangParser::T__21)
          | (1ULL << GPULangParser::T__22)
          | (1ULL << GPULangParser::T__23)
          | (1ULL << GPULangParser::T__24)
          | (1ULL << GPULangParser::T__25)
          | (1ULL << GPULangParser::T__26)
          | (1ULL << GPULangParser::T__27)
          | (1ULL << GPULangParser::T__28)
          | (1ULL << GPULangParser::T__29)
          | (1ULL << GPULangParser::T__30)
          | (1ULL << GPULangParser::T__31)
          | (1ULL << GPULangParser::T__32)
          | (1ULL << GPULangParser::T__33)
          | (1ULL << GPULangParser::SC)
          | (1ULL << GPULangParser::CO)
          | (1ULL << GPULangParser::COL)
          | (1ULL << GPULangParser::LP)
          | (1ULL << GPULangParser::RP)
          | (1ULL << GPULangParser::LB)
          | (1ULL << GPULangParser::RB)
          | (1ULL << GPULangParser::LL)
          | (1ULL << GPULangParser::RR)
          | (1ULL << GPULangParser::DOT)
          | (1ULL << GPULangParser::NOT)
          | (1ULL << GPULangParser::EQ)
          | (1ULL << GPULangParser::QO)
          | (1ULL << GPULangParser::QU)
          | (1ULL << GPULangParser::AND)
          | (1ULL << GPULangParser::ANDSET)
          | (1ULL << GPULangParser::OR)
          | (1ULL << GPULangParser::ORSET)
          | (1ULL << GPULangParser::XOR)
          | (1ULL << GPULangParser::XORSET)
          | (1ULL << GPULangParser::CONNJUGATE)
          | (1ULL << GPULangParser::NU)
          | (1ULL << GPULangParser::FORWARDSLASH)
          | (1ULL << GPULangParser::LESS)
          | (1ULL << GPULangParser::LESSEQ)
          | (1ULL << GPULangParser::GREATER)
          | (1ULL << GPULangParser::GREATEREQ)
          | (1ULL << GPULangParser::LOGICEQ))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 64)) & ((1ULL << (GPULangParser::NOTEQ - 64))
          | (1ULL << (GPULangParser::LOGICAND - 64))
          | (1ULL << (GPULangParser::LOGICOR - 64))
          | (1ULL << (GPULangParser::MOD - 64))
          | (1ULL << (GPULangParser::UNDERSC - 64))
          | (1ULL << (GPULangParser::SOBAKA - 64))
          | (1ULL << (GPULangParser::ADD_OP - 64))
          | (1ULL << (GPULangParser::SUB_OP - 64))
          | (1ULL << (GPULangParser::DIV_OP - 64))
          | (1ULL << (GPULangParser::MUL_OP - 64))
          | (1ULL << (GPULangParser::ARROW - 64))
          | (1ULL << (GPULangParser::INTEGERLITERAL - 64))
          | (1ULL << (GPULangParser::UINTEGERLITERAL - 64))
          | (1ULL << (GPULangParser::COMMENT - 64))
          | (1ULL << (GPULangParser::ML_COMMENT - 64))
          | (1ULL << (GPULangParser::FLOATLITERAL - 64))
          | (1ULL << (GPULangParser::EXPONENT - 64))
          | (1ULL << (GPULangParser::DOUBLELITERAL - 64))
          | (1ULL << (GPULangParser::HEX - 64))
          | (1ULL << (GPULangParser::IDENTIFIER - 64))
          | (1ULL << (GPULangParser::WS - 64)))) != 0)) {
          setState(106);
          dynamic_cast<StringContext *>(_localctx)->data = _input->LT(1);
          _la = _input->LA(1);
          if (_la == 0 || _la == Token::EOF || (_la == GPULangParser::Q)) {
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

          dynamic_cast<BooleanContext *>(_localctx)->val =  false;
      

  auto onExit = finally([=] {
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
         dynamic_cast<BooleanContext *>(_localctx)->val =  true; 
        break;
      }

      case GPULangParser::T__1: {
        enterOuterAlt(_localctx, 2);
        setState(118);
        match(GPULangParser::T__1);
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
          match(GPULangParser::T__2);
          setState(124);
          dynamic_cast<PreprocessContext *>(_localctx)->line = match(GPULangParser::INTEGERLITERAL);
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

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(136);
    dynamic_cast<EntryContext *>(_localctx)->effectContext = effect();

            dynamic_cast<EntryContext *>(_localctx)->returnEffect =  dynamic_cast<EntryContext *>(_localctx)->effectContext->eff;
        
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
          match(GPULangParser::SC);
           _localctx->eff->symbols.push_back(dynamic_cast<EffectContext *>(_localctx)->aliasContext->sym); 
          break;
        }

        case 2: {
          setState(144);
          dynamic_cast<EffectContext *>(_localctx)->functionDeclarationContext = functionDeclaration();
          setState(145);
          match(GPULangParser::SC);
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
          match(GPULangParser::SC);
           for (Variable* var : dynamic_cast<EffectContext *>(_localctx)->variablesContext->list) { _localctx->eff->symbols.push_back(var); } 
          break;
        }

        case 5: {
          setState(155);
          dynamic_cast<EffectContext *>(_localctx)->structureContext = structure();
          setState(156);
          match(GPULangParser::SC);
           _localctx->eff->symbols.push_back(dynamic_cast<EffectContext *>(_localctx)->structureContext->sym); 
          break;
        }

        case 6: {
          setState(159);
          dynamic_cast<EffectContext *>(_localctx)->enumerationContext = enumeration();
          setState(160);
          match(GPULangParser::SC);
           _localctx->eff->symbols.push_back(dynamic_cast<EffectContext *>(_localctx)->enumerationContext->sym); 
          break;
        }

        case 7: {
          setState(163);
          dynamic_cast<EffectContext *>(_localctx)->stateContext = state();
          setState(164);
          match(GPULangParser::SC);
           _localctx->eff->symbols.push_back(dynamic_cast<EffectContext *>(_localctx)->stateContext->sym); 
          break;
        }

        case 8: {
          setState(167);
          dynamic_cast<EffectContext *>(_localctx)->programContext = program();
          setState(168);
          match(GPULangParser::SC);
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

          dynamic_cast<AliasContext *>(_localctx)->sym =  nullptr;
          std::string name;
          std::string type;
      

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(176);
    match(GPULangParser::T__3);
    setState(177);
    dynamic_cast<AliasContext *>(_localctx)->name = match(GPULangParser::IDENTIFIER);
    setState(178);
    match(GPULangParser::T__4);
    setState(179);
    dynamic_cast<AliasContext *>(_localctx)->type = match(GPULangParser::IDENTIFIER);
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

GPULangParser::AnnotationContext::AnnotationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
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

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(183);
    match(GPULangParser::SOBAKA);

    setState(184);
    dynamic_cast<AnnotationContext *>(_localctx)->name = match(GPULangParser::IDENTIFIER);
    setState(185);
    match(GPULangParser::LP);
    setState(186);
    dynamic_cast<AnnotationContext *>(_localctx)->value = expression();
    setState(187);
    match(GPULangParser::RP);
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

GPULangParser::AttributeContext::AttributeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

GPULangParser::ExpressionContext* GPULangParser::AttributeContext::expression() {
  return getRuleContext<GPULangParser::ExpressionContext>(0);
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
      dynamic_cast<AttributeContext *>(_localctx)->name = match(GPULangParser::IDENTIFIER);
      setState(191);
      match(GPULangParser::LP);
      setState(192);
      dynamic_cast<AttributeContext *>(_localctx)->expressionContext = expression();
      setState(193);
      match(GPULangParser::RP);
       _localctx->attr.name = (dynamic_cast<AttributeContext *>(_localctx)->name != nullptr ? dynamic_cast<AttributeContext *>(_localctx)->name->getText() : ""); _localctx->attr.expression = dynamic_cast<AttributeContext *>(_localctx)->expressionContext->tree; 
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(196);
      dynamic_cast<AttributeContext *>(_localctx)->name = match(GPULangParser::IDENTIFIER);
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

GPULangParser::TypeDeclarationContext::TypeDeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* GPULangParser::TypeDeclarationContext::IDENTIFIER() {
  return getToken(GPULangParser::IDENTIFIER, 0);
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
      

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(200);
    dynamic_cast<TypeDeclarationContext *>(_localctx)->identifierToken = match(GPULangParser::IDENTIFIER);
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

std::vector<GPULangParser::AssignmentExpressionContext *> GPULangParser::VariablesContext::assignmentExpression() {
  return getRuleContexts<GPULangParser::AssignmentExpressionContext>();
}

GPULangParser::AssignmentExpressionContext* GPULangParser::VariablesContext::assignmentExpression(size_t i) {
  return getRuleContext<GPULangParser::AssignmentExpressionContext>(i);
}

std::vector<tree::TerminalNode *> GPULangParser::VariablesContext::INTEGERLITERAL() {
  return getTokens(GPULangParser::INTEGERLITERAL);
}

tree::TerminalNode* GPULangParser::VariablesContext::INTEGERLITERAL(size_t i) {
  return getToken(GPULangParser::INTEGERLITERAL, i);
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

          std::vector<Type::FullType> types;
          std::vector<Annotation> annotations;
          std::vector<Attribute> attributes;
          std::vector<std::string> names;
          std::vector<Expression*> valueExpressions;
          std::vector<Symbol::Location> locations;
          Symbol::Location location;
          Type::FullType type;
      
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
    while (_la == GPULangParser::SOBAKA) {
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
    dynamic_cast<VariablesContext *>(_localctx)->typeName = match(GPULangParser::IDENTIFIER);
     type.name = (dynamic_cast<VariablesContext *>(_localctx)->typeName != nullptr ? dynamic_cast<VariablesContext *>(_localctx)->typeName->getText() : ""); 
    setState(225);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == GPULangParser::MUL_OP) {
      setState(221);
      match(GPULangParser::MUL_OP);
       type.AddModifier(Type::FullType::Modifier::PointerLevel); 
      setState(227);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(228);
    dynamic_cast<VariablesContext *>(_localctx)->varName = match(GPULangParser::IDENTIFIER);
     names.push_back((dynamic_cast<VariablesContext *>(_localctx)->varName != nullptr ? dynamic_cast<VariablesContext *>(_localctx)->varName->getText() : "")); valueExpressions.push_back(nullptr); locations.push_back(SetupFile()); 
    setState(239);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == GPULangParser::LL) {
       type.AddModifier(Type::FullType::Modifier::ArrayLevel); 
      setState(231);
      match(GPULangParser::LL);
      setState(234);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == GPULangParser::INTEGERLITERAL) {
        setState(232);
        dynamic_cast<VariablesContext *>(_localctx)->arraySize0 = match(GPULangParser::INTEGERLITERAL);
         type.UpdateValue(atoi((dynamic_cast<VariablesContext *>(_localctx)->arraySize0 != nullptr ? dynamic_cast<VariablesContext *>(_localctx)->arraySize0->getText() : "").c_str())); 
      }
      setState(236);
      match(GPULangParser::RR);
      setState(241);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
     types.push_back(type); type.ClearModifiers(); 
    setState(247);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == GPULangParser::EQ) {
      setState(243);
      match(GPULangParser::EQ);
      setState(244);
      dynamic_cast<VariablesContext *>(_localctx)->valueExpr = assignmentExpression();
       valueExpressions.back() = dynamic_cast<VariablesContext *>(_localctx)->valueExpr->tree; 
    }
    setState(280);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == GPULangParser::CO) {
      setState(249);
      match(GPULangParser::CO);
      setState(254);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == GPULangParser::MUL_OP) {
        setState(250);
        match(GPULangParser::MUL_OP);
         type.AddModifier(Type::FullType::Modifier::PointerLevel); 
        setState(256);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(257);
      dynamic_cast<VariablesContext *>(_localctx)->varNameN = match(GPULangParser::IDENTIFIER);
       names.push_back((dynamic_cast<VariablesContext *>(_localctx)->varNameN != nullptr ? dynamic_cast<VariablesContext *>(_localctx)->varNameN->getText() : "")); valueExpressions.push_back(nullptr); locations.push_back(SetupFile()); 
      setState(268);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == GPULangParser::LL) {
         type.AddModifier(Type::FullType::Modifier::ArrayLevel); 
        setState(260);
        match(GPULangParser::LL);
        setState(263);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == GPULangParser::INTEGERLITERAL) {
          setState(261);
          dynamic_cast<VariablesContext *>(_localctx)->arraySizeN = match(GPULangParser::INTEGERLITERAL);
           type.UpdateValue(atoi((dynamic_cast<VariablesContext *>(_localctx)->arraySizeN != nullptr ? dynamic_cast<VariablesContext *>(_localctx)->arraySizeN->getText() : "").c_str())); 
        }
        setState(265);
        match(GPULangParser::RR);
        setState(270);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
       types.push_back(type); type.ClearModifiers(); 
      setState(276);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == GPULangParser::EQ) {
        setState(272);
        match(GPULangParser::EQ);
        setState(273);
        dynamic_cast<VariablesContext *>(_localctx)->valueExprN = assignmentExpression();
         valueExpressions.back() = dynamic_cast<VariablesContext *>(_localctx)->valueExprN->tree; 
      }
      setState(282);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }

            for (size_t i = 0; i < names.size(); i++)
            {
                Variable* var = new Variable(); 
                var->type = types[i]; 
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

//----------------- VariableContext ------------------------------------------------------------------

GPULangParser::VariableContext::VariableContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> GPULangParser::VariableContext::IDENTIFIER() {
  return getTokens(GPULangParser::IDENTIFIER);
}

tree::TerminalNode* GPULangParser::VariableContext::IDENTIFIER(size_t i) {
  return getToken(GPULangParser::IDENTIFIER, i);
}

std::vector<GPULangParser::AttributeContext *> GPULangParser::VariableContext::attribute() {
  return getRuleContexts<GPULangParser::AttributeContext>();
}

GPULangParser::AttributeContext* GPULangParser::VariableContext::attribute(size_t i) {
  return getRuleContext<GPULangParser::AttributeContext>(i);
}

GPULangParser::ExpressionContext* GPULangParser::VariableContext::expression() {
  return getRuleContext<GPULangParser::ExpressionContext>(0);
}

std::vector<tree::TerminalNode *> GPULangParser::VariableContext::INTEGERLITERAL() {
  return getTokens(GPULangParser::INTEGERLITERAL);
}

tree::TerminalNode* GPULangParser::VariableContext::INTEGERLITERAL(size_t i) {
  return getToken(GPULangParser::INTEGERLITERAL, i);
}


size_t GPULangParser::VariableContext::getRuleIndex() const {
  return GPULangParser::RuleVariable;
}

void GPULangParser::VariableContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterVariable(this);
}

void GPULangParser::VariableContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<GPULangListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitVariable(this);
}

GPULangParser::VariableContext* GPULangParser::variable() {
  VariableContext *_localctx = _tracker.createInstance<VariableContext>(_ctx, getState());
  enterRule(_localctx, 20, GPULangParser::RuleVariable);

          dynamic_cast<VariableContext *>(_localctx)->sym =  nullptr;
          Type::FullType type;
          std::vector<Attribute> attributes;
          std::string name;
          Expression* valueExpression = nullptr;
          Symbol::Location location;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(290);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 20, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(285);
        dynamic_cast<VariableContext *>(_localctx)->attributeContext = attribute();
         attributes.push_back(dynamic_cast<VariableContext *>(_localctx)->attributeContext->attr);  
      }
      setState(292);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 20, _ctx);
    }
    setState(293);
    dynamic_cast<VariableContext *>(_localctx)->typeName = match(GPULangParser::IDENTIFIER);
     type.name = (dynamic_cast<VariableContext *>(_localctx)->typeName != nullptr ? dynamic_cast<VariableContext *>(_localctx)->typeName->getText() : ""); location = SetupFile(); 
    setState(299);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == GPULangParser::MUL_OP) {
      setState(295);
      match(GPULangParser::MUL_OP);
       type.AddModifier(Type::FullType::Modifier::PointerLevel); 
      setState(301);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(302);
    dynamic_cast<VariableContext *>(_localctx)->varName = match(GPULangParser::IDENTIFIER);
     name = (dynamic_cast<VariableContext *>(_localctx)->varName != nullptr ? dynamic_cast<VariableContext *>(_localctx)->varName->getText() : ""); 
    setState(313);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == GPULangParser::LL) {
       type.AddModifier(Type::FullType::Modifier::ArrayLevel); 
      setState(305);
      match(GPULangParser::LL);
      setState(308);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == GPULangParser::INTEGERLITERAL) {
        setState(306);
        dynamic_cast<VariableContext *>(_localctx)->arraySize0 = match(GPULangParser::INTEGERLITERAL);
         type.UpdateValue(atoi((dynamic_cast<VariableContext *>(_localctx)->arraySize0 != nullptr ? dynamic_cast<VariableContext *>(_localctx)->arraySize0->getText() : "").c_str())); 
      }
      setState(310);
      match(GPULangParser::RR);
      setState(315);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(320);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == GPULangParser::EQ) {
      setState(316);
      match(GPULangParser::EQ);
      setState(317);
      dynamic_cast<VariableContext *>(_localctx)->valueExpr = expression();
       valueExpression = dynamic_cast<VariableContext *>(_localctx)->valueExpr->tree; 
    }

            dynamic_cast<VariableContext *>(_localctx)->sym =  new Variable(); 
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
  enterRule(_localctx, 22, GPULangParser::RuleStructureDeclaration);

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
    setState(329);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == GPULangParser::SOBAKA) {
      setState(324);
      dynamic_cast<StructureDeclarationContext *>(_localctx)->annotationContext = annotation();
       annotations.push_back(dynamic_cast<StructureDeclarationContext *>(_localctx)->annotationContext->annot); 
      setState(331);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(332);
    match(GPULangParser::T__5);
    setState(338);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 26, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(333);
        dynamic_cast<StructureDeclarationContext *>(_localctx)->attributeContext = attribute();
         attributes.push_back(dynamic_cast<StructureDeclarationContext *>(_localctx)->attributeContext->attr);  
      }
      setState(340);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 26, _ctx);
    }
    setState(341);
    dynamic_cast<StructureDeclarationContext *>(_localctx)->name = match(GPULangParser::IDENTIFIER);
     
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

GPULangParser::StructureContext::StructureContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

GPULangParser::StructureDeclarationContext* GPULangParser::StructureContext::structureDeclaration() {
  return getRuleContext<GPULangParser::StructureDeclarationContext>(0);
}

std::vector<GPULangParser::VariablesContext *> GPULangParser::StructureContext::variables() {
  return getRuleContexts<GPULangParser::VariablesContext>();
}

GPULangParser::VariablesContext* GPULangParser::StructureContext::variables(size_t i) {
  return getRuleContext<GPULangParser::VariablesContext>(i);
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
  enterRule(_localctx, 24, GPULangParser::RuleStructure);

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
    setState(344);
    dynamic_cast<StructureContext *>(_localctx)->structureDeclarationContext = structureDeclaration();
     dynamic_cast<StructureContext *>(_localctx)->sym =  dynamic_cast<StructureContext *>(_localctx)->structureDeclarationContext->sym; 
    setState(346);
    match(GPULangParser::LB);
    setState(353);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == GPULangParser::SOBAKA

    || _la == GPULangParser::IDENTIFIER) {
      setState(347);
      dynamic_cast<StructureContext *>(_localctx)->variablesContext = variables();
       for(Variable* var : dynamic_cast<StructureContext *>(_localctx)->variablesContext->list) { members.push_back(var); }
      setState(349);
      match(GPULangParser::SC);
      setState(355);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(356);
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

std::vector<tree::TerminalNode *> GPULangParser::EnumerationContext::IDENTIFIER() {
  return getTokens(GPULangParser::IDENTIFIER);
}

tree::TerminalNode* GPULangParser::EnumerationContext::IDENTIFIER(size_t i) {
  return getToken(GPULangParser::IDENTIFIER, i);
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
  enterRule(_localctx, 26, GPULangParser::RuleEnumeration);

          dynamic_cast<EnumerationContext *>(_localctx)->sym =  nullptr;
          std::vector<std::string> enumLabels;
          std::vector<Expression*> enumValues;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(359);
    match(GPULangParser::T__6);
    setState(360);
    dynamic_cast<EnumerationContext *>(_localctx)->name = match(GPULangParser::IDENTIFIER);
    setState(361);
    match(GPULangParser::LB);
    setState(362);
    dynamic_cast<EnumerationContext *>(_localctx)->label = match(GPULangParser::IDENTIFIER);
     Expression* expr = nullptr; 
    setState(368);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == GPULangParser::EQ) {
      setState(364);
      match(GPULangParser::EQ);
      setState(365);
      dynamic_cast<EnumerationContext *>(_localctx)->value = expression();
       expr = dynamic_cast<EnumerationContext *>(_localctx)->value->tree; 
    }

                enumLabels.push_back((dynamic_cast<EnumerationContext *>(_localctx)->label != nullptr ? dynamic_cast<EnumerationContext *>(_localctx)->label->getText() : ""));
                enumValues.push_back(expr);
            
    setState(371);
    match(GPULangParser::RB);

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

GPULangParser::FunctionDeclarationContext::FunctionDeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

GPULangParser::TypeDeclarationContext* GPULangParser::FunctionDeclarationContext::typeDeclaration() {
  return getRuleContext<GPULangParser::TypeDeclarationContext>(0);
}

tree::TerminalNode* GPULangParser::FunctionDeclarationContext::IDENTIFIER() {
  return getToken(GPULangParser::IDENTIFIER, 0);
}

std::vector<GPULangParser::AttributeContext *> GPULangParser::FunctionDeclarationContext::attribute() {
  return getRuleContexts<GPULangParser::AttributeContext>();
}

GPULangParser::AttributeContext* GPULangParser::FunctionDeclarationContext::attribute(size_t i) {
  return getRuleContext<GPULangParser::AttributeContext>(i);
}

std::vector<GPULangParser::VariableContext *> GPULangParser::FunctionDeclarationContext::variable() {
  return getRuleContexts<GPULangParser::VariableContext>();
}

GPULangParser::VariableContext* GPULangParser::FunctionDeclarationContext::variable(size_t i) {
  return getRuleContext<GPULangParser::VariableContext>(i);
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
    setState(379);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 29, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(374);
        dynamic_cast<FunctionDeclarationContext *>(_localctx)->attributeContext = attribute();
         attributes.push_back(dynamic_cast<FunctionDeclarationContext *>(_localctx)->attributeContext->attr);  
      }
      setState(381);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 29, _ctx);
    }
    setState(382);
    dynamic_cast<FunctionDeclarationContext *>(_localctx)->returnType = typeDeclaration();
    setState(383);
    dynamic_cast<FunctionDeclarationContext *>(_localctx)->name = match(GPULangParser::IDENTIFIER);
     location = SetupFile(); 
    setState(385);
    match(GPULangParser::LP);
    setState(397);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == GPULangParser::IDENTIFIER) {
      setState(386);
      dynamic_cast<FunctionDeclarationContext *>(_localctx)->arg0 = variable();
       variables.push_back(dynamic_cast<FunctionDeclarationContext *>(_localctx)->arg0->sym); 
      setState(394);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == GPULangParser::CO) {
        setState(388);
        match(GPULangParser::CO);
        setState(389);
        dynamic_cast<FunctionDeclarationContext *>(_localctx)->argn = variable();
         variables.push_back(dynamic_cast<FunctionDeclarationContext *>(_localctx)->argn->sym); 
        setState(396);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(399);
    match(GPULangParser::RP);

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

GPULangParser::CodeblockContext::CodeblockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
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

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(411);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case GPULangParser::LB: {
        enterOuterAlt(_localctx, 1);
        setState(402);
        match(GPULangParser::LB);
        setState(406);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << GPULangParser::T__0)
          | (1ULL << GPULangParser::T__1)
          | (1ULL << GPULangParser::T__2)
          | (1ULL << GPULangParser::T__3)
          | (1ULL << GPULangParser::T__4)
          | (1ULL << GPULangParser::T__5)
          | (1ULL << GPULangParser::T__6)
          | (1ULL << GPULangParser::T__7)
          | (1ULL << GPULangParser::T__8)
          | (1ULL << GPULangParser::T__9)
          | (1ULL << GPULangParser::T__10)
          | (1ULL << GPULangParser::T__11)
          | (1ULL << GPULangParser::T__12)
          | (1ULL << GPULangParser::T__13)
          | (1ULL << GPULangParser::T__14)
          | (1ULL << GPULangParser::T__15)
          | (1ULL << GPULangParser::T__16)
          | (1ULL << GPULangParser::T__17)
          | (1ULL << GPULangParser::T__18)
          | (1ULL << GPULangParser::T__19)
          | (1ULL << GPULangParser::T__20)
          | (1ULL << GPULangParser::T__21)
          | (1ULL << GPULangParser::T__22)
          | (1ULL << GPULangParser::T__23)
          | (1ULL << GPULangParser::T__24)
          | (1ULL << GPULangParser::T__25)
          | (1ULL << GPULangParser::T__26)
          | (1ULL << GPULangParser::T__27)
          | (1ULL << GPULangParser::T__28)
          | (1ULL << GPULangParser::T__29)
          | (1ULL << GPULangParser::T__30)
          | (1ULL << GPULangParser::T__31)
          | (1ULL << GPULangParser::T__32)
          | (1ULL << GPULangParser::T__33)
          | (1ULL << GPULangParser::SC)
          | (1ULL << GPULangParser::CO)
          | (1ULL << GPULangParser::COL)
          | (1ULL << GPULangParser::LP)
          | (1ULL << GPULangParser::RP)
          | (1ULL << GPULangParser::LB)
          | (1ULL << GPULangParser::LL)
          | (1ULL << GPULangParser::RR)
          | (1ULL << GPULangParser::DOT)
          | (1ULL << GPULangParser::NOT)
          | (1ULL << GPULangParser::EQ)
          | (1ULL << GPULangParser::QO)
          | (1ULL << GPULangParser::QU)
          | (1ULL << GPULangParser::AND)
          | (1ULL << GPULangParser::ANDSET)
          | (1ULL << GPULangParser::OR)
          | (1ULL << GPULangParser::ORSET)
          | (1ULL << GPULangParser::XOR)
          | (1ULL << GPULangParser::XORSET)
          | (1ULL << GPULangParser::CONNJUGATE)
          | (1ULL << GPULangParser::Q)
          | (1ULL << GPULangParser::NU)
          | (1ULL << GPULangParser::FORWARDSLASH)
          | (1ULL << GPULangParser::LESS)
          | (1ULL << GPULangParser::LESSEQ)
          | (1ULL << GPULangParser::GREATER)
          | (1ULL << GPULangParser::GREATEREQ)
          | (1ULL << GPULangParser::LOGICEQ))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 64)) & ((1ULL << (GPULangParser::NOTEQ - 64))
          | (1ULL << (GPULangParser::LOGICAND - 64))
          | (1ULL << (GPULangParser::LOGICOR - 64))
          | (1ULL << (GPULangParser::MOD - 64))
          | (1ULL << (GPULangParser::UNDERSC - 64))
          | (1ULL << (GPULangParser::SOBAKA - 64))
          | (1ULL << (GPULangParser::ADD_OP - 64))
          | (1ULL << (GPULangParser::SUB_OP - 64))
          | (1ULL << (GPULangParser::DIV_OP - 64))
          | (1ULL << (GPULangParser::MUL_OP - 64))
          | (1ULL << (GPULangParser::ARROW - 64))
          | (1ULL << (GPULangParser::INTEGERLITERAL - 64))
          | (1ULL << (GPULangParser::UINTEGERLITERAL - 64))
          | (1ULL << (GPULangParser::COMMENT - 64))
          | (1ULL << (GPULangParser::ML_COMMENT - 64))
          | (1ULL << (GPULangParser::FLOATLITERAL - 64))
          | (1ULL << (GPULangParser::EXPONENT - 64))
          | (1ULL << (GPULangParser::DOUBLELITERAL - 64))
          | (1ULL << (GPULangParser::HEX - 64))
          | (1ULL << (GPULangParser::IDENTIFIER - 64))
          | (1ULL << (GPULangParser::WS - 64)))) != 0)) {
          setState(403);
          codeblock();
          setState(408);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(409);
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
        setState(410);
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

          dynamic_cast<FunctionContext *>(_localctx)->sym =  nullptr;
          Token* startToken = nullptr;
          Token* endToken = nullptr;
      

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(413);
    dynamic_cast<FunctionContext *>(_localctx)->functionDeclarationContext = functionDeclaration();
     dynamic_cast<FunctionContext *>(_localctx)->sym =  dynamic_cast<FunctionContext *>(_localctx)->functionDeclarationContext->sym; 

            startToken = _input->LT(2);
        
    setState(416);
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

GPULangParser::ProgramContext::ProgramContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
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
    setState(424);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == GPULangParser::SOBAKA) {
      setState(419);
      dynamic_cast<ProgramContext *>(_localctx)->annotationContext = annotation();
       annotations.push_back(dynamic_cast<ProgramContext *>(_localctx)->annotationContext->annot); 
      setState(426);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(427);
    match(GPULangParser::T__7);
    setState(428);
    dynamic_cast<ProgramContext *>(_localctx)->name = match(GPULangParser::IDENTIFIER);
     location = SetupFile(); 
    setState(430);
    match(GPULangParser::LB);
    setState(437);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << GPULangParser::T__0)
      | (1ULL << GPULangParser::T__1)
      | (1ULL << GPULangParser::T__31)
      | (1ULL << GPULangParser::T__32)
      | (1ULL << GPULangParser::LP)
      | (1ULL << GPULangParser::LB)
      | (1ULL << GPULangParser::NOT)
      | (1ULL << GPULangParser::QO)
      | (1ULL << GPULangParser::CONNJUGATE)
      | (1ULL << GPULangParser::Q))) != 0) || ((((_la - 70) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 70)) & ((1ULL << (GPULangParser::ADD_OP - 70))
      | (1ULL << (GPULangParser::SUB_OP - 70))
      | (1ULL << (GPULangParser::MUL_OP - 70))
      | (1ULL << (GPULangParser::INTEGERLITERAL - 70))
      | (1ULL << (GPULangParser::UINTEGERLITERAL - 70))
      | (1ULL << (GPULangParser::FLOATLITERAL - 70))
      | (1ULL << (GPULangParser::DOUBLELITERAL - 70))
      | (1ULL << (GPULangParser::HEX - 70))
      | (1ULL << (GPULangParser::IDENTIFIER - 70)))) != 0)) {
      setState(431);
      dynamic_cast<ProgramContext *>(_localctx)->assignment = expression();
       entries.push_back(dynamic_cast<ProgramContext *>(_localctx)->assignment->tree); 
      setState(433);
      match(GPULangParser::SC);
      setState(439);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(440);
    match(GPULangParser::RB);
     
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

GPULangParser::StateContext::StateContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* GPULangParser::StateContext::IDENTIFIER() {
  return getToken(GPULangParser::IDENTIFIER, 0);
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
          Symbol::Location location;
          std::vector<Expression*> entries;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(447);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case GPULangParser::T__8: {
        setState(443);
        match(GPULangParser::T__8);
         dynamic_cast<StateContext *>(_localctx)->sym =  new SamplerState(); 
        break;
      }

      case GPULangParser::T__9: {
        setState(445);
        match(GPULangParser::T__9);
         dynamic_cast<StateContext *>(_localctx)->sym =  new RenderState(); 
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(449);
    dynamic_cast<StateContext *>(_localctx)->name = match(GPULangParser::IDENTIFIER);
     location = SetupFile(); 
    setState(451);
    match(GPULangParser::LB);
    setState(458);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << GPULangParser::T__0)
      | (1ULL << GPULangParser::T__1)
      | (1ULL << GPULangParser::T__31)
      | (1ULL << GPULangParser::T__32)
      | (1ULL << GPULangParser::LP)
      | (1ULL << GPULangParser::LB)
      | (1ULL << GPULangParser::NOT)
      | (1ULL << GPULangParser::QO)
      | (1ULL << GPULangParser::CONNJUGATE)
      | (1ULL << GPULangParser::Q))) != 0) || ((((_la - 70) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 70)) & ((1ULL << (GPULangParser::ADD_OP - 70))
      | (1ULL << (GPULangParser::SUB_OP - 70))
      | (1ULL << (GPULangParser::MUL_OP - 70))
      | (1ULL << (GPULangParser::INTEGERLITERAL - 70))
      | (1ULL << (GPULangParser::UINTEGERLITERAL - 70))
      | (1ULL << (GPULangParser::FLOATLITERAL - 70))
      | (1ULL << (GPULangParser::DOUBLELITERAL - 70))
      | (1ULL << (GPULangParser::HEX - 70))
      | (1ULL << (GPULangParser::IDENTIFIER - 70)))) != 0)) {
      setState(452);
      dynamic_cast<StateContext *>(_localctx)->assign = expression();
       entries.push_back(dynamic_cast<StateContext *>(_localctx)->assign->tree); 
      setState(454);
      match(GPULangParser::SC);
      setState(460);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(461);
    match(GPULangParser::RB);

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

          dynamic_cast<StatementContext *>(_localctx)->tree =  nullptr;
      

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(489);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 38, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(464);
      dynamic_cast<StatementContext *>(_localctx)->ifStatementContext = ifStatement();
       dynamic_cast<StatementContext *>(_localctx)->tree =  dynamic_cast<StatementContext *>(_localctx)->ifStatementContext->tree; 
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(467);
      dynamic_cast<StatementContext *>(_localctx)->scopeStatementContext = scopeStatement();
       dynamic_cast<StatementContext *>(_localctx)->tree =  dynamic_cast<StatementContext *>(_localctx)->scopeStatementContext->tree; 
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(470);
      dynamic_cast<StatementContext *>(_localctx)->forStatementContext = forStatement();
       dynamic_cast<StatementContext *>(_localctx)->tree =  dynamic_cast<StatementContext *>(_localctx)->forStatementContext->tree; 
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(473);
      dynamic_cast<StatementContext *>(_localctx)->whileStatementContext = whileStatement();
       dynamic_cast<StatementContext *>(_localctx)->tree =  dynamic_cast<StatementContext *>(_localctx)->whileStatementContext->tree; 
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(476);
      dynamic_cast<StatementContext *>(_localctx)->returnStatementContext = returnStatement();
       dynamic_cast<StatementContext *>(_localctx)->tree =  dynamic_cast<StatementContext *>(_localctx)->returnStatementContext->tree; 
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(479);
      dynamic_cast<StatementContext *>(_localctx)->continueStatementContext = continueStatement();
       dynamic_cast<StatementContext *>(_localctx)->tree =  dynamic_cast<StatementContext *>(_localctx)->continueStatementContext->tree; 
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(482);
      dynamic_cast<StatementContext *>(_localctx)->breakStatementContext = breakStatement();
       dynamic_cast<StatementContext *>(_localctx)->tree =  dynamic_cast<StatementContext *>(_localctx)->breakStatementContext->tree; 
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(485);
      dynamic_cast<StatementContext *>(_localctx)->expressionStatementContext = expressionStatement();
      setState(486);
      match(GPULangParser::SC);
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

          dynamic_cast<ExpressionStatementContext *>(_localctx)->tree =  nullptr;
      

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(491);
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

GPULangParser::IfStatementContext::IfStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
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
    setState(494);
    match(GPULangParser::T__10);
     location = SetupFile(); 
    setState(496);
    match(GPULangParser::LP);
    setState(497);
    dynamic_cast<IfStatementContext *>(_localctx)->condition = expression();
     condition = dynamic_cast<IfStatementContext *>(_localctx)->condition->tree; 
    setState(499);
    match(GPULangParser::RP);
    setState(500);
    dynamic_cast<IfStatementContext *>(_localctx)->ifBody = statement();
     ifBody = dynamic_cast<IfStatementContext *>(_localctx)->ifBody->tree; 
    setState(506);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 39, _ctx)) {
    case 1: {
      setState(502);
      match(GPULangParser::T__11);
      setState(503);
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

GPULangParser::ForStatementContext::ForStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
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
    setState(510);
    match(GPULangParser::T__12);
     location = SetupFile(); 
    setState(512);
    match(GPULangParser::LP);
    setState(516);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == GPULangParser::SOBAKA

    || _la == GPULangParser::IDENTIFIER) {
      setState(513);
      dynamic_cast<ForStatementContext *>(_localctx)->variablesContext = variables();
       declarations = dynamic_cast<ForStatementContext *>(_localctx)->variablesContext->list; 
    }
    setState(518);
    match(GPULangParser::SC);
    setState(522);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << GPULangParser::T__0)
      | (1ULL << GPULangParser::T__1)
      | (1ULL << GPULangParser::T__31)
      | (1ULL << GPULangParser::T__32)
      | (1ULL << GPULangParser::LP)
      | (1ULL << GPULangParser::LB)
      | (1ULL << GPULangParser::NOT)
      | (1ULL << GPULangParser::QO)
      | (1ULL << GPULangParser::CONNJUGATE)
      | (1ULL << GPULangParser::Q))) != 0) || ((((_la - 70) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 70)) & ((1ULL << (GPULangParser::ADD_OP - 70))
      | (1ULL << (GPULangParser::SUB_OP - 70))
      | (1ULL << (GPULangParser::MUL_OP - 70))
      | (1ULL << (GPULangParser::INTEGERLITERAL - 70))
      | (1ULL << (GPULangParser::UINTEGERLITERAL - 70))
      | (1ULL << (GPULangParser::FLOATLITERAL - 70))
      | (1ULL << (GPULangParser::DOUBLELITERAL - 70))
      | (1ULL << (GPULangParser::HEX - 70))
      | (1ULL << (GPULangParser::IDENTIFIER - 70)))) != 0)) {
      setState(519);
      dynamic_cast<ForStatementContext *>(_localctx)->condition = expression();
       conditionExpression = dynamic_cast<ForStatementContext *>(_localctx)->condition->tree; 
    }
    setState(524);
    match(GPULangParser::SC);
    setState(528);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << GPULangParser::T__0)
      | (1ULL << GPULangParser::T__1)
      | (1ULL << GPULangParser::T__31)
      | (1ULL << GPULangParser::T__32)
      | (1ULL << GPULangParser::LP)
      | (1ULL << GPULangParser::LB)
      | (1ULL << GPULangParser::NOT)
      | (1ULL << GPULangParser::QO)
      | (1ULL << GPULangParser::CONNJUGATE)
      | (1ULL << GPULangParser::Q))) != 0) || ((((_la - 70) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 70)) & ((1ULL << (GPULangParser::ADD_OP - 70))
      | (1ULL << (GPULangParser::SUB_OP - 70))
      | (1ULL << (GPULangParser::MUL_OP - 70))
      | (1ULL << (GPULangParser::INTEGERLITERAL - 70))
      | (1ULL << (GPULangParser::UINTEGERLITERAL - 70))
      | (1ULL << (GPULangParser::FLOATLITERAL - 70))
      | (1ULL << (GPULangParser::DOUBLELITERAL - 70))
      | (1ULL << (GPULangParser::HEX - 70))
      | (1ULL << (GPULangParser::IDENTIFIER - 70)))) != 0)) {
      setState(525);
      dynamic_cast<ForStatementContext *>(_localctx)->loop = expression();
       loopExpression = dynamic_cast<ForStatementContext *>(_localctx)->loop->tree; 
    }
    setState(530);
    match(GPULangParser::RP);
    setState(531);
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

GPULangParser::ForRangeStatementContext::ForRangeStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
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

          dynamic_cast<ForRangeStatementContext *>(_localctx)->tree =  nullptr;
          Statement* contents = nullptr;
          Symbol::Location location;
      

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(535);
    match(GPULangParser::T__12);
     location = SetupFile(); 
    setState(537);
    match(GPULangParser::LP);
    setState(538);
    dynamic_cast<ForRangeStatementContext *>(_localctx)->iterator = match(GPULangParser::IDENTIFIER);
    setState(539);
    match(GPULangParser::COL);
    setState(540);
    dynamic_cast<ForRangeStatementContext *>(_localctx)->start = match(GPULangParser::IDENTIFIER);
    setState(541);
    match(GPULangParser::T__13);
    setState(542);
    dynamic_cast<ForRangeStatementContext *>(_localctx)->end = match(GPULangParser::IDENTIFIER);
    setState(543);
    match(GPULangParser::RP);
    setState(544);
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

GPULangParser::WhileStatementContext::WhileStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

GPULangParser::ExpressionContext* GPULangParser::WhileStatementContext::expression() {
  return getRuleContext<GPULangParser::ExpressionContext>(0);
}

GPULangParser::StatementContext* GPULangParser::WhileStatementContext::statement() {
  return getRuleContext<GPULangParser::StatementContext>(0);
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

          dynamic_cast<WhileStatementContext *>(_localctx)->tree =  nullptr;
          Expression* conditionExpression = nullptr;
          Statement* contents = nullptr;
          bool isDoWhile = false;
          Symbol::Location location;
      

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(570);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case GPULangParser::T__14: {
        enterOuterAlt(_localctx, 1);
        setState(548);
        match(GPULangParser::T__14);
         location = SetupFile(); 
        setState(550);
        match(GPULangParser::LP);
        setState(551);
        dynamic_cast<WhileStatementContext *>(_localctx)->condition = expression();
         conditionExpression = dynamic_cast<WhileStatementContext *>(_localctx)->condition->tree; 
        setState(553);
        match(GPULangParser::RP);
        setState(554);
        dynamic_cast<WhileStatementContext *>(_localctx)->content = statement();
         contents = dynamic_cast<WhileStatementContext *>(_localctx)->content->tree; 

                dynamic_cast<WhileStatementContext *>(_localctx)->tree =  new WhileStatement(conditionExpression, contents, isDoWhile);
                _localctx->tree->location = location;
            
        break;
      }

      case GPULangParser::T__15: {
        enterOuterAlt(_localctx, 2);
        setState(558);
        match(GPULangParser::T__15);
         location = SetupFile(); 
        setState(560);
        dynamic_cast<WhileStatementContext *>(_localctx)->content = statement();
         contents = dynamic_cast<WhileStatementContext *>(_localctx)->content->tree; isDoWhile = true; 
        setState(562);
        match(GPULangParser::T__14);
        setState(563);
        match(GPULangParser::LP);
        setState(564);
        dynamic_cast<WhileStatementContext *>(_localctx)->condition = expression();
         conditionExpression = dynamic_cast<WhileStatementContext *>(_localctx)->condition->tree; 
        setState(566);
        match(GPULangParser::RP);
        setState(567);
        match(GPULangParser::SC);

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

GPULangParser::ScopeStatementContext::ScopeStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<GPULangParser::StatementContext *> GPULangParser::ScopeStatementContext::statement() {
  return getRuleContexts<GPULangParser::StatementContext>();
}

GPULangParser::StatementContext* GPULangParser::ScopeStatementContext::statement(size_t i) {
  return getRuleContext<GPULangParser::StatementContext>(i);
}

std::vector<GPULangParser::VariablesContext *> GPULangParser::ScopeStatementContext::variables() {
  return getRuleContexts<GPULangParser::VariablesContext>();
}

GPULangParser::VariablesContext* GPULangParser::ScopeStatementContext::variables(size_t i) {
  return getRuleContext<GPULangParser::VariablesContext>(i);
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

          dynamic_cast<ScopeStatementContext *>(_localctx)->tree =  nullptr;
          std::vector<Symbol*> contents;
          Symbol::Location location;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(572);
    match(GPULangParser::LB);
     location = SetupFile(); 
    setState(583);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << GPULangParser::T__0)
      | (1ULL << GPULangParser::T__1)
      | (1ULL << GPULangParser::T__10)
      | (1ULL << GPULangParser::T__12)
      | (1ULL << GPULangParser::T__14)
      | (1ULL << GPULangParser::T__15)
      | (1ULL << GPULangParser::T__16)
      | (1ULL << GPULangParser::T__17)
      | (1ULL << GPULangParser::T__21)
      | (1ULL << GPULangParser::T__31)
      | (1ULL << GPULangParser::T__32)
      | (1ULL << GPULangParser::LP)
      | (1ULL << GPULangParser::LB)
      | (1ULL << GPULangParser::NOT)
      | (1ULL << GPULangParser::QO)
      | (1ULL << GPULangParser::CONNJUGATE)
      | (1ULL << GPULangParser::Q))) != 0) || ((((_la - 69) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 69)) & ((1ULL << (GPULangParser::SOBAKA - 69))
      | (1ULL << (GPULangParser::ADD_OP - 69))
      | (1ULL << (GPULangParser::SUB_OP - 69))
      | (1ULL << (GPULangParser::MUL_OP - 69))
      | (1ULL << (GPULangParser::INTEGERLITERAL - 69))
      | (1ULL << (GPULangParser::UINTEGERLITERAL - 69))
      | (1ULL << (GPULangParser::FLOATLITERAL - 69))
      | (1ULL << (GPULangParser::DOUBLELITERAL - 69))
      | (1ULL << (GPULangParser::HEX - 69))
      | (1ULL << (GPULangParser::IDENTIFIER - 69)))) != 0)) {
      setState(581);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 44, _ctx)) {
      case 1: {
        setState(574);
        dynamic_cast<ScopeStatementContext *>(_localctx)->statementContext = statement();
         contents.push_back(dynamic_cast<ScopeStatementContext *>(_localctx)->statementContext->tree); 
        break;
      }

      case 2: {
        setState(577);
        dynamic_cast<ScopeStatementContext *>(_localctx)->variablesContext = variables();
        setState(578);
        match(GPULangParser::SC);
         for(Variable* var : dynamic_cast<ScopeStatementContext *>(_localctx)->variablesContext->list) { contents.push_back(var); } 
        break;
      }

      }
      setState(585);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(586);
    match(GPULangParser::RB);

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

GPULangParser::ReturnStatementContext::ReturnStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
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

          dynamic_cast<ReturnStatementContext *>(_localctx)->tree =  nullptr;
          Expression* returnValue = nullptr;
          Symbol::Location location;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(589);
    match(GPULangParser::T__16);
     location = SetupFile(); 
    setState(594);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << GPULangParser::T__0)
      | (1ULL << GPULangParser::T__1)
      | (1ULL << GPULangParser::T__31)
      | (1ULL << GPULangParser::T__32)
      | (1ULL << GPULangParser::LP)
      | (1ULL << GPULangParser::LB)
      | (1ULL << GPULangParser::NOT)
      | (1ULL << GPULangParser::QO)
      | (1ULL << GPULangParser::CONNJUGATE)
      | (1ULL << GPULangParser::Q))) != 0) || ((((_la - 70) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 70)) & ((1ULL << (GPULangParser::ADD_OP - 70))
      | (1ULL << (GPULangParser::SUB_OP - 70))
      | (1ULL << (GPULangParser::MUL_OP - 70))
      | (1ULL << (GPULangParser::INTEGERLITERAL - 70))
      | (1ULL << (GPULangParser::UINTEGERLITERAL - 70))
      | (1ULL << (GPULangParser::FLOATLITERAL - 70))
      | (1ULL << (GPULangParser::DOUBLELITERAL - 70))
      | (1ULL << (GPULangParser::HEX - 70))
      | (1ULL << (GPULangParser::IDENTIFIER - 70)))) != 0)) {
      setState(591);
      dynamic_cast<ReturnStatementContext *>(_localctx)->value = expression();
       returnValue = dynamic_cast<ReturnStatementContext *>(_localctx)->value->tree; 
    }
    setState(596);
    match(GPULangParser::SC);

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

GPULangParser::ContinueStatementContext::ContinueStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
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

          dynamic_cast<ContinueStatementContext *>(_localctx)->tree =  nullptr;
          Symbol::Location location;
      

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(599);
    match(GPULangParser::T__17);
     location = SetupFile(); 
    setState(601);
    match(GPULangParser::SC);

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

GPULangParser::SwitchStatementContext::SwitchStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

GPULangParser::ExpressionContext* GPULangParser::SwitchStatementContext::expression() {
  return getRuleContext<GPULangParser::ExpressionContext>(0);
}

std::vector<tree::TerminalNode *> GPULangParser::SwitchStatementContext::IDENTIFIER() {
  return getTokens(GPULangParser::IDENTIFIER);
}

tree::TerminalNode* GPULangParser::SwitchStatementContext::IDENTIFIER(size_t i) {
  return getToken(GPULangParser::IDENTIFIER, i);
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
    setState(604);
    match(GPULangParser::T__18);
     location = SetupFile(); 
    setState(606);
    match(GPULangParser::LP);
    setState(607);
    dynamic_cast<SwitchStatementContext *>(_localctx)->expressionContext = expression();
    setState(608);
    match(GPULangParser::RP);
     switchExpression = dynamic_cast<SwitchStatementContext *>(_localctx)->expressionContext->tree; 
    setState(610);
    match(GPULangParser::LB);
    setState(619);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == GPULangParser::T__19) {
      setState(611);
      match(GPULangParser::T__19);
      setState(612);
      dynamic_cast<SwitchStatementContext *>(_localctx)->identifierToken = match(GPULangParser::IDENTIFIER);
      setState(613);
      match(GPULangParser::COL);
      setState(614);
      dynamic_cast<SwitchStatementContext *>(_localctx)->statementContext = statement();
       
                      caseValues.push_back((dynamic_cast<SwitchStatementContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<SwitchStatementContext *>(_localctx)->identifierToken->getText() : "")); 
                      caseStatements.push_back(dynamic_cast<SwitchStatementContext *>(_localctx)->statementContext->tree);
                  
      setState(621);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(627);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == GPULangParser::T__20) {
      setState(622);
      match(GPULangParser::T__20);
      setState(623);
      match(GPULangParser::COL);
      setState(624);
      dynamic_cast<SwitchStatementContext *>(_localctx)->statementContext = statement();

                      defaultStatement = dynamic_cast<SwitchStatementContext *>(_localctx)->statementContext->tree;
                  
    }
    setState(629);
    match(GPULangParser::RB);

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

GPULangParser::BreakStatementContext::BreakStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
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

          dynamic_cast<BreakStatementContext *>(_localctx)->tree =  nullptr;
          Symbol::Location location;
      

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(632);
    match(GPULangParser::T__21);
    setState(633);
    match(GPULangParser::SC);

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

          dynamic_cast<ExpressionContext *>(_localctx)->tree =  nullptr;
      

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(636);
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

GPULangParser::CommaExpressionContext::CommaExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<GPULangParser::AssignmentExpressionContext *> GPULangParser::CommaExpressionContext::assignmentExpression() {
  return getRuleContexts<GPULangParser::AssignmentExpressionContext>();
}

GPULangParser::AssignmentExpressionContext* GPULangParser::CommaExpressionContext::assignmentExpression(size_t i) {
  return getRuleContext<GPULangParser::AssignmentExpressionContext>(i);
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

          dynamic_cast<CommaExpressionContext *>(_localctx)->tree =  nullptr;
      

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(639);
    dynamic_cast<CommaExpressionContext *>(_localctx)->e1 = assignmentExpression();
     dynamic_cast<CommaExpressionContext *>(_localctx)->tree =  dynamic_cast<CommaExpressionContext *>(_localctx)->e1->tree; 
    setState(647);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 49, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(641);
        match(GPULangParser::CO);
        setState(642);
        dynamic_cast<CommaExpressionContext *>(_localctx)->e2 = assignmentExpression();

                    CommaExpression* expr = new CommaExpression(_localctx->tree, dynamic_cast<CommaExpressionContext *>(_localctx)->e2->tree);
                    expr->location = SetupFile();
                    dynamic_cast<CommaExpressionContext *>(_localctx)->tree =  expr;
                 
      }
      setState(649);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 49, _ctx);
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

std::vector<GPULangParser::ExpressionContext *> GPULangParser::AssignmentExpressionContext::expression() {
  return getRuleContexts<GPULangParser::ExpressionContext>();
}

GPULangParser::ExpressionContext* GPULangParser::AssignmentExpressionContext::expression(size_t i) {
  return getRuleContext<GPULangParser::ExpressionContext>(i);
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

          dynamic_cast<AssignmentExpressionContext *>(_localctx)->tree =  nullptr;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(650);
    dynamic_cast<AssignmentExpressionContext *>(_localctx)->e1 = logicalOrExpression();
     dynamic_cast<AssignmentExpressionContext *>(_localctx)->tree =  dynamic_cast<AssignmentExpressionContext *>(_localctx)->e1->tree; 
    setState(664);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 51, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(662);
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
            setState(652);
            dynamic_cast<AssignmentExpressionContext *>(_localctx)->op = _input->LT(1);
            _la = _input->LA(1);
            if (!((((_la & ~ 0x3fULL) == 0) &&
              ((1ULL << _la) & ((1ULL << GPULangParser::T__22)
              | (1ULL << GPULangParser::T__23)
              | (1ULL << GPULangParser::T__24)
              | (1ULL << GPULangParser::T__25)
              | (1ULL << GPULangParser::T__26)
              | (1ULL << GPULangParser::T__27)
              | (1ULL << GPULangParser::T__28)
              | (1ULL << GPULangParser::EQ)
              | (1ULL << GPULangParser::ANDSET)
              | (1ULL << GPULangParser::ORSET)
              | (1ULL << GPULangParser::XORSET))) != 0))) {
              dynamic_cast<AssignmentExpressionContext *>(_localctx)->op = _errHandler->recoverInline(this);
            }
            else {
              _errHandler->reportMatch(this);
              consume();
            }
            setState(653);
            dynamic_cast<AssignmentExpressionContext *>(_localctx)->e2 = logicalOrExpression();
             
                        BinaryExpression* expr = new BinaryExpression(StringToFourCC((dynamic_cast<AssignmentExpressionContext *>(_localctx)->op != nullptr ? dynamic_cast<AssignmentExpressionContext *>(_localctx)->op->getText() : "")), _localctx->tree, dynamic_cast<AssignmentExpressionContext *>(_localctx)->e2->tree);
                        expr->location = SetupFile();
                        dynamic_cast<AssignmentExpressionContext *>(_localctx)->tree =  expr;
                    
            break;
          }

          case GPULangParser::QU: {
            setState(656);
            match(GPULangParser::QU);
            setState(657);
            dynamic_cast<AssignmentExpressionContext *>(_localctx)->ifBody = expression();
            setState(658);
            match(GPULangParser::COL);
            setState(659);
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
      setState(666);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 51, _ctx);
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

          dynamic_cast<LogicalOrExpressionContext *>(_localctx)->tree =  nullptr;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(667);
    dynamic_cast<LogicalOrExpressionContext *>(_localctx)->e1 = logicalAndExpression();
     dynamic_cast<LogicalOrExpressionContext *>(_localctx)->tree =  dynamic_cast<LogicalOrExpressionContext *>(_localctx)->e1->tree; 
    setState(675);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == GPULangParser::LOGICOR) {
      setState(669);
      match(GPULangParser::LOGICOR);
      setState(670);
      dynamic_cast<LogicalOrExpressionContext *>(_localctx)->e2 = logicalAndExpression();

                  BinaryExpression* expr = new BinaryExpression('||', _localctx->tree, dynamic_cast<LogicalOrExpressionContext *>(_localctx)->e2->tree);
                  expr->location = SetupFile();
                  dynamic_cast<LogicalOrExpressionContext *>(_localctx)->tree =  expr;
              
      setState(677);
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

          dynamic_cast<LogicalAndExpressionContext *>(_localctx)->tree =  nullptr;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(678);
    dynamic_cast<LogicalAndExpressionContext *>(_localctx)->e1 = orExpression();
     dynamic_cast<LogicalAndExpressionContext *>(_localctx)->tree =  dynamic_cast<LogicalAndExpressionContext *>(_localctx)->e1->tree; 
    setState(686);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == GPULangParser::LOGICAND) {
      setState(680);
      match(GPULangParser::LOGICAND);
      setState(681);
      dynamic_cast<LogicalAndExpressionContext *>(_localctx)->e2 = orExpression();

                  BinaryExpression* expr = new BinaryExpression('&&', _localctx->tree, dynamic_cast<LogicalAndExpressionContext *>(_localctx)->e2->tree);
                  expr->location = SetupFile();
                  dynamic_cast<LogicalAndExpressionContext *>(_localctx)->tree =  expr;
              
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

          dynamic_cast<OrExpressionContext *>(_localctx)->tree =  nullptr;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(689);
    dynamic_cast<OrExpressionContext *>(_localctx)->e1 = xorExpression();
     dynamic_cast<OrExpressionContext *>(_localctx)->tree =  dynamic_cast<OrExpressionContext *>(_localctx)->e1->tree; 
    setState(697);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == GPULangParser::OR) {
      setState(691);
      match(GPULangParser::OR);
      setState(692);
      dynamic_cast<OrExpressionContext *>(_localctx)->e2 = xorExpression();

                  BinaryExpression* expr = new BinaryExpression('|', _localctx->tree, dynamic_cast<OrExpressionContext *>(_localctx)->e2->tree);
                  expr->location = SetupFile();
                  dynamic_cast<OrExpressionContext *>(_localctx)->tree =  expr;
              
      setState(699);
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

          dynamic_cast<XorExpressionContext *>(_localctx)->tree =  nullptr;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(700);
    dynamic_cast<XorExpressionContext *>(_localctx)->e1 = andExpression();
     dynamic_cast<XorExpressionContext *>(_localctx)->tree =  dynamic_cast<XorExpressionContext *>(_localctx)->e1->tree; 
    setState(708);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == GPULangParser::XOR) {
      setState(702);
      match(GPULangParser::XOR);
      setState(703);
      dynamic_cast<XorExpressionContext *>(_localctx)->e2 = andExpression();

                  BinaryExpression* expr = new BinaryExpression('^', _localctx->tree, dynamic_cast<XorExpressionContext *>(_localctx)->e2->tree);
                  expr->location = SetupFile();
                  dynamic_cast<XorExpressionContext *>(_localctx)->tree =  expr;
              
      setState(710);
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

          dynamic_cast<AndExpressionContext *>(_localctx)->tree =  nullptr;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(711);
    dynamic_cast<AndExpressionContext *>(_localctx)->e1 = equivalencyExpression();
     dynamic_cast<AndExpressionContext *>(_localctx)->tree =  dynamic_cast<AndExpressionContext *>(_localctx)->e1->tree;	
    setState(719);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == GPULangParser::AND) {
      setState(713);
      match(GPULangParser::AND);
      setState(714);
      dynamic_cast<AndExpressionContext *>(_localctx)->e2 = equivalencyExpression();

                  BinaryExpression* expr = new BinaryExpression('&', _localctx->tree, dynamic_cast<AndExpressionContext *>(_localctx)->e2->tree);
                  expr->location = SetupFile();
                  dynamic_cast<AndExpressionContext *>(_localctx)->tree =  expr;
              
      setState(721);
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

          dynamic_cast<EquivalencyExpressionContext *>(_localctx)->tree =  nullptr;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(722);
    dynamic_cast<EquivalencyExpressionContext *>(_localctx)->e1 = relationalExpression();
     dynamic_cast<EquivalencyExpressionContext *>(_localctx)->tree =  dynamic_cast<EquivalencyExpressionContext *>(_localctx)->e1->tree; 
    setState(730);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == GPULangParser::LOGICEQ

    || _la == GPULangParser::NOTEQ) {
      setState(724);
      dynamic_cast<EquivalencyExpressionContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!(_la == GPULangParser::LOGICEQ

      || _la == GPULangParser::NOTEQ)) {
        dynamic_cast<EquivalencyExpressionContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(725);
      dynamic_cast<EquivalencyExpressionContext *>(_localctx)->e2 = relationalExpression();

                  BinaryExpression* expr = new BinaryExpression(StringToFourCC((dynamic_cast<EquivalencyExpressionContext *>(_localctx)->op != nullptr ? dynamic_cast<EquivalencyExpressionContext *>(_localctx)->op->getText() : "")), _localctx->tree, dynamic_cast<EquivalencyExpressionContext *>(_localctx)->e2->tree);
                  expr->location = SetupFile();
                  dynamic_cast<EquivalencyExpressionContext *>(_localctx)->tree =  expr;
              
      setState(732);
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

          dynamic_cast<RelationalExpressionContext *>(_localctx)->tree =  nullptr;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(733);
    dynamic_cast<RelationalExpressionContext *>(_localctx)->e1 = shiftExpression();
     dynamic_cast<RelationalExpressionContext *>(_localctx)->tree =  dynamic_cast<RelationalExpressionContext *>(_localctx)->e1->tree; 
    setState(741);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << GPULangParser::LESS)
      | (1ULL << GPULangParser::LESSEQ)
      | (1ULL << GPULangParser::GREATER)
      | (1ULL << GPULangParser::GREATEREQ))) != 0)) {
      setState(735);
      dynamic_cast<RelationalExpressionContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << GPULangParser::LESS)
        | (1ULL << GPULangParser::LESSEQ)
        | (1ULL << GPULangParser::GREATER)
        | (1ULL << GPULangParser::GREATEREQ))) != 0))) {
        dynamic_cast<RelationalExpressionContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(736);
      dynamic_cast<RelationalExpressionContext *>(_localctx)->e2 = shiftExpression();

                  BinaryExpression* expr = new BinaryExpression(StringToFourCC((dynamic_cast<RelationalExpressionContext *>(_localctx)->op != nullptr ? dynamic_cast<RelationalExpressionContext *>(_localctx)->op->getText() : "")), _localctx->tree, dynamic_cast<RelationalExpressionContext *>(_localctx)->e2->tree);
                  expr->location = SetupFile();
                  dynamic_cast<RelationalExpressionContext *>(_localctx)->tree =  expr;
              
      setState(743);
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

          dynamic_cast<ShiftExpressionContext *>(_localctx)->tree =  nullptr;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(744);
    dynamic_cast<ShiftExpressionContext *>(_localctx)->e1 = addSubtractExpression();
     dynamic_cast<ShiftExpressionContext *>(_localctx)->tree =  dynamic_cast<ShiftExpressionContext *>(_localctx)->e1->tree; 
    setState(752);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == GPULangParser::T__29

    || _la == GPULangParser::T__30) {
      setState(746);
      dynamic_cast<ShiftExpressionContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!(_la == GPULangParser::T__29

      || _la == GPULangParser::T__30)) {
        dynamic_cast<ShiftExpressionContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(747);
      dynamic_cast<ShiftExpressionContext *>(_localctx)->e2 = addSubtractExpression();

                  BinaryExpression* expr = new BinaryExpression(StringToFourCC((dynamic_cast<ShiftExpressionContext *>(_localctx)->op != nullptr ? dynamic_cast<ShiftExpressionContext *>(_localctx)->op->getText() : "")), _localctx->tree, dynamic_cast<ShiftExpressionContext *>(_localctx)->e2->tree);
                  expr->location = SetupFile();
                  dynamic_cast<ShiftExpressionContext *>(_localctx)->tree =  expr;
              
      setState(754);
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

          dynamic_cast<AddSubtractExpressionContext *>(_localctx)->tree =  nullptr;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(755);
    dynamic_cast<AddSubtractExpressionContext *>(_localctx)->e1 = multiplyDivideExpression();
     dynamic_cast<AddSubtractExpressionContext *>(_localctx)->tree =  dynamic_cast<AddSubtractExpressionContext *>(_localctx)->e1->tree; 
    setState(763);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == GPULangParser::ADD_OP

    || _la == GPULangParser::SUB_OP) {
      setState(757);
      dynamic_cast<AddSubtractExpressionContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!(_la == GPULangParser::ADD_OP

      || _la == GPULangParser::SUB_OP)) {
        dynamic_cast<AddSubtractExpressionContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(758);
      dynamic_cast<AddSubtractExpressionContext *>(_localctx)->e2 = multiplyDivideExpression();

                  BinaryExpression* expr = new BinaryExpression(StringToFourCC((dynamic_cast<AddSubtractExpressionContext *>(_localctx)->op != nullptr ? dynamic_cast<AddSubtractExpressionContext *>(_localctx)->op->getText() : "")), _localctx->tree, dynamic_cast<AddSubtractExpressionContext *>(_localctx)->e2->tree);
                  expr->location = SetupFile();
                  dynamic_cast<AddSubtractExpressionContext *>(_localctx)->tree =  expr;
              
      setState(765);
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

          dynamic_cast<MultiplyDivideExpressionContext *>(_localctx)->tree =  nullptr;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(766);
    dynamic_cast<MultiplyDivideExpressionContext *>(_localctx)->e1 = prefixExpression();
     dynamic_cast<MultiplyDivideExpressionContext *>(_localctx)->tree =  dynamic_cast<MultiplyDivideExpressionContext *>(_localctx)->e1->tree; 
    setState(774);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (((((_la - 67) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 67)) & ((1ULL << (GPULangParser::MOD - 67))
      | (1ULL << (GPULangParser::DIV_OP - 67))
      | (1ULL << (GPULangParser::MUL_OP - 67)))) != 0)) {
      setState(768);
      dynamic_cast<MultiplyDivideExpressionContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!(((((_la - 67) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 67)) & ((1ULL << (GPULangParser::MOD - 67))
        | (1ULL << (GPULangParser::DIV_OP - 67))
        | (1ULL << (GPULangParser::MUL_OP - 67)))) != 0))) {
        dynamic_cast<MultiplyDivideExpressionContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(769);
      dynamic_cast<MultiplyDivideExpressionContext *>(_localctx)->e2 = prefixExpression();

                  BinaryExpression* expr = new BinaryExpression(StringToFourCC((dynamic_cast<MultiplyDivideExpressionContext *>(_localctx)->op != nullptr ? dynamic_cast<MultiplyDivideExpressionContext *>(_localctx)->op->getText() : "")), _localctx->tree, dynamic_cast<MultiplyDivideExpressionContext *>(_localctx)->e2->tree);
                  expr->location = SetupFile();
                  dynamic_cast<MultiplyDivideExpressionContext *>(_localctx)->tree =  expr;
              
      setState(776);
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

          dynamic_cast<PrefixExpressionContext *>(_localctx)->tree =  nullptr;
          std::vector<uint32_t> ops;
          std::vector<Symbol::Location> locations;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(781);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (((((_la - 32) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 32)) & ((1ULL << (GPULangParser::T__31 - 32))
      | (1ULL << (GPULangParser::T__32 - 32))
      | (1ULL << (GPULangParser::NOT - 32))
      | (1ULL << (GPULangParser::CONNJUGATE - 32))
      | (1ULL << (GPULangParser::ADD_OP - 32))
      | (1ULL << (GPULangParser::SUB_OP - 32))
      | (1ULL << (GPULangParser::MUL_OP - 32)))) != 0)) {
      setState(777);
      dynamic_cast<PrefixExpressionContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!(((((_la - 32) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 32)) & ((1ULL << (GPULangParser::T__31 - 32))
        | (1ULL << (GPULangParser::T__32 - 32))
        | (1ULL << (GPULangParser::NOT - 32))
        | (1ULL << (GPULangParser::CONNJUGATE - 32))
        | (1ULL << (GPULangParser::ADD_OP - 32))
        | (1ULL << (GPULangParser::SUB_OP - 32))
        | (1ULL << (GPULangParser::MUL_OP - 32)))) != 0))) {
        dynamic_cast<PrefixExpressionContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
       ops.push_back(StringToFourCC((dynamic_cast<PrefixExpressionContext *>(_localctx)->op != nullptr ? dynamic_cast<PrefixExpressionContext *>(_localctx)->op->getText() : ""))); locations.push_back(SetupFile()); 
      setState(783);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(784);
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

GPULangParser::SuffixExpressionContext::SuffixExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

GPULangParser::BinaryexpatomContext* GPULangParser::SuffixExpressionContext::binaryexpatom() {
  return getRuleContext<GPULangParser::BinaryexpatomContext>(0);
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
    setState(840);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 69, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(787);
      dynamic_cast<SuffixExpressionContext *>(_localctx)->e1 = binaryexpatom();
      setState(792);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == GPULangParser::T__31

      || _la == GPULangParser::T__32) {
        setState(788);
        dynamic_cast<SuffixExpressionContext *>(_localctx)->op = _input->LT(1);
        _la = _input->LA(1);
        if (!(_la == GPULangParser::T__31

        || _la == GPULangParser::T__32)) {
          dynamic_cast<SuffixExpressionContext *>(_localctx)->op = _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
         ops.push_back(StringToFourCC((dynamic_cast<SuffixExpressionContext *>(_localctx)->op != nullptr ? dynamic_cast<SuffixExpressionContext *>(_localctx)->op->getText() : ""))); locations.push_back(SetupFile()); 
        setState(794);
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
      setState(797);
      dynamic_cast<SuffixExpressionContext *>(_localctx)->e1 = binaryexpatom();

              dynamic_cast<SuffixExpressionContext *>(_localctx)->tree =  dynamic_cast<SuffixExpressionContext *>(_localctx)->e1->tree;
          
      setState(837);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 68, _ctx);
      while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
        if (alt == 1) {
          setState(835);
          _errHandler->sync(this);
          switch (_input->LA(1)) {
            case GPULangParser::LP: {
              setState(799);
              match(GPULangParser::LP);
               location = SetupFile(); 
              setState(812);
              _errHandler->sync(this);

              _la = _input->LA(1);
              if ((((_la & ~ 0x3fULL) == 0) &&
                ((1ULL << _la) & ((1ULL << GPULangParser::T__0)
                | (1ULL << GPULangParser::T__1)
                | (1ULL << GPULangParser::T__31)
                | (1ULL << GPULangParser::T__32)
                | (1ULL << GPULangParser::LP)
                | (1ULL << GPULangParser::LB)
                | (1ULL << GPULangParser::NOT)
                | (1ULL << GPULangParser::QO)
                | (1ULL << GPULangParser::CONNJUGATE)
                | (1ULL << GPULangParser::Q))) != 0) || ((((_la - 70) & ~ 0x3fULL) == 0) &&
                ((1ULL << (_la - 70)) & ((1ULL << (GPULangParser::ADD_OP - 70))
                | (1ULL << (GPULangParser::SUB_OP - 70))
                | (1ULL << (GPULangParser::MUL_OP - 70))
                | (1ULL << (GPULangParser::INTEGERLITERAL - 70))
                | (1ULL << (GPULangParser::UINTEGERLITERAL - 70))
                | (1ULL << (GPULangParser::FLOATLITERAL - 70))
                | (1ULL << (GPULangParser::DOUBLELITERAL - 70))
                | (1ULL << (GPULangParser::HEX - 70))
                | (1ULL << (GPULangParser::IDENTIFIER - 70)))) != 0)) {
                setState(801);
                dynamic_cast<SuffixExpressionContext *>(_localctx)->arg0 = logicalOrExpression();
                 args.push_back(dynamic_cast<SuffixExpressionContext *>(_localctx)->arg0->tree); 
                setState(809);
                _errHandler->sync(this);
                _la = _input->LA(1);
                while (_la == GPULangParser::CO) {
                  setState(803);
                  match(GPULangParser::CO);
                  setState(804);
                  dynamic_cast<SuffixExpressionContext *>(_localctx)->argn = logicalOrExpression();
                   args.push_back(dynamic_cast<SuffixExpressionContext *>(_localctx)->argn->tree); 
                  setState(811);
                  _errHandler->sync(this);
                  _la = _input->LA(1);
                }
              }
              setState(814);
              match(GPULangParser::RP);

                          CallExpression* expr = new CallExpression(_localctx->tree, args);
                          expr->location = location;
                          dynamic_cast<SuffixExpressionContext *>(_localctx)->tree =  expr;
                      
              break;
            }

            case GPULangParser::DOT: {
              setState(816);
              match(GPULangParser::DOT);
               location = SetupFile(); 
              setState(818);
              dynamic_cast<SuffixExpressionContext *>(_localctx)->e2 = suffixExpression();

                          AccessExpression* expr = new AccessExpression(_localctx->tree, dynamic_cast<SuffixExpressionContext *>(_localctx)->e2->tree, false);
                          expr->location = location;
                          dynamic_cast<SuffixExpressionContext *>(_localctx)->tree =  expr;
                      
              break;
            }

            case GPULangParser::ARROW: {
              setState(821);
              match(GPULangParser::ARROW);
               location = SetupFile(); 
              setState(823);
              dynamic_cast<SuffixExpressionContext *>(_localctx)->e2 = suffixExpression();

                          AccessExpression* expr = new AccessExpression(_localctx->tree, dynamic_cast<SuffixExpressionContext *>(_localctx)->e2->tree, true);
                          expr->location = location;
                          dynamic_cast<SuffixExpressionContext *>(_localctx)->tree =  expr;
                      
              break;
            }

            case GPULangParser::LL: {
              setState(826);
              match(GPULangParser::LL);
               location = SetupFile(); 
              setState(831);
              _errHandler->sync(this);

              _la = _input->LA(1);
              if ((((_la & ~ 0x3fULL) == 0) &&
                ((1ULL << _la) & ((1ULL << GPULangParser::T__0)
                | (1ULL << GPULangParser::T__1)
                | (1ULL << GPULangParser::T__31)
                | (1ULL << GPULangParser::T__32)
                | (1ULL << GPULangParser::LP)
                | (1ULL << GPULangParser::LB)
                | (1ULL << GPULangParser::NOT)
                | (1ULL << GPULangParser::QO)
                | (1ULL << GPULangParser::CONNJUGATE)
                | (1ULL << GPULangParser::Q))) != 0) || ((((_la - 70) & ~ 0x3fULL) == 0) &&
                ((1ULL << (_la - 70)) & ((1ULL << (GPULangParser::ADD_OP - 70))
                | (1ULL << (GPULangParser::SUB_OP - 70))
                | (1ULL << (GPULangParser::MUL_OP - 70))
                | (1ULL << (GPULangParser::INTEGERLITERAL - 70))
                | (1ULL << (GPULangParser::UINTEGERLITERAL - 70))
                | (1ULL << (GPULangParser::FLOATLITERAL - 70))
                | (1ULL << (GPULangParser::DOUBLELITERAL - 70))
                | (1ULL << (GPULangParser::HEX - 70))
                | (1ULL << (GPULangParser::IDENTIFIER - 70)))) != 0)) {
                setState(828);
                dynamic_cast<SuffixExpressionContext *>(_localctx)->e3 = expression();
                 arrayIndexExpr = dynamic_cast<SuffixExpressionContext *>(_localctx)->e3->tree; 
              }
              setState(833);
              match(GPULangParser::RR);

                          ArrayIndexExpression* expr = new ArrayIndexExpression(_localctx->tree, arrayIndexExpr);
                          expr->location = location;
                          dynamic_cast<SuffixExpressionContext *>(_localctx)->tree =  expr;
                      
              break;
            }

          default:
            throw NoViableAltException(this);
          } 
        }
        setState(839);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 68, _ctx);
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

          dynamic_cast<NamespaceExpressionContext *>(_localctx)->tree =  nullptr;
      

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(842);
    dynamic_cast<NamespaceExpressionContext *>(_localctx)->e1 = binaryexpatom();
    setState(843);
    match(GPULangParser::T__33);
    setState(844);
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

GPULangParser::ExpressionContext* GPULangParser::BinaryexpatomContext::expression() {
  return getRuleContext<GPULangParser::ExpressionContext>(0);
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

          dynamic_cast<BinaryexpatomContext *>(_localctx)->tree =  nullptr;
      

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(873);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case GPULangParser::INTEGERLITERAL: {
        enterOuterAlt(_localctx, 1);
        setState(847);
        dynamic_cast<BinaryexpatomContext *>(_localctx)->integerliteralToken = match(GPULangParser::INTEGERLITERAL);
         dynamic_cast<BinaryexpatomContext *>(_localctx)->tree =  new IntExpression(atoi((dynamic_cast<BinaryexpatomContext *>(_localctx)->integerliteralToken != nullptr ? dynamic_cast<BinaryexpatomContext *>(_localctx)->integerliteralToken->getText() : "").c_str())); _localctx->tree->location = SetupFile(); 
        break;
      }

      case GPULangParser::UINTEGERLITERAL: {
        enterOuterAlt(_localctx, 2);
        setState(849);
        dynamic_cast<BinaryexpatomContext *>(_localctx)->uintegerliteralToken = match(GPULangParser::UINTEGERLITERAL);
         dynamic_cast<BinaryexpatomContext *>(_localctx)->tree =  new UIntExpression(strtoul((dynamic_cast<BinaryexpatomContext *>(_localctx)->uintegerliteralToken != nullptr ? dynamic_cast<BinaryexpatomContext *>(_localctx)->uintegerliteralToken->getText() : "").c_str(), nullptr, 10)); _localctx->tree->location = SetupFile(); 
        break;
      }

      case GPULangParser::FLOATLITERAL: {
        enterOuterAlt(_localctx, 3);
        setState(851);
        dynamic_cast<BinaryexpatomContext *>(_localctx)->floatliteralToken = match(GPULangParser::FLOATLITERAL);
         dynamic_cast<BinaryexpatomContext *>(_localctx)->tree =  new FloatExpression(atof((dynamic_cast<BinaryexpatomContext *>(_localctx)->floatliteralToken != nullptr ? dynamic_cast<BinaryexpatomContext *>(_localctx)->floatliteralToken->getText() : "").c_str())); _localctx->tree->location = SetupFile(); 
        break;
      }

      case GPULangParser::DOUBLELITERAL: {
        enterOuterAlt(_localctx, 4);
        setState(853);
        dynamic_cast<BinaryexpatomContext *>(_localctx)->doubleliteralToken = match(GPULangParser::DOUBLELITERAL);
         dynamic_cast<BinaryexpatomContext *>(_localctx)->tree =  new FloatExpression(atof((dynamic_cast<BinaryexpatomContext *>(_localctx)->doubleliteralToken != nullptr ? dynamic_cast<BinaryexpatomContext *>(_localctx)->doubleliteralToken->getText() : "").c_str())); _localctx->tree->location = SetupFile(); 
        break;
      }

      case GPULangParser::HEX: {
        enterOuterAlt(_localctx, 5);
        setState(855);
        dynamic_cast<BinaryexpatomContext *>(_localctx)->hexToken = match(GPULangParser::HEX);
         dynamic_cast<BinaryexpatomContext *>(_localctx)->tree =  new UIntExpression(strtoul((dynamic_cast<BinaryexpatomContext *>(_localctx)->hexToken != nullptr ? dynamic_cast<BinaryexpatomContext *>(_localctx)->hexToken->getText() : "").c_str(), nullptr, 16)); _localctx->tree->location = SetupFile(); 
        break;
      }

      case GPULangParser::QO:
      case GPULangParser::Q: {
        enterOuterAlt(_localctx, 6);
        setState(857);
        dynamic_cast<BinaryexpatomContext *>(_localctx)->stringContext = string();
         dynamic_cast<BinaryexpatomContext *>(_localctx)->tree =  new StringExpression(dynamic_cast<BinaryexpatomContext *>(_localctx)->stringContext->val); _localctx->tree->location = SetupFile(); 
        break;
      }

      case GPULangParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 7);
        setState(860);
        dynamic_cast<BinaryexpatomContext *>(_localctx)->identifierToken = match(GPULangParser::IDENTIFIER);
         dynamic_cast<BinaryexpatomContext *>(_localctx)->tree =  new SymbolExpression((dynamic_cast<BinaryexpatomContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<BinaryexpatomContext *>(_localctx)->identifierToken->getText() : "")); _localctx->tree->location = SetupFile(); 
        break;
      }

      case GPULangParser::T__0:
      case GPULangParser::T__1: {
        enterOuterAlt(_localctx, 8);
        setState(862);
        dynamic_cast<BinaryexpatomContext *>(_localctx)->booleanContext = boolean();
         dynamic_cast<BinaryexpatomContext *>(_localctx)->tree =  new BoolExpression(dynamic_cast<BinaryexpatomContext *>(_localctx)->booleanContext->val); _localctx->tree->location = SetupFile(); 
        break;
      }

      case GPULangParser::LB: {
        enterOuterAlt(_localctx, 9);
        setState(865);
        dynamic_cast<BinaryexpatomContext *>(_localctx)->initializerExpressionContext = initializerExpression();
         dynamic_cast<BinaryexpatomContext *>(_localctx)->tree =  dynamic_cast<BinaryexpatomContext *>(_localctx)->initializerExpressionContext->tree; 
        break;
      }

      case GPULangParser::LP: {
        enterOuterAlt(_localctx, 10);
        setState(868);
        match(GPULangParser::LP);
        setState(869);
        dynamic_cast<BinaryexpatomContext *>(_localctx)->expressionContext = expression();
        setState(870);
        match(GPULangParser::RP);
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

GPULangParser::InitializerExpressionContext::InitializerExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<GPULangParser::AssignmentExpressionContext *> GPULangParser::InitializerExpressionContext::assignmentExpression() {
  return getRuleContexts<GPULangParser::AssignmentExpressionContext>();
}

GPULangParser::AssignmentExpressionContext* GPULangParser::InitializerExpressionContext::assignmentExpression(size_t i) {
  return getRuleContext<GPULangParser::AssignmentExpressionContext>(i);
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

          dynamic_cast<InitializerExpressionContext *>(_localctx)->tree =  nullptr;
          std::vector<Expression*> exprs;
      
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(875);
    match(GPULangParser::LB);
    setState(887);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << GPULangParser::T__0)
      | (1ULL << GPULangParser::T__1)
      | (1ULL << GPULangParser::T__31)
      | (1ULL << GPULangParser::T__32)
      | (1ULL << GPULangParser::LP)
      | (1ULL << GPULangParser::LB)
      | (1ULL << GPULangParser::NOT)
      | (1ULL << GPULangParser::QO)
      | (1ULL << GPULangParser::CONNJUGATE)
      | (1ULL << GPULangParser::Q))) != 0) || ((((_la - 70) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 70)) & ((1ULL << (GPULangParser::ADD_OP - 70))
      | (1ULL << (GPULangParser::SUB_OP - 70))
      | (1ULL << (GPULangParser::MUL_OP - 70))
      | (1ULL << (GPULangParser::INTEGERLITERAL - 70))
      | (1ULL << (GPULangParser::UINTEGERLITERAL - 70))
      | (1ULL << (GPULangParser::FLOATLITERAL - 70))
      | (1ULL << (GPULangParser::DOUBLELITERAL - 70))
      | (1ULL << (GPULangParser::HEX - 70))
      | (1ULL << (GPULangParser::IDENTIFIER - 70)))) != 0)) {
      setState(876);
      dynamic_cast<InitializerExpressionContext *>(_localctx)->arg0 = assignmentExpression();
       exprs.push_back(dynamic_cast<InitializerExpressionContext *>(_localctx)->arg0->tree); 
      setState(884);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == GPULangParser::CO) {
        setState(878);
        match(GPULangParser::CO);
        setState(879);
        dynamic_cast<InitializerExpressionContext *>(_localctx)->argN = assignmentExpression();
         exprs.push_back(dynamic_cast<InitializerExpressionContext *>(_localctx)->argN->tree); 
        setState(886);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(889);
    match(GPULangParser::RB);

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
std::vector<dfa::DFA> GPULangParser::_decisionToDFA;
atn::PredictionContextCache GPULangParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN GPULangParser::_atn;
std::vector<uint16_t> GPULangParser::_serializedATN;

std::vector<std::string> GPULangParser::_ruleNames = {
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

std::vector<std::string> GPULangParser::_literalNames = {
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

std::vector<std::string> GPULangParser::_symbolicNames = {
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

dfa::Vocabulary GPULangParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> GPULangParser::_tokenNames;

GPULangParser::Initializer::Initializer() {
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
    0x3, 0x56, 0x37f, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
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
    0x7, 0xb, 0xe2, 0xa, 0xb, 0xc, 0xb, 0xe, 0xb, 0xe5, 0xb, 0xb, 0x3, 0xb, 
    0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x5, 0xb, 0xed, 0xa, 
    0xb, 0x3, 0xb, 0x7, 0xb, 0xf0, 0xa, 0xb, 0xc, 0xb, 0xe, 0xb, 0xf3, 0xb, 
    0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x5, 0xb, 0xfa, 
    0xa, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x7, 0xb, 0xff, 0xa, 0xb, 0xc, 
    0xb, 0xe, 0xb, 0x102, 0xb, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 
    0x3, 0xb, 0x3, 0xb, 0x5, 0xb, 0x10a, 0xa, 0xb, 0x3, 0xb, 0x7, 0xb, 0x10d, 
    0xa, 0xb, 0xc, 0xb, 0xe, 0xb, 0x110, 0xb, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 
    0xb, 0x3, 0xb, 0x3, 0xb, 0x5, 0xb, 0x117, 0xa, 0xb, 0x7, 0xb, 0x119, 
    0xa, 0xb, 0xc, 0xb, 0xe, 0xb, 0x11c, 0xb, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 
    0xc, 0x3, 0xc, 0x3, 0xc, 0x7, 0xc, 0x123, 0xa, 0xc, 0xc, 0xc, 0xe, 0xc, 
    0x126, 0xb, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x7, 0xc, 0x12c, 
    0xa, 0xc, 0xc, 0xc, 0xe, 0xc, 0x12f, 0xb, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 
    0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x5, 0xc, 0x137, 0xa, 0xc, 0x3, 0xc, 
    0x7, 0xc, 0x13a, 0xa, 0xc, 0xc, 0xc, 0xe, 0xc, 0x13d, 0xb, 0xc, 0x3, 
    0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x5, 0xc, 0x143, 0xa, 0xc, 0x3, 0xc, 
    0x3, 0xc, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x7, 0xd, 0x14a, 0xa, 0xd, 0xc, 
    0xd, 0xe, 0xd, 0x14d, 0xb, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 
    0x7, 0xd, 0x153, 0xa, 0xd, 0xc, 0xd, 0xe, 0xd, 0x156, 0xb, 0xd, 0x3, 
    0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 
    0xe, 0x3, 0xe, 0x3, 0xe, 0x7, 0xe, 0x162, 0xa, 0xe, 0xc, 0xe, 0xe, 0xe, 
    0x165, 0xb, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xf, 0x3, 0xf, 0x3, 
    0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x5, 
    0xf, 0x173, 0xa, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0x10, 
    0x3, 0x10, 0x3, 0x10, 0x7, 0x10, 0x17c, 0xa, 0x10, 0xc, 0x10, 0xe, 0x10, 
    0x17f, 0xb, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 
    0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x7, 0x10, 0x18b, 
    0xa, 0x10, 0xc, 0x10, 0xe, 0x10, 0x18e, 0xb, 0x10, 0x5, 0x10, 0x190, 
    0xa, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x11, 0x3, 0x11, 0x7, 
    0x11, 0x197, 0xa, 0x11, 0xc, 0x11, 0xe, 0x11, 0x19a, 0xb, 0x11, 0x3, 
    0x11, 0x3, 0x11, 0x5, 0x11, 0x19e, 0xa, 0x11, 0x3, 0x12, 0x3, 0x12, 
    0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x13, 0x3, 0x13, 0x3, 
    0x13, 0x7, 0x13, 0x1a9, 0xa, 0x13, 0xc, 0x13, 0xe, 0x13, 0x1ac, 0xb, 
    0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 
    0x3, 0x13, 0x3, 0x13, 0x7, 0x13, 0x1b6, 0xa, 0x13, 0xc, 0x13, 0xe, 0x13, 
    0x1b9, 0xb, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x14, 0x3, 0x14, 
    0x3, 0x14, 0x3, 0x14, 0x5, 0x14, 0x1c2, 0xa, 0x14, 0x3, 0x14, 0x3, 0x14, 
    0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x7, 0x14, 0x1cb, 
    0xa, 0x14, 0xc, 0x14, 0xe, 0x14, 0x1ce, 0xb, 0x14, 0x3, 0x14, 0x3, 0x14, 
    0x3, 0x14, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 
    0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 
    0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 
    0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 
    0x5, 0x15, 0x1ec, 0xa, 0x15, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x3, 0x17, 
    0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x3, 
    0x17, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x5, 0x17, 0x1fd, 
    0xa, 0x17, 0x3, 0x17, 0x3, 0x17, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 
    0x18, 0x3, 0x18, 0x3, 0x18, 0x5, 0x18, 0x207, 0xa, 0x18, 0x3, 0x18, 
    0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x5, 0x18, 0x20d, 0xa, 0x18, 0x3, 0x18, 
    0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x5, 0x18, 0x213, 0xa, 0x18, 0x3, 0x18, 
    0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x19, 0x3, 0x19, 0x3, 
    0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 
    0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x1a, 0x3, 0x1a, 0x3, 
    0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 
    0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 
    0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 
    0x5, 0x1a, 0x23d, 0xa, 0x1a, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 
    0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x7, 0x1b, 0x248, 
    0xa, 0x1b, 0xc, 0x1b, 0xe, 0x1b, 0x24b, 0xb, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 
    0x3, 0x1b, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x5, 
    0x1c, 0x255, 0xa, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1d, 
    0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1e, 0x3, 0x1e, 0x3, 
    0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 
    0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x7, 0x1e, 0x26c, 0xa, 0x1e, 
    0xc, 0x1e, 0xe, 0x1e, 0x26f, 0xb, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 
    0x3, 0x1e, 0x3, 0x1e, 0x5, 0x1e, 0x276, 0xa, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 
    0x3, 0x1e, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x20, 0x3, 
    0x20, 0x3, 0x20, 0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 
    0x3, 0x21, 0x7, 0x21, 0x288, 0xa, 0x21, 0xc, 0x21, 0xe, 0x21, 0x28b, 
    0xb, 0x21, 0x3, 0x22, 0x3, 0x22, 0x3, 0x22, 0x3, 0x22, 0x3, 0x22, 0x3, 
    0x22, 0x3, 0x22, 0x3, 0x22, 0x3, 0x22, 0x3, 0x22, 0x3, 0x22, 0x3, 0x22, 
    0x7, 0x22, 0x299, 0xa, 0x22, 0xc, 0x22, 0xe, 0x22, 0x29c, 0xb, 0x22, 
    0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x7, 
    0x23, 0x2a4, 0xa, 0x23, 0xc, 0x23, 0xe, 0x23, 0x2a7, 0xb, 0x23, 0x3, 
    0x24, 0x3, 0x24, 0x3, 0x24, 0x3, 0x24, 0x3, 0x24, 0x3, 0x24, 0x7, 0x24, 
    0x2af, 0xa, 0x24, 0xc, 0x24, 0xe, 0x24, 0x2b2, 0xb, 0x24, 0x3, 0x25, 
    0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 0x7, 0x25, 0x2ba, 
    0xa, 0x25, 0xc, 0x25, 0xe, 0x25, 0x2bd, 0xb, 0x25, 0x3, 0x26, 0x3, 0x26, 
    0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 0x7, 0x26, 0x2c5, 0xa, 0x26, 
    0xc, 0x26, 0xe, 0x26, 0x2c8, 0xb, 0x26, 0x3, 0x27, 0x3, 0x27, 0x3, 0x27, 
    0x3, 0x27, 0x3, 0x27, 0x3, 0x27, 0x7, 0x27, 0x2d0, 0xa, 0x27, 0xc, 0x27, 
    0xe, 0x27, 0x2d3, 0xb, 0x27, 0x3, 0x28, 0x3, 0x28, 0x3, 0x28, 0x3, 0x28, 
    0x3, 0x28, 0x3, 0x28, 0x7, 0x28, 0x2db, 0xa, 0x28, 0xc, 0x28, 0xe, 0x28, 
    0x2de, 0xb, 0x28, 0x3, 0x29, 0x3, 0x29, 0x3, 0x29, 0x3, 0x29, 0x3, 0x29, 
    0x3, 0x29, 0x7, 0x29, 0x2e6, 0xa, 0x29, 0xc, 0x29, 0xe, 0x29, 0x2e9, 
    0xb, 0x29, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x3, 
    0x2a, 0x7, 0x2a, 0x2f1, 0xa, 0x2a, 0xc, 0x2a, 0xe, 0x2a, 0x2f4, 0xb, 
    0x2a, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 
    0x7, 0x2b, 0x2fc, 0xa, 0x2b, 0xc, 0x2b, 0xe, 0x2b, 0x2ff, 0xb, 0x2b, 
    0x3, 0x2c, 0x3, 0x2c, 0x3, 0x2c, 0x3, 0x2c, 0x3, 0x2c, 0x3, 0x2c, 0x7, 
    0x2c, 0x307, 0xa, 0x2c, 0xc, 0x2c, 0xe, 0x2c, 0x30a, 0xb, 0x2c, 0x3, 
    0x2d, 0x3, 0x2d, 0x7, 0x2d, 0x30e, 0xa, 0x2d, 0xc, 0x2d, 0xe, 0x2d, 
    0x311, 0xb, 0x2d, 0x3, 0x2d, 0x3, 0x2d, 0x3, 0x2d, 0x3, 0x2e, 0x3, 0x2e, 
    0x3, 0x2e, 0x7, 0x2e, 0x319, 0xa, 0x2e, 0xc, 0x2e, 0xe, 0x2e, 0x31c, 
    0xb, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 
    0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 
    0x7, 0x2e, 0x32a, 0xa, 0x2e, 0xc, 0x2e, 0xe, 0x2e, 0x32d, 0xb, 0x2e, 
    0x5, 0x2e, 0x32f, 0xa, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 
    0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 
    0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 
    0x5, 0x2e, 0x342, 0xa, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x7, 0x2e, 0x346, 
    0xa, 0x2e, 0xc, 0x2e, 0xe, 0x2e, 0x349, 0xb, 0x2e, 0x5, 0x2e, 0x34b, 
    0xa, 0x2e, 0x3, 0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x3, 
    0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 
    0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 
    0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 
    0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x5, 
    0x30, 0x36c, 0xa, 0x30, 0x3, 0x31, 0x3, 0x31, 0x3, 0x31, 0x3, 0x31, 
    0x3, 0x31, 0x3, 0x31, 0x3, 0x31, 0x7, 0x31, 0x375, 0xa, 0x31, 0xc, 0x31, 
    0xe, 0x31, 0x378, 0xb, 0x31, 0x5, 0x31, 0x37a, 0xa, 0x31, 0x3, 0x31, 
    0x3, 0x31, 0x3, 0x31, 0x3, 0x31, 0x4, 0x85, 0xaf, 0x2, 0x32, 0x2, 0x4, 
    0x6, 0x8, 0xa, 0xc, 0xe, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1a, 0x1c, 0x1e, 
    0x20, 0x22, 0x24, 0x26, 0x28, 0x2a, 0x2c, 0x2e, 0x30, 0x32, 0x34, 0x36, 
    0x38, 0x3a, 0x3c, 0x3e, 0x40, 0x42, 0x44, 0x46, 0x48, 0x4a, 0x4c, 0x4e, 
    0x50, 0x52, 0x54, 0x56, 0x58, 0x5a, 0x5c, 0x5e, 0x60, 0x2, 0xd, 0x3, 
    0x2, 0x31, 0x31, 0x3, 0x2, 0x3a, 0x3a, 0x3, 0x2, 0x2a, 0x2b, 0x7, 0x2, 
    0x19, 0x1f, 0x30, 0x30, 0x34, 0x34, 0x36, 0x36, 0x38, 0x38, 0x3, 0x2, 
    0x41, 0x42, 0x3, 0x2, 0x3d, 0x40, 0x3, 0x2, 0x20, 0x21, 0x3, 0x2, 0x48, 
    0x49, 0x4, 0x2, 0x45, 0x45, 0x4a, 0x4b, 0x7, 0x2, 0x22, 0x23, 0x2f, 
    0x2f, 0x39, 0x39, 0x48, 0x49, 0x4b, 0x4b, 0x3, 0x2, 0x22, 0x23, 0x2, 
    0x3ad, 0x2, 0x74, 0x3, 0x2, 0x2, 0x2, 0x4, 0x7a, 0x3, 0x2, 0x2, 0x2, 
    0x6, 0x85, 0x3, 0x2, 0x2, 0x2, 0x8, 0x8a, 0x3, 0x2, 0x2, 0x2, 0xa, 0xaf, 
    0x3, 0x2, 0x2, 0x2, 0xc, 0xb2, 0x3, 0x2, 0x2, 0x2, 0xe, 0xb9, 0x3, 0x2, 
    0x2, 0x2, 0x10, 0xc8, 0x3, 0x2, 0x2, 0x2, 0x12, 0xca, 0x3, 0x2, 0x2, 
    0x2, 0x14, 0xd2, 0x3, 0x2, 0x2, 0x2, 0x16, 0x124, 0x3, 0x2, 0x2, 0x2, 
    0x18, 0x14b, 0x3, 0x2, 0x2, 0x2, 0x1a, 0x15a, 0x3, 0x2, 0x2, 0x2, 0x1c, 
    0x169, 0x3, 0x2, 0x2, 0x2, 0x1e, 0x17d, 0x3, 0x2, 0x2, 0x2, 0x20, 0x19d, 
    0x3, 0x2, 0x2, 0x2, 0x22, 0x19f, 0x3, 0x2, 0x2, 0x2, 0x24, 0x1aa, 0x3, 
    0x2, 0x2, 0x2, 0x26, 0x1c1, 0x3, 0x2, 0x2, 0x2, 0x28, 0x1eb, 0x3, 0x2, 
    0x2, 0x2, 0x2a, 0x1ed, 0x3, 0x2, 0x2, 0x2, 0x2c, 0x1f0, 0x3, 0x2, 0x2, 
    0x2, 0x2e, 0x200, 0x3, 0x2, 0x2, 0x2, 0x30, 0x219, 0x3, 0x2, 0x2, 0x2, 
    0x32, 0x23c, 0x3, 0x2, 0x2, 0x2, 0x34, 0x23e, 0x3, 0x2, 0x2, 0x2, 0x36, 
    0x24f, 0x3, 0x2, 0x2, 0x2, 0x38, 0x259, 0x3, 0x2, 0x2, 0x2, 0x3a, 0x25e, 
    0x3, 0x2, 0x2, 0x2, 0x3c, 0x27a, 0x3, 0x2, 0x2, 0x2, 0x3e, 0x27e, 0x3, 
    0x2, 0x2, 0x2, 0x40, 0x281, 0x3, 0x2, 0x2, 0x2, 0x42, 0x28c, 0x3, 0x2, 
    0x2, 0x2, 0x44, 0x29d, 0x3, 0x2, 0x2, 0x2, 0x46, 0x2a8, 0x3, 0x2, 0x2, 
    0x2, 0x48, 0x2b3, 0x3, 0x2, 0x2, 0x2, 0x4a, 0x2be, 0x3, 0x2, 0x2, 0x2, 
    0x4c, 0x2c9, 0x3, 0x2, 0x2, 0x2, 0x4e, 0x2d4, 0x3, 0x2, 0x2, 0x2, 0x50, 
    0x2df, 0x3, 0x2, 0x2, 0x2, 0x52, 0x2ea, 0x3, 0x2, 0x2, 0x2, 0x54, 0x2f5, 
    0x3, 0x2, 0x2, 0x2, 0x56, 0x300, 0x3, 0x2, 0x2, 0x2, 0x58, 0x30f, 0x3, 
    0x2, 0x2, 0x2, 0x5a, 0x34a, 0x3, 0x2, 0x2, 0x2, 0x5c, 0x34c, 0x3, 0x2, 
    0x2, 0x2, 0x5e, 0x36b, 0x3, 0x2, 0x2, 0x2, 0x60, 0x36d, 0x3, 0x2, 0x2, 
    0x2, 0x62, 0x67, 0x7, 0x31, 0x2, 0x2, 0x63, 0x64, 0xa, 0x2, 0x2, 0x2, 
    0x64, 0x66, 0x8, 0x2, 0x1, 0x2, 0x65, 0x63, 0x3, 0x2, 0x2, 0x2, 0x66, 
    0x69, 0x3, 0x2, 0x2, 0x2, 0x67, 0x65, 0x3, 0x2, 0x2, 0x2, 0x67, 0x68, 
    0x3, 0x2, 0x2, 0x2, 0x68, 0x6a, 0x3, 0x2, 0x2, 0x2, 0x69, 0x67, 0x3, 
    0x2, 0x2, 0x2, 0x6a, 0x75, 0x7, 0x31, 0x2, 0x2, 0x6b, 0x70, 0x7, 0x3a, 
    0x2, 0x2, 0x6c, 0x6d, 0xa, 0x3, 0x2, 0x2, 0x6d, 0x6f, 0x8, 0x2, 0x1, 
    0x2, 0x6e, 0x6c, 0x3, 0x2, 0x2, 0x2, 0x6f, 0x72, 0x3, 0x2, 0x2, 0x2, 
    0x70, 0x6e, 0x3, 0x2, 0x2, 0x2, 0x70, 0x71, 0x3, 0x2, 0x2, 0x2, 0x71, 
    0x73, 0x3, 0x2, 0x2, 0x2, 0x72, 0x70, 0x3, 0x2, 0x2, 0x2, 0x73, 0x75, 
    0x7, 0x3a, 0x2, 0x2, 0x74, 0x62, 0x3, 0x2, 0x2, 0x2, 0x74, 0x6b, 0x3, 
    0x2, 0x2, 0x2, 0x75, 0x3, 0x3, 0x2, 0x2, 0x2, 0x76, 0x77, 0x7, 0x3, 
    0x2, 0x2, 0x77, 0x7b, 0x8, 0x3, 0x1, 0x2, 0x78, 0x79, 0x7, 0x4, 0x2, 
    0x2, 0x79, 0x7b, 0x8, 0x3, 0x1, 0x2, 0x7a, 0x76, 0x3, 0x2, 0x2, 0x2, 
    0x7a, 0x78, 0x3, 0x2, 0x2, 0x2, 0x7b, 0x5, 0x3, 0x2, 0x2, 0x2, 0x7c, 
    0x7d, 0x8, 0x4, 0x1, 0x2, 0x7d, 0x7e, 0x7, 0x5, 0x2, 0x2, 0x7e, 0x7f, 
    0x7, 0x4d, 0x2, 0x2, 0x7f, 0x80, 0x5, 0x2, 0x2, 0x2, 0x80, 0x81, 0x8, 
    0x4, 0x1, 0x2, 0x81, 0x84, 0x3, 0x2, 0x2, 0x2, 0x82, 0x84, 0xb, 0x2, 
    0x2, 0x2, 0x83, 0x7c, 0x3, 0x2, 0x2, 0x2, 0x83, 0x82, 0x3, 0x2, 0x2, 
    0x2, 0x84, 0x87, 0x3, 0x2, 0x2, 0x2, 0x85, 0x86, 0x3, 0x2, 0x2, 0x2, 
    0x85, 0x83, 0x3, 0x2, 0x2, 0x2, 0x86, 0x88, 0x3, 0x2, 0x2, 0x2, 0x87, 
    0x85, 0x3, 0x2, 0x2, 0x2, 0x88, 0x89, 0x7, 0x2, 0x2, 0x3, 0x89, 0x7, 
    0x3, 0x2, 0x2, 0x2, 0x8a, 0x8b, 0x5, 0xa, 0x6, 0x2, 0x8b, 0x8c, 0x8, 
    0x5, 0x1, 0x2, 0x8c, 0x8d, 0x7, 0x2, 0x2, 0x3, 0x8d, 0x9, 0x3, 0x2, 
    0x2, 0x2, 0x8e, 0x8f, 0x5, 0xc, 0x7, 0x2, 0x8f, 0x90, 0x7, 0x25, 0x2, 
    0x2, 0x90, 0x91, 0x8, 0x6, 0x1, 0x2, 0x91, 0xae, 0x3, 0x2, 0x2, 0x2, 
    0x92, 0x93, 0x5, 0x1e, 0x10, 0x2, 0x93, 0x94, 0x7, 0x25, 0x2, 0x2, 0x94, 
    0x95, 0x8, 0x6, 0x1, 0x2, 0x95, 0xae, 0x3, 0x2, 0x2, 0x2, 0x96, 0x97, 
    0x5, 0x22, 0x12, 0x2, 0x97, 0x98, 0x8, 0x6, 0x1, 0x2, 0x98, 0xae, 0x3, 
    0x2, 0x2, 0x2, 0x99, 0x9a, 0x5, 0x14, 0xb, 0x2, 0x9a, 0x9b, 0x7, 0x25, 
    0x2, 0x2, 0x9b, 0x9c, 0x8, 0x6, 0x1, 0x2, 0x9c, 0xae, 0x3, 0x2, 0x2, 
    0x2, 0x9d, 0x9e, 0x5, 0x1a, 0xe, 0x2, 0x9e, 0x9f, 0x7, 0x25, 0x2, 0x2, 
    0x9f, 0xa0, 0x8, 0x6, 0x1, 0x2, 0xa0, 0xae, 0x3, 0x2, 0x2, 0x2, 0xa1, 
    0xa2, 0x5, 0x1c, 0xf, 0x2, 0xa2, 0xa3, 0x7, 0x25, 0x2, 0x2, 0xa3, 0xa4, 
    0x8, 0x6, 0x1, 0x2, 0xa4, 0xae, 0x3, 0x2, 0x2, 0x2, 0xa5, 0xa6, 0x5, 
    0x26, 0x14, 0x2, 0xa6, 0xa7, 0x7, 0x25, 0x2, 0x2, 0xa7, 0xa8, 0x8, 0x6, 
    0x1, 0x2, 0xa8, 0xae, 0x3, 0x2, 0x2, 0x2, 0xa9, 0xaa, 0x5, 0x24, 0x13, 
    0x2, 0xaa, 0xab, 0x7, 0x25, 0x2, 0x2, 0xab, 0xac, 0x8, 0x6, 0x1, 0x2, 
    0xac, 0xae, 0x3, 0x2, 0x2, 0x2, 0xad, 0x8e, 0x3, 0x2, 0x2, 0x2, 0xad, 
    0x92, 0x3, 0x2, 0x2, 0x2, 0xad, 0x96, 0x3, 0x2, 0x2, 0x2, 0xad, 0x99, 
    0x3, 0x2, 0x2, 0x2, 0xad, 0x9d, 0x3, 0x2, 0x2, 0x2, 0xad, 0xa1, 0x3, 
    0x2, 0x2, 0x2, 0xad, 0xa5, 0x3, 0x2, 0x2, 0x2, 0xad, 0xa9, 0x3, 0x2, 
    0x2, 0x2, 0xae, 0xb1, 0x3, 0x2, 0x2, 0x2, 0xaf, 0xb0, 0x3, 0x2, 0x2, 
    0x2, 0xaf, 0xad, 0x3, 0x2, 0x2, 0x2, 0xb0, 0xb, 0x3, 0x2, 0x2, 0x2, 
    0xb1, 0xaf, 0x3, 0x2, 0x2, 0x2, 0xb2, 0xb3, 0x7, 0x6, 0x2, 0x2, 0xb3, 
    0xb4, 0x7, 0x55, 0x2, 0x2, 0xb4, 0xb5, 0x7, 0x7, 0x2, 0x2, 0xb5, 0xb6, 
    0x7, 0x55, 0x2, 0x2, 0xb6, 0xb7, 0x8, 0x7, 0x1, 0x2, 0xb7, 0xb8, 0x8, 
    0x7, 0x1, 0x2, 0xb8, 0xd, 0x3, 0x2, 0x2, 0x2, 0xb9, 0xba, 0x7, 0x47, 
    0x2, 0x2, 0xba, 0xbb, 0x7, 0x55, 0x2, 0x2, 0xbb, 0xbc, 0x7, 0x28, 0x2, 
    0x2, 0xbc, 0xbd, 0x5, 0x3e, 0x20, 0x2, 0xbd, 0xbe, 0x7, 0x29, 0x2, 0x2, 
    0xbe, 0xbf, 0x8, 0x8, 0x1, 0x2, 0xbf, 0xf, 0x3, 0x2, 0x2, 0x2, 0xc0, 
    0xc1, 0x7, 0x55, 0x2, 0x2, 0xc1, 0xc2, 0x7, 0x28, 0x2, 0x2, 0xc2, 0xc3, 
    0x5, 0x3e, 0x20, 0x2, 0xc3, 0xc4, 0x7, 0x29, 0x2, 0x2, 0xc4, 0xc5, 0x8, 
    0x9, 0x1, 0x2, 0xc5, 0xc9, 0x3, 0x2, 0x2, 0x2, 0xc6, 0xc7, 0x7, 0x55, 
    0x2, 0x2, 0xc7, 0xc9, 0x8, 0x9, 0x1, 0x2, 0xc8, 0xc0, 0x3, 0x2, 0x2, 
    0x2, 0xc8, 0xc6, 0x3, 0x2, 0x2, 0x2, 0xc9, 0x11, 0x3, 0x2, 0x2, 0x2, 
    0xca, 0xcb, 0x7, 0x55, 0x2, 0x2, 0xcb, 0xcc, 0x8, 0xa, 0x1, 0x2, 0xcc, 
    0x13, 0x3, 0x2, 0x2, 0x2, 0xcd, 0xce, 0x5, 0xe, 0x8, 0x2, 0xce, 0xcf, 
    0x8, 0xb, 0x1, 0x2, 0xcf, 0xd1, 0x3, 0x2, 0x2, 0x2, 0xd0, 0xcd, 0x3, 
    0x2, 0x2, 0x2, 0xd1, 0xd4, 0x3, 0x2, 0x2, 0x2, 0xd2, 0xd0, 0x3, 0x2, 
    0x2, 0x2, 0xd2, 0xd3, 0x3, 0x2, 0x2, 0x2, 0xd3, 0xda, 0x3, 0x2, 0x2, 
    0x2, 0xd4, 0xd2, 0x3, 0x2, 0x2, 0x2, 0xd5, 0xd6, 0x5, 0x10, 0x9, 0x2, 
    0xd6, 0xd7, 0x8, 0xb, 0x1, 0x2, 0xd7, 0xd9, 0x3, 0x2, 0x2, 0x2, 0xd8, 
    0xd5, 0x3, 0x2, 0x2, 0x2, 0xd9, 0xdc, 0x3, 0x2, 0x2, 0x2, 0xda, 0xd8, 
    0x3, 0x2, 0x2, 0x2, 0xda, 0xdb, 0x3, 0x2, 0x2, 0x2, 0xdb, 0xdd, 0x3, 
    0x2, 0x2, 0x2, 0xdc, 0xda, 0x3, 0x2, 0x2, 0x2, 0xdd, 0xde, 0x7, 0x55, 
    0x2, 0x2, 0xde, 0xe3, 0x8, 0xb, 0x1, 0x2, 0xdf, 0xe0, 0x7, 0x4b, 0x2, 
    0x2, 0xe0, 0xe2, 0x8, 0xb, 0x1, 0x2, 0xe1, 0xdf, 0x3, 0x2, 0x2, 0x2, 
    0xe2, 0xe5, 0x3, 0x2, 0x2, 0x2, 0xe3, 0xe1, 0x3, 0x2, 0x2, 0x2, 0xe3, 
    0xe4, 0x3, 0x2, 0x2, 0x2, 0xe4, 0xe6, 0x3, 0x2, 0x2, 0x2, 0xe5, 0xe3, 
    0x3, 0x2, 0x2, 0x2, 0xe6, 0xe7, 0x7, 0x55, 0x2, 0x2, 0xe7, 0xf1, 0x8, 
    0xb, 0x1, 0x2, 0xe8, 0xe9, 0x8, 0xb, 0x1, 0x2, 0xe9, 0xec, 0x7, 0x2c, 
    0x2, 0x2, 0xea, 0xeb, 0x7, 0x4d, 0x2, 0x2, 0xeb, 0xed, 0x8, 0xb, 0x1, 
    0x2, 0xec, 0xea, 0x3, 0x2, 0x2, 0x2, 0xec, 0xed, 0x3, 0x2, 0x2, 0x2, 
    0xed, 0xee, 0x3, 0x2, 0x2, 0x2, 0xee, 0xf0, 0x7, 0x2d, 0x2, 0x2, 0xef, 
    0xe8, 0x3, 0x2, 0x2, 0x2, 0xf0, 0xf3, 0x3, 0x2, 0x2, 0x2, 0xf1, 0xef, 
    0x3, 0x2, 0x2, 0x2, 0xf1, 0xf2, 0x3, 0x2, 0x2, 0x2, 0xf2, 0xf4, 0x3, 
    0x2, 0x2, 0x2, 0xf3, 0xf1, 0x3, 0x2, 0x2, 0x2, 0xf4, 0xf9, 0x8, 0xb, 
    0x1, 0x2, 0xf5, 0xf6, 0x7, 0x30, 0x2, 0x2, 0xf6, 0xf7, 0x5, 0x42, 0x22, 
    0x2, 0xf7, 0xf8, 0x8, 0xb, 0x1, 0x2, 0xf8, 0xfa, 0x3, 0x2, 0x2, 0x2, 
    0xf9, 0xf5, 0x3, 0x2, 0x2, 0x2, 0xf9, 0xfa, 0x3, 0x2, 0x2, 0x2, 0xfa, 
    0x11a, 0x3, 0x2, 0x2, 0x2, 0xfb, 0x100, 0x7, 0x26, 0x2, 0x2, 0xfc, 0xfd, 
    0x7, 0x4b, 0x2, 0x2, 0xfd, 0xff, 0x8, 0xb, 0x1, 0x2, 0xfe, 0xfc, 0x3, 
    0x2, 0x2, 0x2, 0xff, 0x102, 0x3, 0x2, 0x2, 0x2, 0x100, 0xfe, 0x3, 0x2, 
    0x2, 0x2, 0x100, 0x101, 0x3, 0x2, 0x2, 0x2, 0x101, 0x103, 0x3, 0x2, 
    0x2, 0x2, 0x102, 0x100, 0x3, 0x2, 0x2, 0x2, 0x103, 0x104, 0x7, 0x55, 
    0x2, 0x2, 0x104, 0x10e, 0x8, 0xb, 0x1, 0x2, 0x105, 0x106, 0x8, 0xb, 
    0x1, 0x2, 0x106, 0x109, 0x7, 0x2c, 0x2, 0x2, 0x107, 0x108, 0x7, 0x4d, 
    0x2, 0x2, 0x108, 0x10a, 0x8, 0xb, 0x1, 0x2, 0x109, 0x107, 0x3, 0x2, 
    0x2, 0x2, 0x109, 0x10a, 0x3, 0x2, 0x2, 0x2, 0x10a, 0x10b, 0x3, 0x2, 
    0x2, 0x2, 0x10b, 0x10d, 0x7, 0x2d, 0x2, 0x2, 0x10c, 0x105, 0x3, 0x2, 
    0x2, 0x2, 0x10d, 0x110, 0x3, 0x2, 0x2, 0x2, 0x10e, 0x10c, 0x3, 0x2, 
    0x2, 0x2, 0x10e, 0x10f, 0x3, 0x2, 0x2, 0x2, 0x10f, 0x111, 0x3, 0x2, 
    0x2, 0x2, 0x110, 0x10e, 0x3, 0x2, 0x2, 0x2, 0x111, 0x116, 0x8, 0xb, 
    0x1, 0x2, 0x112, 0x113, 0x7, 0x30, 0x2, 0x2, 0x113, 0x114, 0x5, 0x42, 
    0x22, 0x2, 0x114, 0x115, 0x8, 0xb, 0x1, 0x2, 0x115, 0x117, 0x3, 0x2, 
    0x2, 0x2, 0x116, 0x112, 0x3, 0x2, 0x2, 0x2, 0x116, 0x117, 0x3, 0x2, 
    0x2, 0x2, 0x117, 0x119, 0x3, 0x2, 0x2, 0x2, 0x118, 0xfb, 0x3, 0x2, 0x2, 
    0x2, 0x119, 0x11c, 0x3, 0x2, 0x2, 0x2, 0x11a, 0x118, 0x3, 0x2, 0x2, 
    0x2, 0x11a, 0x11b, 0x3, 0x2, 0x2, 0x2, 0x11b, 0x11d, 0x3, 0x2, 0x2, 
    0x2, 0x11c, 0x11a, 0x3, 0x2, 0x2, 0x2, 0x11d, 0x11e, 0x8, 0xb, 0x1, 
    0x2, 0x11e, 0x15, 0x3, 0x2, 0x2, 0x2, 0x11f, 0x120, 0x5, 0x10, 0x9, 
    0x2, 0x120, 0x121, 0x8, 0xc, 0x1, 0x2, 0x121, 0x123, 0x3, 0x2, 0x2, 
    0x2, 0x122, 0x11f, 0x3, 0x2, 0x2, 0x2, 0x123, 0x126, 0x3, 0x2, 0x2, 
    0x2, 0x124, 0x122, 0x3, 0x2, 0x2, 0x2, 0x124, 0x125, 0x3, 0x2, 0x2, 
    0x2, 0x125, 0x127, 0x3, 0x2, 0x2, 0x2, 0x126, 0x124, 0x3, 0x2, 0x2, 
    0x2, 0x127, 0x128, 0x7, 0x55, 0x2, 0x2, 0x128, 0x12d, 0x8, 0xc, 0x1, 
    0x2, 0x129, 0x12a, 0x7, 0x4b, 0x2, 0x2, 0x12a, 0x12c, 0x8, 0xc, 0x1, 
    0x2, 0x12b, 0x129, 0x3, 0x2, 0x2, 0x2, 0x12c, 0x12f, 0x3, 0x2, 0x2, 
    0x2, 0x12d, 0x12b, 0x3, 0x2, 0x2, 0x2, 0x12d, 0x12e, 0x3, 0x2, 0x2, 
    0x2, 0x12e, 0x130, 0x3, 0x2, 0x2, 0x2, 0x12f, 0x12d, 0x3, 0x2, 0x2, 
    0x2, 0x130, 0x131, 0x7, 0x55, 0x2, 0x2, 0x131, 0x13b, 0x8, 0xc, 0x1, 
    0x2, 0x132, 0x133, 0x8, 0xc, 0x1, 0x2, 0x133, 0x136, 0x7, 0x2c, 0x2, 
    0x2, 0x134, 0x135, 0x7, 0x4d, 0x2, 0x2, 0x135, 0x137, 0x8, 0xc, 0x1, 
    0x2, 0x136, 0x134, 0x3, 0x2, 0x2, 0x2, 0x136, 0x137, 0x3, 0x2, 0x2, 
    0x2, 0x137, 0x138, 0x3, 0x2, 0x2, 0x2, 0x138, 0x13a, 0x7, 0x2d, 0x2, 
    0x2, 0x139, 0x132, 0x3, 0x2, 0x2, 0x2, 0x13a, 0x13d, 0x3, 0x2, 0x2, 
    0x2, 0x13b, 0x139, 0x3, 0x2, 0x2, 0x2, 0x13b, 0x13c, 0x3, 0x2, 0x2, 
    0x2, 0x13c, 0x142, 0x3, 0x2, 0x2, 0x2, 0x13d, 0x13b, 0x3, 0x2, 0x2, 
    0x2, 0x13e, 0x13f, 0x7, 0x30, 0x2, 0x2, 0x13f, 0x140, 0x5, 0x3e, 0x20, 
    0x2, 0x140, 0x141, 0x8, 0xc, 0x1, 0x2, 0x141, 0x143, 0x3, 0x2, 0x2, 
    0x2, 0x142, 0x13e, 0x3, 0x2, 0x2, 0x2, 0x142, 0x143, 0x3, 0x2, 0x2, 
    0x2, 0x143, 0x144, 0x3, 0x2, 0x2, 0x2, 0x144, 0x145, 0x8, 0xc, 0x1, 
    0x2, 0x145, 0x17, 0x3, 0x2, 0x2, 0x2, 0x146, 0x147, 0x5, 0xe, 0x8, 0x2, 
    0x147, 0x148, 0x8, 0xd, 0x1, 0x2, 0x148, 0x14a, 0x3, 0x2, 0x2, 0x2, 
    0x149, 0x146, 0x3, 0x2, 0x2, 0x2, 0x14a, 0x14d, 0x3, 0x2, 0x2, 0x2, 
    0x14b, 0x149, 0x3, 0x2, 0x2, 0x2, 0x14b, 0x14c, 0x3, 0x2, 0x2, 0x2, 
    0x14c, 0x14e, 0x3, 0x2, 0x2, 0x2, 0x14d, 0x14b, 0x3, 0x2, 0x2, 0x2, 
    0x14e, 0x154, 0x7, 0x8, 0x2, 0x2, 0x14f, 0x150, 0x5, 0x10, 0x9, 0x2, 
    0x150, 0x151, 0x8, 0xd, 0x1, 0x2, 0x151, 0x153, 0x3, 0x2, 0x2, 0x2, 
    0x152, 0x14f, 0x3, 0x2, 0x2, 0x2, 0x153, 0x156, 0x3, 0x2, 0x2, 0x2, 
    0x154, 0x152, 0x3, 0x2, 0x2, 0x2, 0x154, 0x155, 0x3, 0x2, 0x2, 0x2, 
    0x155, 0x157, 0x3, 0x2, 0x2, 0x2, 0x156, 0x154, 0x3, 0x2, 0x2, 0x2, 
    0x157, 0x158, 0x7, 0x55, 0x2, 0x2, 0x158, 0x159, 0x8, 0xd, 0x1, 0x2, 
    0x159, 0x19, 0x3, 0x2, 0x2, 0x2, 0x15a, 0x15b, 0x5, 0x18, 0xd, 0x2, 
    0x15b, 0x15c, 0x8, 0xe, 0x1, 0x2, 0x15c, 0x163, 0x7, 0x2a, 0x2, 0x2, 
    0x15d, 0x15e, 0x5, 0x14, 0xb, 0x2, 0x15e, 0x15f, 0x8, 0xe, 0x1, 0x2, 
    0x15f, 0x160, 0x7, 0x25, 0x2, 0x2, 0x160, 0x162, 0x3, 0x2, 0x2, 0x2, 
    0x161, 0x15d, 0x3, 0x2, 0x2, 0x2, 0x162, 0x165, 0x3, 0x2, 0x2, 0x2, 
    0x163, 0x161, 0x3, 0x2, 0x2, 0x2, 0x163, 0x164, 0x3, 0x2, 0x2, 0x2, 
    0x164, 0x166, 0x3, 0x2, 0x2, 0x2, 0x165, 0x163, 0x3, 0x2, 0x2, 0x2, 
    0x166, 0x167, 0x7, 0x2b, 0x2, 0x2, 0x167, 0x168, 0x8, 0xe, 0x1, 0x2, 
    0x168, 0x1b, 0x3, 0x2, 0x2, 0x2, 0x169, 0x16a, 0x7, 0x9, 0x2, 0x2, 0x16a, 
    0x16b, 0x7, 0x55, 0x2, 0x2, 0x16b, 0x16c, 0x7, 0x2a, 0x2, 0x2, 0x16c, 
    0x16d, 0x7, 0x55, 0x2, 0x2, 0x16d, 0x172, 0x8, 0xf, 0x1, 0x2, 0x16e, 
    0x16f, 0x7, 0x30, 0x2, 0x2, 0x16f, 0x170, 0x5, 0x3e, 0x20, 0x2, 0x170, 
    0x171, 0x8, 0xf, 0x1, 0x2, 0x171, 0x173, 0x3, 0x2, 0x2, 0x2, 0x172, 
    0x16e, 0x3, 0x2, 0x2, 0x2, 0x172, 0x173, 0x3, 0x2, 0x2, 0x2, 0x173, 
    0x174, 0x3, 0x2, 0x2, 0x2, 0x174, 0x175, 0x8, 0xf, 0x1, 0x2, 0x175, 
    0x176, 0x7, 0x2b, 0x2, 0x2, 0x176, 0x177, 0x8, 0xf, 0x1, 0x2, 0x177, 
    0x1d, 0x3, 0x2, 0x2, 0x2, 0x178, 0x179, 0x5, 0x10, 0x9, 0x2, 0x179, 
    0x17a, 0x8, 0x10, 0x1, 0x2, 0x17a, 0x17c, 0x3, 0x2, 0x2, 0x2, 0x17b, 
    0x178, 0x3, 0x2, 0x2, 0x2, 0x17c, 0x17f, 0x3, 0x2, 0x2, 0x2, 0x17d, 
    0x17b, 0x3, 0x2, 0x2, 0x2, 0x17d, 0x17e, 0x3, 0x2, 0x2, 0x2, 0x17e, 
    0x180, 0x3, 0x2, 0x2, 0x2, 0x17f, 0x17d, 0x3, 0x2, 0x2, 0x2, 0x180, 
    0x181, 0x5, 0x12, 0xa, 0x2, 0x181, 0x182, 0x7, 0x55, 0x2, 0x2, 0x182, 
    0x183, 0x8, 0x10, 0x1, 0x2, 0x183, 0x18f, 0x7, 0x28, 0x2, 0x2, 0x184, 
    0x185, 0x5, 0x16, 0xc, 0x2, 0x185, 0x18c, 0x8, 0x10, 0x1, 0x2, 0x186, 
    0x187, 0x7, 0x26, 0x2, 0x2, 0x187, 0x188, 0x5, 0x16, 0xc, 0x2, 0x188, 
    0x189, 0x8, 0x10, 0x1, 0x2, 0x189, 0x18b, 0x3, 0x2, 0x2, 0x2, 0x18a, 
    0x186, 0x3, 0x2, 0x2, 0x2, 0x18b, 0x18e, 0x3, 0x2, 0x2, 0x2, 0x18c, 
    0x18a, 0x3, 0x2, 0x2, 0x2, 0x18c, 0x18d, 0x3, 0x2, 0x2, 0x2, 0x18d, 
    0x190, 0x3, 0x2, 0x2, 0x2, 0x18e, 0x18c, 0x3, 0x2, 0x2, 0x2, 0x18f, 
    0x184, 0x3, 0x2, 0x2, 0x2, 0x18f, 0x190, 0x3, 0x2, 0x2, 0x2, 0x190, 
    0x191, 0x3, 0x2, 0x2, 0x2, 0x191, 0x192, 0x7, 0x29, 0x2, 0x2, 0x192, 
    0x193, 0x8, 0x10, 0x1, 0x2, 0x193, 0x1f, 0x3, 0x2, 0x2, 0x2, 0x194, 
    0x198, 0x7, 0x2a, 0x2, 0x2, 0x195, 0x197, 0x5, 0x20, 0x11, 0x2, 0x196, 
    0x195, 0x3, 0x2, 0x2, 0x2, 0x197, 0x19a, 0x3, 0x2, 0x2, 0x2, 0x198, 
    0x196, 0x3, 0x2, 0x2, 0x2, 0x198, 0x199, 0x3, 0x2, 0x2, 0x2, 0x199, 
    0x19b, 0x3, 0x2, 0x2, 0x2, 0x19a, 0x198, 0x3, 0x2, 0x2, 0x2, 0x19b, 
    0x19e, 0x7, 0x2b, 0x2, 0x2, 0x19c, 0x19e, 0xa, 0x4, 0x2, 0x2, 0x19d, 
    0x194, 0x3, 0x2, 0x2, 0x2, 0x19d, 0x19c, 0x3, 0x2, 0x2, 0x2, 0x19e, 
    0x21, 0x3, 0x2, 0x2, 0x2, 0x19f, 0x1a0, 0x5, 0x1e, 0x10, 0x2, 0x1a0, 
    0x1a1, 0x8, 0x12, 0x1, 0x2, 0x1a1, 0x1a2, 0x8, 0x12, 0x1, 0x2, 0x1a2, 
    0x1a3, 0x5, 0x34, 0x1b, 0x2, 0x1a3, 0x1a4, 0x8, 0x12, 0x1, 0x2, 0x1a4, 
    0x23, 0x3, 0x2, 0x2, 0x2, 0x1a5, 0x1a6, 0x5, 0xe, 0x8, 0x2, 0x1a6, 0x1a7, 
    0x8, 0x13, 0x1, 0x2, 0x1a7, 0x1a9, 0x3, 0x2, 0x2, 0x2, 0x1a8, 0x1a5, 
    0x3, 0x2, 0x2, 0x2, 0x1a9, 0x1ac, 0x3, 0x2, 0x2, 0x2, 0x1aa, 0x1a8, 
    0x3, 0x2, 0x2, 0x2, 0x1aa, 0x1ab, 0x3, 0x2, 0x2, 0x2, 0x1ab, 0x1ad, 
    0x3, 0x2, 0x2, 0x2, 0x1ac, 0x1aa, 0x3, 0x2, 0x2, 0x2, 0x1ad, 0x1ae, 
    0x7, 0xa, 0x2, 0x2, 0x1ae, 0x1af, 0x7, 0x55, 0x2, 0x2, 0x1af, 0x1b0, 
    0x8, 0x13, 0x1, 0x2, 0x1b0, 0x1b7, 0x7, 0x2a, 0x2, 0x2, 0x1b1, 0x1b2, 
    0x5, 0x3e, 0x20, 0x2, 0x1b2, 0x1b3, 0x8, 0x13, 0x1, 0x2, 0x1b3, 0x1b4, 
    0x7, 0x25, 0x2, 0x2, 0x1b4, 0x1b6, 0x3, 0x2, 0x2, 0x2, 0x1b5, 0x1b1, 
    0x3, 0x2, 0x2, 0x2, 0x1b6, 0x1b9, 0x3, 0x2, 0x2, 0x2, 0x1b7, 0x1b5, 
    0x3, 0x2, 0x2, 0x2, 0x1b7, 0x1b8, 0x3, 0x2, 0x2, 0x2, 0x1b8, 0x1ba, 
    0x3, 0x2, 0x2, 0x2, 0x1b9, 0x1b7, 0x3, 0x2, 0x2, 0x2, 0x1ba, 0x1bb, 
    0x7, 0x2b, 0x2, 0x2, 0x1bb, 0x1bc, 0x8, 0x13, 0x1, 0x2, 0x1bc, 0x25, 
    0x3, 0x2, 0x2, 0x2, 0x1bd, 0x1be, 0x7, 0xb, 0x2, 0x2, 0x1be, 0x1c2, 
    0x8, 0x14, 0x1, 0x2, 0x1bf, 0x1c0, 0x7, 0xc, 0x2, 0x2, 0x1c0, 0x1c2, 
    0x8, 0x14, 0x1, 0x2, 0x1c1, 0x1bd, 0x3, 0x2, 0x2, 0x2, 0x1c1, 0x1bf, 
    0x3, 0x2, 0x2, 0x2, 0x1c2, 0x1c3, 0x3, 0x2, 0x2, 0x2, 0x1c3, 0x1c4, 
    0x7, 0x55, 0x2, 0x2, 0x1c4, 0x1c5, 0x8, 0x14, 0x1, 0x2, 0x1c5, 0x1cc, 
    0x7, 0x2a, 0x2, 0x2, 0x1c6, 0x1c7, 0x5, 0x3e, 0x20, 0x2, 0x1c7, 0x1c8, 
    0x8, 0x14, 0x1, 0x2, 0x1c8, 0x1c9, 0x7, 0x25, 0x2, 0x2, 0x1c9, 0x1cb, 
    0x3, 0x2, 0x2, 0x2, 0x1ca, 0x1c6, 0x3, 0x2, 0x2, 0x2, 0x1cb, 0x1ce, 
    0x3, 0x2, 0x2, 0x2, 0x1cc, 0x1ca, 0x3, 0x2, 0x2, 0x2, 0x1cc, 0x1cd, 
    0x3, 0x2, 0x2, 0x2, 0x1cd, 0x1cf, 0x3, 0x2, 0x2, 0x2, 0x1ce, 0x1cc, 
    0x3, 0x2, 0x2, 0x2, 0x1cf, 0x1d0, 0x7, 0x2b, 0x2, 0x2, 0x1d0, 0x1d1, 
    0x8, 0x14, 0x1, 0x2, 0x1d1, 0x27, 0x3, 0x2, 0x2, 0x2, 0x1d2, 0x1d3, 
    0x5, 0x2c, 0x17, 0x2, 0x1d3, 0x1d4, 0x8, 0x15, 0x1, 0x2, 0x1d4, 0x1ec, 
    0x3, 0x2, 0x2, 0x2, 0x1d5, 0x1d6, 0x5, 0x34, 0x1b, 0x2, 0x1d6, 0x1d7, 
    0x8, 0x15, 0x1, 0x2, 0x1d7, 0x1ec, 0x3, 0x2, 0x2, 0x2, 0x1d8, 0x1d9, 
    0x5, 0x2e, 0x18, 0x2, 0x1d9, 0x1da, 0x8, 0x15, 0x1, 0x2, 0x1da, 0x1ec, 
    0x3, 0x2, 0x2, 0x2, 0x1db, 0x1dc, 0x5, 0x32, 0x1a, 0x2, 0x1dc, 0x1dd, 
    0x8, 0x15, 0x1, 0x2, 0x1dd, 0x1ec, 0x3, 0x2, 0x2, 0x2, 0x1de, 0x1df, 
    0x5, 0x36, 0x1c, 0x2, 0x1df, 0x1e0, 0x8, 0x15, 0x1, 0x2, 0x1e0, 0x1ec, 
    0x3, 0x2, 0x2, 0x2, 0x1e1, 0x1e2, 0x5, 0x38, 0x1d, 0x2, 0x1e2, 0x1e3, 
    0x8, 0x15, 0x1, 0x2, 0x1e3, 0x1ec, 0x3, 0x2, 0x2, 0x2, 0x1e4, 0x1e5, 
    0x5, 0x3c, 0x1f, 0x2, 0x1e5, 0x1e6, 0x8, 0x15, 0x1, 0x2, 0x1e6, 0x1ec, 
    0x3, 0x2, 0x2, 0x2, 0x1e7, 0x1e8, 0x5, 0x2a, 0x16, 0x2, 0x1e8, 0x1e9, 
    0x7, 0x25, 0x2, 0x2, 0x1e9, 0x1ea, 0x8, 0x15, 0x1, 0x2, 0x1ea, 0x1ec, 
    0x3, 0x2, 0x2, 0x2, 0x1eb, 0x1d2, 0x3, 0x2, 0x2, 0x2, 0x1eb, 0x1d5, 
    0x3, 0x2, 0x2, 0x2, 0x1eb, 0x1d8, 0x3, 0x2, 0x2, 0x2, 0x1eb, 0x1db, 
    0x3, 0x2, 0x2, 0x2, 0x1eb, 0x1de, 0x3, 0x2, 0x2, 0x2, 0x1eb, 0x1e1, 
    0x3, 0x2, 0x2, 0x2, 0x1eb, 0x1e4, 0x3, 0x2, 0x2, 0x2, 0x1eb, 0x1e7, 
    0x3, 0x2, 0x2, 0x2, 0x1ec, 0x29, 0x3, 0x2, 0x2, 0x2, 0x1ed, 0x1ee, 0x5, 
    0x3e, 0x20, 0x2, 0x1ee, 0x1ef, 0x8, 0x16, 0x1, 0x2, 0x1ef, 0x2b, 0x3, 
    0x2, 0x2, 0x2, 0x1f0, 0x1f1, 0x7, 0xd, 0x2, 0x2, 0x1f1, 0x1f2, 0x8, 
    0x17, 0x1, 0x2, 0x1f2, 0x1f3, 0x7, 0x28, 0x2, 0x2, 0x1f3, 0x1f4, 0x5, 
    0x3e, 0x20, 0x2, 0x1f4, 0x1f5, 0x8, 0x17, 0x1, 0x2, 0x1f5, 0x1f6, 0x7, 
    0x29, 0x2, 0x2, 0x1f6, 0x1f7, 0x5, 0x28, 0x15, 0x2, 0x1f7, 0x1fc, 0x8, 
    0x17, 0x1, 0x2, 0x1f8, 0x1f9, 0x7, 0xe, 0x2, 0x2, 0x1f9, 0x1fa, 0x5, 
    0x28, 0x15, 0x2, 0x1fa, 0x1fb, 0x8, 0x17, 0x1, 0x2, 0x1fb, 0x1fd, 0x3, 
    0x2, 0x2, 0x2, 0x1fc, 0x1f8, 0x3, 0x2, 0x2, 0x2, 0x1fc, 0x1fd, 0x3, 
    0x2, 0x2, 0x2, 0x1fd, 0x1fe, 0x3, 0x2, 0x2, 0x2, 0x1fe, 0x1ff, 0x8, 
    0x17, 0x1, 0x2, 0x1ff, 0x2d, 0x3, 0x2, 0x2, 0x2, 0x200, 0x201, 0x7, 
    0xf, 0x2, 0x2, 0x201, 0x202, 0x8, 0x18, 0x1, 0x2, 0x202, 0x206, 0x7, 
    0x28, 0x2, 0x2, 0x203, 0x204, 0x5, 0x14, 0xb, 0x2, 0x204, 0x205, 0x8, 
    0x18, 0x1, 0x2, 0x205, 0x207, 0x3, 0x2, 0x2, 0x2, 0x206, 0x203, 0x3, 
    0x2, 0x2, 0x2, 0x206, 0x207, 0x3, 0x2, 0x2, 0x2, 0x207, 0x208, 0x3, 
    0x2, 0x2, 0x2, 0x208, 0x20c, 0x7, 0x25, 0x2, 0x2, 0x209, 0x20a, 0x5, 
    0x3e, 0x20, 0x2, 0x20a, 0x20b, 0x8, 0x18, 0x1, 0x2, 0x20b, 0x20d, 0x3, 
    0x2, 0x2, 0x2, 0x20c, 0x209, 0x3, 0x2, 0x2, 0x2, 0x20c, 0x20d, 0x3, 
    0x2, 0x2, 0x2, 0x20d, 0x20e, 0x3, 0x2, 0x2, 0x2, 0x20e, 0x212, 0x7, 
    0x25, 0x2, 0x2, 0x20f, 0x210, 0x5, 0x3e, 0x20, 0x2, 0x210, 0x211, 0x8, 
    0x18, 0x1, 0x2, 0x211, 0x213, 0x3, 0x2, 0x2, 0x2, 0x212, 0x20f, 0x3, 
    0x2, 0x2, 0x2, 0x212, 0x213, 0x3, 0x2, 0x2, 0x2, 0x213, 0x214, 0x3, 
    0x2, 0x2, 0x2, 0x214, 0x215, 0x7, 0x29, 0x2, 0x2, 0x215, 0x216, 0x5, 
    0x28, 0x15, 0x2, 0x216, 0x217, 0x8, 0x18, 0x1, 0x2, 0x217, 0x218, 0x8, 
    0x18, 0x1, 0x2, 0x218, 0x2f, 0x3, 0x2, 0x2, 0x2, 0x219, 0x21a, 0x7, 
    0xf, 0x2, 0x2, 0x21a, 0x21b, 0x8, 0x19, 0x1, 0x2, 0x21b, 0x21c, 0x7, 
    0x28, 0x2, 0x2, 0x21c, 0x21d, 0x7, 0x55, 0x2, 0x2, 0x21d, 0x21e, 0x7, 
    0x27, 0x2, 0x2, 0x21e, 0x21f, 0x7, 0x55, 0x2, 0x2, 0x21f, 0x220, 0x7, 
    0x10, 0x2, 0x2, 0x220, 0x221, 0x7, 0x55, 0x2, 0x2, 0x221, 0x222, 0x7, 
    0x29, 0x2, 0x2, 0x222, 0x223, 0x5, 0x28, 0x15, 0x2, 0x223, 0x224, 0x8, 
    0x19, 0x1, 0x2, 0x224, 0x225, 0x8, 0x19, 0x1, 0x2, 0x225, 0x31, 0x3, 
    0x2, 0x2, 0x2, 0x226, 0x227, 0x7, 0x11, 0x2, 0x2, 0x227, 0x228, 0x8, 
    0x1a, 0x1, 0x2, 0x228, 0x229, 0x7, 0x28, 0x2, 0x2, 0x229, 0x22a, 0x5, 
    0x3e, 0x20, 0x2, 0x22a, 0x22b, 0x8, 0x1a, 0x1, 0x2, 0x22b, 0x22c, 0x7, 
    0x29, 0x2, 0x2, 0x22c, 0x22d, 0x5, 0x28, 0x15, 0x2, 0x22d, 0x22e, 0x8, 
    0x1a, 0x1, 0x2, 0x22e, 0x22f, 0x8, 0x1a, 0x1, 0x2, 0x22f, 0x23d, 0x3, 
    0x2, 0x2, 0x2, 0x230, 0x231, 0x7, 0x12, 0x2, 0x2, 0x231, 0x232, 0x8, 
    0x1a, 0x1, 0x2, 0x232, 0x233, 0x5, 0x28, 0x15, 0x2, 0x233, 0x234, 0x8, 
    0x1a, 0x1, 0x2, 0x234, 0x235, 0x7, 0x11, 0x2, 0x2, 0x235, 0x236, 0x7, 
    0x28, 0x2, 0x2, 0x236, 0x237, 0x5, 0x3e, 0x20, 0x2, 0x237, 0x238, 0x8, 
    0x1a, 0x1, 0x2, 0x238, 0x239, 0x7, 0x29, 0x2, 0x2, 0x239, 0x23a, 0x7, 
    0x25, 0x2, 0x2, 0x23a, 0x23b, 0x8, 0x1a, 0x1, 0x2, 0x23b, 0x23d, 0x3, 
    0x2, 0x2, 0x2, 0x23c, 0x226, 0x3, 0x2, 0x2, 0x2, 0x23c, 0x230, 0x3, 
    0x2, 0x2, 0x2, 0x23d, 0x33, 0x3, 0x2, 0x2, 0x2, 0x23e, 0x23f, 0x7, 0x2a, 
    0x2, 0x2, 0x23f, 0x249, 0x8, 0x1b, 0x1, 0x2, 0x240, 0x241, 0x5, 0x28, 
    0x15, 0x2, 0x241, 0x242, 0x8, 0x1b, 0x1, 0x2, 0x242, 0x248, 0x3, 0x2, 
    0x2, 0x2, 0x243, 0x244, 0x5, 0x14, 0xb, 0x2, 0x244, 0x245, 0x7, 0x25, 
    0x2, 0x2, 0x245, 0x246, 0x8, 0x1b, 0x1, 0x2, 0x246, 0x248, 0x3, 0x2, 
    0x2, 0x2, 0x247, 0x240, 0x3, 0x2, 0x2, 0x2, 0x247, 0x243, 0x3, 0x2, 
    0x2, 0x2, 0x248, 0x24b, 0x3, 0x2, 0x2, 0x2, 0x249, 0x247, 0x3, 0x2, 
    0x2, 0x2, 0x249, 0x24a, 0x3, 0x2, 0x2, 0x2, 0x24a, 0x24c, 0x3, 0x2, 
    0x2, 0x2, 0x24b, 0x249, 0x3, 0x2, 0x2, 0x2, 0x24c, 0x24d, 0x7, 0x2b, 
    0x2, 0x2, 0x24d, 0x24e, 0x8, 0x1b, 0x1, 0x2, 0x24e, 0x35, 0x3, 0x2, 
    0x2, 0x2, 0x24f, 0x250, 0x7, 0x13, 0x2, 0x2, 0x250, 0x254, 0x8, 0x1c, 
    0x1, 0x2, 0x251, 0x252, 0x5, 0x3e, 0x20, 0x2, 0x252, 0x253, 0x8, 0x1c, 
    0x1, 0x2, 0x253, 0x255, 0x3, 0x2, 0x2, 0x2, 0x254, 0x251, 0x3, 0x2, 
    0x2, 0x2, 0x254, 0x255, 0x3, 0x2, 0x2, 0x2, 0x255, 0x256, 0x3, 0x2, 
    0x2, 0x2, 0x256, 0x257, 0x7, 0x25, 0x2, 0x2, 0x257, 0x258, 0x8, 0x1c, 
    0x1, 0x2, 0x258, 0x37, 0x3, 0x2, 0x2, 0x2, 0x259, 0x25a, 0x7, 0x14, 
    0x2, 0x2, 0x25a, 0x25b, 0x8, 0x1d, 0x1, 0x2, 0x25b, 0x25c, 0x7, 0x25, 
    0x2, 0x2, 0x25c, 0x25d, 0x8, 0x1d, 0x1, 0x2, 0x25d, 0x39, 0x3, 0x2, 
    0x2, 0x2, 0x25e, 0x25f, 0x7, 0x15, 0x2, 0x2, 0x25f, 0x260, 0x8, 0x1e, 
    0x1, 0x2, 0x260, 0x261, 0x7, 0x28, 0x2, 0x2, 0x261, 0x262, 0x5, 0x3e, 
    0x20, 0x2, 0x262, 0x263, 0x7, 0x29, 0x2, 0x2, 0x263, 0x264, 0x8, 0x1e, 
    0x1, 0x2, 0x264, 0x26d, 0x7, 0x2a, 0x2, 0x2, 0x265, 0x266, 0x7, 0x16, 
    0x2, 0x2, 0x266, 0x267, 0x7, 0x55, 0x2, 0x2, 0x267, 0x268, 0x7, 0x27, 
    0x2, 0x2, 0x268, 0x269, 0x5, 0x28, 0x15, 0x2, 0x269, 0x26a, 0x8, 0x1e, 
    0x1, 0x2, 0x26a, 0x26c, 0x3, 0x2, 0x2, 0x2, 0x26b, 0x265, 0x3, 0x2, 
    0x2, 0x2, 0x26c, 0x26f, 0x3, 0x2, 0x2, 0x2, 0x26d, 0x26b, 0x3, 0x2, 
    0x2, 0x2, 0x26d, 0x26e, 0x3, 0x2, 0x2, 0x2, 0x26e, 0x275, 0x3, 0x2, 
    0x2, 0x2, 0x26f, 0x26d, 0x3, 0x2, 0x2, 0x2, 0x270, 0x271, 0x7, 0x17, 
    0x2, 0x2, 0x271, 0x272, 0x7, 0x27, 0x2, 0x2, 0x272, 0x273, 0x5, 0x28, 
    0x15, 0x2, 0x273, 0x274, 0x8, 0x1e, 0x1, 0x2, 0x274, 0x276, 0x3, 0x2, 
    0x2, 0x2, 0x275, 0x270, 0x3, 0x2, 0x2, 0x2, 0x275, 0x276, 0x3, 0x2, 
    0x2, 0x2, 0x276, 0x277, 0x3, 0x2, 0x2, 0x2, 0x277, 0x278, 0x7, 0x2b, 
    0x2, 0x2, 0x278, 0x279, 0x8, 0x1e, 0x1, 0x2, 0x279, 0x3b, 0x3, 0x2, 
    0x2, 0x2, 0x27a, 0x27b, 0x7, 0x18, 0x2, 0x2, 0x27b, 0x27c, 0x7, 0x25, 
    0x2, 0x2, 0x27c, 0x27d, 0x8, 0x1f, 0x1, 0x2, 0x27d, 0x3d, 0x3, 0x2, 
    0x2, 0x2, 0x27e, 0x27f, 0x5, 0x40, 0x21, 0x2, 0x27f, 0x280, 0x8, 0x20, 
    0x1, 0x2, 0x280, 0x3f, 0x3, 0x2, 0x2, 0x2, 0x281, 0x282, 0x5, 0x42, 
    0x22, 0x2, 0x282, 0x289, 0x8, 0x21, 0x1, 0x2, 0x283, 0x284, 0x7, 0x26, 
    0x2, 0x2, 0x284, 0x285, 0x5, 0x42, 0x22, 0x2, 0x285, 0x286, 0x8, 0x21, 
    0x1, 0x2, 0x286, 0x288, 0x3, 0x2, 0x2, 0x2, 0x287, 0x283, 0x3, 0x2, 
    0x2, 0x2, 0x288, 0x28b, 0x3, 0x2, 0x2, 0x2, 0x289, 0x287, 0x3, 0x2, 
    0x2, 0x2, 0x289, 0x28a, 0x3, 0x2, 0x2, 0x2, 0x28a, 0x41, 0x3, 0x2, 0x2, 
    0x2, 0x28b, 0x289, 0x3, 0x2, 0x2, 0x2, 0x28c, 0x28d, 0x5, 0x44, 0x23, 
    0x2, 0x28d, 0x29a, 0x8, 0x22, 0x1, 0x2, 0x28e, 0x28f, 0x9, 0x5, 0x2, 
    0x2, 0x28f, 0x290, 0x5, 0x44, 0x23, 0x2, 0x290, 0x291, 0x8, 0x22, 0x1, 
    0x2, 0x291, 0x299, 0x3, 0x2, 0x2, 0x2, 0x292, 0x293, 0x7, 0x32, 0x2, 
    0x2, 0x293, 0x294, 0x5, 0x3e, 0x20, 0x2, 0x294, 0x295, 0x7, 0x27, 0x2, 
    0x2, 0x295, 0x296, 0x5, 0x3e, 0x20, 0x2, 0x296, 0x297, 0x8, 0x22, 0x1, 
    0x2, 0x297, 0x299, 0x3, 0x2, 0x2, 0x2, 0x298, 0x28e, 0x3, 0x2, 0x2, 
    0x2, 0x298, 0x292, 0x3, 0x2, 0x2, 0x2, 0x299, 0x29c, 0x3, 0x2, 0x2, 
    0x2, 0x29a, 0x298, 0x3, 0x2, 0x2, 0x2, 0x29a, 0x29b, 0x3, 0x2, 0x2, 
    0x2, 0x29b, 0x43, 0x3, 0x2, 0x2, 0x2, 0x29c, 0x29a, 0x3, 0x2, 0x2, 0x2, 
    0x29d, 0x29e, 0x5, 0x46, 0x24, 0x2, 0x29e, 0x2a5, 0x8, 0x23, 0x1, 0x2, 
    0x29f, 0x2a0, 0x7, 0x44, 0x2, 0x2, 0x2a0, 0x2a1, 0x5, 0x46, 0x24, 0x2, 
    0x2a1, 0x2a2, 0x8, 0x23, 0x1, 0x2, 0x2a2, 0x2a4, 0x3, 0x2, 0x2, 0x2, 
    0x2a3, 0x29f, 0x3, 0x2, 0x2, 0x2, 0x2a4, 0x2a7, 0x3, 0x2, 0x2, 0x2, 
    0x2a5, 0x2a3, 0x3, 0x2, 0x2, 0x2, 0x2a5, 0x2a6, 0x3, 0x2, 0x2, 0x2, 
    0x2a6, 0x45, 0x3, 0x2, 0x2, 0x2, 0x2a7, 0x2a5, 0x3, 0x2, 0x2, 0x2, 0x2a8, 
    0x2a9, 0x5, 0x48, 0x25, 0x2, 0x2a9, 0x2b0, 0x8, 0x24, 0x1, 0x2, 0x2aa, 
    0x2ab, 0x7, 0x43, 0x2, 0x2, 0x2ab, 0x2ac, 0x5, 0x48, 0x25, 0x2, 0x2ac, 
    0x2ad, 0x8, 0x24, 0x1, 0x2, 0x2ad, 0x2af, 0x3, 0x2, 0x2, 0x2, 0x2ae, 
    0x2aa, 0x3, 0x2, 0x2, 0x2, 0x2af, 0x2b2, 0x3, 0x2, 0x2, 0x2, 0x2b0, 
    0x2ae, 0x3, 0x2, 0x2, 0x2, 0x2b0, 0x2b1, 0x3, 0x2, 0x2, 0x2, 0x2b1, 
    0x47, 0x3, 0x2, 0x2, 0x2, 0x2b2, 0x2b0, 0x3, 0x2, 0x2, 0x2, 0x2b3, 0x2b4, 
    0x5, 0x4a, 0x26, 0x2, 0x2b4, 0x2bb, 0x8, 0x25, 0x1, 0x2, 0x2b5, 0x2b6, 
    0x7, 0x35, 0x2, 0x2, 0x2b6, 0x2b7, 0x5, 0x4a, 0x26, 0x2, 0x2b7, 0x2b8, 
    0x8, 0x25, 0x1, 0x2, 0x2b8, 0x2ba, 0x3, 0x2, 0x2, 0x2, 0x2b9, 0x2b5, 
    0x3, 0x2, 0x2, 0x2, 0x2ba, 0x2bd, 0x3, 0x2, 0x2, 0x2, 0x2bb, 0x2b9, 
    0x3, 0x2, 0x2, 0x2, 0x2bb, 0x2bc, 0x3, 0x2, 0x2, 0x2, 0x2bc, 0x49, 0x3, 
    0x2, 0x2, 0x2, 0x2bd, 0x2bb, 0x3, 0x2, 0x2, 0x2, 0x2be, 0x2bf, 0x5, 
    0x4c, 0x27, 0x2, 0x2bf, 0x2c6, 0x8, 0x26, 0x1, 0x2, 0x2c0, 0x2c1, 0x7, 
    0x37, 0x2, 0x2, 0x2c1, 0x2c2, 0x5, 0x4c, 0x27, 0x2, 0x2c2, 0x2c3, 0x8, 
    0x26, 0x1, 0x2, 0x2c3, 0x2c5, 0x3, 0x2, 0x2, 0x2, 0x2c4, 0x2c0, 0x3, 
    0x2, 0x2, 0x2, 0x2c5, 0x2c8, 0x3, 0x2, 0x2, 0x2, 0x2c6, 0x2c4, 0x3, 
    0x2, 0x2, 0x2, 0x2c6, 0x2c7, 0x3, 0x2, 0x2, 0x2, 0x2c7, 0x4b, 0x3, 0x2, 
    0x2, 0x2, 0x2c8, 0x2c6, 0x3, 0x2, 0x2, 0x2, 0x2c9, 0x2ca, 0x5, 0x4e, 
    0x28, 0x2, 0x2ca, 0x2d1, 0x8, 0x27, 0x1, 0x2, 0x2cb, 0x2cc, 0x7, 0x33, 
    0x2, 0x2, 0x2cc, 0x2cd, 0x5, 0x4e, 0x28, 0x2, 0x2cd, 0x2ce, 0x8, 0x27, 
    0x1, 0x2, 0x2ce, 0x2d0, 0x3, 0x2, 0x2, 0x2, 0x2cf, 0x2cb, 0x3, 0x2, 
    0x2, 0x2, 0x2d0, 0x2d3, 0x3, 0x2, 0x2, 0x2, 0x2d1, 0x2cf, 0x3, 0x2, 
    0x2, 0x2, 0x2d1, 0x2d2, 0x3, 0x2, 0x2, 0x2, 0x2d2, 0x4d, 0x3, 0x2, 0x2, 
    0x2, 0x2d3, 0x2d1, 0x3, 0x2, 0x2, 0x2, 0x2d4, 0x2d5, 0x5, 0x50, 0x29, 
    0x2, 0x2d5, 0x2dc, 0x8, 0x28, 0x1, 0x2, 0x2d6, 0x2d7, 0x9, 0x6, 0x2, 
    0x2, 0x2d7, 0x2d8, 0x5, 0x50, 0x29, 0x2, 0x2d8, 0x2d9, 0x8, 0x28, 0x1, 
    0x2, 0x2d9, 0x2db, 0x3, 0x2, 0x2, 0x2, 0x2da, 0x2d6, 0x3, 0x2, 0x2, 
    0x2, 0x2db, 0x2de, 0x3, 0x2, 0x2, 0x2, 0x2dc, 0x2da, 0x3, 0x2, 0x2, 
    0x2, 0x2dc, 0x2dd, 0x3, 0x2, 0x2, 0x2, 0x2dd, 0x4f, 0x3, 0x2, 0x2, 0x2, 
    0x2de, 0x2dc, 0x3, 0x2, 0x2, 0x2, 0x2df, 0x2e0, 0x5, 0x52, 0x2a, 0x2, 
    0x2e0, 0x2e7, 0x8, 0x29, 0x1, 0x2, 0x2e1, 0x2e2, 0x9, 0x7, 0x2, 0x2, 
    0x2e2, 0x2e3, 0x5, 0x52, 0x2a, 0x2, 0x2e3, 0x2e4, 0x8, 0x29, 0x1, 0x2, 
    0x2e4, 0x2e6, 0x3, 0x2, 0x2, 0x2, 0x2e5, 0x2e1, 0x3, 0x2, 0x2, 0x2, 
    0x2e6, 0x2e9, 0x3, 0x2, 0x2, 0x2, 0x2e7, 0x2e5, 0x3, 0x2, 0x2, 0x2, 
    0x2e7, 0x2e8, 0x3, 0x2, 0x2, 0x2, 0x2e8, 0x51, 0x3, 0x2, 0x2, 0x2, 0x2e9, 
    0x2e7, 0x3, 0x2, 0x2, 0x2, 0x2ea, 0x2eb, 0x5, 0x54, 0x2b, 0x2, 0x2eb, 
    0x2f2, 0x8, 0x2a, 0x1, 0x2, 0x2ec, 0x2ed, 0x9, 0x8, 0x2, 0x2, 0x2ed, 
    0x2ee, 0x5, 0x54, 0x2b, 0x2, 0x2ee, 0x2ef, 0x8, 0x2a, 0x1, 0x2, 0x2ef, 
    0x2f1, 0x3, 0x2, 0x2, 0x2, 0x2f0, 0x2ec, 0x3, 0x2, 0x2, 0x2, 0x2f1, 
    0x2f4, 0x3, 0x2, 0x2, 0x2, 0x2f2, 0x2f0, 0x3, 0x2, 0x2, 0x2, 0x2f2, 
    0x2f3, 0x3, 0x2, 0x2, 0x2, 0x2f3, 0x53, 0x3, 0x2, 0x2, 0x2, 0x2f4, 0x2f2, 
    0x3, 0x2, 0x2, 0x2, 0x2f5, 0x2f6, 0x5, 0x56, 0x2c, 0x2, 0x2f6, 0x2fd, 
    0x8, 0x2b, 0x1, 0x2, 0x2f7, 0x2f8, 0x9, 0x9, 0x2, 0x2, 0x2f8, 0x2f9, 
    0x5, 0x56, 0x2c, 0x2, 0x2f9, 0x2fa, 0x8, 0x2b, 0x1, 0x2, 0x2fa, 0x2fc, 
    0x3, 0x2, 0x2, 0x2, 0x2fb, 0x2f7, 0x3, 0x2, 0x2, 0x2, 0x2fc, 0x2ff, 
    0x3, 0x2, 0x2, 0x2, 0x2fd, 0x2fb, 0x3, 0x2, 0x2, 0x2, 0x2fd, 0x2fe, 
    0x3, 0x2, 0x2, 0x2, 0x2fe, 0x55, 0x3, 0x2, 0x2, 0x2, 0x2ff, 0x2fd, 0x3, 
    0x2, 0x2, 0x2, 0x300, 0x301, 0x5, 0x58, 0x2d, 0x2, 0x301, 0x308, 0x8, 
    0x2c, 0x1, 0x2, 0x302, 0x303, 0x9, 0xa, 0x2, 0x2, 0x303, 0x304, 0x5, 
    0x58, 0x2d, 0x2, 0x304, 0x305, 0x8, 0x2c, 0x1, 0x2, 0x305, 0x307, 0x3, 
    0x2, 0x2, 0x2, 0x306, 0x302, 0x3, 0x2, 0x2, 0x2, 0x307, 0x30a, 0x3, 
    0x2, 0x2, 0x2, 0x308, 0x306, 0x3, 0x2, 0x2, 0x2, 0x308, 0x309, 0x3, 
    0x2, 0x2, 0x2, 0x309, 0x57, 0x3, 0x2, 0x2, 0x2, 0x30a, 0x308, 0x3, 0x2, 
    0x2, 0x2, 0x30b, 0x30c, 0x9, 0xb, 0x2, 0x2, 0x30c, 0x30e, 0x8, 0x2d, 
    0x1, 0x2, 0x30d, 0x30b, 0x3, 0x2, 0x2, 0x2, 0x30e, 0x311, 0x3, 0x2, 
    0x2, 0x2, 0x30f, 0x30d, 0x3, 0x2, 0x2, 0x2, 0x30f, 0x310, 0x3, 0x2, 
    0x2, 0x2, 0x310, 0x312, 0x3, 0x2, 0x2, 0x2, 0x311, 0x30f, 0x3, 0x2, 
    0x2, 0x2, 0x312, 0x313, 0x5, 0x5a, 0x2e, 0x2, 0x313, 0x314, 0x8, 0x2d, 
    0x1, 0x2, 0x314, 0x59, 0x3, 0x2, 0x2, 0x2, 0x315, 0x31a, 0x5, 0x5e, 
    0x30, 0x2, 0x316, 0x317, 0x9, 0xc, 0x2, 0x2, 0x317, 0x319, 0x8, 0x2e, 
    0x1, 0x2, 0x318, 0x316, 0x3, 0x2, 0x2, 0x2, 0x319, 0x31c, 0x3, 0x2, 
    0x2, 0x2, 0x31a, 0x318, 0x3, 0x2, 0x2, 0x2, 0x31a, 0x31b, 0x3, 0x2, 
    0x2, 0x2, 0x31b, 0x31d, 0x3, 0x2, 0x2, 0x2, 0x31c, 0x31a, 0x3, 0x2, 
    0x2, 0x2, 0x31d, 0x31e, 0x8, 0x2e, 0x1, 0x2, 0x31e, 0x34b, 0x3, 0x2, 
    0x2, 0x2, 0x31f, 0x320, 0x5, 0x5e, 0x30, 0x2, 0x320, 0x347, 0x8, 0x2e, 
    0x1, 0x2, 0x321, 0x322, 0x7, 0x28, 0x2, 0x2, 0x322, 0x32e, 0x8, 0x2e, 
    0x1, 0x2, 0x323, 0x324, 0x5, 0x44, 0x23, 0x2, 0x324, 0x32b, 0x8, 0x2e, 
    0x1, 0x2, 0x325, 0x326, 0x7, 0x26, 0x2, 0x2, 0x326, 0x327, 0x5, 0x44, 
    0x23, 0x2, 0x327, 0x328, 0x8, 0x2e, 0x1, 0x2, 0x328, 0x32a, 0x3, 0x2, 
    0x2, 0x2, 0x329, 0x325, 0x3, 0x2, 0x2, 0x2, 0x32a, 0x32d, 0x3, 0x2, 
    0x2, 0x2, 0x32b, 0x329, 0x3, 0x2, 0x2, 0x2, 0x32b, 0x32c, 0x3, 0x2, 
    0x2, 0x2, 0x32c, 0x32f, 0x3, 0x2, 0x2, 0x2, 0x32d, 0x32b, 0x3, 0x2, 
    0x2, 0x2, 0x32e, 0x323, 0x3, 0x2, 0x2, 0x2, 0x32e, 0x32f, 0x3, 0x2, 
    0x2, 0x2, 0x32f, 0x330, 0x3, 0x2, 0x2, 0x2, 0x330, 0x331, 0x7, 0x29, 
    0x2, 0x2, 0x331, 0x346, 0x8, 0x2e, 0x1, 0x2, 0x332, 0x333, 0x7, 0x2e, 
    0x2, 0x2, 0x333, 0x334, 0x8, 0x2e, 0x1, 0x2, 0x334, 0x335, 0x5, 0x5a, 
    0x2e, 0x2, 0x335, 0x336, 0x8, 0x2e, 0x1, 0x2, 0x336, 0x346, 0x3, 0x2, 
    0x2, 0x2, 0x337, 0x338, 0x7, 0x4c, 0x2, 0x2, 0x338, 0x339, 0x8, 0x2e, 
    0x1, 0x2, 0x339, 0x33a, 0x5, 0x5a, 0x2e, 0x2, 0x33a, 0x33b, 0x8, 0x2e, 
    0x1, 0x2, 0x33b, 0x346, 0x3, 0x2, 0x2, 0x2, 0x33c, 0x33d, 0x7, 0x2c, 
    0x2, 0x2, 0x33d, 0x341, 0x8, 0x2e, 0x1, 0x2, 0x33e, 0x33f, 0x5, 0x3e, 
    0x20, 0x2, 0x33f, 0x340, 0x8, 0x2e, 0x1, 0x2, 0x340, 0x342, 0x3, 0x2, 
    0x2, 0x2, 0x341, 0x33e, 0x3, 0x2, 0x2, 0x2, 0x341, 0x342, 0x3, 0x2, 
    0x2, 0x2, 0x342, 0x343, 0x3, 0x2, 0x2, 0x2, 0x343, 0x344, 0x7, 0x2d, 
    0x2, 0x2, 0x344, 0x346, 0x8, 0x2e, 0x1, 0x2, 0x345, 0x321, 0x3, 0x2, 
    0x2, 0x2, 0x345, 0x332, 0x3, 0x2, 0x2, 0x2, 0x345, 0x337, 0x3, 0x2, 
    0x2, 0x2, 0x345, 0x33c, 0x3, 0x2, 0x2, 0x2, 0x346, 0x349, 0x3, 0x2, 
    0x2, 0x2, 0x347, 0x345, 0x3, 0x2, 0x2, 0x2, 0x347, 0x348, 0x3, 0x2, 
    0x2, 0x2, 0x348, 0x34b, 0x3, 0x2, 0x2, 0x2, 0x349, 0x347, 0x3, 0x2, 
    0x2, 0x2, 0x34a, 0x315, 0x3, 0x2, 0x2, 0x2, 0x34a, 0x31f, 0x3, 0x2, 
    0x2, 0x2, 0x34b, 0x5b, 0x3, 0x2, 0x2, 0x2, 0x34c, 0x34d, 0x5, 0x5e, 
    0x30, 0x2, 0x34d, 0x34e, 0x7, 0x24, 0x2, 0x2, 0x34e, 0x34f, 0x5, 0x5e, 
    0x30, 0x2, 0x34f, 0x350, 0x8, 0x2f, 0x1, 0x2, 0x350, 0x5d, 0x3, 0x2, 
    0x2, 0x2, 0x351, 0x352, 0x7, 0x4d, 0x2, 0x2, 0x352, 0x36c, 0x8, 0x30, 
    0x1, 0x2, 0x353, 0x354, 0x7, 0x4e, 0x2, 0x2, 0x354, 0x36c, 0x8, 0x30, 
    0x1, 0x2, 0x355, 0x356, 0x7, 0x51, 0x2, 0x2, 0x356, 0x36c, 0x8, 0x30, 
    0x1, 0x2, 0x357, 0x358, 0x7, 0x53, 0x2, 0x2, 0x358, 0x36c, 0x8, 0x30, 
    0x1, 0x2, 0x359, 0x35a, 0x7, 0x54, 0x2, 0x2, 0x35a, 0x36c, 0x8, 0x30, 
    0x1, 0x2, 0x35b, 0x35c, 0x5, 0x2, 0x2, 0x2, 0x35c, 0x35d, 0x8, 0x30, 
    0x1, 0x2, 0x35d, 0x36c, 0x3, 0x2, 0x2, 0x2, 0x35e, 0x35f, 0x7, 0x55, 
    0x2, 0x2, 0x35f, 0x36c, 0x8, 0x30, 0x1, 0x2, 0x360, 0x361, 0x5, 0x4, 
    0x3, 0x2, 0x361, 0x362, 0x8, 0x30, 0x1, 0x2, 0x362, 0x36c, 0x3, 0x2, 
    0x2, 0x2, 0x363, 0x364, 0x5, 0x60, 0x31, 0x2, 0x364, 0x365, 0x8, 0x30, 
    0x1, 0x2, 0x365, 0x36c, 0x3, 0x2, 0x2, 0x2, 0x366, 0x367, 0x7, 0x28, 
    0x2, 0x2, 0x367, 0x368, 0x5, 0x3e, 0x20, 0x2, 0x368, 0x369, 0x7, 0x29, 
    0x2, 0x2, 0x369, 0x36a, 0x8, 0x30, 0x1, 0x2, 0x36a, 0x36c, 0x3, 0x2, 
    0x2, 0x2, 0x36b, 0x351, 0x3, 0x2, 0x2, 0x2, 0x36b, 0x353, 0x3, 0x2, 
    0x2, 0x2, 0x36b, 0x355, 0x3, 0x2, 0x2, 0x2, 0x36b, 0x357, 0x3, 0x2, 
    0x2, 0x2, 0x36b, 0x359, 0x3, 0x2, 0x2, 0x2, 0x36b, 0x35b, 0x3, 0x2, 
    0x2, 0x2, 0x36b, 0x35e, 0x3, 0x2, 0x2, 0x2, 0x36b, 0x360, 0x3, 0x2, 
    0x2, 0x2, 0x36b, 0x363, 0x3, 0x2, 0x2, 0x2, 0x36b, 0x366, 0x3, 0x2, 
    0x2, 0x2, 0x36c, 0x5f, 0x3, 0x2, 0x2, 0x2, 0x36d, 0x379, 0x7, 0x2a, 
    0x2, 0x2, 0x36e, 0x36f, 0x5, 0x42, 0x22, 0x2, 0x36f, 0x376, 0x8, 0x31, 
    0x1, 0x2, 0x370, 0x371, 0x7, 0x26, 0x2, 0x2, 0x371, 0x372, 0x5, 0x42, 
    0x22, 0x2, 0x372, 0x373, 0x8, 0x31, 0x1, 0x2, 0x373, 0x375, 0x3, 0x2, 
    0x2, 0x2, 0x374, 0x370, 0x3, 0x2, 0x2, 0x2, 0x375, 0x378, 0x3, 0x2, 
    0x2, 0x2, 0x376, 0x374, 0x3, 0x2, 0x2, 0x2, 0x376, 0x377, 0x3, 0x2, 
    0x2, 0x2, 0x377, 0x37a, 0x3, 0x2, 0x2, 0x2, 0x378, 0x376, 0x3, 0x2, 
    0x2, 0x2, 0x379, 0x36e, 0x3, 0x2, 0x2, 0x2, 0x379, 0x37a, 0x3, 0x2, 
    0x2, 0x2, 0x37a, 0x37b, 0x3, 0x2, 0x2, 0x2, 0x37b, 0x37c, 0x7, 0x2b, 
    0x2, 0x2, 0x37c, 0x37d, 0x8, 0x31, 0x1, 0x2, 0x37d, 0x61, 0x3, 0x2, 
    0x2, 0x2, 0x4b, 0x67, 0x70, 0x74, 0x7a, 0x83, 0x85, 0xad, 0xaf, 0xc8, 
    0xd2, 0xda, 0xe3, 0xec, 0xf1, 0xf9, 0x100, 0x109, 0x10e, 0x116, 0x11a, 
    0x124, 0x12d, 0x136, 0x13b, 0x142, 0x14b, 0x154, 0x163, 0x172, 0x17d, 
    0x18c, 0x18f, 0x198, 0x19d, 0x1aa, 0x1b7, 0x1c1, 0x1cc, 0x1eb, 0x1fc, 
    0x206, 0x20c, 0x212, 0x23c, 0x247, 0x249, 0x254, 0x26d, 0x275, 0x289, 
    0x298, 0x29a, 0x2a5, 0x2b0, 0x2bb, 0x2c6, 0x2d1, 0x2dc, 0x2e7, 0x2f2, 
    0x2fd, 0x308, 0x30f, 0x31a, 0x32b, 0x32e, 0x341, 0x345, 0x347, 0x34a, 
    0x36b, 0x376, 0x379, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

GPULangParser::Initializer GPULangParser::_init;
