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

****

![内部时钟源选择](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250510234004213.png)

通用定时器也可选择内部时钟也就是系统频率72MHz

****

![外部时钟模式2](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250510234454628.png)

*如上图所示,选择外部时钟TIMx_ETR作为外部时钟源->预分频器->输入滤波->触发控制器->时基单元,这一路也被叫作外部时钟模式2*

选择外部时钟源为TIMx_ETR, TIMx_ETR引脚的映射参考引脚映射表,如下

![TIM2_CH1_ETR映射表](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511030909964.png)

*如上图所示, TIM1_CH1_ETR映射在PA0引脚上*

****

也可通过TRGI来提供时钟,主要用作触发输入来使用,也可作为外部时钟来使用,当TRGI当做外部时钟使用时,这一路被叫做"外部时钟模式1",如下图所示

![TRGI作为外部时钟](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511031756751.png)

可通过TIMx_ETR作外部时钟走外部时钟模式1,此时会占用触发输入通道,如下图所示

![ETR作外部输入走外部时钟模式1](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511032102900.png)

****

也可通过**ITR信号**作外部时钟走外部时钟模式1,如下图所示

![ITR作外部时钟走外部时钟模式1](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511032303569.png)

*如上图红线所示,使用ITR作外部时钟走外部时钟模式1的路线;如上图所示蓝线,TRGO输出可通向其他定时器,如ITR*

ITR0-3分别来自其他定时器的TRGO输出,如下图所示

![TIMx内部触发连接](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511032603568.png)

*如上图所示,TIM2的ITR0接在了TIM1的TRGO上,TIM2的ITR1接在TIM8的TRGO上等等*

通过TIMx内部触发连接的关系可以实现定时器级联的功能,*比如可以初始化TIM3,使用主模式把它的更新事件映射到TRGO上,再初始化TIM2,选择ITR2对应着TIM3的TRGO,再选择时钟为外部时钟模式1*,这样TIM3的更新事件就可以驱动TIM2的时基单元,实现定时器级联的功能.

****

也可通过**TI1F_ED信号**作外部时钟走外部时钟模式1,如下图所示

![TI1F_ED作外部时钟走外部时钟模式1](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511033311304.png)

*如上图红线所示,使用TI1F_ED作外部时钟走外部时钟模式1的路线,ED是Edge边沿的意思,通过这一路输入的时钟,上升沿和下降沿均有效*

****

也可通过**TI1FP1和TI2FP2**作外部时钟走外部时钟模式1,如下图所示

![TI1FP1和TI2FP2作外部时钟走外部时钟模式1](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511033756964.png)

*如上图红线所示,使用TI1FP1和TI2FP2作外部时钟走外部时钟模式1的路线*

总结:**对于通用定时器可选择外部时钟如ETR引脚,其他定时器,CH1引脚的边沿,CH1引脚和CH2引脚.**

![编码器接口和TRGO](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511034105713.png)

- 编码器接口可以读取正交编码器的输出波形
- 可以将内部一些事件映射到TRGO引脚上用于触发其他定时器,DAC/ADC

输出比较电路如下图所示

![输出比较电路](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511230809940.png)

可以用于输出PWM波形，驱动电机

输入捕获电路如下图所示

![输入捕获电路](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511230906765.png)

可以用于测量输入方波的频率等

### 高级定时器

![重复次数计数器](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511231424497.png)

重复次数计数器可以实现每隔几个计数周期才发生一次更新事件和更新中断.

![DTG和输出引脚](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511231741639.png)

- 输出引脚变为两个互补的输出,可以输出一对互补的PWM波,为了驱动三相无刷电机.
- DTG是死区生成电路,为了防止互补输出的PWM驱动桥臂时,在开关切换的瞬间,由于器件的不理想,造成短暂的直通现象.

![刹车输入](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511232232481.png)

刹车输入为了给电机驱动提供安全保障,如果外部引脚TIMx_BKIN产生了刹车信号或者内部时钟产生失效事件,那么控制电路就会自动切断电机的输出,防止以外的发生.

## 定时中断基本结构

![定时中断基本结构](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511232650059.png)

## 时序图

### 预分频器时序

![预分频器时序CK_PSC](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511234217282.png)

![基本定时器框图CK_PSC](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511234340343.png)

*其中CK_PSC为预分频器的输入时钟,选内部时钟则为72MHz,不断运行*

![预分频器时序CNT_EN](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511234859878.png)

CNT_EN为计数器使能,高电平时计数器正常运行,低电平计数器停止,*如上图所示,在红线时CNT_EN使能,计数器正常运行*


![预分频器时序CK_CNT](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511235928683.png)

*当CNT_EN为低电平时,定时器时钟未使能,当CNT_EN为高电平时,此时预分频器系数为1,定时器时序与CK_PSC一致,当预分频器系数变为2时,定时器时序为CK_PSC的一半*

![基本定时器框图CK_CNT](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511235258985.png)

*CK_CNT为计数器时钟,既是预分频器PSC的时钟输出,又是CNT计数器的时钟输入*

![预分频器时序计数器寄存器](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250512000545400.png)

1. 在1阶段时,CNT_EN未使能
2. 在2阶段时,CNT_EN使能,但是下一个CK_PSC脉冲信号还未到来,定时器时钟仍未启动
3. 在3阶段时,CK_PSC脉冲信号到来,定时器时钟启动,计数器开始计数,同时计数器寄存器开始自增
4. 在4阶段时,计数器寄存器自增到目标值,产生更新事件,并且计数器寄存器清零
5. 在5阶段时,涉及到预分频系数的改变,在下面预分频控制寄存器中介绍

![预分频控制寄存器](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250512062429835.png)

1. 在1阶段时,预分频控制寄存器为0,预分频器系数为1,定时器时钟为CK_PSC的一分频
2. 在2阶段时,预分频控制寄存器为1,预分频器系数为2,但是由于计数器寄存器正在执行一分频的计数功能,为避免数据混乱,预分频器系数的改变要等到计数器寄存器清零后才会生效,此时预分频缓冲器仍为0
3. 在3阶段时,计数器寄存器清零,预分频器系数生效,定时器时钟为CK_PSC的二分频,此时预分频缓冲器为1

![预分频计数器](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250512063042163.png)

1. 在1阶段时,预分频缓冲器为0,预分频计数器为0,直接输出原频率
2. 在2阶段时,预分频缓冲器为1,预分频计数器就01,01进行计数,在回到0时输出脉冲,从而实现二分频

计数器计数频率`CK_CNT = CK_PSC / (PSC + 1)`

- CK_CNT: 计数器时钟频率
- CK_PSC: 预分频器时钟频率
- PSC: 预分频器的值

### 计数器时序

![计数器时序](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250512063943737.png)

1. 在1阶段时,CNT_EN未使能,计数器停止
2. 在2阶段时,CNT_EN使能,计数器开始计数,计数器寄存器自增
3. 在3阶段时,计数器寄存器自增到目标值,产生计数器溢出,更新事件和更新标志位,并且计数器寄存器清零

计数器溢出频率`CK_CNT_OV = CK_CNT / (ARR + 1) = CK_PSC / (PSC+1) / (ARR + 1)`

由计数器溢出频率公式可计算定时时间 ` T = 1 / CK_CNT_OV`

### 计数器无预装时序

当APPEN为0时,计数器无预装,如下图所示

![计数器无预装时序](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250512064805010.png)

1. 在1阶段时,CNT_EN未使能,计数器停止
2. 在2阶段时,CNT_EN使能,计数器开始计数,计数器寄存器自增
3. 在3阶段时,自动加载寄存器由FF修改为36,目标值即刻生效,计数器寄存器自增到36时发生溢出并且产生更新事件和更新中断标志位
4. 在4阶段时,计数器寄存器清零,计数器寄存器重新开始自增计数

计数器无语装时序可能出现的问题: *以上图时序为例,当自动加载寄存器修改的值为30时,由于此刻的计数器寄存器的值已经大于30了,故计数器寄存器需要自增到最大值溢出到0再增到30才能产生更新事件*

### 计数器有预装时序

![计数器有预装时序](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250512065740864.png)

1. 在1阶段时,CNT_EN未使能,计数器停止
2. 在2阶段时,CNT_EN使能,计数器开始计数,计数器寄存器自增,且自动加载寄存器的值修改为36,此时计数的目标值不会立即生效
3. 在3阶段时,计数器寄存器自增到F5时,产生更新事件,此时自动加载影子寄存器的值改为36,这时计数的目标值生效
4. 在4阶段时,计数的目标值变为36

*引入影子寄存器的目的是为了让目标值的变化和更新事件同步发生,避免在计数途中进行更改造成错误*

## RCC时钟树

### 时钟产生电路分析

时钟树是STM32中用来产生和配置时钟并且把配置好的时钟发送到各个外设的系统

程序中主函数之前会执行一个SystemInit函数,这个函数就是用来配置时钟树的.

![时钟产生电路](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250512165249464.png)

如上图所示,时钟产生电路有四个震荡源

1. HSI RC: 内部8MHz高速RC振荡器,用来提供系统时钟
2. HSE OSC: 外部4-16MHz高速石英晶体振荡器,一般接8M,用来提供系统时钟
3. LES OSC: 外部32.768kHz低速石晶振,用来提供RTC时钟
4. LSI RC: 内部40kHz低速RC振荡器,用来提供看门狗时钟

在SystemInit函数里,ST先启动内部8MHz高速RC振荡器来提供8MHz的系统时钟,如下图所示

![内部8MHz高速RC振荡器提供系统时钟](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250512165851986.png)

然后启动外部时钟,进入PLL锁相环进行倍频得到72MHz的系统时钟,并且屏蔽内部时钟,如下图所示

![外部8MHz高速石英晶振提供系统时钟](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250512170158346.png)

如果此时外部时钟出现问题,会自动切换到内部时钟,系统频率为8MHz

![CSS](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250512170533432.png)

CSS: Clock Security System,时钟安全系统,可以监测外部时钟的运行状态,一旦外部时钟出现问题,会自动切换到内部时钟,系统频率为8MHz,从而保证系统时钟的运行,防止程序卡死造成事故.

### 时钟分配电路分析

![时钟分配电路分析](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250512171745601.png)

如上图红线所示,以72MHz系统时钟为例

1. SYSCLK进入AHB预分频器进行/1分频,此时频率为 72MHz / 1 = 72MHz
2. AHB预分频器的输出进入APB1预分频器进行/2分频,此时频率为 72MHz / 2 = 36MHz至APB1外设
3. 因APB1预分频系数不为1,所以频率x2,得到频率36MHz * 2 = 72MHz至定时器2~7

如上图蓝线所示,以72MHz系统时钟为例

1. SYSCLK进入AHB预分频器进行/1分频,此时频率为 72MHz / 1 = 72MHz
2. AHB预分频器的输出进入APB2预分频器进行/1分频,此时频率为 72MHz / 1 = 72MHz至APB2外设
3. 因APB2预分频系数为1,所以频率不变,得到频率72MHz至定时器1和定时器8

综上所述, 所有的定时器均为72MHz

![外设时钟使能](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250512172429273.png)

如上图所示,外设时钟使能就是程序中写RCC_APB2PeriphClockCmd和RCC_APB1PeriphClockCmd函数的地方,使能定时器的时钟,使能后定时器才能工作.
