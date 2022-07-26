#include "bsp_UpDownData.h"
#include "bsp_usart_dma.h"
#include "bsp_sd3078.h"
#include "bsp_key.h"
#include "bsp_setdata_eeprom.h"
#include "bsp_hj12864d.h"
u16 uHeartBeatSendcount=300;
u8 SendPos;
u8 SendLen;
bool bOnLineFlag;
bool	bStatusDataShowFlag;
u16 uOnLineCount;
uErrorStuate_TypeDef ErrorStuate;
extern uint8_t SendBuff[SENDBUFF_SIZE];
extern uint8_t RevdBuff[REVBUFF_SIZE];
extern u16	uTopicTimeCount;
extern	uSetDatas_TypeDef SetdataSend;
extern bool bErrorFlag;
enum TitlePos{
	OverMaxtempSCRSettingPos=0x0020,					//超温设置  SCR
	OverMaxtempMainBordSettingPos,			//超温设置  主板
	LanguagePos,												//语言
	FaultEnablePos,										//故障使能
	FaultJudgeTimePos,								//故障判定时间
	SCROnDelayPos,										//启动延时
	SCROffDelayPos,										//断电延时
	FultResetTimePos,									//故障复位时间
	SpecialCtrlPos										//特殊控制
};

uMachineStuate_TypeDef uMachineStatusDatas;
extern	u16  Usart1_Rec_Cnt; 
//u8	uDataSendFlag=0;				//数据发送标志  0xf0 参数发送标志		0x0f 心跳 
/**CRC16 MODBUS**/
u16 crc16l(u8 *ptr,u8 len)        // ptr 数据指针,len 数据长度
{ 
	u16 i,j,tmp,CRC16;
	CRC16=0xffff;
	for (i=0;i<len;i++)
	{  
		CRC16=*ptr^CRC16;
		for (j=0;j< 8;j++)
		 {
			tmp=CRC16 & 0x0001;
			CRC16 =CRC16 >>1;
			if (tmp)
				CRC16=CRC16 ^ 0xa001;    
			}
		ptr++;
	}
	return(CRC16);
}
/*
addr：地址
*buf：数据存储位置
len：数据长度SendBuff[6]
*/
void SendData(u16* buf,u16 addr,u8 cmd,u8 len)		//协议内数据发送
{
	u8 i;
	u16 crc16;
	if(addr>=0x20)
	buf+=addr-0x20;//
	SendBuff[0]=0x5a;
	SendBuff[1]=0xa5;
	SendBuff[2]=len+6;		//长度
	SendBuff[3]=cmd;
	SendBuff[4]=addr>>8;			//地址
	SendBuff[5]=addr;
	SendBuff[6]=len;		//数据长度
	for(i=0;i<(len>>1);i++)
	{
		SendBuff[7+(i<<1)]=(*buf)>>8;
		SendBuff[8+(i<<1)]=*buf;
		buf++;
	}
	crc16=crc16l(SendBuff,len+7);
	SendBuff[len+7]=crc16>>8;
	SendBuff[len+8]=crc16;
	CMD_Send(len+9);
	uHeartBeatSendcount=0;
}
void ErrorListGet(u16 pos)
{
	u16 crc16;
	if(bErrReadFlag==0)
	{
		bErrReadFlag=1;
		SendBuff[0]=0x5a;
		SendBuff[1]=0xa5;
		SendBuff[2]=0x08;
		SendBuff[3]=0x81;
		SendBuff[4]=0x00;
		SendBuff[5]=0x0e;
		SendBuff[6]=0x02;
		SendBuff[7]=pos>>8;
		SendBuff[8]=pos;
		crc16=crc16l(SendBuff,9);
		SendBuff[9]=crc16>>8;
		SendBuff[10]=crc16;
		CMD_Send(11);
		bErrorGetFlag=0;
		uHeartBeatSendcount=0;
	}
}
void HeartBeat(u16 time)
{
	u16 crc;
//	RTC_ReadDate(&sysTime);
	if(uHeartBeatSendcount>time)
	{
		uHeartBeatSendcount=0;
		SendBuff[0]=0x5a;
		SendBuff[1]=0xa5;
		SendBuff[2]=0x0b;
		SendBuff[3]=0x80;
		SendBuff[4]=0;
		SendBuff[5]=0;
		SendBuff[6]=0x05;
		SendBuff[7]=sysTime.year;
		SendBuff[8]=sysTime.month;
		SendBuff[9]=sysTime.day;
		SendBuff[10]=sysTime.hour;
		SendBuff[11]=sysTime.minute;
		crc=crc16l(SendBuff,12);
		SendBuff[12]=crc>>8;
		SendBuff[13]=crc;
		CMD_Send(14);
	}
}
bool bSendCMDFlag;
void SendSetUpData(void)
{
	if(bSendCMDFlag==1)
	{
		bSendCMDFlag=0;
		if(uMachineStatusDatas.RunningStatus==0)
		{
			switch(uUIPageNum)
			{
				case 12:
					SendPos=OverMaxtempSCRSettingPos;
					SendLen=4;
					break;
				case 13:
					SendPos=SCROnDelayPos;
					SendLen=2;
					break;
				case 14:
					SendPos=SCROffDelayPos;
					SendLen=2;
					break;
				case 22:
					SendPos=FaultEnablePos;
					SendLen=2;
					break;
				case 23:
					SendPos=FaultJudgeTimePos;
					SendLen=2;
					break;
				case 24:
					SendPos=FultResetTimePos;
					SendLen=2;
					break;
				case 25:
					SendPos=SpecialCtrlPos;
					SendLen=2;
					break;
				case 26:		//恢复出厂
					SendPos=OverMaxtempSCRSettingPos;
					SendLen=18;
					TransmitData(&SetdataSend.OverMaxtempSCRSetting,&uFactorydata.OverMaxtempSCRSetting,SendLen>>1);
					break;
				case 34:
					SendPos=OverMaxtempSCRSettingPos;
					SendLen=18;
					TransmitData(&SetdataSend.OverMaxtempSCRSetting,&RecoverData[uUILineNum].OverMaxtempSCRSetting,SendLen>>1);
					break;
			}
			SendData(&SetdataSend.OverMaxtempSCRSetting,SendPos,0x83,SendLen);
		}
		else uSettingUpFlag=0xf1;
		bDisplayFlag=1;
		uTopicTimeCount=0;			
	}
}
void DataREV(void)
{
	u16 crc16;
	u8 len,i;
	u16	addr;
	u8 datalen;
	u8	cmdbyte;
	u16 *revdata;
	u16	*senddata;
	if(bREVEndFlag==1)
	{
		bREVEndFlag=0;
		if((RevdBuff[0]==0x5a)&&(RevdBuff[1]==0xa5))
		{
			len=RevdBuff[2]+3;
			if(len==Usart1_Rec_Cnt)
			{
								crc16=RevdBuff[len-2];
				crc16<<=8;
				crc16|=RevdBuff[len-1];
				if(crc16==(crc16l(RevdBuff,len-2)))		//校验通过
				{
					uHeartBeatSendcount=0;
					uOnLineCount=0;
					cmdbyte=RevdBuff[3];
					addr=RevdBuff[4];
					addr<<=8;
					addr|=RevdBuff[5];
					datalen=RevdBuff[6];
					if(cmdbyte==0x83)		//参数
					{
						revdata=&CurrentSetdata.OverMaxtempSCRSetting;
						senddata=&SetdataSend.OverMaxtempSCRSetting;
						if(datalen==0)//应答
						{
							if(uSettingUpFlag==0xf0)
							{
								if(addr==SendPos)
								{
									for(i=0;i<(SendLen>>1);i++)
									{
										revdata[SendPos+i-1]=senddata[SendPos+i-1];
									}
									uSettingUpFlag=1;
									bDisplayFlag=1;
									uTopicTimeCount=0;			
								}
							}
						}
						else
						{
							for(i=0;i<(datalen>>1);i++)
							{
								revdata[addr+i-1]=RevdBuff[(i<<1)+7];
								revdata[addr+i-1]<<=8;
								revdata[addr+i-1]|=RevdBuff[(i<<1)+8];
							}
							SendData(&SetdataSend.OverMaxtempSCRSetting,addr,0x83,0);
						}
					}
					else if(cmdbyte==0x80)		//心跳
					{
						if(datalen==0)//应答
						{
//							if(bOnLineFlag==0)
//							{
//								bOnLineFlag=1;
//								bDisplayFlag=1;
//								uOnLineCount=0;
//								SendData(0xffff,0x83,0);		//获取参数
//							}
//							else  uOnLineCount=0;
						}
					}
					else if(cmdbyte==0x81)		//运行状态
					{
						if(addr==0x000e)	//故障内容
						{
							ErrorStuate.ErrorTime[0]=RevdBuff[7];
							ErrorStuate.ErrorTime[1]=RevdBuff[8];
							ErrorStuate.ErrorTime[2]=RevdBuff[9];
							ErrorStuate.ErrorTime[3]=RevdBuff[10];
							ErrorStuate.ErrorTime[4]=RevdBuff[11];
							ErrorStuate.ErrorCurrentNum=RevdBuff[12];
							ErrorStuate.ErrorCurrentNum<<=8;
							ErrorStuate.ErrorCurrentNum|=RevdBuff[13];
							ErrorStuate.ErrorListH=RevdBuff[14];
							ErrorStuate.ErrorListH<<=8;
							ErrorStuate.ErrorListH|=RevdBuff[15];
							ErrorStuate.ErrorListL=RevdBuff[16];
							ErrorStuate.ErrorListL<<=8;
							ErrorStuate.ErrorListL|=RevdBuff[17];
							ErrorStuate.ErrorDetail=RevdBuff[18];
							ErrorStuate.ErrorDetail<<=8;
							ErrorStuate.ErrorDetail=RevdBuff[19];
							uOnLineCount=0;
							SendData(&SetdataSend.OverMaxtempSCRSetting,addr,0x81,0);
							bErrorGetFlag=1;
							bDisplayFlag=1;
						}
						else
						{
							revdata=&uMachineStatusDatas.RunningStatus;
							for(i=0;i<(datalen>>1);i++)
							{
								revdata[addr+i-1]=RevdBuff[(i<<1)+7];
								revdata[addr+i-1]<<=8;
								revdata[addr+i-1]|=RevdBuff[(i<<1)+8];
							}
							if((uUIPageNum==1)||(uUIPageNum==5))
							{
								bDisplayFlag=1;
								bStatusDataShowFlag=1;
							}
							if(uMachineStatusDatas.ErrorListL!=0)
							{
								bErrorFlag=1;
	//							ErrorDataSave();
							}
							else
							{
								bErrorFlag=0;
							}
							if(bOnLineFlag==0)
							{
								bOnLineFlag=1;
								bDisplayFlag=1;
								uOnLineCount=0;
								SendData(&SetdataSend.OverMaxtempSCRSetting,0xffff,0x83,0);		//获取参数
							}
							else  
							{
								uOnLineCount=0;
								SendData(&SetdataSend.OverMaxtempSCRSetting,addr,0x81,0);
							}
						}
					}
					else if(cmdbyte==0x00)		//错误
					{
						
					}
					else			//未知功能码
					{
						SendData(&SetdataSend.OverMaxtempSCRSetting,0,0x00,0);
					}
				}
				else		//crc校验错误
				{
					SendData(&SetdataSend.OverMaxtempSCRSetting,1,0x00,0);
				}
			}
			else		//长度错误
			{
				SendData(&SetdataSend.OverMaxtempSCRSetting,3,0x00,0);
			}
		}
		else		//帧头错误
		{
			SendData(&SetdataSend.OverMaxtempSCRSetting,2,0x00,0);
		}
	}
}
