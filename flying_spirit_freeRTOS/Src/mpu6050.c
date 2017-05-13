#include "mpu6050.h"

volatile InitialData_TypeDef InitialData;

void MPU6050_Init(void)
{
    I2C_WriteByte(MPU6050_Addr, PWR_MGMT_1, 0x80);//reset register
    HAL_Delay(50);
    I2C_WriteByte(MPU6050_Addr, SMPLRT_DIV, 0x00);//Sample Rate = Gyroscope Output Rate / (1 + SMPLRT_DIV)
    I2C_WriteByte(MPU6050_Addr, PWR_MGMT_1, 0x03);//PLL with Z Gyro reference

    I2C_WriteByte(MPU6050_Addr, INT_PIN_CFG, 0x02);  //enable auxiliary I2C
    I2C_WriteByte(MPU6050_Addr, CONFIG, 0x03);//DLPF bandwidth 44 42

    I2C_WriteByte(MPU6050_Addr, GYRO_CONFIG, 0x18);//Set Full Scale Range 2000
    I2C_WriteByte(MPU6050_Addr, ACCEL_CONFIG, 0x10);// Accel scale 8g (4096 LSB/g)
}

void Read_MPU6050()
{
	uint8_t buf[6];
	HAL_I2C_Mem_Read(&hi2c1,MPU6050_Addr,ACCEL_XOUT_H,I2C_MEMADD_SIZE_8BIT,buf,6,100);
	InitialData.AccData[0]=(int16_t)((buf[0] << 8) | buf[1]);
	InitialData.AccData[1]=(int16_t)((buf[2] << 8) | buf[3]);
	InitialData.AccData[2]=(int16_t)((buf[4] << 8) | buf[5]);
	
	HAL_I2C_Mem_Read(&hi2c1,MPU6050_Addr,GYRO_XOUT_H,I2C_MEMADD_SIZE_8BIT,buf,6,100);
	InitialData.GyroData[0]=(int16_t)((buf[0] << 8) | buf[1]);
	InitialData.GyroData[1]=(int16_t)((buf[2] << 8) | buf[3]);
	InitialData.GyroData[2]=(int16_t)((buf[4] << 8) | buf[5]);
}
