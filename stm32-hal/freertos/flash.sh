export BINARY=stm32-freertos-F103C8

/usr/local/bin/st-flash write ./build/$BINARY.bin 0x8000000