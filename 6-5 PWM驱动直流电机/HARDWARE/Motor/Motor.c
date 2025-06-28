#include "stm32f10x.h"
#include "PWM.h"

// 初始化PWM(TIM2_CH3)为PA4, PA5控制电机方向和速度
void Motor_Init(void) 
{
    // 初始化PA4, PA5进行电机方向控制
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启GPIOA时钟
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 设置为推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5; // 选择PA4,PA5引脚
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 设置GPIO速度为50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure); // 初始化GPIOA

    // 初始化PWM(TIM2_CH3)
    PWM_Init(); 

}

// 设置电机速度和方向
void Motor_SetSpeed(int8_t Speed)
{
    if (Speed >= 0)
    {
        // 正转
        GPIO_SetBits(GPIOA, GPIO_Pin_4); // 设置PA4为高电平
        GPIO_ResetBits(GPIOA, GPIO_Pin_5); // 设置PA5为低电平
        PWM_SetCompare3(Speed); // 设置PWM占空比
    } 
    else 
    {
        // 反转
        GPIO_ResetBits(GPIOA, GPIO_Pin_4); // 设置PA4为低电平
        GPIO_SetBits(GPIOA, GPIO_Pin_5); // 设置PA5为高电平
        PWM_SetCompare3(-Speed); // 设置PWM占空比(取绝对值)
    }
}
