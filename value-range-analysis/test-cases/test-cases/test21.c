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
   y = x + 1;  //range of y is [1..11] 

   if (y < x) { 
      // range of x is [2..10] 
      z = x;
   }  
   return z;       //range of z is[2..100]
}

/// Expected output
/// [2..100]
