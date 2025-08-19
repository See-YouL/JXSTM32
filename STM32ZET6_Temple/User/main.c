#include "stm32f10x.h"
#include "Delay.h"
#include "Oled.h"
#include "Pwm.h"
#include "Ic.h"

PWM_Config_t PWM_config = {
    .TIMx = TIM2,
    .TIMx_CLK = RCC_APB1Periph_TIM2,
    .Prescaler = 720 - 1, // PSC = 719, fTIM = 72MHz/720 = 100kHz
    .Period = 100 - 1, // ARR = 99, fPWM = 100kHz/100 = 1kHz
    .Pulse = 50,                  // CCR = 50, 占空比 50%
    .Channel = 1,                 // TIM2_CH1 (PA0)
    .GPIOx = GPIOA,
    .GPIO_Pin = GPIO_Pin_0,
    .ClockDivision = TIM_CKD_DIV1,
    .CounterMode = TIM_CounterMode_Up,
    .OCMode = TIM_OCMode_PWM1,
    .OCPolarity = TIM_OCPolarity_High,
    .OutputState = TIM_OutputState_Enable
};

IC_Config_t IC_Config = {
    .TIMx = TIM3,
    .TIMx_CLK = RCC_APB1Periph_TIM3,
    .Prescaler = 72 - 1,          // PSC = 71, fTIM = 72MHz/72 = 1MHz
    .Period = 65536 - 1,             // 最大计数
    .ClockDivision = TIM_CKD_DIV1,
    .CounterMode = TIM_CounterMode_Up,
    .RepetitionCounter = 0,       // 基本定时器无效

    .Channel = 1,                 // TIM3_CH1 (PA6)
    .GPIOx = GPIOA,
    .GPIO_Pin = GPIO_Pin_6,

    .ICPolarity = TIM_ICPolarity_Rising, // 上升沿捕获
    .ICSelection = TIM_ICSelection_DirectTI,
    .ICPrescaler = TIM_ICPSC_DIV1,
    .ICFilter = 0x00,

    .SlaveMode = TIM_SlaveMode_Reset,   // 从模式复位
    .TriggerSource = TIM_TS_TI1FP1      // TI1FP1 作为触发源
};

uint32_t freq = 0;

int main(void)
{   
    OLED_Init();
    PWM_Init(&PWM_config);
    IC_Init(&IC_Config);

    OLED_Clear();
    OLED_ShowString(1, 1, "Freq:"); 
    OLED_ShowString(1, 13, "Hz");

    PWM_UpdateFreqDuty(&PWM_config, 1000, 50); // 设置 PWM 占空比50%和频率1KHz

    while(1)
    {
        freq = IC_GetFreq(&IC_Config);   // 获取测量频率
        OLED_ShowNum(1, 7, freq, 5);   // 显示频率 (最大5位数)
        Delay_ms(20);                 // 延时，避免刷屏过快
    }
}
