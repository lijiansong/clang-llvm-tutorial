# Swing Modulo Scheduling Algorithm

> Software pipelining is a technique that improves the scheduling of instructions in loops by overlapping instructions from different iterations.Modulo scheduling is an approach for constructing software pipelines that focuses on minimizing the cycle count of the loops and thereby optimize performance.

Software pipelining is an optimization for improving ILP by overlapping loop iterations. Swing Modulo Scheduling (SMS) is an implementation of software pipelining that attempts to reduce register pressure and generating efficient pipelines with a low compile-time cost.

It does so by first estimating a lower bound on the number of cycles it takes to execute the loop. This number is called the initiation interval, and the bound is called a minimum. 
Swing Modulo Scheduling (SMS) is a heuristic approach that aims to reduce register pressure.

In LLVM, the implementation of SMS is a target-independent back-end pass.
When enabled, the pass runs just prior to the register allocation pass, while the machine IR is in SSA form. If software pipelining is successful, then the original loop is replaced by the optimized loop. The optimized loop contains one or more prolog blocks, the pipelined kernel, and one or more epilog blocks.


The SMS algorithm consists of three main steps after computing the minimal initiation interval (MII).

- 1. Analyze the dependence graph and compute information about each instruction in the graph.
- 2. Order the nodes (instructions) by priority based upon the heuristics described in the algorithm.
- 3. Attempt to schedule the nodes in the specified order using the MII.

If all the instructions can be scheduled in the specified order, then the algorithm is successful. Otherwise, we increase the MII by one and try again. When the algorithm is successful, we need to replace the original loop with one or more prolog blocks, the optimized kernel, and one or more epilog blocks. The number of prolog and epilog blocks depends on the number of stages in the scheduled loop. When creating the new blocks, we need to generate new SSA names and generate Phis for the new kernel and each epilog block. The process of creating the new, pipelined loop is quite complex and hard to understand. This part of the code can certainly use some additional work to simplify and to improve the readability.



## REFs

- PACT'1996: Swing module scheduling: a lifetime-sensitive approach
- LLVM Implementation: <https://github.com/llvm/llvm-project/blob/master/llvm/lib/CodeGen/MachinePipeliner.cpp>
- An implementation of Swing Modulo Scheduling: <https://reviews.llvm.org/D16829>
- Loop Pipelining and Loop Unrolling: <https://www.xilinx.com/support/documentation/sw_manuals/xilinx2015_2/sdsoc_doc/topics/calling-coding-guidelines/concept_pipelining_loop_unrolling.html>
