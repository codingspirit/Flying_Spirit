#include "usart.h"

#define BT_BAUD 115200
#define BT_NAME "Flying_Spirit"
#define BT_PIN "1234"

typedef struct
{
	int MsgLength;
	unsigned char Msg[];
}SendingMsg_TypeDef;

void BlueTooth_Send(void);
