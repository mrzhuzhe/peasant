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
	clang -S -emit-llvm simd.c

test:
	opt -passes="function<eager-inv>(drop-unnecessary-assumes,\
loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,\
infer-alignment,\
instcombine<max-iterations=1;no-verify-fixpoint>)" simd.ll -S 