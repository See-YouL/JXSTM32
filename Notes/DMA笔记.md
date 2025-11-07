# DMA笔记

## 理论知识

### DMA简介

- DMA（Direct Memory Access，直接内存访问）是一种允许外设直接与系统内存交换数据的技术，
而无需CPU的干预。这种方式可以大大提高数据传输的效率，减轻CPU的负担。
- DMA可以提供外设和存储器或者存储器和存储器之间的高速数据传输，无需CPU参与。
- 有12个独立可配置的通道：DMA1有7个通道，DMA2有5个通道
- 每个通道都支持软件触发和特定的硬件触发
- STM32F103C8T6的DMA资源：DMA1(7个通道)

### 存储器映像

![存储器映像](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20251107122814076.png)

### DMA框图

![DMA框图](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20251107123927902.png)

### DMA基本结构

![DMA基本结构](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20251107130143600.png)

### DMA请求

![DMA请求](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20251107131128296.png)
