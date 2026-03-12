#pragma once
//**************************************
// cDeclsNode.h
//
// Defines a class to represent a list of declarations.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cAstNode.h"
#include <string>
#include "cDeclNode.h"

class cDeclsNode : public cAstNode
{
    public:
        int GetSize() const { return m_size; }
        void SetSize(int sz) { m_size = sz; }

    public:
        // param is the first decl in this decls
        cDeclsNode(cDeclNode *decl) : cAstNode()
        {
            AddChild(decl);
        }

        // Add a decl to the list
        void Insert(cDeclNode *decl)
        {
            AddChild(decl);
        }

        virtual string NodeType() { return string("decls"); }
        virtual string AttributesToString() override
        {
            // Lab 6 requires size to always be present (even when 0)
            return string(" size=\"") + std::to_string(m_size) + "\"";
        }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
private:
        int m_size{0};
};