%option noyywrap nodefault yylineno case-insensitive
%option debug

%{
int fileno(FILE *stream);

#include "parser.h"
#include "subtok.h"
#include <string.h>


void yyerror(const char *s, ...);
%}


letter [a-zA-Z_]
digit [0-9]
space [ \t\n]+

symbol {letter}({letter}|{digit})*
int [+-]?{digit}+
float [+-]?({digit}+[\.]?{digit}*)
str (\".*\")|(\'.*\')

%%

SELECT { return SELECT; }
UPDATE { return UPDATE; }
DELETE { return DELETE; }
INSERT { return INSERT; }
CREATE { return CREATE; }
DROP { return DROP; }
TABLE { return TABLE; }
FROM { return FROM; }
CROSS_JOIN { return CROSS_JOIN; }
WHERE { return WHERE; }
AS { return AS; }
SET { return SET; }
INTO { return INTO; }
VALUES { return VALUES; }

STR { yylval.subtok = DATA_STR; return TYPE; }
BOOL { yylval.subtok = DATA_BOOL; return TYPE; }
DOUBLE { yylval.subtok = DATA_DOUBLE; return TYPE; }
INT32 { yylval.subtok = DATA_INT32; return TYPE; }

[.,*();] { return yytext[0]; }

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

true { yylval.ival = 1; return BOOLEAN; }
false { yylval.ival = 0; return BOOLEAN; }

{int} { yylval.ival = atoi(yytext); return INTNUM; }
{float} { yylval.fval = atof(yytext); return FLOATNUM; }
{symbol} { yylval.sval = yytext; return NAME; }
{str} { yylval.sval = strdup(yytext+1); yylval.sval[yyleng-2] = 0; return STRING; }

{space} { }
<<EOF>> { fprintf(stdout, "%d", yylineno); yyterminate(); }

. { yyerror("Lexer: undefined character read: '%s'", yytext); }

%%
