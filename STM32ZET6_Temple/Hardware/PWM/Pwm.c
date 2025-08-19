/**
 * @file PWM.c
 * @brief PWM驱动实现文件（支持可移植配置）
 * @version 1.0
 * @date 2025-08-17
 * @author 含生
 */

#include "stm32f10x.h"
#include <stddef.h>   // 定义了NULL
#include "Pwm.h"


#define SYS_CLOCK 72000000UL   // 系统时钟 72MHz

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

/**
 * @brief 设置TIMx的预分频器 (PSC)
 * @param config 指向PWM配置的结构体
 * @param Prescaler 要写入的PSC值 (真实PSC = Prescaler - 1)
 * @retval None
 */
void PWM_SetPrescaler(PWM_Config_t *config, uint16_t Prescaler)
{
    config->Prescaler = Prescaler; // 更新结构体成员，便于后续使用
    TIM_PrescalerConfig(config->TIMx, Prescaler - 1, TIM_PSCReloadMode_Update);
}

/**
 * @brief 设置PWM周期 (修改ARR)
 * @param config 指向PWM配置的结构体
 * @param Period 要写入的ARR值 (真实ARR = Period - 1)
 * @retval None
 */
void PWM_SetPeriod(PWM_Config_t *config, uint16_t Period)
{
    TIM_SetAutoreload(config->TIMx, Period - 1);
}

/**
 * @brief 更新PWM的频率和占空比
 * @param config PWM配置结构体
 * @param freq 目标频率 (Hz)
 * @param duty 占空比 (0 ~ 100 [%])
 */
void PWM_UpdateFreqDuty(PWM_Config_t *config, uint32_t freq, float duty)
{
    uint32_t psc = 0;
    uint32_t arr = 0;

    if (freq == 0) return;  // 防止除零

    // 计算合适的 PSC 和 ARR
    // 目标： F_pwm = SYS_CLOCK / ((PSC+1) * (ARR+1))
    // 我们固定 PSC 在 0~65535，先粗略取一个
    psc = (SYS_CLOCK / (freq * 65536UL));
    if (psc < 1) psc = 1;
    if (psc > 0xFFFF) psc = 0xFFFF;

    arr = (SYS_CLOCK / (freq * (psc + 1))) - 1;
    if (arr > 0xFFFF) arr = 0xFFFF;

    // 更新定时器参数
    TIM_PrescalerConfig(config->TIMx, psc, TIM_PSCReloadMode_Update);
    TIM_SetAutoreload(config->TIMx, arr);

    // 更新占空比
    uint16_t ccr = (uint16_t)((duty / 100.0f) * (arr + 1));
    PWM_SetCompare(config, ccr);
}

/**
 * @brief 获取当前PWM的实际频率和占空比
 * @param config PWM配置结构体
 * @param freq 指向保存实际频率的变量
 * @param duty 指向保存占空比的变量
 */
void PWM_GetFreqDuty(PWM_Config_t *config, uint32_t *freq, float *duty)
{
    uint32_t psc = config->TIMx->PSC;   // 当前预分频
    uint32_t arr = config->TIMx->ARR;   // 当前自动重装载

    // 实际频率 = SYS_CLOCK / ((PSC+1)*(ARR+1))
    if (freq != NULL)
    {
        *freq = SYS_CLOCK / ((psc + 1) * (arr + 1));
    }

    // 当前 CCR (比较寄存器)
    uint32_t ccr = 0;
    switch (config->Channel)
    {
    case 1: ccr = config->TIMx->CCR1; break;
    case 2: ccr = config->TIMx->CCR2; break;
    case 3: ccr = config->TIMx->CCR3; break;
    case 4: ccr = config->TIMx->CCR4; break;
    default: break;
    }

    // 占空比 = CCR / (ARR+1) * 100%
    if (duty != NULL)
    {
        *duty = ((float)ccr / (float)(arr + 1)) * 100.0f;
    }
}

// /*
// 硬件连接:
// PA6 (TIM3_CH1) -> LED正极
// LED负极 -> GND
// */
// #include "stm32f10x.h"
// #include "Delay.h"
// #include "Led.h"
// #include "Pwm.h"

// // LED PWM配置
// PWM_Config_t LED_PWM = {
//     .TIMx = TIM3,                // TIM3控制LED
//     .TIMx_CLK = RCC_APB1Periph_TIM3,
//     .Prescaler = 720-1,          // 72MHz / 720 = 100kHz
//     .Period = 100-1,             // PWM周期100
//     .Pulse = 0,                  // 初始占空比0
//     .Channel = 1,                // TIM3 CH1
//     .GPIOx = GPIOA,
//     .GPIO_Pin = GPIO_Pin_6,      // 对应PA6引脚
//     .ClockDivision = TIM_CKD_DIV1,
//     .CounterMode = TIM_CounterMode_Up,
//     .OCMode = TIM_OCMode_PWM1,
//     .OCPolarity = TIM_OCPolarity_High,
//     .OutputState = TIM_OutputState_Enable
// };

// int main(void)
// {   
//     uint16_t duty = 0;
//     int8_t step = 1;

//     // 初始化PWM
//     PWM_Init(&LED_PWM);

//     while(1)
//     {
//         // 设置PWM占空比控制LED亮度
//         PWM_SetCompare(&LED_PWM, duty);

//         // 呼吸灯效果
//         duty += step;
//         if(duty == 100) step = -1;
//         else if(duty == 0) step = 1;

//         Delay_ms(10); // 调整呼吸速度
//     }
// }
