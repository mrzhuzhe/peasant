TARGETS = main func

all: $(TARGETS)
.PHONY: all dump load

$(TARGETS): %: %.o 

%.o: %.c
	clang \
	    -target bpf \
		-I/usr/include/$(shell uname -m)-linux-gnu \
		-g \
	    -O2 -o $@ -c $<

clean: 
	- rm *.bpf.o
	- rm -f /sys/fs/bpf/hello 
	- rm -f /sys/fs/bpf/hello-func

dump:
	llvm-objdump -S main.o

load:
	bpftool prog load main.o /sys/fs/bpf/hello 