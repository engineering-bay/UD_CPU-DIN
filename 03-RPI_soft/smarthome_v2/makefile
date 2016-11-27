smarthome: cfg_parser.o queue.o engine.o events.o m_climat.o m_registrator.o
	gcc smarthome.c cfg_parser.o queue.o engine.o events.o m_climat.o m_registrator.o -pthread -o smarthome
	rm -f *.o

cfg_parser: cfg_parser.c cfg_parser.h
	gcc -c cfg_parser.c -o cfg_parser.o

queue: queue.c queue.h
	gcc -c queue.c -o queue.o

engine: engine.c engine.h
	gcc -c engine.c -o engine.o

events: events.c events.h
	gcc -c events.c -o events.o

m_climat: m_climat.c m_climat.h
	gcc -c m_climat.c -o m_climat.h

m_registrator: m_registrator.c m_registrator.h
	gcc -c m_registrator.c -o m_registrator.o

clean:
	rm -f *.o
	rm -f *.out
	rm -f smarthome
	rm -f *.c~
	rm -f *.h~
	rm -f *.cfg~

