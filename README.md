## Lab-1 DBMS (doc is out of date)

Create a basic example of DBMS with rational structure which is located in single file.

### Build system

```sh
# Some aliases for cmake usage (read it to get actual commands)
./cmake.sh -h 

# Basic commands:
./cmake.sh gen [(debug|release)] # generate build-tree + build (default to debug)

# Tests
./cmake.sh [target] {test [sep]}|{valgrind}
# Sanitized
./cmake.sh gen sanitized && ./cmake.sh sanitized test

# Run executable (if app/main.c is present):
./build/(debug|release|sanitized)/bin/dbms_exec
```

### Uniformed operations for function naming

#### Operations with sections/pages in FILE

* create - create page
* alter - update page
* drop - remove page

#### Function prefix

* dbms - function managed by DBMS
* dp - database page
* tp - table page
* da - data page (for strings)

#### Operations with RAM

* other than before
* construct, destruct ...

### Roadmap

- [x] A lot of different stuff

- [ ] Implement plan_delete
- [ ] Implement predicate_nodes for like where cases (maybe they will be not like basic plan nodes)
- [ ] Add predicate in select (also in constructor)
- [ ] Add predicate in delete (also in constructor)
- [ ] Add predicate in update (also in constructor)
- [ ] Implement plan_join
