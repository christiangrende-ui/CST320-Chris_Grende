//**************************************
// langparse.cpp
//
// Hand-written recursive-descent parser for Lab 4.
// Builds an AST for tests 1-4.
//

#include <iostream>
#include <memory>
#include <string>

#include "lex.h"
#include "langparse.h"
#include "astnodes.h"
#include "cSymbolTable.h"

extern cSymbolTable g_symbolTable;

cAstNode* yyast_root = nullptr;

namespace
{
    int g_lookahead = 0;

    void Next() { g_lookahead = yylex(); }

    bool Match(int tok)
    {
        if (g_lookahead == tok)
        {
            Next();
            return true;
        }
        return false;
    }

    void ParseError(const std::string& msg)
    {
        std::cerr << "ERROR: " << msg << " at symbol " << (yytext ? yytext : "")
                  << " on line " << yylineno << "\n";
        yynerrs++;
    }

    void Expect(int tok, const std::string& what)
    {
        if (!Match(tok))
        {
            ParseError("expected " + what);
        }
    }

    // Forward declarations
    cBlockNode* ParseBlock();
    cDeclsNode* ParseDecls();
    cDeclNode* ParseDecl();
    cStmtsNode* ParseStmts();
    cStmtNode* ParseStmt();
    cExprNode* ParseExpr();
    cExprNode* ParseAddit();
    cExprNode* ParseTerm();
    cExprNode* ParseFact();

    // Parse: program -> PROGRAM block
    cProgramNode* ParseProgram()
    {
        Expect(PROGRAM, "'program'");
        cBlockNode* block = ParseBlock();
        return new cProgramNode(block);
    }

    // Parse: block -> '{' decls? stmts? '}'
    cBlockNode* ParseBlock()
    {
        Expect('{', "'{'");
        g_symbolTable.IncreaseScope();

        cDeclsNode* decls = nullptr;
        cStmtsNode* stmts = nullptr;

        // decls start with TYPE_ID
        if (g_lookahead == TYPE_ID)
        {
            decls = ParseDecls();
        }

        // stmts start with PRINT or '{'
        if (g_lookahead == PRINT || g_lookahead == '{')
        {
            stmts = ParseStmts();
        }

        Expect('}', "'}'");
        g_symbolTable.DecreaseScope();

        return new cBlockNode(decls, stmts);
    }

    // decls -> decl decls | decl
    cDeclsNode* ParseDecls()
    {
        cDeclNode* first = ParseDecl();
        auto* decls = new cDeclsNode(first);

        while (g_lookahead == TYPE_ID)
        {
            decls->Insert(ParseDecl());
        }
        return decls;
    }

    // decl -> TYPE_ID IDENTIFIER ';'
    cDeclNode* ParseDecl()
    {
        if (g_lookahead != TYPE_ID)
        {
            ParseError("declaration must start with a type");
            return nullptr;
        }
        cSymbol* typeSym = yylval.symbol;
        Next();

        if (g_lookahead != IDENTIFIER)
        {
            ParseError("expected identifier in declaration");
            // try to recover
            while (g_lookahead != ';' && g_lookahead != 0) Next();
            Match(';');
            return nullptr;
        }
        cSymbol* nameSym = yylval.symbol;
        Next();

        Expect(';', "';'");
        return new cVarDeclNode(typeSym, nameSym);
    }

    // stmts -> stmt stmts | stmt
    cStmtsNode* ParseStmts()
    {
        cStmtNode* first = ParseStmt();
        auto* stmts = new cStmtsNode(first);

        while (g_lookahead == PRINT || g_lookahead == '{')
        {
            stmts->Insert(ParseStmt());
        }
        return stmts;
    }

    // stmt -> print '(' expr ')' ';' | block
    cStmtNode* ParseStmt()
    {
        if (g_lookahead == PRINT)
        {
            Next();
            Expect('(', "'('");
            cExprNode* e = ParseExpr();
            Expect(')', "')'");
            Expect(';', "';'");
            return new cPrintNode(e);
        }

        if (g_lookahead == '{')
        {
            return ParseBlock();
        }

        ParseError("unexpected token in statement");
        // recover: skip to ';' or '}'
        while (g_lookahead != ';' && g_lookahead != '}' && g_lookahead != 0) Next();
        if (g_lookahead == ';') Next();
        return nullptr;
    }

    // expr -> addit ( (== | !=) addit )*
    cExprNode* ParseExpr()
    {
        cExprNode* left = ParseAddit();
        while (g_lookahead == EQUALS || g_lookahead == NOT_EQUALS)
        {
            int op = g_lookahead;
            Next();
            cExprNode* right = ParseAddit();
            left = new cBinaryExprNode(left, new cOpNode(op), right);
        }
        return left;
    }

    // addit -> term ((+|-) term)*
    cExprNode* ParseAddit()
    {
        cExprNode* left = ParseTerm();
        while (g_lookahead == '+' || g_lookahead == '-')
        {
            int op = g_lookahead;
            Next();
            cExprNode* right = ParseTerm();
            left = new cBinaryExprNode(left, new cOpNode(op), right);
        }
        return left;
    }

    // term -> fact ((*|/|%) fact)*
    cExprNode* ParseTerm()
    {
        cExprNode* left = ParseFact();
        while (g_lookahead == '*' || g_lookahead == '/' || g_lookahead == '%')
        {
            int op = g_lookahead;
            Next();
            cExprNode* right = ParseFact();
            left = new cBinaryExprNode(left, new cOpNode(op), right);
        }
        return left;
    }

    // fact -> '(' expr ')' | INT_VAL | FLOAT_VAL | IDENTIFIER
    cExprNode* ParseFact()
    {
        if (g_lookahead == '(')
        {
            Next();
            cExprNode* e = ParseExpr();
            Expect(')', "')'");
            return e;
        }
        if (g_lookahead == INT_VAL)
        {
            int v = yylval.int_val;
            Next();
            return new cIntExprNode(v);
        }
        if (g_lookahead == FLOAT_VAL)
        {
            float v = yylval.float_val;
            Next();
            return new cFloatExprNode(v);
        }
        if (g_lookahead == IDENTIFIER)
        {
            cSymbol* s = yylval.symbol;
            Next();
            return new cVarExprNode(s);
        }

        ParseError("unexpected token in expression");
        Next();
        return new cIntExprNode(0);
    }
}

int yyparse()
{
    yyast_root = nullptr;
    yynerrs = 0;

    Next();
    cProgramNode* root = ParseProgram();

    // After program, ensure EOF
    if (g_lookahead != 0)
    {
        ParseError("junk at end of program");
    }

    yyast_root = root;
    return (yynerrs == 0) ? 0 : 1;
}

// Semantic error hook used by AST nodes / later labs.
void SemanticParseError(std::string error)
{
    std::cerr << "ERROR: " << error << " near line " << yylineno << "\n";
    yynerrs++;
}
