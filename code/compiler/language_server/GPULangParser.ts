// Generated from antlr4/grammar/GPULang.g4 by ANTLR 4.13.2
// noinspection ES6UnusedImports,JSUnusedGlobalSymbols,JSUnusedLocalSymbols

import {
	ATN,
	ATNDeserializer, DecisionState, DFA, FailedPredicateException,
	RecognitionException, NoViableAltException, BailErrorStrategy,
	Parser, ParserATNSimulator,
	RuleContext, ParserRuleContext, PredictionMode, PredictionContextCache,
	TerminalNode, RuleNode,
	Token, TokenStream,
	Interval, IntervalSet
} from 'antlr4';
import GPULangListener from "./GPULangListener.js";
// for running tests with parameters, TODO: discuss strategy for typed parameters in CI
// eslint-disable-next-line no-unused-vars
type int = number;



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
#include "ast/expressions/callexpression.h"
#include "ast/expressions/commaexpression.h"
#include "ast/expressions/expression.h"
#include "ast/expressions/floatexpression.h"
#include "ast/expressions/initializerexpression.h"
#include "ast/expressions/arrayinitializerexpression.h"
#include "ast/expressions/intexpression.h"
#include "ast/expressions/stringexpression.h"
#include "ast/expressions/symbolexpression.h"
#include "ast/expressions/ternaryexpression.h"
#include "ast/expressions/uintexpression.h"
#include "ast/expressions/unaryexpression.h"
#include "util.h"
#include "memory.h"

using namespace GPULang;


export default class GPULangParser extends Parser {
	public static readonly T__0 = 1;
	public static readonly T__1 = 2;
	public static readonly T__2 = 3;
	public static readonly T__3 = 4;
	public static readonly T__4 = 5;
	public static readonly T__5 = 6;
	public static readonly T__6 = 7;
	public static readonly T__7 = 8;
	public static readonly T__8 = 9;
	public static readonly T__9 = 10;
	public static readonly T__10 = 11;
	public static readonly T__11 = 12;
	public static readonly T__12 = 13;
	public static readonly T__13 = 14;
	public static readonly T__14 = 15;
	public static readonly T__15 = 16;
	public static readonly T__16 = 17;
	public static readonly T__17 = 18;
	public static readonly T__18 = 19;
	public static readonly T__19 = 20;
	public static readonly T__20 = 21;
	public static readonly T__21 = 22;
	public static readonly T__22 = 23;
	public static readonly T__23 = 24;
	public static readonly T__24 = 25;
	public static readonly T__25 = 26;
	public static readonly T__26 = 27;
	public static readonly T__27 = 28;
	public static readonly T__28 = 29;
	public static readonly T__29 = 30;
	public static readonly T__30 = 31;
	public static readonly T__31 = 32;
	public static readonly T__32 = 33;
	public static readonly T__33 = 34;
	public static readonly T__34 = 35;
	public static readonly T__35 = 36;
	public static readonly T__36 = 37;
	public static readonly SC = 38;
	public static readonly CO = 39;
	public static readonly COL = 40;
	public static readonly LP = 41;
	public static readonly RP = 42;
	public static readonly LB = 43;
	public static readonly RB = 44;
	public static readonly LL = 45;
	public static readonly RR = 46;
	public static readonly DOT = 47;
	public static readonly NOT = 48;
	public static readonly EQ = 49;
	public static readonly QO = 50;
	public static readonly QU = 51;
	public static readonly AND = 52;
	public static readonly ANDSET = 53;
	public static readonly OR = 54;
	public static readonly ORSET = 55;
	public static readonly XOR = 56;
	public static readonly XORSET = 57;
	public static readonly CONJUGATE = 58;
	public static readonly Q = 59;
	public static readonly NU = 60;
	public static readonly FORWARDSLASH = 61;
	public static readonly LESS = 62;
	public static readonly LESSEQ = 63;
	public static readonly GREATER = 64;
	public static readonly GREATEREQ = 65;
	public static readonly LOGICEQ = 66;
	public static readonly NOTEQ = 67;
	public static readonly LOGICAND = 68;
	public static readonly LOGICOR = 69;
	public static readonly MOD = 70;
	public static readonly UNDERSC = 71;
	public static readonly SOBAKA = 72;
	public static readonly ADD_OP = 73;
	public static readonly SUB_OP = 74;
	public static readonly DIV_OP = 75;
	public static readonly MUL_OP = 76;
	public static readonly ARROW = 77;
	public static readonly INTEGERLITERAL = 78;
	public static readonly UINTEGERLITERAL = 79;
	public static readonly COMMENT = 80;
	public static readonly ML_COMMENT = 81;
	public static readonly FLOATLITERAL = 82;
	public static readonly EXPONENT = 83;
	public static readonly DOUBLELITERAL = 84;
	public static readonly HEX = 85;
	public static readonly IDENTIFIER = 86;
	public static readonly WS = 87;
	public static override readonly EOF = Token.EOF;
	public static readonly RULE_string = 0;
	public static readonly RULE_boolean = 1;
	public static readonly RULE_entry = 2;
	public static readonly RULE_effect = 3;
	public static readonly RULE_linePreprocessorEntry = 4;
	public static readonly RULE_alias = 5;
	public static readonly RULE_annotation = 6;
	public static readonly RULE_attribute = 7;
	public static readonly RULE_typeDeclaration = 8;
	public static readonly RULE_variables = 9;
	public static readonly RULE_structureDeclaration = 10;
	public static readonly RULE_structure = 11;
	public static readonly RULE_enumeration = 12;
	public static readonly RULE_parameter = 13;
	public static readonly RULE_functionDeclaration = 14;
	public static readonly RULE_function = 15;
	public static readonly RULE_program = 16;
	public static readonly RULE_sampler = 17;
	public static readonly RULE_state = 18;
	public static readonly RULE_statement = 19;
	public static readonly RULE_expressionStatement = 20;
	public static readonly RULE_ifStatement = 21;
	public static readonly RULE_forStatement = 22;
	public static readonly RULE_forRangeStatement = 23;
	public static readonly RULE_whileStatement = 24;
	public static readonly RULE_scopeStatement = 25;
	public static readonly RULE_terminateStatement = 26;
	public static readonly RULE_continueStatement = 27;
	public static readonly RULE_switchStatement = 28;
	public static readonly RULE_breakStatement = 29;
	public static readonly RULE_expression = 30;
	public static readonly RULE_commaExpression = 31;
	public static readonly RULE_assignmentExpression = 32;
	public static readonly RULE_logicalOrExpression = 33;
	public static readonly RULE_logicalAndExpression = 34;
	public static readonly RULE_orExpression = 35;
	public static readonly RULE_xorExpression = 36;
	public static readonly RULE_andExpression = 37;
	public static readonly RULE_equivalencyExpression = 38;
	public static readonly RULE_relationalExpression = 39;
	public static readonly RULE_shiftExpression = 40;
	public static readonly RULE_addSubtractExpression = 41;
	public static readonly RULE_multiplyDivideExpression = 42;
	public static readonly RULE_suffixExpression = 43;
	public static readonly RULE_prefixExpression = 44;
	public static readonly RULE_namespaceExpression = 45;
	public static readonly RULE_binaryexpatom = 46;
	public static readonly RULE_initializerExpression = 47;
	public static readonly RULE_arrayInitializerExpression = 48;
	public static readonly literalNames: (string | null)[] = [ null, "'true'", 
                                                            "'false'", "'#line'", 
                                                            "'alias'", "'as'", 
                                                            "'struct'", 
                                                            "'enum'", "'program'", 
                                                            "'sampler_state'", 
                                                            "'render_state'", 
                                                            "'if'", "'else'", 
                                                            "'for'", "'..'", 
                                                            "'while'", "'do'", 
                                                            "'return'", 
                                                            "'discard'", 
                                                            "'ray_ignore'", 
                                                            "'ray_terminate'", 
                                                            "'continue'", 
                                                            "'switch'", 
                                                            "'case'", "'default'", 
                                                            "'break'", "'+='", 
                                                            "'-='", "'*='", 
                                                            "'/='", "'%='", 
                                                            "'<<='", "'>>='", 
                                                            "'<<'", "'>>'", 
                                                            "'++'", "'--'", 
                                                            "'::'", "';'", 
                                                            "','", "':'", 
                                                            "'('", "')'", 
                                                            "'{'", "'}'", 
                                                            "'['", "']'", 
                                                            "'.'", "'!'", 
                                                            "'='", "'\"'", 
                                                            "'?'", "'&'", 
                                                            "'&='", "'|'", 
                                                            "'|='", "'^'", 
                                                            "'^='", "'~'", 
                                                            "'''", "'#'", 
                                                            "'\\'", "'<'", 
                                                            "'<='", "'>'", 
                                                            "'>='", "'=='", 
                                                            "'!='", "'&&'", 
                                                            "'||'", "'%'", 
                                                            "'_'", "'@'", 
                                                            "'+'", "'-'", 
                                                            "'/'", "'*'", 
                                                            "'->'" ];
	public static readonly symbolicNames: (string | null)[] = [ null, null, 
                                                             null, null, 
                                                             null, null, 
                                                             null, null, 
                                                             null, null, 
                                                             null, null, 
                                                             null, null, 
                                                             null, null, 
                                                             null, null, 
                                                             null, null, 
                                                             null, null, 
                                                             null, null, 
                                                             null, null, 
                                                             null, null, 
                                                             null, null, 
                                                             null, null, 
                                                             null, null, 
                                                             null, null, 
                                                             null, null, 
                                                             "SC", "CO", 
                                                             "COL", "LP", 
                                                             "RP", "LB", 
                                                             "RB", "LL", 
                                                             "RR", "DOT", 
                                                             "NOT", "EQ", 
                                                             "QO", "QU", 
                                                             "AND", "ANDSET", 
                                                             "OR", "ORSET", 
                                                             "XOR", "XORSET", 
                                                             "CONJUGATE", 
                                                             "Q", "NU", 
                                                             "FORWARDSLASH", 
                                                             "LESS", "LESSEQ", 
                                                             "GREATER", 
                                                             "GREATEREQ", 
                                                             "LOGICEQ", 
                                                             "NOTEQ", "LOGICAND", 
                                                             "LOGICOR", 
                                                             "MOD", "UNDERSC", 
                                                             "SOBAKA", "ADD_OP", 
                                                             "SUB_OP", "DIV_OP", 
                                                             "MUL_OP", "ARROW", 
                                                             "INTEGERLITERAL", 
                                                             "UINTEGERLITERAL", 
                                                             "COMMENT", 
                                                             "ML_COMMENT", 
                                                             "FLOATLITERAL", 
                                                             "EXPONENT", 
                                                             "DOUBLELITERAL", 
                                                             "HEX", "IDENTIFIER", 
                                                             "WS" ];
	// tslint:disable:no-trailing-whitespace
	public static readonly ruleNames: string[] = [
		"string", "boolean", "entry", "effect", "linePreprocessorEntry", "alias", 
		"annotation", "attribute", "typeDeclaration", "variables", "structureDeclaration", 
		"structure", "enumeration", "parameter", "functionDeclaration", "function", 
		"program", "sampler", "state", "statement", "expressionStatement", "ifStatement", 
		"forStatement", "forRangeStatement", "whileStatement", "scopeStatement", 
		"terminateStatement", "continueStatement", "switchStatement", "breakStatement", 
		"expression", "commaExpression", "assignmentExpression", "logicalOrExpression", 
		"logicalAndExpression", "orExpression", "xorExpression", "andExpression", 
		"equivalencyExpression", "relationalExpression", "shiftExpression", "addSubtractExpression", 
		"multiplyDivideExpression", "suffixExpression", "prefixExpression", "namespaceExpression", 
		"binaryexpatom", "initializerExpression", "arrayInitializerExpression",
	];
	public get grammarFileName(): string { return "GPULang.g4"; }
	public get literalNames(): (string | null)[] { return GPULangParser.literalNames; }
	public get symbolicNames(): (string | null)[] { return GPULangParser.symbolicNames; }
	public get ruleNames(): string[] { return GPULangParser.ruleNames; }
	public get serializedATN(): number[] { return GPULangParser._serializedATN; }

	protected createFailedPredicateException(predicate?: string, message?: string): FailedPredicateException {
		return new FailedPredicateException(this, predicate, message);
	}




	// setup function which binds the compiler state to the current AST node
	Symbol::Location
	SetupFile(bool updateLine = true)
	{
	    Symbol::Location location;
	    ::GPULangToken* token = (::GPULangToken*)_input->LT(-1);

	    // assume the previous token is the latest file
	    location.file = token->file;
	    location.line = token->line;
	    location.column = token->getCharPositionInLine();
	    return location;
	}


	constructor(input: TokenStream) {
		super(input);
		this._interp = new ParserATNSimulator(this, GPULangParser._ATN, GPULangParser.DecisionsToDFA, new PredictionContextCache());
	}
	// @RuleVersion(0)
	public string_(): StringContext {
		let localctx: StringContext = new StringContext(this, this._ctx, this.state);
		this.enterRule(localctx, 0, GPULangParser.RULE_string);
		let _la: number;
		try {
			this.state = 116;
			this._errHandler.sync(this);
			switch (this._input.LA(1)) {
			case 50:
				this.enterOuterAlt(localctx, 1);
				{
				this.state = 98;
				this.match(GPULangParser.QO);
				this.state = 103;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
				while ((((_la) & ~0x1F) === 0 && ((1 << _la) & 4294967294) !== 0) || ((((_la - 32)) & ~0x1F) === 0 && ((1 << (_la - 32)) & 4294705151) !== 0) || ((((_la - 64)) & ~0x1F) === 0 && ((1 << (_la - 64)) & 16777215) !== 0)) {
					{
					{
					this.state = 99;
					localctx._data = this._input.LT(1);
					_la = this._input.LA(1);
					if(_la<=0 || _la===50) {
					    localctx._data = this._errHandler.recoverInline(this);
					}
					else {
						this._errHandler.reportMatch(this);
					    this.consume();
					}
					 localctx.val.append((localctx._data != null ? localctx._data.text : undefined)); 
					}
					}
					this.state = 105;
					this._errHandler.sync(this);
					_la = this._input.LA(1);
				}
				this.state = 106;
				this.match(GPULangParser.QO);
				}
				break;
			case 59:
				this.enterOuterAlt(localctx, 2);
				{
				this.state = 107;
				this.match(GPULangParser.Q);
				this.state = 112;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
				while ((((_la) & ~0x1F) === 0 && ((1 << _la) & 4294967294) !== 0) || ((((_la - 32)) & ~0x1F) === 0 && ((1 << (_la - 32)) & 4160749567) !== 0) || ((((_la - 64)) & ~0x1F) === 0 && ((1 << (_la - 64)) & 16777215) !== 0)) {
					{
					{
					this.state = 108;
					localctx._data = this._input.LT(1);
					_la = this._input.LA(1);
					if(_la<=0 || _la===59) {
					    localctx._data = this._errHandler.recoverInline(this);
					}
					else {
						this._errHandler.reportMatch(this);
					    this.consume();
					}
					 localctx.val.append((localctx._data != null ? localctx._data.text : undefined)); 
					}
					}
					this.state = 114;
					this._errHandler.sync(this);
					_la = this._input.LA(1);
				}
				this.state = 115;
				this.match(GPULangParser.Q);
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public boolean_(): BooleanContext {
		let localctx: BooleanContext = new BooleanContext(this, this._ctx, this.state);
		this.enterRule(localctx, 2, GPULangParser.RULE_boolean);

		        localctx.val =  false;
		    
		try {
			this.state = 122;
			this._errHandler.sync(this);
			switch (this._input.LA(1)) {
			case 1:
				this.enterOuterAlt(localctx, 1);
				{
				this.state = 118;
				this.match(GPULangParser.T__0);
				 localctx.val =  true; 
				}
				break;
			case 2:
				this.enterOuterAlt(localctx, 2);
				{
				this.state = 120;
				this.match(GPULangParser.T__1);
				 localctx.val =  false; 
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public entry(): EntryContext {
		let localctx: EntryContext = new EntryContext(this, this._ctx, this.state);
		this.enterRule(localctx, 4, GPULangParser.RULE_entry);
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 124;
			localctx._effect = this.effect();

			        localctx.returnEffect =  localctx._effect.eff;
			    
			this.state = 126;
			this.match(GPULangParser.EOF);
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public effect(): EffectContext {
		let localctx: EffectContext = new EffectContext(this, this._ctx, this.state);
		this.enterRule(localctx, 6, GPULangParser.RULE_effect);

		        localctx.eff =  Alloc<Effect>();
		    
		try {
			let _alt: number;
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 166;
			this._errHandler.sync(this);
			_alt = this._interp.adaptivePredict(this._input, 5, this._ctx);
			while (_alt !== 1 && _alt !== ATN.INVALID_ALT_NUMBER) {
				if (_alt === 1 + 1) {
					{
					this.state = 164;
					this._errHandler.sync(this);
					switch ( this._interp.adaptivePredict(this._input, 4, this._ctx) ) {
					case 1:
						{
						this.state = 128;
						this.linePreprocessorEntry();
						}
						break;
					case 2:
						{
						this.state = 129;
						localctx._alias = this.alias();
						this.state = 130;
						this.match(GPULangParser.SC);
						 localctx.eff->symbols.push_back(localctx._alias.sym); 
						}
						break;
					case 3:
						{
						this.state = 133;
						localctx._functionDeclaration = this.functionDeclaration();
						this.state = 134;
						this.match(GPULangParser.SC);
						 localctx.eff->symbols.push_back(localctx._functionDeclaration.sym); 
						}
						break;
					case 4:
						{
						this.state = 137;
						localctx._function_ = this.function_();
						 localctx.eff->symbols.push_back(localctx._function.sym); 
						}
						break;
					case 5:
						{
						this.state = 140;
						localctx._variables = this.variables();
						this.state = 141;
						this.match(GPULangParser.SC);
						 for (Variable* var : localctx._variables.list) { localctx.eff->symbols.push_back(var); } 
						}
						break;
					case 6:
						{
						this.state = 144;
						localctx._structure = this.structure();
						this.state = 145;
						this.match(GPULangParser.SC);
						 localctx.eff->symbols.push_back(localctx._structure.sym); 
						}
						break;
					case 7:
						{
						this.state = 148;
						localctx._enumeration = this.enumeration();
						this.state = 149;
						this.match(GPULangParser.SC);
						 localctx.eff->symbols.push_back(localctx._enumeration.sym); 
						}
						break;
					case 8:
						{
						this.state = 152;
						localctx._state_ = this.state_();
						this.state = 153;
						this.match(GPULangParser.SC);
						 localctx.eff->symbols.push_back(localctx._state.sym); 
						}
						break;
					case 9:
						{
						this.state = 156;
						localctx._sampler = this.sampler();
						this.state = 157;
						this.match(GPULangParser.SC);
						 localctx.eff->symbols.push_back(localctx._sampler.sym); 
						}
						break;
					case 10:
						{
						this.state = 160;
						localctx._program = this.program();
						this.state = 161;
						this.match(GPULangParser.SC);
						 localctx.eff->symbols.push_back(localctx._program.sym); 
						}
						break;
					}
					}
				}
				this.state = 168;
				this._errHandler.sync(this);
				_alt = this._interp.adaptivePredict(this._input, 5, this._ctx);
			}
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public linePreprocessorEntry(): LinePreprocessorEntryContext {
		let localctx: LinePreprocessorEntryContext = new LinePreprocessorEntryContext(this, this._ctx, this.state);
		this.enterRule(localctx, 8, GPULangParser.RULE_linePreprocessorEntry);

		        size_t origLine;
		    
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 169;
			this.match(GPULangParser.T__2);
			 origLine = _input->LT(-1)->getLine(); 
			this.state = 171;
			localctx._line = this.match(GPULangParser.INTEGERLITERAL);
			this.state = 172;
			localctx._path = this.string_();
			 LineStack.push_back( {origLine, atoi((localctx._line != null ? localctx._line.text : undefined).c_str()), localctx._path.val }); 
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public alias(): AliasContext {
		let localctx: AliasContext = new AliasContext(this, this._ctx, this.state);
		this.enterRule(localctx, 10, GPULangParser.RULE_alias);

		        localctx.sym =  nullptr;
		        std::string name;
		        std::string type;
		    
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 175;
			this.match(GPULangParser.T__3);
			this.state = 176;
			localctx._name = this.match(GPULangParser.IDENTIFIER);
			this.state = 177;
			this.match(GPULangParser.T__4);
			this.state = 178;
			localctx._type_ = this.match(GPULangParser.IDENTIFIER);
			 name = (localctx._name != null ? localctx._name.text : undefined); type = (localctx._type != null ? localctx._type.text : undefined); 

			        localctx.sym =  Alloc<Alias>();
			        localctx.sym->name = name;
			        localctx.sym->type = type;
			    
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public annotation(): AnnotationContext {
		let localctx: AnnotationContext = new AnnotationContext(this, this._ctx, this.state);
		this.enterRule(localctx, 12, GPULangParser.RULE_annotation);
		try {
			this.enterOuterAlt(localctx, 1);
			{
			{
			this.state = 182;
			this.match(GPULangParser.SOBAKA);
			{
			this.state = 183;
			localctx._name = this.match(GPULangParser.IDENTIFIER);
			this.state = 184;
			this.match(GPULangParser.LP);
			this.state = 185;
			localctx._value = this.expression();
			this.state = 186;
			this.match(GPULangParser.RP);
			 localctx.annot.name = (localctx._name != null ? localctx._name.text : undefined); localctx.annot.value = localctx._value.tree; 
			}
			}
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public attribute(): AttributeContext {
		let localctx: AttributeContext = new AttributeContext(this, this._ctx, this.state);
		this.enterRule(localctx, 14, GPULangParser.RULE_attribute);
		try {
			this.state = 197;
			this._errHandler.sync(this);
			switch ( this._interp.adaptivePredict(this._input, 6, this._ctx) ) {
			case 1:
				this.enterOuterAlt(localctx, 1);
				{
				this.state = 189;
				localctx._name = this.match(GPULangParser.IDENTIFIER);
				this.state = 190;
				this.match(GPULangParser.LP);
				this.state = 191;
				localctx._expression = this.expression();
				this.state = 192;
				this.match(GPULangParser.RP);
				 localctx.attr.name = (localctx._name != null ? localctx._name.text : undefined); localctx.attr.expression = localctx._expression.tree; 
				}
				break;
			case 2:
				this.enterOuterAlt(localctx, 2);
				{
				this.state = 195;
				localctx._name = this.match(GPULangParser.IDENTIFIER);
				 localctx.attr.name = (localctx._name != null ? localctx._name.text : undefined); localctx.attr.expression = nullptr; 
				}
				break;
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public typeDeclaration(): TypeDeclarationContext {
		let localctx: TypeDeclarationContext = new TypeDeclarationContext(this, this._ctx, this.state);
		this.enterRule(localctx, 16, GPULangParser.RULE_typeDeclaration);

		        localctx.type_.name = "";
		    
		let _la: number;
		try {
			let _alt: number;
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 213;
			this._errHandler.sync(this);
			_alt = this._interp.adaptivePredict(this._input, 9, this._ctx);
			while (_alt !== 2 && _alt !== ATN.INVALID_ALT_NUMBER) {
				if (_alt === 1) {
					{
					this.state = 211;
					this._errHandler.sync(this);
					switch (this._input.LA(1)) {
					case 76:
						{
						this.state = 199;
						this.match(GPULangParser.MUL_OP);
						 localctx.type_.AddModifier(Type::FullType::Modifier::Pointer); 
						}
						break;
					case 45:
						{
						this.state = 201;
						this.match(GPULangParser.LL);
						 localctx.type_.AddModifier(Type::FullType::Modifier::Array); 
						this.state = 206;
						this._errHandler.sync(this);
						_la = this._input.LA(1);
						if (_la===1 || _la===2 || ((((_la - 35)) & ~0x1F) === 0 && ((1 << (_la - 35)) & 25207875) !== 0) || ((((_la - 73)) & ~0x1F) === 0 && ((1 << (_la - 73)) & 14955) !== 0)) {
							{
							this.state = 203;
							localctx._arraySize0 = this.expression();
							 localctx.type_.UpdateValue(localctx._arraySize0.tree); 
							}
						}

						this.state = 208;
						this.match(GPULangParser.RR);
						}
						break;
					case 86:
						{
						this.state = 209;
						localctx._IDENTIFIER = this.match(GPULangParser.IDENTIFIER);
						 localctx.type_.AddQualifier((localctx._IDENTIFIER != null ? localctx._IDENTIFIER.text : undefined)); 
						}
						break;
					default:
						throw new NoViableAltException(this);
					}
					}
				}
				this.state = 215;
				this._errHandler.sync(this);
				_alt = this._interp.adaptivePredict(this._input, 9, this._ctx);
			}
			this.state = 216;
			localctx._typeName = this.match(GPULangParser.IDENTIFIER);
			 localctx.type_.name = (localctx._typeName != null ? localctx._typeName.text : undefined); 
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public variables(): VariablesContext {
		let localctx: VariablesContext = new VariablesContext(this, this._ctx, this.state);
		this.enterRule(localctx, 18, GPULangParser.RULE_variables);

		        std::vector<Annotation> annotations;
		        std::vector<Attribute> attributes;
		        std::vector<std::string> names;
		        std::vector<Expression*> valueExpressions;
		        std::vector<Symbol::Location> locations;
		        unsigned initCounter = 0;
		        Type::FullType type = { "<undefined>" };
		    
		let _la: number;
		try {
			let _alt: number;
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 224;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la===72) {
				{
				{
				this.state = 219;
				localctx._annotation = this.annotation();
				 annotations.push_back(std::move(localctx._annotation.annot)); 
				}
				}
				this.state = 226;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 230;
			this._errHandler.sync(this);
			_alt = 1;
			do {
				switch (_alt) {
				case 1:
					{
					{
					this.state = 227;
					localctx._attribute = this.attribute();
					 attributes.push_back(std::move(localctx._attribute.attr)); 
					}
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				this.state = 232;
				this._errHandler.sync(this);
				_alt = this._interp.adaptivePredict(this._input, 11, this._ctx);
			} while (_alt !== 2 && _alt !== ATN.INVALID_ALT_NUMBER);
			this.state = 234;
			localctx._varName = this.match(GPULangParser.IDENTIFIER);
			 names.push_back((localctx._varName != null ? localctx._varName.text : undefined)); valueExpressions.push_back(nullptr); locations.push_back(SetupFile()); 
			this.state = 241;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la===39) {
				{
				{
				this.state = 236;
				this.match(GPULangParser.CO);
				this.state = 237;
				localctx._varNameN = this.match(GPULangParser.IDENTIFIER);
				 names.push_back((localctx._varNameN != null ? localctx._varNameN.text : undefined)); valueExpressions.push_back(nullptr); locations.push_back(SetupFile()); 
				}
				}
				this.state = 243;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 248;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la===40) {
				{
				this.state = 244;
				this.match(GPULangParser.COL);
				this.state = 245;
				localctx._typeDeclaration = this.typeDeclaration();
				 type = localctx._typeDeclaration.type_; 
				}
			}

			this.state = 262;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la===49) {
				{
				this.state = 250;
				this.match(GPULangParser.EQ);
				this.state = 251;
				localctx._valueExpr = this.logicalOrExpression();
				 if (initCounter < names.size()) { valueExpressions[initCounter++] = localctx._valueExpr.tree; }  
				this.state = 259;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
				while (_la===39) {
					{
					{
					this.state = 253;
					this.match(GPULangParser.CO);
					this.state = 254;
					localctx._valueExprN = this.logicalOrExpression();
					 if (initCounter < names.size()) { valueExpressions[initCounter++] = localctx._valueExprN.tree; }; 
					}
					}
					this.state = 261;
					this._errHandler.sync(this);
					_la = this._input.LA(1);
				}
				}
			}


			        for (size_t i = 0; i < names.size(); i++)
			        {
			            Variable* var = Alloc<Variable>(); 
			            var->type = type; 
			            var->location = locations[i]; 
			            var->annotations = annotations;
			            var->attributes = attributes;
			            var->name = names[i];
			            var->valueExpression = valueExpressions[i];
			            localctx.list.push_back(var);
			        }
			    
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public structureDeclaration(): StructureDeclarationContext {
		let localctx: StructureDeclarationContext = new StructureDeclarationContext(this, this._ctx, this.state);
		this.enterRule(localctx, 20, GPULangParser.RULE_structureDeclaration);

		        localctx.sym =  nullptr;
		        std::vector<Annotation> annotations;
		        std::vector<Attribute> attributes;
		    
		let _la: number;
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 271;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la===72) {
				{
				{
				this.state = 266;
				localctx._annotation = this.annotation();
				 annotations.push_back(std::move(localctx._annotation.annot)); 
				}
				}
				this.state = 273;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 279;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la===86) {
				{
				{
				this.state = 274;
				localctx._attribute = this.attribute();
				 attributes.push_back(std::move(localctx._attribute.attr)); 
				}
				}
				this.state = 281;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 282;
			this.match(GPULangParser.T__5);
			this.state = 283;
			localctx._name = this.match(GPULangParser.IDENTIFIER);
			 
			        localctx.sym =  Alloc<Structure>();
			        localctx.sym->name = (localctx._name != null ? localctx._name.text : undefined); 
			        localctx.sym->annotations = std::move(annotations);
			        localctx.sym->attributes = std::move(attributes);
			        localctx.sym->location = SetupFile();
			    
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public structure(): StructureContext {
		let localctx: StructureContext = new StructureContext(this, this._ctx, this.state);
		this.enterRule(localctx, 22, GPULangParser.RULE_structure);

		        localctx.sym =  nullptr;
		        std::vector<Symbol*> members;
		        bool isArray = false;
		        Expression* arraySizeExpression = nullptr;
		        std::string instanceName;
		        Symbol::Location varLocation;
		        Type::FullType varType;
		        std::string varName;
		    
		let _la: number;
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 286;
			localctx._structureDeclaration = this.structureDeclaration();
			 localctx.sym =  localctx._structureDeclaration.sym; 
			this.state = 288;
			this.match(GPULangParser.LB);
			this.state = 313;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la===86) {
				{
				{
				this.state = 289;
				localctx._varName = this.match(GPULangParser.IDENTIFIER);
				 varName = (localctx._varName != null ? localctx._varName.text : undefined); varLocation = SetupFile(); 
				this.state = 291;
				this.match(GPULangParser.COL);
				this.state = 304;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
				while (_la===45 || _la===76) {
					{
					this.state = 302;
					this._errHandler.sync(this);
					switch (this._input.LA(1)) {
					case 76:
						{
						this.state = 292;
						this.match(GPULangParser.MUL_OP);
						 varType.AddModifier(Type::FullType::Modifier::Pointer); 
						}
						break;
					case 45:
						{
						this.state = 294;
						this.match(GPULangParser.LL);
						 varType.AddModifier(Type::FullType::Modifier::Array); 
						this.state = 299;
						this._errHandler.sync(this);
						_la = this._input.LA(1);
						if (_la===1 || _la===2 || ((((_la - 35)) & ~0x1F) === 0 && ((1 << (_la - 35)) & 25207875) !== 0) || ((((_la - 73)) & ~0x1F) === 0 && ((1 << (_la - 73)) & 14955) !== 0)) {
							{
							this.state = 296;
							localctx._arraySize0 = this.expression();
							 varType.UpdateValue(localctx._arraySize0.tree); 
							}
						}

						this.state = 301;
						this.match(GPULangParser.RR);
						}
						break;
					default:
						throw new NoViableAltException(this);
					}
					}
					this.state = 306;
					this._errHandler.sync(this);
					_la = this._input.LA(1);
				}
				this.state = 307;
				localctx._varTypeName = this.match(GPULangParser.IDENTIFIER);
				 varType.name = (localctx._varTypeName != null ? localctx._varTypeName.text : undefined); 
				this.state = 309;
				this.match(GPULangParser.SC);

				                Variable* var = Alloc<Variable>(); 
				                var->type = varType; 
				                var->location = varLocation; 
				                var->name = varName;
				                var->valueExpression = nullptr;
				                members.push_back(var);
				                
				                varType = Type::FullType();
				            
				}
				}
				this.state = 315;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 316;
			this.match(GPULangParser.RB);
			 
			        localctx.sym->symbols = members; 
			        //localctx.sym->instanceName = instanceName;
			        //localctx.sym->isArray = isArray;
			        //localctx.sym->arraySizeExpression = arraySizeExpression;
			    
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public enumeration(): EnumerationContext {
		let localctx: EnumerationContext = new EnumerationContext(this, this._ctx, this.state);
		this.enterRule(localctx, 24, GPULangParser.RULE_enumeration);

		        localctx.sym =  nullptr;
		        std::vector<std::string> enumLabels;
		        std::vector<Expression*> enumValues;
		        std::string name;
		        Type::FullType type = { "u32" };
		        Symbol::Location location;
		    
		let _la: number;
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 319;
			this.match(GPULangParser.T__6);
			this.state = 320;
			localctx._name = this.match(GPULangParser.IDENTIFIER);
			 name = (localctx._name != null ? localctx._name.text : undefined); location = SetupFile(); 
			this.state = 326;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la===40) {
				{
				this.state = 322;
				this.match(GPULangParser.COL);
				this.state = 323;
				localctx._typeDeclaration = this.typeDeclaration();
				 type = localctx._typeDeclaration.type_; 
				}
			}

			this.state = 328;
			this.match(GPULangParser.LB);
			this.state = 353;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la===86) {
				{
				this.state = 329;
				localctx._label = this.match(GPULangParser.IDENTIFIER);
				 Expression* expr = nullptr; 
				this.state = 335;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
				if (_la===49) {
					{
					this.state = 331;
					this.match(GPULangParser.EQ);
					this.state = 332;
					localctx._value = this.expression();
					 expr = localctx._value.tree; 
					}
				}


				                enumLabels.push_back((localctx._label != null ? localctx._label.text : undefined));
				                enumValues.push_back(expr);
				            
				this.state = 350;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
				while (_la===39) {
					{
					{
					this.state = 338;
					this.match(GPULangParser.CO);
					this.state = 339;
					localctx._label = this.match(GPULangParser.IDENTIFIER);
					 Expression* expr = nullptr; 
					this.state = 345;
					this._errHandler.sync(this);
					_la = this._input.LA(1);
					if (_la===49) {
						{
						this.state = 341;
						this.match(GPULangParser.EQ);
						this.state = 342;
						localctx._value = this.expression();
						 expr = localctx._value.tree; 
						}
					}


					                    enumLabels.push_back((localctx._label != null ? localctx._label.text : undefined));
					                    enumValues.push_back(expr);
					                
					}
					}
					this.state = 352;
					this._errHandler.sync(this);
					_la = this._input.LA(1);
				}
				}
			}

			this.state = 355;
			this.match(GPULangParser.RB);

			        localctx.sym =  Alloc<Enumeration>();
			        localctx.sym->name = name;
			        type.literal = true;
			        localctx.sym->type = type;
			        localctx.sym->labels = enumLabels;
			        localctx.sym->values = enumValues;
			        localctx.sym->location = location;
			    
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public parameter(): ParameterContext {
		let localctx: ParameterContext = new ParameterContext(this, this._ctx, this.state);
		this.enterRule(localctx, 26, GPULangParser.RULE_parameter);

		        std::vector<Attribute> attributes;
		        std::string name;
		        Expression* valueExpression = nullptr;
		        Symbol::Location location;
		        Type::FullType type = { "unknown" };
		    
		let _la: number;
		try {
			let _alt: number;
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 363;
			this._errHandler.sync(this);
			_alt = this._interp.adaptivePredict(this._input, 27, this._ctx);
			while (_alt !== 2 && _alt !== ATN.INVALID_ALT_NUMBER) {
				if (_alt === 1) {
					{
					{
					this.state = 358;
					localctx._attribute = this.attribute();
					 attributes.push_back(std::move(localctx._attribute.attr)); 
					}
					}
				}
				this.state = 365;
				this._errHandler.sync(this);
				_alt = this._interp.adaptivePredict(this._input, 27, this._ctx);
			}
			this.state = 366;
			localctx._varName = this.match(GPULangParser.IDENTIFIER);
			 name = (localctx._varName != null ? localctx._varName.text : undefined); location = SetupFile(); 
			this.state = 368;
			this.match(GPULangParser.COL);
			this.state = 369;
			localctx._typeDeclaration = this.typeDeclaration();
			 type = localctx._typeDeclaration.type_; 
			this.state = 375;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la===49) {
				{
				this.state = 371;
				this.match(GPULangParser.EQ);
				this.state = 372;
				localctx._valueExpr = this.logicalOrExpression();
				 valueExpression = localctx._valueExpr.tree; 
				}
			}


			            localctx.sym =  Alloc<Variable>(); 
			            localctx.sym->type = type; 
			            localctx.sym->location = location; 
			            localctx.sym->attributes = std::move(attributes);
			            localctx.sym->name = name;
			            localctx.sym->valueExpression = valueExpression;
			    
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public functionDeclaration(): FunctionDeclarationContext {
		let localctx: FunctionDeclarationContext = new FunctionDeclarationContext(this, this._ctx, this.state);
		this.enterRule(localctx, 28, GPULangParser.RULE_functionDeclaration);

		        localctx.sym =  nullptr;
		        std::vector<Variable*> variables;
		        std::vector<Attribute> attributes;
		        Symbol::Location location;
		    
		let _la: number;
		try {
			let _alt: number;
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 384;
			this._errHandler.sync(this);
			_alt = this._interp.adaptivePredict(this._input, 29, this._ctx);
			while (_alt !== 2 && _alt !== ATN.INVALID_ALT_NUMBER) {
				if (_alt === 1) {
					{
					{
					this.state = 379;
					localctx._attribute = this.attribute();
					 attributes.push_back(std::move(localctx._attribute.attr)); 
					}
					}
				}
				this.state = 386;
				this._errHandler.sync(this);
				_alt = this._interp.adaptivePredict(this._input, 29, this._ctx);
			}
			this.state = 387;
			localctx._name = this.match(GPULangParser.IDENTIFIER);
			 location = SetupFile(); 
			this.state = 389;
			this.match(GPULangParser.LP);
			this.state = 401;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la===86) {
				{
				this.state = 390;
				localctx._arg0 = this.parameter();
				 variables.push_back(localctx._arg0.sym); 
				this.state = 398;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
				while (_la===39) {
					{
					{
					this.state = 392;
					this.match(GPULangParser.CO);
					this.state = 393;
					localctx._argn = this.parameter();
					 variables.push_back(localctx._argn.sym); 
					}
					}
					this.state = 400;
					this._errHandler.sync(this);
					_la = this._input.LA(1);
				}
				}
			}

			this.state = 403;
			this.match(GPULangParser.RP);
			this.state = 404;
			localctx._returnType = this.typeDeclaration();

			        localctx.sym =  Alloc<Function>(); 
			        localctx.sym->hasBody = false;
			        localctx.sym->location = location;
			        localctx.sym->returnType = localctx._returnType.type_; 
			        localctx.sym->name = (localctx._name != null ? localctx._name.text : undefined); 
			        localctx.sym->parameters = variables; 
			        localctx.sym->attributes = std::move(attributes);
			    
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public function_(): FunctionContext {
		let localctx: FunctionContext = new FunctionContext(this, this._ctx, this.state);
		this.enterRule(localctx, 30, GPULangParser.RULE_function);

		        localctx.sym =  nullptr;
		        Token* startToken = nullptr;
		        Token* endToken = nullptr;
		    
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 407;
			localctx._functionDeclaration = this.functionDeclaration();
			 localctx.sym =  localctx._functionDeclaration.sym; 

			        startToken = _input->LT(2);
			    
			this.state = 410;
			localctx._scopeStatement = this.scopeStatement();

			        endToken = _input->LT(-2);

			        // extract code from between tokens
			        antlr4::misc::Interval interval;
			        interval.a = startToken->getTokenIndex();
			        interval.b = endToken->getTokenIndex();
			        localctx.sym->body = _input->getText(interval);
			        localctx.sym->hasBody = true;
			        localctx.sym->ast = localctx._scopeStatement.tree;
			    
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public program(): ProgramContext {
		let localctx: ProgramContext = new ProgramContext(this, this._ctx, this.state);
		this.enterRule(localctx, 32, GPULangParser.RULE_program);

		        localctx.sym =  nullptr;
		        Symbol::Location location;
		        std::vector<Program::SubroutineMapping> subroutines;
		        std::vector<Expression*> entries;
		        std::vector<Annotation> annotations;
		    
		let _la: number;
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 418;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la===72) {
				{
				{
				this.state = 413;
				localctx._annotation = this.annotation();
				 annotations.push_back(std::move(localctx._annotation.annot)); 
				}
				}
				this.state = 420;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 421;
			this.match(GPULangParser.T__7);
			this.state = 422;
			localctx._name = this.match(GPULangParser.IDENTIFIER);
			 location = SetupFile(); 
			this.state = 424;
			this.match(GPULangParser.LB);
			this.state = 431;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la===1 || _la===2 || ((((_la - 35)) & ~0x1F) === 0 && ((1 << (_la - 35)) & 25207875) !== 0) || ((((_la - 73)) & ~0x1F) === 0 && ((1 << (_la - 73)) & 14955) !== 0)) {
				{
				{
				this.state = 425;
				localctx._assignment = this.expression();
				 entries.push_back(localctx._assignment.tree); 
				this.state = 427;
				this.match(GPULangParser.SC);
				}
				}
				this.state = 433;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 434;
			this.match(GPULangParser.RB);
			 
			        localctx.sym =  Alloc<Program>();
			        localctx.sym->location = location;
			        localctx.sym->name = (localctx._name != null ? localctx._name.text : undefined);
			        localctx.sym->annotations = std::move(annotations);
			        localctx.sym->entries = entries;
			    
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public sampler(): SamplerContext {
		let localctx: SamplerContext = new SamplerContext(this, this._ctx, this.state);
		this.enterRule(localctx, 34, GPULangParser.RULE_sampler);

		        std::vector<Expression*> entries;
		    
		let _la: number;
		try {
			this.enterOuterAlt(localctx, 1);
			{
			{
			this.state = 437;
			this.match(GPULangParser.T__8);
			 localctx.sym =  Alloc<SamplerState>(); localctx.sym->isImmutable = true; 
			}
			this.state = 440;
			localctx._name = this.match(GPULangParser.IDENTIFIER);
			 localctx.sym->location = SetupFile(); 
			this.state = 442;
			this.match(GPULangParser.LB);
			this.state = 449;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la===1 || _la===2 || ((((_la - 35)) & ~0x1F) === 0 && ((1 << (_la - 35)) & 25207875) !== 0) || ((((_la - 73)) & ~0x1F) === 0 && ((1 << (_la - 73)) & 14955) !== 0)) {
				{
				{
				this.state = 443;
				localctx._assign = this.expression();
				 entries.push_back(localctx._assign.tree); 
				this.state = 445;
				this.match(GPULangParser.SC);
				}
				}
				this.state = 451;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 452;
			this.match(GPULangParser.RB);

			        localctx.sym->name = (localctx._name != null ? localctx._name.text : undefined);
			        localctx.sym->entries = entries;
			    
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public state_(): StateContext {
		let localctx: StateContext = new StateContext(this, this._ctx, this.state);
		this.enterRule(localctx, 36, GPULangParser.RULE_state);

		        std::vector<Expression*> entries;
		    
		let _la: number;
		try {
			this.enterOuterAlt(localctx, 1);
			{
			{
			this.state = 455;
			this.match(GPULangParser.T__9);
			 localctx.sym =  Alloc<RenderState>(); 
			}
			this.state = 458;
			localctx._name = this.match(GPULangParser.IDENTIFIER);
			 localctx.sym->location = SetupFile(); 
			this.state = 460;
			this.match(GPULangParser.LB);
			this.state = 467;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la===1 || _la===2 || ((((_la - 35)) & ~0x1F) === 0 && ((1 << (_la - 35)) & 25207875) !== 0) || ((((_la - 73)) & ~0x1F) === 0 && ((1 << (_la - 73)) & 14955) !== 0)) {
				{
				{
				this.state = 461;
				localctx._assign = this.expression();
				 entries.push_back(localctx._assign.tree); 
				this.state = 463;
				this.match(GPULangParser.SC);
				}
				}
				this.state = 469;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 470;
			this.match(GPULangParser.RB);

			        localctx.sym->name = (localctx._name != null ? localctx._name.text : undefined);
			        localctx.sym->entries = entries;
			    
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public statement(): StatementContext {
		let localctx: StatementContext = new StatementContext(this, this._ctx, this.state);
		this.enterRule(localctx, 38, GPULangParser.RULE_statement);

		        localctx.tree =  nullptr;
		    
		try {
			this.state = 501;
			this._errHandler.sync(this);
			switch (this._input.LA(1)) {
			case 11:
				this.enterOuterAlt(localctx, 1);
				{
				this.state = 473;
				localctx._ifStatement = this.ifStatement();
				 localctx.tree =  localctx._ifStatement.tree; 
				}
				break;
			case 43:
				this.enterOuterAlt(localctx, 2);
				{
				this.state = 476;
				localctx._scopeStatement = this.scopeStatement();
				 localctx.tree =  localctx._scopeStatement.tree; 
				}
				break;
			case 13:
				this.enterOuterAlt(localctx, 3);
				{
				this.state = 479;
				localctx._forStatement = this.forStatement();
				 localctx.tree =  localctx._forStatement.tree; 
				}
				break;
			case 15:
			case 16:
				this.enterOuterAlt(localctx, 4);
				{
				this.state = 482;
				localctx._whileStatement = this.whileStatement();
				 localctx.tree =  localctx._whileStatement.tree; 
				}
				break;
			case 22:
				this.enterOuterAlt(localctx, 5);
				{
				this.state = 485;
				localctx._switchStatement = this.switchStatement();
				 localctx.tree =  localctx._switchStatement.tree; 
				}
				break;
			case 17:
			case 18:
			case 19:
			case 20:
				this.enterOuterAlt(localctx, 6);
				{
				this.state = 488;
				localctx._terminateStatement = this.terminateStatement();
				 localctx.tree =  localctx._terminateStatement.tree; 
				}
				break;
			case 21:
				this.enterOuterAlt(localctx, 7);
				{
				this.state = 491;
				localctx._continueStatement = this.continueStatement();
				 localctx.tree =  localctx._continueStatement.tree; 
				}
				break;
			case 25:
				this.enterOuterAlt(localctx, 8);
				{
				this.state = 494;
				localctx._breakStatement = this.breakStatement();
				 localctx.tree =  localctx._breakStatement.tree; 
				}
				break;
			case 1:
			case 2:
			case 35:
			case 36:
			case 41:
			case 45:
			case 48:
			case 50:
			case 58:
			case 59:
			case 73:
			case 74:
			case 76:
			case 78:
			case 79:
			case 82:
			case 84:
			case 85:
			case 86:
				this.enterOuterAlt(localctx, 9);
				{
				this.state = 497;
				localctx._expressionStatement = this.expressionStatement();
				this.state = 498;
				this.match(GPULangParser.SC);
				 localctx.tree =  localctx._expressionStatement.tree; 
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public expressionStatement(): ExpressionStatementContext {
		let localctx: ExpressionStatementContext = new ExpressionStatementContext(this, this._ctx, this.state);
		this.enterRule(localctx, 40, GPULangParser.RULE_expressionStatement);

		        localctx.tree =  nullptr;
		    
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 503;
			localctx._expression = this.expression();

			        localctx.tree =  Alloc<ExpressionStatement>((localctx._expression.tree));
			        localctx.tree->location = SetupFile();
			    
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public ifStatement(): IfStatementContext {
		let localctx: IfStatementContext = new IfStatementContext(this, this._ctx, this.state);
		this.enterRule(localctx, 42, GPULangParser.RULE_ifStatement);

		        localctx.tree =  nullptr;
		        Expression* condition = nullptr;
		        Statement* ifBody = nullptr;
		        Statement* elseBody = nullptr;
		        Symbol::Location location;
		    
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 506;
			this.match(GPULangParser.T__10);
			 location = SetupFile(); 
			this.state = 508;
			this.match(GPULangParser.LP);
			this.state = 509;
			localctx._condition = this.expression();
			 condition = localctx._condition.tree; 
			this.state = 511;
			this.match(GPULangParser.RP);
			this.state = 512;
			localctx._ifBody = this.statement();
			 ifBody = localctx._ifBody.tree; 
			this.state = 518;
			this._errHandler.sync(this);
			switch ( this._interp.adaptivePredict(this._input, 37, this._ctx) ) {
			case 1:
				{
				this.state = 514;
				this.match(GPULangParser.T__11);
				this.state = 515;
				localctx._elseBody = this.statement();
				 elseBody = localctx._elseBody.tree; 
				}
				break;
			}

			        localctx.tree =  Alloc<IfStatement>(condition, ifBody, elseBody);
			        localctx.tree->location = location;
			    
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public forStatement(): ForStatementContext {
		let localctx: ForStatementContext = new ForStatementContext(this, this._ctx, this.state);
		this.enterRule(localctx, 44, GPULangParser.RULE_forStatement);

		        localctx.tree =  nullptr;
		        std::vector<Variable*> declarations;
		        Expression* conditionExpression = nullptr;
		        Expression* loopExpression = nullptr;
		        Statement* contents = nullptr;
		        std::vector<Attribute> attributes;
		        Symbol::Location location;
		    
		let _la: number;
		try {
			let _alt: number;
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 522;
			this.match(GPULangParser.T__12);
			 location = SetupFile(); 
			this.state = 524;
			this.match(GPULangParser.LP);
			this.state = 528;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la===72 || _la===86) {
				{
				this.state = 525;
				localctx._variables = this.variables();
				 declarations = localctx._variables.list; 
				}
			}

			this.state = 530;
			this.match(GPULangParser.SC);
			this.state = 534;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la===1 || _la===2 || ((((_la - 35)) & ~0x1F) === 0 && ((1 << (_la - 35)) & 25207875) !== 0) || ((((_la - 73)) & ~0x1F) === 0 && ((1 << (_la - 73)) & 14955) !== 0)) {
				{
				this.state = 531;
				localctx._condition = this.expression();
				 conditionExpression = localctx._condition.tree; 
				}
			}

			this.state = 536;
			this.match(GPULangParser.SC);
			this.state = 540;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la===1 || _la===2 || ((((_la - 35)) & ~0x1F) === 0 && ((1 << (_la - 35)) & 25207875) !== 0) || ((((_la - 73)) & ~0x1F) === 0 && ((1 << (_la - 73)) & 14955) !== 0)) {
				{
				this.state = 537;
				localctx._loop = this.expression();
				 loopExpression = localctx._loop.tree; 
				}
			}

			this.state = 542;
			this.match(GPULangParser.RP);
			this.state = 548;
			this._errHandler.sync(this);
			_alt = this._interp.adaptivePredict(this._input, 41, this._ctx);
			while (_alt !== 2 && _alt !== ATN.INVALID_ALT_NUMBER) {
				if (_alt === 1) {
					{
					{
					this.state = 543;
					localctx._attribute = this.attribute();
					 attributes.push_back(std::move(localctx._attribute.attr)); 
					}
					}
				}
				this.state = 550;
				this._errHandler.sync(this);
				_alt = this._interp.adaptivePredict(this._input, 41, this._ctx);
			}
			this.state = 551;
			localctx._content = this.statement();
			 contents = localctx._content.tree; 

			        localctx.tree =  Alloc<ForStatement>(declarations, conditionExpression, loopExpression, contents);
			        localctx.tree->location = location;
			        localctx.tree->attributes = std::move(attributes);
			    
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public forRangeStatement(): ForRangeStatementContext {
		let localctx: ForRangeStatementContext = new ForRangeStatementContext(this, this._ctx, this.state);
		this.enterRule(localctx, 46, GPULangParser.RULE_forRangeStatement);

		        localctx.tree =  nullptr;
		        Statement* contents = nullptr;
		        Symbol::Location location;
		    
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 555;
			this.match(GPULangParser.T__12);
			 location = SetupFile(); 
			this.state = 557;
			this.match(GPULangParser.LP);
			this.state = 558;
			localctx._iterator = this.match(GPULangParser.IDENTIFIER);
			this.state = 559;
			this.match(GPULangParser.COL);
			this.state = 560;
			localctx._start = this.match(GPULangParser.IDENTIFIER);
			this.state = 561;
			this.match(GPULangParser.T__13);
			this.state = 562;
			localctx._end = this.match(GPULangParser.IDENTIFIER);
			this.state = 563;
			this.match(GPULangParser.RP);
			this.state = 564;
			localctx._content = this.statement();
			 contents = localctx._content.tree; 


			    
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public whileStatement(): WhileStatementContext {
		let localctx: WhileStatementContext = new WhileStatementContext(this, this._ctx, this.state);
		this.enterRule(localctx, 48, GPULangParser.RULE_whileStatement);

		        localctx.tree =  nullptr;
		        Expression* conditionExpression = nullptr;
		        Statement* contents = nullptr;
		        bool isDoWhile = false;
		        Symbol::Location location;
		    
		try {
			this.state = 590;
			this._errHandler.sync(this);
			switch (this._input.LA(1)) {
			case 15:
				this.enterOuterAlt(localctx, 1);
				{
				this.state = 568;
				this.match(GPULangParser.T__14);
				 location = SetupFile(); 
				this.state = 570;
				this.match(GPULangParser.LP);
				this.state = 571;
				localctx._condition = this.expression();
				 conditionExpression = localctx._condition.tree; 
				this.state = 573;
				this.match(GPULangParser.RP);
				this.state = 574;
				localctx._content = this.statement();
				 contents = localctx._content.tree; 

				        localctx.tree =  Alloc<WhileStatement>(conditionExpression, contents, isDoWhile);
				        localctx.tree->location = location;
				    
				}
				break;
			case 16:
				this.enterOuterAlt(localctx, 2);
				{
				this.state = 578;
				this.match(GPULangParser.T__15);
				 location = SetupFile(); 
				this.state = 580;
				localctx._content = this.statement();
				 contents = localctx._content.tree; isDoWhile = true; 
				this.state = 582;
				this.match(GPULangParser.T__14);
				this.state = 583;
				this.match(GPULangParser.LP);
				this.state = 584;
				localctx._condition = this.expression();
				 conditionExpression = localctx._condition.tree; 
				this.state = 586;
				this.match(GPULangParser.RP);
				this.state = 587;
				this.match(GPULangParser.SC);

				        localctx.tree =  Alloc<WhileStatement>(conditionExpression, contents, isDoWhile);
				        localctx.tree->location = location;
				    
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public scopeStatement(): ScopeStatementContext {
		let localctx: ScopeStatementContext = new ScopeStatementContext(this, this._ctx, this.state);
		this.enterRule(localctx, 50, GPULangParser.RULE_scopeStatement);

		        localctx.tree =  nullptr;
		        std::vector<Symbol*> contents;
		        Symbol::Location location;
		    
		let _la: number;
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 592;
			this.match(GPULangParser.LB);
			 location = SetupFile(); 
			this.state = 604;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while ((((_la) & ~0x1F) === 0 && ((1 << _la) & 41920526) !== 0) || ((((_la - 35)) & ~0x1F) === 0 && ((1 << (_la - 35)) & 25208131) !== 0) || ((((_la - 72)) & ~0x1F) === 0 && ((1 << (_la - 72)) & 29911) !== 0)) {
				{
				this.state = 602;
				this._errHandler.sync(this);
				switch ( this._interp.adaptivePredict(this._input, 43, this._ctx) ) {
				case 1:
					{
					this.state = 594;
					localctx._variables = this.variables();
					this.state = 595;
					this.match(GPULangParser.SC);
					 for(Variable* var : localctx._variables.list) { contents.push_back(var); } 
					}
					break;
				case 2:
					{
					this.state = 598;
					localctx._statement = this.statement();
					 contents.push_back(localctx._statement.tree); 
					}
					break;
				case 3:
					{
					this.state = 601;
					this.linePreprocessorEntry();
					}
					break;
				}
				}
				this.state = 606;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 607;
			this.match(GPULangParser.RB);

			        localctx.tree =  Alloc<ScopeStatement>(contents);
			        localctx.tree->location = location;
			    
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public terminateStatement(): TerminateStatementContext {
		let localctx: TerminateStatementContext = new TerminateStatementContext(this, this._ctx, this.state);
		this.enterRule(localctx, 52, GPULangParser.RULE_terminateStatement);

		        localctx.tree =  nullptr;
		        Expression* returnValue = nullptr;
		        Symbol::Location location;
		    
		let _la: number;
		try {
			this.state = 631;
			this._errHandler.sync(this);
			switch (this._input.LA(1)) {
			case 17:
				this.enterOuterAlt(localctx, 1);
				{
				this.state = 610;
				this.match(GPULangParser.T__16);
				 location = SetupFile(); 
				this.state = 615;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
				if (_la===1 || _la===2 || ((((_la - 35)) & ~0x1F) === 0 && ((1 << (_la - 35)) & 25207875) !== 0) || ((((_la - 73)) & ~0x1F) === 0 && ((1 << (_la - 73)) & 14955) !== 0)) {
					{
					this.state = 612;
					localctx._value = this.expression();
					 returnValue = localctx._value.tree; 
					}
				}

				this.state = 617;
				this.match(GPULangParser.SC);

				        localctx.tree =  Alloc<TerminateStatement>(returnValue, TerminateStatement::TerminationType::Return);
				        localctx.tree->location = location;
				    
				}
				break;
			case 18:
				this.enterOuterAlt(localctx, 2);
				{
				this.state = 619;
				this.match(GPULangParser.T__17);
				 location = SetupFile(); 
				this.state = 621;
				this.match(GPULangParser.SC);

				      localctx.tree =  Alloc<TerminateStatement>(returnValue, TerminateStatement::TerminationType::Discard);
				      localctx.tree->location = location;
				    
				}
				break;
			case 19:
				this.enterOuterAlt(localctx, 3);
				{
				this.state = 623;
				this.match(GPULangParser.T__18);
				 location = SetupFile(); 
				this.state = 625;
				this.match(GPULangParser.SC);

				      localctx.tree =  Alloc<TerminateStatement>(returnValue, TerminateStatement::TerminationType::RayIgnoreIntersection);
				      localctx.tree->location = location;
				    
				}
				break;
			case 20:
				this.enterOuterAlt(localctx, 4);
				{
				this.state = 627;
				this.match(GPULangParser.T__19);
				 location = SetupFile(); 
				this.state = 629;
				this.match(GPULangParser.SC);

				      localctx.tree =  Alloc<TerminateStatement>(returnValue, TerminateStatement::TerminationType::RayTerminate);
				      localctx.tree->location = location;
				    
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public continueStatement(): ContinueStatementContext {
		let localctx: ContinueStatementContext = new ContinueStatementContext(this, this._ctx, this.state);
		this.enterRule(localctx, 54, GPULangParser.RULE_continueStatement);

		        localctx.tree =  nullptr;
		        Symbol::Location location;
		    
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 633;
			this.match(GPULangParser.T__20);
			 location = SetupFile(); 
			this.state = 635;
			this.match(GPULangParser.SC);

			        localctx.tree =  Alloc<ContinueStatement>();
			        localctx.tree->location = location;
			    
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public switchStatement(): SwitchStatementContext {
		let localctx: SwitchStatementContext = new SwitchStatementContext(this, this._ctx, this.state);
		this.enterRule(localctx, 56, GPULangParser.RULE_switchStatement);

		        localctx.tree =  nullptr;
		        Expression* switchExpression;
		        std::vector<Expression*> caseExpressions;
		        std::vector<Statement*> caseStatements;
		        Symbol::Location location;
		        Statement* defaultStatement = nullptr;
		    
		let _la: number;
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 638;
			this.match(GPULangParser.T__21);
			 location = SetupFile(); 
			this.state = 640;
			this.match(GPULangParser.LP);
			this.state = 641;
			localctx._expression = this.expression();
			this.state = 642;
			this.match(GPULangParser.RP);
			 switchExpression = localctx._expression.tree; 
			this.state = 644;
			this.match(GPULangParser.LB);
			this.state = 656;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la===23) {
				{
				{
				this.state = 645;
				this.match(GPULangParser.T__22);
				this.state = 646;
				localctx._expression = this.expression();
				this.state = 647;
				this.match(GPULangParser.COL);
				 caseExpressions.push_back(localctx._expression.tree); caseStatements.push_back(nullptr); 
				this.state = 652;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
				if ((((_la) & ~0x1F) === 0 && ((1 << _la) & 41920518) !== 0) || ((((_la - 35)) & ~0x1F) === 0 && ((1 << (_la - 35)) & 25208131) !== 0) || ((((_la - 73)) & ~0x1F) === 0 && ((1 << (_la - 73)) & 14955) !== 0)) {
					{
					this.state = 649;
					localctx._statement = this.statement();
					 
					                    caseStatements.back() = localctx._statement.tree;
					                
					}
				}

				}
				}
				this.state = 658;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 666;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la===24) {
				{
				this.state = 659;
				this.match(GPULangParser.T__23);
				this.state = 660;
				this.match(GPULangParser.COL);
				this.state = 664;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
				if ((((_la) & ~0x1F) === 0 && ((1 << _la) & 41920518) !== 0) || ((((_la - 35)) & ~0x1F) === 0 && ((1 << (_la - 35)) & 25208131) !== 0) || ((((_la - 73)) & ~0x1F) === 0 && ((1 << (_la - 73)) & 14955) !== 0)) {
					{
					this.state = 661;
					localctx._statement = this.statement();

					                    defaultStatement = localctx._statement.tree;
					                
					}
				}

				}
			}

			this.state = 668;
			this.match(GPULangParser.RB);

			        localctx.tree =  Alloc<SwitchStatement>(switchExpression, caseExpressions, caseStatements, defaultStatement);
			        localctx.tree->location = location;
			    
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public breakStatement(): BreakStatementContext {
		let localctx: BreakStatementContext = new BreakStatementContext(this, this._ctx, this.state);
		this.enterRule(localctx, 58, GPULangParser.RULE_breakStatement);

		        localctx.tree =  nullptr;
		        Symbol::Location location;
		    
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 671;
			this.match(GPULangParser.T__24);
			 location = SetupFile(); 
			this.state = 673;
			this.match(GPULangParser.SC);

			        localctx.tree =  Alloc<BreakStatement>();
			        localctx.tree->location = location;
			    
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public expression(): ExpressionContext {
		let localctx: ExpressionContext = new ExpressionContext(this, this._ctx, this.state);
		this.enterRule(localctx, 60, GPULangParser.RULE_expression);

		        localctx.tree =  nullptr;
		    
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 676;
			localctx._commaExpression = this.commaExpression();
			 localctx.tree =  localctx._commaExpression.tree; localctx.tree->location = SetupFile(); 
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public commaExpression(): CommaExpressionContext {
		let localctx: CommaExpressionContext = new CommaExpressionContext(this, this._ctx, this.state);
		this.enterRule(localctx, 62, GPULangParser.RULE_commaExpression);

		        localctx.tree =  nullptr;
		        Symbol::Location location;
		    
		try {
			let _alt: number;
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 679;
			localctx._e1 = this.assignmentExpression();
			 localctx.tree =  localctx._e1.tree; 
			this.state = 688;
			this._errHandler.sync(this);
			_alt = this._interp.adaptivePredict(this._input, 51, this._ctx);
			while (_alt !== 2 && _alt !== ATN.INVALID_ALT_NUMBER) {
				if (_alt === 1) {
					{
					{
					this.state = 681;
					this.match(GPULangParser.CO);
					 location = SetupFile(); 
					this.state = 683;
					localctx._e2 = this.assignmentExpression();

					            CommaExpression* expr = Alloc<CommaExpression>(localctx.tree, localctx._e2.tree);
					            expr->location = location;
					            localctx.tree =  expr;
					        
					}
					}
				}
				this.state = 690;
				this._errHandler.sync(this);
				_alt = this._interp.adaptivePredict(this._input, 51, this._ctx);
			}
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public assignmentExpression(): AssignmentExpressionContext {
		let localctx: AssignmentExpressionContext = new AssignmentExpressionContext(this, this._ctx, this.state);
		this.enterRule(localctx, 64, GPULangParser.RULE_assignmentExpression);

		        localctx.tree =  nullptr;
		        Symbol::Location location;
		    
		let _la: number;
		try {
			let _alt: number;
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 691;
			localctx._e1 = this.logicalOrExpression();
			 localctx.tree =  localctx._e1.tree; 
			this.state = 707;
			this._errHandler.sync(this);
			_alt = this._interp.adaptivePredict(this._input, 53, this._ctx);
			while (_alt !== 2 && _alt !== ATN.INVALID_ALT_NUMBER) {
				if (_alt === 1) {
					{
					this.state = 705;
					this._errHandler.sync(this);
					switch (this._input.LA(1)) {
					case 26:
					case 27:
					case 28:
					case 29:
					case 30:
					case 31:
					case 32:
					case 49:
					case 53:
					case 55:
					case 57:
						{
						this.state = 693;
						localctx._op = this._input.LT(1);
						_la = this._input.LA(1);
						if(!(((((_la - 26)) & ~0x1F) === 0 && ((1 << (_la - 26)) & 2826961023) !== 0))) {
						    localctx._op = this._errHandler.recoverInline(this);
						}
						else {
							this._errHandler.reportMatch(this);
						    this.consume();
						}
						 location = SetupFile(); 
						this.state = 695;
						localctx._e2 = this.logicalOrExpression();
						 
						            BinaryExpression* expr = Alloc<BinaryExpression>(StringToFourCC((localctx._op != null ? localctx._op.text : undefined)), localctx.tree, localctx._e2.tree);
						            expr->location = location;
						            localctx.tree =  expr;
						        
						}
						break;
					case 51:
						{
						this.state = 698;
						this.match(GPULangParser.QU);
						 location = SetupFile(); 
						this.state = 700;
						localctx._ifBody = this.expression();
						this.state = 701;
						this.match(GPULangParser.COL);
						this.state = 702;
						localctx._elseBody = this.expression();
						 
						            TernaryExpression* expr = Alloc<TernaryExpression>(localctx.tree, localctx._ifBody.tree, localctx._elseBody.tree);
						            expr->location = location;
						            localctx.tree =  expr;
						        
						}
						break;
					default:
						throw new NoViableAltException(this);
					}
					}
				}
				this.state = 709;
				this._errHandler.sync(this);
				_alt = this._interp.adaptivePredict(this._input, 53, this._ctx);
			}
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public logicalOrExpression(): LogicalOrExpressionContext {
		let localctx: LogicalOrExpressionContext = new LogicalOrExpressionContext(this, this._ctx, this.state);
		this.enterRule(localctx, 66, GPULangParser.RULE_logicalOrExpression);

		        localctx.tree =  nullptr;
		        Symbol::Location location;
		    
		let _la: number;
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 710;
			localctx._e1 = this.logicalAndExpression();
			 localctx.tree =  localctx._e1.tree; 
			this.state = 719;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la===69) {
				{
				{
				{
				this.state = 712;
				this.match(GPULangParser.LOGICOR);
				}
				 location = SetupFile(); 
				this.state = 714;
				localctx._e2 = this.logicalAndExpression();

				            BinaryExpression* expr = Alloc<BinaryExpression>('||', localctx.tree, localctx._e2.tree);
				            expr->location = location;
				            localctx.tree =  expr;
				        
				}
				}
				this.state = 721;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public logicalAndExpression(): LogicalAndExpressionContext {
		let localctx: LogicalAndExpressionContext = new LogicalAndExpressionContext(this, this._ctx, this.state);
		this.enterRule(localctx, 68, GPULangParser.RULE_logicalAndExpression);

		        localctx.tree =  nullptr;
		        Symbol::Location location;

		    
		let _la: number;
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 722;
			localctx._e1 = this.orExpression();
			 localctx.tree =  localctx._e1.tree; 
			this.state = 731;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la===68) {
				{
				{
				{
				this.state = 724;
				this.match(GPULangParser.LOGICAND);
				}
				 location = SetupFile(); 
				this.state = 726;
				localctx._e2 = this.orExpression();

				            BinaryExpression* expr = Alloc<BinaryExpression>('&&', localctx.tree, localctx._e2.tree);
				            expr->location = location;
				            localctx.tree =  expr;
				        
				}
				}
				this.state = 733;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public orExpression(): OrExpressionContext {
		let localctx: OrExpressionContext = new OrExpressionContext(this, this._ctx, this.state);
		this.enterRule(localctx, 70, GPULangParser.RULE_orExpression);

		        localctx.tree =  nullptr;
		        Symbol::Location location;

		    
		let _la: number;
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 734;
			localctx._e1 = this.xorExpression();
			 localctx.tree =  localctx._e1.tree; 
			this.state = 743;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la===54) {
				{
				{
				this.state = 736;
				this.match(GPULangParser.OR);
				 location = SetupFile(); 
				this.state = 738;
				localctx._e2 = this.xorExpression();

				            BinaryExpression* expr = Alloc<BinaryExpression>('|', localctx.tree, localctx._e2.tree);
				            expr->location = location;
				            localctx.tree =  expr;
				        
				}
				}
				this.state = 745;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public xorExpression(): XorExpressionContext {
		let localctx: XorExpressionContext = new XorExpressionContext(this, this._ctx, this.state);
		this.enterRule(localctx, 72, GPULangParser.RULE_xorExpression);

		        localctx.tree =  nullptr;
		        Symbol::Location location;
		    
		let _la: number;
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 746;
			localctx._e1 = this.andExpression();
			 localctx.tree =  localctx._e1.tree; 
			this.state = 755;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la===56) {
				{
				{
				this.state = 748;
				this.match(GPULangParser.XOR);
				 location = SetupFile(); 
				this.state = 750;
				localctx._e2 = this.andExpression();

				            BinaryExpression* expr = Alloc<BinaryExpression>('^', localctx.tree, localctx._e2.tree);
				            expr->location = location;
				            localctx.tree =  expr;
				        
				}
				}
				this.state = 757;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public andExpression(): AndExpressionContext {
		let localctx: AndExpressionContext = new AndExpressionContext(this, this._ctx, this.state);
		this.enterRule(localctx, 74, GPULangParser.RULE_andExpression);

		        localctx.tree =  nullptr;
		        Symbol::Location location;
		    
		let _la: number;
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 758;
			localctx._e1 = this.equivalencyExpression();
			 localctx.tree =  localctx._e1.tree;	
			this.state = 767;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la===52) {
				{
				{
				this.state = 760;
				this.match(GPULangParser.AND);
				 location = SetupFile(); 
				this.state = 762;
				localctx._e2 = this.equivalencyExpression();

				            BinaryExpression* expr = Alloc<BinaryExpression>('&', localctx.tree, localctx._e2.tree);
				            expr->location = location;
				            localctx.tree =  expr;
				        
				}
				}
				this.state = 769;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public equivalencyExpression(): EquivalencyExpressionContext {
		let localctx: EquivalencyExpressionContext = new EquivalencyExpressionContext(this, this._ctx, this.state);
		this.enterRule(localctx, 76, GPULangParser.RULE_equivalencyExpression);

		        localctx.tree =  nullptr;
		        Symbol::Location location;
		    
		let _la: number;
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 770;
			localctx._e1 = this.relationalExpression();
			 localctx.tree =  localctx._e1.tree; 
			this.state = 779;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la===66 || _la===67) {
				{
				{
				this.state = 772;
				localctx._op = this._input.LT(1);
				_la = this._input.LA(1);
				if(!(_la===66 || _la===67)) {
				    localctx._op = this._errHandler.recoverInline(this);
				}
				else {
					this._errHandler.reportMatch(this);
				    this.consume();
				}
				 location = SetupFile(); 
				this.state = 774;
				localctx._e2 = this.relationalExpression();

				            BinaryExpression* expr = Alloc<BinaryExpression>(StringToFourCC((localctx._op != null ? localctx._op.text : undefined)), localctx.tree, localctx._e2.tree);
				            expr->location = location;
				            localctx.tree =  expr;
				        
				}
				}
				this.state = 781;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public relationalExpression(): RelationalExpressionContext {
		let localctx: RelationalExpressionContext = new RelationalExpressionContext(this, this._ctx, this.state);
		this.enterRule(localctx, 78, GPULangParser.RULE_relationalExpression);

		        localctx.tree =  nullptr;
		        Symbol::Location location;
		    
		let _la: number;
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 782;
			localctx._e1 = this.shiftExpression();
			 localctx.tree =  localctx._e1.tree; 
			this.state = 791;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (((((_la - 62)) & ~0x1F) === 0 && ((1 << (_la - 62)) & 15) !== 0)) {
				{
				{
				this.state = 784;
				localctx._op = this._input.LT(1);
				_la = this._input.LA(1);
				if(!(((((_la - 62)) & ~0x1F) === 0 && ((1 << (_la - 62)) & 15) !== 0))) {
				    localctx._op = this._errHandler.recoverInline(this);
				}
				else {
					this._errHandler.reportMatch(this);
				    this.consume();
				}
				 location = SetupFile(); 
				this.state = 786;
				localctx._e2 = this.shiftExpression();

				            BinaryExpression* expr = Alloc<BinaryExpression>(StringToFourCC((localctx._op != null ? localctx._op.text : undefined)), localctx.tree, localctx._e2.tree);
				            expr->location = location;
				            localctx.tree =  expr;
				        
				}
				}
				this.state = 793;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public shiftExpression(): ShiftExpressionContext {
		let localctx: ShiftExpressionContext = new ShiftExpressionContext(this, this._ctx, this.state);
		this.enterRule(localctx, 80, GPULangParser.RULE_shiftExpression);

		        localctx.tree =  nullptr;
		        Symbol::Location location;
		    
		let _la: number;
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 794;
			localctx._e1 = this.addSubtractExpression();
			 localctx.tree =  localctx._e1.tree; 
			this.state = 803;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la===33 || _la===34) {
				{
				{
				this.state = 796;
				localctx._op = this._input.LT(1);
				_la = this._input.LA(1);
				if(!(_la===33 || _la===34)) {
				    localctx._op = this._errHandler.recoverInline(this);
				}
				else {
					this._errHandler.reportMatch(this);
				    this.consume();
				}
				 location = SetupFile(); 
				this.state = 798;
				localctx._e2 = this.addSubtractExpression();

				            BinaryExpression* expr = Alloc<BinaryExpression>(StringToFourCC((localctx._op != null ? localctx._op.text : undefined)), localctx.tree, localctx._e2.tree);
				            expr->location = location;
				            localctx.tree =  expr;
				        
				}
				}
				this.state = 805;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public addSubtractExpression(): AddSubtractExpressionContext {
		let localctx: AddSubtractExpressionContext = new AddSubtractExpressionContext(this, this._ctx, this.state);
		this.enterRule(localctx, 82, GPULangParser.RULE_addSubtractExpression);

		        localctx.tree =  nullptr;
		        Symbol::Location location;
		    
		let _la: number;
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 806;
			localctx._e1 = this.multiplyDivideExpression();
			 localctx.tree =  localctx._e1.tree; 
			this.state = 815;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la===73 || _la===74) {
				{
				{
				this.state = 808;
				localctx._op = this._input.LT(1);
				_la = this._input.LA(1);
				if(!(_la===73 || _la===74)) {
				    localctx._op = this._errHandler.recoverInline(this);
				}
				else {
					this._errHandler.reportMatch(this);
				    this.consume();
				}
				 location = SetupFile(); 
				this.state = 810;
				localctx._e2 = this.multiplyDivideExpression();

				            BinaryExpression* expr = Alloc<BinaryExpression>(StringToFourCC((localctx._op != null ? localctx._op.text : undefined)), localctx.tree, localctx._e2.tree);
				            expr->location = location;
				            localctx.tree =  expr;
				        
				}
				}
				this.state = 817;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public multiplyDivideExpression(): MultiplyDivideExpressionContext {
		let localctx: MultiplyDivideExpressionContext = new MultiplyDivideExpressionContext(this, this._ctx, this.state);
		this.enterRule(localctx, 84, GPULangParser.RULE_multiplyDivideExpression);

		        localctx.tree =  nullptr;
		        Symbol::Location location;
		    
		let _la: number;
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 818;
			localctx._e1 = this.suffixExpression();
			 localctx.tree =  localctx._e1.tree; 
			this.state = 827;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (((((_la - 70)) & ~0x1F) === 0 && ((1 << (_la - 70)) & 97) !== 0)) {
				{
				{
				this.state = 820;
				localctx._op = this._input.LT(1);
				_la = this._input.LA(1);
				if(!(((((_la - 70)) & ~0x1F) === 0 && ((1 << (_la - 70)) & 97) !== 0))) {
				    localctx._op = this._errHandler.recoverInline(this);
				}
				else {
					this._errHandler.reportMatch(this);
				    this.consume();
				}
				 location = SetupFile(); 
				this.state = 822;
				localctx._e2 = this.suffixExpression();

				            BinaryExpression* expr = Alloc<BinaryExpression>(StringToFourCC((localctx._op != null ? localctx._op.text : undefined)), localctx.tree, localctx._e2.tree);
				            expr->location = location;
				            localctx.tree =  expr;
				        
				}
				}
				this.state = 829;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public suffixExpression(): SuffixExpressionContext {
		let localctx: SuffixExpressionContext = new SuffixExpressionContext(this, this._ctx, this.state);
		this.enterRule(localctx, 86, GPULangParser.RULE_suffixExpression);

		        localctx.tree =  nullptr;

		        Symbol::Location location;
		        std::vector<Expression*> args;
		        Expression* arrayIndexExpr = nullptr;

		        std::vector<uint32_t> ops;
		        std::vector<Symbol::Location> locations;
		    
		let _la: number;
		try {
			let _alt: number;
			this.state = 883;
			this._errHandler.sync(this);
			switch ( this._interp.adaptivePredict(this._input, 70, this._ctx) ) {
			case 1:
				this.enterOuterAlt(localctx, 1);
				{
				this.state = 830;
				localctx._e1 = this.prefixExpression();

				        localctx.tree =  localctx._e1.tree;
				    
				this.state = 870;
				this._errHandler.sync(this);
				_alt = this._interp.adaptivePredict(this._input, 68, this._ctx);
				while (_alt !== 2 && _alt !== ATN.INVALID_ALT_NUMBER) {
					if (_alt === 1) {
						{
						this.state = 868;
						this._errHandler.sync(this);
						switch (this._input.LA(1)) {
						case 41:
							{
							this.state = 832;
							this.match(GPULangParser.LP);
							 location = SetupFile(); 
							this.state = 845;
							this._errHandler.sync(this);
							_la = this._input.LA(1);
							if (_la===1 || _la===2 || ((((_la - 35)) & ~0x1F) === 0 && ((1 << (_la - 35)) & 25207875) !== 0) || ((((_la - 73)) & ~0x1F) === 0 && ((1 << (_la - 73)) & 14955) !== 0)) {
								{
								this.state = 834;
								localctx._arg0 = this.logicalOrExpression();
								 args.push_back(localctx._arg0.tree); 
								this.state = 842;
								this._errHandler.sync(this);
								_la = this._input.LA(1);
								while (_la===39) {
									{
									{
									this.state = 836;
									this.match(GPULangParser.CO);
									this.state = 837;
									localctx._argn = this.logicalOrExpression();
									 args.push_back(localctx._argn.tree); 
									}
									}
									this.state = 844;
									this._errHandler.sync(this);
									_la = this._input.LA(1);
								}
								}
							}

							this.state = 847;
							this.match(GPULangParser.RP);

							            CallExpression* expr = Alloc<CallExpression>(localctx.tree, args);
							            expr->location = location;
							            localctx.tree =  expr;
							        
							}
							break;
						case 47:
							{
							this.state = 849;
							this.match(GPULangParser.DOT);
							 location = SetupFile(); 
							this.state = 851;
							localctx._e2 = this.suffixExpression();

							            AccessExpression* expr = Alloc<AccessExpression>(localctx.tree, localctx._e2.tree, false);
							            expr->location = location;
							            localctx.tree =  expr;
							        
							}
							break;
						case 77:
							{
							this.state = 854;
							this.match(GPULangParser.ARROW);
							 location = SetupFile(); 
							this.state = 856;
							localctx._e2 = this.suffixExpression();

							            AccessExpression* expr = Alloc<AccessExpression>(localctx.tree, localctx._e2.tree, true);
							            expr->location = location;
							            localctx.tree =  expr;
							        
							}
							break;
						case 45:
							{
							this.state = 859;
							this.match(GPULangParser.LL);
							 location = SetupFile(); 
							this.state = 864;
							this._errHandler.sync(this);
							_la = this._input.LA(1);
							if (_la===1 || _la===2 || ((((_la - 35)) & ~0x1F) === 0 && ((1 << (_la - 35)) & 25207875) !== 0) || ((((_la - 73)) & ~0x1F) === 0 && ((1 << (_la - 73)) & 14955) !== 0)) {
								{
								this.state = 861;
								localctx._e3 = this.expression();
								 arrayIndexExpr = localctx._e3.tree; 
								}
							}

							this.state = 866;
							this.match(GPULangParser.RR);

							            ArrayIndexExpression* expr = Alloc<ArrayIndexExpression>(localctx.tree, arrayIndexExpr);
							            expr->location = location;
							            localctx.tree =  expr;
							        
							}
							break;
						default:
							throw new NoViableAltException(this);
						}
						}
					}
					this.state = 872;
					this._errHandler.sync(this);
					_alt = this._interp.adaptivePredict(this._input, 68, this._ctx);
				}
				}
				break;
			case 2:
				this.enterOuterAlt(localctx, 2);
				{
				this.state = 873;
				localctx._e1 = this.prefixExpression();
				this.state = 878;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
				while (_la===35 || _la===36) {
					{
					{
					this.state = 874;
					localctx._op = this._input.LT(1);
					_la = this._input.LA(1);
					if(!(_la===35 || _la===36)) {
					    localctx._op = this._errHandler.recoverInline(this);
					}
					else {
						this._errHandler.reportMatch(this);
					    this.consume();
					}
					 ops.push_back(StringToFourCC((localctx._op != null ? localctx._op.text : undefined))); locations.push_back(SetupFile()); 
					}
					}
					this.state = 880;
					this._errHandler.sync(this);
					_la = this._input.LA(1);
				}

				        localctx.tree =  localctx._e1.tree;
				        localctx.tree->location = SetupFile();
				        for (size_t i = 0; i < ops.size(); i++)
				        {
				            localctx.tree =  Alloc<UnaryExpression>(ops[i], false, localctx.tree);
				            localctx.tree->location = locations[i];
				        }
				    
				}
				break;
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public prefixExpression(): PrefixExpressionContext {
		let localctx: PrefixExpressionContext = new PrefixExpressionContext(this, this._ctx, this.state);
		this.enterRule(localctx, 88, GPULangParser.RULE_prefixExpression);

		        localctx.tree =  nullptr;
		        std::vector<uint32_t> ops;
		        std::vector<Symbol::Location> locations;
		    
		let _la: number;
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 889;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (((((_la - 35)) & ~0x1F) === 0 && ((1 << (_la - 35)) & 8396803) !== 0) || ((((_la - 73)) & ~0x1F) === 0 && ((1 << (_la - 73)) & 11) !== 0)) {
				{
				{
				this.state = 885;
				localctx._op = this._input.LT(1);
				_la = this._input.LA(1);
				if(!(((((_la - 35)) & ~0x1F) === 0 && ((1 << (_la - 35)) & 8396803) !== 0) || ((((_la - 73)) & ~0x1F) === 0 && ((1 << (_la - 73)) & 11) !== 0))) {
				    localctx._op = this._errHandler.recoverInline(this);
				}
				else {
					this._errHandler.reportMatch(this);
				    this.consume();
				}
				 ops.push_back(StringToFourCC((localctx._op != null ? localctx._op.text : undefined))); locations.push_back(SetupFile()); 
				}
				}
				this.state = 891;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 892;
			localctx._e1 = this.binaryexpatom();

			        localctx.tree =  localctx._e1.tree;
			        localctx.tree->location = SetupFile();
			        for (size_t i = 0; i < ops.size(); i++)
			        {
			            localctx.tree =  Alloc<UnaryExpression>(ops[i], true, localctx.tree);
			            localctx.tree->location = locations[i];
			        }
			    
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public namespaceExpression(): NamespaceExpressionContext {
		let localctx: NamespaceExpressionContext = new NamespaceExpressionContext(this, this._ctx, this.state);
		this.enterRule(localctx, 90, GPULangParser.RULE_namespaceExpression);

		        localctx.tree =  nullptr;
		    
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 895;
			localctx._e1 = this.binaryexpatom();
			this.state = 896;
			this.match(GPULangParser.T__36);
			this.state = 897;
			localctx._e2 = this.binaryexpatom();


			    
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public binaryexpatom(): BinaryexpatomContext {
		let localctx: BinaryexpatomContext = new BinaryexpatomContext(this, this._ctx, this.state);
		this.enterRule(localctx, 92, GPULangParser.RULE_binaryexpatom);

		        localctx.tree =  nullptr;
		    
		try {
			this.state = 929;
			this._errHandler.sync(this);
			switch ( this._interp.adaptivePredict(this._input, 72, this._ctx) ) {
			case 1:
				this.enterOuterAlt(localctx, 1);
				{
				this.state = 900;
				localctx._initializerExpression = this.initializerExpression();
				 localctx.tree =  localctx._initializerExpression.tree; 
				}
				break;
			case 2:
				this.enterOuterAlt(localctx, 2);
				{
				this.state = 903;
				localctx._arrayInitializerExpression = this.arrayInitializerExpression();
				 localctx.tree =  localctx._arrayInitializerExpression.tree; 
				}
				break;
			case 3:
				this.enterOuterAlt(localctx, 3);
				{
				this.state = 906;
				this.match(GPULangParser.LP);
				this.state = 907;
				localctx._expression = this.expression();
				this.state = 908;
				this.match(GPULangParser.RP);
				 localctx.tree =  localctx._expression.tree; 
				}
				break;
			case 4:
				this.enterOuterAlt(localctx, 4);
				{
				this.state = 911;
				localctx._INTEGERLITERAL = this.match(GPULangParser.INTEGERLITERAL);
				 localctx.tree =  Alloc<IntExpression>(atoi((localctx._INTEGERLITERAL != null ? localctx._INTEGERLITERAL.text : undefined).c_str())); localctx.tree->location = SetupFile(); 
				}
				break;
			case 5:
				this.enterOuterAlt(localctx, 5);
				{
				this.state = 913;
				localctx._UINTEGERLITERAL = this.match(GPULangParser.UINTEGERLITERAL);
				 localctx.tree =  Alloc<UIntExpression>(strtoul((localctx._UINTEGERLITERAL != null ? localctx._UINTEGERLITERAL.text : undefined).c_str(), nullptr, 10)); localctx.tree->location = SetupFile(); 
				}
				break;
			case 6:
				this.enterOuterAlt(localctx, 6);
				{
				this.state = 915;
				localctx._FLOATLITERAL = this.match(GPULangParser.FLOATLITERAL);
				 localctx.tree =  Alloc<FloatExpression>(atof((localctx._FLOATLITERAL != null ? localctx._FLOATLITERAL.text : undefined).c_str())); localctx.tree->location = SetupFile(); 
				}
				break;
			case 7:
				this.enterOuterAlt(localctx, 7);
				{
				this.state = 917;
				localctx._DOUBLELITERAL = this.match(GPULangParser.DOUBLELITERAL);
				 localctx.tree =  Alloc<FloatExpression>(atof((localctx._DOUBLELITERAL != null ? localctx._DOUBLELITERAL.text : undefined).c_str())); localctx.tree->location = SetupFile(); 
				}
				break;
			case 8:
				this.enterOuterAlt(localctx, 8);
				{
				this.state = 919;
				localctx._HEX = this.match(GPULangParser.HEX);
				 localctx.tree =  Alloc<UIntExpression>(strtoul((localctx._HEX != null ? localctx._HEX.text : undefined).c_str(), nullptr, 16)); localctx.tree->location = SetupFile(); 
				}
				break;
			case 9:
				this.enterOuterAlt(localctx, 9);
				{
				this.state = 921;
				localctx._string_ = this.string_();
				 localctx.tree =  Alloc<StringExpression>(localctx._string.val); localctx.tree->location = SetupFile(); 
				}
				break;
			case 10:
				this.enterOuterAlt(localctx, 10);
				{
				this.state = 924;
				localctx._IDENTIFIER = this.match(GPULangParser.IDENTIFIER);
				 localctx.tree =  Alloc<SymbolExpression>((localctx._IDENTIFIER != null ? localctx._IDENTIFIER.text : undefined)); localctx.tree->location = SetupFile(); 
				}
				break;
			case 11:
				this.enterOuterAlt(localctx, 11);
				{
				this.state = 926;
				localctx._boolean_ = this.boolean_();
				 localctx.tree =  Alloc<BoolExpression>(localctx._boolean.val); localctx.tree->location = SetupFile(); 
				}
				break;
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public initializerExpression(): InitializerExpressionContext {
		let localctx: InitializerExpressionContext = new InitializerExpressionContext(this, this._ctx, this.state);
		this.enterRule(localctx, 94, GPULangParser.RULE_initializerExpression);

		        localctx.tree =  nullptr;
		        std::vector<Expression*> exprs;
		        std::string type = "";
		        Symbol::Location location;
		    
		let _la: number;
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 931;
			localctx._type_ = this.match(GPULangParser.IDENTIFIER);
			 type = (localctx._type != null ? localctx._type.text : undefined); 
			this.state = 933;
			this.match(GPULangParser.LB);
			 location = SetupFile(); 
			this.state = 946;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la===1 || _la===2 || ((((_la - 35)) & ~0x1F) === 0 && ((1 << (_la - 35)) & 25207875) !== 0) || ((((_la - 73)) & ~0x1F) === 0 && ((1 << (_la - 73)) & 14955) !== 0)) {
				{
				this.state = 935;
				localctx._arg0 = this.logicalOrExpression();
				 exprs.push_back(localctx._arg0.tree); 
				this.state = 943;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
				while (_la===39) {
					{
					{
					this.state = 937;
					this.match(GPULangParser.CO);
					this.state = 938;
					localctx._argN = this.logicalOrExpression();
					 exprs.push_back(localctx._argN.tree); 
					}
					}
					this.state = 945;
					this._errHandler.sync(this);
					_la = this._input.LA(1);
				}
				}
			}

			this.state = 948;
			this.match(GPULangParser.RB);

			        localctx.tree =  Alloc<InitializerExpression>(exprs, type);
			        localctx.tree->location = location;
			    
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}
	// @RuleVersion(0)
	public arrayInitializerExpression(): ArrayInitializerExpressionContext {
		let localctx: ArrayInitializerExpressionContext = new ArrayInitializerExpressionContext(this, this._ctx, this.state);
		this.enterRule(localctx, 96, GPULangParser.RULE_arrayInitializerExpression);

		        localctx.tree =  nullptr;
		        std::vector<Expression*> exprs;
		        Symbol::Location location;
		    
		let _la: number;
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 951;
			this.match(GPULangParser.LL);
			 location = SetupFile(); 
			this.state = 964;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la===1 || _la===2 || ((((_la - 35)) & ~0x1F) === 0 && ((1 << (_la - 35)) & 25207875) !== 0) || ((((_la - 73)) & ~0x1F) === 0 && ((1 << (_la - 73)) & 14955) !== 0)) {
				{
				this.state = 953;
				localctx._arg0 = this.logicalOrExpression();
				 exprs.push_back(localctx._arg0.tree); 
				this.state = 961;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
				while (_la===39) {
					{
					{
					this.state = 955;
					this.match(GPULangParser.CO);
					this.state = 956;
					localctx._argN = this.logicalOrExpression();
					 exprs.push_back(localctx._argN.tree); 
					}
					}
					this.state = 963;
					this._errHandler.sync(this);
					_la = this._input.LA(1);
				}
				}
			}

			this.state = 966;
			this.match(GPULangParser.RR);

			        localctx.tree =  Alloc<ArrayInitializerExpression>(exprs);
			        localctx.tree->location = location;
			    
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return localctx;
	}

	public static readonly _serializedATN: number[] = [4,1,87,970,2,0,7,0,2,
	1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,7,7,7,2,8,7,8,2,9,7,9,2,
	10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,14,2,15,7,15,2,16,7,16,2,17,
	7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,21,2,22,7,22,2,23,7,23,2,24,7,
	24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,28,2,29,7,29,2,30,7,30,2,31,7,31,
	2,32,7,32,2,33,7,33,2,34,7,34,2,35,7,35,2,36,7,36,2,37,7,37,2,38,7,38,2,
	39,7,39,2,40,7,40,2,41,7,41,2,42,7,42,2,43,7,43,2,44,7,44,2,45,7,45,2,46,
	7,46,2,47,7,47,2,48,7,48,1,0,1,0,1,0,5,0,102,8,0,10,0,12,0,105,9,0,1,0,
	1,0,1,0,1,0,5,0,111,8,0,10,0,12,0,114,9,0,1,0,3,0,117,8,0,1,1,1,1,1,1,1,
	1,3,1,123,8,1,1,2,1,2,1,2,1,2,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,
	3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,
	3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,5,3,165,8,3,10,3,12,3,168,9,3,1,4,1,4,1,4,
	1,4,1,4,1,4,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,7,
	1,7,1,7,1,7,1,7,1,7,1,7,1,7,3,7,198,8,7,1,8,1,8,1,8,1,8,1,8,1,8,1,8,3,8,
	207,8,8,1,8,1,8,1,8,5,8,212,8,8,10,8,12,8,215,9,8,1,8,1,8,1,8,1,9,1,9,1,
	9,5,9,223,8,9,10,9,12,9,226,9,9,1,9,1,9,1,9,4,9,231,8,9,11,9,12,9,232,1,
	9,1,9,1,9,1,9,1,9,5,9,240,8,9,10,9,12,9,243,9,9,1,9,1,9,1,9,1,9,3,9,249,
	8,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,5,9,258,8,9,10,9,12,9,261,9,9,3,9,263,8,
	9,1,9,1,9,1,10,1,10,1,10,5,10,270,8,10,10,10,12,10,273,9,10,1,10,1,10,1,
	10,5,10,278,8,10,10,10,12,10,281,9,10,1,10,1,10,1,10,1,10,1,11,1,11,1,11,
	1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,3,11,300,8,11,1,11,5,
	11,303,8,11,10,11,12,11,306,9,11,1,11,1,11,1,11,1,11,5,11,312,8,11,10,11,
	12,11,315,9,11,1,11,1,11,1,11,1,12,1,12,1,12,1,12,1,12,1,12,1,12,3,12,327,
	8,12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,3,12,336,8,12,1,12,1,12,1,12,1,
	12,1,12,1,12,1,12,1,12,3,12,346,8,12,1,12,5,12,349,8,12,10,12,12,12,352,
	9,12,3,12,354,8,12,1,12,1,12,1,12,1,13,1,13,1,13,5,13,362,8,13,10,13,12,
	13,365,9,13,1,13,1,13,1,13,1,13,1,13,1,13,1,13,1,13,1,13,3,13,376,8,13,
	1,13,1,13,1,14,1,14,1,14,5,14,383,8,14,10,14,12,14,386,9,14,1,14,1,14,1,
	14,1,14,1,14,1,14,1,14,1,14,1,14,5,14,397,8,14,10,14,12,14,400,9,14,3,14,
	402,8,14,1,14,1,14,1,14,1,14,1,15,1,15,1,15,1,15,1,15,1,15,1,16,1,16,1,
	16,5,16,417,8,16,10,16,12,16,420,9,16,1,16,1,16,1,16,1,16,1,16,1,16,1,16,
	1,16,5,16,430,8,16,10,16,12,16,433,9,16,1,16,1,16,1,16,1,17,1,17,1,17,1,
	17,1,17,1,17,1,17,1,17,1,17,1,17,5,17,448,8,17,10,17,12,17,451,9,17,1,17,
	1,17,1,17,1,18,1,18,1,18,1,18,1,18,1,18,1,18,1,18,1,18,1,18,5,18,466,8,
	18,10,18,12,18,469,9,18,1,18,1,18,1,18,1,19,1,19,1,19,1,19,1,19,1,19,1,
	19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,
	1,19,1,19,1,19,1,19,1,19,1,19,1,19,3,19,502,8,19,1,20,1,20,1,20,1,21,1,
	21,1,21,1,21,1,21,1,21,1,21,1,21,1,21,1,21,1,21,1,21,3,21,519,8,21,1,21,
	1,21,1,22,1,22,1,22,1,22,1,22,1,22,3,22,529,8,22,1,22,1,22,1,22,1,22,3,
	22,535,8,22,1,22,1,22,1,22,1,22,3,22,541,8,22,1,22,1,22,1,22,1,22,5,22,
	547,8,22,10,22,12,22,550,9,22,1,22,1,22,1,22,1,22,1,23,1,23,1,23,1,23,1,
	23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,24,1,24,1,24,1,24,1,24,1,24,
	1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,
	24,1,24,3,24,591,8,24,1,25,1,25,1,25,1,25,1,25,1,25,1,25,1,25,1,25,1,25,
	5,25,603,8,25,10,25,12,25,606,9,25,1,25,1,25,1,25,1,26,1,26,1,26,1,26,1,
	26,3,26,616,8,26,1,26,1,26,1,26,1,26,1,26,1,26,1,26,1,26,1,26,1,26,1,26,
	1,26,1,26,1,26,3,26,632,8,26,1,27,1,27,1,27,1,27,1,27,1,28,1,28,1,28,1,
	28,1,28,1,28,1,28,1,28,1,28,1,28,1,28,1,28,1,28,1,28,3,28,653,8,28,5,28,
	655,8,28,10,28,12,28,658,9,28,1,28,1,28,1,28,1,28,1,28,3,28,665,8,28,3,
	28,667,8,28,1,28,1,28,1,28,1,29,1,29,1,29,1,29,1,29,1,30,1,30,1,30,1,31,
	1,31,1,31,1,31,1,31,1,31,1,31,5,31,687,8,31,10,31,12,31,690,9,31,1,32,1,
	32,1,32,1,32,1,32,1,32,1,32,1,32,1,32,1,32,1,32,1,32,1,32,1,32,5,32,706,
	8,32,10,32,12,32,709,9,32,1,33,1,33,1,33,1,33,1,33,1,33,1,33,5,33,718,8,
	33,10,33,12,33,721,9,33,1,34,1,34,1,34,1,34,1,34,1,34,1,34,5,34,730,8,34,
	10,34,12,34,733,9,34,1,35,1,35,1,35,1,35,1,35,1,35,1,35,5,35,742,8,35,10,
	35,12,35,745,9,35,1,36,1,36,1,36,1,36,1,36,1,36,1,36,5,36,754,8,36,10,36,
	12,36,757,9,36,1,37,1,37,1,37,1,37,1,37,1,37,1,37,5,37,766,8,37,10,37,12,
	37,769,9,37,1,38,1,38,1,38,1,38,1,38,1,38,1,38,5,38,778,8,38,10,38,12,38,
	781,9,38,1,39,1,39,1,39,1,39,1,39,1,39,1,39,5,39,790,8,39,10,39,12,39,793,
	9,39,1,40,1,40,1,40,1,40,1,40,1,40,1,40,5,40,802,8,40,10,40,12,40,805,9,
	40,1,41,1,41,1,41,1,41,1,41,1,41,1,41,5,41,814,8,41,10,41,12,41,817,9,41,
	1,42,1,42,1,42,1,42,1,42,1,42,1,42,5,42,826,8,42,10,42,12,42,829,9,42,1,
	43,1,43,1,43,1,43,1,43,1,43,1,43,1,43,1,43,1,43,5,43,841,8,43,10,43,12,
	43,844,9,43,3,43,846,8,43,1,43,1,43,1,43,1,43,1,43,1,43,1,43,1,43,1,43,
	1,43,1,43,1,43,1,43,1,43,1,43,1,43,1,43,3,43,865,8,43,1,43,1,43,5,43,869,
	8,43,10,43,12,43,872,9,43,1,43,1,43,1,43,5,43,877,8,43,10,43,12,43,880,
	9,43,1,43,1,43,3,43,884,8,43,1,44,1,44,5,44,888,8,44,10,44,12,44,891,9,
	44,1,44,1,44,1,44,1,45,1,45,1,45,1,45,1,45,1,46,1,46,1,46,1,46,1,46,1,46,
	1,46,1,46,1,46,1,46,1,46,1,46,1,46,1,46,1,46,1,46,1,46,1,46,1,46,1,46,1,
	46,1,46,1,46,1,46,1,46,1,46,1,46,1,46,1,46,3,46,930,8,46,1,47,1,47,1,47,
	1,47,1,47,1,47,1,47,1,47,1,47,1,47,5,47,942,8,47,10,47,12,47,945,9,47,3,
	47,947,8,47,1,47,1,47,1,47,1,48,1,48,1,48,1,48,1,48,1,48,1,48,1,48,5,48,
	960,8,48,10,48,12,48,963,9,48,3,48,965,8,48,1,48,1,48,1,48,1,48,1,166,0,
	49,0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,
	50,52,54,56,58,60,62,64,66,68,70,72,74,76,78,80,82,84,86,88,90,92,94,96,
	0,10,1,0,50,50,1,0,59,59,5,0,26,32,49,49,53,53,55,55,57,57,1,0,66,67,1,
	0,62,65,1,0,33,34,1,0,73,74,2,0,70,70,75,76,1,0,35,36,5,0,35,36,48,48,58,
	58,73,74,76,76,1027,0,116,1,0,0,0,2,122,1,0,0,0,4,124,1,0,0,0,6,166,1,0,
	0,0,8,169,1,0,0,0,10,175,1,0,0,0,12,182,1,0,0,0,14,197,1,0,0,0,16,213,1,
	0,0,0,18,224,1,0,0,0,20,271,1,0,0,0,22,286,1,0,0,0,24,319,1,0,0,0,26,363,
	1,0,0,0,28,384,1,0,0,0,30,407,1,0,0,0,32,418,1,0,0,0,34,437,1,0,0,0,36,
	455,1,0,0,0,38,501,1,0,0,0,40,503,1,0,0,0,42,506,1,0,0,0,44,522,1,0,0,0,
	46,555,1,0,0,0,48,590,1,0,0,0,50,592,1,0,0,0,52,631,1,0,0,0,54,633,1,0,
	0,0,56,638,1,0,0,0,58,671,1,0,0,0,60,676,1,0,0,0,62,679,1,0,0,0,64,691,
	1,0,0,0,66,710,1,0,0,0,68,722,1,0,0,0,70,734,1,0,0,0,72,746,1,0,0,0,74,
	758,1,0,0,0,76,770,1,0,0,0,78,782,1,0,0,0,80,794,1,0,0,0,82,806,1,0,0,0,
	84,818,1,0,0,0,86,883,1,0,0,0,88,889,1,0,0,0,90,895,1,0,0,0,92,929,1,0,
	0,0,94,931,1,0,0,0,96,951,1,0,0,0,98,103,5,50,0,0,99,100,8,0,0,0,100,102,
	6,0,-1,0,101,99,1,0,0,0,102,105,1,0,0,0,103,101,1,0,0,0,103,104,1,0,0,0,
	104,106,1,0,0,0,105,103,1,0,0,0,106,117,5,50,0,0,107,112,5,59,0,0,108,109,
	8,1,0,0,109,111,6,0,-1,0,110,108,1,0,0,0,111,114,1,0,0,0,112,110,1,0,0,
	0,112,113,1,0,0,0,113,115,1,0,0,0,114,112,1,0,0,0,115,117,5,59,0,0,116,
	98,1,0,0,0,116,107,1,0,0,0,117,1,1,0,0,0,118,119,5,1,0,0,119,123,6,1,-1,
	0,120,121,5,2,0,0,121,123,6,1,-1,0,122,118,1,0,0,0,122,120,1,0,0,0,123,
	3,1,0,0,0,124,125,3,6,3,0,125,126,6,2,-1,0,126,127,5,0,0,1,127,5,1,0,0,
	0,128,165,3,8,4,0,129,130,3,10,5,0,130,131,5,38,0,0,131,132,6,3,-1,0,132,
	165,1,0,0,0,133,134,3,28,14,0,134,135,5,38,0,0,135,136,6,3,-1,0,136,165,
	1,0,0,0,137,138,3,30,15,0,138,139,6,3,-1,0,139,165,1,0,0,0,140,141,3,18,
	9,0,141,142,5,38,0,0,142,143,6,3,-1,0,143,165,1,0,0,0,144,145,3,22,11,0,
	145,146,5,38,0,0,146,147,6,3,-1,0,147,165,1,0,0,0,148,149,3,24,12,0,149,
	150,5,38,0,0,150,151,6,3,-1,0,151,165,1,0,0,0,152,153,3,36,18,0,153,154,
	5,38,0,0,154,155,6,3,-1,0,155,165,1,0,0,0,156,157,3,34,17,0,157,158,5,38,
	0,0,158,159,6,3,-1,0,159,165,1,0,0,0,160,161,3,32,16,0,161,162,5,38,0,0,
	162,163,6,3,-1,0,163,165,1,0,0,0,164,128,1,0,0,0,164,129,1,0,0,0,164,133,
	1,0,0,0,164,137,1,0,0,0,164,140,1,0,0,0,164,144,1,0,0,0,164,148,1,0,0,0,
	164,152,1,0,0,0,164,156,1,0,0,0,164,160,1,0,0,0,165,168,1,0,0,0,166,167,
	1,0,0,0,166,164,1,0,0,0,167,7,1,0,0,0,168,166,1,0,0,0,169,170,5,3,0,0,170,
	171,6,4,-1,0,171,172,5,78,0,0,172,173,3,0,0,0,173,174,6,4,-1,0,174,9,1,
	0,0,0,175,176,5,4,0,0,176,177,5,86,0,0,177,178,5,5,0,0,178,179,5,86,0,0,
	179,180,6,5,-1,0,180,181,6,5,-1,0,181,11,1,0,0,0,182,183,5,72,0,0,183,184,
	5,86,0,0,184,185,5,41,0,0,185,186,3,60,30,0,186,187,5,42,0,0,187,188,6,
	6,-1,0,188,13,1,0,0,0,189,190,5,86,0,0,190,191,5,41,0,0,191,192,3,60,30,
	0,192,193,5,42,0,0,193,194,6,7,-1,0,194,198,1,0,0,0,195,196,5,86,0,0,196,
	198,6,7,-1,0,197,189,1,0,0,0,197,195,1,0,0,0,198,15,1,0,0,0,199,200,5,76,
	0,0,200,212,6,8,-1,0,201,202,5,45,0,0,202,206,6,8,-1,0,203,204,3,60,30,
	0,204,205,6,8,-1,0,205,207,1,0,0,0,206,203,1,0,0,0,206,207,1,0,0,0,207,
	208,1,0,0,0,208,212,5,46,0,0,209,210,5,86,0,0,210,212,6,8,-1,0,211,199,
	1,0,0,0,211,201,1,0,0,0,211,209,1,0,0,0,212,215,1,0,0,0,213,211,1,0,0,0,
	213,214,1,0,0,0,214,216,1,0,0,0,215,213,1,0,0,0,216,217,5,86,0,0,217,218,
	6,8,-1,0,218,17,1,0,0,0,219,220,3,12,6,0,220,221,6,9,-1,0,221,223,1,0,0,
	0,222,219,1,0,0,0,223,226,1,0,0,0,224,222,1,0,0,0,224,225,1,0,0,0,225,230,
	1,0,0,0,226,224,1,0,0,0,227,228,3,14,7,0,228,229,6,9,-1,0,229,231,1,0,0,
	0,230,227,1,0,0,0,231,232,1,0,0,0,232,230,1,0,0,0,232,233,1,0,0,0,233,234,
	1,0,0,0,234,235,5,86,0,0,235,241,6,9,-1,0,236,237,5,39,0,0,237,238,5,86,
	0,0,238,240,6,9,-1,0,239,236,1,0,0,0,240,243,1,0,0,0,241,239,1,0,0,0,241,
	242,1,0,0,0,242,248,1,0,0,0,243,241,1,0,0,0,244,245,5,40,0,0,245,246,3,
	16,8,0,246,247,6,9,-1,0,247,249,1,0,0,0,248,244,1,0,0,0,248,249,1,0,0,0,
	249,262,1,0,0,0,250,251,5,49,0,0,251,252,3,66,33,0,252,259,6,9,-1,0,253,
	254,5,39,0,0,254,255,3,66,33,0,255,256,6,9,-1,0,256,258,1,0,0,0,257,253,
	1,0,0,0,258,261,1,0,0,0,259,257,1,0,0,0,259,260,1,0,0,0,260,263,1,0,0,0,
	261,259,1,0,0,0,262,250,1,0,0,0,262,263,1,0,0,0,263,264,1,0,0,0,264,265,
	6,9,-1,0,265,19,1,0,0,0,266,267,3,12,6,0,267,268,6,10,-1,0,268,270,1,0,
	0,0,269,266,1,0,0,0,270,273,1,0,0,0,271,269,1,0,0,0,271,272,1,0,0,0,272,
	279,1,0,0,0,273,271,1,0,0,0,274,275,3,14,7,0,275,276,6,10,-1,0,276,278,
	1,0,0,0,277,274,1,0,0,0,278,281,1,0,0,0,279,277,1,0,0,0,279,280,1,0,0,0,
	280,282,1,0,0,0,281,279,1,0,0,0,282,283,5,6,0,0,283,284,5,86,0,0,284,285,
	6,10,-1,0,285,21,1,0,0,0,286,287,3,20,10,0,287,288,6,11,-1,0,288,313,5,
	43,0,0,289,290,5,86,0,0,290,291,6,11,-1,0,291,304,5,40,0,0,292,293,5,76,
	0,0,293,303,6,11,-1,0,294,295,5,45,0,0,295,299,6,11,-1,0,296,297,3,60,30,
	0,297,298,6,11,-1,0,298,300,1,0,0,0,299,296,1,0,0,0,299,300,1,0,0,0,300,
	301,1,0,0,0,301,303,5,46,0,0,302,292,1,0,0,0,302,294,1,0,0,0,303,306,1,
	0,0,0,304,302,1,0,0,0,304,305,1,0,0,0,305,307,1,0,0,0,306,304,1,0,0,0,307,
	308,5,86,0,0,308,309,6,11,-1,0,309,310,5,38,0,0,310,312,6,11,-1,0,311,289,
	1,0,0,0,312,315,1,0,0,0,313,311,1,0,0,0,313,314,1,0,0,0,314,316,1,0,0,0,
	315,313,1,0,0,0,316,317,5,44,0,0,317,318,6,11,-1,0,318,23,1,0,0,0,319,320,
	5,7,0,0,320,321,5,86,0,0,321,326,6,12,-1,0,322,323,5,40,0,0,323,324,3,16,
	8,0,324,325,6,12,-1,0,325,327,1,0,0,0,326,322,1,0,0,0,326,327,1,0,0,0,327,
	328,1,0,0,0,328,353,5,43,0,0,329,330,5,86,0,0,330,335,6,12,-1,0,331,332,
	5,49,0,0,332,333,3,60,30,0,333,334,6,12,-1,0,334,336,1,0,0,0,335,331,1,
	0,0,0,335,336,1,0,0,0,336,337,1,0,0,0,337,350,6,12,-1,0,338,339,5,39,0,
	0,339,340,5,86,0,0,340,345,6,12,-1,0,341,342,5,49,0,0,342,343,3,60,30,0,
	343,344,6,12,-1,0,344,346,1,0,0,0,345,341,1,0,0,0,345,346,1,0,0,0,346,347,
	1,0,0,0,347,349,6,12,-1,0,348,338,1,0,0,0,349,352,1,0,0,0,350,348,1,0,0,
	0,350,351,1,0,0,0,351,354,1,0,0,0,352,350,1,0,0,0,353,329,1,0,0,0,353,354,
	1,0,0,0,354,355,1,0,0,0,355,356,5,44,0,0,356,357,6,12,-1,0,357,25,1,0,0,
	0,358,359,3,14,7,0,359,360,6,13,-1,0,360,362,1,0,0,0,361,358,1,0,0,0,362,
	365,1,0,0,0,363,361,1,0,0,0,363,364,1,0,0,0,364,366,1,0,0,0,365,363,1,0,
	0,0,366,367,5,86,0,0,367,368,6,13,-1,0,368,369,5,40,0,0,369,370,3,16,8,
	0,370,375,6,13,-1,0,371,372,5,49,0,0,372,373,3,66,33,0,373,374,6,13,-1,
	0,374,376,1,0,0,0,375,371,1,0,0,0,375,376,1,0,0,0,376,377,1,0,0,0,377,378,
	6,13,-1,0,378,27,1,0,0,0,379,380,3,14,7,0,380,381,6,14,-1,0,381,383,1,0,
	0,0,382,379,1,0,0,0,383,386,1,0,0,0,384,382,1,0,0,0,384,385,1,0,0,0,385,
	387,1,0,0,0,386,384,1,0,0,0,387,388,5,86,0,0,388,389,6,14,-1,0,389,401,
	5,41,0,0,390,391,3,26,13,0,391,398,6,14,-1,0,392,393,5,39,0,0,393,394,3,
	26,13,0,394,395,6,14,-1,0,395,397,1,0,0,0,396,392,1,0,0,0,397,400,1,0,0,
	0,398,396,1,0,0,0,398,399,1,0,0,0,399,402,1,0,0,0,400,398,1,0,0,0,401,390,
	1,0,0,0,401,402,1,0,0,0,402,403,1,0,0,0,403,404,5,42,0,0,404,405,3,16,8,
	0,405,406,6,14,-1,0,406,29,1,0,0,0,407,408,3,28,14,0,408,409,6,15,-1,0,
	409,410,6,15,-1,0,410,411,3,50,25,0,411,412,6,15,-1,0,412,31,1,0,0,0,413,
	414,3,12,6,0,414,415,6,16,-1,0,415,417,1,0,0,0,416,413,1,0,0,0,417,420,
	1,0,0,0,418,416,1,0,0,0,418,419,1,0,0,0,419,421,1,0,0,0,420,418,1,0,0,0,
	421,422,5,8,0,0,422,423,5,86,0,0,423,424,6,16,-1,0,424,431,5,43,0,0,425,
	426,3,60,30,0,426,427,6,16,-1,0,427,428,5,38,0,0,428,430,1,0,0,0,429,425,
	1,0,0,0,430,433,1,0,0,0,431,429,1,0,0,0,431,432,1,0,0,0,432,434,1,0,0,0,
	433,431,1,0,0,0,434,435,5,44,0,0,435,436,6,16,-1,0,436,33,1,0,0,0,437,438,
	5,9,0,0,438,439,6,17,-1,0,439,440,1,0,0,0,440,441,5,86,0,0,441,442,6,17,
	-1,0,442,449,5,43,0,0,443,444,3,60,30,0,444,445,6,17,-1,0,445,446,5,38,
	0,0,446,448,1,0,0,0,447,443,1,0,0,0,448,451,1,0,0,0,449,447,1,0,0,0,449,
	450,1,0,0,0,450,452,1,0,0,0,451,449,1,0,0,0,452,453,5,44,0,0,453,454,6,
	17,-1,0,454,35,1,0,0,0,455,456,5,10,0,0,456,457,6,18,-1,0,457,458,1,0,0,
	0,458,459,5,86,0,0,459,460,6,18,-1,0,460,467,5,43,0,0,461,462,3,60,30,0,
	462,463,6,18,-1,0,463,464,5,38,0,0,464,466,1,0,0,0,465,461,1,0,0,0,466,
	469,1,0,0,0,467,465,1,0,0,0,467,468,1,0,0,0,468,470,1,0,0,0,469,467,1,0,
	0,0,470,471,5,44,0,0,471,472,6,18,-1,0,472,37,1,0,0,0,473,474,3,42,21,0,
	474,475,6,19,-1,0,475,502,1,0,0,0,476,477,3,50,25,0,477,478,6,19,-1,0,478,
	502,1,0,0,0,479,480,3,44,22,0,480,481,6,19,-1,0,481,502,1,0,0,0,482,483,
	3,48,24,0,483,484,6,19,-1,0,484,502,1,0,0,0,485,486,3,56,28,0,486,487,6,
	19,-1,0,487,502,1,0,0,0,488,489,3,52,26,0,489,490,6,19,-1,0,490,502,1,0,
	0,0,491,492,3,54,27,0,492,493,6,19,-1,0,493,502,1,0,0,0,494,495,3,58,29,
	0,495,496,6,19,-1,0,496,502,1,0,0,0,497,498,3,40,20,0,498,499,5,38,0,0,
	499,500,6,19,-1,0,500,502,1,0,0,0,501,473,1,0,0,0,501,476,1,0,0,0,501,479,
	1,0,0,0,501,482,1,0,0,0,501,485,1,0,0,0,501,488,1,0,0,0,501,491,1,0,0,0,
	501,494,1,0,0,0,501,497,1,0,0,0,502,39,1,0,0,0,503,504,3,60,30,0,504,505,
	6,20,-1,0,505,41,1,0,0,0,506,507,5,11,0,0,507,508,6,21,-1,0,508,509,5,41,
	0,0,509,510,3,60,30,0,510,511,6,21,-1,0,511,512,5,42,0,0,512,513,3,38,19,
	0,513,518,6,21,-1,0,514,515,5,12,0,0,515,516,3,38,19,0,516,517,6,21,-1,
	0,517,519,1,0,0,0,518,514,1,0,0,0,518,519,1,0,0,0,519,520,1,0,0,0,520,521,
	6,21,-1,0,521,43,1,0,0,0,522,523,5,13,0,0,523,524,6,22,-1,0,524,528,5,41,
	0,0,525,526,3,18,9,0,526,527,6,22,-1,0,527,529,1,0,0,0,528,525,1,0,0,0,
	528,529,1,0,0,0,529,530,1,0,0,0,530,534,5,38,0,0,531,532,3,60,30,0,532,
	533,6,22,-1,0,533,535,1,0,0,0,534,531,1,0,0,0,534,535,1,0,0,0,535,536,1,
	0,0,0,536,540,5,38,0,0,537,538,3,60,30,0,538,539,6,22,-1,0,539,541,1,0,
	0,0,540,537,1,0,0,0,540,541,1,0,0,0,541,542,1,0,0,0,542,548,5,42,0,0,543,
	544,3,14,7,0,544,545,6,22,-1,0,545,547,1,0,0,0,546,543,1,0,0,0,547,550,
	1,0,0,0,548,546,1,0,0,0,548,549,1,0,0,0,549,551,1,0,0,0,550,548,1,0,0,0,
	551,552,3,38,19,0,552,553,6,22,-1,0,553,554,6,22,-1,0,554,45,1,0,0,0,555,
	556,5,13,0,0,556,557,6,23,-1,0,557,558,5,41,0,0,558,559,5,86,0,0,559,560,
	5,40,0,0,560,561,5,86,0,0,561,562,5,14,0,0,562,563,5,86,0,0,563,564,5,42,
	0,0,564,565,3,38,19,0,565,566,6,23,-1,0,566,567,6,23,-1,0,567,47,1,0,0,
	0,568,569,5,15,0,0,569,570,6,24,-1,0,570,571,5,41,0,0,571,572,3,60,30,0,
	572,573,6,24,-1,0,573,574,5,42,0,0,574,575,3,38,19,0,575,576,6,24,-1,0,
	576,577,6,24,-1,0,577,591,1,0,0,0,578,579,5,16,0,0,579,580,6,24,-1,0,580,
	581,3,38,19,0,581,582,6,24,-1,0,582,583,5,15,0,0,583,584,5,41,0,0,584,585,
	3,60,30,0,585,586,6,24,-1,0,586,587,5,42,0,0,587,588,5,38,0,0,588,589,6,
	24,-1,0,589,591,1,0,0,0,590,568,1,0,0,0,590,578,1,0,0,0,591,49,1,0,0,0,
	592,593,5,43,0,0,593,604,6,25,-1,0,594,595,3,18,9,0,595,596,5,38,0,0,596,
	597,6,25,-1,0,597,603,1,0,0,0,598,599,3,38,19,0,599,600,6,25,-1,0,600,603,
	1,0,0,0,601,603,3,8,4,0,602,594,1,0,0,0,602,598,1,0,0,0,602,601,1,0,0,0,
	603,606,1,0,0,0,604,602,1,0,0,0,604,605,1,0,0,0,605,607,1,0,0,0,606,604,
	1,0,0,0,607,608,5,44,0,0,608,609,6,25,-1,0,609,51,1,0,0,0,610,611,5,17,
	0,0,611,615,6,26,-1,0,612,613,3,60,30,0,613,614,6,26,-1,0,614,616,1,0,0,
	0,615,612,1,0,0,0,615,616,1,0,0,0,616,617,1,0,0,0,617,618,5,38,0,0,618,
	632,6,26,-1,0,619,620,5,18,0,0,620,621,6,26,-1,0,621,622,5,38,0,0,622,632,
	6,26,-1,0,623,624,5,19,0,0,624,625,6,26,-1,0,625,626,5,38,0,0,626,632,6,
	26,-1,0,627,628,5,20,0,0,628,629,6,26,-1,0,629,630,5,38,0,0,630,632,6,26,
	-1,0,631,610,1,0,0,0,631,619,1,0,0,0,631,623,1,0,0,0,631,627,1,0,0,0,632,
	53,1,0,0,0,633,634,5,21,0,0,634,635,6,27,-1,0,635,636,5,38,0,0,636,637,
	6,27,-1,0,637,55,1,0,0,0,638,639,5,22,0,0,639,640,6,28,-1,0,640,641,5,41,
	0,0,641,642,3,60,30,0,642,643,5,42,0,0,643,644,6,28,-1,0,644,656,5,43,0,
	0,645,646,5,23,0,0,646,647,3,60,30,0,647,648,5,40,0,0,648,652,6,28,-1,0,
	649,650,3,38,19,0,650,651,6,28,-1,0,651,653,1,0,0,0,652,649,1,0,0,0,652,
	653,1,0,0,0,653,655,1,0,0,0,654,645,1,0,0,0,655,658,1,0,0,0,656,654,1,0,
	0,0,656,657,1,0,0,0,657,666,1,0,0,0,658,656,1,0,0,0,659,660,5,24,0,0,660,
	664,5,40,0,0,661,662,3,38,19,0,662,663,6,28,-1,0,663,665,1,0,0,0,664,661,
	1,0,0,0,664,665,1,0,0,0,665,667,1,0,0,0,666,659,1,0,0,0,666,667,1,0,0,0,
	667,668,1,0,0,0,668,669,5,44,0,0,669,670,6,28,-1,0,670,57,1,0,0,0,671,672,
	5,25,0,0,672,673,6,29,-1,0,673,674,5,38,0,0,674,675,6,29,-1,0,675,59,1,
	0,0,0,676,677,3,62,31,0,677,678,6,30,-1,0,678,61,1,0,0,0,679,680,3,64,32,
	0,680,688,6,31,-1,0,681,682,5,39,0,0,682,683,6,31,-1,0,683,684,3,64,32,
	0,684,685,6,31,-1,0,685,687,1,0,0,0,686,681,1,0,0,0,687,690,1,0,0,0,688,
	686,1,0,0,0,688,689,1,0,0,0,689,63,1,0,0,0,690,688,1,0,0,0,691,692,3,66,
	33,0,692,707,6,32,-1,0,693,694,7,2,0,0,694,695,6,32,-1,0,695,696,3,66,33,
	0,696,697,6,32,-1,0,697,706,1,0,0,0,698,699,5,51,0,0,699,700,6,32,-1,0,
	700,701,3,60,30,0,701,702,5,40,0,0,702,703,3,60,30,0,703,704,6,32,-1,0,
	704,706,1,0,0,0,705,693,1,0,0,0,705,698,1,0,0,0,706,709,1,0,0,0,707,705,
	1,0,0,0,707,708,1,0,0,0,708,65,1,0,0,0,709,707,1,0,0,0,710,711,3,68,34,
	0,711,719,6,33,-1,0,712,713,5,69,0,0,713,714,6,33,-1,0,714,715,3,68,34,
	0,715,716,6,33,-1,0,716,718,1,0,0,0,717,712,1,0,0,0,718,721,1,0,0,0,719,
	717,1,0,0,0,719,720,1,0,0,0,720,67,1,0,0,0,721,719,1,0,0,0,722,723,3,70,
	35,0,723,731,6,34,-1,0,724,725,5,68,0,0,725,726,6,34,-1,0,726,727,3,70,
	35,0,727,728,6,34,-1,0,728,730,1,0,0,0,729,724,1,0,0,0,730,733,1,0,0,0,
	731,729,1,0,0,0,731,732,1,0,0,0,732,69,1,0,0,0,733,731,1,0,0,0,734,735,
	3,72,36,0,735,743,6,35,-1,0,736,737,5,54,0,0,737,738,6,35,-1,0,738,739,
	3,72,36,0,739,740,6,35,-1,0,740,742,1,0,0,0,741,736,1,0,0,0,742,745,1,0,
	0,0,743,741,1,0,0,0,743,744,1,0,0,0,744,71,1,0,0,0,745,743,1,0,0,0,746,
	747,3,74,37,0,747,755,6,36,-1,0,748,749,5,56,0,0,749,750,6,36,-1,0,750,
	751,3,74,37,0,751,752,6,36,-1,0,752,754,1,0,0,0,753,748,1,0,0,0,754,757,
	1,0,0,0,755,753,1,0,0,0,755,756,1,0,0,0,756,73,1,0,0,0,757,755,1,0,0,0,
	758,759,3,76,38,0,759,767,6,37,-1,0,760,761,5,52,0,0,761,762,6,37,-1,0,
	762,763,3,76,38,0,763,764,6,37,-1,0,764,766,1,0,0,0,765,760,1,0,0,0,766,
	769,1,0,0,0,767,765,1,0,0,0,767,768,1,0,0,0,768,75,1,0,0,0,769,767,1,0,
	0,0,770,771,3,78,39,0,771,779,6,38,-1,0,772,773,7,3,0,0,773,774,6,38,-1,
	0,774,775,3,78,39,0,775,776,6,38,-1,0,776,778,1,0,0,0,777,772,1,0,0,0,778,
	781,1,0,0,0,779,777,1,0,0,0,779,780,1,0,0,0,780,77,1,0,0,0,781,779,1,0,
	0,0,782,783,3,80,40,0,783,791,6,39,-1,0,784,785,7,4,0,0,785,786,6,39,-1,
	0,786,787,3,80,40,0,787,788,6,39,-1,0,788,790,1,0,0,0,789,784,1,0,0,0,790,
	793,1,0,0,0,791,789,1,0,0,0,791,792,1,0,0,0,792,79,1,0,0,0,793,791,1,0,
	0,0,794,795,3,82,41,0,795,803,6,40,-1,0,796,797,7,5,0,0,797,798,6,40,-1,
	0,798,799,3,82,41,0,799,800,6,40,-1,0,800,802,1,0,0,0,801,796,1,0,0,0,802,
	805,1,0,0,0,803,801,1,0,0,0,803,804,1,0,0,0,804,81,1,0,0,0,805,803,1,0,
	0,0,806,807,3,84,42,0,807,815,6,41,-1,0,808,809,7,6,0,0,809,810,6,41,-1,
	0,810,811,3,84,42,0,811,812,6,41,-1,0,812,814,1,0,0,0,813,808,1,0,0,0,814,
	817,1,0,0,0,815,813,1,0,0,0,815,816,1,0,0,0,816,83,1,0,0,0,817,815,1,0,
	0,0,818,819,3,86,43,0,819,827,6,42,-1,0,820,821,7,7,0,0,821,822,6,42,-1,
	0,822,823,3,86,43,0,823,824,6,42,-1,0,824,826,1,0,0,0,825,820,1,0,0,0,826,
	829,1,0,0,0,827,825,1,0,0,0,827,828,1,0,0,0,828,85,1,0,0,0,829,827,1,0,
	0,0,830,831,3,88,44,0,831,870,6,43,-1,0,832,833,5,41,0,0,833,845,6,43,-1,
	0,834,835,3,66,33,0,835,842,6,43,-1,0,836,837,5,39,0,0,837,838,3,66,33,
	0,838,839,6,43,-1,0,839,841,1,0,0,0,840,836,1,0,0,0,841,844,1,0,0,0,842,
	840,1,0,0,0,842,843,1,0,0,0,843,846,1,0,0,0,844,842,1,0,0,0,845,834,1,0,
	0,0,845,846,1,0,0,0,846,847,1,0,0,0,847,848,5,42,0,0,848,869,6,43,-1,0,
	849,850,5,47,0,0,850,851,6,43,-1,0,851,852,3,86,43,0,852,853,6,43,-1,0,
	853,869,1,0,0,0,854,855,5,77,0,0,855,856,6,43,-1,0,856,857,3,86,43,0,857,
	858,6,43,-1,0,858,869,1,0,0,0,859,860,5,45,0,0,860,864,6,43,-1,0,861,862,
	3,60,30,0,862,863,6,43,-1,0,863,865,1,0,0,0,864,861,1,0,0,0,864,865,1,0,
	0,0,865,866,1,0,0,0,866,867,5,46,0,0,867,869,6,43,-1,0,868,832,1,0,0,0,
	868,849,1,0,0,0,868,854,1,0,0,0,868,859,1,0,0,0,869,872,1,0,0,0,870,868,
	1,0,0,0,870,871,1,0,0,0,871,884,1,0,0,0,872,870,1,0,0,0,873,878,3,88,44,
	0,874,875,7,8,0,0,875,877,6,43,-1,0,876,874,1,0,0,0,877,880,1,0,0,0,878,
	876,1,0,0,0,878,879,1,0,0,0,879,881,1,0,0,0,880,878,1,0,0,0,881,882,6,43,
	-1,0,882,884,1,0,0,0,883,830,1,0,0,0,883,873,1,0,0,0,884,87,1,0,0,0,885,
	886,7,9,0,0,886,888,6,44,-1,0,887,885,1,0,0,0,888,891,1,0,0,0,889,887,1,
	0,0,0,889,890,1,0,0,0,890,892,1,0,0,0,891,889,1,0,0,0,892,893,3,92,46,0,
	893,894,6,44,-1,0,894,89,1,0,0,0,895,896,3,92,46,0,896,897,5,37,0,0,897,
	898,3,92,46,0,898,899,6,45,-1,0,899,91,1,0,0,0,900,901,3,94,47,0,901,902,
	6,46,-1,0,902,930,1,0,0,0,903,904,3,96,48,0,904,905,6,46,-1,0,905,930,1,
	0,0,0,906,907,5,41,0,0,907,908,3,60,30,0,908,909,5,42,0,0,909,910,6,46,
	-1,0,910,930,1,0,0,0,911,912,5,78,0,0,912,930,6,46,-1,0,913,914,5,79,0,
	0,914,930,6,46,-1,0,915,916,5,82,0,0,916,930,6,46,-1,0,917,918,5,84,0,0,
	918,930,6,46,-1,0,919,920,5,85,0,0,920,930,6,46,-1,0,921,922,3,0,0,0,922,
	923,6,46,-1,0,923,930,1,0,0,0,924,925,5,86,0,0,925,930,6,46,-1,0,926,927,
	3,2,1,0,927,928,6,46,-1,0,928,930,1,0,0,0,929,900,1,0,0,0,929,903,1,0,0,
	0,929,906,1,0,0,0,929,911,1,0,0,0,929,913,1,0,0,0,929,915,1,0,0,0,929,917,
	1,0,0,0,929,919,1,0,0,0,929,921,1,0,0,0,929,924,1,0,0,0,929,926,1,0,0,0,
	930,93,1,0,0,0,931,932,5,86,0,0,932,933,6,47,-1,0,933,934,5,43,0,0,934,
	946,6,47,-1,0,935,936,3,66,33,0,936,943,6,47,-1,0,937,938,5,39,0,0,938,
	939,3,66,33,0,939,940,6,47,-1,0,940,942,1,0,0,0,941,937,1,0,0,0,942,945,
	1,0,0,0,943,941,1,0,0,0,943,944,1,0,0,0,944,947,1,0,0,0,945,943,1,0,0,0,
	946,935,1,0,0,0,946,947,1,0,0,0,947,948,1,0,0,0,948,949,5,44,0,0,949,950,
	6,47,-1,0,950,95,1,0,0,0,951,952,5,45,0,0,952,964,6,48,-1,0,953,954,3,66,
	33,0,954,961,6,48,-1,0,955,956,5,39,0,0,956,957,3,66,33,0,957,958,6,48,
	-1,0,958,960,1,0,0,0,959,955,1,0,0,0,960,963,1,0,0,0,961,959,1,0,0,0,961,
	962,1,0,0,0,962,965,1,0,0,0,963,961,1,0,0,0,964,953,1,0,0,0,964,965,1,0,
	0,0,965,966,1,0,0,0,966,967,5,46,0,0,967,968,6,48,-1,0,968,97,1,0,0,0,77,
	103,112,116,122,164,166,197,206,211,213,224,232,241,248,259,262,271,279,
	299,302,304,313,326,335,345,350,353,363,375,384,398,401,418,431,449,467,
	501,518,528,534,540,548,590,602,604,615,631,652,656,664,666,688,705,707,
	719,731,743,755,767,779,791,803,815,827,842,845,864,868,870,878,883,889,
	929,943,946,961,964];

	private static __ATN: ATN;
	public static get _ATN(): ATN {
		if (!GPULangParser.__ATN) {
			GPULangParser.__ATN = new ATNDeserializer().deserialize(GPULangParser._serializedATN);
		}

		return GPULangParser.__ATN;
	}


	static DecisionsToDFA = GPULangParser._ATN.decisionToState.map( (ds: DecisionState, index: number) => new DFA(ds, index) );

}

export class StringContext extends ParserRuleContext {
	public val: std::string;
	public _data!: Token;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public QO_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.QO);
	}
	public QO(i: number): TerminalNode {
		return this.getToken(GPULangParser.QO, i);
	}
	public Q_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.Q);
	}
	public Q(i: number): TerminalNode {
		return this.getToken(GPULangParser.Q, i);
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_string;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterString) {
	 		listener.enterString(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitString) {
	 		listener.exitString(this);
		}
	}
}


export class BooleanContext extends ParserRuleContext {
	public val: bool;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_boolean;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterBoolean) {
	 		listener.enterBoolean(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitBoolean) {
	 		listener.exitBoolean(this);
		}
	}
}


export class EntryContext extends ParserRuleContext {
	public returnEffect: Effect*;
	public _effect!: EffectContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public effect(): EffectContext {
		return this.getTypedRuleContext(EffectContext, 0) as EffectContext;
	}
	public EOF(): TerminalNode {
		return this.getToken(GPULangParser.EOF, 0);
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_entry;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterEntry) {
	 		listener.enterEntry(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitEntry) {
	 		listener.exitEntry(this);
		}
	}
}


export class EffectContext extends ParserRuleContext {
	public eff: Effect*;
	public _alias!: AliasContext;
	public _functionDeclaration!: FunctionDeclarationContext;
	public _function_!: FunctionContext;
	public _variables!: VariablesContext;
	public _structure!: StructureContext;
	public _enumeration!: EnumerationContext;
	public _state_!: StateContext;
	public _sampler!: SamplerContext;
	public _program!: ProgramContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public linePreprocessorEntry_list(): LinePreprocessorEntryContext[] {
		return this.getTypedRuleContexts(LinePreprocessorEntryContext) as LinePreprocessorEntryContext[];
	}
	public linePreprocessorEntry(i: number): LinePreprocessorEntryContext {
		return this.getTypedRuleContext(LinePreprocessorEntryContext, i) as LinePreprocessorEntryContext;
	}
	public alias_list(): AliasContext[] {
		return this.getTypedRuleContexts(AliasContext) as AliasContext[];
	}
	public alias(i: number): AliasContext {
		return this.getTypedRuleContext(AliasContext, i) as AliasContext;
	}
	public SC_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.SC);
	}
	public SC(i: number): TerminalNode {
		return this.getToken(GPULangParser.SC, i);
	}
	public functionDeclaration_list(): FunctionDeclarationContext[] {
		return this.getTypedRuleContexts(FunctionDeclarationContext) as FunctionDeclarationContext[];
	}
	public functionDeclaration(i: number): FunctionDeclarationContext {
		return this.getTypedRuleContext(FunctionDeclarationContext, i) as FunctionDeclarationContext;
	}
	public function__list(): FunctionContext[] {
		return this.getTypedRuleContexts(FunctionContext) as FunctionContext[];
	}
	public function_(i: number): FunctionContext {
		return this.getTypedRuleContext(FunctionContext, i) as FunctionContext;
	}
	public variables_list(): VariablesContext[] {
		return this.getTypedRuleContexts(VariablesContext) as VariablesContext[];
	}
	public variables(i: number): VariablesContext {
		return this.getTypedRuleContext(VariablesContext, i) as VariablesContext;
	}
	public structure_list(): StructureContext[] {
		return this.getTypedRuleContexts(StructureContext) as StructureContext[];
	}
	public structure(i: number): StructureContext {
		return this.getTypedRuleContext(StructureContext, i) as StructureContext;
	}
	public enumeration_list(): EnumerationContext[] {
		return this.getTypedRuleContexts(EnumerationContext) as EnumerationContext[];
	}
	public enumeration(i: number): EnumerationContext {
		return this.getTypedRuleContext(EnumerationContext, i) as EnumerationContext;
	}
	public state__list(): StateContext[] {
		return this.getTypedRuleContexts(StateContext) as StateContext[];
	}
	public state_(i: number): StateContext {
		return this.getTypedRuleContext(StateContext, i) as StateContext;
	}
	public sampler_list(): SamplerContext[] {
		return this.getTypedRuleContexts(SamplerContext) as SamplerContext[];
	}
	public sampler(i: number): SamplerContext {
		return this.getTypedRuleContext(SamplerContext, i) as SamplerContext;
	}
	public program_list(): ProgramContext[] {
		return this.getTypedRuleContexts(ProgramContext) as ProgramContext[];
	}
	public program(i: number): ProgramContext {
		return this.getTypedRuleContext(ProgramContext, i) as ProgramContext;
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_effect;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterEffect) {
	 		listener.enterEffect(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitEffect) {
	 		listener.exitEffect(this);
		}
	}
}


export class LinePreprocessorEntryContext extends ParserRuleContext {
	public _line!: Token;
	public _path!: StringContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public INTEGERLITERAL(): TerminalNode {
		return this.getToken(GPULangParser.INTEGERLITERAL, 0);
	}
	public string_(): StringContext {
		return this.getTypedRuleContext(StringContext, 0) as StringContext;
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_linePreprocessorEntry;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterLinePreprocessorEntry) {
	 		listener.enterLinePreprocessorEntry(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitLinePreprocessorEntry) {
	 		listener.exitLinePreprocessorEntry(this);
		}
	}
}


export class AliasContext extends ParserRuleContext {
	public sym: Alias*;
	public _name!: Token;
	public _type_!: Token;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public IDENTIFIER_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.IDENTIFIER);
	}
	public IDENTIFIER(i: number): TerminalNode {
		return this.getToken(GPULangParser.IDENTIFIER, i);
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_alias;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterAlias) {
	 		listener.enterAlias(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitAlias) {
	 		listener.exitAlias(this);
		}
	}
}


export class AnnotationContext extends ParserRuleContext {
	public annot: Annotation;
	public _name!: Token;
	public _value!: ExpressionContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public SOBAKA(): TerminalNode {
		return this.getToken(GPULangParser.SOBAKA, 0);
	}
	public LP(): TerminalNode {
		return this.getToken(GPULangParser.LP, 0);
	}
	public RP(): TerminalNode {
		return this.getToken(GPULangParser.RP, 0);
	}
	public IDENTIFIER(): TerminalNode {
		return this.getToken(GPULangParser.IDENTIFIER, 0);
	}
	public expression(): ExpressionContext {
		return this.getTypedRuleContext(ExpressionContext, 0) as ExpressionContext;
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_annotation;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterAnnotation) {
	 		listener.enterAnnotation(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitAnnotation) {
	 		listener.exitAnnotation(this);
		}
	}
}


export class AttributeContext extends ParserRuleContext {
	public attr: Attribute;
	public _name!: Token;
	public _expression!: ExpressionContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public LP(): TerminalNode {
		return this.getToken(GPULangParser.LP, 0);
	}
	public expression(): ExpressionContext {
		return this.getTypedRuleContext(ExpressionContext, 0) as ExpressionContext;
	}
	public RP(): TerminalNode {
		return this.getToken(GPULangParser.RP, 0);
	}
	public IDENTIFIER(): TerminalNode {
		return this.getToken(GPULangParser.IDENTIFIER, 0);
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_attribute;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterAttribute) {
	 		listener.enterAttribute(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitAttribute) {
	 		listener.exitAttribute(this);
		}
	}
}


export class TypeDeclarationContext extends ParserRuleContext {
	public type_: Type::FullType;
	public _arraySize0!: ExpressionContext;
	public _IDENTIFIER!: Token;
	public _typeName!: Token;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public IDENTIFIER_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.IDENTIFIER);
	}
	public IDENTIFIER(i: number): TerminalNode {
		return this.getToken(GPULangParser.IDENTIFIER, i);
	}
	public MUL_OP_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.MUL_OP);
	}
	public MUL_OP(i: number): TerminalNode {
		return this.getToken(GPULangParser.MUL_OP, i);
	}
	public LL_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.LL);
	}
	public LL(i: number): TerminalNode {
		return this.getToken(GPULangParser.LL, i);
	}
	public RR_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.RR);
	}
	public RR(i: number): TerminalNode {
		return this.getToken(GPULangParser.RR, i);
	}
	public expression_list(): ExpressionContext[] {
		return this.getTypedRuleContexts(ExpressionContext) as ExpressionContext[];
	}
	public expression(i: number): ExpressionContext {
		return this.getTypedRuleContext(ExpressionContext, i) as ExpressionContext;
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_typeDeclaration;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterTypeDeclaration) {
	 		listener.enterTypeDeclaration(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitTypeDeclaration) {
	 		listener.exitTypeDeclaration(this);
		}
	}
}


export class VariablesContext extends ParserRuleContext {
	public list: std::vector<Variable*>;
	public _annotation!: AnnotationContext;
	public _attribute!: AttributeContext;
	public _varName!: Token;
	public _varNameN!: Token;
	public _typeDeclaration!: TypeDeclarationContext;
	public _valueExpr!: LogicalOrExpressionContext;
	public _valueExprN!: LogicalOrExpressionContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public IDENTIFIER_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.IDENTIFIER);
	}
	public IDENTIFIER(i: number): TerminalNode {
		return this.getToken(GPULangParser.IDENTIFIER, i);
	}
	public annotation_list(): AnnotationContext[] {
		return this.getTypedRuleContexts(AnnotationContext) as AnnotationContext[];
	}
	public annotation(i: number): AnnotationContext {
		return this.getTypedRuleContext(AnnotationContext, i) as AnnotationContext;
	}
	public attribute_list(): AttributeContext[] {
		return this.getTypedRuleContexts(AttributeContext) as AttributeContext[];
	}
	public attribute(i: number): AttributeContext {
		return this.getTypedRuleContext(AttributeContext, i) as AttributeContext;
	}
	public CO_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.CO);
	}
	public CO(i: number): TerminalNode {
		return this.getToken(GPULangParser.CO, i);
	}
	public COL(): TerminalNode {
		return this.getToken(GPULangParser.COL, 0);
	}
	public typeDeclaration(): TypeDeclarationContext {
		return this.getTypedRuleContext(TypeDeclarationContext, 0) as TypeDeclarationContext;
	}
	public EQ(): TerminalNode {
		return this.getToken(GPULangParser.EQ, 0);
	}
	public logicalOrExpression_list(): LogicalOrExpressionContext[] {
		return this.getTypedRuleContexts(LogicalOrExpressionContext) as LogicalOrExpressionContext[];
	}
	public logicalOrExpression(i: number): LogicalOrExpressionContext {
		return this.getTypedRuleContext(LogicalOrExpressionContext, i) as LogicalOrExpressionContext;
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_variables;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterVariables) {
	 		listener.enterVariables(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitVariables) {
	 		listener.exitVariables(this);
		}
	}
}


export class StructureDeclarationContext extends ParserRuleContext {
	public sym: Structure*;
	public _annotation!: AnnotationContext;
	public _attribute!: AttributeContext;
	public _name!: Token;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public IDENTIFIER(): TerminalNode {
		return this.getToken(GPULangParser.IDENTIFIER, 0);
	}
	public annotation_list(): AnnotationContext[] {
		return this.getTypedRuleContexts(AnnotationContext) as AnnotationContext[];
	}
	public annotation(i: number): AnnotationContext {
		return this.getTypedRuleContext(AnnotationContext, i) as AnnotationContext;
	}
	public attribute_list(): AttributeContext[] {
		return this.getTypedRuleContexts(AttributeContext) as AttributeContext[];
	}
	public attribute(i: number): AttributeContext {
		return this.getTypedRuleContext(AttributeContext, i) as AttributeContext;
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_structureDeclaration;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterStructureDeclaration) {
	 		listener.enterStructureDeclaration(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitStructureDeclaration) {
	 		listener.exitStructureDeclaration(this);
		}
	}
}


export class StructureContext extends ParserRuleContext {
	public sym: Structure*;
	public _structureDeclaration!: StructureDeclarationContext;
	public _varName!: Token;
	public _arraySize0!: ExpressionContext;
	public _varTypeName!: Token;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public structureDeclaration(): StructureDeclarationContext {
		return this.getTypedRuleContext(StructureDeclarationContext, 0) as StructureDeclarationContext;
	}
	public LB(): TerminalNode {
		return this.getToken(GPULangParser.LB, 0);
	}
	public RB(): TerminalNode {
		return this.getToken(GPULangParser.RB, 0);
	}
	public COL_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.COL);
	}
	public COL(i: number): TerminalNode {
		return this.getToken(GPULangParser.COL, i);
	}
	public SC_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.SC);
	}
	public SC(i: number): TerminalNode {
		return this.getToken(GPULangParser.SC, i);
	}
	public IDENTIFIER_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.IDENTIFIER);
	}
	public IDENTIFIER(i: number): TerminalNode {
		return this.getToken(GPULangParser.IDENTIFIER, i);
	}
	public MUL_OP_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.MUL_OP);
	}
	public MUL_OP(i: number): TerminalNode {
		return this.getToken(GPULangParser.MUL_OP, i);
	}
	public LL_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.LL);
	}
	public LL(i: number): TerminalNode {
		return this.getToken(GPULangParser.LL, i);
	}
	public RR_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.RR);
	}
	public RR(i: number): TerminalNode {
		return this.getToken(GPULangParser.RR, i);
	}
	public expression_list(): ExpressionContext[] {
		return this.getTypedRuleContexts(ExpressionContext) as ExpressionContext[];
	}
	public expression(i: number): ExpressionContext {
		return this.getTypedRuleContext(ExpressionContext, i) as ExpressionContext;
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_structure;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterStructure) {
	 		listener.enterStructure(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitStructure) {
	 		listener.exitStructure(this);
		}
	}
}


export class EnumerationContext extends ParserRuleContext {
	public sym: Enumeration*;
	public _name!: Token;
	public _typeDeclaration!: TypeDeclarationContext;
	public _label!: Token;
	public _value!: ExpressionContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public LB(): TerminalNode {
		return this.getToken(GPULangParser.LB, 0);
	}
	public RB(): TerminalNode {
		return this.getToken(GPULangParser.RB, 0);
	}
	public IDENTIFIER_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.IDENTIFIER);
	}
	public IDENTIFIER(i: number): TerminalNode {
		return this.getToken(GPULangParser.IDENTIFIER, i);
	}
	public COL(): TerminalNode {
		return this.getToken(GPULangParser.COL, 0);
	}
	public typeDeclaration(): TypeDeclarationContext {
		return this.getTypedRuleContext(TypeDeclarationContext, 0) as TypeDeclarationContext;
	}
	public EQ_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.EQ);
	}
	public EQ(i: number): TerminalNode {
		return this.getToken(GPULangParser.EQ, i);
	}
	public CO_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.CO);
	}
	public CO(i: number): TerminalNode {
		return this.getToken(GPULangParser.CO, i);
	}
	public expression_list(): ExpressionContext[] {
		return this.getTypedRuleContexts(ExpressionContext) as ExpressionContext[];
	}
	public expression(i: number): ExpressionContext {
		return this.getTypedRuleContext(ExpressionContext, i) as ExpressionContext;
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_enumeration;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterEnumeration) {
	 		listener.enterEnumeration(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitEnumeration) {
	 		listener.exitEnumeration(this);
		}
	}
}


export class ParameterContext extends ParserRuleContext {
	public sym: Variable*;
	public _attribute!: AttributeContext;
	public _varName!: Token;
	public _typeDeclaration!: TypeDeclarationContext;
	public _valueExpr!: LogicalOrExpressionContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public COL(): TerminalNode {
		return this.getToken(GPULangParser.COL, 0);
	}
	public typeDeclaration(): TypeDeclarationContext {
		return this.getTypedRuleContext(TypeDeclarationContext, 0) as TypeDeclarationContext;
	}
	public IDENTIFIER(): TerminalNode {
		return this.getToken(GPULangParser.IDENTIFIER, 0);
	}
	public attribute_list(): AttributeContext[] {
		return this.getTypedRuleContexts(AttributeContext) as AttributeContext[];
	}
	public attribute(i: number): AttributeContext {
		return this.getTypedRuleContext(AttributeContext, i) as AttributeContext;
	}
	public EQ(): TerminalNode {
		return this.getToken(GPULangParser.EQ, 0);
	}
	public logicalOrExpression(): LogicalOrExpressionContext {
		return this.getTypedRuleContext(LogicalOrExpressionContext, 0) as LogicalOrExpressionContext;
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_parameter;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterParameter) {
	 		listener.enterParameter(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitParameter) {
	 		listener.exitParameter(this);
		}
	}
}


export class FunctionDeclarationContext extends ParserRuleContext {
	public sym: Function*;
	public _attribute!: AttributeContext;
	public _name!: Token;
	public _arg0!: ParameterContext;
	public _argn!: ParameterContext;
	public _returnType!: TypeDeclarationContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public LP(): TerminalNode {
		return this.getToken(GPULangParser.LP, 0);
	}
	public RP(): TerminalNode {
		return this.getToken(GPULangParser.RP, 0);
	}
	public IDENTIFIER(): TerminalNode {
		return this.getToken(GPULangParser.IDENTIFIER, 0);
	}
	public typeDeclaration(): TypeDeclarationContext {
		return this.getTypedRuleContext(TypeDeclarationContext, 0) as TypeDeclarationContext;
	}
	public attribute_list(): AttributeContext[] {
		return this.getTypedRuleContexts(AttributeContext) as AttributeContext[];
	}
	public attribute(i: number): AttributeContext {
		return this.getTypedRuleContext(AttributeContext, i) as AttributeContext;
	}
	public parameter_list(): ParameterContext[] {
		return this.getTypedRuleContexts(ParameterContext) as ParameterContext[];
	}
	public parameter(i: number): ParameterContext {
		return this.getTypedRuleContext(ParameterContext, i) as ParameterContext;
	}
	public CO_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.CO);
	}
	public CO(i: number): TerminalNode {
		return this.getToken(GPULangParser.CO, i);
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_functionDeclaration;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterFunctionDeclaration) {
	 		listener.enterFunctionDeclaration(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitFunctionDeclaration) {
	 		listener.exitFunctionDeclaration(this);
		}
	}
}


export class FunctionContext extends ParserRuleContext {
	public sym: Function*;
	public _functionDeclaration!: FunctionDeclarationContext;
	public _scopeStatement!: ScopeStatementContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public functionDeclaration(): FunctionDeclarationContext {
		return this.getTypedRuleContext(FunctionDeclarationContext, 0) as FunctionDeclarationContext;
	}
	public scopeStatement(): ScopeStatementContext {
		return this.getTypedRuleContext(ScopeStatementContext, 0) as ScopeStatementContext;
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_function;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterFunction) {
	 		listener.enterFunction(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitFunction) {
	 		listener.exitFunction(this);
		}
	}
}


export class ProgramContext extends ParserRuleContext {
	public sym: Program*;
	public _annotation!: AnnotationContext;
	public _name!: Token;
	public _assignment!: ExpressionContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public LB(): TerminalNode {
		return this.getToken(GPULangParser.LB, 0);
	}
	public RB(): TerminalNode {
		return this.getToken(GPULangParser.RB, 0);
	}
	public IDENTIFIER(): TerminalNode {
		return this.getToken(GPULangParser.IDENTIFIER, 0);
	}
	public annotation_list(): AnnotationContext[] {
		return this.getTypedRuleContexts(AnnotationContext) as AnnotationContext[];
	}
	public annotation(i: number): AnnotationContext {
		return this.getTypedRuleContext(AnnotationContext, i) as AnnotationContext;
	}
	public SC_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.SC);
	}
	public SC(i: number): TerminalNode {
		return this.getToken(GPULangParser.SC, i);
	}
	public expression_list(): ExpressionContext[] {
		return this.getTypedRuleContexts(ExpressionContext) as ExpressionContext[];
	}
	public expression(i: number): ExpressionContext {
		return this.getTypedRuleContext(ExpressionContext, i) as ExpressionContext;
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_program;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterProgram) {
	 		listener.enterProgram(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitProgram) {
	 		listener.exitProgram(this);
		}
	}
}


export class SamplerContext extends ParserRuleContext {
	public sym: SamplerState*;
	public _name!: Token;
	public _assign!: ExpressionContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public LB(): TerminalNode {
		return this.getToken(GPULangParser.LB, 0);
	}
	public RB(): TerminalNode {
		return this.getToken(GPULangParser.RB, 0);
	}
	public IDENTIFIER(): TerminalNode {
		return this.getToken(GPULangParser.IDENTIFIER, 0);
	}
	public SC_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.SC);
	}
	public SC(i: number): TerminalNode {
		return this.getToken(GPULangParser.SC, i);
	}
	public expression_list(): ExpressionContext[] {
		return this.getTypedRuleContexts(ExpressionContext) as ExpressionContext[];
	}
	public expression(i: number): ExpressionContext {
		return this.getTypedRuleContext(ExpressionContext, i) as ExpressionContext;
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_sampler;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterSampler) {
	 		listener.enterSampler(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitSampler) {
	 		listener.exitSampler(this);
		}
	}
}


export class StateContext extends ParserRuleContext {
	public sym: State*;
	public _name!: Token;
	public _assign!: ExpressionContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public LB(): TerminalNode {
		return this.getToken(GPULangParser.LB, 0);
	}
	public RB(): TerminalNode {
		return this.getToken(GPULangParser.RB, 0);
	}
	public IDENTIFIER(): TerminalNode {
		return this.getToken(GPULangParser.IDENTIFIER, 0);
	}
	public SC_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.SC);
	}
	public SC(i: number): TerminalNode {
		return this.getToken(GPULangParser.SC, i);
	}
	public expression_list(): ExpressionContext[] {
		return this.getTypedRuleContexts(ExpressionContext) as ExpressionContext[];
	}
	public expression(i: number): ExpressionContext {
		return this.getTypedRuleContext(ExpressionContext, i) as ExpressionContext;
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_state;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterState) {
	 		listener.enterState(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitState) {
	 		listener.exitState(this);
		}
	}
}


export class StatementContext extends ParserRuleContext {
	public tree: Statement*;
	public _ifStatement!: IfStatementContext;
	public _scopeStatement!: ScopeStatementContext;
	public _forStatement!: ForStatementContext;
	public _whileStatement!: WhileStatementContext;
	public _switchStatement!: SwitchStatementContext;
	public _terminateStatement!: TerminateStatementContext;
	public _continueStatement!: ContinueStatementContext;
	public _breakStatement!: BreakStatementContext;
	public _expressionStatement!: ExpressionStatementContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public ifStatement(): IfStatementContext {
		return this.getTypedRuleContext(IfStatementContext, 0) as IfStatementContext;
	}
	public scopeStatement(): ScopeStatementContext {
		return this.getTypedRuleContext(ScopeStatementContext, 0) as ScopeStatementContext;
	}
	public forStatement(): ForStatementContext {
		return this.getTypedRuleContext(ForStatementContext, 0) as ForStatementContext;
	}
	public whileStatement(): WhileStatementContext {
		return this.getTypedRuleContext(WhileStatementContext, 0) as WhileStatementContext;
	}
	public switchStatement(): SwitchStatementContext {
		return this.getTypedRuleContext(SwitchStatementContext, 0) as SwitchStatementContext;
	}
	public terminateStatement(): TerminateStatementContext {
		return this.getTypedRuleContext(TerminateStatementContext, 0) as TerminateStatementContext;
	}
	public continueStatement(): ContinueStatementContext {
		return this.getTypedRuleContext(ContinueStatementContext, 0) as ContinueStatementContext;
	}
	public breakStatement(): BreakStatementContext {
		return this.getTypedRuleContext(BreakStatementContext, 0) as BreakStatementContext;
	}
	public expressionStatement(): ExpressionStatementContext {
		return this.getTypedRuleContext(ExpressionStatementContext, 0) as ExpressionStatementContext;
	}
	public SC(): TerminalNode {
		return this.getToken(GPULangParser.SC, 0);
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_statement;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterStatement) {
	 		listener.enterStatement(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitStatement) {
	 		listener.exitStatement(this);
		}
	}
}


export class ExpressionStatementContext extends ParserRuleContext {
	public tree: Statement*;
	public _expression!: ExpressionContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public expression(): ExpressionContext {
		return this.getTypedRuleContext(ExpressionContext, 0) as ExpressionContext;
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_expressionStatement;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterExpressionStatement) {
	 		listener.enterExpressionStatement(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitExpressionStatement) {
	 		listener.exitExpressionStatement(this);
		}
	}
}


export class IfStatementContext extends ParserRuleContext {
	public tree: Statement*;
	public _condition!: ExpressionContext;
	public _ifBody!: StatementContext;
	public _elseBody!: StatementContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public LP(): TerminalNode {
		return this.getToken(GPULangParser.LP, 0);
	}
	public RP(): TerminalNode {
		return this.getToken(GPULangParser.RP, 0);
	}
	public expression(): ExpressionContext {
		return this.getTypedRuleContext(ExpressionContext, 0) as ExpressionContext;
	}
	public statement_list(): StatementContext[] {
		return this.getTypedRuleContexts(StatementContext) as StatementContext[];
	}
	public statement(i: number): StatementContext {
		return this.getTypedRuleContext(StatementContext, i) as StatementContext;
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_ifStatement;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterIfStatement) {
	 		listener.enterIfStatement(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitIfStatement) {
	 		listener.exitIfStatement(this);
		}
	}
}


export class ForStatementContext extends ParserRuleContext {
	public tree: Statement*;
	public _variables!: VariablesContext;
	public _condition!: ExpressionContext;
	public _loop!: ExpressionContext;
	public _attribute!: AttributeContext;
	public _content!: StatementContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public LP(): TerminalNode {
		return this.getToken(GPULangParser.LP, 0);
	}
	public SC_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.SC);
	}
	public SC(i: number): TerminalNode {
		return this.getToken(GPULangParser.SC, i);
	}
	public RP(): TerminalNode {
		return this.getToken(GPULangParser.RP, 0);
	}
	public statement(): StatementContext {
		return this.getTypedRuleContext(StatementContext, 0) as StatementContext;
	}
	public variables(): VariablesContext {
		return this.getTypedRuleContext(VariablesContext, 0) as VariablesContext;
	}
	public attribute_list(): AttributeContext[] {
		return this.getTypedRuleContexts(AttributeContext) as AttributeContext[];
	}
	public attribute(i: number): AttributeContext {
		return this.getTypedRuleContext(AttributeContext, i) as AttributeContext;
	}
	public expression_list(): ExpressionContext[] {
		return this.getTypedRuleContexts(ExpressionContext) as ExpressionContext[];
	}
	public expression(i: number): ExpressionContext {
		return this.getTypedRuleContext(ExpressionContext, i) as ExpressionContext;
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_forStatement;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterForStatement) {
	 		listener.enterForStatement(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitForStatement) {
	 		listener.exitForStatement(this);
		}
	}
}


export class ForRangeStatementContext extends ParserRuleContext {
	public tree: Statement*;
	public _iterator!: Token;
	public _start!: Token;
	public _end!: Token;
	public _content!: StatementContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public LP(): TerminalNode {
		return this.getToken(GPULangParser.LP, 0);
	}
	public COL(): TerminalNode {
		return this.getToken(GPULangParser.COL, 0);
	}
	public RP(): TerminalNode {
		return this.getToken(GPULangParser.RP, 0);
	}
	public IDENTIFIER_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.IDENTIFIER);
	}
	public IDENTIFIER(i: number): TerminalNode {
		return this.getToken(GPULangParser.IDENTIFIER, i);
	}
	public statement(): StatementContext {
		return this.getTypedRuleContext(StatementContext, 0) as StatementContext;
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_forRangeStatement;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterForRangeStatement) {
	 		listener.enterForRangeStatement(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitForRangeStatement) {
	 		listener.exitForRangeStatement(this);
		}
	}
}


export class WhileStatementContext extends ParserRuleContext {
	public tree: Statement*;
	public _condition!: ExpressionContext;
	public _content!: StatementContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public LP(): TerminalNode {
		return this.getToken(GPULangParser.LP, 0);
	}
	public RP(): TerminalNode {
		return this.getToken(GPULangParser.RP, 0);
	}
	public expression(): ExpressionContext {
		return this.getTypedRuleContext(ExpressionContext, 0) as ExpressionContext;
	}
	public statement(): StatementContext {
		return this.getTypedRuleContext(StatementContext, 0) as StatementContext;
	}
	public SC(): TerminalNode {
		return this.getToken(GPULangParser.SC, 0);
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_whileStatement;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterWhileStatement) {
	 		listener.enterWhileStatement(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitWhileStatement) {
	 		listener.exitWhileStatement(this);
		}
	}
}


export class ScopeStatementContext extends ParserRuleContext {
	public tree: Statement*;
	public _variables!: VariablesContext;
	public _statement!: StatementContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public LB(): TerminalNode {
		return this.getToken(GPULangParser.LB, 0);
	}
	public RB(): TerminalNode {
		return this.getToken(GPULangParser.RB, 0);
	}
	public variables_list(): VariablesContext[] {
		return this.getTypedRuleContexts(VariablesContext) as VariablesContext[];
	}
	public variables(i: number): VariablesContext {
		return this.getTypedRuleContext(VariablesContext, i) as VariablesContext;
	}
	public SC_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.SC);
	}
	public SC(i: number): TerminalNode {
		return this.getToken(GPULangParser.SC, i);
	}
	public statement_list(): StatementContext[] {
		return this.getTypedRuleContexts(StatementContext) as StatementContext[];
	}
	public statement(i: number): StatementContext {
		return this.getTypedRuleContext(StatementContext, i) as StatementContext;
	}
	public linePreprocessorEntry_list(): LinePreprocessorEntryContext[] {
		return this.getTypedRuleContexts(LinePreprocessorEntryContext) as LinePreprocessorEntryContext[];
	}
	public linePreprocessorEntry(i: number): LinePreprocessorEntryContext {
		return this.getTypedRuleContext(LinePreprocessorEntryContext, i) as LinePreprocessorEntryContext;
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_scopeStatement;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterScopeStatement) {
	 		listener.enterScopeStatement(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitScopeStatement) {
	 		listener.exitScopeStatement(this);
		}
	}
}


export class TerminateStatementContext extends ParserRuleContext {
	public tree: Statement*;
	public _value!: ExpressionContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public SC(): TerminalNode {
		return this.getToken(GPULangParser.SC, 0);
	}
	public expression(): ExpressionContext {
		return this.getTypedRuleContext(ExpressionContext, 0) as ExpressionContext;
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_terminateStatement;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterTerminateStatement) {
	 		listener.enterTerminateStatement(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitTerminateStatement) {
	 		listener.exitTerminateStatement(this);
		}
	}
}


export class ContinueStatementContext extends ParserRuleContext {
	public tree: Statement*;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public SC(): TerminalNode {
		return this.getToken(GPULangParser.SC, 0);
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_continueStatement;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterContinueStatement) {
	 		listener.enterContinueStatement(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitContinueStatement) {
	 		listener.exitContinueStatement(this);
		}
	}
}


export class SwitchStatementContext extends ParserRuleContext {
	public tree: Statement*;
	public _expression!: ExpressionContext;
	public _statement!: StatementContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public LP(): TerminalNode {
		return this.getToken(GPULangParser.LP, 0);
	}
	public expression_list(): ExpressionContext[] {
		return this.getTypedRuleContexts(ExpressionContext) as ExpressionContext[];
	}
	public expression(i: number): ExpressionContext {
		return this.getTypedRuleContext(ExpressionContext, i) as ExpressionContext;
	}
	public RP(): TerminalNode {
		return this.getToken(GPULangParser.RP, 0);
	}
	public LB(): TerminalNode {
		return this.getToken(GPULangParser.LB, 0);
	}
	public RB(): TerminalNode {
		return this.getToken(GPULangParser.RB, 0);
	}
	public COL_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.COL);
	}
	public COL(i: number): TerminalNode {
		return this.getToken(GPULangParser.COL, i);
	}
	public statement_list(): StatementContext[] {
		return this.getTypedRuleContexts(StatementContext) as StatementContext[];
	}
	public statement(i: number): StatementContext {
		return this.getTypedRuleContext(StatementContext, i) as StatementContext;
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_switchStatement;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterSwitchStatement) {
	 		listener.enterSwitchStatement(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitSwitchStatement) {
	 		listener.exitSwitchStatement(this);
		}
	}
}


export class BreakStatementContext extends ParserRuleContext {
	public tree: Statement*;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public SC(): TerminalNode {
		return this.getToken(GPULangParser.SC, 0);
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_breakStatement;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterBreakStatement) {
	 		listener.enterBreakStatement(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitBreakStatement) {
	 		listener.exitBreakStatement(this);
		}
	}
}


export class ExpressionContext extends ParserRuleContext {
	public tree: Expression*;
	public _commaExpression!: CommaExpressionContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public commaExpression(): CommaExpressionContext {
		return this.getTypedRuleContext(CommaExpressionContext, 0) as CommaExpressionContext;
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_expression;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterExpression) {
	 		listener.enterExpression(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitExpression) {
	 		listener.exitExpression(this);
		}
	}
}


export class CommaExpressionContext extends ParserRuleContext {
	public tree: Expression*;
	public _e1!: AssignmentExpressionContext;
	public _e2!: AssignmentExpressionContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public assignmentExpression_list(): AssignmentExpressionContext[] {
		return this.getTypedRuleContexts(AssignmentExpressionContext) as AssignmentExpressionContext[];
	}
	public assignmentExpression(i: number): AssignmentExpressionContext {
		return this.getTypedRuleContext(AssignmentExpressionContext, i) as AssignmentExpressionContext;
	}
	public CO_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.CO);
	}
	public CO(i: number): TerminalNode {
		return this.getToken(GPULangParser.CO, i);
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_commaExpression;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterCommaExpression) {
	 		listener.enterCommaExpression(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitCommaExpression) {
	 		listener.exitCommaExpression(this);
		}
	}
}


export class AssignmentExpressionContext extends ParserRuleContext {
	public tree: Expression*;
	public _e1!: LogicalOrExpressionContext;
	public _op!: Token;
	public _e2!: LogicalOrExpressionContext;
	public _ifBody!: ExpressionContext;
	public _elseBody!: ExpressionContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public logicalOrExpression_list(): LogicalOrExpressionContext[] {
		return this.getTypedRuleContexts(LogicalOrExpressionContext) as LogicalOrExpressionContext[];
	}
	public logicalOrExpression(i: number): LogicalOrExpressionContext {
		return this.getTypedRuleContext(LogicalOrExpressionContext, i) as LogicalOrExpressionContext;
	}
	public QU_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.QU);
	}
	public QU(i: number): TerminalNode {
		return this.getToken(GPULangParser.QU, i);
	}
	public COL_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.COL);
	}
	public COL(i: number): TerminalNode {
		return this.getToken(GPULangParser.COL, i);
	}
	public expression_list(): ExpressionContext[] {
		return this.getTypedRuleContexts(ExpressionContext) as ExpressionContext[];
	}
	public expression(i: number): ExpressionContext {
		return this.getTypedRuleContext(ExpressionContext, i) as ExpressionContext;
	}
	public ANDSET_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.ANDSET);
	}
	public ANDSET(i: number): TerminalNode {
		return this.getToken(GPULangParser.ANDSET, i);
	}
	public XORSET_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.XORSET);
	}
	public XORSET(i: number): TerminalNode {
		return this.getToken(GPULangParser.XORSET, i);
	}
	public ORSET_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.ORSET);
	}
	public ORSET(i: number): TerminalNode {
		return this.getToken(GPULangParser.ORSET, i);
	}
	public EQ_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.EQ);
	}
	public EQ(i: number): TerminalNode {
		return this.getToken(GPULangParser.EQ, i);
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_assignmentExpression;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterAssignmentExpression) {
	 		listener.enterAssignmentExpression(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitAssignmentExpression) {
	 		listener.exitAssignmentExpression(this);
		}
	}
}


export class LogicalOrExpressionContext extends ParserRuleContext {
	public tree: Expression*;
	public _e1!: LogicalAndExpressionContext;
	public _e2!: LogicalAndExpressionContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public logicalAndExpression_list(): LogicalAndExpressionContext[] {
		return this.getTypedRuleContexts(LogicalAndExpressionContext) as LogicalAndExpressionContext[];
	}
	public logicalAndExpression(i: number): LogicalAndExpressionContext {
		return this.getTypedRuleContext(LogicalAndExpressionContext, i) as LogicalAndExpressionContext;
	}
	public LOGICOR_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.LOGICOR);
	}
	public LOGICOR(i: number): TerminalNode {
		return this.getToken(GPULangParser.LOGICOR, i);
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_logicalOrExpression;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterLogicalOrExpression) {
	 		listener.enterLogicalOrExpression(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitLogicalOrExpression) {
	 		listener.exitLogicalOrExpression(this);
		}
	}
}


export class LogicalAndExpressionContext extends ParserRuleContext {
	public tree: Expression*;
	public _e1!: OrExpressionContext;
	public _e2!: OrExpressionContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public orExpression_list(): OrExpressionContext[] {
		return this.getTypedRuleContexts(OrExpressionContext) as OrExpressionContext[];
	}
	public orExpression(i: number): OrExpressionContext {
		return this.getTypedRuleContext(OrExpressionContext, i) as OrExpressionContext;
	}
	public LOGICAND_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.LOGICAND);
	}
	public LOGICAND(i: number): TerminalNode {
		return this.getToken(GPULangParser.LOGICAND, i);
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_logicalAndExpression;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterLogicalAndExpression) {
	 		listener.enterLogicalAndExpression(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitLogicalAndExpression) {
	 		listener.exitLogicalAndExpression(this);
		}
	}
}


export class OrExpressionContext extends ParserRuleContext {
	public tree: Expression*;
	public _e1!: XorExpressionContext;
	public _e2!: XorExpressionContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public xorExpression_list(): XorExpressionContext[] {
		return this.getTypedRuleContexts(XorExpressionContext) as XorExpressionContext[];
	}
	public xorExpression(i: number): XorExpressionContext {
		return this.getTypedRuleContext(XorExpressionContext, i) as XorExpressionContext;
	}
	public OR_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.OR);
	}
	public OR(i: number): TerminalNode {
		return this.getToken(GPULangParser.OR, i);
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_orExpression;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterOrExpression) {
	 		listener.enterOrExpression(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitOrExpression) {
	 		listener.exitOrExpression(this);
		}
	}
}


export class XorExpressionContext extends ParserRuleContext {
	public tree: Expression*;
	public _e1!: AndExpressionContext;
	public _e2!: AndExpressionContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public andExpression_list(): AndExpressionContext[] {
		return this.getTypedRuleContexts(AndExpressionContext) as AndExpressionContext[];
	}
	public andExpression(i: number): AndExpressionContext {
		return this.getTypedRuleContext(AndExpressionContext, i) as AndExpressionContext;
	}
	public XOR_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.XOR);
	}
	public XOR(i: number): TerminalNode {
		return this.getToken(GPULangParser.XOR, i);
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_xorExpression;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterXorExpression) {
	 		listener.enterXorExpression(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitXorExpression) {
	 		listener.exitXorExpression(this);
		}
	}
}


export class AndExpressionContext extends ParserRuleContext {
	public tree: Expression*;
	public _e1!: EquivalencyExpressionContext;
	public _e2!: EquivalencyExpressionContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public equivalencyExpression_list(): EquivalencyExpressionContext[] {
		return this.getTypedRuleContexts(EquivalencyExpressionContext) as EquivalencyExpressionContext[];
	}
	public equivalencyExpression(i: number): EquivalencyExpressionContext {
		return this.getTypedRuleContext(EquivalencyExpressionContext, i) as EquivalencyExpressionContext;
	}
	public AND_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.AND);
	}
	public AND(i: number): TerminalNode {
		return this.getToken(GPULangParser.AND, i);
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_andExpression;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterAndExpression) {
	 		listener.enterAndExpression(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitAndExpression) {
	 		listener.exitAndExpression(this);
		}
	}
}


export class EquivalencyExpressionContext extends ParserRuleContext {
	public tree: Expression*;
	public _e1!: RelationalExpressionContext;
	public _op!: Token;
	public _e2!: RelationalExpressionContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public relationalExpression_list(): RelationalExpressionContext[] {
		return this.getTypedRuleContexts(RelationalExpressionContext) as RelationalExpressionContext[];
	}
	public relationalExpression(i: number): RelationalExpressionContext {
		return this.getTypedRuleContext(RelationalExpressionContext, i) as RelationalExpressionContext;
	}
	public LOGICEQ_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.LOGICEQ);
	}
	public LOGICEQ(i: number): TerminalNode {
		return this.getToken(GPULangParser.LOGICEQ, i);
	}
	public NOTEQ_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.NOTEQ);
	}
	public NOTEQ(i: number): TerminalNode {
		return this.getToken(GPULangParser.NOTEQ, i);
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_equivalencyExpression;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterEquivalencyExpression) {
	 		listener.enterEquivalencyExpression(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitEquivalencyExpression) {
	 		listener.exitEquivalencyExpression(this);
		}
	}
}


export class RelationalExpressionContext extends ParserRuleContext {
	public tree: Expression*;
	public _e1!: ShiftExpressionContext;
	public _op!: Token;
	public _e2!: ShiftExpressionContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public shiftExpression_list(): ShiftExpressionContext[] {
		return this.getTypedRuleContexts(ShiftExpressionContext) as ShiftExpressionContext[];
	}
	public shiftExpression(i: number): ShiftExpressionContext {
		return this.getTypedRuleContext(ShiftExpressionContext, i) as ShiftExpressionContext;
	}
	public LESS_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.LESS);
	}
	public LESS(i: number): TerminalNode {
		return this.getToken(GPULangParser.LESS, i);
	}
	public GREATER_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.GREATER);
	}
	public GREATER(i: number): TerminalNode {
		return this.getToken(GPULangParser.GREATER, i);
	}
	public LESSEQ_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.LESSEQ);
	}
	public LESSEQ(i: number): TerminalNode {
		return this.getToken(GPULangParser.LESSEQ, i);
	}
	public GREATEREQ_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.GREATEREQ);
	}
	public GREATEREQ(i: number): TerminalNode {
		return this.getToken(GPULangParser.GREATEREQ, i);
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_relationalExpression;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterRelationalExpression) {
	 		listener.enterRelationalExpression(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitRelationalExpression) {
	 		listener.exitRelationalExpression(this);
		}
	}
}


export class ShiftExpressionContext extends ParserRuleContext {
	public tree: Expression*;
	public _e1!: AddSubtractExpressionContext;
	public _op!: Token;
	public _e2!: AddSubtractExpressionContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public addSubtractExpression_list(): AddSubtractExpressionContext[] {
		return this.getTypedRuleContexts(AddSubtractExpressionContext) as AddSubtractExpressionContext[];
	}
	public addSubtractExpression(i: number): AddSubtractExpressionContext {
		return this.getTypedRuleContext(AddSubtractExpressionContext, i) as AddSubtractExpressionContext;
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_shiftExpression;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterShiftExpression) {
	 		listener.enterShiftExpression(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitShiftExpression) {
	 		listener.exitShiftExpression(this);
		}
	}
}


export class AddSubtractExpressionContext extends ParserRuleContext {
	public tree: Expression*;
	public _e1!: MultiplyDivideExpressionContext;
	public _op!: Token;
	public _e2!: MultiplyDivideExpressionContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public multiplyDivideExpression_list(): MultiplyDivideExpressionContext[] {
		return this.getTypedRuleContexts(MultiplyDivideExpressionContext) as MultiplyDivideExpressionContext[];
	}
	public multiplyDivideExpression(i: number): MultiplyDivideExpressionContext {
		return this.getTypedRuleContext(MultiplyDivideExpressionContext, i) as MultiplyDivideExpressionContext;
	}
	public ADD_OP_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.ADD_OP);
	}
	public ADD_OP(i: number): TerminalNode {
		return this.getToken(GPULangParser.ADD_OP, i);
	}
	public SUB_OP_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.SUB_OP);
	}
	public SUB_OP(i: number): TerminalNode {
		return this.getToken(GPULangParser.SUB_OP, i);
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_addSubtractExpression;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterAddSubtractExpression) {
	 		listener.enterAddSubtractExpression(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitAddSubtractExpression) {
	 		listener.exitAddSubtractExpression(this);
		}
	}
}


export class MultiplyDivideExpressionContext extends ParserRuleContext {
	public tree: Expression*;
	public _e1!: SuffixExpressionContext;
	public _op!: Token;
	public _e2!: SuffixExpressionContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public suffixExpression_list(): SuffixExpressionContext[] {
		return this.getTypedRuleContexts(SuffixExpressionContext) as SuffixExpressionContext[];
	}
	public suffixExpression(i: number): SuffixExpressionContext {
		return this.getTypedRuleContext(SuffixExpressionContext, i) as SuffixExpressionContext;
	}
	public MUL_OP_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.MUL_OP);
	}
	public MUL_OP(i: number): TerminalNode {
		return this.getToken(GPULangParser.MUL_OP, i);
	}
	public DIV_OP_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.DIV_OP);
	}
	public DIV_OP(i: number): TerminalNode {
		return this.getToken(GPULangParser.DIV_OP, i);
	}
	public MOD_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.MOD);
	}
	public MOD(i: number): TerminalNode {
		return this.getToken(GPULangParser.MOD, i);
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_multiplyDivideExpression;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterMultiplyDivideExpression) {
	 		listener.enterMultiplyDivideExpression(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitMultiplyDivideExpression) {
	 		listener.exitMultiplyDivideExpression(this);
		}
	}
}


export class SuffixExpressionContext extends ParserRuleContext {
	public tree: Expression*;
	public _e1!: PrefixExpressionContext;
	public _arg0!: LogicalOrExpressionContext;
	public _argn!: LogicalOrExpressionContext;
	public _e2!: SuffixExpressionContext;
	public _e3!: ExpressionContext;
	public _op!: Token;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public prefixExpression(): PrefixExpressionContext {
		return this.getTypedRuleContext(PrefixExpressionContext, 0) as PrefixExpressionContext;
	}
	public LP_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.LP);
	}
	public LP(i: number): TerminalNode {
		return this.getToken(GPULangParser.LP, i);
	}
	public RP_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.RP);
	}
	public RP(i: number): TerminalNode {
		return this.getToken(GPULangParser.RP, i);
	}
	public DOT_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.DOT);
	}
	public DOT(i: number): TerminalNode {
		return this.getToken(GPULangParser.DOT, i);
	}
	public ARROW_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.ARROW);
	}
	public ARROW(i: number): TerminalNode {
		return this.getToken(GPULangParser.ARROW, i);
	}
	public LL_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.LL);
	}
	public LL(i: number): TerminalNode {
		return this.getToken(GPULangParser.LL, i);
	}
	public RR_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.RR);
	}
	public RR(i: number): TerminalNode {
		return this.getToken(GPULangParser.RR, i);
	}
	public suffixExpression_list(): SuffixExpressionContext[] {
		return this.getTypedRuleContexts(SuffixExpressionContext) as SuffixExpressionContext[];
	}
	public suffixExpression(i: number): SuffixExpressionContext {
		return this.getTypedRuleContext(SuffixExpressionContext, i) as SuffixExpressionContext;
	}
	public logicalOrExpression_list(): LogicalOrExpressionContext[] {
		return this.getTypedRuleContexts(LogicalOrExpressionContext) as LogicalOrExpressionContext[];
	}
	public logicalOrExpression(i: number): LogicalOrExpressionContext {
		return this.getTypedRuleContext(LogicalOrExpressionContext, i) as LogicalOrExpressionContext;
	}
	public expression_list(): ExpressionContext[] {
		return this.getTypedRuleContexts(ExpressionContext) as ExpressionContext[];
	}
	public expression(i: number): ExpressionContext {
		return this.getTypedRuleContext(ExpressionContext, i) as ExpressionContext;
	}
	public CO_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.CO);
	}
	public CO(i: number): TerminalNode {
		return this.getToken(GPULangParser.CO, i);
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_suffixExpression;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterSuffixExpression) {
	 		listener.enterSuffixExpression(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitSuffixExpression) {
	 		listener.exitSuffixExpression(this);
		}
	}
}


export class PrefixExpressionContext extends ParserRuleContext {
	public tree: Expression*;
	public _op!: Token;
	public _e1!: BinaryexpatomContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public binaryexpatom(): BinaryexpatomContext {
		return this.getTypedRuleContext(BinaryexpatomContext, 0) as BinaryexpatomContext;
	}
	public SUB_OP_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.SUB_OP);
	}
	public SUB_OP(i: number): TerminalNode {
		return this.getToken(GPULangParser.SUB_OP, i);
	}
	public ADD_OP_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.ADD_OP);
	}
	public ADD_OP(i: number): TerminalNode {
		return this.getToken(GPULangParser.ADD_OP, i);
	}
	public NOT_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.NOT);
	}
	public NOT(i: number): TerminalNode {
		return this.getToken(GPULangParser.NOT, i);
	}
	public CONJUGATE_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.CONJUGATE);
	}
	public CONJUGATE(i: number): TerminalNode {
		return this.getToken(GPULangParser.CONJUGATE, i);
	}
	public MUL_OP_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.MUL_OP);
	}
	public MUL_OP(i: number): TerminalNode {
		return this.getToken(GPULangParser.MUL_OP, i);
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_prefixExpression;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterPrefixExpression) {
	 		listener.enterPrefixExpression(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitPrefixExpression) {
	 		listener.exitPrefixExpression(this);
		}
	}
}


export class NamespaceExpressionContext extends ParserRuleContext {
	public tree: Expression*;
	public _e1!: BinaryexpatomContext;
	public _e2!: BinaryexpatomContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public binaryexpatom_list(): BinaryexpatomContext[] {
		return this.getTypedRuleContexts(BinaryexpatomContext) as BinaryexpatomContext[];
	}
	public binaryexpatom(i: number): BinaryexpatomContext {
		return this.getTypedRuleContext(BinaryexpatomContext, i) as BinaryexpatomContext;
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_namespaceExpression;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterNamespaceExpression) {
	 		listener.enterNamespaceExpression(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitNamespaceExpression) {
	 		listener.exitNamespaceExpression(this);
		}
	}
}


export class BinaryexpatomContext extends ParserRuleContext {
	public tree: Expression*;
	public _initializerExpression!: InitializerExpressionContext;
	public _arrayInitializerExpression!: ArrayInitializerExpressionContext;
	public _expression!: ExpressionContext;
	public _INTEGERLITERAL!: Token;
	public _UINTEGERLITERAL!: Token;
	public _FLOATLITERAL!: Token;
	public _DOUBLELITERAL!: Token;
	public _HEX!: Token;
	public _string_!: StringContext;
	public _IDENTIFIER!: Token;
	public _boolean_!: BooleanContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public initializerExpression(): InitializerExpressionContext {
		return this.getTypedRuleContext(InitializerExpressionContext, 0) as InitializerExpressionContext;
	}
	public arrayInitializerExpression(): ArrayInitializerExpressionContext {
		return this.getTypedRuleContext(ArrayInitializerExpressionContext, 0) as ArrayInitializerExpressionContext;
	}
	public LP(): TerminalNode {
		return this.getToken(GPULangParser.LP, 0);
	}
	public expression(): ExpressionContext {
		return this.getTypedRuleContext(ExpressionContext, 0) as ExpressionContext;
	}
	public RP(): TerminalNode {
		return this.getToken(GPULangParser.RP, 0);
	}
	public INTEGERLITERAL(): TerminalNode {
		return this.getToken(GPULangParser.INTEGERLITERAL, 0);
	}
	public UINTEGERLITERAL(): TerminalNode {
		return this.getToken(GPULangParser.UINTEGERLITERAL, 0);
	}
	public FLOATLITERAL(): TerminalNode {
		return this.getToken(GPULangParser.FLOATLITERAL, 0);
	}
	public DOUBLELITERAL(): TerminalNode {
		return this.getToken(GPULangParser.DOUBLELITERAL, 0);
	}
	public HEX(): TerminalNode {
		return this.getToken(GPULangParser.HEX, 0);
	}
	public string_(): StringContext {
		return this.getTypedRuleContext(StringContext, 0) as StringContext;
	}
	public IDENTIFIER(): TerminalNode {
		return this.getToken(GPULangParser.IDENTIFIER, 0);
	}
	public boolean_(): BooleanContext {
		return this.getTypedRuleContext(BooleanContext, 0) as BooleanContext;
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_binaryexpatom;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterBinaryexpatom) {
	 		listener.enterBinaryexpatom(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitBinaryexpatom) {
	 		listener.exitBinaryexpatom(this);
		}
	}
}


export class InitializerExpressionContext extends ParserRuleContext {
	public tree: Expression*;
	public _type_!: Token;
	public _arg0!: LogicalOrExpressionContext;
	public _argN!: LogicalOrExpressionContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public LB(): TerminalNode {
		return this.getToken(GPULangParser.LB, 0);
	}
	public RB(): TerminalNode {
		return this.getToken(GPULangParser.RB, 0);
	}
	public IDENTIFIER(): TerminalNode {
		return this.getToken(GPULangParser.IDENTIFIER, 0);
	}
	public logicalOrExpression_list(): LogicalOrExpressionContext[] {
		return this.getTypedRuleContexts(LogicalOrExpressionContext) as LogicalOrExpressionContext[];
	}
	public logicalOrExpression(i: number): LogicalOrExpressionContext {
		return this.getTypedRuleContext(LogicalOrExpressionContext, i) as LogicalOrExpressionContext;
	}
	public CO_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.CO);
	}
	public CO(i: number): TerminalNode {
		return this.getToken(GPULangParser.CO, i);
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_initializerExpression;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterInitializerExpression) {
	 		listener.enterInitializerExpression(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitInitializerExpression) {
	 		listener.exitInitializerExpression(this);
		}
	}
}


export class ArrayInitializerExpressionContext extends ParserRuleContext {
	public tree: Expression*;
	public _arg0!: LogicalOrExpressionContext;
	public _argN!: LogicalOrExpressionContext;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public LL(): TerminalNode {
		return this.getToken(GPULangParser.LL, 0);
	}
	public RR(): TerminalNode {
		return this.getToken(GPULangParser.RR, 0);
	}
	public logicalOrExpression_list(): LogicalOrExpressionContext[] {
		return this.getTypedRuleContexts(LogicalOrExpressionContext) as LogicalOrExpressionContext[];
	}
	public logicalOrExpression(i: number): LogicalOrExpressionContext {
		return this.getTypedRuleContext(LogicalOrExpressionContext, i) as LogicalOrExpressionContext;
	}
	public CO_list(): TerminalNode[] {
	    	return this.getTokens(GPULangParser.CO);
	}
	public CO(i: number): TerminalNode {
		return this.getToken(GPULangParser.CO, i);
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_arrayInitializerExpression;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterArrayInitializerExpression) {
	 		listener.enterArrayInitializerExpression(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitArrayInitializerExpression) {
	 		listener.exitArrayInitializerExpression(this);
		}
	}
}
