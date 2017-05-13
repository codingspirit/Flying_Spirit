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
// ����MPU6050�ڲ���ַ
//****************************************
#define	SMPLRT_DIV		0x19	//�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define	CONFIG			  0x1A	//��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s) 2000*8.75/1000=17.5
#define	ACCEL_CONFIG	0x1C	//���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
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
#define	PWR_MGMT_1		0x6B	//��Դ��������ֵ��0x00(��������)
#define	WHO_AM_I			0x75	//IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)
#define	MPU6050_Addr	0xD0	//IICд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ

//����������IIC������HMC5883L
#define	HMC5883L_Addr   0x3C	//�ų�������������ַ

#define HMC58X3_R_CONFA 0
#define HMC58X3_R_CONFB 1
#define HMC58X3_R_MODE 2

//����������IIC�����е�MS561101BA

#define MS561101BA_Addr   0xEE

#define MS561101BA_D1_OSR_4096   0x48 
#define MS561101BA_D2_OSR_4096   0x58 
#define MS561101BA_D1      0x40 
#define MS561101BA_D2      0x50 
#define MS561101BA_RST     0x1E //��λָ��

#define MS561101BA_ADC_RD     0x00 
#define MS561101BA_PROM_RD    0xA0 
#define MS561101BA_PROM_CRC   0xAE
//IIC������������
#define SCL_H         GPIOC->BSRR = GPIO_Pin_3
#define SCL_L         GPIOC->BRR  = GPIO_Pin_3 
   
#define SDA_H         GPIOC->BSRR = GPIO_Pin_5
#define SDA_L         GPIOC->BRR  = GPIO_Pin_5

#define SCL_read      GPIOC->IDR  & GPIO_Pin_3
#define SDA_read      GPIOC->IDR  & GPIO_Pin_5
//****************************************
//�������ͼ�����
//****************************************
//int	Temperature,Temp_h,Temp_l;	//�¶ȼ��ߵ�λ����
//****************************************
//��������
//****************************************
//void  delay(unsigned int k);										//��ʱ
void  lcd_printf(u8 *s,short int temp_data);
//MPU6050��������
void  Init_MPU6050(void);//��ʼ��MPU6050
u8 Single_ReadI2C(u8 Slave_Address,u8 REG_Address);//��ȡI2C����
bool Single_WriteI2C(u8 Slave_Address,u8 REG_Address,u8 REG_data);	//��I2Cд������//stm32 IIC ����
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