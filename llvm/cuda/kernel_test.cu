#include "cuda_runtime.h"
#include "stdio.h"
#include "kernel_test.h"

__global__ void test_kernel(float *a, float *b , float * c, int n){
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i>=n) return;    
    c[i] = a[i] + b[i];
};

extern void testFn(float *a, float *b , float *c, int n){
    float *da, *db, *dc;
    size_t size = n * sizeof(float);

    cudaMalloc((void**)&da, size);
    cudaMalloc((void**)&db, size);
    cudaMalloc((void**)&dc, size);
    
    cudaMemcpy(da, a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(db, b, size, cudaMemcpyHostToDevice);

    int threads = 256;
    int blocks =n / threads + 1;
    test_kernel<<<blocks, threads>>>(da, db ,dc, n);

    cudaDeviceSynchronize();

    cudaMemcpy(c, dc, size, cudaMemcpyDeviceToHost);

    cudaFree(da);
    cudaFree(db);
    cudaFree(dc);
}