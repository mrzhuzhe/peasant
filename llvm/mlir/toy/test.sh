export BUILD_DIR=~/build/llvm-project/build
export SRC_DIR=~/build/llvm-project/mlir
${BUILD_DIR}/bin/mlir-opt --help

${BUILD_DIR}/bin/mlir-tblgen -gen-dialect-decls ./Ops.td -I ${SRC_DIR}/include/

# ${BUILD_DIR}/bin/mlir-tblgen -gen-dialect-decls ${SRC_DIR}/examples/toy/Ch2/include/toy/Ops.td -I ${SRC_DIR}/include/

# https://mlir.llvm.org/docs/Tutorials/Toy/Ch-1/