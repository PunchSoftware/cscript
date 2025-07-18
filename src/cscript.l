%{
#include <math.h>

#include "cscript_parser.h"

void Count( void );

%}

%option yylineno
%option noyywrap

num     [0-9]
id      [a-z|_][a-z0-9]*

%%

"int"				{ Count(); return INT; }
"char"				{ Count(); return CHAR; }


{num}+              { 
	Count();
	yylval.intval = atoi( yytext );
	return INTVAL; 
}

{num}+"."{num}*     {
	Count();
	yylval.floatval = atof( yytext );
	return FLOATVAL; 
}

"="					{ Count(); return '='; }

"+"                 { Count(); return '+'; }
"-"                 { Count(); return '-'; }
"*"                 { Count(); return '*'; }
"/"                 { Count(); return '/'; }

"("	                { Count(); return '('; }
")"	                { Count(); return ')'; }

"int"				{ Count(); return( INT ); }
"float"				{ Count(); return( FLOAT ); }


{id}        		{ 
	Count(); 
	yylval.strval = yytext;
	return IDENT;
}

"if"				{ Count(); return( IF ); }
"else"				{ Count(); return( ELSE ); }


"//".*       // One line comments
";"         		{ Count(); return ';'; }
[ \t\v\n\f]+        { Count(); } /* eat up whitespace */
.           printf( "Unrecognized character: %s\n", yytext );

%%

int col = 0;

void Count( void ) {
	for ( int i = 0; yytext[i] != '\0'; i++ ) {
		if ( yytext[i] == '\n' ){
			col = 0;
		} else if ( yytext[i] == '\t' ){
			col += 8 - ( col % 8 );
		} else {
			col++;
		}
	}

	//ECHO;
}