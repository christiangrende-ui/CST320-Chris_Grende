#pragma once
#include "cStmtNode.h"
#include "cStmtsNode.h"

// A statement wrapper around a list of statements (no braces in source)
class cStmtBlockNode : public cStmtNode
{
public:
    explicit cStmtBlockNode(cStmtsNode* stmts)
    {
        AddChild(stmts);
    }

    virtual string NodeType() override { return "stmt_block"; }
    virtual void Visit(cVisitor* v) override { v->Visit(this); }
};