#ifndef __BSP_HJ12864D_H
#define	__BSP_HJ12864D_H
#include "stm32f10x.h"
#include "stdbool.h"
/* 定义连接的GPIO端口, 只需要修改下面的代码即可改变控制的引脚 */
//#define RS_GPIO_PORT    	GPIOA			              /* GPIO端口 */
//#define RS_GPIO_CLK 	    RCC_APB2Periph_GPIOA		/* GPIO端口时钟 */
//#define RS_GPIO_PIN		  	GPIO_Pin_8			        /* 连接到SCL时钟线的GPIO */

//#define RW_GPIO_PORT    	GPIOC			              /* GPIO端口 */
//#define RW_GPIO_CLK 	    RCC_APB2Periph_GPIOC		/* GPIO端口时钟 */
//#define RW_GPIO_PIN		  	GPIO_Pin_9			        /* 连接到SCL时钟线的GPIO */

//#define E_GPIO_PORT    		GPIOC			              /* GPIO端口 */
//#define E_GPIO_CLK 	    	RCC_APB2Periph_GPIOC		/* GPIO端口时钟 */
//#define E_GPIO_PIN		  	GPIO_Pin_8			        /* 连接到SCL时钟线的GPIO */

//#define CSA_GPIO_PORT    		GPIOC			              /* GPIO端口 */
//#define CSA_GPIO_CLK 	    	RCC_APB2Periph_GPIOC		/* GPIO端口时钟 */
//#define CSA_GPIO_PIN		  	GPIO_Pin_15			        /* 连接到SCL时钟线的GPIO */

//#define CSB_GPIO_PORT    		GPIOC			              /* GPIO端口 */
//#define CSB_GPIO_CLK 	    	RCC_APB2Periph_GPIOC		/* GPIO端口时钟 */
//#define CSB_GPIO_PIN		  	GPIO_Pin_14			        /* 连接到SCL时钟线的GPIO */
///** the macro definition to trigger the led on or off 
//  * 1 - off
//  *0 - on
//  */
//#define ON  0
//#define OFF 1

///* 使用标准的固件库控制IO*/
//#define RS(a)	if (a)	\
//					GPIO_SetBits(RS_GPIO_PORT,RS_GPIO_PIN);\
//					else		\
//					GPIO_ResetBits(RS_GPIO_PORT,RS_GPIO_PIN)

//#define RW(a)	if (a)	\
//					GPIO_SetBits(RW_GPIO_PORT,RW_GPIO_PIN);\
//					else		\
//					GPIO_ResetBits(RW_GPIO_PORT,RW_GPIO_PIN)

//#define E(a)	if (a)	\
//					GPIO_SetBits(E_GPIO_PORT,E_GPIO_PIN);\
//					else		\
//					GPIO_ResetBits(E_GPIO_PORT,E_GPIO_PIN)
//					
//#define CSA(a)	if (a)	\
//					GPIO_SetBits(CSA_GPIO_PORT,CSA_GPIO_PIN);\
//					else		\
//					GPIO_ResetBits(CSA_GPIO_PORT,CSA_GPIO_PIN)
//					
//#define CSB(a)	if (a)	\
//					GPIO_SetBits(CSB_GPIO_PORT,CSB_GPIO_PIN);\
//					else		\
//					GPIO_ResetBits(CSB_GPIO_PORT,CSB_GPIO_PIN)
//					
//#define DATA_IN()  {GPIOC->CRL=0;GPIOC->CRL|=0x88888888;}
//#define DATA_OUT() {GPIOC->CRL=0;GPIOC->CRL|=0x33333333;}

/* 直接操作寄存器的方法控制IO */
/*#define	digitalHi(p,i)		 {p->BSRR=i;}	 //输出为高电平		
#define digitalLo(p,i)		 {p->BRR=i;}	 //输出低电平
#define digitalToggle(p,i) {p->ODR ^=i;} //输出反转状态*/


/* 定义控制IO的宏 */
/*#define LED1_TOGGLE		 digitalToggle(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_OFF		   digitalHi(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_ON			   digitalLo(LED1_GPIO_PORT,LED1_GPIO_PIN)

#define LED2_TOGGLE		 digitalToggle(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED2_OFF		   digitalHi(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED2_ON			   digitalLo(LED2_GPIO_PORT,LED2_GPIO_PIN)

#define LED3_TOGGLE		 digitalToggle(LED3_GPIO_PORT,LED3_GPIO_PIN)
#define LED3_OFF		   digitalHi(LED3_GPIO_PORT,LED3_GPIO_PIN)
#define LED3_ON			   digitalLo(LED3_GPIO_PORT,LED3_GPIO_PIN)*/

extern bool bDisplayFlag;
extern u8 uErrorNum;
extern u16 uErrCount;
extern bool bErrReadFlag;
extern bool bErrorGetFlag;
void HJ12864D_Init(void);
void UIDisplay(void);
void RTCUpdata(void);
void SettingUpDisplay(u8 pos);
#endif 
