#include "print_page.h"

void print_page(base_page *page) {
  int width = 30;
  int cols = page->header.size.bytes / width;
  for (int i = 0; i < cols; ++i) {
    for (int j = 0; j < width; ++j) {
      printf("%02X", ((unsigned char *)page)[i * width + j]);
    }
    printf("    ");
    for (int j = 0; j < width; ++j) {
      unsigned char c = ((unsigned char *)page)[i * width + j];
      if (c < 32 || c == 127) {
        printf(".");
      } else {
        printf("%c", c);
      }
    }
    printf("\n");
  }
}

void print_page_full(base_page *page, fileoff_t addr) {
  printf("address: %zu\n", addr.bytes);
  print_page(page);
}
