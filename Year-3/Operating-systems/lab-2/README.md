## Вариант
```
ioctl: memblock, pci_dev
```

## Версия ядра и системы
Debian GNU/Linux 11 (bullseye)

Linux 6.0.0 x86_64 GNU/Linux

Debian GLIBC 2.31-13+deb11u5
## Примечание для того, что прога завелась при перекомпиляции ядра

В `mm/memblock.c` добавить:

```c
# mm/memblock.c

EXPORT_SYMBOL(memblock);
```

В противном случае получите undefined при компиляции модуля, так как символ не будет найден в System.map(`__kstrtab_memblock`) и не будет виден для модулей ядра.
