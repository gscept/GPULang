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
		RULE_expressionList = 33, RULE_expression = 34, RULE_binaryexpatom = 35, 
		RULE_initializerExpression = 36, RULE_arrayInitializerExpression = 37, 
		RULE_floatVecLiteralExpression = 38, RULE_doubleVecLiteralExpression = 39, 
		RULE_intVecLiteralExpression = 40, RULE_uintVecLiteralExpression = 41, 
		RULE_booleanVecLiteralExpression = 42;
	private static String[] makeRuleNames() {
		return new String[] {
			"string", "path", "boolean", "entry", "effect", "linePreprocessorEntry", 
			"alias", "annotation", "attribute", "typeDeclaration", "generate", "variables", 
			"structureDeclaration", "structure", "enumeration", "parameter", "functionDeclaration", 
			"function", "program", "sampler", "state", "statement", "expressionStatement", 
			"ifStatement", "forStatement", "forRangeStatement", "forUniformValueStatement", 
			"whileStatement", "scopeStatement", "terminateStatement", "continueStatement", 
			"switchStatement", "breakStatement", "expressionList", "expression", 
			"binaryexpatom", "initializerExpression", "arrayInitializerExpression", 
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
			setState(104);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case QO:
				enterOuterAlt(_localctx, 1);
				{
				setState(86);
				match(QO);
				setState(91);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while ((((_la) & ~0x3f) == 0 && ((1L << _la) & -2251799813685250L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & 16777215L) != 0)) {
					{
					{
					setState(87);
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
					setState(93);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				setState(94);
				match(QO);
				}
				break;
			case Q:
				enterOuterAlt(_localctx, 2);
				{
				setState(95);
				match(Q);
				setState(100);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while ((((_la) & ~0x3f) == 0 && ((1L << _la) & -1152921504606846978L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & 16777215L) != 0)) {
					{
					{
					setState(96);
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
					setState(102);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				setState(103);
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
			setState(124);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case QO:
				enterOuterAlt(_localctx, 1);
				{
				setState(106);
				match(QO);
				setState(111);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while ((((_la) & ~0x3f) == 0 && ((1L << _la) & -2251799813685250L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & 16777215L) != 0)) {
					{
					{
					setState(107);
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
					setState(113);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				setState(114);
				match(QO);
				}
				break;
			case LESS:
				enterOuterAlt(_localctx, 2);
				{
				setState(115);
				match(LESS);
				setState(120);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while ((((_la) & ~0x3f) == 0 && ((1L << _la) & -2L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & 16777213L) != 0)) {
					{
					{
					setState(116);
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
					setState(122);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				setState(123);
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
			setState(130);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__0:
				enterOuterAlt(_localctx, 1);
				{
				setState(126);
				match(T__0);
				 ((BooleanContext)_localctx).val =  true; 
				}
				break;
			case T__1:
				enterOuterAlt(_localctx, 2);
				{
				setState(128);
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
			setState(132);
			((EntryContext)_localctx).effect = effect();

			        ((EntryContext)_localctx).returnEffect =  ((EntryContext)_localctx).effect.eff;
			    
			setState(134);
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
			setState(178);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,8,_ctx);
			while ( _alt!=1 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1+1 ) {
					{
					setState(176);
					_errHandler.sync(this);
					switch ( getInterpreter().adaptivePredict(_input,7,_ctx) ) {
					case 1:
						{
						setState(136);
						linePreprocessorEntry();
						}
						break;
					case 2:
						{
						setState(137);
						((EffectContext)_localctx).generate = generate();
						setState(138);
						match(SC);
						 _localctx.eff->symbols.Append(((EffectContext)_localctx).generate.sym); 
						}
						break;
					case 3:
						{
						setState(141);
						((EffectContext)_localctx).alias = alias();
						setState(142);
						match(SC);
						 _localctx.eff->symbols.Append(((EffectContext)_localctx).alias.sym); 
						}
						break;
					case 4:
						{
						setState(145);
						((EffectContext)_localctx).functionDeclaration = functionDeclaration();
						setState(146);
						match(SC);
						 _localctx.eff->symbols.Append(((EffectContext)_localctx).functionDeclaration.sym); 
						}
						break;
					case 5:
						{
						setState(149);
						((EffectContext)_localctx).function = function();
						 _localctx.eff->symbols.Append(((EffectContext)_localctx).function.sym); 
						}
						break;
					case 6:
						{
						setState(152);
						((EffectContext)_localctx).variables = variables();
						setState(153);
						match(SC);
						 for (Variable* var : ((EffectContext)_localctx).variables.vars) { _localctx.eff->symbols.Append(var); } 
						}
						break;
					case 7:
						{
						setState(156);
						((EffectContext)_localctx).structure = structure();
						setState(157);
						match(SC);
						 _localctx.eff->symbols.Append(((EffectContext)_localctx).structure.sym); 
						}
						break;
					case 8:
						{
						setState(160);
						((EffectContext)_localctx).enumeration = enumeration();
						setState(161);
						match(SC);
						 _localctx.eff->symbols.Append(((EffectContext)_localctx).enumeration.sym); 
						}
						break;
					case 9:
						{
						setState(164);
						((EffectContext)_localctx).state = state();
						setState(165);
						match(SC);
						 _localctx.eff->symbols.Append(((EffectContext)_localctx).state.sym); 
						}
						break;
					case 10:
						{
						setState(168);
						((EffectContext)_localctx).sampler = sampler();
						setState(169);
						match(SC);
						 _localctx.eff->symbols.Append(((EffectContext)_localctx).sampler.sym); 
						}
						break;
					case 11:
						{
						setState(172);
						((EffectContext)_localctx).program = program();
						setState(173);
						match(SC);
						 _localctx.eff->symbols.Append(((EffectContext)_localctx).program.sym); 
						}
						break;
					}
					} 
				}
				setState(180);
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
			setState(181);
			match(T__2);
			 origLine = _input->LT(-1)->getLine(); 
			setState(183);
			((LinePreprocessorEntryContext)_localctx).line = match(INTEGERLITERAL);
			setState(184);
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
		        FixedString name;
		        FixedString type;
		    
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(187);
			match(T__3);
			setState(188);
			((AliasContext)_localctx).name = match(IDENTIFIER);
			setState(189);
			match(T__4);
			setState(190);
			((AliasContext)_localctx).type = match(IDENTIFIER);
			 name = FixedString((((AliasContext)_localctx).name!=null?((AliasContext)_localctx).name.getText():null)); type = FixedString((((AliasContext)_localctx).type!=null?((AliasContext)_localctx).type.getText():null)); 

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
			setState(194);
			match(SOBAKA);
			 ((AnnotationContext)_localctx).annot =  Alloc<Annotation>(); 
			{
			setState(196);
			((AnnotationContext)_localctx).name = match(IDENTIFIER);
			 _localctx.annot->location = SetupFile(); 
			setState(198);
			match(LP);
			setState(199);
			((AnnotationContext)_localctx).value = expression(0);
			setState(200);
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
			setState(212);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,9,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(203);
				((AttributeContext)_localctx).name = match(IDENTIFIER);
				 ((AttributeContext)_localctx).attr =  Alloc<Attribute>(); _localctx.attr->location = SetupFile(); _localctx.attr->name = (((AttributeContext)_localctx).name!=null?((AttributeContext)_localctx).name.getText():null); 
				setState(205);
				match(LP);
				setState(206);
				((AttributeContext)_localctx).e = ((AttributeContext)_localctx).expression = expression(0);
				setState(207);
				match(RP);
				 _localctx.attr->expression = ((AttributeContext)_localctx).expression.tree; 
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(210);
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
			setState(230);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,12,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					{
					setState(228);
					_errHandler.sync(this);
					switch (_input.LA(1)) {
					case MUL_OP:
						{
						setState(215);
						match(MUL_OP);
						 _localctx.type.type.AddModifier(Type::FullType::Modifier::Pointer); 
						}
						break;
					case LL:
						{
						setState(217);
						match(LL);
						 _localctx.type.type.AddModifier(Type::FullType::Modifier::Array); 
						setState(222);
						_errHandler.sync(this);
						_la = _input.LA(1);
						if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 1732271774273372174L) != 0) || ((((_la - 74)) & ~0x3f) == 0 && ((1L << (_la - 74)) & 7787L) != 0)) {
							{
							setState(219);
							((TypeDeclarationContext)_localctx).arraySize0 = expression(0);
							 _localctx.type.type.UpdateValue(((TypeDeclarationContext)_localctx).arraySize0.tree); 
							}
						}

						setState(224);
						match(RR);
						}
						break;
					case IDENTIFIER:
						{
						setState(225);
						((TypeDeclarationContext)_localctx).qual = match(IDENTIFIER);
						 _localctx.type.type.AddQualifier(FixedString((((TypeDeclarationContext)_localctx).qual!=null?((TypeDeclarationContext)_localctx).qual.getText():null))); 
						}
						break;
					case T__2:
						{
						setState(227);
						linePreprocessorEntry();
						}
						break;
					default:
						throw new NoViableAltException(this);
					}
					} 
				}
				setState(232);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,12,_ctx);
			}
			setState(233);
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
			setState(236);
			match(T__5);
			setState(237);
			match(LESS);
			setState(243);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & 1732289366627078158L) != 0) || ((((_la - 74)) & ~0x3f) == 0 && ((1L << (_la - 74)) & 7787L) != 0)) {
				{
				{
				setState(238);
				((GenerateContext)_localctx).statement = statement();
				 symbols.Append(((GenerateContext)_localctx).statement.tree); 
				}
				}
				setState(245);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(246);
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
		enterRule(_localctx, 22, RULE_variables);

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
			setState(251);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__2) {
				{
				{
				setState(248);
				linePreprocessorEntry();
				}
				}
				setState(253);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(259);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==SOBAKA) {
				{
				{
				setState(254);
				((VariablesContext)_localctx).annotation = annotation();
				 if (annotations.Full()) { throw IndexOutOfBoundsException("Maximum of 32 annotations reached"); } annotations.Append(std::move(((VariablesContext)_localctx).annotation.annot)); 
				}
				}
				setState(261);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(265); 
			_errHandler.sync(this);
			_alt = 1;
			do {
				switch (_alt) {
				case 1:
					{
					{
					setState(262);
					((VariablesContext)_localctx).attribute = attribute();
					 if (attributes.Full()) { throw IndexOutOfBoundsException("Maximum of 32 attributes reached"); } attributes.Append(std::move(((VariablesContext)_localctx).attribute.attr)); 
					}
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				setState(267); 
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,16,_ctx);
			} while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER );
			setState(269);
			((VariablesContext)_localctx).varName = match(IDENTIFIER);
			 names.Append(FixedString((((VariablesContext)_localctx).varName!=null?((VariablesContext)_localctx).varName.getText():null))); valueExpressions.Append(nullptr); locations.Append(SetupFile()); 
			setState(272);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,17,_ctx) ) {
			case 1:
				{
				setState(271);
				linePreprocessorEntry();
				}
				break;
			}
			setState(280);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__2 || _la==CO) {
				{
				setState(278);
				_errHandler.sync(this);
				switch (_input.LA(1)) {
				case CO:
					{
					setState(274);
					match(CO);
					setState(275);
					((VariablesContext)_localctx).varNameN = match(IDENTIFIER);
					 if (names.Full()) { throw IndexOutOfBoundsException("Maximum of 256 variable declarations reached"); } names.Append(FixedString((((VariablesContext)_localctx).varNameN!=null?((VariablesContext)_localctx).varNameN.getText():null))); valueExpressions.Append(nullptr); locations.Append(SetupFile()); 
					}
					break;
				case T__2:
					{
					setState(277);
					linePreprocessorEntry();
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				}
				setState(282);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(287);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COL) {
				{
				setState(283);
				match(COL);
				setState(284);
				((VariablesContext)_localctx).typeDeclaration = typeDeclaration();
				 type = ((VariablesContext)_localctx).typeDeclaration.type; 
				}
			}

			setState(302);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==EQ) {
				{
				setState(289);
				match(EQ);
				setState(290);
				((VariablesContext)_localctx).valueExpr = expression(0);
				 if (initCounter < names.size) { valueExpressions[initCounter++] = ((VariablesContext)_localctx).valueExpr.tree; }  
				setState(299);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while (_la==T__2 || _la==CO) {
					{
					setState(297);
					_errHandler.sync(this);
					switch (_input.LA(1)) {
					case CO:
						{
						setState(292);
						match(CO);
						setState(293);
						((VariablesContext)_localctx).valueExprN = expression(0);
						 if (initCounter < names.size) { valueExpressions[initCounter++] = ((VariablesContext)_localctx).valueExprN.tree; }; 
						}
						break;
					case T__2:
						{
						setState(296);
						linePreprocessorEntry();
						}
						break;
					default:
						throw new NoViableAltException(this);
					}
					}
					setState(301);
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
		        TransientArray<Annotation*> annotations(32);
		        TransientArray<Attribute*> attributes(32);
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(309);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__2) {
				{
				{
				setState(306);
				linePreprocessorEntry();
				}
				}
				setState(311);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(317);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==SOBAKA) {
				{
				{
				setState(312);
				((StructureDeclarationContext)_localctx).annotation = annotation();
				 if (annotations.Full()) { throw IndexOutOfBoundsException("Maximum of 32 annotations reached"); } annotations.Append(std::move(((StructureDeclarationContext)_localctx).annotation.annot)); 
				}
				}
				setState(319);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(325);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==IDENTIFIER) {
				{
				{
				setState(320);
				((StructureDeclarationContext)_localctx).attribute = attribute();
				 if (attributes.Full()) { throw IndexOutOfBoundsException("Maximum of 32 attributes reached"); } attributes.Append(std::move(((StructureDeclarationContext)_localctx).attribute.attr)); 
				}
				}
				setState(327);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(328);
			match(T__6);
			setState(329);
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
			setState(332);
			((StructureContext)_localctx).structureDeclaration = structureDeclaration();
			 ((StructureContext)_localctx).sym =  ((StructureContext)_localctx).structureDeclaration.sym; 
			setState(334);
			match(LB);
			setState(361);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__2 || _la==IDENTIFIER) {
				{
				setState(359);
				_errHandler.sync(this);
				switch (_input.LA(1)) {
				case IDENTIFIER:
					{
					setState(335);
					((StructureContext)_localctx).varName = match(IDENTIFIER);
					 varName = FixedString((((StructureContext)_localctx).varName!=null?((StructureContext)_localctx).varName.getText():null)); varLocation = SetupFile(); 
					setState(337);
					match(COL);
					 typeRange = BeginLocationRange(); 
					setState(351);
					_errHandler.sync(this);
					_la = _input.LA(1);
					while (_la==LL || _la==MUL_OP) {
						{
						setState(349);
						_errHandler.sync(this);
						switch (_input.LA(1)) {
						case MUL_OP:
							{
							setState(339);
							match(MUL_OP);
							 varType.AddModifier(Type::FullType::Modifier::Pointer); 
							}
							break;
						case LL:
							{
							setState(341);
							match(LL);
							 varType.AddModifier(Type::FullType::Modifier::Array); 
							setState(346);
							_errHandler.sync(this);
							_la = _input.LA(1);
							if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 1732271774273372174L) != 0) || ((((_la - 74)) & ~0x3f) == 0 && ((1L << (_la - 74)) & 7787L) != 0)) {
								{
								setState(343);
								((StructureContext)_localctx).arraySize0 = expression(0);
								 varType.UpdateValue(((StructureContext)_localctx).arraySize0.tree); 
								}
							}

							setState(348);
							match(RR);
							}
							break;
						default:
							throw new NoViableAltException(this);
						}
						}
						setState(353);
						_errHandler.sync(this);
						_la = _input.LA(1);
					}
					setState(354);
					((StructureContext)_localctx).varTypeName = match(IDENTIFIER);
					 if (members.Full()) { throw IndexOutOfBoundsException("Maximum of 1024 struct members reached"); } varType.name = (((StructureContext)_localctx).varTypeName!=null?((StructureContext)_localctx).varTypeName.getText():null); varTypeLocation = EndLocationRange(typeRange); 
					setState(356);
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
					setState(358);
					linePreprocessorEntry();
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				}
				setState(363);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(364);
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
		        TransientArray<FixedString> enumLabels(256);
		        TransientArray<Expression*> enumValues(256);
		        TransientArray<Symbol::Location> enumLocations(256);
		        FixedString name;
		        TypeDeclaration type = TypeDeclaration{ .type = Type::FullType{"u32"} };
		        Symbol::Location location;
		        Symbol::Location labelLocation;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(367);
			match(T__7);
			setState(368);
			((EnumerationContext)_localctx).name = match(IDENTIFIER);
			 name = FixedString((((EnumerationContext)_localctx).name!=null?((EnumerationContext)_localctx).name.getText():null)); location = SetupFile(); 
			setState(374);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COL) {
				{
				setState(370);
				match(COL);
				setState(371);
				((EnumerationContext)_localctx).typeDeclaration = typeDeclaration();
				 type = ((EnumerationContext)_localctx).typeDeclaration.type; 
				}
			}

			setState(376);
			match(LB);
			setState(406);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case IDENTIFIER:
				{
				setState(377);
				((EnumerationContext)_localctx).label = match(IDENTIFIER);
				 Expression* expr = nullptr; labelLocation = SetupFile(); 
				setState(383);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==EQ) {
					{
					setState(379);
					match(EQ);
					setState(380);
					((EnumerationContext)_localctx).value = expression(0);
					 expr = ((EnumerationContext)_localctx).value.tree; 
					}
				}


				                enumLabels.Append(FixedString((((EnumerationContext)_localctx).label!=null?((EnumerationContext)_localctx).label.getText():null)));
				                enumValues.Append(expr);
				                enumLocations.Append(labelLocation);
				            
				setState(387);
				_errHandler.sync(this);
				switch ( getInterpreter().adaptivePredict(_input,34,_ctx) ) {
				case 1:
					{
					setState(386);
					linePreprocessorEntry();
					}
					break;
				}
				setState(402);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while (_la==T__2 || _la==CO) {
					{
					setState(400);
					_errHandler.sync(this);
					switch (_input.LA(1)) {
					case CO:
						{
						setState(389);
						match(CO);
						setState(390);
						((EnumerationContext)_localctx).label = match(IDENTIFIER);
						 if (enumLabels.Full()) { throw IndexOutOfBoundsException("Maximum of 256 enum labels"); } Expression* expr = nullptr; labelLocation = SetupFile(); 
						setState(396);
						_errHandler.sync(this);
						_la = _input.LA(1);
						if (_la==EQ) {
							{
							setState(392);
							match(EQ);
							setState(393);
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
						setState(399);
						linePreprocessorEntry();
						}
						break;
					default:
						throw new NoViableAltException(this);
					}
					}
					setState(404);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				}
				break;
			case T__2:
				{
				setState(405);
				linePreprocessorEntry();
				}
				break;
			case RB:
				break;
			default:
				break;
			}
			setState(408);
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
		enterRule(_localctx, 30, RULE_parameter);

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
			setState(414);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__2) {
				{
				{
				setState(411);
				linePreprocessorEntry();
				}
				}
				setState(416);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(422);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,40,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					{
					{
					setState(417);
					((ParameterContext)_localctx).attribute = attribute();
					 if (attributes.Full()) { throw IndexOutOfBoundsException("Maximum of 32 attributes reached"); } attributes.Append(std::move(((ParameterContext)_localctx).attribute.attr)); 
					}
					} 
				}
				setState(424);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,40,_ctx);
			}
			setState(425);
			((ParameterContext)_localctx).varName = match(IDENTIFIER);
			 name = FixedString((((ParameterContext)_localctx).varName!=null?((ParameterContext)_localctx).varName.getText():null)); location = SetupFile(); 
			setState(427);
			match(COL);
			setState(428);
			((ParameterContext)_localctx).typeDeclaration = typeDeclaration();
			 type = ((ParameterContext)_localctx).typeDeclaration.type; 
			setState(434);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==EQ) {
				{
				setState(430);
				match(EQ);
				setState(431);
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
		enterRule(_localctx, 32, RULE_functionDeclaration);

		        ((FunctionDeclarationContext)_localctx).sym =  nullptr;
		        TransientArray<Variable*> variables(32);
		        TransientArray<Attribute*> attributes(32);
		        Symbol::Location location;
		    
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(443);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,42,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					{
					{
					setState(438);
					((FunctionDeclarationContext)_localctx).attribute = attribute();
					 if (attributes.Full()) { throw IndexOutOfBoundsException("Maximum of 32 attributes reached"); } attributes.Append(std::move(((FunctionDeclarationContext)_localctx).attribute.attr)); 
					}
					} 
				}
				setState(445);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,42,_ctx);
			}
			setState(446);
			((FunctionDeclarationContext)_localctx).name = match(IDENTIFIER);
			 location = SetupFile(); 
			setState(448);
			match(LP);
			setState(464);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__2 || _la==IDENTIFIER) {
				{
				setState(449);
				((FunctionDeclarationContext)_localctx).arg0 = parameter();
				 variables.Append(((FunctionDeclarationContext)_localctx).arg0.sym); 
				setState(452);
				_errHandler.sync(this);
				switch ( getInterpreter().adaptivePredict(_input,43,_ctx) ) {
				case 1:
					{
					setState(451);
					linePreprocessorEntry();
					}
					break;
				}
				setState(461);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while (_la==T__2 || _la==CO) {
					{
					setState(459);
					_errHandler.sync(this);
					switch (_input.LA(1)) {
					case CO:
						{
						setState(454);
						match(CO);
						setState(455);
						((FunctionDeclarationContext)_localctx).argn = parameter();
						 if (variables.Full()) throw IndexOutOfBoundsException("Maximum of 32 variables reached"); variables.Append(((FunctionDeclarationContext)_localctx).argn.sym); 
						}
						break;
					case T__2:
						{
						setState(458);
						linePreprocessorEntry();
						}
						break;
					default:
						throw new NoViableAltException(this);
					}
					}
					setState(463);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				}
			}

			setState(466);
			match(RP);
			setState(467);
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
			setState(470);
			((FunctionContext)_localctx).functionDeclaration = functionDeclaration();
			 ((FunctionContext)_localctx).sym =  ((FunctionContext)_localctx).functionDeclaration.sym; 
			setState(472);
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
		        TransientArray<Expression*> entries(32);
		        TransientArray<Annotation*> annotations(32);
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(480);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==SOBAKA) {
				{
				{
				setState(475);
				((ProgramContext)_localctx).annotation = annotation();
				 if (annotations.Full()) { throw IndexOutOfBoundsException("Maximum of 32 annotations reached"); } annotations.Append(std::move(((ProgramContext)_localctx).annotation.annot)); 
				}
				}
				setState(482);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(483);
			match(T__8);
			 ((ProgramContext)_localctx).sym =  Alloc<Program>(); 
			setState(485);
			((ProgramContext)_localctx).name = match(IDENTIFIER);
			 _localctx.sym->location = SetupFile(); 
			setState(487);
			match(LB);
			setState(494);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & 1732271774273372174L) != 0) || ((((_la - 74)) & ~0x3f) == 0 && ((1L << (_la - 74)) & 7787L) != 0)) {
				{
				{
				setState(488);
				((ProgramContext)_localctx).assignment = expression(0);
				 if (entries.Full()) throw IndexOutOfBoundsException("Maximum of 32 entries reached"); entries.Append(((ProgramContext)_localctx).assignment.tree); 
				setState(490);
				match(SC);
				}
				}
				setState(496);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(497);
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

		        TransientArray<Attribute*> attributes(32);
		        TransientArray<Annotation*> annotations(32);
		        TransientArray<Expression*> entries(32);
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			{
			setState(505);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==SOBAKA) {
				{
				{
				setState(500);
				((SamplerContext)_localctx).annotation = annotation();
				 if (annotations.Full()) throw IndexOutOfBoundsException("Maximum of 32 annotations reached"); annotations.Append(std::move(((SamplerContext)_localctx).annotation.annot)); 
				}
				}
				setState(507);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(513);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==IDENTIFIER) {
				{
				{
				setState(508);
				((SamplerContext)_localctx).attribute = attribute();
				 if (attributes.Full()) throw IndexOutOfBoundsException("Maximum of 32 attributes reached"); attributes.Append(std::move(((SamplerContext)_localctx).attribute.attr)); 
				}
				}
				setState(515);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(516);
			match(T__9);
			 ((SamplerContext)_localctx).sym =  Alloc<SamplerState>(); _localctx.sym->isImmutable = true; 
			}
			setState(519);
			((SamplerContext)_localctx).name = match(IDENTIFIER);
			 _localctx.sym->location = SetupFile(); 
			setState(521);
			match(LB);
			setState(528);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & 1732271774273372174L) != 0) || ((((_la - 74)) & ~0x3f) == 0 && ((1L << (_la - 74)) & 7787L) != 0)) {
				{
				{
				setState(522);
				((SamplerContext)_localctx).assign = expression(0);
				 if (entries.Full()) throw IndexOutOfBoundsException("Maximum of 32 entries reached"); entries.Append(((SamplerContext)_localctx).assign.tree); 
				setState(524);
				match(SC);
				}
				}
				setState(530);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(531);
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

		        TransientArray<Expression*> entries(32);
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			{
			setState(534);
			match(T__10);
			 ((StateContext)_localctx).sym =  Alloc<RenderState>(); 
			}
			setState(537);
			((StateContext)_localctx).name = match(IDENTIFIER);
			 _localctx.sym->location = SetupFile(); 
			setState(539);
			match(LB);
			setState(546);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & 1732271774273372174L) != 0) || ((((_la - 74)) & ~0x3f) == 0 && ((1L << (_la - 74)) & 7787L) != 0)) {
				{
				{
				setState(540);
				((StateContext)_localctx).assign = expression(0);
				 if (entries.Full()) throw IndexOutOfBoundsException("Maximum of 32 entries reached"); entries.Append(((StateContext)_localctx).assign.tree); 
				setState(542);
				match(SC);
				}
				}
				setState(548);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(549);
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
			setState(580);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__11:
				enterOuterAlt(_localctx, 1);
				{
				setState(552);
				((StatementContext)_localctx).ifStatement = ifStatement();
				 ((StatementContext)_localctx).tree =  ((StatementContext)_localctx).ifStatement.tree; 
				}
				break;
			case LB:
				enterOuterAlt(_localctx, 2);
				{
				setState(555);
				((StatementContext)_localctx).scopeStatement = scopeStatement();
				 ((StatementContext)_localctx).tree =  ((StatementContext)_localctx).scopeStatement.tree; 
				}
				break;
			case T__13:
				enterOuterAlt(_localctx, 3);
				{
				setState(558);
				((StatementContext)_localctx).forStatement = forStatement();
				 ((StatementContext)_localctx).tree =  ((StatementContext)_localctx).forStatement.tree; 
				}
				break;
			case T__16:
			case T__17:
				enterOuterAlt(_localctx, 4);
				{
				setState(561);
				((StatementContext)_localctx).whileStatement = whileStatement();
				 ((StatementContext)_localctx).tree =  ((StatementContext)_localctx).whileStatement.tree; 
				}
				break;
			case T__23:
				enterOuterAlt(_localctx, 5);
				{
				setState(564);
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
				setState(567);
				((StatementContext)_localctx).terminateStatement = terminateStatement();
				 ((StatementContext)_localctx).tree =  ((StatementContext)_localctx).terminateStatement.tree; 
				}
				break;
			case T__22:
				enterOuterAlt(_localctx, 7);
				{
				setState(570);
				((StatementContext)_localctx).continueStatement = continueStatement();
				 ((StatementContext)_localctx).tree =  ((StatementContext)_localctx).continueStatement.tree; 
				}
				break;
			case T__26:
				enterOuterAlt(_localctx, 8);
				{
				setState(573);
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
				setState(576);
				((StatementContext)_localctx).expressionStatement = expressionStatement();
				setState(577);
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
		enterRule(_localctx, 44, RULE_expressionStatement);

		        ((ExpressionStatementContext)_localctx).tree =  nullptr;
		    
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(582);
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
		enterRule(_localctx, 46, RULE_ifStatement);

		        ((IfStatementContext)_localctx).tree =  nullptr;
		        Expression* condition = nullptr;
		        Statement* ifBody = nullptr;
		        Statement* elseBody = nullptr;
		        Symbol::Location location;
		    
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(585);
			match(T__11);
			 location = SetupFile(); 
			setState(587);
			match(LP);
			setState(588);
			((IfStatementContext)_localctx).condition = expression(0);
			 condition = ((IfStatementContext)_localctx).condition.tree; 
			setState(590);
			match(RP);
			setState(591);
			((IfStatementContext)_localctx).ifBody = statement();
			 ifBody = ((IfStatementContext)_localctx).ifBody.tree; 
			setState(597);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,54,_ctx) ) {
			case 1:
				{
				setState(593);
				match(T__12);
				setState(594);
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
		        TransientArray<Attribute*> attributes(32);
		        Symbol::Location location;
		    
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(601);
			match(T__13);
			 location = SetupFile(); 
			setState(603);
			match(LP);
			setState(607);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__2 || _la==SOBAKA || _la==IDENTIFIER) {
				{
				setState(604);
				((ForStatementContext)_localctx).variables = variables();
				 declarations = ((ForStatementContext)_localctx).variables.vars; 
				}
			}

			setState(609);
			match(SC);
			setState(613);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 1732271774273372174L) != 0) || ((((_la - 74)) & ~0x3f) == 0 && ((1L << (_la - 74)) & 7787L) != 0)) {
				{
				setState(610);
				((ForStatementContext)_localctx).condition = expression(0);
				 conditionExpression = ((ForStatementContext)_localctx).condition.tree; 
				}
			}

			setState(615);
			match(SC);
			setState(619);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 1732271774273372174L) != 0) || ((((_la - 74)) & ~0x3f) == 0 && ((1L << (_la - 74)) & 7787L) != 0)) {
				{
				setState(616);
				((ForStatementContext)_localctx).loop = expression(0);
				 loopExpression = ((ForStatementContext)_localctx).loop.tree; 
				}
			}

			setState(621);
			match(RP);
			setState(627);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,58,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					{
					{
					setState(622);
					((ForStatementContext)_localctx).attribute = attribute();
					 if (attributes.Full()) { throw IndexOutOfBoundsException("Maximum of 32 attributes reached"); } attributes.Append(std::move(((ForStatementContext)_localctx).attribute.attr)); 
					}
					} 
				}
				setState(629);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,58,_ctx);
			}
			setState(630);
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
			setState(634);
			match(T__13);
			 location = SetupFile(); 
			setState(636);
			match(LP);
			setState(637);
			((ForRangeStatementContext)_localctx).iterator = match(IDENTIFIER);
			setState(638);
			match(COL);
			setState(639);
			((ForRangeStatementContext)_localctx).start = match(IDENTIFIER);
			setState(640);
			match(T__14);
			setState(641);
			((ForRangeStatementContext)_localctx).end = match(IDENTIFIER);
			setState(642);
			match(RP);
			setState(643);
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
			setState(647);
			match(T__15);
			 location = SetupFile(); 
			setState(649);
			match(LP);
			setState(650);
			expression(0);
			setState(651);
			match(RP);
			setState(652);
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
			setState(678);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__16:
				enterOuterAlt(_localctx, 1);
				{
				setState(656);
				match(T__16);
				 location = SetupFile(); 
				setState(658);
				match(LP);
				setState(659);
				((WhileStatementContext)_localctx).condition = expression(0);
				 conditionExpression = ((WhileStatementContext)_localctx).condition.tree; 
				setState(661);
				match(RP);
				setState(662);
				((WhileStatementContext)_localctx).content = statement();
				 contents = ((WhileStatementContext)_localctx).content.tree; 

				        ((WhileStatementContext)_localctx).tree =  Alloc<WhileStatement>(conditionExpression, contents, isDoWhile);
				        _localctx.tree->location = location;
				    
				}
				break;
			case T__17:
				enterOuterAlt(_localctx, 2);
				{
				setState(666);
				match(T__17);
				 location = SetupFile(); 
				setState(668);
				((WhileStatementContext)_localctx).content = statement();
				 contents = ((WhileStatementContext)_localctx).content.tree; isDoWhile = true; 
				setState(670);
				match(T__16);
				setState(671);
				match(LP);
				setState(672);
				((WhileStatementContext)_localctx).condition = expression(0);
				 conditionExpression = ((WhileStatementContext)_localctx).condition.tree; 
				setState(674);
				match(RP);
				setState(675);
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
			setState(680);
			match(LB);
			 location = SetupFile(); 
			setState(692);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & 1732289366627078158L) != 0) || ((((_la - 73)) & ~0x3f) == 0 && ((1L << (_la - 73)) & 15575L) != 0)) {
				{
				setState(690);
				_errHandler.sync(this);
				switch ( getInterpreter().adaptivePredict(_input,60,_ctx) ) {
				case 1:
					{
					setState(682);
					((ScopeStatementContext)_localctx).variables = variables();
					setState(683);
					match(SC);
					 for(Variable* var : ((ScopeStatementContext)_localctx).variables.vars) { contents.Append(var); } 
					}
					break;
				case 2:
					{
					setState(686);
					((ScopeStatementContext)_localctx).statement = statement();
					 contents.Append(((ScopeStatementContext)_localctx).statement.tree); 
					}
					break;
				case 3:
					{
					setState(689);
					linePreprocessorEntry();
					}
					break;
				}
				}
				setState(694);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(695);
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
			setState(720);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__18:
				enterOuterAlt(_localctx, 1);
				{
				setState(699);
				match(T__18);
				 location = SetupFile(); 
				setState(704);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 1732271774273372174L) != 0) || ((((_la - 74)) & ~0x3f) == 0 && ((1L << (_la - 74)) & 7787L) != 0)) {
					{
					setState(701);
					((TerminateStatementContext)_localctx).value = expression(0);
					 returnValue = ((TerminateStatementContext)_localctx).value.tree; 
					}
				}

				setState(706);
				match(SC);

				        ((TerminateStatementContext)_localctx).tree =  Alloc<TerminateStatement>(returnValue, TerminateStatement::TerminationType::Return);
				        _localctx.tree->location = location;
				    
				}
				break;
			case T__19:
				enterOuterAlt(_localctx, 2);
				{
				setState(708);
				match(T__19);
				 location = SetupFile(); 
				setState(710);
				match(SC);

				      ((TerminateStatementContext)_localctx).tree =  Alloc<TerminateStatement>(returnValue, TerminateStatement::TerminationType::Discard);
				      _localctx.tree->location = location;
				    
				}
				break;
			case T__20:
				enterOuterAlt(_localctx, 3);
				{
				setState(712);
				match(T__20);
				 location = SetupFile(); 
				setState(714);
				match(SC);

				      ((TerminateStatementContext)_localctx).tree =  Alloc<TerminateStatement>(returnValue, TerminateStatement::TerminationType::RayIgnoreIntersection);
				      _localctx.tree->location = location;
				    
				}
				break;
			case T__21:
				enterOuterAlt(_localctx, 4);
				{
				setState(716);
				match(T__21);
				 location = SetupFile(); 
				setState(718);
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
			setState(722);
			match(T__22);
			 location = SetupFile(); 
			setState(724);
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
		        TransientArray<Expression*> caseExpressions(256);
		        TransientArray<Statement*> caseStatements(256);
		        Symbol::Location location;
		        Statement* defaultStatement = nullptr;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(727);
			match(T__23);
			 location = SetupFile(); 
			setState(729);
			match(LP);
			setState(730);
			((SwitchStatementContext)_localctx).expression = expression(0);
			setState(731);
			match(RP);
			 switchExpression = ((SwitchStatementContext)_localctx).expression.tree; 
			setState(733);
			match(LB);
			setState(745);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__24) {
				{
				{
				setState(734);
				match(T__24);
				setState(735);
				((SwitchStatementContext)_localctx).expression = expression(0);
				setState(736);
				match(COL);
				 if (caseExpressions.Full()) { throw IndexOutOfBoundsException("Maximum of 256 case expressions reached"); } caseExpressions.Append(((SwitchStatementContext)_localctx).expression.tree); caseStatements.Append(nullptr); 
				setState(741);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 1732289366627078158L) != 0) || ((((_la - 74)) & ~0x3f) == 0 && ((1L << (_la - 74)) & 7787L) != 0)) {
					{
					setState(738);
					((SwitchStatementContext)_localctx).statement = statement();
					 
					                    caseStatements.back() = ((SwitchStatementContext)_localctx).statement.tree;
					                
					}
				}

				}
				}
				setState(747);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(755);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__25) {
				{
				setState(748);
				match(T__25);
				setState(749);
				match(COL);
				setState(753);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 1732289366627078158L) != 0) || ((((_la - 74)) & ~0x3f) == 0 && ((1L << (_la - 74)) & 7787L) != 0)) {
					{
					setState(750);
					((SwitchStatementContext)_localctx).statement = statement();

					                    defaultStatement = ((SwitchStatementContext)_localctx).statement.tree;
					                
					}
				}

				}
			}

			setState(757);
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
			setState(760);
			match(T__26);
			 location = SetupFile(); 
			setState(762);
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
		enterRule(_localctx, 66, RULE_expressionList);

		        TransientArray<Expression*> list(256);
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(766);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,68,_ctx) ) {
			case 1:
				{
				setState(765);
				linePreprocessorEntry();
				}
				break;
			}
			setState(768);
			((ExpressionListContext)_localctx).e1 = expression(0);
			 list.Append(((ExpressionListContext)_localctx).e1.tree); 
			setState(777);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__2 || _la==CO) {
				{
				setState(775);
				_errHandler.sync(this);
				switch (_input.LA(1)) {
				case CO:
					{
					setState(770);
					match(CO);
					setState(771);
					((ExpressionListContext)_localctx).e2 = expression(0);

					        list.Append(((ExpressionListContext)_localctx).e2.tree);
					    
					}
					break;
				case T__2:
					{
					setState(774);
					linePreprocessorEntry();
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				}
				setState(779);
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
		int _startState = 68;
		enterRecursionRule(_localctx, 68, RULE_expression, _p);

		        ((ExpressionContext)_localctx).tree =  nullptr;
		        Symbol::Location location;
		        FixedArray<Expression*> args;
		    
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(790);
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
				setState(783);
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
				setState(784);
				((ExpressionContext)_localctx).p = expression(14);

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
				setState(787);
				((ExpressionContext)_localctx).atom = binaryexpatom();
				 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).atom.tree; 
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
			_ctx.stop = _input.LT(-1);
			setState(918);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,75,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					if ( _parseListeners!=null ) triggerExitRuleEvent();
					_prevctx = _localctx;
					{
					setState(916);
					_errHandler.sync(this);
					switch ( getInterpreter().adaptivePredict(_input,74,_ctx) ) {
					case 1:
						{
						_localctx = new ExpressionContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expression);
						setState(792);
						if (!(precpred(_ctx, 17))) throw new FailedPredicateException(this, "precpred(_ctx, 17)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(794);
						match(DOT);
						 location = SetupFile(); 
						setState(796);
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
						setState(799);
						if (!(precpred(_ctx, 16))) throw new FailedPredicateException(this, "precpred(_ctx, 16)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(801);
						match(ARROW);
						 location = SetupFile(); 
						setState(803);
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
						setState(806);
						if (!(precpred(_ctx, 13))) throw new FailedPredicateException(this, "precpred(_ctx, 13)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(808);
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
						setState(810);
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
						setState(813);
						if (!(precpred(_ctx, 12))) throw new FailedPredicateException(this, "precpred(_ctx, 12)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(815);
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
						setState(817);
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
						setState(820);
						if (!(precpred(_ctx, 11))) throw new FailedPredicateException(this, "precpred(_ctx, 11)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(822);
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
						setState(824);
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
						setState(827);
						if (!(precpred(_ctx, 10))) throw new FailedPredicateException(this, "precpred(_ctx, 10)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(829);
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
						setState(831);
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
						setState(834);
						if (!(precpred(_ctx, 9))) throw new FailedPredicateException(this, "precpred(_ctx, 9)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(836);
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
						setState(838);
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
						setState(841);
						if (!(precpred(_ctx, 8))) throw new FailedPredicateException(this, "precpred(_ctx, 8)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(843);
						match(AND);
						 location = SetupFile(); 
						setState(845);
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
						setState(848);
						if (!(precpred(_ctx, 7))) throw new FailedPredicateException(this, "precpred(_ctx, 7)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(850);
						match(XOR);
						 location = SetupFile(); 
						setState(852);
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
						setState(855);
						if (!(precpred(_ctx, 6))) throw new FailedPredicateException(this, "precpred(_ctx, 6)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(857);
						match(OR);
						 location = SetupFile(); 
						setState(859);
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
						setState(862);
						if (!(precpred(_ctx, 5))) throw new FailedPredicateException(this, "precpred(_ctx, 5)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(864);
						match(LOGICAND);
						 location = SetupFile(); 
						setState(866);
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
						setState(869);
						if (!(precpred(_ctx, 4))) throw new FailedPredicateException(this, "precpred(_ctx, 4)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(871);
						match(LOGICOR);
						 location = SetupFile(); 
						setState(873);
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
						setState(876);
						if (!(precpred(_ctx, 3))) throw new FailedPredicateException(this, "precpred(_ctx, 3)");
						setState(877);
						match(QU);
						 location = SetupFile(); 
						setState(879);
						((ExpressionContext)_localctx).ifBody = expression(0);
						setState(880);
						match(COL);
						setState(881);
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
						setState(884);
						if (!(precpred(_ctx, 2))) throw new FailedPredicateException(this, "precpred(_ctx, 2)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(886);
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
						setState(888);
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
						setState(891);
						if (!(precpred(_ctx, 19))) throw new FailedPredicateException(this, "precpred(_ctx, 19)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(893);
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
						setState(896);
						if (!(precpred(_ctx, 18))) throw new FailedPredicateException(this, "precpred(_ctx, 18)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(898);
						match(LP);
						 location = SetupFile(); 
						setState(903);
						_errHandler.sync(this);
						_la = _input.LA(1);
						if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 1732271774273372174L) != 0) || ((((_la - 74)) & ~0x3f) == 0 && ((1L << (_la - 74)) & 7787L) != 0)) {
							{
							setState(900);
							((ExpressionContext)_localctx).list = expressionList();
							 args = std::move(((ExpressionContext)_localctx).list.expressions); 
							}
						}

						setState(905);
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
						setState(907);
						if (!(precpred(_ctx, 15))) throw new FailedPredicateException(this, "precpred(_ctx, 15)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(909);
						match(LL);
						 location = SetupFile(); 
						setState(912);
						_errHandler.sync(this);
						_la = _input.LA(1);
						if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 1732271774273372174L) != 0) || ((((_la - 74)) & ~0x3f) == 0 && ((1L << (_la - 74)) & 7787L) != 0)) {
							{
							setState(911);
							((ExpressionContext)_localctx).e3 = expression(0);
							}
						}

						setState(914);
						match(RR);

						                  ArrayIndexExpression* expr = Alloc<ArrayIndexExpression>(_localctx.tree, ((ExpressionContext)_localctx).e3.tree);
						                  expr->location = ((ExpressionContext)_localctx).e1.tree->location;
						                  ((ExpressionContext)_localctx).tree =  expr;
						              
						}
						break;
					}
					} 
				}
				setState(920);
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
		enterRule(_localctx, 70, RULE_binaryexpatom);

		        ((BinaryexpatomContext)_localctx).tree =  nullptr;
		        Symbol::Location begin = BeginLocationRange();
		    
		try {
			setState(951);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,76,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(921);
				((BinaryexpatomContext)_localctx).initializerExpression = initializerExpression();
				 ((BinaryexpatomContext)_localctx).tree =  ((BinaryexpatomContext)_localctx).initializerExpression.tree; 
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(924);
				((BinaryexpatomContext)_localctx).arrayInitializerExpression = arrayInitializerExpression();
				 ((BinaryexpatomContext)_localctx).tree =  ((BinaryexpatomContext)_localctx).arrayInitializerExpression.tree; 
				}
				break;
			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(927);
				match(LP);
				setState(928);
				((BinaryexpatomContext)_localctx).expression = expression(0);
				setState(929);
				match(RP);
				 ((BinaryexpatomContext)_localctx).tree =  ((BinaryexpatomContext)_localctx).expression.tree; 
				}
				break;
			case 4:
				enterOuterAlt(_localctx, 4);
				{
				setState(932);
				((BinaryexpatomContext)_localctx).INTEGERLITERAL = match(INTEGERLITERAL);
				 ((BinaryexpatomContext)_localctx).tree =  Alloc<IntExpression>(atoi((((BinaryexpatomContext)_localctx).INTEGERLITERAL!=null?((BinaryexpatomContext)_localctx).INTEGERLITERAL.getText():null).c_str())); _localctx.tree->location = SetupFile(); 
				}
				break;
			case 5:
				enterOuterAlt(_localctx, 5);
				{
				setState(934);
				((BinaryexpatomContext)_localctx).UINTEGERLITERAL = match(UINTEGERLITERAL);
				 ((BinaryexpatomContext)_localctx).tree =  Alloc<UIntExpression>(strtoul((((BinaryexpatomContext)_localctx).UINTEGERLITERAL!=null?((BinaryexpatomContext)_localctx).UINTEGERLITERAL.getText():null).c_str(), nullptr, 10)); _localctx.tree->location = SetupFile(); 
				}
				break;
			case 6:
				enterOuterAlt(_localctx, 6);
				{
				setState(936);
				((BinaryexpatomContext)_localctx).FLOATLITERAL = match(FLOATLITERAL);
				 ((BinaryexpatomContext)_localctx).tree =  Alloc<FloatExpression>(atof((((BinaryexpatomContext)_localctx).FLOATLITERAL!=null?((BinaryexpatomContext)_localctx).FLOATLITERAL.getText():null).c_str())); _localctx.tree->location = SetupFile(); 
				}
				break;
			case 7:
				enterOuterAlt(_localctx, 7);
				{
				setState(938);
				((BinaryexpatomContext)_localctx).DOUBLELITERAL = match(DOUBLELITERAL);
				 ((BinaryexpatomContext)_localctx).tree =  Alloc<FloatExpression>(atof((((BinaryexpatomContext)_localctx).DOUBLELITERAL!=null?((BinaryexpatomContext)_localctx).DOUBLELITERAL.getText():null).c_str())); _localctx.tree->location = SetupFile(); 
				}
				break;
			case 8:
				enterOuterAlt(_localctx, 8);
				{
				setState(940);
				((BinaryexpatomContext)_localctx).HEX = match(HEX);
				 ((BinaryexpatomContext)_localctx).tree =  Alloc<UIntExpression>(strtoul((((BinaryexpatomContext)_localctx).HEX!=null?((BinaryexpatomContext)_localctx).HEX.getText():null).c_str(), nullptr, 16)); _localctx.tree->location = SetupFile(); 
				}
				break;
			case 9:
				enterOuterAlt(_localctx, 9);
				{
				setState(942);
				((BinaryexpatomContext)_localctx).string = string();
				 ((BinaryexpatomContext)_localctx).tree =  Alloc<StringExpression>(((BinaryexpatomContext)_localctx).string.val); _localctx.tree->location = EndLocationRange(begin); 
				}
				break;
			case 10:
				enterOuterAlt(_localctx, 10);
				{
				setState(945);
				((BinaryexpatomContext)_localctx).IDENTIFIER = match(IDENTIFIER);
				 ((BinaryexpatomContext)_localctx).tree =  Alloc<SymbolExpression>(FixedString((((BinaryexpatomContext)_localctx).IDENTIFIER!=null?((BinaryexpatomContext)_localctx).IDENTIFIER.getText():null))); _localctx.tree->location = SetupFile(); 
				}
				break;
			case 11:
				enterOuterAlt(_localctx, 11);
				{
				setState(947);
				((BinaryexpatomContext)_localctx).boolean_ = boolean_();
				 ((BinaryexpatomContext)_localctx).tree =  Alloc<BoolExpression>(((BinaryexpatomContext)_localctx).boolean.val); _localctx.tree->location = SetupFile(); 
				}
				break;
			case 12:
				enterOuterAlt(_localctx, 12);
				{
				setState(950);
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
		enterRule(_localctx, 72, RULE_initializerExpression);

		        ((InitializerExpressionContext)_localctx).tree =  nullptr;
		        FixedString type;
		        Symbol::Location location;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(953);
			((InitializerExpressionContext)_localctx).type = match(IDENTIFIER);
			 type = FixedString((((InitializerExpressionContext)_localctx).type!=null?((InitializerExpressionContext)_localctx).type.getText():null)); 
			setState(955);
			match(LB);
			 location = SetupFile(); 
			setState(958);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 1732271774273372174L) != 0) || ((((_la - 74)) & ~0x3f) == 0 && ((1L << (_la - 74)) & 7787L) != 0)) {
				{
				setState(957);
				((InitializerExpressionContext)_localctx).list = expressionList();
				}
			}

			setState(960);
			match(RB);

			        ((InitializerExpressionContext)_localctx).tree =  Alloc<InitializerExpression>(((InitializerExpressionContext)_localctx).list.expressions, type);
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
		enterRule(_localctx, 74, RULE_arrayInitializerExpression);

		        ((ArrayInitializerExpressionContext)_localctx).tree =  nullptr;
		        Symbol::Location location;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(963);
			match(LL);
			 location = SetupFile(); 
			setState(966);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 1732271774273372174L) != 0) || ((((_la - 74)) & ~0x3f) == 0 && ((1L << (_la - 74)) & 7787L) != 0)) {
				{
				setState(965);
				((ArrayInitializerExpressionContext)_localctx).list = expressionList();
				}
			}

			setState(968);
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
		enterRule(_localctx, 76, RULE_floatVecLiteralExpression);

		        ((FloatVecLiteralExpressionContext)_localctx).tree =  nullptr;
		        TransientArray<float> values(32);
		        Symbol::Location location;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(971);
			match(LESS);
			 location = SetupFile(); 
			{
			setState(973);
			((FloatVecLiteralExpressionContext)_localctx).arg0 = match(FLOATLITERAL);
			 values.Append(atof((((FloatVecLiteralExpressionContext)_localctx).arg0!=null?((FloatVecLiteralExpressionContext)_localctx).arg0.getText():null).c_str())); 
			}
			setState(977);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__2) {
				{
				setState(976);
				linePreprocessorEntry();
				}
			}

			setState(982); 
			_errHandler.sync(this);
			_la = _input.LA(1);
			do {
				{
				{
				setState(979);
				match(CO);
				setState(980);
				((FloatVecLiteralExpressionContext)_localctx).argN = match(FLOATLITERAL);
				 values.Append(atof((((FloatVecLiteralExpressionContext)_localctx).argN!=null?((FloatVecLiteralExpressionContext)_localctx).argN.getText():null).c_str())); 
				}
				}
				setState(984); 
				_errHandler.sync(this);
				_la = _input.LA(1);
			} while ( _la==CO );
			setState(986);
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
		enterRule(_localctx, 78, RULE_doubleVecLiteralExpression);

		        ((DoubleVecLiteralExpressionContext)_localctx).tree =  nullptr;
		        TransientArray<float> values(32);
		        Symbol::Location location;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(989);
			match(LESS);
			 location = SetupFile(); 
			{
			setState(991);
			((DoubleVecLiteralExpressionContext)_localctx).arg0 = match(DOUBLELITERAL);
			 values.Append(atof((((DoubleVecLiteralExpressionContext)_localctx).arg0!=null?((DoubleVecLiteralExpressionContext)_localctx).arg0.getText():null).c_str())); 
			}
			setState(995);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__2) {
				{
				setState(994);
				linePreprocessorEntry();
				}
			}

			setState(1000); 
			_errHandler.sync(this);
			_la = _input.LA(1);
			do {
				{
				{
				setState(997);
				match(CO);
				setState(998);
				((DoubleVecLiteralExpressionContext)_localctx).argN = match(DOUBLELITERAL);
				 values.Append(atof((((DoubleVecLiteralExpressionContext)_localctx).argN!=null?((DoubleVecLiteralExpressionContext)_localctx).argN.getText():null).c_str())); 
				}
				}
				setState(1002); 
				_errHandler.sync(this);
				_la = _input.LA(1);
			} while ( _la==CO );
			setState(1004);
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
		enterRule(_localctx, 80, RULE_intVecLiteralExpression);

		        ((IntVecLiteralExpressionContext)_localctx).tree =  nullptr;
		        TransientArray<int> values(32);
		        Symbol::Location location;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(1007);
			match(LESS);
			 location = SetupFile(); 
			{
			setState(1009);
			((IntVecLiteralExpressionContext)_localctx).arg0 = match(INTEGERLITERAL);
			 values.Append(atof((((IntVecLiteralExpressionContext)_localctx).arg0!=null?((IntVecLiteralExpressionContext)_localctx).arg0.getText():null).c_str())); 
			}
			setState(1013);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__2) {
				{
				setState(1012);
				linePreprocessorEntry();
				}
			}

			setState(1018); 
			_errHandler.sync(this);
			_la = _input.LA(1);
			do {
				{
				{
				setState(1015);
				match(CO);
				setState(1016);
				((IntVecLiteralExpressionContext)_localctx).argN = match(INTEGERLITERAL);
				 values.Append(atof((((IntVecLiteralExpressionContext)_localctx).argN!=null?((IntVecLiteralExpressionContext)_localctx).argN.getText():null).c_str())); 
				}
				}
				setState(1020); 
				_errHandler.sync(this);
				_la = _input.LA(1);
			} while ( _la==CO );
			setState(1022);
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
		enterRule(_localctx, 82, RULE_uintVecLiteralExpression);

		        ((UintVecLiteralExpressionContext)_localctx).tree =  nullptr;
		        TransientArray<unsigned int> values(32);
		        Symbol::Location location;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(1025);
			match(LESS);
			 location = SetupFile(); 
			{
			setState(1027);
			((UintVecLiteralExpressionContext)_localctx).arg0 = match(UINTEGERLITERAL);
			 values.Append(atof((((UintVecLiteralExpressionContext)_localctx).arg0!=null?((UintVecLiteralExpressionContext)_localctx).arg0.getText():null).c_str())); 
			}
			setState(1031);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__2) {
				{
				setState(1030);
				linePreprocessorEntry();
				}
			}

			setState(1036); 
			_errHandler.sync(this);
			_la = _input.LA(1);
			do {
				{
				{
				setState(1033);
				match(CO);
				setState(1034);
				((UintVecLiteralExpressionContext)_localctx).argN = match(UINTEGERLITERAL);
				 values.Append(atof((((UintVecLiteralExpressionContext)_localctx).argN!=null?((UintVecLiteralExpressionContext)_localctx).argN.getText():null).c_str())); 
				}
				}
				setState(1038); 
				_errHandler.sync(this);
				_la = _input.LA(1);
			} while ( _la==CO );
			setState(1040);
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
		enterRule(_localctx, 84, RULE_booleanVecLiteralExpression);

		        ((BooleanVecLiteralExpressionContext)_localctx).tree =  nullptr;
		        TransientArray<bool> values(32);
		        Symbol::Location location;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(1043);
			match(LESS);
			 location = SetupFile(); 
			{
			setState(1045);
			((BooleanVecLiteralExpressionContext)_localctx).arg0 = boolean_();
			 values.Append(atof((((BooleanVecLiteralExpressionContext)_localctx).arg0!=null?_input.getText(((BooleanVecLiteralExpressionContext)_localctx).arg0.start,((BooleanVecLiteralExpressionContext)_localctx).arg0.stop):null).c_str())); 
			}
			setState(1049);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__2) {
				{
				setState(1048);
				linePreprocessorEntry();
				}
			}

			setState(1055); 
			_errHandler.sync(this);
			_la = _input.LA(1);
			do {
				{
				{
				setState(1051);
				match(CO);
				setState(1052);
				((BooleanVecLiteralExpressionContext)_localctx).argN = boolean_();
				 values.Append(atof((((BooleanVecLiteralExpressionContext)_localctx).argN!=null?_input.getText(((BooleanVecLiteralExpressionContext)_localctx).argN.start,((BooleanVecLiteralExpressionContext)_localctx).argN.stop):null).c_str())); 
				}
				}
				setState(1057); 
				_errHandler.sync(this);
				_la = _input.LA(1);
			} while ( _la==CO );
			setState(1059);
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
		case 34:
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
		"\u0004\u0001W\u0427\u0002\u0000\u0007\u0000\u0002\u0001\u0007\u0001\u0002"+
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
		"(\u0007(\u0002)\u0007)\u0002*\u0007*\u0001\u0000\u0001\u0000\u0001\u0000"+
		"\u0005\u0000Z\b\u0000\n\u0000\f\u0000]\t\u0000\u0001\u0000\u0001\u0000"+
		"\u0001\u0000\u0001\u0000\u0005\u0000c\b\u0000\n\u0000\f\u0000f\t\u0000"+
		"\u0001\u0000\u0003\u0000i\b\u0000\u0001\u0001\u0001\u0001\u0001\u0001"+
		"\u0005\u0001n\b\u0001\n\u0001\f\u0001q\t\u0001\u0001\u0001\u0001\u0001"+
		"\u0001\u0001\u0001\u0001\u0005\u0001w\b\u0001\n\u0001\f\u0001z\t\u0001"+
		"\u0001\u0001\u0003\u0001}\b\u0001\u0001\u0002\u0001\u0002\u0001\u0002"+
		"\u0001\u0002\u0003\u0002\u0083\b\u0002\u0001\u0003\u0001\u0003\u0001\u0003"+
		"\u0001\u0003\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004"+
		"\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004"+
		"\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004"+
		"\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004"+
		"\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004"+
		"\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004"+
		"\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0005\u0004"+
		"\u00b1\b\u0004\n\u0004\f\u0004\u00b4\t\u0004\u0001\u0005\u0001\u0005\u0001"+
		"\u0005\u0001\u0005\u0001\u0005\u0001\u0005\u0001\u0006\u0001\u0006\u0001"+
		"\u0006\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0007\u0001"+
		"\u0007\u0001\u0007\u0001\u0007\u0001\u0007\u0001\u0007\u0001\u0007\u0001"+
		"\u0007\u0001\u0007\u0001\b\u0001\b\u0001\b\u0001\b\u0001\b\u0001\b\u0001"+
		"\b\u0001\b\u0001\b\u0003\b\u00d5\b\b\u0001\t\u0001\t\u0001\t\u0001\t\u0001"+
		"\t\u0001\t\u0001\t\u0001\t\u0003\t\u00df\b\t\u0001\t\u0001\t\u0001\t\u0001"+
		"\t\u0005\t\u00e5\b\t\n\t\f\t\u00e8\t\t\u0001\t\u0001\t\u0001\t\u0001\n"+
		"\u0001\n\u0001\n\u0001\n\u0001\n\u0005\n\u00f2\b\n\n\n\f\n\u00f5\t\n\u0001"+
		"\n\u0001\n\u0001\u000b\u0005\u000b\u00fa\b\u000b\n\u000b\f\u000b\u00fd"+
		"\t\u000b\u0001\u000b\u0001\u000b\u0001\u000b\u0005\u000b\u0102\b\u000b"+
		"\n\u000b\f\u000b\u0105\t\u000b\u0001\u000b\u0001\u000b\u0001\u000b\u0004"+
		"\u000b\u010a\b\u000b\u000b\u000b\f\u000b\u010b\u0001\u000b\u0001\u000b"+
		"\u0001\u000b\u0003\u000b\u0111\b\u000b\u0001\u000b\u0001\u000b\u0001\u000b"+
		"\u0001\u000b\u0005\u000b\u0117\b\u000b\n\u000b\f\u000b\u011a\t\u000b\u0001"+
		"\u000b\u0001\u000b\u0001\u000b\u0001\u000b\u0003\u000b\u0120\b\u000b\u0001"+
		"\u000b\u0001\u000b\u0001\u000b\u0001\u000b\u0001\u000b\u0001\u000b\u0001"+
		"\u000b\u0001\u000b\u0005\u000b\u012a\b\u000b\n\u000b\f\u000b\u012d\t\u000b"+
		"\u0003\u000b\u012f\b\u000b\u0001\u000b\u0001\u000b\u0001\f\u0005\f\u0134"+
		"\b\f\n\f\f\f\u0137\t\f\u0001\f\u0001\f\u0001\f\u0005\f\u013c\b\f\n\f\f"+
		"\f\u013f\t\f\u0001\f\u0001\f\u0001\f\u0005\f\u0144\b\f\n\f\f\f\u0147\t"+
		"\f\u0001\f\u0001\f\u0001\f\u0001\f\u0001\r\u0001\r\u0001\r\u0001\r\u0001"+
		"\r\u0001\r\u0001\r\u0001\r\u0001\r\u0001\r\u0001\r\u0001\r\u0001\r\u0001"+
		"\r\u0003\r\u015b\b\r\u0001\r\u0005\r\u015e\b\r\n\r\f\r\u0161\t\r\u0001"+
		"\r\u0001\r\u0001\r\u0001\r\u0001\r\u0005\r\u0168\b\r\n\r\f\r\u016b\t\r"+
		"\u0001\r\u0001\r\u0001\r\u0001\u000e\u0001\u000e\u0001\u000e\u0001\u000e"+
		"\u0001\u000e\u0001\u000e\u0001\u000e\u0003\u000e\u0177\b\u000e\u0001\u000e"+
		"\u0001\u000e\u0001\u000e\u0001\u000e\u0001\u000e\u0001\u000e\u0001\u000e"+
		"\u0003\u000e\u0180\b\u000e\u0001\u000e\u0001\u000e\u0003\u000e\u0184\b"+
		"\u000e\u0001\u000e\u0001\u000e\u0001\u000e\u0001\u000e\u0001\u000e\u0001"+
		"\u000e\u0001\u000e\u0003\u000e\u018d\b\u000e\u0001\u000e\u0001\u000e\u0005"+
		"\u000e\u0191\b\u000e\n\u000e\f\u000e\u0194\t\u000e\u0001\u000e\u0003\u000e"+
		"\u0197\b\u000e\u0001\u000e\u0001\u000e\u0001\u000e\u0001\u000f\u0005\u000f"+
		"\u019d\b\u000f\n\u000f\f\u000f\u01a0\t\u000f\u0001\u000f\u0001\u000f\u0001"+
		"\u000f\u0005\u000f\u01a5\b\u000f\n\u000f\f\u000f\u01a8\t\u000f\u0001\u000f"+
		"\u0001\u000f\u0001\u000f\u0001\u000f\u0001\u000f\u0001\u000f\u0001\u000f"+
		"\u0001\u000f\u0001\u000f\u0003\u000f\u01b3\b\u000f\u0001\u000f\u0001\u000f"+
		"\u0001\u0010\u0001\u0010\u0001\u0010\u0005\u0010\u01ba\b\u0010\n\u0010"+
		"\f\u0010\u01bd\t\u0010\u0001\u0010\u0001\u0010\u0001\u0010\u0001\u0010"+
		"\u0001\u0010\u0001\u0010\u0003\u0010\u01c5\b\u0010\u0001\u0010\u0001\u0010"+
		"\u0001\u0010\u0001\u0010\u0001\u0010\u0005\u0010\u01cc\b\u0010\n\u0010"+
		"\f\u0010\u01cf\t\u0010\u0003\u0010\u01d1\b\u0010\u0001\u0010\u0001\u0010"+
		"\u0001\u0010\u0001\u0010\u0001\u0011\u0001\u0011\u0001\u0011\u0001\u0011"+
		"\u0001\u0011\u0001\u0012\u0001\u0012\u0001\u0012\u0005\u0012\u01df\b\u0012"+
		"\n\u0012\f\u0012\u01e2\t\u0012\u0001\u0012\u0001\u0012\u0001\u0012\u0001"+
		"\u0012\u0001\u0012\u0001\u0012\u0001\u0012\u0001\u0012\u0001\u0012\u0005"+
		"\u0012\u01ed\b\u0012\n\u0012\f\u0012\u01f0\t\u0012\u0001\u0012\u0001\u0012"+
		"\u0001\u0012\u0001\u0013\u0001\u0013\u0001\u0013\u0005\u0013\u01f8\b\u0013"+
		"\n\u0013\f\u0013\u01fb\t\u0013\u0001\u0013\u0001\u0013\u0001\u0013\u0005"+
		"\u0013\u0200\b\u0013\n\u0013\f\u0013\u0203\t\u0013\u0001\u0013\u0001\u0013"+
		"\u0001\u0013\u0001\u0013\u0001\u0013\u0001\u0013\u0001\u0013\u0001\u0013"+
		"\u0001\u0013\u0001\u0013\u0005\u0013\u020f\b\u0013\n\u0013\f\u0013\u0212"+
		"\t\u0013\u0001\u0013\u0001\u0013\u0001\u0013\u0001\u0014\u0001\u0014\u0001"+
		"\u0014\u0001\u0014\u0001\u0014\u0001\u0014\u0001\u0014\u0001\u0014\u0001"+
		"\u0014\u0001\u0014\u0005\u0014\u0221\b\u0014\n\u0014\f\u0014\u0224\t\u0014"+
		"\u0001\u0014\u0001\u0014\u0001\u0014\u0001\u0015\u0001\u0015\u0001\u0015"+
		"\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015"+
		"\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015"+
		"\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015"+
		"\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015\u0001\u0015"+
		"\u0001\u0015\u0003\u0015\u0245\b\u0015\u0001\u0016\u0001\u0016\u0001\u0016"+
		"\u0001\u0017\u0001\u0017\u0001\u0017\u0001\u0017\u0001\u0017\u0001\u0017"+
		"\u0001\u0017\u0001\u0017\u0001\u0017\u0001\u0017\u0001\u0017\u0001\u0017"+
		"\u0003\u0017\u0256\b\u0017\u0001\u0017\u0001\u0017\u0001\u0018\u0001\u0018"+
		"\u0001\u0018\u0001\u0018\u0001\u0018\u0001\u0018\u0003\u0018\u0260\b\u0018"+
		"\u0001\u0018\u0001\u0018\u0001\u0018\u0001\u0018\u0003\u0018\u0266\b\u0018"+
		"\u0001\u0018\u0001\u0018\u0001\u0018\u0001\u0018\u0003\u0018\u026c\b\u0018"+
		"\u0001\u0018\u0001\u0018\u0001\u0018\u0001\u0018\u0005\u0018\u0272\b\u0018"+
		"\n\u0018\f\u0018\u0275\t\u0018\u0001\u0018\u0001\u0018\u0001\u0018\u0001"+
		"\u0018\u0001\u0019\u0001\u0019\u0001\u0019\u0001\u0019\u0001\u0019\u0001"+
		"\u0019\u0001\u0019\u0001\u0019\u0001\u0019\u0001\u0019\u0001\u0019\u0001"+
		"\u0019\u0001\u0019\u0001\u001a\u0001\u001a\u0001\u001a\u0001\u001a\u0001"+
		"\u001a\u0001\u001a\u0001\u001a\u0001\u001a\u0001\u001a\u0001\u001b\u0001"+
		"\u001b\u0001\u001b\u0001\u001b\u0001\u001b\u0001\u001b\u0001\u001b\u0001"+
		"\u001b\u0001\u001b\u0001\u001b\u0001\u001b\u0001\u001b\u0001\u001b\u0001"+
		"\u001b\u0001\u001b\u0001\u001b\u0001\u001b\u0001\u001b\u0001\u001b\u0001"+
		"\u001b\u0001\u001b\u0001\u001b\u0003\u001b\u02a7\b\u001b\u0001\u001c\u0001"+
		"\u001c\u0001\u001c\u0001\u001c\u0001\u001c\u0001\u001c\u0001\u001c\u0001"+
		"\u001c\u0001\u001c\u0001\u001c\u0005\u001c\u02b3\b\u001c\n\u001c\f\u001c"+
		"\u02b6\t\u001c\u0001\u001c\u0001\u001c\u0001\u001c\u0001\u001c\u0001\u001d"+
		"\u0001\u001d\u0001\u001d\u0001\u001d\u0001\u001d\u0003\u001d\u02c1\b\u001d"+
		"\u0001\u001d\u0001\u001d\u0001\u001d\u0001\u001d\u0001\u001d\u0001\u001d"+
		"\u0001\u001d\u0001\u001d\u0001\u001d\u0001\u001d\u0001\u001d\u0001\u001d"+
		"\u0001\u001d\u0001\u001d\u0003\u001d\u02d1\b\u001d\u0001\u001e\u0001\u001e"+
		"\u0001\u001e\u0001\u001e\u0001\u001e\u0001\u001f\u0001\u001f\u0001\u001f"+
		"\u0001\u001f\u0001\u001f\u0001\u001f\u0001\u001f\u0001\u001f\u0001\u001f"+
		"\u0001\u001f\u0001\u001f\u0001\u001f\u0001\u001f\u0001\u001f\u0003\u001f"+
		"\u02e6\b\u001f\u0005\u001f\u02e8\b\u001f\n\u001f\f\u001f\u02eb\t\u001f"+
		"\u0001\u001f\u0001\u001f\u0001\u001f\u0001\u001f\u0001\u001f\u0003\u001f"+
		"\u02f2\b\u001f\u0003\u001f\u02f4\b\u001f\u0001\u001f\u0001\u001f\u0001"+
		"\u001f\u0001 \u0001 \u0001 \u0001 \u0001 \u0001!\u0003!\u02ff\b!\u0001"+
		"!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0005!\u0308\b!\n!\f!\u030b"+
		"\t!\u0001!\u0001!\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001"+
		"\"\u0001\"\u0003\"\u0317\b\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001"+
		"\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001"+
		"\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001"+
		"\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001"+
		"\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001"+
		"\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001"+
		"\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001"+
		"\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001"+
		"\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001"+
		"\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001"+
		"\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001"+
		"\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001"+
		"\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0003\"\u0388\b\"\u0001"+
		"\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0003\"\u0391\b\"\u0001"+
		"\"\u0001\"\u0005\"\u0395\b\"\n\"\f\"\u0398\t\"\u0001#\u0001#\u0001#\u0001"+
		"#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001"+
		"#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001"+
		"#\u0001#\u0001#\u0001#\u0001#\u0001#\u0001#\u0003#\u03b8\b#\u0001$\u0001"+
		"$\u0001$\u0001$\u0001$\u0003$\u03bf\b$\u0001$\u0001$\u0001$\u0001%\u0001"+
		"%\u0001%\u0003%\u03c7\b%\u0001%\u0001%\u0001%\u0001&\u0001&\u0001&\u0001"+
		"&\u0001&\u0001&\u0003&\u03d2\b&\u0001&\u0001&\u0001&\u0004&\u03d7\b&\u000b"+
		"&\f&\u03d8\u0001&\u0001&\u0001&\u0001\'\u0001\'\u0001\'\u0001\'\u0001"+
		"\'\u0001\'\u0003\'\u03e4\b\'\u0001\'\u0001\'\u0001\'\u0004\'\u03e9\b\'"+
		"\u000b\'\f\'\u03ea\u0001\'\u0001\'\u0001\'\u0001(\u0001(\u0001(\u0001"+
		"(\u0001(\u0001(\u0003(\u03f6\b(\u0001(\u0001(\u0001(\u0004(\u03fb\b(\u000b"+
		"(\f(\u03fc\u0001(\u0001(\u0001(\u0001)\u0001)\u0001)\u0001)\u0001)\u0001"+
		")\u0003)\u0408\b)\u0001)\u0001)\u0001)\u0004)\u040d\b)\u000b)\f)\u040e"+
		"\u0001)\u0001)\u0001)\u0001*\u0001*\u0001*\u0001*\u0001*\u0001*\u0003"+
		"*\u041a\b*\u0001*\u0001*\u0001*\u0001*\u0004*\u0420\b*\u000b*\f*\u0421"+
		"\u0001*\u0001*\u0001*\u0001*\u0001\u00b2\u0001D+\u0000\u0002\u0004\u0006"+
		"\b\n\f\u000e\u0010\u0012\u0014\u0016\u0018\u001a\u001c\u001e \"$&(*,."+
		"02468:<>@BDFHJLNPRT\u0000\u000b\u0001\u000033\u0001\u0000<<\u0001\u0000"+
		"AA\u0005\u0000\u001c\u001d11;;JKMM\u0002\u0000GGLM\u0001\u0000JK\u0001"+
		"\u0000\u001e\u001f\u0001\u0000?B\u0001\u0000CD\u0005\u0000 &226688::\u0001"+
		"\u0000\u001c\u001d\u0483\u0000h\u0001\u0000\u0000\u0000\u0002|\u0001\u0000"+
		"\u0000\u0000\u0004\u0082\u0001\u0000\u0000\u0000\u0006\u0084\u0001\u0000"+
		"\u0000\u0000\b\u00b2\u0001\u0000\u0000\u0000\n\u00b5\u0001\u0000\u0000"+
		"\u0000\f\u00bb\u0001\u0000\u0000\u0000\u000e\u00c2\u0001\u0000\u0000\u0000"+
		"\u0010\u00d4\u0001\u0000\u0000\u0000\u0012\u00d6\u0001\u0000\u0000\u0000"+
		"\u0014\u00ec\u0001\u0000\u0000\u0000\u0016\u00fb\u0001\u0000\u0000\u0000"+
		"\u0018\u0135\u0001\u0000\u0000\u0000\u001a\u014c\u0001\u0000\u0000\u0000"+
		"\u001c\u016f\u0001\u0000\u0000\u0000\u001e\u019e\u0001\u0000\u0000\u0000"+
		" \u01bb\u0001\u0000\u0000\u0000\"\u01d6\u0001\u0000\u0000\u0000$\u01e0"+
		"\u0001\u0000\u0000\u0000&\u01f9\u0001\u0000\u0000\u0000(\u0216\u0001\u0000"+
		"\u0000\u0000*\u0244\u0001\u0000\u0000\u0000,\u0246\u0001\u0000\u0000\u0000"+
		".\u0249\u0001\u0000\u0000\u00000\u0259\u0001\u0000\u0000\u00002\u027a"+
		"\u0001\u0000\u0000\u00004\u0287\u0001\u0000\u0000\u00006\u02a6\u0001\u0000"+
		"\u0000\u00008\u02a8\u0001\u0000\u0000\u0000:\u02d0\u0001\u0000\u0000\u0000"+
		"<\u02d2\u0001\u0000\u0000\u0000>\u02d7\u0001\u0000\u0000\u0000@\u02f8"+
		"\u0001\u0000\u0000\u0000B\u02fe\u0001\u0000\u0000\u0000D\u0316\u0001\u0000"+
		"\u0000\u0000F\u03b7\u0001\u0000\u0000\u0000H\u03b9\u0001\u0000\u0000\u0000"+
		"J\u03c3\u0001\u0000\u0000\u0000L\u03cb\u0001\u0000\u0000\u0000N\u03dd"+
		"\u0001\u0000\u0000\u0000P\u03ef\u0001\u0000\u0000\u0000R\u0401\u0001\u0000"+
		"\u0000\u0000T\u0413\u0001\u0000\u0000\u0000V[\u00053\u0000\u0000WX\b\u0000"+
		"\u0000\u0000XZ\u0006\u0000\uffff\uffff\u0000YW\u0001\u0000\u0000\u0000"+
		"Z]\u0001\u0000\u0000\u0000[Y\u0001\u0000\u0000\u0000[\\\u0001\u0000\u0000"+
		"\u0000\\^\u0001\u0000\u0000\u0000][\u0001\u0000\u0000\u0000^i\u00053\u0000"+
		"\u0000_d\u0005<\u0000\u0000`a\b\u0001\u0000\u0000ac\u0006\u0000\uffff"+
		"\uffff\u0000b`\u0001\u0000\u0000\u0000cf\u0001\u0000\u0000\u0000db\u0001"+
		"\u0000\u0000\u0000de\u0001\u0000\u0000\u0000eg\u0001\u0000\u0000\u0000"+
		"fd\u0001\u0000\u0000\u0000gi\u0005<\u0000\u0000hV\u0001\u0000\u0000\u0000"+
		"h_\u0001\u0000\u0000\u0000i\u0001\u0001\u0000\u0000\u0000jo\u00053\u0000"+
		"\u0000kl\b\u0000\u0000\u0000ln\u0006\u0001\uffff\uffff\u0000mk\u0001\u0000"+
		"\u0000\u0000nq\u0001\u0000\u0000\u0000om\u0001\u0000\u0000\u0000op\u0001"+
		"\u0000\u0000\u0000pr\u0001\u0000\u0000\u0000qo\u0001\u0000\u0000\u0000"+
		"r}\u00053\u0000\u0000sx\u0005?\u0000\u0000tu\b\u0002\u0000\u0000uw\u0006"+
		"\u0001\uffff\uffff\u0000vt\u0001\u0000\u0000\u0000wz\u0001\u0000\u0000"+
		"\u0000xv\u0001\u0000\u0000\u0000xy\u0001\u0000\u0000\u0000y{\u0001\u0000"+
		"\u0000\u0000zx\u0001\u0000\u0000\u0000{}\u0005A\u0000\u0000|j\u0001\u0000"+
		"\u0000\u0000|s\u0001\u0000\u0000\u0000}\u0003\u0001\u0000\u0000\u0000"+
		"~\u007f\u0005\u0001\u0000\u0000\u007f\u0083\u0006\u0002\uffff\uffff\u0000"+
		"\u0080\u0081\u0005\u0002\u0000\u0000\u0081\u0083\u0006\u0002\uffff\uffff"+
		"\u0000\u0082~\u0001\u0000\u0000\u0000\u0082\u0080\u0001\u0000\u0000\u0000"+
		"\u0083\u0005\u0001\u0000\u0000\u0000\u0084\u0085\u0003\b\u0004\u0000\u0085"+
		"\u0086\u0006\u0003\uffff\uffff\u0000\u0086\u0087\u0005\u0000\u0000\u0001"+
		"\u0087\u0007\u0001\u0000\u0000\u0000\u0088\u00b1\u0003\n\u0005\u0000\u0089"+
		"\u008a\u0003\u0014\n\u0000\u008a\u008b\u0005\'\u0000\u0000\u008b\u008c"+
		"\u0006\u0004\uffff\uffff\u0000\u008c\u00b1\u0001\u0000\u0000\u0000\u008d"+
		"\u008e\u0003\f\u0006\u0000\u008e\u008f\u0005\'\u0000\u0000\u008f\u0090"+
		"\u0006\u0004\uffff\uffff\u0000\u0090\u00b1\u0001\u0000\u0000\u0000\u0091"+
		"\u0092\u0003 \u0010\u0000\u0092\u0093\u0005\'\u0000\u0000\u0093\u0094"+
		"\u0006\u0004\uffff\uffff\u0000\u0094\u00b1\u0001\u0000\u0000\u0000\u0095"+
		"\u0096\u0003\"\u0011\u0000\u0096\u0097\u0006\u0004\uffff\uffff\u0000\u0097"+
		"\u00b1\u0001\u0000\u0000\u0000\u0098\u0099\u0003\u0016\u000b\u0000\u0099"+
		"\u009a\u0005\'\u0000\u0000\u009a\u009b\u0006\u0004\uffff\uffff\u0000\u009b"+
		"\u00b1\u0001\u0000\u0000\u0000\u009c\u009d\u0003\u001a\r\u0000\u009d\u009e"+
		"\u0005\'\u0000\u0000\u009e\u009f\u0006\u0004\uffff\uffff\u0000\u009f\u00b1"+
		"\u0001\u0000\u0000\u0000\u00a0\u00a1\u0003\u001c\u000e\u0000\u00a1\u00a2"+
		"\u0005\'\u0000\u0000\u00a2\u00a3\u0006\u0004\uffff\uffff\u0000\u00a3\u00b1"+
		"\u0001\u0000\u0000\u0000\u00a4\u00a5\u0003(\u0014\u0000\u00a5\u00a6\u0005"+
		"\'\u0000\u0000\u00a6\u00a7\u0006\u0004\uffff\uffff\u0000\u00a7\u00b1\u0001"+
		"\u0000\u0000\u0000\u00a8\u00a9\u0003&\u0013\u0000\u00a9\u00aa\u0005\'"+
		"\u0000\u0000\u00aa\u00ab\u0006\u0004\uffff\uffff\u0000\u00ab\u00b1\u0001"+
		"\u0000\u0000\u0000\u00ac\u00ad\u0003$\u0012\u0000\u00ad\u00ae\u0005\'"+
		"\u0000\u0000\u00ae\u00af\u0006\u0004\uffff\uffff\u0000\u00af\u00b1\u0001"+
		"\u0000\u0000\u0000\u00b0\u0088\u0001\u0000\u0000\u0000\u00b0\u0089\u0001"+
		"\u0000\u0000\u0000\u00b0\u008d\u0001\u0000\u0000\u0000\u00b0\u0091\u0001"+
		"\u0000\u0000\u0000\u00b0\u0095\u0001\u0000\u0000\u0000\u00b0\u0098\u0001"+
		"\u0000\u0000\u0000\u00b0\u009c\u0001\u0000\u0000\u0000\u00b0\u00a0\u0001"+
		"\u0000\u0000\u0000\u00b0\u00a4\u0001\u0000\u0000\u0000\u00b0\u00a8\u0001"+
		"\u0000\u0000\u0000\u00b0\u00ac\u0001\u0000\u0000\u0000\u00b1\u00b4\u0001"+
		"\u0000\u0000\u0000\u00b2\u00b3\u0001\u0000\u0000\u0000\u00b2\u00b0\u0001"+
		"\u0000\u0000\u0000\u00b3\t\u0001\u0000\u0000\u0000\u00b4\u00b2\u0001\u0000"+
		"\u0000\u0000\u00b5\u00b6\u0005\u0003\u0000\u0000\u00b6\u00b7\u0006\u0005"+
		"\uffff\uffff\u0000\u00b7\u00b8\u0005O\u0000\u0000\u00b8\u00b9\u0003\u0000"+
		"\u0000\u0000\u00b9\u00ba\u0006\u0005\uffff\uffff\u0000\u00ba\u000b\u0001"+
		"\u0000\u0000\u0000\u00bb\u00bc\u0005\u0004\u0000\u0000\u00bc\u00bd\u0005"+
		"V\u0000\u0000\u00bd\u00be\u0005\u0005\u0000\u0000\u00be\u00bf\u0005V\u0000"+
		"\u0000\u00bf\u00c0\u0006\u0006\uffff\uffff\u0000\u00c0\u00c1\u0006\u0006"+
		"\uffff\uffff\u0000\u00c1\r\u0001\u0000\u0000\u0000\u00c2\u00c3\u0005I"+
		"\u0000\u0000\u00c3\u00c4\u0006\u0007\uffff\uffff\u0000\u00c4\u00c5\u0005"+
		"V\u0000\u0000\u00c5\u00c6\u0006\u0007\uffff\uffff\u0000\u00c6\u00c7\u0005"+
		"*\u0000\u0000\u00c7\u00c8\u0003D\"\u0000\u00c8\u00c9\u0005+\u0000\u0000"+
		"\u00c9\u00ca\u0006\u0007\uffff\uffff\u0000\u00ca\u000f\u0001\u0000\u0000"+
		"\u0000\u00cb\u00cc\u0005V\u0000\u0000\u00cc\u00cd\u0006\b\uffff\uffff"+
		"\u0000\u00cd\u00ce\u0005*\u0000\u0000\u00ce\u00cf\u0003D\"\u0000\u00cf"+
		"\u00d0\u0005+\u0000\u0000\u00d0\u00d1\u0006\b\uffff\uffff\u0000\u00d1"+
		"\u00d5\u0001\u0000\u0000\u0000\u00d2\u00d3\u0005V\u0000\u0000\u00d3\u00d5"+
		"\u0006\b\uffff\uffff\u0000\u00d4\u00cb\u0001\u0000\u0000\u0000\u00d4\u00d2"+
		"\u0001\u0000\u0000\u0000\u00d5\u0011\u0001\u0000\u0000\u0000\u00d6\u00e6"+
		"\u0006\t\uffff\uffff\u0000\u00d7\u00d8\u0005M\u0000\u0000\u00d8\u00e5"+
		"\u0006\t\uffff\uffff\u0000\u00d9\u00da\u0005.\u0000\u0000\u00da\u00de"+
		"\u0006\t\uffff\uffff\u0000\u00db\u00dc\u0003D\"\u0000\u00dc\u00dd\u0006"+
		"\t\uffff\uffff\u0000\u00dd\u00df\u0001\u0000\u0000\u0000\u00de\u00db\u0001"+
		"\u0000\u0000\u0000\u00de\u00df\u0001\u0000\u0000\u0000\u00df\u00e0\u0001"+
		"\u0000\u0000\u0000\u00e0\u00e5\u0005/\u0000\u0000\u00e1\u00e2\u0005V\u0000"+
		"\u0000\u00e2\u00e5\u0006\t\uffff\uffff\u0000\u00e3\u00e5\u0003\n\u0005"+
		"\u0000\u00e4\u00d7\u0001\u0000\u0000\u0000\u00e4\u00d9\u0001\u0000\u0000"+
		"\u0000\u00e4\u00e1\u0001\u0000\u0000\u0000\u00e4\u00e3\u0001\u0000\u0000"+
		"\u0000\u00e5\u00e8\u0001\u0000\u0000\u0000\u00e6\u00e4\u0001\u0000\u0000"+
		"\u0000\u00e6\u00e7\u0001\u0000\u0000\u0000\u00e7\u00e9\u0001\u0000\u0000"+
		"\u0000\u00e8\u00e6\u0001\u0000\u0000\u0000\u00e9\u00ea\u0005V\u0000\u0000"+
		"\u00ea\u00eb\u0006\t\uffff\uffff\u0000\u00eb\u0013\u0001\u0000\u0000\u0000"+
		"\u00ec\u00ed\u0005\u0006\u0000\u0000\u00ed\u00f3\u0005?\u0000\u0000\u00ee"+
		"\u00ef\u0003*\u0015\u0000\u00ef\u00f0\u0006\n\uffff\uffff\u0000\u00f0"+
		"\u00f2\u0001\u0000\u0000\u0000\u00f1\u00ee\u0001\u0000\u0000\u0000\u00f2"+
		"\u00f5\u0001\u0000\u0000\u0000\u00f3\u00f1\u0001\u0000\u0000\u0000\u00f3"+
		"\u00f4\u0001\u0000\u0000\u0000\u00f4\u00f6\u0001\u0000\u0000\u0000\u00f5"+
		"\u00f3\u0001\u0000\u0000\u0000\u00f6\u00f7\u0005A\u0000\u0000\u00f7\u0015"+
		"\u0001\u0000\u0000\u0000\u00f8\u00fa\u0003\n\u0005\u0000\u00f9\u00f8\u0001"+
		"\u0000\u0000\u0000\u00fa\u00fd\u0001\u0000\u0000\u0000\u00fb\u00f9\u0001"+
		"\u0000\u0000\u0000\u00fb\u00fc\u0001\u0000\u0000\u0000\u00fc\u0103\u0001"+
		"\u0000\u0000\u0000\u00fd\u00fb\u0001\u0000\u0000\u0000\u00fe\u00ff\u0003"+
		"\u000e\u0007\u0000\u00ff\u0100\u0006\u000b\uffff\uffff\u0000\u0100\u0102"+
		"\u0001\u0000\u0000\u0000\u0101\u00fe\u0001\u0000\u0000\u0000\u0102\u0105"+
		"\u0001\u0000\u0000\u0000\u0103\u0101\u0001\u0000\u0000\u0000\u0103\u0104"+
		"\u0001\u0000\u0000\u0000\u0104\u0109\u0001\u0000\u0000\u0000\u0105\u0103"+
		"\u0001\u0000\u0000\u0000\u0106\u0107\u0003\u0010\b\u0000\u0107\u0108\u0006"+
		"\u000b\uffff\uffff\u0000\u0108\u010a\u0001\u0000\u0000\u0000\u0109\u0106"+
		"\u0001\u0000\u0000\u0000\u010a\u010b\u0001\u0000\u0000\u0000\u010b\u0109"+
		"\u0001\u0000\u0000\u0000\u010b\u010c\u0001\u0000\u0000\u0000\u010c\u010d"+
		"\u0001\u0000\u0000\u0000\u010d\u010e\u0005V\u0000\u0000\u010e\u0110\u0006"+
		"\u000b\uffff\uffff\u0000\u010f\u0111\u0003\n\u0005\u0000\u0110\u010f\u0001"+
		"\u0000\u0000\u0000\u0110\u0111\u0001\u0000\u0000\u0000\u0111\u0118\u0001"+
		"\u0000\u0000\u0000\u0112\u0113\u0005(\u0000\u0000\u0113\u0114\u0005V\u0000"+
		"\u0000\u0114\u0117\u0006\u000b\uffff\uffff\u0000\u0115\u0117\u0003\n\u0005"+
		"\u0000\u0116\u0112\u0001\u0000\u0000\u0000\u0116\u0115\u0001\u0000\u0000"+
		"\u0000\u0117\u011a\u0001\u0000\u0000\u0000\u0118\u0116\u0001\u0000\u0000"+
		"\u0000\u0118\u0119\u0001\u0000\u0000\u0000\u0119\u011f\u0001\u0000\u0000"+
		"\u0000\u011a\u0118\u0001\u0000\u0000\u0000\u011b\u011c\u0005)\u0000\u0000"+
		"\u011c\u011d\u0003\u0012\t\u0000\u011d\u011e\u0006\u000b\uffff\uffff\u0000"+
		"\u011e\u0120\u0001\u0000\u0000\u0000\u011f\u011b\u0001\u0000\u0000\u0000"+
		"\u011f\u0120\u0001\u0000\u0000\u0000\u0120\u012e\u0001\u0000\u0000\u0000"+
		"\u0121\u0122\u00052\u0000\u0000\u0122\u0123\u0003D\"\u0000\u0123\u012b"+
		"\u0006\u000b\uffff\uffff\u0000\u0124\u0125\u0005(\u0000\u0000\u0125\u0126"+
		"\u0003D\"\u0000\u0126\u0127\u0006\u000b\uffff\uffff\u0000\u0127\u012a"+
		"\u0001\u0000\u0000\u0000\u0128\u012a\u0003\n\u0005\u0000\u0129\u0124\u0001"+
		"\u0000\u0000\u0000\u0129\u0128\u0001\u0000\u0000\u0000\u012a\u012d\u0001"+
		"\u0000\u0000\u0000\u012b\u0129\u0001\u0000\u0000\u0000\u012b\u012c\u0001"+
		"\u0000\u0000\u0000\u012c\u012f\u0001\u0000\u0000\u0000\u012d\u012b\u0001"+
		"\u0000\u0000\u0000\u012e\u0121\u0001\u0000\u0000\u0000\u012e\u012f\u0001"+
		"\u0000\u0000\u0000\u012f\u0130\u0001\u0000\u0000\u0000\u0130\u0131\u0006"+
		"\u000b\uffff\uffff\u0000\u0131\u0017\u0001\u0000\u0000\u0000\u0132\u0134"+
		"\u0003\n\u0005\u0000\u0133\u0132\u0001\u0000\u0000\u0000\u0134\u0137\u0001"+
		"\u0000\u0000\u0000\u0135\u0133\u0001\u0000\u0000\u0000\u0135\u0136\u0001"+
		"\u0000\u0000\u0000\u0136\u013d\u0001\u0000\u0000\u0000\u0137\u0135\u0001"+
		"\u0000\u0000\u0000\u0138\u0139\u0003\u000e\u0007\u0000\u0139\u013a\u0006"+
		"\f\uffff\uffff\u0000\u013a\u013c\u0001\u0000\u0000\u0000\u013b\u0138\u0001"+
		"\u0000\u0000\u0000\u013c\u013f\u0001\u0000\u0000\u0000\u013d\u013b\u0001"+
		"\u0000\u0000\u0000\u013d\u013e\u0001\u0000\u0000\u0000\u013e\u0145\u0001"+
		"\u0000\u0000\u0000\u013f\u013d\u0001\u0000\u0000\u0000\u0140\u0141\u0003"+
		"\u0010\b\u0000\u0141\u0142\u0006\f\uffff\uffff\u0000\u0142\u0144\u0001"+
		"\u0000\u0000\u0000\u0143\u0140\u0001\u0000\u0000\u0000\u0144\u0147\u0001"+
		"\u0000\u0000\u0000\u0145\u0143\u0001\u0000\u0000\u0000\u0145\u0146\u0001"+
		"\u0000\u0000\u0000\u0146\u0148\u0001\u0000\u0000\u0000\u0147\u0145\u0001"+
		"\u0000\u0000\u0000\u0148\u0149\u0005\u0007\u0000\u0000\u0149\u014a\u0005"+
		"V\u0000\u0000\u014a\u014b\u0006\f\uffff\uffff\u0000\u014b\u0019\u0001"+
		"\u0000\u0000\u0000\u014c\u014d\u0003\u0018\f\u0000\u014d\u014e\u0006\r"+
		"\uffff\uffff\u0000\u014e\u0169\u0005,\u0000\u0000\u014f\u0150\u0005V\u0000"+
		"\u0000\u0150\u0151\u0006\r\uffff\uffff\u0000\u0151\u0152\u0005)\u0000"+
		"\u0000\u0152\u015f\u0006\r\uffff\uffff\u0000\u0153\u0154\u0005M\u0000"+
		"\u0000\u0154\u015e\u0006\r\uffff\uffff\u0000\u0155\u0156\u0005.\u0000"+
		"\u0000\u0156\u015a\u0006\r\uffff\uffff\u0000\u0157\u0158\u0003D\"\u0000"+
		"\u0158\u0159\u0006\r\uffff\uffff\u0000\u0159\u015b\u0001\u0000\u0000\u0000"+
		"\u015a\u0157\u0001\u0000\u0000\u0000\u015a\u015b\u0001\u0000\u0000\u0000"+
		"\u015b\u015c\u0001\u0000\u0000\u0000\u015c\u015e\u0005/\u0000\u0000\u015d"+
		"\u0153\u0001\u0000\u0000\u0000\u015d\u0155\u0001\u0000\u0000\u0000\u015e"+
		"\u0161\u0001\u0000\u0000\u0000\u015f\u015d\u0001\u0000\u0000\u0000\u015f"+
		"\u0160\u0001\u0000\u0000\u0000\u0160\u0162\u0001\u0000\u0000\u0000\u0161"+
		"\u015f\u0001\u0000\u0000\u0000\u0162\u0163\u0005V\u0000\u0000\u0163\u0164"+
		"\u0006\r\uffff\uffff\u0000\u0164\u0165\u0005\'\u0000\u0000\u0165\u0168"+
		"\u0006\r\uffff\uffff\u0000\u0166\u0168\u0003\n\u0005\u0000\u0167\u014f"+
		"\u0001\u0000\u0000\u0000\u0167\u0166\u0001\u0000\u0000\u0000\u0168\u016b"+
		"\u0001\u0000\u0000\u0000\u0169\u0167\u0001\u0000\u0000\u0000\u0169\u016a"+
		"\u0001\u0000\u0000\u0000\u016a\u016c\u0001\u0000\u0000\u0000\u016b\u0169"+
		"\u0001\u0000\u0000\u0000\u016c\u016d\u0005-\u0000\u0000\u016d\u016e\u0006"+
		"\r\uffff\uffff\u0000\u016e\u001b\u0001\u0000\u0000\u0000\u016f\u0170\u0005"+
		"\b\u0000\u0000\u0170\u0171\u0005V\u0000\u0000\u0171\u0176\u0006\u000e"+
		"\uffff\uffff\u0000\u0172\u0173\u0005)\u0000\u0000\u0173\u0174\u0003\u0012"+
		"\t\u0000\u0174\u0175\u0006\u000e\uffff\uffff\u0000\u0175\u0177\u0001\u0000"+
		"\u0000\u0000\u0176\u0172\u0001\u0000\u0000\u0000\u0176\u0177\u0001\u0000"+
		"\u0000\u0000\u0177\u0178\u0001\u0000\u0000\u0000\u0178\u0196\u0005,\u0000"+
		"\u0000\u0179\u017a\u0005V\u0000\u0000\u017a\u017f\u0006\u000e\uffff\uffff"+
		"\u0000\u017b\u017c\u00052\u0000\u0000\u017c\u017d\u0003D\"\u0000\u017d"+
		"\u017e\u0006\u000e\uffff\uffff\u0000\u017e\u0180\u0001\u0000\u0000\u0000"+
		"\u017f\u017b\u0001\u0000\u0000\u0000\u017f\u0180\u0001\u0000\u0000\u0000"+
		"\u0180\u0181\u0001\u0000\u0000\u0000\u0181\u0183\u0006\u000e\uffff\uffff"+
		"\u0000\u0182\u0184\u0003\n\u0005\u0000\u0183\u0182\u0001\u0000\u0000\u0000"+
		"\u0183\u0184\u0001\u0000\u0000\u0000\u0184\u0192\u0001\u0000\u0000\u0000"+
		"\u0185\u0186\u0005(\u0000\u0000\u0186\u0187\u0005V\u0000\u0000\u0187\u018c"+
		"\u0006\u000e\uffff\uffff\u0000\u0188\u0189\u00052\u0000\u0000\u0189\u018a"+
		"\u0003D\"\u0000\u018a\u018b\u0006\u000e\uffff\uffff\u0000\u018b\u018d"+
		"\u0001\u0000\u0000\u0000\u018c\u0188\u0001\u0000\u0000\u0000\u018c\u018d"+
		"\u0001\u0000\u0000\u0000\u018d\u018e\u0001\u0000\u0000\u0000\u018e\u0191"+
		"\u0006\u000e\uffff\uffff\u0000\u018f\u0191\u0003\n\u0005\u0000\u0190\u0185"+
		"\u0001\u0000\u0000\u0000\u0190\u018f\u0001\u0000\u0000\u0000\u0191\u0194"+
		"\u0001\u0000\u0000\u0000\u0192\u0190\u0001\u0000\u0000\u0000\u0192\u0193"+
		"\u0001\u0000\u0000\u0000\u0193\u0197\u0001\u0000\u0000\u0000\u0194\u0192"+
		"\u0001\u0000\u0000\u0000\u0195\u0197\u0003\n\u0005\u0000\u0196\u0179\u0001"+
		"\u0000\u0000\u0000\u0196\u0195\u0001\u0000\u0000\u0000\u0196\u0197\u0001"+
		"\u0000\u0000\u0000\u0197\u0198\u0001\u0000\u0000\u0000\u0198\u0199\u0005"+
		"-\u0000\u0000\u0199\u019a\u0006\u000e\uffff\uffff\u0000\u019a\u001d\u0001"+
		"\u0000\u0000\u0000\u019b\u019d\u0003\n\u0005\u0000\u019c\u019b\u0001\u0000"+
		"\u0000\u0000\u019d\u01a0\u0001\u0000\u0000\u0000\u019e\u019c\u0001\u0000"+
		"\u0000\u0000\u019e\u019f\u0001\u0000\u0000\u0000\u019f\u01a6\u0001\u0000"+
		"\u0000\u0000\u01a0\u019e\u0001\u0000\u0000\u0000\u01a1\u01a2\u0003\u0010"+
		"\b\u0000\u01a2\u01a3\u0006\u000f\uffff\uffff\u0000\u01a3\u01a5\u0001\u0000"+
		"\u0000\u0000\u01a4\u01a1\u0001\u0000\u0000\u0000\u01a5\u01a8\u0001\u0000"+
		"\u0000\u0000\u01a6\u01a4\u0001\u0000\u0000\u0000\u01a6\u01a7\u0001\u0000"+
		"\u0000\u0000\u01a7\u01a9\u0001\u0000\u0000\u0000\u01a8\u01a6\u0001\u0000"+
		"\u0000\u0000\u01a9\u01aa\u0005V\u0000\u0000\u01aa\u01ab\u0006\u000f\uffff"+
		"\uffff\u0000\u01ab\u01ac\u0005)\u0000\u0000\u01ac\u01ad\u0003\u0012\t"+
		"\u0000\u01ad\u01b2\u0006\u000f\uffff\uffff\u0000\u01ae\u01af\u00052\u0000"+
		"\u0000\u01af\u01b0\u0003D\"\u0000\u01b0\u01b1\u0006\u000f\uffff\uffff"+
		"\u0000\u01b1\u01b3\u0001\u0000\u0000\u0000\u01b2\u01ae\u0001\u0000\u0000"+
		"\u0000\u01b2\u01b3\u0001\u0000\u0000\u0000\u01b3\u01b4\u0001\u0000\u0000"+
		"\u0000\u01b4\u01b5\u0006\u000f\uffff\uffff\u0000\u01b5\u001f\u0001\u0000"+
		"\u0000\u0000\u01b6\u01b7\u0003\u0010\b\u0000\u01b7\u01b8\u0006\u0010\uffff"+
		"\uffff\u0000\u01b8\u01ba\u0001\u0000\u0000\u0000\u01b9\u01b6\u0001\u0000"+
		"\u0000\u0000\u01ba\u01bd\u0001\u0000\u0000\u0000\u01bb\u01b9\u0001\u0000"+
		"\u0000\u0000\u01bb\u01bc\u0001\u0000\u0000\u0000\u01bc\u01be\u0001\u0000"+
		"\u0000\u0000\u01bd\u01bb\u0001\u0000\u0000\u0000\u01be\u01bf\u0005V\u0000"+
		"\u0000\u01bf\u01c0\u0006\u0010\uffff\uffff\u0000\u01c0\u01d0\u0005*\u0000"+
		"\u0000\u01c1\u01c2\u0003\u001e\u000f\u0000\u01c2\u01c4\u0006\u0010\uffff"+
		"\uffff\u0000\u01c3\u01c5\u0003\n\u0005\u0000\u01c4\u01c3\u0001\u0000\u0000"+
		"\u0000\u01c4\u01c5\u0001\u0000\u0000\u0000\u01c5\u01cd\u0001\u0000\u0000"+
		"\u0000\u01c6\u01c7\u0005(\u0000\u0000\u01c7\u01c8\u0003\u001e\u000f\u0000"+
		"\u01c8\u01c9\u0006\u0010\uffff\uffff\u0000\u01c9\u01cc\u0001\u0000\u0000"+
		"\u0000\u01ca\u01cc\u0003\n\u0005\u0000\u01cb\u01c6\u0001\u0000\u0000\u0000"+
		"\u01cb\u01ca\u0001\u0000\u0000\u0000\u01cc\u01cf\u0001\u0000\u0000\u0000"+
		"\u01cd\u01cb\u0001\u0000\u0000\u0000\u01cd\u01ce\u0001\u0000\u0000\u0000"+
		"\u01ce\u01d1\u0001\u0000\u0000\u0000\u01cf\u01cd\u0001\u0000\u0000\u0000"+
		"\u01d0\u01c1\u0001\u0000\u0000\u0000\u01d0\u01d1\u0001\u0000\u0000\u0000"+
		"\u01d1\u01d2\u0001\u0000\u0000\u0000\u01d2\u01d3\u0005+\u0000\u0000\u01d3"+
		"\u01d4\u0003\u0012\t\u0000\u01d4\u01d5\u0006\u0010\uffff\uffff\u0000\u01d5"+
		"!\u0001\u0000\u0000\u0000\u01d6\u01d7\u0003 \u0010\u0000\u01d7\u01d8\u0006"+
		"\u0011\uffff\uffff\u0000\u01d8\u01d9\u00038\u001c\u0000\u01d9\u01da\u0006"+
		"\u0011\uffff\uffff\u0000\u01da#\u0001\u0000\u0000\u0000\u01db\u01dc\u0003"+
		"\u000e\u0007\u0000\u01dc\u01dd\u0006\u0012\uffff\uffff\u0000\u01dd\u01df"+
		"\u0001\u0000\u0000\u0000\u01de\u01db\u0001\u0000\u0000\u0000\u01df\u01e2"+
		"\u0001\u0000\u0000\u0000\u01e0\u01de\u0001\u0000\u0000\u0000\u01e0\u01e1"+
		"\u0001\u0000\u0000\u0000\u01e1\u01e3\u0001\u0000\u0000\u0000\u01e2\u01e0"+
		"\u0001\u0000\u0000\u0000\u01e3\u01e4\u0005\t\u0000\u0000\u01e4\u01e5\u0006"+
		"\u0012\uffff\uffff\u0000\u01e5\u01e6\u0005V\u0000\u0000\u01e6\u01e7\u0006"+
		"\u0012\uffff\uffff\u0000\u01e7\u01ee\u0005,\u0000\u0000\u01e8\u01e9\u0003"+
		"D\"\u0000\u01e9\u01ea\u0006\u0012\uffff\uffff\u0000\u01ea\u01eb\u0005"+
		"\'\u0000\u0000\u01eb\u01ed\u0001\u0000\u0000\u0000\u01ec\u01e8\u0001\u0000"+
		"\u0000\u0000\u01ed\u01f0\u0001\u0000\u0000\u0000\u01ee\u01ec\u0001\u0000"+
		"\u0000\u0000\u01ee\u01ef\u0001\u0000\u0000\u0000\u01ef\u01f1\u0001\u0000"+
		"\u0000\u0000\u01f0\u01ee\u0001\u0000\u0000\u0000\u01f1\u01f2\u0005-\u0000"+
		"\u0000\u01f2\u01f3\u0006\u0012\uffff\uffff\u0000\u01f3%\u0001\u0000\u0000"+
		"\u0000\u01f4\u01f5\u0003\u000e\u0007\u0000\u01f5\u01f6\u0006\u0013\uffff"+
		"\uffff\u0000\u01f6\u01f8\u0001\u0000\u0000\u0000\u01f7\u01f4\u0001\u0000"+
		"\u0000\u0000\u01f8\u01fb\u0001\u0000\u0000\u0000\u01f9\u01f7\u0001\u0000"+
		"\u0000\u0000\u01f9\u01fa\u0001\u0000\u0000\u0000\u01fa\u0201\u0001\u0000"+
		"\u0000\u0000\u01fb\u01f9\u0001\u0000\u0000\u0000\u01fc\u01fd\u0003\u0010"+
		"\b\u0000\u01fd\u01fe\u0006\u0013\uffff\uffff\u0000\u01fe\u0200\u0001\u0000"+
		"\u0000\u0000\u01ff\u01fc\u0001\u0000\u0000\u0000\u0200\u0203\u0001\u0000"+
		"\u0000\u0000\u0201\u01ff\u0001\u0000\u0000\u0000\u0201\u0202\u0001\u0000"+
		"\u0000\u0000\u0202\u0204\u0001\u0000\u0000\u0000\u0203\u0201\u0001\u0000"+
		"\u0000\u0000\u0204\u0205\u0005\n\u0000\u0000\u0205\u0206\u0006\u0013\uffff"+
		"\uffff\u0000\u0206\u0207\u0001\u0000\u0000\u0000\u0207\u0208\u0005V\u0000"+
		"\u0000\u0208\u0209\u0006\u0013\uffff\uffff\u0000\u0209\u0210\u0005,\u0000"+
		"\u0000\u020a\u020b\u0003D\"\u0000\u020b\u020c\u0006\u0013\uffff\uffff"+
		"\u0000\u020c\u020d\u0005\'\u0000\u0000\u020d\u020f\u0001\u0000\u0000\u0000"+
		"\u020e\u020a\u0001\u0000\u0000\u0000\u020f\u0212\u0001\u0000\u0000\u0000"+
		"\u0210\u020e\u0001\u0000\u0000\u0000\u0210\u0211\u0001\u0000\u0000\u0000"+
		"\u0211\u0213\u0001\u0000\u0000\u0000\u0212\u0210\u0001\u0000\u0000\u0000"+
		"\u0213\u0214\u0005-\u0000\u0000\u0214\u0215\u0006\u0013\uffff\uffff\u0000"+
		"\u0215\'\u0001\u0000\u0000\u0000\u0216\u0217\u0005\u000b\u0000\u0000\u0217"+
		"\u0218\u0006\u0014\uffff\uffff\u0000\u0218\u0219\u0001\u0000\u0000\u0000"+
		"\u0219\u021a\u0005V\u0000\u0000\u021a\u021b\u0006\u0014\uffff\uffff\u0000"+
		"\u021b\u0222\u0005,\u0000\u0000\u021c\u021d\u0003D\"\u0000\u021d\u021e"+
		"\u0006\u0014\uffff\uffff\u0000\u021e\u021f\u0005\'\u0000\u0000\u021f\u0221"+
		"\u0001\u0000\u0000\u0000\u0220\u021c\u0001\u0000\u0000\u0000\u0221\u0224"+
		"\u0001\u0000\u0000\u0000\u0222\u0220\u0001\u0000\u0000\u0000\u0222\u0223"+
		"\u0001\u0000\u0000\u0000\u0223\u0225\u0001\u0000\u0000\u0000\u0224\u0222"+
		"\u0001\u0000\u0000\u0000\u0225\u0226\u0005-\u0000\u0000\u0226\u0227\u0006"+
		"\u0014\uffff\uffff\u0000\u0227)\u0001\u0000\u0000\u0000\u0228\u0229\u0003"+
		".\u0017\u0000\u0229\u022a\u0006\u0015\uffff\uffff\u0000\u022a\u0245\u0001"+
		"\u0000\u0000\u0000\u022b\u022c\u00038\u001c\u0000\u022c\u022d\u0006\u0015"+
		"\uffff\uffff\u0000\u022d\u0245\u0001\u0000\u0000\u0000\u022e\u022f\u0003"+
		"0\u0018\u0000\u022f\u0230\u0006\u0015\uffff\uffff\u0000\u0230\u0245\u0001"+
		"\u0000\u0000\u0000\u0231\u0232\u00036\u001b\u0000\u0232\u0233\u0006\u0015"+
		"\uffff\uffff\u0000\u0233\u0245\u0001\u0000\u0000\u0000\u0234\u0235\u0003"+
		">\u001f\u0000\u0235\u0236\u0006\u0015\uffff\uffff\u0000\u0236\u0245\u0001"+
		"\u0000\u0000\u0000\u0237\u0238\u0003:\u001d\u0000\u0238\u0239\u0006\u0015"+
		"\uffff\uffff\u0000\u0239\u0245\u0001\u0000\u0000\u0000\u023a\u023b\u0003"+
		"<\u001e\u0000\u023b\u023c\u0006\u0015\uffff\uffff\u0000\u023c\u0245\u0001"+
		"\u0000\u0000\u0000\u023d\u023e\u0003@ \u0000\u023e\u023f\u0006\u0015\uffff"+
		"\uffff\u0000\u023f\u0245\u0001\u0000\u0000\u0000\u0240\u0241\u0003,\u0016"+
		"\u0000\u0241\u0242\u0005\'\u0000\u0000\u0242\u0243\u0006\u0015\uffff\uffff"+
		"\u0000\u0243\u0245\u0001\u0000\u0000\u0000\u0244\u0228\u0001\u0000\u0000"+
		"\u0000\u0244\u022b\u0001\u0000\u0000\u0000\u0244\u022e\u0001\u0000\u0000"+
		"\u0000\u0244\u0231\u0001\u0000\u0000\u0000\u0244\u0234\u0001\u0000\u0000"+
		"\u0000\u0244\u0237\u0001\u0000\u0000\u0000\u0244\u023a\u0001\u0000\u0000"+
		"\u0000\u0244\u023d\u0001\u0000\u0000\u0000\u0244\u0240\u0001\u0000\u0000"+
		"\u0000\u0245+\u0001\u0000\u0000\u0000\u0246\u0247\u0003B!\u0000\u0247"+
		"\u0248\u0006\u0016\uffff\uffff\u0000\u0248-\u0001\u0000\u0000\u0000\u0249"+
		"\u024a\u0005\f\u0000\u0000\u024a\u024b\u0006\u0017\uffff\uffff\u0000\u024b"+
		"\u024c\u0005*\u0000\u0000\u024c\u024d\u0003D\"\u0000\u024d\u024e\u0006"+
		"\u0017\uffff\uffff\u0000\u024e\u024f\u0005+\u0000\u0000\u024f\u0250\u0003"+
		"*\u0015\u0000\u0250\u0255\u0006\u0017\uffff\uffff\u0000\u0251\u0252\u0005"+
		"\r\u0000\u0000\u0252\u0253\u0003*\u0015\u0000\u0253\u0254\u0006\u0017"+
		"\uffff\uffff\u0000\u0254\u0256\u0001\u0000\u0000\u0000\u0255\u0251\u0001"+
		"\u0000\u0000\u0000\u0255\u0256\u0001\u0000\u0000\u0000\u0256\u0257\u0001"+
		"\u0000\u0000\u0000\u0257\u0258\u0006\u0017\uffff\uffff\u0000\u0258/\u0001"+
		"\u0000\u0000\u0000\u0259\u025a\u0005\u000e\u0000\u0000\u025a\u025b\u0006"+
		"\u0018\uffff\uffff\u0000\u025b\u025f\u0005*\u0000\u0000\u025c\u025d\u0003"+
		"\u0016\u000b\u0000\u025d\u025e\u0006\u0018\uffff\uffff\u0000\u025e\u0260"+
		"\u0001\u0000\u0000\u0000\u025f\u025c\u0001\u0000\u0000\u0000\u025f\u0260"+
		"\u0001\u0000\u0000\u0000\u0260\u0261\u0001\u0000\u0000\u0000\u0261\u0265"+
		"\u0005\'\u0000\u0000\u0262\u0263\u0003D\"\u0000\u0263\u0264\u0006\u0018"+
		"\uffff\uffff\u0000\u0264\u0266\u0001\u0000\u0000\u0000\u0265\u0262\u0001"+
		"\u0000\u0000\u0000\u0265\u0266\u0001\u0000\u0000\u0000\u0266\u0267\u0001"+
		"\u0000\u0000\u0000\u0267\u026b\u0005\'\u0000\u0000\u0268\u0269\u0003D"+
		"\"\u0000\u0269\u026a\u0006\u0018\uffff\uffff\u0000\u026a\u026c\u0001\u0000"+
		"\u0000\u0000\u026b\u0268\u0001\u0000\u0000\u0000\u026b\u026c\u0001\u0000"+
		"\u0000\u0000\u026c\u026d\u0001\u0000\u0000\u0000\u026d\u0273\u0005+\u0000"+
		"\u0000\u026e\u026f\u0003\u0010\b\u0000\u026f\u0270\u0006\u0018\uffff\uffff"+
		"\u0000\u0270\u0272\u0001\u0000\u0000\u0000\u0271\u026e\u0001\u0000\u0000"+
		"\u0000\u0272\u0275\u0001\u0000\u0000\u0000\u0273\u0271\u0001\u0000\u0000"+
		"\u0000\u0273\u0274\u0001\u0000\u0000\u0000\u0274\u0276\u0001\u0000\u0000"+
		"\u0000\u0275\u0273\u0001\u0000\u0000\u0000\u0276\u0277\u0003*\u0015\u0000"+
		"\u0277\u0278\u0006\u0018\uffff\uffff\u0000\u0278\u0279\u0006\u0018\uffff"+
		"\uffff\u0000\u02791\u0001\u0000\u0000\u0000\u027a\u027b\u0005\u000e\u0000"+
		"\u0000\u027b\u027c\u0006\u0019\uffff\uffff\u0000\u027c\u027d\u0005*\u0000"+
		"\u0000\u027d\u027e\u0005V\u0000\u0000\u027e\u027f\u0005)\u0000\u0000\u027f"+
		"\u0280\u0005V\u0000\u0000\u0280\u0281\u0005\u000f\u0000\u0000\u0281\u0282"+
		"\u0005V\u0000\u0000\u0282\u0283\u0005+\u0000\u0000\u0283\u0284\u0003*"+
		"\u0015\u0000\u0284\u0285\u0006\u0019\uffff\uffff\u0000\u0285\u0286\u0006"+
		"\u0019\uffff\uffff\u0000\u02863\u0001\u0000\u0000\u0000\u0287\u0288\u0005"+
		"\u0010\u0000\u0000\u0288\u0289\u0006\u001a\uffff\uffff\u0000\u0289\u028a"+
		"\u0005*\u0000\u0000\u028a\u028b\u0003D\"\u0000\u028b\u028c\u0005+\u0000"+
		"\u0000\u028c\u028d\u0003*\u0015\u0000\u028d\u028e\u0006\u001a\uffff\uffff"+
		"\u0000\u028e\u028f\u0006\u001a\uffff\uffff\u0000\u028f5\u0001\u0000\u0000"+
		"\u0000\u0290\u0291\u0005\u0011\u0000\u0000\u0291\u0292\u0006\u001b\uffff"+
		"\uffff\u0000\u0292\u0293\u0005*\u0000\u0000\u0293\u0294\u0003D\"\u0000"+
		"\u0294\u0295\u0006\u001b\uffff\uffff\u0000\u0295\u0296\u0005+\u0000\u0000"+
		"\u0296\u0297\u0003*\u0015\u0000\u0297\u0298\u0006\u001b\uffff\uffff\u0000"+
		"\u0298\u0299\u0006\u001b\uffff\uffff\u0000\u0299\u02a7\u0001\u0000\u0000"+
		"\u0000\u029a\u029b\u0005\u0012\u0000\u0000\u029b\u029c\u0006\u001b\uffff"+
		"\uffff\u0000\u029c\u029d\u0003*\u0015\u0000\u029d\u029e\u0006\u001b\uffff"+
		"\uffff\u0000\u029e\u029f\u0005\u0011\u0000\u0000\u029f\u02a0\u0005*\u0000"+
		"\u0000\u02a0\u02a1\u0003D\"\u0000\u02a1\u02a2\u0006\u001b\uffff\uffff"+
		"\u0000\u02a2\u02a3\u0005+\u0000\u0000\u02a3\u02a4\u0005\'\u0000\u0000"+
		"\u02a4\u02a5\u0006\u001b\uffff\uffff\u0000\u02a5\u02a7\u0001\u0000\u0000"+
		"\u0000\u02a6\u0290\u0001\u0000\u0000\u0000\u02a6\u029a\u0001\u0000\u0000"+
		"\u0000\u02a77\u0001\u0000\u0000\u0000\u02a8\u02a9\u0005,\u0000\u0000\u02a9"+
		"\u02b4\u0006\u001c\uffff\uffff\u0000\u02aa\u02ab\u0003\u0016\u000b\u0000"+
		"\u02ab\u02ac\u0005\'\u0000\u0000\u02ac\u02ad\u0006\u001c\uffff\uffff\u0000"+
		"\u02ad\u02b3\u0001\u0000\u0000\u0000\u02ae\u02af\u0003*\u0015\u0000\u02af"+
		"\u02b0\u0006\u001c\uffff\uffff\u0000\u02b0\u02b3\u0001\u0000\u0000\u0000"+
		"\u02b1\u02b3\u0003\n\u0005\u0000\u02b2\u02aa\u0001\u0000\u0000\u0000\u02b2"+
		"\u02ae\u0001\u0000\u0000\u0000\u02b2\u02b1\u0001\u0000\u0000\u0000\u02b3"+
		"\u02b6\u0001\u0000\u0000\u0000\u02b4\u02b2\u0001\u0000\u0000\u0000\u02b4"+
		"\u02b5\u0001\u0000\u0000\u0000\u02b5\u02b7\u0001\u0000\u0000\u0000\u02b6"+
		"\u02b4\u0001\u0000\u0000\u0000\u02b7\u02b8\u0005-\u0000\u0000\u02b8\u02b9"+
		"\u0006\u001c\uffff\uffff\u0000\u02b9\u02ba\u0006\u001c\uffff\uffff\u0000"+
		"\u02ba9\u0001\u0000\u0000\u0000\u02bb\u02bc\u0005\u0013\u0000\u0000\u02bc"+
		"\u02c0\u0006\u001d\uffff\uffff\u0000\u02bd\u02be\u0003D\"\u0000\u02be"+
		"\u02bf\u0006\u001d\uffff\uffff\u0000\u02bf\u02c1\u0001\u0000\u0000\u0000"+
		"\u02c0\u02bd\u0001\u0000\u0000\u0000\u02c0\u02c1\u0001\u0000\u0000\u0000"+
		"\u02c1\u02c2\u0001\u0000\u0000\u0000\u02c2\u02c3\u0005\'\u0000\u0000\u02c3"+
		"\u02d1\u0006\u001d\uffff\uffff\u0000\u02c4\u02c5\u0005\u0014\u0000\u0000"+
		"\u02c5\u02c6\u0006\u001d\uffff\uffff\u0000\u02c6\u02c7\u0005\'\u0000\u0000"+
		"\u02c7\u02d1\u0006\u001d\uffff\uffff\u0000\u02c8\u02c9\u0005\u0015\u0000"+
		"\u0000\u02c9\u02ca\u0006\u001d\uffff\uffff\u0000\u02ca\u02cb\u0005\'\u0000"+
		"\u0000\u02cb\u02d1\u0006\u001d\uffff\uffff\u0000\u02cc\u02cd\u0005\u0016"+
		"\u0000\u0000\u02cd\u02ce\u0006\u001d\uffff\uffff\u0000\u02ce\u02cf\u0005"+
		"\'\u0000\u0000\u02cf\u02d1\u0006\u001d\uffff\uffff\u0000\u02d0\u02bb\u0001"+
		"\u0000\u0000\u0000\u02d0\u02c4\u0001\u0000\u0000\u0000\u02d0\u02c8\u0001"+
		"\u0000\u0000\u0000\u02d0\u02cc\u0001\u0000\u0000\u0000\u02d1;\u0001\u0000"+
		"\u0000\u0000\u02d2\u02d3\u0005\u0017\u0000\u0000\u02d3\u02d4\u0006\u001e"+
		"\uffff\uffff\u0000\u02d4\u02d5\u0005\'\u0000\u0000\u02d5\u02d6\u0006\u001e"+
		"\uffff\uffff\u0000\u02d6=\u0001\u0000\u0000\u0000\u02d7\u02d8\u0005\u0018"+
		"\u0000\u0000\u02d8\u02d9\u0006\u001f\uffff\uffff\u0000\u02d9\u02da\u0005"+
		"*\u0000\u0000\u02da\u02db\u0003D\"\u0000\u02db\u02dc\u0005+\u0000\u0000"+
		"\u02dc\u02dd\u0006\u001f\uffff\uffff\u0000\u02dd\u02e9\u0005,\u0000\u0000"+
		"\u02de\u02df\u0005\u0019\u0000\u0000\u02df\u02e0\u0003D\"\u0000\u02e0"+
		"\u02e1\u0005)\u0000\u0000\u02e1\u02e5\u0006\u001f\uffff\uffff\u0000\u02e2"+
		"\u02e3\u0003*\u0015\u0000\u02e3\u02e4\u0006\u001f\uffff\uffff\u0000\u02e4"+
		"\u02e6\u0001\u0000\u0000\u0000\u02e5\u02e2\u0001\u0000\u0000\u0000\u02e5"+
		"\u02e6\u0001\u0000\u0000\u0000\u02e6\u02e8\u0001\u0000\u0000\u0000\u02e7"+
		"\u02de\u0001\u0000\u0000\u0000\u02e8\u02eb\u0001\u0000\u0000\u0000\u02e9"+
		"\u02e7\u0001\u0000\u0000\u0000\u02e9\u02ea\u0001\u0000\u0000\u0000\u02ea"+
		"\u02f3\u0001\u0000\u0000\u0000\u02eb\u02e9\u0001\u0000\u0000\u0000\u02ec"+
		"\u02ed\u0005\u001a\u0000\u0000\u02ed\u02f1\u0005)\u0000\u0000\u02ee\u02ef"+
		"\u0003*\u0015\u0000\u02ef\u02f0\u0006\u001f\uffff\uffff\u0000\u02f0\u02f2"+
		"\u0001\u0000\u0000\u0000\u02f1\u02ee\u0001\u0000\u0000\u0000\u02f1\u02f2"+
		"\u0001\u0000\u0000\u0000\u02f2\u02f4\u0001\u0000\u0000\u0000\u02f3\u02ec"+
		"\u0001\u0000\u0000\u0000\u02f3\u02f4\u0001\u0000\u0000\u0000\u02f4\u02f5"+
		"\u0001\u0000\u0000\u0000\u02f5\u02f6\u0005-\u0000\u0000\u02f6\u02f7\u0006"+
		"\u001f\uffff\uffff\u0000\u02f7?\u0001\u0000\u0000\u0000\u02f8\u02f9\u0005"+
		"\u001b\u0000\u0000\u02f9\u02fa\u0006 \uffff\uffff\u0000\u02fa\u02fb\u0005"+
		"\'\u0000\u0000\u02fb\u02fc\u0006 \uffff\uffff\u0000\u02fcA\u0001\u0000"+
		"\u0000\u0000\u02fd\u02ff\u0003\n\u0005\u0000\u02fe\u02fd\u0001\u0000\u0000"+
		"\u0000\u02fe\u02ff\u0001\u0000\u0000\u0000\u02ff\u0300\u0001\u0000\u0000"+
		"\u0000\u0300\u0301\u0003D\"\u0000\u0301\u0309\u0006!\uffff\uffff\u0000"+
		"\u0302\u0303\u0005(\u0000\u0000\u0303\u0304\u0003D\"\u0000\u0304\u0305"+
		"\u0006!\uffff\uffff\u0000\u0305\u0308\u0001\u0000\u0000\u0000\u0306\u0308"+
		"\u0003\n\u0005\u0000\u0307\u0302\u0001\u0000\u0000\u0000\u0307\u0306\u0001"+
		"\u0000\u0000\u0000\u0308\u030b\u0001\u0000\u0000\u0000\u0309\u0307\u0001"+
		"\u0000\u0000\u0000\u0309\u030a\u0001\u0000\u0000\u0000\u030a\u030c\u0001"+
		"\u0000\u0000\u0000\u030b\u0309\u0001\u0000\u0000\u0000\u030c\u030d\u0006"+
		"!\uffff\uffff\u0000\u030dC\u0001\u0000\u0000\u0000\u030e\u030f\u0006\""+
		"\uffff\uffff\u0000\u030f\u0310\u0007\u0003\u0000\u0000\u0310\u0311\u0003"+
		"D\"\u000e\u0311\u0312\u0006\"\uffff\uffff\u0000\u0312\u0317\u0001\u0000"+
		"\u0000\u0000\u0313\u0314\u0003F#\u0000\u0314\u0315\u0006\"\uffff\uffff"+
		"\u0000\u0315\u0317\u0001\u0000\u0000\u0000\u0316\u030e\u0001\u0000\u0000"+
		"\u0000\u0316\u0313\u0001\u0000\u0000\u0000\u0317\u0396\u0001\u0000\u0000"+
		"\u0000\u0318\u0319\n\u0011\u0000\u0000\u0319\u031a\u0006\"\uffff\uffff"+
		"\u0000\u031a\u031b\u00050\u0000\u0000\u031b\u031c\u0006\"\uffff\uffff"+
		"\u0000\u031c\u031d\u0003D\"\u0012\u031d\u031e\u0006\"\uffff\uffff\u0000"+
		"\u031e\u0395\u0001\u0000\u0000\u0000\u031f\u0320\n\u0010\u0000\u0000\u0320"+
		"\u0321\u0006\"\uffff\uffff\u0000\u0321\u0322\u0005N\u0000\u0000\u0322"+
		"\u0323\u0006\"\uffff\uffff\u0000\u0323\u0324\u0003D\"\u0011\u0324\u0325"+
		"\u0006\"\uffff\uffff\u0000\u0325\u0395\u0001\u0000\u0000\u0000\u0326\u0327"+
		"\n\r\u0000\u0000\u0327\u0328\u0006\"\uffff\uffff\u0000\u0328\u0329\u0007"+
		"\u0004\u0000\u0000\u0329\u032a\u0006\"\uffff\uffff\u0000\u032a\u032b\u0003"+
		"D\"\u000e\u032b\u032c\u0006\"\uffff\uffff\u0000\u032c\u0395\u0001\u0000"+
		"\u0000\u0000\u032d\u032e\n\f\u0000\u0000\u032e\u032f\u0006\"\uffff\uffff"+
		"\u0000\u032f\u0330\u0007\u0005\u0000\u0000\u0330\u0331\u0006\"\uffff\uffff"+
		"\u0000\u0331\u0332\u0003D\"\r\u0332\u0333\u0006\"\uffff\uffff\u0000\u0333"+
		"\u0395\u0001\u0000\u0000\u0000\u0334\u0335\n\u000b\u0000\u0000\u0335\u0336"+
		"\u0006\"\uffff\uffff\u0000\u0336\u0337\u0007\u0006\u0000\u0000\u0337\u0338"+
		"\u0006\"\uffff\uffff\u0000\u0338\u0339\u0003D\"\f\u0339\u033a\u0006\""+
		"\uffff\uffff\u0000\u033a\u0395\u0001\u0000\u0000\u0000\u033b\u033c\n\n"+
		"\u0000\u0000\u033c\u033d\u0006\"\uffff\uffff\u0000\u033d\u033e\u0007\u0007"+
		"\u0000\u0000\u033e\u033f\u0006\"\uffff\uffff\u0000\u033f\u0340\u0003D"+
		"\"\u000b\u0340\u0341\u0006\"\uffff\uffff\u0000\u0341\u0395\u0001\u0000"+
		"\u0000\u0000\u0342\u0343\n\t\u0000\u0000\u0343\u0344\u0006\"\uffff\uffff"+
		"\u0000\u0344\u0345\u0007\b\u0000\u0000\u0345\u0346\u0006\"\uffff\uffff"+
		"\u0000\u0346\u0347\u0003D\"\n\u0347\u0348\u0006\"\uffff\uffff\u0000\u0348"+
		"\u0395\u0001\u0000\u0000\u0000\u0349\u034a\n\b\u0000\u0000\u034a\u034b"+
		"\u0006\"\uffff\uffff\u0000\u034b\u034c\u00055\u0000\u0000\u034c\u034d"+
		"\u0006\"\uffff\uffff\u0000\u034d\u034e\u0003D\"\t\u034e\u034f\u0006\""+
		"\uffff\uffff\u0000\u034f\u0395\u0001\u0000\u0000\u0000\u0350\u0351\n\u0007"+
		"\u0000\u0000\u0351\u0352\u0006\"\uffff\uffff\u0000\u0352\u0353\u00059"+
		"\u0000\u0000\u0353\u0354\u0006\"\uffff\uffff\u0000\u0354\u0355\u0003D"+
		"\"\b\u0355\u0356\u0006\"\uffff\uffff\u0000\u0356\u0395\u0001\u0000\u0000"+
		"\u0000\u0357\u0358\n\u0006\u0000\u0000\u0358\u0359\u0006\"\uffff\uffff"+
		"\u0000\u0359\u035a\u00057\u0000\u0000\u035a\u035b\u0006\"\uffff\uffff"+
		"\u0000\u035b\u035c\u0003D\"\u0007\u035c\u035d\u0006\"\uffff\uffff\u0000"+
		"\u035d\u0395\u0001\u0000\u0000\u0000\u035e\u035f\n\u0005\u0000\u0000\u035f"+
		"\u0360\u0006\"\uffff\uffff\u0000\u0360\u0361\u0005E\u0000\u0000\u0361"+
		"\u0362\u0006\"\uffff\uffff\u0000\u0362\u0363\u0003D\"\u0006\u0363\u0364"+
		"\u0006\"\uffff\uffff\u0000\u0364\u0395\u0001\u0000\u0000\u0000\u0365\u0366"+
		"\n\u0004\u0000\u0000\u0366\u0367\u0006\"\uffff\uffff\u0000\u0367\u0368"+
		"\u0005F\u0000\u0000\u0368\u0369\u0006\"\uffff\uffff\u0000\u0369\u036a"+
		"\u0003D\"\u0005\u036a\u036b\u0006\"\uffff\uffff\u0000\u036b\u0395\u0001"+
		"\u0000\u0000\u0000\u036c\u036d\n\u0003\u0000\u0000\u036d\u036e\u00054"+
		"\u0000\u0000\u036e\u036f\u0006\"\uffff\uffff\u0000\u036f\u0370\u0003D"+
		"\"\u0000\u0370\u0371\u0005)\u0000\u0000\u0371\u0372\u0003D\"\u0003\u0372"+
		"\u0373\u0006\"\uffff\uffff\u0000\u0373\u0395\u0001\u0000\u0000\u0000\u0374"+
		"\u0375\n\u0002\u0000\u0000\u0375\u0376\u0006\"\uffff\uffff\u0000\u0376"+
		"\u0377\u0007\t\u0000\u0000\u0377\u0378\u0006\"\uffff\uffff\u0000\u0378"+
		"\u0379\u0003D\"\u0002\u0379\u037a\u0006\"\uffff\uffff\u0000\u037a\u0395"+
		"\u0001\u0000\u0000\u0000\u037b\u037c\n\u0013\u0000\u0000\u037c\u037d\u0006"+
		"\"\uffff\uffff\u0000\u037d\u037e\u0007\n\u0000\u0000\u037e\u037f\u0006"+
		"\"\uffff\uffff\u0000\u037f\u0395\u0006\"\uffff\uffff\u0000\u0380\u0381"+
		"\n\u0012\u0000\u0000\u0381\u0382\u0006\"\uffff\uffff\u0000\u0382\u0383"+
		"\u0005*\u0000\u0000\u0383\u0387\u0006\"\uffff\uffff\u0000\u0384\u0385"+
		"\u0003B!\u0000\u0385\u0386\u0006\"\uffff\uffff\u0000\u0386\u0388\u0001"+
		"\u0000\u0000\u0000\u0387\u0384\u0001\u0000\u0000\u0000\u0387\u0388\u0001"+
		"\u0000\u0000\u0000\u0388\u0389\u0001\u0000\u0000\u0000\u0389\u038a\u0005"+
		"+\u0000\u0000\u038a\u0395\u0006\"\uffff\uffff\u0000\u038b\u038c\n\u000f"+
		"\u0000\u0000\u038c\u038d\u0006\"\uffff\uffff\u0000\u038d\u038e\u0005."+
		"\u0000\u0000\u038e\u0390\u0006\"\uffff\uffff\u0000\u038f\u0391\u0003D"+
		"\"\u0000\u0390\u038f\u0001\u0000\u0000\u0000\u0390\u0391\u0001\u0000\u0000"+
		"\u0000\u0391\u0392\u0001\u0000\u0000\u0000\u0392\u0393\u0005/\u0000\u0000"+
		"\u0393\u0395\u0006\"\uffff\uffff\u0000\u0394\u0318\u0001\u0000\u0000\u0000"+
		"\u0394\u031f\u0001\u0000\u0000\u0000\u0394\u0326\u0001\u0000\u0000\u0000"+
		"\u0394\u032d\u0001\u0000\u0000\u0000\u0394\u0334\u0001\u0000\u0000\u0000"+
		"\u0394\u033b\u0001\u0000\u0000\u0000\u0394\u0342\u0001\u0000\u0000\u0000"+
		"\u0394\u0349\u0001\u0000\u0000\u0000\u0394\u0350\u0001\u0000\u0000\u0000"+
		"\u0394\u0357\u0001\u0000\u0000\u0000\u0394\u035e\u0001\u0000\u0000\u0000"+
		"\u0394\u0365\u0001\u0000\u0000\u0000\u0394\u036c\u0001\u0000\u0000\u0000"+
		"\u0394\u0374\u0001\u0000\u0000\u0000\u0394\u037b\u0001\u0000\u0000\u0000"+
		"\u0394\u0380\u0001\u0000\u0000\u0000\u0394\u038b\u0001\u0000\u0000\u0000"+
		"\u0395\u0398\u0001\u0000\u0000\u0000\u0396\u0394\u0001\u0000\u0000\u0000"+
		"\u0396\u0397\u0001\u0000\u0000\u0000\u0397E\u0001\u0000\u0000\u0000\u0398"+
		"\u0396\u0001\u0000\u0000\u0000\u0399\u039a\u0003H$\u0000\u039a\u039b\u0006"+
		"#\uffff\uffff\u0000\u039b\u03b8\u0001\u0000\u0000\u0000\u039c\u039d\u0003"+
		"J%\u0000\u039d\u039e\u0006#\uffff\uffff\u0000\u039e\u03b8\u0001\u0000"+
		"\u0000\u0000\u039f\u03a0\u0005*\u0000\u0000\u03a0\u03a1\u0003D\"\u0000"+
		"\u03a1\u03a2\u0005+\u0000\u0000\u03a2\u03a3\u0006#\uffff\uffff\u0000\u03a3"+
		"\u03b8\u0001\u0000\u0000\u0000\u03a4\u03a5\u0005O\u0000\u0000\u03a5\u03b8"+
		"\u0006#\uffff\uffff\u0000\u03a6\u03a7\u0005P\u0000\u0000\u03a7\u03b8\u0006"+
		"#\uffff\uffff\u0000\u03a8\u03a9\u0005S\u0000\u0000\u03a9\u03b8\u0006#"+
		"\uffff\uffff\u0000\u03aa\u03ab\u0005T\u0000\u0000\u03ab\u03b8\u0006#\uffff"+
		"\uffff\u0000\u03ac\u03ad\u0005U\u0000\u0000\u03ad\u03b8\u0006#\uffff\uffff"+
		"\u0000\u03ae\u03af\u0003\u0000\u0000\u0000\u03af\u03b0\u0006#\uffff\uffff"+
		"\u0000\u03b0\u03b8\u0001\u0000\u0000\u0000\u03b1\u03b2\u0005V\u0000\u0000"+
		"\u03b2\u03b8\u0006#\uffff\uffff\u0000\u03b3\u03b4\u0003\u0004\u0002\u0000"+
		"\u03b4\u03b5\u0006#\uffff\uffff\u0000\u03b5\u03b8\u0001\u0000\u0000\u0000"+
		"\u03b6\u03b8\u0003\n\u0005\u0000\u03b7\u0399\u0001\u0000\u0000\u0000\u03b7"+
		"\u039c\u0001\u0000\u0000\u0000\u03b7\u039f\u0001\u0000\u0000\u0000\u03b7"+
		"\u03a4\u0001\u0000\u0000\u0000\u03b7\u03a6\u0001\u0000\u0000\u0000\u03b7"+
		"\u03a8\u0001\u0000\u0000\u0000\u03b7\u03aa\u0001\u0000\u0000\u0000\u03b7"+
		"\u03ac\u0001\u0000\u0000\u0000\u03b7\u03ae\u0001\u0000\u0000\u0000\u03b7"+
		"\u03b1\u0001\u0000\u0000\u0000\u03b7\u03b3\u0001\u0000\u0000\u0000\u03b7"+
		"\u03b6\u0001\u0000\u0000\u0000\u03b8G\u0001\u0000\u0000\u0000\u03b9\u03ba"+
		"\u0005V\u0000\u0000\u03ba\u03bb\u0006$\uffff\uffff\u0000\u03bb\u03bc\u0005"+
		",\u0000\u0000\u03bc\u03be\u0006$\uffff\uffff\u0000\u03bd\u03bf\u0003B"+
		"!\u0000\u03be\u03bd\u0001\u0000\u0000\u0000\u03be\u03bf\u0001\u0000\u0000"+
		"\u0000\u03bf\u03c0\u0001\u0000\u0000\u0000\u03c0\u03c1\u0005-\u0000\u0000"+
		"\u03c1\u03c2\u0006$\uffff\uffff\u0000\u03c2I\u0001\u0000\u0000\u0000\u03c3"+
		"\u03c4\u0005.\u0000\u0000\u03c4\u03c6\u0006%\uffff\uffff\u0000\u03c5\u03c7"+
		"\u0003B!\u0000\u03c6\u03c5\u0001\u0000\u0000\u0000\u03c6\u03c7\u0001\u0000"+
		"\u0000\u0000\u03c7\u03c8\u0001\u0000\u0000\u0000\u03c8\u03c9\u0005/\u0000"+
		"\u0000\u03c9\u03ca\u0006%\uffff\uffff\u0000\u03caK\u0001\u0000\u0000\u0000"+
		"\u03cb\u03cc\u0005?\u0000\u0000\u03cc\u03cd\u0006&\uffff\uffff\u0000\u03cd"+
		"\u03ce\u0005S\u0000\u0000\u03ce\u03cf\u0006&\uffff\uffff\u0000\u03cf\u03d1"+
		"\u0001\u0000\u0000\u0000\u03d0\u03d2\u0003\n\u0005\u0000\u03d1\u03d0\u0001"+
		"\u0000\u0000\u0000\u03d1\u03d2\u0001\u0000\u0000\u0000\u03d2\u03d6\u0001"+
		"\u0000\u0000\u0000\u03d3\u03d4\u0005(\u0000\u0000\u03d4\u03d5\u0005S\u0000"+
		"\u0000\u03d5\u03d7\u0006&\uffff\uffff\u0000\u03d6\u03d3\u0001\u0000\u0000"+
		"\u0000\u03d7\u03d8\u0001\u0000\u0000\u0000\u03d8\u03d6\u0001\u0000\u0000"+
		"\u0000\u03d8\u03d9\u0001\u0000\u0000\u0000\u03d9\u03da\u0001\u0000\u0000"+
		"\u0000\u03da\u03db\u0005A\u0000\u0000\u03db\u03dc\u0006&\uffff\uffff\u0000"+
		"\u03dcM\u0001\u0000\u0000\u0000\u03dd\u03de\u0005?\u0000\u0000\u03de\u03df"+
		"\u0006\'\uffff\uffff\u0000\u03df\u03e0\u0005T\u0000\u0000\u03e0\u03e1"+
		"\u0006\'\uffff\uffff\u0000\u03e1\u03e3\u0001\u0000\u0000\u0000\u03e2\u03e4"+
		"\u0003\n\u0005\u0000\u03e3\u03e2\u0001\u0000\u0000\u0000\u03e3\u03e4\u0001"+
		"\u0000\u0000\u0000\u03e4\u03e8\u0001\u0000\u0000\u0000\u03e5\u03e6\u0005"+
		"(\u0000\u0000\u03e6\u03e7\u0005T\u0000\u0000\u03e7\u03e9\u0006\'\uffff"+
		"\uffff\u0000\u03e8\u03e5\u0001\u0000\u0000\u0000\u03e9\u03ea\u0001\u0000"+
		"\u0000\u0000\u03ea\u03e8\u0001\u0000\u0000\u0000\u03ea\u03eb\u0001\u0000"+
		"\u0000\u0000\u03eb\u03ec\u0001\u0000\u0000\u0000\u03ec\u03ed\u0005A\u0000"+
		"\u0000\u03ed\u03ee\u0006\'\uffff\uffff\u0000\u03eeO\u0001\u0000\u0000"+
		"\u0000\u03ef\u03f0\u0005?\u0000\u0000\u03f0\u03f1\u0006(\uffff\uffff\u0000"+
		"\u03f1\u03f2\u0005O\u0000\u0000\u03f2\u03f3\u0006(\uffff\uffff\u0000\u03f3"+
		"\u03f5\u0001\u0000\u0000\u0000\u03f4\u03f6\u0003\n\u0005\u0000\u03f5\u03f4"+
		"\u0001\u0000\u0000\u0000\u03f5\u03f6\u0001\u0000\u0000\u0000\u03f6\u03fa"+
		"\u0001\u0000\u0000\u0000\u03f7\u03f8\u0005(\u0000\u0000\u03f8\u03f9\u0005"+
		"O\u0000\u0000\u03f9\u03fb\u0006(\uffff\uffff\u0000\u03fa\u03f7\u0001\u0000"+
		"\u0000\u0000\u03fb\u03fc\u0001\u0000\u0000\u0000\u03fc\u03fa\u0001\u0000"+
		"\u0000\u0000\u03fc\u03fd\u0001\u0000\u0000\u0000\u03fd\u03fe\u0001\u0000"+
		"\u0000\u0000\u03fe\u03ff\u0005A\u0000\u0000\u03ff\u0400\u0006(\uffff\uffff"+
		"\u0000\u0400Q\u0001\u0000\u0000\u0000\u0401\u0402\u0005?\u0000\u0000\u0402"+
		"\u0403\u0006)\uffff\uffff\u0000\u0403\u0404\u0005P\u0000\u0000\u0404\u0405"+
		"\u0006)\uffff\uffff\u0000\u0405\u0407\u0001\u0000\u0000\u0000\u0406\u0408"+
		"\u0003\n\u0005\u0000\u0407\u0406\u0001\u0000\u0000\u0000\u0407\u0408\u0001"+
		"\u0000\u0000\u0000\u0408\u040c\u0001\u0000\u0000\u0000\u0409\u040a\u0005"+
		"(\u0000\u0000\u040a\u040b\u0005P\u0000\u0000\u040b\u040d\u0006)\uffff"+
		"\uffff\u0000\u040c\u0409\u0001\u0000\u0000\u0000\u040d\u040e\u0001\u0000"+
		"\u0000\u0000\u040e\u040c\u0001\u0000\u0000\u0000\u040e\u040f\u0001\u0000"+
		"\u0000\u0000\u040f\u0410\u0001\u0000\u0000\u0000\u0410\u0411\u0005A\u0000"+
		"\u0000\u0411\u0412\u0006)\uffff\uffff\u0000\u0412S\u0001\u0000\u0000\u0000"+
		"\u0413\u0414\u0005?\u0000\u0000\u0414\u0415\u0006*\uffff\uffff\u0000\u0415"+
		"\u0416\u0003\u0004\u0002\u0000\u0416\u0417\u0006*\uffff\uffff\u0000\u0417"+
		"\u0419\u0001\u0000\u0000\u0000\u0418\u041a\u0003\n\u0005\u0000\u0419\u0418"+
		"\u0001\u0000\u0000\u0000\u0419\u041a\u0001\u0000\u0000\u0000\u041a\u041f"+
		"\u0001\u0000\u0000\u0000\u041b\u041c\u0005(\u0000\u0000\u041c\u041d\u0003"+
		"\u0004\u0002\u0000\u041d\u041e\u0006*\uffff\uffff\u0000\u041e\u0420\u0001"+
		"\u0000\u0000\u0000\u041f\u041b\u0001\u0000\u0000\u0000\u0420\u0421\u0001"+
		"\u0000\u0000\u0000\u0421\u041f\u0001\u0000\u0000\u0000\u0421\u0422\u0001"+
		"\u0000\u0000\u0000\u0422\u0423\u0001\u0000\u0000\u0000\u0423\u0424\u0005"+
		"A\u0000\u0000\u0424\u0425\u0006*\uffff\uffff\u0000\u0425U\u0001\u0000"+
		"\u0000\u0000Y[dhox|\u0082\u00b0\u00b2\u00d4\u00de\u00e4\u00e6\u00f3\u00fb"+
		"\u0103\u010b\u0110\u0116\u0118\u011f\u0129\u012b\u012e\u0135\u013d\u0145"+
		"\u015a\u015d\u015f\u0167\u0169\u0176\u017f\u0183\u018c\u0190\u0192\u0196"+
		"\u019e\u01a6\u01b2\u01bb\u01c4\u01cb\u01cd\u01d0\u01e0\u01ee\u01f9\u0201"+
		"\u0210\u0222\u0244\u0255\u025f\u0265\u026b\u0273\u02a6\u02b2\u02b4\u02c0"+
		"\u02d0\u02e5\u02e9\u02f1\u02f3\u02fe\u0307\u0309\u0316\u0387\u0390\u0394"+
		"\u0396\u03b7\u03be\u03c6\u03d1\u03d8\u03e3\u03ea\u03f5\u03fc\u0407\u040e"+
		"\u0419\u0421";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}