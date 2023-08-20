#include "usart.h"
#include "dma.h"
#include "stdlib.h"


extern uint8_t receiveBuffer[20];
volatile uint8_t receiveNewDataFalg=0;
volatile uint8_t receiveNum=0;

void usart_init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Medium_Speed;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;//设置波特率为115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//设置数据位为8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//设置停止位为1位
	USART_InitStructure.USART_Parity = USART_Parity_No;//设置无校验位
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//设置收发模式
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//设置无硬件流
	USART_Init(USART2, &USART_InitStructure);
    
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
    
	NVIC_InitTypeDef NVIC_InitStructure; 
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	  
    DMA1Init();
    DMA_Cmd(DMA1_Stream5, ENABLE);
    while(DMA_GetCmdStatus(DMA1_Stream5)==DISABLE);
    
    USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
    
	USART_Cmd(USART2, ENABLE);
}


void USART_SendString(USART_TypeDef* USARTx, uint8_t* string)
{
	while(*string != '\0')
	{	
		USART_ClearFlag(USARTx, USART_IT_TC);//每次发送的时候都给他手动清0
		USART_SendData(USARTx, *string);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);		
		string++;
	}
}


//重定向C库函数printf到串口，重定向后可使用printf函数
int fputc(int ch,FILE *f)
{
	USART_SendData(USART2, (uint8_t)ch);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE)==RESET);
	return ch;
}

//重定向C库函数scanf到串口，重定向后可使用scanf,getchar函数
int fgetc(FILE *f)
{
	while(USART_GetFlagStatus(USART2, USART_IT_RXNE)==RESET);//等待串口输入数据，所以执行到这一句的时候，程序是停在这的，这不太好
	return (int)USART_ReceiveData(USART2);
}





void USART2_IRQHandler(void)
{
    if(USART_GetITStatus(USART2, USART_IT_IDLE)==SET)
    {
//        USART_ClearITPendingBit(USART2, USART_IT_IDLE);//串口空闲中断标志位通过这一句是清不掉的，要用下面3句才行
        
        uint16_t useless;//定义一个没有用的变量，用来清除空闲中断标志位的
        useless = USART2->SR;
        useless = USART2->DR;//这两步操作用来清除空闲标志位

//        printf("%s\r\n",receiveBuffer);
        
        DMA_Cmd(DMA1_Stream5, DISABLE);//先关闭串口，防止在处理的时候再传来数据，造成干扰
         
        receiveNum = 20 - DMA_GetCurrDataCounter(DMA1_Stream5);//得到接收到的字节数
        receiveNewDataFalg = 1;//接收到新数据标志位置1
        
        DMA_SetCurrDataCounter(DMA1_Stream5, 20);//重新设置为传输的最大字节数
        DMA_Cmd(DMA1_Stream5, ENABLE);
    }
}
