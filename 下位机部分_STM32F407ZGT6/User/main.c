#include "delay.h"
#include "usart.h"
#include "bitband.h"


extern uint8_t receiveBuffer[20];
extern volatile uint8_t receiveNewDataFalg;
extern volatile uint8_t receiveNum;


void parse(uint8_t* cmdline,uint8_t* flag,double* value);

void GOD_Init(void)
{
    usart_init();

//    printf("USART init success...\r\n");
}

int main(void)
{
    GOD_Init();
    
	while(1)
    {
        if(receiveNewDataFalg==1)
        {
            
            
//            printf("接收到%d个字节数据...\r\n",receiveNum);
            receiveNewDataFalg=0;
        }
    }
}


//把字符串转化我浮点数的函数
void parse(uint8_t* cmdline,uint8_t* flag,double* value)
{
    uint8_t* temp=cmdline+1;
    
    *flag=*cmdline;
    *value=atof(temp);
}
