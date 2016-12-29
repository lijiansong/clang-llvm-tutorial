int foo(int a) {
   int x = 10;                
   if (a < 10 && a > 0 ) {
      x = a;                 // range of x is [1..9] 
   } 
   return x;                 // range of x is [1..10]
}

/// Expected output
/// [1..10]
