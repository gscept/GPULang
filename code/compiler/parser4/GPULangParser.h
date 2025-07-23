

// include std container
#include <vector>
#include <typeinfo>
#include <iostream>
#include <string>
#include <stack>
#include <tuple>

#include "gpulangtoken.h"
#include "gpulangcompiler.h"
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
#include "ast/preprocessor.h"
#include "ast/variable.h"
#include "ast/generate.h"
#include "ast/statements/breakstatement.h"
#include "ast/statements/continuestatement.h"
#include "ast/statements/expressionstatement.h"
#include "ast/statements/forstatement.h"
#include "ast/statements/ifstatement.h"
#include "ast/statements/terminatestatement.h"
#include "ast/statements/scopestatement.h"
#include "ast/statements/statement.h"
#include "ast/statements/switchstatement.h"
#include "ast/statements/whilestatement.h"
#include "ast/expressions/accessexpression.h"
#include "ast/expressions/arrayindexexpression.h"
#include "ast/expressions/binaryexpression.h"
#include "ast/expressions/boolexpression.h"
#include "ast/expressions/boolvecexpression.h"
#include "ast/expressions/callexpression.h"
#include "ast/expressions/commaexpression.h"
#include "ast/expressions/expression.h"
#include "ast/expressions/floatexpression.h"
#include "ast/expressions/floatvecexpression.h"
#include "ast/expressions/initializerexpression.h"
#include "ast/expressions/arrayinitializerexpression.h"
#include "ast/expressions/intexpression.h"
#include "ast/expressions/intvecexpression.h"
#include "ast/expressions/stringexpression.h"
#include "ast/expressions/symbolexpression.h"
#include "ast/expressions/ternaryexpression.h"
#include "ast/expressions/uintexpression.h"
#include "ast/expressions/uintvecexpression.h"
#include "ast/expressions/unaryexpression.h"
#include "ast/expressions/declaredexpression.h"
#include "util.h"
#include "memory.h"

using namespace GPULang;



// Generated from antlr4/grammar/GPULang.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"




class  GPULangParser : public antlr4::Parser {
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

  enum {
    RuleString = 0, RulePath = 1, RuleBoolean = 2, RuleEntry = 3, RuleEffect = 4, 
    RuleLinePreprocessorEntry = 5, RuleAlias = 6, RuleAnnotation = 7, RuleAttribute = 8, 
    RuleTypeDeclaration = 9, RuleGenerate = 10, RuleGen_statement = 11, 
    RuleGen_scope_statement = 12, RuleGen_if_statement = 13, RuleVariables = 14, 
    RuleStructureDeclaration = 15, RuleStructure = 16, RuleEnumeration = 17, 
    RuleParameter = 18, RuleFunctionDeclaration = 19, RuleFunction = 20, 
    RuleProgram = 21, RuleSampler = 22, RuleState = 23, RuleStatement = 24, 
    RuleExpressionStatement = 25, RuleIfStatement = 26, RuleForStatement = 27, 
    RuleForRangeStatement = 28, RuleForUniformValueStatement = 29, RuleWhileStatement = 30, 
    RuleScopeStatement = 31, RuleTerminateStatement = 32, RuleContinueStatement = 33, 
    RuleSwitchStatement = 34, RuleBreakStatement = 35, RuleExpressionList = 36, 
    RuleExpression = 37, RuleBinaryexpatom = 38, RuleInitializerExpression = 39, 
    RuleArrayInitializerExpression = 40, RuleFloatVecLiteralExpression = 41, 
    RuleDoubleVecLiteralExpression = 42, RuleIntVecLiteralExpression = 43, 
    RuleUintVecLiteralExpression = 44, RuleBooleanVecLiteralExpression = 45
  };

  explicit GPULangParser(antlr4::TokenStream *input);

  GPULangParser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~GPULangParser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;




  // setup function which binds the compiler state to the current AST node
  Symbol::Location
  SetupFile()
  {
      Symbol::Location location;
      ::GPULangToken* token = (::GPULangToken*)_input->LT(-1);

      auto [rawLine, preprocessedLine, file] = GPULangParser::LineStack.back();
      // assume the previous token is the latest file
      location.file = file;
      location.line = token->line - rawLine + preprocessedLine;
      location.start = token->begin;
      location.end = location.start + token->getText().length();
      return location;
  }

  Symbol::Location
  BeginLocationRange()
  {
      Symbol::Location location;
      ::GPULangToken* token = (::GPULangToken*)_input->LT(1);

      auto [rawLine, preprocessedLine, file] = GPULangParser::LineStack.back();
      location.file = file;
      location.line = token->line - rawLine + preprocessedLine;
      location.start = token->begin;
      location.end = token->end + 1;
      return location;
  }

  Symbol::Location
  EndLocationRange(const Symbol::Location begin)
  {
      Symbol::Location location = begin;
      ::GPULangToken* token = (::GPULangToken*)_input->LT(-1);
      location.end = token->end + 1;
      return location;
  }



  class StringContext;
  class PathContext;
  class BooleanContext;
  class EntryContext;
  class EffectContext;
  class LinePreprocessorEntryContext;
  class AliasContext;
  class AnnotationContext;
  class AttributeContext;
  class TypeDeclarationContext;
  class GenerateContext;
  class Gen_statementContext;
  class Gen_scope_statementContext;
  class Gen_if_statementContext;
  class VariablesContext;
  class StructureDeclarationContext;
  class StructureContext;
  class EnumerationContext;
  class ParameterContext;
  class FunctionDeclarationContext;
  class FunctionContext;
  class ProgramContext;
  class SamplerContext;
  class StateContext;
  class StatementContext;
  class ExpressionStatementContext;
  class IfStatementContext;
  class ForStatementContext;
  class ForRangeStatementContext;
  class ForUniformValueStatementContext;
  class WhileStatementContext;
  class ScopeStatementContext;
  class TerminateStatementContext;
  class ContinueStatementContext;
  class SwitchStatementContext;
  class BreakStatementContext;
  class ExpressionListContext;
  class ExpressionContext;
  class BinaryexpatomContext;
  class InitializerExpressionContext;
  class ArrayInitializerExpressionContext;
  class FloatVecLiteralExpressionContext;
  class DoubleVecLiteralExpressionContext;
  class IntVecLiteralExpressionContext;
  class UintVecLiteralExpressionContext;
  class BooleanVecLiteralExpressionContext; 

  class  StringContext : public antlr4::ParserRuleContext {
  public:
    std::string val;
    antlr4::Token *data = nullptr;
    StringContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> QO();
    antlr4::tree::TerminalNode* QO(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Q();
    antlr4::tree::TerminalNode* Q(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  StringContext* string();

  class  PathContext : public antlr4::ParserRuleContext {
  public:
    std::string val;
    antlr4::Token *data = nullptr;
    PathContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> QO();
    antlr4::tree::TerminalNode* QO(size_t i);
    antlr4::tree::TerminalNode *LESS();
    std::vector<antlr4::tree::TerminalNode *> GREATER();
    antlr4::tree::TerminalNode* GREATER(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  PathContext* path();

  class  BooleanContext : public antlr4::ParserRuleContext {
  public:
    bool val;
    BooleanContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  BooleanContext* boolean();

  class  EntryContext : public antlr4::ParserRuleContext {
  public:
    Effect* returnEffect;
    GPULangParser::EffectContext *effectContext = nullptr;
    EntryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    EffectContext *effect();
    antlr4::tree::TerminalNode *EOF();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  EntryContext* entry();

  class  EffectContext : public antlr4::ParserRuleContext {
  public:
    Effect* eff;
    GPULangParser::GenerateContext *generateContext = nullptr;
    GPULangParser::AliasContext *aliasContext = nullptr;
    GPULangParser::FunctionDeclarationContext *functionDeclarationContext = nullptr;
    GPULangParser::FunctionContext *functionContext = nullptr;
    GPULangParser::VariablesContext *variablesContext = nullptr;
    GPULangParser::StructureContext *structureContext = nullptr;
    GPULangParser::EnumerationContext *enumerationContext = nullptr;
    GPULangParser::StateContext *stateContext = nullptr;
    GPULangParser::SamplerContext *samplerContext = nullptr;
    GPULangParser::ProgramContext *programContext = nullptr;
    EffectContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<LinePreprocessorEntryContext *> linePreprocessorEntry();
    LinePreprocessorEntryContext* linePreprocessorEntry(size_t i);
    std::vector<GenerateContext *> generate();
    GenerateContext* generate(size_t i);
    std::vector<antlr4::tree::TerminalNode *> SC();
    antlr4::tree::TerminalNode* SC(size_t i);
    std::vector<AliasContext *> alias();
    AliasContext* alias(size_t i);
    std::vector<FunctionDeclarationContext *> functionDeclaration();
    FunctionDeclarationContext* functionDeclaration(size_t i);
    std::vector<FunctionContext *> function();
    FunctionContext* function(size_t i);
    std::vector<VariablesContext *> variables();
    VariablesContext* variables(size_t i);
    std::vector<StructureContext *> structure();
    StructureContext* structure(size_t i);
    std::vector<EnumerationContext *> enumeration();
    EnumerationContext* enumeration(size_t i);
    std::vector<StateContext *> state();
    StateContext* state(size_t i);
    std::vector<SamplerContext *> sampler();
    SamplerContext* sampler(size_t i);
    std::vector<ProgramContext *> program();
    ProgramContext* program(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  EffectContext* effect();

  class  LinePreprocessorEntryContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *line = nullptr;
    GPULangParser::StringContext *p = nullptr;
    LinePreprocessorEntryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INTEGERLITERAL();
    StringContext *string();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  LinePreprocessorEntryContext* linePreprocessorEntry();

  class  AliasContext : public antlr4::ParserRuleContext {
  public:
    Alias* sym;
    antlr4::Token *name = nullptr;
    antlr4::Token *type = nullptr;
    AliasContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> IDENTIFIER();
    antlr4::tree::TerminalNode* IDENTIFIER(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AliasContext* alias();

  class  AnnotationContext : public antlr4::ParserRuleContext {
  public:
    Annotation* annot;
    antlr4::Token *name = nullptr;
    GPULangParser::ExpressionContext *value = nullptr;
    AnnotationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SOBAKA();
    antlr4::tree::TerminalNode *LP();
    antlr4::tree::TerminalNode *RP();
    antlr4::tree::TerminalNode *IDENTIFIER();
    ExpressionContext *expression();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AnnotationContext* annotation();

  class  AttributeContext : public antlr4::ParserRuleContext {
  public:
    Attribute* attr;
    antlr4::Token *name = nullptr;
    GPULangParser::ExpressionContext *e = nullptr;
    GPULangParser::ExpressionContext *expressionContext = nullptr;
    AttributeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LP();
    antlr4::tree::TerminalNode *RP();
    antlr4::tree::TerminalNode *IDENTIFIER();
    ExpressionContext *expression();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AttributeContext* attribute();

  class  TypeDeclarationContext : public antlr4::ParserRuleContext {
  public:
    TypeDeclaration type;
    GPULangParser::ExpressionContext *arraySize0 = nullptr;
    antlr4::Token *qual = nullptr;
    antlr4::Token *typeName = nullptr;
    TypeDeclarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> IDENTIFIER();
    antlr4::tree::TerminalNode* IDENTIFIER(size_t i);
    std::vector<antlr4::tree::TerminalNode *> MUL_OP();
    antlr4::tree::TerminalNode* MUL_OP(size_t i);
    std::vector<antlr4::tree::TerminalNode *> LL();
    antlr4::tree::TerminalNode* LL(size_t i);
    std::vector<antlr4::tree::TerminalNode *> RR();
    antlr4::tree::TerminalNode* RR(size_t i);
    std::vector<LinePreprocessorEntryContext *> linePreprocessorEntry();
    LinePreprocessorEntryContext* linePreprocessorEntry(size_t i);
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  TypeDeclarationContext* typeDeclaration();

  class  GenerateContext : public antlr4::ParserRuleContext {
  public:
    Symbol* sym;
    GPULangParser::VariablesContext *variablesContext = nullptr;
    GPULangParser::Gen_statementContext *gen_statementContext = nullptr;
    GPULangParser::AliasContext *aliasContext = nullptr;
    GPULangParser::FunctionDeclarationContext *functionDeclarationContext = nullptr;
    GPULangParser::FunctionContext *functionContext = nullptr;
    GenerateContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LESS();
    antlr4::tree::TerminalNode *GREATER();
    std::vector<VariablesContext *> variables();
    VariablesContext* variables(size_t i);
    std::vector<antlr4::tree::TerminalNode *> SC();
    antlr4::tree::TerminalNode* SC(size_t i);
    std::vector<Gen_statementContext *> gen_statement();
    Gen_statementContext* gen_statement(size_t i);
    std::vector<AliasContext *> alias();
    AliasContext* alias(size_t i);
    std::vector<FunctionDeclarationContext *> functionDeclaration();
    FunctionDeclarationContext* functionDeclaration(size_t i);
    std::vector<FunctionContext *> function();
    FunctionContext* function(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  GenerateContext* generate();

  class  Gen_statementContext : public antlr4::ParserRuleContext {
  public:
    Statement* tree;
    GPULangParser::Gen_if_statementContext *gen_if_statementContext = nullptr;
    GPULangParser::Gen_scope_statementContext *gen_scope_statementContext = nullptr;
    Gen_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Gen_if_statementContext *gen_if_statement();
    Gen_scope_statementContext *gen_scope_statement();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Gen_statementContext* gen_statement();

  class  Gen_scope_statementContext : public antlr4::ParserRuleContext {
  public:
    ScopeStatement* tree;
    GPULangParser::VariablesContext *variablesContext = nullptr;
    GPULangParser::Gen_statementContext *gen_statementContext = nullptr;
    GPULangParser::AliasContext *aliasContext = nullptr;
    GPULangParser::FunctionContext *functionContext = nullptr;
    Gen_scope_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LESS();
    antlr4::tree::TerminalNode *GREATER();
    std::vector<VariablesContext *> variables();
    VariablesContext* variables(size_t i);
    std::vector<antlr4::tree::TerminalNode *> SC();
    antlr4::tree::TerminalNode* SC(size_t i);
    std::vector<Gen_statementContext *> gen_statement();
    Gen_statementContext* gen_statement(size_t i);
    std::vector<AliasContext *> alias();
    AliasContext* alias(size_t i);
    std::vector<FunctionContext *> function();
    FunctionContext* function(size_t i);
    std::vector<LinePreprocessorEntryContext *> linePreprocessorEntry();
    LinePreprocessorEntryContext* linePreprocessorEntry(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Gen_scope_statementContext* gen_scope_statement();

  class  Gen_if_statementContext : public antlr4::ParserRuleContext {
  public:
    Statement* tree;
    GPULangParser::ExpressionContext *condition = nullptr;
    GPULangParser::Gen_statementContext *ifBody = nullptr;
    GPULangParser::Gen_statementContext *elseBody = nullptr;
    Gen_if_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LP();
    antlr4::tree::TerminalNode *RP();
    ExpressionContext *expression();
    std::vector<Gen_statementContext *> gen_statement();
    Gen_statementContext* gen_statement(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Gen_if_statementContext* gen_if_statement();

  class  VariablesContext : public antlr4::ParserRuleContext {
  public:
    FixedArray<Variable*> vars;
    GPULangParser::AnnotationContext *annotationContext = nullptr;
    GPULangParser::AttributeContext *attributeContext = nullptr;
    antlr4::Token *varName = nullptr;
    antlr4::Token *varNameN = nullptr;
    GPULangParser::TypeDeclarationContext *typeDeclarationContext = nullptr;
    GPULangParser::ExpressionContext *valueExpr = nullptr;
    GPULangParser::ExpressionContext *valueExprN = nullptr;
    VariablesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> IDENTIFIER();
    antlr4::tree::TerminalNode* IDENTIFIER(size_t i);
    std::vector<LinePreprocessorEntryContext *> linePreprocessorEntry();
    LinePreprocessorEntryContext* linePreprocessorEntry(size_t i);
    std::vector<AnnotationContext *> annotation();
    AnnotationContext* annotation(size_t i);
    std::vector<AttributeContext *> attribute();
    AttributeContext* attribute(size_t i);
    std::vector<antlr4::tree::TerminalNode *> CO();
    antlr4::tree::TerminalNode* CO(size_t i);
    antlr4::tree::TerminalNode *COL();
    TypeDeclarationContext *typeDeclaration();
    antlr4::tree::TerminalNode *EQ();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  VariablesContext* variables();

  class  StructureDeclarationContext : public antlr4::ParserRuleContext {
  public:
    Structure* sym;
    GPULangParser::AnnotationContext *annotationContext = nullptr;
    GPULangParser::AttributeContext *attributeContext = nullptr;
    antlr4::Token *name = nullptr;
    StructureDeclarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    std::vector<LinePreprocessorEntryContext *> linePreprocessorEntry();
    LinePreprocessorEntryContext* linePreprocessorEntry(size_t i);
    std::vector<AnnotationContext *> annotation();
    AnnotationContext* annotation(size_t i);
    std::vector<AttributeContext *> attribute();
    AttributeContext* attribute(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  StructureDeclarationContext* structureDeclaration();

  class  StructureContext : public antlr4::ParserRuleContext {
  public:
    Structure* sym;
    GPULangParser::StructureDeclarationContext *structureDeclarationContext = nullptr;
    antlr4::Token *varName = nullptr;
    GPULangParser::ExpressionContext *arraySize0 = nullptr;
    antlr4::Token *varTypeName = nullptr;
    StructureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    StructureDeclarationContext *structureDeclaration();
    antlr4::tree::TerminalNode *LB();
    antlr4::tree::TerminalNode *RB();
    std::vector<antlr4::tree::TerminalNode *> COL();
    antlr4::tree::TerminalNode* COL(size_t i);
    std::vector<antlr4::tree::TerminalNode *> SC();
    antlr4::tree::TerminalNode* SC(size_t i);
    std::vector<LinePreprocessorEntryContext *> linePreprocessorEntry();
    LinePreprocessorEntryContext* linePreprocessorEntry(size_t i);
    std::vector<antlr4::tree::TerminalNode *> IDENTIFIER();
    antlr4::tree::TerminalNode* IDENTIFIER(size_t i);
    std::vector<antlr4::tree::TerminalNode *> MUL_OP();
    antlr4::tree::TerminalNode* MUL_OP(size_t i);
    std::vector<antlr4::tree::TerminalNode *> LL();
    antlr4::tree::TerminalNode* LL(size_t i);
    std::vector<antlr4::tree::TerminalNode *> RR();
    antlr4::tree::TerminalNode* RR(size_t i);
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  StructureContext* structure();

  class  EnumerationContext : public antlr4::ParserRuleContext {
  public:
    Enumeration* sym;
    antlr4::Token *name = nullptr;
    GPULangParser::TypeDeclarationContext *typeDeclarationContext = nullptr;
    antlr4::Token *label = nullptr;
    GPULangParser::ExpressionContext *value = nullptr;
    EnumerationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LB();
    antlr4::tree::TerminalNode *RB();
    std::vector<antlr4::tree::TerminalNode *> IDENTIFIER();
    antlr4::tree::TerminalNode* IDENTIFIER(size_t i);
    antlr4::tree::TerminalNode *COL();
    TypeDeclarationContext *typeDeclaration();
    std::vector<LinePreprocessorEntryContext *> linePreprocessorEntry();
    LinePreprocessorEntryContext* linePreprocessorEntry(size_t i);
    std::vector<antlr4::tree::TerminalNode *> EQ();
    antlr4::tree::TerminalNode* EQ(size_t i);
    std::vector<antlr4::tree::TerminalNode *> CO();
    antlr4::tree::TerminalNode* CO(size_t i);
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  EnumerationContext* enumeration();

  class  ParameterContext : public antlr4::ParserRuleContext {
  public:
    Variable* sym;
    GPULangParser::AttributeContext *attributeContext = nullptr;
    antlr4::Token *varName = nullptr;
    GPULangParser::TypeDeclarationContext *typeDeclarationContext = nullptr;
    GPULangParser::ExpressionContext *valueExpr = nullptr;
    ParameterContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *COL();
    TypeDeclarationContext *typeDeclaration();
    antlr4::tree::TerminalNode *IDENTIFIER();
    std::vector<LinePreprocessorEntryContext *> linePreprocessorEntry();
    LinePreprocessorEntryContext* linePreprocessorEntry(size_t i);
    std::vector<AttributeContext *> attribute();
    AttributeContext* attribute(size_t i);
    antlr4::tree::TerminalNode *EQ();
    ExpressionContext *expression();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ParameterContext* parameter();

  class  FunctionDeclarationContext : public antlr4::ParserRuleContext {
  public:
    Function* sym;
    GPULangParser::AttributeContext *attributeContext = nullptr;
    antlr4::Token *name = nullptr;
    GPULangParser::ParameterContext *arg0 = nullptr;
    GPULangParser::ParameterContext *argn = nullptr;
    GPULangParser::TypeDeclarationContext *returnType = nullptr;
    FunctionDeclarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LP();
    antlr4::tree::TerminalNode *RP();
    antlr4::tree::TerminalNode *IDENTIFIER();
    TypeDeclarationContext *typeDeclaration();
    std::vector<AttributeContext *> attribute();
    AttributeContext* attribute(size_t i);
    std::vector<ParameterContext *> parameter();
    ParameterContext* parameter(size_t i);
    std::vector<LinePreprocessorEntryContext *> linePreprocessorEntry();
    LinePreprocessorEntryContext* linePreprocessorEntry(size_t i);
    std::vector<antlr4::tree::TerminalNode *> CO();
    antlr4::tree::TerminalNode* CO(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  FunctionDeclarationContext* functionDeclaration();

  class  FunctionContext : public antlr4::ParserRuleContext {
  public:
    Function* sym;
    GPULangParser::FunctionDeclarationContext *functionDeclarationContext = nullptr;
    GPULangParser::ScopeStatementContext *scopeStatementContext = nullptr;
    FunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    FunctionDeclarationContext *functionDeclaration();
    ScopeStatementContext *scopeStatement();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  FunctionContext* function();

  class  ProgramContext : public antlr4::ParserRuleContext {
  public:
    ProgramInstance* sym;
    GPULangParser::AnnotationContext *annotationContext = nullptr;
    antlr4::Token *name = nullptr;
    GPULangParser::ExpressionContext *assignment = nullptr;
    ProgramContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LB();
    antlr4::tree::TerminalNode *RB();
    antlr4::tree::TerminalNode *IDENTIFIER();
    std::vector<AnnotationContext *> annotation();
    AnnotationContext* annotation(size_t i);
    std::vector<antlr4::tree::TerminalNode *> SC();
    antlr4::tree::TerminalNode* SC(size_t i);
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ProgramContext* program();

  class  SamplerContext : public antlr4::ParserRuleContext {
  public:
    SamplerStateInstance* sym;
    GPULangParser::AnnotationContext *annotationContext = nullptr;
    GPULangParser::AttributeContext *attributeContext = nullptr;
    antlr4::Token *name = nullptr;
    GPULangParser::ExpressionContext *assign = nullptr;
    SamplerContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LB();
    antlr4::tree::TerminalNode *RB();
    antlr4::tree::TerminalNode *IDENTIFIER();
    std::vector<antlr4::tree::TerminalNode *> SC();
    antlr4::tree::TerminalNode* SC(size_t i);
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    std::vector<AnnotationContext *> annotation();
    AnnotationContext* annotation(size_t i);
    std::vector<AttributeContext *> attribute();
    AttributeContext* attribute(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  SamplerContext* sampler();

  class  StateContext : public antlr4::ParserRuleContext {
  public:
    State* sym;
    antlr4::Token *name = nullptr;
    GPULangParser::ExpressionContext *assign = nullptr;
    StateContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LB();
    antlr4::tree::TerminalNode *RB();
    antlr4::tree::TerminalNode *IDENTIFIER();
    std::vector<antlr4::tree::TerminalNode *> SC();
    antlr4::tree::TerminalNode* SC(size_t i);
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  StateContext* state();

  class  StatementContext : public antlr4::ParserRuleContext {
  public:
    Statement* tree;
    GPULangParser::IfStatementContext *ifStatementContext = nullptr;
    GPULangParser::ScopeStatementContext *scopeStatementContext = nullptr;
    GPULangParser::ForStatementContext *forStatementContext = nullptr;
    GPULangParser::WhileStatementContext *whileStatementContext = nullptr;
    GPULangParser::SwitchStatementContext *switchStatementContext = nullptr;
    GPULangParser::TerminateStatementContext *terminateStatementContext = nullptr;
    GPULangParser::ContinueStatementContext *continueStatementContext = nullptr;
    GPULangParser::BreakStatementContext *breakStatementContext = nullptr;
    GPULangParser::ExpressionStatementContext *expressionStatementContext = nullptr;
    StatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IfStatementContext *ifStatement();
    ScopeStatementContext *scopeStatement();
    ForStatementContext *forStatement();
    WhileStatementContext *whileStatement();
    SwitchStatementContext *switchStatement();
    TerminateStatementContext *terminateStatement();
    ContinueStatementContext *continueStatement();
    BreakStatementContext *breakStatement();
    ExpressionStatementContext *expressionStatement();
    antlr4::tree::TerminalNode *SC();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  StatementContext* statement();

  class  ExpressionStatementContext : public antlr4::ParserRuleContext {
  public:
    Statement* tree;
    GPULangParser::ExpressionListContext *expressionListContext = nullptr;
    ExpressionStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExpressionListContext *expressionList();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ExpressionStatementContext* expressionStatement();

  class  IfStatementContext : public antlr4::ParserRuleContext {
  public:
    Statement* tree;
    GPULangParser::ExpressionContext *condition = nullptr;
    GPULangParser::StatementContext *ifBody = nullptr;
    GPULangParser::StatementContext *elseBody = nullptr;
    IfStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LP();
    antlr4::tree::TerminalNode *RP();
    ExpressionContext *expression();
    std::vector<StatementContext *> statement();
    StatementContext* statement(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  IfStatementContext* ifStatement();

  class  ForStatementContext : public antlr4::ParserRuleContext {
  public:
    Statement* tree;
    GPULangParser::VariablesContext *variablesContext = nullptr;
    GPULangParser::ExpressionContext *condition = nullptr;
    GPULangParser::ExpressionContext *loop = nullptr;
    GPULangParser::AttributeContext *attributeContext = nullptr;
    GPULangParser::StatementContext *content = nullptr;
    ForStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LP();
    std::vector<antlr4::tree::TerminalNode *> SC();
    antlr4::tree::TerminalNode* SC(size_t i);
    antlr4::tree::TerminalNode *RP();
    StatementContext *statement();
    VariablesContext *variables();
    std::vector<AttributeContext *> attribute();
    AttributeContext* attribute(size_t i);
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ForStatementContext* forStatement();

  class  ForRangeStatementContext : public antlr4::ParserRuleContext {
  public:
    Statement* tree;
    antlr4::Token *iterator = nullptr;
    antlr4::Token *start = nullptr;
    antlr4::Token *end = nullptr;
    GPULangParser::StatementContext *content = nullptr;
    ForRangeStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LP();
    antlr4::tree::TerminalNode *COL();
    antlr4::tree::TerminalNode *RP();
    std::vector<antlr4::tree::TerminalNode *> IDENTIFIER();
    antlr4::tree::TerminalNode* IDENTIFIER(size_t i);
    StatementContext *statement();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ForRangeStatementContext* forRangeStatement();

  class  ForUniformValueStatementContext : public antlr4::ParserRuleContext {
  public:
    Statement* tree;
    GPULangParser::StatementContext *content = nullptr;
    ForUniformValueStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LP();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *RP();
    StatementContext *statement();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ForUniformValueStatementContext* forUniformValueStatement();

  class  WhileStatementContext : public antlr4::ParserRuleContext {
  public:
    Statement* tree;
    GPULangParser::ExpressionContext *condition = nullptr;
    GPULangParser::StatementContext *content = nullptr;
    WhileStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LP();
    antlr4::tree::TerminalNode *RP();
    ExpressionContext *expression();
    StatementContext *statement();
    antlr4::tree::TerminalNode *SC();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  WhileStatementContext* whileStatement();

  class  ScopeStatementContext : public antlr4::ParserRuleContext {
  public:
    ScopeStatement* tree;
    GPULangParser::VariablesContext *variablesContext = nullptr;
    GPULangParser::StatementContext *statementContext = nullptr;
    GPULangParser::AliasContext *aliasContext = nullptr;
    ScopeStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LB();
    antlr4::tree::TerminalNode *RB();
    std::vector<VariablesContext *> variables();
    VariablesContext* variables(size_t i);
    std::vector<antlr4::tree::TerminalNode *> SC();
    antlr4::tree::TerminalNode* SC(size_t i);
    std::vector<StatementContext *> statement();
    StatementContext* statement(size_t i);
    std::vector<AliasContext *> alias();
    AliasContext* alias(size_t i);
    std::vector<LinePreprocessorEntryContext *> linePreprocessorEntry();
    LinePreprocessorEntryContext* linePreprocessorEntry(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ScopeStatementContext* scopeStatement();

  class  TerminateStatementContext : public antlr4::ParserRuleContext {
  public:
    Statement* tree;
    GPULangParser::ExpressionContext *value = nullptr;
    TerminateStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SC();
    ExpressionContext *expression();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  TerminateStatementContext* terminateStatement();

  class  ContinueStatementContext : public antlr4::ParserRuleContext {
  public:
    Statement* tree;
    ContinueStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SC();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ContinueStatementContext* continueStatement();

  class  SwitchStatementContext : public antlr4::ParserRuleContext {
  public:
    Statement* tree;
    GPULangParser::ExpressionContext *expressionContext = nullptr;
    GPULangParser::StatementContext *statementContext = nullptr;
    SwitchStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LP();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    antlr4::tree::TerminalNode *RP();
    antlr4::tree::TerminalNode *LB();
    antlr4::tree::TerminalNode *RB();
    std::vector<antlr4::tree::TerminalNode *> COL();
    antlr4::tree::TerminalNode* COL(size_t i);
    std::vector<StatementContext *> statement();
    StatementContext* statement(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  SwitchStatementContext* switchStatement();

  class  BreakStatementContext : public antlr4::ParserRuleContext {
  public:
    Statement* tree;
    BreakStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SC();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  BreakStatementContext* breakStatement();

  class  ExpressionListContext : public antlr4::ParserRuleContext {
  public:
    FixedArray<Expression*> expressions;
    GPULangParser::ExpressionContext *e1 = nullptr;
    GPULangParser::ExpressionContext *e2 = nullptr;
    ExpressionListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    std::vector<LinePreprocessorEntryContext *> linePreprocessorEntry();
    LinePreprocessorEntryContext* linePreprocessorEntry(size_t i);
    std::vector<antlr4::tree::TerminalNode *> CO();
    antlr4::tree::TerminalNode* CO(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ExpressionListContext* expressionList();

  class  ExpressionContext : public antlr4::ParserRuleContext {
  public:
    Expression* tree;
    GPULangParser::ExpressionContext *e1 = nullptr;
    antlr4::Token *op = nullptr;
    GPULangParser::ExpressionContext *p = nullptr;
    GPULangParser::BinaryexpatomContext *atom = nullptr;
    GPULangParser::ExpressionContext *e2 = nullptr;
    GPULangParser::ExpressionContext *ifBody = nullptr;
    GPULangParser::ExpressionContext *elseBody = nullptr;
    GPULangParser::ExpressionListContext *list = nullptr;
    GPULangParser::ExpressionContext *e3 = nullptr;
    ExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    antlr4::tree::TerminalNode *SUB_OP();
    antlr4::tree::TerminalNode *ADD_OP();
    antlr4::tree::TerminalNode *NOT();
    antlr4::tree::TerminalNode *CONJUGATE();
    antlr4::tree::TerminalNode *MUL_OP();
    antlr4::tree::TerminalNode *AND();
    BinaryexpatomContext *binaryexpatom();
    antlr4::tree::TerminalNode *DOT();
    antlr4::tree::TerminalNode *ARROW();
    antlr4::tree::TerminalNode *DIV_OP();
    antlr4::tree::TerminalNode *MOD();
    antlr4::tree::TerminalNode *LESS();
    antlr4::tree::TerminalNode *GREATER();
    antlr4::tree::TerminalNode *LESSEQ();
    antlr4::tree::TerminalNode *GREATEREQ();
    antlr4::tree::TerminalNode *LOGICEQ();
    antlr4::tree::TerminalNode *NOTEQ();
    antlr4::tree::TerminalNode *XOR();
    antlr4::tree::TerminalNode *OR();
    antlr4::tree::TerminalNode *LOGICAND();
    antlr4::tree::TerminalNode *LOGICOR();
    antlr4::tree::TerminalNode *QU();
    antlr4::tree::TerminalNode *COL();
    antlr4::tree::TerminalNode *ANDSET();
    antlr4::tree::TerminalNode *XORSET();
    antlr4::tree::TerminalNode *ORSET();
    antlr4::tree::TerminalNode *EQ();
    antlr4::tree::TerminalNode *LP();
    antlr4::tree::TerminalNode *RP();
    ExpressionListContext *expressionList();
    antlr4::tree::TerminalNode *LL();
    antlr4::tree::TerminalNode *RR();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ExpressionContext* expression();
  ExpressionContext* expression(int precedence);
  class  BinaryexpatomContext : public antlr4::ParserRuleContext {
  public:
    Expression* tree;
    GPULangParser::InitializerExpressionContext *initializerExpressionContext = nullptr;
    GPULangParser::ArrayInitializerExpressionContext *arrayInitializerExpressionContext = nullptr;
    GPULangParser::ExpressionContext *expressionContext = nullptr;
    antlr4::Token *integerliteralToken = nullptr;
    antlr4::Token *uintegerliteralToken = nullptr;
    antlr4::Token *floatliteralToken = nullptr;
    antlr4::Token *doubleliteralToken = nullptr;
    antlr4::Token *hexToken = nullptr;
    GPULangParser::StringContext *stringContext = nullptr;
    antlr4::Token *identifierToken = nullptr;
    GPULangParser::BooleanContext *booleanContext = nullptr;
    antlr4::Token *ident = nullptr;
    BinaryexpatomContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    InitializerExpressionContext *initializerExpression();
    ArrayInitializerExpressionContext *arrayInitializerExpression();
    antlr4::tree::TerminalNode *LP();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *RP();
    antlr4::tree::TerminalNode *INTEGERLITERAL();
    antlr4::tree::TerminalNode *UINTEGERLITERAL();
    antlr4::tree::TerminalNode *FLOATLITERAL();
    antlr4::tree::TerminalNode *DOUBLELITERAL();
    antlr4::tree::TerminalNode *HEX();
    StringContext *string();
    antlr4::tree::TerminalNode *IDENTIFIER();
    BooleanContext *boolean();
    antlr4::tree::TerminalNode *LESS();
    antlr4::tree::TerminalNode *GREATER();
    LinePreprocessorEntryContext *linePreprocessorEntry();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  BinaryexpatomContext* binaryexpatom();

  class  InitializerExpressionContext : public antlr4::ParserRuleContext {
  public:
    Expression* tree;
    antlr4::Token *type = nullptr;
    GPULangParser::ExpressionListContext *list = nullptr;
    InitializerExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LB();
    antlr4::tree::TerminalNode *RB();
    antlr4::tree::TerminalNode *IDENTIFIER();
    ExpressionListContext *expressionList();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  InitializerExpressionContext* initializerExpression();

  class  ArrayInitializerExpressionContext : public antlr4::ParserRuleContext {
  public:
    Expression* tree;
    GPULangParser::ExpressionListContext *list = nullptr;
    ArrayInitializerExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LL();
    antlr4::tree::TerminalNode *RR();
    ExpressionListContext *expressionList();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ArrayInitializerExpressionContext* arrayInitializerExpression();

  class  FloatVecLiteralExpressionContext : public antlr4::ParserRuleContext {
  public:
    Expression* tree;
    antlr4::Token *arg0 = nullptr;
    antlr4::Token *argN = nullptr;
    FloatVecLiteralExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LESS();
    antlr4::tree::TerminalNode *GREATER();
    std::vector<antlr4::tree::TerminalNode *> FLOATLITERAL();
    antlr4::tree::TerminalNode* FLOATLITERAL(size_t i);
    LinePreprocessorEntryContext *linePreprocessorEntry();
    std::vector<antlr4::tree::TerminalNode *> CO();
    antlr4::tree::TerminalNode* CO(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  FloatVecLiteralExpressionContext* floatVecLiteralExpression();

  class  DoubleVecLiteralExpressionContext : public antlr4::ParserRuleContext {
  public:
    Expression* tree;
    antlr4::Token *arg0 = nullptr;
    antlr4::Token *argN = nullptr;
    DoubleVecLiteralExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LESS();
    antlr4::tree::TerminalNode *GREATER();
    std::vector<antlr4::tree::TerminalNode *> DOUBLELITERAL();
    antlr4::tree::TerminalNode* DOUBLELITERAL(size_t i);
    LinePreprocessorEntryContext *linePreprocessorEntry();
    std::vector<antlr4::tree::TerminalNode *> CO();
    antlr4::tree::TerminalNode* CO(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  DoubleVecLiteralExpressionContext* doubleVecLiteralExpression();

  class  IntVecLiteralExpressionContext : public antlr4::ParserRuleContext {
  public:
    Expression* tree;
    antlr4::Token *arg0 = nullptr;
    antlr4::Token *argN = nullptr;
    IntVecLiteralExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LESS();
    antlr4::tree::TerminalNode *GREATER();
    std::vector<antlr4::tree::TerminalNode *> INTEGERLITERAL();
    antlr4::tree::TerminalNode* INTEGERLITERAL(size_t i);
    LinePreprocessorEntryContext *linePreprocessorEntry();
    std::vector<antlr4::tree::TerminalNode *> CO();
    antlr4::tree::TerminalNode* CO(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  IntVecLiteralExpressionContext* intVecLiteralExpression();

  class  UintVecLiteralExpressionContext : public antlr4::ParserRuleContext {
  public:
    Expression* tree;
    antlr4::Token *arg0 = nullptr;
    antlr4::Token *argN = nullptr;
    UintVecLiteralExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LESS();
    antlr4::tree::TerminalNode *GREATER();
    std::vector<antlr4::tree::TerminalNode *> UINTEGERLITERAL();
    antlr4::tree::TerminalNode* UINTEGERLITERAL(size_t i);
    LinePreprocessorEntryContext *linePreprocessorEntry();
    std::vector<antlr4::tree::TerminalNode *> CO();
    antlr4::tree::TerminalNode* CO(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  UintVecLiteralExpressionContext* uintVecLiteralExpression();

  class  BooleanVecLiteralExpressionContext : public antlr4::ParserRuleContext {
  public:
    Expression* tree;
    GPULangParser::BooleanContext *arg0 = nullptr;
    GPULangParser::BooleanContext *argN = nullptr;
    BooleanVecLiteralExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LESS();
    antlr4::tree::TerminalNode *GREATER();
    std::vector<BooleanContext *> boolean();
    BooleanContext* boolean(size_t i);
    LinePreprocessorEntryContext *linePreprocessorEntry();
    std::vector<antlr4::tree::TerminalNode *> CO();
    antlr4::tree::TerminalNode* CO(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  BooleanVecLiteralExpressionContext* booleanVecLiteralExpression();


  bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;

  bool expressionSempred(ExpressionContext *_localctx, size_t predicateIndex);

  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  friend class GPULangLexerErrorHandler;
  friend class GPULangParserErrorHandler;
  friend class GPULangTokenFactory;
  friend bool GPULangCompile(const std::string&, GPULang::Compiler::Language, const std::string&, const std::string&, const std::vector<std::string>&, GPULang::Compiler::Options, GPULangErrorBlob*&);
  friend bool GPULangValidate(GPULangFile*, GPULang::Compiler::Language, const std::vector<std::string>&, GPULang::Compiler::Options, GPULangServerResult&);

  friend bool GPULangPreprocess(GPULangFile*, const std::vector<std::string>&, std::string&, GPULang::PinnedArray<GPULang::Symbol*>&, GPULang::PinnedArray<GPULang::Diagnostic>&);
  friend GPULangFile* GPULangLoadFile(const std::string_view&, const std::vector<std::string_view>&);
  static std::vector<std::tuple<size_t, size_t, std::string>> LineStack;

};

