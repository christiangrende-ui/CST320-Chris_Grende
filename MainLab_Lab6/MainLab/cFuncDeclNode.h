#pragma once
#include "cDeclNode.h"
#include "cSymbol.h"
#include "cParamsNode.h"
#include "cDeclsNode.h"
#include "cStmtsNode.h"

// Function decl/proto/def: returnType name(args) [decls stmts]
class cFuncDeclNode : public cDeclNode
{
public:
    cFuncDeclNode(cSymbol* retTypeSym, cSymbol* nameSym, cParamsNode* args, cDeclsNode* decls, cStmtsNode* stmts)
        : m_retTypeSym(retTypeSym), m_nameSym(nameSym)
    {
        AddChild(retTypeSym);
        AddChild(nameSym);
        AddChild(args);
        AddChild(decls);
        AddChild(stmts);
    }

    bool IsFunc() override { return true; }

    // For functions, size attribute is the size of the return type
    int GetSize() override { return m_size; }

    cDeclNode* GetType() override
    {
        return (m_retTypeSym != nullptr) ? m_retTypeSym->GetDecl() : nullptr;
    }

    std::string GetName() override
    {
        return (m_nameSym != nullptr) ? m_nameSym->GetName() : std::string("");
    }

    std::string NodeType() override { return "func"; }
    void Visit(cVisitor* v) override { v->Visit(this); }

    cParamsNode* GetArgs() { return static_cast<cParamsNode*>(GetChild(2)); }
    cDeclsNode* GetDecls() { return static_cast<cDeclsNode*>(GetChild(3)); }
    cStmtsNode* GetStmts() { return static_cast<cStmtsNode*>(GetChild(4)); }

    std::string AttributesToString() override
    {
        if (GetSize() == 0 && GetOffset() == 0) return std::string("");
        std::string r;
        r += " size=\"" + std::to_string(GetSize()) + "\"";
        r += " offset=\"" + std::to_string(GetOffset()) + "\"";
        return r;
    }

private:
    cSymbol* m_retTypeSym;
    cSymbol* m_nameSym;
};
