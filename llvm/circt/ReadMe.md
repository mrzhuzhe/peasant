export llvm_path=~/build/llvm-project/llvm
cmake ${llvm_path} -B build \
    -DCMAKE_BUILD_TYPE=Debug \
    -DLLVM_ENABLE_ASSERTIONS=ON \
    -DLLVM_TARGETS_TO_BUILD=host \
    -DLLVM_ENABLE_PROJECTS=mlir \
    -DLLVM_EXTERNAL_PROJECTS=circt \
    -DLLVM_EXTERNAL_CIRCT_SOURCE_DIR=$PWD


// llvm commitid e80604a6418404934a47bb3bfc14b4a21c1de626