//%define api.prefix {lisp}

%{

#include <math.h>
#include <stdio.h>

#include "ast/ast.h"

prog_t ast;

extern char yytext[];
extern int col;
extern int yylineno;

extern int yylex(void);
void yyerror( const char* );

%}

%code requires {
	#include "ast/ast.h"
}

%require "3.2"
%language "c"

//%parse-param { prog_t **out_ast }

%token <strval> IDENT
%token <intval> INTVAL
%token <floatval> FLOATVAL 
%token <charval> CHARVAL
%token <strval> STRINGVAL
%token INT FLOAT CHAR FUNCTION
%token LTEQ GTEQ EQ NE AND OR
%token IF ELSE

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
	char* ident;

	prog_t prog;
	stmt_t stmt;
	declList_t dlist;
	funcList_t flist;
	stmtList_t slist;
	main_t main;
	juxt_t juxt;
	form_t form;
	atom_t atom;
	expr_t expr;

	decl_t decl;
	type_t type;
}

%nterm <prog>        program
%nterm <dlist>    	 decl_list
%nterm <flist>    	 func_list
%nterm <slist>    	 stmt_list
%nterm <main>        main_func

%nterm <decl>		 decl
%nterm <type>		 type

%nterm <stmt>		 assignment_statement

%nterm <expr>		 expr
%nterm <stmt>		 stmt
%nterm <juxt>		 juxt
%nterm <form>		 form
%nterm <atom>		 atom

%nterm <expr>		 if_stmt

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
	: /* empty */ { $$ = NULL; }
	| decl decl_list { $$ = CreateDeclList( $1, $2 ); }
;

stmt_list
	: /* empty */ { $$ = NULL; }
	| stmt stmt_list { $$ = CreateStmtList( $1, $2 ); }
;

main_func
	: stmt_list expr 
	{
		$$ = CreateMain( NULL, $1, $2 );
	}
;

/* 

types 

*/

type
	: INT { $$ = CreatePrimitiveType( TINT ); }
	| FLOAT { $$ = CreatePrimitiveType( TFLOAT ); }
;


/* 

	Declarations 

*/

decl
	: type IDENT ';' 		 	{ $$ = CreateDecl( $1, $2 ); }
;

/*

	Statements

*/

assignment_statement
	: type IDENT '=' expr ';' 		{ $$ = CreateDeclAssignStmt( $1, $2, $4 ); }
	| IDENT '=' expr ';'	 		{ $$ = CreateAssignStmt( $1, $3 ); }
;

stmt
	: if_stmt				 { $$ = $1; }
	| assignment_statement 	 { $$ = $1; }
;

if_stmt
	: IF '(' expr ')' stmt 	 { $$ = CreateIfStatement( $3, $5 ); }
	| IF '(' expr ')' stmt ELSE stmt { $$ = NULL; }
;


/* Evaluates, to a variable or constant */
expr
	: form					 { $$ = CreateExprConst( $1 ); }
;

form
	: form '+' form          { $$ = CreateFormFromOp( $1, TPLUS, $3 ); }
	| form '-' form       	 { $$ = CreateFormFromOp( $1, TMINUS, $3 ); }
	| form '*' form          { $$ = CreateFormFromOp( $1, TSTAR, $3 ); }
	| form '/' form       	 { $$ = CreateFormFromOp( $1, TSLASH, $3 ); }
	| '(' form ')'			 { $$ = $2; }
	| juxt                   { $$ = CreateFormFromJuxt( $1 ); }
;

juxt
	: atom               	 { $$ = CreateJuxtConst( $1 ); }
;

atom
	: INTVAL             	 { $$ = CreateAtomInt( $1 ); }
	| FLOATVAL           	 { $$ = CreateAtomFloat( $1 ); }
	| CHARVAL           	 { $$ = CreateAtomChar( $1 ); }
	| IDENT					 { $$ = CreateAtomIdent ( $1 ); }
;
%%

void yyerror( const char *str ) {
	fflush( stderr );
  	fprintf( stderr, "CScript parsing error: \n%*s\n%*s:%d:%d\n", col, "^", col, str, yylineno, col );
}
