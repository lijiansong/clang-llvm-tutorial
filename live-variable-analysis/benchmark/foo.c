#include <stdio.h>

int sum(int a, int b) {
  int c = a + b;
  if (c == 10) {
    c += a;
  } else {
    c += b;
  }
  for (int i = 0; i < b; ++i) {
    c += i;
  }
  return c;
}

int main() { sum(1, 2); }
