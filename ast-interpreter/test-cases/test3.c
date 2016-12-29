extern int GET();
extern long MALLOC(int);
extern void FREE(long);
extern void PRINT(int);

int b;
int main() {
   int a;
   a = 10;
   b = 10;
   PRINT(a*b);
}
