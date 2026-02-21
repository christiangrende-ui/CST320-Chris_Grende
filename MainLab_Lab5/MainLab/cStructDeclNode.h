#pragma once
#include "cDeclNode.h"
#include "cSymbol.h"
#include "cDeclsNode.h"

// Represents: struct { ... } TypeName;
class cStructDeclNode : public cDeclNode
{
public:
    cStructDeclNode(cSymbol* nameSym, cDeclsNode* fields)
        : m_nameSym(nameSym)
    {
        AddChild(nameSym);
        if (fields != nullptr) AddChild(fields);
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

private:
    cSymbol* m_nameSym;
};