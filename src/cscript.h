/* Copyright 2025 Punch Software

   A 'single-header' dynamically typed, subset of the C langauge.
*/

#ifndef PUNCH_CSCRIPT_H
#define PUNCH_CSCRIPT_H

#include <stddef.h>
#include <reis/base.h>

#include "cscript_lexer.h"
#include "cscript_parser.h"
#include "cscript_types.h"
#include "ast/ast.h"

/* Private */
extern int yylex( void );
extern void yyerror( const char *s );
extern int yyparse( void );

/* Public API */
csAST_t CS_Parse( char *expr, int opts );
csValue_t* CS_Interp( csAST_t ast, int opts );

void* CS_Get( str ident );
void CS_Set( str ident, void* value );

#endif /* PUNCH_CSCRIPT_H */