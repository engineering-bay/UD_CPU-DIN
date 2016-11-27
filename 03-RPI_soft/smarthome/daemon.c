#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

extern void main_home(void);

int main(int argc, char* argv[])
{
	pid_t parpid;

	if (argc < 2)
	{
		printf("Usage ./daemon -d for daemon or ./daemon -i for interactive\n");
		exit(1);
	}
	if (strcmp(argv[1],"-i")==0) main_home();
	else if (strcmp(argv[1],"-d")==0)
	{
		if((parpid=fork())<0) 
		{ 
			printf("ncan't fork");
			exit(1); 
		}
		else if (parpid!=0)
		exit(0); 
		setsid(); 
		main_home();
	}
	else
	{
		printf("Usage ./daemon -d for daemon or ./daemon -i for interactive\n");
		exit(1);
	} 
	
	return 0;
}