extern int GET();
extern long MALLOC(int);
extern void FREE(long);
extern void PRINT(int);


int main() {
   int* a;
   int b;
   b = 10;
   
   a = MALLOC(sizeof(int) * b);
   *(a+5) = b;
   PRINT(*(a+5));
   FREE(a);
}
