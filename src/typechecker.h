/* Copyright 2025 Punch Software
*/

#ifndef PUNCH_CSCRIPT_TYPECHECKER_H
#define PUNCH_CSCRIPT_TYPECHECKER_H

#define LIBREIS_HASH
#include <reis/hmap.h>

#include "ast/ast.h"

typedef struct Scope *scope_t;

struct Scope {
  hmap_t *table;
  scope_t parent;
};

static scope_t ScopeCreate( scope_t parent );
static void ScopeDestroy( scope_t scope );
static type_t ScopeLookup( scope_t scope, char *ident );

static void TypeError( const char *msg );

bool CompareTypes( type_t type1, type_t type2 );

// Public API
void CheckProg( prog_t prog, bool enterDebugMode );

// Private
static void CheckMain( main_t main );
static void CheckExpr( expr_t expr );



#endif /* PUNCH_CSCRIPT_TYPECHECKER_H */