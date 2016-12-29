extern int GET();
extern long MALLOC(int);
extern void FREE(long);
extern void PRINT(int);

int b = 10;
int main() {
   int a;
   a = 10;
   b = a + 1;
  PRINT(b);
}
