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
	USART_InitStructure.USART_BaudRate = 115200;//���ò�����Ϊ115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//��������λΪ8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//����ֹͣλΪ1λ
	USART_InitStructure.USART_Parity = USART_Parity_No;//������У��λ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�����շ�ģʽ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//������Ӳ����
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
		USART_ClearFlag(USARTx, USART_IT_TC);//ÿ�η��͵�ʱ�򶼸����ֶ���0
		USART_SendData(USARTx, *string);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);		
		string++;
	}
}


//�ض���C�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch,FILE *f)
{
	USART_SendData(USART2, (uint8_t)ch);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE)==RESET);
	return ch;
}

//�ض���C�⺯��scanf�����ڣ��ض�����ʹ��scanf,getchar����
int fgetc(FILE *f)
{
	while(USART_GetFlagStatus(USART2, USART_IT_RXNE)==RESET);//�ȴ������������ݣ�����ִ�е���һ���ʱ�򣬳�����ͣ����ģ��ⲻ̫��
	return (int)USART_ReceiveData(USART2);
}





void USART2_IRQHandler(void)
{
    if(USART_GetITStatus(USART2, USART_IT_IDLE)==SET)
    {
//        USART_ClearITPendingBit(USART2, USART_IT_IDLE);//���ڿ����жϱ�־λͨ����һ�����岻���ģ�Ҫ������3�����
        
        uint16_t useless;//����һ��û���õı�����������������жϱ�־λ��
        useless = USART2->SR;
        useless = USART2->DR;//��������������������б�־λ

//        printf("%s\r\n",receiveBuffer);
        
        DMA_Cmd(DMA1_Stream5, DISABLE);//�ȹرմ��ڣ���ֹ�ڴ����ʱ���ٴ������ݣ���ɸ���
         
        receiveNum = 20 - DMA_GetCurrDataCounter(DMA1_Stream5);//�õ����յ����ֽ���
        receiveNewDataFalg = 1;//���յ������ݱ�־λ��1
        
        DMA_SetCurrDataCounter(DMA1_Stream5, 20);//��������Ϊ���������ֽ���
        DMA_Cmd(DMA1_Stream5, ENABLE);
    }
}
