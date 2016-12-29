extern int GET();
extern long MALLOC(int);
extern void FREE(long);
extern void PRINT(int);

int f(int x) {
  if (x> 0) return x;
  return -x;
}
int main() {
   int a;
   int b;
   a = -10;
   b = f(a);
   PRINT(b);
}
