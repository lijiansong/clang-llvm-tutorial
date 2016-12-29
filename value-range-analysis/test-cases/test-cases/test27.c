int foo(int a, int b) {
   int x = 0;
   int y = 100;   // range of y is [100..100]
   int z = 10;   // range of z is [10..10]
   if (a) {
      x = 0;
   } else {
      x = 10;
   }
   // range of x is [0..10] 
   y = x + 1;  //range of y is [1..11] 
   if (y < x) {
      // range of y is [1..9]
      // range of x is [2..10]
      z = y * x;   //range of z is [2..90]
   } else {
      // range of x is [0..10]
      // range of y is [1..11] 
      z = y * x;  // range of z is [0..110] 
      z = z -30;  // range of z is [-30..80]
   }
  return z;       //range of z is[-30..90]
}

/// Expected output
/// [-30..90]
