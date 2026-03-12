#pragma once
//**************************************
// langparse.h
//
// Public interface shared by lexer + parser.
// Must define token ids and yylval_t.
//**************************************

#include <cstdio>
#include <string>

#include "cSymbol.h"
#include "cAstNode.h"

// Token values (match what your lexer/parser expect)
enum TokenType
{
    // Keywords
    PROGRAM = 256,
    PRINT,
    PRINTS,
    IF,
    ELSE,
    ENDIF,
    WHILE,
    RETURN,
    STRUCT,
    ARRAY,

    // Identifiers / types
    IDENTIFIER,
    TYPE_ID,

    // Literals
    INT_VAL,
    FLOAT_VAL,
    STRING_LIT,

    // Multi-char operators
    EQUALS,        // ==
    NOT_EQUALS,    // !=
    LE,            // <=
    GE,            // >=
    AND,           // &&
    OR,            // ||

    // Error / junk
    JUNK_TOKEN
};

// yylval "union" payload used by lexer and parser
struct yylval_t
{
    cSymbol* symbol = nullptr;
    int int_val = 0;
    float float_val = 0.0f;
    std::string* str_val = nullptr;
};

// Lexer globals
extern FILE* yyin;
extern yylval_t yylval;

// Parser globals
extern cAstNode* yyast_root;

// API
int yylex();
int yyparse();
void yyerror(const char* msg);