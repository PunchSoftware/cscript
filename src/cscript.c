#include "cscript.h"

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
