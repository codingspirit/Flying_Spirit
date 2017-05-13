/*包含头------------------------------------------------------------------*/
#include "stm32f10x_conf.h"
#include "usart1.h"
#include "IIC.h"
#include  "delay.h"
#include "MP6050.h"
#include "MS5611.h"
#include "HMC5883L.h"
#include  <math.h> 

/*宏定义------------------------------------------------------------------*/


/*变量声明----------------------------------------------------------------*/
					 
extern uint16_t GYRO_XOUT,GYRO_YOUT,GYRO_ZOUT,
	         ACCEL_XOUT,ACCEL_YOUT,ACCEL_ZOUT,
           MP6050_Temperature;//X,Y,Z轴，温度	;		 //陀螺仪和加速度计的X,Y,Z轴，温度	




/*函数声明----------------------------------------------------------------*/
extern void delay_init(u8 SYSCLK);
extern void delay_ms(uint16_t nms);
extern void delay_us(uint32_t nus);

extern void USART1_Conf(void);
extern void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...) ;
extern void Usart_Send(u8 *p,uint16_t len) ;


extern void I2C_GPIO_Configuration(void);
extern unsigned char I2C_ReadByte(unsigned char DeviceAddr,unsigned char address);  //从24c02的地址address中读取一个字节数据
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
extern void HMC5883L_Self_Test(void);//自测
extern void Read_HMC5883L(void);//读取

void LED_GPIO_Config(void);

/* Private functions------------------------------------------------------*/


/**
	* 函数名:  Main
	* 输入:  None
  * 输出： None
  */
int main(void)
{
 // int i,y=10;
 
	
  SystemInit();
	delay_init(72);
 	USART1_Conf();
  I2C_GPIO_Configuration();
  LED_GPIO_Config();
  GPIO_SetBits(GPIOA, GPIO_Pin_0);//PA0口灯灭
  Init_MPU6050();
	MPU6050_WHO_AM_I();
		//GPIO_ResetBits(GPIOA, GPIO_Pin_0);//PA0口灯亮
	MS561101BA_Init();
	Identify_HMC5883L();
	HMC5883L_Self_Test();
  while (1){
 		    READ_MPU6050_Gyro();   
   		  printf("陀螺仪：X轴=%4d,Y轴=%4d,Z轴=%4d  ",GYRO_XOUT,GYRO_YOUT,GYRO_ZOUT);
    		 
    		READ_MPU6050_Accel();
    		printf("加速度计：温度=%4d,X轴=%4d,Y轴=%4d,Z轴=%4d  ",MP6050_Temperature,ACCEL_XOUT,ACCEL_YOUT,ACCEL_ZOUT);

  		//  delay_ms(10);
		    printf("气压计:");
	     	SampleANDExchange();
		    printf("数字罗盘:");
	      Read_HMC5883L();
		    printf("\r\n");
	 
	}

}
/************************************************************   
* 函数名:LED_GPIO_Config   
* 描述 : 配置PA0驱动led灯  
* 输入  :无   
* 输出  :无    
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




