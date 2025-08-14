/**
 * @file main.c
 * @brief 主程序文件，包含主函数和中断处理函数
 *  旋转编码器接线说明：
 *  - 旋转编码器VCC -> 3.3V
 *  - 旋转编码器GND -> GND
 *  - 旋转编码器A相 -> PA6
 *  - 旋转编码器B相 -> PA7
 *  OLED显示屏接线说明：
 *  - OLED VCC -> 3.3V
 */
#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "oled.h"
#include "timer.h"
#include "encoder.h"

int16_t Speed;			//定义速度变量

int main(void)
{
    // 1. 模块初始化
	OLED_Init();		//OLED初始化
	Timer_Init();		//定时器初始化
	Encoder_Init();		//编码器初始化
	
	// 2. OLED显示静态文本
	OLED_ShowString(1, 1, "Speed:");		//1行1列显示字符串Speed:
	
    // 3. 死循环显示速度
	while (1)
	{
		OLED_ShowSignedNum(1, 7, Speed, 5);	//不断刷新显示编码器测得的最新速度
	}
}

// 4. TIM2每隔1s触发一次中断，读取编码器计数增量值作为速度值
/**
 * @brief TIM2中断处理函数
 * @param None
 * @retval None
 */
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)		//判断是否是TIM2的更新事件触发的中断
	{
		Speed = Encoder_Get();								//每隔固定时间段读取一次编码器计数增量值，即为速度值
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);			//清除TIM2更新事件的中断标志位
	}
}
