

// include std container
#include <vector>
#include <typeinfo>
#include <iostream>
#include <string>
#include <stack>
#include <tuple>

#include "anyfxtoken.h"
#include "v3/ast/alias.h"
#include "v3/ast/annotation.h"
#include "v3/ast/effect.h"
#include "v3/ast/function.h"
#include "v3/ast/program.h"
#include "v3/ast/renderstate.h"
#include "v3/ast/sampler.h"
#include "v3/ast/state.h"
#include "v3/ast/structure.h"
#include "v3/ast/resource.h"
#include "v3/ast/symbol.h"
#include "v3/ast/variable.h"
#include "expressions/expression.h"
#include "expressions/binaryexpression.h"
#include "expressions/unaryexpression.h"
#include "expressions/intexpression.h"
#include "expressions/boolexpression.h"
#include "expressions/floatexpression.h"
#include "expressions/stringexpression.h"
#include "expressions/symbolexpression.h"

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

  virtual void enterVariable(AnyFXParser::VariableContext *ctx) = 0;
  virtual void exitVariable(AnyFXParser::VariableContext *ctx) = 0;

  virtual void enterStructure(AnyFXParser::StructureContext *ctx) = 0;
  virtual void exitStructure(AnyFXParser::StructureContext *ctx) = 0;

  virtual void enterCodeblock(AnyFXParser::CodeblockContext *ctx) = 0;
  virtual void exitCodeblock(AnyFXParser::CodeblockContext *ctx) = 0;

  virtual void enterFunction(AnyFXParser::FunctionContext *ctx) = 0;
  virtual void exitFunction(AnyFXParser::FunctionContext *ctx) = 0;

  virtual void enterProgram(AnyFXParser::ProgramContext *ctx) = 0;
  virtual void exitProgram(AnyFXParser::ProgramContext *ctx) = 0;

  virtual void enterState(AnyFXParser::StateContext *ctx) = 0;
  virtual void exitState(AnyFXParser::StateContext *ctx) = 0;

  virtual void enterExpression(AnyFXParser::ExpressionContext *ctx) = 0;
  virtual void exitExpression(AnyFXParser::ExpressionContext *ctx) = 0;

  virtual void enterBinaryexp7(AnyFXParser::Binaryexp7Context *ctx) = 0;
  virtual void exitBinaryexp7(AnyFXParser::Binaryexp7Context *ctx) = 0;

  virtual void enterBinaryexp6(AnyFXParser::Binaryexp6Context *ctx) = 0;
  virtual void exitBinaryexp6(AnyFXParser::Binaryexp6Context *ctx) = 0;

  virtual void enterBinaryexp5(AnyFXParser::Binaryexp5Context *ctx) = 0;
  virtual void exitBinaryexp5(AnyFXParser::Binaryexp5Context *ctx) = 0;

  virtual void enterBinaryexp4(AnyFXParser::Binaryexp4Context *ctx) = 0;
  virtual void exitBinaryexp4(AnyFXParser::Binaryexp4Context *ctx) = 0;

  virtual void enterBinaryexp3(AnyFXParser::Binaryexp3Context *ctx) = 0;
  virtual void exitBinaryexp3(AnyFXParser::Binaryexp3Context *ctx) = 0;

  virtual void enterBinaryexp2(AnyFXParser::Binaryexp2Context *ctx) = 0;
  virtual void exitBinaryexp2(AnyFXParser::Binaryexp2Context *ctx) = 0;

  virtual void enterBinaryexp1(AnyFXParser::Binaryexp1Context *ctx) = 0;
  virtual void exitBinaryexp1(AnyFXParser::Binaryexp1Context *ctx) = 0;

  virtual void enterBinaryexpatom(AnyFXParser::BinaryexpatomContext *ctx) = 0;
  virtual void exitBinaryexpatom(AnyFXParser::BinaryexpatomContext *ctx) = 0;

  virtual void enterParantexpression(AnyFXParser::ParantexpressionContext *ctx) = 0;
  virtual void exitParantexpression(AnyFXParser::ParantexpressionContext *ctx) = 0;


};

