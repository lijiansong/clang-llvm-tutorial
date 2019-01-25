inline int __attribute__((__always_inline__)) add1(int a, int b) {
  return a + b;
}

inline int __attribute__((__always_inline__)) add2(int a, int b) {
  int ret = 0;
  if (a == 1)
    ret = b;
  if (a == 10)
    ret = a + 10;
  if (a > b)
    ret += a * 10;
  else
    ret = b * 10;
  return ret;
}

int main() {
  int n = 100;
  int s = 0;
  for (int i = 0; i < n; ++i) {
    s += i;
    if (s < 50) {
      s += 1;
    }
  }
  add1(n, s);
  add2(n, s);
  return 0;
}
