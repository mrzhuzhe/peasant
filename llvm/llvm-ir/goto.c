int main(){
    int a = 456;
    goto test2;
    test: {
        a = 123;
    }    
    test2: {
        a = 789;
    }    
}