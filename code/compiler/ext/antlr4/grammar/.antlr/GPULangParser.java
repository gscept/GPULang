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
#include "ast/generate.h"
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
#include "ast/expressions/declaredexpression.h"
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
		T__38=39, SC=40, CO=41, COL=42, LP=43, RP=44, LB=45, RB=46, LL=47, RR=48, 
		DOT=49, NOT=50, EQ=51, QO=52, QU=53, AND=54, ANDSET=55, OR=56, ORSET=57, 
		XOR=58, XORSET=59, CONJUGATE=60, Q=61, NU=62, FORWARDSLASH=63, LESS=64, 
		LESSEQ=65, GREATER=66, GREATEREQ=67, LOGICEQ=68, NOTEQ=69, LOGICAND=70, 
		LOGICOR=71, MOD=72, UNDERSC=73, SOBAKA=74, ADD_OP=75, SUB_OP=76, DIV_OP=77, 
		MUL_OP=78, ARROW=79, INTEGERLITERAL=80, UINTEGERLITERAL=81, COMMENT=82, 
		ML_COMMENT=83, FLOATLITERAL=84, DOUBLELITERAL=85, HEX=86, IDENTIFIER=87, 
		WS=88;
	public static final int
		RULE_string = 0, RULE_path = 1, RULE_boolean = 2, RULE_entry = 3, RULE_effect = 4, 
		RULE_linePreprocessorEntry = 5, RULE_alias = 6, RULE_annotation = 7, RULE_attribute = 8, 
		RULE_typeDeclaration = 9, RULE_generate = 10, RULE_gen_statement = 11, 
		RULE_gen_scope_statement = 12, RULE_gen_if_statement = 13, RULE_variables = 14, 
		RULE_structureDeclaration = 15, RULE_structure = 16, RULE_enumeration = 17, 
		RULE_parameter = 18, RULE_functionDeclaration = 19, RULE_function = 20, 
		RULE_program = 21, RULE_sampler = 22, RULE_state = 23, RULE_statement = 24, 
		RULE_expressionStatement = 25, RULE_ifStatement = 26, RULE_forStatement = 27, 
		RULE_forRangeStatement = 28, RULE_forUniformValueStatement = 29, RULE_whileStatement = 30, 
		RULE_scopeStatement = 31, RULE_terminateStatement = 32, RULE_continueStatement = 33, 
		RULE_switchStatement = 34, RULE_breakStatement = 35, RULE_expressionList = 36, 
		RULE_expression = 37, RULE_binaryexpatom = 38, RULE_initializerExpression = 39, 
		RULE_arrayInitializerExpression = 40, RULE_floatVecLiteralExpression = 41, 
		RULE_doubleVecLiteralExpression = 42, RULE_intVecLiteralExpression = 43, 
		RULE_uintVecLiteralExpression = 44, RULE_booleanVecLiteralExpression = 45;
	private static String[] makeRuleNames() {
		return new String[] {
			"string", "path", "boolean", "entry", "effect", "linePreprocessorEntry", 
			"alias", "annotation", "attribute", "typeDeclaration", "generate", "gen_statement", 
			"gen_scope_statement", "gen_if_statement", "variables", "structureDeclaration", 
			"structure", "enumeration", "parameter", "functionDeclaration", "function", 
			"program", "sampler", "state", "statement", "expressionStatement", "ifStatement", 
			"forStatement", "forRangeStatement", "forUniformValueStatement", "whileStatement", 
			"scopeStatement", "terminateStatement", "continueStatement", "switchStatement", 
			"breakStatement", "expressionList", "expression", "binaryexpatom", "initializerExpression", 
			"arrayInitializerExpression", "floatVecLiteralExpression", "doubleVecLiteralExpression", 
			"intVecLiteralExpression", "uintVecLiteralExpression", "booleanVecLiteralExpression"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, "'true'", "'false'", "'#line'", "'alias'", "'as'", "'generate'", 
			"'if'", "'else'", "'struct'", "'enum'", "'program'", "'sampler_state'", 
			"'render_state'", "'for'", "'..'", "'for_uniform'", "'while'", "'do'", 
			"'return'", "'discard'", "'ray_ignore'", "'ray_terminate'", "'continue'", 
			"'switch'", "'case'", "'default'", "'break'", "'++'", "'--'", "'<<'", 
			"'>>'", "'+='", "'-='", "'*='", "'/='", "'%='", "'<<='", "'>>='", "'declared'", 
			"';'", "','", "':'", "'('", "')'", "'{'", "'}'", "'['", "']'", "'.'", 
			"'!'", "'='", "'\"'", "'?'", "'&'", "'&='", "'|'", "'|='", "'^'", "'^='", 
			"'~'", "'''", "'#'", "'\\'", "'<'", "'<='", "'>'", "'>='", "'=='", "'!='", 
			"'&&'", "'||'", "'%'", "'_'", "'@'", "'+'", "'-'", "'/'", "'*'", "'->'"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, "SC", "CO", "COL", "LP", "RP", "LB", "RB", "LL", 
			"RR", "DOT", "NOT", "EQ", "QO", "QU", "AND", "ANDSET", "OR", "ORSET", 
			"XOR", "XORSET", "CONJUGATE", "Q", "NU", "FORWARDSLASH", "LESS", "LESSEQ", 
			"GREATER", "GREATEREQ", "LOGICEQ", "NOTEQ", "LOGICAND", "LOGICOR", "MOD", 
			"UNDERSC", "SOBAKA", "ADD_OP", "SUB_OP", "DIV_OP", "MUL_OP", "ARROW", 
			"INTEGERLITERAL", "UINTEGERLITERAL", "COMMENT", "ML_COMMENT", "FLOATLITERAL", 
			"DOUBLELITERAL", "HEX", "IDENTIFIER", "WS"
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
			setState(110);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case QO:
				enterOuterAlt(_localctx, 1);
				{
				setState(92);
				match(QO);
				setState(97);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while ((((_la) & ~0x3f) == 0 && ((1L << _la) & -4503599627370498L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & 33554431L) != 0)) {
					{
					{
					setState(93);
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
					setState(99);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				setState(100);
				match(QO);
				}
				break;
			case Q:
				enterOuterAlt(_localctx, 2);
				{
				setState(101);
				match(Q);
				setState(106);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while ((((_la) & ~0x3f) == 0 && ((1L << _la) & -2305843009213693954L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & 33554431L) != 0)) {
					{
					{
					setState(102);
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
					setState(108);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				setState(109);
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
			setState(130);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case QO:
				enterOuterAlt(_localctx, 1);
				{
				setState(112);
				match(QO);
				setState(117);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while ((((_la) & ~0x3f) == 0 && ((1L << _la) & -4503599627370498L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & 33554431L) != 0)) {
					{
					{
					setState(113);
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
					setState(119);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				setState(120);
				match(QO);
				}
				break;
			case LESS:
				enterOuterAlt(_localctx, 2);
				{
				setState(121);
				match(LESS);
				setState(126);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while ((((_la) & ~0x3f) == 0 && ((1L << _la) & -2L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & 33554427L) != 0)) {
					{
					{
					setState(122);
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
					setState(128);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				setState(129);
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
			setState(136);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__0:
				enterOuterAlt(_localctx, 1);
				{
				setState(132);
				match(T__0);
				 ((BooleanContext)_localctx).val =  true; 
				}
				break;
			case T__1:
				enterOuterAlt(_localctx, 2);
				{
				setState(134);
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
			setState(138);
			((EntryContext)_localctx).effect = effect();

			        ((EntryContext)_localctx).returnEffect =  ((EntryContext)_localctx).effect.eff;
			    
			setState(140);
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
			setState(184);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,8,_ctx);
			while ( _alt!=1 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1+1 ) {
					{
					setState(182);
					_errHandler.sync(this);
					switch ( getInterpreter().adaptivePredict(_input,7,_ctx) ) {
					case 1:
						{
						setState(142);
						linePreprocessorEntry();
						}
						break;
					case 2:
						{
						setState(143);
						((EffectContext)_localctx).generate = generate();
						setState(144);
						match(SC);
						 _localctx.eff->symbols.Append(((EffectContext)_localctx).generate.sym); 
						}
						break;
					case 3:
						{
						setState(147);
						((EffectContext)_localctx).alias = alias();
						setState(148);
						match(SC);
						 _localctx.eff->symbols.Append(((EffectContext)_localctx).alias.sym); 
						}
						break;
					case 4:
						{
						setState(151);
						((EffectContext)_localctx).functionDeclaration = functionDeclaration();
						setState(152);
						match(SC);
						 _localctx.eff->symbols.Append(((EffectContext)_localctx).functionDeclaration.sym); 
						}
						break;
					case 5:
						{
						setState(155);
						((EffectContext)_localctx).function = function();
						 _localctx.eff->symbols.Append(((EffectContext)_localctx).function.sym); 
						}
						break;
					case 6:
						{
						setState(158);
						((EffectContext)_localctx).variables = variables();
						setState(159);
						match(SC);
						 for (Variable* var : ((EffectContext)_localctx).variables.vars) { _localctx.eff->symbols.Append(var); } 
						}
						break;
					case 7:
						{
						setState(162);
						((EffectContext)_localctx).structure = structure();
						setState(163);
						match(SC);
						 _localctx.eff->symbols.Append(((EffectContext)_localctx).structure.sym); 
						}
						break;
					case 8:
						{
						setState(166);
						((EffectContext)_localctx).enumeration = enumeration();
						setState(167);
						match(SC);
						 _localctx.eff->symbols.Append(((EffectContext)_localctx).enumeration.sym); 
						}
						break;
					case 9:
						{
						setState(170);
						((EffectContext)_localctx).state = state();
						setState(171);
						match(SC);
						 _localctx.eff->symbols.Append(((EffectContext)_localctx).state.sym); 
						}
						break;
					case 10:
						{
						setState(174);
						((EffectContext)_localctx).sampler = sampler();
						setState(175);
						match(SC);
						 _localctx.eff->symbols.Append(((EffectContext)_localctx).sampler.sym); 
						}
						break;
					case 11:
						{
						setState(178);
						((EffectContext)_localctx).program = program();
						setState(179);
						match(SC);
						 _localctx.eff->symbols.Append(((EffectContext)_localctx).program.sym); 
						}
						break;
					}
					} 
				}
				setState(186);
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
			setState(187);
			match(T__2);
			 origLine = _input->LT(-1)->getLine(); 
			setState(189);
			((LinePreprocessorEntryContext)_localctx).line = match(INTEGERLITERAL);
			setState(190);
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
			Symbol::Location nameLocation, typeLocation;
		        FixedString name, type;
		    
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(193);
			match(T__3);
			setState(194);
			((AliasContext)_localctx).name = match(IDENTIFIER);
			 nameLocation = SetupFile(); 
			setState(196);
			match(T__4);
			setState(197);
			((AliasContext)_localctx).type = match(IDENTIFIER);
			 typeLocation = SetupFile(); name = FixedString((((AliasContext)_localctx).name!=null?((AliasContext)_localctx).name.getText():null)); type = FixedString((((AliasContext)_localctx).type!=null?((AliasContext)_localctx).type.getText():null)); 

			        ((AliasContext)_localctx).sym =  Alloc<Alias>();
				_localctx.sym->nameLocation = nameLocation;
				_localctx.sym->typeLocation = typeLocation;
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
			setState(201);
			match(SOBAKA);
			 ((AnnotationContext)_localctx).annot =  Alloc<Annotation>(); 
			{
			setState(203);
			((AnnotationContext)_localctx).name = match(IDENTIFIER);
			 _localctx.annot->location = SetupFile(); 
			setState(205);
			match(LP);
			setState(206);
			((AnnotationContext)_localctx).value = expression(0);
			setState(207);
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
			setState(219);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,9,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(210);
				((AttributeContext)_localctx).name = match(IDENTIFIER);
				 ((AttributeContext)_localctx).attr =  Alloc<Attribute>(); _localctx.attr->location = SetupFile(); _localctx.attr->name = (((AttributeContext)_localctx).name!=null?((AttributeContext)_localctx).name.getText():null); 
				setState(212);
				match(LP);
				setState(213);
				((AttributeContext)_localctx).e = ((AttributeContext)_localctx).expression = expression(0);
				setState(214);
				match(RP);
				 _localctx.attr->expression = ((AttributeContext)_localctx).expression.tree; 
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(217);
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
			setState(237);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,12,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					{
					setState(235);
					_errHandler.sync(this);
					switch (_input.LA(1)) {
					case MUL_OP:
						{
						setState(222);
						match(MUL_OP);
						 _localctx.type.type.AddModifier(Type::FullType::Modifier::Pointer); 
						}
						break;
					case LL:
						{
						setState(224);
						match(LL);
						 _localctx.type.type.AddModifier(Type::FullType::Modifier::Array); 
						setState(229);
						_errHandler.sync(this);
						_la = _input.LA(1);
						if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 3482558496006733838L) != 0) || ((((_la - 75)) & ~0x3f) == 0 && ((1L << (_la - 75)) & 7787L) != 0)) {
							{
							setState(226);
							((TypeDeclarationContext)_localctx).arraySize0 = expression(0);
							 _localctx.type.type.UpdateValue(((TypeDeclarationContext)_localctx).arraySize0.tree); 
							}
						}

						setState(231);
						match(RR);
						}
						break;
					case IDENTIFIER:
						{
						setState(232);
						((TypeDeclarationContext)_localctx).qual = match(IDENTIFIER);
						 _localctx.type.type.AddQualifier(FixedString((((TypeDeclarationContext)_localctx).qual!=null?((TypeDeclarationContext)_localctx).qual.getText():null))); 
						}
						break;
					case T__2:
						{
						setState(234);
						linePreprocessorEntry();
						}
						break;
					default:
						throw new NoViableAltException(this);
					}
					} 
				}
				setState(239);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,12,_ctx);
			}
			setState(240);
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
		public VariablesContext variables;
		public Gen_statementContext gen_statement;
		public AliasContext alias;
		public FunctionDeclarationContext functionDeclaration;
		public FunctionContext function;
		public TerminalNode LESS() { return getToken(GPULangParser.LESS, 0); }
		public TerminalNode GREATER() { return getToken(GPULangParser.GREATER, 0); }
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
		public List<Gen_statementContext> gen_statement() {
			return getRuleContexts(Gen_statementContext.class);
		}
		public Gen_statementContext gen_statement(int i) {
			return getRuleContext(Gen_statementContext.class,i);
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
		public GenerateContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_generate; }
	}

	public final GenerateContext generate() throws RecognitionException {
		GenerateContext _localctx = new GenerateContext(_ctx, getState());
		enterRule(_localctx, 20, RULE_generate);

		        PinnedArray<Symbol*> symbols = 0xFFFFF;
			    Symbol::Location location;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(243);
			match(T__5);
			 location = SetupFile(); 
			setState(245);
			match(LESS);
			setState(266);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & 152L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & 8389633L) != 0)) {
				{
				setState(264);
				_errHandler.sync(this);
				switch ( getInterpreter().adaptivePredict(_input,13,_ctx) ) {
				case 1:
					{
					setState(246);
					((GenerateContext)_localctx).variables = variables();
					setState(247);
					match(SC);
					 for(Variable* var : ((GenerateContext)_localctx).variables.vars) { symbols.Append(var); } 
					}
					break;
				case 2:
					{
					setState(250);
					((GenerateContext)_localctx).gen_statement = gen_statement();
					 symbols.Append(((GenerateContext)_localctx).gen_statement.tree); 
					}
					break;
				case 3:
					{
					setState(253);
					((GenerateContext)_localctx).alias = alias();
					setState(254);
					match(SC);
					 symbols.Append(((GenerateContext)_localctx).alias.sym); 
					}
					break;
				case 4:
					{
					setState(257);
					((GenerateContext)_localctx).functionDeclaration = functionDeclaration();
					setState(258);
					match(SC);
					 symbols.Append(((GenerateContext)_localctx).functionDeclaration.sym); 
					}
					break;
				case 5:
					{
					setState(261);
					((GenerateContext)_localctx).function = function();
					 symbols.Append(((GenerateContext)_localctx).function.sym); 
					}
					break;
				}
				}
				setState(268);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(269);
			match(GREATER);

				((GenerateContext)_localctx).sym =  Alloc<Generate>(symbols);
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
	public static class Gen_statementContext extends ParserRuleContext {
		public Statement* tree;
		public Gen_if_statementContext gen_if_statement;
		public Gen_scope_statementContext gen_scope_statement;
		public Gen_if_statementContext gen_if_statement() {
			return getRuleContext(Gen_if_statementContext.class,0);
		}
		public Gen_scope_statementContext gen_scope_statement() {
			return getRuleContext(Gen_scope_statementContext.class,0);
		}
		public Gen_statementContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_gen_statement; }
	}

	public final Gen_statementContext gen_statement() throws RecognitionException {
		Gen_statementContext _localctx = new Gen_statementContext(_ctx, getState());
		enterRule(_localctx, 22, RULE_gen_statement);

		        ((Gen_statementContext)_localctx).tree =  nullptr;
		    
		try {
			setState(278);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__6:
				enterOuterAlt(_localctx, 1);
				{
				setState(272);
				((Gen_statementContext)_localctx).gen_if_statement = gen_if_statement();
				 ((Gen_statementContext)_localctx).tree =  ((Gen_statementContext)_localctx).gen_if_statement.tree; 
				}
				break;
			case LESS:
				enterOuterAlt(_localctx, 2);
				{
				setState(275);
				((Gen_statementContext)_localctx).gen_scope_statement = gen_scope_statement();
				 ((Gen_statementContext)_localctx).tree =  ((Gen_statementContext)_localctx).gen_scope_statement.tree; 
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
	public static class Gen_scope_statementContext extends ParserRuleContext {
		public ScopeStatement* tree;
		public VariablesContext variables;
		public Gen_statementContext gen_statement;
		public AliasContext alias;
		public FunctionContext function;
		public TerminalNode LESS() { return getToken(GPULangParser.LESS, 0); }
		public TerminalNode GREATER() { return getToken(GPULangParser.GREATER, 0); }
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
		public List<Gen_statementContext> gen_statement() {
			return getRuleContexts(Gen_statementContext.class);
		}
		public Gen_statementContext gen_statement(int i) {
			return getRuleContext(Gen_statementContext.class,i);
		}
		public List<AliasContext> alias() {
			return getRuleContexts(AliasContext.class);
		}
		public AliasContext alias(int i) {
			return getRuleContext(AliasContext.class,i);
		}
		public List<FunctionContext> function() {
			return getRuleContexts(FunctionContext.class);
		}
		public FunctionContext function(int i) {
			return getRuleContext(FunctionContext.class,i);
		}
		public List<LinePreprocessorEntryContext> linePreprocessorEntry() {
			return getRuleContexts(LinePreprocessorEntryContext.class);
		}
		public LinePreprocessorEntryContext linePreprocessorEntry(int i) {
			return getRuleContext(LinePreprocessorEntryContext.class,i);
		}
		public Gen_scope_statementContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_gen_scope_statement; }
	}

	public final Gen_scope_statementContext gen_scope_statement() throws RecognitionException {
		Gen_scope_statementContext _localctx = new Gen_scope_statementContext(_ctx, getState());
		enterRule(_localctx, 24, RULE_gen_scope_statement);

		        ((Gen_scope_statementContext)_localctx).tree =  nullptr;
		        PinnedArray<Symbol*> contents(0xFFFFFF);
			    std::vector<Expression*> unfinished;
		        Symbol::Location location;
		        Symbol::Location ends;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(280);
			match(LESS);
			 location = SetupFile(); 
			setState(299);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & 152L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & 8389633L) != 0)) {
				{
				setState(297);
				_errHandler.sync(this);
				switch ( getInterpreter().adaptivePredict(_input,16,_ctx) ) {
				case 1:
					{
					setState(282);
					((Gen_scope_statementContext)_localctx).variables = variables();
					setState(283);
					match(SC);
					 for(Variable* var : ((Gen_scope_statementContext)_localctx).variables.vars) { contents.Append(var); } 
					}
					break;
				case 2:
					{
					setState(286);
					((Gen_scope_statementContext)_localctx).gen_statement = gen_statement();
					 contents.Append(((Gen_scope_statementContext)_localctx).gen_statement.tree); 
					}
					break;
				case 3:
					{
					setState(289);
					((Gen_scope_statementContext)_localctx).alias = alias();
					setState(290);
					match(SC);
					 contents.Append(((Gen_scope_statementContext)_localctx).alias.sym); 
					}
					break;
				case 4:
					{
					setState(293);
					((Gen_scope_statementContext)_localctx).function = function();
					 contents.Append(((Gen_scope_statementContext)_localctx).function.sym); 
					}
					break;
				case 5:
					{
					setState(296);
					linePreprocessorEntry();
					}
					break;
				}
				}
				setState(301);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(302);
			match(GREATER);
			 ends = SetupFile(); 

			        ((Gen_scope_statementContext)_localctx).tree =  Alloc<ScopeStatement>(std::move(contents), unfinished);
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
	public static class Gen_if_statementContext extends ParserRuleContext {
		public Statement* tree;
		public ExpressionContext condition;
		public Gen_statementContext ifBody;
		public Gen_statementContext elseBody;
		public TerminalNode LP() { return getToken(GPULangParser.LP, 0); }
		public TerminalNode RP() { return getToken(GPULangParser.RP, 0); }
		public ExpressionContext expression() {
			return getRuleContext(ExpressionContext.class,0);
		}
		public List<Gen_statementContext> gen_statement() {
			return getRuleContexts(Gen_statementContext.class);
		}
		public Gen_statementContext gen_statement(int i) {
			return getRuleContext(Gen_statementContext.class,i);
		}
		public Gen_if_statementContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_gen_if_statement; }
	}

	public final Gen_if_statementContext gen_if_statement() throws RecognitionException {
		Gen_if_statementContext _localctx = new Gen_if_statementContext(_ctx, getState());
		enterRule(_localctx, 26, RULE_gen_if_statement);

		        ((Gen_if_statementContext)_localctx).tree =  nullptr;
		        Expression* condition = nullptr;
		        Statement* ifBody = nullptr;
		        Statement* elseBody = nullptr;
		        Symbol::Location location;
		    
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(306);
			match(T__6);
			 location = SetupFile(); 
			setState(308);
			match(LP);
			setState(309);
			((Gen_if_statementContext)_localctx).condition = expression(0);
			 condition = ((Gen_if_statementContext)_localctx).condition.tree; 
			setState(311);
			match(RP);
			setState(312);
			((Gen_if_statementContext)_localctx).ifBody = gen_statement();
			 ifBody = ((Gen_if_statementContext)_localctx).ifBody.tree; 
			setState(318);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,18,_ctx) ) {
			case 1:
				{
				setState(314);
				match(T__7);
				setState(315);
				((Gen_if_statementContext)_localctx).elseBody = gen_statement();
				 elseBody = ((Gen_if_statementContext)_localctx).elseBody.tree; 
				}
				break;
			}

			        ((Gen_if_statementContext)_localctx).tree =  Alloc<IfStatement>(condition, ifBody, elseBody);
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
	public static class VariablesContext extends ParserRuleContext {
		public FixedArray<Variable*> vars;
		public AnnotationContext annotation;
		public AttributeContext attribute;
		public Token varName;
		public Token varNameN;
		public TypeDeclarationContext typeDeclaration;
		public ExpressionContext valueExpr;
		public ExpressionContext valueExprN;
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
		public List<ExpressionContext> expression() {
			return getRuleContexts(ExpressionContext.class);
		}
		public ExpressionContext expression(int i) {
			return getRuleContext(ExpressionContext.class,i);
		}
		public VariablesContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_variables; }
	}

	public final VariablesContext variables() throws RecognitionException {
		VariablesContext _localctx = new VariablesContext(_ctx, getState());
		enterRule(_localctx, 28, RULE_variables);

		        TransientArray<Variable*> list(256);
		        TransientArray<Annotation*> annotations(32);
		        TransientArray<Attribute*> attributes(32);
		        TransientArray<FixedString> names(256);
		        TransientArray<Expression*> valueExpressions(256);
		        TransientArray<Symbol::Location> locations(256);
		        unsigned initCounter = 0;
		        TypeDeclaration type = TypeDeclaration{ .type = Type::FullType{UNDEFINED_TYPE} };
		    
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(325);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__2) {
				{
				{
				setState(322);
				linePreprocessorEntry();
				}
				}
				setState(327);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(333);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==SOBAKA) {
				{
				{
				setState(328);
				((VariablesContext)_localctx).annotation = annotation();
				 if (annotations.Full()) { throw IndexOutOfBoundsException("Maximum of 32 annotations reached"); } annotations.Append(std::move(((VariablesContext)_localctx).annotation.annot)); 
				}
				}
				setState(335);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(339); 
			_errHandler.sync(this);
			_alt = 1;
			do {
				switch (_alt) {
				case 1:
					{
					{
					setState(336);
					((VariablesContext)_localctx).attribute = attribute();
					 if (attributes.Full()) { throw IndexOutOfBoundsException("Maximum of 32 attributes reached"); } attributes.Append(std::move(((VariablesContext)_localctx).attribute.attr)); 
					}
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				setState(341); 
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,21,_ctx);
			} while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER );
			setState(343);
			((VariablesContext)_localctx).varName = match(IDENTIFIER);
			 names.Append(FixedString((((VariablesContext)_localctx).varName!=null?((VariablesContext)_localctx).varName.getText():null))); valueExpressions.Append(nullptr); locations.Append(SetupFile()); 
			setState(346);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,22,_ctx) ) {
			case 1:
				{
				setState(345);
				linePreprocessorEntry();
				}
				break;
			}
			setState(354);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__2 || _la==CO) {
				{
				setState(352);
				_errHandler.sync(this);
				switch (_input.LA(1)) {
				case CO:
					{
					setState(348);
					match(CO);
					setState(349);
					((VariablesContext)_localctx).varNameN = match(IDENTIFIER);
					 if (names.Full()) { throw IndexOutOfBoundsException("Maximum of 256 variable declarations reached"); } names.Append(FixedString((((VariablesContext)_localctx).varNameN!=null?((VariablesContext)_localctx).varNameN.getText():null))); valueExpressions.Append(nullptr); locations.Append(SetupFile()); 
					}
					break;
				case T__2:
					{
					setState(351);
					linePreprocessorEntry();
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				}
				setState(356);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(361);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COL) {
				{
				setState(357);
				match(COL);
				setState(358);
				((VariablesContext)_localctx).typeDeclaration = typeDeclaration();
				 type = ((VariablesContext)_localctx).typeDeclaration.type; 
				}
			}

			setState(376);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==EQ) {
				{
				setState(363);
				match(EQ);
				setState(364);
				((VariablesContext)_localctx).valueExpr = expression(0);
				 if (initCounter < names.size) { valueExpressions[initCounter++] = ((VariablesContext)_localctx).valueExpr.tree; }  
				setState(373);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while (_la==T__2 || _la==CO) {
					{
					setState(371);
					_errHandler.sync(this);
					switch (_input.LA(1)) {
					case CO:
						{
						setState(366);
						match(CO);
						setState(367);
						((VariablesContext)_localctx).valueExprN = expression(0);
						 if (initCounter < names.size) { valueExpressions[initCounter++] = ((VariablesContext)_localctx).valueExprN.tree; }; 
						}
						break;
					case T__2:
						{
						setState(370);
						linePreprocessorEntry();
						}
						break;
					default:
						throw new NoViableAltException(this);
					}
					}
					setState(375);
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
		enterRule(_localctx, 30, RULE_structureDeclaration);

		        ((StructureDeclarationContext)_localctx).sym =  nullptr;
		        TransientArray<Annotation*> annotations(32);
		        TransientArray<Attribute*> attributes(32);
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(383);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__2) {
				{
				{
				setState(380);
				linePreprocessorEntry();
				}
				}
				setState(385);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(391);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==SOBAKA) {
				{
				{
				setState(386);
				((StructureDeclarationContext)_localctx).annotation = annotation();
				 if (annotations.Full()) { throw IndexOutOfBoundsException("Maximum of 32 annotations reached"); } annotations.Append(std::move(((StructureDeclarationContext)_localctx).annotation.annot)); 
				}
				}
				setState(393);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(399);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==IDENTIFIER) {
				{
				{
				setState(394);
				((StructureDeclarationContext)_localctx).attribute = attribute();
				 if (attributes.Full()) { throw IndexOutOfBoundsException("Maximum of 32 attributes reached"); } attributes.Append(std::move(((StructureDeclarationContext)_localctx).attribute.attr)); 
				}
				}
				setState(401);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(402);
			match(T__8);
			setState(403);
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
		enterRule(_localctx, 32, RULE_structure);

		        ((StructureContext)_localctx).sym =  nullptr;
		        TransientArray<Symbol*> members(1024);
		        bool isArray = false;
		        Expression* arraySizeExpression = nullptr;
		        FixedString instanceName;
		        Symbol::Location varLocation;
		        Type::FullType varType;
		        Symbol::Location varTypeLocation;
		        Symbol::Location typeRange;
		        FixedString varName;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(406);
			((StructureContext)_localctx).structureDeclaration = structureDeclaration();
			 ((StructureContext)_localctx).sym =  ((StructureContext)_localctx).structureDeclaration.sym; 
			setState(408);
			match(LB);
			setState(435);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__2 || _la==IDENTIFIER) {
				{
				setState(433);
				_errHandler.sync(this);
				switch (_input.LA(1)) {
				case IDENTIFIER:
					{
					setState(409);
					((StructureContext)_localctx).varName = match(IDENTIFIER);
					 varName = FixedString((((StructureContext)_localctx).varName!=null?((StructureContext)_localctx).varName.getText():null)); varLocation = SetupFile(); 
					setState(411);
					match(COL);
					 typeRange = BeginLocationRange(); 
					setState(425);
					_errHandler.sync(this);
					_la = _input.LA(1);
					while (_la==LL || _la==MUL_OP) {
						{
						setState(423);
						_errHandler.sync(this);
						switch (_input.LA(1)) {
						case MUL_OP:
							{
							setState(413);
							match(MUL_OP);
							 varType.AddModifier(Type::FullType::Modifier::Pointer); 
							}
							break;
						case LL:
							{
							setState(415);
							match(LL);
							 varType.AddModifier(Type::FullType::Modifier::Array); 
							setState(420);
							_errHandler.sync(this);
							_la = _input.LA(1);
							if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 3482558496006733838L) != 0) || ((((_la - 75)) & ~0x3f) == 0 && ((1L << (_la - 75)) & 7787L) != 0)) {
								{
								setState(417);
								((StructureContext)_localctx).arraySize0 = expression(0);
								 varType.UpdateValue(((StructureContext)_localctx).arraySize0.tree); 
								}
							}

							setState(422);
							match(RR);
							}
							break;
						default:
							throw new NoViableAltException(this);
						}
						}
						setState(427);
						_errHandler.sync(this);
						_la = _input.LA(1);
					}
					setState(428);
					((StructureContext)_localctx).varTypeName = match(IDENTIFIER);
					 if (members.Full()) { throw IndexOutOfBoundsException("Maximum of 1024 struct members reached"); } varType.name = (((StructureContext)_localctx).varTypeName!=null?((StructureContext)_localctx).varTypeName.getText():null); varTypeLocation = EndLocationRange(typeRange); 
					setState(430);
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
					setState(432);
					linePreprocessorEntry();
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				}
				setState(437);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(438);
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
		enterRule(_localctx, 34, RULE_enumeration);

		        ((EnumerationContext)_localctx).sym =  nullptr;
		        TransientArray<FixedString> enumLabels(256);
		        TransientArray<Expression*> enumValues(256);
		        TransientArray<Symbol::Location> enumLocations(256);
		        FixedString name;
		        TypeDeclaration type = TypeDeclaration{ .type = Type::FullType{ConstantString("u32")} };
		        Symbol::Location location;
		        Symbol::Location labelLocation;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(441);
			match(T__9);
			setState(442);
			((EnumerationContext)_localctx).name = match(IDENTIFIER);
			 name = FixedString((((EnumerationContext)_localctx).name!=null?((EnumerationContext)_localctx).name.getText():null)); location = SetupFile(); 
			setState(448);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COL) {
				{
				setState(444);
				match(COL);
				setState(445);
				((EnumerationContext)_localctx).typeDeclaration = typeDeclaration();
				 type = ((EnumerationContext)_localctx).typeDeclaration.type; 
				}
			}

			setState(450);
			match(LB);
			setState(480);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case IDENTIFIER:
				{
				setState(451);
				((EnumerationContext)_localctx).label = match(IDENTIFIER);
				 Expression* expr = nullptr; labelLocation = SetupFile(); 
				setState(457);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==EQ) {
					{
					setState(453);
					match(EQ);
					setState(454);
					((EnumerationContext)_localctx).value = expression(0);
					 expr = ((EnumerationContext)_localctx).value.tree; 
					}
				}


				                enumLabels.Append(FixedString((((EnumerationContext)_localctx).label!=null?((EnumerationContext)_localctx).label.getText():null)));
				                enumValues.Append(expr);
				                enumLocations.Append(labelLocation);
				            
				setState(461);
				_errHandler.sync(this);
				switch ( getInterpreter().adaptivePredict(_input,39,_ctx) ) {
				case 1:
					{
					setState(460);
					linePreprocessorEntry();
					}
					break;
				}
				setState(476);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while (_la==T__2 || _la==CO) {
					{
					setState(474);
					_errHandler.sync(this);
					switch (_input.LA(1)) {
					case CO:
						{
						setState(463);
						match(CO);
						setState(464);
						((EnumerationContext)_localctx).label = match(IDENTIFIER);
						 if (enumLabels.Full()) { throw IndexOutOfBoundsException("Maximum of 256 enum labels"); } Expression* expr = nullptr; labelLocation = SetupFile(); 
						setState(470);
						_errHandler.sync(this);
						_la = _input.LA(1);
						if (_la==EQ) {
							{
							setState(466);
							match(EQ);
							setState(467);
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
						setState(473);
						linePreprocessorEntry();
						}
						break;
					default:
						throw new NoViableAltException(this);
					}
					}
					setState(478);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				}
				break;
			case T__2:
				{
				setState(479);
				linePreprocessorEntry();
				}
				break;
			case RB:
				break;
			default:
				break;
			}
			setState(482);
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
		public ExpressionContext valueExpr;
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
		public ExpressionContext expression() {
			return getRuleContext(ExpressionContext.class,0);
		}
		public ParameterContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_parameter; }
	}

	public final ParameterContext parameter() throws RecognitionException {
		ParameterContext _localctx = new ParameterContext(_ctx, getState());
		enterRule(_localctx, 36, RULE_parameter);

		        TransientArray<Attribute*> attributes(32);
		        FixedString name;
		        Expression* valueExpression = nullptr;
		        Symbol::Location location;
		        TypeDeclaration type = TypeDeclaration{ .type = Type::FullType{UNDEFINED_TYPE} };
		    
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(488);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__2) {
				{
				{
				setState(485);
				linePreprocessorEntry();
				}
				}
				setState(490);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(496);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,45,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					{
					{
					setState(491);
					((ParameterContext)_localctx).attribute = attribute();
					 if (attributes.Full()) { throw IndexOutOfBoundsException("Maximum of 32 attributes reached"); } attributes.Append(std::move(((ParameterContext)_localctx).attribute.attr)); 
					}
					} 
				}
				setState(498);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,45,_ctx);
			}
			setState(499);
			((ParameterContext)_localctx).varName = match(IDENTIFIER);
			 name = FixedString((((ParameterContext)_localctx).varName!=null?((ParameterContext)_localctx).varName.getText():null)); location = SetupFile(); 
			setState(501);
			match(COL);
			setState(502);
			((ParameterContext)_localctx).typeDeclaration = typeDeclaration();
			 type = ((ParameterContext)_localctx).typeDeclaration.type; 
			setState(508);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==EQ) {
				{
				setState(504);
				match(EQ);
				setState(505);
				((ParameterContext)_localctx).valueExpr = expression(0);
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
		enterRule(_localctx, 38, RULE_functionDeclaration);

		        ((FunctionDeclarationContext)_localctx).sym =  nullptr;
		        TransientArray<Variable*> variables(32);
		        TransientArray<Attribute*> attributes(32);
		        Symbol::Location location;
		    
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(517);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,47,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					{
					{
					setState(512);
					((FunctionDeclarationContext)_localctx).attribute = attribute();
					 if (attributes.Full()) { throw IndexOutOfBoundsException("Maximum of 32 attributes reached"); } attributes.Append(std::move(((FunctionDeclarationContext)_localctx).attribute.attr)); 
					}
					} 
				}
				setState(519);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,47,_ctx);
			}
			setState(520);
			((FunctionDeclarationContext)_localctx).name = match(IDENTIFIER);
			 location = SetupFile(); 
			setState(522);
			match(LP);
			setState(538);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__2 || _la==IDENTIFIER) {
				{
				setState(523);
				((FunctionDeclarationContext)_localctx).arg0 = parameter();
				 variables.Append(((FunctionDeclarationContext)_localctx).arg0.sym); 
				setState(526);
				_errHandler.sync(this);
				switch ( getInterpreter().adaptivePredict(_input,48,_ctx) ) {
				case 1:
					{
					setState(525);
					linePreprocessorEntry();
					}
					break;
				}
				setState(535);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while (_la==T__2 || _la==CO) {
					{
					setState(533);
					_errHandler.sync(this);
					switch (_input.LA(1)) {
					case CO:
						{
						setState(528);
						match(CO);
						setState(529);
						((FunctionDeclarationContext)_localctx).argn = parameter();
						 if (variables.Full()) throw IndexOutOfBoundsException("Maximum of 32 variables reached"); variables.Append(((FunctionDeclarationContext)_localctx).argn.sym); 
						}
						break;
					case T__2:
						{
						setState(532);
						linePreprocessorEntry();
						}
						break;
					default:
						throw new NoViableAltException(this);
					}
					}
					setState(537);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				}
			}

			setState(540);
			match(RP);
			setState(541);
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
		enterRule(_localctx, 40, RULE_function);

		        ((FunctionContext)_localctx).sym =  nullptr;
		    
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(544);
			((FunctionContext)_localctx).functionDeclaration = functionDeclaration();
			 ((FunctionContext)_localctx).sym =  ((FunctionContext)_localctx).functionDeclaration.sym; 
			setState(546);
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
		enterRule(_localctx, 42, RULE_program);

		        ((ProgramContext)_localctx).sym =  nullptr;
		        TransientArray<Expression*> entries(32);
		        TransientArray<Annotation*> annotations(32);
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(554);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==SOBAKA) {
				{
				{
				setState(549);
				((ProgramContext)_localctx).annotation = annotation();
				 if (annotations.Full()) { throw IndexOutOfBoundsException("Maximum of 32 annotations reached"); } annotations.Append(std::move(((ProgramContext)_localctx).annotation.annot)); 
				}
				}
				setState(556);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(557);
			match(T__10);
			 ((ProgramContext)_localctx).sym =  Alloc<Program>(); 
			setState(559);
			((ProgramContext)_localctx).name = match(IDENTIFIER);
			 _localctx.sym->location = SetupFile(); 
			setState(561);
			match(LB);
			setState(568);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & 3482558496006733838L) != 0) || ((((_la - 75)) & ~0x3f) == 0 && ((1L << (_la - 75)) & 7787L) != 0)) {
				{
				{
				setState(562);
				((ProgramContext)_localctx).assignment = expression(0);
				 if (entries.Full()) throw IndexOutOfBoundsException("Maximum of 32 entries reached"); entries.Append(((ProgramContext)_localctx).assignment.tree); 
				setState(564);
				match(SC);
				}
				}
				setState(570);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(571);
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
		enterRule(_localctx, 44, RULE_sampler);

		        TransientArray<Attribute*> attributes(32);
		        TransientArray<Annotation*> annotations(32);
		        TransientArray<Expression*> entries(32);
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			{
			setState(579);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==SOBAKA) {
				{
				{
				setState(574);
				((SamplerContext)_localctx).annotation = annotation();
				 if (annotations.Full()) throw IndexOutOfBoundsException("Maximum of 32 annotations reached"); annotations.Append(std::move(((SamplerContext)_localctx).annotation.annot)); 
				}
				}
				setState(581);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(587);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==IDENTIFIER) {
				{
				{
				setState(582);
				((SamplerContext)_localctx).attribute = attribute();
				 if (attributes.Full()) throw IndexOutOfBoundsException("Maximum of 32 attributes reached"); attributes.Append(std::move(((SamplerContext)_localctx).attribute.attr)); 
				}
				}
				setState(589);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(590);
			match(T__11);
			 ((SamplerContext)_localctx).sym =  Alloc<SamplerState>(); _localctx.sym->isImmutable = true; 
			}
			setState(593);
			((SamplerContext)_localctx).name = match(IDENTIFIER);
			 _localctx.sym->location = SetupFile(); 
			setState(595);
			match(LB);
			setState(602);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & 3482558496006733838L) != 0) || ((((_la - 75)) & ~0x3f) == 0 && ((1L << (_la - 75)) & 7787L) != 0)) {
				{
				{
				setState(596);
				((SamplerContext)_localctx).assign = expression(0);
				 if (entries.Full()) throw IndexOutOfBoundsException("Maximum of 32 entries reached"); entries.Append(((SamplerContext)_localctx).assign.tree); 
				setState(598);
				match(SC);
				}
				}
				setState(604);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(605);
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
		enterRule(_localctx, 46, RULE_state);

		        TransientArray<Expression*> entries(32);
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			{
			setState(608);
			match(T__12);
			 ((StateContext)_localctx).sym =  Alloc<RenderState>(); 
			}
			setState(611);
			((StateContext)_localctx).name = match(IDENTIFIER);
			 _localctx.sym->location = SetupFile(); 
			setState(613);
			match(LB);
			setState(620);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & 3482558496006733838L) != 0) || ((((_la - 75)) & ~0x3f) == 0 && ((1L << (_la - 75)) & 7787L) != 0)) {
				{
				{
				setState(614);
				((StateContext)_localctx).assign = expression(0);
				 if (entries.Full()) throw IndexOutOfBoundsException("Maximum of 32 entries reached"); entries.Append(((StateContext)_localctx).assign.tree); 
				setState(616);
				match(SC);
				}
				}
				setState(622);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(623);
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
		enterRule(_localctx, 48, RULE_statement);

		        ((StatementContext)_localctx).tree =  nullptr;
		    
		try {
			setState(654);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__6:
				enterOuterAlt(_localctx, 1);
				{
				setState(626);
				((StatementContext)_localctx).ifStatement = ifStatement();
				 ((StatementContext)_localctx).tree =  ((StatementContext)_localctx).ifStatement.tree; 
				}
				break;
			case LB:
				enterOuterAlt(_localctx, 2);
				{
				setState(629);
				((StatementContext)_localctx).scopeStatement = scopeStatement();
				 ((StatementContext)_localctx).tree =  ((StatementContext)_localctx).scopeStatement.tree; 
				}
				break;
			case T__13:
				enterOuterAlt(_localctx, 3);
				{
				setState(632);
				((StatementContext)_localctx).forStatement = forStatement();
				 ((StatementContext)_localctx).tree =  ((StatementContext)_localctx).forStatement.tree; 
				}
				break;
			case T__16:
			case T__17:
				enterOuterAlt(_localctx, 4);
				{
				setState(635);
				((StatementContext)_localctx).whileStatement = whileStatement();
				 ((StatementContext)_localctx).tree =  ((StatementContext)_localctx).whileStatement.tree; 
				}
				break;
			case T__23:
				enterOuterAlt(_localctx, 5);
				{
				setState(638);
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
				setState(641);
				((StatementContext)_localctx).terminateStatement = terminateStatement();
				 ((StatementContext)_localctx).tree =  ((StatementContext)_localctx).terminateStatement.tree; 
				}
				break;
			case T__22:
				enterOuterAlt(_localctx, 7);
				{
				setState(644);
				((StatementContext)_localctx).continueStatement = continueStatement();
				 ((StatementContext)_localctx).tree =  ((StatementContext)_localctx).continueStatement.tree; 
				}
				break;
			case T__26:
				enterOuterAlt(_localctx, 8);
				{
				setState(647);
				((StatementContext)_localctx).breakStatement = breakStatement();
				 ((StatementContext)_localctx).tree =  ((StatementContext)_localctx).breakStatement.tree; 
				}
				break;
			case T__0:
			case T__1:
			case T__2:
			case T__27:
			case T__28:
			case T__38:
			case LP:
			case LL:
			case NOT:
			case QO:
			case AND:
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
				setState(650);
				((StatementContext)_localctx).expressionStatement = expressionStatement();
				setState(651);
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
		public ExpressionListContext expressionList;
		public ExpressionListContext expressionList() {
			return getRuleContext(ExpressionListContext.class,0);
		}
		public ExpressionStatementContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_expressionStatement; }
	}

	public final ExpressionStatementContext expressionStatement() throws RecognitionException {
		ExpressionStatementContext _localctx = new ExpressionStatementContext(_ctx, getState());
		enterRule(_localctx, 50, RULE_expressionStatement);

		        ((ExpressionStatementContext)_localctx).tree =  nullptr;
		    
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(656);
			((ExpressionStatementContext)_localctx).expressionList = expressionList();

			        ((ExpressionStatementContext)_localctx).tree =  Alloc<ExpressionStatement>(((ExpressionStatementContext)_localctx).expressionList.expressions);
			        _localctx.tree->location = ((ExpressionStatementContext)_localctx).expressionList.expressions[0]->location; 
			    
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
		enterRule(_localctx, 52, RULE_ifStatement);

		        ((IfStatementContext)_localctx).tree =  nullptr;
		        Expression* condition = nullptr;
		        Statement* ifBody = nullptr;
		        Statement* elseBody = nullptr;
		        Symbol::Location location;
		    
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(659);
			match(T__6);
			 location = SetupFile(); 
			setState(661);
			match(LP);
			setState(662);
			((IfStatementContext)_localctx).condition = expression(0);
			 condition = ((IfStatementContext)_localctx).condition.tree; 
			setState(664);
			match(RP);
			setState(665);
			((IfStatementContext)_localctx).ifBody = statement();
			 ifBody = ((IfStatementContext)_localctx).ifBody.tree; 
			setState(671);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,59,_ctx) ) {
			case 1:
				{
				setState(667);
				match(T__7);
				setState(668);
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
		enterRule(_localctx, 54, RULE_forStatement);

		        ((ForStatementContext)_localctx).tree =  nullptr;
		        FixedArray<Variable*> declarations;
		        Expression* conditionExpression = nullptr;
		        Expression* loopExpression = nullptr;
		        Statement* contents = nullptr;
		        TransientArray<Attribute*> attributes(32);
		        Symbol::Location location;
		    
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(675);
			match(T__13);
			 location = SetupFile(); 
			setState(677);
			match(LP);
			setState(681);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__2 || _la==SOBAKA || _la==IDENTIFIER) {
				{
				setState(678);
				((ForStatementContext)_localctx).variables = variables();
				 declarations = ((ForStatementContext)_localctx).variables.vars; 
				}
			}

			setState(683);
			match(SC);
			setState(687);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 3482558496006733838L) != 0) || ((((_la - 75)) & ~0x3f) == 0 && ((1L << (_la - 75)) & 7787L) != 0)) {
				{
				setState(684);
				((ForStatementContext)_localctx).condition = expression(0);
				 conditionExpression = ((ForStatementContext)_localctx).condition.tree; 
				}
			}

			setState(689);
			match(SC);
			setState(693);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 3482558496006733838L) != 0) || ((((_la - 75)) & ~0x3f) == 0 && ((1L << (_la - 75)) & 7787L) != 0)) {
				{
				setState(690);
				((ForStatementContext)_localctx).loop = expression(0);
				 loopExpression = ((ForStatementContext)_localctx).loop.tree; 
				}
			}

			setState(695);
			match(RP);
			setState(701);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,63,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					{
					{
					setState(696);
					((ForStatementContext)_localctx).attribute = attribute();
					 if (attributes.Full()) { throw IndexOutOfBoundsException("Maximum of 32 attributes reached"); } attributes.Append(std::move(((ForStatementContext)_localctx).attribute.attr)); 
					}
					} 
				}
				setState(703);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,63,_ctx);
			}
			setState(704);
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
		enterRule(_localctx, 56, RULE_forRangeStatement);

		        ((ForRangeStatementContext)_localctx).tree =  nullptr;
		        Statement* contents = nullptr;
		        Symbol::Location location;
		    
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(708);
			match(T__13);
			 location = SetupFile(); 
			setState(710);
			match(LP);
			setState(711);
			((ForRangeStatementContext)_localctx).iterator = match(IDENTIFIER);
			setState(712);
			match(COL);
			setState(713);
			((ForRangeStatementContext)_localctx).start = match(IDENTIFIER);
			setState(714);
			match(T__14);
			setState(715);
			((ForRangeStatementContext)_localctx).end = match(IDENTIFIER);
			setState(716);
			match(RP);
			setState(717);
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
		enterRule(_localctx, 58, RULE_forUniformValueStatement);

		        ((ForUniformValueStatementContext)_localctx).tree =  nullptr;
		        Statement* contents = nullptr;
		        Symbol::Location location;
		    
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(721);
			match(T__15);
			 location = SetupFile(); 
			setState(723);
			match(LP);
			setState(724);
			expression(0);
			setState(725);
			match(RP);
			setState(726);
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
		enterRule(_localctx, 60, RULE_whileStatement);

		        ((WhileStatementContext)_localctx).tree =  nullptr;
		        Expression* conditionExpression = nullptr;
		        Statement* contents = nullptr;
		        bool isDoWhile = false;
		        Symbol::Location location;
		    
		try {
			setState(752);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__16:
				enterOuterAlt(_localctx, 1);
				{
				setState(730);
				match(T__16);
				 location = SetupFile(); 
				setState(732);
				match(LP);
				setState(733);
				((WhileStatementContext)_localctx).condition = expression(0);
				 conditionExpression = ((WhileStatementContext)_localctx).condition.tree; 
				setState(735);
				match(RP);
				setState(736);
				((WhileStatementContext)_localctx).content = statement();
				 contents = ((WhileStatementContext)_localctx).content.tree; 

				        ((WhileStatementContext)_localctx).tree =  Alloc<WhileStatement>(conditionExpression, contents, isDoWhile);
				        _localctx.tree->location = location;
				    
				}
				break;
			case T__17:
				enterOuterAlt(_localctx, 2);
				{
				setState(740);
				match(T__17);
				 location = SetupFile(); 
				setState(742);
				((WhileStatementContext)_localctx).content = statement();
				 contents = ((WhileStatementContext)_localctx).content.tree; isDoWhile = true; 
				setState(744);
				match(T__16);
				setState(745);
				match(LP);
				setState(746);
				((WhileStatementContext)_localctx).condition = expression(0);
				 conditionExpression = ((WhileStatementContext)_localctx).condition.tree; 
				setState(748);
				match(RP);
				setState(749);
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
		public AliasContext alias;
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
		public List<AliasContext> alias() {
			return getRuleContexts(AliasContext.class);
		}
		public AliasContext alias(int i) {
			return getRuleContext(AliasContext.class,i);
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
		enterRule(_localctx, 62, RULE_scopeStatement);

		        ((ScopeStatementContext)_localctx).tree =  nullptr;
		        PinnedArray<Symbol*> contents(0xFFFFFF);
			    std::vector<Expression*> unfinished;
		        Symbol::Location location;
		        Symbol::Location ends;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(754);
			match(LB);
			 location = SetupFile(); 
			setState(770);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & 3482593680546480286L) != 0) || ((((_la - 74)) & ~0x3f) == 0 && ((1L << (_la - 74)) & 15575L) != 0)) {
				{
				setState(768);
				_errHandler.sync(this);
				switch ( getInterpreter().adaptivePredict(_input,65,_ctx) ) {
				case 1:
					{
					setState(756);
					((ScopeStatementContext)_localctx).variables = variables();
					setState(757);
					match(SC);
					 for(Variable* var : ((ScopeStatementContext)_localctx).variables.vars) { contents.Append(var); } 
					}
					break;
				case 2:
					{
					setState(760);
					((ScopeStatementContext)_localctx).statement = statement();
					 contents.Append(((ScopeStatementContext)_localctx).statement.tree); 
					}
					break;
				case 3:
					{
					setState(763);
					((ScopeStatementContext)_localctx).alias = alias();
					setState(764);
					match(SC);
					 contents.Append(((ScopeStatementContext)_localctx).alias.sym); 
					}
					break;
				case 4:
					{
					setState(767);
					linePreprocessorEntry();
					}
					break;
				}
				}
				setState(772);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(773);
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
		enterRule(_localctx, 64, RULE_terminateStatement);

		        ((TerminateStatementContext)_localctx).tree =  nullptr;
		        Expression* returnValue = nullptr;
		        Symbol::Location location;
		    
		int _la;
		try {
			setState(798);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__18:
				enterOuterAlt(_localctx, 1);
				{
				setState(777);
				match(T__18);
				 location = SetupFile(); 
				setState(782);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 3482558496006733838L) != 0) || ((((_la - 75)) & ~0x3f) == 0 && ((1L << (_la - 75)) & 7787L) != 0)) {
					{
					setState(779);
					((TerminateStatementContext)_localctx).value = expression(0);
					 returnValue = ((TerminateStatementContext)_localctx).value.tree; 
					}
				}

				setState(784);
				match(SC);

				        ((TerminateStatementContext)_localctx).tree =  Alloc<TerminateStatement>(returnValue, TerminateStatement::TerminationType::Return);
				        _localctx.tree->location = location;
				    
				}
				break;
			case T__19:
				enterOuterAlt(_localctx, 2);
				{
				setState(786);
				match(T__19);
				 location = SetupFile(); 
				setState(788);
				match(SC);

				      ((TerminateStatementContext)_localctx).tree =  Alloc<TerminateStatement>(returnValue, TerminateStatement::TerminationType::Discard);
				      _localctx.tree->location = location;
				    
				}
				break;
			case T__20:
				enterOuterAlt(_localctx, 3);
				{
				setState(790);
				match(T__20);
				 location = SetupFile(); 
				setState(792);
				match(SC);

				      ((TerminateStatementContext)_localctx).tree =  Alloc<TerminateStatement>(returnValue, TerminateStatement::TerminationType::RayIgnoreIntersection);
				      _localctx.tree->location = location;
				    
				}
				break;
			case T__21:
				enterOuterAlt(_localctx, 4);
				{
				setState(794);
				match(T__21);
				 location = SetupFile(); 
				setState(796);
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
		enterRule(_localctx, 66, RULE_continueStatement);

		        ((ContinueStatementContext)_localctx).tree =  nullptr;
		        Symbol::Location location;
		    
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(800);
			match(T__22);
			 location = SetupFile(); 
			setState(802);
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
		enterRule(_localctx, 68, RULE_switchStatement);

		        ((SwitchStatementContext)_localctx).tree =  nullptr;
		        Expression* switchExpression;
		        TransientArray<Expression*> caseExpressions(256);
		        TransientArray<Statement*> caseStatements(256);
		        Symbol::Location location;
		        Statement* defaultStatement = nullptr;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(805);
			match(T__23);
			 location = SetupFile(); 
			setState(807);
			match(LP);
			setState(808);
			((SwitchStatementContext)_localctx).expression = expression(0);
			setState(809);
			match(RP);
			 switchExpression = ((SwitchStatementContext)_localctx).expression.tree; 
			setState(811);
			match(LB);
			setState(823);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__24) {
				{
				{
				setState(812);
				match(T__24);
				setState(813);
				((SwitchStatementContext)_localctx).expression = expression(0);
				setState(814);
				match(COL);
				 if (caseExpressions.Full()) { throw IndexOutOfBoundsException("Maximum of 256 case expressions reached"); } caseExpressions.Append(((SwitchStatementContext)_localctx).expression.tree); caseStatements.Append(nullptr); 
				setState(819);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 3482593680546480270L) != 0) || ((((_la - 75)) & ~0x3f) == 0 && ((1L << (_la - 75)) & 7787L) != 0)) {
					{
					setState(816);
					((SwitchStatementContext)_localctx).statement = statement();
					 
					                    caseStatements.back() = ((SwitchStatementContext)_localctx).statement.tree;
					                
					}
				}

				}
				}
				setState(825);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(833);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__25) {
				{
				setState(826);
				match(T__25);
				setState(827);
				match(COL);
				setState(831);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 3482593680546480270L) != 0) || ((((_la - 75)) & ~0x3f) == 0 && ((1L << (_la - 75)) & 7787L) != 0)) {
					{
					setState(828);
					((SwitchStatementContext)_localctx).statement = statement();

					                    defaultStatement = ((SwitchStatementContext)_localctx).statement.tree;
					                
					}
				}

				}
			}

			setState(835);
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
		enterRule(_localctx, 70, RULE_breakStatement);

		        ((BreakStatementContext)_localctx).tree =  nullptr;
		        Symbol::Location location;
		    
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(838);
			match(T__26);
			 location = SetupFile(); 
			setState(840);
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
		public ExpressionListContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_expressionList; }
	}

	public final ExpressionListContext expressionList() throws RecognitionException {
		ExpressionListContext _localctx = new ExpressionListContext(_ctx, getState());
		enterRule(_localctx, 72, RULE_expressionList);

		        TransientArray<Expression*> list(256);
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(844);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,73,_ctx) ) {
			case 1:
				{
				setState(843);
				linePreprocessorEntry();
				}
				break;
			}
			setState(846);
			((ExpressionListContext)_localctx).e1 = expression(0);
			 list.Append(((ExpressionListContext)_localctx).e1.tree); 
			setState(855);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__2 || _la==CO) {
				{
				setState(853);
				_errHandler.sync(this);
				switch (_input.LA(1)) {
				case CO:
					{
					setState(848);
					match(CO);
					setState(849);
					((ExpressionListContext)_localctx).e2 = expression(0);

					        list.Append(((ExpressionListContext)_localctx).e2.tree);
					    
					}
					break;
				case T__2:
					{
					setState(852);
					linePreprocessorEntry();
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				}
				setState(857);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}

			        ((ExpressionListContext)_localctx).expressions =  std::move(list);
			    
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
		public ExpressionListContext list;
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
		public TerminalNode AND() { return getToken(GPULangParser.AND, 0); }
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
		public ExpressionListContext expressionList() {
			return getRuleContext(ExpressionListContext.class,0);
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
		int _startState = 74;
		enterRecursionRule(_localctx, 74, RULE_expression, _p);

		        ((ExpressionContext)_localctx).tree =  nullptr;
		        Symbol::Location location;
		        FixedArray<Expression*> args;
		    
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(868);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__27:
			case T__28:
			case NOT:
			case AND:
			case CONJUGATE:
			case ADD_OP:
			case SUB_OP:
			case MUL_OP:
				{
				setState(861);
				((ExpressionContext)_localctx).op = _input.LT(1);
				_la = _input.LA(1);
				if ( !(((((_la - 28)) & ~0x3f) == 0 && ((1L << (_la - 28)) & 1548116738179075L) != 0)) ) {
					((ExpressionContext)_localctx).op = (Token)_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				setState(862);
				((ExpressionContext)_localctx).p = expression(14);

				        ((ExpressionContext)_localctx).tree =  Alloc<UnaryExpression>(StringToFourCC((((ExpressionContext)_localctx).op!=null?((ExpressionContext)_localctx).op.getText():null)), true, ((ExpressionContext)_localctx).p.tree);
				        _localctx.tree->location = ((ExpressionContext)_localctx).p.tree->location;
				    
				}
				break;
			case T__0:
			case T__1:
			case T__2:
			case T__38:
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
				setState(865);
				((ExpressionContext)_localctx).atom = binaryexpatom();
				 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).atom.tree; 
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
			_ctx.stop = _input.LT(-1);
			setState(996);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,80,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					if ( _parseListeners!=null ) triggerExitRuleEvent();
					_prevctx = _localctx;
					{
					setState(994);
					_errHandler.sync(this);
					switch ( getInterpreter().adaptivePredict(_input,79,_ctx) ) {
					case 1:
						{
						_localctx = new ExpressionContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expression);
						setState(870);
						if (!(precpred(_ctx, 17))) throw new FailedPredicateException(this, "precpred(_ctx, 17)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(872);
						match(DOT);
						 location = SetupFile(); 
						setState(874);
						((ExpressionContext)_localctx).e2 = expression(18);

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
						setState(877);
						if (!(precpred(_ctx, 16))) throw new FailedPredicateException(this, "precpred(_ctx, 16)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(879);
						match(ARROW);
						 location = SetupFile(); 
						setState(881);
						((ExpressionContext)_localctx).e2 = expression(17);

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
						setState(884);
						if (!(precpred(_ctx, 13))) throw new FailedPredicateException(this, "precpred(_ctx, 13)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(886);
						((ExpressionContext)_localctx).op = _input.LT(1);
						_la = _input.LA(1);
						if ( !(((((_la - 72)) & ~0x3f) == 0 && ((1L << (_la - 72)) & 97L) != 0)) ) {
							((ExpressionContext)_localctx).op = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						 location = SetupFile(); 
						setState(888);
						((ExpressionContext)_localctx).e2 = expression(14);
							
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
						setState(891);
						if (!(precpred(_ctx, 12))) throw new FailedPredicateException(this, "precpred(_ctx, 12)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(893);
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
						setState(895);
						((ExpressionContext)_localctx).e2 = expression(13);

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
						setState(898);
						if (!(precpred(_ctx, 11))) throw new FailedPredicateException(this, "precpred(_ctx, 11)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(900);
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
						setState(902);
						((ExpressionContext)_localctx).e2 = expression(12);

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
						setState(905);
						if (!(precpred(_ctx, 10))) throw new FailedPredicateException(this, "precpred(_ctx, 10)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(907);
						((ExpressionContext)_localctx).op = _input.LT(1);
						_la = _input.LA(1);
						if ( !(((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & 15L) != 0)) ) {
							((ExpressionContext)_localctx).op = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						 location = SetupFile(); 
						setState(909);
						((ExpressionContext)_localctx).e2 = expression(11);

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
						setState(912);
						if (!(precpred(_ctx, 9))) throw new FailedPredicateException(this, "precpred(_ctx, 9)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(914);
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
						setState(916);
						((ExpressionContext)_localctx).e2 = expression(10);

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
						setState(919);
						if (!(precpred(_ctx, 8))) throw new FailedPredicateException(this, "precpred(_ctx, 8)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(921);
						match(AND);
						 location = SetupFile(); 
						setState(923);
						((ExpressionContext)_localctx).e2 = expression(9);

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
						setState(926);
						if (!(precpred(_ctx, 7))) throw new FailedPredicateException(this, "precpred(_ctx, 7)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(928);
						match(XOR);
						 location = SetupFile(); 
						setState(930);
						((ExpressionContext)_localctx).e2 = expression(8);

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
						setState(933);
						if (!(precpred(_ctx, 6))) throw new FailedPredicateException(this, "precpred(_ctx, 6)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(935);
						match(OR);
						 location = SetupFile(); 
						setState(937);
						((ExpressionContext)_localctx).e2 = expression(7);

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
						setState(940);
						if (!(precpred(_ctx, 5))) throw new FailedPredicateException(this, "precpred(_ctx, 5)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(942);
						match(LOGICAND);
						 location = SetupFile(); 
						setState(944);
						((ExpressionContext)_localctx).e2 = expression(6);

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
						setState(947);
						if (!(precpred(_ctx, 4))) throw new FailedPredicateException(this, "precpred(_ctx, 4)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(949);
						match(LOGICOR);
						 location = SetupFile(); 
						setState(951);
						((ExpressionContext)_localctx).e2 = expression(5);

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
						setState(954);
						if (!(precpred(_ctx, 3))) throw new FailedPredicateException(this, "precpred(_ctx, 3)");
						setState(955);
						match(QU);
						 location = SetupFile(); 
						setState(957);
						((ExpressionContext)_localctx).ifBody = expression(0);
						setState(958);
						match(COL);
						setState(959);
						((ExpressionContext)_localctx).elseBody = expression(3);
						 
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
						setState(962);
						if (!(precpred(_ctx, 2))) throw new FailedPredicateException(this, "precpred(_ctx, 2)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(964);
						((ExpressionContext)_localctx).op = _input.LT(1);
						_la = _input.LA(1);
						if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & 758857082672775168L) != 0)) ) {
							((ExpressionContext)_localctx).op = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						 location = SetupFile(); 
						setState(966);
						((ExpressionContext)_localctx).e2 = expression(2);

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
						setState(969);
						if (!(precpred(_ctx, 19))) throw new FailedPredicateException(this, "precpred(_ctx, 19)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(971);
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
					case 16:
						{
						_localctx = new ExpressionContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expression);
						setState(974);
						if (!(precpred(_ctx, 18))) throw new FailedPredicateException(this, "precpred(_ctx, 18)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(976);
						match(LP);
						 location = SetupFile(); 
						setState(981);
						_errHandler.sync(this);
						_la = _input.LA(1);
						if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 3482558496006733838L) != 0) || ((((_la - 75)) & ~0x3f) == 0 && ((1L << (_la - 75)) & 7787L) != 0)) {
							{
							setState(978);
							((ExpressionContext)_localctx).list = expressionList();
							 args = std::move(((ExpressionContext)_localctx).list.expressions); 
							}
						}

						setState(983);
						match(RP);
						         
						                  CallExpression* expr = Alloc<CallExpression>(_localctx.tree, std::move(args));
						                  expr->location = ((ExpressionContext)_localctx).e1.tree->location;
						                  ((ExpressionContext)_localctx).tree =  expr;
						              
						}
						break;
					case 17:
						{
						_localctx = new ExpressionContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expression);
						setState(985);
						if (!(precpred(_ctx, 15))) throw new FailedPredicateException(this, "precpred(_ctx, 15)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(987);
						match(LL);
						 location = SetupFile(); 
						setState(990);
						_errHandler.sync(this);
						_la = _input.LA(1);
						if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 3482558496006733838L) != 0) || ((((_la - 75)) & ~0x3f) == 0 && ((1L << (_la - 75)) & 7787L) != 0)) {
							{
							setState(989);
							((ExpressionContext)_localctx).e3 = expression(0);
							}
						}

						setState(992);
						match(RR);

						                  ArrayIndexExpression* expr = Alloc<ArrayIndexExpression>(_localctx.tree, ((ExpressionContext)_localctx).e3.tree);
						                  expr->location = ((ExpressionContext)_localctx).e1.tree->location;
						                  ((ExpressionContext)_localctx).tree =  expr;
						              
						}
						break;
					}
					} 
				}
				setState(998);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,80,_ctx);
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
		public Token ident;
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
		public TerminalNode LESS() { return getToken(GPULangParser.LESS, 0); }
		public TerminalNode GREATER() { return getToken(GPULangParser.GREATER, 0); }
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
		enterRule(_localctx, 76, RULE_binaryexpatom);

		        ((BinaryexpatomContext)_localctx).tree =  nullptr;
		        Symbol::Location begin = BeginLocationRange();
		    
		try {
			setState(1035);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,81,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(999);
				((BinaryexpatomContext)_localctx).initializerExpression = initializerExpression();
				 ((BinaryexpatomContext)_localctx).tree =  ((BinaryexpatomContext)_localctx).initializerExpression.tree; 
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(1002);
				((BinaryexpatomContext)_localctx).arrayInitializerExpression = arrayInitializerExpression();
				 ((BinaryexpatomContext)_localctx).tree =  ((BinaryexpatomContext)_localctx).arrayInitializerExpression.tree; 
				}
				break;
			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(1005);
				match(LP);
				setState(1006);
				((BinaryexpatomContext)_localctx).expression = expression(0);
				setState(1007);
				match(RP);
				 ((BinaryexpatomContext)_localctx).tree =  ((BinaryexpatomContext)_localctx).expression.tree; 
				}
				break;
			case 4:
				enterOuterAlt(_localctx, 4);
				{
				setState(1010);
				((BinaryexpatomContext)_localctx).INTEGERLITERAL = match(INTEGERLITERAL);
				 ((BinaryexpatomContext)_localctx).tree =  Alloc<IntExpression>(atoi((((BinaryexpatomContext)_localctx).INTEGERLITERAL!=null?((BinaryexpatomContext)_localctx).INTEGERLITERAL.getText():null).c_str())); _localctx.tree->location = SetupFile(); 
				}
				break;
			case 5:
				enterOuterAlt(_localctx, 5);
				{
				setState(1012);
				((BinaryexpatomContext)_localctx).UINTEGERLITERAL = match(UINTEGERLITERAL);
				 ((BinaryexpatomContext)_localctx).tree =  Alloc<UIntExpression>(strtoul((((BinaryexpatomContext)_localctx).UINTEGERLITERAL!=null?((BinaryexpatomContext)_localctx).UINTEGERLITERAL.getText():null).c_str(), nullptr, 10)); _localctx.tree->location = SetupFile(); 
				}
				break;
			case 6:
				enterOuterAlt(_localctx, 6);
				{
				setState(1014);
				((BinaryexpatomContext)_localctx).FLOATLITERAL = match(FLOATLITERAL);
				 ((BinaryexpatomContext)_localctx).tree =  Alloc<FloatExpression>(atof((((BinaryexpatomContext)_localctx).FLOATLITERAL!=null?((BinaryexpatomContext)_localctx).FLOATLITERAL.getText():null).c_str())); _localctx.tree->location = SetupFile(); 
				}
				break;
			case 7:
				enterOuterAlt(_localctx, 7);
				{
				setState(1016);
				((BinaryexpatomContext)_localctx).DOUBLELITERAL = match(DOUBLELITERAL);
				 ((BinaryexpatomContext)_localctx).tree =  Alloc<FloatExpression>(atof((((BinaryexpatomContext)_localctx).DOUBLELITERAL!=null?((BinaryexpatomContext)_localctx).DOUBLELITERAL.getText():null).c_str())); _localctx.tree->location = SetupFile(); 
				}
				break;
			case 8:
				enterOuterAlt(_localctx, 8);
				{
				setState(1018);
				((BinaryexpatomContext)_localctx).HEX = match(HEX);
				 ((BinaryexpatomContext)_localctx).tree =  Alloc<UIntExpression>(strtoul((((BinaryexpatomContext)_localctx).HEX!=null?((BinaryexpatomContext)_localctx).HEX.getText():null).c_str(), nullptr, 16)); _localctx.tree->location = SetupFile(); 
				}
				break;
			case 9:
				enterOuterAlt(_localctx, 9);
				{
				setState(1020);
				((BinaryexpatomContext)_localctx).string = string();
				 ((BinaryexpatomContext)_localctx).tree =  Alloc<StringExpression>(((BinaryexpatomContext)_localctx).string.val); _localctx.tree->location = EndLocationRange(begin); 
				}
				break;
			case 10:
				enterOuterAlt(_localctx, 10);
				{
				setState(1023);
				((BinaryexpatomContext)_localctx).IDENTIFIER = match(IDENTIFIER);
				 ((BinaryexpatomContext)_localctx).tree =  Alloc<SymbolExpression>(FixedString((((BinaryexpatomContext)_localctx).IDENTIFIER!=null?((BinaryexpatomContext)_localctx).IDENTIFIER.getText():null))); _localctx.tree->location = SetupFile(); 
				}
				break;
			case 11:
				enterOuterAlt(_localctx, 11);
				{
				setState(1025);
				((BinaryexpatomContext)_localctx).boolean_ = boolean_();
				 ((BinaryexpatomContext)_localctx).tree =  Alloc<BoolExpression>(((BinaryexpatomContext)_localctx).boolean.val); _localctx.tree->location = SetupFile(); 
				}
				break;
			case 12:
				enterOuterAlt(_localctx, 12);
				{
				setState(1028);
				match(T__38);
				 begin = SetupFile(); 
				setState(1030);
				match(LESS);
				setState(1031);
				((BinaryexpatomContext)_localctx).ident = match(IDENTIFIER);
				setState(1032);
				match(GREATER);

					((BinaryexpatomContext)_localctx).tree =  Alloc<DeclaredExpression>(FixedString((((BinaryexpatomContext)_localctx).ident!=null?((BinaryexpatomContext)_localctx).ident.getText():null)));
				        _localctx.tree->location = begin ; 
				    
				}
				break;
			case 13:
				enterOuterAlt(_localctx, 13);
				{
				setState(1034);
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
		public ExpressionListContext list;
		public TerminalNode LB() { return getToken(GPULangParser.LB, 0); }
		public TerminalNode RB() { return getToken(GPULangParser.RB, 0); }
		public TerminalNode IDENTIFIER() { return getToken(GPULangParser.IDENTIFIER, 0); }
		public ExpressionListContext expressionList() {
			return getRuleContext(ExpressionListContext.class,0);
		}
		public InitializerExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_initializerExpression; }
	}

	public final InitializerExpressionContext initializerExpression() throws RecognitionException {
		InitializerExpressionContext _localctx = new InitializerExpressionContext(_ctx, getState());
		enterRule(_localctx, 78, RULE_initializerExpression);

		        ((InitializerExpressionContext)_localctx).tree =  nullptr;
		        FixedString type;
		        Symbol::Location location;
			FixedArray<Expression*> initializers;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(1037);
			((InitializerExpressionContext)_localctx).type = match(IDENTIFIER);
			 type = FixedString((((InitializerExpressionContext)_localctx).type!=null?((InitializerExpressionContext)_localctx).type.getText():null)); 
			setState(1039);
			match(LB);
			 location = SetupFile(); 
			setState(1044);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 3482558496006733838L) != 0) || ((((_la - 75)) & ~0x3f) == 0 && ((1L << (_la - 75)) & 7787L) != 0)) {
				{
				setState(1041);
				((InitializerExpressionContext)_localctx).list = expressionList();
				 initializers = ((InitializerExpressionContext)_localctx).list.expressions; 
				}
			}

			setState(1046);
			match(RB);

			        ((InitializerExpressionContext)_localctx).tree =  Alloc<InitializerExpression>(std::move(initializers), type);
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
		public ExpressionListContext list;
		public TerminalNode LL() { return getToken(GPULangParser.LL, 0); }
		public TerminalNode RR() { return getToken(GPULangParser.RR, 0); }
		public ExpressionListContext expressionList() {
			return getRuleContext(ExpressionListContext.class,0);
		}
		public ArrayInitializerExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_arrayInitializerExpression; }
	}

	public final ArrayInitializerExpressionContext arrayInitializerExpression() throws RecognitionException {
		ArrayInitializerExpressionContext _localctx = new ArrayInitializerExpressionContext(_ctx, getState());
		enterRule(_localctx, 80, RULE_arrayInitializerExpression);

		        ((ArrayInitializerExpressionContext)_localctx).tree =  nullptr;
		        Symbol::Location location;
		    
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(1049);
			match(LL);
			 location = SetupFile(); 
			{
			setState(1051);
			((ArrayInitializerExpressionContext)_localctx).list = expressionList();
			}
			setState(1052);
			match(RR);

			        ((ArrayInitializerExpressionContext)_localctx).tree =  Alloc<ArrayInitializerExpression>(((ArrayInitializerExpressionContext)_localctx).list.expressions);
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
		enterRule(_localctx, 82, RULE_floatVecLiteralExpression);

		        ((FloatVecLiteralExpressionContext)_localctx).tree =  nullptr;
		        TransientArray<float> values(32);
		        Symbol::Location location;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(1055);
			match(LESS);
			 location = SetupFile(); 
			{
			setState(1057);
			((FloatVecLiteralExpressionContext)_localctx).arg0 = match(FLOATLITERAL);
			 values.Append(atof((((FloatVecLiteralExpressionContext)_localctx).arg0!=null?((FloatVecLiteralExpressionContext)_localctx).arg0.getText():null).c_str())); 
			}
			setState(1061);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__2) {
				{
				setState(1060);
				linePreprocessorEntry();
				}
			}

			setState(1066); 
			_errHandler.sync(this);
			_la = _input.LA(1);
			do {
				{
				{
				setState(1063);
				match(CO);
				setState(1064);
				((FloatVecLiteralExpressionContext)_localctx).argN = match(FLOATLITERAL);
				 values.Append(atof((((FloatVecLiteralExpressionContext)_localctx).argN!=null?((FloatVecLiteralExpressionContext)_localctx).argN.getText():null).c_str())); 
				}
				}
				setState(1068); 
				_errHandler.sync(this);
				_la = _input.LA(1);
			} while ( _la==CO );
			setState(1070);
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
		enterRule(_localctx, 84, RULE_doubleVecLiteralExpression);

		        ((DoubleVecLiteralExpressionContext)_localctx).tree =  nullptr;
		        TransientArray<float> values(32);
		        Symbol::Location location;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(1073);
			match(LESS);
			 location = SetupFile(); 
			{
			setState(1075);
			((DoubleVecLiteralExpressionContext)_localctx).arg0 = match(DOUBLELITERAL);
			 values.Append(atof((((DoubleVecLiteralExpressionContext)_localctx).arg0!=null?((DoubleVecLiteralExpressionContext)_localctx).arg0.getText():null).c_str())); 
			}
			setState(1079);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__2) {
				{
				setState(1078);
				linePreprocessorEntry();
				}
			}

			setState(1084); 
			_errHandler.sync(this);
			_la = _input.LA(1);
			do {
				{
				{
				setState(1081);
				match(CO);
				setState(1082);
				((DoubleVecLiteralExpressionContext)_localctx).argN = match(DOUBLELITERAL);
				 values.Append(atof((((DoubleVecLiteralExpressionContext)_localctx).argN!=null?((DoubleVecLiteralExpressionContext)_localctx).argN.getText():null).c_str())); 
				}
				}
				setState(1086); 
				_errHandler.sync(this);
				_la = _input.LA(1);
			} while ( _la==CO );
			setState(1088);
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
		enterRule(_localctx, 86, RULE_intVecLiteralExpression);

		        ((IntVecLiteralExpressionContext)_localctx).tree =  nullptr;
		        TransientArray<int> values(32);
		        Symbol::Location location;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(1091);
			match(LESS);
			 location = SetupFile(); 
			{
			setState(1093);
			((IntVecLiteralExpressionContext)_localctx).arg0 = match(INTEGERLITERAL);
			 values.Append(atof((((IntVecLiteralExpressionContext)_localctx).arg0!=null?((IntVecLiteralExpressionContext)_localctx).arg0.getText():null).c_str())); 
			}
			setState(1097);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__2) {
				{
				setState(1096);
				linePreprocessorEntry();
				}
			}

			setState(1102); 
			_errHandler.sync(this);
			_la = _input.LA(1);
			do {
				{
				{
				setState(1099);
				match(CO);
				setState(1100);
				((IntVecLiteralExpressionContext)_localctx).argN = match(INTEGERLITERAL);
				 values.Append(atof((((IntVecLiteralExpressionContext)_localctx).argN!=null?((IntVecLiteralExpressionContext)_localctx).argN.getText():null).c_str())); 
				}
				}
				setState(1104); 
				_errHandler.sync(this);
				_la = _input.LA(1);
			} while ( _la==CO );
			setState(1106);
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
		enterRule(_localctx, 88, RULE_uintVecLiteralExpression);

		        ((UintVecLiteralExpressionContext)_localctx).tree =  nullptr;
		        TransientArray<unsigned int> values(32);
		        Symbol::Location location;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(1109);
			match(LESS);
			 location = SetupFile(); 
			{
			setState(1111);
			((UintVecLiteralExpressionContext)_localctx).arg0 = match(UINTEGERLITERAL);
			 values.Append(atof((((UintVecLiteralExpressionContext)_localctx).arg0!=null?((UintVecLiteralExpressionContext)_localctx).arg0.getText():null).c_str())); 
			}
			setState(1115);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__2) {
				{
				setState(1114);
				linePreprocessorEntry();
				}
			}

			setState(1120); 
			_errHandler.sync(this);
			_la = _input.LA(1);
			do {
				{
				{
				setState(1117);
				match(CO);
				setState(1118);
				((UintVecLiteralExpressionContext)_localctx).argN = match(UINTEGERLITERAL);
				 values.Append(atof((((UintVecLiteralExpressionContext)_localctx).argN!=null?((UintVecLiteralExpressionContext)_localctx).argN.getText():null).c_str())); 
				}
				}
				setState(1122); 
				_errHandler.sync(this);
				_la = _input.LA(1);
			} while ( _la==CO );
			setState(1124);
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
		enterRule(_localctx, 90, RULE_booleanVecLiteralExpression);

		        ((BooleanVecLiteralExpressionContext)_localctx).tree =  nullptr;
		        TransientArray<bool> values(32);
		        Symbol::Location location;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(1127);
			match(LESS);
			 location = SetupFile(); 
			{
			setState(1129);
			((BooleanVecLiteralExpressionContext)_localctx).arg0 = boolean_();
			 values.Append(atof((((BooleanVecLiteralExpressionContext)_localctx).arg0!=null?_input.getText(((BooleanVecLiteralExpressionContext)_localctx).arg0.start,((BooleanVecLiteralExpressionContext)_localctx).arg0.stop):null).c_str())); 
			}
			setState(1133);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__2) {
				{
				setState(1132);
				linePreprocessorEntry();
				}
			}

			setState(1139); 
			_errHandler.sync(this);
			_la = _input.LA(1);
			do {
				{
				{
				setState(1135);
				match(CO);
				setState(1136);
				((BooleanVecLiteralExpressionContext)_localctx).argN = boolean_();
				 values.Append(atof((((BooleanVecLiteralExpressionContext)_localctx).argN!=null?_input.getText(((BooleanVecLiteralExpressionContext)_localctx).argN.start,((BooleanVecLiteralExpressionContext)_localctx).argN.stop):null).c_str())); 
				}
				}
				setState(1141); 
				_errHandler.sync(this);
				_la = _input.LA(1);
			} while ( _la==CO );
			setState(1143);
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
		case 37:
			return expression_sempred((ExpressionContext)_localctx, predIndex);
		}
		return true;
	}
	private boolean expression_sempred(ExpressionContext _localctx, int predIndex) {
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

	public static final String _serializedATN =
		"\u0004\u0001X\u047b\u0002\u0000\u0007\u0000\u0002\u0001\u0007\u0001\u0002"+
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
		"(\u0007(\u0002)\u0007)\u0002*\u0007*\u0002+\u0007+\u0002,\u0007,\u0002"+
		"-\u0007-\u0001\u0000\u0001\u0000\u0001\u0000\u0005\u0000`\b\u0000\n\u0000"+
		"\f\u0000c\t\u0000\u0001\u0000\u0001\u0000\u0001\u0000\u0001\u0000\u0005"+
		"\u0000i\b\u0000\n\u0000\f\u0000l\t\u0000\u0001\u0000\u0003\u0000o\b\u0000"+
		"\u0001\u0001\u0001\u0001\u0001\u0001\u0005\u0001t\b\u0001\n\u0001\f\u0001"+
		"w\t\u0001\u0001\u0001\u0001\u0001\u0001\u0001\u0001\u0001\u0005\u0001"+
		"}\b\u0001\n\u0001\f\u0001\u0080\t\u0001\u0001\u0001\u0003\u0001\u0083"+
		"\b\u0001\u0001\u0002\u0001\u0002\u0001\u0002\u0001\u0002\u0003\u0002\u0089"+
		"\b\u0002\u0001\u0003\u0001\u0003\u0001\u0003\u0001\u0003\u0001\u0004\u0001"+
		"\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001"+
		"\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001"+
		"\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001"+
		"\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001"+
		"\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001"+
		"\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001"+
		"\u0004\u0001\u0004\u0001\u0004\u0005\u0004\u00b7\b\u0004\n\u0004\f\u0004"+
		"\u00ba\t\u0004\u0001\u0005\u0001\u0005\u0001\u0005\u0001\u0005\u0001\u0005"+
		"\u0001\u0005\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0006"+
		"\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0007\u0001\u0007\u0001\u0007"+
		"\u0001\u0007\u0001\u0007\u0001\u0007\u0001\u0007\u0001\u0007\u0001\u0007"+
		"\u0001\b\u0001\b\u0001\b\u0001\b\u0001\b\u0001\b\u0001\b\u0001\b\u0001"+
		"\b\u0003\b\u00dc\b\b\u0001\t\u0001\t\u0001\t\u0001\t\u0001\t\u0001\t\u0001"+
		"\t\u0001\t\u0003\t\u00e6\b\t\u0001\t\u0001\t\u0001\t\u0001\t\u0005\t\u00ec"+
		"\b\t\n\t\f\t\u00ef\t\t\u0001\t\u0001\t\u0001\t\u0001\n\u0001\n\u0001\n"+
		"\u0001\n\u0001\n\u0001\n\u0001\n\u0001\n\u0001\n\u0001\n\u0001\n\u0001"+
		"\n\u0001\n\u0001\n\u0001\n\u0001\n\u0001\n\u0001\n\u0001\n\u0001\n\u0001"+
		"\n\u0005\n\u0109\b\n\n\n\f\n\u010c\t\n\u0001\n\u0001\n\u0001\n\u0001\u000b"+
		"\u0001\u000b\u0001\u000b\u0001\u000b\u0001\u000b\u0001\u000b\u0003\u000b"+
		"\u0117\b\u000b\u0001\f\u0001\f\u0001\f\u0001\f\u0001\f\u0001\f\u0001\f"+
		"\u0001\f\u0001\f\u0001\f\u0001\f\u0001\f\u0001\f\u0001\f\u0001\f\u0001"+
		"\f\u0001\f\u0005\f\u012a\b\f\n\f\f\f\u012d\t\f\u0001\f\u0001\f\u0001\f"+
		"\u0001\f\u0001\r\u0001\r\u0001\r\u0001\r\u0001\r\u0001\r\u0001\r\u0001"+
		"\r\u0001\r\u0001\r\u0001\r\u0001\r\u0003\r\u013f\b\r\u0001\r\u0001\r\u0001"+
		"\u000e\u0005\u000e\u0144\b\u000e\n\u000e\f\u000e\u0147\t\u000e\u0001\u000e"+
		"\u0001\u000e\u0001\u000e\u0005\u000e\u014c\b\u000e\n\u000e\f\u000e\u014f"+
		"\t\u000e\u0001\u000e\u0001\u000e\u0001\u000e\u0004\u000e\u0154\b\u000e"+
		"\u000b\u000e\f\u000e\u0155\u0001\u000e\u0001\u000e\u0001\u000e\u0003\u000e"+
		"\u015b\b\u000e\u0001\u000e\u0001\u000e\u0001\u000e\u0001\u000e\u0005\u000e"+
		"\u0161\b\u000e\n\u000e\f\u000e\u0164\t\u000e\u0001\u000e\u0001\u000e\u0001"+
		"\u000e\u0001\u000e\u0003\u000e\u016a\b\u000e\u0001\u000e\u0001\u000e\u0001"+
		"\u000e\u0001\u000e\u0001\u000e\u0001\u000e\u0001\u000e\u0001\u000e\u0005"+
		"\u000e\u0174\b\u000e\n\u000e\f\u000e\u0177\t\u000e\u0003\u000e\u0179\b"+
		"\u000e\u0001\u000e\u0001\u000e\u0001\u000f\u0005\u000f\u017e\b\u000f\n"+
		"\u000f\f\u000f\u0181\t\u000f\u0001\u000f\u0001\u000f\u0001\u000f\u0005"+
		"\u000f\u0186\b\u000f\n\u000f\f\u000f\u0189\t\u000f\u0001\u000f\u0001\u000f"+
		"\u0001\u000f\u0005\u000f\u018e\b\u000f\n\u000f\f\u000f\u0191\t\u000f\u0001"+
		"\u000f\u0001\u000f\u0001\u000f\u0001\u000f\u0001\u0010\u0001\u0010\u0001"+
		"\u0010\u0001\u0010\u0001\u0010\u0001\u0010\u0001\u0010\u0001\u0010\u0001"+
		"\u0010\u0001\u0010\u0001\u0010\u0001\u0010\u0001\u0010\u0001\u0010\u0003"+
		"\u0010\u01a5\b\u0010\u0001\u0010\u0005\u0010\u01a8\b\u0010\n\u0010\f\u0010"+
		"\u01ab\t\u0010\u0001\u0010\u0001\u0010\u0001\u0010\u0001\u0010\u0001\u0010"+
		"\u0005\u0010\u01b2\b\u0010\n\u0010\f\u0010\u01b5\t\u0010\u0001\u0010\u0001"+
		"\u0010\u0001\u0010\u0001\u0011\u0001\u0011\u0001\u0011\u0001\u0011\u0001"+
		"\u0011\u0001\u0011\u0001\u0011\u0003\u0011\u01c1\b\u0011\u0001\u0011\u0001"+
		"\u0011\u0001\u0011\u0001\u0011\u0001\u0011\u0001\u0011\u0001\u0011\u0003"+
		"\u0011\u01ca\b\u0011\u0001\u0011\u0001\u0011\u0003\u0011\u01ce\b\u0011"+
		"\u0001\u0011\u0001\u0011\u0001\u0011\u0001\u0011\u0001\u0011\u0001\u0011"+
		"\u0001\u0011\u0003\u0011\u01d7\b\u0011\u0001\u0011\u0001\u0011\u0005\u0011"+
		"\u01db\b\u0011\n\u0011\f\u0011\u01de\t\u0011\u0001\u0011\u0003\u0011\u01e1"+
		"\b\u0011\u0001\u0011\u0001\u0011\u0001\u0011\u0001\u0012\u0005\u0012\u01e7"+
		"\b\u0012\n\u0012\f\u0012\u01ea\t\u0012\u0001\u0012\u0001\u0012\u0001\u0012"+
		"\u0005\u0012\u01ef\b\u0012\n\u0012\f\u0012\u01f2\t\u0012\u0001\u0012\u0001"+
		"\u0012\u0001\u0012\u0001\u0012\u0001\u0012\u0001\u0012\u0001\u0012\u0001"+
		"\u0012\u0001\u0012\u0003\u0012\u01fd\b\u0012\u0001\u0012\u0001\u0012\u0001"+
		"\u0013\u0001\u0013\u0001\u0013\u0005\u0013\u0204\b\u0013\n\u0013\f\u0013"+
		"\u0207\t\u0013\u0001\u0013\u0001\u0013\u0001\u0013\u0001\u0013\u0001\u0013"+
		"\u0001\u0013\u0003\u0013\u020f\b\u0013\u0001\u0013\u0001\u0013\u0001\u0013"+
		"\u0001\u0013\u0001\u0013\u0005\u0013\u0216\b\u0013\n\u0013\f\u0013\u0219"+
		"\t\u0013\u0003\u0013\u021b\b\u0013\u0001\u0013\u0001\u0013\u0001\u0013"+
		"\u0001\u0013\u0001\u0014\u0001\u0014\u0001\u0014\u0001\u0014\u0001\u0014"+
		"\u0001\u0015\u0001\u0015\u0001\u0015\u0005\u0015\u0229\b\u0015\n\u0015"+
		"\f\u0015\u022c\t\u0015\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015"+
		"\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015\u0005\u0015"+
		"\u0237\b\u0015\n\u0015\f\u0015\u023a\t\u0015\u0001\u0015\u0001\u0015\u0001"+
		"\u0015\u0001\u0016\u0001\u0016\u0001\u0016\u0005\u0016\u0242\b\u0016\n"+
		"\u0016\f\u0016\u0245\t\u0016\u0001\u0016\u0001\u0016\u0001\u0016\u0005"+
		"\u0016\u024a\b\u0016\n\u0016\f\u0016\u024d\t\u0016\u0001\u0016\u0001\u0016"+
		"\u0001\u0016\u0001\u0016\u0001\u0016\u0001\u0016\u0001\u0016\u0001\u0016"+
		"\u0001\u0016\u0001\u0016\u0005\u0016\u0259\b\u0016\n\u0016\f\u0016\u025c"+
		"\t\u0016\u0001\u0016\u0001\u0016\u0001\u0016\u0001\u0017\u0001\u0017\u0001"+
		"\u0017\u0001\u0017\u0001\u0017\u0001\u0017\u0001\u0017\u0001\u0017\u0001"+
		"\u0017\u0001\u0017\u0005\u0017\u026b\b\u0017\n\u0017\f\u0017\u026e\t\u0017"+
		"\u0001\u0017\u0001\u0017\u0001\u0017\u0001\u0018\u0001\u0018\u0001\u0018"+
		"\u0001\u0018\u0001\u0018\u0001\u0018\u0001\u0018\u0001\u0018\u0001\u0018"+
		"\u0001\u0018\u0001\u0018\u0001\u0018\u0001\u0018\u0001\u0018\u0001\u0018"+
		"\u0001\u0018\u0001\u0018\u0001\u0018\u0001\u0018\u0001\u0018\u0001\u0018"+
		"\u0001\u0018\u0001\u0018\u0001\u0018\u0001\u0018\u0001\u0018\u0001\u0018"+
		"\u0001\u0018\u0003\u0018\u028f\b\u0018\u0001\u0019\u0001\u0019\u0001\u0019"+
		"\u0001\u001a\u0001\u001a\u0001\u001a\u0001\u001a\u0001\u001a\u0001\u001a"+
		"\u0001\u001a\u0001\u001a\u0001\u001a\u0001\u001a\u0001\u001a\u0001\u001a"+
		"\u0003\u001a\u02a0\b\u001a\u0001\u001a\u0001\u001a\u0001\u001b\u0001\u001b"+
		"\u0001\u001b\u0001\u001b\u0001\u001b\u0001\u001b\u0003\u001b\u02aa\b\u001b"+
		"\u0001\u001b\u0001\u001b\u0001\u001b\u0001\u001b\u0003\u001b\u02b0\b\u001b"+
		"\u0001\u001b\u0001\u001b\u0001\u001b\u0001\u001b\u0003\u001b\u02b6\b\u001b"+
		"\u0001\u001b\u0001\u001b\u0001\u001b\u0001\u001b\u0005\u001b\u02bc\b\u001b"+
		"\n\u001b\f\u001b\u02bf\t\u001b\u0001\u001b\u0001\u001b\u0001\u001b\u0001"+
		"\u001b\u0001\u001c\u0001\u001c\u0001\u001c\u0001\u001c\u0001\u001c\u0001"+
		"\u001c\u0001\u001c\u0001\u001c\u0001\u001c\u0001\u001c\u0001\u001c\u0001"+
		"\u001c\u0001\u001c\u0001\u001d\u0001\u001d\u0001\u001d\u0001\u001d\u0001"+
		"\u001d\u0001\u001d\u0001\u001d\u0001\u001d\u0001\u001d\u0001\u001e\u0001"+
		"\u001e\u0001\u001e\u0001\u001e\u0001\u001e\u0001\u001e\u0001\u001e\u0001"+
		"\u001e\u0001\u001e\u0001\u001e\u0001\u001e\u0001\u001e\u0001\u001e\u0001"+
		"\u001e\u0001\u001e\u0001\u001e\u0001\u001e\u0001\u001e\u0001\u001e\u0001"+
		"\u001e\u0001\u001e\u0001\u001e\u0003\u001e\u02f1\b\u001e\u0001\u001f\u0001"+
		"\u001f\u0001\u001f\u0001\u001f\u0001\u001f\u0001\u001f\u0001\u001f\u0001"+
		"\u001f\u0001\u001f\u0001\u001f\u0001\u001f\u0001\u001f\u0001\u001f\u0001"+
		"\u001f\u0005\u001f\u0301\b\u001f\n\u001f\f\u001f\u0304\t\u001f\u0001\u001f"+
		"\u0001\u001f\u0001\u001f\u0001\u001f\u0001 \u0001 \u0001 \u0001 \u0001"+
		" \u0003 \u030f\b \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001"+
		" \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0003 \u031f\b \u0001!\u0001"+
		"!\u0001!\u0001!\u0001!\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\""+
		"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0003"+
		"\"\u0334\b\"\u0005\"\u0336\b\"\n\"\f\"\u0339\t\"\u0001\"\u0001\"\u0001"+
		"\"\u0001\"\u0001\"\u0003\"\u0340\b\"\u0003\"\u0342\b\"\u0001\"\u0001\""+
		"\u0001\"\u0001#\u0001#\u0001#\u0001#\u0001#\u0001$\u0003$\u034d\b$\u0001"+
		"$\u0001$\u0001$\u0001$\u0001$\u0001$\u0001$\u0005$\u0356\b$\n$\f$\u0359"+
		"\t$\u0001$\u0001$\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001"+
		"%\u0003%\u0365\b%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001"+
		"%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001"+
		"%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001"+
		"%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001"+
		"%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001"+
		"%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001"+
		"%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001"+
		"%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001"+
		"%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001"+
		"%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001"+
		"%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0001"+
		"%\u0001%\u0001%\u0001%\u0003%\u03d6\b%\u0001%\u0001%\u0001%\u0001%\u0001"+
		"%\u0001%\u0001%\u0003%\u03df\b%\u0001%\u0001%\u0005%\u03e3\b%\n%\f%\u03e6"+
		"\t%\u0001&\u0001&\u0001&\u0001&\u0001&\u0001&\u0001&\u0001&\u0001&\u0001"+
		"&\u0001&\u0001&\u0001&\u0001&\u0001&\u0001&\u0001&\u0001&\u0001&\u0001"+
		"&\u0001&\u0001&\u0001&\u0001&\u0001&\u0001&\u0001&\u0001&\u0001&\u0001"+
		"&\u0001&\u0001&\u0001&\u0001&\u0001&\u0001&\u0003&\u040c\b&\u0001\'\u0001"+
		"\'\u0001\'\u0001\'\u0001\'\u0001\'\u0001\'\u0003\'\u0415\b\'\u0001\'\u0001"+
		"\'\u0001\'\u0001(\u0001(\u0001(\u0001(\u0001(\u0001(\u0001)\u0001)\u0001"+
		")\u0001)\u0001)\u0001)\u0003)\u0426\b)\u0001)\u0001)\u0001)\u0004)\u042b"+
		"\b)\u000b)\f)\u042c\u0001)\u0001)\u0001)\u0001*\u0001*\u0001*\u0001*\u0001"+
		"*\u0001*\u0003*\u0438\b*\u0001*\u0001*\u0001*\u0004*\u043d\b*\u000b*\f"+
		"*\u043e\u0001*\u0001*\u0001*\u0001+\u0001+\u0001+\u0001+\u0001+\u0001"+
		"+\u0003+\u044a\b+\u0001+\u0001+\u0001+\u0004+\u044f\b+\u000b+\f+\u0450"+
		"\u0001+\u0001+\u0001+\u0001,\u0001,\u0001,\u0001,\u0001,\u0001,\u0003"+
		",\u045c\b,\u0001,\u0001,\u0001,\u0004,\u0461\b,\u000b,\f,\u0462\u0001"+
		",\u0001,\u0001,\u0001-\u0001-\u0001-\u0001-\u0001-\u0001-\u0003-\u046e"+
		"\b-\u0001-\u0001-\u0001-\u0001-\u0004-\u0474\b-\u000b-\f-\u0475\u0001"+
		"-\u0001-\u0001-\u0001-\u0001\u00b8\u0001J.\u0000\u0002\u0004\u0006\b\n"+
		"\f\u000e\u0010\u0012\u0014\u0016\u0018\u001a\u001c\u001e \"$&(*,.0246"+
		"8:<>@BDFHJLNPRTVXZ\u0000\u000b\u0001\u000044\u0001\u0000==\u0001\u0000"+
		"BB\u0006\u0000\u001c\u001d2266<<KLNN\u0002\u0000HHMN\u0001\u0000KL\u0001"+
		"\u0000\u001e\u001f\u0001\u0000@C\u0001\u0000DE\u0005\u0000 &337799;;\u0001"+
		"\u0000\u001c\u001d\u04e0\u0000n\u0001\u0000\u0000\u0000\u0002\u0082\u0001"+
		"\u0000\u0000\u0000\u0004\u0088\u0001\u0000\u0000\u0000\u0006\u008a\u0001"+
		"\u0000\u0000\u0000\b\u00b8\u0001\u0000\u0000\u0000\n\u00bb\u0001\u0000"+
		"\u0000\u0000\f\u00c1\u0001\u0000\u0000\u0000\u000e\u00c9\u0001\u0000\u0000"+
		"\u0000\u0010\u00db\u0001\u0000\u0000\u0000\u0012\u00dd\u0001\u0000\u0000"+
		"\u0000\u0014\u00f3\u0001\u0000\u0000\u0000\u0016\u0116\u0001\u0000\u0000"+
		"\u0000\u0018\u0118\u0001\u0000\u0000\u0000\u001a\u0132\u0001\u0000\u0000"+
		"\u0000\u001c\u0145\u0001\u0000\u0000\u0000\u001e\u017f\u0001\u0000\u0000"+
		"\u0000 \u0196\u0001\u0000\u0000\u0000\"\u01b9\u0001\u0000\u0000\u0000"+
		"$\u01e8\u0001\u0000\u0000\u0000&\u0205\u0001\u0000\u0000\u0000(\u0220"+
		"\u0001\u0000\u0000\u0000*\u022a\u0001\u0000\u0000\u0000,\u0243\u0001\u0000"+
		"\u0000\u0000.\u0260\u0001\u0000\u0000\u00000\u028e\u0001\u0000\u0000\u0000"+
		"2\u0290\u0001\u0000\u0000\u00004\u0293\u0001\u0000\u0000\u00006\u02a3"+
		"\u0001\u0000\u0000\u00008\u02c4\u0001\u0000\u0000\u0000:\u02d1\u0001\u0000"+
		"\u0000\u0000<\u02f0\u0001\u0000\u0000\u0000>\u02f2\u0001\u0000\u0000\u0000"+
		"@\u031e\u0001\u0000\u0000\u0000B\u0320\u0001\u0000\u0000\u0000D\u0325"+
		"\u0001\u0000\u0000\u0000F\u0346\u0001\u0000\u0000\u0000H\u034c\u0001\u0000"+
		"\u0000\u0000J\u0364\u0001\u0000\u0000\u0000L\u040b\u0001\u0000\u0000\u0000"+
		"N\u040d\u0001\u0000\u0000\u0000P\u0419\u0001\u0000\u0000\u0000R\u041f"+
		"\u0001\u0000\u0000\u0000T\u0431\u0001\u0000\u0000\u0000V\u0443\u0001\u0000"+
		"\u0000\u0000X\u0455\u0001\u0000\u0000\u0000Z\u0467\u0001\u0000\u0000\u0000"+
		"\\a\u00054\u0000\u0000]^\b\u0000\u0000\u0000^`\u0006\u0000\uffff\uffff"+
		"\u0000_]\u0001\u0000\u0000\u0000`c\u0001\u0000\u0000\u0000a_\u0001\u0000"+
		"\u0000\u0000ab\u0001\u0000\u0000\u0000bd\u0001\u0000\u0000\u0000ca\u0001"+
		"\u0000\u0000\u0000do\u00054\u0000\u0000ej\u0005=\u0000\u0000fg\b\u0001"+
		"\u0000\u0000gi\u0006\u0000\uffff\uffff\u0000hf\u0001\u0000\u0000\u0000"+
		"il\u0001\u0000\u0000\u0000jh\u0001\u0000\u0000\u0000jk\u0001\u0000\u0000"+
		"\u0000km\u0001\u0000\u0000\u0000lj\u0001\u0000\u0000\u0000mo\u0005=\u0000"+
		"\u0000n\\\u0001\u0000\u0000\u0000ne\u0001\u0000\u0000\u0000o\u0001\u0001"+
		"\u0000\u0000\u0000pu\u00054\u0000\u0000qr\b\u0000\u0000\u0000rt\u0006"+
		"\u0001\uffff\uffff\u0000sq\u0001\u0000\u0000\u0000tw\u0001\u0000\u0000"+
		"\u0000us\u0001\u0000\u0000\u0000uv\u0001\u0000\u0000\u0000vx\u0001\u0000"+
		"\u0000\u0000wu\u0001\u0000\u0000\u0000x\u0083\u00054\u0000\u0000y~\u0005"+
		"@\u0000\u0000z{\b\u0002\u0000\u0000{}\u0006\u0001\uffff\uffff\u0000|z"+
		"\u0001\u0000\u0000\u0000}\u0080\u0001\u0000\u0000\u0000~|\u0001\u0000"+
		"\u0000\u0000~\u007f\u0001\u0000\u0000\u0000\u007f\u0081\u0001\u0000\u0000"+
		"\u0000\u0080~\u0001\u0000\u0000\u0000\u0081\u0083\u0005B\u0000\u0000\u0082"+
		"p\u0001\u0000\u0000\u0000\u0082y\u0001\u0000\u0000\u0000\u0083\u0003\u0001"+
		"\u0000\u0000\u0000\u0084\u0085\u0005\u0001\u0000\u0000\u0085\u0089\u0006"+
		"\u0002\uffff\uffff\u0000\u0086\u0087\u0005\u0002\u0000\u0000\u0087\u0089"+
		"\u0006\u0002\uffff\uffff\u0000\u0088\u0084\u0001\u0000\u0000\u0000\u0088"+
		"\u0086\u0001\u0000\u0000\u0000\u0089\u0005\u0001\u0000\u0000\u0000\u008a"+
		"\u008b\u0003\b\u0004\u0000\u008b\u008c\u0006\u0003\uffff\uffff\u0000\u008c"+
		"\u008d\u0005\u0000\u0000\u0001\u008d\u0007\u0001\u0000\u0000\u0000\u008e"+
		"\u00b7\u0003\n\u0005\u0000\u008f\u0090\u0003\u0014\n\u0000\u0090\u0091"+
		"\u0005(\u0000\u0000\u0091\u0092\u0006\u0004\uffff\uffff\u0000\u0092\u00b7"+
		"\u0001\u0000\u0000\u0000\u0093\u0094\u0003\f\u0006\u0000\u0094\u0095\u0005"+
		"(\u0000\u0000\u0095\u0096\u0006\u0004\uffff\uffff\u0000\u0096\u00b7\u0001"+
		"\u0000\u0000\u0000\u0097\u0098\u0003&\u0013\u0000\u0098\u0099\u0005(\u0000"+
		"\u0000\u0099\u009a\u0006\u0004\uffff\uffff\u0000\u009a\u00b7\u0001\u0000"+
		"\u0000\u0000\u009b\u009c\u0003(\u0014\u0000\u009c\u009d\u0006\u0004\uffff"+
		"\uffff\u0000\u009d\u00b7\u0001\u0000\u0000\u0000\u009e\u009f\u0003\u001c"+
		"\u000e\u0000\u009f\u00a0\u0005(\u0000\u0000\u00a0\u00a1\u0006\u0004\uffff"+
		"\uffff\u0000\u00a1\u00b7\u0001\u0000\u0000\u0000\u00a2\u00a3\u0003 \u0010"+
		"\u0000\u00a3\u00a4\u0005(\u0000\u0000\u00a4\u00a5\u0006\u0004\uffff\uffff"+
		"\u0000\u00a5\u00b7\u0001\u0000\u0000\u0000\u00a6\u00a7\u0003\"\u0011\u0000"+
		"\u00a7\u00a8\u0005(\u0000\u0000\u00a8\u00a9\u0006\u0004\uffff\uffff\u0000"+
		"\u00a9\u00b7\u0001\u0000\u0000\u0000\u00aa\u00ab\u0003.\u0017\u0000\u00ab"+
		"\u00ac\u0005(\u0000\u0000\u00ac\u00ad\u0006\u0004\uffff\uffff\u0000\u00ad"+
		"\u00b7\u0001\u0000\u0000\u0000\u00ae\u00af\u0003,\u0016\u0000\u00af\u00b0"+
		"\u0005(\u0000\u0000\u00b0\u00b1\u0006\u0004\uffff\uffff\u0000\u00b1\u00b7"+
		"\u0001\u0000\u0000\u0000\u00b2\u00b3\u0003*\u0015\u0000\u00b3\u00b4\u0005"+
		"(\u0000\u0000\u00b4\u00b5\u0006\u0004\uffff\uffff\u0000\u00b5\u00b7\u0001"+
		"\u0000\u0000\u0000\u00b6\u008e\u0001\u0000\u0000\u0000\u00b6\u008f\u0001"+
		"\u0000\u0000\u0000\u00b6\u0093\u0001\u0000\u0000\u0000\u00b6\u0097\u0001"+
		"\u0000\u0000\u0000\u00b6\u009b\u0001\u0000\u0000\u0000\u00b6\u009e\u0001"+
		"\u0000\u0000\u0000\u00b6\u00a2\u0001\u0000\u0000\u0000\u00b6\u00a6\u0001"+
		"\u0000\u0000\u0000\u00b6\u00aa\u0001\u0000\u0000\u0000\u00b6\u00ae\u0001"+
		"\u0000\u0000\u0000\u00b6\u00b2\u0001\u0000\u0000\u0000\u00b7\u00ba\u0001"+
		"\u0000\u0000\u0000\u00b8\u00b9\u0001\u0000\u0000\u0000\u00b8\u00b6\u0001"+
		"\u0000\u0000\u0000\u00b9\t\u0001\u0000\u0000\u0000\u00ba\u00b8\u0001\u0000"+
		"\u0000\u0000\u00bb\u00bc\u0005\u0003\u0000\u0000\u00bc\u00bd\u0006\u0005"+
		"\uffff\uffff\u0000\u00bd\u00be\u0005P\u0000\u0000\u00be\u00bf\u0003\u0000"+
		"\u0000\u0000\u00bf\u00c0\u0006\u0005\uffff\uffff\u0000\u00c0\u000b\u0001"+
		"\u0000\u0000\u0000\u00c1\u00c2\u0005\u0004\u0000\u0000\u00c2\u00c3\u0005"+
		"W\u0000\u0000\u00c3\u00c4\u0006\u0006\uffff\uffff\u0000\u00c4\u00c5\u0005"+
		"\u0005\u0000\u0000\u00c5\u00c6\u0005W\u0000\u0000\u00c6\u00c7\u0006\u0006"+
		"\uffff\uffff\u0000\u00c7\u00c8\u0006\u0006\uffff\uffff\u0000\u00c8\r\u0001"+
		"\u0000\u0000\u0000\u00c9\u00ca\u0005J\u0000\u0000\u00ca\u00cb\u0006\u0007"+
		"\uffff\uffff\u0000\u00cb\u00cc\u0005W\u0000\u0000\u00cc\u00cd\u0006\u0007"+
		"\uffff\uffff\u0000\u00cd\u00ce\u0005+\u0000\u0000\u00ce\u00cf\u0003J%"+
		"\u0000\u00cf\u00d0\u0005,\u0000\u0000\u00d0\u00d1\u0006\u0007\uffff\uffff"+
		"\u0000\u00d1\u000f\u0001\u0000\u0000\u0000\u00d2\u00d3\u0005W\u0000\u0000"+
		"\u00d3\u00d4\u0006\b\uffff\uffff\u0000\u00d4\u00d5\u0005+\u0000\u0000"+
		"\u00d5\u00d6\u0003J%\u0000\u00d6\u00d7\u0005,\u0000\u0000\u00d7\u00d8"+
		"\u0006\b\uffff\uffff\u0000\u00d8\u00dc\u0001\u0000\u0000\u0000\u00d9\u00da"+
		"\u0005W\u0000\u0000\u00da\u00dc\u0006\b\uffff\uffff\u0000\u00db\u00d2"+
		"\u0001\u0000\u0000\u0000\u00db\u00d9\u0001\u0000\u0000\u0000\u00dc\u0011"+
		"\u0001\u0000\u0000\u0000\u00dd\u00ed\u0006\t\uffff\uffff\u0000\u00de\u00df"+
		"\u0005N\u0000\u0000\u00df\u00ec\u0006\t\uffff\uffff\u0000\u00e0\u00e1"+
		"\u0005/\u0000\u0000\u00e1\u00e5\u0006\t\uffff\uffff\u0000\u00e2\u00e3"+
		"\u0003J%\u0000\u00e3\u00e4\u0006\t\uffff\uffff\u0000\u00e4\u00e6\u0001"+
		"\u0000\u0000\u0000\u00e5\u00e2\u0001\u0000\u0000\u0000\u00e5\u00e6\u0001"+
		"\u0000\u0000\u0000\u00e6\u00e7\u0001\u0000\u0000\u0000\u00e7\u00ec\u0005"+
		"0\u0000\u0000\u00e8\u00e9\u0005W\u0000\u0000\u00e9\u00ec\u0006\t\uffff"+
		"\uffff\u0000\u00ea\u00ec\u0003\n\u0005\u0000\u00eb\u00de\u0001\u0000\u0000"+
		"\u0000\u00eb\u00e0\u0001\u0000\u0000\u0000\u00eb\u00e8\u0001\u0000\u0000"+
		"\u0000\u00eb\u00ea\u0001\u0000\u0000\u0000\u00ec\u00ef\u0001\u0000\u0000"+
		"\u0000\u00ed\u00eb\u0001\u0000\u0000\u0000\u00ed\u00ee\u0001\u0000\u0000"+
		"\u0000\u00ee\u00f0\u0001\u0000\u0000\u0000\u00ef\u00ed\u0001\u0000\u0000"+
		"\u0000\u00f0\u00f1\u0005W\u0000\u0000\u00f1\u00f2\u0006\t\uffff\uffff"+
		"\u0000\u00f2\u0013\u0001\u0000\u0000\u0000\u00f3\u00f4\u0005\u0006\u0000"+
		"\u0000\u00f4\u00f5\u0006\n\uffff\uffff\u0000\u00f5\u010a\u0005@\u0000"+
		"\u0000\u00f6\u00f7\u0003\u001c\u000e\u0000\u00f7\u00f8\u0005(\u0000\u0000"+
		"\u00f8\u00f9\u0006\n\uffff\uffff\u0000\u00f9\u0109\u0001\u0000\u0000\u0000"+
		"\u00fa\u00fb\u0003\u0016\u000b\u0000\u00fb\u00fc\u0006\n\uffff\uffff\u0000"+
		"\u00fc\u0109\u0001\u0000\u0000\u0000\u00fd\u00fe\u0003\f\u0006\u0000\u00fe"+
		"\u00ff\u0005(\u0000\u0000\u00ff\u0100\u0006\n\uffff\uffff\u0000\u0100"+
		"\u0109\u0001\u0000\u0000\u0000\u0101\u0102\u0003&\u0013\u0000\u0102\u0103"+
		"\u0005(\u0000\u0000\u0103\u0104\u0006\n\uffff\uffff\u0000\u0104\u0109"+
		"\u0001\u0000\u0000\u0000\u0105\u0106\u0003(\u0014\u0000\u0106\u0107\u0006"+
		"\n\uffff\uffff\u0000\u0107\u0109\u0001\u0000\u0000\u0000\u0108\u00f6\u0001"+
		"\u0000\u0000\u0000\u0108\u00fa\u0001\u0000\u0000\u0000\u0108\u00fd\u0001"+
		"\u0000\u0000\u0000\u0108\u0101\u0001\u0000\u0000\u0000\u0108\u0105\u0001"+
		"\u0000\u0000\u0000\u0109\u010c\u0001\u0000\u0000\u0000\u010a\u0108\u0001"+
		"\u0000\u0000\u0000\u010a\u010b\u0001\u0000\u0000\u0000\u010b\u010d\u0001"+
		"\u0000\u0000\u0000\u010c\u010a\u0001\u0000\u0000\u0000\u010d\u010e\u0005"+
		"B\u0000\u0000\u010e\u010f\u0006\n\uffff\uffff\u0000\u010f\u0015\u0001"+
		"\u0000\u0000\u0000\u0110\u0111\u0003\u001a\r\u0000\u0111\u0112\u0006\u000b"+
		"\uffff\uffff\u0000\u0112\u0117\u0001\u0000\u0000\u0000\u0113\u0114\u0003"+
		"\u0018\f\u0000\u0114\u0115\u0006\u000b\uffff\uffff\u0000\u0115\u0117\u0001"+
		"\u0000\u0000\u0000\u0116\u0110\u0001\u0000\u0000\u0000\u0116\u0113\u0001"+
		"\u0000\u0000\u0000\u0117\u0017\u0001\u0000\u0000\u0000\u0118\u0119\u0005"+
		"@\u0000\u0000\u0119\u012b\u0006\f\uffff\uffff\u0000\u011a\u011b\u0003"+
		"\u001c\u000e\u0000\u011b\u011c\u0005(\u0000\u0000\u011c\u011d\u0006\f"+
		"\uffff\uffff\u0000\u011d\u012a\u0001\u0000\u0000\u0000\u011e\u011f\u0003"+
		"\u0016\u000b\u0000\u011f\u0120\u0006\f\uffff\uffff\u0000\u0120\u012a\u0001"+
		"\u0000\u0000\u0000\u0121\u0122\u0003\f\u0006\u0000\u0122\u0123\u0005("+
		"\u0000\u0000\u0123\u0124\u0006\f\uffff\uffff\u0000\u0124\u012a\u0001\u0000"+
		"\u0000\u0000\u0125\u0126\u0003(\u0014\u0000\u0126\u0127\u0006\f\uffff"+
		"\uffff\u0000\u0127\u012a\u0001\u0000\u0000\u0000\u0128\u012a\u0003\n\u0005"+
		"\u0000\u0129\u011a\u0001\u0000\u0000\u0000\u0129\u011e\u0001\u0000\u0000"+
		"\u0000\u0129\u0121\u0001\u0000\u0000\u0000\u0129\u0125\u0001\u0000\u0000"+
		"\u0000\u0129\u0128\u0001\u0000\u0000\u0000\u012a\u012d\u0001\u0000\u0000"+
		"\u0000\u012b\u0129\u0001\u0000\u0000\u0000\u012b\u012c\u0001\u0000\u0000"+
		"\u0000\u012c\u012e\u0001\u0000\u0000\u0000\u012d\u012b\u0001\u0000\u0000"+
		"\u0000\u012e\u012f\u0005B\u0000\u0000\u012f\u0130\u0006\f\uffff\uffff"+
		"\u0000\u0130\u0131\u0006\f\uffff\uffff\u0000\u0131\u0019\u0001\u0000\u0000"+
		"\u0000\u0132\u0133\u0005\u0007\u0000\u0000\u0133\u0134\u0006\r\uffff\uffff"+
		"\u0000\u0134\u0135\u0005+\u0000\u0000\u0135\u0136\u0003J%\u0000\u0136"+
		"\u0137\u0006\r\uffff\uffff\u0000\u0137\u0138\u0005,\u0000\u0000\u0138"+
		"\u0139\u0003\u0016\u000b\u0000\u0139\u013e\u0006\r\uffff\uffff\u0000\u013a"+
		"\u013b\u0005\b\u0000\u0000\u013b\u013c\u0003\u0016\u000b\u0000\u013c\u013d"+
		"\u0006\r\uffff\uffff\u0000\u013d\u013f\u0001\u0000\u0000\u0000\u013e\u013a"+
		"\u0001\u0000\u0000\u0000\u013e\u013f\u0001\u0000\u0000\u0000\u013f\u0140"+
		"\u0001\u0000\u0000\u0000\u0140\u0141\u0006\r\uffff\uffff\u0000\u0141\u001b"+
		"\u0001\u0000\u0000\u0000\u0142\u0144\u0003\n\u0005\u0000\u0143\u0142\u0001"+
		"\u0000\u0000\u0000\u0144\u0147\u0001\u0000\u0000\u0000\u0145\u0143\u0001"+
		"\u0000\u0000\u0000\u0145\u0146\u0001\u0000\u0000\u0000\u0146\u014d\u0001"+
		"\u0000\u0000\u0000\u0147\u0145\u0001\u0000\u0000\u0000\u0148\u0149\u0003"+
		"\u000e\u0007\u0000\u0149\u014a\u0006\u000e\uffff\uffff\u0000\u014a\u014c"+
		"\u0001\u0000\u0000\u0000\u014b\u0148\u0001\u0000\u0000\u0000\u014c\u014f"+
		"\u0001\u0000\u0000\u0000\u014d\u014b\u0001\u0000\u0000\u0000\u014d\u014e"+
		"\u0001\u0000\u0000\u0000\u014e\u0153\u0001\u0000\u0000\u0000\u014f\u014d"+
		"\u0001\u0000\u0000\u0000\u0150\u0151\u0003\u0010\b\u0000\u0151\u0152\u0006"+
		"\u000e\uffff\uffff\u0000\u0152\u0154\u0001\u0000\u0000\u0000\u0153\u0150"+
		"\u0001\u0000\u0000\u0000\u0154\u0155\u0001\u0000\u0000\u0000\u0155\u0153"+
		"\u0001\u0000\u0000\u0000\u0155\u0156\u0001\u0000\u0000\u0000\u0156\u0157"+
		"\u0001\u0000\u0000\u0000\u0157\u0158\u0005W\u0000\u0000\u0158\u015a\u0006"+
		"\u000e\uffff\uffff\u0000\u0159\u015b\u0003\n\u0005\u0000\u015a\u0159\u0001"+
		"\u0000\u0000\u0000\u015a\u015b\u0001\u0000\u0000\u0000\u015b\u0162\u0001"+
		"\u0000\u0000\u0000\u015c\u015d\u0005)\u0000\u0000\u015d\u015e\u0005W\u0000"+
		"\u0000\u015e\u0161\u0006\u000e\uffff\uffff\u0000\u015f\u0161\u0003\n\u0005"+
		"\u0000\u0160\u015c\u0001\u0000\u0000\u0000\u0160\u015f\u0001\u0000\u0000"+
		"\u0000\u0161\u0164\u0001\u0000\u0000\u0000\u0162\u0160\u0001\u0000\u0000"+
		"\u0000\u0162\u0163\u0001\u0000\u0000\u0000\u0163\u0169\u0001\u0000\u0000"+
		"\u0000\u0164\u0162\u0001\u0000\u0000\u0000\u0165\u0166\u0005*\u0000\u0000"+
		"\u0166\u0167\u0003\u0012\t\u0000\u0167\u0168\u0006\u000e\uffff\uffff\u0000"+
		"\u0168\u016a\u0001\u0000\u0000\u0000\u0169\u0165\u0001\u0000\u0000\u0000"+
		"\u0169\u016a\u0001\u0000\u0000\u0000\u016a\u0178\u0001\u0000\u0000\u0000"+
		"\u016b\u016c\u00053\u0000\u0000\u016c\u016d\u0003J%\u0000\u016d\u0175"+
		"\u0006\u000e\uffff\uffff\u0000\u016e\u016f\u0005)\u0000\u0000\u016f\u0170"+
		"\u0003J%\u0000\u0170\u0171\u0006\u000e\uffff\uffff\u0000\u0171\u0174\u0001"+
		"\u0000\u0000\u0000\u0172\u0174\u0003\n\u0005\u0000\u0173\u016e\u0001\u0000"+
		"\u0000\u0000\u0173\u0172\u0001\u0000\u0000\u0000\u0174\u0177\u0001\u0000"+
		"\u0000\u0000\u0175\u0173\u0001\u0000\u0000\u0000\u0175\u0176\u0001\u0000"+
		"\u0000\u0000\u0176\u0179\u0001\u0000\u0000\u0000\u0177\u0175\u0001\u0000"+
		"\u0000\u0000\u0178\u016b\u0001\u0000\u0000\u0000\u0178\u0179\u0001\u0000"+
		"\u0000\u0000\u0179\u017a\u0001\u0000\u0000\u0000\u017a\u017b\u0006\u000e"+
		"\uffff\uffff\u0000\u017b\u001d\u0001\u0000\u0000\u0000\u017c\u017e\u0003"+
		"\n\u0005\u0000\u017d\u017c\u0001\u0000\u0000\u0000\u017e\u0181\u0001\u0000"+
		"\u0000\u0000\u017f\u017d\u0001\u0000\u0000\u0000\u017f\u0180\u0001\u0000"+
		"\u0000\u0000\u0180\u0187\u0001\u0000\u0000\u0000\u0181\u017f\u0001\u0000"+
		"\u0000\u0000\u0182\u0183\u0003\u000e\u0007\u0000\u0183\u0184\u0006\u000f"+
		"\uffff\uffff\u0000\u0184\u0186\u0001\u0000\u0000\u0000\u0185\u0182\u0001"+
		"\u0000\u0000\u0000\u0186\u0189\u0001\u0000\u0000\u0000\u0187\u0185\u0001"+
		"\u0000\u0000\u0000\u0187\u0188\u0001\u0000\u0000\u0000\u0188\u018f\u0001"+
		"\u0000\u0000\u0000\u0189\u0187\u0001\u0000\u0000\u0000\u018a\u018b\u0003"+
		"\u0010\b\u0000\u018b\u018c\u0006\u000f\uffff\uffff\u0000\u018c\u018e\u0001"+
		"\u0000\u0000\u0000\u018d\u018a\u0001\u0000\u0000\u0000\u018e\u0191\u0001"+
		"\u0000\u0000\u0000\u018f\u018d\u0001\u0000\u0000\u0000\u018f\u0190\u0001"+
		"\u0000\u0000\u0000\u0190\u0192\u0001\u0000\u0000\u0000\u0191\u018f\u0001"+
		"\u0000\u0000\u0000\u0192\u0193\u0005\t\u0000\u0000\u0193\u0194\u0005W"+
		"\u0000\u0000\u0194\u0195\u0006\u000f\uffff\uffff\u0000\u0195\u001f\u0001"+
		"\u0000\u0000\u0000\u0196\u0197\u0003\u001e\u000f\u0000\u0197\u0198\u0006"+
		"\u0010\uffff\uffff\u0000\u0198\u01b3\u0005-\u0000\u0000\u0199\u019a\u0005"+
		"W\u0000\u0000\u019a\u019b\u0006\u0010\uffff\uffff\u0000\u019b\u019c\u0005"+
		"*\u0000\u0000\u019c\u01a9\u0006\u0010\uffff\uffff\u0000\u019d\u019e\u0005"+
		"N\u0000\u0000\u019e\u01a8\u0006\u0010\uffff\uffff\u0000\u019f\u01a0\u0005"+
		"/\u0000\u0000\u01a0\u01a4\u0006\u0010\uffff\uffff\u0000\u01a1\u01a2\u0003"+
		"J%\u0000\u01a2\u01a3\u0006\u0010\uffff\uffff\u0000\u01a3\u01a5\u0001\u0000"+
		"\u0000\u0000\u01a4\u01a1\u0001\u0000\u0000\u0000\u01a4\u01a5\u0001\u0000"+
		"\u0000\u0000\u01a5\u01a6\u0001\u0000\u0000\u0000\u01a6\u01a8\u00050\u0000"+
		"\u0000\u01a7\u019d\u0001\u0000\u0000\u0000\u01a7\u019f\u0001\u0000\u0000"+
		"\u0000\u01a8\u01ab\u0001\u0000\u0000\u0000\u01a9\u01a7\u0001\u0000\u0000"+
		"\u0000\u01a9\u01aa\u0001\u0000\u0000\u0000\u01aa\u01ac\u0001\u0000\u0000"+
		"\u0000\u01ab\u01a9\u0001\u0000\u0000\u0000\u01ac\u01ad\u0005W\u0000\u0000"+
		"\u01ad\u01ae\u0006\u0010\uffff\uffff\u0000\u01ae\u01af\u0005(\u0000\u0000"+
		"\u01af\u01b2\u0006\u0010\uffff\uffff\u0000\u01b0\u01b2\u0003\n\u0005\u0000"+
		"\u01b1\u0199\u0001\u0000\u0000\u0000\u01b1\u01b0\u0001\u0000\u0000\u0000"+
		"\u01b2\u01b5\u0001\u0000\u0000\u0000\u01b3\u01b1\u0001\u0000\u0000\u0000"+
		"\u01b3\u01b4\u0001\u0000\u0000\u0000\u01b4\u01b6\u0001\u0000\u0000\u0000"+
		"\u01b5\u01b3\u0001\u0000\u0000\u0000\u01b6\u01b7\u0005.\u0000\u0000\u01b7"+
		"\u01b8\u0006\u0010\uffff\uffff\u0000\u01b8!\u0001\u0000\u0000\u0000\u01b9"+
		"\u01ba\u0005\n\u0000\u0000\u01ba\u01bb\u0005W\u0000\u0000\u01bb\u01c0"+
		"\u0006\u0011\uffff\uffff\u0000\u01bc\u01bd\u0005*\u0000\u0000\u01bd\u01be"+
		"\u0003\u0012\t\u0000\u01be\u01bf\u0006\u0011\uffff\uffff\u0000\u01bf\u01c1"+
		"\u0001\u0000\u0000\u0000\u01c0\u01bc\u0001\u0000\u0000\u0000\u01c0\u01c1"+
		"\u0001\u0000\u0000\u0000\u01c1\u01c2\u0001\u0000\u0000\u0000\u01c2\u01e0"+
		"\u0005-\u0000\u0000\u01c3\u01c4\u0005W\u0000\u0000\u01c4\u01c9\u0006\u0011"+
		"\uffff\uffff\u0000\u01c5\u01c6\u00053\u0000\u0000\u01c6\u01c7\u0003J%"+
		"\u0000\u01c7\u01c8\u0006\u0011\uffff\uffff\u0000\u01c8\u01ca\u0001\u0000"+
		"\u0000\u0000\u01c9\u01c5\u0001\u0000\u0000\u0000\u01c9\u01ca\u0001\u0000"+
		"\u0000\u0000\u01ca\u01cb\u0001\u0000\u0000\u0000\u01cb\u01cd\u0006\u0011"+
		"\uffff\uffff\u0000\u01cc\u01ce\u0003\n\u0005\u0000\u01cd\u01cc\u0001\u0000"+
		"\u0000\u0000\u01cd\u01ce\u0001\u0000\u0000\u0000\u01ce\u01dc\u0001\u0000"+
		"\u0000\u0000\u01cf\u01d0\u0005)\u0000\u0000\u01d0\u01d1\u0005W\u0000\u0000"+
		"\u01d1\u01d6\u0006\u0011\uffff\uffff\u0000\u01d2\u01d3\u00053\u0000\u0000"+
		"\u01d3\u01d4\u0003J%\u0000\u01d4\u01d5\u0006\u0011\uffff\uffff\u0000\u01d5"+
		"\u01d7\u0001\u0000\u0000\u0000\u01d6\u01d2\u0001\u0000\u0000\u0000\u01d6"+
		"\u01d7\u0001\u0000\u0000\u0000\u01d7\u01d8\u0001\u0000\u0000\u0000\u01d8"+
		"\u01db\u0006\u0011\uffff\uffff\u0000\u01d9\u01db\u0003\n\u0005\u0000\u01da"+
		"\u01cf\u0001\u0000\u0000\u0000\u01da\u01d9\u0001\u0000\u0000\u0000\u01db"+
		"\u01de\u0001\u0000\u0000\u0000\u01dc\u01da\u0001\u0000\u0000\u0000\u01dc"+
		"\u01dd\u0001\u0000\u0000\u0000\u01dd\u01e1\u0001\u0000\u0000\u0000\u01de"+
		"\u01dc\u0001\u0000\u0000\u0000\u01df\u01e1\u0003\n\u0005\u0000\u01e0\u01c3"+
		"\u0001\u0000\u0000\u0000\u01e0\u01df\u0001\u0000\u0000\u0000\u01e0\u01e1"+
		"\u0001\u0000\u0000\u0000\u01e1\u01e2\u0001\u0000\u0000\u0000\u01e2\u01e3"+
		"\u0005.\u0000\u0000\u01e3\u01e4\u0006\u0011\uffff\uffff\u0000\u01e4#\u0001"+
		"\u0000\u0000\u0000\u01e5\u01e7\u0003\n\u0005\u0000\u01e6\u01e5\u0001\u0000"+
		"\u0000\u0000\u01e7\u01ea\u0001\u0000\u0000\u0000\u01e8\u01e6\u0001\u0000"+
		"\u0000\u0000\u01e8\u01e9\u0001\u0000\u0000\u0000\u01e9\u01f0\u0001\u0000"+
		"\u0000\u0000\u01ea\u01e8\u0001\u0000\u0000\u0000\u01eb\u01ec\u0003\u0010"+
		"\b\u0000\u01ec\u01ed\u0006\u0012\uffff\uffff\u0000\u01ed\u01ef\u0001\u0000"+
		"\u0000\u0000\u01ee\u01eb\u0001\u0000\u0000\u0000\u01ef\u01f2\u0001\u0000"+
		"\u0000\u0000\u01f0\u01ee\u0001\u0000\u0000\u0000\u01f0\u01f1\u0001\u0000"+
		"\u0000\u0000\u01f1\u01f3\u0001\u0000\u0000\u0000\u01f2\u01f0\u0001\u0000"+
		"\u0000\u0000\u01f3\u01f4\u0005W\u0000\u0000\u01f4\u01f5\u0006\u0012\uffff"+
		"\uffff\u0000\u01f5\u01f6\u0005*\u0000\u0000\u01f6\u01f7\u0003\u0012\t"+
		"\u0000\u01f7\u01fc\u0006\u0012\uffff\uffff\u0000\u01f8\u01f9\u00053\u0000"+
		"\u0000\u01f9\u01fa\u0003J%\u0000\u01fa\u01fb\u0006\u0012\uffff\uffff\u0000"+
		"\u01fb\u01fd\u0001\u0000\u0000\u0000\u01fc\u01f8\u0001\u0000\u0000\u0000"+
		"\u01fc\u01fd\u0001\u0000\u0000\u0000\u01fd\u01fe\u0001\u0000\u0000\u0000"+
		"\u01fe\u01ff\u0006\u0012\uffff\uffff\u0000\u01ff%\u0001\u0000\u0000\u0000"+
		"\u0200\u0201\u0003\u0010\b\u0000\u0201\u0202\u0006\u0013\uffff\uffff\u0000"+
		"\u0202\u0204\u0001\u0000\u0000\u0000\u0203\u0200\u0001\u0000\u0000\u0000"+
		"\u0204\u0207\u0001\u0000\u0000\u0000\u0205\u0203\u0001\u0000\u0000\u0000"+
		"\u0205\u0206\u0001\u0000\u0000\u0000\u0206\u0208\u0001\u0000\u0000\u0000"+
		"\u0207\u0205\u0001\u0000\u0000\u0000\u0208\u0209\u0005W\u0000\u0000\u0209"+
		"\u020a\u0006\u0013\uffff\uffff\u0000\u020a\u021a\u0005+\u0000\u0000\u020b"+
		"\u020c\u0003$\u0012\u0000\u020c\u020e\u0006\u0013\uffff\uffff\u0000\u020d"+
		"\u020f\u0003\n\u0005\u0000\u020e\u020d\u0001\u0000\u0000\u0000\u020e\u020f"+
		"\u0001\u0000\u0000\u0000\u020f\u0217\u0001\u0000\u0000\u0000\u0210\u0211"+
		"\u0005)\u0000\u0000\u0211\u0212\u0003$\u0012\u0000\u0212\u0213\u0006\u0013"+
		"\uffff\uffff\u0000\u0213\u0216\u0001\u0000\u0000\u0000\u0214\u0216\u0003"+
		"\n\u0005\u0000\u0215\u0210\u0001\u0000\u0000\u0000\u0215\u0214\u0001\u0000"+
		"\u0000\u0000\u0216\u0219\u0001\u0000\u0000\u0000\u0217\u0215\u0001\u0000"+
		"\u0000\u0000\u0217\u0218\u0001\u0000\u0000\u0000\u0218\u021b\u0001\u0000"+
		"\u0000\u0000\u0219\u0217\u0001\u0000\u0000\u0000\u021a\u020b\u0001\u0000"+
		"\u0000\u0000\u021a\u021b\u0001\u0000\u0000\u0000\u021b\u021c\u0001\u0000"+
		"\u0000\u0000\u021c\u021d\u0005,\u0000\u0000\u021d\u021e\u0003\u0012\t"+
		"\u0000\u021e\u021f\u0006\u0013\uffff\uffff\u0000\u021f\'\u0001\u0000\u0000"+
		"\u0000\u0220\u0221\u0003&\u0013\u0000\u0221\u0222\u0006\u0014\uffff\uffff"+
		"\u0000\u0222\u0223\u0003>\u001f\u0000\u0223\u0224\u0006\u0014\uffff\uffff"+
		"\u0000\u0224)\u0001\u0000\u0000\u0000\u0225\u0226\u0003\u000e\u0007\u0000"+
		"\u0226\u0227\u0006\u0015\uffff\uffff\u0000\u0227\u0229\u0001\u0000\u0000"+
		"\u0000\u0228\u0225\u0001\u0000\u0000\u0000\u0229\u022c\u0001\u0000\u0000"+
		"\u0000\u022a\u0228\u0001\u0000\u0000\u0000\u022a\u022b\u0001\u0000\u0000"+
		"\u0000\u022b\u022d\u0001\u0000\u0000\u0000\u022c\u022a\u0001\u0000\u0000"+
		"\u0000\u022d\u022e\u0005\u000b\u0000\u0000\u022e\u022f\u0006\u0015\uffff"+
		"\uffff\u0000\u022f\u0230\u0005W\u0000\u0000\u0230\u0231\u0006\u0015\uffff"+
		"\uffff\u0000\u0231\u0238\u0005-\u0000\u0000\u0232\u0233\u0003J%\u0000"+
		"\u0233\u0234\u0006\u0015\uffff\uffff\u0000\u0234\u0235\u0005(\u0000\u0000"+
		"\u0235\u0237\u0001\u0000\u0000\u0000\u0236\u0232\u0001\u0000\u0000\u0000"+
		"\u0237\u023a\u0001\u0000\u0000\u0000\u0238\u0236\u0001\u0000\u0000\u0000"+
		"\u0238\u0239\u0001\u0000\u0000\u0000\u0239\u023b\u0001\u0000\u0000\u0000"+
		"\u023a\u0238\u0001\u0000\u0000\u0000\u023b\u023c\u0005.\u0000\u0000\u023c"+
		"\u023d\u0006\u0015\uffff\uffff\u0000\u023d+\u0001\u0000\u0000\u0000\u023e"+
		"\u023f\u0003\u000e\u0007\u0000\u023f\u0240\u0006\u0016\uffff\uffff\u0000"+
		"\u0240\u0242\u0001\u0000\u0000\u0000\u0241\u023e\u0001\u0000\u0000\u0000"+
		"\u0242\u0245\u0001\u0000\u0000\u0000\u0243\u0241\u0001\u0000\u0000\u0000"+
		"\u0243\u0244\u0001\u0000\u0000\u0000\u0244\u024b\u0001\u0000\u0000\u0000"+
		"\u0245\u0243\u0001\u0000\u0000\u0000\u0246\u0247\u0003\u0010\b\u0000\u0247"+
		"\u0248\u0006\u0016\uffff\uffff\u0000\u0248\u024a\u0001\u0000\u0000\u0000"+
		"\u0249\u0246\u0001\u0000\u0000\u0000\u024a\u024d\u0001\u0000\u0000\u0000"+
		"\u024b\u0249\u0001\u0000\u0000\u0000\u024b\u024c\u0001\u0000\u0000\u0000"+
		"\u024c\u024e\u0001\u0000\u0000\u0000\u024d\u024b\u0001\u0000\u0000\u0000"+
		"\u024e\u024f\u0005\f\u0000\u0000\u024f\u0250\u0006\u0016\uffff\uffff\u0000"+
		"\u0250\u0251\u0001\u0000\u0000\u0000\u0251\u0252\u0005W\u0000\u0000\u0252"+
		"\u0253\u0006\u0016\uffff\uffff\u0000\u0253\u025a\u0005-\u0000\u0000\u0254"+
		"\u0255\u0003J%\u0000\u0255\u0256\u0006\u0016\uffff\uffff\u0000\u0256\u0257"+
		"\u0005(\u0000\u0000\u0257\u0259\u0001\u0000\u0000\u0000\u0258\u0254\u0001"+
		"\u0000\u0000\u0000\u0259\u025c\u0001\u0000\u0000\u0000\u025a\u0258\u0001"+
		"\u0000\u0000\u0000\u025a\u025b\u0001\u0000\u0000\u0000\u025b\u025d\u0001"+
		"\u0000\u0000\u0000\u025c\u025a\u0001\u0000\u0000\u0000\u025d\u025e\u0005"+
		".\u0000\u0000\u025e\u025f\u0006\u0016\uffff\uffff\u0000\u025f-\u0001\u0000"+
		"\u0000\u0000\u0260\u0261\u0005\r\u0000\u0000\u0261\u0262\u0006\u0017\uffff"+
		"\uffff\u0000\u0262\u0263\u0001\u0000\u0000\u0000\u0263\u0264\u0005W\u0000"+
		"\u0000\u0264\u0265\u0006\u0017\uffff\uffff\u0000\u0265\u026c\u0005-\u0000"+
		"\u0000\u0266\u0267\u0003J%\u0000\u0267\u0268\u0006\u0017\uffff\uffff\u0000"+
		"\u0268\u0269\u0005(\u0000\u0000\u0269\u026b\u0001\u0000\u0000\u0000\u026a"+
		"\u0266\u0001\u0000\u0000\u0000\u026b\u026e\u0001\u0000\u0000\u0000\u026c"+
		"\u026a\u0001\u0000\u0000\u0000\u026c\u026d\u0001\u0000\u0000\u0000\u026d"+
		"\u026f\u0001\u0000\u0000\u0000\u026e\u026c\u0001\u0000\u0000\u0000\u026f"+
		"\u0270\u0005.\u0000\u0000\u0270\u0271\u0006\u0017\uffff\uffff\u0000\u0271"+
		"/\u0001\u0000\u0000\u0000\u0272\u0273\u00034\u001a\u0000\u0273\u0274\u0006"+
		"\u0018\uffff\uffff\u0000\u0274\u028f\u0001\u0000\u0000\u0000\u0275\u0276"+
		"\u0003>\u001f\u0000\u0276\u0277\u0006\u0018\uffff\uffff\u0000\u0277\u028f"+
		"\u0001\u0000\u0000\u0000\u0278\u0279\u00036\u001b\u0000\u0279\u027a\u0006"+
		"\u0018\uffff\uffff\u0000\u027a\u028f\u0001\u0000\u0000\u0000\u027b\u027c"+
		"\u0003<\u001e\u0000\u027c\u027d\u0006\u0018\uffff\uffff\u0000\u027d\u028f"+
		"\u0001\u0000\u0000\u0000\u027e\u027f\u0003D\"\u0000\u027f\u0280\u0006"+
		"\u0018\uffff\uffff\u0000\u0280\u028f\u0001\u0000\u0000\u0000\u0281\u0282"+
		"\u0003@ \u0000\u0282\u0283\u0006\u0018\uffff\uffff\u0000\u0283\u028f\u0001"+
		"\u0000\u0000\u0000\u0284\u0285\u0003B!\u0000\u0285\u0286\u0006\u0018\uffff"+
		"\uffff\u0000\u0286\u028f\u0001\u0000\u0000\u0000\u0287\u0288\u0003F#\u0000"+
		"\u0288\u0289\u0006\u0018\uffff\uffff\u0000\u0289\u028f\u0001\u0000\u0000"+
		"\u0000\u028a\u028b\u00032\u0019\u0000\u028b\u028c\u0005(\u0000\u0000\u028c"+
		"\u028d\u0006\u0018\uffff\uffff\u0000\u028d\u028f\u0001\u0000\u0000\u0000"+
		"\u028e\u0272\u0001\u0000\u0000\u0000\u028e\u0275\u0001\u0000\u0000\u0000"+
		"\u028e\u0278\u0001\u0000\u0000\u0000\u028e\u027b\u0001\u0000\u0000\u0000"+
		"\u028e\u027e\u0001\u0000\u0000\u0000\u028e\u0281\u0001\u0000\u0000\u0000"+
		"\u028e\u0284\u0001\u0000\u0000\u0000\u028e\u0287\u0001\u0000\u0000\u0000"+
		"\u028e\u028a\u0001\u0000\u0000\u0000\u028f1\u0001\u0000\u0000\u0000\u0290"+
		"\u0291\u0003H$\u0000\u0291\u0292\u0006\u0019\uffff\uffff\u0000\u02923"+
		"\u0001\u0000\u0000\u0000\u0293\u0294\u0005\u0007\u0000\u0000\u0294\u0295"+
		"\u0006\u001a\uffff\uffff\u0000\u0295\u0296\u0005+\u0000\u0000\u0296\u0297"+
		"\u0003J%\u0000\u0297\u0298\u0006\u001a\uffff\uffff\u0000\u0298\u0299\u0005"+
		",\u0000\u0000\u0299\u029a\u00030\u0018\u0000\u029a\u029f\u0006\u001a\uffff"+
		"\uffff\u0000\u029b\u029c\u0005\b\u0000\u0000\u029c\u029d\u00030\u0018"+
		"\u0000\u029d\u029e\u0006\u001a\uffff\uffff\u0000\u029e\u02a0\u0001\u0000"+
		"\u0000\u0000\u029f\u029b\u0001\u0000\u0000\u0000\u029f\u02a0\u0001\u0000"+
		"\u0000\u0000\u02a0\u02a1\u0001\u0000\u0000\u0000\u02a1\u02a2\u0006\u001a"+
		"\uffff\uffff\u0000\u02a25\u0001\u0000\u0000\u0000\u02a3\u02a4\u0005\u000e"+
		"\u0000\u0000\u02a4\u02a5\u0006\u001b\uffff\uffff\u0000\u02a5\u02a9\u0005"+
		"+\u0000\u0000\u02a6\u02a7\u0003\u001c\u000e\u0000\u02a7\u02a8\u0006\u001b"+
		"\uffff\uffff\u0000\u02a8\u02aa\u0001\u0000\u0000\u0000\u02a9\u02a6\u0001"+
		"\u0000\u0000\u0000\u02a9\u02aa\u0001\u0000\u0000\u0000\u02aa\u02ab\u0001"+
		"\u0000\u0000\u0000\u02ab\u02af\u0005(\u0000\u0000\u02ac\u02ad\u0003J%"+
		"\u0000\u02ad\u02ae\u0006\u001b\uffff\uffff\u0000\u02ae\u02b0\u0001\u0000"+
		"\u0000\u0000\u02af\u02ac\u0001\u0000\u0000\u0000\u02af\u02b0\u0001\u0000"+
		"\u0000\u0000\u02b0\u02b1\u0001\u0000\u0000\u0000\u02b1\u02b5\u0005(\u0000"+
		"\u0000\u02b2\u02b3\u0003J%\u0000\u02b3\u02b4\u0006\u001b\uffff\uffff\u0000"+
		"\u02b4\u02b6\u0001\u0000\u0000\u0000\u02b5\u02b2\u0001\u0000\u0000\u0000"+
		"\u02b5\u02b6\u0001\u0000\u0000\u0000\u02b6\u02b7\u0001\u0000\u0000\u0000"+
		"\u02b7\u02bd\u0005,\u0000\u0000\u02b8\u02b9\u0003\u0010\b\u0000\u02b9"+
		"\u02ba\u0006\u001b\uffff\uffff\u0000\u02ba\u02bc\u0001\u0000\u0000\u0000"+
		"\u02bb\u02b8\u0001\u0000\u0000\u0000\u02bc\u02bf\u0001\u0000\u0000\u0000"+
		"\u02bd\u02bb\u0001\u0000\u0000\u0000\u02bd\u02be\u0001\u0000\u0000\u0000"+
		"\u02be\u02c0\u0001\u0000\u0000\u0000\u02bf\u02bd\u0001\u0000\u0000\u0000"+
		"\u02c0\u02c1\u00030\u0018\u0000\u02c1\u02c2\u0006\u001b\uffff\uffff\u0000"+
		"\u02c2\u02c3\u0006\u001b\uffff\uffff\u0000\u02c37\u0001\u0000\u0000\u0000"+
		"\u02c4\u02c5\u0005\u000e\u0000\u0000\u02c5\u02c6\u0006\u001c\uffff\uffff"+
		"\u0000\u02c6\u02c7\u0005+\u0000\u0000\u02c7\u02c8\u0005W\u0000\u0000\u02c8"+
		"\u02c9\u0005*\u0000\u0000\u02c9\u02ca\u0005W\u0000\u0000\u02ca\u02cb\u0005"+
		"\u000f\u0000\u0000\u02cb\u02cc\u0005W\u0000\u0000\u02cc\u02cd\u0005,\u0000"+
		"\u0000\u02cd\u02ce\u00030\u0018\u0000\u02ce\u02cf\u0006\u001c\uffff\uffff"+
		"\u0000\u02cf\u02d0\u0006\u001c\uffff\uffff\u0000\u02d09\u0001\u0000\u0000"+
		"\u0000\u02d1\u02d2\u0005\u0010\u0000\u0000\u02d2\u02d3\u0006\u001d\uffff"+
		"\uffff\u0000\u02d3\u02d4\u0005+\u0000\u0000\u02d4\u02d5\u0003J%\u0000"+
		"\u02d5\u02d6\u0005,\u0000\u0000\u02d6\u02d7\u00030\u0018\u0000\u02d7\u02d8"+
		"\u0006\u001d\uffff\uffff\u0000\u02d8\u02d9\u0006\u001d\uffff\uffff\u0000"+
		"\u02d9;\u0001\u0000\u0000\u0000\u02da\u02db\u0005\u0011\u0000\u0000\u02db"+
		"\u02dc\u0006\u001e\uffff\uffff\u0000\u02dc\u02dd\u0005+\u0000\u0000\u02dd"+
		"\u02de\u0003J%\u0000\u02de\u02df\u0006\u001e\uffff\uffff\u0000\u02df\u02e0"+
		"\u0005,\u0000\u0000\u02e0\u02e1\u00030\u0018\u0000\u02e1\u02e2\u0006\u001e"+
		"\uffff\uffff\u0000\u02e2\u02e3\u0006\u001e\uffff\uffff\u0000\u02e3\u02f1"+
		"\u0001\u0000\u0000\u0000\u02e4\u02e5\u0005\u0012\u0000\u0000\u02e5\u02e6"+
		"\u0006\u001e\uffff\uffff\u0000\u02e6\u02e7\u00030\u0018\u0000\u02e7\u02e8"+
		"\u0006\u001e\uffff\uffff\u0000\u02e8\u02e9\u0005\u0011\u0000\u0000\u02e9"+
		"\u02ea\u0005+\u0000\u0000\u02ea\u02eb\u0003J%\u0000\u02eb\u02ec\u0006"+
		"\u001e\uffff\uffff\u0000\u02ec\u02ed\u0005,\u0000\u0000\u02ed\u02ee\u0005"+
		"(\u0000\u0000\u02ee\u02ef\u0006\u001e\uffff\uffff\u0000\u02ef\u02f1\u0001"+
		"\u0000\u0000\u0000\u02f0\u02da\u0001\u0000\u0000\u0000\u02f0\u02e4\u0001"+
		"\u0000\u0000\u0000\u02f1=\u0001\u0000\u0000\u0000\u02f2\u02f3\u0005-\u0000"+
		"\u0000\u02f3\u0302\u0006\u001f\uffff\uffff\u0000\u02f4\u02f5\u0003\u001c"+
		"\u000e\u0000\u02f5\u02f6\u0005(\u0000\u0000\u02f6\u02f7\u0006\u001f\uffff"+
		"\uffff\u0000\u02f7\u0301\u0001\u0000\u0000\u0000\u02f8\u02f9\u00030\u0018"+
		"\u0000\u02f9\u02fa\u0006\u001f\uffff\uffff\u0000\u02fa\u0301\u0001\u0000"+
		"\u0000\u0000\u02fb\u02fc\u0003\f\u0006\u0000\u02fc\u02fd\u0005(\u0000"+
		"\u0000\u02fd\u02fe\u0006\u001f\uffff\uffff\u0000\u02fe\u0301\u0001\u0000"+
		"\u0000\u0000\u02ff\u0301\u0003\n\u0005\u0000\u0300\u02f4\u0001\u0000\u0000"+
		"\u0000\u0300\u02f8\u0001\u0000\u0000\u0000\u0300\u02fb\u0001\u0000\u0000"+
		"\u0000\u0300\u02ff\u0001\u0000\u0000\u0000\u0301\u0304\u0001\u0000\u0000"+
		"\u0000\u0302\u0300\u0001\u0000\u0000\u0000\u0302\u0303\u0001\u0000\u0000"+
		"\u0000\u0303\u0305\u0001\u0000\u0000\u0000\u0304\u0302\u0001\u0000\u0000"+
		"\u0000\u0305\u0306\u0005.\u0000\u0000\u0306\u0307\u0006\u001f\uffff\uffff"+
		"\u0000\u0307\u0308\u0006\u001f\uffff\uffff\u0000\u0308?\u0001\u0000\u0000"+
		"\u0000\u0309\u030a\u0005\u0013\u0000\u0000\u030a\u030e\u0006 \uffff\uffff"+
		"\u0000\u030b\u030c\u0003J%\u0000\u030c\u030d\u0006 \uffff\uffff\u0000"+
		"\u030d\u030f\u0001\u0000\u0000\u0000\u030e\u030b\u0001\u0000\u0000\u0000"+
		"\u030e\u030f\u0001\u0000\u0000\u0000\u030f\u0310\u0001\u0000\u0000\u0000"+
		"\u0310\u0311\u0005(\u0000\u0000\u0311\u031f\u0006 \uffff\uffff\u0000\u0312"+
		"\u0313\u0005\u0014\u0000\u0000\u0313\u0314\u0006 \uffff\uffff\u0000\u0314"+
		"\u0315\u0005(\u0000\u0000\u0315\u031f\u0006 \uffff\uffff\u0000\u0316\u0317"+
		"\u0005\u0015\u0000\u0000\u0317\u0318\u0006 \uffff\uffff\u0000\u0318\u0319"+
		"\u0005(\u0000\u0000\u0319\u031f\u0006 \uffff\uffff\u0000\u031a\u031b\u0005"+
		"\u0016\u0000\u0000\u031b\u031c\u0006 \uffff\uffff\u0000\u031c\u031d\u0005"+
		"(\u0000\u0000\u031d\u031f\u0006 \uffff\uffff\u0000\u031e\u0309\u0001\u0000"+
		"\u0000\u0000\u031e\u0312\u0001\u0000\u0000\u0000\u031e\u0316\u0001\u0000"+
		"\u0000\u0000\u031e\u031a\u0001\u0000\u0000\u0000\u031fA\u0001\u0000\u0000"+
		"\u0000\u0320\u0321\u0005\u0017\u0000\u0000\u0321\u0322\u0006!\uffff\uffff"+
		"\u0000\u0322\u0323\u0005(\u0000\u0000\u0323\u0324\u0006!\uffff\uffff\u0000"+
		"\u0324C\u0001\u0000\u0000\u0000\u0325\u0326\u0005\u0018\u0000\u0000\u0326"+
		"\u0327\u0006\"\uffff\uffff\u0000\u0327\u0328\u0005+\u0000\u0000\u0328"+
		"\u0329\u0003J%\u0000\u0329\u032a\u0005,\u0000\u0000\u032a\u032b\u0006"+
		"\"\uffff\uffff\u0000\u032b\u0337\u0005-\u0000\u0000\u032c\u032d\u0005"+
		"\u0019\u0000\u0000\u032d\u032e\u0003J%\u0000\u032e\u032f\u0005*\u0000"+
		"\u0000\u032f\u0333\u0006\"\uffff\uffff\u0000\u0330\u0331\u00030\u0018"+
		"\u0000\u0331\u0332\u0006\"\uffff\uffff\u0000\u0332\u0334\u0001\u0000\u0000"+
		"\u0000\u0333\u0330\u0001\u0000\u0000\u0000\u0333\u0334\u0001\u0000\u0000"+
		"\u0000\u0334\u0336\u0001\u0000\u0000\u0000\u0335\u032c\u0001\u0000\u0000"+
		"\u0000\u0336\u0339\u0001\u0000\u0000\u0000\u0337\u0335\u0001\u0000\u0000"+
		"\u0000\u0337\u0338\u0001\u0000\u0000\u0000\u0338\u0341\u0001\u0000\u0000"+
		"\u0000\u0339\u0337\u0001\u0000\u0000\u0000\u033a\u033b\u0005\u001a\u0000"+
		"\u0000\u033b\u033f\u0005*\u0000\u0000\u033c\u033d\u00030\u0018\u0000\u033d"+
		"\u033e\u0006\"\uffff\uffff\u0000\u033e\u0340\u0001\u0000\u0000\u0000\u033f"+
		"\u033c\u0001\u0000\u0000\u0000\u033f\u0340\u0001\u0000\u0000\u0000\u0340"+
		"\u0342\u0001\u0000\u0000\u0000\u0341\u033a\u0001\u0000\u0000\u0000\u0341"+
		"\u0342\u0001\u0000\u0000\u0000\u0342\u0343\u0001\u0000\u0000\u0000\u0343"+
		"\u0344\u0005.\u0000\u0000\u0344\u0345\u0006\"\uffff\uffff\u0000\u0345"+
		"E\u0001\u0000\u0000\u0000\u0346\u0347\u0005\u001b\u0000\u0000\u0347\u0348"+
		"\u0006#\uffff\uffff\u0000\u0348\u0349\u0005(\u0000\u0000\u0349\u034a\u0006"+
		"#\uffff\uffff\u0000\u034aG\u0001\u0000\u0000\u0000\u034b\u034d\u0003\n"+
		"\u0005\u0000\u034c\u034b\u0001\u0000\u0000\u0000\u034c\u034d\u0001\u0000"+
		"\u0000\u0000\u034d\u034e\u0001\u0000\u0000\u0000\u034e\u034f\u0003J%\u0000"+
		"\u034f\u0357\u0006$\uffff\uffff\u0000\u0350\u0351\u0005)\u0000\u0000\u0351"+
		"\u0352\u0003J%\u0000\u0352\u0353\u0006$\uffff\uffff\u0000\u0353\u0356"+
		"\u0001\u0000\u0000\u0000\u0354\u0356\u0003\n\u0005\u0000\u0355\u0350\u0001"+
		"\u0000\u0000\u0000\u0355\u0354\u0001\u0000\u0000\u0000\u0356\u0359\u0001"+
		"\u0000\u0000\u0000\u0357\u0355\u0001\u0000\u0000\u0000\u0357\u0358\u0001"+
		"\u0000\u0000\u0000\u0358\u035a\u0001\u0000\u0000\u0000\u0359\u0357\u0001"+
		"\u0000\u0000\u0000\u035a\u035b\u0006$\uffff\uffff\u0000\u035bI\u0001\u0000"+
		"\u0000\u0000\u035c\u035d\u0006%\uffff\uffff\u0000\u035d\u035e\u0007\u0003"+
		"\u0000\u0000\u035e\u035f\u0003J%\u000e\u035f\u0360\u0006%\uffff\uffff"+
		"\u0000\u0360\u0365\u0001\u0000\u0000\u0000\u0361\u0362\u0003L&\u0000\u0362"+
		"\u0363\u0006%\uffff\uffff\u0000\u0363\u0365\u0001\u0000\u0000\u0000\u0364"+
		"\u035c\u0001\u0000\u0000\u0000\u0364\u0361\u0001\u0000\u0000\u0000\u0365"+
		"\u03e4\u0001\u0000\u0000\u0000\u0366\u0367\n\u0011\u0000\u0000\u0367\u0368"+
		"\u0006%\uffff\uffff\u0000\u0368\u0369\u00051\u0000\u0000\u0369\u036a\u0006"+
		"%\uffff\uffff\u0000\u036a\u036b\u0003J%\u0012\u036b\u036c\u0006%\uffff"+
		"\uffff\u0000\u036c\u03e3\u0001\u0000\u0000\u0000\u036d\u036e\n\u0010\u0000"+
		"\u0000\u036e\u036f\u0006%\uffff\uffff\u0000\u036f\u0370\u0005O\u0000\u0000"+
		"\u0370\u0371\u0006%\uffff\uffff\u0000\u0371\u0372\u0003J%\u0011\u0372"+
		"\u0373\u0006%\uffff\uffff\u0000\u0373\u03e3\u0001\u0000\u0000\u0000\u0374"+
		"\u0375\n\r\u0000\u0000\u0375\u0376\u0006%\uffff\uffff\u0000\u0376\u0377"+
		"\u0007\u0004\u0000\u0000\u0377\u0378\u0006%\uffff\uffff\u0000\u0378\u0379"+
		"\u0003J%\u000e\u0379\u037a\u0006%\uffff\uffff\u0000\u037a\u03e3\u0001"+
		"\u0000\u0000\u0000\u037b\u037c\n\f\u0000\u0000\u037c\u037d\u0006%\uffff"+
		"\uffff\u0000\u037d\u037e\u0007\u0005\u0000\u0000\u037e\u037f\u0006%\uffff"+
		"\uffff\u0000\u037f\u0380\u0003J%\r\u0380\u0381\u0006%\uffff\uffff\u0000"+
		"\u0381\u03e3\u0001\u0000\u0000\u0000\u0382\u0383\n\u000b\u0000\u0000\u0383"+
		"\u0384\u0006%\uffff\uffff\u0000\u0384\u0385\u0007\u0006\u0000\u0000\u0385"+
		"\u0386\u0006%\uffff\uffff\u0000\u0386\u0387\u0003J%\f\u0387\u0388\u0006"+
		"%\uffff\uffff\u0000\u0388\u03e3\u0001\u0000\u0000\u0000\u0389\u038a\n"+
		"\n\u0000\u0000\u038a\u038b\u0006%\uffff\uffff\u0000\u038b\u038c\u0007"+
		"\u0007\u0000\u0000\u038c\u038d\u0006%\uffff\uffff\u0000\u038d\u038e\u0003"+
		"J%\u000b\u038e\u038f\u0006%\uffff\uffff\u0000\u038f\u03e3\u0001\u0000"+
		"\u0000\u0000\u0390\u0391\n\t\u0000\u0000\u0391\u0392\u0006%\uffff\uffff"+
		"\u0000\u0392\u0393\u0007\b\u0000\u0000\u0393\u0394\u0006%\uffff\uffff"+
		"\u0000\u0394\u0395\u0003J%\n\u0395\u0396\u0006%\uffff\uffff\u0000\u0396"+
		"\u03e3\u0001\u0000\u0000\u0000\u0397\u0398\n\b\u0000\u0000\u0398\u0399"+
		"\u0006%\uffff\uffff\u0000\u0399\u039a\u00056\u0000\u0000\u039a\u039b\u0006"+
		"%\uffff\uffff\u0000\u039b\u039c\u0003J%\t\u039c\u039d\u0006%\uffff\uffff"+
		"\u0000\u039d\u03e3\u0001\u0000\u0000\u0000\u039e\u039f\n\u0007\u0000\u0000"+
		"\u039f\u03a0\u0006%\uffff\uffff\u0000\u03a0\u03a1\u0005:\u0000\u0000\u03a1"+
		"\u03a2\u0006%\uffff\uffff\u0000\u03a2\u03a3\u0003J%\b\u03a3\u03a4\u0006"+
		"%\uffff\uffff\u0000\u03a4\u03e3\u0001\u0000\u0000\u0000\u03a5\u03a6\n"+
		"\u0006\u0000\u0000\u03a6\u03a7\u0006%\uffff\uffff\u0000\u03a7\u03a8\u0005"+
		"8\u0000\u0000\u03a8\u03a9\u0006%\uffff\uffff\u0000\u03a9\u03aa\u0003J"+
		"%\u0007\u03aa\u03ab\u0006%\uffff\uffff\u0000\u03ab\u03e3\u0001\u0000\u0000"+
		"\u0000\u03ac\u03ad\n\u0005\u0000\u0000\u03ad\u03ae\u0006%\uffff\uffff"+
		"\u0000\u03ae\u03af\u0005F\u0000\u0000\u03af\u03b0\u0006%\uffff\uffff\u0000"+
		"\u03b0\u03b1\u0003J%\u0006\u03b1\u03b2\u0006%\uffff\uffff\u0000\u03b2"+
		"\u03e3\u0001\u0000\u0000\u0000\u03b3\u03b4\n\u0004\u0000\u0000\u03b4\u03b5"+
		"\u0006%\uffff\uffff\u0000\u03b5\u03b6\u0005G\u0000\u0000\u03b6\u03b7\u0006"+
		"%\uffff\uffff\u0000\u03b7\u03b8\u0003J%\u0005\u03b8\u03b9\u0006%\uffff"+
		"\uffff\u0000\u03b9\u03e3\u0001\u0000\u0000\u0000\u03ba\u03bb\n\u0003\u0000"+
		"\u0000\u03bb\u03bc\u00055\u0000\u0000\u03bc\u03bd\u0006%\uffff\uffff\u0000"+
		"\u03bd\u03be\u0003J%\u0000\u03be\u03bf\u0005*\u0000\u0000\u03bf\u03c0"+
		"\u0003J%\u0003\u03c0\u03c1\u0006%\uffff\uffff\u0000\u03c1\u03e3\u0001"+
		"\u0000\u0000\u0000\u03c2\u03c3\n\u0002\u0000\u0000\u03c3\u03c4\u0006%"+
		"\uffff\uffff\u0000\u03c4\u03c5\u0007\t\u0000\u0000\u03c5\u03c6\u0006%"+
		"\uffff\uffff\u0000\u03c6\u03c7\u0003J%\u0002\u03c7\u03c8\u0006%\uffff"+
		"\uffff\u0000\u03c8\u03e3\u0001\u0000\u0000\u0000\u03c9\u03ca\n\u0013\u0000"+
		"\u0000\u03ca\u03cb\u0006%\uffff\uffff\u0000\u03cb\u03cc\u0007\n\u0000"+
		"\u0000\u03cc\u03cd\u0006%\uffff\uffff\u0000\u03cd\u03e3\u0006%\uffff\uffff"+
		"\u0000\u03ce\u03cf\n\u0012\u0000\u0000\u03cf\u03d0\u0006%\uffff\uffff"+
		"\u0000\u03d0\u03d1\u0005+\u0000\u0000\u03d1\u03d5\u0006%\uffff\uffff\u0000"+
		"\u03d2\u03d3\u0003H$\u0000\u03d3\u03d4\u0006%\uffff\uffff\u0000\u03d4"+
		"\u03d6\u0001\u0000\u0000\u0000\u03d5\u03d2\u0001\u0000\u0000\u0000\u03d5"+
		"\u03d6\u0001\u0000\u0000\u0000\u03d6\u03d7\u0001\u0000\u0000\u0000\u03d7"+
		"\u03d8\u0005,\u0000\u0000\u03d8\u03e3\u0006%\uffff\uffff\u0000\u03d9\u03da"+
		"\n\u000f\u0000\u0000\u03da\u03db\u0006%\uffff\uffff\u0000\u03db\u03dc"+
		"\u0005/\u0000\u0000\u03dc\u03de\u0006%\uffff\uffff\u0000\u03dd\u03df\u0003"+
		"J%\u0000\u03de\u03dd\u0001\u0000\u0000\u0000\u03de\u03df\u0001\u0000\u0000"+
		"\u0000\u03df\u03e0\u0001\u0000\u0000\u0000\u03e0\u03e1\u00050\u0000\u0000"+
		"\u03e1\u03e3\u0006%\uffff\uffff\u0000\u03e2\u0366\u0001\u0000\u0000\u0000"+
		"\u03e2\u036d\u0001\u0000\u0000\u0000\u03e2\u0374\u0001\u0000\u0000\u0000"+
		"\u03e2\u037b\u0001\u0000\u0000\u0000\u03e2\u0382\u0001\u0000\u0000\u0000"+
		"\u03e2\u0389\u0001\u0000\u0000\u0000\u03e2\u0390\u0001\u0000\u0000\u0000"+
		"\u03e2\u0397\u0001\u0000\u0000\u0000\u03e2\u039e\u0001\u0000\u0000\u0000"+
		"\u03e2\u03a5\u0001\u0000\u0000\u0000\u03e2\u03ac\u0001\u0000\u0000\u0000"+
		"\u03e2\u03b3\u0001\u0000\u0000\u0000\u03e2\u03ba\u0001\u0000\u0000\u0000"+
		"\u03e2\u03c2\u0001\u0000\u0000\u0000\u03e2\u03c9\u0001\u0000\u0000\u0000"+
		"\u03e2\u03ce\u0001\u0000\u0000\u0000\u03e2\u03d9\u0001\u0000\u0000\u0000"+
		"\u03e3\u03e6\u0001\u0000\u0000\u0000\u03e4\u03e2\u0001\u0000\u0000\u0000"+
		"\u03e4\u03e5\u0001\u0000\u0000\u0000\u03e5K\u0001\u0000\u0000\u0000\u03e6"+
		"\u03e4\u0001\u0000\u0000\u0000\u03e7\u03e8\u0003N\'\u0000\u03e8\u03e9"+
		"\u0006&\uffff\uffff\u0000\u03e9\u040c\u0001\u0000\u0000\u0000\u03ea\u03eb"+
		"\u0003P(\u0000\u03eb\u03ec\u0006&\uffff\uffff\u0000\u03ec\u040c\u0001"+
		"\u0000\u0000\u0000\u03ed\u03ee\u0005+\u0000\u0000\u03ee\u03ef\u0003J%"+
		"\u0000\u03ef\u03f0\u0005,\u0000\u0000\u03f0\u03f1\u0006&\uffff\uffff\u0000"+
		"\u03f1\u040c\u0001\u0000\u0000\u0000\u03f2\u03f3\u0005P\u0000\u0000\u03f3"+
		"\u040c\u0006&\uffff\uffff\u0000\u03f4\u03f5\u0005Q\u0000\u0000\u03f5\u040c"+
		"\u0006&\uffff\uffff\u0000\u03f6\u03f7\u0005T\u0000\u0000\u03f7\u040c\u0006"+
		"&\uffff\uffff\u0000\u03f8\u03f9\u0005U\u0000\u0000\u03f9\u040c\u0006&"+
		"\uffff\uffff\u0000\u03fa\u03fb\u0005V\u0000\u0000\u03fb\u040c\u0006&\uffff"+
		"\uffff\u0000\u03fc\u03fd\u0003\u0000\u0000\u0000\u03fd\u03fe\u0006&\uffff"+
		"\uffff\u0000\u03fe\u040c\u0001\u0000\u0000\u0000\u03ff\u0400\u0005W\u0000"+
		"\u0000\u0400\u040c\u0006&\uffff\uffff\u0000\u0401\u0402\u0003\u0004\u0002"+
		"\u0000\u0402\u0403\u0006&\uffff\uffff\u0000\u0403\u040c\u0001\u0000\u0000"+
		"\u0000\u0404\u0405\u0005\'\u0000\u0000\u0405\u0406\u0006&\uffff\uffff"+
		"\u0000\u0406\u0407\u0005@\u0000\u0000\u0407\u0408\u0005W\u0000\u0000\u0408"+
		"\u0409\u0005B\u0000\u0000\u0409\u040c\u0006&\uffff\uffff\u0000\u040a\u040c"+
		"\u0003\n\u0005\u0000\u040b\u03e7\u0001\u0000\u0000\u0000\u040b\u03ea\u0001"+
		"\u0000\u0000\u0000\u040b\u03ed\u0001\u0000\u0000\u0000\u040b\u03f2\u0001"+
		"\u0000\u0000\u0000\u040b\u03f4\u0001\u0000\u0000\u0000\u040b\u03f6\u0001"+
		"\u0000\u0000\u0000\u040b\u03f8\u0001\u0000\u0000\u0000\u040b\u03fa\u0001"+
		"\u0000\u0000\u0000\u040b\u03fc\u0001\u0000\u0000\u0000\u040b\u03ff\u0001"+
		"\u0000\u0000\u0000\u040b\u0401\u0001\u0000\u0000\u0000\u040b\u0404\u0001"+
		"\u0000\u0000\u0000\u040b\u040a\u0001\u0000\u0000\u0000\u040cM\u0001\u0000"+
		"\u0000\u0000\u040d\u040e\u0005W\u0000\u0000\u040e\u040f\u0006\'\uffff"+
		"\uffff\u0000\u040f\u0410\u0005-\u0000\u0000\u0410\u0414\u0006\'\uffff"+
		"\uffff\u0000\u0411\u0412\u0003H$\u0000\u0412\u0413\u0006\'\uffff\uffff"+
		"\u0000\u0413\u0415\u0001\u0000\u0000\u0000\u0414\u0411\u0001\u0000\u0000"+
		"\u0000\u0414\u0415\u0001\u0000\u0000\u0000\u0415\u0416\u0001\u0000\u0000"+
		"\u0000\u0416\u0417\u0005.\u0000\u0000\u0417\u0418\u0006\'\uffff\uffff"+
		"\u0000\u0418O\u0001\u0000\u0000\u0000\u0419\u041a\u0005/\u0000\u0000\u041a"+
		"\u041b\u0006(\uffff\uffff\u0000\u041b\u041c\u0003H$\u0000\u041c\u041d"+
		"\u00050\u0000\u0000\u041d\u041e\u0006(\uffff\uffff\u0000\u041eQ\u0001"+
		"\u0000\u0000\u0000\u041f\u0420\u0005@\u0000\u0000\u0420\u0421\u0006)\uffff"+
		"\uffff\u0000\u0421\u0422\u0005T\u0000\u0000\u0422\u0423\u0006)\uffff\uffff"+
		"\u0000\u0423\u0425\u0001\u0000\u0000\u0000\u0424\u0426\u0003\n\u0005\u0000"+
		"\u0425\u0424\u0001\u0000\u0000\u0000\u0425\u0426\u0001\u0000\u0000\u0000"+
		"\u0426\u042a\u0001\u0000\u0000\u0000\u0427\u0428\u0005)\u0000\u0000\u0428"+
		"\u0429\u0005T\u0000\u0000\u0429\u042b\u0006)\uffff\uffff\u0000\u042a\u0427"+
		"\u0001\u0000\u0000\u0000\u042b\u042c\u0001\u0000\u0000\u0000\u042c\u042a"+
		"\u0001\u0000\u0000\u0000\u042c\u042d\u0001\u0000\u0000\u0000\u042d\u042e"+
		"\u0001\u0000\u0000\u0000\u042e\u042f\u0005B\u0000\u0000\u042f\u0430\u0006"+
		")\uffff\uffff\u0000\u0430S\u0001\u0000\u0000\u0000\u0431\u0432\u0005@"+
		"\u0000\u0000\u0432\u0433\u0006*\uffff\uffff\u0000\u0433\u0434\u0005U\u0000"+
		"\u0000\u0434\u0435\u0006*\uffff\uffff\u0000\u0435\u0437\u0001\u0000\u0000"+
		"\u0000\u0436\u0438\u0003\n\u0005\u0000\u0437\u0436\u0001\u0000\u0000\u0000"+
		"\u0437\u0438\u0001\u0000\u0000\u0000\u0438\u043c\u0001\u0000\u0000\u0000"+
		"\u0439\u043a\u0005)\u0000\u0000\u043a\u043b\u0005U\u0000\u0000\u043b\u043d"+
		"\u0006*\uffff\uffff\u0000\u043c\u0439\u0001\u0000\u0000\u0000\u043d\u043e"+
		"\u0001\u0000\u0000\u0000\u043e\u043c\u0001\u0000\u0000\u0000\u043e\u043f"+
		"\u0001\u0000\u0000\u0000\u043f\u0440\u0001\u0000\u0000\u0000\u0440\u0441"+
		"\u0005B\u0000\u0000\u0441\u0442\u0006*\uffff\uffff\u0000\u0442U\u0001"+
		"\u0000\u0000\u0000\u0443\u0444\u0005@\u0000\u0000\u0444\u0445\u0006+\uffff"+
		"\uffff\u0000\u0445\u0446\u0005P\u0000\u0000\u0446\u0447\u0006+\uffff\uffff"+
		"\u0000\u0447\u0449\u0001\u0000\u0000\u0000\u0448\u044a\u0003\n\u0005\u0000"+
		"\u0449\u0448\u0001\u0000\u0000\u0000\u0449\u044a\u0001\u0000\u0000\u0000"+
		"\u044a\u044e\u0001\u0000\u0000\u0000\u044b\u044c\u0005)\u0000\u0000\u044c"+
		"\u044d\u0005P\u0000\u0000\u044d\u044f\u0006+\uffff\uffff\u0000\u044e\u044b"+
		"\u0001\u0000\u0000\u0000\u044f\u0450\u0001\u0000\u0000\u0000\u0450\u044e"+
		"\u0001\u0000\u0000\u0000\u0450\u0451\u0001\u0000\u0000\u0000\u0451\u0452"+
		"\u0001\u0000\u0000\u0000\u0452\u0453\u0005B\u0000\u0000\u0453\u0454\u0006"+
		"+\uffff\uffff\u0000\u0454W\u0001\u0000\u0000\u0000\u0455\u0456\u0005@"+
		"\u0000\u0000\u0456\u0457\u0006,\uffff\uffff\u0000\u0457\u0458\u0005Q\u0000"+
		"\u0000\u0458\u0459\u0006,\uffff\uffff\u0000\u0459\u045b\u0001\u0000\u0000"+
		"\u0000\u045a\u045c\u0003\n\u0005\u0000\u045b\u045a\u0001\u0000\u0000\u0000"+
		"\u045b\u045c\u0001\u0000\u0000\u0000\u045c\u0460\u0001\u0000\u0000\u0000"+
		"\u045d\u045e\u0005)\u0000\u0000\u045e\u045f\u0005Q\u0000\u0000\u045f\u0461"+
		"\u0006,\uffff\uffff\u0000\u0460\u045d\u0001\u0000\u0000\u0000\u0461\u0462"+
		"\u0001\u0000\u0000\u0000\u0462\u0460\u0001\u0000\u0000\u0000\u0462\u0463"+
		"\u0001\u0000\u0000\u0000\u0463\u0464\u0001\u0000\u0000\u0000\u0464\u0465"+
		"\u0005B\u0000\u0000\u0465\u0466\u0006,\uffff\uffff\u0000\u0466Y\u0001"+
		"\u0000\u0000\u0000\u0467\u0468\u0005@\u0000\u0000\u0468\u0469\u0006-\uffff"+
		"\uffff\u0000\u0469\u046a\u0003\u0004\u0002\u0000\u046a\u046b\u0006-\uffff"+
		"\uffff\u0000\u046b\u046d\u0001\u0000\u0000\u0000\u046c\u046e\u0003\n\u0005"+
		"\u0000\u046d\u046c\u0001\u0000\u0000\u0000\u046d\u046e\u0001\u0000\u0000"+
		"\u0000\u046e\u0473\u0001\u0000\u0000\u0000\u046f\u0470\u0005)\u0000\u0000"+
		"\u0470\u0471\u0003\u0004\u0002\u0000\u0471\u0472\u0006-\uffff\uffff\u0000"+
		"\u0472\u0474\u0001\u0000\u0000\u0000\u0473\u046f\u0001\u0000\u0000\u0000"+
		"\u0474\u0475\u0001\u0000\u0000\u0000\u0475\u0473\u0001\u0000\u0000\u0000"+
		"\u0475\u0476\u0001\u0000\u0000\u0000\u0476\u0477\u0001\u0000\u0000\u0000"+
		"\u0477\u0478\u0005B\u0000\u0000\u0478\u0479\u0006-\uffff\uffff\u0000\u0479"+
		"[\u0001\u0000\u0000\u0000]ajnu~\u0082\u0088\u00b6\u00b8\u00db\u00e5\u00eb"+
		"\u00ed\u0108\u010a\u0116\u0129\u012b\u013e\u0145\u014d\u0155\u015a\u0160"+
		"\u0162\u0169\u0173\u0175\u0178\u017f\u0187\u018f\u01a4\u01a7\u01a9\u01b1"+
		"\u01b3\u01c0\u01c9\u01cd\u01d6\u01da\u01dc\u01e0\u01e8\u01f0\u01fc\u0205"+
		"\u020e\u0215\u0217\u021a\u022a\u0238\u0243\u024b\u025a\u026c\u028e\u029f"+
		"\u02a9\u02af\u02b5\u02bd\u02f0\u0300\u0302\u030e\u031e\u0333\u0337\u033f"+
		"\u0341\u034c\u0355\u0357\u0364\u03d5\u03de\u03e2\u03e4\u040b\u0414\u0425"+
		"\u042c\u0437\u043e\u0449\u0450\u045b\u0462\u046d\u0475";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}