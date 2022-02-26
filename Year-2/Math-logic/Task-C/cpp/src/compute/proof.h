#ifndef CPP_PROOF_H
#define  CPP_PROOF_H

#include "../expression/expression.h"
#include <list>

class proof;


bool is_in_proof_vector(std::list<proof*> *v, expression* expr);


class proof : public expression {
private:
    expression* _expr;
    std::size_t _line;
    bool _last_in_proof;

public:
    proof(expression* expr, std::size_t line) : _expr(expr), _line(line) {
        _last_in_proof = false;
        _hash = expr->hashcode();
    }
    proof(expression* expr, std::size_t line, bool last_in_proof) :
                                        _expr(expr), _line(line), _last_in_proof(last_in_proof) {
        _hash = expr->hashcode();
    }


    expression* get_expr() { return _expr; }

    std::size_t get_line() const { return _line; }

    bool is_last_in_proof() const { return _last_in_proof; }


    bool calc() override { return _expr->calc(); }


    std::string to_string() const override { return _expr->to_string(); }

    bool equals(expression *expr) override {
        if (expr->hashcode() == _expr->hashcode()) {
            return this->to_string() == expr->to_string();
        } else {
            return false;
        }
    }

    // desctr
    ~proof() = default;;
};


#endif //CPP_PROOF_H
