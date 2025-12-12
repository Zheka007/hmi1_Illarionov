#include "VariableDatabase.hpp"

namespace xsmall_hmi {

void VariableDatabase::setVariable(const std::string& name, const ValueType& value) {
    variables_[name] = value;
    
    auto range = callbacks_.equal_range(name);
    for (auto it = range.first; it != range.second; ++it) {
        it->second(name, value);
    }
}

std::optional<VariableDatabase::ValueType> 
VariableDatabase::getVariable(const std::string& name) const {
    auto it = variables_.find(name);
    if (it != variables_.end()) {
        return it->second;
    }
    return std::nullopt;
}

bool VariableDatabase::hasVariable(const std::string& name) const {
    return variables_.find(name) != variables_.end();
}

void VariableDatabase::removeVariable(const std::string& name) {
    variables_.erase(name);
    callbacks_.erase(name);
}

void VariableDatabase::subscribe(const std::string& name, Callback callback) {
    callbacks_.emplace(name, std::move(callback));
}

} // namespace xsmall_hmi