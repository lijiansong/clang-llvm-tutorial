int foo(int a) {
   int x = 10;                
   if (a < 10) {
      x = a;                 // range of x is [min..9] 
   } 
   return x;                 // range of x is [min..10]
}

/// Expected output
/// [min..10]
