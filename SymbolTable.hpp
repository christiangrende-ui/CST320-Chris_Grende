#pragma once
#ifndef SYMBOLTABLE_HPP
#define SYMBOLTABLE_HPP

#include <memory>
#include <string>

class Node;        // forward declare
class SymbolTable; // forward declare for the extern

extern std::shared_ptr<SymbolTable> g_symbolTable;

class SymbolTable
{
public:
    static std::shared_ptr<SymbolTable> createInstance();

    virtual std::shared_ptr<Node> lookup(const std::string& name) = 0;
    virtual void insert(const std::string& name, const std::shared_ptr<Node> node) = 0;

    virtual ~SymbolTable() = default;
};

#endif // SYMBOLTABLE_HPP
