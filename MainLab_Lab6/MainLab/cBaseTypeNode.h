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
        : cDeclNode(), m_name(name), m_typeSize(size), m_isFloat(isFloat)
    {
    }

    // cDeclNode predicates
    bool IsType() override  { return true; }
    bool IsFloat() override { return m_isFloat; }
    bool IsInt() override   { return !m_isFloat; }
    bool IsChar() override  { return (!m_isFloat && m_typeSize == 1); }
    int  GetSize() override { return m_typeSize; }

    cDeclNode* GetType() override { return this; }

    std::string GetName() override { return m_name; }

    std::string NodeType() override { return std::string("type"); }
    void Visit(cVisitor* visitor) override { visitor->Visit(this); }
    std::string AttributesToString() override
    {
        std::string result;
        result += " name=\"" + m_name + "\"";
        result += " size=\"" + std::to_string(m_typeSize) + "\"";
        return result;
    }

private:
    std::string m_name;
    int m_typeSize{0};
    bool m_isFloat{false};
};
