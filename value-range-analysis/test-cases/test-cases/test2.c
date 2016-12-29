int foo(int a) {
   int x = 0;
   int y = 1;
   if (a) {
      x = 0;
   } else {
      x = 10;
   }
   y = x + 1;    
   return y;
}

/// Expected output
/// [1..11]
