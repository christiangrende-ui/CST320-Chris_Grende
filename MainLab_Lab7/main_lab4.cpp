//**************************************
// main for Lab 7 lang compiler (stackl code generation)
//**************************************

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

#include "cSymbolTable.h"
#include "cBaseTypeNode.h"
#include "lex.h"
#include "astnodes.h"
#include "langparse.h"
#include "cSizeOffsetVisitor.h"
#include "cCodeGenVisitor.h"
#include "emit.h"

cSymbolTable g_symbolTable;
long long cSymbol::nextId = 0;

static void InitBuiltInTypes()
{
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
        sym->SetDecl(new cBaseTypeNode(b.name, b.size, b.isFloat));
    }
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: mainlab <input_file> [output_base]\n";
        return 1;
    }

    std::string outBase = (argc >= 3) ? argv[2] : "langout";

    yyin = std::fopen(argv[1], "r");
    if (yyin == nullptr)
    {
        std::cerr << "ERROR: Unable to open file " << argv[1] << "\n";
        return 1;
    }

    cSymbol::nextId = 0;
    InitBuiltInTypes();

    int result = yyparse();
    if (result == 0 && yyast_root != nullptr)
    {
        cSizeOffsetVisitor so;
        so.VisitAllNodes(yyast_root);

        if (!InitOutput(outBase + ".sl"))
        {
            std::fclose(yyin);
            return 1;
        }

        cCodeGenVisitor cg;
        cg.VisitAllNodes(yyast_root);
        FinalizeOutput();
    }
    else if (result != 0)
    {
        std::cerr << "Errors in compile\n";
    }

    std::fclose(yyin);
    return result;
}
