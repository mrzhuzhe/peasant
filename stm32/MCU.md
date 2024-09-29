# Instruction set	
Thumb-1, Thumb-2,
Saturated (some), Divide


# linux tty
```
# show config 
sudo stty -F /dev/ttyUSB0 -a

# baud rate to 38400, 8 bits, 1 stop bit, no parity:
sudo stty -F /dev/ttyUSB0 38400 cs8 -cstopb -parenb -echo -icanon -onlcr
```