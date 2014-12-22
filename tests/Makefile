all: unicode chtowide

unicode: ../src/unicode.c ../src/unicode.h ./unicode.c
	gcc -Wall -o ../src/unicode.o -c ../src/unicode.c
	gcc -Wall -lncursesw -o unicode ../src/unicode.o unicode.c

chtowide: ./chtowide.c
	gcc -Wall -o chtowide chtowide.c
