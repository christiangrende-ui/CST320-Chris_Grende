#pragma once
//**************************************
// cVarDeclNode.h
//
// Variable declaration.
// Lab 5a: duplicate definitions + symbol->decl linkage.
//**************************************

#include <string>

#include "cDeclNode.h"
#include "cSymbol.h"
#include "cSymbolTable.h"

// Global symbol table defined in main.cpp
extern cSymbolTable g_symbolTable;

class cVarDeclNode : public cDeclNode
{
public:
    cVarDeclNode(cSymbol* typeSym, cSymbol* nameSym)
        : cDeclNode()
    {
        AddChild(typeSym);
        AddChild(nameSym);

        // Duplicate definition in current scope (Lab 5a)
        if (nameSym)
        {
            cSymbol* existing = g_symbolTable.LookupLocal(nameSym->GetName());
            // If the symbol is already declared in this scope, it is a duplicate definition.
            if (existing != nullptr && existing->GetDecl() != nullptr)
            {
                SemanticParseError("Symbol " + nameSym->GetName() + " already defined in current scope");
            }
        }

        // Link symbol -> declaration, even if duplicate (to avoid nullptr crashes later)
        if (nameSym) nameSym->SetDecl(this);
    }

    // Predicates
    bool IsVar() override { return true; }

    // DeclNode API
    cDeclNode* GetType() override
    {
        cSymbol* t = GetTypeSym();
        if (t == nullptr || t->GetDecl() == nullptr) return nullptr;
        return t->GetDecl()->GetType();
    }

    std::string GetName() override
    {
        cSymbol* n = GetNameSym();
        return (n == nullptr) ? std::string("<unnamed>") : n->GetName();
    }

    virtual std::string NodeType() override { return std::string("var_decl"); }
    virtual void Visit(cVisitor* visitor) override { visitor->Visit(this); }

    cSymbol* GetTypeSym() { return static_cast<cSymbol*>(GetChild(0)); }
    cSymbol* GetNameSym() { return static_cast<cSymbol*>(GetChild(1)); }
};
