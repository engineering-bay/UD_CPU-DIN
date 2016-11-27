#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "smarthome.h"

extern struct _ParamStruct param[SENSORS_COUNT];
unsigned char param_index[]={0, 1, 2, 3};

void GetSysData(void);

//=== Wake up PC thru Wake-On-LAN magic packet
void PC_Wakeup(void)
{
    FILE *sys;
    time_t ta;
    char filename[20];
    struct tm *tm_ptr;
    (void) time(&ta);
    tm_ptr = localtime(&ta);
    sys = fopen("./logs/system.log", "a");
    fprintf(sys, "[%04d-%02d-%02d] %02d:%02d wday = [%d] PC wakeup initiated!\n", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_wday);
    fclose(sys);
    system("wakeonlan -p 4848 10:c3:7b:6c:c7:0b");
}
//=== Log SmartHome system parameters in file
void param_logger(void)
{
	FILE *log;
	time_t ta;
	int i;
	char filename[20];
	char str[255] = "", pname[100] = "", request[] = "";
	struct tm *tm_ptr;
	(void) time(&ta);
	tm_ptr = localtime(&ta);
	param[0].ts = ta;
	GetSysData();
	sprintf(filename, "./logs/%04d-%02d-%02d.log", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday);
	log = fopen(filename, "a");
	fprintf(log, "%02d:%02d", tm_ptr->tm_hour, tm_ptr->tm_min);
	//printf("index_size: [%d]\n", sizeof(param_index));
	sprintf(str, "placeid=2&setdata=%d", sizeof(param_index));
	for(i = 0; i < sizeof(param_index); i++)
	{
		//fprintf(log, " %5.1f", param_acc[i]/acc_index[i]);
		fprintf(log, " %5.1f", param[param_index[i]].p1);
		//param_acc[param_index[i]] = 0;
		//acc_index[param_index[i]] = 0;
		sprintf(pname, "&param%d=%02x %.3f %.3f %.3f %d %.1f", i, param_index[i], param[param_index[i]].p1, param[param_index[i]].p2,param[param_index[i]].vbat, param[param_index[i]].ts, param[param_index[i]].rssi);
		//printf("%d %d ts:%f\n", i, param_index[i], param[param_index[i]][2]);
		strcat(str, pname);
	}
	fprintf(log, "\n");
	fclose(log);
	printf("[HANDLER] Parameter logger executed!\n");
	//printf("%s\n", str);
	strcat(request, "curl --connect-timeout 2 -d \"");
	strcat(request, str);
	//strcat(request, "\" http:\/\/192.168.0.100/postd.php");
	strcat(request, "\" http:\/\/engineering-bay.site40.net/postd.php");
	//strcat(request, "\" http:\/\/engineering-bay.zz.mu/postd.php");
	//printf("%s\n", request);
	//system("curl --connect-timeout 2 -d \"setdata=1&param0=03 -10.6 2.49 0012\" http:\/\/192.168.0.100/post.php");
	system(request);
}
//=== Get water meter values and print a bill
void Bill_print(void)
{
	FILE *sys;
	time_t ta;
	char filename[20];
	struct tm *tm_ptr;
	(void) time(&ta);
	tm_ptr = localtime(&ta);
	sys = fopen("./logs/system.log", "a");
	fprintf(sys, "[%04d-%02d-%02d] %02d:%02d WaterBill print executed!\n", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min);
	fclose(sys);
	printf("[HANDLER] Bill print executed!\n");
}
//=== Play sound
void Budilnik(void)
{
	FILE *sys;
	time_t ta;
	char filename[20];
	struct tm *tm_ptr;
	(void) time(&ta);
	tm_ptr = localtime(&ta);
	sys = fopen("./logs/system.log", "a");
	fprintf(sys, "[%04d-%02d-%02d] %02d:%02d Budilnik executed!\n", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min);
	fclose(sys);
	printf("[HANDLER] Budilnik executed!\n");
}
//=== Do nothing. For debug only!
void battery_logger(void)
{
	FILE *sys;
	time_t ta;
	char filename[20];
	struct tm *tm_ptr;
	(void) time(&ta);
	tm_ptr = localtime(&ta);
	sys = fopen("./logs/system.log", "a");
	//bat = fopen("./logs/battery.log", "a");
	fprintf(sys, "===== [%04d-%02d-%02d] ===============================================================\n", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday);
	fprintf(sys, "[%04d-%02d-%02d] %02d:%02d Battery logger executed!\n", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min);
	//fprintf(bat, "%.2f\n", param[3][3]);
	fclose(sys);
	//fclose(bat);
	printf("[HANDLER] Battery logger executed!\n");
}

void execute_handler(int num)
{
	switch(num)
	{
		case 1: param_logger(); break;
		case 2: PC_Wakeup(); break;
		case 3: Bill_print(); break;
		case 4: Bill_print(); break;
		case 5: battery_logger(); break;
		case 6: Budilnik(); break;
	}
}

void GetSysData(void)
{
  FILE *tmp;
  int i;
  char str[100], s1[10];
  system("df -h | grep rootfs > file.tmp");
  tmp = fopen("file.tmp", "r");
  fgets(str, 100, tmp);
  fclose(tmp);
  memset(s1, 0, 10);
  strncpy(s1, str + 52, 3);
  param[0].p1 = (float)atoi(s1);
  system("rm -f file.tmp");
  //printf("hdd usage [%.1f]\% \n", hdd_usage);
}