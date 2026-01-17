#pragma once
#include <cstdio>

#include "cSymbol.hpp"

// Flex globals
extern char* yytext;
extern int yylineno;
extern FILE* yyin;

int yylex();

// semantic value (Lab 2)
typedef struct
{
    cSymbol* symbol; // pointer to symbol table entry
} yylval_t;

extern yylval_t yylval;
