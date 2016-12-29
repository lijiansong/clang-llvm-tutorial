extern int GET();
extern long MALLOC(int);
extern void FREE(long);
extern void PRINT(int);

int main() {
   int a;
   a = 0;
   while (a < 10) {
      a = a + 1;
   }
   PRINT(a);
}
