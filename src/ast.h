/* Copyright 2025 Punch Software
*/

#ifndef PUNCH_CSCRIPT_AST_H
#define PUNCH_CSCRIPT_AST_H

typedef char* str;
typedef struct Prog* cscript_ast_t;
typedef enum { TInt, TChar } typename_e;
typedef enum { E_op_ref, E_op_deref, 
    TPLUS, TMINUS, TNOT, TSTAR, 
    TSLASH, E_op_mod, E_op_eq, E_op_ne, 
    E_op_lt, E_op_le, E_op_gt, E_op_ge, 
    E_op_and, E_op_or } op_e;
typedef struct ExprList* exprlist_t;
typedef struct Expr* expr_t;
typedef struct Stmt* stmt_t;
typedef struct StmtList* stmtlist_t;
typedef struct Type* type_t;
typedef struct TypeList* typelist_t;
typedef struct Decl* decl_t;
typedef struct DeclList* decllist_t;
typedef struct ParamList* paramlist_t;
typedef struct Func* func_t;
typedef struct FuncList* funclist_t;
typedef struct Main* main_t;

typedef struct Form* form_t;
typedef struct Juxt* juxt_t;
typedef struct Atom* atom_t;


struct Prog {
  decllist_t decllist;
  funclist_t funclist;
  main_t main;
};

struct ExprList {
  expr_t expr;
  exprlist_t tail;
};

struct Expr {
  enum { CONST, E_identexpr, E_callexpr, E_intexpr, E_charexpr, E_strexpr, E_arrayexpr, E_unaryexpr, E_binaryexpr, E_castexpr } kind;
  union {
    form_t constant;
    str identexpr;
    struct { str ident; exprlist_t args; } callexpr;
    int intexpr;
    char charexpr;
    str strexpr;
    struct { expr_t expr; expr_t index; } arrayexpr;
    struct { op_e op; expr_t expr; } unaryexpr;
    struct { expr_t left; op_e op; expr_t right; } binaryexpr;
    struct { type_t type; expr_t expr; } castexpr;
  };
  type_t type;
};

struct Atom {
  enum { AINT, AFLOAT, ACHAR, ASTRING } kind; // Mainly for the purpose of printing.
  union {
    int intval;
    float floatval;
    char charval;
    str stringval;
  };
  type_t type;
};

struct Juxt {
  // Unary operators
  atom_t ATOM;
  type_t type;
};

struct Form {
  // Binary operators
  enum { FORM_BINARYOP, FORM_CONST } kind;
  union {
    juxt_t atomic;
    struct{ form_t left; op_e op; form_t right; } binaryop;
  };
  type_t type;
};

struct Stmt {
  enum { E_assignstmt, E_ifstmt, E_ifelsestmt, E_whilestmt, E_compoundstmt } kind;
  union {
    struct { expr_t left; expr_t right; } assignstmt;
    struct { expr_t cond; expr_t body; } ifstmt;
    struct { expr_t cond; expr_t ifbranch; stmt_t elsebranch; } ifelsestmt;
    struct { expr_t cond; expr_t body; } whilestmt;
    struct { decllist_t decllist; decllist_t stmtlist; } compoundstmt;
  };
};

struct StmtList {
  stmt_t stmt;
  stmtlist_t tail;
};

struct Type {
  enum { E_primitivetype, E_pointertype, E_arraytype, E_functiontype } kind;
  union {
    typename_e primitivetype;
    type_t pointertype;
    struct { int size; type_t type; } arraytype;
    struct { typelist_t paramtypes; type_t returntype; } functiontype;
  };
};

struct TypeList {
  type_t type;
  typelist_t tail;
};

struct Decl {
  type_t type;
  str ident;
};

struct DeclList {
  decl_t decl;
  decllist_t tail;
};

struct ParamList {
  str ident;
  paramlist_t tail;
};

struct Func {
  str ident;
  paramlist_t paramlist;
  type_t type;
  decllist_t decllist;
  stmtlist_t stmtlist;
  expr_t returnexpr;
};

struct FuncList {
  func_t func;
  funclist_t tail;
};

struct Main {
  decllist_t decllist;
  stmtlist_t stmtlist;
  expr_t returnexpr;
};

cscript_ast_t CreateProg( decllist_t decllist, funclist_t funclist, main_t main );
main_t CreateMain( decllist_t decllist, stmtlist_t stmtlist, expr_t returnexpr );


// Printing functions
void PrintIndent( int depth );
void PrintString( str s, int depth );
void PrintInt( int value, int depth );
void PrintChar( char value, int depth );
void PrintProg( cscript_ast_t prog_node, int indent );

atom_t CreateIntAtom( int intexpr );
atom_t CreateFloatAtom( float floatexpr );
juxt_t CreateJuxt( atom_t atom );


form_t CreateFormFromJuxt( juxt_t juxt );
form_t CreateFormFromOp( form_t left, op_e op, form_t right );
expr_t CreateExprConst( form_t constant );
void PrintIntAtom( atom_t aint, int indent );
void PrintExpr( expr_t expr_node, int indent );
void PrintConst( form_t form_node, int indent );

/*void print_typename( Type type, int indent );
void print_op( E_op op, int indent );
void print_exprlist( T_exprlist exprlist_node, int indent );
void print_identexpr( T_expr identexpr_node, int indent );
void print_callexpr( T_expr callexpr_node, int indent );
void print_intexpr( T_expr intexpr_node, int indent );
void print_charexpr( T_expr charexpr_node, int indent );
void print_strexpr( T_expr strexpr_node, int indent );
void print_arrayexpr( T_expr arrayexpr_node, int indent );
void print_unaryexpr( T_expr unaryexpr_node, int indent );
void print_binaryexpr( T_expr binaryexpr_node, int indent );
void print_castexpr( T_expr castexpr_node, int indent );
void print_assignstmt( T_stmt assignstmt_node, int indent );
void print_ifstmt( T_stmt ifstmt_node, int indent );
void print_ifelsestmt( T_stmt ifelsestmt_node, int indent );
void print_whilestmt( T_stmt whilestmt_node, int indent );
void print_compoundstmt( T_stmt compoundstmt_node, int indent );
void print_stmt( T_stmt stmt_node, int indent );
void print_stmtlist( T_stmtlist stmtlist_node, int indent );
void print_primitivetype( T_type primitivetype_node, int indent );
void print_pointertype( T_type pointertype_node, int indent );
void print_arraytype( T_type arraytype_node, int indent );
void print_functiontype( T_type functiontype_node, int indent );
void print_type( T_type type_node, int indent );
void print_typelist( T_typelist typelist_node, int indent );
void print_decl( T_decl decl_node, int indent );
void print_decllist( T_decllist decllist_node, int indent );
void print_paramlist( T_paramlist paramlist_node, int indent );
void print_func( T_func func_node, int indent );
void print_funclist( T_funclist funclist_node, int indent );*/
void PrintMain( main_t main_node, int indent );

#endif /* PUNCH_CSCRIPT_AST_H */