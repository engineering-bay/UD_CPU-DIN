//==================================
// includes
//==================================
#include <stdint.h>
#include "cfg_parser.h"
//==================================
// definitions
//==================================
//==================================
// data structures
//==================================
typedef struct __ClimatCfgStruct
	{
	uint8_t enabled;
	int s_interval[MAX_NUM_INTERVALS];
	int m_interval[MAX_NUM_INTERVALS];
	char places[MAX_NUM_PLACES][64];
	uint8_t iterations;
	char inner_place[64];
	float inner_temp_high;
	float inner_temp_low;
	float inner_hum_high;
	float inner_hum_low;
	float inner_co2_high;
	float inner_co2_low;
	char outer_place[64];
	float outer_temp_high;
	float outer_temp_low;
	//float outer_press_high;
	//float outer_press_low;
	//float outer_place_wind_thr;
	//float outer_place_rain_thr;
	}_ClimatCfgStruct;
//==================================
// function prototypes
//==================================
int m_climat(void);
void m_climat_temp(int placeindex, _ParamStruct *ParamStruct, int SensorNum);
void m_climat_hum(int placeindex, _ParamStruct *ParamStruct, int SensorNum);
void m_climat_co2(int placeindex, _ParamStruct *ParamStruct, int SensorNum);
void ClimatModuleCfg(void);
