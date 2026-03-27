## Separate build mlir and circt
```
export llvm_path=~/build/llvm-project
cmake . -B build \
    -DCMAKE_BUILD_TYPE=Debug \
    -DLLVM_ENABLE_ASSERTIONS=ON \
    -DMLIR_DIR=${llvm_path}/build/lib/cmake/mlir \
    -DLLVM_DIR=${llvm_path}/build/lib/cmake/llvm
```

// llvm commitid e80604a6418404934a47bb3bfc14b4a21c1de626
// circt commitid 95242acafbff83dc12aba082b8c29160059148db
// check circt fail may be in wrong tag