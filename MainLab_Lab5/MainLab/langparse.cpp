//**************************************
// langparse.cpp (Lab 5-ready)
//
// Supports tests 1-11 syntax:
// - var decls, array type decls, struct type decls
// - function prototypes + definitions
// - if/else/endif (else-if works naturally), while
// - return, print, prints
// - assignments to lvalues with [] and . chains
// - function calls as statements/expressions
// - precedence for ==, +/-, */%
//

#include <iostream>
#include <string>
#include <vector>

#include "lex.h"
#include "langparse.h"
#include "astnodes.h"
#include "cSymbolTable.h"

extern cSymbolTable g_symbolTable;

cAstNode* yyast_root = nullptr;

namespace
{
    int g_tok = 0;

    void Next() { g_tok = yylex(); }

    void ParseError(const std::string& msg)
    {
        std::cerr << "ERROR: " << msg << " at symbol " << (yytext ? yytext : "")
                  << " on line " << yylineno << "\n";
        yynerrs++;
    }

    bool Match(int t)
    {
        if (g_tok == t) { Next(); return true; }
        return false;
    }

    void Expect(int t, const std::string& what)
    {
        if (!Match(t))
            ParseError("expected " + what);
    }

    // ---------- Forward decls ----------
    cProgramNode* ParseProgram();
    cBlockNode* ParseBlock();

    cDeclsNode* ParseDecls();
    cDeclNode*  ParseDecl();      // any declaration

    cDeclNode*  ParseStructTypeDecl(); // struct { ... } name;
    cDeclNode*  ParseArrayTypeDecl();  // array TYPE[INT] name;

    cDeclNode*  ParseVarOrFuncDecl();  // TYPE name ... (var, proto, def)

    cStmtsNode* ParseStmts();
    cStmtNode*  ParseStmt();

    cStmtNode*  ParseIfStmt();
    cStmtNode*  ParseWhileStmt();
    cStmtNode*  ParseReturnStmt();
    cStmtNode*  ParsePrintStmt();
    cStmtNode*  ParsePrintsStmt();
    cStmtNode*  ParseAssignOrCallStmt(); // starts with IDENTIFIER

    // expressions
    cExprNode* ParseExpr();      // equality
    cExprNode* ParseEquality();
    cExprNode* ParseAdd();
    cExprNode* ParseMul();
    cExprNode* ParsePostfix();
    cExprNode* ParsePrimary();

    // helpers
    bool IsDeclStart()
    {
        return (g_tok == TYPE_ID || g_tok == STRUCT || g_tok == ARRAY);
    }

    bool IsStmtStart()
    {
        return (g_tok == PRINT || g_tok == PRINTS || g_tok == IF || g_tok == WHILE ||
                g_tok == RETURN || g_tok == '{' || g_tok == IDENTIFIER);
    }

    // ---------- Program ----------
    cProgramNode* ParseProgram()
    {
        Expect(PROGRAM, "'program'");
        cBlockNode* block = ParseBlock();
        return new cProgramNode(block);
    }

    // block -> '{' decls? stmts? '}'
   cBlockNode* ParseBlock()
{
    Expect('{', "'{'");
    g_symbolTable.IncreaseScope();

    cDeclsNode* decls = nullptr;
    cStmtsNode* stmts = nullptr;

    // Allow decls and stmts in any order
    while (g_tok != '}' && g_tok != 0)
    {
        if (IsDeclStart())
        {
            cDeclNode* d = ParseDecl();
            if (d != nullptr)
            {
                if (decls == nullptr) decls = new cDeclsNode(d);
                else decls->Insert(d);
            }
        }
        else if (IsStmtStart())
        {
            cStmtNode* s = ParseStmt();
            if (s != nullptr)
            {
                if (stmts == nullptr) stmts = new cStmtsNode(s);
                else stmts->Insert(s);
            }
        }
        else
        {
            ParseError("unexpected token in block");
            Next(); // recovery
        }
    }

    Expect('}', "'}'");
    g_symbolTable.DecreaseScope();

    return new cBlockNode(decls, stmts);
}

    // decls -> decl*
    cDeclsNode* ParseDecls()
    {
        cDeclsNode* decls = nullptr;

        while (IsDeclStart())
        {
            cDeclNode* d = ParseDecl();
            if (d != nullptr)
            {
                if (decls == nullptr) decls = new cDeclsNode(d);
                else decls->Insert(d);
            }
        }

        return decls;
    }

    // decl -> struct_type_decl | array_type_decl | var_or_func_decl
    cDeclNode* ParseDecl()
    {
        if (g_tok == STRUCT) return ParseStructTypeDecl();
        if (g_tok == ARRAY)  return ParseArrayTypeDecl();
        if (g_tok == TYPE_ID) return ParseVarOrFuncDecl();

        ParseError("unexpected token in declaration");
        Next();
        return nullptr;
    }

   // struct { (var_decl)* } IDENTIFIER ;
//
// Fields are parsed in their own temporary scope so their names
// do NOT collide with variables in the surrounding scope (test7).
    cDeclNode* ParseStructTypeDecl()
    {
        Expect(STRUCT, "'struct'");
        Expect('{', "'{'");

        // isolate field names from outer scope
        g_symbolTable.IncreaseScope();

        cDeclsNode* fields = nullptr;

        while (g_tok == TYPE_ID)
        {
            // field decl: TYPE_ID IDENTIFIER ;
            cSymbol* fieldType = yylval.symbol;
            Next();

            if (g_tok != IDENTIFIER)
            {
                ParseError("expected field name");
                while (g_tok != ';' && g_tok != '}' && g_tok != 0) Next();
                Match(';');
                continue;
            }

            cSymbol* fieldName = yylval.symbol;
            Next();

            Expect(';', "';'");
            cDeclNode* fieldDecl = new cVarDeclNode(fieldType, fieldName);

            if (fields == nullptr) fields = new cDeclsNode(fieldDecl);
            else fields->Insert(fieldDecl);
        }

        // end isolated field scope
        g_symbolTable.DecreaseScope();

        Expect('}', "'}'");

        // struct type name
        if (g_tok != IDENTIFIER)
        {
            ParseError("expected struct type name");
            return nullptr;
        }

        cSymbol* typeName = yylval.symbol;
        Next();
        Expect(';', "';'");

        // Create type decl node + mark symbol as a TYPE
        cStructDeclNode* decl = new cStructDeclNode(typeName, fields);
        typeName->SetDecl(decl);

        return decl;
    }

    // array TYPE_ID '[' INT_VAL ']' IDENTIFIER ;
    // Example: array int[10] int10 ;
    cDeclNode* ParseArrayTypeDecl()
    {
        Expect(ARRAY, "'array'");

        if (g_tok != TYPE_ID)
        {
            ParseError("expected element type after array");
            return nullptr;
        }
        cSymbol* elemType = yylval.symbol;
        Next();

        Expect('[', "'['");
        if (g_tok != INT_VAL)
        {
            ParseError("expected integer size inside []");
            return nullptr;
        }
        int size = yylval.int_val;
        Next();
        Expect(']', "']'");

        if (g_tok != IDENTIFIER)
        {
            ParseError("expected array type name");
            return nullptr;
        }
        cSymbol* typeName = yylval.symbol;
        Next();
        Expect(';', "';'");

        cArrayTypeDeclNode* decl = new cArrayTypeDeclNode(typeName, elemType, size);
        typeName->SetDecl(decl); // make this a TYPE
        return decl;
    }

    // TYPE_ID IDENTIFIER ( var | func-proto | func-def )
    cDeclNode* ParseVarOrFuncDecl()
    {
        cSymbol* typeSym = yylval.symbol;
        Next();

        if (g_tok != IDENTIFIER)
        {
            ParseError("expected identifier after type");
            while (g_tok != ';' && g_tok != 0) Next();
            Match(';');
            return nullptr;
        }
        cSymbol* nameSym = yylval.symbol;
        Next();

        // function? (prototype or definition)
        if (g_tok == '(')
        {
            Next(); // '('

            cParamsNode* params = new cParamsNode();

            // params: ( TYPE_ID IDENTIFIER (',' ...)* )?
            if (g_tok != ')')
            {
                while (true)
                {
                    if (g_tok != TYPE_ID)
                    {
                        ParseError("expected parameter type");
                        break;
                    }
                    cSymbol* pType = yylval.symbol;
                    Next();

                    if (g_tok != IDENTIFIER)
                    {
                        ParseError("expected parameter name");
                        break;
                    }
                    cSymbol* pName = yylval.symbol;
                    Next();

                    params->AddParam(new cParamNode(pType, pName));

                    if (g_tok == ',')
                    {
                        Next();
                        continue;
                    }
                    break;
                }
            }

            Expect(')', "')'");

            // prototype: ... ) ;
            if (g_tok == ';')
            {
                Next();
                cFuncDeclNode* proto = new cFuncDeclNode(typeSym, nameSym, params, nullptr);
                // For many labs, function name becomes a symbol with decl:
                nameSym->SetDecl(proto);
                return proto;
            }

            // definition: ... ) block
            if (g_tok == '{')
            {
                cBlockNode* body = ParseBlock();
                cFuncDeclNode* defn = new cFuncDeclNode(typeSym, nameSym, params, body);
                nameSym->SetDecl(defn);
                return defn;
            }

            ParseError("expected ';' or function body block after ')'");
            return nullptr;
        }

        // variable decl: TYPE_ID IDENTIFIER ;
        Expect(';', "';'");
        cVarDeclNode* var = new cVarDeclNode(typeSym, nameSym);
        // mark symbol as variable
        nameSym->SetDecl(var);
        return var;
    }

    // stmts -> stmt*
    cStmtsNode* ParseStmts()
    {
        cStmtsNode* stmts = nullptr;

        while (IsStmtStart())
        {
            cStmtNode* s = ParseStmt();
            if (s != nullptr)
            {
                if (stmts == nullptr) stmts = new cStmtsNode(s);
                else stmts->Insert(s);
            }
        }

        return stmts;
    }

// Parse a "statement group" for if/while bodies when braces are omitted.
// It consumes 1 or more statements until it reaches a terminator token.
cStmtNode* ParseStmtGroupUntil(std::initializer_list<int> terminators)
{
    auto isTerminator = [&](int tok) {
        for (int t : terminators) if (tok == t) return true;
        return false;
    };

    // Always parse at least one statement
    cStmtNode* first = ParseStmt();

    // If the next token starts another statement and we're not at a terminator,
    // keep consuming statements and wrap them in a cStmtsNode "block-like" node.
    if (IsStmtStart() && !isTerminator(g_tok))
    {
        cStmtsNode* list = new cStmtsNode(first);

        while (IsStmtStart() && !isTerminator(g_tok))
            list->Insert(ParseStmt());

        // Wrap statement-list in a block node with no decls (so it behaves like a statement)
        return new cStmtBlockNode(list);  // new small node (see below)
    }

    return first;
}

    cStmtNode* ParseStmt()
    {
        if (g_tok == PRINT)  return ParsePrintStmt();
        if (g_tok == PRINTS) return ParsePrintsStmt();
        if (g_tok == IF)     return ParseIfStmt();
        if (g_tok == WHILE)  return ParseWhileStmt();
        if (g_tok == RETURN) return ParseReturnStmt();
        if (g_tok == '{')    return ParseBlock();
        if (g_tok == IDENTIFIER) return ParseAssignOrCallStmt();

        ParseError("unexpected token in statement");
        Next();
        return nullptr;
    }

    cStmtNode* ParsePrintStmt()
    {
        Expect(PRINT, "'print'");
        Expect('(', "'('");
        cExprNode* e = ParseExpr();
        Expect(')', "')'");
        Expect(';', "';'");
        return new cPrintNode(e);
    }

    cStmtNode* ParsePrintsStmt()
{
    Expect(PRINTS, "'prints'");
    Expect('(', "'('");

    if (g_tok != STRING_LIT)
    {
        ParseError("prints() expects a string literal");
        // try to recover
        while (g_tok != ')' && g_tok != ';' && g_tok != 0) Next();
    }

    // yylval.str_val is a std::string*
    std::string s = (yylval.str_val != nullptr) ? *yylval.str_val : std::string("");

    // (optional) free token string to avoid leaks if your lexer allocates with new
    if (yylval.str_val != nullptr)
    {
        delete yylval.str_val;
        yylval.str_val = nullptr;
    }

    Next(); // consume STRING_LIT

    Expect(')', "')'");
    Expect(';', "';'");
    return new cPrintsNode(s);
}

    cStmtNode* ParseReturnStmt()
    {
        Expect(RETURN, "'return'");

        // return expr; OR return(expr);
        cExprNode* e = nullptr;
        if (g_tok == '(')
        {
            Next();
            e = ParseExpr();
            Expect(')', "')'");
        }
        else
        {
            e = ParseExpr();
        }

        Expect(';', "';'");
        return new cReturnNode(e);
    }

    // if '(' expr ')' stmt (else stmt)? endif ';'
    cStmtNode* ParseIfStmt()
    {
        Expect(IF, "'if'");
        Expect('(', "'('");
        cExprNode* cond = ParseExpr();
        Expect(')', "')'");
        
        cStmtNode* thenStmt = ParseStmtGroupUntil({ ELSE, ENDIF, '}' });

        cStmtNode* elseStmt = nullptr;
        if (g_tok == ELSE)
        {
            Next();
            elseStmt = ParseStmtGroupUntil({ ENDIF, '}' });        
        }

        Expect(ENDIF, "'endif'");
        Expect(';', "';'");
        return new cIfNode(cond, thenStmt, elseStmt);
    }

    // while '(' expr ')' stmt
    cStmtNode* ParseWhileStmt()
    {
        Expect(WHILE, "'while'");
        Expect('(', "'('");
        cExprNode* cond = ParseExpr();
        Expect(')', "')'");
        
        cStmtNode* body = ParseStmtGroupUntil({ ENDIF, ELSE, '}' });
        return new cWhileNode(cond, body);
    }

    // stmt starting with IDENTIFIER:
    // - call: name '(' args? ')' ';'
    // - assign: lvalue '=' expr ';'
    cStmtNode* ParseAssignOrCallStmt()
    {
        cSymbol* base = yylval.symbol;
        Next();

        // call statement
        if (g_tok == '(')
        {
            Next();
            cArgsNode* args = new cArgsNode();
            if (g_tok != ')')
            {
                while (true)
                {
                    args->AddArg(ParseExpr());
                    if (g_tok == ',') { Next(); continue; }
                    break;
                }
            }
            Expect(')', "')'");
            Expect(';', "';'");
            return new cCallStmtNode(new cCallExprNode(base, args));
        }

        // otherwise: parse lvalue tail: [expr] and .id chains
        cLValueNode* lv = new cLValueNode(base);

        while (g_tok == '[' || g_tok == '.')
        {
            if (g_tok == '[')
            {
                Next();
                cExprNode* idx = ParseExpr();
                Expect(']', "']'");
                lv->AddIndex(idx);
            }
            else // '.'
            {
                Next();
                if (g_tok != IDENTIFIER)
                {
                    ParseError("expected identifier after '.'");
                    break;
                }
                cSymbol* field = yylval.symbol;
                Next();
                lv->AddField(field);
            }
        }

        Expect('=', "'='");
        cExprNode* rhs = ParseExpr();
        Expect(';', "';'");

        return new cAssignNode(lv, rhs);
    }

    // ---------- Expressions ----------
    cExprNode* ParseExpr() { return ParseEquality(); }

    // equality: add ( '==' add )*
    cExprNode* ParseEquality()
    {
        cExprNode* left = ParseAdd();
        while (g_tok == EQUALS)
        {
            int op = g_tok;
            Next();
            cExprNode* right = ParseAdd();
            left = new cBinaryExprNode(left, new cOpNode(op), right);
        }
        return left;
    }

    // add: mul (('+'|'-') mul)*
    cExprNode* ParseAdd()
    {
        cExprNode* left = ParseMul();
        while (g_tok == '+' || g_tok == '-')
        {
            int op = g_tok;
            Next();
            cExprNode* right = ParseMul();
            left = new cBinaryExprNode(left, new cOpNode(op), right);
        }
        return left;
    }

    // mul: postfix (('*'|'/'|'%') postfix)*
    cExprNode* ParseMul()
    {
        cExprNode* left = ParsePostfix();
        while (g_tok == '*' || g_tok == '/' || g_tok == '%')
        {
            int op = g_tok;
            Next();
            cExprNode* right = ParsePostfix();
            left = new cBinaryExprNode(left, new cOpNode(op), right);
        }
        return left;
    }

    // postfix: primary ( call | index | field )*
    cExprNode* ParsePostfix()
    {
        cExprNode* e = ParsePrimary();

        while (true)
        {
            // function call: (args)
            if (g_tok == '(')
            {
                // only valid if base is a varref/call already; we support call on identifier-style primary
                Next();
                cArgsNode* args = new cArgsNode();
                if (g_tok != ')')
                {
                    while (true)
                    {
                        args->AddArg(ParseExpr());
                        if (g_tok == ',') { Next(); continue; }
                        break;
                    }
                }
                Expect(')', "')'");
                e = new cCallExprPostfixNode(e, args);
                continue;
            }

            // indexing: [expr]
            if (g_tok == '[')
            {
                Next();
                cExprNode* idx = ParseExpr();
                Expect(']', "']'");
                e = new cIndexExprNode(e, idx);
                continue;
            }

            // field: .IDENTIFIER
            if (g_tok == '.')
            {
                Next();
                if (g_tok != IDENTIFIER)
                {
                    ParseError("expected identifier after '.'");
                    break;
                }
                cSymbol* field = yylval.symbol;
                Next();
                e = new cDotExprNode(e, field);
                continue;
            }

            break;
        }

        return e;
    }

    // primary: '(' expr ')' | INT | FLOAT | IDENTIFIER
    cExprNode* ParsePrimary()
    {
        if (g_tok == '(')
        {
            Next();
            cExprNode* e = ParseExpr();
            Expect(')', "')'");
            return e;
        }

        if (g_tok == INT_VAL)
        {
            int v = yylval.int_val;
            Next();
            return new cIntExprNode(v);
        }

        if (g_tok == FLOAT_VAL)
        {
            float v = yylval.float_val;
            Next();
            return new cFloatExprNode(v);
        }

        if (g_tok == IDENTIFIER || g_tok == TYPE_ID)
        {
            // in expressions, TYPE_ID can occur as a var name only if someone mis-declared;
            // but for safety, treat it like identifier reference.
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

    if (g_tok != 0)
        std::cerr << "ERROR: junk at end of program at symbol " << (yytext ? yytext : "")
                  << " on line " << yylineno << "\n";

    yyast_root = root;
    return (yynerrs == 0) ? 0 : 1;
}

void SemanticParseError(std::string error)
{
    std::cerr << "ERROR: " << error << " near line " << yylineno << "\n";
    yynerrs++;
}
