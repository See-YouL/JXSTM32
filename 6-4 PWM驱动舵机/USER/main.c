/*
 * 实验目标: 实现按键按下增加舵机角度并在OLED显示当前角度
 *
 * OLED接线:
 * GND -> GND(需要与STM32的GND共地)
 * VDD -> 3.3V
 * SCK -> PG12
 * SDA -> PD5
 * RES -> PD4
 * DC -> PD15
 * CS -> PD1
 *
 * 舵机接线:
 * VCC(舵机红色线) -> +5V
 * GND(舵机棕色线) -> GND(需要与STM32的GND共地)
 * PWM(舵机黄色线) -> TIM2_CH2引脚(PA1)
 */

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
#include "PWM.h"
#include "Servo.h"
#include "KEY.h"

uint8_t KeyNum; // 接收键码的变量
float Angle; // 舵机角度变量

int main(void)
{
	delay_init(); // 延时初始化
	OLED_Init(); // OLED初始化
    Servo_Init(); // 舵机初始化
	
	OLED_Clear(); // OLED清屏
	OLED_ShowString(1, 11, "Angle:", 12); // 在(1, 11)位置显示"Angle:"字体大小12
	OLED_Refresh(); // 更新显存到OLED

	while(1)
	{
        KeyNum = KEY_GetNum(); // 获取按键值
        if (KeyNum == 1) // 按键按下
        {
            Angle += 10; // 每次按下增加10度
            if (Angle > 180) // 限制角度在0~180度之间
            {
                Angle = 0;
            }
            Servo_SetAngle(Angle); // 设置舵机角度
            OLED_ShowNum(41, 11, Angle, 3, 12); // 在(41, 11)位置显示当前角度, 字体大小12
            OLED_Refresh(); // 更新显存到OLED
        }
	}
}
