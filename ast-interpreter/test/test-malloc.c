extern int GET();
extern long MALLOC(int);
extern void FREE(int *);
extern void PRINT(int);

int main() {
   int a;
   int b;
   int *ptr;
   int **dptr;

   ptr = (int *)MALLOC(sizeof(int));
   dptr = (int **)MALLOC(sizeof(int *));
   *dptr = ptr;
   a = GET();
   *ptr = a;
   b = **dptr;
   PRINT(b);
   FREE((int *)ptr);
   FREE((int *)dptr);
}


