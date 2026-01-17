#pragma once
#include <cstdint>
#include <string>

class cSymbol
{
private:
    std::uint64_t m_id{};
    std::string   m_name;
    int           m_scopeLevel{};

public:
    cSymbol(std::uint64_t id, std::string name, int scopeLevel)
        : m_id(id), m_name(std::move(name)), m_scopeLevel(scopeLevel)
    {}

    std::uint64_t GetId() const { return m_id; }
    const std::string& GetName() const { return m_name; }
    int GetScopeLevel() const { return m_scopeLevel; }
};
