## Before the scene

Before doing this, managed to set up UEFI on MBR partition. Now legacy/BOOT and UEFI both can be used to load virtual machine.

## Initial steps

Append gnu-efi root Makefile with name of your directory

```

SUBDIRS = lib gnuefi inc apps uefi-app <- uefi-app is appended
gnuefi: lib
```
