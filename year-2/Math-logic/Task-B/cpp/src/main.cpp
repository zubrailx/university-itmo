#include <iostream>
#include <vector>
#include "expression.h"
#include "util.h"
#include "parser/expression.tab.h"
#include "parser/expression.lexer.h"

extern expression* result;
extern std::vector<variable *> vector_var;

int yywrap() {
    return 1;
}

void yyerror(const char *error) {
    std::cerr << error;
}

int main() {
    std::string expression;
    std::cin >> expression;
    yy_scan_string(expression.c_str());
    yyparse();
    struct options rez = resolve(result, &vector_var);
    std::cout << options_to_string(rez) << std::endl;
    return 0;
}
