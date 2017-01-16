#  Andersen Pointer Analysis
`Pointer analysis`, or `points-to analysis`, is a static code analysis technique that establishes which pointers, or heap references, can point to which variables, or storage locations. Techniques range widely in performance and precision. Relevant algorithm is [Steensgaard's algorithm](https://en.wikipedia.org/wiki/Steensgaard%27s_algorithm) and Andersen's algorithm.<br>
For Andersen's pointer analysis algorithm, more details can refer to his [Ph.D paper...](https://github.com/lijiansong/clang-llvm-tutorial/blob/master/andersen-pointer-analysis/paper/andersen-thesis94.pdf) It can be summarized in this form:<br>
`Base: a = & b; b ∈ pts(a)
Assign: a = b; pts(b) = pts(a)
Store: *p = b; ∀ v ∈ pts(p), pts(b) = pts(v)
Load: a = *q; ∀ v ∈ pts(q), pts(v) = pts(a)`<br>
In LLVM, it has its own pointer analysis infrastructure, more details can be seen [here](http://llvm.org/docs/AliasAnalysis.html)...<br>
In this archieve I will implement Andersen's pointer analysis algorithm by taking use of llvm.
