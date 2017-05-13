#include "hmc5883l.h"
#include "i2c.h"

void HMC5883L_Init(void)
{
	uint8_t IDA,IDB,IDC;
	IDA=I2C_ReadByte(HMC58X3_ADDR,HMC58X3_R_IDA);
	IDB=I2C_ReadByte(HMC58X3_ADDR,HMC58X3_R_IDB);
	IDC=I2C_ReadByte(HMC58X3_ADDR,HMC58X3_R_IDC);
	if((IDA==0x48)&&(IDB==0x34)&&(IDC==0x33))//Identified
	{
		
	}
}
