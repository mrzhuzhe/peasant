/*
    CC = llvm-g++ -stdlib=libc++ -std=c++14
    CC = clang++ -stdlib=libstdc++ -std=c++14
    1. seems lllvm-project not compile libc++ yet
    2. seems no lld, gcc ld need specific llvm libs 
    3. #CC = llc -debug-pass=Structure
*/
  

#include <stdio.h>

int main(){
    printf("test template\n");
    return 0;
}