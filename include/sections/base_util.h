#pragma once

#include "dbmeta.h"

#define SSO_STRING_SIZE (sizeof(size_t) - sizeof(bool))
#define SSO_MXLEN (sizeof(struct StrNoIn))

my_defstruct(SOPointer);
struct SOPointer {
	fileoff_t fileoff;
	sectoff_t offset;
};

// DATABASE SECTION TABLE TYPLE
// Small string optimizations
my_defstruct(StrNoIn);
struct StrNoIn {
	char ssize[SSO_STRING_SIZE];// string size, low bytes starts from start
	struct SOPointer ptr;
} __attribute__((packed));

struct SSO {
	bool is_inline;
	union {
		struct StrNoIn noin;
		char name[SSO_MXLEN];
	} u;
} __attribute__((packed));


size_t sso_to_size(const char *ssize);
void size_to_sso(size_t size, char *ssize);
