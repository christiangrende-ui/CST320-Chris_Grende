#pragma once
//**************************************
// cVarExprNode.h
//
// AST node for a variable reference.
// Lab 5: undefined symbol checks + expression type.
//**************************************

#include <string>

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
    virtual void Visit(cVisitor* visitor) override { visitor->Visit(this); }

    cSymbol* GetSym(int idx = 0)
    {
        return static_cast<cSymbol*>(GetChild(idx));
    }
};
