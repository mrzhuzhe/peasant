
all:
	nvcc --shared --compiler-options '-fPIC' -arch=sm_60 kernel_test.cu -o outputs/kernel_test.so
	g++ main.c ./outputs/kernel_test.so -o outputs/main.o