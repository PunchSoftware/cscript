#include "cscript.h"

int main( int argc, char **argv ){
		
		char *expr = "5 - (2 + 4);";
		cscript_ast_t *ast = CScriptParse( expr );

		return 0;
}
