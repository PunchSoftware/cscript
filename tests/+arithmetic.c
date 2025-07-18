#include <cscript.h>
#include <stdio.h>

int main( void ) {
	char *expr = "x = 4; 6 - (8 + 2)";
	cscript_ast_t ast = CScriptParse( expr, DEFAULT | SHOW_AST );

	int res = CScriptInterp( ast, DEFAULT | DEBUG );

	printf( "Final result: %d\n", res );

	return 0;
}
