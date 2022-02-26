#ifndef CPP_SOLUTION_EXPRESSION_H
#define CPP_SOLUTION_EXPRESSION_H

#include <string>

class expression {
public:
    virtual std::string print() = 0;

    virtual ~expression() {};
};

class variable : public expression {
private:
    std::string _name;

public:
    // want be modified for caller
    variable(std::string &name) : _name(name) {
    }

    virtual std::string print() {
        return _name;
    }
};

class implication : public expression {
private:
    expression *_left;
    expression *_right;

public:
    implication(expression *left, expression *right) : _left(left), _right(right) {
    }

    virtual std::string print() {
        return ("(->," + _left->print() + "," + _right->print() + ")");
    }
};

class disjunction : public expression {
private:
    expression *_left;
    expression *_right;

public:
    disjunction(expression *left, expression *right) : _left(left), _right(right) {
    }

    virtual std::string print() {
        return ("(|," + _left->print() + "," + _right->print() + ")");
    }
};

class conjunction : public expression {
private:
    expression *_left;
    expression *_right;

public:
    conjunction(expression *left, expression *right) : _left(left), _right(right) {
    }

    virtual std::string print() {
        return ("(&," + _left->print() + "," + _right->print() + ")");
    }
};

class negation : public expression {
private:
    expression *_expr;

public:
    negation(expression *expr) : _expr(expr) {
    }

    virtual std::string print() {
        return ("(!" + _expr->print() + ")");
    }
};

#endif //CPP_SOLUTION_EXPRESSION_H
