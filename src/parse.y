%{
#include "subtok.h"
#include "lexer.h"
#include <ast.hpp>
#include <stdio.h>
#include <stdarg.h>
%}

%define parse.error verbose

%union {
  int ival;
  double fval;
  char *sval;
  int subtok; // subtokens 
}

%{
void yyerror(const char *s, ...);
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
%token ';'
%token FROM
%token JOIN
%token WHERE
%token AS
%token SET
%token INTO
%token VALUES
%token <subtok> TYPE

%token ','
%token '.'
%token '('
%token ')'
%token '*'

/* operators with precedence */
%left OR
%left AND
%nonassoc IN
%left NOT
%left <subtok> COMPARE /* = != > < <= >= */

%start stmt_list

%%

stmt_list: 
  stmt ';' YYEOF {}
| stmt_list stmt ';' YYEOF {}
;

stmt: 
  select_stmt {}
| update_stmt {}
| delete_stmt {}
| insert_stmt {}
| drop_stmt   {}
| create_stmt {}

select_stmt:
  SELECT column_list FROM source WHERE condition {}
| SELECT column_list FROM source {}

update_stmt:
  UPDATE table_name SET assign_column_list WHERE condition {}
| UPDATE table_name SET assign_column_list {}

delete_stmt:
  DELETE FROM table_name WHERE condition {}

insert_stmt:
  INSERT INTO table_name '(' column_list ')' VALUES '(' value_list ')' {}
| INSERT INTO table_name VALUES '(' value_list ')' {}

drop_stmt:
  DROP TABLE table_name {}

create_stmt:
  CREATE TABLE table_name '(' create_loc_list ')' {}

create_loc_list:
  create_definition {}
| create_loc_list ',' create_definition {}

create_definition:
  column_name TYPE {}

assign_column_list:
  column_name COMPARE value 
  { if ($2 != COMPARE_EQ) { yyerror("bad update assignment to '%s', $1"); YYERROR; }}
| assign_column_list ',' column_name COMPARE value
  { if ($4 != COMPARE_EQ) { yyerror("bad update assignment to '%s', $1"); YYERROR; }}

column_list:
  '*' {}
| column_list_h {}

column_list_h:
  column {}
| column_list_h ',' column {}

column:
  table_name '.' column_name {}
| column_name {}

column_name:
  NAME {}

source:
  table_name {}
| '(' select_stmt ')' AS table_name {}

table_name:
  NAME {}

condition:
  condition AND condition {}
| condition OR condition {}
| '(' condition ')' {}
| statement {}

statement:
  BOOLEAN
| column COMPARE column {}
| column COMPARE value {}
| value COMPARE column {}

value_list:
  value {}
| value_list ',' value {}

value:
  INTNUM {}
| BOOLEAN {}
| FLOATNUM {}
| STRING {}

%%

void yyerror(const char *s, ...){
  extern int yylineno;
  va_list lst;
  va_start(lst, s);
  fprintf(stderr, "%d: error: ", yylineno);
  vfprintf(stderr, s, lst);
  fprintf(stderr, "\n");
}
