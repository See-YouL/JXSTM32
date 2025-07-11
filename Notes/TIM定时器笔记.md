# TIM 定时器笔记

## 目录结构

[TIM定时中断部分理论知识视频链接]( https://www.bilibili.com/video/BV1th411z7sn/?p=13&share_source=copy_web&vd_source=0db47c15b9f51dbaa4548ec2dc55dea4)

TIM定时中断包含如下内容

- TIM定时器简介
- 定时中断基本结构
- 时序图
- RCC时钟树

[定时器定时中断实验/定时器外部时钟实验视频链接]( https://www.bilibili.com/video/BV1th411z7sn/?p=14&share_source=copy_web&vd_source=0db47c15b9f51dbaa4548ec2dc55dea4)

定时器定时中断实验/定时器外部中断实验包含如下内容

- 定时器定时中断实验
- 定时器外部时钟实验

[TIM输出比较部分理论知识视频链]( https://www.bilibili.com/video/BV1th411z7sn/?p=15&share_source=copy_web&vd_source=0db47c15b9f51dbaa4548ec2dc55dea4)

TIM输出比较包含如下内容

- 定时器输出比较简介
- PWM简介
- 输出比较通道(通用定时器)
- PWM基本结构
- 输出比较通道(高级定时器)

[PWN驱动LED呼吸灯/PWM驱动舵机/PWM驱动直流电机实验视频链接]( https://www.bilibili.com/video/BV1th411z7sn/?p=16&share_source=copy_web&vd_source=0db47c15b9f51dbaa4548ec2dc55dea4)

PWM驱动LED呼吸灯/PWM驱动舵机/PWM驱动直流电机实验包含如下内容

- PWM驱动LED呼吸灯实验
- PWM驱动舵机实验
- PWM驱动直流电机实验

[定时器输入捕获部分理论知识视频链接]( https://www.bilibili.com/video/BV1th411z7sn/?p=17&share_source=copy_web&vd_source=0db47c15b9f51dbaa4548ec2dc55dea4)

定时器输入捕获包含如下内容

- TIM输入捕获

[输入捕获测频率/PWMI模式测频率占空比](https://www.bilibili.com/video/BV1th411z7sn/?p=18&share_source=copy_web&vd_source=0db47c15b9f51dbaa4548ec2dc55dea4)

输入捕获测频率/PWMI模式测频率占空比包含如下内容

- 输入捕获实验

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

如上图所示,选择外部时钟TIMx_ETR作为外部时钟源->预分频器->输入滤波->触发控制器->时基单元,这一路也被叫作外部时钟模式2

选择外部时钟源为TIMx_ETR, TIMx_ETR引脚的映射参考引脚映射表,如下

![TIM2_CH1_ETR映射表](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511030909964.png)

如上图所示, TIM1_CH1_ETR映射在PA0引脚上

****

也可通过TRGI来提供时钟,主要用作触发输入来使用,也可作为外部时钟来使用,当TRGI当做外部时钟使用时,这一路被叫做"外部时钟模式1",如下图所示

![TRGI作为外部时钟](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511031756751.png)

可通过TIMx_ETR作外部时钟走外部时钟模式1,此时会占用触发输入通道,如下图所示

![ETR作外部输入走外部时钟模式1](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511032102900.png)

****

也可通过**ITR信号**作外部时钟走外部时钟模式1,如下图所示

![ITR作外部时钟走外部时钟模式1](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511032303569.png)

如上图红线所示,使用ITR作外部时钟走外部时钟模式1的路线;如上图所示蓝线,TRGO输出可通向其他定时器,如ITR

ITR0-3分别来自其他定时器的TRGO输出,如下图所示

![TIMx内部触发连接](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511032603568.png)

如上图所示,TIM2的ITR0接在了TIM1的TRGO上,TIM2的ITR1接在TIM8的TRGO上等等

通过TIMx内部触发连接的关系可以实现定时器级联的功能,*比如可以初始化TIM3,使用主模式把它的更新事件映射到TRGO上,再初始化TIM2,选择ITR2对应着TIM3的TRGO,再选择时钟为外部时钟模式1*,这样TIM3的更新事件就可以驱动TIM2的时基单元,实现定时器级联的功能.

****

也可通过**TI1F_ED信号**作外部时钟走外部时钟模式1,如下图所示

![TI1F_ED作外部时钟走外部时钟模式1](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511033311304.png)

如上图红线所示,使用TI1F_ED作外部时钟走外部时钟模式1的路线,ED是Edge边沿的意思,通过这一路输入的时钟,上升沿和下降沿均有效

****

也可通过**TI1FP1和TI2FP2**作外部时钟走外部时钟模式1,如下图所示

![TI1FP1和TI2FP2作外部时钟走外部时钟模式1](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511033756964.png)

如上图红线所示,使用TI1FP1和TI2FP2作外部时钟走外部时钟模式1的路线

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

其中CK_PSC为预分频器的输入时钟,选内部时钟则为72MHz,不断运行

![预分频器时序CNT_EN](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511234859878.png)

CNT_EN为计数器使能,高电平时计数器正常运行,低电平计数器停止,*如上图所示,在红线时CNT_EN使能,计数器正常运行*

![预分频器时序CK_CNT](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511235928683.png)

当CNT_EN为低电平时,定时器时钟未使能,当CNT_EN为高电平时,此时预分频器系数为1,定时器时序与CK_PSC一致,当预分频器系数变为2时,定时器时序为CK_PSC的一半

![基本定时器框图CK_CNT](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511235258985.png)

CK_CNT为计数器时钟,既是预分频器PSC的时钟输出,又是CNT计数器的时钟输入

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

由计数器溢出频率公式可计算定时时间 `T = 1 / CK_CNT_OV`

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

引入影子寄存器的目的是为了让目标值的变化和更新事件同步发生，避免在计数途中进行更改造成错误。

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

### 硬件连接（定时中断实验）

[0.96寸7针OLED显示屏SPI接口资料](https://telesky.yuque.com/bdys8w/01/lw9nqcxkk0hffiuz)

[0.96寸7针OLED显示屏SPI接口购买链接](https://detail.tmall.com/item.htm?ali_trackid=2%3Amm_26632943_457000242_108858100157%2C123%3A1752247506496_557215226_0&bxsign=tbktZejeaT6MloquHvxd6qDPk26u0_fjPoLk-l99lLIojBUzU6GXGDVZfwsz_JGquPlwm-NjAy1u24jEoHABX5P66pHeEPjvDCsKELgAQ7fYNjx7p00OOtOzTUosTBnTp6lWxpb8rQk1N_KNQ2EoAbk98Hib81l4Fw96H8mWDcuaI8MWbHM7Mx2K2vk5h4jE_5O&id=42044259331&pvid=null&relationId=3131571508&rid=3131571508&scm=null&spm=a21wu.21508199.product.mm_26632943_457000242_108858100157&unid=123&union_lens=lensId%3AOPT%401752247504%4021508606_0db1_197fa170dde_b4fe%4001%40eyJmbG9vcklkIjoxMDYzOTN9%3Brecoveryid%3A557215226_0%401752247506499)

OLED连接

- SCK 接 PG12
- SDA 接 PD5
- RES 接 PD4
- DC 接 PD15
- CS 接 PD1

### 常用库函数(定时器中断部分)

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

### 硬件连接（外部时钟实验）

[0.96寸7针OLED显示屏SPI接口资料](https://telesky.yuque.com/bdys8w/01/lw9nqcxkk0hffiuz)

[0.96寸7针OLED显示屏SPI接口购买链接](https://detail.tmall.com/item.htm?ali_trackid=2%3Amm_26632943_457000242_108858100157%2C123%3A1752247506496_557215226_0&bxsign=tbktZejeaT6MloquHvxd6qDPk26u0_fjPoLk-l99lLIojBUzU6GXGDVZfwsz_JGquPlwm-NjAy1u24jEoHABX5P66pHeEPjvDCsKELgAQ7fYNjx7p00OOtOzTUosTBnTp6lWxpb8rQk1N_KNQ2EoAbk98Hib81l4Fw96H8mWDcuaI8MWbHM7Mx2K2vk5h4jE_5O&id=42044259331&pvid=null&relationId=3131571508&rid=3131571508&scm=null&spm=a21wu.21508199.product.mm_26632943_457000242_108858100157&unid=123&union_lens=lensId%3AOPT%401752247504%4021508606_0db1_197fa170dde_b4fe%4001%40eyJmbG9vcklkIjoxMDYzOTN9%3Brecoveryid%3A557215226_0%401752247506499)

OLED连接

- SCK 接 PG12
- SDA 接 PD5
- RES 接 PD4
- DC 接 PD15
- CS 接 PD1

[火焰传感器3针资料](https://telesky.yuque.com/bdys8w/01/qyw1o22t6g5e278p)

[火焰传感器3针购买链接](https://detail.tmall.com/item.htm?id=16492681878&pisk=g9s06ljbiZ8XW6PJygtb8xBMLUU8Dnt1b1n96hdazQR7Cq3YhFJNI1jOkOR9j1fGsILaGnIGZ96ThGeX0lvN9_xOHIdOZRfACcFb5nnZss6UGtdThGAwZs5M5-O9_CfOQZe8vkBfhhtwjWELvyylSn1mbFRaQQJeQ8p4FZrN8hta950-bEZHfsJvulmZzLRWIdlq01orzdAyQq-N_Q8yLph2b15w44Jpdclw3c8y4Q9i7cu274kyBKlq_1-aETRWQhJN_hlFccRhbgszwfO-D4Wrut9DoBW2gtQR458nlTOkhKIynTxC3ViZbgvDog8VBpvwkNWBRiB0j0tAKa-yn1rUg3WFr9LcmWmv1ssM5gLqQJT5UdYebUDZbTxJZUS2YklkFaWA7i1mIcJfDMLMOU2ZfFKPve7FiAwCUnJN1esTOmOVQ9sCJHqi4nYN413r8kijfLyO4Vg6uL9k93yRkgmuq0u_E80QlEJWHWeuEVgsOco3982oRjT2FKFd.&spm=tbpc.boughtlist.suborder_itempic.d16492681878.12852e8dOh0AD5)

火焰传感器连接

- DO 接 PA0

### 更换时钟源

火焰传感器:当火焰强度超过阈值时,输出低电平,否则输出高电平,所以需要配置为下降沿触发

**TIM2的ETR引脚固定为PA0**,故将火焰传感器的输出引脚连接到PA0,将PA0引脚设置为TIM2的ETR输入引脚,并且选择外部时钟模式2,下降沿有效

在STM32F103x的数据手册中第三节引脚定义中可查到默认复用功能,如下图所示

![数据手册](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250614193542.png)

在STM32F10x参考手册中8.3.7定时器复用功能重映射可找到TIM2的复用影响,如下图所示

![参考手册](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250614194006.png)

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

如上图所示,黑色的数字信号为实际信号,紫色的正弦波信号为通过PWM的等效出来的模拟信号

PWM参数:

- 频率 = 1 / Ts
- 占空比 = Ton / Ts
- 分辨率 = 占空比变化步距, 如分辨率可以1.1%,1.2%,1.3%跳变则分辨率为0.1w

![PWM参数](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250613173310.png)

## 输出比较通道(通用定时器)

![输出比较通道通用定时器](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250613174155.png)

上图捕获/比较通道的输出部分在通用定时器框图中的对应关系如下图所示

![在通用定时器框图中的对应关系](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250613174726.png)

如上图所示,输入部分为CNT与CCR的比较结果,输出部分为TIMx_CH1~TIMx_CH4引脚的输出

![流程分析](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250613180753.png)

1. 通过比较CNT与CCR1的大小关系,输出模式控制器会改变oc1ref的高低电平
2. oc1ref信号可以被映射到主模式的TRGO输出
3. TIMx_CCER为极性选择,写0信号会往上走即电平不翻转,写1会经过逻辑非门即电平翻转
4. 输出使能电路,选择是否输出
5. OC1引脚就是CH1通道的引脚,在引脚定义表里就可以知道具体是哪个GPIO口

### 输出比较模式

![输出比较模式对应](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250613181411.png)

如上图所示,输出比较模式对应该框图中输出模式控制器的执行逻辑

![输出比较模式](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250613181448.png)

输出模式控制器**输入为CNT与CCR的比较结果,输出为oc1ref的高低电平**,可选择多种模式来控制REF输出,**模式的选择可以通过寄存器TIMx_CCMR1来进行配置**

## PWM基本结构

![PWM基本结构框图](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250613184816.png)

![PWM波形分析](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250613185159.png)

1. 在1阶段时, CNT < CCR, REF置有效电平
2. 在2阶段时, CNT >= CCR, REF置无效电平

如上图所示, 通过调节CCR的值即可调节PWM波的占空比

### PWM的参数计算

![PWM的参数计算](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250613191338.png)

如上图所示,PWM的频率 = 计数器的更新频率

## 输出比较通道(高级定时器)

![输出比较通道(高级定时器)](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250613192252.png)

如上图所示,高级定时器的输出比较通道与通用定时器的输出比较通道的区别在于,高级定时器的输出比较通道有死区生成和互补输出的功能, 即上图中红框部分

![推挽电路](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250613193511.png)

高级定时器的输出比较电路的输出一般会接上图所示的推挽电路

1. Q1导通,Q2断开,输出高电平
2. Q1断开,Q2导通,输出低电平
3. Q1和Q2都断开,输出高阻态
4. Q1和Q2都导通,输出短路,不允许

**两个推挽电路构成H桥电路可以控制直流电机正反转;三个推挽电路可以驱动三相无刷电机**,OC1和OC1N为互补的的输出端口,分别控制上下管的导通和关断.

在切换上下管导通状态时,可能因为器件的不理想出现短暂的上下管同事导通的现象,会导致功率损耗,引起器件发热.**为避免这个问题,使用死区生成电路会在上下管切换时,在切换的过程中插入一个死区时间,使得上下管不会同时导通.**

## PWM驱动LED呼吸灯实验

工程文件目录: `6-3 PWM驱动LED呼吸灯`

实验目标: **实现PWM驱动LED呼吸灯**

在HARDWARE文件夹中添加`PWM.c`和`PWM.h`文件

### 硬件连接（PWM呼吸灯实验）

TIM2_CHx的复用功能重映射可在STM32F10x参考手册的**8.3.7定时器复用功能重映射**章节中查看,*如下图所示*

![TIM2_CH1的复用功能重映射](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250614195759.png)

[三色LED模块购买链接](https://detail.tmall.com/item.htm?ali_trackid=2%3Amm_26632943_457000242_108858100157%2C123%3A1752247698256_554130571_0&bxsign=tbkP7-hJm4UxHApHSTyxzoEhnh5jJpeZkkIGfhrRLEDYcb7cAMrnSTAQxTaipPF6iYeAysW6ucwiltB_wo_1vRuJsJyd5hcCoyPvmp7s1tzBCIDz2_n6SUJXsmP3tg48GC-yzjacDIYPZeX-lbqKmkrO9cgaskmmgQiCCwDLnTgAchg4crSVOvcmgonZ-7QdvZL&id=673190083230&pvid=null&relationId=3131571508&rid=3131571508&scm=null&spm=a21wu.21508199.product.mm_26632943_457000242_108858100157&unid=123&union_lens=lensId%3AOPT%401752247696%40213c9516_1ce0_197fa19fda9_88c8%4001%40eyJmbG9vcklkIjoxMDYzOTN9%3Brecoveryid%3A554130571_0%401752247698260)

LED连接:

- LED的正极 -> TIM2_CH1_ETR引脚(PA0)
- 负极 -> GND

### 常用库函数(PWM驱动LED呼吸灯实验)

在`stm32f10x_tim.h`文件中常用的库函数

```c
void TIM_OC1Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct);
void TIM_OC2Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct);
void TIM_OC3Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct);
void TIM_OC4Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct);
```

- TIMx: 选择定时器
- TIM_OCInitStruct: 结构体变量,输出比较的参数

`用来配置输出比较模块,如下图所示`

![输出比较模块](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250613231141.png)

****

```c
void TIM_OCStructInit(TIM_OCInitTypeDef* TIM_OCInitStruct);
```

用来给输出比较结构体赋默认值

****

```c
void TIM_ForcedOC1Config(TIM_TypeDef* TIMx, uint16_t TIM_ForcedAction);
void TIM_ForcedOC2Config(TIM_TypeDef* TIMx, uint16_t TIM_ForcedAction);
void TIM_ForcedOC3Config(TIM_TypeDef* TIMx, uint16_t TIM_ForcedAction);
void TIM_ForcedOC4Config(TIM_TypeDef* TIMx, uint16_t TIM_ForcedAction);
```

用来配置强制输出模式,**在运行中想要暂停输出波形并且强制输出高/低电平**可用该函数

****

```c
void TIM_OC1PreloadConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPreload);
void TIM_OC2PreloadConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPreload);
void TIM_OC3PreloadConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPreload);
void TIM_OC4PreloadConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPreload);
```

用来配置输出比较预装载功能,**在运行中想要修改CCR寄存器的值,需要开启预装载功能,否则修改的值不会生效**

****

```c
void TIM_OC1PolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPolarity);
void TIM_OC1NPolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCNPolarity);
void TIM_OC2PolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPolarity);
void TIM_OC2NPolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCNPolarity);
void TIM_OC3PolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPolarity);
void TIM_OC3NPolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCNPolarity);
void TIM_OC4PolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPolarity);
```

用来单独设置输出比较的极性,带N的为高级定时器里互补通道的配置

****

```c
void TIM_CCxCmd(TIM_TypeDef* TIMx, uint16_t TIM_Channel, uint16_t TIM_CCx);
void TIM_CCxNCmd(TIM_TypeDef* TIMx, uint16_t TIM_Channel, uint16_t TIM_CCxN);
```

用来单独修改输出使能参数

****

```c
void TIM_SelectOCxM(TIM_TypeDef* TIMx, uint16_t TIM_Channel, uint16_t TIM_OCMode);
```

用来单独更改输出比较模式的函数

****

```c
void TIM_SetCompare1(TIM_TypeDef* TIMx, uint16_t Compare1);
void TIM_SetCompare2(TIM_TypeDef* TIMx, uint16_t Compare2);
void TIM_SetCompare3(TIM_TypeDef* TIMx, uint16_t Compare3);
void TIM_SetCompare4(TIM_TypeDef* TIMx, uint16_t Compare4);
```

用来单独更改CCR寄存器值的函数,**更改占空比使用该函数**

****

```c
void TIM_CtrlPWMOutputs(TIM_TypeDef* TIMx, FunctionalState NewState);
```

仅高级定时器使用,在使用高级定时器输出PWM时需要调用该函数使能主输出,否则无法输出PWM波形

### PWM初始化函数(PWM驱动LED呼吸灯实验)

TIM2_CHx的复用功能重映射可在STM32F10x参考手册的**8.3.7定时器复用功能重映射**章节中查看,*如下图所示*

![TIM2_CH1的复用功能重映射](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250614195759.png)

TIM2_CHx的GPIO配置可在STM32F10x参考手册的**8.1.11外设的GPIO配置**章节中查看,*如下图所示*

![TIM2_CHx的GPIO配置](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250614195516.png)

所以, 应该使用**PA0作为LED灯的PWM输出引脚,将PA0引脚设置为TIM2_CH1的复用推挽输出模式**

```c
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启GPIOA时钟
GPIO_InitTypeDef GPIO_InitStructure; // 定义GPIO初始化结构体
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; // 选择PA0引脚
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 设置为复用推挽输出
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 设置GPIO速度为50MHz
GPIO_Init(GPIOA, &GPIO_InitStructure); // 初始化GPIOA
```

****

参数计算

- PWM频率计算公式: Freq = CK_PSC / (PSC + 1) / (ARR + 1)
- PWM占空比: Duty = CCR / (ARR + 1)
- PWM分辨率: Res = 1 / (ARR + 1)

若产生一个 **频率为1KHz,占空比为50%,分辨率为1%** 的PWM波形,则

- 1KHz = 72MHz / (PSC + 1) / (ARR + 1)
- 50% = CCR / (ARR + 1)
- 1% = 1 / (ARR + 1)

解得

- (ARR + 1) = 100
- CCR = 50
- (PSC + 1) = 720

```c
TIM_TimeBaseInitStructure.TIM_Period = 100 - 1; // ARR + 1 = 100  
TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1; // PSC + 1 = 720
TIM_OCInitStructure.TIM_Pulse = 50; // CCR = 50
```

****

PWM初始化流程:

1. RCC开启时钟,打开TIM外设和GPIO外设的时钟
2. 选择时基单元时钟源
3. 配置时基单元
4. 配置输出比较通道
5. 配置PWM对应的GPIO口为复用推挽输出模式
6. 运行控制,启动计数器

```c
void PWM_Init(void)
{
    // 1. 开启时钟, 打开TIM2外设和GPIO外设的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // 开启TIM2时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启GPIOA时钟

    // 2. 选择时基单元时钟源(默认使用内部时钟)
    TIM_InternalClockConfig(TIM2); // 选择内部时钟

    // 3. 配置时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 滤波采样频率一分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1; // ARR + 1 = 100  
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1; // PSC + 1 = 720
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // 重复计数器(高级定时器才有的),这里不用,赋值0
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure); // 初始化TIM2时基单元

    // 4. 配置输出比较通道
    TIM_OCInitTypeDef TIM_OCInitStructure; // 定义输出比较结构体
    TIM_OCStructInit(&TIM_OCInitStructure); // 初始化输出比较结构体
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; // 设置输出比较模式为PWM1
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // 设置输出极性为高
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 使能输出比较
    TIM_OCInitStructure.TIM_Pulse = 50; // CCR = 50
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);

    // 5. 配置PWM对应的GPIO
    GPIO_InitTypeDef GPIO_InitStructure; // 定义GPIO初始化结构体
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; // 选择PA0引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 设置为复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 设置GPIO速度为50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure); // 初始化GPIOA

    // 6. 运行控制,启动计数器
    TIM_Cmd(TIM2, ENABLE); // 使能TIM2计数器

}

```

### 实现呼吸灯效果

在`PWM.c`文件中实现一个设置CCR值的函数

```c
void PWM_SetCompare1(uint16_t Compare)
{
    TIM_SetCompare1(TIM2, Compare); // 设置TIM2通道1的比较值
}
```

在`main.c`文件中实现呼吸灯效果

```c
uint8_t i;

int main(void)
{
	delay_init();
    PWM_Init(); // TIM2的PWM初始化

	while(1)
	{
        for(i = 0; i <= 100; i++)
        {
            PWM_SetCompare1(i); // 设置PWM占空比
            Delay_ms(10); // 延时10ms
        }

        for(i = 0; i <= 100; i++)
        {
            PWM_SetCompare1(100 - i); // 设置PWM占空比
            Delay_ms(10); // 延时10ms
        }
	}
}
```

### 引脚重映射(TIM2_CH1_ETR)

硬件连接需要将LED正极连接到PA15引脚

![引脚重映射TIM2_CH1_ETR](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250618154154.png)

如图所示,TIM2_CH1_ETR引脚可以重映射到PA15引脚上

使用AFIO需要开启AFIO时钟,在`PWM_Init`函数中添加

```c
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // 开启AFIO时钟
```

需要用GPIO_PinRemapConfig函数来配置引脚重映射,在`PWM_Init`函数中添加

```c
GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE); // 部分重映射TIM2到PA15
```

GPIO_PinRemapConfig函数的第一个参数来确定重映射的类型

```c
/**
  *     @arg GPIO_PartialRemap1_TIM2     : TIM2 Partial1 Alternate Function mapping
  *     @arg GPIO_PartialRemap2_TIM2     : TIM2 Partial2 Alternate Function mapping
  *     @arg GPIO_FullRemap_TIM2         : TIM2 Full Alternate Function mapping
  **/
```

| 重映射类型 | 对应参数 |
| - | - |
| TIM2_REMAP[1:0] = 01 | GPIO_PartialRemap1_TIM2 |
| TIM2_REMAP[1:0] = 10 | GPIO_PartialRemap2_TIM2 |
| TIM2_REMAP[1:0] = 11 | GPIO_FullRemap_TIM2 |

上表对应下图

![重映射类型](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250618160226.png)

PA15引脚默认复用功能为JTDI调试引脚, 如下图所示

![PA15引脚默认复用功能](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250618160501.png)

需要关闭PA15引脚的调试功能,在`PWM_Init`函数中添加

```c
GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); // 禁用JTAG功能
```

解除PA15, PB3, PB4的JTAG复用功能, 使能SWD调试功能, 这样就可以将PA15引脚作为TIM2_CH1_ETR引脚使用了

其他关于调试功能默认复用的参数,在`stm32f10x_gpio.h`文件中可以找到,如下所示

```c
/**
  *     @arg GPIO_Remap_SWJ_NoJTRST      : Full SWJ Enabled (JTAG-DP + SW-DP) but without JTRST
  *     @arg GPIO_Remap_SWJ_JTAGDisable  : JTAG-DP Disabled and SW-DP Enabled
  *     @arg GPIO_Remap_SWJ_Disable      : Full SWJ Disabled (JTAG-DP + SW-DP)
  **/
```

| 重映射端口 | 对应参数 |
| - | - |
| PB4 | GPIO_Remap_SWJ_NoJTRST |
| PA15, PB3, PB4 | GPIO_Remap_SWJ_JTAGDisable |
| PA13, PA14, PA15, PB3, PB4 | GPIO_Remap_SWJ_Disable |

上表对应下图

![端口类型](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250618161752.png)

关于调试端口映像和IO引脚分配可以在STM32F10x参考手册中找到,如下图所示

![调试端口映像](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250618162404.png)

还需要在`PWM_Init`函数中配置PA15引脚为TIM2_CH1_ETR的复用推挽输出模式,如下所示

```c
GPIO_InitTypeDef GPIO_InitStructure; // 定义GPIO初始化结构体
//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; // 选择PA0引脚, TIM2_CH1默认映射到PA0
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; // 选择PA15引脚, TIM2_CH1_ETR重映射到PA15
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 设置为复用推挽输出
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 设置GPIO速度为50MHz
GPIO_Init(GPIOA, &GPIO_InitStructure); // 初始化GPIOA
```

## PWM驱动舵机实验

工程文件目录: `6-4 PWM驱动舵机`

实验目标: **实现按键按下增加舵机角度并在OLED显示当前角度**

### 硬件连接(PWM驱动舵机实验)

舵机型号为SG90 9G 90度-180度通用

[舵机资料](https://telesky.yuque.com/bdys8w/01/tq7mgxs352qi0kkb)

[SG90舵机购买链接](https://detail.tmall.com/item.htm?id=41248626148&pisk=glQg6fb_IG-_oSwpvwT_4fCDaHEdtFTfut3vHEp4YpJQGcnT1nRVitbAXsJvotXcndK4CFQcK_1Y1ZF67rAVe9YA6dpAKSXOGqe_cF3qnO1zC1pY1Zv2KOWDc59v3KXAgGFLyzC11ET2o8U8yuPhmF6iuh-4gHRJaWdaOGzV4ET4e-mKuhaklOR9bg3qYBJBiIkZ7tkELIvegc8V3p-eaQHwutW28DRJNqk2_q-e8pOy_IJquD-eZI0q3t84tHJBgERV3EleCqJluw_Upx9KWD8r4eOMjL5wQ1QN8PTZfs9hZ_7es6Yc_mgquwAMjw-NMQA2Xi5WVNCgo2TOZM8estzzQe5Px_KGS809hO_DcwKZg7tCTIxyuHcqu6YpKH7wzzkHOM5O0N6iiqR1WaKDFHVqlnLFy3SPIjN5TFRVh3_YFV9Ng__52U4m8FxV4PnE4zgslBPA8miXbBOHeePdXw03-2obtWm7fhRB68F3tmijFquueWVnVAtwOCeR.&spm=tbpc.boughtlist.suborder_itempic.d41248626148.12852e8dOh0AD5)

使用TIM2_CH2引脚来输出PWM波, TIM2_CH2默认复用为PA1引脚, 如下图所示

![TIM2_CH2默认复用](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250618182038.png)

舵机接线:

- VCC(舵机红色线) -> +5V
- GND(舵机棕色线) -> GND(需要与STM32的GND共地)
- PWM(舵机黄色线) -> TIM2_CH2引脚(PA1)

[0.96寸7针OLED显示屏SPI接口资料](https://telesky.yuque.com/bdys8w/01/lw9nqcxkk0hffiuz)

[0.96寸7针OLED显示屏SPI接口购买链接](https://detail.tmall.com/item.htm?ali_trackid=2%3Amm_26632943_457000242_108858100157%2C123%3A1752247506496_557215226_0&bxsign=tbktZejeaT6MloquHvxd6qDPk26u0_fjPoLk-l99lLIojBUzU6GXGDVZfwsz_JGquPlwm-NjAy1u24jEoHABX5P66pHeEPjvDCsKELgAQ7fYNjx7p00OOtOzTUosTBnTp6lWxpb8rQk1N_KNQ2EoAbk98Hib81l4Fw96H8mWDcuaI8MWbHM7Mx2K2vk5h4jE_5O&id=42044259331&pvid=null&relationId=3131571508&rid=3131571508&scm=null&spm=a21wu.21508199.product.mm_26632943_457000242_108858100157&unid=123&union_lens=lensId%3AOPT%401752247504%4021508606_0db1_197fa170dde_b4fe%4001%40eyJmbG9vcklkIjoxMDYzOTN9%3Brecoveryid%3A557215226_0%401752247506499)

OLED接线:

- GND -> GND(需要与STM32的GND共地)
- VDD -> 3.3V
- SCK -> PG12
- SDA -> PD5
- RES -> PD4
- DC -> PD15
- CS -> PD1

按键使用霸道开发板按键(PA0),如下图所示

![按键](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250618202248.png)

### PWM初始化函数(PWM驱动舵机)

修改`PWM_Init`函数,将PA1引脚设置为TIM2_CH2的复用推挽输出模式

```c
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; // 选择PA1引脚, TIM2_CH2默认映射到PA1
```

修改`PWM_Init`函数,将TIM2_CH2的输出比较通道配置为PWM2模式

```c
TIM_OC2Init(TIM2, &TIM_OCInitStructure); // 配置TIM2通道2的输出比较
```

也可使用不同通道一起初始化

```c
TIM_OC1Init(TIM2, &TIM_OCInitStructure); // 配置TIM2通道1的输出比较
TIM_OC2Init(TIM2, &TIM_OCInitStructure); // 配置TIM2通道2的输出比较
TIM_OC3Init(TIM2, &TIM_OCInitStructure); // 配置TIM2通道3的输出比较
TIM_OC4Init(TIM2, &TIM_OCInitStructure); // 配置TIM2通道4的输出比较
```

对于同一个定时器的不同通道输出的PWM波

1. 不同通道共用一个计数器, 频率一样
2. 占空比由各自的CCR决定, 可以各自设定
3. 由于计数器更新, 所有PWM同时跳变, 相位一致

如果驱动多个舵机或者直流电机, 使用一个定时器不同通道的PWM波形, 可以节省资源, 只需要一个计数器就可以了

新增`PWM_SetCompare2`函数, 用来设置TIM2_CH2的比较值

```c
void PWM_SetCompare2(uint16_t Compare)
{
    TIM_SetCompare2(TIM2, Compare); // 设置TIM2通道2的比较值
}
```

参数计算:

[参考舵机资料](https://telesky.yuque.com/bdys8w/01/tq7mgxs352qi0kkb)

![频率](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250618190139.png)

1. 舵机的工作频率为50Hz, 即20ms一个周期
2. 舵机高电平时长0.5ms到2.5ms, 占空比大致为2.5%到12.5%

根据公式可列出如下关系

- 50Hz = 72MHz / (PSC + 1) / (ARR + 1)
- 2.5%~12.5% = CCR / (ARR + 1)

可以取ARR + 1 = 20K, PSC + 1 = 72, CCR = 500~2500

1. ARR + 1 = 20000
2. PSC + 1 = 72
3. CCR = 500 ~ 2500 对应占空比 2.5% ~ 12.5%

对应进行参数修改

```c
TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1; // ARR + 1 = 20K
TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1; // PSC + 1 = 72
TIM_OCInitStructure.TIM_Pulse = 500; // CCR = 500
```

### 舵机角度函数

新建`servo.c`和`servo.h`文件, 封装舵机角度函数

在`servo.c`文件中实现舵机角度函数

```c
void Servo_SetAngle(float angle)
{
    if (angle > 180) // 限制角度在0~180度之间
    {
        angle = 180;
    }
    
    // 将角度转换为对应的CCR值
    // 0度对应500, 180度对应2500
    uint16_t ccr_value = 500 + (angle * (2500 - 500)) / 180; // 线性映射
    PWM_SetCompare2(ccr_value); // 设置TIM2_CH2的比较值
}
```

### 实现按键增加舵机角度并在OLED显示当前角度

在`main.c`文件中实现按键增加舵机角度并在OLED显示当前角度

```c
uint8_t KeyNum; // 接收键码的变量
float Angle; // 舵机角度变量

int main(void)
{
	delay_init(); // 延时初始化
	OLED_Init(); // OLED初始化
    Servo_Init(); // 舵机初始化
	
	OLED_Clear(); // OLED清屏
	OLED_ShowString(1, 11, "Angle:", 12); // 在(1, 11)位置显示"Angle:"字体大小12
	OLED_Refresh(); // 更新显存到OLED

	while(1)
	{
        KeyNum = KEY_GetNum(); // 获取按键值
        if (KeyNum == 1) // 按键按下
        {
            Angle += 10; // 每次按下增加10度
            if (Angle > 180) // 限制角度在0~180度之间
            {
                Angle = 0;
            }
            Servo_SetAngle(Angle); // 设置舵机角度
            OLED_ShowNum(41, 11, Angle, 3, 12); // 在(41, 11)位置显示当前角度, 字体大小12
            OLED_Refresh(); // 更新显存到OLED
        }
	}
}

```

## PWM驱动直流电机实验

工程文件目录: `6-5 PWM驱动直流电机`

实验目标: **实现按键控制直流电机转速并在OLED显示当前速度**

### 硬件连接

直流电机型号为**130小马达电动机**

[130小马达电动机淘宝链接](https://detail.tmall.com/item.htm?id=41310780866&pisk=gE_36hbQKM-Qnse9XNTQ0XCkghE9jeT5zT3JeUpzbdJ_NDnLO3RPZTbRv_JJrTXlEpKzdeQlIs18OaFB8zAPHOYR9ppRI7XdN4eQVe3rE914d6p8OavyI9WkVW9JULXRaMFTkrCCOUTyr-UYkoPGqe6nz3oUacR2t5dU1MzP0UTzH8mtzHaDP9Rp4T3rbCJ6ZQkE8TkZQQvwaD8PUd-wgIHezTWy7cRvG4kyT4-w7dO2T2-ezAlwNBor4T8zs1J6aURPUUlFS4JczN_aBY9tJc8EyEOHxK5eL6QF7fYKAsJG2g7wt1YlT0grzNAHxN-FwIAyvg5XcwC3rVTdiG8wtTz4LF5VjsKh--0pF9_kVNKEaSt1_Qx2zhcrz1Y9Ih7eurkM1G5d4w6nZ4RCJZKkhhVrP3LNknSVKbNf_eRPFn_8h29Fas_fDE4o7exP45nZ0rgIPCPR70iWYCOMHFPOvN0gSVo7s5mbAHR69-Fgs0iSh4u0H5Vicvte1BeA.&spm=tbpc.boughtlist.suborder_itempic.d41310780866.12852e8dOh0AD5)

电机驱动板型号为**TB6612FNG**

[TB6612FNG电机驱动板资料](https://telesky.yuque.com/bdys8w/01/tuogxw59rm1v97bc)

[TB6612FNG电机驱动板购买链接](https://detail.tmall.com/item.htm?id=41248562401&pisk=grBa6zvI5ReN1bL9s99q8012ZCv9yKz7otTXmijDfFYgMhwm3iSkclUYX6-clwEYXdsDuZ7BJxtb6rHV8Nsl1C_f1ExD-wjsCEw9gZS5yZMbB1tm3wsviZ6VDx-cmZE9llFCXGpvnza5u8sOXl8HDdWwoB2hv3-MoWOi_JS_Aza7F-PixKzaPZO1mrmH43vDjIDi-k-k2nvDihj3YnxjSjDGokrec3GiSEDmKDx20KDDsIxnx3tWmxvDskSHJhvDoZA0YMcF0GjmL3OgD-TRc-3aUsTljekmU0p9scQEGvkENhdMrBvmnH6ebC8ljTzmvlxhd9RWOmaWKgCCSHJz3vvwa1bwxMUqUFf52gTD10MAYM6OusAUjRjeQFRAKQom_d5cWtRO_mERxdbdwUdg9yKFCTO2yBu3SMseSQ5V5yHD5_Sh8Q6IRY9hq6JF4DgvxrGfH1umgCxpYUZUYK9Pu_ugZ9qZMjdHeH87fhGxMCxKhfWrUjhvt3KePlt1.&spm=tbpc.boughtlist.suborder_itempic.d41248562401.12852e8dOh0AD5)

130电机和TB6612FNG电机驱动板连线如下:

- VM -> 5V
- VCC -> 3.3V
- GND -> GND(需要与STM32的GND共地)
- AO1 -> 电机端口1
- AO2 -> 电机端口2(不分正反)
- PWMA -> TIM2_CH3引脚(PA2)
- AIN2 -> PA5
- AIN1 -> PA4
- STBY -> 3.3V

[0.96寸7针OLED显示屏SPI接口资料](https://telesky.yuque.com/bdys8w/01/lw9nqcxkk0hffiuz)

[0.96寸7针OLED显示屏SPI接口购买链接](https://detail.tmall.com/item.htm?ali_trackid=2%3Amm_26632943_457000242_108858100157%2C123%3A1752247506496_557215226_0&bxsign=tbktZejeaT6MloquHvxd6qDPk26u0_fjPoLk-l99lLIojBUzU6GXGDVZfwsz_JGquPlwm-NjAy1u24jEoHABX5P66pHeEPjvDCsKELgAQ7fYNjx7p00OOtOzTUosTBnTp6lWxpb8rQk1N_KNQ2EoAbk98Hib81l4Fw96H8mWDcuaI8MWbHM7Mx2K2vk5h4jE_5O&id=42044259331&pvid=null&relationId=3131571508&rid=3131571508&scm=null&spm=a21wu.21508199.product.mm_26632943_457000242_108858100157&unid=123&union_lens=lensId%3AOPT%401752247504%4021508606_0db1_197fa170dde_b4fe%4001%40eyJmbG9vcklkIjoxMDYzOTN9%3Brecoveryid%3A557215226_0%401752247506499)

OLED连接:

- GND -> GND(需要与STM32的GND共地)
- VDD -> 3.3V
- SCK -> PG12
- SDA -> PD5
- RES -> PD4
- DC -> PD15
- CS -> PD1

按键使用霸道开发板按键(PA0),如下图所示

![按键](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250618202248.png)

### PWM初始化函数(PWM驱动直流电机)

```c
// 初始化 TIM2_CH3(PA2)为PWM输出
void PWM_Init(void)
{
    // 1. 开启时钟, 打开TIM2外设, GPIO外设和AFIO的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // 开启TIM2时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // 开启AFIO时钟, 复用功能需要

    // 2. 选择时基单元时钟源(默认使用内部时钟)
    TIM_InternalClockConfig(TIM2); // 选择内部时钟

    // 3. 配置时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 滤波采样频率一分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1; // ARR + 1 = 100  
	TIM_TimeBaseInitStructure.TIM_Prescaler = 36 - 1; // PSC + 1 = 36
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 时钟分频系数, 不分频
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // 重复计数器(高级定时器才有的),这里不用,赋值0
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // 重复计数器(高级定时器才有的),这里不用,赋值0
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure); // 初始化TIM2时基单元

    // 4. 配置输出比较通道
    TIM_OCInitTypeDef TIM_OCInitStructure; // 定义输出比较结构体
    TIM_OCStructInit(&TIM_OCInitStructure); // 初始化输出比较结构体
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; // 设置输出比较模式为PWM1
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // 设置输出极性为高
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 使能输出比较
    TIM_OCInitStructure.TIM_Pulse = 50; // CCR = 50
    TIM_OC3Init(TIM2, &TIM_OCInitStructure); // 初始化TIM2通道3的输出比较

    // 5. 配置PWM对应的GPIO
    GPIO_InitTypeDef GPIO_InitStructure; // 定义GPIO初始化结构体
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; // 选择PA2引脚, TIM2_CH3默认映射到PA2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 设置为复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 设置GPIO速度为50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure); // 初始化GPIOA

    // 6. 运行控制,启动计数器
    TIM_Cmd(TIM2, ENABLE); // 使能TIM2计数器

}
```

### 直流电机初始化函数

```c
// 初始化PWM(TIM2_CH3)为PA4, PA5控制电机方向和速度
void Motor_Init(void) 
{
    // 初始化PA4, PA5进行电机方向控制
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启GPIOA时钟
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 设置为推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5; // 选择PA4,PA5引脚
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 设置GPIO速度为50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure); // 初始化GPIOA

    // 初始化PWM(TIM2_CH3)
    PWM_Init(); 

}
```

### 直流电机控制函数

```c
// 设置电机速度和方向
void Motor_SetSpeed(int8_t Speed)
{
    if (Speed >= 0)
    {
        // 正转
        GPIO_SetBits(GPIOA, GPIO_Pin_4); // 设置PA4为高电平
        GPIO_ResetBits(GPIOA, GPIO_Pin_5); // 设置PA5为低电平
        PWM_SetCompare3(Speed); // 设置PWM占空比
    } 
    else 
    {
        // 反转
        GPIO_ResetBits(GPIOA, GPIO_Pin_4); // 设置PA4为低电平
        GPIO_SetBits(GPIOA, GPIO_Pin_5); // 设置PA5为高电平
        PWM_SetCompare3(-Speed); // 设置PWM占空比(取绝对值)
    }
}
```

### 实现按键控制直流电机速度并在OLED显示当前速度

```c
uint8_t KeyNum; // 按键编号变量
int8_t Speed; // 电机速度变量

int main(void)
{
	delay_init(); // 延时初始化
    OLED_Init(); // OLED初始化
    Motor_Init(); // 电机初始化

	OLED_Clear(); // OLED清屏
	OLED_ShowString(1, 11, "Speed:", 12); // 在(1, 11)位置显示"Speed:"字体大小12
	OLED_Refresh(); // 更新显存到OLED

    Motor_SetSpeed(0); // 设置电机初始速度为0

	while(1)
	{
        KeyNum = KEY_GetNum(); // 获取按键编号
        if (KeyNum == 1) // 按键按下
        {
            Speed += 20; // 增加速度
            if (Speed > 100) // 限制最大速度为100
            {
                Speed = -100;
            }
            Motor_SetSpeed(Speed); // 设置电机速度
            OLED_ShowNum(41, 11, Speed, 3, 12); // 显示负数时有BUG
            OLED_Refresh(); // 更新显存到OLED
        }
	}
}

```

## TIM输入捕获

### 输入捕获简介

IC(Input Capture)输入捕获

输入捕获就是下图中对应的部分,4个输入捕获和输出比较通道共用4个CCR寄存器,引脚CH1到CH4也是共用的

对于同一个定时器,输入捕获和输出比较只能使用其中一个功能,不能同时使用

![输入捕获部分](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250629092943.png)

输入捕获模式下,**当通道输入引脚出现指定电平跳变时,当前CNT的值将被锁存到CCR中**,可用于测量PWM波
形的频率,占空比,脉冲间隔,电平持续时间等参数

每个高级定时器和通用定时器都有4个输入捕获通道

可配置为PWMI模式,同时测量频率和占空比

可配合主从触发模式,实现硬件全自动测量

### 频率测量

![频率测量](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250629102455.png)

- 测频法是在闸门时间T内计次,从而得到频率 $f_x = N/T$
- 测频法测量的是在闸门时间的多个周期,所以自带均值滤波,如果在闸门时间内波形频率有变化,得到的是这段时间的平均频率,所以结果更新慢,由于测量的是平均值所以数值平滑
- 测周法是在一个周期内计时,计时是通过以标准频率计次来实现的,计时时间为 $(1/f_c) * N$ ,从而得到频率 $fx = 1/ ((1/f_c)*N) = f_c/N$
- 测周法只测一个周期,所以出结果的速度取决于待测信号的频率,结构更新快,由于只测一个周期,如果待测信号频率有变化,会导致数据跳变,所以结果不稳定

| 特征 | 测频法 | 测周法 |
|-|-|-|
| 频率 $f_x$ 公式 | $f_x = N/T$ | $f_x = f_c/N$ |
| 测量结果更新时间 | 更新慢,数值稳定 | 更新快,数据跳变快 |

中界频率是测频法和测周法的分界点,中界频率是指在测量时二者误差相等的点,表现为N相等

$$
\begin{align}
    N = f_x * T \tag{1} \\
    N = f_c / f_x \tag{2} \\
    f_x = \sqrt{(fc / T)}\tag{3} \\
\end{align}
$$

1式为测频法频率公式,2式为测周法频率公式,3式中的 $f_x$ 定义为中界频率 $f_m$

### 输入捕获电路执行流程

![输入捕获电路流程图](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250629120008.png)

1. TIMx_CH1,2,3,4为输入捕获的4个输入引脚,参考引脚定义表可知其复用端口位置
2. 三输入的异或门接在通道1,2,3端口为三相无刷电机服务
3. 输入滤波器可避免一些高频的毛刺信号误触发;边沿检测器可选高/低电平触发
4. 输入滤波和边沿学则电路有两套,TI1经过滤波和极性选择后得到TI1FP1到IC1,得到TI1FP2到IC2,同理TI2经过滤波和极性选择后得到TI2FP1到IC1,得到TI2FP2到IC2,可以进行交叉选择
    1. 交叉选择可以灵活切换后续捕获电路的输入
    2. 交叉选择可以把一个引脚的输入同时映射到两个捕获单元,也是PWMI模式的经典结构
5. TRC信号也可作为捕获部分的输入,服务于无刷电机的驱动
6. 预分频器可以对输入信号IC1-4进行分频得到ICxPS(x:1-4)可触发捕获电路进行工作,每来一个触发信号CNT的值就会向CCR转运一次,同时发生捕获事件,也可产生中断

![输入捕获通道框图](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250629145629.png)

1. TI1进入滤波器输出TI1F, $ f_{DTS} $ 是滤波器的采样时钟来源, TIMx_CCMR1寄存器里的ICF位可以控制滤波器的参数,
2. 滤波之后的信号TI1F通过边沿检测器捕获上升沿TI1F_Rising或下降沿TI1F_Falling;通过TIMx_CCER寄存器的CC1P位选择极性最终得到TI1FP1触发信号
3. TI1FP1经过数据选择器到IC1,TIMx_CCMR1寄存器的CC1S位可以选择TI1FP1/TI2FP1/TRC
4. IC1经过分频器到IC1PS,TIMx_CCMR1寄存器的CPS位可选择分频系数/1,/2,/4,/8
5. TIMx_CCER的CC1E位控制输出使能/失能

其中TC1F位的工作原理如下所示

以采样频率对输入信号进行采样,当连续N个值为高电平,输出才为高电平;如果信号出现高频抖动,导致连续采样N个值不全一样,输出就不会变化,达到滤波效果

如下图所示,采样频率 $f_{SAMPLING}$ 越低,采样个数N越大,滤波效果越好;实际应用中,如果波形噪声比较大,就可以把IC1F位的参数设置大些

![IC1F位](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250629144319.png)

### 测周法的执行流程

1. 使用上升沿用于触发输入捕获,CNT用于计数计时
2. 上升沿触发捕获事件,CNT的值自动转运到CCR中
3. 由于CNT计数器是由内部的标准时钟驱动,频率为 $f_c$ ,捕获到的值就是测周法中的N, 也就是计时的次数
    - 每次捕获后需要将CNT清零, 以便下次捕获时从0开始计时
    - 该清零操作可用主从触发模式实现
4. 由此可得频率 $f_x = f_c / N$ , 其中N为捕获到的值

### 主从触发模式

主从触发模式不是专业术语,是主模式,从模式和触发源选择三个功能的简称

![主从触发模式框图](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250629164820.png)

- 主模式可以将定时器内部的信号映射到TRGO引脚用于触发别的外设
- 从模式是接收其他外设或者自身外设的信号用于控制自身定时器的运行,也就是被别的信号控制
- 触发源选择是选择从模式的触发信号源,也可以认为是从模式的一部分,选择指定的信号触发TRGI,TRGI去触发从模式,从模式可选列表其中一个信号自动执行

若想要实现测周法,即让TI1FP1信号自动触发CNT清零,则触发源选择为TI1FP1,从模式执行的操作为Reset

### 输入捕获基本结构

![输入捕获基本结构](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250630225759.png)

只使用一个通道,只能测量频率

1. 配置时基单元启动定时器,CNT自增,此时的标准频率 $f_c = 72MHz/PSC预分频系数$
    - ARR一般设置为65535,故CNT的计数范围为0~65535
    - 如果信号频率太低,CNT计数值可会溢出
2. 输入捕获的GPIO口输入一个方波信号,经过滤波器和边沿检测,选择TI1FP1为上升沿触发,分频器选择不分频
3. 当TI1FP1出现上升沿后,CNT当前的计数值转运到CCR1里
    - CCR1的计数值即为测周法的N, $f_x = f_c / N$
4. 触发源选择TI1FP1为触发信号,从模式选择Rest操作
    - 触发源选择只有TI1FP1和TI2FP2
    - 使用从模式自动清零CNT只能用通道1和通道2

![PWMI基本结构](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250630230857.png)

TI1FP1配置上升沿触发,触发捕获和清零CNT,TI1FP2配置下降沿触发通过交叉通道触发通道2的捕获单元

1. 上升沿CCR1捕获同时清零CNT,CNT++
2. 下降沿CCR2捕获,CCR2的值为从上升沿到下降沿的计数值,即高电平的计数值,不触发CNT清零
3. 下一次上升沿CCR1捕获,CNT清零

此时,CCR2的值为高电平时的计数值,CCR1的值为一整个周期的计数值,CCR2/CCR1即为占空比

## 输入捕获模式测频率实验

工程文件目录: `6-6 输入捕获模式测频率`

实验目标: **实现显示测周法频率**

### 硬件连接(输入捕获模式测频率实验)

PWM接线:

PWM信号由PA0产生,PA6进行输入捕获的接收,接线如下所示

- PA6 -> PA0
- PA0 -> PA6

[0.96寸7针OLED显示屏SPI接口资料](https://telesky.yuque.com/bdys8w/01/lw9nqcxkk0hffiuz)

[0.96寸7针OLED显示屏SPI接口购买链接](https://detail.tmall.com/item.htm?ali_trackid=2%3Amm_26632943_457000242_108858100157%2C123%3A1752247506496_557215226_0&bxsign=tbktZejeaT6MloquHvxd6qDPk26u0_fjPoLk-l99lLIojBUzU6GXGDVZfwsz_JGquPlwm-NjAy1u24jEoHABX5P66pHeEPjvDCsKELgAQ7fYNjx7p00OOtOzTUosTBnTp6lWxpb8rQk1N_KNQ2EoAbk98Hib81l4Fw96H8mWDcuaI8MWbHM7Mx2K2vk5h4jE_5O&id=42044259331&pvid=null&relationId=3131571508&rid=3131571508&scm=null&spm=a21wu.21508199.product.mm_26632943_457000242_108858100157&unid=123&union_lens=lensId%3AOPT%401752247504%4021508606_0db1_197fa170dde_b4fe%4001%40eyJmbG9vcklkIjoxMDYzOTN9%3Brecoveryid%3A557215226_0%401752247506499)

OLED接线:

- GND -> GND(需要与STM32的GND共地)
- VDD -> 3.3V
- SCK -> PG12
- SDA -> PD5
- RES -> PD4
- DC -> PD15
- CS -> PD1

### 设置PWM波形的频率

频率计算公式为 Freq = 72MHz / (PSC + 1) / (ARR + 1)

通过固定(ARR+1)的值为100,则只需要修改PSC的值即可更改频率

在`PWM.c`中增加`PWM_SetPrescaler`函数用来单独修改PSC的值

```c
// 单独写入PSC
void PWM_SetPrescaler(uint16_t Prescaler)
{
    TIM_PrescalerConfig(TIM2, Prescaler, TIM_PSCReloadMode_Immediate); // 设置TIM2的PSC的值
}
```

### 输入捕获初始化函数

在`stm32f10x_tim.h`中找到关于输入捕获的函数,如下所示

```c
// 用结构体配置输入捕获单元的函数
void TIM_ICInit(TIM_TypeDef* TIMx, TIM_ICInitTypeDef* TIM_ICInitStruct);

// 用结构体配置时基单元的函数,可快速配置两个通道,配置为PWMI模式
void TIM_PWMIConfig(TIM_TypeDef* TIMx, TIM_ICInitTypeDef* TIM_ICInitStruct);

// 给输入捕获结构体赋初值的函数
void TIM_ICStyyuctInit(TIM_ICInitTypeDef* TIM_ICInitStruct);

// 选择从模式触发源的函数
void TIM_SelectInputTrigger(TIM_TypeDef* TIMx, uint16_t TIM_InputTriggerSource);

// 选择主模式输出触发源的函数
void TIM_SelectOutputTrigger(TIM_TypeDef* TIMx, uint16_t TIM_TRGOSource);

// 选择从模式
void TIM_SelectSlaveMode(TIM_TypeDef* TIMx, uint16_t TIM_SlaveMode);

// 分别单独配置通道1234的分频器 
void TIM_SetIC1Prescaler(TIM_TypeDef* TIMx, uint16_t TIM_ICPSC);
void TIM_SetIC2Prescaler(TIM_TypeDef* TIMx, uint16_t TIM_ICPSC);
void TIM_SetIC3Prescaler(TIM_TypeDef* TIMx, uint16_t TIM_ICPSC);
void TIM_SetIC4Prescaler(TIM_TypeDef* TIMx, uint16_t TIM_ICPSC);

// 分别读取四个通道的CCR,输入捕获模式下CCR只读
uint16_t TIM_GetCapture1(TIM_TypeDef* TIMx);
uint16_t TIM_GetCapture2(TIM_TypeDef* TIMx);
uint16_t TIM_GetCapture3(TIM_TypeDef* TIMx);
uint16_t TIM_GetCapture4(TIM_TypeDef* TIMx);

// 分别写入四个通道的CCR,输出比较模式下CCR只写
void TIM_SetCompare1(TIM_TypeDef* TIMx, uint16_t Compare1);
void TIM_SetCompare2(TIM_TypeDef* TIMx, uint16_t Compare2);
void TIM_SetCompare3(TIM_TypeDef* TIMx, uint16_t Compare3);
void TIM_SetCompare4(TIM_TypeDef* TIMx, uint16_t Compare4);
```

![输入捕获流程图](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250711214900.png)

如上图输入捕获结构图所示,流程如下:

1. 开启时钟,打开GPIO和TIM的时钟
2. GPIO初始化,把GPIO配置成输入模式,一般选择上拉输入/浮空输入
3. 配置时基单元,让CNT计数器在内部时钟的驱动下自增运行
4. 配置输入捕获单元,包括滤波器,极性,直连/交叉通道,分频器
5. 选择从模式的触发源,选择为TI1FP1
6. 选择触发后执行的操作,从模式选择Reset操作
7. 调用TIM_Cmd函数,开启定时器

在`IC.c`中增加`IC_Init`函数, 用于初始化输入捕获

```c
void IC_Init(void)
{
    // 1. 开启时钟, 打开TIM3外设, GPIO外设的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // 开启TIM3时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启GPIOA时钟


    // 2. 配置PWM对应的GPIO
    GPIO_InitTypeDef GPIO_InitStructure; // 定义GPIO初始化结构体
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; // 选择PA6引脚, TIM3_CH1默认映射到PA6
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 设置为上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 设置GPIO速度为50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure); // 初始化GPIOA

    // 3. 选择时基单元时钟源(默认使用内部时钟)
    TIM_InternalClockConfig(TIM3); // 选择内部时钟

    // 4. 配置时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 滤波采样频率一分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1; // 为了防止溢出,ARR的值设置最大值 
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1; // 标准频率为72M/72=1MHz
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // 重复计数器(高级定时器才有的),这里不用,赋值0
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure); // 初始化TIM3时基单元

    // 5. 初始化输入捕获单元
    TIM_ICInitTypeDef TIM_ICInitStructure; // 定义输入捕获初始化结构体
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; // 选择通道1
    TIM_ICInitSturcture.TIM_ICFilter = 0xF; // 设置滤波器参数, 0xF为最大值
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; // 设置极性为上升沿触发
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; // 设置分频器为不分频
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // 选择直连通道
    TIM_ICInit(TIM3, &TIM_ICInitStructure); // 配置TIM3的输入捕获

    // 6. 配置TRGI的触发源为TI1FP1
    TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1); // 选择触发源为TI1FP1

    // 7. 配置从模式为Reset
    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset); // 设置从模式为复位模式

    // 8. 启动定时器
    TIM_Cmd(TIM3, ENABLE); // 使能TIM3定时器
}
```

### 测周法频率读取函数

测周法的标准频率 $f_c = 72MHz / (PSC+1) = 72MHz/(72-1+1) = 1MHz$

测周法测得的频率为 $f_x = f_c / N = 1MHz / N$ ,其中N为CRR的计数值

在`IC.c`中增加`IC_GetFreq`函数, 用于读取测周法的频率

```c
uint32_t IC_GetFreq(void)
{
    /*
     * Fc = 72MHz/(PSC+1) = 72MHz/(72-1+1) = 1MHz
     * Fx = Fc/N = 1MHz / N
     */
    return 1000000 / (TIM_GetCapture1(TIM3)); // 返回频率Fx
}
```

### 实现显示测周法频率

```c
int main(void)
{
	delay_init(); // 延时函数初始化
    PWM_Init(); // TIM2的PWM初始化
    IC_Init(); // 输入捕获初始化
    OLED_Init(); // OLED初始化
    
	OLED_Clear(); // OLED清屏
	OLED_ShowString(1, 11, "Freq::", 12); // 在(1, 11)位置显示"Freq:"字体大小12
	OLED_Refresh(); // 更新显存到OLED

    PWM_SetPrescaler(720-1); // 设置PSC, PWM频率为72M/720/100 = 1KHz
    PWM_SetCompare1(50); // 设置CCR, PWM占空比为50%

	while(1)
	{
        OLED_ShowNum(1, 31, IC_GetFreq(), 5, 12); // 显示频率
        OLED_Refresh(); // 更新显存到OLED
	}
}
```

### BUG解决(输入捕获实验)

实际测试中,测出的频率会比实际频率+1,在`IC_GetFreq`函数中修改

```c
// 返回测周法测出的频率
uint32_t IC_GetFreq(void)
{
    /*
     * Fc = 72MHz/(PSC+1) = 72MHz/(72-1+1) = 1MHz
     * Fx = Fc/N = 1MHz / N
     */
    return 1000000 / (TIM_GetCapture1(TIM3) + 1); // 返回频率Fx
}
```

仅为美化数据

## PWMI模式测频率占空比

工程文件目录: `6-7 PWMI模式测频率占空比`

实验目标: **实现PWMI模式测量频率和占空比并在OLED显示**

### 硬件连接(PWMI模式测频率和占空比)

PWM接线:

PWM信号由PA0产生,PA6进行输入捕获的接收,接线如下所示

- PA6 -> PA0
- PA0 -> PA6

[0.96寸7针OLED显示屏SPI接口资料](https://telesky.yuque.com/bdys8w/01/lw9nqcxkk0hffiuz)

[0.96寸7针OLED显示屏SPI接口购买链接](https://detail.tmall.com/item.htm?ali_trackid=2%3Amm_26632943_457000242_108858100157%2C123%3A1752247506496_557215226_0&bxsign=tbktZejeaT6MloquHvxd6qDPk26u0_fjPoLk-l99lLIojBUzU6GXGDVZfwsz_JGquPlwm-NjAy1u24jEoHABX5P66pHeEPjvDCsKELgAQ7fYNjx7p00OOtOzTUosTBnTp6lWxpb8rQk1N_KNQ2EoAbk98Hib81l4Fw96H8mWDcuaI8MWbHM7Mx2K2vk5h4jE_5O&id=42044259331&pvid=null&relationId=3131571508&rid=3131571508&scm=null&spm=a21wu.21508199.product.mm_26632943_457000242_108858100157&unid=123&union_lens=lensId%3AOPT%401752247504%4021508606_0db1_197fa170dde_b4fe%4001%40eyJmbG9vcklkIjoxMDYzOTN9%3Brecoveryid%3A557215226_0%401752247506499)

OLED接线:

- GND -> GND(需要与STM32的GND共地)
- VDD -> 3.3V
- SCK -> PG12
- SDA -> PD5
- RES -> PD4
- DC -> PD15
- CS -> PD1

### PWMI初始化函数

![PWMI基本结构](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250711235449.png)

PWMI流程图如上所示

在`IC.c`中`IC_Init`函数里使用`TIM_PWMIConfig`函数进行PWMI配置

```c
// 5. 初始化输入捕获单元
TIM_ICInitTypeDef TIM_ICInitStructure; // 定义输入捕获初始化结构体
TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; // 选择通道1
TIM_ICInitStructure.TIM_ICFilter = 0xF; // 设置滤波器参数, 0xF为最大值
TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; // 设置极性为上升沿触发
TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; // 设置分频器为不分频
TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // 选择直连通道
TIM_PWMIConfig(TIM3, &TIM_ICInitStructure); // 配置TIM3的输入捕获
```

上述代码等效于下面的代码,下面的代码为不使用`TIM_PWMIConfig`函数,手动配置通道2实现PWMI模式

```c
// 5. 初始化输入捕获单元
TIM_ICInitTypeDef TIM_ICInitStructure; // 定义输入捕获初始化结构体
TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; // 选择通道1
TIM_ICInitStructure.TIM_ICFilter = 0xF; // 设置滤波器参数, 0xF为最大值
TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; // 设置极性为上升沿触发
TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; // 设置分频器为不分频
TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // 选择直连通道
TIM_ICInit(TIM3, &TIM_ICInitStructure); // 配置TIM3的输入捕获
TIM_ICInitStructure.TIM_Channel = TIM_Channel_2; // 选择通道2
TIM_ICInitStructure.TIM_ICFilter = 0xF; // 设置滤波器参数, 0xF为最大值
TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling; // 设置极性为下降沿触发
TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; // 设置分频器为不分频
TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI; // 选择交叉通道
TIM_ICInit(TIM3, &TIM_ICInitStructure); // 配置TIM3的输入捕获
```

可见,使用`TIM_PWMIConfig`函数可以简化PWMI模式的配置,只需要配置一次输入捕获结构体即可

`TIM_PWMIConfig`函数仅支持通道1和通道2的配置

PWMI模式的初始化函数与输入捕获模式的初始化函数类似,只需要在`IC_Init`函数中修改PWMI配置即可

```c
void IC_Init(void)
{
    // 1. 开启时钟, 打开TIM3外设, GPIO外设的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // 开启TIM3时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启GPIOA时钟


    // 2. 配置PWM对应的GPIO
    GPIO_InitTypeDef GPIO_InitStructure; // 定义GPIO初始化结构体
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; // 选择PA6引脚, TIM3_CH1默认映射到PA6
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 设置为上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 设置GPIO速度为50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure); // 初始化GPIOA

    // 3. 选择时基单元时钟源(默认使用内部时钟)
    TIM_InternalClockConfig(TIM3); // 选择内部时钟

    // 4. 配置时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 滤波采样频率一分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1; // 为了防止溢出,ARR的值设置最大值 
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1; // 标准频率为72M/72=1MHz
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // 重复计数器(高级定时器才有的),这里不用,赋值0
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure); // 初始化TIM3时基单元

    // 5. 初始化输入捕获单元
    TIM_ICInitTypeDef TIM_ICInitStructure; // 定义输入捕获初始化结构体
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; // 选择通道1
    TIM_ICInitStructure.TIM_ICFilter = 0xF; // 设置滤波器参数, 0xF为最大值
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; // 设置极性为上升沿触发
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; // 设置分频器为不分频
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // 选择直连通道
    TIM_PWMIConfig(TIM3, &TIM_ICInitStructure); // 配置TIM3的PWMI模式输入捕获

    // 6. 配置TRGI的触发源为TI1FP1
    TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1); // 选择触发源为TI1FP1

    // 7. 配置从模式为Reset
    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset); // 设置从模式为复位模式

    // 8. 启动定时器
    TIM_Cmd(TIM3, ENABLE); // 使能TIM3定时器
}
```

### PWMI模式测频率占空比函数

```c
// 返回测周法测出的占空比
uint32_t IC_GetDuty(void)
{
    /*
     * 占空比 = CCR2的计数值/CCR1的计数值
     * CCR2的值*100则返回0~100的数值,否则返回0~1
     * CCR2和CCR1 + 1是为了美化数据,否则测量频率和实际频率相差1(BUG)
     */
    return (TIM_GetCapture2(TIM3) + 1) * 100 / (TIM_GetCapture1(TIM3) + 1);
}
```

### 实现PWMI测量频率和占空比

```c
int main(void)
{
	delay_init(); // 延时函数初始化
    PWM_Init(); // TIM2的PWM初始化
    IC_Init(); // 输入捕获初始化
    OLED_Init(); // OLED初始化
    
	OLED_Clear(); // OLED清屏
	OLED_ShowString(1, 11, "Freq:", 12); // 在(1, 11)位置显示"Freq:"字体大小12
    OLED_ShowString(61, 11, "Hz", 12); // 在(61, 11)位置显示"Hz"字体大小12
	OLED_ShowString(1, 31, "Duty:", 12); // 在(1, 31)位置显示"Duty:"字体大小12
    OLED_ShowString(45, 31, "%", 12); // 在(45, 31)位置显示"%"字体大小12
	OLED_Refresh(); // 更新显存到OLED

    PWM_SetPrescaler(720-1); // 设置PSC, PWM频率为72M/720/100 = 1KHz
    PWM_SetCompare1(50); // 设置CCR, PWM占空比为50%

	while(1)
	{
        OLED_ShowNum(31, 11, IC_GetFreq(), 5, 12); // 在(31, 11)位置显示频率,宽度5,字体大小12
        OLED_ShowNum(31, 31, IC_GetDuty(), 2, 12); // 在(31, 31)位置显示占空比,宽度2,字体大小12
	    OLED_Refresh(); // 更新显存到OLED
	}
}
```
