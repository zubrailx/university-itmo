#include "hipothesis.h"

bool is_in_hipothesis_vector(std::list<expression *> *v, expression *expr) {
    bool found = false;
    for (const auto& h : *v) {
        if (found) break;
        found = expr->equals(h);
    }
    return found;
}
