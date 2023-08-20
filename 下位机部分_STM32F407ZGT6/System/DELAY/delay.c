#include "delay.h"


//΢�뼶��ʱ����
void delay_us(uint16_t us)
{		
	uint32_t temp;	
	SysTick->LOAD = us*21;//�趨��ʱ����װ��ֵ  		 
	SysTick->VAL = 0X00;//��ն�ʱ����ǰֵ
	SysTick->CTRL |= 1;//ʹ�ܶ�ʱ������ʼ����ʱ������Ƶ��ΪAHB/8��������0���������жϣ�	 
	do
	{
		temp=SysTick->CTRL;
	}while((temp & 0x01) && !(temp & (1<<16)));	//�ȴ�ʱ�䵽��   
	SysTick->CTRL &= 0; //�رռ�����
	SysTick->VAL =0X00;//��ն�ʱ����ǰֵ�������ź��������ط�ʹ�������ʱ��
}



//���뼶��ʱ����
void delay_ms(uint16_t ms)
{		
	uint32_t temp;	
	SysTick->LOAD = ms*21000;//�趨��ʱ����װ��ֵ  		 
	SysTick->VAL = 0X00;//��ն�ʱ����ǰֵ
	SysTick->CTRL |= 1;//ʹ�ܶ�ʱ������ʼ����ʱ������Ƶ��ΪAHB/8��������0���������жϣ�	 
	do
	{
		temp=SysTick->CTRL;
	}while((temp & 0x01) && !(temp & (1<<16)));	//�ȴ�ʱ�䵽��   
	SysTick->CTRL &= 0; //�رռ�����
	SysTick->VAL =0X00;//��ն�ʱ����ǰֵ�������ź��������ط�ʹ�������ʱ��
}



