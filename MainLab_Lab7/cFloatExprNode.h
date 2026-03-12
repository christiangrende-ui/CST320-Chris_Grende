#pragma once
//**************************************
// cFloatExprNode.h
//
// Floating-point constant (literal).
//**************************************

#include "cExprNode.h"
#include "cSymbolTable.h"

// Global symbol table defined in main.cpp
extern cSymbolTable g_symbolTable;

class cFloatExprNode : public cExprNode
{
public:
    explicit cFloatExprNode(float value) : cExprNode(), m_value(value) {}

    float GetValue() const { return m_value; }

    cDeclNode* GetType() override
    {
        // Our language uses float literals only (Lab 4 scanner produces FLOAT_VAL)
        cSymbol* sym = g_symbolTable.Lookup("float");
        return (sym && sym->GetDecl()) ? sym->GetDecl()->GetType() : nullptr;
    }

    // Lab 6 storage (set by Size/Offset visitor)
    int GetSize() const { return m_size; }
    void SetSize(int sz) { m_size = sz; }

    virtual string AttributesToString() override
    {
        // Lab 6 requires size to be present
        return " value=\"" + std::to_string(m_value) + "\"" +
               " size=\"" + std::to_string(m_size) + "\"";
    }
    virtual string NodeType() override { return string("float"); }
    virtual void Visit(cVisitor *visitor) override { visitor->Visit(this); }

private:
    float m_value;
    int m_size{0};
};