.PHONY: all default app install clean export

GNU_EFI_DIR=gnu-efi-3.0.15
SRC_DIR=uefi-app

all: default app

default:
	make -C $(GNU_EFI_DIR)

app: install
	make -C $(GNU_EFI_DIR) $(SRC_DIR)
	
install:
	cp -r $(SRC_DIR) $(GNU_EFI_DIR)/

clean:
	make -C $(GNU_EFI_DIR) clean

export:
	scp -P 3222 $(GNU_EFI_DIR)/x86_64/$(SRC_DIR)/main.efi nikit@127.0.0.1:/home/nikit

