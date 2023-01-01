#include "sso.h"

size_t sso_to_size(const unsigned char *ssize) {
  size_t res = 0;
  size_t mask = 0xff;
  for (size_t i = 0; i != SSO_SSIZE_SIZE; ++i) {
    res = (res << 8) | (mask & ssize[SSO_SSIZE_SIZE - 1 - i]);
  }
  return res;
}

void size_to_sso(const size_t size, unsigned char *ssize) {
  size_t mask = 0xff;
  for (size_t i = 0; i != SSO_SSIZE_SIZE; ++i) {
    ssize[i] = (unsigned char)((size >> 8 * i) & mask);
  }
}
