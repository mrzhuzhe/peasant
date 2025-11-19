#include <stdio.h>
#include <stdlib.h>

int testfn(int i){
    i = i + 44;
    return i;
}

int main(){
    int a = 55;
    int b = testfn(a);
    printf("this is %d", b);
    
    int *c = NULL;
    int  d = 100;
    c = (int*)malloc(d*sizeof(int));
    c[10] = 465;
    free(c);
    return 0;
}