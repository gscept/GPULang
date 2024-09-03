

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



// Generated from antlr4/grammar/AnyFX.g4 by ANTLR 4.7.1

#pragma once


#include "antlr4-runtime.h"
#include "AnyFXParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by AnyFXParser.
 */
class  AnyFXListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterString(AnyFXParser::StringContext *ctx) = 0;
  virtual void exitString(AnyFXParser::StringContext *ctx) = 0;

  virtual void enterBoolean(AnyFXParser::BooleanContext *ctx) = 0;
  virtual void exitBoolean(AnyFXParser::BooleanContext *ctx) = 0;

  virtual void enterPreprocess(AnyFXParser::PreprocessContext *ctx) = 0;
  virtual void exitPreprocess(AnyFXParser::PreprocessContext *ctx) = 0;

  virtual void enterEntry(AnyFXParser::EntryContext *ctx) = 0;
  virtual void exitEntry(AnyFXParser::EntryContext *ctx) = 0;

  virtual void enterEffect(AnyFXParser::EffectContext *ctx) = 0;
  virtual void exitEffect(AnyFXParser::EffectContext *ctx) = 0;

  virtual void enterAlias(AnyFXParser::AliasContext *ctx) = 0;
  virtual void exitAlias(AnyFXParser::AliasContext *ctx) = 0;

  virtual void enterAnnotation(AnyFXParser::AnnotationContext *ctx) = 0;
  virtual void exitAnnotation(AnyFXParser::AnnotationContext *ctx) = 0;

  virtual void enterAttribute(AnyFXParser::AttributeContext *ctx) = 0;
  virtual void exitAttribute(AnyFXParser::AttributeContext *ctx) = 0;

  virtual void enterTypeDeclaration(AnyFXParser::TypeDeclarationContext *ctx) = 0;
  virtual void exitTypeDeclaration(AnyFXParser::TypeDeclarationContext *ctx) = 0;

  virtual void enterVariables(AnyFXParser::VariablesContext *ctx) = 0;
  virtual void exitVariables(AnyFXParser::VariablesContext *ctx) = 0;

  virtual void enterVariable(AnyFXParser::VariableContext *ctx) = 0;
  virtual void exitVariable(AnyFXParser::VariableContext *ctx) = 0;

  virtual void enterStructureDeclaration(AnyFXParser::StructureDeclarationContext *ctx) = 0;
  virtual void exitStructureDeclaration(AnyFXParser::StructureDeclarationContext *ctx) = 0;

  virtual void enterStructure(AnyFXParser::StructureContext *ctx) = 0;
  virtual void exitStructure(AnyFXParser::StructureContext *ctx) = 0;

  virtual void enterEnumeration(AnyFXParser::EnumerationContext *ctx) = 0;
  virtual void exitEnumeration(AnyFXParser::EnumerationContext *ctx) = 0;

  virtual void enterFunctionDeclaration(AnyFXParser::FunctionDeclarationContext *ctx) = 0;
  virtual void exitFunctionDeclaration(AnyFXParser::FunctionDeclarationContext *ctx) = 0;

  virtual void enterCodeblock(AnyFXParser::CodeblockContext *ctx) = 0;
  virtual void exitCodeblock(AnyFXParser::CodeblockContext *ctx) = 0;

  virtual void enterFunction(AnyFXParser::FunctionContext *ctx) = 0;
  virtual void exitFunction(AnyFXParser::FunctionContext *ctx) = 0;

  virtual void enterProgram(AnyFXParser::ProgramContext *ctx) = 0;
  virtual void exitProgram(AnyFXParser::ProgramContext *ctx) = 0;

  virtual void enterState(AnyFXParser::StateContext *ctx) = 0;
  virtual void exitState(AnyFXParser::StateContext *ctx) = 0;

  virtual void enterStatement(AnyFXParser::StatementContext *ctx) = 0;
  virtual void exitStatement(AnyFXParser::StatementContext *ctx) = 0;

  virtual void enterExpressionStatement(AnyFXParser::ExpressionStatementContext *ctx) = 0;
  virtual void exitExpressionStatement(AnyFXParser::ExpressionStatementContext *ctx) = 0;

  virtual void enterIfStatement(AnyFXParser::IfStatementContext *ctx) = 0;
  virtual void exitIfStatement(AnyFXParser::IfStatementContext *ctx) = 0;

  virtual void enterForStatement(AnyFXParser::ForStatementContext *ctx) = 0;
  virtual void exitForStatement(AnyFXParser::ForStatementContext *ctx) = 0;

  virtual void enterForRangeStatement(AnyFXParser::ForRangeStatementContext *ctx) = 0;
  virtual void exitForRangeStatement(AnyFXParser::ForRangeStatementContext *ctx) = 0;

  virtual void enterWhileStatement(AnyFXParser::WhileStatementContext *ctx) = 0;
  virtual void exitWhileStatement(AnyFXParser::WhileStatementContext *ctx) = 0;

  virtual void enterScopeStatement(AnyFXParser::ScopeStatementContext *ctx) = 0;
  virtual void exitScopeStatement(AnyFXParser::ScopeStatementContext *ctx) = 0;

  virtual void enterReturnStatement(AnyFXParser::ReturnStatementContext *ctx) = 0;
  virtual void exitReturnStatement(AnyFXParser::ReturnStatementContext *ctx) = 0;

  virtual void enterContinueStatement(AnyFXParser::ContinueStatementContext *ctx) = 0;
  virtual void exitContinueStatement(AnyFXParser::ContinueStatementContext *ctx) = 0;

  virtual void enterSwitchStatement(AnyFXParser::SwitchStatementContext *ctx) = 0;
  virtual void exitSwitchStatement(AnyFXParser::SwitchStatementContext *ctx) = 0;

  virtual void enterBreakStatement(AnyFXParser::BreakStatementContext *ctx) = 0;
  virtual void exitBreakStatement(AnyFXParser::BreakStatementContext *ctx) = 0;

  virtual void enterExpression(AnyFXParser::ExpressionContext *ctx) = 0;
  virtual void exitExpression(AnyFXParser::ExpressionContext *ctx) = 0;

  virtual void enterCommaExpression(AnyFXParser::CommaExpressionContext *ctx) = 0;
  virtual void exitCommaExpression(AnyFXParser::CommaExpressionContext *ctx) = 0;

  virtual void enterAssignmentExpression(AnyFXParser::AssignmentExpressionContext *ctx) = 0;
  virtual void exitAssignmentExpression(AnyFXParser::AssignmentExpressionContext *ctx) = 0;

  virtual void enterLogicalOrExpression(AnyFXParser::LogicalOrExpressionContext *ctx) = 0;
  virtual void exitLogicalOrExpression(AnyFXParser::LogicalOrExpressionContext *ctx) = 0;

  virtual void enterLogicalAndExpression(AnyFXParser::LogicalAndExpressionContext *ctx) = 0;
  virtual void exitLogicalAndExpression(AnyFXParser::LogicalAndExpressionContext *ctx) = 0;

  virtual void enterOrExpression(AnyFXParser::OrExpressionContext *ctx) = 0;
  virtual void exitOrExpression(AnyFXParser::OrExpressionContext *ctx) = 0;

  virtual void enterXorExpression(AnyFXParser::XorExpressionContext *ctx) = 0;
  virtual void exitXorExpression(AnyFXParser::XorExpressionContext *ctx) = 0;

  virtual void enterAndExpression(AnyFXParser::AndExpressionContext *ctx) = 0;
  virtual void exitAndExpression(AnyFXParser::AndExpressionContext *ctx) = 0;

  virtual void enterEquivalencyExpression(AnyFXParser::EquivalencyExpressionContext *ctx) = 0;
  virtual void exitEquivalencyExpression(AnyFXParser::EquivalencyExpressionContext *ctx) = 0;

  virtual void enterRelationalExpression(AnyFXParser::RelationalExpressionContext *ctx) = 0;
  virtual void exitRelationalExpression(AnyFXParser::RelationalExpressionContext *ctx) = 0;

  virtual void enterShiftExpression(AnyFXParser::ShiftExpressionContext *ctx) = 0;
  virtual void exitShiftExpression(AnyFXParser::ShiftExpressionContext *ctx) = 0;

  virtual void enterAddSubtractExpression(AnyFXParser::AddSubtractExpressionContext *ctx) = 0;
  virtual void exitAddSubtractExpression(AnyFXParser::AddSubtractExpressionContext *ctx) = 0;

  virtual void enterMultiplyDivideExpression(AnyFXParser::MultiplyDivideExpressionContext *ctx) = 0;
  virtual void exitMultiplyDivideExpression(AnyFXParser::MultiplyDivideExpressionContext *ctx) = 0;

  virtual void enterPrefixExpression(AnyFXParser::PrefixExpressionContext *ctx) = 0;
  virtual void exitPrefixExpression(AnyFXParser::PrefixExpressionContext *ctx) = 0;

  virtual void enterSuffixExpression(AnyFXParser::SuffixExpressionContext *ctx) = 0;
  virtual void exitSuffixExpression(AnyFXParser::SuffixExpressionContext *ctx) = 0;

  virtual void enterNamespaceExpression(AnyFXParser::NamespaceExpressionContext *ctx) = 0;
  virtual void exitNamespaceExpression(AnyFXParser::NamespaceExpressionContext *ctx) = 0;

  virtual void enterBinaryexpatom(AnyFXParser::BinaryexpatomContext *ctx) = 0;
  virtual void exitBinaryexpatom(AnyFXParser::BinaryexpatomContext *ctx) = 0;

  virtual void enterInitializerExpression(AnyFXParser::InitializerExpressionContext *ctx) = 0;
  virtual void exitInitializerExpression(AnyFXParser::InitializerExpressionContext *ctx) = 0;


};

