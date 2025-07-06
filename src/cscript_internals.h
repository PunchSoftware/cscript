/* Copyright 2025 Punch Soft.

*/

#ifndef PUNCH_CSCRIPT_INTERNALS_H
#define PUNCH_CSCRIPT_INTERNALS_H

#define LIBREIS_HASH
#include <reis/hmap.h>

// All variables are going to be global for now so they can be accessed from outside the class.
extern hmap_t *variables;

#endif /* PUNCH_CSCRIPT_INTERNALS_H */
