
// 基本定时器TIMx,x[6,7]定时初始化函数

#include "bsp_TiMbase.h" 
#include "stdbool.h"
#include "bsp_hj12864d.h"
#include "bsp_key.h"
#include "bsp_UpDownData.h"
// 中断优先级配置
static void BASIC_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQ ;	
		// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 *-----------------------------------------------------------------------------
 *typedef struct
 *{ TIM_Prescaler            都有
 *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
 *  TIM_Period               都有
 *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
 *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
 *}TIM_TimeBaseInitTypeDef; 
 *-----------------------------------------------------------------------------
 */


static void BASIC_TIM_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		// 开启定时器时钟,即内部时钟CK_INT=72M
    BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK, ENABLE);
	
		// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Period = BASIC_TIM_Period;	

	  // 时钟预分频数为
    TIM_TimeBaseStructure.TIM_Prescaler= BASIC_TIM_Prescaler;
	
		// 时钟分频因子 ，基本定时器没有，不用管
    //TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		
		// 计数器计数模式，基本定时器只能向上计数，没有计数模式的设置
    //TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
		
		// 重复计数器的值，基本定时器没有，不用管
		//TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
	  // 初始化定时器
    TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStructure);
		
		// 清除计数器中断标志位
    TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);
	  
		// 开启计数器中断
    TIM_ITConfig(BASIC_TIM,TIM_IT_Update,ENABLE);
		
		// 使能计数器
    TIM_Cmd(BASIC_TIM, ENABLE);	
}

void BASIC_TIM_Init(void)
{
	BASIC_TIM_NVIC_Config();
	BASIC_TIM_Mode_Config();
}
extern bool	bKeyPressFlag;
extern u16 uKeyPressCount;
extern	u8	uKeyScanCount;
extern bool bPowOneShowFlag;
u16 uPowOneShowCount;
extern u16 uFeedDogCount;
extern u16 uReadRTCCount;
u16	uTopicTimeCount;
void  BASIC_TIM_IRQHandler (void)	//1ms中断
{
	if ( TIM_GetITStatus( BASIC_TIM, TIM_IT_Update) != RESET ) 
	{	
		uKeyScanCount++;
		uHeartBeatSendcount++;
		uReadRTCCount++;
		if(bKeyPressFlag==1)uKeyPressCount++;
		if(bPowOneShowFlag==1)
		{
			uPowOneShowCount++;
			if(uPowOneShowCount>1500)
			{
				bPowOneShowFlag=0;
				bDisplayFlag=1;
				uUIPageNum=1;
			}
		}
		uFeedDogCount++;
		if((uSettingUpFlag !=0xff))
		{
			uTopicTimeCount++;
			if(uSettingUpFlag==0xf0)
			{
				if(uTopicTimeCount>1500)
				{
					uSettingUpFlag=0x00;
					uTopicTimeCount=0;
					bDisplayFlag=1;
				}				
			}
			else
			{
				if(uTopicTimeCount>600)
				{
					uSettingUpFlag=0xff;
					uTopicTimeCount=0;
					bDisplayFlag=1;
				}				
			}
		}
		if(bOnLineFlag==1)
		{
			uOnLineCount++;
			if(uOnLineCount>500)
			{
				uOnLineCount=0;
				bOnLineFlag=0;
				bDisplayFlag=1;
			}
		}
		TIM_ClearITPendingBit(BASIC_TIM , TIM_FLAG_Update);  		 
	}		 	
}
/*********************************************END OF FILE**********************/
