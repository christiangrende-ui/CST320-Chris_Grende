#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "cSymbol.hpp"

class cSymbolTable
{
private:
    // Each scope is a map name -> symbol pointer
    std::vector<std::unordered_map<std::string, cSymbol*>> m_scopes;

    // Own all symbols so pointers stay valid
    std::vector<std::unique_ptr<cSymbol>> m_storage;

    std::uint64_t m_nextId{1};

public:
    cSymbolTable()
    {
        // Start with global scope
        m_scopes.emplace_back();
    }

    void IncreaseScope()
    {
        m_scopes.emplace_back();
    }

    void DecreaseScope()
    {
        if (m_scopes.size() > 1)
        {
            m_scopes.pop_back();
        }
        // If size == 1, stay at global scope (don’t delete it)
    }

    // Look only in the current scope (top of stack)
    cSymbol* LookupLocal(const std::string& name)
    {
        if (m_scopes.empty()) return nullptr;

        auto& top = m_scopes.back();
        auto it = top.find(name);
        if (it != top.end())
            return it->second;

        return nullptr;
    }

    // Insert into current scope (top of stack)
    cSymbol* Insert(const std::string& name)
    {
        if (m_scopes.empty())
            m_scopes.emplace_back();

        auto sym = std::make_unique<cSymbol>(m_nextId++, name, static_cast<int>(m_scopes.size() - 1));
        cSymbol* raw = sym.get();

        m_storage.emplace_back(std::move(sym));
        m_scopes.back()[name] = raw;

        return raw;
    }
};
