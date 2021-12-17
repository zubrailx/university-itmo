#ifndef CPP_AXIOM
#define CPP_AXIOM

#include <vector>
#include "../expression/expression.h"

class axiom;


uint32_t find_axiom_number(expression *expr);
axiom* create_axiom(uint32_t number, ...);


class axiom : public expression {
private:
    expression* _expr;
    uint32_t _number;

public:
    axiom(expression* expr, uint32_t number) : _expr(expr), _number(number) {
        _hash = expr->hashcode();
    }


    expression* get_expr() { return _expr; }

    uint32_t get_number() const { return _number; }


    bool calc() override { return true; }

    std::string to_string() const override { return _expr->to_string(); }

    bool equals(expression *expr) override {
        if (expr->hashcode() == _expr->hashcode()) {
            return this->to_string() == expr->to_string();
        } else {
            return false;
        }
    }


    ~axiom() override = default;;
};
#endif // CPP_AXIOM
