%{
/**********************************************
 * calc.l - CST320 Lab 3 scanner
 **********************************************/
%}

%option noyywrap
%option yylineno

%{
#include "lex.h"
%}

DIGIT      [0-9]
ID_START   [A-Za-z_]
ID_CONT    [A-Za-z0-9_]

%%

[ \t\r\n]+            ;

"//"[^\n]*            ;

"end"                 { return TOK_END; }

{DIGIT}+              { return TOK_NUM; }

{ID_START}{ID_CONT}*  { return TOK_IDENTIFIER; }

"+"                   { return '+'; }
"-"                   { return '-'; }
"*"                   { return '*'; }
"/"                   { return '/'; }
"("                   { return '('; }
")"                   { return ')'; }
"["                   { return '['; }
"]"                   { return ']'; }
"{"                   { return '{'; }
"}"                   { return '}'; }
"="                   { return '='; }
";"                   { return ';'; }

.                     { return yytext[0]; }

<<EOF>>               { return 0; }

%%
