//==============================
// includes
//==============================
#include <stdint.h>
#include <stdlib.h>
//==============================
// definitions
//==============================
#define MODULE_TYPE_SYNC	1
#define MODULE_TYPE_ASYNC	2
#define MODULE_MAX_EVENTS_SUBS	5
#define MAX_NUM_INTERVALS	6
#define MAX_NUM_PLACES		5
//==============================
// data structures
//==============================
typedef struct __ParamStruct
	{
	uint16_t sid;
	char place[64];
	uint16_t pnum;
	float p_val[4];
	char p_name[4][64];
	uint32_t timestamp;
	}_ParamStruct;
typedef struct __GlobalParam
	{
	uint8_t day_start_hour;
	uint8_t day_start_min;
	uint8_t night_start_hour;
	uint8_t night_start_min;
	int weekend_wdays[6];
	int holidays_ydays[20];
	}_GlobalParam;
typedef struct __ModuleStruct
	{
	uint8_t id;
        char name[32];
	uint8_t type;
	uint8_t enabled;
	uint8_t reserved;
	int s_interval[MAX_NUM_INTERVALS];
	int m_interval[MAX_NUM_INTERVALS];
	uint16_t subscribe_events[MODULE_MAX_EVENTS_SUBS];
	uint16_t subscribe_event_opt[MODULE_MAX_EVENTS_SUBS];
	}_ModuleStruct;
//==============================
// function prototypes
int cfg_parser(void);
int sensors_cfg_parser(void);
int global_cfg_parser(void);
int modules_cfg_parser(void);
