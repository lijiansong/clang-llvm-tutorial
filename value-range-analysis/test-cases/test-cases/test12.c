int foo(int a, int b) {
   int x = 0;
   int y = 100;   // range of y is [100..100]
   int z = 100;   // range of z is [100..100]
   if (a) {
      x = 0;
   } else {
      x = 10;
   }
   // range of x is [0..10] 
   if (b) {
      y = x + 1;  //range of y is [1..11] 
   } else {
      y = x -1;  // range of y is [-1..9]
      z = y * x; //range of z is [-10..90]
   }
   // range of z is [-10, 100] 
   return z;    
}

/// Expected output
/// [-10..100]
