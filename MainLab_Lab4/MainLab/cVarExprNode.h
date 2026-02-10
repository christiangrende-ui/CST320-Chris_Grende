#pragma once
//**************************************
// cVarExprNode.h
//
// Defines an AST node for a variable reference.
//

#include <string>

#include "cExprNode.h"
#include "cSymbol.h"

class cVarExprNode : public cExprNode
{
public:
    explicit cVarExprNode(cSymbol* sym) : cExprNode()
    {
        AddChild(sym);
    }

    virtual std::string NodeType() override { return std::string("varref"); }
    virtual void Visit(cVisitor* visitor) override { visitor->Visit(this); }

    cSymbol* GetSym(int idx = 0)
    {
        return static_cast<cSymbol*>(GetChild(idx));
    }
};
