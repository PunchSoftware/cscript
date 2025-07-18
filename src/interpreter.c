#include <reis/base.h>

#include "interpreter.h"
#include "ast/ast.h"
#include "ast/ast_types.h"

static bool debugMode = false;

/* TODO this module is still under development, MANY features are still mssing. */

int InterpProg( prog_t prog, bool enterDebugMode ) {
    if ( enterDebugMode ) {
        debugMode = true;
    }

    int res = InterpMain( prog->main );
    
    printf( "[cscript][debug] finished interpretation.\n" );

    return res;
}

static int InterpMain( main_t main ) {
  return InterpExpr( main->returnexpr );
}

static int InterpExpr( expr_t expr ) {
    CheckForNullNode( expr, INTERPRETER, FATAL, __func__ );

    int res = 0;

    switch ( expr->kind ) {
        case EXPR_CONST: 
            res = InterpConst( expr->constant )->intval; 
            break;
        default: 
            fprintf( stderr, "FATAL: unexpected expr kind\n" ); 
            exit( 1 ); 
    }

    return res;
}

static atom_t InterpConst( form_t form ) {

    switch ( form->kind ) {
        case FORM_CONST: {
            atom_t a = InterpJuxt( form->atomic );
            switch ( a->kind ) {
                case TINT:
                case TFLOAT:
                case TCHAR:
                    return a;
                default:
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