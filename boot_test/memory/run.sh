nasm -f bin boot_sect_memory.asm -o boot_sect_memory.bin

# qemu-system-x86_64 boot_sect_memory.bin