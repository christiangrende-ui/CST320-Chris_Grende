#pragma once
#include "cAstNode.h"
#include "cExprNode.h"

class cArgsNode : public cAstNode
{
public:
    cArgsNode() {}
    void AddArg(cExprNode* e) { AddChild(e); }

    virtual string NodeType() { return "args"; }
    virtual void Visit(cVisitor* v) { v->Visit(this); }
};