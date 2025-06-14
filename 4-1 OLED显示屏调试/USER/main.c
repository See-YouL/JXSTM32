/*
 * OLED7针SPI接线:
 * SCK 接 PG12
 * SDA 接 PD5
 * RES 接 PD4
 * DC 接 PD15
 * CS 接 PD1
 */
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"


int main(void)
{
	delay_init();
	OLED_Init();
	OLED_Clear();
	OLED_ShowChar(1, 1, 'A', 12); // 第二个参数为行
	OLED_ShowString(1, 11, "Hello World", 12);
	OLED_ShowNum(1, 21, 12345, 5, 12);
	OLED_Refresh();
	while(1)
	{
		/*
		delay_ms(500);
		OLED_Clear();
		OLED_ShowString(8,16,"ZHONGJINGYUAN",16);
		OLED_ShowString(20,32,"2014/05/01",16);
		OLED_ShowString(0,48,"ASCII:",16);  
		OLED_ShowString(63,48,"CODE:",16);	
		
		OLED_Refresh();	

		delay_ms(500);
		OLED_Clear();
		OLED_ShowString(0,0,"ABC",12);//6*12 “ABC”
	    OLED_ShowString(0,12,"ABC",16);//8*16 “ABC”
		OLED_ShowString(0,28,"ABC",24);//12*24 “ABC”
	    OLED_Refresh();
		*/
		

	}
}

