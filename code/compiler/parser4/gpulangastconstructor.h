
// Implement visitor
#include <memory>
#include "GPULangBaseVisitor.h"
class GPULangASTConstructor : public GPULangBaseVisitor
{
    virtual std::any visitString(GPULangParser::StringContext* ctx) override {
        const std::string& str = ctx->getText();
        return str.substr(1, str.length() - 2);
    }

    virtual std::any visitPath(GPULangParser::PathContext* ctx) override {
        const std::string& str = ctx->getText();
        return str.substr(1, str.length() - 2);
    }

    virtual std::any visitBoolean(GPULangParser::BooleanContext* ctx) override {
        return ctx->getText() == "true" ? true : false;
    }

    virtual std::any visitEntry(GPULangParser::EntryContext* ctx) override {
        return visitChildren(ctx);
    }

    virtual std::any visitEffect(GPULangParser::EffectContext* ctx) override {
        auto effect = Alloc<Effect>();
        auto res = visitChildren(ctx);
        return effect;
    }

    virtual std::any visitLinePreprocessorEntry(GPULangParser::LinePreprocessorEntryContext* ctx) override {
        return visitChildren(ctx);
    }

    virtual std::any visitAlias(GPULangParser::AliasContext* ctx) override {
        auto alias = Alloc<Alias>();
        alias->name = ctx->name->getText();
        alias->type = ctx->type->getText();
        return alias;
    }

    virtual std::any visitAnnotation(GPULangParser::AnnotationContext* ctx) override {
        auto annotation = Alloc<Annotation>();
        annotation->name = ctx->name->getText();
        annotation->value = ctx->value->tree;
        return annotation;
    }

    virtual std::any visitAttribute(GPULangParser::AttributeContext* ctx) override {
        auto attribute = Alloc<Attribute>();
        attribute->name = ctx->name->getText();
        attribute->expression = ctx->e->tree;
        return attribute;
    }

    virtual std::any visitTypeDeclaration(GPULangParser::TypeDeclarationContext* ctx) override {
        return ctx->type;
    }

    virtual std::any visitVariables(GPULangParser::VariablesContext* ctx) override {
        return ctx->vars;
    }

    virtual std::any visitStructureDeclaration(GPULangParser::StructureDeclarationContext* ctx) override {
        return visitChildren(ctx);
    }

    virtual std::any visitStructure(GPULangParser::StructureContext* ctx) override {
        return visitChildren(ctx);
    }

    virtual std::any visitEnumeration(GPULangParser::EnumerationContext* ctx) override {
        return visitChildren(ctx);
    }

    virtual std::any visitParameter(GPULangParser::ParameterContext* ctx) override {
        return visitChildren(ctx);
    }

    virtual std::any visitFunctionDeclaration(GPULangParser::FunctionDeclarationContext* ctx) override {
        return visitChildren(ctx);
    }

    virtual std::any visitFunction(GPULangParser::FunctionContext* ctx) override {
        return visitChildren(ctx);
    }

    virtual std::any visitProgram(GPULangParser::ProgramContext* ctx) override {
        return visitChildren(ctx);
    }

    virtual std::any visitSampler(GPULangParser::SamplerContext* ctx) override {
        return visitChildren(ctx);
    }

    virtual std::any visitState(GPULangParser::StateContext* ctx) override {
        return visitChildren(ctx);
    }

    virtual std::any visitStatement(GPULangParser::StatementContext* ctx) override {
        return visitChildren(ctx);
    }

    virtual std::any visitExpressionStatement(GPULangParser::ExpressionStatementContext* ctx) override {
        return visitChildren(ctx);
    }

    virtual std::any visitIfStatement(GPULangParser::IfStatementContext* ctx) override {
        return visitChildren(ctx);
    }

    virtual std::any visitForStatement(GPULangParser::ForStatementContext* ctx) override {
        return visitChildren(ctx);
    }

    virtual std::any visitForRangeStatement(GPULangParser::ForRangeStatementContext* ctx) override {
        return visitChildren(ctx);
    }

    virtual std::any visitForUniformValueStatement(GPULangParser::ForUniformValueStatementContext* ctx) override {
        return visitChildren(ctx);
    }

    virtual std::any visitWhileStatement(GPULangParser::WhileStatementContext* ctx) override {
        return visitChildren(ctx);
    }

    virtual std::any visitScopeStatement(GPULangParser::ScopeStatementContext* ctx) override {
        return visitChildren(ctx);
    }

    virtual std::any visitTerminateStatement(GPULangParser::TerminateStatementContext* ctx) override {
        return visitChildren(ctx);
    }

    virtual std::any visitContinueStatement(GPULangParser::ContinueStatementContext* ctx) override {
        return visitChildren(ctx);
    }

    virtual std::any visitSwitchStatement(GPULangParser::SwitchStatementContext* ctx) override {
        return visitChildren(ctx);
    }

    virtual std::any visitBreakStatement(GPULangParser::BreakStatementContext* ctx) override {
        return visitChildren(ctx);
    }

    virtual std::any visitExpressionNoComma(GPULangParser::ExpressionNoCommaContext* ctx) override {
        return visitChildren(ctx);
    }

    virtual std::any visitExpression(GPULangParser::ExpressionContext* ctx) override {
        return visitChildren(ctx);
    }

    virtual std::any visitBinaryexpatom(GPULangParser::BinaryexpatomContext* ctx) override {
        return visitChildren(ctx);
    }

    virtual std::any visitInitializerExpression(GPULangParser::InitializerExpressionContext* ctx) override {
        return visitChildren(ctx);
    }

    virtual std::any visitArrayInitializerExpression(GPULangParser::ArrayInitializerExpressionContext* ctx) override {
        return visitChildren(ctx);
    }

    virtual std::any visitFloatVecLiteralExpression(GPULangParser::FloatVecLiteralExpressionContext* ctx) override {
        return visitChildren(ctx);
    }

    virtual std::any visitDoubleVecLiteralExpression(GPULangParser::DoubleVecLiteralExpressionContext* ctx) override {
        return visitChildren(ctx);
    }

    virtual std::any visitIntVecLiteralExpression(GPULangParser::IntVecLiteralExpressionContext* ctx) override {
        return visitChildren(ctx);
    }

    virtual std::any visitUintVecLiteralExpression(GPULangParser::UintVecLiteralExpressionContext* ctx) override {
        return visitChildren(ctx);
    }

    virtual std::any visitBooleanVecLiteralExpression(GPULangParser::BooleanVecLiteralExpressionContext* ctx) override {
        return visitChildren(ctx);
    }

};