#ifndef TASK_C_VARIABLE_H
#define TASK_C_VARIABLE_H

#include "expression.h"

class variable : public expression {
private:
    std::string _name;
    bool _value{};

public:
    // want to be modified for caller
    variable(std::string &name) : _name(name) {
        type = expr_type::VARIABLE;
        _hash = hasher(name);
    }


    std::string get_name() {
        return _name;
    }


    bool calc() override { return _value; }


    std::string to_string() const override {
        return _name;
    }

    bool equals(expression* expr) override {
        if (expr->hashcode() == this->hashcode()) {
            return this->to_string() == expr->to_string();
        }
        else {
            return false;
        }
    }
};


#endif //TASK_C_VARIABLE_H
