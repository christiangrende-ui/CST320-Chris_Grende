#pragma once
#include "cDeclNode.h"
#include "cSymbol.h"

// Type decl: array <elemType>[size] <TypeName>;
class cArrayTypeDeclNode : public cDeclNode
{
public:
    cArrayTypeDeclNode(cSymbol* typeNameSym, cSymbol* elemTypeSym, int size)
        : m_typeNameSym(typeNameSym), m_elemTypeSym(elemTypeSym), m_size(size)
    {
        AddChild(typeNameSym);
        AddChild(elemTypeSym);
    }

    virtual bool IsArray() override { return true; }
    virtual bool IsType() override { return true; }

    virtual int GetSize() override { return m_size; }

    // REQUIRED BY cDeclNode
    virtual cDeclNode* GetType() override
    {
        // A type declaration's "type" can be itself for now
        return this;
    }

    virtual std::string GetName() override
    {
        return (m_typeNameSym != nullptr) ? m_typeNameSym->GetName() : std::string("");
    }

    virtual string NodeType() override { return "array_type_decl"; }
    virtual void Visit(cVisitor* v) override { v->Visit(this); }

private:
    cSymbol* m_typeNameSym;
    cSymbol* m_elemTypeSym;
    int m_size;
};