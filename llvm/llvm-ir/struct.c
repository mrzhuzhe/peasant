
struct tests {
    char a;
    short b;
    int c;
    long d;
};

int g = 123;
struct testsfather {
    float e;
    int* f;
    double h;
    char* i;
    struct tests child;
};

int main(){    
    struct testsfather test1;
    test1.child.a = '1';
    test1.child.b = 22;
    test1.child.c = 33;
    test1.child.d = 44;
    test1.e = 55;
    test1.f = &g;
    test1.h = 66;
    test1.i = "you are silly pink";

    return 0;
}