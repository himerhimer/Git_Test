#ifndef __IWDG_H
#define	__IWDG_H


#include "stm32f10x.h"
void IWDG_Init(void);
void IWDG_Feed(u16 time);
#endif /* __IWDG_H */
