#include "../include/scope_handler.h"
#include <unordered_map>

void scope_handler::add_scope() {
    scope_stack.push_back(std::unordered_map<std::string, int>());
}

void scope_handler::remove_scope() {
    scope_stack.pop_back();
}

int scope_handler::add_var(std::string name) {
    int total_var = 0;
    for (auto &scope : scope_stack) {
        total_var += scope.size();
    }

    int offset = -8 * (total_var + 1);
    scope_stack.back()[name] = offset;
    return offset;
}

std::optional<int> scope_handler::get_var(std::string var_name, int depth) {

    for (auto it = scope_stack.rbegin() + depth; it != scope_stack.rend(); it++) {
        auto &var_table = *it;
        if (var_table.find(var_name) == var_table.end()) {
            continue;
        }

        return var_table[var_name];
    }

    return std::nullopt;
}

bool scope_handler::var_exists_curr_scope(std::string name) {
    if (scope_stack.back().find(name) == scope_stack.back().end()) {
        return false;
    }
    return true;
}

int scope_handler::get_current_depth() {
    return scope_stack.size();
}
