int foo(int a) {
   int x = 0;
   int y = 1;
   int z = 1;
   if (a) {
      x = 0;
   } else {
      x = 10;
   }
   // range of x is [0..10] 
   y = x + 1;    // range of y is [1..11] 
   z = x - y;    // range of z is [-11..9]
   return z;
}

/// Expected output
/// [-11..9]
