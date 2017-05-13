/*����ͷ------------------------------------------------------------------*/
#include "stm32f10x_conf.h"
//#include "stm32f10x_gpio.h"  
//#include "stm32f10x_rcc.h"    
//#include "stm32f10x_usart.h" 
#include <stdlib.h>
#include <stdio.h>
#include "stdarg.h"
/*�궨��------------------------------------------------------------------*/


/*��������----------------------------------------------------------------*/

						 
/*��������----------------------------------------------------------------*/
void USART1_Conf(void);
static char *itoa(int value, char *string, int radix); 
void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...) ;	
void Usart_Send(u8 *p,uint16_t len) ;


/**************************************************************************/
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
//_sys_exit(int x) 
//{ 
//	x = x; 
//} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*ʹ��microLib�ķ���*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
/***************************************************************************/

/************************************************************   
* ������:USART1_Config   
* ���� :USART1 GPIO ����ģʽ���ã�115200 8-N-1   
* ����  :��   
* ���  :��    
*/  
void USART1_Conf(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	/* USART1 GPIO config */   
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */ 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;    
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Configure USART1 Rx (PA.10) as input floating */   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;    
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    
  GPIO_Init(GPIOA, &GPIO_InitStructure);    
   
  /* USART1 mode config */ 
	
	USART_InitStruct.USART_BaudRate=115200;
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;
	USART_InitStruct.USART_StopBits=USART_StopBits_1;
	USART_InitStruct.USART_Parity=USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode= USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStruct);
	USART_Cmd(USART1, ENABLE); 
	
}
void Usart_Send(u8 *p,uint16_t len) //���ڷ�������
{
	uint16_t j;
	for(j=0;j<len;j++)
	{
		USART_SendData(USART1,*p);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);//�ȴ����ͽ���
		p++;
	}
}
/****************************************************************  
* ������:USART1_printf   
* ���� :��ʽ�����������printf���õ���"stdarg.h"
* ���� :-USARTx ����ͨ��   
*           -Data  Ҫ���͵�����ָ�� 
*             -...  �������� 
* ���  :�� 
*/ 
void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...)    
{    
    const char *s;    
    int d;       
    char buf[16];    
    va_list ap;    
    va_start(ap, Data);    
    while ( *Data != 0)     // ????????????    
    {                                             
        if ( *Data == 0x5c )  //'\'    
        {                                         
       switch ( *++Data )    
            {    
                case 'r':                                     //???    
                    USART_SendData(USARTx, 0x0d);    
                   Data ++;    
                   break;    
  
                case 'n':                                     //???    
                    USART_SendData(USARTx, 0x0a);       
                    Data ++;    
                    break;    
                     
                default:    
                    Data ++;    
                    break;    
            }                
        }    
        else if ( *Data == '%')    
        {                                     //    
             switch ( *++Data )    
           {                   
           case 's':          
                  s = va_arg(ap, const char *);    
            for ( ; *s; s++)     
            {    
            USART_SendData(USARTx,*s);    
            while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET ); 
            }  
           Data++;    
           break;  
   
           case 'd':                                       //???    
           d = va_arg(ap,int);    
           itoa(d, buf, 10);    
           for (s = buf; *s; s++)     
           {    
           USART_SendData(USARTx,*s);    
           while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET ); 
           }    
           Data++;    
           break;    
           default:    
                 Data++;    
                 break;    
             }            
        } /* end of else if */   
        else USART_SendData(USARTx, *Data++);    
        while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );   
   }    
} 
/***********************************************************************   
* ������:itoa  
* ���� :����������ת��Ϊ�ַ�
* ���� :-radix =10 ��ʾʮ����   
*        -value Ҫת��������ֵ  
*        -buf ת������ַ��� 
*        -radix = 10    
* ��� :��
* ���� :��USART1_printf()����
*/
static char *itoa(int value, char *string, int radix)    
{    
      int     i, d;    
      int     flag = 0;    
      char    *ptr = string;    
     
      /* This implementation only works for decimal numbers. */   
      if (radix != 10)    
      {    
          *ptr = 0;    
         return string;    
      }    
     
      if (!value)    
      {    
          *ptr++ = 0x30;    
          *ptr = 0;    
          return string;    
      }    
     
      /* if this is a negative value insert the minus sign. */   
      if (value < 0)    
      {    
          *ptr++ = '-';    
          /* Make the value positive. */   
          value *= -1;    
      }    
      for (i = 10000; i > 0; i /= 10)    
      {    
          d = value / i;    
          if (d || flag)    
          {    
              *ptr++ = (char)(d + 0x30);    
              value -= (d * i);    
              flag = 1;    
          }    
      }    
     
      /* Null terminate the string. */   
      *ptr = 0;    
      return string;    
} /* NCL_Itoa */
