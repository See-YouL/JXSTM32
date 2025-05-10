# TIM 定时器笔记

## TIM 定时器简介

定时器可以对输入的时钟进行计数,并在计数值达到设定值时触发中断.

16位计数器,预分频器,自动重装寄存器的时基单元,在72MHz计数时钟下可以实现最大59.65s的计时.

不仅具备基本的定时中断功能, 而且还包含内外时钟源选择,输入捕获,输出比较,编码器接口,主从触发模式
等多种功能.

根据复杂度和应用场景分为了高级定时器,通用定时器,基本定时器三种类型.

### 定时器类型

![定时器类型分类图](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250418184513595.png)

### 基本定时器

![基本定时器结构图](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250422050704690.png)

定时中断流程:

1. 预分频器(PSC)之前连接基准计数时钟的输入,可认为来自内部时钟(CK_INT),内部时钟的来源为RCC的TIMxCLK, 一般频率值为72MHz.
2. 预分频器可对输入的时钟进行分频, 实际分频系数=预分频器的值+1,输出频率=输入频率/(预分频器的值+1).
3. 计数器(CNT)是一个16位的向上计数器,计数范围为0~65535,当自增到目标值会产生更新事件,并且可以产生中断.
4. 自动重装寄存器(ARR)是一个16位的寄存器,用于设置计数器的最大值(目标值),当计数器达到该值时会产生更新事件,并且可以产生中断,并且清零计数器.

![基本定时器结构图下部箭头](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250510232443307.png)

- 向上的折线箭头表示会产生中断信号，计数值等于自动重装值产生的中断叫做“更新中断”,更新中断会通往NVIC.
- 向下的箭头表示会产生一个事件,对应的事件叫做"更新事件",更新事件不会触发中断,但可以触发内部其他电路的工作.

主模式触发DAC:

![主模式触发DAC](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250510233223306.png)

主模式可以把定时器的更新事件映射到触发输出控制器的TRGO位置,TRGO直接接到DAC的触发转换引脚上,可以
直接触发DAC的转换,不需要额外触发中断.


### 通用定时器

![通用定时器的时基单元部分](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250510233537087.png)

时基单元与基本定时器原理类似

通用定时器和高级定时器支持向上计数,向下计数,中心对齐计数三种模式,最常用还是向上计数模式


![通用定时器的内外时钟源选择和主从触发模式部分](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250506125316891.png)


内外时钟源选择和主从触发模式部分:

![内部时钟源选择](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250510234004213.png)

通用定时器也可选择内部时钟也就是系统频率72MHz

![外部时钟模式2](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250510234454628.png)

*如上图所示,选择外部时钟TIMx_ETR作为外部时钟源->预分频器->输入滤波->触发控制器->时基单元,这一路也被叫作外部时钟模式2*

选择外部时钟源为TIMx_ETR, TIMx_ETR引脚的映射参考引脚映射表,如下

![TIM2_CH1_ETR映射表](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511030909964.png)

*如上图所示, TIM1_CH1_ETR映射在PA0引脚上*

也可通过TRGI来提供时钟,主要用作触发输入来使用,也可作为外部时钟来使用,当TRGI当做外部时钟使用时,这一路被叫做"外部时钟模式1",如下图所示

![TRGI作为外部时钟](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511031756751.png)

可通过TIMx_ETR作外部时钟走外部时钟模式1,此时会占用触发输入通道,如下图所示

![ETR作外部输入走外部时钟模式1](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511032102900.png)

也可通过**ITR信号**作外部时钟走外部时钟模式1,如下图所示

![ITR作外部时钟走外部时钟模式1](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511032303569.png)

*如上图红线所示,使用ITR作外部时钟走外部时钟模式1的路线;如上图所示蓝线,TRGO输出可通向其他定时器,如ITR*

ITR0-3分别来自其他定时器的TRGO输出,如下图所示

![TIMx内部触发连接](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511032603568.png)

*如上图所示,TIM2的ITR0接在了TIM1的TRGO上,TIM2的ITR1接在TIM8的TRGO上等等*

通过TIMx内部触发连接的关系可以实现定时器级联的功能,*比如可以初始化TIM3,使用主模式把它的更新事件映射到TRGO上,再初始化TIM2,选择ITR2对应着TIM3的TRGO,再选择时钟为外部时钟模式1*,这样TIM3的更新事件就可以驱动TIM2的时基单元,实现定时器级联的功能.

也可通过**TI1F_ED信号**作外部时钟走外部时钟模式1,如下图所示

![TI1F_ED作外部时钟走外部时钟模式1](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511033311304.png)

*如上图红线所示,使用TI1F_ED作外部时钟走外部时钟模式1的路线,ED是Edge边沿的意思,通过这一路输入的时钟,上升沿和下降沿均有效*

也可通过**TI1FP1和TI2FP2**作外部时钟走外部时钟模式1,如下图所示

![TI1FP1和TI2FP2作外部时钟走外部时钟模式1](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511033756964.png)

*如上图红线所示,使用TI1FP1和TI2FP2作外部时钟走外部时钟模式1的路线*

总结:**对于通用定时器可选择外部时钟如ETR引脚,其他定时器,CH1引脚的边沿,CH1引脚和CH2引脚.**

![编码器接口和TRGO](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511034105713.png)

- 编码器接口可以读取正交编码器的输出波形
- 可以将内部一些事件映射到TRGO引脚上用于触发其他定时器,DAC/ADC


----------------------------------------------------------------------

通用定时器捕获和比较部分:

![通用定时器捕获和比较部分](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250506130446268.png)

- 右侧为输出比较电路,总共有四个通道,分别对应CH1到CH4的引脚,可以用于输出PWM波形,驱动电机.
- 左侧为输入捕获电路,有四个通道,分别对应CH1到CH4的引脚,可以用于测量输入方波的频率等.
- 中间捕获/比较寄存器是两侧电路共用的

### 高级定时器

![高级定时器结构图](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250506143316492.png)

重复次数计数器: 可以实现每隔几个计数周期