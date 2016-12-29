extern int GET();
extern long MALLOC(int);
extern void FREE(long);
extern void PRINT(int);


int  f(int b) {
   return b+1;
}

int main() {
   int a;
   a = 10;

   a = f(a);
}
