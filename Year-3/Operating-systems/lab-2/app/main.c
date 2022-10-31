#include <linux/pci.h>

#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "../mod/ioctl.h"

#define DEVICE_PATH "/dev/dev_ioctl"

int device_open(const char *device_name);
void device_close(const char *device_name, int fd_device);

int device_open(const char *device_name) {
  printf("[INFO]: Open device\n");
  int fd_device = open(device_name, O_RDWR);
  if (fd_device == -1) {
    printf("[ERROR]: Can't open device with name '%s'\n", device_name);
    exit(EXIT_FAILURE);
  }
  return fd_device;
}

void device_close(const char *device_name, int fd_device) {
  printf("[INFO]: Close device\n");
  int result = close(fd_device);
  if (result == -1) {
    printf("[ERROR]: Can't close device with name '%s'\n", device_name);
    exit(EXIT_FAILURE);
  }
}

// Pretty printing
#define STRUCT(type, pInstance, ...)                                           \
  {                                                                            \
    printf("%s=%p: {\n", #type, &pInstance);                                   \
    type *pStr = &pInstance;                                                   \
    __VA_ARGS__                                                                \
    printf("}\n");                                                             \
  }
#define FIELD(pPat, pInstance)                                                 \
  { printf("  %s=%" #pPat "\n", #pInstance, pStr->pInstance); }
// for inttypes...
#define PFIELD(pPat, pInstance)                                                \
  { printf("  %s=%" pPat "\n", #pInstance, pStr->pInstance); }

int main() {
  int fd = device_open(DEVICE_PATH);
  { // test
    uint32_t value = 0x87654321;
    if (ioctl(fd, IOCTL_TEST, &value) < 0) {
      perror("[ERROR]: ioctl");
      exit(EXIT_FAILURE);
    }
    printf("%s: received value(HEX)=%x\n", "IOCTL_TEST", value);
  }
  { // read memblock
    struct user_memblock value;
    if (ioctl(fd, IOCTL_READ_MEMBLOCK, &value) < 0) {
      perror("[ERROR]: ioctl");
      exit(EXIT_FAILURE);
    }
    printf("%s: received ", "IOCTL_READ_MEMBLOCK");
    STRUCT(struct user_memblock, value,
           FIELD(u, bottom_up) PFIELD(PRIu64, current_limit) FIELD(p, memory)
               FIELD(p, reserved))
  }
  { // read pci_dev
    struct user_pci_dev value;
    if (ioctl(fd, IOCTL_READ_PCIDEV, &value) < 0) {
      perror("[ERROR]: ioctl");
      exit(EXIT_FAILURE);
    }
    printf("%s: received ", "IOCTL_READ_PCIDEV");
    STRUCT(struct user_pci_dev, value,
           FIELD(u, devfn) FIELD(u, vendor) FIELD(u, device)
               FIELD(u, subsystem_vendor) FIELD(u, subsystem_device))
  }
  device_close(DEVICE_PATH, fd);
  return EXIT_SUCCESS;
}
