#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include "smarthome.h"

#define SENSOR_DATA_LENGTH	24
#define SENSOR_MSG_LENGTH	36
#define XBEE_SYST_LENGTH	15
#define XBEE_ADDR_OFFSET	4

extern struct _ParamStruct param[SENSORS_COUNT];
//extern float param_acc[SENSORS_COUNT];
//extern int acc_index[SENSORS_COUNT];

FILE *input;
int rx_index = 0, FrameStartFlag = 0, FrameLength = 100;

uint8_t crc_check(uint8_t *ptr, int len, unsigned char chksum);
uint8_t crc_calc(uint8_t *ptr, int len);
float xbee_get_rssi(void);
void xbee_my_addr_request(void);

void *usb_getdata(void * arg)
{
int stat, adc, i, z;
int sid, dindex, ts;
float temp = 0;
char  p_val[100];
time_t ta;
struct tm *tn;
struct termios init_set, new_set;
input = fopen("/dev/ttyAMA0", "r+b");

for(i=0;i<24;i++) p_val[i] = 0;

if(!input)
{
  printf("Unable to open serial device!\n");
  exit(EXIT_FAILURE);
}
else
{
  printf("[RF thread] Serial device opened successfully!\n");
  tcgetattr(fileno(input),&init_set);
  new_set = init_set;
  new_set.c_iflag |= IGNBRK;
  new_set.c_iflag |= IGNPAR;
  new_set.c_iflag |= IGNCR;
  new_set.c_oflag &= ~OPOST;
  new_set.c_cflag |= CLOCAL;
  new_set.c_cflag |= CREAD;
  new_set.c_cflag |= CS8;
  new_set.c_cflag &= ~CSIZE;
  new_set.c_cflag &= ~CSTOPB;
  new_set.c_cflag &= ~PARENB;
  new_set.c_cflag &= ~HUPCL;
  new_set.c_lflag &= ~ECHO;
  new_set.c_lflag &= ~ICANON;
  cfmakeraw(&new_set);
  new_set.c_cc[VMIN] = 1;
  new_set.c_cc[VTIME] = 2;
  cfsetispeed(&new_set, B9600);
  cfsetospeed(&new_set, B9600);
  stat = tcsetattr(fileno(input), TCSANOW, &new_set);
  if(stat !=0)
  {
    printf("ERROR: could not set attributes\n");
  }
  else
  {
    //printf("TTY attributes sucessfully set\n");
  }
  //xbee_my_addr_request();
  while(1)
  {
//== 0x01 ===============================================================================
  i = fread((void*)p_val + rx_index, 1, 1, input);
  //printf("%02X ", p_val[rx_index]);
  //(void) time(&ta);
  //tn = localtime(&ta);
  if(p_val[rx_index] == 0x7E && FrameStartFlag == 0) FrameStartFlag = 1;
  if(rx_index == 2)
  {
    FrameLength = (p_val[1] * 256 + p_val[2]);
    if(FrameLength == 0)
    {
      FrameStartFlag = 0;
      rx_index = 0;
    }
    /*else
    {
      printf("\nFrame length [%d]\n", FrameLength);
    }*/
  }
  if(FrameStartFlag == 1)
  {
    if(rx_index >= FrameLength + 3)
    {
       printf("\n[ Frame captured ]\n");
       /*printf("Sender address: %02X%02X%02X%02X %02X%02X%02X%02X\n", p_val[XBEE_ADDR_OFFSET+0], p_val[XBEE_ADDR_OFFSET+1], p_val[XBEE_ADDR_OFFSET+2], p_val[XBEE_ADDR_OFFSET+3],
                                                                   p_val[XBEE_ADDR_OFFSET+4], p_val[XBEE_ADDR_OFFSET+5], p_val[XBEE_ADDR_OFFSET+6], p_val[XBEE_ADDR_OFFSET+7]);*/
       if(crc_check((uint8_t *)(p_val + 3), FrameLength, p_val[rx_index]) == 1)
       {
         if(p_val[3] == 0x90)
         {
           if(FrameLength == SENSOR_MSG_LENGTH)
           {
             //printf("[Data frame received]\n");
             //temp = xbee_get_rssi();
             sid = (int) p_val[XBEE_SYST_LENGTH];
             if(sid < 16)
             {
               //ts = (int)time(NULL);
               //tn = localtime(&ta);
	       //parameter #1
               *((char*)(&temp)+0) = p_val[XBEE_SYST_LENGTH + 4];
               *((char*)(&temp)+1) = p_val[XBEE_SYST_LENGTH + 5];
               *((char*)(&temp)+2) = p_val[XBEE_SYST_LENGTH + 6];
               *((char*)(&temp)+3) = p_val[XBEE_SYST_LENGTH + 7];
               param[sid].p1 = temp;
               //parameter #2
               *((char*)(&temp)+0) = p_val[XBEE_SYST_LENGTH + 9];
               *((char*)(&temp)+1) = p_val[XBEE_SYST_LENGTH + 10];
               *((char*)(&temp)+2) = p_val[XBEE_SYST_LENGTH + 11];
               *((char*)(&temp)+3) = p_val[XBEE_SYST_LENGTH + 12];
               param[sid].p2 = temp;
               //timestamp
               //param[sid][2] = (float) (tn->tm_hour * 100 + tn->tm_min);
               param[sid].ts = (int) time(NULL);
               //parameter #4
               *((char*)(&temp)+0) = p_val[XBEE_SYST_LENGTH + 19];
               *((char*)(&temp)+1) = p_val[XBEE_SYST_LENGTH + 20];
               *((char*)(&temp)+2) = p_val[XBEE_SYST_LENGTH + 21];
               *((char*)(&temp)+3) = p_val[XBEE_SYST_LENGTH + 22];
               param[sid].vbat = temp;
               param[sid].rssi = xbee_get_rssi();
               printf("\nNode [%2d] - [1]: %7.3f [2]: %7.3f [TS]: %d [Vbat]: %5.2f [RSSI]: %.1f\n", sid,
		 								  param[sid].p1,
										  param[sid].p2,
										  param[sid].ts,
	   									  param[sid].vbat,
										  param[sid].rssi);
               //printf("RSSI = [%.1f]\n", xbee_get_rssi());
             }
             else printf("[Broken frame] Invalid sensor ID!\n");
           }
           else printf("[Broken frame] Invalid frame length!\n");
         }
         else printf("[Unknown API frame]\n");
       }
       FrameStartFlag = 0;
       rx_index = 0;
       FrameLength = 100;
    }
    else rx_index++;
  }
  if(rx_index >= 100) rx_index = 0;
  }
  fclose(input);
}
}

uint8_t crc_check(uint8_t *ptr, int len, unsigned char chksum)
{
  int i;
  uint8_t sum;

  for(i = 0; i <= len; i++)
  {
    sum += ptr[i];
  }
  if(sum == 0xFF)
  {
    //printf("Checksum [%02X] is correct!\n", chksum);
    return 1;
  }
  else
  {
//    printf("Checksum [%02X] is INCORRECT! Sum = [%02X]\n", chksum, sum);
//    printf("Calculated checksum is [%02X]\n", crc_calc(ptr, len));
    return 0;
  }
}

uint8_t crc_calc(uint8_t *ptr, int len)
{
  uint8_t sum = 0;
  int i;

  for(i = 0; i < len; i++)
  {
    sum += ptr[i];
  }
  return (0xFF - sum);
}

float xbee_get_rssi(void)
{
  int i;
  unsigned char req[20], resp[20];
  float rssi = -200.0;

  req[0] = 0x7E; // delimiter
  req[1] = 0x00; // length MSB
  req[2] = 0x04; // length LSB
  req[3] = 0x08; // API identifier "AT command"
  req[4] = 0x52; // frame ID 'R' means read
  req[5] = 'D';  // AT-command
  req[6] = 'B';  // AT-command
  req[7] = crc_calc((uint8_t *) (req + 3), 4); // checksum

  fwrite(req, 8, 1, input);
  i = fread(resp, 10, 1, input);
  if(resp[3] == 0x88)
  {
    //printf("[AT reesponse frame received]\n");
    if(resp[5] == 'D' && resp[6] == 'B')
    {
      rssi = (float) ((int)resp[8] * (-1));
    }
  }

  return rssi;
}

void xbee_my_addr_request(void)
{
  int i;
  uint8_t req[20], resp[20];
  req[0] = 0x7E; // delimiter
  req[1] = 0x00; // length MSB
  req[2] = 0x04; // length LSB
  req[3] = 0x08; // API identifier "AT command"
  req[4] = 0x53; // frame ID
  req[5] = 'D';  // AT-command
  req[6] = 'H';  // AT-command
  req[7] = crc_calc((uint8_t *) (req + 3), 4); // checksum

  fwrite(req, 8, 1, input);
  i = fread(resp, 13, 1, input);
  if(resp[4] == 0x53 && resp[5] == 'D' && resp[6] == 'H')
  {
    printf("Address High: [%02X%02X%02X%02X]\n", resp[7], resp[8], resp[9], resp[10]);
  }
  req[6] = 'L'; // low address word needed
  req[7] = crc_calc((uint8_t *) (req + 3), 4); // recalculate checksum

  fwrite(req, 8, 1, input);
  i = fread(resp, 13, 1, input);
  if(resp[4] == 0x53 && resp[5] == 'D' && resp[6] == 'L')
  {
    printf("Address Low : [%02X%02X%02X%02X]\n", resp[7], resp[8], resp[9], resp[10]);
  }
  // 16-bit network address request AT-command
  req[5] = 'M';
  req[6] = 'Y';
  req[7] = crc_calc((uint8_t *) (req + 3), 4); // recalculate checksum

  fwrite(req, 8, 1, input);
  i = fread(resp, 11, 1, input);
  if(resp[4] == req[4] && resp[5] == req[5] && resp[6] == req[6])
  {
    printf("Network Addr : [%02X%02X]\n", resp[7], resp[8]);
  }
}
