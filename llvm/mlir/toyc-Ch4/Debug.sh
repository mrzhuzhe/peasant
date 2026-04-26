gdb --ex "b ShapeInferencePass.cpp:61" --args ./build/bin/testzz test/test3.toy --emit mlir --opt

#gdb --ex "b ShapeInferencePass.cpp:61" --args /home/sanszhu/build/llvm-project/build/bin/toyc-ch4 test/test3.toy --emit mlir --opt
