
#define MS561101BA_ADDR   0xee

// registers of the device
#define MS561101BA_D1 0x40
#define MS561101BA_D2 0x50
#define MS561101BA_RESET 0x1E

// D1 and D2 result size (bytes)
#define MS561101BA_D1D2_SIZE 3

// OSR (Over Sampling Ratio) constants
#define MS561101BA_OSR_256 0x00  //Conversion time 0.6ms  Resolution 0.065mbar
#define MS561101BA_OSR_512 0x02  //Conversion time 1.2ms  Resolution 0.042mbar
#define MS561101BA_OSR_1024 0x04 //Conversion time 2.3ms  Resolution 0.027mbar
#define MS561101BA_OSR_2048 0x06 //Conversion time 4.6ms  Resolution 0.018mbar
#define MS561101BA_OSR_4096 0x08 //Conversion time 9.1ms  Resolution 0.012mbar

#define  MS561101BA_D1_OSR_4096 0x48
#define  MS561101BA_D2_OSR_4096 0x58

#define MS561101BA_PROM_BASE_ADDR 0xA2
// by adding ints from 0 to 6 we can read all the prom configuration values.
// C1 will be at 0xA2 and all the subsequent are multiples of 2

#define MS561101BA_PROM_REG_COUNT 6 // number of registers in the PROM
#define MS561101BA_PROM_REG_SIZE 2 // size in bytes of a prom registry.

typedef struct
{
    int Temperature;
    float Pressure;
    float Pressure_Zero; // Pressure when take off
    float Altitude;
} MS5611_TypeDef;

extern volatile MS5611_TypeDef MS5611_Data;

void MS5611_Init(void);
void Read_MS5611(void);
