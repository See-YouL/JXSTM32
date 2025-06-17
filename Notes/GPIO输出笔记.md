# GPIO输出笔记

## 理论知识

[GPIO输出理论知识视频链接](https://www.bilibili.com/video/BV1th411z7sn?spm_id_from=333.788.videopod.episodes&vd_source=82fdaa13f57d339420a33b8e98a53858&p=5)

### GPIO简介

共八种输入输出模式，引脚电平为0V~3.3V, 部分引脚可容忍5V电平。

- 输出模式下可控制端口输出输出高低电平，用来驱动led，蜂鸣器，模拟通信协议输出时序等。
- 输入模式下可读取端口的高低电平或电压值，用来读取案件输入，外接模块电平信号输入，ADC电压采集，模拟通信协议接收数据等。

### GPIO基本结构

![GPIO基本结构](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250415221259703.png)

### GPIO位结构

![GPIO位结构](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250415221528161.png)

IO引脚(0~3.3V)

- 上保护二极管: 如果输入电压大于3.3V, 二极管导通，电流流入VDD引脚，保护内部电路。
- 下保护二极管: 如果输入电压小于0V, 二极管导通，电流流出IO引脚，保护内部电路。

输入驱动器

1. 上拉输入模式：上面导通，下面断开，连接到VDD, 当IO引脚悬空时，输入电平默认为高电平。
2. 下拉输入模式：下面导通，上面断开，连接到VSS, 当IO引脚悬空时，输入电平默认为低电平。
3. 浮空输入模式：上面和下面都断开，当IO引脚悬空时，输入电平不确定。
4. 施密特触发器：肖特基触发器为翻译错误，实际应为施密特触发器，作用为当输入电压高于某一阈值时，输出高电平，当输入电压低于某一阈值时，输出低电平。
5. 模拟输入: 在进入施密特触发器前引出来，输入电压范围为0V~3.3V, 适合ADC采集。
6. 复用功能输入：经过施密特触发器后引出，为数字量。
7. 读出：经过施密特触发器后进入输入数据寄存器，读出数据。

输出寄存器

1. 位设置/清除寄存器：可单独设置或清除某一位的数据且不影响其他位。
2. 输出数据寄存器：同时控制16个端口且只能整体读写。
3. 推挽输出模式：PMOS和NMOS均有效，数据寄存器为1时上管导通，下管断开，输出为VDD即高电平；数据寄存器为0是下管导通，上管断开，输出为VSS即低电平，高低电平均有驱动能力。
4. 开漏输出模式：PMOS无效，数据寄存器为1时下管断开，输出为高组态模式；数据寄存器为0时下管导通，输出为低电平，对高电平没有驱动能力，可作为I2C的通信引脚，在多级通讯下可避免设备干扰，还可以用于输出5V的电平信号。
5. 关闭输出：当引脚配置为输入模式时，PMOSS和NMOS均无效，端口电平由外部电平控制。

### GPIO模式

GPIO模式

![GPIO模式](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250415233712806.png)

浮空/上拉/下拉输入

![输入浮空/上拉/下拉配置](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250415233836743.png)

模拟输入

![模拟输入](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250415234242718.png)

开漏/推挽输出

![开漏/推挽输出](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250415234345490.png)

复用开漏/推挽输出

![复用开漏/推挽输出](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250415234440037.png)

## 实验测试

工程文件目录: `3-2 GPIO输出-LED闪烁`

[GPIO输出实验测试视频链接](https://www.bilibili.com/video/BV1th411z7sn?spm_id_from=333.788.videopod.episodes&vd_source=82fdaa13f57d339420a33b8e98a53858&p=6)

### 实验目标

使用GPIO输出高低电平，控制LED灯的亮灭。

### LED硬件电路图

![LED硬件连线电路图](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250415235324570.png)

LED红绿蓝均为低电平点亮，红灯接PB5,绿灯接PB0,蓝灯接PB1。

### 软件设计

PB5（红灯）为GPIO输出模式，推挽输出模式。低电平点亮。

添加系统延时模块在System文件夹下

Delay.c中定义

```c
#include "stm32f10x.h"

/**
  * @brief  微秒级延时
  * @param  xus 延时时长，范围：0~233015
  * @retval 无
  */
void Delay_us(uint32_t xus)
{
	SysTick->LOAD = 72 * xus;				//设置定时器重装值
	SysTick->VAL = 0x00;					//清空当前计数值
	SysTick->CTRL = 0x00000005;				//设置时钟源为HCLK，启动定时器
	while(!(SysTick->CTRL & 0x00010000));	//等待计数到0
	SysTick->CTRL = 0x00000004;				//关闭定时器
}

/**
  * @brief  毫秒级延时
  * @param  xms 延时时长，范围：0~4294967295
  * @retval 无
  */
void Delay_ms(uint32_t xms)
{
	while(xms--)
	{
		Delay_us(1000);
	}
}
 
/**
  * @brief  秒级延时
  * @param  xs 延时时长，范围：0~4294967295
  * @retval 无
  */
void Delay_s(uint32_t xs)
{
	while(xs--)
	{
		Delay_ms(1000);
	}
} 


```

Delay.h中声明

```c
#ifndef __DELAY_H
#define __DELAY_H

void Delay_us(uint32_t us);
void Delay_ms(uint32_t ms);
void Delay_s(uint32_t s);

#endif

```

在主函数中控制LED闪烁

1. 打开GPIOB时钟
2. 初始化GPIOB，设置PB5为推挽输出模式，速度50MHz
3. 进入死循环，先将PB5拉低，延时500ms，再将PB5拉高，延时500ms。

代码如下

```c
/*
 * 实验目标：实现LED红灯闪烁，接在PB5，低电平点亮
 */
 
#include "stm32f10x.h"
#include "Delay.h"


int main(void)
{
	// 打开GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	
	// 初始化GPIOB
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; // Pin5
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	while (1)
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_5); // 低电平点亮
		Delay_ms(500); // 500ms延时
		GPIO_SetBits(GPIOB, GPIO_Pin_5); // 高电平熄灭
		Delay_ms(500); // 500ms延时
	}
}

```
