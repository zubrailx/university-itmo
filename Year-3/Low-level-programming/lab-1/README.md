## Lab-1 (DBMS)

Create a basic example of DBMS with rational structure which is located in single file.

### Uniformed operations for function naming

#### Structure / Section in RAM:
* Load - load in ram
* Unload - unload from ram

#### Section in file + sync with RAM:
* Alter - update section in file (write some data to it)
* Create - create structure in file
* Drop - remove structure from file

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
* section - base section
* database - database meta to store information about different types of sections
* ds - database section
* dt - database typle - typle that contains info about table
* ityple - typle that is inlined (e.g. string pointers are actual strings)
* ts - table section
* tt - table typle (row in table) 
