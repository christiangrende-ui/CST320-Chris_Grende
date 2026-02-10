//**************************************************************
// Implementation of the virtual Visitor class.
// This class must be subclassed (and made concrete) for all visitors
//

#include "lex.h"
#include "cVisitor.h"
#include "astnodes.h"

//******************************************
// All AST node types need a Visit method. 
// The code for each method is identical: simply visit all children
void cVisitor::Visit(cAstNode *node)          { node->VisitAllChildren(this); }
void cVisitor::Visit(cBlockNode *node)        { node->VisitAllChildren(this); }
void cVisitor::Visit(cDeclNode *node)         { node->VisitAllChildren(this); }
void cVisitor::Visit(cDeclsNode *node)        { node->VisitAllChildren(this); }
void cVisitor::Visit(cExprNode *node)         { node->VisitAllChildren(this); }
void cVisitor::Visit(cIntExprNode *node)      { node->VisitAllChildren(this); }
void cVisitor::Visit(cOpNode *node)           { node->VisitAllChildren(this); }
void cVisitor::Visit(cPrintNode *node)        { node->VisitAllChildren(this); }
void cVisitor::Visit(cProgramNode *node)      { node->VisitAllChildren(this); }
void cVisitor::Visit(cStmtNode *node)         { node->VisitAllChildren(this); }
void cVisitor::Visit(cStmtsNode *node)        { node->VisitAllChildren(this); }
void cVisitor::Visit(cSymbol *node)           { node->VisitAllChildren(this); }

