#ifndef __BSP_GENERALTIME_PWM_H
#define __BSP_GENERALTIME_PWM_H


#include "stm32f10x.h"


/************ͨ�ö�ʱ��TIM�������壬ֻ��TIM2��3��4��5************/
// ��ʹ�ò�ͬ�Ķ�ʱ����ʱ�򣬶�Ӧ��GPIO�ǲ�һ���ģ����Ҫע��

#define            GENERAL_TIM                   TIM4
#define            GENERAL_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            GENERAL_TIM_CLK               RCC_APB1Periph_TIM4
#define            GENERAL_TIM_Period            9
#define            GENERAL_TIM_Prescaler         71

// TIM4 ����Ƚ�ͨ��4
#define            GENERAL_TIM_CH4_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            GENERAL_TIM_CH4_PORT          GPIOB
#define            GENERAL_TIM_CH4_PIN           GPIO_Pin_9

/**************************��������********************************/

void BL_PWM_Init(void);
void	BL_CTRL(u8 data);

#endif	/* __BSP_GENERALTIME_H */


