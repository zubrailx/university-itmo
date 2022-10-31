MOD_DIR=mod
APP_DIR=app

.PHONY: $(APP_DIR) $(MOD_DIR)

VM_ADDRESS=nikit@127.0.0.1
VM_PORT=3122
VM_PATH=/home/nikit/data/linux/
KERNEL_DIR=/home/nikit/data/kernel/linux-6.0

export VM_ADDRESS
export VM_PORT
export VM_PATH
export KERNEL_DIR

all: 
	cd mod && make default mv_ko
	tar cf lab.tar $(APP_DIR) $(MOD_DIR) Makefile 
	scp -P $(VM_PORT) lab.tar $(VM_ADDRESS):$(VM_PATH)
