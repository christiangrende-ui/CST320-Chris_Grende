#pragma once
#include "cExprNode.h"
#include "cArgsNode.h"

// postfix call: (callee)(args) where callee is an expression node
class cCallExprPostfixNode : public cExprNode
{
public:
    cCallExprPostfixNode(cExprNode* callee, cArgsNode* args)
    {
        AddChild(callee);
        AddChild(args);
    }

    virtual string NodeType() override { return "call"; }
    virtual void Visit(cVisitor* v) override { v->Visit(this); }

    virtual cDeclNode* GetType() override
    {
        return nullptr; // semantic later
    }
};