# Clang offloading bundler

In clang, the offload action is expected to be used in four different situations:

```

a) Set a toolchain/architecture/kind for a host action:

   Host Action 1 -> OffloadAction -> Host Action 2

b) Set a toolchain/architecture/kind for a device action;

   Device Action 1 -> OffloadAction -> Device Action 2

c) Specify a device dependence to a host action;

   Device Action 1  _
                     \
     Host Action 1  ---> OffloadAction -> Host Action 2

d) Specify a host dependence to a device action.

     Host Action 1  _
                     \
   Device Action 1  ---> OffloadAction -> Device Action 2

```

For a) and b), clang just returns the job generated for the dependence. For
c) and d) clang overrides the current action with the host/device dependence
if the current toolchain is host/device and set the offload dependences
info with the jobs obtained from the device/host dependence(s).

## Examples
We will obtain the following action graph when compiling two source files for host and a CUDA device, namely kernel-call.cu and a.cpp:

![image](./img/export.png "CUDA offload example")

As is shown from the figure above, if we are generating code for the device or we are in a backend phase, we attempt to generate a fat binary. Clang compiles each arch to `ptx and assemble to cubin`, then feed the `cubin and the ptx` into a device "link" action, which uses `cuda-fatbin` to combine these cubins into one fatbin.  The fatbin is then an input to the host action. During cuda device phase, clang creates an offloading action for backend and assembler action respectively. And an offload action is used to add a host dependence to the device linker actions.

By command `clang kernel-call.cu a.cpp -ccc-print-phases`, we will get following pipelined phases behind clang compiling:

```
0: input, "kernel-call.cu", cuda, (host-cuda)
1: preprocessor, {0}, cuda-cpp-output, (host-cuda)
2: compiler, {1}, ir, (host-cuda)
3: input, "kernel-call.cu", cuda, (device-cuda, sm_20)
4: preprocessor, {3}, cuda-cpp-output, (device-cuda, sm_20)
5: compiler, {4}, ir, (device-cuda, sm_20)
6: backend, {5}, assembler, (device-cuda, sm_20)
7: assembler, {6}, object, (device-cuda, sm_20)
8: offload, "device-cuda (nvptx64-nvidia-cuda:sm_20)" {7}, object
9: offload, "device-cuda (nvptx64-nvidia-cuda:sm_20)" {6}, assembler
10: linker, {8, 9}, cuda-fatbin, (device-cuda)
11: offload, "host-cuda (x86_64-apple-darwin17.2.0)" {2}, "device-cuda (nvptx64-nvidia-cuda)" {10}, ir
12: backend, {11}, assembler, (host-cuda)
13: assembler, {12}, object, (host-cuda)
14: input, "a.cpp", c++, (host-cuda)
15: preprocessor, {14}, c++-cpp-output, (host-cuda)
16: compiler, {15}, ir, (host-cuda)
17: backend, {16}, assembler, (host-cuda)
18: assembler, {17}, object, (host-cuda)
19: linker, {13, 18}, image, (host-cuda)
20: bind-arch, "x86_64", {19}, image
```

And we can get the following bindings result by typing command `clang kernel-call.cu a.cpp -ccc-print-bindings`:

```
# "nvptx64-nvidia-cuda" - "clang", inputs: ["kernel-call.cu"], output: "/var/folders/sp/yzrv8j5s4dg6mc4bgzvcc1h80000gn/T/kernel-call-10d9de.s"

# "nvptx64-nvidia-cuda" - "NVPTX::Assembler", inputs: ["/var/folders/sp/yzrv8j5s4dg6mc4bgzvcc1h80000gn/T/kernel-call-10d9de.s"], output: "/var/folders/sp/yzrv8j5s4dg6mc4bgzvcc1h80000gn/T/kernel-call-6a0708.o"

# "nvptx64-nvidia-cuda" - "NVPTX::Linker", inputs: ["/var/folders/sp/yzrv8j5s4dg6mc4bgzvcc1h80000gn/T/kernel-call-6a0708.o", "/var/folders/sp/yzrv8j5s4dg6mc4bgzvcc1h80000gn/T/kernel-call-10d9de.s"], output: "/var/folders/sp/yzrv8j5s4dg6mc4bgzvcc1h80000gn/T/kernel-call-c3b1ee.fatbin"

# "x86_64-apple-darwin17.2.0" - "clang", inputs: ["kernel-call.cu", "/var/folders/sp/yzrv8j5s4dg6mc4bgzvcc1h80000gn/T/kernel-call-c3b1ee.fatbin"], output: "/var/folders/sp/yzrv8j5s4dg6mc4bgzvcc1h80000gn/T/kernel-call-a9bed9.o"

# "x86_64-apple-darwin17.2.0" - "clang", inputs: ["a.cpp"], output: "/var/folders/sp/yzrv8j5s4dg6mc4bgzvcc1h80000gn/T/a-cfaee5.o"

# "x86_64-apple-darwin17.2.0" - "darwin::Linker", inputs: ["/var/folders/sp/yzrv8j5s4dg6mc4bgzvcc1h80000gn/T/kernel-call-a9bed9.o", "/var/folders/sp/yzrv8j5s4dg6mc4bgzvcc1h80000gn/T/a-cfaee5.o"], output: "a.out"

```
Another example is OpenMP, below is the action graph obtained when compiling two source files for host and an OpenMP device:

![image](./img/openmp-offload-example.png "OpenMP offload example")

As we can see, an offload action is used to add a host dependence to the device compile actions and add a device dependence to the host linking action[[2]](#ibm_offload_paper). The host depends on device action in the linking phase, when all the device images have to be embedded in the host image. Besides, when generating code for OpenMP, clang use the host compile phase output as a dependence to the device compile phase so that it can learn what declarations should be emitted.

## REF
- [1] OpenMP offloading support, slide: <https://llvm-hpc3-workshop.github.io/slides/Bertolli.pdf> 
- [2] <span id="ibm_offload_paper">OpenMP offloading support, paper: <https://researcher.watson.ibm.com/researcher/files/us-zsura/17_llvmATSC2016.pdf> </span>
- [3] Generic Offload File Bundler Tool: <http://clang-developers.42468.n3.nabble.com/RFC-OpenMP-CUDA-Generic-Offload-File-Bundler-Tool-td4050147.html> and [example](https://chromium.googlesource.com/external/github.com/llvm-mirror/clang/+/refs/heads/master/test/Driver/openmp-offload-gpu.c)
- [4] Clang Driver Internals: <https://clang.llvm.org/docs/DriverInternals.html>
