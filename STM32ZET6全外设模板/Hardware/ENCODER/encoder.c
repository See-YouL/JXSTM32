/**
 * @file encoder.c
 * @brief 编码器初始化和获取编码器速度的函数实现
 * @note 该文件包含编码器的初始化函数和获取编码器速度的函数。(PA6, PA7)
 *       编码器使用TIM3作为输入捕获通道，PA6和PA7作为输入引脚。
 *       TIM3配置为向上计数模式，预分频器为0，计数周期为65535。
 *       输入捕获通道1和通道2配置为不反相，滤波器参数为0xF。
 *       编码器模式为TI12，极性为不反相。
 *       获取编码器速度时，读取TIM3的计数器值，并将计数器清零。
 * @version 1.0
 * @date 2025-08-15
 * @author 含生
 */
#include "stm32f10x.h"                  // Device header
#include "stm32f10x_tim.h"

/**
 * @brief 编码器初始化函数
 * @note 该函数配置TIM3作为编码器接口，使用PA6和PA7作为输入通道。
 *       TIM3配置为向上计数模式，预分频器为0，计数周期为65535。
 *       输入捕获通道1和通道2配置为不反相，滤波器参数为0xF。
 *       编码器模式为TI12，极性为不反相。
 * @param None
 * @retval None
 */
void Encoder_Init(void)
{
    // 1. 开启时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //开启TIM3的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //开启GPIOA的时钟
	
    // 2. GPIO配置
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // PA6 PA7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 速度50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
    // 3. 配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式，选择向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1; //计数周期，即ARR的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1; //预分频器，即PSC的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; //重复计数器，高级定时器才会用到
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
    // 4. 配置输入捕获单元
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure); //结构体初始化，避免结构体初值不确定
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; //选择配置定时器通道1
	TIM_ICInitStructure.TIM_ICFilter = 0xF; //输入滤波器参数，可以过滤信号抖动
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; // 极性不反相
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2; //选择配置定时器通道2
	TIM_ICInitStructure.TIM_ICFilter = 0xF; //输入滤波器参数，可以过滤信号抖动
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; // 极性不反相
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
    // 5. 配置编码器接口
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
    // 6. 使能定时器
	TIM_Cmd(TIM3, ENABLE);			//使能TIM3，定时器开始运行

}

/**
 * @brief 获取编码器速度
 * @note 该函数读取TIM3的计数器值，并将计数器清零。
 * @param None
 * @retval 返回编码器速度
 */
int16_t Encoder_Get(void)
{
	// 使用Temp变量作为中继，目的是返回CNT后将其清零
	int16_t Temp;
	Temp = TIM_GetCounter(TIM3); // 读取TIM3_CNT
	TIM_SetCounter(TIM3, 0); // 清零TIM3_CNT
	return Temp;
}
