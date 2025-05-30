grammar GPULang;

// Lexer API hooks
@lexer::apifuncs {

}

@lexer::members {
#include <iostream>
}

@lexer::header {
    #include <string>
    #include <vector>
    #include "gpulangtoken.h"
}

// parser API hooks
@parser::apifuncs {

}

@parser::declarations {
friend class GPULangLexerErrorHandler;
friend class GPULangParserErrorHandler;
friend class GPULangTokenFactory;
friend bool GPULangCompile(const std::string&, GPULang::Compiler::Language, const std::string&, const std::string&, const std::vector<std::string>&, GPULang::Compiler::Options, GPULangErrorBlob*&);
friend bool GPULangValidate(GPULangFile*, const std::vector<std::string>&, GPULang::Compiler::Options, GPULangServerResult&);
friend bool GPULangValidateFile(const std::string&, const std::vector<std::string>&, GPULang::Compiler::Options, GPULangServerResult&);
friend bool GPULangPreprocess(GPULangFile*, const std::string&, const std::vector<std::string>&, std::string&, std::string&);
friend GPULangFile* GPULangLoadFile(const char*, const std::vector<std::string_view>&);
static std::vector<std::tuple<size_t, size_t, std::string>> LineStack;
}

@parser::definitions {
std::vector<std::tuple<size_t, size_t, std::string>> GPULangParser::LineStack;
}

@parser::members {


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

}

string
    returns[ std::string val ]:
    '"' (data = ~'"' { $val.append($data.text); })* '"'
    | '\'' (data = ~'\'' { $val.append($data.text); })* '\''
    ;

path
    returns[ std::string val ]:
    '"' (data = ~'"' { $val.append($data.text); })* '"'
    | '<' (data = ~'>' { $val.append($data.text); })* '>'
    ;

boolean
    returns[ bool val ]
    @init {
        $val = false;
    }: 'true' { $val = true; } | 'false' { $val = false; };

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
        $eff = Alloc<Effect>();
    }
    :
    (
        linePreprocessorEntry
        //| preprocessor
        | alias ';'                 { $eff->symbols.push_back($alias.sym); }
        | functionDeclaration ';'   { $eff->symbols.push_back($functionDeclaration.sym); }    
        | function                  { $eff->symbols.push_back($function.sym); }    
        | variables ';'             { for (Variable* var : $variables.list) { $eff->symbols.push_back(var); } }
        | structure ';'             { $eff->symbols.push_back($structure.sym); }
        | enumeration ';'           { $eff->symbols.push_back($enumeration.sym); }
        | state ';'                 { $eff->symbols.push_back($state.sym); }
        | sampler ';'               { $eff->symbols.push_back($sampler.sym); }
        | program ';'               { $eff->symbols.push_back($program.sym); }
    )*?;

    
//preprocessor
//    returns [Preprocessor* pp]
//    @init
//    {
//        Symbol::Location location;
//    }
//    :
//    '#' (method = IDENTIFIER) { $pp = Alloc<Preprocessor>(); $pp->method = $method.text; $pp->type = Preprocessor::EndIf; }
//    (
//        (file = path { $pp->args.push_back($file.val); })                                                                                  { $pp->type = Preprocessor::Include; }    // include
//        | '(' (arg0 = IDENTIFIER { $pp->args.push_back($arg0.text); } (',' argn = IDENTIFIER { $pp->args.push_back($argn.text); })* )? ')'  { $pp->type = Preprocessor::Macro; }    // macro
//        | IDENTIFIER                                                                                                                        { $pp->type = Preprocessor::IfDef; }
//        | line = INTEGERLITERAL p = string
//    )
//    ;
    
    
linePreprocessorEntry
    @init
    {
        size_t origLine;
    }
    :
    '#line' { origLine = _input->LT(-1)->getLine(); } line = INTEGERLITERAL p = string { LineStack.push_back( {origLine, atoi($line.text.c_str()), $p.val }); }
    ;
    
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
        $sym = Alloc<Alias>();
        $sym->name = name;
        $sym->type = type;
    }
    ;

// metarule for annotation - API layer data to be passed to program reading shader
annotation
    returns[ Annotation* annot ]
    @init
    {
        $annot = nullptr;
    }:
        ('@' (name = IDENTIFIER '(' value = expression ')' { $annot = Alloc<Annotation>(); $annot->name = $name.text; $annot->value = $value.tree; }))
    ;
    
// metarule for attribute - compiler layer data to be passed to target language compiler
attribute
    returns[ Attribute* attr ]
    @init 
    {
        $attr = nullptr;
    }:
    name = IDENTIFIER { $attr = Alloc<Attribute>(); $attr->location = SetupFile(); $attr->name = $name.text; } '(' expression ')' { $attr->expression = $expression.tree; } 
    | name = IDENTIFIER { $attr = Alloc<Attribute>(); $attr->location = SetupFile(); $attr->name = $name.text; $attr->expression = nullptr; }
    ;

typeDeclaration
    returns[ TypeDeclaration type ]
    @init
    {
        $type.type.name = "";
        Symbol::Location typeRange;
    }:

    { typeRange = BeginLocationRange(); }
    ( 
        '*' { $type.type.AddModifier(Type::FullType::Modifier::Pointer); } |
        '[' { $type.type.AddModifier(Type::FullType::Modifier::Array); } 
            ( arraySize0 = expression { $type.type.UpdateValue($arraySize0.tree); } )? 
        ']'
        | IDENTIFIER { $type.type.AddQualifier(FixedString($IDENTIFIER.text)); }
        | linePreprocessorEntry
    )* 
    typeName = IDENTIFIER { $type.type.name = $typeName.text; $type.location = EndLocationRange(typeRange); }
    ;

    // Variable declaration <annotation>* <attribute>* instance0, .. instanceN : <type_modifiers> <type> 
variables
    returns[ std::vector<Variable*> list ]
    @init
    {
        std::vector<Annotation*> annotations;
        std::vector<Attribute*> attributes;
        std::vector<std::string> names;
        std::vector<Expression*> valueExpressions;
        std::vector<Symbol::Location> locations;
        unsigned initCounter = 0;
        TypeDeclaration type = TypeDeclaration{ .type = Type::FullType{UNDEFINED_TYPE} };
    }:
    (linePreprocessorEntry)*
    (annotation { annotations.push_back(std::move($annotation.annot)); })*
    (attribute { attributes.push_back(std::move($attribute.attr)); })+
    
    varName = IDENTIFIER { names.push_back($varName.text); valueExpressions.push_back(nullptr); locations.push_back(SetupFile()); } 
    (linePreprocessorEntry)?
    (',' varNameN = IDENTIFIER { names.push_back($varNameN.text); valueExpressions.push_back(nullptr); locations.push_back(SetupFile()); } | linePreprocessorEntry)*
    
    ( ':' 
       typeDeclaration { type = $typeDeclaration.type; }
    )?
    (
        '=' valueExpr = logicalOrExpression { if (initCounter < names.size()) { valueExpressions[initCounter++] = $valueExpr.tree; }  } 
        (',' valueExprN = logicalOrExpression { if (initCounter < names.size()) { valueExpressions[initCounter++] = $valueExprN.tree; }; } | linePreprocessorEntry)*
    )?
    {
        for (size_t i = 0; i < names.size(); i++)
        {
            Variable* var = Alloc<Variable>(); 
            var->type = type.type; 
            var->typeLocation = type.location;
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
        std::vector<Annotation*> annotations;
        std::vector<Attribute*> attributes;
    }:
    (linePreprocessorEntry)*
    (annotation { annotations.push_back(std::move($annotation.annot)); })*
    (attribute { attributes.push_back(std::move($attribute.attr)); })*
    'struct' 
    name = IDENTIFIER 
    { 
        $sym = Alloc<Structure>();
        $sym->name = $name.text; 
        $sym->annotations = std::move(annotations);
        $sym->attributes = std::move(attributes);
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
        Symbol::Location varTypeLocation;
        Symbol::Location typeRange;
        std::string varName;
    }:
    structureDeclaration { $sym = $structureDeclaration.sym; }
    '{' 
        (varName = IDENTIFIER { varName = $varName.text; varLocation = SetupFile(); } ':'         
            { typeRange = BeginLocationRange(); }
            ( 
                '*' { varType.AddModifier(Type::FullType::Modifier::Pointer); } |
                '[' { varType.AddModifier(Type::FullType::Modifier::Array); } 
                    ( arraySize0 = expression { varType.UpdateValue($arraySize0.tree); } )?  
                ']'
            )* 
            varTypeName = IDENTIFIER { varType.name = $varTypeName.text; varTypeLocation = EndLocationRange(typeRange); } ';'
            {
                Variable* var = Alloc<Variable>(); 
                var->type = varType; 
                var->location = varLocation; 
                var->typeLocation = varTypeLocation;
                var->name = varName;
                var->valueExpression = nullptr;
                members.push_back(var);
                
                varType = Type::FullType();
            }
            | linePreprocessorEntry
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
        std::vector<Symbol::Location> enumLocations;
        std::string name;
        TypeDeclaration type = TypeDeclaration{ .type = Type::FullType{"u32"} };
        Symbol::Location location;
        Symbol::Location labelLocation;
    }:
    'enum' name = IDENTIFIER { name = $name.text; location = SetupFile(); }
    (':' typeDeclaration { type = $typeDeclaration.type; })?
    '{'
        (
            label = IDENTIFIER { Expression* expr = nullptr; labelLocation = SetupFile(); } ('=' value = expression { expr = $value.tree; })?
            {
                enumLabels.push_back($label.text);
                enumValues.push_back(expr);
                enumLocations.push_back(labelLocation);
            }
            (linePreprocessorEntry)?
            (
                ',' label = IDENTIFIER { Expression* expr = nullptr; labelLocation = SetupFile(); } ('=' value = expression { expr = $value.tree; })?
                {
                    enumLabels.push_back($label.text);
                    enumValues.push_back(expr);
                    enumLocations.push_back(labelLocation);
                }
                |
                linePreprocessorEntry
            )*
            |
            linePreprocessorEntry
        )?
    '}'
    {
        $sym = Alloc<Enumeration>();
        $sym->name = name;
        $sym->type = type.type;
        $sym->type.literal = true;
        $sym->labels = enumLabels;
        $sym->values = enumValues;
        $sym->labelLocations = enumLocations;
        $sym->location = location;
    }
    ;

// Parameter declaration <attribute>* instance0, .. instanceN : <type_modifiers> <type> 
parameter
    returns[ Variable* sym ]
    @init
    {
        std::vector<Attribute*> attributes;
        std::string name;
        Expression* valueExpression = nullptr;
        Symbol::Location location;
        TypeDeclaration type = TypeDeclaration{ .type = Type::FullType{UNDEFINED_TYPE} };
    }:
    (linePreprocessorEntry)*
    (attribute { attributes.push_back(std::move($attribute.attr)); })*
    varName = IDENTIFIER { name = $varName.text; location = SetupFile(); } 
    ':' 
    typeDeclaration { type = $typeDeclaration.type; }
    (
        '=' valueExpr = logicalOrExpression { valueExpression = $valueExpr.tree; } 
    )?
    {
            $sym = Alloc<Variable>(); 
            $sym->type = type.type; 
            $sym->typeLocation = type.location;
            $sym->location = location; 
            $sym->attributes = std::move(attributes);
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
        std::vector<Attribute*> attributes;
        Symbol::Location location;
    }:
    (attribute { attributes.push_back(std::move($attribute.attr)); })*
    name = IDENTIFIER { location = SetupFile(); } '(' 
        (
            arg0 = parameter { variables.push_back($arg0.sym); } (linePreprocessorEntry)? (',' argn = parameter { variables.push_back($argn.sym); } | linePreprocessorEntry)* 
        )? 
        ')' returnType = typeDeclaration
    {
        $sym = Alloc<Function>(); 
        $sym->hasBody = false;
        $sym->location = location;
        $sym->returnType = $returnType.type.type; 
        $sym->returnTypeLocation = $returnType.type.location;
        $sym->name = $name.text; 
        $sym->parameters = variables; 
        $sym->attributes = std::move(attributes);
    }
    ;

function
    returns[ Function* sym ]
    @init
    {
        $sym = nullptr;
    }:
    functionDeclaration { $sym = $functionDeclaration.sym; }
    scopeStatement
    {
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
        std::vector<Annotation*> annotations;
    }:
    (annotation { annotations.push_back(std::move($annotation.annot)); })*
    'program' name = IDENTIFIER { location = SetupFile(); }
    '{'
        ( assignment = expression { entries.push_back($assignment.tree); } ';' )*
    '}'
    { 
        $sym = Alloc<Program>();
        $sym->location = location;
        $sym->name = $name.text;
        $sym->annotations = std::move(annotations);
        $sym->entries = entries;
    }
    ;

sampler
    returns[ SamplerState* sym ]
    @init
    {
        std::vector<Attribute*> attributes;
        std::vector<Annotation*> annotations;
        std::vector<Expression*> entries;
    }:
    (
        //'inline_sampler' { $sym = Alloc<SamplerState>(); $sym->isInline = true; }
        (annotation { annotations.push_back(std::move($annotation.annot)); })*
        (attribute { attributes.push_back(std::move($attribute.attr)); })*
        'sampler_state' { $sym = Alloc<SamplerState>(); $sym->isImmutable = true; }
    ) name = IDENTIFIER { $sym->location = SetupFile(); }
    '{'
        (assign = expression { entries.push_back($assign.tree); } ';' )*
    '}'
    {
        $sym->name = $name.text;
        $sym->attributes = std::move(attributes);
        $sym->annotations = std::move(annotations);
        $sym->entries = entries;
    }
    ;
        
state
    returns[ State* sym ]
    @init
    {
        std::vector<Expression*> entries;
    }:
    (
        'render_state' { $sym = Alloc<RenderState>(); } 
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
    | terminateStatement        { $tree = $terminateStatement.tree; }
    | continueStatement         { $tree = $continueStatement.tree; }
    | breakStatement            { $tree = $breakStatement.tree; }
    | expressionStatement ';'   { $tree = $expressionStatement.tree; }
    //| ';'                       // empty statement
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
        $tree = Alloc<ExpressionStatement>($expression.tree);
        $tree->location = $expression.tree->location;
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
        $tree = Alloc<IfStatement>(condition, ifBody, elseBody);
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
        std::vector<Attribute*> attributes;
        Symbol::Location location;
    }:
    'for' { location = SetupFile(); }
    '(' 
        (variables { declarations = $variables.list; })? ';'
        (condition = expression { conditionExpression = $condition.tree; })? ';' 
        (loop = expression      { loopExpression = $loop.tree; })?
    ')' (attribute { attributes.push_back(std::move($attribute.attr)); })*
    content = statement { contents = $content.tree; }
    {
        $tree = Alloc<ForStatement>(declarations, conditionExpression, loopExpression, contents);
        $tree->location = location;
        $tree->attributes = std::move(attributes);
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
    
forUniformValueStatement
    returns [ Statement* tree ]
    @init
    {
        $tree = nullptr;
        Statement* contents = nullptr;
        Symbol::Location location;
    }: 'for_uniform' { location = SetupFile(); } '(' expression ')'
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
        $tree = Alloc<WhileStatement>(conditionExpression, contents, isDoWhile);
        $tree->location = location;
    }
    | 'do' { location = SetupFile(); }
    content = statement { contents = $content.tree; isDoWhile = true; } 
    'while' '(' condition = expression { conditionExpression = $condition.tree; } ')' ';'
    {
        $tree = Alloc<WhileStatement>(conditionExpression, contents, isDoWhile);
        $tree->location = location;
    }
    ;

scopeStatement
    returns[ ScopeStatement* tree ]
    @init
    {
        $tree = nullptr;
        std::vector<Symbol*> contents;
	std::vector<Expression*> unfinished;
        Symbol::Location location;
        Symbol::Location ends;
    }:
    '{' { location = SetupFile(); }
    (
        variables ';' { for(Variable* var : $variables.list) { contents.push_back(var); } }
        | statement { contents.push_back($statement.tree); }
        | linePreprocessorEntry
        //| expression { unfinished.push_back($expression.tree); } // This is really bullshit and won't be consumed by anything, but is needed for the parser to recognize scopes with half-finished content in it
    )* 
    '}' { ends = SetupFile(); } 
    {
        $tree = Alloc<ScopeStatement>(contents, unfinished);
        $tree->ends = ends;
        $tree->location = location;
    }
    ;
    
terminateStatement
    returns [ Statement* tree ]
    @init
    {
        $tree = nullptr;
        Expression* returnValue = nullptr;
        Symbol::Location location;
    }:
    'return' { location = SetupFile(); } (value = expression { returnValue = $value.tree; })? ';'
    {
        $tree = Alloc<TerminateStatement>(returnValue, TerminateStatement::TerminationType::Return);
        $tree->location = location;
    }
    | 'discard' { location = SetupFile(); } ';'
    {
      $tree = Alloc<TerminateStatement>(returnValue, TerminateStatement::TerminationType::Discard);
      $tree->location = location;
    }
    | 'ray_ignore' { location = SetupFile(); } ';'
    {
      $tree = Alloc<TerminateStatement>(returnValue, TerminateStatement::TerminationType::RayIgnoreIntersection);
      $tree->location = location;
    }
    | 'ray_terminate' { location = SetupFile(); } ';'
    {
      $tree = Alloc<TerminateStatement>(returnValue, TerminateStatement::TerminationType::RayTerminate);
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
        $tree = Alloc<ContinueStatement>();
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
            'case' expression ':' { caseExpressions.push_back($expression.tree); caseStatements.push_back(nullptr); }
            (
                statement
                { 
                    caseStatements.back() = $statement.tree;
                }
            )?
        )*
        (
            'default' ':'
            (
                statement
                {
                    defaultStatement = $statement.tree;
                }
            )?
        )?
    '}'
    {
        $tree = Alloc<SwitchStatement>(switchExpression, caseExpressions, caseStatements, defaultStatement);
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
        $tree = Alloc<BreakStatement>();
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
    commaExpression { $tree = $commaExpression.tree; }
    ;

// Series of expressions separated by comma
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
            CommaExpression* expr = Alloc<CommaExpression>($tree, $e2.tree);
            expr->location = location;
            $tree = expr;
        }
    )*
    ;

// assignment or ternary
assignmentExpression
    returns[ Expression* tree ]
    @init 
    {
        $tree = nullptr;
        Symbol::Location location;
    }:
    e1 = logicalOrExpression { $tree = $e1.tree; } 
    (
        op = ('+=' | '-=' | '*=' | '/=' | '%=' | '<<=' | '>>=' | '&=' | '^=' | '|=' | '=') { location = SetupFile(); } e2 = logicalOrExpression
        { 
            BinaryExpression* expr = Alloc<BinaryExpression>(StringToFourCC($op.text), $tree, $e2.tree);
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
        '?' ifBody = logicalOrExpression ':' elseBody = logicalOrExpression
        { 
            TernaryExpression* expr = Alloc<TernaryExpression>($tree, $ifBody.tree, $elseBody.tree);
            expr->location = $e1.tree->location;
            $tree = expr;
        }
        |
        ('||') e2 = logicalAndExpression
        {
            BinaryExpression* expr = Alloc<BinaryExpression>('||', $tree, $e2.tree);
            expr->location = $e1.tree->location;
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
            BinaryExpression* expr = Alloc<BinaryExpression>('&&', $tree, $e2.tree);
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
            BinaryExpression* expr = Alloc<BinaryExpression>('|', $tree, $e2.tree);
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
            BinaryExpression* expr = Alloc<BinaryExpression>('^', $tree, $e2.tree);
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
            BinaryExpression* expr = Alloc<BinaryExpression>('&', $tree, $e2.tree);
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
            BinaryExpression* expr = Alloc<BinaryExpression>(StringToFourCC($op.text), $tree, $e2.tree);
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
            BinaryExpression* expr = Alloc<BinaryExpression>(StringToFourCC($op.text), $tree, $e2.tree);
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
            BinaryExpression* expr = Alloc<BinaryExpression>(StringToFourCC($op.text), $tree, $e2.tree);
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
            BinaryExpression* expr = Alloc<BinaryExpression>(StringToFourCC($op.text), $tree, $e2.tree);
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
            BinaryExpression* expr = Alloc<BinaryExpression>(StringToFourCC($op.text), $tree, $e2.tree);
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
        if ($tree != nullptr)
        {
            for (size_t i = 0; i < ops.size(); i++)
            {
                $tree = Alloc<UnaryExpression>(ops[i], true, $tree);
                $tree->location = locations[i];
            }
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
    e1 = binaryexpatom
    {
        $tree = $e1.tree;
        $tree->location = SetupFile();
    }
    (
        '(' 
            (
                arg0 = logicalOrExpression { args.push_back($arg0.tree); } (linePreprocessorEntry)? (',' argn = logicalOrExpression { args.push_back($argn.tree); } | linePreprocessorEntry)* 
            )? 
        ')'
        {
            CallExpression* expr = Alloc<CallExpression>($tree, args);
            expr->location = $e1.tree->location;
            $tree = expr;
        }
        | '.' e2 = suffixExpression
        {
            AccessExpression* expr = Alloc<AccessExpression>($tree, $e2.tree, false);
            expr->location = $tree->location;
            $tree = expr;
        }
        | '->' e2 = suffixExpression
        {
            AccessExpression* expr = Alloc<AccessExpression>($tree, $e2.tree, true);
            expr->location = $tree->location;
            $tree = expr;
        }
        | '[' (e3 = expression { arrayIndexExpr = $e3.tree; })? ']'
        {
            ArrayIndexExpression* expr = Alloc<ArrayIndexExpression>($tree, arrayIndexExpr);
            expr->location = $tree->location;
            $tree = expr;
        }
    )*
    | e1 = binaryexpatom (op = ('++' | '--') { ops.push_back(StringToFourCC($op.text)); locations.push_back(SetupFile()); } )* 
    {
        $tree = $e1.tree;
        $tree->location = SetupFile();
        for (size_t i = 0; i < ops.size(); i++)
        {
            $tree = Alloc<UnaryExpression>(ops[i], false, $tree);
            $tree->location = locations[i];
        }
    }
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
        Symbol::Location begin = BeginLocationRange();
    }:
    initializerExpression           { $tree = $initializerExpression.tree; }
    | arrayInitializerExpression    { $tree = $arrayInitializerExpression.tree; } 
    | '(' expression ')'            { $tree = $expression.tree; }
    | INTEGERLITERAL                { $tree = Alloc<IntExpression>(atoi($INTEGERLITERAL.text.c_str())); $tree->location = SetupFile(); }
    | UINTEGERLITERAL               { $tree = Alloc<UIntExpression>(strtoul($UINTEGERLITERAL.text.c_str(), nullptr, 10)); $tree->location = SetupFile(); }
    | FLOATLITERAL                  { $tree = Alloc<FloatExpression>(atof($FLOATLITERAL.text.c_str())); $tree->location = SetupFile(); }
    | DOUBLELITERAL                 { $tree = Alloc<FloatExpression>(atof($DOUBLELITERAL.text.c_str())); $tree->location = SetupFile(); }
    | HEX                           { $tree = Alloc<UIntExpression>(strtoul($HEX.text.c_str(), nullptr, 16)); $tree->location = SetupFile(); }
    | string                        { $tree = Alloc<StringExpression>($string.val); $tree->location = EndLocationRange(begin); }
    | IDENTIFIER                    { $tree = Alloc<SymbolExpression>($IDENTIFIER.text); $tree->location = SetupFile(); }
    | boolean                       { $tree = Alloc<BoolExpression>($boolean.val); $tree->location = SetupFile(); }
    //| floatVecLiteralExpression     { $tree = $floatVecLiteralExpression.tree; }
    //| doubleVecLiteralExpression    { $tree = $doubleVecLiteralExpression.tree; }
    //| intVecLiteralExpression       { $tree = $intVecLiteralExpression.tree; }
    //| uintVecLiteralExpression      { $tree = $uintVecLiteralExpression.tree; }
    //| booleanVecLiteralExpression   { $tree = $booleanVecLiteralExpression.tree; }    
    | linePreprocessorEntry
    ;

initializerExpression
    returns[ Expression* tree ]
    @init
    {
        $tree = nullptr;
        std::vector<Expression*> exprs;
        std::string type = "";
        Symbol::Location location;
    }:
    type = IDENTIFIER { type = $type.text; } '{' { location = SetupFile(); } ( arg0 = logicalOrExpression { if ($arg0.tree != nullptr) exprs.push_back($arg0.tree); } (linePreprocessorEntry)? (',' argN = logicalOrExpression { if ($argN.tree != nullptr) exprs.push_back($argN.tree); } | linePreprocessorEntry)* )? '}'
    {
        $tree = Alloc<InitializerExpression>(exprs, type);
        $tree->location = location;
    }
    ;
  
arrayInitializerExpression
    returns[ Expression* tree ]
    @init
    {
        $tree = nullptr;
        std::vector<Expression*> exprs;
        Symbol::Location location;
    }:
    '[' { location = SetupFile(); } ( arg0 = logicalOrExpression { if ($arg0.tree != nullptr) exprs.push_back($arg0.tree); } (linePreprocessorEntry)? (',' argN = logicalOrExpression { if ($argN.tree != nullptr) exprs.push_back($argN.tree); } | linePreprocessorEntry)* )? ']'
    {
        $tree = Alloc<ArrayInitializerExpression>(exprs);
        $tree->location = location;
    }
    ;
   
floatVecLiteralExpression
    returns[ Expression* tree ]
    @init
    {
        $tree = nullptr;
        std::vector<float> values;
        Symbol::Location location;
    }:
    '<' { location = SetupFile(); } ( arg0 = FLOATLITERAL { values.push_back(atof($arg0.text.c_str())); } ) (linePreprocessorEntry)? (',' argN = FLOATLITERAL { values.push_back(atof($argN.text.c_str())); })+ '>'
    {
        $tree = Alloc<FloatVecExpression>(values);
        $tree->location = location;
    }
    ;
    
doubleVecLiteralExpression
    returns[ Expression* tree ]
    @init
    {
        $tree = nullptr;
        std::vector<float> values;
        Symbol::Location location;
    }:
    '<' { location = SetupFile(); } ( arg0 = DOUBLELITERAL { values.push_back(atof($arg0.text.c_str())); } ) (linePreprocessorEntry)? (',' argN = DOUBLELITERAL { values.push_back(atof($argN.text.c_str())); } )+ '>'
    {
        $tree = Alloc<FloatVecExpression>(values);
        $tree->location = location;
    }
    ;
    
intVecLiteralExpression
    returns[ Expression* tree ]
    @init
    {
        $tree = nullptr;
        std::vector<int> values;
        Symbol::Location location;
    }:
    '<' { location = SetupFile(); } ( arg0 = INTEGERLITERAL { values.push_back(atof($arg0.text.c_str())); } ) (linePreprocessorEntry)? (',' argN = INTEGERLITERAL { values.push_back(atof($argN.text.c_str())); } )+ '>'
    {
        $tree = Alloc<IntVecExpression>(values);
        $tree->location = location;
    }
    ;
    
uintVecLiteralExpression
    returns[ Expression* tree ]
    @init
    {
        $tree = nullptr;
        std::vector<unsigned int> values;
        Symbol::Location location;
    }:
    '<' { location = SetupFile(); } ( arg0 = UINTEGERLITERAL { values.push_back(atof($arg0.text.c_str())); } ) (linePreprocessorEntry)? (',' argN = UINTEGERLITERAL { values.push_back(atof($argN.text.c_str())); } )+ '>'
    {
        $tree = Alloc<UIntVecExpression>(values);
        $tree->location = location;
    }
    ;
    
booleanVecLiteralExpression
    returns[ Expression* tree ]
    @init
    {
        $tree = nullptr;
        std::vector<bool> values;
        Symbol::Location location;
    }:
    '<' { location = SetupFile(); } ( arg0 = boolean { values.push_back(atof($arg0.text.c_str())); } ) (linePreprocessorEntry)? (',' argN = boolean { values.push_back(atof($argN.text.c_str())); } )+ '>'
    {
        $tree = Alloc<BoolVecExpression>(values);
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
CONJUGATE: '~';
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

fragment EXPONENT: ('e' | 'E') ('+' | '-')? INTEGER+;

FLOATLITERAL:
    INTEGER+ DOT INTEGER* EXPONENT? 'f'
    | DOT INTEGER+ EXPONENT? 'f'
    | INTEGER+ EXPONENT? 'f';

DOUBLELITERAL:
    INTEGER+ DOT INTEGER* EXPONENT?
    | DOT INTEGER+ EXPONENT?
    | INTEGER+ EXPONENT;
    

HEX: '0' 'x' ('0' ..'9' | 'a' ..'f' | 'A' .. 'F')* ('u' | 'U')?;

// Any alphabetical character, both lower and upper case
fragment ALPHABET: ('A'..'Z' | 'a'..'z');

// Identifier, must begin with alphabetical token, but can be followed by integer literal or underscore
IDENTIFIER: ('_')* ALPHABET (ALPHABET | INTEGERLITERAL | '_')*;

WS: ( '\t' | ' ' | '\r' | '\n' | '\u000C')+ -> channel(HIDDEN);
