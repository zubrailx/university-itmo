#pragma once

struct dto_row_list dto_row_list_construct();
void dto_row_list_destruct(struct dto_row_list *lst);
void dto_row_list_append(struct dto_row_list *lst, const void *row[]);
