#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include "smarthome.h"

extern void *t_logger(void *arg);
extern void *socket_handler(void *arg);
extern void *usb_getdata(void *arg);

struct _ParamStruct param[SENSORS_COUNT];
//float param_acc[SENSORS_COUNT];
//int acc_index[SENSORS_COUNT];

void main_home (void)
{
FILE *init_data;
//time_t ta;
int i, j, n = 0, stat, sid = 0;
float tmp1, tmp2, tmp3, tmp4;
//char filename[20];
//struct tm *tm_ptr;
pthread_t a_thread, tm_thread, rf_thread;

for(i=0;i<SENSORS_COUNT;i++)
{
	param[i].p1 = 0.0;
	param[i].p2 = 0.0;
	param[i].vbat = 0.0;
	param[i].rssi = 0.0;
	param[i].ts = 0;
}
// get init data (counters)
/*init_data = fopen("./logs/counter.log", "r");
while(!feof(init_data))
{
	fscanf(init_data, "%2x %f %f %f %f", &sid, &tmp1, &tmp2, &tmp3, &tmp4);
	param[sid][0] = tmp1;
	param[sid][1] = tmp2;
	param[sid][2] = tmp3;
	param[sid][3] = tmp4;
        param[sid][4] = -200.0;
}
fclose(init_data);*/

/*for(i=0;i<SENSORS_COUNT;i++)
{
	printf("[%2x] %.3f %.3f %4.0f %4.2f\n", i, param[i][0], param[i][1], param[i][2], param[i][3]);
}*/

stat = pthread_create(&a_thread, NULL, socket_handler, NULL);
if(stat!=0)
{
  perror("LAN Thread creation failed");
  exit(EXIT_FAILURE);
}
stat = pthread_create(&tm_thread, NULL, t_logger, NULL);
if(stat!=0)
{
  perror("TIMER Thread creation failed");
  exit(EXIT_FAILURE);
}
stat = pthread_create(&rf_thread, NULL, usb_getdata, NULL);
if(stat!=0)
{
  perror("RF Thread creation failed");
  exit(EXIT_FAILURE);
}

while(1)
{
  //stat = usb_getdata();
  //printf("%5.1f %5.1f %5.1f\n", param[3][0], param[3][1], param[3][2]);
  sleep(1);
}
}
