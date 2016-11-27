#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void main(void)
{
  float hdd_usage = 0.0;
  FILE *tmp;
  int i;
  char str[100], s1[10];
  system("df -h | grep rootfs > file.tmp");
  tmp = fopen("file.tmp", "r");
  fgets(str, 100, tmp);
  fclose(tmp);
  memset(s1, 0, 10);
  strncpy(s1, str + 52, 3);
  hdd_usage = (float)atoi(s1);
  system("rm -f file.tmp");
  printf("hdd usage [%.1f]\% \n", hdd_usage);
}
