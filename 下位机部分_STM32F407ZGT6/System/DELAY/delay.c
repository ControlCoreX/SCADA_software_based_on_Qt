#include "delay.h"


//微秒级延时函数
void delay_us(uint16_t us)
{		
	uint32_t temp;	
	SysTick->LOAD = us*21;//设定定时器重装载值  		 
	SysTick->VAL = 0X00;//清空定时器当前值
	SysTick->CTRL |= 1;//使能定时器，开始倒计时；计数频率为AHB/8；计数到0，不产生中断；	 
	do
	{
		temp=SysTick->CTRL;
	}while((temp & 0x01) && !(temp & (1<<16)));	//等待时间到达   
	SysTick->CTRL &= 0; //关闭计数器
	SysTick->VAL =0X00;//清空定时器当前值，不干扰后面其它地方使用这个定时器
}



//毫秒级延时函数
void delay_ms(uint16_t ms)
{		
	uint32_t temp;	
	SysTick->LOAD = ms*21000;//设定定时器重装载值  		 
	SysTick->VAL = 0X00;//清空定时器当前值
	SysTick->CTRL |= 1;//使能定时器，开始倒计时；计数频率为AHB/8；计数到0，不产生中断；	 
	do
	{
		temp=SysTick->CTRL;
	}while((temp & 0x01) && !(temp & (1<<16)));	//等待时间到达   
	SysTick->CTRL &= 0; //关闭计数器
	SysTick->VAL =0X00;//清空定时器当前值，不干扰后面其它地方使用这个定时器
}



