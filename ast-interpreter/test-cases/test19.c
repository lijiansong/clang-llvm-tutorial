extern int GET();
extern long MALLOC(int);
extern void FREE(long);
extern void PRINT(int);


int main() {
   int* a;
   a = MALLOC(sizeof(int)*2);
   FREE(a);
   a = MALLOC(sizeof(int)*2);
   *a = 0;
   PRINT(a);
   FREE(a);
}
