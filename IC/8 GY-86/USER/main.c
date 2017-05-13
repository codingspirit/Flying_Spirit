/*����ͷ------------------------------------------------------------------*/
#include "stm32f10x_conf.h"
#include "usart1.h"
#include "IIC.h"
#include  "delay.h"
#include "MP6050.h"
#include "MS5611.h"
#include "HMC5883L.h"
#include  <math.h> 

/*�궨��------------------------------------------------------------------*/


/*��������----------------------------------------------------------------*/
					 
extern uint16_t GYRO_XOUT,GYRO_YOUT,GYRO_ZOUT,
	         ACCEL_XOUT,ACCEL_YOUT,ACCEL_ZOUT,
           MP6050_Temperature;//X,Y,Z�ᣬ�¶�	;		 //�����Ǻͼ��ٶȼƵ�X,Y,Z�ᣬ�¶�	




/*��������----------------------------------------------------------------*/
extern void delay_init(u8 SYSCLK);
extern void delay_ms(uint16_t nms);
extern void delay_us(uint32_t nus);

extern void USART1_Conf(void);
extern void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...) ;
extern void Usart_Send(u8 *p,uint16_t len) ;


extern void I2C_GPIO_Configuration(void);
extern unsigned char I2C_ReadByte(unsigned char DeviceAddr,unsigned char address);  //��24c02�ĵ�ַaddress�ж�ȡһ���ֽ�����
extern void I2C_WriteByte(unsigned char DeviceAddr,unsigned char address,unsigned char info);
extern void I2C_NoAddr_WriteByte(unsigned char DeviceAddr,unsigned char info);

extern void Init_MPU6050(void);
extern void READ_MPU6050_Gyro(void);
extern void READ_MPU6050_Accel(void);
extern void MPU6050_WHO_AM_I(void);



extern void MS561101BA_Reset(void);
extern void MS561101BA_readPROM(void);
extern uint32_t MS561101BA_DO_CONVERSION(u8 command);
extern void MS561101BA_GetTemperature(u8 OSR_Temp);
extern void MS561101BA_GetPressure(u8 OSR_Pres);
extern void MS561101BA_Init(void);
extern void SampleANDExchange(void);

extern void Identify_HMC5883L(void);
extern void HMC5883L_Self_Test(void);//�Բ�
extern void Read_HMC5883L(void);//��ȡ

void LED_GPIO_Config(void);

/* Private functions------------------------------------------------------*/


/**
	* ������:  Main
	* ����:  None
  * ����� None
  */
int main(void)
{
 // int i,y=10;
 
	
  SystemInit();
	delay_init(72);
 	USART1_Conf();
  I2C_GPIO_Configuration();
  LED_GPIO_Config();
  GPIO_SetBits(GPIOA, GPIO_Pin_0);//PA0�ڵ���
  Init_MPU6050();
	MPU6050_WHO_AM_I();
		//GPIO_ResetBits(GPIOA, GPIO_Pin_0);//PA0�ڵ���
	MS561101BA_Init();
	Identify_HMC5883L();
	HMC5883L_Self_Test();
  while (1){
 		    READ_MPU6050_Gyro();   
   		  printf("�����ǣ�X��=%4d,Y��=%4d,Z��=%4d  ",GYRO_XOUT,GYRO_YOUT,GYRO_ZOUT);
    		 
    		READ_MPU6050_Accel();
    		printf("���ٶȼƣ��¶�=%4d,X��=%4d,Y��=%4d,Z��=%4d  ",MP6050_Temperature,ACCEL_XOUT,ACCEL_YOUT,ACCEL_ZOUT);

  		//  delay_ms(10);
		    printf("��ѹ��:");
	     	SampleANDExchange();
		    printf("��������:");
	      Read_HMC5883L();
		    printf("\r\n");
	 
	}

}
/************************************************************   
* ������:LED_GPIO_Config   
* ���� : ����PA0����led��  
* ����  :��   
* ���  :��    
*/  
void LED_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_OD;// GPIO_Mode_AF_OD;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

}




