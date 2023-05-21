# Configure input

This will set the baud rate to 9600, 8 bits, 1 stop bit, no parity:

```bash
stty -F /dev/ttyUSB1 9600 cs8 -cstopb -parenb
```

```bash
echo -ne "\0xFF" > /dev/ttyUSB1
```
