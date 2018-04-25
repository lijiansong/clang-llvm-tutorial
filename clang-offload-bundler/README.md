# clang offloading bundler

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

For a) and b), we just return the job generated for the dependence. For
c) and d) we override the current action with the host/device dependence
if the current toolchain is host/device and set the offload dependences
info with the jobs obtained from the device/host dependence(s).

## Examples
TBD.

## REF
- OpenMP offloading support, slide: <https://llvm-hpc3-workshop.github.io/slides/Bertolli.pdf> 
- OpenMP offloading support, paper: <https://researcher.watson.ibm.com/researcher/files/us-zsura/17_llvmATSC2016.pdf>
- Generic Offload File Bundler Tool: <http://clang-developers.42468.n3.nabble.com/RFC-OpenMP-CUDA-Generic-Offload-File-Bundler-Tool-td4050147.html> and [example](https://chromium.googlesource.com/external/github.com/llvm-mirror/clang/+/refs/heads/master/test/Driver/openmp-offload-gpu.c)
