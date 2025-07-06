//%define api.prefix {lisp}

%{

#include <math.h>
#include <stdio.h>

#include "ast.h"

cscript_ast_t ast;

extern char yytext[];
extern int col;
extern int yylineno;

extern int yylex(void);
void yyerror( const char* );

%}

%code requires {
	#include "ast.h"
}

%require "3.2"
%language "c"

//%parse-param { cscript_ast_t **out_ast }

%token <strval> IDENT
%token <intval> INTVAL
%token <floatval> FLOATVAL 
%token <charval> CHARVAL
%token <strval> STRINGVAL
%token INT CHAR FUNCTION
%token LTEQ GTEQ EQ NE AND OR

%start program

%left '-' '+'
%left '*' '/' '%'
%left '<' '>' LTEQ GTEQ
%left EQ NE
%left AND OR

/* for dangling else */
//%expect 1


%union {
	char* strval;
	int intval;
	float floatval;
	char charval;
	cscript_ast_t prog;
	stmt_t stmt;
	decllist_t dlist;
	funclist_t flist;
	main_t main;
	juxt_t juxt;
	form_t form;
	atom_t atom;
	expr_t expr;
}

%nterm <prog>        program
%nterm <dlist>    	 decl_list
%nterm <flist>    	 func_list
%nterm <main>        main_func

%nterm <expr>		 expr
%nterm <stmt>		 stmt
%nterm <juxt>		 juxt
%nterm <form>		 form
%nterm <atom>		 atom

%%

program
	: decl_list func_list main_func
	{
		$$ = CreateProg( $1, $2, $3 );
		ast = $$;
	}
;


func_list
	: /* empty */ { $$ = NULL; }
;

decl_list
	: /* empty */ 			{ $$ = NULL; }
;

main_func
	: expr 
	{
		$$ = CreateMain( NULL, NULL, $1 );
	}
;

/* Executes code */
/*stmt
	: form               	 { $$ = $1; }
;*/

/* Evaluates, to a variable or constant */
expr
	: form					 { $$ = CreateExprConst( $1 ); }
;

form
	: form '+' form          { $$ = CreateFormFromOp( $1, TPLUS, $3 ); }
	| form '-' form       	 { $$ = CreateFormFromOp( $1, TMINUS, $3 ); }
	| juxt                   { $$ = CreateFormFromJuxt( $1 ); }

;

juxt
	: atom               	 { $$ = CreateJuxt( $1 ); }
;

atom
	: INTVAL             	 { $$ = CreateIntAtom( $1 ); }
	| FLOATVAL           	 { $$ = CreateFloatAtom( $1 ); }
;
%%

void yyerror( const char *str ) {
	fflush( stderr );
  	fprintf( stderr, "CScript parsing error: \n%*s\n%*s:%d:%d\n", col, "^", col, str, yylineno, col );
}
