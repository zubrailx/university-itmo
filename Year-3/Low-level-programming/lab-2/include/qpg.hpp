#pragma once

#include "ast.hpp"
#include <iostream>
#include <string>

struct AstWrapper {
  AstList<Ast>* list = nullptr;

  ~AstWrapper() {
    if (list) {
      delete list;
    }
  }
};

int parse_input(const std::string &str, AstWrapper &res);
