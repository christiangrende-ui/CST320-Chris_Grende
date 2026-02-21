#pragma once
#include "cAstNode.h"
#include "cSymbol.h"
#include "cExprNode.h"

class cLValueNode : public cAstNode
{
public:
    explicit cLValueNode(cSymbol* base)
    {
        AddChild(base);
    }

    void AddIndex(cExprNode* idx) { AddChild(idx); }    // represent as extra children
    void AddField(cSymbol* field) { AddChild(field); }  // represent as extra children

    virtual string NodeType() { return "lvalue"; }
    virtual void Visit(cVisitor* v) { v->Visit(this); }
};