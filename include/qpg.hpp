#pragma once

#include "ast.hpp"
#include <iostream>
#include <string>

struct AstWrapper {
  std::unique_ptr<Ast> ast;
};

int parse_input(const std::string &str, AstWrapper &res);
