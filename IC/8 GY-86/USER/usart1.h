#ifndef _USART1_H_
#define _USART1_H_
#include "stm32f10x_conf.h"
void USART1_Conf(void);
void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...) ;
void Usart_Send(u8 *p,uint16_t len) ;
#endif
