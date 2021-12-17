#include "axiom.h"

#include <cstdarg>

#include "../expression/conjunction.h"
#include "../expression/disjunction.h"
#include "../expression/implication.h"
#include "../expression/negation.h"


static bool is_axiom_number(expression *expr, uint32_t number);

static axiom *create_axiom_number(uint32_t number, ...);


static const std::vector<std::string> axiom_strings = {
        /*0*/ "",
        /*1*/ "A->(B->A)",
        /*2*/ "(A->B)->((A->B->C)->(A->C))",
        /*3*/ "A->(B->(A&B))",
        /*4*/ "(A&B)->A",
        /*5*/ "(A&B)->B",
        /*6*/ "A->(A|B)",
        /*7*/ "B->(A|B)",
        /*8*/ "(A->C)->((B->C)->((A|B)->C))",
        /*9*/ "(A->B)->((A->(!B))->(!C))",
        /*10*/ "(!!A)->A",
};

// if returns 0 - not found
uint32_t find_axiom_number(expression *expr) {
    size_t number = 1;
    bool found = false;
    for (; !found && number < axiom_strings.size(); ++number)
        found = is_axiom_number(expr, number);
    return found ? --number : 0;
}

axiom *create_axiom(uint32_t number, ...) {
    va_list args;
    va_start(args, number);
    axiom *a = create_axiom_number(number, args);
    va_end(args);
    return a;
}

static axiom *create_axiom_number(uint32_t number, ...) {
    axiom *ax;

    va_list args;
    va_start(args, number);
    switch (number) {
        case (1): {
            auto *a = va_arg(args, expression*);
            auto *b = va_arg(args, expression*);

            ax = new axiom(new implication(a, new implication(b, a)), number);
            break;
        }
        case (2) : {
            auto *a = va_arg(args, expression*);
            auto *b = va_arg(args, expression*);
            auto *c = va_arg(args, expression*);

            ax = new axiom(
                    new implication(
                            new implication(a, b),
                            new implication(
                                    new implication(a, new implication(b, c)),
                                    new implication(a, c)
                            )
                    ), number);
            break;
        }
        case (3) : {
            auto *a = va_arg(args, expression*);
            auto *b = va_arg(args, expression*);

            ax = new axiom(new implication(a, new implication(b, new conjunction(a, b))), number);
            break;
        }
        case (4) : {
            auto *a = va_arg(args, expression*);
            auto *b = va_arg(args, expression*);

            ax = new axiom(new implication(new conjunction(a, b), a), number);
            break;
        }
        case (5) : {
            auto *a = va_arg(args, expression*);
            auto *b = va_arg(args, expression*);

            ax = new axiom(new implication(new conjunction(a, b), b), number);
            break;
        }
        case (6) : {
            auto *a = va_arg(args, expression*);
            auto *b = va_arg(args, expression*);

            ax = new axiom(new implication(a, new disjunction(a, b)), number);
            break;
        }
        case (7) : {
            auto *a = va_arg(args, expression*);
            auto *b = va_arg(args, expression*);

            ax = new axiom(new implication(b, new disjunction(a, b)), number);
            break;
        }
        case (8) : {
            auto *a = va_arg(args, expression*);
            auto *b = va_arg(args, expression*);
            auto *c = va_arg(args, expression*);
            ax = new axiom(
                    new implication(
                            new implication(a, c),
                            new implication(
                                    new implication(b, c),
                                    new implication(new disjunction(a, b), c)
                            )
                    ), number);
            break;
        }
        case (9) : {
            auto *a = va_arg(args, expression*);
            auto *b = va_arg(args, expression*);
            auto *c = va_arg(args, expression*);

            ax = new axiom(
                    new implication(
                            new implication(a, b),
                            new implication(
                                    new implication(a, new negation(b)),
                                    new negation(c)
                            )
                    ), number);
            break;
        }
        case (10) : {
            auto *a = va_arg(args, expression*);

            ax = new axiom(new implication(new negation(new negation(a)), a), number);
            break;
        }
        default : {
            ax = nullptr;
            break;
        }
    }
    va_end(args);
    return ax;
}

static bool is_axiom_number(expression *expr, uint32_t number) {
    switch (number) {
        case 1: {
            if (expr->get_type() != expr_type::IMPLICATION) return false;
            auto *imp = ((implication*) expr);

            auto *right = imp->get_right();
            if (right->get_type() != expr_type::IMPLICATION) return false;

            return ((implication*)right)->get_right()->equals(imp->get_left());
        }
        case 2: {
            if (expr->get_type() != expr_type::IMPLICATION) return false;
            auto *imp = (implication*) expr;

            if (imp->get_right()->get_type() != expr_type::IMPLICATION) return false;
            auto *imp_right = (implication*)(imp->get_right());

            if (imp->get_left()->get_type() != expr_type::IMPLICATION ||
                imp_right->get_left()->get_type() != expr_type::IMPLICATION ||
                imp_right->get_right()->get_type() != expr_type::IMPLICATION) return false;
            auto *first_br = (implication*)(imp->get_left());
            auto *second_br = (implication*)(imp_right->get_left());
            auto *third_br = (implication*)(imp_right->get_right());

            if (second_br->get_right()->get_type() != expr_type::IMPLICATION) return false;
            auto *second_inner_right = ((implication*)(second_br->get_right()));

            expression *a1 = first_br->get_left();
            expression *a2 = second_br->get_left();
            expression *a3 = third_br->get_left();
            if (!a1->equals(a2) || !a2->equals(a3)) return false;

            expression *b1 = first_br->get_right();
            expression *b2 = second_inner_right->get_left();
            if (!b1->equals(b2)) return false;

            expression *c1 = second_inner_right->get_right();
            expression *c2 = third_br->get_right();

            return c1->equals(c2);
            break;
        }
        case 3: {
            if (expr->get_type() != expr_type::IMPLICATION) return false;
            auto *imp = (implication*)expr;

            if (imp->get_right()->get_type() != expr_type::IMPLICATION) return false;
            auto *right = (implication*)(imp->get_right());

            if (right->get_right()->get_type() != expr_type::CONJUNCTION) return false;
            auto *right_right = (implication*)(right->get_right());

            auto *a1 = imp->get_left();
            auto *a2 = right_right->get_left();
            if (!a1->equals(a2)) return false;

            return right->get_left()->equals(right_right->get_right());
        }
        case 4: {
            if (expr->get_type() != expr_type::IMPLICATION) return false;
            auto *imp = (implication*)(expr);

            if (imp->get_left()->get_type() != expr_type::CONJUNCTION) return false;
            auto *left = (conjunction*)(imp->get_left());

            auto *a1 = left->get_left();
            auto *a2 = imp->get_right();

            return a1->equals(a2);
        }
        case 5: {
            if (expr->get_type() != expr_type::IMPLICATION) return false;
            auto *imp = (implication*)(expr);

            if (imp->get_left()->get_type() != expr_type::CONJUNCTION) return false;
            auto *left = (conjunction*)(imp->get_left());
            auto *b1 = left->get_right();
            auto *b2 = imp->get_right();

            return b1->equals(b2);
        }
        case 6: {
            if (expr->get_type() != expr_type::IMPLICATION) return false;
            auto *imp = (implication*)(expr);

            if (imp->get_right()->get_type() != expr_type::DISJUNCTION) return false;
            auto *right = (disjunction*)(imp->get_right());

            return imp->get_left()->equals(right->get_left());
        }
        case 7: {
            if (expr->get_type() != expr_type::IMPLICATION) return false;
            auto *imp = (implication*)(expr);

            if (imp->get_right()->get_type() != expr_type::DISJUNCTION) return false;
            auto *right = (disjunction*)(imp->get_right());

            return imp->get_left()->equals(right->get_right());
        }
        case 8: {
            if (expr->get_type() != expr_type::IMPLICATION) return false;
            auto *imp = (implication*) expr;

            if (imp->get_right()->get_type() != expr_type::IMPLICATION) return false;
            auto *right = (implication*)(imp->get_right());

            if (imp->get_left()->get_type() != expr_type::IMPLICATION ||
            right->get_left()->get_type() != expr_type::IMPLICATION ||
            right->get_right()->get_type() != expr_type::IMPLICATION) return false;
            auto *first_br = (implication*)(imp->get_left());
            auto *second_br = (implication*)(right->get_left());
            auto *third_br = (implication*)(right->get_right());

            if (third_br->get_left()->get_type() != expr_type::DISJUNCTION) return false;
            auto *third_br_left = (disjunction*)(third_br->get_left());

            auto *a1 = first_br->get_left();
            auto *a2 = third_br_left->get_left();
            if (!a1->equals(a2)) return false;

            auto *b1 = second_br->get_left();
            auto *b2 = third_br_left->get_right();
            if (!b1->equals(b2)) return false;

            auto *c1 = first_br->get_right();
            auto *c2 = second_br->get_right();
            auto *c3 = third_br->get_right();
            return c1->equals(c2) && c2->equals(c3);
        }
        case 9: {
            if (expr->get_type() != expr_type::IMPLICATION) return false;
            auto *imp = (implication*)(expr);

            if (imp->get_right()->get_type() != expr_type::IMPLICATION) return false;
            auto *right = (implication*)(imp->get_right());

            if (imp->get_left()->get_type() != expr_type::IMPLICATION ||
                right->get_left()->get_type() != expr_type::IMPLICATION ||
                right->get_right()->get_type() != expr_type::NEGATION) return false;
            auto *first_br = (implication*)(imp->get_left());
            auto *second_br = (implication*)(right->get_left());
            auto *third_br = (negation*)(right->get_right());

            auto *a1 = first_br->get_left();
            auto *a2 = second_br->get_left();
            if (!a1->equals(a2)) return false;

            if (second_br->get_right()->get_type() != expr_type::NEGATION) return false;
            auto *b2_neg = (negation*)(second_br->get_right());

            auto *b1 = first_br->get_right();
            auto *b2 = b2_neg->get_expr();

            return b1->equals(b2);
        }
        case 10: {
            if (expr->get_type() != expr_type::IMPLICATION) return false;
            auto *imp = (implication*)(expr);

            if (imp->get_left()->get_type() != expr_type::NEGATION) return false;
            auto *a_neg_neg = (negation*)(imp->get_left());

            if (a_neg_neg->get_expr()->get_type() != expr_type::NEGATION) return false;
            auto *a_neg = (negation*)(a_neg_neg->get_expr());

            return a_neg->get_expr()->equals(imp->get_right());
        }
        default:
            return false;
    }
}
