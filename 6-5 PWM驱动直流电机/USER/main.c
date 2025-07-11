/*
 * 实验目标: 实现按键控制直流电机转速并在OLED显示当前速度
 *
 * 130电机和TB6612FNG电机驱动板连线如下:
 *
 * - VM -> 5V
 * - VCC -> 3.3V
 * - GND -> GND(需要与STM32的GND共地)
 * - AO1 -> 电机端口1
 * - AO2 -> 电机端口2(不分正反)
 * - PWMA -> TIM2_CH3引脚(PA2)
 * - AIN2 -> PA5
 * - AIN1 -> PA4
 * - STBY -> 3.3V
 *
 * OLED连接:
 *
 * - GND -> GND(需要与STM32的GND共地)
 * - VDD -> 3.3V
 * - SCK -> PG12
 * - SDA -> PD5
 * - RES -> PD4
 * - DC -> PD15
 * - CS -> PD1
 *
 * 按键使用霸道开发板按键(PA0)
 */

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
#include "Motor.h"
#include "KEY.h"

uint8_t KeyNum; // 按键编号变量
int8_t Speed; // 电机速度变量

int main(void)
{
	delay_init(); // 延时初始化
    OLED_Init(); // OLED初始化
    Motor_Init(); // 电机初始化

	OLED_Clear(); // OLED清屏
	OLED_ShowString(1, 11, "Speed:", 12); // 在(1, 11)位置显示"Speed:"字体大小12
	OLED_Refresh(); // 更新显存到OLED

    Motor_SetSpeed(0); // 设置电机初始速度为0

	while(1)
	{
        KeyNum = KEY_GetNum(); // 获取按键编号
        if (KeyNum == 1) // 按键按下
        {
            Speed += 20; // 增加速度
            if (Speed > 100) // 限制最大速度为100
            {
                Speed = -100;
            }
            Motor_SetSpeed(Speed); // 设置电机速度
            OLED_ShowNum(41, 11, Speed, 3, 12); // 显示负数时有BUG
            OLED_Refresh(); // 更新显存到OLED
        }
	}
}
