#include "stm32f10x.h"                  // Device header
#include "stm32f10x_conf.h"

extern uint16_t Num; // 在timer.c中声明跨文件Num变量

// 初始化TIM2
void Timer_Init(void)
{
    // 1. 开启时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // 开启TIM2时钟

    // 2. 选择时基单元时钟(默认使用内部时钟)
    TIM_InternalClockConfig(TIM2); // 选择内部时钟

    // 3. 配置时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 滤波采样频率一分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1; // 在10k的频率下记10000个数需要1s时间
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1; // 对72M进行7200分频得到10kHz
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // 重复计数器(高级定时器才有的),这里不用,赋值0
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure); // 初始化TIM2时基单元

    // 避免复位立刻进入中断
    TIM_ClearFlag(TIM2, TIM_FLAG_Update); // 清除TIM2更新中断标志位

    // 4. 使能更新中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // 使能TIM2更新中断

    // 5. 配置NVIC
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置NVIC分组
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; // 定时器2中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	// 使能NVIC线路
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; // 响应优先级
    NVIC_Init(&NVIC_InitStructure); // 初始化NVIC

    // 6. 使能TIM2计数器
    TIM_Cmd(TIM2, ENABLE); // 使能TIM2计数器

}

// 定时器2中断函数
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) // 判断是否为更新中断
    {
        Num++;
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 清除更新中断标志位
    }
}
