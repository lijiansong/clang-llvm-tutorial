int foo(int a) {
   int x = 10;
   for (; x > 0; ) {  
    ///First pass, 
    ///compute x as [min..max], apply the special operator after two iterations
    /// old range of x is [8..12], new range of x is [7..13], widening the lowerbound and the upper bound
    /// so we have new range of x as [min..max]
    ///Second pass,
    ///compute x as [1..max], initial range of x is [min..max]  
    if (a) 
       x = x + 1;
    else 
       x = x -1 ;           
   }
  return x;
}

/// Expected output
/// [0..0]
