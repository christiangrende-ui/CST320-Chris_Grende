#pragma once
//**************************************
// cSymbol.h
//
// Defines class used to represent symbols.
// Symbols are also AST nodes in Lab output.
//**************************************

#include <string>
using std::string;

#include "cAstNode.h"

class cDeclNode;

class cSymbol : public cAstNode
{
public:
    explicit cSymbol(const string& name) : cAstNode(), m_id(++nextId), m_name(name) {}

    string GetName() const { return m_name; }
    long long GetId() const { return m_id; }

    // Declaration associated with this symbol (types, vars, funcs, etc.)
    cDeclNode* GetDecl() const { return m_decl; }
    void SetDecl(cDeclNode* decl) { m_decl = decl; }

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
    cDeclNode* m_decl{nullptr};
};
