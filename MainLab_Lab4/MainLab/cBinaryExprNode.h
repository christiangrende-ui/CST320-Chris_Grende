#pragma once
//**************************************
// cBinaryExprNode.h
//
// Defines an AST node for a binary expression.
//

#include <string>
#include "cExprNode.h"
#include "cOpNode.h"

class cBinaryExprNode : public cExprNode
{
public:
    cBinaryExprNode(cExprNode* left, cOpNode* op, cExprNode* right)
        : cExprNode()
    {
        AddChild(left);
        AddChild(op);
        AddChild(right);
    }

    virtual std::string NodeType() override { return std::string("expr"); }
    virtual void Visit(cVisitor* visitor) override { visitor->Visit(this); }

    cExprNode* GetLeft()  { return static_cast<cExprNode*>(GetChild(0)); }
    cOpNode* GetOp()      { return static_cast<cOpNode*>(GetChild(1)); }
    cExprNode* GetRight() { return static_cast<cExprNode*>(GetChild(2)); }
};
