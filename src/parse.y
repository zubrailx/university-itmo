%{

#include "subtok.h"
#include "lexer.h"
#include <ast.hpp>
#include <stdio.h>
#include <stdarg.h>

%}

%union {
  int ival;
  double fval;
  char *sval;
  int subtok; // subtokens 
}

%{
void yyerror(char *s, ...);
%}

/* parameters */
%token <sval> NAME
%token <sval> STRING
%token <ival> INTNUM
%token <ival> BOOLEAN
%token <fval> FLOATNUM

/* reserved keywords */
%token SELECT UPDATE DELETE INSERT
%token CREATE DROP
%token TABLE
%token EOEXPR
%token FROM
%token JOIN
%token WHERE
%token AS

%token COMMA ","
%token DOT "."
%token BO "("
%token BC ")"
%token ALL "*"

/* operators with precedence */
%left OR
%left AND
%nonassoc IN
%left NOT
%left <subtok> COMPARE /* = != > < <= >= */

%start stmt_list

%%

stmt_list: 
  stmt EOEXPR YYEOF
| stmt_list stmt EOEXPR YYEOF
;

stmt: 
  select_stmt { printf("select\n"); }

  /* SELECT STATEMENT */
select_stmt:
  SELECT column_list FROM source WHERE condition
| SELECT column_list FROM source

  /* UPDATE STATEMENT */
/* stmt:  */
/*   update_stmt { printf("update\n"); } */

  /* DELETE STATEMENT */
/* stmt: */
/*   delete_stmt { printf("delete\n"); } */

  /* INSERT STATEMENT */
/* stmt: */
/*   insert_stmt { printf("insert\n"); } */

  /* DROP STATEMENT */
/* stmt: */
/*   drop_stmt { printf("drop\n"); } */

  /* CREATE STATEMENT */
/* stmt: */
/*   create_stmt { printf("create\n"); } */

  /* OTHER DEFINITIONS */
column_list:
  ALL
| column_list_h

column_list_h:
  column
| column_list_h COMMA column

// table_name.column_name
column:
  table DOT NAME

// table_name 
// ( select ... ) as table_name
source:
  table
| BO select_stmt BC AS table

// "table_name"
table:
  NAME

condition:
  condition AND condition
| condition OR condition
| BO condition BC
| statement

// column BINOP value
// column BINOP column
statement:
  column COMPARE column
| column COMPARE value
| value COMPARE column

value:
  INTNUM
| BOOLEAN
| FLOATNUM
| STRING

%%

void yyerror(char *s, ...){
  extern int yylineno;
  va_list lst;
  va_start(lst, s);
  fprintf(stderr, "%d: error: ", yylineno);
  vfprintf(stderr, s, lst);
  fprintf(stderr, "\n");
}
