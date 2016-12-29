int add(int a, int b) {
   return a+b;
}

int sub(int a, int b) {
   return a-b;
}

int (*goo(int a, int b, int (*a_fptr)(int, int), int(*b_fptr)(int, int) ))(int, int) {
   return a_fptr;
}

int (*foo(int a, int b, int (*a_fptr)(int, int), int(*b_fptr)(int, int) ))(int, int) {
   return b_fptr;
}

int moo(char x, int op1, int op2) {
    int (*a_fptr)(int, int) = add;
    int (*s_fptr)(int, int) = sub;
    int (* (*goo_ptr)(int, int, int (*)(int, int), int(*)(int, int)))(int, int);
    int (*t_fptr)(int, int) = 0;

    if (x == '+') {
       goo_ptr = goo;
    } 
    else if (x == '-') {
       goo_ptr = foo;
    }

    t_fptr = goo_ptr(op1, op2, a_fptr, s_fptr);
    t_fptr(op1, op2);
    
    return 0;
}


/// 15 : goo
/// 16 : add <You need to replace this call instruction to direct call> 
/// 32 : foo 
