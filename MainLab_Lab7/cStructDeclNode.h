#pragma once
#include "cDeclNode.h"
#include "cSymbol.h"
#include "cDeclsNode.h"

// Represents: struct { ... } TypeName;
class cStructDeclNode : public cDeclNode
{
public:
    cStructDeclNode(cDeclsNode* fields, cSymbol* nameSym)
        : m_nameSym(nameSym)
    {
        if (fields != nullptr) AddChild(fields);
        AddChild(nameSym);
    }

    virtual bool IsStruct() override { return true; }
    virtual bool IsType() override { return true; }

    // REQUIRED BY cDeclNode:
    // GetType() must return cDeclNode*
    virtual cDeclNode* GetType() override
    {
        // For a type declaration node, returning itself is fine for parsing stage
        return this;
    }

    // GetName() must return std::string
    virtual std::string GetName() override
    {
        return (m_nameSym != nullptr) ? m_nameSym->GetName() : std::string("");
    }

    virtual string NodeType() override { return "struct_decl"; }
    virtual void Visit(cVisitor* v) override { v->Visit(this); }

    cDeclsNode* GetFields() { return static_cast<cDeclsNode*>(GetChild(0)); }
    cSymbol* GetNameSym() { return static_cast<cSymbol*>(GetChild(1)); }

    std::string AttributesToString() override
    {
        // Lab 6 requires size/offset to always be present (even when 0)
        std::string r;
        r += " size=\"" + std::to_string(GetSize()) + "\"";
        r += " offset=\"" + std::to_string(GetOffset()) + "\"";
        return r;
    }

private:
    cSymbol* m_nameSym;
};