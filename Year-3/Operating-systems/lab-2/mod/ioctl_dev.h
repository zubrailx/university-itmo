#ifndef __IOCTL_DEV
#define __IOCTL_DEV

struct device_interface {
  atomic_t available;
  struct cdev cdev;
};

int device_open(struct inode *inode, struct file *file);
int device_release(struct inode *inode, struct file *file);
long device_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

#endif // !__IOCTL_DEV
