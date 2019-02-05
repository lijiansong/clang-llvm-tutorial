# LLVM Memory SSA

LLVM's memory SSA implement, see its source: [lib/Analysis/MemorySSA.cpp](https://github.com/llvm-mirror/llvm/blob/master/lib/Analysis/MemorySSA.cpp)

LLVM's memory dependence analysis pass implementation checkout this page: [lib/Analysis/MemoryDependenceAnalysis.cpp](https://github.com/llvm-mirror/llvm/blob/master/lib/Analysis/MemoryDependenceAnalysis.cpp)

## REF
Memory SSA:

- LLVM official page: <https://llvm.org/docs/MemorySSA.html>
- GCC Memory SSA implement note: <https://www.airs.com/dnovillo/Papers/mem-ssa.pdf>
- 2007 GCC developers' summit: <https://ols.fedoraproject.org/GCC/Reprints-2007/novillo-reprint.pdf>
- JSS'18, Parallel construction of interprocedual memory SSA form: <https://yuleisui.github.io/publications/jss18.pdf>

SSA:

- CC'96, Effective Representation of Alias and Indirect Memory Operations in SSA Form: <https://link.springer.com/content/pdf/10.1007%2F3-540-61053-7_66.pdf>

LLVM Examples:

- LLVM patch adds MemorySSA: <https://reviews.llvm.org/rL259595>
- GVN-Hoist: Hoisting Computations from Branches, <https://llvm.org/devmtg/2016-11/Slides/Kumar-Pop-GVNHoist.pdf> and source: <https://github.com/llvm-mirror/llvm/blob/master/lib/Transforms/Scalar/GVNHoist.cpp>
- LLVM early cse, a pass performs a simple dominator tree walk that eliminates trivially redundant instructions: <https://llvm.org/doxygen/EarlyCSE_8cpp_source.html>
