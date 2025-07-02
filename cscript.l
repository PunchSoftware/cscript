%{
#include <math.h>
#include "cscript_parser.h"

%}

%option noyywrap
%option header-file="cscript_lexer.h"

num     [0-9]
id      [a-z|_][a-z0-9]*


%%

// Values
{num}+              { 
		yylval.intval = atoi( yytext );
		return INTVAL; 
}

{num}+"."{num}*     { 
		yylval.floatval = atof( yytext );
		return FLOATVAL; 
}

// Arithmetic ops
"+"                 { return PLUS; }
"-"                 { return MINUS; }
"*"                 { return STAR; }
"/"                 { return SLASH; }

"("	                { return LPAREN; }
")"	                { return RPAREN; }

if|then|begin|end|procedure|function        {
            printf( "A keyword: %s\n", yytext );
            }

{id}        printf( "An identifier: %s\n", yytext );

"//".*       // One line comments

[ \t\n]+         /* eat up whitespace */

";"         { return ';'; }

.           printf( "Unrecognized character: %s\n", yytext );

%%
