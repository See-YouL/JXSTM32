# GPIO输入笔记

## 实验测试

### LED驱动

新建Hardware文件夹来存放驱动文件

其中`LED.h`和`LED.c`为LED的驱动文件

LED硬件电路如下图所示

![LED硬件电路](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250417165941162.png)

- 红色LED在PB5
- 绿色LED在PB0
- 蓝色LED在PB1
- 红绿蓝LED均为低电平点亮，共阳极接法

在`LED.c`中定义红色LED的驱动函数

- LED_R_Init()：初始化红色LED
- LED_R_ON()：点亮红色LED
- LED_R_OFF()：熄灭红色LED
- LED_R_Turn()：翻转红色LED状态

```c
#include "stm32f10x.h"                  // Device header

// 初始化红色LED（PB5）
void LED_R_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 打开GPIOB时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 配置为推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; // 配置端口为5
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 配置端口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure); // 完成PB5初始化
}

// 点亮红色LED(低电平点亮）
void LED_R_ON(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
}

// 熄灭红色LED(高电平熄灭）
void LED_R_OFF(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_5);
}

// 翻转LED状态
void LED_R_Turn(void)
{
	if (0 == GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_5))
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_5); // PB5低电平时设置为高电平
	}
	else
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_5); // PB5高电平时设置为低电平
	}
}

````

在`LED.h`中提供函数接口

```c
#ifndef __LED_H
#define __LED_H

void LED_R_Init(void); // LED_R初始化
void LED_R_ON(void); // LED_R点亮函数
void LED_R_OFF(void); // LED_R熄灭函数
void LED_R_Turn(void); // LED_R翻转状态函数

#endif

```

### 按键驱动

按键接在PA0引脚, 按键按下PA0为高电平

按键的硬件连接图如下图所示

![按键连接图](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250417170945925.png)

配置PA0引脚为下拉输入模式

在`KEY.c`定义按键的驱动函数

```c
#include "stm32f10x.h"                  // Device header
#include "Delay.h" // 延时模块

// 初始化按键PA0
void KEY_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 打开GPIOA时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; // 配置下拉输入模式,默认低电平
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; // 配置端口0
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 配置输出速度50MHz不影响输入模式
	GPIO_Init(GPIOA, &GPIO_InitStructure);  // 完成初始化
}

// 按键按下返回1
uint8_t KEY_GetNum(void)
{
	uint8_t KEYNum = 0;
	if (1 == GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)) // 按键按下PA0为高电平
	{
		Delay_ms(20); // 按下按键延时消抖
		while (1 == GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
		{
			;
		}
		Delay_ms(20); // 松开按键延时消抖
		KEYNum = 1; // 按键按下KEYNum为1
	}
	
	return KEYNum;
}

```

在`KEY.h`中提供函数接口

```c
#ifndef __KEY_H
#define __KEY_H

void KEY_Init(void); // 按键初始化函数
uint8_t KEY_GetNum(void); // 读取按键函数

#endif

```

### 软件设计

执行流程

1. 在`main.c`中定义全局变量KEYNum用来存储按键状态
2. 进行LED和按键的初始化
3. 在while循环中不断读取按键的状态并存储在KEYNum中
4. 判断按键是否按下,按下则翻转LED状态,未按下则不执行翻转操作

在`main.c`中实现主控制流程

```c
/*
 * 实验目标：实现按键控制LED点亮
 * 硬件连接: 红色LED接在PB5, 按键接在PA0
 */
 
#include "stm32f10x.h"
#include "Delay.h"
#include "LED.h"
#include "KEY.h"

uint8_t KEYNum = 0; // 全局变量KEYNum

int main(void)
{
	// 初始化红色LED
	LED_R_Init();
	// 初始化按键
	KEY_Init();

	while (1)
	{
		KEYNum = KEY_GetNum(); // 读取按键值
		if (1 == KEYNum) // 按键按下
		{
			LED_R_Turn();
		}
	
	}
}

```

