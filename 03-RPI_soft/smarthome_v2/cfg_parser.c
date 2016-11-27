#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "cfg_parser.h"
#include "engine.h"

_ParamStruct *ParamStruct;
_GlobalParam GlobalParam;
_ModuleStruct *ModuleStruct;

int SensorNum, ModuleNum;

int cfg_parser(void)
{
  int retval, status;
  status = sensors_cfg_parser();
  status = global_cfg_parser();
  status = modules_cfg_parser();
  return retval;
}

int sensors_cfg_parser(void)
{
  FILE *cfg;
  int length, retcode, StartFlag = 0;
  char *str, strtmp[255];
  unsigned int i, tmp, pnum;

  str = malloc(1024);
  //ParamStruct = malloc(10*sizeof(_ParamStruct));
  printf("===== Configuration parser =====\n");
  printf("=== Sensors ===\n");
  printf("Opening configuration file... ");
  cfg = fopen("system.cfg", "r");
  // set file pointer to zero position
  if(cfg != 0)
  {
    printf("[OK]\n");
    SensorNum = 0;
    while(fgets(&str[0], 1023, cfg) != NULL)
    {
      if(str[0] == '#')
      {
        if(StartFlag == 1) StartFlag = 0;
        if(strstr(str, "Sensors") != NULL) StartFlag = 1;
      }
      else
      {
        if(StartFlag == 1)
        {
          SensorNum++;
        }
      }
    }
    printf("[%d] strings found...\n", SensorNum);
    printf("Allocating memory... ");
    ParamStruct = malloc(SensorNum * sizeof(_ParamStruct));
    if(ParamStruct != 0)
    {
      printf("[OK]\t[%d] bytes allocated\n", (int) (SensorNum * sizeof(_ParamStruct)));
    }
    else
    {
      retcode = -2;
      printf("[ERROR]\nError allocating memory! Exiting with [%d]!\n", retcode);
      return retcode;
    }
    fseek(cfg, 0, SEEK_SET);
    StartFlag = 0;
    i = 0;
    pnum = 0;
    while(fgets(&str[0], 1023, cfg) != NULL)
    {
      if(str[0] == '#')
      {
        if(StartFlag == 1) StartFlag = 0;
        if(strstr(str, "Sensors") != NULL) StartFlag = 1;
      }
      else
      {
        if(StartFlag == 1)
        {
          sscanf(str, "%x,%[^','],%d,%[^','],%[^','],%[^,','],%[^','],",
              &ParamStruct[i].sid,
              &ParamStruct[i].place,
              &ParamStruct[i].pnum,
              &ParamStruct[i].p_name[0][0],
              &ParamStruct[i].p_name[1][0],
              &ParamStruct[i].p_name[2][0],
              &ParamStruct[i].p_name[3][0]);
          printf("[%d]\n\tsid [%x]\n\tplace [%s]\n\tpnum [%d]",
              (int)i,
              ParamStruct[i].sid,
              ParamStruct[i].place,
              ParamStruct[i].pnum);
          for(tmp = 0; tmp < ParamStruct[i].pnum; tmp++)
          {
            printf("\n\tp%d_name[%s]", tmp, &ParamStruct[i].p_name[tmp][0]);
          }
          for(tmp = ParamStruct[i].pnum; tmp < 4; tmp++)
          {
            strcpy(&ParamStruct[i].p_name[tmp][0], "null");
          }
          printf("\n");
          pnum = pnum + ParamStruct[i].pnum;
          i++;
        }
      }
    }
    printf("Closing configuration file...");
    if(fclose(cfg) == 0)
    {
      printf("[done]\n");
      sprintf(strtmp, "Found [%d] devices, [%d] parameters!\n", i, pnum);
      write_syslog(strtmp, WITH_ECHO, NO_TIME);
      retcode = 0;
      return retcode;
    }
  }
  else
  {
    retcode = -1;
    printf("[ERROR]\n File open error! Exiting with [%d]!\n", retcode);
    return retcode;
  }
}


int global_cfg_parser(void)
{
  FILE *cfg;
  int length, retcode, StartFlag = 0, idxo, idxp;
  char *str, pname[255], pval[255], strtmp[255];
  unsigned int i, tmp, pnum;

  str = malloc(1024);
  printf("=== Global parameters ===\n");
  printf("Opening configuration file... ");
  cfg = fopen("system.cfg", "r");
  // set file pointer to zero position
  if(cfg != 0)
  {
    printf("[OK]\n");
    while(fgets(&str[0], 1023, cfg) != NULL)
    {
      if(str[0] == '#')
      {
        if(StartFlag == 1) StartFlag = 0;
        if(strstr(str, "System_config") != NULL) StartFlag = 1;
      }
      else
      {
        if(StartFlag == 1)
        {
          sscanf(str, "%[^'=']=%s", &pname, &pval);
          if(strstr(pname, "day_start_hour") != NULL)
          {
            GlobalParam.day_start_hour = (uint8_t) atoi(pval);
            //printf("day_start_hour = [%02d]\n", GlobalParam.day_start_hour);
          }
          if(strstr(pname, "day_start_min") != NULL)
          {
            GlobalParam.day_start_min = (uint8_t) atoi(pval);
            printf("Day start = [%02d:%02d]\n", GlobalParam.day_start_hour, GlobalParam.day_start_min);
          }
          if(strstr(pname, "night_start_hour") != NULL)
          {
            GlobalParam.night_start_hour = (uint8_t) atoi(pval);
            //printf("day_start_hour = [%02d]\n", GlobalParam.day_start_hour);
          }
          if(strstr(pname, "night_start_min") != NULL)
          {
            GlobalParam.night_start_min = (uint8_t) atoi(pval);
            printf("Night start = [%02d:%02d]\n", GlobalParam.night_start_hour, GlobalParam.night_start_min);
          }
          if(strstr(pname, "weekend_wdays") != NULL)
          {
            idxp = 0;
            idxo = 0;
            printf("Weekend days = [");
            for(i = 0; i < strlen(pval); i++)
            {
              if(pval[i] == ',')
              {
                strncpy(strtmp, (pval + idxo), (i - idxo));
                GlobalParam.weekend_wdays[idxp] = atoi(strtmp);
                printf("%d ", GlobalParam.weekend_wdays[idxp]);
                idxo = i + 1;
                idxp++;
              }
            }
            for(i = idxp; i < 6; i++)
            {
              GlobalParam.weekend_wdays[i] = -1;
              //printf("%d:[%02d] ", i, GlobalParam.weekend_wdays[i]);
            if(i == 5) printf("]\n");
            }
          }
          if(strstr(pname, "holidays_ydays") != NULL)
          {
            idxp = 0;
            idxo = 0;
            printf("Holidays = [");
            for(i = 0; i < strlen(pval); i++)
            {
              if(pval[i] == ',')
              {
                strncpy(strtmp, (pval + idxo), (i - idxo));
                GlobalParam.holidays_ydays[idxp] = atoi(strtmp);
                printf("%d ", GlobalParam.holidays_ydays[idxp]);
                idxo = i + 1;
                idxp++;
              }
            }
            for(i = idxp; i < 20; i++)
            {
              GlobalParam.holidays_ydays[i] = -1;
              //printf("%d:[%02d] ", i, GlobalParam.weekend_wdays[i]);
            if(i == 19) printf("]\n");
            }
          }

        }
      }
    }
    retcode = 0;
    return retcode;
  }
  else
  {
    retcode = -1;
    printf("[ERROR]\n File open error! Exiting with [%d]!\n", retcode);
    return retcode;
  }
}

int modules_cfg_parser(void)
{
  FILE *cfg;
  int length, retcode, StartFlag = 0, idxo, idxp, curid;
  char *str, pname[255], pval[255], strtmp[255];
  unsigned int i, tmp, pnum;

  str = malloc(1024);
  printf("=== Software modules ===\n");
  printf("Opening configuration file... ");
  cfg = fopen("system.cfg", "r");
  // set file pointer to zero position
  if(cfg != 0)
  {
    printf("[OK]\n");
    ModuleNum = 0;
    while(fgets(&str[0], 1023, cfg) != NULL)
    {
      if(strstr(str, "module_id") != NULL) ModuleNum++;
    }
    ModuleStruct = malloc(ModuleNum * sizeof(_ModuleStruct));
    if(ModuleStruct != 0)
    {
      //printf("[%d] bytes allocated\n", (int) (SensorNum * sizeof(_ParamStruct)));
    }
    else
    {
      retcode = -2;
      printf("[ERROR]\nError allocating memory! Exiting with [%d]!\n", retcode);
      return retcode;
    }
    fseek(cfg, 0, SEEK_SET);
    pnum = -1;
    while(fgets(&str[0], 1023, cfg) != NULL)
    {
      if(str[0] == '#')
      {
        if(StartFlag == 1)
        {
          StartFlag = 0;
//          printf("id [%d] type [%d]\n", ModuleStruct[pnum].id, ModuleStruct[pnum].type);
        }
      }
      else
      {
        if(strstr(str, "module_id") != NULL)
        {
          StartFlag = 1;
          pnum++;
        }
        if(StartFlag == 1)
        {
          sscanf(str, "%[^'=']=%s", &pname, &pval);
          if(strstr(pname, "module_id") != NULL)
          {
            ModuleStruct[pnum].id = (uint8_t) atoi(pval);
          }
          if(strstr(pname, "module_name") != NULL)
          {
            strcpy(ModuleStruct[pnum].name, pval);
          }
	  if(strstr(pname, "module_type") != NULL)
          {
            ModuleStruct[pnum].type = (uint8_t) atoi(pval);
          }
          //===== s_interval ==========
          if(strstr(pname, "s_interval") != NULL)
          {
            idxp = 0;
            idxo = 0;
            for(i = 0; i < 255; i++) strtmp[i] = 0;
            for(i = 0; i < strlen(pval); i++)
            {
              if(pval[i] == ',')
              {
                strncpy(strtmp, (pval + idxo), (i - idxo));
                ModuleStruct[pnum].s_interval[idxp] = atoi(strtmp);
//                printf("%d:[%02d] ", idxp, ModuleStruct[pnum].s_interval[idxp]);
                idxo = i + 1;
                idxp++;
              }
            }
            for(i = idxp; i < MAX_NUM_INTERVALS; i++)
            {
              ModuleStruct[pnum].s_interval[i] = -1;
//              printf("%d:[%02d] ", i, ModuleStruct[pnum].s_interval[i]);
//              if(i == (MAX_NUM_INTERVALS - 1)) printf("\n");
            }
          }
          //===== m_interval ==========
          if(strstr(pname, "m_interval") != NULL)
          {
            for(i = 0; i < 255; i++) strtmp[i] = 0;
            if(pval[0] == '*')
            {
              for(i = 0; i < MAX_NUM_INTERVALS; i++)
              {
                ModuleStruct[pnum].m_interval[i] = -2;
//                printf("%d:[%02d] ", i, ModuleStruct[pnum].m_interval[i]);
//                if(i == (MAX_NUM_INTERVALS - 1)) printf("\n");
              }
            }
            else
            {
              idxp = 0;
              idxo = 0;
              for(i = 0; i < strlen(pval); i++)
              {
                if(pval[i] == ',')
                {
                  strncpy(strtmp, (pval + idxo), (i - idxo));
                  ModuleStruct[pnum].m_interval[idxp] = atoi(strtmp);
//                  printf("%d:[%02d] ", idxp, ModuleStruct[pnum].m_interval[idxp]);
                  idxo = i + 1;
                  idxp++;
                }
              }
              for(i = idxp; i < MAX_NUM_INTERVALS; i++)
              {
                ModuleStruct[pnum].m_interval[i] = -1;
//                printf("%d:[%02d] ", i, ClimatCfgStruct.m_interval[i]);
//                if(i == (MAX_NUM_INTERVALS - 1)) printf("\n");
              }
            }
          }
          //===================================
        }
      }
    }
    for(tmp = 0; tmp <= pnum; tmp++)
    {
      printf("id [%d] name [%s] type [%d] s_int [", ModuleStruct[tmp].id, ModuleStruct[tmp].name, ModuleStruct[tmp].type);
      for(i = 0; i < MAX_NUM_INTERVALS; i++) printf("%2d ", ModuleStruct[tmp].s_interval[i]);
      printf("] m_int [");
      for(i = 0; i < MAX_NUM_INTERVALS; i++) printf("%2d ", ModuleStruct[tmp].m_interval[i]);
      printf("]\n");
    }
    printf("Software modules found: [%d]\n", ModuleNum);
  }
  else
  {
    retcode = -1;
    printf("[ERROR]\n File open error! Exiting with [%d]!\n", retcode);
    return retcode;
  }
}
