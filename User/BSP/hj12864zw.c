//深圳绘晶科技有限公司、
//12864点阵中文字库,单片机:89S52, 晶振:12M,
//STC单片机完全兼容
//串并行共用程序
//sbit REST=P2^1;
//sbit RS=P3^1;//串口时为CS
//sbit RW=P3^4; //串口为SID
//sbit E1=P3^5; //串口为时钟SCLK
//sbit PSB=P2^0;//并口时，PSB=1;串口时，PSB=0
//sbit stop=P3^2;//低电平触发
typedef unsigned int Uint;
typedef unsigned char Uchar;
Uchar z,z1,d,d1,s,s1,s10,s100,m1;
//汉字,直接可以写入字形,写入标点符号后要加空格键
unsigned char const uctech[] = {"绘晶科技公司地址区石岩镇麻布第二"};
//显示在第1，3行
unsigned char const uctech3[] = {"广东省深圳市宝安工业区第4 栋2 楼"};
//显示在第2，4行
unsigned char const uctech6[] = {"汇 精焦 128*64 至"};
//显示在第1，3行
unsigned char const uctech7[] = {"聚 绘晶科技 品点 23146001 上"};
//显示在第2，4行
unsigned char const uctech1[] = {"绘晶科技"};
//显示在第2行
unsigned char const uctech2[] = {" 128*64 "};
//显示在第3行
unsigned char const uctech4[] = {"TIME"};
unsigned char const uctech5[] = {"绘晶科技23146001"};
unsigned char const uctech8[] = {"有8192个中文字型"};
unsigned char const uctech9[] = {"有126 个字母符号"};
//这个是在串口时指令和数据之间的延时
/*
void delay10US(Uchar x)
{
Uchar k;
for(k=0;k<x;k++);
}
*/
const Uchar delay=250; //延时时间常数
static void Wait1ms(void)//延迟1 ms
{
	Uchar cnt=0;
	while (cnt<delay) cnt++;
}
//延迟n ms
void WaitNms(int n)
{
	Uchar i;
	for(i=1;i<=n;i++)
	Wait1ms();
}
//**************************************************************************//
//以下是并口时才开的
//读忙标志,
void RDBF(void)
{
	Uchar temp;
	RS=0; // RS=0
	RW=1; // RW=1
	while(1)
	{
		P1=0xFF; //数据线为输入
		E1=1;
		temp=P1;
		E1=0; // E=0
		if ((temp&0x80)==0) break;
	}
}
//写数据到指令寄存器
void WRCommandH(Uchar comm)
{
	RDBF();
	RS=0;
	RW=0;
	P1=comm;
	E1=1;
	E1=0;
}
//写数据到数据寄存器
void WRDataH(Uchar TEMP)
{
	RDBF();
	RS=1;
	RW=0;
	P1=TEMP;
	E1=1;
	E1=0;
}
/////////////////////////////////////////////////////////////////////////////////
//以下是串口时开的读写时序
/*
void SendByteLCDH(Uchar WLCDData)
{
Uchar i;
for(i=0;i<8;i++)
{
if((WLCDData<<i)&0x80)RW=1;
else RW=0;
E1=0;
E1=1 ;
}
}
SPIWH(Uchar Wdata,Uchar WRS)
{
SendByteLCDH(0xf8+(WRS<<1));//寄存器选择WRS
SendByteLCDH(Wdata&0xf0);
SendByteLCDH((Wdata<<4)&0xf0);
}
void WRCommandH(Uchar CMD)
{
RS=0;
RS=1;
SPIWH(CMD,0);
delay10US(90);//89S52来模拟串行通信,所以,加上89S52的延时,
}
void WRDataH(Uchar Data)
{
RS=0;
RS=1;
SPIWH(Data,1);
}
*/
/*******************************************************************/
//初始化LCD-8位接口
void LCDInit(void)
{ 
	//PSB=0; //串口
	PSB=1;//并口时选这个,上一行取消
	// REST=1;
	// REST=0;
	// REST=1;
	WRCommandH(0x30); //基本指令集,8位并行
	WRCommandH(0x06); //启始点设定：光标右移
	WRCommandH(0x01); //清除显示DDRAM
	WRCommandH(0x0C); //显示状态开关：整体显示开，光标显示关，光标显示反白关
	WRCommandH(0x02); //地址归零
}
//addr为汉字显示位置,*hanzi汉字指针;count为输入汉字串字符数
void ShowQQCharH(Uchar addr,Uchar *hanzi,Uchar count)
{
	Uchar i;
	WRCommandH(addr); //设定DDRAM地址
	for(i=0;i<count;)
	{
		WRDataH(hanzi[i*2]);
		WRDataH(hanzi[i*2+1]);
		i++;
	}
}
//addr为半宽字符首个地址,i为首个半宽字符代码,count为需要输入字符个数
void ShowNUMCharH(Uchar addr,Uchar i,Uchar count)
{
	Uchar j;
	for(j=0;j<count;)
	{
		WRCommandH(addr); //设定DDRAM地址
		WRDataH(i+j);//必为两个16*8位字符拼成一个16*16才能显示
		j++;
		WRDataH(i+j);
		addr++;
		j++;
	}
}
//自定义字符写入CGRAM
//data1是高八位,data2是低八位,一存必须存两个字节,横向存两字节,后纵向累加,共16行
//一个自定义字符为16*16点阵
//第一个存入字节为从40H开始,到4F结束为第一个自定义汉字符, 之后调出地址从8000H为始第
一个
//addr为存入头地址
void WRCGRAMH(Uchar data1,Uchar data2,Uchar addr)
{
	Uchar i;
	for(i=0;i<16;)
	{
		WRCommandH(addr+i); //设定CGRAM地址
		WRDataH(data1);
		WRDataH(data1);
		i++;
		WRCommandH(addr+i); //设定CGRAM地址
		WRDataH(data2);
		WRDataH(data2);
		i++;
	}
}
//自定义字符写入CGRAM
//显示上半屏自定义的字符,并把这个字符填满全屏16*16
//addr为显示地址,把自定义字符当一个汉字调出,从8000H开始为第一个,
//8100H为第二个,8200H为第三个,8300H为第四个,中文字库只能自定义四个字符
//i为自定义字符调出地址,先输入低位,再输入高位
//IC决定,中文字库类,一个IC最多只能显示16*2个汉字
void ShowCGCharH(Uchar addr,Uchar i)
{
	Uchar j;
	for(j=0;j<0x20;)
	{
		WRCommandH(addr+j); //设定DDRAM地址
		WRDataH(0x00);//字符地址低八位
		WRDataH(i);//字符地址高八位
		j++;
	}
}
void WRGDRAM128X8(Uchar x1,Uchar y1,Uchar d1 )
{
	Uchar j,i;
	WRCommandH(0x34); //去扩展指令寄存器
	WRCommandH(0x36); //打开绘图功能
	for(j=0;j<16;j++) //
	{
		WRCommandH(0x80+y1+j); //Y总坐标,即第几行
		WRCommandH(0x80+x1); //X坐标，即横数第几个字节开始写起,80H为第一个		字节
		for(i=0;i<8;i++) //写入一行
		{
			WRDataH(d1);
			WRDataH(d1);
		}
	}
}
//上半屏清除图形区数据
void CLEARGDRAMH(Uchar c)
{
	Uchar j;
	Uchar i;
	WRCommandH(0x34);
	WRCommandH(0x36);
	for(j=0;j<32;j++)
	{
		WRCommandH(0x80+j);
		WRCommandH(0x80);//X坐标
		for(i=0;i<16;i++)//
		{
			WRDataH(c);
			WRDataH(c);
		}
	}
}
void WRGDRAM1(Uchar x,Uchar l,Uchar r )
{
	Uchar j;
	Uchar i;
	WRCommandH(0x34); //去扩展指令寄存器
	WRCommandH(0x36); //打开绘图功能
	//两横的上边框 下边框
	for(j=0;j<2;j++) //2行
	{
		WRCommandH(0x80+j); //Y总坐标,即第几行
		WRCommandH(0x80); //X坐标，即横数第几个字节开始写起,80H为第一个字节
		for(i=0;i<8;i++) //写入一行
		{
			WRDataH(x);
			WRDataH(x);
		}
		WRCommandH(0x80+30+j); //Y总坐标,即第几行
		WRCommandH(0x80+8); //X坐标，即横数第几个字节开始写起,80H为第一个字节
		for(i=0;i<8;i++) //写入一行
		{
			WRDataH(x);
			WRDataH(x);
		}
	}
	//上半屏两横的右左边框
	for(j=2;j<32;j++) //30行 左
	{ //先上半屏
		WRCommandH(0x80+j); //Y总坐标,即第三行开始
		WRCommandH(0x80); //X坐标，即横数第几个字节开始写起,80H为第一个字节
		WRDataH(l);
		WRDataH(0x00);
		WRCommandH(0x80+j); //Y总坐标,即第三行开始
		WRCommandH(0x80+7); //X坐标，即横数第几个字节开始写起,80H为第一个字节
		WRDataH(0x00);WRDataH(r);
	}
	//下半屏两横右左的边框
	for(j=0;j<30;j++) //30行
	{ //
		WRCommandH(0x80+j); //Y总坐标,即第一行开始
		WRCommandH(0x80+15); //X坐标，即横数第几个字节开始写起,80H为第一个字节
		WRDataH(0x00);
		WRDataH(r);
		WRCommandH(0x80+j); //Y总坐标,即第几行
		WRCommandH(0x80+8); //X坐标，即横数第几个字节开始写起,80H为第一个字节
		WRDataH(l); WRDataH(0x00);
	}
}
//P3.2按键中断
void ini_int1(void)
{
	EA=1;
	EX0=1;//允许外部INT0的中断
	IT0=1;// 允许中断
}
int scankey1() interrupt 0 using 3 //使用外部中断1,寄存器组3
{
	while(P3^2==0)
	{
		for(;;)
		{;}
	}
	IE1=0;//中断标志清零
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~程序从这里开始
//主函数
void TEST_SHOW(void)
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~测试
	for(m1=0;m1<50;m1++)
	{
		ini_int1();//开中断
		WaitNms(250);
		LCDInit();//初始化
		ShowNUMCharH(0x80,0x01,32);//显示半宽特殊符号
		ShowNUMCharH(0x90,0x30,32);//显示半宽0~?数字标点
		WaitNms(250); //等待时间
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~1，显示8*16字符
		LCDInit();//初始化
		WRCommandH(0x01); //清除显示DDRAM
		WRCGRAMH(0xff,0x00,0x40);//写入横（自编特殊符号）
		WRCGRAMH(0x00,0xff,0x50);//写入横2
		WRCGRAMH(0xaa,0xaa,0x60);//写入竖
		WRCGRAMH(0x55,0x55,0x70);//写入竖2
		ShowCGCharH(0x80,0x00);//显示横并填满
		WaitNms(250); //等待时间
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~2，隔横显示
		WRCommandH(0x01); //清除显示DDRAM
		ShowCGCharH(0x80,02);//显示横2并填满
		WaitNms(250); //等待时间
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~3，隔横显示
		WRCommandH(0x01); //清除显示DDRAM
		ShowCGCharH(0x80,04);//显示竖并填满
		WaitNms(250); //等待时间
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~4，隔列显示
		WRCommandH(0x01); //清除显示DDRAM
		ShowCGCharH(0x80,06);//显示竖2并填满
		WaitNms(250); //等待时间
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~5，隔列显示
		WRCommandH(0x01); //清除显示DDRAM
		WRCGRAMH(0x00,0x00,0x40);//清CGRAM1（重复使用自编）
		WRCGRAMH(0x00,0x00,0x50);//清CGRAM2
		WRCGRAMH(0xaa,0x55,0x40);//写入点（自编特殊符号）
		WRCGRAMH(0x55,0xaa,0x50);//写入点2
		ShowCGCharH(0x80,00);//显示点并填满
		WaitNms(250); //等待时间
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~6，隔点显示
		WRCommandH(0x01); //清除显示DDRAM
		ShowCGCharH(0x80,02);//显示点2并填满
		WaitNms(250); //等待时间
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~7，隔点显示
		//显示汉字一屏
		LCDInit();//初始化
		ShowQQCharH(0x80,uctech,16);//调用字库
		ShowQQCharH(0x90,uctech3,16);
		CLEARGDRAMH(0x00); //清除显示绘图
		WRGDRAM128X8(0,0,0xff);//单独一行反白
		WaitNms(250); //等待时间
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~8，显示内部汉字
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~演示单行反白
		LCDInit();//初始化
		ShowQQCharH(0x92,uctech1,4);//显示'绘晶科技'
		ShowQQCharH(0x8A,uctech2,4);//显示' 128*64 '
		CLEARGDRAMH(0x00); //清除显示绘图
		WRGDRAM1(0xff,0xc0,0x03); //绘图演示-画边框
		WaitNms(250); //等待时间
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~8，显示图文混合
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~文字+边框
		LCDInit();//初始化
		ShowQQCharH(0x80,uctech6,16);//调用字库
		ShowQQCharH(0x90,uctech7,16);
		CLEARGDRAMH(0xff); //
		WaitNms(250); //等待时间
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~9，显示图文混合
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~文字+全显（反白）
	}
	//------------------------------------- -以下老化测试99小时
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~文字左移
	LCDInit();//初始化
	ShowQQCharH(0x80,uctech4,2);//调用字库
	ShowQQCharH(0x90,uctech9,8);//调用字库
	ShowQQCharH(0x88,uctech8,8);//调用字库
	ShowQQCharH(0x98,uctech5,8);//调用字库
	for(z=0;z<10;z++)
	{
		WRCommandH(0x80+2);//写地址
		WRDataH(0x3a);
		WRDataH(0x30+z); //分10
		for(z1=0;z1<10;z1++)
		{
			WRCommandH(0x80+3);//写地址
			WRDataH(0x30+z1); //分10
			WRDataH(0x3a);
			for(d=0;d<6;d++)
			{
				for(d1=0;d1<10;d1++)
				{
					WRCommandH(0x80+4);//写地址
					WRDataH(0x30+d); //分10
					WRDataH(0x30+d1); //分01
					// WRCommandH(0x10);
					for(s=0;s<6;s++)
					{
						WRCommandH(0x80+5);//写地址
						WRDataH(0x3a);
						WRDataH(0x30+s); //秒10
						for(s1=0;s1<10;s1++)
						{
							WRCommandH(0x80+6);//写地址
							WRDataH(0x30+s1); //秒01
							WRDataH(0x3a);
							WaitNms(5);////延时 x ms
							WRCommandH(0x18);
							for(s10=0;s10<10;s10++)
							{
								WaitNms(5);////延时 x ms
								for(s100=0;s100<10;s100++)
								{
									WRCommandH(0x80+7);//写地址
									WRDataH(0x30+s10);//100MS
									WRDataH(0x30+s100);//10MS
									WaitNms(5);////延时 x ms
								}
							}
						}
					}
				}
			}
		}
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~完~~~~~~~~~~~~~~~~~~~~~~~~






