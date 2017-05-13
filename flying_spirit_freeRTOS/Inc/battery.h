#define BAT_CHECK_PERIOD 5000 //unit ms
#define BAT_CHARG_VAL 1.0//when charging adc is NC unit v
#define BAT_ALARM_VAL 3.7


typedef struct
{
    float BatteryVoltage;
    enum
    {
        Charging,
        Normal,
        Alarm
    } BatteryState;
} Battery_TypeDef;

void BatteryCheckInit(void);
void BatteryCheck(void);
