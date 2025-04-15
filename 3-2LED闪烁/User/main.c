/*
 * 实验目标：实现LED红灯闪烁，接在PB5，低电平点亮
 */
 
#include "stm32f10x.h"
#include "Delay.h"


int main(void)
{
	// 打开GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	
	// 初始化GPIOB
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; // Pin5
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	while (1)
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_5); // 低电平点亮
		Delay_ms(500); // 500ms延时
		GPIO_SetBits(GPIOB, GPIO_Pin_5); // 高电平熄灭
		Delay_ms(500); // 500ms延时
	}
}
