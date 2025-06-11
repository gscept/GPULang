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
		T__31=32, T__32=33, T__33=34, T__34=35, T__35=36, T__36=37, SC=38, CO=39, 
		COL=40, LP=41, RP=42, LB=43, RB=44, LL=45, RR=46, DOT=47, NOT=48, EQ=49, 
		QO=50, QU=51, AND=52, ANDSET=53, OR=54, ORSET=55, XOR=56, XORSET=57, CONJUGATE=58, 
		Q=59, NU=60, FORWARDSLASH=61, LESS=62, LESSEQ=63, GREATER=64, GREATEREQ=65, 
		LOGICEQ=66, NOTEQ=67, LOGICAND=68, LOGICOR=69, MOD=70, UNDERSC=71, SOBAKA=72, 
		ADD_OP=73, SUB_OP=74, DIV_OP=75, MUL_OP=76, ARROW=77, INTEGERLITERAL=78, 
		UINTEGERLITERAL=79, COMMENT=80, ML_COMMENT=81, FLOATLITERAL=82, DOUBLELITERAL=83, 
		HEX=84, IDENTIFIER=85, WS=86;
	public static final int
		RULE_string = 0, RULE_path = 1, RULE_boolean = 2, RULE_entry = 3, RULE_effect = 4, 
		RULE_linePreprocessorEntry = 5, RULE_alias = 6, RULE_annotation = 7, RULE_attribute = 8, 
		RULE_typeDeclaration = 9, RULE_variables = 10, RULE_structureDeclaration = 11, 
		RULE_structure = 12, RULE_enumeration = 13, RULE_parameter = 14, RULE_functionDeclaration = 15, 
		RULE_function = 16, RULE_program = 17, RULE_sampler = 18, RULE_state = 19, 
		RULE_statement = 20, RULE_expressionStatement = 21, RULE_ifStatement = 22, 
		RULE_forStatement = 23, RULE_forRangeStatement = 24, RULE_forUniformValueStatement = 25, 
		RULE_whileStatement = 26, RULE_scopeStatement = 27, RULE_terminateStatement = 28, 
		RULE_continueStatement = 29, RULE_switchStatement = 30, RULE_breakStatement = 31, 
		RULE_expressionNoComma = 32, RULE_expression = 33, RULE_binaryexpatom = 34, 
		RULE_initializerExpression = 35, RULE_arrayInitializerExpression = 36, 
		RULE_floatVecLiteralExpression = 37, RULE_doubleVecLiteralExpression = 38, 
		RULE_intVecLiteralExpression = 39, RULE_uintVecLiteralExpression = 40, 
		RULE_booleanVecLiteralExpression = 41;
	private static String[] makeRuleNames() {
		return new String[] {
			"string", "path", "boolean", "entry", "effect", "linePreprocessorEntry", 
			"alias", "annotation", "attribute", "typeDeclaration", "variables", "structureDeclaration", 
			"structure", "enumeration", "parameter", "functionDeclaration", "function", 
			"program", "sampler", "state", "statement", "expressionStatement", "ifStatement", 
			"forStatement", "forRangeStatement", "forUniformValueStatement", "whileStatement", 
			"scopeStatement", "terminateStatement", "continueStatement", "switchStatement", 
			"breakStatement", "expressionNoComma", "expression", "binaryexpatom", 
			"initializerExpression", "arrayInitializerExpression", "floatVecLiteralExpression", 
			"doubleVecLiteralExpression", "intVecLiteralExpression", "uintVecLiteralExpression", 
			"booleanVecLiteralExpression"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, "'true'", "'false'", "'#line'", "'alias'", "'as'", "'struct'", 
			"'enum'", "'program'", "'sampler_state'", "'render_state'", "'if'", "'else'", 
			"'for'", "'..'", "'for_uniform'", "'while'", "'do'", "'return'", "'discard'", 
			"'ray_ignore'", "'ray_terminate'", "'continue'", "'switch'", "'case'", 
			"'default'", "'break'", "'++'", "'--'", "'<<'", "'>>'", "'+='", "'-='", 
			"'*='", "'/='", "'%='", "'<<='", "'>>='", "';'", "','", "':'", "'('", 
			"')'", "'{'", "'}'", "'['", "']'", "'.'", "'!'", "'='", "'\"'", "'?'", 
			"'&'", "'&='", "'|'", "'|='", "'^'", "'^='", "'~'", "'''", "'#'", "'\\'", 
			"'<'", "'<='", "'>'", "'>='", "'=='", "'!='", "'&&'", "'||'", "'%'", 
			"'_'", "'@'", "'+'", "'-'", "'/'", "'*'", "'->'"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, "SC", "CO", "COL", "LP", "RP", "LB", "RB", "LL", "RR", "DOT", 
			"NOT", "EQ", "QO", "QU", "AND", "ANDSET", "OR", "ORSET", "XOR", "XORSET", 
			"CONJUGATE", "Q", "NU", "FORWARDSLASH", "LESS", "LESSEQ", "GREATER", 
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
			setState(102);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case QO:
				enterOuterAlt(_localctx, 1);
				{
				setState(84);
				match(QO);
				setState(89);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while ((((_la) & ~0x3f) == 0 && ((1L << _la) & -1125899906842626L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & 8388607L) != 0)) {
					{
					{
					setState(85);
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
					setState(91);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				setState(92);
				match(QO);
				}
				break;
			case Q:
				enterOuterAlt(_localctx, 2);
				{
				setState(93);
				match(Q);
				setState(98);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while ((((_la) & ~0x3f) == 0 && ((1L << _la) & -576460752303423490L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & 8388607L) != 0)) {
					{
					{
					setState(94);
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
					setState(100);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				setState(101);
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
			setState(122);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case QO:
				enterOuterAlt(_localctx, 1);
				{
				setState(104);
				match(QO);
				setState(109);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while ((((_la) & ~0x3f) == 0 && ((1L << _la) & -1125899906842626L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & 8388607L) != 0)) {
					{
					{
					setState(105);
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
					setState(111);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				setState(112);
				match(QO);
				}
				break;
			case LESS:
				enterOuterAlt(_localctx, 2);
				{
				setState(113);
				match(LESS);
				setState(118);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while ((((_la) & ~0x3f) == 0 && ((1L << _la) & -2L) != 0) || ((((_la - 65)) & ~0x3f) == 0 && ((1L << (_la - 65)) & 4194303L) != 0)) {
					{
					{
					setState(114);
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
					setState(120);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				setState(121);
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
			setState(128);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__0:
				enterOuterAlt(_localctx, 1);
				{
				setState(124);
				match(T__0);
				 ((BooleanContext)_localctx).val =  true; 
				}
				break;
			case T__1:
				enterOuterAlt(_localctx, 2);
				{
				setState(126);
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
			setState(130);
			((EntryContext)_localctx).effect = effect();

			        ((EntryContext)_localctx).returnEffect =  ((EntryContext)_localctx).effect.eff;
			    
			setState(132);
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
		public List<AliasContext> alias() {
			return getRuleContexts(AliasContext.class);
		}
		public AliasContext alias(int i) {
			return getRuleContext(AliasContext.class,i);
		}
		public List<TerminalNode> SC() { return getTokens(GPULangParser.SC); }
		public TerminalNode SC(int i) {
			return getToken(GPULangParser.SC, i);
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
			setState(172);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,8,_ctx);
			while ( _alt!=1 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1+1 ) {
					{
					setState(170);
					_errHandler.sync(this);
					switch ( getInterpreter().adaptivePredict(_input,7,_ctx) ) {
					case 1:
						{
						setState(134);
						linePreprocessorEntry();
						}
						break;
					case 2:
						{
						setState(135);
						((EffectContext)_localctx).alias = alias();
						setState(136);
						match(SC);
						 _localctx.eff->symbols.Append(((EffectContext)_localctx).alias.sym); 
						}
						break;
					case 3:
						{
						setState(139);
						((EffectContext)_localctx).functionDeclaration = functionDeclaration();
						setState(140);
						match(SC);
						 _localctx.eff->symbols.Append(((EffectContext)_localctx).functionDeclaration.sym); 
						}
						break;
					case 4:
						{
						setState(143);
						((EffectContext)_localctx).function = function();
						 _localctx.eff->symbols.Append(((EffectContext)_localctx).function.sym); 
						}
						break;
					case 5:
						{
						setState(146);
						((EffectContext)_localctx).variables = variables();
						setState(147);
						match(SC);
						 for (Variable* var : ((EffectContext)_localctx).variables.vars) { _localctx.eff->symbols.Append(var); } 
						}
						break;
					case 6:
						{
						setState(150);
						((EffectContext)_localctx).structure = structure();
						setState(151);
						match(SC);
						 _localctx.eff->symbols.Append(((EffectContext)_localctx).structure.sym); 
						}
						break;
					case 7:
						{
						setState(154);
						((EffectContext)_localctx).enumeration = enumeration();
						setState(155);
						match(SC);
						 _localctx.eff->symbols.Append(((EffectContext)_localctx).enumeration.sym); 
						}
						break;
					case 8:
						{
						setState(158);
						((EffectContext)_localctx).state = state();
						setState(159);
						match(SC);
						 _localctx.eff->symbols.Append(((EffectContext)_localctx).state.sym); 
						}
						break;
					case 9:
						{
						setState(162);
						((EffectContext)_localctx).sampler = sampler();
						setState(163);
						match(SC);
						 _localctx.eff->symbols.Append(((EffectContext)_localctx).sampler.sym); 
						}
						break;
					case 10:
						{
						setState(166);
						((EffectContext)_localctx).program = program();
						setState(167);
						match(SC);
						 _localctx.eff->symbols.Append(((EffectContext)_localctx).program.sym); 
						}
						break;
					}
					} 
				}
				setState(174);
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
			setState(175);
			match(T__2);
			 origLine = _input->LT(-1)->getLine(); 
			setState(177);
			((LinePreprocessorEntryContext)_localctx).line = match(INTEGERLITERAL);
			setState(178);
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
			setState(181);
			match(T__3);
			setState(182);
			((AliasContext)_localctx).name = match(IDENTIFIER);
			setState(183);
			match(T__4);
			setState(184);
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
			setState(188);
			match(SOBAKA);
			{
			setState(189);
			((AnnotationContext)_localctx).name = match(IDENTIFIER);
			setState(190);
			match(LP);
			setState(191);
			((AnnotationContext)_localctx).value = expression(0);
			setState(192);
			match(RP);
			 ((AnnotationContext)_localctx).annot =  Alloc<Annotation>(); _localctx.annot->name = (((AnnotationContext)_localctx).name!=null?((AnnotationContext)_localctx).name.getText():null); _localctx.annot->value = ((AnnotationContext)_localctx).value.tree; 
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
			setState(204);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,9,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(195);
				((AttributeContext)_localctx).name = match(IDENTIFIER);
				 ((AttributeContext)_localctx).attr =  Alloc<Attribute>(); _localctx.attr->location = SetupFile(); _localctx.attr->name = (((AttributeContext)_localctx).name!=null?((AttributeContext)_localctx).name.getText():null); 
				setState(197);
				match(LP);
				setState(198);
				((AttributeContext)_localctx).e = ((AttributeContext)_localctx).expression = expression(0);
				setState(199);
				match(RP);
				 _localctx.attr->expression = ((AttributeContext)_localctx).expression.tree; 
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(202);
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
			setState(222);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,12,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					{
					setState(220);
					_errHandler.sync(this);
					switch (_input.LA(1)) {
					case MUL_OP:
						{
						setState(207);
						match(MUL_OP);
						 _localctx.type.type.AddModifier(Type::FullType::Modifier::Pointer); 
						}
						break;
					case LL:
						{
						setState(209);
						match(LL);
						 _localctx.type.type.AddModifier(Type::FullType::Modifier::Array); 
						setState(214);
						_errHandler.sync(this);
						_la = _input.LA(1);
						if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 866135887136686094L) != 0) || ((((_la - 73)) & ~0x3f) == 0 && ((1L << (_la - 73)) & 7787L) != 0)) {
							{
							setState(211);
							((TypeDeclarationContext)_localctx).arraySize0 = expression(0);
							 _localctx.type.type.UpdateValue(((TypeDeclarationContext)_localctx).arraySize0.tree); 
							}
						}

						setState(216);
						match(RR);
						}
						break;
					case IDENTIFIER:
						{
						setState(217);
						((TypeDeclarationContext)_localctx).qual = match(IDENTIFIER);
						 _localctx.type.type.AddQualifier(FixedString((((TypeDeclarationContext)_localctx).qual!=null?((TypeDeclarationContext)_localctx).qual.getText():null))); 
						}
						break;
					case T__2:
						{
						setState(219);
						linePreprocessorEntry();
						}
						break;
					default:
						throw new NoViableAltException(this);
					}
					} 
				}
				setState(224);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,12,_ctx);
			}
			setState(225);
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
		enterRule(_localctx, 20, RULE_variables);

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
			setState(231);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__2) {
				{
				{
				setState(228);
				linePreprocessorEntry();
				}
				}
				setState(233);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(239);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==SOBAKA) {
				{
				{
				setState(234);
				((VariablesContext)_localctx).annotation = annotation();
				 if (annotations.Full()) { throw IndexOutOfBoundsException("Maximum of 32 annotations reached"); } annotations.Append(std::move(((VariablesContext)_localctx).annotation.annot)); 
				}
				}
				setState(241);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(245); 
			_errHandler.sync(this);
			_alt = 1;
			do {
				switch (_alt) {
				case 1:
					{
					{
					setState(242);
					((VariablesContext)_localctx).attribute = attribute();
					 if (attributes.Full()) { throw IndexOutOfBoundsException("Maximum of 32 attributes reached"); } attributes.Append(std::move(((VariablesContext)_localctx).attribute.attr)); 
					}
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				setState(247); 
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,15,_ctx);
			} while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER );
			setState(249);
			((VariablesContext)_localctx).varName = match(IDENTIFIER);
			 names.Append((((VariablesContext)_localctx).varName!=null?((VariablesContext)_localctx).varName.getText():null)); valueExpressions.Append(nullptr); locations.Append(SetupFile()); 
			setState(252);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,16,_ctx) ) {
			case 1:
				{
				setState(251);
				linePreprocessorEntry();
				}
				break;
			}
			setState(260);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__2 || _la==CO) {
				{
				setState(258);
				_errHandler.sync(this);
				switch (_input.LA(1)) {
				case CO:
					{
					setState(254);
					match(CO);
					setState(255);
					((VariablesContext)_localctx).varNameN = match(IDENTIFIER);
					 if (names.Full()) { throw IndexOutOfBoundsException("Maximum of 256 variable declarations reached"); } names.Append((((VariablesContext)_localctx).varNameN!=null?((VariablesContext)_localctx).varNameN.getText():null)); valueExpressions.Append(nullptr); locations.Append(SetupFile()); 
					}
					break;
				case T__2:
					{
					setState(257);
					linePreprocessorEntry();
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				}
				setState(262);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(267);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COL) {
				{
				setState(263);
				match(COL);
				setState(264);
				((VariablesContext)_localctx).typeDeclaration = typeDeclaration();
				 type = ((VariablesContext)_localctx).typeDeclaration.type; 
				}
			}

			setState(282);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==EQ) {
				{
				setState(269);
				match(EQ);
				setState(270);
				((VariablesContext)_localctx).valueExpr = expressionNoComma(0);
				 if (initCounter < names.size) { valueExpressions[initCounter++] = ((VariablesContext)_localctx).valueExpr.tree; }  
				setState(279);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while (_la==T__2 || _la==CO) {
					{
					setState(277);
					_errHandler.sync(this);
					switch (_input.LA(1)) {
					case CO:
						{
						setState(272);
						match(CO);
						setState(273);
						((VariablesContext)_localctx).valueExprN = expressionNoComma(0);
						 if (initCounter < names.size) { valueExpressions[initCounter++] = ((VariablesContext)_localctx).valueExprN.tree; }; 
						}
						break;
					case T__2:
						{
						setState(276);
						linePreprocessorEntry();
						}
						break;
					default:
						throw new NoViableAltException(this);
					}
					}
					setState(281);
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
		enterRule(_localctx, 22, RULE_structureDeclaration);

		        ((StructureDeclarationContext)_localctx).sym =  nullptr;
		        StackArray<Annotation*> annotations(32);
		        StackArray<Attribute*> attributes(32);
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(289);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__2) {
				{
				{
				setState(286);
				linePreprocessorEntry();
				}
				}
				setState(291);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(297);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==SOBAKA) {
				{
				{
				setState(292);
				((StructureDeclarationContext)_localctx).annotation = annotation();
				 if (annotations.Full()) { throw IndexOutOfBoundsException("Maximum of 32 annotations reached"); } annotations.Append(std::move(((StructureDeclarationContext)_localctx).annotation.annot)); 
				}
				}
				setState(299);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(305);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==IDENTIFIER) {
				{
				{
				setState(300);
				((StructureDeclarationContext)_localctx).attribute = attribute();
				 if (attributes.Full()) { throw IndexOutOfBoundsException("Maximum of 32 attributes reached"); } attributes.Append(std::move(((StructureDeclarationContext)_localctx).attribute.attr)); 
				}
				}
				setState(307);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(308);
			match(T__5);
			setState(309);
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
		enterRule(_localctx, 24, RULE_structure);

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
			setState(312);
			((StructureContext)_localctx).structureDeclaration = structureDeclaration();
			 ((StructureContext)_localctx).sym =  ((StructureContext)_localctx).structureDeclaration.sym; 
			setState(314);
			match(LB);
			setState(341);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__2 || _la==IDENTIFIER) {
				{
				setState(339);
				_errHandler.sync(this);
				switch (_input.LA(1)) {
				case IDENTIFIER:
					{
					setState(315);
					((StructureContext)_localctx).varName = match(IDENTIFIER);
					 varName = (((StructureContext)_localctx).varName!=null?((StructureContext)_localctx).varName.getText():null); varLocation = SetupFile(); 
					setState(317);
					match(COL);
					 typeRange = BeginLocationRange(); 
					setState(331);
					_errHandler.sync(this);
					_la = _input.LA(1);
					while (_la==LL || _la==MUL_OP) {
						{
						setState(329);
						_errHandler.sync(this);
						switch (_input.LA(1)) {
						case MUL_OP:
							{
							setState(319);
							match(MUL_OP);
							 varType.AddModifier(Type::FullType::Modifier::Pointer); 
							}
							break;
						case LL:
							{
							setState(321);
							match(LL);
							 varType.AddModifier(Type::FullType::Modifier::Array); 
							setState(326);
							_errHandler.sync(this);
							_la = _input.LA(1);
							if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 866135887136686094L) != 0) || ((((_la - 73)) & ~0x3f) == 0 && ((1L << (_la - 73)) & 7787L) != 0)) {
								{
								setState(323);
								((StructureContext)_localctx).arraySize0 = expression(0);
								 varType.UpdateValue(((StructureContext)_localctx).arraySize0.tree); 
								}
							}

							setState(328);
							match(RR);
							}
							break;
						default:
							throw new NoViableAltException(this);
						}
						}
						setState(333);
						_errHandler.sync(this);
						_la = _input.LA(1);
					}
					setState(334);
					((StructureContext)_localctx).varTypeName = match(IDENTIFIER);
					 if (members.Full()) { throw IndexOutOfBoundsException("Maximum of 1024 struct members reached"); } varType.name = (((StructureContext)_localctx).varTypeName!=null?((StructureContext)_localctx).varTypeName.getText():null); varTypeLocation = EndLocationRange(typeRange); 
					setState(336);
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
					setState(338);
					linePreprocessorEntry();
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				}
				setState(343);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(344);
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
		enterRule(_localctx, 26, RULE_enumeration);

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
			setState(347);
			match(T__6);
			setState(348);
			((EnumerationContext)_localctx).name = match(IDENTIFIER);
			 name = (((EnumerationContext)_localctx).name!=null?((EnumerationContext)_localctx).name.getText():null); location = SetupFile(); 
			setState(354);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COL) {
				{
				setState(350);
				match(COL);
				setState(351);
				((EnumerationContext)_localctx).typeDeclaration = typeDeclaration();
				 type = ((EnumerationContext)_localctx).typeDeclaration.type; 
				}
			}

			setState(356);
			match(LB);
			setState(386);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case IDENTIFIER:
				{
				setState(357);
				((EnumerationContext)_localctx).label = match(IDENTIFIER);
				 Expression* expr = nullptr; labelLocation = SetupFile(); 
				setState(363);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==EQ) {
					{
					setState(359);
					match(EQ);
					setState(360);
					((EnumerationContext)_localctx).value = expression(0);
					 expr = ((EnumerationContext)_localctx).value.tree; 
					}
				}


				                enumLabels.Append(FixedString((((EnumerationContext)_localctx).label!=null?((EnumerationContext)_localctx).label.getText():null)));
				                enumValues.Append(expr);
				                enumLocations.Append(labelLocation);
				            
				setState(367);
				_errHandler.sync(this);
				switch ( getInterpreter().adaptivePredict(_input,33,_ctx) ) {
				case 1:
					{
					setState(366);
					linePreprocessorEntry();
					}
					break;
				}
				setState(382);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while (_la==T__2 || _la==CO) {
					{
					setState(380);
					_errHandler.sync(this);
					switch (_input.LA(1)) {
					case CO:
						{
						setState(369);
						match(CO);
						setState(370);
						((EnumerationContext)_localctx).label = match(IDENTIFIER);
						 if (enumLabels.Full()) { throw IndexOutOfBoundsException("Maximum of 256 enum labels"); } Expression* expr = nullptr; labelLocation = SetupFile(); 
						setState(376);
						_errHandler.sync(this);
						_la = _input.LA(1);
						if (_la==EQ) {
							{
							setState(372);
							match(EQ);
							setState(373);
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
						setState(379);
						linePreprocessorEntry();
						}
						break;
					default:
						throw new NoViableAltException(this);
					}
					}
					setState(384);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				}
				break;
			case T__2:
				{
				setState(385);
				linePreprocessorEntry();
				}
				break;
			case RB:
				break;
			default:
				break;
			}
			setState(388);
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
		enterRule(_localctx, 28, RULE_parameter);

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
			setState(394);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__2) {
				{
				{
				setState(391);
				linePreprocessorEntry();
				}
				}
				setState(396);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(402);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,39,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					{
					{
					setState(397);
					((ParameterContext)_localctx).attribute = attribute();
					 if (attributes.Full()) { throw IndexOutOfBoundsException("Maximum of 32 attributes reached"); } attributes.Append(std::move(((ParameterContext)_localctx).attribute.attr)); 
					}
					} 
				}
				setState(404);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,39,_ctx);
			}
			setState(405);
			((ParameterContext)_localctx).varName = match(IDENTIFIER);
			 name = (((ParameterContext)_localctx).varName!=null?((ParameterContext)_localctx).varName.getText():null); location = SetupFile(); 
			setState(407);
			match(COL);
			setState(408);
			((ParameterContext)_localctx).typeDeclaration = typeDeclaration();
			 type = ((ParameterContext)_localctx).typeDeclaration.type; 
			setState(414);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==EQ) {
				{
				setState(410);
				match(EQ);
				setState(411);
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
		enterRule(_localctx, 30, RULE_functionDeclaration);

		        ((FunctionDeclarationContext)_localctx).sym =  nullptr;
		        StackArray<Variable*> variables(32);
		        StackArray<Attribute*> attributes(32);
		        Symbol::Location location;
		    
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(423);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,41,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					{
					{
					setState(418);
					((FunctionDeclarationContext)_localctx).attribute = attribute();
					 if (attributes.Full()) { throw IndexOutOfBoundsException("Maximum of 32 attributes reached"); } attributes.Append(std::move(((FunctionDeclarationContext)_localctx).attribute.attr)); 
					}
					} 
				}
				setState(425);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,41,_ctx);
			}
			setState(426);
			((FunctionDeclarationContext)_localctx).name = match(IDENTIFIER);
			 location = SetupFile(); 
			setState(428);
			match(LP);
			setState(444);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__2 || _la==IDENTIFIER) {
				{
				setState(429);
				((FunctionDeclarationContext)_localctx).arg0 = parameter();
				 variables.Append(((FunctionDeclarationContext)_localctx).arg0.sym); 
				setState(432);
				_errHandler.sync(this);
				switch ( getInterpreter().adaptivePredict(_input,42,_ctx) ) {
				case 1:
					{
					setState(431);
					linePreprocessorEntry();
					}
					break;
				}
				setState(441);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while (_la==T__2 || _la==CO) {
					{
					setState(439);
					_errHandler.sync(this);
					switch (_input.LA(1)) {
					case CO:
						{
						setState(434);
						match(CO);
						setState(435);
						((FunctionDeclarationContext)_localctx).argn = parameter();
						 if (variables.Full()) throw IndexOutOfBoundsException("Maximum of 32 variables reached"); variables.Append(((FunctionDeclarationContext)_localctx).argn.sym); 
						}
						break;
					case T__2:
						{
						setState(438);
						linePreprocessorEntry();
						}
						break;
					default:
						throw new NoViableAltException(this);
					}
					}
					setState(443);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				}
			}

			setState(446);
			match(RP);
			setState(447);
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
		enterRule(_localctx, 32, RULE_function);

		        ((FunctionContext)_localctx).sym =  nullptr;
		    
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(450);
			((FunctionContext)_localctx).functionDeclaration = functionDeclaration();
			 ((FunctionContext)_localctx).sym =  ((FunctionContext)_localctx).functionDeclaration.sym; 
			setState(452);
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
		enterRule(_localctx, 34, RULE_program);

		        ((ProgramContext)_localctx).sym =  nullptr;
		        StackArray<Expression*> entries(32);
		        StackArray<Annotation*> annotations(32);
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(460);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==SOBAKA) {
				{
				{
				setState(455);
				((ProgramContext)_localctx).annotation = annotation();
				 if (annotations.Full()) { throw IndexOutOfBoundsException("Maximum of 32 annotations reached"); } annotations.Append(std::move(((ProgramContext)_localctx).annotation.annot)); 
				}
				}
				setState(462);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(463);
			match(T__7);
			setState(464);
			((ProgramContext)_localctx).name = match(IDENTIFIER);
			 _localctx.sym->location = SetupFile(); 
			setState(466);
			match(LB);
			setState(473);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & 866135887136686094L) != 0) || ((((_la - 73)) & ~0x3f) == 0 && ((1L << (_la - 73)) & 7787L) != 0)) {
				{
				{
				setState(467);
				((ProgramContext)_localctx).assignment = expression(0);
				 if (entries.Full()) throw IndexOutOfBoundsException("Maximum of 32 entries reached"); entries.Append(((ProgramContext)_localctx).assignment.tree); 
				setState(469);
				match(SC);
				}
				}
				setState(475);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(476);
			match(RB);
			 
			        ((ProgramContext)_localctx).sym =  Alloc<Program>();
			        _localctx.sym->location = location;
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
		enterRule(_localctx, 36, RULE_sampler);

		        StackArray<Attribute*> attributes(32);
		        StackArray<Annotation*> annotations(32);
		        StackArray<Expression*> entries(32);
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			{
			setState(484);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==SOBAKA) {
				{
				{
				setState(479);
				((SamplerContext)_localctx).annotation = annotation();
				 if (annotations.Full()) throw IndexOutOfBoundsException("Maximum of 32 annotations reached"); annotations.Append(std::move(((SamplerContext)_localctx).annotation.annot)); 
				}
				}
				setState(486);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(492);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==IDENTIFIER) {
				{
				{
				setState(487);
				((SamplerContext)_localctx).attribute = attribute();
				 if (attributes.Full()) throw IndexOutOfBoundsException("Maximum of 32 attributes reached"); attributes.Append(std::move(((SamplerContext)_localctx).attribute.attr)); 
				}
				}
				setState(494);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(495);
			match(T__8);
			 ((SamplerContext)_localctx).sym =  Alloc<SamplerState>(); _localctx.sym->isImmutable = true; 
			}
			setState(498);
			((SamplerContext)_localctx).name = match(IDENTIFIER);
			 _localctx.sym->location = SetupFile(); 
			setState(500);
			match(LB);
			setState(507);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & 866135887136686094L) != 0) || ((((_la - 73)) & ~0x3f) == 0 && ((1L << (_la - 73)) & 7787L) != 0)) {
				{
				{
				setState(501);
				((SamplerContext)_localctx).assign = expression(0);
				 if (entries.Full()) throw IndexOutOfBoundsException("Maximum of 32 entries reached"); entries.Append(((SamplerContext)_localctx).assign.tree); 
				setState(503);
				match(SC);
				}
				}
				setState(509);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(510);
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
		enterRule(_localctx, 38, RULE_state);

		        StackArray<Expression*> entries(32);
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			{
			setState(513);
			match(T__9);
			 ((StateContext)_localctx).sym =  Alloc<RenderState>(); 
			}
			setState(516);
			((StateContext)_localctx).name = match(IDENTIFIER);
			 _localctx.sym->location = SetupFile(); 
			setState(518);
			match(LB);
			setState(525);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & 866135887136686094L) != 0) || ((((_la - 73)) & ~0x3f) == 0 && ((1L << (_la - 73)) & 7787L) != 0)) {
				{
				{
				setState(519);
				((StateContext)_localctx).assign = expression(0);
				 if (entries.Full()) throw IndexOutOfBoundsException("Maximum of 32 entries reached"); entries.Append(((StateContext)_localctx).assign.tree); 
				setState(521);
				match(SC);
				}
				}
				setState(527);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(528);
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
		enterRule(_localctx, 40, RULE_statement);

		        ((StatementContext)_localctx).tree =  nullptr;
		    
		try {
			setState(559);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__10:
				enterOuterAlt(_localctx, 1);
				{
				setState(531);
				((StatementContext)_localctx).ifStatement = ifStatement();
				 ((StatementContext)_localctx).tree =  ((StatementContext)_localctx).ifStatement.tree; 
				}
				break;
			case LB:
				enterOuterAlt(_localctx, 2);
				{
				setState(534);
				((StatementContext)_localctx).scopeStatement = scopeStatement();
				 ((StatementContext)_localctx).tree =  ((StatementContext)_localctx).scopeStatement.tree; 
				}
				break;
			case T__12:
				enterOuterAlt(_localctx, 3);
				{
				setState(537);
				((StatementContext)_localctx).forStatement = forStatement();
				 ((StatementContext)_localctx).tree =  ((StatementContext)_localctx).forStatement.tree; 
				}
				break;
			case T__15:
			case T__16:
				enterOuterAlt(_localctx, 4);
				{
				setState(540);
				((StatementContext)_localctx).whileStatement = whileStatement();
				 ((StatementContext)_localctx).tree =  ((StatementContext)_localctx).whileStatement.tree; 
				}
				break;
			case T__22:
				enterOuterAlt(_localctx, 5);
				{
				setState(543);
				((StatementContext)_localctx).switchStatement = switchStatement();
				 ((StatementContext)_localctx).tree =  ((StatementContext)_localctx).switchStatement.tree; 
				}
				break;
			case T__17:
			case T__18:
			case T__19:
			case T__20:
				enterOuterAlt(_localctx, 6);
				{
				setState(546);
				((StatementContext)_localctx).terminateStatement = terminateStatement();
				 ((StatementContext)_localctx).tree =  ((StatementContext)_localctx).terminateStatement.tree; 
				}
				break;
			case T__21:
				enterOuterAlt(_localctx, 7);
				{
				setState(549);
				((StatementContext)_localctx).continueStatement = continueStatement();
				 ((StatementContext)_localctx).tree =  ((StatementContext)_localctx).continueStatement.tree; 
				}
				break;
			case T__25:
				enterOuterAlt(_localctx, 8);
				{
				setState(552);
				((StatementContext)_localctx).breakStatement = breakStatement();
				 ((StatementContext)_localctx).tree =  ((StatementContext)_localctx).breakStatement.tree; 
				}
				break;
			case T__0:
			case T__1:
			case T__2:
			case T__26:
			case T__27:
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
				setState(555);
				((StatementContext)_localctx).expressionStatement = expressionStatement();
				setState(556);
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
		enterRule(_localctx, 42, RULE_expressionStatement);

		        ((ExpressionStatementContext)_localctx).tree =  nullptr;
		    
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(561);
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
		enterRule(_localctx, 44, RULE_ifStatement);

		        ((IfStatementContext)_localctx).tree =  nullptr;
		        Expression* condition = nullptr;
		        Statement* ifBody = nullptr;
		        Statement* elseBody = nullptr;
		        Symbol::Location location;
		    
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(564);
			match(T__10);
			 location = SetupFile(); 
			setState(566);
			match(LP);
			setState(567);
			((IfStatementContext)_localctx).condition = expression(0);
			 condition = ((IfStatementContext)_localctx).condition.tree; 
			setState(569);
			match(RP);
			setState(570);
			((IfStatementContext)_localctx).ifBody = statement();
			 ifBody = ((IfStatementContext)_localctx).ifBody.tree; 
			setState(576);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,53,_ctx) ) {
			case 1:
				{
				setState(572);
				match(T__11);
				setState(573);
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
		enterRule(_localctx, 46, RULE_forStatement);

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
			setState(580);
			match(T__12);
			 location = SetupFile(); 
			setState(582);
			match(LP);
			setState(586);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__2 || _la==SOBAKA || _la==IDENTIFIER) {
				{
				setState(583);
				((ForStatementContext)_localctx).variables = variables();
				 declarations = ((ForStatementContext)_localctx).variables.vars; 
				}
			}

			setState(588);
			match(SC);
			setState(592);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 866135887136686094L) != 0) || ((((_la - 73)) & ~0x3f) == 0 && ((1L << (_la - 73)) & 7787L) != 0)) {
				{
				setState(589);
				((ForStatementContext)_localctx).condition = expression(0);
				 conditionExpression = ((ForStatementContext)_localctx).condition.tree; 
				}
			}

			setState(594);
			match(SC);
			setState(598);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 866135887136686094L) != 0) || ((((_la - 73)) & ~0x3f) == 0 && ((1L << (_la - 73)) & 7787L) != 0)) {
				{
				setState(595);
				((ForStatementContext)_localctx).loop = expression(0);
				 loopExpression = ((ForStatementContext)_localctx).loop.tree; 
				}
			}

			setState(600);
			match(RP);
			setState(606);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,57,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					{
					{
					setState(601);
					((ForStatementContext)_localctx).attribute = attribute();
					 if (attributes.Full()) { throw IndexOutOfBoundsException("Maximum of 32 attributes reached"); } attributes.Append(std::move(((ForStatementContext)_localctx).attribute.attr)); 
					}
					} 
				}
				setState(608);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,57,_ctx);
			}
			setState(609);
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
		enterRule(_localctx, 48, RULE_forRangeStatement);

		        ((ForRangeStatementContext)_localctx).tree =  nullptr;
		        Statement* contents = nullptr;
		        Symbol::Location location;
		    
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(613);
			match(T__12);
			 location = SetupFile(); 
			setState(615);
			match(LP);
			setState(616);
			((ForRangeStatementContext)_localctx).iterator = match(IDENTIFIER);
			setState(617);
			match(COL);
			setState(618);
			((ForRangeStatementContext)_localctx).start = match(IDENTIFIER);
			setState(619);
			match(T__13);
			setState(620);
			((ForRangeStatementContext)_localctx).end = match(IDENTIFIER);
			setState(621);
			match(RP);
			setState(622);
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
		enterRule(_localctx, 50, RULE_forUniformValueStatement);

		        ((ForUniformValueStatementContext)_localctx).tree =  nullptr;
		        Statement* contents = nullptr;
		        Symbol::Location location;
		    
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(626);
			match(T__14);
			 location = SetupFile(); 
			setState(628);
			match(LP);
			setState(629);
			expression(0);
			setState(630);
			match(RP);
			setState(631);
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
		enterRule(_localctx, 52, RULE_whileStatement);

		        ((WhileStatementContext)_localctx).tree =  nullptr;
		        Expression* conditionExpression = nullptr;
		        Statement* contents = nullptr;
		        bool isDoWhile = false;
		        Symbol::Location location;
		    
		try {
			setState(657);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__15:
				enterOuterAlt(_localctx, 1);
				{
				setState(635);
				match(T__15);
				 location = SetupFile(); 
				setState(637);
				match(LP);
				setState(638);
				((WhileStatementContext)_localctx).condition = expression(0);
				 conditionExpression = ((WhileStatementContext)_localctx).condition.tree; 
				setState(640);
				match(RP);
				setState(641);
				((WhileStatementContext)_localctx).content = statement();
				 contents = ((WhileStatementContext)_localctx).content.tree; 

				        ((WhileStatementContext)_localctx).tree =  Alloc<WhileStatement>(conditionExpression, contents, isDoWhile);
				        _localctx.tree->location = location;
				    
				}
				break;
			case T__16:
				enterOuterAlt(_localctx, 2);
				{
				setState(645);
				match(T__16);
				 location = SetupFile(); 
				setState(647);
				((WhileStatementContext)_localctx).content = statement();
				 contents = ((WhileStatementContext)_localctx).content.tree; isDoWhile = true; 
				setState(649);
				match(T__15);
				setState(650);
				match(LP);
				setState(651);
				((WhileStatementContext)_localctx).condition = expression(0);
				 conditionExpression = ((WhileStatementContext)_localctx).condition.tree; 
				setState(653);
				match(RP);
				setState(654);
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
		enterRule(_localctx, 54, RULE_scopeStatement);

		        ((ScopeStatementContext)_localctx).tree =  nullptr;
		        PinnedArray<Symbol*> contents(0xFFFFFF);
			    std::vector<Expression*> unfinished;
		        Symbol::Location location;
		        Symbol::Location ends;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(659);
			match(LB);
			 location = SetupFile(); 
			setState(671);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & 866144683313539086L) != 0) || ((((_la - 72)) & ~0x3f) == 0 && ((1L << (_la - 72)) & 15575L) != 0)) {
				{
				setState(669);
				_errHandler.sync(this);
				switch ( getInterpreter().adaptivePredict(_input,59,_ctx) ) {
				case 1:
					{
					setState(661);
					((ScopeStatementContext)_localctx).variables = variables();
					setState(662);
					match(SC);
					 for(Variable* var : ((ScopeStatementContext)_localctx).variables.vars) { contents.Append(var); } 
					}
					break;
				case 2:
					{
					setState(665);
					((ScopeStatementContext)_localctx).statement = statement();
					 contents.Append(((ScopeStatementContext)_localctx).statement.tree); 
					}
					break;
				case 3:
					{
					setState(668);
					linePreprocessorEntry();
					}
					break;
				}
				}
				setState(673);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(674);
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
		enterRule(_localctx, 56, RULE_terminateStatement);

		        ((TerminateStatementContext)_localctx).tree =  nullptr;
		        Expression* returnValue = nullptr;
		        Symbol::Location location;
		    
		int _la;
		try {
			setState(699);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__17:
				enterOuterAlt(_localctx, 1);
				{
				setState(678);
				match(T__17);
				 location = SetupFile(); 
				setState(683);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 866135887136686094L) != 0) || ((((_la - 73)) & ~0x3f) == 0 && ((1L << (_la - 73)) & 7787L) != 0)) {
					{
					setState(680);
					((TerminateStatementContext)_localctx).value = expression(0);
					 returnValue = ((TerminateStatementContext)_localctx).value.tree; 
					}
				}

				setState(685);
				match(SC);

				        ((TerminateStatementContext)_localctx).tree =  Alloc<TerminateStatement>(returnValue, TerminateStatement::TerminationType::Return);
				        _localctx.tree->location = location;
				    
				}
				break;
			case T__18:
				enterOuterAlt(_localctx, 2);
				{
				setState(687);
				match(T__18);
				 location = SetupFile(); 
				setState(689);
				match(SC);

				      ((TerminateStatementContext)_localctx).tree =  Alloc<TerminateStatement>(returnValue, TerminateStatement::TerminationType::Discard);
				      _localctx.tree->location = location;
				    
				}
				break;
			case T__19:
				enterOuterAlt(_localctx, 3);
				{
				setState(691);
				match(T__19);
				 location = SetupFile(); 
				setState(693);
				match(SC);

				      ((TerminateStatementContext)_localctx).tree =  Alloc<TerminateStatement>(returnValue, TerminateStatement::TerminationType::RayIgnoreIntersection);
				      _localctx.tree->location = location;
				    
				}
				break;
			case T__20:
				enterOuterAlt(_localctx, 4);
				{
				setState(695);
				match(T__20);
				 location = SetupFile(); 
				setState(697);
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
		enterRule(_localctx, 58, RULE_continueStatement);

		        ((ContinueStatementContext)_localctx).tree =  nullptr;
		        Symbol::Location location;
		    
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(701);
			match(T__21);
			 location = SetupFile(); 
			setState(703);
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
		enterRule(_localctx, 60, RULE_switchStatement);

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
			setState(706);
			match(T__22);
			 location = SetupFile(); 
			setState(708);
			match(LP);
			setState(709);
			((SwitchStatementContext)_localctx).expression = expression(0);
			setState(710);
			match(RP);
			 switchExpression = ((SwitchStatementContext)_localctx).expression.tree; 
			setState(712);
			match(LB);
			setState(724);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__23) {
				{
				{
				setState(713);
				match(T__23);
				setState(714);
				((SwitchStatementContext)_localctx).expression = expression(0);
				setState(715);
				match(COL);
				 if (caseExpressions.Full()) { throw IndexOutOfBoundsException("Maximum of 256 case expressions reached"); } caseExpressions.Append(((SwitchStatementContext)_localctx).expression.tree); caseStatements.Append(nullptr); 
				setState(720);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 866144683313539086L) != 0) || ((((_la - 73)) & ~0x3f) == 0 && ((1L << (_la - 73)) & 7787L) != 0)) {
					{
					setState(717);
					((SwitchStatementContext)_localctx).statement = statement();
					 
					                    caseStatements.back() = ((SwitchStatementContext)_localctx).statement.tree;
					                
					}
				}

				}
				}
				setState(726);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(734);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__24) {
				{
				setState(727);
				match(T__24);
				setState(728);
				match(COL);
				setState(732);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 866144683313539086L) != 0) || ((((_la - 73)) & ~0x3f) == 0 && ((1L << (_la - 73)) & 7787L) != 0)) {
					{
					setState(729);
					((SwitchStatementContext)_localctx).statement = statement();

					                    defaultStatement = ((SwitchStatementContext)_localctx).statement.tree;
					                
					}
				}

				}
			}

			setState(736);
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
		enterRule(_localctx, 62, RULE_breakStatement);

		        ((BreakStatementContext)_localctx).tree =  nullptr;
		        Symbol::Location location;
		    
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(739);
			match(T__25);
			 location = SetupFile(); 
			setState(741);
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
		int _startState = 64;
		enterRecursionRule(_localctx, 64, RULE_expressionNoComma, _p);

		        ((ExpressionNoCommaContext)_localctx).tree =  nullptr;
		        Symbol::Location location;
		        StackArray<Expression*> args(256);
		    
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(752);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__26:
			case T__27:
			case NOT:
			case CONJUGATE:
			case ADD_OP:
			case SUB_OP:
			case MUL_OP:
				{
				setState(745);
				((ExpressionNoCommaContext)_localctx).op = _input.LT(1);
				_la = _input.LA(1);
				if ( !(((((_la - 27)) & ~0x3f) == 0 && ((1L << (_la - 27)) & 774058335535107L) != 0)) ) {
					((ExpressionNoCommaContext)_localctx).op = (Token)_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				setState(746);
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
				setState(749);
				((ExpressionNoCommaContext)_localctx).atom = binaryexpatom();
				 ((ExpressionNoCommaContext)_localctx).tree =  ((ExpressionNoCommaContext)_localctx).atom.tree; 
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
			_ctx.stop = _input.LT(-1);
			setState(892);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,74,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					if ( _parseListeners!=null ) triggerExitRuleEvent();
					_prevctx = _localctx;
					{
					setState(890);
					_errHandler.sync(this);
					switch ( getInterpreter().adaptivePredict(_input,73,_ctx) ) {
					case 1:
						{
						_localctx = new ExpressionNoCommaContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expressionNoComma);
						setState(754);
						if (!(precpred(_ctx, 17))) throw new FailedPredicateException(this, "precpred(_ctx, 17)");
						 ((ExpressionNoCommaContext)_localctx).tree =  ((ExpressionNoCommaContext)_localctx).e1.tree; 
						setState(756);
						match(DOT);
						 location = SetupFile(); 
						setState(758);
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
						setState(761);
						if (!(precpred(_ctx, 16))) throw new FailedPredicateException(this, "precpred(_ctx, 16)");
						 ((ExpressionNoCommaContext)_localctx).tree =  ((ExpressionNoCommaContext)_localctx).e1.tree; 
						setState(763);
						match(ARROW);
						 location = SetupFile(); 
						setState(765);
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
						setState(768);
						if (!(precpred(_ctx, 13))) throw new FailedPredicateException(this, "precpred(_ctx, 13)");
						 ((ExpressionNoCommaContext)_localctx).tree =  ((ExpressionNoCommaContext)_localctx).e1.tree; 
						setState(770);
						((ExpressionNoCommaContext)_localctx).op = _input.LT(1);
						_la = _input.LA(1);
						if ( !(((((_la - 70)) & ~0x3f) == 0 && ((1L << (_la - 70)) & 97L) != 0)) ) {
							((ExpressionNoCommaContext)_localctx).op = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						 location = SetupFile(); 
						setState(772);
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
						setState(775);
						if (!(precpred(_ctx, 12))) throw new FailedPredicateException(this, "precpred(_ctx, 12)");
						 ((ExpressionNoCommaContext)_localctx).tree =  ((ExpressionNoCommaContext)_localctx).e1.tree; 
						setState(777);
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
						setState(779);
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
						setState(782);
						if (!(precpred(_ctx, 11))) throw new FailedPredicateException(this, "precpred(_ctx, 11)");
						 ((ExpressionNoCommaContext)_localctx).tree =  ((ExpressionNoCommaContext)_localctx).e1.tree; 
						setState(784);
						((ExpressionNoCommaContext)_localctx).op = _input.LT(1);
						_la = _input.LA(1);
						if ( !(_la==T__28 || _la==T__29) ) {
							((ExpressionNoCommaContext)_localctx).op = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						 location = SetupFile(); 
						setState(786);
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
						setState(789);
						if (!(precpred(_ctx, 10))) throw new FailedPredicateException(this, "precpred(_ctx, 10)");
						 ((ExpressionNoCommaContext)_localctx).tree =  ((ExpressionNoCommaContext)_localctx).e1.tree; 
						setState(791);
						((ExpressionNoCommaContext)_localctx).op = _input.LT(1);
						_la = _input.LA(1);
						if ( !(((((_la - 62)) & ~0x3f) == 0 && ((1L << (_la - 62)) & 15L) != 0)) ) {
							((ExpressionNoCommaContext)_localctx).op = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						 location = SetupFile(); 
						setState(793);
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
						setState(796);
						if (!(precpred(_ctx, 9))) throw new FailedPredicateException(this, "precpred(_ctx, 9)");
						 ((ExpressionNoCommaContext)_localctx).tree =  ((ExpressionNoCommaContext)_localctx).e1.tree; 
						setState(798);
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
						setState(800);
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
						setState(803);
						if (!(precpred(_ctx, 8))) throw new FailedPredicateException(this, "precpred(_ctx, 8)");
						 ((ExpressionNoCommaContext)_localctx).tree =  ((ExpressionNoCommaContext)_localctx).e1.tree; 
						setState(805);
						match(AND);
						 location = SetupFile(); 
						setState(807);
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
						setState(810);
						if (!(precpred(_ctx, 7))) throw new FailedPredicateException(this, "precpred(_ctx, 7)");
						 ((ExpressionNoCommaContext)_localctx).tree =  ((ExpressionNoCommaContext)_localctx).e1.tree; 
						setState(812);
						match(XOR);
						 location = SetupFile(); 
						setState(814);
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
						setState(817);
						if (!(precpred(_ctx, 6))) throw new FailedPredicateException(this, "precpred(_ctx, 6)");
						 ((ExpressionNoCommaContext)_localctx).tree =  ((ExpressionNoCommaContext)_localctx).e1.tree; 
						setState(819);
						match(OR);
						 location = SetupFile(); 
						setState(821);
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
						setState(824);
						if (!(precpred(_ctx, 5))) throw new FailedPredicateException(this, "precpred(_ctx, 5)");
						 ((ExpressionNoCommaContext)_localctx).tree =  ((ExpressionNoCommaContext)_localctx).e1.tree; 
						setState(826);
						match(LOGICAND);
						 location = SetupFile(); 
						setState(828);
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
						setState(831);
						if (!(precpred(_ctx, 4))) throw new FailedPredicateException(this, "precpred(_ctx, 4)");
						 ((ExpressionNoCommaContext)_localctx).tree =  ((ExpressionNoCommaContext)_localctx).e1.tree; 
						setState(833);
						match(LOGICOR);
						 location = SetupFile(); 
						setState(835);
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
						setState(838);
						if (!(precpred(_ctx, 3))) throw new FailedPredicateException(this, "precpred(_ctx, 3)");
						setState(839);
						match(QU);
						 location = SetupFile(); 
						setState(841);
						((ExpressionNoCommaContext)_localctx).ifBody = expressionNoComma(0);
						setState(842);
						match(COL);
						setState(843);
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
						setState(846);
						if (!(precpred(_ctx, 2))) throw new FailedPredicateException(this, "precpred(_ctx, 2)");
						 ((ExpressionNoCommaContext)_localctx).tree =  ((ExpressionNoCommaContext)_localctx).e1.tree; 
						setState(848);
						((ExpressionNoCommaContext)_localctx).op = _input.LT(1);
						_la = _input.LA(1);
						if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & 189714407033405440L) != 0)) ) {
							((ExpressionNoCommaContext)_localctx).op = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						 location = SetupFile(); 
						setState(850);
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
						setState(853);
						if (!(precpred(_ctx, 19))) throw new FailedPredicateException(this, "precpred(_ctx, 19)");
						 ((ExpressionNoCommaContext)_localctx).tree =  ((ExpressionNoCommaContext)_localctx).e1.tree; 
						setState(855);
						((ExpressionNoCommaContext)_localctx).op = _input.LT(1);
						_la = _input.LA(1);
						if ( !(_la==T__26 || _la==T__27) ) {
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
						setState(858);
						if (!(precpred(_ctx, 18))) throw new FailedPredicateException(this, "precpred(_ctx, 18)");
						 ((ExpressionNoCommaContext)_localctx).tree =  ((ExpressionNoCommaContext)_localctx).e1.tree; 
						setState(860);
						match(LP);
						 location = SetupFile(); 
						setState(877);
						_errHandler.sync(this);
						_la = _input.LA(1);
						if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 866135887136686094L) != 0) || ((((_la - 73)) & ~0x3f) == 0 && ((1L << (_la - 73)) & 7787L) != 0)) {
							{
							setState(862);
							((ExpressionNoCommaContext)_localctx).arg0 = expressionNoComma(0);
							 args.Append(((ExpressionNoCommaContext)_localctx).arg0.tree); 
							setState(865);
							_errHandler.sync(this);
							switch ( getInterpreter().adaptivePredict(_input,68,_ctx) ) {
							case 1:
								{
								setState(864);
								linePreprocessorEntry();
								}
								break;
							}
							setState(874);
							_errHandler.sync(this);
							_la = _input.LA(1);
							while (_la==T__2 || _la==CO) {
								{
								setState(872);
								_errHandler.sync(this);
								switch (_input.LA(1)) {
								case CO:
									{
									setState(867);
									match(CO);
									setState(868);
									((ExpressionNoCommaContext)_localctx).argn = expressionNoComma(0);
									 if (args.Full()) { throw IndexOutOfBoundsException("Maximum of 256 arguments reached"); } args.Append(((ExpressionNoCommaContext)_localctx).argn.tree); 
									}
									break;
								case T__2:
									{
									setState(871);
									linePreprocessorEntry();
									}
									break;
								default:
									throw new NoViableAltException(this);
								}
								}
								setState(876);
								_errHandler.sync(this);
								_la = _input.LA(1);
							}
							}
						}

						setState(879);
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
						setState(881);
						if (!(precpred(_ctx, 15))) throw new FailedPredicateException(this, "precpred(_ctx, 15)");
						 ((ExpressionNoCommaContext)_localctx).tree =  ((ExpressionNoCommaContext)_localctx).e1.tree; 
						setState(883);
						match(LL);
						 location = SetupFile(); 
						setState(886);
						_errHandler.sync(this);
						_la = _input.LA(1);
						if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 866135887136686094L) != 0) || ((((_la - 73)) & ~0x3f) == 0 && ((1L << (_la - 73)) & 7787L) != 0)) {
							{
							setState(885);
							((ExpressionNoCommaContext)_localctx).e3 = expressionNoComma(0);
							}
						}

						setState(888);
						match(RR);

						                  ArrayIndexExpression* expr = Alloc<ArrayIndexExpression>(_localctx.tree, ((ExpressionNoCommaContext)_localctx).e3.tree);
						                  expr->location = ((ExpressionNoCommaContext)_localctx).e1.tree->location;
						                  ((ExpressionNoCommaContext)_localctx).tree =  expr;
						              
						}
						break;
					}
					} 
				}
				setState(894);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,74,_ctx);
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
		int _startState = 66;
		enterRecursionRule(_localctx, 66, RULE_expression, _p);

		        ((ExpressionContext)_localctx).tree =  nullptr;
		        Symbol::Location location;
		        StackArray<Expression*> args(256);
		    
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(903);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__26:
			case T__27:
			case NOT:
			case CONJUGATE:
			case ADD_OP:
			case SUB_OP:
			case MUL_OP:
				{
				setState(896);
				((ExpressionContext)_localctx).op = _input.LT(1);
				_la = _input.LA(1);
				if ( !(((((_la - 27)) & ~0x3f) == 0 && ((1L << (_la - 27)) & 774058335535107L) != 0)) ) {
					((ExpressionContext)_localctx).op = (Token)_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				setState(897);
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
				setState(900);
				((ExpressionContext)_localctx).atom = binaryexpatom();
				 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).atom.tree; 
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
			_ctx.stop = _input.LT(-1);
			setState(1050);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,82,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					if ( _parseListeners!=null ) triggerExitRuleEvent();
					_prevctx = _localctx;
					{
					setState(1048);
					_errHandler.sync(this);
					switch ( getInterpreter().adaptivePredict(_input,81,_ctx) ) {
					case 1:
						{
						_localctx = new ExpressionContext(_parentctx, _parentState);
						_localctx.e1 = _prevctx;
						pushNewRecursionContext(_localctx, _startState, RULE_expression);
						setState(905);
						if (!(precpred(_ctx, 18))) throw new FailedPredicateException(this, "precpred(_ctx, 18)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(907);
						match(DOT);
						 location = SetupFile(); 
						setState(909);
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
						setState(912);
						if (!(precpred(_ctx, 17))) throw new FailedPredicateException(this, "precpred(_ctx, 17)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(914);
						match(ARROW);
						 location = SetupFile(); 
						setState(916);
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
						setState(919);
						if (!(precpred(_ctx, 14))) throw new FailedPredicateException(this, "precpred(_ctx, 14)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(921);
						((ExpressionContext)_localctx).op = _input.LT(1);
						_la = _input.LA(1);
						if ( !(((((_la - 70)) & ~0x3f) == 0 && ((1L << (_la - 70)) & 97L) != 0)) ) {
							((ExpressionContext)_localctx).op = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						 location = SetupFile(); 
						setState(923);
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
						setState(926);
						if (!(precpred(_ctx, 13))) throw new FailedPredicateException(this, "precpred(_ctx, 13)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(928);
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
						setState(930);
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
						setState(933);
						if (!(precpred(_ctx, 12))) throw new FailedPredicateException(this, "precpred(_ctx, 12)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(935);
						((ExpressionContext)_localctx).op = _input.LT(1);
						_la = _input.LA(1);
						if ( !(_la==T__28 || _la==T__29) ) {
							((ExpressionContext)_localctx).op = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						 location = SetupFile(); 
						setState(937);
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
						setState(940);
						if (!(precpred(_ctx, 11))) throw new FailedPredicateException(this, "precpred(_ctx, 11)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(942);
						((ExpressionContext)_localctx).op = _input.LT(1);
						_la = _input.LA(1);
						if ( !(((((_la - 62)) & ~0x3f) == 0 && ((1L << (_la - 62)) & 15L) != 0)) ) {
							((ExpressionContext)_localctx).op = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						 location = SetupFile(); 
						setState(944);
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
						setState(947);
						if (!(precpred(_ctx, 10))) throw new FailedPredicateException(this, "precpred(_ctx, 10)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(949);
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
						setState(951);
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
						setState(954);
						if (!(precpred(_ctx, 9))) throw new FailedPredicateException(this, "precpred(_ctx, 9)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(956);
						match(AND);
						 location = SetupFile(); 
						setState(958);
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
						setState(961);
						if (!(precpred(_ctx, 8))) throw new FailedPredicateException(this, "precpred(_ctx, 8)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(963);
						match(XOR);
						 location = SetupFile(); 
						setState(965);
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
						setState(968);
						if (!(precpred(_ctx, 7))) throw new FailedPredicateException(this, "precpred(_ctx, 7)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(970);
						match(OR);
						 location = SetupFile(); 
						setState(972);
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
						setState(975);
						if (!(precpred(_ctx, 6))) throw new FailedPredicateException(this, "precpred(_ctx, 6)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(977);
						match(LOGICAND);
						 location = SetupFile(); 
						setState(979);
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
						setState(982);
						if (!(precpred(_ctx, 5))) throw new FailedPredicateException(this, "precpred(_ctx, 5)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(984);
						match(LOGICOR);
						 location = SetupFile(); 
						setState(986);
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
						setState(989);
						if (!(precpred(_ctx, 4))) throw new FailedPredicateException(this, "precpred(_ctx, 4)");
						setState(990);
						match(QU);
						 location = SetupFile(); 
						setState(992);
						((ExpressionContext)_localctx).ifBody = expression(0);
						setState(993);
						match(COL);
						setState(994);
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
						setState(997);
						if (!(precpred(_ctx, 3))) throw new FailedPredicateException(this, "precpred(_ctx, 3)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(999);
						((ExpressionContext)_localctx).op = _input.LT(1);
						_la = _input.LA(1);
						if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & 189714407033405440L) != 0)) ) {
							((ExpressionContext)_localctx).op = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						 location = SetupFile(); 
						setState(1001);
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
						setState(1004);
						if (!(precpred(_ctx, 2))) throw new FailedPredicateException(this, "precpred(_ctx, 2)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(1006);
						match(CO);
						 location = SetupFile(); 
						setState(1008);
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
						setState(1011);
						if (!(precpred(_ctx, 20))) throw new FailedPredicateException(this, "precpred(_ctx, 20)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(1013);
						((ExpressionContext)_localctx).op = _input.LT(1);
						_la = _input.LA(1);
						if ( !(_la==T__26 || _la==T__27) ) {
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
						setState(1016);
						if (!(precpred(_ctx, 19))) throw new FailedPredicateException(this, "precpred(_ctx, 19)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(1018);
						match(LP);
						 location = SetupFile(); 
						setState(1035);
						_errHandler.sync(this);
						_la = _input.LA(1);
						if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 866135887136686094L) != 0) || ((((_la - 73)) & ~0x3f) == 0 && ((1L << (_la - 73)) & 7787L) != 0)) {
							{
							setState(1020);
							((ExpressionContext)_localctx).arg0 = expressionNoComma(0);
							 args.Append(((ExpressionContext)_localctx).arg0.tree); 
							setState(1023);
							_errHandler.sync(this);
							switch ( getInterpreter().adaptivePredict(_input,76,_ctx) ) {
							case 1:
								{
								setState(1022);
								linePreprocessorEntry();
								}
								break;
							}
							setState(1032);
							_errHandler.sync(this);
							_la = _input.LA(1);
							while (_la==T__2 || _la==CO) {
								{
								setState(1030);
								_errHandler.sync(this);
								switch (_input.LA(1)) {
								case CO:
									{
									setState(1025);
									match(CO);
									setState(1026);
									((ExpressionContext)_localctx).argn = expressionNoComma(0);
									 if (args.Full()) { throw IndexOutOfBoundsException("Maximum of 256 arguments reached"); } args.Append(((ExpressionContext)_localctx).argn.tree); 
									}
									break;
								case T__2:
									{
									setState(1029);
									linePreprocessorEntry();
									}
									break;
								default:
									throw new NoViableAltException(this);
								}
								}
								setState(1034);
								_errHandler.sync(this);
								_la = _input.LA(1);
							}
							}
						}

						setState(1037);
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
						setState(1039);
						if (!(precpred(_ctx, 16))) throw new FailedPredicateException(this, "precpred(_ctx, 16)");
						 ((ExpressionContext)_localctx).tree =  ((ExpressionContext)_localctx).e1.tree; 
						setState(1041);
						match(LL);
						 location = SetupFile(); 
						setState(1044);
						_errHandler.sync(this);
						_la = _input.LA(1);
						if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 866135887136686094L) != 0) || ((((_la - 73)) & ~0x3f) == 0 && ((1L << (_la - 73)) & 7787L) != 0)) {
							{
							setState(1043);
							((ExpressionContext)_localctx).e3 = expression(0);
							}
						}

						setState(1046);
						match(RR);

						                  ArrayIndexExpression* expr = Alloc<ArrayIndexExpression>(_localctx.tree, ((ExpressionContext)_localctx).e3.tree);
						                  expr->location = ((ExpressionContext)_localctx).e1.tree->location;
						                  ((ExpressionContext)_localctx).tree =  expr;
						              
						}
						break;
					}
					} 
				}
				setState(1052);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,82,_ctx);
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
		enterRule(_localctx, 68, RULE_binaryexpatom);

		        ((BinaryexpatomContext)_localctx).tree =  nullptr;
		        Symbol::Location begin = BeginLocationRange();
		    
		try {
			setState(1083);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,83,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(1053);
				((BinaryexpatomContext)_localctx).initializerExpression = initializerExpression();
				 ((BinaryexpatomContext)_localctx).tree =  ((BinaryexpatomContext)_localctx).initializerExpression.tree; 
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(1056);
				((BinaryexpatomContext)_localctx).arrayInitializerExpression = arrayInitializerExpression();
				 ((BinaryexpatomContext)_localctx).tree =  ((BinaryexpatomContext)_localctx).arrayInitializerExpression.tree; 
				}
				break;
			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(1059);
				match(LP);
				setState(1060);
				((BinaryexpatomContext)_localctx).expression = expression(0);
				setState(1061);
				match(RP);
				 ((BinaryexpatomContext)_localctx).tree =  ((BinaryexpatomContext)_localctx).expression.tree; 
				}
				break;
			case 4:
				enterOuterAlt(_localctx, 4);
				{
				setState(1064);
				((BinaryexpatomContext)_localctx).INTEGERLITERAL = match(INTEGERLITERAL);
				 ((BinaryexpatomContext)_localctx).tree =  Alloc<IntExpression>(atoi((((BinaryexpatomContext)_localctx).INTEGERLITERAL!=null?((BinaryexpatomContext)_localctx).INTEGERLITERAL.getText():null).c_str())); _localctx.tree->location = SetupFile(); 
				}
				break;
			case 5:
				enterOuterAlt(_localctx, 5);
				{
				setState(1066);
				((BinaryexpatomContext)_localctx).UINTEGERLITERAL = match(UINTEGERLITERAL);
				 ((BinaryexpatomContext)_localctx).tree =  Alloc<UIntExpression>(strtoul((((BinaryexpatomContext)_localctx).UINTEGERLITERAL!=null?((BinaryexpatomContext)_localctx).UINTEGERLITERAL.getText():null).c_str(), nullptr, 10)); _localctx.tree->location = SetupFile(); 
				}
				break;
			case 6:
				enterOuterAlt(_localctx, 6);
				{
				setState(1068);
				((BinaryexpatomContext)_localctx).FLOATLITERAL = match(FLOATLITERAL);
				 ((BinaryexpatomContext)_localctx).tree =  Alloc<FloatExpression>(atof((((BinaryexpatomContext)_localctx).FLOATLITERAL!=null?((BinaryexpatomContext)_localctx).FLOATLITERAL.getText():null).c_str())); _localctx.tree->location = SetupFile(); 
				}
				break;
			case 7:
				enterOuterAlt(_localctx, 7);
				{
				setState(1070);
				((BinaryexpatomContext)_localctx).DOUBLELITERAL = match(DOUBLELITERAL);
				 ((BinaryexpatomContext)_localctx).tree =  Alloc<FloatExpression>(atof((((BinaryexpatomContext)_localctx).DOUBLELITERAL!=null?((BinaryexpatomContext)_localctx).DOUBLELITERAL.getText():null).c_str())); _localctx.tree->location = SetupFile(); 
				}
				break;
			case 8:
				enterOuterAlt(_localctx, 8);
				{
				setState(1072);
				((BinaryexpatomContext)_localctx).HEX = match(HEX);
				 ((BinaryexpatomContext)_localctx).tree =  Alloc<UIntExpression>(strtoul((((BinaryexpatomContext)_localctx).HEX!=null?((BinaryexpatomContext)_localctx).HEX.getText():null).c_str(), nullptr, 16)); _localctx.tree->location = SetupFile(); 
				}
				break;
			case 9:
				enterOuterAlt(_localctx, 9);
				{
				setState(1074);
				((BinaryexpatomContext)_localctx).string = string();
				 ((BinaryexpatomContext)_localctx).tree =  Alloc<StringExpression>(((BinaryexpatomContext)_localctx).string.val); _localctx.tree->location = EndLocationRange(begin); 
				}
				break;
			case 10:
				enterOuterAlt(_localctx, 10);
				{
				setState(1077);
				((BinaryexpatomContext)_localctx).IDENTIFIER = match(IDENTIFIER);
				 ((BinaryexpatomContext)_localctx).tree =  Alloc<SymbolExpression>((((BinaryexpatomContext)_localctx).IDENTIFIER!=null?((BinaryexpatomContext)_localctx).IDENTIFIER.getText():null)); _localctx.tree->location = SetupFile(); 
				}
				break;
			case 11:
				enterOuterAlt(_localctx, 11);
				{
				setState(1079);
				((BinaryexpatomContext)_localctx).boolean_ = boolean_();
				 ((BinaryexpatomContext)_localctx).tree =  Alloc<BoolExpression>(((BinaryexpatomContext)_localctx).boolean.val); _localctx.tree->location = SetupFile(); 
				}
				break;
			case 12:
				enterOuterAlt(_localctx, 12);
				{
				setState(1082);
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
		enterRule(_localctx, 70, RULE_initializerExpression);

		        ((InitializerExpressionContext)_localctx).tree =  nullptr;
		        StackArray<Expression*> exprs(4096);
		        std::string type = "";
		        Symbol::Location location;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(1085);
			((InitializerExpressionContext)_localctx).type = match(IDENTIFIER);
			 type = (((InitializerExpressionContext)_localctx).type!=null?((InitializerExpressionContext)_localctx).type.getText():null); 
			setState(1087);
			match(LB);
			 location = SetupFile(); 
			setState(1104);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 866135887136686094L) != 0) || ((((_la - 73)) & ~0x3f) == 0 && ((1L << (_la - 73)) & 7787L) != 0)) {
				{
				setState(1089);
				((InitializerExpressionContext)_localctx).arg0 = expressionNoComma(0);
				 if (((InitializerExpressionContext)_localctx).arg0.tree != nullptr) exprs.Append(((InitializerExpressionContext)_localctx).arg0.tree); 
				setState(1092);
				_errHandler.sync(this);
				switch ( getInterpreter().adaptivePredict(_input,84,_ctx) ) {
				case 1:
					{
					setState(1091);
					linePreprocessorEntry();
					}
					break;
				}
				setState(1101);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while (_la==T__2 || _la==CO) {
					{
					setState(1099);
					_errHandler.sync(this);
					switch (_input.LA(1)) {
					case CO:
						{
						setState(1094);
						match(CO);
						setState(1095);
						((InitializerExpressionContext)_localctx).argN = expressionNoComma(0);
						 if (exprs.Full()) { throw IndexOutOfBoundsException("Maximum of 4096 expressions reached"); } exprs.Append(((InitializerExpressionContext)_localctx).argN.tree); 
						}
						break;
					case T__2:
						{
						setState(1098);
						linePreprocessorEntry();
						}
						break;
					default:
						throw new NoViableAltException(this);
					}
					}
					setState(1103);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				}
			}

			setState(1106);
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
		enterRule(_localctx, 72, RULE_arrayInitializerExpression);

		        ((ArrayInitializerExpressionContext)_localctx).tree =  nullptr;
		        StackArray<Expression*> exprs(4096);
		        Symbol::Location location;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(1109);
			match(LL);
			 location = SetupFile(); 
			setState(1126);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 866135887136686094L) != 0) || ((((_la - 73)) & ~0x3f) == 0 && ((1L << (_la - 73)) & 7787L) != 0)) {
				{
				setState(1111);
				((ArrayInitializerExpressionContext)_localctx).arg0 = expressionNoComma(0);
				 if (((ArrayInitializerExpressionContext)_localctx).arg0.tree != nullptr) exprs.Append(((ArrayInitializerExpressionContext)_localctx).arg0.tree); 
				setState(1114);
				_errHandler.sync(this);
				switch ( getInterpreter().adaptivePredict(_input,88,_ctx) ) {
				case 1:
					{
					setState(1113);
					linePreprocessorEntry();
					}
					break;
				}
				setState(1123);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while (_la==T__2 || _la==CO) {
					{
					setState(1121);
					_errHandler.sync(this);
					switch (_input.LA(1)) {
					case CO:
						{
						setState(1116);
						match(CO);
						setState(1117);
						((ArrayInitializerExpressionContext)_localctx).argN = expressionNoComma(0);
						 if (exprs.Full()) { throw IndexOutOfBoundsException("Maximum of 4096 expressions reached"); } exprs.Append(((ArrayInitializerExpressionContext)_localctx).argN.tree); 
						}
						break;
					case T__2:
						{
						setState(1120);
						linePreprocessorEntry();
						}
						break;
					default:
						throw new NoViableAltException(this);
					}
					}
					setState(1125);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				}
			}

			setState(1128);
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
		enterRule(_localctx, 74, RULE_floatVecLiteralExpression);

		        ((FloatVecLiteralExpressionContext)_localctx).tree =  nullptr;
		        std::vector<float> values;
		        Symbol::Location location;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(1131);
			match(LESS);
			 location = SetupFile(); 
			{
			setState(1133);
			((FloatVecLiteralExpressionContext)_localctx).arg0 = match(FLOATLITERAL);
			 values.push_back(atof((((FloatVecLiteralExpressionContext)_localctx).arg0!=null?((FloatVecLiteralExpressionContext)_localctx).arg0.getText():null).c_str())); 
			}
			setState(1137);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__2) {
				{
				setState(1136);
				linePreprocessorEntry();
				}
			}

			setState(1142); 
			_errHandler.sync(this);
			_la = _input.LA(1);
			do {
				{
				{
				setState(1139);
				match(CO);
				setState(1140);
				((FloatVecLiteralExpressionContext)_localctx).argN = match(FLOATLITERAL);
				 values.push_back(atof((((FloatVecLiteralExpressionContext)_localctx).argN!=null?((FloatVecLiteralExpressionContext)_localctx).argN.getText():null).c_str())); 
				}
				}
				setState(1144); 
				_errHandler.sync(this);
				_la = _input.LA(1);
			} while ( _la==CO );
			setState(1146);
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
		enterRule(_localctx, 76, RULE_doubleVecLiteralExpression);

		        ((DoubleVecLiteralExpressionContext)_localctx).tree =  nullptr;
		        std::vector<float> values;
		        Symbol::Location location;
		    
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(1149);
			match(LESS);
			 location = SetupFile(); 
			{
			setState(1151);
			((DoubleVecLiteralExpressionContext)_localctx).arg0 = match(DOUBLELITERAL);
			 values.push_back(atof((((DoubleVecLiteralExpressionContext)_localctx).arg0!=null?((DoubleVecLiteralExpressionContext)_localctx).arg0.getText():null).c_str())); 
			}
			setState(1155);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__2) {
				{
				setState(1154);
				linePreprocessorEntry();
				}
			}

			setState(1160); 
			_errHandler.sync(this);
			_la = _input.LA(1);
			do {
				{
				{
				setState(1157);
				match(CO);
				setState(1158);
				((DoubleVecLiteralExpressionContext)_localctx).argN = match(DOUBLELITERAL);
				 values.push_back(atof((((DoubleVecLiteralExpressionContext)_localctx).argN!=null?((DoubleVecLiteralExpressionContext)_localctx).argN.getText():null).c_str())); 
				}
				}
				setState(1162); 
				_errHandler.sync(this);
				_la = _input.LA(1);
			} while ( _la==CO );
			setState(1164);
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
		enterRule(_localctx, 78, RULE_intVecLiteralExpression);

		        ((IntVecLiteralExpressionContext)_localctx).tree =  nullptr;
		        std::vector<int> values;
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
			((IntVecLiteralExpressionContext)_localctx).arg0 = match(INTEGERLITERAL);
			 values.push_back(atof((((IntVecLiteralExpressionContext)_localctx).arg0!=null?((IntVecLiteralExpressionContext)_localctx).arg0.getText():null).c_str())); 
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
				((IntVecLiteralExpressionContext)_localctx).argN = match(INTEGERLITERAL);
				 values.push_back(atof((((IntVecLiteralExpressionContext)_localctx).argN!=null?((IntVecLiteralExpressionContext)_localctx).argN.getText():null).c_str())); 
				}
				}
				setState(1180); 
				_errHandler.sync(this);
				_la = _input.LA(1);
			} while ( _la==CO );
			setState(1182);
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
		enterRule(_localctx, 80, RULE_uintVecLiteralExpression);

		        ((UintVecLiteralExpressionContext)_localctx).tree =  nullptr;
		        std::vector<unsigned int> values;
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
			((UintVecLiteralExpressionContext)_localctx).arg0 = match(UINTEGERLITERAL);
			 values.push_back(atof((((UintVecLiteralExpressionContext)_localctx).arg0!=null?((UintVecLiteralExpressionContext)_localctx).arg0.getText():null).c_str())); 
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
				((UintVecLiteralExpressionContext)_localctx).argN = match(UINTEGERLITERAL);
				 values.push_back(atof((((UintVecLiteralExpressionContext)_localctx).argN!=null?((UintVecLiteralExpressionContext)_localctx).argN.getText():null).c_str())); 
				}
				}
				setState(1198); 
				_errHandler.sync(this);
				_la = _input.LA(1);
			} while ( _la==CO );
			setState(1200);
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
		enterRule(_localctx, 82, RULE_booleanVecLiteralExpression);

		        ((BooleanVecLiteralExpressionContext)_localctx).tree =  nullptr;
		        std::vector<bool> values;
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
			((BooleanVecLiteralExpressionContext)_localctx).arg0 = boolean_();
			 values.push_back(atof((((BooleanVecLiteralExpressionContext)_localctx).arg0!=null?_input.getText(((BooleanVecLiteralExpressionContext)_localctx).arg0.start,((BooleanVecLiteralExpressionContext)_localctx).arg0.stop):null).c_str())); 
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

			setState(1215); 
			_errHandler.sync(this);
			_la = _input.LA(1);
			do {
				{
				{
				setState(1211);
				match(CO);
				setState(1212);
				((BooleanVecLiteralExpressionContext)_localctx).argN = boolean_();
				 values.push_back(atof((((BooleanVecLiteralExpressionContext)_localctx).argN!=null?_input.getText(((BooleanVecLiteralExpressionContext)_localctx).argN.start,((BooleanVecLiteralExpressionContext)_localctx).argN.stop):null).c_str())); 
				}
				}
				setState(1217); 
				_errHandler.sync(this);
				_la = _input.LA(1);
			} while ( _la==CO );
			setState(1219);
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
		case 32:
			return expressionNoComma_sempred((ExpressionNoCommaContext)_localctx, predIndex);
		case 33:
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
		"\u0004\u0001V\u04c7\u0002\u0000\u0007\u0000\u0002\u0001\u0007\u0001\u0002"+
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
		"(\u0007(\u0002)\u0007)\u0001\u0000\u0001\u0000\u0001\u0000\u0005\u0000"+
		"X\b\u0000\n\u0000\f\u0000[\t\u0000\u0001\u0000\u0001\u0000\u0001\u0000"+
		"\u0001\u0000\u0005\u0000a\b\u0000\n\u0000\f\u0000d\t\u0000\u0001\u0000"+
		"\u0003\u0000g\b\u0000\u0001\u0001\u0001\u0001\u0001\u0001\u0005\u0001"+
		"l\b\u0001\n\u0001\f\u0001o\t\u0001\u0001\u0001\u0001\u0001\u0001\u0001"+
		"\u0001\u0001\u0005\u0001u\b\u0001\n\u0001\f\u0001x\t\u0001\u0001\u0001"+
		"\u0003\u0001{\b\u0001\u0001\u0002\u0001\u0002\u0001\u0002\u0001\u0002"+
		"\u0003\u0002\u0081\b\u0002\u0001\u0003\u0001\u0003\u0001\u0003\u0001\u0003"+
		"\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004"+
		"\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004"+
		"\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004"+
		"\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004"+
		"\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004"+
		"\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004"+
		"\u0005\u0004\u00ab\b\u0004\n\u0004\f\u0004\u00ae\t\u0004\u0001\u0005\u0001"+
		"\u0005\u0001\u0005\u0001\u0005\u0001\u0005\u0001\u0005\u0001\u0006\u0001"+
		"\u0006\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0006\u0001"+
		"\u0007\u0001\u0007\u0001\u0007\u0001\u0007\u0001\u0007\u0001\u0007\u0001"+
		"\u0007\u0001\b\u0001\b\u0001\b\u0001\b\u0001\b\u0001\b\u0001\b\u0001\b"+
		"\u0001\b\u0003\b\u00cd\b\b\u0001\t\u0001\t\u0001\t\u0001\t\u0001\t\u0001"+
		"\t\u0001\t\u0001\t\u0003\t\u00d7\b\t\u0001\t\u0001\t\u0001\t\u0001\t\u0005"+
		"\t\u00dd\b\t\n\t\f\t\u00e0\t\t\u0001\t\u0001\t\u0001\t\u0001\n\u0005\n"+
		"\u00e6\b\n\n\n\f\n\u00e9\t\n\u0001\n\u0001\n\u0001\n\u0005\n\u00ee\b\n"+
		"\n\n\f\n\u00f1\t\n\u0001\n\u0001\n\u0001\n\u0004\n\u00f6\b\n\u000b\n\f"+
		"\n\u00f7\u0001\n\u0001\n\u0001\n\u0003\n\u00fd\b\n\u0001\n\u0001\n\u0001"+
		"\n\u0001\n\u0005\n\u0103\b\n\n\n\f\n\u0106\t\n\u0001\n\u0001\n\u0001\n"+
		"\u0001\n\u0003\n\u010c\b\n\u0001\n\u0001\n\u0001\n\u0001\n\u0001\n\u0001"+
		"\n\u0001\n\u0001\n\u0005\n\u0116\b\n\n\n\f\n\u0119\t\n\u0003\n\u011b\b"+
		"\n\u0001\n\u0001\n\u0001\u000b\u0005\u000b\u0120\b\u000b\n\u000b\f\u000b"+
		"\u0123\t\u000b\u0001\u000b\u0001\u000b\u0001\u000b\u0005\u000b\u0128\b"+
		"\u000b\n\u000b\f\u000b\u012b\t\u000b\u0001\u000b\u0001\u000b\u0001\u000b"+
		"\u0005\u000b\u0130\b\u000b\n\u000b\f\u000b\u0133\t\u000b\u0001\u000b\u0001"+
		"\u000b\u0001\u000b\u0001\u000b\u0001\f\u0001\f\u0001\f\u0001\f\u0001\f"+
		"\u0001\f\u0001\f\u0001\f\u0001\f\u0001\f\u0001\f\u0001\f\u0001\f\u0001"+
		"\f\u0003\f\u0147\b\f\u0001\f\u0005\f\u014a\b\f\n\f\f\f\u014d\t\f\u0001"+
		"\f\u0001\f\u0001\f\u0001\f\u0001\f\u0005\f\u0154\b\f\n\f\f\f\u0157\t\f"+
		"\u0001\f\u0001\f\u0001\f\u0001\r\u0001\r\u0001\r\u0001\r\u0001\r\u0001"+
		"\r\u0001\r\u0003\r\u0163\b\r\u0001\r\u0001\r\u0001\r\u0001\r\u0001\r\u0001"+
		"\r\u0001\r\u0003\r\u016c\b\r\u0001\r\u0001\r\u0003\r\u0170\b\r\u0001\r"+
		"\u0001\r\u0001\r\u0001\r\u0001\r\u0001\r\u0001\r\u0003\r\u0179\b\r\u0001"+
		"\r\u0001\r\u0005\r\u017d\b\r\n\r\f\r\u0180\t\r\u0001\r\u0003\r\u0183\b"+
		"\r\u0001\r\u0001\r\u0001\r\u0001\u000e\u0005\u000e\u0189\b\u000e\n\u000e"+
		"\f\u000e\u018c\t\u000e\u0001\u000e\u0001\u000e\u0001\u000e\u0005\u000e"+
		"\u0191\b\u000e\n\u000e\f\u000e\u0194\t\u000e\u0001\u000e\u0001\u000e\u0001"+
		"\u000e\u0001\u000e\u0001\u000e\u0001\u000e\u0001\u000e\u0001\u000e\u0001"+
		"\u000e\u0003\u000e\u019f\b\u000e\u0001\u000e\u0001\u000e\u0001\u000f\u0001"+
		"\u000f\u0001\u000f\u0005\u000f\u01a6\b\u000f\n\u000f\f\u000f\u01a9\t\u000f"+
		"\u0001\u000f\u0001\u000f\u0001\u000f\u0001\u000f\u0001\u000f\u0001\u000f"+
		"\u0003\u000f\u01b1\b\u000f\u0001\u000f\u0001\u000f\u0001\u000f\u0001\u000f"+
		"\u0001\u000f\u0005\u000f\u01b8\b\u000f\n\u000f\f\u000f\u01bb\t\u000f\u0003"+
		"\u000f\u01bd\b\u000f\u0001\u000f\u0001\u000f\u0001\u000f\u0001\u000f\u0001"+
		"\u0010\u0001\u0010\u0001\u0010\u0001\u0010\u0001\u0010\u0001\u0011\u0001"+
		"\u0011\u0001\u0011\u0005\u0011\u01cb\b\u0011\n\u0011\f\u0011\u01ce\t\u0011"+
		"\u0001\u0011\u0001\u0011\u0001\u0011\u0001\u0011\u0001\u0011\u0001\u0011"+
		"\u0001\u0011\u0001\u0011\u0005\u0011\u01d8\b\u0011\n\u0011\f\u0011\u01db"+
		"\t\u0011\u0001\u0011\u0001\u0011\u0001\u0011\u0001\u0012\u0001\u0012\u0001"+
		"\u0012\u0005\u0012\u01e3\b\u0012\n\u0012\f\u0012\u01e6\t\u0012\u0001\u0012"+
		"\u0001\u0012\u0001\u0012\u0005\u0012\u01eb\b\u0012\n\u0012\f\u0012\u01ee"+
		"\t\u0012\u0001\u0012\u0001\u0012\u0001\u0012\u0001\u0012\u0001\u0012\u0001"+
		"\u0012\u0001\u0012\u0001\u0012\u0001\u0012\u0001\u0012\u0005\u0012\u01fa"+
		"\b\u0012\n\u0012\f\u0012\u01fd\t\u0012\u0001\u0012\u0001\u0012\u0001\u0012"+
		"\u0001\u0013\u0001\u0013\u0001\u0013\u0001\u0013\u0001\u0013\u0001\u0013"+
		"\u0001\u0013\u0001\u0013\u0001\u0013\u0001\u0013\u0005\u0013\u020c\b\u0013"+
		"\n\u0013\f\u0013\u020f\t\u0013\u0001\u0013\u0001\u0013\u0001\u0013\u0001"+
		"\u0014\u0001\u0014\u0001\u0014\u0001\u0014\u0001\u0014\u0001\u0014\u0001"+
		"\u0014\u0001\u0014\u0001\u0014\u0001\u0014\u0001\u0014\u0001\u0014\u0001"+
		"\u0014\u0001\u0014\u0001\u0014\u0001\u0014\u0001\u0014\u0001\u0014\u0001"+
		"\u0014\u0001\u0014\u0001\u0014\u0001\u0014\u0001\u0014\u0001\u0014\u0001"+
		"\u0014\u0001\u0014\u0001\u0014\u0001\u0014\u0003\u0014\u0230\b\u0014\u0001"+
		"\u0015\u0001\u0015\u0001\u0015\u0001\u0016\u0001\u0016\u0001\u0016\u0001"+
		"\u0016\u0001\u0016\u0001\u0016\u0001\u0016\u0001\u0016\u0001\u0016\u0001"+
		"\u0016\u0001\u0016\u0001\u0016\u0003\u0016\u0241\b\u0016\u0001\u0016\u0001"+
		"\u0016\u0001\u0017\u0001\u0017\u0001\u0017\u0001\u0017\u0001\u0017\u0001"+
		"\u0017\u0003\u0017\u024b\b\u0017\u0001\u0017\u0001\u0017\u0001\u0017\u0001"+
		"\u0017\u0003\u0017\u0251\b\u0017\u0001\u0017\u0001\u0017\u0001\u0017\u0001"+
		"\u0017\u0003\u0017\u0257\b\u0017\u0001\u0017\u0001\u0017\u0001\u0017\u0001"+
		"\u0017\u0005\u0017\u025d\b\u0017\n\u0017\f\u0017\u0260\t\u0017\u0001\u0017"+
		"\u0001\u0017\u0001\u0017\u0001\u0017\u0001\u0018\u0001\u0018\u0001\u0018"+
		"\u0001\u0018\u0001\u0018\u0001\u0018\u0001\u0018\u0001\u0018\u0001\u0018"+
		"\u0001\u0018\u0001\u0018\u0001\u0018\u0001\u0018\u0001\u0019\u0001\u0019"+
		"\u0001\u0019\u0001\u0019\u0001\u0019\u0001\u0019\u0001\u0019\u0001\u0019"+
		"\u0001\u0019\u0001\u001a\u0001\u001a\u0001\u001a\u0001\u001a\u0001\u001a"+
		"\u0001\u001a\u0001\u001a\u0001\u001a\u0001\u001a\u0001\u001a\u0001\u001a"+
		"\u0001\u001a\u0001\u001a\u0001\u001a\u0001\u001a\u0001\u001a\u0001\u001a"+
		"\u0001\u001a\u0001\u001a\u0001\u001a\u0001\u001a\u0001\u001a\u0003\u001a"+
		"\u0292\b\u001a\u0001\u001b\u0001\u001b\u0001\u001b\u0001\u001b\u0001\u001b"+
		"\u0001\u001b\u0001\u001b\u0001\u001b\u0001\u001b\u0001\u001b\u0005\u001b"+
		"\u029e\b\u001b\n\u001b\f\u001b\u02a1\t\u001b\u0001\u001b\u0001\u001b\u0001"+
		"\u001b\u0001\u001b\u0001\u001c\u0001\u001c\u0001\u001c\u0001\u001c\u0001"+
		"\u001c\u0003\u001c\u02ac\b\u001c\u0001\u001c\u0001\u001c\u0001\u001c\u0001"+
		"\u001c\u0001\u001c\u0001\u001c\u0001\u001c\u0001\u001c\u0001\u001c\u0001"+
		"\u001c\u0001\u001c\u0001\u001c\u0001\u001c\u0001\u001c\u0003\u001c\u02bc"+
		"\b\u001c\u0001\u001d\u0001\u001d\u0001\u001d\u0001\u001d\u0001\u001d\u0001"+
		"\u001e\u0001\u001e\u0001\u001e\u0001\u001e\u0001\u001e\u0001\u001e\u0001"+
		"\u001e\u0001\u001e\u0001\u001e\u0001\u001e\u0001\u001e\u0001\u001e\u0001"+
		"\u001e\u0001\u001e\u0003\u001e\u02d1\b\u001e\u0005\u001e\u02d3\b\u001e"+
		"\n\u001e\f\u001e\u02d6\t\u001e\u0001\u001e\u0001\u001e\u0001\u001e\u0001"+
		"\u001e\u0001\u001e\u0003\u001e\u02dd\b\u001e\u0003\u001e\u02df\b\u001e"+
		"\u0001\u001e\u0001\u001e\u0001\u001e\u0001\u001f\u0001\u001f\u0001\u001f"+
		"\u0001\u001f\u0001\u001f\u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001"+
		" \u0001 \u0003 \u02f1\b \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001"+
		" \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001"+
		" \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001"+
		" \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001"+
		" \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001"+
		" \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001"+
		" \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001"+
		" \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001"+
		" \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001"+
		" \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001"+
		" \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001 \u0001"+
		" \u0001 \u0001 \u0001 \u0001 \u0003 \u0362\b \u0001 \u0001 \u0001 \u0001"+
		" \u0001 \u0005 \u0369\b \n \f \u036c\t \u0003 \u036e\b \u0001 \u0001 "+
		"\u0001 \u0001 \u0001 \u0001 \u0001 \u0003 \u0377\b \u0001 \u0001 \u0005"+
		" \u037b\b \n \f \u037e\t \u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001"+
		"!\u0001!\u0003!\u0388\b!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001"+
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
		"!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001"+
		"!\u0001!\u0003!\u0400\b!\u0001!\u0001!\u0001!\u0001!\u0001!\u0005!\u0407"+
		"\b!\n!\f!\u040a\t!\u0003!\u040c\b!\u0001!\u0001!\u0001!\u0001!\u0001!"+
		"\u0001!\u0001!\u0003!\u0415\b!\u0001!\u0001!\u0005!\u0419\b!\n!\f!\u041c"+
		"\t!\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001"+
		"\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001"+
		"\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001\"\u0001"+
		"\"\u0001\"\u0001\"\u0001\"\u0003\"\u043c\b\"\u0001#\u0001#\u0001#\u0001"+
		"#\u0001#\u0001#\u0001#\u0003#\u0445\b#\u0001#\u0001#\u0001#\u0001#\u0001"+
		"#\u0005#\u044c\b#\n#\f#\u044f\t#\u0003#\u0451\b#\u0001#\u0001#\u0001#"+
		"\u0001$\u0001$\u0001$\u0001$\u0001$\u0003$\u045b\b$\u0001$\u0001$\u0001"+
		"$\u0001$\u0001$\u0005$\u0462\b$\n$\f$\u0465\t$\u0003$\u0467\b$\u0001$"+
		"\u0001$\u0001$\u0001%\u0001%\u0001%\u0001%\u0001%\u0001%\u0003%\u0472"+
		"\b%\u0001%\u0001%\u0001%\u0004%\u0477\b%\u000b%\f%\u0478\u0001%\u0001"+
		"%\u0001%\u0001&\u0001&\u0001&\u0001&\u0001&\u0001&\u0003&\u0484\b&\u0001"+
		"&\u0001&\u0001&\u0004&\u0489\b&\u000b&\f&\u048a\u0001&\u0001&\u0001&\u0001"+
		"\'\u0001\'\u0001\'\u0001\'\u0001\'\u0001\'\u0003\'\u0496\b\'\u0001\'\u0001"+
		"\'\u0001\'\u0004\'\u049b\b\'\u000b\'\f\'\u049c\u0001\'\u0001\'\u0001\'"+
		"\u0001(\u0001(\u0001(\u0001(\u0001(\u0001(\u0003(\u04a8\b(\u0001(\u0001"+
		"(\u0001(\u0004(\u04ad\b(\u000b(\f(\u04ae\u0001(\u0001(\u0001(\u0001)\u0001"+
		")\u0001)\u0001)\u0001)\u0001)\u0003)\u04ba\b)\u0001)\u0001)\u0001)\u0001"+
		")\u0004)\u04c0\b)\u000b)\f)\u04c1\u0001)\u0001)\u0001)\u0001)\u0001\u00ac"+
		"\u0002@B*\u0000\u0002\u0004\u0006\b\n\f\u000e\u0010\u0012\u0014\u0016"+
		"\u0018\u001a\u001c\u001e \"$&(*,.02468:<>@BDFHJLNPR\u0000\u000b\u0001"+
		"\u000022\u0001\u0000;;\u0001\u0000@@\u0005\u0000\u001b\u001c00::IJLL\u0002"+
		"\u0000FFKL\u0001\u0000IJ\u0001\u0000\u001d\u001e\u0001\u0000>A\u0001\u0000"+
		"BC\u0005\u0000\u001f%11557799\u0001\u0000\u001b\u001c\u0540\u0000f\u0001"+
		"\u0000\u0000\u0000\u0002z\u0001\u0000\u0000\u0000\u0004\u0080\u0001\u0000"+
		"\u0000\u0000\u0006\u0082\u0001\u0000\u0000\u0000\b\u00ac\u0001\u0000\u0000"+
		"\u0000\n\u00af\u0001\u0000\u0000\u0000\f\u00b5\u0001\u0000\u0000\u0000"+
		"\u000e\u00bc\u0001\u0000\u0000\u0000\u0010\u00cc\u0001\u0000\u0000\u0000"+
		"\u0012\u00ce\u0001\u0000\u0000\u0000\u0014\u00e7\u0001\u0000\u0000\u0000"+
		"\u0016\u0121\u0001\u0000\u0000\u0000\u0018\u0138\u0001\u0000\u0000\u0000"+
		"\u001a\u015b\u0001\u0000\u0000\u0000\u001c\u018a\u0001\u0000\u0000\u0000"+
		"\u001e\u01a7\u0001\u0000\u0000\u0000 \u01c2\u0001\u0000\u0000\u0000\""+
		"\u01cc\u0001\u0000\u0000\u0000$\u01e4\u0001\u0000\u0000\u0000&\u0201\u0001"+
		"\u0000\u0000\u0000(\u022f\u0001\u0000\u0000\u0000*\u0231\u0001\u0000\u0000"+
		"\u0000,\u0234\u0001\u0000\u0000\u0000.\u0244\u0001\u0000\u0000\u00000"+
		"\u0265\u0001\u0000\u0000\u00002\u0272\u0001\u0000\u0000\u00004\u0291\u0001"+
		"\u0000\u0000\u00006\u0293\u0001\u0000\u0000\u00008\u02bb\u0001\u0000\u0000"+
		"\u0000:\u02bd\u0001\u0000\u0000\u0000<\u02c2\u0001\u0000\u0000\u0000>"+
		"\u02e3\u0001\u0000\u0000\u0000@\u02f0\u0001\u0000\u0000\u0000B\u0387\u0001"+
		"\u0000\u0000\u0000D\u043b\u0001\u0000\u0000\u0000F\u043d\u0001\u0000\u0000"+
		"\u0000H\u0455\u0001\u0000\u0000\u0000J\u046b\u0001\u0000\u0000\u0000L"+
		"\u047d\u0001\u0000\u0000\u0000N\u048f\u0001\u0000\u0000\u0000P\u04a1\u0001"+
		"\u0000\u0000\u0000R\u04b3\u0001\u0000\u0000\u0000TY\u00052\u0000\u0000"+
		"UV\b\u0000\u0000\u0000VX\u0006\u0000\uffff\uffff\u0000WU\u0001\u0000\u0000"+
		"\u0000X[\u0001\u0000\u0000\u0000YW\u0001\u0000\u0000\u0000YZ\u0001\u0000"+
		"\u0000\u0000Z\\\u0001\u0000\u0000\u0000[Y\u0001\u0000\u0000\u0000\\g\u0005"+
		"2\u0000\u0000]b\u0005;\u0000\u0000^_\b\u0001\u0000\u0000_a\u0006\u0000"+
		"\uffff\uffff\u0000`^\u0001\u0000\u0000\u0000ad\u0001\u0000\u0000\u0000"+
		"b`\u0001\u0000\u0000\u0000bc\u0001\u0000\u0000\u0000ce\u0001\u0000\u0000"+
		"\u0000db\u0001\u0000\u0000\u0000eg\u0005;\u0000\u0000fT\u0001\u0000\u0000"+
		"\u0000f]\u0001\u0000\u0000\u0000g\u0001\u0001\u0000\u0000\u0000hm\u0005"+
		"2\u0000\u0000ij\b\u0000\u0000\u0000jl\u0006\u0001\uffff\uffff\u0000ki"+
		"\u0001\u0000\u0000\u0000lo\u0001\u0000\u0000\u0000mk\u0001\u0000\u0000"+
		"\u0000mn\u0001\u0000\u0000\u0000np\u0001\u0000\u0000\u0000om\u0001\u0000"+
		"\u0000\u0000p{\u00052\u0000\u0000qv\u0005>\u0000\u0000rs\b\u0002\u0000"+
		"\u0000su\u0006\u0001\uffff\uffff\u0000tr\u0001\u0000\u0000\u0000ux\u0001"+
		"\u0000\u0000\u0000vt\u0001\u0000\u0000\u0000vw\u0001\u0000\u0000\u0000"+
		"wy\u0001\u0000\u0000\u0000xv\u0001\u0000\u0000\u0000y{\u0005@\u0000\u0000"+
		"zh\u0001\u0000\u0000\u0000zq\u0001\u0000\u0000\u0000{\u0003\u0001\u0000"+
		"\u0000\u0000|}\u0005\u0001\u0000\u0000}\u0081\u0006\u0002\uffff\uffff"+
		"\u0000~\u007f\u0005\u0002\u0000\u0000\u007f\u0081\u0006\u0002\uffff\uffff"+
		"\u0000\u0080|\u0001\u0000\u0000\u0000\u0080~\u0001\u0000\u0000\u0000\u0081"+
		"\u0005\u0001\u0000\u0000\u0000\u0082\u0083\u0003\b\u0004\u0000\u0083\u0084"+
		"\u0006\u0003\uffff\uffff\u0000\u0084\u0085\u0005\u0000\u0000\u0001\u0085"+
		"\u0007\u0001\u0000\u0000\u0000\u0086\u00ab\u0003\n\u0005\u0000\u0087\u0088"+
		"\u0003\f\u0006\u0000\u0088\u0089\u0005&\u0000\u0000\u0089\u008a\u0006"+
		"\u0004\uffff\uffff\u0000\u008a\u00ab\u0001\u0000\u0000\u0000\u008b\u008c"+
		"\u0003\u001e\u000f\u0000\u008c\u008d\u0005&\u0000\u0000\u008d\u008e\u0006"+
		"\u0004\uffff\uffff\u0000\u008e\u00ab\u0001\u0000\u0000\u0000\u008f\u0090"+
		"\u0003 \u0010\u0000\u0090\u0091\u0006\u0004\uffff\uffff\u0000\u0091\u00ab"+
		"\u0001\u0000\u0000\u0000\u0092\u0093\u0003\u0014\n\u0000\u0093\u0094\u0005"+
		"&\u0000\u0000\u0094\u0095\u0006\u0004\uffff\uffff\u0000\u0095\u00ab\u0001"+
		"\u0000\u0000\u0000\u0096\u0097\u0003\u0018\f\u0000\u0097\u0098\u0005&"+
		"\u0000\u0000\u0098\u0099\u0006\u0004\uffff\uffff\u0000\u0099\u00ab\u0001"+
		"\u0000\u0000\u0000\u009a\u009b\u0003\u001a\r\u0000\u009b\u009c\u0005&"+
		"\u0000\u0000\u009c\u009d\u0006\u0004\uffff\uffff\u0000\u009d\u00ab\u0001"+
		"\u0000\u0000\u0000\u009e\u009f\u0003&\u0013\u0000\u009f\u00a0\u0005&\u0000"+
		"\u0000\u00a0\u00a1\u0006\u0004\uffff\uffff\u0000\u00a1\u00ab\u0001\u0000"+
		"\u0000\u0000\u00a2\u00a3\u0003$\u0012\u0000\u00a3\u00a4\u0005&\u0000\u0000"+
		"\u00a4\u00a5\u0006\u0004\uffff\uffff\u0000\u00a5\u00ab\u0001\u0000\u0000"+
		"\u0000\u00a6\u00a7\u0003\"\u0011\u0000\u00a7\u00a8\u0005&\u0000\u0000"+
		"\u00a8\u00a9\u0006\u0004\uffff\uffff\u0000\u00a9\u00ab\u0001\u0000\u0000"+
		"\u0000\u00aa\u0086\u0001\u0000\u0000\u0000\u00aa\u0087\u0001\u0000\u0000"+
		"\u0000\u00aa\u008b\u0001\u0000\u0000\u0000\u00aa\u008f\u0001\u0000\u0000"+
		"\u0000\u00aa\u0092\u0001\u0000\u0000\u0000\u00aa\u0096\u0001\u0000\u0000"+
		"\u0000\u00aa\u009a\u0001\u0000\u0000\u0000\u00aa\u009e\u0001\u0000\u0000"+
		"\u0000\u00aa\u00a2\u0001\u0000\u0000\u0000\u00aa\u00a6\u0001\u0000\u0000"+
		"\u0000\u00ab\u00ae\u0001\u0000\u0000\u0000\u00ac\u00ad\u0001\u0000\u0000"+
		"\u0000\u00ac\u00aa\u0001\u0000\u0000\u0000\u00ad\t\u0001\u0000\u0000\u0000"+
		"\u00ae\u00ac\u0001\u0000\u0000\u0000\u00af\u00b0\u0005\u0003\u0000\u0000"+
		"\u00b0\u00b1\u0006\u0005\uffff\uffff\u0000\u00b1\u00b2\u0005N\u0000\u0000"+
		"\u00b2\u00b3\u0003\u0000\u0000\u0000\u00b3\u00b4\u0006\u0005\uffff\uffff"+
		"\u0000\u00b4\u000b\u0001\u0000\u0000\u0000\u00b5\u00b6\u0005\u0004\u0000"+
		"\u0000\u00b6\u00b7\u0005U\u0000\u0000\u00b7\u00b8\u0005\u0005\u0000\u0000"+
		"\u00b8\u00b9\u0005U\u0000\u0000\u00b9\u00ba\u0006\u0006\uffff\uffff\u0000"+
		"\u00ba\u00bb\u0006\u0006\uffff\uffff\u0000\u00bb\r\u0001\u0000\u0000\u0000"+
		"\u00bc\u00bd\u0005H\u0000\u0000\u00bd\u00be\u0005U\u0000\u0000\u00be\u00bf"+
		"\u0005)\u0000\u0000\u00bf\u00c0\u0003B!\u0000\u00c0\u00c1\u0005*\u0000"+
		"\u0000\u00c1\u00c2\u0006\u0007\uffff\uffff\u0000\u00c2\u000f\u0001\u0000"+
		"\u0000\u0000\u00c3\u00c4\u0005U\u0000\u0000\u00c4\u00c5\u0006\b\uffff"+
		"\uffff\u0000\u00c5\u00c6\u0005)\u0000\u0000\u00c6\u00c7\u0003B!\u0000"+
		"\u00c7\u00c8\u0005*\u0000\u0000\u00c8\u00c9\u0006\b\uffff\uffff\u0000"+
		"\u00c9\u00cd\u0001\u0000\u0000\u0000\u00ca\u00cb\u0005U\u0000\u0000\u00cb"+
		"\u00cd\u0006\b\uffff\uffff\u0000\u00cc\u00c3\u0001\u0000\u0000\u0000\u00cc"+
		"\u00ca\u0001\u0000\u0000\u0000\u00cd\u0011\u0001\u0000\u0000\u0000\u00ce"+
		"\u00de\u0006\t\uffff\uffff\u0000\u00cf\u00d0\u0005L\u0000\u0000\u00d0"+
		"\u00dd\u0006\t\uffff\uffff\u0000\u00d1\u00d2\u0005-\u0000\u0000\u00d2"+
		"\u00d6\u0006\t\uffff\uffff\u0000\u00d3\u00d4\u0003B!\u0000\u00d4\u00d5"+
		"\u0006\t\uffff\uffff\u0000\u00d5\u00d7\u0001\u0000\u0000\u0000\u00d6\u00d3"+
		"\u0001\u0000\u0000\u0000\u00d6\u00d7\u0001\u0000\u0000\u0000\u00d7\u00d8"+
		"\u0001\u0000\u0000\u0000\u00d8\u00dd\u0005.\u0000\u0000\u00d9\u00da\u0005"+
		"U\u0000\u0000\u00da\u00dd\u0006\t\uffff\uffff\u0000\u00db\u00dd\u0003"+
		"\n\u0005\u0000\u00dc\u00cf\u0001\u0000\u0000\u0000\u00dc\u00d1\u0001\u0000"+
		"\u0000\u0000\u00dc\u00d9\u0001\u0000\u0000\u0000\u00dc\u00db\u0001\u0000"+
		"\u0000\u0000\u00dd\u00e0\u0001\u0000\u0000\u0000\u00de\u00dc\u0001\u0000"+
		"\u0000\u0000\u00de\u00df\u0001\u0000\u0000\u0000\u00df\u00e1\u0001\u0000"+
		"\u0000\u0000\u00e0\u00de\u0001\u0000\u0000\u0000\u00e1\u00e2\u0005U\u0000"+
		"\u0000\u00e2\u00e3\u0006\t\uffff\uffff\u0000\u00e3\u0013\u0001\u0000\u0000"+
		"\u0000\u00e4\u00e6\u0003\n\u0005\u0000\u00e5\u00e4\u0001\u0000\u0000\u0000"+
		"\u00e6\u00e9\u0001\u0000\u0000\u0000\u00e7\u00e5\u0001\u0000\u0000\u0000"+
		"\u00e7\u00e8\u0001\u0000\u0000\u0000\u00e8\u00ef\u0001\u0000\u0000\u0000"+
		"\u00e9\u00e7\u0001\u0000\u0000\u0000\u00ea\u00eb\u0003\u000e\u0007\u0000"+
		"\u00eb\u00ec\u0006\n\uffff\uffff\u0000\u00ec\u00ee\u0001\u0000\u0000\u0000"+
		"\u00ed\u00ea\u0001\u0000\u0000\u0000\u00ee\u00f1\u0001\u0000\u0000\u0000"+
		"\u00ef\u00ed\u0001\u0000\u0000\u0000\u00ef\u00f0\u0001\u0000\u0000\u0000"+
		"\u00f0\u00f5\u0001\u0000\u0000\u0000\u00f1\u00ef\u0001\u0000\u0000\u0000"+
		"\u00f2\u00f3\u0003\u0010\b\u0000\u00f3\u00f4\u0006\n\uffff\uffff\u0000"+
		"\u00f4\u00f6\u0001\u0000\u0000\u0000\u00f5\u00f2\u0001\u0000\u0000\u0000"+
		"\u00f6\u00f7\u0001\u0000\u0000\u0000\u00f7\u00f5\u0001\u0000\u0000\u0000"+
		"\u00f7\u00f8\u0001\u0000\u0000\u0000\u00f8\u00f9\u0001\u0000\u0000\u0000"+
		"\u00f9\u00fa\u0005U\u0000\u0000\u00fa\u00fc\u0006\n\uffff\uffff\u0000"+
		"\u00fb\u00fd\u0003\n\u0005\u0000\u00fc\u00fb\u0001\u0000\u0000\u0000\u00fc"+
		"\u00fd\u0001\u0000\u0000\u0000\u00fd\u0104\u0001\u0000\u0000\u0000\u00fe"+
		"\u00ff\u0005\'\u0000\u0000\u00ff\u0100\u0005U\u0000\u0000\u0100\u0103"+
		"\u0006\n\uffff\uffff\u0000\u0101\u0103\u0003\n\u0005\u0000\u0102\u00fe"+
		"\u0001\u0000\u0000\u0000\u0102\u0101\u0001\u0000\u0000\u0000\u0103\u0106"+
		"\u0001\u0000\u0000\u0000\u0104\u0102\u0001\u0000\u0000\u0000\u0104\u0105"+
		"\u0001\u0000\u0000\u0000\u0105\u010b\u0001\u0000\u0000\u0000\u0106\u0104"+
		"\u0001\u0000\u0000\u0000\u0107\u0108\u0005(\u0000\u0000\u0108\u0109\u0003"+
		"\u0012\t\u0000\u0109\u010a\u0006\n\uffff\uffff\u0000\u010a\u010c\u0001"+
		"\u0000\u0000\u0000\u010b\u0107\u0001\u0000\u0000\u0000\u010b\u010c\u0001"+
		"\u0000\u0000\u0000\u010c\u011a\u0001\u0000\u0000\u0000\u010d\u010e\u0005"+
		"1\u0000\u0000\u010e\u010f\u0003@ \u0000\u010f\u0117\u0006\n\uffff\uffff"+
		"\u0000\u0110\u0111\u0005\'\u0000\u0000\u0111\u0112\u0003@ \u0000\u0112"+
		"\u0113\u0006\n\uffff\uffff\u0000\u0113\u0116\u0001\u0000\u0000\u0000\u0114"+
		"\u0116\u0003\n\u0005\u0000\u0115\u0110\u0001\u0000\u0000\u0000\u0115\u0114"+
		"\u0001\u0000\u0000\u0000\u0116\u0119\u0001\u0000\u0000\u0000\u0117\u0115"+
		"\u0001\u0000\u0000\u0000\u0117\u0118\u0001\u0000\u0000\u0000\u0118\u011b"+
		"\u0001\u0000\u0000\u0000\u0119\u0117\u0001\u0000\u0000\u0000\u011a\u010d"+
		"\u0001\u0000\u0000\u0000\u011a\u011b\u0001\u0000\u0000\u0000\u011b\u011c"+
		"\u0001\u0000\u0000\u0000\u011c\u011d\u0006\n\uffff\uffff\u0000\u011d\u0015"+
		"\u0001\u0000\u0000\u0000\u011e\u0120\u0003\n\u0005\u0000\u011f\u011e\u0001"+
		"\u0000\u0000\u0000\u0120\u0123\u0001\u0000\u0000\u0000\u0121\u011f\u0001"+
		"\u0000\u0000\u0000\u0121\u0122\u0001\u0000\u0000\u0000\u0122\u0129\u0001"+
		"\u0000\u0000\u0000\u0123\u0121\u0001\u0000\u0000\u0000\u0124\u0125\u0003"+
		"\u000e\u0007\u0000\u0125\u0126\u0006\u000b\uffff\uffff\u0000\u0126\u0128"+
		"\u0001\u0000\u0000\u0000\u0127\u0124\u0001\u0000\u0000\u0000\u0128\u012b"+
		"\u0001\u0000\u0000\u0000\u0129\u0127\u0001\u0000\u0000\u0000\u0129\u012a"+
		"\u0001\u0000\u0000\u0000\u012a\u0131\u0001\u0000\u0000\u0000\u012b\u0129"+
		"\u0001\u0000\u0000\u0000\u012c\u012d\u0003\u0010\b\u0000\u012d\u012e\u0006"+
		"\u000b\uffff\uffff\u0000\u012e\u0130\u0001\u0000\u0000\u0000\u012f\u012c"+
		"\u0001\u0000\u0000\u0000\u0130\u0133\u0001\u0000\u0000\u0000\u0131\u012f"+
		"\u0001\u0000\u0000\u0000\u0131\u0132\u0001\u0000\u0000\u0000\u0132\u0134"+
		"\u0001\u0000\u0000\u0000\u0133\u0131\u0001\u0000\u0000\u0000\u0134\u0135"+
		"\u0005\u0006\u0000\u0000\u0135\u0136\u0005U\u0000\u0000\u0136\u0137\u0006"+
		"\u000b\uffff\uffff\u0000\u0137\u0017\u0001\u0000\u0000\u0000\u0138\u0139"+
		"\u0003\u0016\u000b\u0000\u0139\u013a\u0006\f\uffff\uffff\u0000\u013a\u0155"+
		"\u0005+\u0000\u0000\u013b\u013c\u0005U\u0000\u0000\u013c\u013d\u0006\f"+
		"\uffff\uffff\u0000\u013d\u013e\u0005(\u0000\u0000\u013e\u014b\u0006\f"+
		"\uffff\uffff\u0000\u013f\u0140\u0005L\u0000\u0000\u0140\u014a\u0006\f"+
		"\uffff\uffff\u0000\u0141\u0142\u0005-\u0000\u0000\u0142\u0146\u0006\f"+
		"\uffff\uffff\u0000\u0143\u0144\u0003B!\u0000\u0144\u0145\u0006\f\uffff"+
		"\uffff\u0000\u0145\u0147\u0001\u0000\u0000\u0000\u0146\u0143\u0001\u0000"+
		"\u0000\u0000\u0146\u0147\u0001\u0000\u0000\u0000\u0147\u0148\u0001\u0000"+
		"\u0000\u0000\u0148\u014a\u0005.\u0000\u0000\u0149\u013f\u0001\u0000\u0000"+
		"\u0000\u0149\u0141\u0001\u0000\u0000\u0000\u014a\u014d\u0001\u0000\u0000"+
		"\u0000\u014b\u0149\u0001\u0000\u0000\u0000\u014b\u014c\u0001\u0000\u0000"+
		"\u0000\u014c\u014e\u0001\u0000\u0000\u0000\u014d\u014b\u0001\u0000\u0000"+
		"\u0000\u014e\u014f\u0005U\u0000\u0000\u014f\u0150\u0006\f\uffff\uffff"+
		"\u0000\u0150\u0151\u0005&\u0000\u0000\u0151\u0154\u0006\f\uffff\uffff"+
		"\u0000\u0152\u0154\u0003\n\u0005\u0000\u0153\u013b\u0001\u0000\u0000\u0000"+
		"\u0153\u0152\u0001\u0000\u0000\u0000\u0154\u0157\u0001\u0000\u0000\u0000"+
		"\u0155\u0153\u0001\u0000\u0000\u0000\u0155\u0156\u0001\u0000\u0000\u0000"+
		"\u0156\u0158\u0001\u0000\u0000\u0000\u0157\u0155\u0001\u0000\u0000\u0000"+
		"\u0158\u0159\u0005,\u0000\u0000\u0159\u015a\u0006\f\uffff\uffff\u0000"+
		"\u015a\u0019\u0001\u0000\u0000\u0000\u015b\u015c\u0005\u0007\u0000\u0000"+
		"\u015c\u015d\u0005U\u0000\u0000\u015d\u0162\u0006\r\uffff\uffff\u0000"+
		"\u015e\u015f\u0005(\u0000\u0000\u015f\u0160\u0003\u0012\t\u0000\u0160"+
		"\u0161\u0006\r\uffff\uffff\u0000\u0161\u0163\u0001\u0000\u0000\u0000\u0162"+
		"\u015e\u0001\u0000\u0000\u0000\u0162\u0163\u0001\u0000\u0000\u0000\u0163"+
		"\u0164\u0001\u0000\u0000\u0000\u0164\u0182\u0005+\u0000\u0000\u0165\u0166"+
		"\u0005U\u0000\u0000\u0166\u016b\u0006\r\uffff\uffff\u0000\u0167\u0168"+
		"\u00051\u0000\u0000\u0168\u0169\u0003B!\u0000\u0169\u016a\u0006\r\uffff"+
		"\uffff\u0000\u016a\u016c\u0001\u0000\u0000\u0000\u016b\u0167\u0001\u0000"+
		"\u0000\u0000\u016b\u016c\u0001\u0000\u0000\u0000\u016c\u016d\u0001\u0000"+
		"\u0000\u0000\u016d\u016f\u0006\r\uffff\uffff\u0000\u016e\u0170\u0003\n"+
		"\u0005\u0000\u016f\u016e\u0001\u0000\u0000\u0000\u016f\u0170\u0001\u0000"+
		"\u0000\u0000\u0170\u017e\u0001\u0000\u0000\u0000\u0171\u0172\u0005\'\u0000"+
		"\u0000\u0172\u0173\u0005U\u0000\u0000\u0173\u0178\u0006\r\uffff\uffff"+
		"\u0000\u0174\u0175\u00051\u0000\u0000\u0175\u0176\u0003B!\u0000\u0176"+
		"\u0177\u0006\r\uffff\uffff\u0000\u0177\u0179\u0001\u0000\u0000\u0000\u0178"+
		"\u0174\u0001\u0000\u0000\u0000\u0178\u0179\u0001\u0000\u0000\u0000\u0179"+
		"\u017a\u0001\u0000\u0000\u0000\u017a\u017d\u0006\r\uffff\uffff\u0000\u017b"+
		"\u017d\u0003\n\u0005\u0000\u017c\u0171\u0001\u0000\u0000\u0000\u017c\u017b"+
		"\u0001\u0000\u0000\u0000\u017d\u0180\u0001\u0000\u0000\u0000\u017e\u017c"+
		"\u0001\u0000\u0000\u0000\u017e\u017f\u0001\u0000\u0000\u0000\u017f\u0183"+
		"\u0001\u0000\u0000\u0000\u0180\u017e\u0001\u0000\u0000\u0000\u0181\u0183"+
		"\u0003\n\u0005\u0000\u0182\u0165\u0001\u0000\u0000\u0000\u0182\u0181\u0001"+
		"\u0000\u0000\u0000\u0182\u0183\u0001\u0000\u0000\u0000\u0183\u0184\u0001"+
		"\u0000\u0000\u0000\u0184\u0185\u0005,\u0000\u0000\u0185\u0186\u0006\r"+
		"\uffff\uffff\u0000\u0186\u001b\u0001\u0000\u0000\u0000\u0187\u0189\u0003"+
		"\n\u0005\u0000\u0188\u0187\u0001\u0000\u0000\u0000\u0189\u018c\u0001\u0000"+
		"\u0000\u0000\u018a\u0188\u0001\u0000\u0000\u0000\u018a\u018b\u0001\u0000"+
		"\u0000\u0000\u018b\u0192\u0001\u0000\u0000\u0000\u018c\u018a\u0001\u0000"+
		"\u0000\u0000\u018d\u018e\u0003\u0010\b\u0000\u018e\u018f\u0006\u000e\uffff"+
		"\uffff\u0000\u018f\u0191\u0001\u0000\u0000\u0000\u0190\u018d\u0001\u0000"+
		"\u0000\u0000\u0191\u0194\u0001\u0000\u0000\u0000\u0192\u0190\u0001\u0000"+
		"\u0000\u0000\u0192\u0193\u0001\u0000\u0000\u0000\u0193\u0195\u0001\u0000"+
		"\u0000\u0000\u0194\u0192\u0001\u0000\u0000\u0000\u0195\u0196\u0005U\u0000"+
		"\u0000\u0196\u0197\u0006\u000e\uffff\uffff\u0000\u0197\u0198\u0005(\u0000"+
		"\u0000\u0198\u0199\u0003\u0012\t\u0000\u0199\u019e\u0006\u000e\uffff\uffff"+
		"\u0000\u019a\u019b\u00051\u0000\u0000\u019b\u019c\u0003@ \u0000\u019c"+
		"\u019d\u0006\u000e\uffff\uffff\u0000\u019d\u019f\u0001\u0000\u0000\u0000"+
		"\u019e\u019a\u0001\u0000\u0000\u0000\u019e\u019f\u0001\u0000\u0000\u0000"+
		"\u019f\u01a0\u0001\u0000\u0000\u0000\u01a0\u01a1\u0006\u000e\uffff\uffff"+
		"\u0000\u01a1\u001d\u0001\u0000\u0000\u0000\u01a2\u01a3\u0003\u0010\b\u0000"+
		"\u01a3\u01a4\u0006\u000f\uffff\uffff\u0000\u01a4\u01a6\u0001\u0000\u0000"+
		"\u0000\u01a5\u01a2\u0001\u0000\u0000\u0000\u01a6\u01a9\u0001\u0000\u0000"+
		"\u0000\u01a7\u01a5\u0001\u0000\u0000\u0000\u01a7\u01a8\u0001\u0000\u0000"+
		"\u0000\u01a8\u01aa\u0001\u0000\u0000\u0000\u01a9\u01a7\u0001\u0000\u0000"+
		"\u0000\u01aa\u01ab\u0005U\u0000\u0000\u01ab\u01ac\u0006\u000f\uffff\uffff"+
		"\u0000\u01ac\u01bc\u0005)\u0000\u0000\u01ad\u01ae\u0003\u001c\u000e\u0000"+
		"\u01ae\u01b0\u0006\u000f\uffff\uffff\u0000\u01af\u01b1\u0003\n\u0005\u0000"+
		"\u01b0\u01af\u0001\u0000\u0000\u0000\u01b0\u01b1\u0001\u0000\u0000\u0000"+
		"\u01b1\u01b9\u0001\u0000\u0000\u0000\u01b2\u01b3\u0005\'\u0000\u0000\u01b3"+
		"\u01b4\u0003\u001c\u000e\u0000\u01b4\u01b5\u0006\u000f\uffff\uffff\u0000"+
		"\u01b5\u01b8\u0001\u0000\u0000\u0000\u01b6\u01b8\u0003\n\u0005\u0000\u01b7"+
		"\u01b2\u0001\u0000\u0000\u0000\u01b7\u01b6\u0001\u0000\u0000\u0000\u01b8"+
		"\u01bb\u0001\u0000\u0000\u0000\u01b9\u01b7\u0001\u0000\u0000\u0000\u01b9"+
		"\u01ba\u0001\u0000\u0000\u0000\u01ba\u01bd\u0001\u0000\u0000\u0000\u01bb"+
		"\u01b9\u0001\u0000\u0000\u0000\u01bc\u01ad\u0001\u0000\u0000\u0000\u01bc"+
		"\u01bd\u0001\u0000\u0000\u0000\u01bd\u01be\u0001\u0000\u0000\u0000\u01be"+
		"\u01bf\u0005*\u0000\u0000\u01bf\u01c0\u0003\u0012\t\u0000\u01c0\u01c1"+
		"\u0006\u000f\uffff\uffff\u0000\u01c1\u001f\u0001\u0000\u0000\u0000\u01c2"+
		"\u01c3\u0003\u001e\u000f\u0000\u01c3\u01c4\u0006\u0010\uffff\uffff\u0000"+
		"\u01c4\u01c5\u00036\u001b\u0000\u01c5\u01c6\u0006\u0010\uffff\uffff\u0000"+
		"\u01c6!\u0001\u0000\u0000\u0000\u01c7\u01c8\u0003\u000e\u0007\u0000\u01c8"+
		"\u01c9\u0006\u0011\uffff\uffff\u0000\u01c9\u01cb\u0001\u0000\u0000\u0000"+
		"\u01ca\u01c7\u0001\u0000\u0000\u0000\u01cb\u01ce\u0001\u0000\u0000\u0000"+
		"\u01cc\u01ca\u0001\u0000\u0000\u0000\u01cc\u01cd\u0001\u0000\u0000\u0000"+
		"\u01cd\u01cf\u0001\u0000\u0000\u0000\u01ce\u01cc\u0001\u0000\u0000\u0000"+
		"\u01cf\u01d0\u0005\b\u0000\u0000\u01d0\u01d1\u0005U\u0000\u0000\u01d1"+
		"\u01d2\u0006\u0011\uffff\uffff\u0000\u01d2\u01d9\u0005+\u0000\u0000\u01d3"+
		"\u01d4\u0003B!\u0000\u01d4\u01d5\u0006\u0011\uffff\uffff\u0000\u01d5\u01d6"+
		"\u0005&\u0000\u0000\u01d6\u01d8\u0001\u0000\u0000\u0000\u01d7\u01d3\u0001"+
		"\u0000\u0000\u0000\u01d8\u01db\u0001\u0000\u0000\u0000\u01d9\u01d7\u0001"+
		"\u0000\u0000\u0000\u01d9\u01da\u0001\u0000\u0000\u0000\u01da\u01dc\u0001"+
		"\u0000\u0000\u0000\u01db\u01d9\u0001\u0000\u0000\u0000\u01dc\u01dd\u0005"+
		",\u0000\u0000\u01dd\u01de\u0006\u0011\uffff\uffff\u0000\u01de#\u0001\u0000"+
		"\u0000\u0000\u01df\u01e0\u0003\u000e\u0007\u0000\u01e0\u01e1\u0006\u0012"+
		"\uffff\uffff\u0000\u01e1\u01e3\u0001\u0000\u0000\u0000\u01e2\u01df\u0001"+
		"\u0000\u0000\u0000\u01e3\u01e6\u0001\u0000\u0000\u0000\u01e4\u01e2\u0001"+
		"\u0000\u0000\u0000\u01e4\u01e5\u0001\u0000\u0000\u0000\u01e5\u01ec\u0001"+
		"\u0000\u0000\u0000\u01e6\u01e4\u0001\u0000\u0000\u0000\u01e7\u01e8\u0003"+
		"\u0010\b\u0000\u01e8\u01e9\u0006\u0012\uffff\uffff\u0000\u01e9\u01eb\u0001"+
		"\u0000\u0000\u0000\u01ea\u01e7\u0001\u0000\u0000\u0000\u01eb\u01ee\u0001"+
		"\u0000\u0000\u0000\u01ec\u01ea\u0001\u0000\u0000\u0000\u01ec\u01ed\u0001"+
		"\u0000\u0000\u0000\u01ed\u01ef\u0001\u0000\u0000\u0000\u01ee\u01ec\u0001"+
		"\u0000\u0000\u0000\u01ef\u01f0\u0005\t\u0000\u0000\u01f0\u01f1\u0006\u0012"+
		"\uffff\uffff\u0000\u01f1\u01f2\u0001\u0000\u0000\u0000\u01f2\u01f3\u0005"+
		"U\u0000\u0000\u01f3\u01f4\u0006\u0012\uffff\uffff\u0000\u01f4\u01fb\u0005"+
		"+\u0000\u0000\u01f5\u01f6\u0003B!\u0000\u01f6\u01f7\u0006\u0012\uffff"+
		"\uffff\u0000\u01f7\u01f8\u0005&\u0000\u0000\u01f8\u01fa\u0001\u0000\u0000"+
		"\u0000\u01f9\u01f5\u0001\u0000\u0000\u0000\u01fa\u01fd\u0001\u0000\u0000"+
		"\u0000\u01fb\u01f9\u0001\u0000\u0000\u0000\u01fb\u01fc\u0001\u0000\u0000"+
		"\u0000\u01fc\u01fe\u0001\u0000\u0000\u0000\u01fd\u01fb\u0001\u0000\u0000"+
		"\u0000\u01fe\u01ff\u0005,\u0000\u0000\u01ff\u0200\u0006\u0012\uffff\uffff"+
		"\u0000\u0200%\u0001\u0000\u0000\u0000\u0201\u0202\u0005\n\u0000\u0000"+
		"\u0202\u0203\u0006\u0013\uffff\uffff\u0000\u0203\u0204\u0001\u0000\u0000"+
		"\u0000\u0204\u0205\u0005U\u0000\u0000\u0205\u0206\u0006\u0013\uffff\uffff"+
		"\u0000\u0206\u020d\u0005+\u0000\u0000\u0207\u0208\u0003B!\u0000\u0208"+
		"\u0209\u0006\u0013\uffff\uffff\u0000\u0209\u020a\u0005&\u0000\u0000\u020a"+
		"\u020c\u0001\u0000\u0000\u0000\u020b\u0207\u0001\u0000\u0000\u0000\u020c"+
		"\u020f\u0001\u0000\u0000\u0000\u020d\u020b\u0001\u0000\u0000\u0000\u020d"+
		"\u020e\u0001\u0000\u0000\u0000\u020e\u0210\u0001\u0000\u0000\u0000\u020f"+
		"\u020d\u0001\u0000\u0000\u0000\u0210\u0211\u0005,\u0000\u0000\u0211\u0212"+
		"\u0006\u0013\uffff\uffff\u0000\u0212\'\u0001\u0000\u0000\u0000\u0213\u0214"+
		"\u0003,\u0016\u0000\u0214\u0215\u0006\u0014\uffff\uffff\u0000\u0215\u0230"+
		"\u0001\u0000\u0000\u0000\u0216\u0217\u00036\u001b\u0000\u0217\u0218\u0006"+
		"\u0014\uffff\uffff\u0000\u0218\u0230\u0001\u0000\u0000\u0000\u0219\u021a"+
		"\u0003.\u0017\u0000\u021a\u021b\u0006\u0014\uffff\uffff\u0000\u021b\u0230"+
		"\u0001\u0000\u0000\u0000\u021c\u021d\u00034\u001a\u0000\u021d\u021e\u0006"+
		"\u0014\uffff\uffff\u0000\u021e\u0230\u0001\u0000\u0000\u0000\u021f\u0220"+
		"\u0003<\u001e\u0000\u0220\u0221\u0006\u0014\uffff\uffff\u0000\u0221\u0230"+
		"\u0001\u0000\u0000\u0000\u0222\u0223\u00038\u001c\u0000\u0223\u0224\u0006"+
		"\u0014\uffff\uffff\u0000\u0224\u0230\u0001\u0000\u0000\u0000\u0225\u0226"+
		"\u0003:\u001d\u0000\u0226\u0227\u0006\u0014\uffff\uffff\u0000\u0227\u0230"+
		"\u0001\u0000\u0000\u0000\u0228\u0229\u0003>\u001f\u0000\u0229\u022a\u0006"+
		"\u0014\uffff\uffff\u0000\u022a\u0230\u0001\u0000\u0000\u0000\u022b\u022c"+
		"\u0003*\u0015\u0000\u022c\u022d\u0005&\u0000\u0000\u022d\u022e\u0006\u0014"+
		"\uffff\uffff\u0000\u022e\u0230\u0001\u0000\u0000\u0000\u022f\u0213\u0001"+
		"\u0000\u0000\u0000\u022f\u0216\u0001\u0000\u0000\u0000\u022f\u0219\u0001"+
		"\u0000\u0000\u0000\u022f\u021c\u0001\u0000\u0000\u0000\u022f\u021f\u0001"+
		"\u0000\u0000\u0000\u022f\u0222\u0001\u0000\u0000\u0000\u022f\u0225\u0001"+
		"\u0000\u0000\u0000\u022f\u0228\u0001\u0000\u0000\u0000\u022f\u022b\u0001"+
		"\u0000\u0000\u0000\u0230)\u0001\u0000\u0000\u0000\u0231\u0232\u0003B!"+
		"\u0000\u0232\u0233\u0006\u0015\uffff\uffff\u0000\u0233+\u0001\u0000\u0000"+
		"\u0000\u0234\u0235\u0005\u000b\u0000\u0000\u0235\u0236\u0006\u0016\uffff"+
		"\uffff\u0000\u0236\u0237\u0005)\u0000\u0000\u0237\u0238\u0003B!\u0000"+
		"\u0238\u0239\u0006\u0016\uffff\uffff\u0000\u0239\u023a\u0005*\u0000\u0000"+
		"\u023a\u023b\u0003(\u0014\u0000\u023b\u0240\u0006\u0016\uffff\uffff\u0000"+
		"\u023c\u023d\u0005\f\u0000\u0000\u023d\u023e\u0003(\u0014\u0000\u023e"+
		"\u023f\u0006\u0016\uffff\uffff\u0000\u023f\u0241\u0001\u0000\u0000\u0000"+
		"\u0240\u023c\u0001\u0000\u0000\u0000\u0240\u0241\u0001\u0000\u0000\u0000"+
		"\u0241\u0242\u0001\u0000\u0000\u0000\u0242\u0243\u0006\u0016\uffff\uffff"+
		"\u0000\u0243-\u0001\u0000\u0000\u0000\u0244\u0245\u0005\r\u0000\u0000"+
		"\u0245\u0246\u0006\u0017\uffff\uffff\u0000\u0246\u024a\u0005)\u0000\u0000"+
		"\u0247\u0248\u0003\u0014\n\u0000\u0248\u0249\u0006\u0017\uffff\uffff\u0000"+
		"\u0249\u024b\u0001\u0000\u0000\u0000\u024a\u0247\u0001\u0000\u0000\u0000"+
		"\u024a\u024b\u0001\u0000\u0000\u0000\u024b\u024c\u0001\u0000\u0000\u0000"+
		"\u024c\u0250\u0005&\u0000\u0000\u024d\u024e\u0003B!\u0000\u024e\u024f"+
		"\u0006\u0017\uffff\uffff\u0000\u024f\u0251\u0001\u0000\u0000\u0000\u0250"+
		"\u024d\u0001\u0000\u0000\u0000\u0250\u0251\u0001\u0000\u0000\u0000\u0251"+
		"\u0252\u0001\u0000\u0000\u0000\u0252\u0256\u0005&\u0000\u0000\u0253\u0254"+
		"\u0003B!\u0000\u0254\u0255\u0006\u0017\uffff\uffff\u0000\u0255\u0257\u0001"+
		"\u0000\u0000\u0000\u0256\u0253\u0001\u0000\u0000\u0000\u0256\u0257\u0001"+
		"\u0000\u0000\u0000\u0257\u0258\u0001\u0000\u0000\u0000\u0258\u025e\u0005"+
		"*\u0000\u0000\u0259\u025a\u0003\u0010\b\u0000\u025a\u025b\u0006\u0017"+
		"\uffff\uffff\u0000\u025b\u025d\u0001\u0000\u0000\u0000\u025c\u0259\u0001"+
		"\u0000\u0000\u0000\u025d\u0260\u0001\u0000\u0000\u0000\u025e\u025c\u0001"+
		"\u0000\u0000\u0000\u025e\u025f\u0001\u0000\u0000\u0000\u025f\u0261\u0001"+
		"\u0000\u0000\u0000\u0260\u025e\u0001\u0000\u0000\u0000\u0261\u0262\u0003"+
		"(\u0014\u0000\u0262\u0263\u0006\u0017\uffff\uffff\u0000\u0263\u0264\u0006"+
		"\u0017\uffff\uffff\u0000\u0264/\u0001\u0000\u0000\u0000\u0265\u0266\u0005"+
		"\r\u0000\u0000\u0266\u0267\u0006\u0018\uffff\uffff\u0000\u0267\u0268\u0005"+
		")\u0000\u0000\u0268\u0269\u0005U\u0000\u0000\u0269\u026a\u0005(\u0000"+
		"\u0000\u026a\u026b\u0005U\u0000\u0000\u026b\u026c\u0005\u000e\u0000\u0000"+
		"\u026c\u026d\u0005U\u0000\u0000\u026d\u026e\u0005*\u0000\u0000\u026e\u026f"+
		"\u0003(\u0014\u0000\u026f\u0270\u0006\u0018\uffff\uffff\u0000\u0270\u0271"+
		"\u0006\u0018\uffff\uffff\u0000\u02711\u0001\u0000\u0000\u0000\u0272\u0273"+
		"\u0005\u000f\u0000\u0000\u0273\u0274\u0006\u0019\uffff\uffff\u0000\u0274"+
		"\u0275\u0005)\u0000\u0000\u0275\u0276\u0003B!\u0000\u0276\u0277\u0005"+
		"*\u0000\u0000\u0277\u0278\u0003(\u0014\u0000\u0278\u0279\u0006\u0019\uffff"+
		"\uffff\u0000\u0279\u027a\u0006\u0019\uffff\uffff\u0000\u027a3\u0001\u0000"+
		"\u0000\u0000\u027b\u027c\u0005\u0010\u0000\u0000\u027c\u027d\u0006\u001a"+
		"\uffff\uffff\u0000\u027d\u027e\u0005)\u0000\u0000\u027e\u027f\u0003B!"+
		"\u0000\u027f\u0280\u0006\u001a\uffff\uffff\u0000\u0280\u0281\u0005*\u0000"+
		"\u0000\u0281\u0282\u0003(\u0014\u0000\u0282\u0283\u0006\u001a\uffff\uffff"+
		"\u0000\u0283\u0284\u0006\u001a\uffff\uffff\u0000\u0284\u0292\u0001\u0000"+
		"\u0000\u0000\u0285\u0286\u0005\u0011\u0000\u0000\u0286\u0287\u0006\u001a"+
		"\uffff\uffff\u0000\u0287\u0288\u0003(\u0014\u0000\u0288\u0289\u0006\u001a"+
		"\uffff\uffff\u0000\u0289\u028a\u0005\u0010\u0000\u0000\u028a\u028b\u0005"+
		")\u0000\u0000\u028b\u028c\u0003B!\u0000\u028c\u028d\u0006\u001a\uffff"+
		"\uffff\u0000\u028d\u028e\u0005*\u0000\u0000\u028e\u028f\u0005&\u0000\u0000"+
		"\u028f\u0290\u0006\u001a\uffff\uffff\u0000\u0290\u0292\u0001\u0000\u0000"+
		"\u0000\u0291\u027b\u0001\u0000\u0000\u0000\u0291\u0285\u0001\u0000\u0000"+
		"\u0000\u02925\u0001\u0000\u0000\u0000\u0293\u0294\u0005+\u0000\u0000\u0294"+
		"\u029f\u0006\u001b\uffff\uffff\u0000\u0295\u0296\u0003\u0014\n\u0000\u0296"+
		"\u0297\u0005&\u0000\u0000\u0297\u0298\u0006\u001b\uffff\uffff\u0000\u0298"+
		"\u029e\u0001\u0000\u0000\u0000\u0299\u029a\u0003(\u0014\u0000\u029a\u029b"+
		"\u0006\u001b\uffff\uffff\u0000\u029b\u029e\u0001\u0000\u0000\u0000\u029c"+
		"\u029e\u0003\n\u0005\u0000\u029d\u0295\u0001\u0000\u0000\u0000\u029d\u0299"+
		"\u0001\u0000\u0000\u0000\u029d\u029c\u0001\u0000\u0000\u0000\u029e\u02a1"+
		"\u0001\u0000\u0000\u0000\u029f\u029d\u0001\u0000\u0000\u0000\u029f\u02a0"+
		"\u0001\u0000\u0000\u0000\u02a0\u02a2\u0001\u0000\u0000\u0000\u02a1\u029f"+
		"\u0001\u0000\u0000\u0000\u02a2\u02a3\u0005,\u0000\u0000\u02a3\u02a4\u0006"+
		"\u001b\uffff\uffff\u0000\u02a4\u02a5\u0006\u001b\uffff\uffff\u0000\u02a5"+
		"7\u0001\u0000\u0000\u0000\u02a6\u02a7\u0005\u0012\u0000\u0000\u02a7\u02ab"+
		"\u0006\u001c\uffff\uffff\u0000\u02a8\u02a9\u0003B!\u0000\u02a9\u02aa\u0006"+
		"\u001c\uffff\uffff\u0000\u02aa\u02ac\u0001\u0000\u0000\u0000\u02ab\u02a8"+
		"\u0001\u0000\u0000\u0000\u02ab\u02ac\u0001\u0000\u0000\u0000\u02ac\u02ad"+
		"\u0001\u0000\u0000\u0000\u02ad\u02ae\u0005&\u0000\u0000\u02ae\u02bc\u0006"+
		"\u001c\uffff\uffff\u0000\u02af\u02b0\u0005\u0013\u0000\u0000\u02b0\u02b1"+
		"\u0006\u001c\uffff\uffff\u0000\u02b1\u02b2\u0005&\u0000\u0000\u02b2\u02bc"+
		"\u0006\u001c\uffff\uffff\u0000\u02b3\u02b4\u0005\u0014\u0000\u0000\u02b4"+
		"\u02b5\u0006\u001c\uffff\uffff\u0000\u02b5\u02b6\u0005&\u0000\u0000\u02b6"+
		"\u02bc\u0006\u001c\uffff\uffff\u0000\u02b7\u02b8\u0005\u0015\u0000\u0000"+
		"\u02b8\u02b9\u0006\u001c\uffff\uffff\u0000\u02b9\u02ba\u0005&\u0000\u0000"+
		"\u02ba\u02bc\u0006\u001c\uffff\uffff\u0000\u02bb\u02a6\u0001\u0000\u0000"+
		"\u0000\u02bb\u02af\u0001\u0000\u0000\u0000\u02bb\u02b3\u0001\u0000\u0000"+
		"\u0000\u02bb\u02b7\u0001\u0000\u0000\u0000\u02bc9\u0001\u0000\u0000\u0000"+
		"\u02bd\u02be\u0005\u0016\u0000\u0000\u02be\u02bf\u0006\u001d\uffff\uffff"+
		"\u0000\u02bf\u02c0\u0005&\u0000\u0000\u02c0\u02c1\u0006\u001d\uffff\uffff"+
		"\u0000\u02c1;\u0001\u0000\u0000\u0000\u02c2\u02c3\u0005\u0017\u0000\u0000"+
		"\u02c3\u02c4\u0006\u001e\uffff\uffff\u0000\u02c4\u02c5\u0005)\u0000\u0000"+
		"\u02c5\u02c6\u0003B!\u0000\u02c6\u02c7\u0005*\u0000\u0000\u02c7\u02c8"+
		"\u0006\u001e\uffff\uffff\u0000\u02c8\u02d4\u0005+\u0000\u0000\u02c9\u02ca"+
		"\u0005\u0018\u0000\u0000\u02ca\u02cb\u0003B!\u0000\u02cb\u02cc\u0005("+
		"\u0000\u0000\u02cc\u02d0\u0006\u001e\uffff\uffff\u0000\u02cd\u02ce\u0003"+
		"(\u0014\u0000\u02ce\u02cf\u0006\u001e\uffff\uffff\u0000\u02cf\u02d1\u0001"+
		"\u0000\u0000\u0000\u02d0\u02cd\u0001\u0000\u0000\u0000\u02d0\u02d1\u0001"+
		"\u0000\u0000\u0000\u02d1\u02d3\u0001\u0000\u0000\u0000\u02d2\u02c9\u0001"+
		"\u0000\u0000\u0000\u02d3\u02d6\u0001\u0000\u0000\u0000\u02d4\u02d2\u0001"+
		"\u0000\u0000\u0000\u02d4\u02d5\u0001\u0000\u0000\u0000\u02d5\u02de\u0001"+
		"\u0000\u0000\u0000\u02d6\u02d4\u0001\u0000\u0000\u0000\u02d7\u02d8\u0005"+
		"\u0019\u0000\u0000\u02d8\u02dc\u0005(\u0000\u0000\u02d9\u02da\u0003(\u0014"+
		"\u0000\u02da\u02db\u0006\u001e\uffff\uffff\u0000\u02db\u02dd\u0001\u0000"+
		"\u0000\u0000\u02dc\u02d9\u0001\u0000\u0000\u0000\u02dc\u02dd\u0001\u0000"+
		"\u0000\u0000\u02dd\u02df\u0001\u0000\u0000\u0000\u02de\u02d7\u0001\u0000"+
		"\u0000\u0000\u02de\u02df\u0001\u0000\u0000\u0000\u02df\u02e0\u0001\u0000"+
		"\u0000\u0000\u02e0\u02e1\u0005,\u0000\u0000\u02e1\u02e2\u0006\u001e\uffff"+
		"\uffff\u0000\u02e2=\u0001\u0000\u0000\u0000\u02e3\u02e4\u0005\u001a\u0000"+
		"\u0000\u02e4\u02e5\u0006\u001f\uffff\uffff\u0000\u02e5\u02e6\u0005&\u0000"+
		"\u0000\u02e6\u02e7\u0006\u001f\uffff\uffff\u0000\u02e7?\u0001\u0000\u0000"+
		"\u0000\u02e8\u02e9\u0006 \uffff\uffff\u0000\u02e9\u02ea\u0007\u0003\u0000"+
		"\u0000\u02ea\u02eb\u0003@ \u000e\u02eb\u02ec\u0006 \uffff\uffff\u0000"+
		"\u02ec\u02f1\u0001\u0000\u0000\u0000\u02ed\u02ee\u0003D\"\u0000\u02ee"+
		"\u02ef\u0006 \uffff\uffff\u0000\u02ef\u02f1\u0001\u0000\u0000\u0000\u02f0"+
		"\u02e8\u0001\u0000\u0000\u0000\u02f0\u02ed\u0001\u0000\u0000\u0000\u02f1"+
		"\u037c\u0001\u0000\u0000\u0000\u02f2\u02f3\n\u0011\u0000\u0000\u02f3\u02f4"+
		"\u0006 \uffff\uffff\u0000\u02f4\u02f5\u0005/\u0000\u0000\u02f5\u02f6\u0006"+
		" \uffff\uffff\u0000\u02f6\u02f7\u0003@ \u0012\u02f7\u02f8\u0006 \uffff"+
		"\uffff\u0000\u02f8\u037b\u0001\u0000\u0000\u0000\u02f9\u02fa\n\u0010\u0000"+
		"\u0000\u02fa\u02fb\u0006 \uffff\uffff\u0000\u02fb\u02fc\u0005M\u0000\u0000"+
		"\u02fc\u02fd\u0006 \uffff\uffff\u0000\u02fd\u02fe\u0003@ \u0011\u02fe"+
		"\u02ff\u0006 \uffff\uffff\u0000\u02ff\u037b\u0001\u0000\u0000\u0000\u0300"+
		"\u0301\n\r\u0000\u0000\u0301\u0302\u0006 \uffff\uffff\u0000\u0302\u0303"+
		"\u0007\u0004\u0000\u0000\u0303\u0304\u0006 \uffff\uffff\u0000\u0304\u0305"+
		"\u0003@ \u000e\u0305\u0306\u0006 \uffff\uffff\u0000\u0306\u037b\u0001"+
		"\u0000\u0000\u0000\u0307\u0308\n\f\u0000\u0000\u0308\u0309\u0006 \uffff"+
		"\uffff\u0000\u0309\u030a\u0007\u0005\u0000\u0000\u030a\u030b\u0006 \uffff"+
		"\uffff\u0000\u030b\u030c\u0003@ \r\u030c\u030d\u0006 \uffff\uffff\u0000"+
		"\u030d\u037b\u0001\u0000\u0000\u0000\u030e\u030f\n\u000b\u0000\u0000\u030f"+
		"\u0310\u0006 \uffff\uffff\u0000\u0310\u0311\u0007\u0006\u0000\u0000\u0311"+
		"\u0312\u0006 \uffff\uffff\u0000\u0312\u0313\u0003@ \f\u0313\u0314\u0006"+
		" \uffff\uffff\u0000\u0314\u037b\u0001\u0000\u0000\u0000\u0315\u0316\n"+
		"\n\u0000\u0000\u0316\u0317\u0006 \uffff\uffff\u0000\u0317\u0318\u0007"+
		"\u0007\u0000\u0000\u0318\u0319\u0006 \uffff\uffff\u0000\u0319\u031a\u0003"+
		"@ \u000b\u031a\u031b\u0006 \uffff\uffff\u0000\u031b\u037b\u0001\u0000"+
		"\u0000\u0000\u031c\u031d\n\t\u0000\u0000\u031d\u031e\u0006 \uffff\uffff"+
		"\u0000\u031e\u031f\u0007\b\u0000\u0000\u031f\u0320\u0006 \uffff\uffff"+
		"\u0000\u0320\u0321\u0003@ \n\u0321\u0322\u0006 \uffff\uffff\u0000\u0322"+
		"\u037b\u0001\u0000\u0000\u0000\u0323\u0324\n\b\u0000\u0000\u0324\u0325"+
		"\u0006 \uffff\uffff\u0000\u0325\u0326\u00054\u0000\u0000\u0326\u0327\u0006"+
		" \uffff\uffff\u0000\u0327\u0328\u0003@ \t\u0328\u0329\u0006 \uffff\uffff"+
		"\u0000\u0329\u037b\u0001\u0000\u0000\u0000\u032a\u032b\n\u0007\u0000\u0000"+
		"\u032b\u032c\u0006 \uffff\uffff\u0000\u032c\u032d\u00058\u0000\u0000\u032d"+
		"\u032e\u0006 \uffff\uffff\u0000\u032e\u032f\u0003@ \b\u032f\u0330\u0006"+
		" \uffff\uffff\u0000\u0330\u037b\u0001\u0000\u0000\u0000\u0331\u0332\n"+
		"\u0006\u0000\u0000\u0332\u0333\u0006 \uffff\uffff\u0000\u0333\u0334\u0005"+
		"6\u0000\u0000\u0334\u0335\u0006 \uffff\uffff\u0000\u0335\u0336\u0003@"+
		" \u0007\u0336\u0337\u0006 \uffff\uffff\u0000\u0337\u037b\u0001\u0000\u0000"+
		"\u0000\u0338\u0339\n\u0005\u0000\u0000\u0339\u033a\u0006 \uffff\uffff"+
		"\u0000\u033a\u033b\u0005D\u0000\u0000\u033b\u033c\u0006 \uffff\uffff\u0000"+
		"\u033c\u033d\u0003@ \u0006\u033d\u033e\u0006 \uffff\uffff\u0000\u033e"+
		"\u037b\u0001\u0000\u0000\u0000\u033f\u0340\n\u0004\u0000\u0000\u0340\u0341"+
		"\u0006 \uffff\uffff\u0000\u0341\u0342\u0005E\u0000\u0000\u0342\u0343\u0006"+
		" \uffff\uffff\u0000\u0343\u0344\u0003@ \u0005\u0344\u0345\u0006 \uffff"+
		"\uffff\u0000\u0345\u037b\u0001\u0000\u0000\u0000\u0346\u0347\n\u0003\u0000"+
		"\u0000\u0347\u0348\u00053\u0000\u0000\u0348\u0349\u0006 \uffff\uffff\u0000"+
		"\u0349\u034a\u0003@ \u0000\u034a\u034b\u0005(\u0000\u0000\u034b\u034c"+
		"\u0003@ \u0003\u034c\u034d\u0006 \uffff\uffff\u0000\u034d\u037b\u0001"+
		"\u0000\u0000\u0000\u034e\u034f\n\u0002\u0000\u0000\u034f\u0350\u0006 "+
		"\uffff\uffff\u0000\u0350\u0351\u0007\t\u0000\u0000\u0351\u0352\u0006 "+
		"\uffff\uffff\u0000\u0352\u0353\u0003@ \u0002\u0353\u0354\u0006 \uffff"+
		"\uffff\u0000\u0354\u037b\u0001\u0000\u0000\u0000\u0355\u0356\n\u0013\u0000"+
		"\u0000\u0356\u0357\u0006 \uffff\uffff\u0000\u0357\u0358\u0007\n\u0000"+
		"\u0000\u0358\u0359\u0006 \uffff\uffff\u0000\u0359\u037b\u0006 \uffff\uffff"+
		"\u0000\u035a\u035b\n\u0012\u0000\u0000\u035b\u035c\u0006 \uffff\uffff"+
		"\u0000\u035c\u035d\u0005)\u0000\u0000\u035d\u036d\u0006 \uffff\uffff\u0000"+
		"\u035e\u035f\u0003@ \u0000\u035f\u0361\u0006 \uffff\uffff\u0000\u0360"+
		"\u0362\u0003\n\u0005\u0000\u0361\u0360\u0001\u0000\u0000\u0000\u0361\u0362"+
		"\u0001\u0000\u0000\u0000\u0362\u036a\u0001\u0000\u0000\u0000\u0363\u0364"+
		"\u0005\'\u0000\u0000\u0364\u0365\u0003@ \u0000\u0365\u0366\u0006 \uffff"+
		"\uffff\u0000\u0366\u0369\u0001\u0000\u0000\u0000\u0367\u0369\u0003\n\u0005"+
		"\u0000\u0368\u0363\u0001\u0000\u0000\u0000\u0368\u0367\u0001\u0000\u0000"+
		"\u0000\u0369\u036c\u0001\u0000\u0000\u0000\u036a\u0368\u0001\u0000\u0000"+
		"\u0000\u036a\u036b\u0001\u0000\u0000\u0000\u036b\u036e\u0001\u0000\u0000"+
		"\u0000\u036c\u036a\u0001\u0000\u0000\u0000\u036d\u035e\u0001\u0000\u0000"+
		"\u0000\u036d\u036e\u0001\u0000\u0000\u0000\u036e\u036f\u0001\u0000\u0000"+
		"\u0000\u036f\u0370\u0005*\u0000\u0000\u0370\u037b\u0006 \uffff\uffff\u0000"+
		"\u0371\u0372\n\u000f\u0000\u0000\u0372\u0373\u0006 \uffff\uffff\u0000"+
		"\u0373\u0374\u0005-\u0000\u0000\u0374\u0376\u0006 \uffff\uffff\u0000\u0375"+
		"\u0377\u0003@ \u0000\u0376\u0375\u0001\u0000\u0000\u0000\u0376\u0377\u0001"+
		"\u0000\u0000\u0000\u0377\u0378\u0001\u0000\u0000\u0000\u0378\u0379\u0005"+
		".\u0000\u0000\u0379\u037b\u0006 \uffff\uffff\u0000\u037a\u02f2\u0001\u0000"+
		"\u0000\u0000\u037a\u02f9\u0001\u0000\u0000\u0000\u037a\u0300\u0001\u0000"+
		"\u0000\u0000\u037a\u0307\u0001\u0000\u0000\u0000\u037a\u030e\u0001\u0000"+
		"\u0000\u0000\u037a\u0315\u0001\u0000\u0000\u0000\u037a\u031c\u0001\u0000"+
		"\u0000\u0000\u037a\u0323\u0001\u0000\u0000\u0000\u037a\u032a\u0001\u0000"+
		"\u0000\u0000\u037a\u0331\u0001\u0000\u0000\u0000\u037a\u0338\u0001\u0000"+
		"\u0000\u0000\u037a\u033f\u0001\u0000\u0000\u0000\u037a\u0346\u0001\u0000"+
		"\u0000\u0000\u037a\u034e\u0001\u0000\u0000\u0000\u037a\u0355\u0001\u0000"+
		"\u0000\u0000\u037a\u035a\u0001\u0000\u0000\u0000\u037a\u0371\u0001\u0000"+
		"\u0000\u0000\u037b\u037e\u0001\u0000\u0000\u0000\u037c\u037a\u0001\u0000"+
		"\u0000\u0000\u037c\u037d\u0001\u0000\u0000\u0000\u037dA\u0001\u0000\u0000"+
		"\u0000\u037e\u037c\u0001\u0000\u0000\u0000\u037f\u0380\u0006!\uffff\uffff"+
		"\u0000\u0380\u0381\u0007\u0003\u0000\u0000\u0381\u0382\u0003B!\u000f\u0382"+
		"\u0383\u0006!\uffff\uffff\u0000\u0383\u0388\u0001\u0000\u0000\u0000\u0384"+
		"\u0385\u0003D\"\u0000\u0385\u0386\u0006!\uffff\uffff\u0000\u0386\u0388"+
		"\u0001\u0000\u0000\u0000\u0387\u037f\u0001\u0000\u0000\u0000\u0387\u0384"+
		"\u0001\u0000\u0000\u0000\u0388\u041a\u0001\u0000\u0000\u0000\u0389\u038a"+
		"\n\u0012\u0000\u0000\u038a\u038b\u0006!\uffff\uffff\u0000\u038b\u038c"+
		"\u0005/\u0000\u0000\u038c\u038d\u0006!\uffff\uffff\u0000\u038d\u038e\u0003"+
		"B!\u0013\u038e\u038f\u0006!\uffff\uffff\u0000\u038f\u0419\u0001\u0000"+
		"\u0000\u0000\u0390\u0391\n\u0011\u0000\u0000\u0391\u0392\u0006!\uffff"+
		"\uffff\u0000\u0392\u0393\u0005M\u0000\u0000\u0393\u0394\u0006!\uffff\uffff"+
		"\u0000\u0394\u0395\u0003B!\u0012\u0395\u0396\u0006!\uffff\uffff\u0000"+
		"\u0396\u0419\u0001\u0000\u0000\u0000\u0397\u0398\n\u000e\u0000\u0000\u0398"+
		"\u0399\u0006!\uffff\uffff\u0000\u0399\u039a\u0007\u0004\u0000\u0000\u039a"+
		"\u039b\u0006!\uffff\uffff\u0000\u039b\u039c\u0003B!\u000f\u039c\u039d"+
		"\u0006!\uffff\uffff\u0000\u039d\u0419\u0001\u0000\u0000\u0000\u039e\u039f"+
		"\n\r\u0000\u0000\u039f\u03a0\u0006!\uffff\uffff\u0000\u03a0\u03a1\u0007"+
		"\u0005\u0000\u0000\u03a1\u03a2\u0006!\uffff\uffff\u0000\u03a2\u03a3\u0003"+
		"B!\u000e\u03a3\u03a4\u0006!\uffff\uffff\u0000\u03a4\u0419\u0001\u0000"+
		"\u0000\u0000\u03a5\u03a6\n\f\u0000\u0000\u03a6\u03a7\u0006!\uffff\uffff"+
		"\u0000\u03a7\u03a8\u0007\u0006\u0000\u0000\u03a8\u03a9\u0006!\uffff\uffff"+
		"\u0000\u03a9\u03aa\u0003B!\r\u03aa\u03ab\u0006!\uffff\uffff\u0000\u03ab"+
		"\u0419\u0001\u0000\u0000\u0000\u03ac\u03ad\n\u000b\u0000\u0000\u03ad\u03ae"+
		"\u0006!\uffff\uffff\u0000\u03ae\u03af\u0007\u0007\u0000\u0000\u03af\u03b0"+
		"\u0006!\uffff\uffff\u0000\u03b0\u03b1\u0003B!\f\u03b1\u03b2\u0006!\uffff"+
		"\uffff\u0000\u03b2\u0419\u0001\u0000\u0000\u0000\u03b3\u03b4\n\n\u0000"+
		"\u0000\u03b4\u03b5\u0006!\uffff\uffff\u0000\u03b5\u03b6\u0007\b\u0000"+
		"\u0000\u03b6\u03b7\u0006!\uffff\uffff\u0000\u03b7\u03b8\u0003B!\u000b"+
		"\u03b8\u03b9\u0006!\uffff\uffff\u0000\u03b9\u0419\u0001\u0000\u0000\u0000"+
		"\u03ba\u03bb\n\t\u0000\u0000\u03bb\u03bc\u0006!\uffff\uffff\u0000\u03bc"+
		"\u03bd\u00054\u0000\u0000\u03bd\u03be\u0006!\uffff\uffff\u0000\u03be\u03bf"+
		"\u0003B!\n\u03bf\u03c0\u0006!\uffff\uffff\u0000\u03c0\u0419\u0001\u0000"+
		"\u0000\u0000\u03c1\u03c2\n\b\u0000\u0000\u03c2\u03c3\u0006!\uffff\uffff"+
		"\u0000\u03c3\u03c4\u00058\u0000\u0000\u03c4\u03c5\u0006!\uffff\uffff\u0000"+
		"\u03c5\u03c6\u0003B!\t\u03c6\u03c7\u0006!\uffff\uffff\u0000\u03c7\u0419"+
		"\u0001\u0000\u0000\u0000\u03c8\u03c9\n\u0007\u0000\u0000\u03c9\u03ca\u0006"+
		"!\uffff\uffff\u0000\u03ca\u03cb\u00056\u0000\u0000\u03cb\u03cc\u0006!"+
		"\uffff\uffff\u0000\u03cc\u03cd\u0003B!\b\u03cd\u03ce\u0006!\uffff\uffff"+
		"\u0000\u03ce\u0419\u0001\u0000\u0000\u0000\u03cf\u03d0\n\u0006\u0000\u0000"+
		"\u03d0\u03d1\u0006!\uffff\uffff\u0000\u03d1\u03d2\u0005D\u0000\u0000\u03d2"+
		"\u03d3\u0006!\uffff\uffff\u0000\u03d3\u03d4\u0003B!\u0007\u03d4\u03d5"+
		"\u0006!\uffff\uffff\u0000\u03d5\u0419\u0001\u0000\u0000\u0000\u03d6\u03d7"+
		"\n\u0005\u0000\u0000\u03d7\u03d8\u0006!\uffff\uffff\u0000\u03d8\u03d9"+
		"\u0005E\u0000\u0000\u03d9\u03da\u0006!\uffff\uffff\u0000\u03da\u03db\u0003"+
		"B!\u0006\u03db\u03dc\u0006!\uffff\uffff\u0000\u03dc\u0419\u0001\u0000"+
		"\u0000\u0000\u03dd\u03de\n\u0004\u0000\u0000\u03de\u03df\u00053\u0000"+
		"\u0000\u03df\u03e0\u0006!\uffff\uffff\u0000\u03e0\u03e1\u0003B!\u0000"+
		"\u03e1\u03e2\u0005(\u0000\u0000\u03e2\u03e3\u0003B!\u0004\u03e3\u03e4"+
		"\u0006!\uffff\uffff\u0000\u03e4\u0419\u0001\u0000\u0000\u0000\u03e5\u03e6"+
		"\n\u0003\u0000\u0000\u03e6\u03e7\u0006!\uffff\uffff\u0000\u03e7\u03e8"+
		"\u0007\t\u0000\u0000\u03e8\u03e9\u0006!\uffff\uffff\u0000\u03e9\u03ea"+
		"\u0003B!\u0003\u03ea\u03eb\u0006!\uffff\uffff\u0000\u03eb\u0419\u0001"+
		"\u0000\u0000\u0000\u03ec\u03ed\n\u0002\u0000\u0000\u03ed\u03ee\u0006!"+
		"\uffff\uffff\u0000\u03ee\u03ef\u0005\'\u0000\u0000\u03ef\u03f0\u0006!"+
		"\uffff\uffff\u0000\u03f0\u03f1\u0003B!\u0003\u03f1\u03f2\u0006!\uffff"+
		"\uffff\u0000\u03f2\u0419\u0001\u0000\u0000\u0000\u03f3\u03f4\n\u0014\u0000"+
		"\u0000\u03f4\u03f5\u0006!\uffff\uffff\u0000\u03f5\u03f6\u0007\n\u0000"+
		"\u0000\u03f6\u03f7\u0006!\uffff\uffff\u0000\u03f7\u0419\u0006!\uffff\uffff"+
		"\u0000\u03f8\u03f9\n\u0013\u0000\u0000\u03f9\u03fa\u0006!\uffff\uffff"+
		"\u0000\u03fa\u03fb\u0005)\u0000\u0000\u03fb\u040b\u0006!\uffff\uffff\u0000"+
		"\u03fc\u03fd\u0003@ \u0000\u03fd\u03ff\u0006!\uffff\uffff\u0000\u03fe"+
		"\u0400\u0003\n\u0005\u0000\u03ff\u03fe\u0001\u0000\u0000\u0000\u03ff\u0400"+
		"\u0001\u0000\u0000\u0000\u0400\u0408\u0001\u0000\u0000\u0000\u0401\u0402"+
		"\u0005\'\u0000\u0000\u0402\u0403\u0003@ \u0000\u0403\u0404\u0006!\uffff"+
		"\uffff\u0000\u0404\u0407\u0001\u0000\u0000\u0000\u0405\u0407\u0003\n\u0005"+
		"\u0000\u0406\u0401\u0001\u0000\u0000\u0000\u0406\u0405\u0001\u0000\u0000"+
		"\u0000\u0407\u040a\u0001\u0000\u0000\u0000\u0408\u0406\u0001\u0000\u0000"+
		"\u0000\u0408\u0409\u0001\u0000\u0000\u0000\u0409\u040c\u0001\u0000\u0000"+
		"\u0000\u040a\u0408\u0001\u0000\u0000\u0000\u040b\u03fc\u0001\u0000\u0000"+
		"\u0000\u040b\u040c\u0001\u0000\u0000\u0000\u040c\u040d\u0001\u0000\u0000"+
		"\u0000\u040d\u040e\u0005*\u0000\u0000\u040e\u0419\u0006!\uffff\uffff\u0000"+
		"\u040f\u0410\n\u0010\u0000\u0000\u0410\u0411\u0006!\uffff\uffff\u0000"+
		"\u0411\u0412\u0005-\u0000\u0000\u0412\u0414\u0006!\uffff\uffff\u0000\u0413"+
		"\u0415\u0003B!\u0000\u0414\u0413\u0001\u0000\u0000\u0000\u0414\u0415\u0001"+
		"\u0000\u0000\u0000\u0415\u0416\u0001\u0000\u0000\u0000\u0416\u0417\u0005"+
		".\u0000\u0000\u0417\u0419\u0006!\uffff\uffff\u0000\u0418\u0389\u0001\u0000"+
		"\u0000\u0000\u0418\u0390\u0001\u0000\u0000\u0000\u0418\u0397\u0001\u0000"+
		"\u0000\u0000\u0418\u039e\u0001\u0000\u0000\u0000\u0418\u03a5\u0001\u0000"+
		"\u0000\u0000\u0418\u03ac\u0001\u0000\u0000\u0000\u0418\u03b3\u0001\u0000"+
		"\u0000\u0000\u0418\u03ba\u0001\u0000\u0000\u0000\u0418\u03c1\u0001\u0000"+
		"\u0000\u0000\u0418\u03c8\u0001\u0000\u0000\u0000\u0418\u03cf\u0001\u0000"+
		"\u0000\u0000\u0418\u03d6\u0001\u0000\u0000\u0000\u0418\u03dd\u0001\u0000"+
		"\u0000\u0000\u0418\u03e5\u0001\u0000\u0000\u0000\u0418\u03ec\u0001\u0000"+
		"\u0000\u0000\u0418\u03f3\u0001\u0000\u0000\u0000\u0418\u03f8\u0001\u0000"+
		"\u0000\u0000\u0418\u040f\u0001\u0000\u0000\u0000\u0419\u041c\u0001\u0000"+
		"\u0000\u0000\u041a\u0418\u0001\u0000\u0000\u0000\u041a\u041b\u0001\u0000"+
		"\u0000\u0000\u041bC\u0001\u0000\u0000\u0000\u041c\u041a\u0001\u0000\u0000"+
		"\u0000\u041d\u041e\u0003F#\u0000\u041e\u041f\u0006\"\uffff\uffff\u0000"+
		"\u041f\u043c\u0001\u0000\u0000\u0000\u0420\u0421\u0003H$\u0000\u0421\u0422"+
		"\u0006\"\uffff\uffff\u0000\u0422\u043c\u0001\u0000\u0000\u0000\u0423\u0424"+
		"\u0005)\u0000\u0000\u0424\u0425\u0003B!\u0000\u0425\u0426\u0005*\u0000"+
		"\u0000\u0426\u0427\u0006\"\uffff\uffff\u0000\u0427\u043c\u0001\u0000\u0000"+
		"\u0000\u0428\u0429\u0005N\u0000\u0000\u0429\u043c\u0006\"\uffff\uffff"+
		"\u0000\u042a\u042b\u0005O\u0000\u0000\u042b\u043c\u0006\"\uffff\uffff"+
		"\u0000\u042c\u042d\u0005R\u0000\u0000\u042d\u043c\u0006\"\uffff\uffff"+
		"\u0000\u042e\u042f\u0005S\u0000\u0000\u042f\u043c\u0006\"\uffff\uffff"+
		"\u0000\u0430\u0431\u0005T\u0000\u0000\u0431\u043c\u0006\"\uffff\uffff"+
		"\u0000\u0432\u0433\u0003\u0000\u0000\u0000\u0433\u0434\u0006\"\uffff\uffff"+
		"\u0000\u0434\u043c\u0001\u0000\u0000\u0000\u0435\u0436\u0005U\u0000\u0000"+
		"\u0436\u043c\u0006\"\uffff\uffff\u0000\u0437\u0438\u0003\u0004\u0002\u0000"+
		"\u0438\u0439\u0006\"\uffff\uffff\u0000\u0439\u043c\u0001\u0000\u0000\u0000"+
		"\u043a\u043c\u0003\n\u0005\u0000\u043b\u041d\u0001\u0000\u0000\u0000\u043b"+
		"\u0420\u0001\u0000\u0000\u0000\u043b\u0423\u0001\u0000\u0000\u0000\u043b"+
		"\u0428\u0001\u0000\u0000\u0000\u043b\u042a\u0001\u0000\u0000\u0000\u043b"+
		"\u042c\u0001\u0000\u0000\u0000\u043b\u042e\u0001\u0000\u0000\u0000\u043b"+
		"\u0430\u0001\u0000\u0000\u0000\u043b\u0432\u0001\u0000\u0000\u0000\u043b"+
		"\u0435\u0001\u0000\u0000\u0000\u043b\u0437\u0001\u0000\u0000\u0000\u043b"+
		"\u043a\u0001\u0000\u0000\u0000\u043cE\u0001\u0000\u0000\u0000\u043d\u043e"+
		"\u0005U\u0000\u0000\u043e\u043f\u0006#\uffff\uffff\u0000\u043f\u0440\u0005"+
		"+\u0000\u0000\u0440\u0450\u0006#\uffff\uffff\u0000\u0441\u0442\u0003@"+
		" \u0000\u0442\u0444\u0006#\uffff\uffff\u0000\u0443\u0445\u0003\n\u0005"+
		"\u0000\u0444\u0443\u0001\u0000\u0000\u0000\u0444\u0445\u0001\u0000\u0000"+
		"\u0000\u0445\u044d\u0001\u0000\u0000\u0000\u0446\u0447\u0005\'\u0000\u0000"+
		"\u0447\u0448\u0003@ \u0000\u0448\u0449\u0006#\uffff\uffff\u0000\u0449"+
		"\u044c\u0001\u0000\u0000\u0000\u044a\u044c\u0003\n\u0005\u0000\u044b\u0446"+
		"\u0001\u0000\u0000\u0000\u044b\u044a\u0001\u0000\u0000\u0000\u044c\u044f"+
		"\u0001\u0000\u0000\u0000\u044d\u044b\u0001\u0000\u0000\u0000\u044d\u044e"+
		"\u0001\u0000\u0000\u0000\u044e\u0451\u0001\u0000\u0000\u0000\u044f\u044d"+
		"\u0001\u0000\u0000\u0000\u0450\u0441\u0001\u0000\u0000\u0000\u0450\u0451"+
		"\u0001\u0000\u0000\u0000\u0451\u0452\u0001\u0000\u0000\u0000\u0452\u0453"+
		"\u0005,\u0000\u0000\u0453\u0454\u0006#\uffff\uffff\u0000\u0454G\u0001"+
		"\u0000\u0000\u0000\u0455\u0456\u0005-\u0000\u0000\u0456\u0466\u0006$\uffff"+
		"\uffff\u0000\u0457\u0458\u0003@ \u0000\u0458\u045a\u0006$\uffff\uffff"+
		"\u0000\u0459\u045b\u0003\n\u0005\u0000\u045a\u0459\u0001\u0000\u0000\u0000"+
		"\u045a\u045b\u0001\u0000\u0000\u0000\u045b\u0463\u0001\u0000\u0000\u0000"+
		"\u045c\u045d\u0005\'\u0000\u0000\u045d\u045e\u0003@ \u0000\u045e\u045f"+
		"\u0006$\uffff\uffff\u0000\u045f\u0462\u0001\u0000\u0000\u0000\u0460\u0462"+
		"\u0003\n\u0005\u0000\u0461\u045c\u0001\u0000\u0000\u0000\u0461\u0460\u0001"+
		"\u0000\u0000\u0000\u0462\u0465\u0001\u0000\u0000\u0000\u0463\u0461\u0001"+
		"\u0000\u0000\u0000\u0463\u0464\u0001\u0000\u0000\u0000\u0464\u0467\u0001"+
		"\u0000\u0000\u0000\u0465\u0463\u0001\u0000\u0000\u0000\u0466\u0457\u0001"+
		"\u0000\u0000\u0000\u0466\u0467\u0001\u0000\u0000\u0000\u0467\u0468\u0001"+
		"\u0000\u0000\u0000\u0468\u0469\u0005.\u0000\u0000\u0469\u046a\u0006$\uffff"+
		"\uffff\u0000\u046aI\u0001\u0000\u0000\u0000\u046b\u046c\u0005>\u0000\u0000"+
		"\u046c\u046d\u0006%\uffff\uffff\u0000\u046d\u046e\u0005R\u0000\u0000\u046e"+
		"\u046f\u0006%\uffff\uffff\u0000\u046f\u0471\u0001\u0000\u0000\u0000\u0470"+
		"\u0472\u0003\n\u0005\u0000\u0471\u0470\u0001\u0000\u0000\u0000\u0471\u0472"+
		"\u0001\u0000\u0000\u0000\u0472\u0476\u0001\u0000\u0000\u0000\u0473\u0474"+
		"\u0005\'\u0000\u0000\u0474\u0475\u0005R\u0000\u0000\u0475\u0477\u0006"+
		"%\uffff\uffff\u0000\u0476\u0473\u0001\u0000\u0000\u0000\u0477\u0478\u0001"+
		"\u0000\u0000\u0000\u0478\u0476\u0001\u0000\u0000\u0000\u0478\u0479\u0001"+
		"\u0000\u0000\u0000\u0479\u047a\u0001\u0000\u0000\u0000\u047a\u047b\u0005"+
		"@\u0000\u0000\u047b\u047c\u0006%\uffff\uffff\u0000\u047cK\u0001\u0000"+
		"\u0000\u0000\u047d\u047e\u0005>\u0000\u0000\u047e\u047f\u0006&\uffff\uffff"+
		"\u0000\u047f\u0480\u0005S\u0000\u0000\u0480\u0481\u0006&\uffff\uffff\u0000"+
		"\u0481\u0483\u0001\u0000\u0000\u0000\u0482\u0484\u0003\n\u0005\u0000\u0483"+
		"\u0482\u0001\u0000\u0000\u0000\u0483\u0484\u0001\u0000\u0000\u0000\u0484"+
		"\u0488\u0001\u0000\u0000\u0000\u0485\u0486\u0005\'\u0000\u0000\u0486\u0487"+
		"\u0005S\u0000\u0000\u0487\u0489\u0006&\uffff\uffff\u0000\u0488\u0485\u0001"+
		"\u0000\u0000\u0000\u0489\u048a\u0001\u0000\u0000\u0000\u048a\u0488\u0001"+
		"\u0000\u0000\u0000\u048a\u048b\u0001\u0000\u0000\u0000\u048b\u048c\u0001"+
		"\u0000\u0000\u0000\u048c\u048d\u0005@\u0000\u0000\u048d\u048e\u0006&\uffff"+
		"\uffff\u0000\u048eM\u0001\u0000\u0000\u0000\u048f\u0490\u0005>\u0000\u0000"+
		"\u0490\u0491\u0006\'\uffff\uffff\u0000\u0491\u0492\u0005N\u0000\u0000"+
		"\u0492\u0493\u0006\'\uffff\uffff\u0000\u0493\u0495\u0001\u0000\u0000\u0000"+
		"\u0494\u0496\u0003\n\u0005\u0000\u0495\u0494\u0001\u0000\u0000\u0000\u0495"+
		"\u0496\u0001\u0000\u0000\u0000\u0496\u049a\u0001\u0000\u0000\u0000\u0497"+
		"\u0498\u0005\'\u0000\u0000\u0498\u0499\u0005N\u0000\u0000\u0499\u049b"+
		"\u0006\'\uffff\uffff\u0000\u049a\u0497\u0001\u0000\u0000\u0000\u049b\u049c"+
		"\u0001\u0000\u0000\u0000\u049c\u049a\u0001\u0000\u0000\u0000\u049c\u049d"+
		"\u0001\u0000\u0000\u0000\u049d\u049e\u0001\u0000\u0000\u0000\u049e\u049f"+
		"\u0005@\u0000\u0000\u049f\u04a0\u0006\'\uffff\uffff\u0000\u04a0O\u0001"+
		"\u0000\u0000\u0000\u04a1\u04a2\u0005>\u0000\u0000\u04a2\u04a3\u0006(\uffff"+
		"\uffff\u0000\u04a3\u04a4\u0005O\u0000\u0000\u04a4\u04a5\u0006(\uffff\uffff"+
		"\u0000\u04a5\u04a7\u0001\u0000\u0000\u0000\u04a6\u04a8\u0003\n\u0005\u0000"+
		"\u04a7\u04a6\u0001\u0000\u0000\u0000\u04a7\u04a8\u0001\u0000\u0000\u0000"+
		"\u04a8\u04ac\u0001\u0000\u0000\u0000\u04a9\u04aa\u0005\'\u0000\u0000\u04aa"+
		"\u04ab\u0005O\u0000\u0000\u04ab\u04ad\u0006(\uffff\uffff\u0000\u04ac\u04a9"+
		"\u0001\u0000\u0000\u0000\u04ad\u04ae\u0001\u0000\u0000\u0000\u04ae\u04ac"+
		"\u0001\u0000\u0000\u0000\u04ae\u04af\u0001\u0000\u0000\u0000\u04af\u04b0"+
		"\u0001\u0000\u0000\u0000\u04b0\u04b1\u0005@\u0000\u0000\u04b1\u04b2\u0006"+
		"(\uffff\uffff\u0000\u04b2Q\u0001\u0000\u0000\u0000\u04b3\u04b4\u0005>"+
		"\u0000\u0000\u04b4\u04b5\u0006)\uffff\uffff\u0000\u04b5\u04b6\u0003\u0004"+
		"\u0002\u0000\u04b6\u04b7\u0006)\uffff\uffff\u0000\u04b7\u04b9\u0001\u0000"+
		"\u0000\u0000\u04b8\u04ba\u0003\n\u0005\u0000\u04b9\u04b8\u0001\u0000\u0000"+
		"\u0000\u04b9\u04ba\u0001\u0000\u0000\u0000\u04ba\u04bf\u0001\u0000\u0000"+
		"\u0000\u04bb\u04bc\u0005\'\u0000\u0000\u04bc\u04bd\u0003\u0004\u0002\u0000"+
		"\u04bd\u04be\u0006)\uffff\uffff\u0000\u04be\u04c0\u0001\u0000\u0000\u0000"+
		"\u04bf\u04bb\u0001\u0000\u0000\u0000\u04c0\u04c1\u0001\u0000\u0000\u0000"+
		"\u04c1\u04bf\u0001\u0000\u0000\u0000\u04c1\u04c2\u0001\u0000\u0000\u0000"+
		"\u04c2\u04c3\u0001\u0000\u0000\u0000\u04c3\u04c4\u0005@\u0000\u0000\u04c4"+
		"\u04c5\u0006)\uffff\uffff\u0000\u04c5S\u0001\u0000\u0000\u0000fYbfmvz"+
		"\u0080\u00aa\u00ac\u00cc\u00d6\u00dc\u00de\u00e7\u00ef\u00f7\u00fc\u0102"+
		"\u0104\u010b\u0115\u0117\u011a\u0121\u0129\u0131\u0146\u0149\u014b\u0153"+
		"\u0155\u0162\u016b\u016f\u0178\u017c\u017e\u0182\u018a\u0192\u019e\u01a7"+
		"\u01b0\u01b7\u01b9\u01bc\u01cc\u01d9\u01e4\u01ec\u01fb\u020d\u022f\u0240"+
		"\u024a\u0250\u0256\u025e\u0291\u029d\u029f\u02ab\u02bb\u02d0\u02d4\u02dc"+
		"\u02de\u02f0\u0361\u0368\u036a\u036d\u0376\u037a\u037c\u0387\u03ff\u0406"+
		"\u0408\u040b\u0414\u0418\u041a\u043b\u0444\u044b\u044d\u0450\u045a\u0461"+
		"\u0463\u0466\u0471\u0478\u0483\u048a\u0495\u049c\u04a7\u04ae\u04b9\u04c1";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}