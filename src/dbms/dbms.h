#pragma once

#include "dbfile.h"

my_defstruct(dbms);
struct dbms {
  struct dbfile *file; 
  struct dbmeta *meta;
};
