gdb --ex "b ShapeInferencePass.cpp:61" --args ./build/bin/testzz test/test3.toy --emit mlir --opt

#/home/sanszhu/build/llvm-project/build/bin/toyc-ch6 test/test3.toy --emit llvm --opt
#/home/sanszhu/build/llvm-project/build/bin/toyc-ch6 test/test3.toy --emit mlir-llvm --opt