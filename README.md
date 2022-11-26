## Lab-1 DBMS (DEPRECATED INFO)

Create a basic example of DBMS with rational structure which is located in single file.

### Uniformed operations for function naming

#### Structure / Section in RAM:

* -

#### Operations with sections/pages in FILE

* create - create page
* alter - update page
* drop - remove page

#### Operations with RAM

* other than before
* construct, destruct ...

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
