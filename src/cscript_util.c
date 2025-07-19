
#include "cscript_util.h"

char* ToStrVal( char *yytext ) {
	size_t len = strlen( yytext );
	char *strval = (char*)MALLOC( sizeof(char *) * len + 1 );
	strval = strncpy( strval, yytext, len );
	strval[len] = '\0';
	return strval;
}