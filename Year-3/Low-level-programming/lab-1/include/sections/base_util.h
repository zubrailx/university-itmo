#pragma once

#include "dbmeta.h"

#define SSO_STRING_SIZE (sizeof(size_t) - 1)

struct SOPointer {
	fileoff_t sect_address;
	sectoff_t offset;
};

// DATABASE SECTION TABLE TYPLE
// Small string optimizations
struct StrNotIn {
	char ssize[SSO_STRING_SIZE];// string size, low bytes starts from start
	struct SOPointer ptr;
} __attribute__((packed));

struct SSO {
	bool is_inline;
	union {
		struct StrNotIn noin;
		char name[sizeof(struct StrNotIn)];
	} u;
} __attribute__((packed));

size_t sso_to_size(const char *ssize);
void size_to_sso(size_t size, char *ssize);
