// Generated from antlr4/grammar/GPULang.g4 by ANTLR 4.13.2

import {ParseTreeListener} from "antlr4";


import { StringContext } from "./GPULangParser.js";
import { BooleanContext } from "./GPULangParser.js";
import { EntryContext } from "./GPULangParser.js";
import { EffectContext } from "./GPULangParser.js";
import { LinePreprocessorEntryContext } from "./GPULangParser.js";
import { AliasContext } from "./GPULangParser.js";
import { AnnotationContext } from "./GPULangParser.js";
import { AttributeContext } from "./GPULangParser.js";
import { TypeDeclarationContext } from "./GPULangParser.js";
import { VariablesContext } from "./GPULangParser.js";
import { StructureDeclarationContext } from "./GPULangParser.js";
import { StructureContext } from "./GPULangParser.js";
import { EnumerationContext } from "./GPULangParser.js";
import { ParameterContext } from "./GPULangParser.js";
import { FunctionDeclarationContext } from "./GPULangParser.js";
import { FunctionContext } from "./GPULangParser.js";
import { ProgramContext } from "./GPULangParser.js";
import { SamplerContext } from "./GPULangParser.js";
import { StateContext } from "./GPULangParser.js";
import { StatementContext } from "./GPULangParser.js";
import { ExpressionStatementContext } from "./GPULangParser.js";
import { IfStatementContext } from "./GPULangParser.js";
import { ForStatementContext } from "./GPULangParser.js";
import { ForRangeStatementContext } from "./GPULangParser.js";
import { WhileStatementContext } from "./GPULangParser.js";
import { ScopeStatementContext } from "./GPULangParser.js";
import { ReturnStatementContext } from "./GPULangParser.js";
import { DiscardStatementContext } from "./GPULangParser.js";
import { ContinueStatementContext } from "./GPULangParser.js";
import { SwitchStatementContext } from "./GPULangParser.js";
import { BreakStatementContext } from "./GPULangParser.js";
import { ExpressionContext } from "./GPULangParser.js";
import { CommaExpressionContext } from "./GPULangParser.js";
import { AssignmentExpressionContext } from "./GPULangParser.js";
import { LogicalOrExpressionContext } from "./GPULangParser.js";
import { LogicalAndExpressionContext } from "./GPULangParser.js";
import { OrExpressionContext } from "./GPULangParser.js";
import { XorExpressionContext } from "./GPULangParser.js";
import { AndExpressionContext } from "./GPULangParser.js";
import { EquivalencyExpressionContext } from "./GPULangParser.js";
import { RelationalExpressionContext } from "./GPULangParser.js";
import { ShiftExpressionContext } from "./GPULangParser.js";
import { AddSubtractExpressionContext } from "./GPULangParser.js";
import { MultiplyDivideExpressionContext } from "./GPULangParser.js";
import { SuffixExpressionContext } from "./GPULangParser.js";
import { PrefixExpressionContext } from "./GPULangParser.js";
import { NamespaceExpressionContext } from "./GPULangParser.js";
import { BinaryexpatomContext } from "./GPULangParser.js";
import { InitializerExpressionContext } from "./GPULangParser.js";
import { ArrayInitializerExpressionContext } from "./GPULangParser.js";


/**
 * This interface defines a complete listener for a parse tree produced by
 * `GPULangParser`.
 */
export default class GPULangListener extends ParseTreeListener {
	/**
	 * Enter a parse tree produced by `GPULangParser.string`.
	 * @param ctx the parse tree
	 */
	enterString?: (ctx: StringContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.string`.
	 * @param ctx the parse tree
	 */
	exitString?: (ctx: StringContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.boolean`.
	 * @param ctx the parse tree
	 */
	enterBoolean?: (ctx: BooleanContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.boolean`.
	 * @param ctx the parse tree
	 */
	exitBoolean?: (ctx: BooleanContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.entry`.
	 * @param ctx the parse tree
	 */
	enterEntry?: (ctx: EntryContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.entry`.
	 * @param ctx the parse tree
	 */
	exitEntry?: (ctx: EntryContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.effect`.
	 * @param ctx the parse tree
	 */
	enterEffect?: (ctx: EffectContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.effect`.
	 * @param ctx the parse tree
	 */
	exitEffect?: (ctx: EffectContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.linePreprocessorEntry`.
	 * @param ctx the parse tree
	 */
	enterLinePreprocessorEntry?: (ctx: LinePreprocessorEntryContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.linePreprocessorEntry`.
	 * @param ctx the parse tree
	 */
	exitLinePreprocessorEntry?: (ctx: LinePreprocessorEntryContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.alias`.
	 * @param ctx the parse tree
	 */
	enterAlias?: (ctx: AliasContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.alias`.
	 * @param ctx the parse tree
	 */
	exitAlias?: (ctx: AliasContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.annotation`.
	 * @param ctx the parse tree
	 */
	enterAnnotation?: (ctx: AnnotationContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.annotation`.
	 * @param ctx the parse tree
	 */
	exitAnnotation?: (ctx: AnnotationContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.attribute`.
	 * @param ctx the parse tree
	 */
	enterAttribute?: (ctx: AttributeContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.attribute`.
	 * @param ctx the parse tree
	 */
	exitAttribute?: (ctx: AttributeContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.typeDeclaration`.
	 * @param ctx the parse tree
	 */
	enterTypeDeclaration?: (ctx: TypeDeclarationContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.typeDeclaration`.
	 * @param ctx the parse tree
	 */
	exitTypeDeclaration?: (ctx: TypeDeclarationContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.variables`.
	 * @param ctx the parse tree
	 */
	enterVariables?: (ctx: VariablesContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.variables`.
	 * @param ctx the parse tree
	 */
	exitVariables?: (ctx: VariablesContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.structureDeclaration`.
	 * @param ctx the parse tree
	 */
	enterStructureDeclaration?: (ctx: StructureDeclarationContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.structureDeclaration`.
	 * @param ctx the parse tree
	 */
	exitStructureDeclaration?: (ctx: StructureDeclarationContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.structure`.
	 * @param ctx the parse tree
	 */
	enterStructure?: (ctx: StructureContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.structure`.
	 * @param ctx the parse tree
	 */
	exitStructure?: (ctx: StructureContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.enumeration`.
	 * @param ctx the parse tree
	 */
	enterEnumeration?: (ctx: EnumerationContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.enumeration`.
	 * @param ctx the parse tree
	 */
	exitEnumeration?: (ctx: EnumerationContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.parameter`.
	 * @param ctx the parse tree
	 */
	enterParameter?: (ctx: ParameterContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.parameter`.
	 * @param ctx the parse tree
	 */
	exitParameter?: (ctx: ParameterContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.functionDeclaration`.
	 * @param ctx the parse tree
	 */
	enterFunctionDeclaration?: (ctx: FunctionDeclarationContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.functionDeclaration`.
	 * @param ctx the parse tree
	 */
	exitFunctionDeclaration?: (ctx: FunctionDeclarationContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.function`.
	 * @param ctx the parse tree
	 */
	enterFunction?: (ctx: FunctionContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.function`.
	 * @param ctx the parse tree
	 */
	exitFunction?: (ctx: FunctionContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.program`.
	 * @param ctx the parse tree
	 */
	enterProgram?: (ctx: ProgramContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.program`.
	 * @param ctx the parse tree
	 */
	exitProgram?: (ctx: ProgramContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.sampler`.
	 * @param ctx the parse tree
	 */
	enterSampler?: (ctx: SamplerContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.sampler`.
	 * @param ctx the parse tree
	 */
	exitSampler?: (ctx: SamplerContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.state`.
	 * @param ctx the parse tree
	 */
	enterState?: (ctx: StateContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.state`.
	 * @param ctx the parse tree
	 */
	exitState?: (ctx: StateContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.statement`.
	 * @param ctx the parse tree
	 */
	enterStatement?: (ctx: StatementContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.statement`.
	 * @param ctx the parse tree
	 */
	exitStatement?: (ctx: StatementContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.expressionStatement`.
	 * @param ctx the parse tree
	 */
	enterExpressionStatement?: (ctx: ExpressionStatementContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.expressionStatement`.
	 * @param ctx the parse tree
	 */
	exitExpressionStatement?: (ctx: ExpressionStatementContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.ifStatement`.
	 * @param ctx the parse tree
	 */
	enterIfStatement?: (ctx: IfStatementContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.ifStatement`.
	 * @param ctx the parse tree
	 */
	exitIfStatement?: (ctx: IfStatementContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.forStatement`.
	 * @param ctx the parse tree
	 */
	enterForStatement?: (ctx: ForStatementContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.forStatement`.
	 * @param ctx the parse tree
	 */
	exitForStatement?: (ctx: ForStatementContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.forRangeStatement`.
	 * @param ctx the parse tree
	 */
	enterForRangeStatement?: (ctx: ForRangeStatementContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.forRangeStatement`.
	 * @param ctx the parse tree
	 */
	exitForRangeStatement?: (ctx: ForRangeStatementContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.whileStatement`.
	 * @param ctx the parse tree
	 */
	enterWhileStatement?: (ctx: WhileStatementContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.whileStatement`.
	 * @param ctx the parse tree
	 */
	exitWhileStatement?: (ctx: WhileStatementContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.scopeStatement`.
	 * @param ctx the parse tree
	 */
	enterScopeStatement?: (ctx: ScopeStatementContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.scopeStatement`.
	 * @param ctx the parse tree
	 */
	exitScopeStatement?: (ctx: ScopeStatementContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.returnStatement`.
	 * @param ctx the parse tree
	 */
	enterReturnStatement?: (ctx: ReturnStatementContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.returnStatement`.
	 * @param ctx the parse tree
	 */
	exitReturnStatement?: (ctx: ReturnStatementContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.discardStatement`.
	 * @param ctx the parse tree
	 */
	enterDiscardStatement?: (ctx: DiscardStatementContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.discardStatement`.
	 * @param ctx the parse tree
	 */
	exitDiscardStatement?: (ctx: DiscardStatementContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.continueStatement`.
	 * @param ctx the parse tree
	 */
	enterContinueStatement?: (ctx: ContinueStatementContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.continueStatement`.
	 * @param ctx the parse tree
	 */
	exitContinueStatement?: (ctx: ContinueStatementContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.switchStatement`.
	 * @param ctx the parse tree
	 */
	enterSwitchStatement?: (ctx: SwitchStatementContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.switchStatement`.
	 * @param ctx the parse tree
	 */
	exitSwitchStatement?: (ctx: SwitchStatementContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.breakStatement`.
	 * @param ctx the parse tree
	 */
	enterBreakStatement?: (ctx: BreakStatementContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.breakStatement`.
	 * @param ctx the parse tree
	 */
	exitBreakStatement?: (ctx: BreakStatementContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.expression`.
	 * @param ctx the parse tree
	 */
	enterExpression?: (ctx: ExpressionContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.expression`.
	 * @param ctx the parse tree
	 */
	exitExpression?: (ctx: ExpressionContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.commaExpression`.
	 * @param ctx the parse tree
	 */
	enterCommaExpression?: (ctx: CommaExpressionContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.commaExpression`.
	 * @param ctx the parse tree
	 */
	exitCommaExpression?: (ctx: CommaExpressionContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.assignmentExpression`.
	 * @param ctx the parse tree
	 */
	enterAssignmentExpression?: (ctx: AssignmentExpressionContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.assignmentExpression`.
	 * @param ctx the parse tree
	 */
	exitAssignmentExpression?: (ctx: AssignmentExpressionContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.logicalOrExpression`.
	 * @param ctx the parse tree
	 */
	enterLogicalOrExpression?: (ctx: LogicalOrExpressionContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.logicalOrExpression`.
	 * @param ctx the parse tree
	 */
	exitLogicalOrExpression?: (ctx: LogicalOrExpressionContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.logicalAndExpression`.
	 * @param ctx the parse tree
	 */
	enterLogicalAndExpression?: (ctx: LogicalAndExpressionContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.logicalAndExpression`.
	 * @param ctx the parse tree
	 */
	exitLogicalAndExpression?: (ctx: LogicalAndExpressionContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.orExpression`.
	 * @param ctx the parse tree
	 */
	enterOrExpression?: (ctx: OrExpressionContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.orExpression`.
	 * @param ctx the parse tree
	 */
	exitOrExpression?: (ctx: OrExpressionContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.xorExpression`.
	 * @param ctx the parse tree
	 */
	enterXorExpression?: (ctx: XorExpressionContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.xorExpression`.
	 * @param ctx the parse tree
	 */
	exitXorExpression?: (ctx: XorExpressionContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.andExpression`.
	 * @param ctx the parse tree
	 */
	enterAndExpression?: (ctx: AndExpressionContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.andExpression`.
	 * @param ctx the parse tree
	 */
	exitAndExpression?: (ctx: AndExpressionContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.equivalencyExpression`.
	 * @param ctx the parse tree
	 */
	enterEquivalencyExpression?: (ctx: EquivalencyExpressionContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.equivalencyExpression`.
	 * @param ctx the parse tree
	 */
	exitEquivalencyExpression?: (ctx: EquivalencyExpressionContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.relationalExpression`.
	 * @param ctx the parse tree
	 */
	enterRelationalExpression?: (ctx: RelationalExpressionContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.relationalExpression`.
	 * @param ctx the parse tree
	 */
	exitRelationalExpression?: (ctx: RelationalExpressionContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.shiftExpression`.
	 * @param ctx the parse tree
	 */
	enterShiftExpression?: (ctx: ShiftExpressionContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.shiftExpression`.
	 * @param ctx the parse tree
	 */
	exitShiftExpression?: (ctx: ShiftExpressionContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.addSubtractExpression`.
	 * @param ctx the parse tree
	 */
	enterAddSubtractExpression?: (ctx: AddSubtractExpressionContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.addSubtractExpression`.
	 * @param ctx the parse tree
	 */
	exitAddSubtractExpression?: (ctx: AddSubtractExpressionContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.multiplyDivideExpression`.
	 * @param ctx the parse tree
	 */
	enterMultiplyDivideExpression?: (ctx: MultiplyDivideExpressionContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.multiplyDivideExpression`.
	 * @param ctx the parse tree
	 */
	exitMultiplyDivideExpression?: (ctx: MultiplyDivideExpressionContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.suffixExpression`.
	 * @param ctx the parse tree
	 */
	enterSuffixExpression?: (ctx: SuffixExpressionContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.suffixExpression`.
	 * @param ctx the parse tree
	 */
	exitSuffixExpression?: (ctx: SuffixExpressionContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.prefixExpression`.
	 * @param ctx the parse tree
	 */
	enterPrefixExpression?: (ctx: PrefixExpressionContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.prefixExpression`.
	 * @param ctx the parse tree
	 */
	exitPrefixExpression?: (ctx: PrefixExpressionContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.namespaceExpression`.
	 * @param ctx the parse tree
	 */
	enterNamespaceExpression?: (ctx: NamespaceExpressionContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.namespaceExpression`.
	 * @param ctx the parse tree
	 */
	exitNamespaceExpression?: (ctx: NamespaceExpressionContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.binaryexpatom`.
	 * @param ctx the parse tree
	 */
	enterBinaryexpatom?: (ctx: BinaryexpatomContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.binaryexpatom`.
	 * @param ctx the parse tree
	 */
	exitBinaryexpatom?: (ctx: BinaryexpatomContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.initializerExpression`.
	 * @param ctx the parse tree
	 */
	enterInitializerExpression?: (ctx: InitializerExpressionContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.initializerExpression`.
	 * @param ctx the parse tree
	 */
	exitInitializerExpression?: (ctx: InitializerExpressionContext) => void;
	/**
	 * Enter a parse tree produced by `GPULangParser.arrayInitializerExpression`.
	 * @param ctx the parse tree
	 */
	enterArrayInitializerExpression?: (ctx: ArrayInitializerExpressionContext) => void;
	/**
	 * Exit a parse tree produced by `GPULangParser.arrayInitializerExpression`.
	 * @param ctx the parse tree
	 */
	exitArrayInitializerExpression?: (ctx: ArrayInitializerExpressionContext) => void;
}

