int add(int a, int b) {
   return a+b;
}

int sub(int a, int b) {
   return a-b;
}

int foo(int a, int b, int (*a_fptr)(int, int)) {
    return a_fptr(a, b);
}


int moo(char x, int op1, int op2) {
    int (*a_fptr)(int, int) = add;
    int (*s_fptr)(int, int) = sub;
    int (*t_fptr)(int, int) = 0;

    if (x == '+') {
       t_fptr = a_fptr;
    } 
    else if (x == '-') {
       t_fptr = s_fptr;
    }

    unsigned result = foo(op1, op2, t_fptr);
    return 0;
}

/// It is OK to print 10 : add, sub, NULL
/// 10 : add, sub
/// 26: foo
