#ifndef CPP_SOLUTION_EXPRESSION_H
#define CPP_SOLUTION_EXPRESSION_H

#include <iostream>
#include <string>

class expression {
public:
    virtual std::string print() = 0;
    virtual bool calc() = 0;

    virtual ~expression() = default;;
};

class variable : public expression {
private:
    std::string _name;
    bool _value;

public:
    // want be modified for caller
    variable(std::string &name) : _name(name) {
    }

    std::string get_name() {
        return _name;
    }

    void set_value(bool value) { 
        _value = value;
    }

    std::string print() override {
        std::cout << _value << " ";
        return _name;
    }

    bool calc() override { return _value; }
};

class implication : public expression {
private:
    expression *_left;
    expression *_right;

public:
    implication(expression *left, expression *right) : _left(left), _right(right) {
    }

    std::string print() override {
        return ("(->," + _left->print() + "," + _right->print() + ")");
    }

    bool calc() override { return _left->calc() <= _right->calc(); }
};

class disjunction : public expression {
private:
    expression *_left;
    expression *_right;

public:
    disjunction(expression *left, expression *right) : _left(left), _right(right) {
    }

    std::string print() override {
        return ("(|," + _left->print() + "," + _right->print() + ")");
    }

    bool calc() override { return _left->calc() || _right->calc(); }
};

class conjunction : public expression {
private:
    expression *_left;
    expression *_right;

public:
    conjunction(expression *left, expression *right) : _left(left), _right(right) {
    }

    std::string print() override {
        return ("(&," + _left->print() + "," + _right->print() + ")");
    }

    bool calc() override { return _left->calc() && _right->calc(); }
};

class negation : public expression {
private:
    expression *_expr;

public:
    negation(expression *expr) : _expr(expr) {
    }

    std::string print() override {
        return ("(!" + _expr->print() + ")");
    }

    bool calc() override { return !_expr->calc(); }
};

#endif //CPP_SOLUTION_EXPRESSION_H
