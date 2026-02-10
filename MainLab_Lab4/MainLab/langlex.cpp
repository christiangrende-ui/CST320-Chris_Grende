//**************************************
// langlex.cpp
//
// Hand-written lexer for Lab 4.
// Produces the subset of tokens needed for tests 1-4.
//

#include <cctype>
#include <cstdio>
#include <cstring>
#include <string>

#include "lex.h"
#include "langparse.h"
#include "cSymbolTable.h"

// Flex-like globals expected by the framework
FILE* yyin = nullptr;
int yylineno = 1;
int yynerrs = 0;

// Keep yytext as a stable C-string pointer
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
    // yytext is required to be a mutable char*. We provide a pointer to the internal buffer.
    yytext = const_cast<char*>(g_yytext.c_str());
}

static void SkipWhitespaceAndComments()
{
    while (true)
    {
        int c = PeekChar();
        if (c == EOF) return;

        // whitespace
        if (std::isspace(c))
        {
            GetChar();
            continue;
        }

        // line comment //...
        if (c == '/')
        {
            GetChar();
            int n = PeekChar();
            if (n == '/')
            {
                // consume rest of line
                while (true)
                {
                    int ch = GetChar();
                    if (ch == EOF || ch == '\n') break;
                }
                continue;
            }
            // not a comment: put back
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
        return existing->IsType() ? TYPE_ID : IDENTIFIER;
    }

    // insert new
    cSymbol* sym = g_symbolTable.Insert(name, false);
    yylval.symbol = sym;
    SetYYText(name);
    return IDENTIFIER;
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

    // Numbers: INT or FLOAT
    if (std::isdigit(c))
    {
        std::string num;
        bool is_float = false;

        while (std::isdigit(PeekChar()))
        {
            num.push_back(static_cast<char>(GetChar()));
        }

        if (PeekChar() == '.')
        {
            is_float = true;
            num.push_back(static_cast<char>(GetChar()));
            while (std::isdigit(PeekChar()))
            {
                num.push_back(static_cast<char>(GetChar()));
            }
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

    // Identifiers
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

    // Multi-char operators
    auto take2 = [&]() -> std::string {
        int c1 = GetChar();
        int c2 = PeekChar();
        if (c2 == EOF) return std::string(1, static_cast<char>(c1));
        int c2take = GetChar();
        std::string s;
        s.push_back(static_cast<char>(c1));
        s.push_back(static_cast<char>(c2take));
        return s;
    };

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

        // single-char '=' '<' '>' '!' etc
        return static_cast<unsigned char>(op[0]);
    }

    // Single-character punctuation/operators
    c = GetChar();
    SetYYText(std::string(1, static_cast<char>(c)));

    switch (c)
    {
        case '{': case '}': case ';': case '(': case ')':
        case '+': case '-': case '*': case '/': case '%':
        case '[': case ']': case ',': case '.':
            return c;
        default:
            return JUNK_TOKEN;
    }
}
