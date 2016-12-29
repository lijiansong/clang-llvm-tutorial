extern int GET();
extern int MALLOC();
extern void FREE(int);
extern void PRINT(int);

int main() {
   int a;
   int *p = &a;
   a = GET();
   PRINT(*p);
}
