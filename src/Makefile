AFLAGS = -Wall -O2
DFLAGS = -Wall -g -DDEBUG -DPLAZA_DEBUGON

all: CFLAGS = $(AFLAGS)
all: ../plaza

debug: CFLAGS = $(DFLAGS)
debug: ../plaza

clean:
	rm -f *.o 2>/dev/null

../plaza: globals.o io.o gui.o main.c
	gcc -lncursesw $(CFLAGS) -o ../plaza unicode.o globals.o utils.o message.o io.o gui.o main.c

globals.o: globals.h globals.c
	gcc -c $(CFLAGS) -o globals.o globals.c

gui.o: gui.h unicode.o gui.c utils.o message.o settings.h
	gcc -c $(CFLAGS) -o gui.o gui.c

io.o: io.c io.h settings.h message.o
	gcc -c $(CFLAGS) -o io.o io.c

message.o: message.h message.c
	gcc -c $(CFLAGS) -o message.o message.c

utils.o: utils.c utils.h settings.h
	gcc -c $(CFLAGS) -o utils.o utils.c

unicode.o: unicode.h unicode.c utils.o
	gcc -c $(CFLAGS) -o unicode.o unicode.c
