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
        z = y + 4;   //range of z is [5..10]
   } else {
        z = y;      //range of z is[7..11]
   }
   
   return z;       //range of z is[5..11]
}

/// Expected output
/// [5..11]
