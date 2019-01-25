inline void __attribute__((__always_inline__))
__sv_add(int *c, int *a, int *b, int n) {
  for (int i = 0; i < n; ++i) {
    c[i] = a[i] + b[i];
  }
}

int main() {
  int a[10], b[10], c[10];
  __sv_add(c, a, b, 10);
  return 0;
}
