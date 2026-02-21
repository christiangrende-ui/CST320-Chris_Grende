#pragma once
#include "cAstNode.h"
#include "cParamNode.h"

class cParamsNode : public cAstNode
{
public:
    cParamsNode() {}
    void AddParam(cParamNode* p) { AddChild(p); }

    virtual string NodeType() { return "params"; }
    virtual void Visit(cVisitor* v) { v->Visit(this); }
};