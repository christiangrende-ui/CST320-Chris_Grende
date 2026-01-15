#include "Node.hpp"
#include "SymbolTable.hpp"
#include <iostream>
#include <memory>
#include <string>

#define ERROR_INVALID_ACCESS 12
#define ERROR_SUCCESS 0 // Best named constant ever

static std::shared_ptr<SymbolTable> symbol_table = SymbolTable::createInstance();

int main()
{
    std::cout << "Symbol Table Reference Implementation" << std::endl;

    std::shared_ptr<Node> child_node = Node::createNode(nullptr, "child", Node::SymbolType::VARIABLE, "file.cpp", 10);
    std::shared_ptr<Node> nested_node = Node::createNode(child_node, "nested", Node::SymbolType::VARIABLE, "file.cpp", 10);
    child_node->addChild(nested_node);
    symbol_table->insert("child", child_node);
    if (!symbol_table->lookup("nested"))
    {
        return ERROR_INVALID_ACCESS;
    }
    return ERROR_SUCCESS;
}
