#pragma once
#include "cStmtNode.h"
#include "cExprNode.h"

class cWhileNode : public cStmtNode
{
public:
    cWhileNode(cExprNode* cond, cStmtNode* body)
    {
        AddChild(cond);
        AddChild(body);
    }
    virtual string NodeType() { return "while"; }
    virtual void Visit(cVisitor* v) { v->Visit(this); }
};