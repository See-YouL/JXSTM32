#include "PWM.h"
#include "stm32f10x.h"
#include "stm32f10x_conf.h"

void PWM_Init(void)
{
    // 1. 开启时钟, 打开TIM2外设, GPIO外设和AFIO的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // 开启TIM2时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // 开启AFIO时钟, 复用功能需要

    /*    // 如果需要将TIM2_CH1_ETR重映射到PA15, 需要进行以下配置
    // TIM2_CH1_ETR重映射到PA15
    GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE); // 部分重映射TIM2_CH1_ETR到PA15

    // 解除PA15的默认调试复用功能
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); // 禁用JTAG, 只保留SWD功能

    // 2. 选择时基单元时钟源(默认使用内部时钟)
    TIM_InternalClockConfig(TIM2); // 选择内部时钟
    */

    // 3. 配置时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 滤波采样频率一分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1; // ARR + 1 = 100  
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1; // PSC + 1 = 720
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // 重复计数器(高级定时器才有的),这里不用,赋值0
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure); // 初始化TIM2时基单元

    // 4. 配置输出比较通道
    TIM_OCInitTypeDef TIM_OCInitStructure; // 定义输出比较结构体
    TIM_OCStructInit(&TIM_OCInitStructure); // 初始化输出比较结构体
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; // 设置输出比较模式为PWM1
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // 设置输出极性为高
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 使能输出比较
    TIM_OCInitStructure.TIM_Pulse = 50; // CCR = 50
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);

    // 5. 配置PWM对应的GPIO
    GPIO_InitTypeDef GPIO_InitStructure; // 定义GPIO初始化结构体
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; // 选择PA0引脚, TIM2_CH1默认映射到PA0
    /*    // 如果需要将TIM2_CH1_ETR重映射到PA15, 则使用以下配置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; // 选择PA15引脚, TIM2_CH1_ETR重映射到PA15
    */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 设置为复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 设置GPIO速度为50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure); // 初始化GPIOA

    // 6. 运行控制,启动计数器
    TIM_Cmd(TIM2, ENABLE); // 使能TIM2计数器

}

void PWM_SetCompare1(uint16_t Compare)
{
    TIM_SetCompare1(TIM2, Compare); // 设置TIM2通道1的比较值
}
