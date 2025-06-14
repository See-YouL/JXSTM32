/*
 * OLED7针SPI接线:
 * SCK 接 PG12
 * SDA 接 PD5
 * RES 接 PD4
 * DC 接 PD15
 * CS 接 PD1
 */

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "delay.h"
#include "sys.h"
// #include "oled.h"
// #include "bmp.h"
#include "PWM.h"

uint8_t i;

int main(void)
{
	delay_init();
    PWM_Init(); // TIM2的PWM初始化

	while(1)
	{
        for(i = 0; i <= 100; i++)
        {
            PWM_SetCompare1(i); // 设置PWM占空比
            delay_ms(10); // 延时10ms
        }

        for(i = 0; i <= 100; i++)
        {
            PWM_SetCompare1(100 - i); // 设置PWM占空比
            delay_ms(10); // 延时10ms
        }
	}
}

