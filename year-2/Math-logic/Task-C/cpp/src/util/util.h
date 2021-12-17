#ifndef CPP_UTIL_H
#define CPP_UTIL_H

#include <string>
#include <vector>
#include <cstddef>
#include <iostream>

#include "../expression/variable.h"


variable* vector_find_variable(std::vector<variable *> *v, variable *var);

#endif //CPP_UTIL_H
