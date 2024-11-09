#include <iostream>

int main(){
    int n1 = 0;
    for (int i=0; i <31;i++){
        n1 += 1 << i;
    } // limit
    std::cout<< n1 << std::endl;
    printf("%u %32b \n", n1, n1);

    int n2 = n1 + 1; // overflow 
    std::cout<< n2 << std::endl;
    printf("%u %32b \n", n2, n2);
    return 0;
}