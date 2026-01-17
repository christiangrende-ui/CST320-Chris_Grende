%{
#include "lex.h"
#include "tokens.h"
#include "cSymbolTable.hpp"
#include <string>

extern cSymbolTable g_symbolTable;

static int Process_ID(const char* text);
%}

%option noyywrap
%option noinput
%option nounput
%option yylineno

LETTER      [A-Za-z_]
DIGIT       [0-9]
IDENT       {LETTER}({LETTER}|{DIGIT})*
INTCONST    {DIGIT}+

WS          [ \t\r\n]+
COMMENT     "//".*

%%

{WS}            ;
{COMMENT}       ;

"{"             { g_symbolTable.IncreaseScope(); return '{'; }
"}"             { g_symbolTable.DecreaseScope(); return '}'; }

"+"             { return '+'; }
"-"             { return '-'; }

{INTCONST}      { return INT_VAL; }
{IDENT}         { return Process_ID(yytext); }

.               { return JUNK_TOKEN; }

%%

static int Process_ID(const char* text)
{
    std::string name(text);
    cSymbol* sym = g_symbolTable.LookupLocal(name);
    if (!sym) sym = g_symbolTable.Insert(name);
    yylval.symbol = sym;
    return IDENTIFIER;
}
