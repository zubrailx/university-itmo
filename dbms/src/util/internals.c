#include <dbms/internals/page_loc.h>

extern inline bool fileoff_is_null(struct fileoff_t off);
extern inline struct pageoff_t get_pageoff_t(uint32_t size);
extern inline struct bodyoff_t get_bodyoff_t(uint32_t size);
extern inline struct fileoff_t get_fileoff_t(int64_t size);
