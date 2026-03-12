//**************************************
// langlex.cpp (Lab 5-ready, matched to your token/union)
//
// Uses STRING_LIT (not STRING_VAL)
// Assumes yylval has: std::string* str_val, int int_val, float float_val, cSymbol* symbol
//

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <string>

#include "lex.h"
#include "langparse.h"
#include "cSymbolTable.h"
#include "cDeclNode.h"

FILE* yyin = nullptr;
int yylineno = 1;
int yynerrs = 0;

static std::string g_yytext;
char* yytext = nullptr;

yylval_t yylval;

extern cSymbolTable g_symbolTable;

static int PeekChar()
{
    int c = std::fgetc(yyin);
    if (c != EOF) std::ungetc(c, yyin);
    return c;
}

static int GetChar()
{
    int c = std::fgetc(yyin);
    if (c == '\n') yylineno++;
    return c;
}

static void SetYYText(const std::string& s)
{
    g_yytext = s;
    yytext = const_cast<char*>(g_yytext.c_str());
}

static void SkipWhitespaceAndComments()
{
    while (true)
    {
        int c = PeekChar();
        if (c == EOF) return;

        if (std::isspace(c))
        {
            GetChar();
            continue;
        }

        // // comment
        if (c == '/')
        {
            GetChar();
            int n = PeekChar();
            if (n == '/')
            {
                while (true)
                {
                    int ch = GetChar();
                    if (ch == EOF || ch == '\n') break;
                }
                continue;
            }
            std::ungetc('/', yyin);
        }

        return;
    }
}

static int ProcessIdentifier(const std::string& name)
{
    // keywords
    if (name == "program") { SetYYText(name); return PROGRAM; }
    if (name == "print")   { SetYYText(name); return PRINT; }
    if (name == "prints")  { SetYYText(name); return PRINTS; }

    if (name == "if")      { SetYYText(name); return IF; }
    if (name == "else")    { SetYYText(name); return ELSE; }
    if (name == "endif")   { SetYYText(name); return ENDIF; }
    if (name == "while")   { SetYYText(name); return WHILE; }
    if (name == "return")  { SetYYText(name); return RETURN; }

    if (name == "struct")  { SetYYText(name); return STRUCT; }
    if (name == "array")   { SetYYText(name); return ARRAY; }

    // built-in types
    if (name == "char" || name == "int" || name == "float" || name == "long" || name == "double")
    {
        yylval.symbol = g_symbolTable.Lookup(name);
        SetYYText(name);
        return TYPE_ID;
    }

    // existing symbol?
    if (cSymbol* existing = g_symbolTable.Lookup(name))
    {
        yylval.symbol = existing;
        SetYYText(name);
        return (existing->GetDecl() && existing->GetDecl()->IsType()) ? TYPE_ID : IDENTIFIER;
    }

    // insert new identifier
    cSymbol* sym = g_symbolTable.Insert(name);
    yylval.symbol = sym;
    SetYYText(name);
    return IDENTIFIER;
}

static int ReadStringLiteral()
{
    // consume opening quote
    GetChar();

    std::string s;

    while (true)
    {
        int c = GetChar();
        if (c == EOF) break;
        if (c == '"') break;

        if (c == '\\')
        {
            int n = GetChar();
            if (n == 'n') s.push_back('\n');
            else if (n == 't') s.push_back('\t');
            else if (n == '"') s.push_back('"');
            else if (n == '\\') s.push_back('\\');
            else s.push_back(static_cast<char>(n));
        }
        else
        {
            s.push_back(static_cast<char>(c));
        }
    }

    SetYYText(s);

    // IMPORTANT: your yylval expects std::string*
    yylval.str_val = new std::string(s);

    return STRING_LIT;
}

int yylex()
{
    if (yyin == nullptr) yyin = stdin;

    SkipWhitespaceAndComments();

    int c = PeekChar();
    if (c == EOF)
    {
        SetYYText("");
        return 0;
    }

    // string
    if (c == '"')
        return ReadStringLiteral();

    // number
    if (std::isdigit(c))
    {
        std::string num;
        bool is_float = false;

        while (std::isdigit(PeekChar()))
            num.push_back(static_cast<char>(GetChar()));

        if (PeekChar() == '.')
        {
            is_float = true;
            num.push_back(static_cast<char>(GetChar()));
            while (std::isdigit(PeekChar()))
                num.push_back(static_cast<char>(GetChar()));
        }

        SetYYText(num);
        if (is_float)
        {
            yylval.float_val = static_cast<float>(std::atof(num.c_str()));
            return FLOAT_VAL;
        }
        else
        {
            yylval.int_val = std::atoi(num.c_str());
            return INT_VAL;
        }
    }

    // identifier / keyword
    if (std::isalpha(c) || c == '_')
    {
        std::string id;
        while (true)
        {
            int p = PeekChar();
            if (!(std::isalnum(p) || p == '_')) break;
            id.push_back(static_cast<char>(GetChar()));
        }
        return ProcessIdentifier(id);
    }

    // == != <= >= && ||
    if (c == '=' || c == '!' || c == '<' || c == '>' || c == '&' || c == '|')
    {
        std::string op;
        op.push_back(static_cast<char>(GetChar()));
        int n = PeekChar();

        if ((op[0] == '=' && n == '=') || (op[0] == '!' && n == '=') ||
            (op[0] == '<' && n == '=') || (op[0] == '>' && n == '=') ||
            (op[0] == '&' && n == '&') || (op[0] == '|' && n == '|'))
        {
            op.push_back(static_cast<char>(GetChar()));
        }

        SetYYText(op);
        if (op == "==") return EQUALS;
        if (op == "!=") return NOT_EQUALS;
        if (op == "<=") return LE;
        if (op == ">=") return GE;
        if (op == "&&") return AND;
        if (op == "||") return OR;

        return static_cast<unsigned char>(op[0]);
    }

    // single-char tokens
    c = GetChar();
    SetYYText(std::string(1, static_cast<char>(c)));

    switch (c)
    {
        case '{': case '}': case ';': case '(': case ')':
        case '+': case '-': case '*': case '/': case '%':
        case '[': case ']': case ',': case '.':
        case '=':
            return c;
        default:
            return JUNK_TOKEN;
    }
}