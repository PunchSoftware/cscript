#include "ast/ast_types.h"
#include <stdio.h>
#include <reis/base.h>
#define LIBREIS_HASH
#include <reis/hmap.h>

#include "typechecker.h"
#include "ast/ast.h"

static type_t INTTYPE;
static type_t FLOATTYPE;
static type_t CHARTYPE;
static type_t STRINGTYPE;
static type_t STRINGARRAYTYPE;

static scope_t current_scope;

static bool debugMode = false;

void TypeError( const char *msg ) {
    fprintf( stdout, "%s\n", msg );
    exit( 1 );
}

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
        type_t type = (type_t)HM_Get( scope->table, ident );
        if ( type != NULL ) {
            return type;
        }
        // no binding in this scope, so check the parent
        scope = scope->parent;
    }

    return NULL; // No binding found in any scope :[
}

static void ScopeUpdate( char *ident, type_t type ) {
    HM_Set( current_scope->table, ident, (void*)type );
}

void CheckProg( prog_t prog, bool enterDebugMode ) {
    if ( enterDebugMode ) {
        debugMode = true;
    }

    // initialize useful types
    INTTYPE = CreatePrimitiveType( TINT );
    FLOATTYPE = CreatePrimitiveType( TFLOAT );
    CHARTYPE = CreatePrimitiveType( TCHAR );
    //STRINGTYPE = create_pointertype(CHARTYPE);
    //STRINGARRAYTYPE = create_pointertype(STRINGTYPE);
    // create the global scope
    current_scope = ScopeCreate( NULL );
    // add the global declarations
    CheckDeclList( prog->decllist );
    // check the function definitions
    //check_funclist(prog->funclist);
    // check the main function
    CheckMain( prog->main );
    
    printf( "[cscript][debug] passed type checker.\n" );
    ScopeDestroy( current_scope );
}

static void CheckDeclList( declList_t decls ) {
    while ( decls != NULL ) {
        CheckDecl( decls->decl );
        decls = decls->tail;
    }
}

static void CheckDecl( decl_t decl ) {
    CheckForNullNode( decl, INTERPRETER, FATAL, __func__ );
    
    if ( debugMode ) {
        fprintf( stdout, 
            "[cscript ~ Parser] updating current scope with: %s of type TODO\n", 
            decl->ident );
        // print type
    }

    ScopeUpdate( decl->ident, decl->type );
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
        case TINT:
            atom->type = INTTYPE;
            break;
        case TFLOAT:
            atom->type = FLOATTYPE;
            break;
        case TIDENT: {
            type_t scopeType = (type_t)ScopeLookup( current_scope, atom->ident );
            if ( scopeType == NULL ) {
                fprintf( stderr, "[cscript] TYPE ERR: variable %s not in current scope.\n", atom->ident );
                exit( 1 );
            }
            atom->type =  scopeType;
            break;
        }
        default:
            TypeError( "Atom type unknown" );
    }

    return atom->type;
}

static type_t CheckAtomNumeric( atom_t atom ) {
    switch ( atom->kind ) {
        case TINT:
            atom->type = INTTYPE;
            break;
        case TFLOAT:
            atom->type = FLOATTYPE;
            break;
        default:
            TypeError( "Negate only works on ints and floats" );
    }

    return atom->type;
}

static type_t CheckJuxt( juxt_t juxt ) {
    switch ( juxt->kind ) {
        case JUXT_CONST:
            juxt->type = CheckAtom( juxt->ATOM );
            return juxt->type;
        case JUXT_NEGATE:
            juxt->type = CheckAtom( juxt->ATOM );
            return  juxt->type;
        default:
            fprintf( stderr, "FATAL: unexpected juxt\n" ); 
            exit( 1 );
    }

    return NULL;
}

void PrintPrimType( type_t type ) {
    switch ( type->primitivetype ){
        case TINT:
            printf( "INT" );
            break;
        default:
            fprintf( stderr, "FATAL: unexpected primitive type\n" ); 
            exit( 1 );
    }
}

void PrintType( type_t type ) {
    switch ( type->kind ) {
        case PrimitiveType:
            PrintPrimType( type );
            break;
        default:
            return;
    }
}

static type_t CheckConst( form_t form ) {
    switch ( form->kind ) {
        case FORM_CONST:
            form->type = CheckJuxt( form->atomic );
            break;
        case FORM_BINARYOP: 
            switch ( form->binaryop.op ) {
                case TPLUS:
                case TSTAR:
                case TMINUS:
                case TSLASH:
                    CheckConst( form->binaryop.left );
                    CheckConst( form->binaryop.right );
                    break;
                default:
                    break;
            }
            CompareTypes( form->binaryop.left->type, form->binaryop.right->type );
            form->type = form->binaryop.left->type; // Doesn't matter right or left.
            break;
        default: 
            fprintf(stderr, "FATAL: unexpected form\n"); 
            exit(1);
    }

    return form->type;
}

static void CheckExpr( expr_t expr ) {
    CheckForNullNode( expr, TYPECHECKER, FATAL, __func__ );

    switch ( expr->kind ) {
        case EXPR_CONST: CheckConst( expr->constant ); break;
        default: fprintf(stderr, "FATAL: unexpected expr kind\n"); exit(1);
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