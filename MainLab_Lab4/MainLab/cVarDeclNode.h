#pragma once
//**************************************
// cVarDeclNode.h
//
// Defines an AST node for a variable declaration.
#include <string>
//

#include "cDeclNode.h"
#include "cSymbol.h"

class cVarDeclNode : public cDeclNode
{
public:
    cVarDeclNode(cSymbol* typeSym, cSymbol* nameSym)
        : cDeclNode()
    {
        AddChild(typeSym);
        AddChild(nameSym);
    }

    virtual std::string NodeType() override { return std::string("var_decl"); }
    virtual void Visit(cVisitor* visitor) override { visitor->Visit(this); }

    cSymbol* GetType() { return static_cast<cSymbol*>(GetChild(0)); }
    cSymbol* GetName() { return static_cast<cSymbol*>(GetChild(1)); }
};
