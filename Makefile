all: minesweeper
minesweeper: conio.o minesweeper.o
	gcc conio.o minesweeper.o -o minesweeper
conio.o: conio.c conio.h
	gcc -c conio.c
minesweeper.o: minesweeper.c conio.h
	gcc -c minesweeper.c
.PHONY: all clean
clean:
	rm conio.o minesweeper.o minesweeper
