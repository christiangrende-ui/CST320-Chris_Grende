#pragma once
#include <stdint.h>
#include <memory>
#include <string>
#include <cstdint>

class Node
{
public:
    enum class SymbolType
    {
        VARIABLE,
        FUNCTION,
        CLASS,
        NAMESPACE
    };

    virtual std::string getName() const = 0;
    virtual std::uint32_t getIdentifier() const = 0;
    virtual SymbolType getType() const = 0;
    virtual std::string getFile() const = 0;
    virtual std::uint32_t getLine() const = 0;
    virtual std::weak_ptr<Node> getParent() const = 0;
    virtual void addChild(const std::shared_ptr<Node> child) = 0;

    virtual ~Node() = default;

    virtual std::shared_ptr<Node> lookup(const std::string& name) = 0;   // instance lookup
    static std::shared_ptr<Node> lookup(const std::uint32_t identifier); // global lookup

    static std::shared_ptr<Node> createNode(const std::shared_ptr<Node> parent, const std::string& name,
        const SymbolType type, const std::string& file, const std::uint32_t line);
};