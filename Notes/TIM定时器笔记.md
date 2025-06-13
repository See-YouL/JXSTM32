# TIM 定时器笔记

## 目录结构

[TIM定时中断部分理论知识视频链接]( https://www.bilibili.com/video/BV1th411z7sn/?p=13&share_source=copy_web&vd_source=0db47c15b9f51dbaa4548ec2dc55dea4)

**TIM定时中断包含如下内容**

- TIM定时器简介
- 定时中断基本结构
- 时序图
- RCC时钟树

[定时器定时中断实验/定时器外部时钟实验视频链接]( https://www.bilibili.com/video/BV1th411z7sn/?p=14&share_source=copy_web&vd_source=0db47c15b9f51dbaa4548ec2dc55dea4)

**定时器定时中断实验/定时器外部中断实验包含如下内容**

- 定时器定时中断实验
- 定时器外部时钟实验

[TIM输出比较部分理论知识视频链]( https://www.bilibili.com/video/BV1th411z7sn/?p=15&share_source=copy_web&vd_source=0db47c15b9f51dbaa4548ec2dc55dea4)

**TIM输出比较包含如下内容**

- 定时器输出比较简介
- PWM简介
- 输出比较通道(通用定时器)
- PWM基本结构
- 输出比较通道(高级定时器)

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

## 定时器定时中断实验

工程文件目录: `6-1 定时器定时中断`

实验目标: **使用定时器每秒自增变量在OLED上显示**

在SYSTEM文件夹中新建timer文件夹用来存放关于定时器的代码文件

在timer文件夹中新建timer.c和timer.h文件

### 硬件连接

OLED连接

- SCK 接 PG12
- SDA 接 PD5
- RES 接 PD4
- DC 接 PD15
- CS 接 PD1

### 常用库函数介绍

在`stm32f10x_tim.h`文件中常用的库函数

```c
void TIM_DeInit(TIM_TypeDef* TIMx); // 恢复缺省配置
void TIM_TimeBaseInit(TIM_TypeDef* TIMx, TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct); // 时基单元初始化
void TIM_TimeBaseStructInit(TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct); // 把结构体变量赋一个默认值
void TIM_Cmd(TIM_TypeDef* TIMx, FunctionalState NewState); // 使能计数器
void TIM_ITConfig(TIM_TypeDef* TIMx, uint16_t TIM_IT, FunctionalState NewState); // 使能中断输出信号
void TIM_InternalClockConfig(TIM_TypeDef* TIMx); // 选择内部时钟模式
void TIM_ITRxExternalClockConfig(TIM_TypeDef* TIMx, uint16_t TIM_InputTriggerSource); // 选择ITRx其他定时器的时钟
void TIM_TIxExternalClockConfig(TIM_TypeDef* TIMx, uint16_t TIM_TIxExternalCLKSource,
                                uint16_t TIM_ICPolarity, uint16_t ICFilter); // 选择TIx捕获通道的时钟
void TIM_ETRClockMode1Config(TIM_TypeDef* TIMx, uint16_t TIM_ExtTRGPrescaler, uint16_t TIM_ExtTRGPolarity,
                             uint16_t ExtTRGFilter); // 选择ETR通过外部时钟模式1输入的时钟
void TIM_ETRClockMode2Config(TIM_TypeDef* TIMx, uint16_t TIM_ExtTRGPrescaler, 
                             uint16_t TIM_ExtTRGPolarity, uint16_t ExtTRGFilter); // 选择ETR通过外部时钟模式2输入的时钟
void TIM_ETRConfig(TIM_TypeDef* TIMx, uint16_t TIM_ExtTRGPrescaler, uint16_t TIM_ExtTRGPolarity,
                   uint16_t ExtTRGFilter); // 配置ETR引脚的预分频器,极性和滤波器参数
void TIM_PrescalerConfig(TIM_TypeDef* TIMx, uint16_t Prescaler, uint16_t TIM_PSCReloadMode); // 单独修改预分频值
void TIM_CounterModeConfig(TIM_TypeDef* TIMx, uint16_t TIM_CounterMode); // 改变计数器的计数模式
void TIM_ARRPreloadConfig(TIM_TypeDef* TIMx, FunctionalState NewState); // 自动重装寄存器预装功能配置
void TIM_SetCounter(TIM_TypeDef* TIMx, uint16_t Counter); // 给计数器写入一个值
void TIM_SetAutoreload(TIM_TypeDef* TIMx, uint16_t Autoreload); // 给自动重装寄存器手动写一个值
uint16_t TIM_GetCounter(TIM_TypeDef* TIMx); // 获取当前计数器的值
uint16_t TIM_GetPrescaler(TIM_TypeDef* TIMx); // 获取当前预分频器的值
/*
 * 下面四个函数为获取和清楚标志位的函数
 */
FlagStatus TIM_GetFlagStatus(TIM_TypeDef* TIMx, uint16_t TIM_FLAG);
void TIM_ClearFlag(TIM_TypeDef* TIMx, uint16_t TIM_FLAG);
ITStatus TIM_GetITStatus(TIM_TypeDef* TIMx, uint16_t TIM_IT);
void TIM_ClearITPendingBit(TIM_TypeDef* TIMx, uint16_t TIM_IT);

```

****

时基单元时钟选择部分函数对应的流程图如下

`void TIM_InternalClockConfig(TIM_TypeDef* TIMx); // 选择内部时钟模式`如下图所示

![选择内部时钟模式](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250512181243466.png)

`void TIM_ITRxExternalClockConfig(TIM_TypeDef* TIMx, uint16_t TIM_InputTriggerSource); // 选择ITRx其他定时器的时钟`如下图所示

![选择ITRx其他定时器的时钟](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250512181539334.png)

`void TIM_TIxExternalClockConfig(TIM_TypeDef* TIMx, uint16_t TIM_TIxExternalCLKSource,uint16_t TIM_ICPolarity, uint16_t ICFilter); // 选择TIx捕获通道的时钟`如下图所示

![选择TIx捕获通道的时钟](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250512181839401.png)

`void TIM_ETRClockMode1Config(TIM_TypeDef* TIMx, uint16_t TIM_ExtTRGPrescaler, uint16_t TIM_ExtTRGPolarity,uint16_t ExtTRGFilter); // 选择ETR通过外部时钟模式1输入的时钟`如下图所示

![选择ETR通过外部时钟模式1输入的时钟](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250512182258546.png)

`void TIM_ETRClockMode2Config(TIM_TypeDef* TIMx, uint16_t TIM_ExtTRGPrescaler,uint16_t TIM_ExtTRGPolarity, uint16_t ExtTRGFilter); // 选择ETR通过外部时钟模式2输入的时钟`如下图所示

![选择ETR通过外部时钟模式2输入的时钟](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250512182434730.png)

### 初始化定时器流程

![初始化定时器流程](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250512175339510.png)

在`timer.c`文件中初始化定时器,大体步骤如下

1. RCC开启时钟
2. 选择时基单元的时钟源为内部时钟源
3. 配置时基单元包括预分频器,自动重装器,计数模式等
4. 配置输出中断控制允许更新中断输出到NVIC
5. 配置NVIC,在NVIC中打开定时器中断的通道,并分配一个优先级
6. 运行控制,使能计数器
7. 写一个定时器2中断函数

```c
// 初始化TIM2
void Timer_Init(void)
{
    // 1. 开启时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // 开启TIM2时钟

    // 2. 选择时基单元时钟(默认使用内部时钟)
    TIM_InternalClockConfig(TIM2); // 选择内部时钟

    // 3. 配置时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 滤波采样频率一分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1; // 在10k的频率下记10000个数需要1s时间
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1; // 对72M进行7200分频得到10kHz
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // 重复计数器(高级定时器才有的),这里不用,赋值0
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure); // 初始化TIM2时基单元

    // 4. 使能更新中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // 使能TIM2更新中断

    // 5. 配置NVIC
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置NVIC分组
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; // 定时器2中断通道
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	// 使能NVIC线路
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; // 响应优先级
    NVIC_Init(&NVIC_InitStructure); // 初始化NVIC

    // 6. 使能TIM2计数器
    TIM_Cmd(TIM2, ENABLE); // 使能TIM2计数器

}

// 7. 定时器2中断函数
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) // 判断是否为更新中断
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 清除更新中断标志位
        // 这里写定时器中断要执行的代码
    }
}

```

TIM_TimeBaseInitTypeDef结构体中`TIM_ClockDivision`成员的作用:

![输入滤波](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250513013833761.png)

上图所示的输入滤波器在一个固定的时钟频率f下进行采样,如果连续N个采样点都为相同的电平,则表示输入信号稳定了,就把这个采样值输出出去;如果这N个采样点不全都相同,就说明信号有抖动,这时就保持上一次的输出或者直接输出低电平,这样就能保证输出信号在一定程度上的滤波.

上述中的采样频率f可以由内部时钟直接而来,也可是由内部时钟加一个时钟分频而来,**其中分频多少就由TIM_ClockDivision成员来决定**.

计时时间的计算公式和参数选择如下:

`T = 1 / CK_CNT_OV = 1 / (CK_PSC / (PSC + 1) / (ARR + 1))`

若实现1s的定时,则CK_CNT_OV=1Hz,CK_PSC=72MHz,PSC=7200-1,ARR=10000-1

```c
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;
```

也可理解为,对72M进行7200分频得到10kHz,在10k的频率下记10000个数需要1s时间

### 主函数完善流程

在`main.c`文件中实现

1. 定义计数变量Num
2. 调用初始化定时器的函数
3. 在主函数中调用OLED显示Num的值


```c
/*
 * 实验目标: 使用定时器每秒自增变量在OLED上显示
 */
 
#include "stm32f10x.h"                  // Device header
#include "stm32f10x_conf.h"
#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
#include "timer.h"

// 1. 定义计数变量Num
uint16_t Num = 0; // 定义一个变量用来计数

int main(void)
{
	delay_init(); // 初始化延时函数
	OLED_Init(); // 初始化OLED

    // 2. 调用初始化定时器的函数
	Timer_Init(); // 初始化定时器2
	
    // 3. 在主函数中调用OLED显示Num的值
	OLED_Clear(); // 清屏
    OLED_ShowString(1, 1, "Num = ", 12); // 显示"Num ="

	while(1)
	{
        OLED_ShowNum(40, 1, Num, 5, 12); // 显示Num的值
		OLED_Refresh();
	}
}

```

### 完善中断服务函数

在`TIM2_IRQHandler`中断函数中实现Num自增,在`timer.c`文件中实现

```c
extern uint16_t Num; // 在timer.c中声明跨文件Num变量

// 定时器2中断函数
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) // 判断是否为更新中断
    {
        Num++;
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 清除更新中断标志位
    }
}

```

使用`extern`关键字可以在其他文件中引用Num变量,这样就可以在定时器中断函数中自增Num变量了.

### 复位立刻进入中断的问题

**问题描述:** 经测试,在如上代码的条件下进行测试,发现复位后Num的值是从1开始而不是从0开始,说明复位立刻进入了TIM2中断函数,这时Num的值已经自增了,所以Num的值是1,而不是0.

**问题成因分析:** 在`TIM_TimeBaseInit`函数函数中,如下图所示

![TIM_TimeBaseInit函数截图](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250513031817121.png)

注释内容: 立刻生成一个更新事件以重新装载预分频器和重复计数器的值.

预分频器有个缓冲寄存器,我们写的值只有在更新事件时才会真正起作用,所以这里为了让值立刻起作用,就在这里立刻生成一个更新事件,这时就会触发TIM2中断函数,所以Num的值自增了1. 

**解决方法:** 在`TIM_TimeBaseInit`函数之后,开启中断之前,手动调用`TIM_ClearFlag`函数,将更新中断标志位手动清除一下.

即在3.配置时基单元和4.使能更新中断之间加入`TIM_ClearFlag(TIM2, TIM_FLAG_Update)`

在`timer.c`文件中实现如下

```c
// 初始化TIM2
void Timer_Init(void)
{
    // 1. 开启时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // 开启TIM2时钟

    // 2. 选择时基单元时钟(默认使用内部时钟)
    TIM_InternalClockConfig(TIM2); // 选择内部时钟

    // 3. 配置时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 滤波采样频率一分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1; // 在10k的频率下记10000个数需要1s时间
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1; // 对72M进行7200分频得到10kHz
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // 重复计数器(高级定时器才有的),这里不用,赋值0
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure); // 初始化TIM2时基单元

    // 避免复位立刻进入中断
    TIM_ClearFlag(TIM2, TIM_FLAG_Update); // 清除TIM2更新中断标志位

    // 4. 使能更新中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // 使能TIM2更新中断

    // 5. 配置NVIC
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置NVIC分组
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; // 定时器2中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	// 使能NVIC线路
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; // 响应优先级
    NVIC_Init(&NVIC_InitStructure); // 初始化NVIC

    // 6. 使能TIM2计数器
    TIM_Cmd(TIM2, ENABLE); // 使能TIM2计数器

}
```

此时复位后Num的值就为0了,不会立刻进入TIM2中断函数了.

## 定时器外部时钟实验

工程文件目录: `6-2 定时器外部时钟`

实验目标: **每3次火焰传感器触发一次中断,在OLED上显示计数器的值**

### 硬件连接

OLED连接

- SCK 接 PG12
- SDA 接 PD5
- RES 接 PD4
- DC 接 PD15
- CS 接 PD1

火焰传感器连接

- DO 接 PA0

### 更换时钟源

火焰传感器:当火焰强度超过阈值时,输出低电平,否则输出高电平,所以需要配置为下降沿触发

TIM2的ETR引脚固定为PA0,故将火焰传感器的输出引脚连接到PA0,将PA0引脚设置为TIM2的ETR输入引脚,并且选择外部时钟模式2,下降沿有效

在`timer.c`文件中,将时钟源改为外部时钟

```c
// 初始化TIM2外部时钟PA0
void Timer_Init(void)
{

    // 1. 开启时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // 开启TIM2时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启GPIOA时钟

    // PA0引脚配置
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; // PA0引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure); // 初始化PA0引脚

    // 2. 选择时基单元时钟(默认使用内部时钟)
    TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_Inverted, 0x00); // 配置TIM2使用ETR时钟走外部时钟模式2,下降沿有效 

    // 3. 配置时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 滤波采样频率一分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = 3 - 1; // 在72MHz的频率下记3个数
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1; // 对72M进行1分频得到72MHz
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // 重复计数器(高级定时器才有的),这里不用,赋值0
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure); // 初始化TIM2时基单元

    // 避免复位立刻进入中断
    TIM_ClearFlag(TIM2, TIM_FLAG_Update); // 清除TIM2更新中断标志位

    // 4. 使能更新中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // 使能TIM2更新中断

    // 5. 配置NVIC
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置NVIC分组
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; // 定时器2中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	// 使能NVIC线路
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; // 响应优先级
    NVIC_Init(&NVIC_InitStructure); // 初始化NVIC

    // 6. 使能TIM2计数器
    TIM_Cmd(TIM2, ENABLE); // 使能TIM2计数器

}
```

关于PA0引脚配置的GPIO模式的选择,可参考下图

![GPIO模式选择](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250513035334225.png)

### 封装CNT计数器读取函数

在`timer.c`文件中封装一个读取计数器的函数,在`timer.c`文件中实现

```c
uint16_t Timer_GetCounter(void)
{
    return TIM_GetCounter(TIM2); // 读取TIM2计数器的值
}
```

### 完善主函数流程

增加显示计数器的部分,在`main.c`文件中实现

```c
/*
 * 实验目标: 每3次火焰传感器触发一次中断,在OLED上显示计数器的值
 * 硬件连接:
 *  OLED连接
 * - SCK 接 PG12
 * - SDA 接 PD5
 * - RES 接 PD4
 * - DC 接 PD15
 * - CS 接 PD1
 *  火焰传感器连接
 * - DO 接 PA0
 */
 
#include "stm32f10x.h"                  // Device header
#include "stm32f10x_conf.h"
#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
#include "timer.h"

// 1. 定义计数变量Num
uint16_t Num = 0; // 定义一个变量用来计数

int main(void)
{
    delay_init(); // 初始化延时函数
	OLED_Init(); // 初始化OLED
	Timer_Init(); // 初始化定时器2
	
	OLED_Clear(); // 清屏
    OLED_ShowString(1, 1, "Num = ", 12); // 显示"Num ="
    OLED_ShowString(2, 1, "CNT = ", 12); // 显示"CNT ="

	while(1)
	{
        OLED_ShowNum(40, 1, Num, 5, 12); // 显示Num的值
        OLED_ShowNum(40, 2, Timer_GetCounter(), 5, 12); // 显示计数器的值
		OLED_Refresh();
	}
}

```

## 定时器输出比较简介

OC(Output Compare)输出比较是定时器的一个功能,可以用来产生PWM波形,控制外设的输出等.

输出比较可以通过比较CNT与CCR寄存器值的关系,来对输出电平进行置1,置0或翻转的操作,用于输出一定频率
和占空比的PWM波形

CNT与CCR寄存器的关系如下图所示

![CNT与CCR寄存器](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250613165759.png)

- CNT是时基单元里的计数器
- CCR是捕获/比较寄存器,该寄存器是输入捕获和输出比较公用的
- 通过比较CNT和CCR的值,其中CNT计数自增,CCR是给定的值,当CNT大于,小于,等于CCR时,可以对输出进行置1,置0等操作
- 从而在TIMx_CH1~TIMx_CH4引脚上产生PWM波形

****

每个高级定时器和通用定时器都拥有4个输出比较通道,如下图所示

![基本定时器无输出比较功能](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250613171429.png)

![通用定时器有4个输出比较通道](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250613171602.png)

![高级定时器有4个输出比较通道](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250613171713.png)

****

高级定时器的前3个通道额外拥有死区生成和互补输出的功能,如下图所示

![死区生成和互补输出功能](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250613171832.png)   

## PWM简介

PWM(Pulse Width Modulation)脉冲宽度调制

在具有惯性的系统中,可以通过对一系列脉冲的宽度进行调制,来等效地获得所需要的模拟参量,常用于电机控速等领域.

![PWM等效模拟](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250613172702.png)

*如上图所示,黑色的数字信号为实际信号,紫色的正弦波信号为通过PWM的等效出来的模拟信号*

PWM参数:

- 频率 = 1 / Ts
- 占空比 = Ton / Ts
- 分辨率 = 占空比变化步距, 如分辨率可以1.1%,1.2%,1.3%跳变则分辨率为0.1w

![PWM参数](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250613173310.png)

## 输出比较通道(通用定时器)

![输出比较通道通用定时器](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250613174155.png)

*上图捕获/比较通道的输出部分在通用定时器框图中的对应关系如下图所示*

![在通用定时器框图中的对应关系](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250613174726.png)

*如上图所示,输入部分为CNT与CCR的比较结果,输出部分为TIMx_CH1~TIMx_CH4引脚的输出*

![流程分析](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250613180753.png)

1. 通过比较CNT与CCR1的大小关系,输出模式控制器会改变oc1ref的高低电平
2. oc1ref信号可以被映射到主模式的TRGO输出
3. TIMx_CCER为极性选择,写0信号会往上走即电平不翻转,写1会经过逻辑非门即电平翻转
4. 输出使能电路,选择是否输出
5. OC1引脚就是CH1通道的引脚,在引脚定义表里就可以知道具体是哪个GPIO口

### 输出比较模式

![输出比较模式对应](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250613181411.png)

*如上图所示,输出比较模式对应该框图中输出模式控制器的执行逻辑*

![输出比较模式](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250613181448.png)

输出模式控制器**输入为CNT与CCR的比较结果,输出为oc1ref的高低电平**,可选择多种模式来控制REF输出,**模式的选择可以通过寄存器TIMx_CCMR1来进行配置**

## PWM基本结构

![PWM基本结构框图](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250613184816.png)

![PWM波形分析](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250613185159.png)

1. 在1阶段时, CNT < CCR, REF置有效电平
2. 在2阶段时, CNT >= CCR, REF置无效电平

*如上图所示, 通过调节CCR的值即可调节PWM波的占空比*

### PWM的参数计算

![PWM的参数计算](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250613191338.png)

*如上图所示,PWM的频率 = 计数器的更新频率*

## 输出比较通道(高级定时器)

![输出比较通道(高级定时器)](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250613192252.png)

*如上图所示,高级定时器的输出比较通道与通用定时器的输出比较通道的区别在于,高级定时器的输出比较通道有死区生成和互补输出的功能, 即上图中红框部分*

![推挽电路](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250613193511.png)

*高级定时器的输出比较电路的输出一般会接上图所示的推挽电路*

1. Q1导通,Q2断开,输出高电平
2. Q1断开,Q2导通,输出低电平
3. Q1和Q2都断开,输出高阻态
4. Q1和Q2都导通,输出短路,不允许

**两个推挽电路构成H桥电路可以控制直流电机正反转;三个推挽电路可以驱动三相无刷电机**,OC1和OC1N为互补的的输出端口,分别控制上下管的导通和关断.

在切换上下管导通状态时,可能因为器件的不理想出现短暂的上下管同事导通的现象,会导致功率损耗,引起器件发热.**为避免这个问题,使用死区生成电路会在上下管切换时,在切换的过程中插入一个死区时间,使得上下管不会同时导通.**




