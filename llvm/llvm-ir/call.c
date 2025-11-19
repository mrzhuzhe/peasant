#include <stdio.h>

int testfn(int i){
    i = i + 44;
    return i;
}

int main(){
    int a = 55;
    int b = testfn(a);
    printf("this is %d", b);
    return 0;
}