
// Generated from antlr4/grammar/GPULang.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "GPULangVisitor.h"


/**
 * This class provides an empty implementation of GPULangVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  GPULangBaseVisitor : public GPULangVisitor {
public:

  virtual std::any visitString(GPULangParser::StringContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPath(GPULangParser::PathContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBoolean(GPULangParser::BooleanContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEntry(GPULangParser::EntryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEffect(GPULangParser::EffectContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLinePreprocessorEntry(GPULangParser::LinePreprocessorEntryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAlias(GPULangParser::AliasContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAnnotation(GPULangParser::AnnotationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAttribute(GPULangParser::AttributeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTypeDeclaration(GPULangParser::TypeDeclarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVariables(GPULangParser::VariablesContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStructureDeclaration(GPULangParser::StructureDeclarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStructure(GPULangParser::StructureContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEnumeration(GPULangParser::EnumerationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitParameter(GPULangParser::ParameterContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFunctionDeclaration(GPULangParser::FunctionDeclarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFunction(GPULangParser::FunctionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitProgram(GPULangParser::ProgramContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSampler(GPULangParser::SamplerContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitState(GPULangParser::StateContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStatement(GPULangParser::StatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpressionStatement(GPULangParser::ExpressionStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIfStatement(GPULangParser::IfStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitForStatement(GPULangParser::ForStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitForRangeStatement(GPULangParser::ForRangeStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitForUniformValueStatement(GPULangParser::ForUniformValueStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitWhileStatement(GPULangParser::WhileStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitScopeStatement(GPULangParser::ScopeStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTerminateStatement(GPULangParser::TerminateStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitContinueStatement(GPULangParser::ContinueStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSwitchStatement(GPULangParser::SwitchStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBreakStatement(GPULangParser::BreakStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpressionNoComma(GPULangParser::ExpressionNoCommaContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpression(GPULangParser::ExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBinaryexpatom(GPULangParser::BinaryexpatomContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInitializerExpression(GPULangParser::InitializerExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitArrayInitializerExpression(GPULangParser::ArrayInitializerExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFloatVecLiteralExpression(GPULangParser::FloatVecLiteralExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDoubleVecLiteralExpression(GPULangParser::DoubleVecLiteralExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIntVecLiteralExpression(GPULangParser::IntVecLiteralExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUintVecLiteralExpression(GPULangParser::UintVecLiteralExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBooleanVecLiteralExpression(GPULangParser::BooleanVecLiteralExpressionContext *ctx) override {
    return visitChildren(ctx);
  }


};

