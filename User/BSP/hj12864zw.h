#ifndef __HJ12864ZW_H
#define	__HJ12864ZW_H
#include "stm32f10x.h"
/* �������ӵ�GPIO�˿�, ֻ��Ҫ�޸�����Ĵ��뼴�ɸı���Ƶ����� */
#define RS_GPIO_PORT    	GPIOB			              /* GPIO�˿� */
#define RS_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define RS_GPIO_PIN		  	GPIO_Pin_0			        /* ���ӵ�SCLʱ���ߵ�GPIO */

#define RW_GPIO_PORT    	GPIOB			              /* GPIO�˿� */
#define RW_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define RW_GPIO_PIN		  	GPIO_Pin_1			        /* ���ӵ�SCLʱ���ߵ�GPIO */

#define E_GPIO_PORT    		GPIOB			              /* GPIO�˿� */
#define E_GPIO_CLK 	    	RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define E_GPIO_PIN		  	GPIO_Pin_5			        /* ���ӵ�SCLʱ���ߵ�GPIO */

#define CSA_GPIO_PORT    		GPIOB			              /* GPIO�˿� */
#define CSA_GPIO_CLK 	    	RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define CSA_GPIO_PIN		  	GPIO_Pin_5			        /* ���ӵ�SCLʱ���ߵ�GPIO */

#define CSB_GPIO_PORT    		GPIOB			              /* GPIO�˿� */
#define CSB_GPIO_CLK 	    	RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define CSB_GPIO_PIN		  	GPIO_Pin_5			        /* ���ӵ�SCLʱ���ߵ�GPIO */
/** the macro definition to trigger the led on or off 
  * 1 - off
  *0 - on
  */
#define ON  0
#define OFF 1

/* ʹ�ñ�׼�Ĺ̼������IO*/
#define RS(a)	if (a)	\
					GPIO_SetBits(RS_GPIO_PORT,RS_GPIO_PIN);\
					else		\
					GPIO_ResetBits(RS_GPIO_PORT,RS_GPIO_PIN)

#define RW(a)	if (a)	\
					GPIO_SetBits(RW_GPIO_PORT,RW_GPIO_PIN);\
					else		\
					GPIO_ResetBits(RW_GPIO_PORT,RW_GPIO_PIN)

#define E(a)	if (a)	\
					GPIO_SetBits(E_GPIO_PORT,E_GPIO_PIN);\
					else		\
					GPIO_ResetBits(E_GPIO_PORT,E_GPIO_PIN)
					
#define CSA(a)	if (a)	\
					GPIO_SetBits(CSA_GPIO_PORT,CSA_GPIO_PIN);\
					else		\
					GPIO_ResetBits(CSA_GPIO_PORT,CSA_GPIO_PIN)
					
#define CSB(a)	if (a)	\
					GPIO_SetBits(CSB_GPIO_PORT,CSB_GPIO_PIN);\
					else		\
					GPIO_ResetBits(CSB_GPIO_PORT,CSB_GPIO_PIN)


/* ֱ�Ӳ����Ĵ����ķ�������IO */
/*#define	digitalHi(p,i)		 {p->BSRR=i;}	 //���Ϊ�ߵ�ƽ		
#define digitalLo(p,i)		 {p->BRR=i;}	 //����͵�ƽ
#define digitalToggle(p,i) {p->ODR ^=i;} //�����ת״̬*/


/* �������IO�ĺ� */
/*#define LED1_TOGGLE		 digitalToggle(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_OFF		   digitalHi(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_ON			   digitalLo(LED1_GPIO_PORT,LED1_GPIO_PIN)

#define LED2_TOGGLE		 digitalToggle(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED2_OFF		   digitalHi(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED2_ON			   digitalLo(LED2_GPIO_PORT,LED2_GPIO_PIN)

#define LED3_TOGGLE		 digitalToggle(LED3_GPIO_PORT,LED3_GPIO_PIN)
#define LED3_OFF		   digitalHi(LED3_GPIO_PORT,LED3_GPIO_PIN)
#define LED3_ON			   digitalLo(LED3_GPIO_PORT,LED3_GPIO_PIN)*/

#endif 
