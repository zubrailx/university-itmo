%{
#include <string>
#include "../expression/expression.h"
#include "expression.tab.h"
%}

%option outfile="lex.c" header-file="lex.h"


white [ \t]+
digit [0-9]
letter [A-Z]
chars '
symbol {letter}({letter}|{digit}|{chars})*

%%
{white} // do nothing
{symbol} {
    yylval.name=new std::string(yytext);
    return NAME;
}
"&" return AND;
"|" return OR;
"->" return IMP;
"!" return NEG;
"(" return LEFT;
")" return RIGHT;
"\n" return END;
%%
