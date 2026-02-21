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
#include "cBaseTypeNode.h"
#include "lex.h"
#include "astnodes.h"
#include "langparse.h"

// Global symbol table (required by lexer/parser)
cSymbolTable g_symbolTable;

// Static symbol id counter (must be defined exactly once)
long long cSymbol::nextId = 0;

static void InitBuiltInTypes()
{
    // Lab 5: built-in types inserted into global scope in this exact order.
    struct Builtin { const char* name; int size; bool isFloat; };
    const Builtin builtins[] = {
        {"char",   1, false},
        {"int",    4, false},
        {"float",  4, true },
        {"long",   8, false},
        {"double", 8, true },
    };

    for (const auto& b : builtins)
    {
        cSymbol* sym = g_symbolTable.InsertGlobal(b.name);
        // Each built-in type symbol points at a cBaseTypeNode
        sym->SetDecl(new cBaseTypeNode(b.name, b.size, b.isFloat));
    }
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