#ifndef _BSP_UPDOWNDATA_H
#define _BSP_UPDOWNDATA_H
#include "stdbool.h"
#include "stm32f10x.h"
typedef struct
{
	u16 RunningStatus;								//����״̬
  u16	Voltage_UV;        						//����ѹ-UV
	u16	Voltage_VW; 								//����ѹ-VW
	u16	Current_U;										//����-U
	u16	Current_V;										//����-V
	u16	Current_W;											//����-W
	int16_t	MainBordTemperature;				//�����¶�
	int16_t	SCRTemperature;							//SCR�¶�
	u16	SoftWareEdition;						//����汾
	u16	RunningTime;										//����ʱ��
	u16	ErrorListH;										//�����б��8λ
	u16	ErrorListL;												//�����б��8λ
	u16	ErrorDetail;										//����˵��
	u16	ErrorExistNum;									//�Ѵ��������
}uMachineStuate_TypeDef;
typedef struct
{
	u8  ErrorTime[5];											//���Ϸ���ʱ��
	u16	ErrorCurrentNum;								//��ǰ����λ��
	u16	ErrorListH;										//�����б��8λ
	u16	ErrorListL;												//�����б��8λ
	u16	ErrorDetail;										//����˵��
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
