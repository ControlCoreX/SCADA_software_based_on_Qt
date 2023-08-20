#ifndef _USART_H_
#define _USART_H_

#include "stm32f4xx.h"
#include <stdio.h>

extern void usart_init(void);
extern void USART_SendString(USART_TypeDef* USARTx, uint8_t* string);

#endif //_USART_H_

