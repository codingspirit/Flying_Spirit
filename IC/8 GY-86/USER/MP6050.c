/*����ͷ------------------------------------------------------------------*/
#include "MP6050.h"



/*�궨��------------------------------------------------------------------*/

#define	MPU6050_Addr   0xD0	  //����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�
// ����MPU6050�ڲ���ַ
//****************************************
#define	SMPLRT_DIV		0x19	//�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define	CONFIG			0x1A	//��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define	ACCEL_CONFIG	0x1C	//���ټ��Լ������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)

#define INT_PIN_CFG     0x37    //������·��Ч ��ֵ��0x42 AUX_DA�ĸ���I2C
#define USER_CTRL       0x6A    //�û����üĴ��� ��ֵ��0x40  AUX_DA�ĸ���I2C

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
#define	WHO_AM_I		0x75	//IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)

//***************************************************************/


/*��������----------------------------------------------------------------*/
u8 TX_DATA[4];  	 //��ʾ�ݻ�����
u8 BUF[10];       //�������ݻ�����		 		
uint16_t GYRO_XOUT,GYRO_YOUT,GYRO_ZOUT,ACCEL_XOUT,ACCEL_YOUT,ACCEL_ZOUT,MP6050_Temperature;//X,Y,Z�ᣬ�¶�	
/*��������----------------------------------------------------------------*/
extern void delay_init(u8 SYSCLK);
extern void delay_ms(uint16_t nms);
extern void delay_us(uint32_t nus);


extern void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...) ;

extern void I2C_GPIO_Configuration(void);
extern unsigned char I2C_ReadByte(unsigned char DeviceAddr,unsigned char address);  //��24c02�ĵ�ַaddress�ж�ȡһ���ֽ�����
extern void I2C_WriteByte(unsigned char DeviceAddr,unsigned char address,unsigned char info);
extern void I2C_Read_MultiBytes(unsigned char DeviceAddr,unsigned char address,unsigned char BytesNum,unsigned char * OutDate );

void Init_MPU6050(void);
void READ_MPU6050_Gyro(void);
void READ_MPU6050_Accel(void);
void MPU6050_WHO_AM_I(void);

/* Private functions------------------------------------------------------*/


/************************************************************   
* ������:Init_MPU6050   
* ���� ����ʼ��MPU6050��������Ҫ��ο�pdf�����޸�
* ���� :��   
* ��� :��    
*/
void Init_MPU6050(void)
{

  I2C_WriteByte(MPU6050_Addr,PWR_MGMT_1, 0x00);	//�������״̬
	
	
	I2C_WriteByte(MPU6050_Addr,SMPLRT_DIV, 0x07);    //�����ǲ�����
	I2C_WriteByte(MPU6050_Addr,CONFIG, 0x06);        //5Hz 
	
	I2C_WriteByte(MPU6050_Addr,INT_PIN_CFG, 0x42);   //ʹ����·I2C
	I2C_WriteByte(MPU6050_Addr,USER_CTRL, 0x40);     //ʹ����·I2C
	
	I2C_WriteByte(MPU6050_Addr,GYRO_CONFIG, 0x18);   
	I2C_WriteByte(MPU6050_Addr,ACCEL_CONFIG, 0x01);
	
}
	
/************************************************************   
* ������:MP6050_WHO_AM_I   
* ���� ����ȡMPU6050�豸��Ϣ
* ���� :��   
* ��� :��    
*/
void MPU6050_WHO_AM_I(void)
{
	u8 dev=0;
		if(dev=I2C_ReadByte(MPU6050_Addr,WHO_AM_I),dev==0x68)
  { 
		  GPIO_ResetBits(GPIOA, GPIO_Pin_0);
    	printf("\r�豸MP6050ʶ��ɹ���id=0x%x\r\n\r",dev);
  }
	else{ printf("\r����!�޷�����豸MP6050��id=0x%x\r\n\r");}
}

	
/************************************************************   
* ������:READ_MPU6050_Gyro   
* ���� : ��ȡMPU6050����������
* ����  :��   
* ���  :��    
*/
void READ_MPU6050_Gyro(void)
{
   BUF[0]=I2C_ReadByte(MPU6050_Addr,GYRO_XOUT_L); 
   BUF[1]=I2C_ReadByte(MPU6050_Addr,GYRO_XOUT_H);
   GYRO_XOUT=	(BUF[1]<<8)|BUF[0];
   GYRO_XOUT/=16.4; 						   //��ȡ����X������

   BUF[2]=I2C_ReadByte(MPU6050_Addr,GYRO_YOUT_L);
   BUF[3]=I2C_ReadByte(MPU6050_Addr,GYRO_YOUT_H);
   GYRO_YOUT=	(BUF[3]<<8)|BUF[2];
   GYRO_YOUT/=16.4; 						   //��ȡ����Y������
   BUF[4]=I2C_ReadByte(MPU6050_Addr,GYRO_ZOUT_L);
   BUF[5]=I2C_ReadByte(MPU6050_Addr,GYRO_ZOUT_H);
   GYRO_ZOUT=	(BUF[5]<<8)|BUF[4];
   GYRO_ZOUT/=16.4; 					       //��ȡ����Z������

	
  // BUF[6]=I2C_ReadByte(MPU6050_Addr,TEMP_OUT_L); 
  // BUF[7]=I2C_ReadByte(MPU6050_Addr,TEMP_OUT_H); 
  // T_T=(BUF[7]<<8)|BUF[6];
  // T_T = 35+ ((double) (T_T + 13200)) / 280;// ��ȡ������¶�
}
/************************************************************   
* ������:READ_MPU6050_Accel   
* ���� : ��ȡMPU6050���ٶ�����
* ����  :��   
* ���  :��    
*/
void READ_MPU6050_Accel(void)
{
	BUF[0]=I2C_ReadByte(MPU6050_Addr,ACCEL_XOUT_L); 
	BUF[1]=I2C_ReadByte(MPU6050_Addr,ACCEL_XOUT_H);
	ACCEL_XOUT=	(BUF[1]<<8)|BUF[0];
	ACCEL_XOUT=(float)((float)ACCEL_XOUT/16384)*100; 		//����100��	       //��ȡ����X������

	BUF[2]=I2C_ReadByte(MPU6050_Addr,ACCEL_YOUT_L);
	BUF[3]=I2C_ReadByte(MPU6050_Addr,ACCEL_YOUT_H);
	ACCEL_YOUT=	(BUF[3]<<8)|BUF[2];
	ACCEL_YOUT=(float)((float)ACCEL_YOUT/16384)*100; 						   //��ȡ����Y������
   
	BUF[4]=I2C_ReadByte(MPU6050_Addr,ACCEL_ZOUT_L);
	BUF[5]=I2C_ReadByte(MPU6050_Addr,ACCEL_ZOUT_H);
	ACCEL_ZOUT=	(BUF[5]<<8)|BUF[4];
	ACCEL_ZOUT=(float)((float)ACCEL_ZOUT/16384)*100; 					       //��ȡ����Z������

	
	BUF[6]=I2C_ReadByte(MPU6050_Addr,TEMP_OUT_L); 
	BUF[7]=I2C_ReadByte(MPU6050_Addr,TEMP_OUT_H); 
	MP6050_Temperature=(BUF[7]<<8)|BUF[6];
//	MP6050_Temperature = 35+ ((double) (T_T + 13200)) / 280;// ��ȡ������¶�
	MP6050_Temperature = (((double) MP6050_Temperature )/340+36.53)*10 ;//+36.53;  // ��ȡ������¶�
}
