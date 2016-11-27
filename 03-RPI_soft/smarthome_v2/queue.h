//==============================
// includes
//==============================
#include <stdint.h>
#include <stdlib.h>
//==============================
// definitions
#define MIN_QUEUE_LENGTH	1
#define MAX_QUEUE_LENGTH	255
#define QUEUE_MSG_LENGTH	32
//==============================
// data structures
//==============================
typedef struct __QStruct
	{
	uint8_t qlen; // queue length
	uint8_t sstat; // queue slot status: [0] - free (allow write), [1] - used (contain data)
	uint8_t qnum;  // number in queue (least number pulled out first)
        uint8_t actlen; // actual data length (may differ from, but must be <= QUEUE_MSG_LENGTH)
 	uint8_t data[QUEUE_MSG_LENGTH]; //data
	}_QStruct;
//==============================
// function prototypes
//==============================
/*
* QueueCreate(int)
* Create queue - allocate memory
* Arguments:
* (int) QueueLength -  queue length (number of slots), must be >0
* Return value:
* (void)
*/
_QStruct* QueueCreate(int QueueLength);

void QueueStatus(_QStruct *queue_s);

void QueuePush(_QStruct *queue_s, uint8_t actlen, void *data);

void QueuePull(_QStruct *queue_s, void *ptr);
