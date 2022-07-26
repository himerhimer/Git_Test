#include "bsp_key.h"
#include "stm32f10x.h"
#include "bsp_hj12864d.h"
#include "bsp_sd3078.h"
#include "bsp_setdata_eeprom.h"
#include "bsp_UpDownData.h"
#include "bsp_GeneralTim_PWM.h" 
#define	BASICSETKEY	123
#define	SYSTEMSETKEY	543
#define	ADMINKEY	256
#define	EPSETKEY	666
u16	uKeyPressCount;
bool	bKeyPressFlag;
u16 KeyValue;
bool bLongDownFlag;
u16 uKeyOld;
u8	uKeyScanCount;
u8	uKeyScanStep;
u8 uUIPageNum=0;
u8	uUILineNum;
u8 uKeyPosFlag;		//1.基本设置密码 2.系统设置密码 3.EP密码
u16 uKeyPressData;		//密码键入值
u16 uSetDataValuel;		//参数设置暂存位置
u16 uSetDataValuel2;	//参数暂存位置2
u16 uErrorEnableFlag;
bool	bAdminPassFlag=0;
bool bOnlineModeFlag=0;
bool bOfflineModeFlag=0;
bool bFactoryModeFlag=0;
u8 uSettingUpFlag=0xff;//ff无设置  f0设置中		0设置失败		1 设置成功
extern u8 uErrorNum;
extern u16 uErrorOld;
extern bool bStuatesRefresh;
uSetDatas_TypeDef SetdataSend;
void Key_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*开启按键端口的时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD| RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //PB3,PB4,PA15做普通IO,PA13&14用于SWD调试
	//选择按键的引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; 
	// 设置按键的引脚为浮空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	//使用结构体初始化按键
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}
u16 KeyData(void)
{
	u16 data;
	data=KEY9;
	data<<=1;
	data|=KEY8;
	data<<=1;
	data|=KEY7;
	data<<=1;
	data|=KEY6;
	data<<=1;
	data|=KEY5;
	data<<=1;
	data|=KEY4;
	data<<=1;
	data|=KEY3;
	data<<=1;
	data|=KEY2;
	data<<=1;
	data|=KEY1;
	return data;
}
/*void KeyScan(u8 time)
{
	u16 keytemp;
	if(uKeyScanCount>time)
	{
		uKeyScanCount=0;
		keytemp = KeyData();
		if(keytemp !=0x1ff)
		{
			bKeyPressFlag=1;
			if(uKeyOld==keytemp)
			{
				if(uKeyPressCount>=1000)
				{
					uKeyPressCount=900;
					bKeyPressFlag=0;
					bLongDownFlag=1;
					KeyValue	= uKeyOld;
				}
			}
			else
			{
				uKeyOld=keytemp;
				uKeyPressCount=0;
			}
		}
		else if(bKeyPressFlag==1)
		{
			bKeyPressFlag=0;
			if((uKeyPressCount>=20)&&(uKeyPressCount<1000))
			{
				bLongDownFlag=0;
				uKeyPressCount=0;
				KeyValue	=	uKeyOld;
			}
			else 
			{
				uKeyPressCount=0;
			}
		}
		else bLongDownFlag=0;
	}
}*/
void KeyScan(u8 time)
{
	u16 keytemp;
	if(uKeyScanCount>time)
	{
		uKeyScanCount=0;
		keytemp = KeyData();
		if(keytemp !=0x1ff)
		{
			bKeyPressFlag=1;
			if(uKeyOld==keytemp)
			{
				if(uKeyScanStep==1)
				{
					uKeyScanStep=2;
				}
				else if(uKeyScanStep==2)
				{
					uKeyScanStep=0;
					KeyValue	=	uKeyOld;
				}
				if(keytemp==0xfe)
				{
					if(uKeyPressCount>=3000)
					{
						uKeyPressCount=0;
					bKeyPressFlag=0;
					bLongDownFlag=1;
					KeyValue	= uKeyOld|0x8000;
					}
				}
				else if(uKeyPressCount>=1000)
				{
					uKeyPressCount=900;
					bKeyPressFlag=0;
					bLongDownFlag=1;
					KeyValue	= uKeyOld;
				}
			}
			else
			{
				uKeyOld=keytemp;
				uKeyPressCount=0;
				uKeyScanStep=1;
			}
		}
		else if(bKeyPressFlag==1)
		{
			bKeyPressFlag=0;
			bLongDownFlag=0;
			uKeyPressCount=0;
			uKeyOld=0;
		}
	}
}
void KeyCtrl(void)
{
	KeyScan(10);
	if((KeyValue !=0)&&(uSettingUpFlag==0xff))
	{
		switch(uUIPageNum)
		{
			case 0:
				break;
			case 1:
			switch (KeyValue)
			{
				case 0x1fe:		//left up
					if(bOnLineFlag==1)
					{
						uUIPageNum=2;
						uUILineNum=0;
						bOfflineModeFlag=0;
						bFactoryModeFlag=0;
						bOnlineModeFlag=1;
					}
					break;
				case 0x1fb:		//right up
					break;
				case 0x1f7:		//left
					break;
				case 0x1df:		//right
					break;
				case 0x1fd:		//up
					if(uMachineStatusDatas.ErrorListL!=0)
					{
						if(uUILineNum>0)uUILineNum--;
					}
					break;
				case 0x17f:		//down
					if(uMachineStatusDatas.ErrorListL!=0)
					{
						if(uUILineNum<uErrorNum-2)uUILineNum++;
					}
					break;
				case 0x1ef:		//centor
					break;
				case 0x1bf:		//down left
					break;
				case 0x0ff:		//down right
					break;
				case 0x0bf:		//down right+down left
					bOfflineModeFlag=1;
					bFactoryModeFlag=0;
					bOnlineModeFlag=0;
					uUIPageNum=2;
					uUILineNum=0;
					break;
				case 0x80fe:		//left up+down right 3s
					bFactoryModeFlag=1;
					bOfflineModeFlag=0;
					bOnlineModeFlag=0;
					uUIPageNum=2;
					uUILineNum=0;
					break;
			}
				break;
			case 2:
			switch (KeyValue)
			{
				case 0x1fe:		//left up
					switch (uUILineNum)
					{
						case 0:
							uUIPageNum=3;
							uUILineNum=0;
							break;
						case 1:
							uUIPageNum=4;
							uUILineNum=0;
						uErrCount=0;
						bErrReadFlag=0;
							break;
						case 2:
							uUIPageNum=5;
							uUILineNum=0;
						bStuatesRefresh=1;
							break;
						case 3:
							uUIPageNum=6;
							uUILineNum=0;
							break;
						case 4:
							uUIPageNum=7;
							uUILineNum=0;
							break;
					}
					break;
				case 0x1fb:		//right up
					uUIPageNum=1;
					uUILineNum=0;
					break;
				case 0x1f7:		//left
					uUILineNum=0;
					break;
				case 0x1df:		//right
					uUILineNum=4;
					break;
				case 0x1fd:		//up
					if(uUILineNum>0)uUILineNum--;
					else uUILineNum=4;
					break;
				case 0x17f:		//down
					if(uUILineNum<4)uUILineNum++;
					else uUILineNum=0;
					break;
				case 0x1ef:		//centor
					break;
				case 0x1bf:		//down left
					break;
				case 0x0ff:		//down right
					break;
			}
				break;
			case 3:
			switch (KeyValue)
			{
				case 0x1fe:		//left up
					switch (uUILineNum)
					{
						case 0:
							uUIPageNum=10;
							uUILineNum=0;
							uKeyPosFlag=1;
							break;
						case 1:
							uUIPageNum=18;
							uUILineNum=0;
							break;
						case 2:
							uUIPageNum=10;
							uUILineNum=0;
							uKeyPosFlag=2;
							break;
					}
					break;
				case 0x1fb:		//right up
					uUIPageNum=2;
					uUILineNum=0;
					break;
				case 0x1f7:		//left
					uUILineNum=0;
					break;
				case 0x1df:		//right
					uUILineNum=2;
					break;
				case 0x1fd:		//up
					if(uUILineNum>0)uUILineNum--;
					else uUILineNum=2;
					break;
				case 0x17f:		//down
					if(uUILineNum<2)uUILineNum++;
					else uUILineNum=0;
					break;
				case 0x1ef:		//centor
					break;
				case 0x1bf:		//down left
					break;
				case 0x0ff:		//down right
					break;
			}
				break;
			case 4:
			switch (KeyValue)
			{
				case 0x1fe:		//left up
					break;
				case 0x1fb:		//right up
					uUIPageNum=2;
					uUILineNum=0;
					break;
				case 0x1f7:		//left
					if(uErrCount>0)
						{
							uErrCount--;
							bErrReadFlag=0;
							uUILineNum=0;
						}
					break;
				case 0x1df:		//right
				if(uMachineStatusDatas.ErrorExistNum>(uErrCount+1))
						{
							uUILineNum=0;
							uErrCount++;
							bErrReadFlag=0;
						}
					break;
				case 0x1fd:		//up
					if(uUILineNum>0)
						uUILineNum--;
					else 
					{
						if(uErrCount>0)
						{
							uErrCount--;
							bErrReadFlag=0;
						}
					}
					break;
				case 0x17f:		//down
					if(uUILineNum>(uErrorNum-3))
					{
						if(uMachineStatusDatas.ErrorExistNum>(uErrCount+1))
						{
							uUILineNum=0;
							uErrCount++;
							bErrReadFlag=0;
						}
						else 
						{
//							uUILineNum=uErrorNum-2;
							if(uErrorNum>1)uUILineNum=uErrorNum-2;
							else uUILineNum=0;
						}
					}
					else uUILineNum++;
					break;
				case 0x1ef:		//centor
					break;
				case 0x1bf:		//down left
					break;
				case 0x0ff:		//down right
					break;
			}
				break;
			case 5:
				switch (KeyValue)
			{
				case 0x1fe:		//left up
					break;
				case 0x1fb:		//right up
					uUIPageNum=2;
					uUILineNum=0;
					break;
				case 0x1f7:		//left
					uUILineNum=0;
					break;
				case 0x1df:		//right
					uUILineNum=2;
					break;
				case 0x1fd:		//up
					if(uUILineNum>0)uUILineNum--;
					else uUILineNum=2;
					break;
				case 0x17f:		//down
					if(uUILineNum<2)uUILineNum++;
					else uUILineNum=0;
					break;
				case 0x1ef:		//centor
					break;
				case 0x1bf:		//down left
					break;
				case 0x0ff:		//down right
					break;
			}
			bStuatesRefresh=1;
				break;
			case 6:
				switch (KeyValue)
			{
				case 0x1fe:		//left up
					if(uUILineNum==0)
					{
						uUIPageNum=8;
						uUILineNum=0;
					}
					else
					{
						uUIPageNum=9;
						uUILineNum=0;
					}
					break;
				case 0x1fb:		//right up
					uUIPageNum=2;
					uUILineNum=0;
					break;
				case 0x1f7:		//left
					uUILineNum=0;
					break;
				case 0x1df:		//right
					uUILineNum=1;
					break;
				case 0x1fd:		//up
					if(uUILineNum>0)uUILineNum--;
					else uUILineNum=1;
					break;
				case 0x17f:		//down
					if(uUILineNum<1)uUILineNum++;
					else uUILineNum=0;
					break;
				case 0x1ef:		//centor
					break;
				case 0x1bf:		//down left
					break;
				case 0x0ff:		//down right
					break;
			}
				break;
			case 7:
				switch (KeyValue)
			{
				case 0x1fe:		//left up
					break;
				case 0x1fb:		//right up
					uUIPageNum=2;
					uUILineNum=0;
					break;
				case 0x1f7:		//left
					break;
				case 0x1df:		//right
					break;
				case 0x1fd:		//up
					break;
				case 0x17f:		//down
					break;
				case 0x1ef:		//centor
					break;
				case 0x1bf:		//down left
					break;
				case 0x0ff:		//down right
					break;
			}
				break;
			case 8:
				switch (KeyValue)
			{
				case 0x1fe:		//left up
					break;
				case 0x1fb:		//right up
					uUIPageNum=6;
					uUILineNum=0;
					break;
				case 0x1f7:		//left
					break;
				case 0x1df:		//right
					break;
				case 0x1fd:		//up
					if(uUILineNum>0)uUILineNum--;
					break;
				case 0x17f:		//down
					if(uUILineNum<15)uUILineNum++;
					break;
				case 0x1ef:		//centor
					break;
				case 0x1bf:		//down left
					break;
				case 0x0ff:		//down right
					break;
			}
				break;
			case 9:
				switch (KeyValue)
			{
				case 0x1fe:		//left up
					break;
				case 0x1fb:		//right up
					uUIPageNum=6;
					uUILineNum=0;
					break;
				case 0x1f7:		//left
					break;
				case 0x1df:		//right
					break;
				case 0x1fd:		//up
					if(uUILineNum>0)uUILineNum--;
					break;
				case 0x17f:		//down
					if(uUILineNum<14)uUILineNum++;
					break;
				case 0x1ef:		//centor
					break;
				case 0x1bf:		//down left
					break;
				case 0x0ff:		//down right
					break;
			}
				break;
			case 10:
				switch (KeyValue)
			{
				case 0x1fe:		//left up
					switch (uKeyPosFlag)
					{
						case 1:
							if(uKeyPressData==BASICSETKEY)
							{
								uUIPageNum=11;
								uUILineNum=0;
							}
							else
							{
								uSettingUpFlag=0x0f;
							}
							uKeyPressData=0;
							break;
						case 2:
							if(uKeyPressData==SYSTEMSETKEY)
							{
								uUIPageNum=21;
								uUILineNum=0;
								bAdminPassFlag=0;
							}
//							else if(uKeyPressData==ADMINKEY)
//							{
//								uUIPageNum=21;
//								uUILineNum=0;
//								bAdminPassFlag=1;
//							}
							else
							{
								uSettingUpFlag=0x0f;
							}
							uKeyPressData=0;
							break;
						case 3:
							if(uKeyPressData==EPSETKEY)
							{
								uUIPageNum=27;
								uUILineNum=0;
							}
							else
							{
								uSettingUpFlag=0x0f;
							}
							uKeyPressData=0;
							break;
					}
					break;
				case 0x1fb:		//right up
					if(uKeyPosFlag==3)
					{
						uUIPageNum=21;
						uUILineNum=0;
					}
					else 
					{
						uUIPageNum=3;
						uUILineNum=0;
					}
					uKeyPressData=0;
					break;
				case 0x1f7:		//left
					if(uUILineNum>0)uUILineNum--;
					else uUILineNum=2;
					break;
				case 0x1df:		//right
					if(uUILineNum<2)uUILineNum++;
					else uUILineNum=0;
					break;
				case 0x1fd:		//up
					switch(uUILineNum)
					{
						case 0:
							if(uKeyPressData<900)uKeyPressData+=100;
							else uKeyPressData=uKeyPressData%100;
							break;
						case 1:
							if((uKeyPressData%100/10)<9)uKeyPressData+=10;
							else uKeyPressData=((uKeyPressData/100)*100)+(uKeyPressData%10);
							break;
						case 2:
							if((uKeyPressData%10)<9)uKeyPressData++;
							else uKeyPressData=(uKeyPressData/10)*10;
							break;
					}
					break;
				case 0x17f:		//down
					switch(uUILineNum)
					{
						case 0:
							if((uKeyPressData/100)>0)uKeyPressData-=100;
							else uKeyPressData+=900;
							break;
						case 1:
							if((uKeyPressData%100/10)>0)uKeyPressData-=10;
							else uKeyPressData+=90;
							break;
						case 2:
							if((uKeyPressData%10)>0)uKeyPressData--;
							else uKeyPressData+=9;
							break;
					}
					break;
				case 0x1ef:		//centor
					break;
				case 0x1bf:		//down left
					break;
				case 0x0ff:		//down right
					break;
			}
				break;
			case 11:
				switch (KeyValue)
			{
				case 0x1fe:		//left up
					switch(uUILineNum)
					{
						case 0:
							uUIPageNum=12;
							uUILineNum=0;
							if(bOfflineModeFlag==1)		
							{
								uSetDataValuel=OFFLinedata.OverMaxtempSCRSetting;
								uSetDataValuel2=OFFLinedata.OverMaxtempMainBordSetting;
							}
							else if(bFactoryModeFlag==1)
							{
								uSetDataValuel=uFactorydata.OverMaxtempSCRSetting;
								uSetDataValuel2=uFactorydata.OverMaxtempMainBordSetting;
							}
							else if(bOnlineModeFlag==1)
							{
								uSetDataValuel=CurrentSetdata.OverMaxtempSCRSetting;
								uSetDataValuel2=CurrentSetdata.OverMaxtempMainBordSetting;
							}
							break;
						case 1:
							uUIPageNum=13;
							uUILineNum=0;
							if(bOfflineModeFlag==1)
							uSetDataValuel=OFFLinedata.SCROnDelay;
							else if(bFactoryModeFlag==1)
								uSetDataValuel=uFactorydata.SCROnDelay;
							else if(bOnlineModeFlag==1)
								uSetDataValuel=CurrentSetdata.SCROnDelay;
							break;
						case 2:
							uUIPageNum=14;
							uUILineNum=0;
							if(bOfflineModeFlag==1)
							uSetDataValuel=OFFLinedata.SCROffDelay;
							else if(bFactoryModeFlag==1)
								uSetDataValuel=uFactorydata.SCROffDelay;
							else if(bOnlineModeFlag==1)
								uSetDataValuel=CurrentSetdata.SCROffDelay;
							break;
					}
					break;
				case 0x1fb:		//right up
					uUIPageNum=3;
					uUILineNum=0;
					break;
				case 0x1f7:		//left
					uUILineNum=0;
					break;
				case 0x1df:		//right
					uUILineNum=2;
					break;
				case 0x1fd:		//up
					if(uUILineNum>0)uUILineNum--;
					else uUILineNum=2;
					break;
				case 0x17f:		//down
					if(uUILineNum<2)uUILineNum++;
					else uUILineNum=0;
					break;
				case 0x1ef:		//centor
					
					break;
				case 0x1bf:		//down left
					break;
				case 0x0ff:		//down right
					break;
			}
				break;
			case 12:
				switch (KeyValue)
			{
				case 0x1fe:		//left up
//					uUIPageNum=11;
//					uUILineNum=0;
					if(bOfflineModeFlag==1)
					{
						OFFLinedata.OverMaxtempSCRSetting=uSetDataValuel;
						OFFLinedata.OverMaxtempMainBordSetting=uSetDataValuel2;
						uSettingUpFlag=OFFLineSettingData_Save();
					}
					else if(bFactoryModeFlag==1)
					{
						uFactorydata.OverMaxtempSCRSetting=uSetDataValuel;
						uFactorydata.OverMaxtempMainBordSetting=uSetDataValuel2;
						uSettingUpFlag=FactorySettingData_Save();
					}
					else if(bOnlineModeFlag==1)
					{
						SetdataSend.OverMaxtempSCRSetting=uSetDataValuel;
						SetdataSend.OverMaxtempMainBordSetting=uSetDataValuel2;
						bSendCMDFlag=1;
						uSettingUpFlag=0xf0;
					}
					break;
				case 0x1fb:		//right up
					uUIPageNum=11;
					uUILineNum=0;
				uSetDataValuel=0;
					uSetDataValuel2=0;
					break;
				case 0x1f7:		//left
					if(uUILineNum==1)uUILineNum=0;
					else uUILineNum=1;
					break;
				case 0x1df:		//right
					if(uUILineNum==1)uUILineNum=0;
					else uUILineNum=1;
					break;
				case 0x1fd:		//up
					if(uUILineNum==0)
					{
						if(uSetDataValuel<100)uSetDataValuel++;
						else uSetDataValuel=0;
					}
					else 
					{
						if(uSetDataValuel2<100)uSetDataValuel2++;
						else uSetDataValuel2=0;
					}
					break;
				case 0x17f:		//down
					if(uUILineNum==0)
					{
						if(uSetDataValuel>0)uSetDataValuel--;
						else uSetDataValuel=100;
					}
					else 
					{
						if(uSetDataValuel2>0)uSetDataValuel2--;
						else uSetDataValuel2=100;
					}
					break;
				case 0x1ef:		//centor

					break;
				case 0x1bf:		//down left
					break;
				case 0x0ff:		//down right
					break;
			}
				break;
			case 13:
				switch (KeyValue)
			{
				case 0x1fe:		//left up
//					uUIPageNum=11;
//					uUILineNum=0;
					if(bOfflineModeFlag==1)
					{
						OFFLinedata.SCROnDelay=uSetDataValuel;
						uSettingUpFlag=OFFLineSettingData_Save();
					}
					else if(bFactoryModeFlag==1)
					{
						uFactorydata.SCROnDelay=uSetDataValuel;
						uSettingUpFlag=FactorySettingData_Save();
					}
					else if(bOnlineModeFlag==1)
					{
						SetdataSend.SCROnDelay=uSetDataValuel;
						bSendCMDFlag=1;
						uSettingUpFlag=0xf0;
					}
					break;
				case 0x1fb:		//right up
					uUIPageNum=11;
//					uUILineNum=0;
				uSetDataValuel=0;
					break;
				case 0x1f7:		//left
					uSetDataValuel=5000;
					break;
				case 0x1df:		//right
					uSetDataValuel=0;
					break;
				case 0x1fd:		//up
					if(uSetDataValuel<=4900)uSetDataValuel+=100;
					else uSetDataValuel=0;
					break;
				case 0x17f:		//down
					if(uSetDataValuel>=100)uSetDataValuel-=100;
					else uSetDataValuel=5000;
					break;
				case 0x1ef:		//centor

					break;
				case 0x1bf:		//down left
					break;
				case 0x0ff:		//down right
					break;
			}
				break;
			case 14:
				switch (KeyValue)
			{
				case 0x1fe:		//left up
//					uUIPageNum=11;
//					uUILineNum=0;
					if(bOfflineModeFlag==1)
					{
						OFFLinedata.SCROffDelay=uSetDataValuel;
						uSettingUpFlag=OFFLineSettingData_Save();
					}
					else if(bFactoryModeFlag==1)
					{
						uFactorydata.SCROffDelay=uSetDataValuel;
						uSettingUpFlag=FactorySettingData_Save();
					}
					else if(bOnlineModeFlag==1)
					{
						SetdataSend.SCROffDelay=uSetDataValuel;
						bSendCMDFlag=1;
						uSettingUpFlag=0xf0;
					}
					break;
				case 0x1fb:		//right up
					uUIPageNum=11;
					uUILineNum=0;
				uSetDataValuel=0;
					break;
				case 0x1f7:		//left
					uSetDataValuel=5000;
					break;
				case 0x1df:		//right
					uSetDataValuel=0;
					break;
				case 0x1fd:		//up
					if(uSetDataValuel<=4900)uSetDataValuel+=100;
					else uSetDataValuel=0;
					break;
				case 0x17f:		//down
					if(uSetDataValuel>=100)uSetDataValuel-=100;
					else uSetDataValuel=5000;
					break;
				case 0x1ef:		//centor

					break;
				case 0x1bf:		//down left
					break;
				case 0x0ff:		//down right
					break;
			}
				break;
			case 15:
				switch (KeyValue)
			{
				case 0x1fe:		//left up
//					if(bOfflineModeFlag==1)
//					{
//						OFFLinedata.Unbalancedcoefficient=uSetDataValuel;
//						uSettingUpFlag=OFFLineSettingData_Save();
//					}
//					else if(bFactoryModeFlag==1)
//					{
//						uFactorydata.Unbalancedcoefficient=uSetDataValuel;
//						uSettingUpFlag=FactorySettingData_Save();
//					}
//					else if(bOnlineModeFlag==1)
//					{
//						SetdataSend.Unbalancedcoefficient=uSetDataValuel;
//						bSendCMDFlag=1;
//						uSettingUpFlag=0xf0;
//					}
					break;
				case 0x1fb:		//right up
					uUIPageNum=11;
//					uUILineNum=0;
				uSetDataValuel=0;
					break;
				case 0x1f7:		//left
					uSetDataValuel=8;
					break;
				case 0x1df:		//right
					uSetDataValuel=0;
					break;
				case 0x1fd:		//up
					if(uSetDataValuel<50)uSetDataValuel++;
					else uSetDataValuel=0;
					break;
				case 0x17f:		//down
					if(uSetDataValuel>0)uSetDataValuel--;
					else uSetDataValuel=50;
					break;
				case 0x1ef:		//centor

					break;
				case 0x1bf:		//down left
					break;
				case 0x0ff:		//down right
					break;
			}
				break;
			case 16:
				break;
			case 17:	
				switch (KeyValue)
			{
				case 0x1fe:		//left up
//					uUIPageNum=11;
//					uUILineNum=0;
					if(bOfflineModeFlag==1)
					{
						OFFLinedata.OverMaxtempSCRSetting=uSetDataValuel;
						OFFLinedata.OverMaxtempMainBordSetting=uSetDataValuel2;
						uSettingUpFlag=OFFLineSettingData_Save();
					}
					else if(bFactoryModeFlag==1)
					{
						uFactorydata.OverMaxtempSCRSetting=uSetDataValuel;
						uFactorydata.OverMaxtempMainBordSetting=uSetDataValuel2;
						uSettingUpFlag=FactorySettingData_Save();
					}
					else if(bOnlineModeFlag==1)
					{
						SetdataSend.OverMaxtempSCRSetting=uSetDataValuel;
						SetdataSend.OverMaxtempMainBordSetting=uSetDataValuel2;
						bSendCMDFlag=1;
						uSettingUpFlag=0xf0;
					}
					break;
				case 0x1fb:		//right up
					uUIPageNum=11;
					uUILineNum=0;
				uSetDataValuel=0;
					uSetDataValuel2=0;
					break;
				case 0x1f7:		//left
					if(uUILineNum==1)uUILineNum=0;
					else uUILineNum=1;
					break;
				case 0x1df:		//right
					if(uUILineNum==1)uUILineNum=0;
					else uUILineNum=1;
					break;
				case 0x1fd:		//up
					if(uUILineNum==0)
					{
						if(uSetDataValuel<100)uSetDataValuel++;
						else uSetDataValuel=0;
					}
					else 
					{
						if(uSetDataValuel2<100)uSetDataValuel2++;
						else uSetDataValuel2=0;
					}
					break;
				case 0x17f:		//down
					if(uUILineNum==0)
					{
						if(uSetDataValuel>0)uSetDataValuel--;
						else uSetDataValuel=100;
					}
					else 
					{
						if(uSetDataValuel2>0)uSetDataValuel2--;
						else uSetDataValuel2=100;
					}
					break;
				case 0x1ef:		//centor

					break;
				case 0x1bf:		//down left
					break;
				case 0x0ff:		//down right
					break;
			}
				break;
			case 18:
				switch (KeyValue)
			{
				case 0x1fe:		//left up
					if(uUILineNum==0)uUIPageNum=19;
					else if(uUILineNum==1)
					{
						 uUIPageNum=20;
						RTC_ReadDate(&sysTime);
						time_init.year=sysTime.year;
						time_init.month=sysTime.month;
						time_init.day=sysTime.day;
						time_init.hour=sysTime.hour;
						time_init.minute=sysTime.minute;
					}
					else
					{
						uUIPageNum=35;
						uSetDataValuel=uBackLightData;
					}
					uUILineNum=0;
					break;
				case 0x1fb:		//right up
					uUIPageNum=3;
					uUILineNum=0;
					break;
				case 0x1f7:		//left
					uUILineNum=0;
					break;
				case 0x1df:		//right
					uUILineNum=2;
					break;
				case 0x1fd:		//up
					if(uUILineNum>0)uUILineNum--;
					else uUILineNum=2;
					break;
				case 0x17f:		//down
					if(uUILineNum<2)uUILineNum++;
					else uUILineNum=0;
					break;
				case 0x1ef:		//centor
					
					break;
				case 0x1bf:		//down left
					break;
				case 0x0ff:		//down right
					break;
			}
				break;
			case 19:
				switch (KeyValue)
			{
				case 0x1fe:		//left up
					uUIPageNum=18;
					uUILineNum=0;
					break;
				case 0x1fb:		//right up
					uUIPageNum=18;
					uUILineNum=0;
					break;
				case 0x1f7:		//left
					if(uUILineNum==0)uUILineNum=1;
					else uUILineNum=0;
					break;
				case 0x1df:		//right
					if(uUILineNum==0)uUILineNum=1;
					else uUILineNum=0;
					break;
				case 0x1fd:		//up
					if(uUILineNum==0)uUILineNum=1;
					else uUILineNum=0;
					break;
				case 0x17f:		//down
					if(uUILineNum==0)uUILineNum=1;
					else uUILineNum=0;
					break;
				case 0x1ef:		//centor
					
					break;
				case 0x1bf:		//down left
					break;
				case 0x0ff:		//down right
					break;
			}
				break;
			case 20:
				switch (KeyValue)
			{
				case 0x1fe:		//left up
					uSettingUpFlag=RTC_WriteDate(&time_init);
					uUIPageNum=18;
					uUILineNum=0;
					break;
				case 0x1fb:		//right up
					uUIPageNum=18;
					uUILineNum=0;
					break;
				case 0x1f7:		//left
					if(uUILineNum>0)uUILineNum--;
					else uUILineNum=4;
					break;
				case 0x1df:		//right
					if(uUILineNum<4)uUILineNum++;
					else uUILineNum=0;
					break;
				case 0x1fd:		//up
					switch(uUILineNum)
					{
						case 0:
							if(time_init.year<0x99)
							{
								if((time_init.year&0x0f)==9)time_init.year+=7;
								else time_init.year++;
							}
							else time_init.year=0;
							break;
						case 1:
							if(time_init.month<0x12)
							{
								if((time_init.month&0x0f)==9)time_init.month+=7;
								else time_init.month++;
							}
							else time_init.month=0;
							break;
						case 2:
							if(time_init.day<0x31)
							{
								if((time_init.day&0x0f)==9)time_init.day+=7;
								else time_init.day++;
							}
							else time_init.day=0;
							break;
						case 3:
							if(time_init.hour<0x23)
							{
								if((time_init.hour&0x0f)==9)time_init.hour+=7;
								else time_init.hour++;
							}
							else time_init.hour=0;
							break;
						case 4:
							if(time_init.minute<0x59)
							{
								if((time_init.minute&0x0f)==9)time_init.minute+=7;
								else time_init.minute++;
							}
							else time_init.minute=0;
							break;
					}
					break;
				case 0x17f:		//down
					switch(uUILineNum)
					{
						case 0:
							if(time_init.year>0)
							{
								if((time_init.year&0x0f)==0)time_init.year-=7;
								else time_init.year--;
							}
							else time_init.year=0x99;
							break;
						case 1:
							if(time_init.month>0)
							{
								if((time_init.month&0x0f)==0)time_init.month-=7;
								else time_init.month--;
							}
							else time_init.month=0x12;
							break;
						case 2:
							if(time_init.day>0)
							{
								if((time_init.day&0x0f)==0)time_init.day-=7;
								else time_init.day--;
							}
							else time_init.day=0x31;
							break;
						case 3:
							if(time_init.hour>0)
							{
								if((time_init.hour&0x0f)==0)time_init.hour-=7;
								else time_init.hour--;
							}
							else time_init.hour=0x23;
							break;
						case 4:
							if(time_init.minute>0)
							{
								if((time_init.minute&0x0f)==0)time_init.minute-=7;
								else time_init.minute--;
							}
							else time_init.minute=0x59;
							break;
					}
					break;
				case 0x1ef:		//centor
					
					break;
				case 0x1bf:		//down left
					break;
				case 0x0ff:		//down right
					break;
			}
				break;
			case 21:
				switch (KeyValue)
			{
				case 0x1fe:		//left up
					switch (uUILineNum)
					{
						case 0:
							uUIPageNum=22;
						if(bOfflineModeFlag==1)
							uErrorEnableFlag=OFFLinedata.FaultEnable;
						else if(bFactoryModeFlag==1)
							uErrorEnableFlag=uFactorydata.FaultEnable;
						else if(bOnlineModeFlag==1)
								uErrorEnableFlag=CurrentSetdata.FaultEnable;
						uUILineNum=0;
							break;
						case 1:
							uUIPageNum=23;
						if(bOfflineModeFlag==1)
							uSetDataValuel=OFFLinedata.FaultJudgeTime;
						else if(bFactoryModeFlag==1)
							uSetDataValuel=uFactorydata.FaultJudgeTime;
						else if(bOnlineModeFlag==1)
								uSetDataValuel=CurrentSetdata.FaultJudgeTime;
						uUILineNum=0;
							break;
						case 2:
							uUIPageNum=24;
							if(bOfflineModeFlag==1)
							uSetDataValuel=OFFLinedata.FultResetTime;
							else if(bFactoryModeFlag==1)
							uSetDataValuel=uFactorydata.FultResetTime;
							else if(bOnlineModeFlag==1)
								uSetDataValuel=CurrentSetdata.FultResetTime;
							break;
						case 3:
							uUIPageNum=25;
						uUILineNum=0;
						if(bOfflineModeFlag==1)
							uSetDataValuel=OFFLinedata.SpecialCtrl;
						else if(bFactoryModeFlag==1)
							uSetDataValuel=uFactorydata.SpecialCtrl;
						else if(bOnlineModeFlag==1)
								uSetDataValuel=CurrentSetdata.SpecialCtrl;
							break;
						case 4:
							uUIPageNum=33;
						uUILineNum=0;
							break;
						case 5:
							uUIPageNum=34;
						uUILineNum=0;
							break;
						case 6:
							uUIPageNum=26;
							uUILineNum=0;
							break;
						case 7:
							uUIPageNum=10;
							uKeyPosFlag=3;
							uUILineNum=0;
							break;
					}
					break;
				case 0x1fb:		//right up
					uUIPageNum=3;
					uUILineNum=0;
					break;
				case 0x1f7:		//left
					uUILineNum=0;
					break;
				case 0x1df:		//right
					if(bAdminPassFlag==1)uUILineNum=7;
					else uUILineNum=6;
					break;
				case 0x1fd:		//up
					if(bAdminPassFlag==1)
					{
						if(uUILineNum>0)uUILineNum--;
						else uUILineNum=7;
					}
					else 
					{
						if(uUILineNum>0)uUILineNum--;
						else uUILineNum=6;
					}
					break;
				case 0x17f:		//down
					if(bAdminPassFlag==1)
					{
						if(uUILineNum<7)uUILineNum++;
						else uUILineNum=0;
					}
					else
					{
						if(uUILineNum<6)uUILineNum++;
					else uUILineNum=0;
					}
					break;
				case 0x1ef:		//centor
					
					break;
				case 0x1bf:		//down left
					break;
				case 0x0ff:		//down right
					break;
			}
				break;
			case 22:
				switch (KeyValue)
			{
				case 0x1fe:		//left up
//					uUIPageNum=21;
//					uUILineNum=0;
					if(bOfflineModeFlag==1)
					{
						OFFLinedata.FaultEnable=uErrorEnableFlag;
						uSettingUpFlag=OFFLineSettingData_Save();
					}
					else if(bFactoryModeFlag==1)
					{
						uFactorydata.FaultEnable=uErrorEnableFlag;
						uSettingUpFlag=FactorySettingData_Save();
					}
					else if(bOnlineModeFlag==1)
					{
						SetdataSend.FaultEnable=uErrorEnableFlag;
						bSendCMDFlag=1;
						uSettingUpFlag=0xf0;
					}
					break;
				case 0x1fb:		//right up
					uUIPageNum=21;
					uUILineNum=0;
					break;
				case 0x1f7:		//left
					if(uUILineNum==3)
					{
						if(uErrorEnableFlag&0x8000)uErrorEnableFlag=0;
						else uErrorEnableFlag=0x8007;
					}
					else{
						if(uErrorEnableFlag&(1<<uUILineNum))uErrorEnableFlag&=(~(1<<uUILineNum));
						else uErrorEnableFlag|=(1<<uUILineNum);
					}
					break;
				case 0x1df:		//right
					if(uUILineNum==3)
					{
						if(uErrorEnableFlag&0x8000)uErrorEnableFlag=0;
						else uErrorEnableFlag=0x8007;
					}
					else{
						if(uErrorEnableFlag&(1<<uUILineNum))uErrorEnableFlag&=(~(1<<uUILineNum));
						else uErrorEnableFlag|=(1<<uUILineNum);
					}
					break;
				case 0x1fd:		//up
					if(uUILineNum>0)uUILineNum--;
					else uUILineNum=3;
					break;
				case 0x17f:		//down
					if(uUILineNum<3)uUILineNum++;
					else uUILineNum=0;
					break;
				case 0x1ef:		//centor

					break;
				case 0x1bf:		//down left
					break;
				case 0x0ff:		//down right
					break;
			}
				break;
			case 23:
				switch (KeyValue)
			{
				case 0x1fe:		//left up
//					uUIPageNum=21;
//					uUILineNum=0;
					if(bOfflineModeFlag==1)
					{
						OFFLinedata.FaultJudgeTime=uSetDataValuel;
						uSettingUpFlag=OFFLineSettingData_Save();
					}
					else if(bFactoryModeFlag==1)
					{
						uFactorydata.FaultJudgeTime=uSetDataValuel;
						uSettingUpFlag=FactorySettingData_Save();
					}
					else if(bOnlineModeFlag==1)
					{
						SetdataSend.FaultJudgeTime=uSetDataValuel;
						bSendCMDFlag=1;
						uSettingUpFlag=0xf0;
					}
					break;
				case 0x1fb:		//right up
					uUIPageNum=21;
					uUILineNum=0;
				uSetDataValuel=0;
					break;
				case 0x1f7:		//left
					uSetDataValuel=5000;
					break;
				case 0x1df:		//right
					uSetDataValuel=0;
					break;
				case 0x1fd:		//up
					if(uSetDataValuel<=4900)uSetDataValuel+=100;
					else uSetDataValuel=0;
					break;
				case 0x17f:		//down
					if(uSetDataValuel>=100)uSetDataValuel-=100;
					else uSetDataValuel=5000;
					break;
				case 0x1ef:		//centor

					break;
				case 0x1bf:		//down left
					break;
				case 0x0ff:		//down right
					break;
			}
				break;
			case 24:
				switch (KeyValue)
			{
				case 0x1fe:		//left up
//					uUIPageNum=21;
					if(bOfflineModeFlag==1)
					{
						OFFLinedata.FultResetTime=uSetDataValuel;
						uSettingUpFlag=OFFLineSettingData_Save();
					}
					else if(bFactoryModeFlag==1)
					{
						uFactorydata.FultResetTime=uSetDataValuel;
						uSettingUpFlag=FactorySettingData_Save();
					}
					else if(bOnlineModeFlag==1)
					{
						SetdataSend.FultResetTime=uSetDataValuel;
						bSendCMDFlag=1;
						uSettingUpFlag=0xf0;
					}
//					uUILineNum=0;
					break;
				case 0x1fb:		//right up
					uUIPageNum=21;
					uUILineNum=0;
					break;
				case 0x1f7:		//left
					uSetDataValuel=5000;
					break;
				case 0x1df:		//right
					uSetDataValuel=0;
					break;
				case 0x1fd:		//up
					if(uSetDataValuel<=4900)uSetDataValuel+=100;
					else uSetDataValuel=0;
					break;
				case 0x17f:		//down
					if(uSetDataValuel>=100)uSetDataValuel-=100;
					else uSetDataValuel=5000;
					break;
				case 0x1ef:		//centor

					break;
				case 0x1bf:		//down left
					break;
				case 0x0ff:		//down right
					break;
			}
				break;
			case 25:
				switch (KeyValue)
			{
				case 0x1fe:		//left up
//					uUIPageNum=21;
//					uUILineNum=0;
					if(bOfflineModeFlag==1)
					{
						OFFLinedata.SpecialCtrl=uSetDataValuel;
						uSettingUpFlag=OFFLineSettingData_Save();
					}
					else if(bFactoryModeFlag==1)
					{
						uFactorydata.SpecialCtrl=uSetDataValuel;
						uSettingUpFlag=FactorySettingData_Save();
					}
					else if(bOnlineModeFlag==1)
					{
						SetdataSend.SpecialCtrl=uSetDataValuel;
						bSendCMDFlag=1;
						uSettingUpFlag=0xf0;
					}
					break;
				case 0x1fb:		//right up
					uUIPageNum=21;
					uUILineNum=0;
				uSetDataValuel=0;
					break;
				case 0x1f7:		//left
					if(uSetDataValuel&(1<<uUILineNum))uSetDataValuel&=(~(1<<uUILineNum));
					else uSetDataValuel|=(1<<uUILineNum);
					break;
				case 0x1df:		//right
					if(uSetDataValuel&(1<<uUILineNum))uSetDataValuel&=(~(1<<uUILineNum));
					else uSetDataValuel|=(1<<uUILineNum);
					break;
				case 0x1fd:		//up
					if(uUILineNum==1)uUILineNum=0;
				else uUILineNum=1;
					break;
				case 0x17f:		//down
					if(uUILineNum==1)uUILineNum=0;
				else uUILineNum=1;
					break;
				case 0x1ef:		//centor

					break;
				case 0x1bf:		//down left
					break;
				case 0x0ff:		//down right
					break;
			}
				break;
			case 26:
				switch (KeyValue)
			{
				case 0x1fe:		//left up
//					uUIPageNum=21;
//					uUILineNum=0;
					if(bOfflineModeFlag==1)uSettingUpFlag=OfflineDataRecovery(&uFactorydata.OverMaxtempSCRSetting,bAdminPassFlag);
					else if(bFactoryModeFlag==1)uSettingUpFlag=FactoryRecoverFromFactory(bAdminPassFlag);		//空函数
					else if(bOnlineModeFlag==1)
					{
						bSendCMDFlag=1;
						uSettingUpFlag=0xf0;
					}
					break;
				case 0x1fb:		//right up
					uUIPageNum=21;
					uUILineNum=0;
					break;
				case 0x1f7:		//left
					break;
				case 0x1df:		//right
					break;
				case 0x1fd:		//up
					break;
				case 0x17f:		//down
					break;
				case 0x1ef:		//centor

					break;
				case 0x1bf:		//down left
					break;
				case 0x0ff:		//down right
					break;
			}
				break;
			case 27:
				break;
			case 28:
				break;
			case 29:
				break;
			case 30:
				break;
			case 31:
				break;
			case 32:
				break;
			case 33:
				switch (KeyValue)
			{
				case 0x1fe:		//left up
					uUIPageNum=33;
					RTC_ReadDate(&sysTime);
					if(bOfflineModeFlag==1)
					{
						RecoverData_Write(uUILineNum,OFFLinedata);
					}
					else if(bFactoryModeFlag==1)
					{
						RecoverData_Write(uUILineNum,uFactorydata);
					}
					else if(bOnlineModeFlag==1)
					{
						RecoverData_Write(uUILineNum,CurrentSetdata);
					}
					uUILineNum=0;
					break;
				case 0x1fb:		//right up
					uUIPageNum=21;
					uUILineNum=0;
					break;
				case 0x1f7:		//left
					uUILineNum=0;
					break;
				case 0x1df:		//right
					uUILineNum=4;
					break;
				case 0x1fd:		//up
					if(uUILineNum>0)uUILineNum--;
					else uUILineNum=4;
					break;
				case 0x17f:		//down
					if(uUILineNum<4)uUILineNum++;
					else uUILineNum=0;
					break;
				case 0x1ef:		//centor
					
					break;
				case 0x1bf:		//down left
					break;
				case 0x0ff:		//down right
					break;
			}
				break;
			case 34:
				switch (KeyValue)
			{
				case 0x1fe:		//left up
					if(bOfflineModeFlag==1)
					{
						if(RecoverDataPos&(1<<uUILineNum))
						{
							uSettingUpFlag=OfflineDataRecovery(&RecoverData[uUILineNum].OverMaxtempSCRSetting,bAdminPassFlag);
							uSettingUpFlag=OFFLineSettingData_Save();
						}
						else
						{
							uSettingUpFlag=0;
						}
					}
					else if(bFactoryModeFlag==1)
					{
						
					}
					else if(bOnlineModeFlag==1)
					{
						if(RecoverDataPos&(1<<uUILineNum))
						{
							bSendCMDFlag=1;
							uSettingUpFlag=0xf0;
						}
						else uSettingUpFlag=0;
					}
					break;
				case 0x1fb:		//right up
					uUIPageNum=21;
					uUILineNum=0;
					break;
				case 0x1f7:		//left
					uUILineNum=0;
					break;
				case 0x1df:		//right
					uUILineNum=4;
					break;
				case 0x1fd:		//up
					if(uUILineNum>0)uUILineNum--;
					else uUILineNum=4;
					break;
				case 0x17f:		//down
					if(uUILineNum<4)uUILineNum++;
					else uUILineNum=0;
					break;
				case 0x1ef:		//centor
					
					break;
				case 0x1bf:		//down left
					break;
				case 0x0ff:		//down right
					break;
			}
				break;
			case 35:
				switch (KeyValue)
			{
				case 0x1fe:		//left up
					uUIPageNum=18;
					uUILineNum=0;
					uBackLightData=uSetDataValuel;
					uSettingUpFlag=LocalDataSave();
					break;
				case 0x1fb:		//right up
					uUIPageNum=18;
					uUILineNum=0;
				uSetDataValuel=0;
				BL_CTRL(uBackLightData);
					break;
				case 0x1f7:		//left
					uSetDataValuel=0;
					BL_CTRL(uSetDataValuel);
					break;
				case 0x1df:		//right
					uSetDataValuel=9;
					BL_CTRL(uSetDataValuel);
					break;
				case 0x1fd:		//up
					if(uSetDataValuel<9)uSetDataValuel++;
					else uSetDataValuel=0;
					BL_CTRL(uSetDataValuel);
					break;
				case 0x17f:		//down
					if(uSetDataValuel>0)uSetDataValuel--;
					else uSetDataValuel=9;
					BL_CTRL(uSetDataValuel);
					break;
				case 0x1ef:		//centor

					break;
				case 0x1bf:		//down left
					break;
				case 0x0ff:		//down right
					break;
			}
				break;
			case 36:
				break;
		}
		KeyValue=0;
		uErrorOld=0;
		bDisplayFlag=1;
	}
	else KeyValue=0;
}
