# Lab-2 QPG (query plan generator)

### Условия

* На равенство и неравенство для чисел, строк и булевских значений
* На строгие и нестрогие сравнения для чисел
* Существование подстроки
* Логическую комбинацию произвольного количества условий и булевских значений
* В качестве любого аргумента условий могут выступать литеральные значения (константы) или ссылки на значения, ассоциированные с элементами данных (поля, атрибуты, свойства)
* 

## Build
```sh
# Default build type - build

# Some aliases for cmake usage (read it to get actual commands)
./cmake.sh -h 

# Basic commands:
./cmake.sh gen [(debug|released|sanitized)] # generate build-tree + build (default to debug)

# Tests
./cmake.sh [build-type] {({test [sep]}|valgrind)}
# Sanitized
./cmake.sh gen sanitized && ./cmake.sh sanitized test

# Run executable (if app/main.c is present):
./build/(debug|released|sanitized)/bin/dbms_exec

# Crossdev
./cmake.sh released gen reset -v -DCMAKE_C_COMPILER=i686-w64-mingw32-gcc -DCMAKE_CXX_COMPILER=i686-w64-mingw32-g++

# Clean project:
./cmake.sh {build-type} clean

# Reset project (rm -rf):
./cmake.sh {build-type} reset

# Commands can be combined.
```
