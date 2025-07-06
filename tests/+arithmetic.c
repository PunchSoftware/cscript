#include <cscript.h>

int main( void ) {
	char *expr = "2 + 4";
	cscript_ast_t ast = CScriptParse( expr );

	//int res = CScriptInterp( ast );

	return 0;
}
