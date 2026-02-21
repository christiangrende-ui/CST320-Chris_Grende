#pragma once
#include "cStmtNode.h"
#include "cCallExprNode.h"

class cCallStmtNode : public cStmtNode
{
public:
    explicit cCallStmtNode(cCallExprNode* call)
    {
        AddChild(call);
    }
    virtual string NodeType() { return "call_stmt"; }
    virtual void Visit(cVisitor* v) { v->Visit(this); }
};