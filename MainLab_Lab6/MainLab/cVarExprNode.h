#pragma once
//**************************************
// cVarExprNode.h
//
// AST node for a variable reference.
// Lab 5: undefined symbol checks + expression type.
//**************************************

#include <string>
#include <vector>

#include "cExprNode.h"
#include "cSymbol.h"
#include "cDeclNode.h"

class cVarExprNode : public cExprNode
{
public:
    explicit cVarExprNode(cSymbol* sym) : cExprNode()
    {
        AddChild(sym);

        // Undefined references (Lab 5a)
        if (sym == nullptr || sym->GetDecl() == nullptr)
        {
            std::string name = (sym == nullptr) ? "<unknown>" : sym->GetName();
            SemanticParseError("Symbol " + name + " not defined");
        }
    }

    cDeclNode* GetType() override
    {
        cSymbol* s = GetSym();
        if (s == nullptr || s->GetDecl() == nullptr) return nullptr;
        return s->GetDecl()->GetType();
    }

    virtual std::string NodeType() override { return std::string("varref"); }

    // Lab 6 storage
    int GetSize() const { return m_size; }
    void SetSize(int sz) { m_size = sz; }

    int GetOffset() const { return m_offset; }
    void SetOffset(int off) { m_offset = off; }

    const std::vector<int>& GetRowSizes() const { return m_rowsizes; }
    void SetRowSizes(const std::vector<int>& rs) { m_rowsizes = rs; }

    std::string AttributesToString() override
    {
        // Lab 6 requires size/offset to always be present (even when 0)
        std::string r;
        r += " size=\"" + std::to_string(m_size) + "\"";
        r += " offset=\"" + std::to_string(m_offset) + "\"";
        if (!m_rowsizes.empty())
        {
            r += " rowsizes=\"";
            for (size_t i=0;i<m_rowsizes.size();i++)
            {
                if (i) r += " ";
                r += std::to_string(m_rowsizes[i]);
            }
            r += "\"";
        }
        return r;
    }
    virtual void Visit(cVisitor* visitor) override { visitor->Visit(this); }

    cSymbol* GetSym(int idx = 0)
    {
        return static_cast<cSymbol*>(GetChild(idx));
    }

    int NumParts() { return NumChildren(); }

    // Building varrefs with fields/indices
    void AddIndex(cExprNode* idx) { AddChild(idx); }
    void AddField(cSymbol* field) { AddChild(field); }
private:
    int m_size{0};
    int m_offset{0};
    std::vector<int> m_rowsizes;
};