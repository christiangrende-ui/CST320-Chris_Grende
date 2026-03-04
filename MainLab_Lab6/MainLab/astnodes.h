#pragma once
//**************************************
// astnodes.h
//
// Single include for ALL AST node headers.
// Must include langparse.h first so token enums exist (EQUALS, NOT_EQUALS, etc.).
//**************************************

#include "langparse.h"   // <-- IMPORTANT: defines tokens + yylval_t

#include "cAstNode.h"
#include "cSymbol.h"

#include "cStmtNode.h"
#include "cStmtsNode.h"

#include "cDeclNode.h"
#include "cDeclsNode.h"

#include "cBlockNode.h"
#include "cProgramNode.h"

#include "cOpNode.h"
#include "cPrintNode.h"
#include "cPrintsNode.h"

#include "cExprNode.h"
#include "cIntExprNode.h"
#include "cFloatExprNode.h"
#include "cBinaryExprNode.h"

#include "cVarDeclNode.h"
#include "cVarExprNode.h"

#include "cReturnNode.h"
#include "cIfNode.h"
#include "cWhileNode.h"

#include "cParamNode.h"
#include "cParamsNode.h"
#include "cFuncDeclNode.h"

#include "cArgsNode.h"
#include "cCallExprNode.h"
#include "cCallStmtNode.h"
#include "cCallExprPostfixNode.h"

#include "cLValueNode.h"
#include "cAssignNode.h"

#include "cIndexExprNode.h"
#include "cDotExprNode.h"

#include "cStructDeclNode.h"
#include "cArrayTypeDeclNode.h"

#include "cStmtBlockNode.h"