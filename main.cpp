#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "lex.h"
#include "tokens.h"
#include "cSymbolTable.hpp"

// Uncomment AFTER your Test1 works, required for Test2 behavior
#define TEST2

// Required globals
cSymbolTable g_symbolTable;
yylval_t yylval;

int main(int argc, char** argv)
{
    // Lab requirement (if required by your doc): print your name first
    std::cout << "Chris Grende\n";

    if (argc > 1)
    {
        yyin = std::fopen(argv[1], "r");
        if (!yyin)
        {
            std::cerr << "Unable to open file " << argv[1] << "\n";
            return 1;
        }
    }

    int token = yylex();
    while (token != 0)
    {
#ifdef TEST2
        // If your lab uses argc>3 to enable test2 printing, keep this:
        bool do_test2 = (argc > 3);

        if (do_test2 && token == IDENTIFIER && yylval.symbol != nullptr)
            std::printf("%d:%s:%lld\n", token, yytext, (long long)yylval.symbol->GetId());
        else
            std::printf("%d:%s\n", token, yytext);
#else
        std::printf("%d:%s\n", token, yytext);
#endif
        token = yylex();
    }

    return 0;
}
