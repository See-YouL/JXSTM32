# EXTI中断

## 理论知识

### 中断系统

中断: 在主程序运行过程中,出现了特定的中断触发条件(中断源), 使得CPU暂停当前正在运行的程序,转而处
理中断程序,处理完成后又返回原来被暂停的位置继续运行.

中断优先级: 当有多个中断源同时申请中断时,CPU会根据中断源的轻重缓急进行裁决,优先相应更加紧急的中断源.

中断嵌套: 当一个中断程序正在运行时,又有新的更高优先级的中断源申请中断,CPU再次暂停当前中断程序,转而处理新的中断程序,处理完成后依次进行返回.

### 中断执行流程

中断执行流程图如下图所示：

![中断执行流程图](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250418003747046.png)

### NVIC的基本结构

![NVIC基本结构图](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250418005846195.png)

### NVIC优先级分组

![NVIC优先级分组](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250418010043897.png) 

### EXTI简介

外部中断（External Interrupt）是指来自外部设备或信号的中断请求。

EXTI可以监测GPIO口的电平信号,当其指定的GPIO口电平发生变化时,EXTI将立即向NVIC发出中断申请,
经过NVIC裁决后即可中断CPU主程序,使CPU执行EXTI对应的中断程序.

支持的触发方式有：上升沿/下降沿/双边沿/软件触发。

支持的GPIO口: 所有GPIO口均可触发外部中断但是相同的Pin不能同时触发中断(如PA0和PB0不能同时触发中断).

通道数: 16个GPIO_Pin,外加PVD输出,RTC闹钟,USB唤醒,以太网唤醒.

触发响应方式: 中断响应/事件响应(用来触发其他外设的事件).

### EXTI的基本结构

![EXTI的基本结构图](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250418010738918.png)

- 经过AFIO选择后仅有16路GPIO口可以触发EXTI中断,所以PA0和PB0不可以同时触发中断.
- EXTI9_5和EXTI15_10需要通过标志位来区分是哪个GPIO口触发的中断.
- EXTI给其他外设的中断为事件响应

AFIO结构图如下:

![AFIO结构图](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250418011608366.png)

通过数据选择器来将PA0~PG0选择其中一条线到EXTI0.

EXTI框图如下:

![EXTI框图](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250418011805075.png)

1. 输入线进入边沿监测电路,选择上升沿/下降沿/双边沿触发.
2. 边沿监测电路信号和软件中断事件寄存器一起进入逻辑或门
3. 触发信号通过逻辑或门后分两路,上面一路触发中断,下面一路触发事件.
4. 触发中断会置请求挂起寄存器, 请求挂起寄存器和中断屏蔽器一起进入逻辑与门,进入NVIC中断控制器.
5. 触发事件和事件屏蔽寄存器一起进入逻辑与门,再通过脉冲发生器用来触发其他外设.

## 实验测试

实验目标: 使用火焰传感器监测火焰并通过OLED屏显示火焰警报

### 硬件连接

OLED连接

- SCK 接 PG12
- SDA 接 PD5
- RES 接 PD4
- DC 接 PD15
- CS 接 PD1

火焰传感器连接

- DO 接 PB13

### 火焰传感器

实物如图所示

![火焰传感器3针蓝版](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250418044012051.png)

资料链接 : [火焰传感器链接](https://telesky.yuque.com/bdys8w/01/qyw1o22t6g5e278p)

传感器型号: 3针版

当火焰强度超过阈值时,输出低电平,否则输出高电平,所以需要配置为下降沿触发

### 程序设计

在`fire.c`文件中编写如下代码

 ```c
#include "stm32f10x.h"                  // Device header
#include "stm32f10x_exti.h"

uint16_t FireState = 0; // 火焰传感器状态变量 0:无火焰,1:有火焰  

/*
 * 火焰传感器初始化函数
 * 1. 打开GPIOB时钟和AFIO时钟
 * 2. 配置PB14为上拉输入模式,速度50MHz
 * 3. 配置中断线为PB14
 * 4. 配置中断线为EXTI_Line14,使能中断线,模式选为中断模式,触发模式选为下降沿触发
 * 4. NVIC分组设置为2组即2个抢占优先级,2个响应优先级
 * 5. NVIC通道设置为EXTI15_10,使能NVIC通道,抢占优先级设置为1,响应优先级设置为1
 */
void Fire_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 打开GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // 打开AFIO时钟
	
	// GPIO PB14初始化
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 配置上拉输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; // 配置端口为14
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 配置输出速度50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure); // 完成PB14初始化
	
	// AFIO初始化(中断线配置)
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14); // 配置中断线为PB14
	
	
	// EXTI初始化
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line14; // 中断线选为14
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; // 中断线使能
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; // 模式选为中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // 触发模式选为下降沿触发
	EXTI_Init(&EXTI_InitStructure);
	
	// NVIC初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // NVIC分组设置为2组即2个抢占优先级,2个响应优先级
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; // NVIC通道设置为EXTI15_10
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // 使能NVIC通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 抢占优先级设置为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; // 响应优先级设置为1
	NVIC_Init(&NVIC_InitStructure);
}

/*
 * 火焰传感器状态获取函数
 * 返回值: 0:无火焰,1:有火焰
 */
uint16_t FireState_Get(void)
{
	return FireState;	
}

/*
 * 火焰传感器状态清除函数
 * 清除火焰传感器状态,将状态变量清零
 */
void FireState_Clear(void)
{
	FireState = 0;	
}

/*
 * EXTI15_10中断处理函数
 * 1. 判断中断线是否为EXTI_Line14,如果是,则将火焰状态变量置为1,表示有火焰
 * 2. 清除中断标志位
 */
void EXTI15_10_IRQHandler(void)
{ 
	if (SET == EXTI_GetITStatus(EXTI_Line14))
	{
		FireState = 1;
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
	
}

```

在`fire.h`文件中编写如下代码

```c
#ifndef __FIRE_H
#define __FIRE_H

void Fire_Init(void); // 初始化火焰传感器
uint16_t FireState_Get(void); // 获取火焰传感器状态
void FireState_Clear(void); // 清除火焰传感器状态

#endif

```

在`main.c`文件中编写如下代码

```c
#include "stm32f10x.h"                  // Device header
#include "stm32f10x_exti.h"
#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
#include "fire.h"

int main(void)
{
    // 初始化延迟函数
	delay_init();
    // 初始化OLED屏幕
	OLED_Init();
    // 初始化火焰传感器
	Fire_Init();
	
	while(1)
	{
		if (1 == FireState_Get()) // 如果火焰传感器检测到火焰
		{
			OLED_Clear(); // 清屏
			OLED_ShowString(1, 1, "Fire Warning", 16); // 显示火焰警报
			OLED_Refresh(); // 刷新屏幕
			delay_ms(5000); // 延时5秒
			FireState_Clear(); // 清除火焰传感器状态
			OLED_Clear(); // 清屏
		}
		else if (0 == FireState_Get()) // 如果火焰传感器没有检测到火焰
		{
			OLED_Clear(); // 清屏
			OLED_ShowString(1, 1, "Stand By", 16); // 显示待机状态
			OLED_Refresh(); // 刷新屏幕
			delay_ms(5000); // 延时5秒
			OLED_Clear(); // 清屏
		}
        else // 如果火焰传感器状态异常
        {
            OLED_Clear(); // 清屏
            OLED_ShowString(1, 1, "Sensor Error", 16); // 显示传感器错误
            OLED_Refresh(); // 刷新屏幕
            delay_ms(5000); // 延时5秒
            OLED_Clear(); // 清屏
        }
	}
}

```
