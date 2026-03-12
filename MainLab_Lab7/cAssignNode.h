#pragma once
#include "cStmtNode.h"
#include "cVarExprNode.h"
#include "cExprNode.h"

class cAssignNode : public cStmtNode
{
public:
    cAssignNode(cVarExprNode* lhs, cExprNode* rhs)
    {
        AddChild(lhs);
        AddChild(rhs);
    }
    virtual string NodeType() { return "assign"; }
    virtual void Visit(cVisitor* v) { v->Visit(this); }
};