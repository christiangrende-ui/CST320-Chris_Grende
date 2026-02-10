//**************************************
// main_lab4.cpp
//
// Main function for Lab 4 lang compiler (AST output)
//
// Author: Phil Howard
//**************************************

#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "cSymbolTable.h"
#include "lex.h"
#include "astnodes.h"
#include "langparse.h"

// Global symbol table (required by lexer/parser)
cSymbolTable g_symbolTable;

// Static symbol id counter (must be defined exactly once)
long long cSymbol::nextId = 0;

static void InitBuiltInTypes()
{
    // Lab 4 requirement: standard types inserted into the global scope in this exact order.
    g_symbolTable.Insert("char", true);
    g_symbolTable.Insert("int", true);
    g_symbolTable.Insert("float", true);
    g_symbolTable.Insert("long", true);
    g_symbolTable.Insert("double", true);
}

// Usage: mainlab <input_file>
int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: mainlab <input_file>\n";
        return 1;
    }

    yyin = std::fopen(argv[1], "r");
    if (yyin == nullptr)
    {
        std::cerr << "ERROR: Unable to open file " << argv[1] << "\n";
        return 1;
    }

    // Ensure ids start at 1 for built-ins.
    cSymbol::nextId = 0;
    InitBuiltInTypes();

    int result = yyparse();
    if (result == 0 && yyast_root != nullptr)
    {
        std::cout << yyast_root->ToString();
        std::cout.flush();
    }
    else if (result != 0)
    {
        std::cerr << "Errors in compile\n";
    }

    std::fclose(yyin);
    return result;
}