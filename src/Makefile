all: ../plaza

clean:
	rm -f *.o 2>/dev/null

../plaza: globals.o io.o gui.o main.c
	gcc -lncursesw -Wall -O2 -o ../plaza globals.o utils.o message.o io.o gui.o main.c

globals.o: globals.h globals.c
	gcc -c -Wall -O2 -o globals.o globals.c

gui.o: gui.h gui.c utils.o message.o settings.h
	gcc -c -Wall -O2 -o gui.o gui.c

io.o: io.c io.h settings.h message.o
	gcc -c -Wall -O2 -o io.o io.c

message.o: message.h message.c
	gcc -c -Wall -O2 -o message.o message.c

utils.o: utils.c utils.h settings.h
	gcc -c -Wall -O2 -o utils.o utils.c
