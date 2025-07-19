/* Copyright 2025 Punch Software
*/

#ifndef PUNCH_CSCRIPT_INTERPRETER_H
#define PUNCH_CSCRIPT_INTERPRETER_H

#include "ast/ast.h"
#include "ast/ast_types.h"
#include "cscript_types.h"

// TODO makes sense to return any type, breaks C standard but for scripting could prove more useful.
csValue_t* InterpProg( prog_t prog, bool enterDebugMode );

static atom_t InterpMain( main_t main );

static void InterpDeclList( declList_t decls );
static void InterpDecl( decl_t decl );

static atom_t InterpExpr( expr_t expr );

static atom_t InterpConst( form_t form );
static atom_t InterpJuxt( juxt_t juxt );

#endif /* PUNCH_CSCRIPT_INTERPRETER_H */
