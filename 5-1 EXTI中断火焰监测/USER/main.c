/*
 * 实验目标: 使用火焰传感器监测火焰并通过OLED屏显示火焰警报
 */
 
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
