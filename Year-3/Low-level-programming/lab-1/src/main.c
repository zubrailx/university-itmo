#include <stdio.h>

#include "class.h"

int main() {
  struct A a;
  a.a = 5;
  printf("%d", a.a);
  return 0;
}
