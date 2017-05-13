#include "usbd_cdc_if.h"

#define BYTE0(dwTemp)          (*(char *)(&dwTemp))
#define BYTE1(dwTemp)          (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)          (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)          (*((char *)(&dwTemp) + 3))

void Data_Send_Status(float Pitch, float Roll, float Yaw);
void Send_Data(int16_t *Gyro, int16_t *Accel);
