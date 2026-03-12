#pragma once
#include "cExprNode.h"

// expr[index]
class cIndexExprNode : public cExprNode
{
public:
    cIndexExprNode(cExprNode* base, cExprNode* index)
    {
        AddChild(base);
        AddChild(index);
    }

    virtual string NodeType() override { return "index"; }
    virtual void Visit(cVisitor* v) override { v->Visit(this); }

    virtual cDeclNode* GetType() override
    {
        return nullptr; // semantic later
    }
};