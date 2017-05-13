typedef struct
{
	int BatteryMin;//Min Limit
	int BatteryADVal;//AD Value Right Now
	enum {Charging,Normal,Low}BatteryState;
}Battery_TypeDef;