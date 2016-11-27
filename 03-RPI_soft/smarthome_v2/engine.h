//==============================
// includes
//==============================
#include <stdint.h>
#include <stdlib.h>
//==============================
// definitions
//==============================
//#define _DEBUG_MODE
#define SIGNAL_STOP		0
#define SIGNAL_WORK		1
#define MAX_NUM_INTERVALS	6
#define MAX_NUM_PLACES		5
#define WITH_TIME		1
#define NO_TIME			0
#define WITH_ECHO		1
#define NO_ECHO			0
#define STATUS_UNKNOWN		-1
#define STATUS_ENABLED		1
#define STATUS_DISABLED		2
#define STATUS_DAY		3
#define STATUS_NIGHT		4
#define STATUS_WEEKEND		5
#define STATUS_WORKDAY		6
#define STATUS_HOLIDAY		7
//==============================
// data structures
//==============================
typedef struct __GlobalStatus
	{
	int daytime;  
	int weekend;   
	int holiday;  
	int mute;
	}_GlobalStatus;
//==============================
// function prototypes
//==============================
void main_cycle(void);
int write_syslog(char *str, int echo, int tt);
