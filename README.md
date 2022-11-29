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

- [x] Create dumped page container to store pages that are deleted (dumped)

- [x] Update table_typle to store pointer to last page and add gappy page.
- [ ] Update table_page to work with page_holes (like indexes but points to empty holes in page to do insertion and deletion with O(1)).
- [ ] Implement insertion and deletion of  row in table.
- [ ] Create special linked list of pages for table to store there not-full pages where typles can be written.
- [ ] Create module in dbms to store deleted pages (after table was deleted or table page has become empty).
- [ ] Implement update of row in table
- [ ] Split strings if size of page is larger
- [ ] Implement selection using where statements
- [ ] Implement joins
- [ ] Implement projection
