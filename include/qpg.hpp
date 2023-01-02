#pragma once

#include "ast.hpp"
#include <iostream>
#include <string>

std::unique_ptr<Ast> parse_input(const std::string& str);
