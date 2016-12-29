extern int GET();
extern long MALLOC(int);
extern void FREE(long);
extern void PRINT(int);


int * f(int b) {
   int *a = MALLOC(sizeof(int));
   *a = b;
   return a;
}

int main() {
   int* a;

   a = f(b);
   
   PRINT(*a);
   FREE(a);
}
