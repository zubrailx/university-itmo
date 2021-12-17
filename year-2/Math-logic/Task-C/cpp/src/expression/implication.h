#ifndef TASK_C_IMPLICATION_H
#define TASK_C_IMPLICATION_H

#include "expression.h"


class implication : public expression {
private:
    expression *_left;
    expression *_right;

public:
    implication(expression *left, expression *right) : _left(left), _right(right) {
        type = expr_type::IMPLICATION;
        _hash = _left->hashcode() * _right->hashcode() * 71;
    }


    expression* get_left() {
        return _left;
    }

    expression* get_right() {
        return _right;
    }


    bool calc() override { return _left->calc() <= _right->calc(); }


    std::string to_string() const override {
        return "(" + _left->to_string() + "->" + _right->to_string() + ")";
    }

    bool equals(expression* expr) override {
        if (expr->hashcode() == this->hashcode()) {
            return this->to_string() == expr->to_string();
        } else {
            return false;
        }
    }

};


#endif //TASK_C_IMPLICATION_H
