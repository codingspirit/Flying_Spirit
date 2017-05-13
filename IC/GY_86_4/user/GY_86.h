#ifndef _GY_86_H
#define _GY_86_H

#include "stm32f10x.h"
#include"stm32f10x_i2c.h"
#include "math.h"
#include "usart1.h"
//typedef unsigned short ushort;
typedef unsigned  short int   uint;
typedef enum {FALSE = 0, TRUE = !FALSE} bool;
//****************************************
// 定义MPU6050内部地址
//****************************************
#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			  0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s) 2000*8.75/1000=17.5
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I			0x75	//IIC地址寄存器(默认数值0x68，只读)
#define	MPU6050_Addr	0xD0	//IIC写入时的地址字节数据，+1为读取

//定义器件在IIC总线中HMC5883L
#define	HMC5883L_Addr   0x3C	//磁场传感器器件地址

#define HMC58X3_R_CONFA 0
#define HMC58X3_R_CONFB 1
#define HMC58X3_R_MODE 2

//定义器件在IIC总线中的MS561101BA

#define MS561101BA_Addr   0xEE

#define MS561101BA_D1_OSR_4096   0x48 
#define MS561101BA_D2_OSR_4096   0x58 
#define MS561101BA_D1      0x40 
#define MS561101BA_D2      0x50 
#define MS561101BA_RST     0x1E //复位指令

#define MS561101BA_ADC_RD     0x00 
#define MS561101BA_PROM_RD    0xA0 
#define MS561101BA_PROM_CRC   0xAE
//IIC总线引脚配置
#define SCL_H         GPIOC->BSRR = GPIO_Pin_3
#define SCL_L         GPIOC->BRR  = GPIO_Pin_3 
   
#define SDA_H         GPIOC->BSRR = GPIO_Pin_5
#define SDA_L         GPIOC->BRR  = GPIO_Pin_5

#define SCL_read      GPIOC->IDR  & GPIO_Pin_3
#define SDA_read      GPIOC->IDR  & GPIO_Pin_5
//****************************************
//定义类型及变量
//****************************************
//int	Temperature,Temp_h,Temp_l;	//温度及高低位数据
//****************************************
//函数声明
//****************************************
//void  delay(unsigned int k);										//延时
void  lcd_printf(u8 *s,short int temp_data);
//MPU6050操作函数
void  Init_MPU6050(void);//初始化MPU6050
u8 Single_ReadI2C(u8 Slave_Address,u8 REG_Address);//读取I2C数据
bool Single_WriteI2C(u8 Slave_Address,u8 REG_Address,u8 REG_data);	//向I2C写入数据//stm32 IIC 函数
void IIC_GY521_GPIO_Init(void);
void Display10BitData(short int value,u8 x,u8 y);
////////////////////////////////
void  Init_HMC5883L(void);
u8 GetData_hw5883(void);
/////////////////////////////////
void Init_MS561101BA();
bool MS561101BA_RESET();
bool MS561101BA_PROM_READ();
void GetData_ms5611();
void Exchange_Number();
u8 crc4(u16 n_prom[]) ;
u8 MS561101BA_getPressure(u8 OSR_Pres);
void MS561101BA_getTemperature(u8 OSR_Temp);
u32 MS561101BA_DO_CONVERSION(u8 command);
/////////////////////////////////
short int GetData_mpu6050(u8 REG_Address);
static void I2C_GPIO_Config(void);
static void I2C_Mode_Config(void);
void display_temp(void);
void I2C_SendByte(u8 dat);
extern int Mag_x;
extern int Mag_y;
extern int Mag_z;


#endif/* __GY_521_H */