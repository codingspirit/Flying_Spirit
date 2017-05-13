/******************** (C) COPYRIGHT 2013  **************************
 * 文件名  ：main.c
 * 描述    ：实现GY_86模块的读取数据       
 * 实验平台：奋斗STM32开发板
 * 库版本  ：ST3.5.0
 * 显示    ：采用电脑串口的超级终端
 * 作者    ：LYJ
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
 * 函数名：main
 * 描述  ：主函数
 * 输入  ：无
 * 输出  ：无
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
		USART1_printf(USART1,"\r\n ACCEL_XOUT_H: ");//未经过处理的X轴加速度值
		USART1_printf(USART1," %d \r\n",GetData_mpu6050(ACCEL_XOUT_H));
		USART1_printf(USART1,"\r\n GYRO_XOUT_H: ");//未经过处理的X轴陀螺仪值
		USART1_printf(USART1," %d \r\n",GetData_mpu6050(GYRO_XOUT_H));
	  if(GetData_hw5883())
		{
			USART1_printf(USART1,"\r\n Mag_x: ");//未经过处理的X轴陀螺仪值
			USART1_printf(USART1," %d \r\n",Mag_x);
		}
		GetData_ms5611();
		Exchange_Number();
		USART1_printf(USART1,"\r\n TEMP: ");//未经过处理的X轴陀螺仪值
		USART1_printf(USART1," %d \r\n",TEMP);
		USART1_printf(USART1,"\r\n Pressure: ",(int)(TEMP/100));//未经过处理的MS5611的侧量温度值
		USART1_printf( USART1, "\r\n %s \r\n",exchange_num);//经过处理的气压值
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
