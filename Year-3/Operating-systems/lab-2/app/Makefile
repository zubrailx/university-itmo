CC=gcc
LD=$(CC)
APP_NAME=app
BUILD_DIR=build

.PHONY: default export $(BUILD_DIR) run

# KERNEL_INCLUDES=-I${KERNEL_DIR}/include 
# KERNEL_INCLUDES+=-I${KERNEL_DIR}/arch/x86/include 

# set $KERNEL_INCLUDE to find KERNEL_SOURCES includes
# default: $(BUILD_DIR)
# 	$(CC) $(KERNEL_INCLUDES) -c main.c -o $(BUILD_DIR)/main.o
# 	$(LD) $(KERNEL_INCLUDES) -o $(BUILD_DIR)/$(APP_NAME) $(BUILD_DIR)/main.o

default: $(BUILD_DIR)
	$(CC) -c main.c -o $(BUILD_DIR)/main.o
	$(LD) -o $(BUILD_DIR)/$(APP_NAME) $(BUILD_DIR)/main.o

$(BUILD_DIR):
	mkdir -p $@

run:
	sudo ./$(BUILD_DIR)/$(APP_NAME)

# because my vbox has different glibc, i compile there
# scp -P ${VM_PORT} $(BUILD_DIR)/$(APP_NAME) ${VM_ADDRESS}:${VM_PATH}
# export:
# 	$(eval PREV_DIR := ${PWD}/)
# 	cd ..
# 	scp -r -P ${VM_PORT} $(PREV_DIR) ${VM_ADDRESS}:${VM_PATH}
