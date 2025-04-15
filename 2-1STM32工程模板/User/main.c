#include "stm32f10x.h"                  // Device header

int main(void)
{
	// 打开GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	
	// 初始化GPIOB
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// PB5低电平点亮
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);

	while (1)
	{
		;
	}
}
