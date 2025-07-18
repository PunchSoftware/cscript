#include "ast.h"
#include "ast_types.h"
#include <reis/base.h>
#include <reis/memory.h>
#include <stdio.h>

// `main` will always be the final statement rather than having
// to manually write main each time.
// Therefore if nothing is returned proper then it defaults to 0
// Or if it is left on some calculation i.e. `2 + 3`, the result
// will be 5. The main statement will have no ; at the end.
prog_t CreateProg( declList_t decllist, 
                    funcList_t funclist, main_t main ) {
    if ( NULL == main ) {
        fprintf( stderr, 
            "FATAL: setting main in create_prog to NULL!\n" );
        exit( 1 );
    }
    prog_t val = MALLOC( sizeof(*val) );
    val->decllist = decllist;
    val->funclist = funclist;
    val->main = main;
    return val;
}

main_t CreateMain( declList_t decllist, stmtList_t stmtlist, expr_t returnexpr ) {
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

stmt_t CreateAssignStmt( expr_t id, expr_t expr ) {
    CheckForNullNode( id, PARSER, FATAL, __func__ );
    CheckForNullNode( expr, PARSER, FATAL, __func__ );

    stmt_t val = MALLOC( sizeof(*val) );
    val->kind = TASSIGNSTMT;
    val->assignStmt.id = id;
    val->assignStmt.value = expr;
    return val;
}

stmtList_t CreateStmtList( stmt_t stmt, stmtList_t tail ) {
    CheckForNullNode( stmt, PARSER, FATAL, __func__ );

    stmtList_t stmts = MALLOC( sizeof(*stmts) );
    stmts->stmt = stmt;
    stmts->tail = tail;
    return stmts;
}

stmt_t CreateDeclAssignStmt( type_t type, expr_t id, expr_t expr ) {
    CheckForNullNode( id, PARSER, FATAL, __func__ );
    CheckForNullNode( expr, PARSER, FATAL, __func__ );

    stmt_t val = MALLOC( sizeof(*val) );
    val->kind = TDECLASSIGNSTMT;
    val->assignDeclStmt.type = type;
    val->assignDeclStmt.id = id;
    val->assignDeclStmt.value = expr;
    return val;
}

declList_t CreateDeclList( decl_t decl, declList_t tail ) {
    CheckForNullNode( decl, PARSER, FATAL, __func__ );

    declList_t dlist = MALLOC( sizeof(*dlist) );
    dlist->decl = decl;
    dlist->tail = tail;
    return dlist;
}

decl_t CreateDecl( type_t type, str ident ) {
    CheckForNullNode( type, PARSER, FATAL, __func__ );

    decl_t decl = MALLOC( sizeof(*decl) );
    decl->type = type;
    decl->ident = ident;
    return decl;
}

juxt_t CreateJuxtConst( atom_t atom ) {
    juxt_t juxt = MALLOC( sizeof(*juxt) );
    juxt->kind = JUXT_CONST;
    juxt->ATOM = atom;
    return juxt;
}

juxt_t CreateJuxtNegate( atom_t atom ) {
    juxt_t juxt = MALLOC( sizeof(*juxt) );
    juxt->kind = JUXT_NEGATE;
    juxt->ATOM = atom;
    return juxt;
}

form_t CreateFormFromJuxt( juxt_t juxt ) {
    form_t val = MALLOC( sizeof(*val) );
    val->kind = FORM_CONST;
    val->atomic = juxt;
    return val;
}

form_t CreateFormFromOp( form_t left, op_t op, form_t right ) {
    CheckForNullNode( left, PARSER, FATAL, __func__ );
    CheckForNullNode( right, PARSER, FATAL, __func__ );
    form_t val = MALLOC( sizeof(*val) );
    val->kind = FORM_BINARYOP;
    val->binaryop.left = left;
    val->binaryop.op = op;
    val->binaryop.right = right;
    return val;
}

expr_t CreateExprConst( form_t constant ) {
    expr_t val = MALLOC( sizeof(*val) );
    val->kind = EXPR_CONST;
    val->constant = constant;
    return val;
}

type_t CreatePrimitiveType( typename_t primtype ) {
    type_t val = MALLOC( sizeof(*val) );
    val->kind = PrimitiveType;
    val->primitivetype = primtype;
    return val;
}

stmt_t CreateIfStatement( expr_t conditon, stmt_t body ) {
    CheckForNullNode( conditon, PARSER, FATAL, __func__ );
    CheckForNullNode( body, PARSER, FATAL, __func__ );

    stmt_t ifStmt = MALLOC( sizeof(*ifStmt) );
    ifStmt->kind = TIFSTMT;
    ifStmt->ifStmt.conditon = conditon;
    ifStmt->ifStmt.body = body;
}

/*

    Atom Functions

*/

atom_t CreateAtomInt( int i ) {
    atom_t val = MALLOC( sizeof(*val) );
    val->intval = i;
    val->kind = TINT;
    return val;
}

atom_t CreateAtomFloat( float f ) {
    atom_t val = MALLOC( sizeof(*val) );
    val->floatval = f;
    val->kind = TFLOAT;
    return val;
}

atom_t CreateAtomChar( char c ) {
    atom_t val = MALLOC( sizeof(*val) );
    val->floatval = c;
    val->kind = TCHAR;
    return val;
}

atom_t CreateAtomIdent( str i ) {
    atom_t val = MALLOC( sizeof(*val) );
    val->ident = i;
    val->kind = TIDENT;
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

void PrintProg( prog_t prog_node, int indent ) {
    CheckForNullNode(prog_node, PARSER, FATAL, __func__ );
    PrintIndent( indent );
    printf( "prog\n" );
    indent++;
    //print_decllist( prog_node->decllist, indent );
    //print_funclist( prog_node->funclist, indent );
    PrintMain( prog_node->main, indent );
}

void PrintMain( main_t main_node, int indent ) {
    CheckForNullNode(main_node, PARSER, FATAL, __func__ );
    PrintIndent( indent );
    printf( "main\n" );
    indent++;
    PrintExpr( main_node->returnexpr, indent );
    printf( "\n" );
}

void PrintBinaryOp( form_t form_node, int indent ) {
    PrintConst( form_node->binaryop.left, indent );

    char c[1] = {};
    switch ( form_node->binaryop.op ) {
        case TPLUS: c[0] = '+'; break;
        case TSTAR: c[0] = '*'; break;
        case TMINUS: c[0] = '-'; break;
        case TSLASH: c[0] = '/'; break;
        default: fprintf(stderr, "FATAL: unexpected operator\n"); exit(1); break;
    }
    printf( " %s ", c );
    PrintConst( form_node->binaryop.right, -1 );
    fflush( stdout );
}

void PrintConst( form_t form_node, int indent ) {
    switch ( form_node->kind ) {
        case FORM_CONST: PrintIntAtom( form_node->atomic->ATOM, indent ); break;
        case FORM_BINARYOP: PrintBinaryOp( form_node, indent ); break;
        default: fprintf(stderr, "FATAL: unexpected form\n"); exit(1); break;
    }
}


void PrintExpr( expr_t expr_node, int indent ) {
    CheckForNullNode(expr_node, PARSER, FATAL, __func__ );

    switch ( expr_node->kind ) {
        case EXPR_CONST: PrintConst( expr_node->constant, indent ); break;
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

void CheckForNullNode( void *node, csMod_t module, csSeverity_t severity, const char *funcName ) {

    char moduleName[14] = {'\0'};
    char severityName[12] = {'\0'};

    switch ( module ) {
        case PARSER:
            strncpy( moduleName, "Parser", 7 );
            break;
        case TYPECHECKER:
            strncpy( moduleName, "TypeChecker", 12 );
            break;
        case INTERPRETER:
            strncpy( moduleName, "Interpreter", 12 );
            break;
        default:
            strncpy( moduleName, "Unknown", 8 );
            break;
    }

    switch ( severity ) {
        case FATAL:
            strncpy( severityName, "FATAL", 6 );
            break;
        default:
            strncpy( severityName, "ERROR", 6 );
            break;
    }

    if ( node == NULL ) {
        fprintf( stderr, "[cscript - %s] %s: %s encountered an unexpected NULL node.\n", moduleName, severity, funcName );
        exit( 1 );
    }
}