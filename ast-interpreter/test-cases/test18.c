extern int GET();
extern long MALLOC(int);
extern void FREE(long);
extern void PRINT(int);


int main() {
   int* a;
   a = MALLOC(sizeof(int)*2);
   *a = 10;
   *(a+1) = 20;
   PRINT(*a);
   PRINT(*(a+1));
   FREE(a);
}
