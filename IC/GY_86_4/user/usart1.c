#include "usart1.h"
#include <stdarg.h>
 void USART1_Config(void)
 {
 		GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate=9600;//波特率
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;//8位数据格式
	USART_InitStructure.USART_StopBits=USART_StopBits_1;//1位停止位
	USART_InitStructure.USART_Parity=USART_Parity_No;//无奇偶校验
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//无硬件控制流控制
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//收发模式
	USART_Init(USART1,&USART_InitStructure);
	USART_Cmd(USART1,ENABLE);


 }
 int fputc(int ch,FILE *f)
 {
 	USART_SendData(USART1,(unsigned char) ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
	return(ch);
 }
 /*
 * 函数名：itoa
 * 描述  ：将整形数据转换成字符串
 * 输入  ：-radix =10 表示10进制，其他结果为0
 *         -value 要转换的整形数
 *         -buf 转换后的字符串
 *         -radix = 10
 * 输出  ：无
 * 返回  ：无
 * 调用  ：被USART1_printf()调用
 */
 static char *itoa(int value, char *string, int radix)//将整形数据转化成字符串
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)//radix =10 表示10进制，其他结果为0
    {
        *ptr = 0;
        return string;
    }

    if (!value)//value 要转换的整形数，如果为0，转换成ASCII码为0+48=0x30
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)//如果小于0，在数据前加上负号，并把value转化成正数
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)//
    {
        d = value / i;            //value可取的最大值为65536的话，从高位取数

        if (d || flag)//判断首位数据，当首位不为0时，开始保存数据
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;//结束位为0

    return string;

} /* NCL_Itoa */

/*
 * 函数名：USART2_printf
 * 描述  ：格式化输出，类似于C库中的printf，但这里没有用到C库
 * 输入  ：-USARTx 串口通道，这里只用到了串口2，即USART2
 *		     -Data   要发送到串口的内容的指针
 *			   -...    其他参数
 * 输出  ：无
 * 返回  ：无 
 * 调用  ：外部调用
 *         典型应用USART2_printf( USART2, "\r\n this is a demo \r\n" );
 *            		 USART2_printf( USART2, "\r\n %d \r\n", i );
 *            		 USART2_printf( USART2, "\r\n %s \r\n", j );
 */
void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...)//发送函数
{
	const char *s;
  int d;   
  char buf[16];

  va_list ap;//定义一个VA_LIST 可变参的变量
  va_start(ap, Data);//初始化定义的变量

	while ( *Data != 0)     // 判断是否到达字符串结束符
	{				                          
		if ( *Data == 0x5c )  //'\'  ASCII码为92
		{									  
			switch ( *++Data )
			{
				case 'r':							          //回车符
					USART_SendData(USARTx, 0x0d);
					Data ++;
					break;

				case 'n':							          //换行符
					USART_SendData(USARTx, 0x0a);	
					Data ++;
					break;
				
				default:
					Data ++;
				    break;
			}			 
		}
		else if ( *Data == '%')
		{									  //
			switch ( *++Data )
			{				
				case 's':										  //字符串
					s = va_arg(ap, const char *);
          for ( ; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
          }
					Data++;
          break;

        case 'd':										//十进制
          d = va_arg(ap, int);//返回可变的参数，第二个为要返回的参数的类型
          itoa(d, buf, 10);//调用了转换函数
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
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );//等待数据发送完毕
	}
}

