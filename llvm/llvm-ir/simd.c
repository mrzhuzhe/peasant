#include "stdio.h"
int main(){
    const int loop = 10000;
    float a[loop];
    for (int i=0;i<loop;i++){
        a[i] = (float)i + 1.0f;
    }
    printf("%f", a[3]);
    return 0;
}