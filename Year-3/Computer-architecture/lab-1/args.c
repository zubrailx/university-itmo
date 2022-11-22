#include <stdio.h>

int main(int argc, char **argv, char **envp) {
  printf("&argc = %p\n", &argc);
  printf("&argv = %p\n", &argv);
  printf("&envp = %p\n", &envp);
  printf("argv[0] = %p\n", (void *)argv[0]);
  printf("envp[0] = %p\n", (void *)envp[0]);
  while (1);
}
