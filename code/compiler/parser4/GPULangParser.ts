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
#include "ast/statements/discardstatement.h"
#include "ast/statements/expressionstatement.h"
#include "ast/statements/forstatement.h"
#include "ast/statements/ifstatement.h"
#include "ast/statements/returnstatement.h"
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
	public static readonly SC = 36;
	public static readonly CO = 37;
	public static readonly COL = 38;
	public static readonly LP = 39;
	public static readonly RP = 40;
	public static readonly LB = 41;
	public static readonly RB = 42;
	public static readonly LL = 43;
	public static readonly RR = 44;
	public static readonly DOT = 45;
	public static readonly NOT = 46;
	public static readonly EQ = 47;
	public static readonly QO = 48;
	public static readonly QU = 49;
	public static readonly AND = 50;
	public static readonly ANDSET = 51;
	public static readonly OR = 52;
	public static readonly ORSET = 53;
	public static readonly XOR = 54;
	public static readonly XORSET = 55;
	public static readonly CONJUGATE = 56;
	public static readonly Q = 57;
	public static readonly NU = 58;
	public static readonly FORWARDSLASH = 59;
	public static readonly LESS = 60;
	public static readonly LESSEQ = 61;
	public static readonly GREATER = 62;
	public static readonly GREATEREQ = 63;
	public static readonly LOGICEQ = 64;
	public static readonly NOTEQ = 65;
	public static readonly LOGICAND = 66;
	public static readonly LOGICOR = 67;
	public static readonly MOD = 68;
	public static readonly UNDERSC = 69;
	public static readonly SOBAKA = 70;
	public static readonly ADD_OP = 71;
	public static readonly SUB_OP = 72;
	public static readonly DIV_OP = 73;
	public static readonly MUL_OP = 74;
	public static readonly ARROW = 75;
	public static readonly INTEGERLITERAL = 76;
	public static readonly UINTEGERLITERAL = 77;
	public static readonly COMMENT = 78;
	public static readonly ML_COMMENT = 79;
	public static readonly FLOATLITERAL = 80;
	public static readonly EXPONENT = 81;
	public static readonly DOUBLELITERAL = 82;
	public static readonly HEX = 83;
	public static readonly IDENTIFIER = 84;
	public static readonly WS = 85;
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
	public static readonly RULE_returnStatement = 26;
	public static readonly RULE_discardStatement = 27;
	public static readonly RULE_continueStatement = 28;
	public static readonly RULE_switchStatement = 29;
	public static readonly RULE_breakStatement = 30;
	public static readonly RULE_expression = 31;
	public static readonly RULE_commaExpression = 32;
	public static readonly RULE_assignmentExpression = 33;
	public static readonly RULE_logicalOrExpression = 34;
	public static readonly RULE_logicalAndExpression = 35;
	public static readonly RULE_orExpression = 36;
	public static readonly RULE_xorExpression = 37;
	public static readonly RULE_andExpression = 38;
	public static readonly RULE_equivalencyExpression = 39;
	public static readonly RULE_relationalExpression = 40;
	public static readonly RULE_shiftExpression = 41;
	public static readonly RULE_addSubtractExpression = 42;
	public static readonly RULE_multiplyDivideExpression = 43;
	public static readonly RULE_suffixExpression = 44;
	public static readonly RULE_prefixExpression = 45;
	public static readonly RULE_namespaceExpression = 46;
	public static readonly RULE_binaryexpatom = 47;
	public static readonly RULE_initializerExpression = 48;
	public static readonly RULE_arrayInitializerExpression = 49;
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
		"returnStatement", "discardStatement", "continueStatement", "switchStatement", 
		"breakStatement", "expression", "commaExpression", "assignmentExpression", 
		"logicalOrExpression", "logicalAndExpression", "orExpression", "xorExpression", 
		"andExpression", "equivalencyExpression", "relationalExpression", "shiftExpression", 
		"addSubtractExpression", "multiplyDivideExpression", "suffixExpression", 
		"prefixExpression", "namespaceExpression", "binaryexpatom", "initializerExpression", 
		"arrayInitializerExpression",
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
			this.state = 118;
			this._errHandler.sync(this);
			switch (this._input.LA(1)) {
			case 48:
				this.enterOuterAlt(localctx, 1);
				{
				this.state = 100;
				this.match(GPULangParser.QO);
				this.state = 105;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
				while ((((_la) & ~0x1F) === 0 && ((1 << _la) & 4294967294) !== 0) || ((((_la - 32)) & ~0x1F) === 0 && ((1 << (_la - 32)) & 4294901759) !== 0) || ((((_la - 64)) & ~0x1F) === 0 && ((1 << (_la - 64)) & 4194303) !== 0)) {
					{
					{
					this.state = 101;
					localctx._data = this._input.LT(1);
					_la = this._input.LA(1);
					if(_la<=0 || _la===48) {
					    localctx._data = this._errHandler.recoverInline(this);
					}
					else {
						this._errHandler.reportMatch(this);
					    this.consume();
					}
					 localctx.val.append((localctx._data != null ? localctx._data.text : undefined)); 
					}
					}
					this.state = 107;
					this._errHandler.sync(this);
					_la = this._input.LA(1);
				}
				this.state = 108;
				this.match(GPULangParser.QO);
				}
				break;
			case 57:
				this.enterOuterAlt(localctx, 2);
				{
				this.state = 109;
				this.match(GPULangParser.Q);
				this.state = 114;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
				while ((((_la) & ~0x1F) === 0 && ((1 << _la) & 4294967294) !== 0) || ((((_la - 32)) & ~0x1F) === 0 && ((1 << (_la - 32)) & 4261412863) !== 0) || ((((_la - 64)) & ~0x1F) === 0 && ((1 << (_la - 64)) & 4194303) !== 0)) {
					{
					{
					this.state = 110;
					localctx._data = this._input.LT(1);
					_la = this._input.LA(1);
					if(_la<=0 || _la===57) {
					    localctx._data = this._errHandler.recoverInline(this);
					}
					else {
						this._errHandler.reportMatch(this);
					    this.consume();
					}
					 localctx.val.append((localctx._data != null ? localctx._data.text : undefined)); 
					}
					}
					this.state = 116;
					this._errHandler.sync(this);
					_la = this._input.LA(1);
				}
				this.state = 117;
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
			this.state = 124;
			this._errHandler.sync(this);
			switch (this._input.LA(1)) {
			case 1:
				this.enterOuterAlt(localctx, 1);
				{
				this.state = 120;
				this.match(GPULangParser.T__0);
				 localctx.val =  true; 
				}
				break;
			case 2:
				this.enterOuterAlt(localctx, 2);
				{
				this.state = 122;
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
			this.state = 126;
			localctx._effect = this.effect();

			        localctx.returnEffect =  localctx._effect.eff;
			    
			this.state = 128;
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
			this.state = 168;
			this._errHandler.sync(this);
			_alt = this._interp.adaptivePredict(this._input, 5, this._ctx);
			while (_alt !== 1 && _alt !== ATN.INVALID_ALT_NUMBER) {
				if (_alt === 1 + 1) {
					{
					this.state = 166;
					this._errHandler.sync(this);
					switch ( this._interp.adaptivePredict(this._input, 4, this._ctx) ) {
					case 1:
						{
						this.state = 130;
						this.linePreprocessorEntry();
						}
						break;
					case 2:
						{
						this.state = 131;
						localctx._alias = this.alias();
						this.state = 132;
						this.match(GPULangParser.SC);
						 localctx.eff->symbols.push_back(localctx._alias.sym); 
						}
						break;
					case 3:
						{
						this.state = 135;
						localctx._functionDeclaration = this.functionDeclaration();
						this.state = 136;
						this.match(GPULangParser.SC);
						 localctx.eff->symbols.push_back(localctx._functionDeclaration.sym); 
						}
						break;
					case 4:
						{
						this.state = 139;
						localctx._function_ = this.function_();
						 localctx.eff->symbols.push_back(localctx._function.sym); 
						}
						break;
					case 5:
						{
						this.state = 142;
						localctx._variables = this.variables();
						this.state = 143;
						this.match(GPULangParser.SC);
						 for (Variable* var : localctx._variables.list) { localctx.eff->symbols.push_back(var); } 
						}
						break;
					case 6:
						{
						this.state = 146;
						localctx._structure = this.structure();
						this.state = 147;
						this.match(GPULangParser.SC);
						 localctx.eff->symbols.push_back(localctx._structure.sym); 
						}
						break;
					case 7:
						{
						this.state = 150;
						localctx._enumeration = this.enumeration();
						this.state = 151;
						this.match(GPULangParser.SC);
						 localctx.eff->symbols.push_back(localctx._enumeration.sym); 
						}
						break;
					case 8:
						{
						this.state = 154;
						localctx._state_ = this.state_();
						this.state = 155;
						this.match(GPULangParser.SC);
						 localctx.eff->symbols.push_back(localctx._state.sym); 
						}
						break;
					case 9:
						{
						this.state = 158;
						localctx._sampler = this.sampler();
						this.state = 159;
						this.match(GPULangParser.SC);
						 localctx.eff->symbols.push_back(localctx._sampler.sym); 
						}
						break;
					case 10:
						{
						this.state = 162;
						localctx._program = this.program();
						this.state = 163;
						this.match(GPULangParser.SC);
						 localctx.eff->symbols.push_back(localctx._program.sym); 
						}
						break;
					}
					}
				}
				this.state = 170;
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
			this.state = 171;
			this.match(GPULangParser.T__2);
			 origLine = _input->LT(-1)->getLine(); 
			this.state = 173;
			localctx._line = this.match(GPULangParser.INTEGERLITERAL);
			this.state = 174;
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
			this.state = 177;
			this.match(GPULangParser.T__3);
			this.state = 178;
			localctx._name = this.match(GPULangParser.IDENTIFIER);
			this.state = 179;
			this.match(GPULangParser.T__4);
			this.state = 180;
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
			this.state = 184;
			this.match(GPULangParser.SOBAKA);
			{
			this.state = 185;
			localctx._name = this.match(GPULangParser.IDENTIFIER);
			this.state = 186;
			this.match(GPULangParser.LP);
			this.state = 187;
			localctx._value = this.expression();
			this.state = 188;
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
			this.state = 199;
			this._errHandler.sync(this);
			switch ( this._interp.adaptivePredict(this._input, 6, this._ctx) ) {
			case 1:
				this.enterOuterAlt(localctx, 1);
				{
				this.state = 191;
				localctx._name = this.match(GPULangParser.IDENTIFIER);
				this.state = 192;
				this.match(GPULangParser.LP);
				this.state = 193;
				localctx._expression = this.expression();
				this.state = 194;
				this.match(GPULangParser.RP);
				 localctx.attr.name = (localctx._name != null ? localctx._name.text : undefined); localctx.attr.expression = localctx._expression.tree; 
				}
				break;
			case 2:
				this.enterOuterAlt(localctx, 2);
				{
				this.state = 197;
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
			this.state = 215;
			this._errHandler.sync(this);
			_alt = this._interp.adaptivePredict(this._input, 9, this._ctx);
			while (_alt !== 2 && _alt !== ATN.INVALID_ALT_NUMBER) {
				if (_alt === 1) {
					{
					this.state = 213;
					this._errHandler.sync(this);
					switch (this._input.LA(1)) {
					case 74:
						{
						this.state = 201;
						this.match(GPULangParser.MUL_OP);
						 localctx.type_.AddModifier(Type::FullType::Modifier::Pointer); 
						}
						break;
					case 43:
						{
						this.state = 203;
						this.match(GPULangParser.LL);
						 localctx.type_.AddModifier(Type::FullType::Modifier::Array); 
						this.state = 208;
						this._errHandler.sync(this);
						_la = this._input.LA(1);
						if (_la===1 || _la===2 || ((((_la - 33)) & ~0x1F) === 0 && ((1 << (_la - 33)) & 25207875) !== 0) || ((((_la - 71)) & ~0x1F) === 0 && ((1 << (_la - 71)) & 14955) !== 0)) {
							{
							this.state = 205;
							localctx._arraySize0 = this.expression();
							 localctx.type_.UpdateValue(localctx._arraySize0.tree); 
							}
						}

						this.state = 210;
						this.match(GPULangParser.RR);
						}
						break;
					case 84:
						{
						this.state = 211;
						localctx._IDENTIFIER = this.match(GPULangParser.IDENTIFIER);
						 localctx.type_.AddQualifier((localctx._IDENTIFIER != null ? localctx._IDENTIFIER.text : undefined)); 
						}
						break;
					default:
						throw new NoViableAltException(this);
					}
					}
				}
				this.state = 217;
				this._errHandler.sync(this);
				_alt = this._interp.adaptivePredict(this._input, 9, this._ctx);
			}
			this.state = 218;
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
			this.state = 226;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la===70) {
				{
				{
				this.state = 221;
				localctx._annotation = this.annotation();
				 annotations.push_back(std::move(localctx._annotation.annot)); 
				}
				}
				this.state = 228;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 232;
			this._errHandler.sync(this);
			_alt = 1;
			do {
				switch (_alt) {
				case 1:
					{
					{
					this.state = 229;
					localctx._attribute = this.attribute();
					 attributes.push_back(std::move(localctx._attribute.attr)); 
					}
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				this.state = 234;
				this._errHandler.sync(this);
				_alt = this._interp.adaptivePredict(this._input, 11, this._ctx);
			} while (_alt !== 2 && _alt !== ATN.INVALID_ALT_NUMBER);
			this.state = 236;
			localctx._varName = this.match(GPULangParser.IDENTIFIER);
			 names.push_back((localctx._varName != null ? localctx._varName.text : undefined)); valueExpressions.push_back(nullptr); locations.push_back(SetupFile()); 
			this.state = 243;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la===37) {
				{
				{
				this.state = 238;
				this.match(GPULangParser.CO);
				this.state = 239;
				localctx._varNameN = this.match(GPULangParser.IDENTIFIER);
				 names.push_back((localctx._varNameN != null ? localctx._varNameN.text : undefined)); valueExpressions.push_back(nullptr); locations.push_back(SetupFile()); 
				}
				}
				this.state = 245;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 250;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la===38) {
				{
				this.state = 246;
				this.match(GPULangParser.COL);
				this.state = 247;
				localctx._typeDeclaration = this.typeDeclaration();
				 type = localctx._typeDeclaration.type_; 
				}
			}

			this.state = 264;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la===47) {
				{
				this.state = 252;
				this.match(GPULangParser.EQ);
				this.state = 253;
				localctx._valueExpr = this.logicalOrExpression();
				 if (initCounter < names.size()) { valueExpressions[initCounter++] = localctx._valueExpr.tree; }  
				this.state = 261;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
				while (_la===37) {
					{
					{
					this.state = 255;
					this.match(GPULangParser.CO);
					this.state = 256;
					localctx._valueExprN = this.logicalOrExpression();
					 if (initCounter < names.size()) { valueExpressions[initCounter++] = localctx._valueExprN.tree; }; 
					}
					}
					this.state = 263;
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
			this.state = 273;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la===70) {
				{
				{
				this.state = 268;
				localctx._annotation = this.annotation();
				 annotations.push_back(std::move(localctx._annotation.annot)); 
				}
				}
				this.state = 275;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 281;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la===84) {
				{
				{
				this.state = 276;
				localctx._attribute = this.attribute();
				 attributes.push_back(std::move(localctx._attribute.attr)); 
				}
				}
				this.state = 283;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 284;
			this.match(GPULangParser.T__5);
			this.state = 285;
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
			this.state = 288;
			localctx._structureDeclaration = this.structureDeclaration();
			 localctx.sym =  localctx._structureDeclaration.sym; 
			this.state = 290;
			this.match(GPULangParser.LB);
			this.state = 315;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la===84) {
				{
				{
				this.state = 291;
				localctx._varName = this.match(GPULangParser.IDENTIFIER);
				 varName = (localctx._varName != null ? localctx._varName.text : undefined); varLocation = SetupFile(); 
				this.state = 293;
				this.match(GPULangParser.COL);
				this.state = 306;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
				while (_la===43 || _la===74) {
					{
					this.state = 304;
					this._errHandler.sync(this);
					switch (this._input.LA(1)) {
					case 74:
						{
						this.state = 294;
						this.match(GPULangParser.MUL_OP);
						 varType.AddModifier(Type::FullType::Modifier::Pointer); 
						}
						break;
					case 43:
						{
						this.state = 296;
						this.match(GPULangParser.LL);
						 varType.AddModifier(Type::FullType::Modifier::Array); 
						this.state = 301;
						this._errHandler.sync(this);
						_la = this._input.LA(1);
						if (_la===1 || _la===2 || ((((_la - 33)) & ~0x1F) === 0 && ((1 << (_la - 33)) & 25207875) !== 0) || ((((_la - 71)) & ~0x1F) === 0 && ((1 << (_la - 71)) & 14955) !== 0)) {
							{
							this.state = 298;
							localctx._arraySize0 = this.expression();
							 varType.UpdateValue(localctx._arraySize0.tree); 
							}
						}

						this.state = 303;
						this.match(GPULangParser.RR);
						}
						break;
					default:
						throw new NoViableAltException(this);
					}
					}
					this.state = 308;
					this._errHandler.sync(this);
					_la = this._input.LA(1);
				}
				this.state = 309;
				localctx._varTypeName = this.match(GPULangParser.IDENTIFIER);
				 varType.name = (localctx._varTypeName != null ? localctx._varTypeName.text : undefined); 
				this.state = 311;
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
				this.state = 317;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 318;
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
			this.state = 321;
			this.match(GPULangParser.T__6);
			this.state = 322;
			localctx._name = this.match(GPULangParser.IDENTIFIER);
			 name = (localctx._name != null ? localctx._name.text : undefined); location = SetupFile(); 
			this.state = 328;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la===38) {
				{
				this.state = 324;
				this.match(GPULangParser.COL);
				this.state = 325;
				localctx._typeDeclaration = this.typeDeclaration();
				 type = localctx._typeDeclaration.type_; 
				}
			}

			this.state = 330;
			this.match(GPULangParser.LB);
			this.state = 355;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la===84) {
				{
				this.state = 331;
				localctx._label = this.match(GPULangParser.IDENTIFIER);
				 Expression* expr = nullptr; 
				this.state = 337;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
				if (_la===47) {
					{
					this.state = 333;
					this.match(GPULangParser.EQ);
					this.state = 334;
					localctx._value = this.expression();
					 expr = localctx._value.tree; 
					}
				}


				                enumLabels.push_back((localctx._label != null ? localctx._label.text : undefined));
				                enumValues.push_back(expr);
				            
				this.state = 352;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
				while (_la===37) {
					{
					{
					this.state = 340;
					this.match(GPULangParser.CO);
					this.state = 341;
					localctx._label = this.match(GPULangParser.IDENTIFIER);
					 Expression* expr = nullptr; 
					this.state = 347;
					this._errHandler.sync(this);
					_la = this._input.LA(1);
					if (_la===47) {
						{
						this.state = 343;
						this.match(GPULangParser.EQ);
						this.state = 344;
						localctx._value = this.expression();
						 expr = localctx._value.tree; 
						}
					}


					                    enumLabels.push_back((localctx._label != null ? localctx._label.text : undefined));
					                    enumValues.push_back(expr);
					                
					}
					}
					this.state = 354;
					this._errHandler.sync(this);
					_la = this._input.LA(1);
				}
				}
			}

			this.state = 357;
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
			this.state = 365;
			this._errHandler.sync(this);
			_alt = this._interp.adaptivePredict(this._input, 27, this._ctx);
			while (_alt !== 2 && _alt !== ATN.INVALID_ALT_NUMBER) {
				if (_alt === 1) {
					{
					{
					this.state = 360;
					localctx._attribute = this.attribute();
					 attributes.push_back(std::move(localctx._attribute.attr)); 
					}
					}
				}
				this.state = 367;
				this._errHandler.sync(this);
				_alt = this._interp.adaptivePredict(this._input, 27, this._ctx);
			}
			this.state = 368;
			localctx._varName = this.match(GPULangParser.IDENTIFIER);
			 name = (localctx._varName != null ? localctx._varName.text : undefined); location = SetupFile(); 
			this.state = 370;
			this.match(GPULangParser.COL);
			this.state = 371;
			localctx._typeDeclaration = this.typeDeclaration();
			 type = localctx._typeDeclaration.type_; 
			this.state = 377;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la===47) {
				{
				this.state = 373;
				this.match(GPULangParser.EQ);
				this.state = 374;
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
			this.state = 386;
			this._errHandler.sync(this);
			_alt = this._interp.adaptivePredict(this._input, 29, this._ctx);
			while (_alt !== 2 && _alt !== ATN.INVALID_ALT_NUMBER) {
				if (_alt === 1) {
					{
					{
					this.state = 381;
					localctx._attribute = this.attribute();
					 attributes.push_back(std::move(localctx._attribute.attr)); 
					}
					}
				}
				this.state = 388;
				this._errHandler.sync(this);
				_alt = this._interp.adaptivePredict(this._input, 29, this._ctx);
			}
			this.state = 389;
			localctx._name = this.match(GPULangParser.IDENTIFIER);
			 location = SetupFile(); 
			this.state = 391;
			this.match(GPULangParser.LP);
			this.state = 403;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la===84) {
				{
				this.state = 392;
				localctx._arg0 = this.parameter();
				 variables.push_back(localctx._arg0.sym); 
				this.state = 400;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
				while (_la===37) {
					{
					{
					this.state = 394;
					this.match(GPULangParser.CO);
					this.state = 395;
					localctx._argn = this.parameter();
					 variables.push_back(localctx._argn.sym); 
					}
					}
					this.state = 402;
					this._errHandler.sync(this);
					_la = this._input.LA(1);
				}
				}
			}

			this.state = 405;
			this.match(GPULangParser.RP);
			this.state = 406;
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
			this.state = 409;
			localctx._functionDeclaration = this.functionDeclaration();
			 localctx.sym =  localctx._functionDeclaration.sym; 

			        startToken = _input->LT(2);
			    
			this.state = 412;
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
			this.state = 420;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la===70) {
				{
				{
				this.state = 415;
				localctx._annotation = this.annotation();
				 annotations.push_back(std::move(localctx._annotation.annot)); 
				}
				}
				this.state = 422;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 423;
			this.match(GPULangParser.T__7);
			this.state = 424;
			localctx._name = this.match(GPULangParser.IDENTIFIER);
			 location = SetupFile(); 
			this.state = 426;
			this.match(GPULangParser.LB);
			this.state = 433;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la===1 || _la===2 || ((((_la - 33)) & ~0x1F) === 0 && ((1 << (_la - 33)) & 25207875) !== 0) || ((((_la - 71)) & ~0x1F) === 0 && ((1 << (_la - 71)) & 14955) !== 0)) {
				{
				{
				this.state = 427;
				localctx._assignment = this.expression();
				 entries.push_back(localctx._assignment.tree); 
				this.state = 429;
				this.match(GPULangParser.SC);
				}
				}
				this.state = 435;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 436;
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
			this.state = 439;
			this.match(GPULangParser.T__8);
			 localctx.sym =  Alloc<SamplerState>(); localctx.sym->isImmutable = true; 
			}
			this.state = 442;
			localctx._name = this.match(GPULangParser.IDENTIFIER);
			 localctx.sym->location = SetupFile(); 
			this.state = 444;
			this.match(GPULangParser.LB);
			this.state = 451;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la===1 || _la===2 || ((((_la - 33)) & ~0x1F) === 0 && ((1 << (_la - 33)) & 25207875) !== 0) || ((((_la - 71)) & ~0x1F) === 0 && ((1 << (_la - 71)) & 14955) !== 0)) {
				{
				{
				this.state = 445;
				localctx._assign = this.expression();
				 entries.push_back(localctx._assign.tree); 
				this.state = 447;
				this.match(GPULangParser.SC);
				}
				}
				this.state = 453;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 454;
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
			this.state = 457;
			this.match(GPULangParser.T__9);
			 localctx.sym =  Alloc<RenderState>(); 
			}
			this.state = 460;
			localctx._name = this.match(GPULangParser.IDENTIFIER);
			 localctx.sym->location = SetupFile(); 
			this.state = 462;
			this.match(GPULangParser.LB);
			this.state = 469;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la===1 || _la===2 || ((((_la - 33)) & ~0x1F) === 0 && ((1 << (_la - 33)) & 25207875) !== 0) || ((((_la - 71)) & ~0x1F) === 0 && ((1 << (_la - 71)) & 14955) !== 0)) {
				{
				{
				this.state = 463;
				localctx._assign = this.expression();
				 entries.push_back(localctx._assign.tree); 
				this.state = 465;
				this.match(GPULangParser.SC);
				}
				}
				this.state = 471;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 472;
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
			this.state = 506;
			this._errHandler.sync(this);
			switch (this._input.LA(1)) {
			case 11:
				this.enterOuterAlt(localctx, 1);
				{
				this.state = 475;
				localctx._ifStatement = this.ifStatement();
				 localctx.tree =  localctx._ifStatement.tree; 
				}
				break;
			case 41:
				this.enterOuterAlt(localctx, 2);
				{
				this.state = 478;
				localctx._scopeStatement = this.scopeStatement();
				 localctx.tree =  localctx._scopeStatement.tree; 
				}
				break;
			case 13:
				this.enterOuterAlt(localctx, 3);
				{
				this.state = 481;
				localctx._forStatement = this.forStatement();
				 localctx.tree =  localctx._forStatement.tree; 
				}
				break;
			case 15:
			case 16:
				this.enterOuterAlt(localctx, 4);
				{
				this.state = 484;
				localctx._whileStatement = this.whileStatement();
				 localctx.tree =  localctx._whileStatement.tree; 
				}
				break;
			case 20:
				this.enterOuterAlt(localctx, 5);
				{
				this.state = 487;
				localctx._switchStatement = this.switchStatement();
				 localctx.tree =  localctx._switchStatement.tree; 
				}
				break;
			case 17:
				this.enterOuterAlt(localctx, 6);
				{
				this.state = 490;
				localctx._returnStatement = this.returnStatement();
				 localctx.tree =  localctx._returnStatement.tree; 
				}
				break;
			case 18:
				this.enterOuterAlt(localctx, 7);
				{
				this.state = 493;
				localctx._discardStatement = this.discardStatement();
				 localctx.tree =  localctx._discardStatement.tree; 
				}
				break;
			case 19:
				this.enterOuterAlt(localctx, 8);
				{
				this.state = 496;
				localctx._continueStatement = this.continueStatement();
				 localctx.tree =  localctx._continueStatement.tree; 
				}
				break;
			case 23:
				this.enterOuterAlt(localctx, 9);
				{
				this.state = 499;
				localctx._breakStatement = this.breakStatement();
				 localctx.tree =  localctx._breakStatement.tree; 
				}
				break;
			case 1:
			case 2:
			case 33:
			case 34:
			case 39:
			case 43:
			case 46:
			case 48:
			case 56:
			case 57:
			case 71:
			case 72:
			case 74:
			case 76:
			case 77:
			case 80:
			case 82:
			case 83:
			case 84:
				this.enterOuterAlt(localctx, 10);
				{
				this.state = 502;
				localctx._expressionStatement = this.expressionStatement();
				this.state = 503;
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
			this.state = 508;
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
			this.state = 511;
			this.match(GPULangParser.T__10);
			 location = SetupFile(); 
			this.state = 513;
			this.match(GPULangParser.LP);
			this.state = 514;
			localctx._condition = this.expression();
			 condition = localctx._condition.tree; 
			this.state = 516;
			this.match(GPULangParser.RP);
			this.state = 517;
			localctx._ifBody = this.statement();
			 ifBody = localctx._ifBody.tree; 
			this.state = 523;
			this._errHandler.sync(this);
			switch ( this._interp.adaptivePredict(this._input, 37, this._ctx) ) {
			case 1:
				{
				this.state = 519;
				this.match(GPULangParser.T__11);
				this.state = 520;
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
			this.state = 527;
			this.match(GPULangParser.T__12);
			 location = SetupFile(); 
			this.state = 529;
			this.match(GPULangParser.LP);
			this.state = 533;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la===70 || _la===84) {
				{
				this.state = 530;
				localctx._variables = this.variables();
				 declarations = localctx._variables.list; 
				}
			}

			this.state = 535;
			this.match(GPULangParser.SC);
			this.state = 539;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la===1 || _la===2 || ((((_la - 33)) & ~0x1F) === 0 && ((1 << (_la - 33)) & 25207875) !== 0) || ((((_la - 71)) & ~0x1F) === 0 && ((1 << (_la - 71)) & 14955) !== 0)) {
				{
				this.state = 536;
				localctx._condition = this.expression();
				 conditionExpression = localctx._condition.tree; 
				}
			}

			this.state = 541;
			this.match(GPULangParser.SC);
			this.state = 545;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la===1 || _la===2 || ((((_la - 33)) & ~0x1F) === 0 && ((1 << (_la - 33)) & 25207875) !== 0) || ((((_la - 71)) & ~0x1F) === 0 && ((1 << (_la - 71)) & 14955) !== 0)) {
				{
				this.state = 542;
				localctx._loop = this.expression();
				 loopExpression = localctx._loop.tree; 
				}
			}

			this.state = 547;
			this.match(GPULangParser.RP);
			this.state = 553;
			this._errHandler.sync(this);
			_alt = this._interp.adaptivePredict(this._input, 41, this._ctx);
			while (_alt !== 2 && _alt !== ATN.INVALID_ALT_NUMBER) {
				if (_alt === 1) {
					{
					{
					this.state = 548;
					localctx._attribute = this.attribute();
					 attributes.push_back(std::move(localctx._attribute.attr)); 
					}
					}
				}
				this.state = 555;
				this._errHandler.sync(this);
				_alt = this._interp.adaptivePredict(this._input, 41, this._ctx);
			}
			this.state = 556;
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
			this.state = 560;
			this.match(GPULangParser.T__12);
			 location = SetupFile(); 
			this.state = 562;
			this.match(GPULangParser.LP);
			this.state = 563;
			localctx._iterator = this.match(GPULangParser.IDENTIFIER);
			this.state = 564;
			this.match(GPULangParser.COL);
			this.state = 565;
			localctx._start = this.match(GPULangParser.IDENTIFIER);
			this.state = 566;
			this.match(GPULangParser.T__13);
			this.state = 567;
			localctx._end = this.match(GPULangParser.IDENTIFIER);
			this.state = 568;
			this.match(GPULangParser.RP);
			this.state = 569;
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
			this.state = 595;
			this._errHandler.sync(this);
			switch (this._input.LA(1)) {
			case 15:
				this.enterOuterAlt(localctx, 1);
				{
				this.state = 573;
				this.match(GPULangParser.T__14);
				 location = SetupFile(); 
				this.state = 575;
				this.match(GPULangParser.LP);
				this.state = 576;
				localctx._condition = this.expression();
				 conditionExpression = localctx._condition.tree; 
				this.state = 578;
				this.match(GPULangParser.RP);
				this.state = 579;
				localctx._content = this.statement();
				 contents = localctx._content.tree; 

				        localctx.tree =  Alloc<WhileStatement>(conditionExpression, contents, isDoWhile);
				        localctx.tree->location = location;
				    
				}
				break;
			case 16:
				this.enterOuterAlt(localctx, 2);
				{
				this.state = 583;
				this.match(GPULangParser.T__15);
				 location = SetupFile(); 
				this.state = 585;
				localctx._content = this.statement();
				 contents = localctx._content.tree; isDoWhile = true; 
				this.state = 587;
				this.match(GPULangParser.T__14);
				this.state = 588;
				this.match(GPULangParser.LP);
				this.state = 589;
				localctx._condition = this.expression();
				 conditionExpression = localctx._condition.tree; 
				this.state = 591;
				this.match(GPULangParser.RP);
				this.state = 592;
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
			this.state = 597;
			this.match(GPULangParser.LB);
			 location = SetupFile(); 
			this.state = 609;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while ((((_la) & ~0x1F) === 0 && ((1 << _la) & 10463246) !== 0) || ((((_la - 33)) & ~0x1F) === 0 && ((1 << (_la - 33)) & 25208131) !== 0) || ((((_la - 70)) & ~0x1F) === 0 && ((1 << (_la - 70)) & 29911) !== 0)) {
				{
				this.state = 607;
				this._errHandler.sync(this);
				switch ( this._interp.adaptivePredict(this._input, 43, this._ctx) ) {
				case 1:
					{
					this.state = 599;
					localctx._variables = this.variables();
					this.state = 600;
					this.match(GPULangParser.SC);
					 for(Variable* var : localctx._variables.list) { contents.push_back(var); } 
					}
					break;
				case 2:
					{
					this.state = 603;
					localctx._statement = this.statement();
					 contents.push_back(localctx._statement.tree); 
					}
					break;
				case 3:
					{
					this.state = 606;
					this.linePreprocessorEntry();
					}
					break;
				}
				}
				this.state = 611;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 612;
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
	public returnStatement(): ReturnStatementContext {
		let localctx: ReturnStatementContext = new ReturnStatementContext(this, this._ctx, this.state);
		this.enterRule(localctx, 52, GPULangParser.RULE_returnStatement);

		        localctx.tree =  nullptr;
		        Expression* returnValue = nullptr;
		        Symbol::Location location;
		    
		let _la: number;
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 615;
			this.match(GPULangParser.T__16);
			 location = SetupFile(); 
			this.state = 620;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la===1 || _la===2 || ((((_la - 33)) & ~0x1F) === 0 && ((1 << (_la - 33)) & 25207875) !== 0) || ((((_la - 71)) & ~0x1F) === 0 && ((1 << (_la - 71)) & 14955) !== 0)) {
				{
				this.state = 617;
				localctx._value = this.expression();
				 returnValue = localctx._value.tree; 
				}
			}

			this.state = 622;
			this.match(GPULangParser.SC);

			        localctx.tree =  Alloc<ReturnStatement>(returnValue);
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
	public discardStatement(): DiscardStatementContext {
		let localctx: DiscardStatementContext = new DiscardStatementContext(this, this._ctx, this.state);
		this.enterRule(localctx, 54, GPULangParser.RULE_discardStatement);

		        localctx.tree =  nullptr;
		        Symbol::Location location;
		    
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 625;
			this.match(GPULangParser.T__17);
			 location = SetupFile(); 
			this.state = 627;
			this.match(GPULangParser.SC);

			        localctx.tree =  Alloc<DiscardStatement>();
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
	public continueStatement(): ContinueStatementContext {
		let localctx: ContinueStatementContext = new ContinueStatementContext(this, this._ctx, this.state);
		this.enterRule(localctx, 56, GPULangParser.RULE_continueStatement);

		        localctx.tree =  nullptr;
		        Symbol::Location location;
		    
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 630;
			this.match(GPULangParser.T__18);
			 location = SetupFile(); 
			this.state = 632;
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
		this.enterRule(localctx, 58, GPULangParser.RULE_switchStatement);

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
			this.state = 635;
			this.match(GPULangParser.T__19);
			 location = SetupFile(); 
			this.state = 637;
			this.match(GPULangParser.LP);
			this.state = 638;
			localctx._expression = this.expression();
			this.state = 639;
			this.match(GPULangParser.RP);
			 switchExpression = localctx._expression.tree; 
			this.state = 641;
			this.match(GPULangParser.LB);
			this.state = 653;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la===21) {
				{
				{
				this.state = 642;
				this.match(GPULangParser.T__20);
				this.state = 643;
				localctx._expression = this.expression();
				this.state = 644;
				this.match(GPULangParser.COL);
				 caseExpressions.push_back(localctx._expression.tree); caseStatements.push_back(nullptr); 
				this.state = 649;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
				if ((((_la) & ~0x1F) === 0 && ((1 << _la) & 10463238) !== 0) || ((((_la - 33)) & ~0x1F) === 0 && ((1 << (_la - 33)) & 25208131) !== 0) || ((((_la - 71)) & ~0x1F) === 0 && ((1 << (_la - 71)) & 14955) !== 0)) {
					{
					this.state = 646;
					localctx._statement = this.statement();
					 
					                    caseStatements.back() = localctx._statement.tree;
					                
					}
				}

				}
				}
				this.state = 655;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 663;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la===22) {
				{
				this.state = 656;
				this.match(GPULangParser.T__21);
				this.state = 657;
				this.match(GPULangParser.COL);
				this.state = 661;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
				if ((((_la) & ~0x1F) === 0 && ((1 << _la) & 10463238) !== 0) || ((((_la - 33)) & ~0x1F) === 0 && ((1 << (_la - 33)) & 25208131) !== 0) || ((((_la - 71)) & ~0x1F) === 0 && ((1 << (_la - 71)) & 14955) !== 0)) {
					{
					this.state = 658;
					localctx._statement = this.statement();

					                    defaultStatement = localctx._statement.tree;
					                
					}
				}

				}
			}

			this.state = 665;
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
		this.enterRule(localctx, 60, GPULangParser.RULE_breakStatement);

		        localctx.tree =  nullptr;
		        Symbol::Location location;
		    
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 668;
			this.match(GPULangParser.T__22);
			 location = SetupFile(); 
			this.state = 670;
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
		this.enterRule(localctx, 62, GPULangParser.RULE_expression);

		        localctx.tree =  nullptr;
		    
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 673;
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
		this.enterRule(localctx, 64, GPULangParser.RULE_commaExpression);

		        localctx.tree =  nullptr;
		        Symbol::Location location;
		    
		try {
			let _alt: number;
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 676;
			localctx._e1 = this.assignmentExpression();
			 localctx.tree =  localctx._e1.tree; 
			this.state = 685;
			this._errHandler.sync(this);
			_alt = this._interp.adaptivePredict(this._input, 50, this._ctx);
			while (_alt !== 2 && _alt !== ATN.INVALID_ALT_NUMBER) {
				if (_alt === 1) {
					{
					{
					this.state = 678;
					this.match(GPULangParser.CO);
					 location = SetupFile(); 
					this.state = 680;
					localctx._e2 = this.assignmentExpression();

					            CommaExpression* expr = Alloc<CommaExpression>(localctx.tree, localctx._e2.tree);
					            expr->location = location;
					            localctx.tree =  expr;
					        
					}
					}
				}
				this.state = 687;
				this._errHandler.sync(this);
				_alt = this._interp.adaptivePredict(this._input, 50, this._ctx);
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
		this.enterRule(localctx, 66, GPULangParser.RULE_assignmentExpression);

		        localctx.tree =  nullptr;
		        Symbol::Location location;
		    
		let _la: number;
		try {
			let _alt: number;
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 688;
			localctx._e1 = this.logicalOrExpression();
			 localctx.tree =  localctx._e1.tree; 
			this.state = 704;
			this._errHandler.sync(this);
			_alt = this._interp.adaptivePredict(this._input, 52, this._ctx);
			while (_alt !== 2 && _alt !== ATN.INVALID_ALT_NUMBER) {
				if (_alt === 1) {
					{
					this.state = 702;
					this._errHandler.sync(this);
					switch (this._input.LA(1)) {
					case 24:
					case 25:
					case 26:
					case 27:
					case 28:
					case 29:
					case 30:
					case 47:
					case 51:
					case 53:
					case 55:
						{
						this.state = 690;
						localctx._op = this._input.LT(1);
						_la = this._input.LA(1);
						if(!(((((_la - 24)) & ~0x1F) === 0 && ((1 << (_la - 24)) & 2826961023) !== 0))) {
						    localctx._op = this._errHandler.recoverInline(this);
						}
						else {
							this._errHandler.reportMatch(this);
						    this.consume();
						}
						 location = SetupFile(); 
						this.state = 692;
						localctx._e2 = this.logicalOrExpression();
						 
						            BinaryExpression* expr = Alloc<BinaryExpression>(StringToFourCC((localctx._op != null ? localctx._op.text : undefined)), localctx.tree, localctx._e2.tree);
						            expr->location = location;
						            localctx.tree =  expr;
						        
						}
						break;
					case 49:
						{
						this.state = 695;
						this.match(GPULangParser.QU);
						 location = SetupFile(); 
						this.state = 697;
						localctx._ifBody = this.expression();
						this.state = 698;
						this.match(GPULangParser.COL);
						this.state = 699;
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
				this.state = 706;
				this._errHandler.sync(this);
				_alt = this._interp.adaptivePredict(this._input, 52, this._ctx);
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
		this.enterRule(localctx, 68, GPULangParser.RULE_logicalOrExpression);

		        localctx.tree =  nullptr;
		        Symbol::Location location;
		    
		try {
			let _alt: number;
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 707;
			localctx._e1 = this.logicalAndExpression();
			 localctx.tree =  localctx._e1.tree; 
			this.state = 716;
			this._errHandler.sync(this);
			_alt = this._interp.adaptivePredict(this._input, 53, this._ctx);
			while (_alt !== 2 && _alt !== ATN.INVALID_ALT_NUMBER) {
				if (_alt === 1) {
					{
					{
					{
					this.state = 709;
					this.match(GPULangParser.LOGICOR);
					}
					 location = SetupFile(); 
					this.state = 711;
					localctx._e2 = this.logicalAndExpression();

					            BinaryExpression* expr = Alloc<BinaryExpression>('||', localctx.tree, localctx._e2.tree);
					            expr->location = location;
					            localctx.tree =  expr;
					        
					}
					}
				}
				this.state = 718;
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
	public logicalAndExpression(): LogicalAndExpressionContext {
		let localctx: LogicalAndExpressionContext = new LogicalAndExpressionContext(this, this._ctx, this.state);
		this.enterRule(localctx, 70, GPULangParser.RULE_logicalAndExpression);

		        localctx.tree =  nullptr;
		        Symbol::Location location;

		    
		try {
			let _alt: number;
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 719;
			localctx._e1 = this.orExpression();
			 localctx.tree =  localctx._e1.tree; 
			this.state = 728;
			this._errHandler.sync(this);
			_alt = this._interp.adaptivePredict(this._input, 54, this._ctx);
			while (_alt !== 2 && _alt !== ATN.INVALID_ALT_NUMBER) {
				if (_alt === 1) {
					{
					{
					{
					this.state = 721;
					this.match(GPULangParser.LOGICAND);
					}
					 location = SetupFile(); 
					this.state = 723;
					localctx._e2 = this.orExpression();

					            BinaryExpression* expr = Alloc<BinaryExpression>('&&', localctx.tree, localctx._e2.tree);
					            expr->location = location;
					            localctx.tree =  expr;
					        
					}
					}
				}
				this.state = 730;
				this._errHandler.sync(this);
				_alt = this._interp.adaptivePredict(this._input, 54, this._ctx);
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
		this.enterRule(localctx, 72, GPULangParser.RULE_orExpression);

		        localctx.tree =  nullptr;
		        Symbol::Location location;

		    
		try {
			let _alt: number;
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 731;
			localctx._e1 = this.xorExpression();
			 localctx.tree =  localctx._e1.tree; 
			this.state = 740;
			this._errHandler.sync(this);
			_alt = this._interp.adaptivePredict(this._input, 55, this._ctx);
			while (_alt !== 2 && _alt !== ATN.INVALID_ALT_NUMBER) {
				if (_alt === 1) {
					{
					{
					this.state = 733;
					this.match(GPULangParser.OR);
					 location = SetupFile(); 
					this.state = 735;
					localctx._e2 = this.xorExpression();

					            BinaryExpression* expr = Alloc<BinaryExpression>('|', localctx.tree, localctx._e2.tree);
					            expr->location = location;
					            localctx.tree =  expr;
					        
					}
					}
				}
				this.state = 742;
				this._errHandler.sync(this);
				_alt = this._interp.adaptivePredict(this._input, 55, this._ctx);
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
		this.enterRule(localctx, 74, GPULangParser.RULE_xorExpression);

		        localctx.tree =  nullptr;
		        Symbol::Location location;
		    
		try {
			let _alt: number;
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 743;
			localctx._e1 = this.andExpression();
			 localctx.tree =  localctx._e1.tree; 
			this.state = 752;
			this._errHandler.sync(this);
			_alt = this._interp.adaptivePredict(this._input, 56, this._ctx);
			while (_alt !== 2 && _alt !== ATN.INVALID_ALT_NUMBER) {
				if (_alt === 1) {
					{
					{
					this.state = 745;
					this.match(GPULangParser.XOR);
					 location = SetupFile(); 
					this.state = 747;
					localctx._e2 = this.andExpression();

					            BinaryExpression* expr = Alloc<BinaryExpression>('^', localctx.tree, localctx._e2.tree);
					            expr->location = location;
					            localctx.tree =  expr;
					        
					}
					}
				}
				this.state = 754;
				this._errHandler.sync(this);
				_alt = this._interp.adaptivePredict(this._input, 56, this._ctx);
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
		this.enterRule(localctx, 76, GPULangParser.RULE_andExpression);

		        localctx.tree =  nullptr;
		        Symbol::Location location;
		    
		try {
			let _alt: number;
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 755;
			localctx._e1 = this.equivalencyExpression();
			 localctx.tree =  localctx._e1.tree;	
			this.state = 764;
			this._errHandler.sync(this);
			_alt = this._interp.adaptivePredict(this._input, 57, this._ctx);
			while (_alt !== 2 && _alt !== ATN.INVALID_ALT_NUMBER) {
				if (_alt === 1) {
					{
					{
					this.state = 757;
					this.match(GPULangParser.AND);
					 location = SetupFile(); 
					this.state = 759;
					localctx._e2 = this.equivalencyExpression();

					            BinaryExpression* expr = Alloc<BinaryExpression>('&', localctx.tree, localctx._e2.tree);
					            expr->location = location;
					            localctx.tree =  expr;
					        
					}
					}
				}
				this.state = 766;
				this._errHandler.sync(this);
				_alt = this._interp.adaptivePredict(this._input, 57, this._ctx);
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
		this.enterRule(localctx, 78, GPULangParser.RULE_equivalencyExpression);

		        localctx.tree =  nullptr;
		        Symbol::Location location;
		    
		let _la: number;
		try {
			let _alt: number;
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 767;
			localctx._e1 = this.relationalExpression();
			 localctx.tree =  localctx._e1.tree; 
			this.state = 776;
			this._errHandler.sync(this);
			_alt = this._interp.adaptivePredict(this._input, 58, this._ctx);
			while (_alt !== 2 && _alt !== ATN.INVALID_ALT_NUMBER) {
				if (_alt === 1) {
					{
					{
					this.state = 769;
					localctx._op = this._input.LT(1);
					_la = this._input.LA(1);
					if(!(_la===64 || _la===65)) {
					    localctx._op = this._errHandler.recoverInline(this);
					}
					else {
						this._errHandler.reportMatch(this);
					    this.consume();
					}
					 location = SetupFile(); 
					this.state = 771;
					localctx._e2 = this.relationalExpression();

					            BinaryExpression* expr = Alloc<BinaryExpression>(StringToFourCC((localctx._op != null ? localctx._op.text : undefined)), localctx.tree, localctx._e2.tree);
					            expr->location = location;
					            localctx.tree =  expr;
					        
					}
					}
				}
				this.state = 778;
				this._errHandler.sync(this);
				_alt = this._interp.adaptivePredict(this._input, 58, this._ctx);
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
		this.enterRule(localctx, 80, GPULangParser.RULE_relationalExpression);

		        localctx.tree =  nullptr;
		        Symbol::Location location;
		    
		let _la: number;
		try {
			let _alt: number;
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 779;
			localctx._e1 = this.shiftExpression();
			 localctx.tree =  localctx._e1.tree; 
			this.state = 788;
			this._errHandler.sync(this);
			_alt = this._interp.adaptivePredict(this._input, 59, this._ctx);
			while (_alt !== 2 && _alt !== ATN.INVALID_ALT_NUMBER) {
				if (_alt === 1) {
					{
					{
					this.state = 781;
					localctx._op = this._input.LT(1);
					_la = this._input.LA(1);
					if(!(((((_la - 60)) & ~0x1F) === 0 && ((1 << (_la - 60)) & 15) !== 0))) {
					    localctx._op = this._errHandler.recoverInline(this);
					}
					else {
						this._errHandler.reportMatch(this);
					    this.consume();
					}
					 location = SetupFile(); 
					this.state = 783;
					localctx._e2 = this.shiftExpression();

					            BinaryExpression* expr = Alloc<BinaryExpression>(StringToFourCC((localctx._op != null ? localctx._op.text : undefined)), localctx.tree, localctx._e2.tree);
					            expr->location = location;
					            localctx.tree =  expr;
					        
					}
					}
				}
				this.state = 790;
				this._errHandler.sync(this);
				_alt = this._interp.adaptivePredict(this._input, 59, this._ctx);
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
		this.enterRule(localctx, 82, GPULangParser.RULE_shiftExpression);

		        localctx.tree =  nullptr;
		        Symbol::Location location;
		    
		let _la: number;
		try {
			let _alt: number;
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 791;
			localctx._e1 = this.addSubtractExpression();
			 localctx.tree =  localctx._e1.tree; 
			this.state = 800;
			this._errHandler.sync(this);
			_alt = this._interp.adaptivePredict(this._input, 60, this._ctx);
			while (_alt !== 2 && _alt !== ATN.INVALID_ALT_NUMBER) {
				if (_alt === 1) {
					{
					{
					this.state = 793;
					localctx._op = this._input.LT(1);
					_la = this._input.LA(1);
					if(!(_la===31 || _la===32)) {
					    localctx._op = this._errHandler.recoverInline(this);
					}
					else {
						this._errHandler.reportMatch(this);
					    this.consume();
					}
					 location = SetupFile(); 
					this.state = 795;
					localctx._e2 = this.addSubtractExpression();

					            BinaryExpression* expr = Alloc<BinaryExpression>(StringToFourCC((localctx._op != null ? localctx._op.text : undefined)), localctx.tree, localctx._e2.tree);
					            expr->location = location;
					            localctx.tree =  expr;
					        
					}
					}
				}
				this.state = 802;
				this._errHandler.sync(this);
				_alt = this._interp.adaptivePredict(this._input, 60, this._ctx);
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
		this.enterRule(localctx, 84, GPULangParser.RULE_addSubtractExpression);

		        localctx.tree =  nullptr;
		        Symbol::Location location;
		    
		let _la: number;
		try {
			let _alt: number;
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 803;
			localctx._e1 = this.multiplyDivideExpression();
			 localctx.tree =  localctx._e1.tree; 
			this.state = 812;
			this._errHandler.sync(this);
			_alt = this._interp.adaptivePredict(this._input, 61, this._ctx);
			while (_alt !== 2 && _alt !== ATN.INVALID_ALT_NUMBER) {
				if (_alt === 1) {
					{
					{
					this.state = 805;
					localctx._op = this._input.LT(1);
					_la = this._input.LA(1);
					if(!(_la===71 || _la===72)) {
					    localctx._op = this._errHandler.recoverInline(this);
					}
					else {
						this._errHandler.reportMatch(this);
					    this.consume();
					}
					 location = SetupFile(); 
					this.state = 807;
					localctx._e2 = this.multiplyDivideExpression();

					            BinaryExpression* expr = Alloc<BinaryExpression>(StringToFourCC((localctx._op != null ? localctx._op.text : undefined)), localctx.tree, localctx._e2.tree);
					            expr->location = location;
					            localctx.tree =  expr;
					        
					}
					}
				}
				this.state = 814;
				this._errHandler.sync(this);
				_alt = this._interp.adaptivePredict(this._input, 61, this._ctx);
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
		this.enterRule(localctx, 86, GPULangParser.RULE_multiplyDivideExpression);

		        localctx.tree =  nullptr;
		        Symbol::Location location;
		    
		let _la: number;
		try {
			let _alt: number;
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 815;
			localctx._e1 = this.suffixExpression();
			 localctx.tree =  localctx._e1.tree; 
			this.state = 824;
			this._errHandler.sync(this);
			_alt = this._interp.adaptivePredict(this._input, 62, this._ctx);
			while (_alt !== 2 && _alt !== ATN.INVALID_ALT_NUMBER) {
				if (_alt === 1) {
					{
					{
					this.state = 817;
					localctx._op = this._input.LT(1);
					_la = this._input.LA(1);
					if(!(((((_la - 68)) & ~0x1F) === 0 && ((1 << (_la - 68)) & 97) !== 0))) {
					    localctx._op = this._errHandler.recoverInline(this);
					}
					else {
						this._errHandler.reportMatch(this);
					    this.consume();
					}
					 location = SetupFile(); 
					this.state = 819;
					localctx._e2 = this.suffixExpression();

					            BinaryExpression* expr = Alloc<BinaryExpression>(StringToFourCC((localctx._op != null ? localctx._op.text : undefined)), localctx.tree, localctx._e2.tree);
					            expr->location = location;
					            localctx.tree =  expr;
					        
					}
					}
				}
				this.state = 826;
				this._errHandler.sync(this);
				_alt = this._interp.adaptivePredict(this._input, 62, this._ctx);
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
		this.enterRule(localctx, 88, GPULangParser.RULE_suffixExpression);

		        localctx.tree =  nullptr;

		        Symbol::Location location;
		        std::vector<Expression*> args;
		        Expression* arrayIndexExpr = nullptr;

		        std::vector<uint32_t> ops;
		        std::vector<Symbol::Location> locations;
		    
		let _la: number;
		try {
			let _alt: number;
			this.state = 880;
			this._errHandler.sync(this);
			switch ( this._interp.adaptivePredict(this._input, 69, this._ctx) ) {
			case 1:
				this.enterOuterAlt(localctx, 1);
				{
				this.state = 827;
				localctx._e1 = this.prefixExpression();

				        localctx.tree =  localctx._e1.tree;
				    
				this.state = 867;
				this._errHandler.sync(this);
				_alt = this._interp.adaptivePredict(this._input, 67, this._ctx);
				while (_alt !== 2 && _alt !== ATN.INVALID_ALT_NUMBER) {
					if (_alt === 1) {
						{
						this.state = 865;
						this._errHandler.sync(this);
						switch (this._input.LA(1)) {
						case 39:
							{
							this.state = 829;
							this.match(GPULangParser.LP);
							 location = SetupFile(); 
							this.state = 842;
							this._errHandler.sync(this);
							_la = this._input.LA(1);
							if (_la===1 || _la===2 || ((((_la - 33)) & ~0x1F) === 0 && ((1 << (_la - 33)) & 25207875) !== 0) || ((((_la - 71)) & ~0x1F) === 0 && ((1 << (_la - 71)) & 14955) !== 0)) {
								{
								this.state = 831;
								localctx._arg0 = this.logicalOrExpression();
								 args.push_back(localctx._arg0.tree); 
								this.state = 839;
								this._errHandler.sync(this);
								_la = this._input.LA(1);
								while (_la===37) {
									{
									{
									this.state = 833;
									this.match(GPULangParser.CO);
									this.state = 834;
									localctx._argn = this.logicalOrExpression();
									 args.push_back(localctx._argn.tree); 
									}
									}
									this.state = 841;
									this._errHandler.sync(this);
									_la = this._input.LA(1);
								}
								}
							}

							this.state = 844;
							this.match(GPULangParser.RP);

							            CallExpression* expr = Alloc<CallExpression>(localctx.tree, args);
							            expr->location = location;
							            localctx.tree =  expr;
							        
							}
							break;
						case 45:
							{
							this.state = 846;
							this.match(GPULangParser.DOT);
							 location = SetupFile(); 
							this.state = 848;
							localctx._e2 = this.assignmentExpression();

							            AccessExpression* expr = Alloc<AccessExpression>(localctx.tree, localctx._e2.tree, false);
							            expr->location = location;
							            localctx.tree =  expr;
							        
							}
							break;
						case 75:
							{
							this.state = 851;
							this.match(GPULangParser.ARROW);
							 location = SetupFile(); 
							this.state = 853;
							localctx._e2 = this.assignmentExpression();

							            AccessExpression* expr = Alloc<AccessExpression>(localctx.tree, localctx._e2.tree, true);
							            expr->location = location;
							            localctx.tree =  expr;
							        
							}
							break;
						case 43:
							{
							this.state = 856;
							this.match(GPULangParser.LL);
							 location = SetupFile(); 
							this.state = 861;
							this._errHandler.sync(this);
							_la = this._input.LA(1);
							if (_la===1 || _la===2 || ((((_la - 33)) & ~0x1F) === 0 && ((1 << (_la - 33)) & 25207875) !== 0) || ((((_la - 71)) & ~0x1F) === 0 && ((1 << (_la - 71)) & 14955) !== 0)) {
								{
								this.state = 858;
								localctx._e3 = this.expression();
								 arrayIndexExpr = localctx._e3.tree; 
								}
							}

							this.state = 863;
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
					this.state = 869;
					this._errHandler.sync(this);
					_alt = this._interp.adaptivePredict(this._input, 67, this._ctx);
				}
				}
				break;
			case 2:
				this.enterOuterAlt(localctx, 2);
				{
				this.state = 870;
				localctx._e1 = this.prefixExpression();
				this.state = 875;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
				while (_la===33 || _la===34) {
					{
					{
					this.state = 871;
					localctx._op = this._input.LT(1);
					_la = this._input.LA(1);
					if(!(_la===33 || _la===34)) {
					    localctx._op = this._errHandler.recoverInline(this);
					}
					else {
						this._errHandler.reportMatch(this);
					    this.consume();
					}
					 ops.push_back(StringToFourCC((localctx._op != null ? localctx._op.text : undefined))); locations.push_back(SetupFile()); 
					}
					}
					this.state = 877;
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
		this.enterRule(localctx, 90, GPULangParser.RULE_prefixExpression);

		        localctx.tree =  nullptr;
		        std::vector<uint32_t> ops;
		        std::vector<Symbol::Location> locations;
		    
		let _la: number;
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 886;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (((((_la - 33)) & ~0x1F) === 0 && ((1 << (_la - 33)) & 8396803) !== 0) || ((((_la - 71)) & ~0x1F) === 0 && ((1 << (_la - 71)) & 11) !== 0)) {
				{
				{
				this.state = 882;
				localctx._op = this._input.LT(1);
				_la = this._input.LA(1);
				if(!(((((_la - 33)) & ~0x1F) === 0 && ((1 << (_la - 33)) & 8396803) !== 0) || ((((_la - 71)) & ~0x1F) === 0 && ((1 << (_la - 71)) & 11) !== 0))) {
				    localctx._op = this._errHandler.recoverInline(this);
				}
				else {
					this._errHandler.reportMatch(this);
				    this.consume();
				}
				 ops.push_back(StringToFourCC((localctx._op != null ? localctx._op.text : undefined))); locations.push_back(SetupFile()); 
				}
				}
				this.state = 888;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 889;
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
		this.enterRule(localctx, 92, GPULangParser.RULE_namespaceExpression);

		        localctx.tree =  nullptr;
		    
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 892;
			localctx._e1 = this.binaryexpatom();
			this.state = 893;
			this.match(GPULangParser.T__34);
			this.state = 894;
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
		this.enterRule(localctx, 94, GPULangParser.RULE_binaryexpatom);

		        localctx.tree =  nullptr;
		    
		try {
			this.state = 926;
			this._errHandler.sync(this);
			switch ( this._interp.adaptivePredict(this._input, 71, this._ctx) ) {
			case 1:
				this.enterOuterAlt(localctx, 1);
				{
				this.state = 897;
				localctx._initializerExpression = this.initializerExpression();
				 localctx.tree =  localctx._initializerExpression.tree; 
				}
				break;
			case 2:
				this.enterOuterAlt(localctx, 2);
				{
				this.state = 900;
				localctx._arrayInitializerExpression = this.arrayInitializerExpression();
				 localctx.tree =  localctx._arrayInitializerExpression.tree; 
				}
				break;
			case 3:
				this.enterOuterAlt(localctx, 3);
				{
				this.state = 903;
				this.match(GPULangParser.LP);
				this.state = 904;
				localctx._expression = this.expression();
				this.state = 905;
				this.match(GPULangParser.RP);
				 localctx.tree =  localctx._expression.tree; 
				}
				break;
			case 4:
				this.enterOuterAlt(localctx, 4);
				{
				this.state = 908;
				localctx._INTEGERLITERAL = this.match(GPULangParser.INTEGERLITERAL);
				 localctx.tree =  Alloc<IntExpression>(atoi((localctx._INTEGERLITERAL != null ? localctx._INTEGERLITERAL.text : undefined).c_str())); localctx.tree->location = SetupFile(); 
				}
				break;
			case 5:
				this.enterOuterAlt(localctx, 5);
				{
				this.state = 910;
				localctx._UINTEGERLITERAL = this.match(GPULangParser.UINTEGERLITERAL);
				 localctx.tree =  Alloc<UIntExpression>(strtoul((localctx._UINTEGERLITERAL != null ? localctx._UINTEGERLITERAL.text : undefined).c_str(), nullptr, 10)); localctx.tree->location = SetupFile(); 
				}
				break;
			case 6:
				this.enterOuterAlt(localctx, 6);
				{
				this.state = 912;
				localctx._FLOATLITERAL = this.match(GPULangParser.FLOATLITERAL);
				 localctx.tree =  Alloc<FloatExpression>(atof((localctx._FLOATLITERAL != null ? localctx._FLOATLITERAL.text : undefined).c_str())); localctx.tree->location = SetupFile(); 
				}
				break;
			case 7:
				this.enterOuterAlt(localctx, 7);
				{
				this.state = 914;
				localctx._DOUBLELITERAL = this.match(GPULangParser.DOUBLELITERAL);
				 localctx.tree =  Alloc<FloatExpression>(atof((localctx._DOUBLELITERAL != null ? localctx._DOUBLELITERAL.text : undefined).c_str())); localctx.tree->location = SetupFile(); 
				}
				break;
			case 8:
				this.enterOuterAlt(localctx, 8);
				{
				this.state = 916;
				localctx._HEX = this.match(GPULangParser.HEX);
				 localctx.tree =  Alloc<UIntExpression>(strtoul((localctx._HEX != null ? localctx._HEX.text : undefined).c_str(), nullptr, 16)); localctx.tree->location = SetupFile(); 
				}
				break;
			case 9:
				this.enterOuterAlt(localctx, 9);
				{
				this.state = 918;
				localctx._string_ = this.string_();
				 localctx.tree =  Alloc<StringExpression>(localctx._string.val); localctx.tree->location = SetupFile(); 
				}
				break;
			case 10:
				this.enterOuterAlt(localctx, 10);
				{
				this.state = 921;
				localctx._IDENTIFIER = this.match(GPULangParser.IDENTIFIER);
				 localctx.tree =  Alloc<SymbolExpression>((localctx._IDENTIFIER != null ? localctx._IDENTIFIER.text : undefined)); localctx.tree->location = SetupFile(); 
				}
				break;
			case 11:
				this.enterOuterAlt(localctx, 11);
				{
				this.state = 923;
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
		this.enterRule(localctx, 96, GPULangParser.RULE_initializerExpression);

		        localctx.tree =  nullptr;
		        std::vector<Expression*> exprs;
		        std::string type = "";
		        Symbol::Location location;
		    
		let _la: number;
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 928;
			localctx._type_ = this.match(GPULangParser.IDENTIFIER);
			 type = (localctx._type != null ? localctx._type.text : undefined); 
			this.state = 930;
			this.match(GPULangParser.LB);
			 location = SetupFile(); 
			this.state = 943;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la===1 || _la===2 || ((((_la - 33)) & ~0x1F) === 0 && ((1 << (_la - 33)) & 25207875) !== 0) || ((((_la - 71)) & ~0x1F) === 0 && ((1 << (_la - 71)) & 14955) !== 0)) {
				{
				this.state = 932;
				localctx._arg0 = this.logicalOrExpression();
				 exprs.push_back(localctx._arg0.tree); 
				this.state = 940;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
				while (_la===37) {
					{
					{
					this.state = 934;
					this.match(GPULangParser.CO);
					this.state = 935;
					localctx._argN = this.logicalOrExpression();
					 exprs.push_back(localctx._argN.tree); 
					}
					}
					this.state = 942;
					this._errHandler.sync(this);
					_la = this._input.LA(1);
				}
				}
			}

			this.state = 945;
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
		this.enterRule(localctx, 98, GPULangParser.RULE_arrayInitializerExpression);

		        localctx.tree =  nullptr;
		        std::vector<Expression*> exprs;
		        Symbol::Location location;
		    
		let _la: number;
		try {
			this.enterOuterAlt(localctx, 1);
			{
			this.state = 948;
			this.match(GPULangParser.LL);
			 location = SetupFile(); 
			this.state = 961;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la===1 || _la===2 || ((((_la - 33)) & ~0x1F) === 0 && ((1 << (_la - 33)) & 25207875) !== 0) || ((((_la - 71)) & ~0x1F) === 0 && ((1 << (_la - 71)) & 14955) !== 0)) {
				{
				this.state = 950;
				localctx._arg0 = this.logicalOrExpression();
				 exprs.push_back(localctx._arg0.tree); 
				this.state = 958;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
				while (_la===37) {
					{
					{
					this.state = 952;
					this.match(GPULangParser.CO);
					this.state = 953;
					localctx._argN = this.logicalOrExpression();
					 exprs.push_back(localctx._argN.tree); 
					}
					}
					this.state = 960;
					this._errHandler.sync(this);
					_la = this._input.LA(1);
				}
				}
			}

			this.state = 963;
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

	public static readonly _serializedATN: number[] = [4,1,85,967,2,0,7,0,2,
	1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,7,7,7,2,8,7,8,2,9,7,9,2,
	10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,14,2,15,7,15,2,16,7,16,2,17,
	7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,21,2,22,7,22,2,23,7,23,2,24,7,
	24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,28,2,29,7,29,2,30,7,30,2,31,7,31,
	2,32,7,32,2,33,7,33,2,34,7,34,2,35,7,35,2,36,7,36,2,37,7,37,2,38,7,38,2,
	39,7,39,2,40,7,40,2,41,7,41,2,42,7,42,2,43,7,43,2,44,7,44,2,45,7,45,2,46,
	7,46,2,47,7,47,2,48,7,48,2,49,7,49,1,0,1,0,1,0,5,0,104,8,0,10,0,12,0,107,
	9,0,1,0,1,0,1,0,1,0,5,0,113,8,0,10,0,12,0,116,9,0,1,0,3,0,119,8,0,1,1,1,
	1,1,1,1,1,3,1,125,8,1,1,2,1,2,1,2,1,2,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,
	3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,
	3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,5,3,167,8,3,10,3,12,3,170,9,3,1,4,
	1,4,1,4,1,4,1,4,1,4,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,6,1,6,1,6,1,6,1,6,1,6,
	1,6,1,7,1,7,1,7,1,7,1,7,1,7,1,7,1,7,3,7,200,8,7,1,8,1,8,1,8,1,8,1,8,1,8,
	1,8,3,8,209,8,8,1,8,1,8,1,8,5,8,214,8,8,10,8,12,8,217,9,8,1,8,1,8,1,8,1,
	9,1,9,1,9,5,9,225,8,9,10,9,12,9,228,9,9,1,9,1,9,1,9,4,9,233,8,9,11,9,12,
	9,234,1,9,1,9,1,9,1,9,1,9,5,9,242,8,9,10,9,12,9,245,9,9,1,9,1,9,1,9,1,9,
	3,9,251,8,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,5,9,260,8,9,10,9,12,9,263,9,9,3,
	9,265,8,9,1,9,1,9,1,10,1,10,1,10,5,10,272,8,10,10,10,12,10,275,9,10,1,10,
	1,10,1,10,5,10,280,8,10,10,10,12,10,283,9,10,1,10,1,10,1,10,1,10,1,11,1,
	11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,3,11,302,8,11,
	1,11,5,11,305,8,11,10,11,12,11,308,9,11,1,11,1,11,1,11,1,11,5,11,314,8,
	11,10,11,12,11,317,9,11,1,11,1,11,1,11,1,12,1,12,1,12,1,12,1,12,1,12,1,
	12,3,12,329,8,12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,3,12,338,8,12,1,12,
	1,12,1,12,1,12,1,12,1,12,1,12,1,12,3,12,348,8,12,1,12,5,12,351,8,12,10,
	12,12,12,354,9,12,3,12,356,8,12,1,12,1,12,1,12,1,13,1,13,1,13,5,13,364,
	8,13,10,13,12,13,367,9,13,1,13,1,13,1,13,1,13,1,13,1,13,1,13,1,13,1,13,
	3,13,378,8,13,1,13,1,13,1,14,1,14,1,14,5,14,385,8,14,10,14,12,14,388,9,
	14,1,14,1,14,1,14,1,14,1,14,1,14,1,14,1,14,1,14,5,14,399,8,14,10,14,12,
	14,402,9,14,3,14,404,8,14,1,14,1,14,1,14,1,14,1,15,1,15,1,15,1,15,1,15,
	1,15,1,16,1,16,1,16,5,16,419,8,16,10,16,12,16,422,9,16,1,16,1,16,1,16,1,
	16,1,16,1,16,1,16,1,16,5,16,432,8,16,10,16,12,16,435,9,16,1,16,1,16,1,16,
	1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,5,17,450,8,17,10,17,12,
	17,453,9,17,1,17,1,17,1,17,1,18,1,18,1,18,1,18,1,18,1,18,1,18,1,18,1,18,
	1,18,5,18,468,8,18,10,18,12,18,471,9,18,1,18,1,18,1,18,1,19,1,19,1,19,1,
	19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,
	1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,3,19,507,
	8,19,1,20,1,20,1,20,1,21,1,21,1,21,1,21,1,21,1,21,1,21,1,21,1,21,1,21,1,
	21,1,21,3,21,524,8,21,1,21,1,21,1,22,1,22,1,22,1,22,1,22,1,22,3,22,534,
	8,22,1,22,1,22,1,22,1,22,3,22,540,8,22,1,22,1,22,1,22,1,22,3,22,546,8,22,
	1,22,1,22,1,22,1,22,5,22,552,8,22,10,22,12,22,555,9,22,1,22,1,22,1,22,1,
	22,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,24,
	1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,
	24,1,24,1,24,1,24,1,24,1,24,1,24,3,24,596,8,24,1,25,1,25,1,25,1,25,1,25,
	1,25,1,25,1,25,1,25,1,25,5,25,608,8,25,10,25,12,25,611,9,25,1,25,1,25,1,
	25,1,26,1,26,1,26,1,26,1,26,3,26,621,8,26,1,26,1,26,1,26,1,27,1,27,1,27,
	1,27,1,27,1,28,1,28,1,28,1,28,1,28,1,29,1,29,1,29,1,29,1,29,1,29,1,29,1,
	29,1,29,1,29,1,29,1,29,1,29,1,29,3,29,650,8,29,5,29,652,8,29,10,29,12,29,
	655,9,29,1,29,1,29,1,29,1,29,1,29,3,29,662,8,29,3,29,664,8,29,1,29,1,29,
	1,29,1,30,1,30,1,30,1,30,1,30,1,31,1,31,1,31,1,32,1,32,1,32,1,32,1,32,1,
	32,1,32,5,32,684,8,32,10,32,12,32,687,9,32,1,33,1,33,1,33,1,33,1,33,1,33,
	1,33,1,33,1,33,1,33,1,33,1,33,1,33,1,33,5,33,703,8,33,10,33,12,33,706,9,
	33,1,34,1,34,1,34,1,34,1,34,1,34,1,34,5,34,715,8,34,10,34,12,34,718,9,34,
	1,35,1,35,1,35,1,35,1,35,1,35,1,35,5,35,727,8,35,10,35,12,35,730,9,35,1,
	36,1,36,1,36,1,36,1,36,1,36,1,36,5,36,739,8,36,10,36,12,36,742,9,36,1,37,
	1,37,1,37,1,37,1,37,1,37,1,37,5,37,751,8,37,10,37,12,37,754,9,37,1,38,1,
	38,1,38,1,38,1,38,1,38,1,38,5,38,763,8,38,10,38,12,38,766,9,38,1,39,1,39,
	1,39,1,39,1,39,1,39,1,39,5,39,775,8,39,10,39,12,39,778,9,39,1,40,1,40,1,
	40,1,40,1,40,1,40,1,40,5,40,787,8,40,10,40,12,40,790,9,40,1,41,1,41,1,41,
	1,41,1,41,1,41,1,41,5,41,799,8,41,10,41,12,41,802,9,41,1,42,1,42,1,42,1,
	42,1,42,1,42,1,42,5,42,811,8,42,10,42,12,42,814,9,42,1,43,1,43,1,43,1,43,
	1,43,1,43,1,43,5,43,823,8,43,10,43,12,43,826,9,43,1,44,1,44,1,44,1,44,1,
	44,1,44,1,44,1,44,1,44,1,44,5,44,838,8,44,10,44,12,44,841,9,44,3,44,843,
	8,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,
	44,1,44,1,44,1,44,3,44,862,8,44,1,44,1,44,5,44,866,8,44,10,44,12,44,869,
	9,44,1,44,1,44,1,44,5,44,874,8,44,10,44,12,44,877,9,44,1,44,1,44,3,44,881,
	8,44,1,45,1,45,5,45,885,8,45,10,45,12,45,888,9,45,1,45,1,45,1,45,1,46,1,
	46,1,46,1,46,1,46,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,
	1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,
	47,1,47,1,47,1,47,3,47,927,8,47,1,48,1,48,1,48,1,48,1,48,1,48,1,48,1,48,
	1,48,1,48,5,48,939,8,48,10,48,12,48,942,9,48,3,48,944,8,48,1,48,1,48,1,
	48,1,49,1,49,1,49,1,49,1,49,1,49,1,49,1,49,5,49,957,8,49,10,49,12,49,960,
	9,49,3,49,962,8,49,1,49,1,49,1,49,1,49,1,168,0,50,0,2,4,6,8,10,12,14,16,
	18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,
	66,68,70,72,74,76,78,80,82,84,86,88,90,92,94,96,98,0,10,1,0,48,48,1,0,57,
	57,5,0,24,30,47,47,51,51,53,53,55,55,1,0,64,65,1,0,60,63,1,0,31,32,1,0,
	71,72,2,0,68,68,73,74,1,0,33,34,5,0,33,34,46,46,56,56,71,72,74,74,1021,
	0,118,1,0,0,0,2,124,1,0,0,0,4,126,1,0,0,0,6,168,1,0,0,0,8,171,1,0,0,0,10,
	177,1,0,0,0,12,184,1,0,0,0,14,199,1,0,0,0,16,215,1,0,0,0,18,226,1,0,0,0,
	20,273,1,0,0,0,22,288,1,0,0,0,24,321,1,0,0,0,26,365,1,0,0,0,28,386,1,0,
	0,0,30,409,1,0,0,0,32,420,1,0,0,0,34,439,1,0,0,0,36,457,1,0,0,0,38,506,
	1,0,0,0,40,508,1,0,0,0,42,511,1,0,0,0,44,527,1,0,0,0,46,560,1,0,0,0,48,
	595,1,0,0,0,50,597,1,0,0,0,52,615,1,0,0,0,54,625,1,0,0,0,56,630,1,0,0,0,
	58,635,1,0,0,0,60,668,1,0,0,0,62,673,1,0,0,0,64,676,1,0,0,0,66,688,1,0,
	0,0,68,707,1,0,0,0,70,719,1,0,0,0,72,731,1,0,0,0,74,743,1,0,0,0,76,755,
	1,0,0,0,78,767,1,0,0,0,80,779,1,0,0,0,82,791,1,0,0,0,84,803,1,0,0,0,86,
	815,1,0,0,0,88,880,1,0,0,0,90,886,1,0,0,0,92,892,1,0,0,0,94,926,1,0,0,0,
	96,928,1,0,0,0,98,948,1,0,0,0,100,105,5,48,0,0,101,102,8,0,0,0,102,104,
	6,0,-1,0,103,101,1,0,0,0,104,107,1,0,0,0,105,103,1,0,0,0,105,106,1,0,0,
	0,106,108,1,0,0,0,107,105,1,0,0,0,108,119,5,48,0,0,109,114,5,57,0,0,110,
	111,8,1,0,0,111,113,6,0,-1,0,112,110,1,0,0,0,113,116,1,0,0,0,114,112,1,
	0,0,0,114,115,1,0,0,0,115,117,1,0,0,0,116,114,1,0,0,0,117,119,5,57,0,0,
	118,100,1,0,0,0,118,109,1,0,0,0,119,1,1,0,0,0,120,121,5,1,0,0,121,125,6,
	1,-1,0,122,123,5,2,0,0,123,125,6,1,-1,0,124,120,1,0,0,0,124,122,1,0,0,0,
	125,3,1,0,0,0,126,127,3,6,3,0,127,128,6,2,-1,0,128,129,5,0,0,1,129,5,1,
	0,0,0,130,167,3,8,4,0,131,132,3,10,5,0,132,133,5,36,0,0,133,134,6,3,-1,
	0,134,167,1,0,0,0,135,136,3,28,14,0,136,137,5,36,0,0,137,138,6,3,-1,0,138,
	167,1,0,0,0,139,140,3,30,15,0,140,141,6,3,-1,0,141,167,1,0,0,0,142,143,
	3,18,9,0,143,144,5,36,0,0,144,145,6,3,-1,0,145,167,1,0,0,0,146,147,3,22,
	11,0,147,148,5,36,0,0,148,149,6,3,-1,0,149,167,1,0,0,0,150,151,3,24,12,
	0,151,152,5,36,0,0,152,153,6,3,-1,0,153,167,1,0,0,0,154,155,3,36,18,0,155,
	156,5,36,0,0,156,157,6,3,-1,0,157,167,1,0,0,0,158,159,3,34,17,0,159,160,
	5,36,0,0,160,161,6,3,-1,0,161,167,1,0,0,0,162,163,3,32,16,0,163,164,5,36,
	0,0,164,165,6,3,-1,0,165,167,1,0,0,0,166,130,1,0,0,0,166,131,1,0,0,0,166,
	135,1,0,0,0,166,139,1,0,0,0,166,142,1,0,0,0,166,146,1,0,0,0,166,150,1,0,
	0,0,166,154,1,0,0,0,166,158,1,0,0,0,166,162,1,0,0,0,167,170,1,0,0,0,168,
	169,1,0,0,0,168,166,1,0,0,0,169,7,1,0,0,0,170,168,1,0,0,0,171,172,5,3,0,
	0,172,173,6,4,-1,0,173,174,5,76,0,0,174,175,3,0,0,0,175,176,6,4,-1,0,176,
	9,1,0,0,0,177,178,5,4,0,0,178,179,5,84,0,0,179,180,5,5,0,0,180,181,5,84,
	0,0,181,182,6,5,-1,0,182,183,6,5,-1,0,183,11,1,0,0,0,184,185,5,70,0,0,185,
	186,5,84,0,0,186,187,5,39,0,0,187,188,3,62,31,0,188,189,5,40,0,0,189,190,
	6,6,-1,0,190,13,1,0,0,0,191,192,5,84,0,0,192,193,5,39,0,0,193,194,3,62,
	31,0,194,195,5,40,0,0,195,196,6,7,-1,0,196,200,1,0,0,0,197,198,5,84,0,0,
	198,200,6,7,-1,0,199,191,1,0,0,0,199,197,1,0,0,0,200,15,1,0,0,0,201,202,
	5,74,0,0,202,214,6,8,-1,0,203,204,5,43,0,0,204,208,6,8,-1,0,205,206,3,62,
	31,0,206,207,6,8,-1,0,207,209,1,0,0,0,208,205,1,0,0,0,208,209,1,0,0,0,209,
	210,1,0,0,0,210,214,5,44,0,0,211,212,5,84,0,0,212,214,6,8,-1,0,213,201,
	1,0,0,0,213,203,1,0,0,0,213,211,1,0,0,0,214,217,1,0,0,0,215,213,1,0,0,0,
	215,216,1,0,0,0,216,218,1,0,0,0,217,215,1,0,0,0,218,219,5,84,0,0,219,220,
	6,8,-1,0,220,17,1,0,0,0,221,222,3,12,6,0,222,223,6,9,-1,0,223,225,1,0,0,
	0,224,221,1,0,0,0,225,228,1,0,0,0,226,224,1,0,0,0,226,227,1,0,0,0,227,232,
	1,0,0,0,228,226,1,0,0,0,229,230,3,14,7,0,230,231,6,9,-1,0,231,233,1,0,0,
	0,232,229,1,0,0,0,233,234,1,0,0,0,234,232,1,0,0,0,234,235,1,0,0,0,235,236,
	1,0,0,0,236,237,5,84,0,0,237,243,6,9,-1,0,238,239,5,37,0,0,239,240,5,84,
	0,0,240,242,6,9,-1,0,241,238,1,0,0,0,242,245,1,0,0,0,243,241,1,0,0,0,243,
	244,1,0,0,0,244,250,1,0,0,0,245,243,1,0,0,0,246,247,5,38,0,0,247,248,3,
	16,8,0,248,249,6,9,-1,0,249,251,1,0,0,0,250,246,1,0,0,0,250,251,1,0,0,0,
	251,264,1,0,0,0,252,253,5,47,0,0,253,254,3,68,34,0,254,261,6,9,-1,0,255,
	256,5,37,0,0,256,257,3,68,34,0,257,258,6,9,-1,0,258,260,1,0,0,0,259,255,
	1,0,0,0,260,263,1,0,0,0,261,259,1,0,0,0,261,262,1,0,0,0,262,265,1,0,0,0,
	263,261,1,0,0,0,264,252,1,0,0,0,264,265,1,0,0,0,265,266,1,0,0,0,266,267,
	6,9,-1,0,267,19,1,0,0,0,268,269,3,12,6,0,269,270,6,10,-1,0,270,272,1,0,
	0,0,271,268,1,0,0,0,272,275,1,0,0,0,273,271,1,0,0,0,273,274,1,0,0,0,274,
	281,1,0,0,0,275,273,1,0,0,0,276,277,3,14,7,0,277,278,6,10,-1,0,278,280,
	1,0,0,0,279,276,1,0,0,0,280,283,1,0,0,0,281,279,1,0,0,0,281,282,1,0,0,0,
	282,284,1,0,0,0,283,281,1,0,0,0,284,285,5,6,0,0,285,286,5,84,0,0,286,287,
	6,10,-1,0,287,21,1,0,0,0,288,289,3,20,10,0,289,290,6,11,-1,0,290,315,5,
	41,0,0,291,292,5,84,0,0,292,293,6,11,-1,0,293,306,5,38,0,0,294,295,5,74,
	0,0,295,305,6,11,-1,0,296,297,5,43,0,0,297,301,6,11,-1,0,298,299,3,62,31,
	0,299,300,6,11,-1,0,300,302,1,0,0,0,301,298,1,0,0,0,301,302,1,0,0,0,302,
	303,1,0,0,0,303,305,5,44,0,0,304,294,1,0,0,0,304,296,1,0,0,0,305,308,1,
	0,0,0,306,304,1,0,0,0,306,307,1,0,0,0,307,309,1,0,0,0,308,306,1,0,0,0,309,
	310,5,84,0,0,310,311,6,11,-1,0,311,312,5,36,0,0,312,314,6,11,-1,0,313,291,
	1,0,0,0,314,317,1,0,0,0,315,313,1,0,0,0,315,316,1,0,0,0,316,318,1,0,0,0,
	317,315,1,0,0,0,318,319,5,42,0,0,319,320,6,11,-1,0,320,23,1,0,0,0,321,322,
	5,7,0,0,322,323,5,84,0,0,323,328,6,12,-1,0,324,325,5,38,0,0,325,326,3,16,
	8,0,326,327,6,12,-1,0,327,329,1,0,0,0,328,324,1,0,0,0,328,329,1,0,0,0,329,
	330,1,0,0,0,330,355,5,41,0,0,331,332,5,84,0,0,332,337,6,12,-1,0,333,334,
	5,47,0,0,334,335,3,62,31,0,335,336,6,12,-1,0,336,338,1,0,0,0,337,333,1,
	0,0,0,337,338,1,0,0,0,338,339,1,0,0,0,339,352,6,12,-1,0,340,341,5,37,0,
	0,341,342,5,84,0,0,342,347,6,12,-1,0,343,344,5,47,0,0,344,345,3,62,31,0,
	345,346,6,12,-1,0,346,348,1,0,0,0,347,343,1,0,0,0,347,348,1,0,0,0,348,349,
	1,0,0,0,349,351,6,12,-1,0,350,340,1,0,0,0,351,354,1,0,0,0,352,350,1,0,0,
	0,352,353,1,0,0,0,353,356,1,0,0,0,354,352,1,0,0,0,355,331,1,0,0,0,355,356,
	1,0,0,0,356,357,1,0,0,0,357,358,5,42,0,0,358,359,6,12,-1,0,359,25,1,0,0,
	0,360,361,3,14,7,0,361,362,6,13,-1,0,362,364,1,0,0,0,363,360,1,0,0,0,364,
	367,1,0,0,0,365,363,1,0,0,0,365,366,1,0,0,0,366,368,1,0,0,0,367,365,1,0,
	0,0,368,369,5,84,0,0,369,370,6,13,-1,0,370,371,5,38,0,0,371,372,3,16,8,
	0,372,377,6,13,-1,0,373,374,5,47,0,0,374,375,3,68,34,0,375,376,6,13,-1,
	0,376,378,1,0,0,0,377,373,1,0,0,0,377,378,1,0,0,0,378,379,1,0,0,0,379,380,
	6,13,-1,0,380,27,1,0,0,0,381,382,3,14,7,0,382,383,6,14,-1,0,383,385,1,0,
	0,0,384,381,1,0,0,0,385,388,1,0,0,0,386,384,1,0,0,0,386,387,1,0,0,0,387,
	389,1,0,0,0,388,386,1,0,0,0,389,390,5,84,0,0,390,391,6,14,-1,0,391,403,
	5,39,0,0,392,393,3,26,13,0,393,400,6,14,-1,0,394,395,5,37,0,0,395,396,3,
	26,13,0,396,397,6,14,-1,0,397,399,1,0,0,0,398,394,1,0,0,0,399,402,1,0,0,
	0,400,398,1,0,0,0,400,401,1,0,0,0,401,404,1,0,0,0,402,400,1,0,0,0,403,392,
	1,0,0,0,403,404,1,0,0,0,404,405,1,0,0,0,405,406,5,40,0,0,406,407,3,16,8,
	0,407,408,6,14,-1,0,408,29,1,0,0,0,409,410,3,28,14,0,410,411,6,15,-1,0,
	411,412,6,15,-1,0,412,413,3,50,25,0,413,414,6,15,-1,0,414,31,1,0,0,0,415,
	416,3,12,6,0,416,417,6,16,-1,0,417,419,1,0,0,0,418,415,1,0,0,0,419,422,
	1,0,0,0,420,418,1,0,0,0,420,421,1,0,0,0,421,423,1,0,0,0,422,420,1,0,0,0,
	423,424,5,8,0,0,424,425,5,84,0,0,425,426,6,16,-1,0,426,433,5,41,0,0,427,
	428,3,62,31,0,428,429,6,16,-1,0,429,430,5,36,0,0,430,432,1,0,0,0,431,427,
	1,0,0,0,432,435,1,0,0,0,433,431,1,0,0,0,433,434,1,0,0,0,434,436,1,0,0,0,
	435,433,1,0,0,0,436,437,5,42,0,0,437,438,6,16,-1,0,438,33,1,0,0,0,439,440,
	5,9,0,0,440,441,6,17,-1,0,441,442,1,0,0,0,442,443,5,84,0,0,443,444,6,17,
	-1,0,444,451,5,41,0,0,445,446,3,62,31,0,446,447,6,17,-1,0,447,448,5,36,
	0,0,448,450,1,0,0,0,449,445,1,0,0,0,450,453,1,0,0,0,451,449,1,0,0,0,451,
	452,1,0,0,0,452,454,1,0,0,0,453,451,1,0,0,0,454,455,5,42,0,0,455,456,6,
	17,-1,0,456,35,1,0,0,0,457,458,5,10,0,0,458,459,6,18,-1,0,459,460,1,0,0,
	0,460,461,5,84,0,0,461,462,6,18,-1,0,462,469,5,41,0,0,463,464,3,62,31,0,
	464,465,6,18,-1,0,465,466,5,36,0,0,466,468,1,0,0,0,467,463,1,0,0,0,468,
	471,1,0,0,0,469,467,1,0,0,0,469,470,1,0,0,0,470,472,1,0,0,0,471,469,1,0,
	0,0,472,473,5,42,0,0,473,474,6,18,-1,0,474,37,1,0,0,0,475,476,3,42,21,0,
	476,477,6,19,-1,0,477,507,1,0,0,0,478,479,3,50,25,0,479,480,6,19,-1,0,480,
	507,1,0,0,0,481,482,3,44,22,0,482,483,6,19,-1,0,483,507,1,0,0,0,484,485,
	3,48,24,0,485,486,6,19,-1,0,486,507,1,0,0,0,487,488,3,58,29,0,488,489,6,
	19,-1,0,489,507,1,0,0,0,490,491,3,52,26,0,491,492,6,19,-1,0,492,507,1,0,
	0,0,493,494,3,54,27,0,494,495,6,19,-1,0,495,507,1,0,0,0,496,497,3,56,28,
	0,497,498,6,19,-1,0,498,507,1,0,0,0,499,500,3,60,30,0,500,501,6,19,-1,0,
	501,507,1,0,0,0,502,503,3,40,20,0,503,504,5,36,0,0,504,505,6,19,-1,0,505,
	507,1,0,0,0,506,475,1,0,0,0,506,478,1,0,0,0,506,481,1,0,0,0,506,484,1,0,
	0,0,506,487,1,0,0,0,506,490,1,0,0,0,506,493,1,0,0,0,506,496,1,0,0,0,506,
	499,1,0,0,0,506,502,1,0,0,0,507,39,1,0,0,0,508,509,3,62,31,0,509,510,6,
	20,-1,0,510,41,1,0,0,0,511,512,5,11,0,0,512,513,6,21,-1,0,513,514,5,39,
	0,0,514,515,3,62,31,0,515,516,6,21,-1,0,516,517,5,40,0,0,517,518,3,38,19,
	0,518,523,6,21,-1,0,519,520,5,12,0,0,520,521,3,38,19,0,521,522,6,21,-1,
	0,522,524,1,0,0,0,523,519,1,0,0,0,523,524,1,0,0,0,524,525,1,0,0,0,525,526,
	6,21,-1,0,526,43,1,0,0,0,527,528,5,13,0,0,528,529,6,22,-1,0,529,533,5,39,
	0,0,530,531,3,18,9,0,531,532,6,22,-1,0,532,534,1,0,0,0,533,530,1,0,0,0,
	533,534,1,0,0,0,534,535,1,0,0,0,535,539,5,36,0,0,536,537,3,62,31,0,537,
	538,6,22,-1,0,538,540,1,0,0,0,539,536,1,0,0,0,539,540,1,0,0,0,540,541,1,
	0,0,0,541,545,5,36,0,0,542,543,3,62,31,0,543,544,6,22,-1,0,544,546,1,0,
	0,0,545,542,1,0,0,0,545,546,1,0,0,0,546,547,1,0,0,0,547,553,5,40,0,0,548,
	549,3,14,7,0,549,550,6,22,-1,0,550,552,1,0,0,0,551,548,1,0,0,0,552,555,
	1,0,0,0,553,551,1,0,0,0,553,554,1,0,0,0,554,556,1,0,0,0,555,553,1,0,0,0,
	556,557,3,38,19,0,557,558,6,22,-1,0,558,559,6,22,-1,0,559,45,1,0,0,0,560,
	561,5,13,0,0,561,562,6,23,-1,0,562,563,5,39,0,0,563,564,5,84,0,0,564,565,
	5,38,0,0,565,566,5,84,0,0,566,567,5,14,0,0,567,568,5,84,0,0,568,569,5,40,
	0,0,569,570,3,38,19,0,570,571,6,23,-1,0,571,572,6,23,-1,0,572,47,1,0,0,
	0,573,574,5,15,0,0,574,575,6,24,-1,0,575,576,5,39,0,0,576,577,3,62,31,0,
	577,578,6,24,-1,0,578,579,5,40,0,0,579,580,3,38,19,0,580,581,6,24,-1,0,
	581,582,6,24,-1,0,582,596,1,0,0,0,583,584,5,16,0,0,584,585,6,24,-1,0,585,
	586,3,38,19,0,586,587,6,24,-1,0,587,588,5,15,0,0,588,589,5,39,0,0,589,590,
	3,62,31,0,590,591,6,24,-1,0,591,592,5,40,0,0,592,593,5,36,0,0,593,594,6,
	24,-1,0,594,596,1,0,0,0,595,573,1,0,0,0,595,583,1,0,0,0,596,49,1,0,0,0,
	597,598,5,41,0,0,598,609,6,25,-1,0,599,600,3,18,9,0,600,601,5,36,0,0,601,
	602,6,25,-1,0,602,608,1,0,0,0,603,604,3,38,19,0,604,605,6,25,-1,0,605,608,
	1,0,0,0,606,608,3,8,4,0,607,599,1,0,0,0,607,603,1,0,0,0,607,606,1,0,0,0,
	608,611,1,0,0,0,609,607,1,0,0,0,609,610,1,0,0,0,610,612,1,0,0,0,611,609,
	1,0,0,0,612,613,5,42,0,0,613,614,6,25,-1,0,614,51,1,0,0,0,615,616,5,17,
	0,0,616,620,6,26,-1,0,617,618,3,62,31,0,618,619,6,26,-1,0,619,621,1,0,0,
	0,620,617,1,0,0,0,620,621,1,0,0,0,621,622,1,0,0,0,622,623,5,36,0,0,623,
	624,6,26,-1,0,624,53,1,0,0,0,625,626,5,18,0,0,626,627,6,27,-1,0,627,628,
	5,36,0,0,628,629,6,27,-1,0,629,55,1,0,0,0,630,631,5,19,0,0,631,632,6,28,
	-1,0,632,633,5,36,0,0,633,634,6,28,-1,0,634,57,1,0,0,0,635,636,5,20,0,0,
	636,637,6,29,-1,0,637,638,5,39,0,0,638,639,3,62,31,0,639,640,5,40,0,0,640,
	641,6,29,-1,0,641,653,5,41,0,0,642,643,5,21,0,0,643,644,3,62,31,0,644,645,
	5,38,0,0,645,649,6,29,-1,0,646,647,3,38,19,0,647,648,6,29,-1,0,648,650,
	1,0,0,0,649,646,1,0,0,0,649,650,1,0,0,0,650,652,1,0,0,0,651,642,1,0,0,0,
	652,655,1,0,0,0,653,651,1,0,0,0,653,654,1,0,0,0,654,663,1,0,0,0,655,653,
	1,0,0,0,656,657,5,22,0,0,657,661,5,38,0,0,658,659,3,38,19,0,659,660,6,29,
	-1,0,660,662,1,0,0,0,661,658,1,0,0,0,661,662,1,0,0,0,662,664,1,0,0,0,663,
	656,1,0,0,0,663,664,1,0,0,0,664,665,1,0,0,0,665,666,5,42,0,0,666,667,6,
	29,-1,0,667,59,1,0,0,0,668,669,5,23,0,0,669,670,6,30,-1,0,670,671,5,36,
	0,0,671,672,6,30,-1,0,672,61,1,0,0,0,673,674,3,64,32,0,674,675,6,31,-1,
	0,675,63,1,0,0,0,676,677,3,66,33,0,677,685,6,32,-1,0,678,679,5,37,0,0,679,
	680,6,32,-1,0,680,681,3,66,33,0,681,682,6,32,-1,0,682,684,1,0,0,0,683,678,
	1,0,0,0,684,687,1,0,0,0,685,683,1,0,0,0,685,686,1,0,0,0,686,65,1,0,0,0,
	687,685,1,0,0,0,688,689,3,68,34,0,689,704,6,33,-1,0,690,691,7,2,0,0,691,
	692,6,33,-1,0,692,693,3,68,34,0,693,694,6,33,-1,0,694,703,1,0,0,0,695,696,
	5,49,0,0,696,697,6,33,-1,0,697,698,3,62,31,0,698,699,5,38,0,0,699,700,3,
	62,31,0,700,701,6,33,-1,0,701,703,1,0,0,0,702,690,1,0,0,0,702,695,1,0,0,
	0,703,706,1,0,0,0,704,702,1,0,0,0,704,705,1,0,0,0,705,67,1,0,0,0,706,704,
	1,0,0,0,707,708,3,70,35,0,708,716,6,34,-1,0,709,710,5,67,0,0,710,711,6,
	34,-1,0,711,712,3,70,35,0,712,713,6,34,-1,0,713,715,1,0,0,0,714,709,1,0,
	0,0,715,718,1,0,0,0,716,714,1,0,0,0,716,717,1,0,0,0,717,69,1,0,0,0,718,
	716,1,0,0,0,719,720,3,72,36,0,720,728,6,35,-1,0,721,722,5,66,0,0,722,723,
	6,35,-1,0,723,724,3,72,36,0,724,725,6,35,-1,0,725,727,1,0,0,0,726,721,1,
	0,0,0,727,730,1,0,0,0,728,726,1,0,0,0,728,729,1,0,0,0,729,71,1,0,0,0,730,
	728,1,0,0,0,731,732,3,74,37,0,732,740,6,36,-1,0,733,734,5,52,0,0,734,735,
	6,36,-1,0,735,736,3,74,37,0,736,737,6,36,-1,0,737,739,1,0,0,0,738,733,1,
	0,0,0,739,742,1,0,0,0,740,738,1,0,0,0,740,741,1,0,0,0,741,73,1,0,0,0,742,
	740,1,0,0,0,743,744,3,76,38,0,744,752,6,37,-1,0,745,746,5,54,0,0,746,747,
	6,37,-1,0,747,748,3,76,38,0,748,749,6,37,-1,0,749,751,1,0,0,0,750,745,1,
	0,0,0,751,754,1,0,0,0,752,750,1,0,0,0,752,753,1,0,0,0,753,75,1,0,0,0,754,
	752,1,0,0,0,755,756,3,78,39,0,756,764,6,38,-1,0,757,758,5,50,0,0,758,759,
	6,38,-1,0,759,760,3,78,39,0,760,761,6,38,-1,0,761,763,1,0,0,0,762,757,1,
	0,0,0,763,766,1,0,0,0,764,762,1,0,0,0,764,765,1,0,0,0,765,77,1,0,0,0,766,
	764,1,0,0,0,767,768,3,80,40,0,768,776,6,39,-1,0,769,770,7,3,0,0,770,771,
	6,39,-1,0,771,772,3,80,40,0,772,773,6,39,-1,0,773,775,1,0,0,0,774,769,1,
	0,0,0,775,778,1,0,0,0,776,774,1,0,0,0,776,777,1,0,0,0,777,79,1,0,0,0,778,
	776,1,0,0,0,779,780,3,82,41,0,780,788,6,40,-1,0,781,782,7,4,0,0,782,783,
	6,40,-1,0,783,784,3,82,41,0,784,785,6,40,-1,0,785,787,1,0,0,0,786,781,1,
	0,0,0,787,790,1,0,0,0,788,786,1,0,0,0,788,789,1,0,0,0,789,81,1,0,0,0,790,
	788,1,0,0,0,791,792,3,84,42,0,792,800,6,41,-1,0,793,794,7,5,0,0,794,795,
	6,41,-1,0,795,796,3,84,42,0,796,797,6,41,-1,0,797,799,1,0,0,0,798,793,1,
	0,0,0,799,802,1,0,0,0,800,798,1,0,0,0,800,801,1,0,0,0,801,83,1,0,0,0,802,
	800,1,0,0,0,803,804,3,86,43,0,804,812,6,42,-1,0,805,806,7,6,0,0,806,807,
	6,42,-1,0,807,808,3,86,43,0,808,809,6,42,-1,0,809,811,1,0,0,0,810,805,1,
	0,0,0,811,814,1,0,0,0,812,810,1,0,0,0,812,813,1,0,0,0,813,85,1,0,0,0,814,
	812,1,0,0,0,815,816,3,88,44,0,816,824,6,43,-1,0,817,818,7,7,0,0,818,819,
	6,43,-1,0,819,820,3,88,44,0,820,821,6,43,-1,0,821,823,1,0,0,0,822,817,1,
	0,0,0,823,826,1,0,0,0,824,822,1,0,0,0,824,825,1,0,0,0,825,87,1,0,0,0,826,
	824,1,0,0,0,827,828,3,90,45,0,828,867,6,44,-1,0,829,830,5,39,0,0,830,842,
	6,44,-1,0,831,832,3,68,34,0,832,839,6,44,-1,0,833,834,5,37,0,0,834,835,
	3,68,34,0,835,836,6,44,-1,0,836,838,1,0,0,0,837,833,1,0,0,0,838,841,1,0,
	0,0,839,837,1,0,0,0,839,840,1,0,0,0,840,843,1,0,0,0,841,839,1,0,0,0,842,
	831,1,0,0,0,842,843,1,0,0,0,843,844,1,0,0,0,844,845,5,40,0,0,845,866,6,
	44,-1,0,846,847,5,45,0,0,847,848,6,44,-1,0,848,849,3,66,33,0,849,850,6,
	44,-1,0,850,866,1,0,0,0,851,852,5,75,0,0,852,853,6,44,-1,0,853,854,3,66,
	33,0,854,855,6,44,-1,0,855,866,1,0,0,0,856,857,5,43,0,0,857,861,6,44,-1,
	0,858,859,3,62,31,0,859,860,6,44,-1,0,860,862,1,0,0,0,861,858,1,0,0,0,861,
	862,1,0,0,0,862,863,1,0,0,0,863,864,5,44,0,0,864,866,6,44,-1,0,865,829,
	1,0,0,0,865,846,1,0,0,0,865,851,1,0,0,0,865,856,1,0,0,0,866,869,1,0,0,0,
	867,865,1,0,0,0,867,868,1,0,0,0,868,881,1,0,0,0,869,867,1,0,0,0,870,875,
	3,90,45,0,871,872,7,8,0,0,872,874,6,44,-1,0,873,871,1,0,0,0,874,877,1,0,
	0,0,875,873,1,0,0,0,875,876,1,0,0,0,876,878,1,0,0,0,877,875,1,0,0,0,878,
	879,6,44,-1,0,879,881,1,0,0,0,880,827,1,0,0,0,880,870,1,0,0,0,881,89,1,
	0,0,0,882,883,7,9,0,0,883,885,6,45,-1,0,884,882,1,0,0,0,885,888,1,0,0,0,
	886,884,1,0,0,0,886,887,1,0,0,0,887,889,1,0,0,0,888,886,1,0,0,0,889,890,
	3,94,47,0,890,891,6,45,-1,0,891,91,1,0,0,0,892,893,3,94,47,0,893,894,5,
	35,0,0,894,895,3,94,47,0,895,896,6,46,-1,0,896,93,1,0,0,0,897,898,3,96,
	48,0,898,899,6,47,-1,0,899,927,1,0,0,0,900,901,3,98,49,0,901,902,6,47,-1,
	0,902,927,1,0,0,0,903,904,5,39,0,0,904,905,3,62,31,0,905,906,5,40,0,0,906,
	907,6,47,-1,0,907,927,1,0,0,0,908,909,5,76,0,0,909,927,6,47,-1,0,910,911,
	5,77,0,0,911,927,6,47,-1,0,912,913,5,80,0,0,913,927,6,47,-1,0,914,915,5,
	82,0,0,915,927,6,47,-1,0,916,917,5,83,0,0,917,927,6,47,-1,0,918,919,3,0,
	0,0,919,920,6,47,-1,0,920,927,1,0,0,0,921,922,5,84,0,0,922,927,6,47,-1,
	0,923,924,3,2,1,0,924,925,6,47,-1,0,925,927,1,0,0,0,926,897,1,0,0,0,926,
	900,1,0,0,0,926,903,1,0,0,0,926,908,1,0,0,0,926,910,1,0,0,0,926,912,1,0,
	0,0,926,914,1,0,0,0,926,916,1,0,0,0,926,918,1,0,0,0,926,921,1,0,0,0,926,
	923,1,0,0,0,927,95,1,0,0,0,928,929,5,84,0,0,929,930,6,48,-1,0,930,931,5,
	41,0,0,931,943,6,48,-1,0,932,933,3,68,34,0,933,940,6,48,-1,0,934,935,5,
	37,0,0,935,936,3,68,34,0,936,937,6,48,-1,0,937,939,1,0,0,0,938,934,1,0,
	0,0,939,942,1,0,0,0,940,938,1,0,0,0,940,941,1,0,0,0,941,944,1,0,0,0,942,
	940,1,0,0,0,943,932,1,0,0,0,943,944,1,0,0,0,944,945,1,0,0,0,945,946,5,42,
	0,0,946,947,6,48,-1,0,947,97,1,0,0,0,948,949,5,43,0,0,949,961,6,49,-1,0,
	950,951,3,68,34,0,951,958,6,49,-1,0,952,953,5,37,0,0,953,954,3,68,34,0,
	954,955,6,49,-1,0,955,957,1,0,0,0,956,952,1,0,0,0,957,960,1,0,0,0,958,956,
	1,0,0,0,958,959,1,0,0,0,959,962,1,0,0,0,960,958,1,0,0,0,961,950,1,0,0,0,
	961,962,1,0,0,0,962,963,1,0,0,0,963,964,5,44,0,0,964,965,6,49,-1,0,965,
	99,1,0,0,0,76,105,114,118,124,166,168,199,208,213,215,226,234,243,250,261,
	264,273,281,301,304,306,315,328,337,347,352,355,365,377,386,400,403,420,
	433,451,469,506,523,533,539,545,553,595,607,609,620,649,653,661,663,685,
	702,704,716,728,740,752,764,776,788,800,812,824,839,842,861,865,867,875,
	880,886,926,940,943,958,961];

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
	public _returnStatement!: ReturnStatementContext;
	public _discardStatement!: DiscardStatementContext;
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
	public returnStatement(): ReturnStatementContext {
		return this.getTypedRuleContext(ReturnStatementContext, 0) as ReturnStatementContext;
	}
	public discardStatement(): DiscardStatementContext {
		return this.getTypedRuleContext(DiscardStatementContext, 0) as DiscardStatementContext;
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


export class ReturnStatementContext extends ParserRuleContext {
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
    	return GPULangParser.RULE_returnStatement;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterReturnStatement) {
	 		listener.enterReturnStatement(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitReturnStatement) {
	 		listener.exitReturnStatement(this);
		}
	}
}


export class DiscardStatementContext extends ParserRuleContext {
	public tree: Statement*;
	constructor(parser?: GPULangParser, parent?: ParserRuleContext, invokingState?: number) {
		super(parent, invokingState);
    	this.parser = parser;
	}
	public SC(): TerminalNode {
		return this.getToken(GPULangParser.SC, 0);
	}
    public get ruleIndex(): number {
    	return GPULangParser.RULE_discardStatement;
	}
	public enterRule(listener: GPULangListener): void {
	    if(listener.enterDiscardStatement) {
	 		listener.enterDiscardStatement(this);
		}
	}
	public exitRule(listener: GPULangListener): void {
	    if(listener.exitDiscardStatement) {
	 		listener.exitDiscardStatement(this);
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
	public _e2!: AssignmentExpressionContext;
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
	public assignmentExpression_list(): AssignmentExpressionContext[] {
		return this.getTypedRuleContexts(AssignmentExpressionContext) as AssignmentExpressionContext[];
	}
	public assignmentExpression(i: number): AssignmentExpressionContext {
		return this.getTypedRuleContext(AssignmentExpressionContext, i) as AssignmentExpressionContext;
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
