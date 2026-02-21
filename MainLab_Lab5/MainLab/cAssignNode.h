#pragma once
#include "cStmtNode.h"
#include "cLValueNode.h"
#include "cExprNode.h"

class cAssignNode : public cStmtNode
{
public:
    cAssignNode(cLValueNode* lhs, cExprNode* rhs)
    {
        AddChild(lhs);
        AddChild(rhs);
    }
    virtual string NodeType() { return "assign"; }
    virtual void Visit(cVisitor* v) { v->Visit(this); }
};