

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

#pragma once


#include "antlr4-runtime.h"
#include "GPULangParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by GPULangParser.
 */
class  GPULangListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterString(GPULangParser::StringContext *ctx) = 0;
  virtual void exitString(GPULangParser::StringContext *ctx) = 0;

  virtual void enterBoolean(GPULangParser::BooleanContext *ctx) = 0;
  virtual void exitBoolean(GPULangParser::BooleanContext *ctx) = 0;

  virtual void enterPreprocess(GPULangParser::PreprocessContext *ctx) = 0;
  virtual void exitPreprocess(GPULangParser::PreprocessContext *ctx) = 0;

  virtual void enterEntry(GPULangParser::EntryContext *ctx) = 0;
  virtual void exitEntry(GPULangParser::EntryContext *ctx) = 0;

  virtual void enterEffect(GPULangParser::EffectContext *ctx) = 0;
  virtual void exitEffect(GPULangParser::EffectContext *ctx) = 0;

  virtual void enterAlias(GPULangParser::AliasContext *ctx) = 0;
  virtual void exitAlias(GPULangParser::AliasContext *ctx) = 0;

  virtual void enterAnnotation(GPULangParser::AnnotationContext *ctx) = 0;
  virtual void exitAnnotation(GPULangParser::AnnotationContext *ctx) = 0;

  virtual void enterAttribute(GPULangParser::AttributeContext *ctx) = 0;
  virtual void exitAttribute(GPULangParser::AttributeContext *ctx) = 0;

  virtual void enterTypeDeclaration(GPULangParser::TypeDeclarationContext *ctx) = 0;
  virtual void exitTypeDeclaration(GPULangParser::TypeDeclarationContext *ctx) = 0;

  virtual void enterVariables(GPULangParser::VariablesContext *ctx) = 0;
  virtual void exitVariables(GPULangParser::VariablesContext *ctx) = 0;

  virtual void enterVariable(GPULangParser::VariableContext *ctx) = 0;
  virtual void exitVariable(GPULangParser::VariableContext *ctx) = 0;

  virtual void enterStructureDeclaration(GPULangParser::StructureDeclarationContext *ctx) = 0;
  virtual void exitStructureDeclaration(GPULangParser::StructureDeclarationContext *ctx) = 0;

  virtual void enterStructure(GPULangParser::StructureContext *ctx) = 0;
  virtual void exitStructure(GPULangParser::StructureContext *ctx) = 0;

  virtual void enterEnumeration(GPULangParser::EnumerationContext *ctx) = 0;
  virtual void exitEnumeration(GPULangParser::EnumerationContext *ctx) = 0;

  virtual void enterFunctionDeclaration(GPULangParser::FunctionDeclarationContext *ctx) = 0;
  virtual void exitFunctionDeclaration(GPULangParser::FunctionDeclarationContext *ctx) = 0;

  virtual void enterCodeblock(GPULangParser::CodeblockContext *ctx) = 0;
  virtual void exitCodeblock(GPULangParser::CodeblockContext *ctx) = 0;

  virtual void enterFunction(GPULangParser::FunctionContext *ctx) = 0;
  virtual void exitFunction(GPULangParser::FunctionContext *ctx) = 0;

  virtual void enterProgram(GPULangParser::ProgramContext *ctx) = 0;
  virtual void exitProgram(GPULangParser::ProgramContext *ctx) = 0;

  virtual void enterState(GPULangParser::StateContext *ctx) = 0;
  virtual void exitState(GPULangParser::StateContext *ctx) = 0;

  virtual void enterStatement(GPULangParser::StatementContext *ctx) = 0;
  virtual void exitStatement(GPULangParser::StatementContext *ctx) = 0;

  virtual void enterExpressionStatement(GPULangParser::ExpressionStatementContext *ctx) = 0;
  virtual void exitExpressionStatement(GPULangParser::ExpressionStatementContext *ctx) = 0;

  virtual void enterIfStatement(GPULangParser::IfStatementContext *ctx) = 0;
  virtual void exitIfStatement(GPULangParser::IfStatementContext *ctx) = 0;

  virtual void enterForStatement(GPULangParser::ForStatementContext *ctx) = 0;
  virtual void exitForStatement(GPULangParser::ForStatementContext *ctx) = 0;

  virtual void enterForRangeStatement(GPULangParser::ForRangeStatementContext *ctx) = 0;
  virtual void exitForRangeStatement(GPULangParser::ForRangeStatementContext *ctx) = 0;

  virtual void enterWhileStatement(GPULangParser::WhileStatementContext *ctx) = 0;
  virtual void exitWhileStatement(GPULangParser::WhileStatementContext *ctx) = 0;

  virtual void enterScopeStatement(GPULangParser::ScopeStatementContext *ctx) = 0;
  virtual void exitScopeStatement(GPULangParser::ScopeStatementContext *ctx) = 0;

  virtual void enterReturnStatement(GPULangParser::ReturnStatementContext *ctx) = 0;
  virtual void exitReturnStatement(GPULangParser::ReturnStatementContext *ctx) = 0;

  virtual void enterContinueStatement(GPULangParser::ContinueStatementContext *ctx) = 0;
  virtual void exitContinueStatement(GPULangParser::ContinueStatementContext *ctx) = 0;

  virtual void enterSwitchStatement(GPULangParser::SwitchStatementContext *ctx) = 0;
  virtual void exitSwitchStatement(GPULangParser::SwitchStatementContext *ctx) = 0;

  virtual void enterBreakStatement(GPULangParser::BreakStatementContext *ctx) = 0;
  virtual void exitBreakStatement(GPULangParser::BreakStatementContext *ctx) = 0;

  virtual void enterExpression(GPULangParser::ExpressionContext *ctx) = 0;
  virtual void exitExpression(GPULangParser::ExpressionContext *ctx) = 0;

  virtual void enterCommaExpression(GPULangParser::CommaExpressionContext *ctx) = 0;
  virtual void exitCommaExpression(GPULangParser::CommaExpressionContext *ctx) = 0;

  virtual void enterAssignmentExpression(GPULangParser::AssignmentExpressionContext *ctx) = 0;
  virtual void exitAssignmentExpression(GPULangParser::AssignmentExpressionContext *ctx) = 0;

  virtual void enterLogicalOrExpression(GPULangParser::LogicalOrExpressionContext *ctx) = 0;
  virtual void exitLogicalOrExpression(GPULangParser::LogicalOrExpressionContext *ctx) = 0;

  virtual void enterLogicalAndExpression(GPULangParser::LogicalAndExpressionContext *ctx) = 0;
  virtual void exitLogicalAndExpression(GPULangParser::LogicalAndExpressionContext *ctx) = 0;

  virtual void enterOrExpression(GPULangParser::OrExpressionContext *ctx) = 0;
  virtual void exitOrExpression(GPULangParser::OrExpressionContext *ctx) = 0;

  virtual void enterXorExpression(GPULangParser::XorExpressionContext *ctx) = 0;
  virtual void exitXorExpression(GPULangParser::XorExpressionContext *ctx) = 0;

  virtual void enterAndExpression(GPULangParser::AndExpressionContext *ctx) = 0;
  virtual void exitAndExpression(GPULangParser::AndExpressionContext *ctx) = 0;

  virtual void enterEquivalencyExpression(GPULangParser::EquivalencyExpressionContext *ctx) = 0;
  virtual void exitEquivalencyExpression(GPULangParser::EquivalencyExpressionContext *ctx) = 0;

  virtual void enterRelationalExpression(GPULangParser::RelationalExpressionContext *ctx) = 0;
  virtual void exitRelationalExpression(GPULangParser::RelationalExpressionContext *ctx) = 0;

  virtual void enterShiftExpression(GPULangParser::ShiftExpressionContext *ctx) = 0;
  virtual void exitShiftExpression(GPULangParser::ShiftExpressionContext *ctx) = 0;

  virtual void enterAddSubtractExpression(GPULangParser::AddSubtractExpressionContext *ctx) = 0;
  virtual void exitAddSubtractExpression(GPULangParser::AddSubtractExpressionContext *ctx) = 0;

  virtual void enterMultiplyDivideExpression(GPULangParser::MultiplyDivideExpressionContext *ctx) = 0;
  virtual void exitMultiplyDivideExpression(GPULangParser::MultiplyDivideExpressionContext *ctx) = 0;

  virtual void enterPrefixExpression(GPULangParser::PrefixExpressionContext *ctx) = 0;
  virtual void exitPrefixExpression(GPULangParser::PrefixExpressionContext *ctx) = 0;

  virtual void enterSuffixExpression(GPULangParser::SuffixExpressionContext *ctx) = 0;
  virtual void exitSuffixExpression(GPULangParser::SuffixExpressionContext *ctx) = 0;

  virtual void enterNamespaceExpression(GPULangParser::NamespaceExpressionContext *ctx) = 0;
  virtual void exitNamespaceExpression(GPULangParser::NamespaceExpressionContext *ctx) = 0;

  virtual void enterBinaryexpatom(GPULangParser::BinaryexpatomContext *ctx) = 0;
  virtual void exitBinaryexpatom(GPULangParser::BinaryexpatomContext *ctx) = 0;

  virtual void enterInitializerExpression(GPULangParser::InitializerExpressionContext *ctx) = 0;
  virtual void exitInitializerExpression(GPULangParser::InitializerExpressionContext *ctx) = 0;


};

