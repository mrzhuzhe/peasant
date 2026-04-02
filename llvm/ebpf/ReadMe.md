1. user space and kernel space https://eunomia.dev/tutorials/11-bootstrap/
2. https://docs.ebpf.io/concepts/core/ 


```
bpftool gen skeleton

bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h
```