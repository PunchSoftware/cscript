//%define api.prefix {lisp}

%{
#include <math.h>
#include <stdio.h>

extern int yylex(void);
void yyerror( const char* );
%}

%require "3.2"
%language "c"
%defines "src/cscript_parser.h"

//%parse-param { cscript_ast_t **out_ast }

%union {
		int intval;
		float floatval;
		double val; // Used for debug printing
}

%token <intval> INTVAL 
%token <floatval> FLOATVAL 
%token IDENT
%token PLUS
%token MINUS
%token STAR 
%token SLASH
%token CARET
%token LPAREN
%token RPAREN

%type <val> form stmt juxt atom

%start expr 

%nonassoc RPAREN
%left PLUS MINUS
%left STAR SLASH 
%left CARET 

%%

expr:     line
;

line:     /* empty */
        | stmt ';'           { printf("Parsed expression result: %.10g\n", $1); }
;

stmt:     
		form               	 { $$ = $1; }

;

form
		: form PLUS form       { $$ = $1 + $3; }
		| form MINUS form      { $$ = $1 - $3; }
		| juxt               { $$ = $1; }

;

juxt
		: MINUS atom           { $$ = 0 - $2; }
		| atom               { $$ = $1; }
;

atom
		: LPAREN stmt RPAREN   { $$ = $2; }
		| INTVAL               { $$ = $1; }
		| FLOATVAL             { $$ = $1; }
;
%%

void yyerror( const char *str ) {
		fprintf( stderr, "CScript parsing error: %s\n", str );
}
