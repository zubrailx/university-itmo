%{
#include "lexer.hpp"
#include <cstdio>
#include <cstdarg>
%}

%code requires {
#include "subtok.hpp"
}

%define parse.error verbose

%union {
  int ival;
  double fval;
  char *sval;
  CompareType ctype;
  DataType dtype;
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
%token CROSS_JOIN
%token <dtype> DTYPE;

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
%left <ctype> COMPARE /* = != > < <= >= */

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
;

  /* SELECT */
select_stmt:
  SELECT column_list FROM table_ref WHERE condition {}
| SELECT column_list FROM table_ref {}
;

table_ref:
  table_source {}
| join_table {}
;

table_source:
  table_name opt_as_alias {}
| table_subquery as_alias {}
;

table_subquery:
  '(' select_stmt ')' {}
;

// left ast
join_table:
  table_ref CROSS_JOIN table_source{}
;

opt_as_alias:
  as_alias  {}
| /* nil */ {}
;

as_alias:
  AS table_name
;

  /* UPDATE */
update_stmt:
  UPDATE table_name SET assign_column_list WHERE condition {}
| UPDATE table_name SET assign_column_list {}
;

  /* DELETE */
delete_stmt:
  DELETE FROM table_name WHERE condition {}
;

  /* INSERT */
insert_stmt:
  INSERT INTO table_name '(' column_list ')' VALUES '(' value_list ')' {}
| INSERT INTO table_name VALUES '(' value_list ')' {}
;

  /* DROP */
drop_stmt:
  DROP TABLE table_name {}
;

  /* CREATE */
create_stmt:
  CREATE TABLE table_name '(' create_loc_list ')' {}
;

create_loc_list:
  create_definition {}
| create_loc_list ',' create_definition {}
;

create_definition:
  column_name DTYPE {}
;


assign_column_list:
  column_name COMPARE value 
  { if ($2 != CompareType::EQ) { yyerror("bad update assignment to '%s', $1"); YYERROR; }}
| assign_column_list ',' column_name COMPARE value
  { if ($4 != CompareType::EQ) { yyerror("bad update assignment to '%s', $1"); YYERROR; }}
;

column_list:
  '*' {}
| column_list_h {}
;

column_list_h:
  column {}
| column_list_h ',' column {}
;

column:
  table_name '.' column_name {}
| column_name {}
;

column_name:
  NAME {}
;

table_name:
  NAME {}
;

condition:
  condition AND condition {}
| condition OR condition {}
| '(' condition ')' {}
| statement {}
;

statement:
  BOOLEAN
| column COMPARE column {}
| column COMPARE value {}
| value COMPARE column {}
;

value_list:
  value {}
| value_list ',' value {}
;

value:
  INTNUM {}
| BOOLEAN {}
| FLOATNUM {}
| STRING {}
;

%%

void yyerror(const char *s, ...){
  extern int yylineno;
  va_list lst;
  va_start(lst, s);
  fprintf(stderr, "%d: error: ", yylineno);
  vfprintf(stderr, s, lst);
  fprintf(stderr, "\n");
}
