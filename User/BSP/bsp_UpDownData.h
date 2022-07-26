#ifndef _BSP_UPDOWNDATA_H
#define _BSP_UPDOWNDATA_H
#include "stdbool.h"
#include "stm32f10x.h"
typedef struct
{
	u16 RunningStatus;								//运行状态
  u16	Voltage_UV;        						//相间电压-UV
	u16	Voltage_VW; 								//相间电压-VW
	u16	Current_U;										//电流-U
	u16	Current_V;										//电流-V
	u16	Current_W;											//电流-W
	int16_t	MainBordTemperature;				//主控温度
	int16_t	SCRTemperature;							//SCR温度
	u16	SoftWareEdition;						//软件版本
	u16	RunningTime;										//运行时间
	u16	ErrorListH;										//故障列表高8位
	u16	ErrorListL;												//故障列表低8位
	u16	ErrorDetail;										//故障说明
	u16	ErrorExistNum;									//已存故障数量
}uMachineStuate_TypeDef;
typedef struct
{
	u8  ErrorTime[5];											//故障发生时间
	u16	ErrorCurrentNum;								//当前故障位置
	u16	ErrorListH;										//故障列表高8位
	u16	ErrorListL;												//故障列表低8位
	u16	ErrorDetail;										//故障说明
}uErrorStuate_TypeDef;
extern uErrorStuate_TypeDef ErrorStuate;
extern uMachineStuate_TypeDef uMachineStatusDatas;
extern bool bOnLineFlag;
extern bool bSendCMDFlag;
extern u16 uOnLineCount;
extern u16 uHeartBeatSendcount;
extern bool	bStatusDataShowFlag;
void HeartBeat(u16 time);
void DataREV(void);
void SendSetUpData(void);
void ErrorListGet(u16 pos);
#endif
