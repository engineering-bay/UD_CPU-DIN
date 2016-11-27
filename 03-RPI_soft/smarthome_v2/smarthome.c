#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <pthread.h>
#include "cfg_parser.h"
#include "queue.h"
#include "engine.h"
#include "events.h"

#define ACT_QUEUE_LENGTH	32
extern _ParamStruct *ParamStruct;
extern int SensorNum;
_EventStruct *events;
_QStruct *queue1;

//int write_syslog(char *str);

int main(void)
{
  pthread_t hnd_thread, rf_thread;
  char *strtmp;
  int status = -1, i, n, tmp;
  float fl;

  write_syslog("=== SmartHome utility started! ===\n", WITH_ECHO, WITH_TIME);
  status = cfg_parser();
  if(status == 0)
  {
    write_syslog("Configuration parsed succesfully!\n", WITH_ECHO, NO_TIME);
    for(i = 0; i < SensorNum; i++)
    {
      tmp = ParamStruct[i].pnum;
      printf("[%04X] ", ParamStruct[i].sid);
      for(n = 0; n < tmp; n++)
      {
        printf("%f ", ParamStruct[i].p_val[n]);
      }
      printf("\n");
    }
    //write_syslog("=== Checking software modules ===\n", WITH_ECHO, NO_TIME);
    //ClimatModuleCfg();
    //RegistratorModuleCfg();
    queue1 = QueueCreate(ACT_QUEUE_LENGTH);
    if(queue1 != 0)
    {
      QueueStatus(queue1);
      events = CreateEventQueue(MAX_EVENTS_NUM);
      if(events != 0)
      {
        printf("[Event] Free slots in event queue [%d]\n", EventQueueStatus(events));
        status = pthread_create(&hnd_thread, NULL, EventHandlers, events);
        if(status!=0)
        {
          perror("[SYSTEM] Handlers thread creation failed");
          exit(EXIT_FAILURE);
        }
        main_cycle();
      }
      else
      {
        return -3;
      }
    }
    else
    {
      return -2;
    }
  }
  else
  {
    printf(strtmp, "Configuration parsing error! Errno = [%d]", -1);
//    write_syslog(strtmp, 1);
    return -1;
  }
}

int write_syslog(char *str, int echo, int tt)
{
  int status;
  char filename[255];
  FILE *syslog;
  time_t rawtime;
  struct tm local;

  time(&rawtime);
  localtime_r(&rawtime, &local);
  if(echo == 1) printf("%s", str);
  sprintf(filename, "./logs/system/%04d-%02d-%02d_system.log", local.tm_year + 1900, local.tm_mon + 1, local.tm_mday);
  syslog = fopen(filename, "a+");
  if(syslog != NULL)
  {
    if(tt == 1) fprintf(syslog, "%02d:%02d:%02d %s", local.tm_hour, local.tm_min, local.tm_sec, str);
    else fprintf(syslog, "%s", str);
    fclose(syslog);
    return 0;
  }
  else
  {
    return -1;
  }
}
