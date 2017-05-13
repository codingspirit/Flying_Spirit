/*包含头------------------------------------------------------------------*/
#include "MP6050.h"



/*宏定义------------------------------------------------------------------*/

#define	MPU6050_Addr   0xD0	  //定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改
// 定义MPU6050内部地址
//****************************************
#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)

#define INT_PIN_CFG     0x37    //设置旁路有效 打开值：0x42 AUX_DA的辅助I2C
#define USER_CTRL       0x6A    //用户配置寄存器 打开值：0x40  AUX_DA的辅助I2C

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
#define	WHO_AM_I		0x75	//IIC地址寄存器(默认数值0x68，只读)

//***************************************************************/


/*变量声明----------------------------------------------------------------*/
u8 TX_DATA[4];  	 //显示据缓存区
u8 BUF[10];       //接收数据缓存区		 		
uint16_t GYRO_XOUT,GYRO_YOUT,GYRO_ZOUT,ACCEL_XOUT,ACCEL_YOUT,ACCEL_ZOUT,MP6050_Temperature;//X,Y,Z轴，温度	
/*函数声明----------------------------------------------------------------*/
extern void delay_init(u8 SYSCLK);
extern void delay_ms(uint16_t nms);
extern void delay_us(uint32_t nus);


extern void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...) ;

extern void I2C_GPIO_Configuration(void);
extern unsigned char I2C_ReadByte(unsigned char DeviceAddr,unsigned char address);  //从24c02的地址address中读取一个字节数据
extern void I2C_WriteByte(unsigned char DeviceAddr,unsigned char address,unsigned char info);
extern void I2C_Read_MultiBytes(unsigned char DeviceAddr,unsigned char address,unsigned char BytesNum,unsigned char * OutDate );

void Init_MPU6050(void);
void READ_MPU6050_Gyro(void);
void READ_MPU6050_Accel(void);
void MPU6050_WHO_AM_I(void);

/* Private functions------------------------------------------------------*/


/************************************************************   
* 函数名:Init_MPU6050   
* 描述 ：初始化MPU6050，根据需要请参考pdf进行修改
* 输入 :无   
* 输出 :无    
*/
void Init_MPU6050(void)
{

  I2C_WriteByte(MPU6050_Addr,PWR_MGMT_1, 0x00);	//解除休眠状态
	
	
	I2C_WriteByte(MPU6050_Addr,SMPLRT_DIV, 0x07);    //陀螺仪采样率
	I2C_WriteByte(MPU6050_Addr,CONFIG, 0x06);        //5Hz 
	
	I2C_WriteByte(MPU6050_Addr,INT_PIN_CFG, 0x42);   //使能旁路I2C
	I2C_WriteByte(MPU6050_Addr,USER_CTRL, 0x40);     //使能旁路I2C
	
	I2C_WriteByte(MPU6050_Addr,GYRO_CONFIG, 0x18);   
	I2C_WriteByte(MPU6050_Addr,ACCEL_CONFIG, 0x01);
	
}
	
/************************************************************   
* 函数名:MP6050_WHO_AM_I   
* 描述 ：读取MPU6050设备信息
* 输入 :无   
* 输出 :无    
*/
void MPU6050_WHO_AM_I(void)
{
	u8 dev=0;
		if(dev=I2C_ReadByte(MPU6050_Addr,WHO_AM_I),dev==0x68)
  { 
		  GPIO_ResetBits(GPIOA, GPIO_Pin_0);
    	printf("\r设备MP6050识别成功，id=0x%x\r\n\r",dev);
  }
	else{ printf("\r错误!无法设别设备MP6050，id=0x%x\r\n\r");}
}

	
/************************************************************   
* 函数名:READ_MPU6050_Gyro   
* 描述 : 读取MPU6050陀螺仪数据
* 输入  :无   
* 输出  :无    
*/
void READ_MPU6050_Gyro(void)
{
   BUF[0]=I2C_ReadByte(MPU6050_Addr,GYRO_XOUT_L); 
   BUF[1]=I2C_ReadByte(MPU6050_Addr,GYRO_XOUT_H);
   GYRO_XOUT=	(BUF[1]<<8)|BUF[0];
   GYRO_XOUT/=16.4; 						   //读取计算X轴数据

   BUF[2]=I2C_ReadByte(MPU6050_Addr,GYRO_YOUT_L);
   BUF[3]=I2C_ReadByte(MPU6050_Addr,GYRO_YOUT_H);
   GYRO_YOUT=	(BUF[3]<<8)|BUF[2];
   GYRO_YOUT/=16.4; 						   //读取计算Y轴数据
   BUF[4]=I2C_ReadByte(MPU6050_Addr,GYRO_ZOUT_L);
   BUF[5]=I2C_ReadByte(MPU6050_Addr,GYRO_ZOUT_H);
   GYRO_ZOUT=	(BUF[5]<<8)|BUF[4];
   GYRO_ZOUT/=16.4; 					       //读取计算Z轴数据

	
  // BUF[6]=I2C_ReadByte(MPU6050_Addr,TEMP_OUT_L); 
  // BUF[7]=I2C_ReadByte(MPU6050_Addr,TEMP_OUT_H); 
  // T_T=(BUF[7]<<8)|BUF[6];
  // T_T = 35+ ((double) (T_T + 13200)) / 280;// 读取计算出温度
}
/************************************************************   
* 函数名:READ_MPU6050_Accel   
* 描述 : 读取MPU6050加速度数据
* 输入  :无   
* 输出  :无    
*/
void READ_MPU6050_Accel(void)
{
	BUF[0]=I2C_ReadByte(MPU6050_Addr,ACCEL_XOUT_L); 
	BUF[1]=I2C_ReadByte(MPU6050_Addr,ACCEL_XOUT_H);
	ACCEL_XOUT=	(BUF[1]<<8)|BUF[0];
	ACCEL_XOUT=(float)((float)ACCEL_XOUT/16384)*100; 		//扩大100倍	       //读取计算X轴数据

	BUF[2]=I2C_ReadByte(MPU6050_Addr,ACCEL_YOUT_L);
	BUF[3]=I2C_ReadByte(MPU6050_Addr,ACCEL_YOUT_H);
	ACCEL_YOUT=	(BUF[3]<<8)|BUF[2];
	ACCEL_YOUT=(float)((float)ACCEL_YOUT/16384)*100; 						   //读取计算Y轴数据
   
	BUF[4]=I2C_ReadByte(MPU6050_Addr,ACCEL_ZOUT_L);
	BUF[5]=I2C_ReadByte(MPU6050_Addr,ACCEL_ZOUT_H);
	ACCEL_ZOUT=	(BUF[5]<<8)|BUF[4];
	ACCEL_ZOUT=(float)((float)ACCEL_ZOUT/16384)*100; 					       //读取计算Z轴数据

	
	BUF[6]=I2C_ReadByte(MPU6050_Addr,TEMP_OUT_L); 
	BUF[7]=I2C_ReadByte(MPU6050_Addr,TEMP_OUT_H); 
	MP6050_Temperature=(BUF[7]<<8)|BUF[6];
//	MP6050_Temperature = 35+ ((double) (T_T + 13200)) / 280;// 读取计算出温度
	MP6050_Temperature = (((double) MP6050_Temperature )/340+36.53)*10 ;//+36.53;  // 读取计算出温度
}
