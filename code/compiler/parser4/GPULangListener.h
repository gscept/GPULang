
// Generated from antlr4/grammar/GPULang.g4 by ANTLR 4.13.2

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

  virtual void enterPath(GPULangParser::PathContext *ctx) = 0;
  virtual void exitPath(GPULangParser::PathContext *ctx) = 0;

  virtual void enterBoolean(GPULangParser::BooleanContext *ctx) = 0;
  virtual void exitBoolean(GPULangParser::BooleanContext *ctx) = 0;

  virtual void enterEntry(GPULangParser::EntryContext *ctx) = 0;
  virtual void exitEntry(GPULangParser::EntryContext *ctx) = 0;

  virtual void enterEffect(GPULangParser::EffectContext *ctx) = 0;
  virtual void exitEffect(GPULangParser::EffectContext *ctx) = 0;

  virtual void enterLinePreprocessorEntry(GPULangParser::LinePreprocessorEntryContext *ctx) = 0;
  virtual void exitLinePreprocessorEntry(GPULangParser::LinePreprocessorEntryContext *ctx) = 0;

  virtual void enterAlias(GPULangParser::AliasContext *ctx) = 0;
  virtual void exitAlias(GPULangParser::AliasContext *ctx) = 0;

  virtual void enterAnnotation(GPULangParser::AnnotationContext *ctx) = 0;
  virtual void exitAnnotation(GPULangParser::AnnotationContext *ctx) = 0;

  virtual void enterAttribute(GPULangParser::AttributeContext *ctx) = 0;
  virtual void exitAttribute(GPULangParser::AttributeContext *ctx) = 0;

  virtual void enterTypeDeclaration(GPULangParser::TypeDeclarationContext *ctx) = 0;
  virtual void exitTypeDeclaration(GPULangParser::TypeDeclarationContext *ctx) = 0;

  virtual void enterGenerate(GPULangParser::GenerateContext *ctx) = 0;
  virtual void exitGenerate(GPULangParser::GenerateContext *ctx) = 0;

  virtual void enterGen_statement(GPULangParser::Gen_statementContext *ctx) = 0;
  virtual void exitGen_statement(GPULangParser::Gen_statementContext *ctx) = 0;

  virtual void enterGen_scope_statement(GPULangParser::Gen_scope_statementContext *ctx) = 0;
  virtual void exitGen_scope_statement(GPULangParser::Gen_scope_statementContext *ctx) = 0;

  virtual void enterGen_if_statement(GPULangParser::Gen_if_statementContext *ctx) = 0;
  virtual void exitGen_if_statement(GPULangParser::Gen_if_statementContext *ctx) = 0;

  virtual void enterVariables(GPULangParser::VariablesContext *ctx) = 0;
  virtual void exitVariables(GPULangParser::VariablesContext *ctx) = 0;

  virtual void enterStructureDeclaration(GPULangParser::StructureDeclarationContext *ctx) = 0;
  virtual void exitStructureDeclaration(GPULangParser::StructureDeclarationContext *ctx) = 0;

  virtual void enterStructure(GPULangParser::StructureContext *ctx) = 0;
  virtual void exitStructure(GPULangParser::StructureContext *ctx) = 0;

  virtual void enterEnumeration(GPULangParser::EnumerationContext *ctx) = 0;
  virtual void exitEnumeration(GPULangParser::EnumerationContext *ctx) = 0;

  virtual void enterParameter(GPULangParser::ParameterContext *ctx) = 0;
  virtual void exitParameter(GPULangParser::ParameterContext *ctx) = 0;

  virtual void enterFunctionDeclaration(GPULangParser::FunctionDeclarationContext *ctx) = 0;
  virtual void exitFunctionDeclaration(GPULangParser::FunctionDeclarationContext *ctx) = 0;

  virtual void enterFunction(GPULangParser::FunctionContext *ctx) = 0;
  virtual void exitFunction(GPULangParser::FunctionContext *ctx) = 0;

  virtual void enterProgram(GPULangParser::ProgramContext *ctx) = 0;
  virtual void exitProgram(GPULangParser::ProgramContext *ctx) = 0;

  virtual void enterSampler(GPULangParser::SamplerContext *ctx) = 0;
  virtual void exitSampler(GPULangParser::SamplerContext *ctx) = 0;

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

  virtual void enterForUniformValueStatement(GPULangParser::ForUniformValueStatementContext *ctx) = 0;
  virtual void exitForUniformValueStatement(GPULangParser::ForUniformValueStatementContext *ctx) = 0;

  virtual void enterWhileStatement(GPULangParser::WhileStatementContext *ctx) = 0;
  virtual void exitWhileStatement(GPULangParser::WhileStatementContext *ctx) = 0;

  virtual void enterScopeStatement(GPULangParser::ScopeStatementContext *ctx) = 0;
  virtual void exitScopeStatement(GPULangParser::ScopeStatementContext *ctx) = 0;

  virtual void enterTerminateStatement(GPULangParser::TerminateStatementContext *ctx) = 0;
  virtual void exitTerminateStatement(GPULangParser::TerminateStatementContext *ctx) = 0;

  virtual void enterContinueStatement(GPULangParser::ContinueStatementContext *ctx) = 0;
  virtual void exitContinueStatement(GPULangParser::ContinueStatementContext *ctx) = 0;

  virtual void enterSwitchStatement(GPULangParser::SwitchStatementContext *ctx) = 0;
  virtual void exitSwitchStatement(GPULangParser::SwitchStatementContext *ctx) = 0;

  virtual void enterBreakStatement(GPULangParser::BreakStatementContext *ctx) = 0;
  virtual void exitBreakStatement(GPULangParser::BreakStatementContext *ctx) = 0;

  virtual void enterExpressionList(GPULangParser::ExpressionListContext *ctx) = 0;
  virtual void exitExpressionList(GPULangParser::ExpressionListContext *ctx) = 0;

  virtual void enterExpression(GPULangParser::ExpressionContext *ctx) = 0;
  virtual void exitExpression(GPULangParser::ExpressionContext *ctx) = 0;

  virtual void enterBinaryexpatom(GPULangParser::BinaryexpatomContext *ctx) = 0;
  virtual void exitBinaryexpatom(GPULangParser::BinaryexpatomContext *ctx) = 0;

  virtual void enterInitializerExpression(GPULangParser::InitializerExpressionContext *ctx) = 0;
  virtual void exitInitializerExpression(GPULangParser::InitializerExpressionContext *ctx) = 0;

  virtual void enterArrayInitializerExpression(GPULangParser::ArrayInitializerExpressionContext *ctx) = 0;
  virtual void exitArrayInitializerExpression(GPULangParser::ArrayInitializerExpressionContext *ctx) = 0;

  virtual void enterFloatVecLiteralExpression(GPULangParser::FloatVecLiteralExpressionContext *ctx) = 0;
  virtual void exitFloatVecLiteralExpression(GPULangParser::FloatVecLiteralExpressionContext *ctx) = 0;

  virtual void enterDoubleVecLiteralExpression(GPULangParser::DoubleVecLiteralExpressionContext *ctx) = 0;
  virtual void exitDoubleVecLiteralExpression(GPULangParser::DoubleVecLiteralExpressionContext *ctx) = 0;

  virtual void enterIntVecLiteralExpression(GPULangParser::IntVecLiteralExpressionContext *ctx) = 0;
  virtual void exitIntVecLiteralExpression(GPULangParser::IntVecLiteralExpressionContext *ctx) = 0;

  virtual void enterUintVecLiteralExpression(GPULangParser::UintVecLiteralExpressionContext *ctx) = 0;
  virtual void exitUintVecLiteralExpression(GPULangParser::UintVecLiteralExpressionContext *ctx) = 0;

  virtual void enterBooleanVecLiteralExpression(GPULangParser::BooleanVecLiteralExpressionContext *ctx) = 0;
  virtual void exitBooleanVecLiteralExpression(GPULangParser::BooleanVecLiteralExpressionContext *ctx) = 0;


};

