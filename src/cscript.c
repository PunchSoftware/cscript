#include <iso646.h>
#include <stdarg.h>

#include "cscript.h"
#include "cscript_types.h"
#include "typechecker.h"
#include "interpreter.h"

hmap_t *variables = NULL;

static void CScriptError( const char *msg ) {
	//va_list args;
	//va_start( args, msg );
	fprintf( stderr, "[cscript] %s\n", msg );
	//va_end( args );
}


/**
 * @brief 
 * 
 * @param expr 
 * @param ... 
 * @return cscript_ast_t
 * @note Options look like, so far:
 * 
 */
cscript_ast_t CScriptParse( char *expr, int opts ) {
	variables = HM_New();
	extern cscript_ast_t ast;

	// Options
	bool shouldPrintAST = false;
	bool debugMode = false;

	switch ( opts ) {
		case -1:
			debugMode = true;
			break;
		case 0:
			break;
		case 1:
			shouldPrintAST = true;
			break;
		default:
			break;
	}

	//HM_Set( variables, "dd", (void*)2);
	YY_BUFFER_STATE buffer_state = NULL;
	buffer_state = yy_scan_string( expr );
	int ret = yyparse();
	yy_delete_buffer(buffer_state);

	if ( ret != 0 ) {
		CScriptError( "Failed to parse C expression." );
		return NULL;
	}

	if ( shouldPrintAST ) {
		PrintProg( ast, 0 );
	}

	return ast;
}

int CScriptInterp( cscript_ast_t ast, int opts ) {
	if ( ast == NULL ) {
		CScriptError( "AST passed to CScriptInterp was NULL." );
		return -1;
	}

	int res = 0;

	// Options
	bool shouldTypeCheck = true;
	bool debugMode = false;

	switch ( opts ) {
		case -1:
			debugMode = true;
			break;
		case 0:
			break;
		case 1:
			shouldTypeCheck = false;
			break;
		default:
			break;
	}

	if ( shouldTypeCheck ) {
		CheckProg( ast, debugMode );
	}

	res = InterpProg( ast, debugMode );
	

	return res;
}
