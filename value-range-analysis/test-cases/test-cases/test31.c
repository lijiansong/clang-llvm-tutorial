int foo(int a) {
   int x = 10;
   for (; x > 0; ) {  
    ///First pass, 
    ///compute x as [min..10], apply the special operator after two iterations
    /// old range of x is [8..10], new range of x is [7..10], since 8!=7, widening the lower
    /// bound to min, so we have new range of x as [min..10]
    ///Second pass,
    ///compute x as [1..10], initial range of x is [min..10]  
 
     x = x -1 ;           
   }
  return x;
}

/// Expected output
/// [0..0]
