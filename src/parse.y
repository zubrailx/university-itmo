%{
#include "lexer.hpp"
#include <cstdio>
#include <cstdarg>

#include <iostream>

%}

%code requires {
#include <ast.hpp>
}

%define parse.error verbose

%union {
  int ival;
  double fval;
  char *sval;
  CompareType ctype;
  DataType dtype;

  Ast* nterm;
}

%{
Ast * parsed_result;

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

%nterm <nterm> value value_list column column_list column_list_h table_source table_ref condition join_table table_subquery statement assign_column_list create_definition create_loc_list
%nterm <nterm> stmt stmt_list select_stmt update_stmt delete_stmt insert_stmt drop_stmt create_stmt

%nterm <sval> table_name column_name opt_as_alias as_alias

%start stmt_list

%%

stmt_list: 
  stmt ';' YYEOF { parsed_result = $1; }
| stmt_list stmt ';' YYEOF { parsed_result = $1; }
;

stmt: 
  select_stmt {}
| update_stmt {}
| delete_stmt {}
| insert_stmt {}
| drop_stmt   {}
| create_stmt {}

  /* SELECT */
select_stmt:
  SELECT column_list FROM table_ref WHERE condition {
    $$ = new AstSelect((AstColumnList*)$2, $4, (AstStatement*)$6); 
  }
| SELECT column_list FROM table_ref {$$ = new AstSelect((AstColumnList*)$2, $4, nullptr); }
;

table_ref:
  table_source { $$ = $1; }
| join_table { $$ = $1; }
;

table_source:
  table_name opt_as_alias { $$ = new AstTable(std::string($1), $2); free($1); free($2); }
| table_subquery as_alias { ((AstSubquery*)$1)->setAlias($2); free($2); }
;

table_subquery:
  '(' select_stmt ')' { $$ = new AstSubquery((AstSelect*)$2); }
;

// left ast
join_table:
  table_ref CROSS_JOIN table_source{ $$ = new AstJoin($1, JoinType::CROSS_JOIN, $3); }
;

opt_as_alias:
  as_alias  {}
| /* nil */ { $$ = nullptr; }
;

as_alias:
  AS table_name { $$ = $2; }
;

  /* UPDATE */
update_stmt:
  UPDATE table_name SET assign_column_list WHERE condition { 
    $$ = new AstUpdate($2, (AstList<AstColumnValue>*)$4, (AstStatement*)$6); 
    free($2);
  }
| UPDATE table_name SET assign_column_list { 
    $$ = new AstUpdate($2, (AstList<AstColumnValue>*)$4, nullptr); 
    free($2);
  }
;

  /* DELETE */
delete_stmt:
  DELETE FROM table_name WHERE condition {
    $$ = new AstDelete($3, (AstStatement*)$5);
    free($3);
  }
|  DELETE FROM table_name {
    $$ = new AstDelete($3, nullptr);
    free($3);
  }
;

  /* INSERT */
insert_stmt:
  INSERT INTO table_name '(' column_list ')' VALUES '(' value_list ')' {
    $$ = new AstInsert($3, (AstColumnList*)$5, (AstList<AstValue>*)$9);
    free($3);
  }
| INSERT INTO table_name VALUES '(' value_list ')' {
    $$ = new AstInsert($3, new AstColumnList(), (AstList<AstValue>*)$6);
    free($3);
  }
;

  /* DROP */
drop_stmt:
  DROP TABLE table_name { $$ = new AstDrop($3); free($3); }
;

  /* CREATE */
create_stmt:
  CREATE TABLE table_name '(' create_loc_list ')' { 
    $$ = new AstCreate($3, (AstList<AstColumnType>*)$5);
    free($3);
  }
;

create_loc_list:
  create_definition { 
    $$ = new AstList<AstColumnType>((AstColumnType*)$1, AstType::COLUMN_TYPE_LIST); 
  }
| create_loc_list ',' create_definition {
    auto collist = (AstList<AstColumnType> *)$1;
    $$ = new AstList<AstColumnType>(collist, (AstColumnType*)$3);
    delete $1;
  }
;

create_definition:
  column_name DTYPE { $$ = new AstColumnType(new AstColumn($1), $2); free($1); }
;


assign_column_list:
  column_name COMPARE value 
  { if ($2 != CompareType::EQ) { 
    yyerror("bad update assignment to '%s', $1"); YYERROR; 
    }
    auto colval = new AstColumnValue($1, (AstValue*)$3);
    $$ = new AstList<AstColumnValue>(colval, AstType::COLUMN_VALUE_LIST);
    free($1);
  }
| assign_column_list ',' column_name COMPARE value
  { if ($4 != CompareType::EQ) { 
    yyerror("bad update assignment to '%s', $1"); YYERROR; 
    }
    auto colval = new AstColumnValue($3, (AstValue*)$5);
    auto collist = (AstList<AstColumnValue>*)$1;
    $$ = new AstList<AstColumnValue>(collist, colval);
    delete $1;
  }
;

column_list:
  '*' { $$ = new AstColumnList(); }
| column_list_h { $$ = $1; }
;

column_list_h:
  column { $$ = new AstColumnList((AstColumn*)$1); }
| column_list_h ',' column {
  $$ = new AstColumnList((AstColumnList*)$1, (AstColumn*)$3);
  delete $1;
}
;

column:
  table_name '.' column_name { $$ = new AstColumn($1, $3); free($1); free($3); }
| column_name { $$ = new AstColumn($1); free($1); }
;

column_name:
  NAME { $$ = $1; }
;

table_name:
  NAME { $$ = $1; }
;

condition:
  condition AND condition { $$ = new AstStatement($1, CompareType::AND, $3); }
| condition OR condition { $$ = new AstStatement($1, CompareType::OR, $3); }
| '(' condition ')' { $$ = $2; }
| statement {}
;

statement:
  BOOLEAN { $$ = new AstStatement($1); }
| column COMPARE column { $$ = new AstStatement($1, $2, $3); }
| column COMPARE value { $$ = new AstStatement($1, $2, $3); }
| value COMPARE column { $$ = new AstStatement($1, $2, $3); }
;

value_list:
  value { $$ = new AstList((AstValue*)$1, AstType::VARIABLE_LIST); }
| value_list ',' value 
  { $$ = new AstList((AstList<AstValue>*)$1, (AstValue*)$3); delete $1; }
;

value:
  INTNUM { $$ = new AstValue((int32_t)$1, DataType::INT32); }
| BOOLEAN { $$ = new AstValue((bool)$1, DataType::BOOL); }
| FLOATNUM { $$ = new AstValue((double)$1, DataType::DOUBLE); }
| STRING { $$ = new AstValue(std::string($1), DataType::STR); free($1); }
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
