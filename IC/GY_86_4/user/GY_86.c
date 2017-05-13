//****************************************
// Update to MPU6050 by shinetop
// MCU: STM32F103VE
// 2013.12.12
// ����: ��ʾ���ٶȼƺ������ǵ�10λԭʼ����
//****************************************
// GY-86 MPU6050��HMC5883L��MS5611-01BA03 IIC���Գ���
// STM32F103VE 
// ����72MHZ
// ��ʾ���������
// ���뻷�� Keil uVision4
// ʱ�䣺2013��12��12��
//****************************************
#include "GY_86.h"

u8 dis[4];//��ʾ����(-511��512)���ַ�����
unsigned short int dis_data;//����
u16 Cal_C[8];//uint16
u16 nprom[]={0x3132,0x3334,0x3536,0x3738,0x3940,0x4142,0x4344,0x450b};
u32 D1_Pres=0,D2_Temp=0,Aux,OFF2,SENS2;//int32
int64_t dT=0;
int32_t TEMP=0;
int64_t OFF_, SENS; //int64
signed int Pressure=0;//int32
signed int TEMP2; 
signed int ex_Pressure=0; 
extern u8 exchange_num[11];
/*******************************************
 I2C_GPIO_Config();
*****************************************/
static void I2C_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
		/* ʹ���� I2C�йص�ʱ�� */
	RCC_APB2PeriphClockCmd  (RCC_APB2Periph_GPIOC, ENABLE );  

	 /* PC3-I2C_SCL��PC5-I2C_SDA*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_3; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
}
///////////IIC��ʼ��//////////////
void IIC_GY86_GPIO_Init(void)
{	   
 	I2C_GPIO_Config();
}  
////////////������ʱ����//////////
void Delay_1us(u16 n)//Լ1us,1100k
{
	 unsigned int x=5,i=0;
	for(i=0;i<n;i++)
	 {while(x--);x=5;}
}
void Delay_ms(u8 n)//Լ0.5ms��2k
{
	 unsigned int x=5000,i=0;
	for(i=0;i<n;i++)
	 {while(x--);x=5000;}
}
////////IIC��������//////////
void I2C_Start(void)
{
		SDA_H; 	
		SCL_H;
		Delay_1us(1);
		if(!SDA_read) return;//SDA��Ϊ�͵�ƽ������æ,�˳�
		SDA_L;
		Delay_1us(1);
	  if(SDA_read) return;//SDA��Ϊ�ߵ�ƽ�����߳���,�˳�
		SDA_L;
		Delay_1us(1);
		SCL_L;
}
//**************************************
//IICֹͣ�ź�
//**************************************
void I2C_Stop(void)
{
    SDA_L;
		SCL_L;
		Delay_1us(1);
		SCL_H;
		SDA_H;
		Delay_1us(1);                 //��ʱ
} 
//**************************************
//IIC����Ӧ���ź�
//��ڲ���:ack (0:ACK 1:NAK)
//**************************************
void I2C_SendACK(u8 i)
{
    if(1==i)SDA_H;                  //дӦ���ź�
    else SDA_L;
	  SCL_H;                    //����ʱ����
    Delay_1us(1);                 //��ʱ
    SCL_L ;                  //����ʱ����
    Delay_1us(1);    
} 
//**************************************
//IIC�ȴ�Ӧ��
//����ֵ��ack (1:ACK 0:NAK)
//**************************************
 bool I2C_WaitAck(void) 	 //����Ϊ:=1��ACK,=0��ACK
{
	 unsigned int i;
	SDA_H;			
	Delay_1us(1);
	SCL_H;
	Delay_1us(1);
	while(SDA_read){i++;if(i==5000)break;}
	if(SDA_read)
	{SCL_L;
	Delay_1us(1);
	return FALSE;}

	SCL_L;
	Delay_1us(1);
	return TRUE;
}

//**************************************
//��IIC���߷���һ���ֽ�����
//**************************************
void I2C_SendByte(u8 dat)
{
  unsigned int i;
	unsigned char ack=1;
	
	SCL_L;
    for (i=0; i<8; i++)         //8λ������
    {
		 if(dat&0x80){SDA_H;}   //�����ݿ�
		 else SDA_L;
        SCL_H;                //����ʱ����
        Delay_1us(1);             //��ʱ
        SCL_L;                //����ʱ����
        Delay_1us(1); 		  //��ʱ
		     dat <<= 1;          //�Ƴ����ݵ����λ  
    }					 
}

//**************************************
//��IIC���߽���һ���ֽ�����
//**************************************
u8 I2C_RecvByte()
{
    u8 i;
    u8 dat = 0;
     SDA_H;                    //ʹ���ڲ�����,׼����ȡ����,
    for (i=0; i<8; i++)         //8λ������
    {
        
		    dat <<= 1;
        SCL_H;                //����ʱ����
        Delay_1us(1);            //��ʱ
		  if(SDA_read) //������    
		  {
		   dat |=0x01;
		  }                           
        SCL_L;                //����ʱ����
        Delay_1us(1);
    } 
		
    return dat;
} 
//**************************************
//��IIC�豸д��һ���ֽ�����
//**************************************
bool Single_WriteI2C(u8 Slave_Address,u8 REG_Address,u8 REG_data)
{
    I2C_Start();              //��ʼ�ź�

    I2C_SendByte(Slave_Address);   //�����豸��ַ+д�ź�
 	  if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
   
		I2C_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ��
 	  if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
   
		I2C_SendByte(REG_data);       //�ڲ��Ĵ������ݣ�
	  if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
	  
		I2C_Stop();                   //����ֹͣ�ź�
}
//**************************************
//��IIC�豸��ȡһ���ֽ�����
//**************************************
u8 Single_ReadI2C(u8 Slave_Address,u8 REG_Address)
{
	u8 REG_data;
	I2C_Start();                   //��ʼ�ź�
	 
	I2C_SendByte(Slave_Address);    //�����豸��ַ+д�ź�
 	if(!I2C_WaitAck()){I2C_Stop(); return FALSE;} 
	
	I2C_SendByte(REG_Address);     //���ʹ洢��Ԫ��ַ����0��ʼ
 	if(!I2C_WaitAck()){I2C_Stop(); return FALSE;} 
	
	I2C_Start();                   //��ʼ�ź�

	I2C_SendByte(Slave_Address+1);  //�����豸��ַ+���ź�
 	if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
	
	REG_data=I2C_RecvByte();       //�����Ĵ�������
	 
	I2C_SendACK(1);                //����ֹͣ�����ź�
	 
	I2C_Stop();                    //ֹͣ�ź�
	return REG_data;
}
//////////////////////////////////
////////////MS5611��λ///////////
bool MS561101BA_RESET() 

{  

 
	SCL_L;
	Delay_1us(6);
	SCL_H;
	Delay_1us(6);
	SCL_L;
	Delay_1us(6);
	SCL_H;
	Delay_1us(6);
	SCL_L;
	Delay_1us(6);
	SCL_H;
  I2C_Start();
  I2C_SendByte(0xEE); 
	if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
  I2C_SendByte(MS561101BA_RST);
	Delay_ms(8);
  if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
 
  I2C_Stop(); 

  return TRUE;
} 
///////////////////////////////////////////////
//////////��ȡPROM�еĳ���У׼ֵ///////////////
bool MS561101BA_PROM_READ() 

{ 

 u8 d1,d2,i; 
	
 for(i=0;i<8;i++) 

 { 
  
   I2C_Start(); 

  I2C_SendByte(MS561101BA_Addr); 
	if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
  
	I2C_SendByte((MS561101BA_PROM_RD+i*2)); //C1-C6����У׼���ݵ�ַA0-AE
	if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
  I2C_Start(); 

  I2C_SendByte(MS561101BA_Addr+1); 
  if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
  d1=I2C_RecvByte(); 

  I2C_SendACK(0); 

  d2=I2C_RecvByte(); 

  I2C_SendACK(1); 
  I2C_Stop(); 
  Cal_C[i]=((uint)d1<<8)|d2; //����У׼����

 } 
return TRUE;
}
/////////////////////////////////////////////////
/////��ȡ�����¶�ֵ������ѹ��ֵ��iic����//////
u32 MS561101BA_DO_CONVERSION(u8 command)
{ 

 u32 conversion=0; 
 u32 conv[3]={0,0,0};//conv2,conv3;  
 conversion=0;
 conv[0]=0;
 conv[1]=0;
 conv[2]=0;
 I2C_Start(); 
 I2C_SendByte(MS561101BA_Addr); //���͵�ַ
if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
 I2C_SendByte(command);//����D1/D2
if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
 I2C_Stop();
	switch(command&0x0f)
{
	case 0:Delay_1us(900);break;
	case 2:Delay_ms(6);break;
	case 4:Delay_ms(8);break;
	case 6:Delay_ms(12);break;
	case 8:Delay_ms(20);break;
}
 I2C_Start();
	
 I2C_SendByte(MS561101BA_Addr); //�ٷ���һ�ε�ַ
 if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
 I2C_SendByte(0); //����һ��0����
 if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
	I2C_Start(); 

 I2C_SendByte(MS561101BA_Addr+1); //���Ͷ���������
 if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}

 conv[0]=I2C_RecvByte(); //����8λ
 I2C_SendACK(0); 					//Ӧ��

 conv[1]=I2C_RecvByte(); //����8λ
 I2C_SendACK(0); 					//Ӧ��

 conv[2]=I2C_RecvByte(); //����8λ
 I2C_SendACK(1); //��Ӧ��
 I2C_Stop();  
 conversion=conv[0]*65536+conv[1]*256+conv[2]; //24bit����
 
 return conversion; 

}
//////////////////////////////////////////
///////��ȡ�����¶Ȳ�����ʵ���¶�/////////
void MS561101BA_getTemperature(u8 OSR_Temp) 

{ 
  int32_t dT_C;
 
	D2_Temp= MS561101BA_DO_CONVERSION(OSR_Temp); 
 
	dT=D2_Temp - ((Cal_C[5])*pow(2,8)); //�ο�Ӣ�������ֲ�Page 7

	dT_C=(dT*Cal_C[6])/pow(2,23);
 
	TEMP=2000+((dT*Cal_C[6])/pow(2,23)); //��ʽTEMP=2000+dT*C6/8388608
 
}
////////////////////////////////////////////////////
////��ȡ����ѹ��ֵ�����ö����¶Ȳ�������ʵ��ѹ��////
///////////�ο�Ӣ�������ֲ�Page 7-Page 8///////////
u8 MS561101BA_getPressure(u8 OSR_Temp) 

{ 

 D1_Pres= MS561101BA_DO_CONVERSION(OSR_Temp); //��ȡ24bit����
	if(D1_Pres==0)
	return FALSE; 
	
 OFF_=Cal_C[2]*pow(2,16)+((Cal_C[4]*dT)/pow(2,7)); //����ʵ���¶ȵ���
 
 SENS=(Cal_C[1]*pow(2,15))+((Cal_C[3]*dT)/pow(2,8)); //����ʵ���¶�������

 if(TEMP<2000) //�����¶Ȳ���

 { 

  // second order temperature compensation when under 20 degrees C 

  TEMP2 = (dT*dT)/pow(2,31); 

  Aux = (TEMP-2000)*(TEMP-2000); 
  
  OFF2 = 2.5*Aux; 

  SENS2 = 1.25*Aux; 

  TEMP = TEMP - TEMP2; 

  OFF_ = OFF_ - OFF2; 

  SENS = SENS - SENS2;  

 } 

 Pressure=((D1_Pres*SENS)/pow(2,21)-OFF_)/pow(2,15); //�¶Ȳ���ѹ����λmbar(1mbar=100pa)

 return TRUE;
}																							//0��C��׼����ѹΪ100kpa=1000mbar�����ͣ�
/////////////////////////////
void Exchange_Number() 

{ 
  ex_Pressure=Pressure; 

    if(ex_Pressure<0) 

 { 

  ex_Pressure=-ex_Pressure; 

  exchange_num[0]='-'; 

 } 

 else exchange_num[0]=' '; 

 exchange_num[1]=ex_Pressure/100000+0x30;//+0x30;

 ex_Pressure=ex_Pressure%100000; 

 exchange_num[2]=ex_Pressure/10000+0x30; 

 ex_Pressure=ex_Pressure%10000; 

 exchange_num[3]=ex_Pressure/1000+0x30; 

 ex_Pressure=ex_Pressure%1000; 

 exchange_num[4]=ex_Pressure/100+0x30; 

 ex_Pressure=ex_Pressure%100; 

 exchange_num[5]='.'; 

 exchange_num[6]=ex_Pressure/10+0x30; 

 ex_Pressure=ex_Pressure%10; 
 
 exchange_num[7]=ex_Pressure+0x30; 
 
}
void Init_MS561101BA() 

{ u8 i,n_crc,n=0;
	
	if(MS561101BA_RESET())

{
	
	   if(MS561101BA_PROM_READ())
		   n_crc=crc4(Cal_C);
		 if(n_crc==(Cal_C[7]&0x0f))
      USART1_printf(USART1,"\r\n Init_MS561101BA-OK\r\n");  
	}
}
/////////////MS5611 CRCУ�����///////////////
u8 crc4(u16 n_prom[]) 
{ 
	int16_t cnt;            // simple counter   
	u16 n_rem;         // crc reminder 
	u16 crc_read;        // original value of the crc 
	u8  n_bit; 
	n_rem = 0x00; 
	crc_read=n_prom[7];         //s ave read CRC 
	n_prom[7]=(0xFF00 & (n_prom[7]));     //CRC byte is replaced by 0 
	for (cnt = 0; cnt < 16; cnt++)        // operation is performed on bytes 
	{  // choose LSB or MSB 
		if (cnt%2==1)
		{
			n_rem ^= (u8) ((n_prom[cnt>>1]) & 0x00FF); 
		}
		else
		{
			n_rem ^= (u8) (n_prom[cnt>>1]>>8);
		}
        for (n_bit = 8; n_bit > 0; n_bit--) 
        { 
            if (n_rem & (0x8000)) 
            { 
				n_rem = (n_rem << 1) ^ 0x3000; 
            } 
            else 
            { 
				n_rem = (n_rem << 1); 
            } 
        } 
      } 
	n_rem=  (0x000F & (n_rem >> 12)); // // final 4-bit reminder is CRC code 
	n_prom[7]=crc_read;     // restore the crc_read to its original place 
	return (n_rem ^ 0x00); 
}
//////////////��ʼ��HMC5883L/////////////
void  Init_HMC5883L(void)
{
	Single_WriteI2C(MPU6050_Addr,0x6A,0x00);//close Master Mode
	
	Single_WriteI2C(MPU6050_Addr,0x37,0x02);//turn on Bypass Mode 
  
	Single_WriteI2C(HMC5883L_Addr,HMC58X3_R_CONFA,0x78);   // 8 samples averaged, 75Hz frequency, no artificial bias.

  Single_WriteI2C(HMC5883L_Addr,HMC58X3_R_CONFB,0x00);   //������Χ+��4.7Ga	390 counts/��˹

  Single_WriteI2C(HMC5883L_Addr,HMC58X3_R_MODE,0x00);    //
 
}

//**************************************
//��ʼ��MPU6050
//**************************************
void Init_MPU6050()
{	 
    
	Single_WriteI2C(MPU6050_Addr,PWR_MGMT_1, 0x00);	//�������״̬
	Single_WriteI2C(MPU6050_Addr,SMPLRT_DIV, 0x07);
	Single_WriteI2C(MPU6050_Addr,CONFIG, 0x06);
	Single_WriteI2C(MPU6050_Addr,GYRO_CONFIG, 0x18);
	Single_WriteI2C(MPU6050_Addr,ACCEL_CONFIG, 0x01);

}
//**************************************
//�ϳ�����
//**************************************
short int GetData_mpu6050(u8 REG_Address)
{
	char H,L;
	H=Single_ReadI2C(MPU6050_Addr,REG_Address);
	L=Single_ReadI2C(MPU6050_Addr,REG_Address+1);
	return ((H<<8)|L);   //�ϳ�����
}
////////��ȡHMC5883L����////////////
u8 GetData_hw5883(void)
{
	  u8 databuff[6]={0};
     
	  if(Single_ReadI2C(HMC5883L_Addr,0x09)&0x01)
	 {
			 
		    databuff[0]=Single_ReadI2C(HMC5883L_Addr,0x03);//OUT_X_L_A

        databuff[1]=Single_ReadI2C(HMC5883L_Addr,0x04);//OUT_X_H_A
	
				databuff[2]=Single_ReadI2C(HMC5883L_Addr,0x05);//OUT_Y_L_A
	      
				databuff[3]=Single_ReadI2C(HMC5883L_Addr,0x06);//OUT_Y_H_A
	
				databuff[4]=Single_ReadI2C(HMC5883L_Addr,0x07);//OUT_Z_L_A
	       
				databuff[5]=Single_ReadI2C(HMC5883L_Addr,0x08);//OUT_Z_H_A
	
	      Mag_x=(databuff[0] << 8) | databuff[1]; //Combine MSB and LSB of X Data output register
	      Mag_y=(databuff[2] << 8) | databuff[3]; //Combine MSB and LSB of Y Data output register
				Mag_z=(databuff[4] << 8) | databuff[5]; //Combine MSB and LSB of Z Data output register
	
	      if(Mag_x>0x7fff)Mag_x-=0xffff;	  
	      if(Mag_y>0x7fff)Mag_y-=0xffff;
		    if(Mag_z>0x7fff)Mag_z-=0xffff;
			
		   return 1;
	  }
	  return 0;    	  
}
////////////��ȡMS5611�¶Ⱥ���ѹ����//////////////
void GetData_ms5611()
{
	MS561101BA_getTemperature(MS561101BA_D2_OSR_4096); 	
	MS561101BA_getPressure(MS561101BA_D1_OSR_4096);	
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/