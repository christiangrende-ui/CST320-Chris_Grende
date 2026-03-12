#pragma once
#include "cExprNode.h"
#include "cSymbol.h"

// expr.field
class cDotExprNode : public cExprNode
{
public:
    cDotExprNode(cExprNode* base, cSymbol* field)
    {
        AddChild(base);
        AddChild(field);
    }

    virtual string NodeType() override { return "dot"; }
    virtual void Visit(cVisitor* v) override { v->Visit(this); }

    virtual cDeclNode* GetType() override
    {
        return nullptr; // semantic later
    }
};