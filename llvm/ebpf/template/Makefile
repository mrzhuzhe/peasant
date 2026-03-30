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
	- rm *.o
	- rm -f /sys/fs/bpf/hello 
	- rm -f /sys/fs/bpf/func

dump:
	llvm-objdump -S main.o

load:
	bpftool prog load main.o /sys/fs/bpf/hello 

load2:
	bpftool prog load func.o /sys/fs/bpf/func 