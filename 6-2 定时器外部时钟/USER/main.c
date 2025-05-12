/*
 * 实验目标: 使用外部时钟信号进行定时中断
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
    OLED_ShowString(1, 20, "CNT = ", 12); // 显示"CNT ="

	while(1)
	{
        OLED_ShowNum(40, 1, Num, 5, 12); // 显示Num的值
        OLED_ShowNum(40, 20, Timer_GetCounter(), 5, 12); // 显示计数器的值
		OLED_Refresh();
	}
}
