#include "cuda.h"
#include "stdio.h"
#include "kernel_test.h"

int main(){
    unsigned n = 100;
    size_t size = n * sizeof(float);
    float *a, *b, *c;
    
    a = (float *)malloc(size);
    b = (float *)malloc(size);
    c = (float *)malloc(size);

    for (int i =0; i<n ;i++){
        a[i] = i;
        b[i] = i;
    }

    testFn(a, b, c, n);
    


    printf("test %f", c[10]);


    return 0;
}