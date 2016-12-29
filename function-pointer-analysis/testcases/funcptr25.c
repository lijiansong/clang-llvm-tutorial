int add(int a, int b) {
   return a+b;
}

int sub(int a, int b) {
   return a-b;
}

int (*goo(int a, int b, int (*a_fptr)(int, int), int(*b_fptr)(int, int) ))(int, int) {
   return b_fptr;
}

int foo(int a, int b, int (*a_fptr)(int, int), int(*b_fptr)(int, int)) {
   int (*s_fptr)(int, int);
   s_fptr = goo(a, b, a_fptr, b_fptr);
   return s_fptr(a, b);
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

    unsigned result = foo(op1, op2, a_fptr, t_fptr);
    return 0;
}


/// 15 : goo
/// 16 : add, sub 
/// 32 : foo 
