#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

class scope_handler {
  public:
    void add_scope();
    void remove_scope();

    void add_var(std::string name);

    std::optional<int> get_var(std::string var_name, int depth = 0);
    bool var_exists_curr_scope(std::string name);
    int get_current_depth();

  private:
    std::vector<std::unordered_map<std::string, int>> scope_stack;
};
