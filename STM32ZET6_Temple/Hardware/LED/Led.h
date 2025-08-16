#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

// GPIO端口定义
#define LED_GPIO_PORT       GPIOB
#define LED_GPIO_CLK        RCC_APB2Periph_GPIOB

// LED引脚定义
#define LEDG_PIN            GPIO_Pin_0
#define LEDB_PIN            GPIO_Pin_1
#define LEDR_PIN            GPIO_Pin_5

// LED逻辑电平定义
// LED默认关闭为高电平，打开为低电平
#define LED_ON_LEVEL        Bit_RESET
#define LED_OFF_LEVEL       Bit_SET

// LED操作宏定义，简化LED控制
#define LED_ON(GPIOx, PIN)     GPIO_WriteBit(GPIOx, PIN, LED_ON_LEVEL)
#define LED_OFF(GPIOx, PIN)    GPIO_WriteBit(GPIOx, PIN, LED_OFF_LEVEL)
#define LED_TOGGLE(GPIOx, PIN) \
    do { \
        if (GPIO_ReadOutputDataBit(GPIOx, PIN) == LED_ON_LEVEL) \
            GPIO_WriteBit(GPIOx, PIN, LED_OFF_LEVEL); \
        else \
            GPIO_WriteBit(GPIOx, PIN, LED_ON_LEVEL); \
    } while(0)

// 函数声明
void LED_Init(void);

void LEDG_ON(void);
void LEDG_OFF(void);
void LEDG_Turn(void);

void LEDB_ON(void);
void LEDB_OFF(void);
void LEDB_Turn(void);

void LEDR_ON(void);
void LEDR_OFF(void);
void LEDR_Turn(void);

#endif
