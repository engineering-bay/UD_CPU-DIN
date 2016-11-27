#include <stdio.h>
#include <time.h>
#include "engine.h"
//#include "cfg_parser.h"
#include "m_registrator.h"

extern  int SensorNum;
extern _ParamStruct *ParamStruct;

int m_registrator(void)
{
  time_t rawtime;
  struct tm local;
  int i;
  char fname[32];
  FILE *log;

  printf("Registrator module started\n");
  time(&rawtime);
  localtime_r(&rawtime, &local);
  sprintf(fname, "./logs/data/%04d-%02d-%02d.log", local.tm_year + 1900, local.tm_mon + 1, local.tm_mday);

  log = fopen(fname, "r");
  // if file does not exist
  if(log == NULL)
  {
    // open file for append
    log = fopen(fname, "a+");
    if(log != NULL)
    {
      for(i = 0; i < SensorNum; i++)
      {
        // write header to log
        fprintf(log, "#%04X:%s:%s:%s:%s:%s\n", ParamStruct[i].sid, ParamStruct[i].place,
                                       ParamStruct[i].p_name[0], ParamStruct[i].p_name[1], ParamStruct[i].p_name[2], ParamStruct[i].p_name[3]);
      }
      // write data
      printf("Added:\n");
      for(i = 0; i < SensorNum; i++)
      {
        printf("%.1f %.1f %.1f %.1f %ld ", ParamStruct[i].p_val[0], ParamStruct[i].p_val[1], ParamStruct[i].p_val[2], ParamStruct[i].p_val[3], ParamStruct[i].timestamp);
        fprintf(log, "%.1f %.1f %.1f %.1f %ld ", ParamStruct[i].p_val[0], ParamStruct[i].p_val[1], ParamStruct[i].p_val[2], ParamStruct[i].p_val[3], ParamStruct[i].timestamp);
      }
      printf("\n");
      fprintf(log, "\n");
      fclose(log);
    }
    else
    {
      write_syslog("[Registrator] Error opening log file for writing!\n", WITH_ECHO, WITH_TIME);
    }
  }
  // if exist
  else
  {
    fclose(log);
    // open file for append
    log = fopen(fname, "a+");
    if(log != NULL)
    {
      // write data
      printf("Added:\n");
      for(i = 0; i < SensorNum; i++)
      {
        printf("%.1f %.1f %.1f %.1f %ld ", ParamStruct[i].p_val[0], ParamStruct[i].p_val[1], ParamStruct[i].p_val[2], ParamStruct[i].p_val[3], ParamStruct[i].timestamp);
        fprintf(log, "%.1f %.1f %.1f %.1f %ld ", ParamStruct[i].p_val[0], ParamStruct[i].p_val[1], ParamStruct[i].p_val[2], ParamStruct[i].p_val[3], ParamStruct[i].timestamp);
      }
      printf("\n");
      fprintf(log, "\n");
      fclose(log);
    }
    else
    {
      write_syslog("[Registrator] Error opening log file for writing!\n", WITH_ECHO, WITH_TIME);
    }

  }


  return 0;
}
