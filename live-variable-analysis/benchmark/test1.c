int foo(int a) {
   int x = 0;
   if (a) {
      x = 0;
   } else {
      x = 10;
   }
   return x;    /// Range of x is [0..10]
}

/// Expected output
/// [0..10]
