#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

int counter = 0;

SEC("xdp")
int hello(struct xdp_md *ctx) {
    bpf_printk("Hello World %d", counter);
    //bpf_trace_printk("Hello World %d", counter);
    counter++; 
    return XDP_PASS;
}

char LICENSE[] SEC("license") = "Dual BSD/GPL";

/*
bpftool prog list
bpftool prog show id 132 --pretty

// show bytecode
bpftool prog dump xlated name hello 
// show jit-compiled machine code
bpftool prog dump jited name hello 
// attaching to an event
bpftool net attach xdp id 132 dev enp0s31f6

bpftool net list
// path not exist
cat /sys/kernel/debug/tracing/trace_pipe
cat /sys/kernel/tracing/trace_pipe 
// show nothing
bpftool prog tracelog

// detached
bpftool net detach xdp dev enp0s31f6


*/