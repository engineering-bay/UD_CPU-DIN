#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "cfg_parser.h"
#include "m_climat.h"

_ClimatCfgStruct ClimatCfgStruct;
extern _ParamStruct *ParamStruct;
extern int SensorNum;

int m_climat(void)
{
  int i;

  ClimatModuleCfg();
  if(ClimatCfgStruct.enabled == 1)
  {
    printf("[CLIMAT] Climatic module started!\n");

    for(i = 0; i < ClimatCfgStruct.iterations; i++)
    {
      printf("   [%d of %d] [%s]\n", i+1, ClimatCfgStruct.iterations, ClimatCfgStruct.places[i]);
      //m_climat_temp(i);
      //m_climat_hum(i);
      //m_climat_co2(i);
    }
  }
  return 0;
}

void m_climat_temp(int placeindex, _ParamStruct *ParamStruct, int SensorNum)
{
  int n, p;

  //printf("\t=== Analyzing temperature ===\n");
  for(n = 0; n < SensorNum; n++)
  {
    if(strstr(ParamStruct[n].place, ClimatCfgStruct.places[placeindex]) != NULL)
    {
      for(p = 0; p < ParamStruct[n].pnum; p++)
      {
        if(strstr(ParamStruct[n].p_name[p], "Температура") != NULL)
        {
          printf("\t%s [%.1f] ", ParamStruct[n].p_name[p], ParamStruct[n].p_val[p]);
          // inner_place
          if(strstr(ClimatCfgStruct.places[placeindex], ClimatCfgStruct.inner_place) != NULL)
          {
            if(ParamStruct[n].p_val[p] < ClimatCfgStruct.inner_temp_low)
            {
              printf("[LOW] ");
              //low_temp_action();
            }
            if(ParamStruct[n].p_val[p] > ClimatCfgStruct.inner_temp_high)
            {
              printf("[HIGH] ");
              //high_temp_action();
            }
            if(ParamStruct[n].p_val[p] < ClimatCfgStruct.inner_temp_high && ParamStruct[n].p_val[p] > ClimatCfgStruct.inner_temp_low)
            {
              printf("[NORMAL] ");
              // no action required
            }
          }
          // outer_place
          if(strstr(ClimatCfgStruct.places[placeindex], ClimatCfgStruct.outer_place) != NULL)
          {
            if(ParamStruct[n].p_val[p] < ClimatCfgStruct.outer_temp_low)
            {
              printf("[LOW] ");
            }
            if(ParamStruct[n].p_val[p] > ClimatCfgStruct.outer_temp_high)
            {
              printf("[HIGH] ");
            }
            if(ParamStruct[n].p_val[p] < ClimatCfgStruct.outer_temp_high && ParamStruct[n].p_val[p] > ClimatCfgStruct.outer_temp_low)
            {
              printf("[NORMAL] ");
            }
          }
          printf("\n");
        }
      }
    }
  }
}

void m_climat_hum(int placeindex, _ParamStruct *ParamStruct, int SensorNum)
{
  int n, p;

  //printf("\t=== Analyzing humidity ===\n");
  for(n = 0; n < SensorNum; n++)
  {
    if(strstr(ParamStruct[n].place, ClimatCfgStruct.places[placeindex]) != NULL)
    {
      for(p = 0; p < ParamStruct[n].pnum; p++)
      {
        if(strstr(ParamStruct[n].p_name[p], "Относительная влажность") != NULL)
        {
          printf("\t%s [%.1f] ", ParamStruct[n].p_name[p], ParamStruct[n].p_val[p]);
          // inner_place
          if(strstr(ClimatCfgStruct.places[placeindex], ClimatCfgStruct.inner_place) != NULL)
          {
            if(ParamStruct[n].p_val[p] < ClimatCfgStruct.inner_hum_low)
            {
              printf("[LOW] ");
              //low_hum_action();
            }
            if(ParamStruct[n].p_val[p] > ClimatCfgStruct.inner_hum_high)
            {
              printf("[HIGH] ");
              //high_hum_action();
            }
            if(ParamStruct[n].p_val[p] < ClimatCfgStruct.inner_hum_high && ParamStruct[n].p_val[p] > ClimatCfgStruct.inner_hum_low)
            {
              printf("[NORMAL] ");
              // no action required
            }
          }
          // outer_place
/*          if(strstr(ClimatCfgStruct.places[placeindex], ClimatCfgStruct.outer_place) != NULL)
          {
            if(ParamStruct[n].p_val[p] < ClimatCfgStruct.outer_hum_low)
            {
              printf("[LOW] ");
            }
            if(ParamStruct[n].p_val[p] > ClimatCfgStruct.outer_temp_high)
            {
              printf("[HIGH] ");
            }
            if(ParamStruct[n].p_val[p] < ClimatCfgStruct.outer_temp_high && ParamStruct[n].p_val[p] > ClimatCfgStruct.outer_temp_low)
            {
              printf("[NORMAL] ");
            }
          }*/
          printf("\n");
        }
      }
    }
  }
}
void m_climat_co2(int placeindex, _ParamStruct *ParamStruct, int SensorNum)
{
  int n, p;

  //printf("\t=== Analyzing CO2 concentration ===\n");
  for(n = 0; n < SensorNum; n++)
  {
    if(strstr(ParamStruct[n].place, ClimatCfgStruct.places[placeindex]) != NULL)
    {
      for(p = 0; p < ParamStruct[n].pnum; p++)
      {
        if(strstr(ParamStruct[n].p_name[p], "Содержание CO2") != NULL)
        {
          printf("\t%s [%.1f] ", ParamStruct[n].p_name[p], ParamStruct[n].p_val[p]);
          // inner_place
          if(strstr(ClimatCfgStruct.places[placeindex], ClimatCfgStruct.inner_place) != NULL)
          {
            if(ParamStruct[n].p_val[p] < ClimatCfgStruct.inner_co2_low)
            {
              printf("[LOW] ");
              //low_co2_action();
            }
            if(ParamStruct[n].p_val[p] > ClimatCfgStruct.inner_co2_high)
            {
              printf("[HIGH] ");
              //high_co2_action();
            }
            if(ParamStruct[n].p_val[p] < ClimatCfgStruct.inner_co2_high && ParamStruct[n].p_val[p] > ClimatCfgStruct.inner_co2_low)
            {
              printf("[NORMAL] ");
              // no action required
            }
          }
          // outer_place
/*          if(strstr(ClimatCfgStruct.places[placeindex], ClimatCfgStruct.outer_place) != NULL)
          {
            if(ParamStruct[n].p_val[p] < ClimatCfgStruct.outer_temp_low)
            {
              printf("[LOW] ");
            }
            if(ParamStruct[n].p_val[p] > ClimatCfgStruct.outer_temp_high)
            {
              printf("[HIGH] ");
            }
            if(ParamStruct[n].p_val[p] < ClimatCfgStruct.outer_temp_high && ParamStruct[n].p_val[p] > ClimatCfgStruct.outer_temp_low)
            {
              printf("[NORMAL] ");
            }
          }*/
          printf("\n");
        }
      }
    }
  }
}

void ClimatModuleCfg(void)
{
  FILE *mcfg;
  char str[1024], pname[255], pval[255], strtmp[64];
  int i, idxp, idxo;

  printf("  Climatic module...");
  mcfg = fopen("m_climat.cfg", "r");
  if(mcfg != 0)
  {
    printf("\t\t[discovered] ");
    while(fgets(&str[0], 1023, mcfg) != NULL)
    {
      if(str[0] != '#')
      {
/*#ifdef _DEBUG_MODE
        printf("%s", str);
#endif*/
        sscanf(str, "%[^'=']=%s", &pname, &pval);
       // printf("%s_%s\n", pname, pval);
        if(strstr(pname, "enabled") != NULL)
        {
          ClimatCfgStruct.enabled = (uint8_t) atoi(pval);
          if(ClimatCfgStruct.enabled == 1) printf("[enabled]\n");
          else printf("[disabled]\n");
#ifdef _DEBUG_MODE
          printf("[enabled] = [%d]\n", ClimatCfgStruct.enabled);
#endif
        }
//===== s_interval ==========
        if(strstr(pname, "s_interval") != NULL)
        {
          idxp = 0;
          idxo = 0;
          for(i = 0; i < strlen(pval); i++)
          {
            if(pval[i] == ',')
            {
              strncpy(strtmp, (pval + idxo), (i - idxo));
              ClimatCfgStruct.s_interval[idxp] = atoi(strtmp);
#ifdef _DEBUG_MODE
              printf("%d:[%02d] ", idxp, ClimatCfgStruct.s_interval[idxp]);
#endif
              idxo = i + 1;
              idxp++;
            }
          }
          for(i = idxp; i < MAX_NUM_INTERVALS; i++)
          {
            ClimatCfgStruct.s_interval[i] = -1;
#ifdef _DEBUG_MODE
            printf("%d:[%02d] ", i, ClimatCfgStruct.s_interval[i]);
            if(i == (MAX_NUM_INTERVALS - 1)) printf("\n");
#endif
          }
        }
//===== m_interval ==========
        if(strstr(pname, "m_interval") != NULL)
        {
          if(pval[0] == '*')
          {
            for(i = 0; i < MAX_NUM_INTERVALS; i++)
            {
              ClimatCfgStruct.m_interval[i] = -2;
#ifdef _DEBUG_MODE
              printf("%d:[%02d] ", i, ClimatCfgStruct.m_interval[i]);
              if(i == (MAX_NUM_INTERVALS - 1)) printf("\n");
#endif
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
                ClimatCfgStruct.m_interval[idxp] = atoi(strtmp);
#ifdef _DEBUG_MODE
                printf("%d:[%02d] ", idxp, ClimatCfgStruct.m_interval[idxp]);
#endif
                idxo = i + 1;
                idxp++;
              }
            }
            for(i = idxp; i < MAX_NUM_INTERVALS; i++)
            {
              ClimatCfgStruct.m_interval[i] = -1;
#ifdef _DEBUG_MODE
              printf("%d:[%02d] ", i, ClimatCfgStruct.m_interval[i]);
              if(i == (MAX_NUM_INTERVALS - 1)) printf("\n");
#endif
            }
          }
        }
//===== places ==========
        if(strstr(pname, "places") != NULL)
        {
          idxp = 0;
          idxo = 0;
          for(i = 0; i < strlen(pval); i++)
          {
            if(pval[i] == ',')
            {
              //strncpy(strtmp, (pval + idxo), (i - idxo));
              if(idxp < MAX_NUM_PLACES) strncpy(ClimatCfgStruct.places[idxp], (pval + idxo), (i - idxo));
              else strcpy(ClimatCfgStruct.places[idxp], "null");
#ifdef _DEBUG_MODE
              printf("%d:[%s] ", idxp, ClimatCfgStruct.places[idxp]);
#endif
              idxo = i + 1;
              idxp++;
            }
          }
          ClimatCfgStruct.iterations = idxp;
          for(i = idxp; i < MAX_NUM_PLACES; i++)
          {
            strcpy(ClimatCfgStruct.places[i], "null");
#ifdef _DEBUG_MODE
            printf("%d:[%s] ", i, ClimatCfgStruct.places[i]);
            if(i == (MAX_NUM_PLACES - 1)) printf("\n");
#endif
          }
#ifdef _DEBUG_MODE
          printf("[iterations] = [%d]\n", ClimatCfgStruct.iterations);
#endif
        }
//===== inner_place ==========
        if(strstr(pname, "inner_place") != NULL)
        {
          strncpy(ClimatCfgStruct.inner_place, pval, strlen(pval)-1);
#ifdef _DEBUG_MODE
          printf("[inner_place] = [%s]\n", ClimatCfgStruct.inner_place);
#endif
        }
//===== inner_temp_high ==========
        if(strstr(pname, "inner_temp_high") != NULL)
        {
          ClimatCfgStruct.inner_temp_high = atof(pval);
#ifdef _DEBUG_MODE
          printf("[inner_temp_high] = [%.1f]\n", ClimatCfgStruct.inner_temp_high);
#endif
        }
//===== inner_temp_low ==========
        if(strstr(pname, "inner_temp_low") != NULL)
        {
          ClimatCfgStruct.inner_temp_low = atof(pval);
#ifdef _DEBUG_MODE
          printf("[inner_temp_low] = [%.1f]\n", ClimatCfgStruct.inner_temp_low);
#endif
        }
//===== inner_hum_high ==========
        if(strstr(pname, "inner_hum_high") != NULL)
        {
          ClimatCfgStruct.inner_hum_high = atof(pval);
#ifdef _DEBUG_MODE
          printf("[inner_hum_high] = [%.1f]\n", ClimatCfgStruct.inner_hum_high);
#endif
        }
//===== inner_hum_low ==========
        if(strstr(pname, "inner_hum_low") != NULL)
        {
          ClimatCfgStruct.inner_hum_low = atof(pval);
#ifdef _DEBUG_MODE
          printf("[inner_hum_low] = [%.1f]\n", ClimatCfgStruct.inner_hum_low);
#endif
        }
//===== inner_co2_high ==========
        if(strstr(pname, "inner_co2_high") != NULL)
        {
          ClimatCfgStruct.inner_co2_high = atof(pval);
#ifdef _DEBUG_MODE
          printf("[inner_co2_high] = [%.1f]\n", ClimatCfgStruct.inner_co2_high);
#endif
        }
//===== inner_co2_low ==========
        if(strstr(pname, "inner_co2_low") != NULL)
        {
          ClimatCfgStruct.inner_co2_low = atof(pval);
#ifdef _DEBUG_MODE
          printf("[inner_co2_low] = [%.1f]\n", ClimatCfgStruct.inner_co2_low);
#endif
        }
//===== outer_place ==========
        if(strstr(pname, "outer_place") != NULL)
        {
          strncpy(ClimatCfgStruct.outer_place, pval, strlen(pval)-1);
#ifdef _DEBUG_MODE
          printf("[outer_place] = [%s]\n", ClimatCfgStruct.outer_place);
#endif
        }
//===== outer_temp_high ==========
        if(strstr(pname, "outer_temp_high") != NULL)
        {
          ClimatCfgStruct.outer_temp_high = atof(pval);
#ifdef _DEBUG_MODE
          printf("[outer_temp_high] = [%.1f]\n", ClimatCfgStruct.outer_temp_high);
#endif
        }
//===== outer_temp_low ==========
        if(strstr(pname, "outer_temp_low") != NULL)
        {
          ClimatCfgStruct.outer_temp_low = atof(pval);
#ifdef _DEBUG_MODE
          printf("[outer_temp_low] = [%.1f]\n", ClimatCfgStruct.outer_temp_low);
#endif
        }

      }
    }
    fclose(mcfg);
  }
  else
  {
    printf("\t[not found]\n");
  }
}

