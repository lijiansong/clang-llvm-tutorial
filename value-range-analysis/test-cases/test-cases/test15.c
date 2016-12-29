int foo(int a, int b) {
   int x = 0;
   int y = 100;   // range of y is [100..100]
   int z = 100;   // range of z is [-10..100]
   if (a) {
      x = 0;
   } else {
      x = 10;
   }
   // range of x is [0..10] 
   if (b) {
      y = x - 5;  //range of y is [-5..5] 
   } else {
      y = x -6;  // range of y is [-6..4]
   }
   // range of y is [-6.. 5] 
   z = y * y;    // range of z is [0..36]
   return z;    
}

/// Expected output
/// [0..36]
