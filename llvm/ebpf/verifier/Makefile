vmlinux.h:
	bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h

hello-verify.bpf.o:
	clang \
	    -target bpf \
        -D __TARGET_ARCH_x86 \
	    -Wall \
	    -O2 -g -o $@ -c hello-verify.bpf.c

hello-verif.o:
	clang -Wall -o hello-verify.o hello-verify.c -l:libbpf.so -lelf -lz


genbpfskel:
	bpftool gen skeleton hello-verify.bpf.o > hello-verify.skel.h

dotgen:
	bpftool prog dump xlated name kprobe_exec visual > ouputs/out.dot
	dot -Tpng out.dot > ouputs/out.png