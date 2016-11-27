//==============================
// includes
//==============================
#include <stdint.h>
#include <stdlib.h>
//==============================
// definitions
//==============================
#define MAX_EVENTS_NUM			16
#define MIN_EVENTS_NUM			4
#define EVENT_TYPE_SYNC			1
#define EVENT_TYPE_ASYNC		2
#define EVENT_CONT_SINGLE		1
#define EVENT_CONT_LONGTERM		2

#define EVENT_ID_DAYTIME		0x0001
#define EVENT_OPT_DAY			0x0001
#define EVENT_OPT_NIGHT			0x0002
#define EVENT_OPT_WEEKEND		0x0003
#define EVENT_OPT_WORKDAY		0x0004
#define EVENT_OPT_HOLIDAY		0x0005

#define EVENT_ID_SENSOR_DATA_RECV	0x1001
#define EVENT_NOOPT			0
//==============================
// data structures
//==============================
typedef struct __EventStruct
	{
	uint8_t type;
	uint8_t cont;
	uint16_t id;
	uint16_t opt; // options: sensor type..., etc.
	int timestamp;
	}_EventStruct;
//==============================
// function prototypes
//==============================
_EventStruct* CreateEventQueue(int len);
int EventQueueStatus(_EventStruct *ptr);
int AddEventToQueue(_EventStruct *ptr, uint8_t type, uint8_t cont, uint16_t id, uint16_t opt, int ts);
int RemoveEventFromQueue(_EventStruct *ptr, uint16_t id, uint16_t opt);
void PrintEventQueue(_EventStruct *ptr);
void EventHandlers(_EventStruct *ptr);
