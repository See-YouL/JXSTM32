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
