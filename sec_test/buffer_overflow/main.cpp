#include <iostream>

int main(){
    int n=10;
    int a[n];
    int b[n];
    int *ptr_a = &a[0];   
    int *ptr_b = &b[0];    
    for (int i=0;i<n;i++){
        a[i] = i;
        b[i] = 10 + i;
    }

    for (int i=0;i<2*n;i++){
        std::cout << "ptr_b+" << i << ": " << *(ptr_b+i) << std::endl;
    }

    std::cout << "&a[0]: " << &a[0] << " &b[0]: " << &b[0] << std::endl;
    return 0;
}