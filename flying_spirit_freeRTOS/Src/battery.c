#include "battery.h"
#include "adc.h"

Battery_TypeDef Battery;
uint32_t ADC_ConvertedValue;

void BatteryCheckInit()
{
    HAL_ADC_Start_DMA(&hadc1, &ADC_ConvertedValue, sizeof(ADC_ConvertedValue));
}

void BatteryCheck()
{
    Battery.BatteryVoltage = (float)(ADC_ConvertedValue & 0xFFF) * 6.6 / 4096;

    if(Battery.BatteryVoltage <= BAT_CHARG_VAL) //charging
    {
        Battery.BatteryState = Charging;
    }

    else if(Battery.BatteryVoltage <= BAT_ALARM_VAL)//low voltage
    {
        Battery.BatteryState = Alarm;
    }

    else Battery.BatteryState = Normal;
}
