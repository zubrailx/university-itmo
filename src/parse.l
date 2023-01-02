%option noyywrap nodefault yylineno case-insensitive
%option debug

%{
#include "parser.h"
#include "subtok.h"
#include <stdio.h>

void yyerror(const char *s, ...);
%}

int [+-]?[0-9]+
float [+-]?([0-9]+[.]?[0-9]*)
space [ \t \n]+

%%

  /* keywords */
SELECT { return SELECT; }
UPDATE { return UPDATE; }
DELETE { return DELETE; }
INSERT { return INSERT; }
CREATE { return CREATE; }
DROP { return DROP; }
TABLE { return TABLE; }
";" { return EOEXPR; }
FROM { return FROM; }
JOIN { return JOIN; }
WHERE { return WHERE; }

  /* operators */
OR { return OR; }
AND { return AND; }
IN { return IN; }
NOT { return NOT; }

"=" { yylval.subtok = COMPARE_EQ; return COMPARE; }
"<>" | 
"!=" { yylval.subtok = COMPARE_NEQ; return COMPARE; }
">" { yylval.subtok = COMPARE_GR; return COMPARE; }
"<" { yylval.subtok = COMPARE_LO; return COMPARE; }
"<=" { yylval.subtok = COMPARE_LEQ; return COMPARE; }
">=" { yylval.subtok = COMPARE_GEQ; return COMPARE; }

[.,] { return yytext[0]; }

true { yylval.ival = 1; return BOOLEAN; }
false { yylval.ival = 0; return BOOLEAN; }

{int} { yylval.ival = atoi(yytext); return INTNUM; }
{float} { yylval.fval = atof(yytext); return FLOATNUM; }
{space} {}
. { yyerror("Lexer: undefined character read: '%s'", yytext); }

<<EOF>> { fprintf(stdout, "%d", yylineno); yyterminate(); }

%%
