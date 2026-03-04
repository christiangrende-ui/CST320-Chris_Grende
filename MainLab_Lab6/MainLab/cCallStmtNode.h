#pragma once
#include "cStmtNode.h"
#include "cSymbol.h"
#include "cArgsNode.h"

// Function call as a statement: f(args);
class cCallStmtNode : public cStmtNode
{
public:
    cCallStmtNode(cSymbol* fn, cArgsNode* args)
    {
        AddChild(fn);
        AddChild(args);
    }

    virtual string NodeType() override { return "funcCall"; }
    virtual void Visit(cVisitor* v) override { v->Visit(this); }

    cSymbol* GetFnSym() { return static_cast<cSymbol*>(GetChild(0)); }
    cArgsNode* GetParams() { return static_cast<cArgsNode*>(GetChild(1)); }
};
