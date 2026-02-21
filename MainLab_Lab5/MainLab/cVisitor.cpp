//**************************************************************
// Implementation of the virtual Visitor class.
//

#include "lex.h"
#include "cVisitor.h"
#include "astnodes.h"

void cVisitor::Visit(cAstNode* node)          { node->VisitAllChildren(this); }
void cVisitor::Visit(cBlockNode* node)        { node->VisitAllChildren(this); }
void cVisitor::Visit(cDeclNode* node)         { node->VisitAllChildren(this); }
void cVisitor::Visit(cDeclsNode* node)        { node->VisitAllChildren(this); }
void cVisitor::Visit(cExprNode* node)         { node->VisitAllChildren(this); }
void cVisitor::Visit(cIntExprNode* node)      { node->VisitAllChildren(this); }
void cVisitor::Visit(cOpNode* node)           { node->VisitAllChildren(this); }
void cVisitor::Visit(cPrintNode* node)        { node->VisitAllChildren(this); }
void cVisitor::Visit(cProgramNode* node)      { node->VisitAllChildren(this); }
void cVisitor::Visit(cStmtNode* node)         { node->VisitAllChildren(this); }
void cVisitor::Visit(cStmtsNode* node)        { node->VisitAllChildren(this); }
void cVisitor::Visit(cSymbol* node)           { node->VisitAllChildren(this); }

// NEW (Lab 5A)
void cVisitor::Visit(cIfNode* node)           { node->VisitAllChildren(this); }
void cVisitor::Visit(cReturnNode* node)       { node->VisitAllChildren(this); }