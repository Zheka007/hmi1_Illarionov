#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <optional>
#include <variant>
#include <functional>

namespace xsmall_hmi {

class VariableDatabase {
public:
    using ValueType = std::variant<int, float, double, bool, std::string>;
    using Callback = std::function<void(const std::string&, const ValueType&)>;
    
    VariableDatabase() = default;
    
    void setVariable(const std::string& name, const ValueType& value);
    std::optional<ValueType> getVariable(const std::string& name) const;
    
    bool hasVariable(const std::string& name) const;
    void removeVariable(const std::string& name);
    
    void subscribe(const std::string& name, Callback callback);
    
    template<typename T>
    std::optional<T> getVariableAs(const std::string& name) const;

private:
    std::unordered_map<std::string, ValueType> variables_;
    std::unordered_multimap<std::string, Callback> callbacks_;
};

template<typename T>
std::optional<T> VariableDatabase::getVariableAs(const std::string& name) const {
    auto it = variables_.find(name);
    if (it != variables_.end()) {
        if (auto* value = std::get_if<T>(&it->second)) {
            return *value;
        }
    }
    return std::nullopt;
}

} // namespace xsmall_hmi