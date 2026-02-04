export PREFIX=/home/sanszhu/build/llvm-project/build
export BUILD_DIR=/home/sanszhu/build/llvm-project/build
mkdir build
cd build
cmake .. -DMLIR_DIR=$PREFIX/lib/cmake/mlir -DLLVM_EXTERNAL_LIT=$BUILD_DIR/bin/llvm-lit

#--target check-standalone