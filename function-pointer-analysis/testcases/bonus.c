struct A {
   int op;
   int (*a_fptr)(int, int);
};


int add(int a, int b) {
   return a+b;
}

int foo() {
    struct A a;
    a.a_fptr = add;

    int op1 =1,  op2=2;

    unsigned result = a.a_fptr(op1, op2);
    return 0;
}


///  10 : add
