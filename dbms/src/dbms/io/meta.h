#pragma once

#include <stdio.h>

#include <dbms/database.h>

struct meta;

void meta_load(struct meta *meta, FILE *file);

void meta_alter(const struct meta *meta, FILE *file);
void meta_create(const struct meta *meta, FILE *file);
