
// Generated from antlr4/grammar/GPULang.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "GPULangListener.h"


/**
 * This class provides an empty implementation of GPULangListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  GPULangBaseListener : public GPULangListener {
public:

  virtual void enterString(GPULangParser::StringContext * /*ctx*/) override { }
  virtual void exitString(GPULangParser::StringContext * /*ctx*/) override { }

  virtual void enterBoolean(GPULangParser::BooleanContext * /*ctx*/) override { }
  virtual void exitBoolean(GPULangParser::BooleanContext * /*ctx*/) override { }

  virtual void enterEntry(GPULangParser::EntryContext * /*ctx*/) override { }
  virtual void exitEntry(GPULangParser::EntryContext * /*ctx*/) override { }

  virtual void enterEffect(GPULangParser::EffectContext * /*ctx*/) override { }
  virtual void exitEffect(GPULangParser::EffectContext * /*ctx*/) override { }

  virtual void enterLinePreprocessorEntry(GPULangParser::LinePreprocessorEntryContext * /*ctx*/) override { }
  virtual void exitLinePreprocessorEntry(GPULangParser::LinePreprocessorEntryContext * /*ctx*/) override { }

  virtual void enterAlias(GPULangParser::AliasContext * /*ctx*/) override { }
  virtual void exitAlias(GPULangParser::AliasContext * /*ctx*/) override { }

  virtual void enterAnnotation(GPULangParser::AnnotationContext * /*ctx*/) override { }
  virtual void exitAnnotation(GPULangParser::AnnotationContext * /*ctx*/) override { }

  virtual void enterAttribute(GPULangParser::AttributeContext * /*ctx*/) override { }
  virtual void exitAttribute(GPULangParser::AttributeContext * /*ctx*/) override { }

  virtual void enterTypeDeclaration(GPULangParser::TypeDeclarationContext * /*ctx*/) override { }
  virtual void exitTypeDeclaration(GPULangParser::TypeDeclarationContext * /*ctx*/) override { }

  virtual void enterVariables(GPULangParser::VariablesContext * /*ctx*/) override { }
  virtual void exitVariables(GPULangParser::VariablesContext * /*ctx*/) override { }

  virtual void enterStructureDeclaration(GPULangParser::StructureDeclarationContext * /*ctx*/) override { }
  virtual void exitStructureDeclaration(GPULangParser::StructureDeclarationContext * /*ctx*/) override { }

  virtual void enterStructure(GPULangParser::StructureContext * /*ctx*/) override { }
  virtual void exitStructure(GPULangParser::StructureContext * /*ctx*/) override { }

  virtual void enterEnumeration(GPULangParser::EnumerationContext * /*ctx*/) override { }
  virtual void exitEnumeration(GPULangParser::EnumerationContext * /*ctx*/) override { }

  virtual void enterParameter(GPULangParser::ParameterContext * /*ctx*/) override { }
  virtual void exitParameter(GPULangParser::ParameterContext * /*ctx*/) override { }

  virtual void enterFunctionDeclaration(GPULangParser::FunctionDeclarationContext * /*ctx*/) override { }
  virtual void exitFunctionDeclaration(GPULangParser::FunctionDeclarationContext * /*ctx*/) override { }

  virtual void enterCodeblock(GPULangParser::CodeblockContext * /*ctx*/) override { }
  virtual void exitCodeblock(GPULangParser::CodeblockContext * /*ctx*/) override { }

  virtual void enterFunction(GPULangParser::FunctionContext * /*ctx*/) override { }
  virtual void exitFunction(GPULangParser::FunctionContext * /*ctx*/) override { }

  virtual void enterProgram(GPULangParser::ProgramContext * /*ctx*/) override { }
  virtual void exitProgram(GPULangParser::ProgramContext * /*ctx*/) override { }

  virtual void enterSampler(GPULangParser::SamplerContext * /*ctx*/) override { }
  virtual void exitSampler(GPULangParser::SamplerContext * /*ctx*/) override { }

  virtual void enterState(GPULangParser::StateContext * /*ctx*/) override { }
  virtual void exitState(GPULangParser::StateContext * /*ctx*/) override { }

  virtual void enterStatement(GPULangParser::StatementContext * /*ctx*/) override { }
  virtual void exitStatement(GPULangParser::StatementContext * /*ctx*/) override { }

  virtual void enterExpressionStatement(GPULangParser::ExpressionStatementContext * /*ctx*/) override { }
  virtual void exitExpressionStatement(GPULangParser::ExpressionStatementContext * /*ctx*/) override { }

  virtual void enterIfStatement(GPULangParser::IfStatementContext * /*ctx*/) override { }
  virtual void exitIfStatement(GPULangParser::IfStatementContext * /*ctx*/) override { }

  virtual void enterForStatement(GPULangParser::ForStatementContext * /*ctx*/) override { }
  virtual void exitForStatement(GPULangParser::ForStatementContext * /*ctx*/) override { }

  virtual void enterForRangeStatement(GPULangParser::ForRangeStatementContext * /*ctx*/) override { }
  virtual void exitForRangeStatement(GPULangParser::ForRangeStatementContext * /*ctx*/) override { }

  virtual void enterWhileStatement(GPULangParser::WhileStatementContext * /*ctx*/) override { }
  virtual void exitWhileStatement(GPULangParser::WhileStatementContext * /*ctx*/) override { }

  virtual void enterScopeStatement(GPULangParser::ScopeStatementContext * /*ctx*/) override { }
  virtual void exitScopeStatement(GPULangParser::ScopeStatementContext * /*ctx*/) override { }

  virtual void enterReturnStatement(GPULangParser::ReturnStatementContext * /*ctx*/) override { }
  virtual void exitReturnStatement(GPULangParser::ReturnStatementContext * /*ctx*/) override { }

  virtual void enterDiscardStatement(GPULangParser::DiscardStatementContext * /*ctx*/) override { }
  virtual void exitDiscardStatement(GPULangParser::DiscardStatementContext * /*ctx*/) override { }

  virtual void enterContinueStatement(GPULangParser::ContinueStatementContext * /*ctx*/) override { }
  virtual void exitContinueStatement(GPULangParser::ContinueStatementContext * /*ctx*/) override { }

  virtual void enterSwitchStatement(GPULangParser::SwitchStatementContext * /*ctx*/) override { }
  virtual void exitSwitchStatement(GPULangParser::SwitchStatementContext * /*ctx*/) override { }

  virtual void enterBreakStatement(GPULangParser::BreakStatementContext * /*ctx*/) override { }
  virtual void exitBreakStatement(GPULangParser::BreakStatementContext * /*ctx*/) override { }

  virtual void enterExpression(GPULangParser::ExpressionContext * /*ctx*/) override { }
  virtual void exitExpression(GPULangParser::ExpressionContext * /*ctx*/) override { }

  virtual void enterCommaExpression(GPULangParser::CommaExpressionContext * /*ctx*/) override { }
  virtual void exitCommaExpression(GPULangParser::CommaExpressionContext * /*ctx*/) override { }

  virtual void enterAssignmentExpression(GPULangParser::AssignmentExpressionContext * /*ctx*/) override { }
  virtual void exitAssignmentExpression(GPULangParser::AssignmentExpressionContext * /*ctx*/) override { }

  virtual void enterLogicalOrExpression(GPULangParser::LogicalOrExpressionContext * /*ctx*/) override { }
  virtual void exitLogicalOrExpression(GPULangParser::LogicalOrExpressionContext * /*ctx*/) override { }

  virtual void enterLogicalAndExpression(GPULangParser::LogicalAndExpressionContext * /*ctx*/) override { }
  virtual void exitLogicalAndExpression(GPULangParser::LogicalAndExpressionContext * /*ctx*/) override { }

  virtual void enterOrExpression(GPULangParser::OrExpressionContext * /*ctx*/) override { }
  virtual void exitOrExpression(GPULangParser::OrExpressionContext * /*ctx*/) override { }

  virtual void enterXorExpression(GPULangParser::XorExpressionContext * /*ctx*/) override { }
  virtual void exitXorExpression(GPULangParser::XorExpressionContext * /*ctx*/) override { }

  virtual void enterAndExpression(GPULangParser::AndExpressionContext * /*ctx*/) override { }
  virtual void exitAndExpression(GPULangParser::AndExpressionContext * /*ctx*/) override { }

  virtual void enterEquivalencyExpression(GPULangParser::EquivalencyExpressionContext * /*ctx*/) override { }
  virtual void exitEquivalencyExpression(GPULangParser::EquivalencyExpressionContext * /*ctx*/) override { }

  virtual void enterRelationalExpression(GPULangParser::RelationalExpressionContext * /*ctx*/) override { }
  virtual void exitRelationalExpression(GPULangParser::RelationalExpressionContext * /*ctx*/) override { }

  virtual void enterShiftExpression(GPULangParser::ShiftExpressionContext * /*ctx*/) override { }
  virtual void exitShiftExpression(GPULangParser::ShiftExpressionContext * /*ctx*/) override { }

  virtual void enterAddSubtractExpression(GPULangParser::AddSubtractExpressionContext * /*ctx*/) override { }
  virtual void exitAddSubtractExpression(GPULangParser::AddSubtractExpressionContext * /*ctx*/) override { }

  virtual void enterMultiplyDivideExpression(GPULangParser::MultiplyDivideExpressionContext * /*ctx*/) override { }
  virtual void exitMultiplyDivideExpression(GPULangParser::MultiplyDivideExpressionContext * /*ctx*/) override { }

  virtual void enterPrefixExpression(GPULangParser::PrefixExpressionContext * /*ctx*/) override { }
  virtual void exitPrefixExpression(GPULangParser::PrefixExpressionContext * /*ctx*/) override { }

  virtual void enterSuffixExpression(GPULangParser::SuffixExpressionContext * /*ctx*/) override { }
  virtual void exitSuffixExpression(GPULangParser::SuffixExpressionContext * /*ctx*/) override { }

  virtual void enterNamespaceExpression(GPULangParser::NamespaceExpressionContext * /*ctx*/) override { }
  virtual void exitNamespaceExpression(GPULangParser::NamespaceExpressionContext * /*ctx*/) override { }

  virtual void enterBinaryexpatom(GPULangParser::BinaryexpatomContext * /*ctx*/) override { }
  virtual void exitBinaryexpatom(GPULangParser::BinaryexpatomContext * /*ctx*/) override { }

  virtual void enterInitializerExpression(GPULangParser::InitializerExpressionContext * /*ctx*/) override { }
  virtual void exitInitializerExpression(GPULangParser::InitializerExpressionContext * /*ctx*/) override { }

  virtual void enterArrayInitializerExpression(GPULangParser::ArrayInitializerExpressionContext * /*ctx*/) override { }
  virtual void exitArrayInitializerExpression(GPULangParser::ArrayInitializerExpressionContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

