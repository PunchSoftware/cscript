#include <iso646.h>
#include <stdarg.h>
#define LIBREIS_HASH
#include <reis/hmap.h>

#include "cscript.h"
#include "cscript_types.h"
#include "cscript_internals.h"
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
csAST_t CS_Parse( char *expr, int opts ) {
	variables = HM_New();
	extern csAST_t ast;

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

	YY_BUFFER_STATE buffer_state = NULL;
	buffer_state = yy_scan_string( expr );
	int ret = yyparse();
	yy_delete_buffer( buffer_state );

	if ( ret != 0 ) {
		CScriptError( "Failed to parse C expression." );
		return NULL;
	}

	if ( shouldPrintAST ) {
		PrintProg( ast, 0 );
	}

	return ast;
}

csValue_t* CS_Interp( csAST_t ast, int opts ) {
	if ( ast == NULL ) {
		CScriptError( "AST passed to CScriptInterp was NULL." );
		return NULL;
	}

	csValue_t *res;

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

void* CS_Get( str ident ) {
	void* value = HM_Get( variables, ident );
	return value;
}

void CS_Set( str ident, void* value ) {
	HM_Set( variables, ident, value );
}