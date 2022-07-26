#ifndef _BSP_SETDATA_EEPROM_H
#define _BSP_SETDATA_EEPROM_H
#include "stm32f10x.h"
typedef struct
{
	u16	OverMaxtempSCRSetting;						//��������  SCR
	u16	OverMaxtempMainBordSetting;				//��������  ����
	u16	Language;													//����
	u16	FaultEnable;											//����ʹ��
	u16	FaultJudgeTime;										//�����ж�ʱ��
	u16	SCROnDelay;												//������ʱ
	u16	SCROffDelay;											//�ϵ���ʱ
	u16	FultResetTime;										//���ϸ�λʱ��
	u16 SpecialCtrl;												//�������
}uSetDatas_TypeDef;
typedef struct
{
  u16	ErrorSavedNum;        						//�ѱ���Ĺ���
	u16	ErrorMaxNum;								//����������
	u16	NewErrorPos;										//���¹���λ��
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
