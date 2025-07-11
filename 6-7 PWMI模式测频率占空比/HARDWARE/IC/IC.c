#include "stm32f10x.h"

void IC_Init(void)
{
    // 1. 开启时钟, 打开TIM3外设, GPIO外设的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // 开启TIM3时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启GPIOA时钟


    // 2. 配置PWM对应的GPIO
    GPIO_InitTypeDef GPIO_InitStructure; // 定义GPIO初始化结构体
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; // 选择PA6引脚, TIM3_CH1默认映射到PA6
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 设置为上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 设置GPIO速度为50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure); // 初始化GPIOA

    // 3. 选择时基单元时钟源(默认使用内部时钟)
    TIM_InternalClockConfig(TIM3); // 选择内部时钟

    // 4. 配置时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 滤波采样频率一分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1; // 为了防止溢出,ARR的值设置最大值 
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1; // 标准频率为72M/72=1MHz
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // 重复计数器(高级定时器才有的),这里不用,赋值0
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure); // 初始化TIM3时基单元

    // 5. 初始化输入捕获单元
    TIM_ICInitTypeDef TIM_ICInitStructure; // 定义输入捕获初始化结构体
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; // 选择通道1
    TIM_ICInitStructure.TIM_ICFilter = 0xF; // 设置滤波器参数, 0xF为最大值
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; // 设置极性为上升沿触发
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; // 设置分频器为不分频
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // 选择直连通道
    TIM_PWMIConfig(TIM3, &TIM_ICInitStructure); // 配置TIM3的PWMI模式输入捕获

    // 6. 配置TRGI的触发源为TI1FP1
    TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1); // 选择触发源为TI1FP1

    // 7. 配置从模式为Reset
    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset); // 设置从模式为复位模式

    // 8. 启动定时器
    TIM_Cmd(TIM3, ENABLE); // 使能TIM3定时器
}

// 返回测周法测出的频率
uint32_t IC_GetFreq(void)
{
    /*
     * Fc = 72MHz/(PSC+1) = 72MHz/(72-1+1) = 1MHz
     * Fx = Fc/N = 1MHz / N
     */
    return 1000000 / (TIM_GetCapture1(TIM3) + 1); // 返回频率Fx, +1仅为美化数据
}

// 返回测周法测出的占空比
uint32_t IC_GetDuty(void)
{
    /*
     * 占空比 = CCR2的计数值/CCR1的计数值
     * CCR2的值*100则返回0~100的数值,否则返回0~1
     * CCR2和CCR1 + 1是为了美化数据,否则测量频率和实际频率相差1(BUG)
     */
    return (TIM_GetCapture2(TIM3) + 1) * 100 / (TIM_GetCapture1(TIM3) + 1);
}
