#include "bsp_usart_dma.h"

uint8_t SendBuff[SENDBUFF_SIZE];
uint8_t RevdBuff[REVBUFF_SIZE];
bool bCMDSendFlag=0;
bool	bREVEndFlag=0;
u16  Usart1_Rec_Cnt=0; 
void	Uart2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	// 打开串口GPIO的时钟
	DEBUG_USART_GPIO_APBxClkCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// 打开串口外设的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

  // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = 115200;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(USART2, &USART_InitStructure);
	
	// 串口中断优先级配置
//	NVIC_Configuration();
  
  /* 嵌套向量中断控制器组选择 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  /* 抢断优先级*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  /* 子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
	
	// 使能串口接收中断
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	
	
	// 使能串口
	USART_Cmd(USART2, ENABLE);		

  // 清除发送完成标志
//  USART_ClearFlag(USART1, USART_FLAG_TC);     
}
static void RS422_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*开启按键端口的时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//选择按键的引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	//使用结构体初始化按键
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	GPIO_SetBits(GPIOB,GPIO_Pin_13);
}
/**
  * @brief  USART GPIO 配置,工作参数配置
  * @param  无
  * @retval 无
  */
static void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	// 打开串口GPIO的时钟
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	
	// 打开串口外设的时钟
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);
// 开启DMA时钟
//		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级 
	NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USARTx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(DEBUG_USARTx, &USART_InitStructure);	
	
	USART_ITConfig(DEBUG_USARTx, USART_IT_IDLE, ENABLE);//开启空闲中断
//	USART_DMACmd(DEBUG_USARTx,USART_DMAReq_Rx,ENABLE);   //使能串口 DMA接收
	// 使能串口
	USART_Cmd(DEBUG_USARTx, ENABLE);	    
}
/*****************  发送一个字节 **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/****************** 发送8位的数组 ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
  uint8_t i;
	
	for(i=0; i<num; i++)
  {
	    /* 发送一个字节数据到USART */
	    Usart_SendByte(pUSARTx,array[i]);	
  
  }
	/* 等待发送完成 */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

/*****************  发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* 等待发送完成 */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/*****************  发送一个16位数 **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	/* 取出高八位 */
	temp_h = (ch&0XFF00)>>8;
	/* 取出低八位 */
	temp_l = ch&0XFF;
	
	/* 发送高八位 */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	/* 发送低八位 */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口 */
		USART_SendData(DEBUG_USARTx, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USARTx);
}

/**
  * @brief  USARTx TX DMA 配置，内存到外设(USART1->DR)
  * @param  无
  * @retval 无
  */
static void USARTx_DMA_Config(void)
{
		DMA_InitTypeDef DMA_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		// 开启DMA时钟
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		// 设置DMA源地址：串口数据寄存器地址*/
    DMA_InitStructure.DMA_PeripheralBaseAddr = USART_DR_ADDRESS;
		// 内存地址(要传输的变量的指针)
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)SendBuff;
		// 方向：从内存到外设	
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
		// 传输大小	
		DMA_InitStructure.DMA_BufferSize = 0;
		// 外设地址不增	    
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		// 内存地址自增
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		// 外设数据单位	
		DMA_InitStructure.DMA_PeripheralDataSize = 
	  DMA_PeripheralDataSize_Byte;
		// 内存数据单位
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 
		// DMA模式，一次或者循环模式
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
		//DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	
		// 优先级：中	
		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; 
		// 禁止内存到内存的传输
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		// 配置DMA通道		   
		DMA_Init(USART_TX_DMA_CHANNEL, &DMA_InitStructure);		
		DMA_ITConfig(USART_TX_DMA_CHANNEL,DMA_IT_TC,ENABLE);   //发送完成产生中断
		// 使能DMA
		DMA_Cmd (USART_TX_DMA_CHANNEL,DISABLE);
		
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	 
	 
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级3
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	  NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
		
/****USART 接收配置***/
		// 开启DMA时钟
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		DMA_DeInit(USART_RX_DMA_CHANNEL);
		// 设置DMA源地址：串口数据寄存器地址*/
    DMA_InitStructure.DMA_PeripheralBaseAddr = USART_DR_ADDRESS;
		// 内存地址(要传输的变量的指针)
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)RevdBuff;
		// 方向：从内存到外设	
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
		// 传输大小	
		DMA_InitStructure.DMA_BufferSize = REVBUFF_SIZE;
		// 外设地址不增	    
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		// 内存地址自增
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		// 外设数据单位	
		DMA_InitStructure.DMA_PeripheralDataSize = 
	  DMA_PeripheralDataSize_Byte;
		// 内存数据单位
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 
		// DMA模式，一次或者循环模式
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
//		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	
		// 优先级：中	
		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; 
		// 禁止内存到内存的传输
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		// 配置DMA通道		   
		DMA_Init(USART_RX_DMA_CHANNEL, &DMA_InitStructure);		
//		DMA_ITConfig(USART_RX_DMA_CHANNEL,DMA_IT_TC,ENABLE);   //发送完成产生中断
		// 使能DMA
		DMA_Cmd (USART_RX_DMA_CHANNEL,ENABLE);
		USART_DMACmd(DEBUG_USARTx,USART_DMAReq_Rx,ENABLE);   //使能串口x DMA接收
}
void USARTx_DMA_Init(void)
{
	RS422_IO_Init();
	USART_Config();
	USARTx_DMA_Config();
}
void CMD_Send(uint16_t size)
{
	DMA_Cmd (USART_TX_DMA_CHANNEL,DISABLE);
	USART_TX_DMA_CHANNEL->CMAR =  (u32)SendBuff;
	USART_TX_DMA_CHANNEL->CNDTR = size;
	DMA_Cmd (USART_TX_DMA_CHANNEL,ENABLE); 
	USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Tx, ENABLE);
	bCMDSendFlag=1;
}
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{ 
	DMA_Cmd(DMA_CHx, DISABLE );  //关闭USART1 TX DMA1 所指示的通道      
 	DMA_SetCurrDataCounter(DMA_CHx,REVBUFF_SIZE);//DMA通道的DMA缓存的大小
 	DMA_Cmd(DMA_CHx, ENABLE);  //使能USART1 TX DMA1 所指示的通道 
}	
void DMA1_Channel4_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA1_FLAG_TC4)==SET) 
	{  
		DMA_ClearITPendingBit(DMA1_IT_GL4);
		DMA_ClearFlag(DMA1_FLAG_TC4);
		DMA_Cmd (DMA1_Channel4,DISABLE);
		bCMDSendFlag=0;
	}       
}
//串口中断函数
void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	if(USART_GetITStatus(DEBUG_USARTx, USART_IT_IDLE) != RESET)  //接收中断
		{
		  USART_ReceiveData(DEBUG_USARTx);//读取数据 注意：这句必须要，否则不能够清除中断标志位。我也不知道为啥！
 		  Usart1_Rec_Cnt = REVBUFF_SIZE-DMA_GetCurrDataCounter(USART_RX_DMA_CHANNEL);	//算出接本帧数据长度
			USART_ClearITPendingBit(DEBUG_USARTx, USART_IT_IDLE);         //清除中断标志
			MYDMA_Enable(USART_RX_DMA_CHANNEL);                   //恢复DMA指针，等待下一次的接收
			bREVEndFlag=1;
    } 
} 
