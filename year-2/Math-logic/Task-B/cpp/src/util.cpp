#include "util.h"
#include <cmath>
#include <string>

static void set_values_to_variables(std::vector<variable *> *v, size_t value) {
    for (std::size_t i = 0; i < v->size(); ++i) {
        ((*v)[i])->set_value(value % 2);
        value /= 2;
    }
}

variable* vector_find_variable(std::vector<variable *> *v, variable *var) {
    for (auto &e : *v) {
        if (e->get_name().compare(var->get_name()) == 0) {
            return e;
        }
    }
    return NULL; 
}

struct options resolve(expression *result, std::vector<variable *> *v) {
    struct options rez = {.valid = 0, .total = (std::size_t) pow(2, v->size())};
    for (std::size_t i = 0; i < rez.total; ++i) {
        set_values_to_variables(v, i);
        rez.valid += result->calc();
    }
    return rez;
}

std::string options_to_string(struct options o) {
    if (o.valid == o.total) {
        return ("Valid");
    } else if (o.valid == 0) {
        return ("Unsatisfiable");
    } else {
        return "Satisfiable and invalid, " + std::to_string(o.valid) + " true and " + 
            std::to_string(o.total - o.valid) + " false cases";
    }
}
