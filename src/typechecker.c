#include <stdio.h>
#include <stdbool.h>
#define LIBREIS_HASH
#include <reis/hmap.h>

#include "typechecker.h"
#include "ast.h"

static type_t INTTYPE;
static type_t FLOATTYPE;
static type_t CHARTYPE;
static type_t STRINGTYPE;
static type_t STRINGARRAYTYPE;

static scope_t current_scope;

static scope_t ScopeCreate( scope_t parent ) {
    scope_t scope = MALLOC( sizeof(*scope) );
    scope->table = HM_New();
    scope->parent = parent;
    return scope;
}

static void ScopeDestroy( scope_t scope ) {
    FREE( scope );
}

static type_t ScopeLookup( scope_t scope, char *ident ) {
    while ( NULL != scope ) {
        // check for a binding in this scope
        type_t type = (type_t) HM_Get( scope->table, ident );
        if ( NULL != type ) {
            return type;
        }
        // no binding in this scope, so check the parent
        scope = scope->parent;
    }

    return NULL; // No binding found in any scope :[
}

void CheckProg( cscript_ast_t prog ) {
    // initialize useful types
    INTTYPE = CreatePrimitiveType( TInt );
    FLOATTYPE = CreatePrimitiveType( TFloat );
    CHARTYPE = CreatePrimitiveType( TChar );
    //STRINGTYPE = create_pointertype(CHARTYPE);
    //STRINGARRAYTYPE = create_pointertype(STRINGTYPE);
    // create the global scope
    current_scope = ScopeCreate( NULL );
    // add the global declarations
    //check_decllist(prog->decllist);
    // check the function definitions
    //check_funclist(prog->funclist);
    // check the main function
    CheckMain( prog->main );

    printf( "[cscript][debug] passed type checker.\n" );
    ScopeDestroy( current_scope );
}

static void CheckMain( main_t main ) {
  // create a new scope
  current_scope = ScopeCreate( current_scope );

  // add argc and argv with their C runtime types
  //insert(current_scope->table, "argc", INTTYPE);
  //insert(current_scope->table, "argv", STRINGARRAYTYPE);
  // add the declarations to the symtab
  //check_decllist(main->decllist);
  // check the function body for type errors
  //check_stmtlist(main->stmtlist);
  // check the return expression for type errors
  CheckExpr( main->returnexpr );

  // check that the return type is an int, per C runtime
  /*if (! compare_types(main->returnexpr->type, INTTYPE)) {
    type_error("the return expression type does not match the function type");
  }*/

  // restore parent scope
  scope_t parent_scope = current_scope->parent;
  ScopeDestroy( current_scope ); current_scope = parent_scope;
}

static type_t CheckAtom( atom_t atom ) {
    switch ( atom->kind ) {
        case AINT:
            atom->type = INTTYPE;
            break;
        case AFLOAT:
            atom->type = FLOATTYPE;
            break;
        default:
            break;
    }

    return atom->type;
}

static type_t CheckJuxt( juxt_t juxt ) {
    switch ( juxt->kind ) {
        case JUXTCONST:
            juxt->type = CheckAtom( juxt->ATOM );
            return juxt->type;
        default:
            break;
    }

    return NULL;
}

static type_t CheckConst( form_t form ) {
    switch ( form->kind ) {
        case FORM_CONST:
            form->type = CheckJuxt( form->atomic );
            break;
        case FORM_BINARYOP: 
            switch ( form->binaryop.op ) {
                case TPLUS:
                    CheckConst( form->binaryop.left );
                    CheckConst( form->binaryop.right );
                    break;
                default:
                    break;
            }
            CompareTypes( form->binaryop.left->type, form->binaryop.right->type ); 
            break;
        default: fprintf(stderr, "FATAL: unexpected form\n"); exit(1); break;
    }

    return form->type;
}

static void CheckExpr( expr_t expr ) {
    CheckForNullNode( expr, "TypeChecker", "FATAL", "unexpected NULL in check_expr" );

    switch ( expr->kind ) {
        case CONST: CheckConst( expr->constant ); break;
        default: fprintf(stderr, "FATAL: unexpected expr kind\n"); exit(1); break;
    }
}


bool CompareTypes( type_t type1, type_t type2 ) {
    if ( type1 == NULL || type2 == NULL  ) {
        fprintf( stderr, "[cscript][TypeChecker] FATAL: unexpected NULL values in compare_types\n" );
        exit( 1 );
    }
    if ( type1->kind == type2->kind ) {
        switch ( type1->kind ) {
            case PrimitiveType:
                return type1->primitivetype == type2->primitivetype;
                break;
            default:
                fprintf(stderr, "FATAL: unexpected kind in compare_types\n");
                exit(1);
        }
    } else {
        return false;
    }
}