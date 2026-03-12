//**************************************************************
// cVisitor.cpp
//
// Default visitor that just visits all children.
//**************************************************************

#include "lex.h"
#include "cVisitor.h"

// IMPORTANT: makes cIntExprNode / cFloatExprNode complete types here
#include "astnodes.h"

void cVisitor::Visit(cAstNode* node)              { node->VisitAllChildren(this); }
void cVisitor::Visit(cProgramNode* node)          { node->VisitAllChildren(this); }
void cVisitor::Visit(cBlockNode* node)            { node->VisitAllChildren(this); }
void cVisitor::Visit(cDeclsNode* node)            { node->VisitAllChildren(this); }

void cVisitor::Visit(cDeclNode* node)             { node->VisitAllChildren(this); }
void cVisitor::Visit(cVarDeclNode* node)          { node->VisitAllChildren(this); }
void cVisitor::Visit(cStructDeclNode* node)       { node->VisitAllChildren(this); }
void cVisitor::Visit(cArrayTypeDeclNode* node)    { node->VisitAllChildren(this); }
void cVisitor::Visit(cFuncDeclNode* node)         { node->VisitAllChildren(this); }

void cVisitor::Visit(cStmtsNode* node)            { node->VisitAllChildren(this); }
void cVisitor::Visit(cStmtNode* node)             { node->VisitAllChildren(this); }
void cVisitor::Visit(cAssignNode* node)           { node->VisitAllChildren(this); }
void cVisitor::Visit(cIfNode* node)               { node->VisitAllChildren(this); }
void cVisitor::Visit(cWhileNode* node)            { node->VisitAllChildren(this); }
void cVisitor::Visit(cReturnNode* node)           { node->VisitAllChildren(this); }
void cVisitor::Visit(cPrintNode* node)            { node->VisitAllChildren(this); }
void cVisitor::Visit(cPrintsNode* node)           { node->VisitAllChildren(this); }
void cVisitor::Visit(cStmtBlockNode* node)        { node->VisitAllChildren(this); }
void cVisitor::Visit(cCallStmtNode* node)         { node->VisitAllChildren(this); }

void cVisitor::Visit(cExprNode* node)             { node->VisitAllChildren(this); }
void cVisitor::Visit(cBinaryExprNode* node)       { node->VisitAllChildren(this); }
void cVisitor::Visit(cVarExprNode* node)          { node->VisitAllChildren(this); }
void cVisitor::Visit(cIntExprNode* node)          { node->VisitAllChildren(this); }
void cVisitor::Visit(cFloatExprNode* node)        { node->VisitAllChildren(this); }
void cVisitor::Visit(cOpNode* node)               { node->VisitAllChildren(this); }
void cVisitor::Visit(cCallExprNode* node)         { node->VisitAllChildren(this); }
void cVisitor::Visit(cCallExprPostfixNode* node)  { node->VisitAllChildren(this); }
void cVisitor::Visit(cArgsNode* node)             { node->VisitAllChildren(this); }
void cVisitor::Visit(cParamsNode* node)           { node->VisitAllChildren(this); }
void cVisitor::Visit(cLValueNode* node)           { node->VisitAllChildren(this); }
void cVisitor::Visit(cIndexExprNode* node)        { node->VisitAllChildren(this); }
void cVisitor::Visit(cDotExprNode* node)          { node->VisitAllChildren(this); }

void cVisitor::Visit(cSymbol* node)               { node->VisitAllChildren(this); }