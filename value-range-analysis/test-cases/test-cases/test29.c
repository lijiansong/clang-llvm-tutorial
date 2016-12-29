int foo(int a) {
   int x = 0;
   int y = 1;
   for (; x < 10; ) {    
    ///First pass, 
    ///compute x as [0..max], apply the special operator after two iterations
    /// old range of x is [0..2], new range of x is [0..3], since 2!=3, widening the upper
    /// bound to max, so we have new range of x as [0..max]
    ///Second pass,
    ///compute x as [0..9], initial range of x is [0..max]  
     x = x + 1;
   }
  return x;
}

/// Expected output
/// [10..10]
