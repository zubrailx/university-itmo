#ifndef __IOCTL_H
#define __IOCTL_H

#include <linux/ioctl.h>
#include <linux/types.h>


struct user_pci_dev {
	unsigned int	devfn;		/* Encoded device & function index */
	unsigned short	vendor;
	unsigned short	device;
	unsigned short	subsystem_vendor;
	unsigned short	subsystem_device;
};

struct user_memblock {
	_Bool bottom_up;  /* is bottom up direction? */
	uint64_t current_limit;
	struct memblock_type *memory;
	struct memblock_type *reserved;
};

#define IOCTL_BASE 'i'

/* NOTE: _IOW means userland is writing and kernel is reading. _IOR*/
#define IOCTL_TEST _IO(IOCTL_BASE, 1)
#define IOCTL_READ_MEMBLOCK _IOR(IOCTL_BASE, 2, struct user_memblock*)
#define IOCTL_READ_PCIDEV _IOR(IOCTL_BASE, 3, struct user_pci_dev*)

#endif // !__IOCTL_H
