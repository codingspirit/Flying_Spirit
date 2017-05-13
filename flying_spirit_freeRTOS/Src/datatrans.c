#include "datatrans.h"

void Data_Send_Status(float Pitch, float Roll, float Yaw)
{
    unsigned char i = 0;
    unsigned char _cnt = 0, sum = 0;
    unsigned int _temp;
    uint8_t data_to_send[50];

    data_to_send[_cnt++] = 0xAA;
    data_to_send[_cnt++] = 0xAA;
    data_to_send[_cnt++] = 0x01;
    data_to_send[_cnt++] = 0;

    _temp = (int)(Roll * 100);
    data_to_send[_cnt++] = BYTE1(_temp);
    data_to_send[_cnt++] = BYTE0(_temp);
    _temp = 0 - (int)(Pitch * 100);
    data_to_send[_cnt++] = BYTE1(_temp);
    data_to_send[_cnt++] = BYTE0(_temp);
    _temp = (int)(Yaw * 100);
    data_to_send[_cnt++] = BYTE1(_temp);
    data_to_send[_cnt++] = BYTE0(_temp);
    _temp = 0;
    data_to_send[_cnt++] = BYTE3(_temp);
    data_to_send[_cnt++] = BYTE2(_temp);
    data_to_send[_cnt++] = BYTE1(_temp);
    data_to_send[_cnt++] = BYTE0(_temp);

    data_to_send[_cnt++] = 0xA0;

    data_to_send[3] = _cnt - 4;

    //��У��
    for(i = 0; i < _cnt; i++)
        sum += data_to_send[i];

    data_to_send[_cnt++] = sum;

    //���ڷ�������
//	for(i=0;i<_cnt;i++)
//		usart_send_char(data_to_send[i]);
    CDC_Transmit_FS(data_to_send, _cnt);
}

/*�������ܣ���������������λ��Э��д����ʾ���������ݣ���λ��0512�汾��
 *����Э��˵����鿴��λ������İ���˵����
 */
void Send_Data(int16_t *Gyro, int16_t *Accel)
{
    unsigned char i = 0;
    unsigned char _cnt = 0, sum = 0;
//	unsigned int _temp;
    uint8_t data_to_send[50];

    data_to_send[_cnt++] = 0xAA;
    data_to_send[_cnt++] = 0xAA;
    data_to_send[_cnt++] = 0x02;
    data_to_send[_cnt++] = 0;


    data_to_send[_cnt++] = BYTE1(Accel[0]);
    data_to_send[_cnt++] = BYTE0(Accel[0]);
    data_to_send[_cnt++] = BYTE1(Accel[1]);
    data_to_send[_cnt++] = BYTE0(Accel[1]);
    data_to_send[_cnt++] = BYTE1(Accel[2]);
    data_to_send[_cnt++] = BYTE0(Accel[2]);

    data_to_send[_cnt++] = BYTE1(Gyro[0]);
    data_to_send[_cnt++] = BYTE0(Gyro[0]);
    data_to_send[_cnt++] = BYTE1(Gyro[1]);
    data_to_send[_cnt++] = BYTE0(Gyro[1]);
    data_to_send[_cnt++] = BYTE1(Gyro[2]);
    data_to_send[_cnt++] = BYTE0(Gyro[2]);
    data_to_send[_cnt++] = 0;
    data_to_send[_cnt++] = 0;
    data_to_send[_cnt++] = 0;
    data_to_send[_cnt++] = 0;
    data_to_send[_cnt++] = 0;
    data_to_send[_cnt++] = 0;

    data_to_send[3] = _cnt - 4;

    //��У��
    for(i = 0; i < _cnt; i++)
        sum += data_to_send[i];

    data_to_send[_cnt++] = sum;

    //���ڷ�������
//	for(i=0;i<_cnt;i++)
//		usart_send_char(data_to_send[i]);
    CDC_Transmit_FS(data_to_send, _cnt);
}
