#pragma once
//**************************************
// cFloatExprNode.h
//
// Defines an AST node for a float constant (literals).
//

#include <sstream>
#include <string>

#include "cExprNode.h"

class cFloatExprNode : public cExprNode
{
public:
    explicit cFloatExprNode(float value) : cExprNode(), m_value(value) {}

protected:
    virtual std::string AttributesToString() override
    {
        // Keep default formatting reasonably stable.
        std::ostringstream out;
        out << m_value;
        return " value=\"" + out.str() + "\"";
    }

    virtual std::string NodeType() override { return std::string("float"); }
    virtual void Visit(cVisitor* visitor) override { visitor->Visit(this); }

private:
    float m_value;
};
