/* Copyright 2025 Punch Software
*/

#ifndef PUNCH_CSCRIPT_TYPES_H
#define PUNCH_CSCRIPT_TYPES_H

#include <stddef.h>

#include "ast/ast.h"

typedef prog_t csAST_t;
typedef decl_t csVar_t;
typedef struct Atom csValue_t;

typedef enum {
    DEFAULT = 0,
    DEBUG = -1,
} csSharedOpts_t;

typedef enum {
    SHOW_AST = 1
} csParseOpts_t;

typedef enum {
    NO_TYPECHECK = 1
} csInterpOpts_t;

#endif /* PUNCH_CSCRIPT_TYPES_H */