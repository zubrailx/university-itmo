#pragma once

#include "../util/define.h"

my_defstruct(dbms);
struct dbms {
  struct dbfile *file; 
  struct dbmeta *meta;
};
