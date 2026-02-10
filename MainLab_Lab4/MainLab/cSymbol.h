#pragma once
//**************************************
// cSymbol.h
//
// Defines class used to represent symbols.
// Symbols are also AST nodes in Lab 4 output.
//

#include <string>

using std::string;

#include "cAstNode.h"

class cSymbol : public cAstNode
{
public:
    explicit cSymbol(const string& name) : cAstNode(), m_id(++nextId), m_name(name) {}

    string GetName() const { return m_name; }
    long long GetId() const { return m_id; }

    // Used in later tests to distinguish type identifiers from normal identifiers.
    bool IsType() const { return m_isType; }
    void SetIsType(bool isType) { m_isType = isType; }

protected:
    virtual string AttributesToString() override
    {
        string result(" id=\"");
        result += std::to_string(m_id);
        result += "\" name=\"" + m_name + "\"";
        return result;
    }

    virtual string NodeType() override { return string("sym"); }
    virtual void Visit(cVisitor* visitor) override { visitor->Visit(this); }

public:
    static long long nextId; // Next available symbol ID (defined in main.cpp)

private:
    long long m_id;
    string m_name;
    bool m_isType{false};
};
