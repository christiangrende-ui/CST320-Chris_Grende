#pragma once
//*******************************************************
// Declaration of the visitor class.
// As you add AST node types, add forward decl + Visit.
//

class cAstNode;
class cProgramNode;
class cBlockNode;
class cDeclsNode;
class cDeclNode;
class cVarDeclNode;
class cStructDeclNode;
class cArrayTypeDeclNode;
class cFuncDeclNode;

class cStmtsNode;
class cStmtNode;
class cAssignNode;
class cIfNode;
class cWhileNode;
class cReturnNode;
class cPrintNode;
class cPrintsNode;
class cStmtBlockNode;
class cCallStmtNode;

class cExprNode;
class cBinaryExprNode;
class cVarExprNode;
class cIntExprNode;
class cFloatExprNode;
class cOpNode;
class cCallExprNode;
class cCallExprPostfixNode;
class cArgsNode;
class cParamsNode;
class cLValueNode;
class cIndexExprNode;
class cDotExprNode;

class cSymbol;

class cVisitor
{
public:
    cVisitor() {}
    virtual ~cVisitor() = default;

    virtual void VisitAllNodes(cAstNode* node) = 0;

    virtual void Visit(cAstNode* node);
    virtual void Visit(cProgramNode* node);
    virtual void Visit(cBlockNode* node);
    virtual void Visit(cDeclsNode* node);

    virtual void Visit(cDeclNode* node);
    virtual void Visit(cVarDeclNode* node);
    virtual void Visit(cStructDeclNode* node);
    virtual void Visit(cArrayTypeDeclNode* node);
    virtual void Visit(cFuncDeclNode* node);

    virtual void Visit(cStmtsNode* node);
    virtual void Visit(cStmtNode* node);
    virtual void Visit(cAssignNode* node);
    virtual void Visit(cIfNode* node);
    virtual void Visit(cWhileNode* node);
    virtual void Visit(cReturnNode* node);
    virtual void Visit(cPrintNode* node);
    virtual void Visit(cPrintsNode* node);
    virtual void Visit(cStmtBlockNode* node);
    virtual void Visit(cCallStmtNode* node);

    virtual void Visit(cExprNode* node);
    virtual void Visit(cBinaryExprNode* node);
    virtual void Visit(cVarExprNode* node);
    virtual void Visit(cIntExprNode* node);
    virtual void Visit(cFloatExprNode* node);
    virtual void Visit(cOpNode* node);
    virtual void Visit(cCallExprNode* node);
    virtual void Visit(cCallExprPostfixNode* node);
    virtual void Visit(cArgsNode* node);
    virtual void Visit(cParamsNode* node);
    virtual void Visit(cLValueNode* node);
    virtual void Visit(cIndexExprNode* node);
    virtual void Visit(cDotExprNode* node);

    virtual void Visit(cSymbol* node);
};
