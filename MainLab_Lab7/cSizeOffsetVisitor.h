#pragma once
#include <vector>
#include <algorithm>

#include "cVisitor.h"
#include "cAstNode.h"

// Forward declarations ONLY (prevents circular include issues)
class cProgramNode;
class cBlockNode;
class cDeclsNode;

class cVarDeclNode;
class cStructDeclNode;
class cArrayTypeDeclNode;
class cFuncDeclNode;

class cVarExprNode;
class cArgsNode;
class cParamsNode;
class cCallExprNode;
class cCallStmtNode;

class cIntExprNode;
class cFloatExprNode;

class cSymbol;
class cDeclNode;

class cSizeOffsetVisitor : public cVisitor
{
public:
    cSizeOffsetVisitor() = default;

    void VisitAllNodes(cAstNode* node) override
    {
        if (node) node->Visit(this);
    }

    // Top-level structure
    void Visit(cProgramNode* node) override;
    void Visit(cBlockNode* node) override;
    void Visit(cDeclsNode* node) override;

    // Decls/types
    void Visit(cVarDeclNode* node) override;
    void Visit(cStructDeclNode* node) override;
    void Visit(cArrayTypeDeclNode* node) override;
    void Visit(cFuncDeclNode* node) override;

    // Expressions/calls
    void Visit(cIntExprNode* node) override;
    void Visit(cFloatExprNode* node) override;
    void Visit(cVarExprNode* node) override;
    void Visit(cArgsNode* node) override;   // actual params in calls
    void Visit(cParamsNode* node) override; // formal args in functions
    void Visit(cCallExprNode* node) override;
    void Visit(cCallStmtNode* node) override;

private:
    struct ScopeFrame
    {
        int cur{0};
        int high{0};
        int saved_cur{0};
    };

    std::vector<ScopeFrame> m_scopes;

    static int Align(int offset, int size)
    {
        if (size <= 1) return offset;
        int rem = offset % 4;
        return (rem == 0) ? offset : (offset + (4 - rem));
    }

    int& Cur()  { return m_scopes.back().cur; }
    int& High() { return m_scopes.back().high; }

    void PushScope()
    {
        m_scopes.push_back(ScopeFrame{0,0,0});
    }

    void PushScopeFromCurrent()
    {
        m_scopes.push_back(ScopeFrame{Cur(), High(), Cur()});
    }

    void PopScopeToParent()
    {
        ScopeFrame child = m_scopes.back();
        m_scopes.pop_back();
        if (!m_scopes.empty())
        {
            Cur()  = child.saved_cur;
            High() = std::max(High(), child.high);
        }
    }

    int TypeSizeFromSym(cSymbol* typeSym);
    int TypeSizeFromDecl(cDeclNode* decl);

    void ComputeStructLayout(cStructDeclNode* s);
    void ComputeArrayTypeSize(cArrayTypeDeclNode* a);

    void ComputeVarRefLayout(cVarExprNode* v);
};