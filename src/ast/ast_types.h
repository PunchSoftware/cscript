/* Copyright 2025 Punch Software
 *
 * All types used byt the AST package.
 */

#ifndef PUNCH_AST_TYPES_H
#define PUNCH_AST_TYPES_H

typedef char* str;

typedef struct prog_t* prog_t;
typedef enum {  
    TFLOAT,
    TCHAR,
    TSTRING, // TODO make typical C-string once arrays are done
    TINT,
} typename_t; // Default primitive types
typedef enum { E_op_ref, E_op_deref, 
    TPLUS, TMINUS, TNOT, TSTAR, 
    TSLASH, E_op_mod, E_op_eq, E_op_ne, 
    E_op_lt, E_op_le, E_op_gt, E_op_ge, 
    E_op_and, E_op_or } op_t;
typedef struct expr_t* expr_t;
typedef struct exprList_t* exprList_t;
typedef struct form_t* form_t;
typedef struct juxt_t* juxt_t;
typedef struct atom_t* atom_t;
typedef struct stmt_t* stmt_t;
typedef struct stmtList_t* stmtList_t;
typedef struct Type* type_t;
typedef struct TypeList* typeList_t;
typedef struct Decl* decl_t;
typedef struct DeclList* declList_t;
typedef struct ParamList* paramList_t;
typedef struct Func* func_t;
typedef struct FuncList* funcList_t;
typedef struct Main* main_t;


struct prog_t {
  declList_t decllist;
  funcList_t funclist;
  main_t main;
};

struct exprList_t {
  expr_t expr;
  exprList_t tail;
};

struct expr_t {
  enum { EXPR_CONST, E_identexpr, E_callexpr, E_arrayexpr, E_unaryexpr, E_binaryexpr, E_castexpr } kind;
  union {
    form_t constant;
    str identexpr;
    struct { str ident; exprList_t args; } callexpr;
    struct { expr_t expr; expr_t index; } arrayexpr;
    struct { op_t op; expr_t expr; } unaryexpr;
    struct { expr_t left; op_t op; expr_t right; } binaryexpr;
    struct { type_t type; expr_t expr; } castexpr;
  };
  type_t type;
};

struct form_t {
  // Binary operators
  enum { FORM_BINARYOP, FORM_CONST } kind;
  union {
    juxt_t atomic;
    struct{ form_t left; op_t op; form_t right; } binaryop;
  };
  type_t type;
};

struct juxt_t {
  // Unary operators
  enum { JUXT_CONST, JUXT_NEGATE } kind;
  atom_t ATOM;
  type_t type;
};

struct atom_t {
  // Make use of typename_t
  enum { TIDENT = TINT + 1 } kind; // Mainly for the purpose of printing.
  union {
      int intval;
      float floatval;
      char charval;
      str stringval;
      str ident;
  };
  type_t type;
};

struct stmt_t {
  enum { TASSIGNSTMT, TIFSTMT, TIFELSESTMT, TWHILESTMT, TCOMPOUNDSTMT } kind;
  union {
    struct { expr_t id; expr_t value; } assignStmt;
    struct { expr_t conditon; stmt_t body; } ifStmt;
    struct { expr_t cond; expr_t ifbranch; stmt_t elsebranch; } ifElseStmt;
    struct { expr_t cond; expr_t body; } whileStmt;
    struct { declList_t decllist; declList_t stmtlist; } compoundStmt;
  };
};

struct stmtList_t {
  stmt_t stmt;
  stmtList_t tail;
};

struct Type {
  enum { PrimitiveType, PointerType, ArrayType, FunctionType } kind;
  union {
    typename_t primitivetype;
    type_t pointertype;
    struct { int size; type_t type; } arraytype;
    struct { typeList_t paramtypes; type_t returntype; } functiontype;
  };
};

struct TypeList {
  type_t type;
  typeList_t tail;
};

struct Decl {
  type_t type;
  str ident;
  expr_t value;
};

struct DeclList {
  decl_t decl;
  declList_t tail;
};

struct ParamList {
  str ident;
  paramList_t tail;
};

struct Func {
  str ident;
  paramList_t paramlist;
  type_t type;
  declList_t decllist;
  stmtList_t stmtlist;
  expr_t returnexpr;
};

struct FuncList {
  func_t func;
  funcList_t tail;
};

struct Main {
  declList_t decllist;
  stmtList_t stmtlist;
  expr_t returnexpr;
};

#endif /* PUNCH_AST_TYPES_H */
