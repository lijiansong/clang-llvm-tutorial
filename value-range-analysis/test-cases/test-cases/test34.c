int foo(int a) {
   int x = 0;
   int y = 0;
   for (; x < 10; ) {
     x = x + 1;
     y = y - 1;
   }
  return y;
}

/// Expected output
/// [min..0]
