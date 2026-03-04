#pragma once
//**************************************
// cIntExprNode.h
//
// Integer constant (literal).
// Lab 6: prints size attribute.
//**************************************

#include <string>
#include "cExprNode.h"
#include "cSymbolTable.h"

extern cSymbolTable g_symbolTable;

class cIntExprNode : public cExprNode
{
public:
    explicit cIntExprNode(int value) : m_value(value) {}

    int GetValue() const { return m_value; }

    // IMPORTANT: int literals are INT (not char)
    cDeclNode* GetType() override
    {
        cSymbol* sym = g_symbolTable.Lookup("int");
        return (sym && sym->GetDecl()) ? sym->GetDecl()->GetType() : nullptr;
    }

    void SetSize(int sz) { m_size = sz; }
    int  GetSize() const { return m_size; }

    std::string NodeType() override { return "int"; }

    std::string AttributesToString() override
    {
        return " value=\"" + std::to_string(m_value) + "\""
               " size=\""  + std::to_string(m_size) + "\"";
    }

    void Visit(cVisitor* visitor) override { visitor->Visit(this); }

private:
    int m_value{0};
    int m_size{0};
};