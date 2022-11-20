#pragma once

#include <stdio.h>

#include <database.h>

struct dbmeta;

void meta_load(struct dbmeta *meta, FILE *file);

void meta_alter(const struct dbmeta *meta, FILE *file);
void meta_create(const struct dbmeta *meta, FILE *file);
