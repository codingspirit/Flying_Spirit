#ifndef _MP6050_H_
#define _MP6050_H_

/*包含头------------------------------------------------------------------*/
#include "stm32f10x_conf.h"
#include "usart1.h"
#include "IIC.h"
#include  "delay.h"
/*变量声明----------------------------------------------------------------*/

/*函数声明----------------------------------------------------------------*/
void Init_MPU6050(void);
void READ_MPU6050_Gyro(void);
void READ_MPU6050_Accel(void);
void MPU6050_WHO_AM_I(void);

#endif
