#pragma once
#include "cStmtNode.h"
#include "cExprNode.h"

// if (cond) thenStmt [else elseStmt] endif;
class cIfNode : public cStmtNode
{
public:
    // elseStmt is optional (nullptr if no else)
    cIfNode(cExprNode* cond, cStmtNode* thenStmt, cStmtNode* elseStmt = nullptr)
    {
        AddChild(cond);
        AddChild(thenStmt);
        if (elseStmt != nullptr) AddChild(elseStmt);
    }

    virtual string NodeType() override { return "if"; }
    virtual void Visit(cVisitor* v) override { v->Visit(this); }
};