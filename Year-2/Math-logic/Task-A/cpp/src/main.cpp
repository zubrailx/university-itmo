#include <iostream>
#include "expression.h"
#include "parser/expression.tab.h"
#include "parser/expression.lexer.h"

extern expression* result;

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
    std::cout << result->print() << std::endl;
    return 0;
}
