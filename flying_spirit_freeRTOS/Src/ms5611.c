#include "ms5611.h"
#include "i2c.h"
#include "math.h"

uint16_t PROM_C[MS561101BA_PROM_REG_COUNT];
volatile MS5611_TypeDef MS5611_Data = {0};

void MS5611_Init()
{
    //RESET
    uint8_t resetAddr = MS561101BA_RESET;
    HAL_I2C_Master_Transmit(&hi2c1, MS561101BA_ADDR, &resetAddr, 1, 100);
    HAL_Delay(100);
    //READ PROM
    HAL_I2C_Mem_Read(&hi2c1, MS561101BA_ADDR, MS561101BA_PROM_BASE_ADDR, I2C_MEMADD_SIZE_16BIT, (uint8_t *)PROM_C, 6, 100);
}

void Read_MS5611(void)
{
    //GetTemperature
    uint8_t command = MS561101BA_D2_OSR_4096;
    uint8_t data[3];
    uint32_t conversion;
    uint64_t dT;
    int64_t off, sens;
    int64_t TEMP, T2, Aux_64, OFF2, SENS2;
    HAL_I2C_Master_Transmit(&hi2c1, MS561101BA_ADDR, &command, 1, 100);
    HAL_Delay(10);
    HAL_I2C_Master_Receive(&hi2c1, MS561101BA_ADDR, data, 3, 100);
    conversion = data[0] * 66536 + data[1] * 256 + data[2];
    dT = conversion - (((int32_t)PROM_C[4]) << 8);
    TEMP = 2000 + (dT * (int64_t)PROM_C[5]) / 8388608; //100 times of Temperature;

    //GetPressure
    command = MS561101BA_D1_OSR_4096;
    HAL_I2C_Master_Transmit(&hi2c1, MS561101BA_ADDR, &command, 1, 100);
    HAL_Delay(10);
    HAL_I2C_Master_Receive(&hi2c1, MS561101BA_ADDR, data, 3, 100);
    conversion = data[0] * 66536 + data[1] * 256 + data[2];

    off = (((int64_t)PROM_C[1]) << 16) + ((((int64_t)PROM_C[3]) * dT) >> 7);
    sens = (((int64_t)PROM_C[0]) << 15) + (((int64_t)(PROM_C[2]) * dT) >> 8);

    if(TEMP < 2000)
    {
        T2 = (((int64_t)dT) * dT) >> 31;
        Aux_64 = (TEMP - 2000) * (TEMP - 2000);
        OFF2 = (5 * Aux_64) >> 1;
        SENS2 = (5 * Aux_64) >> 2;
        TEMP = TEMP - T2;
        off = off - OFF2;
        sens = sens - SENS2;
    }

    MS5611_Data.Pressure = (((((int64_t)conversion) * sens) >> 21) - off) / 32768;
    MS5611_Data.Temperature = TEMP;

    //Calculate Altitude
    if(MS5611_Data.Pressure_Zero == 0)
    {
        MS5611_Data.Pressure_Zero = MS5611_Data.Pressure;
        MS5611_Data.Altitude = 0;
    }

    else
    {
        MS5611_Data.Altitude = 4433000.0 * (1 - pow((MS5611_Data.Pressure / MS5611_Data.Pressure_Zero), 0.1903)) * 0.01f;
    }
}
