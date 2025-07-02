/* Copyright 2025 Punch Software

   A 'single-header' dynamically typed, subset of the C langauge.
*/

#ifndef PUNCH_CSCRIPT_H
#define PUNCH_CSCRIPT_H

#include <stddef.h>
#include "cscript_lexer.h"
#include "cscript_parser.h"

extern int yylex( void );
extern void yyerror( const char *s );
extern int yyparse( void );

typedef struct {
		char *raw_data;
} cscript_ast_t;

cscript_ast_t* CScriptParse( char *expr ) {
		cscript_ast_t* ast = NULL;
    YY_BUFFER_STATE buffer_state = NULL;
		buffer_state = yy_scan_string( expr );
		int ret = yyparse();

    yy_delete_buffer( buffer_state );


		if ( ret != 0 ) {
				fprintf( stderr, "Failed to parse C expression.\n" );
				return NULL;
		}
		
		return ast;	
}

#endif /* PUNCH_CSCRIPT_H */
