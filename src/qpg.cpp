#include <qpg.hpp>

extern "C" {
  #include "parser.h"
  #include "lexer.h"
}

void parse_input(std::string str) {
  yy_scan_string(str.c_str());
  yyparse();
}
