#include <linux/cdev.h>
#include <linux/export.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/memblock.h>
#include <linux/module.h>
#include <linux/pci.h>

#include "ioctl.h"
#include "ioctl_dev.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nikita Kulakov");

#define DEVICE_CLASS "devc_ioctl"
#define DEVICE_NAME "dev_ioctl"
char *ioctl_dev_name = DEVICE_NAME;

struct class *dev_class;
dev_t devno = 0;

struct ioctl_message {
  struct memblock memblock;
  struct pci_dev pci_dev;
} ioctl_message;

struct device_interface device_interface;

struct file_operations file_operations = {
    .owner = THIS_MODULE,
    .read = NULL,
    .write = NULL,
    .open = device_open,
    .release = device_release,
    .unlocked_ioctl = device_ioctl,
};

// Functions for manipulating device_interface
static void ioctl_init_device_interface(struct device_interface *interface) {
  memset(interface, 0, sizeof(struct device_interface));
  atomic_set(&interface->available, 1);
}

static void ioctl_del_device_interface(struct device_interface *interface) {
  memset(interface, 0, sizeof(struct device_interface));
}

static int ioctl_setup_cdev(struct device_interface *interface) {
  cdev_init(&interface->cdev, &file_operations);
  return cdev_add(&interface->cdev, devno, 1);
}

// On module load
static void __exit ioctl_mod_exit(void) {
  if (dev_class) {
    device_destroy(dev_class, devno);
    class_destroy(dev_class);
    printk(KERN_INFO "ioctl_mod: dev=%s, class=%s destroyed", DEVICE_NAME,
           DEVICE_CLASS);
  }
  cdev_del(&device_interface.cdev);
  unregister_chrdev_region(devno, 1);
  ioctl_del_device_interface(&device_interface);
  printk(KERN_INFO "ioctl_mod: interface unloaded\n");
}

// On module exit
static int __init ioctl_mod_init(void) {
  int result = 0;

  ioctl_init_device_interface(&device_interface);

  result = alloc_chrdev_region(&devno, 0, 1, DEVICE_NAME);
  if (result < 0) {
    printk(KERN_WARNING "ioctl_mod: can't get major number %d\n", MAJOR(devno));
    goto fail;
  }
  result = ioctl_setup_cdev(&device_interface);
  if (result < 0) {
    printk(KERN_WARNING "ioctl_mod: error %d setuping cdev\n", result);
    goto fail;
  }
  printk(KERN_INFO "ioctl_mod: interface loaded\n");

  // create file in /dev
  dev_class = class_create(THIS_MODULE, DEVICE_CLASS);
  if (dev_class == NULL) {
    printk(KERN_WARNING "ioctl_mod: can't create device class");
    goto fail;
  }
  if (device_create(dev_class, NULL, devno, NULL, DEVICE_NAME) == NULL) {
    printk(KERN_WARNING "ioctl_mod: can't create device file");
    goto fail;
  }
  printk(KERN_INFO "ioctl_mod: dev=%s, class=%s created", DEVICE_NAME,
         DEVICE_CLASS);
  return 0;

fail:
  ioctl_mod_exit();
  return -1;
}

// public API
int device_open(struct inode *inode, struct file *file) {
  // inode->i_cdev is offset pointed to device_interface
  struct device_interface *dev_interface;
  dev_interface = container_of(inode->i_cdev, struct device_interface, cdev);
  printk(KERN_INFO "ioctl_mod: OPENED");
  if (!atomic_dec_and_test(&dev_interface->available)) {
    atomic_inc(&dev_interface->available);
    printk(KERN_ALERT "ioctl_mod: interface is busy, unable to open\n");
    return -EBUSY;
  }
  return 0;
}

int device_release(struct inode *inode, struct file *file) {
  struct device_interface *dev_interface;
  dev_interface = container_of(inode->i_cdev, struct device_interface, cdev);
  printk(KERN_INFO "ioctl_mod: RELEASED");
  atomic_inc(&dev_interface->available);
  return 0;
}

long device_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
  switch (cmd) {
  case IOCTL_TEST: {
    uint32_t value;
    printk(KERN_INFO "ioctl_mod: ioctl(IOCTL_TEST)");
    if (copy_from_user(&value, (uint32_t *)arg, sizeof(value))) {
      return -EFAULT;
    }
    printk(KERN_INFO "\treceived=%u\n", value);

    value = 0x12345678;
    if (copy_to_user((uint32_t *)arg, &value, sizeof(value))) {
      return -EFAULT;
    }
    printk(KERN_INFO "\tsent=%u\n", value);

    break;
  }
  case IOCTL_READ_MEMBLOCK: { // convert memblock to user_memblock
    struct user_memblock u_memblock =
        (struct user_memblock){.bottom_up = memblock.bottom_up,
                               .current_limit = memblock.current_limit,
                               .memory = &memblock.memory,
                               .reserved = &memblock.reserved};
    printk(KERN_INFO "ioctl_mod: ioctl(IOCTL_READ_MEMBLOCK)");
    if (copy_to_user((struct user_memblock *)arg, &u_memblock,
                     sizeof(u_memblock))) {
      return -EFAULT;
    }
    break;
  }
  case IOCTL_READ_PCIDEV: {
    struct pci_dev *dev;
    struct user_pci_dev u_dev;
    printk(KERN_INFO "ioctl_mod: ioctl(IOCTL_READ_PCIDEV)");
    dev = pci_get_device(PCI_ANY_ID, PCI_ANY_ID, NULL);
    u_dev = (struct user_pci_dev){.device = dev->device,
                                  .subsystem_device = dev->subsystem_device,
                                  .subsystem_vendor = dev->subsystem_vendor,
                                  .vendor = dev->vendor,
                                  .devfn = dev->devfn};
    if (copy_to_user((struct user_pci_dev *)arg, &u_dev, sizeof(u_dev))) {
      return -EFAULT;
    }
    break;
  }
  }
  return 0;
}

module_init(ioctl_mod_init);
module_exit(ioctl_mod_exit);
