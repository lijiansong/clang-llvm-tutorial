int add(int a, int b) {
   return a+b;
}

int foo() {
    int (*a_fptr)(int, int) = add;

    int op1 =1,  op2=2;

    unsigned result = a_fptr(op1, op2);
    return 0;
}


///  10 : add
