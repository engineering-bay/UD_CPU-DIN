#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

_QStruct* QueueCreate(int QueueLength)
{
  _QStruct *queue_s;
  int i;

  if(QueueLength >= MIN_QUEUE_LENGTH && QueueLength <= MAX_QUEUE_LENGTH)
  {
    queue_s = malloc(QueueLength * sizeof(_QStruct));
    if(queue_s != 0)
    {
      for(i = 0; i < QueueLength; i++)
      {
        queue_s[i].qlen = QueueLength;
        queue_s[i].sstat = 0;
        queue_s[i].qnum = 0;
        queue_s[i].actlen = 0;
      }
      printf("[QueueCreate] Queue created successfully! Length [%d] Memory allocated [%d B] \n", QueueLength, (int) (QueueLength * sizeof(_QStruct)));
      return(queue_s);
    }
    else
    {
      printf("[QueueCreate] Error creating queue!\n");
      return 0;
    }
  }
  else
  {
    printf("[QueueCreate] Invalid argument [%d]!\n", QueueLength);
  }
}

void QueueStatus(_QStruct *queue_s)
{
  int i, freecount = 0, usedcount = 0, qlen;

  qlen = queue_s[0].qlen;
  if(qlen >= MIN_QUEUE_LENGTH && qlen <= MAX_QUEUE_LENGTH)
  {
    for(i = 0; i < queue_s[0].qlen; i++)
    {
      if(queue_s[i].sstat == 0)
      {
        freecount++;
      }
      else
      {
        usedcount++;
      }
    }
    printf("[QueueStatus] Queue length [%d], used slots [%d], free slots [%d]\n", qlen, usedcount, freecount);
  }
  else
  {
    printf("[QueueStatus] ERROR! Invalid pointer to queue!\n");
  }
}
