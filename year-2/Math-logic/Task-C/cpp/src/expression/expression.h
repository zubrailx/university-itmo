#ifndef CPP_EXPRESSION_H
#define CPP_EXPRESSION_H

#include <iostream>
#include <string>
#include <unordered_map>

static std::hash<std::string> hasher;

enum class expr_type {
    VARIABLE = 0,
    CONJUNCTION,
    DISJUNCTION,
    IMPLICATION,
    NEGATION
};

class expression {
protected:
    expr_type type;
    size_t _hash;

public:
    expr_type get_type() {
        return type;
    }
    size_t hashcode() const { return _hash; }

    virtual bool calc() = 0;

    virtual std::string to_string() const = 0;

    virtual bool equals(expression *expr) = 0;


    virtual ~expression() = default;;
};


#endif //CPP_EXPRESSION_H
