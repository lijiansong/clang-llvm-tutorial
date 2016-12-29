extern int GET();
extern int MALLOC();
extern void FREE(int);
extern void PRINT(int);

int f(int a) {
  int b = a + 10;
  return b;
}
int main() {
   int a;
   a = GET();
   PRINT(f(a));
}
