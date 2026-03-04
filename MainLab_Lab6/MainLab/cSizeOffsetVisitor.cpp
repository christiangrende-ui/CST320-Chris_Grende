#include "cSizeOffsetVisitor.h"
#include "astnodes.h"

#include <algorithm>
#include <vector>

int cSizeOffsetVisitor::TypeSizeFromDecl(cDeclNode* decl)
{
    if (decl == nullptr) return 0;

    if (auto arr = dynamic_cast<cArrayTypeDeclNode*>(decl))
    {
        if (arr->GetSize() == 0) ComputeArrayTypeSize(arr);
        return arr->GetSize();
    }

    if (auto st = dynamic_cast<cStructDeclNode*>(decl))
    {
        if (st->GetSize() == 0) ComputeStructLayout(st);
        return st->GetSize();
    }

    return decl->GetSize();
}

int cSizeOffsetVisitor::TypeSizeFromSym(cSymbol* typeSym)
{
    if (typeSym == nullptr) return 0;
    return TypeSizeFromDecl(typeSym->GetDecl());
}

void cSizeOffsetVisitor::Visit(cProgramNode* node)
{
    PushScope();
    node->VisitAllChildren(this);
    node->SetSize(High());
    PopScopeToParent();
}

void cSizeOffsetVisitor::Visit(cBlockNode* node)
{
    if (m_scopes.empty()) PushScope();
    else PushScopeFromCurrent();

    node->VisitAllChildren(this);

    node->SetSize(High());
    PopScopeToParent();
}

void cSizeOffsetVisitor::Visit(cDeclsNode* node)
{
    node->VisitAllChildren(this);
    node->SetSize(High());
}

void cSizeOffsetVisitor::ComputeArrayTypeSize(cArrayTypeDeclNode* a)
{
    if (!a) return;
    int elemSize = TypeSizeFromSym(a->GetElemTypeSym());
    a->SetSize(elemSize * a->GetCount());
}

void cSizeOffsetVisitor::ComputeStructLayout(cStructDeclNode* s)
{
    if (!s) return;

    cDeclsNode* fields = s->GetFields();
    if (!fields)
    {
        s->SetSize(0);
        return;
    }

    int off = 0;
    int high = 0;

    for (int i = 0; i < fields->NumChildren(); i++)
    {
        cVarDeclNode* fld = dynamic_cast<cVarDeclNode*>(fields->GetChild(i));
        if (!fld) continue;

        int sz = TypeSizeFromSym(fld->GetTypeSym());
        fld->SetSize(sz);

        off = Align(off, sz);
        fld->SetOffset(off);
        off += sz;

        high = std::max(high, off);
    }

    fields->SetSize(high);
    s->SetSize(high);
    s->SetOffset(0);
}

void cSizeOffsetVisitor::Visit(cArrayTypeDeclNode* node)
{
    if (node->GetSize() == 0) ComputeArrayTypeSize(node);
}

void cSizeOffsetVisitor::Visit(cStructDeclNode* node)
{
    if (node->GetSize() == 0) ComputeStructLayout(node);
}

void cSizeOffsetVisitor::Visit(cVarDeclNode* node)
{
    int sz = TypeSizeFromSym(node->GetTypeSym());
    node->SetSize(sz);

    int off = Align(Cur(), sz);
    node->SetOffset(off);

    Cur() = off + sz;
    High() = std::max(High(), Cur());
}

void cSizeOffsetVisitor::Visit(cParamsNode* node)
{
    int offset = -12;
    int total = 0;

    for (int i = 0; i < node->NumChildren(); i++)
    {
        cVarDeclNode* p = dynamic_cast<cVarDeclNode*>(node->GetChild(i));
        if (!p) continue;

        int sz = TypeSizeFromSym(p->GetTypeSym());
        p->SetSize(sz);

        p->SetOffset(offset);
        offset -= sz;

        total += sz;
    }

    node->SetSize(total);
}

void cSizeOffsetVisitor::Visit(cFuncDeclNode* node)
{
    node->SetOffset(0);

    if (auto args = node->GetArgs())
        args->Visit(this);

    int parentCur = m_scopes.empty() ? 0 : Cur();
    int parentHigh = m_scopes.empty() ? 0 : High();

    PushScope();

    if (auto decls = node->GetDecls()) decls->Visit(this);
    if (auto stmts = node->GetStmts()) stmts->Visit(this);

    if (auto decls = node->GetDecls()) decls->SetSize(High());

    m_scopes.pop_back();

    if (!m_scopes.empty())
    {
        Cur() = parentCur;
        High() = parentHigh;
    }
}

void cSizeOffsetVisitor::ComputeVarRefLayout(cVarExprNode* v)
{
    if (!v) return;

    cSymbol* baseSym = v->GetSym();
    if (!baseSym || !baseSym->GetDecl()) return;

    cDeclNode* baseDecl = baseSym->GetDecl();
    cDeclNode* curType = baseDecl->GetType();

    int curOffset = baseDecl->GetOffset();
    int curSize   = TypeSizeFromDecl(curType);
    std::vector<int> rowSizes;

    for (int i = 1; i < v->NumChildren(); i++)
    {
        cAstNode* child = v->GetChild(i);

        if (dynamic_cast<cExprNode*>(child) != nullptr)
        {
            auto arr = dynamic_cast<cArrayTypeDeclNode*>(curType);
            if (!arr) break;

            int elemSize = TypeSizeFromSym(arr->GetElemTypeSym());
            rowSizes.push_back(elemSize);

            if (auto intNode = dynamic_cast<cIntExprNode*>(child))
                curOffset += intNode->GetValue() * elemSize;

            curType = arr->GetElemTypeSym() ? arr->GetElemTypeSym()->GetDecl() : nullptr;
            curSize = elemSize;
            continue;
        }

        if (auto fieldSym = dynamic_cast<cSymbol*>(child))
        {
            auto st = dynamic_cast<cStructDeclNode*>(curType);
            if (!st) break;

            if (st->GetSize() == 0) ComputeStructLayout(st);

            cDeclsNode* fields = st->GetFields();
            if (!fields) break;

            cVarDeclNode* fldDecl = nullptr;
            for (int j = 0; j < fields->NumChildren(); j++)
            {
                auto vd = dynamic_cast<cVarDeclNode*>(fields->GetChild(j));
                if (vd && vd->GetNameSym() && vd->GetNameSym()->GetName() == fieldSym->GetName())
                {
                    fldDecl = vd;
                    break;
                }
            }
            if (!fldDecl) break;

            curOffset += fldDecl->GetOffset();
            curType = fldDecl->GetType();
            curSize = TypeSizeFromDecl(curType);
        }
    }

    v->SetOffset(curOffset);
    v->SetSize(curSize);
    v->SetRowSizes(rowSizes);
}

void cSizeOffsetVisitor::Visit(cVarExprNode* node)
{
    node->VisitAllChildren(this);
    ComputeVarRefLayout(node);
}

void cSizeOffsetVisitor::Visit(cIntExprNode* node)
{
    // FORCE Lab 6 expectation: int literals are 4 bytes
    if (!node) return;
    node->SetSize(4);
}

void cSizeOffsetVisitor::Visit(cFloatExprNode* node)
{
    if (!node) return;
    // float literals are 4 bytes
    node->SetSize(4);
}

void cSizeOffsetVisitor::Visit(cArgsNode* node)
{
    node->VisitAllChildren(this);

    int total = 0;
    for (int i = 0; i < node->NumChildren(); i++)
    {
        cAstNode* ch = node->GetChild(i);

        if (auto v = dynamic_cast<cVarExprNode*>(ch)) total += v->GetSize();
        else if (auto iN = dynamic_cast<cIntExprNode*>(ch)) total += iN->GetSize();
        else if (auto fN = dynamic_cast<cFloatExprNode*>(ch)) total += fN->GetSize();
    }

    node->SetSize(total);
}

void cSizeOffsetVisitor::Visit(cCallExprNode* node)
{
    if (auto params = node->GetParams()) params->Visit(this);
}

void cSizeOffsetVisitor::Visit(cCallStmtNode* node)
{
    if (auto params = node->GetParams()) params->Visit(this);
}