int add(int a, int b) {
   return a+b;
}

int foo(int a, int b, int (*a_fptr)(int, int)) {
    return a_fptr(a, b);
}


int moo(char x, int op1, int op2) {
    int (*a_fptr)(int, int) = add;
    unsigned result = foo(op1, op2, a_fptr);
    return 0;
}

/// 6 : add  < you need to replace the call with call to add
// 12 : foo
