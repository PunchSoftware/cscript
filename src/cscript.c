#include "cscript.h"
#include "typechecker.h"

hmap_t *variables = NULL;

cscript_ast_t CScriptParse( char *expr ) {
	variables = HM_New();
	extern cscript_ast_t ast; 

	//HM_Set( variables, "dd", (void*)2);
	YY_BUFFER_STATE buffer_state = NULL;
	buffer_state = yy_scan_string( expr );
	int ret = yyparse();
	yy_delete_buffer(buffer_state);

	if ( ret != 0 ) {
		fprintf(stderr, "Failed to parse C expression.\n");
		return NULL;
	}

	// Type check
	CheckProg( ast );

	// Debug
	PrintProg( ast, 0 );

	return ast;
}

int CScriptInterp( cscript_ast_t ast ) {
	int res = 0;
	if ( ast == NULL ) {
		fprintf( stderr, "[cscript] AST passed to CScriptInterp was NULl.\n" );
		return -1;
	}


	return res;
}
