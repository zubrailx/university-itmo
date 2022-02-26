%{
#include <string>
#include <vector>
#include "../expression.h"
#include "../util.h"


int yylex(void);
void yyerror(const char *);


expression *result = NULL;
std::vector<variable *> vector_var;

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
      NAME { 
        variable *v = new variable(*$1);
        variable *rez = vector_find_variable(&vector_var, v);
        if (rez == NULL) {
            vector_var.push_back(v);
            $$=v;
        } else {
            $$=rez;
        }
    }
    | LEFT Expression RIGHT     { $$=$2; }
    | Expression IMP Expression { $$=new implication($1, $3); }
    | Expression OR Expression  { $$=new disjunction($1, $3); }
    | Expression AND Expression { $$=new conjunction($1, $3); }
    | NEG Expression            { $$=new negation($2); }
;
%%
