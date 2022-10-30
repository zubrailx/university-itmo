KDIR=/home/nikit/data/kernel/linux-6.0
BUILD_KDIR=$(KDIR)/build
BUILD_DIR=$(PWD)/build

MODNAME=ioctlmod

obj-m += $(MODNAME).o

all: default export

default:
	make -C $(KDIR) O=$(BUILD_KDIR) M=$(BUILD_DIR) src=$(PWD) modules

clean:
	make -C $(KDIR) O=$(BUILD_KDIR) M=$(BUILD_DIR) src=$(PWD) clean
	rm -rf $(BUILD_DIR)

VM_ADDRESS=nikit@127.0.0.1
VM_PORT=3122
VM_PATH=/home/nikit/data/linux/

export:
	scp -P $(VM_PORT) $(BUILD_DIR)/$(MODNAME).ko $(VM_ADDRESS):$(VM_PATH)
