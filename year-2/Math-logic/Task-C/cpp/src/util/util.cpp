#include "util.h"


// functions
variable* vector_find_variable(std::vector<variable *> *v, variable *var) {
    for (auto &e : *v) {
        if (e->get_name() == var->get_name()) {
            return e;
        }
    }
    return nullptr;
}
