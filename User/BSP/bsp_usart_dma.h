#ifndef __USARTDMA_H
#define	__USARTDMA_H
#include <stdbool.h>

#include "stm32f10x.h"
#include <stdio.h>


// 串口工作参数宏定义
#define  DEBUG_USARTx                   USART1
#define  DEBUG_USARTx_IRQn              USART1_IRQn
#define  DEBUG_USART_CLK                RCC_APB2Periph_USART1
#define  DEBUG_USART_APBxClkCmd         RCC_APB2PeriphClockCmd
#define  DEBUG_USART_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  DEBUG_USART_TX_GPIO_PORT       GPIOA   
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_9
#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_10

// 串口对应的DMA请求通道
#define  USART_TX_DMA_CHANNEL     DMA1_Channel4
#define  USART_RX_DMA_CHANNEL     DMA1_Channel5
// 外设寄存器地址
#define  USART_DR_ADDRESS        (USART1_BASE+0x04)
// 一次发送的数据量
#define  SENDBUFF_SIZE            100
#define  REVBUFF_SIZE            100

extern bool	bREVEndFlag;
void USARTx_DMA_Init(void);
void CMD_Send(uint16_t size);

#endif /* __USARTDMA_H */
