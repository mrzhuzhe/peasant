# Instruction set	
Thumb-1, Thumb-2,
Saturated (some), Divide


# linux tty
```
# show config 
sudo stty -F /dev/ttyUSB0 -a

# baud rate to 38400, 8 bits, 1 stop bit, no parity:
chmod 666 /dev/ttyUSB0 
stty -F /dev/ttyUSB0 38400 cs8 -cstopb -parenb -echo -icanon -onlcr
```

# Major catagories in RTOS
1. multitasking and scheduling
    preemptive multitasking
    cooperative multitasking (coroutines)
2. Message queues
3. semaphores and mutexes
4. timers 
5. event groups