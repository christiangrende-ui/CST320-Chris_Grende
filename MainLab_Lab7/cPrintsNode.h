#pragma once
#include "cStmtNode.h"
#include <string>

class cPrintsNode : public cStmtNode
{
public:
    // Your parser likely passes a std::string (or one it dereferenced from yylval.str_val)
    explicit cPrintsNode(const std::string& s) : m_text(s) {}

    std::string NodeType() override { return "prints"; }
    void Visit(cVisitor* v) override { v->Visit(this); }

    std::string AttributesToString() override
    {
        // Minimal XML escaping
        std::string esc;
        esc.reserve(m_text.size());
        for (char c : m_text)
        {
            if (c == '&') esc += "&amp;";
            else if (c == '"') esc += "&quot;";
            else esc += c;
        }
        return " value=\"" + esc + "\"";
    }

public:
    const std::string& GetText() const { return m_text; }

private:
    std::string m_text;
};