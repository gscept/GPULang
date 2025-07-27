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
friend bool GPULangValidate(GPULangFile*, GPULang::Compiler::Language, const std::vector<std::string>&, GPULang::Compiler::Options, GPULangServerResult&);

friend bool GPULangPreprocess(GPULangFile*, const std::vector<std::string>&, std::string&, GPULang::PinnedArray<GPULang::Symbol*>&, GPULang::PinnedArray<GPULang::Diagnostic>&);
friend GPULangFile* GPULangLoadFile(const std::string_view&, const std::vector<std::string_view>&);
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
        | generate ';'              { $eff->symbols.Append($generate.sym); }
        | alias ';'                 { $eff->symbols.Append($alias.sym); }
        | functionDeclaration ';'   { $eff->symbols.Append($functionDeclaration.sym); }    
        | function                  { $eff->symbols.Append($function.sym); }    
        | variables ';'             { for (Variable* var : $variables.vars) { $eff->symbols.Append(var); } }
        | structure ';'             { $eff->symbols.Append($structure.sym); }
        | enumeration ';'           { $eff->symbols.Append($enumeration.sym); }
        | state ';'                 { $eff->symbols.Append($state.sym); }
        | sampler ';'               { $eff->symbols.Append($sampler.sym); }
        | program ';'               { $eff->symbols.Append($program.sym); }
    )*?;

    
    
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
	Symbol::Location nameLocation, typeLocation;
        FixedString name, type;
    }
    : 'alias' name = IDENTIFIER { nameLocation = SetupFile(); } 'as' type = IDENTIFIER { typeLocation = SetupFile(); name = FixedString($name.text); type = FixedString($type.text); }
    {
        $sym = Alloc<Alias>();
	$sym->nameLocation = nameLocation;
	$sym->typeLocation = typeLocation;
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
    ('@' { $annot = Alloc<Annotation>(); }  (name = IDENTIFIER { $annot->location = SetupFile(); } '(' value = expression ')' { $annot->name = $name.text; $annot->value = $value.tree; }))
    ;
    
// metarule for attribute - compiler layer data to be passed to target language compiler
attribute
    returns[ Attribute* attr ]
    @init 
    {
        $attr = nullptr;
    }:
    name = IDENTIFIER { $attr = Alloc<Attribute>(); $attr->location = SetupFile(); $attr->name = $name.text; } '(' e = expression ')' { $attr->expression = $expression.tree; } 
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
        '*' { $type.type.AddModifier(Type::FullType::Modifier::Pointer); }
        | '[' { $type.type.AddModifier(Type::FullType::Modifier::Array); } 
            ( arraySize0 = expression { $type.type.UpdateValue($arraySize0.tree); } )? 
        ']'
        | qual = IDENTIFIER { $type.type.AddQualifier(FixedString($qual.text)); }
        | linePreprocessorEntry
    )* 
    typeName = IDENTIFIER { $type.type.name = $typeName.text; $type.location = EndLocationRange(typeRange); }
    ;

generate
    returns[ Symbol* sym ]
    @init
    {
        PinnedArray<Symbol*> symbols = 0xFFFFF;
	    Symbol::Location location;
    }
    :
    'generate' { location = SetupFile(); } '<' 
    ( 
        variables ';' { for(Variable* var : $variables.vars) { symbols.Append(var); } }
        | gen_statement { symbols.Append($gen_statement.tree); }
        | alias ';' { symbols.Append($alias.sym); }
        | functionDeclaration ';'   { symbols.Append($functionDeclaration.sym); }    
        | function                  { symbols.Append($function.sym); }    
    )* '>'
    {
	$sym = Alloc<Generate>(symbols);
	$sym->location = location;
    }
    ;

gen_statement
    returns[ Statement* tree ]
    @init
    {
        $tree = nullptr;
    }:
    gen_if_statement               { $tree = $gen_if_statement.tree; }
    | gen_scope_statement          { $tree = $gen_scope_statement.tree; }
    ;

gen_scope_statement
    returns[ ScopeStatement* tree ]
    @init
    {
        $tree = nullptr;
        PinnedArray<Symbol*> contents(0xFFFFFF);
	    std::vector<Expression*> unfinished;
        Symbol::Location location;
        Symbol::Location ends;
    }:
    '<' { location = SetupFile(); }
    (
        variables ';' 		{ for(Variable* var : $variables.vars) { contents.Append(var); } }
        | gen_statement 	{ contents.Append($gen_statement.tree); }
        | alias ';' 		{ contents.Append($alias.sym); }
        | function         	{ contents.Append($function.sym); }    
        | linePreprocessorEntry
        //| expression { unfinished.push_back($expression.tree); } // This is really bullshit and won't be consumed by anything, but is needed for the parser to recognize scopes with half-finished content in it
    )* 
    '>' { ends = SetupFile(); } 
    {
        $tree = Alloc<ScopeStatement>(std::move(contents), unfinished);
        $tree->ends = ends;
        $tree->location = location;
    }
    ;

gen_if_statement
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
        ifBody = gen_statement { ifBody = $ifBody.tree; }
    
    ('else' elseBody = gen_statement { elseBody = $elseBody.tree; } )?
    {
        $tree = Alloc<IfStatement>(condition, ifBody, elseBody);
        $tree->location = location;
    }
    ;

    // Variable declaration <annotation>* <attribute>* instance0, .. instanceN : <type_modifiers> <type> 
variables
    returns[ FixedArray<Variable*> vars ]
    @init
    {
        TransientArray<Variable*> list(256);
        TransientArray<Annotation*> annotations(32);
        TransientArray<Attribute*> attributes(32);
        TransientArray<FixedString> names(256);
        TransientArray<Expression*> valueExpressions(256);
        TransientArray<Symbol::Location> locations(256);
        unsigned initCounter = 0;
        TypeDeclaration type = TypeDeclaration{ .type = Type::FullType{UNDEFINED_TYPE} };
    }:
    (linePreprocessorEntry)*
    (annotation { if (annotations.Full()) { throw IndexOutOfBoundsException("Maximum of 32 annotations reached"); } annotations.Append(std::move($annotation.annot)); })*
    (attribute { if (attributes.Full()) { throw IndexOutOfBoundsException("Maximum of 32 attributes reached"); } attributes.Append(std::move($attribute.attr)); })+
    
    varName = IDENTIFIER { names.Append(FixedString($varName.text)); valueExpressions.Append(nullptr); locations.Append(SetupFile()); } 
    (linePreprocessorEntry)?
    (',' varNameN = IDENTIFIER { if (names.Full()) { throw IndexOutOfBoundsException("Maximum of 256 variable declarations reached"); } names.Append(FixedString($varNameN.text)); valueExpressions.Append(nullptr); locations.Append(SetupFile()); } | linePreprocessorEntry)*
    
    ( ':' 
       typeDeclaration { type = $typeDeclaration.type; }
    )?
    (
        '=' valueExpr = expression { if (initCounter < names.size) { valueExpressions[initCounter++] = $valueExpr.tree; }  } 
        (',' valueExprN = expression { if (initCounter < names.size) { valueExpressions[initCounter++] = $valueExprN.tree; }; } | linePreprocessorEntry)*
    )?
    {
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
        $vars = list;
    }
    ;

structureDeclaration
    returns[ Structure* sym ]
    @init
    {
        $sym = nullptr;
        TransientArray<Annotation*> annotations(32);
        TransientArray<Attribute*> attributes(32);
    }:
    (linePreprocessorEntry)*
    (annotation { if (annotations.Full()) { throw IndexOutOfBoundsException("Maximum of 32 annotations reached"); } annotations.Append(std::move($annotation.annot)); })*
    (attribute { if (attributes.Full()) { throw IndexOutOfBoundsException("Maximum of 32 attributes reached"); } attributes.Append(std::move($attribute.attr)); })*
    'struct' 
    name = IDENTIFIER 
    { 
        $sym = Alloc<Structure>();
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
        TransientArray<Symbol*> members(1024);
        bool isArray = false;
        Expression* arraySizeExpression = nullptr;
        FixedString instanceName;
        Symbol::Location varLocation;
        Type::FullType varType;
        Symbol::Location varTypeLocation;
        Symbol::Location typeRange;
        FixedString varName;
    }:
    structureDeclaration { $sym = $structureDeclaration.sym; }
    '{' 
        (varName = IDENTIFIER { varName = FixedString($varName.text); varLocation = SetupFile(); } ':'         
            { typeRange = BeginLocationRange(); }
            ( 
                '*' { varType.AddModifier(Type::FullType::Modifier::Pointer); }
                | '[' { varType.AddModifier(Type::FullType::Modifier::Array); } 
                    ( arraySize0 = expression { varType.UpdateValue($arraySize0.tree); } )?  
                ']'
            )* 
            varTypeName = IDENTIFIER { if (members.Full()) { throw IndexOutOfBoundsException("Maximum of 1024 struct members reached"); } varType.name = $varTypeName.text; varTypeLocation = EndLocationRange(typeRange); } ';'
            {
                Variable* var = Alloc<Variable>(); 
                var->type = varType; 
                var->location = varLocation; 
                var->typeLocation = varTypeLocation;
                var->name = varName;
                var->valueExpression = nullptr;
                members.Append(var);
                
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
        TransientArray<FixedString> enumLabels(256);
        TransientArray<Expression*> enumValues(256);
        TransientArray<Symbol::Location> enumLocations(256);
        FixedString name;
        TypeDeclaration type = TypeDeclaration{ .type = Type::FullType{ConstantString("u32")} };
        Symbol::Location location;
        Symbol::Location labelLocation;
    }:
    'enum' name = IDENTIFIER { name = FixedString($name.text); location = SetupFile(); }
    (':' typeDeclaration { type = $typeDeclaration.type; })?
    '{'
        (
            label = IDENTIFIER { Expression* expr = nullptr; labelLocation = SetupFile(); } ('=' value = expression { expr = $value.tree; })?
            {
                enumLabels.Append(FixedString($label.text));
                enumValues.Append(expr);
                enumLocations.Append(labelLocation);
            }
            (linePreprocessorEntry)?
            (
                ',' label = IDENTIFIER { if (enumLabels.Full()) { throw IndexOutOfBoundsException("Maximum of 256 enum labels"); } Expression* expr = nullptr; labelLocation = SetupFile(); } ('=' value = expression { expr = $value.tree; })?
                {
                    enumLabels.Append(FixedString($label.text));
                    enumValues.Append(expr);
                    enumLocations.Append(labelLocation);
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
        TransientArray<Attribute*> attributes(32);
        FixedString name;
        Expression* valueExpression = nullptr;
        Symbol::Location location;
        TypeDeclaration type = TypeDeclaration{ .type = Type::FullType{UNDEFINED_TYPE} };
    }:
    (linePreprocessorEntry)*
    (attribute { if (attributes.Full()) { throw IndexOutOfBoundsException("Maximum of 32 attributes reached"); } attributes.Append(std::move($attribute.attr)); })*
    varName = IDENTIFIER { name = FixedString($varName.text); location = SetupFile(); } 
    ':' 
    typeDeclaration { type = $typeDeclaration.type; }
    (
        '=' valueExpr = expression { valueExpression = $valueExpr.tree; } 
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
        TransientArray<Variable*> variables(32);
        TransientArray<Attribute*> attributes(32);
        Symbol::Location location;
    }:
    (attribute { if (attributes.Full()) { throw IndexOutOfBoundsException("Maximum of 32 attributes reached"); } attributes.Append(std::move($attribute.attr)); })*
    name = IDENTIFIER { location = SetupFile(); } '(' 
        (
            arg0 = parameter { variables.Append($arg0.sym); } (linePreprocessorEntry)? (',' argn = parameter { if (variables.Full()) throw IndexOutOfBoundsException("Maximum of 32 variables reached"); variables.Append($argn.sym); } | linePreprocessorEntry)* 
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
        $sym->attributes = attributes;
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
    returns[ ProgramInstance* sym ]
    @init
    {
        $sym = nullptr;
        TransientArray<Expression*> entries(32);
        TransientArray<Annotation*> annotations(32);
    }:
    (annotation { if (annotations.Full()) { throw IndexOutOfBoundsException("Maximum of 32 annotations reached"); } annotations.Append(std::move($annotation.annot)); })*
    'program' { $sym = Alloc<ProgramInstance>(); } name = IDENTIFIER { $sym->location = SetupFile(); }
    '{'
        ( assignment = expression { if (entries.Full()) throw IndexOutOfBoundsException("Maximum of 32 entries reached"); entries.Append($assignment.tree); } ';' )*
    '}'
    { 
        $sym->name = $name.text;
        $sym->annotations = annotations;
        $sym->entries = entries;
    }
    ;

sampler
    returns[ SamplerStateInstance* sym ]
    @init
    {
        TransientArray<Attribute*> attributes(32);
        TransientArray<Annotation*> annotations(32);
        TransientArray<Expression*> entries(32);
    }:
    (
        //'inline_sampler' { $sym = Alloc<SamplerStateInstance>(); $sym->isInline = true; }
        (annotation { if (annotations.Full()) throw IndexOutOfBoundsException("Maximum of 32 annotations reached"); annotations.Append(std::move($annotation.annot)); })*
        (attribute { if (attributes.Full()) throw IndexOutOfBoundsException("Maximum of 32 attributes reached"); attributes.Append(std::move($attribute.attr)); })*
        'sampler_state' { $sym = Alloc<SamplerStateInstance>(); $sym->isImmutable = true; }
    ) name = IDENTIFIER { $sym->location = SetupFile(); }
    '{'
        (assign = expression { if (entries.Full()) throw IndexOutOfBoundsException("Maximum of 32 entries reached"); entries.Append($assign.tree); } ';' )*
    '}'
    {
        $sym->name = $name.text;
        $sym->attributes = attributes;
        $sym->annotations = annotations;
        $sym->entries = entries;
    }
    ;
        
state
    returns[ State* sym ]
    @init
    {
        TransientArray<Expression*> entries(32);
    }:
    (
        'render_state' { $sym = Alloc<RenderStateInstance>(); } 
    ) name = IDENTIFIER { $sym->location = SetupFile(); }
    '{'
        (assign = expression { if (entries.Full()) throw IndexOutOfBoundsException("Maximum of 32 entries reached"); entries.Append($assign.tree); } ';' )*
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
    expressionList
    {
        $tree = Alloc<ExpressionStatement>($expressionList.expressions);
        $tree->location = $expressionList.expressions[0]->location; 
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
        FixedArray<Variable*> declarations;
        Expression* conditionExpression = nullptr;
        Expression* loopExpression = nullptr;
        Statement* contents = nullptr;
        TransientArray<Attribute*> attributes(32);
        Symbol::Location location;
    }:
    'for' { location = SetupFile(); }
    '(' 
        (variables { declarations = $variables.vars; })? ';'
        (condition = expression { conditionExpression = $condition.tree; })? ';' 
        (loop = expression      { loopExpression = $loop.tree; })?
    ')' (attribute { if (attributes.Full()) { throw IndexOutOfBoundsException("Maximum of 32 attributes reached"); } attributes.Append(std::move($attribute.attr)); })*
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
        PinnedArray<Symbol*> contents(0xFFFFFF);
	    std::vector<Expression*> unfinished;
        Symbol::Location location;
        Symbol::Location ends;
    }:
    '{' { location = SetupFile(); }
    (
        variables ';' { for(Variable* var : $variables.vars) { contents.Append(var); } }
        | statement { contents.Append($statement.tree); }
        | alias ';' { contents.Append($alias.sym); }
        | linePreprocessorEntry
        //| expression { unfinished.push_back($expression.tree); } // This is really bullshit and won't be consumed by anything, but is needed for the parser to recognize scopes with half-finished content in it
    )* 
    '}' { ends = SetupFile(); } 
    {
        $tree = Alloc<ScopeStatement>(std::move(contents), unfinished);
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
        TransientArray<Expression*> caseExpressions(256);
        TransientArray<Statement*> caseStatements(256);
        Symbol::Location location;
        Statement* defaultStatement = nullptr;
    }:
    'switch' { location = SetupFile(); } '(' expression ')' { switchExpression = $expression.tree; }
    '{'
        (
            'case' expression ':' { if (caseExpressions.Full()) { throw IndexOutOfBoundsException("Maximum of 256 case expressions reached"); } caseExpressions.Append($expression.tree); caseStatements.Append(nullptr); }
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
        $tree = Alloc<SwitchStatement>(switchExpression, std::move(caseExpressions), std::move(caseStatements), defaultStatement);
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

expressionList
    returns [FixedArray<Expression*> expressions]
    @init
    {
        TransientArray<Expression*> list(256);
    }
    :
    (linePreprocessorEntry)?
    e1 = expression { list.Append($e1.tree); } (',' 
    e2 = expression
    {
        list.Append($e2.tree);
    }
    | linePreprocessorEntry
    )*
    {
        $expressions = std::move(list);
    }
    ;

expression
    returns[ Expression* tree ]
    @init 
    {
        $tree = nullptr;
        Symbol::Location location;
        FixedArray<Expression*> args;
    }: 
    
    e1 = expression { $tree = $e1.tree; } op = ('++' | '--') { location = SetupFile(); }
    {
        $tree = Alloc<UnaryExpression>(StringToFourCC($op.text), false, $tree);
        $tree->location = location;
    }
    |
    e1 = expression { $tree = $e1.tree; } 
    '(' { location = SetupFile(); }
        (
            list = expressionList
            { args = std::move($list.expressions); }
        )? 
    ')'
    {         
        CallExpression* expr = Alloc<CallExpression>($tree, std::move(args));
        expr->location = $e1.tree->location;
        $tree = expr;
    }
    | e1 = expression { $tree = $e1.tree; } '.' { location = SetupFile(); } e2 = expression
    {
        AccessExpression* expr = Alloc<AccessExpression>($tree, $e2.tree, false);
        expr->location = $e1.tree->location;
        $tree = expr;
    }
    | e1 = expression { $tree = $e1.tree; } '->' { location = SetupFile(); } e2 = expression
    {
        AccessExpression* expr = Alloc<AccessExpression>($tree, $e2.tree, true);
        expr->location = $e1.tree->location;
        $tree = expr;
    }
    | e1 = expression { $tree = $e1.tree; } '[' { location = SetupFile(); } ( e3 = expression )? ']'
    {
        ArrayIndexExpression* expr = Alloc<ArrayIndexExpression>($tree, $e3.tree);
        expr->location = $e1.tree->location;
        $tree = expr;
    }
    | <assoc=right> op = ('-' | '+' | '!' | '~' | '++' | '--' | '*' | '&') p = expression
    {
        $tree = Alloc<UnaryExpression>(StringToFourCC($op.text), true, $p.tree);
        $tree->location = $p.tree->location;
    }
    | e1 = expression { $tree = $e1.tree; } op = ('*' | '/' | '%') { location = SetupFile(); } e2 = expression
    {	
        BinaryExpression* expr = Alloc<BinaryExpression>(StringToFourCC($op.text), $tree, $e2.tree);
        expr->location = $e1.tree->location;
        $tree = expr;
    }
    | e1 = expression { $tree = $e1.tree; } op = ('+' | '-') { location = SetupFile(); } e2 = expression
    {
        BinaryExpression* expr = Alloc<BinaryExpression>(StringToFourCC($op.text), $tree, $e2.tree);
        expr->location = $e1.tree->location;
        $tree = expr;
    }
    | e1 = expression { $tree = $e1.tree; } op = ('<<' | '>>') { location = SetupFile(); } e2 = expression
    {
        BinaryExpression* expr = Alloc<BinaryExpression>(StringToFourCC($op.text), $tree, $e2.tree);
        expr->location = $e1.tree->location;
        $tree = expr;
    }
    | e1 = expression { $tree = $e1.tree; } op = ('<' | '>' | '<=' | '>=' ) { location = SetupFile(); } e2 = expression
    {
        BinaryExpression* expr = Alloc<BinaryExpression>(StringToFourCC($op.text), $tree, $e2.tree);
        expr->location = $e1.tree->location;
        $tree = expr;
    }
    | e1 = expression { $tree = $e1.tree; } op = ('==' | '!=')  { location = SetupFile(); } e2 = expression
    {
        BinaryExpression* expr = Alloc<BinaryExpression>(StringToFourCC($op.text), $tree, $e2.tree);
        expr->location = $e1.tree->location;
        $tree = expr;
    }
    | e1 = expression { $tree = $e1.tree; } '&' { location = SetupFile(); } e2 = expression
    {
        BinaryExpression* expr = Alloc<BinaryExpression>('&', $tree, $e2.tree);
        expr->location = $e1.tree->location;
        $tree = expr;
    }
    | e1 = expression { $tree = $e1.tree; } '^' { location = SetupFile(); } e2 = expression
    {
        BinaryExpression* expr = Alloc<BinaryExpression>('^', $tree, $e2.tree);
        expr->location = location;
        $tree = expr;
    }
    | e1 = expression { $tree = $e1.tree; } '|' { location = SetupFile(); } e2 = expression
    {
        BinaryExpression* expr = Alloc<BinaryExpression>('|', $tree, $e2.tree);
        expr->location = $e1.tree->location;
        $tree = expr;
    }
    | e1 = expression { $tree = $e1.tree; } '&&' { location = SetupFile(); } e2 = expression
    {
        BinaryExpression* expr = Alloc<BinaryExpression>('&&', $tree, $e2.tree);
        expr->location = $e1.tree->location;
        $tree = expr;
    }
    | e1 = expression { $tree = $e1.tree; } '||' { location = SetupFile(); } e2 = expression
    {
        BinaryExpression* expr = Alloc<BinaryExpression>('||', $tree, $e2.tree);
        expr->location = $e1.tree->location;
        $tree = expr;
    }
    | <assoc=right> e1 = expression '?' { location = SetupFile(); } ifBody = expression ':' elseBody = expression
    { 
        TernaryExpression* expr = Alloc<TernaryExpression>($e1.tree, $ifBody.tree, $elseBody.tree);
        expr->location = $e1.tree->location;
        $tree = expr;
    }
    | <assoc=right> e1 = expression { $tree = $e1.tree; } op = ('+=' | '-=' | '*=' | '/=' | '%=' | '<<=' | '>>=' | '&=' | '^=' | '|=' | '=') { location = SetupFile(); } e2 = expression
    {
        BinaryExpression* expr = Alloc<BinaryExpression>(StringToFourCC($op.text), $e1.tree, $e2.tree);
        expr->location = $e1.tree->location;
        $tree = expr;
    }
    |
    atom = binaryexpatom { $tree = $atom.tree; }
    ;

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
    | IDENTIFIER                    { $tree = Alloc<SymbolExpression>(FixedString($IDENTIFIER.text)); $tree->location = SetupFile(); }
    | boolean                       { $tree = Alloc<BoolExpression>($boolean.val); $tree->location = SetupFile(); }
    | 'declared' { begin = SetupFile(); } '<' ident = IDENTIFIER '>'
    {
	$tree = Alloc<DeclaredExpression>(FixedString($ident.text));
        $tree->location = begin ; 
    }
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
        FixedString type;
        Symbol::Location location;
	FixedArray<Expression*> initializers;
    }:
    type = IDENTIFIER { type = FixedString($type.text); } '{' { location = SetupFile(); } (list = expressionList { initializers = $list.expressions; })? '}'
    {
        $tree = Alloc<InitializerExpression>(std::move(initializers), type);
        $tree->location = location;
    }
    ;
  
arrayInitializerExpression
    returns[ Expression* tree ]
    @init
    {
        $tree = nullptr;
        Symbol::Location location;
    }:
    '[' { location = SetupFile(); } (list = expressionList) ']'
    {
        $tree = Alloc<ArrayInitializerExpression>($list.expressions);
        $tree->location = location;
    }
    ;
   
floatVecLiteralExpression
    returns[ Expression* tree ]
    @init
    {
        $tree = nullptr;
        TransientArray<float> values(32);
        Symbol::Location location;
    }:
    '<' { location = SetupFile(); } ( arg0 = FLOATLITERAL { values.Append(atof($arg0.text.c_str())); } ) (linePreprocessorEntry)? (',' argN = FLOATLITERAL { values.Append(atof($argN.text.c_str())); })+ '>'
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
        TransientArray<float> values(32);
        Symbol::Location location;
    }:
    '<' { location = SetupFile(); } ( arg0 = DOUBLELITERAL { values.Append(atof($arg0.text.c_str())); } ) (linePreprocessorEntry)? (',' argN = DOUBLELITERAL { values.Append(atof($argN.text.c_str())); } )+ '>'
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
        TransientArray<int> values(32);
        Symbol::Location location;
    }:
    '<' { location = SetupFile(); } ( arg0 = INTEGERLITERAL { values.Append(atof($arg0.text.c_str())); } ) (linePreprocessorEntry)? (',' argN = INTEGERLITERAL { values.Append(atof($argN.text.c_str())); } )+ '>'
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
        TransientArray<unsigned int> values(32);
        Symbol::Location location;
    }:
    '<' { location = SetupFile(); } ( arg0 = UINTEGERLITERAL { values.Append(atof($arg0.text.c_str())); } ) (linePreprocessorEntry)? (',' argN = UINTEGERLITERAL { values.Append(atof($argN.text.c_str())); } )+ '>'
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
        TransientArray<bool> values(32);
        Symbol::Location location;
    }:
    '<' { location = SetupFile(); } ( arg0 = boolean { values.Append(atof($arg0.text.c_str())); } ) (linePreprocessorEntry)? (',' argN = boolean { values.Append(atof($argN.text.c_str())); } )+ '>'
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
    INTEGER+ DOT INTEGER* EXPONENT? ('f'|'F')
    | DOT INTEGER+ EXPONENT? ('f'|'F')
    | INTEGER+ EXPONENT? ('f'|'F');

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
