#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

/* LED逻辑电平定义 */
#define LED_ON_LEVEL     Bit_RESET
#define LED_OFF_LEVEL    Bit_SET

/* LED结构体定义 */
typedef struct
{
    GPIO_TypeDef* GPIOx;   // LED对应端口
    uint16_t Pin;          // LED对应引脚
} LED_TypeDef;

/* LED数量 */
#define LED_COUNT 3

/* LED数组声明 */
extern LED_TypeDef LED_Array[LED_COUNT];

/* LED操作宏 */
#define LED_ON(led)       GPIO_WriteBit((led).GPIOx, (led).Pin, LED_ON_LEVEL)
#define LED_OFF(led)      GPIO_WriteBit((led).GPIOx, (led).Pin, LED_OFF_LEVEL)
#define LED_TOGGLE(led)   \
    do { \
        if (GPIO_ReadOutputDataBit((led).GPIOx, (led).Pin) == LED_ON_LEVEL) \
            GPIO_WriteBit((led).GPIOx, (led).Pin, LED_OFF_LEVEL); \
        else \
            GPIO_WriteBit((led).GPIOx, (led).Pin, LED_ON_LEVEL); \
    } while(0)

/* 函数声明 */
void LED_Init(void);

/* 单独LED控制函数 */
void LEDG_ON(void); void LEDG_OFF(void); void LEDG_Turn(void);
void LEDB_ON(void); void LEDB_OFF(void); void LEDB_Turn(void);
void LEDR_ON(void); void LEDR_OFF(void); void LEDR_Turn(void);

#endif
