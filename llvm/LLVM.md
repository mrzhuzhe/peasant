## LLVM debug for test suite
cmake -DCMAKE_BUILD_TYPE=Debug \
    -DLLVM_ENABLE_PROJECTS="clang;compiler-rt" \
    -DLLVM_TARGETS_TO_BUILD="X86" \
    -DLLVM_OPTIMIZED_TABLEGEN=On \
    -DLLVM_PARALLEL_COMPILE_JOBS=8 \
    -DLLVM_PARALLEL_LINK_JOBS=1 \
    ../llvm

## LLVM CPU0
cmake -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_COMPILER=clang++ \
  -DCMAKE_C_COMPILER=clang \
  -DLLVM_TARGETS_TO_BUILD=Cpu0 \
  -DLLVM_ENABLE_PROJECTS="clang" \
  -DLLVM_OPTIMIZED_TABLEGEN=On \
  -DLLVM_PARALLEL_COMPILE_JOBS=4 \
  -DLLVM_PARALLEL_LINK_JOBS=1 \
  -G "Unix Makefiles" ../llvm \
  time make -j4


## LLVM origin

cmake -DLLVM_ENABLE_PROJECTS="clang" \
    -DLLVM_TARGETS_TO_BUILD="X86" \
    -DCMAKE_BUILD_TYPE=Release \
    ../llvm \
    --install-prefix $PWD../install
    

cmake -DLLVM_ENABLE_PROJECTS="mlir" \ 
    -DLLVM_BUILD_EXAMPLES=ON \
    -DLLVM_TARGETS_TO_BUILD="X86;NVPTX" \
    -DCMAKE_BUILD_TYPE=Debug \
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


## LLVM test suite

cmake -DCMAKE_C_COMPILER=$(pwd)/../debug/build/bin/clang \
    -C../llvm-test-suite/cmake/caches/O3.cmake \
    -DCMAKE_C_FLAGS=-fPIE \
    -DCMAKE_CXX_FLAGS=-fPIE \
    ../llvm-test-suite
  make