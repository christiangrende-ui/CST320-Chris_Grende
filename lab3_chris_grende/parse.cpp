//**************************************
// parse.cpp
//
// Top-down recursive-descent parser for CST320 Lab 3.
// Implements the grammar exactly as provided:
//
// PROG   -> STMTS end
// STMTS  -> STMT STMTS | λ
// STMT   -> EXPR ; | identifier = EXPR ;
// EXPR   -> (EXPR) EXPR’ | TERM
// EXPR’  -> PLUSOP (EXPR) EXPR’ | λ
// PLUSOP -> + | -
// TERM   -> [EXPR] TERM’ | num | { identifier }
// TERM’  -> TIMESOP [EXPR] TERM’ | λ
// TIMESOP-> * | /
//
// Error recovery:
// - Statements synchronize on ';' or 'end'.
// - For missing closing delimiters (')' and ']'), we scan forward to the
//   expected delimiter or a statement sync point.
//
// Author: (student)

#include <iostream>
#include <string>

#include "lex.h"
#include "parse.h"
#include "utils.h"

namespace
{
    // FIRST(STMT)
    bool IsStmtStart(int tok)
    {
        return tok == '(' || tok == '[' || tok == '{' || tok == TOK_NUM || tok == TOK_IDENTIFIER;
    }

    // Consume tokens until we hit a statement boundary.
    // If we stop on ';', consume it (so parsing can continue with the next stmt).
    void SyncToStatementBoundary()
    {
        while (true)
        {
            int tok = PeekToken();
            if (tok == 0 || tok == ';' || tok == TOK_END) break;
            AdvanceToken();
        }

        if (PeekToken() == ';') AdvanceToken();
    }

    // Consume tokens until we see expectedDelim, or a stmt boundary.
    // If expectedDelim is found, consume it.
    void RecoverToDelimiter(int expectedDelim)
    {
        while (true)
        {
            int tok = PeekToken();
            if (tok == 0 || tok == ';' || tok == TOK_END || tok == expectedDelim) break;
            AdvanceToken();
        }
        if (PeekToken() == expectedDelim) AdvanceToken();
    }

    bool Match(int expectedTok, const std::string &expecting)
    {
        if (PeekToken() == expectedTok)
        {
            AdvanceToken();
            return true;
        }

        Error(expecting);
        return false;
    }
}

//*******************************************
// PROG -> STMTS end
bool FindPROG()
{
    // Parse statements (possibly empty)
    FindSTMTS();

    // Expect end
    if (!Match(TOK_END, "end"))
    {
        // If we don't see end, try to recover by skipping tokens until end/EOF.
        while (PeekToken() != 0 && PeekToken() != TOK_END)
        {
            AdvanceToken();
        }
        if (PeekToken() == TOK_END) AdvanceToken();
    }

    return true;
}

//*******************************************
// STMTS -> STMT STMTS | λ
bool FindSTMTS()
{
    while (IsStmtStart(PeekToken()))
    {
        FindSTMT();
    }
    return true; // λ allowed
}

//*******************************************
// STMT -> EXPR ; | identifier = EXPR ;
bool FindSTMT()
{
    int tok = PeekToken();
    bool ok = true;

    if (tok == TOK_IDENTIFIER)
    {
        AdvanceToken(); // identifier
        if (!Match('=', "'='") ) ok = false;
        if (!FindEXPR()) ok = false;
    }
    else
    {
        if (!FindEXPR()) ok = false;
    }

    // If the expression/assignment was invalid, recover at the statement level.
    // This prevents cascaded errors like "expecting ';'" when we're still
    // sitting on the token that caused the earlier failure.
    if (!ok)
    {
        SyncToStatementBoundary();
        return false;
    }

    // Expect ';' (always report if missing)
    if (!Match(';', "';'"))
    {
        ok = false;
        // recover to next statement
        SyncToStatementBoundary();
    }

    // Only print for valid statements (as required by the lab).
    if (ok)
    {
        std::cout << "Found a statement\n";
    }

    // If invalid, we still return false so STMTS can keep going.
    return ok;
}

//*******************************************
// EXPR -> (EXPR) EXPR’ | TERM
bool FindEXPR()
{
    int tok = PeekToken();
    if (tok == '(')
    {
        AdvanceToken(); // '('
        bool ok = FindEXPR();

        if (!Match(')', "')'"))
        {
            ok = false;
            RecoverToDelimiter(')');
        }

        if (!FindEXPR_P()) ok = false;
        return ok;
    }

    return FindTERM();
}

//*******************************************
// EXPR’ -> PLUSOP (EXPR) EXPR’ | λ
bool FindEXPR_P()
{
    bool ok = true;
    while (PeekToken() == '+' || PeekToken() == '-')
    {
        if (!FindPLUSOP()) ok = false;

        if (!Match('(', "'('"))
        {
            ok = false;
            RecoverToDelimiter('(');
        }

        if (!FindEXPR()) ok = false;

        if (!Match(')', "')'"))
        {
            ok = false;
            RecoverToDelimiter(')');
        }
    }

    return ok; // λ
}

//*******************************************
// PLUSOP -> + | -
bool FindPLUSOP()
{
    if (PeekToken() == '+' || PeekToken() == '-')
    {
        AdvanceToken();
        return true;
    }

    Error("PLUSOP");
    return false;
}

//*******************************************
// TERM -> [EXPR] TERM’ | num | { identifier }
bool FindTERM()
{
    int tok = PeekToken();
    if (tok == '[')
    {
        AdvanceToken(); // '['
        bool ok = FindEXPR();
        if (!Match(']', "']'"))
        {
            ok = false;
            RecoverToDelimiter(']');
        }
        if (!FindTERM_P()) ok = false;
        return ok;
    }

    if (tok == TOK_NUM)
    {
        AdvanceToken();
        return true;
    }

    if (tok == '{')
    {
        AdvanceToken();
        bool ok = true;
        if (!Match(TOK_IDENTIFIER, "identifier"))
        {
            ok = false;
            RecoverToDelimiter('}');
            return ok;
        }

        if (!Match('}', "'}'"))
        {
            ok = false;
            RecoverToDelimiter('}');
        }

        return ok;
    }

    // Not a TERM start.
    Error("TERM");
    return false;
}

//*******************************************
// TERM’ -> TIMESOP [EXPR] TERM’ | λ
bool FindTERM_P()
{
    bool ok = true;
    while (PeekToken() == '*' || PeekToken() == '/')
    {
        if (!FindTIMESOP()) ok = false;

        // Grammar requires [EXPR] after TIMESOP.
        if (!Match('[', "'['"))
        {
            ok = false;
            return ok; // let STMT sync on ';'
        }

        if (!FindEXPR()) ok = false;

        if (!Match(']', "']'"))
        {
            ok = false;
            RecoverToDelimiter(']');
        }
    }

    return ok; // λ
}

//*******************************************
// TIMESOP -> * | /
bool FindTIMESOP()
{
    if (PeekToken() == '*' || PeekToken() == '/')
    {
        AdvanceToken();
        return true;
    }

    Error("TIMESOP");
    return false;
}
