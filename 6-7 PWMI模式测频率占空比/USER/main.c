/*
 * 实验目标: 实现PWMI模式测量频率和占空比并在OLED显示
 *
 * PWM接线:
 *
 * PWM信号由PA0产生,PA6进行输入捕获的接收,接线如下所示
 *
 * - PA6 -> PA0
 * - PA0 -> PA6
 * 
 * OLED接线:
 *
 * - GND -> GND(需要与STM32的GND共地)
 * - VDD -> 3.3V
 * - SCK -> PG12
 * - SDA -> PD5
 * - RES -> PD4
 * - DC -> PD15
 * - CS -> PD1
 */
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
#include "PWM.h"
#include "IC.h"


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
