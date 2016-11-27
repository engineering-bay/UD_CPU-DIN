#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

extern void execute_handler(int);

char sstr_dest[5];

char *substr(char *source, int start, int length)
{
	int i, id=0;
	//char dest[5];
	for(i = start; i < (start+length); i++)
	{
		sstr_dest[id] = source[i];
		id++;
	}
	for(i = id; i < 5; i++)
	{
		sstr_dest[i] = 0;
	}
	return sstr_dest;
}

void *t_logger  (void *arg)
{
  int i, exec_now = 1, num;
  FILE *shed, *sys;
  time_t ta;
  char filename[30], str[254], *sstr;
  struct tm *tn;
  //float T1_l[120], T2_l[120], T1_o = 0, T2_o = 0;
  printf("[Timer thread] Logger started!\n");
  while(1)
  {
    (void) time(&ta);
    tn = localtime(&ta);
    if(tn->tm_sec == 0) // every minute
    {
		// ==== Shedule file parser ================================================================
		// =========================================================================================
		shed = fopen("shedule.rul", "r");
		while(!feof(shed))
		{
			exec_now = 1;
			if(fgets(str, 254, shed))
			{
			sstr = substr(str, 0, 2);
			if(sstr[0] == '#' && sstr[1] == '#')
			{
				//printf("[IGNORED] Comments detected.\n");
				//do nothing - ignore shedule file comments
			}
			else
			{
				num = atoi(sstr);
				sstr = substr(str, 3, 4); // get task shedule time: year
				if(sstr[0]=='X'&&sstr[1]=='X'&&sstr[2]=='X'&&sstr[3]=='X') exec_now = exec_now * 1;
				else
				{
					if(atoi(sstr) == tn->tm_year+1900) exec_now = exec_now * 1;
					else exec_now = exec_now * 0;
				}
				sstr = substr(str, 8, 2); // get task shedule time: month
				if(sstr[0]=='X'&&sstr[1]=='X') exec_now = exec_now * 1;
				else
				{
					if(atoi(sstr) == tn->tm_mon+1) exec_now = exec_now * 1;
					else exec_now = exec_now * 0;
				}
				sstr = substr(str, 11, 2); // get task shedule time: day of month
				if(sstr[0]=='X'&&sstr[1]=='X') exec_now = exec_now * 1;
				else
				{
					if(atoi(sstr) == tn->tm_mday) exec_now = exec_now * 1;
					else exec_now = exec_now * 0;
				}
				sstr = substr(str, 14, 1); // get task shedule time: day of week
				if(atoi(sstr) == 7) exec_now = exec_now * 1;
				else
				{
					if(atoi(sstr) == 8 && (tn->tm_wday <= 5 && tn->tm_wday > 0)) exec_now = exec_now * 1;
					else
					{
						if(atoi(sstr) == 9 && (tn->tm_wday == 6 || tn->tm_wday == 0)) exec_now = exec_now * 1;
						else
						{
							if(atoi(sstr) == tn->tm_wday) exec_now = exec_now * 1;
							else exec_now = exec_now * 0;
						}
					}
				}
				sstr = substr(str, 16, 2); // get task shedule time: hour
				if(sstr[0]=='X'&&sstr[1]=='X') exec_now = exec_now * 1;
				else
				{
					if(atoi(sstr) == tn->tm_hour) exec_now = exec_now * 1;
					else exec_now = exec_now * 0;
				}
				sstr = substr(str, 19, 2); // get task shedule time: minute
				if(sstr[0]=='X'&&sstr[1]=='X') exec_now = exec_now * 1;
				else
				{
					if(atoi(sstr) == tn->tm_min) exec_now = exec_now * 1;
					else exec_now = exec_now * 0;
				}
		// if task shedule date&time match current date&time
				if (exec_now == 1)
				{
					execute_handler(num);
					printf("Task #%02d named TASKNAME executed!\n", num);
					/*if(num > 1)
					{
						sys = fopen("./logs/system.log", "a");
						fprintf(sys, "%04d-%02d-%02d [%02d:%02d] Task #%02d executed!\n", tn->tm_year+1900, tn->tm_mon+1, tn->tm_mday, tn->tm_hour, tn->tm_min, num);
						fclose(sys);
					}*/
				}
				else
				{
					//printf("Task #%02d named TASKNAME ignored!\n", num);
				}
			}
		}
		}
	fclose(shed);
	usleep(500000);
    }
    usleep(600000);
  } // end of while(1)
} // end of main()
