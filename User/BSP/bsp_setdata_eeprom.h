#ifndef _BSP_SETDATA_EEPROM_H
#define _BSP_SETDATA_EEPROM_H
#include "stm32f10x.h"
typedef struct
{
	u16	OverMaxtempSCRSetting;						//超温设置  SCR
	u16	OverMaxtempMainBordSetting;				//超温设置  主板
	u16	Language;													//语言
	u16	FaultEnable;											//故障使能
	u16	FaultJudgeTime;										//故障判定时间
	u16	SCROnDelay;												//启动延时
	u16	SCROffDelay;											//断电延时
	u16	FultResetTime;										//故障复位时间
	u16 SpecialCtrl;												//特殊控制
}uSetDatas_TypeDef;
typedef struct
{
  u16	ErrorSavedNum;        						//已保存的故障
	u16	ErrorMaxNum;								//故障总容量
	u16	NewErrorPos;										//最新故障位置
}uError_TypeDef;
extern uError_TypeDef uErrorInf;
extern uSetDatas_TypeDef OFFLinedata;
extern uSetDatas_TypeDef uFactorydata;
extern uSetDatas_TypeDef CurrentSetdata;
extern u8 RecoverDataPos;
extern u8 uCoverTime[5][5];
extern uSetDatas_TypeDef RecoverData[5];
extern u8	uBackLightData;
//unsigned char FactoryData_Read_Save(void);
//unsigned char OFFLineSettingData_Read(void);
unsigned char OFFLineSettingData_Save(void);
u8	OfflineDataRecovery(u16* data2,u8 flag);
u8	FactoryRecoverFromFactory(u8 flag);
u8 FactorySettingData_Save(void);
u8 SavingDataReadOout(void);
u8 RecoverData_Write(u8 pos ,uSetDatas_TypeDef setdata);
void TransmitData(u16* data1,u16* data2,u8 len);
u8 ErrorDataSave(void);
u8 ErrorDataReadOut(u8* data,u8 pos,u8 num);
u8 LocalDataSave(void);
#endif
