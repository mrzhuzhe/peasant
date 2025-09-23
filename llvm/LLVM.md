
# Reffers

wordcode https://llvm.org/devmtg/2011-09-16/EuroLLVM2011-MoreTargetIndependentLLVMBitcode.pdf

MLIR talk https://mlir.llvm.org/talks/

llc https://llvm.org/docs/CommandGuide/llc.html

## Other 

posix regexp.h

## LLVM test suite
cmake -DCMAKE_BUILD_TYPE=Debug -DLLVM_ENABLE_PROJECTS="clang;compiler-rt" \
    -DLLVM_OPTIMIZED_TABLEGEN=On \
    -DLLVM_PARALLEL_COMPILE_JOBS=8 -DLLVM_PARALLEL_LINK_JOBS=1 ../llvm


## LLVM origin

cmake -DCMAKE_BUILD_TYPE=Debug -DLLVM_ENABLE_PROJECTS="clang;compiler-rt;mlir" \
    -DLLVM_BUILD_EXAMPLES=ON \
    -DLLVM_TARGETS_TO_BUILD="Native;NVPTX;AMDGPU" \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo \
    -DLLVM_ENABLE_ASSERTIONS=ON \
    ../llvm

## Done

1. optimize pass


## Todos 

1. JIT
2. why dialect exist