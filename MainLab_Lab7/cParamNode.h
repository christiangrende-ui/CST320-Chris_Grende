#pragma once
#include "cAstNode.h"
#include "cSymbol.h"

class cParamNode : public cAstNode
{
public:
    cParamNode(cSymbol* typeSym, cSymbol* nameSym)
    {
        AddChild(typeSym);
        AddChild(nameSym);
    }
    virtual string NodeType() { return "param"; }
    virtual void Visit(cVisitor* v) { v->Visit(this); }
};