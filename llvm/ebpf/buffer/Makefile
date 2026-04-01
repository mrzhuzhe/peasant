TARGET = hello-buffer-config
ARCH = $(shell uname -m | sed 's/x86_64/x86/' | sed 's/aarch64/arm64/')

BPF_OBJ = ${TARGET:=.bpf.o}
USER_C = ${TARGET:=.c}
USER_SKEL = ${TARGET:=.skel.h}

all: $(TARGET) $(BPF_OBJ) find-map
.PHONY: all 

$(TARGET): $(USER_C) $(USER_SKEL) 
	gcc -Wall -o $(TARGET).o $(USER_C) -l:libbpf.so -lelf -lz

%.bpf.o: %.bpf.c vmlinux.h
	clang \
	    -target bpf \
        -D __TARGET_ARCH_$(ARCH) \
	    -Wall \
	    -O2 -g -o $@ -c $<
	llvm-strip -g $@

$(USER_SKEL): $(BPF_OBJ)
	bpftool gen skeleton $< > $@

vmlinux.h:
	bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h

clean:
	- rm $(BPF_OBJ)
	- rm $(TARGET).o
	- rm find-map.o

find-map: findmap.c
	gcc -Wall -o find-map.o findmap.c -l:libbpf.so -lelf -lz

readelf:
	readelf -S hello-buffer-config.bpf.o

createMap:
	bpftool map create /sys/fs/bpf/findme type array key 4 value 32 entries 4 name findmezz