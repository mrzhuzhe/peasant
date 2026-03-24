.PHONY:	all

all:
	nvcc --shared --compiler-options '-fPIC' -arch=sm_60 kernel_test.cu -o outputs/kernel_test.so
	g++ main.c ./outputs/kernel_test.so -o outputs/main.o

testptx:
	nvcc -ptx -m64 --shared --compiler-options '-fPIC' -arch=sm_60 kernel_test.cu -o outputs/kernel_test.ptx
#	nvcc -ptx -m64 -allow-unsupported-compiler -ccbin clang++ --shared --compiler-options '-fPIC -emit-llvm -std=c++17' -arch=sm_60 kernel_test.cu -o outputs/kernel_test.ptx