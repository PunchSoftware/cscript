#include <cscript.h>
#include <stdio.h>

int main( void ) {
	char *expr = "int x = 4; 1 + 2 * x";
	csAST_t ast = CS_Parse( expr, DEFAULT | SHOW_AST );

	int *x = (int*)MALLOC( sizeof(int) );
	*x = 10;
	CS_Set( "x", (void*)x );

	csValue_t *res = CS_Interp( ast, DEFAULT | DEBUG );

	printf( "Final result: %d\n", res->intval );

	x = (int*)CS_Get("x");

	printf( "%d\n", *(int*)x );

	return 0;
}
