#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include "Node.hpp"
#include "SymbolTable.hpp"

class SymbolTableImplementation : public SymbolTable, std::enable_shared_from_this<SymbolTableImplementation>
{
private:
    std::vector<std::shared_ptr<Node>> children;
    std::mutex children_mutex;

public:
    virtual ~SymbolTableImplementation() = default;

    virtual std::shared_ptr<Node> lookup(const std::string& name) override
    {
        std::lock_guard<std::mutex> guard(children_mutex);
        std::shared_ptr<Node> result = nullptr;
        for (auto& child : children)
        {
    if (child->getName() == name)
            {
                return child;
            }
            result = child->lookup(name);
            if (result)
            {
                break;
            }
        }
        return result;       
    }

    virtual void insert(const std::string& name, const std::shared_ptr<Node> node) override
    {
        std::lock_guard<std::mutex> guard(children_mutex);
        children.emplace_back(node);
    }

};

std::shared_ptr<SymbolTable> SymbolTable::createInstance()
{
    return std::dynamic_pointer_cast<SymbolTable>(std::make_shared<SymbolTableImplementation>());
}
