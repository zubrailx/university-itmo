##

Замутил кучу приколов с MBR, чтобы и legacy и uefi запускать. Зато теперь могу и так и так запускаться.

## Initial steps

Append gnu-efi root Makefile with name of your directory

```

SUBDIRS = lib gnuefi inc apps uefi-app <- uefi-app is appended
gnuefi: lib
```
