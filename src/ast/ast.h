/* Copyright 2025 Punch Software
*/

#ifndef PUNCH_CSCRIPT_AST_H
#define PUNCH_CSCRIPT_AST_H

#include <reis/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <reis/base.h>
#include <unistd.h>

#include "ast_types.h"


prog_t CreateProg( declList_t decllist, funcList_t funclist, main_t main );
main_t CreateMain( declList_t decllist, stmtList_t stmtlist, expr_t returnexpr );

declList_t CreateDeclList( decl_t decl, declList_t tail );
decl_t CreateDecl( type_t type, str ident );\

stmtList_t CreateStmtList( stmt_t stmt, stmtList_t tail );
stmt_t CreateAssignStmt( expr_t id, expr_t expr );
stmt_t CreateDeclAssignStmt( type_t type, expr_t id, expr_t expr );


// Printing functions
void PrintIndent( int depth );
void PrintString( str s, int depth );
void PrintInt( int value, int depth );
void PrintChar( char value, int depth );
void PrintProg( prog_t prog_node, int indent );

expr_t CreateExprConst( form_t constant );

form_t CreateFormFromJuxt( juxt_t juxt );
form_t CreateFormFromOp( form_t left, op_t op, form_t right );

juxt_t CreateJuxtConst( atom_t atom );
juxt_t CreateJuxtNegate( atom_t atom );

atom_t CreateAtomInt( int i );
atom_t CreateAtomFloat( float f );
atom_t CreateAtomChar( char c );
atom_t CreateAtomIdent( str i );


type_t CreatePrimitiveType( typename_t primtype );

void PrintIntAtom( atom_t aint, int indent );
void PrintExpr( expr_t expr_node, int indent );
void PrintConst( form_t form_node, int indent );

/*void print_typename( Type type, int indent );
void print_op( E_op op, int indent );
void print_exprlist( T_exprlist exprlist_node, int indent );
void print_identexpr( T_expr identexpr_node, int indent );
void print_callexpr( T_expr callexpr_node, int indent );
void print_intexpr( T_expr intexpr_node, int indent );
void print_charexpr( T_expr charexpr_node, int indent );
void print_strexpr( T_expr strexpr_node, int indent );
void print_arrayexpr( T_expr arrayexpr_node, int indent );
void print_unaryexpr( T_expr unaryexpr_node, int indent );
void print_binaryexpr( T_expr binaryexpr_node, int indent );
void print_castexpr( T_expr castexpr_node, int indent );
void print_assignstmt( T_stmt assignstmt_node, int indent );
void print_ifstmt( T_stmt ifstmt_node, int indent );
void print_ifelsestmt( T_stmt ifelsestmt_node, int indent );
void print_whilestmt( T_stmt whilestmt_node, int indent );
void print_compoundstmt( T_stmt compoundstmt_node, int indent );
void print_stmt( T_stmt stmt_node, int indent );
void print_stmtlist( T_stmtlist stmtlist_node, int indent );
void print_primitivetype( T_type primitivetype_node, int indent );
void print_pointertype( T_type pointertype_node, int indent );
void print_arraytype( T_type arraytype_node, int indent );
void print_functiontype( T_type functiontype_node, int indent );
void print_type( T_type type_node, int indent );
void print_typelist( T_typelist typelist_node, int indent );
void print_decl( T_decl decl_node, int indent );
void print_decllist( T_decllist decllist_node, int indent );
void print_paramlist( T_paramlist paramlist_node, int indent );
void print_func( T_func func_node, int indent );
void print_funclist( T_funclist funclist_node, int indent );*/
void PrintMain( main_t main_node, int indent );


// Util

typedef enum {
  PARSER,
  TYPECHECKER,
  INTERPRETER,
} csMod_t;

typedef enum {
  FATAL = 0,
} csSeverity_t;

void CheckForNullNode( void *node, csMod_t module, csSeverity_t severity, const char *funcName );

#endif /* PUNCH_CSCRIPT_AST_H */