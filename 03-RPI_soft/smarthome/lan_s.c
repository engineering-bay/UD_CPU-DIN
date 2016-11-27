#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "smarthome.h"

extern struct _ParamStruct param[SENSORS_COUNT];

void *socket_handler(void * arg)
{
    int sock, listener;
    struct sockaddr_in addr;
    char buf[255];
    int bytes_write, i;

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0)
    {
        perror("socket");
        //exit(1);
    }
    printf("[TCP SERVER] Socket created sucessfully!\n");
    printf("sizeof param = %d\n", sizeof(param));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        //exit(2);
    }

    listen(listener, 1);

    while(1)
    {
        sock = accept(listener, NULL, NULL);
        if(sock < 0)
        {
            perror("accept");
            //exit(3);
        }
        printf("[TCP SERVER] Connection accepted!\n");

        buf[0]=0;

        /*for(i = 3; i < 7; i++)
        {
          sprintf(buf+strlen(buf), "[%2d]=%.3f %.3f %.0f %.2f:", i, param[i][0], param[i][1], param[i][2], param[i][3]);
        }*/
        bytes_write = send(sock, (void *) param, sizeof(param), 0);
        if(bytes_write < 0)
        {
          printf("[TCP SERVER] Send failed!\n");
        }
        else
        {
          printf("[TCP SERVER] %d bytes sent successfully!\n", bytes_write);
        }

        close(sock);
        printf("[TCP SERVER] Client socket closed!\n");
    }
    printf("[TCP SERVER] Exit!\n");
    //return 0;
}
