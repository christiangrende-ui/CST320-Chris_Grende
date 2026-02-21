#pragma once
//**************************************
// cExprNode.h
//
// Defines base class for all expressions
// Lab 5 adds expression type inference.
//**************************************

#include "cStmtNode.h"
#include "cDeclNode.h"

class cExprNode : public cStmtNode
{
public:
    cExprNode() : cStmtNode() {}

    // Return the declaration node representing the type of this expression.
    virtual cDeclNode *GetType() = 0;
};
