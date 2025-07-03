// Generated from /Users/gustavsterbrant/Code/GPULang/code/compiler/ext/antlr4/grammar/GPULang.g4 by ANTLR 4.13.1
import org.antlr.v4.runtime.tree.ParseTreeListener;

/**
 * This interface defines a complete listener for a parse tree produced by
 * {@link GPULangParser}.
 */
public interface GPULangListener extends ParseTreeListener {
	/**
	 * Enter a parse tree produced by {@link GPULangParser#string}.
	 * @param ctx the parse tree
	 */
	void enterString(GPULangParser.StringContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#string}.
	 * @param ctx the parse tree
	 */
	void exitString(GPULangParser.StringContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#path}.
	 * @param ctx the parse tree
	 */
	void enterPath(GPULangParser.PathContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#path}.
	 * @param ctx the parse tree
	 */
	void exitPath(GPULangParser.PathContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#boolean}.
	 * @param ctx the parse tree
	 */
	void enterBoolean(GPULangParser.BooleanContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#boolean}.
	 * @param ctx the parse tree
	 */
	void exitBoolean(GPULangParser.BooleanContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#entry}.
	 * @param ctx the parse tree
	 */
	void enterEntry(GPULangParser.EntryContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#entry}.
	 * @param ctx the parse tree
	 */
	void exitEntry(GPULangParser.EntryContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#effect}.
	 * @param ctx the parse tree
	 */
	void enterEffect(GPULangParser.EffectContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#effect}.
	 * @param ctx the parse tree
	 */
	void exitEffect(GPULangParser.EffectContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#linePreprocessorEntry}.
	 * @param ctx the parse tree
	 */
	void enterLinePreprocessorEntry(GPULangParser.LinePreprocessorEntryContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#linePreprocessorEntry}.
	 * @param ctx the parse tree
	 */
	void exitLinePreprocessorEntry(GPULangParser.LinePreprocessorEntryContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#alias}.
	 * @param ctx the parse tree
	 */
	void enterAlias(GPULangParser.AliasContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#alias}.
	 * @param ctx the parse tree
	 */
	void exitAlias(GPULangParser.AliasContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#annotation}.
	 * @param ctx the parse tree
	 */
	void enterAnnotation(GPULangParser.AnnotationContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#annotation}.
	 * @param ctx the parse tree
	 */
	void exitAnnotation(GPULangParser.AnnotationContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#attribute}.
	 * @param ctx the parse tree
	 */
	void enterAttribute(GPULangParser.AttributeContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#attribute}.
	 * @param ctx the parse tree
	 */
	void exitAttribute(GPULangParser.AttributeContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#typeDeclaration}.
	 * @param ctx the parse tree
	 */
	void enterTypeDeclaration(GPULangParser.TypeDeclarationContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#typeDeclaration}.
	 * @param ctx the parse tree
	 */
	void exitTypeDeclaration(GPULangParser.TypeDeclarationContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#generate}.
	 * @param ctx the parse tree
	 */
	void enterGenerate(GPULangParser.GenerateContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#generate}.
	 * @param ctx the parse tree
	 */
	void exitGenerate(GPULangParser.GenerateContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#gen_statement}.
	 * @param ctx the parse tree
	 */
	void enterGen_statement(GPULangParser.Gen_statementContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#gen_statement}.
	 * @param ctx the parse tree
	 */
	void exitGen_statement(GPULangParser.Gen_statementContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#gen_scope_statement}.
	 * @param ctx the parse tree
	 */
	void enterGen_scope_statement(GPULangParser.Gen_scope_statementContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#gen_scope_statement}.
	 * @param ctx the parse tree
	 */
	void exitGen_scope_statement(GPULangParser.Gen_scope_statementContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#gen_if_statement}.
	 * @param ctx the parse tree
	 */
	void enterGen_if_statement(GPULangParser.Gen_if_statementContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#gen_if_statement}.
	 * @param ctx the parse tree
	 */
	void exitGen_if_statement(GPULangParser.Gen_if_statementContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#variables}.
	 * @param ctx the parse tree
	 */
	void enterVariables(GPULangParser.VariablesContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#variables}.
	 * @param ctx the parse tree
	 */
	void exitVariables(GPULangParser.VariablesContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#structureDeclaration}.
	 * @param ctx the parse tree
	 */
	void enterStructureDeclaration(GPULangParser.StructureDeclarationContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#structureDeclaration}.
	 * @param ctx the parse tree
	 */
	void exitStructureDeclaration(GPULangParser.StructureDeclarationContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#structure}.
	 * @param ctx the parse tree
	 */
	void enterStructure(GPULangParser.StructureContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#structure}.
	 * @param ctx the parse tree
	 */
	void exitStructure(GPULangParser.StructureContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#enumeration}.
	 * @param ctx the parse tree
	 */
	void enterEnumeration(GPULangParser.EnumerationContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#enumeration}.
	 * @param ctx the parse tree
	 */
	void exitEnumeration(GPULangParser.EnumerationContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#parameter}.
	 * @param ctx the parse tree
	 */
	void enterParameter(GPULangParser.ParameterContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#parameter}.
	 * @param ctx the parse tree
	 */
	void exitParameter(GPULangParser.ParameterContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#functionDeclaration}.
	 * @param ctx the parse tree
	 */
	void enterFunctionDeclaration(GPULangParser.FunctionDeclarationContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#functionDeclaration}.
	 * @param ctx the parse tree
	 */
	void exitFunctionDeclaration(GPULangParser.FunctionDeclarationContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#function}.
	 * @param ctx the parse tree
	 */
	void enterFunction(GPULangParser.FunctionContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#function}.
	 * @param ctx the parse tree
	 */
	void exitFunction(GPULangParser.FunctionContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#program}.
	 * @param ctx the parse tree
	 */
	void enterProgram(GPULangParser.ProgramContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#program}.
	 * @param ctx the parse tree
	 */
	void exitProgram(GPULangParser.ProgramContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#sampler}.
	 * @param ctx the parse tree
	 */
	void enterSampler(GPULangParser.SamplerContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#sampler}.
	 * @param ctx the parse tree
	 */
	void exitSampler(GPULangParser.SamplerContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#state}.
	 * @param ctx the parse tree
	 */
	void enterState(GPULangParser.StateContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#state}.
	 * @param ctx the parse tree
	 */
	void exitState(GPULangParser.StateContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#statement}.
	 * @param ctx the parse tree
	 */
	void enterStatement(GPULangParser.StatementContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#statement}.
	 * @param ctx the parse tree
	 */
	void exitStatement(GPULangParser.StatementContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#expressionStatement}.
	 * @param ctx the parse tree
	 */
	void enterExpressionStatement(GPULangParser.ExpressionStatementContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#expressionStatement}.
	 * @param ctx the parse tree
	 */
	void exitExpressionStatement(GPULangParser.ExpressionStatementContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#ifStatement}.
	 * @param ctx the parse tree
	 */
	void enterIfStatement(GPULangParser.IfStatementContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#ifStatement}.
	 * @param ctx the parse tree
	 */
	void exitIfStatement(GPULangParser.IfStatementContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#forStatement}.
	 * @param ctx the parse tree
	 */
	void enterForStatement(GPULangParser.ForStatementContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#forStatement}.
	 * @param ctx the parse tree
	 */
	void exitForStatement(GPULangParser.ForStatementContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#forRangeStatement}.
	 * @param ctx the parse tree
	 */
	void enterForRangeStatement(GPULangParser.ForRangeStatementContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#forRangeStatement}.
	 * @param ctx the parse tree
	 */
	void exitForRangeStatement(GPULangParser.ForRangeStatementContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#forUniformValueStatement}.
	 * @param ctx the parse tree
	 */
	void enterForUniformValueStatement(GPULangParser.ForUniformValueStatementContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#forUniformValueStatement}.
	 * @param ctx the parse tree
	 */
	void exitForUniformValueStatement(GPULangParser.ForUniformValueStatementContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#whileStatement}.
	 * @param ctx the parse tree
	 */
	void enterWhileStatement(GPULangParser.WhileStatementContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#whileStatement}.
	 * @param ctx the parse tree
	 */
	void exitWhileStatement(GPULangParser.WhileStatementContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#scopeStatement}.
	 * @param ctx the parse tree
	 */
	void enterScopeStatement(GPULangParser.ScopeStatementContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#scopeStatement}.
	 * @param ctx the parse tree
	 */
	void exitScopeStatement(GPULangParser.ScopeStatementContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#terminateStatement}.
	 * @param ctx the parse tree
	 */
	void enterTerminateStatement(GPULangParser.TerminateStatementContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#terminateStatement}.
	 * @param ctx the parse tree
	 */
	void exitTerminateStatement(GPULangParser.TerminateStatementContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#continueStatement}.
	 * @param ctx the parse tree
	 */
	void enterContinueStatement(GPULangParser.ContinueStatementContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#continueStatement}.
	 * @param ctx the parse tree
	 */
	void exitContinueStatement(GPULangParser.ContinueStatementContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#switchStatement}.
	 * @param ctx the parse tree
	 */
	void enterSwitchStatement(GPULangParser.SwitchStatementContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#switchStatement}.
	 * @param ctx the parse tree
	 */
	void exitSwitchStatement(GPULangParser.SwitchStatementContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#breakStatement}.
	 * @param ctx the parse tree
	 */
	void enterBreakStatement(GPULangParser.BreakStatementContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#breakStatement}.
	 * @param ctx the parse tree
	 */
	void exitBreakStatement(GPULangParser.BreakStatementContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#expressionList}.
	 * @param ctx the parse tree
	 */
	void enterExpressionList(GPULangParser.ExpressionListContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#expressionList}.
	 * @param ctx the parse tree
	 */
	void exitExpressionList(GPULangParser.ExpressionListContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#expression}.
	 * @param ctx the parse tree
	 */
	void enterExpression(GPULangParser.ExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#expression}.
	 * @param ctx the parse tree
	 */
	void exitExpression(GPULangParser.ExpressionContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#binaryexpatom}.
	 * @param ctx the parse tree
	 */
	void enterBinaryexpatom(GPULangParser.BinaryexpatomContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#binaryexpatom}.
	 * @param ctx the parse tree
	 */
	void exitBinaryexpatom(GPULangParser.BinaryexpatomContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#initializerExpression}.
	 * @param ctx the parse tree
	 */
	void enterInitializerExpression(GPULangParser.InitializerExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#initializerExpression}.
	 * @param ctx the parse tree
	 */
	void exitInitializerExpression(GPULangParser.InitializerExpressionContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#arrayInitializerExpression}.
	 * @param ctx the parse tree
	 */
	void enterArrayInitializerExpression(GPULangParser.ArrayInitializerExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#arrayInitializerExpression}.
	 * @param ctx the parse tree
	 */
	void exitArrayInitializerExpression(GPULangParser.ArrayInitializerExpressionContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#floatVecLiteralExpression}.
	 * @param ctx the parse tree
	 */
	void enterFloatVecLiteralExpression(GPULangParser.FloatVecLiteralExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#floatVecLiteralExpression}.
	 * @param ctx the parse tree
	 */
	void exitFloatVecLiteralExpression(GPULangParser.FloatVecLiteralExpressionContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#doubleVecLiteralExpression}.
	 * @param ctx the parse tree
	 */
	void enterDoubleVecLiteralExpression(GPULangParser.DoubleVecLiteralExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#doubleVecLiteralExpression}.
	 * @param ctx the parse tree
	 */
	void exitDoubleVecLiteralExpression(GPULangParser.DoubleVecLiteralExpressionContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#intVecLiteralExpression}.
	 * @param ctx the parse tree
	 */
	void enterIntVecLiteralExpression(GPULangParser.IntVecLiteralExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#intVecLiteralExpression}.
	 * @param ctx the parse tree
	 */
	void exitIntVecLiteralExpression(GPULangParser.IntVecLiteralExpressionContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#uintVecLiteralExpression}.
	 * @param ctx the parse tree
	 */
	void enterUintVecLiteralExpression(GPULangParser.UintVecLiteralExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#uintVecLiteralExpression}.
	 * @param ctx the parse tree
	 */
	void exitUintVecLiteralExpression(GPULangParser.UintVecLiteralExpressionContext ctx);
	/**
	 * Enter a parse tree produced by {@link GPULangParser#booleanVecLiteralExpression}.
	 * @param ctx the parse tree
	 */
	void enterBooleanVecLiteralExpression(GPULangParser.BooleanVecLiteralExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link GPULangParser#booleanVecLiteralExpression}.
	 * @param ctx the parse tree
	 */
	void exitBooleanVecLiteralExpression(GPULangParser.BooleanVecLiteralExpressionContext ctx);
}