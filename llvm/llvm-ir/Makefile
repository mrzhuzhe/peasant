.phony: all

test:
	echo "gogogo"

all:
	clang -S -emit-llvm main.c
	clang -S -emit-llvm add.c
	clang -S -emit-llvm branch.c
	clang -S -emit-llvm for.c
	clang -S -emit-llvm call.c
	clang -S -emit-llvm struct.c
	clang -S -emit-llvm goto.c
	clang -O2 -fno-vectorize -S -emit-llvm simd.c

test:
	opt -passes="function<eager-inv>(drop-unnecessary-assumes,\
float2int,\
lower-constant-intrinsics,\
loop(loop-rotate<header-duplication;no-prepare-for-lto>,\
loop-deletion),\
loop-distribute,\
inject-tli-mappings,\
loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,\
infer-alignment,\
loop-load-elim,\
instcombine<max-iterations=1;no-verify-fixpoint>,\
simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,\
slp-vectorizer,\
vector-combine,\
instcombine<max-iterations=1;no-verify-fixpoint>,\
loop-unroll<O2>,\
transform-warning,\
sroa<preserve-cfg>,\
infer-alignment,\
instcombine<max-iterations=1;no-verify-fixpoint>,\
loop-mssa(licm<allowspeculation>),\
alignment-from-assumptions,\
loop-sink,\
instsimplify,\
div-rem-pairs,\
tailcallelim,\
simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" simd.ll -S 