export BINARY=stm32-blinky-f1

/usr/local/bin/st-flash write ./build/$BINARY.bin 0x8000000