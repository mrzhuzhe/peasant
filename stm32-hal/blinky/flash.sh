export BINARY=stm32-blinky-f1

$HOME/opt/cross-arm/bin/arm-none-eabi-objcopy -Obinary ./build/$BINARY.elf ./build/$BINARY.bin
/usr/local/bin/st-flash write ./build/$BINARY.bin 0x8000000