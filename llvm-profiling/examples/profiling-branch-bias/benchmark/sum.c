void __sv_add(int *c, int *a, int *b, int n) {
  for (int i = 0; i < n; ++i)
    c[i] = a[i] + b[i];
}

int main() {
  int a[1024], b[1024], c[1024];
  for (int i = 0; i < 1024; ++i) {
    a[i] = i;
    b[i] = i + 1;
    c[i] = i + 2;
  }
  __sv_add(c, a, b, 1024);
  return 0;
}
