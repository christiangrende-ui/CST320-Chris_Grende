#pragma once
#include "cDeclNode.h"
#include "cSymbol.h"

// Type decl: array <elemType>[count] <TypeName>;
class cArrayTypeDeclNode : public cDeclNode
{
public:
    // IMPORTANT: parameter order matches langparse.cpp usage:
    // new cArrayTypeDeclNode(typeName, elemType, count)
    cArrayTypeDeclNode(cSymbol* typeNameSym, cSymbol* elemTypeSym, int count)
        : m_typeNameSym(typeNameSym), m_elemTypeSym(elemTypeSym), m_count(count)
    {
        // XML expects element type then type name
        AddChild(elemTypeSym);
        AddChild(typeNameSym);
    }

    bool IsArray() override { return true; }
    bool IsType()  override { return true; }

    int GetSize() override { return m_size; }

    cDeclNode* GetType() override
    {
        // A type declaration node's "type" can be itself
        return this;
    }

    std::string GetName() override
    {
        return (m_typeNameSym != nullptr) ? m_typeNameSym->GetName() : std::string("");
    }

    std::string NodeType() override { return "array_decl"; }

    std::string AttributesToString() override
    {
        std::string r;
        r += " count=\"" + std::to_string(m_count) + "\"";
        r += " size=\""  + std::to_string(GetSize()) + "\"";
        r += " offset=\"" + std::to_string(GetOffset()) + "\"";
        return r;
    }

    void Visit(cVisitor* v) override { v->Visit(this); }

    int GetCount() const { return m_count; }
    cSymbol* GetElemTypeSym() const { return m_elemTypeSym; }
    cSymbol* GetTypeNameSym() const { return m_typeNameSym; }

private:
    cSymbol* m_typeNameSym{nullptr};
    cSymbol* m_elemTypeSym{nullptr};
    int m_count{0};
};