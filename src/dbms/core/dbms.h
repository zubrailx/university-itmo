#pragma once

typedef struct dbms {
  struct dbfile *dbfile;
  struct dbmeta *meta;
} dbms;
