#pragma once
//**************************************
// cSymbolTable.h
//
// Simple scoped symbol table used by the lexer/parser.
// Keeps symbol pointers stable by owning them in a storage vector.
//**************************************

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "cSymbol.h"

class cSymbolTable
{
private:
    using Scope = std::unordered_map<std::string, cSymbol*>;

    std::vector<Scope> m_scopes;
    std::vector<std::unique_ptr<cSymbol>> m_storage;

public:
    cSymbolTable()
    {
        // Start with the global scope.
        m_scopes.emplace_back();
    }

    int GetScopeDepth() const { return static_cast<int>(m_scopes.size()); }

    void IncreaseScope() { m_scopes.emplace_back(); }

    void DecreaseScope()
    {
        // Never remove the global scope.
        if (m_scopes.size() > 1) m_scopes.pop_back();
    }

    // Look only in current (innermost) scope.
    cSymbol* LookupLocal(const std::string& name)
    {
        if (m_scopes.empty()) return nullptr;
        auto& top = m_scopes.back();
        auto it = top.find(name);
        return (it == top.end()) ? nullptr : it->second;
    }

    // Look in all scopes, from inner to outer.
    cSymbol* Lookup(const std::string& name)
    {
        for (auto it = m_scopes.rbegin(); it != m_scopes.rend(); ++it)
        {
            auto found = it->find(name);
            if (found != it->end()) return found->second;
        }
        return nullptr;
    }

    // Insert a symbol in the current scope (creating it if needed).
    // NOTE: This method intentionally returns an existing symbol if it already exists
    // in the current scope. Semantic checking (duplicate definitions, function merging, etc.)
    // is performed by the AST nodes / visitors per lab requirements.
    cSymbol* Insert(const std::string& name)
    {
        if (m_scopes.empty()) m_scopes.emplace_back();

        // If already in current scope, return it.
        if (cSymbol* existing = LookupLocal(name)) return existing;

        std::unique_ptr<cSymbol> sym(new cSymbol(name));
        cSymbol* raw = sym.get();
        m_storage.emplace_back(std::move(sym));
        m_scopes.back()[name] = raw;
        return raw;
    }

    // Insert into the global scope, regardless of current scope depth.
    cSymbol* InsertGlobal(const std::string& name)
    {
        if (m_scopes.empty()) m_scopes.emplace_back();

        auto& global = m_scopes.front();
        auto it = global.find(name);
        if (it != global.end()) return it->second;

        std::unique_ptr<cSymbol> sym(new cSymbol(name));
        cSymbol* raw = sym.get();
        m_storage.emplace_back(std::move(sym));
        global[name] = raw;
        return raw;
    }
};
