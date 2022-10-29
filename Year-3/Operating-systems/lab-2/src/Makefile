KDIR=/home/nikit/data/kernel/linux-6.0
BUILD_KDIR=$(KDIR)/build
BUILD_DIR=$(PWD)/build

obj-m += us_ioctlmod.o

default:
	make -C $(KDIR) O=$(BUILD_KDIR) M=$(BUILD_DIR) src=$(PWD) modules

clean:
	make -C $(KDIR) O=$(BUILD_KDIR) M=$(BUILD_DIR) src=$(PWD) clean
