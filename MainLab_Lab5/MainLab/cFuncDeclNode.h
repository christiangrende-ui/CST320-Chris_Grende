#pragma once
#include "cDeclNode.h"
#include "cSymbol.h"
#include "cParamsNode.h"
#include "cBlockNode.h"

// Function decl/proto: returnType name(params) [body]
class cFuncDeclNode : public cDeclNode
{
public:
    cFuncDeclNode(cSymbol* retTypeSym, cSymbol* nameSym, cParamsNode* params, cBlockNode* body)
        : m_retTypeSym(retTypeSym), m_nameSym(nameSym)
    {
        // keep children for XML output / visitor traversal
        AddChild(retTypeSym);
        AddChild(nameSym);
        AddChild(params);
        if (body != nullptr) AddChild(body);
    }

    virtual bool IsFunc() override { return true; }

    // REQUIRED BY cDeclNode
    virtual cDeclNode* GetType() override
    {
        // return type is represented by the decl attached to the type symbol (e.g., cBaseTypeNode)
        return (m_retTypeSym != nullptr) ? m_retTypeSym->GetDecl() : nullptr;
    }

    virtual std::string GetName() override
    {
        return (m_nameSym != nullptr) ? m_nameSym->GetName() : std::string("");
    }

    virtual string NodeType() override { return "func_decl"; }
    virtual void Visit(cVisitor* v) override { v->Visit(this); }

private:
    cSymbol* m_retTypeSym;
    cSymbol* m_nameSym;
};