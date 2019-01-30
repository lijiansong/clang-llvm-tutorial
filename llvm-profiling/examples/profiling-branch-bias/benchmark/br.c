#include <stdlib.h>
#include <time.h>
void __sv_add(int *c, int *a, int *b, int n) {
  for (int i = 0; i < n; ++i)
    c[i] = a[i] + b[i];
}

int main() {
  srand(time(NULL));
  int n = rand() % 1024;
  int *a = (int *)malloc(n * sizeof(int));
  int *b = (int *)malloc(n * sizeof(int));
  int *c = (int *)malloc(n * sizeof(int));
  for (int i = 0; i < n; ++i) {
    if ((i % 2) == 1) {
      a[i] = i / 2 + 0;
      b[i] = i / 2 + 1;
      c[i] = i % 2 + 1;
    } else {
      a[i] = i % 2 + 0;
      b[i] = i % 2 + 1;
      c[i] = i / 2 + 1;
    }
  }
  __sv_add(c, a, b, n);
  return 0;
}
