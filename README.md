## Lab-1 (DBMS)

Create a basic example of DBMS with rational structure which is located in single file.

### Uniformed operations for function naming

#### Structure / Section in RAM:
* Load - load in ram
* Unload - unload from ram

#### Operations with RAM

* construct - malloc and init
* destruct - free

#### Operations with sections/pages in FILE

* create - create page
* alter - update page
* drop - remove page

### Build system

```sh
# Some aliases for cmake usage (read it to get actual commands)
./cmake.sh -h 

# Basic commands:
./cmake.sh gen [(debug|release)] # generate build-tree + build (default to debug)

# Tests
./cmake.sh test # run tests

# Run executable (if src/main.c is present):
./build/(debug|release)/bin/dbms_exec
```

### Function abbreviations

* meta - meta page with information about database
* dp - database page
* tp - table page
* da - data page (for strings)
