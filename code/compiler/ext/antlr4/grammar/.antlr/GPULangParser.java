// Generated from /Users/gustavsterbrant/Code/GPULang/code/compiler/ext/antlr4/grammar/GPULang.g4 by ANTLR 4.13.1


// include std container
#include <vector>
#include <typeinfo>
#include <iostream>
#include <string>
#include <stack>
#include <tuple>

#include "gpulangtoken.h"
#include "gpulangcompiler.h"
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
#include "ast/preprocessor.h"
#include "ast/variable.h"
#include "ast/statements/breakstatement.h"
#include "ast/statements/continuestatement.h"
#include "ast/statements/expressionstatement.h"
#include "ast/statements/forstatement.h"
#include "ast/statements/ifstatement.h"
#include "ast/statements/terminatestatement.h"
#include "ast/statements/scopestatement.h"
#include "ast/statements/statement.h"
#include "ast/statements/switchstatement.h"
#include "ast/statements/whilestatement.h"
#include "ast/expressions/accessexpression.h"
#include "ast/expressions/arrayindexexpression.h"
#include "ast/expressions/binaryexpression.h"
#include "ast/expressions/boolexpression.h"
#include "ast/expressions/boolvecexpression.h"
#include "ast/expressions/callexpression.h"
#include "ast/expressions/commaexpression.h"
#include "ast/expressions/expression.h"
#include "ast/expressions/floatexpression.h"
#include "ast/expressions/floatvecexpression.h"
#include "ast/expressions/initializerexpression.h"
#include "ast/expressions/arrayinitializerexpression.h"
#include "ast/expressions/intexpression.h"
#include "ast/expressions/intvecexpression.h"
#include "ast/expressions/stringexpression.h"
#include "ast/expressions/symbolexpression.h"
#include "ast/expressions/ternaryexpression.h"
#include "ast/expressions/uintexpression.h"
#include "ast/expressions/uintvecexpression.h"
#include "ast/expressions/unaryexpression.h"
#include "util.h"
#include "memory.h"

using namespace GPULang;


import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.misc.*;
import org.antlr.v4.runtime.tree.*;
import java.util.List;
import java.util.Iterator;
import java.util.ArrayList;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast", "CheckReturnValue"})
public class GPULangParser extends Parser {
	static { RuntimeMetaData.checkVersion("4.13.1", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		T__0=1, T__1=2, T__2=3, T__3=4, T__4=5, T__5=6, T__6=7, T__7=8, T__8=9, 
		T__9=10, T__10=11, T__11=12, T__12=13, T__13=14, T__14=15, T__15=16, T__16=17, 
		T__17=18, T__18=19, T__19=20, T__20=21, T__21=22, T__22=23, T__23=24, 
		T__24=25, T__25=26, T__26=27, T__27=28, T__28=29, T__29=30, T__30=31, 
		T__31=32, T__32=33, T__33=34, T__34=35, T__35=36, T__36=37, T__37=38, 
		SC=39, CO=40, COL=41, LP=42, RP=43, LB=44, RB=45, LL=46, RR=47, DOT=48, 
		NOT=49, EQ=50, QO=51, QU=52, AND=53, ANDSET=54, OR=55, ORSET=56, XOR=57, 
		XORSET=58, CONJUGATE=59, Q=60, NU=61, FORWARDSLASH=62, LESS=63, LESSEQ=64, 
		GREATER=65, GREATEREQ=66, LOGICEQ=67, NOTEQ=68, LOGICAND=69, LOGICOR=70, 
		MOD=71, UNDERSC=72, SOBAKA=73, ADD_OP=74, SUB_OP=75, DIV_OP=76, MUL_OP=77, 
		ARROW=78, INTEGERLITERAL=79, UINTEGERLITERAL=80, COMMENT=81, ML_COMMENT=82, 
		FLOATLITERAL=83, DOUBLELITERAL=84, HEX=85, IDENTIFIER=86, WS=87;
	public static final int
		RULE_string = 0, RULE_path = 1, RULE_boolean = 2, RULE_entry = 3, RULE_effect = 4, 
		RULE_linePreprocessorEntry = 5, RULE_alias = 6, RULE_annotation = 7, RULE_attribute = 8, 
		RULE_typeDeclaration = 9, RULE_generate = 10, RULE_variables = 11, RULE_structureDeclaration = 12, 
		RULE_structure = 13, RULE_enumeration = 14, RULE_parameter = 15, RULE_functionDeclaration = 16, 
		RULE_function = 17, RULE_program = 18, RULE_sampler = 19, RULE_state = 20, 
		RULE_statement = 21, RULE_expressionStatement = 22, RULE_ifStatement = 23, 
		RULE_forStatement = 24, RULE_forRangeStatement = 25, RULE_forUniformValueStatement = 26, 
		RULE_whileStatement = 27, RULE_scopeStatement = 28, RULE_terminateStatement = 29, 
		RULE_continueStatement = 30, RULE_switchStatement = 31, RULE_breakStatement = 32, 
		RULE_expressionNoComma = 33, RULE_expressionList = 34, RULE_expression = 35, 
		RULE_binaryexpatom = 36, RULE_initializerExpression = 37, RULE_arrayInitializerExpression = 38, 
		RULE_floatVecLiteralExpression = 39, RULE_doubleVecLiteralExpression = 40, 
		RULE_intVecLiteralExpression = 41, RULE_uintVecLiteralExpression = 42, 
		RULE_booleanVecLiteralExpression = 43;
	private static String[] makeRuleNames() {
		return new String[] {
			"string", "path", "boolean", "entry", "effect", "linePreprocessorEntry", 
			"alias", "annotation", "attribute", "typeDeclaration", "generate", "variables", 
			"structureDeclaration", "structure", "enumeration", "parameter", "functionDeclaration", 
			"function", "program", "sampler", "state", "statement", "expressionStatement", 
			"ifStatement", "forStatement", "forRangeStatement", "forUniformValueStatement", 
			"whileStatement", "scopeStatement", "terminateStatement", "continueStatement", 
			"switchStatement", "breakStatement", "expressionNoComma", "expressionList", 
			"expression", "binaryexpatom", "initializerExpression", "arrayInitializerExpression", 
			"floatVecLiteralExpression", "doubleVecLiteralExpression", "intVecLiteralExpression", 
			"uintVecLiteralExpression", "booleanVecLiteralExpression"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, "'true'", "'false'", "'#line'", "'alias'", "'as'", "'generate'", 
			"'struct'", "'enum'", "'program'", "'sampler_state'", "'render_state'", 
			"'if'", "'else'", "'for'", "'..'", "'for_uniform'", "'while'", "'do'", 
			"'return'", "'discard'", "'ray_ignore'", "'ray_terminate'", "'continue'", 
			"'switch'", "'case'", "'default'", "'break'", "'++'", "'--'", "'<<'", 
			"'>>'", "'+='", "'-='", "'*='", "'/='", "'%='", "'<<='", "'>>='", "';'", 
			"','", "':'", "'('", "')'", "'{'", "'}'", "'['", "']'", "'.'", "'!'", 
			"'='", "'\"'", "'?'", "'&'", "'&='", "'|'", "'|='", "'^'", "'^='", "'~'", 
			"'''", "'#'", "'\\'", "'<'", "'<='", "'>'", "'>='", "'=='", "'!='", "'&&'", 
			"'||'", "'%'", "'_'", "'@'", "'+'", "'-'", "'/'", "'*'", "'->'"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, "SC", "CO", "COL", "LP", "RP", "LB", "RB", "LL", "RR", 
			"DOT", "NOT", "EQ", "QO", "QU", "AND", "ANDSET", "OR", "ORSET", "XOR", 
			"XORSET", "CONJUGATE", "Q", "NU", "FORWARDSLASH", "LESS", "LESSEQ", "GREATER", 
			"GREATEREQ", "LOGICEQ", "NOTEQ", "LOGICAND", "LOGICOR", "MOD", "UNDERSC", 
			"SOBAKA", "ADD_OP", "SUB_OP", "DIV_OP", "MUL_OP", "ARROW", "INTEGERLITERAL", 
			"UINTEGERLITERAL", "COMMENT", "ML_COMMENT", "FLOATLITERAL", "DOUBLELITERAL", 
			"HEX", "IDENTIFIER", "WS"
		};
	}
	private static final String[] _SYMBOLIC_NAMES = makeSymbolicNames();
	public static final Vocabulary VOCABULARY = new VocabularyImpl(_LITERAL_NAMES, _SYMBOLIC_NAMES);

	/**
	 * @deprecated Use {@link #VOCABULARY} instead.
	 */
	@Deprecated
	public static final String[] tokenNames;
	static {
		tokenNames = new String[_SYMBOLIC_NAMES.length];
		for (int i = 0; i < tokenNames.length; i++) {
			tokenNames[i] = VOCABULARY.getLiteralName(i);
			if (tokenNames[i] == null) {
				tokenNames[i] = VOCABULARY.getSymbolicName(i);
			}

			if (tokenNames[i] == null) {
				tokenNames[i] = "<INVALID>";
			}
		}
	}

	@Override
	@Deprecated
	public String[] getTokenNames() {
		return tokenNames;
	}

	@Override

	public Vocabulary getVocabulary() {
		return VOCABULARY;
	}

	@Override
	public String getGrammarFileName() { return "GPULang.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public ATN getATN() { return _ATN; }




	// setup function which binds the compiler state to the current AST node
	Symbol::Location
	SetupFile()
	{
	    Symbol::Location location;
	    ::GPULangToken* token = (::GPULangToken*)_input->LT(-1);

	    auto [rawLine, preprocessedLine, file] = GPULangParser::LineStack.back();
	    // assume the previous token is the latest file
	    location.file = file;
	    location.line = token->line - rawLine + preprocessedLine;
	    location.start = token->begin;
	    location.end = location.start + token->getText().length();
	    return location;
	}

	Symbol::Location
	BeginLocationRange()
	{
	    Symbol::Location location;
	    ::GPULangToken* token = (::GPULangToken*)_input->LT(1);

	    auto [rawLine, preprocessedLine, file] = GPULangParser::LineStack.back();
	    location.file = file;
	    location.line = token->line - rawLine + preprocessedLine;
	    location.start = token->begin;
	    location.end = token->end + 1;
	    return location;
	}

	Symbol::Location
	EndLocationRange(const Symbol::Location begin)
	{
	    Symbol::Location location = begin;
	    ::GPULangToken* token = (::GPULangToken*)_input->LT(-1);
	    location.end = token->end + 1;
	    return location;
	}


	public GPULangParser(TokenStream input) {
		super(input);
		_interp = new ParserATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	@SuppressWarnings("CheckReturnValue")
	public static class StringContext extends ParserRuleContext {
		public std::string val;
		public Token data;
		public List<TerminalNode> QO() { return getTokens(GPULangParser.QO); }
		public TerminalNode QO(int i) {
			return getToken(GPULangParser.QO, i);
		}
		public List<TerminalNode> Q() { return getTokens(GPULangParser.Q); }
		public TerminalNode Q(int i) {
			return getToken(GPULangParser.Q, i);
		}
		public StringContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_string; }
	}

	public final StringContext string() throws RecognitionException {
		StringContext _localctx = new StringContext(_ctx, getState());
		enterRule(_localctx, 0, RULE_string);
		int _la;
		try {
			setState(106);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case QO:
				enterOuterAlt(_localctx, 1);
				{
				setState(88);
				match(QO);
				setState(93);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while ((((_la) & ~0x3f) == 0 && ((1L << _la) & -2251799813685250L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & 16777215L) != 0)) {
					{
					{
					setState(89);
					((StringContext)_localctx).data = _input.LT(1);
					_la = _input.LA(1);
					if ( _la <= 0 || (_la==QO) ) {
						((StringContext)_localctx).data = (Token)_errHandler.recoverInline(this);
					}
					else {
						if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
						_errHandler.reportMatch(this);
						consume();
					}
					 _localctx.val.append((((StringContext)_localctx).data!=null?((StringContext)_localctx).data.getText():null)); 
					}
					}
					setState(95);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				setState(96);
				match(QO);
				}
				break;
			case Q:
				enterOuterAlt(_localctx, 2);
				{
				setState(97);
				match(Q);
				setState(102);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while ((((_la) & ~0x3f) == 0 && ((1L << _la) & -1152921504606846978L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & 16777215L) != 0)) {
					{
					{
					setState(98);
					((StringContext)_localctx).data = _input.LT(1);
					_la = _input.LA(1);
					if ( _la <= 0 || (_la==Q) ) {
						((StringContext)_localctx).data = (Token)_errHandler.recoverInline(this);
					}
					else {
						if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
						_errHandler.reportMatch(this);
						consume();
					}
					 _localctx.val.append((((StringContext)_localctx).data!=null?((StringContext)_localctx).data.getText():null)); 
					}
					}
					setState(104);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				setState(105);
				match(Q);
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class PathContext extends ParserRuleContext {
		public std::string val;
		public Token data;
		public List<TerminalNode> QO() { return getTokens(GPULangParser.QO); }
		public TerminalNode QO(int i) {
			return getToken(GPULangParser.QO, i);
		}
		public TerminalNode LESS() { return getToken(GPULangParser.LESS, 0); }
		public List<TerminalNode> GREATER() { return getTokens(GPULangParser.GREATER); }
		public TerminalNode GREATER(int i) {
			return getToken(GPULangParser.GREATER, i);
		}
		public PathContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_path; }
	}

	public final PathContext path() throws RecognitionException {
		PathContext _localctx = new PathContext(_ctx, getState());
		enterRule(_localctx, 2, RULE_path);
		int _la;
		try {
			setState(126);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case QO:
				enterOuterAlt(_localctx, 1);
				{
				setState(108);
				match(QO);
				setState(113);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while ((((_la) & ~0x3f) == 0 && ((1L << _la) & -2251799813685250L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & 16777215L) != 0)) {
					{
					{
					setState(109);
					((PathContext)_localctx).data = _input.LT(1);
					_la = _input.LA(1);
					if ( _la <= 0 || (_la==QO) ) {
						((PathContext)_localctx).data = (Token)_errHandler.recoverInline(this);
					}
					else {
						if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
						_errHandler.reportMatch(this);
						consume();
					}
					 _localctx.val.append((((PathContext)_localctx).data!=null?((PathContext)_localctx).data.getText():null)); 
					}
					}
					setState(115);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				setState(116);
				match(QO);
				}
				break;
			case LESS:
				enterOuterAlt(_localctx, 2);
				{
				setState(117);
				match(LESS);
				setState(122);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while ((((_la) & ~0x3f) == 0 && ((1L << _la) & -2L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & 16777213L) != 0)) {
					{
					{
					setState(118);
					((PathContext)_localctx).data = _input.LT(1);
					_la = _input.LA(1);
					if ( _la <= 0 || (_la==GREATER) ) {
						((PathContext)_localctx).data = (Token)_errHandler.recoverInline(this);
					}
					else {
						if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
						_errHandler.reportMatch(this);
						consume();
					}
					 _localctx.val.append((((PathContext)_localctx).data!=null?((PathContext)_localctx).data.getText():null)); 
					}
					}
					setState(124);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				setState(125);
				match(GREATER);
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class BooleanContext extends ParserRuleContext {
		public bool val;
		public BooleanContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_boolean; }
	}

	public final BooleanContext boolean_() throws RecognitionException {
		BooleanContext _localctx = new BooleanContext(_ctx, getState());
		enterRule(_localctx, 4, RULE_boolean);

		        ((BooleanContext)_localctx).val =  false;
		    
		try {
			setState(132);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__0:
				enterOuterAlt(_localctx, 1);
				{
				setState(128);
				match(T__0);
				 ((BooleanContext)_localctx).val =  true; 
				}
				break;
			case T__1:
				enterOuterAlt(_localctx, 2);
				{
				setState(130);
				match(T__1);
				 ((BooleanContext)_localctx).val =  false; 
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class EntryContext extends ParserRuleContext {
		public Effect* returnEffect;
		public EffectContext effect;
		public EffectContext effect() {
			return getRuleContext(EffectContext.class,0);
		}
		public TerminalNode EOF() { return getToken(GPULangParser.EOF, 0); }
		public EntryContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_entry; }
	}

	public final EntryContext entry() throws RecognitionException {
		EntryContext _localctx = new EntryContext(_ctx, getState());
		enterRule(_localctx, 6, RULE_entry);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(134);
			((EntryContext)_localctx).effect = effect();

			        ((EntryContext)_localctx).returnEffect =  ((EntryContext)_localctx).effect.eff;
			    
			setState(136);
			match(EOF);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class EffectContext extends ParserRuleContext {
		public Effect* eff;
		public GenerateContext generate;
		public AliasContext alias;
		public FunctionDeclarationContext functionDeclaration;
		public FunctionContext function;
		public VariablesContext variables;
		public StructureContext structure;
		public EnumerationContext enumeration;
		public StateContext state;
		public SamplerContext sampler;
		public ProgramContext program;
		public List<LinePreprocessorEntryContext> linePreprocessorEntry() {
			return getRuleContexts(LinePreprocessorEntryContext.class);
		}
		public LinePreprocessorEntryContext linePreprocessorEntry(int i) {
			return getRuleContext(LinePreprocessorEntryContext.class,i);
		}
		public List<GenerateContext> generate() {
			return getRuleContexts(GenerateContext.class);
		}
		public GenerateContext generate(int i) {
			return getRuleContext(GenerateContext.class,i);
		}
		public List<TerminalNode> SC() { return getTokens(GPULangParser.SC); }
		public TerminalNode SC(int i) {
			return getToken(GPULangParser.SC, i);
		}
		public List<AliasContext> alias() {
			return getRuleContexts(AliasContext.class);
		}
		public AliasContext alias(int i) {
			return getRuleContext(AliasContext.class,i);
		}
		public List<FunctionDeclarationContext> functionDeclaration() {
			return getRuleContexts(FunctionDeclarationContext.class);
		}
		public FunctionDeclarationContext functionDeclaration(int i) {
			return getRuleContext(FunctionDeclarationContext.class,i);
		}
		public List<FunctionContext> function() {
			return getRuleContexts(FunctionContext.class);
		}
		public FunctionContext function(int i) {
			return getRuleContext(FunctionContext.class,i);
		}
		public List<VariablesContext> variables() {
			return getRuleContexts(VariablesContext.class);
		}
		public VariablesContext variables(int i) {
			return getRuleContext(VariablesContext.class,i);
		}
		public List<StructureContext> structure() {
			return getRuleContexts(StructureContext.class);
		}
		public StructureContext structure(int i) {
			return getRuleContext(StructureContext.class,i);
		}
		public List<EnumerationContext> enumeration() {
			return getRuleContexts(EnumerationContext.class);
		}
		public EnumerationContext enumeration(int i) {
			return getRuleContext(EnumerationContext.class,i);
		}
		public List<StateContext> state() {
			return getRuleContexts(StateContext.class);
		}
		public StateContext state(int i) {
			return getRuleContext(StateContext.class,i);
		}
		public List<SamplerContext> sampler() {
			return getRuleContexts(SamplerContext.class);
		}
		public SamplerContext sampler(int i) {
			return getRuleContext(SamplerContext.class,i);
		}
		public List<ProgramContext> program() {
			return getRuleContexts(ProgramContext.class);
		}
		public ProgramContext program(int i) {
			return getRuleContext(ProgramContext.class,i);
		}
		public EffectContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_effect; }
	}

	public final EffectContext effect() throws RecognitionException {
		EffectContext _localctx = new EffectContext(_ctx, getState());
		enterRule(_localctx, 8, RULE_effect);

		        ((EffectContext)_localctx).eff =  Alloc<Effect>();
		    
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(180);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,8,_ctx);
			while ( _alt!=1 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1+1 ) {
					{
					setState(178);
					_errHandler.sync(this);
					switch ( getInterpreter().adaptivePredict(_input,7,_ctx) ) {
					case 1:
						{
						setState(138);
						linePreprocessorEntry();
						}
						break;
					case 2:
						{
						setState(139);
						((EffectContext)_localctx).generate = generate();
						setState(140);
						match(SC);
						 _localctx.eff->symbols.Append(((EffectContext)_localctx).generate.sym); 
						}
						break;
					case 3:
						{
						setState(143);
						((EffectContext)_localctx).alias = alias();
						setState(144);
						match(SC);
						 _localctx.eff->symbols.Append(((EffectContext)_localctx).alias.sym); 
						}
						break;
					case 4:
						{
						setState(147);
						((EffectContext)_localctx).functionDeclaration = functionDeclaration();
						setState(148);
						match(SC);
						 _localctx.eff->symbols.Append(((EffectContext)_localctx).functionDeclaration.sym); 
						}
						break;
					case 5:
						{
						setState(151);
						((EffectContext)_localctx).function = function();
						 _localctx.eff->symbols.Append(((EffectContext)_localctx).function.sym); 
						}
						break;
					case 6:
						{
						setState(154);
						((EffectContext)_localctx).variables = variables();
						setState(155);
						match(SC);
						 for (Variable* var : ((EffectContext)_localctx).variables.vars) { _localctx.eff->symbols.Append(var); } 
						}
						break;
					case 7:
						{
						setState(158);
						((EffectContext)_localctx).structure = structure();
						setState(159);
						match(SC);
						 _localctx.eff->symbols.Append(((EffectContext)_localctx).structure.sym); 
						}
						break;
					case 8:
						{
						setState(162);
						((EffectContext)_localctx).enumeration = enumeration();
						setState(163);
						match(SC);
						 _localctx.eff->symbols.Append(((EffectContext)_localctx).enumeration.sym); 
						}
						break;
					case 9:
						{
						setState(166);
						((EffectContext)_localctx).state = state();
						setState(167);
						match(SC);
						 _localctx.eff->symbols.Append(((EffectContext)_localctx).state.sym); 
						}
						break;
					case 10:
						{
						setState(170);
						((EffectContext)_localctx).sampler = sampler();
						setState(171);
						match(SC);
						 _localctx.eff->symbols.Append(((EffectContext)_localctx).sampler.sym); 
						}
						break;
					case 11:
						{
						setState(174);
						((EffectContext)_localctx).program = program();
						setState(175);
						match(SC);
						 _localctx.eff->symbols.Append(((EffectContext)_localctx).program.sym); 
						}
						break;
					}
					} 
				}
				setState(182);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,8,_ctx);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class LinePreprocessorEntryContext extends ParserRuleContext {
		public Token line;
		public StringContext p;
		public TerminalNode INTEGERLITERAL() { return getToken(GPULangParser.INTEGERLITERAL, 0); }
		public StringContext string() {
			return getRuleContext(StringContext.class,0);
		}
		public LinePreprocessorEntryContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_linePreprocessorEntry; }
	}

	public final LinePreprocessorEntryContext linePreprocessorEntry() throws RecognitionException {
		LinePreprocessorEntryContext _localctx = new LinePreprocessorEntryContext(_ctx, getState());
		enterRule(_localctx, 10, RULE_linePreprocessorEntry);

		        size_t origLine;
		    
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(183);
			match(T__2);
			 origLine = _input->LT(-1)->getLine(); 
			setState(185);
			((LinePreprocessorEntryContext)_localctx).line = match(INTEGERLITERAL);
			setState(186);
			((LinePreprocessorEntryContext)_localctx).p = string();
			 LineStack.push_back( {origLine, atoi((((LinePreprocessorEntryContext)_localctx).line!=null?((LinePreprocessorEntryContext)_localctx).line.getText():null).c_str()), ((LinePreprocessorEntryContext)_localctx).p.val }); 
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class AliasContext extends ParserRuleContext {
		public Alias* sym;
		public Token name;
		public Token type;
		public List<TerminalNode> IDENTIFIER() { return getTokens(GPULangParser.IDENTIFIER); }
		public TerminalNode IDENTIFIER(int i) {
			return getToken(GPULangParser.IDENTIFIER, i);
		}
		public AliasContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_alias; }
	}

	public final AliasContext alias() throws RecognitionException {
		AliasContext _localctx = new AliasContext(_ctx, getState());
		enterRule(_localctx, 12, RULE_alias);

		        ((AliasContext)_localctx).sym =  nullptr;
		        std::string name;
		        std::string type;
		    
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(189);
			match(T__3);
			setState(190);
			((AliasContext)_localctx).name = match(IDENTIFIER);
			setState(191);
			match(T__4);
			setState(192);
			((AliasContext)_localctx).type = match(IDENTIFIER);
			 name = (((AliasContext)_localctx).name!=null?((AliasContext)_localctx).name.getText():null); type = (((AliasContext)_localctx).type!=null?((AliasContext)_localctx).type.getText():null); 

			        ((AliasContext)_localctx).sym =  Alloc<Alias>();
			        _localctx.sym->name = name;
			        _localctx.sym->type = type;
			    
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class AnnotationContext extends ParserRuleContext {
		public Annotation* annot;
		public Token name;
		public ExpressionContext value;
		public TerminalNode SOBAKA() { return getToken(GPULangParser.SOBAKA, 0); }
		public TerminalNode LP() { return getToken(GPULangParser.LP, 0); }
		public TerminalNode RP() { return getToken(GPULangParser.RP, 0); }
		public TerminalNode IDENTIFIER() { return getToken(GPULangParser.IDENTIFIER, 0); }
		public ExpressionContext expression() {
			return getRuleContext(ExpressionContext.class,0);
		}
		public AnnotationContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_annotation; }
	}

	public final AnnotationContext annotation() throws RecognitionException {
		AnnotationContext _localctx = new AnnotationContext(_ctx, getState());
		enterRule(_localctx, 14, RULE_annotation);

		        ((AnnotationContext)_localctx).annot =  nullptr;
		    
		try {
			enterOuterAlt(_localctx, 1);
			{
			{
			setState(196);
			match(SOBAKA);
			 ((AnnotationContext)_localctx).annot =  Alloc<Annotation>(); 
			{
			setState(198);
			((AnnotationContext)_localctx).name = match(IDENTIFIER);
			 _localctx.annot->location = SetupFile(); 
			setState(200);
			match(LP);
			setState(201);
			((AnnotationContext)_localctx).value = expression(0);
			setState(202);
			match(RP);
			 _localctx.annot->name = (((AnnotationContext)_localctx).name!=null?((AnnotationContext)_localctx).name.getText():null); _localctx.annot->value = ((AnnotationContext)_localctx).value.tree; 
			}
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class AttributeContext extends ParserRuleContext {
		public Attribute* attr;
		public Token name;
		public ExpressionContext e;
		public ExpressionContext expression;
		public TerminalNode LP() { return getToken(GPULangParser.LP, 0); }
		public TerminalNode RP() { return getToken(GPULangParser.RP, 0); }
		public TerminalNode IDENTIFIER() { return getToken(GPULangParser.IDENTIFIER, 0); }
		public ExpressionContext expression() {
			return getRuleContext(ExpressionContext.class,0);
		}
		public AttributeContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_attribute; }
	}

	public final AttributeContext attribute() throws RecognitionException {
		AttributeContext _localctx = new AttributeContext(_ctx, getState());
		enterRule(_localctx, 16, RULE_attribute);

		        ((AttributeContext)_localctx).attr =  nullptr;
		    
		try {
			setState(214);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,9,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(205);
				((AttributeContext)_localctx).name = match(IDENTIFIER);
				 ((AttributeContext)_localctx).attr =  Alloc<Attribute>(); _localctx.attr->location = SetupFile(); _localctx.attr->name = (((AttributeContext)_localctx).name!=null?((AttributeContext)_localctx).name.getText():null); 
				setState(207);
				match(LP);
				setState(208);
				((AttributeContext)_localctx).e = ((AttributeContext)_localctx).expression = expression(0);
				setState(209);
				match(RP);
				 _localctx.attr->expression = ((AttributeContext)_localctx).expression.tree; 
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(212);
				((AttributeContext)_localctx).name = match(IDENTIFIER);
				 ((AttributeContext)_localctx).attr =  Alloc<Attribute>(); _localctx.attr->location = SetupFile(); _localctx.attr->name = (((AttributeContext)_localctx).name!=null?((AttributeContext)_localctx).name.getText():null); _localctx.attr->expression = nullptr; 
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class TypeDeclarationContext extends ParserRuleContext {
		public TypeDeclaration type;
		public ExpressionContext arraySize0;
		public Token qual;
		public Token typeName;
		public List<TerminalNode> IDENTIFIER() { return getTokens(GPULangParser.IDENTIFIER); }
		public TerminalNode IDENTIFIER(int i) {
			return getToken(GPULangParser.IDENTIFIER, i);
		}
		public List<TerminalNode> MUL_OP() { return getTokens(GPULangParser.MUL_OP); }
		public TerminalNode MUL_OP(int i) {
			return getToken(GPULangParser.MUL_OP, i);
		}
		public List<TerminalNode> LL() { return getTokens(GPULangParser.LL); }
		public TerminalNode LL(int i) {
			return getToken(GPULangParser.LL, i);
		}
		public List<TerminalNode> RR() { return getTokens(GPULangParser.RR); }
		public TerminalNode RR(int i) {
			return getToken(GPULangParser.RR, i);
		}
		public List<LinePreprocessorEntryContext> linePreprocessorEntry() {
			return getRuleContexts(LinePreprocessorEntryContext.class);
		}
		public LinePreprocessorEntryContext linePreprocessorEntry(int i) {
			return getRuleContext(LinePreprocessorEntryContext.class,i);
		}
		public List<ExpressionContext> expression() {
			return getRuleContexts(ExpressionContext.class);
		}
		public ExpressionContext expression(int i) {
			return getRuleContext(ExpressionContext.class,i);
		}
		public TypeDeclarationContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_typeDeclaration; }
	}

	public final TypeDeclarationContext typeDeclaration() throws RecognitionException {
		TypeDeclarationContext _localctx = new TypeDeclarationContext(_ctx, getState());
		enterRule(_localctx, 18, RULE_typeDeclaration);

		        _localctx.type.type.name = "";
		        Symbol::Location typeRange;
		    
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			 typeRange = BeginLocationRange(); 
			setState(232);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,12,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					{
					setState(230);
					_errHandler.sync(this);
					switch (_input.LA(1)) {
					case MUL_OP:
						{
						setState(217);
						match(MUL_OP);
						 _localctx.type.type.AddModifier(Type::FullType::Modifier::Pointer); 
						}
						break;
					case LL:
						{
						setState(219);
						match(LL);
						 _localctx.type.type.AddModifier(Type::FullType::Modifier::Array); 
						setState(224);
						_errHandler.sync(this);
						_la = _input.LA(1);
						if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 1732271774273372174L) != 0) || ((((_la - 74)) & ~0x3f) == 0 && ((1L << (_la - 74)) & 7787L) != 0)) {
							{
							setState(221);
							((TypeDeclarationContext)_localctx).arraySize0 = expression(0);
							 _localctx.type.type.UpdateValue(((TypeDeclarationContext)_localctx).arraySize0.tree); 
							}
						}

						setState(226);
						match(RR);
						}
						break;
					case IDENTIFIER:
						{
						setState(227);
						((TypeDeclarationContext)_localctx).qual = match(IDENTIFIER);
						 _localctx.type.type.AddQualifier(FixedString((((TypeDeclarationContext)_localctx).qual!=null?((TypeDeclarationContext)_localctx).qual.getText():null))); 
						}
						break;
					case T__2:
						{
						setState(229);
						linePreprocessorEntry();
						}
						break;
					default:
						throw new NoViableAltException(this);
					}
					} 
				}
				setState(234);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,12,_ctx);
			}
			setState(235);
			((TypeDeclarationContext)_localctx).typeName = match(IDENTIFIER);
			 _localctx.type.type.name = (((TypeDeclarationContext)_localctx).typeName!=null?((TypeDeclarationContext)_localctx).typeName.getText():null); _localctx.type.location = EndLocationRange(typeRange); 
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class GenerateContext extends ParserRuleContext {
		public Symbol* sym;
		public StatementContext statement;
		public TerminalNode LESS() { return getToken(GPULangParser.LESS, 0); }
		public TerminalNode GREATER() { return getToken(GPULangParser.GREATER, 0); }
		public List<StatementContext> statement() {
			return getRuleContexts(StatementContext.class);
		}
		public StatementContext statement(int i) {
			return getRuleContext(StatementContext.class,i);
		}
		public GenerateContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_generate; }
	}

	public final GenerateContext generate() throws RecognitionException {
		GenerateContext _localctx = new GenerateContext(_ctx, getState());
		enterRule(_localctx, 20, RULE_generate);

		        PinnedArray<Symbol*> symbols = 0xFFFFF;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(238);
			match(T__5);
			setState(239);
			match(LESS);
			setState(245);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & 1732289366627078158L) != 0) || ((((_la - 74)) & ~0x3f) == 0 && ((1L << (_la - 74)) & 7787L) != 0)) {
				{
				{
				setState(240);
				((GenerateContext)_localctx).statement = statement();
				 symbols.Append(((GenerateContext)_localctx).statement.tree); 
				}
				}
				setState(247);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(248);
			match(GREATER);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class VariablesContext extends ParserRuleContext {
		public FixedArray<Variable*> vars;
		public AnnotationContext annotation;
		public AttributeContext attribute;
		public Token varName;
		public Token varNameN;
		public TypeDeclarationContext typeDeclaration;
		public ExpressionNoCommaContext valueExpr;
		public ExpressionNoCommaContext valueExprN;
		public List<TerminalNode> IDENTIFIER() { return getTokens(GPULangParser.IDENTIFIER); }
		public TerminalNode IDENTIFIER(int i) {
			return getToken(GPULangParser.IDENTIFIER, i);
		}
		public List<LinePreprocessorEntryContext> linePreprocessorEntry() {
			return getRuleContexts(LinePreprocessorEntryContext.class);
		}
		public LinePreprocessorEntryContext linePreprocessorEntry(int i) {
			return getRuleContext(LinePreprocessorEntryContext.class,i);
		}
		public List<AnnotationContext> annotation() {
			return getRuleContexts(AnnotationContext.class);
		}
		public AnnotationContext annotation(int i) {
			return getRuleContext(AnnotationContext.class,i);
		}
		public List<AttributeContext> attribute() {
			return getRuleContexts(AttributeContext.class);
		}
		public AttributeContext attribute(int i) {
			return getRuleContext(AttributeContext.class,i);
		}
		public List<TerminalNode> CO() { return getTokens(GPULangParser.CO); }
		public TerminalNode CO(int i) {
			return getToken(GPULangParser.CO, i);
		}
		public TerminalNode COL() { return getToken(GPULangParser.COL, 0); }
		public TypeDeclarationContext typeDeclaration() {
			return getRuleContext(TypeDeclarationContext.class,0);
		}
		public TerminalNode EQ() { return getToken(GPULangParser.EQ, 0); }
		public List<ExpressionNoCommaContext> expressionNoComma() {
			return getRuleContexts(ExpressionNoCommaContext.class);
		}
		public ExpressionNoCommaContext expressionNoComma(int i) {
			return getRuleContext(ExpressionNoCommaContext.class,i);
		}
		public VariablesContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_variables; }
	}

	public final VariablesContext variables() throws RecognitionException {
		VariablesContext _localctx = new VariablesContext(_ctx, getState());
		enterRule(_localctx, 22, RULE_variables);

		        StackArray<Variable*> list(256);
		        StackArray<Annotation*> annotations(32);
		        StackArray<Attribute*> attributes(32);
		        StackArray<std::string> names(256);
		        StackArray<Expression*> valueExpressions(256);
		        StackArray<Symbol::Location> locations(256);
		        unsigned initCounter = 0;
		        TypeDeclaration type = TypeDeclaration{ .type = Type::FullType{UNDEFINED_TYPE} };
		    
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(253);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__2) {
				{
				{
				setState(250);
				linePreprocessorEntry();
				}
				}
				setState(255);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(261);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==SOBAKA) {
				{
				{
				setState(256);
				((VariablesContext)_localctx).annotation = annotation();
				 if (annotations.Full()) { throw IndexOutOfBoundsException("Maximum of 32 annotations reached"); } annotations.Append(std::move(((VariablesContext)_localctx).annotation.annot)); 
				}
				}
				setState(263);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(267); 
			_errHandler.sync(this);
			_alt = 1;
			do {
				switch (_alt) {
				case 1:
					{
					{
					setState(264);
					((VariablesContext)_localctx).attribute = attribute();
					 if (attributes.Full()) { throw IndexOutOfBoundsException("Maximum of 32 attributes reached"); } attributes.Append(std::move(((VariablesContext)_localctx).attribute.attr)); 
					}
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				setState(269); 
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,16,_ctx);
			} while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER );
			setState(271);
			((VariablesContext)_localctx).varName = match(IDENTIFIER);
			 names.Append((((VariablesContext)_localctx).varName!=null?((VariablesContext)_localctx).varName.getText():null)); valueExpressions.Append(nullptr); locations.Append(SetupFile()); 
			setState(274);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,17,_ctx) ) {
			case 1:
				{
				setState(273);
				linePreprocessorEntry();
				}
				break;
			}
			setState(282);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__2 || _la==CO) {
				{
				setState(280);
				_errHandler.sync(this);
				switch (_input.LA(1)) {
				case CO:
					{
					setState(276);
					match(CO);
					setState(277);
					((VariablesContext)_localctx).varNameN = match(IDENTIFIER);
					 if (names.Full()) { throw IndexOutOfBoundsException("Maximum of 256 variable declarations reached"); } names.Append((((VariablesContext)_localctx).varNameN!=null?((VariablesContext)_localctx).varNameN.getText():null)); valueExpressions.Append(nullptr); locations.Append(SetupFile()); 
					}
					break;
				case T__2:
					{
					setState(279);
					linePreprocessorEntry();
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				}
				setState(284);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(289);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COL) {
				{
				setState(285);
				match(COL);
				setState(286);
				((VariablesContext)_localctx).typeDeclaration = typeDeclaration();
				 type = ((VariablesContext)_localctx).typeDeclaration.type; 
				}
			}

			setState(304);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==EQ) {
				{
				setState(291);
				match(EQ);
				setState(292);
				((VariablesContext)_localctx).valueExpr = expressionNoComma(0);
				 if (initCounter < names.size) { valueExpressions[initCounter++] = ((VariablesContext)_localctx).valueExpr.tree; }  
				setState(301);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while (_la==T__2 || _la==CO) {
					{
					setState(299);
					_errHandler.sync(this);
					switch (_input.LA(1)) {
					case CO:
						{
						setState(294);
						match(CO);
						setState(295);
						((VariablesContext)_localctx).valueExprN = expressionNoComma(0);
						 if (initCounter < names.size) { valueExpressions[initCounter++] = ((VariablesContext)_localctx).valueExprN.tree; }; 
						}
						break;
					case T__2:
						{
						setState(298);
						linePreprocessorEntry();
						}
						break;
					default:
						throw new NoViableAltException(this);
					}
					}
					setState(303);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				}
			}


			        for (size_t i = 0; i < names.size; i++)
			        {
			            Variable* var = Alloc<Variable>(); 
			            var->type = type.type; 
			            var->typeLocation = type.location;
			            var->location = locations[i]; 
			            var->annotations = annotations;
			            var->attributes = attributes;
			            var->name = names[i];
			            var->valueExpression = valueExpressions[i];
			            list.Append(var);
			        }
			        ((VariablesContext)_localctx).vars =  list;
			    
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class StructureDeclarationContext extends ParserRuleContext {
		public Structure* sym;
		public AnnotationContext annotation;
		public AttributeContext attribute;
		public Token name;
		public TerminalNode IDENTIFIER() { return getToken(GPULangParser.IDENTIFIER, 0); }
		public List<LinePreprocessorEntryContext> linePreprocessorEntry() {
			return getRuleContexts(LinePreprocessorEntryContext.class);
		}
		public LinePreprocessorEntryContext linePreprocessorEntry(int i) {
			return getRuleContext(LinePreprocessorEntryContext.class,i);
		}
		public List<AnnotationContext> annotation() {
			return getRuleContexts(AnnotationContext.class);
		}
		public AnnotationContext annotation(int i) {
			return getRuleContext(AnnotationContext.class,i);
		}
		public List<AttributeContext> attribute() {
			return getRuleContexts(AttributeContext.class);
		}
		public AttributeContext attribute(int i) {
			return getRuleContext(AttributeContext.class,i);
		}
		public StructureDeclarationContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_structureDeclaration; }
	}

	public final StructureDeclarationContext structureDeclaration() throws RecognitionException {
		StructureDeclarationContext _localctx = new StructureDeclarationContext(_ctx, getState());
		enterRule(_localctx, 24, RULE_structureDeclaration);

		        ((StructureDeclarationContext)_localctx).sym =  nullptr;
		        StackArray<Annotation*> annotations(32);
		        StackArray<Attribute*> attributes(32);
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(311);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__2) {
				{
				{
				setState(308);
				linePreprocessorEntry();
				}
				}
				setState(313);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(319);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==SOBAKA) {
				{
				{
				setState(314);
				((StructureDeclarationContext)_localctx).annotation = annotation();
				 if (annotations.Full()) { throw IndexOutOfBoundsException("Maximum of 32 annotations reached"); } annotations.Append(std::move(((StructureDeclarationContext)_localctx).annotation.annot)); 
				}
				}
				setState(321);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(327);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==IDENTIFIER) {
				{
				{
				setState(322);
				((StructureDeclarationContext)_localctx).attribute = attribute();
				 if (attributes.Full()) { throw IndexOutOfBoundsException("Maximum of 32 attributes reached"); } attributes.Append(std::move(((StructureDeclarationContext)_localctx).attribute.attr)); 
				}
				}
				setState(329);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(330);
			match(T__6);
			setState(331);
			((StructureDeclarationContext)_localctx).name = match(IDENTIFIER);
			 
			        ((StructureDeclarationContext)_localctx).sym =  Alloc<Structure>();
			        _localctx.sym->name = (((StructureDeclarationContext)_localctx).name!=null?((StructureDeclarationContext)_localctx).name.getText():null); 
			        _localctx.sym->annotations = annotations;
			        _localctx.sym->attributes = attributes;
			        _localctx.sym->location = SetupFile();
			    
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class StructureContext extends ParserRuleContext {
		public Structure* sym;
		public StructureDeclarationContext structureDeclaration;
		public Token varName;
		public ExpressionContext arraySize0;
		public Token varTypeName;
		public StructureDeclarationContext structureDeclaration() {
			return getRuleContext(StructureDeclarationContext.class,0);
		}
		public TerminalNode LB() { return getToken(GPULangParser.LB, 0); }
		public TerminalNode RB() { return getToken(GPULangParser.RB, 0); }
		public List<TerminalNode> COL() { return getTokens(GPULangParser.COL); }
		public TerminalNode COL(int i) {
			return getToken(GPULangParser.COL, i);
		}
		public List<TerminalNode> SC() { return getTokens(GPULangParser.SC); }
		public TerminalNode SC(int i) {
			return getToken(GPULangParser.SC, i);
		}
		public List<LinePreprocessorEntryContext> linePreprocessorEntry() {
			return getRuleContexts(LinePreprocessorEntryContext.class);
		}
		public LinePreprocessorEntryContext linePreprocessorEntry(int i) {
			return getRuleContext(LinePreprocessorEntryContext.class,i);
		}
		public List<TerminalNode> IDENTIFIER() { return getTokens(GPULangParser.IDENTIFIER); }
		public TerminalNode IDENTIFIER(int i) {
			return getToken(GPULangParser.IDENTIFIER, i);
		}
		public List<TerminalNode> MUL_OP() { return getTokens(GPULangParser.MUL_OP); }
		public TerminalNode MUL_OP(int i) {
			return getToken(GPULangParser.MUL_OP, i);
		}
		public List<TerminalNode> LL() { return getTokens(GPULangParser.LL); }
		public TerminalNode LL(int i) {
			return getToken(GPULangParser.LL, i);
		}
		public List<TerminalNode> RR() { return getTokens(GPULangParser.RR); }
		public TerminalNode RR(int i) {
			return getToken(GPULangParser.RR, i);
		}
		public List<ExpressionContext> expression() {
			return getRuleContexts(ExpressionContext.class);
		}
		public ExpressionContext expression(int i) {
			return getRuleContext(ExpressionContext.class,i);
		}
		public StructureContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_structure; }
	}

	public final StructureContext structure() throws RecognitionException {
		StructureContext _localctx = new StructureContext(_ctx, getState());
		enterRule(_localctx, 26, RULE_structure);

		        ((StructureContext)_localctx).sym =  nullptr;
		        StackArray<Symbol*> members(1024);
		        bool isArray = false;
		        Expression* arraySizeExpression = nullptr;
		        std::string instanceName;
		        Symbol::Location varLocation;
		        Type::FullType varType;
		        Symbol::Location varTypeLocation;
		        Symbol::Location typeRange;
		        std::string varName;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(334);
			((StructureContext)_localctx).structureDeclaration = structureDeclaration();
			 ((StructureContext)_localctx).sym =  ((StructureContext)_localctx).structureDeclaration.sym; 
			setState(336);
			match(LB);
			setState(363);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__2 || _la==IDENTIFIER) {
				{
				setState(361);
				_errHandler.sync(this);
				switch (_input.LA(1)) {
				case IDENTIFIER:
					{
					setState(337);
					((StructureContext)_localctx).varName = match(IDENTIFIER);
					 varName = (((StructureContext)_localctx).varName!=null?((StructureContext)_localctx).varName.getText():null); varLocation = SetupFile(); 
					setState(339);
					match(COL);
					 typeRange = BeginLocationRange(); 
					setState(353);
					_errHandler.sync(this);
					_la = _input.LA(1);
					while (_la==LL || _la==MUL_OP) {
						{
						setState(351);
						_errHandler.sync(this);
						switch (_input.LA(1)) {
						case MUL_OP:
							{
							setState(341);
							match(MUL_OP);
							 varType.AddModifier(Type::FullType::Modifier::Pointer); 
							}
							break;
						case LL:
							{
							setState(343);
							match(LL);
							 varType.AddModifier(Type::FullType::Modifier::Array); 
							setState(348);
							_errHandler.sync(this);
							_la = _input.LA(1);
							if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 1732271774273372174L) != 0) || ((((_la - 74)) & ~0x3f) == 0 && ((1L << (_la - 74)) & 7787L) != 0)) {
								{
								setState(345);
								((StructureContext)_localctx).arraySize0 = expression(0);
								 varType.UpdateValue(((StructureContext)_localctx).arraySize0.tree); 
								}
							}

							setState(350);
							match(RR);
							}
							break;
						default:
							throw new NoViableAltException(this);
						}
						}
						setState(355);
						_errHandler.sync(this);
						_la = _input.LA(1);
					}
					setState(356);
					((StructureContext)_localctx).varTypeName = match(IDENTIFIER);
					 if (members.Full()) { throw IndexOutOfBoundsException("Maximum of 1024 struct members reached"); } varType.name = (((StructureContext)_localctx).varTypeName!=null?((StructureContext)_localctx).varTypeName.getText():null); varTypeLocation = EndLocationRange(typeRange); 
					setState(358);
					match(SC);

					                Variable* var = Alloc<Variable>(); 
					                var->type = varType; 
					                var->location = varLocation; 
					                var->typeLocation = varTypeLocation;
					                var->name = varName;
					                var->valueExpression = nullptr;
					                members.Append(var);
					                
					                varType = Type::FullType();
					            
					}
					break;
				case T__2:
					{
					setState(360);
					linePreprocessorEntry();
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				}
				setState(365);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(366);
			match(RB);
			 
			        _localctx.sym->symbols = members; 
			        //_localctx.sym->instanceName = instanceName;
			        //_localctx.sym->isArray = isArray;
			        //_localctx.sym->arraySizeExpression = arraySizeExpression;
			    
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class EnumerationContext extends ParserRuleContext {
		public Enumeration* sym;
		public Token name;
		public TypeDeclarationContext typeDeclaration;
		public Token label;
		public ExpressionContext value;
		public TerminalNode LB() { return getToken(GPULangParser.LB, 0); }
		public TerminalNode RB() { return getToken(GPULangParser.RB, 0); }
		public List<TerminalNode> IDENTIFIER() { return getTokens(GPULangParser.IDENTIFIER); }
		public TerminalNode IDENTIFIER(int i) {
			return getToken(GPULangParser.IDENTIFIER, i);
		}
		public TerminalNode COL() { return getToken(GPULangParser.COL, 0); }
		public TypeDeclarationContext typeDeclaration() {
			return getRuleContext(TypeDeclarationContext.class,0);
		}
		public List<LinePreprocessorEntryContext> linePreprocessorEntry() {
			return getRuleContexts(LinePreprocessorEntryContext.class);
		}
		public LinePreprocessorEntryContext linePreprocessorEntry(int i) {
			return getRuleContext(LinePreprocessorEntryContext.class,i);
		}
		public List<TerminalNode> EQ() { return getTokens(GPULangParser.EQ); }
		public TerminalNode EQ(int i) {
			return getToken(GPULangParser.EQ, i);
		}
		public List<TerminalNode> CO() { return getTokens(GPULangParser.CO); }
		public TerminalNode CO(int i) {
			return getToken(GPULangParser.CO, i);
		}
		public List<ExpressionContext> expression() {
			return getRuleContexts(ExpressionContext.class);
		}
		public ExpressionContext expression(int i) {
			return getRuleContext(ExpressionContext.class,i);
		}
		public EnumerationContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_enumeration; }
	}

	public final EnumerationContext enumeration() throws RecognitionException {
		EnumerationContext _localctx = new EnumerationContext(_ctx, getState());
		enterRule(_localctx, 28, RULE_enumeration);

		        ((EnumerationContext)_localctx).sym =  nullptr;
		        StackArray<FixedString> enumLabels(256);
		        StackArray<Expression*> enumValues(256);
		        StackArray<Symbol::Location> enumLocations(256);
		        std::string name;
		        TypeDeclaration type = TypeDeclaration{ .type = Type::FullType{"u32"} };
		        Symbol::Location location;
		        Symbol::Location labelLocation;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(369);
			match(T__7);
			setState(370);
			((EnumerationContext)_localctx).name = match(IDENTIFIER);
			 name = (((EnumerationContext)_localctx).name!=null?((EnumerationContext)_localctx).name.getText():null); location = SetupFile(); 
			setState(376);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COL) {
				{
				setState(372);
				match(COL);
				setState(373);
				((EnumerationContext)_localctx).typeDeclaration = typeDeclaration();
				 type = ((EnumerationContext)_localctx).typeDeclaration.type; 
				}
			}

			setState(378);
			match(LB);
			setState(408);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case IDENTIFIER:
				{
				setState(379);
				((EnumerationContext)_localctx).label = match(IDENTIFIER);
				 Expression* expr = nullptr; labelLocation = SetupFile(); 
				setState(385);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==EQ) {
					{
					setState(381);
					match(EQ);
					setState(382);
					((EnumerationContext)_localctx).value = expression(0);
					 expr = ((EnumerationContext)_localctx).value.tree; 
					}
				}


				                enumLabels.Append(FixedString((((EnumerationContext)_localctx).label!=null?((EnumerationContext)_localctx).label.getText():null)));
				                enumValues.Append(expr);
				                enumLocations.Append(labelLocation);
				            
				setState(389);
				_errHandler.sync(this);
				switch ( getInterpreter().adaptivePredict(_input,34,_ctx) ) {
				case 1:
					{
					setState(388);
					linePreprocessorEntry();
					}
					break;
				}
				setState(404);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while (_la==T__2 || _la==CO) {
					{
					setState(402);
					_errHandler.sync(this);
					switch (_input.LA(1)) {
					case CO:
						{
						setState(391);
						match(CO);
						setState(392);
						((EnumerationContext)_localctx).label = match(IDENTIFIER);
						 if (enumLabels.Full()) { throw IndexOutOfBoundsException("Maximum of 256 enum labels"); } Expression* expr = nullptr; labelLocation = SetupFile(); 
						setState(398);
						_errHandler.sync(this);
						_la = _input.LA(1);
						if (_la==EQ) {
							{
							setState(394);
							match(EQ);
							setState(395);
							((EnumerationContext)_localctx).value = expression(0);
							 expr = ((EnumerationContext)_localctx).value.tree; 
							}
						}


						                    enumLabels.Append(FixedString((((EnumerationContext)_localctx).label!=null?((EnumerationContext)_localctx).label.getText():null)));
						                    enumValues.Append(expr);
						                    enumLocations.Append(labelLocation);
						                
						}
						break;
					case T__2:
						{
						setState(401);
						linePreprocessorEntry();
						}
						break;
					default:
						throw new NoViableAltException(this);
					}
					}
					setState(406);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				}
				break;
			case T__2:
				{
				setState(407);
				linePreprocessorEntry();
				}
				break;
			case RB:
				break;
			default:
				break;
			}
			setState(410);
			match(RB);

			        ((EnumerationContext)_localctx).sym =  Alloc<Enumeration>();
			        _localctx.sym->name = name;
			        _localctx.sym->type = type.type;
			        _localctx.sym->type.literal = true;
			        _localctx.sym->labels = enumLabels;
			        _localctx.sym->values = enumValues;
			        _localctx.sym->labelLocations = enumLocations;
			        _localctx.sym->location = location;
			    
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ParameterContext extends ParserRuleContext {
		public Variable* sym;
		public AttributeContext attribute;
		public Token varName;
		public TypeDeclarationContext typeDeclaration;
		public ExpressionNoCommaContext valueExpr;
		public TerminalNode COL() { return getToken(GPULangParser.COL, 0); }
		public TypeDeclarationContext typeDeclaration() {
			return getRuleContext(TypeDeclarationContext.class,0);
		}
		public TerminalNode IDENTIFIER() { return getToken(GPULangParser.IDENTIFIER, 0); }
		public List<LinePreprocessorEntryContext> linePreprocessorEntry() {
			return getRuleContexts(LinePreprocessorEntryContext.class);
		}
		public LinePreprocessorEntryContext linePreprocessorEntry(int i) {
			return getRuleContext(LinePreprocessorEntryContext.class,i);
		}
		public List<AttributeContext> attribute() {
			return getRuleContexts(AttributeContext.class);
		}
		public AttributeContext attribute(int i) {
			return getRuleContext(AttributeContext.class,i);
		}
		public TerminalNode EQ() { return getToken(GPULangParser.EQ, 0); }
		public ExpressionNoCommaContext expressionNoComma() {
			return getRuleContext(ExpressionNoCommaContext.class,0);
		}
		public ParameterContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_parameter; }
	}

	public final ParameterContext parameter() throws RecognitionException {
		ParameterContext _localctx = new ParameterContext(_ctx, getState());
		enterRule(_localctx, 30, RULE_parameter);

		        StackArray<Attribute*> attributes(32);
		        std::string name;
		        Expression* valueExpression = nullptr;
		        Symbol::Location location;
		        TypeDeclaration type = TypeDeclaration{ .type = Type::FullType{UNDEFINED_TYPE} };
		    
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(416);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__2) {
				{
				{
				setState(413);
				linePreprocessorEntry();
				}
				}
				setState(418);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(424);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,40,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					{
					{
					setState(419);
					((ParameterContext)_localctx).attribute = attribute();
					 if (attributes.Full()) { throw IndexOutOfBoundsException("Maximum of 32 attributes reached"); } attributes.Append(std::move(((ParameterContext)_localctx).attribute.attr)); 
					}
					} 
				}
				setState(426);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,40,_ctx);
			}
			setState(427);
			((ParameterContext)_localctx).varName = match(IDENTIFIER);
			 name = (((ParameterContext)_localctx).varName!=null?((ParameterContext)_localctx).varName.getText():null); location = SetupFile(); 
			setState(429);
			match(COL);
			setState(430);
			((ParameterContext)_localctx).typeDeclaration = typeDeclaration();
			 type = ((ParameterContext)_localctx).typeDeclaration.type; 
			setState(436);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==EQ) {
				{
				setState(432);
				match(EQ);
				setState(433);
				((ParameterContext)_localctx).valueExpr = expressionNoComma(0);
				 valueExpression = ((ParameterContext)_localctx).valueExpr.tree; 
				}
			}


			            ((ParameterContext)_localctx).sym =  Alloc<Variable>(); 
			            _localctx.sym->type = type.type; 
			            _localctx.sym->typeLocation = type.location;
			            _localctx.sym->location = location; 
			            _localctx.sym->attributes = std::move(attributes);
			            _localctx.sym->name = name;
			            _localctx.sym->valueExpression = valueExpression;
			    
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class FunctionDeclarationContext extends ParserRuleContext {
		public Function* sym;
		public AttributeContext attribute;
		public Token name;
		public ParameterContext arg0;
		public ParameterContext argn;
		public TypeDeclarationContext returnType;
		public TerminalNode LP() { return getToken(GPULangParser.LP, 0); }
		public TerminalNode RP() { return getToken(GPULangParser.RP, 0); }
		public TerminalNode IDENTIFIER() { return getToken(GPULangParser.IDENTIFIER, 0); }
		public TypeDeclarationContext typeDeclaration() {
			return getRuleContext(TypeDeclarationContext.class,0);
		}
		public List<AttributeContext> attribute() {
			return getRuleContexts(AttributeContext.class);
		}
		public AttributeContext attribute(int i) {
			return getRuleContext(AttributeContext.class,i);
		}
		public List<ParameterContext> parameter() {
			return getRuleContexts(ParameterContext.class);
		}
		public ParameterContext parameter(int i) {
			return getRuleContext(ParameterContext.class,i);
		}
		public List<LinePreprocessorEntryContext> linePreprocessorEntry() {
			return getRuleContexts(LinePreprocessorEntryContext.class);
		}
		public LinePreprocessorEntryContext linePreprocessorEntry(int i) {
			return getRuleContext(LinePreprocessorEntryContext.class,i);
		}
		public List<TerminalNode> CO() { return getTokens(GPULangParser.CO); }
		public TerminalNode CO(int i) {
			return getToken(GPULangParser.CO, i);
		}
		public FunctionDeclarationContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_functionDeclaration; }
	}

	public final FunctionDeclarationContext functionDeclaration() throws RecognitionException {
		FunctionDeclarationContext _localctx = new FunctionDeclarationContext(_ctx, getState());
		enterRule(_localctx, 32, RULE_functionDeclaration);

		        ((FunctionDeclarationContext)_localctx).sym =  nullptr;
		        StackArray<Variable*> variables(32);
		        StackArray<Attribute*> attributes(32);
		        Symbol::Location location;
		    
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(445);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,42,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					{
					{
					setState(440);
					((FunctionDeclarationContext)_localctx).attribute = attribute();
					 if (attributes.Full()) { throw IndexOutOfBoundsException("Maximum of 32 attributes reached"); } attributes.Append(std::move(((FunctionDeclarationContext)_localctx).attribute.attr)); 
					}
					} 
				}
				setState(447);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,42,_ctx);
			}
			setState(448);
			((FunctionDeclarationContext)_localctx).name = match(IDENTIFIER);
			 location = SetupFile(); 
			setState(450);
			match(LP);
			setState(466);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__2 || _la==IDENTIFIER) {
				{
				setState(451);
				((FunctionDeclarationContext)_localctx).arg0 = parameter();
				 variables.Append(((FunctionDeclarationContext)_localctx).arg0.sym); 
				setState(454);
				_errHandler.sync(this);
				switch ( getInterpreter().adaptivePredict(_input,43,_ctx) ) {
				case 1:
					{
					setState(453);
					linePreprocessorEntry();
					}
					break;
				}
				setState(463);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while (_la==T__2 || _la==CO) {
					{
					setState(461);
					_errHandler.sync(this);
					switch (_input.LA(1)) {
					case CO:
						{
						setState(456);
						match(CO);
						setState(457);
						((FunctionDeclarationContext)_localctx).argn = parameter();
						 if (variables.Full()) throw IndexOutOfBoundsException("Maximum of 32 variables reached"); variables.Append(((FunctionDeclarationContext)_localctx).argn.sym); 
						}
						break;
					case T__2:
						{
						setState(460);
						linePreprocessorEntry();
						}
						break;
					default:
						throw new NoViableAltException(this);
					}
					}
					setState(465);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				}
			}

			setState(468);
			match(RP);
			setState(469);
			((FunctionDeclarationContext)_localctx).returnType = typeDeclaration();

			        ((FunctionDeclarationContext)_localctx).sym =  Alloc<Function>(); 
			        _localctx.sym->hasBody = false;
			        _localctx.sym->location = location;
			        _localctx.sym->returnType = ((FunctionDeclarationContext)_localctx).returnType.type.type; 
			        _localctx.sym->returnTypeLocation = ((FunctionDeclarationContext)_localctx).returnType.type.location;
			        _localctx.sym->name = (((FunctionDeclarationContext)_localctx).name!=null?((FunctionDeclarationContext)_localctx).name.getText():null); 
			        _localctx.sym->parameters = variables; 
			        _localctx.sym->attributes = attributes;
			    
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class FunctionContext extends ParserRuleContext {
		public Function* sym;
		public FunctionDeclarationContext functionDeclaration;
		public ScopeStatementContext scopeStatement;
		public FunctionDeclarationContext functionDeclaration() {
			return getRuleContext(FunctionDeclarationContext.class,0);
		}
		public ScopeStatementContext scopeStatement() {
			return getRuleContext(ScopeStatementContext.class,0);
		}
		public FunctionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_function; }
	}

	public final FunctionContext function() throws RecognitionException {
		FunctionContext _localctx = new FunctionContext(_ctx, getState());
		enterRule(_localctx, 34, RULE_function);

		        ((FunctionContext)_localctx).sym =  nullptr;
		    
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(472);
			((FunctionContext)_localctx).functionDeclaration = functionDeclaration();
			 ((FunctionContext)_localctx).sym =  ((FunctionContext)_localctx).functionDeclaration.sym; 
			setState(474);
			((FunctionContext)_localctx).scopeStatement = scopeStatement();

			        _localctx.sym->hasBody = true;
			        _localctx.sym->ast = ((FunctionContext)_localctx).scopeStatement.tree;
			    
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ProgramContext extends ParserRuleContext {
		public Program* sym;
		public AnnotationContext annotation;
		public Token name;
		public ExpressionContext assignment;
		public TerminalNode LB() { return getToken(GPULangParser.LB, 0); }
		public TerminalNode RB() { return getToken(GPULangParser.RB, 0); }
		public TerminalNode IDENTIFIER() { return getToken(GPULangParser.IDENTIFIER, 0); }
		public List<AnnotationContext> annotation() {
			return getRuleContexts(AnnotationContext.class);
		}
		public AnnotationContext annotation(int i) {
			return getRuleContext(AnnotationContext.class,i);
		}
		public List<TerminalNode> SC() { return getTokens(GPULangParser.SC); }
		public TerminalNode SC(int i) {
			return getToken(GPULangParser.SC, i);
		}
		public List<ExpressionContext> expression() {
			return getRuleContexts(ExpressionContext.class);
		}
		public ExpressionContext expression(int i) {
			return getRuleContext(ExpressionContext.class,i);
		}
		public ProgramContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_program; }
	}

	public final ProgramContext program() throws RecognitionException {
		ProgramContext _localctx = new ProgramContext(_ctx, getState());
		enterRule(_localctx, 36, RULE_program);

		        ((ProgramContext)_localctx).sym =  nullptr;
		        StackArray<Expression*> entries(32);
		        StackArray<Annotation*> annotations(32);
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(482);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==SOBAKA) {
				{
				{
				setState(477);
				((ProgramContext)_localctx).annotation = annotation();
				 if (annotations.Full()) { throw IndexOutOfBoundsException("Maximum of 32 annotations reached"); } annotations.Append(std::move(((ProgramContext)_localctx).annotation.annot)); 
				}
				}
				setState(484);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(485);
			match(T__8);
			 ((ProgramContext)_localctx).sym =  Alloc<Program>(); 
			setState(487);
			((ProgramContext)_localctx).name = match(IDENTIFIER);
			 _localctx.sym->location = SetupFile(); 
			setState(489);
			match(LB);
			setState(496);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & 1732271774273372174L) != 0) || ((((_la - 74)) & ~0x3f) == 0 && ((1L << (_la - 74)) & 7787L) != 0)) {
				{
				{
				setState(490);
				((ProgramContext)_localctx).assignment = expression(0);
				 if (entries.Full()) throw IndexOutOfBoundsException("Maximum of 32 entries reached"); entries.Append(((ProgramContext)_localctx).assignment.tree); 
				setState(492);
				match(SC);
				}
				}
				setState(498);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(499);
			match(RB);
			 
			        _localctx.sym->name = (((ProgramContext)_localctx).name!=null?((ProgramContext)_localctx).name.getText():null);
			        _localctx.sym->annotations = annotations;
			        _localctx.sym->entries = entries;
			    
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class SamplerContext extends ParserRuleContext {
		public SamplerState* sym;
		public AnnotationContext annotation;
		public AttributeContext attribute;
		public Token name;
		public ExpressionContext assign;
		public TerminalNode LB() { return getToken(GPULangParser.LB, 0); }
		public TerminalNode RB() { return getToken(GPULangParser.RB, 0); }
		public TerminalNode IDENTIFIER() { return getToken(GPULangParser.IDENTIFIER, 0); }
		public List<TerminalNode> SC() { return getTokens(GPULangParser.SC); }
		public TerminalNode SC(int i) {
			return getToken(GPULangParser.SC, i);
		}
		public List<ExpressionContext> expression() {
			return getRuleContexts(ExpressionContext.class);
		}
		public ExpressionContext expression(int i) {
			return getRuleContext(ExpressionContext.class,i);
		}
		public List<AnnotationContext> annotation() {
			return getRuleContexts(AnnotationContext.class);
		}
		public AnnotationContext annotation(int i) {
			return getRuleContext(AnnotationContext.class,i);
		}
		public List<AttributeContext> attribute() {
			return getRuleContexts(AttributeContext.class);
		}
		public AttributeContext attribute(int i) {
			return getRuleContext(AttributeContext.class,i);
		}
		public SamplerContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_sampler; }
	}

	public final SamplerContext sampler() throws RecognitionException {
		SamplerContext _localctx = new SamplerContext(_ctx, getState());
		enterRule(_localctx, 38, RULE_sampler);

		        StackArray<Attribute*> attributes(32);
		        StackArray<Annotation*> annotations(32);
		        StackArray<Expression*> entries(32);
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			{
			setState(507);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==SOBAKA) {
				{
				{
				setState(502);
				((SamplerContext)_localctx).annotation = annotation();
				 if (annotations.Full()) throw IndexOutOfBoundsException("Maximum of 32 annotations reached"); annotations.Append(std::move(((SamplerContext)_localctx).annotation.annot)); 
				}
				}
				setState(509);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(515);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==IDENTIFIER) {
				{
				{
				setState(510);
				((SamplerContext)_localctx).attribute = attribute();
				 if (attributes.Full()) throw IndexOutOfBoundsException("Maximum of 32 attributes reached"); attributes.Append(std::move(((SamplerContext)_localctx).attribute.attr)); 
				}
				}
				setState(517);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(518);
			match(T__9);
			 ((SamplerContext)_localctx).sym =  Alloc<SamplerState>(); _localctx.sym->isImmutable = true; 
			}
			setState(521);
			((SamplerContext)_localctx).name = match(IDENTIFIER);
			 _localctx.sym->location = SetupFile(); 
			setState(523);
			match(LB);
			setState(530);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & 1732271774273372174L) != 0) || ((((_la - 74)) & ~0x3f) == 0 && ((1L << (_la - 74)) & 7787L) != 0)) {
				{
				{
				setState(524);
				((SamplerContext)_localctx).assign = expression(0);
				 if (entries.Full()) throw IndexOutOfBoundsException("Maximum of 32 entries reached"); entries.Append(((SamplerContext)_localctx).assign.tree); 
				setState(526);
				match(SC);
				}
				}
				setState(532);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(533);
			match(RB);

			        _localctx.sym->name = (((SamplerContext)_localctx).name!=null?((SamplerContext)_localctx).name.getText():null);
			        _localctx.sym->attributes = attributes;
			        _localctx.sym->annotations = annotations;
			        _localctx.sym->entries = entries;
			    
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class StateContext extends ParserRuleContext {
		public State* sym;
		public Token name;
		public ExpressionContext assign;
		public TerminalNode LB() { return getToken(GPULangParser.LB, 0); }
		public TerminalNode RB() { return getToken(GPULangParser.RB, 0); }
		public TerminalNode IDENTIFIER() { return getToken(GPULangParser.IDENTIFIER, 0); }
		public List<TerminalNode> SC() { return getTokens(GPULangParser.SC); }
		public TerminalNode SC(int i) {
			return getToken(GPULangParser.SC, i);
		}
		public List<ExpressionContext> expression() {
			return getRuleContexts(ExpressionContext.class);
		}
		public ExpressionContext expression(int i) {
			return getRuleContext(ExpressionContext.class,i);
		}
		public StateContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_state; }
	}

	public final StateContext state() throws RecognitionException {
		StateContext _localctx = new StateContext(_ctx, getState());
		enterRule(_localctx, 40, RULE_state);

		        StackArray<Expression*> entries(32);
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			{
			setState(536);
			match(T__10);
			 ((StateContext)_localctx).sym =  Alloc<RenderState>(); 
			}
			setState(539);
			((StateContext)_localctx).name = match(IDENTIFIER);
			 _localctx.sym->location = SetupFile(); 
			setState(541);
			match(LB);
			setState(548);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & 1732271774273372174L) != 0) || ((((_la - 74)) & ~0x3f) == 0 && ((1L << (_la - 74)) & 7787L) != 0)) {
				{
				{
				setState(542);
				((StateContext)_localctx).assign = expression(0);
				 if (entries.Full()) throw IndexOutOfBoundsException("Maximum of 32 entries reached"); entries.Append(((StateContext)_localctx).assign.tree); 
				setState(544);
				match(SC);
				}
				}
				setState(550);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(551);
			match(RB);

			        _localctx.sym->name = (((StateContext)_localctx).name!=null?((StateContext)_localctx).name.getText():null);
			        _localctx.sym->entries = entries;
			    
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class StatementContext extends ParserRuleContext {
		public Statement* tree;
		public IfStatementContext ifStatement;
		public ScopeStatementContext scopeStatement;
		public ForStatementContext forStatement;
		public WhileStatementContext whileStatement;
		public SwitchStatementContext switchStatement;
		public TerminateStatementContext terminateStatement;
		public ContinueStatementContext continueStatement;
		public BreakStatementContext breakStatement;
		public ExpressionStatementContext expressionStatement;
		public IfStatementContext ifStatement() {
			return getRuleContext(IfStatementContext.class,0);
		}
		public ScopeStatementContext scopeStatement() {
			return getRuleContext(ScopeStatementContext.class,0);
		}
		public ForStatementContext forStatement() {
			return getRuleContext(ForStatementContext.class,0);
		}
		public WhileStatementContext whileStatement() {
			return getRuleContext(WhileStatementContext.class,0);
		}
		public SwitchStatementContext switchStatement() {
			return getRuleContext(SwitchStatementContext.class,0);
		}
		public TerminateStatementContext terminateStatement() {
			return getRuleContext(TerminateStatementContext.class,0);
		}
		public ContinueStatementContext continueStatement() {
			return getRuleContext(ContinueStatementContext.class,0);
		}
		public BreakStatementContext breakStatement() {
			return getRuleContext(BreakStatementContext.class,0);
		}
		public ExpressionStatementContext expressionStatement() {
			return getRuleContext(ExpressionStatementContext.class,0);
		}
		public TerminalNode SC() { return getToken(GPULangParser.SC, 0); }
		public StatementContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_statement; }
	}

	public final StatementContext statement() throws RecognitionException {
		StatementContext _localctx = new StatementContext(_ctx, getState());
		enterRule(_localctx, 42, RULE_statement);

		        ((StatementContext)_localctx).tree =  nullptr;
		    
		try {
			setState(582);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__11:
				enterOuterAlt(_localctx, 1);
				{
				setState(554);
				((StatementContext)_localctx).ifStatement = ifStatement();
				 ((StatementContext)_localctx).tree =  ((StatementContext)_localctx).ifStatement.tree; 
				}
				break;
			case LB:
				enterOuterAlt(_localctx, 2);
				{
				setState(557);
				((StatementContext)_localctx).scopeStatement = scopeStatement();
				 ((StatementContext)_localctx).tree =  ((StatementContext)_localctx).scopeStatement.tree; 
				}
				break;
			case T__13:
				enterOuterAlt(_localctx, 3);
				{
				setState(560);
				((StatementContext)_localctx).forStatement = forStatement();
				 ((StatementContext)_localctx).tree =  ((StatementContext)_localctx).forStatement.tree; 
				}
				break;
			case T__16:
			case T__17:
				enterOuterAlt(_localctx, 4);
				{
				setState(563);
				((StatementContext)_localctx).whileStatement = whileStatement();
				 ((StatementContext)_localctx).tree =  ((StatementContext)_localctx).whileStatement.tree; 
				}
				break;
			case T__23:
				enterOuterAlt(_localctx, 5);
				{
				setState(566);
				((StatementContext)_localctx).switchStatement = switchStatement();
				 ((StatementContext)_localctx).tree =  ((StatementContext)_localctx).switchStatement.tree; 
				}
				break;
			case T__18:
			case T__19:
			case T__20:
			case T__21:
				enterOuterAlt(_localctx, 6);
				{
				setState(569);
				((StatementContext)_localctx).terminateStatement = terminateStatement();
				 ((StatementContext)_localctx).tree =  ((StatementContext)_localctx).terminateStatement.tree; 
				}
				break;
			case T__22:
				enterOuterAlt(_localctx, 7);
				{
				setState(572);
				((StatementContext)_localctx).continueStatement = continueStatement();
				 ((StatementContext)_localctx).tree =  ((StatementContext)_localctx).continueStatement.tree; 
				}
				break;
			case T__26:
				enterOuterAlt(_localctx, 8);
				{
				setState(575);
				((StatementContext)_localctx).breakStatement = breakStatement();
				 ((StatementContext)_localctx).tree =  ((StatementContext)_localctx).breakStatement.tree; 
				}
				break;
			case T__0:
			case T__1:
			case T__2:
			case T__27:
			case T__28:
			case LP:
			case LL:
			case NOT:
			case QO:
			case CONJUGATE:
			case Q:
			case ADD_OP:
			case SUB_OP:
			case MUL_OP:
			case INTEGERLITERAL:
			case UINTEGERLITERAL:
			case FLOATLITERAL:
			case DOUBLELITERAL:
			case HEX:
			case IDENTIFIER:
				enterOuterAlt(_localctx, 9);
				{
				setState(578);
				((StatementContext)_localctx).expressionStatement = expressionStatement();
				setState(579);
				match(SC);
				 ((StatementContext)_localctx).tree =  ((StatementContext)_localctx).expressionStatement.tree; 
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ExpressionStatementContext extends ParserRuleContext {
		public Statement* tree;
		public ExpressionContext expression;
		public ExpressionContext expression() {
			return getRuleContext(ExpressionContext.class,0);
		}
		public ExpressionStatementContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_expressionStatement; }
	}

	public final ExpressionStatementContext expressionStatement() throws RecognitionException {
		ExpressionStatementContext _localctx = new ExpressionStatementContext(_ctx, getState());
		enterRule(_localctx, 44, RULE_expressionStatement);

		        ((ExpressionStatementContext)_localctx).tree =  nullptr;
		    
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(584);
			((ExpressionStatementContext)_localctx).expression = expression(0);

			        ((ExpressionStatementContext)_localctx).tree =  Alloc<ExpressionStatement>(((ExpressionStatementContext)_localctx).expression.tree);
			        _localctx.tree->location = ((ExpressionStatementContext)_localctx).expression.tree->location;
			    
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class IfStatementContext extends ParserRuleContext {
		public Statement* tree;
		public ExpressionContext condition;
		public StatementContext ifBody;
		public StatementContext elseBody;
		public TerminalNode LP() { return getToken(GPULangParser.LP, 0); }
		public TerminalNode RP() { return getToken(GPULangParser.RP, 0); }
		public ExpressionContext expression() {
			return getRuleContext(ExpressionContext.class,0);
		}
		public List<StatementContext> statement() {
			return getRuleContexts(StatementContext.class);
		}
		public StatementContext statement(int i) {
			return getRuleContext(StatementContext.class,i);
		}
		public IfStatementContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_ifStatement; }
	}

	public final IfStatementContext ifStatement() throws RecognitionException {
		IfStatementContext _localctx = new IfStatementContext(_ctx, getState());
		enterRule(_localctx, 46, RULE_ifStatement);

		        ((IfStatementContext)_localctx).tree =  nullptr;
		        Expression* condition = nullptr;
		        Statement* ifBody = nullptr;
		        Statement* elseBody = nullptr;
		        Symbol::Location location;
		    
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(587);
			match(T__11);
			 location = SetupFile(); 
			setState(589);
			match(LP);
			setState(590);
			((IfStatementContext)_localctx).condition = expression(0);
			 condition = ((IfStatementContext)_localctx).condition.tree; 
			setState(592);
			match(RP);
			setState(593);
			((IfStatementContext)_localctx).ifBody = statement();
			 ifBody = ((IfStatementContext)_localctx).ifBody.tree; 
			setState(599);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,54,_ctx) ) {
			case 1:
				{
				setState(595);
				match(T__12);
				setState(596);
				((IfStatementContext)_localctx).elseBody = statement();
				 elseBody = ((IfStatementContext)_localctx).elseBody.tree; 
				}
				break;
			}

			        ((IfStatementContext)_localctx).tree =  Alloc<IfStatement>(condition, ifBody, elseBody);
			        _localctx.tree->location = location;
			    
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ForStatementContext extends ParserRuleContext {
		public Statement* tree;
		public VariablesContext variables;
		public ExpressionContext condition;
		public ExpressionContext loop;
		public AttributeContext attribute;
		public StatementContext content;
		public TerminalNode LP() { return getToken(GPULangParser.LP, 0); }
		public List<TerminalNode> SC() { return getTokens(GPULangParser.SC); }
		public TerminalNode SC(int i) {
			return getToken(GPULangParser.SC, i);
		}
		public TerminalNode RP() { return getToken(GPULangParser.RP, 0); }
		public StatementContext statement() {
			return getRuleContext(StatementContext.class,0);
		}
		public VariablesContext variables() {
			return getRuleContext(VariablesContext.class,0);
		}
		public List<AttributeContext> attribute() {
			return getRuleContexts(AttributeContext.class);
		}
		public AttributeContext attribute(int i) {
			return getRuleContext(AttributeContext.class,i);
		}
		public List<ExpressionContext> expression() {
			return getRuleContexts(ExpressionContext.class);
		}
		public ExpressionContext expression(int i) {
			return getRuleContext(ExpressionContext.class,i);
		}
		public ForStatementContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_forStatement; }
	}

	public final ForStatementContext forStatement() throws RecognitionException {
		ForStatementContext _localctx = new ForStatementContext(_ctx, getState());
		enterRule(_localctx, 48, RULE_forStatement);

		        ((ForStatementContext)_localctx).tree =  nullptr;
		        FixedArray<Variable*> declarations;
		        Expression* conditionExpression = nullptr;
		        Expression* loopExpression = nullptr;
		        Statement* contents = nullptr;
		        StackArray<Attribute*> attributes(32);
		        Symbol::Location location;
		    
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(603);
			match(T__13);
			 location = SetupFile(); 
			setState(605);
			match(LP);
			setState(609);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__2 || _la==SOBAKA || _la==IDENTIFIER) {
				{
				setState(606);
				((ForStatementContext)_localctx).variables = variables();
				 declarations = ((ForStatementContext)_localctx).variables.vars; 
				}
			}

			setState(611);
			match(SC);
			setState(615);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 1732271774273372174L) != 0) || ((((_la - 74)) & ~0x3f) == 0 && ((1L << (_la - 74)) & 7787L) != 0)) {
				{
				setState(612);
				((ForStatementContext)_localctx).condition = expression(0);
				 conditionExpression = ((ForStatementContext)_localctx).condition.tree; 
				}
			}

			setState(617);
			match(SC);
			setState(621);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 1732271774273372174L) != 0) || ((((_la - 74)) & ~0x3f) == 0 && ((1L << (_la - 74)) & 7787L) != 0)) {
				{
				setState(618);
				((ForStatementContext)_localctx).loop = expression(0);
				 loopExpression = ((ForStatementContext)_localctx).loop.tree; 
				}
			}

			setState(623);
			match(RP);
			setState(629);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,58,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					{
					{
					setState(624);
					((ForStatementContext)_localctx).attribute = attribute();
					 if (attributes.Full()) { throw IndexOutOfBoundsException("Maximum of 32 attributes reached"); } attributes.Append(std::move(((ForStatementContext)_localctx).attribute.attr)); 
					}
					} 
				}
				setState(631);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,58,_ctx);
			}
			setState(632);
			((ForStatementContext)_localctx).content = statement();
			 contents = ((ForStatementContext)_localctx).content.tree; 

			        ((ForStatementContext)_localctx).tree =  Alloc<ForStatement>(declarations, conditionExpression, loopExpression, contents);
			        _localctx.tree->location = location;
			        _localctx.tree->attributes = std::move(attributes);
			    
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ForRangeStatementContext extends ParserRuleContext {
		public Statement* tree;
		public Token iterator;
		public Token start;
		public Token end;
		public StatementContext content;
		public TerminalNode LP() { return getToken(GPULangParser.LP, 0); }
		public TerminalNode COL() { return getToken(GPULangParser.COL, 0); }
		public TerminalNode RP() { return getToken(GPULangParser.RP, 0); }
		public List<TerminalNode> IDENTIFIER() { return getTokens(GPULangParser.IDENTIFIER); }
		public TerminalNode IDENTIFIER(int i) {
			return getToken(GPULangParser.IDENTIFIER, i);
		}
		public StatementContext statement() {
			return getRuleContext(StatementContext.class,0);
		}
		public ForRangeStatementContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_forRangeStatement; }
	}

	public final ForRangeStatementContext forRangeStatement() throws RecognitionException {
		ForRangeStatementContext _localctx = new ForRangeStatementContext(_ctx, getState());
		enterRule(_localctx, 50, RULE_forRangeStatement);

		        ((ForRangeStatementContext)_localctx).tree =  nullptr;
		        Statement* contents = nullptr;
		        Symbol::Location location;
		    
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(636);
			match(T__13);
			 location = SetupFile(); 
			setState(638);
			match(LP);
			setState(639);
			((ForRangeStatementContext)_localctx).iterator = match(IDENTIFIER);
			setState(640);
			match(COL);
			setState(641);
			((ForRangeStatementContext)_localctx).start = match(IDENTIFIER);
			setState(642);
			match(T__14);
			setState(643);
			((ForRangeStatementContext)_localctx).end = match(IDENTIFIER);
			setState(644);
			match(RP);
			setState(645);
			((ForRangeStatementContext)_localctx).content = statement();
			 contents = ((ForRangeStatementContext)_localctx).content.tree; 


			    
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ForUniformValueStatementContext extends ParserRuleContext {
		public Statement* tree;
		public StatementContext content;
		public TerminalNode LP() { return getToken(GPULangParser.LP, 0); }
		public ExpressionContext expression() {
			return getRuleContext(ExpressionContext.class,0);
		}
		public TerminalNode RP() { return getToken(GPULangParser.RP, 0); }
		public StatementContext statement() {
			return getRuleContext(StatementContext.class,0);
		}
		public ForUniformValueStatementContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_forUniformValueStatement; }
	}

	public final ForUniformValueStatementContext forUniformValueStatement() throws RecognitionException {
		ForUniformValueStatementContext _localctx = new ForUniformValueStatementContext(_ctx, getState());
		enterRule(_localctx, 52, RULE_forUniformValueStatement);

		        ((ForUniformValueStatementContext)_localctx).tree =  nullptr;
		        Statement* contents = nullptr;
		        Symbol::Location location;
		    
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(649);
			match(T__15);
			 location = SetupFile(); 
			setState(651);
			match(LP);
			setState(652);
			expression(0);
			setState(653);
			match(RP);
			setState(654);
			((ForUniformValueStatementContext)_localctx).content = statement();
			 contents = ((ForUniformValueStatementContext)_localctx).content.tree; 

			    
			    
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class WhileStatementContext extends ParserRuleContext {
		public Statement* tree;
		public ExpressionContext condition;
		public StatementContext content;
		public TerminalNode LP() { return getToken(GPULangParser.LP, 0); }
		public TerminalNode RP() { return getToken(GPULangParser.RP, 0); }
		public ExpressionContext expression() {
			return getRuleContext(ExpressionContext.class,0);
		}
		public StatementContext statement() {
			return getRuleContext(StatementContext.class,0);
		}
		public TerminalNode SC() { return getToken(GPULangParser.SC, 0); }
		public WhileStatementContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_whileStatement; }
	}

	public final WhileStatementContext whileStatement() throws RecognitionException {
		WhileStatementContext _localctx = new WhileStatementContext(_ctx, getState());
		enterRule(_localctx, 54, RULE_whileStatement);

		        ((WhileStatementContext)_localctx).tree =  nullptr;
		        Expression* conditionExpression = nullptr;
		        Statement* contents = nullptr;
		        bool isDoWhile = false;
		        Symbol::Location location;
		    
		try {
			setState(680);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__16:
				enterOuterAlt(_localctx, 1);
				{
				setState(658);
				match(T__16);
				 location = SetupFile(); 
				setState(660);
				match(LP);
				setState(661);
				((WhileStatementContext)_localctx).condition = expression(0);
				 conditionExpression = ((WhileStatementContext)_localctx).condition.tree; 
				setState(663);
				match(RP);
				setState(664);
				((WhileStatementContext)_localctx).content = statement();
				 contents = ((WhileStatementContext)_localctx).content.tree; 

				        ((WhileStatementContext)_localctx).tree =  Alloc<WhileStatement>(conditionExpression, contents, isDoWhile);
				        _localctx.tree->location = location;
				    
				}
				break;
			case T__17:
				enterOuterAlt(_localctx, 2);
				{
				setState(668);
				match(T__17);
				 location = SetupFile(); 
				setState(670);
				((WhileStatementContext)_localctx).content = statement();
				 contents = ((WhileStatementContext)_localctx).content.tree; isDoWhile = true; 
				setState(672);
				match(T__16);
				setState(673);
				match(LP);
				setState(674);
				((WhileStatementContext)_localctx).condition = expression(0);
				 conditionExpression = ((WhileStatementContext)_localctx).condition.tree; 
				setState(676);
				match(RP);
				setState(677);
				match(SC);

				        ((WhileStatementContext)_localctx).tree =  Alloc<WhileStatement>(conditionExpression, contents, isDoWhile);
				        _localctx.tree->location = location;
				    
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ScopeStatementContext extends ParserRuleContext {
		public ScopeStatement* tree;
		public VariablesContext variables;
		public StatementContext statement;
		public TerminalNode LB() { return getToken(GPULangParser.LB, 0); }
		public TerminalNode RB() { return getToken(GPULangParser.RB, 0); }
		public List<VariablesContext> variables() {
			return getRuleContexts(VariablesContext.class);
		}
		public VariablesContext variables(int i) {
			return getRuleContext(VariablesContext.class,i);
		}
		public List<TerminalNode> SC() { return getTokens(GPULangParser.SC); }
		public TerminalNode SC(int i) {
			return getToken(GPULangParser.SC, i);
		}
		public List<StatementContext> statement() {
			return getRuleContexts(StatementContext.class);
		}
		public StatementContext statement(int i) {
			return getRuleContext(StatementContext.class,i);
		}
		public List<LinePreprocessorEntryContext> linePreprocessorEntry() {
			return getRuleContexts(LinePreprocessorEntryContext.class);
		}
		public LinePreprocessorEntryContext linePreprocessorEntry(int i) {
			return getRuleContext(LinePreprocessorEntryContext.class,i);
		}
		public ScopeStatementContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_scopeStatement; }
	}

	public final ScopeStatementContext scopeStatement() throws RecognitionException {
		ScopeStatementContext _localctx = new ScopeStatementContext(_ctx, getState());
		enterRule(_localctx, 56, RULE_scopeStatement);

		        ((ScopeStatementContext)_localctx).tree =  nullptr;
		        PinnedArray<Symbol*> contents(0xFFFFFF);
			    std::vector<Expression*> unfinished;
		        Symbol::Location location;
		        Symbol::Location ends;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(682);
			match(LB);
			 location = SetupFile(); 
			setState(694);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & 1732289366627078158L) != 0) || ((((_la - 73)) & ~0x3f) == 0 && ((1L << (_la - 73)) & 15575L) != 0)) {
				{
				setState(692);
				_errHandler.sync(this);
				switch ( getInterpreter().adaptivePredict(_input,60,_ctx) ) {
				case 1:
					{
					setState(684);
					((ScopeStatementContext)_localctx).variables = variables();
					setState(685);
					match(SC);
					 for(Variable* var : ((ScopeStatementContext)_localctx).variables.vars) { contents.Append(var); } 
					}
					break;
				case 2:
					{
					setState(688);
					((ScopeStatementContext)_localctx).statement = statement();
					 contents.Append(((ScopeStatementContext)_localctx).statement.tree); 
					}
					break;
				case 3:
					{
					setState(691);
					linePreprocessorEntry();
					}
					break;
				}
				}
				setState(696);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(697);
			match(RB);
			 ends = SetupFile(); 

			        ((ScopeStatementContext)_localctx).tree =  Alloc<ScopeStatement>(std::move(contents), unfinished);
			        _localctx.tree->ends = ends;
			        _localctx.tree->location = location;
			    
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class TerminateStatementContext extends ParserRuleContext {
		public Statement* tree;
		public ExpressionContext value;
		public TerminalNode SC() { return getToken(GPULangParser.SC, 0); }
		public ExpressionContext expression() {
			return getRuleContext(ExpressionContext.class,0);
		}
		public TerminateStatementContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_terminateStatement; }
	}

	public final TerminateStatementContext terminateStatement() throws RecognitionException {
		TerminateStatementContext _localctx = new TerminateStatementContext(_ctx, getState());
		enterRule(_localctx, 58, RULE_terminateStatement);

		        ((TerminateStatementContext)_localctx).tree =  nullptr;
		        Expression* returnValue = nullptr;
		        Symbol::Location location;
		    
		int _la;
		try {
			setState(722);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__18:
				enterOuterAlt(_localctx, 1);
				{
				setState(701);
				match(T__18);
				 location = SetupFile(); 
				setState(706);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 1732271774273372174L) != 0) || ((((_la - 74)) & ~0x3f) == 0 && ((1L << (_la - 74)) & 7787L) != 0)) {
					{
					setState(703);
					((TerminateStatementContext)_localctx).value = expression(0);
					 returnValue = ((TerminateStatementContext)_localctx).value.tree; 
					}
				}

				setState(708);
				match(SC);

				        ((TerminateStatementContext)_localctx).tree =  Alloc<TerminateStatement>(returnValue, TerminateStatement::TerminationType::Return);
				        _localctx.tree->location = location;
				    
				}
				break;
			case T__19:
				enterOuterAlt(_localctx, 2);
				{
				setState(710);
				match(T__19);
				 location = SetupFile(); 
				setState(712);
				match(SC);

				      ((TerminateStatementContext)_localctx).tree =  Alloc<TerminateStatement>(returnValue, TerminateStatement::TerminationType::Discard);
				      _localctx.tree->location = location;
				    
				}
				break;
			case T__20:
				enterOuterAlt(_localctx, 3);
				{
				setState(714);
				match(T__20);
				 location = SetupFile(); 
				setState(716);
				match(SC);

				      ((TerminateStatementContext)_localctx).tree =  Alloc<TerminateStatement>(returnValue, TerminateStatement::TerminationType::RayIgnoreIntersection);
				      _localctx.tree->location = location;
				    
				}
				break;
			case T__21:
				enterOuterAlt(_localctx, 4);
				{
				setState(718);
				match(T__21);
				 location = SetupFile(); 
				setState(720);
				match(SC);

				      ((TerminateStatementContext)_localctx).tree =  Alloc<TerminateStatement>(returnValue, TerminateStatement::TerminationType::RayTerminate);
				      _localctx.tree->location = location;
				    
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ContinueStatementContext extends ParserRuleContext {
		public Statement* tree;
		public TerminalNode SC() { return getToken(GPULangParser.SC, 0); }
		public ContinueStatementContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_continueStatement; }
	}

	public final ContinueStatementContext continueStatement() throws RecognitionException {
		ContinueStatementContext _localctx = new ContinueStatementContext(_ctx, getState());
		enterRule(_localctx, 60, RULE_continueStatement);

		        ((ContinueStatementContext)_localctx).tree =  nullptr;
		        Symbol::Location location;
		    
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(724);
			match(T__22);
			 location = SetupFile(); 
			setState(726);
			match(SC);

			        ((ContinueStatementContext)_localctx).tree =  Alloc<ContinueStatement>();
			        _localctx.tree->location = location;
			    
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class SwitchStatementContext extends ParserRuleContext {
		public Statement* tree;
		public ExpressionContext expression;
		public StatementContext statement;
		public TerminalNode LP() { return getToken(GPULangParser.LP, 0); }
		public List<ExpressionContext> expression() {
			return getRuleContexts(ExpressionContext.class);
		}
		public ExpressionContext expression(int i) {
			return getRuleContext(ExpressionContext.class,i);
		}
		public TerminalNode RP() { return getToken(GPULangParser.RP, 0); }
		public TerminalNode LB() { return getToken(GPULangParser.LB, 0); }
		public TerminalNode RB() { return getToken(GPULangParser.RB, 0); }
		public List<TerminalNode> COL() { return getTokens(GPULangParser.COL); }
		public TerminalNode COL(int i) {
			return getToken(GPULangParser.COL, i);
		}
		public List<StatementContext> statement() {
			return getRuleContexts(StatementContext.class);
		}
		public StatementContext statement(int i) {
			return getRuleContext(StatementContext.class,i);
		}
		public SwitchStatementContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_switchStatement; }
	}

	public final SwitchStatementContext switchStatement() throws RecognitionException {
		SwitchStatementContext _localctx = new SwitchStatementContext(_ctx, getState());
		enterRule(_localctx, 62, RULE_switchStatement);

		        ((SwitchStatementContext)_localctx).tree =  nullptr;
		        Expression* switchExpression;
		        StackArray<Expression*> caseExpressions(256);
		        StackArray<Statement*> caseStatements(256);
		        Symbol::Location location;
		        Statement* defaultStatement = nullptr;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(729);
			match(T__23);
			 location = SetupFile(); 
			setState(731);
			match(LP);
			setState(732);
			((SwitchStatementContext)_localctx).expression = expression(0);
			setState(733);
			match(RP);
			 switchExpression = ((SwitchStatementContext)_localctx).expression.tree; 
			setState(735);
			match(LB);
			setState(747);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__24) {
				{
				{
				setState(736);
				match(T__24);
				setState(737);
				((SwitchStatementContext)_localctx).expression = expression(0);
				setState(738);
				match(COL);
				 if (caseExpressions.Full()) { throw IndexOutOfBoundsException("Maximum of 256 case expressions reached"); } caseExpressions.Append(((SwitchStatementContext)_localctx).expression.tree); caseStatements.Append(nullptr); 
				setState(743);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 1732289366627078158L) != 0) || ((((_la - 74)) & ~0x3f) == 0 && ((1L << (_la - 74)) & 7787L) != 0)) {
					{
					setState(740);
					((SwitchStatementContext)_localctx).statement = statement();
					 
					                    caseStatements.back() = ((SwitchStatementContext)_localctx).statement.tree;
					                
					}
				}

				}
				}
				setState(749);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(757);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__25) {
				{
				setState(750);
				match(T__25);
				setState(751);
				match(COL);
				setState(755);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 1732289366627078158L) != 0) || ((((_la - 74)) & ~0x3f) == 0 && ((1L << (_la - 74)) & 7787L) != 0)) {
					{
					setState(752);
					((SwitchStatementContext)_localctx).statement = statement();

					                    defaultStatement = ((SwitchStatementContext)_localctx).statement.tree;
					                
					}
				}

				}
			}

			setState(759);
			match(RB);

			        ((SwitchStatementContext)_localctx).tree =  Alloc<SwitchStatement>(switchExpression, std::move(caseExpressions), std::move(caseStatements), defaultStatement);
			        _localctx.tree->location = location;
			    
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class BreakStatementContext extends ParserRuleContext {
		public Statement* tree;
		public TerminalNode SC() { return getToken(GPULangParser.SC, 0); }
		public BreakStatementContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_breakStatement; }
	}

	public final BreakStatementContext breakStatement() throws RecognitionException {
		BreakStatementContext _localctx = new BreakStatementContext(_ctx, getState());
		enterRule(_localctx, 64, RULE_breakStatement);

		        ((BreakStatementContext)_localctx).tree =  nullptr;
		        Symbol::Location location;
		    
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(762);
			match(T__26);
			 location = SetupFile(); 
			setState(764);
			match(SC);

			        ((BreakStatementContext)_localctx).tree =  Alloc<BreakStatement>();
			        _localctx.tree->location = location;
			    
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ExpressionNoCommaContext extends ParserRuleContext {
		public Expression* tree;
		public ExpressionNoCommaContext e1;
		public Token op;
		public ExpressionNoCommaContext p;
		public BinaryexpatomContext atom;
		public ExpressionNoCommaContext e2;
		public ExpressionNoCommaContext ifBody;
		public ExpressionNoCommaContext elseBody;
		public ExpressionNoCommaContext arg0;
		public ExpressionNoCommaContext argn;
		public ExpressionNoCommaContext e3;
		public List<ExpressionNoCommaContext> expressionNoComma() {
			return getRuleContexts(ExpressionNoCommaContext.class);
		}
		public ExpressionNoCommaContext expressionNoComma(int i) {
			return getRuleContext(ExpressionNoCommaContext.class,i);
		}
		public TerminalNode SUB_OP() { return getToken(GPULangParser.SUB_OP, 0); }
		public TerminalNode ADD_OP() { return getToken(GPULangParser.ADD_OP, 0); }
		public TerminalNode NOT() { return getToken(GPULangParser.NOT, 0); }
		public TerminalNode CONJUGATE() { return getToken(GPULangParser.CONJUGATE, 0); }
		public TerminalNode MUL_OP() { return getToken(GPULangParser.MUL_OP, 0); }
		public BinaryexpatomContext binaryexpatom() {
			return getRuleContext(BinaryexpatomContext.class,0);
		}
		public TerminalNode DOT() { return getToken(GPULangParser.DOT, 0); }
		public TerminalNode ARROW() { return getToken(GPULangParser.ARROW, 0); }
		public TerminalNode DIV_OP() { return getToken(GPULangParser.DIV_OP, 0); }
		public TerminalNode MOD() { return getToken(GPULangParser.MOD, 0); }
		public TerminalNode LESS() { return getToken(GPULangParser.LESS, 0); }
		public TerminalNode GREATER() { return getToken(GPULangParser.GREATER, 0); }
		public TerminalNode LESSEQ() { return getToken(GPULangParser.LESSEQ, 0); }
		public TerminalNode GREATEREQ() { return getToken(GPULangParser.GREATEREQ, 0); }
		public TerminalNode LOGICEQ() { return getToken(GPULangParser.LOGICEQ, 0); }
		public TerminalNode NOTEQ() { return getToken(GPULangParser.NOTEQ, 0); }
		public TerminalNode AND() { return getToken(GPULangParser.AND, 0); }
		public TerminalNode XOR() { return getToken(GPULangParser.XOR, 0); }
		public TerminalNode OR() { return getToken(GPULangParser.OR, 0); }
		public TerminalNode LOGICAND() { return getToken(GPULangParser.LOGICAND, 0); }
		public TerminalNode LOGICOR() { return getToken(GPULangParser.LOGICOR, 0); }
		public TerminalNode QU() { return getToken(GPULangParser.QU, 0); }
		public TerminalNode COL() { return getToken(GPULangParser.COL, 0); }
		public TerminalNode ANDSET() { return getToken(GPULangParser.ANDSET, 0); }
		public TerminalNode XORSET() { return getToken(GPULangParser.XORSET, 0); }
		public TerminalNode ORSET() { return getToken(GPULangParser.ORSET, 0); }
		public TerminalNode EQ() { return getToken(GPULangParser.EQ, 0); }
		public TerminalNode LP() { return getToken(GPULangParser.LP, 0); }
		public TerminalNode RP() { return getToken(GPULangParser.RP, 0); }
		public List<LinePreprocessorEntryContext> linePreprocessorEntry() {
			return getRuleContexts(LinePreprocessorEntryContext.class);
		}
		public LinePreprocessorEntryContext linePreprocessorEntry(int i) {
			return getRuleContext(LinePreprocessorEntryContext.class,i);
		}
		public List<TerminalNode> CO() { return getTokens(GPULangParser.CO); }
		public TerminalNode CO(int i) {
			return getToken(GPULangParser.CO, i);
		}
		public TerminalNode LL() { return getToken(GPULangParser.LL, 0); }
		public TerminalNode RR() { return getToken(GPULangParser.RR, 0); }
		public ExpressionNoCommaContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_expressionNoComma; }
	}

	public final ExpressionNoCommaContext expressionNoComma() throws RecognitionException {
		return expressionNoComma(0);
	}

	private ExpressionNoCommaContext expressionNoComma(int _p) throws RecognitionException {
		ParserRuleContext _parentctx = _ctx;
		int _parentState = getState();
		ExpressionNoCommaContext _localctx = new ExpressionNoCommaContext(_ctx, _parentState);
		ExpressionNoCommaContext _prevctx = _localctx;
		int _startState = 66;
		enterRecursionRule(_localctx, 66, RULE_expressionNoComma, _p);

		        ((ExpressionNoCommaContext)_localctx).tree =  nullptr;
		        Symbol::Location location;
		        StackArray<Expression*> args(256);
		    
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(775);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__27:
			case T__28:
			case NOT:
			case CONJUGATE:
			case ADD_OP:
			case SUB_OP:
			case MUL_OP:
				{
				setState(768);
				((ExpressionNoCommaContext)_localctx).op = _input.LT(1);
				_la = _input.LA(1);
				if ( !(((((_la - 28)) & ~0x3f) == 0 && ((1L << (_la - 28)) & 774058335535107L) != 0)) ) {
					((ExpressionNoCommaContext)_localctx).op = (Token)_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				setState(769);
				((ExpressionNoCommaContext)_localctx).p = expressionNoComma(14);

				        ((ExpressionNoCommaContext)_localctx).tree =  Alloc<UnaryExpression>(StringToFourCC((((ExpressionNoCommaContext)_localctx).op!=null?((ExpressionNoCommaContext)_localctx).op.getText():null)), true, ((ExpressionNoCommaContext)_localctx).p.tree);
				        _localctx.tree->location = SetupFile();
				    
				}
				break;
			case T__0:
			case T__1:
			case T__2:
			case LP:
			case LL:
			case QO:
			case Q:
			case INTEGERLITERAL:
			case UINTEGERLITERAL:
			case FLOATLITERAL:
			case DOUBLELITERAL:
			case HEX:
			case IDENTIFIER:
				{
				setState(772);
				((ExpressionNoCommaContext)_localctx).atom = binaryexpatom();
				 ((ExpressionNoCommaContext)_localctx).tree =  ((ExpressionNoCommaContext)_localctx).atom.tree; 
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
			_ctx.stop = _input.LT(-1);
			setState(915);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,75,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					if ( _parseListeners!=null ) triggerExitRuleEvent();
					_prevctx = _localctx;
					{
					setState(913);
					_errHandler.sync(this);
					switch ( getInterpreter().adaptivePredict(_input,74,_ctx) ) {
					case 1:
						{
						_localctx = new ExpressionNoCommaContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expressionNoComma);
						setState(777);
						if (!(precpred(_ctx, 17))) throw new FailedPredicateException(this, "precpred(_ctx, 17)");
						 ((ExpressionNoCommaContext)_localctx).tree =  ((ExpressionNoCommaContext)_localctx).e1.tree; 
						setState(779);
						match(DOT);
						 location = SetupFile(); 
						setState(781);
						((ExpressionNoCommaContext)_localctx).e2 = expressionNoComma(18);

						                  AccessExpression* expr = Alloc<AccessExpression>(_localctx.tree, ((ExpressionNoCommaContext)_localctx).e2.tree, false);
						                  expr->location = ((ExpressionNoCommaContext)_localctx).e1.tree->location;
						                  ((ExpressionNoCommaContext)_localctx).tree =  expr;
						              
						}
						break;
					case 2:
						{
						_localctx = new ExpressionNoCommaContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expressionNoComma);
						setState(784);
						if (!(precpred(_ctx, 16))) throw new FailedPredicateException(this, "precpred(_ctx, 16)");
						 ((ExpressionNoCommaContext)_localctx).tree =  ((ExpressionNoCommaContext)_localctx).e1.tree; 
						setState(786);
						match(ARROW);
						 location = SetupFile(); 
						setState(788);
						((ExpressionNoCommaContext)_localctx).e2 = expressionNoComma(17);

						                  AccessExpression* expr = Alloc<AccessExpression>(_localctx.tree, ((ExpressionNoCommaContext)_localctx).e2.tree, true);
						                  expr->location = ((ExpressionNoCommaContext)_localctx).e1.tree->location;
						                  ((ExpressionNoCommaContext)_localctx).tree =  expr;
						              
						}
						break;
					case 3:
						{
						_localctx = new ExpressionNoCommaContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expressionNoComma);
						setState(791);
						if (!(precpred(_ctx, 13))) throw new FailedPredicateException(this, "precpred(_ctx, 13)");
						 ((ExpressionNoCommaContext)_localctx).tree =  ((ExpressionNoCommaContext)_localctx).e1.tree; 
						setState(793);
						((ExpressionNoCommaContext)_localctx).op = _input.LT(1);
						_la = _input.LA(1);
						if ( !(((((_la - 71)) & ~0x3f) == 0 && ((1L << (_la - 71)) & 97L) != 0)) ) {
							((ExpressionNoCommaContext)_localctx).op = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						 location = SetupFile(); 
						setState(795);
						((ExpressionNoCommaContext)_localctx).e2 = expressionNoComma(14);

						                  BinaryExpression* expr = Alloc<BinaryExpression>(StringToFourCC((((ExpressionNoCommaContext)_localctx).op!=null?((ExpressionNoCommaContext)_localctx).op.getText():null)), _localctx.tree, ((ExpressionNoCommaContext)_localctx).e2.tree);
						                  expr->location = location;
						                  ((ExpressionNoCommaContext)_localctx).tree =  expr;
						              
						}
						break;
					case 4:
						{
						_localctx = new ExpressionNoCommaContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expressionNoComma);
						setState(798);
						if (!(precpred(_ctx, 12))) throw new FailedPredicateException(this, "precpred(_ctx, 12)");
						 ((ExpressionNoCommaContext)_localctx).tree =  ((ExpressionNoCommaContext)_localctx).e1.tree; 
						setState(800);
						((ExpressionNoCommaContext)_localctx).op = _input.LT(1);
						_la = _input.LA(1);
						if ( !(_la==ADD_OP || _la==SUB_OP) ) {
							((ExpressionNoCommaContext)_localctx).op = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						 location = SetupFile(); 
						setState(802);
						((ExpressionNoCommaContext)_localctx).e2 = expressionNoComma(13);

						                  BinaryExpression* expr = Alloc<BinaryExpression>(StringToFourCC((((ExpressionNoCommaContext)_localctx).op!=null?((ExpressionNoCommaContext)_localctx).op.getText():null)), _localctx.tree, ((ExpressionNoCommaContext)_localctx).e2.tree);
						                  expr->location = location;
						                  ((ExpressionNoCommaContext)_localctx).tree =  expr;
						              
						}
						break;
					case 5:
						{
						_localctx = new ExpressionNoCommaContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expressionNoComma);
						setState(805);
						if (!(precpred(_ctx, 11))) throw new FailedPredicateException(this, "precpred(_ctx, 11)");
						 ((ExpressionNoCommaContext)_localctx).tree =  ((ExpressionNoCommaContext)_localctx).e1.tree; 
						setState(807);
						((ExpressionNoCommaContext)_localctx).op = _input.LT(1);
						_la = _input.LA(1);
						if ( !(_la==T__29 || _la==T__30) ) {
							((ExpressionNoCommaContext)_localctx).op = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						 location = SetupFile(); 
						setState(809);
						((ExpressionNoCommaContext)_localctx).e2 = expressionNoComma(12);

						                  BinaryExpression* expr = Alloc<BinaryExpression>(StringToFourCC((((ExpressionNoCommaContext)_localctx).op!=null?((ExpressionNoCommaContext)_localctx).op.getText():null)), _localctx.tree, ((ExpressionNoCommaContext)_localctx).e2.tree);
						                  expr->location = location;
						                  ((ExpressionNoCommaContext)_localctx).tree =  expr;
						              
						}
						break;
					case 6:
						{
						_localctx = new ExpressionNoCommaContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expressionNoComma);
						setState(812);
						if (!(precpred(_ctx, 10))) throw new FailedPredicateException(this, "precpred(_ctx, 10)");
						 ((ExpressionNoCommaContext)_localctx).tree =  ((ExpressionNoCommaContext)_localctx).e1.tree; 
						setState(814);
						((ExpressionNoCommaContext)_localctx).op = _input.LT(1);
						_la = _input.LA(1);
						if ( !(((((_la - 63)) & ~0x3f) == 0 && ((1L << (_la - 63)) & 15L) != 0)) ) {
							((ExpressionNoCommaContext)_localctx).op = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						 location = SetupFile(); 
						setState(816);
						((ExpressionNoCommaContext)_localctx).e2 = expressionNoComma(11);

						                  BinaryExpression* expr = Alloc<BinaryExpression>(StringToFourCC((((ExpressionNoCommaContext)_localctx).op!=null?((ExpressionNoCommaContext)_localctx).op.getText():null)), _localctx.tree, ((ExpressionNoCommaContext)_localctx).e2.tree);
						                  expr->location = location;
						                  ((ExpressionNoCommaContext)_localctx).tree =  expr;
						              
						}
						break;
					case 7:
						{
						_localctx = new ExpressionNoCommaContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expressionNoComma);
						setState(819);
						if (!(precpred(_ctx, 9))) throw new FailedPredicateException(this, "precpred(_ctx, 9)");
						 ((ExpressionNoCommaContext)_localctx).tree =  ((ExpressionNoCommaContext)_localctx).e1.tree; 
						setState(821);
						((ExpressionNoCommaContext)_localctx).op = _input.LT(1);
						_la = _input.LA(1);
						if ( !(_la==LOGICEQ || _la==NOTEQ) ) {
							((ExpressionNoCommaContext)_localctx).op = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						 location = SetupFile(); 
						setState(823);
						((ExpressionNoCommaContext)_localctx).e2 = expressionNoComma(10);

						                  BinaryExpression* expr = Alloc<BinaryExpression>(StringToFourCC((((ExpressionNoCommaContext)_localctx).op!=null?((ExpressionNoCommaContext)_localctx).op.getText():null)), _localctx.tree, ((ExpressionNoCommaContext)_localctx).e2.tree);
						                  expr->location = location;
						                  ((ExpressionNoCommaContext)_localctx).tree =  expr;
						              
						}
						break;
					case 8:
						{
						_localctx = new ExpressionNoCommaContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expressionNoComma);
						setState(826);
						if (!(precpred(_ctx, 8))) throw new FailedPredicateException(this, "precpred(_ctx, 8)");
						 ((ExpressionNoCommaContext)_localctx).tree =  ((ExpressionNoCommaContext)_localctx).e1.tree; 
						setState(828);
						match(AND);
						 location = SetupFile(); 
						setState(830);
						((ExpressionNoCommaContext)_localctx).e2 = expressionNoComma(9);

						                  BinaryExpression* expr = Alloc<BinaryExpression>('&', _localctx.tree, ((ExpressionNoCommaContext)_localctx).e2.tree);
						                  expr->location = location;
						                  ((ExpressionNoCommaContext)_localctx).tree =  expr;
						              
						}
						break;
					case 9:
						{
						_localctx = new ExpressionNoCommaContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expressionNoComma);
						setState(833);
						if (!(precpred(_ctx, 7))) throw new FailedPredicateException(this, "precpred(_ctx, 7)");
						 ((ExpressionNoCommaContext)_localctx).tree =  ((ExpressionNoCommaContext)_localctx).e1.tree; 
						setState(835);
						match(XOR);
						 location = SetupFile(); 
						setState(837);
						((ExpressionNoCommaContext)_localctx).e2 = expressionNoComma(8);

						                  BinaryExpression* expr = Alloc<BinaryExpression>('^', _localctx.tree, ((ExpressionNoCommaContext)_localctx).e2.tree);
						                  expr->location = location;
						                  ((ExpressionNoCommaContext)_localctx).tree =  expr;
						              
						}
						break;
					case 10:
						{
						_localctx = new ExpressionNoCommaContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expressionNoComma);
						setState(840);
						if (!(precpred(_ctx, 6))) throw new FailedPredicateException(this, "precpred(_ctx, 6)");
						 ((ExpressionNoCommaContext)_localctx).tree =  ((ExpressionNoCommaContext)_localctx).e1.tree; 
						setState(842);
						match(OR);
						 location = SetupFile(); 
						setState(844);
						((ExpressionNoCommaContext)_localctx).e2 = expressionNoComma(7);

						                  BinaryExpression* expr = Alloc<BinaryExpression>('|', _localctx.tree, ((ExpressionNoCommaContext)_localctx).e2.tree);
						                  expr->location = location;
						                  ((ExpressionNoCommaContext)_localctx).tree =  expr;
						              
						}
						break;
					case 11:
						{
						_localctx = new ExpressionNoCommaContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expressionNoComma);
						setState(847);
						if (!(precpred(_ctx, 5))) throw new FailedPredicateException(this, "precpred(_ctx, 5)");
						 ((ExpressionNoCommaContext)_localctx).tree =  ((ExpressionNoCommaContext)_localctx).e1.tree; 
						setState(849);
						match(LOGICAND);
						 location = SetupFile(); 
						setState(851);
						((ExpressionNoCommaContext)_localctx).e2 = expressionNoComma(6);

						                  BinaryExpression* expr = Alloc<BinaryExpression>('&&', _localctx.tree, ((ExpressionNoCommaContext)_localctx).e2.tree);
						                  expr->location = location;
						                  ((ExpressionNoCommaContext)_localctx).tree =  expr;
						              
						}
						break;
					case 12:
						{
						_localctx = new ExpressionNoCommaContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expressionNoComma);
						setState(854);
						if (!(precpred(_ctx, 4))) throw new FailedPredicateException(this, "precpred(_ctx, 4)");
						 ((ExpressionNoCommaContext)_localctx).tree =  ((ExpressionNoCommaContext)_localctx).e1.tree; 
						setState(856);
						match(LOGICOR);
						 location = SetupFile(); 
						setState(858);
						((ExpressionNoCommaContext)_localctx).e2 = expressionNoComma(5);

						                  BinaryExpression* expr = Alloc<BinaryExpression>('||', _localctx.tree, ((ExpressionNoCommaContext)_localctx).e2.tree);
						                  expr->location = location;
						                  ((ExpressionNoCommaContext)_localctx).tree =  expr;
						              
						}
						break;
					case 13:
						{
						_localctx = new ExpressionNoCommaContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expressionNoComma);
						setState(861);
						if (!(precpred(_ctx, 3))) throw new FailedPredicateException(this, "precpred(_ctx, 3)");
						setState(862);
						match(QU);
						 location = SetupFile(); 
						setState(864);
						((ExpressionNoCommaContext)_localctx).ifBody = expressionNoComma(0);
						setState(865);
						match(COL);
						setState(866);
						((ExpressionNoCommaContext)_localctx).elseBody = expressionNoComma(3);
						 
						                  TernaryExpression* expr = Alloc<TernaryExpression>(((ExpressionNoCommaContext)_localctx).e1.tree, ((ExpressionNoCommaContext)_localctx).ifBody.tree, ((ExpressionNoCommaContext)_localctx).elseBody.tree);
						                  expr->location = location;
						                  ((ExpressionNoCommaContext)_localctx).tree =  expr;
						              
						}
						break;
					case 14:
						{
						_localctx = new ExpressionNoCommaContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expressionNoComma);
						setState(869);
						if (!(precpred(_ctx, 2))) throw new FailedPredicateException(this, "precpred(_ctx, 2)");
						 ((ExpressionNoCommaContext)_localctx).tree =  ((ExpressionNoCommaContext)_localctx).e1.tree; 
						setState(871);
						((ExpressionNoCommaContext)_localctx).op = _input.LT(1);
						_la = _input.LA(1);
						if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & 379428814066810880L) != 0)) ) {
							((ExpressionNoCommaContext)_localctx).op = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						 location = SetupFile(); 
						setState(873);
						((ExpressionNoCommaContext)_localctx).e2 = expressionNoComma(2);

						                  BinaryExpression* expr = Alloc<BinaryExpression>(StringToFourCC((((ExpressionNoCommaContext)_localctx).op!=null?((ExpressionNoCommaContext)_localctx).op.getText():null)), ((ExpressionNoCommaContext)_localctx).e1.tree, ((ExpressionNoCommaContext)_localctx).e2.tree);
						                  expr->location = location;
						                  ((ExpressionNoCommaContext)_localctx).tree =  expr;
						              
						}
						break;
					case 15:
						{
						_localctx = new ExpressionNoCommaContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expressionNoComma);
						setState(876);
						if (!(precpred(_ctx, 19))) throw new FailedPredicateException(this, "precpred(_ctx, 19)");
						 ((ExpressionNoCommaContext)_localctx).tree =  ((ExpressionNoCommaContext)_localctx).e1.tree; 
						setState(878);
						((ExpressionNoCommaContext)_localctx).op = _input.LT(1);
						_la = _input.LA(1);
						if ( !(_la==T__27 || _la==T__28) ) {
							((ExpressionNoCommaContext)_localctx).op = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						 location = SetupFile(); 

						                  ((ExpressionNoCommaContext)_localctx).tree =  Alloc<UnaryExpression>(StringToFourCC((((ExpressionNoCommaContext)_localctx).op!=null?((ExpressionNoCommaContext)_localctx).op.getText():null)), false, _localctx.tree);
						                  _localctx.tree->location = location;
						              
						}
						break;
					case 16:
						{
						_localctx = new ExpressionNoCommaContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expressionNoComma);
						setState(881);
						if (!(precpred(_ctx, 18))) throw new FailedPredicateException(this, "precpred(_ctx, 18)");
						 ((ExpressionNoCommaContext)_localctx).tree =  ((ExpressionNoCommaContext)_localctx).e1.tree; 
						setState(883);
						match(LP);
						 location = SetupFile(); 
						setState(900);
						_errHandler.sync(this);
						_la = _input.LA(1);
						if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 1732271774273372174L) != 0) || ((((_la - 74)) & ~0x3f) == 0 && ((1L << (_la - 74)) & 7787L) != 0)) {
							{
							setState(885);
							((ExpressionNoCommaContext)_localctx).arg0 = expressionNoComma(0);
							 args.Append(((ExpressionNoCommaContext)_localctx).arg0.tree); 
							setState(888);
							_errHandler.sync(this);
							switch ( getInterpreter().adaptivePredict(_input,69,_ctx) ) {
							case 1:
								{
								setState(887);
								linePreprocessorEntry();
								}
								break;
							}
							setState(897);
							_errHandler.sync(this);
							_la = _input.LA(1);
							while (_la==T__2 || _la==CO) {
								{
								setState(895);
								_errHandler.sync(this);
								switch (_input.LA(1)) {
								case CO:
									{
									setState(890);
									match(CO);
									setState(891);
									((ExpressionNoCommaContext)_localctx).argn = expressionNoComma(0);
									 if (args.Full()) { throw IndexOutOfBoundsException("Maximum of 256 arguments reached"); } args.Append(((ExpressionNoCommaContext)_localctx).argn.tree); 
									}
									break;
								case T__2:
									{
									setState(894);
									linePreprocessorEntry();
									}
									break;
								default:
									throw new NoViableAltException(this);
								}
								}
								setState(899);
								_errHandler.sync(this);
								_la = _input.LA(1);
							}
							}
						}

						setState(902);
						match(RP);

						                  CallExpression* expr = Alloc<CallExpression>(_localctx.tree, std::move(FixedArray<Expression*>(args)));
						                  expr->location = ((ExpressionNoCommaContext)_localctx).e1.tree->location;
						                  ((ExpressionNoCommaContext)_localctx).tree =  expr;
						              
						}
						break;
					case 17:
						{
						_localctx = new ExpressionNoCommaContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expressionNoComma);
						setState(904);
						if (!(precpred(_ctx, 15))) throw new FailedPredicateException(this, "precpred(_ctx, 15)");
						 ((ExpressionNoCommaContext)_localctx).tree =  ((ExpressionNoCommaContext)_localctx).e1.tree; 
						setState(906);
						match(LL);
						 location = SetupFile(); 
						setState(909);
						_errHandler.sync(this);
						_la = _input.LA(1);
						if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 1732271774273372174L) != 0) || ((((_la - 74)) & ~0x3f) == 0 && ((1L << (_la - 74)) & 7787L) != 0)) {
							{
							setState(908);
							((ExpressionNoCommaContext)_localctx).e3 = expressionNoComma(0);
							}
						}

						setState(911);
						match(RR);

						                  ArrayIndexExpression* expr = Alloc<ArrayIndexExpression>(_localctx.tree, ((ExpressionNoCommaContext)_localctx).e3.tree);
						                  expr->location = ((ExpressionNoCommaContext)_localctx).e1.tree->location;
						                  ((ExpressionNoCommaContext)_localctx).tree =  expr;
						              
						}
						break;
					}
					} 
				}
				setState(917);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,75,_ctx);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			unrollRecursionContexts(_parentctx);
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ExpressionListContext extends ParserRuleContext {
		public FixedArray<Expression*> expressions;
		public ExpressionContext e1;
		public ExpressionContext e2;
		public List<ExpressionContext> expression() {
			return getRuleContexts(ExpressionContext.class);
		}
		public ExpressionContext expression(int i) {
			return getRuleContext(ExpressionContext.class,i);
		}
		public List<TerminalNode> CO() { return getTokens(GPULangParser.CO); }
		public TerminalNode CO(int i) {
			return getToken(GPULangParser.CO, i);
		}
		public ExpressionListContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_expressionList; }
	}

	public final ExpressionListContext expressionList() throws RecognitionException {
		ExpressionListContext _localctx = new ExpressionListContext(_ctx, getState());
		enterRule(_localctx, 68, RULE_expressionList);

		        StackArray<Expression*> list(256);
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(918);
			((ExpressionListContext)_localctx).e1 = expression(0);
			 list.Append(((ExpressionListContext)_localctx).e1.tree); 
			setState(926);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==CO) {
				{
				{
				setState(920);
				match(CO);
				setState(921);
				((ExpressionListContext)_localctx).e2 = expression(0);

				        list.Append(((ExpressionListContext)_localctx).e2.tree);
				    
				}
				}
				setState(928);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}

			        ((ExpressionListContext)_localctx).expressions =  std::move(FixedArray<Expression*>(list));
			    
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ExpressionContext extends ParserRuleContext {
		public Expression* tree;
		public ExpressionContext e1;
		public Token op;
		public ExpressionContext p;
		public BinaryexpatomContext atom;
		public ExpressionContext e2;
		public ExpressionContext ifBody;
		public ExpressionContext elseBody;
		public ExpressionNoCommaContext arg0;
		public ExpressionNoCommaContext argn;
		public ExpressionContext e3;
		public List<ExpressionContext> expression() {
			return getRuleContexts(ExpressionContext.class);
		}
		public ExpressionContext expression(int i) {
			return getRuleContext(ExpressionContext.class,i);
		}
		public TerminalNode SUB_OP() { return getToken(GPULangParser.SUB_OP, 0); }
		public TerminalNode ADD_OP() { return getToken(GPULangParser.ADD_OP, 0); }
		public TerminalNode NOT() { return getToken(GPULangParser.NOT, 0); }
		public TerminalNode CONJUGATE() { return getToken(GPULangParser.CONJUGATE, 0); }
		public TerminalNode MUL_OP() { return getToken(GPULangParser.MUL_OP, 0); }
		public BinaryexpatomContext binaryexpatom() {
			return getRuleContext(BinaryexpatomContext.class,0);
		}
		public TerminalNode DOT() { return getToken(GPULangParser.DOT, 0); }
		public TerminalNode ARROW() { return getToken(GPULangParser.ARROW, 0); }
		public TerminalNode DIV_OP() { return getToken(GPULangParser.DIV_OP, 0); }
		public TerminalNode MOD() { return getToken(GPULangParser.MOD, 0); }
		public TerminalNode LESS() { return getToken(GPULangParser.LESS, 0); }
		public TerminalNode GREATER() { return getToken(GPULangParser.GREATER, 0); }
		public TerminalNode LESSEQ() { return getToken(GPULangParser.LESSEQ, 0); }
		public TerminalNode GREATEREQ() { return getToken(GPULangParser.GREATEREQ, 0); }
		public TerminalNode LOGICEQ() { return getToken(GPULangParser.LOGICEQ, 0); }
		public TerminalNode NOTEQ() { return getToken(GPULangParser.NOTEQ, 0); }
		public TerminalNode AND() { return getToken(GPULangParser.AND, 0); }
		public TerminalNode XOR() { return getToken(GPULangParser.XOR, 0); }
		public TerminalNode OR() { return getToken(GPULangParser.OR, 0); }
		public TerminalNode LOGICAND() { return getToken(GPULangParser.LOGICAND, 0); }
		public TerminalNode LOGICOR() { return getToken(GPULangParser.LOGICOR, 0); }
		public TerminalNode QU() { return getToken(GPULangParser.QU, 0); }
		public TerminalNode COL() { return getToken(GPULangParser.COL, 0); }
		public TerminalNode ANDSET() { return getToken(GPULangParser.ANDSET, 0); }
		public TerminalNode XORSET() { return getToken(GPULangParser.XORSET, 0); }
		public TerminalNode ORSET() { return getToken(GPULangParser.ORSET, 0); }
		public TerminalNode EQ() { return getToken(GPULangParser.EQ, 0); }
		public List<TerminalNode> CO() { return getTokens(GPULangParser.CO); }
		public TerminalNode CO(int i) {
			return getToken(GPULangParser.CO, i);
		}
		public TerminalNode LP() { return getToken(GPULangParser.LP, 0); }
		public TerminalNode RP() { return getToken(GPULangParser.RP, 0); }
		public List<ExpressionNoCommaContext> expressionNoComma() {
			return getRuleContexts(ExpressionNoCommaContext.class);
		}
		public ExpressionNoCommaContext expressionNoComma(int i) {
			return getRuleContext(ExpressionNoCommaContext.class,i);
		}
		public List<LinePreprocessorEntryContext> linePreprocessorEntry() {
			return getRuleContexts(LinePreprocessorEntryContext.class);
		}
		public LinePreprocessorEntryContext linePreprocessorEntry(int i) {
			return getRuleContext(LinePreprocessorEntryContext.class,i);
		}
		public TerminalNode LL() { return getToken(GPULangParser.LL, 0); }
		public TerminalNode RR() { return getToken(GPULangParser.RR, 0); }
		public ExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_expression; }
	}

	public final ExpressionContext expression() throws RecognitionException {
		return expression(0);
	}

	private ExpressionContext expression(int _p) throws RecognitionException {
		ParserRuleContext _parentctx = _ctx;
		int _parentState = getState();
		ExpressionContext _localctx = new ExpressionContext(_ctx, _parentState);
		ExpressionContext _prevctx = _localctx;
		int _startState = 70;
		enterRecursionRule(_localctx, 70, RULE_expression, _p);

		        ((ExpressionContext)_localctx).tree =  nullptr;
		        Symbol::Location location;
		        StackArray<Expression*> args(256);
		    
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(939);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__27:
			case T__28:
			case NOT:
			case CONJUGATE:
			case ADD_OP:
			case SUB_OP:
			case MUL_OP:
				{
				setState(932);
				((ExpressionContext)_localctx).op = _input.LT(1);
				_la = _input.LA(1);
				if ( !(((((_la - 28)) & ~0x3f) == 0 && ((1L << (_la - 28)) & 774058335535107L) != 0)) ) {
					((ExpressionContext)_localctx).op = (Token)_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				setState(933);
				((ExpressionContext)_localctx).p = expression(15);

				        ((ExpressionContext)_localctx).tree =  Alloc<UnaryExpression>(StringToFourCC((((ExpressionContext)_localctx).op!=null?((ExpressionContext)_localctx).op.getText():null)), true, ((ExpressionContext)_localctx).p.tree);
				        _localctx.tree->location = ((ExpressionContext)_localctx).p.tree->location;
				    
				}
				break;
			case T__0:
			case T__1:
			case T__2:
			case LP:
			case LL:
			case QO:
			case Q:
			case INTEGERLITERAL:
			case UINTEGERLITERAL:
			case FLOATLITERAL:
			case DOUBLELITERAL:
			case HEX:
			case IDENTIFIER:
				{
				setState(936);
				((ExpressionContext)_localctx).atom = binaryexpatom();
				 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).atom.tree; 
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
			_ctx.stop = _input.LT(-1);
			setState(1086);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,84,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					if ( _parseListeners!=null ) triggerExitRuleEvent();
					_prevctx = _localctx;
					{
					setState(1084);
					_errHandler.sync(this);
					switch ( getInterpreter().adaptivePredict(_input,83,_ctx) ) {
					case 1:
						{
						_localctx = new ExpressionContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expression);
						setState(941);
						if (!(precpred(_ctx, 18))) throw new FailedPredicateException(this, "precpred(_ctx, 18)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(943);
						match(DOT);
						 location = SetupFile(); 
						setState(945);
						((ExpressionContext)_localctx).e2 = expression(19);

						                  AccessExpression* expr = Alloc<AccessExpression>(_localctx.tree, ((ExpressionContext)_localctx).e2.tree, false);
						                  expr->location = ((ExpressionContext)_localctx).e1.tree->location;
						                  ((ExpressionContext)_localctx).tree =  expr;
						              
						}
						break;
					case 2:
						{
						_localctx = new ExpressionContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expression);
						setState(948);
						if (!(precpred(_ctx, 17))) throw new FailedPredicateException(this, "precpred(_ctx, 17)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(950);
						match(ARROW);
						 location = SetupFile(); 
						setState(952);
						((ExpressionContext)_localctx).e2 = expression(18);

						                  AccessExpression* expr = Alloc<AccessExpression>(_localctx.tree, ((ExpressionContext)_localctx).e2.tree, true);
						                  expr->location = ((ExpressionContext)_localctx).e1.tree->location;
						                  ((ExpressionContext)_localctx).tree =  expr;
						              
						}
						break;
					case 3:
						{
						_localctx = new ExpressionContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expression);
						setState(955);
						if (!(precpred(_ctx, 14))) throw new FailedPredicateException(this, "precpred(_ctx, 14)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(957);
						((ExpressionContext)_localctx).op = _input.LT(1);
						_la = _input.LA(1);
						if ( !(((((_la - 71)) & ~0x3f) == 0 && ((1L << (_la - 71)) & 97L) != 0)) ) {
							((ExpressionContext)_localctx).op = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						 location = SetupFile(); 
						setState(959);
						((ExpressionContext)_localctx).e2 = expression(15);

						                  BinaryExpression* expr = Alloc<BinaryExpression>(StringToFourCC((((ExpressionContext)_localctx).op!=null?((ExpressionContext)_localctx).op.getText():null)), _localctx.tree, ((ExpressionContext)_localctx).e2.tree);
						                  expr->location = ((ExpressionContext)_localctx).e1.tree->location;
						                  ((ExpressionContext)_localctx).tree =  expr;
						              
						}
						break;
					case 4:
						{
						_localctx = new ExpressionContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expression);
						setState(962);
						if (!(precpred(_ctx, 13))) throw new FailedPredicateException(this, "precpred(_ctx, 13)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(964);
						((ExpressionContext)_localctx).op = _input.LT(1);
						_la = _input.LA(1);
						if ( !(_la==ADD_OP || _la==SUB_OP) ) {
							((ExpressionContext)_localctx).op = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						 location = SetupFile(); 
						setState(966);
						((ExpressionContext)_localctx).e2 = expression(14);

						                  BinaryExpression* expr = Alloc<BinaryExpression>(StringToFourCC((((ExpressionContext)_localctx).op!=null?((ExpressionContext)_localctx).op.getText():null)), _localctx.tree, ((ExpressionContext)_localctx).e2.tree);
						                  expr->location = ((ExpressionContext)_localctx).e1.tree->location;
						                  ((ExpressionContext)_localctx).tree =  expr;
						              
						}
						break;
					case 5:
						{
						_localctx = new ExpressionContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expression);
						setState(969);
						if (!(precpred(_ctx, 12))) throw new FailedPredicateException(this, "precpred(_ctx, 12)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(971);
						((ExpressionContext)_localctx).op = _input.LT(1);
						_la = _input.LA(1);
						if ( !(_la==T__29 || _la==T__30) ) {
							((ExpressionContext)_localctx).op = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						 location = SetupFile(); 
						setState(973);
						((ExpressionContext)_localctx).e2 = expression(13);

						                  BinaryExpression* expr = Alloc<BinaryExpression>(StringToFourCC((((ExpressionContext)_localctx).op!=null?((ExpressionContext)_localctx).op.getText():null)), _localctx.tree, ((ExpressionContext)_localctx).e2.tree);
						                  expr->location = ((ExpressionContext)_localctx).e1.tree->location;
						                  ((ExpressionContext)_localctx).tree =  expr;
						              
						}
						break;
					case 6:
						{
						_localctx = new ExpressionContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expression);
						setState(976);
						if (!(precpred(_ctx, 11))) throw new FailedPredicateException(this, "precpred(_ctx, 11)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(978);
						((ExpressionContext)_localctx).op = _input.LT(1);
						_la = _input.LA(1);
						if ( !(((((_la - 63)) & ~0x3f) == 0 && ((1L << (_la - 63)) & 15L) != 0)) ) {
							((ExpressionContext)_localctx).op = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						 location = SetupFile(); 
						setState(980);
						((ExpressionContext)_localctx).e2 = expression(12);

						                  BinaryExpression* expr = Alloc<BinaryExpression>(StringToFourCC((((ExpressionContext)_localctx).op!=null?((ExpressionContext)_localctx).op.getText():null)), _localctx.tree, ((ExpressionContext)_localctx).e2.tree);
						                  expr->location = ((ExpressionContext)_localctx).e1.tree->location;
						                  ((ExpressionContext)_localctx).tree =  expr;
						              
						}
						break;
					case 7:
						{
						_localctx = new ExpressionContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expression);
						setState(983);
						if (!(precpred(_ctx, 10))) throw new FailedPredicateException(this, "precpred(_ctx, 10)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(985);
						((ExpressionContext)_localctx).op = _input.LT(1);
						_la = _input.LA(1);
						if ( !(_la==LOGICEQ || _la==NOTEQ) ) {
							((ExpressionContext)_localctx).op = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						 location = SetupFile(); 
						setState(987);
						((ExpressionContext)_localctx).e2 = expression(11);

						                  BinaryExpression* expr = Alloc<BinaryExpression>(StringToFourCC((((ExpressionContext)_localctx).op!=null?((ExpressionContext)_localctx).op.getText():null)), _localctx.tree, ((ExpressionContext)_localctx).e2.tree);
						                  expr->location = ((ExpressionContext)_localctx).e1.tree->location;
						                  ((ExpressionContext)_localctx).tree =  expr;
						              
						}
						break;
					case 8:
						{
						_localctx = new ExpressionContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expression);
						setState(990);
						if (!(precpred(_ctx, 9))) throw new FailedPredicateException(this, "precpred(_ctx, 9)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(992);
						match(AND);
						 location = SetupFile(); 
						setState(994);
						((ExpressionContext)_localctx).e2 = expression(10);

						                  BinaryExpression* expr = Alloc<BinaryExpression>('&', _localctx.tree, ((ExpressionContext)_localctx).e2.tree);
						                  expr->location = ((ExpressionContext)_localctx).e1.tree->location;
						                  ((ExpressionContext)_localctx).tree =  expr;
						              
						}
						break;
					case 9:
						{
						_localctx = new ExpressionContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expression);
						setState(997);
						if (!(precpred(_ctx, 8))) throw new FailedPredicateException(this, "precpred(_ctx, 8)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(999);
						match(XOR);
						 location = SetupFile(); 
						setState(1001);
						((ExpressionContext)_localctx).e2 = expression(9);

						                  BinaryExpression* expr = Alloc<BinaryExpression>('^', _localctx.tree, ((ExpressionContext)_localctx).e2.tree);
						                  expr->location = location;
						                  ((ExpressionContext)_localctx).tree =  expr;
						              
						}
						break;
					case 10:
						{
						_localctx = new ExpressionContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expression);
						setState(1004);
						if (!(precpred(_ctx, 7))) throw new FailedPredicateException(this, "precpred(_ctx, 7)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(1006);
						match(OR);
						 location = SetupFile(); 
						setState(1008);
						((ExpressionContext)_localctx).e2 = expression(8);

						                  BinaryExpression* expr = Alloc<BinaryExpression>('|', _localctx.tree, ((ExpressionContext)_localctx).e2.tree);
						                  expr->location = ((ExpressionContext)_localctx).e1.tree->location;
						                  ((ExpressionContext)_localctx).tree =  expr;
						              
						}
						break;
					case 11:
						{
						_localctx = new ExpressionContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expression);
						setState(1011);
						if (!(precpred(_ctx, 6))) throw new FailedPredicateException(this, "precpred(_ctx, 6)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(1013);
						match(LOGICAND);
						 location = SetupFile(); 
						setState(1015);
						((ExpressionContext)_localctx).e2 = expression(7);

						                  BinaryExpression* expr = Alloc<BinaryExpression>('&&', _localctx.tree, ((ExpressionContext)_localctx).e2.tree);
						                  expr->location = ((ExpressionContext)_localctx).e1.tree->location;
						                  ((ExpressionContext)_localctx).tree =  expr;
						              
						}
						break;
					case 12:
						{
						_localctx = new ExpressionContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expression);
						setState(1018);
						if (!(precpred(_ctx, 5))) throw new FailedPredicateException(this, "precpred(_ctx, 5)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(1020);
						match(LOGICOR);
						 location = SetupFile(); 
						setState(1022);
						((ExpressionContext)_localctx).e2 = expression(6);

						                  BinaryExpression* expr = Alloc<BinaryExpression>('||', _localctx.tree, ((ExpressionContext)_localctx).e2.tree);
						                  expr->location = ((ExpressionContext)_localctx).e1.tree->location;
						                  ((ExpressionContext)_localctx).tree =  expr;
						              
						}
						break;
					case 13:
						{
						_localctx = new ExpressionContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expression);
						setState(1025);
						if (!(precpred(_ctx, 4))) throw new FailedPredicateException(this, "precpred(_ctx, 4)");
						setState(1026);
						match(QU);
						 location = SetupFile(); 
						setState(1028);
						((ExpressionContext)_localctx).ifBody = expression(0);
						setState(1029);
						match(COL);
						setState(1030);
						((ExpressionContext)_localctx).elseBody = expression(4);
						 
						                  TernaryExpression* expr = Alloc<TernaryExpression>(((ExpressionContext)_localctx).e1.tree, ((ExpressionContext)_localctx).ifBody.tree, ((ExpressionContext)_localctx).elseBody.tree);
						                  expr->location = ((ExpressionContext)_localctx).e1.tree->location;
						                  ((ExpressionContext)_localctx).tree =  expr;
						              
						}
						break;
					case 14:
						{
						_localctx = new ExpressionContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expression);
						setState(1033);
						if (!(precpred(_ctx, 3))) throw new FailedPredicateException(this, "precpred(_ctx, 3)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(1035);
						((ExpressionContext)_localctx).op = _input.LT(1);
						_la = _input.LA(1);
						if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & 379428814066810880L) != 0)) ) {
							((ExpressionContext)_localctx).op = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						 location = SetupFile(); 
						setState(1037);
						((ExpressionContext)_localctx).e2 = expression(3);

						                  BinaryExpression* expr = Alloc<BinaryExpression>(StringToFourCC((((ExpressionContext)_localctx).op!=null?((ExpressionContext)_localctx).op.getText():null)), ((ExpressionContext)_localctx).e1.tree, ((ExpressionContext)_localctx).e2.tree);
						                  expr->location = ((ExpressionContext)_localctx).e1.tree->location;
						                  ((ExpressionContext)_localctx).tree =  expr;
						              
						}
						break;
					case 15:
						{
						_localctx = new ExpressionContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expression);
						setState(1040);
						if (!(precpred(_ctx, 2))) throw new FailedPredicateException(this, "precpred(_ctx, 2)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(1042);
						match(CO);
						 location = SetupFile(); 
						setState(1044);
						((ExpressionContext)_localctx).e2 = expression(3);

						                  CommaExpression* expr = Alloc<CommaExpression>(_localctx.tree, ((ExpressionContext)_localctx).e2.tree);
						                  expr->location = ((ExpressionContext)_localctx).e1.tree->location;
						                  ((ExpressionContext)_localctx).tree =  expr;
						              
						}
						break;
					case 16:
						{
						_localctx = new ExpressionContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expression);
						setState(1047);
						if (!(precpred(_ctx, 20))) throw new FailedPredicateException(this, "precpred(_ctx, 20)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(1049);
						((ExpressionContext)_localctx).op = _input.LT(1);
						_la = _input.LA(1);
						if ( !(_la==T__27 || _la==T__28) ) {
							((ExpressionContext)_localctx).op = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						 location = SetupFile(); 

						                  ((ExpressionContext)_localctx).tree =  Alloc<UnaryExpression>(StringToFourCC((((ExpressionContext)_localctx).op!=null?((ExpressionContext)_localctx).op.getText():null)), false, _localctx.tree);
						                  _localctx.tree->location = location;
						              
						}
						break;
					case 17:
						{
						_localctx = new ExpressionContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expression);
						setState(1052);
						if (!(precpred(_ctx, 19))) throw new FailedPredicateException(this, "precpred(_ctx, 19)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(1054);
						match(LP);
						 location = SetupFile(); 
						setState(1071);
						_errHandler.sync(this);
						_la = _input.LA(1);
						if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 1732271774273372174L) != 0) || ((((_la - 74)) & ~0x3f) == 0 && ((1L << (_la - 74)) & 7787L) != 0)) {
							{
							setState(1056);
							((ExpressionContext)_localctx).arg0 = expressionNoComma(0);
							 args.Append(((ExpressionContext)_localctx).arg0.tree); 
							setState(1059);
							_errHandler.sync(this);
							switch ( getInterpreter().adaptivePredict(_input,78,_ctx) ) {
							case 1:
								{
								setState(1058);
								linePreprocessorEntry();
								}
								break;
							}
							setState(1068);
							_errHandler.sync(this);
							_la = _input.LA(1);
							while (_la==T__2 || _la==CO) {
								{
								setState(1066);
								_errHandler.sync(this);
								switch (_input.LA(1)) {
								case CO:
									{
									setState(1061);
									match(CO);
									setState(1062);
									((ExpressionContext)_localctx).argn = expressionNoComma(0);
									 if (args.Full()) { throw IndexOutOfBoundsException("Maximum of 256 arguments reached"); } args.Append(((ExpressionContext)_localctx).argn.tree); 
									}
									break;
								case T__2:
									{
									setState(1065);
									linePreprocessorEntry();
									}
									break;
								default:
									throw new NoViableAltException(this);
								}
								}
								setState(1070);
								_errHandler.sync(this);
								_la = _input.LA(1);
							}
							}
						}

						setState(1073);
						match(RP);

						                  CallExpression* expr = Alloc<CallExpression>(_localctx.tree, std::move(FixedArray<Expression*>(args)));
						                  expr->location = ((ExpressionContext)_localctx).e1.tree->location;
						                  ((ExpressionContext)_localctx).tree =  expr;
						              
						}
						break;
					case 18:
						{
						_localctx = new ExpressionContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expression);
						setState(1075);
						if (!(precpred(_ctx, 16))) throw new FailedPredicateException(this, "precpred(_ctx, 16)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(1077);
						match(LL);
						 location = SetupFile(); 
						setState(1080);
						_errHandler.sync(this);
						_la = _input.LA(1);
						if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 1732271774273372174L) != 0) || ((((_la - 74)) & ~0x3f) == 0 && ((1L << (_la - 74)) & 7787L) != 0)) {
							{
							setState(1079);
							((ExpressionContext)_localctx).e3 = expression(0);
							}
						}

						setState(1082);
						match(RR);

						                  ArrayIndexExpression* expr = Alloc<ArrayIndexExpression>(_localctx.tree, ((ExpressionContext)_localctx).e3.tree);
						                  expr->location = ((ExpressionContext)_localctx).e1.tree->location;
						                  ((ExpressionContext)_localctx).tree =  expr;
						              
						}
						break;
					}
					} 
				}
				setState(1088);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,84,_ctx);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			unrollRecursionContexts(_parentctx);
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class BinaryexpatomContext extends ParserRuleContext {
		public Expression* tree;
		public InitializerExpressionContext initializerExpression;
		public ArrayInitializerExpressionContext arrayInitializerExpression;
		public ExpressionContext expression;
		public Token INTEGERLITERAL;
		public Token UINTEGERLITERAL;
		public Token FLOATLITERAL;
		public Token DOUBLELITERAL;
		public Token HEX;
		public StringContext string;
		public Token IDENTIFIER;
		public BooleanContext boolean_;
		public InitializerExpressionContext initializerExpression() {
			return getRuleContext(InitializerExpressionContext.class,0);
		}
		public ArrayInitializerExpressionContext arrayInitializerExpression() {
			return getRuleContext(ArrayInitializerExpressionContext.class,0);
		}
		public TerminalNode LP() { return getToken(GPULangParser.LP, 0); }
		public ExpressionContext expression() {
			return getRuleContext(ExpressionContext.class,0);
		}
		public TerminalNode RP() { return getToken(GPULangParser.RP, 0); }
		public TerminalNode INTEGERLITERAL() { return getToken(GPULangParser.INTEGERLITERAL, 0); }
		public TerminalNode UINTEGERLITERAL() { return getToken(GPULangParser.UINTEGERLITERAL, 0); }
		public TerminalNode FLOATLITERAL() { return getToken(GPULangParser.FLOATLITERAL, 0); }
		public TerminalNode DOUBLELITERAL() { return getToken(GPULangParser.DOUBLELITERAL, 0); }
		public TerminalNode HEX() { return getToken(GPULangParser.HEX, 0); }
		public StringContext string() {
			return getRuleContext(StringContext.class,0);
		}
		public TerminalNode IDENTIFIER() { return getToken(GPULangParser.IDENTIFIER, 0); }
		public BooleanContext boolean_() {
			return getRuleContext(BooleanContext.class,0);
		}
		public LinePreprocessorEntryContext linePreprocessorEntry() {
			return getRuleContext(LinePreprocessorEntryContext.class,0);
		}
		public BinaryexpatomContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_binaryexpatom; }
	}

	public final BinaryexpatomContext binaryexpatom() throws RecognitionException {
		BinaryexpatomContext _localctx = new BinaryexpatomContext(_ctx, getState());
		enterRule(_localctx, 72, RULE_binaryexpatom);

		        ((BinaryexpatomContext)_localctx).tree =  nullptr;
		        Symbol::Location begin = BeginLocationRange();
		    
		try {
			setState(1119);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,85,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(1089);
				((BinaryexpatomContext)_localctx).initializerExpression = initializerExpression();
				 ((BinaryexpatomContext)_localctx).tree =  ((BinaryexpatomContext)_localctx).initializerExpression.tree; 
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(1092);
				((BinaryexpatomContext)_localctx).arrayInitializerExpression = arrayInitializerExpression();
				 ((BinaryexpatomContext)_localctx).tree =  ((BinaryexpatomContext)_localctx).arrayInitializerExpression.tree; 
				}
				break;
			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(1095);
				match(LP);
				setState(1096);
				((BinaryexpatomContext)_localctx).expression = expression(0);
				setState(1097);
				match(RP);
				 ((BinaryexpatomContext)_localctx).tree =  ((BinaryexpatomContext)_localctx).expression.tree; 
				}
				break;
			case 4:
				enterOuterAlt(_localctx, 4);
				{
				setState(1100);
				((BinaryexpatomContext)_localctx).INTEGERLITERAL = match(INTEGERLITERAL);
				 ((BinaryexpatomContext)_localctx).tree =  Alloc<IntExpression>(atoi((((BinaryexpatomContext)_localctx).INTEGERLITERAL!=null?((BinaryexpatomContext)_localctx).INTEGERLITERAL.getText():null).c_str())); _localctx.tree->location = SetupFile(); 
				}
				break;
			case 5:
				enterOuterAlt(_localctx, 5);
				{
				setState(1102);
				((BinaryexpatomContext)_localctx).UINTEGERLITERAL = match(UINTEGERLITERAL);
				 ((BinaryexpatomContext)_localctx).tree =  Alloc<UIntExpression>(strtoul((((BinaryexpatomContext)_localctx).UINTEGERLITERAL!=null?((BinaryexpatomContext)_localctx).UINTEGERLITERAL.getText():null).c_str(), nullptr, 10)); _localctx.tree->location = SetupFile(); 
				}
				break;
			case 6:
				enterOuterAlt(_localctx, 6);
				{
				setState(1104);
				((BinaryexpatomContext)_localctx).FLOATLITERAL = match(FLOATLITERAL);
				 ((BinaryexpatomContext)_localctx).tree =  Alloc<FloatExpression>(atof((((BinaryexpatomContext)_localctx).FLOATLITERAL!=null?((BinaryexpatomContext)_localctx).FLOATLITERAL.getText():null).c_str())); _localctx.tree->location = SetupFile(); 
				}
				break;
			case 7:
				enterOuterAlt(_localctx, 7);
				{
				setState(1106);
				((BinaryexpatomContext)_localctx).DOUBLELITERAL = match(DOUBLELITERAL);
				 ((BinaryexpatomContext)_localctx).tree =  Alloc<FloatExpression>(atof((((BinaryexpatomContext)_localctx).DOUBLELITERAL!=null?((BinaryexpatomContext)_localctx).DOUBLELITERAL.getText():null).c_str())); _localctx.tree->location = SetupFile(); 
				}
				break;
			case 8:
				enterOuterAlt(_localctx, 8);
				{
				setState(1108);
				((BinaryexpatomContext)_localctx).HEX = match(HEX);
				 ((BinaryexpatomContext)_localctx).tree =  Alloc<UIntExpression>(strtoul((((BinaryexpatomContext)_localctx).HEX!=null?((BinaryexpatomContext)_localctx).HEX.getText():null).c_str(), nullptr, 16)); _localctx.tree->location = SetupFile(); 
				}
				break;
			case 9:
				enterOuterAlt(_localctx, 9);
				{
				setState(1110);
				((BinaryexpatomContext)_localctx).string = string();
				 ((BinaryexpatomContext)_localctx).tree =  Alloc<StringExpression>(((BinaryexpatomContext)_localctx).string.val); _localctx.tree->location = EndLocationRange(begin); 
				}
				break;
			case 10:
				enterOuterAlt(_localctx, 10);
				{
				setState(1113);
				((BinaryexpatomContext)_localctx).IDENTIFIER = match(IDENTIFIER);
				 ((BinaryexpatomContext)_localctx).tree =  Alloc<SymbolExpression>((((BinaryexpatomContext)_localctx).IDENTIFIER!=null?((BinaryexpatomContext)_localctx).IDENTIFIER.getText():null)); _localctx.tree->location = SetupFile(); 
				}
				break;
			case 11:
				enterOuterAlt(_localctx, 11);
				{
				setState(1115);
				((BinaryexpatomContext)_localctx).boolean_ = boolean_();
				 ((BinaryexpatomContext)_localctx).tree =  Alloc<BoolExpression>(((BinaryexpatomContext)_localctx).boolean.val); _localctx.tree->location = SetupFile(); 
				}
				break;
			case 12:
				enterOuterAlt(_localctx, 12);
				{
				setState(1118);
				linePreprocessorEntry();
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class InitializerExpressionContext extends ParserRuleContext {
		public Expression* tree;
		public Token type;
		public ExpressionNoCommaContext arg0;
		public ExpressionNoCommaContext argN;
		public TerminalNode LB() { return getToken(GPULangParser.LB, 0); }
		public TerminalNode RB() { return getToken(GPULangParser.RB, 0); }
		public TerminalNode IDENTIFIER() { return getToken(GPULangParser.IDENTIFIER, 0); }
		public List<ExpressionNoCommaContext> expressionNoComma() {
			return getRuleContexts(ExpressionNoCommaContext.class);
		}
		public ExpressionNoCommaContext expressionNoComma(int i) {
			return getRuleContext(ExpressionNoCommaContext.class,i);
		}
		public List<LinePreprocessorEntryContext> linePreprocessorEntry() {
			return getRuleContexts(LinePreprocessorEntryContext.class);
		}
		public LinePreprocessorEntryContext linePreprocessorEntry(int i) {
			return getRuleContext(LinePreprocessorEntryContext.class,i);
		}
		public List<TerminalNode> CO() { return getTokens(GPULangParser.CO); }
		public TerminalNode CO(int i) {
			return getToken(GPULangParser.CO, i);
		}
		public InitializerExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_initializerExpression; }
	}

	public final InitializerExpressionContext initializerExpression() throws RecognitionException {
		InitializerExpressionContext _localctx = new InitializerExpressionContext(_ctx, getState());
		enterRule(_localctx, 74, RULE_initializerExpression);

		        ((InitializerExpressionContext)_localctx).tree =  nullptr;
		        StackArray<Expression*> exprs(4096);
		        std::string type = "";
		        Symbol::Location location;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(1121);
			((InitializerExpressionContext)_localctx).type = match(IDENTIFIER);
			 type = (((InitializerExpressionContext)_localctx).type!=null?((InitializerExpressionContext)_localctx).type.getText():null); 
			setState(1123);
			match(LB);
			 location = SetupFile(); 
			setState(1140);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 1732271774273372174L) != 0) || ((((_la - 74)) & ~0x3f) == 0 && ((1L << (_la - 74)) & 7787L) != 0)) {
				{
				setState(1125);
				((InitializerExpressionContext)_localctx).arg0 = expressionNoComma(0);
				 if (((InitializerExpressionContext)_localctx).arg0.tree != nullptr) exprs.Append(((InitializerExpressionContext)_localctx).arg0.tree); 
				setState(1128);
				_errHandler.sync(this);
				switch ( getInterpreter().adaptivePredict(_input,86,_ctx) ) {
				case 1:
					{
					setState(1127);
					linePreprocessorEntry();
					}
					break;
				}
				setState(1137);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while (_la==T__2 || _la==CO) {
					{
					setState(1135);
					_errHandler.sync(this);
					switch (_input.LA(1)) {
					case CO:
						{
						setState(1130);
						match(CO);
						setState(1131);
						((InitializerExpressionContext)_localctx).argN = expressionNoComma(0);
						 if (exprs.Full()) { throw IndexOutOfBoundsException("Maximum of 4096 expressions reached"); } exprs.Append(((InitializerExpressionContext)_localctx).argN.tree); 
						}
						break;
					case T__2:
						{
						setState(1134);
						linePreprocessorEntry();
						}
						break;
					default:
						throw new NoViableAltException(this);
					}
					}
					setState(1139);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				}
			}

			setState(1142);
			match(RB);

			        ((InitializerExpressionContext)_localctx).tree =  Alloc<InitializerExpression>(exprs, type);
			        _localctx.tree->location = location;
			    
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ArrayInitializerExpressionContext extends ParserRuleContext {
		public Expression* tree;
		public ExpressionNoCommaContext arg0;
		public ExpressionNoCommaContext argN;
		public TerminalNode LL() { return getToken(GPULangParser.LL, 0); }
		public TerminalNode RR() { return getToken(GPULangParser.RR, 0); }
		public List<ExpressionNoCommaContext> expressionNoComma() {
			return getRuleContexts(ExpressionNoCommaContext.class);
		}
		public ExpressionNoCommaContext expressionNoComma(int i) {
			return getRuleContext(ExpressionNoCommaContext.class,i);
		}
		public List<LinePreprocessorEntryContext> linePreprocessorEntry() {
			return getRuleContexts(LinePreprocessorEntryContext.class);
		}
		public LinePreprocessorEntryContext linePreprocessorEntry(int i) {
			return getRuleContext(LinePreprocessorEntryContext.class,i);
		}
		public List<TerminalNode> CO() { return getTokens(GPULangParser.CO); }
		public TerminalNode CO(int i) {
			return getToken(GPULangParser.CO, i);
		}
		public ArrayInitializerExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_arrayInitializerExpression; }
	}

	public final ArrayInitializerExpressionContext arrayInitializerExpression() throws RecognitionException {
		ArrayInitializerExpressionContext _localctx = new ArrayInitializerExpressionContext(_ctx, getState());
		enterRule(_localctx, 76, RULE_arrayInitializerExpression);

		        ((ArrayInitializerExpressionContext)_localctx).tree =  nullptr;
		        StackArray<Expression*> exprs(4096);
		        Symbol::Location location;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(1145);
			match(LL);
			 location = SetupFile(); 
			setState(1162);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 1732271774273372174L) != 0) || ((((_la - 74)) & ~0x3f) == 0 && ((1L << (_la - 74)) & 7787L) != 0)) {
				{
				setState(1147);
				((ArrayInitializerExpressionContext)_localctx).arg0 = expressionNoComma(0);
				 if (((ArrayInitializerExpressionContext)_localctx).arg0.tree != nullptr) exprs.Append(((ArrayInitializerExpressionContext)_localctx).arg0.tree); 
				setState(1150);
				_errHandler.sync(this);
				switch ( getInterpreter().adaptivePredict(_input,90,_ctx) ) {
				case 1:
					{
					setState(1149);
					linePreprocessorEntry();
					}
					break;
				}
				setState(1159);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while (_la==T__2 || _la==CO) {
					{
					setState(1157);
					_errHandler.sync(this);
					switch (_input.LA(1)) {
					case CO:
						{
						setState(1152);
						match(CO);
						setState(1153);
						((ArrayInitializerExpressionContext)_localctx).argN = expressionNoComma(0);
						 if (exprs.Full()) { throw IndexOutOfBoundsException("Maximum of 4096 expressions reached"); } exprs.Append(((ArrayInitializerExpressionContext)_localctx).argN.tree); 
						}
						break;
					case T__2:
						{
						setState(1156);
						linePreprocessorEntry();
						}
						break;
					default:
						throw new NoViableAltException(this);
					}
					}
					setState(1161);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				}
			}

			setState(1164);
			match(RR);

			        ((ArrayInitializerExpressionContext)_localctx).tree =  Alloc<ArrayInitializerExpression>(exprs);
			        _localctx.tree->location = location;
			    
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class FloatVecLiteralExpressionContext extends ParserRuleContext {
		public Expression* tree;
		public Token arg0;
		public Token argN;
		public TerminalNode LESS() { return getToken(GPULangParser.LESS, 0); }
		public TerminalNode GREATER() { return getToken(GPULangParser.GREATER, 0); }
		public List<TerminalNode> FLOATLITERAL() { return getTokens(GPULangParser.FLOATLITERAL); }
		public TerminalNode FLOATLITERAL(int i) {
			return getToken(GPULangParser.FLOATLITERAL, i);
		}
		public LinePreprocessorEntryContext linePreprocessorEntry() {
			return getRuleContext(LinePreprocessorEntryContext.class,0);
		}
		public List<TerminalNode> CO() { return getTokens(GPULangParser.CO); }
		public TerminalNode CO(int i) {
			return getToken(GPULangParser.CO, i);
		}
		public FloatVecLiteralExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_floatVecLiteralExpression; }
	}

	public final FloatVecLiteralExpressionContext floatVecLiteralExpression() throws RecognitionException {
		FloatVecLiteralExpressionContext _localctx = new FloatVecLiteralExpressionContext(_ctx, getState());
		enterRule(_localctx, 78, RULE_floatVecLiteralExpression);

		        ((FloatVecLiteralExpressionContext)_localctx).tree =  nullptr;
		        std::vector<float> values;
		        Symbol::Location location;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(1167);
			match(LESS);
			 location = SetupFile(); 
			{
			setState(1169);
			((FloatVecLiteralExpressionContext)_localctx).arg0 = match(FLOATLITERAL);
			 values.push_back(atof((((FloatVecLiteralExpressionContext)_localctx).arg0!=null?((FloatVecLiteralExpressionContext)_localctx).arg0.getText():null).c_str())); 
			}
			setState(1173);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__2) {
				{
				setState(1172);
				linePreprocessorEntry();
				}
			}

			setState(1178); 
			_errHandler.sync(this);
			_la = _input.LA(1);
			do {
				{
				{
				setState(1175);
				match(CO);
				setState(1176);
				((FloatVecLiteralExpressionContext)_localctx).argN = match(FLOATLITERAL);
				 values.push_back(atof((((FloatVecLiteralExpressionContext)_localctx).argN!=null?((FloatVecLiteralExpressionContext)_localctx).argN.getText():null).c_str())); 
				}
				}
				setState(1180); 
				_errHandler.sync(this);
				_la = _input.LA(1);
			} while ( _la==CO );
			setState(1182);
			match(GREATER);

			        ((FloatVecLiteralExpressionContext)_localctx).tree =  Alloc<FloatVecExpression>(values);
			        _localctx.tree->location = location;
			    
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class DoubleVecLiteralExpressionContext extends ParserRuleContext {
		public Expression* tree;
		public Token arg0;
		public Token argN;
		public TerminalNode LESS() { return getToken(GPULangParser.LESS, 0); }
		public TerminalNode GREATER() { return getToken(GPULangParser.GREATER, 0); }
		public List<TerminalNode> DOUBLELITERAL() { return getTokens(GPULangParser.DOUBLELITERAL); }
		public TerminalNode DOUBLELITERAL(int i) {
			return getToken(GPULangParser.DOUBLELITERAL, i);
		}
		public LinePreprocessorEntryContext linePreprocessorEntry() {
			return getRuleContext(LinePreprocessorEntryContext.class,0);
		}
		public List<TerminalNode> CO() { return getTokens(GPULangParser.CO); }
		public TerminalNode CO(int i) {
			return getToken(GPULangParser.CO, i);
		}
		public DoubleVecLiteralExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_doubleVecLiteralExpression; }
	}

	public final DoubleVecLiteralExpressionContext doubleVecLiteralExpression() throws RecognitionException {
		DoubleVecLiteralExpressionContext _localctx = new DoubleVecLiteralExpressionContext(_ctx, getState());
		enterRule(_localctx, 80, RULE_doubleVecLiteralExpression);

		        ((DoubleVecLiteralExpressionContext)_localctx).tree =  nullptr;
		        std::vector<float> values;
		        Symbol::Location location;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(1185);
			match(LESS);
			 location = SetupFile(); 
			{
			setState(1187);
			((DoubleVecLiteralExpressionContext)_localctx).arg0 = match(DOUBLELITERAL);
			 values.push_back(atof((((DoubleVecLiteralExpressionContext)_localctx).arg0!=null?((DoubleVecLiteralExpressionContext)_localctx).arg0.getText():null).c_str())); 
			}
			setState(1191);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__2) {
				{
				setState(1190);
				linePreprocessorEntry();
				}
			}

			setState(1196); 
			_errHandler.sync(this);
			_la = _input.LA(1);
			do {
				{
				{
				setState(1193);
				match(CO);
				setState(1194);
				((DoubleVecLiteralExpressionContext)_localctx).argN = match(DOUBLELITERAL);
				 values.push_back(atof((((DoubleVecLiteralExpressionContext)_localctx).argN!=null?((DoubleVecLiteralExpressionContext)_localctx).argN.getText():null).c_str())); 
				}
				}
				setState(1198); 
				_errHandler.sync(this);
				_la = _input.LA(1);
			} while ( _la==CO );
			setState(1200);
			match(GREATER);

			        ((DoubleVecLiteralExpressionContext)_localctx).tree =  Alloc<FloatVecExpression>(values);
			        _localctx.tree->location = location;
			    
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class IntVecLiteralExpressionContext extends ParserRuleContext {
		public Expression* tree;
		public Token arg0;
		public Token argN;
		public TerminalNode LESS() { return getToken(GPULangParser.LESS, 0); }
		public TerminalNode GREATER() { return getToken(GPULangParser.GREATER, 0); }
		public List<TerminalNode> INTEGERLITERAL() { return getTokens(GPULangParser.INTEGERLITERAL); }
		public TerminalNode INTEGERLITERAL(int i) {
			return getToken(GPULangParser.INTEGERLITERAL, i);
		}
		public LinePreprocessorEntryContext linePreprocessorEntry() {
			return getRuleContext(LinePreprocessorEntryContext.class,0);
		}
		public List<TerminalNode> CO() { return getTokens(GPULangParser.CO); }
		public TerminalNode CO(int i) {
			return getToken(GPULangParser.CO, i);
		}
		public IntVecLiteralExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_intVecLiteralExpression; }
	}

	public final IntVecLiteralExpressionContext intVecLiteralExpression() throws RecognitionException {
		IntVecLiteralExpressionContext _localctx = new IntVecLiteralExpressionContext(_ctx, getState());
		enterRule(_localctx, 82, RULE_intVecLiteralExpression);

		        ((IntVecLiteralExpressionContext)_localctx).tree =  nullptr;
		        std::vector<int> values;
		        Symbol::Location location;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(1203);
			match(LESS);
			 location = SetupFile(); 
			{
			setState(1205);
			((IntVecLiteralExpressionContext)_localctx).arg0 = match(INTEGERLITERAL);
			 values.push_back(atof((((IntVecLiteralExpressionContext)_localctx).arg0!=null?((IntVecLiteralExpressionContext)_localctx).arg0.getText():null).c_str())); 
			}
			setState(1209);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__2) {
				{
				setState(1208);
				linePreprocessorEntry();
				}
			}

			setState(1214); 
			_errHandler.sync(this);
			_la = _input.LA(1);
			do {
				{
				{
				setState(1211);
				match(CO);
				setState(1212);
				((IntVecLiteralExpressionContext)_localctx).argN = match(INTEGERLITERAL);
				 values.push_back(atof((((IntVecLiteralExpressionContext)_localctx).argN!=null?((IntVecLiteralExpressionContext)_localctx).argN.getText():null).c_str())); 
				}
				}
				setState(1216); 
				_errHandler.sync(this);
				_la = _input.LA(1);
			} while ( _la==CO );
			setState(1218);
			match(GREATER);

			        ((IntVecLiteralExpressionContext)_localctx).tree =  Alloc<IntVecExpression>(values);
			        _localctx.tree->location = location;
			    
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class UintVecLiteralExpressionContext extends ParserRuleContext {
		public Expression* tree;
		public Token arg0;
		public Token argN;
		public TerminalNode LESS() { return getToken(GPULangParser.LESS, 0); }
		public TerminalNode GREATER() { return getToken(GPULangParser.GREATER, 0); }
		public List<TerminalNode> UINTEGERLITERAL() { return getTokens(GPULangParser.UINTEGERLITERAL); }
		public TerminalNode UINTEGERLITERAL(int i) {
			return getToken(GPULangParser.UINTEGERLITERAL, i);
		}
		public LinePreprocessorEntryContext linePreprocessorEntry() {
			return getRuleContext(LinePreprocessorEntryContext.class,0);
		}
		public List<TerminalNode> CO() { return getTokens(GPULangParser.CO); }
		public TerminalNode CO(int i) {
			return getToken(GPULangParser.CO, i);
		}
		public UintVecLiteralExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_uintVecLiteralExpression; }
	}

	public final UintVecLiteralExpressionContext uintVecLiteralExpression() throws RecognitionException {
		UintVecLiteralExpressionContext _localctx = new UintVecLiteralExpressionContext(_ctx, getState());
		enterRule(_localctx, 84, RULE_uintVecLiteralExpression);

		        ((UintVecLiteralExpressionContext)_localctx).tree =  nullptr;
		        std::vector<unsigned int> values;
		        Symbol::Location location;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(1221);
			match(LESS);
			 location = SetupFile(); 
			{
			setState(1223);
			((UintVecLiteralExpressionContext)_localctx).arg0 = match(UINTEGERLITERAL);
			 values.push_back(atof((((UintVecLiteralExpressionContext)_localctx).arg0!=null?((UintVecLiteralExpressionContext)_localctx).arg0.getText():null).c_str())); 
			}
			setState(1227);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__2) {
				{
				setState(1226);
				linePreprocessorEntry();
				}
			}

			setState(1232); 
			_errHandler.sync(this);
			_la = _input.LA(1);
			do {
				{
				{
				setState(1229);
				match(CO);
				setState(1230);
				((UintVecLiteralExpressionContext)_localctx).argN = match(UINTEGERLITERAL);
				 values.push_back(atof((((UintVecLiteralExpressionContext)_localctx).argN!=null?((UintVecLiteralExpressionContext)_localctx).argN.getText():null).c_str())); 
				}
				}
				setState(1234); 
				_errHandler.sync(this);
				_la = _input.LA(1);
			} while ( _la==CO );
			setState(1236);
			match(GREATER);

			        ((UintVecLiteralExpressionContext)_localctx).tree =  Alloc<UIntVecExpression>(values);
			        _localctx.tree->location = location;
			    
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class BooleanVecLiteralExpressionContext extends ParserRuleContext {
		public Expression* tree;
		public BooleanContext arg0;
		public BooleanContext argN;
		public TerminalNode LESS() { return getToken(GPULangParser.LESS, 0); }
		public TerminalNode GREATER() { return getToken(GPULangParser.GREATER, 0); }
		public List<BooleanContext> boolean_() {
			return getRuleContexts(BooleanContext.class);
		}
		public BooleanContext boolean_(int i) {
			return getRuleContext(BooleanContext.class,i);
		}
		public LinePreprocessorEntryContext linePreprocessorEntry() {
			return getRuleContext(LinePreprocessorEntryContext.class,0);
		}
		public List<TerminalNode> CO() { return getTokens(GPULangParser.CO); }
		public TerminalNode CO(int i) {
			return getToken(GPULangParser.CO, i);
		}
		public BooleanVecLiteralExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_booleanVecLiteralExpression; }
	}

	public final BooleanVecLiteralExpressionContext booleanVecLiteralExpression() throws RecognitionException {
		BooleanVecLiteralExpressionContext _localctx = new BooleanVecLiteralExpressionContext(_ctx, getState());
		enterRule(_localctx, 86, RULE_booleanVecLiteralExpression);

		        ((BooleanVecLiteralExpressionContext)_localctx).tree =  nullptr;
		        std::vector<bool> values;
		        Symbol::Location location;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(1239);
			match(LESS);
			 location = SetupFile(); 
			{
			setState(1241);
			((BooleanVecLiteralExpressionContext)_localctx).arg0 = boolean_();
			 values.push_back(atof((((BooleanVecLiteralExpressionContext)_localctx).arg0!=null?_input.getText(((BooleanVecLiteralExpressionContext)_localctx).arg0.start,((BooleanVecLiteralExpressionContext)_localctx).arg0.stop):null).c_str())); 
			}
			setState(1245);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__2) {
				{
				setState(1244);
				linePreprocessorEntry();
				}
			}

			setState(1251); 
			_errHandler.sync(this);
			_la = _input.LA(1);
			do {
				{
				{
				setState(1247);
				match(CO);
				setState(1248);
				((BooleanVecLiteralExpressionContext)_localctx).argN = boolean_();
				 values.push_back(atof((((BooleanVecLiteralExpressionContext)_localctx).argN!=null?_input.getText(((BooleanVecLiteralExpressionContext)_localctx).argN.start,((BooleanVecLiteralExpressionContext)_localctx).argN.stop):null).c_str())); 
				}
				}
				setState(1253); 
				_errHandler.sync(this);
				_la = _input.LA(1);
			} while ( _la==CO );
			setState(1255);
			match(GREATER);

			        ((BooleanVecLiteralExpressionContext)_localctx).tree =  Alloc<BoolVecExpression>(values);
			        _localctx.tree->location = location;
			    
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public boolean sempred(RuleContext _localctx, int ruleIndex, int predIndex) {
		switch (ruleIndex) {
		case 33:
			return expressionNoComma_sempred((ExpressionNoCommaContext)_localctx, predIndex);
		case 35:
			return expression_sempred((ExpressionContext)_localctx, predIndex);
		}
		return true;
	}
	private boolean expressionNoComma_sempred(ExpressionNoCommaContext _localctx, int predIndex) {
		switch (predIndex) {
		case 0:
			return precpred(_ctx, 17);
		case 1:
			return precpred(_ctx, 16);
		case 2:
			return precpred(_ctx, 13);
		case 3:
			return precpred(_ctx, 12);
		case 4:
			return precpred(_ctx, 11);
		case 5:
			return precpred(_ctx, 10);
		case 6:
			return precpred(_ctx, 9);
		case 7:
			return precpred(_ctx, 8);
		case 8:
			return precpred(_ctx, 7);
		case 9:
			return precpred(_ctx, 6);
		case 10:
			return precpred(_ctx, 5);
		case 11:
			return precpred(_ctx, 4);
		case 12:
			return precpred(_ctx, 3);
		case 13:
			return precpred(_ctx, 2);
		case 14:
			return precpred(_ctx, 19);
		case 15:
			return precpred(_ctx, 18);
		case 16:
			return precpred(_ctx, 15);
		}
		return true;
	}
	private boolean expression_sempred(ExpressionContext _localctx, int predIndex) {
		switch (predIndex) {
		case 17:
			return precpred(_ctx, 18);
		case 18:
			return precpred(_ctx, 17);
		case 19:
			return precpred(_ctx, 14);
		case 20:
			return precpred(_ctx, 13);
		case 21:
			return precpred(_ctx, 12);
		case 22:
			return precpred(_ctx, 11);
		case 23:
			return precpred(_ctx, 10);
		case 24:
			return precpred(_ctx, 9);
		case 25:
			return precpred(_ctx, 8);
		case 26:
			return precpred(_ctx, 7);
		case 27:
			return precpred(_ctx, 6);
		case 28:
			return precpred(_ctx, 5);
		case 29:
			return precpred(_ctx, 4);
		case 30:
			return precpred(_ctx, 3);
		case 31:
			return precpred(_ctx, 2);
		case 32:
			return precpred(_ctx, 20);
		case 33:
			return precpred(_ctx, 19);
		case 34:
			return precpred(_ctx, 16);
		}
		return true;
	}

	public static final String _serializedATN =
		"\u0004\u0001W\u04eb\u0002\u0000\u0007\u0000\u0002\u0001\u0007\u0001\u0002"+
		"\u0002\u0007\u0002\u0002\u0003\u0007\u0003\u0002\u0004\u0007\u0004\u0002"+
		"\u0005\u0007\u0005\u0002\u0006\u0007\u0006\u0002\u0007\u0007\u0007\u0002"+
		"\b\u0007\b\u0002\t\u0007\t\u0002\n\u0007\n\u0002\u000b\u0007\u000b\u0002"+
		"\f\u0007\f\u0002\r\u0007\r\u0002\u000e\u0007\u000e\u0002\u000f\u0007\u000f"+
		"\u0002\u0010\u0007\u0010\u0002\u0011\u0007\u0011\u0002\u0012\u0007\u0012"+
		"\u0002\u0013\u0007\u0013\u0002\u0014\u0007\u0014\u0002\u0015\u0007\u0015"+
		"\u0002\u0016\u0007\u0016\u0002\u0017\u0007\u0017\u0002\u0018\u0007\u0018"+
		"\u0002\u0019\u0007\u0019\u0002\u001a\u0007\u001a\u0002\u001b\u0007\u001b"+
		"\u0002\u001c\u0007\u001c\u0002\u001d\u0007\u001d\u0002\u001e\u0007\u001e"+
		"\u0002\u001f\u0007\u001f\u0002 \u0007 \u0002!\u0007!\u0002\"\u0007\"\u0002"+
		"#\u0007#\u0002$\u0007$\u0002%\u0007%\u0002&\u0007&\u0002\'\u0007\'\u0002"+
		"(\u0007(\u0002)\u0007)\u0002*\u0007*\u0002+\u0007+\u0001\u0000\u0001\u0000"+
		"\u0001\u0000\u0005\u0000\\\b\u0000\n\u0000\f\u0000_\t\u0000\u0001\u0000"+
		"\u0001\u0000\u0001\u0000\u0001\u0000\u0005\u0000e\b\u0000\n\u0000\f\u0000"+
		"h\t\u0000\u0001\u0000\u0003\u0000k\b\u0000\u0001\u0001\u0001\u0001\u0001"+
		"\u0001\u0005\u0001p\b\u0001\n\u0001\f\u0001s\t\u0001\u0001\u0001\u0001"+
		"\u0001\u0001\u0001\u0001\u0001\u0005\u0001y\b\u0001\n\u0001\f\u0001|\t"+
		"\u0001\u0001\u0001\u0003\u0001\u007f\b\u0001\u0001\u0002\u0001\u0002\u0001"+
		"\u0002\u0001\u0002\u0003\u0002\u0085\b\u0002\u0001\u0003\u0001\u0003\u0001"+
		"\u0003\u0001\u0003\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001"+
		"\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001"+
		"\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001"+
		"\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001"+
		"\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001"+
		"\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001"+
		"\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0005"+
		"\u0004\u00b3\b\u0004\n\u0004\f\u0004\u00b6\t\u0004\u0001\u0005\u0001\u0005"+
		"\u0001\u0005\u0001\u0005\u0001\u0005\u0001\u0005\u0001\u0006\u0001\u0006"+
		"\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0007"+
		"\u0001\u0007\u0001\u0007\u0001\u0007\u0001\u0007\u0001\u0007\u0001\u0007"+
		"\u0001\u0007\u0001\u0007\u0001\b\u0001\b\u0001\b\u0001\b\u0001\b\u0001"+
		"\b\u0001\b\u0001\b\u0001\b\u0003\b\u00d7\b\b\u0001\t\u0001\t\u0001\t\u0001"+
		"\t\u0001\t\u0001\t\u0001\t\u0001\t\u0003\t\u00e1\b\t\u0001\t\u0001\t\u0001"+
		"\t\u0001\t\u0005\t\u00e7\b\t\n\t\f\t\u00ea\t\t\u0001\t\u0001\t\u0001\t"+
		"\u0001\n\u0001\n\u0001\n\u0001\n\u0001\n\u0005\n\u00f4\b\n\n\n\f\n\u00f7"+
		"\t\n\u0001\n\u0001\n\u0001\u000b\u0005\u000b\u00fc\b\u000b\n\u000b\f\u000b"+
		"\u00ff\t\u000b\u0001\u000b\u0001\u000b\u0001\u000b\u0005\u000b\u0104\b"+
		"\u000b\n\u000b\f\u000b\u0107\t\u000b\u0001\u000b\u0001\u000b\u0001\u000b"+
		"\u0004\u000b\u010c\b\u000b\u000b\u000b\f\u000b\u010d\u0001\u000b\u0001"+
		"\u000b\u0001\u000b\u0003\u000b\u0113\b\u000b\u0001\u000b\u0001\u000b\u0001"+
		"\u000b\u0001\u000b\u0005\u000b\u0119\b\u000b\n\u000b\f\u000b\u011c\t\u000b"+
		"\u0001\u000b\u0001\u000b\u0001\u000b\u0001\u000b\u0003\u000b\u0122\b\u000b"+
		"\u0001\u000b\u0001\u000b\u0001\u000b\u0001\u000b\u0001\u000b\u0001\u000b"+
		"\u0001\u000b\u0001\u000b\u0005\u000b\u012c\b\u000b\n\u000b\f\u000b\u012f"+
		"\t\u000b\u0003\u000b\u0131\b\u000b\u0001\u000b\u0001\u000b\u0001\f\u0005"+
		"\f\u0136\b\f\n\f\f\f\u0139\t\f\u0001\f\u0001\f\u0001\f\u0005\f\u013e\b"+
		"\f\n\f\f\f\u0141\t\f\u0001\f\u0001\f\u0001\f\u0005\f\u0146\b\f\n\f\f\f"+
		"\u0149\t\f\u0001\f\u0001\f\u0001\f\u0001\f\u0001\r\u0001\r\u0001\r\u0001"+
		"\r\u0001\r\u0001\r\u0001\r\u0001\r\u0001\r\u0001\r\u0001\r\u0001\r\u0001"+
		"\r\u0001\r\u0003\r\u015d\b\r\u0001\r\u0005\r\u0160\b\r\n\r\f\r\u0163\t"+
		"\r\u0001\r\u0001\r\u0001\r\u0001\r\u0001\r\u0005\r\u016a\b\r\n\r\f\r\u016d"+
		"\t\r\u0001\r\u0001\r\u0001\r\u0001\u000e\u0001\u000e\u0001\u000e\u0001"+
		"\u000e\u0001\u000e\u0001\u000e\u0001\u000e\u0003\u000e\u0179\b\u000e\u0001"+
		"\u000e\u0001\u000e\u0001\u000e\u0001\u000e\u0001\u000e\u0001\u000e\u0001"+
		"\u000e\u0003\u000e\u0182\b\u000e\u0001\u000e\u0001\u000e\u0003\u000e\u0186"+
		"\b\u000e\u0001\u000e\u0001\u000e\u0001\u000e\u0001\u000e\u0001\u000e\u0001"+
		"\u000e\u0001\u000e\u0003\u000e\u018f\b\u000e\u0001\u000e\u0001\u000e\u0005"+
		"\u000e\u0193\b\u000e\n\u000e\f\u000e\u0196\t\u000e\u0001\u000e\u0003\u000e"+
		"\u0199\b\u000e\u0001\u000e\u0001\u000e\u0001\u000e\u0001\u000f\u0005\u000f"+
		"\u019f\b\u000f\n\u000f\f\u000f\u01a2\t\u000f\u0001\u000f\u0001\u000f\u0001"+
		"\u000f\u0005\u000f\u01a7\b\u000f\n\u000f\f\u000f\u01aa\t\u000f\u0001\u000f"+
		"\u0001\u000f\u0001\u000f\u0001\u000f\u0001\u000f\u0001\u000f\u0001\u000f"+
		"\u0001\u000f\u0001\u000f\u0003\u000f\u01b5\b\u000f\u0001\u000f\u0001\u000f"+
		"\u0001\u0010\u0001\u0010\u0001\u0010\u0005\u0010\u01bc\b\u0010\n\u0010"+
		"\f\u0010\u01bf\t\u0010\u0001\u0010\u0001\u0010\u0001\u0010\u0001\u0010"+
		"\u0001\u0010\u0001\u0010\u0003\u0010\u01c7\b\u0010\u0001\u0010\u0001\u0010"+
		"\u0001\u0010\u0001\u0010\u0001\u0010\u0005\u0010\u01ce\b\u0010\n\u0010"+
		"\f\u0010\u01d1\t\u0010\u0003\u0010\u01d3\b\u0010\u0001\u0010\u0001\u0010"+
		"\u0001\u0010\u0001\u0010\u0001\u0011\u0001\u0011\u0001\u0011\u0001\u0011"+
		"\u0001\u0011\u0001\u0012\u0001\u0012\u0001\u0012\u0005\u0012\u01e1\b\u0012"+
		"\n\u0012\f\u0012\u01e4\t\u0012\u0001\u0012\u0001\u0012\u0001\u0012\u0001"+
		"\u0012\u0001\u0012\u0001\u0012\u0001\u0012\u0001\u0012\u0001\u0012\u0005"+
		"\u0012\u01ef\b\u0012\n\u0012\f\u0012\u01f2\t\u0012\u0001\u0012\u0001\u0012"+
		"\u0001\u0012\u0001\u0013\u0001\u0013\u0001\u0013\u0005\u0013\u01fa\b\u0013"+
		"\n\u0013\f\u0013\u01fd\t\u0013\u0001\u0013\u0001\u0013\u0001\u0013\u0005"+
		"\u0013\u0202\b\u0013\n\u0013\f\u0013\u0205\t\u0013\u0001\u0013\u0001\u0013"+
		"\u0001\u0013\u0001\u0013\u0001\u0013\u0001\u0013\u0001\u0013\u0001\u0013"+
		"\u0001\u0013\u0001\u0013\u0005\u0013\u0211\b\u0013\n\u0013\f\u0013\u0214"+
		"\t\u0013\u0001\u0013\u0001\u0013\u0001\u0013\u0001\u0014\u0001\u0014\u0001"+
		"\u0014\u0001\u0014\u0001\u0014\u0001\u0014\u0001\u0014\u0001\u0014\u0001"+
		"\u0014\u0001\u0014\u0005\u0014\u0223\b\u0014\n\u0014\f\u0014\u0226\t\u0014"+
		"\u0001\u0014\u0001\u0014\u0001\u0014\u0001\u0015\u0001\u0015\u0001\u0015"+
		"\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015"+
		"\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015"+
		"\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015"+
		"\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015"+
		"\u0001\u0015\u0003\u0015\u0247\b\u0015\u0001\u0016\u0001\u0016\u0001\u0016"+
		"\u0001\u0017\u0001\u0017\u0001\u0017\u0001\u0017\u0001\u0017\u0001\u0017"+
		"\u0001\u0017\u0001\u0017\u0001\u0017\u0001\u0017\u0001\u0017\u0001\u0017"+
		"\u0003\u0017\u0258\b\u0017\u0001\u0017\u0001\u0017\u0001\u0018\u0001\u0018"+
		"\u0001\u0018\u0001\u0018\u0001\u0018\u0001\u0018\u0003\u0018\u0262\b\u0018"+
		"\u0001\u0018\u0001\u0018\u0001\u0018\u0001\u0018\u0003\u0018\u0268\b\u0018"+
		"\u0001\u0018\u0001\u0018\u0001\u0018\u0001\u0018\u0003\u0018\u026e\b\u0018"+
		"\u0001\u0018\u0001\u0018\u0001\u0018\u0001\u0018\u0005\u0018\u0274\b\u0018"+
		"\n\u0018\f\u0018\u0277\t\u0018\u0001\u0018\u0001\u0018\u0001\u0018\u0001"+
		"\u0018\u0001\u0019\u0001\u0019\u0001\u0019\u0001\u0019\u0001\u0019\u0001"+
		"\u0019\u0001\u0019\u0001\u0019\u0001\u0019\u0001\u0019\u0001\u0019\u0001"+
		"\u0019\u0001\u0019\u0001\u001a\u0001\u001a\u0001\u001a\u0001\u001a\u0001"+
		"\u001a\u0001\u001a\u0001\u001a\u0001\u001a\u0001\u001a\u0001\u001b\u0001"+
		"\u001b\u0001\u001b\u0001\u001b\u0001\u001b\u0001\u001b\u0001\u001b\u0001"+
		"\u001b\u0001\u001b\u0001\u001b\u0001\u001b\u0001\u001b\u0001\u001b\u0001"+
		"\u001b\u0001\u001b\u0001\u001b\u0001\u001b\u0001\u001b\u0001\u001b\u0001"+
		"\u001b\u0001\u001b\u0001\u001b\u0003\u001b\u02a9\b\u001b\u0001\u001c\u0001"+
		"\u001c\u0001\u001c\u0001\u001c\u0001\u001c\u0001\u001c\u0001\u001c\u0001"+
		"\u001c\u0001\u001c\u0001\u001c\u0005\u001c\u02b5\b\u001c\n\u001c\f\u001c"+
		"\u02b8\t\u001c\u0001\u001c\u0001\u001c\u0001\u001c\u0001\u001c\u0001\u001d"+
		"\u0001\u001d\u0001\u001d\u0001\u001d\u0001\u001d\u0003\u001d\u02c3\b\u001d"+
		"\u0001\u001d\u0001\u001d\u0001\u001d\u0001\u001d\u0001\u001d\u0001\u001d"+
		"\u0001\u001d\u0001\u001d\u0001\u001d\u0001\u001d\u0001\u001d\u0001\u001d"+
		"\u0001\u001d\u0001\u001d\u0003\u001d\u02d3\b\u001d\u0001\u001e\u0001\u001e"+
		"\u0001\u001e\u0001\u001e\u0001\u001e\u0001\u001f\u0001\u001f\u0001\u001f"+
		"\u0001\u001f\u0001\u001f\u0001\u001f\u0001\u001f\u0001\u001f\u0001\u001f"+
		"\u0001\u001f\u0001\u001f\u0001\u001f\u0001\u001f\u0001\u001f\u0003\u001f"+
		"\u02e8\b\u001f\u0005\u001f\u02ea\b\u001f\n\u001f\f\u001f\u02ed\t\u001f"+
		"\u0001\u001f\u0001\u001f\u0001\u001f\u0001\u001f\u0001\u001f\u0003\u001f"+
		"\u02f4\b\u001f\u0003\u001f\u02f6\b\u001f\u0001\u001f\u0001\u001f\u0001"+
		"\u001f\u0001 \u0001 \u0001 \u0001 \u0001 \u0001!\u0001!\u0001!\u0001!"+
		"\u0001!\u0001!\u0001!\u0001!\u0003!\u0308\b!\u0001!\u0001!\u0001!\u0001"+
		"!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001"+
		"!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001"+
		"!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001"+
		"!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001"+
		"!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001"+
		"!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001"+
		"!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001"+
		"!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001"+
		"!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001"+
		"!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001"+
		"!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0003!\u0379\b!\u0001"+
		"!\u0001!\u0001!\u0001!\u0001!\u0005!\u0380\b!\n!\f!\u0383\t!\u0003!\u0385"+
		"\b!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0003!\u038e\b!\u0001"+
		"!\u0001!\u0005!\u0392\b!\n!\f!\u0395\t!\u0001\"\u0001\"\u0001\"\u0001"+
		"\"\u0001\"\u0001\"\u0005\"\u039d\b\"\n\"\f\"\u03a0\t\"\u0001\"\u0001\""+
		"\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0003#\u03ac"+
		"\b#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001"+
		"#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001"+
		"#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001"+
		"#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001"+
		"#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001"+
		"#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001"+
		"#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001"+
		"#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001"+
		"#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001"+
		"#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001"+
		"#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001"+
		"#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0003#\u0424"+
		"\b#\u0001#\u0001#\u0001#\u0001#\u0001#\u0005#\u042b\b#\n#\f#\u042e\t#"+
		"\u0003#\u0430\b#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0003"+
		"#\u0439\b#\u0001#\u0001#\u0005#\u043d\b#\n#\f#\u0440\t#\u0001$\u0001$"+
		"\u0001$\u0001$\u0001$\u0001$\u0001$\u0001$\u0001$\u0001$\u0001$\u0001"+
		"$\u0001$\u0001$\u0001$\u0001$\u0001$\u0001$\u0001$\u0001$\u0001$\u0001"+
		"$\u0001$\u0001$\u0001$\u0001$\u0001$\u0001$\u0001$\u0001$\u0003$\u0460"+
		"\b$\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0003%\u0469\b%\u0001"+
		"%\u0001%\u0001%\u0001%\u0001%\u0005%\u0470\b%\n%\f%\u0473\t%\u0003%\u0475"+
		"\b%\u0001%\u0001%\u0001%\u0001&\u0001&\u0001&\u0001&\u0001&\u0003&\u047f"+
		"\b&\u0001&\u0001&\u0001&\u0001&\u0001&\u0005&\u0486\b&\n&\f&\u0489\t&"+
		"\u0003&\u048b\b&\u0001&\u0001&\u0001&\u0001\'\u0001\'\u0001\'\u0001\'"+
		"\u0001\'\u0001\'\u0003\'\u0496\b\'\u0001\'\u0001\'\u0001\'\u0004\'\u049b"+
		"\b\'\u000b\'\f\'\u049c\u0001\'\u0001\'\u0001\'\u0001(\u0001(\u0001(\u0001"+
		"(\u0001(\u0001(\u0003(\u04a8\b(\u0001(\u0001(\u0001(\u0004(\u04ad\b(\u000b"+
		"(\f(\u04ae\u0001(\u0001(\u0001(\u0001)\u0001)\u0001)\u0001)\u0001)\u0001"+
		")\u0003)\u04ba\b)\u0001)\u0001)\u0001)\u0004)\u04bf\b)\u000b)\f)\u04c0"+
		"\u0001)\u0001)\u0001)\u0001*\u0001*\u0001*\u0001*\u0001*\u0001*\u0003"+
		"*\u04cc\b*\u0001*\u0001*\u0001*\u0004*\u04d1\b*\u000b*\f*\u04d2\u0001"+
		"*\u0001*\u0001*\u0001+\u0001+\u0001+\u0001+\u0001+\u0001+\u0003+\u04de"+
		"\b+\u0001+\u0001+\u0001+\u0001+\u0004+\u04e4\b+\u000b+\f+\u04e5\u0001"+
		"+\u0001+\u0001+\u0001+\u0001\u00b4\u0002BF,\u0000\u0002\u0004\u0006\b"+
		"\n\f\u000e\u0010\u0012\u0014\u0016\u0018\u001a\u001c\u001e \"$&(*,.02"+
		"468:<>@BDFHJLNPRTV\u0000\u000b\u0001\u000033\u0001\u0000<<\u0001\u0000"+
		"AA\u0005\u0000\u001c\u001d11;;JKMM\u0002\u0000GGLM\u0001\u0000JK\u0001"+
		"\u0000\u001e\u001f\u0001\u0000?B\u0001\u0000CD\u0005\u0000 &226688::\u0001"+
		"\u0000\u001c\u001d\u0565\u0000j\u0001\u0000\u0000\u0000\u0002~\u0001\u0000"+
		"\u0000\u0000\u0004\u0084\u0001\u0000\u0000\u0000\u0006\u0086\u0001\u0000"+
		"\u0000\u0000\b\u00b4\u0001\u0000\u0000\u0000\n\u00b7\u0001\u0000\u0000"+
		"\u0000\f\u00bd\u0001\u0000\u0000\u0000\u000e\u00c4\u0001\u0000\u0000\u0000"+
		"\u0010\u00d6\u0001\u0000\u0000\u0000\u0012\u00d8\u0001\u0000\u0000\u0000"+
		"\u0014\u00ee\u0001\u0000\u0000\u0000\u0016\u00fd\u0001\u0000\u0000\u0000"+
		"\u0018\u0137\u0001\u0000\u0000\u0000\u001a\u014e\u0001\u0000\u0000\u0000"+
		"\u001c\u0171\u0001\u0000\u0000\u0000\u001e\u01a0\u0001\u0000\u0000\u0000"+
		" \u01bd\u0001\u0000\u0000\u0000\"\u01d8\u0001\u0000\u0000\u0000$\u01e2"+
		"\u0001\u0000\u0000\u0000&\u01fb\u0001\u0000\u0000\u0000(\u0218\u0001\u0000"+
		"\u0000\u0000*\u0246\u0001\u0000\u0000\u0000,\u0248\u0001\u0000\u0000\u0000"+
		".\u024b\u0001\u0000\u0000\u00000\u025b\u0001\u0000\u0000\u00002\u027c"+
		"\u0001\u0000\u0000\u00004\u0289\u0001\u0000\u0000\u00006\u02a8\u0001\u0000"+
		"\u0000\u00008\u02aa\u0001\u0000\u0000\u0000:\u02d2\u0001\u0000\u0000\u0000"+
		"<\u02d4\u0001\u0000\u0000\u0000>\u02d9\u0001\u0000\u0000\u0000@\u02fa"+
		"\u0001\u0000\u0000\u0000B\u0307\u0001\u0000\u0000\u0000D\u0396\u0001\u0000"+
		"\u0000\u0000F\u03ab\u0001\u0000\u0000\u0000H\u045f\u0001\u0000\u0000\u0000"+
		"J\u0461\u0001\u0000\u0000\u0000L\u0479\u0001\u0000\u0000\u0000N\u048f"+
		"\u0001\u0000\u0000\u0000P\u04a1\u0001\u0000\u0000\u0000R\u04b3\u0001\u0000"+
		"\u0000\u0000T\u04c5\u0001\u0000\u0000\u0000V\u04d7\u0001\u0000\u0000\u0000"+
		"X]\u00053\u0000\u0000YZ\b\u0000\u0000\u0000Z\\\u0006\u0000\uffff\uffff"+
		"\u0000[Y\u0001\u0000\u0000\u0000\\_\u0001\u0000\u0000\u0000][\u0001\u0000"+
		"\u0000\u0000]^\u0001\u0000\u0000\u0000^`\u0001\u0000\u0000\u0000_]\u0001"+
		"\u0000\u0000\u0000`k\u00053\u0000\u0000af\u0005<\u0000\u0000bc\b\u0001"+
		"\u0000\u0000ce\u0006\u0000\uffff\uffff\u0000db\u0001\u0000\u0000\u0000"+
		"eh\u0001\u0000\u0000\u0000fd\u0001\u0000\u0000\u0000fg\u0001\u0000\u0000"+
		"\u0000gi\u0001\u0000\u0000\u0000hf\u0001\u0000\u0000\u0000ik\u0005<\u0000"+
		"\u0000jX\u0001\u0000\u0000\u0000ja\u0001\u0000\u0000\u0000k\u0001\u0001"+
		"\u0000\u0000\u0000lq\u00053\u0000\u0000mn\b\u0000\u0000\u0000np\u0006"+
		"\u0001\uffff\uffff\u0000om\u0001\u0000\u0000\u0000ps\u0001\u0000\u0000"+
		"\u0000qo\u0001\u0000\u0000\u0000qr\u0001\u0000\u0000\u0000rt\u0001\u0000"+
		"\u0000\u0000sq\u0001\u0000\u0000\u0000t\u007f\u00053\u0000\u0000uz\u0005"+
		"?\u0000\u0000vw\b\u0002\u0000\u0000wy\u0006\u0001\uffff\uffff\u0000xv"+
		"\u0001\u0000\u0000\u0000y|\u0001\u0000\u0000\u0000zx\u0001\u0000\u0000"+
		"\u0000z{\u0001\u0000\u0000\u0000{}\u0001\u0000\u0000\u0000|z\u0001\u0000"+
		"\u0000\u0000}\u007f\u0005A\u0000\u0000~l\u0001\u0000\u0000\u0000~u\u0001"+
		"\u0000\u0000\u0000\u007f\u0003\u0001\u0000\u0000\u0000\u0080\u0081\u0005"+
		"\u0001\u0000\u0000\u0081\u0085\u0006\u0002\uffff\uffff\u0000\u0082\u0083"+
		"\u0005\u0002\u0000\u0000\u0083\u0085\u0006\u0002\uffff\uffff\u0000\u0084"+
		"\u0080\u0001\u0000\u0000\u0000\u0084\u0082\u0001\u0000\u0000\u0000\u0085"+
		"\u0005\u0001\u0000\u0000\u0000\u0086\u0087\u0003\b\u0004\u0000\u0087\u0088"+
		"\u0006\u0003\uffff\uffff\u0000\u0088\u0089\u0005\u0000\u0000\u0001\u0089"+
		"\u0007\u0001\u0000\u0000\u0000\u008a\u00b3\u0003\n\u0005\u0000\u008b\u008c"+
		"\u0003\u0014\n\u0000\u008c\u008d\u0005\'\u0000\u0000\u008d\u008e\u0006"+
		"\u0004\uffff\uffff\u0000\u008e\u00b3\u0001\u0000\u0000\u0000\u008f\u0090"+
		"\u0003\f\u0006\u0000\u0090\u0091\u0005\'\u0000\u0000\u0091\u0092\u0006"+
		"\u0004\uffff\uffff\u0000\u0092\u00b3\u0001\u0000\u0000\u0000\u0093\u0094"+
		"\u0003 \u0010\u0000\u0094\u0095\u0005\'\u0000\u0000\u0095\u0096\u0006"+
		"\u0004\uffff\uffff\u0000\u0096\u00b3\u0001\u0000\u0000\u0000\u0097\u0098"+
		"\u0003\"\u0011\u0000\u0098\u0099\u0006\u0004\uffff\uffff\u0000\u0099\u00b3"+
		"\u0001\u0000\u0000\u0000\u009a\u009b\u0003\u0016\u000b\u0000\u009b\u009c"+
		"\u0005\'\u0000\u0000\u009c\u009d\u0006\u0004\uffff\uffff\u0000\u009d\u00b3"+
		"\u0001\u0000\u0000\u0000\u009e\u009f\u0003\u001a\r\u0000\u009f\u00a0\u0005"+
		"\'\u0000\u0000\u00a0\u00a1\u0006\u0004\uffff\uffff\u0000\u00a1\u00b3\u0001"+
		"\u0000\u0000\u0000\u00a2\u00a3\u0003\u001c\u000e\u0000\u00a3\u00a4\u0005"+
		"\'\u0000\u0000\u00a4\u00a5\u0006\u0004\uffff\uffff\u0000\u00a5\u00b3\u0001"+
		"\u0000\u0000\u0000\u00a6\u00a7\u0003(\u0014\u0000\u00a7\u00a8\u0005\'"+
		"\u0000\u0000\u00a8\u00a9\u0006\u0004\uffff\uffff\u0000\u00a9\u00b3\u0001"+
		"\u0000\u0000\u0000\u00aa\u00ab\u0003&\u0013\u0000\u00ab\u00ac\u0005\'"+
		"\u0000\u0000\u00ac\u00ad\u0006\u0004\uffff\uffff\u0000\u00ad\u00b3\u0001"+
		"\u0000\u0000\u0000\u00ae\u00af\u0003$\u0012\u0000\u00af\u00b0\u0005\'"+
		"\u0000\u0000\u00b0\u00b1\u0006\u0004\uffff\uffff\u0000\u00b1\u00b3\u0001"+
		"\u0000\u0000\u0000\u00b2\u008a\u0001\u0000\u0000\u0000\u00b2\u008b\u0001"+
		"\u0000\u0000\u0000\u00b2\u008f\u0001\u0000\u0000\u0000\u00b2\u0093\u0001"+
		"\u0000\u0000\u0000\u00b2\u0097\u0001\u0000\u0000\u0000\u00b2\u009a\u0001"+
		"\u0000\u0000\u0000\u00b2\u009e\u0001\u0000\u0000\u0000\u00b2\u00a2\u0001"+
		"\u0000\u0000\u0000\u00b2\u00a6\u0001\u0000\u0000\u0000\u00b2\u00aa\u0001"+
		"\u0000\u0000\u0000\u00b2\u00ae\u0001\u0000\u0000\u0000\u00b3\u00b6\u0001"+
		"\u0000\u0000\u0000\u00b4\u00b5\u0001\u0000\u0000\u0000\u00b4\u00b2\u0001"+
		"\u0000\u0000\u0000\u00b5\t\u0001\u0000\u0000\u0000\u00b6\u00b4\u0001\u0000"+
		"\u0000\u0000\u00b7\u00b8\u0005\u0003\u0000\u0000\u00b8\u00b9\u0006\u0005"+
		"\uffff\uffff\u0000\u00b9\u00ba\u0005O\u0000\u0000\u00ba\u00bb\u0003\u0000"+
		"\u0000\u0000\u00bb\u00bc\u0006\u0005\uffff\uffff\u0000\u00bc\u000b\u0001"+
		"\u0000\u0000\u0000\u00bd\u00be\u0005\u0004\u0000\u0000\u00be\u00bf\u0005"+
		"V\u0000\u0000\u00bf\u00c0\u0005\u0005\u0000\u0000\u00c0\u00c1\u0005V\u0000"+
		"\u0000\u00c1\u00c2\u0006\u0006\uffff\uffff\u0000\u00c2\u00c3\u0006\u0006"+
		"\uffff\uffff\u0000\u00c3\r\u0001\u0000\u0000\u0000\u00c4\u00c5\u0005I"+
		"\u0000\u0000\u00c5\u00c6\u0006\u0007\uffff\uffff\u0000\u00c6\u00c7\u0005"+
		"V\u0000\u0000\u00c7\u00c8\u0006\u0007\uffff\uffff\u0000\u00c8\u00c9\u0005"+
		"*\u0000\u0000\u00c9\u00ca\u0003F#\u0000\u00ca\u00cb\u0005+\u0000\u0000"+
		"\u00cb\u00cc\u0006\u0007\uffff\uffff\u0000\u00cc\u000f\u0001\u0000\u0000"+
		"\u0000\u00cd\u00ce\u0005V\u0000\u0000\u00ce\u00cf\u0006\b\uffff\uffff"+
		"\u0000\u00cf\u00d0\u0005*\u0000\u0000\u00d0\u00d1\u0003F#\u0000\u00d1"+
		"\u00d2\u0005+\u0000\u0000\u00d2\u00d3\u0006\b\uffff\uffff\u0000\u00d3"+
		"\u00d7\u0001\u0000\u0000\u0000\u00d4\u00d5\u0005V\u0000\u0000\u00d5\u00d7"+
		"\u0006\b\uffff\uffff\u0000\u00d6\u00cd\u0001\u0000\u0000\u0000\u00d6\u00d4"+
		"\u0001\u0000\u0000\u0000\u00d7\u0011\u0001\u0000\u0000\u0000\u00d8\u00e8"+
		"\u0006\t\uffff\uffff\u0000\u00d9\u00da\u0005M\u0000\u0000\u00da\u00e7"+
		"\u0006\t\uffff\uffff\u0000\u00db\u00dc\u0005.\u0000\u0000\u00dc\u00e0"+
		"\u0006\t\uffff\uffff\u0000\u00dd\u00de\u0003F#\u0000\u00de\u00df\u0006"+
		"\t\uffff\uffff\u0000\u00df\u00e1\u0001\u0000\u0000\u0000\u00e0\u00dd\u0001"+
		"\u0000\u0000\u0000\u00e0\u00e1\u0001\u0000\u0000\u0000\u00e1\u00e2\u0001"+
		"\u0000\u0000\u0000\u00e2\u00e7\u0005/\u0000\u0000\u00e3\u00e4\u0005V\u0000"+
		"\u0000\u00e4\u00e7\u0006\t\uffff\uffff\u0000\u00e5\u00e7\u0003\n\u0005"+
		"\u0000\u00e6\u00d9\u0001\u0000\u0000\u0000\u00e6\u00db\u0001\u0000\u0000"+
		"\u0000\u00e6\u00e3\u0001\u0000\u0000\u0000\u00e6\u00e5\u0001\u0000\u0000"+
		"\u0000\u00e7\u00ea\u0001\u0000\u0000\u0000\u00e8\u00e6\u0001\u0000\u0000"+
		"\u0000\u00e8\u00e9\u0001\u0000\u0000\u0000\u00e9\u00eb\u0001\u0000\u0000"+
		"\u0000\u00ea\u00e8\u0001\u0000\u0000\u0000\u00eb\u00ec\u0005V\u0000\u0000"+
		"\u00ec\u00ed\u0006\t\uffff\uffff\u0000\u00ed\u0013\u0001\u0000\u0000\u0000"+
		"\u00ee\u00ef\u0005\u0006\u0000\u0000\u00ef\u00f5\u0005?\u0000\u0000\u00f0"+
		"\u00f1\u0003*\u0015\u0000\u00f1\u00f2\u0006\n\uffff\uffff\u0000\u00f2"+
		"\u00f4\u0001\u0000\u0000\u0000\u00f3\u00f0\u0001\u0000\u0000\u0000\u00f4"+
		"\u00f7\u0001\u0000\u0000\u0000\u00f5\u00f3\u0001\u0000\u0000\u0000\u00f5"+
		"\u00f6\u0001\u0000\u0000\u0000\u00f6\u00f8\u0001\u0000\u0000\u0000\u00f7"+
		"\u00f5\u0001\u0000\u0000\u0000\u00f8\u00f9\u0005A\u0000\u0000\u00f9\u0015"+
		"\u0001\u0000\u0000\u0000\u00fa\u00fc\u0003\n\u0005\u0000\u00fb\u00fa\u0001"+
		"\u0000\u0000\u0000\u00fc\u00ff\u0001\u0000\u0000\u0000\u00fd\u00fb\u0001"+
		"\u0000\u0000\u0000\u00fd\u00fe\u0001\u0000\u0000\u0000\u00fe\u0105\u0001"+
		"\u0000\u0000\u0000\u00ff\u00fd\u0001\u0000\u0000\u0000\u0100\u0101\u0003"+
		"\u000e\u0007\u0000\u0101\u0102\u0006\u000b\uffff\uffff\u0000\u0102\u0104"+
		"\u0001\u0000\u0000\u0000\u0103\u0100\u0001\u0000\u0000\u0000\u0104\u0107"+
		"\u0001\u0000\u0000\u0000\u0105\u0103\u0001\u0000\u0000\u0000\u0105\u0106"+
		"\u0001\u0000\u0000\u0000\u0106\u010b\u0001\u0000\u0000\u0000\u0107\u0105"+
		"\u0001\u0000\u0000\u0000\u0108\u0109\u0003\u0010\b\u0000\u0109\u010a\u0006"+
		"\u000b\uffff\uffff\u0000\u010a\u010c\u0001\u0000\u0000\u0000\u010b\u0108"+
		"\u0001\u0000\u0000\u0000\u010c\u010d\u0001\u0000\u0000\u0000\u010d\u010b"+
		"\u0001\u0000\u0000\u0000\u010d\u010e\u0001\u0000\u0000\u0000\u010e\u010f"+
		"\u0001\u0000\u0000\u0000\u010f\u0110\u0005V\u0000\u0000\u0110\u0112\u0006"+
		"\u000b\uffff\uffff\u0000\u0111\u0113\u0003\n\u0005\u0000\u0112\u0111\u0001"+
		"\u0000\u0000\u0000\u0112\u0113\u0001\u0000\u0000\u0000\u0113\u011a\u0001"+
		"\u0000\u0000\u0000\u0114\u0115\u0005(\u0000\u0000\u0115\u0116\u0005V\u0000"+
		"\u0000\u0116\u0119\u0006\u000b\uffff\uffff\u0000\u0117\u0119\u0003\n\u0005"+
		"\u0000\u0118\u0114\u0001\u0000\u0000\u0000\u0118\u0117\u0001\u0000\u0000"+
		"\u0000\u0119\u011c\u0001\u0000\u0000\u0000\u011a\u0118\u0001\u0000\u0000"+
		"\u0000\u011a\u011b\u0001\u0000\u0000\u0000\u011b\u0121\u0001\u0000\u0000"+
		"\u0000\u011c\u011a\u0001\u0000\u0000\u0000\u011d\u011e\u0005)\u0000\u0000"+
		"\u011e\u011f\u0003\u0012\t\u0000\u011f\u0120\u0006\u000b\uffff\uffff\u0000"+
		"\u0120\u0122\u0001\u0000\u0000\u0000\u0121\u011d\u0001\u0000\u0000\u0000"+
		"\u0121\u0122\u0001\u0000\u0000\u0000\u0122\u0130\u0001\u0000\u0000\u0000"+
		"\u0123\u0124\u00052\u0000\u0000\u0124\u0125\u0003B!\u0000\u0125\u012d"+
		"\u0006\u000b\uffff\uffff\u0000\u0126\u0127\u0005(\u0000\u0000\u0127\u0128"+
		"\u0003B!\u0000\u0128\u0129\u0006\u000b\uffff\uffff\u0000\u0129\u012c\u0001"+
		"\u0000\u0000\u0000\u012a\u012c\u0003\n\u0005\u0000\u012b\u0126\u0001\u0000"+
		"\u0000\u0000\u012b\u012a\u0001\u0000\u0000\u0000\u012c\u012f\u0001\u0000"+
		"\u0000\u0000\u012d\u012b\u0001\u0000\u0000\u0000\u012d\u012e\u0001\u0000"+
		"\u0000\u0000\u012e\u0131\u0001\u0000\u0000\u0000\u012f\u012d\u0001\u0000"+
		"\u0000\u0000\u0130\u0123\u0001\u0000\u0000\u0000\u0130\u0131\u0001\u0000"+
		"\u0000\u0000\u0131\u0132\u0001\u0000\u0000\u0000\u0132\u0133\u0006\u000b"+
		"\uffff\uffff\u0000\u0133\u0017\u0001\u0000\u0000\u0000\u0134\u0136\u0003"+
		"\n\u0005\u0000\u0135\u0134\u0001\u0000\u0000\u0000\u0136\u0139\u0001\u0000"+
		"\u0000\u0000\u0137\u0135\u0001\u0000\u0000\u0000\u0137\u0138\u0001\u0000"+
		"\u0000\u0000\u0138\u013f\u0001\u0000\u0000\u0000\u0139\u0137\u0001\u0000"+
		"\u0000\u0000\u013a\u013b\u0003\u000e\u0007\u0000\u013b\u013c\u0006\f\uffff"+
		"\uffff\u0000\u013c\u013e\u0001\u0000\u0000\u0000\u013d\u013a\u0001\u0000"+
		"\u0000\u0000\u013e\u0141\u0001\u0000\u0000\u0000\u013f\u013d\u0001\u0000"+
		"\u0000\u0000\u013f\u0140\u0001\u0000\u0000\u0000\u0140\u0147\u0001\u0000"+
		"\u0000\u0000\u0141\u013f\u0001\u0000\u0000\u0000\u0142\u0143\u0003\u0010"+
		"\b\u0000\u0143\u0144\u0006\f\uffff\uffff\u0000\u0144\u0146\u0001\u0000"+
		"\u0000\u0000\u0145\u0142\u0001\u0000\u0000\u0000\u0146\u0149\u0001\u0000"+
		"\u0000\u0000\u0147\u0145\u0001\u0000\u0000\u0000\u0147\u0148\u0001\u0000"+
		"\u0000\u0000\u0148\u014a\u0001\u0000\u0000\u0000\u0149\u0147\u0001\u0000"+
		"\u0000\u0000\u014a\u014b\u0005\u0007\u0000\u0000\u014b\u014c\u0005V\u0000"+
		"\u0000\u014c\u014d\u0006\f\uffff\uffff\u0000\u014d\u0019\u0001\u0000\u0000"+
		"\u0000\u014e\u014f\u0003\u0018\f\u0000\u014f\u0150\u0006\r\uffff\uffff"+
		"\u0000\u0150\u016b\u0005,\u0000\u0000\u0151\u0152\u0005V\u0000\u0000\u0152"+
		"\u0153\u0006\r\uffff\uffff\u0000\u0153\u0154\u0005)\u0000\u0000\u0154"+
		"\u0161\u0006\r\uffff\uffff\u0000\u0155\u0156\u0005M\u0000\u0000\u0156"+
		"\u0160\u0006\r\uffff\uffff\u0000\u0157\u0158\u0005.\u0000\u0000\u0158"+
		"\u015c\u0006\r\uffff\uffff\u0000\u0159\u015a\u0003F#\u0000\u015a\u015b"+
		"\u0006\r\uffff\uffff\u0000\u015b\u015d\u0001\u0000\u0000\u0000\u015c\u0159"+
		"\u0001\u0000\u0000\u0000\u015c\u015d\u0001\u0000\u0000\u0000\u015d\u015e"+
		"\u0001\u0000\u0000\u0000\u015e\u0160\u0005/\u0000\u0000\u015f\u0155\u0001"+
		"\u0000\u0000\u0000\u015f\u0157\u0001\u0000\u0000\u0000\u0160\u0163\u0001"+
		"\u0000\u0000\u0000\u0161\u015f\u0001\u0000\u0000\u0000\u0161\u0162\u0001"+
		"\u0000\u0000\u0000\u0162\u0164\u0001\u0000\u0000\u0000\u0163\u0161\u0001"+
		"\u0000\u0000\u0000\u0164\u0165\u0005V\u0000\u0000\u0165\u0166\u0006\r"+
		"\uffff\uffff\u0000\u0166\u0167\u0005\'\u0000\u0000\u0167\u016a\u0006\r"+
		"\uffff\uffff\u0000\u0168\u016a\u0003\n\u0005\u0000\u0169\u0151\u0001\u0000"+
		"\u0000\u0000\u0169\u0168\u0001\u0000\u0000\u0000\u016a\u016d\u0001\u0000"+
		"\u0000\u0000\u016b\u0169\u0001\u0000\u0000\u0000\u016b\u016c\u0001\u0000"+
		"\u0000\u0000\u016c\u016e\u0001\u0000\u0000\u0000\u016d\u016b\u0001\u0000"+
		"\u0000\u0000\u016e\u016f\u0005-\u0000\u0000\u016f\u0170\u0006\r\uffff"+
		"\uffff\u0000\u0170\u001b\u0001\u0000\u0000\u0000\u0171\u0172\u0005\b\u0000"+
		"\u0000\u0172\u0173\u0005V\u0000\u0000\u0173\u0178\u0006\u000e\uffff\uffff"+
		"\u0000\u0174\u0175\u0005)\u0000\u0000\u0175\u0176\u0003\u0012\t\u0000"+
		"\u0176\u0177\u0006\u000e\uffff\uffff\u0000\u0177\u0179\u0001\u0000\u0000"+
		"\u0000\u0178\u0174\u0001\u0000\u0000\u0000\u0178\u0179\u0001\u0000\u0000"+
		"\u0000\u0179\u017a\u0001\u0000\u0000\u0000\u017a\u0198\u0005,\u0000\u0000"+
		"\u017b\u017c\u0005V\u0000\u0000\u017c\u0181\u0006\u000e\uffff\uffff\u0000"+
		"\u017d\u017e\u00052\u0000\u0000\u017e\u017f\u0003F#\u0000\u017f\u0180"+
		"\u0006\u000e\uffff\uffff\u0000\u0180\u0182\u0001\u0000\u0000\u0000\u0181"+
		"\u017d\u0001\u0000\u0000\u0000\u0181\u0182\u0001\u0000\u0000\u0000\u0182"+
		"\u0183\u0001\u0000\u0000\u0000\u0183\u0185\u0006\u000e\uffff\uffff\u0000"+
		"\u0184\u0186\u0003\n\u0005\u0000\u0185\u0184\u0001\u0000\u0000\u0000\u0185"+
		"\u0186\u0001\u0000\u0000\u0000\u0186\u0194\u0001\u0000\u0000\u0000\u0187"+
		"\u0188\u0005(\u0000\u0000\u0188\u0189\u0005V\u0000\u0000\u0189\u018e\u0006"+
		"\u000e\uffff\uffff\u0000\u018a\u018b\u00052\u0000\u0000\u018b\u018c\u0003"+
		"F#\u0000\u018c\u018d\u0006\u000e\uffff\uffff\u0000\u018d\u018f\u0001\u0000"+
		"\u0000\u0000\u018e\u018a\u0001\u0000\u0000\u0000\u018e\u018f\u0001\u0000"+
		"\u0000\u0000\u018f\u0190\u0001\u0000\u0000\u0000\u0190\u0193\u0006\u000e"+
		"\uffff\uffff\u0000\u0191\u0193\u0003\n\u0005\u0000\u0192\u0187\u0001\u0000"+
		"\u0000\u0000\u0192\u0191\u0001\u0000\u0000\u0000\u0193\u0196\u0001\u0000"+
		"\u0000\u0000\u0194\u0192\u0001\u0000\u0000\u0000\u0194\u0195\u0001\u0000"+
		"\u0000\u0000\u0195\u0199\u0001\u0000\u0000\u0000\u0196\u0194\u0001\u0000"+
		"\u0000\u0000\u0197\u0199\u0003\n\u0005\u0000\u0198\u017b\u0001\u0000\u0000"+
		"\u0000\u0198\u0197\u0001\u0000\u0000\u0000\u0198\u0199\u0001\u0000\u0000"+
		"\u0000\u0199\u019a\u0001\u0000\u0000\u0000\u019a\u019b\u0005-\u0000\u0000"+
		"\u019b\u019c\u0006\u000e\uffff\uffff\u0000\u019c\u001d\u0001\u0000\u0000"+
		"\u0000\u019d\u019f\u0003\n\u0005\u0000\u019e\u019d\u0001\u0000\u0000\u0000"+
		"\u019f\u01a2\u0001\u0000\u0000\u0000\u01a0\u019e\u0001\u0000\u0000\u0000"+
		"\u01a0\u01a1\u0001\u0000\u0000\u0000\u01a1\u01a8\u0001\u0000\u0000\u0000"+
		"\u01a2\u01a0\u0001\u0000\u0000\u0000\u01a3\u01a4\u0003\u0010\b\u0000\u01a4"+
		"\u01a5\u0006\u000f\uffff\uffff\u0000\u01a5\u01a7\u0001\u0000\u0000\u0000"+
		"\u01a6\u01a3\u0001\u0000\u0000\u0000\u01a7\u01aa\u0001\u0000\u0000\u0000"+
		"\u01a8\u01a6\u0001\u0000\u0000\u0000\u01a8\u01a9\u0001\u0000\u0000\u0000"+
		"\u01a9\u01ab\u0001\u0000\u0000\u0000\u01aa\u01a8\u0001\u0000\u0000\u0000"+
		"\u01ab\u01ac\u0005V\u0000\u0000\u01ac\u01ad\u0006\u000f\uffff\uffff\u0000"+
		"\u01ad\u01ae\u0005)\u0000\u0000\u01ae\u01af\u0003\u0012\t\u0000\u01af"+
		"\u01b4\u0006\u000f\uffff\uffff\u0000\u01b0\u01b1\u00052\u0000\u0000\u01b1"+
		"\u01b2\u0003B!\u0000\u01b2\u01b3\u0006\u000f\uffff\uffff\u0000\u01b3\u01b5"+
		"\u0001\u0000\u0000\u0000\u01b4\u01b0\u0001\u0000\u0000\u0000\u01b4\u01b5"+
		"\u0001\u0000\u0000\u0000\u01b5\u01b6\u0001\u0000\u0000\u0000\u01b6\u01b7"+
		"\u0006\u000f\uffff\uffff\u0000\u01b7\u001f\u0001\u0000\u0000\u0000\u01b8"+
		"\u01b9\u0003\u0010\b\u0000\u01b9\u01ba\u0006\u0010\uffff\uffff\u0000\u01ba"+
		"\u01bc\u0001\u0000\u0000\u0000\u01bb\u01b8\u0001\u0000\u0000\u0000\u01bc"+
		"\u01bf\u0001\u0000\u0000\u0000\u01bd\u01bb\u0001\u0000\u0000\u0000\u01bd"+
		"\u01be\u0001\u0000\u0000\u0000\u01be\u01c0\u0001\u0000\u0000\u0000\u01bf"+
		"\u01bd\u0001\u0000\u0000\u0000\u01c0\u01c1\u0005V\u0000\u0000\u01c1\u01c2"+
		"\u0006\u0010\uffff\uffff\u0000\u01c2\u01d2\u0005*\u0000\u0000\u01c3\u01c4"+
		"\u0003\u001e\u000f\u0000\u01c4\u01c6\u0006\u0010\uffff\uffff\u0000\u01c5"+
		"\u01c7\u0003\n\u0005\u0000\u01c6\u01c5\u0001\u0000\u0000\u0000\u01c6\u01c7"+
		"\u0001\u0000\u0000\u0000\u01c7\u01cf\u0001\u0000\u0000\u0000\u01c8\u01c9"+
		"\u0005(\u0000\u0000\u01c9\u01ca\u0003\u001e\u000f\u0000\u01ca\u01cb\u0006"+
		"\u0010\uffff\uffff\u0000\u01cb\u01ce\u0001\u0000\u0000\u0000\u01cc\u01ce"+
		"\u0003\n\u0005\u0000\u01cd\u01c8\u0001\u0000\u0000\u0000\u01cd\u01cc\u0001"+
		"\u0000\u0000\u0000\u01ce\u01d1\u0001\u0000\u0000\u0000\u01cf\u01cd\u0001"+
		"\u0000\u0000\u0000\u01cf\u01d0\u0001\u0000\u0000\u0000\u01d0\u01d3\u0001"+
		"\u0000\u0000\u0000\u01d1\u01cf\u0001\u0000\u0000\u0000\u01d2\u01c3\u0001"+
		"\u0000\u0000\u0000\u01d2\u01d3\u0001\u0000\u0000\u0000\u01d3\u01d4\u0001"+
		"\u0000\u0000\u0000\u01d4\u01d5\u0005+\u0000\u0000\u01d5\u01d6\u0003\u0012"+
		"\t\u0000\u01d6\u01d7\u0006\u0010\uffff\uffff\u0000\u01d7!\u0001\u0000"+
		"\u0000\u0000\u01d8\u01d9\u0003 \u0010\u0000\u01d9\u01da\u0006\u0011\uffff"+
		"\uffff\u0000\u01da\u01db\u00038\u001c\u0000\u01db\u01dc\u0006\u0011\uffff"+
		"\uffff\u0000\u01dc#\u0001\u0000\u0000\u0000\u01dd\u01de\u0003\u000e\u0007"+
		"\u0000\u01de\u01df\u0006\u0012\uffff\uffff\u0000\u01df\u01e1\u0001\u0000"+
		"\u0000\u0000\u01e0\u01dd\u0001\u0000\u0000\u0000\u01e1\u01e4\u0001\u0000"+
		"\u0000\u0000\u01e2\u01e0\u0001\u0000\u0000\u0000\u01e2\u01e3\u0001\u0000"+
		"\u0000\u0000\u01e3\u01e5\u0001\u0000\u0000\u0000\u01e4\u01e2\u0001\u0000"+
		"\u0000\u0000\u01e5\u01e6\u0005\t\u0000\u0000\u01e6\u01e7\u0006\u0012\uffff"+
		"\uffff\u0000\u01e7\u01e8\u0005V\u0000\u0000\u01e8\u01e9\u0006\u0012\uffff"+
		"\uffff\u0000\u01e9\u01f0\u0005,\u0000\u0000\u01ea\u01eb\u0003F#\u0000"+
		"\u01eb\u01ec\u0006\u0012\uffff\uffff\u0000\u01ec\u01ed\u0005\'\u0000\u0000"+
		"\u01ed\u01ef\u0001\u0000\u0000\u0000\u01ee\u01ea\u0001\u0000\u0000\u0000"+
		"\u01ef\u01f2\u0001\u0000\u0000\u0000\u01f0\u01ee\u0001\u0000\u0000\u0000"+
		"\u01f0\u01f1\u0001\u0000\u0000\u0000\u01f1\u01f3\u0001\u0000\u0000\u0000"+
		"\u01f2\u01f0\u0001\u0000\u0000\u0000\u01f3\u01f4\u0005-\u0000\u0000\u01f4"+
		"\u01f5\u0006\u0012\uffff\uffff\u0000\u01f5%\u0001\u0000\u0000\u0000\u01f6"+
		"\u01f7\u0003\u000e\u0007\u0000\u01f7\u01f8\u0006\u0013\uffff\uffff\u0000"+
		"\u01f8\u01fa\u0001\u0000\u0000\u0000\u01f9\u01f6\u0001\u0000\u0000\u0000"+
		"\u01fa\u01fd\u0001\u0000\u0000\u0000\u01fb\u01f9\u0001\u0000\u0000\u0000"+
		"\u01fb\u01fc\u0001\u0000\u0000\u0000\u01fc\u0203\u0001\u0000\u0000\u0000"+
		"\u01fd\u01fb\u0001\u0000\u0000\u0000\u01fe\u01ff\u0003\u0010\b\u0000\u01ff"+
		"\u0200\u0006\u0013\uffff\uffff\u0000\u0200\u0202\u0001\u0000\u0000\u0000"+
		"\u0201\u01fe\u0001\u0000\u0000\u0000\u0202\u0205\u0001\u0000\u0000\u0000"+
		"\u0203\u0201\u0001\u0000\u0000\u0000\u0203\u0204\u0001\u0000\u0000\u0000"+
		"\u0204\u0206\u0001\u0000\u0000\u0000\u0205\u0203\u0001\u0000\u0000\u0000"+
		"\u0206\u0207\u0005\n\u0000\u0000\u0207\u0208\u0006\u0013\uffff\uffff\u0000"+
		"\u0208\u0209\u0001\u0000\u0000\u0000\u0209\u020a\u0005V\u0000\u0000\u020a"+
		"\u020b\u0006\u0013\uffff\uffff\u0000\u020b\u0212\u0005,\u0000\u0000\u020c"+
		"\u020d\u0003F#\u0000\u020d\u020e\u0006\u0013\uffff\uffff\u0000\u020e\u020f"+
		"\u0005\'\u0000\u0000\u020f\u0211\u0001\u0000\u0000\u0000\u0210\u020c\u0001"+
		"\u0000\u0000\u0000\u0211\u0214\u0001\u0000\u0000\u0000\u0212\u0210\u0001"+
		"\u0000\u0000\u0000\u0212\u0213\u0001\u0000\u0000\u0000\u0213\u0215\u0001"+
		"\u0000\u0000\u0000\u0214\u0212\u0001\u0000\u0000\u0000\u0215\u0216\u0005"+
		"-\u0000\u0000\u0216\u0217\u0006\u0013\uffff\uffff\u0000\u0217\'\u0001"+
		"\u0000\u0000\u0000\u0218\u0219\u0005\u000b\u0000\u0000\u0219\u021a\u0006"+
		"\u0014\uffff\uffff\u0000\u021a\u021b\u0001\u0000\u0000\u0000\u021b\u021c"+
		"\u0005V\u0000\u0000\u021c\u021d\u0006\u0014\uffff\uffff\u0000\u021d\u0224"+
		"\u0005,\u0000\u0000\u021e\u021f\u0003F#\u0000\u021f\u0220\u0006\u0014"+
		"\uffff\uffff\u0000\u0220\u0221\u0005\'\u0000\u0000\u0221\u0223\u0001\u0000"+
		"\u0000\u0000\u0222\u021e\u0001\u0000\u0000\u0000\u0223\u0226\u0001\u0000"+
		"\u0000\u0000\u0224\u0222\u0001\u0000\u0000\u0000\u0224\u0225\u0001\u0000"+
		"\u0000\u0000\u0225\u0227\u0001\u0000\u0000\u0000\u0226\u0224\u0001\u0000"+
		"\u0000\u0000\u0227\u0228\u0005-\u0000\u0000\u0228\u0229\u0006\u0014\uffff"+
		"\uffff\u0000\u0229)\u0001\u0000\u0000\u0000\u022a\u022b\u0003.\u0017\u0000"+
		"\u022b\u022c\u0006\u0015\uffff\uffff\u0000\u022c\u0247\u0001\u0000\u0000"+
		"\u0000\u022d\u022e\u00038\u001c\u0000\u022e\u022f\u0006\u0015\uffff\uffff"+
		"\u0000\u022f\u0247\u0001\u0000\u0000\u0000\u0230\u0231\u00030\u0018\u0000"+
		"\u0231\u0232\u0006\u0015\uffff\uffff\u0000\u0232\u0247\u0001\u0000\u0000"+
		"\u0000\u0233\u0234\u00036\u001b\u0000\u0234\u0235\u0006\u0015\uffff\uffff"+
		"\u0000\u0235\u0247\u0001\u0000\u0000\u0000\u0236\u0237\u0003>\u001f\u0000"+
		"\u0237\u0238\u0006\u0015\uffff\uffff\u0000\u0238\u0247\u0001\u0000\u0000"+
		"\u0000\u0239\u023a\u0003:\u001d\u0000\u023a\u023b\u0006\u0015\uffff\uffff"+
		"\u0000\u023b\u0247\u0001\u0000\u0000\u0000\u023c\u023d\u0003<\u001e\u0000"+
		"\u023d\u023e\u0006\u0015\uffff\uffff\u0000\u023e\u0247\u0001\u0000\u0000"+
		"\u0000\u023f\u0240\u0003@ \u0000\u0240\u0241\u0006\u0015\uffff\uffff\u0000"+
		"\u0241\u0247\u0001\u0000\u0000\u0000\u0242\u0243\u0003,\u0016\u0000\u0243"+
		"\u0244\u0005\'\u0000\u0000\u0244\u0245\u0006\u0015\uffff\uffff\u0000\u0245"+
		"\u0247\u0001\u0000\u0000\u0000\u0246\u022a\u0001\u0000\u0000\u0000\u0246"+
		"\u022d\u0001\u0000\u0000\u0000\u0246\u0230\u0001\u0000\u0000\u0000\u0246"+
		"\u0233\u0001\u0000\u0000\u0000\u0246\u0236\u0001\u0000\u0000\u0000\u0246"+
		"\u0239\u0001\u0000\u0000\u0000\u0246\u023c\u0001\u0000\u0000\u0000\u0246"+
		"\u023f\u0001\u0000\u0000\u0000\u0246\u0242\u0001\u0000\u0000\u0000\u0247"+
		"+\u0001\u0000\u0000\u0000\u0248\u0249\u0003F#\u0000\u0249\u024a\u0006"+
		"\u0016\uffff\uffff\u0000\u024a-\u0001\u0000\u0000\u0000\u024b\u024c\u0005"+
		"\f\u0000\u0000\u024c\u024d\u0006\u0017\uffff\uffff\u0000\u024d\u024e\u0005"+
		"*\u0000\u0000\u024e\u024f\u0003F#\u0000\u024f\u0250\u0006\u0017\uffff"+
		"\uffff\u0000\u0250\u0251\u0005+\u0000\u0000\u0251\u0252\u0003*\u0015\u0000"+
		"\u0252\u0257\u0006\u0017\uffff\uffff\u0000\u0253\u0254\u0005\r\u0000\u0000"+
		"\u0254\u0255\u0003*\u0015\u0000\u0255\u0256\u0006\u0017\uffff\uffff\u0000"+
		"\u0256\u0258\u0001\u0000\u0000\u0000\u0257\u0253\u0001\u0000\u0000\u0000"+
		"\u0257\u0258\u0001\u0000\u0000\u0000\u0258\u0259\u0001\u0000\u0000\u0000"+
		"\u0259\u025a\u0006\u0017\uffff\uffff\u0000\u025a/\u0001\u0000\u0000\u0000"+
		"\u025b\u025c\u0005\u000e\u0000\u0000\u025c\u025d\u0006\u0018\uffff\uffff"+
		"\u0000\u025d\u0261\u0005*\u0000\u0000\u025e\u025f\u0003\u0016\u000b\u0000"+
		"\u025f\u0260\u0006\u0018\uffff\uffff\u0000\u0260\u0262\u0001\u0000\u0000"+
		"\u0000\u0261\u025e\u0001\u0000\u0000\u0000\u0261\u0262\u0001\u0000\u0000"+
		"\u0000\u0262\u0263\u0001\u0000\u0000\u0000\u0263\u0267\u0005\'\u0000\u0000"+
		"\u0264\u0265\u0003F#\u0000\u0265\u0266\u0006\u0018\uffff\uffff\u0000\u0266"+
		"\u0268\u0001\u0000\u0000\u0000\u0267\u0264\u0001\u0000\u0000\u0000\u0267"+
		"\u0268\u0001\u0000\u0000\u0000\u0268\u0269\u0001\u0000\u0000\u0000\u0269"+
		"\u026d\u0005\'\u0000\u0000\u026a\u026b\u0003F#\u0000\u026b\u026c\u0006"+
		"\u0018\uffff\uffff\u0000\u026c\u026e\u0001\u0000\u0000\u0000\u026d\u026a"+
		"\u0001\u0000\u0000\u0000\u026d\u026e\u0001\u0000\u0000\u0000\u026e\u026f"+
		"\u0001\u0000\u0000\u0000\u026f\u0275\u0005+\u0000\u0000\u0270\u0271\u0003"+
		"\u0010\b\u0000\u0271\u0272\u0006\u0018\uffff\uffff\u0000\u0272\u0274\u0001"+
		"\u0000\u0000\u0000\u0273\u0270\u0001\u0000\u0000\u0000\u0274\u0277\u0001"+
		"\u0000\u0000\u0000\u0275\u0273\u0001\u0000\u0000\u0000\u0275\u0276\u0001"+
		"\u0000\u0000\u0000\u0276\u0278\u0001\u0000\u0000\u0000\u0277\u0275\u0001"+
		"\u0000\u0000\u0000\u0278\u0279\u0003*\u0015\u0000\u0279\u027a\u0006\u0018"+
		"\uffff\uffff\u0000\u027a\u027b\u0006\u0018\uffff\uffff\u0000\u027b1\u0001"+
		"\u0000\u0000\u0000\u027c\u027d\u0005\u000e\u0000\u0000\u027d\u027e\u0006"+
		"\u0019\uffff\uffff\u0000\u027e\u027f\u0005*\u0000\u0000\u027f\u0280\u0005"+
		"V\u0000\u0000\u0280\u0281\u0005)\u0000\u0000\u0281\u0282\u0005V\u0000"+
		"\u0000\u0282\u0283\u0005\u000f\u0000\u0000\u0283\u0284\u0005V\u0000\u0000"+
		"\u0284\u0285\u0005+\u0000\u0000\u0285\u0286\u0003*\u0015\u0000\u0286\u0287"+
		"\u0006\u0019\uffff\uffff\u0000\u0287\u0288\u0006\u0019\uffff\uffff\u0000"+
		"\u02883\u0001\u0000\u0000\u0000\u0289\u028a\u0005\u0010\u0000\u0000\u028a"+
		"\u028b\u0006\u001a\uffff\uffff\u0000\u028b\u028c\u0005*\u0000\u0000\u028c"+
		"\u028d\u0003F#\u0000\u028d\u028e\u0005+\u0000\u0000\u028e\u028f\u0003"+
		"*\u0015\u0000\u028f\u0290\u0006\u001a\uffff\uffff\u0000\u0290\u0291\u0006"+
		"\u001a\uffff\uffff\u0000\u02915\u0001\u0000\u0000\u0000\u0292\u0293\u0005"+
		"\u0011\u0000\u0000\u0293\u0294\u0006\u001b\uffff\uffff\u0000\u0294\u0295"+
		"\u0005*\u0000\u0000\u0295\u0296\u0003F#\u0000\u0296\u0297\u0006\u001b"+
		"\uffff\uffff\u0000\u0297\u0298\u0005+\u0000\u0000\u0298\u0299\u0003*\u0015"+
		"\u0000\u0299\u029a\u0006\u001b\uffff\uffff\u0000\u029a\u029b\u0006\u001b"+
		"\uffff\uffff\u0000\u029b\u02a9\u0001\u0000\u0000\u0000\u029c\u029d\u0005"+
		"\u0012\u0000\u0000\u029d\u029e\u0006\u001b\uffff\uffff\u0000\u029e\u029f"+
		"\u0003*\u0015\u0000\u029f\u02a0\u0006\u001b\uffff\uffff\u0000\u02a0\u02a1"+
		"\u0005\u0011\u0000\u0000\u02a1\u02a2\u0005*\u0000\u0000\u02a2\u02a3\u0003"+
		"F#\u0000\u02a3\u02a4\u0006\u001b\uffff\uffff\u0000\u02a4\u02a5\u0005+"+
		"\u0000\u0000\u02a5\u02a6\u0005\'\u0000\u0000\u02a6\u02a7\u0006\u001b\uffff"+
		"\uffff\u0000\u02a7\u02a9\u0001\u0000\u0000\u0000\u02a8\u0292\u0001\u0000"+
		"\u0000\u0000\u02a8\u029c\u0001\u0000\u0000\u0000\u02a97\u0001\u0000\u0000"+
		"\u0000\u02aa\u02ab\u0005,\u0000\u0000\u02ab\u02b6\u0006\u001c\uffff\uffff"+
		"\u0000\u02ac\u02ad\u0003\u0016\u000b\u0000\u02ad\u02ae\u0005\'\u0000\u0000"+
		"\u02ae\u02af\u0006\u001c\uffff\uffff\u0000\u02af\u02b5\u0001\u0000\u0000"+
		"\u0000\u02b0\u02b1\u0003*\u0015\u0000\u02b1\u02b2\u0006\u001c\uffff\uffff"+
		"\u0000\u02b2\u02b5\u0001\u0000\u0000\u0000\u02b3\u02b5\u0003\n\u0005\u0000"+
		"\u02b4\u02ac\u0001\u0000\u0000\u0000\u02b4\u02b0\u0001\u0000\u0000\u0000"+
		"\u02b4\u02b3\u0001\u0000\u0000\u0000\u02b5\u02b8\u0001\u0000\u0000\u0000"+
		"\u02b6\u02b4\u0001\u0000\u0000\u0000\u02b6\u02b7\u0001\u0000\u0000\u0000"+
		"\u02b7\u02b9\u0001\u0000\u0000\u0000\u02b8\u02b6\u0001\u0000\u0000\u0000"+
		"\u02b9\u02ba\u0005-\u0000\u0000\u02ba\u02bb\u0006\u001c\uffff\uffff\u0000"+
		"\u02bb\u02bc\u0006\u001c\uffff\uffff\u0000\u02bc9\u0001\u0000\u0000\u0000"+
		"\u02bd\u02be\u0005\u0013\u0000\u0000\u02be\u02c2\u0006\u001d\uffff\uffff"+
		"\u0000\u02bf\u02c0\u0003F#\u0000\u02c0\u02c1\u0006\u001d\uffff\uffff\u0000"+
		"\u02c1\u02c3\u0001\u0000\u0000\u0000\u02c2\u02bf\u0001\u0000\u0000\u0000"+
		"\u02c2\u02c3\u0001\u0000\u0000\u0000\u02c3\u02c4\u0001\u0000\u0000\u0000"+
		"\u02c4\u02c5\u0005\'\u0000\u0000\u02c5\u02d3\u0006\u001d\uffff\uffff\u0000"+
		"\u02c6\u02c7\u0005\u0014\u0000\u0000\u02c7\u02c8\u0006\u001d\uffff\uffff"+
		"\u0000\u02c8\u02c9\u0005\'\u0000\u0000\u02c9\u02d3\u0006\u001d\uffff\uffff"+
		"\u0000\u02ca\u02cb\u0005\u0015\u0000\u0000\u02cb\u02cc\u0006\u001d\uffff"+
		"\uffff\u0000\u02cc\u02cd\u0005\'\u0000\u0000\u02cd\u02d3\u0006\u001d\uffff"+
		"\uffff\u0000\u02ce\u02cf\u0005\u0016\u0000\u0000\u02cf\u02d0\u0006\u001d"+
		"\uffff\uffff\u0000\u02d0\u02d1\u0005\'\u0000\u0000\u02d1\u02d3\u0006\u001d"+
		"\uffff\uffff\u0000\u02d2\u02bd\u0001\u0000\u0000\u0000\u02d2\u02c6\u0001"+
		"\u0000\u0000\u0000\u02d2\u02ca\u0001\u0000\u0000\u0000\u02d2\u02ce\u0001"+
		"\u0000\u0000\u0000\u02d3;\u0001\u0000\u0000\u0000\u02d4\u02d5\u0005\u0017"+
		"\u0000\u0000\u02d5\u02d6\u0006\u001e\uffff\uffff\u0000\u02d6\u02d7\u0005"+
		"\'\u0000\u0000\u02d7\u02d8\u0006\u001e\uffff\uffff\u0000\u02d8=\u0001"+
		"\u0000\u0000\u0000\u02d9\u02da\u0005\u0018\u0000\u0000\u02da\u02db\u0006"+
		"\u001f\uffff\uffff\u0000\u02db\u02dc\u0005*\u0000\u0000\u02dc\u02dd\u0003"+
		"F#\u0000\u02dd\u02de\u0005+\u0000\u0000\u02de\u02df\u0006\u001f\uffff"+
		"\uffff\u0000\u02df\u02eb\u0005,\u0000\u0000\u02e0\u02e1\u0005\u0019\u0000"+
		"\u0000\u02e1\u02e2\u0003F#\u0000\u02e2\u02e3\u0005)\u0000\u0000\u02e3"+
		"\u02e7\u0006\u001f\uffff\uffff\u0000\u02e4\u02e5\u0003*\u0015\u0000\u02e5"+
		"\u02e6\u0006\u001f\uffff\uffff\u0000\u02e6\u02e8\u0001\u0000\u0000\u0000"+
		"\u02e7\u02e4\u0001\u0000\u0000\u0000\u02e7\u02e8\u0001\u0000\u0000\u0000"+
		"\u02e8\u02ea\u0001\u0000\u0000\u0000\u02e9\u02e0\u0001\u0000\u0000\u0000"+
		"\u02ea\u02ed\u0001\u0000\u0000\u0000\u02eb\u02e9\u0001\u0000\u0000\u0000"+
		"\u02eb\u02ec\u0001\u0000\u0000\u0000\u02ec\u02f5\u0001\u0000\u0000\u0000"+
		"\u02ed\u02eb\u0001\u0000\u0000\u0000\u02ee\u02ef\u0005\u001a\u0000\u0000"+
		"\u02ef\u02f3\u0005)\u0000\u0000\u02f0\u02f1\u0003*\u0015\u0000\u02f1\u02f2"+
		"\u0006\u001f\uffff\uffff\u0000\u02f2\u02f4\u0001\u0000\u0000\u0000\u02f3"+
		"\u02f0\u0001\u0000\u0000\u0000\u02f3\u02f4\u0001\u0000\u0000\u0000\u02f4"+
		"\u02f6\u0001\u0000\u0000\u0000\u02f5\u02ee\u0001\u0000\u0000\u0000\u02f5"+
		"\u02f6\u0001\u0000\u0000\u0000\u02f6\u02f7\u0001\u0000\u0000\u0000\u02f7"+
		"\u02f8\u0005-\u0000\u0000\u02f8\u02f9\u0006\u001f\uffff\uffff\u0000\u02f9"+
		"?\u0001\u0000\u0000\u0000\u02fa\u02fb\u0005\u001b\u0000\u0000\u02fb\u02fc"+
		"\u0006 \uffff\uffff\u0000\u02fc\u02fd\u0005\'\u0000\u0000\u02fd\u02fe"+
		"\u0006 \uffff\uffff\u0000\u02feA\u0001\u0000\u0000\u0000\u02ff\u0300\u0006"+
		"!\uffff\uffff\u0000\u0300\u0301\u0007\u0003\u0000\u0000\u0301\u0302\u0003"+
		"B!\u000e\u0302\u0303\u0006!\uffff\uffff\u0000\u0303\u0308\u0001\u0000"+
		"\u0000\u0000\u0304\u0305\u0003H$\u0000\u0305\u0306\u0006!\uffff\uffff"+
		"\u0000\u0306\u0308\u0001\u0000\u0000\u0000\u0307\u02ff\u0001\u0000\u0000"+
		"\u0000\u0307\u0304\u0001\u0000\u0000\u0000\u0308\u0393\u0001\u0000\u0000"+
		"\u0000\u0309\u030a\n\u0011\u0000\u0000\u030a\u030b\u0006!\uffff\uffff"+
		"\u0000\u030b\u030c\u00050\u0000\u0000\u030c\u030d\u0006!\uffff\uffff\u0000"+
		"\u030d\u030e\u0003B!\u0012\u030e\u030f\u0006!\uffff\uffff\u0000\u030f"+
		"\u0392\u0001\u0000\u0000\u0000\u0310\u0311\n\u0010\u0000\u0000\u0311\u0312"+
		"\u0006!\uffff\uffff\u0000\u0312\u0313\u0005N\u0000\u0000\u0313\u0314\u0006"+
		"!\uffff\uffff\u0000\u0314\u0315\u0003B!\u0011\u0315\u0316\u0006!\uffff"+
		"\uffff\u0000\u0316\u0392\u0001\u0000\u0000\u0000\u0317\u0318\n\r\u0000"+
		"\u0000\u0318\u0319\u0006!\uffff\uffff\u0000\u0319\u031a\u0007\u0004\u0000"+
		"\u0000\u031a\u031b\u0006!\uffff\uffff\u0000\u031b\u031c\u0003B!\u000e"+
		"\u031c\u031d\u0006!\uffff\uffff\u0000\u031d\u0392\u0001\u0000\u0000\u0000"+
		"\u031e\u031f\n\f\u0000\u0000\u031f\u0320\u0006!\uffff\uffff\u0000\u0320"+
		"\u0321\u0007\u0005\u0000\u0000\u0321\u0322\u0006!\uffff\uffff\u0000\u0322"+
		"\u0323\u0003B!\r\u0323\u0324\u0006!\uffff\uffff\u0000\u0324\u0392\u0001"+
		"\u0000\u0000\u0000\u0325\u0326\n\u000b\u0000\u0000\u0326\u0327\u0006!"+
		"\uffff\uffff\u0000\u0327\u0328\u0007\u0006\u0000\u0000\u0328\u0329\u0006"+
		"!\uffff\uffff\u0000\u0329\u032a\u0003B!\f\u032a\u032b\u0006!\uffff\uffff"+
		"\u0000\u032b\u0392\u0001\u0000\u0000\u0000\u032c\u032d\n\n\u0000\u0000"+
		"\u032d\u032e\u0006!\uffff\uffff\u0000\u032e\u032f\u0007\u0007\u0000\u0000"+
		"\u032f\u0330\u0006!\uffff\uffff\u0000\u0330\u0331\u0003B!\u000b\u0331"+
		"\u0332\u0006!\uffff\uffff\u0000\u0332\u0392\u0001\u0000\u0000\u0000\u0333"+
		"\u0334\n\t\u0000\u0000\u0334\u0335\u0006!\uffff\uffff\u0000\u0335\u0336"+
		"\u0007\b\u0000\u0000\u0336\u0337\u0006!\uffff\uffff\u0000\u0337\u0338"+
		"\u0003B!\n\u0338\u0339\u0006!\uffff\uffff\u0000\u0339\u0392\u0001\u0000"+
		"\u0000\u0000\u033a\u033b\n\b\u0000\u0000\u033b\u033c\u0006!\uffff\uffff"+
		"\u0000\u033c\u033d\u00055\u0000\u0000\u033d\u033e\u0006!\uffff\uffff\u0000"+
		"\u033e\u033f\u0003B!\t\u033f\u0340\u0006!\uffff\uffff\u0000\u0340\u0392"+
		"\u0001\u0000\u0000\u0000\u0341\u0342\n\u0007\u0000\u0000\u0342\u0343\u0006"+
		"!\uffff\uffff\u0000\u0343\u0344\u00059\u0000\u0000\u0344\u0345\u0006!"+
		"\uffff\uffff\u0000\u0345\u0346\u0003B!\b\u0346\u0347\u0006!\uffff\uffff"+
		"\u0000\u0347\u0392\u0001\u0000\u0000\u0000\u0348\u0349\n\u0006\u0000\u0000"+
		"\u0349\u034a\u0006!\uffff\uffff\u0000\u034a\u034b\u00057\u0000\u0000\u034b"+
		"\u034c\u0006!\uffff\uffff\u0000\u034c\u034d\u0003B!\u0007\u034d\u034e"+
		"\u0006!\uffff\uffff\u0000\u034e\u0392\u0001\u0000\u0000\u0000\u034f\u0350"+
		"\n\u0005\u0000\u0000\u0350\u0351\u0006!\uffff\uffff\u0000\u0351\u0352"+
		"\u0005E\u0000\u0000\u0352\u0353\u0006!\uffff\uffff\u0000\u0353\u0354\u0003"+
		"B!\u0006\u0354\u0355\u0006!\uffff\uffff\u0000\u0355\u0392\u0001\u0000"+
		"\u0000\u0000\u0356\u0357\n\u0004\u0000\u0000\u0357\u0358\u0006!\uffff"+
		"\uffff\u0000\u0358\u0359\u0005F\u0000\u0000\u0359\u035a\u0006!\uffff\uffff"+
		"\u0000\u035a\u035b\u0003B!\u0005\u035b\u035c\u0006!\uffff\uffff\u0000"+
		"\u035c\u0392\u0001\u0000\u0000\u0000\u035d\u035e\n\u0003\u0000\u0000\u035e"+
		"\u035f\u00054\u0000\u0000\u035f\u0360\u0006!\uffff\uffff\u0000\u0360\u0361"+
		"\u0003B!\u0000\u0361\u0362\u0005)\u0000\u0000\u0362\u0363\u0003B!\u0003"+
		"\u0363\u0364\u0006!\uffff\uffff\u0000\u0364\u0392\u0001\u0000\u0000\u0000"+
		"\u0365\u0366\n\u0002\u0000\u0000\u0366\u0367\u0006!\uffff\uffff\u0000"+
		"\u0367\u0368\u0007\t\u0000\u0000\u0368\u0369\u0006!\uffff\uffff\u0000"+
		"\u0369\u036a\u0003B!\u0002\u036a\u036b\u0006!\uffff\uffff\u0000\u036b"+
		"\u0392\u0001\u0000\u0000\u0000\u036c\u036d\n\u0013\u0000\u0000\u036d\u036e"+
		"\u0006!\uffff\uffff\u0000\u036e\u036f\u0007\n\u0000\u0000\u036f\u0370"+
		"\u0006!\uffff\uffff\u0000\u0370\u0392\u0006!\uffff\uffff\u0000\u0371\u0372"+
		"\n\u0012\u0000\u0000\u0372\u0373\u0006!\uffff\uffff\u0000\u0373\u0374"+
		"\u0005*\u0000\u0000\u0374\u0384\u0006!\uffff\uffff\u0000\u0375\u0376\u0003"+
		"B!\u0000\u0376\u0378\u0006!\uffff\uffff\u0000\u0377\u0379\u0003\n\u0005"+
		"\u0000\u0378\u0377\u0001\u0000\u0000\u0000\u0378\u0379\u0001\u0000\u0000"+
		"\u0000\u0379\u0381\u0001\u0000\u0000\u0000\u037a\u037b\u0005(\u0000\u0000"+
		"\u037b\u037c\u0003B!\u0000\u037c\u037d\u0006!\uffff\uffff\u0000\u037d"+
		"\u0380\u0001\u0000\u0000\u0000\u037e\u0380\u0003\n\u0005\u0000\u037f\u037a"+
		"\u0001\u0000\u0000\u0000\u037f\u037e\u0001\u0000\u0000\u0000\u0380\u0383"+
		"\u0001\u0000\u0000\u0000\u0381\u037f\u0001\u0000\u0000\u0000\u0381\u0382"+
		"\u0001\u0000\u0000\u0000\u0382\u0385\u0001\u0000\u0000\u0000\u0383\u0381"+
		"\u0001\u0000\u0000\u0000\u0384\u0375\u0001\u0000\u0000\u0000\u0384\u0385"+
		"\u0001\u0000\u0000\u0000\u0385\u0386\u0001\u0000\u0000\u0000\u0386\u0387"+
		"\u0005+\u0000\u0000\u0387\u0392\u0006!\uffff\uffff\u0000\u0388\u0389\n"+
		"\u000f\u0000\u0000\u0389\u038a\u0006!\uffff\uffff\u0000\u038a\u038b\u0005"+
		".\u0000\u0000\u038b\u038d\u0006!\uffff\uffff\u0000\u038c\u038e\u0003B"+
		"!\u0000\u038d\u038c\u0001\u0000\u0000\u0000\u038d\u038e\u0001\u0000\u0000"+
		"\u0000\u038e\u038f\u0001\u0000\u0000\u0000\u038f\u0390\u0005/\u0000\u0000"+
		"\u0390\u0392\u0006!\uffff\uffff\u0000\u0391\u0309\u0001\u0000\u0000\u0000"+
		"\u0391\u0310\u0001\u0000\u0000\u0000\u0391\u0317\u0001\u0000\u0000\u0000"+
		"\u0391\u031e\u0001\u0000\u0000\u0000\u0391\u0325\u0001\u0000\u0000\u0000"+
		"\u0391\u032c\u0001\u0000\u0000\u0000\u0391\u0333\u0001\u0000\u0000\u0000"+
		"\u0391\u033a\u0001\u0000\u0000\u0000\u0391\u0341\u0001\u0000\u0000\u0000"+
		"\u0391\u0348\u0001\u0000\u0000\u0000\u0391\u034f\u0001\u0000\u0000\u0000"+
		"\u0391\u0356\u0001\u0000\u0000\u0000\u0391\u035d\u0001\u0000\u0000\u0000"+
		"\u0391\u0365\u0001\u0000\u0000\u0000\u0391\u036c\u0001\u0000\u0000\u0000"+
		"\u0391\u0371\u0001\u0000\u0000\u0000\u0391\u0388\u0001\u0000\u0000\u0000"+
		"\u0392\u0395\u0001\u0000\u0000\u0000\u0393\u0391\u0001\u0000\u0000\u0000"+
		"\u0393\u0394\u0001\u0000\u0000\u0000\u0394C\u0001\u0000\u0000\u0000\u0395"+
		"\u0393\u0001\u0000\u0000\u0000\u0396\u0397\u0003F#\u0000\u0397\u039e\u0006"+
		"\"\uffff\uffff\u0000\u0398\u0399\u0005(\u0000\u0000\u0399\u039a\u0003"+
		"F#\u0000\u039a\u039b\u0006\"\uffff\uffff\u0000\u039b\u039d\u0001\u0000"+
		"\u0000\u0000\u039c\u0398\u0001\u0000\u0000\u0000\u039d\u03a0\u0001\u0000"+
		"\u0000\u0000\u039e\u039c\u0001\u0000\u0000\u0000\u039e\u039f\u0001\u0000"+
		"\u0000\u0000\u039f\u03a1\u0001\u0000\u0000\u0000\u03a0\u039e\u0001\u0000"+
		"\u0000\u0000\u03a1\u03a2\u0006\"\uffff\uffff\u0000\u03a2E\u0001\u0000"+
		"\u0000\u0000\u03a3\u03a4\u0006#\uffff\uffff\u0000\u03a4\u03a5\u0007\u0003"+
		"\u0000\u0000\u03a5\u03a6\u0003F#\u000f\u03a6\u03a7\u0006#\uffff\uffff"+
		"\u0000\u03a7\u03ac\u0001\u0000\u0000\u0000\u03a8\u03a9\u0003H$\u0000\u03a9"+
		"\u03aa\u0006#\uffff\uffff\u0000\u03aa\u03ac\u0001\u0000\u0000\u0000\u03ab"+
		"\u03a3\u0001\u0000\u0000\u0000\u03ab\u03a8\u0001\u0000\u0000\u0000\u03ac"+
		"\u043e\u0001\u0000\u0000\u0000\u03ad\u03ae\n\u0012\u0000\u0000\u03ae\u03af"+
		"\u0006#\uffff\uffff\u0000\u03af\u03b0\u00050\u0000\u0000\u03b0\u03b1\u0006"+
		"#\uffff\uffff\u0000\u03b1\u03b2\u0003F#\u0013\u03b2\u03b3\u0006#\uffff"+
		"\uffff\u0000\u03b3\u043d\u0001\u0000\u0000\u0000\u03b4\u03b5\n\u0011\u0000"+
		"\u0000\u03b5\u03b6\u0006#\uffff\uffff\u0000\u03b6\u03b7\u0005N\u0000\u0000"+
		"\u03b7\u03b8\u0006#\uffff\uffff\u0000\u03b8\u03b9\u0003F#\u0012\u03b9"+
		"\u03ba\u0006#\uffff\uffff\u0000\u03ba\u043d\u0001\u0000\u0000\u0000\u03bb"+
		"\u03bc\n\u000e\u0000\u0000\u03bc\u03bd\u0006#\uffff\uffff\u0000\u03bd"+
		"\u03be\u0007\u0004\u0000\u0000\u03be\u03bf\u0006#\uffff\uffff\u0000\u03bf"+
		"\u03c0\u0003F#\u000f\u03c0\u03c1\u0006#\uffff\uffff\u0000\u03c1\u043d"+
		"\u0001\u0000\u0000\u0000\u03c2\u03c3\n\r\u0000\u0000\u03c3\u03c4\u0006"+
		"#\uffff\uffff\u0000\u03c4\u03c5\u0007\u0005\u0000\u0000\u03c5\u03c6\u0006"+
		"#\uffff\uffff\u0000\u03c6\u03c7\u0003F#\u000e\u03c7\u03c8\u0006#\uffff"+
		"\uffff\u0000\u03c8\u043d\u0001\u0000\u0000\u0000\u03c9\u03ca\n\f\u0000"+
		"\u0000\u03ca\u03cb\u0006#\uffff\uffff\u0000\u03cb\u03cc\u0007\u0006\u0000"+
		"\u0000\u03cc\u03cd\u0006#\uffff\uffff\u0000\u03cd\u03ce\u0003F#\r\u03ce"+
		"\u03cf\u0006#\uffff\uffff\u0000\u03cf\u043d\u0001\u0000\u0000\u0000\u03d0"+
		"\u03d1\n\u000b\u0000\u0000\u03d1\u03d2\u0006#\uffff\uffff\u0000\u03d2"+
		"\u03d3\u0007\u0007\u0000\u0000\u03d3\u03d4\u0006#\uffff\uffff\u0000\u03d4"+
		"\u03d5\u0003F#\f\u03d5\u03d6\u0006#\uffff\uffff\u0000\u03d6\u043d\u0001"+
		"\u0000\u0000\u0000\u03d7\u03d8\n\n\u0000\u0000\u03d8\u03d9\u0006#\uffff"+
		"\uffff\u0000\u03d9\u03da\u0007\b\u0000\u0000\u03da\u03db\u0006#\uffff"+
		"\uffff\u0000\u03db\u03dc\u0003F#\u000b\u03dc\u03dd\u0006#\uffff\uffff"+
		"\u0000\u03dd\u043d\u0001\u0000\u0000\u0000\u03de\u03df\n\t\u0000\u0000"+
		"\u03df\u03e0\u0006#\uffff\uffff\u0000\u03e0\u03e1\u00055\u0000\u0000\u03e1"+
		"\u03e2\u0006#\uffff\uffff\u0000\u03e2\u03e3\u0003F#\n\u03e3\u03e4\u0006"+
		"#\uffff\uffff\u0000\u03e4\u043d\u0001\u0000\u0000\u0000\u03e5\u03e6\n"+
		"\b\u0000\u0000\u03e6\u03e7\u0006#\uffff\uffff\u0000\u03e7\u03e8\u0005"+
		"9\u0000\u0000\u03e8\u03e9\u0006#\uffff\uffff\u0000\u03e9\u03ea\u0003F"+
		"#\t\u03ea\u03eb\u0006#\uffff\uffff\u0000\u03eb\u043d\u0001\u0000\u0000"+
		"\u0000\u03ec\u03ed\n\u0007\u0000\u0000\u03ed\u03ee\u0006#\uffff\uffff"+
		"\u0000\u03ee\u03ef\u00057\u0000\u0000\u03ef\u03f0\u0006#\uffff\uffff\u0000"+
		"\u03f0\u03f1\u0003F#\b\u03f1\u03f2\u0006#\uffff\uffff\u0000\u03f2\u043d"+
		"\u0001\u0000\u0000\u0000\u03f3\u03f4\n\u0006\u0000\u0000\u03f4\u03f5\u0006"+
		"#\uffff\uffff\u0000\u03f5\u03f6\u0005E\u0000\u0000\u03f6\u03f7\u0006#"+
		"\uffff\uffff\u0000\u03f7\u03f8\u0003F#\u0007\u03f8\u03f9\u0006#\uffff"+
		"\uffff\u0000\u03f9\u043d\u0001\u0000\u0000\u0000\u03fa\u03fb\n\u0005\u0000"+
		"\u0000\u03fb\u03fc\u0006#\uffff\uffff\u0000\u03fc\u03fd\u0005F\u0000\u0000"+
		"\u03fd\u03fe\u0006#\uffff\uffff\u0000\u03fe\u03ff\u0003F#\u0006\u03ff"+
		"\u0400\u0006#\uffff\uffff\u0000\u0400\u043d\u0001\u0000\u0000\u0000\u0401"+
		"\u0402\n\u0004\u0000\u0000\u0402\u0403\u00054\u0000\u0000\u0403\u0404"+
		"\u0006#\uffff\uffff\u0000\u0404\u0405\u0003F#\u0000\u0405\u0406\u0005"+
		")\u0000\u0000\u0406\u0407\u0003F#\u0004\u0407\u0408\u0006#\uffff\uffff"+
		"\u0000\u0408\u043d\u0001\u0000\u0000\u0000\u0409\u040a\n\u0003\u0000\u0000"+
		"\u040a\u040b\u0006#\uffff\uffff\u0000\u040b\u040c\u0007\t\u0000\u0000"+
		"\u040c\u040d\u0006#\uffff\uffff\u0000\u040d\u040e\u0003F#\u0003\u040e"+
		"\u040f\u0006#\uffff\uffff\u0000\u040f\u043d\u0001\u0000\u0000\u0000\u0410"+
		"\u0411\n\u0002\u0000\u0000\u0411\u0412\u0006#\uffff\uffff\u0000\u0412"+
		"\u0413\u0005(\u0000\u0000\u0413\u0414\u0006#\uffff\uffff\u0000\u0414\u0415"+
		"\u0003F#\u0003\u0415\u0416\u0006#\uffff\uffff\u0000\u0416\u043d\u0001"+
		"\u0000\u0000\u0000\u0417\u0418\n\u0014\u0000\u0000\u0418\u0419\u0006#"+
		"\uffff\uffff\u0000\u0419\u041a\u0007\n\u0000\u0000\u041a\u041b\u0006#"+
		"\uffff\uffff\u0000\u041b\u043d\u0006#\uffff\uffff\u0000\u041c\u041d\n"+
		"\u0013\u0000\u0000\u041d\u041e\u0006#\uffff\uffff\u0000\u041e\u041f\u0005"+
		"*\u0000\u0000\u041f\u042f\u0006#\uffff\uffff\u0000\u0420\u0421\u0003B"+
		"!\u0000\u0421\u0423\u0006#\uffff\uffff\u0000\u0422\u0424\u0003\n\u0005"+
		"\u0000\u0423\u0422\u0001\u0000\u0000\u0000\u0423\u0424\u0001\u0000\u0000"+
		"\u0000\u0424\u042c\u0001\u0000\u0000\u0000\u0425\u0426\u0005(\u0000\u0000"+
		"\u0426\u0427\u0003B!\u0000\u0427\u0428\u0006#\uffff\uffff\u0000\u0428"+
		"\u042b\u0001\u0000\u0000\u0000\u0429\u042b\u0003\n\u0005\u0000\u042a\u0425"+
		"\u0001\u0000\u0000\u0000\u042a\u0429\u0001\u0000\u0000\u0000\u042b\u042e"+
		"\u0001\u0000\u0000\u0000\u042c\u042a\u0001\u0000\u0000\u0000\u042c\u042d"+
		"\u0001\u0000\u0000\u0000\u042d\u0430\u0001\u0000\u0000\u0000\u042e\u042c"+
		"\u0001\u0000\u0000\u0000\u042f\u0420\u0001\u0000\u0000\u0000\u042f\u0430"+
		"\u0001\u0000\u0000\u0000\u0430\u0431\u0001\u0000\u0000\u0000\u0431\u0432"+
		"\u0005+\u0000\u0000\u0432\u043d\u0006#\uffff\uffff\u0000\u0433\u0434\n"+
		"\u0010\u0000\u0000\u0434\u0435\u0006#\uffff\uffff\u0000\u0435\u0436\u0005"+
		".\u0000\u0000\u0436\u0438\u0006#\uffff\uffff\u0000\u0437\u0439\u0003F"+
		"#\u0000\u0438\u0437\u0001\u0000\u0000\u0000\u0438\u0439\u0001\u0000\u0000"+
		"\u0000\u0439\u043a\u0001\u0000\u0000\u0000\u043a\u043b\u0005/\u0000\u0000"+
		"\u043b\u043d\u0006#\uffff\uffff\u0000\u043c\u03ad\u0001\u0000\u0000\u0000"+
		"\u043c\u03b4\u0001\u0000\u0000\u0000\u043c\u03bb\u0001\u0000\u0000\u0000"+
		"\u043c\u03c2\u0001\u0000\u0000\u0000\u043c\u03c9\u0001\u0000\u0000\u0000"+
		"\u043c\u03d0\u0001\u0000\u0000\u0000\u043c\u03d7\u0001\u0000\u0000\u0000"+
		"\u043c\u03de\u0001\u0000\u0000\u0000\u043c\u03e5\u0001\u0000\u0000\u0000"+
		"\u043c\u03ec\u0001\u0000\u0000\u0000\u043c\u03f3\u0001\u0000\u0000\u0000"+
		"\u043c\u03fa\u0001\u0000\u0000\u0000\u043c\u0401\u0001\u0000\u0000\u0000"+
		"\u043c\u0409\u0001\u0000\u0000\u0000\u043c\u0410\u0001\u0000\u0000\u0000"+
		"\u043c\u0417\u0001\u0000\u0000\u0000\u043c\u041c\u0001\u0000\u0000\u0000"+
		"\u043c\u0433\u0001\u0000\u0000\u0000\u043d\u0440\u0001\u0000\u0000\u0000"+
		"\u043e\u043c\u0001\u0000\u0000\u0000\u043e\u043f\u0001\u0000\u0000\u0000"+
		"\u043fG\u0001\u0000\u0000\u0000\u0440\u043e\u0001\u0000\u0000\u0000\u0441"+
		"\u0442\u0003J%\u0000\u0442\u0443\u0006$\uffff\uffff\u0000\u0443\u0460"+
		"\u0001\u0000\u0000\u0000\u0444\u0445\u0003L&\u0000\u0445\u0446\u0006$"+
		"\uffff\uffff\u0000\u0446\u0460\u0001\u0000\u0000\u0000\u0447\u0448\u0005"+
		"*\u0000\u0000\u0448\u0449\u0003F#\u0000\u0449\u044a\u0005+\u0000\u0000"+
		"\u044a\u044b\u0006$\uffff\uffff\u0000\u044b\u0460\u0001\u0000\u0000\u0000"+
		"\u044c\u044d\u0005O\u0000\u0000\u044d\u0460\u0006$\uffff\uffff\u0000\u044e"+
		"\u044f\u0005P\u0000\u0000\u044f\u0460\u0006$\uffff\uffff\u0000\u0450\u0451"+
		"\u0005S\u0000\u0000\u0451\u0460\u0006$\uffff\uffff\u0000\u0452\u0453\u0005"+
		"T\u0000\u0000\u0453\u0460\u0006$\uffff\uffff\u0000\u0454\u0455\u0005U"+
		"\u0000\u0000\u0455\u0460\u0006$\uffff\uffff\u0000\u0456\u0457\u0003\u0000"+
		"\u0000\u0000\u0457\u0458\u0006$\uffff\uffff\u0000\u0458\u0460\u0001\u0000"+
		"\u0000\u0000\u0459\u045a\u0005V\u0000\u0000\u045a\u0460\u0006$\uffff\uffff"+
		"\u0000\u045b\u045c\u0003\u0004\u0002\u0000\u045c\u045d\u0006$\uffff\uffff"+
		"\u0000\u045d\u0460\u0001\u0000\u0000\u0000\u045e\u0460\u0003\n\u0005\u0000"+
		"\u045f\u0441\u0001\u0000\u0000\u0000\u045f\u0444\u0001\u0000\u0000\u0000"+
		"\u045f\u0447\u0001\u0000\u0000\u0000\u045f\u044c\u0001\u0000\u0000\u0000"+
		"\u045f\u044e\u0001\u0000\u0000\u0000\u045f\u0450\u0001\u0000\u0000\u0000"+
		"\u045f\u0452\u0001\u0000\u0000\u0000\u045f\u0454\u0001\u0000\u0000\u0000"+
		"\u045f\u0456\u0001\u0000\u0000\u0000\u045f\u0459\u0001\u0000\u0000\u0000"+
		"\u045f\u045b\u0001\u0000\u0000\u0000\u045f\u045e\u0001\u0000\u0000\u0000"+
		"\u0460I\u0001\u0000\u0000\u0000\u0461\u0462\u0005V\u0000\u0000\u0462\u0463"+
		"\u0006%\uffff\uffff\u0000\u0463\u0464\u0005,\u0000\u0000\u0464\u0474\u0006"+
		"%\uffff\uffff\u0000\u0465\u0466\u0003B!\u0000\u0466\u0468\u0006%\uffff"+
		"\uffff\u0000\u0467\u0469\u0003\n\u0005\u0000\u0468\u0467\u0001\u0000\u0000"+
		"\u0000\u0468\u0469\u0001\u0000\u0000\u0000\u0469\u0471\u0001\u0000\u0000"+
		"\u0000\u046a\u046b\u0005(\u0000\u0000\u046b\u046c\u0003B!\u0000\u046c"+
		"\u046d\u0006%\uffff\uffff\u0000\u046d\u0470\u0001\u0000\u0000\u0000\u046e"+
		"\u0470\u0003\n\u0005\u0000\u046f\u046a\u0001\u0000\u0000\u0000\u046f\u046e"+
		"\u0001\u0000\u0000\u0000\u0470\u0473\u0001\u0000\u0000\u0000\u0471\u046f"+
		"\u0001\u0000\u0000\u0000\u0471\u0472\u0001\u0000\u0000\u0000\u0472\u0475"+
		"\u0001\u0000\u0000\u0000\u0473\u0471\u0001\u0000\u0000\u0000\u0474\u0465"+
		"\u0001\u0000\u0000\u0000\u0474\u0475\u0001\u0000\u0000\u0000\u0475\u0476"+
		"\u0001\u0000\u0000\u0000\u0476\u0477\u0005-\u0000\u0000\u0477\u0478\u0006"+
		"%\uffff\uffff\u0000\u0478K\u0001\u0000\u0000\u0000\u0479\u047a\u0005."+
		"\u0000\u0000\u047a\u048a\u0006&\uffff\uffff\u0000\u047b\u047c\u0003B!"+
		"\u0000\u047c\u047e\u0006&\uffff\uffff\u0000\u047d\u047f\u0003\n\u0005"+
		"\u0000\u047e\u047d\u0001\u0000\u0000\u0000\u047e\u047f\u0001\u0000\u0000"+
		"\u0000\u047f\u0487\u0001\u0000\u0000\u0000\u0480\u0481\u0005(\u0000\u0000"+
		"\u0481\u0482\u0003B!\u0000\u0482\u0483\u0006&\uffff\uffff\u0000\u0483"+
		"\u0486\u0001\u0000\u0000\u0000\u0484\u0486\u0003\n\u0005\u0000\u0485\u0480"+
		"\u0001\u0000\u0000\u0000\u0485\u0484\u0001\u0000\u0000\u0000\u0486\u0489"+
		"\u0001\u0000\u0000\u0000\u0487\u0485\u0001\u0000\u0000\u0000\u0487\u0488"+
		"\u0001\u0000\u0000\u0000\u0488\u048b\u0001\u0000\u0000\u0000\u0489\u0487"+
		"\u0001\u0000\u0000\u0000\u048a\u047b\u0001\u0000\u0000\u0000\u048a\u048b"+
		"\u0001\u0000\u0000\u0000\u048b\u048c\u0001\u0000\u0000\u0000\u048c\u048d"+
		"\u0005/\u0000\u0000\u048d\u048e\u0006&\uffff\uffff\u0000\u048eM\u0001"+
		"\u0000\u0000\u0000\u048f\u0490\u0005?\u0000\u0000\u0490\u0491\u0006\'"+
		"\uffff\uffff\u0000\u0491\u0492\u0005S\u0000\u0000\u0492\u0493\u0006\'"+
		"\uffff\uffff\u0000\u0493\u0495\u0001\u0000\u0000\u0000\u0494\u0496\u0003"+
		"\n\u0005\u0000\u0495\u0494\u0001\u0000\u0000\u0000\u0495\u0496\u0001\u0000"+
		"\u0000\u0000\u0496\u049a\u0001\u0000\u0000\u0000\u0497\u0498\u0005(\u0000"+
		"\u0000\u0498\u0499\u0005S\u0000\u0000\u0499\u049b\u0006\'\uffff\uffff"+
		"\u0000\u049a\u0497\u0001\u0000\u0000\u0000\u049b\u049c\u0001\u0000\u0000"+
		"\u0000\u049c\u049a\u0001\u0000\u0000\u0000\u049c\u049d\u0001\u0000\u0000"+
		"\u0000\u049d\u049e\u0001\u0000\u0000\u0000\u049e\u049f\u0005A\u0000\u0000"+
		"\u049f\u04a0\u0006\'\uffff\uffff\u0000\u04a0O\u0001\u0000\u0000\u0000"+
		"\u04a1\u04a2\u0005?\u0000\u0000\u04a2\u04a3\u0006(\uffff\uffff\u0000\u04a3"+
		"\u04a4\u0005T\u0000\u0000\u04a4\u04a5\u0006(\uffff\uffff\u0000\u04a5\u04a7"+
		"\u0001\u0000\u0000\u0000\u04a6\u04a8\u0003\n\u0005\u0000\u04a7\u04a6\u0001"+
		"\u0000\u0000\u0000\u04a7\u04a8\u0001\u0000\u0000\u0000\u04a8\u04ac\u0001"+
		"\u0000\u0000\u0000\u04a9\u04aa\u0005(\u0000\u0000\u04aa\u04ab\u0005T\u0000"+
		"\u0000\u04ab\u04ad\u0006(\uffff\uffff\u0000\u04ac\u04a9\u0001\u0000\u0000"+
		"\u0000\u04ad\u04ae\u0001\u0000\u0000\u0000\u04ae\u04ac\u0001\u0000\u0000"+
		"\u0000\u04ae\u04af\u0001\u0000\u0000\u0000\u04af\u04b0\u0001\u0000\u0000"+
		"\u0000\u04b0\u04b1\u0005A\u0000\u0000\u04b1\u04b2\u0006(\uffff\uffff\u0000"+
		"\u04b2Q\u0001\u0000\u0000\u0000\u04b3\u04b4\u0005?\u0000\u0000\u04b4\u04b5"+
		"\u0006)\uffff\uffff\u0000\u04b5\u04b6\u0005O\u0000\u0000\u04b6\u04b7\u0006"+
		")\uffff\uffff\u0000\u04b7\u04b9\u0001\u0000\u0000\u0000\u04b8\u04ba\u0003"+
		"\n\u0005\u0000\u04b9\u04b8\u0001\u0000\u0000\u0000\u04b9\u04ba\u0001\u0000"+
		"\u0000\u0000\u04ba\u04be\u0001\u0000\u0000\u0000\u04bb\u04bc\u0005(\u0000"+
		"\u0000\u04bc\u04bd\u0005O\u0000\u0000\u04bd\u04bf\u0006)\uffff\uffff\u0000"+
		"\u04be\u04bb\u0001\u0000\u0000\u0000\u04bf\u04c0\u0001\u0000\u0000\u0000"+
		"\u04c0\u04be\u0001\u0000\u0000\u0000\u04c0\u04c1\u0001\u0000\u0000\u0000"+
		"\u04c1\u04c2\u0001\u0000\u0000\u0000\u04c2\u04c3\u0005A\u0000\u0000\u04c3"+
		"\u04c4\u0006)\uffff\uffff\u0000\u04c4S\u0001\u0000\u0000\u0000\u04c5\u04c6"+
		"\u0005?\u0000\u0000\u04c6\u04c7\u0006*\uffff\uffff\u0000\u04c7\u04c8\u0005"+
		"P\u0000\u0000\u04c8\u04c9\u0006*\uffff\uffff\u0000\u04c9\u04cb\u0001\u0000"+
		"\u0000\u0000\u04ca\u04cc\u0003\n\u0005\u0000\u04cb\u04ca\u0001\u0000\u0000"+
		"\u0000\u04cb\u04cc\u0001\u0000\u0000\u0000\u04cc\u04d0\u0001\u0000\u0000"+
		"\u0000\u04cd\u04ce\u0005(\u0000\u0000\u04ce\u04cf\u0005P\u0000\u0000\u04cf"+
		"\u04d1\u0006*\uffff\uffff\u0000\u04d0\u04cd\u0001\u0000\u0000\u0000\u04d1"+
		"\u04d2\u0001\u0000\u0000\u0000\u04d2\u04d0\u0001\u0000\u0000\u0000\u04d2"+
		"\u04d3\u0001\u0000\u0000\u0000\u04d3\u04d4\u0001\u0000\u0000\u0000\u04d4"+
		"\u04d5\u0005A\u0000\u0000\u04d5\u04d6\u0006*\uffff\uffff\u0000\u04d6U"+
		"\u0001\u0000\u0000\u0000\u04d7\u04d8\u0005?\u0000\u0000\u04d8\u04d9\u0006"+
		"+\uffff\uffff\u0000\u04d9\u04da\u0003\u0004\u0002\u0000\u04da\u04db\u0006"+
		"+\uffff\uffff\u0000\u04db\u04dd\u0001\u0000\u0000\u0000\u04dc\u04de\u0003"+
		"\n\u0005\u0000\u04dd\u04dc\u0001\u0000\u0000\u0000\u04dd\u04de\u0001\u0000"+
		"\u0000\u0000\u04de\u04e3\u0001\u0000\u0000\u0000\u04df\u04e0\u0005(\u0000"+
		"\u0000\u04e0\u04e1\u0003\u0004\u0002\u0000\u04e1\u04e2\u0006+\uffff\uffff"+
		"\u0000\u04e2\u04e4\u0001\u0000\u0000\u0000\u04e3\u04df\u0001\u0000\u0000"+
		"\u0000\u04e4\u04e5\u0001\u0000\u0000\u0000\u04e5\u04e3\u0001\u0000\u0000"+
		"\u0000\u04e5\u04e6\u0001\u0000\u0000\u0000\u04e6\u04e7\u0001\u0000\u0000"+
		"\u0000\u04e7\u04e8\u0005A\u0000\u0000\u04e8\u04e9\u0006+\uffff\uffff\u0000"+
		"\u04e9W\u0001\u0000\u0000\u0000h]fjqz~\u0084\u00b2\u00b4\u00d6\u00e0\u00e6"+
		"\u00e8\u00f5\u00fd\u0105\u010d\u0112\u0118\u011a\u0121\u012b\u012d\u0130"+
		"\u0137\u013f\u0147\u015c\u015f\u0161\u0169\u016b\u0178\u0181\u0185\u018e"+
		"\u0192\u0194\u0198\u01a0\u01a8\u01b4\u01bd\u01c6\u01cd\u01cf\u01d2\u01e2"+
		"\u01f0\u01fb\u0203\u0212\u0224\u0246\u0257\u0261\u0267\u026d\u0275\u02a8"+
		"\u02b4\u02b6\u02c2\u02d2\u02e7\u02eb\u02f3\u02f5\u0307\u0378\u037f\u0381"+
		"\u0384\u038d\u0391\u0393\u039e\u03ab\u0423\u042a\u042c\u042f\u0438\u043c"+
		"\u043e\u045f\u0468\u046f\u0471\u0474\u047e\u0485\u0487\u048a\u0495\u049c"+
		"\u04a7\u04ae\u04b9\u04c0\u04cb\u04d2\u04dd\u04e5";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}