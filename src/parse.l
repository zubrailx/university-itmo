%option noyywrap nodefault yylineno case-insensitive noinput nounput 
/* %option reentrant bison-bridge */
/* %option debug */

%{
int fileno(FILE *stream);

#include "parser.hpp"
#include <ast.hpp>
#include <cstring>

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

STR { yylval.dtype = DataType::STR; return DTYPE; }
BOOL { yylval.dtype = DataType::BOOL; return DTYPE; }
DOUBLE { yylval.dtype = DataType::DOUBLE; return DTYPE; }
INT32 { yylval.dtype = DataType::INT32; return DTYPE; }

[.,*();] { return yytext[0]; }

OR { yylval.optype = OperationType::OR; return COMPARE; }
AND { yylval.optype = OperationType::AND; return COMPARE; }
NOT { yylval.optype = OperationType::NOT; return COMPARE; }
"=" { yylval.optype = OperationType::EQ; return COMPARE; }
"<>" | 
"!=" { yylval.optype = OperationType::NEQ; return COMPARE; }
">" { yylval.optype = OperationType::GR; return COMPARE; }
"<" { yylval.optype = OperationType::LO; return COMPARE; }
"<=" { yylval.optype = OperationType::LEQ; return COMPARE; }
">=" { yylval.optype = OperationType::GEQ; return COMPARE; }

true { yylval.ival = 1; return BOOLEAN; }
false { yylval.ival = 0; return BOOLEAN; }
{int} { yylval.ival = atoi(yytext); return INTNUM; }
{float} { yylval.fval = atof(yytext); return FLOATNUM; }
{symbol} { 
  yylval.sval = strdup(yytext);
  return NAME;
}
{str} { 
  yylval.sval = strdup(yytext+1);
  yylval.sval[yyleng-2] = 0;
  return STRING;
}

{space} { }

. { yyerror("Lexer: undefined character read: '%s'", yytext); }

%%
