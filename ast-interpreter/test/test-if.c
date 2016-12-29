extern int GET();
extern int MALLOC();
extern void FREE(int);
extern void PRINT(int);

int main() {
   int a;
   a = GET();
   if (a < 0) {
      PRINT(-a);
   }
   else {
     PRINT(a);
   }
}
