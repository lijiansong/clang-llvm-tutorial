extern int GET();
extern int MALLOC();
extern void FREE(int);
extern void PRINT(int);

struct TY{
   int a;
   int b;
};
int main() {
   struct TY t;

   t.a = GET();
   t.b = GET();
   PRINT(t.a);
   PRINT(t.b);
}
