#include "Ic.h"

/**
 * @brief 输入捕获初始化函数
 * @param config 指向输入捕获配置的结构体
 */
void IC_Init(IC_Config_t *config)
{
    // 1. 开启定时器时钟
    if (config->TIMx == TIM1 || config->TIMx == TIM8)
        RCC_APB2PeriphClockCmd(config->TIMx_CLK, ENABLE);
    else
        RCC_APB1PeriphClockCmd(config->TIMx_CLK, ENABLE);

    // 2. 开启GPIO时钟
    if (config->GPIOx == GPIOA) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    else if (config->GPIOx == GPIOB) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    else if (config->GPIOx == GPIOC) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    else if (config->GPIOx == GPIOD) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    else if (config->GPIOx == GPIOE) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

    // 3. 配置GPIO为输入上拉
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = config->GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   // 上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(config->GPIOx, &GPIO_InitStructure);

    // 4. 定时器时钟源
    TIM_InternalClockConfig(config->TIMx);

    // 5. 配置时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = config->ClockDivision;
    TIM_TimeBaseInitStructure.TIM_CounterMode = config->CounterMode;
    TIM_TimeBaseInitStructure.TIM_Period = config->Period - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = config->Prescaler - 1;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = config->RepetitionCounter;
    TIM_TimeBaseInit(config->TIMx, &TIM_TimeBaseInitStructure);

    // 6. 配置输入捕获
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_Channel = (config->Channel == 1) ? TIM_Channel_1 :
                                      (config->Channel == 2) ? TIM_Channel_2 :
                                      (config->Channel == 3) ? TIM_Channel_3 : TIM_Channel_4;
    TIM_ICInitStructure.TIM_ICPolarity  = config->ICPolarity;
    TIM_ICInitStructure.TIM_ICSelection = config->ICSelection;
    TIM_ICInitStructure.TIM_ICPrescaler = config->ICPrescaler;
    TIM_ICInitStructure.TIM_ICFilter    = config->ICFilter;

    if (config->UsePWMI) // PWM输入捕获
    {
        TIM_PWMIConfig(config->TIMx, &TIM_ICInitStructure);
    }
    else // 普通输入捕获
    {
        TIM_ICInit(config->TIMx, &TIM_ICInitStructure);
    }

    // 7. 配置触发源和从模式
    TIM_SelectInputTrigger(config->TIMx, config->TriggerSource);
    TIM_SelectSlaveMode(config->TIMx, config->SlaveMode);

    // 8. 启动定时器
    TIM_Cmd(config->TIMx, ENABLE);
}

uint32_t IC_GetCapture(IC_Config_t *config, uint8_t which)
{
    if (config->UsePWMI)  // 使用 PWM 输入模式
    {
        if (config->Channel == 1)
        {
            if (which == 0) return TIM_GetCapture2(config->TIMx); // Ton
            else            return TIM_GetCapture1(config->TIMx); // Ttotal
        }
        else if (config->Channel == 2)
        {
            if (which == 0) return TIM_GetCapture1(config->TIMx); // Ton
            else            return TIM_GetCapture2(config->TIMx); // Ttotal
        }
        else if (config->Channel == 3)
        {
            if (which == 0) return TIM_GetCapture4(config->TIMx); // Ton
            else            return TIM_GetCapture3(config->TIMx); // Ttotal
        }
        else if (config->Channel == 4)
        {
            if (which == 0) return TIM_GetCapture3(config->TIMx); // Ton
            else            return TIM_GetCapture4(config->TIMx); // Ttotal
        }
    }
    else  // 普通输入捕获
    {
        switch(config->Channel)
        {
        case 1: return (which == 0) ? TIM_GetCapture1(config->TIMx) : TIM_GetCapture2(config->TIMx);
        case 2: return (which == 0) ? TIM_GetCapture2(config->TIMx) : TIM_GetCapture1(config->TIMx);
        case 3: return (which == 0) ? TIM_GetCapture3(config->TIMx) : TIM_GetCapture4(config->TIMx);
        case 4: return (which == 0) ? TIM_GetCapture4(config->TIMx) : TIM_GetCapture3(config->TIMx);
        }
    }
    return 0;
}


uint32_t IC_GetFreq(IC_Config_t *config)
{
    uint32_t Fc = 72000000 / config->Prescaler; // 计数频率
    uint32_t Ttotal = IC_GetCapture(config, 1); // 周期计数值

    if (Ttotal == 0) return 0;  // 防止除零
    return Fc / Ttotal;         // f = Fc / N
}

uint8_t IC_GetDuty(IC_Config_t *config)
{
    uint32_t Ton    = IC_GetCapture(config, 0); // 高电平时间
    uint32_t Ttotal = IC_GetCapture(config, 1); // 周期

    if (Ttotal == 0) return 0;
    if (Ton > Ttotal) Ton = Ttotal; // 修正异常值

    return (uint8_t)((Ton * 100) / Ttotal);
}


// // 输入捕获模式测频率测试函数
// /*
// 硬件连接：
// OLED_SCK -> PB8
// OLED_SDA -> PB9
// PA0 -> PA6
// */
// #include "stm32f10x.h"
// #include "Delay.h"
// #include "Oled.h"
// #include "Pwm.h"
// #include "Ic.h"

// PWM_Config_t PWM_config = {
//     .TIMx = TIM2,
//     .TIMx_CLK = RCC_APB1Periph_TIM2,
//     .Prescaler = 720 - 1, // PSC = 719, fTIM = 72MHz/720 = 100kHz
//     .Period = 100 - 1, // ARR = 99, fPWM = 100kHz/100 = 1kHz
//     .Pulse = 50,                  // CCR = 50, 占空比 50%
//     .Channel = 1,                 // TIM2_CH1 (PA0)
//     .GPIOx = GPIOA,
//     .GPIO_Pin = GPIO_Pin_0,
//     .ClockDivision = TIM_CKD_DIV1,
//     .CounterMode = TIM_CounterMode_Up,
//     .OCMode = TIM_OCMode_PWM1,
//     .OCPolarity = TIM_OCPolarity_High,
//     .OutputState = TIM_OutputState_Enable
// };

// IC_Config_t IC_Config = {
//     .TIMx = TIM3,
//     .TIMx_CLK = RCC_APB1Periph_TIM3,
//     .Prescaler = 72 - 1,          // PSC = 71, fTIM = 72MHz/72 = 1MHz
//     .Period = 65536 - 1,             // 最大计数
//     .ClockDivision = TIM_CKD_DIV1,
//     .CounterMode = TIM_CounterMode_Up,
//     .RepetitionCounter = 0,       // 基本定时器无效

//     .Channel = 1,                 // TIM3_CH1 (PA6)
//     .GPIOx = GPIOA,
//     .GPIO_Pin = GPIO_Pin_6,

//     .ICPolarity = TIM_ICPolarity_Rising, // 上升沿捕获
//     .ICSelection = TIM_ICSelection_DirectTI,
//     .ICPrescaler = TIM_ICPSC_DIV1,
//     .ICFilter = 0x00,

//     .SlaveMode = TIM_SlaveMode_Reset,   // 从模式复位
//     .TriggerSource = TIM_TS_TI1FP1      // TI1FP1 作为触发源
// };

// uint32_t freq = 0;

// int main(void)
// {   
//     OLED_Init();
//     PWM_Init(&PWM_config);
//     IC_Init(&IC_Config);

//     OLED_Clear();
//     OLED_ShowString(1, 1, "Freq:"); 
//     OLED_ShowString(1, 13, "Hz");

//     PWM_SetCompare(&PWM_config, 50); // 设置占空比为50%
//     PWM_SetPrescaler(&PWM_config, 720); // 设置PSC, PWM频率为 72M/720/100 = 1KHz

//     while(1)
//     {
//         freq = IC_GetFreq(&IC_Config);   // 获取测量频率
//         OLED_ShowNum(1, 7, freq, 5);   // 显示频率 (最大5位数)
//         Delay_ms(20);                 // 延时，避免刷屏过快
//     }
// }

// // PWMI模式测量占空比测试函数
// #include "stm32f10x.h"
// #include "Delay.h"
// #include "Oled.h"
// #include "Pwm.h"
// #include "Ic.h"

// PWM_Config_t PWM_Config = {
//     .TIMx = TIM2,
//     .TIMx_CLK = RCC_APB1Periph_TIM2,
//     .Prescaler = 720 - 1, // PSC = 719, fTIM = 72MHz/720 = 100kHz
//     .Period = 100 - 1, // ARR = 99, fPWM = 100kHz/100 = 1kHz
//     .Pulse = 50,                  // CCR = 50, 占空比 50%
//     .Channel = 1,                 // TIM2_CH1 (PA0)
//     .GPIOx = GPIOA,
//     .GPIO_Pin = GPIO_Pin_0,
//     .ClockDivision = TIM_CKD_DIV1,
//     .CounterMode = TIM_CounterMode_Up,
//     .OCMode = TIM_OCMode_PWM1,
//     .OCPolarity = TIM_OCPolarity_High,
//     .OutputState = TIM_OutputState_Enable
// };

// IC_Config_t IC_Config = {
//     .TIMx = TIM3,
//     .TIMx_CLK = RCC_APB1Periph_TIM3,
//     .Prescaler = 72 - 1,          // PSC = 71, fTIM = 72MHz/72 = 1MHz
//     .Period = 65536 - 1,             // 最大计数
//     .ClockDivision = TIM_CKD_DIV1,
//     .CounterMode = TIM_CounterMode_Up,
//     .RepetitionCounter = 0,       // 基本定时器无效

//     .Channel = 1,                 // TIM3_CH1 (PA6)
//     .GPIOx = GPIOA,
//     .GPIO_Pin = GPIO_Pin_6,

//     .ICPolarity = TIM_ICPolarity_Rising, // 上升沿捕获
//     .ICSelection = TIM_ICSelection_DirectTI,
//     .ICPrescaler = TIM_ICPSC_DIV1,
//     .ICFilter = 0x00,

//     .SlaveMode = TIM_SlaveMode_Reset,   // 从模式复位
//     .TriggerSource = TIM_TS_TI1FP1,      // TI1FP1 作为触发源
//     .UsePWMI = 1 // 使能PWMI 模式
// };

// uint32_t freq = 0;
// uint32_t duty = 0;

// int main(void)
// {   
//     OLED_Init();
//     PWM_Init(&PWM_Config);
//     IC_Init(&IC_Config);

//     OLED_Clear();
//     OLED_ShowString(1, 1, "Freq:"); 
//     OLED_ShowString(1, 13, "Hz");
//     OLED_ShowString(2, 1, "Duty:"); 
//     OLED_ShowString(2, 13, "%");

//     PWM_UpdateFreqDuty(&PWM_Config, 1000, 50); // 设置 PWM 占空比50%和频率1KHz

//     while(1)
//     {
//         freq = IC_GetFreq(&IC_Config);   // 获取测量频率
//         duty = IC_GetDuty(&IC_Config);   // 获取占空比
//         OLED_ShowNum(1, 7, freq, 5);   // 显示频率 (最大5位数)
//         OLED_ShowNum(2, 7, duty, 5);   // 显示占空比 (最大5位数)
//         Delay_ms(20);                 // 延时，避免刷屏过快
//     }
// }
