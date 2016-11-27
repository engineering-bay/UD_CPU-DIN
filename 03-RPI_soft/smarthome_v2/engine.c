#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "engine.h"
#include "cfg_parser.h"
#include "events.h"

int SignalState;
_GlobalStatus GlobalStatus;
extern _GlobalParam GlobalParam;
extern _EventStruct *events;

void CheckDayNight(time_t rawtime, struct tm local);
void CheckWeekend(time_t rawtime, struct tm local);
void CheckHoliday(time_t rawtime, struct tm local);
void CheckSchedule(time_t rawtime, struct tm local);

void main_cycle(void)
{
  int i, n, dtime, ntime, ctime, status;
  time_t rawtime;
  struct tm local;

  SignalState = SIGNAL_WORK;
  GlobalStatus.daytime = STATUS_UNKNOWN;
  GlobalStatus.weekend = STATUS_UNKNOWN;
  GlobalStatus.holiday = STATUS_UNKNOWN;
  GlobalStatus.mute = STATUS_DISABLED;
  while(SignalState == SIGNAL_WORK)
  {
    sleep(1);
    time(&rawtime);
    localtime_r(&rawtime, &local);
    if(local.tm_hour == 0 && local.tm_min == 0 && local.tm_sec == 0)
    {
      GlobalStatus.weekend = STATUS_UNKNOWN;
      GlobalStatus.holiday = STATUS_UNKNOWN;
    }
    // check for day/night
    CheckDayNight(rawtime, local);
    // check for weekend
    if(GlobalStatus.weekend == STATUS_UNKNOWN)
    {
      CheckWeekend(rawtime, local);
    }
    // check for holiday
    if(GlobalStatus.holiday == STATUS_UNKNOWN)
    {
      CheckHoliday(rawtime, local);
    }
  }
}

void CheckDayNight(time_t rawtime ,struct tm local)
{
  int i, n, dtime, ntime, ctime, status;

    dtime = GlobalParam.day_start_hour*60 + GlobalParam.day_start_min;
    ntime = GlobalParam.night_start_hour*60 + GlobalParam.night_start_min;
    ctime = local.tm_hour*60 + local.tm_min;
    if(ctime >=dtime && ctime < ntime)
    {
      if(GlobalStatus.daytime != STATUS_DAY)
      {
        GlobalStatus.daytime = STATUS_DAY;
        // Generate event
        status = AddEventToQueue(events, EVENT_TYPE_SYNC, EVENT_CONT_SINGLE, EVENT_ID_DAYTIME, EVENT_OPT_DAY, rawtime);
        if(status == 0)
        {
          printf("[%02d:%02d:%02d] [ENGINE] Event ID [%04x][%04x] generated!\n", local.tm_hour, local.tm_min, local.tm_sec, EVENT_ID_DAYTIME, EVENT_OPT_DAY);
          printf("[Event] Event free slots remaining [%d]\n", EventQueueStatus(events));
          //PrintEventQueue(events);
        }
        else printf("[%02d:%02d:%02d] [ENGINE] Event generation FAILED with [%d]!\n", local.tm_hour, local.tm_min, local.tm_sec, status);
      }
    }
    else
    {
      if(GlobalStatus.daytime != STATUS_NIGHT)
      {
        GlobalStatus.daytime = STATUS_NIGHT;
        // Generate event
        status = AddEventToQueue(events, EVENT_TYPE_SYNC, EVENT_CONT_SINGLE, EVENT_ID_DAYTIME, EVENT_OPT_NIGHT, rawtime);
        if(status == 0)
        {
          printf("[%02d:%02d:%02d] [ENGINE] Event ID [%04x][%04x] generated!\n", local.tm_hour, local.tm_min, local.tm_sec, EVENT_ID_DAYTIME, EVENT_OPT_NIGHT);
          printf("[Event] Event free slots remaining [%d]\n", EventQueueStatus(events));
          //PrintEventQueue(events);
        }
        else printf("[%02d:%02d:%02d] [ENGINE] Event generation FAILED with [%d]!\n", local.tm_hour, local.tm_min, local.tm_sec, status);
      }
    }
}

void CheckWeekend(time_t rawtime, struct tm local)
{
  int i, status;
  for(i = 0; i < sizeof(GlobalParam.weekend_wdays)/sizeof(int); i++)
  {
    if(GlobalParam.weekend_wdays[i] == local.tm_wday)
    {
      GlobalStatus.weekend = 1; 
      // Generate event
      status = AddEventToQueue(events, EVENT_TYPE_SYNC, EVENT_CONT_SINGLE, EVENT_ID_DAYTIME, EVENT_OPT_WEEKEND, rawtime);
      if(status == 0)
      {
        printf("[%02d:%02d:%02d] [ENGINE] Event ID [%04x][%04x] generated!\n", local.tm_hour, local.tm_min, local.tm_sec, EVENT_ID_DAYTIME, EVENT_OPT_WEEKEND);
        printf("[Event] Event free slots remaining [%d]\n", EventQueueStatus(events));
        //PrintEventQueue(events);
      }
      else
      {
        printf("[%02d:%02d:%02d] [ENGINE] Event generation FAILED with [%d]!\n", local.tm_hour, local.tm_min, local.tm_sec, status);   
      }
      break;
    }
  }
  if(GlobalStatus.weekend == STATUS_UNKNOWN)
  {
    GlobalStatus.weekend = 0;
    // Generate event
    status = AddEventToQueue(events, EVENT_TYPE_SYNC, EVENT_CONT_SINGLE, EVENT_ID_DAYTIME, EVENT_OPT_WORKDAY, rawtime);
    if(status == 0)
    {
      printf("[%02d:%02d:%02d] [ENGINE] Event ID [%04x][%04x] generated!\n", local.tm_hour, local.tm_min, local.tm_sec, EVENT_ID_DAYTIME, EVENT_OPT_WORKDAY);
      printf("[Event] Event free slots remaining [%d]\n", EventQueueStatus(events));
      //PrintEventQueue(events);
    }
    else
    {
      printf("[%02d:%02d:%02d] [ENGINE] Event generation FAILED with [%d]!\n", local.tm_hour, local.tm_min, local.tm_sec, status);   
    }
  }
}

void CheckHoliday(time_t rawtime, struct tm local)
{
  int i, status;
  for(i = 0; i < sizeof(GlobalParam.weekend_wdays)/sizeof(int); i++)
  {
    if(GlobalParam.holidays_ydays[i] == local.tm_yday)
    {
      GlobalStatus.holiday = 1; 
      // Generate event
      status = AddEventToQueue(events, EVENT_TYPE_SYNC, EVENT_CONT_SINGLE, EVENT_ID_DAYTIME, EVENT_OPT_HOLIDAY, rawtime);
      if(status == 0)
      {
        printf("[%02d:%02d:%02d] [ENGINE] Event ID [%04x][%04x] generated!\n", local.tm_hour, local.tm_min, local.tm_sec, EVENT_ID_DAYTIME, EVENT_OPT_HOLIDAY);
        printf("[Event] Event free slots remaining [%d]\n", EventQueueStatus(events));
        //PrintEventQueue(events);
      }
      else
      {
        printf("[%02d:%02d:%02d] [ENGINE] Event generation FAILED with [%d]!\n", local.tm_hour, local.tm_min, local.tm_sec, status);   
      }
      break;
    }
  }
  if(GlobalStatus.weekend == STATUS_UNKNOWN)
  {
    GlobalStatus.weekend = 0;
    // Generate event
    status = AddEventToQueue(events, EVENT_TYPE_SYNC, EVENT_CONT_SINGLE, EVENT_ID_DAYTIME, EVENT_OPT_WORKDAY, rawtime);
    if(status == 0)
    {
      printf("[%02d:%02d:%02d] [ENGINE] Event ID [%04x][%04x] generated!\n", local.tm_hour, local.tm_min, local.tm_sec, EVENT_ID_DAYTIME, EVENT_OPT_WORKDAY);
      printf("[Event] Event free slots remaining [%d]\n", EventQueueStatus(events));
      //PrintEventQueue(events);
    }
    else
    {
      printf("[%02d:%02d:%02d] [ENGINE] Event generation FAILED with [%d]!\n", local.tm_hour, local.tm_min, local.tm_sec, status);   
    }
  }
}


void CheckSchedule(time_t rawtime, struct tm local)
{
  ;
}

