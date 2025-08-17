#include "led.h"

/* LED数组定义 */
LED_TypeDef LED_Array[LED_COUNT] = {
    {GPIOB, GPIO_Pin_0},  // LEDG
    {GPIOB, GPIO_Pin_1},  // LEDB
    {GPIOB, GPIO_Pin_5}   // LEDR
};

/* LED GPIO 时钟判断函数 */
static void LED_EnableClock(GPIO_TypeDef* GPIOx)
{
    if (GPIOx == GPIOA) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    else if (GPIOx == GPIOB) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    else if (GPIOx == GPIOC) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    else if (GPIOx == GPIOD) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    else if (GPIOx == GPIOE) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
}

/* 初始化LED */
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    for (uint8_t i = 0; i < LED_COUNT; i++)
    {
        LED_EnableClock(LED_Array[i].GPIOx);

        GPIO_InitStructure.GPIO_Pin = LED_Array[i].Pin;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(LED_Array[i].GPIOx, &GPIO_InitStructure);

        /* 默认关闭LED */
        GPIO_WriteBit(LED_Array[i].GPIOx, LED_Array[i].Pin, LED_OFF_LEVEL);
    }
}

/* 绿色LED */
void LEDG_ON(void)  { LED_ON(LED_Array[0]); }
void LEDG_OFF(void) { LED_OFF(LED_Array[0]); }
void LEDG_Turn(void) { LED_TOGGLE(LED_Array[0]); }

/* 蓝色LED */
void LEDB_ON(void)  { LED_ON(LED_Array[1]); }
void LEDB_OFF(void) { LED_OFF(LED_Array[1]); }
void LEDB_Turn(void) { LED_TOGGLE(LED_Array[1]); }

/* 红色LED */
void LEDR_ON(void)  { LED_ON(LED_Array[2]); }
void LEDR_OFF(void) { LED_OFF(LED_Array[2]); }
void LEDR_Turn(void) { LED_TOGGLE(LED_Array[2]); }

// LED驱动测试函数
// #include "stm32f10x.h"
// #include "Delay.h"
// #include "Led.h"

// int main(void)
// {   
//     LED_Init();
//     while (1)
//     {
//         /* 红色LED闪烁 */
//         LEDR_ON();
//         Delay_ms(500);
//         LEDR_OFF();
//         Delay_ms(500);

//         /* 蓝色LED闪烁 */
//         LEDB_ON();
//         Delay_ms(500);
//         LEDB_OFF();
//         Delay_ms(500);

//         /* 绿色LED闪烁 */
//         LEDG_ON();
//         Delay_ms(500);
//         LEDG_OFF();
//         Delay_ms(500);  
//     }
// }
