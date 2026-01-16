#include "Node.hpp"
#include "SymbolTable.hpp"
#include <iostream>
#include <memory>
#include <string>

#include <iostream>
#include <cstdio>
#include "scanner.hpp"

int main(int argc, char* argv[])
{
    std::cout << "CST320 Lab 2 Scanner Running\n";

    if (argc > 1)
    {
        yyin = fopen(argv[1], "r");
        if (!yyin)
        {
            std::cerr << "Failed to open input file.\n";
            return 1;
        }
    }

    int token;
    while ((token = yylex()) != 0)
    {
        std::cout << "Token: " << token << std::endl;
    }

    return 0;
}

