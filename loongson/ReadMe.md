qemu-system-loongarch64 -M virt -m 12G -cpu la464 -smp 12 -bios /vm/QEMU_EFI.fd -drive file=/vm/OpenCloudOS-Stream-23-20240808-loongarch64-preview.qcow2  -net nic  -net bridge,br=br0  --nographic

Reffer

1.  https://github.com/sunhaiyong1978/CLFS-for-LoongArch/blob/main/CLFS_For_LoongArch64.md