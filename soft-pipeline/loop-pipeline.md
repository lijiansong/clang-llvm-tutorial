
Tips:

硬件加速器（FPGA/ASIC）本质上是在实现一个定制化的计算流水线，所以片上buffer得大小、port数量、并行度等超参数的设置都要服务于流水线。
流水线中最重要的指标是II(initiation interval)，II=1最好，所以编译器朝着这个方向优化，很容易推导出buffer之类的最优参数。
