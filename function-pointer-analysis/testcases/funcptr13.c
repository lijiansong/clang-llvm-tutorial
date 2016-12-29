#include<stdlib.h>
int add(int a, int b) {
   return a+b;
}

int sub(int a, int b) {
   return a-b;
}

int foo(int x) {
    int (*a_fptr)(int, int) = add;
    int (*s_fptr)(int, int) = sub;
    int (*t_fptr)(int, int) = 0;
    int (*q_fptr)(int, int) = 0;
    int (*r_fptr)(int, int) = 0;

    int op1=1, op2=2;

    if (x >= 3) {
       t_fptr = a_fptr;
    } 
    if (x >= 4) {
       t_fptr = s_fptr;
    }
    if (x >= 5) {
       q_fptr = t_fptr;
    }

    if (t_fptr != NULL) {
       unsigned result = q_fptr(op1, op2);
    }  
   return 0;
}

/// 30 : add, sub
