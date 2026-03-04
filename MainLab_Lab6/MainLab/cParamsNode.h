#pragma once
#include <string>
#include "cAstNode.h"
#include "cVarDeclNode.h"

class cParamsNode : public cAstNode
{
public:
    cParamsNode() {}

    int GetSize() const { return m_size; }
    void SetSize(int sz) { m_size = sz; }

    void AddParam(cVarDeclNode* p) { AddChild(p); }

    virtual string NodeType() { return "args"; }
    virtual void Visit(cVisitor* v) { v->Visit(this); }

    std::string AttributesToString() override
    {
        // Lab 6 requires size to always be present (even when 0)
        return std::string(" size=\"") + std::to_string(m_size) + "\"";
    }

private:
    int m_size{0};
};