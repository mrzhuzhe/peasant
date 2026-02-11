#include "cuda.h"
#include "stdio.h"
#include "kernel_test.h"

// add a runtime function with llvm
// use a macro or something 

int main(){
    unsigned n = 1e6;
    size_t size = n * sizeof(float);
    float *a, *b, *c;
    
    a = (float *)malloc(size);
    b = (float *)malloc(size);
    c = (float *)malloc(size);

    for (int i =0; i<n ;i++){
        a[i] = i;
        b[i] = i;
    }
    for (int i = 0; i < 1e3; i++) {
       testFn(a, b, c, n);
    }


    printf("test %f", c[10]);


    return 0;
}