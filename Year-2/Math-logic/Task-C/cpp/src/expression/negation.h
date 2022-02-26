#ifndef TASK_C_NEGATION_H
#define TASK_C_NEGATION_H

#include "expression.h"

class negation : public expression {
private:
    expression *_expr;

public:
    negation(expression *expr) : _expr(expr) {
        type = expr_type::NEGATION;
        _hash = ~(expr->hashcode());
    }


    expression* get_expr() {
        return _expr;
    }


    bool calc() override { return !_expr->calc(); }


    std::string to_string() const override {
        return ("(!" + _expr->to_string() + ")");
    }

    bool equals(expression *expr) override {
        if (expr->hashcode() == this->hashcode()) {
            return this->to_string() == expr->to_string();
        } else {
            return false;
        }
    }

};


#endif //TASK_C_NEGATION_H
