
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

cmake -DLLVM_ENABLE_PROJECTS="mlir" \ 
    -DLLVM_BUILD_EXAMPLES=ON \
    -DLLVM_TARGETS_TO_BUILD="X86;NVPTX" \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo \
    -DLLVM_ENABLE_ASSERTIONS=ON \
    ../llvm

# Using clang and lld speeds up the build, we recommend adding:
#  -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DLLVM_ENABLE_LLD=ON
# CCache can drastically speed up further rebuilds, try adding:
#  -DLLVM_CCACHE_BUILD=ON
# Optionally, using ASAN/UBSAN can find bugs early in development, enable with:
# -DLLVM_USE_SANITIZER="Address;Undefined"
# Optionally, enabling integration tests as well
# -DMLIR_INCLUDE_INTEGRATION_TESTS=ON
cmake --build . --target check-mlir

## Done

1. optimize pass


## Todos 

1. JIT
2. why dialect exist