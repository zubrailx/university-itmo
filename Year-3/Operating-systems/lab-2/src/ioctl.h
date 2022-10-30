#ifndef __IOCTL_H
#define __IOCTL_H

#include <linux/ioctl.h>

#define IOCTL_BASE 'i'

/* NOTE: _IOW means userland is writing and kernel is reading. _IOR*/
#define IOCTL_BASE_OPER _IO(IOCTL_BASE, 1)
// #define IOCTL_REQUEST_W _IOW(IOCTL_BASE, 2, void* message)
// #define IOCTL_RECEIVE_R _IOR(IOCTL_BASE, 3, void* message)

#endif // !__IOCTL_H
