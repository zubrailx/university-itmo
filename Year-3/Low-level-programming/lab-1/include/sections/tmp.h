#pragma once

#include "base.h"

// TMP
typedef struct TmpHeader {
  BaseSection base_section;
  fileoff_t next;
} TmpHeader;

typedef struct TmpSection {
  TmpHeader tmp_header;
  void *body;
} TmpSection;
