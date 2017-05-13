/******************** (C) COPYRIGHT 2013  **************************
 * �ļ���  ��main.c
 * ����    ��ʵ��GY_86ģ��Ķ�ȡ����       
 * ʵ��ƽ̨���ܶ�STM32������
 * ��汾  ��ST3.5.0
 * ��ʾ    �����õ��Դ��ڵĳ����ն�
 * ����    ��LYJ
 * 
**********************************************************************************/
#include "stm32f10x.h"
#include "usart1.h"
#include "GY_86.h"
int Mag_x=0;
int Mag_y=0;
int Mag_z=0;
u8 exchange_num[8];
extern int TEMP;
extern int Pressure;
/*
 * ��������main
 * ����  ��������
 * ����  ����
 * ���  ����
 */
 void Delay(u16 x)
 {
	while(x--);
}
int main(void)
{	
	USART1_Config();
	IIC_GY86_GPIO_Init();
	Init_MPU6050();
	Init_HMC5883L();
	Init_MS561101BA();
	while(1)
	{
		USART1_printf(USART1,"\r\n ACCEL_XOUT_H: ");//δ���������X����ٶ�ֵ
		USART1_printf(USART1," %d \r\n",GetData_mpu6050(ACCEL_XOUT_H));
		USART1_printf(USART1,"\r\n GYRO_XOUT_H: ");//δ���������X��������ֵ
		USART1_printf(USART1," %d \r\n",GetData_mpu6050(GYRO_XOUT_H));
	  if(GetData_hw5883())
		{
			USART1_printf(USART1,"\r\n Mag_x: ");//δ���������X��������ֵ
			USART1_printf(USART1," %d \r\n",Mag_x);
		}
		GetData_ms5611();
		Exchange_Number();
		USART1_printf(USART1,"\r\n TEMP: ");//δ���������X��������ֵ
		USART1_printf(USART1," %d \r\n",TEMP);
		USART1_printf(USART1,"\r\n Pressure: ",(int)(TEMP/100));//δ���������MS5611�Ĳ����¶�ֵ
		USART1_printf( USART1, "\r\n %s \r\n",exchange_num);//�����������ѹֵ
		Delay(60000);
		Delay(60000);
		Delay(60000);
		Delay(60000);
		Delay(60000);
		Delay(60000);
		Delay(60000);
		Delay(60000);
		Delay(60000);
		
  }
    
}


/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
