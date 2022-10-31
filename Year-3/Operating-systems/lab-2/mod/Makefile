KDIR=/home/nikit/data/kernel/linux-6.0
BUILD_KDIR=$(KDIR)/build
BUILD_DIR=$(PWD)/build

MODNAME=ioctlmod

obj-m += $(MODNAME).o

default:
	make -C $(KDIR) O=$(BUILD_KDIR) M=$(BUILD_DIR) src=$(PWD) modules

clean:
	make -C $(KDIR) O=$(BUILD_KDIR) M=$(BUILD_DIR) src=$(PWD) clean
	rm -rf $(BUILD_DIR)

mv_ko: default
	mv $(BUILD_DIR)/$(MODNAME).ko ./

# export:
# 	scp -P ${VM_PORT} $(BUILD_DIR)/$(MODNAME).ko ${VM_ADDRESS}:${VM_PATH}
# 	# also copy whole directory for buiding app
# 	$(eval PREV_DIR := ${PWD}/)
# 	cd ..
# 	scp -r -P ${VM_PORT} $(PREV_DIR) ${VM_ADDRESS}:${VM_PATH}
