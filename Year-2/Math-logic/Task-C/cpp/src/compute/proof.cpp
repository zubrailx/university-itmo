#include "proof.h"


bool is_in_proof_vector(std::list<proof*> *v, expression* expr) {
    bool found = false;
    for (const auto& p : *v) {
        if (found) break;
        found = expr->equals(p);
    }

    return found;
}
