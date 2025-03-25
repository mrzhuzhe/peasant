# Linux tty setup
```
# show config 
sudo stty -F /dev/ttyUSB0 -a

# baud rate to 38400, 8 bits, 1 stop bit, no parity:
chmod 666 /dev/ttyUSB0 
stty -F /dev/ttyUSB0 38400 cs8 -cstopb -parenb -echo -icanon -onlcr

# USB port
# USB cdc https://github.com/libopencm3/libopencm3-examples/tree/master/examples/stm32/f1/stm32-maple/usb_cdcacm
chmod 666 /dev/ttyACM0 
stty -F /dev/ttyACM0  115200  cs8 -cstopb -parenb -echo -icanon -onlcr


# USB spi
chmod 666 /dev/ttyACM0 
stty -F /dev/ttyACM0  2400  cs8 -cstopb -parenb -echo -icanon -onlcr
```


# Debug stlink set up

st-util -p 4500

(arm-none-eabi-gdb) target extended-remote localhost:4500

(arm-none-eabi-gdb) file rtos-template.elf


# Cavest
```
//  https://gcc.gnu.org/onlinedocs/gcc-7.3.0/gcc/Optimize-Options.html#Optimize-Options
use -Os instend if -O0 to reduce code size or will cause stackoverflow
```

# Cortex-M3 Instruction set	
Thumb-1, Thumb-2,
Saturated (some), Divide

# Major catagories in RTOS
1. multitasking and scheduling
    preemptive multitasking
    cooperative multitasking (coroutines)
2. Message queues
3. semaphores and mutexes
4. timers 
5. event groups

# Todos
1. wifi module esp01 https://docs.ai-thinker.com/_media/esp8266/esp8266_series_modules_user_manual_en.pdf
2. Battery module
3. turn axie
4. 4 axies helicoper
