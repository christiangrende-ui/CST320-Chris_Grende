#include "cCodeGenVisitor.h"

#include <sstream>

#include "astnodes.h"
#include "emit.h"

static std::string QuoteString(const std::string& s)
{
    std::string out("\"");
    for (char c : s)
    {
        switch (c)
        {
            case '\\': out += "\\\\"; break;
            case '"':  out += "\\\""; break;
            case '\n': out += "\\n"; break;
            case '\t': out += "\\t"; break;
            default:   out.push_back(c); break;
        }
    }
    out += "\"";
    return out;
}

void cCodeGenVisitor::VisitAllNodes(cAstNode* node)
{
    if (node) node->Visit(this);
}

void cCodeGenVisitor::EmitLine(const std::string& s)
{
    EmitString(s);
    EmitString("\n");
}

void cCodeGenVisitor::EmitLabel(const std::string& label)
{
    EmitString(label);
    EmitString(":\n");
}

int cCodeGenVisitor::CountArgs(cArgsNode* args)
{
    return (args == nullptr) ? 0 : args->NumChildren();
}

void cCodeGenVisitor::EmitCleanupArgs(int count, bool discardReturn)
{
    for (int i = 0; i < count; i++)
    {
        EmitLine("SWAP");
        EmitLine("POP");
    }

    if (discardReturn)
    {
        EmitLine("POP");
    }
}

void cCodeGenVisitor::EmitLoadFromAddress(int sizeBytes)
{
    if (sizeBytes == 1) EmitLine("PUSHCVARIND");
    else EmitLine("PUSHVARIND");
}

void cCodeGenVisitor::EmitStoreToAddress(int sizeBytes)
{
    if (sizeBytes == 1) EmitLine("POPCVARIND");
    else EmitLine("POPVARIND");
}

void cCodeGenVisitor::EmitAddress(cVarExprNode* node)
{
    if (node == nullptr) return;

    std::ostringstream out;
    out << "PUSH " << node->GetOffset();
    EmitLine(out.str());
    EmitLine("PUSHFP");
    EmitLine("PLUS");

    int rowIndex = 0;
    for (int i = 1; i < node->NumChildren(); i++)
    {
        cAstNode* child = node->GetChild(i);
        if (dynamic_cast<cExprNode*>(child) != nullptr)
        {
            EmitExpr(static_cast<cExprNode*>(child));
            int elemSize = 1;
            if (rowIndex < static_cast<int>(node->GetRowSizes().size()))
            {
                elemSize = node->GetRowSizes()[rowIndex];
            }

            std::ostringstream mul;
            mul << "PUSH " << elemSize;
            EmitLine(mul.str());
            EmitLine("TIMES");
            EmitLine("PLUS");
            rowIndex++;
        }
    }
}

void cCodeGenVisitor::EmitExpr(cExprNode* node)
{
    if (node != nullptr) node->Visit(this);
}

void cCodeGenVisitor::EmitCall(cSymbol* fn, cArgsNode* args, bool discardReturn)
{
    if (args != nullptr)
    {
        for (int i = args->NumChildren() - 1; i >= 0; i--)
        {
            EmitExpr(static_cast<cExprNode*>(args->GetChild(i)));
        }
    }

    EmitString("CALL @");
    EmitString(fn->GetName());
    EmitString("\n");

    EmitCleanupArgs(CountArgs(args), discardReturn);
}

void cCodeGenVisitor::Visit(cProgramNode* node)
{
    EmitLine("CALL @main");
    EmitLine("POP");

    cBlockNode* block = static_cast<cBlockNode*>(node->GetChild(0));
    cDeclsNode* decls = block ? block->GetDecls() : nullptr;
    if (decls != nullptr)
    {
        for (int i = 0; i < decls->NumChildren(); i++)
        {
            cFuncDeclNode* fn = dynamic_cast<cFuncDeclNode*>(decls->GetChild(i));
            if (fn != nullptr && fn->GetStmts() != nullptr)
            {
                fn->Visit(this);
            }
        }
    }

    EmitLine(".function main");
    EmitLabel("main");
    std::ostringstream adj;
    adj << "ADJSP " << block->GetSize();
    EmitLine(adj.str());

    if (block != nullptr && block->GetStmts() != nullptr)
    {
        block->GetStmts()->Visit(this);
    }

    EmitLine("PUSH 0");
    EmitLine("RETURNV");
}

void cCodeGenVisitor::Visit(cFuncDeclNode* node)
{
    if (node == nullptr || node->GetStmts() == nullptr) return;

    EmitString(".function ");
    EmitString(node->GetName());
    EmitString("\n");
    EmitLabel(node->GetName());

    int frameSize = 0;
    if (node->GetDecls() != nullptr) frameSize = node->GetDecls()->GetSize();

    std::ostringstream adj;
    adj << "ADJSP " << frameSize;
    EmitLine(adj.str());

    m_funcStack.push_back(node);
    node->GetStmts()->Visit(this);
    m_funcStack.pop_back();

    EmitLine("PUSH 0");
    EmitLine("RETURNV");
}

void cCodeGenVisitor::Visit(cBlockNode* node)
{
    if (node != nullptr && node->GetStmts() != nullptr) node->GetStmts()->Visit(this);
}

void cCodeGenVisitor::Visit(cStmtsNode* node)
{
    if (node != nullptr) node->VisitAllChildren(this);
}

void cCodeGenVisitor::Visit(cStmtBlockNode* node)
{
    if (node != nullptr) node->VisitAllChildren(this);
}

void cCodeGenVisitor::Visit(cAssignNode* node)
{
    cVarExprNode* lhs = static_cast<cVarExprNode*>(node->GetChild(0));
    cExprNode* rhs = static_cast<cExprNode*>(node->GetChild(1));

    EmitExpr(rhs);
    EmitLine("DUP");
    EmitAddress(lhs);
    EmitStoreToAddress(lhs->GetSize());
    EmitLine("POP");
}

void cCodeGenVisitor::Visit(cIfNode* node)
{
    std::string elseLabel = GenerateLabel();
    std::string endLabel = GenerateLabel();

    EmitExpr(static_cast<cExprNode*>(node->GetChild(0)));
    EmitLine("JUMPE @" + elseLabel);

    static_cast<cStmtNode*>(node->GetChild(1))->Visit(this);
    EmitLine("JUMP @" + endLabel);

    EmitLabel(elseLabel);
    if (node->NumChildren() > 2 && node->GetChild(2) != nullptr)
    {
        static_cast<cStmtNode*>(node->GetChild(2))->Visit(this);
    }

    EmitLabel(endLabel);
}

void cCodeGenVisitor::Visit(cWhileNode* node)
{
    std::string topLabel = GenerateLabel();
    std::string endLabel = GenerateLabel();

    EmitLabel(topLabel);
    EmitExpr(static_cast<cExprNode*>(node->GetChild(0)));
    EmitLine("JUMPE @" + endLabel);
    static_cast<cStmtNode*>(node->GetChild(1))->Visit(this);
    EmitLine("JUMP @" + topLabel);
    EmitLabel(endLabel);
}

void cCodeGenVisitor::Visit(cReturnNode* node)
{
    EmitExpr(static_cast<cExprNode*>(node->GetChild(0)));
    EmitLine("RETURNV");
}

void cCodeGenVisitor::Visit(cPrintNode* node)
{
    EmitExpr(static_cast<cExprNode*>(node->GetChild(0)));
    EmitLine("CALL @print");
    EmitLine("SWAP");
    EmitLine("POP");
    EmitLine("POP");
}

void cCodeGenVisitor::Visit(cPrintsNode* node)
{
    std::string label = GenerateLabel();
    EmitLine("PUSH @" + label);
    EmitLine(".dataseg");
    EmitLabel(label);
    EmitString(".string ");
    EmitString(QuoteString(node->GetText()));
    EmitString("\n");
    EmitLine(".codeseg");
    EmitLine("OUTS");
}

void cCodeGenVisitor::Visit(cCallStmtNode* node)
{
    EmitCall(node->GetFnSym(), node->GetParams(), true);
}

void cCodeGenVisitor::Visit(cBinaryExprNode* node)
{
    EmitExpr(node->GetLeft());
    EmitExpr(node->GetRight());

    switch (node->GetOp()->GetOp())
    {
        case '+': EmitLine("PLUS"); break;
        case '-': EmitLine("MINUS"); break;
        case '*': EmitLine("TIMES"); break;
        case '/': EmitLine("DIVIDE"); break;
        case '%': EmitLine("MOD"); break;
        case '<': EmitLine("LT"); break;
        case '>': EmitLine("GT"); break;
        case EQUALS: EmitLine("EQ"); break;
        case NOT_EQUALS:
            EmitLine("EQ");
            EmitLine("PUSH 0");
            EmitLine("EQ");
            break;
        case LE:
            EmitLine("GT");
            EmitLine("PUSH 0");
            EmitLine("EQ");
            break;
        case GE:
            EmitLine("LT");
            EmitLine("PUSH 0");
            EmitLine("EQ");
            break;
        case AND: EmitLine("AND"); break;
        case OR: EmitLine("OR"); break;
        default: break;
    }
}

void cCodeGenVisitor::Visit(cVarExprNode* node)
{
    EmitAddress(node);
    EmitLoadFromAddress(node->GetSize());
}

void cCodeGenVisitor::Visit(cIntExprNode* node)
{
    std::ostringstream out;
    out << "PUSH " << node->GetValue();
    EmitLine(out.str());
}

void cCodeGenVisitor::Visit(cCallExprNode* node)
{
    EmitCall(node->GetFnSym(), node->GetParams(), false);
}
