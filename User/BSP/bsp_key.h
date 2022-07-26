#ifndef _BSP_KEY_H
#define _BSP_KEY_H
#include "stdbool.h"
#define KEY1 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15);
#define KEY2 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15);
#define KEY3 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14);
#define KEY4 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12);
#define KEY5 GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2);
#define KEY6 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3);
#define KEY7 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4);
#define KEY8 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5);
#define KEY9 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8);
extern unsigned char uUIPageNum;
extern unsigned char	uUILineNum;
extern bool bOfflineModeFlag;
extern bool bFactoryModeFlag;
extern unsigned char uSettingUpFlag;
extern	bool	bAdminPassFlag;
void Key_IO_Init(void);
void KeyCtrl(void);
#endif
