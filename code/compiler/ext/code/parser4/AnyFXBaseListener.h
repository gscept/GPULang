

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

  virtual void enterVariable(AnyFXParser::VariableContext * /*ctx*/) override { }
  virtual void exitVariable(AnyFXParser::VariableContext * /*ctx*/) override { }

  virtual void enterStructure(AnyFXParser::StructureContext * /*ctx*/) override { }
  virtual void exitStructure(AnyFXParser::StructureContext * /*ctx*/) override { }

  virtual void enterCodeblock(AnyFXParser::CodeblockContext * /*ctx*/) override { }
  virtual void exitCodeblock(AnyFXParser::CodeblockContext * /*ctx*/) override { }

  virtual void enterFunction(AnyFXParser::FunctionContext * /*ctx*/) override { }
  virtual void exitFunction(AnyFXParser::FunctionContext * /*ctx*/) override { }

  virtual void enterProgram(AnyFXParser::ProgramContext * /*ctx*/) override { }
  virtual void exitProgram(AnyFXParser::ProgramContext * /*ctx*/) override { }

  virtual void enterState(AnyFXParser::StateContext * /*ctx*/) override { }
  virtual void exitState(AnyFXParser::StateContext * /*ctx*/) override { }

  virtual void enterExpression(AnyFXParser::ExpressionContext * /*ctx*/) override { }
  virtual void exitExpression(AnyFXParser::ExpressionContext * /*ctx*/) override { }

  virtual void enterBinaryexp7(AnyFXParser::Binaryexp7Context * /*ctx*/) override { }
  virtual void exitBinaryexp7(AnyFXParser::Binaryexp7Context * /*ctx*/) override { }

  virtual void enterBinaryexp6(AnyFXParser::Binaryexp6Context * /*ctx*/) override { }
  virtual void exitBinaryexp6(AnyFXParser::Binaryexp6Context * /*ctx*/) override { }

  virtual void enterBinaryexp5(AnyFXParser::Binaryexp5Context * /*ctx*/) override { }
  virtual void exitBinaryexp5(AnyFXParser::Binaryexp5Context * /*ctx*/) override { }

  virtual void enterBinaryexp4(AnyFXParser::Binaryexp4Context * /*ctx*/) override { }
  virtual void exitBinaryexp4(AnyFXParser::Binaryexp4Context * /*ctx*/) override { }

  virtual void enterBinaryexp3(AnyFXParser::Binaryexp3Context * /*ctx*/) override { }
  virtual void exitBinaryexp3(AnyFXParser::Binaryexp3Context * /*ctx*/) override { }

  virtual void enterBinaryexp2(AnyFXParser::Binaryexp2Context * /*ctx*/) override { }
  virtual void exitBinaryexp2(AnyFXParser::Binaryexp2Context * /*ctx*/) override { }

  virtual void enterBinaryexp1(AnyFXParser::Binaryexp1Context * /*ctx*/) override { }
  virtual void exitBinaryexp1(AnyFXParser::Binaryexp1Context * /*ctx*/) override { }

  virtual void enterBinaryexpatom(AnyFXParser::BinaryexpatomContext * /*ctx*/) override { }
  virtual void exitBinaryexpatom(AnyFXParser::BinaryexpatomContext * /*ctx*/) override { }

  virtual void enterParantexpression(AnyFXParser::ParantexpressionContext * /*ctx*/) override { }
  virtual void exitParantexpression(AnyFXParser::ParantexpressionContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

