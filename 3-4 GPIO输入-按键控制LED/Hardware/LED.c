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
