#pragma once
#include "cStmtNode.h"
#include <string>

class cPrintsNode : public cStmtNode
{
public:
    explicit cPrintsNode(const std::string& s) : m_text(s) {}
    virtual string NodeType() { return "prints"; }
    virtual void Visit(cVisitor* v) { v->Visit(this); }

    const std::string& GetText() const { return m_text; }

private:
    std::string m_text;
};