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

   if (y  < 7) {
        // range of y is [1..6] 
        z = y * 4;   //range of z is [4..24]
   } else {
        z = y * 3;      //range of z is[21..33]
   }
   
   return z;       //range of z is[4..33]
}

/// Expected output
/// [4..33]
