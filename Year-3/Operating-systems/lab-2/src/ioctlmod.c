#include <linux/export.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/cdev.h>
#include <linux/memblock.h>
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


static void ioctl_init_device_interface(struct device_interface *interface) {
  memset(interface, 0, sizeof(struct device_interface));
  atomic_set(&interface->available, 1);
  sema_init(&interface->sem, 1);
}

static void ioctl_del_device_interface(struct device_interface *interface) {
  memset(interface, 0, sizeof(struct device_interface));
}

static int ioctl_setup_cdev(struct device_interface *interface) {
  cdev_init(&interface->cdev, &file_operations);
  return cdev_add(&interface->cdev, devno, 1);
}

static void ioctl_mod_exit(void) {
  if (dev_class) {
    device_destroy(dev_class, devno);
    class_destroy(dev_class);
    printk(KERN_INFO "ioctl_mod: dev=%s, class=%s destroyed", DEVICE_NAME, DEVICE_CLASS);
  }
  cdev_del(&device_interface.cdev);
  unregister_chrdev_region(devno, 1);
  ioctl_del_device_interface(&device_interface);
  printk(KERN_INFO "ioctl_mod: interface unloaded\n");
}

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
  printk(KERN_INFO "ioctl_mod: dev=%s, class=%s created", DEVICE_NAME, DEVICE_CLASS);
  return 0;

fail:
  ioctl_mod_exit();
  return -1;
}

// public API
int device_open(struct inode *inode, struct file *file) {
  // inode->i_cdev is offset pointed to device_interface
  struct pci_dev *dev = NULL;
  struct device_interface *dev_interface;
  dev_interface = container_of(inode->i_cdev, struct device_interface, cdev);
  printk(KERN_INFO "ioctl_mod (DEBUG)[open]: address %p", dev_interface);
  if (!atomic_dec_and_test(&dev_interface->available)) {
    atomic_inc(&dev_interface->available);
    printk(KERN_ALERT "ioctl_mod: interface is busy, unable to open\n");
    return -EBUSY;
  }
  // TESTING
  printk(KERN_INFO "ioctl_mod: memblock_start_of_DRAM %llu", memblock_start_of_DRAM());
  dev = pci_get_device(PCI_ANY_ID, PCI_ANY_ID, dev);
  // printk(KERN_INFO "ioctl: memblock - %p", &memblock);
  return 0;
}

int device_release(struct inode *inode, struct file *file) {
  struct device_interface *dev_interface;
  dev_interface = container_of(inode->i_cdev, struct device_interface, cdev);
  printk(KERN_INFO "ioctl_mod (DEBUG)[release]: address %p", dev_interface);
  atomic_inc(&dev_interface->available);
  return 0;
}


long device_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
  return 0;
}

module_init(ioctl_mod_init);
module_exit(ioctl_mod_exit);
