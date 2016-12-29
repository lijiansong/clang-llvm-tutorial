extern int GET();
extern int MALLOC();
extern void FREE(int);
extern void PRINT(int);

int main() {
   int a[2];
   int i;

   i = 0;
   a[0] = GET();
   a[1] = GET();
   PRINT(a[1]);
   PRINT(a[0]);
}
