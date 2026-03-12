#pragma once
#include <vector>
#include <string>

#include "cVisitor.h"
#include "cAstNode.h"

class cCodeGenVisitor : public cVisitor
{
public:
    cCodeGenVisitor() = default;
    void VisitAllNodes(cAstNode* node) override;

    void Visit(cProgramNode* node) override;
    void Visit(cFuncDeclNode* node) override;

    void Visit(cBlockNode* node) override;
    void Visit(cStmtsNode* node) override;
    void Visit(cStmtBlockNode* node) override;

    void Visit(cAssignNode* node) override;
    void Visit(cIfNode* node) override;
    void Visit(cWhileNode* node) override;
    void Visit(cReturnNode* node) override;
    void Visit(cPrintNode* node) override;
    void Visit(cPrintsNode* node) override;
    void Visit(cCallStmtNode* node) override;

    void Visit(cBinaryExprNode* node) override;
    void Visit(cVarExprNode* node) override;
    void Visit(cIntExprNode* node) override;
    void Visit(cCallExprNode* node) override;

private:
    std::vector<cFuncDeclNode*> m_funcStack;

    void EmitLine(const std::string& s);
    void EmitLabel(const std::string& label);

    void EmitExpr(cExprNode* node);
    void EmitCall(cSymbol* fn, cArgsNode* args, bool discardReturn);
    void EmitAddress(cVarExprNode* node);
    void EmitLoadFromAddress(int sizeBytes);
    void EmitStoreToAddress(int sizeBytes);

    void EmitCleanupArgs(int count, bool discardReturn);
    int  CountArgs(cArgsNode* args);
};
