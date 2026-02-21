#pragma once
//**************************************
// cBaseTypeNode.h
//
// Declaration node for built-in scalar types (char,int,long,float,double).
// Implements size + "is type" predicates for Lab 5.
//**************************************

#include <string>
#include "cDeclNode.h"

class cBaseTypeNode : public cDeclNode
{
public:
    cBaseTypeNode(const std::string& name, int size, bool isFloat)
        : cDeclNode(), m_name(name), m_size(size), m_isFloat(isFloat)
    {
    }

    // cDeclNode predicates
    bool IsType() override  { return true; }
    bool IsFloat() override { return m_isFloat; }
    bool IsInt() override   { return !m_isFloat; }
    bool IsChar() override  { return (!m_isFloat && m_size == 1); }
    int  GetSize() override { return m_size; }

    cDeclNode* GetType() override { return this; }

    std::string GetName() override { return m_name; }

    std::string NodeType() override { return std::string("type"); }
    void Visit(cVisitor* visitor) override { visitor->Visit(this); }
    std::string AttributesToString() override
    {
        std::string result;
        result += " name=\"" + m_name + "\"";
        result += " size=\"" + std::to_string(m_size) + "\"";
        return result;
    }

private:
    std::string m_name;
    int m_size{0};
    bool m_isFloat{false};
};
