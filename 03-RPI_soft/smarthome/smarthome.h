#define SENSORS_COUNT		16
#define PARAMETERS_COUNT	5

struct _ParamStruct
{
	float p1;		// parameter 1
	float p2;		// parameter 2
	float vbat;		// battery voltage
	float rssi;		// RSSI
	int ts;			// timestamp
};
