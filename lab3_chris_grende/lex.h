#pragma once
//**************************************
// lex.h
//
// Defines global symbols used the the flex generated lexer 
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

int yylex();

// Token values returned by the scanner.
//
// NOTE:
// - Single-character tokens (e.g., '+', '(', ';', etc.) are returned as their
//   ASCII character codes.
// - Multi-character tokens use the enum values below.
enum Token
{
    TOK_NUM = 256,
    TOK_IDENTIFIER,
    TOK_END
};

extern char *yytext;
extern int yylineno;
extern int yynerrs;
extern FILE *yyin;

