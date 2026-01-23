//**********************************************
// scanner.cpp
//
// Fallback scanner (no-flex) for CST320 Lab 3.
//
// Your instructor asked for a flex-generated scanner. The project also
// includes calc.l for that purpose. However, some environments may not have
// flex installed. This file provides a small compatible scanner that can be
// used instead (the Makefile will prefer flex when available).
//
// The scanner supports ONLY the tokens in the lab grammar:
//   end, num, identifier, and the punctuation/operators:
//   + - * / ( ) [ ] { } = ;
// plus C++ single-line comments.

#include <cctype>
#include <cstdio>
#include <cstring>

#include "lex.h"

// Required globals (normally provided by flex)
FILE *yyin = nullptr;
char *yytext = nullptr;
int yylineno = 1;
int yynerrs = 0;

namespace
{
    // Storage for the current token text
    char g_text[256];

    int NextChar()
    {
        int c = fgetc(yyin ? yyin : stdin);
        return c;
    }

    void UngetChar(int c)
    {
        if (c == EOF) return;
        ungetc(c, yyin ? yyin : stdin);
    }

    void SetText(const char *s)
    {
        std::strncpy(g_text, s, sizeof(g_text) - 1);
        g_text[sizeof(g_text) - 1] = '\0';
        yytext = g_text;
    }

    void SetTextChar(int c)
    {
        g_text[0] = static_cast<char>(c);
        g_text[1] = '\0';
        yytext = g_text;
    }
}

int yylex()
{
    // Skip whitespace + comments
    while (true)
    {
        int c = NextChar();
        if (c == EOF)
        {
            SetText("");
            return 0;
        }

        // Track line numbers
        if (c == '\n')
        {
            yylineno++;
            continue;
        }

        // Skip spaces/tabs/CR
        if (c == ' ' || c == '\t' || c == '\r')
        {
            continue;
        }

        // C++ comment
        if (c == '/')
        {
            int d = NextChar();
            if (d == '/')
            {
                // consume until end of line or EOF
                while (true)
                {
                    int e = NextChar();
                    if (e == EOF)
                    {
                        SetText("");
                        return 0;
                    }
                    if (e == '\n')
                    {
                        yylineno++;
                        break;
                    }
                }
                continue; // restart outer skip loop
            }

            // Not a comment: treat '/' as token
            UngetChar(d);
            SetTextChar('/');
            return '/';
        }

        // Number
        if (std::isdigit(c))
        {
            size_t i = 0;
            g_text[i++] = static_cast<char>(c);
            while (i < sizeof(g_text) - 1)
            {
                int d = NextChar();
                if (d == EOF || !std::isdigit(d))
                {
                    UngetChar(d);
                    break;
                }
                g_text[i++] = static_cast<char>(d);
            }
            g_text[i] = '\0';
            yytext = g_text;
            return TOK_NUM;
        }

        // Identifier or keyword
        if (std::isalpha(c) || c == '_')
        {
            size_t i = 0;
            g_text[i++] = static_cast<char>(c);
            while (i < sizeof(g_text) - 1)
            {
                int d = NextChar();
                if (d == EOF || !(std::isalnum(d) || d == '_'))
                {
                    UngetChar(d);
                    break;
                }
                g_text[i++] = static_cast<char>(d);
            }
            g_text[i] = '\0';
            yytext = g_text;

            if (std::strcmp(g_text, "end") == 0)
            {
                return TOK_END;
            }
            return TOK_IDENTIFIER;
        }

        // Single-character tokens
        switch (c)
        {
        case '+':
        case '-':
        case '*':
        case '(': case ')':
        case '[': case ']':
        case '{': case '}':
        case '=':
        case ';':
            SetTextChar(c);
            return c;
        default:
            // Unknown character: pass through for better error messages
            SetTextChar(c);
            return c;
        }
    }
}
