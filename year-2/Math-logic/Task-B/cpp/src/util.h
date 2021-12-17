#include "expression.h"
#include <string>
#include <vector>
#include <cstddef>

struct options {
    std::size_t valid;
    std::size_t total;
};

variable* vector_find_variable(std::vector<variable *> *v, variable *var);

struct options resolve(expression *result, std::vector<variable *> *v); 

std::string options_to_string(struct options o);
