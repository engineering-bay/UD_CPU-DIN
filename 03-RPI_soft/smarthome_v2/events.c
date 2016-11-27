#include <stdint.h>
#include <stdio.h>
#include "cfg_parser.h"
#include "events.h"

int EventQueueLength = 0;
extern _ModuleStruct *ModuleStruct;
extern int ModuleNum;

_EventStruct* CreateEventQueue(int len)
{
  _EventStruct *event_s;
  int i;
  
  if(len >= MIN_EVENTS_NUM && len <= MAX_EVENTS_NUM)
  {
    event_s = malloc(len * sizeof(_EventStruct));
    if(event_s != 0)
    {
      for(i = 0; i < len; i++)
      {
        event_s[i].type = 0;
        event_s[i].cont = 0;
        event_s[i].id = 0;
        event_s[i].opt = 0;
        event_s[i].timestamp = 0;
      }
      EventQueueLength = len;
      printf("[Events] Events queue created successfully! Length [%d] Memory allocated [%d B] \n", len, (int) (len * sizeof(_EventStruct)));
      return(event_s);
    }
    else
    {
      printf("[Events] Error creating queue!\n");
      return 0;
    }
  }
  else
  {
    printf("[Events] Invalid argument [%d]!\n", len);
  }
  return 0;
}
// Returns number of free event slots in queue
int EventQueueStatus(_EventStruct *ptr)
{
  int i, ecount;
  ecount = 0;
  for(i = 0; i < EventQueueLength; i++)
  {
    if(ptr[i].type != 0 && ptr[i].id != 0) ecount++;
  }
  return (EventQueueLength - ecount);
}
int AddEventToQueue(_EventStruct *ptr, uint8_t type, uint8_t cont, uint16_t id, uint16_t opt, int ts)
{
  int i;
  if(EventQueueStatus(ptr) >= 1)
  {
    for(i = 0; i < EventQueueLength; i++)
    {
      if(ptr[i].type == 0 && ptr[i].id == 0)
      {
        ptr[i].type = type;
        ptr[i].cont = cont;
        ptr[i].id = id;
        ptr[i].opt = opt;
        ptr[i].timestamp = ts;
        return 0;
      }
    }
    return -2;
  }
  else return -1; // нет места в очереди
}

int RemoveEventFromQueue(_EventStruct *ptr, uint16_t id, uint16_t opt)
{
  int i;
  for(i = 0; i < EventQueueLength; i++)
  {
    if(ptr[i].id == id && ptr[i].opt == opt)
    {
      ptr[i].type = 0;
      ptr[i].cont = 0;
      ptr[i].id = 0;
      ptr[i].opt = 0;
      ptr[i].timestamp = 0;
      return 0;
    }
  }
  return -1;
}

int ClearEventQueue(_EventStruct *ptr)
{
  int i;
  for(i = 0; i < EventQueueLength; i++)
  {
      ptr[i].type = 0;
      ptr[i].cont = 0;
      ptr[i].id = 0;
      ptr[i].opt = 0;
      ptr[i].timestamp = 0;
  }
  return 0;
}

void PrintEventQueue(_EventStruct *ptr)
{
  int i;
  for(i = 0; i < EventQueueLength; i++)
  {
     printf("[%2d] [%d] [%d] [%04x] [%04x] [%d]\n", i,
      ptr[i].type,
      ptr[i].cont,
      ptr[i].id,
      ptr[i].opt,
      ptr[i].timestamp);
  }
}

void EventHandlers(_EventStruct *ptr)
{
  int i, n, j, c;
  sleep(2);
  while(1)
  {
    sleep(1);
    for(i = 0; i < EventQueueLength; i++)
    {
      c = 0;
      if(ptr[i].type != 0 && ptr[i].id != 0)
      {
        printf("Event [%4x] detected on slot [%d]!\n", ptr[i].id, i);
        // execute subscriber modules
	for(n = 0; n < ModuleNum; n++)
        {
          for(j = 0; j < MODULE_MAX_EVENTS_SUBS; j++)
          {
            if(ModuleStruct[n].subscribe_events[j] == ptr[i].id)
            {
              //execute module handler
              c++;
              break;
            }
          }
        }
        if(c > 0) printf("[HANDLER] %d modules executed for event [%d]\n", c, ptr[i].id);
        else printf("[HANDLER] No modules subscribed to event [%d]\n", ptr[i].id);
        RemoveEventFromQueue(ptr, ptr[i].id, ptr[i].opt);
      }
    }
  }
}
