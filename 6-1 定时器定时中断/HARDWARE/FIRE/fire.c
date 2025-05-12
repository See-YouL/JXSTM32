/*
 * 火焰传感器:
 * 传感器链接: https://telesky.yuque.com/bdys8w/01/qyw1o22t6g5e278p
 * 传感器型号: 3针版
 * 注意事项: 当火焰强度超过阈值时,输出低电平,否则输出高电平
 * 传感器连接方法: DO 接 PB14
 */
#include "stm32f10x.h"                  // Device header
#include "stm32f10x_exti.h"

uint16_t FireState = 0; // 火焰传感器状态变量 0:无火焰,1:有火焰  

/*
 * 火焰传感器初始化函数
 * 1. 打开GPIOB时钟和AFIO时钟
 * 2. 配置PB14为上拉输入模式,速度50MHz
 * 3. 配置中断线为PB14
 * 4. 配置中断线为EXTI_Line14,使能中断线,模式选为中断模式,触发模式选为下降沿触发
 * 4. NVIC分组设置为2组即2个抢占优先级,2个响应优先级
 * 5. NVIC通道设置为EXTI15_10,使能NVIC通道,抢占优先级设置为1,响应优先级设置为1
 */
void Fire_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 打开GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // 打开AFIO时钟
	
	// GPIO PB14初始化
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 配置上拉输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; // 配置端口为14
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 配置输出速度50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure); // 完成PB14初始化
	
	// AFIO初始化(中断线配置)
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14); // 配置中断线为PB14
	
	
	// EXTI初始化
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line14; // 中断线选为14
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; // 中断线使能
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; // 模式选为中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // 触发模式选为下降沿触发
	EXTI_Init(&EXTI_InitStructure);
	
	// NVIC初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // NVIC分组设置为2组即2个抢占优先级,2个响应优先级
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; // NVIC通道设置为EXTI15_10
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // 使能NVIC通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 抢占优先级设置为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; // 响应优先级设置为1
	NVIC_Init(&NVIC_InitStructure);
}

/*
 * 火焰传感器状态获取函数
 * 返回值: 0:无火焰,1:有火焰
 */
uint16_t FireState_Get(void)
{
	return FireState;	
}

/*
 * 火焰传感器状态清除函数
 * 清除火焰传感器状态,将状态变量清零
 */
void FireState_Clear(void)
{
	FireState = 0;	
}

/*
 * EXTI15_10中断处理函数
 * 1. 判断中断线是否为EXTI_Line14,如果是,则将火焰状态变量置为1,表示有火焰
 * 2. 清除中断标志位
 */
void EXTI15_10_IRQHandler(void)
{ 
	if (SET == EXTI_GetITStatus(EXTI_Line14))
	{
		FireState = 1;
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
	
}
