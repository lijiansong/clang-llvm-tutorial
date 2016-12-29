extern int GET();
extern long MALLOC(int);
extern void FREE(long);
extern void PRINT(int);

int f(int x) {
  return x + 10;
}
int main() {
   int a;
   int b;
   a = 0;
   b = f(a);
   PRINT(b);
}
