#include "stm32f10x.h"
#include "bsp_hj12864d.h"
#include "bsp_spi_flash.h"
#include	"math.h"
#include "bsp_key.h"
#include "bsp_sd3078.h"
#include "bsp_setdata_eeprom.h"
#include "bsp_UpDownData.h"
#define	DISPLAYMODE		0
//�����л澧�Ƽ���ʾģ�飻STM32���̡���˾��ַ www.huijinglcm.com
//12864-0108-M68
//CS�ߵ�ƽ��Чר��
//�˿ڸߵ�ƽ	 OK
#define LCD_RS_H		GPIO_SetBits(GPIOA,GPIO_Pin_8)	//	(��Ӧ��ʾ���ĵ�4��)
#define	LCD_WR_H		GPIO_SetBits(GPIOC,GPIO_Pin_9)	//	
#define	LCD_E_H			GPIO_SetBits(GPIOC,GPIO_Pin_8)	//
#define	LCD_D0_H		GPIO_SetBits(GPIOC,GPIO_Pin_7)	//
#define	LCD_D1_H		GPIO_SetBits(GPIOC,GPIO_Pin_6)	//
#define	LCD_D2_H		GPIO_SetBits(GPIOC,GPIO_Pin_5)	//
#define	LCD_D3_H		GPIO_SetBits(GPIOC,GPIO_Pin_4)	//
#define	LCD_D4_H		GPIO_SetBits(GPIOC,GPIO_Pin_3)	//
#define	LCD_D5_H		GPIO_SetBits(GPIOC,GPIO_Pin_2)	//
#define LCD_D6_H		GPIO_SetBits(GPIOC,GPIO_Pin_1)
#define LCD_D7_H		GPIO_SetBits(GPIOC,GPIO_Pin_0)
#define LCD_CS1_H		GPIO_SetBits(GPIOC,GPIO_Pin_15)	
#define LCD_CS2_H		GPIO_SetBits(GPIOC,GPIO_Pin_14)	//
#define LCD_RST_H		GPIO_SetBits(GPIOC,GPIO_Pin_9)	//	(��Ӧ��ʾ���ĵ�17��)
//vee

//�˿ڵ͵�ƽ
#define LCD_RS_L		GPIO_ResetBits(GPIOA,GPIO_Pin_8)	//	(��Ӧ��ʾ���ĵ�4��)
#define	LCD_WR_L		GPIO_ResetBits(GPIOC,GPIO_Pin_9)	//	
#define	LCD_E_L			GPIO_ResetBits(GPIOC,GPIO_Pin_8)	//
#define	LCD_D0_L		GPIO_ResetBits(GPIOC,GPIO_Pin_7)	//
#define	LCD_D1_L		GPIO_ResetBits(GPIOC,GPIO_Pin_6)	//
#define	LCD_D2_L		GPIO_ResetBits(GPIOC,GPIO_Pin_5)	//
#define	LCD_D3_L		GPIO_ResetBits(GPIOC,GPIO_Pin_4)	//
#define	LCD_D4_L		GPIO_ResetBits(GPIOC,GPIO_Pin_3)	//
#define	LCD_D5_L		GPIO_ResetBits(GPIOC,GPIO_Pin_2)	//
#define LCD_D6_L		GPIO_ResetBits(GPIOC,GPIO_Pin_1)
#define LCD_D7_L		GPIO_ResetBits(GPIOC,GPIO_Pin_0)
#define LCD_CS1_L		GPIO_ResetBits(GPIOC,GPIO_Pin_15)	
#define LCD_CS2_L		GPIO_ResetBits(GPIOC,GPIO_Pin_14)	//
#define LCD_RST_L		GPIO_ResetBits(GPIOC,GPIO_Pin_9)	//	(��Ӧ��ʾ���ĵ�17��)
u8 arrow[]={0x00,0x00,0x00,0x00,0xC0,0xC0,0xC0,0xC0,0xF0,0xE0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x07,0x07,0x07,0x1F,0x0F,0x07,0x03,0x01,0x00,0x00,0x00};/*16*16*/
u8 sheshi[]={0x06,0x09,0x09,0xE6,0xF8,0x0C,0x04,0x02,0x02,0x02,0x02,0x02,0x04,0x1E,0x00,0x00,0x00,0x00,0x00,0x07,0x1F,0x30,0x20,0x40,0x40,0x40,0x40,0x40,0x20,0x10,0x00,0x00};/*16*16*/
u8	ArrowDown[24]={0x00,0x40,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x40,0x00,0x00,0x00,0x01,0x03,0x07,0x0F,0x07,0x03,0x01,0x00,0x00};/*12*12*/
u8 ArrowUp[24]={0x00,0x20,0x30,0x38,0x3C,0x3E,0x3F,0x3E,0x3C,0x38,0x30,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};/*12*12*/
u8 YZK_Logo[720]={
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xE0,0xF8,0xFE,0xFF,0x7F,0x3F,
	0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,
	0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x3F,0xFF,0xFF,0xFF,0xFC,0xF0,0xC0,0x00,0x00,0x00,
	0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xF0,0xFC,
	0xFF,0xFF,0x3F,0x0F,0x07,0x01,0x80,0x80,0x80,0x80,0x80,0xC0,0xC0,0xC0,0xC0,0xC0,
	0xE0,0xE0,0xE0,0xE0,0xE0,0x00,0x00,0xE0,0xF0,0xF8,0xF8,0xF8,0xF8,0xF8,0xFC,0xFC,
	0xFD,0xFC,0xFE,0xFE,0xFE,0xFE,0xFE,0xFF,0x3F,0x8F,0x03,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,
	0x30,0x30,0x30,0x30,0x30,0x30,0x00,0x00,0x00,0xA0,0xB0,0xBC,0xBC,0xF0,0xF0,0xB0,
	0xB0,0xB0,0x80,0xF0,0xF0,0xF0,0x30,0x30,0xF0,0xF0,0xF0,0x00,0x00,0xC0,0xC0,0xF8,
	0xF8,0xF8,0xC0,0x40,0xF0,0xF0,0x30,0xB0,0xB8,0x38,0xB8,0xB0,0x30,0xF0,0xF0,0x00,
	0x80,0xE0,0xF8,0xFE,0xFF,0xFF,0x3F,0x0F,0x03,0x00,0x00,0x01,0x03,0x03,0x07,0x07,
	0x0F,0x1F,0x3F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x07,0xF0,0xFE,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x3F,0x0F,0x07,0x01,0x1C,
	0x7F,0xFF,0xFE,0xF8,0xF0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x06,0x06,0x06,
	0x86,0xE6,0xFE,0x7E,0x3E,0x0E,0x06,0x06,0x06,0xC6,0xC6,0xC6,0x06,0x06,0x06,0x00,
	0x00,0x19,0x19,0xED,0xEF,0xE7,0x63,0x67,0x6D,0x7D,0x79,0x6F,0x6F,0x6F,0x6C,0xEC,
	0xEF,0xEF,0x07,0x00,0x00,0x70,0x70,0xFF,0xFF,0xFF,0x18,0x18,0x6C,0x6E,0x67,0x63,
	0xE1,0xE0,0xE1,0x63,0x67,0x6E,0x6C,0x08,0x07,0x1F,0x3F,0xFF,0xFF,0xFE,0xF8,0xF0,
	0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xFF,0xFF,0xFF,0x7F,
	0x1F,0xC7,0xF1,0xFC,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x3F,0x1F,0x0F,
	0x03,0x01,0x00,0x00,0x00,0x80,0xE0,0xF0,0xFC,0xFF,0xFF,0x7F,0x1F,0x07,0x01,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x1E,0x1F,0x1F,0x0F,0x0D,0x0C,0x0C,0x0C,0x0C,0x0C,
	0x0C,0x0C,0x0F,0x1F,0x1F,0x1C,0x00,0x00,0x00,0x00,0x00,0x3F,0x3F,0x3F,0x33,0x33,
	0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x3F,0x1F,0x0F,0x00,0x00,0x00,0x30,0x30,0x3F,
	0x3F,0x1F,0x00,0x18,0x18,0x18,0x18,0x18,0x1F,0x1F,0x1F,0x18,0x18,0x18,0x18,0x18,
	0x00,0x00,0x00,0x00,0x01,0x07,0x0F,0x3F,0x7F,0xFF,0xFE,0xF8,0xF0,0xE0,0xE0,0xE0,
	0xE0,0xE0,0xE8,0xEE,0xF7,0xFB,0xFD,0xFE,0xFF,0xFF,0xFF,0xFF,0x7F,0x3F,0x1F,0x0F,
	0x07,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xF0,0xFC,0xFE,0xFF,0xFF,0x3F,
	0x0F,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x07,0x7C,0x7C,
	0x07,0x03,0x00,0x3F,0x7F,0x60,0x60,0x7F,0x3F,0x00,0x7F,0x7F,0x0E,0x18,0x7F,0x7F,
	0x00,0x73,0x7B,0x6F,0x67,0x63,0x00,0x7F,0x7F,0x08,0x7F,0x7F,0x00,0x7F,0x7F,0x0C,
	0x1C,0x77,0x63,0x00,0x3E,0x7F,0x63,0x63,0x7F,0x3E,0x00,0x7F,0x7F,0x0F,0x1C,0x7F,
	0x7F,0x00,0x3E,0x7F,0x6B,0x6B,0x7B,0x3A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x01,0x09,0x1B,0x73,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF3,0xF3,
	0xF1,0xF1,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF8,0xFC,0xFE,
	0xFF,0xFF,0x7F,0x1F,0x07,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};/*"D:\0.WORK\1.????\??\???logo-120x48.bmp",0*/



u8 ErrorDisplayContenr[12][11]={
{"��Դȱ��"},
{"����"},
{"˲�����"},
{"���಻ƽ��"},
{"SCR����"},
{"���ư峬��"},
{"SCR����"},
{"��Դ��ѹ"},
{"���ȱ��"},
{"�洢ʧЧ"},
{"�ӵع���"},
{"©�籣��"}
};
//typedef struct
//{
//	u16	ErrorListH;										//�����б��8λ
//	u16	ErrorListL;												//�����б��8λ
//	u16	ErrorDetail;										//����˵��
//}uErrorStuate_TypeDef;
//uErrorStuate_TypeDef  ErrorStuate;
bool bDisplayFlag=1;
extern u8	uUILineNum;
extern u16 uKeyPressData;		//�������ֵ
extern u16 uSetDataValuel;		//���������ݴ�λ��
extern u16 uSetDataValuel2;	//�����ݴ�λ��2
extern u16 uErrorEnableFlag;
bool bPowOneShowFlag=1;
bool bErrorShowedFlag;
u8 uErrorNum;
u16 uErrorOld;
void GPIO_Configuration(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//ʵ��
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//�������
	GPIO_Init(GPIOA,&GPIO_InitStructure);		//
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_15;//
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//�������
	GPIO_Init(GPIOC,&GPIO_InitStructure);	
}
void GPIO_Configuration_in(void)	
{	
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//��������
	GPIO_Init(GPIOC,&GPIO_InitStructure);	
	
}
void delay(u32 nCount)		//1/72΢����ʱ��������
{
	for(;nCount!=0;nCount--);
}

void delay_us(u32 nus)		//΢����ʱ����
{
	 u32 temp;
	 SysTick->LOAD = 9*nus;
	 SysTick->VAL=0X00;//��ռ�����
	 SysTick->CTRL=0X01;//ʹ�ܣ����������޶����������ⲿʱ��Դ
	 do
	 {
		temp=SysTick->CTRL;//��ȡ��ǰ������ֵ
	 }	while((temp&0x01)&&(!(temp&(1<<16))));//�ȴ�ʱ�䵽��
	 
	 SysTick->CTRL=0x00; //�رռ�����
	 SysTick->VAL =0X00; //��ռ�����
}

void delay_ms(u16 nms)		//������ʱ����
{
	 u32 temp;
	 SysTick->LOAD = 9000*nms;
	 SysTick->VAL=0X00;//��ռ�����
	 SysTick->CTRL=0X01;//ʹ�ܣ����������޶����������ⲿʱ��Դ
	 do
	 {
		temp=SysTick->CTRL;//��ȡ��ǰ������ֵ
		#ifdef Stop				
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2)==Bit_SET)	{while(1){delay(0xff); if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2)==Bit_SET) break;}}	//��ͣ
		#endif

	 }	while((temp&0x01)&&(!(temp&(1<<16))));//�ȴ�ʱ�䵽��
	 SysTick->CTRL=0x00; //�رռ�����
	 SysTick->VAL =0X00; //��ռ�����
}
void PORT_Assignment(unsigned char WriteData)//��Ϊ�ֽڵ�λ���ҵģ��õ��������
{
	if (WriteData & 0x01)  LCD_D0_H;	else	LCD_D0_L;
	if (WriteData & 0x02)  LCD_D1_H;    else	LCD_D1_L;
	if (WriteData & 0x04)  LCD_D2_H;    else	LCD_D2_L;
	if (WriteData & 0x08)  LCD_D3_H;    else	LCD_D3_L;
	if (WriteData & 0x10)  LCD_D4_H;    else	LCD_D4_L;
	if (WriteData & 0x20)  LCD_D5_H;    else	LCD_D5_L;
	if (WriteData & 0x40)  LCD_D6_H;   else	LCD_D6_L;
	if (WriteData & 0x80)  LCD_D7_H;   else	LCD_D7_L;
}
/*�ж��Ƿ�Ϊæ?*/
void Busy(void)
{
	u8 temp;
	LCD_CS1_H;
	LCD_CS2_H;
	LCD_RS_L;
	LCD_WR_H;
	GPIO_Configuration_in();//��������
	//P1=0xff;
	while(1)
	{
		LCD_E_H;//delay(1);		//>540nS			
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)==Bit_SET) temp=1;	else	temp=0;		//��״̬��
		LCD_E_L;	delay(10);	//>540nS
		if ((temp&1)==0) 	break; //�ж�æ��־�Ƿ�Ϊ0	
	}	
}

void wrcomL(u8 command)   //д�����ָ��    
{   
   Busy();
   GPIO_Configuration();
	LCD_CS1_H;
    LCD_CS2_L;
	LCD_WR_L;
	LCD_RS_L;
	PORT_Assignment(command);
	LCD_E_H;	delay(10);	//>540nS
	LCD_E_L;	delay(10);	//>540nS

}
void wrcomR(u8 command) //д�Ұ���ָ��
{
  Busy();
  GPIO_Configuration();
  //delay_us(20);
    LCD_CS1_L;
	LCD_CS2_H;

	LCD_WR_L;
	LCD_RS_L;
	PORT_Assignment(command);
	LCD_E_H;	delay(10);	//>540nS
	LCD_E_L;	delay(10);	//>540nS

}
void wrdataL(u8 DATA) //д���������
{
   Busy();
   GPIO_Configuration();
 //delay_us(20);
	LCD_CS1_H;
	LCD_CS2_L;
	LCD_WR_L;
	LCD_RS_H;
	PORT_Assignment(DATA);
	LCD_E_H;	delay(10);	//>540nS
	LCD_E_L;	delay(10);	//>540nS

}
void wrdataR(u8 DATA) //д���������
{
   Busy();
   GPIO_Configuration();
 //delay_us(20);
	LCD_CS1_L;
	LCD_CS2_H;
	LCD_WR_L;
	LCD_RS_H;
	PORT_Assignment(DATA);
	LCD_E_H;	delay(10);	//>540nS
	LCD_E_L;	delay(10);	//>540nS

}
void dispsameone(u8 k)//��һ���ֽ���������ȫ��
{
	u8 i;
	u8 j;
	wrcomL(0xc0);//����C0Ϊ����
	wrcomL(0xb8);//���õ�һҳ
	wrcomL(0x40);//���õ�һ��

	wrcomR(0xc0);//����C0Ϊ����
	wrcomR(0xb8);//���õ�һҳ
	wrcomR(0x40);//���õ�һ��
	for(j=0;j<8;j++)//��д��һ��
	{
		wrcomR(j+0xB8);
		wrcomL(j+0xB8);
		if(j<7)
		{
			for(i=0;i<64;i++)//д��һ��
			{
				wrdataL(k);//ָ���Զ���һ
				wrdataR(k);
			}
		}
		else
		{
			for(i=0;i<64;i++)//д��һ��
			{
				wrdataL(k&0x7f);//ָ���Զ���һ
				wrdataR(k&0x7f);
			}
		}
	}
}
void dispsametow(u8 k,u8 q)//�������������ֽ���������ȫ��
{
	u8 i;
	u8 j;
	wrcomL(0xc0);
	wrcomL(0xb8);
	wrcomL(0x40);

	wrcomR(0xc0);
	wrcomR(0xb8);
	wrcomR(0x40);
	for(j=0;j<8;j++)//��д��һ��
	{
		wrcomR(j+0xB8);
		wrcomL(j+0xB8);
		for(i=0;i<32;i++)//д��һ��
		{
			wrdataL(k);//ָ���Զ���һ
			wrdataL(q);//ָ���Զ���һ
			wrdataR(k);
			wrdataR(q);
		}
	}
}

void Display_One_8x16(u8 hh,u16 page,u16 column,u8 value)
{
	u16 k,n;
	u32 addr;
	u8 data[16];
	if((value>0x00)&&(column<=120))
	{
		if((value>=0x20)&&(value<=0x7e))
		{
			addr=((value-0x20)*16)+0x2000;
			SPI_FLASH_BufferRead(data,addr,16);
			for(n=0;n<2;n++)
			{				
				wrcomL(0xc0);wrcomR(0xc0);
				wrcomL(0xb8+page+n);wrcomR(0xb8+page+n);
				if(column<56)
				{
					wrcomL(0x40+column);
					for(k=0;k<8;k++)
					if((page+n)<7)
					{
						if(hh==0)	wrdataL(data[k+8*n]);	
						else		wrdataL(~(data[k+8*n]));
					}
					else
					{
						if(hh==0)	wrdataL(data[k+8*n]&0x7f);	
						else		wrdataL((~data[k+8*n])&0x7f);
					}
				}
				else if(column<64)
				{
					wrcomL(0x40+column);
					for(k=0;k<64-column;k++)
					if((page+n)<7)
					{
						if(hh==0)	wrdataL(data[k+8*n]);	
						else		wrdataL(~(data[k+8*n]));
					}
					else
					{
						if(hh==0)	wrdataL(data[k+8*n]&0x7f);	
						else		wrdataL((~data[k+8*n])&0x7f);
					}
					
					wrcomR(0x40);
					for(k=64-column;k<8;k++)
					if((page+n)<7)
					{
						if(hh==0)	wrdataR(data[k+8*n]);	
						else		wrdataR(~(data[k+8*n]));
					}
					else
					{
						if(hh==0)	wrdataR(data[k+8*n]&0x7f);	
						else		wrdataR((~data[k+8*n])&0x7f);
					}
				}
				else
				{
					wrcomR(0x40+(column-64));
					for(k=0;k<8;k++)
					if((page+n)<7)
					{
						if(hh==0)	wrdataR(data[k+8*n]);	
						else		wrdataR(~(data[k+8*n]));
					}
					else
					{
						if(hh==0)	wrdataR(data[k+8*n]&0x7f);	
						else		wrdataR((~data[k+8*n])&0x7f);
					}
				}				
			}
		}
	}
}
void display_string_8x16(u8 hh,u16 page,u16 column,u8 *text)
{
	u16 i=0,k,n;
	u32 addr;
	u8 data[16];
	while((text[i]>0x00)&&(column<=120))
	{
		if((text[i]>=0x20)&&(text[i]<=0x7e))
		{
			addr=((text[i]-0x20)*16)+0x2000;
			SPI_FLASH_BufferRead(data,addr,16);
			for(n=0;n<2;n++)
			{				
				wrcomL(0xc0);wrcomR(0xc0);
				wrcomL(0xb8+page+n);wrcomR(0xb8+page+n);
				if(column<56)
				{
					wrcomL(0x40+column);
					for(k=0;k<8;k++)
					if((page+n)<7)
					{
						if(hh==0)	wrdataL(data[k+8*n]);	
						else		wrdataL(~(data[k+8*n]));
					}
					else
					{
						if(hh==0)	wrdataL(data[k+8*n]&0x7f);	
						else		wrdataL((~data[k+8*n])&0x7f);
					}
				}
				else if(column<64)
				{
					wrcomL(0x40+column);
					for(k=0;k<64-column;k++)
					if((page+n)<7)
					{
						if(hh==0)	wrdataL(data[k+8*n]);	
						else		wrdataL(~(data[k+8*n]));
					}
					else
					{
						if(hh==0)	wrdataL(data[k+8*n]&0x7f);	
						else		wrdataL((~data[k+8*n])&0x7f);
					}
					
					wrcomR(0x40);
					for(k=64-column;k<8;k++)
					if((page+n)<7)
					{
						if(hh==0)	wrdataR(data[k+8*n]);	
						else		wrdataR(~(data[k+8*n]));
					}
					else
					{
						if(hh==0)	wrdataR(data[k+8*n]&0x7f);	
						else		wrdataR((~data[k+8*n])&0x7f);
					}
				}
				else
				{
					wrcomR(0x40+(column-64));
					for(k=0;k<8;k++)
					if((page+n)<7)
					{
						if(hh==0)	wrdataR(data[k+8*n]);	
						else		wrdataR(~(data[k+8*n]));
					}
					else
					{
						if(hh==0)	wrdataR(data[k+8*n]&0x7f);	
						else		wrdataR((~data[k+8*n])&0x7f);
					}
				}				
			}
			i++;
			column+=8;
		}
	else
	i++;
	}
}
 

void display_string_5x8(u8 hh,u16 page,u16 column,u8 *text)
{
	u16 i=0,k,n;
	u32	addr;
	u8 data[5];
	while((text[i]>0x00)&&(column<=123))
	{
		if((text[i]>=0x20)&&(text[i]<0x7e))
		{
			addr=((text[i]-0x20)*5)+0x1000;
			SPI_FLASH_BufferRead(data,addr,5);
			for(n=0;n<2;n++)
			{
				wrcomL(0xc0);wrcomR(0xc0);
				wrcomL(0xb8+page+n);wrcomR(0xb8+page+n);
				if(column<59)
				{
					wrcomL(0x40+column);
					for(k=0;k<5;k++)
					{
						if(n==0)
						{
							if(hh==0)wrdataL((data[k])<<4);
							else wrdataL(~(data[k]<<4));
						}
						else 
						{
							if(page+n<7)
							{
								if(hh==0)wrdataL((data[k])>>4);
								else wrdataL(~(data[k]>>4));
							}
							else
							{
								if(hh==0)wrdataL(((data[k])>>4)&0x7f);
								else wrdataL((~(data[k]>>4))&0x7f);
							}
						}
					}
				} 
				else if(column<64)
				{
					wrcomL(0x40+column);
					for(k=0;k<64-column;k++)
					{
						if(n==0)
						{
							if(hh==0)wrdataL((data[k])<<4);
							else wrdataL(~(data[k]<<4));
						}
						else 
						{
							if(page+n<7)
							{
								if(hh==0)wrdataL((data[k])>>4);
								else wrdataL(~(data[k]>>4));
							}
							else
							{
								if(hh==0)wrdataL(((data[k])>>4)&0x7f);
								else wrdataL((~(data[k]>>4))&0x7f);
							}
						}
					}
					
					wrcomR(0x40);
					for(k=64-column;k<5;k++)
					{
						if(n==0)
						{
							if(hh==0)wrdataR((data[k])<<4);
							else wrdataR(~(data[k]<<4));
						}
						else 
						{
							if(page+n<7)
							{
								if(hh==0)wrdataR((data[k])>>4);
								else wrdataR(~(data[k]>>4));
							}
							else
							{
								if(hh==0)wrdataR(((data[k])>>4)&0x7f);
								else wrdataR((~(data[k]>>4))&0x7f);
							}
						}
					}
				}
				else
				{
					wrcomR(0x40+(column-64));
					for(k=0;k<5;k++)
					{
						if(n==0)
						{
							if(hh==0)wrdataR((data[k])<<4);
							else wrdataR(~(data[k]<<4));
						}
						else 
						{
							if(page+n<7)
							{
								if(hh==0)wrdataR((data[k])>>4);
								else wrdataR(~(data[k]>>4));
							}
							else
							{
								if(hh==0)wrdataR(((data[k])>>4)&0x7f);
								else wrdataR((~(data[k]>>4))&0x7f);
							}
						}
					}
				} 
			}
			i++;
			column+=6;
		} 
		else
		i++;
	}
}

//��ʾ128x64 ����߿�
void display_bk(void)
{
	u16 i,j;
	//���
	for(j=0;j<8;j++)
	{
		wrcomL(0xb8+j);
		wrcomL(0x40+0);
		wrdataL(0xff); 
		wrdataL(0xff); 
	}
	//�ҿ�
	for(j=0;j<8;j++)
	{
		wrcomR(0xb8+j);
		wrcomR(0x40+62);
		wrdataR(0xff); 
		wrdataR(0xff); 
	}
	//�Ͽ�
	wrcomL(0xb8+0);wrcomR(0xb8+0);
	wrcomL(0x40+2);wrcomR(0x40+0);	
	for (i=0;i<62;i++)
	{
		wrdataL(0x03); 
		wrdataR(0x03); 
	}
	//�¿�
	wrcomL(0xb8+7);wrcomR(0xb8+7);
	wrcomL(0x40+2);wrcomR(0x40+0);	
	for (i=0;i<62;i++)
	{
		wrdataL(0xC0); 
		wrdataR(0xC0); 
	}
}

//��ʾ16x16 ����ͼ�񡢺��֡���Ƨ�ֻ�16x16 ���������ͼ��
void display_graphic_16x16(u8 hh,u8 page,u8 column,u8 *dp )
{
	u16 i,j;
	for(j=0;j<2;j++)
	{
		wrcomL(0xC0);wrcomR(0xC0);//����
		wrcomL(0xb8+page+j);wrcomR(0xb8+page+j);
		if(column<48)
		{
			wrcomL(0x40+column);
			for (i=0;i<16;i++)
			{
				if(page+j<7)
				{
					if(hh==0) wrdataL(*dp);
					else wrdataL(~(*dp)); //д���ݵ�LCD,ÿд��һ��8 λ�����ݺ��е�ַ�Զ���1
				}
				else
				{
					if(hh==0) wrdataL((*dp)&0x7f);
					else wrdataL((~(*dp))&0x7f);
				}
				
				dp++;
			}
		}
		else if(column<64)
		{
			wrcomL(0x40+column);
			for (i=0;i<(64-column);i++)
			{
				if(page+j<7)
				{
					if(hh==0) wrdataL(*dp);
					else wrdataL(~(*dp)); //д���ݵ�LCD,ÿд��һ��8 λ�����ݺ��е�ַ�Զ���1
				}
				else
				{
					if(hh==0) wrdataL((*dp)&0x7f);
					else wrdataL((~(*dp))&0x7f);
				}
				dp++;
			}
			wrcomR(0x40);
			for (i=0;i<(column-48);i++)
			{
				if(page+j<7)
				{
					if(hh==0) wrdataR(*dp);
					else wrdataR(~(*dp)); //д���ݵ�LCD,ÿд��һ��8 λ�����ݺ��е�ַ�Զ���1
				}
				else
				{
					if(hh==0) wrdataR((*dp)&0x7f);
					else wrdataR((~(*dp))&0x7f);
				}
				dp++;
			}
		}
		else
		{
			wrcomR(0x40+(column-64));
			for (i=0;i<16;i++)
			{
				if(page+j<7)
				{
					if(hh==0) wrdataR(*dp);
					else wrdataR(~(*dp)); //д���ݵ�LCD,ÿд��һ��8 λ�����ݺ��е�ַ�Զ���1
				}
				else
				{
					if(hh==0) wrdataR((*dp)&0x7f);
					else wrdataR((~(*dp))&0x7f);
				}
				dp++;
			}
		}			
	}
}
void display_graphic_12x12(u8 hh,u8 page,u8 column,u8 *dp )
{
	u16 i,j;
	for(j=0;j<2;j++)
	{
		wrcomL(0xC0);wrcomR(0xC0);//����
		wrcomL(0xb8+page+j);wrcomR(0xb8+page+j);
		if(column<52)
		{
			wrcomL(0x40+column);
			for (i=0;i<12;i++)
			{
				if(page+j<7)
				{
					if(hh==0) wrdataL(*dp);
					else wrdataL(~(*dp)); //д���ݵ�LCD,ÿд��һ��8 λ�����ݺ��е�ַ�Զ���1
				}
				else
				{
					if(hh==0) wrdataL((*dp)&0x7f);
					else wrdataL((~(*dp))&0x7f);
				}
				dp++;
			}
		}
		else if(column<64)
		{
			wrcomL(0x40+column);
			for (i=0;i<(64-column);i++)
			{
				if(page+j<7)
				{
					if(hh==0) wrdataL(*dp);
					else wrdataL(~(*dp)); //д���ݵ�LCD,ÿд��һ��8 λ�����ݺ��е�ַ�Զ���1
				}
				else
				{
					if(hh==0) wrdataL((*dp)&0x7f);
					else wrdataL((~(*dp))&0x7f);
				}
				dp++;
			}
			wrcomR(0x40);
			for (i=0;i<(column-52);i++)
			{
				if(page+j<7)
				{
					if(hh==0) wrdataR(*dp);
					else wrdataR(~(*dp)); //д���ݵ�LCD,ÿд��һ��8 λ�����ݺ��е�ַ�Զ���1
				}
				else
				{
					if(hh==0) wrdataR((*dp)&0x7f);
					else wrdataR((~(*dp))&0x7f);
				}
				dp++;
			}
		}
		else
		{
			wrcomR(0x40+(column-64));
			for (i=0;i<12;i++)
			{
				if(page+j<7)
				{
					if(hh==0) wrdataR(*dp);
					else wrdataR(~(*dp)); //д���ݵ�LCD,ÿд��һ��8 λ�����ݺ��е�ַ�Զ���1
				}
				else
				{
					if(hh==0) wrdataR((*dp)&0x7f);
					else wrdataR((~(*dp))&0x7f);
				}
				dp++;
			}
		}			
	}
}
void Display_GBK12X12(u8 hh,u16 page,u16 column, u8 *text)
{
	u32 addr;
	u8 i=0;
	u8 data[24];
	while((text[i]>0x00)&&(column<=120))
	{
		if((text[i]>0xa0)&&(text[i+1]>0xa0))
		{
			addr=(text[i]-0xb0)*94+text[i+1]-0xa1;
			addr*=24;
			addr+=0x3000;
			SPI_FLASH_BufferRead(data,addr,24);
			display_graphic_12x12(hh,page,column,data);
			column+=13;
		}
		i+=2;
	}
}
/*void	Display_Graphic_1616(u8 hh,u8 x,u8 y,u8 *dp)
{
	u8 page,first, i,j;
	if(y>=48)
	{
		first=0xc0;
		page=6;
	}
	else
	{
		page=(y/8)+1;
		first=8-(y%8);
		first |= 0xc0;
	}
	for(j=0;j<2;j++)
	{
		wrcomL(first);wrcomR(first);//����
		wrcomL(0xb8+page+j);wrcomR(0xb8+page+j);
		if(x<48)
		{
			wrcomL(0x40+x);
			for (i=0;i<16;i++)
			{
				if(hh==0) wrdataL(*dp);
				else wrdataL(~(*dp)); //д���ݵ�LCD,ÿд��һ��8 λ�����ݺ��е�ַ�Զ���1
				dp++;
			}
		}
		else if(x<64)
		{
			wrcomL(0x40+x);
			for (i=0;i<(64-x);i++)
			{
				if(hh==0) wrdataL(*dp);
				else wrdataL(~(*dp)); //д���ݵ�LCD,ÿд��һ��8 λ�����ݺ��е�ַ�Զ���1
				dp++;
			}
			wrcomR(0x40);
			for (i=0;i<(x-48);i++)
			{
				if(hh==0) wrdataR(*dp);
				else wrdataR(~(*dp)); //д���ݵ�LCD,ÿд��һ��8 λ�����ݺ��е�ַ�Զ���1
				dp++;
			}
		}
		else
		{
			wrcomR(0x40+(x-64));
			for (i=0;i<16;i++)
			{
				if(hh==0) wrdataR(*dp);
				else wrdataR(~(*dp)); //д���ݵ�LCD,ÿд��һ��8 λ�����ݺ��е�ַ�Զ���1
				dp++;
			}
		}			
	}
}*/
void	Display_Graphic_XXX(u8 hh,u8 x,u8 page,u8 gx,u8 gy,u8 *dp)
{
	u8 i,j,pagenum;
	if((gy%8)==0)pagenum=gy/8;
	else	pagenum=(gy/8)+1;
	for(j=0;j<pagenum;j++)
	{
		wrcomL(0xc0);wrcomR(0xc0);//����
		wrcomL(0xb8+page+j);wrcomR(0xb8+page+j);
		if((x+gx)<64)
		{
			wrcomL(0x40+x);
			for (i=0;i<gx;i++)
			{
				if(hh==0) wrdataL(*dp);
				else wrdataL(~(*dp)); //д���ݵ�LCD,ÿд��һ��8 λ�����ݺ��е�ַ�Զ���1
				dp++;
			}
		}
		else if(x<64)
		{
			wrcomL(0x40+x);
			for (i=0;i<(16-(x-48));i++)
			{
				if(hh==0) wrdataL(*dp);
				else wrdataL(~(*dp)); //д���ݵ�LCD,ÿд��һ��8 λ�����ݺ��е�ַ�Զ���1
				dp++;
			}
			wrcomR(0x40);
			for (i=0;i<(gx+x-64);i++)
			{
				if(hh==0) wrdataR(*dp);
				else wrdataR(~(*dp)); //д���ݵ�LCD,ÿд��һ��8 λ�����ݺ��е�ַ�Զ���1
				dp++;
			}
		}
		else
		{
			wrcomR(0x40+(x-64));
			for (i=0;i<gx;i++)
			{
				if(hh==0) wrdataR(*dp);
				else wrdataR(~(*dp)); //д���ݵ�LCD,ÿд��һ��8 λ�����ݺ��е�ַ�Զ���1
				dp++;
			}
		}			
	}
}
void	Display_Fill_XXX(u8 hh,u8 x,u8 y,u8 gx,u8 gy)
{
	u8 page,first, i,j,pagenum;
	if((gy%8)==0)pagenum=gy/8;
	else	pagenum=(gy/8)+1;
	if((y+gy)>=64)
	{
		first=0xc0;
		page=6;
	}
	else
	{
		page=(y/8)+1;
		first=8-(y%8);
		first |= 0xc0;
	}
	for(j=0;j<pagenum;j++)
	{
		wrcomL(first);wrcomR(first);//����
		wrcomL(0xb8+page+j);wrcomR(0xb8+page+j);
		if((x+gx)<64)
		{
			wrcomL(0x40+x);
			for (i=0;i<gx;i++)
			{
				if(hh==1) wrdataL(0xff);
				else wrdataL(0); //д���ݵ�LCD,ÿд��һ��8 λ�����ݺ��е�ַ�Զ���1
			}
		}
		else if(x<64)
		{
			wrcomL(0x40+x);
			for (i=0;i<(16-(x-48));i++)
			{
				if(hh==1) wrdataL(0xff);
				else wrdataL(0); //д���ݵ�LCD,ÿд��һ��8 λ�����ݺ��е�ַ�Զ���1
			}
			wrcomR(0x40);
			for (i=0;i<(gx+x-64);i++)
			{
				if(hh==1) wrdataR(0xff);
				else wrdataR(0); //д���ݵ�LCD,ÿд��һ��8 λ�����ݺ��е�ַ�Զ���1
			}
		}
		else
		{
			wrcomR(0x40+(x-64));
			for (i=0;i<gx;i++)
			{
				if(hh==0) wrdataR(0xff);
				else wrdataR(1); //д���ݵ�LCD,ÿд��һ��8 λ�����ݺ��е�ַ�Զ���1
			}
		}			
	}
}
void DisplayString(u8 hh,u8 page,u8 column,u8 *text)//��ʾ12*12���֣�8*16Ӣ���ַ����
{
	u16 i=0,k,n;
	u32 addr;
	u8 data[24];
	while((text[i]>0x00)&&(column<=120))
	{
		if((text[i]>=0x20)&&(text[i]<=0x7e))
		{
			addr=((text[i]-0x20)*16)+0x2000;
			SPI_FLASH_BufferRead(data,addr,16);
			for(n=0;n<2;n++)
			{				
				wrcomL(0xc0);wrcomR(0xc0);
				wrcomL(0xb8+page+n);wrcomR(0xb8+page+n);
				if(column<56)
				{
					wrcomL(0x40+column);
					for(k=0;k<8;k++)
					if(page+n<7)
					{
						if(hh==0)	wrdataL(data[k+8*n]);	
						else		wrdataL(~(data[k+8*n]));		
					}
					else
					{
						if(hh==0)	wrdataL((data[k+8*n])&0x7f);	
						else		wrdataL((~(data[k+8*n]))&0x7f);		
					}
				}
				else if(column<64)
				{
					wrcomL(0x40+column);
					for(k=0;k<64-column;k++)
					if(page+n<7)
					{
						if(hh==0)	wrdataL(data[k+8*n]);	
						else		wrdataL(~(data[k+8*n]));		
					}
					else
					{
						if(hh==0)	wrdataL((data[k+8*n])&0x7f);	
						else		wrdataL((~(data[k+8*n]))&0x7f);		
					}
					
					wrcomR(0x40);
					for(k=64-column;k<8;k++)
					if(page+n<7)
					{
						if(hh==0)	wrdataR(data[k+8*n]);	
						else		wrdataR(~(data[k+8*n]));		
					}
					else
					{
						if(hh==0)	wrdataR((data[k+8*n])&0x7f);	
						else		wrdataR((~(data[k+8*n]))&0x7f);		
					}
				}
				else
				{
					wrcomR(0x40+(column-64));
					for(k=0;k<8;k++)
					if(page+n<7)
					{
						if(hh==0)	wrdataR(data[k+8*n]);	
						else		wrdataR(~(data[k+8*n]));		
					}
					else
					{
						if(hh==0)	wrdataR((data[k+8*n])&0x7f);	
						else		wrdataR((~(data[k+8*n]))&0x7f);		
					}
				}				
			}
			i++;
			column+=8;
		}
		else if((text[i]>0xa0)&&(text[i+1]>0xa0))
		{
			addr=(text[i]-0xb0)*94+text[i+1]-0xa1;
			addr*=24;
			addr+=0x3000;
			SPI_FLASH_BufferRead(data,addr,24);
			display_graphic_12x12(hh,page,column,data);
			column+=13;
			i+=2;
		}
	}
}
void lcdint(void)
{
	LCD_RST_L;
	delay_ms(15);
	LCD_RST_H;
	wrcomR(0x3e);delay_ms(1);//��ʾ��
	wrcomR(0x3f);delay_ms(1);//��ʾ��
	wrcomL(0x3e);delay_ms(1);
	wrcomL(0x3f);delay_ms(1);
	wrcomL(0xC0);delay_ms(1);//����
	wrcomR(0xC0);delay_ms(1);//����
}
void WindowClear(u8 flag)
{
	if(flag==0)dispsameone(0x00);
	else dispsameone(0xff);
}
void HJ12864D_Init(void)
{
	GPIO_Configuration();	//��Ƭ���˿�����
	lcdint();
	WindowClear(DISPLAYMODE);
}
u8	DECToText(u8 *data,u16 num)
{
	u8 len;
	if(num >=1000)
	{
		data[0]=(num/1000)+0x30;
		data[1]=((num%1000)/100)+0x30;
		data[2]=(num%1000%100/10)+0x30;
		data[3]=(num%10)+0x30;
		data[4]='\0';
		len=4;
	}
	else if(num>=100)
	{
		data[0]=(num/100)+0x30;
		data[1]=(num%100/10)+0x30;
		data[2]=(num%10)+0x30;
		data[3]='\0';
		len=3;
	}
	else if(num>=10)
	{
		data[0]=(num/10)+0x30;
		data[1]=(num%10)+0x30;
		data[2]='\0';
		len=2;
	}
	else
	{
		data[0]=num+0x30;
		data[1]='\0';
		len=1;
	}
	return len;
}
void	DECToTextRight(u8 *data,int16_t num)
{
	if(num>=0)
	{
		if(num >=1000)
		{
			data[0]=(num/1000)+0x30;
			data[1]=((num%1000)/100)+0x30;
			data[2]=(num%1000%100/10)+0x30;
			data[3]=(num%10)+0x30;
			data[4]='\0';
		}
		else if(num>=100)
		{
			data[0]=' ';
			data[1]=(num/100)+0x30;
			data[2]=(num%100/10)+0x30;
			data[3]=(num%10)+0x30;
			data[4]='\0';
		}
		else if(num>=10)
		{
			data[0]=' ';
			data[1]=' ';
			data[2]=(num/10)+0x30;
			data[3]=(num%10)+0x30;
			data[4]='\0';
		}
		else
		{
			data[0]=' ';
			data[1]=' ';
			data[2]=' ';
			data[3]=num+0x30;
			data[4]='\0';
		}
	}
	else 
	{
		num=0-num;
		if(num>=100)
		{
			data[0]='-';
			data[1]=(num/100)+0x30;
			data[2]=(num%100/10)+0x30;
			data[3]=(num%10)+0x30;
			data[4]='\0';
		}
		else if(num>=10)
		{
			data[0]=' ';
			data[1]='-';
			data[2]=(num/10)+0x30;
			data[3]=(num%10)+0x30;
			data[4]='\0';
		}
		else
		{
			data[0]=' ';
			data[1]=' ';
			data[2]='-';
			data[3]=num+0x30;
			data[4]='\0';
		}
	}
}
void Page0(void)
{
	Display_Graphic_XXX(DISPLAYMODE,3,1,120,48,YZK_Logo);
}
void Page1(void)
{
	if(bStatusDataShowFlag==1)
	{
		bStatusDataShowFlag=0;
	}
	else WindowClear(DISPLAYMODE);
	if(bErrorShowedFlag==1)
	{
		bErrorShowedFlag=0;
		dispsameone(0x00);
	}
	if(bOnLineFlag==0)
	Display_GBK12X12(DISPLAYMODE,0,4,"����");
	else
	{
		Display_GBK12X12(DISPLAYMODE,0,4,"����");
		if(uMachineStatusDatas.RunningStatus&1)
		{
			DisplayString(DISPLAYMODE,3,35,"��"); 
		}
		else
		{
			DisplayString(DISPLAYMODE,3,35,"��"); 
		}
		if(uMachineStatusDatas.RunningStatus&2)
		{
			DisplayString(DISPLAYMODE,5,35,"��"); 
		}
		else
		{
			DisplayString(DISPLAYMODE,5,35,"��"); 
		}
		if(uMachineStatusDatas.RunningStatus&4)
		{
			DisplayString(DISPLAYMODE,3,99,"��"); 
		}
		else
		{
			DisplayString(DISPLAYMODE,3,99,"��"); 
		}
		if(uMachineStatusDatas.RunningStatus&8)
		{
			DisplayString(DISPLAYMODE,5,99,"��"); 
		}
		else
		{
			DisplayString(DISPLAYMODE,5,99,"��"); 
		}
	}
	display_string_5x8(DISPLAYMODE,3,0,"1");
	DisplayString(DISPLAYMODE,3,7,"·"); 
	display_string_5x8(DISPLAYMODE,3,19,":");
	display_string_5x8(DISPLAYMODE,5,0,"2");
	DisplayString(DISPLAYMODE,5,7,"·"); 
	display_string_5x8(DISPLAYMODE,5,19,":");
	display_string_5x8(DISPLAYMODE,3,64,"3");
	DisplayString(DISPLAYMODE,3,71,"·"); 
	display_string_5x8(DISPLAYMODE,3,83,":");
	display_string_5x8(DISPLAYMODE,5,64,"4");
	DisplayString(DISPLAYMODE,5,71,"·"); 
	display_string_5x8(DISPLAYMODE,5,83,":");

//	DECToTextRight(data,uMachineStatusDatas.Voltage_UV);
//	display_string_5x8(DISPLAYMODE,4,50,data);
//	DECToTextRight(data,uMachineStatusDatas.Voltage_VW);
//	display_string_5x8(DISPLAYMODE,4,86,data);
//	DECToTextRight(data,uMachineStatusDatas.Current_U);
//	display_string_5x8(DISPLAYMODE,6,38,data);
//	DECToTextRight(data,uMachineStatusDatas.Current_V);
//	display_string_5x8(DISPLAYMODE,6,68,data);
//	DECToTextRight(data,uMachineStatusDatas.Current_W);
//	display_string_5x8(DISPLAYMODE,6,98,data);
}
void Page2(u8 line)
{
	WindowClear(DISPLAYMODE);
	Display_GBK12X12(DISPLAYMODE,0,4,"���˵�");
	switch(line)
	{
		case 0:
			display_graphic_16x16(DISPLAYMODE,2,10,arrow);
			display_string_5x8(DISPLAYMODE,2,28,"1.");
			Display_GBK12X12(DISPLAYMODE,2,40,"����");
			display_string_5x8(DISPLAYMODE,4,28,"2.");
			Display_GBK12X12(DISPLAYMODE,4,40,"���ϲ�ѯ");
			display_string_5x8(DISPLAYMODE,6,28,"3.");
			Display_GBK12X12(DISPLAYMODE,6,40,"״̬��Ϣ");
			break;
		case 1:
			display_graphic_16x16(DISPLAYMODE,4,10,arrow);
			display_string_5x8(DISPLAYMODE,2,28,"1.");
			Display_GBK12X12(DISPLAYMODE,2,40,"����");
			display_string_5x8(DISPLAYMODE,4,28,"2.");
			Display_GBK12X12(DISPLAYMODE,4,40,"���ϲ�ѯ");
			display_string_5x8(DISPLAYMODE,6,28,"3.");
			Display_GBK12X12(DISPLAYMODE,6,40,"״̬��Ϣ");
			break;
		case 2:
			display_graphic_16x16(DISPLAYMODE,6,10,arrow);
			display_string_5x8(DISPLAYMODE,2,28,"1.");
			Display_GBK12X12(DISPLAYMODE,2,40,"����");
			display_string_5x8(DISPLAYMODE,4,28,"2.");
			Display_GBK12X12(DISPLAYMODE,4,40,"���ϲ�ѯ");
			display_string_5x8(DISPLAYMODE,6,28,"3.");
			Display_GBK12X12(DISPLAYMODE,6,40,"״̬��Ϣ");
			break;
		case 3:
			display_graphic_16x16(DISPLAYMODE,6,10,arrow);
			display_string_5x8(DISPLAYMODE,2,28,"2.");
			Display_GBK12X12(DISPLAYMODE,2,40,"���ϲ�ѯ");
			display_string_5x8(DISPLAYMODE,4,28,"3.");
			Display_GBK12X12(DISPLAYMODE,4,40,"״̬��Ϣ");
			display_string_5x8(DISPLAYMODE,6,28,"4.");
			Display_GBK12X12(DISPLAYMODE,6,40,"����");
			break;
		case 4:
			display_graphic_16x16(DISPLAYMODE,6,10,arrow);
			display_string_5x8(DISPLAYMODE,2,28,"3.");
			Display_GBK12X12(DISPLAYMODE,2,40,"״̬��Ϣ");
			display_string_5x8(DISPLAYMODE,4,28,"4.");
			Display_GBK12X12(DISPLAYMODE,4,40,"����");
			display_string_5x8(DISPLAYMODE,6,28,"5.");
			Display_GBK12X12(DISPLAYMODE,6,40,"����");
			break;
	}
}
void Page3(u8 line)
{
	WindowClear(DISPLAYMODE);
	Display_GBK12X12(DISPLAYMODE,0,4,"����");
	switch(line)
	{
		case 0:
			display_graphic_16x16(DISPLAYMODE,2,10,arrow);
			display_string_5x8(DISPLAYMODE,2,28,"1.");
			Display_GBK12X12(DISPLAYMODE,2,40,"��������");
			display_string_5x8(DISPLAYMODE,4,28,"2.");
			Display_GBK12X12(DISPLAYMODE,4,40,"��ʾ����");
			display_string_5x8(DISPLAYMODE,6,28,"3.");
			Display_GBK12X12(DISPLAYMODE,6,40,"ϵͳ����");
			break;
		case 1:
			display_graphic_16x16(DISPLAYMODE,4,10,arrow);
			display_string_5x8(DISPLAYMODE,2,28,"1.");
			Display_GBK12X12(DISPLAYMODE,2,40,"��������");
			display_string_5x8(DISPLAYMODE,4,28,"2.");
			Display_GBK12X12(DISPLAYMODE,4,40,"��ʾ����");
			display_string_5x8(DISPLAYMODE,6,28,"3.");
			Display_GBK12X12(DISPLAYMODE,6,40,"ϵͳ����");
			break;
		case 2:
			display_graphic_16x16(DISPLAYMODE,6,10,arrow);
			display_string_5x8(DISPLAYMODE,2,28,"1.");
			Display_GBK12X12(DISPLAYMODE,2,40,"��������");
			display_string_5x8(DISPLAYMODE,4,28,"2.");
			Display_GBK12X12(DISPLAYMODE,4,40,"��ʾ����");
			display_string_5x8(DISPLAYMODE,6,28,"3.");
			Display_GBK12X12(DISPLAYMODE,6,40,"ϵͳ����");
			break;
	}
}
bool bErrReadFlag=0;
u16 uErrCount=0;
void ErrorListShow(u8 * data,u8* line)
{
	u8 i;
	u8 string[17];
	u8 list[4]={0,'.',0,0};
	u8 errorpos[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	string[0]='2';
	string[1]='0';
	string[2]=(ErrorStuate.ErrorTime[0]>>4)+0X30;
	string[3]=(ErrorStuate.ErrorTime[0]&0X0F)+0X30;
	string[4]='-';
	string[5]=(ErrorStuate.ErrorTime[1]>>4)+0X30;
	string[6]=(ErrorStuate.ErrorTime[1]&0X0F)+0X30;
	string[7]='-';
	string[8]=(ErrorStuate.ErrorTime[2]>>4)+0X30;
	string[9]=(ErrorStuate.ErrorTime[2]&0X0F)+0X30;
	string[10]=' ';
	string[11]=(ErrorStuate.ErrorTime[3]>>4)+0X30;
	string[12]=(ErrorStuate.ErrorTime[3]&0X0F)+0X30;
	string[13]=':';
	string[14]=(ErrorStuate.ErrorTime[4]>>4)+0X30;
	string[15]=(ErrorStuate.ErrorTime[4]&0X0F)+0X30;
	string[16]=0;
	display_string_5x8(DISPLAYMODE,2,2,string);
//	ErrorStuate.ErrorListL=data[8];
//	ErrorStuate.ErrorListL<<=8;
//	ErrorStuate.ErrorListL|=data[7];
//	ErrorStuate.ErrorDetail=data[10];
//	ErrorStuate.ErrorDetail<<=8;
//	ErrorStuate.ErrorDetail|=data[9];
	uErrorNum=0;
	for(i=0;i<16;i++)
	{
		if(ErrorStuate.ErrorListL&(1<<i))
		{
			errorpos[uErrorNum]=i;
			uErrorNum++;
		}
	}
	if(uErrorNum>2)
	{
		if(*line+2<uErrorNum)
		display_graphic_12x12(DISPLAYMODE,6,107,ArrowDown);
		if(*line>0)display_graphic_12x12(DISPLAYMODE,4,107,ArrowUp);
		if((*line+1)<10)
		{
			list[0]=*line+0x31;
			list[1]='.';
			list[2]=0;
			display_string_5x8(DISPLAYMODE,4,20,list);
		}
		else
		{
			list[0]=((*line+1)/10)+0X30;
			list[1]=((*line+1)%10)+0X30;
			list[2]='.';
			list[3]=0;
			display_string_5x8(DISPLAYMODE,4,14,list);
		}
		if((*line+2)<10)
		{
			list[0]=*line+0x32;
			list[1]='.';
			list[2]=0;
			display_string_5x8(DISPLAYMODE,6,20,list);
		}
		else
		{
			list[0]=((*line+2)/10)+0X30;
			list[1]=((*line+2)%10)+0X30;
			list[2]='.';
			list[3]=0;
			display_string_5x8(DISPLAYMODE,6,14,list);
		}
		DisplayString(DISPLAYMODE,4,32,ErrorDisplayContenr[errorpos[*line]]);
		DisplayString(DISPLAYMODE,6,32,ErrorDisplayContenr[errorpos[*line+1]]);
	}
	else if(uErrorNum==2)
	{
		display_string_5x8(DISPLAYMODE,4,20,"1.");
		display_string_5x8(DISPLAYMODE,6,20,"2.");
		DisplayString(DISPLAYMODE,4,32,ErrorDisplayContenr[errorpos[0]]);
		DisplayString(DISPLAYMODE,6,32,ErrorDisplayContenr[errorpos[1]]);
	}
	else if(uErrorNum==1)
	{
		display_string_5x8(DISPLAYMODE,4,20,"1.");
		DisplayString(DISPLAYMODE,4,32,ErrorDisplayContenr[errorpos[0]]);
	}
}
u8 uErrData[11];
bool bErrorGetFlag;
void	Page4(u8* line)
{
//	u8	data[11];
	u8 string[10];
	u8 len,len2;
	len=DECToText(string,ErrorStuate.ErrorCurrentNum+1);
	string[len]='/';
	len2=DECToText(&string[len+1],uMachineStatusDatas.ErrorExistNum);
	string[len+len2+1]=0;
	WindowClear(DISPLAYMODE);
	Display_GBK12X12(DISPLAYMODE,0,0,"���ϲ�ѯ");
	display_string_5x8(DISPLAYMODE,0,55,string);
	if(uMachineStatusDatas.ErrorExistNum==0)
	{
		Display_GBK12X12(DISPLAYMODE,3,42,"�޼�¼");
	}
	else
	{
		ErrorListGet(uErrCount);
		if(bErrorGetFlag==0)Display_GBK12X12(DISPLAYMODE,4,42,"��ȡ��");
		else 
		{
			display_string_8x16(DISPLAYMODE,4,74,"    ");
			ErrorListShow(uErrData,line);
		}
	}
}
bool bStuatesRefresh;
void	Page5(u8 line)
{
	u8 data[5];
	if(bStuatesRefresh)
	{
		bStuatesRefresh=0;
		WindowClear(DISPLAYMODE);
		switch(line)
	{
		case 0:
			Display_GBK12X12(DISPLAYMODE,0,40,"����״̬");
			display_string_5x8(DISPLAYMODE,3,0,"1");
			DisplayString(DISPLAYMODE,3,7,"·"); 
			display_string_5x8(DISPLAYMODE,3,19,":");
			display_string_5x8(DISPLAYMODE,5,0,"2");
			DisplayString(DISPLAYMODE,5,7,"·"); 
			display_string_5x8(DISPLAYMODE,5,19,":");
			display_string_5x8(DISPLAYMODE,3,64,"3");
			DisplayString(DISPLAYMODE,3,71,"·"); 
			display_string_5x8(DISPLAYMODE,3,83,":");
			display_string_5x8(DISPLAYMODE,5,64,"4");
			DisplayString(DISPLAYMODE,5,71,"·"); 
			display_string_5x8(DISPLAYMODE,5,83,":");
			break;
		case 1:
			Display_GBK12X12(DISPLAYMODE,0,40,"�ڲ��¶�");
			Display_GBK12X12(DISPLAYMODE,3,4,"�����¶�");
		display_graphic_16x16(DISPLAYMODE,3,106,sheshi);
		display_string_8x16(DISPLAYMODE,5,2,"SCR");
		Display_GBK12X12(DISPLAYMODE,5,28,"�¶�");
		display_graphic_16x16(DISPLAYMODE,5,106,sheshi);
			break;
		case 2:
			Display_GBK12X12(DISPLAYMODE,0,52,"����");
		Display_GBK12X12(DISPLAYMODE,2,4,"��ʾ�汾");
		display_string_8x16(DISPLAYMODE,2,70,"V1.30");
		Display_GBK12X12(DISPLAYMODE,4,4,"����汾");
		display_string_8x16(DISPLAYMODE,4,70,"V");
		Display_One_8x16(DISPLAYMODE,4,87,'.');
		Display_GBK12X12(DISPLAYMODE,6,4,"����ʱ��");
		display_string_8x16(DISPLAYMODE,6,98,"h");
			break;
	}
	}
	switch(line)
	{
		case 0:
			if(uMachineStatusDatas.RunningStatus&1)
			{
				DisplayString(DISPLAYMODE,3,35,"��"); 
			}
			else
			{
				DisplayString(DISPLAYMODE,3,35,"��"); 
			}
			if(uMachineStatusDatas.RunningStatus&2)
			{
				DisplayString(DISPLAYMODE,5,35,"��"); 
			}
			else
			{
				DisplayString(DISPLAYMODE,5,35,"��"); 
			}
			if(uMachineStatusDatas.RunningStatus&4)
			{
				DisplayString(DISPLAYMODE,3,99,"��"); 
			}
			else
			{
				DisplayString(DISPLAYMODE,3,99,"��"); 
			}
			if(uMachineStatusDatas.RunningStatus&8)
			{
				DisplayString(DISPLAYMODE,5,99,"��"); 
			}
			else
			{
				DisplayString(DISPLAYMODE,5,99,"��"); 
			}
			break;
		case 1:
		DECToTextRight(data,uMachineStatusDatas.MainBordTemperature);
		display_string_8x16(DISPLAYMODE,3,70,data);
		DECToTextRight(data,uMachineStatusDatas.SCRTemperature);
		display_string_8x16(DISPLAYMODE,5,70,data);
			break;
		case 2:
		Display_One_8x16(DISPLAYMODE,4,79,(uMachineStatusDatas.SoftWareEdition%1000/100)+0x30);
		Display_One_8x16(DISPLAYMODE,4,95,(uMachineStatusDatas.SoftWareEdition%100/10)+0x30);
		Display_One_8x16(DISPLAYMODE,4,103,(uMachineStatusDatas.SoftWareEdition%10)+0x30);
		display_string_8x16(DISPLAYMODE,4,79,data);
		DECToTextRight(data,uMachineStatusDatas.RunningTime);
		display_string_8x16(DISPLAYMODE,6,60,data);
			break;
	}
}
void	Page6(u8 line)
{
	WindowClear(DISPLAYMODE);
	Display_GBK12X12(DISPLAYMODE,0,4,"����");
	switch(line)
	{
		case 0:
			display_graphic_16x16(DISPLAYMODE,2,10,arrow);
			display_string_5x8(DISPLAYMODE,2,28,"1.");
			Display_GBK12X12(DISPLAYMODE,2,40,"����˵��");
			display_string_5x8(DISPLAYMODE,4,28,"2.");
			Display_GBK12X12(DISPLAYMODE,4,40,"����˵��");
			break;
		case 1:
			display_graphic_16x16(DISPLAYMODE,4,10,arrow);
			display_string_5x8(DISPLAYMODE,2,28,"1.");
			Display_GBK12X12(DISPLAYMODE,2,40,"����˵��");
			display_string_5x8(DISPLAYMODE,4,28,"2.");
			Display_GBK12X12(DISPLAYMODE,4,40,"����˵��");
			break;
	}
}
void Page7(void)
{
	WindowClear(DISPLAYMODE);
	Display_GBK12X12(DISPLAYMODE,4,4,"��ϵ��ʽ");
	display_string_8x16(DISPLAYMODE,6,20,"0516-83790799");
}
void Page8(u8 line)
{
	WindowClear(DISPLAYMODE);
	Display_GBK12X12(DISPLAYMODE,0,4,"����˵��");
	switch (line)
	{
		case 0:
			Display_GBK12X12(DISPLAYMODE,2,4,"��Դ��ѹ");
			Display_GBK12X12(DISPLAYMODE,4,12,"����·�������");
			Display_GBK12X12(DISPLAYMODE,6,12,"�����ѹ");
			break;
		case 1:
			Display_GBK12X12(DISPLAYMODE,2,4,"����ʱ��");
			Display_GBK12X12(DISPLAYMODE,4,12,"��ǰ�������ڿ���");
			Display_GBK12X12(DISPLAYMODE,6,12,"��������С�ڼ���");//
			break;
		case 2:
			Display_GBK12X12(DISPLAYMODE,2,4,"����ʱ��");
			Display_GBK12X12(DISPLAYMODE,4,12,"����������ά��ʱ");
			Display_GBK12X12(DISPLAYMODE,6,12,"��");
			break;
		case 3:
			DisplayString(DISPLAYMODE,2,4,"���޵���");
			DisplayString(DISPLAYMODE,4,12,"�������ܳ��ܵ���");
			DisplayString(DISPLAYMODE,6,12,"�����ֵ,��ֵ=��");
			break;
		case 4:
			DisplayString(DISPLAYMODE,2,4,"���޵���");
			DisplayString(DISPLAYMODE,4,12,"��������*���޵�");
			DisplayString(DISPLAYMODE,6,12,"��ϵ��");
			break;
		case 5:
			DisplayString(DISPLAYMODE,2,4,"����ʧ��ϵ��");
			DisplayString(DISPLAYMODE,4,12,"��������һ�����");
			DisplayString(DISPLAYMODE,6,12,"��ƽ�������ı�ֵ");
			break;
		case 6:
			DisplayString(DISPLAYMODE,2,4,"����ʧ��ϵ��");
			DisplayString(DISPLAYMODE,4,12,"�������õ����಻");
			DisplayString(DISPLAYMODE,6,12,"ƽ��ٷֱ�");
			break;
		case 7:
			DisplayString(DISPLAYMODE,2,4,"��������");
			DisplayString(DISPLAYMODE,4,12,"��բ�ܻ���ư��");
			DisplayString(DISPLAYMODE,6,12,"���¾���ֵ,����");
			break;
		case 8:
			DisplayString(DISPLAYMODE,2,4,"��Դ��ѹ");
			DisplayString(DISPLAYMODE,4,12,"�趨ֵ,�����ڲ�");
			DisplayString(DISPLAYMODE,6,12,"ɢ�ȷ��");
			break;
		case 9:
			DisplayString(DISPLAYMODE,2,4,"��������");
			DisplayString(DISPLAYMODE,4,12,"��բ�ܻ���ư��");
			DisplayString(DISPLAYMODE,6,12,"���±���ֵ");
			break;
		case 10:
			DisplayString(DISPLAYMODE,2,4,"����ʹ��");
			DisplayString(DISPLAYMODE,4,12,"��������Ե����");
			DisplayString(DISPLAYMODE,6,12,"����ĸ��ֱ�����");
			break;
		case 11:
			DisplayString(DISPLAYMODE,2,4,"����ʹ��");
			DisplayString(DISPLAYMODE,4,12,"�ܵ����ú͹ر�");
			break;
		case 12:
			DisplayString(DISPLAYMODE,2,4,"CT��ֵ");
			DisplayString(DISPLAYMODE,4,12,"������������CT��");
			DisplayString(DISPLAYMODE,6,12,"ֵ");
			break;
		case 13:
			DisplayString(DISPLAYMODE,2,4,"������");
			DisplayString(DISPLAYMODE,4,12,"485ͨѶ�Ĳ�����");
			break;
		case 14:
			DisplayString(DISPLAYMODE,2,4,"ͨѶ��ַ");
			DisplayString(DISPLAYMODE,4,12,"485ͨѶ��������ַ");
			break;
		case 15:
			DisplayString(DISPLAYMODE,2,4,"�ָ���������");
			DisplayString(DISPLAYMODE,4,12,"�������ĸ�������");
		DisplayString(DISPLAYMODE,6,12,"��Ϊ����ֵ");
			break;
	}
	
	
}
void Page9(u8 line)
{
	WindowClear(DISPLAYMODE);
	Display_GBK12X12(DISPLAYMODE,0,40,"����˵��");
	switch(line)
	{
		case 0:
			DisplayString(DISPLAYMODE,2,4,"��Դȱ��");
			DisplayString(DISPLAYMODE,4,12,"����һ���ѹС��");
			DisplayString(DISPLAYMODE,6,12,"300V");
			break;
		case 1:
			DisplayString(DISPLAYMODE,2,4,"����");
			DisplayString(DISPLAYMODE,4,12,"��������ƽ��ֵ��");
			DisplayString(DISPLAYMODE,6,12,"�ڿ���������,С��");
			break;
		case 2:
			DisplayString(DISPLAYMODE,2,4,"����");
			DisplayString(DISPLAYMODE,4,12,"˲�����ֵ,�ҳ���");
			DisplayString(DISPLAYMODE,6,12,"����ʱ��");
			break;
		case 3:
			DisplayString(DISPLAYMODE,2,4,"˲�����");
			DisplayString(DISPLAYMODE,4,12,"���������ڹ���");
			DisplayString(DISPLAYMODE,6,12,"����*���޵���ϵ��");
			break;
		case 4:
			DisplayString(DISPLAYMODE,2,4,"���಻ƽ��");
			DisplayString(DISPLAYMODE,4,12,"�����������֮��");
			DisplayString(DISPLAYMODE,6,12,"�Ĳ�ֵ�����ϸ���");
			break;
		case 5:
			DisplayString(DISPLAYMODE,2,4,"���಻ƽ��");
			DisplayString(DISPLAYMODE,4,12,"����ֵ��������ʧ");
			DisplayString(DISPLAYMODE,6,12,"��ϵ��");
			break;
		case 6:
			DisplayString(DISPLAYMODE,2,4,"SCR����");
			DisplayString(DISPLAYMODE,4,12,"��բ���¶ȳ�����");
			DisplayString(DISPLAYMODE,6,12,"բ�ܳ����¶�");
			break;
		case 7:
			DisplayString(DISPLAYMODE,2,4,"���ư峬��");
			DisplayString(DISPLAYMODE,4,12,"���ư��¶ȳ�����");
			DisplayString(DISPLAYMODE,6,12,"�ư峬���¶�");
			break;
		case 8:
			DisplayString(DISPLAYMODE,2,4,"SCR����");
			DisplayString(DISPLAYMODE,4,12,"��δ����״̬��,��");
			DisplayString(DISPLAYMODE,6,12,"�⵽�о�բ�����,");
			break;
		case 9:
			DisplayString(DISPLAYMODE,2,4,"SCR����");
			DisplayString(DISPLAYMODE,4,12,"������������ź�");
			DisplayString(DISPLAYMODE,6,12,"�����źŲ�ƥ��");
			break;
		case 10:
			DisplayString(DISPLAYMODE,2,4,"��Դ��ѹ");
			DisplayString(DISPLAYMODE,4,12,"ƽ����ѹ���ڵ�Դ");
			DisplayString(DISPLAYMODE,6,12,"��ѹ");
			break;
		case 11:
			DisplayString(DISPLAYMODE,2,4,"���ȱ��");
			DisplayString(DISPLAYMODE,4,12,"���й�����,������");
			DisplayString(DISPLAYMODE,6,12,"����ÿһ��ĵ�");
			break;
		case 12:
			DisplayString(DISPLAYMODE,2,4,"���ȱ��");
			DisplayString(DISPLAYMODE,4,12,"��ֵ,��С��2A,��");
			DisplayString(DISPLAYMODE,6,12,"����������С1A,��");
			break;
		case 13:
			DisplayString(DISPLAYMODE,2,4,"���ȱ��");
			DisplayString(DISPLAYMODE,4,12,"��Ϊ���ȱ�ಢ��");
			DisplayString(DISPLAYMODE,6,12,"�����");
			break;
		case 14:
			DisplayString(DISPLAYMODE,2,4,"�洢ʧЧ");
			DisplayString(DISPLAYMODE,4,12,"�洢����4������δ");
			DisplayString(DISPLAYMODE,6,12,"����ɹ�");
			break;
	}
}
void Page10(u16 key,u8 line)
{
	u8 data[3];
	data[0]=(key/100)+0x30;
	data[1]=(key%100/10)+0x30;
	data[2]=(key%10)+0x30;
	WindowClear(DISPLAYMODE);
	switch(line)
	{
		case 0:
			Display_One_8x16(DISPLAYMODE,3,36,data[0]);
			Display_One_8x16(DISPLAYMODE,3,58,'*');
			Display_One_8x16(DISPLAYMODE,3,80,'*');
			break;
		case 1:
			Display_One_8x16(DISPLAYMODE,3,36,'*');
			Display_One_8x16(DISPLAYMODE,3,58,data[1]);
			Display_One_8x16(DISPLAYMODE,3,80,'*');
			break;
		case 2:
			Display_One_8x16(DISPLAYMODE,3,36,'*');
			Display_One_8x16(DISPLAYMODE,3,58,'*');
			Display_One_8x16(DISPLAYMODE,3,80,data[2]);
			break;
	}
}
void Page11(u8 line)
{
	WindowClear(DISPLAYMODE);
	Display_GBK12X12(DISPLAYMODE,0,4,"��������");
	switch(line)
	{
		case 0:
			display_graphic_16x16(DISPLAYMODE,2,10,arrow);
			display_string_5x8(DISPLAYMODE,2,28,"1.");
			Display_GBK12X12(DISPLAYMODE,2,40,"��������");
			display_string_5x8(DISPLAYMODE,4,28,"2.");
			Display_GBK12X12(DISPLAYMODE,4,40,"������ʱ");
			display_string_5x8(DISPLAYMODE,6,28,"3.");
			Display_GBK12X12(DISPLAYMODE,6,40,"�ϵ���ʱ");
			break;
		case 1:
			display_graphic_16x16(DISPLAYMODE,4,10,arrow);
			display_string_5x8(DISPLAYMODE,2,28,"1.");
			Display_GBK12X12(DISPLAYMODE,2,40,"��������");
			display_string_5x8(DISPLAYMODE,4,28,"2.");
			Display_GBK12X12(DISPLAYMODE,4,40,"������ʱ");
			display_string_5x8(DISPLAYMODE,6,28,"3.");
			Display_GBK12X12(DISPLAYMODE,6,40,"�ϵ���ʱ");
			break;
		case 2:
			display_graphic_16x16(DISPLAYMODE,6,10,arrow);
			display_string_5x8(DISPLAYMODE,2,28,"1.");
			Display_GBK12X12(DISPLAYMODE,2,40,"��������");
			display_string_5x8(DISPLAYMODE,4,28,"2.");
			Display_GBK12X12(DISPLAYMODE,4,40,"������ʱ");
			display_string_5x8(DISPLAYMODE,6,28,"3.");
			Display_GBK12X12(DISPLAYMODE,6,40,"�ϵ���ʱ");
			break;
	}
}

void Page13(u16	value)
{
	u8 data[5];
	WindowClear(DISPLAYMODE);
	Display_GBK12X12(DISPLAYMODE,0,40,"������ʱ");	
	DECToTextRight(data,value);
	display_string_8x16(DISPLAYMODE,3,40,data);
	display_string_8x16(DISPLAYMODE,3,74,"ms");
}
void Page14(u16	value)
{
	u8 data[5];
	WindowClear(DISPLAYMODE);
	Display_GBK12X12(DISPLAYMODE,0,40,"�ϵ���ʱ");	
	DECToTextRight(data,value);
	display_string_8x16(DISPLAYMODE,3,40,data);
	display_string_8x16(DISPLAYMODE,3,74,"ms");
}
void Page15(u16	value)
{
	u8 data[2];
	data[0]=(value/10)+0x30;
	data[1]=(value%10)+0x30;
	WindowClear(DISPLAYMODE);
	Display_GBK12X12(DISPLAYMODE,0,4,"����ʧ��ϵ��");	
	Display_One_8x16(DISPLAYMODE,3,45,data[0]);
	Display_One_8x16(DISPLAYMODE,3,57,data[1]);
	Display_One_8x16(DISPLAYMODE,3,69,'%');
}
void Page16(u8	scr,u8 main,u8 line)
{
	u8 scrdata[4],maindata[4];
	u8 select;
	u8 noselect;
	if(DISPLAYMODE==0)
	{
		select=1;
		noselect=0;
	}
	else
	{
		select=0;
		noselect=1;
	}
	DECToText(scrdata,scr);
	DECToText(maindata,main);
	WindowClear(DISPLAYMODE);
	Display_GBK12X12(DISPLAYMODE,0,40,"��������");	
//	display_string_8x16(0,4,0,"SCR");
//	Display_GBK12X12(0,4,25,"�¶�");	
	DisplayString(DISPLAYMODE,4,0,"SCR�¶�");
	Display_GBK12X12(DISPLAYMODE,6,0,"�����¶�");	
	if(line==0)
	{
		display_string_8x16(select,4,76,scrdata);
		display_graphic_16x16(DISPLAYMODE,4,106,sheshi);
		display_string_8x16(noselect,6,76,maindata);
		display_graphic_16x16(DISPLAYMODE,6,106,sheshi);
	}
	else if(line==1)
	{
		display_string_8x16(noselect,4,76,scrdata);
		display_graphic_16x16(DISPLAYMODE,4,106,sheshi);
		display_string_8x16(select,6,76,maindata);
		display_graphic_16x16(DISPLAYMODE,6,106,sheshi);
	}
}
void Page12(u8	scr,u8 main,u8 line)
{
	u8 scrdata[4],maindata[4];
	u8 select;
	u8 noselect;
	if(DISPLAYMODE==0)
	{
		select=1;
		noselect=0;
	}
	else
	{
		select=0;
		noselect=1;
	}
	DECToText(scrdata,scr);
	DECToText(maindata,main);
	WindowClear(DISPLAYMODE);
	Display_GBK12X12(DISPLAYMODE,0,40,"��������");	
	display_string_8x16(DISPLAYMODE,4,0,"SCR");
	Display_GBK12X12(DISPLAYMODE,4,25,"�¶�");	
	Display_GBK12X12(DISPLAYMODE,6,0,"�����¶�");	
	if(line==0)
	{
		display_string_8x16(select,4,76,scrdata);
		display_graphic_16x16(DISPLAYMODE,4,106,sheshi);
		display_string_8x16(noselect,6,76,maindata);
		display_graphic_16x16(DISPLAYMODE,6,106,sheshi);
	}
	else if(line==1)
	{
		display_string_8x16(noselect,4,76,scrdata);
		display_graphic_16x16(DISPLAYMODE,4,106,sheshi);
		display_string_8x16(select,6,76,maindata);
		display_graphic_16x16(DISPLAYMODE,6,106,sheshi);
	}
}
void Page18(u8 line)
{
	WindowClear(DISPLAYMODE);
	Display_GBK12X12(DISPLAYMODE,0,4,"��ʾ����");
	switch(line)
	{
		case 0:
			display_graphic_16x16(DISPLAYMODE,2,10,arrow);
			display_string_5x8(DISPLAYMODE,2,28,"1.");
			Display_GBK12X12(DISPLAYMODE,2,40,"��������");
			display_string_5x8(DISPLAYMODE,4,28,"2.");
			Display_GBK12X12(DISPLAYMODE,4,40,"ʱ������");
		display_string_5x8(DISPLAYMODE,6,28,"3.");
			Display_GBK12X12(DISPLAYMODE,6,40,"��������");
			break;
		case 1:
			display_graphic_16x16(DISPLAYMODE,4,10,arrow);
			display_string_5x8(DISPLAYMODE,2,28,"1.");
			Display_GBK12X12(DISPLAYMODE,2,40,"��������");
			display_string_5x8(DISPLAYMODE,4,28,"2.");
			Display_GBK12X12(DISPLAYMODE,4,40,"ʱ������");
		display_string_5x8(DISPLAYMODE,6,28,"3.");
			Display_GBK12X12(DISPLAYMODE,6,40,"��������");
			break;
		case 2:
			display_graphic_16x16(DISPLAYMODE,6,10,arrow);
			display_string_5x8(DISPLAYMODE,2,28,"1.");
			Display_GBK12X12(DISPLAYMODE,2,40,"��������");
			display_string_5x8(DISPLAYMODE,4,28,"2.");
			Display_GBK12X12(DISPLAYMODE,4,40,"ʱ������");
		display_string_5x8(DISPLAYMODE,6,28,"3.");
			Display_GBK12X12(DISPLAYMODE,6,40,"��������");
			break;
	}
}
void Page19(u8 line)
{
	WindowClear(DISPLAYMODE);
	Display_GBK12X12(DISPLAYMODE,0,4,"��������");
	switch(line)
	{
		case 0:
			display_graphic_16x16(DISPLAYMODE,2,10,arrow);
			display_string_5x8(DISPLAYMODE,2,28,"1.");
			Display_GBK12X12(DISPLAYMODE,2,40,"��������");
			display_string_5x8(DISPLAYMODE,4,28,"2.");
			display_string_8x16(DISPLAYMODE,4,40,"English");
			break;
		case 1:
			display_graphic_16x16(DISPLAYMODE,4,10,arrow);
			display_string_5x8(DISPLAYMODE,2,28,"1.");
			Display_GBK12X12(DISPLAYMODE,2,40,"��������");
			display_string_5x8(DISPLAYMODE,4,28,"2.");
			display_string_8x16(DISPLAYMODE,4,40,"English");
			break;
	}
}
void Page20(u8 line ,Time_Def time)
{
	u8 year[5];
	u8 month[3];
	u8 day[3];
	u8 hour[3];
	u8 min[3];
	u8 select;
	u8 noselect;
	if(DISPLAYMODE==0)
	{
		select=1;
		noselect=0;
	}
	else
	{
		select=0;
		noselect=1;
	}
	year[0]=0x32;
	year[1]=0x30;
	year[2]=(time.year>>4)+0x30;
	year[3]=(time.year&0x0f)+0x30;
	year[4]=0;
	month[0]=(time.month>>4)+0x30;
	month[1]=(time.month&0x0f)+0x30;
	month[2]=0;
	day[0]=(time.day>>4)+0x30;
	day[1]=(time.day&0x0f)+0x30;
	day[2]=0;
	hour[0]=(time.hour>>4)+0x30;
	hour[1]=(time.hour&0x0f)+0x30;
	hour[2]=0;
	min[0]=(time.minute>>4)+0x30;
	min[1]=(time.minute&0x0f)+0x30;
	min[2]=0;
	WindowClear(DISPLAYMODE);
	Display_GBK12X12(DISPLAYMODE,0,4,"ʱ������");
	switch(line)
	{
		case 0:
			display_string_8x16(select,3,20,year);
		Display_One_8x16(DISPLAYMODE,3,52,'-');
		display_string_8x16(noselect,3,60,month);
		Display_One_8x16(DISPLAYMODE,3,76,'-');
		display_string_8x16(noselect,3,84,day);
		display_string_8x16(noselect,5,40,hour);
		Display_One_8x16(DISPLAYMODE,5,56,':');
		display_string_8x16(noselect,5,64,min);
			break;
		case 1:
			display_string_8x16(noselect,3,20,year);
		Display_One_8x16(DISPLAYMODE,3,52,'-');
		display_string_8x16(select,3,60,month);
		Display_One_8x16(DISPLAYMODE,3,76,'-');
		display_string_8x16(noselect,3,84,day);
		display_string_8x16(noselect,5,40,hour);
		Display_One_8x16(DISPLAYMODE,5,56,':');
		display_string_8x16(noselect,5,64,min);
			break;
		case 2:
			display_string_8x16(noselect,3,20,year);
		Display_One_8x16(DISPLAYMODE,3,52,'-');
		display_string_8x16(noselect,3,60,month);
		Display_One_8x16(DISPLAYMODE,3,76,'-');
		display_string_8x16(select,3,84,day);
		display_string_8x16(noselect,5,40,hour);
		Display_One_8x16(DISPLAYMODE,5,56,':');
		display_string_8x16(noselect,5,64,min);
			break;
		case 3:
			display_string_8x16(noselect,3,20,year);
		Display_One_8x16(DISPLAYMODE,3,52,'-');
		display_string_8x16(noselect,3,60,month);
		Display_One_8x16(DISPLAYMODE,3,76,'-');
		display_string_8x16(noselect,3,84,day);
		display_string_8x16(select,5,40,hour);
		Display_One_8x16(DISPLAYMODE,5,56,':');
		display_string_8x16(noselect,5,64,min);
			break;
		case 4:
			display_string_8x16(noselect,3,20,year);
		Display_One_8x16(DISPLAYMODE,3,52,'-');
		display_string_8x16(noselect,3,60,month);
		Display_One_8x16(DISPLAYMODE,3,76,'-');
		display_string_8x16(noselect,3,84,day);
		display_string_8x16(noselect,5,40,hour);
		Display_One_8x16(DISPLAYMODE,5,56,':');
		display_string_8x16(select,5,64,min);
			break;
	}
}

void Page21(u8 line)
{
	WindowClear(DISPLAYMODE);
	Display_GBK12X12(DISPLAYMODE,0,4,"ϵͳ����");
	switch(line)
	{
		case 0:
			display_graphic_16x16(DISPLAYMODE,2,10,arrow);
			display_string_5x8(DISPLAYMODE,2,28,"1.");
			Display_GBK12X12(DISPLAYMODE,2,40,"����ʹ��");
			display_string_5x8(DISPLAYMODE,4,28,"2.");
			Display_GBK12X12(DISPLAYMODE,4,40,"�����ж�ʱ��");
			display_string_5x8(DISPLAYMODE,6,28,"3.");
			Display_GBK12X12(DISPLAYMODE,6,40,"���ϸ�λʱ��");
			break;
		case 1:
			display_graphic_16x16(DISPLAYMODE,4,10,arrow);
			display_string_5x8(DISPLAYMODE,2,28,"1.");
			Display_GBK12X12(DISPLAYMODE,2,40,"����ʹ��");
			display_string_5x8(DISPLAYMODE,4,28,"2.");
			Display_GBK12X12(DISPLAYMODE,4,40,"�����ж�ʱ��");
			display_string_5x8(DISPLAYMODE,6,28,"3.");
			Display_GBK12X12(DISPLAYMODE,6,40,"���ϸ�λʱ��");
			break;
		case 2:
			display_graphic_16x16(DISPLAYMODE,6,10,arrow);
			display_string_5x8(DISPLAYMODE,2,28,"1.");
			Display_GBK12X12(DISPLAYMODE,2,40,"����ʹ��");
			display_string_5x8(DISPLAYMODE,4,28,"2.");
			Display_GBK12X12(DISPLAYMODE,4,40,"�����ж�ʱ��");
			display_string_5x8(DISPLAYMODE,6,28,"3.");
			Display_GBK12X12(DISPLAYMODE,6,40,"���ϸ�λʱ��");
			break;
		case 3:
			display_graphic_16x16(DISPLAYMODE,6,10,arrow);
			display_string_5x8(DISPLAYMODE,2,28,"2.");
			Display_GBK12X12(DISPLAYMODE,2,40,"�����ж�ʱ��");
			display_string_5x8(DISPLAYMODE,4,28,"3.");
			Display_GBK12X12(DISPLAYMODE,4,40,"���ϸ�λʱ��");
			display_string_5x8(DISPLAYMODE,6,28,"4.");
			Display_GBK12X12(DISPLAYMODE,6,40,"�������");
			break;
		case 4:
			display_graphic_16x16(DISPLAYMODE,6,10,arrow);
			display_string_5x8(DISPLAYMODE,2,28,"3.");
			Display_GBK12X12(DISPLAYMODE,2,40,"���ϸ�λʱ��");
			display_string_5x8(DISPLAYMODE,4,28,"4.");
			Display_GBK12X12(DISPLAYMODE,4,40,"�������");
			display_string_5x8(DISPLAYMODE,6,28,"5.");
			Display_GBK12X12(DISPLAYMODE,6,40,"��������");
			break;
		case 5:
			display_graphic_16x16(DISPLAYMODE,6,10,arrow);
			display_string_5x8(DISPLAYMODE,2,28,"4.");
			Display_GBK12X12(DISPLAYMODE,2,40,"�������");
			display_string_5x8(DISPLAYMODE,4,28,"5.");
			Display_GBK12X12(DISPLAYMODE,4,40,"��������");
			display_string_5x8(DISPLAYMODE,6,28,"6.");
			if(bOfflineModeFlag==0)
			Display_GBK12X12(DISPLAYMODE,6,40,"������ԭ");
			else Display_GBK12X12(DISPLAYMODE,6,40,"�����鿴");
			break;
		case 6:
			display_graphic_16x16(DISPLAYMODE,6,10,arrow);
			display_string_5x8(DISPLAYMODE,2,28,"5.");
			Display_GBK12X12(DISPLAYMODE,2,40,"��������");
			display_string_5x8(DISPLAYMODE,4,28,"6.");
		if(bOfflineModeFlag==0)
			Display_GBK12X12(DISPLAYMODE,4,40,"������ԭ");
		else Display_GBK12X12(DISPLAYMODE,4,40,"�����鿴");
			display_string_5x8(DISPLAYMODE,6,28,"7.");
			Display_GBK12X12(DISPLAYMODE,6,40,"�ָ���������");
			break;
		case 7:
			display_graphic_16x16(DISPLAYMODE,6,10,arrow);
			display_string_5x8(DISPLAYMODE,2,28,"6.");
		if(bOfflineModeFlag==0)
			Display_GBK12X12(DISPLAYMODE,2,40,"������ԭ");
		else Display_GBK12X12(DISPLAYMODE,2,40,"�����鿴");
			display_string_5x8(DISPLAYMODE,4,28,"7.");
			Display_GBK12X12(DISPLAYMODE,4,40,"�ָ���������");
			display_string_5x8(DISPLAYMODE,6,28,"8.");
			display_string_8x16(DISPLAYMODE,6,40,"EP");
			break;
	}
}
void Page22(u8 line,u16 flag)
{
	WindowClear(DISPLAYMODE);
	Display_GBK12X12(DISPLAYMODE,0,4,"����ʹ��");
	switch(line)
	{
		case 0:
			display_graphic_16x16(DISPLAYMODE,2,10,arrow);
			display_string_5x8(DISPLAYMODE,2,28,"1.");
			DisplayString(DISPLAYMODE,2,40,"SCR����");
		if(flag&0x0001)Display_GBK12X12(DISPLAYMODE,2,114,"��");
		else Display_GBK12X12(DISPLAYMODE,2,114,"��");
			display_string_5x8(DISPLAYMODE,4,28,"2.");
			DisplayString(DISPLAYMODE,4,40,"SCR����");
		if(flag&0x0002)Display_GBK12X12(DISPLAYMODE,4,114,"��");
		else Display_GBK12X12(DISPLAYMODE,4,114,"��");
			display_string_5x8(DISPLAYMODE,6,28,"3.");
			Display_GBK12X12(DISPLAYMODE,6,40,"���ư峬��");
		if(flag&0x0004)Display_GBK12X12(DISPLAYMODE,6,114,"��");
		else Display_GBK12X12(DISPLAYMODE,6,114,"��");
			break;
		case 1:
			display_graphic_16x16(DISPLAYMODE,4,10,arrow);
			display_string_5x8(DISPLAYMODE,2,28,"1.");
			DisplayString(DISPLAYMODE,2,40,"SCR����");
		if(flag&0x0001)Display_GBK12X12(DISPLAYMODE,2,114,"��");
		else Display_GBK12X12(DISPLAYMODE,2,114,"��");
			display_string_5x8(DISPLAYMODE,4,28,"2.");
			DisplayString(DISPLAYMODE,4,40,"SCR����");
		if(flag&0x0002)Display_GBK12X12(DISPLAYMODE,4,114,"��");
		else Display_GBK12X12(DISPLAYMODE,4,114,"��");
			display_string_5x8(DISPLAYMODE,6,28,"3.");
			Display_GBK12X12(DISPLAYMODE,6,40,"���ư峬��");
		if(flag&0x0004)Display_GBK12X12(DISPLAYMODE,6,114,"��");
		else Display_GBK12X12(DISPLAYMODE,6,114,"��");
			break;
		case 2:
			display_graphic_16x16(DISPLAYMODE,6,10,arrow);
			display_string_5x8(DISPLAYMODE,2,28,"1.");
			DisplayString(DISPLAYMODE,2,40,"SCR����");
		if(flag&0x0001)Display_GBK12X12(DISPLAYMODE,2,114,"��");
		else Display_GBK12X12(DISPLAYMODE,2,114,"��");
			display_string_5x8(DISPLAYMODE,4,28,"2.");
			DisplayString(DISPLAYMODE,4,40,"SCR����");
		if(flag&0x0002)Display_GBK12X12(DISPLAYMODE,4,114,"��");
		else Display_GBK12X12(DISPLAYMODE,4,114,"��");
			display_string_5x8(DISPLAYMODE,6,28,"3.");
			Display_GBK12X12(DISPLAYMODE,6,40,"���ư峬��");
		if(flag&0x0004)Display_GBK12X12(DISPLAYMODE,6,114,"��");
		else Display_GBK12X12(DISPLAYMODE,6,114,"��");
			break;
		case 3:
			display_graphic_16x16(DISPLAYMODE,6,10,arrow);
			display_string_5x8(DISPLAYMODE,2,28,"2.");
			DisplayString(DISPLAYMODE,2,40,"SCR����");
		if(flag&0x0002)Display_GBK12X12(DISPLAYMODE,2,114,"��");
		else Display_GBK12X12(DISPLAYMODE,2,114,"��");
			display_string_5x8(DISPLAYMODE,4,28,"3.");
			Display_GBK12X12(DISPLAYMODE,4,40,"���ư峬��");
		if(flag&0x0004)Display_GBK12X12(DISPLAYMODE,4,114,"��");
		else Display_GBK12X12(DISPLAYMODE,4,114,"��");
			display_string_5x8(DISPLAYMODE,6,28,"4.");
			DisplayString(DISPLAYMODE,6,40,"һ��ȡ��");
		if(flag&0x8000)Display_GBK12X12(DISPLAYMODE,6,114,"��");
		else Display_GBK12X12(DISPLAYMODE,6,114,"��");
			break;
	}
}
void Page23(u16 value)
{
	u8 data[5];
	DECToTextRight(data,value);
	WindowClear(DISPLAYMODE);
	Display_GBK12X12(DISPLAYMODE,0,10,"�����ж�ʱ��");	
	display_string_8x16(DISPLAYMODE,3,40,data);
	display_string_8x16(DISPLAYMODE,3,74,"ms");
}
void Page24(u16 value)
{
	u8 data[5];
	DECToTextRight(data,value);
	WindowClear(DISPLAYMODE);
	Display_GBK12X12(DISPLAYMODE,0,10,"���ϸ�λʱ��");	
	display_string_8x16(DISPLAYMODE,3,40,data);
	display_string_8x16(DISPLAYMODE,3,74,"ms");
}
void Page25(u8 line, u8 flag)
{
	WindowClear(DISPLAYMODE);
	Display_GBK12X12(DISPLAYMODE,0,4,"�������");	
	switch(line)
	{
		case 0:
			display_graphic_16x16(DISPLAYMODE,2,0,arrow);
			display_string_5x8(DISPLAYMODE,2,18,"1.");
			Display_GBK12X12(DISPLAYMODE,2,30,"ƽ����ʹ��");
		if(flag&0x0001)Display_GBK12X12(DISPLAYMODE,2,114,"��");
		else Display_GBK12X12(DISPLAYMODE,2,114,"��");
			display_string_5x8(DISPLAYMODE,4,18,"2.");
			DisplayString(DISPLAYMODE,4,30,"�洢ʧЧ����");
		DisplayString(DISPLAYMODE,6,30,"���ϼ̵���");
		if(flag&0x0002)Display_GBK12X12(DISPLAYMODE,6,114,"��");
		else Display_GBK12X12(DISPLAYMODE,6,114,"��");
			break;
		case 1:
			display_graphic_16x16(DISPLAYMODE,4,0,arrow);
			display_string_5x8(DISPLAYMODE,2,18,"1.");
			Display_GBK12X12(DISPLAYMODE,2,30,"ƽ����ʹ��");
		if(flag&0x0001)Display_GBK12X12(DISPLAYMODE,2,114,"��");
		else Display_GBK12X12(DISPLAYMODE,2,114,"��");
			display_string_5x8(DISPLAYMODE,4,18,"2.");
			DisplayString(DISPLAYMODE,4,30,"�洢ʧЧ����");
		DisplayString(DISPLAYMODE,6,30,"���ϼ̵���");
		if(flag&0x0002)Display_GBK12X12(DISPLAYMODE,6,114,"��");
		else Display_GBK12X12(DISPLAYMODE,6,114,"��");
			break;
	}
}
void Page26(void)
{
	WindowClear(DISPLAYMODE);
	Display_GBK12X12(DISPLAYMODE,3,28,"�ָ���������");	
	Display_GBK12X12(DISPLAYMODE,6,4,"��");	
	Display_GBK12X12(DISPLAYMODE,6,114,"��");	
}
void Page27(u8 line)
{
	WindowClear(DISPLAYMODE);
	display_string_8x16(DISPLAYMODE,0,4,"EP");
	switch(line)
	{
		case 0:
			display_graphic_16x16(DISPLAYMODE,2,10,arrow);
			display_string_5x8(DISPLAYMODE,2,28,"1.");
			Display_GBK12X12(DISPLAYMODE,2,40,"����������");
			display_string_5x8(DISPLAYMODE,4,28,"2.");
			Display_GBK12X12(DISPLAYMODE,4,40,"������ʱ");
			display_string_5x8(DISPLAYMODE,6,28,"3.");
			Display_GBK12X12(DISPLAYMODE,6,40,"����ϵ��");
			break;
		case 1:
			display_graphic_16x16(DISPLAYMODE,4,10,arrow);
			display_string_5x8(DISPLAYMODE,2,28,"1.");
			Display_GBK12X12(DISPLAYMODE,2,40,"����������");
			display_string_5x8(DISPLAYMODE,4,28,"2.");
			Display_GBK12X12(DISPLAYMODE,4,40,"������ʱ");
			display_string_5x8(DISPLAYMODE,6,28,"3.");
			Display_GBK12X12(DISPLAYMODE,6,40,"����ϵ��");
			break;
		case 2:
			display_graphic_16x16(DISPLAYMODE,6,10,arrow);
			display_string_5x8(DISPLAYMODE,2,28,"1.");
			Display_GBK12X12(DISPLAYMODE,2,40,"����������");
			display_string_5x8(DISPLAYMODE,4,28,"2.");
			Display_GBK12X12(DISPLAYMODE,4,40,"������ʱ");
			display_string_5x8(DISPLAYMODE,6,28,"3.");
			Display_GBK12X12(DISPLAYMODE,6,40,"����ϵ��");
			break;
		case 3:
			display_graphic_16x16(DISPLAYMODE,6,10,arrow);
			display_string_5x8(DISPLAYMODE,2,28,"2.");
			Display_GBK12X12(DISPLAYMODE,2,40,"������ʱ");
			display_string_5x8(DISPLAYMODE,4,28,"3.");
			Display_GBK12X12(DISPLAYMODE,4,40,"����ϵ��");
			display_string_5x8(DISPLAYMODE,6,28,"4.");
			Display_GBK12X12(DISPLAYMODE,6,40,"����ʱ��");
			break;
		case 4:
			display_graphic_16x16(DISPLAYMODE,6,10,arrow);
			display_string_5x8(DISPLAYMODE,2,28,"3.");
			Display_GBK12X12(DISPLAYMODE,2,40,"����ϵ��");
			display_string_5x8(DISPLAYMODE,4,28,"4.");
			Display_GBK12X12(DISPLAYMODE,4,40,"����ʱ��");
			display_string_5x8(DISPLAYMODE,6,28,"5.");
			Display_GBK12X12(DISPLAYMODE,6,40,"����ʱ��");
			break;
		case 5:
			display_graphic_16x16(DISPLAYMODE,6,10,arrow);
			display_string_5x8(DISPLAYMODE,2,28,"4.");
			Display_GBK12X12(DISPLAYMODE,2,40,"����ʱ��");
			display_string_5x8(DISPLAYMODE,4,28,"5.");
			Display_GBK12X12(DISPLAYMODE,4,40,"����ʱ��");
			display_string_5x8(DISPLAYMODE,6,28,"6.");
			Display_GBK12X12(DISPLAYMODE,6,40,"��ѹУ׼");
			break;
	}
}
void Page28(u16	value)
{
	u8 data[4];
	DECToText(data,value);
	WindowClear(DISPLAYMODE);
	Display_GBK12X12(DISPLAYMODE,0,40,"����������");	
	display_string_8x16(DISPLAYMODE,3,50,data);
	Display_One_8x16(DISPLAYMODE,3,76,'A');
}
void Page29(u16	value)
{
	u8 data[3];
	DECToText(data,value);
	WindowClear(DISPLAYMODE);
	Display_GBK12X12(DISPLAYMODE,0,40,"������ʱ");	
	display_string_8x16(DISPLAYMODE,3,50,data);
	Display_One_8x16(DISPLAYMODE,3,76,'S');
}
void Page30(u16	value)
{
	u8 data[3];
	DECToText(data,value);
	WindowClear(DISPLAYMODE);
	Display_GBK12X12(DISPLAYMODE,0,40,"����ϵ��");	
	display_string_8x16(DISPLAYMODE,3,50,data);
	Display_One_8x16(DISPLAYMODE,3,76,'%');
}
void Page31(u16	value)
{
	u8 data[5];
	DECToText(data,value);
	WindowClear(DISPLAYMODE);
	Display_GBK12X12(DISPLAYMODE,0,40,"����ʱ��");	
	display_string_8x16(DISPLAYMODE,3,42,data);
	display_string_8x16(DISPLAYMODE,3,76,"MS");
}
void Page32(u16	value)
{
	u8 data[3];
	DECToText(data,value);
	WindowClear(DISPLAYMODE);
	Display_GBK12X12(DISPLAYMODE,0,40,"����ʱ��");	
	display_string_8x16(DISPLAYMODE,3,50,data);
	Display_One_8x16(DISPLAYMODE,3,76,'S');
}
void Page33_TimeShow(u8 line,u8 showline)
{
	u8 string[3];
	display_string_5x8(DISPLAYMODE,showline,26,"20");
	string[0]=(uCoverTime[line][0]>>4)+0x30;
	string[1]=(uCoverTime[line][0]&0x0f)+0x30;
	string[2]=0;
	display_string_5x8(DISPLAYMODE,showline,38,string);
	display_string_5x8(DISPLAYMODE,showline,50,"-");
	string[0]=(uCoverTime[line][1]>>4)+0x30;
	string[1]=(uCoverTime[line][1]&0x0f)+0x30;
	string[2]=0;
	display_string_5x8(DISPLAYMODE,showline,56,string);
	display_string_5x8(DISPLAYMODE,showline,68,"-");
	string[0]=(uCoverTime[line][2]>>4)+0x30;
	string[1]=(uCoverTime[line][2]&0x0f)+0x30;
	string[2]=0;
	display_string_5x8(DISPLAYMODE,showline,74,string);
	display_string_5x8(DISPLAYMODE,showline,86," ");
	string[0]=(uCoverTime[line][3]>>4)+0x30;
	string[1]=(uCoverTime[line][3]&0x0f)+0x30;
	string[2]=0;
	display_string_5x8(DISPLAYMODE,showline,92,string);
	display_string_5x8(DISPLAYMODE,showline,104,":");
	string[0]=(uCoverTime[line][4]>>4)+0x30;
	string[1]=(uCoverTime[line][4]&0x0f)+0x30;
	string[2]=0;
	display_string_5x8(DISPLAYMODE,showline,110,string);
}
void Page33(u8 line)
{
	WindowClear(DISPLAYMODE);
	Display_GBK12X12(DISPLAYMODE,0,4,"��������");
	switch(line)
	{
		case 0:
			display_graphic_16x16(DISPLAYMODE,2,0,arrow);
			display_string_5x8(DISPLAYMODE,2,15,"1.");
			display_string_5x8(DISPLAYMODE,4,15,"2.");
			display_string_5x8(DISPLAYMODE,6,15,"3.");
			if(RecoverDataPos&1)Page33_TimeShow(0,2);
			else display_string_5x8(DISPLAYMODE,2,26,"____-__-__ __:__");
			if(RecoverDataPos&2)Page33_TimeShow(1,4);
			else display_string_5x8(DISPLAYMODE,4,26,"____-__-__ __:__");
			if(RecoverDataPos&4)Page33_TimeShow(2,6);
			else display_string_5x8(DISPLAYMODE,6,26,"____-__-__ __:__");
			break;
		case 1:
			display_graphic_16x16(DISPLAYMODE,4,0,arrow);
			display_string_5x8(DISPLAYMODE,2,15,"1.");
			display_string_5x8(DISPLAYMODE,4,15,"2.");
			display_string_5x8(DISPLAYMODE,6,15,"3.");
			if(RecoverDataPos&1)Page33_TimeShow(0,2);
			else display_string_5x8(DISPLAYMODE,2,26,"____-__-__ __:__");
			if(RecoverDataPos&2)Page33_TimeShow(1,4);
			else display_string_5x8(DISPLAYMODE,4,26,"____-__-__ __:__");
			if(RecoverDataPos&4)Page33_TimeShow(2,6);
			else display_string_5x8(DISPLAYMODE,6,26,"____-__-__ __:__");
			break;
		case 2:
			display_graphic_16x16(DISPLAYMODE,6,0,arrow);
			display_string_5x8(DISPLAYMODE,2,15,"1.");
			display_string_5x8(DISPLAYMODE,4,15,"2.");
			display_string_5x8(DISPLAYMODE,6,15,"3.");
			if(RecoverDataPos&1)Page33_TimeShow(0,2);
			else display_string_5x8(DISPLAYMODE,2,26,"____-__-__ __:__");
			if(RecoverDataPos&2)Page33_TimeShow(1,4);
			else display_string_5x8(DISPLAYMODE,4,26,"____-__-__ __:__");
			if(RecoverDataPos&4)Page33_TimeShow(2,6);
			else display_string_5x8(DISPLAYMODE,6,26,"____-__-__ __:__");
			break;
		case 3:
			display_graphic_16x16(DISPLAYMODE,6,0,arrow);
			display_string_5x8(DISPLAYMODE,2,15,"2.");
			display_string_5x8(DISPLAYMODE,4,15,"3.");
			display_string_5x8(DISPLAYMODE,6,15,"4.");
			if(RecoverDataPos&2)Page33_TimeShow(1,2);
			else display_string_5x8(DISPLAYMODE,2,26,"____-__-__ __:__");
			if(RecoverDataPos&4)Page33_TimeShow(2,4);
			else display_string_5x8(DISPLAYMODE,4,26,"____-__-__ __:__");
			if(RecoverDataPos&8)Page33_TimeShow(3,6);
			else display_string_5x8(DISPLAYMODE,6,26,"____-__-__ __:__");
			break;
		case 4:
			display_graphic_16x16(DISPLAYMODE,6,0,arrow);
			display_string_5x8(DISPLAYMODE,2,15,"3.");
			display_string_5x8(DISPLAYMODE,4,15,"4.");
			display_string_5x8(DISPLAYMODE,6,15,"5.");
			if(RecoverDataPos&4)Page33_TimeShow(2,2);
			else display_string_5x8(DISPLAYMODE,2,26,"____-__-__ __:__");
			if(RecoverDataPos&8)Page33_TimeShow(3,4);
			else display_string_5x8(DISPLAYMODE,4,26,"____-__-__ __:__");
			if(RecoverDataPos&0x10)Page33_TimeShow(4,6);
			else display_string_5x8(DISPLAYMODE,6,26,"____-__-__ __:__");
			break;
	}
}
void Page34(u8 line)
{
//extern u8 uCoverTime[3][5];
	
	WindowClear(DISPLAYMODE);
	if(bOfflineModeFlag==0)
	Display_GBK12X12(DISPLAYMODE,0,4,"������ԭ");
	else Display_GBK12X12(DISPLAYMODE,0,4,"�����鿴");
		switch(line)
	{
		case 0:
			display_graphic_16x16(DISPLAYMODE,2,0,arrow);
			display_string_5x8(DISPLAYMODE,2,15,"1.");
			display_string_5x8(DISPLAYMODE,4,15,"2.");
			display_string_5x8(DISPLAYMODE,6,15,"3.");
			if(RecoverDataPos&1)Page33_TimeShow(0,2);
			else display_string_5x8(DISPLAYMODE,2,26,"____-__-__ __:__");
			if(RecoverDataPos&2)Page33_TimeShow(1,4);
			else display_string_5x8(DISPLAYMODE,4,26,"____-__-__ __:__");
			if(RecoverDataPos&4)Page33_TimeShow(2,6);
			else display_string_5x8(DISPLAYMODE,6,26,"____-__-__ __:__");
			break;
		case 1:
			display_graphic_16x16(DISPLAYMODE,4,0,arrow);
			display_string_5x8(DISPLAYMODE,2,15,"1.");
			display_string_5x8(DISPLAYMODE,4,15,"2.");
			display_string_5x8(DISPLAYMODE,6,15,"3.");
			if(RecoverDataPos&1)Page33_TimeShow(0,2);
			else display_string_5x8(DISPLAYMODE,2,26,"____-__-__ __:__");
			if(RecoverDataPos&2)Page33_TimeShow(1,4);
			else display_string_5x8(DISPLAYMODE,4,26,"____-__-__ __:__");
			if(RecoverDataPos&4)Page33_TimeShow(2,6);
			else display_string_5x8(DISPLAYMODE,6,26,"____-__-__ __:__");
			break;
		case 2:
			display_graphic_16x16(DISPLAYMODE,6,0,arrow);
			display_string_5x8(DISPLAYMODE,2,15,"1.");
			display_string_5x8(DISPLAYMODE,4,15,"2.");
			display_string_5x8(DISPLAYMODE,6,15,"3.");
			if(RecoverDataPos&1)Page33_TimeShow(0,2);
			else display_string_5x8(DISPLAYMODE,2,26,"____-__-__ __:__");
			if(RecoverDataPos&2)Page33_TimeShow(1,4);
			else display_string_5x8(DISPLAYMODE,4,26,"____-__-__ __:__");
			if(RecoverDataPos&4)Page33_TimeShow(2,6);
			else display_string_5x8(DISPLAYMODE,6,26,"____-__-__ __:__");
			break;
		case 3:
			display_graphic_16x16(DISPLAYMODE,6,0,arrow);
			display_string_5x8(DISPLAYMODE,2,15,"2.");
			display_string_5x8(DISPLAYMODE,4,15,"3.");
			display_string_5x8(DISPLAYMODE,6,15,"4.");
			if(RecoverDataPos&2)Page33_TimeShow(1,2);
			else display_string_5x8(DISPLAYMODE,2,26,"____-__-__ __:__");
			if(RecoverDataPos&4)Page33_TimeShow(2,4);
			else display_string_5x8(DISPLAYMODE,4,26,"____-__-__ __:__");
			if(RecoverDataPos&8)Page33_TimeShow(3,6);
			else display_string_5x8(DISPLAYMODE,6,26,"____-__-__ __:__");
			break;
		case 4:
			display_graphic_16x16(DISPLAYMODE,6,0,arrow);
			display_string_5x8(DISPLAYMODE,2,15,"3.");
			display_string_5x8(DISPLAYMODE,4,15,"4.");
			display_string_5x8(DISPLAYMODE,6,15,"5.");
			if(RecoverDataPos&4)Page33_TimeShow(2,2);
			else display_string_5x8(DISPLAYMODE,2,26,"____-__-__ __:__");
			if(RecoverDataPos&8)Page33_TimeShow(3,4);
			else display_string_5x8(DISPLAYMODE,4,26,"____-__-__ __:__");
			if(RecoverDataPos&0x10)Page33_TimeShow(4,6);
			else display_string_5x8(DISPLAYMODE,6,26,"____-__-__ __:__");
			break;
	}
}
void Page35(u16	value)
{
	u8 data[4];
	DECToText(data,value);
	WindowClear(DISPLAYMODE);
	Display_GBK12X12(DISPLAYMODE,0,4,"��������");	
	display_string_8x16(DISPLAYMODE,3,60,data);
	
}
void Page36(u16	UV,u16 VW,u8 line)
{
		u8 scrdata[4],maindata[4];
	u8 select;
	u8 noselect;
	if(DISPLAYMODE==0)
	{
		select=1;
		noselect=0;
	}
	else
	{
		select=0;
		noselect=1;
	}
	DECToText(scrdata,UV);
	DECToText(maindata,VW);
	WindowClear(DISPLAYMODE);
	Display_GBK12X12(DISPLAYMODE,0,40,"��ѹУ׼");	
	DisplayString(DISPLAYMODE,4,20,"UV");
	DisplayString(DISPLAYMODE,6,20,"VW");	
	if(line==0)
	{
		display_string_8x16(select,4,76,scrdata);
		display_string_8x16(noselect,6,76,maindata);
	}
	else if(line==1)
	{
		display_string_8x16(noselect,4,76,scrdata);
		display_string_8x16(select,6,76,maindata);
	}
	
}
void	CreatTopicWindow(void)
{
	u16 i,j;
	//���
	for(j=0;j<6;j++)
	{
		wrcomL(0xb8+j+1);
		wrcomL(0x40+7);
		if(j==5) 
		{
			if(DISPLAYMODE==0)wrdataL(0x3f);
			else wrdataL(0xC0);
		}
		else
		{
			if(DISPLAYMODE==0)wrdataL(0xff);
			else wrdataL(0x00);
		}
	}
	//�ҿ�
	for(j=0;j<6;j++)
	{
		wrcomR(0xb8+j+1);
		wrcomR(0x40+56);
		if(j==5)
		{
			if(DISPLAYMODE==0)wrdataR(0x3f);
			else wrdataR(0xC0);
		}
		else 
		{
			if(DISPLAYMODE==0)wrdataR(0xff);
			else wrdataR(0x00);
		}
	}
	//�Ͽ�
	wrcomL(0xb8+1);wrcomR(0xb8+1);
	wrcomL(0x40+8);wrcomR(0x40+0);	
	for (i=0;i<56;i++)
	{
		if(DISPLAYMODE==0)
		{
			wrdataL(0x01); 
			wrdataR(0x01); 
		}
		else
		{
			wrdataL(0xfe); 
			wrdataR(0xfe); 
		}
	}
	//�¿�
	wrcomL(0xb8+6);wrcomR(0xb8+6);
	wrcomL(0x40+8);wrcomR(0x40+0);	
	for (i=0;i<56;i++)
	{
		if(DISPLAYMODE==0)
		{
			wrdataL(0x20); 
			wrdataR(0x20);  
		}
		else
		{
			wrdataL(0xdf); 
			wrdataR(0xdf); 
		}
	}
	/**���**/
	for(j=0;j<4;j++)
	{
		wrcomR(0xb8+j+2);
		wrcomR(0x40);
		
		wrcomL(0xb8+j+2);
		wrcomL(0x40+8);
		for (i=0;i<56;i++)
		{
			if(DISPLAYMODE==0)
			{
				wrdataL(0x00); 
				wrdataR(0x00);  
			}
			else
			{
				wrdataL(0xff); 
				wrdataR(0xff); 
			}
		}
	}
//	Display_GBK12X12(0,2,10,"����ʱ��");	
//	Display_GBK12X12(0,4,10,"����ʱ��");	
}
void SettingUpDisplay(u8 pos)
{
	CreatTopicWindow();
	switch (pos)
	{
		case 0:
			Display_GBK12X12(DISPLAYMODE,3,38,"����ʧ��");	
			break;
		case 1:
			Display_GBK12X12(DISPLAYMODE,3,38,"�������");	
			break;
		case 0xf0:
			Display_GBK12X12(DISPLAYMODE,3,45,"������");	
			break;
		case 0xf1:
			Display_GBK12X12(DISPLAYMODE,3,38,"��ֹ����");	
			break;
		case 0x0f:
			Display_GBK12X12(DISPLAYMODE,3,38,"�������");	
			break;
	}
}
void ErrorDisplay(u8 line)
{
	u8 i;
	u8 list[4]={0,'.',0,0};
	u8 errorpos[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	if(uErrorOld!=uMachineStatusDatas.ErrorListL)
	{
		uErrorOld=uMachineStatusDatas.ErrorListL;
		uErrorNum=0;
		CreatTopicWindow();
		for(i=0;i<16;i++)
		{
			if(uMachineStatusDatas.ErrorListL&(1<<i))
			{
				errorpos[uErrorNum]=i;
				uErrorNum++;
			}
		}
		if(uErrorNum>2)
		{
			if(line+2<uErrorNum)
			display_graphic_12x12(DISPLAYMODE,4,107,ArrowDown);
			if(line>0)display_graphic_12x12(DISPLAYMODE,2,107,ArrowUp);
			if((line+1)<10)
			{
				list[0]=line+0x31;
				list[1]='.';
				list[2]=0;
				display_string_5x8(DISPLAYMODE,2,20,list);
			}
			else
			{
				list[0]=((line+1)/10)+0X30;
				list[1]=((line+1)%10)+0X30;
				list[2]='.';
				list[3]=0;
				display_string_5x8(DISPLAYMODE,2,14,list);
			}
			if((line+2)<10)
			{
				list[0]=line+0x32;
				list[1]='.';
				list[2]=0;
				display_string_5x8(DISPLAYMODE,4,20,list);
			}
			else
			{
				list[0]=((line+2)/10)+0X30;
				list[1]=((line+2)%10)+0X30;
				list[2]='.';
				list[3]=0;
				display_string_5x8(DISPLAYMODE,4,14,list);
			}
			DisplayString(DISPLAYMODE,2,32,ErrorDisplayContenr[errorpos[line]]);
			DisplayString(DISPLAYMODE,4,32,ErrorDisplayContenr[errorpos[line+1]]);
		}
		else if(uErrorNum==2)
		{
			display_string_5x8(DISPLAYMODE,2,20,"1.");
			display_string_5x8(DISPLAYMODE,4,20,"2.");
			DisplayString(DISPLAYMODE,2,32,ErrorDisplayContenr[errorpos[0]]);
			DisplayString(DISPLAYMODE,4,32,ErrorDisplayContenr[errorpos[1]]);
		}
		else
		{
			display_string_5x8(DISPLAYMODE,3,20,"1.");
			DisplayString(DISPLAYMODE,3,32,ErrorDisplayContenr[errorpos[0]]);
		}
		bErrorShowedFlag=1;
	}
}
void RTCUpdata(void)
{
	u8 data[6];
	if(uMachineStatusDatas.ErrorListL==0)
	if(uUIPageNum==1)
	{
		data[0]=(sysTime.hour>>4)+0x30;
		data[1]=(sysTime.hour&0x0f)+0x30;
		data[2]=':';
		data[3]=(sysTime.minute>>4)+0x30;
		data[4]=(sysTime.minute&0x0f)+0x30;
		data[5]=0;
		display_string_5x8(DISPLAYMODE,0,95,data);
	}
}
void UIDisplay(void)
{
	if(bDisplayFlag)
	{
		bDisplayFlag=0;
		if(uSettingUpFlag==0xff)
		{
			switch(uUIPageNum)
			{
				case 0:
					Page0();
					break;
				case 1:
					if(uMachineStatusDatas.ErrorListL==0)
					{
						uErrorOld=0;
						Page1();
						RTCUpdata();
					}
					else
					ErrorDisplay(uUILineNum);
					break;
				case 2:
					bStatusDataShowFlag=0;
					Page2(uUILineNum);
					break;
				case 3:
					Page3(uUILineNum);
					break;
				case 4:
					Page4(&uUILineNum);
					break;
				case 5:
					Page5(uUILineNum);
					break;
				case 6:
					Page6(uUILineNum);
					break;
				case 7:
					Page7();
					break;
				case 8:
					Page8(uUILineNum);
					break;
				case 9:
					Page9(uUILineNum);
					break;
				case 10:
					Page10(uKeyPressData,uUILineNum);
					break;
				case 11:
					Page11(uUILineNum);
					break;
				case 12:
					Page12(uSetDataValuel,uSetDataValuel2,uUILineNum);
					break;
				case 13:
					Page13(uSetDataValuel);
					break;
				case 14:
					Page14(uSetDataValuel);
					break;
				case 15:
					Page15(uSetDataValuel);
					break;
				case 16:
					Page16(uSetDataValuel,uSetDataValuel2,uUILineNum);
					break;
				case 17:
//					Page17(uSetDataValuel,uSetDataValuel2,uUILineNum);
					break;
				case 18:
					Page18(uUILineNum);
					break;
				case 19:
					Page19(uUILineNum);
					break;
				case 20:
					Page20(uUILineNum,time_init);
					break;
				case 21:
					Page21(uUILineNum);
					break;
				case 22:
					Page22(uUILineNum,uErrorEnableFlag);
					break;
				case 23:
					Page23(uSetDataValuel);
					break;
				case 24:
					Page24(uSetDataValuel);
					break;
				case 25:
					Page25(uUILineNum,uSetDataValuel);
					break;
				case 26:
					Page26();
					break;
				case 27:
					Page27(uUILineNum);
					break;
				case 28:
					Page28(uSetDataValuel);
					break;
				case 29:
					Page29(uSetDataValuel);
					break;
				case 30:
					Page30(uSetDataValuel);
					break;
				case 31:
					Page31(uSetDataValuel);
					break;
				case 32:
					Page32(uSetDataValuel);
					break;
				case 33:
					Page33(uUILineNum);
					break;
				case 34:
					Page34(uUILineNum);
					break;
				case 35:
					Page35(uSetDataValuel);
					break;
				case 36:
					Page36(uSetDataValuel,uSetDataValuel2,uUILineNum);
					break;
			}
			if(uUIPageNum>1)
			{
				if(bOfflineModeFlag)Display_GBK12X12(DISPLAYMODE,0,100,"����");
				else if(bFactoryModeFlag)Display_GBK12X12(DISPLAYMODE,0,100,"����");
			}
		}
		else if((uSettingUpFlag !=0xff))
		SettingUpDisplay(uSettingUpFlag);
	}
}

