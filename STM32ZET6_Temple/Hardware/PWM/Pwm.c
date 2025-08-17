/**
 * @file PWM.c
 * @brief PWM驱动实现文件（支持可移植配置）
 * @version 1.0
 * @date 2025-08-17
 * @author 含生
 */

#include "Pwm.h"
#include "stm32f10x.h"

/**
 * @brief PWM初始化函数
 * @note 该函数用于初始化 PWM, 包括时钟、GPIO、时基单元和输出比较
 *     并在所选的通道上输出 PWM
 * @param config 指向PWM配置的结构体
 * @retval None
 */
void PWM_Init(PWM_Config_t *config)
{
    // 1. 开启时钟
    if (config->TIMx == TIM1) RCC_APB2PeriphClockCmd(config->TIMx_CLK, ENABLE);
    else RCC_APB1PeriphClockCmd(config->TIMx_CLK, ENABLE);

    // 开启GPIO时钟
    if (config->GPIOx == GPIOA) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    else if (config->GPIOx == GPIOB) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    else if (config->GPIOx == GPIOC) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    else if (config->GPIOx == GPIOD) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    else if (config->GPIOx == GPIOE) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

    // 使能复用功能时钟(复用功能需要)
    // RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    // 2. 内部时钟
    TIM_InternalClockConfig(config->TIMx);

    // 3. 配置时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = config->ClockDivision; // 时钟分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = config->CounterMode; // 计数器模式
    TIM_TimeBaseInitStructure.TIM_Period = config->Period - 1; // ARR + 1 = Period
    TIM_TimeBaseInitStructure.TIM_Prescaler = config->Prescaler - 1; // PSC + 1 = Prescaler
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // 重复计数器(高级定时器才有的), 这里不用, 赋值0
    TIM_TimeBaseInit(config->TIMx, &TIM_TimeBaseInitStructure);

    // 4. 配置输出比较
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = config->OCMode; // 设置输出比较模式
    TIM_OCInitStructure.TIM_OCPolarity = config->OCPolarity; // 设置输出极性
    TIM_OCInitStructure.TIM_OutputState = config->OutputState; // 使能输出比较
    TIM_OCInitStructure.TIM_Pulse = config->Pulse; // CCR = Pulse

    switch (config->Channel) // 选择通道进行输出比较初始化
    {
    case 1: TIM_OC1Init(config->TIMx, &TIM_OCInitStructure); break;
    case 2: TIM_OC2Init(config->TIMx, &TIM_OCInitStructure); break;
    case 3: TIM_OC3Init(config->TIMx, &TIM_OCInitStructure); break;
    case 4: TIM_OC4Init(config->TIMx, &TIM_OCInitStructure); break;
    default: break;
    }

    // 5. 配置GPIO
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = config->GPIO_Pin; // 选择GPIO引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 设置为复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 设置GPIO速度为50MHz
    GPIO_Init(config->GPIOx, &GPIO_InitStructure);

    // 6. 启动定时器
    TIM_Cmd(config->TIMx, ENABLE);
}

/**
 * @brief 单独写入TIMx通道的CCR
 * @param config 指向PWM配置的结构体
 * @param Compare 要写入的CCR值
 * @retval None
 */
void PWM_SetCompare(PWM_Config_t *config, uint16_t Compare)
{
    switch (config->Channel)
    {
    case 1: TIM_SetCompare1(config->TIMx, Compare); break;
    case 2: TIM_SetCompare2(config->TIMx, Compare); break;
    case 3: TIM_SetCompare3(config->TIMx, Compare); break;
    case 4: TIM_SetCompare4(config->TIMx, Compare); break;
    default: break;
    }
}
