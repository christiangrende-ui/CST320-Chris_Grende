#pragma once
//**************************************
// langparse.h
//
// Token definitions + semantic value for the hand-written parser.
//

#include <string>

#include "cSymbol.h"

// Token values.
// Single-character tokens use their ASCII value.
enum yytokentype
{
    // Bison typically starts at 258 for non-ASCII tokens.
    IDENTIFIER = 258,
    TYPE_ID,
    CHAR_VAL,
    INT_VAL,
    FLOAT_VAL,
    LE,
    GE,
    AND,
    OR,
    EQUALS,
    NOT_EQUALS,
    STRING_LIT,

    PROGRAM,
    PRINT,
    PRINTS,
    WHILE,
    IF,
    ELSE,
    ENDIF,
    STRUCT,
    ARRAY,
    RETURN,

    JUNK_TOKEN
};

// Semantic value (subset of the bison %union used in Lab 4).
struct yylval_t
{
    int         int_val{0};
    float       float_val{0.0f};
    std::string* str_val{nullptr};
    cSymbol*    symbol{nullptr};
};

extern yylval_t yylval;
