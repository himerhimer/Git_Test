#include "bsp_led.h"
#include "stm32f10x.h"
#include "bsp_UpDownData.h"
bool bErrorFlag;
void LED_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	LED1(1);
	LED2(0);
	LED3(1);
	LED4(1);
	LED5(1);
	LED6(1);
	LED7(1);
	LED8(1);
	/*开启按键端口的时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);
	//选择按键的引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_11|GPIO_Pin_12; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	//使用结构体初始化按键
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
void LEDCtrl(void)
{
	if(bErrorFlag==1)
	{
		LED3(0);
	}
	else
	{
		LED3(1);
	}
	switch (uMachineStatusDatas.RunningStatus)
		{
			case 0:
				LED7(1);
				LED8(1);
				break;
			case 1:
				LED8(1);
				LED7(0);
				break;
			case 2:
				LED8(0);
				LED7(1);
				break;
		}
}
