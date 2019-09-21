## Programming Languages

### 1. Continuation

Q: What is continuation?
A: A continuation is a representation of the control flow of your program at any point in time, essentially the stack.

In abstract terms, it represents "the rest of your program."
In some programming languages like `Scheme` that `expose continuations as first-class values`, you can capture the current continuation and invoke it later. When invoked, the current program state is replaced with the state at which the continuation was captured (i.e. the current stack is replaced with the stack from the continuation).

Continuations allow you to literally "jump" to different places in your code. They are a low-level primitive that gives you control over execution flow, allowing you implement everything from resumable exceptions to coroutines. Understanding continuations was the best thing I did as a young programmer; it forces you to understand how control flow works.

The control and data flow of a program can be represented using continuations.

在目前的DNN领域，计算图的表示方式，其本质上只能表达到函数调用这个级别，是无法表达分支等语义的,

An interesting idea: 在计算图中引入continuation的概念，是否存在一些有意思的研究问题？

#### REFs

- <https://en.wikipedia.org/wiki/Continuation>
- <https://jlongster.com/Whats-in-a-Continuation>
- Graph Oriented Programming: <https://docs.jboss.org/jbpm/v3/userguide/graphorientedprogramming.html>
- Asynchronous continuations: <https://docs.jboss.org/jbpm/v3/userguide/asynchronouscontinuations.html>
- A nice book, <Compiling with Continuations>

### 2. CPS(Continuation-passing style)


#### REFs

- <http://cgi.soic.indiana.edu/~c311/lib/exe/fetch.php?media=cps-notes.scm>
- <http://baruchel.github.io/blog/python/2015/07/10/continuation-passing-style-in-python/>
