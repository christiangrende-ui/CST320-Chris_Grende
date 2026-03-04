#pragma once
//**************************************
// cBinaryExprNode.h
//
// AST node for a binary expression.
// Lab 5: expression type inference (promotion rules).
//**************************************

#include <string>
#include "cExprNode.h"
#include "cOpNode.h"
#include "cSymbolTable.h"

// Global symbol table defined in main.cpp
extern cSymbolTable g_symbolTable;

class cBinaryExprNode : public cExprNode
{
public:
    cBinaryExprNode(cExprNode* left, cOpNode* op, cExprNode* right)
        : cExprNode()
    {
        AddChild(left);
        AddChild(op);
        AddChild(right);
    }

    cExprNode* GetLeft()  { return static_cast<cExprNode*>(GetChild(0)); }
    cOpNode*   GetOp()    { return static_cast<cOpNode*>(GetChild(1)); }
    cExprNode* GetRight() { return static_cast<cExprNode*>(GetChild(2)); }

    cDeclNode* GetType() override
    {
        cDeclNode* lt = (GetLeft()  ? GetLeft()->GetType()  : nullptr);
        cDeclNode* rt = (GetRight() ? GetRight()->GetType() : nullptr);
        if (lt == nullptr || rt == nullptr) return nullptr;

        const int op = GetOp() ? GetOp()->GetOp() : 0;

        // Relational/logical operators always return int
        if (op == '>' || op == '<' || op == EQUALS || op == NOT_EQUALS ||
            op == OR || op == AND)
        {
            cSymbol* intSym = g_symbolTable.Lookup("int");
            return (intSym && intSym->GetDecl()) ? intSym->GetDecl()->GetType() : nullptr;
        }

        // Unary not handled here; for binary, return "largest" type.
        // Within category: smaller promotes to larger by size.
        // Across categories: integer promotes to floating-point.
        if (lt->IsFloat() || rt->IsFloat())
        {
            // Pick larger floating-point type.
            cDeclNode* bigger = lt;
            if (rt->IsFloat() && rt->GetSize() > lt->GetSize()) bigger = rt;
            if (bigger->IsFloat()) return bigger;
            // One is float, other is int: result is the float one.
            return lt->IsFloat() ? lt : rt;
        }
        // Both integer category
        return (rt->GetSize() > lt->GetSize()) ? rt : lt;
    }

    virtual std::string NodeType() override { return std::string("expr"); }
    virtual void Visit(cVisitor* visitor) override { visitor->Visit(this); }
};
