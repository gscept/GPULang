
// Generated from antlr4/grammar/GPULang.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "GPULangParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by GPULangParser.
 */
class  GPULangVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by GPULangParser.
   */
    virtual std::any visitString(GPULangParser::StringContext *context) = 0;

    virtual std::any visitPath(GPULangParser::PathContext *context) = 0;

    virtual std::any visitBoolean(GPULangParser::BooleanContext *context) = 0;

    virtual std::any visitEntry(GPULangParser::EntryContext *context) = 0;

    virtual std::any visitEffect(GPULangParser::EffectContext *context) = 0;

    virtual std::any visitLinePreprocessorEntry(GPULangParser::LinePreprocessorEntryContext *context) = 0;

    virtual std::any visitAlias(GPULangParser::AliasContext *context) = 0;

    virtual std::any visitAnnotation(GPULangParser::AnnotationContext *context) = 0;

    virtual std::any visitAttribute(GPULangParser::AttributeContext *context) = 0;

    virtual std::any visitTypeDeclaration(GPULangParser::TypeDeclarationContext *context) = 0;

    virtual std::any visitVariables(GPULangParser::VariablesContext *context) = 0;

    virtual std::any visitStructureDeclaration(GPULangParser::StructureDeclarationContext *context) = 0;

    virtual std::any visitStructure(GPULangParser::StructureContext *context) = 0;

    virtual std::any visitEnumeration(GPULangParser::EnumerationContext *context) = 0;

    virtual std::any visitParameter(GPULangParser::ParameterContext *context) = 0;

    virtual std::any visitFunctionDeclaration(GPULangParser::FunctionDeclarationContext *context) = 0;

    virtual std::any visitFunction(GPULangParser::FunctionContext *context) = 0;

    virtual std::any visitProgram(GPULangParser::ProgramContext *context) = 0;

    virtual std::any visitSampler(GPULangParser::SamplerContext *context) = 0;

    virtual std::any visitState(GPULangParser::StateContext *context) = 0;

    virtual std::any visitStatement(GPULangParser::StatementContext *context) = 0;

    virtual std::any visitExpressionStatement(GPULangParser::ExpressionStatementContext *context) = 0;

    virtual std::any visitIfStatement(GPULangParser::IfStatementContext *context) = 0;

    virtual std::any visitForStatement(GPULangParser::ForStatementContext *context) = 0;

    virtual std::any visitForRangeStatement(GPULangParser::ForRangeStatementContext *context) = 0;

    virtual std::any visitForUniformValueStatement(GPULangParser::ForUniformValueStatementContext *context) = 0;

    virtual std::any visitWhileStatement(GPULangParser::WhileStatementContext *context) = 0;

    virtual std::any visitScopeStatement(GPULangParser::ScopeStatementContext *context) = 0;

    virtual std::any visitTerminateStatement(GPULangParser::TerminateStatementContext *context) = 0;

    virtual std::any visitContinueStatement(GPULangParser::ContinueStatementContext *context) = 0;

    virtual std::any visitSwitchStatement(GPULangParser::SwitchStatementContext *context) = 0;

    virtual std::any visitBreakStatement(GPULangParser::BreakStatementContext *context) = 0;

    virtual std::any visitExpressionNoComma(GPULangParser::ExpressionNoCommaContext *context) = 0;

    virtual std::any visitExpression(GPULangParser::ExpressionContext *context) = 0;

    virtual std::any visitBinaryexpatom(GPULangParser::BinaryexpatomContext *context) = 0;

    virtual std::any visitInitializerExpression(GPULangParser::InitializerExpressionContext *context) = 0;

    virtual std::any visitArrayInitializerExpression(GPULangParser::ArrayInitializerExpressionContext *context) = 0;

    virtual std::any visitFloatVecLiteralExpression(GPULangParser::FloatVecLiteralExpressionContext *context) = 0;

    virtual std::any visitDoubleVecLiteralExpression(GPULangParser::DoubleVecLiteralExpressionContext *context) = 0;

    virtual std::any visitIntVecLiteralExpression(GPULangParser::IntVecLiteralExpressionContext *context) = 0;

    virtual std::any visitUintVecLiteralExpression(GPULangParser::UintVecLiteralExpressionContext *context) = 0;

    virtual std::any visitBooleanVecLiteralExpression(GPULangParser::BooleanVecLiteralExpressionContext *context) = 0;


};

