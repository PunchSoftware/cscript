/* Copyright 2025 Punchline Software

*/

#ifndef PUNCHLINE_ARITHMETIC_H
#define PUNCHLINE_ARITHMETIC_H
#include <cscript.h>
#include <stdio.h>

inline int TestVariables( void ) {
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

inline static int TestArithmeticOperations( void ) {
	char *expr = "1 + 2 * 4";
	csAST_t ast = CS_Parse( expr, DEFAULT | SHOW_AST );

	csValue_t *res = CS_Interp( ast, DEFAULT | DEBUG );

	if ( res->intval == 9 ) {
			return 0;
	} else {
			exit( 1 );
	}
}

#endif /* PUNCHLINE_ARITHMETIC_H */
