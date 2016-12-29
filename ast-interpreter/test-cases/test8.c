extern int GET();
extern long MALLOC(int);
extern void FREE(long);
extern void PRINT(int);

int b = 10;
int main() {
   int a;
   a = 10;
   if (a > 5 ) {
     b = a+5;
   } else {
     b = a;
   }
   PRINT(b);
}
