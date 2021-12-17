%{
#include <iostream>
#include <string>
#include "../expression.h"


int yylex(void);
void yyerror(const char *);


expression *result = NULL;


%}
%union {
    expression *expr;
    std::string *name;
}

%token<name> NAME
%token<expr> IMP OR AND NEG
%token<expr> LEFT RIGHT
%token<expr> END

// priorities
%right IMP
%left OR
%left AND
%left NEG

%type<expr> Expression
%type<name> Input

%start Input


%%
Input: Expression { result = $1; }
;

Expression:
      NAME                      { $$=new variable(*$1); }
    | LEFT Expression RIGHT     { $$=$2; }
    | Expression IMP Expression { $$=new implication($1, $3); }
    | Expression OR Expression  { $$=new disjunction($1, $3); }
    | Expression AND Expression { $$=new conjunction($1, $3); }
    | NEG Expression            { $$=new negation($2); }
;
%%
