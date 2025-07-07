#include "ast.h"
#include <reis/base.h>
#include <reis/memory.h>
#include <stdio.h>

// `main` will always be the final statement rather than having
// to manually write main each time.
// Therefore if nothing is returned proper then it defaults to 0
// Or if it is left on some calculation i.e. `2 + 3`, the result
// will be 5. The main statement will have no ; at the end.
cscript_ast_t CreateProg( decllist_t decllist, 
                    funclist_t funclist, main_t main ) {
    if ( NULL == main ) {
        fprintf( stderr, 
            "FATAL: setting main in create_prog to NULL!\n" );
        exit( 1 );
    }
    cscript_ast_t val = MALLOC( sizeof(*val) );
    val->decllist = decllist;
    val->funclist = funclist;
    val->main = main;
    return val;
}

main_t CreateMain( decllist_t decllist, stmtlist_t stmtlist, expr_t returnexpr ) {
    if ( NULL == returnexpr ) {
        fprintf( stderr, "FATAL: setting returnexpr in create_main to NULL!\n" );
        exit( 1 );
    }
    main_t val = MALLOC( sizeof(*val) );
    val->decllist = decllist;
    val->stmtlist = stmtlist;
    val->returnexpr = returnexpr;
    return val;
}

atom_t CreateIntAtom( int intexpr ) {
  atom_t val = MALLOC( sizeof(*val) );
  val->intval = intexpr;
  val->kind = AINT;
  return val;
}

atom_t CreateFloatAtom( float floatexpr ) {
  atom_t val = MALLOC( sizeof(*val) );
  val->floatval = floatexpr;
  val->kind = AFLOAT;
  return val;
}

juxt_t CreateJuxt( atom_t atom ) {
  juxt_t val = MALLOC( sizeof(*val) );
  val->ATOM = atom;
  return val;
}

form_t CreateFormFromJuxt( juxt_t juxt ) {
    form_t val = MALLOC( sizeof(*val) );
    val->kind = FORM_CONST;
    val->atomic = juxt;
    return val;
}

form_t CreateFormFromOp( form_t left, op_e op, form_t right ) {
    if ( NULL == left ) {
        fprintf( stderr, "FATAL: setting left in create_binaryexpr to NULL!\n" );
        exit( 1 );
    }
    if ( NULL == right ) {
        fprintf( stderr, "FATAL: setting right in create_binaryexpr to NULL!\n" );
        exit( 1 );
    }

    form_t val = MALLOC( sizeof(*val) );
    val->kind = FORM_BINARYOP;
    val->binaryop.left = left;
    val->binaryop.op = op;
    val->binaryop.right = right;
    return val;
}

expr_t CreateExprConst( form_t constant ) {
    expr_t val = MALLOC( sizeof(*val) );
    val->kind = CONST;
    val->constant = constant;
    return val;
}

type_t CreatePrimitiveType( typename_e primtype ) {
    type_t val = MALLOC(sizeof(*val));
    val->kind = PrimitiveType;
    val->primitivetype = primtype;
    return val;
}




#define NUM_SYMS 2
static char syms[NUM_SYMS] = { '|', '|' };

void PrintIndent( int depth ) {
    int curr_char = 0;
    if ( depth > 0 ) {
        for ( int i = 0; i< depth; i++ ) {
            printf( "|  " );
            curr_char = (curr_char + 1) % NUM_SYMS;
        }
    }
    if ( depth == -1 ){
        return;
    }
    
    printf( "+-" );
}

// Printing

void PrintProg( cscript_ast_t prog_node, int indent ) {
    CheckForNullNode(prog_node, "Parser", "FATAL", "prog_node is NULL in void print_prog(T_prog prog_node, int indent)" );
    PrintIndent( indent );
    printf( "prog\n" );
    indent++;
    //print_decllist( prog_node->decllist, indent );
    //print_funclist( prog_node->funclist, indent );
    PrintMain( prog_node->main, indent );
}

void PrintMain( main_t main_node, int indent ) {
    CheckForNullNode(main_node, "Parser", "FATAL", "main_node is NULL in void print_main(T_main main_node, int indent)" );
    PrintIndent( indent );
    printf( "main\n" );
    indent++;
    PrintExpr( main_node->returnexpr, indent );
}

// TODO have a print buffer before moving down in depth.

void PrintBinaryOp( form_t form_node, int indent ) {
    PrintConst( form_node->binaryop.left, indent );

    char c[1] = {};
    switch ( form_node->binaryop.op ) {
        case TPLUS: c[0] = '+'; break;
        default: fprintf(stderr, "FATAL: unexpected operator\n"); exit(1); break;
    }
    printf( " %s ", c );
    PrintConst( form_node->binaryop.right, -1 );
    printf( "\n" );
}

void PrintConst( form_t form_node, int indent ) {
    switch ( form_node->kind ) {
        case FORM_CONST: PrintIntAtom( form_node->atomic->ATOM, indent ); break;
        case FORM_BINARYOP: PrintBinaryOp( form_node, indent ); break;
        default: fprintf(stderr, "FATAL: unexpected form\n"); exit(1); break;
    }
}


void PrintExpr( expr_t expr_node, int indent ) {
    CheckForNullNode(expr_node, "Parser", "FATAL", "expr_node is NULL in void print_expr(T_expr expr_node, int indent)" );

    switch ( expr_node->kind ) {
        case CONST: PrintConst( expr_node->constant, indent ); break;
        default: fprintf(stderr, "FATAL: unexpected expr kind\n"); exit(1); break;
    }
}



void PrintInt( int value, int depth ) {
    PrintIndent( depth );
    printf( "%d", value );
}

void PrintIntAtom( atom_t aint, int indent ) {
  if ( NULL == aint ) {
    fprintf(stderr, "FATAL: intexpr_node is NULL in void print_intexpr(T_expr intexpr_node, int indent)\n");
    exit(1);
  }

  PrintIndent( indent );
  printf( "(Int " );
  PrintInt( aint->intval, -1 );
  printf( ")" );
}

// Util

void CheckForNullNode( void *node, const char *module, const char *severity, const char *msg ){
    if ( node == NULL ) {
        fprintf( stderr, "[cscript][%s] %s: %s\n", module, severity, msg );
        exit( 1 );
    }
}