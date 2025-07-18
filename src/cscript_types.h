/* Copyright 2025 Punch Software
*/

#ifndef PUNCH_CSCRIPT_TYPES_H
#define PUNCH_CSCRIPT_TYPES_H

#include <stddef.h>

#include "ast/ast.h"

typedef prog_t cscript_ast_t;

typedef enum {
    DEFAULT = 0,
    DEBUG = -1,
} sharedOpts_t;

typedef enum {
    SHOW_AST = 1
} astOpts_t;

typedef enum {
    NO_TYPECHECK = 1
} interpOpts_t;

#endif /* PUNCH_CSCRIPT_TYPES_H */