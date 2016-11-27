
smarthome: mainhome.o uart.o lan_s.o timer.o handlers.o demon.o
	gcc -o smarthome mainhome.o uart.o lan_s.o timer.o handlers.o demon.o  -l pthread
	-rm *.o

mainhome.o: mainhome.c
	gcc -c mainhome.c

uart.o: uart.c
	gcc -c uart.c

lan_s.o: lan_s.c
	gcc -c lan_s.c

timer.o: timer.c
	gcc -c timer.c

handlers.o: handlers.c
	gcc -c handlers.c

demon.o: daemon.c
	gcc -c daemon.c -o demon.o