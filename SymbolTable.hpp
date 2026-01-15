#pragma once
#include <memory>
#include <string>

class Node;

class SymbolTable
{
public:
    static std::shared_ptr<SymbolTable> createInstance();

    virtual std::shared_ptr<Node> lookup(const std::string& name) = 0;

    virtual void insert(const std::string& name, const std::shared_ptr<Node> node) = 0;

    virtual ~SymbolTable() = default;

};