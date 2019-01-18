int sum(int *a, int *b, int *c, int n) {
  int *p_a = &a[1];
  int *p_b = &b[1];
  int *p_c = &c[1];
  for (int i = 1; i < n; ++i) {
    p_c[i] = p_a[i] + p_b[i];
  }
  switch (n) {
    case 0:
        return p_c[0];
    case 1:
        return p_c[1];
    case 2:
        return p_c[2];
  }
  return p_c[n - 1];
}
