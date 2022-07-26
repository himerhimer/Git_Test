#include "bsp_setdata_eeprom.h"
#include "bsp_i2c_ee.h"
#include "bsp_sd3078.h"
#include "bsp_UpDownData.h"
#define SETTINGDATANUM		18//(9*2)
#define	LOCALSETADR					0
#define FACTORYDATABASICADR		10
#define	OFFLINEDATABASICADR		50
#define	SETDATARECOVERBASICADR	100
#define	ERRORDATABASICADR		605
uSetDatas_TypeDef uFactorydata={70,60,0,0,500,500,500,500,0};
uSetDatas_TypeDef OFFLinedata;
uSetDatas_TypeDef CurrentSetdata;

uSetDatas_TypeDef RecoverData[5];

u8 RecoverDataPos=0;
u8 uCoverTime[5][5];
u8	uBackLightData;
uError_TypeDef uErrorInf;
void TransmitData(u16* data1,u16* data2,u8 len)
{
	u8 i;
	for(i=0;i<len;i++)
	{
		data1[i]=data2[i];
	}
}
u8 FactorySettingData_Save(void)
{
	u8 flag;
	flag=ee_WriteOneBytes(0xaa,FACTORYDATABASICADR);
	if(flag==0)goto EEpromErr;
	flag=ee_WriteOneBytes(SETTINGDATANUM,FACTORYDATABASICADR+1);
	if(flag==0)goto EEpromErr;
	flag=ee_WriteBytes((u8 *)&uFactorydata.OverMaxtempSCRSetting,FACTORYDATABASICADR+2,SETTINGDATANUM);
	if(flag==0)goto EEpromErr;
	return 1;
	EEpromErr:
	return 0;
}
u8 FactoryData_Read_Save(void)		//出厂设置参数  10-49
{
	u8 flag;
	u8	basicadrdata; 
	u8 num;
	flag=ee_ReadOneBytes(&basicadrdata,FACTORYDATABASICADR);
	if(flag==0)goto EEpromErr;
	if(basicadrdata==0xaa)
	{
		flag=ee_ReadOneBytes(&num,FACTORYDATABASICADR+1);
		if(flag==0)goto EEpromErr;
		if(num>SETTINGDATANUM)num=SETTINGDATANUM;
		flag=ee_ReadBytes((u8 *)&uFactorydata.OverMaxtempSCRSetting,FACTORYDATABASICADR+2,num);
		if(flag==0)goto EEpromErr;
	}
	else
	{
		flag=ee_WriteOneBytes(0xaa,FACTORYDATABASICADR);
		if(flag==0)goto EEpromErr;
		flag=ee_WriteOneBytes(SETTINGDATANUM,FACTORYDATABASICADR+1);
		if(flag==0)goto EEpromErr;
		flag=ee_WriteBytes((u8 *)&uFactorydata.OverMaxtempSCRSetting,FACTORYDATABASICADR+2,SETTINGDATANUM);
		if(flag==0)goto EEpromErr;
	}
	return 1;
	EEpromErr:
	return 0;
}
u8 OFFLineSettingData_Read(void)
{
	u8 flag;
	u8	basicadrdata; 
	u8 num;
	u8 i;
	u16 *data1;
	u16 *data2;
	flag=ee_ReadOneBytes(&basicadrdata,OFFLINEDATABASICADR);
	if(flag==0)goto EEpromErr;
	if(basicadrdata==0xaa)
	{
		flag=ee_ReadOneBytes(&num,OFFLINEDATABASICADR+1);
		if(flag==0)goto EEpromErr;
		if(num>SETTINGDATANUM)num=SETTINGDATANUM;
		flag=ee_ReadBytes((u8 *)&OFFLinedata.OverMaxtempSCRSetting,OFFLINEDATABASICADR+2,num);
		if(flag==0)goto EEpromErr;
	}
	else
	{
		data1=&OFFLinedata.OverMaxtempSCRSetting;
		data2=&uFactorydata.OverMaxtempSCRSetting;
		for(i=0;i<(SETTINGDATANUM/2);i++)
		{
			data1[i]=data2[i];
		}
	}
	return 1;
	EEpromErr:
	return 0;
}
u8 OFFLineSettingData_Save(void)
{
	u8 flag;
	flag=ee_WriteOneBytes(0xaa,OFFLINEDATABASICADR);
	if(flag==0)goto EEpromErr;
	flag=ee_WriteOneBytes(SETTINGDATANUM,OFFLINEDATABASICADR+1);
	if(flag==0)goto EEpromErr;
	flag=ee_WriteBytes((u8 *)&OFFLinedata.OverMaxtempSCRSetting,OFFLINEDATABASICADR+2,SETTINGDATANUM);
	if(flag==0)goto EEpromErr;
	return 1;
	EEpromErr:
	return 0;
}
u8	OfflineDataRecovery(u16* data2,u8 flag)
{
	u8 len;
	u16 *data1;
	data1=&OFFLinedata.OverMaxtempSCRSetting;
	len=SETTINGDATANUM/2;
	TransmitData(data1,data2,len);
	return (OFFLineSettingData_Save());
}
u8	FactoryRecoverFromFactory(u8 flag)
{
	return 1;
}
u8	RecoverData_Read(void)
{
	u8 flag,i;
	u8	basicadrdata;
	u8 len;
	flag=ee_ReadOneBytes(&basicadrdata,SETDATARECOVERBASICADR);
	if(flag==0)goto EEpromErr;
	if(basicadrdata==0x5a)
	{
		flag=ee_ReadOneBytes(&RecoverDataPos,SETDATARECOVERBASICADR+2);
		if(flag==0)goto EEpromErr;
		
		for(i=0;i<5;i++)
		{
			if(RecoverDataPos&(1<<i))
			{
				flag=ee_ReadBytes(uCoverTime[i],SETDATARECOVERBASICADR+5+(i*50),5);
				if(flag==0)goto EEpromErr;
				flag=ee_ReadOneBytes(&len,SETDATARECOVERBASICADR+10+(i*50));
				if(flag==0)goto EEpromErr;
				if(len>SETTINGDATANUM)len=SETTINGDATANUM;
				flag=ee_ReadBytes((u8 *)&RecoverData[i].OverMaxtempSCRSetting,SETDATARECOVERBASICADR+11+(i*50),len);
				if(flag==0)goto EEpromErr;
			}
		}
	}
	return 1;
	EEpromErr:
	return 0;
}
u8 RecoverData_Write(u8 pos ,uSetDatas_TypeDef setdata)
{
	u8 flag,i;
	u16 * coverdata;
	u16 * data;
	flag=ee_WriteOneBytes(0x5a,SETDATARECOVERBASICADR);
	if(flag==0)goto EEpromErr;
	RecoverDataPos |= 1<<pos;
	flag=ee_WriteOneBytes(RecoverDataPos,SETDATARECOVERBASICADR+2);
	if(flag==0)goto EEpromErr;
	uCoverTime[pos][0]=sysTime.year;
	uCoverTime[pos][1]=sysTime.month;
	uCoverTime[pos][2]=sysTime.day;
	uCoverTime[pos][3]=sysTime.hour;
	uCoverTime[pos][4]=sysTime.minute;
	for(i=0;i<5;i++)
	{
		flag=ee_WriteOneBytes(uCoverTime[pos][i],SETDATARECOVERBASICADR+5+(pos*50)+i);
		if(flag==0)goto EEpromErr;
	}
	flag=ee_WriteOneBytes(SETTINGDATANUM,SETDATARECOVERBASICADR+10+(pos*50));
	if(flag==0)goto EEpromErr;
	coverdata=&RecoverData[pos].OverMaxtempSCRSetting;
	data=&setdata.OverMaxtempSCRSetting;
	TransmitData(coverdata,data,18);
	flag=ee_WriteBytes((u8 *)&setdata.OverMaxtempSCRSetting,SETDATARECOVERBASICADR+11+(pos*50),SETTINGDATANUM);
	if(flag==0)goto EEpromErr;
	return 1;
	EEpromErr:
	return 0;
}
u8 ErrorInfDataReadOut(void)
{
	u8 flag;
	u8	basicadrdata;
	flag=ee_ReadOneBytes(&basicadrdata,ERRORDATABASICADR);
	if(flag==0)goto EEpromErr;
	if(basicadrdata==0x5a)
	{
		flag=ee_ReadBytes((u8*)&uErrorInf.ErrorSavedNum,ERRORDATABASICADR+1,6);
		if(flag==0)goto EEpromErr;
	}
	else
	{
		uErrorInf.ErrorMaxNum=256;
		uErrorInf.ErrorSavedNum=0;
		uErrorInf.NewErrorPos=0;
	}
	return 1;
	EEpromErr:
	return 0;
}
u8 ErrorDataReadOut(u8* data,u8 pos,u8 num)
{
	u8 flag;
	u8 i;
	for(i=0;i<num;i++)
	{
		flag=ee_ReadBytes(data,ERRORDATABASICADR+10+((pos+i)*11),11);
		if(flag==0)goto EEpromErr;
	}
	return 1;
	EEpromErr:
	return 0;
}
u8 ErrorDataSave(void)
{
	u8 flag;
	u8	time[5];
	time[0]=sysTime.year;
	time[1]=sysTime.month;
	time[2]=sysTime.day;
	time[3]=sysTime.hour;
	time[4]=sysTime.minute;
		/**故障引导区修改**/
	if((uErrorInf.NewErrorPos<uErrorInf.ErrorMaxNum-1)&&(uErrorInf.ErrorSavedNum!=0))uErrorInf.NewErrorPos++;
	else uErrorInf.NewErrorPos=0;
	if(uErrorInf.ErrorSavedNum<uErrorInf.ErrorMaxNum)
	uErrorInf.ErrorSavedNum++;
	flag=ee_WriteOneBytes(0x5A,ERRORDATABASICADR);
	if(flag==0)goto EEpromErr;
	flag=ee_WriteBytes((u8 *)&uErrorInf.ErrorSavedNum,ERRORDATABASICADR+1,6);
	if(flag==0)goto EEpromErr;
	
	/**故障信息存储**/
	flag=ee_WriteBytes(time,ERRORDATABASICADR+10+(uErrorInf.NewErrorPos*11),5);
	if(flag==0)goto EEpromErr;
	flag=ee_WriteBytes((u8 *)&uMachineStatusDatas.ErrorListH,ERRORDATABASICADR+15+(uErrorInf.NewErrorPos*11),6);
	if(flag==0)goto EEpromErr;

	return 1;
	EEpromErr:
	return 0;
}	
u8 LocalDataReadOut(void)
{
	u8 flag;
	u8	basicadrdata; 
	flag=ee_ReadOneBytes(&basicadrdata,LOCALSETADR);
	if(flag==0)goto EEpromErr;
	if(basicadrdata==0xaa)
	{
		flag=ee_ReadOneBytes(&uBackLightData,LOCALSETADR+1);
		if(flag==0)goto EEpromErr;
	}
	else
	{
		uBackLightData=9;
	}
	return 1;
	EEpromErr:
	return 0;
}
u8 LocalDataSave(void)
{
	u8 flag;
	flag=ee_WriteOneBytes(0xaa,LOCALSETADR);
	if(flag==0)goto EEpromErr;
	flag=ee_WriteOneBytes(uBackLightData,LOCALSETADR+1);
	if(flag==0)goto EEpromErr;
	return 1;
	EEpromErr:
	return 0;
}
u8 SavingDataReadOout(void)
{
	u8 flag;
	flag=LocalDataReadOut();
	if(flag==0)goto EEpromErr;
	flag=OFFLineSettingData_Read();
	if(flag==0)goto EEpromErr;
	flag=RecoverData_Read();
	if(flag==0)goto EEpromErr;
	flag=FactoryData_Read_Save();
	if(flag==0)goto EEpromErr;
//	flag=ErrorInfDataReadOut();
//	if(flag==0)goto EEpromErr;
	return 1;
	EEpromErr:
	return 0;
}

