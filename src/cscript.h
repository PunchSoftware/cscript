/* Copyright 2025 Punch Software

   A 'single-header' dynamically typed, subset of the C langauge.
*/

#ifndef PUNCH_CSCRIPT_H
#define PUNCH_CSCRIPT_H

#include <stddef.h>
#include <reis/base.h>
#define LIBREIS_HASH
#include <reis/hmap.h>

#include "cscript_lexer.h"
#include "cscript_parser.h"
#include "cscript_types.h"
#include "ast/ast.h"

extern int yylex( void );
extern void yyerror( const char *s );
extern int yyparse( void );

cscript_ast_t CScriptParse( char *expr, int opts );
int CScriptInterp( cscript_ast_t ast, int opts );

#endif /* PUNCH_CSCRIPT_H */