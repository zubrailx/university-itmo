#pragma once

#include <qpg/ast.hpp>
#include <iostream>
#include <string>

struct AstWrapper {
  AstList<Ast>* list = nullptr;
  std::string err_msg;

  ~AstWrapper() {
    if (list) {
      delete list;
    }
  }
};

int parse_command(const std::string &str, AstWrapper &res);
