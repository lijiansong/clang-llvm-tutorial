extern int GET();
extern long MALLOC(int);
extern void FREE(long);
extern void PRINT(int);


int main() {
   int* a;
   a = MALLOC(sizeof(int));
   *a = 10;
   PRINT(*a);
   FREE(a);
}
