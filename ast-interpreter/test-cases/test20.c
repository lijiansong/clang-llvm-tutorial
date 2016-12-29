extern int GET();
extern long MALLOC(int);
extern void FREE(long);
extern void PRINT(int);


int main() {
   int* a;
   int b;
   b = 10;
   
   a = MALLOC(sizeof(int));
   *a = b;
   PRINT(*a);
   FREE(a);
}
