grammar GPULang;

options {
    language = Cpp;
    backtracking = true;
}

// Lexer API hooks
@lexer::apifuncs {

}

@lexer::members {
#include <iostream>

misc::Interval interval;
std::string currentFile;
int currentLine = 0;
}

@lexer::header {
    #include <string>
    #include <vector>
    #include "gpulangtoken.h"
}

// parser API hooks
@parser::apifuncs {

}

@parser::members {

// setup function which binds the compiler state to the current AST node
Symbol::Location
SetupFile(bool updateLine = true)
{
    Symbol::Location location;
    if (this->lines.empty())
        return location;
    ::GPULangToken* token = (::GPULangToken*)_input->LT(-1);

    if (updateLine)
        UpdateLine(_input, -1);

    // assume the previous token is the latest file
    auto tu2 = this->lines[this->currentLine];
    location.file = std::get<4>(tu2);
    location.line = lineOffset;
    location.column = token->getCharPositionInLine();
    return location;
}

// update and get current line
void
UpdateLine(antlr4::TokenStream* stream, int index = -1)
{
    ::GPULangToken* token = (::GPULangToken*)stream->LT(index);

      // find the next parsed row which comes after the token
      int loop = this->currentLine;
      int tokenLine = token->getLine();
      while (loop < this->lines.size() - 1)
      {
          // look ahead, if the next line is beyond the token, abort
          if (std::get<1>(this->lines[loop + 1]) > tokenLine)
              break;
          else
              loop++;
      }

      auto line = this->lines[loop];
      this->currentLine = loop;

      // where the target compiler expects the output token to be and where we put it may differ
      // so we calculate a padding between the token and the #line directive output by the preprocessing stage (which includes the #line token line)
      int padding = (tokenLine - 1) - std::get<1>(line);
      this->lineOffset = std::get<0>(line) + padding;
}



int currentLine = 0;
int lineOffset = 0;
std::vector<std::tuple<int, size_t, size_t, size_t, std::string>> lines;
}

// parser includes
@parser::header {

// include std container
#include <vector>
#include <typeinfo>
#include <iostream>
#include <string>
#include <stack>
#include <tuple>

#include "gpulangtoken.h"
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
#include "ast/expressions/intexpression.h"
#include "ast/expressions/stringexpression.h"
#include "ast/expressions/symbolexpression.h"
#include "ast/expressions/ternaryexpression.h"
#include "ast/expressions/uintexpression.h"
#include "ast/expressions/unaryexpression.h"
#include "util.h"

using namespace GPULang;

}

string
    returns[ std::string val ]:
    '"' (data = ~'"' { $val.append($data.text); })* '"'
    | '\'' (data = ~'\'' { $val.append($data.text); })* '\'';

boolean
    returns[ bool val ]
    @init {
        $val = false;
    }: 'true' { $val = true; } | 'false' { $val = false; };

// preprocess
preprocess
    @init {
        Token* start = nullptr;
    }:
    (
        { start = _input->LT(1); } '#line' line = INTEGERLITERAL path = string { this->lines.push_back(std::make_tuple(atoi($line.text.c_str()), _input->LT(-1)->getLine(), start->getStartIndex(), _input->LT(1)->getStartIndex(), $path.val)); }
        | .
    )*? EOF;

// main entry point
entry
    returns[ Effect* returnEffect ]:
    effect {
        $returnEffect = $effect.eff;
    } EOF;

// entry point for effect, call this function to begin parsing
effect
    returns[ Effect* eff ]
    @init
    {
        $eff = new Effect();
    }
    :
    (
        alias ';'                   { $eff->symbols.push_back($alias.sym); }
        | functionDeclaration ';'   { $eff->symbols.push_back($functionDeclaration.sym); }    
        | function                  { $eff->symbols.push_back($function.sym); }    
        | variables ';'             { for (Variable* var : $variables.list) { $eff->symbols.push_back(var); } }
        | structure ';'             { $eff->symbols.push_back($structure.sym); }
        | enumeration ';'           { $eff->symbols.push_back($enumeration.sym); }
        | state ';'                 { $eff->symbols.push_back($state.sym); }
        | program ';'               { $eff->symbols.push_back($program.sym); }
    )*?;

alias
    returns[ Alias* sym ]
    @init
    {
        $sym = nullptr;
        std::string name;
        std::string type;
    }
    : 'alias' name = IDENTIFIER 'as' type = IDENTIFIER { name = $name.text; type = $type.text; }
    {
        $sym = new Alias();
        $sym->name = name;
        $sym->type = type;
    }
    ;

// metarule for annotation - API layer data to be passed to program reading shader
annotation
    returns[ Annotation annot ]:
        ('@' (name = IDENTIFIER '(' value = expression ')' { $annot.name = $name.text; $annot.value = $value.tree; }))
    ;
    
// metarule for attribute - compiler layer data to be passed to target language compiler
attribute
    returns[ Attribute attr ]: 
    name = IDENTIFIER '(' expression ')' { $attr.name = $name.text; $attr.expression = $expression.tree; } 
    | name = IDENTIFIER { $attr.name = $name.text; $attr.expression = nullptr; }
    ;

typeDeclaration
    returns[ Type::FullType type ]
    @init
    {
        $type.name = "";
    }:
    ( 
        '*' { $type.AddModifier(Type::FullType::Modifier::Pointer); } |
        '[' { $type.AddModifier(Type::FullType::Modifier::Array); } 
                ( arraySize0 = INTEGERLITERAL { $type.UpdateValue(atoi($arraySize0.text.c_str())); } )? 
        ']'
        | IDENTIFIER { $type.AddQualifier($IDENTIFIER.text); }
    )* 
    typeName = IDENTIFIER { $type.name = $typeName.text; }
    ;

    // Variable declaration <annotation>* <attribute>* instance0, .. instanceN : <type_modifiers> <type> 
variables
    returns[ std::vector<Variable*> list ]
    @init
    {
        std::vector<Annotation> annotations;
        std::vector<Attribute> attributes;
        std::vector<std::string> names;
        std::vector<Expression*> valueExpressions;
        std::vector<Symbol::Location> locations;
        unsigned initCounter = 0;
        Type::FullType type = { "unknown" };
    }:
    (annotation { annotations.push_back($annotation.annot); })*
    (attribute { attributes.push_back($attribute.attr); })*
    
    varName = IDENTIFIER { names.push_back($varName.text); valueExpressions.push_back(nullptr); locations.push_back(SetupFile()); } 
    (',' varNameN = IDENTIFIER { names.push_back($varNameN.text); valueExpressions.push_back(nullptr); locations.push_back(SetupFile()); })*
    
    ( ':' 
       typeDeclaration { type = $typeDeclaration.type; }
    )?
    (
        '=' valueExpr = assignmentExpression { if (initCounter == names.size()) { valueExpressions.push_back(nullptr); } valueExpressions[initCounter++] = $valueExpr.tree; } 
        (',' valueExprN = assignmentExpression { if (initCounter == names.size()) { valueExpressions.push_back(nullptr); } valueExpressions[initCounter++] = $valueExprN.tree; } )*
    )?
    {
        for (size_t i = 0; i < names.size(); i++)
        {
            Variable* var = new Variable(); 
            var->type = type; 
            var->location = locations[i]; 
            var->annotations = annotations; 
            var->attributes = attributes;
            var->name = names[i];
            var->valueExpression = valueExpressions[i];
            $list.push_back(var);
        }
    }
    ;

structureDeclaration
    returns[ Structure* sym ]
    @init
    {
        $sym = nullptr;
        std::vector<Annotation> annotations;
        std::vector<Attribute> attributes;
    }:
    (annotation { annotations.push_back($annotation.annot); })*
    'struct' 
    (attribute { attributes.push_back($attribute.attr); })*
    name = IDENTIFIER 
    { 
        $sym = new Structure();
        $sym->name = $name.text; 
        $sym->annotations = annotations;
        $sym->attributes = attributes;
        $sym->location = SetupFile();
    }
    ;

structure
    returns[ Structure* sym ]
    @init
    {
        $sym = nullptr;
        std::vector<Symbol*> members;
        bool isArray = false;
        Expression* arraySizeExpression = nullptr;
        std::string instanceName;
        Symbol::Location varLocation;
        Type::FullType varType;
        std::string varName;
    }:
    structureDeclaration { $sym = $structureDeclaration.sym; }
    '{' 
        (varName = IDENTIFIER { varName = $varName.text; varLocation = SetupFile(); } ':'         
            ( 
                '*' { varType.AddModifier(Type::FullType::Modifier::Pointer); } |
                '[' { varType.AddModifier(Type::FullType::Modifier::Array); } 
                        ( arraySize0 = INTEGERLITERAL { varType.UpdateValue(atoi($arraySize0.text.c_str())); } )? 
                ']'
            )* 
            varTypeName = IDENTIFIER { varType.name = $varTypeName.text; } ';'
            {
                Variable* var = new Variable(); 
                var->type = varType; 
                var->location = varLocation; 
                var->name = varName;
                var->valueExpression = nullptr;
                members.push_back(var);
            }
        )*
    '}' 
    // Disable tail as structs can't be created locally (yet)
    // tail, like } myStruct[];
    /*
    (
        instanceName = IDENTIFIER { instanceName = $instanceName.text; }
        ( '[' (expression { arraySizeExpression = $expression.tree; })? ']' { isArray = true; } )?
    )?
    */
    { 
        $sym->symbols = members; 
        //$sym->instanceName = instanceName;
        //$sym->isArray = isArray;
        //$sym->arraySizeExpression = arraySizeExpression;
    }
    ;

enumeration
    returns[ Enumeration* sym ]
    @init
    {
        $sym = nullptr;
        std::vector<std::string> enumLabels;
        std::vector<Expression*> enumValues;
        std::string name;
        Type::FullType type = { "u32" };
        Symbol::Location location;
    }:
    'enum' name = IDENTIFIER { name = $name.text; location = SetupFile(); }
    (':' typeDeclaration { type = $typeDeclaration.type; })?
    '{'
        label = IDENTIFIER { Expression* expr = nullptr; } ('=' value = expression { expr = $value.tree; })?
        {
            enumLabels.push_back($label.text);
            enumValues.push_back(expr);
        }
    '}'
    {
        $sym = new Enumeration();
        $sym->name = name;
        type.literal = true;
        $sym->type = type;
        $sym->labels = enumLabels;
        $sym->values = enumValues;
        $sym->location = location;
    }
    ;

// Variable declaration <annotation>* <attribute>* instance0, .. instanceN : <type_modifiers> <type> 
parameter
    returns[ Variable* sym ]
    @init
    {
        std::vector<Annotation> annotations;
        std::vector<Attribute> attributes;
        std::string name;
        Expression* valueExpression = nullptr;
        Symbol::Location location;
        Type::FullType type = { "unknown" };
    }:
    (attribute { attributes.push_back($attribute.attr); })*
    
    varName = IDENTIFIER { name = $varName.text; location = SetupFile(); } 
    ':' 
    typeDeclaration { type = $typeDeclaration.type; }
    (
        '=' valueExpr = assignmentExpression { valueExpression = $valueExpr.tree; } 
    )?
    {
            $sym = new Variable(); 
            $sym->type = type; 
            $sym->location = location; 
            $sym->attributes = attributes;
            $sym->name = name;
            $sym->valueExpression = valueExpression;
    }
    ;

functionDeclaration
    returns[ Function* sym ]
    @init
    {
        $sym = nullptr;
        std::vector<Variable*> variables;
        std::vector<Attribute> attributes;
        Symbol::Location location;
    }:
    (attribute { attributes.push_back($attribute.attr); })*
    name = IDENTIFIER { location = SetupFile(); } '(' (arg0 = parameter { variables.push_back($arg0.sym); } (',' argn = parameter { variables.push_back($argn.sym); })* )? ')' returnType = typeDeclaration
    {
        $sym = new Function(); 
        $sym->hasBody = false;
        $sym->location = location;
        $sym->returnType = $returnType.type; 
        $sym->name = $name.text; 
        $sym->parameters = variables; 
        $sym->attributes = attributes; 
    }
    ;

// metarule for the code content of a function
codeblock: '{' (codeblock)* '}' | ~('{' | '}');

function
    returns[ Function* sym ]
    @init
    {
        $sym = nullptr;
        Token* startToken = nullptr;
        Token* endToken = nullptr;
    }:
    functionDeclaration { $sym = $functionDeclaration.sym; }
    {
        startToken = _input->LT(2);
    }
    scopeStatement
    {
        endToken = _input->LT(-2);

        // extract code from between tokens
        antlr4::misc::Interval interval;
        interval.a = startToken->getTokenIndex();
        interval.b = endToken->getTokenIndex();
        $sym->body = _input->getText(interval);
        $sym->hasBody = true;
        $sym->ast = $scopeStatement.tree;
    } 
    ;

program
    returns[ Program* sym ]
    @init
    {
        $sym = nullptr;
        Symbol::Location location;
        std::vector<Program::SubroutineMapping> subroutines;
        std::vector<Expression*> entries;
        std::vector<Annotation> annotations;
    }:
    (annotation { annotations.push_back($annotation.annot); })*
    'program' name = IDENTIFIER { location = SetupFile(); }
    '{'
        ( assignment = expression { entries.push_back($assignment.tree); } ';' )*
    '}'
    { 
        $sym = new Program();
        $sym->location = location;
        $sym->name = $name.text;
        $sym->annotations = annotations;
        $sym->entries = entries;
    }
    ;

state
    returns[ State* sym ]
    @init
    {
        Expression* arrayExpression = nullptr;
        std::vector<Expression*> entries;
    }:
    (
        'sampler' { $sym = new SamplerState(); }
        | 'render_state' { $sym = new RenderState(); } 
    ) name = IDENTIFIER { $sym->location = SetupFile(); }
    '{'
        (assign = expression { entries.push_back($assign.tree); } ';' )*
    '}'
    {
        $sym->name = $name.text;
        $sym->entries = entries;
    }
    ;

statement
    returns[ Statement* tree ]
    @init
    {
        $tree = nullptr;
    }:
    ifStatement               { $tree = $ifStatement.tree; }
    | scopeStatement            { $tree = $scopeStatement.tree; }
    | forStatement              { $tree = $forStatement.tree; }
    | whileStatement            { $tree = $whileStatement.tree; }
    | switchStatement           { $tree = $switchStatement.tree; }
    | returnStatement           { $tree = $returnStatement.tree; }
    | continueStatement         { $tree = $continueStatement.tree; }
    | breakStatement            { $tree = $breakStatement.tree; }
    | expressionStatement ';'   { $tree = $expressionStatement.tree; }
    ;

// expression list as a statement, basically supposing the expression will have a side effect
expressionStatement
    returns[ Statement* tree ]
    @init 
    {
        $tree = nullptr;
    }: 
    expression
    {
        $tree = new ExpressionStatement($expression.tree);
        $tree->location = SetupFile();
    }
    ;


ifStatement
    returns[ Statement* tree ]
    @init
    {
        $tree = nullptr;
        Expression* condition = nullptr;
        Statement* ifBody = nullptr;
        Statement* elseBody = nullptr;
        Symbol::Location location;
    }:
    'if' { location = SetupFile(); } '(' condition = expression { condition = $condition.tree; } ')' 
    ifBody = statement { ifBody = $ifBody.tree; }
    
    ('else' elseBody = statement { elseBody = $elseBody.tree; })?
    {
        $tree = new IfStatement(condition, ifBody, elseBody);
        $tree->location = location;
    }
    ;

forStatement
    returns[ Statement* tree ]
    @init
    {
        $tree = nullptr;
        std::vector<Variable*> declarations;
        Expression* conditionExpression = nullptr;
        Expression* loopExpression = nullptr;
        Statement* contents = nullptr;
        Symbol::Location location;
    }:
    'for' { location = SetupFile(); }
    '(' 
        (variables { declarations = $variables.list; })? ';'
        (condition = expression { conditionExpression = $condition.tree; })? ';' 
        (loop = expression      { loopExpression = $loop.tree; })?
    ')'
    content = statement { contents = $content.tree; }
    {
        $tree = new ForStatement(declarations, conditionExpression, loopExpression, contents);
        $tree->location = location;
    }
    ;

forRangeStatement
    returns[ Statement* tree ]
    @init
    {
        $tree = nullptr;
        Statement* contents = nullptr;
        Symbol::Location location;
    }:
    'for' { location = SetupFile(); } '(' iterator = IDENTIFIER ':' start = IDENTIFIER '..' end = IDENTIFIER ')'
    content = statement { contents = $content.tree; }
    {

    }
    ;

whileStatement
    returns[ Statement* tree ]
    @init
    {
        $tree = nullptr;
        Expression* conditionExpression = nullptr;
        Statement* contents = nullptr;
        bool isDoWhile = false;
        Symbol::Location location;
    }:
    'while' { location = SetupFile(); } '(' condition = expression { conditionExpression = $condition.tree; } ')'
    content = statement { contents = $content.tree; }
    {
        $tree = new WhileStatement(conditionExpression, contents, isDoWhile);
        $tree->location = location;
    }
    | 'do' { location = SetupFile(); }
    content = statement { contents = $content.tree; isDoWhile = true; } 
    'while' '(' condition = expression { conditionExpression = $condition.tree; } ')' ';'
    {
        $tree = new WhileStatement(conditionExpression, contents, isDoWhile);
        $tree->location = location;
    }
    ;

scopeStatement
    returns[ Statement* tree ]
    @init
    {
        $tree = nullptr;
        std::vector<Symbol*> contents;
        Symbol::Location location;
    }:
    '{' { location = SetupFile(); }
    (
        variables ';' { for(Variable* var : $variables.list) { contents.push_back(var); } }
        | statement { contents.push_back($statement.tree); }
    )* 
    '}'
    {
        $tree = new ScopeStatement(contents);
        $tree->location = location;
    }
    ;

returnStatement
    returns[ Statement* tree ]
    @init
    {
        $tree = nullptr;
        Expression* returnValue = nullptr;
        Symbol::Location location;
    }:
    'return' { location = SetupFile(); } (value = expression { returnValue = $value.tree; })? ';'
    {
        $tree = new ReturnStatement(returnValue);
        $tree->location = location;
    }
    ;

continueStatement
    returns[ Statement* tree ]
    @init 
    {
        $tree = nullptr;
        Symbol::Location location;
    }: 
    'continue' { location = SetupFile(); } ';' 
    {
        $tree = new ContinueStatement();
        $tree->location = location;
    }
    ;

switchStatement
    returns[ Statement* tree ]
    @init
    {
        $tree = nullptr;
        Expression* switchExpression;
        std::vector<Expression*> caseExpressions;
        std::vector<Statement*> caseStatements;
        Symbol::Location location;
        Statement* defaultStatement = nullptr;
    }:
    'switch' { location = SetupFile(); } '(' expression ')' { switchExpression = $expression.tree; }
    '{'
        (
            'case' INTEGERLITERAL ':' { caseExpressions.push_back(new IntExpression(atoi($INTEGERLITERAL.text.c_str()))); caseStatements.push_back(nullptr); }
            (statement
            { 
                caseStatements.back() = $statement.tree;
            })?
        )*
        (
            'default' ':'
            (statement
            {
                defaultStatement = $statement.tree;
            })?
        )?
    '}'
    {
        $tree = new SwitchStatement(switchExpression, caseExpressions, caseStatements, defaultStatement);
        $tree->location = location;
    }
    ;

breakStatement
    returns[ Statement* tree ]
    @init 
    {
        $tree = nullptr;
        Symbol::Location location;
    }: 
    'break' { location = SetupFile(); } ';'
    {
        $tree = new BreakStatement();
        $tree->location = location;
    }
    ;

// an expression is any symbol that can evaluate to a certain value or type
expression
    returns[ Expression* tree ]
    @init 
    {
        $tree = nullptr;
    }: 
    commaExpression { $tree = $commaExpression.tree; $tree->location = SetupFile(); }
    ;

commaExpression
    returns[ Expression* tree ]
    @init
    {
        $tree = nullptr;
        Symbol::Location location;
    }:
    e1 = assignmentExpression { $tree = $e1.tree; }
    (
        ',' { location = SetupFile(); } e2 = assignmentExpression
        {
            CommaExpression* expr = new CommaExpression($tree, $e2.tree);
            expr->location = location;
            $tree = expr;
        }
    )*
    ;

// start of with ||
assignmentExpression
    returns[ Expression* tree ]
    @init 
    {
        $tree = nullptr;
        Symbol::Location location;
    }:
    e1 = logicalOrExpression { $tree = $e1.tree; } 
    (
        op = ('+=' | '-=' | '*=' | '/=' | '%=' | '<<=' | '>>=' | '&=' | '^=' | '|=' | '=') e2 = logicalOrExpression
        { 
            BinaryExpression* expr = new BinaryExpression(StringToFourCC($op.text), $tree, $e2.tree);
            expr->location = SetupFile();
            $tree = expr;
        } 
        | '?' { location = SetupFile(); } ifBody = expression ':' elseBody = expression
        { 
            TernaryExpression* expr = new TernaryExpression($tree, $ifBody.tree, $elseBody.tree);
            expr->location = location;
            $tree = expr;
        }
    )*
    ;

// start of with ||
logicalOrExpression
    returns[ Expression* tree ]
    @init 
    {
        $tree = nullptr;
        Symbol::Location location;
    }:
    e1 = logicalAndExpression { $tree = $e1.tree; } 
    (
        ('||') { location = SetupFile(); } e2 = logicalAndExpression
        {
            BinaryExpression* expr = new BinaryExpression('||', $tree, $e2.tree);
            expr->location = location;
            $tree = expr;
        }
    )*
    ;

// then solve &&
logicalAndExpression
    returns[ Expression* tree ]
    @init 
    {
        $tree = nullptr;
        Symbol::Location location;

    }:
    e1 = orExpression { $tree = $e1.tree; } 
    (
        ('&&') { location = SetupFile(); } e2 = orExpression
        {
            BinaryExpression* expr = new BinaryExpression('&&', $tree, $e2.tree);
            expr->location = location;
            $tree = expr;
        }
    )*
    ;

// |
orExpression
    returns[ Expression* tree ]
    @init 
    {
        $tree = nullptr;
        Symbol::Location location;

    }:
    e1 = xorExpression { $tree = $e1.tree; } 
    (
        '|' { location = SetupFile(); } e2 = xorExpression
        {
            BinaryExpression* expr = new BinaryExpression('|', $tree, $e2.tree);
            expr->location = location;
            $tree = expr;
        }
    )*
    ;

// ^
xorExpression
    returns[ Expression* tree ]
    @init 
    {
        $tree = nullptr;
        Symbol::Location location;
    }:
    e1 = andExpression { $tree = $e1.tree; } 
    (
        '^' { location = SetupFile(); } e2 = andExpression
        {
            BinaryExpression* expr = new BinaryExpression('^', $tree, $e2.tree);
            expr->location = location;
            $tree = expr;
        }
    )*
    ;

// &
andExpression
    returns[ Expression* tree ]
    @init 
    {
        $tree = nullptr;
        Symbol::Location location;
    }:
    e1 = equivalencyExpression { $tree = $e1.tree;	} 
    (
        '&' { location = SetupFile(); } e2 = equivalencyExpression
        {
            BinaryExpression* expr = new BinaryExpression('&', $tree, $e2.tree);
            expr->location = location;
            $tree = expr;
        }
    )*
    ;

// == and !=
equivalencyExpression
    returns[ Expression* tree ]
    @init 
    {
        $tree = nullptr;
        Symbol::Location location;
    }:
    e1 = relationalExpression { $tree = $e1.tree; } 
    (
        op = ('==' | '!=')  { location = SetupFile(); } e2 = relationalExpression
        {
            BinaryExpression* expr = new BinaryExpression(StringToFourCC($op.text), $tree, $e2.tree);
            expr->location = location;
            $tree = expr;
        }
    )*
    ;

// <, >, <= and >=
relationalExpression
    returns[ Expression* tree ]
    @init 
    {
        $tree = nullptr;
        Symbol::Location location;
    }:
    e1 = shiftExpression { $tree = $e1.tree; } 
    (
        op = ('<' | '>' | '<=' | '>=' ) { location = SetupFile(); } e2 = shiftExpression
        {
            BinaryExpression* expr = new BinaryExpression(StringToFourCC($op.text), $tree, $e2.tree);
            expr->location = location;
            $tree = expr;
        }
    )*
    ;

// <<, >>
shiftExpression
    returns[ Expression* tree ]
    @init 
    {
        $tree = nullptr;
        Symbol::Location location;
    }:
    e1 = addSubtractExpression { $tree = $e1.tree; } 
    (
        op = ('<<' | '>>') { location = SetupFile(); } e2 = addSubtractExpression 
        {
            BinaryExpression* expr = new BinaryExpression(StringToFourCC($op.text), $tree, $e2.tree);
            expr->location = location;
            $tree = expr;
        }
    )*
    ;

// + and -
addSubtractExpression
    returns[ Expression* tree ]
    @init 
    {
        $tree = nullptr;
        Symbol::Location location;
    }:
    e1 = multiplyDivideExpression { $tree = $e1.tree; } 
    (
        op = ('+' | '-') { location = SetupFile(); } e2 = multiplyDivideExpression 
        {
            BinaryExpression* expr = new BinaryExpression(StringToFourCC($op.text), $tree, $e2.tree);
            expr->location = location;
            $tree = expr;
        }
    )*
    ;

// * and /
multiplyDivideExpression
    returns[ Expression* tree ]
    @init 
    {
        $tree = nullptr;
        Symbol::Location location;
    }:
    e1 = prefixExpression { $tree = $e1.tree; }
    (
        op = ('*' | '/' | '%') { location = SetupFile(); } e2 = prefixExpression 
        {
            BinaryExpression* expr = new BinaryExpression(StringToFourCC($op.text), $tree, $e2.tree);
            expr->location = location;
            $tree = expr;
        }
    )*
    ;

// unary expressions. Create chain of unary expressions by removing one token from the left and create new unary expressions
prefixExpression
    returns[ Expression* tree ]
    @init 
    {
        $tree = nullptr;
        std::vector<uint32_t> ops;
        std::vector<Symbol::Location> locations;
    }:
    (op = ('-' | '+' | '!' | '~' | '++' | '--' | '*') { ops.push_back(StringToFourCC($op.text)); locations.push_back(SetupFile()); } )* e1 = suffixExpression 
    {
        $tree = $e1.tree;
        $tree->location = SetupFile();
        for (size_t i = 0; i < ops.size(); i++)
        {
            $tree = new UnaryExpression(ops[i], true, $tree);
            $tree->location = locations[i];
        }
    }
    ;

// unary expressions. Create chain of unary expressions by removing one token from the left and create new unary expressions
suffixExpression
    returns[ Expression* tree ]
    @init 
    {
        $tree = nullptr;

        Symbol::Location location;
        std::vector<Expression*> args;
        Expression* arrayIndexExpr = nullptr;

        std::vector<uint32_t> ops;
        std::vector<Symbol::Location> locations;
    }:
    e1 = binaryexpatom (op = ('++' | '--') { ops.push_back(StringToFourCC($op.text)); locations.push_back(SetupFile()); } )* 
    {
        $tree = $e1.tree;
        $tree->location = SetupFile();
        for (size_t i = 0; i < ops.size(); i++)
        {
            $tree = new UnaryExpression(ops[i], false, $tree);
            $tree->location = locations[i];
        }
    }
    | e1 = binaryexpatom
    {
        $tree = $e1.tree;
    }
    (
        '(' { location = SetupFile(); } (arg0 = logicalOrExpression { args.push_back($arg0.tree); } (',' argn = logicalOrExpression { args.push_back($argn.tree); })* )? ')'
        {
            CallExpression* expr = new CallExpression($tree, args);
            expr->location = location;
            $tree = expr;
        }
        | '.' { location = SetupFile(); } e2 = suffixExpression
        {
            AccessExpression* expr = new AccessExpression($tree, $e2.tree, false);
            expr->location = location;
            $tree = expr;
        }
        | '->' { location = SetupFile(); } e2 = suffixExpression
        {
            AccessExpression* expr = new AccessExpression($tree, $e2.tree, true);
            expr->location = location;
            $tree = expr;
        }
        | '[' { location = SetupFile(); } (e3 = expression { arrayIndexExpr = $e3.tree; })? ']'
        {
            ArrayIndexExpression* expr = new ArrayIndexExpression($tree, arrayIndexExpr);
            expr->location = location;
            $tree = expr;
        }
    )*
    ;

namespaceExpression
    returns[ Expression* tree ]
    @init
    {
        $tree = nullptr;
    }:
    e1 = binaryexpatom '::' e2 = binaryexpatom
    {

    }
    ;


// end of binary expansion, in the end, every expression can be expressed as either an ID or a new expression surrounded by paranthesis.
binaryexpatom
    returns[ Expression* tree ]
    @init 
    {
        $tree = nullptr;
    }:
    INTEGERLITERAL          { $tree = new IntExpression(atoi($INTEGERLITERAL.text.c_str())); $tree->location = SetupFile(); }
    | UINTEGERLITERAL       { $tree = new UIntExpression(strtoul($UINTEGERLITERAL.text.c_str(), nullptr, 10)); $tree->location = SetupFile(); }
    | FLOATLITERAL          { $tree = new FloatExpression(atof($FLOATLITERAL.text.c_str())); $tree->location = SetupFile(); }
    | DOUBLELITERAL         { $tree = new FloatExpression(atof($DOUBLELITERAL.text.c_str())); $tree->location = SetupFile(); }
    | HEX                   { $tree = new UIntExpression(strtoul($HEX.text.c_str(), nullptr, 16)); $tree->location = SetupFile(); }
    | string                { $tree = new StringExpression($string.val); $tree->location = SetupFile(); }
    | IDENTIFIER            { $tree = new SymbolExpression($IDENTIFIER.text); $tree->location = SetupFile(); }
    | boolean               { $tree = new BoolExpression($boolean.val); $tree->location = SetupFile(); }
    | initializerExpression { $tree = $initializerExpression.tree; }
    | '(' expression ')'    { $tree = $expression.tree; }
    ;

initializerExpression
    returns[ Expression* tree ]
    @init
    {
        $tree = nullptr;
        std::vector<Expression*> exprs;
        Symbol::Location location;
    }:
    '{' { location = SetupFile(); } ( arg0 = assignmentExpression { exprs.push_back($arg0.tree); } (',' argN = assignmentExpression { exprs.push_back($argN.tree); })* )? '}'
    {
        $tree = new InitializerExpression(exprs);
        $tree->location = location;
    }
    ;

SC: ';';
CO: ',';
COL: ':';
LP: '(';
RP: ')';
LB: '{';
RB: '}';
LL: '[';
RR: ']';
DOT: '.';
NOT: '!';
EQ: '=';
QO: '"';
QU: '?';
AND: '&';
ANDSET: '&=';
OR: '|';
ORSET: '|=';
XOR: '^';
XORSET: '^=';
CONNJUGATE: '~';
Q: '\'';
NU: '#';
FORWARDSLASH: '\\';
LESS: '<';
LESSEQ: '<=';
GREATER: '>';
GREATEREQ: '>=';
LOGICEQ: '==';
NOTEQ: '!=';
LOGICAND: '&&';
LOGICOR: '||';
MOD: '%';
UNDERSC: '_';
SOBAKA: '@';

ADD_OP: '+';
SUB_OP: '-';
DIV_OP: '/';
MUL_OP: '*';

ARROW: '->';

fragment INTEGER: ('0' .. '9');

INTEGERLITERAL: INTEGER+;
UINTEGERLITERAL: INTEGER+ ('u' | 'U');

// single line comment begins with // and ends with new line
COMMENT: ('//' .*? '\n') -> channel(HIDDEN);

// multi line comment begins with /* and ends with */
ML_COMMENT: '/*' .*? '*/' -> channel(HIDDEN);

FLOATLITERAL:
    INTEGER+ DOT INTEGER* EXPONENT? 'f'
    | DOT INTEGER* EXPONENT? 'f'
    | INTEGER+ EXPONENT? 'f';

EXPONENT: ('e' | 'E') ('+' | '-')? INTEGER+;

DOUBLELITERAL:
    INTEGER+ DOT INTEGER* EXPONENT?
    | DOT INTEGER EXPONENT?
    | INTEGER+ EXPONENT;

HEX: '0' 'x' ('0' ..'9' | 'a' ..'f' | 'A' .. 'F')* ('u' | 'U')?;

// Any alphabetical character, both lower and upper case
fragment ALPHABET: ('A' ..'Z' | 'a' ..'z');

// Identifier, must begin with alphabetical token, but can be followed by integer literal or underscore
IDENTIFIER: ('_')* ALPHABET (ALPHABET | INTEGERLITERAL | '_')*;

WS: ( '\t' | ' ' | '\r' | '\n' | '\u000C')+ -> channel(HIDDEN);
