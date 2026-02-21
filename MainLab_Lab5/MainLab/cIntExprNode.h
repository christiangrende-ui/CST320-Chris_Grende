#pragma once
//**************************************
// cIntExprNode.h
//
// Integer constant (literal).
// Lab 5: determines whether literal fits in char or requires int.
//**************************************

#include "cExprNode.h"
#include "cSymbolTable.h"

// Global symbol table defined in main.cpp
extern cSymbolTable g_symbolTable;

class cIntExprNode : public cExprNode
{
public:
    explicit cIntExprNode(int value) : cExprNode(), m_value(value) {}

    int GetValue() const { return m_value; }

    cDeclNode* GetType() override
    {
        // char range is 0..127 per lab spec
        const bool fitsChar = (m_value >= 0 && m_value <= 127);
        const char* typeName = fitsChar ? "char" : "int";
        cSymbol* sym = g_symbolTable.Lookup(typeName);
        return (sym && sym->GetDecl()) ? sym->GetDecl()->GetType() : nullptr;
    }

    virtual string AttributesToString() override
    {
        return " value=\"" + std::to_string(m_value) + "\"";
    }
    virtual string NodeType() override { return string("int"); }
    virtual void Visit(cVisitor *visitor) override { visitor->Visit(this); }

private:
    int m_value;
};
