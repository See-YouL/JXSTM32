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
