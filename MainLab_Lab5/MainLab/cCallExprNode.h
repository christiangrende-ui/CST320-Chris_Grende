#pragma once
#include "cExprNode.h"
#include "cSymbol.h"
#include "cArgsNode.h"

// call: f(args)
class cCallExprNode : public cExprNode
{
public:
    cCallExprNode(cSymbol* fn, cArgsNode* args)
    {
        AddChild(fn);
        AddChild(args);
    }

    virtual string NodeType() override { return "call"; }
    virtual void Visit(cVisitor* v) override { v->Visit(this); }

    // REQUIRED BY cExprNode
    virtual cDeclNode* GetType() override
    {
        return nullptr; // semantic later
    }
};