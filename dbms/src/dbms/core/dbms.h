#pragma once

typedef struct dbms {
  struct dbfile *dbfile;
  struct meta *meta;
} dbms;
