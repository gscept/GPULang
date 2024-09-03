

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
#include "AnyFXListener.h"


/**
 * This class provides an empty implementation of AnyFXListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  AnyFXBaseListener : public AnyFXListener {
public:

  virtual void enterString(AnyFXParser::StringContext * /*ctx*/) override { }
  virtual void exitString(AnyFXParser::StringContext * /*ctx*/) override { }

  virtual void enterBoolean(AnyFXParser::BooleanContext * /*ctx*/) override { }
  virtual void exitBoolean(AnyFXParser::BooleanContext * /*ctx*/) override { }

  virtual void enterPreprocess(AnyFXParser::PreprocessContext * /*ctx*/) override { }
  virtual void exitPreprocess(AnyFXParser::PreprocessContext * /*ctx*/) override { }

  virtual void enterEntry(AnyFXParser::EntryContext * /*ctx*/) override { }
  virtual void exitEntry(AnyFXParser::EntryContext * /*ctx*/) override { }

  virtual void enterEffect(AnyFXParser::EffectContext * /*ctx*/) override { }
  virtual void exitEffect(AnyFXParser::EffectContext * /*ctx*/) override { }

  virtual void enterAlias(AnyFXParser::AliasContext * /*ctx*/) override { }
  virtual void exitAlias(AnyFXParser::AliasContext * /*ctx*/) override { }

  virtual void enterAnnotation(AnyFXParser::AnnotationContext * /*ctx*/) override { }
  virtual void exitAnnotation(AnyFXParser::AnnotationContext * /*ctx*/) override { }

  virtual void enterAttribute(AnyFXParser::AttributeContext * /*ctx*/) override { }
  virtual void exitAttribute(AnyFXParser::AttributeContext * /*ctx*/) override { }

  virtual void enterTypeDeclaration(AnyFXParser::TypeDeclarationContext * /*ctx*/) override { }
  virtual void exitTypeDeclaration(AnyFXParser::TypeDeclarationContext * /*ctx*/) override { }

  virtual void enterVariables(AnyFXParser::VariablesContext * /*ctx*/) override { }
  virtual void exitVariables(AnyFXParser::VariablesContext * /*ctx*/) override { }

  virtual void enterVariable(AnyFXParser::VariableContext * /*ctx*/) override { }
  virtual void exitVariable(AnyFXParser::VariableContext * /*ctx*/) override { }

  virtual void enterStructureDeclaration(AnyFXParser::StructureDeclarationContext * /*ctx*/) override { }
  virtual void exitStructureDeclaration(AnyFXParser::StructureDeclarationContext * /*ctx*/) override { }

  virtual void enterStructure(AnyFXParser::StructureContext * /*ctx*/) override { }
  virtual void exitStructure(AnyFXParser::StructureContext * /*ctx*/) override { }

  virtual void enterEnumeration(AnyFXParser::EnumerationContext * /*ctx*/) override { }
  virtual void exitEnumeration(AnyFXParser::EnumerationContext * /*ctx*/) override { }

  virtual void enterFunctionDeclaration(AnyFXParser::FunctionDeclarationContext * /*ctx*/) override { }
  virtual void exitFunctionDeclaration(AnyFXParser::FunctionDeclarationContext * /*ctx*/) override { }

  virtual void enterCodeblock(AnyFXParser::CodeblockContext * /*ctx*/) override { }
  virtual void exitCodeblock(AnyFXParser::CodeblockContext * /*ctx*/) override { }

  virtual void enterFunction(AnyFXParser::FunctionContext * /*ctx*/) override { }
  virtual void exitFunction(AnyFXParser::FunctionContext * /*ctx*/) override { }

  virtual void enterProgram(AnyFXParser::ProgramContext * /*ctx*/) override { }
  virtual void exitProgram(AnyFXParser::ProgramContext * /*ctx*/) override { }

  virtual void enterState(AnyFXParser::StateContext * /*ctx*/) override { }
  virtual void exitState(AnyFXParser::StateContext * /*ctx*/) override { }

  virtual void enterStatement(AnyFXParser::StatementContext * /*ctx*/) override { }
  virtual void exitStatement(AnyFXParser::StatementContext * /*ctx*/) override { }

  virtual void enterExpressionStatement(AnyFXParser::ExpressionStatementContext * /*ctx*/) override { }
  virtual void exitExpressionStatement(AnyFXParser::ExpressionStatementContext * /*ctx*/) override { }

  virtual void enterIfStatement(AnyFXParser::IfStatementContext * /*ctx*/) override { }
  virtual void exitIfStatement(AnyFXParser::IfStatementContext * /*ctx*/) override { }

  virtual void enterForStatement(AnyFXParser::ForStatementContext * /*ctx*/) override { }
  virtual void exitForStatement(AnyFXParser::ForStatementContext * /*ctx*/) override { }

  virtual void enterForRangeStatement(AnyFXParser::ForRangeStatementContext * /*ctx*/) override { }
  virtual void exitForRangeStatement(AnyFXParser::ForRangeStatementContext * /*ctx*/) override { }

  virtual void enterWhileStatement(AnyFXParser::WhileStatementContext * /*ctx*/) override { }
  virtual void exitWhileStatement(AnyFXParser::WhileStatementContext * /*ctx*/) override { }

  virtual void enterScopeStatement(AnyFXParser::ScopeStatementContext * /*ctx*/) override { }
  virtual void exitScopeStatement(AnyFXParser::ScopeStatementContext * /*ctx*/) override { }

  virtual void enterReturnStatement(AnyFXParser::ReturnStatementContext * /*ctx*/) override { }
  virtual void exitReturnStatement(AnyFXParser::ReturnStatementContext * /*ctx*/) override { }

  virtual void enterContinueStatement(AnyFXParser::ContinueStatementContext * /*ctx*/) override { }
  virtual void exitContinueStatement(AnyFXParser::ContinueStatementContext * /*ctx*/) override { }

  virtual void enterSwitchStatement(AnyFXParser::SwitchStatementContext * /*ctx*/) override { }
  virtual void exitSwitchStatement(AnyFXParser::SwitchStatementContext * /*ctx*/) override { }

  virtual void enterBreakStatement(AnyFXParser::BreakStatementContext * /*ctx*/) override { }
  virtual void exitBreakStatement(AnyFXParser::BreakStatementContext * /*ctx*/) override { }

  virtual void enterExpression(AnyFXParser::ExpressionContext * /*ctx*/) override { }
  virtual void exitExpression(AnyFXParser::ExpressionContext * /*ctx*/) override { }

  virtual void enterCommaExpression(AnyFXParser::CommaExpressionContext * /*ctx*/) override { }
  virtual void exitCommaExpression(AnyFXParser::CommaExpressionContext * /*ctx*/) override { }

  virtual void enterAssignmentExpression(AnyFXParser::AssignmentExpressionContext * /*ctx*/) override { }
  virtual void exitAssignmentExpression(AnyFXParser::AssignmentExpressionContext * /*ctx*/) override { }

  virtual void enterLogicalOrExpression(AnyFXParser::LogicalOrExpressionContext * /*ctx*/) override { }
  virtual void exitLogicalOrExpression(AnyFXParser::LogicalOrExpressionContext * /*ctx*/) override { }

  virtual void enterLogicalAndExpression(AnyFXParser::LogicalAndExpressionContext * /*ctx*/) override { }
  virtual void exitLogicalAndExpression(AnyFXParser::LogicalAndExpressionContext * /*ctx*/) override { }

  virtual void enterOrExpression(AnyFXParser::OrExpressionContext * /*ctx*/) override { }
  virtual void exitOrExpression(AnyFXParser::OrExpressionContext * /*ctx*/) override { }

  virtual void enterXorExpression(AnyFXParser::XorExpressionContext * /*ctx*/) override { }
  virtual void exitXorExpression(AnyFXParser::XorExpressionContext * /*ctx*/) override { }

  virtual void enterAndExpression(AnyFXParser::AndExpressionContext * /*ctx*/) override { }
  virtual void exitAndExpression(AnyFXParser::AndExpressionContext * /*ctx*/) override { }

  virtual void enterEquivalencyExpression(AnyFXParser::EquivalencyExpressionContext * /*ctx*/) override { }
  virtual void exitEquivalencyExpression(AnyFXParser::EquivalencyExpressionContext * /*ctx*/) override { }

  virtual void enterRelationalExpression(AnyFXParser::RelationalExpressionContext * /*ctx*/) override { }
  virtual void exitRelationalExpression(AnyFXParser::RelationalExpressionContext * /*ctx*/) override { }

  virtual void enterShiftExpression(AnyFXParser::ShiftExpressionContext * /*ctx*/) override { }
  virtual void exitShiftExpression(AnyFXParser::ShiftExpressionContext * /*ctx*/) override { }

  virtual void enterAddSubtractExpression(AnyFXParser::AddSubtractExpressionContext * /*ctx*/) override { }
  virtual void exitAddSubtractExpression(AnyFXParser::AddSubtractExpressionContext * /*ctx*/) override { }

  virtual void enterMultiplyDivideExpression(AnyFXParser::MultiplyDivideExpressionContext * /*ctx*/) override { }
  virtual void exitMultiplyDivideExpression(AnyFXParser::MultiplyDivideExpressionContext * /*ctx*/) override { }

  virtual void enterPrefixExpression(AnyFXParser::PrefixExpressionContext * /*ctx*/) override { }
  virtual void exitPrefixExpression(AnyFXParser::PrefixExpressionContext * /*ctx*/) override { }

  virtual void enterSuffixExpression(AnyFXParser::SuffixExpressionContext * /*ctx*/) override { }
  virtual void exitSuffixExpression(AnyFXParser::SuffixExpressionContext * /*ctx*/) override { }

  virtual void enterNamespaceExpression(AnyFXParser::NamespaceExpressionContext * /*ctx*/) override { }
  virtual void exitNamespaceExpression(AnyFXParser::NamespaceExpressionContext * /*ctx*/) override { }

  virtual void enterBinaryexpatom(AnyFXParser::BinaryexpatomContext * /*ctx*/) override { }
  virtual void exitBinaryexpatom(AnyFXParser::BinaryexpatomContext * /*ctx*/) override { }

  virtual void enterInitializerExpression(AnyFXParser::InitializerExpressionContext * /*ctx*/) override { }
  virtual void exitInitializerExpression(AnyFXParser::InitializerExpressionContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

