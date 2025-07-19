#include "cscript_types.h"
#include <reis/base.h>
#define LIBREIS_HASH
#include <reis/hmap.h>

#include "interpreter.h"
#include "ast/ast.h"
#include "ast/ast_types.h"
#include "cscript_internals.h"

static bool debugMode = false;

/* TODO this module is still under development, MANY features are still mssing. */

csValue_t* InterpProg( prog_t prog, bool enterDebugMode ) {
    if ( enterDebugMode ) {
        debugMode = true;
    }

    // add the global declarations
    InterpDeclList( prog->decllist );

    csValue_t *res = (csValue_t*)InterpMain( prog->main );
    
    printf( "[cscript ~ debug] finished interpretation.\n" );

    return res;
}

static void InterpDeclList( declList_t decls ) {
    while ( decls != NULL ) {
        InterpDecl( decls->decl );
        decls = decls->tail;
    }
}

static void InterpDecl( decl_t decl ) {
    CheckForNullNode( decl, INTERPRETER, FATAL, __func__ );
    
    str ident = decl->ident;
    int *value = (int*)MALLOC( sizeof(int) );
    *value = InterpExpr( decl->value )->intval;

    int *tableValue = (int*)HM_Get( variables, ident );

    if ( debugMode ) {
        fprintf( stdout, 
            "[cscript ~ Interpreter] Setting variable: %s to %d\n", ident, *value );
    }

    if ( tableValue != NULL ) {
        if ( debugMode ) {
            fprintf( stdout, 
        "[cscript ~ Interpreter] variable '%s' is being set externally to %d\n", ident, *tableValue );
        }
    } else {
        HM_Set( variables, ident, (void*)value );
    }    
}

static atom_t InterpMain( main_t main ) {
  return InterpExpr( main->returnexpr );
}

static atom_t InterpExpr( expr_t expr ) {
    CheckForNullNode( expr, INTERPRETER, FATAL, __func__ );

    atom_t res;

    switch ( expr->kind ) {
        case EXPR_CONST: 
            res = InterpConst( expr->constant );
            break;
        default: 
            fprintf( stderr, "FATAL: unexpected expr kind\n" ); 
            exit( 1 ); 
    }

    return res;
}

static atom_t InterpIdent( atom_t atom ) {

    int *value = (int*)HM_Get( variables, atom->ident );
    if ( value == NULL ) {
        fprintf( stderr,
            "[cscript ~ Interpreter] unknown variable '%s'\n",
        atom->ident );
        exit( 1 );
    }
    
    switch ( atom->type->kind ) {
        case PrimitiveType: {
            switch ( atom->type->primitivetype ) {
                case TINT:
                    atom->intval = *value;
                    break;
            }
        }
    }

    return atom;
}

static atom_t InterpConst( form_t form ) {

    switch ( form->kind ) {
        case FORM_CONST: {
            atom_t a = InterpJuxt( form->atomic );
            switch ( a->kind ) {
                case TFLOAT:
                case TCHAR:
                case TSTRING:
                case TINT:
                    return a;
                case TIDENT:
                    return InterpIdent( a );
                default:
                    fprintf( stderr, "No known type of Atom.\n" );
                    exit( 1 );
            }
        }
        case FORM_BINARYOP: {
            atom_t a = InterpConst( form->binaryop.left );
            int l = InterpConst( form->binaryop.left )->intval;
            int r = InterpConst( form->binaryop.right )->intval;
            switch ( form->binaryop.op ) {
                case TPLUS:
                    a->intval = l + r;
                    return a;
                case TMINUS:
                    a->intval = l - r;
                    return a;
                case TSTAR:
                    a->intval = l * r;
                    return a;
                case TSLASH:
                    a->intval = l / r;
                    return a;
                default:
                    break;
            }
            break;
        }
        default: 
            fprintf(stderr, "FATAL: unexpected form\n"); 
            exit(1); 
    }

    // something went wrong
    exit(-1);
}

static atom_t InterpJuxt( juxt_t juxt ) {
        
    switch ( juxt->kind ) {
        case JUXT_CONST:
            return juxt->ATOM;
        case JUXT_NEGATE:
            switch( juxt->type->primitivetype ) {
                case TINT:
                    juxt->ATOM->intval = -juxt->ATOM->intval;
                    break;
                default:
                    break;
            }
            return juxt->ATOM;
        default:
            break;
    }

    // something went wrong
    exit(-1);
}