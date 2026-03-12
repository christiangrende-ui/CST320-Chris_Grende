#pragma once
//**************************************
// cProgramNode.h
//
// Defines AST node for a lang program
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cAstNode.h"
#include <string>
#include "cBlockNode.h"

class cProgramNode : public cAstNode
{
    public:
        int GetSize() const { return m_size; }
        void SetSize(int sz) { m_size = sz; }

    public:
        // param is the block that makes up the program
        cProgramNode(cBlockNode *block) : cAstNode()
        {
            AddChild(block);
        }

        virtual string NodeType() { return string("program"); }
        virtual string AttributesToString() override
        {
            // Lab 6 requires size to always be present (even when 0)
            return string(" size=\"") + std::to_string(m_size) + "\"";
        }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
private:
        int m_size{0};
};